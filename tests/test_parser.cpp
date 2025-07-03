#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Lexer.hpp"
#include "third_party/doctest.h"

TEST_CASE("Testing Lexer and Parser")
{
    std::unordered_map<std::string, std::variant<int, double>> dummy;
    auto expr = from_string("1 + 2 * 3", dummy);
    CHECK(expr.to_string() == "(+ 1 (* 2 3))");

    expr = from_string("1", dummy);
    CHECK(expr.to_string() == "1");

    expr = from_string("a * 2 * b", dummy);
    CHECK(expr.to_string() == "(* (* a 2) b)");

    expr = from_string("a + b * 2 * c + a / 4", dummy);
    CHECK(expr.to_string() == "(+ (+ a (* (* b 2) c)) (/ a 4))");

    expr = from_string("2 + b * 5 - 3 / 5 + 5 -3", dummy);
    CHECK(expr.to_string() == "(- (+ (- (+ 2 (* b 5)) (/ 3 5)) 5) 3)");

    expr = from_string("(2 + b) * 5 ", dummy);
    CHECK(expr.to_string() == "(* (+ 2 b) 5)");

    expr = from_string("(((a)))", dummy);
    CHECK(expr.to_string() == "a");

    expr = from_string("+ 1", dummy);
    CHECK(expr.to_string() == "(+ 1)");

    expr = from_string("--1 * 2", dummy);
    CHECK(expr.to_string() == "(* (- (- 1)) 2)");

    expr = from_string("-(3 + (4 * 6))", dummy);
    CHECK(expr.to_string() == "(- (+ 3 (* 4 6)))");
}
