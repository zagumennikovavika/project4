
#include "Student3.h"

// Вспомогательная проверка числа
void assert_number(ExprNode* node, double expected) {
    assert(node != NULL);
    assert(node->type == NODE_NUMBER);
    assert(node->data.number == expected);
}

// Вспомогательная проверка переменной
void assert_variable(ExprNode* node, const char* name) {
    assert(node != NULL);
    assert(node->type == NODE_VARIABLE);
    assert(strcmp(node->data.var_name, name) == 0);
}

// ===== TESTS =====

// 1. Умножение на 0
void test_mul_zero() {
    ExprNode* expr = createBinary('*',
        createNumber(5),
        createNumber(0)
    );

    expr = simplify(expr);

    assert_number(expr, 0.0);
    freeNode(expr);
}

// 2. Умножение на 1
void test_mul_one() {
    ExprNode* expr = createBinary('*',
        createVariable("x"),
        createNumber(1)
    );

    expr = simplify(expr);

    assert_variable(expr, "x");
    freeNode(expr);
}

// 3. Сложение чисел
void test_add_numbers() {
    ExprNode* expr = createBinary('+',
        createNumber(2),
        createNumber(3)
    );

    expr = simplify(expr);

    assert_number(expr, 5.0);
    freeNode(expr);
}

// 4. x + x = 2x
void test_similar_terms() {
    ExprNode* expr = createBinary('+',
        createVariable("x"),
        createVariable("x")
    );

    expr = simplify(expr);

    assert(expr->type == NODE_BINARY);
    assert(expr->data.binary.op == '*');

    assert_number(expr->data.binary.left, 2.0);
    assert_variable(expr->data.binary.right, "x");

    freeNode(expr);
}

// 5. x - x = 0
void test_sub_same() {
    ExprNode* expr = createBinary('-',
        createVariable("x"),
        createVariable("x")
    );

    expr = simplify(expr);

    assert_number(expr, 0.0);
    freeNode(expr);
}

// 6. Деление на 1
void test_div_one() {
    ExprNode* expr = createBinary('/',
        createVariable("x"),
        createNumber(1)
    );

    expr = simplify(expr);

    assert_variable(expr, "x");
    freeNode(expr);
}

// 7. Деление одинаковых переменных: x/x = 1
void test_div_same() {
    ExprNode* expr = createBinary('/',
        createVariable("x"),
        createVariable("x")
    );

    expr = simplify(expr);

    assert_number(expr, 1.0);
    freeNode(expr);
}

// 8. Унарный минус числа
void test_unary_minus() {
    ExprNode* expr = createUnary('-',
        createNumber(5)
    );

    expr = simplify(expr);

    assert_number(expr, -5.0);
    freeNode(expr);
}

// 9. --x = x
void test_double_minus() {
    ExprNode* expr = createUnary('-',
        createUnary('-', createVariable("x"))
    );

    expr = simplify(expr);

    assert_variable(expr, "x");
    freeNode(expr);
}

// 10. Сложное выражение: (2 * x) + (3 * x) = 5x
void test_complex() {
    ExprNode* left = createBinary('*', createNumber(2), createVariable("x"));
    ExprNode* right = createBinary('*', createNumber(3), createVariable("x"));

    ExprNode* expr = createBinary('+', left, right);

    expr = simplify(expr);

    assert(expr->type == NODE_BINARY);
    assert(expr->data.binary.op == '*');

    assert_number(expr->data.binary.left, 5.0);
    assert_variable(expr->data.binary.right, "x");

    freeNode(expr);
}

// ===== MAIN =====
int main() {
    test_mul_zero();
    test_mul_one();
    test_add_numbers();
    test_similar_terms();
    test_sub_same();
    test_div_one();
    test_div_same();
    test_unary_minus();
    test_double_minus();
    test_complex();

    printf("All tests passed\n");
    return 0;
}