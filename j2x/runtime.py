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

# --- external Java base types our corpus extends/implements but that aren't decompiled.
# Minimal but faithful stubs so inheritance + overrides compile (a missing base header is
# a fatal include error that blocks the whole translation unit). ---------------------------

RUNNABLE_H = r'''#ifndef RUNNABLE_H
#define RUNNABLE_H
// java.lang.Runnable
struct Runnable {
	virtual ~Runnable() {}
	virtual void run() = 0;
};
#endif // RUNNABLE_H
'''

THREAD_H = r'''#ifndef THREAD_H
#define THREAD_H
// java.lang.Thread (stub: runs inline; real threading is a platform concern)
#include <string>
#include "Runnable.h"

class Thread : public Runnable {
public:
	Thread() {}
	Thread(const std::string&) {}
	virtual ~Thread() {}
	virtual void run() {}
	void start() { run(); }
	void setDaemon(bool) {}
	void setName(const std::string&) {}
	void setPriority(int) {}
	void interrupt() {}
	void join() {}
	bool isAlive() { return false; }
	static void sleep(long long) {}
	static void yield() {}
};
#endif // THREAD_H
'''

COMPARATOR_H = r'''#ifndef COMPARATOR_H
#define COMPARATOR_H
// java.util.Comparator (generics erase to void*)
struct Comparator {
	virtual ~Comparator() {}
	virtual int compare(void*, void*) = 0;
};
#endif // COMPARATOR_H
'''

FILE_H = r'''#ifndef FILE_H
#define FILE_H
// java.io.File (stub)
#include <string>

class File {
public:
	File() {}
	File(const std::string&) {}
	File(File*, const std::string&) {}
	virtual ~File() {}
	std::string getName() { return std::string(); }
	std::string getPath() { return std::string(); }
	std::string getAbsolutePath() { return std::string(); }
	bool exists() { return false; }
	bool isDirectory() { return false; }
	bool isFile() { return false; }
	bool mkdir() { return false; }
	bool mkdirs() { return false; }
	bool renameTo(File*) { return false; }
	long long length() { return 0LL; }
};
#endif // FILE_H
'''

FILENAMEFILTER_H = r'''#ifndef FILENAMEFILTER_H
#define FILENAMEFILTER_H
// java.io.FilenameFilter
#include <string>
#include "File.h"
struct FilenameFilter {
	virtual ~FilenameFilter() {}
	virtual bool accept(File*, const std::string&) = 0;
};
#endif // FILENAMEFILTER_H
'''

CANVAS_H = r'''#ifndef CANVAS_H
#define CANVAS_H
// java.awt.Canvas (stub: AWT is desktop-only; console port does not render through it)
struct Canvas {
	virtual ~Canvas() {}
};
#endif // CANVAS_H
'''

APPLET_H = r'''#ifndef APPLET_H
#define APPLET_H
// java.applet.Applet (stub: desktop-only entry point)
struct Applet {
	virtual ~Applet() {}
};
#endif // APPLET_H
'''

# Universal runtime prelude: free-function shims for java.lang.* statics that have no
# clean inline mapping. #included by every emitted .cpp (named JavaRT.h, NOT Math.h, to
# avoid colliding with system <math.h> on a case-insensitive filesystem).
JAVART_H = r'''#ifndef JAVART_H
#define JAVART_H
// j2x runtime prelude -- shims for Java stdlib statics
#include <cstdlib>

// java.lang.Math.random(): pseudo-random double in [0, 1)
inline double javaMathRandom() { return (double)rand() / ((double)RAND_MAX + 1.0); }
// java.lang.Math.signum(x)
template<class T> inline T javaMathSignum(T v) { return v > 0 ? (T)1 : (v < 0 ? (T)-1 : (T)0); }

#endif // JAVART_H
'''

# filename -> content
FILES = {
    'Random.h': RANDOM_H,
    'JavaRT.h': JAVART_H,
    'Runnable.h': RUNNABLE_H,
    'Thread.h': THREAD_H,
    'Comparator.h': COMPARATOR_H,
    'File.h': FILE_H,
    'FilenameFilter.h': FILENAMEFILTER_H,
    'Canvas.h': CANVAS_H,
    'Applet.h': APPLET_H,
}
