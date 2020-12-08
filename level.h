/*
 * level.h
 *
 * Created: 03/10/2019 13:14:25
 *  Author: PETN
 */ 


#ifndef LEVEL_H_
#define LEVEL_H_
#include <stdint.h>
#include "spritePos.h"
#include "waypoint.h"
typedef struct
{
	const uint16_t *pGameMap;		// the level map
	const uint16_t *pMetaTiles;	// the 2x2 metaTiles
	const uint16_t *pPreviewMap;
	const uint16_t *pPreviewMetaTiles;
	const spritePos_t * pInitialSpritePositions;
	const uint8_t *pTiles;		// the actual tiles used. This is copied into RAM when switching level
	const waypoint_t *firstWaypoint;
	const waypoint_t *lapWaypoint;			// this is the waypoint that is used to check lap time.
	const int8_t *pTileTypes;
	int32_t initialCarAngle;	
	uint16_t animTileStart;	// for animation/parallax tiles
	uint16_t maxTileIndex;
	uint16_t numerOfTilesToCopyInRam;		// the number of tiles to be copied in ram.
	uint16_t numberOfSpritePositions;		// the size of the initialSpritePositions
	uint16_t mapSizeX;		// in terms of metatiles (or tiles if not using metaTiles)
	uint16_t mapSizeY;		// in terms of metatiles (or tiles if not using metatiles)
	int16_t pixelSizeX;		// horizontal size in terms of pixel (precalculated to save time)
	int16_t pixelSizeY;		// horizontal size in terms of pixel (precalculated to save time)
	int8_t tileSizeX;		// horizontal metaTile size in terms of pixel (precalculated to save time)
	int8_t tileSizeY;		// horizontal metaTile size in terms of pixel (precalculated to save time)
	uint8_t useMetaTiles;	// 
	uint8_t parallaxEnabled;
	uint8_t numberOfAnimTileFrames;
	uint8_t animDelay;
} level_t;
extern level_t currentLevelData;
extern const level_t levels[];
extern uint8_t currentLevel;
void changeLevel(uint16_t levelNumber, uint8_t updateSprites);

#endif /* LEVEL_H_ */