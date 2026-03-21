#include "Student1.h"


int ApplyOperator(char op, double *valStack, int *valTop, long *numOperations) {


    if (op == '~') {

        valStack[*valTop] = -1 * valStack[*valTop];
        (*numOperations)++;
        return 0;
    }

    double b = valStack[(*valTop)--];
    double a = valStack[(*valTop)--];
    double result;

    switch (op) {
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a * b;
            break;
        case '/':
            if (b == 0) {
                puts( "Division by zero");
                return -1;
            }
            result = a / b;
            break;
        case '^':
            result = pow(a, b);
            break;
    }

    valStack[++(*valTop)] = result;
    (*numOperations)++;
    return 0;
}

int EvaluateInfix(const char *expression, double *result, long *numOperations) {

    // Stacks for values and operators
    double valStack[STACK_LEN];
    char opStack[STACK_LEN];
    int valTop = -1;
    int opTop = -1;
    *numOperations = 0;
    char prev = '\0';
    int expect = 0;

    //go through each symbol of the expression, skip spaces
    for (int i=0; expression[i] != '\0'; i++ ) {
        int c = expression[i];

        if (isspace(c))
            continue;

        //number processing
        if (IsNumberChar(c)) {

            //if no operand was expected
            if (IncorrectExp(expect, 0))
                return -1;

            //determine the beginning, end and length of the potential number
            int numStart = i;

            while (expression[i] != '\0' && IsNumberChar(expression[i]))
                i++;

            int numEnd = i - 1;
            int numLength = numEnd - numStart + 1;


            //expression is not a valid number
            if (IncorrectExp(ValidateNumber(expression + numStart, numLength),0))
                return -1;

            double number = StringToNumber((char*)expression + numStart, numLength);

            if (StackOverflow(valTop, STACK_LEN - 1))
                return -1;

            valStack[++valTop] = number;

            i = numEnd;
            expect = 1;
        }

        //if minus is unary
        //stands at the beginning of a line, stands after
        //an opening bracket, or stands after another operator
        else if (c == '-' && (prev == '\0' || IsOperator(prev) ||
            prev == '(')) {

            //if no operand was expected
            if (IncorrectExp(expect, 0))
                return -1;

            if (StackOverflow(opTop,STACK_LEN - 1))
                return -1;

            //add "~" to the stack so that it is
            //different from the binary minus
            opStack[++opTop] = '~';
            expect = 0;

        }

        // Unary plus - just ignore
        else if (c == '+' && (prev == '\0' || IsOperator(prev) || prev == '(')) {

            if (IncorrectExp(expect, 0))
                return -1;

            expect = 0;
        }

        //if the symbol is a binary operator
        else if (IsOperator(c)) {

            // if no operator was expected
            if (IncorrectExp(expect, 1))
                return -1;

            int curPrecedence = GetPrecedence(c);
            char curAssoc = GetAssociativity(c);

            //while there are operators on the stack,
            //we compare the priority of both the current
            //one and the one on the top of the stack.
            //If the priority of the current one is less than
            //that of the top one, then we push the operator from
            //the top and carry them out, or if the priority is equal
            //and the current operator is left-associative
            while (opTop >= 0 && IsOperator(opStack[opTop])) {

                int topPrec = GetPrecedence(opStack[opTop]);

                if (topPrec > curPrecedence ||
                    (topPrec == curPrecedence && curAssoc == 'L')) {

                    char op = opStack[opTop--];
                    if (ApplyOperator(op, valStack, &valTop,numOperations) == -1)
                        return -1;


                    }
                else
                    break;
            }

            // Push the current operator onto the stack
            if (StackOverflow(opTop,STACK_LEN - 1))
                return -1;

            opStack[++opTop] = c;
            expect = 0;
        }


        else if (c == '(') {

            // if no operand was expected
            if (IncorrectExp(expect, 0))
                return -1;

            //put the bracket on the stack
            if (StackOverflow(opTop,STACK_LEN - 1))
                return -1;

            opStack[++opTop] = c;
            expect = 0;
        }


        else if (c == ')') {

            // if an operand was expected
            if (IncorrectExp(expect, 1))
                return -1;


            //pushes a string of all statements up
            //to the opening parenthesis
            while (opTop >= 0 && opStack[opTop] != '(') {
                char op = opStack[opTop--];
                if (ApplyOperator(op, valStack, &valTop,numOperations) == -1)
                    return -1;
            }
            // there must be an opening parenthesis
            if (opTop < 0){
                puts("Incorrect expression");
                return -1;
            }
            // remove the opening parenthesis
            // from the stack.
            opTop--;
            expect = 1;

            }
        //if unknown character
        else {
            printf( "Error: unknown symbol '%c'", c);
            return -1;
        }

        //Remember the current character as the
        //previous one
        prev = c;
    }

    //an operator should be expected at the end
    if (IncorrectExp(expect,1))
        return -1;

    // Perform all remaining operations
    while (opTop >= 0) {
        if (opStack[opTop] == '(') {
            puts("Incorrect expression");
            return -1;
        }
        char op = opStack[opTop--];
        if (ApplyOperator(op, valStack, &valTop, numOperations) == -1)
            return -1;
    }
    // There must be exactly one result on the stack
    if (valTop != 0) {
        puts( "Incorrection expression");
        return -1;
    }

    *result = valStack[0];

    return 0;

}




