#ifndef CTORANDDTOR_H
#define CTORANDDTOR_H

#include "List.h"
#include <string.h>

ErrorNumbers listCtor(ListInfo* my_list, FILE* log_file);
ErrorNumbers listDtor(ListInfo* my_list, FILE* log_file);

#endif // CTORANDDTOR_H
