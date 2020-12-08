/*
 * usvc_config.h
 *
 * Created: 04/11/2019 16:59:31
 *  Author: PETN
 */ 


#ifndef USVC_CONFIG_H_
#define USVC_CONFIG_H_
/*
	FIXED SECTION
	TILEMODE allows for a fixed section (no-scroll) at the top or the bottom of the screen.
	To activate it: 
	define USE_SECTION as TOP_FIXED_SECTION or BOTTOM_FIXED_SECTION
	eg, for a bottom fixed section:
	#define USE_SECTION TOP_FIXED_SECTION
	Also set the section limit, i.e. the row at which the section starts (for bottom) or ends (for top). The number is in terms of tiles and it is not inclusive.
*/

#define GFX_MODE TILE_MODE1
#define VRAMX 42
#define AUDIO_ENABLED 1
#define USE_MIXER 1
#define ENABLE_TILE_PRIORITY 1
#define ENABLE_SPRITE_PRIORITY 0
#ifndef USE_BOOTLOADER
	#define FORCE_INCLUDE_USB_GAMEPAD_MODULE 1 // 0: use bootloader libraries and save space 
	#define FORCE_INCLUDE_USB_MODULE 1			// 0: use bootloader libraries and save space 
	#define FORCE_INCLUDE_USB_KEYBOARD_MODULE 1	// 0: use bootloader libraries and save space 
#endif
#define TILES_ALL_OPAQUE 1
#define INCLUDE_DEFAULT_WAVES 1

#define MAX_TILES (228 + 100)  // adjust this according to the number of tiles
#define MAX_TEMP_SPRITE_TILES 100	
#define MAX_ONSCREEN_SPRITES 16
#define USE_ROW_REMAPPING 0
#define PER_LINE_X_SCROLL 0
#define PER_TILE_X_SCROLL 0
#define USE_SECTION TOP_FIXED_SECTION
#define SECTION_LIMIT 1
#define SPRITES_ENABLED 1
#define ENABLE_TRANSPARENT_SPRITES  1
// USB CONFIGURATION: MAX 2 Pipes and 1 device. Modify according to your needs. Be aware that HUB does not support low speed devices!
#define USE_USB_HUB 0
#define USB_NUMDEVICES 1
#define NUMBER_OF_USB_PIPES 3
#define MAX_USB_INTERFACES 4
// PUT HERE THE INCLUDES TO ALL YOUR DATA (for instance audio patches, sounds and sprites)
#if AUDIO_ENABLED == 1
	#include "soundWaveList.h"
	#include "patches.h"
#endif
#include "fixedSectionTileData.h"
#if SPRITES_ENABLED == 1
	#include "VGAspriteData.h"
	#if ENABLE_TRANSPARENT_SPRITES == 1
		#include "transparentSpriteColorTable.h"
	#endif
#endif
#endif /* USVC_CONFIG_H_ */