#define BLACK 0x0F
#define DK_GY 0x00
#define LT_GY 0x10
#define WHITE 0x30
#define ORANGE 0x27
#define YELLOW 0x28
#define GREEN 0x2A
#define TEAL 0x2C
#define BROWN 0x18
#define RED 0x16
#define PURPLE 0x14
#define BLUE 0x12
#define DK_GREEN 0x09
#define DK_BLUE 0x01
#define SKIN 0x36

const unsigned char paletteBackground[]=
{
BLACK, DK_BLUE, LT_GY, WHITE, // Sub-Palette 0 
ORANGE,ORANGE,ORANGE,ORANGE, // Sub-Palette 1
ORANGE,ORANGE,ORANGE,ORANGE, // Sub-Palette 2
0,0,0,0 // Sub-Palette 3
}; 

const char paletteSprite[] = 
{
BLACK, SKIN, WHITE, ORANGE, // Sub-Palette 0
BLACK, ORANGE, LT_GY, ORANGE, // Sub-Palette 1
BLACK, ORANGE, LT_GY, DK_BLUE, // Sub Palette 2
0,0,0,0 // Sub Palette 3
};