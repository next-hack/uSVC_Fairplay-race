/*
 * uchipPlay.h
 *
 * Created: 30/05/2018 20:39:49
 *  Author: PETN
 */ 


#ifndef MAIN_H_
#define MAIN_H_
#include "usvc_kernel/usvc_kernel.h"
#include "gameMap.h"
#include "romTiles.h"
#include "VGAspriteData.h"
#include "carPhysics.h"
#include "waypoint.h"
#include "fastApproxMath.h"
#include "waypointList.h"
#include "CarAI.h"
#include "tileTypes.h"
#include "soundData.h"
#include "soundWaveList.h"
#include "patches.h"	
#include "spritePos.h"
#include "projectilesAndBonuses.h"
#include "level.h"
#include "fixedSectionTileData.h"
#include "tiledMapFunctions.h"
uint16_t getFrameTicks(void);
#define NUM_CARS 4
// One car = 32 pixel, about 3.2 meter => one pixel is 0.1 meter. if v is the speed ( 57*v>>16 pixels are added to the object each second - there are 57 fps)
// Therefore if v = 0x10000, then the equivalent v is 5.7 m/s. The speed of sound is 340 m/s, i.e. 60*0x10000
#define SOUND_SPEED (15 << 16)		// it should be 60. But to enhance doppler effect, we will reduce the speed of sound :)
#define FULL_VOLUME_DOPPLER_SQUARE_DISTANCE (128 * 128)
#define CLOSEST_CAR_ENGINE_CHANNEL 1
#define OWN_CAR_ENGINE_CHANNEL 3
#define EXPLOSION_CHANNEL 2
#define PROJECTILE_CHANNEL 0
extern car_t cars[NUM_CARS];

extern uint32_t palette[16];
#endif /* MAIN_H_ */