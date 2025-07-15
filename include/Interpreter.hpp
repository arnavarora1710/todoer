#pragma once

#include "Expression.hpp"
#include <optional>
#include <string>
#include <variant>
#include <memory>
#include <unordered_map>

using VariableMap = std::unordered_map<std::string, std::variant<int, double>>;

enum class Mode
{
    Serial = 's',
    Parallel = 'p',
};

namespace Interpreter
{
    using value = std::variant<int, double>;
    value evaluate(const std::shared_ptr<Expression> &expr, Mode mode = Mode::Serial);
    std::string interpret(std::string_view input, VariableMap &variables, Mode mode = Mode::Parallel);
    value interpret_numeric(std::string_view input, VariableMap &variables, Mode mode = Mode::Parallel);
};
