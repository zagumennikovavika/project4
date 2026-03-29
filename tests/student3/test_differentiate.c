
#include "Student3.h"


void test_differentiate_number() {
    ExprNode* n = createNumber(5.0);
    ExprNode* dn = differentiate(n, "x");
    ExprNode* expected = createNumber(0.0);
    assert(expression_equal(dn, expected));
    printf("test_differentiate_number passed\n");
}

void test_differentiate_variable() {
    ExprNode* x = createVariable("x");
    ExprNode* y = createVariable("y");

    ExprNode* dx = differentiate(x, "x");
    ExprNode* dy = differentiate(y, "x");

    ExprNode* expected_dx = createNumber(1.0);
    ExprNode* expected_dy = createNumber(0.0);

    assert(expression_equal(dx, expected_dx));
    assert(expression_equal(dy, expected_dy));
    printf("test_differentiate_variable passed\n");
}

void test_differentiate_binary() {
    ExprNode* x = createVariable("x");
    ExprNode* n2 = createNumber(2.0);

    ExprNode* expr = createBinary('*', x, n2); 
    ExprNode* dexpr = differentiate(expr, "x");

    ExprNode* expected = createBinary('+',
                                     createBinary('*', copyTree(x), createNumber(0.0)),
                                     createBinary('*', copyTree(n2), createNumber(1.0)));
    assert(expression_equal(dexpr, expected));
    printf("test_differentiate_binary passed\n");
}

void test_differentiate_function() {
    ExprNode* x = createVariable("x");
    ExprNode* sinx = createFunction("sin", &x, 1);

    ExprNode* dsinx = differentiate(sinx, "x");

    ExprNode* arg_copy = copyTree(x);
    ExprNode* cosx = createFunction("cos", &arg_copy, 1);
    ExprNode* expected = createBinary('*', cosx, createNumber(1.0));

    assert(expression_equal(dsinx, expected));
    printf("test_differentiate_function passed\n");
}

int main() {
    test_differentiate_number();
    test_differentiate_variable();
    test_differentiate_binary();
    test_differentiate_function();

    printf("All tests passed!\n");
    return 0;
}