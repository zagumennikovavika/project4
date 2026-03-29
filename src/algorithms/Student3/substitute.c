#include "Student3.h"

ExprNode* substitute(ExprNode *node, const char *var_name, double value) {
    if (node == NULL)
        return NULL;

    // --- NUMBER ---
    if (node->type == NODE_NUMBER)
        return node;

    // --- VARIABLE ---
    if (node->type == NODE_VARIABLE) {
        if (strcmp(node->data.var_name, var_name) == 0) {
            // заменяем переменную на число
            free(node->data.var_name);
            free(node);
            return createNumber(value);
        }
        return node;
    }

    // --- UNARY ---
    if (node->type == NODE_UNARY) {
        node->data.unary.operand =
            substitute(node->data.unary.operand, var_name, value);

        return node;
    }

    // --- BINARY ---
    if (node->type == NODE_BINARY) {
        node->data.binary.left =
            substitute(node->data.binary.left, var_name, value);

        node->data.binary.right =
            substitute(node->data.binary.right, var_name, value);

        return node;
    }

    // --- FUNCTION ---
    if (node->type == NODE_FUNCTION) {
        for (int i = 0; i < node->data.function.arg_count; i++) {
            node->data.function.args[i] =
                substitute(node->data.function.args[i], var_name, value);
        }
        return node;
    }

    return node;
}