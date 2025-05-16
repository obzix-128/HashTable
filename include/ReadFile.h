#ifndef READFILE_H
#define READFILE_H

#include <sys/stat.h>
#include "HashTable.h"
#include "HandleError.h"

ErrorNum readFile(const char** file_name, char** buffer);

#endif // READFILE_H