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

ExprNode* create_node(NodeType type){
    ExprNode *node = (ExprNode*)malloc(sizeof(ExprNode));
    if (node == NULL)
        return NULL;
    node->type = type;
    return node;
}

ExprNode* create_number_node(double val){
    ExprNode *node = create_node(NODE_NUMBER);
    if (node == NULL)
        return NULL;
    node->type = NODE_NUMBER;
    node->data.number = val;
    return node;
}

ExprNode* create_variable_node(char *name){
    ExprNode *node = create_node(NODE_VARIABLE);
    if (node == NULL)
        return NULL;
    node->data.var_name = strdup(name);
    return node;
}

ExprNode* create_unary_node(char op, ExprNode *operand){
    ExprNode *node = create_node(NODE_UNARY);
    if (node == NULL)
        return NULL;
    node->data.unary.op = op;
    node->data.unary.operand = operand;
    return node;
}

ExprNode* create_binary_node(char op, ExprNode *left, ExprNode *right){
    ExprNode *node = create_node(NODE_BINARY);
    if (node == NULL)
        return NULL;
    node->data.binary.op = op;
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

ExprNode* create_function_node(const char *name, ExprNode **args, int arg_count) {
    ExprNode *node = create_node(NODE_FUNCTION);
    if (node == NULL)
        return NULL;
    node->data.function.func_name = strdup(name);
    node->data.function.args = args;
    node->data.function.arg_count = arg_count;
    return node;
}

int is_math_function(const char *s) {
    const char *funcs[] = {"sin", "cos", "ln", "sqrt", "min", "max", NULL};
    for (int i = 0; funcs[i] != NULL; i++) {
        if (strcmp(s, funcs[i]) == 0) return 1;
    }
    return 0;
}

ExprNode* build_ast_from_postfix(const char* postfix){
    char* copy = strdup(postfix);
    char* fragment = strtok(copy, " ");
    ExprNode* buffer[BUFFER_NODE_SIZE];
    int index = -1;

    while(fragment != NULL){
        char *endptr;
        double val = strtod(fragment, &endptr);
        if (*endptr == '\0' && fragment != endptr) {
            ExprNode* node = create_number_node(val);
            if (node == NULL || index > BUFFER_NODE_SIZE - 1)
                goto cleanup;       
            buffer[++index] = node;
        }
        else if (is_math_function(fragment)) {
            int count = (strcmp(fragment, "min") == 0 || strcmp(fragment, "max") == 0) ? 2 : 1;        
            ExprNode **args = malloc(count * sizeof(ExprNode*));
            for (int i = count - 1; i >= 0; i--){
                args[i] = buffer[index--];
            }
            buffer[++index] = create_function_node(fragment, args, count);
        }
        else if (strlen(fragment) == 1 && strchr("+-*/^", fragment[0])) {
            ExprNode *right = buffer[index--];
            ExprNode *left = buffer[index--];
            buffer[++index] = create_binary_node(fragment[0], left, right);
        }
        else if (strlen(fragment) == 1 && fragment[0] == '~') {
            ExprNode *operand = buffer[index--];
            buffer[++index] = create_unary_node('~', operand);
        }
        else {
            buffer[++index] = create_variable_node(fragment);
        }
            
        fragment = strtok(NULL, " ");
    }

    if (index != 0)
        goto cleanup;
    free(copy);
    return buffer[0];

    cleanup:
        while(index >= 0)
            free_ast(buffer[index--]);
        free(copy);
        return NULL;

}


double variable_value(char *name, VariableTable *vars, int size_arr){
    double val;
    for (int i = 0; i < size_arr; i++){
        if (strcmp(vars[i].name, name) == 0){
            val = vars[i].var;
            return val;
        }
    }
    return 0;
}

int evaluate_ast(ExprNode *node, VariableTable *variables, int size_table, double *result) {
    if (!node) return -1;

    switch (node->type) {
        case NODE_NUMBER:
            *result = node->data.number;
            return 0;
        case NODE_VARIABLE:{
            double val = variable_value(node->data.var_name, variables, size_table);
            *result = val;
            return 0;
        }
        case NODE_UNARY:{
            double operand_val;
            if (evaluate_ast(node->data.unary.operand, variables, size_table, &operand_val) != 0)
                return -1;
            if (node->data.unary.op == '~' || node->data.unary.op == '-')
                *result = -operand_val;
            else 
                return -1;
            return 0;
        }
        case NODE_BINARY: {
            double left_val, right_val;
            if (evaluate_ast(node->data.binary.left, variables, size_table, &left_val) != 0) return -1;
            if (evaluate_ast(node->data.binary.right, variables, size_table, &right_val) != 0) return -1;

            switch (node->data.binary.op) {
                case '+': *result = left_val + right_val; break;
                case '-': *result = left_val - right_val; break;
                case '*': *result = left_val * right_val; break;
                case '/': 
                    if (right_val == 0)
                        return -1;
                    *result = left_val / right_val; 
                    break;
                case '^': *result = pow(left_val, right_val); break;
                default: return -1;
            }
            return 0;
        }
        case NODE_FUNCTION: {
            int count = node->data.function.arg_count;
            double *args_vals = malloc(sizeof(double) * count);
            for (int i = 0; i < count; i++) {
                if (evaluate_ast(node->data.function.args[i], variables, size_table, &args_vals[i]) != 0) {
                    free(args_vals);
                    return -1;
                }
            }
            char *fname = node->data.function.func_name;
            int status = 0;
            if (strcmp(fname, "sin") == 0) *result = sin(args_vals[0]);
            else if (strcmp(fname, "cos") == 0) *result = cos(args_vals[0]);
            else if (strcmp(fname, "ln") == 0) {
                if (args_vals[0] <= 0) status = -1;
                else *result = log(args_vals[0]);
            }
            else if (strcmp(fname, "sqrt") == 0) {
                if (args_vals[0] < 0) status = -1;
                else *result = sqrt(args_vals[0]);
            }
            else if (strcmp(fname, "min") == 0) *result = (args_vals[0] < args_vals[1]) ? args_vals[0] : args_vals[1];
            else if (strcmp(fname, "max") == 0) *result = (args_vals[0] > args_vals[1]) ? args_vals[0] : args_vals[1];
            else status = -1;
            free(args_vals);
            return status;
        }
    }
    return -1;
}
