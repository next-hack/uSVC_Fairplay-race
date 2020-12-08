/*
 * romTiles.h
 *
 * Created: 23/08/2019 12:59:06
 *  Author: PETN
 */ 


#ifndef ROMTILES_H_
#define ROMTILES_H_
#include <stdint.h>
#define MAXTILEINDEX_GRASS 304
#define MAXTILEINDEX_SAND 400
#define MAXTILEINDEX_TOXIC_REFINERY 246
#define MAXTILEINDEX_TITLE_SCREEN 307
#define ANIM_START_GRASS (208)	//	where the first water tile starts
#define ANIM_START_SAND (208)	//	where the first water tile starts
#define ANIM_START_TOXIC_REFINERY (198) 
#define RAMTILES_GRASS (ANIM_START_GRASS + 16) // 
#define RAMTILES_SAND (ANIM_START_SAND + 16) //
#define RAMTILES_TOXIC_REFINERY (ANIM_START_TOXIC_REFINERY + 16)
#define TILESIZEX 8
#define TILESIZEY 8
#define MAXTILEINDEX_MENU 18
extern const uint8_t tileData_menu[MAXTILEINDEX_MENU][TILESIZEX * TILESIZEY ];
extern const uint8_t tileData_titleScreen[MAXTILEINDEX_TITLE_SCREEN][TILESIZEX * TILESIZEY ];
extern const uint8_t tileData_grass[MAXTILEINDEX_GRASS][TILESIZEX * TILESIZEY ];
extern const uint8_t tileData_sand[MAXTILEINDEX_SAND][TILESIZEX * TILESIZEY ];
extern const uint8_t tileData_toxicRefinery[MAXTILEINDEX_TOXIC_REFINERY][TILESIZEX * TILESIZEY ];



#endif /* ROMTILES_H_ */