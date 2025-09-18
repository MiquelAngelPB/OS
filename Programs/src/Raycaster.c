#include "API.h"

#define MAP_SIZE 8

//variables
int pixelsPerBlock = 10;

float px = 50;
float py = 50;
float pdx = 0;
float pdy = 0;
int pa = 0;

const short map[MAP_SIZE * MAP_SIZE] = { 
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,1,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,0,0,0,1,1,1,
    1,0,1,1,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1
};

//methods
void drawPlayer();
void drawPlayerExtras();
void drawMap();
void manageInput();

void raycasterMain()
{
    clear();
    drawRectangle(0, 0, SCREEN_WIDTH, 16, 0x09);
    showCursor = 0;
    print("Raycaster                Press q to quit\0", 0, 0x09);

    while (1)
    {
        manageInput();
        drawMap();
        drawPlayer();
        drawPlayerExtras();

        for (int i = 0; i < 1000000; i++) { }
    }
}

void drawMap()
{
    int offsetX = 15;
    int offsetY = 16 + 15;

    for (int x = 0; x < MAP_SIZE; x++)
    {
        for (int y = 0; y < MAP_SIZE; y++)
        {
            int i = x * MAP_SIZE + y;
            if (map[i])
            {
                drawRectangle(offsetX + x * pixelsPerBlock, offsetY + y * pixelsPerBlock, pixelsPerBlock, pixelsPerBlock, 0x0F);
            }
            else
            {
                drawRectangle(offsetX + x * pixelsPerBlock, offsetY + y * pixelsPerBlock, pixelsPerBlock, pixelsPerBlock, 0x00);
            }
        }
    }
}

void drawPlayer()
{
    frameBuffer[ (int)(py) * SCREEN_WIDTH + (int)(px) ] = 0x0E;
}

void drawPlayerExtras()
{
    frameBuffer[ (int)(py + pdy * 5) * SCREEN_WIDTH + (int)(px + pdx * 5) ] = 0x0F;
}

void manageInput()
{
    char k;
    k = readKeyASCII();

    if (k == 'w')
    {
        px += pdx * 0.1;
        py += pdy * 0.1;
    }
    if (k == 's')
    {
        px -= pdx * 0.1;
        py -= pdy * 0.1;
    }
    if (k == 'a')
    {
        pa = norm360(pa - 1);
        pdx = cos(pa);
        pdy = sin[pa];
    }
    if (k == 'd')
    {
        pa = norm360(pa + 1);
        pa %= 360;
        pdx = cos(pa);
        pdy = sin[pa];
    }
    if (k == 'q')
    {
        clear();
        showCursor = 1;
        consoleMain();
    }
}