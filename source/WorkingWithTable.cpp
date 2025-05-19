#include "WorkingWithTable.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Хэш функция. Считает Хэш для слова, пока не встретит \n.
Дополнительная информация по хэш функции здесь: https://en.wikipedia.org/wiki/Cyclic_redundancy_check
------------------------------------------------------------------------------------------------------------------------------------*/
/*uint32_t calculateHash(char *word, size_t length) 
{
    uint32_t crc = 0xFFFFFFFF;

    for(size_t i = 0; i < length; i++) 
    {
        crc ^= word[i];
        for(int j = 0; j < 8; j++) 
        {
            crc = (crc >> 1) ^ (0x82F63B78 & -(crc & 1));
        }
    }
    return ~crc;
}*/
uint32_t calculateHash(char* word, size_t length)
{
    uint32_t hash = 0xFFFFFFFF;

    __asm__ volatile (
        ".intel_syntax noprefix\n"        // Переключаемся на Intel-синтаксис
        "mov %[h], 0xFFFFFFFF\n"          // Инициализация hash = 0xFFFFFFFF
        "test %[len], %[len]\n"           // Проверка длины (length == 0?)
        "jz .Ldone\n"                     // Если длина 0, пропустить цикл
        ".Lloop:\n"
        "crc32 %[h], byte ptr [%[ptr]]\n" // CRC32 для текущего байта (Intel-синтаксис)
        "inc %[ptr]\n"                    // Переход к следующему байту
        "dec %[len]\n"                    // Уменьшение счётчика
        "jnz .Lloop\n"                    // Повторять, пока len != 0
        ".Ldone:\n"
        "not %[h]\n"                      // Инвертирование битов (финальный шаг CRC32)
        ".att_syntax prefix\n"            // Возвращаемся к AT&T-синтаксису (по умолчанию в GCC)
        : [h] "+r" (hash)                 // Выход: обновлённый hash
        : [ptr] "r" (word),               // Вход: указатель на строку
          [len] "r" (length)              // Вход: длина строки
        : "cc", "memory"                  // Сообщаем компилятору о изменении флагов и памяти
    );

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
        uint32_t hash   = 0;
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
        uint32_t hash   = 0;
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
ErrorNum insertWord(HashTableInfo* hash_table, char* buffer, uint32_t hash, FILE* log_file)
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
ErrorNum findWord(HashTableInfo* hash_table, char* buffer, uint32_t hash, size_t length, int* value)
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
}

/*------------------------------------------------------------------------------------------------------------------------------------
Моя версия strncmp, оптимизированная intrinsic-ами. Сравнивает строки длинной не более 32 байт. __attribute__((noinline))
------------------------------------------------------------------------------------------------------------------------------------*/
int myStrncmp(char *str_one, char *str_two, size_t num) 
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
