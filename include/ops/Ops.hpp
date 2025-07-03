#pragma once

#include <any>
#include <concepts>
#include <memory>
#include <string>
#include <type_traits>

// this is supposed to be an interface for operations
// for example, functions here can be implemented for Unary and Binary operations
class Ops
{
public:
    virtual ~Ops() = default;
    virtual std::any execute() const = 0;
};