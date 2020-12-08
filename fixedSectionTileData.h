/*
 * fixedSectionTiles.h
 *
 * Created: 04/10/2019 13:02:36
 *  Author: PETN
 */ 


#ifndef FIXEDSECTIONTILEDATA_H_
#define FIXEDSECTIONTILEDATA_H_
#include <stdint.h>
#define MAXFIXEDTILEINDEX 11
#define FIXED_SECTION_MAPSIZEX 40
#define FIXED_SECTION_MAPSIZEY 1
#define MAX_FIXED_SECTION_TILES (16 + 9)
extern uint32_t fixedSectionTiles[MAX_FIXED_SECTION_TILES][16] __attribute__ ((aligned (4)));
extern uint8_t fixedSectionMap[FIXED_SECTION_MAPSIZEX * FIXED_SECTION_MAPSIZEY]  __attribute__ ((aligned (4)));
extern const uint8_t fixedSectionTileData[MAXFIXEDTILEINDEX][64];
#define SYMBOL_NOBONUS 0
#define SYMBOL_DEPLOY_REMOTE_BOMB 1
#define SYMBOL_DEPLOY_MINE 2
#define SYMBOL_REAR_ROCKET 3
#define SYMBOL_FRONT_ROCKET 4
#define SYMBOL_SPEED 5
#define SYMBOL_REMOTE_BOMB_INACTIVE 6
#define SYMBOL_SET_OFF_REMOTE_BOMB 7
#define SYMBOL_UNKNOWN_BONUS 8
#define SYMBOL_KILLS 9
#define SYMBOL_DEATHS 10


#endif /* FIXEDSECTIONTILEDATA_H_ */