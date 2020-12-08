/*
 * projectiles.c
 *
 * Created: 30/09/2019 10:45:25
 *  Author: PETN
 */ 
/*	Every car can hold at most one bonus. When the car fires/activates the bonus, then it can grab another one.
	THe bonus might be:
	1) Speed bonus
	2) Shield
	3) Random Bonus, which might become any random weapon:
		a) Front rocket
		b) rear rocket
		c) mine
		d) remote bomb
	Some projectiles are stationary and last until either someone collide with them, or they are remotely set off. However, if a player takes another bonus, then any already deployed projectile will be instantly set off.
	Other projectiles have only a limited time before they explode/extinguish.
*/
#include "main.h" 
#define SPEED_BONUS_NUMBER_OF_FRAMES 3
#define NUMBER_OF_ACTIVE_MINE_FRAMES 8
#define NUMBER_OF_ROCKET_FRAMES 2
#define EXPLOSION_ANIMATION_TIME 2
#define MINE_ACTIVE_ANIMATION_TIME 4
#define ROCKET_ANIMATION_TIME 2
void fireBonus(car_t *pCar);
uint16_t numberOfBonuses;
bonus_t bonuses[MAX_INGAME_BONUS_NUMBER];
const uint8_t randomBonusFrames[] = {0, 1, 2, 1};
uint32_t drawBonuses(int16_t cameraX, int16_t cameraY, uint8_t startSprite)
{
	uint32_t numberOfDrawnSprites = 0;
	for (int i = 0; i < numberOfBonuses; i++)
	{
		if ( (bonuses[i].carState & BONUS_STATE_MASK) == BONUS_STATE_AVAILABLE)
		{
			if (bonuses[i].type == BONUSRANDOM)
			{
				numberOfDrawnSprites += putSprite(numberOfDrawnSprites + startSprite, bonuses[i].x - cameraX, bonuses[i].y - cameraY, SPRITE_FLAGS_HANDLE_CENTER, /*(entityData[bonuses[i].type])[0].frameIndex */animData[entityAnimStartIndex[bonuses[i].type]].frameIndex + randomBonusFrames[(bonuses[i].carState & BONUS_FRAME_MASK)]);
			}
			else
			{
				numberOfDrawnSprites += putSprite(numberOfDrawnSprites + startSprite, bonuses[i].x - cameraX, bonuses[i].y - cameraY, SPRITE_FLAGS_HANDLE_CENTER, /*(entityData[bonuses[i].type])[0].frameIndex */animData[entityAnimStartIndex[bonuses[i].type]].frameIndex + (bonuses[i].carState & BONUS_FRAME_MASK));
			}
		}
		else if ( (bonuses[i].carState & BONUS_STATE_MASK) == BONUS_STATE_DISAPPEARING || (bonuses[i].carState & BONUS_STATE_MASK) == BONUS_STATE_REAPPEARING)
			numberOfDrawnSprites += putSprite(numberOfDrawnSprites + startSprite, bonuses[i].x - cameraX, bonuses[i].y - cameraY, SPRITE_FLAGS_HANDLE_CENTER, BONUSTAKEN_DEFAULTANIM_FRAMEINDEX + (bonuses[i].carState & BONUS_FRAME_MASK));
	}
	return numberOfDrawnSprites;	
};
void initBonuses(const spritePos_t *spritePosition, uint16_t numberOfEntries)
{
	numberOfBonuses = 0;
	for (int i = 0; i < numberOfEntries; i++)
	{
		const spritePos_t *pSpritePos = &spritePosition[i];
		if (pSpritePos->entityNumber == BONUSRANDOM || pSpritePos->entityNumber == BONUSSPEED)
		{
			if (numberOfBonuses < MAX_INGAME_BONUS_NUMBER)
			{
				bonuses[numberOfBonuses].carState = BONUS_STATE_AVAILABLE;
				bonuses[numberOfBonuses].frameTime = getFrameTicks();
				bonuses[numberOfBonuses].type = pSpritePos->entityNumber;
				bonuses[numberOfBonuses].x = pSpritePos->posX;
				bonuses[numberOfBonuses].y = pSpritePos->posY;
				numberOfBonuses++;
			}
			else
				break;		// ignore the rest of the bonus list
		}
	}
}

void bonusHandler(void)
{
	uint32_t frameTimeNow = getFrameTicks();
	for (int i = 0; i < numberOfBonuses; i++)
	{
		uint8_t frameNum = bonuses[i].carState & BONUS_FRAME_MASK;
		if ((bonuses[i].carState & BONUS_STATE_MASK) == BONUS_STATE_AVAILABLE)
		{
			// animate
			if ((frameTimeNow - bonuses[i].frameTime) > BONUS_FRAME_TIME)
			{
				frameNum++;
				if ((bonuses[i].type == BONUSSPEED && (frameNum >= SPEED_BONUS_NUMBER_OF_FRAMES)) || (bonuses[i].type == BONUSRANDOM && (frameNum >= sizeof(randomBonusFrames))))
				{
					frameNum = 0;
				}
				bonuses[i].frameTime = frameTimeNow;
				bonuses[i].carState = (bonuses[i].carState & ~BONUS_FRAME_MASK) | frameNum;
			}
			// let's see if instead it is taken by some one
			for (int c = 0; c < NUM_CARS; c++)
			{
				if (cars[c].bonusType != BONUS_NONE)		// if the car already has a bonus, then it cannot take another one!
					continue;
				int32_t x = cars[c].currentPosition.x >> 16;
				int32_t y = cars[c].currentPosition.y >> 16;
				// to check if a bonus has been taken, we do not use pixel-exact collision. We use only a rough distance estimation.
				if ( ( (x - bonuses[i].x) * (x - bonuses[i].x) + (y - bonuses[i].y)*(y - bonuses[i].y)) < (5+8)*(5+8))
				{
					bonuses[i].carState = BONUS_STATE_DISAPPEARING;
					bonuses[i].frameTime = frameTimeNow;
					if (bonuses[i].type == BONUSRANDOM)
						cars[c].bonusType = BONUS_NOT_YET_DECIDED;
					else
						cars[c].bonusType = BONUS_SPEED;					
					cars[c].bonusFrameTime = frameTimeNow;
					// play sound
					if (c == 0)
						triggerNote(PROJECTILE_CHANNEL, 10, 54, 128);
					break;		// only one car can get a bonus!
				}
			}
		}
		else if ((bonuses[i].carState & BONUS_STATE_MASK) == BONUS_STATE_DISAPPEARING)
		{
			if ((frameTimeNow - bonuses[i].frameTime) > BONUS_FRAME_DISAPPEARING_TIME)
			{
				frameNum++;
				if (frameNum >= BONUSTAKEN_DEFAULTANIM_NUMFRAMES)
				{
					bonuses[i].carState = BONUS_STATE_TAKEN;
				}
				else
				bonuses[i].carState = (bonuses[i].carState & ~BONUS_FRAME_MASK) | frameNum;
				bonuses[i].frameTime = frameTimeNow;

			}
		}
		else if ((bonuses[i].carState & BONUS_STATE_MASK) == BONUS_STATE_TAKEN)
		{
			if (frameTimeNow - bonuses[i].frameTime > BONUS_REAPPEAR_TIME)
			{
				bonuses[i].frameTime = frameTimeNow;
				bonuses[i].carState = BONUS_STATE_REAPPEARING | (BONUSTAKEN_DEFAULTANIM_NUMFRAMES - 1);
			}
		}
		else if ((bonuses[i].carState & BONUS_STATE_MASK) == BONUS_STATE_REAPPEARING)
		{
			if ((frameTimeNow - bonuses[i].frameTime) > BONUS_FRAME_DISAPPEARING_TIME)
			{
				frameNum--;
				if ((frameNum  == 255))
				{
					bonuses[i].carState = BONUS_STATE_AVAILABLE;
				}
				else
				bonuses[i].carState = (bonuses[i].carState & ~BONUS_FRAME_MASK) | frameNum;
				bonuses[i].frameTime = frameTimeNow;
			}
		}
	}
	// 
	for (int c = 0; c < NUM_CARS; c++)
	{
		if (cars[c].bonusType == BONUS_NOT_YET_DECIDED && (frameTimeNow - cars[c].bonusFrameTime) > BONUS_DECISION_TIME_FRAME)		// if the car already has a bonus, then it cannot take another one!
			cars[c].bonusType = 1 + (rand() & 0x3);
		else if (cars[c].bonusType == REMOTE_BOMB_ACTIVATION && (frameTimeNow - cars[c].bonusFrameTime) > BOMB_ACTIVATION_TIME_FRAME)
			cars[c].bonusType = BONUS_SET_OFF_REMOTE_BOMB;
	}
}
const uint8_t rocketFrames [32] =
{
	ROCKET_DEFAULTANIM_FRAMEINDEX + 0, ROCKET_DEFAULTANIM_FRAMEINDEX + 2, ROCKET_DEFAULTANIM_FRAMEINDEX + 4, ROCKET_DEFAULTANIM_FRAMEINDEX + 6, ROCKET_DEFAULTANIM_FRAMEINDEX + 8,   // 0 up to 45 ° frame
	ROCKET_DEFAULTANIM_FRAMEINDEX + 6 , ROCKET_DEFAULTANIM_FRAMEINDEX + 4, ROCKET_DEFAULTANIM_FRAMEINDEX + 2,	     // FLAG_INVERTXY180
	ROCKET_DEFAULTANIM_FRAMEINDEX + 0, ROCKET_DEFAULTANIM_FRAMEINDEX + 2, ROCKET_DEFAULTANIM_FRAMEINDEX + 4, ROCKET_DEFAULTANIM_FRAMEINDEX + 6, ROCKET_DEFAULTANIM_FRAMEINDEX + 8,	 // FLAG_ROTATE_270
	ROCKET_DEFAULTANIM_FRAMEINDEX + 6, ROCKET_DEFAULTANIM_FRAMEINDEX + 4, ROCKET_DEFAULTANIM_FRAMEINDEX + 2, ROCKET_DEFAULTANIM_FRAMEINDEX + 0,		 // flip V
	ROCKET_DEFAULTANIM_FRAMEINDEX + 2, ROCKET_DEFAULTANIM_FRAMEINDEX + 4, ROCKET_DEFAULTANIM_FRAMEINDEX + 6, ROCKET_DEFAULTANIM_FRAMEINDEX + 8,		 // FLAG_ROTATE_180
	ROCKET_DEFAULTANIM_FRAMEINDEX + 6, ROCKET_DEFAULTANIM_FRAMEINDEX + 4, ROCKET_DEFAULTANIM_FRAMEINDEX + 2,		 // FLAG_INVERTXY
	ROCKET_DEFAULTANIM_FRAMEINDEX + 0, ROCKET_DEFAULTANIM_FRAMEINDEX + 2, ROCKET_DEFAULTANIM_FRAMEINDEX + 4, ROCKET_DEFAULTANIM_FRAMEINDEX + 6, ROCKET_DEFAULTANIM_FRAMEINDEX + 8,	 // FLAG_ROTATE_90
	ROCKET_DEFAULTANIM_FRAMEINDEX + 6, ROCKET_DEFAULTANIM_FRAMEINDEX + 4, ROCKET_DEFAULTANIM_FRAMEINDEX + 2			 // flip H
};
const uint8_t rocketRotFlags[32] =
{
	0, 0, 0, 0, 0,
	SPRITE_FLAGS_INVERTXY180, SPRITE_FLAGS_INVERTXY180, SPRITE_FLAGS_INVERTXY180,
	SPRITE_FLAGS_ROTATE_270, SPRITE_FLAGS_ROTATE_270, SPRITE_FLAGS_ROTATE_270, SPRITE_FLAGS_ROTATE_270, SPRITE_FLAGS_ROTATE_270,
	SPRITE_FLAGS_FLIP_VERTICAL, SPRITE_FLAGS_FLIP_VERTICAL, SPRITE_FLAGS_FLIP_VERTICAL, SPRITE_FLAGS_FLIP_VERTICAL,
	SPRITE_FLAGS_ROTATE_180, SPRITE_FLAGS_ROTATE_180, SPRITE_FLAGS_ROTATE_180, SPRITE_FLAGS_ROTATE_180,
	SPRITE_FLAGS_INVERTXY, SPRITE_FLAGS_INVERTXY, SPRITE_FLAGS_INVERTXY,
	SPRITE_FLAGS_ROTATE_90, SPRITE_FLAGS_ROTATE_90, SPRITE_FLAGS_ROTATE_90, SPRITE_FLAGS_ROTATE_90, SPRITE_FLAGS_ROTATE_90,
	SPRITE_FLAGS_FLIP_HORIZONTAL, SPRITE_FLAGS_FLIP_HORIZONTAL, SPRITE_FLAGS_FLIP_HORIZONTAL
};
const uint8_t mineFrames[8] = 
{ 
	MINE_DEFAULTANIM_FRAMEINDEX, MINE_DEFAULTANIM_FRAMEINDEX + 1, MINE_DEFAULTANIM_FRAMEINDEX, MINE_DEFAULTANIM_FRAMEINDEX + 1, 
	MINE_DEFAULTANIM_FRAMEINDEX, MINE_DEFAULTANIM_FRAMEINDEX + 1, MINE_DEFAULTANIM_FRAMEINDEX, MINE_DEFAULTANIM_FRAMEINDEX + 1 
};
const uint8_t mineRotFlags[8] = 
{
	0, 0, SPRITE_FLAGS_FLIP_HORIZONTAL, SPRITE_FLAGS_ROTATE_270, SPRITE_FLAGS_ROTATE_180, SPRITE_FLAGS_FLIP_VERTICAL, SPRITE_FLAGS_ROTATE_90, SPRITE_FLAGS_ROTATE_90
};
void drawProjectiles(int16_t cameraX, int16_t cameraY, uint8_t *pNumberOfDrawnSprites, uint8_t *pNextTopSprite)
{
	for (int c = 0; c < NUM_CARS; c++)
	{
		projectile_t *pProjectile = &cars[c].projectile[0];
		if (pProjectile->carState != STATE_BONUS_NONE)
		{
			uint8_t state = pProjectile->carState;
			uint8_t frame = pProjectile->frame;
			int16_t sx = (pProjectile->currentPosition.x >> 16) - cameraX;
			int16_t sy = (pProjectile->currentPosition.y >> 16) - cameraY;
			if (state & PROJECTILE_STATE_EXPLODING)
			{
				*pNextTopSprite -= putSprite(*pNextTopSprite,sx ,sy , SPRITE_FLAGS_HANDLE_CENTER | SPRITE_FLAGS_TRANSPARENT_SPRITE,  EXPLOSION_DEFAULTANIM_FRAMEINDEX + frame);
			}
			else if (state == STATE_ROCKET_FLYING)
			{
				uint8_t spriteAngle = pProjectile->spriteAngle;
				*pNextTopSprite -= putSprite(*pNextTopSprite,sx ,sy , SPRITE_FLAGS_HANDLE_CENTER | rocketRotFlags[spriteAngle], rocketFrames[spriteAngle] + frame);
			}
			else if (state == STATE_MINE_DEPLOYED)
			{
				*pNumberOfDrawnSprites += putSprite(*pNumberOfDrawnSprites,sx ,sy , SPRITE_FLAGS_HANDLE_CENTER, MINEINACTIVE_DEFAULTANIM_FRAMEINDEX);
			}
			else if (state == STATE_BOMB_DEPLOYED || state == STATE_BOMB_ACTIVE)
			{
				*pNumberOfDrawnSprites += putSprite(*pNumberOfDrawnSprites,sx ,sy , SPRITE_FLAGS_HANDLE_CENTER, REMOTEBOMB_DEFAULTANIM_FRAMEINDEX);
			}			
			else if (state == STATE_MINE_ACTIVE)
			{
				*pNumberOfDrawnSprites += putSprite(*pNumberOfDrawnSprites,sx ,sy , SPRITE_FLAGS_HANDLE_CENTER | mineRotFlags[frame], mineFrames[frame]);
			}

		}
	}
}
void projectileHandler(void)
{
	uint16_t frameTimeNow = getFrameTicks();
	for (int c = 0; c < NUM_CARS; c++)
	{
		projectile_t *pProjectile = &cars[c].projectile[0];
		// has the car some active projectile?
		if (pProjectile->carState != STATE_BONUS_NONE)
		{			
			uint8_t state = pProjectile->carState;
			uint8_t frame = pProjectile->frame; 
			if ( state == STATE_MINE_DEPLOYED && (frameTimeNow - pProjectile->frameTime) > MINE_ACTIVATION_TIME_FRAME)
			{
				pProjectile->carState = STATE_MINE_ACTIVE;
			}
			else if ( state == STATE_BOMB_DEPLOYED && (frameTimeNow - pProjectile->frameTime) > BOMB_ACTIVATION_TIME_FRAME)
			{
				pProjectile->carState = STATE_BOMB_ACTIVE;
			}			
			else if (state == STATE_MINE_ACTIVE)
			{

				if ((frameTimeNow - pProjectile->frameTime) > MINE_ACTIVE_ANIMATION_TIME)
				{						
					frame++;
					pProjectile->frameTime = frameTimeNow;
					if (frame >= NUMBER_OF_ACTIVE_MINE_FRAMES)
						frame = 0;
				}
			}
			else if (state & PROJECTILE_STATE_EXPLODING)
			{
				if ((frameTimeNow - pProjectile->frameTime) > EXPLOSION_ANIMATION_TIME)
				{
					frame++;
					pProjectile->frameTime = frameTimeNow;
					if (frame >= EXPLOSION_DEFAULTANIM_NUMFRAMES)
					{
						pProjectile->carState = STATE_BONUS_NONE;
					}
				}
			}
			else if (state == STATE_ROCKET_FLYING)
			{
				if ((frameTimeNow - pProjectile->frameTime) > ROCKET_ANIMATION_TIME)
				{
					frame++;
					pProjectile->frameTime = frameTimeNow;
					if (frame >= NUMBER_OF_ROCKET_FRAMES)
						frame = 0;
				}
				pProjectile->currentPosition.x += ROCKET_SPEED * sinTable[pProjectile->spriteAngle * (NUMBER_OF_SINTABLE_ENTRIES / CAR_SPRITE_DIRECTIONS)];
				pProjectile->currentPosition.y += -ROCKET_SPEED * sinTable[ (pProjectile->spriteAngle * (NUMBER_OF_SINTABLE_ENTRIES / CAR_SPRITE_DIRECTIONS) + NUMBER_OF_SINTABLE_ENTRIES / 4) & SINTABLE_ENTRY_MASK];
				int x = pProjectile->currentPosition.x >> 16;
				int y = pProjectile->currentPosition.y >> 16;
				// rocket explodes close to the screen edges, and close to the walls
				if (x < 0 || y < 0 || x >= levels[currentLevel].pixelSizeX || y >= levels[currentLevel].pixelSizeY || (currentLevelData.pTileTypes[TILEMASK & getMapTileFromTileCoordinates(x >> LOG2_TILE_SIZE_X, y >> LOG2_TILE_SIZE_Y) ] == TILEPROP_SOLID))
				{
					pProjectile->carState |= PROJECTILE_STATE_EXPLODING;				
					triggerNote(2, 5, 43 , 255);
				}
			}
			// now check for collisions
			if (state == STATE_ROCKET_FLYING || state == STATE_MINE_ACTIVE || state == STATE_BOMB_ACTIVE || (state & PROJECTILE_STATE_EXPLODING) )
			{
				for (int cc = 0; cc < NUM_CARS; cc++)
				{
					if (cars[c].carState != CAR_STATE_RUNNING)		// a car which is not running is not damaged
						continue;
					int32_t x = cars[cc].currentPosition.x >> 16;
					int32_t y = cars[cc].currentPosition.y >> 16;
					int32_t xp = pProjectile->currentPosition.x >> 16;
					int32_t yp = pProjectile->currentPosition.y >> 16;
					int32_t distanceQ = (x - xp) * (x - xp) + (y - yp)*(y - yp); 
					// to check if a bonus has been taken, we do not use pixel-exact collision. We use only a rough distance estimation.
					if ( distanceQ < (256 * (1 +  (STATE_BOMB_ACTIVE == (state & ~PROJECTILE_STATE_EXPLODING) ) ) ) && !( ( (state & ~PROJECTILE_STATE_EXPLODING) == STATE_ROCKET_FLYING) && c == cc))
					{
						
						if (state == STATE_BOMB_ACTIVE)		// if the bomb exploded against someone, remove it from the owner's inventory
							cars[c].bonusType = BONUS_NONE;
						#define FRAME_DANGEROUS_EXPLOSION 12		
						if ( (!(state & PROJECTILE_STATE_EXPLODING) || pProjectile->frame < FRAME_DANGEROUS_EXPLOSION) && (cars[cc].carState == CAR_STATE_RUNNING))		// only the first part of the explosion is capable of killing a car
						{
							if (cc == 0)
								stopLoopingFx(OWN_CAR_ENGINE_CHANNEL, 1);
							killCar(&cars[cc], c);
							triggerNote(2, 5, 25 , 255);
						}
						if (! (state & PROJECTILE_STATE_EXPLODING))
						{
							pProjectile->carState |= PROJECTILE_STATE_EXPLODING;
							pProjectile->frame = 0;
							pProjectile->frameTime = frameTimeNow;						
						}
						//break;		
					}
				}
				
			}
			pProjectile->frame = frame;		
		}						
	}
}