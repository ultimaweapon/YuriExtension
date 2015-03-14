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

Configurations::Configurations()
{
}

HRESULT Configurations::LoadFromFile(BSTR name)
{
    HRESULT hr;

    // Validate Parameters
    if (!name)
        return E_POINTER;

    // Initialize XML Reader
    CComPtr<IStream> pFile;
    if (FAILED(hr = SHCreateStreamOnFileEx(name, STGM_READ | STGM_SHARE_DENY_WRITE, FILE_ATTRIBUTE_NORMAL, FALSE, NULL, &pFile)))
        return hr;

    CComPtr<IXmlReader> pReader;
    if (FAILED(hr = CreateXmlReader(__uuidof(IXmlReader), (void **)&pReader, NULL)))
        return hr;

    if (FAILED(hr = pReader->SetInput(pFile)))
        return hr;

    // Parse XML
    while (!pReader->IsEOF())
    {
        // Read next element
        XmlNodeType nodeType;

        while (FAILED(hr = pReader->Read(&nodeType)))
        {
            if (hr != E_PENDING)
                return hr;
        }

        if (hr != S_OK)
            break;

        // Process current element
        switch (nodeType)
        {
        case XmlNodeType_Element:
            break;
        case XmlNodeType_Attribute:
            break;
        case XmlNodeType_Text:
            break;
        case XmlNodeType_CDATA:
            break;
        case XmlNodeType_EndElement:
            break;
        }
    }

    return S_OK;
}
