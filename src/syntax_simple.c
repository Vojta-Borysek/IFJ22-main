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
#include "../headers/prec.h"

Token current_token;
int return_token;
bool token_used;
int my_error;
bool token_is_expr();

int main()
{
    prolog();
    exit(my_error);
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
        exit(1);
    }
}

// 1. pravidlo
bool prolog()
{
    return_token = getToken(&current_token);
    if (return_token == 0)
    {
        if ((strcmp(current_token.data, "<")) == 0)
        {
            token_used = true;
            if (next_token())
            {
                if ((strcmp(current_token.data, "?php")) == 0)
                {
                    token_used = true;
                    if (next_token())
                    {
                        if ((strcmp(current_token.data, "declare")) == 0)
                        {
                            token_used = true;
                            if (next_token())
                            {
                                if ((strcmp(current_token.data, "(")) == 0)
                                {
                                    token_used = true;
                                    if (next_token())
                                    {
                                        if ((strcmp(current_token.data, "strict_types")) == 0)
                                        {
                                            token_used = true;
                                            if (next_token())
                                            {
                                                if ((strcmp(current_token.data, "=")) == 0)
                                                {
                                                    token_used = true;
                                                    if (next_token())
                                                    {
                                                        if ((strcmp(current_token.data, "1")) == 0)
                                                        {
                                                            token_used = true;
                                                            if (next_token())
                                                            {
                                                                if ((strcmp(current_token.data, ")")) == 0)
                                                                {
                                                                    token_used = true;
                                                                    if (next_token())
                                                                    {
                                                                        if ((strcmp(current_token.data, ";")) == 0)
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

// 6. pravidlo
bool function_def()
{
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
                            if(param_list()) {
                                if (next_token()) {
                                    token_used = true;
                                    if (current_token.token_type == RB) {
                                            if (next_token()) {
                                                token_used = true;
                                                if (current_token.token_type == COL) {
                                                        if (next_token())
                                                        {
                                                            token_used = true;
                                                            if(type()) {
                                                                if(next_token()) {
                                                                    token_used = true;
                                                                    if (current_token.token_type == LSB) {
                                                                        if (next_token()) {
                                                                            token_used = true;
                                                                            if (statement()) {
                                                                                if (next_token()) {
                                                                                    token_used = true;
                                                                                    if (current_token.token_type == RSB) {
                                                                                        return prog();
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    } else {
                                                                        err_print(SYN_ERROR,
                                                                                  "chybi { v deklaraci funkce");
                                                                        return false;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                } else {
                                                    err_print(SYN_ERROR, "chybi : v deklaraci funkce");
                                                    return false;
                                                }
                                            }
                                    } else {
                                        err_print(SYN_ERROR, "chybi ) v deklaraci funkce");
                                        return false;
                                    }
                                }
                            }
                        }
                        else
                        {
                            err_print(SYN_ERROR, "chybi ( v deklaraci funkce");
                            return false;
                        }
                    }
                }
                else
                {
                    err_print(SYN_ERROR, "chybi nazev v deklaraci funkce");
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool statement()
{
    if (current_token.token_type == IDP2)
    {
        if (next_token())
        {
            token_used = true;
            if (current_token.token_type == EQT1)
            {
                if (expression() == 0)
                {
                    if (next_token())
                    {
                        token_used = true;
                        // 7. pravidlo
                        if (current_token.token_type == SEM)
                        {
                            if(next_token())
                            {
                                token_used = true;
                                if(current_token.token_type == E_EOF || ((strcmp(current_token.data, "?")) == 0))
                                {
                                    return true;
                                }
                                else if(statement())
                                {
                                    return true;
                                }
                            }
                        }
                        else
                        {
                            err_print(SYN_ERROR, "chybi ; v prirazeni");
                        }
                    }
                }
                else if(next_token())
                {
                    token_used = true;
                    // 10. pravidlo
                    if (current_token.token_type == ID)
                    {
                        // TODO testnout jestli existuje funkce, pokud ne tak error (hashtable)
                        if (next_token()) {
                            token_used = true;
                            if (current_token.token_type == LB)
                            {
                                // mozna nejak checkovat ty argumenty nwm
                                if(args())
                                {
                                    if (next_token())
                                    {
                                        token_used = true;
                                        if (current_token.token_type == RB)
                                        {
                                            if (next_token())
                                            {
                                                token_used = true;
                                                if (current_token.token_type == SEM)
                                                {
                                                    if(next_token())
                                                    {
                                                        token_used = true;
                                                        if(current_token.token_type == E_EOF || ((strcmp(current_token.data, "?")) == 0))
                                                        {
                                                            return true;
                                                        }
                                                        else if(statement())
                                                        {
                                                            return true;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    err_print(SYN_ERROR, "chybi SEM v prirazeni");
                                                }
                                            }
                                        }
                                        else
                                        {
                                            err_print(SYN_ERROR, "chybi ) v prirazeni funkce");
                                        }
                                    }
                                }
                                else
                                {
                                    err_print(SYN_ERROR, "spatne argumenty v prirazeni funkce");
                                }
                            }
                            else
                            {
                                err_print(SYN_ERROR, "chybi ( v prirazeni funkce");
                            }
                        }
                    }
                    else
                    {
                        err_print(SYN_ERROR, "chybi nazev funkce v prirazeni");
                    }
                }
                else
                {
                    err_print(SYN_ERROR, "prec chyba");
                }
            }
            else
            {
                err_print(SYN_ERROR, "chybi = v prirazeni hodnoty nebo funkce");
            }
        }
    }
    // 8. pravidlo
    else if (current_token.token_type == KS)
    {
        if ((strcmp(current_token.data, "if")) == 0)
        {
            if (next_token())
            {
                token_used = true;
                if (current_token.token_type == LB)
                {
                    if(expression() == 0) {
                        if (next_token()) {
                            token_used = true;
                            if (current_token.token_type == RB) {
                                if (next_token()) {
                                    token_used = true;
                                    if (current_token.token_type == LSB) {
                                        if(next_token()) {
                                            token_used = true;
                                            if (statement()) {
                                                if (next_token()) {
                                                    token_used = true;
                                                    if (current_token.token_type == RSB) {
                                                        if (next_token()) {
                                                            token_used = true;
                                                            if (current_token.token_type == KS) {
                                                                if ((strcmp(current_token.data, "else")) == 0) {
                                                                    if (next_token()) {
                                                                        token_used = true;
                                                                        if (current_token.token_type == LSB) {
                                                                            if(next_token())
                                                                            {
                                                                                token_used = true;
                                                                                if(statement()) {
                                                                                    if (next_token()) {
                                                                                        token_used = true;
                                                                                        if (current_token.token_type == RSB) {
                                                                                            if (next_token())
                                                                                            {
                                                                                                token_used = true;
                                                                                                if(current_token.token_type == E_EOF || ((strcmp(current_token.data, "?")) == 0))
                                                                                                {
                                                                                                    return true;
                                                                                                }
                                                                                                else if(statement())
                                                                                                {
                                                                                                    return true;
                                                                                                }
                                                                                            }

                                                                                        } else {
                                                                                            err_print(SYN_ERROR,
                                                                                                      "chybi } ve větvi else");
                                                                                        }
                                                                                    }
                                                                                }
                                                                                else
                                                                                {
                                                                                    err_print(SYN_ERROR, "chyba ve statementu");
                                                                                }
                                                                            }
                                                                        } else {
                                                                            err_print(SYN_ERROR,
                                                                                      "chybi { ve větvi else");
                                                                        }
                                                                    }
                                                                } else {
                                                                    err_print(SYN_ERROR, "if neobsahuje else");
                                                                }
                                                            } else {
                                                                err_print(SYN_ERROR, "if neobsahuje else");
                                                            }
                                                        }
                                                    } else {
                                                        err_print(SYN_ERROR, "chybi } v ifu");
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                err_print(SYN_ERROR, "chyba v statementu");
                                            }
                                        }
                                    } else {
                                        err_print(SYN_ERROR, "chybi { v ifu");
                                    }
                                }
                            } else {
                                err_print(SYN_ERROR, "chybi ) v podmince ifu");
                            }
                        }
                    }
                    else
                    {
                        err_print(SYN_ERROR, "spatne expr v ifu");
                    }
                }
                else
                {
                    err_print(SYN_ERROR, "chybi ( v podmince ifu");
                }
            }
        }
        // 9. pravidlo
        else if ((strcmp(current_token.data, "while")) == 0)
        {
            if (next_token())
            {
                token_used = true;
                if (current_token.token_type == LB)
                {
                    if(expression() == 0) {
                        if (next_token()) {
                            token_used = true;
                            if (current_token.token_type == RB) {
                                if (next_token()) {
                                    token_used = true;
                                    if (current_token.token_type == LSB) {
                                        if(next_token())
                                        {
                                            token_used = true;
                                            if(statement())
                                            {
                                                if (next_token()) {
                                                    token_used = true;
                                                    if (current_token.token_type == RSB) {
                                                        if(next_token())
                                                        {
                                                            token_used = true;
                                                            if(current_token.token_type == E_EOF || ((strcmp(current_token.data, "?")) == 0))
                                                            {
                                                                return true;
                                                            }
                                                            else if(statement())
                                                            {
                                                                return true;
                                                            }
                                                            else
                                                            {
                                                                err_print(SYN_ERROR, "chyba ve statementu while");
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        err_print(SYN_ERROR, "chybi } v while");
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                err_print(SYN_ERROR, "chyba ve statementu while");
                                            }
                                        }
                                    } else {
                                        err_print(SYN_ERROR, "chybi { v while");
                                    }
                                }
                            } else {
                                err_print(SYN_ERROR, "chybi ) v podmince while");
                            }
                        }
                    }
                    else
                    {
                        err_print(SYN_ERROR, "spatne expr v while");
                    }
                }
                else
                {
                    err_print(SYN_ERROR, "chybi ( v podmince while");
                }
            }
        }
        // 12. pravidlo
        else if ((strcmp(current_token.data, "return")) == 0)
        {
            if(next_token())
            {
                token_used = true;
                if(current_token.token_type == SEM)
                {
                    return true;
                }
                else
                {
                    int i = (strlen(current_token.data))-1;
                    char *c = current_token.data;
                    if(c[0] == '$')
                    {
                        while(i != -1)
                        {
                            ungetc(c[i], stdin);
                            i--;
                        }
                    }
                    else if(i == -1)
                    {
                        if(next_token())
                        {
                            token_used = true;
                            if(current_token.token_type == SEM)
                            {
                                return true;
                            }
                            else
                            {
                                err_print(SYN_ERROR, "chybi ; v return");
                            }
                        }
                    }
                    else
                    {
                        ungetc(c[0], stdin);
                    }
                }
                if(expression() == 0)
                {
                    if (next_token())
                    {
                        token_used = true;
                        if (current_token.token_type == SEM)
                        {
                            return true;
                        }
                        else
                        {
                            err_print(SYN_ERROR, "chybi ; v return");
                        }
                    }
                }
                else
                {
                    err_print(SYN_ERROR, "spatny expr v return");
                }
            }
        }
        else if((strcmp(current_token.data, "function")) == 0)
        {
            char* c = current_token.data;
            ungetc(c[7], stdin);
            ungetc(c[6], stdin);
            ungetc(c[5], stdin);
            ungetc(c[4], stdin);
            ungetc(c[3], stdin);
            ungetc(c[2], stdin);
            ungetc(c[1], stdin);
            ungetc(c[0], stdin);
            return true;
        }
        else
        {
            err_print(SYN_ERROR, "spatne klicove slovo ve statementu");
        }
    }
    // 11. pravidlo
    else if (current_token.token_type == ID)
    {
        // TODO testnout jestli existuje funkce, pokud ne tak error (hashtable)
        if (next_token())
        {
            token_used = true;
            if (current_token.token_type == LB)
            {
                // mozna nejak checkovat ty argumenty nwm
                if(args())
                {
                    if (next_token())
                    {
                        token_used = true;
                        if (current_token.token_type == RB)
                        {
                            if (next_token())
                            {
                                token_used = true;
                                if (current_token.token_type == SEM)
                                {
                                    if(next_token())
                                    {
                                        token_used = true;
                                        if(current_token.token_type == E_EOF || ((strcmp(current_token.data, "?")) == 0))
                                        {
                                            return true;
                                        }
                                        else if(statement())
                                        {
                                            return true;
                                        }
                                        else
                                        {
                                            err_print(SYN_ERROR, "chyba ve statementu volani funkce");
                                        }
                                    }
                                }
                                else
                                {
                                    err_print(SYN_ERROR, "chybi ; ve volani funkce");
                                }
                            }
                        }
                        else
                        {
                            err_print(SYN_ERROR, "chybi ) ve volani funkce");
                        }
                    }
                }
                else
                {
                    err_print(SYN_ERROR, "spatne argumenty ve volani funkce");
                }
            }
            else
            {
                err_print(SYN_ERROR, "chybi ( ve volani funkce");
            }
        }
    }
    else if(current_token.token_type == RSB)
    {
        char *c = current_token.data;
        ungetc(c[0], stdin);
        return true;
    }
    else
    {
        return false;
    }
}

bool prog()
{
    if(current_token.token_type == E_EOF)
    {
        return true;
    }
    else if (next_token())
    {
        token_used = true;
        // 4. pravidlo
        if (current_token.token_type == E_EOF)
        {
            return true;
        }
        else if ((strcmp(current_token.data, "?")) == 0)
        {
            if(next_token())
            {
                token_used = true;
                if(current_token.token_type == GT)
                {
                    return true;
                }
            }
        }
        else if (current_token.token_type == GT)
        {
            return true;
        }
            // 2. pravidlo
        else if (function_def())
        {
            return prog();
        }
            // 3. pravidlo
        else if (statement())
        {
            return prog();
        }
        else
        {
            err_print(SYN_ERROR, "spadl prog");
        }
        // nwm jak udelat ?>EOF (5. pravidlo)
    }
}

bool param_list()
{
    if(next_token())
    {
        token_used = true;
        if(current_token.token_type == COM)
        {
            if(next_token())
            {
                token_used = true;
                if(type())
                {
                    if (next_token())
                    {
                        if (current_token.token_type == IDP2)
                        {
                            token_used = true;
                            return param_list();
                        }
                        else
                        {
                            err_print(SYN_ERROR, "k typu neni promenna");
                        }
                    }
                }
                else
                {
                    err_print(SYN_ERROR, "spatny datovy typ");
                }
            }
        }
        else if(type())
        {
            if (next_token())
            {
                if (current_token.token_type == IDP2)
                {
                    token_used = true;
                    return param_list();
                }
                else
                {
                    err_print(SYN_ERROR, "k typu neni promenna 2");
                }
            }
        }
        else if(current_token.token_type == RB)
        {
            char* c = current_token.data;
            ungetc(c[0], stdin);
            return true;
        }
    }

}

bool type()
{
    if (current_token.token_type == KS)
    {
        if (strcmp(current_token.data, "null") || strcmp(current_token.data, "int") ||
            strcmp(current_token.data, "string") || strcmp(current_token.data, "float") ||
            strcmp(current_token.data, "void") || strcmp(current_token.data, "?int") ||
            strcmp(current_token.data, "?float") || strcmp(current_token.data, "?string"))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (current_token.token_type == RB)
    {
        return false;
    }
    else
    {
        err_print(SYN_ERROR, "spatny typ");
    }
}

bool args()
{
    if (next_token())
    {
        token_used = true;
        if (current_token.token_type == COM)
        {
            if(next_token())
            {
                token_used = true;
                if (value())
                {
                    return args();
                }
                else
                {
                    err_print(SYN_ERROR, "nevim");
                }
            }
        }
        else if (value())
        {
            return args();
        }
        else if(current_token.token_type == RB)
        {
            char* c = current_token.data;
            ungetc(c[0], stdin);
            return true;
        }
    }
}

bool value()
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
