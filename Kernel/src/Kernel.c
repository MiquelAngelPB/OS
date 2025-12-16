#include "Console.h"
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
    clearConsole();

    char* string = "Welcome to the kernel! \n";
    char* string2 = "Type help for a list of commands!\n\n";

    setColor(0x08);
    setBGColor(0x00);

    print(title);
    print(string);
    print(string2);

    render();
    consoleMain();
    while(1); //stop
}