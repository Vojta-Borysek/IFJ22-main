//
// Created by MSI on 13.10.2022.
//

#ifndef IFJ22_LEX_H
#define IFJ22_LEX_H

enum FSM_state {
    S,
    ID, IDP1, IDP2, IDT, KS,
    INT, TDES, DES, EXP, DEXP,
    STR, STRING, STES, STES1, STES2, STES3, STES4, STES5, STES6, STHEX1, STHEX2,
    PLUS, MUL, DIV, LB, RB, LT, LTE, GT, GTE, EQT1, EQT2, EQT3, NEQT1, NEQT2, NEQT3, COM, COL, KON, LSB, RSB, SEM, MIN,
    BKOM1, BKOM2, RKOM,
    E_EOF
};

typedef struct{
    enum FSM_state token_type;
    char *data;
    unsigned line;
} Token;

int getToken(Token *token);

#endif //IFJ22_LEX_H
