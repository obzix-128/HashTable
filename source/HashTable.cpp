#include "../libs/List/include/ErrorHandler.h"
#include "../libs/List/include/WorkWithLogFile.h"
#include "HashTable.h"
#include "HashTableCtorDtor.h"
#include "HandleError.h"
#include "ReadFile.h"
#include "WorkingWithTable.h"

int main(const int argc, const char** argv)
{
    const int _NUMBERS_OF_ARGUMENTS = 3;
    if(argc != _NUMBERS_OF_ARGUMENTS)
    {
        errorHandler(_NUMBER_OF_ARG_ERROR, __PRETTY_FUNCTION__);
        return _NUMBER_OF_ARG_ERROR;
    }

    ErrorNumbers check_error_list = _NO_ERROR;

    FILE* log_file = NULL;
    CHECK_ERROR_LIST(openLogFile(&log_file, &argv[1]));
    ErrorNum check_error_table = NO_ERROR;

    HashTableInfo hash_table = {};
    CHECK_ERROR_TABLE(hashTableCtor(&hash_table, log_file));

    char* buffer = NULL;

    CHECK_ERROR_TABLE(readFile(&argv[2], &buffer));

    CHECK_ERROR_TABLE(fillHashTable(&hash_table, buffer, log_file));

    CHECK_ERROR_TABLE(hashTableDtor(&hash_table, log_file));

    printf("DONE\n");

    return 0;
}
