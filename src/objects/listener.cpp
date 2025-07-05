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

#include <dragon/listener.hpp>

#include <dragon/session.hpp>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core/tcp_stream.hpp>

#include <iostream>

namespace dragon {
boost::asio::awaitable<void> listener(
    const boost::asio::ip::tcp::endpoint& endpoint) {
  const auto _executor = co_await boost::asio::this_coro::executor;
  auto _acceptor = boost::asio::use_awaitable_t<>::as_default_on(
      boost::asio::ip::tcp::acceptor(_executor));

  _acceptor.open(endpoint.protocol());
  _acceptor.set_option(boost::asio::socket_base::reuse_address(true));
  _acceptor.bind(endpoint);
  _acceptor.listen(boost::asio::socket_base::max_listen_connections);

  for (;;) {
    auto _stream = boost::beast::tcp_stream(co_await _acceptor.async_accept());
    boost::asio::co_spawn(_acceptor.get_executor(), session(std::move(_stream)),
                          [](const std::exception_ptr& exception) {
                            if (exception)
                              try {
                                std::rethrow_exception(exception);
                              } catch (std::exception& scoped_exception) {
                                std::cerr << "Error in session: "
                                          << scoped_exception.what() << "\n";
                              }
                          });
  }
}
}  // namespace dragon
