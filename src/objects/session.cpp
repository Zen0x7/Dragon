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

#include <dragon/session.hpp>

#include <dragon/handler.hpp>

#include <boost/asio/use_awaitable.hpp>

#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/string_body.hpp>

namespace dragon {

boost::asio::awaitable<void> session(boost::beast::tcp_stream stream) {
  boost::beast::flat_buffer buffer;
  try {
    for (;;) {
      stream.expires_after(std::chrono::seconds(30));

      boost::beast::http::request<boost::beast::http::string_body> req;
      co_await boost::beast::http::async_read(stream, buffer, req);

      boost::beast::http::message_generator msg =
          handler(std::move(req));

      bool keep_alive = msg.keep_alive();
      co_await boost::beast::async_write(stream, std::move(msg),
                                         boost::asio::use_awaitable);

      if (!keep_alive) {
        break;
      }
    }
  } catch (boost::system::system_error& se) {
    if (se.code() != boost::beast::http::error::end_of_stream)
      throw;
  }

  boost::beast::error_code ec;
  stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
}
}  // namespace dragon
