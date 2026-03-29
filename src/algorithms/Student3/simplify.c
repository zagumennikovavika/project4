#include "student3.h"

// Create a number node
ExprNode* createNumber(double value) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    if (!node) return NULL;

    node->type = NODE_NUMBER;
    node->data.number = value;

    return node;
}

// Create a variable node
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

// Create a unary operator node (e.g., +x, -x)
ExprNode* createUnary(char op, ExprNode* operand) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    if (!node) return NULL;

    node->type = NODE_UNARY;
    node->data.unary.op = op;
    node->data.unary.operand = operand;

    return node;
}

// Create a binary operator node (e.g., x + y, x * y)
ExprNode* createBinary(char op, ExprNode* left, ExprNode* right) {
    ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
    if (!node) return NULL;

    node->type = NODE_BINARY;
    node->data.binary.op = op;
    node->data.binary.left = left;
    node->data.binary.right = right;

    return node;
}

// Create a function node (e.g., sin(x), max(a,b))
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

    // Copy the array of arguments
    node->data.function.args = (ExprNode**)malloc(sizeof(ExprNode*) * arg_count);
    for (int i = 0; i < arg_count; i++)
        node->data.function.args[i] = args[i];

    node->data.function.arg_count = arg_count;

    return node;
}

// Free a node and all its children recursively
void freeNode(ExprNode* node) {
    if (node == NULL)
        return;

    switch (node->type) {

        case NODE_NUMBER:
            // Numbers have no extra allocations
            break;

        case NODE_VARIABLE:
            // Free variable name if allocated
            free(node->data.var_name);
            break;

        case NODE_UNARY:
            // Free the operand recursively
            freeNode(node->data.unary.operand);
            break;

        case NODE_BINARY:
            // Free both children recursively
            freeNode(node->data.binary.left);
            freeNode(node->data.binary.right);
            break;

        case NODE_FUNCTION:
            // Free all function arguments recursively
            for (int i = 0; i < node->data.function.arg_count; i++) 
                freeNode(node->data.function.args[i]);
            
            // Free array of arguments and function name
            free(node->data.function.args);     
            free(node->data.function.func_name); 
            break;
    }

    free(node);
}

// Simplify multiplication expressions
ExprNode* isMulti(ExprNode* node){
    if (node == NULL || node->type != NODE_BINARY)
        return node;

    if (!node->data.binary.left || !node->data.binary.right)
        return node;

    ExprNode* left = node->data.binary.left; 
    ExprNode* right = node->data.binary.right;

    // Multiplication by 0
    if ((left->type == NODE_NUMBER && left->data.number == 0.0) ||
        (right->type == NODE_NUMBER && right->data.number == 0.0)) {
        freeNode(node);
        return createNumber(0.0);
    }

    // Multiplication by 1
    if (left->type == NODE_NUMBER && left->data.number == 1.0) {
        ExprNode* result = right;
        node->data.binary.right = NULL; 
        freeNode(node);
        return result;              
    }
    
    if (right->type == NODE_NUMBER && right->data.number == 1.0) {
        ExprNode* result = left;   
        node->data.binary.left = NULL; 
        freeNode(node);             
        return result;              
    }

    // Multiplication by -1
    if (left->type == NODE_NUMBER && left->data.number == -1.0) {
        ExprNode* result = right;   
        node->data.binary.right = NULL; 
        freeNode(node);      
        return createUnary('-', result);              
    }

    if (right->type == NODE_NUMBER && right->data.number == -1.0) {
        ExprNode* result = left;   
        node->data.binary.left = NULL; 
        freeNode(node);             
        return createUnary('-', result);              
    }

    // Both numbers: multiply them
    if (left->type == NODE_NUMBER && right->type == NODE_NUMBER) {
        double num = left->data.number * right->data.number;
        freeNode(node);
        return createNumber(num);
    }

    return node;
}

// Combine similar terms like 2*x + 3*x
ExprNode* similar(ExprNode* node, ExprNode* left, ExprNode* right) {
    double a = 1.0, b = 1.0;
    ExprNode* var1 = NULL;
    ExprNode* var2 = NULL;

    // Extract coefficient and variable from left term
    if (left->type == NODE_BINARY && left->data.binary.op == '*') {
        ExprNode* L = left->data.binary.left;
        ExprNode* R = left->data.binary.right;

        if (L->type == NODE_NUMBER && R->type == NODE_VARIABLE) {
            a = L->data.number;
            var1 = R;
        } else if (L->type == NODE_VARIABLE && R->type == NODE_NUMBER) {
            a = R->data.number;
            var1 = L;
        } else return node;
    }
    else if (left->type == NODE_VARIABLE) {
        var1 = left;
        a = 1.0;
    }
    else return node;

    // Extract coefficient and variable from right term
    if (right->type == NODE_BINARY && right->data.binary.op == '*') {
        ExprNode* L = right->data.binary.left;
        ExprNode* R = right->data.binary.right;

        if (L->type == NODE_NUMBER && R->type == NODE_VARIABLE) {
            b = L->data.number;
            var2 = R;
        } else if (L->type == NODE_VARIABLE && R->type == NODE_NUMBER) {
            b = R->data.number;
            var2 = L;
        } else return node;
    }
    else if (right->type == NODE_VARIABLE) {
        var2 = right;
        b = 1.0;
    }
    else return node;

    // Variables must match
    if (strcmp(var1->data.var_name, var2->data.var_name) != 0)
        return node;

    double sum = a + b;

    // Detach children to avoid freeing them accidentally
    if (left->type == NODE_BINARY) {
        left->data.binary.left = NULL;
        left->data.binary.right = NULL;
    }
    if (right->type == NODE_BINARY) {
        right->data.binary.left = NULL;
        right->data.binary.right = NULL;
    }

    node->data.binary.left = NULL;
    node->data.binary.right = NULL;
    freeNode(node);

    if (sum == 0.0)
        return createNumber(0.0);
    if (sum == 1.0)
        return var1;

    return createBinary('*', createNumber(sum), var1);
}

// Simplify addition expressions
ExprNode* isAddit(ExprNode* node){
    if (node == NULL || node->type != NODE_BINARY)
        return node;

    if (!node->data.binary.left || !node->data.binary.right)
        return node;
    
    ExprNode* left = node->data.binary.left; 
    ExprNode* right = node->data.binary.right;

    // Addition by 0
    if (left->type == NODE_NUMBER && left->data.number == 0.0) {
        ExprNode* result = right;
        node->data.binary.right = NULL;
        freeNode(node);
        return result;
    }
    if (right->type == NODE_NUMBER && right->data.number == 0.0) {
        ExprNode* result = left;
        node->data.binary.left = NULL;
        freeNode(node);
        return result;
    }

    // Both numbers: add them
    if (left->type == NODE_NUMBER && right->type == NODE_NUMBER) {
        double num = left->data.number + right->data.number;
        freeNode(node);
        return createNumber(num);
    }

    // Combine similar terms
    if(node->data.binary.op == '+')
        return similar(node, left, right);

    return node;
}

// Simplify subtraction expressions
ExprNode* isSubt(ExprNode* node){
    if (node == NULL || node->type != NODE_BINARY)
        return node;

    if (!node->data.binary.left || !node->data.binary.right)
        return node;
    
    ExprNode* left = node->data.binary.left; 
    ExprNode* right = node->data.binary.right;

    // 0 - x = -x
    if (left->type == NODE_NUMBER && left->data.number == 0.0) {
        ExprNode* result = right;
        node->data.binary.right = NULL;
        freeNode(node);
        return createUnary('-', result);
    }

    // x - 0 = x
    if (right->type == NODE_NUMBER && right->data.number == 0.0) {
        ExprNode* result = left;
        node->data.binary.left = NULL;
        freeNode(node);
        return result;
    }

    // Both numbers: subtract
    if (left->type == NODE_NUMBER && right->type == NODE_NUMBER) {
        double num = left->data.number - right->data.number;
        freeNode(node);
        return createNumber(num);
    }

    // x - x = 0
    if (left->type == NODE_VARIABLE && right->type == NODE_VARIABLE &&
        strcmp(left->data.var_name, right->data.var_name) == 0) {
        freeNode(node);
        return createNumber(0.0);
    }

    return node;
}

// Simplify division expressions
ExprNode* isDiv(ExprNode* node){
    if (node == NULL || node->type != NODE_BINARY)
        return node;

    if (!node->data.binary.left || !node->data.binary.right)
        return node;
    
    ExprNode* left = node->data.binary.left; 
    ExprNode* right = node->data.binary.right;

    // Division by 0 is invalid
    if (right->type == NODE_NUMBER && right->data.number == 0.0)
        return NULL;

    // 0 / x = 0
    if (left->type == NODE_NUMBER && left->data.number == 0.0) {
        freeNode(node);
        return createNumber(0.0);
    }

    // x / 1 = x
    if (right->type == NODE_NUMBER && right->data.number == 1.0) {
        ExprNode* result = left;
        node->data.binary.left = NULL;
        freeNode(node);
        return result;
    }

    // x / -1 = -x
    if (right->type == NODE_NUMBER && right->data.number == -1.0) {
        ExprNode* result = left;
        node->data.binary.left = NULL;
        freeNode(node);
        return createUnary('-', result);
    }

    // Both numbers: divide
    if (left->type == NODE_NUMBER && right->type == NODE_NUMBER) {
        double num = left->data.number / right->data.number;
        freeNode(node);
        return createNumber(num);
    }

    // x / x = 1
    if (left->type == NODE_VARIABLE && right->type == NODE_VARIABLE &&
        strcmp(left->data.var_name, right->data.var_name) == 0) {
        freeNode(node);
        return createNumber(1.0);
    }

    return node;
}

// Simplify unary expressions
ExprNode* isUnary(ExprNode* node){
    if (node == NULL || node->type != NODE_UNARY)
        return node;

    ExprNode* operand = node->data.unary.operand;
    if (operand == NULL)
        return node;

    char op = node->data.unary.op;

    // Unary plus: remove it
    if (op == '+') {
        ExprNode* result = operand;
        node->data.unary.operand = NULL;
        freeNode(node);                  
        return result;
    }

    // Unary minus on number: negate it
    if (op == '-' && operand->type == NODE_NUMBER) {
        double num = -(operand->data.number);
        freeNode(node); 
        return createNumber(num);
    }

    // Double negation: --x = x
    if (op == '-' && operand->type == NODE_UNARY &&
        operand->data.unary.op == '-') {
        ExprNode* result = operand->data.unary.operand;
        operand->data.unary.operand = NULL;
        node->data.unary.operand = NULL;
        freeNode(node); 
        return result;
    }

    return node;
}

// Apply simplification rules based on node type
ExprNode* rules(ExprNode* node){
    if (node->type == NODE_NUMBER || node->type == NODE_VARIABLE)
        return node;

    if (node->type == NODE_BINARY){
        switch(node->data.binary.op) {
            case '*': return isMulti(node);
            case '+': return isAddit(node);
            case '-': return isSubt(node);
            case '/': return isDiv(node);
        }
    }

    if (node->type == NODE_UNARY)
        return isUnary(node);
    
    return node;
}

// Recursively simplify expression tree
ExprNode* simplify(ExprNode *node){
    if (node == NULL)
        return NULL;
    
    if (node->type == NODE_NUMBER || node->type == NODE_VARIABLE)
        return node;
    
    if (node->type == NODE_UNARY){
        node->data.unary.operand = simplify(node->data.unary.operand);
        return rules(node);
    }

    if (node->type == NODE_BINARY){
        node->data.binary.left = simplify(node->data.binary.left);
        node->data.binary.right = simplify(node->data.binary.right);
        return rules(node);
    }
    
    if (node->type == NODE_FUNCTION){
        for (int i = 0; i < node->data.function.arg_count; i++){
            node->data.function.args[i] = simplify(node->data.function.args[i]);
            // simplify function after each argument
            node = rules(node); 
        }
        return node;
    }

    return node;
}