#ifndef STUDENT3_SIMPLIFIER_H
#define STUDENT3_SIMPLIFIER_H

#include <stdio.h>
#include <stdlib.h>  
#include <ctype.h>
#include <string.h>

typedef enum {
    NODE_NUMBER, NODE_VARIABLE, 
    NODE_UNARY, NODE_BINARY, 
    NODE_FUNCTION 
} NodeType;

typedef struct ExprNode {
    NodeType type;
    union {
        double number;
        char *var_name;
        
        struct {
            struct ExprNode *operand;
            char op;
        } unary;
 
        struct {
            struct ExprNode *left;
            struct ExprNode *right;
            char op;
        } binary;
 
        struct {
            struct ExprNode **args;
            int arg_count;
            char *func_name;
        } function;
        
    } data;
} ExprNode;

ExprNode* createNumber(double value);
ExprNode* createVariable(const char* name);
ExprNode* createUnary(char op, ExprNode* operand);
ExprNode* createBinary(char op, ExprNode* left, ExprNode* right);
ExprNode* createFunction(const char* name, ExprNode** args, int arg_count);

void freeNode(ExprNode* node);

ExprNode* isMulti(ExprNode* node);
ExprNode* isAddit(ExprNode* node);
ExprNode* isSubt(ExprNode* node);
ExprNode* isDiv(ExprNode* node);
ExprNode* isUnary(ExprNode* node);

ExprNode* rules(ExprNode* node);
ExprNode* simplify(ExprNode* node);

#endif