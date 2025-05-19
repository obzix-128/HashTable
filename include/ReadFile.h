#ifndef READFILE_H
#define READFILE_H

#include <sys/stat.h>
#include <string.h>
#include "../libs/List/include/ErrorHandler.h"
#include "HashTable.h"
#include "HandleError.h"

ErrorNum readFile(const char** file_name, char** buffer);
ErrorNum processWordFromBuffer(char* buffer, size_t* length, uint32_t* hash);
ErrorNum skipBlankLines(char** buffer);

#endif // READFILE_H