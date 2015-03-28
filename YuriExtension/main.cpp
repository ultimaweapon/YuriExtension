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
#include "config.h"

#include <yuri_extension.h>

static unsigned int initcnt;
static CRITICAL_SECTION initlock;
static std::exception_ptr initerr;
static volatile bool activated;

static bool init()
{
    // check to see if we should activate
    DWORD nbuf = MAX_PATH / 2;
    std::unique_ptr<wchar_t[]> gamemod;
    const wchar_t *exe;

    do {
        gamemod = std::make_unique<wchar_t[]>(nbuf *= 2);
    } while (GetModuleFileNameW(NULL, gamemod.get(), nbuf) == nbuf);

    exe = PathFindFileNameW(gamemod.get());

    if (_wcsicmp(exe, L"gamemd.exe")) {
        return false;
    }

    // check initialization flag
    if (initerr) {
        std::rethrow_exception(initerr);
    }

    if (initcnt++) {
        return true;
    }

    try {
        init_config();
    } catch (std::exception&) {
        initerr = std::current_exception();
        initcnt--;
        throw;
    }

    return activated = true;
}

static void term()
{
    // check initialization flag
    if (initcnt == 0 || --initcnt) {
        return;
    }

    activated = false;

    term_config();
}

bool yuriext::init()
{
    EnterCriticalSection(&initlock);

    __try {
        return ::init();
    } __finally {
        LeaveCriticalSection(&initlock);
    }
}

bool yuriext::is_activated()
{
    return activated;
}

void yuriext::term()
{
    EnterCriticalSection(&initlock);

    __try {
        ::term();
    } __finally {
        LeaveCriticalSection(&initlock);
    }
}

extern "C" BOOL APIENTRY DllMain(HMODULE Module, DWORD CallReason,
    LPVOID Reserved)
{
    UNREFERENCED_PARAMETER(Module);
    UNREFERENCED_PARAMETER(Reserved);

    switch (CallReason) {
    case DLL_PROCESS_ATTACH:
        InitializeCriticalSection(&initlock);
        break;
    case DLL_PROCESS_DETACH:
        DeleteCriticalSection(&initlock);
        break;
    }

    return TRUE;
}
