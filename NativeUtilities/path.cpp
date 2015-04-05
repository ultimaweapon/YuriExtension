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

#include <utilities/path.h>

std::wstring util::path_filename(const std::wstring& p)
{
    // check length
    auto len = p.length();

    if (len == 0) {
        throw std::invalid_argument("Invalid path name");
    }

    // check format
    auto ch = p[len - 1];

    if (ch == L'\\' || ch == L'/') {
        throw std::invalid_argument("No file's name in the path");
    }

    auto idx = p.find_last_of(L"\\/");

    if (idx == p.npos) {
        return p;
    }

    return p.substr(idx + 1);
}
