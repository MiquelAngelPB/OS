#include "Console.h"
#include "Graphics.h"

char* string = "Welcome to the kernel! \n";
char* string2 = "Type help for a list of commands!\n\n";
char* title = " \n"
" #####  #####  \n"
" #   #  #      \n"
" #   #  #####  \n"
" #   #      #  \n"
" #####  #####  \n"
"               \n";

extern void kernelmain(void)
{
    setCommandMode(0);
    setBGColor(0x00);
    clearScreen();

    setColor(0x09);
    print(title);

    setColor(0x07);
    print(string);
    print(string2);

    render();
    consoleMain();
    while(1); //stop
}