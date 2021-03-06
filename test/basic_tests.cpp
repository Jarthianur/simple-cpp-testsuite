/*
    Copyright (C) 2017  Jarthianur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "basic_tests.hpp"

#include <sstream>

#include "test_traits.hpp"

void
test_traits();

void
basic_tests() {
    test_traits();
}

void
test_traits() {
    // is_streamable
    throw_if_not_streamable<std::ostringstream, streamable>();
    try {
        throw_if_not_streamable<std::ostringstream, void_type>();
        throw Failure("Given type should be not streamable");
    } catch (std::logic_error const&) {
    }
    try {
        throw_if_not_streamable<std::ostringstream, not_streamable>();
        throw Failure("Given type should be not streamable");
    } catch (std::logic_error const&) {
    }

    // is_iterable
    throw_if_not_iterable<iterable>();
    try {
        throw_if_not_iterable<void_type>();
        throw Failure("Given type should be not iterable");
    } catch (std::logic_error const&) {
    }
    try {
        throw_if_not_iterable<not_iterable>();
        throw Failure("Given type should be not iterable");
    } catch (std::logic_error const&) {
    }
}
