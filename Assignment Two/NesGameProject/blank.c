/*	simple template, for cc65, for NES
 *  writing to the screen with rendering disabled
 *	using nesdoug
 *	Doug Fraker 2018
 *  and NESDoug's version of neslib
 *  Modded by Dr Mike Reddy 2024
 */	
 
 
 
#include "LIB/neslib.h"
#include "LIB/nesdoug.h" 

#define BLACK 0x27
#define DK_GY 0x00
#define LT_GY 0x10
#define WHITE 0x30
// there's some oddities in the palette code, black must be 0x0f, white must be 0x30
 
 
 
#pragma bss-name(push, "ZEROPAGE")

// GLOBAL VARIABLES
// all variables should be global for speed
// zeropage global is even faster

unsigned char i;
unsigned char pad1;


const unsigned char text[]="Sigma Boy"; // zero terminated c string

const unsigned char palette[]={
BLACK, DK_GY, LT_GY, WHITE,
0x0A,0x0E,0x15,0x1A,
0,0,0,0,
0,0,0,0
}; 


void setSubPalette0(void) {
    ppu_off();
    // Go to the first nametable's attribute table
    vram_adr(NTADR_A(0,0) + 0x03C0);
    // Fill 64 bytes with 0x00 
    //   -> sub-palette #0 for each 2x2 tile region
    vram_fill(0x00, 64);
    ppu_on_all();
}


void setSubPalette1(void) {
    ppu_off();
    // Go to the first nametable's attribute table
    vram_adr(NTADR_A(0,0) + 0x03C0);
    // Fill 64 bytes with 0x55 
    //   -> sub-palette #1 for each 2x2 tile region
    vram_fill(0x55, 64);
    ppu_on_all();
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
	
	