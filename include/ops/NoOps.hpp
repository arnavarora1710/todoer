#pragma once

#include "ops/Ops.hpp"
#include <string>
#include <any>

template <typename T>
    requires ArithmeticType<T>
class NoOps : public Ops
{
private:
    T arg;

public:
    NoOps(T arg) : arg(arg) {}
    std::variant<int, double> execute() const override
    {
        return arg;
    }
};

template <typename T>
std::unique_ptr<Ops> make_no_ops(T arg)
{
    return std::make_unique<NoOps<T>>(arg);
}