#include "Interpreter.hpp"
#include <iostream>
#include <cstring>

void printUsage(std::string_view programName)
{
    std::cerr << "Usage: " << programName << " [s|p]" << std::endl;
    std::cerr << "s: serial mode" << std::endl;
    std::cerr << "p: parallel mode" << std::endl;
}

int main(int argc, char *argv[])
{
    char mode = 's';
    if (argc > 1)
    {
        // if length of argv[1] is more than 1, return error
        if (strlen(argv[1]) > 1)
        {
            printUsage(argv[0]);
            return 1;
        }
        mode = argv[1][0];
        // if mode is not s or p, return error
        // mode can only be serial or parallel
        if (mode != 's' && mode != 'p')
        {
            printUsage(argv[0]);
            return 1;
        }
    }
    VariableMap variables;
    while (true)
    {
        std::cout << ">>> ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "exit")
            break;
        auto result = Interpreter::interpret(input, variables, mode);
        std::cout << result;
        if (!result.empty())
            std::cout << std::endl;
    }
    return 0;
}
