//
// Created by dava on 20.11.22.
//

#ifndef IFJ22_STACK_H
#define IFJ22_STACK_H

#include "prec.h"

typedef struct T_Stack{
    /** typ symbolu v zásobníku */
    Types type;
    /** ukazatel na dalsi prvek v zasobniku */
    struct T_Stack *next;
} StackI;

typedef struct{
    /** ukazatel na vrchol zasobniku */
    StackI *top;
}Stack;

void StackInit (Stack *);

void StackPush (Stack *, Types type);

void StackPop (Stack *);

StackI* StackTop (Stack *);

StackI* StackTopUseable (Stack *);

void StackPushNext (Stack *);

void StackEmpty (Stack *);

#endif //IFJ22_STACK_H
