#ifndef WORKINGWITHTABLE_H
#define WORKINGWITHTABLE_H

#include <assert.h>
#include <string.h>
#include "../libs/List/include/ErrorHandler.h"
#include "../libs/List/include/DifferentInserts.h"
#include "HashTable.h"
#include "HandleError.h"

ErrorNum fillHashTable        (HashTableInfo* hash_table, char* buffer, FILE* log_file);
ErrorNum insertWord           (HashTableInfo* hash_table, char* buffer, size_t hash, size_t length, FILE* log_file);
ErrorNum hashTableChangeMemory(HashTableInfo* hash_table);
ErrorNum checkAvailability    (HashTableInfo* hash_table, char* buffer, size_t hash, size_t length, int* check_availability);
size_t calculateHash(char* word);
ErrorNum skipBlankLines(char** buffer);

#endif // WORKINGWITHTABLE_H