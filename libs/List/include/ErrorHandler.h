#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include "List.h"

ErrorNumbers errorHandler(ErrorNumbers error, const char* func_name);

#define CHECK_ERROR(func) check_error = func; \
                          if(check_error != _NO_ERROR) \
                          { \
                              errorHandler(check_error, __PRETTY_FUNCTION__); \
                              return check_error; \
                          }

#endif // ERRORHANDLER_H
