#include <stdio.h>
#include <stdlib.h>  // обязательно для malloc и atoi
#include <ctype.h>



typedef enum {
    NODE_NUMBER, NODE_VARIABLE, 
    NODE_UNARY, NODE_BINARY, 
    NODE_FUNCTION 
} NodeType;

typedef struct ExprNode {
    NodeType type;
    union {
        double number;                // для NODE_NUMBER
        char *var_name;                // для NODE_VARIABLE
        
        struct {                       // для NODE_UNARY
            struct ExprNode *operand;
            char op;                    // например '-'
        } unary;

        struct {                       // для NODE_BINARY
            struct ExprNode *left;
            struct ExprNode *right;
            char op;                    // '+', '-', '*', '/', '^'
        } binary;

        struct {                       // для NODE_FUNCTION
            struct ExprNode **args;
            int arg_count;
            char *func_name;            // "sin", "cos" и т.д.
        } function;
        
    } data;
} ExprNode;


ExprNode* createNumber(double value) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    if (!node) return NULL;

    node->type = NODE_NUMBER;
    node->data.number = value;

    return node;
}

ExprNode* createVariable(const char* name) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    if (!node) return NULL;

    node->type = NODE_VARIABLE;

    node->data.var_name = (char*)malloc(strlen(name) + 1);
    if (!node->data.var_name) {
        free(node);
        return NULL;
    }

    strcpy(node->data.var_name, name);

    return node;
}

ExprNode* createUnary(char op, ExprNode* operand) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    if (!node) return NULL;

    node->type = NODE_UNARY;
    node->data.unary.op = op;
    node->data.unary.operand = operand;

    return node;
}

ExprNode* createUnary(char op, ExprNode* operand) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    if (!node) return NULL;

    node->type = NODE_UNARY;
    node->data.unary.op = op;
    node->data.unary.operand = operand;

    return node;
}

ExprNode* createBinary(char op, ExprNode* left, ExprNode* right) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    if (!node) return NULL;

    node->type = NODE_BINARY;
    node->data.binary.op = op;
    node->data.binary.left = left;
    node->data.binary.right = right;

    return node;
}


ExprNode* createFunction(const char* name, ExprNode** args, int arg_count) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    if (!node) return NULL;

    node->type = NODE_FUNCTION;

    node->data.function.func_name = (char*)malloc(strlen(name) + 1);
    if (!node->data.function.func_name) {
        free(node);
        return NULL;
    }

    strcpy(node->data.function.func_name, name);

    node->data.function.args = args;
    node->data.function.arg_count = arg_count;

    return node;
}




void freeNode(ExprNode* node) {
    if (node == NULL)
        return;

    switch (node->type) {

        case NODE_NUMBER:
            // ничего дополнительно освобождать не нужно
            break;

        case NODE_VARIABLE:
            // освобождаем имя переменной, если оно выделялось через malloc
            free(node->data.var_name);
            break;

        case NODE_UNARY:
            freeNode(node->data.unary.operand);
            break;

        case NODE_BINARY:
            freeNode(node->data.binary.left);
            freeNode(node->data.binary.right);
            break;

        case NODE_FUNCTION:
            for (int i = 0; i < node->data.function.arg_count; i++) 
                freeNode(node->data.function.args[i]);
            
            free(node->data.function.args);     // массив аргументов
            free(node->data.function.func_name); // имя функции (если malloc)
            break;
    }

    free(node);
}


ExprNode* isMulti(ExprNode* node){
    if (node == NULL)
        return NULL;
    
    if (node -> type != NODE_BINARY)
        return node;

    if (node -> data.binary.left == NULL ||
        node -> data.binary.right == NULL)
        return node;
    
    
    ExprNode* left = node -> data.binary.left; 
    ExprNode* right = node -> data.binary.right;

    if ((left -> type == NODE_NUMBER && left -> data.number == 0.0) ||
         (right -> type == NODE_NUMBER && right -> data.number == 0.0)){
            freeNode(node);

            node = createNumber(0.0);
            return node;
    }

    if (left -> type == NODE_NUMBER && left -> data.number == 1.0){ 
        ExprNode* result = right;   // сохраняем правого ребенка
        node->data.binary.right = NULL; // отсоединяем его от родителя

        freeNode(node);             // безопасно удаляем родителя
        return result;              // возвращаем правого ребенка
    }
    
    if (right -> type == NODE_NUMBER && right -> data.number == 1.0){
        ExprNode* result = left;   
        node->data.binary.left = NULL; 

        freeNode(node);             
        return result;              
    }

    if (left -> type == NODE_NUMBER && left -> data.number == -1.0){
        ExprNode* result = right;   
        node->data.binary.right = NULL; 

        freeNode(node);      
        return createUnary('-', result);              
    }

    
    if (right -> type == NODE_NUMBER && right -> data.number == -1.0){
        ExprNode* result = left;   
        node->data.binary.left = NULL; 

        freeNode(node);             
        return createUnary('-', result);              
    }

    if (left -> type == NODE_NUMBER && right -> type == NODE_NUMBER){
            double num =  (left -> data.number) * (right -> data.number);

            freeNode(node);
            node = createNumber(num);

            return node;
    }

    return node;
}

ExprNode* isAddit(ExprNode* node){
    if (node == NULL)
        return NULL;
    
    if (node -> type != NODE_BINARY)
        return node;

    if (node -> data.binary.left == NULL ||
        node -> data.binary.right == NULL)
        return node;
    
    ExprNode* left = node -> data.binary.left; 
    ExprNode* right = node -> data.binary.right;

    if (left -> type == NODE_NUMBER && left -> data.number == 0.0){
        ExprNode* result = right;
        node->data.binary.right = NULL;

        freeNode(node);
        return result;
    }

    
    if (right -> type == NODE_NUMBER && right -> data.number == 0.0){
        ExprNode* result = left;
        node->data.binary.left = NULL;

        freeNode(node);
        return result;
    }

    if (left -> type == NODE_NUMBER && right -> type == NODE_NUMBER){
        double num = (left -> data.number) + (right -> data.number);

        freeNode(node);
        node = createNumber(num);

        return node;
    }

    return node;
}

ExprNode* isSubt(ExprNode* node){
    if (node == NULL)
        return NULL;
    
    if (node -> type != NODE_BINARY)
        return node;

    if (node -> data.binary.left == NULL ||
        node -> data.binary.right == NULL)
        return node;
    
    ExprNode* left = node -> data.binary.left; 
    ExprNode* right = node -> data.binary.right;

    if (left -> type == NODE_NUMBER && left -> data.number == 0.0){
        ExprNode* result = right;
        node -> data.binary.right = NULL;
        
        freeNode(node);
        return createUnary('-', result);
    }

    if (right -> type == NODE_NUMBER && right -> data.number == 0.0){
        ExprNode* result = left;
        node -> data.binary.left = NULL;

        freeNode(node);
        return result;
    }

    if (left -> type == NODE_NUMBER && right -> type == NODE_NUMBER){
        double num = (left -> data.number) - (right -> data.number);

        freeNode(node);
        node = createNumber(num);

        return node;
    }

    if (left -> type == NODE_VARIABLE && right -> type == NODE_VARIABLE &&
        strcmp(left->data.var_name, right->data.var_name) == 0){
            freeNode(node);

            return createNumber(0.0);
    }

    return node;
}

ExprNode* isDiv(ExprNode* node){
    if (node == NULL)
        return NULL;
    
    if (node -> type != NODE_BINARY)
        return node;

    if (node -> data.binary.left == NULL ||
        node -> data.binary.right == NULL)
        return node;
    
    ExprNode* left = node -> data.binary.left; 
    ExprNode* right = node -> data.binary.right;
    
    if (right -> type == NODE_NUMBER && right -> data.number == 0.0)
        return NULL;

    if (left -> type == NODE_NUMBER && left -> data.number == 0.0){
        freeNode(node);
        node = createNumber(0.0);

        return node;
    }

    if (right -> type == NODE_NUMBER && right -> data.number == 1.0){
        ExprNode* result = left;
        node -> data.binary.left = NULL;

        freeNode(node);
        return result;
    }

    if (right -> type == NODE_NUMBER && right -> data.number == -1.0){
        ExprNode* result = left;
        node -> data.binary.left = NULL;

        freeNode(node);
        return createUnary('-', result);
    }

    if (left -> type == NODE_NUMBER && right -> type == NODE_NUMBER){
        double num = (left->data.number)/(right->data.number);
        freeNode(node);
        
        return createNumber(num);
    }

    if (left -> type == NODE_VARIABLE && right -> type == NODE_VARIABLE &&
        strcmp(left->data.var_name, right->data.var_name) == 0){
            freeNode(node);

            return createNumber(1.0);
    }

    return node;
}

ExprNode* isUnary(ExprNode* node){



    if (node -> data.unary.op == '-' &&
        node -> data.unary.operand -> data.number < 0){
        double num = (-1)*(node -> data.unary.operand -> data.number);

        freeNode(node);
        return createNumber(num);
    }
    if (node -> data.unary.op == '-' &&
        node -> data.unary.operand -> type == NODE_UNARY ){
        ExprNode* result = 
        
    }
}

ExprNode* rules(ExprNode* node){
    if (node -> type == NODE_NUMBER)
        return node;
    
    if (node -> type == NODE_VARIABLE)
        return node;

    if (node -> type == NODE_BINARY){
        if (node -> data.binary.op == '*')
            return isMulti(node);
        
        if (node -> data.binary.op == '+')
            return isAddit(node);
        
        if (node -> data.binary.op == '-')
            return isSubt(node);
        
        if (node -> data.binary.op == '/')
            return isDiv(node);
    }

    if (node -> type == NODE_UNARY){
        return isUnary(node);
    }
}

ExprNode* simplify(ExprNode *node){
    if (node == NULL)
        return NULL;
    
    if (node -> type == NODE_NUMBER)
        return node;
    
    if (node -> type == NODE_VARIABLE)
        return node;
    
    if (node -> type == NODE_UNARY){
        node -> data.unary.operand = simplify(node -> data.unary.operand);
        
        node = rules(node);
        return node;
    }

    if (node -> type == NODE_BINARY){
        node -> data.binary.left = simplify(node -> data.binary.left);
        node -> data.binary.right = simplify(node -> data.binary.right);

        node = rules(node);
        return node;
    }
    
    if (node -> type == NODE_FUNCTION){
        for (int i = 0; i < (node -> data.function.arg_count); i++){
            node -> data.function.args[i] = simplify(node -> data.function.args[i]);
            node = rules(node);
            }
        return node;
    }

    return node;
}