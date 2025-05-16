#include "ReadFile.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Читает из файла, название которого передаётся в *file_name, и записывает содержимое в буфер
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

    *buffer = (char*) calloc(file_info.st_size, sizeof(char));
    CHECK_NULL_ADDR_ERROR(*buffer, ALLOC_ERROR);

    size_t num_of_elem_read = fread(*buffer, sizeof(char), file_info.st_size, file_with_data);
    if(num_of_elem_read != (size_t) file_info.st_size)
    {
        handleError(READ_ERROR, __PRETTY_FUNCTION__);
        return READ_ERROR;
    }

    fclose(file_with_data);

    return NO_ERROR;
}