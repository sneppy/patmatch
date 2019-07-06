#pragma once

#include "core_types.h"

/**
 * Returns true if number is prime
 * 
 * @param [in] number
 * @return true if is prime
 */
ubyte isPrime(uint32 n);

/**
 * Generate a random prime in the
 * range [a,b)
 * 
 * @param [in] a,b acceptable range
 */
uint32 getRandomPrime(uint32 a, uint32 b);