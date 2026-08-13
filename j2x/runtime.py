# -*- coding: utf-8 -*-
"""
runtime.py -- support files the converter emits alongside the translated classes.

These stand in for Java runtime types that aren't part of the decompiled corpus
(java.util.Random, ...). They are emitted into the output by the pipeline and
#included by any class that references them (see typemap.RUNTIME_HEADERS).
"""

import re

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
// java.lang.Thread -- SELF-CONTAINED STUB for the converter's syntax gate (out, no platform
// headers). start() is a no-op here so output compiles standalone. The REAL build uses the platform
// version docs/platform/reference/Thread.reference.h, which spawns REAL Xbox 360 threads (CreateThread) and a
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
	void join(...) {}                                    // join() / join(millis) -- no-op (start() is a no-op)
	static void dumpStack() {}
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
// A `class` (not struct) so it matches the emitter's `class Comparator;` forward decls (no C4099).
class Comparator {
public:
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
#include "JavaString.h"
struct FilenameFilter {
	virtual ~FilenameFilter() {}
	// Non-pure + JavaString param: the corpus overrides `accept(File*, const JavaString&)`; a
	// `const std::string&` pure signature wouldn't be overridden by it (different param type), leaving
	// the subclass abstract. Default accepts everything.
	virtual bool accept(File*, const JavaString&) { return true; }
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
#include <sstream>
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
	JavaString substring(int a) const { return JavaString(std::string::substr((size_type)a)); }
	JavaString substring(int a, int b) const { return JavaString(std::string::substr((size_type)a, (size_type)(b - a))); }
	// substr() shadows std::string::substr so a Java `.substring()` lowered to `.substr()` still yields a
	// JavaString -- otherwise a std::string result kills a chained `.trim()`/`.length()` (Java String API).
	JavaString substr(size_type a) const { return JavaString(std::string::substr(a)); }
	JavaString substr(size_type a, size_type n) const { return JavaString(std::string::substr(a, n)); }
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
	// String.replace(CharSequence, CharSequence): replace every LITERAL occurrence of `from` with `to`.
	JavaString replace(const std::string& from, const std::string& to) const {
		if (from.empty()) return *this;
		std::string r(*this); size_type p = 0;
		while ((p = r.find(from, p)) != npos) { r.replace(p, from.size(), to); p += to.size(); }
		return JavaString(r);
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
	// Java string concatenation `s + <primitive>` (the JCat lowering can't reach every site, e.g. inside a
	// ternary). String-first only, and numeric-only -- no std::string/char*/JavaString overloads, so the
	// existing free operator+ still resolves string+string with no ambiguity (numbers can't convert to it).
	JavaString operator+(int v) const { std::ostringstream o; o << v; return JavaString((const std::string&)*this + o.str()); }
	JavaString operator+(long v) const { std::ostringstream o; o << v; return JavaString((const std::string&)*this + o.str()); }
	JavaString operator+(long long v) const { std::ostringstream o; o << v; return JavaString((const std::string&)*this + o.str()); }
	JavaString operator+(unsigned v) const { std::ostringstream o; o << v; return JavaString((const std::string&)*this + o.str()); }
	JavaString operator+(double v) const { std::ostringstream o; o << v; return JavaString((const std::string&)*this + o.str()); }
	JavaString operator+(char v) const { return JavaString((const std::string&)*this + std::string(1, v)); }
	// NOTE: deliberately NO operator+(bool)/(const char*)/(std::string) -- a `const char*` literal converts
	// to bool, which would make `s + "literal"` ambiguous against the free std::operator+(string,const char*).
	// String concatenation with a bool is handled by the JCat lowering (which has its own bool overload).
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

JITERATOR_H = r'''#ifndef JITERATOR_H
#define JITERATOR_H

// java.util.Iterator over any container. Its elements are JObject, so iterating a JList<JObject>
// yields the boxed value (a plain void* would drop the kind/int/String tag): next() returns a JObject
// that converts back to the element -- a pointer via operator T*, a boxed primitive via
// .intValue()/operator int, a String via operator JavaString -- exactly as Java casts Iterator.next().
//
// Only FORWARD-DECLARED here: Iterator stores JObject, but JObject (JavaRT.h) is built on JList/JMap,
// which include THIS header -- a cycle. So the full definition + j2xIter/j2xValues/j2xKeys live in
// JavaRT.h AFTER JObject. JList/JSet/JMap only need `Iterator*` (a pointer -> forward decl suffices) and
// call j2xIter in inline bodies that are instantiated at their .cpp use sites, where JObject is complete.
struct JObject;
struct Iterator;
template<class C> Iterator* j2xIter(const C& c);
template<class M> Iterator* j2xValues(const M& m);
template<class M> Iterator* j2xKeys(const M& m);
#endif // JITERATOR_H
'''

JLIST_H = r'''#ifndef JLIST_H
#define JLIST_H
#include <vector>
#include <algorithm>
#include <set>
#include "JIterator.h"

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
	// element-cast copy: `return map.values();` builds a JList<void*> from a JList<JObject> (the raw
	// Collection return type erases the element), and `new ArrayList(otherTypedList)` copies across
	// element types. Same-type copy uses the compiler's copy ctor (an exact match beats this template).
	template<class U> JList(const JList<U>& o) {
		this->reserve(o.size());
		for (typename JList<U>::const_iterator i = o.begin(); i != o.end(); ++i) this->push_back((T)*i);
	}
	// `new ArrayList(someSet)` -- copy a Set (std::set-derived JSet) into a List, element-cast per slot.
	template<class U> JList(const std::set<U>& s) {
		for (typename std::set<U>::const_iterator i = s.begin(); i != s.end(); ++i) this->push_back((T)*i);
	}
	Iterator* iterator() const { return j2xIter(*this); }   // .iterator() on a chained/foreign receiver
	void add(const T& v) { this->push_back(v); }
	void add(int i, const T& v) { this->insert(this->begin() + i, v); }
	template<class C> void addAll(const C& o) { this->insert(this->end(), o.begin(), o.end()); }  // any container (vector/set/list)
	template<class C> bool removeAll(const C& o) {       // remove every element present in o
		bool changed = false;
		for (typename C::const_iterator k = o.begin(); k != o.end(); ++k) {
			_it it = std::find(this->begin(), this->end(), *k);
			if (it != this->end()) { this->erase(it); changed = true; }
		}
		return changed;
	}
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
	// List.remove(int index) with a non-`int` integer index (unsigned/long from a Java int expr). These
	// NON-template overloads beat the by-value template below for integer args, so an index remove never
	// boxes the index into T (JObject(unsigned) is ambiguous) or mistakes it for a remove-by-value.
	T remove(unsigned i) { return this->remove((int)i); }
	T remove(long i) { return this->remove((int)i); }
	T remove(long long i) { return this->remove((int)i); }
	template<class U> bool remove(const U& v) {          // List.remove(Object) -> was-present
		// box U to the element type first: on a JList<JObject>, `std::find(.., EntityPlayer*)` would
		// compare `JObject == EntityPlayer*`, which is ambiguous (JObject has both operator==(JObject)
		// and an implicit EntityPlayer*<->JObject conversion). T(v) forces an element==element compare.
		T needle = T(v);
		_it it = std::find(this->begin(), this->end(), needle);
		if (it == this->end()) return false;
		this->erase(it); return true;
	}
	int hashCode() const { int h = 1; for (_cit i = this->begin(); i != this->end(); ++i) h = 31 * h; return h; }
	template<class C> bool equals(const C& o) const { return this->size() == o.size() && std::equal(this->begin(), this->end(), o.begin()); }
	// Java `list != null` / `== null`: a by-value list is never null (it always exists).
	bool operator==(long long) const { return false; }
	bool operator!=(long long) const { return true; }
};
// Java String-concatenation of a List calls AbstractCollection.toString() -> "[a, b, c]". Provide a
// stream op so `JCat() + list` compiles (used only in exception-message paths on the port).
#include <ostream>
template<class T> inline std::ostream& operator<<(std::ostream& o, const JList<T>& v) {
	o << '[';
	for (typename std::vector<T>::size_type i = 0; i < v.size(); ++i) { if (i) o << ", "; o << v[i]; }
	return o << ']';
}
#endif // JLIST_H
'''

# HashSet / TreeSet -> JSet<T>: a std::set subclass with the Java Set API (add/addAll/removeAll/
# contains/isEmpty/first + collection-copy ctor). Mirrors JList so set method calls compile without
# per-receiver type inference. Ordered (std::set) -> TreeSet's sorted iteration is preserved for free.
JSET_H = r'''#ifndef JSET_H
#define JSET_H
#include <set>
#include "JIterator.h"
template<class T> class JSet : public std::set<T> {
public:
	typedef typename std::set<T>::const_iterator _cit;
	JSet() {}
	JSet(long long) {}                                   // `Set x = null;` -> empty set
	template<class C> JSet(const C& o) { this->insert(o.begin(), o.end()); }   // new TreeSet(collection)
	// templated add/contains/remove cast the arg to the element type T: a RAW `Set<Integer>` becomes
	// JSet<void*>, so `add(intValue)` must C-cast int->void* (stored consistently; membership still works).
	template<class U> bool add(const U& v) { return this->insert((T)v).second; }
	template<class C> bool addAll(const C& o) { std::size_t n = this->size(); this->insert(o.begin(), o.end()); return this->size() != n; }
	template<class C> bool removeAll(const C& o) { bool ch = false; for (typename C::const_iterator k = o.begin(); k != o.end(); ++k) { if (this->erase(*k)) ch = true; } return ch; }
	template<class U> bool remove(const U& v) { return this->erase((T)v) != 0; }
	template<class U> bool contains(const U& v) const { return this->find((T)v) != this->end(); }
	bool isEmpty() const { return this->empty(); }
	T first() const { return *this->begin(); }
	Iterator* iterator() const { return j2xIter(*this); }   // .iterator() on a chained/temporary receiver
	bool operator==(long long) const { return false; }
	bool operator!=(long long) const { return true; }
};
#endif // JSET_H
'''

# HashMap / TreeMap / LinkedHashMap / Map -> JMap<K,V>: a std::map subclass with the Java Map API
# (put/get/containsKey/remove/isEmpty + keySet/values/entrySet views + null-compare). Mirrors JList/
# JSet so map method calls compile WITHOUT per-receiver type inference -- essential on chained/foreign
# receivers (map.entrySet().iterator().next(), a method call whose result is a map). std::map is the
# base, so every rewrite the emitter already does (put->[k]=v, get->[k], containsKey->count,
# remove->erase, isEmpty->empty, clear/size inherited) keeps working; the members catch the calls the
# emitter leaves intact. Views return by-value JList copies so they iterate, feed a JSet/JList copy-ctor
# (new HashSet(map.keySet())), and expose .iterator(). Order is key-sorted (TreeMap-faithful).
JMAP_H = r'''#ifndef JMAP_H
#define JMAP_H
#include <map>
#include <utility>
#include "JIterator.h"
#include "JList.h"
#include "Entry.h"
template<class K, class V>
class JMap : public std::map<K, V> {
public:
	typedef typename std::map<K, V>::iterator _it;
	typedef typename std::map<K, V>::const_iterator _cit;
	JMap() {}
	JMap(long long) {}                                   // Java `Map x = null;` -> empty map
	V put(const K& k, const V& v) {                      // Java returns the PREVIOUS value (or null)
		_it it = this->find(k);
		if (it == this->end()) { this->insert(std::make_pair(k, v)); return V(); }
		V old = it->second; it->second = v; return old;
	}
	V get(const K& k) const {                            // no insert (unlike operator[]); null if absent
		_cit it = this->find(k);
		return it == this->end() ? V() : it->second;
	}
	bool containsKey(const K& k) const { return this->find(k) != this->end(); }
	V remove(const K& k) {                               // Java returns the removed value (or null)
		_it it = this->find(k);
		if (it == this->end()) return V();
		V old = it->second; this->erase(it); return old;
	}
	bool isEmpty() const { return this->empty(); }
	// view methods -> by-value JList copies: begin/end (feed addAll / JSet(...) / JList(...)) + iterator().
	JList<V> values() const {
		JList<V> r; for (_cit i = this->begin(); i != this->end(); ++i) r.push_back(i->second); return r;
	}
	JList<K> keySet() const {
		JList<K> r; for (_cit i = this->begin(); i != this->end(); ++i) r.push_back(i->first); return r;
	}
	JList<Entry*> entrySet() const {                     // Entry.getKey()/getValue() return void* (cast at use)
		JList<Entry*> r;
		for (_cit i = this->begin(); i != this->end(); ++i)
			r.push_back(new Entry((void*)i->first, (void*)i->second));
		return r;
	}
	bool operator==(long long) const { return false; }   // a by-value map is never null
	bool operator!=(long long) const { return true; }
	// java.util.AbstractMap.equals/hashCode (JSON node objects compare their backing map)
	bool equals(const JMap<K, V>& o) const { return (const std::map<K, V>&)*this == (const std::map<K, V>&)o; }
	template<class U> bool equals(const U&) const { return false; }   // non-map argument
	int hashCode() const { int h = 0; for (typename std::map<K, V>::const_iterator i = this->begin(); i != this->end(); ++i) h += (int)(size_t)&i->second; return h; }
};
#endif // JMAP_H
'''

JAVART_H = r'''#ifndef JAVART_H
#define JAVART_H
// j2x runtime prelude -- shims for Java stdlib statics. #included by every .cpp.
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

// java.lang.Math.random(): pseudo-random double in [0, 1)
inline double javaMathRandom() { return (double)rand() / ((double)RAND_MAX + 1.0); }
// java.lang.Math.signum(x)
template<class T> inline T javaMathSignum(T v) { return v > 0 ? (T)1 : (v < 0 ? (T)-1 : (T)0); }
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
	JCat& operator+(bool v) { _s += (v ? "true" : "false"); return *this; }   // Java prints true/false
	JCat& operator+(const char* v) { if (v) _s += v; return *this; }
	JCat& operator+(const std::string& v) { _s += v; return *this; }
	operator JavaString() const { return JavaString(_s); }
	operator std::string() const { return _s; }
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
	JObject(char v) : kind(2), i(v), d(0), p(0), ti(0) {}
	JObject(bool v) : kind(7), i(v ? 1 : 0), d(0), p(0), ti(0) {}
	JObject(float v) : kind(8), i((long long)v), d(v), p(0), ti(0) {}   // boxed Float
	JObject(double v) : kind(8), i((long long)v), d(v), p(0), ti(0) {}  // boxed Double
	JObject(const char* v) : kind(4), i(0), d(0), s(v), p(0), ti(0) {}
	JObject(const JavaString& v) : kind(4), i(0), d(0), s(v), p(0), ti(0) {}
	// box a string-concat builder as its String (a varargs `Object...` element is often a `"x" + n`
	// concat -> JCat; without this it needs two user conversions to reach JObject and silently degrades
	// to the built-in comma operator in the JArrayInit chain -- GuiConnectFailed(..., "..." + host)).
	JObject(const JCat& v) : kind(4), i(0), d(0), s(v), p(0), ti(0) {}
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
	template<class T> bool is() const { return kind == 5 && ti && *ti == typeid(T); }   // instanceof <Class>
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
	bool operator<(const JObject& o) const { return p < o.p || (p == o.p && i < o.i); }   // map-key order
	// conversion operators used by converter-emitted casts: (int)x, (char)x, (String)x, (Block*)x,
	// void* interop. NOTE: no operator bool() -- it would make `if(x)` ambiguous with operator int().
	operator int() const { return (int)i; }
	operator char() const { return (char)i; }
	operator JavaString() const { return s; }
	template<class T> operator T*() const { return (T*)p; }
	// unbox a boxed array: `(int[])o` -> the stored JArray (empty if o didn't box a JArray<T>).
	template<class T> operator JArray<T>() const {
		return (kind == 5 && p && ti && *ti == typeid(JArray<T>)) ? *(JArray<T>*)p : JArray<T>();
	}
	// unbox a boxed Map: `(Map)o` -> the stored JMap (empty if o didn't box a JMap<K,V>).
	template<class K, class V> operator JMap<K, V>() const {
		return (kind == 5 && p && ti && *ti == typeid(JMap<K, V>)) ? *(JMap<K, V>*)p : JMap<K, V>();
	}
	// unbox a boxed List: `(List)o` -> the stored JList (empty if o didn't box a JList<T>).
	template<class T> operator JList<T>() const {
		return (kind == 5 && p && ti && *ti == typeid(JList<T>)) ? *(JList<T>*)p : JList<T>();
	}
};

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
		case 8: return o << j.d;
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
	inline std::string toString(double v) { std::ostringstream o; o << v; return o.str(); }
}
// java.lang.String STATIC methods (String.valueOf / String.format). The String TYPE maps to
// JavaString; this namespace holds only the statics, accessed as String::valueOf(...) (no clash).
namespace String {
	template<class T> inline JavaString valueOf(const T& v) { std::ostringstream o; o << v; return JavaString(o.str()); }
	template<class A> inline JavaString valueOf(const A& a, int, int) { std::ostringstream o; o << a; return JavaString(o.str()); }  // String.valueOf(char[], off, len) (stub)
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
'''

GL11_H = r'''#ifndef GL11_H
#define GL11_H
// LWJGL org.lwjgl.opengl.GL11 stub -- symbols only, so render code COMPILES; the real
// pipeline is the platform layer (Xbox 360 = Direct3D 9). Values are real GL 1.1 enums.

namespace GL11 {
	const int GL_POINTS = 0x0000;
	const int GL_TRIANGLES = 0x0004;
	const int GL_ZERO = 0x0000;
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
	const int GL_GEQUAL = 0x0206;
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
	const int GL_BYTE = 0x1400;
	const int GL_FLOAT = 0x1406;
	const int GL_TEXTURE = 0x1702;
	const int GL_NORMAL_ARRAY = 0x8075;
	const int GL_TEXTURE_MAG_FILTER = 0x2800;
	const int GL_TEXTURE_MIN_FILTER = 0x2801;
	const int GL_TEXTURE_WRAP_S = 0x2802;
	const int GL_TEXTURE_WRAP_T = 0x2803;
	const int GL_NEAREST_MIPMAP_LINEAR = 0x2702;
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
	const int GL_UNPACK_ALIGNMENT = 0x0CF5;
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
	inline void glNormalPointer(...) {}
	inline void glOrtho(...) {}
	inline void glPixelStorei(...) {}
	inline void glPopMatrix(...) {}
	inline void glPushMatrix(...) {}
	inline void glReadPixels(...) {}
	inline void glRenderList(...) {}
	inline void glRotatef(...) {}
	inline void glScaled(...) {}
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
    'NVFogDistance': 'org.lwjgl.opengl',
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
    # every stub method carries a greppable J2X-STUB marker so a scan (`grep -rn J2X-STUB`) enumerates
    # the ENTIRE placeholder surface -- a no-op that turns out to be load-bearing (the glPolygonOffset
    # z-fight class) is never left silently undone.
    for f in void_fns:
        L.append('\tinline void %s(...) {}   // J2X-STUB[%s]: no-op placeholder -- verify not load-bearing' % (f, name))
    for f, (rt, rv) in ret_fns:
        L.append('\tinline %s %s(...) { return %s; }   // J2X-STUB[%s]: returns default -- verify' % (rt, f, rv, name))
    L += ['}', '#endif // %s' % g, '']
    return '\n'.join(L)


# Return-type registry for the emitted stub classes, so expression-type inference (the `.` vs `->`
# chain resolver in emit/statements.py) can type a chain THROUGH a stub method result -- e.g.
# `Runtime::getRuntime()->maxMemory()`, `getDocumentBase()->getHost()`, `var->getParentFile()->mkdirs()`.
# Populated as each stub is built: {class: {'parent': base_or_None, 'methods': {method: cpp_ret_type}}}.
# Last definition wins (matches _GENERATED overwrite for classes redefined, e.g. File). The pipeline
# converts the C++ return spellings to JTypes and merges this into the member environment.
STUB_MEMBERS = {}

# `Type* method(` / `static Type* method(` inside an `extra` line (static factories & pointer getters
# defined verbatim rather than via ret_methods, e.g. Runtime::getRuntime, Toolkit::getDefaultToolkit).
_EXTRA_PTR_METHOD = re.compile(r'(?:static\s+)?([A-Za-z_]\w*)\s*\*\s*(\w+)\s*\(')


def _class_stub(name, void_methods=(), ret_methods=(), ctors=True, bases=(),
                includes=(), fwds=(), extra=()):
    """A class with variadic stub methods (e.g. SoundSystem, DataInput) -- accessed var->X.
    `extra` = verbatim extra member lines (e.g. conversion operators)."""
    rec = {'parent': bases[0] if bases else None, 'methods': {}}
    for _m, (_rt, _rv) in ret_methods:
        rec['methods'][_m] = _rt
    for _line in extra:
        _pm = _EXTRA_PTR_METHOD.search(_line)
        if _pm and _pm.group(1) != 'void':
            rec['methods'][_pm.group(2)] = _pm.group(1) + '*'
    STUB_MEMBERS[name] = rec                          # last def wins (mirrors _GENERATED overwrite)
    g = _guard(name)
    L = ['#ifndef %s' % g, '#define %s' % g] + _ext_banner(name) + ['#include <string>', '#include "JavaString.h"']
    L += ['#include "%s.h"' % h for h in includes]
    L += ['class %s;' % f for f in fwds]
    decl = 'class %s' % name + ((' : ' + ', '.join('public %s' % b for b in bases)) if bases else '')
    L += ['', decl + ' {', 'public:']
    if ctors:
        L.append('\t%s(...) {}' % name)
    L.append('\tvirtual ~%s() {}' % name)
    # J2X-STUB marker on every stub method -> `grep -rn J2X-STUB` enumerates the full placeholder surface.
    for m in void_methods:
        L.append('\tvirtual void %s(...) {}   // J2X-STUB[%s]: no-op placeholder -- verify not load-bearing' % (m, name))
    for m, (rt, rv) in ret_methods:
        # A method that returns a Java String must return JavaString, not std::string -- else the
        # caller's `.startsWith()/.substring()/.trim()` (Java String methods) fail to compile.
        if rt == 'std::string':
            rt, rv = 'JavaString', 'JavaString()'
        L.append('\tvirtual %s %s(...) { return %s; }   // J2X-STUB[%s]: returns default -- verify' % (rt, m, rv, name))
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
          '\t%s* order(...) { return this; }' % name,           # ByteBuffer.order(ByteOrder) -> self (ptr; matches STUB_MEMBERS + chains into a ByteBuffer* local)
          '\tstatic %s* allocateDirect(...) { return new %s(); }' % (name, name),   # ByteBuffer.allocateDirect(n)
          '\tstatic %s* allocate(...) { return new %s(); }' % (name, name),
          '\toperator void*() { return 0; }', '};', '#endif // %s' % g, '']
    return '\n'.join(L)


_GENERATED = {}
# common java.awt Component/Container surface, shared by Canvas/Panel/Component/Applet stubs
_AWT_VOID = ['setFocusable', 'setSize', 'repaint', 'requestFocus', 'setVisible', 'addNotify',
             'removeNotify', 'setPreferredSize', 'setMinimumSize', 'setMaximumSize', 'enableInputMethods',
             'setBackground', 'setForeground', 'validate', 'setLayout', 'add', 'remove', 'removeAll',
             'addKeyListener', 'addMouseListener', 'addMouseMotionListener', 'addMouseWheelListener',
             'addFocusListener', 'addComponentListener', 'addWindowListener', 'setFocusTraversalKeysEnabled',
             'setCursor', 'transferFocus', 'paint', 'update', 'doLayout',
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
# GL_NV_fog_distance extension (EntityRenderer picks radial fog when GL_NV_fog_distance is present).
_GENERATED['NVFogDistance.h'] = _ns_stub('NVFogDistance',
    consts=[('GL_FOG_DISTANCE_MODE_NV', 0x855A), ('GL_EYE_RADIAL_NV', 0x855B),
            ('GL_EYE_PLANE_ABSOLUTE_NV', 0x855C)])
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
# java.util.regex (Beta: chunk-filename matching in ChunkFilePattern/ChunkFolderPattern). The 360
# has no regex engine and file-based chunk save is stubbed, so these are compile-only stubs: match
# calls return defaults. Replace with hand C++ string parsing if region-file loading is ever wired.
_GENERATED['Matcher.h'] = _class_stub('Matcher', includes=['JavaString'],
    ret_methods=[('matches', ('bool', 'false')), ('find', ('bool', 'false')),
                 ('lookingAt', ('bool', 'false')), ('group', ('JavaString', 'JavaString()')),
                 ('groupCount', ('int', '0')), ('start', ('int', '0')), ('end', ('int', '0'))])
_GENERATED['Pattern.h'] = _class_stub('Pattern', includes=['Matcher'],
    ret_methods=[('matcher', ('Matcher*', 'new Matcher()'))],
    extra=['static Pattern* compile(...) { return new Pattern(); }'])
# java.io.FileFilter (Beta ChunkFolderPattern extends it) -- like FilenameFilter.
_GENERATED['FileFilter.h'] = _class_stub('FileFilter', fwds=['File'],
    ret_methods=[('accept', ('bool', 'false'))])
# java.io.ByteArrayOutputStream / InputStreamReader (Beta texture-pack + NBT IO).
_GENERATED['ByteArrayOutputStream.h'] = _class_stub('ByteArrayOutputStream', bases=['OutputStream'],
    includes=['OutputStream', 'JArray'],
    void_methods=['write', 'reset', 'close', 'flush'],
    ret_methods=[('toByteArray', ('JArray<unsigned char >', 'JArray<unsigned char >()')), ('size', ('int', '0'))],
    # Java's protected fields; subclasses (e.g. RegionFileChunkBuffer) read `this.buf`/`this.count` directly.
    extra=['JArray<unsigned char> buf;', 'int count;'])
# java.io.Reader base (StringReader/InputStreamReader/BufferedReader all extend it; a parser param typed
# `Reader*` must accept a StringReader*).
_GENERATED['Reader.h'] = _class_stub('Reader', void_methods=['close'], ret_methods=[('read', ('int', '-1'))])
_GENERATED['InputStreamReader.h'] = _class_stub('InputStreamReader', bases=['Reader'], includes=['Reader'],
    fwds=['InputStream'], void_methods=['close'], ret_methods=[('read', ('int', '-1'))])
# java.net.HttpURLConnection (Beta ThreadDownloadImage / ThreadCheckHasPaid). Dead on the 360.
_GENERATED['HttpURLConnection.h'] = _class_stub('HttpURLConnection', bases=['URLConnection'],
    includes=['URLConnection'], fwds=['InputStream'],
    void_methods=['setDoInput', 'setDoOutput', 'connect', 'disconnect', 'setRequestMethod', 'setRequestProperty'],
    ret_methods=[('getResponseCode', ('int', '-1')), ('getInputStream', ('InputStream*', '0'))])
# java.util.zip.ZipEntry.
_GENERATED['ZipEntry.h'] = _class_stub('ZipEntry',
    ret_methods=[('getName', ('std::string', 'std::string()')), ('getSize', ('long long', '0')),
                 ('isDirectory', ('bool', 'false'))])
# org.w3c.dom / javax.xml.parsers (Beta ThreadDownloadResources texture-pack XML). Dead on the 360;
# newInstance/parse are flagged J2X-REVIEW[reflection-unsupported] at the call sites.
_GENERATED['NodeList.h'] = _class_stub('NodeList', fwds=['Node'],
    ret_methods=[('getLength', ('int', '0')), ('item', ('Node*', '0'))])
_GENERATED['Node.h'] = _class_stub('Node', fwds=['NodeList'],
    ret_methods=[('getNodeType', ('int', '0')), ('getNodeName', ('std::string', 'std::string()')),
                 ('getNodeValue', ('std::string', 'std::string()')), ('getChildNodes', ('NodeList*', '0'))])
_GENERATED['Element.h'] = _class_stub('Element', bases=['Node'], includes=['Node'], fwds=['NodeList'],
    ret_methods=[('getAttribute', ('std::string', 'std::string()')), ('getElementsByTagName', ('NodeList*', '0'))])
_GENERATED['Document.h'] = _class_stub('Document', fwds=['NodeList', 'Element'],
    ret_methods=[('getElementsByTagName', ('NodeList*', '0')), ('getDocumentElement', ('Element*', '0'))])
_GENERATED['DocumentBuilder.h'] = _class_stub('DocumentBuilder', fwds=['Document'],
    ret_methods=[('parse', ('Document*', '0'))])
_GENERATED['DocumentBuilderFactory.h'] = _class_stub('DocumentBuilderFactory', fwds=['DocumentBuilder'],
    ret_methods=[('newDocumentBuilder', ('DocumentBuilder*', '0'))],
    extra=['static DocumentBuilderFactory* newInstance(...) { return new DocumentBuilderFactory(); }'])
# java.util.Date / java.text.SimpleDateFormat (Beta ScreenShotHelper timestamps).
_GENERATED['Date.h'] = _class_stub('Date',
    ret_methods=[('getTime', ('long long', '0LL')), ('toString', ('std::string', 'std::string()'))])
_GENERATED['SimpleDateFormat.h'] = _class_stub('SimpleDateFormat', bases=['DateFormat'], includes=['DateFormat'],
    ret_methods=[('format', ('std::string', 'std::string()'))])
# java.util.zip gzip streams (Beta SaveConverterMcRegion / NBT). Real gzip lives in the hand-written
# Inflate/CompressedStreamTools platform code; these keep the type references compiling.
_GENERATED['GZIPInputStream.h'] = _class_stub('GZIPInputStream', bases=['InputStream'], includes=['InputStream'])
_GENERATED['GZIPOutputStream.h'] = _class_stub('GZIPOutputStream', bases=['OutputStream'], includes=['OutputStream'],
    void_methods=['finish'])
# java.lang.Runtime (Beta memory queries) + java.nio.ByteOrder.
_GENERATED['Runtime.h'] = _class_stub('Runtime',
    void_methods=['gc', 'runFinalization'],
    ret_methods=[('freeMemory', ('long long', '0LL')), ('totalMemory', ('long long', '0LL')),
                 ('maxMemory', ('long long', '0LL'))],
    extra=['static Runtime* getRuntime() { static Runtime r; return &r; }'])
_GENERATED['ByteOrder.h'] = _ns_stub('ByteOrder', consts=[('BIG_ENDIAN', '1'), ('LITTLE_ENDIAN', '0')],
                                     ret_fns=[('nativeOrder', ('int', '1'))])
# java.io.RandomAccessFile (Beta RegionFile chunk store). Real IO is the hand-written platform layer.
_GENERATED['RandomAccessFile.h'] = _class_stub('RandomAccessFile',
    void_methods=['seek', 'write', 'writeInt', 'writeByte', 'writeShort', 'writeLong', 'writeFully',
                  'setLength', 'close', 'readFully'],
    ret_methods=[('read', ('int', '-1')), ('readInt', ('int', '0')), ('readByte', ('int', '0')),
                 ('readShort', ('int', '0')), ('readUnsignedByte', ('int', '0')), ('readLong', ('long long', '0LL')),
                 ('length', ('long long', '0LL')), ('getFilePointer', ('long long', '0LL')),
                 ('skipBytes', ('int', '0'))])
# java.io.Writer / StringWriter / PushbackReader. Writer.append returns the Writer (java.lang.Appendable)
# so `w.append(a).append(b)` chains (argo's compact formatter).
_GENERATED['Writer.h'] = _class_stub('Writer', void_methods=['write', 'flush', 'close'],
    extra=['Writer* append(...) { return this; }'])
_GENERATED['StringWriter.h'] = _class_stub('StringWriter', bases=['Writer'], includes=['Writer'],
    ret_methods=[('toString', ('std::string', 'std::string()')), ('getBuffer', ('void*', '0'))])
_GENERATED['PushbackReader.h'] = _class_stub('PushbackReader', void_methods=['unread', 'close'],
    ret_methods=[('read', ('int', '-1'))])
# java.util.Stack / Properties / Map.Entry / NumberFormat.
_GENERATED['Stack.h'] = (
    '#ifndef STACK_H\n#define STACK_H\n#include <vector>\n'
    'class Stack {\n'                                     # java.util.Stack<T> -> void*-element stub (a pointer class)
    '\tstd::vector<void*> _v;\n'
    'public:\n'
    '\tvoid* push(void* v) { _v.push_back(v); return v; }\n'
    '\tvoid* pop() { if (_v.empty()) return 0; void* v = _v.back(); _v.pop_back(); return v; }\n'
    '\tvoid* peek() const { return _v.empty() ? 0 : _v.back(); }\n'
    '\tbool isEmpty() const { return _v.empty(); }\n'
    '\tint size() const { return (int)_v.size(); }\n'
    '\tvoid* elementAt(int i) const { return (i >= 0 && i < (int)_v.size()) ? _v[i] : 0; }\n'
    '};\n#endif\n')
_GENERATED['Properties.h'] = _class_stub('Properties',
    void_methods=['load', 'store', 'setProperty', 'put'],
    ret_methods=[('getProperty', ('std::string', 'std::string()'))])
# java.util.Map.Entry -- a REAL pair box (JMap.entrySet() heap-allocates one per mapping). getKey()/
# getValue() return the stored void* (the boxed key/value pointer), cast at the call site exactly as
# Java casts Map.Entry.getKey()/getValue() from Object. The no-arg ctor keeps the old stub call sites
# compiling; setValue() mutates the local copy only (entrySet() is a snapshot here, like Iterator).
_GENERATED['Entry.h'] = (
    '#ifndef ENTRY_H\n#define ENTRY_H\n'
    'class Entry {\n'
    '\tvoid* _k; void* _v;\n'
    'public:\n'
    '\tEntry() : _k(0), _v(0) {}\n'
    '\tEntry(void* k, void* v) : _k(k), _v(v) {}\n'
    '\tvoid* getKey() const { return _k; }\n'
    '\tvoid* getValue() const { return _v; }\n'
    '\tvoid* setValue(void* v) { void* o = _v; _v = v; return o; }\n'
    '};\n#endif // ENTRY_H\n')
_GENERATED['NumberFormat.h'] = _class_stub('NumberFormat',
    void_methods=['setMaximumFractionDigits', 'setMinimumFractionDigits'],
    ret_methods=[('format', ('std::string', 'std::string()'))],
    extra=['static NumberFormat* getInstance(...) { return new NumberFormat(); }',
           'static NumberFormat* getIntegerInstance(...) { return new NumberFormat(); }'])
# java.security.MessageDigest (Beta MD5). Real hashing is MCHash/MD5String platform code.
_GENERATED['MessageDigest.h'] = _class_stub('MessageDigest', includes=['JArray'],
    void_methods=['update', 'reset'],
    ret_methods=[('digest', ('JArray<unsigned char >', 'JArray<unsigned char >()'))],
    extra=['static MessageDigest* getInstance(...) { return new MessageDigest(); }'])
# AWT / misc leaf stubs (dead on the 360).
_GENERATED['RenderingHints.h'] = _class_stub('RenderingHints', void_methods=['put'],
    extra=['static const int s_KEY_INTERPOLATION = 0;',   # opaque hint keys/values (dead: no AWT on the 360)
           'static const int s_VALUE_INTERPOLATION_NEAREST_NEIGHBOR = 0;',
           'static const int s_KEY_ANTIALIASING = 0;', 'static const int s_VALUE_ANTIALIAS_OFF = 0;'])
_GENERATED['CodecWav.h'] = _class_stub('CodecWav')   # paulscode WAV codec (dead type stub on the console)
_GENERATED['DataFlavor.h'] = _class_stub('DataFlavor')
_GENERATED['Reference.h'] = _class_stub('Reference', ret_methods=[('get', ('void*', '0'))])
_GENERATED['Logger.h'] = _class_stub('Logger', void_methods=['log', 'info', 'warning', 'severe', 'fine'],
    extra=['static Logger* getLogger(...) { return new Logger(); }'])
# java.util.Calendar / GregorianCalendar (Beta seasonal-chest / timestamp checks).
_GENERATED['Calendar.h'] = _class_stub('Calendar', void_methods=['set', 'setTime', 'add'],
    ret_methods=[('get', ('int', '0')), ('getTime', ('void*', '0')), ('getTimeInMillis', ('long long', '0LL'))],
    extra=['static Calendar* getInstance() { return new Calendar(); }',
           'static const int JANUARY = 0, FEBRUARY = 1, DECEMBER = 11, DATE = 5, MONTH = 2, YEAR = 1;'])
_GENERATED['GregorianCalendar.h'] = _class_stub('GregorianCalendar', bases=['Calendar'], includes=['Calendar'])
# AWT / paulscode leaf stubs (all dead on the 360 -- desktop clipboard / config).
_GENERATED['Clipboard.h'] = _class_stub('Clipboard', fwds=['Transferable'],
    ret_methods=[('getContents', ('Transferable*', '0'))])
_GENERATED['Toolkit.h'] = _class_stub('Toolkit', includes=['Clipboard'],
    extra=['static Toolkit* getDefaultToolkit() { return new Toolkit(); }',
           'Clipboard getSystemClipboard(...) { return Clipboard(); }'])   # value receiver -> .getContents()
_GENERATED['TextArea.h'] = _class_stub('TextArea', ret_methods=[('getText', ('std::string', 'std::string()'))])
_GENERATED['Transferable.h'] = _class_stub('Transferable')
_GENERATED['SoundSystemConfig.h'] = _ns_stub('SoundSystemConfig',
    void_fns=['setCodec', 'setNumberStreamingChannels', 'addLibrary', 'removeLibrary', 'setSoundFilesPackage'])
# java.util.Collection (interface base) / Locale / java.awt.Rectangle.
_GENERATED['Collection.h'] = _class_stub('Collection',
    void_methods=['add', 'clear'], ret_methods=[('size', ('int', '0')), ('isEmpty', ('bool', 'true')),
                                                 ('iterator', ('void*', '0')), ('contains', ('bool', 'false'))])
_GENERATED['Locale.h'] = _class_stub('Locale',
    extra=['static const int US = 0, ENGLISH = 0, ROOT = 0;'])   # Locale.US et al -> dead int consts (locale is a no-op on the 360)
# java.nio.charset.Charset -- Charset.forName("UTF-8") is a static factory; returns a stub instance.
_GENERATED['Charset.h'] = _class_stub('Charset',
    extra=['static Charset* forName(...) { return new Charset(); }'])
_GENERATED['Rectangle.h'] = _class_stub('Rectangle', extra=['int x, y, width, height;'])
# Transferable / TextArea leaf methods (dead AWT; keep them compiling). RenderingHints/DataFlavor
# static-constant accesses (KEY_INTERPOLATION / stringFlavor) remain a few errors -- dead AWT, low pri.
_GENERATED['RenderingHints.h'] = _class_stub('RenderingHints', void_methods=['put'],
    extra=['static const int s_KEY_INTERPOLATION = 0;',   # opaque hint keys/values (dead: no AWT on the 360)
           'static const int s_VALUE_INTERPOLATION_NEAREST_NEIGHBOR = 0;',
           'static const int s_KEY_ANTIALIASING = 0;', 'static const int s_VALUE_ANTIALIAS_OFF = 0;'])
_GENERATED['CodecWav.h'] = _class_stub('CodecWav')   # paulscode WAV codec (dead type stub on the console)
_GENERATED['DataFlavor.h'] = _class_stub('DataFlavor',
    extra=['static const int s_stringFlavor = 0;'])   # DataFlavor.stringFlavor -> dead const (passed to varargs stubs)
_GENERATED['Transferable.h'] = _class_stub('Transferable',
    ret_methods=[('isDataFlavorSupported', ('bool', 'false')), ('getTransferData', ('JObject', 'JObject()'))])
_GENERATED['TextArea.h'] = _class_stub('TextArea', void_methods=['setFont', 'setText'],
    ret_methods=[('getText', ('std::string', 'std::string()'))])
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
    ret_methods=[('getEntry', ('ZipEntry*', '0')), ('getInputStream', ('InputStream*', '0')),
                 ('size', ('int', '0'))], fwds=['InputStream', 'ZipEntry'])
_GENERATED['SocketAddress.h'] = _class_stub('SocketAddress')
_GENERATED['Socket.h'] = _class_stub('Socket',
    void_methods=['close', 'setSoTimeout', 'setTrafficClass', 'setTcpNoDelay', 'setKeepAlive', 'connect'],
    ret_methods=[('getInputStream', ('InputStream*', '0')), ('getOutputStream', ('OutputStream*', '0')),
                 ('getRemoteSocketAddress', ('SocketAddress*', '0')), ('getInetAddress', ('InetAddress*', '0')),
                 ('getPort', ('int', '0')), ('isConnected', ('bool', 'false')), ('isClosed', ('bool', 'false'))],
    fwds=['InputStream', 'OutputStream', 'SocketAddress', 'InetAddress'])
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
                  'setListenerPosition', 'setPitch', 'setVolume', 'stop', 'pause', 'removeSource',
                  'backgroundMusic', 'quickPlay', 'quickStream', 'setPosition', 'fadeOut', 'fadeOutIn'],
    ret_methods=[('playing', ('bool', 'false'))])
# small java.io / java.awt / java.net stubs (Applet/Canvas/Component/File/FileDialog/
# BufferedImage are enriched further below)
_GENERATED['BufferedReader.h'] = _class_stub('BufferedReader',
    void_methods=['close'], ret_methods=[('readLine', ('std::string', 'std::string()')),
                                         ('read', ('int', '0')), ('ready', ('bool', 'false'))])
# character streams for options.txt / crash reports (GameSettings, PanelCrashReport): FileReader is
# wrapped by BufferedReader; PrintWriter wraps FileWriter. Real impls are the Xbox file layer
# (docs/platform/reference/FileIO.reference) -- BufferedReader.readLine etc. read nothing until then.
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
#include <vector>
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
	std::string getCanonicalName() const { return getName(); }
	std::string toString() const { return std::string("class ") + getName(); }
	bool equals(const Class& o) const { return *this == o; }
	bool operator==(const Class& o) const { return _ti == o._ti || (_ti && o._ti && *_ti == *o._ti); }
	bool operator!=(const Class& o) const { return !(*this == o); }
	// A Java `.class` literal maps to a STABLE Class* (j2xClassOf, below); `getClass()` stays a Class
	// value. These let `getClass() == Foo.class` (value == Class*) compare by TYPE, not pointer.
	bool operator==(const Class* o) const { return o && *this == *o; }
	bool operator!=(const Class* o) const { return !(o && *this == *o); }
	const std::type_info* rawType() const { return _ti; }
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
	Class getConstructor(...) { return *this; }   // dead reflection: chain `.newInstance()` compiles; returns NULL
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

// A Java `.class` literal must be a STABLE, non-dangling Class object (Java Class objects are
// process-lifetime singletons). j2xClassOf returns a Class* into a program-lifetime registry keyed
// by std::type_info, so the SAME type always yields the SAME pointer -- `EntityList::addMapping(
// EntityPig.class, ...)` stores a valid pointer (NOT the address of a destroyed temporary, the old
// `Class(typeid(X))` dangling bug) and `a.class == b.class` is correct pointer identity.
inline Class* j2xClassOf(const std::type_info& ti) {
	static std::vector<Class*> reg;                        // leaked once per type == program-lifetime
	for (unsigned i = 0; i < reg.size(); ++i) {
		const std::type_info* rt = reg[i]->rawType();
		if (rt && *rt == ti) return reg[i];
	}
	Class* c = new Class(ti); reg.push_back(c); return c;
}
// free cross-comparisons so `Foo.class == obj->getClass()` (Class* == Class value) also works
inline bool operator==(const Class* a, const Class& b) { return a && *a == b; }
inline bool operator!=(const Class* a, const Class& b) { return !(a && *a == b); }
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
                 ('getHost', ('std::string', 'std::string()')), ('getQuery', ('std::string', 'std::string()')),
                 ('getPort', ('int', '-1')), ('getDefaultPort', ('int', '-1')), ('toURI', ('void*', '0'))],
    extra=['std::string _j2xpath;',
           'URL(const std::string& p) : _j2xpath(p) {}',
           'const std::string& j2xResourcePath() const { return _j2xpath; }'])
_GENERATED['FileDialog.h'] = _class_stub('FileDialog', void_methods=['setVisible', 'setMode'],
    ret_methods=[('getFile', ('std::string', 'std::string()')),
                 ('getDirectory', ('std::string', 'std::string()'))])
_GENERATED['DecimalFormat.h'] = _class_stub('DecimalFormat',
    ret_methods=[('format', ('std::string', 'std::string()'))])
_GENERATED['ImageObserver.h'] = _class_stub('ImageObserver')

# --- additional java.* stubs surfaced by the Beta corpus (all dead or shimmed on the console) ---
# java.util.zip: chunk (de)compression. Real GZIP work happens through the GZIP streams elsewhere;
# these are type stubs so RegionFile / Packet51MapChunk / ChunkLoader compile.
_GENERATED['Inflater.h'] = _class_stub('Inflater',
    void_methods=['setInput', 'end', 'reset'],
    ret_methods=[('inflate', ('int', '0')), ('finished', ('bool', 'false')),
                 ('needsInput', ('bool', 'false')), ('getRemaining', ('int', '0')), ('getTotalOut', ('int', '0'))])
_GENERATED['Deflater.h'] = _class_stub('Deflater',
    void_methods=['setInput', 'finish', 'end', 'setLevel', 'reset'],
    ret_methods=[('deflate', ('int', '0')), ('finished', ('bool', 'false')), ('getTotalOut', ('int', '0'))])
_GENERATED['InflaterInputStream.h'] = _class_stub('InflaterInputStream', bases=['InputStream'], includes=['InputStream'])
_GENERATED['DeflaterOutputStream.h'] = _class_stub('DeflaterOutputStream', bases=['OutputStream'], includes=['OutputStream'])
_GENERATED['ByteArrayInputStream.h'] = _class_stub('ByteArrayInputStream', bases=['InputStream'], includes=['InputStream'])
# java.util.zip exceptions
_GENERATED['DataFormatException.h'] = _class_stub('DataFormatException', bases=['Exception'], includes=['Exception'])
_GENERATED['NoSuchAlgorithmException.h'] = _class_stub('NoSuchAlgorithmException', bases=['Exception'], includes=['Exception'])
# java.math / java.security (MD5String checksum -- dead: no auth on the console)
_GENERATED['BigInteger.h'] = _class_stub('BigInteger',
    ret_methods=[('toString', ('std::string', 'std::string()')), ('abs', ('BigInteger*', '0'))])
# java.lang.ref (RegionFileCache soft cache -> just holds the referent)
_GENERATED['SoftReference.h'] = _class_stub('SoftReference', ret_methods=[('get', ('void*', '0'))])
# java.net host lookup (NetClientHandler -- dead on the 360)
_GENERATED['InetAddress.h'] = _class_stub('InetAddress',
    ret_methods=[('getHostName', ('std::string', 'std::string()')), ('getHostAddress', ('std::string', 'std::string()'))],
    extra=['static InetAddress* getByName(...) { return new InetAddress(); }',
           'static InetAddress* getLocalHost(...) { return new InetAddress(); }'])
_GENERATED['StringReader.h'] = _class_stub('StringReader', bases=['InputStreamReader'], includes=['InputStreamReader'])
# java.awt (applet / isometric-preview / crash-report paths -- all dead on the console)
_GENERATED['Font.h'] = _class_stub('Font', ret_methods=[('getSize', ('int', '0')), ('deriveFont', ('Font*', '0'))])
_GENERATED['FontMetrics.h'] = _class_stub('FontMetrics',
    ret_methods=[('stringWidth', ('int', '0')), ('getHeight', ('int', '0')), ('charWidth', ('int', '0'))])
_GENERATED['AffineTransform.h'] = _class_stub('AffineTransform')
_GENERATED['DataBufferInt.h'] = _class_stub('DataBufferInt', ret_methods=[('getData', ('void*', '0'))])
_GENERATED['Graphics2D.h'] = _class_stub('Graphics2D', bases=['Graphics'],
    includes=['Graphics', 'FontMetrics'], fwds=['AffineTransform', 'Rectangle'],
    void_methods=['setClip', 'setRenderingHint', 'translate', 'scale', 'setColor', 'fillRect',
                  'drawImage', 'setTransform', 'drawString', 'dispose', 'rotate', 'shear'],
    ret_methods=[('getTransform', ('AffineTransform*', '0')), ('getClipBounds', ('Rectangle*', '0')),
                 ('getFontMetrics', ('FontMetrics*', '0'))])
_GENERATED['BufferStrategy.h'] = _class_stub('BufferStrategy', includes=['Graphics'], fwds=['Graphics'],
    void_methods=['show'], ret_methods=[('getDrawGraphics', ('Graphics*', '0'))])
# LWJGL/paulscode sound plug referenced by SoundManager.addLibrary(...)
_GENERATED['LibraryLWJGLOpenAL.h'] = _class_stub('LibraryLWJGLOpenAL')

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
_GENERATED['URI.h'] = _class_stub('URI', fwds=['URL'], ret_methods=[('toURL', ('URL*', '0'))])
_GENERATED['File.h'] = _class_stub('File', fwds=['URL'], includes=['JArray', 'URI'],
    ret_methods=[('getName', ('std::string', 'std::string()')), ('getPath', ('std::string', 'std::string()')),
                 ('getAbsolutePath', ('std::string', 'std::string()')), ('toString', ('std::string', 'std::string()')),
                 ('exists', ('bool', 'false')), ('isDirectory', ('bool', 'false')), ('isFile', ('bool', 'false')),
                 ('mkdir', ('bool', 'false')), ('mkdirs', ('bool', 'false')), ('renameTo', ('bool', 'false')),
                 ('length', ('long long', '0LL')), ('getParentFile', ('File*', '0')),
                 ('getAbsoluteFile', ('File*', '0')), ('toURI', ('URI', 'URI()')),   # value -> .toURL() chains
                 ('listFiles', ('JArray<File* >', 'JArray<File* >()')),   # File[] not void*
                 ('lastModified', ('long long', '0LL')), ('list', ('void*', '0')),
                 ('getParent', ('std::string', 'std::string()')), ('canRead', ('bool', 'false')),
                 ('canWrite', ('bool', 'false')), ('delete_', ('bool', 'false')),   # File.delete() (keyword-renamed)
                 ('createNewFile', ('bool', 'false'))])
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
_GENERATED['Canvas.h'] = _class_stub('Canvas', bases=['Component'], includes=['Component'], fwds=['Graphics', 'BufferStrategy'],
    void_methods=_AWT_VOID,
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0')), ('getLocationOnScreen', ('void*', '0')),
                 ('getBufferStrategy', ('BufferStrategy*', '0'))], extra=['void createBufferStrategy(...) {}'])
_GENERATED['Component.h'] = _class_stub('Component', fwds=['Graphics'], void_methods=_AWT_VOID,
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0')), ('getLocationOnScreen', ('void*', '0'))])
_GENERATED['FileDialog.h'] = _class_stub('FileDialog',
    void_methods=['setVisible', 'setMode', 'setDirectory', 'setFile', 'setFilenameFilter',
                  'setLocationRelativeTo'],
    ret_methods=[('getFile', ('std::string', 'std::string()')),
                 ('getDirectory', ('std::string', 'std::string()'))])
# java.awt.image raster chain: BufferedImage.getRaster().getDataBuffer() -> (DataBufferInt*)-> getData()
# (ImageBufferDownload reads the pixel int[] directly). Dead on the 360, but must type-check.
_GENERATED['DataBuffer.h'] = _class_stub('DataBuffer')
_GENERATED['DataBufferInt.h'] = _class_stub('DataBufferInt', bases=['DataBuffer'], includes=['DataBuffer', 'JArray'],
    ret_methods=[('getData', ('JArray<int >', 'JArray<int >()'))])
_GENERATED['Raster.h'] = _class_stub('Raster', fwds=['DataBuffer'],
    ret_methods=[('getDataBuffer', ('DataBuffer*', '0'))])
_GENERATED['WritableRaster.h'] = _class_stub('WritableRaster', bases=['Raster'], includes=['Raster'])
_GENERATED['BufferedImage.h'] = _class_stub('BufferedImage', fwds=['Graphics'], includes=['Raster'],
    void_methods=['setRGB', 'flush'],
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getRGB', ('int', '0')), ('getData', ('void*', '0')),
                 ('getRaster', ('Raster', 'Raster()')),
                 ('getGraphics', ('Graphics*', '0')), ('createGraphics', ('Graphics*', '0'))])


COLLECTIONS_H = r'''#ifndef COLLECTIONS_H
#define COLLECTIONS_H
#include <algorithm>
#include "JavaRT.h"
#include "JList.h"
#include "Comparator.h"
#include <vector>
#include <cstddef>
class Random;                                            // shuffle(list, Random) -- fwd-decl (no RNG dep here)
// java.util.Collections.
//
// sort(List, Comparator): Java's Collections.sort is a STABLE merge sort that tolerates
// comparators which are NOT a strict weak ordering -- e.g. RenderSorter returns compare(a,a) == -1
// (faithful to the Java source). std::sort REQUIRES a strict weak ordering; feeding it such a
// comparator is undefined behaviour and on the Xbox 360 STL reads out of bounds -> heap corruption
// / IP 0x0 hang. It fires exactly at world entry, where the whole dirty-chunk queue (up to a few
// thousand WorldRenderers) is sorted EVERY frame. So we do NOT use std::sort here: a bottom-up
// stable merge sort that only ever asks compare(a,b) < 0 -- and only within a run -- is memory-safe
// for ANY 3-way Java comparator by construction, and matches Java's stable ordering. Comparator::
// compare takes JObject; the list element type converts to JObject implicitly (void* / typed
// pointers / boxed values all do).
namespace _j2x_coll {
	struct _CmpLess {
		Comparator* c;
		_CmpLess(Comparator* cc) : c(cc) {}
		template<class T> bool operator()(const T& a, const T& b) const { return c->compare(a, b) < 0; }
	};
	// Bottom-up STABLE merge sort over contiguous [a, a+n). Uses less(x,y) only as a predicate,
	// only within run bounds -> never reads out of range no matter what compare() returns.
	// Stable: on equal elements the earlier (left-run) element is emitted first.
	template<class T, class Less>
	void _stable_merge_sort(T* a, std::size_t n, Less less) {
		if (n < 2) { return; }
		std::vector<T> tmp(n);
		for (std::size_t width = 1; width < n; width <<= 1) {
			for (std::size_t lo = 0; lo < n; lo += (width << 1)) {
				std::size_t mid = lo + width;          if (mid > n) { mid = n; }
				std::size_t hi  = lo + (width << 1);   if (hi  > n) { hi  = n; }
				std::size_t i = lo, j = mid, k = lo;
				while (i < mid && j < hi) {
					tmp[k++] = less(a[j], a[i]) ? a[j++] : a[i++];   // take right only if strictly less -> stable
				}
				while (i < mid) { tmp[k++] = a[i++]; }
				while (j < hi)  { tmp[k++] = a[j++]; }
			}
			for (std::size_t x = 0; x < n; ++x) { a[x] = tmp[x]; }
		}
	}
}
struct Collections {
	template<class T> static void sort(JList<T>& list, Comparator* cmp) {
		if (list.size() < 2) { return; }
		_j2x_coll::_stable_merge_sort(&list[0], (std::size_t)list.size(), _j2x_coll::_CmpLess(cmp));
	}
	template<class T> static void sort(JList<T>& list) { std::sort(list.begin(), list.end()); }
	template<class T> static void reverse(JList<T>& list) { std::reverse(list.begin(), list.end()); }
	template<class T> static JList<T>& synchronizedList(JList<T>& list) { return list; }   // no JVM monitor on the port
	template<class M> static M& synchronizedMap(M& m) { return m; }
	template<class T> static JList<T> emptyList() { return JList<T>(); }
	template<class T> static void shuffle(JList<T>&) {}            // no-op (no RNG dependency here)
	template<class T> static void shuffle(JList<T>&, Random*) {}
};
#endif // COLLECTIONS_H
'''


# Runtime container/buffer method result-types, so the chain resolver can type a chain THROUGH them
# and pick `.` vs `->` correctly: `x.entrySet().iterator().next()` (iterator() -> Iterator*, so .next()
# -> ->next()) and `ByteBuffer::allocateDirect(n).order(bo).asIntBuffer()`. Merged into the member env.
STUB_MEMBERS['JList'] = {'parent': None, 'methods': {'iterator': 'Iterator*'}}
STUB_MEMBERS['JSet'] = {'parent': None, 'methods': {'iterator': 'Iterator*'}}
STUB_MEMBERS['JMap'] = {'parent': None, 'methods': {'entrySet': 'JList', 'values': 'JList', 'keySet': 'JSet'}}
STUB_MEMBERS['ByteBuffer'] = {'parent': None, 'methods': {
    'allocateDirect': 'ByteBuffer*', 'allocate': 'ByteBuffer*', 'order': 'ByteBuffer*',
    'asIntBuffer': 'IntBuffer*', 'asFloatBuffer': 'FloatBuffer*'}}

# filename -> content
FILES = {
    'Random.h': RANDOM_H,
    'JavaString.h': JAVASTRING_H,
    'JArray.h': JARRAY_H,
    'JIterator.h': JITERATOR_H,
    # java.util.Iterator maps to the `Iterator` struct, which lives in JIterator.h. Code that names the
    # type emits `#include "Iterator.h"` (RUNTIME_HEADERS), so provide it as a thin alias header.
    'Iterator.h': '#ifndef ITERATOR_H\n#define ITERATOR_H\n#include "JIterator.h"\n#endif\n',
    'JList.h': JLIST_H,
    'JSet.h': JSET_H,
    'JMap.h': JMAP_H,
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


def annotate_stub(text, class_name, platform_id):
    """Inject a platform-specific SDK hint comment after the EXTERNAL-LIBRARY STUB banner.
    If the file isn't a stub or no platform is set, returns the text unchanged."""
    if 'EXTERNAL-LIBRARY STUB' not in text or not platform_id:
        return text
    from .stubs import classify_stub
    from .platforms import get_stub_comment
    subsystem_id, _info = classify_stub(class_name)
    comment = get_stub_comment(platform_id, subsystem_id)
    marker = '// =============================================================================='
    return text.replace(marker, marker + '\n' + comment, 1)


_PLATFORM_BANNER_OLD = 'the real behaviour is the Xbox 360 platform layer'
_PLATFORM_BANNER_SDK_OLD = '// (Direct3D 9 / XInput / file IO / sound).'

def generate_files(cpp_std=None, platform_id=None):
    """Dynamically generate the full runtime file set, applying C++ standard
    transforms and platform-specific stub annotations on the fly.

    This is the dynamic replacement for the static FILES dict. Every call
    produces a fresh dict so the same converter session can re-run with
    different settings without stale cached output."""
    from .emit.std_transforms import apply_std_transforms
    from .platforms import PLATFORMS, generate_stdafx
    plat = PLATFORMS.get(platform_id, {}) if platform_id else {}
    plat_label = plat.get('label', 'Xbox 360')
    std_label = cpp_std.label if cpp_std else 'C++03'

    out = {}

    if platform_id:
        out['stdafx.h'] = generate_stdafx(platform_id, std_label)

    is_non_windows = platform_id in ('ps3', 'wii', 'psp', 'nds')

    for fname, content in FILES.items():
        class_name = fname[:-2] if fname.endswith('.h') else fname
        if platform_id and plat_label != 'Xbox 360':
            content = content.replace(_PLATFORM_BANNER_OLD,
                                      'the real behaviour is the %s platform layer' % plat_label)
            subs = plat.get('subsystems', {})
            from .stubs import classify_stub
            sid, _ = classify_stub(class_name)
            sub_info = subs.get(sid)
            if sub_info:
                content = content.replace(_PLATFORM_BANNER_SDK_OLD,
                                          '// (%s / %s).' % (sub_info['sdk'], sub_info['libs']))
        if is_non_windows and fname in ('JavaRT.h', 'Random.h'):
            content = _patch_timer_for_platform(content, platform_id)
        if platform_id:
            content = annotate_stub(content, class_name, platform_id)
        if cpp_std is not None:
            content = apply_std_transforms(content, cpp_std)
        out[fname] = content
    return out


_TIMER_PATCHES = {
    'ps3': {
        'GetTickCount()': '((unsigned long)(sys_time_get_system_time() / 1000))',
        'currentTimeMillis': (
            'inline long long currentTimeMillis() {\n'
            '\t\treturn (long long)(sys_time_get_system_time() / 1000);\n'
            '\t}'
        ),
        'nanoTime': (
            'inline long long nanoTime() {\n'
            '\t\treturn (long long)sys_time_get_system_time() * 1000LL;\n'
            '\t}'
        ),
    },
    'wii': {
        'GetTickCount()': '((unsigned long)(ticks_to_millisecs(gettime())))',
        'currentTimeMillis': (
            'inline long long currentTimeMillis() {\n'
            '\t\treturn (long long)ticks_to_millisecs(gettime());\n'
            '\t}'
        ),
        'nanoTime': (
            'inline long long nanoTime() {\n'
            '\t\treturn (long long)ticks_to_nanosecs(gettime());\n'
            '\t}'
        ),
    },
    'psp': {
        'GetTickCount()': '((unsigned long)(sceKernelGetSystemTimeLow() / 1000))',
        'currentTimeMillis': (
            'inline long long currentTimeMillis() {\n'
            '\t\tSceRtcTick tick; sceRtcGetCurrentTick(&tick);\n'
            '\t\treturn (long long)(tick.tick / 1000);\n'
            '\t}'
        ),
        'nanoTime': (
            'inline long long nanoTime() {\n'
            '\t\tSceRtcTick tick; sceRtcGetCurrentTick(&tick);\n'
            '\t\treturn (long long)(tick.tick * 1000LL);\n'
            '\t}'
        ),
    },
    'nds': {
        'GetTickCount()': '((unsigned long)(cpuGetTiming() / BUS_CLOCK * 1000))',
        'currentTimeMillis': (
            'inline long long currentTimeMillis() {\n'
            '\t\treturn 0LL; // TODO: implement DS timer\n'
            '\t}'
        ),
        'nanoTime': (
            'inline long long nanoTime() {\n'
            '\t\treturn 0LL; // TODO: implement DS timer\n'
            '\t}'
        ),
    },
}

import re as _re

_CURRENT_TIME_RE = _re.compile(
    r'inline long long currentTimeMillis\(\)[^}]*\}')
_NANO_TIME_RE = _re.compile(
    r'inline long long nanoTime\(\)\s*\{[^}]*\}')
_GET_TICK_COUNT = _re.compile(
    r'\(long long\)GetTickCount\(\)')


def _patch_timer_for_platform(content, platform_id):
    """Replace Windows timer calls in JavaRT.h / Random.h with platform equivalents."""
    patch = _TIMER_PATCHES.get(platform_id)
    if not patch:
        return content
    if 'currentTimeMillis' in patch:
        content = _CURRENT_TIME_RE.sub(patch['currentTimeMillis'], content)
    if 'nanoTime' in patch:
        content = _NANO_TIME_RE.sub(patch['nanoTime'], content)
    if 'GetTickCount()' in patch:
        content = _GET_TICK_COUNT.sub(patch['GetTickCount()'], content)
    return content
