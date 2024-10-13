#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct
{
    int top;
    char items[MAX];
} Stack;

void init(Stack *s)
{
    s->top = -1;
}

int isEmpty(Stack *s)
{
    return s->top == -1;
}

int isFull(Stack *s)
{
    return s->top == MAX - 1;
}

void push(Stack *s, char item)
{
    if (isFull(s))
    {
        printf("Overflow!!!\n");
        return;
    }
    s->top += 1;
    s->items[s->top] = item;
}

char pop(Stack *s)
{
    if (isEmpty(s))
    {
        printf("Underflow!!!\n");
        return '\0'; // NUL
    }
    char item = s->items[s->top];
    s->top -= 1;
    // printf("%c is popped\n", item);
    return item;
}

char peek(Stack *s)
{
    if (isEmpty(s))
    {
        return '\0';
    }
    return s->items[s->top];
}

int precedence(char operator)
{
    switch (operator)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default:
        return 0;
    }
}

void infixToPostfix(char *infix, char *postfix)
{
    Stack s;
    init(&s);
    int j = 0; // Index for postfix string

    for (int i = 0; infix[i] != '\0'; i++)
    {
        char current = infix[i];

        if (isalnum(current))
        {
            postfix[j++] = current; // Add operand to output
        }
        else if (current == '(')
        {
            push(&s, current); // Push '(' onto stack
        }
        else if (current == ')')
        {
            while (!isEmpty(&s) && peek(&s) != '(')
            {
                postfix[j++] = pop(&s); // Pop until '('
            }
            pop(&s); // Discard '('
        }
        else
        { // Operator
            while (!isEmpty(&s) && precedence(peek(&s)) >= precedence(current))
            {
                postfix[j++] = pop(&s); // Pop higher or equal precedence
            }
            push(&s, current); // Push current operator
        }
    }

    while (!isEmpty(&s))
    {
        postfix[j++] = pop(&s); // Pop remaining operators
    }
    postfix[j] = '\0'; // Null-terminate the string
}

void reverse(char *exp)
{
    int n = strlen(exp);
    for (int i = 0; i < n / 2; i++)
    {
        char temp = exp[i];
        exp[i] = exp[n - 1 - i];
        exp[n - 1 - i] = temp;
    }
}

void infixToPrefix(char *infix, char *prefix)
{
    reverse(infix); // Reverse the infix expression

    // Replace '(' with ')' and vice versa
    for (int i = 0; infix[i] != '\0'; i++)
    {
        if (infix[i] == '(')
        {
            infix[i] = ')';
        }
        else if (infix[i] == ')')
        {
            infix[i] = '(';
        }
    }

    char postfix[MAX];
    infixToPostfix(infix, postfix); // Convert to postfix
    reverse(postfix);               // Reverse postfix to get prefix
    strcpy(prefix, postfix);        // Copy result to prefix
}

int evaluatePostfix(char *postfix)
{
    Stack s;
    init(&s);

    for (int i = 0; postfix[i] != '\0'; i++)
    {
        char current = postfix[i];

        if (isalnum(current))
        {
            push(&s, current - '0'); // Convert char to int (single digits)
        }
        else
        {
            int operand2 = pop(&s);
            int operand1 = pop(&s);
            switch (current)
            {
            case '+':
                push(&s, operand1 + operand2);
                break;
            case '-':
                push(&s, operand1 - operand2);
                break;
            case '*':
                push(&s, operand1 * operand2);
                break;
            case '/':
                push(&s, operand1 / operand2);
                break;
            }
        }
    }
    return pop(&s); // Return the final result
}

void menu()
{
    printf("1 - Infix to Postfix\n");
    printf("2 - Infix to Prefix\n");
    printf("3 - Evaluate Postfix Expression\n");
    printf("4 - Exit\n");
}

int main()
{
    char infix[MAX], postfix[MAX], prefix[MAX];
    int choice;
    do
    {
        menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter an infix expression: ");
            scanf("%s", infix);
            infixToPostfix(infix, postfix);
            printf("\n\nPostfix: %s\n\n", postfix);
            break;
        case 2:
            printf("Enter an infix expression: ");
            scanf("%s", infix);
            infixToPrefix(infix, prefix);
            printf("\n\nPrefix: %s\n\n", prefix);
            break;
        case 3:
            printf("Enter a postfix expression to evaluate: ");
            scanf("%s", postfix);
            int result = evaluatePostfix(postfix);
            printf("\n\nPostfix Evaluation Result: %d\n\n", result);
            break;
        case 4:
            printf("\n\nExiting...\n\n");
            break;
        default:
            printf("Enter valid choice!\n\n");
            break;
        }

    } while (choice != 4);

    return 0;
}