#ifndef DIFFERENTGET_H
#define DIFFERENTGET_H

#include "List.h"

ErrorNumbers listGet(ListInfo* my_list, int element_number, ListElem_t* value, FILE* log_file);
ErrorNumbers findElement(ListInfo* my_list, int* element_number, ListElem_t value);
ErrorNumbers findHead(ListInfo* my_list, int* head);
ErrorNumbers findTail(ListInfo* my_list, int* tail);

#endif // DIFFERENTGET_H
