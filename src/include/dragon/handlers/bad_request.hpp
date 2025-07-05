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

#ifndef DRAGON_HANDLERS_BAD_REQUEST_HPP
#define DRAGON_HANDLERS_BAD_REQUEST_HPP

#include <boost/beast/http/string_body.hpp>
#include <boost/json/serialize.hpp>

namespace dragon::handlers {
    using namespace boost::beast::http;
    using namespace boost::json;

    /**
     * Bad request
     *
     * @param request
     * @return
     */
    inline response<string_body> bad_request(const request<string_body> &request) {
        response<string_body> _response{status::bad_request, request.version()};
        _response.set(field::server, SERVER_NAME);
        _response.set(field::content_type, "application/json");
        _response.keep_alive(request.keep_alive());
        _response.body() = serialize(
            object({{"status", 400}, {"message", "Resource bad requested"}})
            );
        _response.prepare_payload();
        return _response;
    }
}

#endif // DRAGON_HANDLERS_BAD_REQUEST_HPP
