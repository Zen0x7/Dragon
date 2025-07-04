// Copyright (C) 2025 Ian Torres
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef DRAGON_SESSION_HPP
#define DRAGON_SESSION_HPP

#include <boost/asio/awaitable.hpp>
#include <boost/beast/core/tcp_stream.hpp>

namespace dragon {
    /**
     * Session
     *
     * @param stream
     * @return
     */
    boost::asio::awaitable<void> session(boost::beast::tcp_stream stream);
}

#endif  // DRAGON_SESSION_HPP
