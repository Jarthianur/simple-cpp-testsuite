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

#ifndef SCTF_TYPES_HPP
#define SCTF_TYPES_HPP

#include <functional>

namespace sctf
{
namespace intern
{
/// A function that does not take arguments, and does not return any value.
using void_function = std::function<void()>;
}  // namespace intern
}  // namespace sctf

#endif  // SCTF_TYPES_HPP
