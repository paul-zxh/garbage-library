/* This is an assembly language implementation of mulsi3, divsi3, and modsi3
   for the sparc processor.

   These routines are derived from the SPARC Architecture Manual, version 8,
   slightly edited to match the desired calling convention, and also to
   optimize them for our purposes.  */

#define L_mulsi3

#ifdef L_mulsi3
.section ".flashinit"
	.align 4
	.global .umul
	.proc 4
.umul:
	or	%o0, %o1, %o4	! logical or of multiplier and multiplicand
	mov	%o0, %y		! multiplier to Y register
	andncc	%o4, 0xfff, %o5	! mask out lower 12 bits
	be	mul_shortway	! can do it the short way
	andcc	%g0, %g0, %o4	! zero the partial product and clear NV cc
	!
	! long multiply
	!
	mulscc	%o4, %o1, %o4	! first iteration of 33
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4	! 32nd iteration
	mulscc	%o4, %g0, %o4	! last iteration only shifts
	! the upper 32 bits of product are wrong, but we do not care
	retl
	rd	%y, %o0
	!
	! short multiply
	!
mul_shortway:
	mulscc	%o4, %o1, %o4	! first iteration of 13
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4
	mulscc	%o4, %o1, %o4	! 12th iteration
	mulscc	%o4, %g0, %o4	! last iteration only shifts
	rd	%y, %o5
	sll	%o4, 12, %o4	! left shift partial product by 12 bits
	srl	%o5, 20, %o5	! right shift partial product by 20 bits
	retl
	or	%o5, %o4, %o0	! merge for true product
#endif

