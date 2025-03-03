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

unsigned char i;
unsigned char pad1; // Variable for controller 1

const unsigned char text[]="CAINE'S TEST PROJECT"; // zero terminated c string

const unsigned char paletteBackground[]=
{
BLACK, DK_GY, LT_GY, WHITE, // Palette 0 
ORANGE,BLUE,PURPLE,YELLOW, // Sub-Palette 1
0,0,0,0, // Sub-Palette 2
0,0,0,0 // Sub-Palette 3
}; 

const char paletteSprite[] = 
{
BLACK, ORANGE, YELLOW, BLUE,
0,0,0,0,
0,0,0,0,
0,0,0,0
};

const unsigned char testSprite[] = 
{
    0, 0, 0x00, 0,  // Relative X, Relative Y, Tile Index, Pallete & Attributes.
    0, 8, 0x10, 0,  // For Example - Relative x is 0, Relative Y is 8, meaning place this 2nd tile 8 pixels below the tile above. 0x10 is the Tile Index for the tile, if viewing in NEXXT, it's the tile directly below the first one, then 0 at the ends means use palette 0..
    8, 0, 0x00, 0|OAM_FLIP_H, // 0|OAM_FLIP_H - means use palette 0, but flip the sprite horizontally. this is useful as it saved tiles for symetrical sprites.
    8, 8, 0x10, 0|OAM_FLIP_H,
    128 // This basically means "this is the end please stop reading sprite data".
};


void setSubPalette0(void); // Prototype for changing palette to 0 function.
void setSubPalette1(void); // Prototype for changing palette to 1 function.
void drawSprite(void); // Prototype for drawing sprite function.


void main (void) {
	
	ppu_off(); // Turns the screen off.

	pal_bg(paletteBackground); //	Sets the Background Palette.
    pal_spr(paletteSprite); // Sets the Sprite Palette.
	
    bank_spr(1); // Tells the program to use the second batch of tiles from the bank for the sprite. Both background and sprite uses 0 by default, however Alpha3 has the sprite tiles on 2.

	vram_adr(NTADR_A(0,0)); // Sets starting point for the text to 0, 0.

	i = 0; // Sets [i] to 0, so the text displays from the beginning.
	while(text[i]){ // Runs while there is still chars in the text string.
		vram_put(text[i]); // Pushes the [i] char from the text string to the screen.
		++i; // Increments [i]
	}	
	
	ppu_on_all(); // Turns on the Screen.
	
	
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
        
        drawSprite();
	}
}
	

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

void drawSprite(void)
{
    oam_clear(); // Clears all sprites from the sprite buffer.

    oam_meta_spr(16, 18, testSprite); // Draws the metasprite at x pos 16, y pos 18 and using the testSprite data. Nes Screen is 256 x 240 in pixels, so max range for sprite drawing is 255, 239.
}