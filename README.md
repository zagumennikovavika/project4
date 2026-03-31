# Expression Library

A library for parsing, calculating, and simplifying mathematical expressions in the C language.

## Launch
```
make run        # Run benchmarks
make examples   # Run examples
make tests      # Run tests
make clean      # Clean build files
```

## Description

The library provides functionality for:
- Parsing infix mathematical expressions
- Converting to postfix (Reverse Polish Notation)
- Evaluating expressions with arithmetic operations
- Counting the number of operations performed
- Simplifying expressions with algebraic rules
- Error handling

## Architecture
The project is divided into modules implemented by different team members:

| Module | Responsibility |
|--------|---------------|
| **Student 1** | Infix to postfix conversion, postfix evaluation |
| **Student 2** | AST construction, evaluation with variables and functions |
| **Student 3** | Expression simplification, differentiation | 
| **Architect** | API design, module integration, benchmarks |


## Error Codes

| Code | Value | Description |
|------|-------|-------------|
| `ERR_OK` | 0 | No error |
| `ERR_SYNTAX` | 1 | Syntax error in expression |
| `ERR_DIVISION_BY_ZERO` | 2 | Division by zero |
| `ERR_MEMORY` | 3 | Memory allocation failed |
| `ERR_INVALID_ARG` | 4 | Invalid argument |
| `ERR_OVERFLOW` | 5 | Buffer overflow |
| `ERR_UNKNOWN_VARIABLE` | 6 | Unknown variable |
| `ERR_UNKNOWN_FUNCTION` | 7 | Unknown function |

## Supported Operations

| Operator | Description | Precedence |
|----------|-------------|------------|
| `+` | Addition | 1 |
| `-` | Subtraction | 1 |
| `*` | Multiplication | 2 |
| `/` | Division | 2 |
| `^` | Exponentiation | 3 |
| `~` | Unary minus | 3 |


