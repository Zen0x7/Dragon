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

#include "app_fixture.hpp"

class serve_test : public app_fixture {};

TEST_F(serve_test, it_can_run) {
  const auto _state = get_app()->get_state();
  ASSERT_TRUE(_state->running_);
  ASSERT_NE(_state->config_.port_, 0);
}

TEST_F(serve_test, it_can_accept_connections) {
  const auto _state = get_app()->get_state();

  boost::asio::io_context _ioc;
  boost::asio::ip::tcp::resolver _resolver(_ioc);
  boost::beast::tcp_stream _stream(_ioc);

  auto const _results = _resolver.resolve(
      _state->config_.address_, std::to_string(_state->config_.port_));
  _stream.connect(_results);

  ASSERT_TRUE(_stream.socket().is_open());

  boost::beast::error_code _error_code;
  _stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                            _error_code);

  ASSERT_FALSE(_error_code && _error_code != boost::beast::errc::not_connected);
}

TEST_F(serve_test, it_can_handle_requests) {
  const auto _state = get_app()->get_state();

  boost::asio::io_context _ioc;
  boost::asio::ip::tcp::resolver _resolver(_ioc);
  boost::beast::tcp_stream _stream(_ioc);

  auto const _results = _resolver.resolve(
      _state->config_.address_, std::to_string(_state->config_.port_));
  _stream.connect(_results);

  boost::beast::http::request<boost::beast::http::string_body> _request{
      boost::beast::http::verb::get, "/not-found", 11};
  _request.set(boost::beast::http::field::host, _state->config_.address_);
  _request.set(boost::beast::http::field::user_agent,
               "Serve Test - It can handle requests");

  boost::beast::http::write(_stream, _request);
  boost::beast::flat_buffer _buffer;

  boost::beast::http::response<boost::beast::http::string_body> _response;
  boost::beast::http::read(_stream, _buffer, _response);

  ASSERT_EQ(_response["Content-Type"], "application/json");
  ASSERT_EQ(_response["Server"], "Dragon");
  ASSERT_EQ(std::stoi(_response["Content-Length"]), 45);

  auto _body = boost::json::parse(_response.body());

  ASSERT_TRUE(_body.is_object());

  auto _body_ = _body.as_object();
  ASSERT_TRUE(_body_.contains("status"));
  ASSERT_TRUE(_body_.at("status").is_number());
  ASSERT_EQ(_body_.at("status").as_int64(), 404);
  ASSERT_TRUE(_body_.contains("message"));
  ASSERT_TRUE(_body_.at("message").is_string());
  ASSERT_EQ(_body_.at("message").as_string(), "Resource not found");

  boost::beast::error_code _error_code;
  _stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                            _error_code);

  if (_error_code && _error_code != boost::beast::errc::not_connected)
    throw boost::system::system_error(_error_code);
}

TEST_F(serve_test, it_can_handle_bad_requests) {
  const auto _state = get_app()->get_state();

  boost::asio::io_context _ioc;
  boost::asio::ip::tcp::resolver _resolver(_ioc);
  boost::beast::tcp_stream _stream(_ioc);

  auto const _results = _resolver.resolve(
      _state->config_.address_, std::to_string(_state->config_.port_));
  _stream.connect(_results);

  std::array<std::string, 2> _paths = {
      "rootless",   // [0] != '/'
      "/../going-back",  // contains "..",
  };

  for (const auto& _path : _paths) {
    boost::beast::http::request<boost::beast::http::string_body> _request{
        boost::beast::http::verb::get, _path, 11};
    _request.set(boost::beast::http::field::host, _state->config_.address_);
    _request.set(boost::beast::http::field::user_agent,
                 "Serve Test - It can handle bad requests");
    _request.set(boost::beast::http::field::connection, "keep-alive");

    boost::beast::http::write(_stream, _request);
    boost::beast::flat_buffer _buffer;

    boost::beast::http::response<boost::beast::http::string_body> _response;
    boost::beast::http::read(_stream, _buffer, _response);

    ASSERT_EQ(_response["Content-Type"], "application/json");
    ASSERT_EQ(_response["Server"], "Dragon");
    ASSERT_EQ(std::stoi(_response["Content-Length"]), 49);

    auto _body = boost::json::parse(_response.body());

    ASSERT_TRUE(_body.is_object());

    auto _body_ = _body.as_object();
    ASSERT_TRUE(_body_.contains("status"));
    ASSERT_TRUE(_body_.at("status").is_number());
    ASSERT_EQ(_body_.at("status").as_int64(), 400);
    ASSERT_TRUE(_body_.contains("message"));
    ASSERT_TRUE(_body_.at("message").is_string());
    ASSERT_EQ(_body_.at("message").as_string(), "Resource bad requested");
  }

  boost::beast::error_code _error_code;
  _stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                            _error_code);

  if (_error_code && _error_code != boost::beast::errc::not_connected)
    throw boost::system::system_error(_error_code);
}
