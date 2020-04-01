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

#include "api.hpp"
#include "assert.hpp"
#include "sctf.hpp"

class MyClass
{
    int    m_i = 0;
    double m_d = 1.0;

public:
    void incr() {
        m_i += 1;
    }
    void mult(double m) {
        m_d *= m;
    }

    int i() const {
        return m_i;
    }
    double d() const {
        return m_d;
    }
};

DESCRIBE_PAR("TestMyClass") {
    MyClass fixture_;

    BEFORE_EACH() {
        fixture_.incr();
    };
    AFTER_EACH() {
        fixture_.mult(2.);
    };

    TEST("some test with i") {
        ASSERT(fixture_.i(), GT(), 0);
    };
    TEST("some test with d") {
        ASSERT(fixture_.d(), LT(), 5.0);
    };

    TEST("check epsilon") {
        std::cout << sctf::epsilon << std::flush;
    };
};