/*
 Copyright_License {

 Copyright (C) 2017 Julian P. Becht
 Author: Julian P. Becht

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License version 3
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 }
 */

#ifndef SCTF_ASSERT_HPP
#define SCTF_ASSERT_HPP

#include "comparator/comparator.hpp"

#include "assertion_failure.hpp"
#include "duration.hpp"
#include "loc.hpp"
#include "stringify.hpp"
#include "types.hpp"

/**
 * Test for successful comparison.
 * @param VALUE The actual value
 * @param COMP The Comparator
 * @param EXPECT The expected value
 */
#define ASSERT(VALUE, COMP, EXPECT)                             \
    sctf::intern::assert_statement(VALUE, EXPECT, sctf::COMP(), \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Test for successful negated comparison.
 * @param VALUE The actual value
 * @param COMP The Comparator
 * @param EXPECT The expected value
 */
#define ASSERT_NOT(VALUE, COMP, EXPECT)                          \
    sctf::intern::assert_statement(VALUE, EXPECT, !sctf::COMP(), \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Test for equality.
 * @param VALUE The actual value
 * @param EXPECT The expected value
 */
#define ASSERT_EQ(VALUE, EXPECT)                                  \
    sctf::intern::assert_statement(VALUE, EXPECT, sctf::EQUALS(), \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Test value to be true.
 * @param VALUE The value
 */
#define ASSERT_TRUE(VALUE)                                      \
    sctf::intern::assert_statement(VALUE, true, sctf::EQUALS(), \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Test value to be false.
 * @param VALUE The value
 */
#define ASSERT_FALSE(VALUE)                                      \
    sctf::intern::assert_statement(VALUE, false, sctf::EQUALS(), \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Test value to be nullptr.
 * @param VALUE The value
 */
#define ASSERT_NULL(VALUE)                                                                   \
    sctf::intern::assert_statement(static_cast<void const*>(VALUE), nullptr, sctf::EQUALS(), \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Test value not to be nullptr.
 * @param VALUE The value
 */
#define ASSERT_NOT_NULL(VALUE)                                                                 \
    sctf::intern::assert_statement(static_cast<void const*>(VALUE), nullptr, sctf::UNEQUALS(), \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Test value to be 0.
 * @param VALUE The value
 */
#define ASSERT_ZERO(VALUE)                                                                 \
    sctf::intern::assert_statement(VALUE, static_cast<decltype(VALUE)>(0), sctf::EQUALS(), \
                                   sctf::intern::loc{__FILE__, __LINE__})

/**
 * Test for FUNC to throw TYPE.
 * @param FUNC The function call
 * @param TYPE The exception type
 */
#define ASSERT_THROWS(STMT, TYPE) \
    sctf::intern::assert_throws<TYPE>([&] { STMT; }, sctf::intern::loc{__FILE__, __LINE__})

/**
 * Test for FUNC not to throw any exception.
 * @param FUNC The function call
 */
#define ASSERT_NOTHROW(STMT) \
    sctf::intern::assert_nothrow([&] { STMT; }, sctf::intern::loc{__FILE__, __LINE__})

/**
 * Test for FUNC to run faster than MILLIS.
 * @param FUNC The function call
 * @param MILLIS The max amount of milliseconds
 */
#define ASSERT_RUNTIME(STMT, MILLIS) \
    sctf::intern::assert_runtime([&] { STMT; }, MILLIS, sctf::intern::loc{__FILE__, __LINE__})

namespace sctf
{
namespace intern
{
template<typename C, typename V, typename E = V>
static void assert_statement(V const& val_, E const& expect_, C&& cmp_, loc const& loc_) {
    comparison res = cmp_(val_, expect_);
    if (!res) {
        throw assertion_failure(*res, loc_);
    }
}

template<typename T>
static void assert_throws(void_function&& fn_, loc const& loc_) {
    try {
        fn_();
    } catch (T const&) {
        return;
    } catch (std::exception const& e) {
        throw assertion_failure("Wrong exception thrown, caught " + to_string(e), loc_);
    } catch (...) {
        throw assertion_failure("Wrong exception thrown", loc_);
    }
    throw assertion_failure("No exception thrown, expected " + name_for_type<T>(), loc_);
}

static void assert_nothrow(void_function&& fn_, loc const& loc_) {
    try {
        fn_();
    } catch (const std::exception& e) {
        throw assertion_failure("Expected no exception, caught " + to_string(e), loc_);
    } catch (...) {
        throw assertion_failure("Expected no exception", loc_);
    }
}

static void assert_runtime(void_function&& fn_, double max_ms_, loc const& loc_) {
    try {
        duration dur;
        fn_();
        double dur_ms = dur.get();
        if (dur_ms > max_ms_) {
            throw assertion_failure("runtime > " + to_string(max_ms_) + "ms", loc_);
        }
    } catch (std::exception const& e) {
        throw assertion_failure(e.what(), loc_);
    } catch (...) {
        throw assertion_failure("Unknown exception thrown", loc_);
    }
}
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_ASSERT_HPP
