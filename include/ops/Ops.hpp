#pragma once

#include <any>
#include <memory>
#include <type_traits>
#include <variant>

// Shared concept for arithmetic types used by all operation classes
template<typename T>
concept ArithmeticType = std::is_same_v<T, int> || std::is_same_v<T, double>;

// this is supposed to be an interface for operations
// for example, functions here can be implemented for Unary and Binary operations
class Ops
{
public:
    virtual ~Ops() = default;
    virtual std::variant<int, double> execute() const = 0;
};
