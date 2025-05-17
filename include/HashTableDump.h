#ifndef HASHTABLEDUMP_H
#define HASHTABLEDUMP_H

#include "../libs/List/include/WorkWithLogFile.h"
#include "../libs/List/include/ErrorHandler.h"
#include "HashTable.h"
#include "HandleError.h"

ErrorNum hashTableDump(HashTableInfo* hash_table, FILE* log_file_table, const char* func_name);

#endif // HASHTABLEDUMP_H