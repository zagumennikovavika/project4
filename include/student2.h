#ifndef STUDENT2_H
#define STUDENT2_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "expression_types.h"

#define BUFFER_NODE_SIZE 128

typedef struct{
    char* name;
    double var;
} VariableTable;

ExprNode* build_ast_from_postfix(const char* postfix);
int evaluate_ast(ExprNode *node, VariableTable* variables, int size_table, double* result);
void free_ast(ExprNode* node);

#endif
