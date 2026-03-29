#include "Student3.h"

// recursively check if two expression trees are structurally and mathematically equal
bool expression_equal(ExprNode *a, ExprNode *b) {
    if (a == NULL && b == NULL)
        return true;

    if (a == NULL || b == NULL)
        return false;

    if (a->type != b->type)
        return false;

    // compare number nodes
    if (a->type == NODE_NUMBER)
        return a->data.number == b->data.number;

    // compare variable nodes by name
    if (a->type == NODE_VARIABLE)
        return strcmp(a->data.var_name, b->data.var_name) == 0;

    // compare unary operation nodes
    if (a->type == NODE_UNARY) {
        if (a->data.unary.op != b->data.unary.op)
            return false;

        return expression_equal(
            a->data.unary.operand,
            b->data.unary.operand
        );
    }

    // compare binary operation nodes
    if (a->type == NODE_BINARY) {
        if (a->data.binary.op != b->data.binary.op)
            return false;

        ExprNode* aL = a->data.binary.left;
        ExprNode* aR = a->data.binary.right;
        ExprNode* bL = b->data.binary.left;
        ExprNode* bR = b->data.binary.right;

        // check standard left-right order
        if (expression_equal(aL, bL) &&
            expression_equal(aR, bR))
            return true;

        // check commutativity for addition and multiplication
        if (a->data.binary.op == '+' || a->data.binary.op == '*') {
            if (expression_equal(aL, bR) &&
                expression_equal(aR, bL))
                return true;
        }

        return false;
    }

    // compare function nodes and their arguments
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