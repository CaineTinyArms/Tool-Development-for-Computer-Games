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
unsigned char pad1; // Variable for the controller.
unsigned char orangePortalCollision; // Variable for if the player is touching the orange portal.
unsigned char bluePortalCollision; // Variable for if the player is touching the blue portal.
unsigned char lastPortalUsed = 0; // 0 for none, 1 for orange, 2 for blue.
unsigned char mode = 0; // 0 for walk mode, 1 for shoot mode.
signed char aimDirectionX = 0; // Variable for the X direction the player is aiming (left and right)
signed char aimDirectionY = 0; // Variable for the Y direction the player is aiming (up and down)
signed char playerVelocity = 0; // Variable for the players velocity, used for making them fall.
#define GRAVITY 1 // Defines the speed of gravity that is added while the player is falling.
#define MAX_FALL_SPEED 4 // Defines the max fall speed of the player.
unsigned char currentLevel; // Variable to track what level the player is currently on.
unsigned char gameState; // Variable to track the current game state, 0 = main menu, 1 = in game etc.
unsigned char blankTiles[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Array of 12 blank tiles, used to simulate the flashing "PRESS START!" text.
unsigned char pressStartTiles[12] = {0x50, 0x52, 0x45, 0x53, 0x53, 0x00, 0x53, 0x54, 0x41, 0x52, 0x54, 0x21}; // Array of 12 tile addresses, used to display "PRESS START!".
unsigned char bluePortalActive = 0;
unsigned char orangePortalActive = 0;
unsigned char orangeBulletActive = 0;
unsigned char blueBulletActive = 0;
signed char orangeBulletDirectionX = 0;
signed char orangeBulletDirectionY = 0;
signed char blueBulletDirectionX = 0;
signed char blueBulletDirectionY = 0;

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
void drawSprite(void); 
void loadLevel(unsigned char level);
void drawMainMenu(void);
void animatePressStartText(void);
void drawOrangePortalSprite(void);
void drawBluePortalSprite(void);

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
			animatePressStartText();
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
	orangeBulletActive = 1; // Sets the bullet active to 1, to indicate it is a orange bullet.

	orangeBulletDirectionX = aimDirectionX; // Takes the X bullet direction from the X aiming direction.
	orangeBulletDirectionY = aimDirectionY; // Takes the Y bullet direction from the Y aiming direction.

	if (orangeBulletDirectionX == 0 && orangeBulletDirectionY == 0) // If the user hasn't pressed any aiming buttons.
	{
		orangeBulletDirectionY = -1; // Default to shooting up.
	}

}

void spawnBlueBullet(void)
{
	blueBulletSpriteData.X = playerSpriteData.X; // Sets the blue bullet X data to the Players X data.
	blueBulletSpriteData.Y = playerSpriteData.Y; // Sets the blue bullet Y data to the Players Y data.
	blueBulletActive = 1; // Sets the bullet active to 2, to indicate it's a blue bullet.

	blueBulletDirectionX = aimDirectionX; // Takes the X bullet direction from the X aiming direction.
	blueBulletDirectionY = aimDirectionY; // Takes the Y bullet direction from the Y aiming direction.
	if (blueBulletDirectionX == 0 && blueBulletDirectionY == 0) // If the user hasn't pressed any aiming buttons.
	{
		blueBulletDirectionY = -1; // Default to shooting up.
	}
}

void updateBullet(void)
{
    if (orangeBulletActive)
	{
		orangeBulletSpriteData.X += orangeBulletDirectionX;
		orangeBulletSpriteData.Y += orangeBulletDirectionY;

		if (playerWallCollision(&orangeBulletSpriteData))
		{
			unsigned char tileX = (orangeBulletSpriteData.X + 4) >> 3;
			unsigned char tileY = (orangeBulletSpriteData.Y + 4) >> 3;

			if (orangeBulletDirectionX > 0 && tileX > 0) 
			{
				tileX--;
			}	
			if (orangeBulletDirectionY > 0 && tileY > 0)
			{
				tileY--;
			}

			if (bluePortalActive)
			{
				unsigned char bluePortalTileX = bluePortalSpriteData.X >> 3;
				unsigned char bluePortalTileY = bluePortalSpriteData.Y >> 3;
				if (bluePortalTileX == tileX && bluePortalTileY == tileY)
				{
					orangeBulletActive = 0;
					return;
				}
			}

			orangePortalSpriteData.X = tileX << 3;
			orangePortalSpriteData.Y = tileY << 3;
			orangePortalActive = 1;
			orangeBulletActive = 0;
		}
	}

	if (blueBulletActive)
	{
		blueBulletSpriteData.X += blueBulletDirectionX;
		blueBulletSpriteData.Y += blueBulletDirectionY;

		

		if (playerWallCollision(&blueBulletSpriteData))
		{
			unsigned char tileX = (blueBulletSpriteData.X + 4) >> 3;
			unsigned char tileY = (blueBulletSpriteData.Y + 4) >> 3;

			if (blueBulletDirectionX > 0 && tileX > 0) 
			{
				tileX--;
			}
			if (blueBulletDirectionY > 0 && tileY > 0)
			{
				tileY--;
			}

			if (orangePortalActive)
			{
				unsigned char orangePortalTileX = orangePortalSpriteData.X >> 3;
				unsigned char orangePortalTileY = orangePortalSpriteData.Y >> 3;
				if (orangePortalTileX == tileX && orangePortalTileY == tileY)
				{
					blueBulletActive = 0;
					return;
				}
			}

			bluePortalSpriteData.X = tileX << 3;
			bluePortalSpriteData.Y = tileY << 3;
			bluePortalActive = 1;
			blueBulletActive = 0;
		}
	}
}


void drawBullet(void)
{
	if (orangeBulletActive) // If the bullet active is orange.
	{
		oam_meta_spr(orangeBulletSpriteData.X, orangeBulletSpriteData.Y, orangeBulletSprite); // Draw the orange sprite.
	}

	if (blueBulletActive) // If the bullet active is blue.
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

	if (!orangeBulletActive && (pad1 & PAD_A))
	{
		spawnOrangeBullet();
	}

	if (!blueBulletActive && (pad1 & PAD_B))
	{
		spawnBlueBullet();
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
	if (mode == 0) // If the player is in move mode.
	{
		return playerSprite; // Return the default player sprite.
	}

	if (aimDirectionX == 0 && aimDirectionY == -1) // If the player is aiming up.
	{
		return playerShootUpSprite; // Return the player aiming up sprite.
	}

	if (aimDirectionX == 0 && aimDirectionY == 1) // If the player is aiming down.
	{
		return playerShootDownSprite; // Return the player aiming down sprite.
	}

	if (aimDirectionX == -1 && aimDirectionY == 0) // If the player is aiming left.
	{
		return playerShootLeftSprite; // Return the player aiming left sprite.
	}

	if (aimDirectionX == 1 && aimDirectionY == 0) // If the player is aiming right.
	{
		return playerShootRightSprite; // Return the player aiming right sprite.
	}

	if (aimDirectionX == -1 && aimDirectionY == -1) // If the player is aiming up and left.
	{
		return playerShootTopLeftSprite; // Return the player aiming up and left sprite.
	}

	if (aimDirectionX == 1 && aimDirectionY == -1) // If the player is aiming up and right.
	{
		return playerShootTopRightSprite; // Return the player aiming up and right sprite.
	}

	if (aimDirectionX == -1 && aimDirectionY == 1) // If the player is aiming down and left.
	{
		return playerShootBottomLeftSprite; // Return the player aiming down and left sprite.
	}

	if (aimDirectionX == 1 && aimDirectionY == 1) // If the player is aiming down and right.
	{
		return playerShootBottomRightSprite; // Return the player aiming down and right sprite.
	}


	return playerShootUpSprite; // If the player isn't aiming anywhere, return the aiming up sprite as that's where the bullets go by default.
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
		playerSpriteData.X = 136; // Sets the player to the bottom left tile, AKA, the starting point.
		playerSpriteData.Y = 216; // Sets the player to the bottom left tile, AKA, the starting point.
		break; // Breaks the switch statement.
	}	

	bluePortalActive = 0; // Disables the Blue portal.
	orangePortalActive = 0; // Disables the Orange portal.
	blueBulletActive = 0; // Disables any bullets still on screen.
	orangeBulletActive = 0;
	lastPortalUsed = 0; // Allows the user to enter any portal.

	ppu_on_all(); // Turn the screen back on.
}

void drawMainMenu(void)
{
	ppu_off(); // Turns the screen off.

	vram_adr(NAMETABLE_A); // Sets the VRAM address to the location of NAMETABLE_A.
	vram_write(menu, 1024); // Writes all the data from the menu array to NAMETABLE_A, including the attribute table.

	ppu_on_all(); // Turns the screen back on.
}

void animatePressStartText(void)
{
	static unsigned char flashTimer = 0; 
	static unsigned char showPressStartText = 1;

	flashTimer++;

	if (flashTimer >= 30)
	{
		flashTimer = 0;

		ppu_off();

		vram_adr(NAMETABLE_A + (17 * 32) + 10);

		if (showPressStartText)
		{
			vram_write(blankTiles, 12);
			showPressStartText = 0;
		}

		else 
		{
			vram_write(pressStartTiles, 12);
			showPressStartText = 1;
		}

		ppu_on_all();
	}
}

void drawOrangePortalSprite(void)
{
    if (orangePortalActive)
    {
        oam_meta_spr(orangePortalSpriteData.X, orangePortalSpriteData.Y, orangePortal);
    }
}

void drawBluePortalSprite(void)
{
    if (bluePortalActive)
    {
        oam_meta_spr(bluePortalSpriteData.X, bluePortalSpriteData.Y, bluePortal);
    }
}
