/*
 * waypoint.h
 *
 * Created: 12/09/2019 11:32:31
 *  Author: PETN
 */ 


#ifndef WAYPOINT_H_
#define WAYPOINT_H_
/*
	waypoints are ellipse shaped area, where we suggest direction. Speed is calculated accordingly by the AI.
	Waypoints must be placed so that there is enough space for steering.
	When a waypoint is touched, the AI might choose one between two next waypoints.
	
	To check if the car is within the waypoint ellipse, one should check ((xcar-xellipse)/a) ^ 2 + ((ycar - yellipse)/b)^2 =< 1
	Since a and b  are in pixels (and they indicate the semiaxis), to avoid a division, we multiply both hands by a^2 and b^2
	therefore we check:
	((xcar-xellipse)*b) ^ 2 + ((ycar - yellipse)*a)^2 < (a*b)^2
	Therefore we call (R*a*b)^2 normR2 (normalized radius square) 
*/
typedef struct waypoint_t
{
	
	int32_t x;			// x position
	int32_t y;			// y position
	uint32_t normR2;	// normalized radius square, see above
	uint32_t halfWidth;		// in terms of pixel
	uint32_t halfHeight;	// in terms of pixel
	//uint32_t angle;			// recommended angle the car should point to when arriving to this waypoint.
	const struct waypoint_t *pNextWaypoint1;
	const struct waypoint_t *pNextWaypoint2;		
} waypoint_t;




#endif /* WAYPOINT_H_ */