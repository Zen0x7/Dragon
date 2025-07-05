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

#include <dragon/app.hpp>
#include <dragon/exceptions.hpp>
#include <dragon/listener.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>

#include <iostream>
#include <thread>

namespace dragon {
app::app(const config& config)
    : state_(std::make_shared<state>(config)),
      ioc_(state_->config_.threads_),
      endpoint_(boost::asio::ip::make_address(state_->config_.address_),
                state_->config_.port_) {}

int app::run() {
  boost::asio::co_spawn(ioc_, listener(state_, endpoint_),
                        exception_handler("acceptor"));

  std::vector<std::jthread> _threads;
  _threads.reserve(state_->config_.threads_ - 1);
  for (auto _i = state_->config_.threads_ - 1; _i > 0; --_i)
    _threads.emplace_back(
        [_self = this->shared_from_this()] { _self->ioc_.run(); });
  ioc_.run();

  return EXIT_SUCCESS;
}

void app::stop() {
  ioc_.stop();
}

std::shared_ptr<state> app::get_state() {
  return state_;
}
}  // namespace dragon
