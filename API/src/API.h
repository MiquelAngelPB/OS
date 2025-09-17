#ifndef API_H
#define API_H

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define TEXT_COLS 40
#define TEXT_ROWS 25

//Utilities.asm
char inpPort(short port);
void outPort(short port, char value);
void halt();

//Utilities.c
int cmpstr(char* str, char* str2);

//Utilities_VGA.c
char readKey();
void moveCursor(short row, short col);
void putCharAt(unsigned int row, unsigned int col, char chr, char color);
void drawRectangle(unsigned short x, unsigned short y, unsigned short width, unsigned short height, char color);
void clearScreen();

//Console.c
void consoleMain();
void clear();
void print(char* str, int commandMode);
void putChar(char c, int commandMode);
void setColor(int c);

#endif