#include "../include/DifferentGet.h"
#include "../include/Verificator.h"
#include "../include/ErrorHandler.h"
#include "../include/WorkWithLogFile.h"
#include "../include/DifferentInserts.h"


ErrorNumbers listGet(ListInfo* my_list, int element_number, ListElem_t* value, FILE* log_file)
{
    CHECK_NULL_ADDR_ERROR(my_list, _NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(value, _NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(log_file, _NULL_ADDRESS_ERROR);

    if(element_number <= 0 &&
       element_number > my_list->capacity &&
       my_list->cell[element_number].prev == -1)
    {
        return _LIST_GET_ERROR;
    }

    ErrorNumbers check_error = _NO_ERROR;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));

    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, element_number, _DELETING_CELL));
    #endif // RELEASE_MODE_D

    *value = my_list->cell[element_number].data;
    my_list->cell[element_number].data = 0;

    my_list->cell[my_list->cell[element_number].next].prev = my_list->cell[element_number].prev;
    my_list->cell[my_list->cell[element_number].prev].next = my_list->cell[element_number].next;

    my_list->cell[element_number].next = my_list->free;
    my_list->free = element_number;

    my_list->cell[element_number].prev = -1;

    my_list->size -= 1;

    CHECK_ERROR(listChangeMemory(my_list, &element_number, &element_number));

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));

    CHECK_ERROR(listDump(my_list, log_file, __PRETTY_FUNCTION__, -1, _STATUS_IS_UNCHANGED));
    #endif // RELEASE_MODE_D

    return check_error;
}

ErrorNumbers findHead(ListInfo* my_list, int* head)
{
    CHECK_NULL_ADDR_ERROR(my_list, _NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = _NO_ERROR;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    *head = my_list->cell[0].next;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    return check_error;
}

ErrorNumbers findTail(ListInfo* my_list, int* tail)
{
    CHECK_NULL_ADDR_ERROR(my_list, _NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = _NO_ERROR;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    *tail = my_list->cell[0].prev;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    return check_error;
}

ErrorNumbers findElement(ListInfo* my_list, int* element_number, ListElem_t value)
{
    CHECK_NULL_ADDR_ERROR(my_list, _NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = _NO_ERROR;

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    for(int i = 1; i < my_list->capacity; i++)
    {
        if(my_list->cell[i].data == value)
        {
            *element_number = i;
            
            #ifndef RELEASE_MODE_D
            CHECK_ERROR(listVerificator(my_list));
            #endif // RELEASE_MODE_D

            return check_error;
        }
    }
    fprintf(stdout, "An element with this value could not be found\n");

    #ifndef RELEASE_MODE_D
    CHECK_ERROR(listVerificator(my_list));
    #endif // RELEASE_MODE_D

    return check_error;
}
