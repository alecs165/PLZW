#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct stack stack;  
struct stack{
    //need the size of the stack to check
    //if empty
    //and val storing the character
    unsigned int val;
    unsigned int size;
    stack *rest;
};


/// @brief goes through the stack and frees each element
/// @param ns the stack to free
void stack_free(stack *ns);


/// @brief creates the initial stack pointer and sets size to 0
/// @param s the empty stack
/// @return the initialized stack
stack* init_stack(stack* s);

/// @brief pushes an element to the top of the stack
/// @param v the value being pushed
/// @param r the stack
/// @return the modified stack

stack *push(unsigned int v, stack *r);

/// @brief pops an element off the stack
/// @param r pointer to pointer of stack r
/// @return the value stored on the stack
unsigned int pop(stack **r);