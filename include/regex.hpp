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

/// @file

#ifndef SCTF_REGEX_HPP
#define SCTF_REGEX_HPP

#include <cstddef>
#include <regex>

namespace sctf
{
/**
 * A wrapper for regular expressions, that contains the compiled regex as well as the expression
 * pattern.
 */
struct regex final
{
    /**
     * @param p_ is the regular expression pattern.
     * @param flags_ is a bit pattern of regex options.
     */
    regex(char const* p_, std::regex_constants::syntax_option_type flags_)
        : pattern(p_), re(p_, flags_) {}

    /**
     * Convert this wrapper to std::regex.
     */
    operator std::regex() const {
        return re;
    }

    char const* const pattern;  ///< Regular expression pattern.
    std::regex const  re;       ///< Compiled regex.
};

/**
 * Literal operator to create a regex from cstring.
 * The produced regex uses ECMAScript syntax, and does not allow capturing.
 *
 * EXAMPLE:
 * @code
 * ".*"_re
 * @endcode
 *
 * @param lit_ is the regular expression.
 * @return a regex built from the given pattern.
 */
inline regex operator"" _re(char const* lit_, std::size_t) {
    return regex(lit_, std::regex::nosubs | std::regex::ECMAScript);
}

/**
 * Literal operator to create a regex from cstring.
 * The produced regex uses ECMAScript syntax, does not allow capturing, and is case insensitive.
 *
 * EXAMPLE:
 * @code
 * ".*"_re_i
 * @endcode
 *
 * @param lit_ is the regular expression.
 * @return a regex built from the given pattern.
 */
inline regex operator"" _re_i(char const* lit_, std::size_t) {
    return regex(lit_, std::regex::nosubs | std::regex::ECMAScript | std::regex::icase);
}
}  // namespace sctf

#endif  // SCTF_REGEX_HPP