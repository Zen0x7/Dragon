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
#include <dragon/listener.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>

#include <iostream>
#include <thread>

int dragon::serve(int argc, char* argv[]) {
  using namespace boost::program_options;

  options_description desc("Options");

  desc.add_options()("address", value<std::string>()->default_value("0.0.0.0"),
                     "Address")(
      "port", value<unsigned short>()->default_value(8000), "Port")(
      "threads", value<int>()->default_value(1), "Threads");

  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);

  if (vm.contains("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  boost::asio::io_context ioc{vm["threads"].as<int>()};

  boost::asio::co_spawn(
      ioc,
      dragon::listener(boost::asio::ip::tcp::endpoint{
          boost::asio::ip::make_address(vm["address"].as<std::string>()),
          vm["port"].as<unsigned short>()}),
      [](std::exception_ptr e) {
        if (e)
          try {
            std::rethrow_exception(e);
          } catch (std::exception& scoped_exception) {
            std::cerr << "Error in acceptor: " << scoped_exception.what()
                      << "\n";
          }
      });

  std::vector<std::thread> v;
  v.reserve(vm["threads"].as<int>() - 1);
  for (auto i = vm["threads"].as<int>() - 1; i > 0; --i)
    v.emplace_back([&ioc] { ioc.run(); });
  ioc.run();

  return EXIT_SUCCESS;
}
