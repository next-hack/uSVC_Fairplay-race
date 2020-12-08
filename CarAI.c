/*
 * CarAI.c
 *
 * Created: 12/09/2019 16:47:11
 *  Author: PETN
 */ 
#include "main.h" 

// this function tries to drive the car to follow waypoints
void carAI(car_t *pCar)
{
	int32_t carX = pCar->currentPosition.x >> 16;
	int32_t carY = pCar->currentPosition.y >> 16;
	const waypoint_t *pWaypoint = pCar->currentWaypoint;
	uint32_t currentAngle = pCar->currentAngle;
	// First check: are we in a waypoint?
	if ( (((carX - pWaypoint->x) * pWaypoint->halfHeight) * ((carX - pWaypoint->x) * pWaypoint->halfHeight) + ((carY - pWaypoint->y) * pWaypoint->halfWidth) * ((carY - pWaypoint->y) * pWaypoint->halfWidth)) < pWaypoint->normR2)
	{
		if (rand() & 0x1)	
			pCar->currentWaypoint = (waypoint_t*) pWaypoint->pNextWaypoint1;
		else
			pCar->currentWaypoint = (waypoint_t*) pWaypoint->pNextWaypoint2;
		pWaypoint = pCar->currentWaypoint;
	}
	// now, find the direction toward the waypoint
	uint32_t desiredAngle = (uint32_t) fastApproxAtan2( pWaypoint->x - carX, pWaypoint-> y - carY) + FIXED_PI / 2;
	// find where whe should steer.
    uint32_t deltaAngle =  desiredAngle - currentAngle;
	uint8_t reverse = pCar->reverse > 0;
	if (pCar->reverse)
		pCar->reverse--;
	//if (deltaAngle > 3/4 * FIXED_PI  && deltaAngle < 5 * 4 *FIXED_PI )
	
	if (deltaAngle > FIXED_PI)
	{
		// better rotate left
		setCarControls(pCar,/* reverse ? CAR_FLAGS_RIGHT :*/ CAR_FLAGS_LEFT);
	}
	else
	{
		if (deltaAngle)
			setCarControls(pCar,/* reverse ? CAR_FLAGS_LEFT :*/ CAR_FLAGS_RIGHT);		
	}
	// now, we need to check if the angle is too large for that distance, in relation with the speed. In fact at higher speeds, the maximum angle per frame is limited.
	// the steering radius depends on speed. 
	//uint32_t distanceQ = (carX - pWaypoint->x) * (carX - pWaypoint->x) + (carY - pWaypoint->y) * (carY - pWaypoint->y) + 1;	// +1 is added to avoid division by 0.
	uint32_t speedQ = FIXED_MUL(pCar->currentSpeed.x , pCar->currentSpeed.x) + FIXED_MUL(pCar->currentSpeed.y, pCar->currentSpeed.y);
	int32_t angleRatio = deltaAngle ; /// distanceQ;
	//brake if speed > MIN_SPEED_TO_RUN, and DELTA_ANGLE > 20 and distance 
	#define MIN_SPEEDQ_TO_RUN 0x40000
	#define ANGLE_TO_DISTANCE_RATIO (0x3000000)///(48 * 48))
	if (angleRatio > ANGLE_TO_DISTANCE_RATIO ||  -angleRatio > ANGLE_TO_DISTANCE_RATIO)
	{
		if (speedQ > MIN_SPEEDQ_TO_RUN )//)
		{
			setCarControls(pCar,CAR_FLAGS_BRAKING);
	//		REG_PORT_OUTSET0 = LEDPIN;
		}
		else
		{
//			REG_PORT_OUTCLR0 = LEDPIN;
		}
	}
	
	setCarControls(pCar, reverse ? CAR_FLAGS_REVERSE : CAR_FLAGS_FORWARD);		
	// now check the other cars position 
	if (pCar->bonusType == BONUS_FRONT_ROCKET || pCar->bonusType == BONUS_REAR_ROCKET)
	{
		int32_t x2, y2, vx2, vy2;
		x2 = pCar->currentPosition.x;
		y2 = pCar->currentPosition.y;
		uint8_t angle = (pCar->bonusType == BONUS_FRONT_ROCKET) ? pCar->currentSpriteAngle : (pCar->currentSpriteAngle + (CAR_SPRITE_DIRECTIONS / 2)) & CAR_SPRITE_DIRECTIONS_MASK;
		vx2 = +ROCKET_SPEED * sinTable[angle * (NUMBER_OF_SINTABLE_ENTRIES / CAR_SPRITE_DIRECTIONS)];
		vy2 = -ROCKET_SPEED * sinTable[ (angle * (NUMBER_OF_SINTABLE_ENTRIES / CAR_SPRITE_DIRECTIONS) + NUMBER_OF_SINTABLE_ENTRIES / 4) & SINTABLE_ENTRY_MASK];
		for (int i = 0 ; i < NUM_CARS; i++)
		{
			if (&cars[i] == pCar)
				continue;
			int32_t A = (cars[i].currentPosition.x - x2) >> 16;
			int32_t B = (cars[i].currentSpeed.x - vx2) >> 16;
			int32_t C = (cars[i].currentPosition.y - y2) >> 16;
			int32_t D = (cars[i].currentSpeed.y - vy2) >> 16;
			int32_t a = B * B + D * D; //FIXED_MUL(B, B) + FIXED_MUL(D, D);		// we can use a 16.16 because the speeds are rather small
			if (a > 0)		// a is almost never = 0. It is 0 only when the projectile and the car have the same speed. Very unlikely by design.
			{
				int32_t b = 2 * (A * B + C * D); // 2 * ((FIXED_MUL(A, B)) + (FIXED_MUL(C, D)));			// this could be large. 
				if (b < 0)	// is the "-sum" of the two solutions positive?
				{
#define RADIUSQ (8 * 8) //FIXED_MUL(0x80000, 0x80000)
					//int64_t c = FIXED_MUL(A, A) + FIXED_MUL(C, C) - RADIUSQ;		// this value is typically large. 
					int32_t c = A *A + C * C - RADIUSQ;
					if (c > 0)	// is the product of the two solutions positive? (i.e. is there a region in the future time, where collision would be detected ?)
					{
						//int64_t delta = FIXED_MUL(b, b) - 4 * (FIXED_MUL(a, c));
						int32_t delta = b * b - 4 * a * c;
						if (delta >= 0)
						{
							pCar->control |= CAR_FLAGS_FIRE;
							break;
						}
					}				
				}
				// 			
			}
		}
	}
	#define MINEBOMB_DEPLOY_CHANCE 512
	else if ((pCar->bonusType == BONUS_DEPLOY_MINE || pCar->bonusType == BONUS_DEPLOY_REMOTE_BOMB) && (rand() & 0xFFFF) < MINEBOMB_DEPLOY_CHANCE)
	{
		pCar->control |= CAR_FLAGS_FIRE;
	}
	else if (pCar->bonusType == BONUS_SET_OFF_REMOTE_BOMB)
	{
		int32_t x2, y2;
		x2 = pCar->projectile[0].currentPosition.x;
		y2 =  pCar->projectile[0].currentPosition.y;
		for (int i = 0 ; i < NUM_CARS; i++)
		{
			if (&cars[i] == pCar)
				continue;
			int32_t A = (cars[i].currentPosition.x - x2) >> 16;
			int32_t B = (cars[i].currentPosition.y - y2) >> 16;
			#define RADIUSQ_SET_OFF_BOMB 512
			if ((A * A + B * B) < RADIUSQ_SET_OFF_BOMB)
			{
				pCar->control |= CAR_FLAGS_FIRE;
				break;	
			}
		}
			
	}	
	else if (pCar->bonusType == BONUS_SPEED)
	{
		if (pCar->lastSpeed > 0x17000)
			pCar->control |= CAR_FLAGS_FIRE;
	}
}