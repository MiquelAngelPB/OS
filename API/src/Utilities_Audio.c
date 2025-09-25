#include "API.h"

// Puerto del temporizador PIT
#define PIT_FREQ 1193180
#define PIT_CMD_PORT 0x43
#define PIT_CH2_PORT 0x42

// Puerto del PC Speaker
#define PC_SPEAKER_PORT 0x61

// Escribir en puerto (inline ASM)
static inline void outPort(int port, char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Leer de puerto
static inline char inb(int port) {
    char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Configura PIT y activa speaker con frecuencia dada (en Hz)
void pc_speaker_play(long freq) {
    if (freq == 0) return;

    long divisor = PIT_FREQ / freq;

    // Configurar PIT en modo 3 (square wave) en canal 2
    outb(PIT_CMD_PORT, 0xB6);
    outb(PIT_CH2_PORT, (char)(divisor & 0xFF));         // LSB
    outb(PIT_CH2_PORT, (char)((divisor >> 8) & 0xFF));  // MSB

    // Activar PC Speaker (bits 0 y 1 en 0x61)
    char tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 3);
}

// Apagar sonido
void pc_speaker_stop() {
    char tmp = inb(PC_SPEAKER_PORT) & 0xFC;
    outb(PC_SPEAKER_PORT, tmp);
}

// Delay simple (no preciso, pero suficiente)
void delay(long ms) {
    for (volatile long i = 0; i < ms * 1000; ++i) {
        asm volatile ("nop");
    }
}

// Ejemplo: reproducir una nota por 1 segundo
void pc_speaker_demo() {
    pc_speaker_play(440);  // Nota A4
    delay(1000);           // Esperar 1s
    pc_speaker_stop();
}