#include "../libs/List/include/ErrorHandler.h"
#include "../libs/List/include/WorkWithLogFile.h"
#include "HashTable.h"
#include "HashTableCtorDtor.h"
#include "HandleError.h"
#include "ReadFile.h"
#include "WorkingWithTable.h"
#include "HashTableDump.h"


int main(const int argc, const char** argv)
{
    const int _NUMBERS_OF_ARGUMENTS = 5;
    if(argc != _NUMBERS_OF_ARGUMENTS)
    {
        errorHandler(_NUMBER_OF_ARG_ERROR, __PRETTY_FUNCTION__);
        return _NUMBER_OF_ARG_ERROR;
    }

    ErrorNumbers check_error_list = _NO_ERROR;

    FILE* log_file_list = NULL;
    CHECK_ERROR_LIST(openLogFile(&log_file_list, &argv[1]));
    FILE* log_file_table = NULL;
    CHECK_ERROR_LIST(openLogFile(&log_file_table, &argv[2]));
    ErrorNum check_error_table = NO_ERROR;

    HashTableInfo hash_table = {};
    CHECK_ERROR_TABLE(hashTableCtor(&hash_table, log_file_list));

    char* buffer = NULL;

    CHECK_ERROR_TABLE(readFile(&argv[3], &buffer));

    CHECK_ERROR_TABLE(fillHashTable(&hash_table, buffer, log_file_list));

    CHECK_ERROR_TABLE(hashTableDump(&hash_table, log_file_table, __PRETTY_FUNCTION__));

    free(buffer);

    CHECK_ERROR_TABLE(readFile(&argv[4], &buffer));

    const int NUM_OF_REPEATS = 1;
    CHECK_ERROR_TABLE(findWordsFromBuffer(&hash_table, buffer, NUM_OF_REPEATS, log_file_list));

    CHECK_ERROR_TABLE(hashTableDtor(&hash_table, log_file_list));

    free(buffer);
    fclose(log_file_list );
    fclose(log_file_table);

    printf("DONE\n");

    return 0;
}
