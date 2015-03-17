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
#include "ConfigurationFileParser.h"

Configurations::Configurations()
{
}

HRESULT Configurations::LoadFromFile(BSTR name)
{
    HRESULT hr;

    // Validate Parameters
    if (!name)
        return E_POINTER;

    // Parse
    try
    {
        CConfigurationFileParser parser(name);

        if (FAILED(hr = parser.Parse()))
            return hr;

        m_conf = parser.GetConfigurations();
    }
    catch (CAtlException& e)
    {
        return e;
    }

    return S_OK;
}
