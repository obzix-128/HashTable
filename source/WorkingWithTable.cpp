#include "WorkingWithTable.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Заполняет хэш таблицу словами из буфера buffer. Заполнение происходит с выравниванием каждого слова по 32 байта. Максимальная длина
слова 31 байт, все не занятые буквами байты заполнятся нулями. Слова в буфере должны лежать по одному и их должно разделять \n.
Все символы между \n воспринимаются как буквы.
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum fillHashTable(HashTableInfo* hash_table, char* buffer, FILE* log_file)
{
    CHECK_NULL_ADDR_ERROR(hash_table, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(buffer,    NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(log_file,  NULL_ADDRESS_ERROR);

    ErrorNum check_error_table = NO_ERROR;
    CHECK_ERROR_TABLE(hashTableVerificator(hash_table));

    CHECK_ERROR_TABLE(skipBlankLines(&buffer));

    while(*buffer != '\0')
    {
        size_t length = 0; // Считаю длину слова в буфере
        while(buffer[length] != '\n' && buffer[length] != '\0')
        {
            length++;
        }

        if(length >= ALIGNMENT_D) // Проверяю слово на превышение длины
        {
            fprintf(stderr, "The error is caused by the word: %.*s\n", (unsigned int)length, buffer);
            handleError(LENGTH_ERROR, __PRETTY_FUNCTION__);
            return LENGTH_ERROR;
        }

        size_t hash = calculateHash(buffer); // Считаю хэш для слова

        int save_val_num_occurrences = hash_table->bucket[hash].num_occurrences;
        for(int i = hash_table->bucket[hash].list.cell[0].next; // Циклом проверяю наличие слова в таблице
            i != 0; 
            i = hash_table->bucket[hash].list.cell[i].next     ) 
        {
            // Если есть, увеличиваем счётчик появлений
            if(!strncmp(buffer, hash_table->buffer + hash_table->bucket[hash].list.cell[i].data, length)) 
            { 
                if(hash_table->buffer[length + hash_table->bucket[hash].list.cell[i].data] == '\0')
                {
                    hash_table->bucket[hash].num_occurrences += 1;
                    break;
                }
            }
        }

        if(save_val_num_occurrences == hash_table->bucket[hash].num_occurrences) // Если нет, то добавляем
        {
            CHECK_ERROR_TABLE(hashTableChangeMemory(hash_table));
            
            for(size_t i = 0; i < length; i++)
            {
                hash_table->buffer[hash_table->size * ALIGNMENT_D + i] = buffer[i];
            }
            for(size_t i = length; i < ALIGNMENT_D; i++)
            {
                hash_table->buffer[hash_table->size * ALIGNMENT_D + i] = '\0';
            }

            ErrorNumbers check_error_list = _NO_ERROR;
            CHECK_ERROR_LIST(listInsertHead(&hash_table->bucket[hash].list, hash_table->size * ALIGNMENT_D, log_file));

            hash_table->bucket[hash].num_occurrences += 1;
            hash_table->size += 1;
        }

        buffer += length;

        CHECK_ERROR_TABLE(skipBlankLines(&buffer));
    }

    CHECK_ERROR_TABLE(hashTableVerificator(hash_table));

    return check_error_table;
}

/*------------------------------------------------------------------------------------------------------------------------------------
При необходимости увеличивает размер буфера хэш таблицы
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum hashTableChangeMemory(HashTableInfo* hash_table)
{
    CHECK_NULL_ADDR_ERROR(hash_table, NULL_ADDRESS_ERROR);

    ErrorNum check_error_table = NO_ERROR;
    CHECK_ERROR_TABLE(hashTableVerificator(hash_table));

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

    CHECK_ERROR_TABLE(hashTableVerificator(hash_table));

    return NO_ERROR;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Пропускает символы '\n' в буфере
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum skipBlankLines(char** buffer)
{
    CHECK_NULL_ADDR_ERROR(buffer, NULL_ADDRESS_ERROR);

    while(**buffer == '\n')
    {
        *buffer += 1;
    }

    return NO_ERROR;
}

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

    hash = hash % NUM_OF_BUCKETS_D; // Приведение к размеру таблицы

    return hash;
}