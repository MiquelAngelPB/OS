//console variables
volatile char* buffer = (volatile char*)0xb8000;
int col = 0;
int row = 0;
const int ROWS = 25;
const int COLUMNS = 80;

char scancodeToASCII[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',  0, '\\',
    'z','x','c','v','b','n','m',',','.','/',  0, '*',  0, ' ',
};

char* title = " \n"
" #####  #####  \n"
" #   #  #      \n"
" #   #  #####  \n"
" #   #      #  \n"
" #####  #####  \n"
"               \n";

void clear();
void print(char* str);
void putchar(char c);

extern void kernelmain(void)
{
    clear();
    char* string = "Welcome to the kernel! Start writing...\n";
    print(title);
    print(string);

    int IsKeyPressed = 0;
    while(1)
    {
        char scancode;
        __asm__ volatile 
        (
            "inb $0x60, %0" : "=a"(scancode)
        );

        if (scancode & 0x80)
        {
            IsKeyPressed = 0;
            continue;
        }

        if (IsKeyPressed == 0)
        {
            char ascii = scancodeToASCII[scancode];
            if (ascii)
            {
                IsKeyPressed = 1;
                putchar(ascii);
            }
        }
    }

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
    if (col >= COLUMNS && row >= ROWS)
    {
        clear();
        col = 0;
        row = 0;
    }
    
    if (col >= COLUMNS)
    {
        col = 0;
        row++;
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