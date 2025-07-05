//
// Created by ian on 5/07/25.
//

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
