#include "API.h"

#define MAP_SIZE 8
#define MAP_OFFSET_X 15
#define MAP_OFFSET_Y 16 + 15
#define SCREEN_OFFSET_X 100
#define SCREEN_OFFSET_Y 16 + 15
#define N_RAYS 1

//variables
int pixelsPerBlock = 10;

int quit = 0;

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

void drawWindow()
{
    clear();
    drawRectangle(0, 0, SCREEN_WIDTH, 16, 0x09);
    showCursor = 0;
    print("Raycaster                Press q to quit\0", 0, 0x09);
}

void raycasterMain()
{
    drawWindow();

    //initialize player direction
    pa = norm360(pa);
    pdx = cos(pa);
    pdy = sin(pa);

    //main loop
    while (1)
    {
        if (quit) { clear(); break; }

        manageInput();
        castRay(norm360(pa));

        drawMap();
        drawPlayer();
        drawPlayerExtras();

        for (int i = 0; i < 1000000; i++) { }
    }
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
    frameBuffer[ (int)(py * pixelsPerBlock + MAP_OFFSET_Y) * SCREEN_WIDTH + (int)(px * pixelsPerBlock + MAP_OFFSET_X) ] = 0x0E;
}

void drawPlayerExtras()
{
    frameBuffer[ (int)((py * pixelsPerBlock) + pdy * 5 + MAP_OFFSET_Y) * SCREEN_WIDTH + (int)((px * pixelsPerBlock) + pdx * 5 + MAP_OFFSET_X) ] = 0x0F;
    frameBuffer[ (int)(debugY * pixelsPerBlock + MAP_OFFSET_Y) * SCREEN_WIDTH + (int)(debugX * pixelsPerBlock + MAP_OFFSET_X) ] = 0x0A;
}

void manageInput()
{
    char k;
    k = readKeyASCII();

    if (k == 'w')
    {
        px += pdx * 0.01;
        py += pdy * 0.01;
    }
    if (k == 's')
    {
        px -= pdx * 0.01;
        py -= pdy * 0.01;
    }
    if (k == 'a')
    {
        pa = norm360(pa - 1);
        pdx = cos(pa);
        pdy = sin(pa);
    }
    if (k == 'd')
    {
        pa = norm360(pa + 1);
        pa %= 360;
        pdx = cos(pa);
        pdy = sin(pa);
    }
    if (k == 'q')
    {
        showCursor = 1;
        quit = 1;
    }
}

void castRay(int a)
{
    float rayAngle;
    float dx, dy;
    float hitX, hitY;

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
    while (!map[mapY * MAP_SIZE + mapX])
    {
        //Vertical lines: x(t) = px + dx*t and y = exactly the vertical line
        //or
        //Horizontal lines: y(t) = py + dy*t and x = exactly the horizontal line

        if (tx < ty)
        {
            hitX = mapX + (signX == 1 ? 1 : 0);
            hitY = py + dy * tx;

            mapX += (int)signX;
            tx += distX0 / abs(dx);
        }
        else
        {
            hitX = px + dx * ty;
            hitY = mapY + (signY == 1 ? 1 : 0);

            mapY += (int)signY;
            ty += distY0 / abs(dy);
        }
    }

    debugX =  hitX;
    debugY =  hitY;
}

void drawColumn(int offset, int height, char color)
{
    drawRectangle(SCREEN_OFFSET_X + offset, SCREEN_OFFSET_Y, 3, height, color);
}