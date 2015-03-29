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

#include <hooking/iat.h>

hooking::function_ptr hooking::hook_iat(const void *target_module,
    const char *target_import, const char *target_func, function_ptr hook)
{
    // get image headers
    auto baseaddr = reinterpret_cast<const std::uint8_t *>(target_module);
    auto doshdr = reinterpret_cast<const IMAGE_DOS_HEADER *>(baseaddr);

    if (doshdr->e_magic != IMAGE_DOS_SIGNATURE) {
        throw std::invalid_argument("Target module is not valid");
    }

    auto nthdr = reinterpret_cast<const IMAGE_NT_HEADERS *>(
        baseaddr + doshdr->e_lfanew);

    if (nthdr->Signature != IMAGE_NT_SIGNATURE) {
        throw std::invalid_argument("Target module is not valid");
    }

    auto opthdr = &nthdr->OptionalHeader;

    if (opthdr->NumberOfRvaAndSizes <= IMAGE_DIRECTORY_ENTRY_IMPORT) {
        throw std::invalid_argument(
            "Target module does not have import directory");
    }

    // search target import
    auto impentry = reinterpret_cast<const IMAGE_IMPORT_DESCRIPTOR *>(
        baseaddr +
        opthdr->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    for (; impentry->Name; impentry++) {
        auto impname = reinterpret_cast<const char *>(baseaddr +
            impentry->Name);

        if (_stricmp(impname, target_import)) {
            continue;
        }

        // search target function
        auto impfunc = reinterpret_cast<const IMAGE_THUNK_DATA *>(
            baseaddr + impentry->OriginalFirstThunk);
        auto iat = reinterpret_cast<const uintptr_t *>(
            baseaddr + impentry->FirstThunk);

        for (; impfunc->u1.Ordinal; impfunc++, iat++) {
            if (IMAGE_SNAP_BY_ORDINAL(impfunc->u1.Ordinal)) {
                auto target_ordinal = reinterpret_cast<uint16_t>(target_func);

                if (target_ordinal > 0xFFFF) {
                    continue;
                }

                if (IMAGE_ORDINAL(impfunc->u1.Ordinal) != target_ordinal) {
                    continue;
                }
            } else {
                if (reinterpret_cast<uint16_t>(target_func) <= 0xFFFF) {
                    continue;
                }

                auto funcname = reinterpret_cast<const IMAGE_IMPORT_BY_NAME *>(
                    baseaddr + impfunc->u1.AddressOfData);

                if (std::strcmp(funcname->Name, target_func)) {
                    continue;
                }
            }

            // install hook
            DWORD old_protect;

            if (!VirtualProtect(const_cast<uintptr_t *>(iat),
                sizeof(function_ptr), PAGE_EXECUTE_READWRITE, &old_protect)) {
                throw std::system_error(GetLastError(), std::system_category(),
                    "Failed to change IAT protection");
            }

            auto oldaddr = InterlockedExchange(const_cast<uintptr_t *>(iat),
                reinterpret_cast<uintptr_t>(hook));

            VirtualProtect(const_cast<uintptr_t *>(iat), sizeof(function_ptr),
                old_protect, &old_protect);

            return reinterpret_cast<function_ptr>(oldaddr);
        }

        return nullptr;
    }

    return nullptr;
}
