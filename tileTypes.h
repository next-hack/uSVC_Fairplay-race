/*
 * tileTypes.h
 *
 * Created: 17/09/2019 16:54:38
 *  Author: PETN
 */ 


#ifndef TILETYPES_H_
#define TILETYPES_H_

#include <stdint.h>
#define TILEPROP_NORMAL 0
#define TILEPROP_SOLID 1
#define TILEPROP_SLOW 2
extern const int8_t tileProps_grass[];
extern const int8_t tileProps_sand[];
extern const int8_t tileProps_toxicRefinery[];

#endif /* TILETYPES_H_ */