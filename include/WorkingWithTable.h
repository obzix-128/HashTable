#ifndef WORKINGWITHTABLE_H
#define WORKINGWITHTABLE_H

#include <assert.h>
#include <string.h>
#include <immintrin.h>
#include <stdint.h>
#include "../libs/List/include/ErrorHandler.h"
#include "../libs/List/include/DifferentInserts.h"
#include "HandleError.h"
#include "HashTable.h"
#include "ReadFile.h"

size_t calculateHash(char* word);
ErrorNum fillHashTable        (HashTableInfo* hash_table, char* buffer, FILE* log_file);
ErrorNum insertWord           (HashTableInfo* hash_table, char* buffer, size_t hash, FILE* log_file);
ErrorNum findWord             (HashTableInfo* hash_table, char* buffer, size_t hash, size_t length, int* value);
ErrorNum findWordsFromBuffer  (HashTableInfo* hash_table, char* buffer, FILE* log_file);
ErrorNum hashTableChangeMemory(HashTableInfo* hash_table);
int myStrncmp(char *str_one, char *str_two, size_t num);

#endif // WORKINGWITHTABLE_H