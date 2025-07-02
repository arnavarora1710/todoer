#pragma once

#include <concepts>
#include <any>
#include <type_traits>
#include <memory>

// this is supposed to be an interface for operations
// for example, functions here can be implemented for Unary and Binary operations
class Ops
{
public:
    virtual ~Ops() = default;
    virtual std::any execute() const = 0;
    virtual std::string to_string() const = 0;
};

// need a no-op operation for an expression like just a number (5)
class NoOps : public Ops
{
private:
    const std::any arg;

public:
    NoOps(std::any arg) : arg(arg) {}

    std::any execute() const override
    {
        return arg;
    }

    std::string to_string() const override
    {
        return "NoOps " + std::to_string(std::any_cast<int>(arg));
    }
};

inline auto make_no_ops(std::any arg)
{
    return std::make_unique<NoOps>(arg);
}

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

    std::string to_string() const override
    {
        return "UnaryOps " + std::to_string(std::any_cast<int>(arg));
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
