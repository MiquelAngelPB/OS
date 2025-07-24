extern void kernelmain(void)
{
    char* video = (char*)0xb8000;
    video[0] = 'C';
    video[1] = 0x0f;
    return;
}