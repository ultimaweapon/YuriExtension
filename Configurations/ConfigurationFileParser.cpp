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
#include "StdAfx.h"
#include "ConfigurationFileParser.h"

CConfigurationFileParser::CConfigurationFileParser(LPCWSTR lpszFile) :
    m_strFile(lpszFile),
    m_currentNodeType(XmlNodeType_None)
{
}

const CConfigurationsValue& CConfigurationFileParser::GetConfigurations() const
{
    return m_conf;
}

HRESULT CConfigurationFileParser::OnCharacterData(IXmlReader * /* pReader */)
{
    return S_FALSE;
}

HRESULT CConfigurationFileParser::OnEndElement(IXmlReader * /* pReader */)
{
    // Truncate node path
    ATLASSERT(m_nodePath.GetCount() != 0);
    m_nodePath.RemoveAt(m_nodePath.GetCount() - 1);

    return S_FALSE;
}

HRESULT CConfigurationFileParser::OnStartElement(IXmlReader *pReader)
{
    HRESULT hr;
    LPCWSTR lpszNodeName;

    ATLASSERT(pReader);

    // Append node path
    if (FAILED(hr = pReader->GetLocalName(&lpszNodeName, NULL)))
        return hr;

    m_nodePath.Add(lpszNodeName);

    // Process node
    if (m_nodePath[0] == L"yuri-extension")
    {
        return S_FALSE;
    }
    else
        return WC_E_SYNTAX;
}

HRESULT CConfigurationFileParser::OnText(IXmlReader * /* pReader */)
{
    return S_FALSE;
}

HRESULT CConfigurationFileParser::Parse()
{
    HRESULT hr;

    // Initialize XML Reader
    CComPtr<IStream> pFile;
    if (FAILED(hr = SHCreateStreamOnFileEx(m_strFile, STGM_READ | STGM_SHARE_DENY_WRITE, FILE_ATTRIBUTE_NORMAL, FALSE, NULL, &pFile)))
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
        while (FAILED(hr = pReader->Read(&m_currentNodeType)))
        {
            if (hr != E_PENDING)
                return hr;
        }

        if (hr != S_OK)
            break;

        // Process current element
        switch (m_currentNodeType)
        {
        case XmlNodeType_Element:
            hr = OnStartElement(pReader);
            break;
        case XmlNodeType_Text:
            hr = OnText(pReader);
            break;
        case XmlNodeType_CDATA:
            hr = OnCharacterData(pReader);
            break;
        case XmlNodeType_EndElement:
            hr = OnEndElement(pReader);
            break;
        default:
            hr = S_FALSE;
        }

        if (FAILED(hr))
            return hr;
    }

    return S_OK;
}
