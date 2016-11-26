//          Copyright Joakim Karlsson & Kim Gräsman 2010-2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <stdexcept>
#include "tests.h"
using namespace snowhouse;


class ClassWithExceptions
{
public:
  int LogicError()
  {
    throw std::logic_error("not logical!");
  }

  double RangeError()
  {
    throw std::range_error("range error!");
  }

  void NoError()
  {
  }
};

void ExceptionTests()
{
  ClassWithExceptions objectUnderTest;

  describe("ExceptionTests");

  it("can detect exceptions");
  {
    AssertThrows(std::exception, objectUnderTest.LogicError());
  }

  it("can assert on last exception");
  {
    AssertThrows(std::logic_error, objectUnderTest.LogicError());
    Assert::That(LastException<std::logic_error>().what(), Contains("not logical!"));
  }

  it("can detect when wrong exception is thrown");
  {
    AssertTestFails(AssertThrows(std::logic_error, objectUnderTest.RangeError()), "Wrong exception");
  }

  it("can print expected exception type when wrong exception is thrown");
  {
    AssertTestFails(AssertThrows(std::logic_error, objectUnderTest.RangeError()), "Expected std::logic_error");
  }

  it("can have several exception assertions in same spec");
  {
    AssertThrows(std::logic_error, objectUnderTest.LogicError());
    Assert::That(LastException<std::logic_error>().what(), Contains("not logical!"));

    AssertThrows(std::range_error, objectUnderTest.RangeError());
    Assert::That(LastException<std::range_error>().what(), Contains("range error!"));
  }

  it("can have several exception assertion for the same exception in same spec");
  {
    AssertThrows(std::logic_error, objectUnderTest.LogicError());
    Assert::That(LastException<std::logic_error>().what(), Contains("not logical!"));

    AssertThrows(std::logic_error, objectUnderTest.LogicError());
    Assert::That(LastException<std::logic_error>().what(), Contains("not logical!"));
  }

  it("can detect when no exception is thrown");
  {
    AssertTestFails(AssertThrows(std::logic_error, objectUnderTest.NoError()), "No exception");
  }

  it("can print expected exception when no exception is thrown");
  {
    AssertTestFails(AssertThrows(std::logic_error, objectUnderTest.NoError()), "Expected std::logic_error");
  }

  it("destroys exceptions when out-of-scope");
  {
    {
      AssertThrows(std::logic_error, objectUnderTest.LogicError());
    }
    AssertThrows(AssertionException, LastException<std::logic_error>());
    Assert::That(LastException<AssertionException>().GetMessage(), Contains("No exception was stored"));
  }
}
