#include "HandleError.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Обрабатывает ошибки
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum handleError(ErrorNum error, const char* func_name)
{
    fprintf(stderr, "%s\n", func_name);

    switch(error)
    {
        case NO_ERROR:
        {
            fprintf(stderr, "No errors were found\n");
            break;
        }
        case LIST_ERROR:
        {
            fprintf(stderr, "An error occurred in the list library\n");
            break;
        }
        case NULL_ADDRESS_ERROR:
        {
            fprintf(stderr, "ERROR: a null address was detected\n");
            break;
        }
        case ALLOC_ERROR:
        {
            fprintf(stderr, "ERROR: Memory allocation error\n");
            break;
        }
        case STAT_ERROR:
        {
            fprintf(stderr, "ERROR: stat\n");
            break;
        }
        case READ_ERROR:
        {
            fprintf(stderr, "ERROR: Error reading file\n");
            break;
        }
        case OPEN_ERROR:
        {
            fprintf(stderr, "ERROR: File not open\n");
            break;
        }
        case LENGTH_ERROR:
        {
            fprintf(stderr, "ERROR: The word being inserted exceeded the allowed length\n");
            break;
        }
        case SIZE_ERROR:
        {
            fprintf(stderr, "ERROR: Hash table buffer is full. You need to increase its value\n");
            break;
        }
        case CAPACITY_ERROR:
        {
            fprintf(stderr, "ERROR: The capacity table hash variable has an impossible value\n");
            break;
        }
        case BUCKET_DUPLICATE_ERROR:
        {
            fprintf(stderr, "ERROR: Two identical words found in one bucket\n");
            break;
        }
        case HASH_ERROR:
        {
            fprintf(stderr, "ERROR: An item with a hash that does not match the bucket was found in the bucket\n");
            break;
        }
        case BUFFER_DUPLICATE_ERROR:
        {
            fprintf(stderr, "ERROR: Two identical words found in hash table buffer\n");
            break;
        }
        default:
        {
            fprintf(stderr, "Unknown error\n");
            break;
        }
    }

    return NO_ERROR;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Проверяет Хэш таблицы на различные ошибки
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum hashTableVerificator(HashTableInfo* hash_table)
{
    CHECK_NULL_ADDR_ERROR(hash_table,         NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(hash_table->buffer, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(hash_table->bucket, NULL_ADDRESS_ERROR);

    if(hash_table->capacity <= 0)
    {
        return CAPACITY_ERROR;
    }

    if(hash_table->size > hash_table->capacity || hash_table->size < 0)
    {
        return SIZE_ERROR;
    }

    for(int counter = 0; counter < NUM_OF_BUCKETS_D; counter++) // Проверяем на наличие одинаковых слов в одном бакете
    {
        for(int i = hash_table->bucket[counter].list.cell[0].next; i != 0; i = hash_table->bucket[counter].list.cell[i].next) 
        {
            size_t word_length = strlen(hash_table->buffer + hash_table->bucket[counter].list.cell[i].data);

            if(word_length >= ALIGNMENT_D) // Проверяю слово на превышение длины
            {
                fprintf(stderr, "The error is caused by the word: %.*s\n", (unsigned int)word_length, 
                        hash_table->buffer + hash_table->bucket[counter].list.cell[i].data           );
                handleError(LENGTH_ERROR, __PRETTY_FUNCTION__);
                return LENGTH_ERROR;
            }
            uint32_t hash = calculateHash(hash_table->buffer + hash_table->bucket[counter].list.cell[i].data, word_length);
            hash = hash % NUM_OF_BUCKETS_D; // Приведение к размеру таблицы

            if(hash != (uint32_t)counter)
            {
                return HASH_ERROR;
            }

            for(int j = hash_table->bucket[counter].list.cell[i].next; j != 0; j = hash_table->bucket[counter].list.cell[j].next)
            {
                if(!strncmp(hash_table->buffer + hash_table->bucket[counter].list.cell[i].data, 
                            hash_table->buffer + hash_table->bucket[counter].list.cell[j].data, ALIGNMENT_D)) 
                { 
                    return BUCKET_DUPLICATE_ERROR;
                }
            }
        }
    }

    /*for(int i = 0; i < hash_table->size; i++) // Проверяем на наличие одинаковых слов в буфере
    {
        for(int j = i + 1; j < hash_table->size; j++)
        {
            if(!strncmp(hash_table->buffer + ALIGNMENT_D * i, 
                        hash_table->buffer + ALIGNMENT_D * j, ALIGNMENT_D)) 
            { 
                return BUFFER_DUPLICATE_ERROR;
            }
        }
    }*/

    return NO_ERROR;
}