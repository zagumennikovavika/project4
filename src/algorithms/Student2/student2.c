#include "student2.h"

void free_ast(ExprNode* node){
    if (!node)
        return;
    switch (node->type){
        case NODE_VARIABLE:
            free(node->data.var_name);
            break;
        case NODE_UNARY:
            free_ast(node->data.unary.operand);
            break;
        case NODE_BINARY:
            free_ast(node->data.binary.left);
            free_ast(node->data.binary.right);
            break;
        case NODE_FUNCTION:
            int count = node->data.function.arg_count;
            for (int i = 0; i < count; i++)
                free_ast(node->data.function.args[i]);
            free(node->data.function.args);
            free(node->data.function.func_name);
            break;
    }
    free(node);
}

ExprNode* create_node_number(double val){
    ExprNode* node = malloc(sizeof(ExprNode));
    if (node == NULL)
        return NULL;           //Error message 
    node->type = NODE_NUMBER;
    node->data.number = val;
    return node;
}

//
typedef struct {
    const char *name;
    int arity;
} FunctionInfo;

static const FunctionInfo supported_functions[] = {
    {"sin", 1}, {"cos", 1}, {"ln", 1}, {"sqrt", 1},{NULL, 0}
};

const FunctionInfo* get_function_info(const char *token) {
    for (int i = 0; supported_functions[i].name != NULL; i++) {
        if (strcmp(token, supported_functions[i].name) == 0) return &supported_functions[i];
    }
    return NULL;
}

ExprNode* build_ast_from_postfix(const char *postfix){
    ExprNode* stack[BUFFER_NODE_SIZE];
    int top = -1;
    char *copy = strdup(postfix);
    char *token = strtok(copy, " ");

    while (token != NULL) {
        char *endptr;
        double val = strtod(token, &endptr);
        if (*endptr == '\0' && token != endptr) {
            if (top >= 127)
                goto cleanup;
            stack[++top] = create_node_number(val);
        }
        else if (isalpha(token[0])) {
            const FunctionInfo *info = get_function_info(token);
            if (info) {
                if (top < info->arity - 1)
                    goto cleanup;
                ExprNode *node = malloc(sizeof(ExprNode));
                node->type = NODE_FUNCTION;
                node->data.function.func_name = strdup(token);
                node->data.function.arg_count = info->arity;
                node->data.function.args = malloc(sizeof(ExprNode*) * info->arity);
                for (int i = info->arity - 1; i >= 0; i--) node->data.function.args[i] = stack[top--];
                stack[++top] = node;
            } else {
                ExprNode *node = malloc(sizeof(ExprNode));
                node->type = NODE_VARIABLE;
                node->data.var_name = strdup(token);
                stack[++top] = node;
            }
        } 
        else {
                if (top < 1) goto cleanup;
                ExprNode *node = malloc(sizeof(ExprNode));
                node->type = NODE_BINARY;
                node->data.binary.op = op;
                node->data.binary.right = stack[top--];
                node->data.binary.left = stack[top--];
                stack[++top] = node;
            }
        token = strtok(NULL, " ");
    }
    
    if (index != 0)
        goto cleanup;
    free(copy);
    return buffer[0];

    cleanup:                             // Error message
        while(index >= 0)
            free_ast(buffer[index--]);
        free(copy);
        return NULL;

}
