////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2015 Putta Khunchalee
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ipx.h"
#include "netadapter.h"
#include "config.h"

#include <yuri_extension.h>

#include <hooking/iat.h>

#include <utilities/module.h>
#include <utilities/path.h>

extern "C" int WSAAPI init(WORD wVersionRequested, LPWSADATA lpWSAData)
{
    std::uint8_t buffer[16384];
    PIP_ADAPTER_ADDRESSES adapters;
    ULONG bufsize;
    int res;
    HRESULT hr;

    // core initialization
    try {
        yuriext::init();
    } catch (std::exception&) {
        return WSASYSNOTREADY;
    }

    // load configurations
    hr = config.CreateInstance(__uuidof(Configurations));

    if (FAILED(hr)) {
        yuriext::term();
        return WSASYSNOTREADY;
    }

    // get network adaptors in the computer
    adapters = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buffer);
    bufsize = sizeof(buffer);

    res = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX |
        GAA_FLAG_INCLUDE_GATEWAYS, nullptr, adapters, &bufsize);

    switch (res) {
    case NO_ERROR:
        for (auto adapter = adapters; adapter; adapter = adapter->Next) {
            netadapter net;

            // get IPs of adaptor
            for (auto addr = adapter->FirstUnicastAddress; addr;
                addr = addr->Next) {
                net.add_addr(netaddress(addr->Address.lpSockaddr,
                    addr->Address.iSockaddrLength));
            }

            netadapters.push_back(std::move(net));
        }
        break;
    case ERROR_NO_DATA:
        break;
    default:
        config.Release();
        yuriext::term();
        return res;
    }

    // initialize winsock
    res = WSAStartup(MAKEWORD(2, 2), lpWSAData);

    if (!res) {
        lpWSAData->wVersion = wVersionRequested;
    } else {
        netadapters.clear();
        config.Release();
        yuriext::term();
    }

    return res;
}

static bool hook_wsock32(HMODULE target_module)
{
    // bind
    if (!hooking::hook_iat(target_module, "wsock32.dll",
        reinterpret_cast<const char *>(2),
        reinterpret_cast<hooking::function_ptr>(ipx_bind))) {
        return false;
    }

    // getsockopt
    if (!hooking::hook_iat(target_module, "wsock32.dll",
        reinterpret_cast<const char *>(7),
        reinterpret_cast<hooking::function_ptr>(ipx_getsockopt))) {
        return false;
    }

    // recvfrom
    if (!hooking::hook_iat(target_module, "wsock32.dll",
        reinterpret_cast<const char *>(17),
        reinterpret_cast<hooking::function_ptr>(ipx_recvfrom))) {
        return false;
    }

    // sendto
    if (!hooking::hook_iat(target_module, "wsock32.dll",
        reinterpret_cast<const char *>(20),
        reinterpret_cast<hooking::function_ptr>(ipx_sendto))) {
        return false;
    }

    // setsockopt
    if (!hooking::hook_iat(target_module, "wsock32.dll",
        reinterpret_cast<const char *>(21),
        reinterpret_cast<hooking::function_ptr>(ipx_setsockopt))) {
        return false;
    }

    // socket
    if (!hooking::hook_iat(target_module, "wsock32.dll",
        reinterpret_cast<const char *>(23),
        reinterpret_cast<hooking::function_ptr>(ipx_socket))) {
        return false;
    }

    // WSAStartup
    if (!hooking::hook_iat(target_module, "wsock32.dll",
        reinterpret_cast<const char *>(115),
        reinterpret_cast<hooking::function_ptr>(init))) {
        return false;
    }

    return true;
}

extern "C" BOOL APIENTRY DllMain(HMODULE Module, DWORD CallReason, LPVOID Reserved)
{
    UNREFERENCED_PARAMETER(Module);
    UNREFERENCED_PARAMETER(Reserved);

    switch (CallReason) {
    case DLL_PROCESS_ATTACH:
        try {
            auto modpath = util::get_module_filename(NULL);
            auto modname = util::path_filename(modpath);

            if (_wcsicmp(modname.c_str(), L"gamemd.exe") == 0) {
                if (!hook_wsock32(GetModuleHandleW(NULL))) {
                    return FALSE;
                }
            }
        } catch (std::exception&) {
            return FALSE;
        }
        break;
    }

    return TRUE;
}
