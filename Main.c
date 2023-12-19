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

const uint8_t MOVE_FRAME_DELAY = 5;
const uint8_t SCROLL_AMT = 16;

/*sprite 0 and 1 are the character sprites drawn in 8x16 mode*/

/*movement and input*/
uint16_t playerX=8,playerY=16;
uint8_t joypadCurrent=0,joypadPrev=0;

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

uint16_t getPlayerX()
{
	uint16_t plX = playerX;
	return plX;
}

uint16_t getPlayerY()
{
	uint16_t plY = playerY;
	return plY;
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

	uint16_t viewportOffsetX = 0;
	uint16_t viewportOffsetY = 0;
	uint8_t moved = 0;
	uint8_t frames = MOVE_FRAME_DELAY;
	

	while(1)
	{
		/*
		*only allow movement once every n frames
		*do not allow movement to invoke the screen wrap
		*/
		frames++;
		if(frames >= MOVE_FRAME_DELAY)
		{
			moved = 0;
			frames = 0;
		}
		joypadPrev = joypadCurrent;
		joypadCurrent = joypad();

		if(!moved)
		{
			if(joypadCurrent & J_LEFT)
			{
				if( getPlayerX() + viewportOffsetX > 8)
				{
					playerX -=8;
					moved =1;
					frames = 0;	
				}			
			} 
			if(joypadCurrent & J_RIGHT)
			{
				if(getPlayerX() + viewportOffsetX < mapWidth * 8)
				{
					playerX +=8;
					moved = 1;
					frames = 0;
				}	
			} 
			if(joypadCurrent & J_UP)
			{
				if(getPlayerY() + viewportOffsetY > 16)
				{
					playerY -=8;
					moved = 1;
					frames = 0;
				}
			}
			if(joypadCurrent & J_DOWN)
			{
				if(getPlayerY() + viewportOffsetY < mapHeight * 8)
				{
					playerY +=8;
					moved = 1;
					frames = 0;
				}	
			} 
				
		}

		/* 
		*scroll the background whenever we hit an edge
		*except when we reach the edge of the map
		*/
		if(getPlayerX() >= SCREENWIDTH)
		{
			playerX -= SCROLL_AMT;
			delay(100);
			scroll_bkg(SCROLL_AMT,0);
			
			viewportOffsetX +=SCROLL_AMT;
		}
		else if(getPlayerX() + viewportOffsetX < (8+viewportOffsetX))
		{
			playerX += SCROLL_AMT;
			delay(100);
			scroll_bkg(-1 * SCROLL_AMT, 0);

			viewportOffsetX -= SCROLL_AMT;
		}
		else if(getPlayerY() >= SCREENHEIGHT)
		{
			playerY-= SCROLL_AMT;
			delay(100);
			scroll_bkg(0,SCROLL_AMT);

			viewportOffsetY += SCROLL_AMT;
		}
		else if(getPlayerY() + viewportOffsetY < (16+viewportOffsetY))
		{
			playerY +=SCROLL_AMT;
			delay(100);
			scroll_bkg(0, -1 * SCROLL_AMT);
			viewportOffsetY -= SCROLL_AMT;
		}
		

		uint16_t playerXa = getPlayerX()+ charSprites_width/2;
		move_sprite(0,playerX, playerY);
		move_sprite(1,playerXa,playerY);
		
		
		wait_vbl_done();
	
	}
	return 0;
	
}