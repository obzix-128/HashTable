#include "../include/DifferentInserts.h"
#include "../include/Verificator.h"
#include "../include/ErrorHandler.h"
#include "../include/WorkWithLogFile.h"


static ErrorNumbers listInsert(ListInfo* my_list, ListElem_t value, int element_Before,
                               int element_after);


ErrorNumbers listInsertHead(ListInfo* my_list, ListElem_t value, FILE* log_file)
{
    ErrorNumbers check_error = _NO_ERROR;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, -1, _STATUS_IS_UNCHANGED));
    #endif // RELEASE_MODE_D

    CHECK_ERROR(listInsert(my_list, value, 0, my_list->cell[0].next));

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, my_list->cell[0].next,
                         _INSERTING_CELL));
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    return check_error;
}

ErrorNumbers listInsertTail(ListInfo* my_list, ListElem_t value, FILE* log_file)
{
    ErrorNumbers check_error = _NO_ERROR;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, -1, _STATUS_IS_UNCHANGED));
    #endif // RELEASE_MODE_D

    CHECK_ERROR(listInsert(my_list, value, my_list->cell[0].prev, 0));

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, my_list->cell[0].prev,
                         _INSERTING_CELL));
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    return check_error;
}

ErrorNumbers listInsertBefore(ListInfo* my_list, int element_number, ListElem_t value, FILE* log_file)
{
    ErrorNumbers check_error = _NO_ERROR;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, -1, _STATUS_IS_UNCHANGED));
    #endif // RELEASE_MODE_D

    CHECK_ERROR(listInsert(my_list, value, element_number, my_list->cell[element_number].next));

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, my_list->cell[element_number].next,
                         _INSERTING_CELL));
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    return check_error;
}

ErrorNumbers listInsertAfter(ListInfo* my_list, int element_number, ListElem_t value, FILE* log_file)
{
    ErrorNumbers check_error = _NO_ERROR;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, -1, _STATUS_IS_UNCHANGED));
    #endif // RELEASE_MODE_D

    CHECK_ERROR(listInsert(my_list, value, my_list->cell[element_number].prev, element_number));

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, my_list->cell[element_number].prev,
                         _INSERTING_CELL));
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    return check_error;
}

static ErrorNumbers listInsert(ListInfo* my_list, ListElem_t value, int element_before,
                               int element_after)
{
    ErrorNumbers check_error = _NO_ERROR;
    CHECK_ERROR(listChangeMemory(my_list, &element_before, &element_after));

    int empty_cell = my_list->free;
    my_list->free = my_list->cell[empty_cell].next;

    my_list->cell[empty_cell].data = value;
    my_list->cell[empty_cell].next = my_list->cell[element_before].next;
    my_list->cell[element_before].next = empty_cell;
    my_list->cell[empty_cell].prev = my_list->cell[element_after].prev;
    my_list->cell[element_after].prev = empty_cell;

    my_list->size += 1;

    return check_error;
}

ErrorNumbers listChangeMemory(ListInfo* my_list, int* element_before, int* element_after)
{
    ErrorNumbers check_error = _NO_ERROR;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    int new_capacity = 0;

    if(my_list->size < MIN_LIST_SIZE - 1)
    {
        return _NO_ERROR;
    }
    else if(my_list->size == my_list->capacity - 1)
    {
        new_capacity = my_list->capacity * 2;
    }
    else if(my_list->size == (my_list->capacity / 4) - 1)
    {
        new_capacity = my_list->capacity / 2;
    }
    else
    {
        return check_error;
    }

    ListCell_t* new_array_of_cells = (ListCell_t*) calloc(new_capacity, sizeof(ListCell_t));

    new_array_of_cells[0].data = my_list->cell[0].data;
    new_array_of_cells[0].next = 1;
    new_array_of_cells[0].prev = my_list->size;

    for(int i = 1, j = my_list->cell[0].next; i <= my_list->size; i++, j = my_list->cell[j].next)
    {
        new_array_of_cells[i].data = my_list->cell[j].data;
        new_array_of_cells[i].next = i + 1;
        new_array_of_cells[i].prev = i - 1;

        if(j == *element_before)
        {
            *element_before = i * (-1); // To avoid repeated changes
        }
        else if(j == *element_after)
        {
            *element_after =  i * (-1); // To avoid repeated changes
        }
    }
    *element_before = (*element_before) * (-1);
    *element_after = (*element_after) * (-1);
    new_array_of_cells[my_list->size].next = 0;

    my_list->free = my_list->size + 1;

    for(int i = my_list->free; i < new_capacity; i++)
    {
        new_array_of_cells[i].data = 0;
        new_array_of_cells[i].next = i + 1;
        new_array_of_cells[i].prev = -1;
    }

    new_array_of_cells[new_capacity - 1].next = 0;

    memset(my_list->cell, POISON_VALUE, sizeof(ListCell_t) * my_list->capacity);

    free(my_list->cell);

    my_list->cell = new_array_of_cells;
    my_list->capacity = new_capacity;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    return check_error;
}
