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
#pragma once

#include "Resources.h"
#include "Configurations_h.h"

class Configurations :
    public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<Configurations, &CLSID_Configurations>,
    public IConfigurations
{
public: // Constructors/Destructors
    Configurations();
public: // ATL & WTL Properties
    DECLARE_NO_REGISTRY()
    DECLARE_CLASSFACTORY()

    BEGIN_COM_MAP(Configurations)
        COM_INTERFACE_ENTRY(IConfigurations)
    END_COM_MAP()
public: // IConfigurations Interface
    virtual HRESULT STDMETHODCALLTYPE LoadFromFile(BSTR name);
};
