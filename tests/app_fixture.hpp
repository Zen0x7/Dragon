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

#ifndef DRAGON_TESTS_APP_FIXTURE_HPP
#define DRAGON_TESTS_APP_FIXTURE_HPP

#include <gtest/gtest.h>
#include <dragon/app.hpp>
#include <memory>
#include <thread>

class app_fixture : public testing::Test {
  /**
   * Thread
   */
  std::unique_ptr<std::jthread> thread_;

  /**
   * App
   */
  std::shared_ptr<dragon::app> app_;

 protected:
  /**
   * Get app
   *
   * @return
   */
  std::shared_ptr<dragon::app> get_app() { return app_; }

  /**
   * Setup
   */
  void SetUp() override {
    dragon::config config_ = {
        .address_ = "0.0.0.0",
        .port_ = 0,
        .threads_ = 4,
    };

    app_ = std::make_shared<dragon::app>(config_);

    thread_ = std::make_unique<std::jthread>([this] { app_->run(); });

    while (!app_->get_state()->running_) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  }

  /**
   * Tear down
   */
  void TearDown() override { app_->stop(); }
};

#endif  // DRAGON_TESTS_APP_FIXTURE_HPP
