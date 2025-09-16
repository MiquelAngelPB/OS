#ifndef API_H
#define API_H

//Utilities.asm
char inpPort(short port);
void outPort(short port, char value);
void halt();

//Utilities.c
char readKey();
void moveCursor(short row, short col);
int cmpstr(char* str, char* str2);

#endif