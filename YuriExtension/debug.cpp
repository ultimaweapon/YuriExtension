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
#include "debug.h"

static HANDLE console = INVALID_HANDLE_VALUE;

void yuriext::debug_message(debug_message_category cat, debug_message_type type,
    const std::wstring& msg)
{
}

void init_debug()
{
    console = CreateFileW(L"\\\\.\\pipe\\yuri-extension-debug-console",
        GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

    if (console == INVALID_HANDLE_VALUE) {
        return;
    }
}

void term_debug()
{
    if (console == INVALID_HANDLE_VALUE) {
        return;
    }

    CloseHandle(console);
    console = INVALID_HANDLE_VALUE;
}
