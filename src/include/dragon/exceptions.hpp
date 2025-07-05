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

#ifndef DRAGON_EXCEPTIONS_HPP
#define DRAGON_EXCEPTIONS_HPP

#include <exception>
#include <iostream>
#include <string_view>

namespace dragon {
inline auto exception_handler(const std::string_view where) {
  return [where](const std::exception_ptr& exception) {
    if (exception) {
      try {
        std::rethrow_exception(exception);
      } catch (std::exception& scoped_exception) {
        std::cerr << "Error in " << where << ": " << scoped_exception.what()
                  << "\n";
      }
    }
  };
}
}  // namespace dragon

#endif  // DRAGON_EXCEPTIONS_HPP
