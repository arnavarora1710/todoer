#include "Task.h"
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

    Task task(1, std::move(op));                                     // create a task with id 1 and the operation
    auto ans = task.execute();                                       // execute the operation
    std::cout << "answer: " << std::any_cast<int>(ans) << std::endl; // print the result
    std::cout << "Task executed successfully." << std::endl;
    return 0;
}
