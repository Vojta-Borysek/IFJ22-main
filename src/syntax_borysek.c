// koment

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "../headers/lex.h"
#include "../headers/dArray.h"
#include "../headers/syntax.h"
#include "../headers/error.h"

Token current_token;
int return_token;
bool token_used;
int my_error;
bool token_is_expr();

int main()
{
    Token kokot;
    prolog();
    do
    {
        if(getToken(&kokot) != 0)
        {
            exit(1);
        }
        if(my_error != 0)
        {
            exit(2);
        }
        printf("%u : %s\n", kokot.token_type, kokot.data);
    }while(kokot.token_type != E_EOF);
}

void err_print(int code_err, char *msg_err)
{
    if (my_error == 0)
    {
        my_error = code_err;

    }
    fprintf(stderr, "Nastala chyba číslo %d, na řádku %u : '%s' - %s\n", code_err, current_token.line, current_token.data, msg_err);
}

bool next_token()
{
    if (token_used)
    {
        free(current_token.data);
        return_token = getToken(&current_token);
        token_used = false;
    }
    if (return_token == 0)
    {
        return true;
    }
    else
    {
        token_used = true;
        err_print(getToken(&current_token), "");
        return false;
    }
}

bool prolog()
{
    return_token = getToken(&current_token);
    if (return_token == 0)
    {
        if ((strcmp(current_token.data, "<")) == 0)
        {
            token_used = true;
            if(next_token())
            {
                if((strcmp(current_token.data, "?php")) == 0)
                {
                    token_used = true;
                    if(next_token())
                    {
                        if((strcmp(current_token.data, "declare")) == 0)
                        {
                            token_used = true;
                            if(next_token())
                            {
                                if((strcmp(current_token.data, "(")) == 0)
                                {
                                    token_used = true;
                                    if(next_token())
                                    {
                                        if((strcmp(current_token.data, "strict_types")) == 0)
                                        {
                                            token_used = true;
                                            if(next_token())
                                            {
                                                if((strcmp(current_token.data, "=")) == 0)
                                                {
                                                    token_used = true;
                                                    if(next_token())
                                                    {
                                                        if((strcmp(current_token.data, "1")) == 0)
                                                        {
                                                            token_used = true;
                                                            if(next_token())
                                                            {
                                                                if((strcmp(current_token.data, ")")) == 0)
                                                                {
                                                                    token_used = true;
                                                                    if(next_token())
                                                                    {
                                                                        if((strcmp(current_token.data, ";")) == 0)
                                                                        {
                                                                            token_used = true;
                                                                            return prog();
                                                                        }
                                                                        else
                                                                        {
                                                                            err_print(SYN_ERROR, "Program musí začínát '<?php declare(strict_types=1);'!");
                                                                        }
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    err_print(SYN_ERROR, "Program musí začínát '<?php declare(strict_types=1);'!");
                                                                }
                                                            }
                                                        }
                                                        else
                                                        {
                                                            err_print(SYN_ERROR, "Program musí začínát '<?php declare(strict_types=1);'!");
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    err_print(SYN_ERROR, "Program musí začínát '<?php declare(strict_types=1);'!");
                                                }
                                            }
                                        }
                                        else
                                        {
                                            err_print(SYN_ERROR, "Program musí začínát '<?php declare(strict_types=1);'!");
                                        }
                                    }
                                }
                                else
                                {
                                    err_print(SYN_ERROR, "Program musí začínát '<?php declare(strict_types=1);'!");
                                }
                            }
                        }
                        else
                        {
                            err_print(SYN_ERROR, "Program musí začínát '<?php declare(strict_types=1);'!");
                        }
                    }
                }
                else
                {
                    err_print(SYN_ERROR, "Program musí začínát '<?php declare(strict_types=1);'!");
                }
            }
        }
        else
        {
            err_print(SYN_ERROR, "Program musí začínát '<?php declare(strict_types=1);'!");
        }
    }
    else
    {
        err_print(getToken(&current_token), "");
    }
}

bool prog()
{
    if (next_token())
    {
        token_used = true;
        if (current_token.token_type == KS)
        {
            if ((strcmp(current_token.data, "function")) == 0)
            {
                if (next_token())
                {
                    token_used = true;
                    if (current_token.token_type == ID)
                    {
                        if (next_token())
                        {
                            token_used = true;
                            if (current_token.token_type == LB)
                            {
                                bool params_syntax_valid = param_list();
                                if (next_token())
                                {
                                    token_used = true;
                                    if (current_token.token_type == RB)
                                    {
                                        if (params_syntax_valid)
                                        {
                                            if (next_token())
                                            {
                                                token_used = true;
                                                if (current_token.token_type == COL)
                                                {
                                                    bool type_syntax_valid = type();
                                                    if (type_syntax_valid)
                                                    {
                                                        if (next_token())
                                                        {
                                                            if (current_token.token_type == LSB)
                                                            {
                                                                bool statement_syntax_valid = statement();
                                                                if (next_token())
                                                                {
                                                                    if ((current_token.token_type == RSB) && statement_syntax_valid)
                                                                    {
                                                                        return prog();
                                                                    }
                                                                }
                                                            }
                                                            else
                                                            {
                                                                err_print(SYN_ERROR, "");
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        err_print(SYN_ERROR, "");
                                                    }
                                                }
                                                else
                                                {
                                                    err_print(SYN_ERROR, "");
                                                }
                                            }
                                        }
                                        else
                                        {
                                            err_print(SYN_ERROR, "");
                                        }
                                    }
                                    else
                                    {
                                        err_print(SYN_ERROR, "");
                                    }
                                }
                            }
                            else
                            {
                                err_print(SYN_ERROR, "");
                            }
                        }
                    }
                    else
                    {
                        err_print(SYN_ERROR, "");
                    }
                }
            }
            else
            {
                err_print(SYN_ERROR, "");
            }
        }
        else if (current_token.token_type == ID)
        {
            char *name_of_function = malloc(sizeof(char) * (strlen(current_token.data) + 1));
            strcpy(name_of_function, current_token.data);
            if (next_token())
            {
                token_used = true;
                if (current_token.token_type == LB)
                {
                    bool params_syntax_valid = param_list();
                    if (next_token())
                    {
                        token_used = true;
                        if (current_token.token_type == RB)
                        {
                            if (params_syntax_valid)
                            {
                                if (next_token())
                                {
                                    token_used = true;
                                    if (current_token.token_type == SEM)
                                    {
                                        return prog();
                                    }
                                    else
                                    {
                                        err_print(SYN_ERROR, "");
                                    }
                                }
                            }
                            else
                            {
                                err_print(SYN_ERROR, "");
                            }
                        }
                        else
                        {
                            err_print(SYN_ERROR, "");
                        }
                    }
                }
                else
                {
                    err_print(SYN_ERROR, "");
                }
            }
        }
        else if (current_token.token_type == E_EOF)
        {
            return true;
        }
        else
        {
            err_print(SYN_ERROR, "");
        }
    }
}

bool param_list()
{
    if (next_token())
    {
        if (current_token.token_type == COM)
        {
            bool type_syntax_valid = type();
            if (type_syntax_valid)
            {
                if (next_token())
                {
                    if (current_token.token_type == ID)
                    {
                        token_used = true;
                        return param_list1();
                    }
                    else
                    {
                        err_print(SYN_ERROR, "");
                    }
                }
            }
            else
            {
                err_print(SYN_ERROR, "");
            }
        }
        else
        {
            err_print(SYN_ERROR, "");
        }
    }
}

bool param_list1()
{
    if (next_token())
    {
        if (current_token.token_type == COM)
        {
            bool type_syntax_valid = type();
            if (next_token())
            {
                if (current_token.token_type == ID)
                {
                    token_used = true;
                    return param_list1();
                }
            }
        }
        else
        {
            return true;
        }
    }
}

bool type()
{
    if (next_token())
    {
        if (current_token.token_type == KS)
        {
            if (strcmp(current_token.data, "null") || strcmp(current_token.data, "int") ||
                strcmp(current_token.data, "string" || strcmp(current_token.data, "float")))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            err_print(SYN_ERROR, "");
        }
    }
}

bool statement() {
    if (next_token()) {
        if (current_token.token_type == IDP2) {     // pravidlo 12,15 a 16
            token_used = true;
            if(next_token()) {
                if(current_token.token_type == EQT1) {  // pravidlo 12 a 15
                    token_used = true;
                    if(next_token()) {
                       bool is_expression = token_is_expr();
                       if(is_expression){           // pravidlo 12
                           token_used = true;
                           //TODO EXPRESSION
                           if(next_token()) {
                               if(current_token.token_type == SEM)
                               {
                                   return statement();
                               }
                               else
                               {
                                   err_print(SYN_ERROR, "");
                               }
                           }
                       }
                       else if(current_token.token_type == IDP2)    // pravidlo 15
                       {
                           token_used = true;
                           if(next_token()) {
                               if(current_token.token_type == LB) {
                                    token_used = true;
                                    if(next_token()) {
                                        bool is_arg = args();
                                        if(is_arg) {
                                            token_used = true;
                                            if (next_token()) {
                                                if(current_token.token_type == RB) {
                                                    token_used = true;
                                                    if(next_token()) {
                                                        if(current_token.token_type == SEM) {
                                                            token_used = true;
                                                            return statement();
                                                        }
                                                        else
                                                        {
                                                            err_print(SYN_ERROR, "");
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    err_print(SYN_ERROR, "");
                                                }
                                            }
                                        }
                                    }
                               }
                               else
                               {
                                   err_print(SYN_ERROR, "");
                               }
                           }
                       }
                       else
                       {
                           err_print(SYN_ERROR, "");
                       }
                    }
                }
                else if (current_token.token_type == LB) {      // pravidlo 16
                    token_used = true;
                    if(next_token()) {
                        bool is_arg = args();
                        if (is_arg) {
                            token_used = true;
                            if (next_token()) {
                                if (current_token.token_type == RB) {
                                    token_used = true;
                                    if (next_token()) {
                                        if (current_token.token_type == SEM) {
                                            token_used = true;
                                            return statement();
                                        } else {
                                            err_print(SYN_ERROR, "");
                                        }
                                    }
                                }
                                else {
                                    err_print(SYN_ERROR, "");
                                }
                            }
                        }
                        else {
                            err_print(SYN_ERROR, "");
                        }
                    }
                }
                else {
                    err_print(SYN_ERROR, "");
                }
            }
        }
        else if ((strcmp(current_token.data, "if")) == 0){      // pravidlo 13
            token_used = true;
            if(next_token()){
                if (current_token.token_type == LB){
                    token_used = true;
                    if(next_token())
                    {
                        bool is_expression = token_is_expr();
                        if (is_expression){
                            if(next_token()){
                                if (current_token.token_type == RB){
                                    token_used = true;
                                    if(next_token()){
                                        if (current_token.token_type == LSB)
                                        {
                                            token_used = true;
                                            bool statement_syntax_valid = statement();
                                            if (next_token())
                                            {
                                                if ((current_token.token_type == RSB) && statement_syntax_valid)
                                                {
                                                    token_used = true;
                                                    if(next_token()){
                                                        if ((strcmp(current_token.data, "else")) == 0){
                                                            token_used = true;
                                                            if (next_token())
                                                            {
                                                                if (current_token.token_type == LSB)
                                                                {
                                                                    token_used = true;
                                                                    bool statement_syntax_valid = statement();
                                                                    if (next_token())
                                                                    {
                                                                        if ((current_token.token_type == RSB) && statement_syntax_valid)
                                                                        {
                                                                            token_used = true;
                                                                            return statement();
                                                                        }
                                                                        else
                                                                        {
                                                                            err_print(SYN_ERROR, "");
                                                                        }
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    err_print(SYN_ERROR, "");
                                                                }
                                                            }
                                                        }
                                                        else
                                                        {
                                                            err_print(SYN_ERROR, "");
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    err_print(SYN_ERROR, "");
                                                }
                                            }
                                        }
                                        else
                                        {
                                            err_print(SYN_ERROR, "");
                                        }
                                    }
                                }
                                else {
                                    err_print(SYN_ERROR, "");
                                }
                            }
                        }
                        else {
                            err_print(SYN_ERROR, "");
                        }
                    }
                }
                else {
                    err_print(SYN_ERROR, "");
                }
            }
        }
        else if ((strcmp(current_token.data, "while")) == 0){       //pravidlo 14
            token_used = true;
            if(next_token()){
                if (current_token.token_type == LB){
                    token_used = true;
                    if(next_token()){
                        bool is_expression = token_is_expr();
                        if (is_expression){
                            token_used = true;
                            if(next_token()){
                                if (current_token.token_type == RB){
                                    token_used = true;
                                    if(next_token()){
                                        if (current_token.token_type == LSB)
                                        {
                                            token_used = true;
                                            bool statement_syntax_valid = statement();
                                            if (next_token())
                                            {
                                                if ((current_token.token_type == RSB) && statement_syntax_valid)
                                                {
                                                    token_used = true;
                                                    return statement();
                                                }
                                                else
                                                {
                                                    err_print(SYN_ERROR, "");
                                                }
                                            }
                                        }
                                        else
                                        {
                                            err_print(SYN_ERROR, "");
                                        }
                                    }
                                }
                                else {
                                    err_print(SYN_ERROR, "");
                                }
                            }
                        }
                        else {
                            err_print(SYN_ERROR, "");
                        }
                    }
                }
                else {
                    err_print(SYN_ERROR, "");
                }
            }
        }
        else if ((strcmp(current_token.data, "return")) == 0){      //pravidlo 17
            token_used = true;
            if(next_token()){
                bool is_expression = token_is_expr();
                if(is_expression){
                    token_used = true;
                    if(next_token()){
                        if(current_token.token_type == SEM){
                            token_used = true;
                            return statement();
                        }
                    }
                }
            }
        }
        else {
            err_print(SYN_ERROR, "");
        }
    }
    else {
        err_print(SYN_ERROR, "");
    }
}

bool args()
{
    if (next_token())
    {
        if (current_token.token_type == COM)
        {
            token_used = true;
            if (value())
            {
                return args();
            }
            else
            {
                err_print(SYN_ERROR, "");
            }
        }
        else if (value())
        {
            return args();
        }
        else
        {
            return true;
        }
    }
}

bool value()
{
    if (next_token())
    {
        if (current_token.token_type == INT || current_token.token_type == STRING ||
            current_token.token_type == DES || current_token.token_type == DEXP ||
            current_token.token_type == IDP2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}


// expression udelat funkci s velkym switchem co vsechno muze byt expresion
bool token_is_expr()
{
    enum FSM_state tkn_type = current_token.token_type;
    switch (tkn_type) {
        case ID:
        case RSB:
        case LSB:
        case RB:
        case LB:
        case COL:
        case COM:
        case SEM:
        case MUL:
        case MIN:
        case PLUS:
        case KON:
        case LT:
        case GT:
        case EQT1:
        case NEQT1:
        case STR:
        case INT:
        case DIV:
        case IDT:
        case IDP1:
            return true;
        default:
            return false;
    }
}
