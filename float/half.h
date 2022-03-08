//
//
//

#pragma once
#ifndef	__HALF_CONVERT__
#define	__HALF_CONVERT__

#include	"TypeDefs.h"

void halfToFloat ( const uint16_t * half, uint32_t * f, int count );
void floatToHalf ( const uint32_t * ft, uint16_t * half, int count );

#endif