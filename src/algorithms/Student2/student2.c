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
