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

#ifndef DRAGON_HANDLER_HPP
#define DRAGON_HANDLER_HPP

#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/json/serialize.hpp>

#ifndef SERVER_STRING
#define SERVER_STRING "Dragon"
#endif

namespace dragon {
/**
 * Handler
 *
 * @param req
 * @return
 */
inline boost::beast::http::message_generator handler(
    boost::beast::http::request<boost::beast::http::string_body>&& req) {
  auto const bad_request = [&req](const boost::beast::string_view why) {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::bad_request, req.version()};
    res.set(boost::beast::http::field::server, SERVER_STRING);
    res.set(boost::beast::http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());
    res.body() = boost::json::serialize(
        boost::json::object({{"status", 400}, {"message", why}}));
    res.prepare_payload();
    return res;
  };

  auto const not_found = [&req]() {
    boost::beast::http::response<boost::beast::http::string_body> res{
        boost::beast::http::status::not_found, req.version()};
    res.set(boost::beast::http::field::server, SERVER_STRING);
    res.set(boost::beast::http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());
    res.body() = boost::json::serialize(boost::json::object(
        {{"status", 404}, {"message", "Resource not found"}}));
    res.prepare_payload();
    return res;
  };

  if (req.target().empty() || req.target()[0] != '/' ||
      req.target().find("..") != boost::beast::string_view::npos)
    return bad_request("Resource bad requested");

  return not_found();
}

}  // namespace dragon

#endif  // DRAGON_HANDLER_HPP
