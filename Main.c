#include <gb/gb.h>
#include <stdio.h>
#include "TileData.c"
#include "BackgroundTiles.c"
#include "testmap.c"
#include "rand.h"

#define mapWidth 32
#define mapHeight 32
#define testmapBank 0

unsigned char map [1024];

BOOLEAN inBounds(int16_t* tile)
{
	return (*tile % 32!=0 && *tile % 33 !=0);
}

int16_t getNextValidMove(int16_t currentPos)
{
	int8_t options = 1;
	
	int16_t up = currentPos - mapWidth;
	int16_t down = currentPos + mapWidth;
	int8_t left = currentPos - 1;
	int8_t right = currentPos + 1;
	options = (up > 0 && inBounds(&up))? options+1 : options;
	options = (down < mapHeight && inBounds(&down))? options+1:  options;
	options = (left > 0 && inBounds(&left))? options+1 : options;
	options = (right < mapWidth && inBounds(&right))? options+1 : options;
	
	
	int8_t selection = (rand() % (options + 1 - 1)) + 1;
	switch (selection)
	{
	case 1:
		return - mapWidth;
	case 2:
		return mapWidth;
	case 3:
		return -1;
	case 4:
		return 1;
	default:
		return 0;
		
	}	
}

void generateNewMap()
{
	int16_t start = rand();
	int16_t end = rand();
	
	start = (start % (324 - 34 + 1)) + 34;
	end = (end % (991- 927 +  1)) + 927;
	start = start % 32 == 0 || start % 33 ==0 ? start + 2 : start;
	end = end % 32 == 0  || end % 33 == 0 ? end -2 : end;
	int16_t pos = start;

	for(int16_t i=0;i< sizeof(testmap)/sizeof(testmap[0]) ;i++)
	{
		map[i] = BackgroundTiles[1];
	}
	map[start] = BackgroundTiles[0];
	map[end] = BackgroundTiles[0];

	int8_t turns = 0;
	while(turns<100)
	{
		map[pos] = BackgroundTiles[0];
		pos += getNextValidMove(pos);
		
		//try a splash fill? then just go over the borders
		turns++;
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