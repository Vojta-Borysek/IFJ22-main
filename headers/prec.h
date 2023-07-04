//
// Created by dava on 20.11.22.
//

#ifndef IFJ22_PREC_H
#define IFJ22_PREC_H

#include "lex.h"

typedef enum{
    T_PLUS,
    T_MINUS,
    T_CON,
    T_MUL,
    T_DIV,
    T_LT,
    T_GT,
    T_LTE,
    T_GTE,
    T_EQ,
    T_NEQ,
    T_LB,
    T_RB,
    T_INT,
    T_FLOAT,
    T_STRING,
    T_ID,
    STOP,
    DOLLAR,
    REDUCTION
}Types;

typedef enum{
    I,
    BREBR,
    EPLUSE,
    EMINUSE,
    ECONE,
    EMULE,
    EDIVE,
    ELTE,
    EGTE,
    ELTEE,
    EGTEE,
    EEQE,
    ENEQE,
    FF
}ReductionRules;

int exp_reduction(Token *token);

int expression();

#endif //IFJ22_PREC_H
