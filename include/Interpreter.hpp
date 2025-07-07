#pragma once
#include "TaskGraph.hpp"
#include "Scheduler.hpp"
#include "Expression.hpp"
#include <string>
#include <variant>
#include <memory>
#include <unordered_map>

typedef std::unordered_map<std::string, std::variant<int, double>> VariableMap;

namespace Interpreter
{
    std::variant<int, double> evaluate(std::shared_ptr<Expression> &expr, char mode = 's');
    std::string interpret(std::string_view input, VariableMap &variables, char mode = 's');
};