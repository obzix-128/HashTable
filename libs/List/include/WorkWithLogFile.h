#ifndef WORKWITHLOGFILE_H
#define WORKWITHLOGFILE_H

#include "List.h"

enum CellStatus
{
    _STATUS_IS_UNCHANGED = 0,
    _DELETING_CELL       = 1,
    _INSERTING_CELL      = 2
};

ErrorNumbers openLogFile(FILE** log_file, const char** log_file_name);

ErrorNumbers listDump(ListInfo* my_list, FILE* log_file, const char* func_name, int element_number,
                      CellStatus status);
ErrorNumbers buildGraphs(ListInfo* my_list, FILE* log_file, int element_number, CellStatus status);

#define LIST_DUMP_PATH_D "libs/List/image/"

#endif // WORKWITHLOGFILE_H
