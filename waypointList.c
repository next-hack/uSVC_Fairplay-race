/*
 * waypointList.c
 *
 * Created: 12/09/2019 17:04:11
 *  Author: PETN
 */  
#include "main.h"
// Put the following lines in a header file!
#include <stdint.h>
// Put the following lines in the C file!
// Put the following lines in the C file! Remember to include the header too!
#include <stdint.h>
const waypoint_t wp_level1[NUMBER_OF_WAYPOINTS_LEVEL1] =
{
	{
		.x = 0x01A4,
		.y = 0x0052,
		.halfWidth = 0x0028,
		.halfHeight = 0x004A,
		.normR2 = 0x0085B100,
		.pNextWaypoint1 = &wp_level1[1],
		.pNextWaypoint2 = &wp_level1[2]
	},
	{
		.x = 0x024A,
		.y = 0x0088,
		.halfWidth = 0x0017,
		.halfHeight = 0x0023,
		.normR2 = 0x0009E359,
		.pNextWaypoint1 = &wp_level1[5],
		.pNextWaypoint2 = &wp_level1[4]
	},
	{
		.x = 0x02B3,
		.y = 0x003E,
		.halfWidth = 0x001C,
		.halfHeight = 0x0018,
		.normR2 = 0x0006E400,
		.pNextWaypoint1 = &wp_level1[3],
		.pNextWaypoint2 = &wp_level1[3]
	},
	{
		.x = 0x02D6,
		.y = 0x0072,
		.halfWidth = 0x0018,
		.halfHeight = 0x0015,
		.normR2 = 0x0003E040,
		.pNextWaypoint1 = &wp_level1[4],
		.pNextWaypoint2 = &wp_level1[4]
	},
	{
		.x = 0x02A9,
		.y = 0x00EB,
		.halfWidth = 0x0010,
		.halfHeight = 0x0010,
		.normR2 = 0x00010000,
		.pNextWaypoint1 = &wp_level1[8],
		.pNextWaypoint2 = &wp_level1[8]
	},
	{
		.x = 0x0242,
		.y = 0x00DC,
		.halfWidth = 0x0010,
		.halfHeight = 0x0010,
		.normR2 = 0x00010000,
		.pNextWaypoint1 = &wp_level1[6],
		.pNextWaypoint2 = &wp_level1[6]
	},
	{
		.x = 0x021F,
		.y = 0x0128,
		.halfWidth = 0x001F,
		.halfHeight = 0x0018,
		.normR2 = 0x00087240,
		.pNextWaypoint1 = &wp_level1[7],
		.pNextWaypoint2 = &wp_level1[8]
	},
	{
		.x = 0x0259,
		.y = 0x015E,
		.halfWidth = 0x0028,
		.halfHeight = 0x0010,
		.normR2 = 0x00064000,
		.pNextWaypoint1 = &wp_level1[9],
		.pNextWaypoint2 = &wp_level1[11]
	},
	{
		.x = 0x0279,
		.y = 0x0192,
		.halfWidth = 0x002E,
		.halfHeight = 0x0010,
		.normR2 = 0x00084400,
		.pNextWaypoint1 = &wp_level1[9],
		.pNextWaypoint2 = &wp_level1[11]
	},
	{
		.x = 0x025A,
		.y = 0x01DD,
		.halfWidth = 0x0038,
		.halfHeight = 0x0010,
		.normR2 = 0x000C4000,
		.pNextWaypoint1 = &wp_level1[10],
		.pNextWaypoint2 = &wp_level1[11]
	},
	{
		.x = 0x01B0,
		.y = 0x01ED,
		.halfWidth = 0x0027,
		.halfHeight = 0x002A,
		.normR2 = 0x0028F0A4,
		.pNextWaypoint1 = &wp_level1[13],
		.pNextWaypoint2 = &wp_level1[13]
	},
	{
		.x = 0x0257,
		.y = 0x021D,
		.halfWidth = 0x0010,
		.halfHeight = 0x0010,
		.normR2 = 0x00010000,
		.pNextWaypoint1 = &wp_level1[12],
		.pNextWaypoint2 = &wp_level1[12]
	},
	{
		.x = 0x01D8,
		.y = 0x022D,
		.halfWidth = 0x0010,
		.halfHeight = 0x0010,
		.normR2 = 0x00010000,
		.pNextWaypoint1 = &wp_level1[10],
		.pNextWaypoint2 = &wp_level1[13]
	},
	{
		.x = 0x0178,
		.y = 0x0134,
		.halfWidth = 0x001F,
		.halfHeight = 0x0011,
		.normR2 = 0x00043CE1,
		.pNextWaypoint1 = &wp_level1[14],
		.pNextWaypoint2 = &wp_level1[14]
	},
	{
		.x = 0x00B2,
		.y = 0x014F,
		.halfWidth = 0x002B,
		.halfHeight = 0x002E,
		.normR2 = 0x003BB324,
		.pNextWaypoint1 = &wp_level1[15],
		.pNextWaypoint2 = &wp_level1[15]
	},
	{
		.x = 0x00A7,
		.y = 0x0205,
		.halfWidth = 0x0022,
		.halfHeight = 0x0010,
		.normR2 = 0x00048400,
		.pNextWaypoint1 = &wp_level1[16],
		.pNextWaypoint2 = &wp_level1[17]
	},
	{
		.x = 0x013C,
		.y = 0x0298,
		.halfWidth = 0x0028,
		.halfHeight = 0x0024,
		.normR2 = 0x001FA400,
		.pNextWaypoint1 = &wp_level1[18],
		.pNextWaypoint2 = &wp_level1[18]
	},
	{
		.x = 0x00A9,
		.y = 0x027E,
		.halfWidth = 0x0027,
		.halfHeight = 0x0031,
		.normR2 = 0x0037B951,
		.pNextWaypoint1 = &wp_level1[16],
		.pNextWaypoint2 = &wp_level1[18]
	},
	{
		.x = 0x02CD,
		.y = 0x02B2,
		.halfWidth = 0x0016,
		.halfHeight = 0x0035,
		.normR2 = 0x0014BEC4,
		.pNextWaypoint1 = &wp_level1[19],
		.pNextWaypoint2 = &wp_level1[19]
	},
	{
		.x = 0x02BA,
		.y = 0x0304,
		.halfWidth = 0x001E,
		.halfHeight = 0x001E,
		.normR2 = 0x000C5C10,
		.pNextWaypoint1 = &wp_level1[20],
		.pNextWaypoint2 = &wp_level1[20]
	},
	{
		.x = 0x0047,
		.y = 0x02E6,
		.halfWidth = 0x0028,
		.halfHeight = 0x001A,
		.normR2 = 0x00108100,
		.pNextWaypoint1 = &wp_level1[21],
		.pNextWaypoint2 = &wp_level1[21]
	},
	{
		.x = 0x0050,
		.y = 0x0070,
		.halfWidth = 0x003C,
		.halfHeight = 0x0031,
		.normR2 = 0x0083E410,
		.pNextWaypoint1 = &wp_level1[0],
		.pNextWaypoint2 = &wp_level1[0]
	},
	{
		.x = 0x01AF,
		.y = 0x0062,
		.halfWidth = 0x0010,
		.halfHeight = 0x0067,
		.normR2 = 0x00297100,
		.pNextWaypoint1 = &wp_level1[23],
		.pNextWaypoint2 = &wp_level1[23]
	},
	{
		.x = 0x014B,
		.y = 0x0297,
		.halfWidth = 0x0011,
		.halfHeight = 0x0034,
		.normR2 = 0x000BEC90,
		.pNextWaypoint1 = &wp_level1[24],
		.pNextWaypoint2 = &wp_level1[24]
	},
	{
		.x = 0x003D,
		.y = 0x02A5,
		.halfWidth = 0x003E,
		.halfHeight = 0x0019,
		.normR2 = 0x0024A8C4,
		.pNextWaypoint1 = &wp_level1[22],
		.pNextWaypoint2 = &wp_level1[22]
	}
};

const waypoint_t wp_level2[NUMBER_OF_WAYPOINTS_LEVEL2] =
{
	{
		.x = 0x02DE,
		.y = 0x025F,
		.halfWidth = 0x0019,
		.halfHeight = 0x003B,
		.normR2 = 0x00213289,
		.pNextWaypoint1 = &wp_level2[1],
		.pNextWaypoint2 = &wp_level2[1]
	},
	{
		.x = 0x02EB,
		.y = 0x02DA,
		.halfWidth = 0x0020,
		.halfHeight = 0x0027,
		.normR2 = 0x0017C400,
		.pNextWaypoint1 = &wp_level2[2],
		.pNextWaypoint2 = &wp_level2[2]
	},
	{
		.x = 0x020F,
		.y = 0x02C5,
		.halfWidth = 0x001B,
		.halfHeight = 0x0018,
		.normR2 = 0x00066840,
		.pNextWaypoint1 = &wp_level2[3],
		.pNextWaypoint2 = &wp_level2[3]
	},
	{
		.x = 0x01CD,
		.y = 0x02D8,
		.halfWidth = 0x001B,
		.halfHeight = 0x0022,
		.normR2 = 0x000CDBE4,
		.pNextWaypoint1 = &wp_level2[4],
		.pNextWaypoint2 = &wp_level2[4]
	},
	{
		.x = 0x0186,
		.y = 0x02FA,
		.halfWidth = 0x001A,
		.halfHeight = 0x001D,
		.normR2 = 0x0008ACC4,
		.pNextWaypoint1 = &wp_level2[5],
		.pNextWaypoint2 = &wp_level2[5]
	},
	{
		.x = 0x0101,
		.y = 0x02D6,
		.halfWidth = 0x001A,
		.halfHeight = 0x0027,
		.normR2 = 0x000FB064,
		.pNextWaypoint1 = &wp_level2[6],
		.pNextWaypoint2 = &wp_level2[7]
	},
	{
		.x = 0x0093,
		.y = 0x02DE,
		.halfWidth = 0x0016,
		.halfHeight = 0x0013,
		.normR2 = 0x0002AA84,
		.pNextWaypoint1 = &wp_level2[7],
		.pNextWaypoint2 = &wp_level2[8]
	},
	{
		.x = 0x0041,
		.y = 0x02C8,
		.halfWidth = 0x001A,
		.halfHeight = 0x0021,
		.normR2 = 0x000B3BA4,
		.pNextWaypoint1 = &wp_level2[8],
		.pNextWaypoint2 = &wp_level2[9]
	},
	{
		.x = 0x0080,
		.y = 0x025C,
		.halfWidth = 0x0010,
		.halfHeight = 0x0010,
		.normR2 = 0x00010000,
		.pNextWaypoint1 = &wp_level2[11],
		.pNextWaypoint2 = &wp_level2[11]
	},
	{
		.x = 0x004B,
		.y = 0x020E,
		.halfWidth = 0x0010,
		.halfHeight = 0x0010,
		.normR2 = 0x00010000,
		.pNextWaypoint1 = &wp_level2[10],
		.pNextWaypoint2 = &wp_level2[11]
	},
	{
		.x = 0x0024,
		.y = 0x01A5,
		.halfWidth = 0x0017,
		.halfHeight = 0x001F,
		.normR2 = 0x0007C1D1,
		.pNextWaypoint1 = &wp_level2[12],
		.pNextWaypoint2 = &wp_level2[13]
	},
	{
		.x = 0x0082,
		.y = 0x0158,
		.halfWidth = 0x0010,
		.halfHeight = 0x0010,
		.normR2 = 0x00010000,
		.pNextWaypoint1 = &wp_level2[12],
		.pNextWaypoint2 = &wp_level2[13]
	},
	{
		.x = 0x005A,
		.y = 0x00FA,
		.halfWidth = 0x001E,
		.halfHeight = 0x0023,
		.normR2 = 0x0010D2A4,
		.pNextWaypoint1 = &wp_level2[13],
		.pNextWaypoint2 = &wp_level2[13]
	},
	{
		.x = 0x0088,
		.y = 0x00D5,
		.halfWidth = 0x001E,
		.halfHeight = 0x001C,
		.normR2 = 0x000AC440,
		.pNextWaypoint1 = &wp_level2[14],
		.pNextWaypoint2 = &wp_level2[14]
	},
	{
		.x = 0x010F,
		.y = 0x00E6,
		.halfWidth = 0x001F,
		.halfHeight = 0x0021,
		.normR2 = 0x000FF801,
		.pNextWaypoint1 = &wp_level2[15],
		.pNextWaypoint2 = &wp_level2[15]
	},
	{
		.x = 0x0120,
		.y = 0x01BA,
		.halfWidth = 0x0028,
		.halfHeight = 0x0010,
		.normR2 = 0x00064000,
		.pNextWaypoint1 = &wp_level2[16],
		.pNextWaypoint2 = &wp_level2[16]
	},
	{
		.x = 0x0199,
		.y = 0x01A5,
		.halfWidth = 0x001A,
		.halfHeight = 0x003D,
		.normR2 = 0x002661C4,
		.pNextWaypoint1 = &wp_level2[17],
		.pNextWaypoint2 = &wp_level2[18]
	},
	{
		.x = 0x01F5,
		.y = 0x01BC,
		.halfWidth = 0x0014,
		.halfHeight = 0x0032,
		.normR2 = 0x000F4240,
		.pNextWaypoint1 = &wp_level2[18],
		.pNextWaypoint2 = &wp_level2[19]
	},
	{
		.x = 0x02D1,
		.y = 0x01AB,
		.halfWidth = 0x0029,
		.halfHeight = 0x0029,
		.normR2 = 0x002B1E21,
		.pNextWaypoint1 = &wp_level2[20],
		.pNextWaypoint2 = &wp_level2[20]
	},
	{
		.x = 0x02DB,
		.y = 0x01DB,
		.halfWidth = 0x0026,
		.halfHeight = 0x0034,
		.normR2 = 0x003B9440,
		.pNextWaypoint1 = &wp_level2[20],
		.pNextWaypoint2 = &wp_level2[20]
	},
	{
		.x = 0x02E8,
		.y = 0x00E2,
		.halfWidth = 0x0028,
		.halfHeight = 0x0022,
		.normR2 = 0x001C3900,
		.pNextWaypoint1 = &wp_level2[21],
		.pNextWaypoint2 = &wp_level2[21]
	},
	{
		.x = 0x02E5,
		.y = 0x002D,
		.halfWidth = 0x0025,
		.halfHeight = 0x0023,
		.normR2 = 0x001996E1,
		.pNextWaypoint1 = &wp_level2[22],
		.pNextWaypoint2 = &wp_level2[22]
	},
	{
		.x = 0x0282,
		.y = 0x002B,
		.halfWidth = 0x0020,
		.halfHeight = 0x0024,
		.normR2 = 0x00144000,
		.pNextWaypoint1 = &wp_level2[24],
		.pNextWaypoint2 = &wp_level2[23]
	},
	{
		.x = 0x0286,
		.y = 0x0105,
		.halfWidth = 0x0016,
		.halfHeight = 0x001F,
		.normR2 = 0x000718E4,
		.pNextWaypoint1 = &wp_level2[25],
		.pNextWaypoint2 = &wp_level2[25]
	},
	{
		.x = 0x022A,
		.y = 0x00DD,
		.halfWidth = 0x001C,
		.halfHeight = 0x0020,
		.normR2 = 0x000C4000,
		.pNextWaypoint1 = &wp_level2[26],
		.pNextWaypoint2 = &wp_level2[27]
	},
	{
		.x = 0x025D,
		.y = 0x012F,
		.halfWidth = 0x0017,
		.halfHeight = 0x001B,
		.normR2 = 0x0005E269,
		.pNextWaypoint1 = &wp_level2[26],
		.pNextWaypoint2 = &wp_level2[26]
	},
	{
		.x = 0x01C6,
		.y = 0x00DD,
		.halfWidth = 0x001F,
		.halfHeight = 0x0020,
		.normR2 = 0x000F0400,
		.pNextWaypoint1 = &wp_level2[27],
		.pNextWaypoint2 = &wp_level2[28]
	},
	{
		.x = 0x017E,
		.y = 0x0134,
		.halfWidth = 0x0023,
		.halfHeight = 0x0024,
		.normR2 = 0x00183990,
		.pNextWaypoint1 = &wp_level2[28],
		.pNextWaypoint2 = &wp_level2[28]
	},
	{
		.x = 0x018D,
		.y = 0x025A,
		.halfWidth = 0x0038,
		.halfHeight = 0x002F,
		.normR2 = 0x0069B440,
		.pNextWaypoint1 = &wp_level2[29],
		.pNextWaypoint2 = &wp_level2[29]
	},
	{
		.x = 0x0279,
		.y = 0x025D,
		.halfWidth = 0x001C,
		.halfHeight = 0x0042,
		.normR2 = 0x00341C40,
		.pNextWaypoint1 = &wp_level2[0],
		.pNextWaypoint2 = &wp_level2[0]
	},
	{
		.x = 0x02D3,
		.y = 0x025E,
		.halfWidth = 0x001C,
		.halfHeight = 0x0042,
		.normR2 = 0x00341C40,
		.pNextWaypoint1 = &wp_level2[31],
		.pNextWaypoint2 = &wp_level2[31]
	},
	{
		.x = 0x0052,
		.y = 0x018A,
		.halfWidth = 0x0064,
		.halfHeight = 0x000D,
		.normR2 = 0x0019C990,
		.pNextWaypoint1 = &wp_level2[32],
		.pNextWaypoint2 = &wp_level2[32]
	},
	{
		.x = 0x029A,
		.y = 0x003E,
		.halfWidth = 0x001C,
		.halfHeight = 0x0042,
		.normR2 = 0x00341C40,
		.pNextWaypoint1 = &wp_level2[30],
		.pNextWaypoint2 = &wp_level2[30]
	}
};

const waypoint_t wp_level3[NUMBER_OF_WAYPOINTS_LEVEL3] =
{
	{
		.x = 0x021E,
		.y = 0x00A5,
		.halfWidth = 0x002C,
		.halfHeight = 0x0030,
		.normR2 = 0x00441000,
		.pNextWaypoint1 = &wp_level3[1],
		.pNextWaypoint2 = &wp_level3[1]
	},
	{
		.x = 0x0295,
		.y = 0x007C,
		.halfWidth = 0x0015,
		.halfHeight = 0x0027,
		.normR2 = 0x000A3C29,
		.pNextWaypoint1 = &wp_level3[2],
		.pNextWaypoint2 = &wp_level3[2]
	},
	{
		.x = 0x02B5,
		.y = 0x00AD,
		.halfWidth = 0x001F,
		.halfHeight = 0x0010,
		.normR2 = 0x0003C100,
		.pNextWaypoint1 = &wp_level3[3],
		.pNextWaypoint2 = &wp_level3[3]
	},
	{
		.x = 0x026C,
		.y = 0x00F6,
		.halfWidth = 0x0014,
		.halfHeight = 0x0012,
		.normR2 = 0x0001FA40,
		.pNextWaypoint1 = &wp_level3[4],
		.pNextWaypoint2 = &wp_level3[4]
	},
	{
		.x = 0x01D9,
		.y = 0x014E,
		.halfWidth = 0x0021,
		.halfHeight = 0x0019,
		.normR2 = 0x000A62B1,
		.pNextWaypoint1 = &wp_level3[5],
		.pNextWaypoint2 = &wp_level3[5]
	},
	{
		.x = 0x01BD,
		.y = 0x01B8,
		.halfWidth = 0x002A,
		.halfHeight = 0x0019,
		.normR2 = 0x0010D2A4,
		.pNextWaypoint1 = &wp_level3[6],
		.pNextWaypoint2 = &wp_level3[6]
	},
	{
		.x = 0x020D,
		.y = 0x01DF,
		.halfWidth = 0x001A,
		.halfHeight = 0x002C,
		.normR2 = 0x0013F840,
		.pNextWaypoint1 = &wp_level3[7],
		.pNextWaypoint2 = &wp_level3[7]
	},
	{
		.x = 0x027B,
		.y = 0x016D,
		.halfWidth = 0x0020,
		.halfHeight = 0x0019,
		.normR2 = 0x0009C400,
		.pNextWaypoint1 = &wp_level3[8],
		.pNextWaypoint2 = &wp_level3[8]
	},
	{
		.x = 0x02AB,
		.y = 0x01AE,
		.halfWidth = 0x0022,
		.halfHeight = 0x0019,
		.normR2 = 0x000B0644,
		.pNextWaypoint1 = &wp_level3[9],
		.pNextWaypoint2 = &wp_level3[9]
	},
	{
		.x = 0x029E,
		.y = 0x0277,
		.halfWidth = 0x002B,
		.halfHeight = 0x0019,
		.normR2 = 0x0011A229,
		.pNextWaypoint1 = &wp_level3[10],
		.pNextWaypoint2 = &wp_level3[10]
	},
	{
		.x = 0x024A,
		.y = 0x0293,
		.halfWidth = 0x0029,
		.halfHeight = 0x0019,
		.normR2 = 0x00100801,
		.pNextWaypoint1 = &wp_level3[11],
		.pNextWaypoint2 = &wp_level3[11]
	},
	{
		.x = 0x01E3,
		.y = 0x0256,
		.halfWidth = 0x001B,
		.halfHeight = 0x0027,
		.normR2 = 0x0010EB49,
		.pNextWaypoint1 = &wp_level3[12],
		.pNextWaypoint2 = &wp_level3[12]
	},
	{
		.x = 0x01B4,
		.y = 0x02E1,
		.halfWidth = 0x001E,
		.halfHeight = 0x0019,
		.normR2 = 0x00089544,
		.pNextWaypoint1 = &wp_level3[13],
		.pNextWaypoint2 = &wp_level3[13]
	},
	{
		.x = 0x0150,
		.y = 0x02D3,
		.halfWidth = 0x0021,
		.halfHeight = 0x0019,
		.normR2 = 0x000A62B1,
		.pNextWaypoint1 = &wp_level3[14],
		.pNextWaypoint2 = &wp_level3[14]
	},
	{
		.x = 0x0166,
		.y = 0x0255,
		.halfWidth = 0x0021,
		.halfHeight = 0x0019,
		.normR2 = 0x000A62B1,
		.pNextWaypoint1 = &wp_level3[15],
		.pNextWaypoint2 = &wp_level3[15]
	},
	{
		.x = 0x00C7,
		.y = 0x0245,
		.halfWidth = 0x002A,
		.halfHeight = 0x0019,
		.normR2 = 0x0010D2A4,
		.pNextWaypoint1 = &wp_level3[16],
		.pNextWaypoint2 = &wp_level3[17]
	},
	{
		.x = 0x00D2,
		.y = 0x00AB,
		.halfWidth = 0x0029,
		.halfHeight = 0x0019,
		.normR2 = 0x00100801,
		.pNextWaypoint1 = &wp_level3[0],
		.pNextWaypoint2 = &wp_level3[0]
	},
	{
		.x = 0x00E8,
		.y = 0x00BE,
		.halfWidth = 0x0026,
		.halfHeight = 0x0019,
		.normR2 = 0x000DC564,
		.pNextWaypoint1 = &wp_level3[0],
		.pNextWaypoint2 = &wp_level3[0]
	},
	{
		.x = 0x01D9,
		.y = 0x00A3,
		.halfWidth = 0x0009,
		.halfHeight = 0x004D,
		.normR2 = 0x000753F9,
		.pNextWaypoint1 = &wp_level3[19],
		.pNextWaypoint2 = &wp_level3[19]
	},
	{
		.x = 0x01C6,
		.y = 0x01A1,
		.halfWidth = 0x003B,
		.halfHeight = 0x0018,
		.normR2 = 0x001E9840,
		.pNextWaypoint1 = &wp_level3[20],
		.pNextWaypoint2 = &wp_level3[20]
	},
	{
		.x = 0x0160,
		.y = 0x0269,
		.halfWidth = 0x003B,
		.halfHeight = 0x0018,
		.normR2 = 0x001E9840,
		.pNextWaypoint1 = &wp_level3[18],
		.pNextWaypoint2 = &wp_level3[18]
	}
};