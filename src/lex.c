//
// Created by MSI on 12.10.2022.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "../headers/lex.h"
#include "../headers/dArray.h"

// main aby fungoval makefile a na testování
/*int main()
{
    Token kokot;
    do
    {
        if(getToken(&kokot) != 0)
        {
            exit(1);
        }
        printf("%u : %s\n", kokot.token_type, kokot.data);
    }while(kokot.token_type != E_EOF);
}*/

/**
 * Funkce kontrolující zda ID nepatří mezi klíčové slova
 * @param id Nalezené ID
 * @param state Stav automatu
 */
void KSlova(char *id, enum FSM_state *state)
{
    char *KSA[] = {"else", "float", "function", "if", "int", "null",
                   "return", "string", "void", "while", "?string", "?int", "?float"};
    int len = sizeof(KSA)/sizeof(KSA[0]);
    for(int i = 0; i < len; i++)
    {
        if(strcmp(KSA[i], id) == 0)
        {
            *state = KS;
            return;
        }
    }
}

double pow(double a, double b)
{
    int c = 1;
    for(int i = 0; i < b; i++)
    {
        c = c*a;
    }
    return c;
}

int OctalToDecimal(char* octal)
{
    int octLength = strlen(octal);
    double dec = 0;

    for (int i = 0; i < octLength; ++i)
    {
        dec += (octal[i] - 48) * pow(8, ((octLength - i) - 1));
    }

    return (int)dec;
}

/**
 * Funkce převádějící Hexadecimální hodnotu na decimální, aby se mohla převést na ascii
 * @param ascii1 první Hex hodnota
 * @param ascii2 druhá Hex hodnota
 * @return decimální hodnota
 */
int HEXTOD(char ascii1, char ascii2)
{
    int result = 0;
    int tmp;
    if(ascii1 >= '0' && ascii1 <= '9')
    {
        tmp = ascii1 - '0';
    }
    else
    {
        tmp = ascii1 - 'A' + 10;
    }
    result *= 16;
    result += tmp;
    if(ascii2 >= '0' && ascii2 <= '9')
    {
        tmp = ascii2 - '0';
    }
    else
    {
        tmp = ascii2 - 'A' + 10;
    }
    result *= 16;
    result += tmp;
    return result;
}

int getToken(Token *token)
{
    static unsigned line = 1;
    int c = getchar();

    tok_arr tok_data;
    array_init(&tok_data, 10);

    char ascii[4];

    bool tok_found = false;
    enum FSM_state state = S;

    while(!tok_found)
    {
        switch(state)
        {
            case S:
                if(c == '\"')
                {
                    state = STR;
                }
                else if(c >= '0' && c <= '9')
                {
                    array_insert(&tok_data, (char) c);
                    state = INT;
                }
                else if(c == '/')
                {
                    state = DIV;
                }
                else if(c == '{')
                {
                    array_insert(&tok_data, (char) c);
                    state = LSB;
                }
                else if(c == '}')
                {
                    array_insert(&tok_data, (char) c);
                    state = RSB;
                }
                else if(c == '(')
                {
                    array_insert(&tok_data, (char) c);
                    state = LB;
                }
                else if(c == ')')
                {
                    array_insert(&tok_data, (char) c);
                    state = RB;
                }
                else if(c == ':')
                {
                    array_insert(&tok_data, (char) c);
                    state = COL;
                }
                else if(c == ',')
                {
                    array_insert(&tok_data, (char) c);
                    state = COM;
                }
                else if(c == ';')
                {
                    array_insert(&tok_data, (char) c);
                    state = SEM;
                }
                else if(c == '*')
                {
                    array_insert(&tok_data, (char) c);
                    state = MUL;
                }
                else if(c == '-')
                {
                    array_insert(&tok_data, (char) c);
                    state = MIN;
                }
                else if(c == '+')
                {
                    array_insert(&tok_data, (char) c);
                    state = PLUS;
                }
                else if(c == '.')
                {
                    array_insert(&tok_data, (char) c);
                    state = KON;
                }
                else if(c == '<')
                {
                    array_insert(&tok_data, (char) c);
                    state = LT;
                }
                else if(c == '>')
                {
                    array_insert(&tok_data, (char) c);
                    state = GT;
                }
                else if(c == '=')
                {
                    array_insert(&tok_data, (char) c);
                    state = EQT1;
                }
                else if(c == '!')
                {
                    array_insert(&tok_data, (char) c);
                    state = NEQT1;
                }
                else if(c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                {
                    array_insert(&tok_data, (char) c);
                    state = ID;
                }
                else if(c == '$')
                {
                    array_insert(&tok_data, (char) c);
                    state = IDP1;
                }
                else if(c == 32 || c == 9)
                {
                    break;
                }
                else if(c == '\n')
                {
                    line++;
                }
                else if(c == EOF)
                {
                    state = E_EOF;
                }
                else if(c == '?')
                {
                    array_insert(&tok_data, (char) c);
                    state = IDT;
                }
                else
                {
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case INT:
                if(c >= '0' && c <= '9')
                {
                    array_insert(&tok_data, (char) c);
                }
                else if(c == '.')
                {
                    array_insert(&tok_data, (char) c);
                    state = TDES;
                }
                else if(tolower(c) == 'e')
                {
                    array_insert(&tok_data, (char) c);
                    state = EXP;
                }
                else
                {
                    ungetc(c, stdin);
                    tok_found = true;
                }
                break;
            case TDES:
                if(c >= '0' && c <= '9')
                {
                    array_insert(&tok_data, (char) c);
                    state = DES;
                }
                else
                {
                    ungetc(c, stdin);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
            case DES:
                if(c >= '0' && c <= '9')
                {
                    array_insert(&tok_data, (char) c);
                }
                else if(tolower(c) == 'e')
                {
                    array_insert(&tok_data, (char) c);
                    state = EXP;
                }
                else
                {
                    ungetc(c, stdin);
                    tok_found = true;
                }
                break;
            case EXP:
                if((c >= '0' && c <= '9') || (c == '+') || (c == '-'))
                {
                    array_insert(&tok_data, (char) c);
                    state = DEXP;
                }
                else
                {
                    ungetc(c, stdin);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case DEXP:
                if(c >= '0' && c <= '9')
                {
                    array_insert(&tok_data, (char) c);
                }
                else
                {
                    ungetc(c, stdin);
                    tok_found = true;
                }
                break;
            case STR:
                if(c == '\"')
                {
                    state = STRING;
                }
                else if(c == '\\')
                {
                    state = STES;
                    ascii[0] = c;
                }
                else if(c >= 32 && c <= 127 && c != '\"' && c != '\\' && c != 36)
                {
                    array_insert(&tok_data, (char) c);
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case STES:
                if(c == '\\')
                {
                    array_insert(&tok_data, 92);
                    state = STR;
                }
                else if(c == 'n')
                {
                    array_insert(&tok_data, 10);
                    state = STR;
                }
                else if(c == 't')
                {
                    array_insert(&tok_data, 9);
                    state = STR;
                }
                else if(c == '\"')
                {
                    array_insert(&tok_data, 34);
                    state = STR;
                }
                else if(c == '$')
                {
                    array_insert(&tok_data, 36);
                    state = STR;
                }
                else if(tolower(c) == 'x')
                {
                    ascii[1] = 'x';
                    state = STHEX1;
                }
                else if(c == '0')
                {
                    ascii[1] = '0';
                    state = STES1;
                }
                else if(c == '1' || c == '2')
                {
                    ascii[1] = c;
                    state = STES2;
                }
                else if(c == '3')
                {
                    ascii[1] = '3';
                    state = STES3;
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case STHEX1:
                if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                {
                    ascii[2] = c;
                    state = STHEX2;
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case STHEX2:
                if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                {
                    ascii[3] = c;
                    int kk = HEXTOD(ascii[2], ascii[3]);
                    char charValue = kk+0;
                    array_insert(&tok_data, charValue);
                    state = STR;
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case STES1:
                if(c == '0')
                {
                    ascii[2] = '0';
                    state = STES4;
                }
                else if(c >= '1' && c <= '7')
                {
                    ascii[2] = c;
                    state = STES5;
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case STES4:
                if(c >= '1' && c <= '7')
                {
                    ascii[3] = c;
                    int result = OctalToDecimal(&ascii[1]);
                    array_insert(&tok_data, (char) result);
                    state = STR;
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case STES2:
                if(c >= '0' && c <= '7')
                {
                    ascii[2] = c;
                    state = STES5;
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case STES3:
                if(c >= '0' && c <= '6')
                {
                    ascii[2] = c;
                    state = STES5;
                }
                else if(c == '7')
                {
                    ascii[2] = '7';
                    state = STES6;
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case STES5:
                if(c >= '0' && c <= '7')
                {
                    ascii[3] = c;
                    int result = OctalToDecimal(&ascii[1]);
                    array_insert(&tok_data, (char) result);
                    state = STR;
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case STES6:
                if(c >= '0' && c <= '7')
                {
                    ascii[3] = c;
                    int result = OctalToDecimal(&ascii[1]);
                    array_insert(&tok_data, (char) result);
                    state = STR;
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case STRING:
                ungetc(c, stdin);
                tok_found = true;
                break;
            case DIV:
                if(c == '/')
                {
                    state = RKOM;
                }
                else if(c == '*')
                {
                    state = BKOM1;
                }
                else
                {
                    array_insert(&tok_data, '/');
                    ungetc(c, stdin);
                    tok_found = true;
                }
                break;
            case RKOM:
                if(c == '\n')
                {
                    line++;
                    state = S;
                }
                else if(c == EOF)
                {
                    ungetc(c, stdin);
                    state = S;
                }
                else
                {
                    state = RKOM;
                }
                break;
            case BKOM1:
                if(c == '*')
                {
                    state = BKOM2;
                }
                else if(c == '\n')
                {
                    line++;
                }
                else if(c == EOF)
                {
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                else
                {
                    state = BKOM1;
                }
                break;
            case BKOM2:
                if(c == '/')
                {
                    state = S;
                }
                else if(c == EOF)
                {
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                else
                {
                    state = BKOM1;
                }
                break;
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
                ungetc(c, stdin);
                tok_found = true;
                break;
            case LT:
                if(c == '=')
                {
                    array_insert(&tok_data, (char) c);
                    state = LTE;
                }
                else
                {
                    ungetc(c, stdin);
                    tok_found = true;
                }
                break;
            case GT:
                if(c == '=')
                {
                    array_insert(&tok_data, (char) c);
                    state = GTE;
                }
                else
                {
                    ungetc(c, stdin);
                    tok_found = true;
                }
                break;
            case GTE:
            case LTE:
                ungetc(c, stdin);
                tok_found = true;
                break;
            case EQT1:
                if(c == '=')
                {
                    array_insert(&tok_data, (char) c);
                    state = EQT2;
                }
                else
                {
                    ungetc(c, stdin);
                    tok_found = true;
                }
                break;
            case EQT2:
                if(c == '=')
                {
                    array_insert(&tok_data, (char) c);
                    state = EQT3;
                }
                else
                {
                    /*ungetc(c, stdin);
                    tok_found = true;*/
                    ungetc(c, stdin);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case EQT3:
                ungetc(c, stdin);
                tok_found = true;
                break;
            case NEQT1:
                if(c == '=')
                {
                    array_insert(&tok_data, (char) c);
                    state = NEQT2;
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case NEQT2:
                if(c == '=')
                {
                    array_insert(&tok_data, (char) c);
                    state = NEQT3;
                }
                else
                {
                    array_insert(&tok_data, (char) c);
                    token->line = line;
                    token->data = tok_data.array;
                    token->token_type = state;
                    return 1;
                }
                break;
            case NEQT3:
                ungetc(c, stdin);
                tok_found = true;
                break;
            case IDT:
                if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                {
                    array_insert(&tok_data, (char) c);
                    state = IDT;
                }
                else
                {
                    ungetc(c, stdin);
                    tok_found = true;
                }
                break;
            case ID:
                if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
                {
                    array_insert(&tok_data, (char) c);
                    state = ID;
                }
                else
                {
                    ungetc(c, stdin);
                    tok_found = true;
                }
                break;
            case IDP1:
                if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
                {
                    array_insert(&tok_data, (char) c);
                    state = IDP2;
                }
                break;
            case IDP2:
                if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
                {
                    array_insert(&tok_data, (char) c);
                    state = IDP2;
                }
                else
                {
                    ungetc(c, stdin);
                    tok_found = true;
                }
                break;
            case E_EOF:
                tok_found = true;
                break;
            default:
                break;
        }
        if(!tok_found)
        {
            c = getchar();
        }
    }
    if(state == ID || state == IDP2 || state == IDT)
    {
        KSlova(tok_data.array, &state);
    }
    token->line = line;
    token->data = tok_data.array;
    token->token_type = state;
    return 0;
}

