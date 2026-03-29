#include "Student3.h"

ExprNode* copyTree(ExprNode* node) {
    if (node == NULL)
        return NULL;

    // --- NUMBER ---
    if (node->type == NODE_NUMBER) {
        return createNumber(node->data.number);
    }

    // --- VARIABLE ---
    if (node->type == NODE_VARIABLE) {
        return createVariable(node->data.var_name);
    }

    // --- UNARY ---
    if (node->type == NODE_UNARY) {
        ExprNode* operand_copy =
            copyTree(node->data.unary.operand);

        return createUnary(
            node->data.unary.op,
            operand_copy
        );
    }

    // --- BINARY ---
    if (node->type == NODE_BINARY) {
        ExprNode* left_copy =
            copyTree(node->data.binary.left);

        ExprNode* right_copy =
            copyTree(node->data.binary.right);

        return createBinary(
            node->data.binary.op,
            left_copy,
            right_copy
        );
    }

    // --- FUNCTION ---
    if (node->type == NODE_FUNCTION) {
        int n = node->data.function.arg_count;

        ExprNode** args_copy =
            (ExprNode**)malloc(sizeof(ExprNode*) * n);

        if (!args_copy)
            return NULL;

        for (int i = 0; i < n; i++) {
            args_copy[i] =
                copyTree(node->data.function.args[i]);
        }

        return createFunction(
            node->data.function.func_name,
            args_copy,
            n
        );
    }

    return NULL;
}

ExprNode* differentiate(ExprNode *node, const char *var_name) {
    if (node == NULL)
        return NULL;

    // --- NUMBER ---
    if (node->type == NODE_NUMBER)
        return createNumber(0.0);

    // --- VARIABLE ---
    if (node->type == NODE_VARIABLE) {
        if (strcmp(node->data.var_name, var_name) == 0)
            return createNumber(1.0);
        else
            return createNumber(0.0);
    }

    // --- UNARY ---
    if (node->type == NODE_UNARY) {
        char op = node->data.unary.op;

        ExprNode* d =
            differentiate(node->data.unary.operand, var_name);

        if (op == '+')
            return d;

        if (op == '-')
            return createUnary('-', d);
    }

    // --- BINARY ---
    if (node->type == NODE_BINARY) {
        char op = node->data.binary.op;

        ExprNode* f = node->data.binary.left;
        ExprNode* g = node->data.binary.right;

        ExprNode* df = differentiate(f, var_name);
        ExprNode* dg = differentiate(g, var_name);

        // --- f + g ---
        if (op == '+')
            return createBinary('+', df, dg);

        // --- f - g ---
        if (op == '-')
            return createBinary('-', df, dg);

        // --- f * g → f*dg + g*df ---
        if (op == '*') {
            ExprNode* term1 =
                createBinary('*', copyTree(f), dg);

            ExprNode* term2 =
                createBinary('*', copyTree(g), df);

            return createBinary('+', term1, term2);
        }

        // --- f / g ---
        if (op == '/') {
            ExprNode* num1 =
                createBinary('*', df, copyTree(g));

            ExprNode* num2 =
                createBinary('*', copyTree(f), dg);

            ExprNode* numerator =
                createBinary('-', num1, num2);

            ExprNode* denom =
                createBinary('^', copyTree(g), createNumber(2.0));

            return createBinary('/', numerator, denom);
        }
    }

    // --- FUNCTION ---
    if (node->type == NODE_FUNCTION) {
        char* name = node->data.function.func_name;
        ExprNode* arg = node->data.function.args[0];

        ExprNode* darg = differentiate(arg, var_name);

        // sin(f) → cos(f) * f'
        if (strcmp(name, "sin") == 0) {
            ExprNode** args = malloc(sizeof(ExprNode*));
            args[0] = copyTree(arg);

            ExprNode* cos_f = createFunction("cos", args, 1);
            return createBinary('*', cos_f, darg);
        }

        // cos(f) → -sin(f) * f'
        if (strcmp(name, "cos") == 0) {
            ExprNode** args = malloc(sizeof(ExprNode*));
            args[0] = copyTree(arg);

            ExprNode* sin_f = createFunction("sin", args, 1);
            ExprNode* neg = createUnary('-', sin_f);

            return createBinary('*', neg, darg);
        }

        // exp(f) → exp(f) * f'
        if (strcmp(name, "exp") == 0) {
            ExprNode** args = malloc(sizeof(ExprNode*));
            args[0] = copyTree(arg);

            ExprNode* exp_f = createFunction("exp", args, 1);
            return createBinary('*', exp_f, darg);
        }

        // ln(f) → f'/f
        if (strcmp(name, "ln") == 0) {
            return createBinary('/', darg, copyTree(arg));
        }
    }

    return NULL;
}



