#pragma once
#include "ops/Ops.hpp"

// concept for unary functions that take one argument and return a result

template <typename T, typename UnaryFunc>
concept UnaryFuncInvocable = std::is_invocable_v<UnaryFunc, T>;
template <typename T>
concept UnaryTDefaultConstructible = std::is_default_constructible_v<T>;

template <typename T, typename UnaryFunc>
    requires UnaryFuncInvocable<T, UnaryFunc>
class UnaryOps
    : public Ops
{
private:
    UnaryFunc m_func;
    const T arg;

public:
    UnaryOps(UnaryFunc func)
        requires UnaryTDefaultConstructible<T>
        : m_func(func), arg(T())
    {
    }
    UnaryOps(UnaryFunc func, T argument) : m_func(func), arg(argument) {}

    // execute the operation
    std::any execute() const override
    {
        return m_func(arg);
    }
};

// Factory function to create unique_ptr with deduction
template <typename UnaryFunc, typename T>
inline auto make_unary_ops(const UnaryFunc &func, const T &arg)
{
    return std::make_unique<UnaryOps<T, UnaryFunc>>(func, arg);
}

// deduction guide for UnaryOp
template <typename T, typename UnaryFunc>
UnaryOps(UnaryFunc func, T argument) -> UnaryOps<T, UnaryFunc>;
