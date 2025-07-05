#ifndef DRAGON_STATE_HPP
#define DRAGON_STATE_HPP

#include <memory>

namespace dragon {
/**
 * State
 */
class state : public std::enable_shared_from_this<state> {
 public:
  /**
   * Running
   */
  bool running_ = false;
};
}  // namespace dragon

#endif  // DRAGON_STATE_HPP
