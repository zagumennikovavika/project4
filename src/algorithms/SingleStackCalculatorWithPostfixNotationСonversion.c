#include "Student1.h"

int IsOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '~');
}

int GetPrecedence(char c) {

    switch (c) {
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        case '~':
        case '^':
            return 3;
    }
}


char GetAssociativity(char c) {
    if (c == '^' || c == '~')
        return 'R';
    return 'L';
}


int IsNumberChar(char c) {
    return isdigit(c) || c == '.';
}

//checks the validity of a number
int ValidateNumber(const char *number, int len) {
    int dotCount = 0;
    int digitBeforeDot = 0;
    int digitAfterDot = 0;
    int dotIndex = -1;

    for (int i = 0; i < len; i++) {

        // invalid character
        if (!isdigit(number[i]) && number[i] != '.') {
            puts("Incorrect expression");
            return -1;
        }

        // remember the point index
        if (number[i] == '.') {
            dotCount++;
            dotIndex = i;

            //if there are several points
            if (dotCount > 1) {
                puts("Incorrect expression");
                return -1;
            }
        }


        else if (isdigit(number[i]))

            if (dotIndex == -1)
                digitBeforeDot++;
            else
                digitAfterDot++;
    }

    //checs if there are digits before and after the dot.
    if (dotCount > 0 && (digitBeforeDot == 0 || digitAfterDot == 0)) {
        puts("Incorrect expression");
        return -1;
    }

    //0 at the beginning of an integer
    if (len > 1 && number[0] == '0' && number[1] != '.') {
        puts("Incorrect expression");
        return -1;
    }

    return 0;
}

//reports an error
int IncorrectExp(int param1, int param2) {
    if (param1 != param2) {
        puts("Incorrect expression");
        return 1;
    }
    return 0;
}

//reports an error
int BufferOverflow(int param1, int param2) {
    if (param1 >= param2) {
        puts("Error: Output buffer overflow");
        return 1;
    }
    return 0;
}

//reports an error
int StackOverflow(int param1, int param2) {
    if (param1 >= param2) {
        puts("Error: Stack overflow");
        return 1;
    }
    return 0;
}


int InfixToPostfix(const char *infix, char *postfix, size_t postfixLen) {

    //temporary buffer
    char rpn[postfixLen];

    //current length of the output string
    int rpnLen = 0;

    char stack[STACK_LEN];

    //the previous character in the line for the current one
    char prev = '\0';

    //index of the top element of the stack
    int top = -1;

    //expected symbol:
    //0 - operand (number, unary operator, opening parenthesis "("),
    //1 - binary operator, closing parenthesis ")"
    int expect = 0;

    //go through each symbol of the expression, skip spaces
    for (int i=0; infix[i] != '\0'; i++ ) {
        int c = infix[i];
        if (isspace(c))
            continue;

        //if the symbol is a potential beginning of a number
        if (IsNumberChar(c)) {

            //if no operand was expected
            if (IncorrectExp(expect, 0))
                return -1;

            //determine the beginning, end and length of the potential number
            int numStart = i;

            while (infix[i] != '\0' && IsNumberChar(infix[i]))
                i++;

            int numEnd = i - 1;
            int numLength = numEnd - numStart + 1;

            //expression is not a valid number
            if (IncorrectExp(ValidateNumber(infix + numStart, numLength),0))
                return -1;

            //add a number to the output string
            for (int j = numStart; j <= numEnd; j++) {

                if (BufferOverflow(rpnLen,postfixLen - 1))
                    return -1;

                rpn[rpnLen++] = infix[j];
            }

            if (BufferOverflow(rpnLen,postfixLen - 1))
                return -1;

            //add a space after the number
            rpn[rpnLen++] = ' ';

            //Correcting the cycle index
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

            if (StackOverflow(top,STACK_LEN - 1))
                return -1;

            //add "~" to the stack so that it is
            //different from the binary minus
            stack[++top] = '~';
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
            //the top into the output string, or if the priority is equal
            //and the current operator is left-associative
            while (top >= 0 && IsOperator(stack[top])) {

                int topPrec = GetPrecedence(stack[top]);

                if (topPrec > curPrecedence ||
                    (topPrec == curPrecedence && curAssoc == 'L')) {

                    if (BufferOverflow(rpnLen,postfixLen - 2))
                        return -1;

                    rpn[rpnLen++] = stack[top--];
                    rpn[rpnLen++] = ' ';
                    }
                else
                    break;

            }

            // Push the current operator onto the stack
            if (StackOverflow(top,STACK_LEN - 1))
                return -1;

            stack[++top] = c;
            expect = 0;


        }
        else if (c == '(') {

            // if no operand was expected
            if (IncorrectExp(expect, 0))
                return -1;

            //put the bracket on the stack
            if (StackOverflow(top,STACK_LEN - 1))
                return -1;

            stack[++top] = c;
            expect = 0;
        }


        else if (c == ')') {

            // if an operand was expected
            if (IncorrectExp(expect, 1))
                return -1;


            //pushes a string of all statements up
            //to the opening parenthesis
            while (top >= 0 && stack[top] != '(') {

                if (BufferOverflow(rpnLen,postfixLen - 2))
                    return -1;

                rpn[rpnLen++] = stack[top--];
                rpn[rpnLen++] = ' ';
            }

            // there must be an opening parenthesis
            if (top < 0){
                puts("Incorrect expression");
                return -1;
            }

            // We remove (ignore) the opening parenthesis
            // from the stack.
            top--;
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

    //we push the remaining operators on the stack to the output string
    while (top >= 0) {

        // If an opening parenthesis remains, there is an error.
        if (stack[top] == '(') {
            puts("Incorrect expression");
            return -1;
        }

        if (BufferOverflow(rpnLen, postfixLen - 2))
            return -1;

        rpn[rpnLen++] = stack[top--];
        rpn[rpnLen++] = ' ';
    }
    strcpy(postfix, rpn);
    postfix[rpnLen - 1] = '\0';

    return 0;

    }


//Converts a character to a Double digit
double CharToDouble(char c) {
    return c - '0';
}

//Converts a string to a Double number
double StringToNumber(char *number, int len) {

    double res = 0.0;
    int dotIndex = -1;

    //find the index of the point, if it exists
    for (int i = 0; i < len; i++) {
        if (number[i] == '.') {
            dotIndex = i;
            break;
        }
    }


    //prepare a multiplier depending on whether the
    //number is a fraction or an integer.
    double multiplier = 1.0;

    if (dotIndex != -1) {

        for (int i = 0; i < len - dotIndex - 1; i++)
            multiplier *= 10.0;

        multiplier = 1.0 / multiplier;
    }

    //find the result bit by bit
    for (int i = len - 1; i >= 0; i--){

        if (number[i] != '.') {
            res += CharToDouble(number[i]) * multiplier;
            multiplier *= 10.0;
        }
    }
    return res;
}


int EvaluatePostfix(const char *postfix, double *result, long *numOperations ) {
    //stack for intermediate calculations
    double stack[STACK_LEN];

    //index of the top element in stack
    int top = -1;


    //go through each character in the postfix notation
    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];

        // If a character is the beginning of a
        // number, we find its beginning,length
        // and convert the string to a double number.
        if (isdigit(c)) {
            const char *start = postfix + i;

            int len = 0;

            while (postfix[i] != ' ' && postfix[i] != '\0') {
                len++;
                i++;
            }
            double number = StringToNumber(start, len);

            //we put a number on the stack
            if (StackOverflow(top,STACK_LEN - 1))
                return -1;

            stack[++top] = number;

        }

        //If we encounter a unary minus, we apply
        //it to the top element in the stack.
        else if (c == '~') {
            stack[top] = -1 * stack[top];
            (*numOperations)++;
        }


        //If the symbol is a binary operator, then we
        //apply it to the top two elements in the stack
        //and put the result on the top of the stack.
        else {

            if (c == '*') {
                stack[top - 1] = stack[top - 1] * stack[top];
                (*numOperations)++;
                top--;
            }

            else if (c == '+') {
                stack[top - 1] = stack[top - 1] + stack[top];
                (*numOperations)++;
                top--;
            }

            else if (c == '-') {
                stack[top - 1] = stack[top - 1] - stack[top];
                (*numOperations)++;
                top--;

            }

            else if (c == '^') {
                stack[top - 1] = pow(stack[top - 1],stack[top]);
                (*numOperations)++;
                top--;

            }

            else if (c == '/') {

                if (stack[top] == 0) {
                    puts("Division by zero");
                    return -1;
                }

                stack[top - 1] = stack[top - 1] / stack[top];
                (*numOperations)++;
                top--;
            }

        }
    }
    *result = stack[top];;
    return 0;
}
