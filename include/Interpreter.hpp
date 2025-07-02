#pragma once

#include "Ops.hpp"
#include <vector>

// this implements a very simple Pratt parser
// just for PEMDAS expressions

enum class ASTPosType
{
    ROOT = 1 << 0,
    LEAF = 1 << 1,
    INTERNAL = 1 << 2,
};

constexpr ASTPosType SINGLETON = ASTPosType::ROOT | ASTPosType::LEAF;

enum class ASTNodeType
{
    BINARY_OP,
    UNARY_OP,
    LITERAL,
};

struct ASTNode
{
    ASTNodeType info;
    ASTPosType pos_info;
    std::size_t parent;
};

class AST
{
private:
    // array representation
    // root at index 0
    // left child at index << 1
    // right child at (index << 1) | 1
    std::size_t m_capacity{};
    std::size_t m_size{};
    std::vector<ASTNode> m_nodes;

public:
    AST(std::size_t capacity) : m_capacity{capacity}, m_nodes(capacity) {}
    //
    // disable copy and move constructors / assignment operators
    AST(const AST &) = delete;
    AST(AST &&) = delete;
    AST &operator=(const AST &) = delete;
    AST &operator=(AST &&) = delete;

    // default destructor
    ~AST() = default;

    void add_node(const Ops &op);
};
