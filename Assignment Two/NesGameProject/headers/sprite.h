const unsigned char playerSprite[] = 
{
    0, 0, 0x20, 0,  // Relative X, Relative Y, Tile Index, Pallete & Attributes.
    0, 8, 0x30, 0,  // For Example - Relative x is 0, Relative Y is 8, meaning place this 2nd tile 8 pixels below the tile above. 0x10 is the Tile Index for the tile, if viewing in NEXXT, it's the tile directly below the first one, then 0 at the ends means use palette 0..
    8, 0, 0x21, 0, // 0|OAM_FLIP_H - means use palette 0, but flip the sprite horizontally. this is useful as it saved tiles for symetrical sprites.
    8, 8, 0x31, 0,
    128 // This basically means "this is the end please stop reading sprite data".
};

const unsigned char playerShootUpSprite[] = 
{
    0, 0, 0x02, 0,  // Relative X, Relative Y, Tile Index, Pallete & Attributes.
    0, 8, 0x12, 0,  // For Example - Relative x is 0, Relative Y is 8, meaning place this 2nd tile 8 pixels below the tile above. 0x10 is the Tile Index for the tile, if viewing in NEXXT, it's the tile directly below the first one, then 0 at the ends means use palette 0..
    8, 0, 0x03, 0, // 0|OAM_FLIP_H - means use palette 0, but flip the sprite horizontally. this is useful as it saved tiles for symetrical sprites.
    8, 8, 0x13, 0,
    128 // This basically means "this is the end please stop reading sprite data".
};

const unsigned char playerShootDownSprite[] = 
{
    0, 0, 0x06, 0,  // Relative X, Relative Y, Tile Index, Pallete & Attributes.
    0, 8, 0x16, 0,  // For Example - Relative x is 0, Relative Y is 8, meaning place this 2nd tile 8 pixels below the tile above. 0x10 is the Tile Index for the tile, if viewing in NEXXT, it's the tile directly below the first one, then 0 at the ends means use palette 0..
    8, 0, 0x07, 0, // 0|OAM_FLIP_H - means use palette 0, but flip the sprite horizontally. this is useful as it saved tiles for symetrical sprites.
    8, 8, 0x17, 0,
    128 // This basically means "this is the end please stop reading sprite data".
};

const unsigned char playerShootLeftSprite[] = 
{
    0, 0, 0x04, 0,  // Relative X, Relative Y, Tile Index, Pallete & Attributes.
    0, 8, 0x14, 0,  // For Example - Relative x is 0, Relative Y is 8, meaning place this 2nd tile 8 pixels below the tile above. 0x10 is the Tile Index for the tile, if viewing in NEXXT, it's the tile directly below the first one, then 0 at the ends means use palette 0..
    8, 0, 0x05, 0, // 0|OAM_FLIP_H - means use palette 0, but flip the sprite horizontally. this is useful as it saved tiles for symetrical sprites.
    8, 8, 0x15, 0,
    128 // This basically means "this is the end please stop reading sprite data".
};

const unsigned char playerShootRightSprite[] = 
{
    0, 0, 0x00, 0,  // Relative X, Relative Y, Tile Index, Pallete & Attributes.
    0, 8, 0x10, 0,  // For Example - Relative x is 0, Relative Y is 8, meaning place this 2nd tile 8 pixels below the tile above. 0x10 is the Tile Index for the tile, if viewing in NEXXT, it's the tile directly below the first one, then 0 at the ends means use palette 0..
    8, 0, 0x01, 0, // 0|OAM_FLIP_H - means use palette 0, but flip the sprite horizontally. this is useful as it saved tiles for symetrical sprites.
    8, 8, 0x11, 0,
    128 // This basically means "this is the end please stop reading sprite data".
};

const unsigned char playerShootTopRightSprite[] = 
{
    0, 0, 0x08, 0,  // Relative X, Relative Y, Tile Index, Pallete & Attributes.
    0, 8, 0x18, 0,  // For Example - Relative x is 0, Relative Y is 8, meaning place this 2nd tile 8 pixels below the tile above. 0x10 is the Tile Index for the tile, if viewing in NEXXT, it's the tile directly below the first one, then 0 at the ends means use palette 0..
    8, 0, 0x09, 0, // 0|OAM_FLIP_H - means use palette 0, but flip the sprite horizontally. this is useful as it saved tiles for symetrical sprites.
    8, 8, 0x19, 0,
    128 // This basically means "this is the end please stop reading sprite data".
};

const unsigned char playerShootBottomRightSprite[] = 
{
    0, 0, 0x0E, 0,  // Relative X, Relative Y, Tile Index, Pallete & Attributes.
    0, 8, 0x1E, 0,  // For Example - Relative x is 0, Relative Y is 8, meaning place this 2nd tile 8 pixels below the tile above. 0x10 is the Tile Index for the tile, if viewing in NEXXT, it's the tile directly below the first one, then 0 at the ends means use palette 0..
    8, 0, 0x0F, 0, // 0|OAM_FLIP_H - means use palette 0, but flip the sprite horizontally. this is useful as it saved tiles for symetrical sprites.
    8, 8, 0x1F, 0,
    128 // This basically means "this is the end please stop reading sprite data".
};

const unsigned char playerShootBottomLeftSprite[] = 
{
    0, 0, 0x0C, 0,  // Relative X, Relative Y, Tile Index, Pallete & Attributes.
    0, 8, 0x1C, 0,  // For Example - Relative x is 0, Relative Y is 8, meaning place this 2nd tile 8 pixels below the tile above. 0x10 is the Tile Index for the tile, if viewing in NEXXT, it's the tile directly below the first one, then 0 at the ends means use palette 0..
    8, 0, 0x0D, 0, // 0|OAM_FLIP_H - means use palette 0, but flip the sprite horizontally. this is useful as it saved tiles for symetrical sprites.
    8, 8, 0x1D, 0,
    128 // This basically means "this is the end please stop reading sprite data".
};

const unsigned char playerShootTopLeftSprite[] = 
{
    0, 0, 0x0A, 0,  // Relative X, Relative Y, Tile Index, Pallete & Attributes.
    0, 8, 0x1A, 0,  // For Example - Relative x is 0, Relative Y is 8, meaning place this 2nd tile 8 pixels below the tile above. 0x10 is the Tile Index for the tile, if viewing in NEXXT, it's the tile directly below the first one, then 0 at the ends means use palette 0..
    8, 0, 0x0B, 0, // 0|OAM_FLIP_H - means use palette 0, but flip the sprite horizontally. this is useful as it saved tiles for symetrical sprites.
    8, 8, 0x1B, 0,
    128 // This basically means "this is the end please stop reading sprite data".
};


const unsigned char orangePortal[] =
{
    0, 0, 0xEF, 1, // Relative X, Relative Y, Tile Index, Tells it to use Pallete 1.
    0, 8, 0xEF, 1|OAM_FLIP_V, // Exact same tile as above, just flipped vertically to make the illusion of a portal.
    8,  0, 0xEF, 1|OAM_FLIP_H, // Same tile, just off set on the X instead of the Y, and flipped horizontally.
    8,  8, 0xEF, 1|OAM_FLIP_H|OAM_FLIP_V, // Same tile, offset on both the X and Y now, and flipped both vertically and horizontally.
    128 // Ends the sprite.
};

const unsigned char bluePortal[] = 
{
    0, 0, 0xEF, 2, // Exact same as the first portal, however tells it to use palette 2 instead.
    0, 8, 0xEF, 2|OAM_FLIP_V,
    8, 0, 0xEF, 2|OAM_FLIP_H,
    8, 8, 0xEF, 2|OAM_FLIP_H|OAM_FLIP_V,
    128 // Ends the sprite.
};

const unsigned char orangeBulletSprite[] =
{
    0, 0, 0xFF, 1,
    128
};

const unsigned char blueBulletSprite[] =
{
    0, 0, 0xFF, 2,
    128
};

struct spriteData // Struct to hold the Sprite Format that every sprite will follow.
{
    unsigned char X;
    unsigned char Y;
    unsigned char height;
    unsigned char width;
};

struct spriteData playerSpriteData = {16, 200, 15, 14}; // Sprite Data For the Player Sprite

struct spriteData orangePortalSpriteData = {0, 0, 16, 16}; // Sprite Data for the First Portal Sprite.

struct spriteData bluePortalSpriteData = {0, 0, 16, 16}; // Sprite Data for the Second Portal Sprite.

struct spriteData orangeBulletSpriteData = {0, 0, 7, 7};

struct spriteData blueBulletSpriteData = {0, 0, 7 ,7};


unsigned char bluePortalActive = 0;
unsigned char orangePortalActive = 0;


// FUNCTION PROTOTYPES
void drawOrangePortalSprite(void);
void drawBluePortalSprite(void);

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
