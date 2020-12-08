/*
 * fastApproxMath.h
 *
 * Created: 12/09/2019 12:20:23
 *  Author: PETN
 */ 


#ifndef FASTAPPROXMATH_H_
#define FASTAPPROXMATH_H_
#include "stdint.h"
#include "atanTable.h"
#include "sqrtTable.h"
typedef struct
{
	int32_t x;
	int32_t y;
} vector_t;
#define FIXED_PI 0x80000000
#define FIXED_MUL(a, b) ( ( ((int64_t) (a)) * ((int64_t) (b)) ) >> 16)		// used to multiply a 16.16 by a 16.16
#define FIXED_MULFRACT(a, b) ( ( ((int64_t) a * (int64_t) b)) >> 15)		// used to multiply a 16.16 by a 1.15

int32_t fastApproxAtan2(int32_t x, int32_t y);
uint32_t fastApproximateSqrt(uint32_t x);

#endif /* FASTAPPROXMATH_H_ */