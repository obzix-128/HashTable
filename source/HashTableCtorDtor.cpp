#include "HashTableCtorDtor.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Создаёт хэш таблицу
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum hashTableCtor(HashTableInfo* hash_table, FILE* log_file)
{
    CHECK_NULL_ADDR_ERROR(hash_table, NULL_ADDRESS_ERROR);

    const int MIN_BUFFER_SIZE = 32768;
    hash_table->capacity = MIN_BUFFER_SIZE;
    hash_table->size = 0;

    hash_table->buffer = (char*) aligned_alloc(ALIGNMENT_D, ALIGNMENT_D * MIN_BUFFER_SIZE);
    CHECK_NULL_ADDR_ERROR(hash_table->buffer, ALLOC_ERROR);

    hash_table->bucket = (Bucket_t*) calloc(NUM_OF_BUCKETS_D, sizeof(Bucket_t));
    CHECK_NULL_ADDR_ERROR(hash_table->bucket, ALLOC_ERROR);

    ErrorNumbers check_error_list = _NO_ERROR;
    for(int i = 0; i < NUM_OF_BUCKETS_D; i++)
    {
        CHECK_ERROR_LIST(listCtor(&(hash_table->bucket[i].list), log_file));
        hash_table->bucket[i].num_occurrences = 0;
    }

    return NO_ERROR;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Удаляет хэш таблицу
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum hashTableDtor(HashTableInfo* hash_table, FILE* log_file)
{
    CHECK_NULL_ADDR_ERROR(hash_table, NULL_ADDRESS_ERROR);

    ErrorNumbers check_error_list = _NO_ERROR;
    for(int i = 0; i < NUM_OF_BUCKETS_D; i++)
    {
        CHECK_ERROR_LIST(listDtor(&(hash_table->bucket[i].list), log_file));
    }

    memset(hash_table->buffer, 0, ALIGNMENT_D      * hash_table->capacity);
    memset(hash_table->bucket, 0, NUM_OF_BUCKETS_D * sizeof(Bucket_t)   );
        
    hash_table->capacity = 0;
    hash_table->size     = 0;

    free(hash_table->buffer);
    free(hash_table->bucket);

    hash_table->buffer = NULL;
    hash_table->bucket = NULL;

    return NO_ERROR;
}