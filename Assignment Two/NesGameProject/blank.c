#include "LIB/neslib.h"
#include "LIB/nesdoug.h" 
#include "headers/sprite.h"
#include "headers/colours.h"
#include "headers/levelOneData.h"
#include "headers/levelOneCollision.h"
#include "headers/menu.h"
#pragma bss-name(push, "ZEROPAGE")

// GLOBAL VARIABLES.
// -====================================-
unsigned char pad1; 
unsigned char orangePortalCollision; 
unsigned char bluePortalCollision;
unsigned char lastPortalUsed = 0; // 0 for none, 1 for orange, 2 for blue.
unsigned char bulletActive = 0; // 0 for none, 1 for orange, 2 for blue.
unsigned char mode = 0; // 0 for walk mode, 1 for shoot mode.
signed char aimDirectionX = 0;
signed char aimDirectionY = 0;
signed char bulletDirectionX = 0;
signed char bulletDirectionY = 0;

signed char playerVelocity = 0;
#define GRAVITY 1
#define MAX_FALL_SPEED 4

unsigned char currentLevel;
unsigned char gameState;

// FUNCTION PROTOTYPES.
// -================================-
void modeToggle(void);
void walkMode(void);
void shootMode(void);
void portalPlayerCollision(void);
unsigned char wallDetection(unsigned char x, unsigned char y);
unsigned char playerWallCollision(struct spriteData *spr);
void spawnOrangeBullet(void);
void spawnBlueBullet(void);
void updateBullet(void);
void drawBullet(void);
unsigned char onGround(void);	
void applyGravity(void);
const unsigned char* getPlayerSprite(void);
void drawSprite(void); // Prototype for drawing sprite function.
void loadLevel(unsigned char level);
void drawMainMenu(void);


// MAIN GAME LOOP
// -=========================================-
void main (void) {
	
	ppu_off(); // Turns the screen off.

	pal_bg(paletteBackground); //	Sets the Background Palette.
    pal_spr(paletteSprite); // Sets the Sprite Palette.
	
	set_scroll_y(0xff); // Moves the background down by 1 pixel.

	gameState = 0; // 0 means main menu.
	drawMainMenu();

	bank_spr(1); // Tells the program to use the second batch of tiles from the bank for the sprite. Both background and sprite uses 0 by default, however Alpha3 has the sprite tiles on 2.
	ppu_on_all(); // Turns on the Screen.

	while (1){

        ppu_wait_nmi();
        pad1 = pad_poll(0);  // read the first controller

		if (gameState == 0)
		{
			if (pad1 & PAD_START) 
			{
                currentLevel = 0; 
                loadLevel(currentLevel);
                gameState = 1;
            }
		}

		else if (gameState == 1)
		{
			modeToggle();

			if (mode == 0)
			{
				walkMode();
			}
			else if (mode == 1)
			{
				shootMode();
			}

			applyGravity();

			updateBullet(); // Moves Bullet.
       		portalPlayerCollision(); // Handle Portal Collision with the Player. 

			oam_clear(); // Clears the OAM buffer.
			drawSprite(); // Draws the player sprite. 
			drawBullet(); // Draws the bullet sprites.
			drawBluePortalSprite();
			drawOrangePortalSprite();
		}	
	}
}
	
// COLLISION FUNCTIONS.
// -===========================================-
void portalPlayerCollision(void)
{
	orangePortalCollision = check_collision(&playerSpriteData, &orangePortalSpriteData); // Checks if the player is colliding with the data for the first portal.
	bluePortalCollision = check_collision(&playerSpriteData, &bluePortalSpriteData); // Checks if the player is colliding with the data for the second portal.

	if (bluePortalActive == 0 || orangePortalActive == 0)
	{
		return;
	}

	if (orangePortalCollision && lastPortalUsed != 1) // If the player is colliding with the first portal, and the previously used portal isn't the first one.
	{ 
		playerSpriteData.X = bluePortalSpriteData.X; // Sets the player X data to the X location of the second portal.
		playerSpriteData.Y = bluePortalSpriteData.Y; // Sets the player Y data to the Y location of the second portal.
		lastPortalUsed = 2; // Sets the last used portal as the first portal, or arrived at portal two.
	}
	else if (bluePortalCollision && lastPortalUsed != 2) // If the player is colliding with the second portal, and the previously used portal isn't the second one.
	{
		playerSpriteData.X = orangePortalSpriteData.X; // Sets the player X data to the X location of the first portal. 
		playerSpriteData.Y = orangePortalSpriteData.Y; // Sets the player Y data to the Y location of the first portal.
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

	switch (currentLevel)
	{
		case 0:
			return levelOneCollision[y * 32 + x];
	}
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



// PORTAL BULLET FUNCTIONS 
// -===============================================================-
void spawnOrangeBullet(void)
{
	orangeBulletSpriteData.X = playerSpriteData.X; // Sets the orange bullet X data to the Players X data.
	orangeBulletSpriteData.Y = playerSpriteData.Y; // Sets the orange bullet Y data to the Players Y data.
	bulletActive = 1; // Sets the bullet active to 1, to indicate it is a orange bullet.

	bulletDirectionX = aimDirectionX; // Takes the X bullet direction from the X aiming direction.
	bulletDirectionY = aimDirectionY; // Takes the Y bullet direction from the Y aiming direction.
	if (bulletDirectionX == 0 && bulletDirectionY == 0) // If the user hasn't pressed any aiming buttons.
	{
		bulletDirectionY = -1; // Default to shooting up.
	}

}

void spawnBlueBullet(void)
{
	blueBulletSpriteData.X = playerSpriteData.X; // Sets the blue bullet X data to the Players X data.
	blueBulletSpriteData.Y = playerSpriteData.Y; // Sets the blue bullet Y data to the Players Y data.
	bulletActive = 2; // Sets the bullet active to 2, to indicate it's a blue bullet.

	bulletDirectionX = aimDirectionX; // Takes the X bullet direction from the X aiming direction.
	bulletDirectionY = aimDirectionY; // Takes the Y bullet direction from the Y aiming direction.
	if (bulletDirectionX == 0 && bulletDirectionY == 0) // If the user hasn't pressed any aiming buttons.
	{
		bulletDirectionY = -1; // Default to shooting up.
	}
}

void updateBullet(void)
{
    if (bulletActive == 0) // If there is no bullet on the screen.
        return; // Do nothing.

    if (bulletActive == 1) // If there is an orange bullet on the screen.
    {
        orangeBulletSpriteData.X += bulletDirectionX; // Move the bullet across the screen.
		orangeBulletSpriteData.Y += bulletDirectionY;

        if (orangeBulletSpriteData.X > 250) // IF the bullet goes too far off the screen.
        {
            bulletActive = 0; // Remove the bullet from being active.
            return; 
        }

        if (playerWallCollision(&orangeBulletSpriteData)) // If the orange bullet hits a wall.
        {

            unsigned char tileX = (orangeBulletSpriteData.X + 4) >> 3; // Get the X tile from the centre of the bullet.
            unsigned char tileY = (orangeBulletSpriteData.Y + 4) >> 3; // Get the Y tile from the centre of the billet.

			if (bulletDirectionX > 0 && tileX > 0) 
			{
   				tileX--;
			}


			if (bulletDirectionY > 0 && tileY > 0) 
			{
   				tileY--;
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
        blueBulletSpriteData.X += bulletDirectionX; // Move the blue bullet sprite across the screen, in the direction of the X aiming variable, I.E, -1 to go left, +1 to go right.
		blueBulletSpriteData.Y += bulletDirectionY; // Move the blue bullet sprite across the screen, in the direction of the Y aiming variable, I.E, -1 to go up, +1 to go down.

        if (blueBulletSpriteData.X > 240) // If the bullet goes too far off the screen.
        { 
            bulletActive = 0; // Remove the bullet.
            return;
        }

        if (playerWallCollision(&blueBulletSpriteData)) // If the blue bullet hits a wall.
        {
            unsigned char tileX = (blueBulletSpriteData.X + 4) >> 3; // Get the X tile from the centre of the bullet.
            unsigned char tileY = (blueBulletSpriteData.Y + 4) >> 3; // Get the Y tile from the centre of the bullet.

			if (bulletDirectionX > 0 && tileX > 0) 
			{
   				tileX--;
			}


			if (bulletDirectionY > 0 && tileY > 0) 
			{
   				tileY--;
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



// WALKING AND SHOOTING FUNCTIONS 
// -=============================================================-
void modeToggle(void)
{
	static unsigned char oldSelect = 0; // Stores the old select value.

	unsigned char newSelect = (pad1 & PAD_SELECT); // Sets the new select value to if the button is pressed.

	if (newSelect && !oldSelect) // If the value is different, AKA, the button has been pressed.
	{
		mode = !mode; // Flip the mode value.
	}

	oldSelect = newSelect; // Set the oldSelect to the newSelect, as it is now the old select state.
}

void walkMode(void)
{
	if (pad1 & PAD_LEFT) // If Left on the DPAD is pressed.
	{
		playerSpriteData.X--; // Decrement the X data of the Player Sprite. 
		if (playerWallCollision(&playerSpriteData)) // If the new location of the Player Sprite is colliding with a wall.
		{
			playerSpriteData.X++; // Increment the X data to move it out of the wall.
		}
	}

	else if (pad1 & PAD_RIGHT) // If Right on the DPAD is pressed.
	{
		playerSpriteData.X++; // Increment the X data of the Player Sprite.
		if (playerWallCollision(&playerSpriteData)) // If the new location of the Player Sprite is colliding with a wall.
		{
			playerSpriteData.X--; // Decrement the X data to move it out of the wall.
		}
	}
}

void shootMode(void)
{
	signed char newDirectionX = 0; // Resets the X aiming direction.
	signed char newDirectionY = 0; // Resets the Y aiming direction.

	if (pad1 & PAD_UP) // If up is pressed on the DPAD.
	{ 
		newDirectionY = -1; // Set the aiming direction for Y to -1.
	}

	else if (pad1 & PAD_DOWN) // If down is pressed on the DPAD.
	{
		newDirectionY = 1; // Set the aiming direction for Y to 1.
	}

	if (pad1 & PAD_LEFT) // If left is pressed on the DPAD.
	{
		newDirectionX = -1; // Set the aiming direction for X to -1.
	}

	else if (pad1 & PAD_RIGHT) // If right is pressed on the DPAD.
	{
		newDirectionX = 1; // Set the aiming direction for X to 1.
	}

	if (newDirectionX != 0 || newDirectionY != 0) // If the aiming has been changed. 
	{
		aimDirectionX = newDirectionX; // Set the aiming direction X to the new direction.
		aimDirectionY = newDirectionY; // Set the aiming direction Y to the new direction.
	}

	if (!bulletActive) // If there is no bullet on the screen currently.
	{
		if (pad1 & PAD_A) // If the A button is pressed.
		{
			spawnOrangeBullet(); // Shoot an orange bullet.
		}
		else if (pad1 & PAD_B) // If the B button is pressed.
		{
			spawnBlueBullet(); // Shoot a blue bullet.
		}
	}
}


// GRAVITY FUNCTIONS 
// -============================================================================-
unsigned char onGround(void)
{
	 unsigned char footY = (playerSpriteData.Y + playerSpriteData.height + 1) >> 3;
	 unsigned char footXLeft = playerSpriteData.X >> 3;
	 unsigned char footXRight = (playerSpriteData.X + playerSpriteData.width) >> 3;

	 if (wallDetection(footXLeft, footY) || wallDetection(footXRight, footY))
	 {
		return 1;
	 }

	 return 0;
}

void applyGravity(void)
{
	if (!onGround()) // If the player is not touching the ground.
	{
        playerVelocity += GRAVITY; // Add gravity to the players velocity.
        if (playerVelocity > MAX_FALL_SPEED) // If the players max velocity is more than the max fall speed.
		{ 
            playerVelocity = MAX_FALL_SPEED; // Sets the velocity to the max fall speed.
        }
    }

	else // If the player is touching the ground.
	{
		if (playerVelocity > 0) // If the player still has velocity.
		{
			playerVelocity = 0; // Set the velocity to 0.
		}
	}

	if (playerVelocity != 0) // If the player has any velocity.
	{
		playerSpriteData.Y += playerVelocity; // Move the player down the screen at the speed of the velocity.
	}
	
	playerVelocity = 0;
}

// PLAYER SPRITE FUNCTIONS 
// -=======================================================================-

void drawSprite(void)
{
	const unsigned char* currentSprite = getPlayerSprite();
    oam_meta_spr(playerSpriteData.X, playerSpriteData.Y, currentSprite); // Draws the metasprite at x pos 64, y pos 80 and using the playerSprite data. Nes Screen is 256 x 240 in pixels, so max range for sprite drawing is 255, 239.
}

const unsigned char* getPlayerSprite(void)
{
	if (mode == 0)
	{
		return playerSprite;
	}

	if (aimDirectionX == 0 && aimDirectionY == -1)
	{
		return playerShootUpSprite;
	}

	if (aimDirectionX == 0 && aimDirectionY == 1)
	{
		return playerShootDownSprite;
	}

	if (aimDirectionX == -1 && aimDirectionY == 0)
	{
		return playerShootLeftSprite;
	}

	if (aimDirectionX == 1 && aimDirectionY == 0)
	{
		return playerShootRightSprite;
	}

	if (aimDirectionX == -1 && aimDirectionY == -1)
	{
		return playerShootTopLeftSprite;
	}

	if (aimDirectionX == 1 && aimDirectionY == -1)
	{
		return playerShootTopRightSprite;
	}

	if (aimDirectionX == -1 && aimDirectionY == 1)
	{
		return playerShootBottomLeftSprite;
	}

	if (aimDirectionX == 1 && aimDirectionY == 1)
	{
		return playerShootBottomRightSprite;
	}


	return playerShootUpSprite;
}


// LEVEL LOADING FUNCTIONS 
//-==============================================================================-
void loadLevel(unsigned char lvl)
{
	ppu_off(); // Turn the screen off.

	switch(lvl) // Switches on level passed through when function is called.
	{
		case 0: // If the level is 0, or the first level in the game.
		vram_adr(NAMETABLE_A); // Sets the VRAM address to nametable A.
		vram_write(levelOneData, 1024); // Writes all the data from levelOneData to nametable A, including the attribute table.
		break; // Breaks the switch statement.
	}	

	playerSpriteData.X = 16; // Sets the player to the bottom left tile, AKA, the starting point.
	playerSpriteData.Y = 200; // Sets the player to the bottom left tile, AKA, the starting point.
	bluePortalActive = 0; // Disables the Blue portal.
	orangePortalActive = 0; // Disables the Orange portal.
	bulletActive = 0; // Disables any bullets still on screen.
	lastPortalUsed = 0; // Allows the user to enter any portal.

	ppu_on_all(); // Turn the screen back on.
}

void drawMainMenu(void)
{
	ppu_off(); // Turns the screen off.

	vram_adr(NAMETABLE_A);
	vram_write(menu, 1024);

	ppu_on_all(); // Turns the screen back on.
}