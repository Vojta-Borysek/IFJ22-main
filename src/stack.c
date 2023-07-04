//
// Created by dava on 20.11.22.
//

#include "../headers/stack.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void StackInit (Stack *stack)
{
    stack->top = NULL;
}

void StackPush (Stack *stack, Types type)
{
    StackI *n = malloc(sizeof(StackI));
    n->type = type;
    n->next = stack->top;
    stack->top = n;
}

void StackPop (Stack *stack)
{
    if(stack->top != NULL)
    {
        StackI *tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
    }
}

StackI* StackTop (Stack *stack)
{
    return stack->top;
}

StackI* StackTopUseable (Stack *stack)
{
    for(StackI *tmp = stack->top; tmp != NULL; tmp = tmp->next)
    {
        if(tmp->type == STOP || tmp->type == REDUCTION)
        {
            continue;
        }
        else
        {
            return tmp;
        }
    }
    return NULL;
}

void StackPushNext (Stack *stack)
{
    if(StackTop(stack)->type == REDUCTION)
    {
        if(StackTopUseable(stack) != NULL)
        {
            StackI *newElement = (StackI*) malloc(sizeof(StackI));
            newElement->type = STOP;
            newElement->next = StackTop(stack)->next;
            StackTop(stack)->next = newElement;
        }
    }
    else
    {
        StackPush(stack, STOP);
    }
}

void StackEmpty (Stack *stack)
{
    while(StackTop(stack) != NULL)
    {
        StackPop(stack);
    }
}