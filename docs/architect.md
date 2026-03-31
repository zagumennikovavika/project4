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

#### 1. API Design
- Created `expression.h` — public interface for library users
- Defined `ErrorInfo` structure for error handling
- Designed `Variable` structure for passing variable values
- Made `Expression` opaque (users see only pointer, not internals)

#### 2. Internal Structure
- Created `expression_core.h` with full `Expression` structure
- Defined `expression_types.h` with AST node types (solved conflict between student2 and student3)
- Added flags (`hasPostfix`, `hasAst`) to track what data is available

#### 3. Integration Code (`expression_api.c`)
- `ExprParseInfix()` — connects Student 1 → Student 2
- `ExprParsePostfix()` — connects directly to Student 2
- `ExprEvaluate()` — decides whether to use Student 1 (numbers) or Student 2 (variables/functions)
- `ExprSimplify()` — connects Student 2 → Student 3
- `ExprFree()` — calls Student 2's `free_ast()`
- Error handling functions (`ExprSetError`, `ExprClearError`, `ExprErrorString`, `ExprPrintError`)
#### 4. Benchmark Development (`benchmark.c`)
- Created `BenchmarkResult` structure for storing measurements
- Implemented `BenchmarkExpression()` — core measurement function
- Created 5 benchmarks:
- Added time measurement using `clock()`
- Added calculation of average time per iteration
- Formatted output in tables for readability

#### 5. Build System
- Created `Makefile` with:
  - Compilation of all modules
  - `run` target for benchmarks
  - `examples` target for example programs
  - `tests` target for unit tests
  - `clean` target for removing build files
  - `all-run` target for running everything
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
