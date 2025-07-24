//console
volatile char* buffer = (volatile char*)0xb8000;
int col = 0;
int row = 0;
const int ROWS = 25;
const int COLUMNS = 80;

extern void kernelmain(void)
{
    clear();
    char* string = "Welcome to the kernel! Start writing...\n";
    print(string);
    while(1); //stop
}

void clear()
{
    for(int c = 0; c < COLUMNS; c++)
    {
        for(int r = 0; r < ROWS; r++)
        {
            int i = (COLUMNS * r + c) * 2;
            buffer[i] = ' ';
        }
    }
}

void print(char* str)
{
    while (*str)
    {
        putchar(*str);
        str++;
    }
}

void putchar(char c)
{
    if (col > COLUMNS && row > ROWS)
    {
        clear();
        col = 0;
        row = 0;
    }
    
    switch (c)
    {
        case '\n':
            col = 0;
            row++;
            break;
    
        default:
            int i = (COLUMNS * row + col) * 2;
            buffer[i] = c;
            col++;
            break;
    }
}