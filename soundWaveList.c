/*
 * soundWaveList.c
 *
 * Created: 24/09/2019 11:01:39
 *  Author: PETN
 */ 
#include "main.h"
// PUT HERE ALL YOUR SOUNDWAVES.  
const soundWave_t soundWaves[] = 
{ 
	{
		.length = 256,
		.wData = (int8_t *)  sineWave,
		.sps = 30000
	},
	{
		.length = 256,
		.wData = (int8_t *)  sawToothWave,
		.sps = 30000
	},
	{
		.length = 256,
		.wData = (int8_t *)  triangleWave,
		.sps = 30000
	},
	{
		.length = 256,
		.wData = (int8_t *)  squareWave25,
		.sps = 30000
	},
	{
		.length = 256,
		.wData = (int8_t *)  squareWave50,
		.sps = 30000
	},
	{
		.length = 256,
		.wData = (int8_t *)  squareWave75,
		.sps = 30000
	},
	{
		.length = 256,
		.wData = (int8_t *)  sineDistoWave1,
		.sps = 30000
	},
	{
		.length = 256,
		.wData = (int8_t *)  sineDistoWave2,
		.sps = 30000
	},
	{
		.length = 256,
		.wData = (int8_t *)  sineDistoWave3,
		.sps = 30000
	},
	{
		.length = 256,
		.wData = (int8_t *)  squareWave50Filtered,
	},
	{
		.length =  MOTOR_NUM_ELEMENTS,
		.wData = (int8_t *) motor_data,
		.sps = MOTOR_SPS
	},
	{
		.length = EXPLOSION_NUM_ELEMENTS,
		.wData = (int8_t *) explosion_data,
		.sps = EXPLOSION_SPS		
	},
	{
		.length = CARHIT_NUM_ELEMENTS,
		.wData = (int8_t *) carHit_data,
		.sps = CARHIT_SPS
	},
	{
		.length = ROCKET_NUM_ELEMENTS,
		.wData = (int8_t *) rocket_data,
		.sps = ROCKET_SPS
	},
	{
		.length = SKID_NUM_ELEMENTS,
		.wData = (int8_t *) skid_data,
		.sps = SKID_SPS
	},
};
int getNumberOfSoundWaves()
{
	return sizeof(soundWaves) / sizeof (soundWave_t);
}