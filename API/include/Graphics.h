#ifndef GRAPHICS_H
#define GRAPHICS_H

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

extern unsigned char backBuffer[64000];
void putCharAt(unsigned int row, unsigned int col, char chr, char color, char bgColor);
void drawRectangle(unsigned short x, unsigned short y, unsigned short width, unsigned short height, char color);
void clearScreen();
void render();

#endif