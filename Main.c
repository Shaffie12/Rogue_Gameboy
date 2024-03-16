#include <gb/gb.h>
#include <stdio.h>
#include "TileData.c"
#include "string.h"
#include "BackgroundTiles.c"
#include "defaultMap.c"
#include "char_1_right.c"
#include "rand.h"

#define tile_size 8
#define mapWidthTiles 32
#define mapHeightTiles 32
#define mapWHPixels 256
#define testmapBank 0
#define no_maps 3
#define room_max_wh 20
#define room_min_wh 7

/* Level */
static uint64_t LEVEL = 1;

/* Maps */
unsigned char map [1024];
int16_t mapSeed;


const uint8_t MOVE_FRAME_DELAY = 5;
const uint8_t ANIM_DELAY = 30;
const uint8_t SCROLL_AMT = 16;

/* Movement and Input */
uint16_t playerX=80,playerY=88;
uint8_t joypadCurrent=0,joypadPrev=0;
uint8_t moved = 0;

/* Animation */
uint8_t current_anim = 0;
uint16_t anim_timer = 0;

/* Scrolling */
uint16_t viewportOffsetX = 0;
uint16_t viewportOffsetY = 0;

void generateRoomSegment(int8_t mapInsertRow, int8_t mapInsertCol, int8_t tileNumber, int8_t numRows, int8_t numCols)
{
	uint16_t r = mapInsertRow;
	uint16_t c = mapInsertCol;
	
	for(uint16_t i = 0; i < numRows; i++)
	{
		if(r >= 31)
			break;
		for(int j = 0; j < numCols; j++)
		{
			if(c >= 31)
				break;	

			uint16_t tile = c + (r* mapWidthTiles);
			uint16_t a = tile;
			map[tile] = BackgroundTiles[tileNumber];
			c++;
		}
		r++;
		c = mapInsertCol;
	}
}

void bridgeRooms(uint8_t* roomsX, uint8_t* roomsY)
{

}
/* 
* TO DO: pass in the room size here based on level. Generate rooms with calculated variable sizes from the notes section. 
* With the ranges, need to set up a data structure to store and get them quickly.
* Track the overlap of rooms to determine how many we have, also written in notes section.
*/
void generateNewMap()
{	
	initrand(DIV_REG); //we should call this after some user input,like after start
	memcpy(&map[0],&defaultMap[0],1024);

	////setup room tracking////
	// const uint8_t maxRooms = ( (defaultMapWidth * defaultMapHeight) * 2) / ( (7 + 1) * (7 + 1) ); 
	// int* roomsX = (int*)malloc(maxRooms* sizeof(uint8_t));
	// int* roomsY = (int*)malloc(maxRooms* sizeof(uint8_t));

	////actually generating the tiles////
	uint8_t rOffset = 1;
	uint8_t cOffset= 1;
	for(uint8_t i = 0; i < 30; i++) //increase the size if fewer rooms
	{
		//pick random number of rows and columns to make the room with
		uint8_t nRows = ( rand() % (7 - 5 + 1)) + 5;
		uint8_t nCols = ( rand() % (7 - 5 + 1)) + 5;

		generateRoomSegment(rOffset, cOffset, 0, nRows, nCols); 
		//record where our just drawn room ended, so we can get a map of our rooms
		 

		//add a random offset and prevent rooms being drawn at the edge
		rOffset +=( ( rand() % (10 - (-5) + 1) ) -5 );
		cOffset +=( ( rand() % (10 - (-5) + 1) ) -5 );
		rOffset  =  (rOffset% 30) + 1;
		cOffset  = (cOffset% 30) + 1;
	}
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
	return map[tilemapIdx]< 1;
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
	generateNewMap();
	set_bkg_tiles(0,0,defaultMapWidth,defaultMapHeight,map);
	
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