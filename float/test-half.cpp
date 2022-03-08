#include	<stdio.h>
#include	<math.h>
#include	"TypeDefs.h"

void halfToFloat ( const uint16_t * half, uint32_t * f, int count );
void floatToHalf ( const uint32_t * ft, uint16_t * half, int count );

float	buf [] =
{
	0.0f,
	1.0f,
	0.25f,
	3.1415926f,
	-0.034f,
	-173.236f,
	-5.0f,
	1e-50,
	-267.0f,
	-2e70
};

#define	NUM	(sizeof(buf)/sizeof(buf[0]))

int main ( int argc, char * argv [] )
{
	uint16_t h [NUM];
	float	 f [NUM];

	floatToHalf ( (uint32_t *) buf, h, NUM );
	halfToFloat ( h, (uint32_t *) f, NUM );

	printf ( "%d\n", NUM );
//	for ( int i = 0; i < NUM; i++ )
//		printf ( "%f\n", buf [i] );
	
	for ( int i = 0; i < NUM; i++ )
		printf ( "%12f, %12f, delta = %12f\n", buf [i], f [i], fabs ( buf [i] - f [i] ) );

	return 0;
}
