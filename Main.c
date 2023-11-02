#include <gb/gb.h>
#include <stdio.h>
#include "TileData.c"
#include "string.h"
#include "BackgroundTiles.c"
#include "testmap.c"
#include "testmap2.c"
#include "testmap3.c"
#include "char_fwd_1.h"
#include "char_fwd_1.c"
#include "rand.h"

#define mapWidth 32
#define mapHeight 32
#define testmapBank 0
#define no_maps 3

unsigned char map [1024];
unsigned char* maps[3] = {testmap,testmap2,testmap3};

/*sprite 0 and 1 are the character sprites drawn in 8x16 mode*/

/*movement and input*/
uint8_t playerX=0,playerY=0;
uint16_t joypadCurrent=0,joypadPrev=0;

void generateNewMap()
{
	int16_t elements = sizeof(testmap)/sizeof(testmap[0]);
	int16_t idx = 0;
	//memcpy(&map[0],&maps[1][0],256);
	//memcpy(&map[257],&maps[0][640],320);
	int chunk_size = (rand() %(5-3 + 1)+3);
	int loops = (BackgroundTilesWidth*BackgroundTilesHeight)/chunk_size;
	for(int8_t i=0; i< 32; i++)
	{
		
		memcpy(&map[idx],&maps[2][idx],32);
		idx+=32;
	} 

	//try randomising size of the chunks to load from mem. ie size of the loop
	

}

int main()
{

	SHOW_BKG;
	SHOW_SPRITES;
	SPRITES_8x16;
	DISPLAY_ON; 

	set_bkg_data(0,tileData_NUM_TILES,TileData);
	set_sprite_data(0,char_NUM_TILES,character);
	initrand(DIV_REG);

	generateNewMap();
	set_bkg_tiles(0,0,testmapWidth,testmapHeight,map);
	set_sprite_tile(0,0);
	set_sprite_tile(1,2);
	move_sprite(0,playerX + charSprites_width/2, playerY + charSprites_height);
	move_sprite(1,playerX + charSprites_width/2 + 8,playerY + charSprites_height);
	uint8_t moved;
	uint8_t moveFrameDelay = 30;
	uint8_t frames = moveFrameDelay;

	

	while(1)
	{
		frames++;
		if(frames >= moveFrameDelay)
			moved = 0;

		joypadPrev = joypadCurrent;
		joypadCurrent = joypad();

		if(!moved)
		{
			if(joypadCurrent & J_LEFT)
			{
				playerX -=8;
				moved =1;
				frames = 0;
			} 
			if(joypadCurrent & J_RIGHT)
			{
				playerX +=8;
				moved = 1;
				frames = 0;
			} 
			if(joypadCurrent & J_UP)
			{
				playerY -=8;
				moved = 1;
				frames = 0;
			}
			if(joypadCurrent & J_DOWN)
			{
				playerY +=8;
				moved = 1;
				frames = 0;
			} 
				
		}

		

		
		move_sprite(0,playerX + charSprites_width/2, playerY + charSprites_height);
		move_sprite(1,playerX + charSprites_width/2 + 8,playerY + charSprites_height);
		
		
		wait_vbl_done();
	
	}
	return 0;
	
}