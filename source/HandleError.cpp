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
        default:
        {
            fprintf(stderr, "Unknown error\n");
            break;
        }
    }

    return NO_ERROR;
}
