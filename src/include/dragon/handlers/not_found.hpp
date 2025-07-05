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

#ifndef DRAGON_HANDLERS_NOT_FOUND_HPP
#define DRAGON_HANDLERS_NOT_FOUND_HPP

#include <boost/beast/http/string_body.hpp>
#include <boost/json/serialize.hpp>

namespace dragon::handlers {
/**
 * Not found
 *
 * @param request
 * @return
 */
inline boost::beast::http::response<boost::beast::http::string_body> not_found(
    const boost::beast::http::request<boost::beast::http::string_body>&
        request) {
  boost::beast::http::response<boost::beast::http::string_body> _response{
      boost::beast::http::status::not_found, request.version()};
  _response.set(boost::beast::http::field::server, SERVER_NAME);
  _response.set(boost::beast::http::field::content_type, "application/json");
  _response.keep_alive(request.keep_alive());
  _response.body() = boost::json::serialize(boost::json::object(
      {{"status", 404}, {"message", "Resource not found"}}));
  _response.prepare_payload();
  return _response;
}
}  // namespace dragon::handlers

#endif  // DRAGON_HANDLERS_NOT_FOUND_HPP
