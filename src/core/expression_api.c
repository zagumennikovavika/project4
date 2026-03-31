#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "expression_core.h"
#include "student1.h"
#include "student2.h"
#include "student3.h"

/* Write error to ErrorInfo structure */
void ExprSetError(ErrorInfo* err, int code, const char* msg) {
    if (!err) return;
    err->code = code;
    strncpy(err->message, msg, sizeof(err->message) - 1);
    err->message[sizeof(err->message) - 1] = '\0';
}

/* Clear error */
void ExprClearError(ErrorInfo* err) {
    if (!err) return;
    err->code = ERR_OK;
    err->message[0] = '\0';
}

/* Check if string contains letters (variables or functions) */
int HasLetters(const char* str) {
    if (!str) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i])) {
            return 1;
        }
    }
    return 0;
}

/* Check syntax */
int ExprCheckSyntax(const char* infix, ErrorInfo* err) {
    if (!infix) {
        if (err) ExprSetError(err, ERR_INVALID_ARG, "infix is NULL");
        return -1;
    }
    if (infix[0] == '\0') {
        if (err) ExprSetError(err, ERR_INVALID_ARG, "empty expression");
        return -1;
    }

    int balance = 0;
    for (int i = 0; infix[i] != '\0'; i++) {
        if (infix[i] == '(') balance++;
        if (infix[i] == ')') balance--;
        if (balance < 0) {
            if (err) ExprSetError(err, ERR_SYNTAX, "unbalanced parentheses");
            return -1;
        }
    }
    if (balance != 0) {
        if (err) ExprSetError(err, ERR_SYNTAX, "unbalanced parentheses");
        return -1;
    }

    for (int i = 0; infix[i] != '\0'; i++) {
        if (infix[i] == '(' && infix[i + 1] == ')') {
            if (err) ExprSetError(err, ERR_SYNTAX, "empty parentheses");
            return -1;
        }
    }

    return 0; 
}

/* Parse infix expression */
Expression* ExprParseInfix(const char* infix, ErrorInfo* err) {
    ExprClearError(err);

    if (!infix) {
        if (err) ExprSetError(err, ERR_INVALID_ARG, "infix is NULL");
            return NULL;
    }
    if (ExprCheckSyntax(infix, err) != 0) {
        return NULL;
    }

    Expression* expr = (Expression*)malloc(sizeof(Expression));
    if (!expr) {
        if (err) ExprSetError(err, ERR_MEMORY, "malloc failed");
        return NULL;
    }
    memset(expr, 0, sizeof(Expression));

    strncpy(expr->infix, infix, sizeof(expr->infix) - 1);

    // Student 1: infix -> postfix
    if (InfixToPostfix(infix, expr->postfix, sizeof(expr->postfix)) != 0) {
        if (err) ExprSetError(err, ERR_SYNTAX, "syntax error in expression");
            free(expr);
            return NULL;
    }
    expr->hasPostfix = 1;

    // Student 2: postfix -> AST
    expr->astRoot = build_ast_from_postfix(expr->postfix);
    if (expr->astRoot) {
        expr->hasAst = 1;
    }

    return expr;
}

/* Parse postfix expression */
Expression* ExprParsePostfix(const char* postfix, ErrorInfo* err) {
    ExprClearError(err);

    if (!postfix) {
        if (err) ExprSetError(err, ERR_INVALID_ARG, "postfix is NULL");
        return NULL;
    }

    Expression* expr = (Expression*)malloc(sizeof(Expression));
    if (!expr) {
        if (err) ExprSetError(err, ERR_MEMORY, "malloc failed");
        return NULL;
    }
    memset(expr, 0, sizeof(Expression));

    strncpy(expr->postfix, postfix, sizeof(expr->postfix) - 1);
    expr->hasPostfix = 1;

    // Student 2: postfix -> AST
    expr->astRoot = build_ast_from_postfix(postfix);
    if (expr->astRoot) {
        expr->hasAst = 1;
    }

    return expr;
}

/* Evaluate expressions */
int ExprEvaluate(const Expression* expr, const Variable* vars,
                  double* result, ErrorInfo* err) {
    ExprClearError(err);

    if (!expr || !result) {
        if (err) ExprSetError(err, ERR_INVALID_ARG, "invalid arguments");
        return -1;
    }

    // Check for letters in infix OR postfix
    int hasVars = HasLetters(expr->infix) || HasLetters(expr->postfix);

    // If there are variables or functions - use AST (student 2)
    if (hasVars && expr->hasAst && expr->astRoot) {
        int varCount = 0;
        if (vars) {
            while (vars[varCount].name[0] != '\0') varCount++;
        }

        VariableTable* varTable = NULL;
        if (varCount > 0) {
            varTable = (VariableTable*)malloc(varCount * sizeof(VariableTable));
            if (!varTable) {
                if (err) ExprSetError(err, ERR_MEMORY, "malloc failed");
                    return -1;
            }
            for (int i = 0; i < varCount; i++) {
                varTable[i].name = strdup(vars[i].name);
                varTable[i].var = vars[i].value;
            }
        }

        // STUDENT 2: evaluate AST
        int evalResult = evaluate_ast(expr->astRoot, varTable, varCount, result);
        
        for (int i = 0; i < varCount; i++) {
            free(varTable[i].name);
        }
        free(varTable);

        if (evalResult != 0) {
            if (err) ExprSetError(err, ERR_DIVISION_BY_ZERO, "evaluation failed");
            return -1;
        }
        
        // Count operations (student 1) - only if postfix exists
        if (expr->hasPostfix) {
            long ops = 0;
            double c;
            if (EvaluatePostfix(expr->postfix, &c, &ops) == 0) {
                ((Expression*)expr)->operationCount = ops;
            }
        }
        ((Expression*)expr)->lastResult = *result;
        
        return 0;
    }

    // If no variables - use fast postfix (student 1)
    if (expr->hasPostfix) {
        long ops = 0;
        // STUDENT 1: evaluate postfix
        if (EvaluatePostfix(expr->postfix, result, &ops) == 0) {
            ((Expression*)expr)->operationCount = ops;
            ((Expression*)expr)->lastResult = *result;
            return 0;
        } else {
            if (err) ExprSetError(err, ERR_DIVISION_BY_ZERO, "evaluation failed");
            return -1;
        }
    }

    if (err) ExprSetError(err, ERR_SYNTAX, "evaluation failed");
    return -1;
}

/* Get infix string */
const char* ExprGetInfix(const Expression* expr) {
    if (!expr) return NULL;
    return expr->infix;
}

/* Get postfix string */
const char* ExprGetPostfix(const Expression* expr) {
    if (!expr) return NULL;
    return expr->postfix;
}

/* Number of operations in expression */
long ExprGetOperations(const Expression* expr) {
    if (!expr) return 0;
    return expr->operationCount;
}

/* Simplify expression */
Expression* ExprSimplify(const Expression* expr, ErrorInfo* err) {
    ExprClearError(err);

    if (!expr) {
        if (err) ExprSetError(err, ERR_INVALID_ARG, "expr is NULL");
        return NULL;
    }
    if (!expr->hasAst || !expr->astRoot) {
        if (err) ExprSetError(err, ERR_SYNTAX, "no AST available for simplification");
        return NULL;
    }

    // Copy AST by building from postfix (student 2)
    ExprNode* copyRoot = build_ast_from_postfix(expr->postfix);
    if (!copyRoot) {
        if (err) ExprSetError(err, ERR_SYNTAX, "failed to copy AST");
        return NULL;
    }

    // STUDENT 3: simplify expression
    ExprNode* simplified = simplify(copyRoot);
    if (!simplified) {
        if (err) ExprSetError(err, ERR_SYNTAX, "simplification failed");
        return NULL;
    }

    // Create new expression
    Expression* newExpr = (Expression*)malloc(sizeof(Expression));
    if (!newExpr) {
        if (err) ExprSetError(err, ERR_MEMORY, "malloc failed");
        freeNode(simplified);
        return NULL;
    }
    memset(newExpr, 0, sizeof(Expression));

    newExpr->astRoot = simplified;
    newExpr->hasAst = 1;
    strncpy(newExpr->infix, expr->infix, sizeof(newExpr->infix) - 1);
    newExpr->hasPostfix = 0;  

    return newExpr;
}

/* Free memory */
void ExprFree(Expression* expr) {
    if (!expr) return;
    if (expr->hasAst && expr->astRoot) {
        free_ast(expr->astRoot); 
    }
    free(expr);
}

/* Error handling */
const char* ExprErrorString(int code) {
    switch (code) {
        case ERR_OK:               return "OK";
        case ERR_SYNTAX:           return "Syntax error";
        case ERR_DIVISION_BY_ZERO: return "Division by zero";
        case ERR_MEMORY:           return "Memory allocation failed";
        case ERR_INVALID_ARG:      return "Invalid argument";
        case ERR_OVERFLOW:         return "Buffer overflow";
        case ERR_UNKNOWN_VARIABLE: return "Unknown variable";
        case ERR_UNKNOWN_FUNCTION: return "Unknown function";
        default:                   return "Unknown error";
    }
}

void ExprPrintError(const ErrorInfo* err) {
    if (!err) return;
    if (err->code == ERR_OK) {
        fprintf(stderr, "[OK] No error\n");
    } else {
        fprintf(stderr, "[ERROR %d] %s: %s\n",
                err->code, ExprErrorString(err->code), err->message);
    }
}