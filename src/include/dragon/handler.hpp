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

#include <dragon/handlers/bad_request.hpp>
#include <dragon/handlers/not_found.hpp>

#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/json/serialize.hpp>

namespace dragon {
/**
 * Handler
 *
 * @param request
 * @return
 */
inline boost::beast::http::message_generator handler(
    const boost::beast::http::request<boost::beast::http::string_body>&
        request) {
  const auto _rootless = request.target()[0] != '/';
  const auto _path_scaling =
      request.target().find("..") != boost::beast::string_view::npos;

  if (_rootless || _path_scaling)
    return handlers::bad_request(request);

  return handlers::not_found(request);
}

}  // namespace dragon

#endif  // DRAGON_HANDLER_HPP
