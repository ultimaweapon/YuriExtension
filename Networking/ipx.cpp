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
#include "netadapter.h"

extern "C" int WSAAPI ipx_bind(SOCKET s, const sockaddr *name, int namelen)
{
    if (name->sa_family == AF_IPX) {
        auto ipxaddr = reinterpret_cast<const sockaddr_ipx *>(name);
        sockaddr_in addr = {0};

        addr.sin_family = AF_INET;
        addr.sin_port = ipxaddr->sa_socket;
        std::memcpy(&addr.sin_addr, ipxaddr->sa_nodenum, 4);

        return bind(s, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
    } else {
        return bind(s, name, namelen);
    }
}

extern "C" int WSAAPI ipx_getsockopt(SOCKET s, int level, int optname, char *optval, int *optlen)
{
    if (level == NSPROTO_IPX) {
        PIPX_ADDRESS_DATA ipxaddr;
        int i;

        switch (optname) {
        case IPX_ADDRESS:
            ipxaddr = reinterpret_cast<PIPX_ADDRESS_DATA>(optval);

            // find requested address
            i = 0;

            for (const auto& net : netadapters) {
                for (const auto& addr : net.addrs()) {
                    if (i == ipxaddr->adapternum) {
                        ipxaddr->wan = FALSE;
                        ipxaddr->status = TRUE;
                        ipxaddr->maxpkt = 1470;
                        ipxaddr->linkspeed = 1000000;

                        std::memset(ipxaddr->netnum, 0,
                            sizeof(ipxaddr->netnum));
                        std::memset(ipxaddr->nodenum, 0,
                            sizeof(ipxaddr->nodenum));
                        std::memcpy(ipxaddr->nodenum,
                            &addr.address<sockaddr_in>()->sin_addr, 4);

                        return 0;
                    } else {
                        i++;
                    }
                }
            }

            return SOCKET_ERROR;
        case IPX_MAX_ADAPTER_NUM:
            reinterpret_cast<int *>(optval)[0] = 0;

            for (const auto& net : netadapters) {
                reinterpret_cast<int *>(optval)[0] += net.addrs().size();
            }

            return 0;
        default:
            return 0;
        }
    } else {
        return getsockopt(s, level, optname, optval, optlen);
    }
}

extern "C" int WSAAPI ipx_recvfrom(SOCKET s, char *buf, int len, int flags,
    sockaddr *from, int *fromlen)
{
    WSABUF buffer = {0};
    DWORD nrecv = 0;

    buffer.buf = buf;
    buffer.len = len;

    if (*fromlen == sizeof(sockaddr_ipx)) {
        auto ipxaddr = reinterpret_cast<sockaddr_ipx *>(from);
        sockaddr_in raddr;
        int raddr_len = sizeof(raddr);

        if (WSARecvFrom(s, &buffer, 1, &nrecv,
            reinterpret_cast<LPDWORD>(&flags),
            reinterpret_cast<sockaddr *>(&raddr), &raddr_len, nullptr,
            nullptr)) {
            nrecv = (DWORD)-1;
        }

        ipxaddr->sa_family = AF_IPX;
        std::memset(ipxaddr->sa_netnum, 0, sizeof(ipxaddr->sa_netnum));
        std::memset(ipxaddr->sa_nodenum, 0, sizeof(ipxaddr->sa_nodenum));
        std::memcpy(ipxaddr->sa_nodenum, &raddr.sin_addr.S_un.S_addr, 4);
        ipxaddr->sa_socket = raddr.sin_port;

        return nrecv;
    } else {
        if (WSARecvFrom(s, &buffer, 1, &nrecv,
            reinterpret_cast<LPDWORD>(&flags), from, fromlen, nullptr,
            nullptr)) {
            return -1;
        } else {
            return nrecv;
        }
    }
}

extern "C" int WSAAPI ipx_sendto(SOCKET s, const char *buf, int len, int flags,
    const sockaddr *to, int tolen)
{
    WSABUF buffer = {0};
    DWORD nsend;

    buffer.buf = const_cast<CHAR *>(buf);
    buffer.len = len;

    if (to->sa_family == AF_IPX) {
        auto ipxaddr = reinterpret_cast<const sockaddr_ipx *>(to);
        sockaddr_in dest = {0};

        dest.sin_family = AF_INET;
        dest.sin_port = ipxaddr->sa_socket;
        std::memcpy(&dest.sin_addr.S_un.S_addr, &ipxaddr->sa_nodenum, 4);

        if (WSASendTo(s, &buffer, 1, &nsend, flags,
            reinterpret_cast<sockaddr *>(&dest), sizeof(dest), nullptr,
            nullptr)) {
            return SOCKET_ERROR;
        } else {
            return nsend;
        }
    } else {
        if (WSASendTo(s, &buffer, 1, &nsend, flags, to, tolen, nullptr,
            nullptr)) {
            return SOCKET_ERROR;
        } else {
            return nsend;
        }
    }
}

extern "C" int WSAAPI ipx_setsockopt(SOCKET s, int level, int optname,
    const char *optval, int optlen)
{
    if (level == NSPROTO_IPX) {
        return 0;
    } else {
        return setsockopt(s, level, optname, optval, optlen);
    }
}

extern "C" SOCKET WSAAPI ipx_socket(int af, int type, int protocol)
{
    if (af == AF_IPX && type == SOCK_DGRAM && protocol == NSPROTO_IPX) {
        return WSASocketW(AF_INET, SOCK_DGRAM, IPPROTO_UDP, nullptr, 0, 0);
    } else {
        return WSASocketW(af, type, protocol, nullptr, 0, 0);
    }
}
