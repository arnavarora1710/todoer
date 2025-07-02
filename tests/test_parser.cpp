#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Lexer.hpp"
#include "third_party/doctest.h"

TEST_CASE("Testing Lexer and Parser")
{
    auto expr = from_string("1 + 2 * 3");
    CHECK(expr.to_string() == "(+ 1 (* 2 3))");

    expr = from_string("1");
    CHECK(expr.to_string() == "1");

    expr = from_string("a * 2 * b");
    CHECK(expr.to_string() == "(* (* a 2) b)");

    expr = from_string("a + b * 2 * c + a / 4");
    CHECK(expr.to_string() == "(+ (+ a (* (* b 2) c)) (/ a 4))");

    expr = from_string("2 + b * 5 - 3 / 5 + 5 -3");
    CHECK(expr.to_string() == "(- (+ (- (+ 2 (* b 5)) (/ 3 5)) 5) 3)");

    expr = from_string("(2 + b) * 5 ");
    CHECK(expr.to_string() == "(* (+ 2 b) 5)");

    expr = from_string("(((a)))");
    CHECK(expr.to_string() == "a");
}
