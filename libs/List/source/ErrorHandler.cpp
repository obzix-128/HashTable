#include "../include/ErrorHandler.h"


ErrorNumbers errorHandler(ErrorNumbers error, const char* func_name)
{
    fprintf(stderr, "%s\n", func_name);

    switch(error)
    {
        case _NO_ERROR:
        {
            fprintf(stderr, "No errors were found\n");
            break;
        }
        case _OPEN_ERROR:
        {
            fprintf(stderr, "ERROR: File not open\n");
            break;
        }
        case _NULL_ADDRESS_ERROR:
        {
            fprintf(stderr, "ERROR: a null address was detected\n");
            break;
        }
        case _VER_FOUND_ERROR:
        {
            fprintf(stderr, "ERROR: The verifier has detected an error\n");
            break;
        }
        case _NUMBER_OF_ARG_ERROR:
        {
            fprintf(stderr, "ERROR: Incorrect number of arguments\n");
            break;
        }
        case _CALLOC_ERROR:
        {
            fprintf(stderr, "ERROR: calloc\n");
            break;
        }
        case _NOT_MIRRORED_ERROR:
        {
            fprintf(stderr, "ERROR: The arrays Next and Prev are not mirrored\n");
            break;
        }
        case _LOOP_ERROR:
        {
            fprintf(stderr, "ERROR: A loop in the Next and/or Prev arrays\n");
            break;
        }
        case _LIST_SIZE_ERROR:
        {
            fprintf(stderr, "The size exceeded the capacity\n");
            break;
        }
        case _LIST_FREE_ERROR:
        {
            fprintf(stderr, "ERROR: The free variable has assumed an impossible value\n");
            break;
        }
        case _LIST_GET_ERROR:
        {
            fprintf(stderr, "ERROR: The specified element cannot be extracted\n");
            break;
        }
        default:
        {
            fprintf(stderr, "Unknown error\n");
            break;
        }
    }

    return _NO_ERROR;
}
