/*
 *  uChip Fairplay Race: race against your opponents. Or kill them. 
 *
 *  Copyright 2019 Nicola Wrachien (next-hack.com)
 *
 *
 *  This file is part of next-hack's uChip Fairplay Race.
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *	
 *  tl;dr 
 *  Do whatever you want, this program is free! Though we won't 
 *  reject donations to https://www.paypal.me/nexthack :)
*/
#ifndef SPRITEPOS_H_
#define SPRITEPOS_H_

#include <stdint.h>
#define NUMBER_OF_SPRITES_LEVEL1 13
#define NUMBER_OF_SPRITES_LEVEL2 17
#define NUMBER_OF_SPRITES_LEVEL3 15
typedef struct
{
	uint16_t posX;
	uint16_t posY;
	uint8_t entityNumber;
} spritePos_t;
extern const spritePos_t initialSpritePositions_level1[NUMBER_OF_SPRITES_LEVEL1];
extern const spritePos_t initialSpritePositions_level2[NUMBER_OF_SPRITES_LEVEL2];
extern const spritePos_t initialSpritePositions_level3[NUMBER_OF_SPRITES_LEVEL3];
#endif /* SPRITEPOS_H_ */