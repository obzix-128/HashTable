#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef int ListElem_t;

const int MIN_LIST_SIZE = 16;
const int POISON_VALUE  = -123;

enum ErrorNumbers
{
    _NO_ERROR            = 0,
    _OPEN_ERROR          = 1,
    _NULL_ADDRESS_ERROR  = 2,
    _VER_FOUND_ERROR     = 3,
    _NUMBER_OF_ARG_ERROR = 4,
    _CALLOC_ERROR        = 5,
    _NOT_MIRRORED_ERROR  = 6,
    _LOOP_ERROR          = 7,
    _LIST_SIZE_ERROR     = 8,
    _LIST_FREE_ERROR     = 9,
    _LIST_GET_ERROR      = 10
};

struct ListCell_t
{
    ListElem_t data;
    int next;
    int prev;
};

struct ListInfo
{
    ListCell_t* cell;
    int capacity;
    int size;
    int free;
};

#define CHECK_NULL_ADDR_ERROR(variable, ERROR_NUMBER) if(variable == NULL) \
                                                          {return ERROR_NUMBER;}

#endif // LIST_H
