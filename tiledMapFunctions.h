/*
 * tiledMapFunctions.h
 *
 * Created: 04/11/2019 17:23:19
 *  Author: PETN
 */ 


#ifndef TILEDMAPFUNCTIONS_H_
#define TILEDMAPFUNCTIONS_H_
#include <stdint.h>
#include "level.h"
static inline uint16_t getMapTileFromTileCoordinates(uint16_t xTile,  uint16_t yTile)
{
	if (currentLevelData.useMetaTiles)
	{
		uint16_t metaNumber = currentLevelData.pGameMap[(xTile >> 1) + (yTile >> 1) * currentLevelData.mapSizeX ];
		return currentLevelData.pMetaTiles[metaNumber * 4 + (xTile & 1) + 2 * (yTile & 1)];
	}
	else
		return currentLevelData.pGameMap[xTile + yTile * currentLevelData.mapSizeX];
}
static inline uint16_t getMapTileFromTileCoordinatesNoMetaTiles(uint16_t xTile,  uint16_t yTile)
{
	return currentLevelData.pGameMap[xTile + yTile * currentLevelData.mapSizeX];
}
static inline uint16_t getMapTileFromTileCoordinatesWithMetaTiles(uint16_t xTile,  uint16_t yTile)
{
	uint16_t metaNumber = currentLevelData.pGameMap[(xTile >> 1) + (yTile >> 1) * currentLevelData.mapSizeX ];
	return currentLevelData.pMetaTiles[metaNumber * 4 + (xTile & 1) + 2 * (yTile & 1)];
}
//uint16_t getMapTileFromCoordinates(int16_t coordX, int16_t coordY);
uint16_t getMapTileFromTileCoordinates(uint16_t xTile,  uint16_t yTile);
int32_t getMapNumTileX();
int32_t getMapNumTileY();
void drawMap(uint16_t xOffset, uint16_t yOffset, uint8_t forceRedraw);



#endif /* TILEDMAPFUNCTIONS_H_ */