#ifndef HANDLEERROR_H
#define HANDLEERROR_H

#include "HashTable.h"
#include "WorkingWithTable.h"

ErrorNum handleError(ErrorNum error, const char* func_name);
ErrorNum hashTableVerificator(HashTableInfo* hash_table);

#ifdef _DEBUG
#define CHECK_ERROR_LIST(func) check_error_list = func; \
                               if(check_error_list != _NO_ERROR) \
                               { \
                                   errorHandler(check_error_list, __PRETTY_FUNCTION__); \
                                   return LIST_ERROR; \
                               }

#define CHECK_ERROR_TABLE(func) check_error_table = func; \
                                if(check_error_table != NO_ERROR) \
                                { \
                                    handleError(check_error_table, __PRETTY_FUNCTION__); \
                                    return check_error_table; \
                                }
#else
#define CHECK_ERROR_LIST(func) func
#define CHECK_ERROR_TABLE(func) func
#endif // _DEBUG

#endif // HANDLEERROR_H