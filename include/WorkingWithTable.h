#ifndef WORKINGWITHTABLE_H
#define WORKINGWITHTABLE_H

#include <assert.h>
#include <string.h>
#include "../libs/List/include/ErrorHandler.h"
#include "../libs/List/include/DifferentInserts.h"
#include "../libs/List/include/WorkWithLogFile.h"
#include "HashTable.h"
#include "HandleError.h"

ErrorNum fillHashTable(HashTableInfo* hash_table, char* buffer, FILE* log_file);
size_t calculateHash(char* word);
ErrorNum skipBlankLines(char** buffer);

#endif // WORKINGWITHTABLE_H