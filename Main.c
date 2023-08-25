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
	//memcpy(&map[0],&testmap[0],sizeof(map)/2);
	//memcpy(&map[elements/2],&testmap2[(elements/2)+1],32);
	for(int8_t i=0; i< 32; i++)
	{
		int8_t r = (rand()% (no_maps - 1 + 1)) + 1;
		memcpy(&map[idx],&maps[r][idx],32);
		idx+=32;
	}
	

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