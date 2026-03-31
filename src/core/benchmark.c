#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/expression.h"

/* Structure for benchmark results */
typedef struct {
    double parseTimeMs;
    double evalTimeMs;
    long operationsCount;
    long totalOperations;
    double result;
} BenchmarkResult;

/* Helper function for measurements */
BenchmarkResult BenchmarkExpression(const char* infix, const Variable* vars, int iterations) {
    BenchmarkResult res = {0, 0, 0, 0, 0};
    ErrorInfo err = {0, ""};

    clock_t t0 = clock();
    Expression* expr = ExprParseInfix(infix, &err);
    clock_t parseTime = clock() - t0;
    res.parseTimeMs = (double)parseTime / CLOCKS_PER_SEC * 1000.0;

    if (!expr) {
        printf("    PARSE ERROR: %s\n", err.message);
        return res;
    }

    double result = 0.0;
    long totalOps = 0;
    
    clock_t t1 = clock();
    for (int i = 0; i < iterations; i++) {
        if (ExprEvaluate(expr, vars, &result, &err) != 0) {
            printf("    EVAL ERROR: %s\n", err.message);
            break;
        }
        totalOps += ExprGetOperations(expr);
    }
    clock_t evalTime = clock() - t1;
    
    res.evalTimeMs = (double)evalTime / CLOCKS_PER_SEC * 1000.0;
    res.operationsCount = totalOps / iterations;
    res.totalOperations = totalOps;
    res.result = result;

    ExprFree(expr);
    return res;
}

/* Benchmark 1: Basic arithmetic */
void BenchBasic(void) {
    printf("\n=== 1. Basic arithmetic (1,000,000 iterations) ===\n");
    
    struct {
        const char* name;
        const char* expr;
    } 
    tests[] = {
        {"3+4", "3+4"},
        {"3+4*2", "3+4*2"},
        {"(3+4)*2", "(3+4)*2"},
        {"3+4*2/(1-5)", "3+4*2/(1-5)"},
        {"2^10", "2^10"},
        {"-5+3*2", "-5+3*2"},
        {"1+2+3+4+5+6+7+8", "1+2+3+4+5+6+7+8"},
        {NULL, NULL}
    };
    
    int iterations = 1000000;
    
    for (int i = 0; tests[i].name != NULL; i++) {
        BenchmarkResult res = BenchmarkExpression(tests[i].expr, NULL, iterations);
        
        if (res.parseTimeMs == 0 && res.evalTimeMs == 0 && res.operationsCount == 0) {
            printf("  %-20s | PARSE/EVAL ERROR\n", tests[i].name);
        } else {
            printf("  %-20s | parse: %6.3f ms | eval: %6.3f ms | ops: %2ld | result: %8.4f\n",
                   tests[i].name, res.parseTimeMs, res.evalTimeMs, 
                   res.operationsCount, res.result);
        }
    }
}

/* Benchmark 2: Variables via postfix */
void BenchVariables(void) {
    printf("\n=== 2. Variables via POSTFIX (1,000,000 iterations) ===\n");
    
    Variable varsX[] = {{"x", 5.0}, {"", 0}};
    Variable varsXY[] = {{"x", 3.0}, {"y", 2.0}, {"", 0}};
    
    struct {
        const char* name;
        const char* postfix;
        const Variable* vars;
    } tests[] = {
        {"x+5", "x 5 +", varsX},
        {"x*2+1", "x 2 * 1 +", varsX},
        {"x^2+2*x", "x 2 ^ 2 x * +", varsX},
        {"x+y", "x y +", varsXY},
        {NULL, NULL, NULL}
    };
    
    int iterations = 1000000;
    
    for (int i = 0; tests[i].name != NULL; i++) {
        ErrorInfo err = {0, ""};
        
        clock_t t0 = clock();
        Expression* expr = ExprParsePostfix(tests[i].postfix, &err);
        clock_t parseTime = clock() - t0;
        
        if (!expr) {
            printf("  %-20s | PARSE ERROR: %s\n", tests[i].name, err.message);
            continue;
        }
        
        double result = 0.0;
        long totalOps = 0;
        
        clock_t t1 = clock();
        for (int j = 0; j < iterations; j++) {
            ExprEvaluate(expr, tests[i].vars, &result, &err);
            totalOps += ExprGetOperations(expr);
        }
        clock_t evalTime = clock() - t1;
        
        double evalMs = (double)evalTime / CLOCKS_PER_SEC * 1000.0;
        
        printf("  %-20s | parse: %6.3f ms | eval: %6.3f ms | ops: %2ld | result: %8.4f\n",
               tests[i].name, (double)parseTime / CLOCKS_PER_SEC * 1000.0,
               evalMs, totalOps / iterations, result);
        
        ExprFree(expr);
    }
}

/* Benchmark 3: Mathematical functions (now working!) */
void BenchFunctions(void) {
    printf("\n=== 3. Math functions via POSTFIX (1,000,000 iterations) ===\n");
    
    struct {
        const char* name;
        const char* postfix;
    } tests[] = {
        {"sin(1.57)", "1.57 sin"},
        {"cos(1.57)", "1.57 cos"},
        {"sqrt(16)", "16 sqrt"},
        {"ln(2.718)", "2.718 ln"},
        {NULL, NULL}
    };
    
    int iterations = 1000000;
    
    for (int i = 0; tests[i].name != NULL; i++) {
        ErrorInfo err = {0, ""};
        
        clock_t t0 = clock();
        Expression* expr = ExprParsePostfix(tests[i].postfix, &err);
        clock_t parseTime = clock() - t0;
        
        if (!expr) {
            printf("  %-20s | PARSE ERROR: %s\n", tests[i].name, err.message);
            continue;
        }
        
        double result = 0.0;
        long totalOps = 0;
        
        clock_t t1 = clock();
        for (int j = 0; j < iterations; j++) {
            ExprEvaluate(expr, NULL, &result, &err);
            totalOps += ExprGetOperations(expr);
        }
        clock_t evalTime = clock() - t1;
        
        double evalMs = (double)evalTime / CLOCKS_PER_SEC * 1000.0;
        
        printf("  %-20s | parse: %6.3f ms | eval: %6.3f ms | ops: %2ld | result: %8.4f\n",
               tests[i].name, (double)parseTime / CLOCKS_PER_SEC * 1000.0, 
               evalMs, totalOps / iterations, result);
        
        ExprFree(expr);
    }
}

/* Benchmark 4: Parse performance */
void BenchParseOnly(void) {
    printf("\n=== 4. Parse performance ===\n");
    
    const char* expr = "3+4*2/(1-5)";
    int iterations = 100000;
    
    clock_t t0 = clock();
    for (int i = 0; i < iterations; i++) {
        ErrorInfo err = {0, ""};
        Expression* e = ExprParseInfix(expr, &err);
        ExprFree(e);
    }
    clock_t elapsed = clock() - t0;
    double ms = (double)elapsed * 1000.0 / CLOCKS_PER_SEC;
    
    printf("  Parsing '%s' %d times: %.3f ms\n", expr, iterations, ms);
    printf("  Average parse time: %.3f us\n", ms * 1000.0 / iterations);
}

/* Benchmark 5: Performance comparison with and without simplification */
void BenchSimplificationComparison(void) {
    printf("\n=== 5. Performance: With vs Without Simplification ===\n");
    
    struct {
        const char* name;
        const char* postfix;
        const char* infix;
    } tests[] = {
        {"x*2+3*x-x", "x 2 * 3 x * + x -", "x*2 + 3*x - x"},
        {"(x+y)*2+(x+y)*3", "x y + 2 * x y + 3 * +", "(x+y)*2 + (x+y)*3"},
        {NULL, NULL, NULL}
    };
    
    Variable varsX[] = {{"x", 5.0}, {"", 0}};
    Variable varsXY[] = {{"x", 3.0}, {"y", 2.0}, {"", 0}};
    
    int iterations = 1000000;
    
    for (int i = 0; tests[i].name != NULL; i++) {
        ErrorInfo err = {0, ""};
        
        Variable* vars = NULL;
        if (strstr(tests[i].postfix, "y") != NULL) {
            vars = varsXY;
        } else if (strstr(tests[i].postfix, "x") != NULL) {
            vars = varsX;
        }
        
        // Without simplification
        Expression* expr_original = ExprParsePostfix(tests[i].postfix, &err);
        if (!expr_original) {
            printf("  %-30s | PARSE ERROR\n", tests[i].name);
            continue;
        }
        
        double result = 0.0;
        
        clock_t t1 = clock();
        for (int j = 0; j < iterations; j++) {
            ExprEvaluate(expr_original, vars, &result, &err);
        }
        clock_t t2 = clock();
        
        double time_without = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000.0;
        
        // With simplification
        Expression* expr_simplified = ExprSimplify(expr_original, &err);
        if (!expr_simplified) {
            printf("  %-30s | SIMPLIFY ERROR\n", tests[i].name);
            ExprFree(expr_original);
            continue;
        }
        
        clock_t t3 = clock();
        for (int j = 0; j < iterations; j++) {
            ExprEvaluate(expr_simplified, vars, &result, &err);
        }
        clock_t t4 = clock();
        
        double time_with = (double)(t4 - t3) / CLOCKS_PER_SEC * 1000.0;
        
        printf("\n  Expression: %s\n", tests[i].infix);
        printf("  Postfix:    %s\n", tests[i].postfix);
        printf("  Without simplification: %8.3f ms\n", time_without);
        printf("  With simplification:    %8.3f ms\n", time_with);
        printf("  Speedup:                %.2fx\n", time_without / time_with);
        
        ExprFree(expr_simplified);
        ExprFree(expr_original);
    }
}

int main(void) {
    printf("   EXPRESSION LIBRARY BENCHMARK (Full Mode - AST enabled)\n");
    printf("   =====================================================\n");

    BenchBasic();
    BenchVariables();
    BenchFunctions();
    BenchParseOnly();
    BenchSimplificationComparison();
    
    return 0;
}