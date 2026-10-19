#ifndef JAVART_H
#define JAVART_H
// j2ctool runtime prelude -- shims for Java stdlib statics. #included by every .cpp.
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <string>
#include <sstream>
#include <typeinfo>
#include <vector>
#include "JavaString.h"
#include "JArray.h"
#include "JIterator.h"
#include "JList.h"
#include "JSet.h"
#include "JMap.h"

// --- Guest-throw tracer -------------------------------------------------------------------------
// On the target console/emulator a C++ `throw` is a NO-OP that does NOT unwind: execution falls
// through the throw with corrupted state (a null result, a half-updated object) and crashes later,
// far from the cause. `throw` is therefore the single most recurring source of hard-to-locate
// crashes on this port. The converter prefixes EVERY emitted `throw` with J2X_GUEST_THROW() so the
// instant a landmine actually fires, the debug log names its exact file:line -- turning the opaque
// "Guest attempted to throw a C++ exception!" into an actionable location. Stays silent until a
// throw truly executes (correct paths never throw), so it costs nothing in the common case.
#if defined(_XBOX) || defined(_WIN32)
extern "C" unsigned long __cdecl DbgPrint(const char* Format, ...);   // kernel debug print (Nexia360 surfaces this)
inline void j2x_guest_throw_trace(const char* f, int line) {
	// Route through DbgPrint, NOT OutputDebugStringA: Nexia360 SWALLOWS the OutputDebugString channel but
	// DOES surface DbgPrint (that's how the visible "(DbgPrint)" [BOOT]/[INIT] lines appear). Using
	// OutputDebugStringA here made every [THROW] invisible in the log.
	DbgPrint("[THROW] %s:%d -- guest throw (no-op on target!)\n", f, line);
}
#else
inline void j2x_guest_throw_trace(const char*, int) {}   // host build (converter self-tests): no-op
#endif
#define J2X_GUEST_THROW() ::j2x_guest_throw_trace(__FILE__, __LINE__)

// Java's (int) and (long) of a float or double SATURATE -- NaN gives 0, out of range gives the nearest
// limit -- where C++'s are undefined (x86 gives 0x80000000). The converter writes every (int)x and
// (long long)x of an expression as these; any other operand converts as the C++ cast would.
// J2X-BUGFIX float-int-cast-saturate: MathHelper.sin(1000000F) indexed SIN_TABLE[(int)(x * 10430.378F)
// & 0xffff] with a C++-overflowed int, reading entry 0 where Java reads entry 65535.
template<class T> inline int j2xToInt(const T& v) { return (int)v; }
inline int j2xToInt(float v) {
	return v != v ? 0 : v >= 2147483647.0f ? 2147483647 : v <= -2147483648.0f ? (-2147483647 - 1) : (int)v;
}
inline int j2xToInt(double v) {
	return v != v ? 0 : v >= 2147483647.0 ? 2147483647 : v <= -2147483648.0 ? (-2147483647 - 1) : (int)v;
}
template<class T> inline long long j2xToLong(const T& v) { return (long long)v; }
inline long long j2xToLong(float v) {
	return v != v ? 0 : v >= 9223372036854775807.0f ? 9223372036854775807LL
	     : v <= -9223372036854775808.0f ? (-9223372036854775807LL - 1) : (long long)v;
}
inline long long j2xToLong(double v) {
	return v != v ? 0 : v >= 9223372036854775807.0 ? 9223372036854775807LL
	     : v <= -9223372036854775808.0 ? (-9223372036854775807LL - 1) : (long long)v;
}

// java.lang.Math.random(): pseudo-random double in [0, 1)
inline double javaMathRandom() { return (double)rand() / ((double)RAND_MAX + 1.0); }
// java.lang.Math.signum(x)
template<class T> inline T javaMathSignum(T v) { return v > 0 ? (T)1 : (v < 0 ? (T)-1 : (T)0); }
// java.lang.Math.sqrt(x): ALWAYS double, like Java. A bare `sqrt(floatExpr)` picks MSVC's float overload,
// so `atan2(Math.sqrt(f), d)` would be an ambiguous atan2(float, double) (C2668). The double parameter
// forces the promotion at the call. J2X-BUGFIX sqrt-double-precision.
inline double javaMathSqrt(double v) { return sqrt(v); }
// java.lang.Math.round(x): round-half-up to an integer (int for float, long long for double)
inline int javaMathRound(float v) { return (int)floorf(v + 0.5F); }
inline long long javaMathRound(double v) { return (long long)floor(v + 0.5); }

// numeric parse helper (C++98-portable; avoids C99 strtoll on the real MSVC target)
template<class T> inline T _javart_parse(const std::string& s) {
	T v = T(); std::istringstream is(s); is >> v; return v;
}

// Java `>>>` (unsigned/logical right shift). C++ `>>` on a signed value is arithmetic (sign-extends),
// so we shift the value reinterpreted as unsigned. Overloaded for the two integer widths Java uses.
inline int j2xUshr(int v, int n) { return (int)((unsigned int)v >> n); }
inline long long j2xUshr(long long v, int n) { return (long long)((unsigned long long)v >> n); }

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
	JCat& operator+(double v) { _s += j2xNumberText(v, false); return *this; }      // as Java prints a double
	JCat& operator+(float v) { _s += j2xNumberText(v, true); return *this; }        // ... and a float
	JCat& operator+(bool v) { _s += (v ? "true" : "false"); return *this; }   // Java prints true/false
	JCat& operator+(const char* v) { if (v) _s += v; return *this; }
	JCat& operator+(const std::string& v) { _s += v; return *this; }
	operator JavaString() const { return JavaString(_s); }   // also reaches std::string (its base)
	// Java lets you keep calling String methods on a concatenation result, e.g.
	// `("" + name).trim()`. trim() drops leading/trailing chars <= ' ' (Java semantics).
	JavaString trim() const {
		std::string::size_type a = 0, b = _s.size();
		while (a < b && (unsigned char)_s[a] <= ' ') ++a;
		while (b > a && (unsigned char)_s[b - 1] <= ' ') --b;
		return JavaString(_s.substr(a, b - a));
	}
	int length() const { return (int)_s.size(); }
};

// java.lang.StringBuilder / StringBuffer. Beta's decompiler emits explicit builders
// (`StringBuilder v = new StringBuilder(); v.append(a); v.append(b); ... v.toString()`) where
// Alpha used `+` concat. Every append() returns `this` (StringBuilder*) so both the inline
// chain `(new StringBuilder())->append(a)->append(b)->toString()` and the multi-statement form
// compile; the converter registers these return types (typemap.RUNTIME_MEMBER_ENV) so `.`->`->`
// resolves. Backed by std::string; append streams any operand via ostringstream (like JCat).
struct StringBuilder {
	std::string _s;
	StringBuilder() {}
	StringBuilder(int /*capacity*/) {}
	StringBuilder(const JavaString& s) : _s(s) {}
	template<class T> StringBuilder* append(const T& v) { std::ostringstream o; o << v; _s += o.str(); return this; }
	StringBuilder* append(double v) { _s += j2xNumberText(v, false); return this; }
	StringBuilder* append(float v) { _s += j2xNumberText(v, true); return this; }
	StringBuilder* append(bool v) { _s += (v ? "true" : "false"); return this; }   // Java prints true/false
	StringBuilder* append(char v) { _s += v; return this; }
	StringBuilder* append(const char* v) { if (v) _s += v; return this; }
	StringBuilder* append(const std::string& v) { _s += v; return this; }
	JavaString toString() const { return JavaString(_s); }
	int length() const { return (int)_s.size(); }
	char charAt(int i) const { return (i >= 0 && i < (int)_s.size()) ? _s[i] : '\0'; }
	StringBuilder* deleteCharAt(int i) { if (i >= 0 && i < (int)_s.size()) _s.erase(i, 1); return this; }
	StringBuilder* delete_(int a, int b) { if (a >= 0 && b <= (int)_s.size() && a <= b) _s.erase(a, b - a); return this; }  // delete(start,end)
	StringBuilder* insert(int i, const std::string& v) { if (i >= 0 && i <= (int)_s.size()) _s.insert(i, v); return this; }
	StringBuilder* reverse() { for (int a = 0, b = (int)_s.size() - 1; a < b; ++a, --b) { char t = _s[a]; _s[a] = _s[b]; _s[b] = t; } return this; }
	void setLength(int n) { _s.resize(n >= 0 ? n : 0); }
	operator JavaString() const { return JavaString(_s); }   // also reaches std::string (its base)
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

// --- Java instanceof is IS-A, not exact-type. C++ typeid gives the EXACT dynamic type, so a plain
// `*ti == typeid(T)` makes `x instanceof Block` FALSE for every Block SUBCLASS (BlockOreStorage, ...) --
// which silently broke every recipe/entity/GUI instanceof. We can't dynamic_cast from the type-erased
// void*, so the converter registers each class's direct base(s)/interface(s) (j2xRegisterBase, emitted
// into j2x_initAll's RTTI prelude) and is<T>() walks that ancestry. Keyed by type_info::name() so it is
// stable across translation units. ---
inline std::map<std::string, std::vector<std::string> >& _j2xBases() {
	static std::map<std::string, std::vector<std::string> > m; return m;
}
inline void j2xRegisterBase(const char* cls, const char* base) { _j2xBases()[cls].push_back(base); }
inline bool _j2xIsA(const std::string& obj, const std::string& tgt) {
	if (obj == tgt) return true;
	std::map<std::string, std::vector<std::string> >::iterator it = _j2xBases().find(obj);
	if (it == _j2xBases().end()) return false;
	for (std::size_t k = 0; k < it->second.size(); ++k)
		if (_j2xIsA(it->second[k], tgt)) return true;
	return false;
}

// Boxed java.lang.Object element, ONLY for `Object[]` (Object[] -> JArray<JObject>). Java's mixed
// `new Object[]{"str", Character.valueOf('c'), block, item}` autoboxes primitives; C++ void* can't
// hold a char/String AND be RTTI-decoded, so JObject is a tagged box. Autoboxes from char/int/
// String/any pointer; converts back via casts; `instanceof Character/String/...` -> isXxx(). Scalar
// `Object` stays void* (opaque refs don't need this).
struct JObject {
	int kind;                         // 0 null, 1 int, 2 char, 4 String, 5 ptr, 6 String[], 7 bool, 8 real
	long long i;
	double d;                         // for kind==8: the boxed float/double value (Float/Double)
	JavaString s;
	JArray<JavaString> sa;
	void* p;
	const std::type_info* ti;         // for kind==5: the boxed pointer's static type (for is<T>())
	JObject() : kind(0), i(0), d(0), p(0), ti(0) {}
	JObject(int v) : kind(v ? 1 : 0), i(v), d(0), p(0), ti(0) {}        // 0 == null (NULL is 0); else boxed int
	JObject(long long v) : kind(v ? 1 : 0), i(v), d(0), p(0), ti(0) {}  // boxed long
	JObject(long v) : kind(v ? 1 : 0), i(v), d(0), p(0), ti(0) {}
	JObject(unsigned int v) : kind(v ? 1 : 0), i(v), d(0), p(0), ti(0) {}
	JObject(unsigned long v) : kind(v ? 1 : 0), i(v), d(0), p(0), ti(0) {}
	JObject(unsigned long long v) : kind(v ? 1 : 0), i((long long)v), d(0), p(0), ti(0) {}   // size_t on 64-bit
	JObject(char v) : kind(2), i(v), d(0), p(0), ti(0) {}
	JObject(unsigned char v) : kind(2), i(v), d(0), p(0), ti(0) {}   // a Java char (boxed Character), not an int
	JObject(bool v) : kind(7), i(v ? 1 : 0), d(0), p(0), ti(0) {}
	JObject(float v) : kind(8), i((long long)v), d(v), p(0), ti(0) {}   // boxed Float
	JObject(double v) : kind(8), i((long long)v), d(v), p(0), ti(0) {}  // boxed Double
	JObject(const char* v) : kind(4), i(0), d(0), s(v), p(0), ti(0) {}
	JObject(const JavaString& v) : kind(4), i(0), d(0), s(v), p(0), ti(0) {}
	// box a string-concat builder as its String (a varargs `Object...` element is often a `"x" + n`
	// concat -> JCat; without this it needs two user conversions to reach JObject and silently degrades
	// to the built-in comma operator in the JArrayInit chain -- GuiConnectFailed(..., "..." + host)).
	JObject(const JCat& v) : kind(4), i(0), d(0), s(v._s), p(0), ti(0) {}   // _s: JCat converts to both JavaString and std::string
	JObject(const JArray<JavaString>& v) : kind(6), i(0), d(0), sa(v), p(0), ti(0) {}
	// box a primitive/reference array (`Object o = intArray`, `map.put(k, int[])`) as a heap copy,
	// RTTI-tagged so `(int[])o` unboxes it. Non-String arrays only (String[] uses the kind-6 ctor above).
	template<class T> JObject(const JArray<T>& v) : kind(5), i(0), d(0), p(new JArray<T>(v)), ti(&typeid(JArray<T>)) {}
	// box a value-container Map the same way (`Object o = map` / a generics bridge returning Map as
	// Object, argo's JsonNodeSelectors). `(Map)o` unboxes via the operator below.
	template<class K, class V> JObject(const JMap<K, V>& m) : kind(5), i(0), d(0), p(new JMap<K, V>(m)), ti(&typeid(JMap<K, V>)) {}
	// box a value-container List too (`Object o = list`, argo's element/array node selectors). `(List)o`
	// unboxes via the operator below.
	template<class T> JObject(const JList<T>& v) : kind(5), i(0), d(0), p(new JList<T>(v)), ti(&typeid(JList<T>)) {}
	// A raw `void*` (opaque handle / a JList<void*> element): can't `typeid(*v)` a void*, so tag it with
	// typeid(void). Non-template -> beats the T* template below for void* args.
	JObject(void* v) : kind(v ? 5 : 0), i(0), d(0), p(v), ti(v ? &typeid(void) : 0) {}
	// Box a typed pointer with its DYNAMIC type (typeid(*v), not typeid(T)) so `o.is<Leaf>()` -- the
	// lowering of `o instanceof Leaf` -- matches the runtime class even when the static box type was a
	// base (a raw List<Entity> element tested `instanceof EntityPainting`). Guarded: typeid(*v) needs v != 0.
	template<class T> JObject(T* v) : kind(v ? 5 : 0), i(0), d(0), p((void*)v), ti(v ? &typeid(*v) : 0) {}
	// Java instanceof:
	bool isCharacter() const { return kind == 2; }
	bool isString() const { return kind == 4; }
	bool isInteger() const { return kind == 1; }
	bool isBoolean() const { return kind == 7; }
	bool isStringArray() const { return kind == 6; }
	// instanceof <Class>: exact dynamic-type match OR is-a via the converter-registered ancestry.
	template<class T> bool is() const {
		return kind == 5 && ti && (*ti == typeid(T) || _j2xIsA(ti->name(), typeid(T).name()));
	}
	// named extractors (Java unbox: ((Integer)o).intValue(), ((Character)o).charValue(), ...). Integral
	// kinds read `i`; Float/Double read `d`. Each round-trips the primitive it was boxed from.
	char charValue() const { return (char)i; }
	signed char byteValue() const { return (signed char)i; }
	short shortValue() const { return (short)i; }
	int intValue() const { return (int)i; }
	long long longValue() const { return i; }
	float floatValue() const { return kind == 8 ? (float)d : (float)i; }
	double doubleValue() const { return kind == 8 ? d : (double)i; }
	bool booleanValue() const { return i != 0; }
	JavaString str() const { return s; }
	JArray<JavaString> strArray() const { return sa; }
	template<class T> T* as() const { return (T*)p; }
	// Object.equals(...): compare by boxed value / pointer identity (Java Object.equals default).
	bool equals(const JObject& o) const { return kind == o.kind && i == o.i && p == o.p && (kind != 4 || s == o.s); }
	bool equals(const JavaString& o) const { return kind == 4 && s == o; }
	template<class T> bool equals(T* q) const { return kind == 5 && p == (void*)q; }
	// null compare (Object x == null -> x == 0, since NULL is 0). operator==(int) is an exact
	// match for the int 0, so it wins over the operator int() user-conversion path -> no ambiguity
	// (verified on g++ -std=c++03: == 0 with both present compiles without warning).
	bool operator==(int v) const { return v == 0 && (kind == 0 || (kind == 5 && p == 0)); }
	bool operator!=(int v) const { return !(*this == v); }
	// value equality (Object.equals default) -- lets JList<JObject>::indexOf/contains/remove(value)
	// (std::find) work now that raw lists hold JObject (boxed primitives + pointers), like raw maps.
	bool operator==(const JObject& o) const { return equals(o); }
	bool operator!=(const JObject& o) const { return !equals(o); }
	// map-key order. J2X-BUGFIX jobject-string-key: MUST branch on kind and order String keys by their
	// VALUE (s). The old `p < o.p || (p==o.p && i<o.i)` ignored `s`, and every boxed String has p==0 &&
	// i==0 -> ALL string keys compared EQUAL -> collapsed into ONE std::map bucket. That silently
	// corrupted every String-keyed JMap: RenderEngine.textureMap (getTexture returned a wrong/stale id
	// -> tiled-mojang menu background + white buttons + Lock2DSurface overflow when an FX bound a small
	// texture's id), StringTranslate (i18n), NBTTagCompound tag maps (save/load). std::map orders by
	// operator< ONLY, so a correct equals() alone is moot.
	bool operator<(const JObject& o) const {
		if (kind != o.kind) return kind < o.kind;
		if (kind == 4) return s < o.s;             // String / boxed char array: order by value
		if (p != o.p) return p < o.p;              // ptr identity (kind 5) / null
		return i < o.i;                            // int/char/bool/real: boxed scalar
	}
	// conversion operators used by converter-emitted casts: (int)x, (char)x, (String)x, (Block*)x,
	// void* interop. NOTE: no operator bool() -- it would make `if(x)` ambiguous with operator int().
	operator int() const { return (int)i; }
	operator char() const { return (char)i; }
	operator unsigned char() const { return (unsigned char)i; }   // a Java char (unsigned): MSVC found int/char ambiguous
	template<class T> operator T*() const { return (T*)p; }
};

// unbox a String: `(String)o`, `String s = o` -> the boxed string (empty if o didn't box one).
inline JavaString::JavaString(const JObject& o) : std::string(o.s) {}
// unbox a boxed array: `(int[])o` -> the stored JArray (empty if o didn't box a JArray<T>).
template<class T> JArray<T>::JArray(const JObject& o) : _buf(0), length(0) {
	if (o.kind == 5 && o.p && o.ti && *o.ti == typeid(JArray<T>)) *this = *(const JArray<T>*)o.p;
}
// unbox a boxed Map: `(Map)o` -> the stored JMap (empty if o didn't box a JMap<K,V>).
template<class K, class V> JMap<K, V>::JMap(const JObject& o) {
	if (o.kind == 5 && o.p && o.ti && *o.ti == typeid(JMap<K, V>)) *this = *(const JMap<K, V>*)o.p;
}
// unbox a boxed List: `(List)o` -> the stored JList (empty if o didn't box a JList<T>).
template<class T> JList<T>::JList(const JObject& o) {
	if (o.kind == 5 && o.p && o.ti && *o.ti == typeid(JList<T>)) *this = *(const JList<T>*)o.p;
}

// java.util.Iterator DEFINITION (forward-declared in JIterator.h). Elements are JObject so a
// JList<JObject> iterates as JObject, not a void* that drops the box tag; next() returns a JObject that
// converts back to the element via JObject's operators. Elements are COPIED in (source may mutate
// mid-iteration; Java would throw ConcurrentModificationException). Defined here, after JObject, because
// JObject depends on the containers that include JIterator.h.
struct Iterator {
	std::vector<JObject> _v; unsigned _i;
	Iterator() : _i(0) {}
	bool hasNext() const { return _i < _v.size(); }
	JObject next() { return _i < _v.size() ? _v[_i++] : JObject(); }
	void remove() {}                                     // no-op (source already snapshotted)
};
// list/set .iterator()  (each element boxed into a JObject)
template<class C> inline Iterator* j2xIter(const C& c) {
	Iterator* it = new Iterator();
	for (typename C::const_iterator i = c.begin(); i != c.end(); ++i) it->_v.push_back(JObject(*i));
	return it;
}
// map.values().iterator()  /  map.keySet().iterator()
template<class M> inline Iterator* j2xValues(const M& m) {
	Iterator* it = new Iterator();
	for (typename M::const_iterator i = m.begin(); i != m.end(); ++i) it->_v.push_back(JObject(i->second));
	return it;
}
template<class M> inline Iterator* j2xKeys(const M& m) {
	Iterator* it = new Iterator();
	for (typename M::const_iterator i = m.begin(); i != m.end(); ++i) it->_v.push_back(JObject(i->first));
	return it;
}

// Java reference-identity `this == o` inside equals(Object o), where o is a nullable Object param -> a
// JObject (value or pointer) boxing the argument. j2xIdentEq compares the receiver pointer to the boxed
// pointer (true iff o boxes `this`) -- exactly Java's `==`. A free operator== can't do this (both sides
// are pointers, and an overloaded operator needs a class-type parameter -> C2803), so the converter
// rewrites `this == <Object var>` to this call. (argo's J_JsonArray/J_JsonObject/... equals shortcut.)
template<class T> inline bool j2xIdentEq(const T* a, const JObject& b) { return b.kind == 5 && b.p == (const void*)a; }
template<class T> inline bool j2xIdentEq(const T* a, const JObject* b) { return b != 0 && b->kind == 5 && b->p == (const void*)a; }

// Stream a JObject (Java String-concatenation of an Object calls its toString()). Without this the
// implicit conversions (operator int / operator T*) make `ostream << JObject` AMBIGUOUS, which breaks
// `JCat() + someObject`. We print the boxed primitive/string; other kinds get a stable placeholder.
inline std::ostream& operator<<(std::ostream& o, const JObject& j) {
	switch (j.kind) {
		case 1: return o << j.i;
		case 2: return o << (char)j.i;
		case 4: return o << j.s;
		case 7: return o << (j.i ? "true" : "false");
		case 8: return o << j2xNumberText(j.d, false);
		case 0: return o << "null";
		default: return o << "[object]";
	}
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
	inline std::string toString(int v, int) { std::ostringstream o; o << v; return o.str(); }  // radix ignored (stub)
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
	inline bool isInfinite(double v) { return v != 0.0 && v * 2.0 == v; }   // only +/-inf: x*2==x (NaN fails, 0 excluded)
	inline std::string toString(double v) { return j2xNumberText(v, false); }
}
// java.lang.String STATIC methods (String.valueOf / String.format). The String TYPE maps to
// JavaString; this namespace holds only the statics, accessed as String::valueOf(...) (no clash).
namespace String {
	template<class T> inline JavaString valueOf(const T& v) { std::ostringstream o; o << v; return JavaString(o.str()); }
	inline JavaString valueOf(double v) { return JavaString(j2xNumberText(v, false)); }
	inline JavaString valueOf(float v) { return JavaString(j2xNumberText(v, true)); }
	template<class A> inline JavaString valueOf(const A& a, int, int) { std::ostringstream o; o << a; return JavaString(o.str()); }  // String.valueOf(char[], off, len) (stub)
	inline JavaString valueOf(const JArray<unsigned char>& a, int off, int n) {   // String.valueOf(char[], off, len)
		std::string s; for (int i = 0; i < n; ++i) s += (char)a[off + i]; return JavaString(s);
	}
	inline JavaString valueOf(const JArray<unsigned char>& a) { return valueOf(a, 0, a.length); }   // String.valueOf(char[])
	inline JavaString valueOf(unsigned char c) { return JavaString(std::string(1, (char)c)); }   // String.valueOf(char)
	inline JavaString valueOf(bool v) { return JavaString(v ? "true" : "false"); }
	inline JavaString valueOf(const char* v) { return JavaString(v ? v : ""); }
	inline JavaString valueOf(const std::string& v) { return JavaString(v); }
	inline JavaString format(const JavaString& fmt) { return fmt; }             // printf-style not ported
	template<class A> inline JavaString format(const JavaString& fmt, const A&) { return fmt; }
	template<class A, class B> inline JavaString format(const JavaString& fmt, const A&, const B&) { return fmt; }
}
namespace Float {
	const float MAX_VALUE = 3.402823466e+38f;   // FLT_MAX (3.4028235e38f rounds ABOVE it -> MSVC C2177)
	const float MIN_VALUE = 1.4e-45f;
	const float POSITIVE_INFINITY = 3.4e38f;
	const float NEGATIVE_INFINITY = -3.4e38f;
	inline float parseFloat(const std::string& s) { return _javart_parse<float>(s); }
	inline float valueOf(float v) { return v; }
	inline std::string toString(float v) { return j2xNumberText(v, true); }
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
	template<class A> inline std::string toString(const A&) { return std::string(); }   // debug only
	template<class A> inline JList<void* > asList(const A&) { return JList<void* >(); }  // stub: empty (elements not recoverable)
}

#endif // JAVART_H
