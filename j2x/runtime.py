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

GL11_H = r'''#ifndef GL11_H
#define GL11_H
// LWJGL org.lwjgl.opengl.GL11 stub -- symbols only, so render code COMPILES; the real
// pipeline is the platform layer (Xbox 360 = Direct3D 9). Values are real GL 1.1 enums.

namespace GL11 {
	const int GL_POINTS = 0x0000;
	const int GL_FRONT = 0x0404;
	const int GL_BACK = 0x0405;
	const int GL_FRONT_AND_BACK = 0x0408;
	const int GL_LIGHTING = 0x0B50;
	const int GL_LIGHT0 = 0x4000;
	const int GL_LIGHT1 = 0x4001;
	const int GL_FOG = 0x0B60;
	const int GL_FOG_MODE = 0x0B65;
	const int GL_FOG_DENSITY = 0x0B62;
	const int GL_FOG_START = 0x0B63;
	const int GL_FOG_END = 0x0B64;
	const int GL_FOG_COLOR = 0x0B66;
	const int GL_DEPTH_TEST = 0x0B71;
	const int GL_CULL_FACE = 0x0B44;
	const int GL_ALPHA_TEST = 0x0BC0;
	const int GL_BLEND = 0x0BE2;
	const int GL_TEXTURE_2D = 0x0DE1;
	const int GL_NORMALIZE = 0x0BA1;
	const int GL_COLOR_MATERIAL = 0x0B57;
	const int GL_MODELVIEW = 0x1700;
	const int GL_PROJECTION = 0x1701;
	const int GL_SMOOTH = 0x1D01;
	const int GL_EXP = 0x0800;
	const int GL_LINEAR = 0x2601;
	const int GL_NEAREST = 0x2600;
	const int GL_GREATER = 0x0204;
	const int GL_EQUAL = 0x0202;
	const int GL_LEQUAL = 0x0203;
	const int GL_SRC_ALPHA = 0x0302;
	const int GL_ONE_MINUS_SRC_ALPHA = 0x0303;
	const int GL_ONE = 0x0001;
	const int GL_DST_COLOR = 0x0306;
	const int GL_SRC_COLOR = 0x0300;
	const int GL_ONE_MINUS_SRC_COLOR = 0x0301;
	const int GL_ONE_MINUS_DST_COLOR = 0x0307;
	const int GL_DST_ALPHA = 0x0304;
	const int GL_RGB = 0x1907;
	const int GL_RGBA = 0x1908;
	const int GL_UNSIGNED_BYTE = 0x1401;
	const int GL_FLOAT = 0x1406;
	const int GL_TEXTURE_MAG_FILTER = 0x2800;
	const int GL_TEXTURE_MIN_FILTER = 0x2801;
	const int GL_TEXTURE_WRAP_S = 0x2802;
	const int GL_TEXTURE_WRAP_T = 0x2803;
	const int GL_REPEAT = 0x2901;
	const int GL_CLAMP = 0x2900;
	const int GL_COLOR_BUFFER_BIT = 0x4000;
	const int GL_DEPTH_BUFFER_BIT = 0x0100;
	const int GL_COMPILE = 0x1300;
	const int GL_VERTEX_ARRAY = 0x8074;
	const int GL_COLOR_ARRAY = 0x8076;
	const int GL_TEXTURE_COORD_ARRAY = 0x8078;
	const int GL_AMBIENT = 0x1200;
	const int GL_DIFFUSE = 0x1201;
	const int GL_SPECULAR = 0x1202;
	const int GL_POSITION = 0x1203;
	const int GL_AMBIENT_AND_DIFFUSE = 0x1602;
	const int GL_LIGHT_MODEL_AMBIENT = 0x0B53;
	const int GL_PACK_ALIGNMENT = 0x0D05;
	const int GL_VENDOR = 0x1F00;
	const int GL_RENDERER = 0x1F01;
	const int GL_VERSION = 0x1F02;

	inline void glAlphaFunc(...) {}
	inline void glBindTexture(...) {}
	inline void glBlendFunc(...) {}
	inline void glCallList(...) {}
	inline void glCallLists(...) {}
	inline void glClear(...) {}
	inline void glClearColor(...) {}
	inline void glClearDepth(...) {}
	inline void glColor3f(...) {}
	inline void glColor4f(...) {}
	inline void glColorMask(...) {}
	inline void glColorMaterial(...) {}
	inline void glColorPointer(...) {}
	inline void glCullFace(...) {}
	inline void glDeleteTextures(...) {}
	inline void glDepthFunc(...) {}
	inline void glDepthMask(...) {}
	inline void glDisable(...) {}
	inline void glDisableClientState(...) {}
	inline void glDrawArrays(...) {}
	inline void glEnable(...) {}
	inline void glEnableClientState(...) {}
	inline void glEndList(...) {}
	inline void glFog(...) {}
	inline void glFogf(...) {}
	inline void glFogi(...) {}
	inline void glGenTextures(...) {}
	inline void glLight(...) {}
	inline void glLightModel(...) {}
	inline void glLineWidth(...) {}
	inline void glLoadIdentity(...) {}
	inline void glMatrixMode(...) {}
	inline void glMultMatrix(...) {}
	inline void glNewList(...) {}
	inline void glNormal3f(...) {}
	inline void glOrtho(...) {}
	inline void glPixelStorei(...) {}
	inline void glPopMatrix(...) {}
	inline void glPushMatrix(...) {}
	inline void glReadPixels(...) {}
	inline void glRenderList(...) {}
	inline void glRotatef(...) {}
	inline void glScalef(...) {}
	inline void glShadeModel(...) {}
	inline void glSkyList(...) {}
	inline void glTexCoordPointer(...) {}
	inline void glTexImage2D(...) {}
	inline void glTexParameteri(...) {}
	inline void glTexSubImage2D(...) {}
	inline void glTranslatef(...) {}
	inline void glVertexPointer(...) {}
	inline void glViewport(...) {}
	inline int glGenLists(...) { return 1; }
	inline int glGenList(...) { return 1; }
	inline int glGetInteger(...) { return 0; }
	inline const char* glGetString(...) { return ""; }
}
#endif // GL11_H
'''

# filename -> content
FILES = {
    'Random.h': RANDOM_H,
    'JavaRT.h': JAVART_H,
    'GL11.h': GL11_H,
    'Runnable.h': RUNNABLE_H,
    'Thread.h': THREAD_H,
    'Comparator.h': COMPARATOR_H,
    'File.h': FILE_H,
    'FilenameFilter.h': FILENAMEFILTER_H,
    'Canvas.h': CANVAS_H,
    'Applet.h': APPLET_H,
}
