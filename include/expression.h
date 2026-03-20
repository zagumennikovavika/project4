#ifndef EXPRESSION_H
#define EXPRESSION_H

typedef enum {
    ERR_OK = 0,
    ERR_SYNTAX,
    ERR_UNKNOWN_VARIABLE,
    ERR_DIVISION_BY_ZERO,
    ERR_MEMORY,
    ERR_INVALID_ARG
} ErrorCode;

typedef struct {
    ErrorCode code;
    char message[256];
} ErrorInfo;


typedef struct Expression Expression;


Expression* expr_parse_infix(const char *infix, ErrorInfo *err);
void expr_free(Expression *expr);



#endif