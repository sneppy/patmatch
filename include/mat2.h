#pragma once

#include "core_types.h"
#include "math.h"

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
		uint64 a, b, c, d;
	};

	/// Array memory
	uint64 array[4];

	/// Matrix memory
	uint64 matrix[2][2];
} umat2;

/**
 * Perform basic element-wise
 * arithmetic operations
 * 
 * @param [out] a destination operand
 * @param [in] b source operand
 * @param [in] mp montgomery parameters
 * @{
 */
static FORCE_INLINE void umat2_mul(umat2 * a, const umat2 * b, const MontgomeryParams * mp)
{
	montgomeryMul(a->a, b->a, mp);
	montgomeryMul(a->b, b->b, mp);
	montgomeryMul(a->c, b->c, mp);
	montgomeryMul(a->d, b->d, mp);
}
/// @}

/**
 * Perform matrix dot product
 * 
 * @param [out] a destination operand
 * @param [in] b source operand
 * @param [in] mp montgomery parameters
 */
static FORCE_INLINE void umat2_dot(umat2 * a, const umat2 * b, const MontgomeryParams * mp)
{
	uint64 x, y, z, w;
	x = montgomeryAdd(montgomeryMul(a->a, b->a, mp), montgomeryMul(a->b, b->c, mp), mp);
	y = montgomeryAdd(montgomeryMul(a->a, b->b, mp), montgomeryMul(a->b, b->d, mp), mp);
	z = montgomeryAdd(montgomeryMul(a->c, b->a, mp), montgomeryMul(a->d, b->c, mp), mp);
	w = montgomeryAdd(montgomeryMul(a->c, b->b, mp), montgomeryMul(a->d, b->d, mp), mp);

	a->a = x;
	a->b = y;
	a->c = z;
	a->d = w;
}
/// @}

/**
 * Like @ref umat2_dot but invert
 * destination operand
 * 
 * @param [in] a source operand
 * @param [out] b destination operand
 * @param [in] mp montgomery parameters
 */
static FORCE_INLINE void umat2_dot_rev(const umat2 * a, umat2 * b, const MontgomeryParams * mp)
{
	uint64 x, y, z, w;
	x = montgomeryAdd(montgomeryMul(a->a, b->a, mp), montgomeryMul(a->b, b->c, mp), mp);
	y = montgomeryAdd(montgomeryMul(a->a, b->b, mp), montgomeryMul(a->b, b->d, mp), mp);
	z = montgomeryAdd(montgomeryMul(a->c, b->a, mp), montgomeryMul(a->d, b->c, mp), mp);
	w = montgomeryAdd(montgomeryMul(a->c, b->b, mp), montgomeryMul(a->d, b->d, mp), mp);

	b->a = x;
	b->b = y;
	b->c = z;
	b->d = w;
}
/// @}

/**
 * Multiply inner matrix by b0 and b1
 * For example B-1 x A x B = A
 * 
 * @param [in] b0,b1 outer matrices
 * @param [out] a destination operand
 */
static FORCE_INLINE void umat2_dot_bab(const umat2 * b0, umat2 * a, const umat2 * b1, const MontgomeryParams * mp)
{
	umat2_dot_rev(b0, a, mp);
	umat2_dot(a, b1, mp);
}
/// @}

/**
 * Return a bitmask where ecah 4-bit
 * is f if values are equal
 * 
 * @param [in] a,b matrices
 * @return compare value
 */
static FORCE_INLINE int umat2_cmpeq(const umat2 * a, const umat2 * b)
{
	return a->a == b->a && a->b == b->b && a->c == b->c && a->d == b->d;
}

/* static FORCE_INLINE void umat2_print(const umat2 * m)
{
	printf("| %4u, %4u |\n", m->a, m->b);
	printf("| %4u, %4u |\n", m->c, m->d);
} */

static FORCE_INLINE void montgomeryFromMat2U(umat2 * m, const MontgomeryParams * mp)
{
	m->a = montgomeryFromU32((uint32)m->a, mp);
	m->b = montgomeryFromU32((uint32)m->b, mp);
	m->c = montgomeryFromU32((uint32)m->c, mp);
	m->d = montgomeryFromU32((uint32)m->d, mp);
}

extern const umat2 eye;