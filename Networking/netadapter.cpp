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

std::vector<netadapter> netadapters;

netadapter::netadapter()
{
}

netadapter::netadapter(netadapter&& src) : addrs_(std::move(src.addrs_))
{
}

netadapter& netadapter::operator=(netadapter&& src)
{
    addrs_ = std::move(src.addrs_);

    return *this;
}

void netadapter::add_addr(const netaddress& addr)
{
    addrs_.push_back(addr);
}

void netadapter::add_addr(netaddress&& addr)
{
    addrs_.push_back(std::move(addr));
}
