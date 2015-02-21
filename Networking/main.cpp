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
#include "netadapter.h"

extern "C" int WSAAPI init(WORD wVersionRequested, LPWSADATA lpWSAData)
{
    std::uint8_t buffer[16384];
    PIP_ADAPTER_ADDRESSES adapters;
    ULONG bufsize;
    int res;

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
        return res;
    }

    // initialize winsock
    res = WSAStartup(MAKEWORD(2, 2), lpWSAData);

    if (!res) {
        lpWSAData->wVersion = wVersionRequested;
    }

    return res;
}

extern "C" BOOL APIENTRY DllMain(HMODULE Module, DWORD CallReason, LPVOID Reserved)
{
    UNREFERENCED_PARAMETER(Module);
    UNREFERENCED_PARAMETER(CallReason);
    UNREFERENCED_PARAMETER(Reserved);

    return TRUE;
}
