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