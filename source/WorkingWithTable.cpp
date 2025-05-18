#include "WorkingWithTable.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Хэш функция. Считает Хэш для слова, пока не встретит \n.
Описание Хэш функции: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
------------------------------------------------------------------------------------------------------------------------------------*/
size_t calculateHash(char* word)
{
    assert(word);

    const size_t OFFSET_BASIS = 2166136261u;  // Начальное значение (константа)
    const size_t FNV_PRIME    = 16777619u;    // Простое число для умножения
    
    size_t hash = OFFSET_BASIS;
    for(; *word != '\n' && *word != '\0'; word++)
    {
        hash ^= (size_t)*word;  // Шаг 1: XOR с текущим байтом
        hash *= FNV_PRIME;      // Шаг 2: Умножение на FNV-prime
    }

    return hash;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Ищу в хэш таблице слова из буфера "repeats" раз.
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum findWordsFromBuffer(HashTableInfo* hash_table, char* buffer, FILE* log_file)
{
    CHECK_NULL_ADDR_ERROR(hash_table, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(buffer,    NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(log_file,  NULL_ADDRESS_ERROR);

    ErrorNum check_error_table = NO_ERROR;

    #ifdef _DEBUG
    CHECK_ERROR_TABLE(hashTableVerificator(hash_table));
    #endif // _DEBUG

    CHECK_ERROR_TABLE(skipBlankLines(&buffer));

    while(*buffer != '\0')
    {
        size_t length = 0;
        size_t hash   = 0;
        CHECK_ERROR_TABLE(processWordFromBuffer(buffer, &length, &hash));

        int data = 0;
        CHECK_ERROR_TABLE(findWord(hash_table, buffer, hash, length, &data));

        buffer += length;

        CHECK_ERROR_TABLE(skipBlankLines(&buffer));
    }

    #ifdef _DEBUG
    CHECK_ERROR_TABLE(hashTableVerificator(hash_table));
    #endif // _DEBUG

    return check_error_table;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Заполняю хэш таблицу словами из буфера. Слова должны быть разделены '\n'.
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum fillHashTable(HashTableInfo* hash_table, char* buffer, FILE* log_file)
{
    CHECK_NULL_ADDR_ERROR(hash_table, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(buffer,    NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(log_file,  NULL_ADDRESS_ERROR);

    ErrorNum check_error_table = NO_ERROR;

    #ifdef _DEBUG
    CHECK_ERROR_TABLE(hashTableVerificator(hash_table));
    #endif // _DEBUG

    CHECK_ERROR_TABLE(skipBlankLines(&buffer));

    while(*buffer != '\0')
    {
        size_t length = 0;
        size_t hash   = 0;
        CHECK_ERROR_TABLE(processWordFromBuffer(buffer, &length, &hash));

        int check_availability = -1;
        CHECK_ERROR_TABLE(findWord(hash_table, buffer, hash, length, &check_availability)); // Проверяем слово на наличие

        if(check_availability == -1) // Если его ещё нет, то добавляем
        {
            CHECK_ERROR_TABLE(insertWord(hash_table, buffer, hash, log_file));
        }    

        buffer += length; // Переходим к следующему слову

        CHECK_ERROR_TABLE(skipBlankLines(&buffer));
    }

    #ifdef _DEBUG
    CHECK_ERROR_TABLE(hashTableVerificator(hash_table));
    #endif // _DEBUG

    return check_error_table;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Вставляет слово в хэш таблицу, если его ещё нет.
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum insertWord(HashTableInfo* hash_table, char* buffer, size_t hash, FILE* log_file)
{
    CHECK_NULL_ADDR_ERROR(hash_table, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(buffer,     NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(log_file,   NULL_ADDRESS_ERROR);

    ErrorNum check_error_table = NO_ERROR;
    CHECK_ERROR_TABLE(hashTableChangeMemory(hash_table));
    
    size_t counter = 0;
    while(buffer[counter] != '\n')
    {
        hash_table->buffer[hash_table->size * ALIGNMENT_D + counter] = buffer[counter];
        counter++;
    }
    while(counter < ALIGNMENT_D)
    {
        hash_table->buffer[hash_table->size * ALIGNMENT_D + counter] = '\0';
        counter++;
    }

    ErrorNumbers check_error_list = _NO_ERROR;
    CHECK_ERROR_LIST(listInsertHead(&hash_table->bucket[hash].list, hash_table->size * ALIGNMENT_D, log_file));

    hash_table->bucket[hash].num_occurrences += 1;
    hash_table->size += 1;

    return check_error_table;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Проверяет текущее слово на наличие в бакете, если оно уже есть, то не добавляем его и просто увеличиваем счётчик появлений.
------------------------------------------------------------------------------------------------------------------------------------*/
/*__attribute__((noinline)) ErrorNum findWord(HashTableInfo* hash_table, char* buffer, size_t hash, size_t length, int* value)
{
    CHECK_NULL_ADDR_ERROR(hash_table, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(buffer,     NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(value,      NULL_ADDRESS_ERROR);

    for(int i = hash_table->bucket[hash].list.cell[0].next; i != 0; i = hash_table->bucket[hash].list.cell[i].next) 
    {
        if(!myStrncmp(hash_table->buffer + hash_table->bucket[hash].list.cell[i].data, buffer, length)) 
        { 
            if((hash_table->buffer + hash_table->bucket[hash].list.cell[i].data)[length] == '\0')
            {
                hash_table->bucket[hash].num_occurrences += 1;
                *value = hash_table->bucket[hash].list.cell[i].data;
                break;
            }
        }
    }

    return NO_ERROR;
}*/

/*------------------------------------------------------------------------------------------------------------------------------------
Моя версия strncmp, оптимизированная intrinsic-ами. Сравнивает строки длинной не более 32 байт.
------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" int myStrncmp(char *str_one, char *str_two, size_t num) 
{
    __m256i first  = _mm256_loadu_si256((__m256i*)(str_one));
    __m256i second = _mm256_loadu_si256((__m256i*)(str_two));

    __m256i cmp = _mm256_cmpeq_epi8(first, second);
                  
    uint32_t mask = _mm256_movemask_epi8(cmp);

    if(mask != 0xFFFFFFFF) 
    {
        if(__builtin_ctz(~mask) < num)
        {
            return 1;
        }
    }

    return 0;
}

/*------------------------------------------------------------------------------------------------------------------------------------
При необходимости увеличивает размер буфера хэш таблицы.
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum hashTableChangeMemory(HashTableInfo* hash_table)
{
    CHECK_NULL_ADDR_ERROR(hash_table, NULL_ADDRESS_ERROR);

    #ifdef _DEBUG
    ErrorNum check_error_table = NO_ERROR;
    CHECK_ERROR_TABLE(hashTableVerificator(hash_table));
    #endif // _DEBUG

    int new_capacity = 0;

    if(hash_table->size == hash_table->capacity)
    {
        new_capacity = hash_table->capacity * 2;
    }
    else
    {
        return NO_ERROR;
    }

    char* new_buffer = (char*) aligned_alloc(ALIGNMENT_D, ALIGNMENT_D * new_capacity);
    CHECK_NULL_ADDR_ERROR(new_buffer, ALLOC_ERROR);

    memcpy(new_buffer, hash_table->buffer, ALIGNMENT_D * hash_table->capacity);
    memset(hash_table->buffer, 0, ALIGNMENT_D * hash_table->capacity);

    free(hash_table->buffer);

    hash_table->buffer = new_buffer;
    hash_table->capacity = new_capacity;

    #ifdef _DEBUG
    CHECK_ERROR_TABLE(hashTableVerificator(hash_table));
    #endif // _DEBUG

    return NO_ERROR;
}
