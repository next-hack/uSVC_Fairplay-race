/*
 * levels.h
 *
 * Created: 03/10/2019 14:14:09
 *  Author: PETN
 */ 


#ifndef LEVELS_H_
#define LEVELS_H_ 
#include "main.h"
level_t currentLevelData;
const level_t levels[] =
{
	// TODO: cleanup and separate level from terrain type... (an index to the terrain type would be enough)
	{
		.pGameMap = map_titleScreen,
		.mapSizeX = MAPSIZEX_TITLESCREEN,
		.mapSizeY = MAPSIZEY_TITLESCREEN,
		.pTiles = &tileData_titleScreen[0][0], // note change this if you are using a different tile set for each level!
		.numerOfTilesToCopyInRam = MAXTILEINDEX_TITLE_SCREEN, // note change this if you are using a different tile set for each level!
		.pMetaTiles = metaTiles_titleScreen,
		.pPreviewMap = previewMap_level1,
		.pPreviewMetaTiles = previewMetaTiles_level1,	
		.pixelSizeX = TILE_SIZE_X * MAPSIZEX_TITLESCREEN * 2,
		.pixelSizeY = TILE_SIZE_Y * MAPSIZEY_TITLESCREEN * 2,
		.tileSizeX = TILE_SIZE_X * 2,
		.tileSizeY = TILE_SIZE_Y * 2,
		.pInitialSpritePositions = NULL,
		.numberOfSpritePositions = 0,
		.useMetaTiles = 1,
		.firstWaypoint =NULL,
		.initialCarAngle =  0,
		.animTileStart = 0,
		.maxTileIndex = MAXTILEINDEX_TITLE_SCREEN,
		.pTileTypes = NULL,
		.lapWaypoint = NULL		// last 3 waypoints are used to determine the lap time checkpoints
	},
	{
		.pGameMap = gameMap_level1,
		.mapSizeX = MAPSIZEX_LEVEL1,
		.mapSizeY = MAPSIZEY_LEVEL1,
		.pTiles = &tileData_grass[0][0], // note change this if you are using a different tile set for each level!
		.numerOfTilesToCopyInRam = RAMTILES_GRASS, // note change this if you are using a different tile set for each level!
		.pMetaTiles = metaTiles_level1,
		.pPreviewMap = previewMap_level1,
		.pPreviewMetaTiles = previewMetaTiles_level1,
		.pixelSizeX = TILE_SIZE_X * MAPSIZEX_LEVEL1 * 2,
		.pixelSizeY = TILE_SIZE_Y * MAPSIZEY_LEVEL1 * 2,
		.tileSizeX = TILE_SIZE_X * 2,
		.tileSizeY = TILE_SIZE_Y * 2,
		.pInitialSpritePositions = &initialSpritePositions_level1[0],
		.numberOfSpritePositions = NUMBER_OF_SPRITES_LEVEL1,
		.useMetaTiles = 1,
		.firstWaypoint = &wp_level1[0],
		.initialCarAngle =  FIXED_PI / 2,
		.animTileStart = ANIM_START_GRASS,
		.maxTileIndex = MAXTILEINDEX_GRASS,
		.pTileTypes = &tileProps_grass[0],
		.lapWaypoint = &wp_level1[NUMBER_OF_WAYPOINTS_LEVEL1 - 3],		// last 3 waypoints are used to determine the lap time checkpoints
		.parallaxEnabled = 1,
		.numberOfAnimTileFrames = 6,
		.animDelay = 6,	
	},
	{
		.pGameMap = gameMap_level2,
		.mapSizeX = MAPSIZEX_LEVEL2,
		.mapSizeY = MAPSIZEY_LEVEL2,
		.pTiles = &tileData_sand[0][0], // note change this if you are using a different tile set for each level!
		.numerOfTilesToCopyInRam = RAMTILES_SAND, // note change this if you are using a different tile set for each level!
		.pMetaTiles = metaTiles_level2,
		.pPreviewMap = previewMap_level2,
		.pPreviewMetaTiles = previewMetaTiles_level2,		
		.pixelSizeX = TILE_SIZE_X * MAPSIZEX_LEVEL2 * 2,
		.pixelSizeY = TILE_SIZE_Y * MAPSIZEY_LEVEL2 * 2,
		.tileSizeX = TILE_SIZE_X * 2,
		.tileSizeY = TILE_SIZE_Y * 2,
		.pInitialSpritePositions = &initialSpritePositions_level2[0],
		.numberOfSpritePositions = NUMBER_OF_SPRITES_LEVEL2,
		.useMetaTiles = 1,
		.firstWaypoint = &wp_level2[0],
		.initialCarAngle =  FIXED_PI / 2,
		.animTileStart = ANIM_START_SAND,
		.maxTileIndex = MAXTILEINDEX_SAND,
		.pTileTypes = &tileProps_sand[0],
		.lapWaypoint = &wp_level2[NUMBER_OF_WAYPOINTS_LEVEL2 - 3],		// last 3 waypoints are used to determine the lap time checkpoints
		.parallaxEnabled = 0,
		.numberOfAnimTileFrames = 12,
		.animDelay = 6,	
	},
	{
		.pGameMap = gameMap_level3,
		.mapSizeX = MAPSIZEX_LEVEL3,
		.mapSizeY = MAPSIZEY_LEVEL3,
		.pTiles = &tileData_toxicRefinery[0][0], // note change this if you are using a different tile set for each level!
		.numerOfTilesToCopyInRam = RAMTILES_TOXIC_REFINERY, // note change this if you are using a different tile set for each level!
		.pMetaTiles = metaTiles_level3,
		.pPreviewMap = previewMap_level3,
		.pPreviewMetaTiles = previewMetaTiles_level3,
		.pixelSizeX = TILE_SIZE_X * MAPSIZEX_LEVEL3 * 2,
		.pixelSizeY = TILE_SIZE_Y * MAPSIZEY_LEVEL3 * 2,
		.tileSizeX = TILE_SIZE_X * 2,
		.tileSizeY = TILE_SIZE_Y * 2,
		.pInitialSpritePositions = &initialSpritePositions_level3[0],
		.numberOfSpritePositions = NUMBER_OF_SPRITES_LEVEL3,
		.useMetaTiles = 1,
		.firstWaypoint = &wp_level3[0],
		.initialCarAngle =  FIXED_PI / 2,
		.animTileStart = ANIM_START_TOXIC_REFINERY,
		.maxTileIndex = MAXTILEINDEX_TOXIC_REFINERY,
		.pTileTypes = &tileProps_toxicRefinery[0],
		.lapWaypoint = &wp_level3[NUMBER_OF_WAYPOINTS_LEVEL3 - 3],		// last 3 waypoints are used to determine the lap time checkpoints
		.parallaxEnabled = 1,
		.numberOfAnimTileFrames = 3,
		.animDelay = 6,
	}		
};
void changeLevel(uint16_t levelNumber, uint8_t updateSprites)
{
	
	// This operation might take a while. To avoid displaying rubbish we firstly clear a tile, then we clear VRAM
	memset(&tiles[0], 0, sizeof(tiles[0]));
	for (int i = 0; i < VRAMX * VRAMY; i++)
		vram[i] = (uint32_t) &tiles[0];
	memcpy(&currentLevelData, &levels[levelNumber], sizeof (currentLevelData));
	// now the screen is black
	currentLevel = levelNumber;
	videoData.ramTiles = levels[levelNumber].numerOfTilesToCopyInRam;
	memcpy(&tiles[0], levels[levelNumber].pTiles, videoData.ramTiles * sizeof(tiles[0]));
	drawMap(0, 0, 1);
	if (updateSprites)
	{
		initBonuses(currentLevelData.pInitialSpritePositions, currentLevelData.numberOfSpritePositions);
	}	
}



#endif /* LEVELS_H_ */