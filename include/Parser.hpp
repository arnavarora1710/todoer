#pragma once

#include <utility>

#include "Lexer.hpp"

class Parser
{
private:
	Lexer lexer;
	VariableMap &variables;

public:
	Parser(Lexer lexer, VariableMap &variables) : lexer(std::move(lexer)), variables(variables) {}
	std::shared_ptr<Expression> parseExpression(int min_binding_power = 0);
};
