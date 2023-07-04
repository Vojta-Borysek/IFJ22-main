//napsat koment

#ifndef IFJ22_ERROR_H
#define IFJ22_ERROR_H



#define LEX_ERROR 1 //chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).
#define SYN_ERROR 2 //chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, chybějící hlavička, atp.).
#define SEM_ERROR 3 //sémantická chyba v programu – nedefinovaná funkce, pokus o redefinice funkce.
#define BAD_PARAM 4 //sémantická/běhová chyba v programu – špatný počet/typ parametrů u volání funkce či typ návratové hodnoty z funkce.
#define UNDEF_VAR 5 //sémantická chyba v programu – použití nedefinované proměnné.
#define MISSING_S 6 //sémantická/běhová chyba v programu – chybějící/přebývající výraz v příkazu návratu z funkce.
#define TYPE_COMP 7 //sémantická/běhová chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.
#define OTHERS    8 //ostatní sémantické chyby.
#define INTERNAL_ERR 99 //interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd.).

#endif