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

#include <dragon/state.hpp>

#include <dragon/handlers/bad_request.hpp>
#include <dragon/handlers/not_found.hpp>

namespace dragon {
/**
 * Handler
 *
 * @param state
 * @param request
 * @return
 */
inline boost::beast::http::message_generator handler(
    const std::shared_ptr<state>& state,
    const boost::beast::http::request<boost::beast::http::string_body>&
        request) {
  const auto _rootless = request.target()[0] != '/';
  const auto _path_scaling =
      request.target().find("..") != boost::beast::string_view::npos;

  if (_rootless || _path_scaling) {
    return handlers::bad_request(request);
  }

  const auto _path = std::string(request.target());
  const auto _callback = state->get_router()->find(request.method(), _path);

  if (!_callback) {
    return handlers::not_found(request);
  }

  const std::unordered_map<std::string, std::string> _params;
  auto _response = _callback->handle(request, _params);
  return boost::beast::http::message_generator{std::move(_response)};
}
}  // namespace dragon

#endif  // DRAGON_HANDLER_HPP
