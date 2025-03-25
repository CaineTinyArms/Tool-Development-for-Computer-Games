#include "LIB/neslib.h"
#include "LIB/nesdoug.h" 
#include "headers/sprite.h"
#include "headers/colours.h"
#include "headers/testlevel.h"
#include "headers/testlevelcollision.h"

#pragma bss-name(push, "ZEROPAGE")

unsigned char i;
unsigned char pad1; 
unsigned char portal1Collision;
unsigned char portal2Collision;

void movement(void);
void portalPlayerCollision(void);
unsigned char wallDetection(unsigned char x, unsigned char y);
unsigned char playerWallCollision(struct spriteData *spr);


void main (void) {
	
	ppu_off(); // Turns the screen off.

	pal_bg(paletteBackground); //	Sets the Background Palette.
    pal_spr(paletteSprite); // Sets the Sprite Palette.
	
	set_scroll_y(0xff);

	vram_adr(NAMETABLE_A); // Sets the address to NameTable A (the first name table), for the data to be written to.
	vram_write(testlevel, sizeof(testlevel)); // Writes the data from the testlevel header 

    bank_spr(1); // Tells the program to use the second batch of tiles from the bank for the sprite. Both background and sprite uses 0 by default, however Alpha3 has the sprite tiles on 2.
	
	ppu_on_all(); // Turns on the Screen.
	
	
	while (1){

        ppu_wait_nmi();
        pad1 = pad_poll(0);  // read the first controller
        drawSprite(); // Draw all sprites.
        movement(); // Handle Player Movement.
        portalPlayerCollision(); // Handle Portal Collision with the Player.
	}
}
	
void movement(void)
{
	unsigned char oldX = testSpriteData.X; // Stores the starting X location, in case collision is detected.
	unsigned char oldY = testSpriteData.Y; // Stores the starting Y location, in case collision is detected.

    if(pad1 & PAD_LEFT){ // If Left on the DPAD is pressed.
		testSpriteData.X -= 1; // Moves the sprite to the left.
		if (playerWallCollision(&testSpriteData))
		{
			testSpriteData.X = oldX;
		}
	}
	else if (pad1 & PAD_RIGHT){ // If Right on the DPAD is pressed, add one to the player's X data.
		testSpriteData.X += 1;
		if (playerWallCollision(&testSpriteData))
		{
			testSpriteData.X = oldX;
		}
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

unsigned char wallDetection(unsigned char x, unsigned char y)
{
    if (x >= 32 || y >= 30) // Checks if the player is out of the screen area somehow.
    {
        return 1; // Returns a 1 to represent collision.
    }
    return testlevelcollision[y * 32 + x]; // Returns the appropriate tile from the testlevelcollision.h array, where 1 represents collision and 0 represents no collision.
}

unsigned char playerWallCollision(struct spriteData *spr)
{
    unsigned char leftTile   = spr->X >> 3; // Converts the pixel location of the left side of the player into a tile location, which makes overlapping easier to detect.
    unsigned char rightTile  = (spr->X + spr->width) >> 3; // Converts the pixel location of right side of the player into a tile location, which makes overlapping easier to detect.
    unsigned char topTile    = spr->Y >> 3; // Converts the pixel location of the top half of the player into a tile location, which makes overlapping easier to detect.
    unsigned char bottomTile = (spr->Y + spr->height) >> 3; // Converts the pixel location of the bottom half of the player into a tile location, which makes overlapping easier to detect.

    if (wallDetection(leftTile, topTile)  || wallDetection(rightTile, topTile) || wallDetection(leftTile, bottomTile) || wallDetection(rightTile, bottomTile)) // This checks if any of the tiles are a wall.
    {
        return 1; // Returns a 1 to represent collision.
    }
    else
    {
        return 0; // Returns a 0 to represent no collision.
    }
}