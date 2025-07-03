#pragma once
#include "ops/Ops.hpp"

// concept for binary functions that take two arguments and return a result

template <typename T, typename U, typename BinaryFunc>
concept BinaryFuncInvocable = std::is_invocable_v<BinaryFunc, T, U>;
template <typename T>
concept BinaryTDefaultConstructible = std::is_default_constructible_v<T>;
template <typename U>
concept BinaryUDefaultConstructible = std::is_default_constructible_v<U>;

template <typename T, typename U, typename BinaryFunc>
    requires BinaryFuncInvocable<T, U, BinaryFunc>
class BinaryOps
    : public Ops
{

private:
    BinaryFunc m_func;
    const T arg1;
    const U arg2;

public:
    BinaryOps(BinaryFunc func)
        requires BinaryTDefaultConstructible<T> && BinaryUDefaultConstructible<U>
        : m_func(func), arg1(T()), arg2(U())
    {
    }
    BinaryOps(BinaryFunc func, T argument1, U argument2)
        : m_func(func), arg1(argument1), arg2(argument2) {}

    // execute the operation
    std::any execute() const override
    {
        return m_func(arg1, arg2);
    }

    std::string to_string() const override
    {
        return "BinaryOps " + std::to_string(std::any_cast<int>(arg1)) + " " + std::to_string(std::any_cast<int>(arg2));
    }
};

// deduction guide for BinaryOps
template <typename T, typename U, typename BinaryFunc>
BinaryOps(BinaryFunc func, T argument1, U argument2) -> BinaryOps<T, U, BinaryFunc>;

// Factory function to create unique_ptr with deduction
template <typename BinaryFunc, typename T, typename U>
inline auto make_binary_ops(const BinaryFunc &func, const T &arg1, const U &arg2)
{
    return std::make_unique<BinaryOps<T, U, BinaryFunc>>(func, arg1, arg2);
}
