#include <gb/gb.h>
#include <stdio.h>
#include "TileData.c"
#include "string.h"
#include "BackgroundTiles.c"
#include "testmap.c"
#include "testmap2.c"
#include "testmap3.c"
#include "char_1_right.c"
#include "rand.h"

#define tile_size 8
#define mapWidthTiles 32
#define mapHeightTiles 32
#define mapWHPixels 256
#define testmapBank 0
#define no_maps 3

unsigned char map [1024];
unsigned char* maps[3] = {testmap,testmap2,testmap3};

const uint8_t MOVE_FRAME_DELAY = 5;
const uint8_t ANIM_DELAY = 30;
const uint8_t SCROLL_AMT = 16;

/* movement and input */
uint16_t playerX=80,playerY=88;
uint8_t joypadCurrent=0,joypadPrev=0;
uint8_t moved = 0;

/* animation */
uint8_t current_anim = 0;
uint16_t anim_timer = 0;

/* scrolling */
uint16_t viewportOffsetX = 0;
uint16_t viewportOffsetY = 0;

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

void check_bkg_scroll()
{
	if(getPlayerX() > SCREENWIDTH)
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
	else if(getPlayerY() > SCREENHEIGHT)
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
}

uint8_t isValidTile(uint16_t x, uint16_t y)
{
	uint8_t column = ((x-8) + viewportOffsetX)/tile_size;
	uint8_t row = ((y-16) + viewportOffsetY)/tile_size;
	uint16_t tilemapIdx = column +( row * mapWidthTiles);
	return testmap2[tilemapIdx]< 1;
}

void update_animations()
{
	anim_timer = anim_timer++ >= ANIM_DELAY ? 0: anim_timer; 
	current_anim = anim_timer >= ANIM_DELAY ? !current_anim : current_anim;
	set_sprite_tile(0,current_anim);
}

int main()
{
	SHOW_BKG;
	SHOW_SPRITES;
	SPRITES_8x8;
	DISPLAY_ON; 

	uint8_t frames = MOVE_FRAME_DELAY;
	
	set_bkg_data(0,tileData_NUM_TILES,TileData);
	set_sprite_data(0,7,character);
	set_bkg_tiles(0,0,testmapWidth,testmapHeight,testmap2);
	

	while(1)
	{
		update_animations();
		
		frames++;
		if(frames >= MOVE_FRAME_DELAY)
		{
			moved = 0;
			frames = 0;
		}
		joypadPrev = joypadCurrent;
		joypadCurrent = joypad();

		/*
		*only allow movement once every n frames
		*do not allow movement to invoke the screen wrap
		*only allow movement on a legal tile
		*/
		if(!moved)
		{
			if(joypadCurrent & J_LEFT)
			{
				if(isValidTile((getPlayerX()-8),getPlayerY() ) )
				{
					if( getPlayerX() + viewportOffsetX > 8)
					{
						playerX -=8;
						current_anim = 3;
						anim_timer = 0;
						moved = 1;
						frames = 0;	
					}
				}
							
			} 
			if(joypadCurrent & J_RIGHT)
			{
				if(isValidTile((getPlayerX()+8), getPlayerY()))
				{
					if(getPlayerX() + viewportOffsetX < mapWidthTiles * 8)
					{
						playerX +=8;
						current_anim = 2;
						anim_timer = 0;
						moved = 1;
						frames = 0;
					}	
				}
				
			} 
			if(joypadCurrent & J_UP)
			{
				if(isValidTile(getPlayerX(),getPlayerY()-8))
				{
					if(getPlayerY() + viewportOffsetY > 16)
					{
						playerY -=8;
						current_anim = 6;
						anim_timer = 0;
						moved = 1;
						frames = 0;
					}
				}
				
			}
			if(joypadCurrent & J_DOWN)
			{
				if(isValidTile(getPlayerX(),getPlayerY()+8))
				{
					if(getPlayerY() + viewportOffsetY < mapWHPixels)
					{
						playerY +=8;
						current_anim = 4;
						anim_timer = 0;
						moved = 1;
						frames = 0;
					}	
				}
				
			} 
				
		}

		check_bkg_scroll();
		move_sprite(0,getPlayerX(), getPlayerY());
		
		wait_vbl_done();
	
	}
	return 0;
	
}