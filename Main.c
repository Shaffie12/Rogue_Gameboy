#include <gb/gb.h>
#include <stdio.h>
#include "TileData.c"
#include "string.h"
#include "BackgroundTiles.c"
#include "testmap.c"
#include "testmap2.c"
#include "testmap3.c"
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
	for(int8_t i=0; i< 16; i++)
	{
		int8_t r = (rand()% (2 - 0 + 1)) + 0;
		memcpy(&map[idx],&maps[r][idx],64);
		idx+=64;
	} 

	//try randomising size of the chunks to load from mem. ie size of the loop
	

}

int main()
{

	SHOW_BKG;
	DISPLAY_ON; 
	set_bkg_data(0,13,TileData);
	initrand(DIV_REG);

	generateNewMap();
	set_bkg_tiles(0,0,testmapWidth,testmapHeight,map);

	while(1)
	{
		wait_vbl_done();
	
	}
	return 0;
	
}