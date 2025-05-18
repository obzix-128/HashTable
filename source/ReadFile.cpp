#include "ReadFile.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Читает из файла, название которого передаётся в *file_name, и записывает содержимое в буфер.
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum readFile(const char** file_name, char** buffer)
{
    CHECK_NULL_ADDR_ERROR(file_name, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(buffer,    NULL_ADDRESS_ERROR);

    struct stat file_info = {};

    int check_error = stat(*file_name, &file_info);
    if(check_error != 0)
    {
        handleError(STAT_ERROR, __PRETTY_FUNCTION__);
        return STAT_ERROR;
    }

    FILE* file_with_data = fopen(*file_name, "r");
    CHECK_NULL_ADDR_ERROR(file_with_data, OPEN_ERROR);

    *buffer = (char*) calloc(file_info.st_size + 1, sizeof(char));
    CHECK_NULL_ADDR_ERROR(*buffer, ALLOC_ERROR);

    size_t num_of_elem_read = fread(*buffer, sizeof(char), file_info.st_size, file_with_data);
    if(num_of_elem_read != (size_t) file_info.st_size)
    {
        handleError(READ_ERROR, __PRETTY_FUNCTION__);
        return READ_ERROR;
    }

    (*buffer)[file_info.st_size] = '\0';

    fclose(file_with_data);

    return NO_ERROR;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Проверяет, чтобы слова в буфере не превышали максимальной допустимой длины и считает их хэш.
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum processWordFromBuffer(char* buffer, size_t* length, size_t* hash)
{
    CHECK_NULL_ADDR_ERROR(buffer, NULL_ADDRESS_ERROR);

    size_t word_length = strcspn(buffer, "\n"); // Считаю длину слова в буфере

    if(word_length >= ALIGNMENT_D) // Проверяю слово на превышение длины
    {
        fprintf(stderr, "The error is caused by the word: %.*s\n", (unsigned int)word_length, buffer);
        handleError(LENGTH_ERROR, __PRETTY_FUNCTION__);
        return LENGTH_ERROR;
    }

    size_t word_hash = calculateHash(buffer); // Считаю хэш для слова
    *hash = word_hash % NUM_OF_BUCKETS_D; // Приведение к размеру таблицы
    *length = word_length;

    return NO_ERROR;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Пропускает в буфере пустые строки.
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNum skipBlankLines(char** buffer)
{
    CHECK_NULL_ADDR_ERROR(buffer,  NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(*buffer, NULL_ADDRESS_ERROR);

    size_t counter = 0;
    while((*buffer)[counter] == '\n')
    {
        counter++;
    }
    *buffer += counter;

    return NO_ERROR;
}

