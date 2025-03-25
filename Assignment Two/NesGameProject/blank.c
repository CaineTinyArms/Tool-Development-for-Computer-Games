#include "LIB/neslib.h"
#include "LIB/nesdoug.h" 
#include "headers/sprite.h"
#include "headers/colours.h"
#include "headers/testlevel.h"
#include "headers/testlevelcollision.h"

#pragma bss-name(push, "ZEROPAGE")

// Global Variables
// -====================================-
unsigned char pad1; 
unsigned char orangePortalCollision; 
unsigned char bluePortalCollision;
unsigned char lastPortalUsed = 0; // 0 for none, 1 for orange, 2 for blue.
unsigned char bulletActive = 0; // 0 for none, 1 for orange, 2 for blue.



// Function Prototypes.
// -================================-
void movement(void);
void portalPlayerCollision(void);
unsigned char wallDetection(unsigned char x, unsigned char y);
unsigned char playerWallCollision(struct spriteData *spr);
void spawnOrangeBullet(void);
void spawnBlueBullet(void);
void updateBullet(void);
void drawBullet(void);


void main (void) {
	
	ppu_off(); // Turns the screen off.

	pal_bg(paletteBackground); //	Sets the Background Palette.
    pal_spr(paletteSprite); // Sets the Sprite Palette.
	
	set_scroll_y(0xff); // Moves the background down by 1 pixel.

	vram_adr(NAMETABLE_A); // Sets the address to NameTable A (the first name table), for the data to be written to.
	vram_write(testlevel, sizeof(testlevel)); // Writes the data from the testlevel header 

    bank_spr(1); // Tells the program to use the second batch of tiles from the bank for the sprite. Both background and sprite uses 0 by default, however Alpha3 has the sprite tiles on 2.
	
	ppu_on_all(); // Turns on the Screen.
	
	
	while (1){

        ppu_wait_nmi();
        pad1 = pad_poll(0);  // read the first controller
		if (pad1 & PAD_A) // If the A button is pressed.
		{
			if(!bulletActive) // If there is no bullets on the screen.
			{
				spawnOrangeBullet(); // Spawn the orange bullet.
			} 
		}
		if (pad1 & PAD_B) // If the B button is pressed.
		{
			if(!bulletActive) // If there is no bullets on the screen.
			{
				spawnBlueBullet(); // Spawn the blue bullet.
			}
		}
        
        movement(); // Handle Player Movement.
		updateBullet(); // Moves Bullet.
        portalPlayerCollision(); // Handle Portal Collision with the Player.
		oam_clear(); // Clears the OAM buffer.
		drawSprite(); // Draws the player sprite. 
		drawBullet(); // Draws the bullet sprites.
		drawBluePortalSprite();
		drawOrangePortalSprite();
	}
}
	
void movement(void)
{
    if(pad1 & PAD_LEFT){ // If Left on the DPAD is pressed.
		testSpriteData.X -= 1; // Moves the sprite to the left.
		if (playerWallCollision(&testSpriteData)) // If the new position is colliding with a wall.
		{
			testSpriteData.X +=1; // Re-moves the sprite to the right.
		}
	}
	else if (pad1 & PAD_RIGHT){ // If Right on the DPAD is pressed.
		testSpriteData.X += 1; // Moves the sprite to the right.
		if (playerWallCollision(&testSpriteData)) // If the new position is colliding with a wall.
		{
			testSpriteData.X -=1; // Removes the sprite to the left.
		}
	}
}

void portalPlayerCollision(void)
{
	orangePortalCollision = check_collision(&testSpriteData, &orangePortalSpriteData); // Checks if the player is colliding with the data for the first portal.
	bluePortalCollision = check_collision(&testSpriteData, &bluePortalSpriteData); // Checks if the player is colliding with the data for the second portal.

	if (orangePortalCollision && lastPortalUsed != 1) // If the player is colliding with the first portal, and the previously used portal isn't the first one.
	{ 
		testSpriteData.X = bluePortalSpriteData.X; // Sets the player X data to the X location of the second portal.
		testSpriteData.Y = bluePortalSpriteData.Y; // Sets the player Y data to the Y location of the second portal.
		lastPortalUsed = 2; // Sets the last used portal as the first portal, or arrived at portal two.
	}
	else if (bluePortalCollision && lastPortalUsed != 2) // If the player is colliding with the second portal, and the previously used portal isn't the second one.
	{
		testSpriteData.X = orangePortalSpriteData.X; // Sets the player X data to the X location of the first portal. 
		testSpriteData.Y = orangePortalSpriteData.Y; // Sets the player Y data to the Y location of the first portal.
		lastPortalUsed = 1; // Sets the last used portal as the second portal, or arrived at portal one.
	}
	else
	{
		if (!orangePortalCollision && !bluePortalCollision) // If the player is not touching any portals.
		{
			lastPortalUsed = 0; // Sets the last used portal as 0, also means the player can now use any portal.
		}
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

void spawnOrangeBullet(void)
{
	orangeBulletSpriteData.X = testSpriteData.X; // Sets the orange bullet X data to the Players X data.
	orangeBulletSpriteData.Y = testSpriteData.Y; // Sets the orange bullet Y data to the Players Y data.
	bulletActive = 1; // Sets the bullet active to 1, to indicate it is a orange bullet.
}

void spawnBlueBullet(void)
{
	blueBulletSpriteData.X = testSpriteData.X; // Sets the blue bullet X data to the Players X data.
	blueBulletSpriteData.Y = testSpriteData.Y; // Sets the blue bullet Y data to the Players Y data.
	bulletActive = 2; // Sets the bullet active to 2, to indicate it's a blue bullet.
}

void updateBullet(void)
{
    if (bulletActive == 0) // If there is no bullet on the screen.
        return; // Do nothing.

    if (bulletActive == 1) // If there is an orange bullet on the screen.
    {
        orangeBulletSpriteData.X++; // Move the bullet across the screen.

        if (orangeBulletSpriteData.X > 250) // IF the bullet goes too far off the screen.
        {
            bulletActive = 0; // Remove the bullet from being active.
            return; 
        }

        if (playerWallCollision(&orangeBulletSpriteData)) // If the orange bullet hits a wall.
        {

            unsigned char tileX = (orangeBulletSpriteData.X + 4) >> 3; // Get the X tile from the centre of the bullet.
            unsigned char tileY = (orangeBulletSpriteData.Y + 4) >> 3; // Get the Y tile from the centre of the billet.

            if (tileX > 0) 
			{ 
				tileX--; // Moves the tile one to the left, to account for the portal being 16 x 16.
			} 

            if (bluePortalActive) // If there is a blue portal active.
            {
                unsigned char pTileX = bluePortalSpriteData.X >> 3; // Get the X tile from the blue portal.
                unsigned char pTileY = bluePortalSpriteData.Y >> 3; // Get the Y tile from the blue portal.
                
                if (pTileX == tileX && pTileY == tileY) // If the tiles are the same for both portals.
                {
                    bulletActive = 0; // Remove the bullet and do nothing.
                    return;
                }
            }

            orangePortalSpriteData.X = tileX << 3; // Sets the orange portal X data to Tile X, if there is no portal there already.
            orangePortalSpriteData.Y = tileY << 3; // Sets the orange portal Y data to Tile Y, if there is no portal there already.
            orangePortalActive = 1; // Sets the orange portal as active.
            bulletActive = 0; // Removes the bullet, allowing for more shots.
        }
    }
   

    else if (bulletActive == 2) // If the bullet is a blue bullet.
    {
        blueBulletSpriteData.X++; // Move the blue bullet sprite across the screen.

        if (blueBulletSpriteData.X > 240) // If the bullet goes too far off the screen.
        { 
            bulletActive = 0; // Remove the bullet.
            return;
        }

        if (playerWallCollision(&blueBulletSpriteData)) // If the blue bullet hits a wall.
        {
            unsigned char tileX = (blueBulletSpriteData.X + 4) >> 3; // Get the X tile from the centre of the bullet.
            unsigned char tileY = (blueBulletSpriteData.Y + 4) >> 3; // Get the Y tile from the centre of the bullet.
            if (tileX > 0)
			{
				tileX --; // Moves the tile one to the left, to account for the portal being 16 x 16.
			}

            if (orangePortalActive) // If there is an orange portal active.
            {
                unsigned char pTileX = orangePortalSpriteData.X >> 3; // Get the X tile from the Orange Portal.
                unsigned char pTileY = orangePortalSpriteData.Y >> 3; // Get the Y tile from the Orange Portal.

                if (pTileX == tileX && pTileY == tileY) // IF the tiles are the same for both portals.
                {
                    bulletActive = 0; // Remove the bullet and do nothing.
                    return;
                }
            }

            bluePortalSpriteData.X = tileX << 3; // Sets the blue portal X data to Tile X, if there is no portal there already.
            bluePortalSpriteData.Y = tileY << 3; // Sets the blue portal Y data to Tile Y, if there is no portal there already.
            bluePortalActive = 1; // Sets the blue portal to active. 

            bulletActive = 0; // Removes the bullet, allowing for more shots.
        }
    }
}


void drawBullet(void)
{
	if (bulletActive == 1) // If the bullet active is orange.
	{
		oam_meta_spr(orangeBulletSpriteData.X, orangeBulletSpriteData.Y, orangeBulletSprite); // Draw the orange sprite.
	}

	else if (bulletActive == 2) // If the bullet active is blue.
	{
		oam_meta_spr(blueBulletSpriteData.X, blueBulletSpriteData.Y, blueBulletSprite); // Draw the blue sprite.
	}
}