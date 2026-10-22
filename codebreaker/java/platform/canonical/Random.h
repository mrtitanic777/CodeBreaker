#ifndef RANDOM_H
#define RANDOM_H
// java.util.Random (48-bit LCG, Java constants) -- runtime support emitted by CodeBreaker

class Random {
	long long seed;
	int next(int bits) {
		seed = (seed * 0x5DEECE66DLL + 0xBLL) & ((1LL << 48) - 1);
		return (int)((long long)((unsigned long long)seed >> (48 - bits)));
	}
public:
	Random() {
		// java.util.Random() is TIME-seeded. A fixed setSeed(0) made every `new Random()` identical each
		// boot -> the world seed (World's no-seed ctor uses new Random()->nextLong()) never changed = the
		// "same world every boot" bug. Vary it: a per-call uniquifier (so distinct Randoms differ within a
		// run) XOR GetTickCount (ms since console boot -> differs by when the world is created). GetTickCount
		// is already in scope via stdafx->xtl.h (same as JavaRT.h's System::currentTimeMillis()).
		static long long s_uniq = 0LL;
		s_uniq = s_uniq * 6364136223846793005LL + 1442695040888963407LL;
		setSeed(s_uniq ^ ((long long)GetTickCount() * 2862933555777941757LL));
	}
	Random(long long s) { setSeed(s); }
	void setSeed(long long s) { seed = (s ^ 0x5DEECE66DLL) & ((1LL << 48) - 1); }
	int nextInt() { return next(32); }
	int nextInt(int bound) {
		// J2X-BUGFIX nextint-nonpositive: Java throws IllegalArgumentException for bound <= 0; on console a
		// throw-that-isn't-caught is worse than a defined result, and a NEGATIVE bound makes the rejection
		// loop below (`bits - val + (bound-1) < 0`) NEVER terminate -> a hard hang. Guard it: return 0. This
		// also defuses the empty-collection idiom `x.get(rand.nextInt(x.size()))` when a converted x.size()
		// comes back as garbage/negative from a defaulted map value (see SoundPool.getRandomSound* / sound_fix).
		if (bound <= 0) return 0;
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
