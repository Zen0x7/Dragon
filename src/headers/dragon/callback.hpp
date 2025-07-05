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

#ifndef DRAGON_CALLBACK_HPP
#define DRAGON_CALLBACK_HPP

#include <boost/beast/http.hpp>

namespace dragon {
/**
 * Callback base
 */
struct callback_base {
  /**
   * Constructor
   */
  virtual ~callback_base() = default;

  /**
   * Handle
   *
   * @return
   */
  virtual boost::beast::http::response<boost::beast::http::string_body> handle(
      const boost::beast::http::request<boost::beast::http::string_body>&,
      const std::unordered_map<std::string, std::string>&) = 0;
};

/**
 * Callback
 */
class callback final : public callback_base {
 public:
  /**
   * Constructor
   *
   * @param lambda
   */
  explicit callback(
      std::function<boost::beast::http::response<
          boost::beast::http::string_body>(
          const boost::beast::http::request<boost::beast::http::string_body>&,
          const std::unordered_map<std::string, std::string>&)> lambda)
      : resolver_(std::move(lambda)) {}

  /**
   * Handle
   *
   * @param request
   * @param params
   * @return
   */
  boost::beast::http::response<boost::beast::http::string_body> handle(
      const boost::beast::http::request<boost::beast::http::string_body>&
          request,
      const std::unordered_map<std::string, std::string>& params) override {
    return resolver_(request, params);
  }

 private:
  /**
   * Resolver
   */
  std::function<boost::beast::http::response<boost::beast::http::string_body>(
      const boost::beast::http::request<boost::beast::http::string_body>&,
      const std::unordered_map<std::string, std::string>&)>
      resolver_;
};
}  // namespace dragon

#endif  // DRAGON_CALLBACK_HPP
