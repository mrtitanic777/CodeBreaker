# -*- coding: utf-8 -*-
"""
runtime.py -- support files the converter emits alongside the translated classes.

These stand in for Java runtime types that aren't part of the decompiled corpus
(java.util.Random, ...). They are emitted into the output by the pipeline and
#included by any class that references them (see typemap.RUNTIME_HEADERS).
"""

# Faithful java.util.Random: 48-bit LCG with Java's exact constants, so generated
# worlds/sequences match the Java original bit-for-bit.
RANDOM_H = r'''#ifndef RANDOM_H
#define RANDOM_H
// java.util.Random (48-bit LCG, Java constants) -- runtime support emitted by j2x

class Random {
	long long seed;
	int next(int bits) {
		seed = (seed * 0x5DEECE66DLL + 0xBLL) & ((1LL << 48) - 1);
		return (int)((long long)((unsigned long long)seed >> (48 - bits)));
	}
public:
	Random() { setSeed(0LL); }
	Random(long long s) { setSeed(s); }
	void setSeed(long long s) { seed = (s ^ 0x5DEECE66DLL) & ((1LL << 48) - 1); }
	int nextInt() { return next(32); }
	int nextInt(int bound) {
		if ((bound & -bound) == bound) return (int)(((long long)bound * (long long)next(31)) >> 31);
		int bits, val;
		do { bits = next(31); val = bits % bound; } while (bits - val + (bound - 1) < 0);
		return val;
	}
	long long nextLong() { return ((long long)next(32) << 32) + (long long)next(32); }
	bool nextBoolean() { return next(1) != 0; }
	float nextFloat() { return next(24) / (float)(1 << 24); }
	double nextDouble() {
		return (double)(((long long)next(26) << 27) + (long long)next(27)) / (double)(1LL << 53);
	}
	float nextGaussian() { return nextFloat(); }   // PORT: approximate (not a true gaussian)
};

#endif // RANDOM_H
'''

# filename -> content
FILES = {'Random.h': RANDOM_H}
