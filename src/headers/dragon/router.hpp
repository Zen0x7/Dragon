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

#ifndef DRAGON_ROUTER_HPP
#define DRAGON_ROUTER_HPP

#include <boost/beast/http.hpp>
#include <dragon/callback.hpp>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace dragon {
/**
 * Router
 */
class router {
 public:
  /**
   * Bind
   * @param method
   * @param expression
   * @param resolver
   */
  void bind(
      const boost::beast::http::verb method,
      const std::string& expression,
      std::function<boost::beast::http::response<
          boost::beast::http::string_body>(
          const boost::beast::http::request<boost::beast::http::string_body>&,
          const std::unordered_map<std::string, std::string>&)> resolver) {
    auto _callback = std::make_shared<callback>(std::move(resolver));
    routes_[compose_key(method, expression)] = std::move(_callback);
  }

  /**
   * Find
   *
   * @param method
   * @param path
   * @return
   */
  std::shared_ptr<callback_base> find(const boost::beast::http::verb method,
                                      const std::string& path) const {
    if (const auto _it = routes_.find(compose_key(method, path));
        _it != routes_.end())
      return _it->second;
    return nullptr;
  }

 private:
  /**
   * Compose key
   *
   * @param method
   * @param expr
   * @return
   */
  static std::string compose_key(const boost::beast::http::verb method,
                                 const std::string& expr) {
    return std::string(boost::beast::http::to_string(method)) + " " + expr;
  }

  /**
   * Routes
   */
  std::unordered_map<std::string, std::shared_ptr<callback_base>> routes_;
};
}  // namespace dragon

#endif  // DRAGON_ROUTER_HPP
