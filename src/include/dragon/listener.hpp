// Copyright (C) 2025 Ian Torres
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any later
// version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef DRAGON_LISTENER_HPP
#define DRAGON_LISTENER_HPP

#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace dragon {
/**
 * Forward state
 */
class state;

/**
 * Listener
 *
 * @param state
 * @param endpoint
 * @return
 */
boost::asio::awaitable<void> listener(
    const std::shared_ptr<state>& state,
    const boost::asio::ip::tcp::endpoint& endpoint);
}  // namespace dragon

#endif  // DRAGON_LISTENER_HPP
