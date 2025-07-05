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

#ifndef DRAGON_APP_HPP
#define DRAGON_APP_HPP

#include <dragon/config.hpp>
#include <dragon/state.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace dragon {
/**
 * Version
 *
 * @return
 */
inline std::string version() {
  return "1.0.0";
}

/**
 * App
 */
class app : public std::enable_shared_from_this<app> {
  /**
   * State
   */
  std::shared_ptr<state> state_ = std::make_shared<state>();

  /**
   * Config
   */
  config config_;

  /**
   * IO Context
   */
  boost::asio::io_context ioc_;

  /**
   * Endpoint
   */
  boost::asio::ip::tcp::endpoint endpoint_;

 public:
  /**
   * App constructor
   *
   * @param config
   */
  explicit app(const config& config);

  /**
   * Run
   *
   * @return
   */
  int run();

  /**
   * Stop
   */
  void stop();

  /**
   * Get state
   *
   * @return
   */
  std::shared_ptr<state> get_state();
};
}  // namespace dragon

#endif  // DRAGON_APP_HPP
