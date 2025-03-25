const unsigned char testSprite[] = 
{
    0, 0, 0x00, 0,  // Relative X, Relative Y, Tile Index, Pallete & Attributes.
    0, 8, 0x10, 0,  // For Example - Relative x is 0, Relative Y is 8, meaning place this 2nd tile 8 pixels below the tile above. 0x10 is the Tile Index for the tile, if viewing in NEXXT, it's the tile directly below the first one, then 0 at the ends means use palette 0..
    8, 0, 0x00, 0|OAM_FLIP_H, // 0|OAM_FLIP_H - means use palette 0, but flip the sprite horizontally. this is useful as it saved tiles for symetrical sprites.
    8, 8, 0x10, 0|OAM_FLIP_H,
    128 // This basically means "this is the end please stop reading sprite data".
};

const unsigned char testPortal1[] =
{
    0, 0, 0x01, 1, // Relative X, Relative Y, Tile Index, Tells it to use Pallete 1.
    0, 8, 0x01, 1|OAM_FLIP_V, // Exact same tile as above, just flipped vertically to make the illusion of a portal.
    128 // Ends the sprite.
};

const unsigned char testPortal2[] = 
{
    0, 0, 0x01, 2, // Exact same as the first portal, however tells it to use palette 2 instead.
    0, 8, 0x01, 2|OAM_FLIP_V,
    128 // Ends the sprite.
};

struct spriteData // Struct to hold the Sprite Format that every sprite will follow.
{
    unsigned char X;
    unsigned char Y;
    unsigned char height;
    unsigned char width;
};

struct spriteData testSpriteData = {16, 200, 15, 14}; // Sprite Data For the Player Sprite

struct spriteData portal1SpriteData = {200, 200, 7, 2}; // Sprite Data for the First Portal Sprite.

struct spriteData portal2SpriteData = {100, 200, 7, 2}; // Sprite Data for the Second Portal Sprite.

// FUNCTION PROTOTYPES
void drawSprite(void); // Prototype for drawing sprite function.

void drawSprite(void)
{
    oam_clear(); // Clears all sprites from the sprite buffer.

    oam_meta_spr(testSpriteData.X, testSpriteData.Y, testSprite); // Draws the metasprite at x pos 64, y pos 80 and using the testSprite data. Nes Screen is 256 x 240 in pixels, so max range for sprite drawing is 255, 239.
    oam_meta_spr(portal1SpriteData.X, portal1SpriteData.Y, testPortal1);
    oam_meta_spr(portal2SpriteData.X, portal2SpriteData.Y, testPortal2);
}

