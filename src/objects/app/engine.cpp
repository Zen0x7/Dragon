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

#include <app/engine.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/core/ignore_unused.hpp>

#include <dragon/handlers/bad_request.hpp>
#include <dragon/handlers/not_found.hpp>

namespace app {
void engine::bootstrap(const std::shared_ptr<dragon::app>& app) {
  // Bootstrap your application

  boost::ignore_unused(app);  // Remove this when app becomes usable.

  // Configure
  // app->config_->set_read_timeout(50);
  // app->config_->set_debug(true);

  // Register routes
  // app->router_->bind(
  //   boost::beast::http::verb::get,
  //   "/welcome",
  //   [](const boost::beast::http::request<boost::beast::http::string_body> &request) {
  //   return dragon::handlers::not_found(request);
  // });
}
}  // namespace app
