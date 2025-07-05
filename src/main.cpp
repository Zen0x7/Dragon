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
#include <dragon/config.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <iostream>

int main(const int argc, char* argv[]) {
  boost::program_options::options_description _description("Options");

  auto _add_option = _description.add_options();

  const auto _address_value = boost::program_options::value<std::string>()->default_value("0.0.0.0");
  const auto _port_value = boost::program_options::value<unsigned short>()->default_value(8000);
  const auto _threads_value = boost::program_options::value<int>()->default_value(1);

  _add_option("address", _address_value, "Address");
  _add_option("port", _port_value, "Port");
  _add_option("threads", _threads_value, "Threads");

  boost::program_options::variables_map _variables;
  store(parse_command_line(argc, argv, _description), _variables);

  if (_variables.contains("help")) {
    std::cout << _description << std::endl;
    return 0;
  }

  const dragon::config _config {
    .address_ = _variables["address"].as<std::string>(),
    .port_ = _variables["port"].as<unsigned short>(),
    .threads_ = _variables["threads"].as<int>()
  };

  return dragon::serve(_config);
}
