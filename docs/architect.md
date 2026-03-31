




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
