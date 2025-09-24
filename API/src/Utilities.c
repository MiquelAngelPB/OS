#include "API.h"

char scancodeToASCII[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-', 0,'\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',  0, '\\',
    'z','x','c','v','b','n','m',',','.','/',  0, '*',  0, ' ',
};

char readKey()
{
    return inpPort(0x60);
}

char readKeyASCII()
{
    char scancode = readKey();
    return scancodeToASCII[scancode];
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