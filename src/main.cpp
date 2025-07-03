#include "Interpreter.hpp"
#include <iostream>

int main()
{
    VariableMap variables;
    while (true)
    {
        std::cout << ">>> ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "exit")
            break;
        std::cout << Interpreter::interpret(input, variables);
    }
    return 0;
}
