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

#ifndef TPP_REPORT_XML_REPORTER_HPP
#define TPP_REPORT_XML_REPORTER_HPP

#include <array>
#include <chrono>
#include <ctime>

#include "report/reporter.hpp"

namespace tpp
{
namespace intern
{
namespace report
{
class xml_reporter : public reporter
{
public:
    static auto
    create(std::ostream& stream_ = std::cout) -> reporter_ptr {
        return std::make_shared<xml_reporter>(enable{}, stream_);
    }

    static auto
    create(std::string const& fname_) -> reporter_ptr {
        return std::make_shared<xml_reporter>(enable{}, fname_);
    }

    /// Constructor for std::make_shared.
    xml_reporter(enable, std::ostream& stream_) : reporter(stream_) {}

    /// Constructor for std::make_shared.
    xml_reporter(enable, std::string const& fname_) : reporter(fname_) {}

private:
    void
    report_testsuite(test::testsuite_ptr const& ts_) override {
        std::time_t           stamp = std::chrono::system_clock::to_time_t(ts_->timestamp());
        std::array<char, 128> buff{};
        std::strftime(buff.data(), 127, "%FT%T", std::localtime(&stamp));
        push_indent();
        newline();
        *this << "<testsuite id=\"" << m_id++ << "\" name=\"" << ts_->name() << "\" errors=\""
              << ts_->statistics().errors() << "\" tests=\"" << ts_->statistics().tests() << "\" failures=\""
              << ts_->statistics().failures() << R"(" skipped="0" time=")" << ts_->statistics().elapsed_time()
              << "\" timestamp=\"" << buff.data() << "\">";

        reporter::report_testsuite(ts_);

        newline();
        *this << "</testsuite>";
        pop_indent();
    }

    void
    report_testcase(test::testcase const& tc_) override {
        push_indent();
        newline();
        *this << "<testcase name=\"" << tc_.name() << "\" classname=\"" << tc_.suite_name() << "\" time=\""
              << tc_.elapsed_time() << "\"";
        if (tc_.result() != test::testcase::HAS_PASSED) {
            auto const unsuccess = [&] { return tc_.result() == test::testcase::HAD_ERROR ? "error" : "failure"; };
            *this << '>';
            push_indent();
            newline();
            *this << '<' << unsuccess() << " message=\"" << tc_.reason() << "\"></" << unsuccess() << '>';
            pop_indent();
            print_system_out(tc_);
            newline();
            *this << "</testcase>";
        } else if (capture()) {
            *this << '>';
            print_system_out(tc_);
            newline();
            *this << "</testcase>";
        } else {
            *this << "/>";
        }
        pop_indent();
    }

    void
    begin_report() override {
        reporter::begin_report();

        *this << R"(<?xml version="1.0" encoding="UTF-8" ?>)";
        newline();
        *this << "<testsuites>";
    }

    void
    end_report() override {
        newline();
        *this << "</testsuites>";
        newline();
    }

    void
    print_system_out(test::testcase const& tc_) {
        if (!capture()) {
            return;
        }
        push_indent();
        newline();
        *this << "<system-out>" << tc_.cout() << "</system-out>";
        newline();
        *this << "<system-err>" << tc_.cerr() << "</system-err>";
        pop_indent();
    }

    std::size_t mutable m_id{0};  ///< Report wide incremental ID for testsuites.
};
}  // namespace report
}  // namespace intern

using xml_reporter = intern::report::xml_reporter;
}  // namespace tpp

#endif  // TPP_REPORT_XML_REPORTER_HPP
