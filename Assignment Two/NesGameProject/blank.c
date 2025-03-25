#include "LIB/neslib.h"
#include "LIB/nesdoug.h" 
#include "sprite.h"
#include "colours.h"

#pragma bss-name(push, "ZEROPAGE")

unsigned char i;
unsigned char pad1; // Variable for controller 1
unsigned char portal1Collision;
unsigned char portal2Collision;

const unsigned char text[]="CAINE'S TEST PROJECT"; // zero terminated c string

void movement(void);
void portalPlayerCollision(void);
void testFunc(void);


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
        drawSprite(); // Draw all sprites.
        movement(); // Handle Player Movement.
        portalPlayerCollision(); // Handle Portal Collision with the Player.
	}
}
	
void movement(void)
{
    if(pad1 & PAD_LEFT){ // If Left on the DPAD is pressed, remove one from the player's X data.
		testSpriteData.X -= 1;
	}
	else if (pad1 & PAD_RIGHT){ // If Right on the DPAD is pressed, add one to the player's X data.
		testSpriteData.X += 1;
	}
}

void portalPlayerCollision(void)
{
	portal1Collision = check_collision(&testSpriteData, &portal1SpriteData); // Checks if the player is colliding with the data for the first portal.
	portal2Collision = check_collision(&testSpriteData, &portal2SpriteData); // Checks if the player is colliding with the data for the second portal.

	if (portal1Collision) // If the player is colliding with the first portal.
	{ 
		testSpriteData.X = portal2SpriteData.X; // Sets the player X data to the X location of the second portal.
		testSpriteData.Y = portal2SpriteData.Y; // Sets the player Y data to the Y location of the second portal.
	}
	else if (portal2Collision) // If the player is colliding with the second portal.
	{
		//testSpriteData.X = portal1SpriteData.X; // Sets the player X data to the X location of the first portal. 
		//testSpriteData.Y = portal1SpriteData.Y; // Sets the player Y data to the Y location of the first portal.
	}
}

void testFunc(void)
{

}