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

#include <dragon/session.hpp>
#include <dragon/state.hpp>

#include <dragon/handler.hpp>

#include <boost/asio/use_awaitable.hpp>

#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/string_body.hpp>

namespace dragon {
using namespace boost::beast::http;

boost::asio::awaitable<void> session(const std::shared_ptr<state>& state,
                                     boost::beast::tcp_stream stream) {
  boost::ignore_unused(state);

  using namespace boost::beast;
  flat_buffer _buffer;
  try {
    for (;;) {
      stream.expires_after(std::chrono::seconds(30));

      request<string_body> _request;
      co_await http::async_read(stream, _buffer, _request);

      message_generator _message = handler(std::move(_request));

      const bool _keep_alive = _message.keep_alive();
      co_await async_write(stream, std::move(_message),
                           boost::asio::use_awaitable);

      if (!_keep_alive) {
        break;
      }
    }
  } catch (boost::system::system_error& system_error) {
    if (system_error.code() != http::error::end_of_stream)
      throw;
  }

  error_code _error_code;
  stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send,
                           _error_code);
}
}  // namespace dragon
