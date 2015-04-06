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

namespace yuriext {
    enum class debug_message_category {
        core,
        networking
    };

    enum class debug_message_type {
        info,
        warning,
        error
    };

    yuriext_api void debug_message(debug_message_category cat,
        debug_message_type type, const std::wstring& msg);

    template<typename... Args>
    void debug_message(debug_message_category cat, debug_message_type type,
        Args... args)
    {
        std::wostringstream buf;
        int dummy[sizeof...(Args)] = {(buf << args, 0)...};
        debug_message(cat, type, buf.str());
    }
} // namespace yuriext
