#include "../include/CtorAndDtor.h"
#include "../include/WorkWithLogFile.h"
#include "../include/ErrorHandler.h"


ErrorNumbers listCtor(ListInfo* my_list, FILE* log_file)
{
    CHECK_NULL_ADDR_ERROR(my_list, _NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(log_file, _NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = _NO_ERROR;

    my_list->capacity = MIN_LIST_SIZE;
    my_list->size = 0;

    const int FIRST_FREE_CELL = 1;
    my_list->free = FIRST_FREE_CELL;

    my_list->cell = (ListCell_t*) calloc(my_list->capacity, sizeof(ListElem_t) + (sizeof(int) * 2));
    CHECK_NULL_ADDR_ERROR(my_list->cell, _CALLOC_ERROR);

    const int EMPTY_CELL = 0;
    for(int i = 0; i < my_list->capacity; i++)
    {
        my_list->cell[i].data = (ListElem_t)EMPTY_CELL;
    }
    my_list->cell[0].data = (ListElem_t)POISON_VALUE;

    my_list->cell[0].next = 0;
    for(int i = 1; i < my_list->capacity; i++)
    {
        my_list->cell[i].next = i + 1;
    }
    my_list->cell[my_list->capacity - 1].next = 0;

    for(int i = 0; i < my_list->capacity; i++)
    {
        my_list->cell[i].prev = EMPTY_CELL;
    }
    my_list->cell[0].prev = 0;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, -1, _STATUS_IS_UNCHANGED));
    #endif //RELEASE_MODE_D

    return check_error;
}

ErrorNumbers listDtor(ListInfo* my_list, FILE* log_file)
{
    CHECK_NULL_ADDR_ERROR(my_list, _NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(log_file, _NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = _NO_ERROR;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, -1, _STATUS_IS_UNCHANGED));
    #endif //RELEASE_MODE_D

    memset(my_list->cell, POISON_VALUE, (sizeof(ListElem_t) + sizeof(int) * 2) * my_list->capacity);

    my_list->capacity = POISON_VALUE;
    my_list->size = POISON_VALUE;
    my_list->free = POISON_VALUE;

    free(my_list->cell);

    my_list->cell = NULL;

    return check_error;
}
