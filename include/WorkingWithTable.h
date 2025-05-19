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

uint32_t calculateHash(char* word, size_t length);
ErrorNum fillHashTable        (HashTableInfo* hash_table, char* buffer, FILE* log_file);
ErrorNum insertWord           (HashTableInfo* hash_table, char* buffer, uint32_t hash, FILE* log_file);
ErrorNum findWordsFromBuffer  (HashTableInfo* hash_table, char* buffer, FILE* log_file);
ErrorNum hashTableChangeMemory(HashTableInfo* hash_table);
int myStrncmp(char *str_one, char *str_two, size_t num);
ErrorNum findWord(HashTableInfo* hash_table, char* buffer, uint32_t hash, size_t length, int* value);
//extern "C" ErrorNum findWord(HashTableInfo* hash_table, char* buffer, uint32_t hash, size_t length, int* value);

#endif // WORKINGWITHTABLE_H