#include "rt_def.h"

#include "watcom.h"

/* 
  C versions of watcom.h assembly.
  Uses the 'long long' type.
 */

fixed FixedMul(fixed a, fixed b)
{
	long long x = a;
	long long y = b;
	long long z = x * y + 0x8000LL;
	
	return (z >> 16) & 0xffffffff;
}

fixed FixedMulShift(fixed a, fixed b, fixed shift)
{
	long long x = a;
	long long y = b;
	long long z = x * y;
	
	return (((unsigned long long)z) >> shift) & 0xffffffff;
}

fixed FixedDiv2(fixed a, fixed b)
{
	long long x = (signed long)a;
	long long y = (signed long)b;
	long long z = x * 65536LL / y;
	
	return (z) & 0xffffffff;
}

fixed FixedScale(fixed orig, fixed factor, fixed divisor)
{
	long long x = orig;
	long long y = factor;
	long long z = divisor;
	
	long long w = (x * y) / z;
	
	return (w) & 0xffffffff;
}
