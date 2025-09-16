#include "API.h"

char readKey()
{
    return inpPort(0x60);
}

void moveCursor(short row, short col)
{
    //https://wiki.osdev.org/Text_Mode_Cursor
    short i = row * 80 + col;

    outPort(0x3D4, 0x0F);
	outPort(0x3D5, (char) (i & 0xFF));
	outPort(0x3D4, 0x0E);
	outPort(0x3D5, (char) ((i >> 8) & 0xFF));
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