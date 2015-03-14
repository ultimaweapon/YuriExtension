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
#include "Configurations.h"
#include "Configurations_h.h"
#include "Configurations_i.c"

BEGIN_OBJECT_MAP(g_lpClasses)
    OBJECT_ENTRY(__uuidof(Configurations), Configurations)
END_OBJECT_MAP()

extern "C" HRESULT __stdcall DllCanUnloadNow()
{
    return _Module.DllCanUnloadNow();
}

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv)
{
    return _Module.DllGetClassObject(rclsid, riid, ppv);
}

extern "C" HRESULT __stdcall DllRegisterServer()
{
    return _Module.DllRegisterServer();
}

extern "C" HRESULT __stdcall DllUnregisterServer()
{
    return _Module.DllUnregisterServer();
}

extern "C" BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID /* lpReserved */)
{
    switch (dwCallReason)
    {
    case DLL_PROCESS_ATTACH:
        if (FAILED(_Module.Init(g_lpClasses, hModule, &LIBID_YuriExtensionConfigurations)))
            return FALSE; // Always E_OUTOFMEMORY.
        break;
    case DLL_PROCESS_DETACH:
        _Module.Term();
        break;
    }

    return TRUE;
}
