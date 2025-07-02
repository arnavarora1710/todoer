#include "Task.hpp"
#include "TaskGraph.hpp"
#include "Expression.hpp"
#include "Lexer.hpp"
#include <iostream>
#include <any>
#include <functional>

int main()
{
    while (true)
    {
        std::cout << ">>> ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "exit")
            break;
        Expression expr = from_string(input);
        TaskGraph tg(expr);
        while (!tg.m_leaves.empty())
        {
            auto &leaf = tg.m_leaves.front();
            auto result = leaf.execute();
            tg.m_leaves.clear();
            auto leaves = getLeaves(expr);
            tg.m_leaves = std::move(leaves);
            if (tg.m_leaves.empty())
            {
                std::cout << std::any_cast<int>(result) << std::endl;
                break;
            }
        }
    }

    return 0;
}
