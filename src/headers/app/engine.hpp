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

#ifndef APP_APP_HPP
#define APP_APP_HPP

#include <dragon/app.hpp>
#include <string>

namespace app {
/**
 * Engine
 */
class engine : public std::enable_shared_from_this<engine> {
 public:
  /**
   * Version
   *
   * @return
   */
  static std::string version() { return "1.0.0"; }

  /**
   * Bootstrap
   *
   * @param app
   */
  static void bootstrap(const std::shared_ptr<dragon::app>& app);
};
}  // namespace app

#endif  // APP_APP_HPP
