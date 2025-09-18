#ifndef API_H
#define API_H

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define TEXT_COLS 40
#define TEXT_ROWS 25

//math.c
float degToRad(float degrees);
float cos(int a);
float tan(int a);
int norm360(int a);
extern float sin[360];

//Utilities.asm
char inpPort(short port);
void outPort(short port, char value);
void halt();

//Utilities.c
int cmpstr(char* str, char* str2);
char readKey();
char readKeyASCII();
void moveCursor(short row, short col);

//Utilities_VGA.c
extern volatile unsigned char* const frameBuffer;
void putCharAt(unsigned int row, unsigned int col, char chr, char color, char bgColor);
void drawRectangle(unsigned short x, unsigned short y, unsigned short width, unsigned short height, char color);
void clearScreen();

//Console.c
extern int showCursor;
void clear();
void print(char* str, int commandMode, char bgColor);
void putChar(char c, int commandMode, char bgColor);
void setColor(int c);

//Programs
void consoleMain();
void raycasterMain();

#endif