#ifndef EXPRESSION_TYPES_H
#define EXPRESSION_TYPES_H

typedef enum {
    NODE_NUMBER,
    NODE_VARIABLE,
    NODE_UNARY,
    NODE_BINARY,
    NODE_FUNCTION,
} NodeType;

typedef struct ExprNode {
    NodeType type;
    union {
        double number;
        char* var_name;
        struct {
            struct ExprNode* operand;
            char op;
        } unary;
        struct {
            struct ExprNode* left;
            struct ExprNode* right;
            char op;
        } binary;
        struct {
            struct ExprNode** args;
            int arg_count;
            char* func_name;
        } function;
    } data;
} ExprNode;

#endif