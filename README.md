# simple-cpp-test-framework

[![Build Status](https://travis-ci.org/Jarthianur/simple-cpp-test-framework.svg?branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![Linux Build Status](http://badges.herokuapp.com/travis/Jarthianur/simple-cpp-test-framework?env=BADGE=linux&label=linux&branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![OSX Build Status](http://badges.herokuapp.com/travis/Jarthianur/simple-cpp-test-framework?env=BADGE=osx&label=osx&branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![Windows Build Status](http://badges.herokuapp.com/travis/Jarthianur/simple-cpp-test-framework?env=BADGE=windows&label=windows&branch=master)](https://travis-ci.org/Jarthianur/simple-cpp-test-framework)
[![codecov](https://codecov.io/gh/Jarthianur/simple-cpp-test-framework/branch/master/graph/badge.svg)](https://codecov.io/gh/Jarthianur/simple-cpp-test-framework)
[![BCH compliance](https://bettercodehub.com/edge/badge/Jarthianur/simple-cpp-test-framework?branch=master)](https://bettercodehub.com/)

**This is a simple header-only testing framework for C++11/14/17 featuring a simple, yet powerfull API, and the capability to parallelize tests, using *OpenMP*.**

To use it, just include the all in one [header](https://github.com/Jarthianur/simple-cpp-test-framework/releases/download/1.2/sctf.hpp) into your builds.

*So why actually writing a new testing framework?*

There are great C++ testing frameworks available out there, but each has its pro's and con's.
When I started to write proper tests for my projects, I couldn't find a testing framework that completely suited my needs.
So I thought, why not just writing my own, which simply has every feature I'd like.
As a side effect this project was - and is still - a great opportunity to improve my C++ skills.
This framework is designed around simplicity and effectiveness, for the user as well as the frameworks code itself.
There is no excessive documentation reading required, or complicated build integration.
Everything is hidden that distracts you from focusing on what really counts, writing qualitative tests.
Of course this framework might not suite all *your* needs, hence feel free to provide suggestions, or request a feature you'd wish.
Please have a look at the full [feature set](#feature-set).

## Contents

- [simple-cpp-test-framework](#simple-cpp-test-framework)
  - [Contents](#contents)
  - [Feature Set](#feature-set)
  - [Usage](#usage)
    - [Test Styles](#test-styles)
    - [Scopes and Fixtures](#scopes-and-fixtures)
    - [Floating Point Numbers](#floating-point-numbers)
    - [Examples](#examples)
      - [Simple Unit Test](#simple-unit-test)
      - [Behavior Driven Test](#behavior-driven-test)
  - [API](#api)
    - [Tests](#tests)
    - [Reporters](#reporters)
    - [Comparators](#comparators)
    - [Assertions](#assertions)
  - [Parallelization Of Tests](#parallelization-of-tests)
  - [How To Extend](#how-to-extend)
    - [Reporters](#reporters-1)
    - [Comparators](#comparators-1)
    - [Value Stringification](#value-stringification)
  - [Contributing](#contributing)

## Feature Set

As a short summary of all features, have a look at this list.

+ Single header file for inclusion
+ Self registering test suites
+ Comparator based assertions
  + equals
  + unequals
  + less than
  + greater than
  + in range (substring, contains)
+ Report generation in different formats
  + JUnit XML
  + markdown
  + console output
+ **Multithreaded test execution with OpenMP**
+ **Output capturing per testcase (even when multithreaded)**
+ Unit and behavior-driven test styles
+ Compatible compilers
  + gcc
  + clang
  + msvc

## Usage

Just inlude the *sctf.hpp* header file into your build.
Tests can then be written in source files and simply linked into your test binary.
If you prefer, you can of course write tests in header files and include them in your main source file.
*Note that it is not possible to write multiple testsuites, or tests in one line, as it would break name generation.*
To run and report all tests, just create a reporter and call its `report()` function in your `main()`, as seen in the examples.
Alternatively invoke the `SCTF_DEFAULT_MAIN(...)` macro in one of your source files and pass it a call to any reporter factory method.
In order to run tests in multiple threads you have to enable OpenMP at compilation (e.g. for gcc add `-fopenmp` flag).
Every output to stdout, or stderr from inside tests is captured per testcase, and can be included in the report.

### Test Styles

Basically two approaches of writing tests exist.
One is the classic unit test style, where tests are generally designed around an implementation.
The other is behavior driven test style (BDD), where tests are designed around the behavior of an implementation.
No matter which of them you prefer, this framework serves both.
Actually both styles require just a little different syntax.
In the end `DESCRIBE` is just an other macro for `SUITE`, same with `IT` and `TEST`.
If you'd wish to name them differently, just define your own wrapper macro for them with whatever name you like.

### Scopes and Fixtures

A testsuite is nothing else than a class definition under the hood, which is instantiated with static lifetime.
Hence the same scoping rules as for usual class definitions apply to testsuites.
Testcases are member functions of their testsuite.
Hence the usual scoping rules for methods apply to them.

You might know the need for fixtures, basically an instance of a unit under test (UUT), in other contexts.
It is up to you to decide whether testcases should be isolated, or run against a designated fixture.
This framework supports the usage of fixtures, as you can just declare any object at testsuite scope - remember, it's just a member field in the end.
Also it is possible to define functions that will be executed once before and after all testcases, and before and after each testcase.
But be carefull when you use these features in multithreaded tests, as there is no additional synchronization happening.
Have a look at the examples, or the [API](#api) to see how this is done exactly.

The use of a testsuite wide UUT instance might also be usefull, if it is expensive to construct and initiate.

### Floating Point Numbers

As floating-point comparison relies on a so called epsilon, we use the machine epsilon by default.
But this may lead into false-negative test results, as it could be too accurate.
In order to use a custom epsilon, there are two ways to achieve this.
First, you can provide a macro in each compilation unit, which is called `SCTF_EPSILON` with a satisfying value like `0.000001`.
A compiler invocation could look like `g++ -std=c++0x test.cpp -DSCTF_EPSILON=0.000001`.
Or you provide it before including *sctf.hpp* like in the example below.
The second way, is to provide it as an extern variable.
Therefor define `SCTF_EXTERN_EPSILON` before including *sctf.hpp* and then invoke the `SCTF_SET_EPSILON` macro with a satisfying value in the source file.

### Examples

#### Simple Unit Test

```cpp
#define SCTF_EXTERN_EPSILON
#include "sctf.hpp"
SCTF_SET_EPSILON(0.001)
SCTF_DEFAULT_MAIN(create_xml_reporter())

SUITE("testSomething") {
    TEST("abc") {
        ASSERT(x+1, EQ, 11);
        ASSERT_FALSE(false);
        ASSERT("xyz"s, !IN, "hello"s);
        int i = 101;
        ASSERT_NOT_NULL(&i);
    }
    TEST("multiple numbers") {
        ASSERT_EQUALS(0, 0);
        ASSERT_ZERO(0.0);
        ASSERT_TRUE(true);
        ASSERT_NOT(.0, EQUALS, 1.0);
        ASSERT(6, IN_RANGE, std::pair<int, int>(1, 5));
    }
    TEST("ranges") {
        ASSERT('w', IN, std::string("world"));
        ASSERT(std::string(""), IN, std::vector<std::string>{""});
    }
    TEST("exceptions") {
        ASSERT_THROWS(throw std::logic_error(""), std::logic_error);
    }
};
```

#### Behavior Driven Test

```cpp
#define SCTF_EPSILON 0.00001
#include "sctf.hpp"

int main(int argc, char** argv) {
    return sctf::create_markdown_reporter("results.md")->report();
}

class MyClass {
    public:
        int i = 0;
        bool function() { return true; }
};

DESCRIBE("testMyClass") {
    MyClass my;

    SETUP() {
        my.i = 1;
    }
    AFTER_EACH() {
        my.i++;
    }

    IT("should return true") {
        ASSERT_TRUE(my.function());
    }
    IT("should hold i larger than 0") {
        ASSERT(my.i, GT, 0);
    }
};
```

## API

### Tests

| Macro                   | Arguments   | Description                                                                                              |
| ----------------------- | ----------- | -------------------------------------------------------------------------------------------------------- |
| SUITE, DESCRIBE         | description | Create a testsuite.                                                                                      |
| SUITE_PAR, DESCRIBE_PAR | description | Create a testsuite, where all tests will get executed concurrently in multiple threads.                  |
| TEST, IT                | description | Create a testcase in a testsuite.                                                                        |
| SETUP                   |             | Define a function, which will be executed once before all testcases. Thrown exceptions will get ignored. |
| TEARDOWN                |             | Define a function, which will be executed once after all testcases. Thrown exceptions will get ignored.  |
| BEFORE_EACH             |             | Define a function, which will be executed before each testcase. Thrown exceptions will get ignored.      |
| AFTER_EACH              |             | Define a function, which will be executed after each testcase. Thrown exceptions will get ignored.       |

### Reporters

| Format                      | Factory Method           | Arguments (default)                                                       |
| --------------------------- | ------------------------ | ------------------------------------------------------------------------- |
| Console oriented plain text | create_console_reporter  | output stream/file (stdout), ANSI colors (false), captured output (false) |
| JUnit XML                   | create_xml_reporter      | output stream/file (stdout), captured output (false)                      |
| Markdown                    | create_markdown_reporter | output stream/file (stdout), captured output (false)                      |

### Comparators

**Note:** While the *comparator* names the function itself, the *shortwrite* is what you actually write in code. Assertions are based on comparators. Every comparator provides a negation operator `!`, which allows the logical negation of the actual comparison. For example `ASSERT(1, !EQ, 1)` and `ASSERT(1, NE, 1)` are logically equivalent.

| Comparator   | Shortwrites               | Description                                                                                                                                          |
| ------------ | ------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| equals       | EQUALS, EQ                | Compare two values to be equal.                                                                                                                      |
| unequals     | UNEQUALS, NE              | Compare two values to be not equal.                                                                                                                  |
| greater_than | GREATER_THAN, GREATER, GT | Compare one value to be greater than another.                                                                                                        |
| less_than    | LESS_THAN, LESS, LT       | Compare one value to be less than another.                                                                                                           |
| in_range     | IN_RANGE, IN              | Check a value to be in range of any container. This means for containers (C++ "ranges") to contain the value and for strings to contain a substring. |

### Assertions

| Assertion       | Parameters          | Description                                                                                                                                                  |
| --------------- | ------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| ASSERT          | VALUE, COMP, EXPECT | Assert successfull comparison of VALUE and EXPECT with COMP.                                                                                                 |
| ASSERT_NOT      | VALUE, COMP, EXPECT | Assert with negated comparison. Equivalent to `!COMP`.                                                                                                       |
| ASSERT_EQ       | VALUE, EXPECT       | Assert using *EQUALS* comparator.                                                                                                                            |
| ASSERT_TRUE     | VALUE               | Assert VALUE to be *true*.                                                                                                                                   |
| ASSERT_FALSE    | VALUE               | Assert VALUE to be *false*.                                                                                                                                  |
| ASSERT_NOT_NULL | VALUE               | Assert VALUE to be not *nullptr*.                                                                                                                            |
| ASSERT_NULL     | VALUE               | Assert VALUE to be *nullptr*.                                                                                                                                |
| ASSERT_ZERO     | VALUE               | Assert VALUE to be *0*.                                                                                                                                      |
| ASSERT_THROWS   | STMT, TYPE          | Assert STMT to throw an exception of TYPE. Multiple statements can be split by `;`.                                                                          |
| ASSERT_NOTHROW  | STMT                | Assert STMT not to throw any exception.  Multiple statements can be split by `;`.                                                                            |
| ASSERT_RUNTIME  | STMT, MILLIS        | Assert STMT to run in MILLIS milliseconds at maximum. The statement is not interrupted, if the time exceeds MILLIS. Multiple statements can be split by `;`. |

## Parallelization Of Tests

This testing framework serves the capability of parallelizing tests, using OpenMP. Actually it is not really parallel, but concurrent.
Nevertheless, it may reduce test durations massively.
Keep in mind that tests, running concurrently, *must* be completely independent from each other.
The same rules for usual multithreading apply here, to not produce dataraces, or deadlocks.
As long as testcases do not share any data, it is completely threadsafe.
If testcases share data, like fixtures, you have to take care of synchronization, meaning that all invokations on your shared data needs to be threadsafe.
This also applies to `BEFORE_EACH` and `AFTER_EACH` definitions, while `SETUP` and `TEARDOWN` are executed synchronous.
You may also use parallel tests to test a components threadsafety - actually not a proper way of testing threadsafety.
Also consider, spawning threads has some overhead.
Thus there is no point in running just a few, anyway fast tests concurrently.
*Usually* the threadpool is kept alive in the background.
So if you use parallel testsuites once, don't be afraid to use them wherever you can, even for short tests, as there is not much more overhead.

## How To Extend

### Reporters

To add a new reporter just implement the [reporter](include/reporter/reporter.hpp) interface. Have a look at the preimplemented reporters, how this is exactly done.

### Comparators

To add a new comparator you basically just need to invoke two macros.
For example:

```c
COMPARATOR(pred, "predecessor of", value + 1 == expect)
PROVIDE_COMPARATOR(pred, PRE)
```

This will create a generic comparator for you. The syntax is as follows:
`COMPARATOR(NAME, CONSTRAINT, PREDICATE)`, where *NAME* is the comparator name.
*CONSTRAINT* is a cstring representing what it does and is used for reporting.
*PREDICATE* is the actual comparison.
If you need some more complex comparators, have a look at the [in_range](include/comparator/inrange.hpp) to see how it is implemented.

### Value Stringification

To allow proper reporting, every asserted value/type needs to be stringified somehow.
This framework uses an approach to convert every value to string if possible, else the typename is used.
If you need to handle custom types in a certain way, just specialize the [to_string](include/common/stringify.hpp) template.

## Contributing

Contribution to this project is always welcome.
