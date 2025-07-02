#include "Task.hpp"
#include <iostream>
#include <any>
#include <functional>

int main()
{
    auto func = [](int x, int y)
    {
        return x + y;
    };
    // Using factory function with template deduction
    auto op = make_binary_ops(func, 10, 20);

    auto u_func = [](int x)
    {
        return -x;
    };
    auto op2 = make_unary_ops(u_func, 10);

    Task task(1, std::move(op)); // create a task with id 1 and the operation
    Task task2(2, std::move(op2));
    auto ans = task.execute(); // execute the operation
    auto ans2 = task2.execute();
    std::cout << "answer: " << std::any_cast<int>(ans) << std::endl;   // print the result
    std::cout << "answer2: " << std::any_cast<int>(ans2) << std::endl; // print the result
    std::cout << "Task executed successfully." << std::endl;
    std::cout << "Task 2 executed successfully." << std::endl;
    return 0;
}
