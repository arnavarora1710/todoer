# Todoer - Concurrent Task Graph Calculator

A high-performance arithmetic expression evaluator that represents mathematical expressions as task graphs and executes them with parallel processing capabilities.

## Overview

Todoer implements a PEMDAS-compliant interpreter that parses mathematical expressions into abstract syntax trees, converts them into task graphs, and executes independent operations concurrently using a custom ThreadPool implementation.

## Features

- **Parallel Expression Evaluation**: Automatically identifies independent operations and executes them concurrently
- **Variable Support**: Define and use variables in expressions (`x = 5`, `y = x * 2`)
- **Comprehensive Operators**: Support for `+`, `-`, `*`, `/`, `^` (power), `&` (bitwise AND), `|` (bitwise OR)
- **Parentheses & Precedence**: Full PEMDAS/BODMAS operator precedence
- **Thread-Safe**: Concurrent expression tree modifications with proper synchronization
- **Performance Testing**: Built-in benchmarks comparing serial vs parallel execution

## Build Instructions

```bash
mkdir -p build
cd build
cmake ..
make
./todoer
```

### Requirements
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10+
- AddressSanitizer support (optional, for debugging)

## Usage

### Interactive Mode

```bash
# Run in serial mode (default)
./todoer s

# Run in parallel mode
./todoer p
```

### Example Session

```
>>> 1 + 2
3
>>> x = 5
>>> y = x * 2
>>> (x + y) * (10 - 3)
105
>>> 2^3 + 4*5
28
>>> exit
```

### Supported Operations

| Operator | Description | Example |
|----------|-------------|---------|
| `+` | Addition | `2 + 3 = 5` |
| `-` | Subtraction/Negation | `5 - 2 = 3`, `-5` |
| `*` | Multiplication | `3 * 4 = 12` |
| `/` | Division | `10 / 2 = 5` |
| `^` | Exponentiation | `2^3 = 8` |
| `&` | Bitwise AND | `5 & 3 = 1` |
| `\|` | Bitwise OR | `5 \| 3 = 7` |
| `()` | Parentheses | `(2 + 3) * 4 = 20` |

## Architecture

### Core Components

- **Lexer**: Tokenizes input expressions with support for numbers, variables, and operators
- **Expression Parser**: Builds abstract syntax trees using operator precedence (Pratt) parsing
- **Task Graph**: Converts expression trees into executable task graphs
- **Scheduler**: Implements both serial and parallel execution strategies
- **ThreadPool**: Custom thread pool with work-stealing for parallel task execution

### Task Graph Example

Expression: `(a + b) * (c + d)`

```
       *
     /   \
    +     +
   / \   / \
  a   b c   d
```

The scheduler identifies that `(a + b)` and `(c + d)` can execute in parallel, while `*` waits for both results.

### Running Benchmarks

```bash
./test_runner
```

Runs correctness tests and benchmarks on small/large and simple/complex expressions (parser and interpreter tests implemented in `tests/`)

## Development

### Project Structure

```
todoer/
├── include/           # Header files
│   ├── ops/          # Operation implementations
│   └── third_party/  # External dependencies
├── src/              # Source files
├── tests/            # Unit tests
└── build/            # Build directory
```

### Running Tests

```bash
cd build
make test_runner
./test_runner
```

## Technical Highlights

- **Modern C++**: Extensive use of C++17/20 features (variants, optional, smart pointers)
- **Concurrency**: ThreadPool, futures/promises, condition variables, atomic operations
- **Memory Safety**: RAII principles, smart pointers, AddressSanitizer integration
- **Expression Parsing**: Pratt parser implementation for operator precedence
- **Task Scheduling**: Level-by-level parallel BFS execution with dependency tracking