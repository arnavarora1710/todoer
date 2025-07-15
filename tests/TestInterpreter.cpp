#include "third_party/doctest.h"
#include "third_party/calculator.hpp"
#include "Interpreter.hpp"
#include <chrono>
#include <sstream>
#include <iomanip>

TEST_CASE("Testing Basic Arithmetic")
{
    VariableMap variables;

    SUBCASE("Addition")
    {
        CHECK(Interpreter::interpret("1 + 2", variables) == "3");
        CHECK(Interpreter::interpret("5 + 7", variables) == "12");
        CHECK(Interpreter::interpret("0 + 0", variables) == "0");
        CHECK(Interpreter::interpret("1.5 + 2.5", variables) == "4.000000");
    }

    SUBCASE("Subtraction")
    {
        CHECK(Interpreter::interpret("5 - 2", variables) == "3");
        CHECK(Interpreter::interpret("10 - 7", variables) == "3");
        CHECK(Interpreter::interpret("0 - 5", variables) == "-5");
        CHECK(Interpreter::interpret("3.7 - 1.2", variables) == "2.500000");
    }

    SUBCASE("Multiplication")
    {
        CHECK(Interpreter::interpret("3 * 4", variables) == "12");
        CHECK(Interpreter::interpret("2 * 0", variables) == "0");
        CHECK(Interpreter::interpret("1.5 * 2", variables) == "3.000000");
        CHECK(Interpreter::interpret("2.5 * 3.2", variables) == "8.000000");
    }

    SUBCASE("Division")
    {
        CHECK(Interpreter::interpret("10 / 2", variables) == "5");
        CHECK(Interpreter::interpret("15 / 3", variables) == "5");
        CHECK(Interpreter::interpret("7.5 / 2.5", variables) == "3.000000");
        CHECK(Interpreter::interpret("1 / 3", variables) == "0"); // Integer division
    }
}

TEST_CASE("Testing Variables")
{
    VariableMap variables;

    SUBCASE("Single Variable Operations")
    {
        variables["x"] = 10;
        CHECK(Interpreter::interpret("x + 5", variables) == "15");
        CHECK(Interpreter::interpret("x - 3", variables) == "7");
        CHECK(Interpreter::interpret("x * 2", variables) == "20");
        CHECK(Interpreter::interpret("x / 2", variables) == "5");
    }

    SUBCASE("Multiple Variable Operations")
    {
        variables["a"] = 5;
        variables["b"] = 3;
        variables["c"] = 2;

        CHECK(Interpreter::interpret("a + b", variables) == "8");
        CHECK(Interpreter::interpret("a - b", variables) == "2");
        CHECK(Interpreter::interpret("a * b", variables) == "15");
        CHECK(Interpreter::interpret("a / b", variables) == "1");
        CHECK(Interpreter::interpret("a + b + c", variables) == "10");
        CHECK(Interpreter::interpret("a * b + c", variables) == "17");
        CHECK(Interpreter::interpret("a + b * c", variables) == "11");
    }

    SUBCASE("Floating Point Variables")
    {
        variables["pi"] = 3.14159;
        variables["e"] = 2.71828;

        CHECK(Interpreter::interpret("pi + e", variables) == "5.859870");
        CHECK(Interpreter::interpret("pi * 2", variables) == "6.283180");
    }
}

TEST_CASE("Testing Operator Precedence")
{
    VariableMap variables;

    CHECK(Interpreter::interpret("1 + 2 * 3", variables) == "7");
    CHECK(Interpreter::interpret("2 * 3 + 1", variables) == "7");
    CHECK(Interpreter::interpret("10 - 2 * 3", variables) == "4");
    CHECK(Interpreter::interpret("2 * 3 * 4", variables) == "24");
    CHECK(Interpreter::interpret("20 / 4 / 2", variables) == "2"); // Left associative
    CHECK(Interpreter::interpret("1 + 2 * 3 + 4", variables) == "11");
    CHECK(Interpreter::interpret("1 * 2 + 3 * 4", variables) == "14");
}

TEST_CASE("Testing Parentheses")
{
    VariableMap variables;

    CHECK(Interpreter::interpret("(1 + 2) * 3", variables) == "9");
    CHECK(Interpreter::interpret("1 + (2 * 3)", variables) == "7");
    CHECK(Interpreter::interpret("(10 - 2) / 2", variables) == "4");
    CHECK(Interpreter::interpret("((1 + 2) * 3) + 4", variables) == "13");
    CHECK(Interpreter::interpret("1 + ((2 + 3) * 4)", variables) == "21");
    CHECK(Interpreter::interpret("(1 + 2) * (3 + 4)", variables) == "21");
}

TEST_CASE("Testing Negative Numbers")
{
    VariableMap variables;

    CHECK(Interpreter::interpret("-5", variables) == "-5");
    CHECK(Interpreter::interpret("-5 + 3", variables) == "-2");
    CHECK(Interpreter::interpret("5 + (-3)", variables) == "2");
    CHECK(Interpreter::interpret("-5 * -3", variables) == "15");
    CHECK(Interpreter::interpret("-(5 + 3)", variables) == "-8");

    variables["neg"] = -10;
    CHECK(Interpreter::interpret("neg + 5", variables) == "-5");
    CHECK(Interpreter::interpret("-neg", variables) == "10");
}

TEST_CASE("Testing Complex Expressions")
{
    VariableMap variables;
    variables["x"] = 2;
    variables["y"] = 3;
    variables["z"] = 4;

    CHECK(Interpreter::interpret("x * y + z", variables) == "10");
    CHECK(Interpreter::interpret("(x + y) * z", variables) == "20");
    CHECK(Interpreter::interpret("x * (y + z)", variables) == "14");
    CHECK(Interpreter::interpret("x + y * z - 1", variables) == "13");
    CHECK(Interpreter::interpret("(x + y) * (z - 1)", variables) == "15");
    CHECK(Interpreter::interpret("x * y * z + x + y + z", variables) == "33");
}

TEST_CASE("Testing Edge Cases")
{
    VariableMap variables;

    SUBCASE("Zero Operations")
    {
        CHECK(Interpreter::interpret("0", variables) == "0");
        CHECK(Interpreter::interpret("0 + 0", variables) == "0");
        CHECK(Interpreter::interpret("0 * 100", variables) == "0");
        CHECK(Interpreter::interpret("100 * 0", variables) == "0");
    }

    SUBCASE("Large Numbers")
    {
        CHECK(Interpreter::interpret("1000000 + 1", variables) == "1000001");
        CHECK(Interpreter::interpret("999 * 1000", variables) == "999000");
    }

    SUBCASE("Many Decimal Places")
    {
        CHECK(Interpreter::interpret("1.23456789 + 2.87654321", variables) == "4.111111");
        CHECK(Interpreter::interpret("3.14159 * 2.71828", variables) == "8.539721");
    }
}

TEST_CASE("Testing Long Expressions")
{
    VariableMap variables;

    // Original tests maintained
    CHECK(Interpreter::interpret("1 + 2.2 * 3 + 4 + 5 + 6", variables) == "22.600000");

    // Additional long expressions
    CHECK(Interpreter::interpret("1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10", variables) == "55");
    CHECK(Interpreter::interpret("1 * 2 * 3 * 4 * 5", variables) == "120");
    CHECK(Interpreter::interpret("100 - 1 - 2 - 3 - 4 - 5", variables) == "85");

    variables["a"] = 1;
    variables["b"] = 2;
    variables["c"] = 3;
    variables["d"] = 4;
    variables["e"] = 5;
    CHECK(Interpreter::interpret("a + b + c + d + e", variables) == "15");
    CHECK(Interpreter::interpret("a * b * c * d * e", variables) == "120");
}

TEST_CASE("Performance Benchmarks")
{
    VariableMap variables;

    SUBCASE("Small Expression Benchmark")
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            Interpreter::interpret_numeric("1 + 2 * 3", variables, Mode::Serial);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::stringstream strSerial;
        strSerial << std::fixed << std::setprecision(5) << "1000 small expressions (serial): "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            Interpreter::interpret_numeric("1 + 2 * 3", variables, Mode::Parallel);
        }
        end = std::chrono::high_resolution_clock::now();
        std::stringstream strParallel;
        strParallel << std::fixed << std::setprecision(5) << "1000 small expressions (parallel): "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            calculator::eval("1 + 2 * 3");
        }
        end = std::chrono::high_resolution_clock::now();
        std::stringstream strHeaderOnlyCalc;
        strHeaderOnlyCalc << std::fixed << std::setprecision(5) << "1000 small expressions (header-only calculator): "
                          << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";
        MESSAGE(strSerial.str());
        MESSAGE(strParallel.str());
        MESSAGE(strHeaderOnlyCalc.str());
    }

    SUBCASE("Medium Expression Benchmark")
    {
        std::string medium_expr = "1 + 2 * 3 - 4 / 2 + 5 * (6 - 7) + 8";
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 10000; i++)
        {
            Interpreter::interpret_numeric(medium_expr, variables, Mode::Serial);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::stringstream strSerial;
        strSerial << std::fixed << std::setprecision(5) << "10000 medium expressions (serial): "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 10000; i++)
        {
            Interpreter::interpret_numeric(medium_expr, variables, Mode::Parallel);
        }
        end = std::chrono::high_resolution_clock::now();
        std::stringstream strParallel;
        strParallel << std::fixed << std::setprecision(5) << "10000 medium expressions (parallel): "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 10000; i++)
        {
            calculator::eval(medium_expr);
        }
        end = std::chrono::high_resolution_clock::now();
        std::stringstream strHeaderOnlyCalc;
        strHeaderOnlyCalc << std::fixed << std::setprecision(5) << "10000 medium expressions (header-only calculator): "
                          << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";
        MESSAGE(strSerial.str());
        MESSAGE(strParallel.str());
        MESSAGE(strHeaderOnlyCalc.str());
    }

    SUBCASE("Variable-Heavy Expression Benchmark")
    {
        variables["a"] = 1;
        variables["b"] = 2;
        variables["c"] = 3;
        variables["d"] = 4;
        variables["e"] = 5;
        variables["f"] = 6;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            Interpreter::interpret_numeric("a + b * c - d / e + f", variables, Mode::Serial);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::stringstream strSerial;
        strSerial << std::fixed << std::setprecision(5) << "1000 variable expressions (serial): "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            Interpreter::interpret_numeric("a + b * c - d / e + f", variables, Mode::Parallel);
        }
        end = std::chrono::high_resolution_clock::now();
        std::stringstream strParallel;
        strParallel << std::fixed << std::setprecision(5) << "1000 variable expressions (parallel): "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";
        MESSAGE(strSerial.str());
        MESSAGE(strParallel.str());
    }

    SUBCASE("Large Addition Chain Benchmark")
    {
        std::string big_input;
        for (int i = 0; i < 1000; i++)
        {
            big_input += "1 + ";
        }
        big_input += "1";

        auto start = std::chrono::high_resolution_clock::now();
        Interpreter::interpret_numeric(big_input, variables, Mode::Serial);
        auto end = std::chrono::high_resolution_clock::now();
        std::stringstream strSerial;
        strSerial << std::fixed << std::setprecision(5) << "1000-term addition (serial): "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";

        start = std::chrono::high_resolution_clock::now();
        Interpreter::interpret_numeric(big_input, variables, Mode::Parallel);
        end = std::chrono::high_resolution_clock::now();
        std::stringstream strParallel;
        strParallel << std::fixed << std::setprecision(5) << "1000-term addition (parallel): "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            calculator::eval(big_input);
        }
        end = std::chrono::high_resolution_clock::now();
        std::stringstream strHeaderOnlyCalc;
        strHeaderOnlyCalc << std::fixed << std::setprecision(5) << "1000-term addition (header-only calculator): "
                          << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";
        MESSAGE(strSerial.str());
        MESSAGE(strParallel.str());
        MESSAGE(strHeaderOnlyCalc.str());
    }

    SUBCASE("Deep Parentheses Benchmark")
    {
        std::string deep_expr = "";
        for (int i = 0; i < 100; i++)
        {
            deep_expr += "(";
        }
        deep_expr += "1";
        for (int i = 0; i < 100; i++)
        {
            deep_expr += " + 1)";
        }

        auto start = std::chrono::high_resolution_clock::now();
        Interpreter::interpret_numeric(deep_expr, variables, Mode::Serial);
        auto end = std::chrono::high_resolution_clock::now();
        std::stringstream strSerial;
        strSerial << std::fixed << std::setprecision(5) << "Deep parentheses (100 levels) (serial): "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";

        start = std::chrono::high_resolution_clock::now();
        Interpreter::interpret_numeric(deep_expr, variables, Mode::Parallel);
        end = std::chrono::high_resolution_clock::now();
        std::stringstream strParallel;
        strParallel << std::fixed << std::setprecision(5) << "Deep parentheses (100 levels) (parallel): "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            calculator::eval(deep_expr);
        }
        end = std::chrono::high_resolution_clock::now();
        std::stringstream strHeaderOnlyCalc;
        strHeaderOnlyCalc << std::fixed << std::setprecision(5) << "Deep parentheses (100 levels) (header-only calculator): "
                          << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";
        MESSAGE(strSerial.str());
        MESSAGE(strParallel.str());
        MESSAGE(strHeaderOnlyCalc.str());
    }

    SUBCASE("Floating Point Heavy Benchmark")
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            Interpreter::interpret_numeric("3.14159 * 2.71828 + 1.41421 / 1.73205", variables, Mode::Serial);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::stringstream strSerial;
        strSerial << std::fixed << std::setprecision(5) << "1000 floating point expressions (serial): "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            Interpreter::interpret_numeric("3.14159 * 2.71828 + 1.41421 / 1.73205", variables, Mode::Parallel);
        }
        end = std::chrono::high_resolution_clock::now();
        std::stringstream strParallel;
        strParallel << std::fixed << std::setprecision(5) << "1000 floating point expressions (parallel): "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms";
        MESSAGE(strSerial.str());
        MESSAGE(strParallel.str());
    }
}
