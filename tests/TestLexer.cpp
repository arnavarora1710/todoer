#include "Interpreter.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Lexer.hpp"
#include "third_party/doctest.h"

using VariableMap = std::unordered_map<std::string, std::variant<int, double>>;

TEST_CASE("Testing Basic Lexer and Parser")
{
    VariableMap dummy;

    SUBCASE("Simple Expressions")
    {
        auto expr = from_string("1 + 2 * 3", dummy);
        CHECK(expr->to_string() == "(+ 1 (* 2 3))");

        expr = from_string("1", dummy);
        CHECK(expr->to_string() == "1");

        expr = from_string("a * 2 * b", dummy);
        CHECK(expr->to_string() == "(* (* a 2) b)");
    }

    SUBCASE("Complex Precedence")
    {
        auto expr = from_string("a + b * 2 * c + a / 4", dummy);
        CHECK(expr->to_string() == "(+ (+ a (* (* b 2) c)) (/ a 4))");

        expr = from_string("2 + b * 5 - 3 / 5 + 5 -3", dummy);
        CHECK(expr->to_string() == "(- (+ (- (+ 2 (* b 5)) (/ 3 5)) 5) 3)");
    }

    SUBCASE("Parentheses")
    {
        auto expr = from_string("(2 + b) * 5 ", dummy);
        CHECK(expr->to_string() == "(* (+ 2 b) 5)");

        expr = from_string("(((a)))", dummy);
        CHECK(expr->to_string() == "a");
    }

    SUBCASE("Unary Operators")
    {
        auto expr = from_string("+ 1", dummy);
        CHECK(expr->to_string() == "(+ 1)");

        expr = from_string("--1 * 2", dummy);
        CHECK(expr->to_string() == "(* (- (- 1)) 2)");

        expr = from_string("-(3 + (4 * 6))", dummy);
        CHECK(expr->to_string() == "(- (+ 3 (* 4 6)))");
    }
}

TEST_CASE("Testing Operator Precedence and Associativity")
{
    VariableMap dummy;

    SUBCASE("Multiplication and Division Precedence")
    {
        auto expr = from_string("1 + 2 * 3 / 4", dummy);
        CHECK(expr->to_string() == "(+ 1 (/ (* 2 3) 4))");

        expr = from_string("1 * 2 + 3 * 4", dummy);
        CHECK(expr->to_string() == "(+ (* 1 2) (* 3 4))");

        expr = from_string("1 / 2 * 3 / 4", dummy);
        CHECK(expr->to_string() == "(/ (* (/ 1 2) 3) 4)");
    }

    SUBCASE("Addition and Subtraction Associativity")
    {
        auto expr = from_string("1 + 2 + 3 + 4", dummy);
        CHECK(expr->to_string() == "(+ (+ (+ 1 2) 3) 4)");

        expr = from_string("10 - 5 - 2 - 1", dummy);
        CHECK(expr->to_string() == "(- (- (- 10 5) 2) 1)");

        expr = from_string("1 + 2 - 3 + 4", dummy);
        CHECK(expr->to_string() == "(+ (- (+ 1 2) 3) 4)");
    }

    SUBCASE("Mixed Operators")
    {
        auto expr = from_string("1 + 2 * 3 - 4 / 2", dummy);
        CHECK(expr->to_string() == "(- (+ 1 (* 2 3)) (/ 4 2))");

        expr = from_string("a * b + c / d - e", dummy);
        CHECK(expr->to_string() == "(- (+ (* a b) (/ c d)) e)");
    }
}

TEST_CASE("Testing Complex Parentheses")
{
    VariableMap dummy;

    SUBCASE("Nested Parentheses")
    {
        auto expr = from_string("((1 + 2) * (3 + 4))", dummy);
        CHECK(expr->to_string() == "(* (+ 1 2) (+ 3 4))");

        expr = from_string("(((1 + 2) * 3) + 4)", dummy);
        CHECK(expr->to_string() == "(+ (* (+ 1 2) 3) 4)");

        expr = from_string("1 + ((2 + 3) * 4)", dummy);
        CHECK(expr->to_string() == "(+ 1 (* (+ 2 3) 4))");
    }

    SUBCASE("Deep Nesting")
    {
        auto expr = from_string("(((((1)))))", dummy);
        CHECK(expr->to_string() == "1");

        expr = from_string("((a + b) * ((c - d) / e))", dummy);
        CHECK(expr->to_string() == "(* (+ a b) (/ (- c d) e))");

        expr = from_string("(1 + (2 * (3 + (4 / 5))))", dummy);
        CHECK(expr->to_string() == "(+ 1 (* 2 (+ 3 (/ 4 5))))");
    }

    SUBCASE("Parentheses with Unary Operators")
    {
        auto expr = from_string("-(a + b)", dummy);
        CHECK(expr->to_string() == "(- (+ a b))");

        expr = from_string("(-a + b)", dummy);
        CHECK(expr->to_string() == "(+ (- a) b)");

        expr = from_string("-(a * -b)", dummy);
        CHECK(expr->to_string() == "(- (* a (- b)))");
    }
}

TEST_CASE("Testing Variable Names and Numbers")
{
    VariableMap dummy;

    SUBCASE("Different Variable Names")
    {
        auto expr = from_string("x + y", dummy);
        CHECK(expr->to_string() == "(+ x y)");

        expr = from_string("variable1 * variable2", dummy);
        CHECK(expr->to_string() == "(* variable1 variable2)");

        expr = from_string("a1 + b2 + c3", dummy);
        CHECK(expr->to_string() == "(+ (+ a1 b2) c3)");
    }

    SUBCASE("Floating Point Numbers")
    {
        auto expr = from_string("1.5 + 2.7", dummy);
        CHECK(expr->to_string() == "(+ 1.5 2.7)");

        expr = from_string("3.14159 * radius", dummy);
        CHECK(expr->to_string() == "(* 3.14159 radius)");

        expr = from_string("0.001 + 1000.999", dummy);
        CHECK(expr->to_string() == "(+ 0.001 1000.999)");
    }

    SUBCASE("Large Numbers")
    {
        auto expr = from_string("1000000 + 1", dummy);
        CHECK(expr->to_string() == "(+ 1000000 1)");

        expr = from_string("999999999 * 2", dummy);
        CHECK(expr->to_string() == "(* 999999999 2)");
    }
}

TEST_CASE("Testing Unary Operators in Various Contexts")
{
    VariableMap dummy;

    SUBCASE("Unary Plus and Minus")
    {
        auto expr = from_string("-1", dummy);
        CHECK(expr->to_string() == "(- 1)");

        expr = from_string("+1", dummy);
        CHECK(expr->to_string() == "(+ 1)");

        expr = from_string("-x", dummy);
        CHECK(expr->to_string() == "(- x)");

        expr = from_string("+variable", dummy);
        CHECK(expr->to_string() == "(+ variable)");
    }

    SUBCASE("Multiple Unary Operators")
    {
        auto expr = from_string("--x", dummy);
        CHECK(expr->to_string() == "(- (- x))");

        expr = from_string("+-1", dummy);
        CHECK(expr->to_string() == "(+ (- 1))");

        expr = from_string("-+x", dummy);
        CHECK(expr->to_string() == "(- (+ x))");

        expr = from_string("---5", dummy);
        CHECK(expr->to_string() == "(- (- (- 5)))");
    }

    SUBCASE("Unary in Expressions")
    {
        auto expr = from_string("1 + -2", dummy);
        CHECK(expr->to_string() == "(+ 1 (- 2))");

        expr = from_string("-1 * +2", dummy);
        CHECK(expr->to_string() == "(* (- 1) (+ 2))");

        expr = from_string("a + -b * c", dummy);
        CHECK(expr->to_string() == "(+ a (* (- b) c))");

        expr = from_string("-a + -b", dummy);
        CHECK(expr->to_string() == "(+ (- a) (- b))");
    }
}

TEST_CASE("Testing Complex Expressions")
{
    VariableMap dummy;

    SUBCASE("Long Expression Chains")
    {
        auto expr = from_string("a + b + c + d + e", dummy);
        CHECK(expr->to_string() == "(+ (+ (+ (+ a b) c) d) e)");

        expr = from_string("1 * 2 * 3 * 4 * 5", dummy);
        CHECK(expr->to_string() == "(* (* (* (* 1 2) 3) 4) 5)");

        expr = from_string("x / y / z / w", dummy);
        CHECK(expr->to_string() == "(/ (/ (/ x y) z) w)");
    }

    SUBCASE("Mixed Operations with Variables")
    {
        auto expr = from_string("x * y + z / w - a + b", dummy);
        CHECK(expr->to_string() == "(+ (- (+ (* x y) (/ z w)) a) b)");

        expr = from_string("(a + b) * (c - d) / (e + f)", dummy);
        CHECK(expr->to_string() == "(/ (* (+ a b) (- c d)) (+ e f))");

        expr = from_string("a * (b + c * d) - e / (f - g)", dummy);
        CHECK(expr->to_string() == "(- (* a (+ b (* c d))) (/ e (- f g)))");
    }

    SUBCASE("Scientific-like Expressions")
    {
        auto expr = from_string("mass * velocity * velocity / 2", dummy);
        CHECK(expr->to_string() == "(/ (* (* mass velocity) velocity) 2)");

        expr = from_string("4 * pi * radius * radius", dummy);
        CHECK(expr->to_string() == "(* (* (* 4 pi) radius) radius)");

        expr = from_string("force * distance * cos_angle", dummy);
        CHECK(expr->to_string() == "(* (* force distance) cos_angle)");
    }
}

TEST_CASE("Testing Edge Cases")
{
    VariableMap dummy;

    SUBCASE("Single Tokens")
    {
        auto expr = from_string("42", dummy);
        CHECK(expr->to_string() == "42");

        expr = from_string("variable", dummy);
        CHECK(expr->to_string() == "variable");

        expr = from_string("3.14159", dummy);
        CHECK(expr->to_string() == "3.14159");
    }

    SUBCASE("Zero in Expressions")
    {
        auto expr = from_string("0 + x", dummy);
        CHECK(expr->to_string() == "(+ 0 x)");

        expr = from_string("x * 0", dummy);
        CHECK(expr->to_string() == "(* x 0)");

        expr = from_string("0 / 1", dummy);
        CHECK(expr->to_string() == "(/ 0 1)");
    }

    SUBCASE("Redundant Parentheses")
    {
        auto expr = from_string("((((x))))", dummy);
        CHECK(expr->to_string() == "x");

        expr = from_string("(1) + (2)", dummy);
        CHECK(expr->to_string() == "(+ 1 2)");

        expr = from_string("((a + b)) * ((c))", dummy);
        CHECK(expr->to_string() == "(* (+ a b) c)");
    }
}
