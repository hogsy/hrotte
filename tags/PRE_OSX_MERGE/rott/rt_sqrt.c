#include "rt_def.h"

#include "rt_sqrt.h"

/* 
  C version of fixed-point Square Root functions 
 */

long FixedSqrtLP(long n)  // Low  Precision (8.8)
{
	/* Not used, so unimplemented */
	STUB_FUNCTION;
	
	return 0;
}

long FixedSqrtHP(long n)  // High Precision (8.16)
{
	long retval;

/*	printf ("FixedSqrtHP: sqrt of %d.%d is ",(n&0x00ff0000)>>16, (n&0xffff)); */

#if (USE_C_SQRT == 1)
	/* This is more or less a direct C transliteration of the asm code.
	   I've replaced right shifting with division, since right shifting
	   signed values is undefined in ANSI C (though it usually works).
	   ROTT does not use this routine heavily. */
	long root, mask, val, d;

	root = 0;
	mask = 0x4000000;
	val = (long)n;
 hp1:
	d = val;
	d -= mask;
	if (d < 0)
	    goto hp2;
	d -= root;
	if (d < 0)
	    goto hp2;
	val = d;
	root /= 2;
	root |= mask;
	mask /= 4;
	if (mask != 0)
	    goto hp1;
	else
	    goto hp5;
 hp2:
	root /= 2;
	mask /= 4;
	if (mask != 0)
	    goto hp1;
 hp5:
	mask = 0x00004000;
	root <<= 16;
	val <<= 16;
 hp3:
	d = val;
	d -= mask;
	if (d < 0)
	    goto hp4;
	val = d;
	root /= 2;
	root |= mask;
	mask /= 4;
	if (mask != 0)
	    goto hp3;
	else
	    goto hp6;
 hp4:
	root /= 2;
	mask /= 4;
	if (mask != 0)
	    goto hp3;
 hp6:
	retval = (long)root;

#else  /* !USE_C_SQRT */

	__asm__ __volatile__ (
		"xorl %%eax, %%eax		\n\t"
		"movl $0x40000000, %%ebx	\n\t"
		"sqrtHP1: movl %%ecx, %%edx	\n\t"
		"subl %%ebx, %%edx		\n\t"
		"jb sqrtHP2			\n\t"
		"subl %%eax, %%edx		\n\t"
		"jb sqrtHP2			\n\t"
		"movl %%edx, %%ecx		\n\t"
		"shrl $1, %%eax			\n\t"
		"orl %%ebx, %%eax		\n\t"
		"shrl $2, %%ebx			\n\t"
		"jnz sqrtHP1			\n\t"
		"jz sqrtHP5			\n\t"
		"sqrtHP2: shrl $1, %%eax	\n\t"
		"shrl $2, %%ebx			\n\t"
		"jnz sqrtHP1			\n\t"
		"sqrtHP5:			\n\t"
		"movl $0x00004000, %%ebx	\n\t"
		"shll $16, %%eax		\n\t"
		"shll $16, %%ecx		\n\t"
		"sqrtHP3:			\n\t"
		"movl %%ecx, %%edx		\n\t"
		"subl %%ebx, %%edx		\n\t"
		"jb sqrtHP4			\n\t"
		"subl %%eax, %%edx		\n\t"
		"jb sqrtHP4			\n\t"
		"movl %%edx, %%ecx		\n\t"
		"shrl $1, %%eax			\n\t"
		"orl %%ebx, %%eax		\n\t"
		"shrl $2, %%ebx			\n\t"
		"jnz sqrtHP3			\n\t"
		"jz sqrtHP6			\n\t"
		"sqrtHP4:			\n\t"
		"shrl $1, %%eax			\n\t"
		"shrl $2, %%ebx			\n\t"
		"jnz sqrtHP3			\n\t"
		"sqrtHP6:			\n\t"
		"nop				\n\t"
	: "=a" (retval) : "c" (n) : "ebx", "edx", "cc");

#endif  /* !USE_C_SQRT */

/*	printf (" %d.%d\n",(retval&0x00ff0000)>>16, (retval&0xffff)); */

	return retval;
}
