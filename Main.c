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

size_t inBounds(size_t* tile)
{
	return (*tile % 32!=0 && *tile % 33 !=0);
}

size_t getNextValidMove(size_t currentPos)
{
	size_t options = 0;
	
	size_t up = currentPos - mapWidth;
	size_t down = currentPos + mapWidth;
	size_t left = currentPos - 1;
	size_t right = currentPos + 1;
	options = (up > 0 && inBounds(&up))? options++ : options;
	options = (down < mapHeight && inBounds(&down))? options++:  options;
	options = (left > 0 && inBounds(&left))? options++ : options;
	options = (right < mapWidth && inBounds(&right))? options++ : options;

	size_t selection = rand(); //not working
	selection = (selection % (options - 1 + 1)) + 1;
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
	size_t start = rand();
	size_t end = rand();
	size_t visited[sizeof(map)] = {0};
	
	
	start = (start % (324 - 34 + 1)) + 34;
	end = (end % (991- 927 +  1)) + 927;
	start = start % 32 == 0 || start % 33 ==0 ? start + 2 : start;
	end = end % 32 == 0  || end % 33 == 0 ? end -2 : end;
	size_t pos = start;

	for(size_t i=0;i< sizeof(testmap)/sizeof(testmap[0]) ;i++)
	{
		map[i] = BackgroundTiles[1];
	}
	map[start] = BackgroundTiles[0];
	map[end] = BackgroundTiles[0];
	
	//visited[start] = 1;
	size_t turns = 0;
	while(turns<10)
	{
		pos += getNextValidMove(pos);
		map[pos]=BackgroundTiles[0];
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