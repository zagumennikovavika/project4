#include "Student3.h"

// recursively replace a variable with a number
ExprNode* substitute(ExprNode *node, const char *var_name, double value) {
    if (node == NULL)
        return NULL;

    // node is a number, nothing to replace
    if (node->type == NODE_NUMBER)
        return node;

    // node is a variable, replace if names match
    if (node->type == NODE_VARIABLE) {
        if (strcmp(node->data.var_name, var_name) == 0) {
            free(node->data.var_name);
            free(node);
            return createNumber(value);
        }
        return node;
    }

    // node is a unary operation, recurse on operand
    if (node->type == NODE_UNARY) {
        node->data.unary.operand =
            substitute(node->data.unary.operand, var_name, value);
        return node;
    }

    // node is a binary operation, recurse on both children
    if (node->type == NODE_BINARY) {
        node->data.binary.left =
            substitute(node->data.binary.left, var_name, value);
        node->data.binary.right =
            substitute(node->data.binary.right, var_name, value);
        return node;
    }

    // node is a function, recurse on all arguments
    if (node->type == NODE_FUNCTION) {
        for (int i = 0; i < node->data.function.arg_count; i++) {
            node->data.function.args[i] =
                substitute(node->data.function.args[i], var_name, value);
        }
        return node;
    }

    // fallback, return node as is
    return node;
}