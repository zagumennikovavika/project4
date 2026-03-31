#ifndef STUDENT2_H
#define STUDENT2_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_NODE_SIZE 128

typedef enum{
    NODE_NUMBER,
    NODE_VARIABLE,
    NODE_UNARY,
    NODE_BINARY,
    NODE_FUNCTION,
} NodeType;

typedef struct ExprNode{
    NodeType type;
    union{
        double number;
        char* var_name;
        struct{
            struct ExprNode* operand;
            char op;
        } unary;
        struct{
            struct ExprNode* left;
            struct ExprNode* right;
            char op;
        } binary;
        struct{
            struct ExprNode** args;
            int arg_count;
            char* func_name;
        } function;
    } data;
} ExprNode;

typedef struct{
    char* name;
    double var;
} VariableTable;

ExprNode* build_ast_from_postfix(const char* postfix);
int evaluate_ast(ExprNode *node, VariableTable* variables, int size_table, double* result);
void free_ast(ExprNode* node);

#endif
