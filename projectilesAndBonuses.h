/*
 * projectiles.h
 *
 * Created: 30/09/2019 10:45:52
 *  Author: PETN
 */ 


#ifndef PROJECTILES_AND_BONUSES_H_
#define PROJECTILES_AND_BONUSES_H_
#include <stdint.h>
#include "spritePos.h"
#define BONUS_DECISION_TIME_FRAME 120U
#define MINE_ACTIVATION_TIME_FRAME 60U
#define BOMB_ACTIVATION_TIME_FRAME 60U
//
#define BONUS_NONE 0
#define BONUS_DEPLOY_REMOTE_BOMB 1
#define BONUS_DEPLOY_MINE 2
#define BONUS_REAR_ROCKET 3
#define BONUS_FRONT_ROCKET 4
#define BONUS_SPEED 5
#define REMOTE_BOMB_ACTIVATION 6
#define BONUS_SET_OFF_REMOTE_BOMB 7
#define BONUS_NOT_YET_DECIDED	8
//
//
#define ROCKET_SPEED 16
//
#define STATE_BONUS_NONE 0x0
#define STATE_BOMB_DEPLOYED 0x1
#define STATE_ROCKET_FLYING 0x2
#define STATE_MINE_DEPLOYED 0x3
#define STATE_MINE_ACTIVE 0x4
#define STATE_BOMB_ACTIVE 0x5
#define PROJECTILE_STATE_EXPLODING 0x80
// 
#define BONUS_STATE_AVAILABLE 0x00
#define BONUS_STATE_TAKEN 0x20
#define BONUS_STATE_DISAPPEARING 0x40
#define BONUS_STATE_REAPPEARING 0x60
//
#define MAX_INGAME_BONUS_NUMBER 16
#define MAX_ONSCREEN_BONUS_NUMBER 8
//
#define BONUS_FRAME_MASK 0x1F
#define BONUS_STATE_MASK 0xE0
#define BONUS_FRAME_TIME 6U
#define BONUS_FRAME_DISAPPEARING_TIME 0U		// this will make the animation to run at the full speed.
#define BONUS_REAPPEAR_TIME 1800U		// after 30 secs, the bonus will be online again
#define BONUS_ACTIVE_NUMBER_OF_FRAMES 3

//
typedef struct
{
	vector_t currentPosition;
	uint16_t frameTime;
	uint8_t spriteAngle;
	uint8_t carState;
	uint8_t frame;

} projectile_t;

typedef struct
{
	int16_t x;
	int16_t y;
	uint16_t frameTime;		// time in number of frames, when the last event occurred
	uint8_t type;
	uint8_t carState;
} bonus_t;
extern bonus_t bonuses[MAX_INGAME_BONUS_NUMBER];
uint32_t drawBonuses(int16_t cameraX, int16_t cameraY, uint8_t startSprite);
void bonusHandler(void);
void projectileHandler(void);
void initBonuses(const spritePos_t *spritePosition, uint16_t numberOfEntries);
void drawProjectiles(int16_t cameraX, int16_t cameraY, uint8_t *pNumberOfDrawnSprites, uint8_t *pNextTopSprite);


#endif /* PROJECTILES_AND_BONUSES_H_ */