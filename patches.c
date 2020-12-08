/*
 * patches.c
 *
 * Created: 24/09/2019 15:13:04
 *  Author: PETN
 */ 
#include "main.h" 
const char patch00[]  =
{
	0, PC_ENV_VOL,0,
	0, PC_ENV_SPEED, 16,
	0, PC_WAVE,0,
	4, PC_ENV_VOL, 0xff,
	10, PC_ENV_SPEED, -4,
	0x50, PC_NOTE_CUT,0,
	0,PATCH_END
};
const char patch01[]  =
{
	0, PC_ENV_VOL,0xFF,
	0, PC_ENV_SPEED, -5,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,	
	57, PC_NOTE_CUT,0,
	0,PATCH_END
};
const char patch02[]  =
{
	0, PC_ENV_VOL,0,
	0,PC_SAMPLE_RATE_NORMALIZE, 0,
	0, PC_ENV_SPEED, 16,
	0, PC_WAVE,2,
	4, PC_ENV_VOL, 0xff,
	10, PC_ENV_SPEED, -4,
	0x50, PC_NOTE_CUT,0,
	0,PATCH_END
};
const char patch_play_specified_sps[]  =
{
	0,PC_SAMPLE_RATE_SPECIFY, 1,
	57,PATCH_END
};
const char patch_rocket[]  =
{
	0, PC_ENV_VOL, 255,
	0, PC_ENV_SPEED, -8,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	1, PC_NOTE_DOWN,1,
	57, PC_NOTE_CUT,0,
	0,PATCH_END
};
const char patch_lap[] =
{
  0, PC_WAVE, 0,
  0, PC_ENV_VOL, 255,
  0, PC_PITCH, 80,
  0, PC_LOOP_START, 6,
  4, PC_ENV_VOL, 0,
  3, PC_ENV_VOL, 255,
  0, PC_LOOP_END, 2,
  5, PC_NOTE_CUT, 0,
  0, PATCH_END
 };
 const char patch_lapRecord[]  = 
 {
	 0, PC_WAVE, 0,
	 0, PC_ENV_VOL, 255,
	 0, PC_LOOP_START, 3,
	 0, PC_PITCH, 80,
	 4, PC_ENV_VOL, 0,
	 3, PC_ENV_VOL, 255,
	 4, PC_PITCH, 92,
	 4, PC_ENV_VOL, 0,
	 3, PC_ENV_VOL, 255,
	 0, PC_LOOP_END, 6,
	 5, PC_NOTE_CUT, 0,
	 0, PATCH_END,
 };
 const char patch_fastestLap[]  = 
 {
	 0, PC_WAVE, 0,
	 0, PC_ENV_VOL, 255,
	 0, PC_PITCH, 80,
	 0, PC_LOOP_START, 3,
	 4, PC_ENV_VOL, 0,
	 3, PC_ENV_VOL, 255,
	 4, PC_PITCH, 92,
	 4, PC_ENV_VOL, 0,
	 3, PC_ENV_VOL, 255,
	 0, PC_LOOP_END, 5,
	 5, PC_NOTE_CUT, 0,
	 0, PATCH_END,
 };

const char patch_bonus[] =
{
	  0, PC_WAVE, 2,
	  0, PC_PITCH, 70,
	  0, PC_ENV_VOL, 255,
	  0, PC_TREMOLO_LEVEL, 255,
	  0, PC_TREMOLO_RATE, 200,
	  0, PC_LOOP_START, 30,
	  1, PC_NOTE_UP, 1,
	  0, PC_LOOP_END, 1,
	  0, PC_NOTE_CUT, 0,
	  50, PATCH_END,
};
const char patch_readyset[]  = 
{
	0, PC_WAVE, 1,
	0, PC_ENV_VOL, 255,
	0, PC_ENV_SPEED, 0,
	0, PC_PITCH, 79,
	0, PC_TREMOLO_LEVEL, 255,
	0, PC_TREMOLO_RATE, 127,
	25, PC_NOTE_CUT, 0,
	20, PATCH_END,
};
const char patch_go[]  =
{
	0, PC_WAVE, 1,
	0, PC_ENV_VOL, 255,
	0, PC_ENV_SPEED, 0,
	0, PC_PITCH, 79,
	0, PC_TREMOLO_LEVEL, 255,
	0, PC_TREMOLO_RATE, 127,
	55, PC_NOTE_CUT, 0,
	20, PATCH_END,
};
const char patch_win[]  = 
{
	0, PC_WAVE, 4,
	0, PC_ENV_VOL, 255,
	0, PC_LOOP_START, 1,
	0, PC_PITCH, 60,
	5, PC_PITCH, 62,
	5, PC_PITCH, 64,
	4, PC_ENV_VOL, 0,
	10, PC_ENV_VOL, 255,
	0, PC_LOOP_END, 0,
	0, PC_PITCH, 60,
	5, PC_PITCH, 62,
	5, PC_PITCH, 67,
	20, PC_NOTE_CUT, 0,
	0, PATCH_END,
};
const char patch_lost[] = 
 {
	0, PC_WAVE, 1,
	0, PC_ENV_VOL, 255,
	0, PC_PITCH, 58,
	1, PC_PITCH, 60,
	1, PC_PITCH, 62,
	1, PC_PITCH, 64,
	1, PC_PITCH, 66,
	1, PC_PITCH, 68,
	1, PC_PITCH, 70,
	0, PC_ENV_SPEED, 12,
	10, PC_ENV_VOL, 0,
	5, PC_ENV_VOL, 255,
	0, PC_PITCH, 59,
	1, PC_PITCH, 61,
	1, PC_PITCH, 63,
	1, PC_PITCH, 65,
	1, PC_PITCH, 67,
	1, PC_PITCH, 69,
	10, PC_ENV_VOL, 0,
	5, PC_ENV_VOL, 255,
	0, PC_PITCH, 56,
	1, PC_PITCH, 58,
	1, PC_PITCH, 60,
	1, PC_PITCH, 62,
	1, PC_PITCH, 64,
	1, PC_PITCH, 66,
	1, PC_PITCH, 68,
	10, PC_ENV_VOL, 0,
	5, PC_ENV_VOL, 255,
	0, PC_PITCH, 55,
	1, PC_PITCH, 57,
	1, PC_PITCH, 59,
	1, PC_PITCH, 61,
	1, PC_PITCH, 63,
	1, PC_PITCH, 65,
	1, PC_PITCH, 67,
	10, PC_ENV_VOL, 0,
	5, PC_ENV_VOL, 255,
	0, PC_PITCH, 54,
	1, PC_PITCH, 56,
	1, PC_PITCH, 58,
	1, PC_PITCH, 60,
	1, PC_PITCH, 62,
	1, PC_PITCH, 64,
	1, PC_PITCH, 66,
	40, PC_NOTE_CUT, 0,
	0, PATCH_END,
};
const char patch_menuSelect[]  = 
{
	0, PC_PITCH, 72,
	0, PC_ENV_VOL, 50,
	0, PC_ENV_SPEED, 1,
	0, PC_WAVE, 8,
	5, PC_ENV_VOL, 0,
	5, PC_NOTE_CUT, 0,
	0, PATCH_END,
};
const char patch_menuMove[]  = 
{
	0, PC_PITCH, 67,
	0, PC_ENV_VOL, 50,
	0, PC_ENV_SPEED, 1,
	0, PC_WAVE, 0,
	5, PC_ENV_VOL, 0,
	5, PC_NOTE_CUT, 0,
	0, PATCH_END,
};

const  patch_t patches[]  =
{
	{0, 0, (uint8_t*) patch00, 256, 256},
	{0, 0, (uint8_t*) patch00, 256, 256},
	{0, 0, (uint8_t*) patch00, 256, 256},
	{0, 0, (uint8_t*) patch00, 256, 256},
	{2, DEFAULT_SOUND_NUMBER + 0, (uint8_t*) NULL, 0, MOTOR_NUM_ELEMENTS},
	{2, DEFAULT_SOUND_NUMBER + 1, (uint8_t*) patch01, 0, EXPLOSION_NUM_ELEMENTS},
	{2, DEFAULT_SOUND_NUMBER + 2, (uint8_t*) NULL, CARHIT_NUM_ELEMENTS - 1, CARHIT_NUM_ELEMENTS},
	{2, DEFAULT_SOUND_NUMBER + 3, (uint8_t*) patch_rocket, ROCKET_NUM_ELEMENTS - 1, ROCKET_NUM_ELEMENTS},
	{2, DEFAULT_SOUND_NUMBER + 4, (uint8_t*) NULL, SKID_NUM_ELEMENTS - 1, SKID_NUM_ELEMENTS},
	{0, 0, (uint8_t*) patch_lap, 0, 256},
	{0, 0, (uint8_t*) patch_bonus, 0, 256},
	{0, 0, (uint8_t*) patch_readyset, 0,  256},
	{0, 0, (uint8_t*) patch_go, 0,  256},
	{0, 0, (uint8_t*) patch_fastestLap, 0, 256},
	{0, 0, (uint8_t*) patch_lapRecord, 0, 256},
	{0, 0, (uint8_t*) patch_win, 0,  256},			// 15
	{0, 0, (uint8_t*) patch_lost, 0,  256},			// 16
	{0, 0, (uint8_t*) patch_menuSelect, 0, 256},	// 17
	{0, 0, (uint8_t*) patch_menuMove, 0, 256},		// 18

};

int getNumberOfPatches()
{
	return sizeof(patches) / sizeof(patch_t);
}