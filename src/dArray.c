//
// Created by MSI on 13.10.2022.
//

#include <stdlib.h>
#include "../headers/dArray.h"

/**
 * Funkce na inicializaci pole
 * @param arr Pole znaků
 * @param size Velikost pole
 * @return Pokud dojde k chybě při alokaci paměti vrací 99
 */
int array_init(tok_arr *arr, size_t size)
{
    arr->array = malloc(size*sizeof(char));
    if(arr->array == NULL)
    {
        return 99;
    }
    else
    {
        arr->asize = 0;
        arr->size = size;
        for(size_t i = 0; i < size; i++)
        {
            arr->array[i] = 0;
        }
        return 0;
    }
}

/**
 * Funkce pro uložení znaku do pole znaků
 * @param arr Pole znaků
 * @param token Znak tokenu
 * @return Pokud dojde k chybě při alokaci paměti vrací 99
 */
int array_insert(tok_arr *arr, char token)
{
    if(arr->asize == (arr->size - 1))
    {
        arr->size += 5;
        arr->array = realloc(arr->array, arr->size*sizeof(char));
        if(arr->array == NULL)
        {
            return 99;
        }
        else
        {
            for(size_t i = arr->asize; i < arr->size; i++)
            {
                arr->array[i] = 0;
            }
        }
    }
    arr->array[arr->asize++] = token;
    return 0;
}