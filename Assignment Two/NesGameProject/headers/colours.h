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

const unsigned char paletteBackground[]=
{
BLACK, DK_GY, LT_GY, WHITE, // Sub-Palette 0 
ORANGE,BLUE,PURPLE,YELLOW, // Sub-Palette 1
0,0,0,0, // Sub-Palette 2
0,0,0,0 // Sub-Palette 3
}; 

const char paletteSprite[] = 
{
BLACK, DK_GY, LT_GY, WHITE, // Sub-Palette 0
BLACK, DK_GY, LT_GY, ORANGE, // Sub-Palette 1
BLACK, DK_GY, LT_GY, DK_BLUE, // Sub Palette 2
0,0,0,0 // Sub Palette 3
};