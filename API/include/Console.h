#ifndef CONSOLE_H
#define CONSOLE_H

#define TEXT_COLS 40
#define TEXT_ROWS 25

//Console.c
void consoleMain();
void print(char* str);
void putChar(char chr);
void clearConsole();

void setColor(char color);
char getConsoleColor();
void setBGColor(char color);
char getBGConsoleColor();
void setCommandMode(int mode);
void setCursorVisibility(int visible);

//Compiler.c
void compile(char* line);

#endif
