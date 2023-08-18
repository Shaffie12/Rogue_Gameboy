#include <gb/gb.h>
#include <stdio.h>
#include "TileData.c"
#include "BackgroundTiles.c"
#include "testmap.c"
#include "rand.h"

unsigned char map [1024];

void generateNewMap()
{
	size_t start = 20;
	size_t end = 335;
	start = (start % (512 - 0 + 1)) + 0;
	end = (end % (1024 - 800 +  1)) + 800;
	for(size_t i=0;i< sizeof(testmap)/sizeof(testmap[0]) ;i++)
	{
		map[i] = BackgroundTiles[1];
	}

	map[start] = BackgroundTiles[0];
	map[end] = BackgroundTiles[0];
	

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