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

extern "C" int WSAAPI ipx_bind(SOCKET s, const sockaddr *name, int namelen);
extern "C" int WSAAPI ipx_getsockopt(SOCKET s, int level, int optname,
    char *optval, int *optlen);
extern "C" int WSAAPI ipx_recvfrom(SOCKET s, char *buf, int len, int flags,
    sockaddr *from, int *fromlen);
extern "C" int WSAAPI ipx_sendto(SOCKET s, const char *buf, int len, int flags,
    const sockaddr *to, int tolen);
extern "C" int WSAAPI ipx_setsockopt(SOCKET s, int level, int optname,
    const char *optval, int optlen);
extern "C" SOCKET WSAAPI ipx_socket(int af, int type, int protocol);
