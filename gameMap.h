/*
 * gameMap.h
 *
 * Created: 01/06/2018 11:20:56
 *  Author: PETN
 */ 


#ifndef GAMEMAP_H_
#define GAMEMAP_H_

/*
#define MAPSIZEX  100
#define MAPSIZEY  103
extern const uint16_t gameMap[MAPSIZEY][MAPSIZEX];
*/
#define MAPSIZEX_LEVEL1 50
#define MAPSIZEY_LEVEL1 52
#define NUMBER_OF_METATILES_LEVEL1 506
#define NUMBER_OF_PREVIEW_METATILES_LEVEL1 20
#define MAPSIZEX_LEVEL2 50
#define MAPSIZEY_LEVEL2 50
#define NUMBER_OF_METATILES_LEVEL2 548
#define NUMBER_OF_PREVIEW_METATILES_LEVEL2 20
#define MAPSIZEX_LEVEL3 50
#define MAPSIZEY_LEVEL3 50
#define NUMBER_OF_METATILES_LEVEL3 664
#define NUMBER_OF_PREVEW_METATILES_LEVEL3 29
#define PREVIEW_MAPSIZEX 5
#define PREVIEW_MAPSIZEY 5

#define MAPSIZEX_TITLESCREEN 20
#define MAPSIZEY_TITLESCREEN 13
#define NUMBER_OF_METATILES_TITLESCREEN 104
extern const uint16_t map_titleScreen[MAPSIZEY_TITLESCREEN * MAPSIZEX_TITLESCREEN + 1];
extern const uint16_t metaTiles_titleScreen[4 * NUMBER_OF_METATILES_TITLESCREEN];
extern const uint16_t metaTiles_level1[4 * NUMBER_OF_METATILES_LEVEL1];
extern const uint16_t gameMap_level1[MAPSIZEY_LEVEL1 * MAPSIZEX_LEVEL1];
extern const uint16_t metaTiles_level2[4 * NUMBER_OF_METATILES_LEVEL2];
extern const uint16_t gameMap_level2[MAPSIZEY_LEVEL2 * MAPSIZEX_LEVEL2];
extern const uint16_t gameMap_level3[MAPSIZEY_LEVEL3 * MAPSIZEX_LEVEL3];
extern const uint16_t metaTiles_level3[4 * NUMBER_OF_METATILES_LEVEL3];
extern const uint16_t previewMetaTiles_level1[4 * NUMBER_OF_PREVIEW_METATILES_LEVEL1];
extern const uint16_t previewMap_level1[PREVIEW_MAPSIZEY * PREVIEW_MAPSIZEX];
extern const uint16_t previewMetaTiles_level2[4 * NUMBER_OF_PREVIEW_METATILES_LEVEL2];
extern const uint16_t previewMap_level2[PREVIEW_MAPSIZEY * PREVIEW_MAPSIZEX];
extern const uint16_t previewMetaTiles_level3[4 * NUMBER_OF_PREVEW_METATILES_LEVEL3];
extern const uint16_t previewMap_level3[PREVIEW_MAPSIZEY * PREVIEW_MAPSIZEX];

#endif /* GAMEMAP_H_ */