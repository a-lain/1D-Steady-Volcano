#include "útil.h"

string printfToString(const char *format, ...)
{
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    return string(buffer);
}
