int strToHex(char* str)
{
    int result = 0;
    while (*str)
    {
        char c = *str;
        int value = 0;
        if (c <= '9' && c >= '0')
        {
            value = c - '0';
        }
        else if (c <= 'F' && c >= 'A')
        {
            value = c - 'A' + 10;
        }
        else if (c <= 'f' && c >= 'a')
        {
            value = c - 'a' + 10;
        }
        else
        {
            break;
        }

        result = (result << 4) | value; // Shift left by 4 bits and add the new value
        str++;
    }
    return result;
}

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