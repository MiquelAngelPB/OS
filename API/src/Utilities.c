#include "API.h"

int cmpstr(char* str, char* str2)
{
    char* str_cp = str;
    char* str2_cp = str2;

    int result;
    while (*str || *str2)
    {
        if (*str == *str2)
        {
            result = 1;
        }
        else
        {
            return 0;
        }

        str++;
        str2++;
    }
    return result;
}