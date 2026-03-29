
#include "Student3.h"

void test_expression_equal_numbers() {
    ExprNode* n1 = createNumber(5.0);
    ExprNode* n2 = createNumber(5.0);
    ExprNode* n3 = createNumber(3.0);

    assert(expression_equal(n1, n2)); 
    assert(!expression_equal(n1, n3)); 
    printf("test_expression_equal_numbers passed\n");
}

void test_expression_equal_variables() {
    ExprNode* x1 = createVariable("x");
    ExprNode* x2 = createVariable("x");
    ExprNode* y = createVariable("y");

    assert(expression_equal(x1, x2)); 
    assert(!expression_equal(x1, y)); 
    printf("test_expression_equal_variables passed\n");
}

void test_expression_equal_unary() {
    ExprNode* x = createVariable("x");
    ExprNode* neg1 = createUnary('-', x);
    ExprNode* neg2 = createUnary('-', copyTree(x));
    ExprNode* plus = createUnary('+', x);

    assert(expression_equal(neg1, neg2)); 
    assert(!expression_equal(neg1, plus)); 
    printf("test_expression_equal_unary passed\n");
}

void test_expression_equal_binary() {
    ExprNode* x = createVariable("x");
    ExprNode* y = createVariable("y");

    ExprNode* add1 = createBinary('+', x, y);
    ExprNode* add2 = createBinary('+', copyTree(x), copyTree(y));
    assert(expression_equal(add1, add2));

    ExprNode* add3 = createBinary('+', copyTree(y), copyTree(x));
    assert(expression_equal(add1, add3));

    ExprNode* sub1 = createBinary('-', x, y);
    ExprNode* sub2 = createBinary('-', copyTree(y), copyTree(x));
    assert(!expression_equal(sub1, sub2));

    printf("test_expression_equal_binary passed\n");
}

void test_expression_equal_function() {
    ExprNode* x = createVariable("x");

    ExprNode* sin1 = createFunction("sin", &x, 1);
    ExprNode* sin2 = createFunction("sin", &x, 1);
    ExprNode* cos1 = createFunction("cos", &x, 1);

    assert(expression_equal(sin1, sin2)); 
    assert(!expression_equal(sin1, cos1)); 

    printf("test_expression_equal_function passed\n");
}

void test_expression_equal_null() {
    assert(expression_equal(NULL, NULL));
    ExprNode* x = createVariable("x");
    assert(!expression_equal(x, NULL));
    assert(!expression_equal(NULL, x));
    printf("test_expression_equal_null passed\n");
}

int main() {
    test_expression_equal_numbers();
    test_expression_equal_variables();
    test_expression_equal_unary();
    test_expression_equal_binary();
    test_expression_equal_function();
    test_expression_equal_null();

    printf("All expression_equal tests passed!\n");
    return 0;
}