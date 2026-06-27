#ifndef JAVART_H
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
	inline long long currentTimeMillis() { return (long long)time(0) * 1000LL; }
	inline long long nanoTime() { return (long long)clock(); }
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
