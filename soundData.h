/*
 * soundData.h
 *
 * Created: 23/09/2019 17:57:27
 *  Author: PETN
 */ 


#ifndef SOUNDDATA_H_
#define SOUNDDATA_H_
#include <stdint.h>
#define MOTOR_NUM_ELEMENTS 963
#define MOTOR_SPS 30000
#define EXPLOSION_NUM_ELEMENTS 1711
#define EXPLOSION_SPS 30000
#define CARHIT_SPS 10000
#define CARHIT_NUM_ELEMENTS 4160
#define ROCKET_NUM_ELEMENTS 5100
#define ROCKET_SPS 11000
#define SKID_NUM_ELEMENTS 6011
#define SKID_SPS 6000

extern const int8_t skid_data[SKID_NUM_ELEMENTS];
extern const int8_t rocket_data[ROCKET_NUM_ELEMENTS];
extern const int8_t motor_data[MOTOR_NUM_ELEMENTS];
extern const int8_t explosion_data[EXPLOSION_NUM_ELEMENTS];
extern const int8_t carHit_data[CARHIT_NUM_ELEMENTS];

#endif /* SOUNDDATA_H_ */