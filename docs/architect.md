# Expression Library — Architecture Documentation

## Core Functions

| Function | Description | Return Value |
|----------|-------------|--------------|
| `ExprParseInfix(infix, err)` | Parse an infix expression | `Expression*` or NULL |
| `ExprParsePostfix(postfix, err)` | Parse a postfix expression | `Expression*` or NULL |
| `ExprEvaluate(expr, vars, result, err)` | Evaluate an expression | 0 on success, -1 on error |
| `ExprSimplify(expr, err)` | Simplify an expression | `Expression*` or NULL |
| `ExprFree(expr)` | Free memory | void |
| `ExprGetOperations(expr)` | Get the number of operations | `long` |
| `ExprGetInfix(expr)` | Get the original infix string | `const char*` |
| `ExprGetPostfix(expr)` | Get the postfix string | `const char*` |
| `ExprCheckSyntax(infix, err)` | Check expression syntax | 0 on success, -1 on error |
| `ExprErrorString(code)` | Get error description by code | `const char*` |
| `ExprPrintError(err)` | Print error to stderr | void |

## Benchmark Results

### Performance Summary

| Test | Time (1M iterations) |
|------|---------------------|
| `3+4` | ~67 ms |
| `2^10` | ~145 ms |
| `x+5` (postfix) | ~31 ms |
| `sin(1.57)` (postfix) | ~54 ms |

### Parsing Speed

| Metric | Value |
|--------|-------|
| 100,000 parses | ~205 ms |
| Per expression | ~2 µs |

### Simplification Speedup

| Expression | Speedup |
|------------|---------|
| `x*2+3*x-x` → `4*x` | 10.4x |
| `(x+y)*2+(x+y)*3` → `5*(x+y)` | 11.3x |
