#define BLACK 0x0F
#define DK_GY 0x00
#define LT_GY 0x10
#define WHITE 0x30
#define ORANGE 0x27
#define YELLOW 0x28
#define GREEN 0x2A
#define TEAL 0x2C
#define RED 0x16
#define PURPLE 0x14
#define BLUE 0x12
#define DK_GREEN 0x09
#define DK_BLUE 0x11
#define SKIN 0x36
#define LT_BLUE 0x31
#define GRILL 0x2C
#define BROWN 0x07

const unsigned char paletteBackground[]=
{
BLACK, DK_BLUE, LT_GY, WHITE, // Sub-Palette 0 
BLACK,BLACK,BLACK,ORANGE, // Sub-Palette 1
BLACK,DK_GY,LT_GY,WHITE, // Sub-Palette 2
BLACK,BLACK,BLACK,BLACK // Sub-Palette 3
}; 

const char paletteSprite[] = 
{
TEAL, SKIN, WHITE, ORANGE, // Sub-Palette 0
TEAL, BLACK, LT_BLUE, LT_GY, // Sub-Palette 1
TEAL, ORANGE, LT_GY, DK_BLUE, // Sub Palette 2
TEAL,LT_GY,DK_GY,0X21 // Sub Palette 3
};

const unsigned char levelPaletteBackground[]=
{
LT_BLUE, DK_GY, LT_GY, RED, // Sub-Palette 0 
LT_BLUE,DK_GY,LT_GY,GRILL, // Sub-Palette 1
LT_BLUE,DK_GY,LT_GY,WHITE, // Sub-Palette 2
LT_BLUE,BLACK,0x21,DK_GY // Sub-Palette 3
}; 

const unsigned char endingScreenPalette[]= 
{
    LT_GY, BROWN, WHITE, RED,
    LT_GY, LT_GY, DK_GY, GREEN,
    LT_GY, BLACK, LT_BLUE, DK_GY,
    LT_GY, DK_GY, WHITE, RED
};