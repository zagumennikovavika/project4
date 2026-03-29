
#include "Student3.h"

bool expression_equal(ExprNode *a, ExprNode *b) {
    if (a == NULL && b == NULL)
        return true;

    if (a == NULL || b == NULL)
        return false;

    if (a->type != b->type)
        return false;

    // --- NUMBER ---
    if (a->type == NODE_NUMBER)
        return a->data.number == b->data.number;

    // --- VARIABLE ---
    if (a->type == NODE_VARIABLE)
        return strcmp(a->data.var_name, b->data.var_name) == 0;

    // --- UNARY ---
    if (a->type == NODE_UNARY) {
        if (a->data.unary.op != b->data.unary.op)
            return false;

        return expression_equal(
            a->data.unary.operand,
            b->data.unary.operand
        );
    }

    // --- BINARY ---
    if (a->type == NODE_BINARY) {
        if (a->data.binary.op != b->data.binary.op)
            return false;

        ExprNode* aL = a->data.binary.left;
        ExprNode* aR = a->data.binary.right;
        ExprNode* bL = b->data.binary.left;
        ExprNode* bR = b->data.binary.right;

        // обычный порядок
        if (expression_equal(aL, bL) &&
            expression_equal(aR, bR))
            return true;

        // --- КОММУТАТИВНОСТЬ ---
        if (a->data.binary.op == '+' || a->data.binary.op == '*') {
            if (expression_equal(aL, bR) &&
                expression_equal(aR, bL))
                return true;
        }

        return false;
    }

    // --- FUNCTION ---
    if (a->type == NODE_FUNCTION) {
        if (strcmp(a->data.function.func_name,
                   b->data.function.func_name) != 0)
            return false;

        if (a->data.function.arg_count != b->data.function.arg_count)
            return false;

        for (int i = 0; i < a->data.function.arg_count; i++) {
            if (!expression_equal(
                    a->data.function.args[i],
                    b->data.function.args[i]))
                return false;
        }

        return true;
    }

    return false;
}
