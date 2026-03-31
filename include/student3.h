#ifndef STUDENT3_SIMPLIFIER_H
#define STUDENT3_SIMPLIFIER_H

#include <stdio.h>
#include <stdlib.h>  
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "expression_types.h"


ExprNode* createNumber(double value);
ExprNode* createVariable(const char* name);
ExprNode* createUnary(char op, ExprNode* operand);
ExprNode* createBinary(char op, ExprNode* left, ExprNode* right);
ExprNode* createFunction(const char* name, ExprNode** args, int arg_count);

void freeNode(ExprNode* node);

ExprNode* isMulti(ExprNode* node);
ExprNode* similar(ExprNode* node, ExprNode* left, ExprNode* right);
ExprNode* isAddit(ExprNode* node);
ExprNode* isSubt(ExprNode* node);
ExprNode* isDiv(ExprNode* node);
ExprNode* isUnary(ExprNode* node);

ExprNode* rules(ExprNode* node);
ExprNode* simplify(ExprNode* node);

ExprNode* substitute(ExprNode *node, const char *var_name, double value);
ExprNode* differentiate(ExprNode *node, const char *var_name);
ExprNode* copyTree(ExprNode* node);

ExprNode* copyTree(ExprNode* node);
bool expression_equal(ExprNode *a, ExprNode *b);

void printExpr(ExprNode* node);

#endif