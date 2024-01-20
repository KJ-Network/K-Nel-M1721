/* -----------------------------------------------------------------------
 *
 *   neon.uc - RAID-6 syndrome calculation using ARM NEON instructions
 *
 *   Copyright (C) 2012 Rob Herring
 *   Copyright (C) 2015 Linaro Ltd. <ard.biesheuvel@linaro.org>
 *
 *   Based on altivec.uc:
 *     Copyright 2002-2004 H. Peter Anvin - All Rights Reserved
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
 *   Boston MA 02111-1307, USA; either version 2 of the License, or
 *   (at your option) any later version; incorporated herein by reference.
 *
 * ----------------------------------------------------------------------- */

/*
 * neon1.c
 *
 * 1-way unrolled NEON intrinsics math RAID-6 instruction set
 *
 * This file is postprocessed using unroll.awk
 */

#include <arm_neon.h>

typedef uint8x16_t unative_t;

#define NBYTES(x) ((unative_t){x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x})
#define NSIZE	sizeof(unative_t)

/*
 * The SHLBYTE() operation shifts each byte left by 1, *not*
 * rolling over into the next byte
 */
static inline unative_t SHLBYTE(unative_t v)
{
	return vshlq_n_u8(v, 1);
}

/*
 * The MASK() operation returns 0xFF in any byte for which the high
 * bit is 1, 0x00 for any byte for which the high bit is 0.
 */
static inline unative_t MASK(unative_t v)
{
	const uint8x16_t temp = NBYTES(0);
	return (unative_t)vcltq_s8((int8x16_t)v, (int8x16_t)temp);
}

void raid6_neon1_gen_syndrome_real(int disks, unsigned long bytes, void **ptrs)
{
	uint8_t **dptr = (uint8_t **)ptrs;
	uint8_t *p, *q;
	int d, z, z0;

	register unative_t wd0, wq0, wp0, w10, w20;
	const unative_t x1d = NBYTES(0x1d);

	z0 = disks - 3;		/* Highest data disk */
	p = dptr[z0+1];		/* XOR parity */
	q = dptr[z0+2];		/* RS syndrome */

	for ( d = 0 ; d < bytes ; d += NSIZE*1 ) {
		wq0 = wp0 = vld1q_u8(&dptr[z0][d+0*NSIZE]);
		for ( z = z0-1 ; z >= 0 ; z-- ) {
			wd0 = vld1q_u8(&dptr[z][d+0*NSIZE]);
			wp0 = veorq_u8(wp0, wd0);
			w20 = MASK(wq0);
			w10 = SHLBYTE(wq0);

			w20 = vandq_u8(w20, x1d);
			w10 = veorq_u8(w10, w20);
			wq0 = veorq_u8(w10, wd0);
		}
		vst1q_u8(&p[d+NSIZE*0], wp0);
		vst1q_u8(&q[d+NSIZE*0], wq0);
	}
}

void raid6_neon1_xor_syndrome_real(int disks, int start, int stop,
				    unsigned long bytes, void **ptrs)
{
	uint8_t **dptr = (uint8_t **)ptrs;
	uint8_t *p, *q;
	int d, z, z0;

	register unative_t wd0, wq0, wp0, w10, w20;
	const unative_t x1d = NBYTES(0x1d);

	z0 = stop;		/* P/Q right side optimization */
	p = dptr[disks-2];	/* XOR parity */
	q = dptr[disks-1];	/* RS syndrome */

	for ( d = 0 ; d < bytes ; d += NSIZE*1 ) {
		wq0 = vld1q_u8(&dptr[z0][d+0*NSIZE]);
		wp0 = veorq_u8(vld1q_u8(&p[d+0*NSIZE]), wq0);

		/* P/Q data pages */
		for ( z = z0-1 ; z >= start ; z-- ) {
			wd0 = vld1q_u8(&dptr[z][d+0*NSIZE]);
			wp0 = veorq_u8(wp0, wd0);
			w20 = MASK(wq0);
			w10 = SHLBYTE(wq0);

			w20 = vandq_u8(w20, x1d);
			w10 = veorq_u8(w10, w20);
			wq0 = veorq_u8(w10, wd0);
		}
		/* P/Q left side optimization */
		for ( z = start-1 ; z >= 0 ; z-- ) {
			w20 = MASK(wq0);
			w10 = SHLBYTE(wq0);

			w20 = vandq_u8(w20, x1d);
			wq0 = veorq_u8(w10, w20);
		}
		w10 = vld1q_u8(&q[d+NSIZE*0]);
		wq0 = veorq_u8(wq0, w10);

		vst1q_u8(&p[d+NSIZE*0], wp0);
		vst1q_u8(&q[d+NSIZE*0], wq0);
	}
}
