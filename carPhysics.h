/*
 * carPhysics.h
 *
 * Created: 30/08/2019 11:45:40
 *  Author: PETN
 */ 


#ifndef CARPHYSICS_H_
#define CARPHYSICS_H_
#include "waypoint.h"
#include "fastApproxMath.h"
#include "projectilesAndBonuses.h"
#include "sinTable.h"
#define CAR_FLAGS_BRAKING 1
#define CAR_FLAGS_REVERSE 2
#define CAR_FLAGS_FORWARD 4
#define CAR_FLAGS_RIGHT 8
#define CAR_FLAGS_LEFT 16
#define CAR_FLAGS_FIRE 32
#define NUMBER_OF_PROJECTILES_PER_CAR 1
#define CAR_SPRITE_DIRECTIONS 32
#define CAR_SPRITE_DIRECTIONS_MASK 0x1F
#define CAR_STATE_FRAME_MASK
#define CAR_STATE_RUNNING 0
#define CAR_STATE_WAITING_FOR_START_RACE 8
#define CAR_STATE_EXPLODING 0x81
#define CAR_STATE_WAITING_TO_APPEAR (0x82)
#define CAR_STATE_WAITING_TO_RUN 0x4
#define CAR_STATE_DESTROYED 0x80
enum
{
	CAR_PROPERTY_ACCELERATION = 0,
	CAR_PROPERTY_STEERING,
	CAR_PROPERTY_SPEED,
	CAR_PROPERTY_BRAKE,
	CAR_PROPERTY_GRIP,	
};
typedef struct 
{
	union
	{
		struct  
		{
			uint8_t acceleration;
			uint8_t steering;
			uint8_t speed;
			uint8_t brake;
			uint8_t grip;
		};
		uint8_t enhancements[5];			
	};
} carDataEnhacements_t;

typedef struct  
{
	union 
	{
		struct  
		{
			uint8_t baseAcceleration;
			uint8_t baseSteering;
			uint8_t baseSpeed;
			uint8_t baseBrake;
			uint8_t baseGrip;
		};
		uint8_t properties[5];
	};
} carData_t;
typedef struct
{
	// all int32_t are 16.16 fixed point
	// note: reordered to save space.
	vector_t currentSpeed;
	vector_t oldPosition;
	vector_t currentPosition;
	int32_t oldAngle;
	uint32_t currentAngle;			// note: normalized so that 0x1000000000 is a full 360
	uint32_t lastSpeed;				// magnitude of the last computed speed. Used for sound effects.
	int32_t currentAngularSpeed;
	const waypoint_t * currentWaypoint;		// used for AI
	const waypoint_t * currentLapWaypoint; // used both for player (To avoid cheating and to detect lap) and
	projectile_t projectile[NUMBER_OF_PROJECTILES_PER_CAR];		//
	uint16_t kills;
	uint16_t deaths;
	//
	int32_t steerAngle;
	int32_t angularvelocity;
	//
	uint32_t currentLapTime;
	uint32_t lastlapTime;
	uint32_t fastestlapTime;
	uint16_t bonusFrameTime;
	uint16_t timeWithoutHits;
	uint16_t cash;
	uint16_t tournamentPoints;
	uint8_t currentSpriteAngle;
	uint8_t control;
	uint8_t collisionCoolDown;
	uint8_t carType;
	uint8_t bonusType;
	uint8_t carState;
	uint8_t explosionFrame;
	uint8_t isSkidding;
	uint8_t hits;
	uint8_t reverse;
	int8_t lap;
	uint8_t finishLinePosition;
	uint8_t unlockedCars;
	carDataEnhacements_t enhancements;
} car_t;
extern const carData_t carData[];
void carPhysics(car_t pCarList[], int numberOfCars);
uint8_t pixelExactCollision(car_t *pCar1, car_t *pCar2,  vector_t * r1, vector_t *r2, vector_t *delta );
//void accelerateCar(car_t *pCar, uint8_t direction);
//void steer(car_t *pCar, uint8_t direction, uint8_t flags);
void setCarControls(car_t *pCar, uint8_t control);
void initCars(car_t *pCarList, int numberOfCars);
void killCar(car_t *pCar, uint8_t killer);

#endif /* CARPHYSICS_H_ */