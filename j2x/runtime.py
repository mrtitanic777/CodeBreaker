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
# java.lang.String -> a thin std::string subclass that adds Java's String methods, so calls
# like s.equals(t) / s.trim() / s.startsWith(p) compile even when the receiver's type wasn't
# resolved at translation time (the receiver-type-gated rewrites in statements.py handle the
# resolved cases; this catches the rest). Implicit ctors from const char*/std::string keep
# literals and concat/substr results assignable.
JAVASTRING_H = r'''#ifndef JAVASTRING_H
#define JAVASTRING_H
#include <string>

class JavaString : public std::string {
	static char _lc(char c) { return (char)((c >= 'A' && c <= 'Z') ? c + 32 : c); }
	static char _uc(char c) { return (char)((c >= 'a' && c <= 'z') ? c - 32 : c); }
public:
	JavaString() {}
	JavaString(const char* s) : std::string(s ? s : "") {}
	JavaString(const std::string& s) : std::string(s) {}

	int length() const { return (int)size(); }
	char charAt(int i) const { return (*this)[(size_type)i]; }
	bool isEmpty() const { return empty(); }
	bool equals(const std::string& o) const { return *this == o; }
	bool equalsIgnoreCase(const std::string& o) const {
		if (size() != o.size()) return false;
		for (size_type i = 0; i < size(); ++i) if (_lc((*this)[i]) != _lc(o[i])) return false;
		return true;
	}
	bool startsWith(const std::string& p) const {
		return size() >= p.size() && compare(0, p.size(), p) == 0;
	}
	bool startsWith(const std::string& p, int off) const {
		return (size_type)off <= size() && compare((size_type)off, p.size(), p) == 0;
	}
	bool endsWith(const std::string& s) const {
		return size() >= s.size() && compare(size() - s.size(), s.size(), s) == 0;
	}
	bool contains(const std::string& s) const { return find(s) != npos; }
	int indexOf(const std::string& s) const { size_type p = find(s); return p == npos ? -1 : (int)p; }
	int indexOf(const std::string& s, int from) const { size_type p = find(s, (size_type)from); return p == npos ? -1 : (int)p; }
	int indexOf(int ch) const { size_type p = find((char)ch); return p == npos ? -1 : (int)p; }
	int indexOf(int ch, int from) const { size_type p = find((char)ch, (size_type)from); return p == npos ? -1 : (int)p; }
	int lastIndexOf(int ch) const { size_type p = rfind((char)ch); return p == npos ? -1 : (int)p; }
	int compareTo(const std::string& o) const { return compare(o); }
	JavaString substring(int a) const { return JavaString(substr((size_type)a)); }
	JavaString substring(int a, int b) const { return JavaString(substr((size_type)a, (size_type)(b - a))); }
	JavaString toLowerCase() const { JavaString r(*this); for (size_type i = 0; i < r.size(); ++i) r[i] = _lc(r[i]); return r; }
	JavaString toUpperCase() const { JavaString r(*this); for (size_type i = 0; i < r.size(); ++i) r[i] = _uc(r[i]); return r; }
	JavaString trim() const {
		size_type a = 0, b = size();
		while (a < b && (unsigned char)(*this)[a] <= ' ') ++a;
		while (b > a && (unsigned char)(*this)[b - 1] <= ' ') --b;
		return JavaString(substr(a, b - a));
	}
	JavaString replace(char from, char to) const {
		JavaString r(*this); for (size_type i = 0; i < r.size(); ++i) if (r[i] == from) r[i] = to; return r;
	}
	JavaString concat(const std::string& s) const { return JavaString(*this + s); }
	JavaString intern() const { return *this; }
	const char* toCharArray() const { return c_str(); }              // PORT: read-only view
	unsigned char* getBytes() const { return (unsigned char*)c_str(); }   // PORT: read-only view
	JavaString replaceAll(const std::string&, const std::string&) const { return *this; }  // PORT: no regex
};
#endif // JAVASTRING_H
'''

JAVART_H = r'''#ifndef JAVART_H
#define JAVART_H
// j2x runtime prelude -- shims for Java stdlib statics. #included by every .cpp.
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include "JavaString.h"

// java.lang.Math.random(): pseudo-random double in [0, 1)
inline double javaMathRandom() { return (double)rand() / ((double)RAND_MAX + 1.0); }
// java.lang.Math.signum(x)
template<class T> inline T javaMathSignum(T v) { return v > 0 ? (T)1 : (v < 0 ? (T)-1 : (T)0); }

// numeric parse helper (C++98-portable; avoids C99 strtoll on the real MSVC target)
template<class T> inline T _javart_parse(const std::string& s) {
	T v = T(); std::istringstream is(s); is >> v; return v;
}

// java.lang.System
namespace System {
	struct _PrintStream {
		template<class T> void println(const T&) {}
		void println() {}
		template<class T> void print(const T&) {}
	};
	static _PrintStream out;
	static _PrintStream err;
	inline long long currentTimeMillis() { return (long long)time(0) * 1000LL; }
	inline long long nanoTime() { return (long long)clock(); }
	inline void gc() {}
	inline void exit(int) {}
	inline std::string getProperty(const std::string&) { return std::string(); }
	inline std::string getenv(const std::string&) { return std::string(); }
	template<class S, class D> inline void arraycopy(S src, int sp, D dst, int dp, int len) {
		for (int i = 0; i < len; ++i) dst[dp + i] = src[sp + i];
	}
}

// java.lang.Integer / Long / Double / Float / Character (boxed types map to primitives)
namespace Integer {
	const int MAX_VALUE = 2147483647;
	const int MIN_VALUE = (-2147483647 - 1);
	inline int parseInt(const std::string& s) { return _javart_parse<int>(s); }
	inline int parseInt(const std::string& s, int) { return _javart_parse<int>(s); }
	inline int valueOf(int v) { return v; }
	inline int valueOf(const std::string& s) { return _javart_parse<int>(s); }
	inline std::string toString(int v) { std::ostringstream o; o << v; return o.str(); }
	inline std::string toBinaryString(int) { return std::string(); }
	inline std::string toHexString(int) { return std::string(); }
}
namespace Long {
	inline long long parseLong(const std::string& s) { return _javart_parse<long long>(s); }
	inline long long valueOf(long long v) { return v; }
	inline std::string toString(long long v) { std::ostringstream o; o << v; return o.str(); }
}
namespace Double {
	inline double parseDouble(const std::string& s) { return _javart_parse<double>(s); }
	inline bool isNaN(double v) { return v != v; }
}
namespace Float {
	const float POSITIVE_INFINITY = 3.4e38f;
	const float NEGATIVE_INFINITY = -3.4e38f;
	inline float parseFloat(const std::string& s) { return _javart_parse<float>(s); }
	inline bool isNaN(float v) { return v != v; }
	inline int floatToRawIntBits(float v) { union { float f; int i; } u; u.f = v; return u.i; }
	inline float intBitsToFloat(int v) { union { float f; int i; } u; u.i = v; return u.f; }
}
namespace Character {
	inline char valueOf(char c) { return c; }
	inline bool isDigit(char c) { return c >= '0' && c <= '9'; }
	inline bool isLetter(char c) { return (c>='a'&&c<='z') || (c>='A'&&c<='Z'); }
}

// java.util.Arrays (stubs: length isn't recoverable generically here)
namespace Arrays {
	template<class A, class V> inline void fill(A, V) {}
	template<class A, class V> inline void fill(A, int, int, V) {}
	template<class A> inline void sort(A) {}
	template<class A, class C> inline void sort(A, C) {}
}

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

# --- generated stubs for the remaining external platform/IO/AWT types ---------------------
# These are SYMBOL stubs so game logic COMPILES; the real implementations are the Xbox 360
# platform layer (Direct3D 9 / XInput / sound / file IO), out of scope for the converter.

def _guard(name):
    return name.upper().replace('.', '_') + '_H'


def _ns_stub(name, consts=(), void_fns=(), ret_fns=(), includes=(), fwds=()):
    """A namespace of static constants + functions (e.g. GL15, Keyboard) -- accessed Name::X."""
    g = _guard(name)
    L = ['#ifndef %s' % g, '#define %s' % g, '#include <string>']
    L += ['#include "%s.h"' % h for h in includes]
    L += ['class %s;' % f for f in fwds]
    L += ['', 'namespace %s {' % name]
    for k, v in consts:
        L.append('\tconst int %s = %s;' % (k, v))
    for f in void_fns:
        L.append('\tinline void %s(...) {}' % f)
    for f, (rt, rv) in ret_fns:
        L.append('\tinline %s %s(...) { return %s; }' % (rt, f, rv))
    L += ['}', '#endif // %s' % g, '']
    return '\n'.join(L)


def _class_stub(name, void_methods=(), ret_methods=(), ctors=True, bases=(),
                includes=(), fwds=()):
    """A class with variadic stub methods (e.g. SoundSystem, DataInput) -- accessed var->X."""
    g = _guard(name)
    L = ['#ifndef %s' % g, '#define %s' % g, '#include <string>']
    L += ['#include "%s.h"' % h for h in includes]
    L += ['class %s;' % f for f in fwds]
    decl = 'class %s' % name + ((' : ' + ', '.join('public %s' % b for b in bases)) if bases else '')
    L += ['', decl + ' {', 'public:']
    if ctors:
        L.append('\t%s(...) {}' % name)
    L.append('\tvirtual ~%s() {}' % name)
    for m in void_methods:
        L.append('\tvirtual void %s(...) {}' % m)
    for m, (rt, rv) in ret_methods:
        L.append('\tvirtual %s %s(...) { return %s; }' % (rt, m, rv))
    L += ['};', '#endif // %s' % g, '']
    return '\n'.join(L)


# NIO buffers: real classes (not raw pointers) so the .clear()/.put()/.flip() calls compile.
# Cross-return by pointer (forward-declared) to avoid include cycles.
def _buffer_stub(name):
    g = _guard(name)
    return '\n'.join([
        '#ifndef %s' % g, '#define %s' % g,
        'class FloatBuffer; class IntBuffer; class ByteBuffer;', '',
        'class %s {' % name, 'public:',
        '\t%s() {}' % name, '\tvirtual ~%s() {}' % name,
        '\t%s& clear(...) { return *this; }' % name,
        '\t%s& flip(...) { return *this; }' % name,
        '\t%s& put(...) { return *this; }' % name,
        '\t%s& position(...) { return *this; }' % name,
        '\t%s& limit(...) { return *this; }' % name,
        '\tint get(...) { return 0; }',
        '\tint remaining() { return 0; }',
        '\tint capacity() { return 0; }',
        '\tFloatBuffer* asFloatBuffer() { return 0; }',
        '\tIntBuffer* asIntBuffer() { return 0; }',
        '};', '#endif // %s' % g, ''])


_GENERATED = {}
# LWJGL input / display (static holders)
_GENERATED['Keyboard.h'] = _ns_stub('Keyboard',
    consts=[('KEY_ESCAPE', 1), ('KEY_LSHIFT', 42), ('KEY_RSHIFT', 54),
            ('KEY_F5', 63), ('KEY_F7', 65), ('KEY_F11', 87)],
    void_fns=['create', 'destroy', 'enableRepeatEvents'],
    ret_fns=[('getEventKey', ('int', '0')), ('getEventKeyState', ('bool', 'false')),
             ('getEventCharacter', ('char', '0')), ('isKeyDown', ('bool', 'false')),
             ('next', ('bool', 'false')), ('getKeyName', ('std::string', 'std::string()'))])
_GENERATED['Mouse.h'] = _ns_stub('Mouse',
    void_fns=['create', 'destroy', 'setNativeCursor'],
    ret_fns=[('getDX', ('int', '0')), ('getDY', ('int', '0')), ('getX', ('int', '0')),
             ('getY', ('int', '0')), ('getEventButton', ('int', '0')),
             ('getEventButtonState', ('bool', 'false')), ('getEventDWheel', ('int', '0')),
             ('getEventX', ('int', '0')), ('getEventY', ('int', '0')),
             ('isButtonDown', ('bool', 'false')), ('next', ('bool', 'false'))])
_GENERATED['DisplayMode.h'] = _class_stub('DisplayMode',
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getFrequency', ('int', '0')), ('getBitsPerPixel', ('int', '0'))])
_GENERATED['Display.h'] = _ns_stub('Display', includes=['DisplayMode'],
    void_fns=['create', 'destroy', 'setDisplayMode', 'setFullscreen', 'setParent',
              'setTitle', 'update'],
    ret_fns=[('isActive', ('bool', 'false')), ('isCloseRequested', ('bool', 'false')),
             ('getDisplayMode', ('DisplayMode', 'DisplayMode()')),
             ('getDesktopDisplayMode', ('DisplayMode', 'DisplayMode()'))])
_GENERATED['GL15.h'] = _ns_stub('GL15',
    consts=[('GL_ARRAY_BUFFER', 0x8892), ('GL_STREAM_DRAW', 0x88E0), ('GL_SAMPLES_PASSED', 0x8914),
            ('GL_QUERY_RESULT', 0x8866), ('GL_QUERY_RESULT_AVAILABLE', 0x8867),
            ('GL_QUERY_COUNTER_BITS', 0x8864)])
_GENERATED['GLU.h'] = _ns_stub('GLU', void_fns=['gluPerspective', 'gluLookAt', 'gluProject', 'gluUnProject'])
_GENERATED['ARBOcclusionQuery.h'] = _ns_stub('ARBOcclusionQuery',
    void_fns=['glBeginQueryARB', 'glEndQueryARB', 'glGetQueryObjectuARB', 'glGetQueryARB'],
    ret_fns=[('glGenQueriesARB', ('int', '0'))])
_GENERATED['ARBVertexBufferObject.h'] = _ns_stub('ARBVertexBufferObject',
    void_fns=['glBindBufferARB', 'glBufferDataARB', 'glDeleteBuffersARB'],
    ret_fns=[('glGenBuffersARB', ('int', '0'))])
_GENERATED['BufferUtils.h'] = _ns_stub('BufferUtils', fwds=['IntBuffer', 'FloatBuffer', 'ByteBuffer'],
    ret_fns=[('createIntBuffer', ('IntBuffer*', '0')), ('createFloatBuffer', ('FloatBuffer*', '0')),
             ('createByteBuffer', ('ByteBuffer*', '0'))])
_GENERATED['ImageIO.h'] = _ns_stub('ImageIO', fwds=['BufferedImage'],
    void_fns=['write'], ret_fns=[('read', ('BufferedImage*', '0'))])
# NIO buffers (real classes)
for _b in ('FloatBuffer', 'IntBuffer', 'ByteBuffer'):
    _GENERATED[_b + '.h'] = _buffer_stub(_b)
# java.io data streams (interfaces -> default-impl structs)
_GENERATED['DataInput.h'] = _class_stub('DataInput', void_methods=['readFully'],
    ret_methods=[('readByte', ('unsigned char', '0')), ('readUnsignedByte', ('int', '0')),
                 ('readShort', ('short', '0')), ('readUnsignedShort', ('int', '0')),
                 ('readInt', ('int', '0')), ('readLong', ('long long', '0LL')),
                 ('readFloat', ('float', '0.0F')), ('readDouble', ('double', '0.0')),
                 ('readBoolean', ('bool', 'false')), ('readChar', ('char', '0')),
                 ('readLine', ('std::string', 'std::string()')),
                 ('readUTF', ('std::string', 'std::string()'))])
_GENERATED['DataOutput.h'] = _class_stub('DataOutput',
    void_methods=['write', 'writeByte', 'writeShort', 'writeInt', 'writeLong', 'writeFloat',
                  'writeDouble', 'writeBoolean', 'writeChar', 'writeUTF', 'writeBytes'])
_GENERATED['SoundSystem.h'] = _class_stub('SoundSystem',
    void_methods=['cleanup', 'newSource', 'newStreamingSource', 'play', 'setListenerOrientation',
                  'setListenerPosition', 'setPitch', 'setVolume', 'stop', 'pause', 'removeSource'],
    ret_methods=[('playing', ('bool', 'false'))])
# small java.io / java.awt / java.net stubs (Applet/Canvas/Component/File/FileDialog/
# BufferedImage are enriched further below)
_GENERATED['BufferedReader.h'] = _class_stub('BufferedReader',
    void_methods=['close'], ret_methods=[('readLine', ('std::string', 'std::string()')),
                                         ('read', ('int', '0')), ('ready', ('bool', 'false'))])
_GENERATED['Graphics.h'] = _class_stub('Graphics',
    void_methods=['drawImage', 'dispose', 'setColor', 'fillRect', 'drawString'])
_GENERATED['Cursor.h'] = _class_stub('Cursor')
_GENERATED['Robot.h'] = _class_stub('Robot', void_methods=['mouseMove'])
_GENERATED['URL.h'] = _class_stub('URL',
    ret_methods=[('openConnection', ('void*', '0')), ('openStream', ('void*', '0')),
                 ('toString', ('std::string', 'std::string()'))])
_GENERATED['FileDialog.h'] = _class_stub('FileDialog', void_methods=['setVisible', 'setMode'],
    ret_methods=[('getFile', ('std::string', 'std::string()')),
                 ('getDirectory', ('std::string', 'std::string()'))])
_GENERATED['DecimalFormat.h'] = _class_stub('DecimalFormat',
    ret_methods=[('format', ('std::string', 'std::string()'))])
_GENERATED['ImageObserver.h'] = _class_stub('ImageObserver')
# Java exception hierarchy (exceptions are OFF on the console; these are just type stubs so
# `extends Error`, `new IOException(...)`, and catch/throws references compile).
_EXC_METHODS = [('getMessage', ('std::string', 'std::string()')),
                ('getLocalizedMessage', ('std::string', 'std::string()')),
                ('toString', ('std::string', 'std::string()'))]
_GENERATED['Throwable.h'] = _class_stub('Throwable', void_methods=['printStackTrace'], ret_methods=_EXC_METHODS)
for _e, _base in [('Exception', 'Throwable'), ('Error', 'Throwable'), ('RuntimeException', 'Exception'),
                  ('IOException', 'Exception'), ('InterruptedException', 'Exception'),
                  ('IllegalStateException', 'RuntimeException'), ('IllegalArgumentException', 'RuntimeException'),
                  ('MalformedURLException', 'IOException'), ('AWTException', 'Exception'),
                  ('LWJGLException', 'Exception'), ('NoSuchFieldError', 'Error')]:
    _GENERATED[_e + '.h'] = _class_stub(_e, bases=[_base], includes=[_base])
# java.io.File (richer than the early minimal stub; overrides it via FILES.update)
_GENERATED['File.h'] = _class_stub('File', fwds=['URL'],
    ret_methods=[('getName', ('std::string', 'std::string()')), ('getPath', ('std::string', 'std::string()')),
                 ('getAbsolutePath', ('std::string', 'std::string()')), ('toString', ('std::string', 'std::string()')),
                 ('exists', ('bool', 'false')), ('isDirectory', ('bool', 'false')), ('isFile', ('bool', 'false')),
                 ('mkdir', ('bool', 'false')), ('mkdirs', ('bool', 'false')), ('renameTo', ('bool', 'false')),
                 ('length', ('long long', '0LL')), ('getParentFile', ('File*', '0')),
                 ('getAbsoluteFile', ('File*', '0')), ('toURI', ('void*', '0')), ('listFiles', ('void*', '0'))])
# java.awt GUI bases (desktop-only entry points: MinecraftApplet extends Applet, etc.)
_GENERATED['Applet.h'] = _class_stub('Applet', fwds=['URL', 'Graphics'],
    void_methods=['validate', 'setLayout', 'add', 'repaint', 'requestFocus', 'setVisible',
                  'resize', 'init', 'start', 'stop', 'remove', 'setSize'],
    ret_methods=[('getParameter', ('std::string', 'std::string()')),
                 ('getDocumentBase', ('URL*', '0')), ('getCodeBase', ('URL*', '0')),
                 ('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0'))])
_GENERATED['Canvas.h'] = _class_stub('Canvas', fwds=['Graphics'],
    void_methods=['setFocusable', 'setSize', 'repaint', 'requestFocus', 'setVisible', 'addNotify',
                  'setPreferredSize'],
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0'))])
_GENERATED['Component.h'] = _class_stub('Component', fwds=['Graphics'],
    void_methods=['setSize', 'setVisible', 'repaint', 'requestFocus', 'validate', 'setLayout', 'add'],
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0')), ('getLocationOnScreen', ('void*', '0'))])
_GENERATED['FileDialog.h'] = _class_stub('FileDialog',
    void_methods=['setVisible', 'setMode', 'setDirectory', 'setFile', 'setFilenameFilter',
                  'setLocationRelativeTo'],
    ret_methods=[('getFile', ('std::string', 'std::string()')),
                 ('getDirectory', ('std::string', 'std::string()'))])
_GENERATED['BufferedImage.h'] = _class_stub('BufferedImage', fwds=['Graphics'],
    void_methods=['setRGB', 'flush'],
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getRGB', ('int', '0')), ('getData', ('void*', '0')),
                 ('getGraphics', ('Graphics*', '0')), ('createGraphics', ('Graphics*', '0'))])


# filename -> content
FILES = {
    'Random.h': RANDOM_H,
    'JavaString.h': JAVASTRING_H,
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
FILES.update(_GENERATED)
