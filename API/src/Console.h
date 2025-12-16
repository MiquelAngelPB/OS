#ifndef CONSOLE_H
#define CONSOLE_H

void consoleMain();
void print(char* str);
void putchar(char chr);
void clearConsole();

void setColor(char color);
void setBGColor(char color);
void setCommandMode(int mode);
void setCursorVisibility(int visible);

#endif