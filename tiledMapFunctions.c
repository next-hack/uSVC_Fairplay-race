/*
 * tiledMapFunctions.c
 *
 * Created: 04/11/2019 17:23:43
 *  Author: PETN
 */ 
#include "usvc_kernel/usvc_kernel.h" 
#include "tiledMapFunctions.h"
#include "main.h"
int32_t getMapNumTileX()
{
	return levels[currentLevel].mapSizeX * (1 + (levels[currentLevel].useMetaTiles != 0));
}
int32_t getMapNumTileY()
{
	return levels[currentLevel].mapSizeY * (1 + (levels[currentLevel].useMetaTiles != 0));
}
#if GFX_MODE == TILE_MODE0 || GFX_MODE == TILE_MODE1

uint16_t getMapTileFromCoordinates(int16_t coordX, int16_t coordY)
{
	return getMapTileFromTileCoordinates(coordX >> LOG2_TILE_SIZE_X, coordY >> LOG2_TILE_SIZE_Y);
}
void drawMap(uint16_t xOffset, uint16_t yOffset, uint8_t forceRedraw)
{
	#if GFX_MODE == TILE_MODE1 || GFX_MODE == TILE_MODE2
	static uint16_t m_old_xStart = 0xFFFF;
	static uint16_t m_old_yStart = 0xFFFF;
	// first, restore the background tiles. The sprites will be redrawn later.

	restoreBackgroundTiles();
	// then copy the tiles required. 
	// we just need to copy part of the map in vram.	
	videoData.xScroll = xOffset & (2 * TILE_SIZE_X - 1);
	videoData.yScroll= yOffset & (TILE_SIZE_Y - 1);
	if (currentLevelData.pixelSizeX <= SCREEN_SIZE_X )
	{
		xOffset = 0;
	}
	else if (xOffset >=  (currentLevelData.pixelSizeX - SCREEN_SIZE_X ))
	{	// this is the last condition for the very last pixel.
		videoData.xScroll = 16;
		xOffset = currentLevelData.pixelSizeX - VRAMX * TILE_SIZE_X;
	}
	else if (xOffset >= (currentLevelData.pixelSizeX) - VRAMX * TILE_SIZE_X)
	{
		xOffset = currentLevelData.pixelSizeX - VRAMX * TILE_SIZE_X;
	}
	if (currentLevelData.pixelSizeY <= SCREEN_SIZE_Y )
	{
		yOffset = 0;
	}
	if (yOffset >= (currentLevelData.pixelSizeY - SCREEN_SIZE_Y ))
	{
		videoData.yScroll = 8;
		yOffset = currentLevelData.pixelSizeY - VRAMY * TILE_SIZE_Y;
	}
	else if (yOffset >= (currentLevelData.pixelSizeY ) - VRAMY * TILE_SIZE_Y)
	{
		yOffset = currentLevelData.pixelSizeY - VRAMY * TILE_SIZE_Y;
	}
	uint16_t xStart = (xOffset >> (LOG2_TILE_SIZE_X + 1)) * 2;
	uint16_t yStart = yOffset >> LOG2_TILE_SIZE_Y;
	if (m_old_xStart != xStart || m_old_yStart != yStart || forceRedraw)
	{
		#if	(GFX_MODE == TILE_MODE1 || GFX_MODE == TILE_MODE2) && ENABLE_TILE_PRIORITY
			int tilePos = 0;		
			uint32_t *pTilePriority =  (uint32_t*) &videoData.tilePriority[0];
			uint32_t tmpTilePri = 0;
		#endif
		if (currentLevelData.useMetaTiles)
		{
			uint16_t* pVram = &vram[0];
			for (uint32_t y = 0; y < VRAMY; y++)
			{
				uint32_t yPos =  y + yStart;
				uint32_t gameMapPos =  (yPos >> 1) * currentLevelData.mapSizeX;		
				for (uint32_t x = 0; x < VRAMX; )
				{
#if	(GFX_MODE == TILE_MODE1 || GFX_MODE == TILE_MODE2) && ENABLE_TILE_PRIORITY

					uint16_t metaNumber = currentLevelData.pGameMap[gameMapPos + ((x + xStart) >> 1)];
					uint16_t tile =  currentLevelData.pMetaTiles[metaNumber * 4 + ((x + xStart) & 1) + 2 * (yPos & 1)];
					// note: since vrram only holds a 16-bit number, and since each tile si 64 bytes long, even if there is the tile priority bit (bit 14)
					// set at 1, this would add a 1048576, which will be discarded later. Therefore we do not perform tile & TILEMASK.					
					*pVram++ = (uint32_t) &tiles[tile][0];
					// NOTE!!! In this game there are no "Transparent tiles" so we can save one &1
					tmpTilePri |= (( tile >> 14) << tilePos);
					tilePos++;  // This is the odd pixel. For sure we won't overflow!
					x++;
					tile =  currentLevelData.pMetaTiles[metaNumber * 4 + ((x + xStart) & 1) + 2 * (yPos & 1)];
					// note: since vrram only holds a 16-bit number, and since each tile si 64 bytes long, even if there is the tile priority bit (bit 14)
					// set at 1, this would add a 1048576, which will be discarded later. Therefore we do not perform tile & TILEMASK.
					*pVram++ = (uint32_t) &tiles[tile][0];
					// NOTE!!! In this game there are no "Transparent tiles" so we can save one &1
					tmpTilePri |= (( tile >> 14) << tilePos);
					tilePos++;  // This is the odd pixel. For sure we won't overflow!
					x++;
					if (tilePos == 32)
					{
						tilePos = 0;
						*pTilePriority++ = tmpTilePri;
						tmpTilePri = 0;
					}
#else
					int16_t metaNumber = currentLevelData.pGameMap[gameMapPos + ((x + xStart) >> 1)];
					uint16_t tile =  currentLevelData.pMetaTiles[metaNumber * 4 + ((x + xStart) & 1) + 2 * (yPos & 1)];
					// note: since vrram only holds a 16-bit number, and since each tile si 64 bytes long, even if there is the tile priority bit (bit 14)
					// set at 1, this would add a 1048576, which will be discarded later. Therefore we do not perform tile & TILEMASK.
					*pVram++ = (uint32_t) &tiles[tile][0];
					// NOTE!!! In this game there are no "Transparent tiles" so we can save one &1
					x++;
					tile =  currentLevelData.pMetaTiles[metaNumber * 4 + ((x + xStart) & 1) + 2 * (yPos & 1)];
					// note: since vrram only holds a 16-bit number, and since each tile si 64 bytes long, even if there is the tile priority bit (bit 14)
					// set at 1, this would add a 1048576, which will be discarded later. Therefore we do not perform tile & TILEMASK.
					*pVram++ = (uint32_t) &tiles[tile][0];
					// NOTE!!! In this game there are no "Transparent tiles" so we can save one &1
					x++;
#endif
				}
			}			
		}
		else
		{
			for (int y = 0; y < VRAMY; y++)
			{
				for (int x = 0; x < VRAMX; x++)
				{
					#if	(GFX_MODE == TILE_MODE1 || GFX_MODE == TILE_MODE2) && ENABLE_TILE_PRIORITY
					uint16_t tile =  getMapTileFromTileCoordinatesNoMetaTiles(x + xStart, y + yStart);
					placeTile(x, y, tile);		// inline function. Optimized!. tile contains also bits 14 and 15 which might be at 1, but these won't cause any problems. (The address is stored in 16 bit format and each tile is 64 bytes long.
					//
					tmpTilePri |= ((( tile >> 14) & 1) << tilePos);
					tilePos++;
					if (tilePos == 8)
					{
						tilePos = 0;
						*pTilePriority++ = tmpTilePri;
						tmpTilePri = 0;
					}
					#else
						placeTile(x, y, getMapTileFromTileCoordinatesNoMetaTiles(x + xStart, y + yStart));
					#endif
				}			
			}	
		}
		#if	(GFX_MODE == TILE_MODE1 || GFX_MODE == TILE_MODE2) && ENABLE_TILE_PRIORITY
			// store last tilePri word if it was not complete
			if (tilePos != 0)
			{
				*pTilePriority++ = tmpTilePri;				
			}
		#endif
		m_old_xStart = xStart;
		m_old_yStart = yStart;
	}
	#endif
}
#endif
