
#include "Student1.h"

int TestsPassed = 0;
int TestsFailed = 0;

#define START(name) printf("\n %s \n\n", name)
#define CHECK(condition, message) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s\n", message); \
            TestsFailed++; \
            return 1; \
        } else { \
            printf("PASS: %s\n", message); \
        } \
    } while(0)
#define END() \
    do { \
        TestsPassed++; \
        printf("\nTest passed\n\n"); \
        return 0; \
    } while(0)


// Helper for postfix comparison
int PostfixEqual(const char* actual, const char* expected) {
    int i = 0, j = 0;
    while (actual[i] && expected[j]) {
        if (actual[i] == ' ' && expected[j] == ' ') {
            i++;
            j++;
            continue;
        }
        if (actual[i] != expected[j])
            return 0;
        i++;
        j++;
    }
    while (actual[i] == ' ')
        i++;
    while (expected[j] == ' ')
        j++;
    return actual[i] == '\0' && expected[j] == '\0';
}


int TestValidateNumber() {
    START("ValidateNumber");

    CHECK(ValidateNumber("123", 3) == 0, "valid: 123");
    CHECK(ValidateNumber("0", 1) == 0, "valid: 0");
    CHECK(ValidateNumber("3.14", 4) == 0, "valid: 3.14");
    CHECK(ValidateNumber("0.5", 3) == 0, "valid: 0.5");
    CHECK(ValidateNumber("0123", 4) == -1, "invalid: leading zero");
    CHECK(ValidateNumber("1.2.3", 5) == -1, "invalid: multiple dots");
    CHECK(ValidateNumber(".123", 4) == -1, "invalid: dot at start");
    CHECK(ValidateNumber("123.", 4) == -1, "invalid: dot at end");
    CHECK(ValidateNumber("12a3", 4) == -1, "invalid: letter");

    END();
}


int TestInfixToPostfix() {
    START("InfixToPostfix");

    char postfix[100];
    int res;


    InfixToPostfix("3+5", postfix, 100);
    CHECK(PostfixEqual(postfix, "3 5 + "), "3+5 -> 3 5 +");

    InfixToPostfix("3+4*5", postfix, 100);
    CHECK(PostfixEqual(postfix, "3 4 5 * + "), "3+4*5 -> 3 4 5 * +");

    InfixToPostfix("(3+5)*2", postfix, 100);
    CHECK(PostfixEqual(postfix, "3 5 + 2 * "), "(3+5)*2 -> 3 5 + 2 *");

    InfixToPostfix("2^3^2", postfix, 100);
    CHECK(PostfixEqual(postfix, "2 3 2 ^ ^ "), "2^3^2 -> 2 3 2 ^ ^ (right assoc)");

    // Unary operators
    InfixToPostfix("-5", postfix, 100);
    CHECK(PostfixEqual(postfix, "5 ~ "), "-5 -> 5 ~");

    InfixToPostfix("--5", postfix, 100);
    CHECK(PostfixEqual(postfix, "5 ~ ~ "), "--5 -> 5 ~ ~(cancels)");

    InfixToPostfix("---5", postfix, 100);
    CHECK(PostfixEqual(postfix, "5 ~ ~ ~ "), "---5 -> 5 ~ ~ ~ ");

    InfixToPostfix("+++5", postfix, 100);
    CHECK(PostfixEqual(postfix, "5 "), "+++5 -> 5 (ignored)");

    InfixToPostfix("+-5", postfix, 100);
    CHECK(PostfixEqual(postfix, "5 ~ "), "+-5 -> 5 ~");

    InfixToPostfix("-+5", postfix, 100);
    CHECK(PostfixEqual(postfix, "5 ~ "), "-+5 -> 5 ~");

    InfixToPostfix("-+-5", postfix, 100);
    CHECK(PostfixEqual(postfix, "5 ~ ~ "), "-+-5 -> 5 ~ ~ ");

    InfixToPostfix("5--3", postfix, 100);
    CHECK(PostfixEqual(postfix, "5 3 ~ - "), "5--3 -> 5 3 ~ -");

    InfixToPostfix("5---3", postfix, 100);
    CHECK(PostfixEqual(postfix, "5 3 ~ ~ - "), "5---3 -> 5 3 ~ ~ -");

    InfixToPostfix("-2*-3", postfix, 100);
    CHECK(PostfixEqual(postfix, "2 ~ 3 ~ * "), "-2*-3 -> 2 ~ 3 ~ *");

    InfixToPostfix("-(3+5)", postfix, 100);
    CHECK(PostfixEqual(postfix, "3 5 + ~ "), "-(3+5) -> 3 5 + ~");

    // Invalid expressions
    InfixToPostfix("3++5", postfix, 100);
    CHECK(PostfixEqual(postfix, "3 5 + "), "3++5 -> 3 5 +");

    res = InfixToPostfix("3**5", postfix, 100);
    CHECK(res == -1, "invalid: 3**5");

    res = InfixToPostfix("(3+5", postfix, 100);
    CHECK(res == -1, "invalid: missing )");

    res = InfixToPostfix("3+5)", postfix, 100);
    CHECK(res == -1, "invalid: extra )");

    END();
}


int TestEvaluatePostfix() {
    START("EvaluatePostfix");

    double result;
    long ops;

    EvaluatePostfix("123 ", &result, &ops);
    CHECK(fabs(result - 123) == 0, "123 -> 123");

    EvaluatePostfix("3 5 + ", &result, &ops);
    CHECK(fabs(result - 8) == 0, "3 5 + -> 8");

    EvaluatePostfix("3 4 5 * + ", &result, &ops);
    CHECK(fabs(result - 23) == 0, "3 4 5 * + -> 23");

    EvaluatePostfix("2 3 2 ^ ^ ", &result, &ops);
    CHECK(fabs(result - 512) == 0, "2 3 2 ^ ^ -> 512");

    EvaluatePostfix("5 ~ ", &result, &ops);
    CHECK(fabs(result + 5) == 0, "5 ~ -> -5");

    EvaluatePostfix("5 ~ ~ ", &result, &ops);
    CHECK(fabs(result - 5) == 0, "5 ~ ~ -> 5");

    EvaluatePostfix("3 ~ 5 + ", &result, &ops);
    CHECK(fabs(result - 2) == 0, "3 ~ 5 + -> 2");

    EvaluatePostfix("5 3 ~ - ", &result, &ops);
    CHECK(fabs(result - 8) < 1e-9, "5 3 ~ - -> 8");

    EvaluatePostfix("2 ~ 3 ~ * ", &result, &ops);
    CHECK(fabs(result - 6) == 0, "2 ~ 3 ~ * -> 6");

    EvaluatePostfix("11 2 ~ 3 ~ / + ", &result, &ops);
    CHECK(fabs( result - 11.666667) < 1e-6, "11 2 ~ 3 ~ / +  -> 11.666667");

    int res = EvaluatePostfix("5 0 / ", &result, &ops);
    CHECK(res == -1, "division by zero -> error");

    END();
}

int TestEvaluateInfix() {
    START("EvaluateInfix");

    double result;
    long ops;


    EvaluateInfix("3+5", &result, &ops);
    CHECK(fabs(result - 8) == 0 , "3+5 = 8");

    EvaluateInfix("3+4*5", &result, &ops);
    CHECK(fabs(result - 23) == 0, "3+4*5 = 23");

    EvaluateInfix("(3+5)*2", &result, &ops);
    CHECK(fabs(result - 16) == 0, "(3+5)*2 = 16");

    EvaluateInfix("2^3^2", &result, &ops);
    CHECK(fabs(result - 512) == 0, "2^3^2 = 512 (right assoc)");

    // Unary operators
    EvaluateInfix("-5", &result, &ops);
    CHECK(fabs(result + 5) == 0, "-5 = -5");

    EvaluateInfix("--5", &result, &ops);
    CHECK(fabs(result - 5) == 0, "--5 = 5");

    EvaluateInfix("---5", &result, &ops);
    CHECK(fabs(result + 5) == 0, "---5 = -5");

    EvaluateInfix("----5", &result, &ops);
    CHECK(fabs(result - 5) == 0, "----5 = 5");

    EvaluateInfix("+++5", &result, &ops);
    CHECK(fabs(result - 5) == 0, "+++5 = 5");

    EvaluateInfix("+-5", &result, &ops);
    CHECK(fabs(result + 5) == 0, "+-5 = -5");

    EvaluateInfix("-+5", &result, &ops);
    CHECK(fabs(result + 5) == 0, "-+5 = -5");

    EvaluateInfix("--+-5", &result, &ops);
    CHECK(fabs(result + 5) == 0, "--+-5 = -5");

    EvaluateInfix("5--3", &result, &ops);
    CHECK(fabs(result - 8) == 0, "5--3 = 8");

    EvaluateInfix("5---3", &result, &ops);
    CHECK(fabs(result - 2) == 0, "5---3 = 2");

    EvaluateInfix("-2*-3", &result, &ops);
    CHECK(fabs(result - 6) == 0, "-2*-3 = 6");

    EvaluateInfix("--2*--3", &result, &ops);
    CHECK(fabs(result - 6) == 0, "--2*--3 = 6");

    EvaluateInfix("-(3+5)", &result, &ops);
    CHECK(fabs(result + 8) == 0, "-(3+5) = -8");

    EvaluateInfix("--(3+5)", &result, &ops);
    CHECK(fabs(result - 8) == 0, "--(3+5) = 8");

    EvaluateInfix("-(-3)", &result, &ops);
    CHECK(fabs(result - 3) == 0, "-(-3) = 3");

    EvaluateInfix("-3.5+2.5", &result, &ops);
    CHECK(fabs(result + 1) == 0, "-3.5+2.5 = -1");

    EvaluateInfix("58.4*7-3*8/11.24", &result, &ops);
    double expected = 58.4 * 7 - 3.0 * 8.0 / 11.24;
    CHECK(fabs(result - expected) < 1e-6, "res: 406.664769");

    int res;

    res = EvaluateInfix("3**5", &result, &ops);
    CHECK(res == -1, "invalid: 3**5");

    res = EvaluateInfix("(3+5", &result, &ops);
    CHECK(res == -1, "invalid: missing )");

    res = EvaluateInfix("3+5)", &result, &ops);
    CHECK(res == -1, "invalid: extra )");

    res = EvaluateInfix("5/0", &result, &ops);
    CHECK(res == -1, "invalid: division by zero");

    END();
}


int main() {


    TestValidateNumber();
    TestInfixToPostfix();
    TestEvaluatePostfix();
    TestEvaluateInfix();

    printf("\n========================================\n");
    printf("RESULTS: %d passed, %d failed\n", TestsPassed, TestsFailed);
    printf("========================================\n");

    return TestsFailed > 0 ? 1 : 0;
}
