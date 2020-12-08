/*
 * atanTable.h
 *
 * Created: 12/09/2019 14:51:15
 *  Author: PETN
 */ 


#ifndef ATANTABLE_H_
#define ATANTABLE_H_
#define NUMBER_OF_ATAN_ENTRIES 256
#define ATAN_PI (0x8000UL * 4) // 0x8000 is pi/4
#include <stdint.h>
extern const int16_t approxAtanTable[NUMBER_OF_ATAN_ENTRIES];

#endif /* ATANTABLE_H_ */