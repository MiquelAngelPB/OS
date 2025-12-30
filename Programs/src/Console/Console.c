#include "Graphics.h"
#include "Console.h"
#include "IO.h"
#include "Audio.h"

#define BYTES_PER_CHAR 2
#define LINE_MAX_CHAR 100

//console variables
volatile char* buffer = (volatile char*)0xb8000;
char actualLine[LINE_MAX_CHAR] = {'\0'};
int showCursor = 1;
char color = 0x07;   //Source: https://wiki.osdev.org/Text_UI
char bgColor = 0x00;
int col = 0;
int row = 0;
int linePos = 0;
int commandMode = 1;

//methods
void clearConsole();
void print(char* str);
void putChar(char chr);
void readLine(char* str);
void setColor(char col);
void setBGColor(char col);
void setCommandMode(int mode);
void setCursorVisibility(int visible);

void consoleMain()
{
    int IsKeyPressed = 0;
    while(1)
    {
        if (showCursor)
        {
            putCharAt(row, col, '_', color, bgColor);
        }        

        char scancode, ascii;
        scancode = readKey();
        ascii = readKeyASCII();

        if (scancode & 0x80)
        {
            IsKeyPressed = 0;
            continue;
        }

        if (IsKeyPressed == 0)
        {
            if (ascii)
            {
                IsKeyPressed = 1;
                putChar(ascii);
            }
        }

        render();
    }

    while(1); //stop
}

void clearConsole()
{
    col = 0;
    row = 0;

    clearScreen();
}

void print(char* str)
{
    while (*str)
    {
        putChar(*str);
        str++;
    }
}

void putChar(char chr)
{
    if (row >= TEXT_ROWS)
    {
        clearConsole();
    }

    if (col >= TEXT_COLS)
    {
        col = 0;
        row++;
    }

    int i = (TEXT_COLS * row + col) * BYTES_PER_CHAR;
    switch (chr)
    {
        case '\n':
            putCharAt(row, col, '\0', 0x00, bgColor);
            col = 0;
            row++;

            if (commandMode == 1)
            {                
                compile(actualLine); //compile here <---------------------------------------------------
                for (int i = 0; i < LINE_MAX_CHAR; i++)
                {
                    actualLine[i] = '\0';
                }
            }

            break;

        case '\b':
            if (col != 0)
            {
                putCharAt(row, col, '\0', 0x00, bgColor);
                col--;
            }
            
            if (commandMode)
            {
                actualLine[linePos] = '\0';
                if (linePos != 0)
                {
                    linePos--;
                }
            }
                        
            break;

        case '\t':
            if (commandMode == 0)
            {
                color++;
                color = color % 10;
            }
            break;

        default:
            putCharAt(row, col, chr, color, bgColor);
            col++;

            if (commandMode)
            {
                if (linePos < LINE_MAX_CHAR)
                {
                    actualLine[linePos] = chr;
                    linePos++;
                }
            }
            break;
    }
}

//TODO, join all of this into setParam(param, value)
void setColor(char col)
{
    color = col;
}

char getConsoleColor()
{
    return color;
}

void setBGColor(char col)
{
    bgColor = col;
}

char getBGConsoleColor()
{
    return bgColor;
}

void setCommandMode(int mode)
{
    commandMode = mode;
}

void setCursorVisibility(int visible)
{
    showCursor = visible;
}
