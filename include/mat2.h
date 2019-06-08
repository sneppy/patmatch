#pragma once

#include "core_types.h"

#include <string.h>
#include <immintrin.h>

/**
 * A 2x2 matrix, used as kernel
 * for karp-rabin fingerprint
 * function
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

/**
 * Perform basic element-wise
 * arithemtic operations
 * 
 * @param [out] a destination operand
 * @param [in] b source operand
 * @{
 */
static FORCE_INLINE void umat2_add(umat2 * a, const umat2 * b)
{
	a->vector = _mm_add_epi32(a->vector, b->vector);
}

static FORCE_INLINE void umat2_mul(umat2 * a, const umat2 * b)
{
	a->vector = _mm_mullo_epi32(a->vector, b->vector);
}
/// @}

/**
 * Perform modulo operation
 * 
 * @param [out] a destination operand
 * @param [in] z source operand
 */
static FORCE_INLINE void umat2_mod(umat2 * a, const umat2 * z)
{
	// ! Very inefficient
	a->vector = _mm_sub_epi32(a->vector, _mm_mullo_epi32(_mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(a->vector), _mm_cvtepi32_ps(z->vector))), z->vector));
}

/**
 * Perform matrix dot product
 * 
 * @param [out] a destination operand
 * @param [in] b source operand
 */
static FORCE_INLINE void umat2_dot(umat2 * a, const umat2 * b)
{
	__m128i y = _mm_shuffle_epi32(b->vector, _MM_SHUFFLE(3, 1, 2, 0));

	__m128i x1 = _mm_shuffle_epi32(a->vector, _MM_SHUFFLE(1, 0, 1, 0));
	x1 = _mm_mullo_epi32(x1, y);

	__m128i x2 = _mm_shuffle_epi32(a->vector, _MM_SHUFFLE(3, 2, 3, 2));
	x2 = _mm_mullo_epi32(x2, y);


	a->vector = _mm_hadd_epi32(x1, x2);
}
/// @}

/**
 * Like @ref umat2_dot but invert
 * destination operand
 * 
 * @param [in] a source operand
 * @param [out] b destination operand
 */
static FORCE_INLINE void umat2_dot_rev(const umat2 * a, umat2 * b)
{
	__m128i y = _mm_shuffle_epi32(b->vector, _MM_SHUFFLE(3, 1, 2, 0));

	__m128i x1 = _mm_shuffle_epi32(a->vector, _MM_SHUFFLE(1, 0, 1, 0));
	x1 = _mm_mullo_epi32(x1, y);

	__m128i x2 = _mm_shuffle_epi32(a->vector, _MM_SHUFFLE(3, 2, 3, 2));
	x2 = _mm_mullo_epi32(x2, y);

	b->vector = _mm_hadd_epi32(x1, x2);
}
/// @}

/**
 * Multiply inner matrix by b0 and b1
 * For example B-1 x A x B = A
 * 
 * @param [in] b0,b1 outer matrices
 * @param [out] a destination operand
 */
static FORCE_INLINE void umat2_dot_bab(const umat2 * b0, umat2 * a, const umat2 * b1)
{
	__m128i x = _mm_shuffle_epi32(a->vector, _MM_SHUFFLE(3, 1, 2, 0));

	__m128i y0 = _mm_shuffle_epi32(b0->vector, _MM_SHUFFLE(1, 0, 1, 0));
	y0 = _mm_mullo_epi32(y0, x);

	__m128i y1 = _mm_shuffle_epi32(b0->vector, _MM_SHUFFLE(3, 2, 3, 2));
	y1 = _mm_mullo_epi32(y1, x);

	__m128i z0 = _mm_shuffle_epi32(b1->vector, _MM_SHUFFLE(2, 2, 0, 0));
	z0 = _mm_hadd_epi32(_mm_mullo_epi32(y0, z0), _mm_mullo_epi32(y1, z0));

	__m128i z1 = _mm_shuffle_epi32(b1->vector, _MM_SHUFFLE(3, 3, 1, 1));
	z1 = _mm_hadd_epi32(_mm_mullo_epi32(y0, z1), _mm_mullo_epi32(y1, z1));

	a->vector = _mm_hadd_epi32(z0, z1);
}
/// @}

static FORCE_INLINE void umat2_dot_bab_z(const umat2 * b0, umat2 * a, const umat2 * b1, const umat2 * z)
{
	umat2_dot_rev(b0, a);
	umat2_mod(a, z);
	umat2_dot(a, b1);
	umat2_mod(a, z);
}

/**
 * Return a bitmask where ecah 4-bit
 * is f if values are equal
 * 
 * @param [in] a,b matrices
 * @return compare value
 */
static FORCE_INLINE int umat2_cmpeq(const umat2 * a, const umat2 * b)
{
	return _mm_movemask_epi8(_mm_cmpeq_epi32(a->vector, b->vector));
}

static FORCE_INLINE void umat2_print(const umat2 * m)
{
	printf("| %4u, %4u |\n", m->a, m->b);
	printf("| %4u, %4u |\n", m->c, m->d);
}

extern const umat2 eye;