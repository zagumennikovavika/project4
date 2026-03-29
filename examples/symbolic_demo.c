#include "student3.h"

void printExpr(ExprNode* node) {
    if (!node) return;

    switch(node->type) {
        case NODE_NUMBER:
            printf("%.2f", node->data.number);
            break;
        case NODE_VARIABLE:
            printf("%s", node->data.var_name);
            break;
        case NODE_UNARY:
            printf("(%c", node->data.unary.op);
            printExpr(node->data.unary.operand);
            printf(")");
            break;
        case NODE_BINARY:
            printf("(");
            printExpr(node->data.binary.left);
            printf(" %c ", node->data.binary.op);
            printExpr(node->data.binary.right);
            printf(")");
            break;
        case NODE_FUNCTION:
            printf("%s(", node->data.function.func_name);
            for (int i = 0; i < node->data.function.arg_count; i++) {
                printExpr(node->data.function.args[i]);
                if (i < node->data.function.arg_count - 1) printf(", ");
            }
            printf(")");
            break;
    }
}

int main() {
    // Example 1: (x + 0) * 1

    ExprNode* x = createVariable("x");
    ExprNode* zero = createNumber(0.0);
    ExprNode* one = createNumber(1.0);

    ExprNode* x_plus_0 = createBinary('+', x, zero);
    ExprNode* expr1 = createBinary('*', x_plus_0, one);

    printf("Исходное выражение 1: ");
    printExpr(expr1);
    printf("\n");

    ExprNode* simplified1 = simplify(copyTree(expr1));
    printf("После simplify: ");
    printExpr(simplified1);
    printf("\n");

    freeNode(expr1);
    freeNode(simplified1);

    // Example 2: (2 * x) + (3 * x)
    
    ExprNode* x1 = createVariable("x");
    ExprNode* x2 = createVariable("x");
    ExprNode* two = createNumber(2.0);
    ExprNode* three = createNumber(3.0);

    ExprNode* two_x = createBinary('*', two, x1);
    ExprNode* three_x = createBinary('*', three, x2);
    ExprNode* expr2 = createBinary('+', two_x, three_x);

    printf("\nИсходное выражение 2: ");
    printExpr(expr2);
    printf("\n");

    ExprNode* simplified2 = simplify(copyTree(expr2));
    printf("После simplify (объединение подобных): ");
    printExpr(simplified2);
    printf("\n");

    freeNode(expr2);
    freeNode(simplified2);

    // Example 3: (x^2 + x) / x
   
    ExprNode* x3 = createVariable("x");
    ExprNode* x4 = createVariable("x");
    ExprNode* x5 = createVariable("x");

    ExprNode* x_sq = createBinary('^', x3, createNumber(2.0));
    ExprNode* numerator = createBinary('+', x_sq, x4);
    ExprNode* expr3 = createBinary('/', numerator, x5);

    printf("\nИсходное выражение 3: ");
    printExpr(expr3);
    printf("\n");

    ExprNode* simplified3 = simplify(copyTree(expr3));
    printf("После simplify: ");
    printExpr(simplified3);
    printf("\n");

    ExprNode* deriv5 = differentiate(copyTree(expr3), "x");
    printf("Дифференцирование по x: ");
    printExpr(deriv5);
    printf("\n");

    freeNode(expr3);
    freeNode(simplified3);
    freeNode(deriv5);

    
    // Example 6: -(-x)
    ExprNode* x6 = createVariable("x");
    ExprNode* unary_minus_x = createUnary('-', x6);
    ExprNode* expr4 = createUnary('-', unary_minus_x);

    printf("\nИсходное выражение 4: ");
    printExpr(expr4);
    printf("\n");

    ExprNode* simplified4 = simplify(copyTree(expr4));
    printf("После simplify (удаление двойного минуса): ");
    printExpr(simplified4);
    printf("\n");

    freeNode(expr4);
    freeNode(simplified4);

    return 0;
}

