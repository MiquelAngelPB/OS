#include "API.h"

char* title = " \n"
    " #####  #####  \n"
    " #   #  #      \n"
    " #   #  #####  \n"
    " #   #      #  \n"
    " #####  #####  \n"
    "               \n";

extern void kernelmain(void)
{
    clear();

    char* string = "Welcome to the kernel! \n";
    char* string2 = "Type help for a list of commands!\n\n";
    
    setColor(6);
    print(title, 0);

    setColor(8);
    print(string, 0);
    print(string2, 0);

    consoleMain();
    while(1); //stop
}