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

#ifndef SCTF_ASSERTION_FAILURE_HPP
#define SCTF_ASSERTION_FAILURE_HPP

#include <exception>
#include <string>

#include "loc.hpp"

namespace sctf
{
namespace intern
{
/**
 * Used to be thrown when an assertion fails.
 */
class assertion_failure : public std::exception
{
public:
    /**
     * @param msg_ is the failure message
     * @param loc_ is the line of code where the assertion took place
     */
    assertion_failure(std::string const& msg_, loc const& loc_)
        : m_msg(msg_ + " at " + loc_.file + ":" + std::to_string(loc_.line)) {}

    virtual ~assertion_failure() noexcept override = default;

    /**
     * Get the failure message.
     */
    inline char const* what() const noexcept override {
        return m_msg.c_str();
    }

protected:
    std::string const m_msg;  ///< Message describing the failure cause.
};
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_ASSERTION_FAILURE_HPP
