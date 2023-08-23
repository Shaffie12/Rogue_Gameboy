#include <gb/gb.h>
#include <stdio.h>
#include "TileData.c"
#include "string.h"
#include "BackgroundTiles.c"
#include "testmap.c"
#include "testmap2.c"
#include "rand.h"

#define mapWidth 32
#define mapHeight 32
#define testmapBank 0

unsigned char map [1024];


void generateNewMap()
{

	memcpy(&map[0],&testmap[0],sizeof(map)/2); //data copies L to R use rows from different maps instead
	memcpy(&map[sizeof(map)/2],&testmap2[105],(sizeof(map)/2));
	//we can check the row above for a walkable tile.  then on the next line, we can verify that the row we will insert, has at least
	//1 tile in common
	
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