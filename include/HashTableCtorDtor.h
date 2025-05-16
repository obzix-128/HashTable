#ifndef HASHTABLECTORDTOR_H
#define HASHTABLECTORDTOR_H

#include <string.h>
#include "HashTable.h"
#include "HandleError.h"
#include "../libs/List/include/ErrorHandler.h"
#include "../libs/List/include/CtorAndDtor.h"

ErrorNum hashTableCtor(HashTableInfo* hashTable, FILE* log_file);
ErrorNum hashTableDtor(HashTableInfo* hashTable, FILE* log_file);

#endif // HASHTABLECTORDTOR_H