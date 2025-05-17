#include "HashTableDump.h"


ErrorNum hashTableDump(HashTableInfo* hash_table, FILE* log_file_table, const char* func_name)
{
    CHECK_NULL_ADDR_ERROR(hash_table, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(log_file_table,   NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(func_name,  NULL_ADDRESS_ERROR);

    fprintf(log_file_table, "<h2>Dump is called from the function %s </h2>\n", func_name);

    fprintf(log_file_table, "&hash_table = {%p}\n", hash_table);

    fprintf(log_file_table, "hash_table->capacity: address = {%p}; value = [%d]\n", &hash_table->capacity, hash_table->capacity);

    fprintf(log_file_table, "hash_table->size: addres = {%p}; value = [%d]\n", &hash_table->size, hash_table->size);

    if(hash_table->bucket != NULL)
    {
        fprintf(log_file_table, "&hash_table->bucket = {%p}\n", hash_table->bucket);

        for(int i = 0; i < NUM_OF_BUCKETS_D; i++)
        {
            fprintf(log_file_table, "hash_table->bucket[%d]:\n", i);
            fprintf(log_file_table, "hash_table->bucket[%d].num_occurrences = [%d]\n", i, hash_table->bucket[i].num_occurrences);
            ErrorNumbers check_error_list = _NO_ERROR;
            CHECK_ERROR_LIST(listDump(&hash_table->bucket[i].list, log_file_table, __PRETTY_FUNCTION__, -1, _STATUS_IS_UNCHANGED));

            if(hash_table->bucket[i].list.capacity > 0 && hash_table->bucket[i].list.cell != NULL)
            {
                fprintf(log_file_table, "hash_table->bucket[%d].list.cell->data:\n", i);
                for(int j = 1; j < hash_table->bucket[i].list.capacity; j++)
                {
                    fprintf(log_file_table, "%*d ", ALIGNMENT_D, j);
                }
                fprintf(log_file_table, "\n");
                for(int j = 1; j <= hash_table->bucket[i].list.size; j++)
                {
                    fprintf(log_file_table, "%*s ", ALIGNMENT_D, (hash_table->buffer + hash_table->bucket[i].list.cell[j].data));
                }
                fprintf(log_file_table, "\n");
            }
        }
    }
    else
    {
        fprintf(log_file_table, "&hash_table->bucket = {NULL}\n");
    }

    if(hash_table->buffer != NULL)
    {
        fprintf(log_file_table, "&hash_table->buffer {%p}:\n", hash_table->buffer);

        for(int i = 0; i < hash_table->size; i++)
        {
            fprintf(log_file_table, "%16d: %s\n", i * ALIGNMENT_D, hash_table->buffer + i * ALIGNMENT_D);
        }
    }
    else
    {
        fprintf(log_file_table, "&hash_table->buffer = {NULL}\n");
    }

    return NO_ERROR;
}