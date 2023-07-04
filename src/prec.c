//
// Created by dava on 20.11.22.
//

#include "../headers/prec.h"
#include "../headers/stack.h"
#include "../headers/lex.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Token c_token;
Token tmp_token;
Stack p_stack;
bool succes = false;
bool error = false;
bool read_token = true;
int isEnd = 0;

int pc_table[15][15] =
        {
            //          +  |   -  |  .  |  *   |  /  | <   |   >   |  <=  |  >= | ===  | !==   | (   |   )   |  i   |   $
                {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},// +
                {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},// -
                {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},// .
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},// *
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},// /
                {1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 0, 1, 0},// <
                {1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 0, 1, 0},// >
                {1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 0, 1, 0},// <=
                {1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 0, 1, 0},// >=
                {1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 0, 1, 0},// ===
                {1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 0, 1, 0},// !==
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 3},// (
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3, 0},// )
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3, 0},// i: int,float,string,id
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 3}// $
                //0 = REDUKCE
                //1 = SHIFT
                //2 = EQUAL
                //3 = ERROR
        };
Types token_to_prec(Token *token)
{
    switch(token->token_type)
    {
        case PLUS:
            return T_PLUS;
        case MIN:
            return T_MINUS;
        case KON:
            return T_CON;
        case DIV:
            return T_DIV;
        case MUL:
            return T_MUL;
        case LT:
            return T_LT;
        case GT:
            return T_GT;
        case LTE:
            return T_LTE;
        case GTE:
            return T_GTE;
        case EQT3:
            return T_EQ;
        case NEQT3:
            return T_NEQ;
        case LB:
            isEnd++;
            return T_LB;
        case RB:
            isEnd--;
            if(isEnd < 0)
            {
                return DOLLAR;
            }
            return T_RB;
        case INT:
            return T_INT;
        case DES:
        case DEXP:
            return T_FLOAT;
        case STRING:
            return T_STRING;
        case IDP2:
            return T_ID;
        case KS:
            if((strcmp(token->data, "null")) == 0)
            {
                return T_ID;
            }
            else
            {
                return DOLLAR;
            }
        default:
            return DOLLAR;
    }
}

int getOper(StackI *st, Types type)
{
    //do stacku pushnu $, potom vemu top stacku a dalsi token v poradi pomoci getToken
    //udělám switch ve kterém budou jednotlivé operatory a podle toho přiřadím číslo
    //danému tokenu v poradi a tokenu z vrcholu zásobníku, například když na vrcholu
    //zásobniku bude i a další token v pořadí bude +, přiřazené čísla podle switche budou
    // 13 a 0, tyto dvě čísla dám do parametrů tabulky a najdu operaci kterou chci vykonat
    //v tomto případě to bude 0 což je redukce, přesunu se do funkce pro redukci
    //takto budu pokračovat dokud neprojdu celý expression (TODO nevím jak poznam konec expressionu)
    //pokud nenastane zádná chyba vrátím na výstup 0 a pokracuji v syntaxi, při chybě vracím 2
    int i0,i1 = 0;
    Types tmp_type;
    for(int i = 0; i < 2; i++)
    {
        int tmp_i = 0;
        if(i == 0)
        {
            tmp_type = st->type;
        }
        else
        {
            tmp_type = type;
        }
        if(tmp_type == DOLLAR)
        {
            tmp_i = 14;
        }
        else if(tmp_type == T_ID || tmp_type == T_INT ||
                tmp_type == T_FLOAT || tmp_type == T_STRING)
        {
            tmp_i = 13;
        }
        else
        {
            switch(tmp_type)
            {
                case T_PLUS:
                    tmp_i = 0;
                    break;
                case T_MINUS:
                    tmp_i = 1;
                    break;
                case T_CON:
                    tmp_i = 2;
                    break;
                case T_MUL:
                    tmp_i = 3;
                    break;
                case T_DIV:
                    tmp_i = 4;
                    break;
                case T_LT:
                    tmp_i = 5;
                    break;
                case T_GT:
                    tmp_i = 6;
                    break;
                case T_LTE:
                    tmp_i = 7;
                    break;
                case T_GTE:
                    tmp_i = 8;
                    break;
                case T_EQ:
                    tmp_i = 9;
                    break;
                case T_NEQ:
                    tmp_i = 10;
                    break;
                case T_LB:
                    tmp_i = 11;
                    isEnd++;
                    break;
                case T_RB:
                    tmp_i = 12;
                    break;
            }
        }
        if(i == 0)
        {
            i0 = tmp_i;
        }
        else
        {
            i1 = tmp_i;
        }
    }
    return pc_table[i0][i1];
}


ReductionRules reduction(int cnt, StackI *s1, StackI *s2, StackI *s3)
{
    if(cnt == 1)
    {
        if(s1->type == T_INT || s1->type == T_FLOAT || s1->type == T_STRING || s1->type == T_ID)
        {
            return I;
        }
        else
        {
            return FF;
        }
    }
    else if(cnt == 3)
    {
        if(s1->type == T_LB && s2->type == REDUCTION && s3->type == T_RB)
        {
            return BREBR;
        }
        else if(s1->type == REDUCTION && s2->type == T_PLUS && s3->type == REDUCTION)
        {
            return EPLUSE;
        }
        else if(s1->type == REDUCTION && s2->type == T_MINUS && s3->type == REDUCTION)
        {
            return EMINUSE;
        }
        else if(s1->type == REDUCTION && s2->type == T_CON && s3->type == REDUCTION)
        {
            return ECONE;
        }
        else if(s1->type == REDUCTION && s2->type == T_MUL && s3->type == REDUCTION)
        {
            return EMULE;
        }
        else if(s1->type == REDUCTION && s2->type == T_DIV && s3->type == REDUCTION)
        {
            return EDIVE;
        }
        else if(s1->type == REDUCTION && s2->type == T_LT && s3->type == REDUCTION)
        {
            return ELTE;
        }
        else if(s1->type == REDUCTION && s2->type == T_GT && s3->type == REDUCTION)
        {
            return EGTE;
        }
        else if(s1->type == REDUCTION && s2->type == T_LTE && s3->type == REDUCTION)
        {
            return ELTEE;
        }
        else if(s1->type == REDUCTION && s2->type == T_GTE && s3->type == REDUCTION)
        {
            return EGTEE;
        }
        else if(s1->type == REDUCTION && s2->type == T_EQ && s3->type == REDUCTION)
        {
            return EEQE;
        }
        else if(s1->type == REDUCTION && s2->type == T_NEQ && s3->type == REDUCTION)
        {
            return ENEQE;
        }
        else
        {
            return FF;
        }
    }
}

void execute_rule(int rule, Types gt)
{
    StackI *tmp;
    switch(rule)
    {
        case 1:
            StackPushNext(&p_stack);
            StackPush(&p_stack, gt);
            read_token = true;
            break;
        case 0:
            tmp = StackTop(&p_stack);
            int cnt = 0;
            ReductionRules rule;
            while(tmp != NULL)
            {
                if(tmp->type != STOP)
                {
                    cnt++;
                }
                else
                {
                    break;
                }
                tmp = tmp->next;
            }
            if(cnt == 1)
            {
                rule = reduction(cnt, p_stack.top, NULL, NULL);
            }
            else if(cnt == 3)
            {
                rule = reduction(cnt, p_stack.top->next->next, p_stack.top->next, p_stack.top);
            }
            if(rule == FF)
            {
                //TODO syn error
            }
            else
            {
                if(cnt == 1)
                {
                    StackPop(&p_stack);
                    StackPop(&p_stack);
                }
                else if(cnt == 3)
                {
                    StackPop(&p_stack);
                    StackPop(&p_stack);
                    StackPop(&p_stack);
                    StackPop(&p_stack);
                }
                StackPush(&p_stack, REDUCTION);
                read_token = false;
            }
            break;
        case 2:
            StackPush(&p_stack, gt);
            read_token = true;
            break;
        case 3:
            if(StackTopUseable(&p_stack)->type == DOLLAR && token_to_prec(&c_token) == DOLLAR && StackTop(&p_stack)->type == REDUCTION)
            {
                succes = true;
            }
            else
            {
                error = true;
            }
            char *c = c_token.data;
            ungetc(c[0], stdin);
            break;
    }
}

int exp_reduction(Token *c_token)
{
    StackI *tmp1 = StackTopUseable(&p_stack);
    Types tmp2 = token_to_prec(c_token);
    int rule = getOper(tmp1, tmp2);
    execute_rule(rule, tmp2);
    if(succes)
    {
        return 0;
    }
    if(error)
    {
        return 2;
    }
    return 2;
}

int expression()
{
    tmp_token.data = "";
    c_token.data = "";
    succes = false;
    error = false;
    read_token = true;
    StackInit(&p_stack);
    StackPush(&p_stack, DOLLAR);
    int f;
    int i = 0;
    do {
        if(read_token)
        {
            if(getToken(&c_token) == 0)
            {
                tmp_token = c_token;
            }
            else
            {
                exit(1);
            }
        }
        else
        {
            c_token = tmp_token;
        }
        f = exp_reduction(&c_token);
        i++;
    } while(!succes && !error);
    StackEmpty(&p_stack);
    return f;
}