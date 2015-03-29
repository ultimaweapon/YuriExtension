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

#include <utilities/module.h>

std::wstring util::get_module_filename(HMODULE mod)
{
    DWORD nbuf = MAX_PATH / 2;
    std::unique_ptr<wchar_t[]> fname;

    do {
        fname = std::make_unique<wchar_t[]>(nbuf *= 2);
    } while (GetModuleFileNameW(mod, fname.get(), nbuf) == nbuf);

    return fname.get();
}
