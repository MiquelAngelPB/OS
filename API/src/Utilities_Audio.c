#include "API.h"
//https://wiki.osdev.org/PC_Speaker

 void makesound();

 void play_sound(long nFrequence) {
 	long Div;
 	char tmp;
 
    //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outPort(0x43, 0xb6);
 	outPort(0x42, (char) (Div));
 	outPort(0x42, (char) (Div >> 8));
 
    //And play the sound using the PC speaker
 	tmp = inpPort(0x61);
  	if (tmp != (tmp | 3)) {
 		outPort(0x61, tmp | 3);
 	}
 }
 
 //make it shut up
 static void nosound() {
 	char tmp = inpPort(0x61) & 0xFC;
     
 	outPort(0x61, tmp);
 }
 
 //Make a beep
 void beep() {
    for (int i = 0; i < 30; i++)
    {
        makesound();
    }
 }

 void makesound()
 {
    play_sound(1000);
 	for (int i = 0; i < 100000; i++) { };
 	nosound();
 }