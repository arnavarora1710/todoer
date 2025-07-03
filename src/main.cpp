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
        auto result = Interpreter::interpret(input, variables);
        std::cout << result;
        if (!result.empty())
            std::cout << std::endl;
    }
    return 0;
}
