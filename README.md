# Expression Library

A library for parsing, calculating, and simplifying mathematical expressions in the C language.

## Description

The library provides functionality for:
- Parsing infix mathematical expressions
- Conversion to postfix (reverse Polish) notation
- Calculations of expressions with support for arithmetic operations
- Counting the number of operations performed
- Error handling

## Main API Functions

| Function | Description | Return Value |
|----------|-------------|---------------|
| `ExprParseInfix(infix, err)` | Parse an infix expression | `Expression*` or NULL |
| `ExprParsePostfix(postfix, err)` | Parse a postfix expression | `Expression*` or NULL |
| `ExprEvaluate(expr, vars, result, err)` | Evaluate an expression | 0 on success, -1 on error |
| `ExprFree(expr)` | Free memory | void |
| `ExprGetOperations(expr)` | Get the number of operations | `long` |
| `ExprGetInfix(expr)` | Get the original infix string | `const char*` |
| `ExprGetPostfix(expr)` | Get the postfix string | `const char*` |
| `ExprSimplify(expr, err)` | Simplify an expression | `Expression*` or NULL |
| `ExprToPrefixString(expr, err)` | Convert to prefix notation | `char*` (requires free) |
| `ExprCheckSyntax(infix, err)` | Check expression syntax | 0 on success, -1 on error |
| `ExprErrorString(code)` | Get error text by code | `const char*` |
| `ExprPrintError(err)` | Print error to stderr | void |

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

| Operator | Description | Precedence | Associativity |
|----------|-------------|------------|---------------|
| `+` | Addition | 1 | Left |
| `-` | Subtraction | 1 | Left |
| `*` | Multiplication | 2 | Left |
| `/` | Division | 2 | Left |
| `^` | Exponentiation | 3 | Right |
| `~` | Unary minus | 3 | Right |
