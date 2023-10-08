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

	//SHOW_BKG;
	SHOW_SPRITES;
	SPRITES_8x16;
	DISPLAY_ON; 

	//set_bkg_data(0,tileData_NUM_TILES,TileData);
	set_sprite_data(0,char_NUM_ILES,character);
	initrand(DIV_REG);

	//generateNewMap();
	//set_bkg_tiles(0,0,testmapWidth,testmapHeight,map);
	set_sprite_tile(0,0);
	set_sprite_tile(1,2);
	move_sprite(0,80,80);
	move_sprite(1,88,80);
		

	

	while(1)
	{
		wait_vbl_done();
	
	}
	return 0;
	
}