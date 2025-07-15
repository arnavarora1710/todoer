#pragma once

#include <utility>

#include "Lexer.hpp"

struct Parser
{
	Lexer lexer;
	VariableMap &variables;

	Parser(Lexer lexer, VariableMap &variables) : lexer(std::move(lexer)), variables(variables) {}
	std::shared_ptr<Expression> parseExpression(int min_binding_power = 0);
};
