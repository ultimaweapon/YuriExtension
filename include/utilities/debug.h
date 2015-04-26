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

#include "api.h"

#include <sstream>
#include <string>

namespace util {
    utilapi void debug_message(const std::string& msg);

    template<typename... Args>
    void debug_message(Args... args)
    {
        std::ostringstream buf;
        int dummy[sizeof...(Args)] = {(buf << args, 0)...};
        debug_message(buf.str());
    }
} // namespace util
