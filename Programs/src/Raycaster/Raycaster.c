#include "Console.h"
#include "Graphics.h"
#include "IO.h"
#include "Math.h"

#define MAP_SIZE 8
#define MAP_OFFSET_X 15
#define MAP_OFFSET_Y 16 + 15
#define SCREEN_OFFSET_X 200
#define SCREEN_OFFSET_Y 16 + 15
#define N_RAYS 64
#define VERTICALITY 10

//variables
int pixelsPerBlock = 10;

int quit = 0;
int IsKeyPressed = 1;

float px = 4;
float py = 3;
float pdx = 0;
float pdy = 0;
int pa = 0;

//debugging
float debugX = 0;
float debugY = 0;

const short map[MAP_SIZE * MAP_SIZE] = { 
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,1,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,0,0,1,1,1,
    1,0,1,1,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1
};

//methods
void drawPlayer();
void drawPlayerExtras();
void drawMap();
void manageInput();
void castRay(int a);
void drawColumn(int offset, int height, char color);
void clearGameScreen();
void drawScreen();

void drawWindow()
{
    clearConsole();
    drawRectangle(0, 0, SCREEN_WIDTH, 16, 0x09);

    setBGColor(0x09);
    setCursorVisibility(0);
    print("Raycaster                Press q to quit\0");
}

void raycasterMain()
{
    drawWindow();

    //initialize player direction
    pa = norm360(pa);
    pdx = cos(pa);
    pdy = sin(pa);

    //main loop
    while (!quit)
    {
        manageInput();
        int RaysPerSide = N_RAYS / 2;

        if (IsKeyPressed)
        {
            clearGameScreen();
            drawScreen();

            for (int a = -RaysPerSide; a < RaysPerSide; a++)
            {
                castRay(norm360(pa + a));
            }

            drawMap();
            drawPlayer();
            drawPlayerExtras();
        }

        for (int i = 0; i < 1000000; i++) { }
        render();
    }

    clearConsole();
}

void drawMap()
{
    int offsetX = MAP_OFFSET_X;
    int offsetY = MAP_OFFSET_Y;

    for (int x = 0; x < MAP_SIZE; x++)
    {
        for (int y = 0; y < MAP_SIZE; y++)
        {
            int i = y * MAP_SIZE + x;
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
    backBuffer[ (int)(py * pixelsPerBlock + MAP_OFFSET_Y) * SCREEN_WIDTH + (int)(px * pixelsPerBlock + MAP_OFFSET_X) ] = 0x0E;
}

void drawPlayerExtras()
{
    backBuffer[(int)((py * pixelsPerBlock) + pdy * 5 + MAP_OFFSET_Y) * SCREEN_WIDTH + (int)((px * pixelsPerBlock) + pdx * 5 + MAP_OFFSET_X) ] = 0x0F;
    //backBuffer[ (int)(debugY * pixelsPerBlock + MAP_OFFSET_Y) * SCREEN_WIDTH + (int)(debugX * pixelsPerBlock + MAP_OFFSET_X) ] = 0x0A;
}

void drawScreen()
{
    int OffsetFromCenter = N_RAYS / 2;
    drawRectangle(SCREEN_OFFSET_X - OffsetFromCenter * 3, SCREEN_OFFSET_Y + 50, N_RAYS * 3, 50, 0x14);
    drawRectangle(SCREEN_OFFSET_X - OffsetFromCenter * 3, SCREEN_OFFSET_Y, N_RAYS * 3, 50, 0x08);
}

void manageInput()
{
    char k;
    k = readKeyASCII();
    
    if (readKey() & 0x80)
    {
        IsKeyPressed = 0;
    }

    if (k == 'w')
    {
        px += pdx * 0.01;
        py += pdy * 0.01;
        IsKeyPressed = 1;
    }
    if (k == 's')
    {
        px -= pdx * 0.01;
        py -= pdy * 0.01;
        IsKeyPressed = 1;
    }
    if (k == 'a')
    {
        pa = norm360(pa - 1);
        pdx = cos(pa);
        pdy = sin(pa);
        IsKeyPressed = 1;
    }
    if (k == 'd')
    {
        pa = norm360(pa + 1);
        pa %= 360;
        pdx = cos(pa);
        pdy = sin(pa);
        IsKeyPressed = 1;
    }
    if (k == 'q')
    {
        setCursorVisibility(1);
        quit = 1;
    }
}

void castRay(int a)
{
    float rayAngle;
    float dx, dy;
    float hitX, hitY;
    float t;
    int wallSide = 0; //0 = vertical, 1 = horizontal

    //calculate ray direction and signs

    dx = cos(a);
    dy = sin(a);

    float signX = dx < 0 ? -1 : 1;
    float signY = dy < 0 ? -1 : 1;

    int mapX = (int)px;
    int mapY = (int)py;
    float fx = frac(px);
    float fy = frac(py);

    float distX0, distY0;

    //first cell
    if (signX == 1)
    {
        distX0 = (1 - fx);
    }
    else
    {
        distX0 = fx;
    }

    if (signY == 1)
    {
        distY0 = (1 - fy);
    }
    else
    {
        distY0 = fy;
    }

    float tx = distX0 / abs(dx);
    float ty = distY0 / abs(dy);
    
    //main loop
    if (signX == 1)
    {
        distX0 = (1 - fx);
    }
    else
    {
        distX0 = fx;
    }

    while (!map[mapY * MAP_SIZE + mapX])
    {
        //Vertical lines: x(t) = px + dx*t and y = exactly the vertical line
        //or
        //Horizontal lines: y(t) = py + dy*t and x = exactly the horizontal line

        if (tx < ty)
        {
            t = tx;
            wallSide = 1;

            hitX = mapX + (signX == 1 ? 1 : 0);
            hitY = py + dy * tx;

            mapX += (int)signX;
            tx += 1 / abs(dx);
        }
        else
        {
            t = ty;
            wallSide = 0;

            hitX = px + dx * ty;
            hitY = mapY + (signY == 1 ? 1 : 0);

            mapY += (int)signY;
            ty += 1 / abs(dy);
        }
    }

    //float tFixed = t * cos(a - pa); //remove fisheye effect
    drawColumn((a - pa) * 3, 100 - (t * VERTICALITY), wallSide ? 0x04 : 0x0C);
}   

void drawColumn(int offset, int height, char color)
{
    drawRectangle(SCREEN_OFFSET_X + offset, SCREEN_OFFSET_Y + 50 - height / 2, 3, height, color);
}

void clearGameScreen()
{
    drawRectangle(0, 16, SCREEN_WIDTH, SCREEN_HEIGHT - 16, 0x00);
}