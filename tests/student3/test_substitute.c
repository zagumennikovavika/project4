#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "Student3.h"


void assert_number(ExprNode* node, double value) {
    assert(node != NULL);
    assert(node->type == NODE_NUMBER);
    assert(fabs(node->data.number - value) < 1e-9);
}

void assert_variable(ExprNode* node, const char* name) {
    assert(node != NULL);
    assert(node->type == NODE_VARIABLE);
    assert(strcmp(node->data.var_name, name) == 0);
}


// 1.  x -> number
void test_simple_variable() {
    ExprNode* x = createVariable("x");
    ExprNode* result = substitute(x, "x", 5);

    assert_number(result, 5);
    freeNode(result);
}

// 2.
void test_no_substitute() {
    ExprNode* y = createVariable("y");
    ExprNode* result = substitute(y, "x", 5);

    assert_variable(result, "y");
    freeNode(result);
}

// 3. -x -> -5
void test_unary_substitute() {
    ExprNode* x = createVariable("x");
    ExprNode* expr = createUnary('-', x);

    expr = substitute(expr, "x", 5);

    assert(expr != NULL);
    assert(expr->type == NODE_UNARY);
    assert(expr->data.unary.operand != NULL);
    assert(expr->data.unary.operand->type == NODE_NUMBER);
    assert(fabs(expr->data.unary.operand->data.number - 5.0) < 1e-9);

    freeNode(expr);
}

// 4. x + 3 -> 5 + 3
void test_binary_substitute() {
    ExprNode* expr = createBinary('+', createVariable("x"), createNumber(3));

    expr = substitute(expr, "x", 5);

    assert(expr != NULL);
    assert(expr->type == NODE_BINARY);
    assert(expr->data.binary.left != NULL);
    assert(expr->data.binary.right != NULL);
    assert_number(expr->data.binary.left, 5);
    assert_number(expr->data.binary.right, 3);

    freeNode(expr);
}

// 5. x*y + z, x->2
void test_nested_substitute() {
    ExprNode* mult = createBinary('*', createVariable("x"), createVariable("y"));
    ExprNode* expr = createBinary('+', mult, createVariable("z"));

    expr = substitute(expr, "x", 2);

    ExprNode* left = expr->data.binary.left;
    ExprNode* right = expr->data.binary.right;

    // x*y -> 2*y
    assert(left->type == NODE_BINARY);
    assert_number(left->data.binary.left, 2);
    assert_variable(left->data.binary.right, "y");

    
    assert_variable(right, "z");

    freeNode(expr);
}

// 6. f(x, y), x->1
void test_function_substitute() {
    ExprNode* args[2];
    args[0] = createVariable("x");
    args[1] = createVariable("y");

    ExprNode* func = createFunction("f", args, 2);

    func = substitute(func, "x", 1);

    assert_number(func->data.function.args[0], 1);
    
    assert_variable(func->data.function.args[1], "y");

    freeNode(func);
}

// 7. x + y, x->3, y->4
void test_multiple_substitute() {
    ExprNode* expr = createBinary('+', createVariable("x"), createVariable("y"));

    expr = substitute(expr, "x", 3);
    expr = substitute(expr, "y", 4);

    assert_number(expr->data.binary.left, 3);
    assert_number(expr->data.binary.right, 4);

    freeNode(expr);
}

// 8.  -(-x) -> -(-5)
void test_double_unary() {
    ExprNode* x = createVariable("x");
    ExprNode* inner = createUnary('-', x);
    ExprNode* expr = createUnary('-', inner);

    expr = substitute(expr, "x", 5);

    assert(expr->type == NODE_UNARY);
    ExprNode* operand = expr->data.unary.operand;
    assert(operand != NULL);
    assert(operand->type == NODE_UNARY);
    assert_number(operand->data.unary.operand, 5);

    freeNode(expr);
}

int main() {
    test_simple_variable();
    test_no_substitute();
    test_unary_substitute();
    test_binary_substitute();
    test_nested_substitute();
    test_function_substitute();
    test_multiple_substitute();
    test_double_unary();

    printf("All substitute tests passed\n");
    return 0;
}