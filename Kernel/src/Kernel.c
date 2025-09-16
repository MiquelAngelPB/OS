extern unsigned char readKey();
extern void halt();

#define LINE_MAX_CHAR 100
#define TOKEN_MAX_CHAR 11 //only 10 are used, the last one is a null character
#define MAX_TOKENS 10

//console variables
volatile char* buffer = (volatile char*)0xb8000;
int color = 8;
char colors[10] = {
    0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06 //Source: https://wiki.osdev.org/Text_UI
};
int col = 0;
int row = 0;
const int ROWS = 25;
const int COLUMNS = 80;

char actualLine[LINE_MAX_CHAR] = {'\0'};
int linePos = 0;
char tokens[MAX_TOKENS][TOKEN_MAX_CHAR] = {'\0'};
int commandMode = 1;

char scancodeToASCII[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-', 0,'\b',
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

//methods
void clear();
void print(char* str, int commandMode);
void putchar(char c, int commandMode);
void readLine(char* str);
void draw(char* target);
void executeCommand();
int tokenizeCommand(char* line);
int cmpstr(char* str, char* str2);

extern void kernelmain(void)
{
    clear();
    char* string = "Welcome to the kernel! \n";
    char* string2 = "Type help for a list of commands!\n\n";
    
    color = 6;
    print(title, 0);

    color = 8;
    print(string, 0);
    print(string2, 0);

    int IsKeyPressed = 0;
    while(1)
    {
        char scancode;
        scancode = readKey();

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
                putchar(ascii, commandMode);
            }
        }
    }

    while(1); //stop
}

void clear()
{
    col = 0;
    row = 0;

    for(int c = 0; c < COLUMNS; c++)
    {
        for(int r = 0; r < ROWS; r++)
        {
            int i = (COLUMNS * r + c) * 2;
            buffer[i] = '\0';
        }
    }
}

void print(char* str, int commandMode)
{
    while (*str)
    {
        putchar(*str, commandMode);
        str++;
    }
}

void putchar(char c, int commandMode)
{
    if (row >= ROWS)
    {
        clear();
    }

    if (col >= COLUMNS)
    {
        col = 0;
        row++;
    }

    int i = (COLUMNS * row + col) * 2;
    switch (c)
    {
        case '\n':
            col = 0;
            row++;

            if (commandMode == 1)
            {
                linePos = 0;
                for (int t = 0; t < MAX_TOKENS; t++)
                {
                    for (int c = 0; c < TOKEN_MAX_CHAR; c++)
                    {
                        tokens[t][c] = '\0';
                    }
                }
                
                readLine(actualLine);
                for (int i = 0; i < LINE_MAX_CHAR; i++)
                {
                    actualLine[i] = '\0';
                }
                
            }

            break;

        case '\b':
            if (col != 0)
            {
                buffer[i - 2] = ' ';
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
            buffer[i] = c;
            buffer[i + 1] = colors[color];
            col++;

            if (commandMode)
            {
                if (linePos < LINE_MAX_CHAR)
                {
                    actualLine[linePos] = c;
                    linePos++;
                }
                else
                {
                    char* errmsg = "That command is too long!\n\0";
                    print(errmsg, 0);
                }
            }

            break;
    }
}

void readLine(char* line)
{
    int error = tokenizeCommand(line);
    if (error == 0)
    {
        executeCommand();
    }
}

void executeCommand()
{
    char* command = &tokens[0][0];
    char* arg1 = &tokens[1][0];

    if (cmpstr(command, "clear\0"))
    {
        clear();
    } 
    else if (cmpstr(command, "halt\0"))
    {
        halt();
    } 
    else if (cmpstr(command, "text\0"))
    {
        commandMode = 0;
        char* msg = "Text mode activated, now you're trapped \n(haven't implemented a way to exit this mode yet)\n"
                    "Press TAB to change color.\n\n\0";
        print(msg, 0);
    } 
    else if (cmpstr(command, "draw\0"))
    {
        draw(arg1);
    } 
    else if (cmpstr(command, "help\0"))
    {
        char* msg = "These are the commands: \n"
        "> clear\n"
        "> halt\n"
        "> text\n"
        "> help\n"
        "> draw\n";
        print(msg, 0);
    } 
    else 
    {
        char* errmsg = "That command doesnt exist! Type help for a list of commands.\n\0";
        print(errmsg, 0);
    }
    
    putchar('\n', 0);
}

void draw(char* target)
{
    int tmpColor = color;

    if (cmpstr(target, "logo\0"))
    {
        clear();
        color = 6;
        char* drawing = " \n"
        " #####  #####  \n"
        " #   #  #      \n"
        " #   #  #####  \n"
        " #   #      #  \n"
        " #####  #####  \n"
        "               \n\n";
        print(drawing, 0);
    }
    else if (cmpstr(target, "templeos\0"))
    {
        clear();
        color = 1;
        char* drawing = "             \n"       
        "                       A     \n"
        "                     AAB     \n"
        "                    AAA      \n"
        "                   AA        \n"
        "         AAAAAAAAAAAA        \n"
        "      AAAAAAAAAAAAAAAAA      \n"
        "      AA       AAA   AA      \n"
        "      AAA     AAA   AAA      \n"
        "      AAA    AAA    AAAA     \n"
        "            AAA              \n"
        "          AAAA               \n"
        "         AAAA                \n"
        "      AAAAAA                 \n"
        "       AAAAA                 \n"
        "      AAA                    \n"
        "     AAA                     \n"
        "                             \n"
        "I was chosen by God because I am the best programmer on the planet\n and God boosted my IQ with divine intellect.\n -Terry A. Davis";
                                          
        print(drawing, 0);
    }
    else if (cmpstr(target, "linux\0"))
    {
        clear();
        color = 0;
        char* drawing = "\n"      
        "          nnnn_\n"
        "        dGGGGMMb\n"
        "       @p~qp~~qMb\n"
        "       M|@||@) M|\n"
        "       @,----.JM|\n"
        "      JS^|__/  qKL\n"
        "     dZP        qKRb\n"
        "    dZP          qKKb\n"
        "   fZP            SMMb\n"
        "   HZM            MMMM\n"
        "   FqM            MMMM\n"
        " __| .         || SoqML\n"
        " |    `.       | `' | q\n"
        "_)      |.___.,|     .`\n"
        "|____   )MMMMMP|   .`\n"
        "     `-·       `--· \n";
        print(drawing, 0);
    }
    else if (cmpstr(target, "help\0"))
    {
        char* msg = "You can draw the following: \n"
        "> draw logo\n"
        "> draw templeos\n"
        "> draw linux\n"
        "> draw help\n";
        print(msg, 0);
    }
    else
    {
        char* errmsg = "That's not a valid argument, type draw help for a list of drawings.\n\0";
        print(errmsg, 0);
    }

    color = tmpColor;
}

int tokenizeCommand(char* line)
{
    int actualToken = 0;
    int charInToken = 0;
    
    for (int i = 0; i < LINE_MAX_CHAR; i++)
    {
        char c = line[i];
        switch (c)
        {
            case ' ':
                if (actualToken < MAX_TOKENS)
                {
                    actualToken++;
                    charInToken = 0;
                    break;
                }
                else
                {
                    char* errmsg = "There cannot be more than 10 commands/arguments in 1 instruction.\n\0";
                    return 1;
                }

            case '\0':
                return 0;
        
            default:
                if (actualToken >= MAX_TOKENS)
                {
                    char* errmsg = "There cannot be more than 10 commands/arguments in 1 instruction.\n\0";
                    return 1;
                }
                

                if (charInToken < TOKEN_MAX_CHAR - 1)
                {
                    tokens[actualToken][charInToken] = c;
                    charInToken++;
                    break;
                }
                else
                {
                    char* errmsg = "All commands/arguments must be less than 10 characters long.\n\0";
                    return 1;
                }
        }
    }
    return 0;
}