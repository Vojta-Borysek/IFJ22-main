//
// Created by MSI on 13.10.2022.
//

#ifndef IFJ22_DARRAY_H
#define IFJ22_DARRAY_H

/**
 * Pole pro uložení jednotlivých znaků tokenů
 * size = celková velikost pole
 * asize = část pole využitá lexem
 */
typedef struct {
    char *array;
    size_t size;
    size_t asize;
} tok_arr;

int array_init(tok_arr *arr, size_t size);
int array_insert(tok_arr *arr, char token);

#endif //IFJ22_DARRAY_H
