#ifndef IO_H
#define IO_H

//IO.asm
char inpPort(short port);
void outPort(short port, char value);
char readMem(short port);
void setMem(short port, char value);
void halt();

//IO.c
char readKey();
char readKeyASCII();

#endif