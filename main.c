/*
 * uChipPlayNew.c
 *
 * Created: 30/05/2018 13:47:22
 * Author : PETN
 */ 
/*
	
	NOTE!!! In project settings, under ARM/GNU Linker, under "Additional Specs" select "Use syscall stubs (--specs=nosys.specs)", otherwise undefined reference error will occur!!!	 
*/
#include "main.h"
#include <stdlib.h>
#define ANIM_TILE_SIZE_X 4
#define ANIM_TILE_SIZE_Y 4
#define MAX_ENHANCEMENTS 5
#define BUTTON_1 1
#define BUTTON_2 2
#define BUTTON_3 4
#define BUTTON_4 8
#define BUTTON_L1 16
#define BUTTON_R1 32
#define BUTTON_L2 64
#define BUTTON_R2 128
#define CAR_EXPLOSION_FRAME_DELAY 3UL
#define CAR_WAIT_FRAME_DELAY 57UL
#define RACE_OVER_DELAY (57 * 5)
#define KILL_PRIZE 50
#define DEATH_PRIZE 80
#define RECORD_LAP_PRIZE 25
#define FASTEST_LAP_PRIZE 10
#define  RACE_RESULT_TEXT_Y_POSITION 5
#define TOURNAMENT_RESULT_TEXT_Y_POSITION 17
//
#define TEXT_FLAGS_COLOR_NORMAL 0
#define TEXT_FLAGS_COLOR_HIGHLIGHT 1
#define TEXT_FLAGS_ALIGN_LEFT 0
#define TEXT_FLAGS_ALIGN_RIGHT 2
#define TEXT_FLAGS_ALIGN_CENTER 4
//
#define PREVIEW_MAP_OFFSET_Y 4
#define PREVIEW_MAP_OFFSET_X 2
static const char *playerNames[] = {"YOU", "CPU 1", "CPU 2", "CPU 3"};
static const uint16_t positionPrize[] = {100, 50, 25, 0};
static const uint16_t carCosts[] = {5000, 1000, 500, 0};
static const uint8_t points[] = {10, 6, 3, 0};
//
typedef struct
{
	const char* text;
	uint8_t x;
	uint8_t y;
	uint8_t flags;			// alignment
	uint8_t selectedIndex;	// 255 = read only.
} menuText_t;
typedef struct
{
	const menuText_t *pTexts;
	uint16_t length;
} levelTexts_t;
typedef enum
{
	STATE_TITLE_SCREEN=0,
	STATE_TITLE_SCREEN_SHOW_PRESS_FIRE,
	STATE_MENU_MAIN,
	STATE_MENU_CREDITS,
	STATE_MENU_HOWTOPLAY,
	STATE_MENU_GAME_PLOT,
	STATE_MENU_TRACK_PREVIEW,
	STATE_MENU_CAR_SELECT,
	STATE_MENU_RACE_SUMMARY,
	STATE_PLAYING,
	STATE_RACE_OVER,
} gameState_t;
typedef struct
{
	int32_t cash;
	int32_t cost;
	int8_t carType;
	// being purchased enhancements.
	union
	{
		struct
		{
			int8_t buyAcceleration;
			int8_t buySteering;
			int8_t buySpeed;
			int8_t buyBrake;
			int8_t buyGrip;
		};
		uint8_t buyEnhancements[5];
	};
} selectCarMenu_t;
const char textPressAnyKey [] = "Press any key!";
const char textNext [] = "Next";
const uint8_t rainbow [] =
{
	COLOR_TORGB332(7, 0, 0),		// red
	COLOR_TORGB332(7, 1, 0),
	COLOR_TORGB332(7, 2, 0),
	COLOR_TORGB332(7, 3, 0),
	COLOR_TORGB332(7, 4, 0),
	COLOR_TORGB332(7, 5, 0),
	COLOR_TORGB332(7, 6, 0),
	COLOR_TORGB332(7, 7, 0),		// yellow
	COLOR_TORGB332(6, 7, 0),	
	COLOR_TORGB332(5, 7, 0),			
	COLOR_TORGB332(4, 7, 0),	
	COLOR_TORGB332(3, 7, 0),	
	COLOR_TORGB332(2, 7, 0),	
	COLOR_TORGB332(1, 7, 0),	
	COLOR_TORGB332(0, 7, 0),		// greeen	
	COLOR_TORGB332(0, 7, 1),
	COLOR_TORGB332(0, 7, 1),		
	COLOR_TORGB332(0, 7, 2),
	COLOR_TORGB332(0, 7, 2),
	COLOR_TORGB332(0, 7, 3),
	COLOR_TORGB332(0, 7, 3),		// cyan
	COLOR_TORGB332(0, 6, 3),
	COLOR_TORGB332(0, 5, 3),
	COLOR_TORGB332(0, 4, 3),
	COLOR_TORGB332(0, 3, 3),
	COLOR_TORGB332(0, 2, 3),
	COLOR_TORGB332(0, 1, 3),
	COLOR_TORGB332(0, 0, 3),		// blue
	COLOR_TORGB332(1, 0, 3),
	COLOR_TORGB332(2, 0, 3),
	COLOR_TORGB332(3, 0, 3),
	COLOR_TORGB332(4, 0, 3),
	COLOR_TORGB332(5, 0, 3),
	COLOR_TORGB332(6, 0, 3),
	COLOR_TORGB332(7, 0, 3),		// purple			
	COLOR_TORGB332(7, 0, 2),
	COLOR_TORGB332(7, 0, 2),
	COLOR_TORGB332(7, 0, 1),
	COLOR_TORGB332(7, 0, 1),
};	
const menuText_t flyingCityDescription[] =
{
//	{	.text = "0123456789ABCDEFGHIL0123456789ABCDEFGHIL", .x = N, .y = M, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "Flying Track", .x = 20, .y = 1, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255},
	{	.text = "As a workaround to global", .x = 26, .y = 4, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "sea level rise, many", .x = 26, .y = 5, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "flying cities have been", .x = 26, .y = 6, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "built. This track is", .x = 26, .y = 7, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "located in the outermost", .x = 26, .y = 8, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "part of a city flying", .x = 26, .y = 9, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "over the sea.", .x = 26, .y = 10, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},

};
const menuText_t sandyTrackDescription[] =
{
//	{	.text = "0123456789ABCDEFGHIL0123456789ABCDEFGHIL", .x = N, .y = M, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "Sandy Track", .x = 20, .y = 1, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "A track located in a", .x = 26, .y = 4, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "sandy environment, where", .x = 26, .y = 5, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "rich men can enjoy", .x = 26, .y = 6, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "swimming pools in the", .x = 26, .y = 7, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "middle of the desert,", .x = 26, .y = 8, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "watching their favorite", .x = 26, .y = 9, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "live motor competition.", .x = 26, .y = 10, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
};
const menuText_t toxicRefineryDescription[] =
{
//	{	.text = "0123456789ABCDEFGHIL0123456789ABCDEFGHIL", .x = N, .y = M, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "Toxic Refinery", .x = 20, .y = 1, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Located in an abandoned", .x = 26, .y = 4, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "refinery, this track is", .x = 26, .y = 5, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "one of the most toxic", .x = 26, .y = 6, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "places in the world.", .x = 26, .y = 7, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "No one dares coming here", .x = 26, .y = 8, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "except you, because", .x = 26, .y = 9, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "you have to.", .x = 26, .y = 10, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
};
const menuText_t commonTrackTexts[] =
{
	{	.text = "Fastest Lap:", .x = 5, .y = 16, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255},
	{	.text = "Number of Laps:", .x = 5, .y = 18, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 0},
	{	.text = textNext, .x = 20, .y = 23, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 2},
	{	.text = "Change Track", .x = 14, .y = 20, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 1},
};
const menuText_t carSetupMenuTexts[] =
{
	//	{	.text = "0123456789ABCDEFGHIL0123456789ABCDEFGHIL", .x = N, .y = M, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "Car Setup", .x = 20,  .y = 2, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255 },
	{	.text = "Cash", .x = 2,  .y = 15, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255 },
	{	.text = "Cost", .x = 2,  .y = 17, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255 },
	{	.text = "Your Car", .x = 3,  .y = 7, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 0 },
	{	.text = "Acceleration:", .x = 29,  .y = 8, .flags = TEXT_FLAGS_ALIGN_RIGHT, .selectedIndex = 1 },
	{	.text = "Steering:", .x = 29,  .y = 10, .flags = TEXT_FLAGS_ALIGN_RIGHT, .selectedIndex = 2 },
	{	.text = "Speed:", .x = 29,  .y = 12, .flags = TEXT_FLAGS_ALIGN_RIGHT, .selectedIndex = 3 },
	{	.text = "Brake:", .x = 29,  .y = 14, .flags = TEXT_FLAGS_ALIGN_RIGHT, .selectedIndex = 4 },
	{	.text = "Grip:", .x = 29,  .y = 16, .flags = TEXT_FLAGS_ALIGN_RIGHT, .selectedIndex = 5 },
};
const menuText_t startMenuTexts[] =
{
	//	{	.text = "0123456789ABCDEFGHIL0123456789ABCDEFGHIL", .x = N, .y = M, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "Main Menu", .x = 20, .y = 2, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Free Race", .x = 20, .y = 9, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 0	},
	{	.text = "Tournament", .x = 20, .y = 11, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 1	},
	{	.text = "Credits", .x = 20, .y = 13, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 2	},
	{	.text = "How To Play", .x = 20, .y = 15, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 3	},
	{	.text = "Game Plot", .x = 20, .y = 17, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 4	},
};
const menuText_t menuGamePlotTexts[] =
{
	//	{	.text = "0123456789ABCDEFGHIL0123456789ABCDEFGHIL", .x = N, .y = M, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "Ok, let's try to find a good tale!", .x = 20, .y = 1, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "It's 2220, and no one remembers when", .x = 20, .y = 3, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "the last disease outbreak began.", .x = 20, .y = 4, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "After the vaccine had been found and", .x = 20, .y = 5, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "the last of the no-vaxes has been", .x = 20, .y = 6, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "obliterated by the virus, everyone is", .x = 20, .y = 7, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "now vaxed, so the GUN (Government of", .x = 20, .y = 8, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "the Unique Nation) has released the", .x = 20, .y = 9, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "worldwide lockdown. People are now", .x = 20, .y = 10, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "thinking again about nonsenses,", .x = 20, .y = 11, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "getting upset by everything.", .x = 20, .y = 12, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "In particular, people are upset about", .x = 20, .y = 13, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "the lack of fairplay in racing", .x = 20, .y = 14, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "competitions. To solve this problem,", .x = 20, .y = 15, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "the GUN found a clever idea: \"Let's", .x = 20, .y = 16, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "redefine the meaning of the word", .x = 20, .y = 17, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "fairplay! From now on, weapons are", .x = 20, .y = 18, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "fairplay in racing competitions!\"", .x = 20, .y = 19, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "The fairplay race was born and all", .x = 20, .y = 20, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "the people were happy again!", .x = 20, .y = 21, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Go back, away from this nonsense!", .x = 20, .y = 23, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 0	},
};
const menuText_t menuGameCreditsTexts[] =
{
	//	{	.text = "0123456789ABCDEFGHIL0123456789ABCDEFGHIL", .x = N, .y = M, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "Fairplay race credits", .x = 20, .y = 1, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "This simple top-down game shows some", .x = 20, .y = 3, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "of 8-bpp tile mode 1 features.", .x = 20, .y = 4, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "See www.next-hack.com for details!", .x = 20, .y = 5, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "+GRAPHICS: ", .x = 20, .y = 7, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "- Road, black house, pavillon, trees,", .x = 20, .y = 8, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "red and yellow cars:", .x = 20, .y = 9, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "CraftPix.net - 2D & 3D Game Assets", .x = 20, .y = 10, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "- Water on sand Level:", .x = 20, .y = 11, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "BustBustedEd's Tips 'n Tricks", .x = 20, .y = 12, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "(CreativeEd on forums.rpgmakerweb.com)", .x = 20, .y = 13, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "- Other Graphics", .x = 20, .y = 14, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Nicola Wrachien", .x = 20, .y = 15, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "+ SOUND FX:", .x = 20, .y = 17, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Public domain", .x = 20, .y = 18, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "+ PROGRAMMING:", .x = 20, .y = 20, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Nicola Wrachien", .x = 20, .y = 21, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Back to main menu", .x = 20, .y = 23, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 0	},
};
const menuText_t menuHowToPlayTexts[] =
{
	//	{	.text = "0123456789ABCDEFGHIL0123456789ABCDEFGHIL", .x = N, .y = M, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "Keyboard Commands", .x = 20, .y = 1, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "a/d steer left/right", .x = 20, .y = 2, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "w/s accelerate/reverse", .x = 20, .y = 3, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "f: brake", .x = 20, .y = 4, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "g: activate current bonus", .x = 20, .y = 5, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Gamepad controls:", .x = 20, .y = 7, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "left/right steer left/right", .x = 20, .y = 8, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Y/1: accelerate", .x = 20, .y = 9, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "B/2: brake", .x = 20, .y = 10, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "A/3: reverse", .x = 20, .y = 11, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "X/4 or R2: activate current bonus", .x = 20, .y = 12, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Back to main menu", .x = 20, .y = 23, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 0	},
};
const menuText_t raceSummaryTexts[] =
{
//	{	.text = "0123456789ABCDEFGHIL0123456789ABCDEFGHIL", .x = N, .y = M, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "Race Summary", .x = 20, .y = 1, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
  	{	.text = "Position Player Kills Deaths Prize Pts", .x = 20, .y = 3, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text =    "#1", .x = 3, .y = 5, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "#2", .x = 3, .y = 6, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "#3", .x = 3, .y = 7, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "#4", .x = 3, .y = 8, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	{	.text = "* fastest lap", .x = 20, .y = 10, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "** lap record", .x = 20, .y = 11, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Driver Standings", .x = 20, .y = 13, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = "Player   Points   Cash", .x = 20, .y = 15, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	{	.text = textNext, .x = 20, .y = 23, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 0	},
};

const levelTexts_t levelTexts[] =
{
	{.pTexts = NULL , .length = 0}, // level 0 is the menu;
	{.pTexts = &flyingCityDescription[0], .length = sizeof(flyingCityDescription) / sizeof(menuText_t)},
	{.pTexts = &sandyTrackDescription[0], .length = sizeof(sandyTrackDescription) / sizeof(menuText_t)},
	{.pTexts = &toxicRefineryDescription[0], .length = sizeof(toxicRefineryDescription) / sizeof(menuText_t)},
};
typedef struct
{
	gameState_t gameState;
	uint8_t level;
	uint8_t positionCounter;
	uint8_t numberOfLaps;
	uint8_t tournament;
	uint8_t selectedItem;
	uint16_t stateTime;
	selectCarMenu_t selectCarMenu;
	uint32_t fastestLaps[sizeof(levelTexts) / sizeof(levelTexts_t)];
} game_t;
game_t game;
uint8_t currentLevel = 0;
car_t cars[NUM_CARS];
const uint16_t costPerEnhacement[] = {10, 25, 60, 150, 450};
static uint16_t g_frame = 0;
static const uint8_t mainMenuCoordinates [][4] =
{
	{15, 1, 11, 3},			// menu title
	{10, 8,20, 11},			// Menu list
};
static const uint8_t rectangleFullScreenCoordinates[][4] =
{
	{0, 0, 40, 25}
};
uint16_t getFrameTicks(void)
{
	return g_frame;
}

void reasonablyFastUint16ToArray(uint16_t  n, uint8_t *array, uint8_t zeroChar)
{
	/*
		Well this function is reasonably fast.  Other variants were tested, converting all the numbers between 0 and 65535.
		snprintf: 3.82s
		itoa 2.03s
		using divisions and "%": 1.79s
		this one: 0.37 s
	*/
	int number = n;
	int mult = 10000;
	int digit;
	int i = 0;
	for (digit = 0; digit < 10 && number >= 0; digit++)
	{
		number -= mult;
	}
	number += mult;
	array[i++] = (digit - 1) + zeroChar;
	mult = 1000;
	for (digit = 0; digit < 10 && number >= 0; digit++)
	{
		number -= mult;
	}
	number += mult;
	array[i++] = (digit - 1) + zeroChar;
	mult = 100;
	for (digit = 0; digit < 10 && number >= 0; digit++)
	{
		number -= mult;
	}
	number += mult;
	array[i++] = (digit - 1) + zeroChar;
	mult = 10;
	for (digit = 0; digit < 10 && number >= 0; digit++)
	{
		number -= mult;
	}
	number += mult;
	array[i++] = (digit - 1) + zeroChar;
	mult = 1;
	for (digit = 0; digit < 10 && number >= 0; digit++)
	{
		number -= mult;
	}
	number += mult;
	array[i++] = (digit - 1) + zeroChar;
}
void updateFixedSection(uint8_t force)
{
	static uint16_t deaths = 0xFFFF;
	static uint16_t kills = 0xFFFF;
	static uint8_t bonus = 0xFF;
	//
	static uint16_t lastLapTime = 0;
	static uint16_t lastFastestLapTime = 0;
	static uint8_t lastRemainingLap = 0;
	// death counter
	if (deaths != cars[0].deaths || force)
	{
		reasonablyFastUint16ToArray(cars[0].deaths, &fixedSectionMap[3], 0);
	    deaths = cars[0].deaths;
	}
	// kill counter
	if (kills != cars[0].kills || force)
	{
		reasonablyFastUint16ToArray(cars[0].kills, &fixedSectionMap[11], 0);
		kills = cars[0].kills;
	}
	// bonus
	if (bonus != cars[0].bonusType || force)
	{
		uint32_t *s = (uint32_t*) fixedSectionTileData[cars[0].bonusType];
		uint32_t *d = (uint32_t*) fixedSectionTiles[15];
		for (int i = 0; i < TILE_SIZE_X * TILE_SIZE_Y / 4; i++)
		{
			*d++= *s++;
		}
		bonus = cars[0].bonusType;
	}
	//  Lap Time
	uint32_t time = (cars[0].currentLapTime + (1 << 15)) >> 16;
	if (lastLapTime != time || force)
	{
		uint8_t minutes = time / 600;
		time -= minutes * 600;
		uint8_t seconds = time / 10;
		time -= seconds * 10;
		uint8_t buf[5];
		reasonablyFastUint16ToArray(minutes, buf, 0);
		//	fixedSectionMap[22] = 11;
		fixedSectionMap[19] = buf[3];
		fixedSectionMap[20] = buf[4];
		fixedSectionMap[21] = 11;
		reasonablyFastUint16ToArray(seconds, buf, 0);
		fixedSectionMap[22] = buf[3];
		fixedSectionMap[23] = buf[4];
		fixedSectionMap[24] = 12;
		reasonablyFastUint16ToArray(time, buf, 0);
		fixedSectionMap[25] = buf[4];		
	}
	lastLapTime = time;
	//
	//  fastestLap Time
	time = (game.fastestLaps[currentLevel - 1] + (1 << 15)) >> 16;
	if ((lastFastestLapTime != time || force ) && time != 0)
	{
		uint8_t minutes = time / 600;
		time -= minutes * 600;
		uint8_t seconds = time / 10;
		time -= seconds * 10;
		uint8_t buf[5];
		reasonablyFastUint16ToArray(minutes, buf, 0);
		//	fixedSectionMap[22] = 11;
		fixedSectionMap[27] = buf[3];
		fixedSectionMap[28] = buf[4];
		fixedSectionMap[29] = 11;
		reasonablyFastUint16ToArray(seconds, buf, 0);
		fixedSectionMap[30] = buf[3];
		fixedSectionMap[31] = buf[4];
		fixedSectionMap[32] = 12;
		reasonablyFastUint16ToArray(time, buf, 0);
		fixedSectionMap[33] = buf[4];
	}
	lastFastestLapTime = time;	 
	//	
	if (game.gameState == STATE_RACE_OVER)
	{
		for (int i = 19; i < 39; i++)
			fixedSectionMap[i] = 10;
		for (int i = 0; i < 9; i++)
			fixedSectionMap[i + 31] = MAX_FIXED_SECTION_TILES - 9 + i;		
	}
	else
	{
		uint8_t buf[5];
		uint8_t remainingLaps = game.numberOfLaps - cars[0].lap;
		if (lastRemainingLap != remainingLaps)
		{
			reasonablyFastUint16ToArray(remainingLaps, buf, 0);
			fixedSectionMap[37] = buf[3];
			fixedSectionMap[38] = buf[4];			
		}
		lastRemainingLap = remainingLaps;
	}
}

inline void putSymbolToFixedTile(uint8_t fixedTileNumber, uint8_t symbol)
{
	uint32_t *s = (uint32_t*) fixedSectionTileData[symbol];
	uint32_t *d = (uint32_t*) fixedSectionTiles[fixedTileNumber];
	for (int i = 0; i < TILE_SIZE_X * TILE_SIZE_Y / 4; i++)
	{
		*d++= *s++;
	}	
}
void initFixedSection()
{
	#if USE_SECTION != NO_FIXED_SECTION
	// tiles: 0-9 have the corresponding number
	// tile 10: white space
	// tile 11: ":"
	// tile 12: "."
	// tile 13: death symbol
	// tile 14: kill symbol
	// tile 15: "current bonus"
	// tite 16: "L" (when the race is not over)
	memset(fixedSectionMap, 0, sizeof(fixedSectionMap));
	/* 
		D 65535 K 65535 B 99:99:9 99:99:9 L 00
	*/
	const uint8_t charList[] = {'0','1','2','3','4','5','6','7','8','9',' ',':','.'};
	for (int i = 0; i < sizeof(charList); i++)
	{
		putCharInTile(NULL, charList[i], 0xFF, 0x00, 0 , (uint8_t *) fixedSectionTiles[i], 1);
	}
	putCharInTile(NULL, 'L', 0xFF, 0x00, 0 , (uint8_t *) fixedSectionTiles[16], 1);
	const uint8_t symbolList[]= {SYMBOL_DEATHS, SYMBOL_KILLS, SYMBOL_NOBONUS};
	for (int i = 0; i < sizeof(symbolList); i++)
	{
		putSymbolToFixedTile(i + 13, symbolList[i]);
	}
	for (int i = 0; i < sizeof(fixedSectionMap); i++)
	{
		fixedSectionMap[i] = 10;		// "white"- actually black!- spaces
	}
	fixedSectionMap[1] = 13;		// death symbol
	fixedSectionMap[9] = 14;		// kills symbol
	fixedSectionMap[17] = 15;		// bonus
	fixedSectionMap[36] = 16;

	updateFixedSection(1);		
	//
	#endif
}
void printLineInMenu (const char * text, int8_t col, uint8_t row, uint8_t flags)
{
	uint16_t charTileSet = (128 - ' ') * (0 != (flags & TEXT_FLAGS_COLOR_HIGHLIGHT));
	if (flags & TEXT_FLAGS_ALIGN_RIGHT)
		col -= strlen(text);	
	else if (flags & TEXT_FLAGS_ALIGN_CENTER)
		col = col - strlen(text) / 2;
	if (col < 0)
		col = 0;
	const uint8_t *pText = (uint8_t*) text;
	for (uint8_t c = col; c < VRAMX && *pText != 0; c++)
		vram[ c + row * VRAMX] = (uint32_t) &tiles[*pText++ + charTileSet - ' '];
}
void goToRace(uint8_t level, uint8_t playerCarType)
{
	game.gameState = STATE_PLAYING;
	game.positionCounter = 0;
	changeLevel(level,1);
	initCars(&cars[0], NUM_CARS);
	cars[0].carType = playerCarType;
	initFixedSection();
	triggerNote(OWN_CAR_ENGINE_CHANNEL, 4, 23 , 255);	// our own car
	triggerNote(CLOSEST_CAR_ENGINE_CHANNEL, 4, 23 , 255);	// the closest car
}
void initMenu()		// create font tiles, and copies menu tiles in ram.
{  
	// first, clear the tile data
	memset(&tiles[0], 0, sizeof(tiles));
	// then put all the 96 ascii chars there, in two colors. 
	uint16_t nTile = 0;
	for (int c = 0 ; c < 2; c++)	// two colors
	{
		for (int i  = 0 ; i < 128 - ' '; i++)
		{
			putCharInTile(NULL, i + ' ', c == 0 ?  COLOR_TORGB332(3, 4, 2) : COLOR_TORGB332(7, 7, 2) , COLOR_TORGB332(2, 2, 1), 0, (uint8_t*) &tiles[i + c * (128 - ' ')], 0);
			nTile++;
		}
		// now copy the menu tiles		
	}
	memcpy(&tiles[nTile], tileData_menu, sizeof(tileData_menu));
	nTile += MAXTILEINDEX_MENU;
	// now we need "+" in black, yellow and blue.
	const uint8_t plusColors[4] =
	{
		COLOR_TORGB332(0, 0, 0),  // available enhancement, but not yet installed
		COLOR_TORGB332(7, 7, 0),  // enhancements installed by default.
		COLOR_TORGB332(0, 3, 3),  // enhacement already bought.
		COLOR_TORGB332(0, 7, 3)  // enhacement being bought.
	};
	for (int i = 0 ; i < 4; i++)
	{
		putCharInTile(NULL, '+', plusColors[i], COLOR_TORGB332(2, 2, 1), 0, (uint8_t*)&tiles[nTile++], 0);   	
	}
	videoData.ramTiles = nTile;	
}
void drawRoundedRectangle(const uint8_t coordinates[4], uint8_t isSelected)
{
	uint8_t x = coordinates[0], y = coordinates[1], w = coordinates[2], h = coordinates[3];
	uint8_t offset = 2 * (128 - ' ');
	for (int yy = y; yy < y + h; yy++)
	{
		for (int xx = x; xx < x + w; xx++)
		{
			uint16_t tile = 1;
			if (xx == x)
			{
				if (yy == y)				// top angle?
					tile = 2;
				else if (yy == y + h - 1)
					tile = 7;
				else
					tile = 5;
			}
			else if (xx == x + w - 1)
			{
				if (yy == y)				// top angle?
					tile =  4;
				else if (yy == y + h - 1)
					tile = 9;
				else
					tile = 6;
			}
			else
			{
				if (yy == y)				// top ?
					tile = 3;
				else if (yy == y + h - 1)
					tile = 8;
			}
			if (isSelected && tile != 1)
				tile += 8;	
			vram[yy * VRAMX + xx] = (uint32_t) &tiles[tile + offset];		
		}	
	}
}
void getEnhancement(uint8_t number, uint8_t *base, uint8_t *bought, uint8_t *buying)
{
	*bought = cars[0].enhancements.enhancements[number];
	*buying = game.selectCarMenu.buyEnhancements[number];
	*base = carData[game.selectCarMenu.carType].properties[number];

}
void drawGenericMenu(uint8_t selectedItem, const uint8_t rectangleCoordinates[][4], uint8_t numberOfRectangles, const menuText_t *menuTexts, uint8_t numberOfMenuTexts)
{
	removeAllSprites(0);
	// clear video data with the correct tile address
	for (int i = 0; i < sizeof(vram) / sizeof(vram[0]); i++)
		vram[i] = (uint32_t) &tiles[ 2 * (128 - ' ')];		// background menu color
	// draw rectangles
	for (int i = 0; i < numberOfRectangles; i++)
	{
		drawRoundedRectangle(rectangleCoordinates[i], 0);
	}
	// draw texts
	for (int i = 0; i < numberOfMenuTexts; i++)
	{
		printLineInMenu(menuTexts[i].text, menuTexts[i].x, menuTexts[i].y, menuTexts[i].flags | (TEXT_FLAGS_COLOR_HIGHLIGHT * (selectedItem == menuTexts[i].selectedIndex)));
	}	
}
void initPreviewMap(uint8_t level)
{
	/* videoData.ramTiles is 214 because: 96 * 2 tiles are used by chars. there are 4 additional chars for car enhanchements (+)
		And there are up to 18 tiles for the menu (this might be reduced to 10).
		Since we have more than 228+100 tiles, and since each preview map is only 100x100, we can just copy the tiles, and put on the map an incrementing number,
		starting from ramtiles.
	*/
	const level_t *pLevel = &levels[level];
	uint16_t tileIndex = videoData.ramTiles;
	for (int y = 0; y < PREVIEW_MAPSIZEY * 2; y++)
	{
		for (int x = 0; x < PREVIEW_MAPSIZEX * 2; x++)
		{
			// get meta tile
			uint16_t metaTile = pLevel->pPreviewMap[(x >> 1) + (y >> 1) * PREVIEW_MAPSIZEX];
			// get tile
			uint16_t tile = pLevel->pPreviewMetaTiles[4 * metaTile + (x & 1) + (y & 1) * 2];
			// copy it
			memcpy (&tiles[tileIndex], &pLevel->pTiles[TILE_SIZE_X * TILE_SIZE_Y * tile], TILE_SIZE_X * TILE_SIZE_Y);
			tileIndex++;
		}
	}	
}
void checkRaceResults(uint8_t *positions, uint16_t *cashWon, uint8_t *fastestLapCar)
{
	/*
	Cars positions are determined only when they cross the finish line.
	Therefore, unless the player is the last one, some positions will be likely missing.
	Since there are 5 seconds of wait time before returning to the menu, other cars might
	cross the finish line. Therefore we must check the position for all cars, and not just for the player. */
	uint8_t unknownPosPlayerNumber;
    uint8_t lastPos = 0;
	do
	{
		uint8_t playerWithUnknownPos[NUM_CARS];
		unknownPosPlayerNumber = 0;
		for (int i = 0; i < NUM_CARS; i++)
		{
			uint8_t pos = cars[i].finishLinePosition;
			if (pos > lastPos)
			{
				lastPos = pos;
			}
			if (pos == 0)
			{
				playerWithUnknownPos[unknownPosPlayerNumber] = i;
				unknownPosPlayerNumber++;
			}
			else
			{	// a position has been found: order the array
				positions[pos - 1] = i;
				int16_t cash = positionPrize[pos - 1] + cars[i].kills * KILL_PRIZE - cars[i].deaths * DEATH_PRIZE;
				if (cash < 0)
					cash = 0;
				cashWon[i] = cash;
			}
		}
        /*
            Now we know the highest position that has been determined (e.g. 2). We must determine the other positions (e.g. 3 and 4)
        */
        // check which players have the same largest lap number
        uint8_t highestLap = 0;
        for (int i = 0; i < unknownPosPlayerNumber; i++)
        {
            if (cars[playerWithUnknownPos[i]].lap > highestLap)
                highestLap = cars[playerWithUnknownPos[i]].lap;
        }
        // highest lap determined. Find all the players with this lap time
        for (int i = 0; i < unknownPosPlayerNumber; i++)
        {
            if (cars[playerWithUnknownPos[i]].lap == highestLap)
            {
                lastPos++;
                cars[playerWithUnknownPos[i]].finishLinePosition = lastPos;	// TODO: MODIFY THIS AS RANDOM
            }
        }
	} while (unknownPosPlayerNumber > 0);
	// now check fastest lap.
	uint32_t fastestLap = 0xFFFFFFFF;
	int8_t fastestCar = -1;
	for (int c = 0; c < NUM_CARS; c++ )
	{
	    // let's
		fastestLapCar[c] = 0;
		if (cars[c].fastestlapTime != 0 && cars[c].fastestlapTime < fastestLap)
		{
			fastestCar = c;
			fastestLap = cars[c].fastestlapTime;
		}
	}
	if (fastestCar != -1)
	{
		if (fastestLap == game.fastestLaps[game.level - 1])
		{
			fastestLapCar[fastestCar] = 2;
			cashWon[fastestCar] += RECORD_LAP_PRIZE;
		}
		else
		{
			fastestLapCar[fastestCar] = 1;
			cashWon[fastestCar] += FASTEST_LAP_PRIZE;
		}
	}
	//
}
void checkResultsAndDrawRaceSummaryMenu(uint8_t selectedIndex)
{
	uint16_t cashWon[NUM_CARS];
	uint8_t positions[NUM_CARS];
	uint8_t gotFastestLap[NUM_CARS];
	checkRaceResults(positions, cashWon, gotFastestLap);
	drawGenericMenu(selectedIndex, rectangleFullScreenCoordinates, sizeof(rectangleFullScreenCoordinates) / sizeof(rectangleFullScreenCoordinates[0]), raceSummaryTexts, sizeof(raceSummaryTexts)/sizeof(menuText_t));
	//
	for (int i = 0; i < NUM_CARS; i++)
	{
		cars[i].cash += cashWon[i];
	}
	// let's draw the results.
	const char *fastLapTexts[] = {"","*","**"};
//  How textPositions are determined:		
//	{	.text = "0123456789ABCDEFGHIL0123456789ABCDEFGHIL", .x = N, .y = M, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
//	{	 .text = "Position Player Kills Deaths Prize Pts", .x = 20, .y = 3, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},
	const uint8_t textPositions[] = {6,12, 19, 25, 32,37};
	for (int p = 0; p < NUM_CARS; p++)
	{
		int car = positions[p];
		car_t *pCar = &cars[car];
		uint8_t ptsBuf[6];
		uint8_t cashBuf[6];
		uint8_t deathsBuf[6];
		uint8_t killsBuf[6];
		uint8_t pts = points[p] + gotFastestLap[car] + pCar->kills;
		reasonablyFastUint16ToArray(pts,ptsBuf, '0');
		pCar->tournamentPoints += pts;
		reasonablyFastUint16ToArray(cashWon[car], cashBuf, '0');
		reasonablyFastUint16ToArray(pCar->deaths, deathsBuf, '0');
		reasonablyFastUint16ToArray(pCar->kills, killsBuf, '0');		
		ptsBuf[5] = 0;
		cashBuf[5] = 0;
		deathsBuf[5] = 0;
		killsBuf[5] = 0;
		char *texts[6];
		texts[0] = (char*) fastLapTexts[gotFastestLap[car]];
		texts[1] = (char*) playerNames[car];
		texts[2] = (char*) killsBuf;
		texts[3] = (char*) deathsBuf;
		texts[4] = (char*) cashBuf;
		texts[5] = (char*) &ptsBuf[2];
		for (int i = 0; i < sizeof(textPositions); i++)
		{ 
			printLineInMenu(texts[i], textPositions[i], p + RACE_RESULT_TEXT_Y_POSITION, TEXT_FLAGS_ALIGN_CENTER);
		}
	}
	// sort cars by points
	// let's reuse positions. For sure positions are (shuffled) 0, 1, 2, 3
	uint8_t bestCar;
	uint16_t largestCash;
	uint16_t highestPts;
	for (int i = 0; i < NUM_CARS; i++)
	{
		uint8_t car = positions[i];
		highestPts = cars[car].tournamentPoints;
		largestCash = cars[car].cash;
		bestCar = i;
		for (int j = i + 1; j < NUM_CARS; j++)
		{
			uint8_t car2 = positions[j];
			if (cars[car2].tournamentPoints > highestPts || (cars[car2].tournamentPoints == highestPts && cars[car2].cash > largestCash))
			{
				highestPts = cars[car2].tournamentPoints;
				largestCash = cars[car2].cash;			// this might be actually smaller
				bestCar = j;
			}
		}
		// swap
		positions[i] = positions[bestCar];
		positions[bestCar] = car;
	}
	// draw the summary
	//	{	.text = "0123456789ABCDEFGHIL0123456789ABCDEFGHIL", .x = N, .y = M, .flags = TEXT_FLAGS_ALIGN_LEFT, .selectedIndex = 255	},
	//	{	.text =          "Player   Points   Cash", .x = 20, .y = 14, .flags = TEXT_FLAGS_ALIGN_CENTER, .selectedIndex = 255	},

	const uint8_t summaryTextPositions [] = {11 , 20, 28}; 

	for (int p = 0; p < NUM_CARS; p++)
	{
		int car = positions[p];
		car_t *pCar = &cars[car];
		uint8_t ptsBuf[6];
		uint8_t cashBuf[6];
		reasonablyFastUint16ToArray(pCar->tournamentPoints,ptsBuf, '0');
		reasonablyFastUint16ToArray(pCar->cash, cashBuf, '0');
		ptsBuf[5] = 0;
		cashBuf[5] = 0;

		char *texts[6];
		texts[0] = (char*) playerNames[car];
		texts[1] = (char*) &ptsBuf[2];
		texts[2] = (char*) cashBuf;
		for (int i = 0; i < sizeof(summaryTextPositions); i++)
		{
			printLineInMenu(texts[i], summaryTextPositions[i], p + TOURNAMENT_RESULT_TEXT_Y_POSITION, TEXT_FLAGS_ALIGN_CENTER);
		}
	}
}
void drawPreviewTrackMenu(uint8_t selectedIndex, uint8_t level)
{
	drawGenericMenu(selectedIndex, rectangleFullScreenCoordinates, sizeof(rectangleFullScreenCoordinates) / sizeof(rectangleFullScreenCoordinates[0]), levelTexts[level].pTexts, levelTexts[level].length);
	// draw common track text data.
	for (int i = 0; i < sizeof(commonTrackTexts)/sizeof(commonTrackTexts[0]) - (game.tournament != 0); i++)
	{
		printLineInMenu(commonTrackTexts[i].text, commonTrackTexts[i].x, commonTrackTexts[i].y, commonTrackTexts[i].flags | (TEXT_FLAGS_COLOR_HIGHLIGHT * (selectedIndex == commonTrackTexts[i].selectedIndex)));
	}	
	// print fastest lap
	// print number of laps
	uint8_t buffer[8] = "--:--.-";		// fastest lap: mm:ss.d	
	uint16_t fastestLap = (game.fastestLaps[level - 1] + (1 << 15)) >> 16;
	uint8_t minutes = fastestLap / 600;
	fastestLap -= minutes * 600;
	uint8_t seconds = fastestLap / 10;
	fastestLap -= seconds * 10;
	// get fastest lap time
	uint8_t buf[5];
	if (fastestLap != 0)
	{
		reasonablyFastUint16ToArray(minutes, buf, '0');
		buffer[0] = buf[3];
		buffer[1] = buf[4];
		reasonablyFastUint16ToArray(seconds, buf, '0');
		buffer[3] = buf[3];
		buffer[4] = buf[4];
		reasonablyFastUint16ToArray(fastestLap, buf, '0');
		buffer[6] = buf[4];
	}
	reasonablyFastUint16ToArray(game.numberOfLaps, buf, '0');
	buf[2] = buf[3];
	buf[3] = buf[4];
	buf[4] = 0;
	printLineInMenu((char*) &buffer[0], 18, 16, TEXT_FLAGS_ALIGN_LEFT);
	printLineInMenu((char*) &buf[2], 21, 18, TEXT_FLAGS_ALIGN_LEFT | (TEXT_FLAGS_COLOR_HIGHLIGHT * (selectedIndex == 0)));
	//
	int tileIndex = videoData.ramTiles;
	for (int y = -1 ; y < PREVIEW_MAPSIZEY * 2 + 1; y++)
	{
		for (int x = -1; x < PREVIEW_MAPSIZEX * 2 + 1; x++)
		{
			uint32_t tileAddr;
			if (y == -1 || x == - 1 || y ==  PREVIEW_MAPSIZEY * 2 || x == PREVIEW_MAPSIZEX * 2)
				tileAddr =  (uint32_t)  &tiles[ 2 * (128 - ' ')];
			else
			{
				tileAddr = (uint32_t) &tiles[tileIndex];
				tileIndex++;
			}
			vram[ (x + PREVIEW_MAP_OFFSET_X) + (y + PREVIEW_MAP_OFFSET_Y) * VRAMX] = tileAddr;
		}		
	}
}
void drawCarSelectMenu(uint8_t selectedItem)
{
	const uint8_t coordinates [][4] =
	{
		{15, 1, 11, 3},			// menu title
		{1, 6, 12, 6},			// car preview
		{1, 14, 12, 5},			// funds
		{15, 6, 24, 13},		// car properties
		{9, 21, 22, 3}			// next
	};
	drawGenericMenu(selectedItem, coordinates, sizeof(coordinates) / sizeof(coordinates[0]), carSetupMenuTexts, sizeof(carSetupMenuTexts) / sizeof(carSetupMenuTexts[0]));

	//
	uint8_t buffer[5];
	reasonablyFastUint16ToArray(game.selectCarMenu.cash, buffer, '0' - ' ');
	// numbers
	for (int i = 0; i < 5; i++)
		vram[VRAMX * 15 + 7 + i] = (uint32_t) &tiles[buffer[i]];
	reasonablyFastUint16ToArray(game.selectCarMenu.cost, buffer, '0' - ' ');
	for (int i = 0; i < 5; i++)
		vram[VRAMX * 17 + 7 + i] = (uint32_t) &tiles[ buffer[i]];
	// print cash
	// selectable
	if (game.selectCarMenu.cash < game.selectCarMenu.cost)
		printLineInMenu("No enough cash!", 20, 22, TEXT_FLAGS_ALIGN_CENTER | (TEXT_FLAGS_COLOR_HIGHLIGHT * (selectedItem == 6)));
	else
		printLineInMenu(textNext, 20, 22, TEXT_FLAGS_ALIGN_CENTER | (TEXT_FLAGS_COLOR_HIGHLIGHT * (selectedItem == 6)));
	// now print the enhancements
	for (int en = 0; en < 5; en ++)
	{
		int y = 6 + 2 + 2 * en;
		uint8_t bought, buying, base;
		getEnhancement(en, &base, &bought, &buying);
		int col = 30;
		for (int i = 0; i < base + MAX_ENHANCEMENTS; i++)
		{
			uint8_t color = 0;  // default not installed	
			if ( i < base)
			{
				color = 1;	// base 
			}
			else if (i < (base + bought))
			{
				color = 2;
			}
			else if (i < (base + bought + buying))
			{
				color = 3;
			}
			vram[ col + y * VRAMX] = (uint32_t) &tiles[videoData.ramTiles - 4 + color];
			col++;
		}
		// clear everything after the maximum enhancement
		for (; col < 38; col++)
			vram[ col + y * VRAMX] = (uint32_t) &tiles[2 * (128 - ' ') + 1];
	}
		//
	// put car
	putSprite(0, 56, 80, SPRITE_FLAGS_HANDLE_CENTER | GET_CAR_ROTFLAGS(game.selectCarMenu.carType, 0, 8), GET_CAR_REAL_FRAME(game.selectCarMenu.carType, 0, 8));
	// draw sprites
	drawSprites();
}
void gameRace(uint16_t key)
{	
	static uint8_t animatedTileTime = 0;
	static uint8_t animatedTileFrame = 0;
	int cameraX = 160;
	int cameraY = 100;	
	// AI
	carAI(&cars[1]);
	carAI(&cars[2]);
	carAI(&cars[3]);
//  Actually no, we want the race to continue even if it is over... after all this is fairplay race :)
// 	for (int i = 0; i < NUM_CARS; i++)
// 	{
// 		if (cars[i].lap >= game.numberOfLaps)
// 		{
// 			// stop cars that finished the race...
// 			cars[i].control = 0;
// 		}
// 	}
	carPhysics(&cars[0], NUM_CARS);
	// check Lap waypoints.
	for (int i = 0; i < NUM_CARS; i++)
	{
		car_t * pCar = &cars[i];
		int32_t carX = pCar->currentPosition.x >> 16;
		int32_t carY = pCar->currentPosition.y >> 16;
		const waypoint_t *pWaypoint = pCar->currentLapWaypoint;
		// Did we arrive to the next waypoint?
		if ( (((carX - pWaypoint->x) * pWaypoint->halfHeight) * ((carX - pWaypoint->x) * pWaypoint->halfHeight) + ((carY - pWaypoint->y) * pWaypoint->halfWidth) * ((carY - pWaypoint->y) * pWaypoint->halfWidth)) < pWaypoint->normR2)
		{
			// point to next waypoint
			pCar->currentLapWaypoint = (waypoint_t*) pWaypoint->pNextWaypoint1;
			if (pWaypoint == currentLevelData.lapWaypoint)		// did we cross the finish line?
			{
				if (pCar->lap > 0)	// the first waypoint crossing does not count...
				{
					pCar->lastlapTime = pCar->currentLapTime;
					pCar->currentLapTime = 0;  // reset current Lap
					uint8_t bestLap = 0;
					if (pCar->lastlapTime < pCar->fastestlapTime || pCar->fastestlapTime == 0)
					{
						pCar->fastestlapTime = pCar->lastlapTime;
						bestLap = 1;
					}
					if (pCar->lastlapTime < game.fastestLaps[game.level - 1] || game.fastestLaps[game.level - 1] == 0)
					{
						game.fastestLaps[game.level - 1] = pCar->lastlapTime;
						bestLap = 2;
					}
					if (pCar->lap == game.numberOfLaps)
					{
						game.positionCounter++;
						pCar->finishLinePosition = game.positionCounter;
					}
					if (i == 0)
					{
						const uint8_t bestLapSounds[] = {9, 13, 14};
						if (pCar->lap == game.numberOfLaps)
						{
							const char winner[] = "You won! ";
							const char loser[] =  "Race over";
							for (int n = 0; n < 9; n++)
							{
								putCharInTile(NULL, 
									pCar->finishLinePosition == 1 ? winner[n] : loser[n], 
									pCar->finishLinePosition == 1 ? COLOR_TORGB332(0,7,2) : COLOR_TORGB332(6,0, 0), 0, 0, 
									(uint8_t *) &fixedSectionTiles[MAX_FIXED_SECTION_TILES - 9 + n], 
									1);								
							}
							if (pCar->finishLinePosition == 1)
								triggerNote(PROJECTILE_CHANNEL, 15, 50, 255);
							else
								triggerNote(PROJECTILE_CHANNEL, 16, 50, 255);						
							game.gameState = STATE_RACE_OVER;
							game.stateTime = g_frame;
						}
						else
						{
							triggerNote(PROJECTILE_CHANNEL, bestLapSounds[bestLap], 50, 255);						
						}
					}
				}
				pCar->lap++;
						
			}
			//	REG_PORT_OUTSET0 = LEDPIN;
		}
		else
		{
			pCar->currentLapTime += 11498;			// this is 10*65536/57...
		}
	}
	// sound with doppler effect
	// if speed == 0, the sample plays at 0x0043
	// Car sounds: ours depend only on our speed. The car must be alive
	if (cars[0].carState == CAR_STATE_RUNNING)
		setMixerPlaySpeed(OWN_CAR_ENGINE_CHANNEL, 0x0043 + (cars[0].lastSpeed >> 8));
	// let's find the closest car
	int32_t squareDistance = 0x7FFFFFFF;
	int8_t closestCar = 0;
	int32_t ourX = cars[0].currentPosition.x >> 16;
	int32_t ourY = cars[0].currentPosition.y >> 16;
	for (int i = 1; i < NUM_CARS; i++)
	{
		int32_t pX = cars[i].currentPosition.x >> 16;
		int32_t pY = cars[i].currentPosition.y >> 16;
		int32_t	sd = (pX - ourX) * (pX - ourX) + (pY - ourY) * (pY - ourY);
		if (sd < squareDistance && cars[i].carState == CAR_STATE_RUNNING)	// the car must be close but also alive!
		{
			closestCar = i;
			squareDistance = sd;
		}
	}
	// now we have the closest car. let's calculate the doppler shift.
	// diresction of closest opponent car's velocity
	uint32_t angleSource = fastApproxAtan2(cars[closestCar].currentSpeed.x, cars[closestCar].currentSpeed.y);
	// our speed direction.
	uint32_t angleObserver = fastApproxAtan2(cars[0].currentSpeed.x, cars[0].currentSpeed.y);
	// The direction of the sound wave
	uint32_t angleWaveStoO = fastApproxAtan2(cars[0].currentPosition.x - cars[closestCar].currentPosition.x, cars[0].currentPosition.y -  cars[closestCar].currentPosition.y);
	uint32_t angleWaveOtoS = angleWaveStoO + FIXED_PI;//fastApproxAtan2(cars[closestCar].currentPosition.x - cars[0].currentPosition.x, cars[closestCar].currentPosition.y -  cars[0].currentPosition.y);
	// projection of the speed (actually last speed to save us a square root) along the sound direction.
	uint32_t tableAngle90source = (((angleSource - angleWaveStoO  + FIXED_PI / 2)) >> (32 - 10)) & 0x3FF;
	uint32_t tableAngle90observer = (((angleObserver - angleWaveOtoS  + FIXED_PI / 2)) >> (32 - 10)) & 0x3FF;
	uint32_t dopplerShift =  SOUND_SPEED + FIXED_MULFRACT(cars[0].lastSpeed, sinTable[tableAngle90observer]);
	dopplerShift = (256 * dopplerShift) / ((SOUND_SPEED - FIXED_MULFRACT(cars[closestCar].lastSpeed, sinTable[tableAngle90source]))/256 ) ;
	setMixerPlaySpeed(CLOSEST_CAR_ENGINE_CHANNEL, FIXED_MUL(0x0043 + (cars[closestCar].lastSpeed >> 8) , dopplerShift));
	if (squareDistance < FULL_VOLUME_DOPPLER_SQUARE_DISTANCE)	// if the car is closer than the minimum distance, the volume is capped at 255
		squareDistance = FULL_VOLUME_DOPPLER_SQUARE_DISTANCE;
	// the volume of the other motor is 0 if the closesCar is 0 (i.e. all the other 3 are dead).
	uint16_t desiredVolume = closestCar == 0 ? 0 : ((uint32_t) 255 * FULL_VOLUME_DOPPLER_SQUARE_DISTANCE / squareDistance);
	setNoteVol(CLOSEST_CAR_ENGINE_CHANNEL, desiredVolume);
	//
	// animate everything:
	bonusHandler();
	//
	projectileHandler();
	//
	cameraX = (cars[0].currentPosition.x >> 16) - 160;
	cameraY = (cars[0].currentPosition.y >> 16) - 100;
	if (cameraX < 0)
		cameraX = 0;
	if (cameraY < 0)
		cameraY = 0;
	if (cameraX > (levels[currentLevel].pixelSizeX - SCREEN_SIZE_X))
		cameraX = (levels[currentLevel].pixelSizeX - SCREEN_SIZE_X);
	if (cameraY > (levels[currentLevel].pixelSizeY - SCREEN_SIZE_Y))
		cameraY = (levels[currentLevel].pixelSizeY - SCREEN_SIZE_Y);
	// now that we know the camera position, we can draw parallax tiles
	animatedTileTime++;
	// animate tiles.
	int offsetX = (cameraX >> 2);
	int offsetY = (cameraY >> 2);
	if (animatedTileTime == currentLevelData.animDelay || currentLevelData.parallaxEnabled)
	{
		if (currentLevelData.parallaxEnabled == 1)
		{
			// let's make a some fake parallax. 
			uint8_t *dest = (uint8_t*) &tiles[currentLevelData.animTileStart][0];
			const uint8_t *src =  &currentLevelData.pTiles[(currentLevelData.animTileStart + animatedTileFrame * (ANIM_TILE_SIZE_X * ANIM_TILE_SIZE_Y)) *sizeof(tiles[0])];
			uint32_t srcY = offsetY;
			for (int yTile = 0; yTile < ANIM_TILE_SIZE_Y; yTile++)
			{
				for (int xTile = 0; xTile < ANIM_TILE_SIZE_X; xTile++)
				{
					for (int y = 0; y < TILE_SIZE_Y; y++)
					{
						// calculate where should we pick the pixel.
						uint32_t srcX = (TILE_SIZE_X * xTile + offsetX);
						uint32_t srcTileY = (srcY & (8 * ANIM_TILE_SIZE_Y - 1)) / TILE_SIZE_Y;
						src += srcTileY * ANIM_TILE_SIZE_X * sizeof(tiles[0]) + (srcY % TILE_SIZE_Y) * TILE_SIZE_X ;
						for (int x = 0; x < TILE_SIZE_X; x++)
						{
							srcX ++;
							uint32_t srcTileX = (srcX & (TILE_SIZE_X * ANIM_TILE_SIZE_X - 1)) / TILE_SIZE_X;
							//uint8_t *src =  &currentLevelData.pTiles[(currentLevelData.animTileStart + animatedTileFrame * (ANIM_TILE_SIZE_X * ANIM_TILE_SIZE_Y)  + srcTileX  + srcTileY * ANIM_TILE_SIZE_X ) * sizeof(tiles[0])];
							*dest++ = *(src +  (srcX % TILE_SIZE_X) + srcTileX  * sizeof(tiles[0]) );
						}
						src -= srcTileY * ANIM_TILE_SIZE_X * sizeof(tiles[0]) + (srcY % TILE_SIZE_Y) * TILE_SIZE_X ;
						srcY++;
					}
					srcY -= TILE_SIZE_Y;
				}
				srcY += TILE_SIZE_Y;				
			}				
		}
		else
		{
			// tiles aligned at 4 bytes boundary. fast copy!
			uint32_t * dest = (uint32_t*) &tiles[currentLevelData.animTileStart][0] ;
			const uint32_t *src = (uint32_t*) &currentLevelData.pTiles[(currentLevelData.animTileStart + animatedTileFrame * ANIM_TILE_SIZE_X * ANIM_TILE_SIZE_Y ) * sizeof(tiles[0])];
			for (int i = 0; i < (ANIM_TILE_SIZE_X * ANIM_TILE_SIZE_Y) * sizeof(tiles[0]) / 4; i++)		// 4 bytes per time.
				*dest++ = *src++;
		}
		if (animatedTileTime >= currentLevelData.animDelay)
		{
			animatedTileTime = 0;
			animatedTileFrame++;
			if (animatedTileFrame >= currentLevelData.numberOfAnimTileFrames)
				animatedTileFrame = 0;
		}		
	}	
	// first draw the map
	drawMap(cameraX, cameraY, 0);	
	// then draw the sprites.
	removeAllSprites(0);			// remove the sprites without actually redrawing the screen (updating vram).
	// The sprites are arranged in this way: bonuses, land mines, cars, projectiles.
	uint8_t numberOfDrawnSprites = 0;
	uint8_t nextTopSprite = MAX_ONSCREEN_SPRITES - 2;		// sprite MAX_ONSCREEN_SPRITES -1 is the top text. Then there are explosions/rockets/effects. Then Cars, then mines then bonues.
	//
	numberOfDrawnSprites += drawBonuses(cameraX, cameraY, numberOfDrawnSprites);
	drawProjectiles(cameraX, cameraY, &numberOfDrawnSprites, &nextTopSprite);
	for (int i = 0; i < NUM_CARS ; i++)
	{
		int sx =  (cars[i].currentPosition.x >> 16) - cameraX;
		int sy = (cars[i].currentPosition.y >> 16) - cameraY;

		numberOfDrawnSprites += putSprite(numberOfDrawnSprites,sx ,sy , SPRITE_FLAGS_HANDLE_CENTER | GET_CAR_ROTFLAGS(cars[i].carType, (cars[i].carState & CAR_STATE_DESTROYED ), cars[i].currentSpriteAngle), GET_CAR_REAL_FRAME(cars[i].carType, (cars[i].carState & CAR_STATE_DESTROYED ), cars[i].currentSpriteAngle));
		if ((cars[i].carState == CAR_STATE_EXPLODING))
		{
			uint8_t frame = cars[i].explosionFrame;
			nextTopSprite -= putSprite(nextTopSprite,sx ,sy , SPRITE_FLAGS_HANDLE_CENTER | SPRITE_FLAGS_TRANSPARENT_SPRITE, EXPLOSION_DEFAULTANIM_FRAMEINDEX + frame);
			if ((getFrameTicks() - cars[i].bonusFrameTime) > CAR_EXPLOSION_FRAME_DELAY)
			{
				cars[i].bonusFrameTime = getFrameTicks();
				frame++;
				if ( frame == EXPLOSION_DEFAULTANIM_NUMFRAMES)
				{
					cars[i].carState = CAR_STATE_WAITING_TO_APPEAR;
					frame = WAIT_DEFAULTANIM_NUMFRAMES - 1;
					if (i == 0)
					triggerNote(PROJECTILE_CHANNEL, 11, 23, 255);
				}
				cars[i].explosionFrame = frame;
			}
		}
		else if (cars[i].carState == CAR_STATE_WAITING_TO_APPEAR || cars[i].carState == CAR_STATE_WAITING_FOR_START_RACE)
		{
			uint8_t frame = cars[i].explosionFrame;
			if (i == 0)	// only for car 0
			{
				int sx =  (cars[i].currentPosition.x >> 16) - cameraX;
				int sy = (cars[i].currentPosition.y >> 16) - cameraY;
				putSprite(MAX_ONSCREEN_SPRITES - 1,sx ,sy , SPRITE_FLAGS_HANDLE_CENTER , frame + WAIT_DEFAULTANIM_FRAMEINDEX);
			}
			if ((getFrameTicks() - cars[i].bonusFrameTime) > CAR_WAIT_FRAME_DELAY)
			{
				cars[i].bonusFrameTime = getFrameTicks();
				if ( frame == 0)
				{
					if (i == 0)	// restart motor
					{
						triggerNote(OWN_CAR_ENGINE_CHANNEL, 4, 23 , 255);	// our own car
					}
					cars[i].carState = CAR_STATE_RUNNING;
				}
				else
				{
					if (i == 0)
					triggerNote(PROJECTILE_CHANNEL, frame == 1 ? 12 : 11, 23, 255);
				}
				frame--;
				cars[i].explosionFrame = frame;
			}
		}
	}
//PROFILING DRAW SPRITES
		SysTick->CTRL = 0;
		SysTick->LOAD = 0xFFFFFF;
		SysTick->VAL = 0;
		SysTick->CTRL = 1;
	//	putSprite(10,100 ,100 , FLAG_HANDLE_CENTER | FLAG_TRANSPARENT_SPRITE, EXPLOSION_DEFAULTANIM_FRAMEINDEX + 10);
	drawSprites();
// 		cars[0].deaths = (0xFFFFFF - SysTick->VAL) >> 4;
// 		if (cars[0].deaths > cars[0].kills)
// 			cars[0].kills = cars[0].deaths;
// 		cars[0].kills = videoData.currentLineNumber;
// END PROFILING
	// finally, redraw the fixed section
	updateFixedSection(0);
	drawFixedSection();
}
void changeSelectedItem(uint16_t key, uint8_t maxItemIndex)
{
	if (key == 's' || key  == 'S')
	{
		triggerNote(PROJECTILE_CHANNEL, 18, 0, 255);
		game.selectedItem++;
		if (game.selectedItem > maxItemIndex)
			game.selectedItem = 0;		
	}
	else if (key == 'w' || key == 'W')
	{
		triggerNote(PROJECTILE_CHANNEL, 18, 0, 255);
		if (game.selectedItem > 0)
			game.selectedItem--;
		else
			game.selectedItem = maxItemIndex;
	}
}

int main(void) 
{	
	initUsvc(patches);
	game.level = 1;
	uint32_t lastTime = 0, testLed = 0;
	initFixedSection();
  	changeLevel(0, 0);
 	memset(&cars[0], 0, sizeof(cars));
    game.gameState = STATE_TITLE_SCREEN;
// TEST
//	goToRace(1,0);
//	cars[0].currentPosition.x = 100 << 16;
// ENDTEST
	uint16_t keyBuffer[6];	
	memset(keyBuffer, 0 ,sizeof (keyBuffer));	
	int16_t oldKey = 0;
	while (1)
	{
		uint32_t timeNow = millis();
		if (timeNow - lastTime > 1000UL)
		{
			setLed(testLed);
			testLed = 1 - testLed;
			lastTime = timeNow;
			setNoteVol(0, 255);

		}
		waitForVerticalBlank();
		g_frame++;
		soundEngine();

		switch (game.gameState)
		{
			case STATE_RACE_OVER:
				if (g_frame - game.stateTime > RACE_OVER_DELAY)
				{
					game.selectedItem = 0 ;
					changeLevel(0, 0);
					initMenu();
					for (int ch = 0; ch < 4; ch++)
						stopLoopingFx(ch, 0);
					checkResultsAndDrawRaceSummaryMenu(0);
					game.gameState = STATE_MENU_RACE_SUMMARY;
					uint8_t hasBestStandings = 1;
					for (int n = 1; n < NUM_CARS; n++)
					{
						if (cars[n].tournamentPoints > cars[0].tournamentPoints || (cars[n].tournamentPoints == cars[0].tournamentPoints && cars[n].cash > cars[0].cash) )
							hasBestStandings = 0;
					}
					if ((game.tournament == 0 && hasBestStandings) || (game.tournament > 0 && cars[0].finishLinePosition == 1))
						triggerNote(PROJECTILE_CHANNEL, 15, 50, 255);
					else
						triggerNote(PROJECTILE_CHANNEL, 16, 50, 255);
					break;
				}
			case STATE_PLAYING:
				gameRace(keyBuffer[0]);
				break;
			case STATE_TITLE_SCREEN:
				if(g_frame > 200)
				{
					game.gameState = STATE_TITLE_SCREEN_SHOW_PRESS_FIRE;
					memset(&tiles[MAXTILEINDEX_TITLE_SCREEN], 0, 64 * (MAX_TILES - MAXTILEINDEX_TITLE_SCREEN) );
					int8_t l = sizeof(textPressAnyKey) - 1;					
					for (int x = 20 - l / 2; x < 20 - l / 2 + l; x ++)
					{
						vram[ x + 22 * VRAMX] = (uint32_t) &tiles[MAXTILEINDEX_TITLE_SCREEN + x - 20 + l/2];
					}
				}
				break;
			case STATE_TITLE_SCREEN_SHOW_PRESS_FIRE:
				// put chars in tile
				for (int i = 0; i < sizeof(textPressAnyKey) - 1; i++)
					putCharInTile(NULL, textPressAnyKey[i], rainbow[g_frame % sizeof(rainbow)], 0, 0, (uint8_t*) &tiles[MAXTILEINDEX_TITLE_SCREEN + i], 0);
				if (keyBuffer[0] != 0)
				{
					initMenu();
					game.gameState = STATE_MENU_MAIN;
					triggerNote(PROJECTILE_CHANNEL, 17, 0, 255);
				}
				break;
			case STATE_MENU_MAIN:
				if (oldKey != keyBuffer[0])
				{
					changeSelectedItem(keyBuffer[0], 4);
					if (keyBuffer[0] == 'g' || keyBuffer[0] == 'G')
					{
						triggerNote(PROJECTILE_CHANNEL, 17, 0, 255);
						switch (game.selectedItem)
						{								
							case 0:
								game.tournament = 0;
								game.gameState = STATE_MENU_TRACK_PREVIEW;
								initPreviewMap(game.level);
								game.numberOfLaps = 10;
								break;
							case 1:
								game.tournament = 3;
								game.gameState = STATE_MENU_TRACK_PREVIEW;
								game.level = 1;
								game.numberOfLaps = 1 + rand() % 20;
								initPreviewMap(game.level);
								break;	
							case 2:
								game.gameState = STATE_MENU_CREDITS;
								break;
							case 3:
								game.gameState =  STATE_MENU_HOWTOPLAY;
								break;
							case 4:
								game.gameState = STATE_MENU_GAME_PLOT;
								break;
						}
						game.selectedItem = 0;
					}
				}
				drawGenericMenu(game.selectedItem, mainMenuCoordinates, sizeof(mainMenuCoordinates) / sizeof(mainMenuCoordinates[0]), startMenuTexts, sizeof(startMenuTexts) / sizeof(menuText_t));
				break;
			case STATE_MENU_TRACK_PREVIEW:
				if (game.tournament != 0)
					game.selectedItem = 2;		// only next available in tournament mode
				if (oldKey != keyBuffer[0])
				{
					if (!game.tournament) // preview only for tournament
						changeSelectedItem(keyBuffer[0], 2);
					if (keyBuffer[0] == 'g' || keyBuffer[0] == 'G')
					{
						triggerNote(PROJECTILE_CHANNEL, 17, 0, 255);
						switch (game.selectedItem)
						{
							case 2:		// next
								game.gameState = STATE_MENU_CAR_SELECT;
								cars[0].unlockedCars |= (1 << CARTYPE_GREENCAR); // green car always available.
								game.selectCarMenu.cash = cars[0].cash;
								if (!(cars[0].unlockedCars & (1 << game.selectCarMenu.carType)))
									game.selectCarMenu.carType = CARTYPE_GREENCAR;
								game.selectedItem = 0;
								break;
							case 1:   // change track
								game.level++;
								if (game.level >= sizeof(levelTexts) / sizeof(levelTexts_t))
								{
									game.level = 1;
								}
								initPreviewMap(game.level);
								break;
							case 0:    // change lap number
								game.numberOfLaps++;
								if (game.numberOfLaps > 20)
									game.numberOfLaps = 1;
								break;
						}						
					}
				}
				drawPreviewTrackMenu(game.selectedItem, game.level);
			break;
			case STATE_MENU_RACE_SUMMARY:
				if (oldKey != keyBuffer[0] && (keyBuffer[0] == 'g' || keyBuffer[0] == 'G'))
				{
					triggerNote(PROJECTILE_CHANNEL, 17, 0, 255);
					if (game.tournament == 0)
						game.gameState = STATE_MENU_MAIN;
					else 
					{
						game.level++;
						initPreviewMap(game.level);
						game.numberOfLaps = 1 + rand() % 20;
						game.tournament--;
						if (game.tournament == 0)
							game.gameState = STATE_MENU_MAIN;
						else
							game.gameState = STATE_MENU_TRACK_PREVIEW;
					}
				}
			break;
			case STATE_MENU_CREDITS:
				if (oldKey != keyBuffer[0] && (keyBuffer[0] == 'g' || keyBuffer[0] == 'G'))
				{
					triggerNote(PROJECTILE_CHANNEL, 17, 0, 255);
					game.gameState = STATE_MENU_MAIN;
				}
				drawGenericMenu(game.selectedItem, rectangleFullScreenCoordinates, sizeof(rectangleFullScreenCoordinates) / sizeof(rectangleFullScreenCoordinates[0]), menuGameCreditsTexts, sizeof(menuGameCreditsTexts) / sizeof(menuText_t));
				break;
			case STATE_MENU_HOWTOPLAY:
				if (oldKey != keyBuffer[0] && (keyBuffer[0] == 'g' || keyBuffer[0] == 'G'))
				{
					triggerNote(PROJECTILE_CHANNEL, 17, 0, 255);
					game.gameState = STATE_MENU_MAIN;
				}
				drawGenericMenu(game.selectedItem, rectangleFullScreenCoordinates, sizeof(rectangleFullScreenCoordinates) / sizeof(rectangleFullScreenCoordinates[0]), menuHowToPlayTexts, sizeof(menuHowToPlayTexts) / sizeof(menuText_t));
				break;	
			case STATE_MENU_GAME_PLOT:
				if (oldKey != keyBuffer[0] && (keyBuffer[0] == 'g' || keyBuffer[0] == 'G'))
				{
					triggerNote(PROJECTILE_CHANNEL, 17, 0, 255);
					game.gameState = STATE_MENU_MAIN;
				}
				drawGenericMenu(game.selectedItem, rectangleFullScreenCoordinates, sizeof(rectangleFullScreenCoordinates) / sizeof(rectangleFullScreenCoordinates[0]), menuGamePlotTexts, sizeof(menuGamePlotTexts) / sizeof(menuText_t));
				break;
			case STATE_MENU_CAR_SELECT:	
				if (oldKey != keyBuffer[0])
				{
					changeSelectedItem(keyBuffer[0], 6);
					if (keyBuffer[0] == 'g' || keyBuffer[0] == 'G')
					{
						triggerNote(PROJECTILE_CHANNEL, 17, 0, 255);
						if (game.selectedItem == 6)
						{
							if (game.selectCarMenu.cost <= game.selectCarMenu.cash)
							{
								for (int i = 0; i < 5; i++)
									cars[0].enhancements.enhancements[i] += game.selectCarMenu.buyEnhancements[i];
								cars[0].cash -= game.selectCarMenu.cost;
								cars[0].unlockedCars |= (1 << game.selectCarMenu.carType); 
								goToRace(game.level, game.selectCarMenu.carType);									
							}
						}
						else if (game.selectedItem == 0)
						{
							int16_t oldCost = game.selectCarMenu.cost;
							oldCost -= (cars[0].unlockedCars & (1 << game.selectCarMenu.carType)) ? 0 : carCosts[game.selectCarMenu.carType];
							game.selectCarMenu.carType++;
							game.selectCarMenu.carType &= 3;
							game.selectCarMenu.cost = oldCost + (cars[0].unlockedCars & (1 << game.selectCarMenu.carType)) ? 0 : carCosts[game.selectCarMenu.carType];
						}
						else if (game.selectedItem > 0)
						{  // upgrade of attributes
							uint8_t en = game.selectedItem - 1;
							uint8_t base, bought, buying;
							getEnhancement(en, &base, &bought, &buying);
							int16_t oldCost = 0;
							for (int i = 0; i < buying; i++)
								oldCost += costPerEnhacement[i];
							if (bought + buying < 5)
							{
								buying ++;
							}
							else 
							{
								buying = 0;
							}
							int16_t newCost = 0;
							for (int i = 0; i < buying; i++)
								newCost += costPerEnhacement[i];
							// calculate new cost.
							game.selectCarMenu.cost += newCost - oldCost;
							switch(en)
							{
								case 0:
									game.selectCarMenu.buyAcceleration = buying;
									break;
								case 1:
									game.selectCarMenu.buySteering = buying;
									break;
								case 2:
									game.selectCarMenu.buySpeed = buying;
									break;
								case 3:
									game.selectCarMenu.buyBrake = buying;
									break;
								case 4:
									game.selectCarMenu.buyGrip = buying;
									break;
							}
 						}
					}
				}
				drawCarSelectMenu(game.selectedItem);
				break;
		}
		oldKey = keyBuffer[0];
		// USB Handler to get keyboard or gamepad keypresses.
		do
		{
			usbHostTask();
			if (usbHidBootKeyboardIsInstalled())
			{
				usbKeyboardPoll();
				usbGetCurrentAsciiKeyboardStateEx(keyBuffer);
			
			}
			if (usbHidGenericGamepadIsInstalled())
			{
				gamePadState_t gps;
				usbHidGenericGamepadPoll();
				// gamepad to keyboard conversion
				getCurrentGamepadState(&gps);
				//
				memset(keyBuffer, 0 ,sizeof (keyBuffer));
				uint32_t buttons = gps.buttons;
				uint8_t k = 0;
 				if (gps.axes[1] == gps.XYZRxMinimum)    // on some gamepad it is difficult to press both up and left-right. We'll use buttons too
 					keyBuffer[k++] = 'w';
 				else if (gps.axes[1] == gps.XYZRxMaximum)
 					keyBuffer[k++] = 's';
				if (gps.axes[0] == gps.XYZRxMinimum)
					keyBuffer[k++] = 'a';
				else if (gps.axes[0] == gps.XYZRxMaximum)
					keyBuffer[k++] = 'd';
				// Buttons
				if (buttons & BUTTON_1)
					keyBuffer[k++] = 'w';
				if (buttons & BUTTON_2)
					keyBuffer[k++]  = 'f';
				if (buttons & BUTTON_3)
					keyBuffer[k++] = 's';
				if (buttons & (BUTTON_4 | BUTTON_R1 | BUTTON_R2))	
					keyBuffer[k++] = 'g';
// 				uint8_t used = 0;
// 				if ((buttons & (BUTTON_L1)) && (g_frame % 32 == 0) && !used)
// 				{
// 					used = 1;
// // 					cars[0].enhancements.acceleration++;
// // 					if (cars[0].enhancements.acceleration + carData[cars[0].carType].baseAcceleration > 8 )
// // 						cars[0].enhancements.acceleration = 0;
// 					cars[0].enhancements.speed++;
// 					if (cars[0].enhancements.speed + carData[cars[0].carType].baseSpeed > 8 )
// 						cars[0].enhancements.speed = 0;
// 				}
// 				if ((buttons & (BUTTON_L2)) && (g_frame % 32 == 0) && !used)
// 				{
// 					used = 1;
// 					cars[0].enhancements.grip++;
// 					if (cars[0].enhancements.grip + carData[cars[0].carType].baseGrip > 8 )
// 					 	cars[0].enhancements.grip = 0;
// 				}
// // 				cars[0].kills = 0;
// // 				for (int i = 1; i < 5; i++)
// // 					cars[0].kills = cars[0].kills * 10 + cars[0].enhancements.enhancements[i];
// // 				cars[0].deaths = cars[0].lastSpeed >> 8;				
			}
		} while (videoData.currentLineNumber < 523);
		for (int i = 0; i < 6; i++)
		{
			if (keyBuffer[i] == 'w' || keyBuffer[i] == 'W')
			{
				setCarControls(&cars[0], CAR_FLAGS_FORWARD);
			}
			else if (keyBuffer[i] == 'S' || keyBuffer[i] == 's')
			{
				setCarControls(&cars[0], CAR_FLAGS_REVERSE);
			}
			else if (keyBuffer[i] == 'A' || keyBuffer[i] == 'a')
			{
				setCarControls(&cars[0], CAR_FLAGS_LEFT);
			}
			else if (keyBuffer[i] == 'D' || keyBuffer[i] == 'd')
			{
				setCarControls(&cars[0], CAR_FLAGS_RIGHT);
			}
			else if (keyBuffer[i] == 'F' || keyBuffer[i] == 'f')
			{
				setCarControls(&cars[0], CAR_FLAGS_BRAKING);
			}
			else if (keyBuffer[i] == 'g' || keyBuffer[i] == 'G')
			{
				setCarControls(&cars[0], CAR_FLAGS_FIRE);
			}						
		}
	}	
}