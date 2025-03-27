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
unsigned char doorCollision; 
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
unsigned char pressStartTiles[12] = {0x50, 0x52, 0x45, 0x53,0x53, 0x00, 0x53, 0x54,0x41, 0x52, 0x54, 0x21}; // Array of 12 tile addresses, used to display "PRESS START!".
unsigned char bluePortalActive = 0; // Tracks if the blue portal is active, 0 = no, 1 = yes.
unsigned char orangePortalActive = 0; // Tracks if the orange portal is active, 0 = no, 1 = yes.
unsigned char orangeBulletActive = 0; // Tracks if the orange bullet is active, 0 = no, 1 = yes.
unsigned char blueBulletActive   = 0; // Tracks if the blue bullet is active, 0 = no, 1 = yes.
signed char orangeBulletDirectionX = 0; // Tracks the X direction of the orange bullet, -1 for left, 1 for right.
signed char orangeBulletDirectionY = 0; // Tracks the Y direction of the orange bullet, -1 for up, 1 for down.
signed char blueBulletDirectionX   = 0; // Tracks the X direction of the blue bullet, -1 for left, 1 for right.
signed char blueBulletDirectionY   = 0; // Tracks the Y direction of the orange bullet, -1 for left, 1 for right.
unsigned char orangePortalOrientation = 0; // Tracks the orientation of the orange portal, 0 = up, 1 = right, 2 = down and 3 = left.
unsigned char bluePortalOrientation   = 0; // Tracks the orientation of the blue portal, 0 = up, 1 = right, 2 = down and 3 = left.
#define PORTAL_WIDTH  16 // Defines the width of the portal sprite.
#define PORTAL_HEIGHT 16 // Defines the height of the portal sprite.


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
unsigned char getCollisionValue(unsigned char x, unsigned char y);
void drawDoorSprite();
void doorPlayerCollision(void);

// MAIN GAME LOOP
// -=========================================- 
void main(void) {

    ppu_off(); // Turns the screen off.
	pal_bg(paletteBackground); // Sets the Background Palette.
    pal_spr(paletteSprite); // Sets the Sprite Palette.
    set_scroll_y(0xff); // Moves the background down by 1 pixel.
	gameState = 0; // 0 means main menu.
    drawMainMenu(); // Displays the main menu.
	bank_spr(1); // Tells the program that the sprites are located on the 2nd side of the chr file.
    ppu_on_all(); // Turns on the Screen.

    while(1) {
        ppu_wait_nmi();
        pad1 = pad_poll(0); // read the first controller
	    if(gameState == 0) // If the game is at the main menu.
		{
            if(pad1 & PAD_START) // Check if the player has pressed start.
			{ 
                currentLevel = 0; // Set current level to 0, meaning the first level.
                loadLevel(currentLevel); // Loads the current level.
                gameState = 1; // Sets the game state to 1, meaning in game.
            }
            animatePressStartText(); // Animates the PRESS START! text if start isn't pressed.
        }
        else if(gameState == 1) // If the game has started and is not on the menu.
		{
            modeToggle(); // Call the mode toggle function to check what mode the player is in.

            if(mode == 0) // If the user is in walk mode.
			{
                walkMode(); // Call the walk mode function.
            }
            else if(mode == 1) // If the is in shoot mode.
			{
                shootMode(); // Call the shoot mode function.
            }
			applyGravity(); // Applies gravity to the player.
			updateBullet(); // Updates bullet location and logic, if any are on the screen.
            portalPlayerCollision(); // Check if the player is colliding with any portals.
            oam_clear(); // Clear the OAM buffer/
            drawSprite(); // Draws the player sprite.
            drawBullet(); // Draws the bullet sprites.
            drawBluePortalSprite(); // Draws the blue portal sprite.
            drawOrangePortalSprite(); // Draws the orange portal sprite.
			drawDoorSprite();
        }
    }
}

// COLLISION FUNCTIONS.
// -===========================================-
void portalPlayerCollision(void)
{
    orangePortalCollision = check_collision(&playerSpriteData, &orangePortalSpriteData); // Checks if the player is colliding with the orange portal.
    bluePortalCollision   = check_collision(&playerSpriteData, &bluePortalSpriteData);   // Checks if the player is colliding with the blue portal.

    if(!bluePortalActive || !orangePortalActive) // If both portals aren't active, don't do anything.
	{
        return;
    }

    if(orangePortalCollision && lastPortalUsed != 1) // If the player is touching the orange portal, and hasn't just teleported into it. 
    {
        playerSpriteData.X = bluePortalSpriteData.X; // Set the player X data to the blue portal X data.
        playerSpriteData.Y = bluePortalSpriteData.Y; // Set the player Y data to the blue portal Y data.
        lastPortalUsed = 2;  // Set the last used portal to 2, meaning they have just arrived at the blue portal.

        if(playerWallCollision(&playerSpriteData)) // If the user is now stuck in a wall after teleporting.
		{
            switch(bluePortalOrientation) // Change based on what orientation the portal was shot from.
			{
                case 0: // If the portal was shot up
                    playerSpriteData.Y += 8; // Move the player down.
                    break;
                case 1: // If the portal was shot right.
                    playerSpriteData.X -= 8; // Move the player left.
                    break;
                case 2: // If the portal was shot down.
                    playerSpriteData.Y -= 8; // Move the player up.
                    break;
                case 3: // If the portal was shot left.
                    playerSpriteData.X += 8; // Move the player right.
                    break;
            }
        }
    }
   
    else if(bluePortalCollision && lastPortalUsed != 2) // If the player is touching the blue portal, and hasn't just teleported into it.
    {
        playerSpriteData.X = orangePortalSpriteData.X; // Sets the player X data to the orange X data.
        playerSpriteData.Y = orangePortalSpriteData.Y; // Sets the player Y data to the orange Y data.
        lastPortalUsed = 1;  // Sets the last portal used to 1, meaning they have just arrived at the orange portal.

        if(playerWallCollision(&playerSpriteData)) // If the user is now stuck in a wall after teleporting.
		{
            switch(orangePortalOrientation) // Change based on what orientation the portal was shot from.
			{
                case 0: // If the portal was shot up
                    playerSpriteData.Y += 8; // Move the player down.
                    break;
                case 1: // If the portal was shot right.
                    playerSpriteData.X -= 8; // Move the player left.
                    break;
                case 2: // If the portal was shot down.
                    playerSpriteData.Y -= 8; // Move the player up.
                    break;
                case 3: // If the portal was shot left.
                    playerSpriteData.X += 8; // Move the player right.
                    break;
            }
        }
    }
    else 
    {
        if(!orangePortalCollision && !bluePortalCollision) // If the player is not touching any portal.
		{
            lastPortalUsed = 0; // Set the last portal used to 0, meaning the player can now use any portal.
        }
    }
}

unsigned char wallDetection(unsigned char x, unsigned char y)
{
    if(x >= 32 || y >= 30) // Checks if the tile is out of the screen area
    {
        return 1; // Return a 1 to say the player is hitting a wall.
    }

    switch(currentLevel) // Changes based on the current level.
	{
        case 0:
            return levelOneCollision[y * 32 + x]; // Returns the value for the tile at X and Y of the collision table.
    }
}

unsigned char playerWallCollision(struct spriteData *spr)
{
    unsigned char leftTile   = spr->X >> 3; // Gets the tile left of the player.
    unsigned char rightTile  = (spr->X + spr->width) >> 3; // Gets the tile to the right of the player.
    unsigned char topTile    = spr->Y >> 3; // Gets the tile above the player.
    unsigned char bottomTile = (spr->Y + spr->height) >> 3; // Gets the tile below the player.

    if(wallDetection(leftTile, topTile) || wallDetection(rightTile, topTile) || wallDetection(leftTile, bottomTile) || wallDetection(rightTile, bottomTile)) // If any of the tiles are a wall.
    {
        return 1; // Return a 1, meaning thats a wall.
    }
    else 
	{
        return 0; // Return a 0, meaing they can move.
    }
}

unsigned char getCollisionValue(unsigned char x, unsigned char y)
{
    if(x >= 32 || y >= 30) // If the tile is out of bounds.
	{
        return 1; // Return a 1 to say it's a wall.
    }

    switch(currentLevel) // Changes based on the current level.
	{
        case 0: 
            return levelOneCollision[y * 32 + x]; // Returns the collision value for the tile at X and Y of the collision table for the level.
    }
}

// PORTAL BULLET FUNCTIONS 
// -===============================================================-
void spawnOrangeBullet(void)
{
    orangeBulletSpriteData.X = playerSpriteData.X; // Sets the orange bullet X data to the player's X data.
    orangeBulletSpriteData.Y = playerSpriteData.Y; // Sets the orange bullet Y data to the player's Y data.
    orangeBulletActive = 1; // Sets that the orange bullet is now active.

    orangeBulletDirectionX = aimDirectionX; // Sets the orange bullets X direction to the X direction the player was aiming.
    orangeBulletDirectionY = aimDirectionY; // Sets the orange bullets Y direction to the Y direction the player was aiming.

    if(orangeBulletDirectionX == 0 && orangeBulletDirectionY == 0) // If the player didn't touch the DPAD to provide any aiming.
	{ 
        orangeBulletDirectionY = -1; // Default to aiming up.
    }
}

void spawnBlueBullet(void)
{
    blueBulletSpriteData.X = playerSpriteData.X; // Sets the blue bullet X data to the player's X data.
    blueBulletSpriteData.Y = playerSpriteData.Y; // Sets the blue bullet Y data to the player's Y data.
    blueBulletActive = 1; // Sets that the blue bullet is now active.

    blueBulletDirectionX = aimDirectionX; // Sets the blue bullets X direction to the X direction the player was aiming.
    blueBulletDirectionY = aimDirectionY; // Sets the blue bullets Y direction to the Y direction the player was aiming.

    if(blueBulletDirectionX == 0 && blueBulletDirectionY == 0)  // If the player didn't touch the DPAD to provide any aiming.
	{
        blueBulletDirectionY = -1; // Default to aiming up.
    }
}

void updateBullet(void)
{
    unsigned char tileX; // Holds the X tile that the bullet has collided with.
    unsigned char tileY; // Holds the Y tile that the bullet has collided with.
    unsigned char collision; // Holds the collision value of the tile the bullet collided with.
	unsigned char otherPortalTileX; // Holds the X tile that the opposite portal is located at.
	unsigned char otherPortalTileY; // Holds the Y tile that the opposite portal is located at.

    if(orangeBulletActive) // If the orange bullet is active.
    {
        orangeBulletSpriteData.X += orangeBulletDirectionX; // Moves the orange bullet in the X direction that the player was aiming when it was shot.
        orangeBulletSpriteData.Y += orangeBulletDirectionY; // Moves the orange bullet in the Y direction that the player was aiming when it was shot.

        tileX = (orangeBulletSpriteData.X + 4) >> 3; // Sets the X tile value to the tile the bullet collided with.
        tileY = (orangeBulletSpriteData.Y + 4) >> 3; // Sets the Y tile value to the tile the bullet collided with.
        collision = getCollisionValue(tileX, tileY); // Gets the collision value for that tile, from the collision map.

        if(collision == 2) // If the tile has a collision value of 2, meaning it can hold a portal.
        {
            if(orangeBulletDirectionX > 0) // If the orange bullet was shot to the right.
			{
                orangePortalOrientation = 1; // Set the portal orientaion to right, so the player gets spit out to the left
            }
            else if(orangeBulletDirectionX < 0) // If the orange bullet was shot to the left.  
			{
                orangePortalOrientation = 3; // Set the portal orientaion to left, so the player gets spit out to the right.
            }
            else if(orangeBulletDirectionY > 0) // If the orange bullet was shot down.
			{
                orangePortalOrientation = 2; // Set the portal orientaion to down, so the player gets spit out up.
            }
            else // If the orange bullet was shot up.
			{
                orangePortalOrientation = 0; // Set the portal orientaion to up, so the player gets spit out down.
            }

			if (bluePortalActive) // If the blue portal is active.
			{
				otherPortalTileX = bluePortalSpriteData.X >> 3; // Get the X tile that the blue portal is located on.
				otherPortalTileY = bluePortalSpriteData.Y >> 3; // Get the Y tile that the blue portal is location on.
				if (otherPortalTileX == tileX && otherPortalTileY == tileY) // Check if the orange portal is trying to be placed at the same location as the blue portal.
				{
					orangeBulletActive = 0; // Disable the orange bullet if true.
					return;
				}
			}

            if(orangePortalOrientation == 1) // If the portal was shot to the right.
			{
                orangePortalSpriteData.X = (tileX << 3) + (8 - PORTAL_WIDTH); // Shift the portal to the left, so it's not stuck fully in the wall and can be entered.
                orangePortalSpriteData.Y = tileY << 3;
            }
            else if(orangePortalOrientation == 2) // If the portal was shot down.
            {
                orangePortalSpriteData.X = tileX << 3; // Shift the portal up.
                orangePortalSpriteData.Y = (tileY << 3) + (8 - PORTAL_HEIGHT);
            }
            else // If the portal was shot left or up, just place the portal normally.
            {
                orangePortalSpriteData.X = tileX << 3;
                orangePortalSpriteData.Y = tileY << 3;
            }

            orangePortalActive = 1; // Set the orange portal as active.
            orangeBulletActive = 0; // Set the orange bullet as not active, so it stops getting drawn.
        }
        else if(collision == 1) // If the bullet hits a normal wall.
        {
            orangeBulletActive = 0; // Set the orange bullet as not active, so it stops getting drawn.
        }
    }


    if(blueBulletActive) // If the blue bullet is active.
    {
        blueBulletSpriteData.X += blueBulletDirectionX; // Move the bullet in the X direction the player was aiming when it was shot.
        blueBulletSpriteData.Y += blueBulletDirectionY; // Move the bullet in the Y direction the player was aiming when it was shot.

        tileX = (blueBulletSpriteData.X + 4) >> 3; // Sets the X tile value to the tile the bullet collided with.
        tileY = (blueBulletSpriteData.Y + 4) >> 3; // Sets the Y tile value to the tile the bullet collided with.
        collision = getCollisionValue(tileX, tileY); // Sets the collision value to the value of the tile in the collision map.

        if(collision == 2) // If the tile has a collision of 2, meaning it can hold a portal.
        {
            if (blueBulletDirectionX > 0) // If the bullet was shot to the right.
			{
                bluePortalOrientation = 1; // Set the portal orientation to the right, so the player gets spit out to the left.
            }
            else if (blueBulletDirectionX < 0) // If the bullet was shot to the left.
			{
                bluePortalOrientation = 3; // Set the portal orientation to the left, so the player gets spit out to the right.
            }
            else if (blueBulletDirectionY > 0) // If the bullet was shot down.
			{
                bluePortalOrientation = 2; // Set the portal orientation to down, so the player gets spit out up.
            }
            else // If the bullet was shot up.
			{
                bluePortalOrientation = 0; // Set the portal orientation to up, so the player gets spit out down.
            }

			if (orangePortalActive) // If the orange portal is active.
			{
				otherPortalTileX = orangePortalSpriteData.X >> 3; // Get the X tile that the orange portal is located at.
				otherPortalTileY = orangePortalSpriteData.Y >> 3; // Get the Y tile that the orange portal is located at.
				if (otherPortalTileX == tileX && otherPortalTileY == tileY) // Check if the blue portal is trying to be placed at the same location of the orange portal.
				{
					blueBulletActive = 0; // Disable the blue bullet if true.
					return;
				}
			}

            if (bluePortalOrientation == 1) // If the portal was shot to the right.
            {
                bluePortalSpriteData.X = (tileX << 3) + (8 - PORTAL_WIDTH); // Shift the portal to the left, so it's not stuck fully in the wall and can be entered.
                bluePortalSpriteData.Y = tileY << 3;
            }
            else if (bluePortalOrientation == 2) // If the portal was shot down.
            {
                bluePortalSpriteData.X = tileX << 3; // Shift the portal up.
                bluePortalSpriteData.Y = (tileY << 3) + (8 - PORTAL_HEIGHT);
            }
            else // If the portal was shot left or up, just place the portal normally.
            {
                bluePortalSpriteData.X = tileX << 3;
                bluePortalSpriteData.Y = tileY << 3;
            }

            bluePortalActive = 1; // Set the blue portal to active.
            blueBulletActive = 0; // Set the blue bullet to not active so it can be shot again.
        }
        else if (collision == 1) // If the bullet hits a wall that can't hold a portal.
        {
            blueBulletActive = 0;// Set the blue bullet to not active so it can be shot again.
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
    unsigned char newSelect = (pad1 & PAD_SELECT); // Sets the new select value.

    if (newSelect && !oldSelect) // If the value changed from not pressed to pressed.
    {
        mode = !mode; // Flip the mode value.
    }
    oldSelect = newSelect; 
}

void walkMode(void)
{
    if (pad1 & PAD_LEFT) // If Left on the DPAD is pressed.
    {
        playerSpriteData.X--; // Move the player to the left.
        if (playerWallCollision(&playerSpriteData)) // If the player is now colliding with a wall.
        {
            playerSpriteData.X++; // Move the player to the right, so they're not in the wall anymore.
        }
    }
    else if (pad1 & PAD_RIGHT) // If Right on the DPAD is pressed.
    {
        playerSpriteData.X++; // Move the player to the right.
        if (playerWallCollision(&playerSpriteData)) // If the player is now colliding with a wall.
        {
            playerSpriteData.X--; // Move the player to the left, so they're not in the wall anymore.
        }
    }
}

void shootMode(void)
{
    signed char newDirectionX = 0; // Resets the X aiming direction.
    signed char newDirectionY = 0; // Resets the Y aiming direction.

    if(pad1 & PAD_UP) // If Up on the DPAD is pressed.
	{
        newDirectionY = -1; // Set the new Y aiming direction to -1, to signify up.
    }

    else if(pad1 & PAD_DOWN) // If Down on the DPAD is pressed.  
	{ 
        newDirectionY = 1; // Set the new Y aiming direction to 1, to signify down.
    }

    if(pad1 & PAD_LEFT) // If Left on the DPAD is pressed.
	{
        newDirectionX = -1; // Set the new X aiming direction to -1, to signify left.
    }

    else if(pad1 & PAD_RIGHT) // If Right on the DPAD is pressed.
	{
        newDirectionX = 1; // Set the new X aiming direction to 1, to signify right.
    }

    if(newDirectionX != 0 || newDirectionY != 0) // If the player has aimed somewhere.
    {
        aimDirectionX = newDirectionX; // Set the global X aiming direction to the new X aiming direction.
        aimDirectionY = newDirectionY; // Set the global Y aiming direction to the new Y aiming direction.
    }

    if(!orangeBulletActive && (pad1 & PAD_A)) // If there is not a orange bullet, and A is pressed.
    {
        spawnOrangeBullet(); // Spawn an orange bullet.
    }
    if(!blueBulletActive && (pad1 & PAD_B)) // If there is not a blue bullet, and B is pressed.
    {
        spawnBlueBullet(); // Spawn a blue bullet.
    }
}

// GRAVITY FUNCTIONS 
// -============================================================================-
unsigned char onGround(void)
{
    unsigned char footY     = (playerSpriteData.Y + playerSpriteData.height + 1) >> 3; // Gets the tile directly below the sprite.
    unsigned char footXLeft = playerSpriteData.X >> 3; // Gets the tile that the "left foot", AKA, the left half of the sprite is touching.
    unsigned char footXRight= (playerSpriteData.X + playerSpriteData.width) >> 3; // Gets the tile that the "right foot", AKA the left half of the player sprite is touching.

    if(wallDetection(footXLeft, footY) || wallDetection(footXRight, footY)) // If either "foot" is touching a wall tile, or there's a wall tile directly below the player.
    {
        return 1; // Return a 1, to show that they're on the floor.
    }
    return 0; // Return a 0, to show that they're in the air.
}

void applyGravity(void) // 
{
    if(!onGround()) // If the player is not on the ground
    {
        playerVelocity += GRAVITY; // Add the gravity value to the player velocity.
        if (playerVelocity > MAX_FALL_SPEED) // If the player has more velocity than the max fall speed.
		{
            playerVelocity = MAX_FALL_SPEED; // Set the velocity to the max fall speed.
        }
    }

    else  // If the player is on the floor.
    {
        if (playerVelocity > 0) // Check if the player has an remaining velocity, 
		{
            playerVelocity = 0; // Set the velocity to 0.
        }
    }

    if(playerVelocity != 0) // If the player has velocity, and is falling. 
	{
        playerSpriteData.Y += playerVelocity; // Move the player down.
    }
    playerVelocity = 0; 
}

// PLAYER SPRITE FUNCTIONS 
// -=======================================================================-
void drawSprite(void)
{
    const unsigned char* currentSprite = getPlayerSprite(); // Holds the current player sprite. 
    oam_meta_spr(playerSpriteData.X, playerSpriteData.Y, currentSprite); // Draw the current sprite at the player's X and Y data.
}

const unsigned char* getPlayerSprite(void)
{
    if(mode == 0) // If the player is in move mode.
    {
        return playerSprite; // Return the default player sprite.
    }

    // Return the appropriate aiming sprite, depening on where the player is aiming.
    if(aimDirectionX == 0 && aimDirectionY == -1) 
	{
        return playerShootUpSprite;
    }
    if(aimDirectionX == 0 && aimDirectionY == 1) 
	{
        return playerShootDownSprite;
    }
    if(aimDirectionX == -1 && aimDirectionY == 0) 
	{
        return playerShootLeftSprite;
    }
    if(aimDirectionX == 1 && aimDirectionY == 0) 
	{
        return playerShootRightSprite;
    }
    if(aimDirectionX == -1 && aimDirectionY == -1) 
	{
        return playerShootTopLeftSprite;
    }
    if(aimDirectionX == 1 && aimDirectionY == -1) 
	{
        return playerShootTopRightSprite;
    }
    if(aimDirectionX == -1 && aimDirectionY == 1) 
	{
        return playerShootBottomLeftSprite;
    }
    if(aimDirectionX == 1 && aimDirectionY == 1) 
	{
        return playerShootBottomRightSprite;
    }

    return playerShootUpSprite; // Return the aiming up sprite, if no directions have pressed.
}

// LEVEL LOADING FUNCTIONS 
//-==============================================================================-
void loadLevel(unsigned char lvl)
{
    ppu_off(); // Turn the screen off.

	pal_bg(levelPaletteBackground); // Sets the Background Palette.

    switch(lvl) // Switch on the current level.
    {
        case 0: 
            vram_adr(NAMETABLE_A); // Set the VRAM address to the start of NameTableA.
            vram_write(levelOneData, 1024); // Fill NameTableA with all 1024 bytes from the levelOneData array, which includes the attribute table.
            playerSpriteData.X = 136; // Sets the player location to the starting location for level 1.
            playerSpriteData.Y = 216; // Sets the player location to the starting location for level 1.
			doorSpriteData.X = 200;
			doorSpriteData.Y = 216;
            break;
    }

    bluePortalActive   = 0; // Reset the blue portal active.
    orangePortalActive = 0; // Reset the orange portal active.
    blueBulletActive   = 0; // Reset the blue bullet being active.
    orangeBulletActive = 0; // Reset the orange bullet being active.
    lastPortalUsed     = 0; // Reset the last portal used.

    ppu_on_all(); // Turn the screen back on.
}

void drawMainMenu(void)
{
    ppu_off(); // Turns the screen off.
    vram_adr(NAMETABLE_A); // Set the VRAM address to the start of NameTableA.
    vram_write(menu, 1024); // Fill NameTableA with all 1024 bytes from the menu array, which includes the attribute table.
    ppu_on_all(); // Turns the screen on.
}

void animatePressStartText(void)
{
    static unsigned char flashTimer = 0; // Tracks thej flash timer. Static to keep the value between function calls.
    static unsigned char showPressStartText = 1; // Tracks if the text is visible. Static to keep the value between function calls.

    flashTimer++; // Increments the flash timer.

    if(flashTimer >= 30) // If the flash timer has gone for 30 frames.
    {
        flashTimer = 0; // Reset the flash timer.
        ppu_off(); // Turn the screen off.

        vram_adr(NAMETABLE_A + (17 * 32) + 10); // Sets the VRAM address to the start of NametableA, and then moves to the location of the 17th row and 10th col to the exact location the PRESS START! tiles are.

        if(showPressStartText) // If the text is visible.
        {
            vram_write(blankTiles, 12); // Change the text tiles to blank tiles.
            showPressStartText = 0; // Set that the text is not visible anymore.
        }
        else 
        {
            vram_write(pressStartTiles, 12); // Change the blank tiles to the text tiles.
            showPressStartText = 1; // Set that the text is now visible.
        }

        ppu_on_all(); // Turn the screen on.
    }
}

void drawOrangePortalSprite(void)
{
    if(orangePortalActive)
    {
        oam_meta_spr(orangePortalSpriteData.X, orangePortalSpriteData.Y, orangePortal);
    }
}

void drawBluePortalSprite(void)
{
    if(bluePortalActive)
    {
        oam_meta_spr(bluePortalSpriteData.X, bluePortalSpriteData.Y, bluePortal);
    }
}

void drawDoorSprite(void)
{
	oam_meta_spr(doorSpriteData.X, doorSpriteData.Y, doorSprite);
}

void doorPlayerCollision(void)
{
	 doorCollision = check_collision(&playerSpriteData, &doorSpriteData);

	 if (doorCollision)
	 {
		currentLevel++;
		loadLevel(currentLevel);
	 }
}