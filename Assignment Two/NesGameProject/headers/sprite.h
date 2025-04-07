
/*
SO ALL SPRITES IN THIS FILE FOLLOW THIS STRUCTURE:

RELATIVE X, RELATIVE Y, TILE INDEX, PALETTE TO USE & ANY ATTRIBUTES
ATTRIBUTES INCLUDE OAM_FLIP_H WHICH FLIP THE TILE HORIZONTALLY, AND OAM_FLIP_V, WHICH FLIP THEM VERTICALLY.

AND THEN 128 - WHICH BASICALLY MEANS "THIS SPRITE IS DONE NOW PLEASE STOP READING THE DATA".
*/


const unsigned char playerSprite[] = 
{
    0, 0, 0x20, 0,  
    0, 8, 0x30, 0,  
    8, 0, 0x21, 0, 
    8, 8, 0x31, 0,
    128 
};

const unsigned char playerShootUpSprite[] = 
{
    0, 0, 0x02, 0,
    0, 8, 0x12, 0,
    8, 0, 0x03, 0,
    8, 8, 0x13, 0,
    128 
};

const unsigned char playerShootDownSprite[] = 
{
    0, 0, 0x06, 0,  
    0, 8, 0x16, 0,  
    8, 0, 0x07, 0, 
    8, 8, 0x17, 0,
    128 
};

const unsigned char playerShootLeftSprite[] = 
{
    0, 0, 0x04, 0,  
    0, 8, 0x14, 0, 
    8, 0, 0x05, 0, 
    8, 8, 0x15, 0,
    128 
};

const unsigned char playerShootRightSprite[] = 
{
    0, 0, 0x00, 0,  
    0, 8, 0x10, 0,  
    8, 0, 0x01, 0, 
    8, 8, 0x11, 0,
    128 
};

const unsigned char playerShootTopRightSprite[] = 
{
    0, 0, 0x08, 0,  
    0, 8, 0x18, 0,  
    8, 0, 0x09, 0, 
    8, 8, 0x19, 0,
    128 
};

const unsigned char playerShootBottomRightSprite[] = 
{
    0, 0, 0x0E, 0,  
    0, 8, 0x1E, 0,  
    8, 0, 0x0F, 0, 
    8, 8, 0x1F, 0,
    128 
};

const unsigned char playerShootBottomLeftSprite[] = 
{
    0, 0, 0x0C, 0, 
    0, 8, 0x1C, 0,  
    8, 0, 0x0D, 0, 
    8, 8, 0x1D, 0,
    128 
};

const unsigned char playerShootTopLeftSprite[] = 
{
    0, 0, 0x0A, 0,  
    0, 8, 0x1A, 0,  
    8, 0, 0x0B, 0, 
    8, 8, 0x1B, 0,
    128 
};


const unsigned char orangePortal[] =
{
    0, 0, 0xEF, 0, 
    0, 8, 0xEF, 0|OAM_FLIP_V, 
    8,  0, 0xEF, 0|OAM_FLIP_H, 
    8,  8, 0xEF, 0|OAM_FLIP_H|OAM_FLIP_V, 
    128 
};

const unsigned char bluePortal[] = 
{
    0, 0, 0xEF, 2,
    0, 8, 0xEF, 2|OAM_FLIP_V,
    8, 0, 0xEF, 2|OAM_FLIP_H,
    8, 8, 0xEF, 2|OAM_FLIP_H|OAM_FLIP_V,
    128 
};

const unsigned char orangeBulletSprite[] =
{
    0, 0, 0xFF, 0,
    128
};

const unsigned char blueBulletSprite[] =
{
    0, 0, 0xFF, 2,
    128
};

const unsigned char boxSprite[] = 
{
    0, 0, 0x22, 3, 
    0, 8, 0x32, 3,
    8, 0, 0x23, 3,
    8, 8, 0x33, 3,
    128 
};


struct spriteData // Struct to hold the Sprite Format that every sprite will follow.
{
    unsigned char X; // X location of the sprite.
    unsigned char Y; // Y location of the sprite.
    unsigned char height; // Height of the sprite.
    unsigned char width; // Width of the sprite.
};

struct spriteData playerSpriteData = {16, 200, 15, 14}; // Sprite Data For the Player Sprite

struct spriteData orangePortalSpriteData = {0, 0, 16, 16}; // Sprite Data for the orange Portal Sprite.

struct spriteData bluePortalSpriteData = {0, 0, 18, 16}; // Sprite Data for the blue Portal Sprite.

struct spriteData orangeBulletSpriteData = {0, 0, 7, 7}; // Sprite Data for the orange bullet sprite.

struct spriteData blueBulletSpriteData = {0, 0, 7 ,7}; // Sprite data for the blue bullet sprite.

struct spriteData boxSpriteData = {0, 0, 16, 16}; // Sprite data for the box sprite.

