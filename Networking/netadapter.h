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

#include "netaddress.h"

class netadapter final {
public:
    netadapter();
    netadapter(netadapter&& src);

    netadapter& operator=(netadapter&& src);

    void add_addr(const netaddress& addr);
    void add_addr(netaddress&& addr);
    const std::vector<netaddress>& addrs() const { return addrs_; }
private:
    std::vector<netaddress> addrs_;
};

extern std::vector<netadapter> netadapters;
