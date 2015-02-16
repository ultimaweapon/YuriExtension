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

extern "C" int WSAAPI ipx_bind(SOCKET s, const sockaddr *name, int namelen)
{
    if (name->sa_family == AF_IPX) {
        sockaddr_in addr = {0};

        addr.sin_family = AF_INET;
        addr.sin_port = reinterpret_cast<const sockaddr_ipx *>(name)->sa_socket;
        addr.sin_addr.S_un.S_addr = INADDR_ANY;

        return ::bind(s, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
    } else {
        return ::bind(s, name, namelen);
    }
}

extern "C" int WSAAPI ipx_getsockopt(SOCKET s, int level, int optname, char *optval, int *optlen)
{
    if (level == NSPROTO_IPX) {
        PIPX_ADDRESS_DATA ipxaddr;

        switch (optname) {
        case IPX_ADDRESS:
            ipxaddr = reinterpret_cast<PIPX_ADDRESS_DATA>(optval);
            ipxaddr->adapternum = 0;
            ipxaddr->wan = FALSE;
            ipxaddr->status = TRUE;
            ipxaddr->maxpkt = 1470;
            ipxaddr->linkspeed = 1000000;

            std::memset(ipxaddr->netnum, 0, sizeof(ipxaddr->netnum));
            std::memset(ipxaddr->nodenum, 0, sizeof(ipxaddr->nodenum));

            return 0;
        case IPX_MAX_ADAPTER_NUM:
            reinterpret_cast<int *>(optval)[0] = 1;
        default:
            return 0;
        }
    } else {
        return ::getsockopt(s, level, optname, optval, optlen);
    }
}
