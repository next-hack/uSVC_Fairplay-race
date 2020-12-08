/*
 * carPhysics.c
 *
 * Created: 30/08/2019 10:02:38
 *  Author: PETN
 */ 
// simple car physics
#include "main.h"  
#include "usvc_kernel/usvc_kernel.h"
#define HIT_ANGULAR_MULT 20		// (50) regulates the amount of angular rotation the car will receive if it hits a wall
#define MAX_REVERSE_SPEED 0x10000
//#define TORQUE (0x400000)
//#define STEERING_ANGLE ((uint32_t)FIXED_PI/6)
//#define THROTTLE 0x1800
//#define MAX_SQUARE_SPEED 0x100000
#define MINIMUM_SPEED 0x800
#define GRASS_MUL_FACTOR 0xFF00
//#define KILL_ORTHOGONAL_IMPULSE 0x6000
#define CAR_TO_CAR_COLLISION_ANGULAR_SPEED_COEFFICIENT (32767 >> 4)
#define REDUCED_TORQUE_COEFFICIENT_WHEN_SKIDDING 28000
#define ANGULAR_DRAG_CONSTANT_SKID 31000
#define RIGHT_VELOCITY_DRAG_COEFF 30000
#define ANGULAR_DRAG_CONSTANT 27000
#define SMALLEST_ANGULAR_SPEED 0xD000
//#define FORWARD_VELOCITY_BRAKING_COEFF 31800
//#define FORWARD_VELOCITY_DRAG_COEFF 32000
#define RESTITUTION_COEFFICIENT 0xF000		// how much a car will bounce off
#define HIT_COUNTER_RESET_TIME 2* 57
#define MINDELTASPACING 1
#define MAX_HITS_TO_BE_CONSIDERED_STUCK 15
//const uint32_t torqueValues[8] = {};
const int16_t throttleValues[9] =    {0x1600, 0x1800, 0x1A00, 0x1C00, 0x1E00, 0x2000, 0x2200, 0x2400, 0x2600};
const int32_t squareSpeedValues[9] = {0x100000, 0x120000, 0x140000, 0x160000, 0x180000, 0x1A0000, 0x1C0000, 0x1E0000, 0x200000};
const int16_t dragValues[9] = {32000, 32080, 32160, 32240, 32320, 32400, 32480, 32560, 32640};
const int32_t torqueValues[9] = {0x400000, 0x410000, 0x420000, 0x430000, 0x440000, 0x450000, 0x460000, 0x470000, 0x480000};
const uint32_t steeringAngleValues[9] = {0x15555555,  0x16555555, 0x18555555, 0x18555555, 0x19555555, 0x1A555555, 0x1B555555, 0x1C555555, 0x1D555555};
const int16_t brakingDragValues[9] = {31800, 31700, 31600, 31500, 31400, 31300, 31200, 31100, 31000};
const int32_t killOrthogonalImpulseValues[9] = {0x6000, 0x6200, 0x6400, 0x6600, 0x6800, 0x6A00, 0x6C00 ,0x6400 ,0x7000};
const carData_t carData[NUM_CARS] =
{
	{	// redCar
		.baseAcceleration = 2,
		.baseSteering = 1,
		.baseSpeed = 3,
		.baseBrake = 0 ,
		.baseGrip = 0,
	},
	{	// yellowCar
		.baseAcceleration = 3,
		.baseSteering = 1,
		.baseSpeed = 1,
		.baseBrake = 0,
		.baseGrip = 1,
	},
	{	// blueCar
		.baseAcceleration = 1,
		.baseSteering = 1,
		.baseSpeed = 1,
		.baseBrake = 1 ,
		.baseGrip = 2,
	},
	{	// greenCar
		.baseAcceleration = 0,
		.baseSteering = 2,
		.baseSpeed = 1,
		.baseBrake = 2,
		.baseGrip = 1,
	},
};

void fireBonus(car_t *pCar);
void killCar(car_t *pCar, uint8_t killer)
{
	if (pCar->carState == CAR_STATE_RUNNING)
	{
		pCar->carState = CAR_STATE_EXPLODING;
		pCar->bonusType = BONUS_NONE;
		pCar->bonusFrameTime = getFrameTicks();
		pCar->explosionFrame = 0;
		pCar->deaths++;		
	}
	if (killer < NUM_CARS && pCar != &cars[killer])	// suicides do not count as kills!
	{
		cars[killer].kills++;
	}
}
uint8_t checkCarAgainstTiles(car_t * pCar);
void setCarControls(car_t *pCar, uint8_t control)
{
	pCar->control |= control;
}
void initCars(car_t *pCarList, int numberOfCars)
{
	uint16_t ft = getFrameTicks();
	for (int i = 0; i < numberOfCars; i++)
	{
		car_t *pCar = &pCarList[i];
		// backup old cash. 
		uint16_t cash = pCar->cash;
		// backup old torunament points
		uint16_t points = pCar->tournamentPoints;
		// backup old unlocked cars
		uint8_t unlockedars = pCar->unlockedCars;
		//
		memset(pCar, 0, sizeof(car_t));		// clear state.
		// Restore backups
		pCar->unlockedCars = unlockedars;		// the green car must always be available.
		pCar->cash = cash;
		pCar->tournamentPoints = points;
		//
		pCar->currentWaypoint = currentLevelData.firstWaypoint;
		pCar->currentLapWaypoint = currentLevelData.lapWaypoint;
		pCar->bonusFrameTime = ft;
		pCar->carState = CAR_STATE_WAITING_FOR_START_RACE;
		pCar->explosionFrame = WAIT_DEFAULTANIM_NUMFRAMES - 1;
		pCar->currentPosition.x = currentLevelData.pInitialSpritePositions[i].posX << 16;
		pCar->currentPosition.y = currentLevelData.pInitialSpritePositions[i].posY << 16;
		pCar->currentAngle = currentLevelData.initialCarAngle;
	}
	int playerCarType = CARTYPE_GREENCAR; 
	for (int i = 1; i < NUM_CARS; i++)
	{
		cars[i].carType =  (playerCarType + i) %  NUM_CARS  ;
	}	
}

uint8_t pixelExactCollision(car_t *pCar1, car_t *pCar2,  vector_t * r1, vector_t *r2, vector_t *delta )
{
	// get the sprite information
	uint8_t frameNum1 = GET_CAR_REAL_FRAME(pCar1->carType, (pCar1->carState & CAR_STATE_DESTROYED) , pCar1->currentSpriteAngle); // realFrame[pCar->currentSpriteAngle];
	uint8_t flags1 =  GET_CAR_ROTFLAGS(pCar1->carType, (pCar1->carState & CAR_STATE_DESTROYED), pCar1->currentSpriteAngle);//rotFlag[pCar->currentSpriteAngle];
	uint8_t frameNum2 = GET_CAR_REAL_FRAME(pCar2->carType, (pCar2->carState & CAR_STATE_DESTROYED), pCar2->currentSpriteAngle); // realFrame[pCar->currentSpriteAngle];
	uint8_t flags2 =  GET_CAR_ROTFLAGS(pCar2->carType, (pCar2->carState & CAR_STATE_DESTROYED), pCar2->currentSpriteAngle);//rotFlag[pCar->currentSpriteAngle];

	frame_t *pFrame1 = (frame_t*) &frameData[frameNum1];
	frame_t *pFrame2 = (frame_t*) &frameData[frameNum2];

	// get the coordinates of the actual sprite	
	
	int16_t handlex1, handley1;
	uint8_t flagH1 =   flags1 & SPRITE_FLAGS_FLIP_HORIZONTAL;
	uint8_t flagV1 =   flags1 & SPRITE_FLAGS_FLIP_VERTICAL ;
	int16_t handlex2, handley2;
	uint8_t flagH2 =   flags2 & SPRITE_FLAGS_FLIP_HORIZONTAL;
	uint8_t flagV2 =   flags2 & SPRITE_FLAGS_FLIP_VERTICAL ;

	// these are just the coordinates of the cars. However, we need to calculate the real onscreen coordinates.
	int16_t x1 = (pCar1->currentPosition.x >> 16);
	int16_t y1 =  (pCar1->currentPosition.y >> 16);

	int16_t x2 = (pCar2->currentPosition.x >> 16);
	int16_t y2 = (pCar2->currentPosition.y >> 16);


	int32_t w1 = pFrame1->w;
	int32_t h1 = pFrame1->h;
	int32_t w2 = pFrame2->w;
	int32_t h2 = pFrame2->h;

	if (flags1 & SPRITE_FLAGS_INVERTXY)
	{
		handley1  = (flagH1 ? (pFrame1->ox - pFrame1->w) : -pFrame1->ox);
		handlex1 = (flagV1 ? (pFrame1->oy - pFrame1->h) : -pFrame1->oy);

	}
	else
	{
		handlex1  = (flagH1 ? (pFrame1->ox - pFrame1->w) : -pFrame1->ox);
		handley1 = (flagV1 ? (pFrame1->oy - pFrame1->h) : -pFrame1->oy);
	}
	x1 += handlex1;
	y1 += handley1;
	if (flags2 & SPRITE_FLAGS_INVERTXY)
	{
		handley2  = (flagH2 ? (pFrame2->ox - pFrame2->w) : -pFrame2->ox);
		handlex2 = (flagV2 ? (pFrame2->oy - pFrame2->h) : -pFrame2->oy);
	}
	else
	{
		handlex2  = (flagH2 ? (pFrame2->ox - pFrame2->w) : -pFrame2->ox);
		handley2 = (flagV2 ? (pFrame2->oy - pFrame2->h) : -pFrame2->oy);
	}
	x2 += handlex2;
	y2 += handley2;

	// 
	// Now the x1,y1 and x2,y2 point to the exact coordinates where we can find the top left pixel of our sprite.
	// We are not interested in the exact position, but rather on the relative distance. We are normalizing sprite1 to position (0,0)
	// and we just consider the sprite2 as if it was at its relative position dx,dy,
	
	int32_t dx =  x2 -  x1;
	int32_t dy =  y2 - y1;
	//
	// get the intersections of the rectangles
	//	

	int32_t minCheckX = dx > 0 ? dx : 0;
	int32_t minCheckY = dy > 0 ? dy : 0;
	
	int32_t maxCheckX;
	maxCheckX= dx < (((flags1 & SPRITE_FLAGS_INVERTXY) ? h1 : w1) - ((flags2 & SPRITE_FLAGS_INVERTXY) ? h2 : w2)) ? dx + ((flags2 & SPRITE_FLAGS_INVERTXY) ? h2 : w2) : ((flags1 & SPRITE_FLAGS_INVERTXY) ? h1 : w1);
	int32_t maxCheckY;
	maxCheckY = dy < ( ((flags1 & SPRITE_FLAGS_INVERTXY) ? w1 : h1) - ((flags2 & SPRITE_FLAGS_INVERTXY) ? w2 : h2) ) ? dy + ((flags2 & SPRITE_FLAGS_INVERTXY) ? w2 : h2)  : ((flags1 & SPRITE_FLAGS_INVERTXY) ? w1 : h1);	

	if (minCheckX >= maxCheckX || minCheckY >= maxCheckY)
		return 0; // no collision at all.
   // now, depending on the orientation, we must compute the increments we need to add to each pointer, as well as the pointer start location.
	const uint8_t *p1 = 0, *p2 = 0; // these are initialized just to avoid warnings
	// reset the vector between the sprite center and the point of contact.
	int horizontalIncrement1 = 0 ;	// these are initialized just to avoid warnings
	int horizontalIncrement2 = 0;
	int verticalIncrement1 = 0;
	int verticalIncrement2 = 0;
	
	if (flags1  == 0)
	{
		p1 = &pFrame1->pData[minCheckY * w1 + minCheckX];
		verticalIncrement1  = w1 - (maxCheckX - minCheckX);
		horizontalIncrement1 = 1;
	}
	else if  (  flags1  == SPRITE_FLAGS_FLIP_VERTICAL)
	{
		p1 = &pFrame1->pData[( h1 - 1 - minCheckY) * w1 + minCheckX];
		verticalIncrement1 = -w1 - (maxCheckX - minCheckX);
		horizontalIncrement1 = 1;
	}

	else if  (  flags1  == SPRITE_FLAGS_FLIP_HORIZONTAL)
	{
		p1 = &pFrame1->pData[minCheckY * w1 + w1 - 1 - minCheckX];
		verticalIncrement1 = w1 + (maxCheckX - minCheckX);
		horizontalIncrement1 = -1;		
	}
	else if  (  flags1 == (SPRITE_FLAGS_FLIP_HORIZONTAL | SPRITE_FLAGS_FLIP_VERTICAL ))
	{
		p1 = &pFrame1->pData[( h1 - 1 - minCheckY) * w1 + w1 - 1 - minCheckX];
		verticalIncrement1 = -w1 + maxCheckX - minCheckX;
		horizontalIncrement1 = -1;		
	}
	else if (flags1  == SPRITE_FLAGS_INVERTXY)
	{ 
		p1 = &pFrame1->pData[minCheckX * w1 + minCheckY];
		verticalIncrement1  = - w1 * (maxCheckX - minCheckX) + 1;
		horizontalIncrement1 = w1; 
	}
	else if (flags1  == SPRITE_FLAGS_ROTATE_90)
	{
		p1 = &pFrame1->pData[minCheckX * w1 + w1 - 1 - minCheckY];
		verticalIncrement1 = -w1*(maxCheckX - minCheckX) -1;
		horizontalIncrement1 = w1;
	}
	else if (flags1  == SPRITE_FLAGS_INVERTXY180)
	{
		p1 = &pFrame1->pData[( h1 - 1 - minCheckX) * w1 + w1 - 1 - minCheckY];
		verticalIncrement1 = +w1 *(maxCheckX - minCheckX) -1;
		horizontalIncrement1 = -w1;
	}
	else if (flags1  == SPRITE_FLAGS_ROTATE_270)
	{
		p1 = &pFrame1->pData[( h1 - 1 - minCheckX) * w1 + minCheckY];
		verticalIncrement1 = +w1 * (maxCheckX - minCheckX) + 1;
		horizontalIncrement1 = -w1;
	}
	// second sprite. now instead of minCheckX there is minCheckX - dx, and instead of minCheckY there is minCheckY - dy. Similar for maxCheckx, therefore the verticalIncrement does not change. 
	if (flags2  == 0)
	{
		p2 = &pFrame2->pData[(minCheckY - dy) * w2 + (minCheckX - dx) ];
		verticalIncrement2  = w2 - (maxCheckX - minCheckX);
		horizontalIncrement2 = 1;
	}
	else if  (  flags2  == SPRITE_FLAGS_FLIP_VERTICAL)
	{
		p2 = &pFrame2->pData[( h2 - 1 - (minCheckY - dy)) * w2 + (minCheckX - dx)];
		verticalIncrement2 = -w2 - (maxCheckX - minCheckX);
		horizontalIncrement2 = 1;
	}

	else if  (  flags2  == SPRITE_FLAGS_FLIP_HORIZONTAL)
	{
		p2 = &pFrame2->pData[(minCheckY - dy) * w2 + w2 - 1 - (minCheckX - dx)];
		verticalIncrement2 = w2 + (maxCheckX - minCheckX);
		horizontalIncrement2= -1;
	}
	else if  (  flags2 == (SPRITE_FLAGS_FLIP_HORIZONTAL | SPRITE_FLAGS_FLIP_VERTICAL ))
	{
		p2 = &pFrame2->pData[( h2 - 1 - (minCheckY - dy)) * w2 + w2 - 1 - (minCheckX - dx)];
		verticalIncrement2 = -w2 + maxCheckX - minCheckX;
		horizontalIncrement2 = -1;
	}
	else if (flags2  == SPRITE_FLAGS_INVERTXY)
	{		
		p2 = &pFrame2->pData[(minCheckX - dx) * w2 + (minCheckY - dy)];
		verticalIncrement2 = -w2 * (maxCheckX - minCheckX) + 1;
		horizontalIncrement2 = w2;
	}
	else if (flags2  == SPRITE_FLAGS_ROTATE_90)
	{
		p2 = &pFrame2->pData[(minCheckX - dx) * w2 + w2 - 1 - (minCheckY - dy)];
		verticalIncrement2 = -w2 * (maxCheckX - minCheckX) - 1;
		horizontalIncrement2 = w2;
	}
	else if (flags2  == SPRITE_FLAGS_INVERTXY180)
	{
		
		p2 = &pFrame2->pData[( h2 - 1 - (minCheckX - dx)) * w2 + w2 - 1 - (minCheckY - dy)];
		verticalIncrement2 = +w2 * (maxCheckX - minCheckX) - 1;
		horizontalIncrement2 = -w2;
	}
	else if (flags2  == SPRITE_FLAGS_ROTATE_270)
	{
		p2 = &pFrame2->pData[( h2 - 1 - (minCheckX - dx)) * w2 + (minCheckY - dy)];
		verticalIncrement2 = +w2 * (maxCheckX - minCheckX) + 1;
		horizontalIncrement2 = -w2;
	}	
	
	r1->x = 0;
	r1->y = 0;
	r2->x = 0;
	r2->y = 0;
	uint8_t collided = 0;	
	uint8_t minX = 0xFF;
	uint8_t maxX = 0;
	uint8_t minY = 0xFF;
	uint8_t maxY = 0;
	for(uint8_t y = minCheckY; y < maxCheckY; y++)
	{
				
		for(uint8_t x = minCheckX; x < maxCheckX; x++)
		{		
			uint8_t pixel1, pixel2;
			pixel1 = *p1;
			pixel2 = *p2;
			p1 = p1 + horizontalIncrement1;
			p2 = p2 + horizontalIncrement2;
			if (pixel1 && pixel2)
			{
				collided++;
				r1->x = r1->x + x  + handlex1;
				r2->x = r2->x + x  + handlex2 + dx;
				r1->y = r1->y + y  + handley1;
				r2->y = r2->y + y  + handley2 + dy;
				// get the extent of collision
				if (x < minX)
					minX = x;
				if (x > maxX)
					maxX = x;
				if (y < minY)
					minY = y;
				if (y > maxY)
					maxY = y;
			}
		}
		p1 = p1 + verticalIncrement1;
		p2 = p2 + verticalIncrement2;
	}
	if (collided)
	{
		r1->x  = (r1->x << 16) / collided;
		r2->x =  (r2->x << 16) / collided;
		r1->y = (r1->y << 16) / collided;
		r2->y = (r2->y << 16) / collided;	
		delta->x = maxX - minX;
		delta->y = maxY - minY;
	}
	return collided;
}
inline vector_t angularToLinearVelocity(vector_t radius, int32_t angulaSpeed)
{
	vector_t v = {.x = - FIXED_MUL(radius.y, angulaSpeed) , FIXED_MUL(radius.x, angulaSpeed)  };
	return v;
}
inline int32_t vectorMagnitudeSquare(vector_t v)
{
	return FIXED_MUL(v.x, v.x) + FIXED_MUL(v.y,  v.y);
}
inline int32_t vectorDotProduct(vector_t v1, vector_t v2)
{
	return FIXED_MUL(v1.x, v2.x) + FIXED_MUL(v1.y,  v2.y);
}
inline vector_t vectorProjection(vector_t v, uint32_t angle)
{
	uint32_t tableAngle = (angle >> (32 - 10)); //&  0x3FF;
	uint32_t tableAngle90 = (((angle + FIXED_PI/2)) >> (32 - 10));// & 0x3FF;	
	// note: the screen has its coordinate system which is clockwise rotated by 90°
	int16_t versorX =  -sinTable[tableAngle];
	int16_t versorY =  +sinTable[tableAngle90];
	int32_t dotProd = FIXED_MULFRACT(v.x, versorX) + FIXED_MULFRACT(v.y, versorY);
	int32_t px = FIXED_MULFRACT (dotProd, versorX);
	int32_t py = FIXED_MULFRACT (dotProd, versorY);
	vector_t p = {.x = px, .y = py};
	return p;
}
inline vector_t versor(uint32_t angle)
{
	uint32_t tableAngle = (angle >> (32 - 10)); //&  0x3FF;
	uint32_t tableAngle90 = (((angle + FIXED_PI/2)) >> (32 - 10));// & 0x3FF;
	// note: the screen has its coordinate system which is clockwise rotated by 90°
	int16_t versorX =  -sinTable[tableAngle];
	int16_t versorY =  +sinTable[tableAngle90];	
	vector_t v = {.x = versorX << 1, .y = versorY << 1};
	return v;
}
inline vector_t versorFromvector(vector_t v)
{
	vector_t vs;
	int32_t magnitude = fastApproximateSqrt(vectorMagnitudeSquare(v));
	vs.x = v.x / magnitude;
	vs.y = v.y / magnitude;
	return vs;
}
inline vector_t vectorSum(vector_t v1, vector_t v2)
{
	vector_t vs =  {.x = v1.x + v2.x, .y = v1.y + v2.y};
	return vs;
}
uint8_t bresenhamCollisionCheckTiles(int x0, int y0, int x1, int y1)
{
	int dx =  x1 > x0 ? (x1 - x0) : x0 - x1;
	int sx = x0 < x1 ? 1 : -1;
	int dy = y1 > y0 ? y0 -  y1 : y1 - y0;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx+dy;  /* error value e_xy */
	while (1)   /* loop */
	{
		if (!(x0 < 0 || y0 < 0 || y0 >= currentLevelData.mapSizeX *2  || x0 >= currentLevelData.mapSizeY * 2))
		{
			if (getMapTileFromTileCoordinates(x0, y0))
			{
				// determine in the old position we checked, where there was the obstacle. We actually need to check only 3 pixels.
				return 1;
			}
		}
		if (x0 == x1 && y0 == y1)
			return 0;
		int e2 = 2*err;
		if (e2 >= dy)
		{
			err += dy;  /* e_xy+e_x > 0 */
			x0 += sx;
		}
		if (e2 <= dx) /* e_xy+e_y < 0 */
		{
			err += dx;
			y0 += sy;
		}
	}
}
uint8_t getCarProperty(car_t *pCar, uint8_t propertyNumber)
{
	uint8_t base = carData[pCar->carType].properties[propertyNumber];
	uint8_t enhancement = pCar->enhancements.enhancements[propertyNumber];
	uint8_t combined = base + enhancement;
	return combined;
}
void carPhysics(car_t pCarList[], int numberOfCars)
{
	// first let's steer
	int32_t steeringAngle = 0;
	for (int i = 0; i < numberOfCars; i++)
	{
		car_t *pCar = &pCarList[i];
		if (pCar->carState != CAR_STATE_RUNNING)
			pCar->control = 0;
		pCar->oldPosition = pCar->currentPosition;
		pCar->oldAngle = pCar->currentAngle;
		if (pCar->collisionCoolDown)
			pCar->collisionCoolDown--;
		uint8_t flags = pCar->control;
		int32_t torque;
		if (flags & CAR_FLAGS_RIGHT)
		{
			// TODO: torque should be the consequence of the action of lateral forces, and not be chosen a-priori 
			torque = torqueValues[getCarProperty(pCar, CAR_PROPERTY_STEERING)];
			steeringAngle = steeringAngleValues[getCarProperty(pCar, CAR_PROPERTY_STEERING)];		
		}
		else if (flags & CAR_FLAGS_LEFT)
		{
			torque = -torqueValues[getCarProperty(pCar, CAR_PROPERTY_STEERING)];	
			steeringAngle = - steeringAngleValues[getCarProperty(pCar, CAR_PROPERTY_STEERING)];			
		}
		else
			torque = 0;
		int32_t	speedQ = vectorMagnitudeSquare(pCar->currentSpeed);
		int32_t speed = fastApproximateSqrt(speedQ);
		pCar->lastSpeed = (uint32_t) speed;		// should never be negative anyway.
		if( flags & CAR_FLAGS_BRAKING)
		{   // when braking, give a 25% torque boost
			torque = torque + (torque >> 2);		
		}
		if (speed < 0x10000)
		{	// for small speed, make the torque proportional to speed
			torque = FIXED_MUL(torque,speed);		
		}
		int32_t power;
		// accelerate
		if (pCar->control & CAR_FLAGS_REVERSE)
		{
			power = -0x1000;		
		}
		else if (pCar->control & CAR_FLAGS_FORWARD)
		{
			power = throttleValues[getCarProperty(pCar, CAR_PROPERTY_ACCELERATION)];		
		}
		else
		{
			power = 0;		
		}
		// let's limit the maximum velocity. Also, let's consider that the larger the speed, the smaller the speed increment we have
		power = power * (1 - speedQ/squareSpeedValues[getCarProperty(pCar, CAR_PROPERTY_SPEED)]);
		if (pCar->control & CAR_FLAGS_BRAKING)
		{
			power = 0;
		}
		// kill speed if it is lower than a minimum. 
		if (speed < MINIMUM_SPEED)
		{
			pCar->currentSpeed.x = 0;
			pCar->currentSpeed.y = 0;
		}
		// note: we are using discrete angles, to avoid awkward movements due to discrete sprite angles.
		uint32_t tableAngle = (pCar->currentAngle >> (32 - 10)) &  0x3E0;
		uint32_t tableAngle90 = (((pCar->currentAngle  + FIXED_PI / 2)) >> (32 - 10)) & 0x3E0;
		uint32_t tableAngleSteer = ((pCar->currentAngle + steeringAngle) >> (32 - 10)) &  0x3E0;
		uint32_t tableAngle90Steer = ((((pCar->currentAngle + steeringAngle)  + FIXED_PI / 2)) >> (32 - 10)) & 0x3E0;
		pCar->currentSpeed.x += FIXED_MULFRACT(power, sinTable[tableAngleSteer]);
		pCar->currentSpeed.y += -FIXED_MULFRACT(power, sinTable[tableAngle90Steer]);	
		// let's kill the ortogonal speed up to a maximum value
		// first get the unitary vector in the direction pointed by the car
		int32_t rightVelocityX, rightVelocityY;
		int32_t forwardVelocityX, forwardVelocityY;
		int16_t forwardX =  -sinTable[tableAngle];
		int16_t forwardY =  +sinTable[tableAngle90];
		int32_t dotProdFwd = FIXED_MULFRACT(pCar->currentSpeed.x, forwardX) + FIXED_MULFRACT(pCar->currentSpeed.y, forwardY);
		forwardVelocityX = FIXED_MULFRACT (dotProdFwd, forwardX);
		forwardVelocityY = FIXED_MULFRACT (dotProdFwd, forwardY);
		int32_t dotProdRight = FIXED_MULFRACT(pCar->currentSpeed.x, forwardY) + FIXED_MULFRACT(pCar->currentSpeed.y, -forwardX);
		rightVelocityX = FIXED_MULFRACT (dotProdRight, forwardY);
		rightVelocityY = FIXED_MULFRACT (dotProdRight, -forwardX);
		// Now, the forward velocity must be decreased if braking (wheels cannot brake in the orthogonal direction)
		int32_t rightVelocityQ =  FIXED_MUL(rightVelocityX, rightVelocityX) + FIXED_MUL(rightVelocityY,  rightVelocityY);
		int32_t killOrthogonalImpulse = killOrthogonalImpulseValues[getCarProperty(pCar, CAR_PROPERTY_GRIP)];
		if (rightVelocityQ >= (FIXED_MUL(killOrthogonalImpulse,killOrthogonalImpulse)) )
		{
			rightVelocityX = FIXED_MULFRACT(rightVelocityX, RIGHT_VELOCITY_DRAG_COEFF); // rightVelocityX - FIXED_MULFRACT (KILL_ORTHOGONAL_IMPULSE, forwardY);
			rightVelocityY =  FIXED_MULFRACT(rightVelocityY, RIGHT_VELOCITY_DRAG_COEFF); // rightVelocityY - FIXED_MULFRACT (KILL_ORTHOGONAL_IMPULSE,  -forwardX);
			if (pCar->control & CAR_FLAGS_BRAKING)
			{
				if (!pCar->isSkidding )
				{
					if (i == 0)
						triggerNote(PROJECTILE_CHANNEL, 8, 0x19, 255);
					pCar->isSkidding = 1;
				}
			}
			pCar->currentAngularSpeed += FIXED_MULFRACT(torque, REDUCED_TORQUE_COEFFICIENT_WHEN_SKIDDING);
			pCar->currentAngularSpeed = FIXED_MULFRACT(pCar->currentAngularSpeed, ANGULAR_DRAG_CONSTANT_SKID);
			pCar->currentAngle += pCar->currentAngularSpeed;
		}
		else
		{
			pCar->currentAngularSpeed += torque;
			pCar->currentAngularSpeed = FIXED_MULFRACT(pCar->currentAngularSpeed, ANGULAR_DRAG_CONSTANT);
			pCar->currentAngle += pCar->currentAngularSpeed;

			if (pCar->isSkidding && i == 0)
			{
				stopLoopingFx(PROJECTILE_CHANNEL, 1);
			}
			pCar->isSkidding = 0;			
			rightVelocityX = 0;
			rightVelocityY = 0;
		}
		if (pCar->currentAngularSpeed < (SMALLEST_ANGULAR_SPEED) && pCar->currentAngularSpeed > - SMALLEST_ANGULAR_SPEED)
		{	// kill small angular speeds
			pCar->currentAngularSpeed = 0;
		}		
		pCar->currentSpeed.x = FIXED_MULFRACT(forwardVelocityX,  ((pCar->control & CAR_FLAGS_BRAKING) ? brakingDragValues[getCarProperty(pCar, CAR_PROPERTY_BRAKE)] : ((pCar->control & (CAR_FLAGS_FORWARD)) ? dragValues[getCarProperty(pCar,CAR_PROPERTY_SPEED)] : dragValues[0] ) ))  + rightVelocityX; // FIXED_MULFRACT(rightVelocityX, 31000);
		pCar->currentSpeed.y =   FIXED_MULFRACT(forwardVelocityY, ((pCar->control & CAR_FLAGS_BRAKING) ? brakingDragValues[getCarProperty(pCar, CAR_PROPERTY_BRAKE)] : ((pCar->control & (CAR_FLAGS_FORWARD)) ? dragValues[getCarProperty(pCar,CAR_PROPERTY_SPEED)] : dragValues[0] ) ))  + rightVelocityY; 
		//
		pCar->currentPosition.x += pCar->currentSpeed.x;
		pCar->currentPosition.y += pCar->currentSpeed.y;
		// finally let's return the current direction as a number between 0 and 31
		pCar->currentSpriteAngle = ((pCar->currentAngle) >> (32-5)) ;	 		
		if (pCar->control & CAR_FLAGS_FIRE)
		{
			fireBonus(pCar);
		}
		pCar->control = 0;	// reset controls

	}
	// now that we know where shold be the positions, let's try to calculate and solve the collisions
	for (int i = 0; i < numberOfCars ; i++)
	{
		car_t *pCar1 = &pCarList[i];
		uint8_t coll = checkCarAgainstTiles(pCar1);
		if (coll && i == 0)  
			triggerNote(EXPLOSION_CHANNEL, 6, 0x20, 255);
		if (i != 0)
		{
			if (coll && i != 0)
			{
				pCar1->timeWithoutHits = 0;
				pCar1->hits++;
				if (pCar1->hits > MAX_HITS_TO_BE_CONSIDERED_STUCK)
				{
					pCar1->hits = 0;	
					pCar1->reverse = 57; // give a second of reverse
					uint8_t strategy = rand() % 4;
					uint32_t closestDistance = 0xFFFFFFFF;
					int32_t x = pCar1->currentPosition.x >> 16;
					int32_t y = pCar1->currentPosition.y >> 16;					
					const waypoint_t * wp = NULL;
					switch (strategy)
					{
						case 0:
							// find previous waypoint
							for (int w = 0; wp != currentLevelData.lapWaypoint; w++)
							{
								wp = &currentLevelData.firstWaypoint[w];
								if (pCar1->currentWaypoint == wp->pNextWaypoint1 || pCar1->currentWaypoint == wp->pNextWaypoint2)
								{
									pCar1->currentWaypoint = wp;
									break;
								}
							}		
							break;	
						case 1:
						{
							// find closest waypoint
							for (int w = 0; wp != currentLevelData.lapWaypoint; w++)
							{
								wp = &currentLevelData.firstWaypoint[w];
								int squareDistance = (wp->x - x) * (wp->x - x) + (wp->y - y) * (wp->y - y);
								if (pCar1->currentWaypoint != wp && squareDistance < closestDistance)
								{
									closestDistance = squareDistance;
									pCar1->currentWaypoint = wp;
								}
							}							
						}
						break;
						case 2:
							// go to next waypoint
							if ((rand() & 1) == 0)
								pCar1->currentWaypoint = pCar1->currentWaypoint->pNextWaypoint1;
							else
								pCar1->currentWaypoint = pCar1->currentWaypoint->pNextWaypoint2;						
							break;
						case 3:
							// find the closest waypoint which offers a direct line of sight (no obstacles)
							for (int w = 0; wp != currentLevelData.lapWaypoint; w++)
							{
								wp = &currentLevelData.firstWaypoint[w];
								int squareDistance = (wp->x - x) * (wp->x - x) + (wp->y - y) * (wp->y - y);
								if (pCar1->currentWaypoint != wp && squareDistance < closestDistance && !bresenhamCollisionCheckTiles(x / TILE_SIZE_X, y / TILE_SIZE_Y, wp->x / TILE_SIZE_X, wp->y / TILESIZEY))
								{
									closestDistance = squareDistance;
									pCar1->currentWaypoint = wp;
								}
							}
							break;															
					}
				}
			}
			else
			{
				pCar1->timeWithoutHits++;
				if (pCar1->timeWithoutHits > HIT_COUNTER_RESET_TIME)
				{
					pCar1->hits = 0;				
				}
			}			
		}
		for (int j = i + 1; j < numberOfCars; j++)
		{
			car_t *pCar2 = &pCarList[j];
			vector_t r1, r2;
			vector_t delta;
			int collision = pixelExactCollision(pCar1, pCar2,  &r1, &r2, &delta);
			// now we know the exact position of the collision and the number of pixel collided
			if (collision && !pCar1->collisionCoolDown && !pCar2->collisionCoolDown)
			{
				if (i == 0)
					triggerNote(EXPLOSION_CHANNEL, 6, 0x22, 255);
				//
				vector_t vp1, vp2;
				vp1 = pCar1->currentSpeed; //  vectorSum(pCar1->currentSpeed, angularToLinearVelocity(r1, (pCar1->currentAngularSpeed) ));	
				vp2 = pCar2->currentSpeed; // vectorSum(pCar2->currentSpeed, angularToLinearVelocity(r2, (pCar2->currentAngularSpeed)));
				vector_t vr = {.x = vp2.x - vp1.x, .y = vp2.y - vp1.y };
// 				pCar1->currentAngularSpeed += ((FIXED_MUL (r1.x,vr.y) - FIXED_MUL (r1.y,vr.x)) >> 3) << 8;
// 				pCar2->currentAngularSpeed += ((FIXED_MUL (r2.x,vr.y) - FIXED_MUL (r2.y,vr.x)) >> 3) << 8 ;
				pCar1->currentAngularSpeed += FIXED_MULFRACT((FIXED_MUL (r1.x,vr.y) - FIXED_MUL (r1.y,vr.x)), CAR_TO_CAR_COLLISION_ANGULAR_SPEED_COEFFICIENT) << 8;
				pCar2->currentAngularSpeed += FIXED_MULFRACT((FIXED_MUL (r2.x,vr.y) - FIXED_MUL (r2.y,vr.x)), CAR_TO_CAR_COLLISION_ANGULAR_SPEED_COEFFICIENT) << 8;

				if (pCar1->currentPosition.y > pCar2->currentPosition.y)
				{
					pCar1->currentPosition.y +=  (delta.y/2 + MINDELTASPACING) << 16;
					pCar2->currentPosition.y +=  - (delta.y/2 + MINDELTASPACING) << 16;
				}
				else
				{
					pCar1->currentPosition.y += - (delta.y/2 + MINDELTASPACING) << 16;
					pCar2->currentPosition.y +=  + (delta.y/2 + MINDELTASPACING) << 16;
				}
				if (pCar1->currentPosition.x > pCar2->currentPosition.x)
				{
					pCar1->currentPosition.x +=  + (delta.x/2 + MINDELTASPACING) << 16;
					pCar2->currentPosition.x +=  - (delta.x/2 + MINDELTASPACING) << 16;
				}
				else
				{
					pCar1->currentPosition.x +=  - (delta.x/2 + MINDELTASPACING) << 16;
					pCar2->currentPosition.x += + (delta.x/2 + MINDELTASPACING) << 16;
				}

// 				pCar1->currentSpeed.x  -= -(vr.x + (vr.x >> 3));
// 				pCar1->currentSpeed.y  -= -(vr.y + (vr.y >> 3));
// 				pCar2->currentSpeed.x  += -(vr.x + (vr.x >> 3));
// 				pCar2->currentSpeed.y  += -(vr.y + (vr.y >> 3));					
				pCar1->currentSpeed.x  -= -(vr.x);
				pCar1->currentSpeed.y  -= -(vr.y);
				pCar2->currentSpeed.x  += -(vr.x);
				pCar2->currentSpeed.y  += -(vr.y);

				pCar1->collisionCoolDown = 1;
				pCar2->collisionCoolDown = 1;

			}
		}
	}
}
void writeVramNumber(int line, int number)// helper function for debug
{
	for (int i = 0; i < VRAMX ; i++)
	{
		if (number & 1)
			vram[i + line * VRAMX] =  (uint32_t) &tiles[7*16];		
		else
			vram[i + line * VRAMX] = (uint32_t) &tiles[16];		
		number = number >> 1;
	}
}
int checkAgainstWalls (int pointerIncrement, int32_t *pCoordinate, const uint8_t *pSpriteFrame, int howMany, int startPixel)
{
	int collided = 0;
	for (int i = 0; i < howMany; i++)
	{
		if (*pSpriteFrame)
		{
			collided ++;
			*pCoordinate = *pCoordinate + startPixel + i;
		}
		pSpriteFrame = pSpriteFrame + pointerIncrement;
	}

	return collided;
}

uint8_t checkCarAgainstTiles(car_t * pCar)
{
	/*	This function checks which tiles the car is on, and acts accordingly to their types.
		There is no need to check each pixel of the sprite. We create a 
	*/
	// get the sprite information

	uint8_t frameNum = GET_CAR_REAL_FRAME(pCar->carType, (pCar->carState & CAR_STATE_DESTROYED), pCar->currentSpriteAngle); // realFrame[pCar->currentSpriteAngle];
	uint8_t flags =  GET_CAR_ROTFLAGS(pCar->carType, (pCar->carState & CAR_STATE_DESTROYED),  pCar->currentSpriteAngle);//rotFlag[pCar->currentSpriteAngle];

	frame_t *pFrame1 = (frame_t*) &frameData[frameNum];

	// get the coordinates of the actual sprite

	int32_t handlex, handley;
	uint8_t flagH =   flags & SPRITE_FLAGS_FLIP_HORIZONTAL;
	uint8_t flagV =   flags & SPRITE_FLAGS_FLIP_VERTICAL ;

	// these are just the coordinates of the cars. However, we need to calculate the real onscreen coordinates.
	int16_t xSpr = (pCar->currentPosition.x >> 16);
	int16_t ySpr =  (pCar->currentPosition.y >> 16);
	int16_t xSprMax; // not inclusive
	int16_t ySprMax;	// not inclusive


	int32_t wSpr = pFrame1->w;
	int32_t hSpr = pFrame1->h;
	int32_t tileXstart, tileXstop, tileYstart, tileYstop;
	if (flags & SPRITE_FLAGS_INVERTXY)
	{
		handley  = (flagH ? (pFrame1->ox - pFrame1->w) : -pFrame1->ox);
		handlex = (flagV ? (pFrame1->oy - pFrame1->h) : -pFrame1->oy);
		xSpr += handlex;
		ySpr += handley;
		//xSpr += handley;
		//ySpr += handlex;
		xSprMax = xSpr + hSpr;
		ySprMax = ySpr + wSpr;
		tileXstart = xSpr >> LOG2_TILE_SIZE_X;
		tileYstart = ySpr >> LOG2_TILE_SIZE_Y;
		tileXstop = (xSpr + hSpr - 1) >> LOG2_TILE_SIZE_X;
		tileYstop = (ySpr + wSpr - 1) >> LOG2_TILE_SIZE_Y;
	}
	else
	{
		handlex  = (flagH ? (pFrame1->ox - pFrame1->w) : -pFrame1->ox);
		handley = (flagV ? (pFrame1->oy - pFrame1->h) : -pFrame1->oy);
		xSpr += handlex;
		ySpr += handley;
		xSprMax = xSpr + wSpr;
		ySprMax = ySpr + hSpr;
		tileXstart = xSpr >> LOG2_TILE_SIZE_X;
		tileYstart = ySpr >> LOG2_TILE_SIZE_Y;
		tileXstop = (xSpr + wSpr - 1) >> LOG2_TILE_SIZE_X;
		tileYstop = (ySpr + hSpr - 1) >> LOG2_TILE_SIZE_Y;		
	}
	// now, depending on the orientation, we must compute the increments we need to add to each pointer, as well as the pointer start location.
	const uint8_t *p =  0;				// these are initialized just to avoid warnings
	int horizontalIncrement = 0 ;	// these are initialized just to avoid warnings
	int verticalIncrement = 0;
	if (flags  == 0)
	{
		p = &pFrame1->pData[0]; // point to the top left pixel.
		verticalIncrement  = wSpr;
		horizontalIncrement = 1;
	}
	else if  (  flags  == SPRITE_FLAGS_FLIP_VERTICAL)
	{
		p = &pFrame1->pData[( hSpr - 1 ) * wSpr];
		verticalIncrement = -wSpr;
		horizontalIncrement = 1;
	}

	else if  (  flags  == SPRITE_FLAGS_FLIP_HORIZONTAL)
	{
		p = &pFrame1->pData[wSpr - 1];
		verticalIncrement = wSpr;
		horizontalIncrement = -1;
	}
	else if  (  flags == (SPRITE_FLAGS_FLIP_HORIZONTAL | SPRITE_FLAGS_FLIP_VERTICAL ))
	{
		p = &pFrame1->pData[( hSpr - 1) * wSpr + wSpr - 1];
		verticalIncrement = -wSpr;
		horizontalIncrement = -1;
	}
	else if (flags  == SPRITE_FLAGS_INVERTXY)
	{
		p = &pFrame1->pData[0];
		verticalIncrement  =  1;
		horizontalIncrement = wSpr;
	}
	else if (flags  == SPRITE_FLAGS_ROTATE_90)
	{
		p = &pFrame1->pData[wSpr - 1];
		verticalIncrement =  -1;
		horizontalIncrement = wSpr;
	}
	else if (flags  == SPRITE_FLAGS_INVERTXY180)
	{
		p = &pFrame1->pData[( hSpr - 1 ) * wSpr + wSpr - 1 ];
		verticalIncrement =  -1;
		horizontalIncrement = -wSpr;
	}
	else if (flags  == SPRITE_FLAGS_ROTATE_270)
	{
		p = &pFrame1->pData[( hSpr - 1 ) * wSpr];
		verticalIncrement =  1;
		horizontalIncrement = -wSpr;
	}
	
	
	vector_t rTop = {0, 0}; 
	vector_t rBottom = {0, 0};
	vector_t rLeft = {0, 0};
	vector_t rRight = {0, 0};
	int16_t collidedTop = 0, collidedBottom = 0, collidedLeft = 0, collidedRight = 0;
	//int typeFlag = 0;	
	for (int yt = tileYstart; yt <= tileYstop; yt++)
	{
		for (int xt = tileXstart; xt <= tileXstop; xt++)
		{
			int tileType = currentLevelData.pTileTypes[/*gameMap[yt][xt]*/TILEMASK & getMapTileFromTileCoordinates(xt, yt)];
			switch (tileType)
			{
				case TILEPROP_NORMAL:
					continue;
				case TILEPROP_SOLID:	
				{			
					int32_t offsetX, offsetY;
					offsetX =  xt * TILE_SIZE_X -  xSpr;
					offsetY =  yt * TILE_SIZE_Y - ySpr;							
					int32_t minCheckX = offsetX > 0 ? offsetX : 0;
					int32_t minCheckY = offsetY > 0 ? offsetY : 0;
					
		
					
					if (yt > 0 && currentLevelData.pTileTypes[TILEMASK & getMapTileFromTileCoordinates(xt, yt - 1)] != TILEPROP_SOLID && (yt * TILE_SIZE_Y) >= ySpr)	 
					{	// check the top wall only if another solid tile is not above. The map edges are considered to be solid.
						// note: we only check that (yt << LOG2_TILE_SIZE_Y) >= ySpr, as tileYstart is calculated as the floor.
						// tileYstop is calculated as ceiling, and for sure the sprite rectangle is touching the row 0 of tiles at tileYstop.
						// also note that offsetY is always non negative here.
						// minCheckX is also tile aligned.
						int howMany = TILE_SIZE_X;
						if (offsetX < 0)  // this can occur only at the leftmost tiles which contain our sprite, so howMany will be less than TILE_SIZE_X.
						{
							howMany = TILE_SIZE_X + offsetX;
						}
						if (xSprMax < ((xt + 1) * TILE_SIZE_X))
						{
							howMany = howMany - (((xt + 1) * TILE_SIZE_X) - xSprMax);
						}
						int newCollisions = checkAgainstWalls (horizontalIncrement, &rTop.x, &p[offsetY * verticalIncrement + horizontalIncrement * minCheckX], howMany, minCheckX);
						rTop.y += (newCollisions * (offsetY - handley));
						collidedTop += newCollisions;
					}
					if (yt < (/*MAPSIZEY*/ getMapNumTileY() - 1) && currentLevelData.pTileTypes[TILEMASK & getMapTileFromTileCoordinates(xt, yt + 1)] != TILEPROP_SOLID && ((yt * TILE_SIZE_Y) | (TILE_SIZE_Y - 1)) < ySprMax )
					{   // check the bottom wall only if another solid tile is not below it. The map edges are considered to be solid.
						// note: we only check that ((yt << LOG2_TILE_SIZE_Y) | (TILE_SIZE_Y - 1)) <= y1max, as tileYstop is calculated as the ceiling
						// tileYstart is calculated as floor, but for sure the sprite rectangle touches the 7-th row of tiles at tileYstart.
						int howMany = TILE_SIZE_X;
						if (offsetX < 0)  // this can occur only at the leftmost tiles which contain our sprite, so howMany will be less than TILE_SIZE_X.
						{
							howMany = TILE_SIZE_X + offsetX;
						}
						if (xSprMax < ((xt + 1) * TILE_SIZE_X))
						{
							howMany = howMany - (((xt + 1) * TILE_SIZE_X) - xSprMax);
						}
						int newCollisions = checkAgainstWalls (horizontalIncrement, &rBottom.x, &p[(offsetY + TILE_SIZE_Y - 1) * verticalIncrement + horizontalIncrement * minCheckX], howMany, minCheckX);
						rBottom.y += (newCollisions * (offsetY + TILE_SIZE_Y - 1 - handley));
						collidedBottom += newCollisions;
					}
					if (xt > 0 && currentLevelData.pTileTypes[TILEMASK & getMapTileFromTileCoordinates(xt - 1, yt)] != TILEPROP_SOLID && (xt * TILE_SIZE_X) >= xSpr)	
					{	// check the left wall only if another solid tile is not on its left. The map edges are considered to be solid.
						// note: we only check that (xt << LOG2_TILE_SIZE_X) >= x1, as tileXstart is calculated as the floor
						// tileXstop is calculated as ceiling, and for sure the sprite rectangle is touching the column 0 of tiles at tileXstop.
						// also note that offsetX is always non negative here.
						// minCheckY is also tile aligned.
						int howMany = TILE_SIZE_Y;
						if (offsetY < 0)  // this can occur only at the topmost tiles which contain our sprite, so howMany will be less than TILE_SIZE_Y.
						{
							howMany = TILE_SIZE_Y + offsetY;
						}
						if (ySprMax < ((yt + 1) * TILE_SIZE_Y))
						{
							howMany = howMany - (((yt + 1) * TILE_SIZE_Y) - ySprMax);
						}
						
						int newCollisions = checkAgainstWalls (verticalIncrement, &rLeft.y, &p[minCheckY * verticalIncrement + horizontalIncrement * offsetX], howMany, minCheckY);			
						rLeft.x += (newCollisions * (offsetX - handlex));
						collidedLeft += newCollisions;								
					}
					if (xt < (/*MAPSIZEX*/ getMapNumTileX()  - 1 ) && currentLevelData.pTileTypes[TILEMASK & getMapTileFromTileCoordinates(xt + 1 , yt)] != TILEPROP_SOLID && ((xt * TILE_SIZE_X) | (TILE_SIZE_X - 1)) < xSprMax )
					{	// check the left wall only if another solid tile is not on its left. The map edges are considered to be solid.
						// note: we only check that ((xt << LOG2_TILE_SIZE_X) | (TILE_SIZE_X - 1)) <= x1max, as tileXstop is calculated as the ceiling
						// tileXstart is calculated as floor, but for sure the sprite rectangle touches the 7-th column of tiles at tileXstart.
						int howMany = TILE_SIZE_Y;
						if (offsetY < 0)  // this can occur only at the topmost tiles which contain our sprite, so howMany will be less than TILE_SIZE_Y.
						{
							howMany = TILE_SIZE_Y + offsetY;
						}
						if (ySprMax < ((yt + 1) * TILE_SIZE_Y))
						{
							howMany = howMany - (((yt + 1) * TILE_SIZE_Y) - ySprMax);
						}			
						int newCollisions = checkAgainstWalls(verticalIncrement, &rRight.y, &p[minCheckY * verticalIncrement + horizontalIncrement * (offsetX + TILE_SIZE_X - 1)] , howMany, minCheckY );
						rRight.x += (newCollisions * (offsetX + TILE_SIZE_X - 1 - handlex));
						collidedRight += newCollisions;
					}
					break;
				}
				case TILEPROP_SLOW:
					pCar->currentSpeed.x = FIXED_MUL(pCar->currentSpeed.x, GRASS_MUL_FACTOR);
					pCar->currentSpeed.y = FIXED_MUL(pCar->currentSpeed.y, GRASS_MUL_FACTOR);
					continue;		
				default: 
					continue;
			}
		}
	}
	// based on the outcomes, we need to calculate the normal.
	if (collidedRight)		// collision with right wall
	{
		pCar->currentPosition.x +=  MINDELTASPACING << 16;
		rRight.y =  ((rRight.y + collidedRight * handley)  << 16) / collidedRight;
		if (pCar->currentSpeed.x < 0)
		{
			pCar->currentAngularSpeed += (FIXED_MUL (rRight.y, pCar->currentSpeed.x) ) * HIT_ANGULAR_MULT;
			pCar->currentSpeed.x  = -(pCar->currentSpeed.x /*+ (pCar->currentSpeed.x >> 3)*/);

		}
		else
		{
			pCar->currentAngularSpeed += -(FIXED_MUL (rRight.y, pCar->currentSpeed.x) ) * HIT_ANGULAR_MULT;
			pCar->currentSpeed.x  = (pCar->currentSpeed.x /*+ (pCar->currentSpeed.x >> 3)*/);
		}
	}
	if (collidedLeft)
	{
		pCar->currentPosition.x -=  MINDELTASPACING << 16;
		rLeft.y =  ((rLeft.y + collidedLeft * handley)  << 16) / collidedLeft;
		if (pCar->currentSpeed.x > 0)
		{
			pCar->currentAngularSpeed += (FIXED_MUL (rLeft.y, pCar->currentSpeed.x) ) * HIT_ANGULAR_MULT;
			pCar->currentSpeed.x  = -(pCar->currentSpeed.x /*+ (pCar->currentSpeed.x >> 3)*/);

		}
		else
		{
			pCar->currentAngularSpeed += -(FIXED_MUL (rLeft.y, pCar->currentSpeed.x) ) * HIT_ANGULAR_MULT;
			pCar->currentSpeed.x  = (pCar->currentSpeed.x /*+ (pCar->currentSpeed.x >> 3)*/);
		}		
	}
	if (collidedBottom)
	{
		pCar->currentPosition.y +=  MINDELTASPACING << 16;
		rBottom.x =  ((rBottom.x + collidedBottom * handlex)  << 16) / collidedBottom;
		if (pCar->currentSpeed.y < 0)
		{
			pCar->currentAngularSpeed += -(FIXED_MUL (rBottom.x, pCar->currentSpeed.y) ) * HIT_ANGULAR_MULT;
			pCar->currentSpeed.y  = -(pCar->currentSpeed.y /*+ (pCar->currentSpeed.y >> 3)*/);

		}
		else
		{
			pCar->currentAngularSpeed += (FIXED_MUL (rBottom.x, pCar->currentSpeed.y) ) * HIT_ANGULAR_MULT;
			pCar->currentSpeed.y  = (pCar->currentSpeed.y /*+ (pCar->currentSpeed.y >> 3)*/);
		}		
	}
	if (collidedTop)
	{
		pCar->currentPosition.y +=  -MINDELTASPACING << 16;
		rTop.x =  ((rTop.x + collidedTop * handlex)  << 16) / collidedTop;
		if (pCar->currentSpeed.y > 0)
		{
			pCar->currentAngularSpeed += -(FIXED_MUL (rTop.x, pCar->currentSpeed.y) ) * HIT_ANGULAR_MULT;
			pCar->currentSpeed.y  = -(pCar->currentSpeed.y /*+ (pCar->currentSpeed.y >> 3)*/);

		}
		else
		{
			pCar->currentAngularSpeed += (FIXED_MUL (rTop.x, pCar->currentSpeed.y) ) * HIT_ANGULAR_MULT;
			pCar->currentSpeed.y  = (pCar->currentSpeed.y /*+ (pCar->currentSpeed.y >> 3)*/);
		}		
	}
	return 0 != collidedTop || 0 != collidedRight || 0 != collidedLeft || 0 != collidedBottom;
}
void fireBonus(car_t *pCar)
{
	projectile_t *pProjectile = &pCar->projectile[0];
	uint16_t frame = getFrameTicks();
	switch (pCar->bonusType)
	{
		case BONUS_NOT_YET_DECIDED:
		case BONUS_NONE:
		case REMOTE_BOMB_ACTIVATION:
			return;
		case BONUS_DEPLOY_REMOTE_BOMB:
			pProjectile->frameTime = frame ;
			pCar->bonusFrameTime = frame;
			pProjectile->carState = STATE_BOMB_DEPLOYED;
			pCar->bonusType =  REMOTE_BOMB_ACTIVATION;
			pProjectile->frame = 0;
			break;
		case BONUS_SET_OFF_REMOTE_BOMB:
			pCar->bonusType = BONUS_NONE;
			pProjectile->frame = 0;		
			pProjectile->carState =STATE_BOMB_ACTIVE | PROJECTILE_STATE_EXPLODING;
			triggerNote(EXPLOSION_CHANNEL, 5, 25 , 255);
			return;	

		case BONUS_REAR_ROCKET:
			triggerNote(PROJECTILE_CHANNEL, 7, 0x18, 255);
			pProjectile->frameTime = frame;
			pProjectile->carState = STATE_ROCKET_FLYING;
			pCar->bonusType = BONUS_NONE;
			pProjectile->spriteAngle = (pCar->currentSpriteAngle + (CAR_SPRITE_DIRECTIONS / 2)) & CAR_SPRITE_DIRECTIONS_MASK;
			pProjectile->frame = 0;
			break;
		case BONUS_FRONT_ROCKET:
			triggerNote(PROJECTILE_CHANNEL, 7, 0x18, 255);
			pProjectile->frameTime = frame;
			pProjectile->carState = STATE_ROCKET_FLYING;
			pCar->bonusType = BONUS_NONE;
			pProjectile->spriteAngle = pCar->currentSpriteAngle;
			pProjectile->frame = 0;
			break;
		case BONUS_DEPLOY_MINE:
			pProjectile->frameTime = frame;
			pCar->projectile[0].carState = STATE_MINE_DEPLOYED;
			pCar->bonusType = BONUS_NONE;
			pProjectile->frame = 0;
			break;
		case BONUS_SPEED:
			pCar->currentSpeed.x *= 2;
			pCar->currentSpeed.y *= 2;
			pCar->bonusType = BONUS_NONE;
			return;
		break;
	}
	pProjectile->currentPosition.x =  pCar->currentPosition.x;
	pProjectile->currentPosition.y =  pCar->currentPosition.y;	
}

