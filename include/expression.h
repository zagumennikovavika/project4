#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdlib.h>  

typedef enum {
    ERR_OK               = 0,
    ERR_SYNTAX           = 1,   
    ERR_DIVISION_BY_ZERO = 2,   
    ERR_MEMORY           = 3,   
    ERR_INVALID_ARG      = 4, 
    ERR_OVERFLOW         = 5,   
    ERR_UNKNOWN_VARIABLE = 6,  
    ERR_UNKNOWN_FUNCTION = 7   
} ErrorCode;

typedef struct {
    int  code;         
    char message[256]; 
} ErrorInfo;

typedef struct Expression Expression;

typedef struct {
    char   name[64];
    double value;
} Variable;

Expression* ExprParseInfix(const char* infix, ErrorInfo* err);
Expression* ExprParsePostfix(const char* postfix, ErrorInfo* err);
Expression* ExprSimplify(const Expression* expr, ErrorInfo* err);

void ExprFree(Expression* expr);
int ExprEvaluate(const Expression* expr, const Variable* vars,
                  double* result, ErrorInfo* err);
long ExprGetOperations(const Expression* expr);
const char* ExprGetInfix(const Expression* expr);
const char* ExprGetPostfix(const Expression* expr);
int ExprCheckSyntax(const char* infix, ErrorInfo* err);
const char* ExprErrorString(int code);
void ExprPrintError(const ErrorInfo* err);


#endif /* EXPRESSION_H */