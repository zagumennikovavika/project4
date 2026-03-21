#ifndef PROJECT3_STUDENT1_H
#define PROJECT3_STUDENT1_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define STACK_LEN 1000

int IsOperator(char c);
int GetPrecedence(char c);
char GetAssociativity(char c);
double CharToDouble(char c);
int IncorrectExp(int param1, int param2);
int BufferOverflow(int param1, int param2);
int StackOverflow(int param1, int param2);
double StringToNumber(char *number, int len);
int IsNumberChar(char c);
int ValidateNumber(const char *number, int len);
int InfixToPostfix(const char *infix, char *postfix, size_t postfixLen);
int  EvaluatePostfix(const char *postfix, double *result,  long *numOperations);
int ApplyOperator(char op, double *valStack, int *valTop, long *numOperations);
int EvaluateInfix(const char *expression, double *result, long *numOperations);
#endif //PROJECT3_STUDENT1_H
