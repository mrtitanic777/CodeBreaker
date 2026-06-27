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
// java.lang.Runnable -- declared `class` (not struct) so it agrees with every forward
// declaration of it (the header emitter forward-declares reference types as `class X;`,
// and MinecraftApplet's local fwd-decl does too) -> no C4099 struct/class mismatch.
class Runnable {
public:
	virtual ~Runnable() {}
	virtual void run() = 0;
};
#endif // RUNNABLE_H
'''

THREAD_H = r'''#ifndef THREAD_H
#define THREAD_H
// java.lang.Thread -- SELF-CONTAINED STUB for the converter's syntax gate (output_alpha, no platform
// headers). start() is a no-op here so output compiles standalone. The REAL build uses the platform
// version docs/platform/Thread.reference.h, which spawns REAL Xbox 360 threads (CreateThread) and a
// REAL Thread::sleep -- the swap copies it over this stub. (Do NOT run run() inline: ThreadSleepForever
// is `while(mc->running) sleep()`, an infinite loop on the main thread.)
#include <string>
#include "Runnable.h"

class Thread : public Runnable {
public:
	Thread() {}
	Thread(const std::string&) {}
	template<class R> Thread(R*) {}                       // new Thread(runnable)
	template<class R> Thread(R*, const std::string&) {}   // new Thread(runnable, name)
	virtual ~Thread() {}
	virtual void run() {}
	void start() {}                                       // NO-OP (see header note) -- do not run inline
	void setDaemon(bool) {}
	void setName(const std::string&) {}
	void setPriority(int) {}
	void interrupt() {}
	void join() {}
	void stop() {}
	void suspend() {}
	void resume() {}
	bool isAlive() { return false; }
	static void sleep(long long) {}
	static void yield() {}
};
#endif // THREAD_H
'''

COMPARATOR_H = r'''#ifndef COMPARATOR_H
#define COMPARATOR_H
#include "JavaRT.h"
// java.util.Comparator. Java's compare(Object, Object) lowers to compare(JObject, JObject);
// JObject converts to/from any T* and void*, so void*-based call sites still bind.
struct Comparator {
	virtual ~Comparator() {}
	virtual int compare(JObject, JObject) = 0;
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
#include <vector>
#include "JArray.h"

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
	int lastIndexOf(const std::string& s) const { size_type p = rfind(s); return p == npos ? -1 : (int)p; }
	int compareTo(const std::string& o) const { return compare(o); }
	int hashCode() const { int h = 0; for (size_type i = 0; i < size(); ++i) h = h * 31 + (*this)[i]; return h; }
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
	unsigned char* getBytes(...) const { return (unsigned char*)c_str(); }   // PORT: read-only view; ignores charset
	JavaString replaceAll(const std::string&, const std::string&) const { return *this; }  // PORT: no regex
	// String.split: splits on a LITERAL delimiter (regex not ported). Returns a JArray.
	JArray<JavaString> split(const std::string& delim) const {
		std::vector<JavaString> parts;
		if (delim.empty()) { parts.push_back(*this); }
		else {
			size_type start = 0, p;
			while ((p = find(delim, start)) != npos) {
				parts.push_back(JavaString(substr(start, p - start)));
				start = p + delim.size();
			}
			parts.push_back(JavaString(substr(start)));
		}
		JArray<JavaString> r((int)parts.size());
		for (size_type i = 0; i < parts.size(); ++i) r[(int)i] = parts[i];
		return r;
	}
	// Java `someString == null` / `!= null`. In the value model a String is never a pointer,
	// so "null" maps to the empty string (absence). NULL is an integer 0 here, hence long long.
	bool operator==(long long v) const { return v == 0 && empty(); }
	bool operator!=(long long v) const { return !(v == 0 && empty()); }
};
#endif // JAVASTRING_H
'''

JARRAY_H = r'''#ifndef JARRAY_H
#define JARRAY_H
// java.lang array: a length-carrying, BOUNDS-CHECKED view over a heap buffer. Java arrays
// know their length and throw on out-of-range; here an out-of-range index returns a safe
// dummy slot instead of corrupting memory or wild-pointing (the 0xC.../IP-0 console crashes).
// Non-owning on copy (shallow) so a by-value copy never double-frees -- game arrays are
// long-lived. Implicit conversion to T* for GL / nio-buffer / System.arraycopy interop.

template<class T>
class JArray {
	T* _buf;
	static T& _oob() { static T dummy; dummy = T(); return dummy; }   // safe slot for bad index
public:
	int length;                                  // Java: arr.length
	JArray() : _buf(0), length(0) {}
	explicit JArray(int n) : _buf(n > 0 ? new T[n]() : 0), length(n > 0 ? n : 0) {}
	JArray(T* buf, int n) : _buf(buf), length(n) {}
	T& operator[](int i) { return (i >= 0 && i < length) ? _buf[i] : _oob(); }
	const T& operator[](int i) const { return (i >= 0 && i < length) ? _buf[i] : _oob(); }
	operator T*() { return _buf; }               // raw-pointer interop
	operator const T*() const { return _buf; }
	T* data() { return _buf; }
};

// Multi-dimensional `new T[a][b]` / `new T[a][b][c]` -> nested JArray construction. C++ has no
// jagged-new in one expression; these build the rectangular nesting (each inner row its own JArray).
template<class T> JArray<JArray<T> > j2x_newarr2(int a, int b) {
	JArray<JArray<T> > r(a);
	for (int i = 0; i < a; ++i) r[i] = JArray<T>(b);
	return r;
}
template<class T> JArray<JArray<JArray<T> > > j2x_newarr3(int a, int b, int c) {
	JArray<JArray<JArray<T> > > r(a);
	for (int i = 0; i < a; ++i) {
		r[i] = JArray<JArray<T> >(b);
		for (int j = 0; j < b; ++j) r[i][j] = JArray<T>(c);
	}
	return r;
}
#endif // JARRAY_H
'''

JLIST_H = r'''#ifndef JLIST_H
#define JLIST_H
#include <vector>
#include <algorithm>

// java.util.List / ArrayList / LinkedList, held BY VALUE. Subclasses std::vector<T> so every
// existing STL use keeps working (iterators, operator[], size(), push_back, range loops, and
// passing where a std::vector& is expected); ADDS the Java List API the converter does not
// rewrite to STL: remove (by index AND by value), contains, indexOf, set, 2-arg add. This is
// the JavaString pattern for collections -- members beat fragile text rewrites, and are the
// only correct option when the receiver is a chained expression or the arg has side effects
// (e.g. list.remove(i--)).
template<class T>
class JList : public std::vector<T> {
public:
	typedef typename std::vector<T>::iterator _it;
	typedef typename std::vector<T>::const_iterator _cit;
	JList() {}
	JList(long long) {}                                  // Java `List x = null;` -> empty list
	void add(const T& v) { this->push_back(v); }
	void add(int i, const T& v) { this->insert(this->begin() + i, v); }
	template<class C> void addAll(const C& o) { this->insert(this->end(), o.begin(), o.end()); }  // any container (vector/set/list)
	T get(int i) const { return (*this)[(size_t)i]; }
	T set(int i, const T& v) { T old = (*this)[(size_t)i]; (*this)[(size_t)i] = v; return old; }
	bool isEmpty() const { return this->empty(); }
	bool contains(const T& v) const { return std::find(this->begin(), this->end(), v) != this->end(); }
	int indexOf(const T& v) const {
		_cit it = std::find(this->begin(), this->end(), v);
		return it == this->end() ? -1 : (int)(it - this->begin());
	}
	T remove(int i) {                                    // List.remove(int index) -> removed element
		T v = (*this)[(size_t)i]; this->erase(this->begin() + i); return v;
	}
	template<class U> bool remove(const U& v) {          // List.remove(Object) -> was-present
		_it it = std::find(this->begin(), this->end(), v);
		if (it == this->end()) return false;
		this->erase(it); return true;
	}
	// Java `list != null` / `== null`: a by-value list is never null (it always exists).
	bool operator==(long long) const { return false; }
	bool operator!=(long long) const { return true; }
};
#endif // JLIST_H
'''

JAVART_H = r'''#ifndef JAVART_H
#define JAVART_H
// j2x runtime prelude -- shims for Java stdlib statics. #included by every .cpp.
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <typeinfo>
#include "JavaString.h"
#include "JArray.h"
#include "JList.h"

// java.lang.Math.random(): pseudo-random double in [0, 1)
inline double javaMathRandom() { return (double)rand() / ((double)RAND_MAX + 1.0); }
// java.lang.Math.signum(x)
template<class T> inline T javaMathSignum(T v) { return v > 0 ? (T)1 : (v < 0 ? (T)-1 : (T)0); }

// numeric parse helper (C++98-portable; avoids C99 strtoll on the real MSVC target)
template<class T> inline T _javart_parse(const std::string& s) {
	T v = T(); std::istringstream is(s); is >> v; return v;
}

// Java string concatenation builder. Java's `"x" + n + obj` mixes a String with any
// type; C++ can't `+` a string literal with an int/float/object. The converter rewrites
// such a chain to `(JCat() + "x" + n + obj)`: each `+` appends the operand, and the
// result converts to JavaString/std::string in the surrounding value context.
// NOTE: accumulates into a std::string (NOT an ostringstream member) so JCat is COPYABLE --
// it gets passed by value through the variadic `(...)` runtime/exception stubs
// (`throw new IOException((JCat()+...))`), where an ostringstream member's private copy ctor
// would be a hard error.
struct JCat {
	std::string _s;
	template<class T> JCat& operator+(const T& v) { std::ostringstream o; o << v; _s += o.str(); return *this; }
	JCat& operator+(bool v) { _s += (v ? "true" : "false"); return *this; }   // Java prints true/false
	JCat& operator+(const char* v) { if (v) _s += v; return *this; }
	JCat& operator+(const std::string& v) { _s += v; return *this; }
	operator JavaString() const { return JavaString(_s); }
	operator std::string() const { return _s; }
};

// Java array aggregate `new T[]{a, b, c}` builder. C++03 has no array initializer-list for a
// heap array, so the converter rewrites it to `(JArrayInit<T>(), a, b, c)`: the comma operator
// appends each element and the result converts to JArray<T> in the value context.
template<class T> struct JArrayInit {
	std::vector<T> _v;
	JArrayInit& operator,(const T& x) { _v.push_back(x); return *this; }
	operator JArray<T>() const {
		JArray<T> r((int)_v.size());
		for (unsigned i = 0; i < _v.size(); ++i) r[(int)i] = _v[i];
		return r;
	}
};

// Boxed java.lang.Object element, ONLY for `Object[]` (Object[] -> JArray<JObject>). Java's mixed
// `new Object[]{"str", Character.valueOf('c'), block, item}` autoboxes primitives; C++ void* can't
// hold a char/String AND be RTTI-decoded, so JObject is a tagged box. Autoboxes from char/int/
// String/any pointer; converts back via casts; `instanceof Character/String/...` -> isXxx(). Scalar
// `Object` stays void* (opaque refs don't need this).
struct JObject {
	int kind;                         // 0 null, 1 int, 2 char, 4 String, 5 ptr, 6 String[], 7 bool
	long long i;
	JavaString s;
	JArray<JavaString> sa;
	void* p;
	const std::type_info* ti;         // for kind==5: the boxed pointer's static type (for is<T>())
	JObject() : kind(0), i(0), p(0), ti(0) {}
	JObject(int v) : kind(v ? 1 : 0), i(v), p(0), ti(0) {}        // 0 == null (NULL is 0); else boxed int
	JObject(char v) : kind(2), i(v), p(0), ti(0) {}
	JObject(bool v) : kind(7), i(v ? 1 : 0), p(0), ti(0) {}
	JObject(const char* v) : kind(4), i(0), s(v), p(0), ti(0) {}
	JObject(const JavaString& v) : kind(4), i(0), s(v), p(0), ti(0) {}
	JObject(const JArray<JavaString>& v) : kind(6), i(0), sa(v), p(0), ti(0) {}
	template<class T> JObject(T* v) : kind(v ? 5 : 0), i(0), p((void*)v), ti(v ? &typeid(T) : 0) {}
	// Java instanceof:
	bool isCharacter() const { return kind == 2; }
	bool isString() const { return kind == 4; }
	bool isInteger() const { return kind == 1; }
	bool isBoolean() const { return kind == 7; }
	bool isStringArray() const { return kind == 6; }
	template<class T> bool is() const { return kind == 5 && ti && *ti == typeid(T); }   // instanceof <Class>
	// named extractors (Java unbox: ((Integer)o).intValue(), ((Character)o).charValue(), ...):
	char charValue() const { return (char)i; }
	int intValue() const { return (int)i; }
	bool booleanValue() const { return i != 0; }
	JavaString str() const { return s; }
	JArray<JavaString> strArray() const { return sa; }
	template<class T> T* as() const { return (T*)p; }
	// null compare (Object x == null -> x == 0, since NULL is 0). operator==(int) is an exact
	// match for the int 0, so it wins over the operator int() user-conversion path -> no ambiguity
	// (verified on g++ -std=c++03: == 0 with both present compiles without warning).
	bool operator==(int v) const { return v == 0 && (kind == 0 || (kind == 5 && p == 0)); }
	bool operator!=(int v) const { return !(*this == v); }
	bool operator<(const JObject& o) const { return p < o.p || (p == o.p && i < o.i); }   // map-key order
	// conversion operators used by converter-emitted casts: (int)x, (char)x, (String)x, (Block*)x,
	// void* interop. NOTE: no operator bool() -- it would make `if(x)` ambiguous with operator int().
	operator int() const { return (int)i; }
	operator char() const { return (char)i; }
	operator JavaString() const { return s; }
	template<class T> operator T*() const { return (T*)p; }
};

// java.lang.System
namespace System {
	struct _PrintStream {
		template<class T> void println(const T&) {}
		void println() {}
		template<class T> void print(const T&) {}
	};
	static _PrintStream out;
	static _PrintStream err;
	// REAL millisecond clock. time(0)*1000 had ONE-SECOND resolution, which broke everything timed:
	// currentTimeMillis()%1000 was always 0 (menu splash never pulsed) and the game Timer drives ticks
	// off this, so runTick/handleInput/animation fired in once-per-second bursts (unresponsive input).
	// GetTickCount() is monotonic ms since boot (wraps at ~49 days -- irrelevant for a session).
	inline long long currentTimeMillis() { return (long long)GetTickCount(); }
	inline long long nanoTime() {
		LARGE_INTEGER f, c; QueryPerformanceFrequency(&f); QueryPerformanceCounter(&c);
		long long s = c.QuadPart / f.QuadPart, r = c.QuadPart % f.QuadPart;   // overflow-safe ns
		return s * 1000000000LL + (r * 1000000000LL) / f.QuadPart;
	}
	inline void gc() {}
	inline void exit(int) {}
	inline JavaString getProperty(const std::string&) { return JavaString(); }
	inline JavaString getProperty(const std::string&, const std::string& def) { return JavaString(def); }
	inline JavaString getenv(const std::string&) { return JavaString(); }
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
	const long long MAX_VALUE = 9223372036854775807LL;
	const long long MIN_VALUE = (-9223372036854775807LL - 1LL);
	inline long long parseLong(const std::string& s) { return _javart_parse<long long>(s); }
	inline long long valueOf(long long v) { return v; }
	inline std::string toString(long long v) { std::ostringstream o; o << v; return o.str(); }
}
namespace Double {
	const double MAX_VALUE = 1.7976931348623157e308;
	const double MIN_VALUE = 4.9e-324;
	inline double parseDouble(const std::string& s) { return _javart_parse<double>(s); }
	inline double valueOf(double v) { return v; }
	inline bool isNaN(double v) { return v != v; }
	inline std::string toString(double v) { std::ostringstream o; o << v; return o.str(); }
}
namespace Float {
	const float MAX_VALUE = 3.402823466e+38f;   // FLT_MAX (3.4028235e38f rounds ABOVE it -> MSVC C2177)
	const float MIN_VALUE = 1.4e-45f;
	const float POSITIVE_INFINITY = 3.4e38f;
	const float NEGATIVE_INFINITY = -3.4e38f;
	inline float parseFloat(const std::string& s) { return _javart_parse<float>(s); }
	inline float valueOf(float v) { return v; }
	inline bool isNaN(float v) { return v != v; }
	inline int floatToRawIntBits(float v) { union { float f; int i; } u; u.f = v; return u.i; }
	inline float intBitsToFloat(int v) { union { float f; int i; } u; u.i = v; return u.f; }
}
namespace Character {
	const char MIN_VALUE = 0;
	const char MAX_VALUE = (char)0xFFFF;
	inline char valueOf(char c) { return c; }
	inline bool isDigit(char c) { return c >= '0' && c <= '9'; }
	inline bool isLetter(char c) { return (c>='a'&&c<='z') || (c>='A'&&c<='Z'); }
	inline bool isLetterOrDigit(char c) { return isLetter(c) || isDigit(c); }
	inline bool isWhitespace(char c) { return c==' '||c=='\t'||c=='\n'||c=='\r'; }
	inline char toLowerCase(char c) { return (c>='A'&&c<='Z') ? (char)(c+32) : c; }
	inline char toUpperCase(char c) { return (c>='a'&&c<='z') ? (char)(c-32) : c; }
	inline int getNumericValue(char c) { return (c>='0'&&c<='9') ? (c-'0') : -1; }
}
namespace Short {
	const short MAX_VALUE = 32767;
	const short MIN_VALUE = (short)-32768;
	inline short parseShort(const std::string& s) { return _javart_parse<short>(s); }
	inline short valueOf(short v) { return v; }
	inline std::string toString(short v) { std::ostringstream o; o << v; return o.str(); }
}
namespace Byte {
	const signed char MAX_VALUE = 127;
	const signed char MIN_VALUE = (signed char)-128;
	inline signed char parseByte(const std::string& s) { return _javart_parse<signed char>(s); }
	inline signed char valueOf(signed char v) { return v; }
}
namespace Boolean {
	// NOTE: no TRUE/FALSE constants -- TRUE/FALSE are #defined as macros by the Windows/Xbox 360
	// headers (xtl.h: `#define TRUE 1`), so `const bool TRUE = true;` becomes `const bool 1 = ...`
	// -> C2059/C2143 in every TU. Java Boolean.TRUE/FALSE (rare) should lower to the bool literals.
	inline bool parseBoolean(const std::string& s) { return s == "true"; }
	inline bool valueOf(bool v) { return v; }
	inline std::string toString(bool v) { return v ? "true" : "false"; }
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
	const int GL_FLAT = 0x1D00;
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
	const int GL_POLYGON_OFFSET_FILL = 0x8037;
	const int GL_PROJECTION_MATRIX = 0x0BA7;
	const int GL_MODELVIEW_MATRIX = 0x0BA6;

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
	inline int glGetError() { return 0; }
	inline void glDeleteLists(...) {}
	inline void glPolygonOffset(...) {}
	inline void glGetFloat(...) {}
}
#endif // GL11_H
'''

# --- generated stubs for the remaining external platform/IO/AWT types ---------------------
# These are SYMBOL stubs so game logic COMPILES; the real implementations are the Xbox 360
# platform layer (Direct3D 9 / XInput / sound / file IO), out of scope for the converter.

def _guard(name):
    return name.upper().replace('.', '_') + '_H'


# Best-effort Java package for the external types we stub, so the banner names the real library.
_EXT_PKG = {
    'GL11': 'org.lwjgl.opengl', 'GL12': 'org.lwjgl.opengl', 'GL15': 'org.lwjgl.opengl',
    'GLU': 'org.lwjgl.util.glu', 'GLContext': 'org.lwjgl.opengl', 'ARBOcclusionQuery': 'org.lwjgl.opengl',
    'ARBVertexBufferObject': 'org.lwjgl.opengl', 'Keyboard': 'org.lwjgl.input', 'Mouse': 'org.lwjgl.input',
    'Controllers': 'org.lwjgl.input', 'Display': 'org.lwjgl.opengl', 'DisplayMode': 'org.lwjgl.opengl',
    'BufferUtils': 'org.lwjgl', 'Sys': 'org.lwjgl',
    'File': 'java.io', 'InputStream': 'java.io', 'OutputStream': 'java.io', 'DataInput': 'java.io',
    'DataOutput': 'java.io', 'DataInputStream': 'java.io', 'DataOutputStream': 'java.io',
    'BufferedInputStream': 'java.io', 'BufferedOutputStream': 'java.io', 'FileInputStream': 'java.io',
    'FileOutputStream': 'java.io', 'BufferedReader': 'java.io',
    'FileReader': 'java.io', 'FileWriter': 'java.io', 'PrintWriter': 'java.io',
    'Socket': 'java.net', 'URL': 'java.net', 'URLConnection': 'java.net',
    'SoundSystem': 'paulscode.sound', 'CodecJOrbis': 'paulscode.sound.codecs',
    'ImageIO': 'javax.imageio', 'BufferedImage': 'java.awt.image', 'ImageObserver': 'java.awt.image',
    'ZipFile': 'java.util.zip', 'DecimalFormat': 'java.text', 'DateFormat': 'java.text',
    'Class': 'java.lang', 'Comparable': 'java.lang', 'Comparator': 'java.util',
    'Canvas': 'java.awt', 'Component': 'java.awt', 'Frame': 'java.awt', 'Panel': 'java.awt',
    'Graphics': 'java.awt', 'Color': 'java.awt', 'Cursor': 'java.awt', 'Robot': 'java.awt',
    'FileDialog': 'java.awt', 'Dimension': 'java.awt', 'Applet': 'java.applet',
    'BorderLayout': 'java.awt', 'GridLayout': 'java.awt', 'FlowLayout': 'java.awt',
    'CardLayout': 'java.awt', 'GridBagLayout': 'java.awt',
    'FloatBuffer': 'java.nio', 'IntBuffer': 'java.nio', 'ByteBuffer': 'java.nio',
}


def _ext_banner(name):
    """Header banner marking a SYMBOL-ONLY stub for an external Java library type (the user asked
    every such stub to say so). Names the real package when known. These are NOT decompiled game
    source; their behaviour is the Xbox 360 platform layer's job."""
    pkg = _EXT_PKG.get(name)
    who = ('%s.%s' % (pkg, name)) if pkg else ('the external Java type %s' % name)
    return ['',
            '// ============================ EXTERNAL-LIBRARY STUB ============================',
            '// Stands in for %s.' % who,
            '// This is NOT part of the decompiled Minecraft source -- it is an external Java',
            '// library. Symbol-only: methods are no-ops / return defaults so the converted game',
            '// code compiles and links; the real behaviour is the Xbox 360 platform layer',
            '// (Direct3D 9 / XInput / file IO / sound).',
            '// ==============================================================================']


def _ns_stub(name, consts=(), void_fns=(), ret_fns=(), includes=(), fwds=()):
    """A namespace of static constants + functions (e.g. GL15, Keyboard) -- accessed Name::X."""
    g = _guard(name)
    L = ['#ifndef %s' % g, '#define %s' % g] + _ext_banner(name) + ['#include <string>']
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
                includes=(), fwds=(), extra=()):
    """A class with variadic stub methods (e.g. SoundSystem, DataInput) -- accessed var->X.
    `extra` = verbatim extra member lines (e.g. conversion operators)."""
    g = _guard(name)
    L = ['#ifndef %s' % g, '#define %s' % g] + _ext_banner(name) + ['#include <string>']
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
    for line in extra:
        L.append('\t' + line)
    L += ['};', '#endif // %s' % g, '']
    return '\n'.join(L)


# NIO buffers: real classes (not raw pointers) so the .clear()/.put()/.flip() calls compile.
# Cross-return by pointer (forward-declared) to avoid include cycles.
def _buffer_stub(name):
    g = _guard(name)
    L = ['#ifndef %s' % g, '#define %s' % g] + _ext_banner(name) + [
         'class FloatBuffer; class IntBuffer; class ByteBuffer;', '',
         'class %s {' % name, 'public:',
         '\t%s() {}' % name, '\tvirtual ~%s() {}' % name]
    for m in ('clear', 'flip', 'rewind', 'mark'):                # no-arg, return self
        L.append('\t%s& %s() { return *this; }' % (name, m))
    for m in ('put', 'putInt', 'putShort', 'putLong', 'putFloat', 'putDouble', 'putChar'):
        L.append('\t%s& %s(...) { return *this; }' % (name, m))   # variadic, return self
    L += ['\tint get(...) { return 0; }', '\tint getInt(...) { return 0; }',
          '\tshort getShort(...) { return 0; }', '\tlong long getLong(...) { return 0; }',
          '\tfloat getFloat(...) { return 0.0F; }', '\tdouble getDouble(...) { return 0.0; }',
          '\tchar getChar(...) { return 0; }']
    for m in ('position', 'limit'):                              # getter + setter (no clash)
        L.append('\tint %s() { return 0; }' % m)
        L.append('\t%s& %s(int) { return *this; }' % (name, m))
    L += ['\tint remaining() { return 0; }', '\tint capacity() { return 0; }',
          '\tFloatBuffer* asFloatBuffer() { return 0; }', '\tIntBuffer* asIntBuffer() { return 0; }',
          '\toperator void*() { return 0; }', '};', '#endif // %s' % g, '']
    return '\n'.join(L)


_GENERATED = {}
# common java.awt Component/Container surface, shared by Canvas/Panel/Component/Applet stubs
_AWT_VOID = ['setFocusable', 'setSize', 'repaint', 'requestFocus', 'setVisible', 'addNotify',
             'removeNotify', 'setPreferredSize', 'setMinimumSize', 'setMaximumSize', 'enableInputMethods',
             'setBackground', 'setForeground', 'validate', 'setLayout', 'add', 'remove', 'removeAll',
             'addKeyListener', 'addMouseListener', 'addMouseMotionListener', 'addMouseWheelListener',
             'addFocusListener', 'addComponentListener', 'addWindowListener', 'setFocusTraversalKeysEnabled',
             'setCursor', 'createBufferStrategy', 'transferFocus', 'paint', 'update', 'doLayout',
             'invalidate', 'revalidate', 'pack']
# LWJGL input / display (static holders)
_GENERATED['Keyboard.h'] = _ns_stub('Keyboard',
    consts=[('KEY_ESCAPE', 1),
            ('KEY_1', 2), ('KEY_2', 3), ('KEY_3', 4), ('KEY_4', 5), ('KEY_5', 6),
            ('KEY_6', 7), ('KEY_7', 8), ('KEY_8', 9), ('KEY_9', 10), ('KEY_0', 11),
            ('KEY_MINUS', 12), ('KEY_EQUALS', 13), ('KEY_BACK', 14), ('KEY_TAB', 15),
            ('KEY_Q', 16), ('KEY_W', 17), ('KEY_E', 18), ('KEY_R', 19), ('KEY_T', 20),
            ('KEY_Y', 21), ('KEY_U', 22), ('KEY_I', 23), ('KEY_O', 24), ('KEY_P', 25),
            ('KEY_LBRACKET', 26), ('KEY_RBRACKET', 27), ('KEY_RETURN', 28), ('KEY_LCONTROL', 29),
            ('KEY_A', 30), ('KEY_S', 31), ('KEY_D', 32), ('KEY_F', 33), ('KEY_G', 34),
            ('KEY_H', 35), ('KEY_J', 36), ('KEY_K', 37), ('KEY_L', 38), ('KEY_SEMICOLON', 39),
            ('KEY_APOSTROPHE', 40), ('KEY_GRAVE', 41), ('KEY_LSHIFT', 42), ('KEY_BACKSLASH', 43),
            ('KEY_Z', 44), ('KEY_X', 45), ('KEY_C', 46), ('KEY_V', 47), ('KEY_B', 48),
            ('KEY_N', 49), ('KEY_M', 50), ('KEY_COMMA', 51), ('KEY_PERIOD', 52), ('KEY_SLASH', 53),
            ('KEY_RSHIFT', 54), ('KEY_MULTIPLY', 55), ('KEY_LMENU', 56), ('KEY_SPACE', 57),
            ('KEY_CAPITAL', 58),
            ('KEY_F1', 59), ('KEY_F2', 60), ('KEY_F3', 61), ('KEY_F4', 62), ('KEY_F5', 63),
            ('KEY_F6', 64), ('KEY_F7', 65), ('KEY_F8', 66), ('KEY_F9', 67), ('KEY_F10', 68),
            ('KEY_NUMLOCK', 69), ('KEY_SCROLL', 70), ('KEY_F11', 87), ('KEY_F12', 88),
            ('KEY_NUMPAD0', 82), ('KEY_NUMPAD1', 79), ('KEY_NUMPAD2', 80), ('KEY_NUMPAD3', 81),
            ('KEY_NUMPAD4', 75), ('KEY_NUMPAD5', 76), ('KEY_NUMPAD6', 77), ('KEY_NUMPAD7', 71),
            ('KEY_NUMPAD8', 72), ('KEY_NUMPAD9', 73), ('KEY_ADD', 78), ('KEY_SUBTRACT', 74),
            ('KEY_DECIMAL', 83), ('KEY_RCONTROL', 157), ('KEY_RMENU', 184), ('KEY_HOME', 199),
            ('KEY_UP', 200), ('KEY_PRIOR', 201), ('KEY_LEFT', 203), ('KEY_RIGHT', 205),
            ('KEY_END', 207), ('KEY_DOWN', 208), ('KEY_NEXT', 209), ('KEY_INSERT', 210),
            ('KEY_DELETE', 211), ('CHAR_NONE', 0), ('KEY_NONE', 0)],
    void_fns=['create', 'destroy', 'enableRepeatEvents'],
    ret_fns=[('getEventKey', ('int', '0')), ('getEventKeyState', ('bool', 'false')),
             ('getEventCharacter', ('char', '0')), ('isKeyDown', ('bool', 'false')),
             ('next', ('bool', 'false')), ('getKeyName', ('std::string', 'std::string()'))])
_GENERATED['Mouse.h'] = _ns_stub('Mouse',
    void_fns=['create', 'destroy', 'setNativeCursor', 'setGrabbed', 'setCursorPosition'],
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
              'setTitle', 'update', 'swapBuffers', 'setVSyncEnabled', 'setIcon', 'sync',
              'setResizable', 'setLocation', 'releaseContext', 'makeCurrent'],
    ret_fns=[('isActive', ('bool', 'false')), ('isCloseRequested', ('bool', 'false')),
             ('isCreated', ('bool', 'false')), ('isDirty', ('bool', 'false')),
             ('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
             ('getDisplayMode', ('DisplayMode', 'DisplayMode()')),
             ('getDesktopDisplayMode', ('DisplayMode', 'DisplayMode()')),
             ('getAvailableDisplayModes', ('void*', '0'))])
_GENERATED['GL15.h'] = _ns_stub('GL15',
    consts=[('GL_ARRAY_BUFFER', 0x8892), ('GL_STREAM_DRAW', 0x88E0), ('GL_SAMPLES_PASSED', 0x8914),
            ('GL_QUERY_RESULT', 0x8866), ('GL_QUERY_RESULT_AVAILABLE', 0x8867),
            ('GL_QUERY_COUNTER_BITS', 0x8864)])
_GENERATED['GL12.h'] = _ns_stub('GL12',
    consts=[('GL_RESCALE_NORMAL', 0x803A), ('GL_CLAMP_TO_EDGE', 0x812F),
            ('GL_SMOOTH_POINT_SIZE_RANGE', 0x0B12)])
_GENERATED['GLU.h'] = _ns_stub('GLU', void_fns=['gluPerspective', 'gluLookAt', 'gluProject', 'gluUnProject'],
    ret_fns=[('gluErrorString', ('std::string', 'std::string()')), ('gluBuild2DMipmaps', ('int', '0'))])
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
                  'writeDouble', 'writeBoolean', 'writeChar', 'writeUTF', 'writeBytes', 'writeChars'])
# java.io concrete streams (DataInputStream/DataOutputStream are the heavy hitters on alpha)
_GENERATED['InputStream.h'] = _class_stub('InputStream', void_methods=['close', 'reset', 'mark'],
    ret_methods=[('read', ('int', '0')), ('available', ('int', '0')), ('skip', ('long long', '0LL'))])
_GENERATED['OutputStream.h'] = _class_stub('OutputStream', void_methods=['write', 'flush', 'close'])
_GENERATED['DataInputStream.h'] = _class_stub('DataInputStream', bases=['DataInput'], includes=['DataInput'],
    void_methods=['close', 'reset', 'mark'],
    ret_methods=[('read', ('int', '0')), ('available', ('int', '0')),
                 ('skip', ('long long', '0LL')), ('skipBytes', ('int', '0'))])
_GENERATED['DataOutputStream.h'] = _class_stub('DataOutputStream', bases=['DataOutput'], includes=['DataOutput'],
    void_methods=['flush', 'close'], ret_methods=[('size', ('int', '0'))])
_GENERATED['BufferedInputStream.h'] = _class_stub('BufferedInputStream', bases=['InputStream'], includes=['InputStream'])
_GENERATED['BufferedOutputStream.h'] = _class_stub('BufferedOutputStream', bases=['OutputStream'], includes=['OutputStream'])
_GENERATED['FileInputStream.h'] = _class_stub('FileInputStream', bases=['InputStream'], includes=['InputStream'])
_GENERATED['FileOutputStream.h'] = _class_stub('FileOutputStream', bases=['OutputStream'], includes=['OutputStream'])
_GENERATED['ZipFile.h'] = _class_stub('ZipFile', void_methods=['close'],
    ret_methods=[('getEntry', ('void*', '0')), ('getInputStream', ('InputStream*', '0')),
                 ('size', ('int', '0'))], fwds=['InputStream'])
_GENERATED['Socket.h'] = _class_stub('Socket',
    void_methods=['close', 'setSoTimeout', 'setTrafficClass', 'setTcpNoDelay', 'setKeepAlive', 'connect'],
    ret_methods=[('getInputStream', ('InputStream*', '0')), ('getOutputStream', ('OutputStream*', '0')),
                 ('getRemoteSocketAddress', ('void*', '0')), ('getInetAddress', ('void*', '0')),
                 ('getPort', ('int', '0')), ('isConnected', ('bool', 'false')), ('isClosed', ('bool', 'false'))],
    fwds=['InputStream', 'OutputStream'])
_GENERATED['DateFormat.h'] = _class_stub('DateFormat',
    ret_methods=[('format', ('std::string', 'std::string()'))])
_GENERATED['Dimension.h'] = _class_stub('Dimension',
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0'))])
# java.awt event listeners/adapters (implemented/extended by the desktop applet+window code)
for _li in ('KeyListener', 'MouseListener', 'MouseMotionListener', 'MouseWheelListener',
            'WindowListener', 'ComponentListener', 'FocusListener'):
    _GENERATED[_li + '.h'] = _class_stub(_li,
        void_methods=['keyPressed', 'keyReleased', 'keyTyped', 'mouseClicked', 'mousePressed',
                      'mouseReleased', 'mouseEntered', 'mouseExited', 'mouseMoved', 'mouseDragged',
                      'mouseWheelMoved', 'windowClosing', 'windowOpened', 'windowClosed',
                      'windowActivated', 'windowDeactivated', 'windowIconified', 'windowDeiconified',
                      'componentResized', 'componentMoved', 'componentShown', 'componentHidden',
                      'focusGained', 'focusLost'])
_GENERATED['WindowAdapter.h'] = _class_stub('WindowAdapter',
    void_methods=['windowClosing', 'windowOpened', 'windowClosed', 'windowActivated',
                  'windowDeactivated', 'windowIconified', 'windowDeiconified'])
# java.awt event objects
for _ev in ('KeyEvent', 'MouseEvent', 'WindowEvent', 'ComponentEvent', 'FocusEvent', 'InputEvent'):
    _GENERATED[_ev + '.h'] = _class_stub(_ev, void_methods=['consume'],
        ret_methods=[('getKeyCode', ('int', '0')), ('getKeyChar', ('char', '0')),
                     ('getX', ('int', '0')), ('getY', ('int', '0')), ('getButton', ('int', '0')),
                     ('getClickCount', ('int', '0')), ('getWheelRotation', ('int', '0')),
                     ('getWindow', ('void*', '0')), ('getComponent', ('void*', '0')),
                     ('isShiftDown', ('bool', 'false')), ('isControlDown', ('bool', 'false')),
                     ('isAltDown', ('bool', 'false'))])
_GENERATED['Panel.h'] = _class_stub('Panel', bases=['Component'], includes=['Component'], fwds=['Graphics'],
    void_methods=_AWT_VOID,
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')), ('getGraphics', ('Graphics*', '0'))])
_GENERATED['Comparable.h'] = _class_stub('Comparable', ret_methods=[('compareTo', ('int', '0'))])
# java.awt layout managers (new BorderLayout() etc. passed to setLayout -- no-op stubs)
for _lm in ('BorderLayout', 'GridLayout', 'FlowLayout', 'CardLayout', 'GridBagLayout'):
    _GENERATED[_lm + '.h'] = _class_stub(_lm)
# java.awt.Frame / Window (MinecraftApplet's desktop host). Extends Component so a Frame* converts
# to the Component* that AWT APIs (and MinecraftImpl) accept.
_GENERATED['Frame.h'] = _class_stub('Frame', bases=['Component'], includes=['Component'], fwds=['Graphics'],
    void_methods=list(_AWT_VOID) + [
    'setTitle', 'setResizable', 'dispose', 'show', 'hide', 'toFront', 'setIconImage', 'setMenuBar',
    'setLocationRelativeTo', 'setLocation', 'setUndecorated', 'setState', 'setExtendedState'],
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0')), ('getInsets', ('void*', '0')),
                 ('getTitle', ('std::string', 'std::string()'))])
# java.awt.Color. Constants are static members named exactly as Java (Color is in RUNTIME_ENUM_LIKE
# so `Color.BLACK` -> `Color::BLACK`, no s_ prefix). They are `static const int` (ARGB) -- a real
# `static Color X;` member would need an out-of-line definition (none, since stubs are header-only)
# and fail to LINK; an integral constant with an in-class initializer needs no definition, and the
# variadic Color ctor / setColor accept the int.
_COLOR_RGB = [('BLACK', 0xFF000000), ('WHITE', 0xFFFFFFFF), ('GRAY', 0xFF808080),
              ('LIGHT_GRAY', 0xFFC0C0C0), ('DARK_GRAY', 0xFF404040), ('RED', 0xFFFF0000),
              ('GREEN', 0xFF00FF00), ('BLUE', 0xFF0000FF), ('YELLOW', 0xFFFFFF00),
              ('CYAN', 0xFF00FFFF), ('MAGENTA', 0xFFFF00FF), ('ORANGE', 0xFFFFC800),
              ('PINK', 0xFFFFAFAF),
              ('black', 0xFF000000), ('white', 0xFFFFFFFF), ('gray', 0xFF808080),
              ('lightGray', 0xFFC0C0C0), ('darkGray', 0xFF404040), ('red', 0xFFFF0000),
              ('green', 0xFF00FF00), ('blue', 0xFF0000FF), ('yellow', 0xFFFFFF00),
              ('cyan', 0xFF00FFFF), ('magenta', 0xFFFF00FF), ('orange', 0xFFFFC800),
              ('pink', 0xFFFFAFAF)]
# java.awt.Color -- NOT a symbol-only stub: getHSBColor/HSBtoRGB/getRGB are REAL. The Alpha sky color
# is MobSpawnerBase::getSkyColorByTemp() -> Color::getHSBColor(h,s,b).getRGB(); a stub returning
# Color()/0 made the in-world SKY render BLACK. Standard java.awt HSB->RGB algorithm below.
_GENERATED['Color.h'] = (
    "#ifndef COLOR_H\n#define COLOR_H\n#include <string>\n#include <math.h>\n"
    "class Color {\n"
    "    int _rgb;\n"
    "public:\n"
    "    Color() : _rgb(0) {}\n"
    "    Color(int rgb) : _rgb(rgb) {}\n"
    "    Color(int rgba, bool) : _rgb(rgba) {}\n"
    "    Color(...) : _rgb(0) {}\n"
    "    virtual ~Color() {}\n"
    "    virtual int getRGB()   { return _rgb; }\n"
    "    virtual int getRed()   { return (_rgb >> 16) & 255; }\n"
    "    virtual int getGreen() { return (_rgb >> 8) & 255; }\n"
    "    virtual int getBlue()  { return _rgb & 255; }\n"
    "    virtual int getAlpha() { return (_rgb >> 24) & 255; }\n"
    "    static int HSBtoRGB(float hue, float saturation, float brightness) {\n"
    "        int r=0,g=0,b=0;\n"
    "        if (saturation == 0.0f) { r=g=b=(int)(brightness*255.0f+0.5f); }\n"
    "        else {\n"
    "            float h=(hue-(float)floorf(hue))*6.0f; float f=h-(float)floorf(h);\n"
    "            float p=brightness*(1.0f-saturation); float q=brightness*(1.0f-saturation*f);\n"
    "            float t=brightness*(1.0f-(saturation*(1.0f-f)));\n"
    "            switch((int)h){\n"
    "            case 0: r=(int)(brightness*255.0f+0.5f); g=(int)(t*255.0f+0.5f); b=(int)(p*255.0f+0.5f); break;\n"
    "            case 1: r=(int)(q*255.0f+0.5f); g=(int)(brightness*255.0f+0.5f); b=(int)(p*255.0f+0.5f); break;\n"
    "            case 2: r=(int)(p*255.0f+0.5f); g=(int)(brightness*255.0f+0.5f); b=(int)(t*255.0f+0.5f); break;\n"
    "            case 3: r=(int)(p*255.0f+0.5f); g=(int)(q*255.0f+0.5f); b=(int)(brightness*255.0f+0.5f); break;\n"
    "            case 4: r=(int)(t*255.0f+0.5f); g=(int)(p*255.0f+0.5f); b=(int)(brightness*255.0f+0.5f); break;\n"
    "            case 5: r=(int)(brightness*255.0f+0.5f); g=(int)(p*255.0f+0.5f); b=(int)(q*255.0f+0.5f); break;\n"
    "            }\n"
    "        }\n"
    "        return (int)(0xFF000000u | ((unsigned)r<<16) | ((unsigned)g<<8) | (unsigned)b);\n"
    "    }\n"
    "    static Color getHSBColor(float h, float s, float b) { return Color(HSBtoRGB(h,s,b)); }\n"
    + ''.join("    static const int %s = (int)0x%08X;\n" % (n, v) for n, v in _COLOR_RGB)
    + "};\n#endif // COLOR_H\n")
# org.lwjgl.input.Controllers (static holder)
_GENERATED['Controllers.h'] = _ns_stub('Controllers',
    void_fns=['create', 'destroy', 'poll', 'clearEvents'],
    ret_fns=[('isCreated', ('bool', 'false')), ('getControllerCount', ('int', '0')),
             ('getController', ('void*', '0')), ('next', ('bool', 'false')),
             ('getEventSource', ('void*', '0')), ('getEventControlIndex', ('int', '0')),
             ('getEventButtonState', ('bool', 'false'))])
# org.lwjgl.Sys + org.lwjgl.opengl.GLContext (static holders)
_GENERATED['Sys.h'] = _ns_stub('Sys', void_fns=['openURL', 'alert'],
    ret_fns=[('getVersion', ('std::string', 'std::string()')), ('getTime', ('long long', '0LL')),
             ('getTimerResolution', ('long long', '1000LL'))])
_GENERATED['GLContext.h'] = r'''#ifndef GLCONTEXT_H
#define GLCONTEXT_H
// org.lwjgl.opengl.GLContext + ContextCapabilities stub. Capability flags are public bools
// (the real values come from the Xbox 360 platform/D3D9 layer) so `getCapabilities().GL_ARB_x`
// compiles. Default false = feature absent.
struct ContextCapabilities {
	bool GL_ARB_occlusion_query, GL_ARB_vertex_buffer_object, GL_ARB_multitexture;
	bool GL_ARB_texture_env_combine, GL_EXT_blend_func_separate, GL_ARB_shader_objects;
	bool GL_NV_fog_distance, GL_EXT_texture_filter_anisotropic, GL_ARB_texture_non_power_of_two;
	bool OpenGL11, OpenGL12, OpenGL13, OpenGL14, OpenGL15, OpenGL20, OpenGL21;
	ContextCapabilities() : GL_ARB_occlusion_query(false), GL_ARB_vertex_buffer_object(false),
		GL_ARB_multitexture(false), GL_ARB_texture_env_combine(false), GL_EXT_blend_func_separate(false),
		GL_ARB_shader_objects(false), GL_NV_fog_distance(false), GL_EXT_texture_filter_anisotropic(false),
		GL_ARB_texture_non_power_of_two(false), OpenGL11(false), OpenGL12(false), OpenGL13(false),
		OpenGL14(false), OpenGL15(false), OpenGL20(false), OpenGL21(false) {}
};
namespace GLContext {
	inline ContextCapabilities getCapabilities() { return ContextCapabilities(); }
}
#endif // GLCONTEXT_H
'''
_GENERATED['URLConnection.h'] = _class_stub('URLConnection', fwds=['InputStream', 'OutputStream'],
    void_methods=['connect', 'setDoInput', 'setDoOutput', 'setUseCaches', 'setRequestProperty',
                  'setConnectTimeout', 'setReadTimeout', 'disconnect'],
    ret_methods=[('getInputStream', ('InputStream*', '0')), ('getOutputStream', ('OutputStream*', '0')),
                 ('getContentLength', ('int', '0')), ('getResponseCode', ('int', '0')),
                 ('getHeaderField', ('std::string', 'std::string()'))])
_GENERATED['CodecJOrbis.h'] = _class_stub('CodecJOrbis', includes=['URL', 'URLConnection'],
    void_methods=['initialize', 'cleanup', 'setAudioFormat', 'reverseByteOrder'],
    ret_methods=[('read', ('void*', '0')), ('readAll', ('void*', '0')), ('getAudioFormat', ('void*', '0')),
                 ('initialized', ('bool', 'false')), ('endOfStream', ('bool', 'false'))],
    extra=['URL* url;', 'URLConnection urlConnection;'])   # paulscode ICodec protected fields
_GENERATED['SoundSystem.h'] = _class_stub('SoundSystem',
    void_methods=['cleanup', 'newSource', 'newStreamingSource', 'play', 'setListenerOrientation',
                  'setListenerPosition', 'setPitch', 'setVolume', 'stop', 'pause', 'removeSource'],
    ret_methods=[('playing', ('bool', 'false'))])
# small java.io / java.awt / java.net stubs (Applet/Canvas/Component/File/FileDialog/
# BufferedImage are enriched further below)
_GENERATED['BufferedReader.h'] = _class_stub('BufferedReader',
    void_methods=['close'], ret_methods=[('readLine', ('std::string', 'std::string()')),
                                         ('read', ('int', '0')), ('ready', ('bool', 'false'))])
# character streams for options.txt / crash reports (GameSettings, PanelCrashReport): FileReader is
# wrapped by BufferedReader; PrintWriter wraps FileWriter. Real impls are the Xbox file layer
# (docs/platform/FileIO.reference) -- BufferedReader.readLine etc. read nothing until then.
_GENERATED['FileReader.h'] = _class_stub('FileReader', void_methods=['close', 'mark', 'reset'],
    ret_methods=[('read', ('int', '0')), ('ready', ('bool', 'false'))])
_GENERATED['FileWriter.h'] = _class_stub('FileWriter', void_methods=['write', 'flush', 'close', 'append'])
# println/print/write take ANY type incl. JavaString -- which derives from std::string (non-trivial)
# and so must NOT be passed through a C-style `...`; member templates keep them compile-clean, the
# same way the GameSettings override models them. flush/close are variadic no-ops.
_GENERATED['PrintWriter.h'] = _class_stub('PrintWriter',
    void_methods=['flush', 'close'], ret_methods=[('checkError', ('bool', 'false'))],
    extra=['template<class T> void println(const T&) {}', 'void println() {}',
           'template<class T> void print(const T&) {}', 'void print() {}',
           'template<class T> void write(const T&) {}'])
_GENERATED['Graphics.h'] = _class_stub('Graphics',
    void_methods=['drawImage', 'dispose', 'setColor', 'fillRect', 'drawString'])
_GENERATED['Cursor.h'] = _class_stub('Cursor')
_GENERATED['Class.h'] = r'''#ifndef CLASS_H
#define CLASS_H
// java.lang.Class -- backed by C++ RTTI (std::type_info). Identity, ==, getName and map-key
// ordering are REAL: `X.class` (Class(typeid(X))) and `obj.getClass()` (Class(typeid(*obj)))
// agree for the same runtime type. newInstance/getConstructor have no reflection equivalent and
// return 0 -- the class-keyed registries (EntityList/Packet/TileEntity) supply explicit factories.
#include <string>
#include <typeinfo>
#include "URL.h"                                          // getResource returns a path-carrying URL
class InputStream;
InputStream* J2X_getResourceAsStream(const char* name);   // FileIO.cpp: "/x.png" -> game:\Content\x.png

class Class {
	const std::type_info* _ti;
public:
	Class() : _ti(0) {}
	Class(const std::type_info& ti) : _ti(&ti) {}
	std::string getName() const {
		if (!_ti) return std::string();
		std::string n(_ti->name());
		// MSVC type_info::name() returns "class Foo" / "struct Foo"; Java getName() is the BARE class
		// name, and the class-keyed render registries (RenderManager / TileEntityRenderer) key by bare-
		// name strings ("EntityPig"). Strip the C++ keyword prefix (+ any namespace) so lookups MATCH.
		if      (n.compare(0,6,"class ")  == 0) n.erase(0,6);
		else if (n.compare(0,7,"struct ") == 0) n.erase(0,7);
		else if (n.compare(0,5,"enum ")   == 0) n.erase(0,5);
		std::string::size_type cc = n.rfind("::");
		if (cc != std::string::npos) n.erase(0, cc + 2);
		return n;
	}
	std::string getSimpleName() const { return getName(); }
	bool equals(const Class& o) const { return *this == o; }
	bool operator==(const Class& o) const { return _ti == o._ti || (_ti && o._ti && *_ti == *o._ti); }
	bool operator!=(const Class& o) const { return !(*this == o); }
	bool operator<(const Class& o) const {                 // strict-weak order for std::map keys
		if (!_ti) return o._ti != 0;
		if (!o._ti) return false;
		return _ti->before(*o._ti) != 0;
	}
	int hashCode() const {
		std::string n = getName(); int h = 0;
		for (std::string::size_type i = 0; i < n.size(); ++i) h = h * 31 + (unsigned char)n[i];
		return h;
	}
	bool isInstance(...) const { return false; }
	bool isAssignableFrom(...) const { return false; }
	Class* getSuperclass() { return 0; }
	void* getConstructor(...) { return 0; }
	void* newInstance(...) { return 0; }
	void* getDeclaredFields(...) { return 0; }
	// Real resource resolution (was `return 0`, which made ImageIO::read(getResource("/x.png")) return
	// a NULL image -> the loader derefed NULL -> crash). Returns a URL carrying the path; ImageIO::
	// read(URL*) resolves it via game:\Content\... The variadic fallback keeps other call shapes.
	URL* getResource(const std::string& name) { return new URL(name); }
	URL* getResource(...) { return 0; }
	// Real resource resolution (was `return 0`, which made every RenderEngine texture load NULL ->
	// readTextureImage crashed on NULL->close()). Routes "/terrain.png","/title/mojang.png",... to
	// game:\Content\... via FileIO. const std::string& accepts JavaString (extends std::string) and
	// const char* literals; the variadic fallback keeps any other call shape compiling.
	InputStream* getResourceAsStream(const std::string& name) { return J2X_getResourceAsStream(name.c_str()); }
	InputStream* getResourceAsStream(...) { return 0; }
	operator Class*() { return this; }                     // usable where a Class* is expected
};
#endif // CLASS_H
'''
_GENERATED['Robot.h'] = _class_stub('Robot', void_methods=['mouseMove'])
# openStream() returns a real InputStream* (not void*) so `new DataInputStream(url.openStream())`
# binds to DataInputStream(InputStream*); parse(void*)/InputStreamReader(...) callers still accept it.
# URL carries a resource path when built by Class::getResource("/x.png") so ImageIO::read(URL*) can
# resolve it to game:\Content\... (the URL form of getResourceAsStream). openStream() returns
# InputStream* so `new DataInputStream(url.openStream())` binds.
_GENERATED['URL.h'] = _class_stub('URL', fwds=('InputStream',),
    ret_methods=[('openConnection', ('void*', '0')), ('openStream', ('InputStream*', '0')),
                 ('toString', ('std::string', 'std::string()')), ('getPath', ('std::string', 'std::string()')),
                 ('getFile', ('std::string', 'std::string()')), ('getProtocol', ('std::string', 'std::string()')),
                 ('getHost', ('std::string', 'std::string()')), ('getQuery', ('std::string', 'std::string()'))],
    extra=['std::string _j2xpath;',
           'URL(const std::string& p) : _j2xpath(p) {}',
           'const std::string& j2xResourcePath() const { return _j2xpath; }'])
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
                  ('LWJGLException', 'Exception'), ('NoSuchFieldError', 'Error'),
                  # java.io / java.net / java.lang exceptions used in catch/throws clauses
                  ('FileNotFoundException', 'IOException'), ('EOFException', 'IOException'),
                  ('SocketException', 'IOException'), ('UnknownHostException', 'IOException'),
                  ('ConnectException', 'SocketException'), ('BindException', 'SocketException'),
                  ('NumberFormatException', 'IllegalArgumentException'),
                  ('NullPointerException', 'RuntimeException'),
                  ('ArrayIndexOutOfBoundsException', 'RuntimeException'),
                  ('IndexOutOfBoundsException', 'RuntimeException'),
                  ('ClassCastException', 'RuntimeException'),
                  ('NoSuchElementException', 'RuntimeException'),
                  ('UnsupportedOperationException', 'RuntimeException'),
                  ('ConcurrentModificationException', 'RuntimeException'),
                  ('OutOfMemoryError', 'Error'), ('NoSuchMethodException', 'Exception'),
                  ('CloneNotSupportedException', 'Exception'), ('ClassNotFoundException', 'Exception')]:
    _GENERATED[_e + '.h'] = _class_stub(_e, bases=[_base], includes=[_base])
# java.io.File (richer than the early minimal stub; overrides it via FILES.update)
_GENERATED['File.h'] = _class_stub('File', fwds=['URL'],
    ret_methods=[('getName', ('std::string', 'std::string()')), ('getPath', ('std::string', 'std::string()')),
                 ('getAbsolutePath', ('std::string', 'std::string()')), ('toString', ('std::string', 'std::string()')),
                 ('exists', ('bool', 'false')), ('isDirectory', ('bool', 'false')), ('isFile', ('bool', 'false')),
                 ('mkdir', ('bool', 'false')), ('mkdirs', ('bool', 'false')), ('renameTo', ('bool', 'false')),
                 ('length', ('long long', '0LL')), ('getParentFile', ('File*', '0')),
                 ('getAbsoluteFile', ('File*', '0')), ('toURI', ('void*', '0')), ('listFiles', ('void*', '0')),
                 ('lastModified', ('long long', '0LL')), ('list', ('void*', '0')),
                 ('getParent', ('std::string', 'std::string()')), ('canRead', ('bool', 'false')),
                 ('canWrite', ('bool', 'false'))])
# java.awt GUI bases (desktop-only entry points: MinecraftApplet extends Applet, etc.)
_GENERATED['Applet.h'] = _class_stub('Applet', bases=['Component'], includes=['Component'], fwds=['URL', 'Graphics'],
    void_methods=['validate', 'setLayout', 'add', 'repaint', 'requestFocus', 'setVisible',
                  'resize', 'init', 'start', 'stop', 'remove', 'removeAll', 'setSize',
                  'setBackground', 'setForeground', 'addKeyListener', 'addMouseListener',
                  'addMouseMotionListener', 'addFocusListener', 'addComponentListener',
                  'setFocusable', 'setFocusTraversalKeysEnabled', 'enableInputMethods'],
    ret_methods=[('getParameter', ('std::string', 'std::string()')),
                 ('getDocumentBase', ('URL*', '0')), ('getCodeBase', ('URL*', '0')),
                 ('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0')), ('isActive', ('bool', 'false')),
                 ('isDisplayable', ('bool', 'false')), ('getParent', ('void*', '0'))])
_GENERATED['Canvas.h'] = _class_stub('Canvas', bases=['Component'], includes=['Component'], fwds=['Graphics'],
    void_methods=_AWT_VOID,
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0')), ('getLocationOnScreen', ('void*', '0')),
                 ('getBufferStrategy', ('void*', '0'))])
_GENERATED['Component.h'] = _class_stub('Component', fwds=['Graphics'], void_methods=_AWT_VOID,
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


COLLECTIONS_H = r'''#ifndef COLLECTIONS_H
#define COLLECTIONS_H
#include <algorithm>
#include "JavaRT.h"
#include "JList.h"
#include "Comparator.h"
// java.util.Collections. sort(List, Comparator) adapts Java's 3-way compare to a strict-weak
// less-than for std::sort. NOTE: std::sort is NOT stable (Collections.sort is guaranteed stable),
// so elements that compare equal may reorder. Comparator::compare takes JObject; the list element
// type converts to JObject implicitly (void* / typed pointers / boxed values all do).
namespace _j2x_coll {
	struct _CmpLess {
		Comparator* c;
		_CmpLess(Comparator* cc) : c(cc) {}
		template<class T> bool operator()(const T& a, const T& b) const { return c->compare(a, b) < 0; }
	};
}
struct Collections {
	template<class T> static void sort(JList<T>& list, Comparator* cmp) {
		std::sort(list.begin(), list.end(), _j2x_coll::_CmpLess(cmp));
	}
	template<class T> static void sort(JList<T>& list) { std::sort(list.begin(), list.end()); }
	template<class T> static void reverse(JList<T>& list) { std::reverse(list.begin(), list.end()); }
	template<class T> static void shuffle(JList<T>&) {}            // no-op (no RNG dependency here)
	template<class T> static void shuffle(JList<T>&, Random*) {}
};
#endif // COLLECTIONS_H
'''


# filename -> content
FILES = {
    'Random.h': RANDOM_H,
    'JavaString.h': JAVASTRING_H,
    'JArray.h': JARRAY_H,
    'JList.h': JLIST_H,
    'JavaRT.h': JAVART_H,
    'GL11.h': GL11_H,
    'Runnable.h': RUNNABLE_H,
    'Thread.h': THREAD_H,
    'Comparator.h': COMPARATOR_H,
    'Collections.h': COLLECTIONS_H,
    'File.h': FILE_H,
    'FilenameFilter.h': FILENAMEFILTER_H,
    'Canvas.h': CANVAS_H,
    'Applet.h': APPLET_H,
}
FILES.update(_GENERATED)
