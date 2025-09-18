#include "API.h"

#define LINE_MAX_CHAR 100
#define TOKEN_MAX_CHAR 11 //only 10 are used, the last one is a null character
#define MAX_TOKENS 10

//console variables
volatile char* buffer = (volatile char*)0xb8000;
int color = 8;
int showCursor = 1;
char bgcolor = 0x00;
char colors[10] = {
    0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06 //Source: https://wiki.osdev.org/Text_UI
};
int col = 0;
int row = 0;

char actualLine[LINE_MAX_CHAR] = {'\0'};
int linePos = 0;
char tokens[MAX_TOKENS][TOKEN_MAX_CHAR] = {'\0'};
int commandMode = 1;

char* errCodes[3] = {
    "Everything is fine, you shouldn't be seeing this\n\0",
    "There cannot be more than 10 commands/arguments in 1 instruction.\n\0",
    "All commands/arguments must be less than 10 characters long.\n\0",
};

//methods
void clear();
void print(char* str, int commandMode, char bgColor);
void putchar(char c, int commandMode, char bgColor);
void readLine(char* str);
void draw(char* target);
void run(char* target);
void executeCommand();
int tokenizeCommand(char* line);

void consoleMain()
{
    showCursor = 1;

    int IsKeyPressed = 0;
    while(1)
    {
        if (showCursor)
        {
            putCharAt(row, col, '_', colors[color], 0x00);
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
                putChar(ascii, commandMode, bgcolor);
                moveCursor(row, col);
            }
        }
    }

    while(1); //stop
}

void setColor(int c)
{
    color = c;
}

void clear()
{
    col = 0;
    row = 0;

    clearScreen();
}

void print(char* str, int commandMode, char bgColor)
{
    while (*str)
    {
        putChar(*str, commandMode, bgColor);
        str++;
    }
}

void putChar(char c, int commandMode,  char bgColor)
{
    if (row >= TEXT_ROWS)
    {
        clear();
    }

    if (col >= TEXT_COLS)
    {
        col = 0;
        row++;
    }

    int i = (TEXT_COLS * row + col) * 2;
    switch (c)
    {
        case '\n':
            putCharAt(row, col, '\0', 0x00, bgColor);
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
            putCharAt(row, col, c, colors[color], bgColor);
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
                    print(errmsg, 0, bgColor);
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
    else
    {
        char* errmsg = errCodes[error];
        char* initialMsg = "Error \0";
        char* initialMsg2 = ": \0";

        print(initialMsg, 0, bgcolor);
        putChar(error + '0', 0, bgcolor);
        print(initialMsg2, 0, bgcolor);
        print(errmsg, 0, bgcolor);
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
        clear();
        char* msg = "Text mode activated, now you're trapped \n(haven't implemented a way to exit this mode yet)\n"
                    "Press TAB to change color.\n\n\0";
        print(msg, 0, bgcolor);
    } 
    else if (cmpstr(command, "run\0"))
    {
        run(arg1);
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
        "> run\n"
        "> draw\n"
        "> help\n";
        print(msg, 0, bgcolor);
    } 
    else 
    {
        char* errmsg = "That command doesnt exist! Type help for a list of commands.\n\0";
        print(errmsg, 0, bgcolor);
    }
    
    putChar('\n', 0, bgcolor);
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
        print(drawing, 0, bgcolor);
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
                                          
        print(drawing, 0, bgcolor);
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
        print(drawing, 0, bgcolor);
    }
    else if (cmpstr(target, "help\0"))
    {
        char* msg = "You can draw the following: \n"
        "> draw logo\n"
        "> draw templeos\n"
        "> draw linux\n"
        "> draw help\n";
        print(msg, 0, bgcolor);
    }
    else
    {
        char* errmsg = "That's not a valid argument, type draw help for a list of drawings.\n\0";
        print(errmsg, 0, bgcolor);
    }

    color = tmpColor;
}

void run(char* target)
{
    int tmpColor = color;

    if (cmpstr(target, "raycaster\0"))
    {
        raycasterMain();
    }
    else if (cmpstr(target, "help\0"))
    {
        char* msg = "You can run the following programs: \n"
        "> run raycaster\n"
        "> run help\n";
        print(msg, 0, bgcolor);
    }
    else
    {
        char* errmsg = "That's not a valid argument, type run help for a list of programs.\n\0";
        print(errmsg, 0, bgcolor);
    }
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
                else { return 1; }

            case '\0':
                return 0;
        
            default:
                if (actualToken >= MAX_TOKENS)
                { return 1; }

                if (charInToken < TOKEN_MAX_CHAR - 1)
                {
                    tokens[actualToken][charInToken] = c;
                    charInToken++;
                    break;
                }
                else { return 2; }
        }
    }
    return 0;
}