/*	simple template, for cc65, for NES
 *  writing to the screen with rendering disabled
 *	using nesdoug
 *	Doug Fraker 2018
 *  and NESDoug's version of neslib
 *  Modded by Dr Mike Reddy 2024
 */	
 
 
 
#include "LIB/neslib.h"
#include "LIB/nesdoug.h" 

#define BLACK 0x0F
#define DK_GY 0x00
#define LT_GY 0x10
#define WHITE 0x30
#define ORANGE 0x27
#define YELLOW 0x28
#define GREEN 0x2A
#define TEAL 0x2C
#define BROWN 0x18
#define RED 0x16
#define PURPLE 0x14
#define BLUE 0x12
#define DK_GREEN 0x09
#define DK_BLUE 0x0B

#pragma bss-name(push, "ZEROPAGE")

// GLOBAL VARIABLES
// all variables should be global for speed
// zeropage global is even faster

unsigned char i;
unsigned char pad1; // Variable for controller 1


const unsigned char text[]="Sigma Boy"; // zero terminated c string

const unsigned char palette[]={
BLACK, DK_GY, LT_GY, WHITE, // Palette 0 
ORANGE,BLUE,PURPLE,YELLOW, // Sub-Palette 1
0,0,0,0, // Sub-Palette 2
0,0,0,0 // Sub-Palette 3
}; 


void setSubPalette0(void) {
    ppu_off(); // Turns the screen off.
    vram_adr(NTADR_A(0,0) + 0x03C0); // Goes to the VRAM address for the first name table. The +0x03C0 jumps to the attribute table of the name table.
    vram_fill(0x00, 64); // Changes all 64 bytes of the attribute table, telling it to use sub-palette zero. The 0x00 sets all 2-bit pairs to 00, meaning to use sub-palette zero.
    ppu_on_all(); // Turns the screen on.
}


void setSubPalette1(void) {
    ppu_off(); // Turns the screen off.
    vram_adr(NTADR_A(0,0) + 0x03C0); // Goes to the VRAM address for the first name table. The +0x03C0 jumps to the attribute table of the name table.
    vram_fill(0x55, 64); // Changes all 64 bytes of the attribute table, telling it to use sub-palette one. The 0x55 sets all 2-bit pairs to 01, meaning to use sub-palette one.
    ppu_on_all(); // Turns the screen on.
}


	

void main (void) {
	
	ppu_off(); // screen off

	pal_bg(palette); //	load the BG palette
		
	// set a starting point on the screen
	// vram_adr(NTADR_A(x,y));
	vram_adr(NTADR_A(2,14)); // screen is 32 x 30 tiles

	i = 0;
	while(text[i]){
		vram_put(text[i]); // this pushes 1 char to the screen
		++i;
	}	
	
	// vram_adr and vram_put only work with screen off
	// NOTE, you could replace everything between i = 0; and here with...
	// vram_write(text,sizeof(text));
	// does the same thing
	

	ppu_on_all(); //	turn on screen
	
	
	while (1){

	   // Wait for NMI and read controller
        ppu_wait_nmi();
        pad1 = pad_poll(0);  // read the first controller

        // If A is pressed, switch to sub-palette #1
        if (pad1 & PAD_A) {
            setSubPalette1();
        }
        // If B is pressed, switch back to sub-palette #0
        if (pad1 & PAD_B) {
            setSubPalette0();
        }
        
	}
}
	
	