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

#include <gtest/gtest.h>

#include "../app_fixture.hpp"

class app_test : public app_fixture {};

TEST_F(app_test, it_can_handle_welcome_request) {
  const auto _state = get_app()->get_state();

  boost::asio::io_context _ioc;
  boost::asio::ip::tcp::resolver _resolver(_ioc);
  boost::beast::tcp_stream _stream(_ioc);

  auto const _results = _resolver.resolve(
      _state->config_.address_, std::to_string(_state->config_.port_));
  _stream.connect(_results);

  boost::beast::http::request<boost::beast::http::string_body> _request{
      boost::beast::http::verb::get, "/welcome", 11};
  _request.set(boost::beast::http::field::host, _state->config_.address_);
  _request.set(boost::beast::http::field::user_agent,
               "App Test - It can handle welcome request");

  boost::beast::http::write(_stream, _request);
  boost::beast::flat_buffer _buffer;

  boost::beast::http::response<boost::beast::http::string_body> _response;
  boost::beast::http::read(_stream, _buffer, _response);

  ASSERT_EQ(_response["Content-Type"], "application/json");
  ASSERT_EQ(_response["Server"], "Dragon");
  ASSERT_EQ(std::stoi(_response["Content-Length"]), 34);

  auto _body = boost::json::parse(_response.body());

  ASSERT_TRUE(_body.is_object());

  auto _body_ = _body.as_object();
  ASSERT_TRUE(_body_.contains("data"));
  ASSERT_TRUE(_body_.at("data").is_string());
  ASSERT_EQ(_body_.at("data").as_string(), "EHLO");

  boost::beast::error_code _error_code;
  _stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                            _error_code);

  if (_error_code && _error_code != boost::beast::errc::not_connected)
    throw boost::system::system_error(_error_code);
}