#pragma once

#include "core_types.h"

#include <string.h>
#include <immintrin.h>

/**
 * 
 */
typedef union Mat2U
{
	struct
	{
		/// Scalar elements
		uint32 a, b, c, d;
	};

	/// Array memory
	uint32 array[4];

	/// Matrix memory
	uint32 matrix[2][2];

	/// Intrinsics memory
	__m128i vector;
} umat2;

FORCE_INLINE void umat2_add(umat2 * a, const umat2 * b)
{
	a->vector = _mm_add_epi32(a->vector, b->vector);
}

FORCE_INLINE void umat2_mullo(umat2 * a, const umat2 * b)
{
	a->vector = _mm_mullo_epi16(a->vector, b->vector);
}

FORCE_INLINE void umat2_mulhi(umat2 * a, const umat2 * b)
{
	a->vector = _mm_mulhi_epi16(a->vector, b->vector);
}

FORCE_INLINE void umat2_mul(umat2 * a, const umat2 * b)
{
	umat2_mullo(a, b);
}

FORCE_INLINE void umat2_dot(umat2 * a, const umat2 * b)
{
	__m128i y = _mm_shuffle_epi32(b->vector, _MM_SHUFFLE(3, 1, 2, 0));
	__m128i x1 = _mm_shuffle_epi32(a->vector, _MM_SHUFFLE(1, 0, 1, 0));
	__m128i x2 = _mm_shuffle_epi32(a->vector, _MM_SHUFFLE(3, 2, 3, 2));

	x1 = _mm_mullo_epi16(x1, y);
	x2 = _mm_mullo_epi16(x2, y);

	a->vector = _mm_hadd_epi32(x1, x2);
}

FORCE_INLINE void umat2_dot_rev(const umat2 * a, umat2 * b)
{
	__m128i y = _mm_shuffle_epi32(b->vector, _MM_SHUFFLE(3, 1, 2, 0));
	__m128i x1 = _mm_shuffle_epi32(a->vector, _MM_SHUFFLE(1, 0, 1, 0));
	__m128i x2 = _mm_shuffle_epi32(a->vector, _MM_SHUFFLE(3, 2, 3, 2));

	x1 = _mm_mullo_epi16(x1, y);
	x2 = _mm_mullo_epi16(x2, y);

	b->vector = _mm_hadd_epi32(x1, x2);
}

FORCE_INLINE void umat2_dot_bac(const umat2 * b, umat2 * a, const umat2 * c)
{
	__m128i b0 = _mm_shuffle_epi32(b->vector, _MM_SHUFFLE(1, 0, 1, 0));
	__m128i b1 = _mm_shuffle_epi32(b->vector, _MM_SHUFFLE(3, 2, 3, 2));
	__m128i a0 = _mm_shuffle_epi32(a->vector, _MM_SHUFFLE(3, 1, 2, 0));
	__m128i c0 = _mm_shuffle_epi32(c->vector, _MM_SHUFFLE(2, 2, 0, 0));
	__m128i c1 = _mm_shuffle_epi32(c->vector, _MM_SHUFFLE(3, 3, 1, 1));

	b0 = _mm_mullo_epi16(b0, a0);
	b1 = _mm_mullo_epi16(b1, a0);

	// ! Modulo
	a0 = _mm_mullo_epi16(b0, c0);
	a->vector = _mm_mullo_epi16(b0, c1);
	b0 = _mm_mullo_epi16(b1, c1);
	b1 = _mm_mullo_epi16(b1, c0);

	a->vector = _mm_hadd_epi32(_mm_hadd_epi32(a0, a->vector), _mm_hadd_epi32(b1, b0));
}

FORCE_INLINE void umat2_dot_bac_z(const umat2 * b, umat2 * a, const umat2 * c, const umat2 * z)
{
	umat2_dot_rev(b, a);
	umat2_mod(a, z);
	umat2_dot(a, c);
	umat2_mod(a, z);
}

FORCE_INLINE void umat2_mod(umat2 * a, const umat2 * b)
{
	// ! Very inefficient
	a->vector = _mm_sub_epi32(a->vector, _mm_mullo_epi32(_mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(a->vector), _mm_cvtepi32_ps(b->vector))), b->vector));
}

FORCE_INLINE int umat2_cmpeq(const umat2 * a, const umat2 * b)
{
	return _mm_movemask_epi8(_mm_cmpeq_epi32(a->vector, b->vector));
}

FORCE_INLINE void umat2_print(const umat2 * m)
{
	printf("| %4u, %4u |\n", m->a, m->b);
	printf("| %4u, %4u |\n", m->c, m->d);
}