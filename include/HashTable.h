#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>

#include "../libs/List/include/List.h"

enum ErrorNum
{
    NO_ERROR               = 0,
    LIST_ERROR             = 1,
    NULL_ADDRESS_ERROR     = 2,
    ALLOC_ERROR            = 3,
    STAT_ERROR             = 4,
    READ_ERROR             = 5,
    OPEN_ERROR             = 6,
    LENGTH_ERROR           = 7,
    SIZE_ERROR             = 8,
    CAPACITY_ERROR         = 9,
    BUCKET_DUPLICATE_ERROR = 10,
    HASH_ERROR             = 11,
    BUFFER_DUPLICATE_ERROR = 12,
};

struct Bucket_t
{
    ListInfo list;
    int num_occurrences;
};

struct HashTableInfo
{
    Bucket_t* bucket;
    char* buffer;
    int capacity;
    int size;
};

#define ALIGNMENT_D 32
#define NUM_OF_BUCKETS_D 2047

#endif // HASHTABLE_H