#ifndef EXPRESSION_CORE_H
#define EXPRESSION_CORE_H

#include "expression.h"
#include "expression_types.h" 

struct Expression {
    char infix[4096];      
    char postfix[4096];     
    int  hasPostfix;       

    ExprNode* astRoot;     
    int hasAst;      

    long operationCount;  
    double lastResult;     
};

void ExprSetError(ErrorInfo* err, int code, const char* msg);
void ExprClearError(ErrorInfo* err);

#endif