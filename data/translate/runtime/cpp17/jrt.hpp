// jrt.hpp -- the Java library in C++17, for whole programs written by codebreaker/translate/backends/cpp17.py.
//
// The object model (docs/TRANSLATOR_DESIGN.md, the C++17 back end):
//   * every Java reference is a jrt::ref<T> (std::shared_ptr): reference counting in place of the collector.
//     A cycle of references is never freed -- the back end lists the classes that can form one, where a
//     collector (Boehm) would be needed to reclaim it;
//   * every class derives (virtually) from jrt::Object; generics are erased, as Java erases them: a type
//     variable is ref<Object>, and a value read back through one is cast to what javac said it is;
//   * a class is made in two phases, as the JVM makes it: allocated with its fields zero, then its Java
//     constructor (ctor_) runs, so a virtual call in a constructor reaches the override (jrt::make);
//   * a Java exception is a C++ exception carrying the Throwable (JavaThrow); a catch tests its type;
//   * strings are UTF-16 (std::u16string), as Java's are; text is written out as UTF-8.
// Only what the translated programs use is here; a library method missing is a compile error, which the
// judge reports as the back end refusing that program.
#ifndef JRT_HPP
#define JRT_HPP
#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <exception>
#include <functional>
#include <initializer_list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace jrt {

template <class T> using ref = std::shared_ptr<T>;
using std::int8_t; using std::int16_t; using std::int32_t; using std::int64_t;

struct String;
struct Object;
struct Throwable;
template <class T> struct Array;
struct IntStream;
ref<String> str(const std::u16string& s);
ref<String> lit(const char16_t* s);
[[noreturn]] void raise(const ref<Throwable>& t);
[[noreturn]] void raise_npe();
template <class T> [[noreturn]] void raise_new(const char16_t* msg);

// ------------------------------------------------------------------------------------------- Object
struct Object : std::enable_shared_from_this<Object> {
	virtual ~Object() {}
	void ctor_() {}
	virtual const char16_t* jclass_() const { return u"java.lang.Object"; }
	virtual ref<String> toString();
	virtual bool equals(ref<Object> o) { return o.get() == this; }
	virtual int32_t hashCode() { return (int32_t)(((uintptr_t)this) >> 4); }
};

template <class T> ref<T> self(T* p) { return std::dynamic_pointer_cast<T>(p->shared_from_this()); }

// allocation in two phases: the object, then its Java constructor
template <class T, class... A> ref<T> make(A&&... a) {
	auto o = std::make_shared<T>();
	o->ctor_(std::forward<A>(a)...);
	return o;
}

struct JavaThrow {
	ref<Throwable> t;
	explicit JavaThrow(ref<Throwable> x) : t(std::move(x)) {}
};

// a checked reference cast: (T) o
template <class T, class U> ref<T> cast(const ref<U>& o);
template <class T, class U> ref<T> as(const ref<U>& o) { return std::dynamic_pointer_cast<T>(o); }
template <class T, class U> bool isa(const ref<U>& o) { return o && std::dynamic_pointer_cast<T>(o) != nullptr; }
template <class T> T* nn(const ref<T>& p) { if (!p) raise_npe(); return p.get(); }

// ------------------------------------------------------------------------------------------- text
inline std::string utf8(const std::u16string& s) {
	std::string out;
	for (size_t i = 0; i < s.size(); ++i) {
		uint32_t c = s[i];
		if (c >= 0xD800 && c < 0xDC00 && i + 1 < s.size() && s[i + 1] >= 0xDC00 && s[i + 1] < 0xE000) {
			c = 0x10000 + ((c - 0xD800) << 10) + (s[i + 1] - 0xDC00);
			++i;
		}
		if (c < 0x80) out += (char)c;
		else if (c < 0x800) { out += (char)(0xC0 | (c >> 6)); out += (char)(0x80 | (c & 63)); }
		else if (c < 0x10000) { out += (char)(0xE0 | (c >> 12)); out += (char)(0x80 | ((c >> 6) & 63)); out += (char)(0x80 | (c & 63)); }
		else { out += (char)(0xF0 | (c >> 18)); out += (char)(0x80 | ((c >> 12) & 63)); out += (char)(0x80 | ((c >> 6) & 63)); out += (char)(0x80 | (c & 63)); }
	}
	return out;
}
inline std::u16string u16(const char* s) { std::u16string r; while (*s) r += (char16_t)(unsigned char)*s++; return r; }

// Java's Double.toString / Float.toString: the shortest digits that read back, laid out as Java does
template <class F> std::u16string ftos(F d) {
	if (d != d) return u"NaN";
	if (d == (F)INFINITY) return u"Infinity";
	if (d == -(F)INFINITY) return u"-Infinity";
	if (d == 0) return std::signbit(d) ? u"-0.0" : u"0.0";
	char buf[64];
	auto r = std::to_chars(buf, buf + sizeof buf, d, std::chars_format::scientific);
	std::string s(buf, r.ptr);
	bool neg = s[0] == '-';
	if (neg) s = s.substr(1);
	size_t e = s.find('e');
	int exp = std::atoi(s.c_str() + e + 1);
	std::string digits;
	for (size_t i = 0; i < e; ++i) if (s[i] != '.') digits += s[i];
	while (digits.size() > 1 && digits.back() == '0') digits.pop_back();
	std::string out;
	F a = neg ? -d : d;
	if (a >= (F)1e-3 && a < (F)1e7) {
		if (exp >= 0) {
			std::string ip = digits.substr(0, std::min<size_t>(digits.size(), exp + 1));
			while ((int)ip.size() < exp + 1) ip += '0';
			std::string fp = (int)digits.size() > exp + 1 ? digits.substr(exp + 1) : "0";
			out = ip + "." + fp;
		} else {
			out = "0." + std::string(-exp - 1, '0') + digits;
		}
	} else {
		out = digits.substr(0, 1) + "." + (digits.size() > 1 ? digits.substr(1) : "0") + "E" + std::to_string(exp);
	}
	return u16(((neg ? "-" : "") + out).c_str());
}

// ------------------------------------------------------------------------------------------- String
struct String : Object {
	std::u16string s;
	String() {}
	explicit String(std::u16string x) : s(std::move(x)) {}
	void ctor_() {}
	void ctor_(const ref<String>& t) { s = nn(t)->s; }
	void ctor_(const ref<Array<char16_t>>& a);
	void ctor_(const ref<Array<char16_t>>& a, int32_t off, int32_t n);
	const char16_t* jclass_() const override { return u"java.lang.String"; }
	ref<String> toString() override { return std::static_pointer_cast<String>(shared_from_this()); }
	bool equals(ref<Object> o) override { auto t = std::dynamic_pointer_cast<String>(o); return t && t->s == s; }
	int32_t hashCode() override { int32_t h = 0; for (char16_t c : s) h = 31 * h + c; return h; }
	int32_t length() { return (int32_t)s.size(); }
	bool isEmpty() { return s.empty(); }
	bool isBlank() { for (char16_t c : s) if (c > ' ') return false; return true; }
	char16_t charAt(int32_t i);
	ref<String> substring(int32_t b);
	ref<String> substring(int32_t b, int32_t e);
	int32_t indexOf(char16_t c) { auto p = s.find(c); return p == std::u16string::npos ? -1 : (int32_t)p; }
	int32_t indexOf(int32_t c) { return indexOf((char16_t)c); }
	int32_t indexOf(const ref<String>& t) { auto p = s.find(nn(t)->s); return p == std::u16string::npos ? -1 : (int32_t)p; }
	int32_t indexOf(const ref<String>& t, int32_t from) { if (from < 0) from = 0; auto p = s.find(nn(t)->s, from); return p == std::u16string::npos ? -1 : (int32_t)p; }
	int32_t indexOf(int32_t c, int32_t from) { if (from < 0) from = 0; auto p = s.find((char16_t)c, from); return p == std::u16string::npos ? -1 : (int32_t)p; }
	int32_t lastIndexOf(int32_t c) { auto p = s.rfind((char16_t)c); return p == std::u16string::npos ? -1 : (int32_t)p; }
	int32_t lastIndexOf(const ref<String>& t) { auto p = s.rfind(nn(t)->s); return p == std::u16string::npos ? -1 : (int32_t)p; }
	bool contains(const ref<Object>& t);
	bool startsWith(const ref<String>& t) { return s.compare(0, nn(t)->s.size(), t->s) == 0 && s.size() >= t->s.size(); }
	bool endsWith(const ref<String>& t) { return s.size() >= nn(t)->s.size() && s.compare(s.size() - t->s.size(), t->s.size(), t->s) == 0; }
	int32_t compareTo(const ref<Object>& o) {
		auto t = std::dynamic_pointer_cast<String>(o);
		if (!t) raise_npe();
		size_t n = std::min(s.size(), t->s.size());
		for (size_t i = 0; i < n; ++i) if (s[i] != t->s[i]) return (int32_t)s[i] - (int32_t)t->s[i];
		return (int32_t)s.size() - (int32_t)t->s.size();
	}
	int32_t compareToIgnoreCase(const ref<String>& t) { return toLowerCase()->compareTo(nn(t)->toLowerCase()); }
	bool equalsIgnoreCase(const ref<String>& t) { return t && toLowerCase()->s == t->toLowerCase()->s; }
	ref<String> toUpperCase() { std::u16string r = s; for (auto& c : r) { if ((c >= 'a' && c <= 'z') || (c >= 0xE0 && c <= 0xFE && c != 0xF7)) c -= 32; } return str(r); }
	ref<String> toLowerCase() { std::u16string r = s; for (auto& c : r) { if ((c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xDE && c != 0xD7)) c += 32; } return str(r); }
	ref<String> trim() { size_t b = 0, e = s.size(); while (b < e && s[b] <= ' ') ++b; while (e > b && s[e - 1] <= ' ') --e; return str(s.substr(b, e - b)); }
	ref<String> strip() { return trim(); }
	ref<String> replace(char16_t a, char16_t b) { std::u16string r = s; for (auto& c : r) if (c == a) c = b; return str(r); }
	ref<String> replace(const ref<Object>& a, const ref<Object>& b);
	ref<String> repeat(int32_t n) { std::u16string r; for (int32_t i = 0; i < n; ++i) r += s; return str(r); }
	ref<String> concat(const ref<String>& t) { return str(s + nn(t)->s); }
	ref<Array<char16_t>> toCharArray();
	ref<Array<ref<String>>> split(const ref<String>& re);
	ref<IntStream> chars();
	ref<String> intern();
	static ref<String> valueOf(int32_t v) { return str(u16(std::to_string(v).c_str())); }
	static ref<String> valueOf(int64_t v) { return str(u16(std::to_string(v).c_str())); }
	static ref<String> valueOf(double v) { return str(ftos(v)); }
	static ref<String> valueOf(float v) { return str(ftos(v)); }
	static ref<String> valueOf(bool v) { return lit(v ? u"true" : u"false"); }
	static ref<String> valueOf(char16_t v) { return str(std::u16string(1, v)); }
	static ref<String> valueOf(const ref<Object>& o) { return o ? o->toString() : lit(u"null"); }
	static ref<String> valueOf(const ref<Array<char16_t>>& a);
	template <class... A> static ref<String> format(const ref<String>& f, A... a);
	static ref<String> join(const ref<Object>& sep, const ref<Object>& parts);
};

inline ref<String> str(const std::u16string& s) { return std::make_shared<String>(s); }
// a literal: interned, as Java interns every literal (two equal literals are the same object)
inline ref<String> lit(const char16_t* s) {
	static std::unordered_map<std::u16string, ref<String>>* pool = new std::unordered_map<std::u16string, ref<String>>();
	auto& slot = (*pool)[s];
	if (!slot) slot = str(s);
	return slot;
}
inline ref<String> String::intern() { return lit(s.c_str()); }
inline ref<String> Object::toString() {
	char buf[32];
	std::snprintf(buf, sizeof buf, "@%x", (unsigned)hashCode());
	return str(std::u16string(jclass_()) + u16(buf));
}

// String.valueOf for each kind of value, as string concatenation applies it
inline std::u16string sv(int32_t v) { return u16(std::to_string(v).c_str()); }
inline std::u16string sv(int64_t v) { return u16(std::to_string(v).c_str()); }
inline std::u16string sv(int16_t v) { return sv((int32_t)v); }
inline std::u16string sv(int8_t v) { return sv((int32_t)v); }
inline std::u16string sv(double v) { return ftos(v); }
inline std::u16string sv(float v) { return ftos(v); }
inline std::u16string sv(bool v) { return v ? u"true" : u"false"; }
inline std::u16string sv(char16_t v) { return std::u16string(1, v); }
inline std::u16string sv(const char16_t* v) { return v; }
template <class T> std::u16string sv(const ref<T>& o) {
	if (!o) return u"null";
	ref<String> t = o->toString();
	return t ? t->s : u"null";
}
template <class... A> ref<String> concat(const A&... a) {
	std::u16string r;
	(r += ... += sv(a));
	return str(r);
}

// ------------------------------------------------------------------------------------------- Throwable
struct Throwable : Object {
	ref<String> message;
	ref<Throwable> cause;
	std::vector<ref<Throwable>> suppressed;
	const char16_t* jclass_() const override { return u"java.lang.Throwable"; }
	void ctor_() {}
	void ctor_(ref<String> m) { message = m; }
	void ctor_(ref<String> m, ref<Throwable> c) { message = m; cause = c; }
	void ctor_(ref<Throwable> c) { cause = c; if (c) message = c->toString(); }
	virtual ref<String> getMessage() { return message; }
	ref<String> getLocalizedMessage() { return getMessage(); }
	ref<Throwable> getCause() { return cause; }
	void addSuppressed(ref<Throwable> t) { suppressed.push_back(t); }
	void printStackTrace();
	ref<String> toString() override {
		ref<String> m = getLocalizedMessage();
		return str(std::u16string(jclass_()) + (m ? u": " + m->s : u""));
	}
};
#define JRT_THROWABLE(Name, Base, QName) \
	struct Name : Base { const char16_t* jclass_() const override { return QName; } };
JRT_THROWABLE(Exception, Throwable, u"java.lang.Exception")
JRT_THROWABLE(Error, Throwable, u"java.lang.Error")
JRT_THROWABLE(RuntimeException, Exception, u"java.lang.RuntimeException")
JRT_THROWABLE(IllegalArgumentException, RuntimeException, u"java.lang.IllegalArgumentException")
JRT_THROWABLE(IllegalStateException, RuntimeException, u"java.lang.IllegalStateException")
JRT_THROWABLE(ArithmeticException, RuntimeException, u"java.lang.ArithmeticException")
JRT_THROWABLE(NullPointerException, RuntimeException, u"java.lang.NullPointerException")
JRT_THROWABLE(ClassCastException, RuntimeException, u"java.lang.ClassCastException")
JRT_THROWABLE(UnsupportedOperationException, RuntimeException, u"java.lang.UnsupportedOperationException")
JRT_THROWABLE(IndexOutOfBoundsException, RuntimeException, u"java.lang.IndexOutOfBoundsException")
JRT_THROWABLE(ArrayIndexOutOfBoundsException, IndexOutOfBoundsException, u"java.lang.ArrayIndexOutOfBoundsException")
JRT_THROWABLE(StringIndexOutOfBoundsException, IndexOutOfBoundsException, u"java.lang.StringIndexOutOfBoundsException")
JRT_THROWABLE(NegativeArraySizeException, RuntimeException, u"java.lang.NegativeArraySizeException")
JRT_THROWABLE(NumberFormatException, IllegalArgumentException, u"java.lang.NumberFormatException")
JRT_THROWABLE(NoSuchElementException, RuntimeException, u"java.util.NoSuchElementException")
JRT_THROWABLE(ConcurrentModificationException, RuntimeException, u"java.util.ConcurrentModificationException")
JRT_THROWABLE(IOException, Exception, u"java.io.IOException")
JRT_THROWABLE(InterruptedException, Exception, u"java.lang.InterruptedException")
JRT_THROWABLE(CloneNotSupportedException, Exception, u"java.lang.CloneNotSupportedException")

inline void raise(const ref<Throwable>& t) {
	if (!t) raise_npe();
	throw JavaThrow(t);
}
template <class T> void raise_new(const char16_t* msg) {
	auto e = std::make_shared<T>();
	if (msg) e->ctor_(lit(msg)); else e->ctor_();
	throw JavaThrow(e);
}
inline void raise_npe() { raise_new<NullPointerException>(nullptr); }
inline void raise_msg(ref<Throwable> e, const std::u16string& m) { e->ctor_(str(m)); throw JavaThrow(e); }

template <class T, class U> ref<T> cast(const ref<U>& o) {
	if (!o) return nullptr;
	auto r = std::dynamic_pointer_cast<T>(o);
	if (!r) raise_msg(std::make_shared<ClassCastException>(), std::u16string(u"class ") + o->jclass_() + u" cannot be cast");
	return r;
}

inline char16_t String::charAt(int32_t i) {
	if (i < 0 || i >= (int32_t)s.size())
		raise_msg(std::make_shared<StringIndexOutOfBoundsException>(), u"index " + sv(i) + u", length " + sv((int32_t)s.size()));
	return s[i];
}
inline ref<String> String::substring(int32_t b) { return substring(b, (int32_t)s.size()); }
inline ref<String> String::substring(int32_t b, int32_t e) {
	if (b < 0 || e > (int32_t)s.size() || b > e)
		raise_msg(std::make_shared<StringIndexOutOfBoundsException>(), u"begin " + sv(b) + u", end " + sv(e) + u", length " + sv((int32_t)s.size()));
	return str(s.substr(b, e - b));
}
inline bool String::contains(const ref<Object>& t) { return s.find(nn(t)->toString()->s) != std::u16string::npos; }
inline ref<String> String::replace(const ref<Object>& a, const ref<Object>& b) {
	std::u16string from = nn(a)->toString()->s, to = nn(b)->toString()->s, r;
	if (from.empty()) { for (char16_t c : s) { r += to; r += c; } return str(r + to); }
	size_t i = 0;
	for (;;) { size_t p = s.find(from, i); if (p == std::u16string::npos) break; r += s.substr(i, p - i); r += to; i = p + from.size(); }
	return str(r + s.substr(i));
}

// ------------------------------------------------------------------------------------------- boxes
#define JRT_BOX(Name, T, QName)                                                                         \
	struct Name : Object {                                                                              \
		T value{};                                                                                      \
		const char16_t* jclass_() const override { return QName; }                                      \
		void ctor_(T v) { value = v; }                                                                  \
		ref<String> toString() override { return str(sv(value)); }                                      \
		bool equals(ref<Object> o) override { auto t = std::dynamic_pointer_cast<Name>(o); return t && same(t->value); } \
		int32_t hashCode() override;                                                                    \
		bool same(T v) const;                                                                           \
		int32_t intValue() { return (int32_t)value; }                                                   \
		int64_t longValue() { return (int64_t)value; }                                                  \
		double doubleValue() { return (double)value; }                                                  \
		float floatValue() { return (float)value; }                                                     \
		int16_t shortValue() { return (int16_t)value; }                                                 \
		int8_t byteValue() { return (int8_t)value; }                                                    \
		int32_t compareTo(const ref<Object>& o) { T b = cast<Name>(o)->value; return value < b ? -1 : (b < value ? 1 : 0); } \
	};
JRT_BOX(Integer, int32_t, u"java.lang.Integer")
JRT_BOX(Long, int64_t, u"java.lang.Long")
JRT_BOX(Short, int16_t, u"java.lang.Short")
JRT_BOX(Byte, int8_t, u"java.lang.Byte")
JRT_BOX(Double, double, u"java.lang.Double")
JRT_BOX(Float, float, u"java.lang.Float")
struct Character : Object {
	char16_t value{};
	const char16_t* jclass_() const override { return u"java.lang.Character"; }
	void ctor_(char16_t v) { value = v; }
	ref<String> toString() override { return str(std::u16string(1, value)); }
	bool equals(ref<Object> o) override { auto t = std::dynamic_pointer_cast<Character>(o); return t && t->value == value; }
	int32_t hashCode() override { return value; }
	char16_t charValue() { return value; }
	int32_t compareTo(const ref<Object>& o) { return (int32_t)value - (int32_t)cast<Character>(o)->value; }
	static bool isDigit(char16_t c) { return c >= '0' && c <= '9'; }
	static bool isLetter(char16_t c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xFF && c != 0xD7 && c != 0xF7); }
	static bool isLetterOrDigit(char16_t c) { return isLetter(c) || isDigit(c); }
	static bool isAlphabetic(int32_t c) { return isLetter((char16_t)c); }
	static bool isWhitespace(char16_t c) { return c == ' ' || (c >= 9 && c <= 13) || (c >= 0x1C && c <= 0x1F); }
	static bool isUpperCase(char16_t c) { return (c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xDE && c != 0xD7); }
	static bool isLowerCase(char16_t c) { return (c >= 'a' && c <= 'z') || (c >= 0xDF && c <= 0xFF && c != 0xF7); }
	static char16_t toUpperCase(char16_t c) { return ((c >= 'a' && c <= 'z') || (c >= 0xE0 && c <= 0xFE && c != 0xF7)) ? c - 32 : c; }
	static char16_t toLowerCase(char16_t c) { return ((c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xDE && c != 0xD7)) ? c + 32 : c; }
	static int32_t getNumericValue(char16_t c) { if (isDigit(c)) return c - '0'; if (c >= 'a' && c <= 'z') return c - 'a' + 10; if (c >= 'A' && c <= 'Z') return c - 'A' + 10; return -1; }
	static int32_t digit(char16_t c, int32_t radix) { int32_t v = getNumericValue(c); return v >= 0 && v < radix ? v : -1; }
	static ref<String> toString(char16_t c) { return str(std::u16string(1, c)); }
	static int32_t hashCode(char16_t c) { return c; }
	static int32_t compare(char16_t a, char16_t b) { return (int32_t)a - (int32_t)b; }
	static ref<Character> valueOf(char16_t c);
};
struct Boolean : Object {
	bool value{};
	const char16_t* jclass_() const override { return u"java.lang.Boolean"; }
	void ctor_(bool v) { value = v; }
	ref<String> toString() override { return lit(value ? u"true" : u"false"); }
	bool equals(ref<Object> o) override { auto t = std::dynamic_pointer_cast<Boolean>(o); return t && t->value == value; }
	int32_t hashCode() override { return value ? 1231 : 1237; }
	bool booleanValue() { return value; }
	static int32_t hashCode(bool b) { return b ? 1231 : 1237; }
	static ref<String> toString(bool b) { return lit(b ? u"true" : u"false"); }
	static bool parseBoolean(const ref<String>& s) { return s && s->toLowerCase()->s == u"true"; }
	static int32_t compare(bool a, bool b) { return a == b ? 0 : (a ? 1 : -1); }
	static ref<Boolean> valueOf(bool b);
};
template <class B, class T> ref<B> make_box(T v) { auto b = std::make_shared<B>(); b->value = v; return b; }
// boxing, with the caches Java keeps (valueOf: -128..127 are the same object every time)
inline ref<Integer> box(int32_t v) {
	static ref<Integer>* cache = nullptr;
	if (v >= -128 && v <= 127) {
		if (!cache) { cache = new ref<Integer>[256]; for (int i = 0; i < 256; ++i) cache[i] = make_box<Integer>(i - 128); }
		return cache[v + 128];
	}
	return make_box<Integer>(v);
}
inline ref<Long> box(int64_t v) {
	static ref<Long>* cache = nullptr;
	if (v >= -128 && v <= 127) {
		if (!cache) { cache = new ref<Long>[256]; for (int i = 0; i < 256; ++i) cache[i] = make_box<Long>((int64_t)i - 128); }
		return cache[v + 128];
	}
	return make_box<Long>(v);
}
inline ref<Short> box(int16_t v) { return make_box<Short>(v); }
inline ref<Byte> box(int8_t v) { return make_box<Byte>(v); }
inline ref<Double> box(double v) { return make_box<Double>(v); }
inline ref<Float> box(float v) { return make_box<Float>(v); }
inline ref<Character> box(char16_t v) {
	static ref<Character>* cache = nullptr;
	if (v <= 127) {
		if (!cache) { cache = new ref<Character>[128]; for (int i = 0; i < 128; ++i) cache[i] = make_box<Character>((char16_t)i); }
		return cache[v];
	}
	return make_box<Character>(v);
}
inline ref<Boolean> box(bool v) {
	static ref<Boolean> t = make_box<Boolean>(true), f = make_box<Boolean>(false);
	return v ? t : f;
}
inline ref<Character> Character::valueOf(char16_t c) { return box(c); }
inline ref<Boolean> Boolean::valueOf(bool b) { return box(b); }
// unboxing: a null box is Java's NullPointerException
template <class T> T unbox(const ref<Object>& o);
template <> inline int32_t unbox<int32_t>(const ref<Object>& o) { if (!o) raise_npe(); if (auto c = as<Character>(o)) return c->value; if (auto b = as<Short>(o)) return b->value; if (auto b = as<Byte>(o)) return b->value; return cast<Integer>(o)->value; }
template <> inline int64_t unbox<int64_t>(const ref<Object>& o) { if (!o) raise_npe(); if (auto i = as<Integer>(o)) return i->value; return cast<Long>(o)->value; }
template <> inline double unbox<double>(const ref<Object>& o) { if (!o) raise_npe(); if (auto i = as<Integer>(o)) return i->value; if (auto l = as<Long>(o)) return (double)l->value; if (auto f = as<Float>(o)) return f->value; return cast<Double>(o)->value; }
template <> inline float unbox<float>(const ref<Object>& o) { if (!o) raise_npe(); if (auto i = as<Integer>(o)) return (float)i->value; return cast<Float>(o)->value; }
template <> inline int16_t unbox<int16_t>(const ref<Object>& o) { if (!o) raise_npe(); return cast<Short>(o)->value; }
template <> inline int8_t unbox<int8_t>(const ref<Object>& o) { if (!o) raise_npe(); return cast<Byte>(o)->value; }
template <> inline char16_t unbox<char16_t>(const ref<Object>& o) { if (!o) raise_npe(); return cast<Character>(o)->value; }
template <> inline bool unbox<bool>(const ref<Object>& o) { if (!o) raise_npe(); return cast<Boolean>(o)->value; }

inline int32_t hash_long(int64_t v) { return (int32_t)(v ^ (int64_t)((uint64_t)v >> 32)); }
inline int64_t dbits(double d) { if (d != d) return 0x7ff8000000000000LL; int64_t b; std::memcpy(&b, &d, 8); return b; }
inline int32_t fbits(float d) { if (d != d) return 0x7fc00000; int32_t b; std::memcpy(&b, &d, 4); return b; }
#define JRT_BOX_SAME(Name, T, H) \
	inline bool Name::same(T v) const { return H(value) == H(v); } \
	inline int32_t Name::hashCode() { return hash_of(value); }
inline int32_t hash_of(int32_t v) { return v; }
inline int32_t hash_of(int64_t v) { return hash_long(v); }
inline int32_t hash_of(int16_t v) { return v; }
inline int32_t hash_of(int8_t v) { return v; }
inline int32_t hash_of(double v) { return hash_long(dbits(v)); }
inline int32_t hash_of(float v) { return fbits(v); }
inline int64_t ident(int64_t v) { return v; }
JRT_BOX_SAME(Integer, int32_t, ident)
JRT_BOX_SAME(Long, int64_t, ident)
JRT_BOX_SAME(Short, int16_t, ident)
JRT_BOX_SAME(Byte, int8_t, ident)
JRT_BOX_SAME(Double, double, dbits)
JRT_BOX_SAME(Float, float, fbits)

// ------------------------------------------------------------------------------------------- numbers
// Java's integer division and remainder: by zero throws, MIN / -1 wraps
template <class T> T div(T a, T b) {
	if (b == 0) raise_new<ArithmeticException>(u"/ by zero");
	if (b == -1) return (T)(0 - (typename std::make_unsigned<T>::type)a);
	return a / b;
}
template <class T> T rem(T a, T b) {
	if (b == 0) raise_new<ArithmeticException>(u"/ by zero");
	if (b == -1) return 0;
	return a % b;
}
inline int32_t d2i(double d) { if (d != d) return 0; if (d >= 2147483647.0) return 2147483647; if (d <= -2147483648.0) return (-2147483647 - 1); return (int32_t)d; }
inline int64_t d2l(double d) { if (d != d) return 0; if (d >= 9223372036854775807.0) return 9223372036854775807LL; if (d <= -9223372036854775808.0) return (-9223372036854775807LL - 1); return (int64_t)d; }
inline int32_t ushr(int32_t a, int32_t n) { return (int32_t)((uint32_t)a >> (n & 31)); }
inline int64_t ushr(int64_t a, int32_t n) { return (int64_t)((uint64_t)a >> (n & 63)); }

inline std::u16string to_radix(uint64_t v, int shift) {
	if (v == 0) return u"0";
	std::u16string r;
	uint64_t mask = (1u << shift) - 1;
	while (v) { r += u"0123456789abcdef"[v & mask]; v >>= shift; }
	std::reverse(r.begin(), r.end());
	return r;
}
inline int64_t parse_long(const ref<String>& s, int radix, int64_t lo, int64_t hi) {
	if (!s) raise_msg(std::make_shared<NumberFormatException>(), u"Cannot parse null string: null");
	const std::u16string& t = s->s;
	size_t i = 0;
	bool neg = false;
	if (!t.empty() && (t[0] == '-' || t[0] == '+')) { neg = t[0] == '-'; i = 1; }
	if (i >= t.size()) raise_msg(std::make_shared<NumberFormatException>(), u"For input string: \"" + t + u"\"");
	__int128 v = 0;
	for (; i < t.size(); ++i) {
		int d = Character::digit(t[i], radix);
		if (d < 0) raise_msg(std::make_shared<NumberFormatException>(), u"For input string: \"" + t + u"\"");
		v = v * radix + d;
		if (v > (__int128)hi + 1) raise_msg(std::make_shared<NumberFormatException>(), u"For input string: \"" + t + u"\"");
	}
	if (neg) v = -v;
	if (v < lo || v > hi) raise_msg(std::make_shared<NumberFormatException>(), u"For input string: \"" + t + u"\"");
	return (int64_t)v;
}
struct IntegerS {
	static constexpr int32_t MAX_VALUE = 2147483647, MIN_VALUE = (-2147483647 - 1);
};
inline int32_t parseInt(const ref<String>& s, int radix = 10) { return (int32_t)parse_long(s, radix, IntegerS::MIN_VALUE, IntegerS::MAX_VALUE); }
struct Integer_ {   // java.lang.Integer's statics (the class itself is the box above)
	static constexpr int32_t MAX_VALUE = 2147483647, MIN_VALUE = (-2147483647 - 1);
	static int32_t parseInt(const ref<String>& s) { return jrt::parseInt(s); }
	static int32_t parseInt(const ref<String>& s, int32_t r) { return jrt::parseInt(s, r); }
	static ref<Integer> valueOf(int32_t v) { return box(v); }
	static ref<Integer> valueOf(const ref<String>& s) { return box(jrt::parseInt(s)); }
	static ref<String> toString(int32_t v) { return str(sv(v)); }
	static ref<String> toString(int32_t v, int32_t radix) { if (radix == 10) return toString(v); bool n = v < 0; uint32_t u = n ? 0u - (uint32_t)v : (uint32_t)v; std::u16string r; if (!u) r = u"0"; while (u) { r += u"0123456789abcdefghijklmnopqrstuvwxyz"[u % radix]; u /= radix; } if (n) r += u'-'; std::reverse(r.begin(), r.end()); return str(r); }
	static ref<String> toHexString(int32_t v) { return str(to_radix((uint32_t)v, 4)); }
	static ref<String> toBinaryString(int32_t v) { return str(to_radix((uint32_t)v, 1)); }
	static ref<String> toOctalString(int32_t v) { return str(to_radix((uint32_t)v, 3)); }
	static int32_t hashCode(int32_t v) { return v; }
	static int32_t compare(int32_t a, int32_t b) { return a < b ? -1 : (a == b ? 0 : 1); }
	static int32_t max(int32_t a, int32_t b) { return a > b ? a : b; }
	static int32_t min(int32_t a, int32_t b) { return a < b ? a : b; }
	static int32_t sum(int32_t a, int32_t b) { return a + b; }
	static int32_t signum(int32_t v) { return (v > 0) - (v < 0); }
	static int32_t abs(int32_t v) { return v < 0 ? -v : v; }
	static int32_t bitCount(int32_t v) { return __builtin_popcount((uint32_t)v); }
	static int32_t reverse(int32_t v) { uint32_t u = (uint32_t)v, r = 0; for (int i = 0; i < 32; ++i) { r = (r << 1) | (u & 1); u >>= 1; } return (int32_t)r; }
	static int32_t numberOfLeadingZeros(int32_t v) { return v == 0 ? 32 : __builtin_clz((uint32_t)v); }
	static int32_t numberOfTrailingZeros(int32_t v) { return v == 0 ? 32 : __builtin_ctz((uint32_t)v); }
	static int32_t highestOneBit(int32_t v) { return v == 0 ? 0 : (int32_t)(0x80000000u >> __builtin_clz((uint32_t)v)); }
	static int32_t rotateLeft(int32_t v, int32_t d) { return (int32_t)(((uint32_t)v << (d & 31)) | ((uint32_t)v >> ((32 - d) & 31))); }
};
struct Long_ {
	static constexpr int64_t MAX_VALUE = 9223372036854775807LL, MIN_VALUE = (-9223372036854775807LL - 1);
	static int64_t parseLong(const ref<String>& s) { return parse_long(s, 10, MIN_VALUE, MAX_VALUE); }
	static ref<Long> valueOf(int64_t v) { return box(v); }
	static ref<String> toString(int64_t v) { return str(sv(v)); }
	static ref<String> toHexString(int64_t v) { return str(to_radix((uint64_t)v, 4)); }
	static ref<String> toBinaryString(int64_t v) { return str(to_radix((uint64_t)v, 1)); }
	static int32_t hashCode(int64_t v) { return hash_long(v); }
	static int32_t compare(int64_t a, int64_t b) { return a < b ? -1 : (a == b ? 0 : 1); }
	static int64_t max(int64_t a, int64_t b) { return a > b ? a : b; }
	static int64_t min(int64_t a, int64_t b) { return a < b ? a : b; }
	static int64_t sum(int64_t a, int64_t b) { return a + b; }
	static int32_t bitCount(int64_t v) { return __builtin_popcountll((uint64_t)v); }
	static int32_t numberOfTrailingZeros(int64_t v) { return v == 0 ? 64 : __builtin_ctzll((uint64_t)v); }
	static int32_t numberOfLeadingZeros(int64_t v) { return v == 0 ? 64 : __builtin_clzll((uint64_t)v); }
};
struct Double_ {
	static constexpr double MAX_VALUE = 1.7976931348623157e308, MIN_VALUE = 4.9e-324;
	static constexpr double POSITIVE_INFINITY = INFINITY, NEGATIVE_INFINITY = -INFINITY, NaN = NAN;
	static ref<Double> valueOf(double v) { return box(v); }
	static ref<String> toString(double v) { return str(ftos(v)); }
	static int32_t hashCode(double v) { return hash_long(dbits(v)); }
	static int32_t compare(double a, double b) { if (a < b) return -1; if (a > b) return 1; int64_t x = dbits(a), y = dbits(b); return x == y ? 0 : (x < y ? -1 : 1); }
	static bool isNaN(double v) { return v != v; }
	static bool isInfinite(double v) { return v == INFINITY || v == -INFINITY; }
	static bool isFinite(double v) { return std::isfinite(v); }
	static int64_t doubleToLongBits(double v) { return dbits(v); }
	static int64_t doubleToRawLongBits(double v) { int64_t b; std::memcpy(&b, &v, 8); return b; }
	static double longBitsToDouble(int64_t b) { double d; std::memcpy(&d, &b, 8); return d; }
	static double parseDouble(const ref<String>& s) { std::string t = utf8(nn(s)->trim()->s); char* e = nullptr; double d = std::strtod(t.c_str(), &e); if (t.empty() || *e) raise_msg(std::make_shared<NumberFormatException>(), u"For input string: \"" + s->s + u"\""); return d; }
	static double sum(double a, double b) { return a + b; }
	static double max(double a, double b) { return std::fmax(a, b); }
	static double min(double a, double b) { return std::fmin(a, b); }
};
struct Float_ {
	static constexpr float MAX_VALUE = 3.4028235e38f, MIN_VALUE = 1.4e-45f;
	static ref<Float> valueOf(float v) { return box(v); }
	static ref<String> toString(float v) { return str(ftos(v)); }
	static int32_t hashCode(float v) { return fbits(v); }
	static int32_t compare(float a, float b) { if (a < b) return -1; if (a > b) return 1; int32_t x = fbits(a), y = fbits(b); return x == y ? 0 : (x < y ? -1 : 1); }
	static bool isNaN(float v) { return v != v; }
	static int32_t floatToIntBits(float v) { return fbits(v); }
	static int32_t floatToRawIntBits(float v) { int32_t b; std::memcpy(&b, &v, 4); return b; }
	static float intBitsToFloat(int32_t b) { float d; std::memcpy(&d, &b, 4); return d; }
	static float parseFloat(const ref<String>& s) { return (float)Double_::parseDouble(s); }
};
struct Short_ { static constexpr int16_t MAX_VALUE = 32767, MIN_VALUE = -32768; static int32_t hashCode(int16_t v) { return v; } static ref<Short> valueOf(int16_t v) { return box(v); } };
struct Byte_ { static constexpr int8_t MAX_VALUE = 127, MIN_VALUE = -128; static int32_t hashCode(int8_t v) { return v; } static ref<Byte> valueOf(int8_t v) { return box(v); } };
struct Character_ {
	static constexpr char16_t MAX_VALUE = 0xFFFF, MIN_VALUE = 0;
};

struct Math {
	static constexpr double PI = 3.141592653589793, E = 2.718281828459045;
	static int32_t abs(int32_t v) { return v < 0 ? (int32_t)(0u - (uint32_t)v) : v; }
	static int64_t abs(int64_t v) { return v < 0 ? (int64_t)(0ull - (uint64_t)v) : v; }
	static double abs(double v) { return std::fabs(v); }
	static float abs(float v) { return std::fabs(v); }
	static int32_t max(int32_t a, int32_t b) { return a > b ? a : b; }
	static int64_t max(int64_t a, int64_t b) { return a > b ? a : b; }
	static double max(double a, double b) { if (a != a) return a; if (a == 0 && b == 0) return std::signbit(a) ? b : a; return a > b ? a : b; }
	static float max(float a, float b) { if (a != a) return a; if (a == 0 && b == 0) return std::signbit(a) ? b : a; return a > b ? a : b; }
	static int32_t min(int32_t a, int32_t b) { return a < b ? a : b; }
	static int64_t min(int64_t a, int64_t b) { return a < b ? a : b; }
	static double min(double a, double b) { if (a != a) return a; if (a == 0 && b == 0) return std::signbit(a) ? a : b; return a < b ? a : b; }
	static float min(float a, float b) { if (a != a) return a; if (a == 0 && b == 0) return std::signbit(a) ? a : b; return a < b ? a : b; }
	static double sqrt(double v) { return std::sqrt(v); }
	static double cbrt(double v) { return std::cbrt(v); }
	static double pow(double a, double b) { return std::pow(a, b); }
	static double exp(double v) { return std::exp(v); }
	static double log(double v) { return std::log(v); }
	static double log10(double v) { return std::log10(v); }
	static double sin(double v) { return std::sin(v); }
	static double cos(double v) { return std::cos(v); }
	static double tan(double v) { return std::tan(v); }
	static double asin(double v) { return std::asin(v); }
	static double acos(double v) { return std::acos(v); }
	static double atan(double v) { return std::atan(v); }
	static double atan2(double a, double b) { return std::atan2(a, b); }
	static double hypot(double a, double b) { return std::hypot(a, b); }
	static double floor(double v) { return std::floor(v); }
	static double ceil(double v) { return std::ceil(v); }
	static double rint(double v) { return std::nearbyint(v); }
	static double signum(double v) { return v > 0 ? 1.0 : (v < 0 ? -1.0 : v); }
	static float signum(float v) { return v > 0 ? 1.0f : (v < 0 ? -1.0f : v); }
	static double toRadians(double v) { return v / 180.0 * PI; }
	static double toDegrees(double v) { return v * 180.0 / PI; }
	static int64_t round(double v) { if (v != v) return 0; return d2l(std::floor(v + 0.5)); }
	static int32_t round(float v) { if (v != v) return 0; return d2i(std::floor((double)v + 0.5)); }
	static int32_t floorDiv(int32_t a, int32_t b) { int32_t q = div(a, b); if ((a % b != 0) && ((a < 0) != (b < 0))) --q; return q; }
	static int32_t floorMod(int32_t a, int32_t b) { int32_t m = rem(a, b); if (m != 0 && ((m < 0) != (b < 0))) m += b; return m; }
	static int64_t floorMod(int64_t a, int64_t b) { int64_t m = rem(a, b); if (m != 0 && ((m < 0) != (b < 0))) m += b; return m; }
	static int32_t addExact(int32_t a, int32_t b) { int32_t r; if (__builtin_add_overflow(a, b, &r)) raise_new<ArithmeticException>(u"integer overflow"); return r; }
	static int32_t multiplyExact(int32_t a, int32_t b) { int32_t r; if (__builtin_mul_overflow(a, b, &r)) raise_new<ArithmeticException>(u"integer overflow"); return r; }
	static int32_t toIntExact(int64_t v) { if (v < IntegerS::MIN_VALUE || v > IntegerS::MAX_VALUE) raise_new<ArithmeticException>(u"integer overflow"); return (int32_t)v; }
};

// ------------------------------------------------------------------------------------------- arrays
template <class T> struct Array : Object {
	std::vector<T> a;
	int32_t length = 0;
	const char16_t* jclass_() const override { return u"[array"; }
	void init(int32_t n) { if (n < 0) raise_msg(std::make_shared<NegativeArraySizeException>(), sv(n)); a.assign(n, T{}); length = n; }
	T& operator[](int32_t i) {
		if ((uint32_t)i >= (uint32_t)length)
			raise_msg(std::make_shared<ArrayIndexOutOfBoundsException>(), u"Index " + sv(i) + u" out of bounds for length " + sv(length));
		return a[i];
	}
	typename std::vector<T>::iterator begin() { return a.begin(); }
	typename std::vector<T>::iterator end() { return a.end(); }
	ref<Array<T>> clone() { auto r = std::make_shared<Array<T>>(); r->a = a; r->length = length; return r; }
};
// for (T x : expr): the range holds the reference, so a temporary array or collection lives through the loop
template <class T> struct Hold {
	ref<T> r;
	auto begin() { return nn(r)->begin(); }
	auto end() { return r->end(); }
};
template <class T> Hold<T> in(const ref<T>& r) { return Hold<T>{r}; }
template <class T> ref<Array<T>> new_array(int32_t n) { auto r = std::make_shared<Array<T>>(); r->init(n); return r; }
template <class T> ref<Array<T>> array_of(std::initializer_list<T> xs) { auto r = std::make_shared<Array<T>>(); r->a = xs; r->length = (int32_t)r->a.size(); return r; }
// new T[a][b]: every row made
template <class T> ref<Array<ref<Array<T>>>> new_array2(int32_t n, int32_t m) {
	auto r = new_array<ref<Array<T>>>(n);
	for (int32_t i = 0; i < n; ++i) (*r)[i] = new_array<T>(m);
	return r;
}
inline ref<Array<char16_t>> String::toCharArray() { auto r = new_array<char16_t>((int32_t)s.size()); std::copy(s.begin(), s.end(), r->a.begin()); return r; }
inline ref<String> String::valueOf(const ref<Array<char16_t>>& a) { return str(std::u16string(nn(a)->a.begin(), a->a.end())); }
inline void String::ctor_(const ref<Array<char16_t>>& a) { s.assign(nn(a)->a.begin(), a->a.end()); }
inline void String::ctor_(const ref<Array<char16_t>>& a, int32_t off, int32_t n) { s.assign(nn(a)->a.begin() + off, a->a.begin() + off + n); }

// ------------------------------------------------------------------------------------------- StringBuilder
struct StringBuilder : Object {
	std::u16string s;
	const char16_t* jclass_() const override { return u"java.lang.StringBuilder"; }
	void ctor_() {}
	void ctor_(int32_t) {}
	void ctor_(const ref<String>& t) { s = nn(t)->s; }
	ref<String> toString() override { return str(s); }
	template <class T> ref<StringBuilder> append(const T& v) { s += sv(v); return self(this); }
	ref<StringBuilder> insert(int32_t at, const ref<Object>& v) { s.insert(at, sv(v)); return self(this); }
	ref<StringBuilder> insert(int32_t at, char16_t v) { s.insert(s.begin() + at, v); return self(this); }
	ref<StringBuilder> insert(int32_t at, int32_t v) { s.insert(at, sv(v)); return self(this); }
	ref<StringBuilder> reverse() { std::reverse(s.begin(), s.end()); return self(this); }
	int32_t length() { return (int32_t)s.size(); }
	char16_t charAt(int32_t i) { if (i < 0 || i >= (int32_t)s.size()) raise_new<StringIndexOutOfBoundsException>(nullptr); return s[i]; }
	void setCharAt(int32_t i, char16_t c) { if (i < 0 || i >= (int32_t)s.size()) raise_new<StringIndexOutOfBoundsException>(nullptr); s[i] = c; }
	ref<StringBuilder> deleteCharAt(int32_t i) { if (i < 0 || i >= (int32_t)s.size()) raise_new<StringIndexOutOfBoundsException>(nullptr); s.erase(i, 1); return self(this); }
	ref<StringBuilder> delete_(int32_t b, int32_t e) { if (e > (int32_t)s.size()) e = (int32_t)s.size(); s.erase(b, e - b); return self(this); }
	void setLength(int32_t n) { s.resize(n, u'\0'); }
	int32_t indexOf(const ref<String>& t) { auto p = s.find(nn(t)->s); return p == std::u16string::npos ? -1 : (int32_t)p; }
	bool isEmpty() { return s.empty(); }
};

// ------------------------------------------------------------------------------------------- output
struct PrintStream : Object {
	FILE* f;
	explicit PrintStream(FILE* x) : f(x) {}
	PrintStream() : f(stdout) {}
	const char16_t* jclass_() const override { return u"java.io.PrintStream"; }
	void w(const std::u16string& s) { std::string t = utf8(s); std::fwrite(t.data(), 1, t.size(), f); }
	template <class T> void print(const T& v) { w(sv(v)); }
	void print(const ref<Array<char16_t>>& a) { w(String::valueOf(a)->s); }
	void println() { w(u"\n"); }
	template <class T> void println(const T& v) { w(sv(v) + u"\n"); }
	void println(const ref<Array<char16_t>>& a) { w(String::valueOf(a)->s + u"\n"); }
	template <class... A> ref<PrintStream> printf(const ref<String>& fmt, A... a) { w(String::format(fmt, a...)->s); return self(this); }
	void flush() { std::fflush(f); }
};
struct System {
	static ref<PrintStream>& out_() { static ref<PrintStream> p = std::make_shared<PrintStream>(stdout); return p; }
	static ref<PrintStream>& err_() { static ref<PrintStream> p = std::make_shared<PrintStream>(stderr); return p; }
	static int64_t currentTimeMillis();
	static int64_t nanoTime();
	static void arraycopy(const ref<Object>& src, int32_t sp, const ref<Object>& dst, int32_t dp, int32_t n);
	static void exit(int32_t code) { std::fflush(stdout); std::exit(code); }
	static int32_t identityHashCode(const ref<Object>& o) { return o ? (int32_t)(((uintptr_t)o.get()) >> 4) : 0; }
};
inline void Throwable::printStackTrace() { System::err_()->println(toString()); }
template <class T> bool copy_typed(const ref<Object>& src, int32_t sp, const ref<Object>& dst, int32_t dp, int32_t n) {
	auto s = std::dynamic_pointer_cast<Array<T>>(src);
	if (!s) return false;
	auto d = cast<Array<T>>(dst);
	if (n < 0 || sp < 0 || dp < 0 || sp + n > s->length || dp + n > d->length) raise_new<ArrayIndexOutOfBoundsException>(u"arraycopy: last source index out of bounds");
	std::vector<T> tmp(s->a.begin() + sp, s->a.begin() + sp + n);
	std::copy(tmp.begin(), tmp.end(), d->a.begin() + dp);
	return true;
}
inline void System::arraycopy(const ref<Object>& src, int32_t sp, const ref<Object>& dst, int32_t dp, int32_t n) {
	if (!src || !dst) raise_npe();
	if (copy_typed<int32_t>(src, sp, dst, dp, n) || copy_typed<int64_t>(src, sp, dst, dp, n) || copy_typed<double>(src, sp, dst, dp, n)
		|| copy_typed<char16_t>(src, sp, dst, dp, n) || copy_typed<int8_t>(src, sp, dst, dp, n) || copy_typed<bool>(src, sp, dst, dp, n)
		|| copy_typed<float>(src, sp, dst, dp, n) || copy_typed<int16_t>(src, sp, dst, dp, n) || copy_typed<ref<Object>>(src, sp, dst, dp, n)
		|| copy_typed<ref<String>>(src, sp, dst, dp, n))
		return;
	raise_new<UnsupportedOperationException>(u"arraycopy of this element type");
}

// ------------------------------------------------------------------------------------------- functional interfaces
// Each is an interface with its one abstract method, erased as Java erases it, and Of<L>: the class a
// lambda becomes (a C++ lambda held by value: its captures copied, as Java's effectively-final ones are).
#define JRT_SAM(Name, QName, Ret, method, Params, Args)                                             \
	struct Name : virtual Object {                                                                   \
		const char16_t* jclass_() const override { return QName; }                                   \
		virtual Ret method Params = 0;                                                               \
		template <class L> struct Of;                                                                \
	};                                                                                               \
	template <class L> struct Name::Of : Name {                                                      \
		L l;                                                                                         \
		explicit Of(L x) : l(std::move(x)) {}                                                        \
		Ret method Params override { return l Args; }                                                \
	};
using O = ref<Object>;
JRT_SAM(Runnable, u"java.lang.Runnable", void, run, (), ())
JRT_SAM(Function, u"java.util.function.Function", O, apply, (O a), (a))
JRT_SAM(BiFunction, u"java.util.function.BiFunction", O, apply, (O a, O b), (a, b))
JRT_SAM(UnaryOperator, u"java.util.function.UnaryOperator", O, apply, (O a), (a))
JRT_SAM(BinaryOperator, u"java.util.function.BinaryOperator", O, apply, (O a, O b), (a, b))
JRT_SAM(Supplier, u"java.util.function.Supplier", O, get, (), ())
JRT_SAM(Consumer, u"java.util.function.Consumer", void, accept, (O a), (a))
JRT_SAM(BiConsumer, u"java.util.function.BiConsumer", void, accept, (O a, O b), (a, b))
JRT_SAM(Predicate, u"java.util.function.Predicate", bool, test, (O a), (a))
JRT_SAM(BiPredicate, u"java.util.function.BiPredicate", bool, test, (O a, O b), (a, b))
JRT_SAM(IntBinaryOperator, u"java.util.function.IntBinaryOperator", int32_t, applyAsInt, (int32_t a, int32_t b), (a, b))
JRT_SAM(IntUnaryOperator, u"java.util.function.IntUnaryOperator", int32_t, applyAsInt, (int32_t a), (a))
JRT_SAM(IntPredicate, u"java.util.function.IntPredicate", bool, test, (int32_t a), (a))
JRT_SAM(IntFunction, u"java.util.function.IntFunction", O, apply, (int32_t a), (a))
JRT_SAM(IntConsumer, u"java.util.function.IntConsumer", void, accept, (int32_t a), (a))
JRT_SAM(IntSupplier, u"java.util.function.IntSupplier", int32_t, getAsInt, (), ())
JRT_SAM(ToIntFunction, u"java.util.function.ToIntFunction", int32_t, applyAsInt, (O a), (a))
JRT_SAM(ToLongFunction, u"java.util.function.ToLongFunction", int64_t, applyAsLong, (O a), (a))
JRT_SAM(ToDoubleFunction, u"java.util.function.ToDoubleFunction", double, applyAsDouble, (O a), (a))
JRT_SAM(IntToDoubleFunction, u"java.util.function.IntToDoubleFunction", double, applyAsDouble, (int32_t a), (a))
JRT_SAM(IntToLongFunction, u"java.util.function.IntToLongFunction", int64_t, applyAsLong, (int32_t a), (a))
JRT_SAM(DoubleUnaryOperator, u"java.util.function.DoubleUnaryOperator", double, applyAsDouble, (double a), (a))
JRT_SAM(DoubleBinaryOperator, u"java.util.function.DoubleBinaryOperator", double, applyAsDouble, (double a, double b), (a, b))
JRT_SAM(LongBinaryOperator, u"java.util.function.LongBinaryOperator", int64_t, applyAsLong, (int64_t a, int64_t b), (a, b))
JRT_SAM(LongUnaryOperator, u"java.util.function.LongUnaryOperator", int64_t, applyAsLong, (int64_t a), (a))
JRT_SAM(Comparator_, u"java.util.Comparator", int32_t, compare, (O a, O b), (a, b))
struct Comparable : virtual Object {
	virtual int32_t compareTo(O o) = 0;
};
// a lambda as an instance of functional interface I
template <class I, class L> ref<I> fn(L l) { return std::make_shared<typename I::template Of<L>>(std::move(l)); }

// java.util.Comparator: the interface, its default methods and its factories
struct Comparator : Comparator_ {
	template <class L> struct Of;
	ref<Comparator> reversed();
	ref<Comparator> thenComparing(ref<Comparator> next);
	ref<Comparator> thenComparing(ref<Function> key) { return thenComparing(comparing(key)); }
	ref<Comparator> thenComparingInt(ref<ToIntFunction> f);
	static ref<Comparator> naturalOrder();
	static ref<Comparator> reverseOrder();
	static ref<Comparator> comparing(ref<Function> key);
	static ref<Comparator> comparing(ref<Function> key, ref<Comparator> c);
	static ref<Comparator> comparingInt(ref<ToIntFunction> key);
	static ref<Comparator> comparingDouble(ref<ToDoubleFunction> key);
};
template <class L> struct Comparator::Of : Comparator {
	L l;
	explicit Of(L x) : l(std::move(x)) {}
	int32_t compare(O a, O b) override { return l(a, b); }
};
int32_t natural_enum(const O& a, const O& b);
inline int32_t natural(const O& a, const O& b) {
	if (!a || !b) raise_npe();
	if (auto s = as<String>(a)) return s->compareTo(b);
	if (auto c = as<Comparable>(a)) return c->compareTo(b);
	if (auto i = as<Integer>(a)) return i->compareTo(b);
	if (auto i = as<Long>(a)) return i->compareTo(b);
	if (auto i = as<Double>(a)) return Double_::compare(i->value, cast<Double>(b)->value);
	if (auto i = as<Float>(a)) return Float_::compare(i->value, cast<Float>(b)->value);
	if (auto i = as<Character>(a)) return i->compareTo(b);
	if (auto i = as<Short>(a)) return i->compareTo(b);
	if (auto i = as<Byte>(a)) return i->compareTo(b);
	if (auto i = as<Boolean>(a)) return Boolean::compare(i->value, cast<Boolean>(b)->value);
	if (auto i = natural_enum(a, b); i != INT32_MIN) return i;
	raise_msg(std::make_shared<ClassCastException>(), std::u16string(a->jclass_()) + u" cannot be cast to class java.lang.Comparable");
}
inline ref<Comparator> Comparator::naturalOrder() { return fn<Comparator>([](O a, O b) { return natural(a, b); }); }
inline ref<Comparator> Comparator::reverseOrder() { return fn<Comparator>([](O a, O b) { return natural(b, a); }); }
inline ref<Comparator> Comparator::reversed() { auto me = self(this); return fn<Comparator>([me](O a, O b) { return me->compare(b, a); }); }
inline ref<Comparator> Comparator::thenComparing(ref<Comparator> n) { auto me = self(this); return fn<Comparator>([me, n](O a, O b) { int32_t r = me->compare(a, b); return r != 0 ? r : n->compare(a, b); }); }
inline ref<Comparator> Comparator::comparing(ref<Function> k) { return fn<Comparator>([k](O a, O b) { return natural(k->apply(a), k->apply(b)); }); }
inline ref<Comparator> Comparator::comparing(ref<Function> k, ref<Comparator> c) { return fn<Comparator>([k, c](O a, O b) { return c->compare(k->apply(a), k->apply(b)); }); }
inline ref<Comparator> Comparator::comparingInt(ref<ToIntFunction> k) { return fn<Comparator>([k](O a, O b) { return Integer_::compare(k->applyAsInt(a), k->applyAsInt(b)); }); }
inline ref<Comparator> Comparator::comparingDouble(ref<ToDoubleFunction> k) { return fn<Comparator>([k](O a, O b) { return Double_::compare(k->applyAsDouble(a), k->applyAsDouble(b)); }); }
inline ref<Comparator> Comparator::thenComparingInt(ref<ToIntFunction> k) { return thenComparing(comparingInt(k)); }
// a comparison, natural when no comparator is given (Collections.sort(list), TreeMap())
inline int32_t compare_with(const ref<Comparator>& c, const O& a, const O& b) { return c ? c->compare(a, b) : natural(a, b); }

// ------------------------------------------------------------------------------------------- collections
struct Iterator : virtual Object {
	const char16_t* jclass_() const override { return u"java.util.Iterator"; }
	virtual bool hasNext() = 0;
	virtual O next() = 0;
	virtual void remove() { raise_new<UnsupportedOperationException>(u"remove"); }
};
// range-for over a Java Iterable: begin()/end() drive its Iterator
struct IterRange {
	ref<Iterator> it;
	struct It {
		ref<Iterator> it; O cur; bool done;
		O operator*() const { return cur; }
		It& operator++() { if (it->hasNext()) cur = it->next(); else done = true; return *this; }
		bool operator!=(const It& o) const { return done != o.done; }
	};
	It begin() { It i{it, nullptr, false}; ++i; return i; }
	It end() { return It{it, nullptr, true}; }
};
struct Stream;
struct Collection;
struct Iterable : virtual Object {
	virtual ref<Iterator> iterator() = 0;
	IterRange::It begin() { return IterRange{iterator()}.begin(); }
	IterRange::It end() { return IterRange::It{nullptr, nullptr, true}; }
	virtual void forEach(ref<Consumer> c) { for (O x : *this) c->accept(x); }
};
// for (T x : xs): each element as T (cast, or unboxed for a primitive)
template <class T> struct Each {
	ref<Iterable> xs;
	struct It {
		IterRange::It i;
		auto operator*() const { if constexpr (std::is_arithmetic<T>::value) return unbox<T>(*i); else return cast<T>(*i); }
		It& operator++() { ++i; return *this; }
		bool operator!=(const It& o) const { return i != o.i; }
	};
	It begin() { return It{nn(xs)->begin()}; }
	It end() { return It{IterRange::It{nullptr, nullptr, true}}; }
};
template <class T, class X> Each<T> each(const ref<X>& xs) { return Each<T>{xs}; }

struct Collection : Iterable {
	virtual int32_t size() = 0;
	virtual bool add(O x) { raise_new<UnsupportedOperationException>(u"add"); }
	virtual bool isEmpty() { return size() == 0; }
	virtual bool contains(O x) { for (O y : *this) if (y ? y->equals(x) : !x) return true; return false; }
	virtual bool remove(O x) { auto it = iterator(); while (it->hasNext()) { O y = it->next(); if (y ? y->equals(x) : !x) { it->remove(); return true; } } return false; }
	virtual bool addAll(ref<Collection> c) { bool ch = false; std::vector<O> tmp; for (O x : *nn(c)) tmp.push_back(x); for (auto& x : tmp) ch |= add(x); return ch; }
	virtual void clear() { auto it = iterator(); while (it->hasNext()) { it->next(); it->remove(); } }
	bool removeIf(ref<Predicate> p) { bool ch = false; auto it = iterator(); while (it->hasNext()) if (p->test(it->next())) { it->remove(); ch = true; } return ch; }
	bool containsAll(ref<Collection> c) { for (O x : *nn(c)) if (!contains(x)) return false; return true; }
	ref<Stream> stream();
	ref<Array<O>> toArray() { auto r = new_array<O>(size()); int32_t i = 0; for (O x : *this) (*r)[i++] = x; return r; }
	ref<String> toString() override {
		std::u16string r = u"[";
		bool first = true;
		for (O x : *this) { if (!first) r += u", "; first = false; r += x.get() == this ? u"(this Collection)" : sv(x); }
		return str(r + u"]");
	}
};
inline ref<String> String::join(const ref<Object>& sep, const ref<Object>& parts) {
	std::u16string r, s = nn(sep)->toString()->s;
	bool first = true;
	if (auto a = as<Array<ref<String>>>(parts)) { for (auto& x : *a) { if (!first) r += s; first = false; r += sv(x); } return str(r); }
	for (O x : *cast<Iterable>(parts)) { if (!first) r += s; first = false; r += sv(x); }
	return str(r);
}

struct List : Collection {
	const char16_t* jclass_() const override { return u"java.util.List"; }
	virtual O get(int32_t i) = 0;
	virtual O set(int32_t i, O x) { raise_new<UnsupportedOperationException>(u"set"); }
	virtual void add(int32_t i, O x) { raise_new<UnsupportedOperationException>(u"add"); }
	using Collection::add;
	virtual O remove(int32_t i) { raise_new<UnsupportedOperationException>(u"remove"); }
	using Collection::remove;
	virtual int32_t indexOf(O x) { int32_t i = 0; for (O y : *this) { if (y ? y->equals(x) : !x) return i; ++i; } return -1; }
	int32_t lastIndexOf(O x) { for (int32_t i = size() - 1; i >= 0; --i) { O y = get(i); if (y ? y->equals(x) : !x) return i; } return -1; }
	virtual void sort(ref<Comparator> c);
	void replaceAll(ref<UnaryOperator> f) { for (int32_t i = 0; i < size(); ++i) set(i, f->apply(get(i))); }
	ref<Iterator> iterator() override;
	ref<List> subList(int32_t b, int32_t e);
	O getFirst() { if (isEmpty()) raise_new<NoSuchElementException>(nullptr); return get(0); }
	O getLast() { if (isEmpty()) raise_new<NoSuchElementException>(nullptr); return get(size() - 1); }
	bool equals(O o) override {
		auto l = as<List>(o);
		if (!l) return false;
		if (l->size() != size()) return false;
		for (int32_t i = 0; i < size(); ++i) { O a = get(i), b = l->get(i); if (!(a ? a->equals(b) : !b)) return false; }
		return true;
	}
	int32_t hashCode() override { int32_t h = 1; for (O x : *this) h = 31 * h + (x ? x->hashCode() : 0); return h; }
	static ref<List> of();
	template <class... A> static ref<List> of(A... a);
	static ref<List> copyOf(ref<Collection> c);
};
struct ArrayList : List {
	std::vector<O> v;
	const char16_t* jclass_() const override { return u"java.util.ArrayList"; }
	void ctor_() {}
	void ctor_(int32_t n) { if (n < 0) raise_new<IllegalArgumentException>(nullptr); }
	void ctor_(ref<Collection> c) { for (O x : *nn(c)) v.push_back(x); }
	void check(int32_t i, int32_t n) { if (i < 0 || i >= n) raise_msg(std::make_shared<IndexOutOfBoundsException>(), u"Index " + sv(i) + u" out of bounds for length " + sv(n)); }
	int32_t size() override { return (int32_t)v.size(); }
	O get(int32_t i) override { check(i, size()); return v[i]; }
	O set(int32_t i, O x) override { check(i, size()); O o = v[i]; v[i] = x; return o; }
	bool add(O x) override { v.push_back(x); return true; }
	void add(int32_t i, O x) override { if (i < 0 || i > size()) check(i, size()); v.insert(v.begin() + i, x); }
	O remove(int32_t i) override { check(i, size()); O o = v[i]; v.erase(v.begin() + i); return o; }
	bool remove(O x) override { int32_t i = indexOf(x); if (i < 0) return false; v.erase(v.begin() + i); return true; }
	void clear() override { v.clear(); }
	void sort(ref<Comparator> c) override { std::stable_sort(v.begin(), v.end(), [&](const O& a, const O& b) { return compare_with(c, a, b) < 0; }); }
};
using LinkedList_ = ArrayList;
struct LinkedList : ArrayList {
	const char16_t* jclass_() const override { return u"java.util.LinkedList"; }
	void addFirst(O x) { v.insert(v.begin(), x); }
	void addLast(O x) { v.push_back(x); }
	O removeFirst() { if (v.empty()) raise_new<NoSuchElementException>(nullptr); O o = v.front(); v.erase(v.begin()); return o; }
	O removeLast() { if (v.empty()) raise_new<NoSuchElementException>(nullptr); O o = v.back(); v.pop_back(); return o; }
	O peekFirst() { return v.empty() ? nullptr : v.front(); }
	O peekLast() { return v.empty() ? nullptr : v.back(); }
	O poll() { return v.empty() ? nullptr : removeFirst(); }
	O peek() { return peekFirst(); }
	bool offer(O x) { v.push_back(x); return true; }
	void push(O x) { addFirst(x); }
	O pop() { return removeFirst(); }
};
struct ListIter : Iterator {
	ref<List> l; int32_t i = 0; int32_t last = -1;
	bool hasNext() override { return i < l->size(); }
	O next() override { if (i >= l->size()) raise_new<NoSuchElementException>(nullptr); last = i; return l->get(i++); }
	void remove() override { if (last < 0) raise_new<IllegalStateException>(nullptr); l->remove(last); i = last; last = -1; }
};
inline ref<Iterator> List::iterator() { auto it = std::make_shared<ListIter>(); it->l = self(this); return it; }
inline void List::sort(ref<Comparator> c) {
	std::vector<O> tmp;
	for (int32_t i = 0; i < size(); ++i) tmp.push_back(get(i));
	std::stable_sort(tmp.begin(), tmp.end(), [&](const O& a, const O& b) { return compare_with(c, a, b) < 0; });
	for (int32_t i = 0; i < size(); ++i) set(i, tmp[i]);
}
struct ImmutableList : ArrayList {
	const char16_t* jclass_() const override { return u"java.util.ImmutableCollections.ListN"; }
	bool add(O) override { raise_new<UnsupportedOperationException>(nullptr); }
	void add(int32_t, O) override { raise_new<UnsupportedOperationException>(nullptr); }
	O set(int32_t, O) override { raise_new<UnsupportedOperationException>(nullptr); }
	O remove(int32_t) override { raise_new<UnsupportedOperationException>(nullptr); }
	bool remove(O) override { raise_new<UnsupportedOperationException>(nullptr); }
	void clear() override { raise_new<UnsupportedOperationException>(nullptr); }
	void sort(ref<Comparator>) override { raise_new<UnsupportedOperationException>(nullptr); }
};
inline ref<List> List::of() { return std::make_shared<ImmutableList>(); }
template <class... A> ref<List> List::of(A... a) { auto l = std::make_shared<ImmutableList>(); l->v = {O(a)...}; for (auto& x : l->v) if (!x) raise_npe(); return l; }
inline ref<List> List::copyOf(ref<Collection> c) { auto l = std::make_shared<ImmutableList>(); for (O x : *nn(c)) l->v.push_back(x); return l; }
inline ref<List> List::subList(int32_t b, int32_t e) { auto l = std::make_shared<ArrayList>(); for (int32_t i = b; i < e; ++i) l->v.push_back(get(i)); return l; }

struct Arrays {
	template <class T> static void sort(const ref<Array<T>>& a) {
		if constexpr (std::is_same<T, double>::value || std::is_same<T, float>::value)
			std::sort(nn(a)->a.begin(), a->a.end(), [](T x, T y) { return Double_::compare(x, y) < 0; });
		else if constexpr (std::is_arithmetic<T>::value) std::sort(nn(a)->a.begin(), a->a.end());
		else std::stable_sort(nn(a)->a.begin(), a->a.end(), [](const T& x, const T& y) { return natural(x, y) < 0; });
	}
	template <class T> static void sort(const ref<Array<T>>& a, ref<Comparator> c) { std::stable_sort(nn(a)->a.begin(), a->a.end(), [&](const T& x, const T& y) { return c->compare(x, y) < 0; }); }
	template <class T> static void fill(const ref<Array<T>>& a, T v) { std::fill(nn(a)->a.begin(), a->a.end(), v); }
	template <class T> static ref<String> toString(const ref<Array<T>>& a) {
		if (!a) return lit(u"null");
		std::u16string r = u"[";
		for (int32_t i = 0; i < a->length; ++i) { if (i) r += u", "; r += sv(a->a[i]); }
		return str(r + u"]");
	}
	template <class T> static ref<Array<T>> copyOf(const ref<Array<T>>& a, int32_t n) { auto r = new_array<T>(n); for (int32_t i = 0; i < n && i < nn(a)->length; ++i) r->a[i] = a->a[i]; return r; }
	template <class T> static ref<Array<T>> copyOfRange(const ref<Array<T>>& a, int32_t b, int32_t e) { auto r = new_array<T>(e - b); for (int32_t i = b; i < e && i < nn(a)->length; ++i) r->a[i - b] = a->a[i]; return r; }
	template <class T> static bool equals(const ref<Array<T>>& a, const ref<Array<T>>& b) { if (a == b) return true; if (!a || !b) return false; return a->a == b->a; }
	template <class T> static int32_t hashCode(const ref<Array<T>>& a) { if (!a) return 0; int32_t h = 1; for (auto& x : a->a) h = 31 * h + hash_of(x); return h; }
	template <class... A> static ref<List> asList(A... a) { auto l = std::make_shared<ArrayList>(); l->v = {O(a)...}; return l; }
	template <class T> static ref<struct Stream> stream(const ref<Array<T>>& a);
	static ref<IntStream> stream(const ref<Array<int32_t>>& a);
	template <class T> static int32_t binarySearch(const ref<Array<T>>& a, T key) {
		int32_t lo = 0, hi = nn(a)->length - 1;
		while (lo <= hi) { int32_t mid = (int32_t)((uint32_t)(lo + hi) >> 1); T v = a->a[mid]; if (v < key) lo = mid + 1; else if (key < v) hi = mid - 1; else return mid; }
		return -(lo + 1);
	}
};

// HashMap: Java's own table -- the same hash spreading, bucket order and resizing -- so iteration order is Java's
struct MapEntry : Object {
	O key, value;
	int32_t hash = 0;
	ref<MapEntry> next;
	const char16_t* jclass_() const override { return u"java.util.HashMap.Node"; }
	O getKey() { return key; }
	O getValue() { return value; }
	O setValue(O v) { O o = value; value = v; return o; }
	ref<String> toString() override { return str(sv(key) + u"=" + sv(value)); }
	bool equals(O o) override { auto e = as<MapEntry>(o); return e && (key ? key->equals(e->key) : !e->key) && (value ? value->equals(e->value) : !e->value); }
	int32_t hashCode() override { return (key ? key->hashCode() : 0) ^ (value ? value->hashCode() : 0); }
};
inline int32_t hash_key(const O& k) { if (!k) return 0; int32_t h = k->hashCode(); return h ^ (int32_t)((uint32_t)h >> 16); }
inline bool same_key(const O& a, const O& b) { return a == b || (a && a->equals(b)); }

struct Set;
struct Map : virtual Object {
	const char16_t* jclass_() const override { return u"java.util.Map"; }
	virtual int32_t size() = 0;
	virtual O get(O k) = 0;
	virtual O put(O k, O v) = 0;
	virtual O remove(O k) = 0;
	virtual bool containsKey(O k) = 0;
	virtual std::vector<ref<MapEntry>> entries() = 0;          // in iteration order
	virtual void clear() = 0;
	bool isEmpty() { return size() == 0; }
	O getOrDefault(O k, O d) { return containsKey(k) ? get(k) : d; }
	O putIfAbsent(O k, O v) { O o = get(k); if (!o) put(k, v); return o; }
	bool containsValue(O v) { for (auto& e : entries()) if (e->value ? e->value->equals(v) : !v) return true; return false; }
	// the compute family inserts as HashMap's own does (at the head of its bin): see HashMap::insert_new
	virtual void insert_new(O k, O v) { put(k, v); }
	O merge(O k, O v, ref<BiFunction> f) { O o = get(k); O n = o ? f->apply(o, v) : v; if (n) { if (o || containsKey(k)) put(k, n); else insert_new(k, n); } else remove(k); return n; }
	O computeIfAbsent(O k, ref<Function> f) { O o = get(k); if (!o) { o = f->apply(k); if (o) { if (containsKey(k)) put(k, o); else insert_new(k, o); } } return o; }
	O compute(O k, ref<BiFunction> f) { bool had = containsKey(k); O n = f->apply(k, get(k)); if (n) { if (had) put(k, n); else insert_new(k, n); } else remove(k); return n; }
	void forEach(ref<BiConsumer> f) { for (auto& e : entries()) f->accept(e->key, e->value); }
	void putAll(ref<Map> m) { for (auto& e : nn(m)->entries()) put(e->key, e->value); }
	ref<Set> keySet();
	ref<Collection> values();
	ref<Set> entrySet();
	ref<String> toString() override {
		std::u16string r = u"{";
		bool first = true;
		for (auto& e : entries()) { if (!first) r += u", "; first = false; r += sv(e->key) + u"=" + sv(e->value); }
		return str(r + u"}");
	}
	bool equals(O o) override {
		auto m = as<Map>(o);
		if (!m || m->size() != size()) return false;
		for (auto& e : entries()) { if (!m->containsKey(e->key)) return false; O v = m->get(e->key); if (!(e->value ? e->value->equals(v) : !v)) return false; }
		return true;
	}
	int32_t hashCode() override { int32_t h = 0; for (auto& e : entries()) h += e->hashCode(); return h; }
	template <class... A> static ref<Map> of(A... a);
};
struct HashMap : Map {
	std::vector<ref<MapEntry>> table;
	int32_t count = 0;
	bool linked = false;                                          // LinkedHashMap: insertion order
	std::vector<ref<MapEntry>> order;
	const char16_t* jclass_() const override { return u"java.util.HashMap"; }
	void ctor_() {}
	void ctor_(int32_t n) { int32_t c = 1; while (c < n) c <<= 1; if (n > 0) table.resize(c); }
	void ctor_(ref<Map> m) { putAll(m); }
	ref<MapEntry> find(const O& k) {
		if (table.empty()) return nullptr;
		int32_t h = hash_key(k);
		for (auto e = table[h & (table.size() - 1)]; e; e = e->next) if (e->hash == h && same_key(e->key, k)) return e;
		return nullptr;
	}
	void resize() {
		size_t n = table.empty() ? 16 : table.size() * 2;
		std::vector<ref<MapEntry>> t(n);
		for (auto& head : table) {                                // split each bin keeping its order, as Java does
			ref<MapEntry> lo, loT, hi, hiT;
			for (auto e = head; e;) {
				auto nx = e->next;
				e->next = nullptr;
				if ((e->hash & table.size()) == 0) { if (loT) loT->next = e; else lo = e; loT = e; }
				else { if (hiT) hiT->next = e; else hi = e; hiT = e; }
				e = nx;
			}
			size_t i = &head - &table[0];
			t[i] = lo;
			t[i + table.size()] = hi;
		}
		table.swap(t);
	}
	int32_t size() override { return count; }
	O get(O k) override { auto e = find(k); return e ? e->value : nullptr; }
	bool containsKey(O k) override { return find(k) != nullptr; }
	O put(O k, O v) override {
		if (table.empty()) resize();
		auto e = find(k);
		if (e) { O o = e->value; e->value = v; return o; }
		auto n = std::make_shared<MapEntry>();
		n->key = k; n->value = v; n->hash = hash_key(k);
		size_t i = n->hash & (table.size() - 1);
		if (!table[i]) table[i] = n;
		else { auto t = table[i]; while (t->next) t = t->next; t->next = n; }
		if (linked) order.push_back(n);
		if (++count > (int32_t)(table.size() * 3 / 4)) resize();
		return nullptr;
	}
	O remove(O k) override {
		if (table.empty()) return nullptr;
		int32_t h = hash_key(k);
		size_t i = h & (table.size() - 1);
		ref<MapEntry> prev;
		for (auto e = table[i]; e; prev = e, e = e->next) {
			if (e->hash == h && same_key(e->key, k)) {
				if (prev) prev->next = e->next; else table[i] = e->next;
				--count;
				if (linked) order.erase(std::find(order.begin(), order.end(), e));
				return e->value;
			}
		}
		return nullptr;
	}
	// HashMap.merge / compute / computeIfAbsent: resize first when already past the threshold, then the new
	// node goes at the head of its bin (put appends at the tail) -- so iteration order is Java's
	void insert_new(O k, O v) override {
		if (table.empty() || count > (int32_t)(table.size() * 3 / 4)) resize();
		auto n = std::make_shared<MapEntry>();
		n->key = k; n->value = v; n->hash = hash_key(k);
		size_t i = n->hash & (table.size() - 1);
		n->next = table[i];
		table[i] = n;
		if (linked) order.push_back(n);
		++count;
	}
	void clear() override { for (auto& t : table) t = nullptr; count = 0; order.clear(); }
	std::vector<ref<MapEntry>> entries() override {
		if (linked) return order;
		std::vector<ref<MapEntry>> r;
		for (auto& head : table) for (auto e = head; e; e = e->next) r.push_back(e);
		return r;
	}
};
struct LinkedHashMap : HashMap {
	const char16_t* jclass_() const override { return u"java.util.LinkedHashMap"; }
	LinkedHashMap() { linked = true; }
};
struct TreeMap : Map {
	ref<Comparator> cmp;
	std::vector<ref<MapEntry>> v;                                 // kept sorted
	const char16_t* jclass_() const override { return u"java.util.TreeMap"; }
	void ctor_() {}
	void ctor_(ref<Comparator> c) { cmp = c; }
	size_t lower(const O& k) { return std::lower_bound(v.begin(), v.end(), k, [&](const ref<MapEntry>& e, const O& x) { return compare_with(cmp, e->key, x) < 0; }) - v.begin(); }
	ref<MapEntry> find(const O& k) { if (!k && !cmp) raise_npe(); size_t i = lower(k); return i < v.size() && compare_with(cmp, v[i]->key, k) == 0 ? v[i] : nullptr; }
	int32_t size() override { return (int32_t)v.size(); }
	O get(O k) override { auto e = find(k); return e ? e->value : nullptr; }
	bool containsKey(O k) override { return find(k) != nullptr; }
	O put(O k, O val) override {
		if (!k && !cmp) raise_npe();
		size_t i = lower(k);
		if (i < v.size() && compare_with(cmp, v[i]->key, k) == 0) { O o = v[i]->value; v[i]->value = val; return o; }
		auto n = std::make_shared<MapEntry>(); n->key = k; n->value = val;
		v.insert(v.begin() + i, n);
		return nullptr;
	}
	O remove(O k) override { size_t i = lower(k); if (i < v.size() && compare_with(cmp, v[i]->key, k) == 0) { O o = v[i]->value; v.erase(v.begin() + i); return o; } return nullptr; }
	void clear() override { v.clear(); }
	std::vector<ref<MapEntry>> entries() override { return v; }
	O firstKey() { if (v.empty()) raise_new<NoSuchElementException>(nullptr); return v.front()->key; }
	O lastKey() { if (v.empty()) raise_new<NoSuchElementException>(nullptr); return v.back()->key; }
};
template <class... A> ref<Map> Map::of(A... a) {
	auto m = std::make_shared<HashMap>();
	O xs[] = {O(a)..., nullptr};
	for (size_t i = 0; i + 1 < sizeof...(A); i += 2) m->put(xs[i], xs[i + 1]);
	return m;
}

struct Set : Collection {
	const char16_t* jclass_() const override { return u"java.util.Set"; }
	bool equals(O o) override { auto s = as<Set>(o); if (!s || s->size() != size()) return false; for (O x : *this) if (!s->contains(x)) return false; return true; }
	int32_t hashCode() override { int32_t h = 0; for (O x : *this) h += x ? x->hashCode() : 0; return h; }
	template <class... A> static ref<Set> of(A... a);
};
struct MapIter : Iterator {
	ref<Map> m; std::vector<ref<MapEntry>> es; size_t i = 0; int kind = 0; O last; bool has = false;
	bool hasNext() override { return i < es.size(); }
	O next() override { if (i >= es.size()) raise_new<NoSuchElementException>(nullptr); auto e = es[i++]; last = e->key; has = true; return kind == 0 ? e->key : (kind == 1 ? e->value : O(e)); }
	void remove() override { if (!has) raise_new<IllegalStateException>(nullptr); m->remove(last); has = false; }
};
// a set backed by a map's keys (HashSet is a HashMap's key set, as in Java)
struct MapSet : Set {
	ref<Map> m;
	int kind = 0;                                                 // 0 keys, 1 values, 2 entries
	const char16_t* jclass_() const override { return u"java.util.HashSet"; }
	int32_t size() override { return m->size(); }
	ref<Iterator> iterator() override { auto it = std::make_shared<MapIter>(); it->m = m; it->es = m->entries(); it->kind = kind; return it; }
	bool contains(O x) override { if (kind == 0) return m->containsKey(x); return Collection::contains(x); }
	bool remove(O x) override { if (kind == 0) { bool had = m->containsKey(x); m->remove(x); return had; } return Collection::remove(x); }
	bool add(O x) override { if (kind != 0) raise_new<UnsupportedOperationException>(nullptr); bool had = m->containsKey(x); if (!had) m->put(x, box(true)); return !had; }
	void clear() override { m->clear(); }
};
struct HashSet : MapSet {
	void ctor_() { m = std::make_shared<HashMap>(); }
	void ctor_(int32_t n) { auto h = std::make_shared<HashMap>(); h->ctor_(n); m = h; }
	void ctor_(ref<Collection> c) { ctor_(); for (O x : *nn(c)) add(x); }
};
struct LinkedHashSet : MapSet {
	const char16_t* jclass_() const override { return u"java.util.LinkedHashSet"; }
	void ctor_() { m = std::make_shared<LinkedHashMap>(); }
	void ctor_(ref<Collection> c) { ctor_(); for (O x : *nn(c)) add(x); }
};
struct TreeSet : MapSet {
	const char16_t* jclass_() const override { return u"java.util.TreeSet"; }
	void ctor_() { m = std::make_shared<TreeMap>(); }
	void ctor_(ref<Comparator> c) { auto t = std::make_shared<TreeMap>(); t->cmp = c; m = t; }
	void ctor_(ref<Collection> c) { ctor_(); for (O x : *nn(c)) add(x); }
	O first() { return std::static_pointer_cast<TreeMap>(m)->firstKey(); }
	O last() { return std::static_pointer_cast<TreeMap>(m)->lastKey(); }
};
template <class... A> ref<Set> Set::of(A... a) { auto s = make<LinkedHashSet>(); for (O x : {O(a)...}) s->add(x); return s; }
inline ref<Set> Map::keySet() { auto s = std::make_shared<MapSet>(); s->m = std::dynamic_pointer_cast<Map>(shared_from_this()); s->kind = 0; return s; }
inline ref<Collection> Map::values() { auto s = std::make_shared<MapSet>(); s->m = std::dynamic_pointer_cast<Map>(shared_from_this()); s->kind = 1; return s; }
inline ref<Set> Map::entrySet() { auto s = std::make_shared<MapSet>(); s->m = std::dynamic_pointer_cast<Map>(shared_from_this()); s->kind = 2; return s; }

// for (Map.Entry<K, V> e : m.entrySet()) whose body only reads e's key and value and leaves the map alone:
// a structured binding over the entries, each key and value cast to what javac said they are
template <class K, class V> std::vector<std::pair<ref<K>, ref<V>>> entries(const ref<Map>& m) {
	std::vector<std::pair<ref<K>, ref<V>>> out;
	for (auto& e : nn(m)->entries()) out.emplace_back(cast<K>(e->key), cast<V>(e->value));
	return out;
}
struct ArrayDeque : LinkedList {
	const char16_t* jclass_() const override { return u"java.util.ArrayDeque"; }
};
struct Deque : LinkedList {};
struct Queue : LinkedList {};

struct Collections {
	static void sort(ref<List> l) { nn(l)->sort(nullptr); }
	static void sort(ref<List> l, ref<Comparator> c) { nn(l)->sort(c); }
	static void reverse(ref<List> l) { int32_t n = nn(l)->size(); for (int32_t i = 0; i < n / 2; ++i) { O a = l->get(i); l->set(i, l->get(n - 1 - i)); l->set(n - 1 - i, a); } }
	static O max(ref<Collection> c) { O best; bool first = true; for (O x : *nn(c)) { if (first || natural(x, best) > 0) best = x; first = false; } if (first) raise_new<NoSuchElementException>(nullptr); return best; }
	static O min(ref<Collection> c) { O best; bool first = true; for (O x : *nn(c)) { if (first || natural(x, best) < 0) best = x; first = false; } if (first) raise_new<NoSuchElementException>(nullptr); return best; }
	static ref<List> emptyList() { return List::of(); }
	static ref<List> unmodifiableList(ref<List> l) { return List::copyOf(l); }
	static int32_t frequency(ref<Collection> c, O x) { int32_t n = 0; for (O y : *nn(c)) if (y ? y->equals(x) : !x) ++n; return n; }
	static void swap(ref<List> l, int32_t i, int32_t j) { O a = l->get(i); l->set(i, l->get(j)); l->set(j, a); }
	template <class... A> static bool addAll(ref<Collection> c, A... a) { bool ch = false; for (O x : {O(a)...}) ch |= c->add(x); return ch; }
};

struct Objects {
	static bool equals(const O& a, const O& b) { return a == b || (a && a->equals(b)); }
	static int32_t hashCode(const O& a) { return a ? a->hashCode() : 0; }
	template <class... A> static int32_t hash(A... a) { int32_t h = 1; for (O x : {O(a)...}) h = 31 * h + (x ? x->hashCode() : 0); return h; }
	static ref<String> toString(const O& a) { return String::valueOf(a); }
	static ref<String> toString(const O& a, const ref<String>& d) { return a ? a->toString() : d; }
	template <class T> static ref<T> requireNonNull(const ref<T>& a) { if (!a) raise_npe(); return a; }
	template <class T> static ref<T> requireNonNull(const ref<T>& a, const ref<String>& m) { if (!a) raise_msg(std::make_shared<NullPointerException>(), m->s); return a; }
	static bool isNull(const O& a) { return !a; }
	static bool nonNull(const O& a) { return (bool)a; }
	template <class T> static ref<T> requireNonNullElse(const ref<T>& a, const ref<T>& d) { return a ? a : d; }
};

// java.util.Optional: std::optional of the reference (it never holds null)
struct Optional : Object {
	std::optional<O> v;
	const char16_t* jclass_() const override { return u"java.util.Optional"; }
	static ref<Optional> of(O x) { if (!x) raise_npe(); auto o = std::make_shared<Optional>(); o->v = x; return o; }
	static ref<Optional> ofNullable(O x) { auto o = std::make_shared<Optional>(); if (x) o->v = x; return o; }
	static ref<Optional> empty() { return std::make_shared<Optional>(); }
	bool isPresent() { return v.has_value(); }
	bool isEmpty() { return !v.has_value(); }
	O get() { if (!v) raise_msg(std::make_shared<NoSuchElementException>(), u"No value present"); return *v; }
	O orElseThrow() { return get(); }
	O orElse(O d) { return v ? *v : d; }
	O orElseGet(ref<Supplier> s) { return v ? *v : s->get(); }
	ref<Optional> map(ref<Function> f) { return v ? ofNullable(f->apply(*v)) : empty(); }
	ref<Optional> filter(ref<Predicate> p) { return v && p->test(*v) ? self(this) : empty(); }
	void ifPresent(ref<Consumer> c) { if (v) c->accept(*v); }
	ref<String> toString() override { return v ? str(u"Optional[" + sv(*v) + u"]") : lit(u"Optional.empty"); }
	bool equals(O o) override { auto p = as<Optional>(o); return p && (v ? (p->v && Objects::equals(*v, *p->v)) : !p->v); }
	int32_t hashCode() override { return v ? (*v)->hashCode() : 0; }
};
struct OptionalInt : Object {
	std::optional<int32_t> v;
	const char16_t* jclass_() const override { return u"java.util.OptionalInt"; }
	bool isPresent() { return v.has_value(); }
	int32_t getAsInt() { if (!v) raise_msg(std::make_shared<NoSuchElementException>(), u"No value present"); return *v; }
	int32_t orElse(int32_t d) { return v ? *v : d; }
	ref<String> toString() override { return v ? str(u"OptionalInt[" + sv(*v) + u"]") : lit(u"OptionalInt.empty"); }
};
struct OptionalDouble : Object {
	std::optional<double> v;
	const char16_t* jclass_() const override { return u"java.util.OptionalDouble"; }
	bool isPresent() { return v.has_value(); }
	double getAsDouble() { if (!v) raise_msg(std::make_shared<NoSuchElementException>(), u"No value present"); return *v; }
	double orElse(double d) { return v ? *v : d; }
	ref<String> toString() override { return v ? str(u"OptionalDouble[" + sv(*v) + u"]") : lit(u"OptionalDouble.empty"); }
};

// ------------------------------------------------------------------------------------------- streams
// Lazy, as Java's are: each stage pulls from the one before, so a short-circuiting end (findFirst, limit,
// anyMatch) stops the pipeline where Java's would, and each lambda runs as often, and in the order, Java runs it.
template <class T> using Pull = std::function<bool(T&)>;
struct Collector;
struct IntStream;
struct Stream : Object {
	Pull<O> pull;
	const char16_t* jclass_() const override { return u"java.util.stream.ReferencePipeline"; }
	static ref<Stream> from(Pull<O> p) { auto s = std::make_shared<Stream>(); s->pull = std::move(p); return s; }
	template <class... A> static ref<Stream> of(A... a) { auto v = std::make_shared<std::vector<O>>(std::initializer_list<O>{O(a)...}); auto i = std::make_shared<size_t>(0); return from([v, i](O& out) { if (*i >= v->size()) return false; out = (*v)[(*i)++]; return true; }); }
	static ref<Stream> iterate(O seed, ref<UnaryOperator> f) { auto cur = std::make_shared<O>(seed); auto first = std::make_shared<bool>(true); return from([cur, first, f](O& out) { if (!*first) *cur = f->apply(*cur); *first = false; out = *cur; return true; }); }
	static ref<Stream> generate(ref<Supplier> f) { return from([f](O& out) { out = f->get(); return true; }); }
	ref<Stream> filter(ref<Predicate> p) { auto up = pull; return from([up, p](O& out) { while (up(out)) if (p->test(out)) return true; return false; }); }
	ref<Stream> map(ref<Function> f) { auto up = pull; return from([up, f](O& out) { O x; if (!up(x)) return false; out = f->apply(x); return true; }); }
	ref<IntStream> mapToInt(ref<ToIntFunction> f);
	ref<Stream> peek(ref<Consumer> c) { auto up = pull; return from([up, c](O& out) { if (!up(out)) return false; c->accept(out); return true; }); }
	ref<Stream> limit(int64_t n) { auto up = pull; auto k = std::make_shared<int64_t>(0); return from([up, n, k](O& out) { if (*k >= n) return false; ++*k; return up(out); }); }
	ref<Stream> skip(int64_t n) { auto up = pull; auto k = std::make_shared<int64_t>(0); return from([up, n, k](O& out) { while (*k < n) { O x; if (!up(x)) return false; ++*k; } return up(out); }); }
	ref<Stream> flatMap(ref<Function> f) {
		auto up = pull; auto inner = std::make_shared<ref<Stream>>();
		return from([up, f, inner](O& out) { for (;;) { if (*inner && (*inner)->pull(out)) return true; O x; if (!up(x)) return false; *inner = cast<Stream>(f->apply(x)); } });
	}
	std::vector<O> drain() { std::vector<O> v; O x; while (pull(x)) v.push_back(x); return v; }
	ref<Stream> sorted() { auto up = pull; auto v = std::make_shared<std::vector<O>>(); auto i = std::make_shared<size_t>(0); auto done = std::make_shared<bool>(false);
		return from([up, v, i, done](O& out) { if (!*done) { O x; while (up(x)) v->push_back(x); std::stable_sort(v->begin(), v->end(), [](const O& a, const O& b) { return natural(a, b) < 0; }); *done = true; } if (*i >= v->size()) return false; out = (*v)[(*i)++]; return true; }); }
	ref<Stream> sorted(ref<Comparator> c) { auto up = pull; auto v = std::make_shared<std::vector<O>>(); auto i = std::make_shared<size_t>(0); auto done = std::make_shared<bool>(false);
		return from([up, c, v, i, done](O& out) { if (!*done) { O x; while (up(x)) v->push_back(x); std::stable_sort(v->begin(), v->end(), [&](const O& a, const O& b) { return c->compare(a, b) < 0; }); *done = true; } if (*i >= v->size()) return false; out = (*v)[(*i)++]; return true; }); }
	ref<Stream> distinct() { auto up = pull; auto seen = std::make_shared<HashSet>(); seen->ctor_(); return from([up, seen](O& out) { while (up(out)) if (seen->add(out)) return true; return false; }); }
	void forEach(ref<Consumer> c) { O x; while (pull(x)) c->accept(x); }
	int64_t count() { int64_t n = 0; O x; while (pull(x)) ++n; return n; }
	ref<List> toList() { auto l = std::make_shared<ImmutableList>(); l->v = drain(); return l; }
	O collect(ref<Collector> c);
	O reduce(O id, ref<BinaryOperator> f) { O acc = id, x; while (pull(x)) acc = f->apply(acc, x); return acc; }
	ref<Optional> reduce(ref<BinaryOperator> f) { O acc, x; bool any = false; while (pull(x)) { acc = any ? f->apply(acc, x) : x; any = true; } return any ? Optional::of(acc) : Optional::empty(); }
	bool anyMatch(ref<Predicate> p) { O x; while (pull(x)) if (p->test(x)) return true; return false; }
	bool allMatch(ref<Predicate> p) { O x; while (pull(x)) if (!p->test(x)) return false; return true; }
	bool noneMatch(ref<Predicate> p) { return !anyMatch(p); }
	ref<Optional> findFirst() { O x; if (pull(x)) return Optional::of(x); return Optional::empty(); }
	ref<Optional> findAny() { return findFirst(); }
	ref<Optional> max(ref<Comparator> c) { O best, x; bool any = false; while (pull(x)) { if (!any || c->compare(x, best) > 0) best = x; any = true; } return any ? Optional::of(best) : Optional::empty(); }
	ref<Optional> min(ref<Comparator> c) { O best, x; bool any = false; while (pull(x)) { if (!any || c->compare(x, best) < 0) best = x; any = true; } return any ? Optional::of(best) : Optional::empty(); }
};
inline ref<Stream> Collection::stream() { auto it = iterator(); return Stream::from([it](O& out) { if (!it->hasNext()) return false; out = it->next(); return true; }); }
template <class T> ref<Stream> Arrays::stream(const ref<Array<T>>& a) { auto i = std::make_shared<int32_t>(0); auto arr = nn(a); return Stream::from([arr, i](O& out) { if (*i >= arr->length) return false; out = arr->a[(*i)++]; return true; }); }

struct IntStream : Object {
	Pull<int32_t> pull;
	const char16_t* jclass_() const override { return u"java.util.stream.IntPipeline"; }
	static ref<IntStream> from(Pull<int32_t> p) { auto s = std::make_shared<IntStream>(); s->pull = std::move(p); return s; }
	static ref<IntStream> range(int32_t a, int32_t b) { auto i = std::make_shared<int64_t>(a); return from([i, b](int32_t& out) { if (*i >= b) return false; out = (int32_t)(*i)++; return true; }); }
	static ref<IntStream> rangeClosed(int32_t a, int32_t b) { auto i = std::make_shared<int64_t>(a); return from([i, b](int32_t& out) { if (*i > b) return false; out = (int32_t)(*i)++; return true; }); }
	template <class... A> static ref<IntStream> of(A... a) { auto v = std::make_shared<std::vector<int32_t>>(std::initializer_list<int32_t>{(int32_t)a...}); auto i = std::make_shared<size_t>(0); return from([v, i](int32_t& out) { if (*i >= v->size()) return false; out = (*v)[(*i)++]; return true; }); }
	static ref<IntStream> iterate(int32_t seed, ref<IntUnaryOperator> f) { auto cur = std::make_shared<int32_t>(seed); auto first = std::make_shared<bool>(true); return from([cur, first, f](int32_t& out) { if (!*first) *cur = f->applyAsInt(*cur); *first = false; out = *cur; return true; }); }
	ref<IntStream> filter(ref<IntPredicate> p) { auto up = pull; return from([up, p](int32_t& out) { while (up(out)) if (p->test(out)) return true; return false; }); }
	ref<IntStream> map(ref<IntUnaryOperator> f) { auto up = pull; return from([up, f](int32_t& out) { int32_t x; if (!up(x)) return false; out = f->applyAsInt(x); return true; }); }
	ref<Stream> mapToObj(ref<IntFunction> f) { auto up = pull; return Stream::from([up, f](O& out) { int32_t x; if (!up(x)) return false; out = f->apply(x); return true; }); }
	ref<Stream> boxed() { auto up = pull; return Stream::from([up](O& out) { int32_t x; if (!up(x)) return false; out = box(x); return true; }); }
	ref<IntStream> limit(int64_t n) { auto up = pull; auto k = std::make_shared<int64_t>(0); return from([up, n, k](int32_t& out) { if (*k >= n) return false; ++*k; return up(out); }); }
	ref<IntStream> skip(int64_t n) { auto up = pull; auto k = std::make_shared<int64_t>(0); return from([up, n, k](int32_t& out) { while (*k < n) { int32_t x; if (!up(x)) return false; ++*k; } return up(out); }); }
	ref<IntStream> peek(ref<IntConsumer> c) { auto up = pull; return from([up, c](int32_t& out) { if (!up(out)) return false; c->accept(out); return true; }); }
	ref<IntStream> sorted() { auto up = pull; auto v = std::make_shared<std::vector<int32_t>>(); auto i = std::make_shared<size_t>(0); auto done = std::make_shared<bool>(false);
		return from([up, v, i, done](int32_t& out) { if (!*done) { int32_t x; while (up(x)) v->push_back(x); std::sort(v->begin(), v->end()); *done = true; } if (*i >= v->size()) return false; out = (*v)[(*i)++]; return true; }); }
	ref<IntStream> distinct() { auto up = pull; auto seen = std::make_shared<std::vector<int32_t>>(); return from([up, seen](int32_t& out) { while (up(out)) if (std::find(seen->begin(), seen->end(), out) == seen->end()) { seen->push_back(out); return true; } return false; }); }
	int32_t sum() { int32_t s = 0, x; while (pull(x)) s += x; return s; }
	int64_t count() { int64_t n = 0; int32_t x; while (pull(x)) ++n; return n; }
	ref<OptionalInt> max() { auto r = std::make_shared<OptionalInt>(); int32_t x; while (pull(x)) if (!r->v || x > *r->v) r->v = x; return r; }
	ref<OptionalInt> min() { auto r = std::make_shared<OptionalInt>(); int32_t x; while (pull(x)) if (!r->v || x < *r->v) r->v = x; return r; }
	ref<OptionalDouble> average() { auto r = std::make_shared<OptionalDouble>(); int64_t s = 0, n = 0; int32_t x; while (pull(x)) { s += x; ++n; } if (n) r->v = (double)s / (double)n; return r; }
	ref<OptionalInt> findFirst() { auto r = std::make_shared<OptionalInt>(); int32_t x; if (pull(x)) r->v = x; return r; }
	int32_t reduce(int32_t id, ref<IntBinaryOperator> f) { int32_t acc = id, x; while (pull(x)) acc = f->applyAsInt(acc, x); return acc; }
	bool anyMatch(ref<IntPredicate> p) { int32_t x; while (pull(x)) if (p->test(x)) return true; return false; }
	bool allMatch(ref<IntPredicate> p) { int32_t x; while (pull(x)) if (!p->test(x)) return false; return true; }
	bool noneMatch(ref<IntPredicate> p) { return !anyMatch(p); }
	void forEach(ref<IntConsumer> c) { int32_t x; while (pull(x)) c->accept(x); }
	ref<Array<int32_t>> toArray() { std::vector<int32_t> v; int32_t x; while (pull(x)) v.push_back(x); auto r = new_array<int32_t>((int32_t)v.size()); r->a = v; return r; }
};
inline ref<IntStream> Stream::mapToInt(ref<ToIntFunction> f) { auto up = pull; return IntStream::from([up, f](int32_t& out) { O x; if (!up(x)) return false; out = f->applyAsInt(x); return true; }); }
inline ref<IntStream> Arrays::stream(const ref<Array<int32_t>>& a) { auto i = std::make_shared<int32_t>(0); auto arr = nn(a); return IntStream::from([arr, i](int32_t& out) { if (*i >= arr->length) return false; out = arr->a[(*i)++]; return true; }); }
inline ref<IntStream> String::chars() { auto me = std::static_pointer_cast<String>(shared_from_this()); auto i = std::make_shared<size_t>(0); return IntStream::from([me, i](int32_t& out) { if (*i >= me->s.size()) return false; out = me->s[(*i)++]; return true; }); }

struct Collector : Object {
	std::function<O(Stream&)> run;
};
inline O Stream::collect(ref<Collector> c) { return c->run(*this); }
struct Collectors {
	static ref<Collector> mk(std::function<O(Stream&)> f) { auto c = std::make_shared<Collector>(); c->run = std::move(f); return c; }
	static ref<Collector> toList() { return mk([](Stream& s) -> O { auto l = std::make_shared<ArrayList>(); l->v = s.drain(); return l; }); }
	static ref<Collector> toSet() { return mk([](Stream& s) -> O { auto h = make_set(); for (auto& x : s.drain()) h->add(x); return h; }); }
	static ref<Collector> joining() { return joining(lit(u""), lit(u""), lit(u"")); }
	static ref<Collector> joining(ref<String> sep) { return joining(sep, lit(u""), lit(u"")); }
	static ref<Collector> joining(ref<String> sep, ref<String> pre, ref<String> post) {
		return mk([sep, pre, post](Stream& s) -> O { std::u16string r = pre->s; bool first = true; for (auto& x : s.drain()) { if (!first) r += sep->s; first = false; r += sv(x); } return str(r + post->s); });
	}
	static ref<Collector> counting() { return mk([](Stream& s) -> O { return box((int64_t)s.drain().size()); }); }
	static ref<Collector> groupingBy(ref<Function> key) {
		return mk([key](Stream& s) -> O { auto m = make<HashMap>(); for (auto& x : s.drain()) { O k = key->apply(x); auto l = cast<List>(m->get(k)); if (!l) { l = std::make_shared<ArrayList>(); m->put(k, l); } l->add(x); } return m; });
	}
	static ref<Collector> groupingBy(ref<Function> key, ref<Collector> down) {
		return mk([key, down](Stream& s) -> O {
			auto groups = make<HashMap>();
			for (auto& x : s.drain()) { O k = key->apply(x); auto l = cast<ArrayList>(groups->get(k)); if (!l) { l = std::make_shared<ArrayList>(); groups->put(k, l); } l->add(x); }
			auto m = make<HashMap>();
			for (auto& e : groups->entries()) { auto l = cast<ArrayList>(e->value); m->put(e->key, down->run(*l->stream())); }
			return m;
		});
	}
	static ref<Collector> toMap(ref<Function> k, ref<Function> v) {
		return mk([k, v](Stream& s) -> O { auto m = make<HashMap>(); for (auto& x : s.drain()) { O key = k->apply(x); if (m->containsKey(key)) raise_msg(std::make_shared<IllegalStateException>(), u"Duplicate key " + sv(key)); m->put(key, v->apply(x)); } return m; });
	}
	static ref<Collector> summingInt(ref<ToIntFunction> f) { return mk([f](Stream& s) -> O { int32_t t = 0; for (auto& x : s.drain()) t += f->applyAsInt(x); return box(t); }); }
	static ref<Set> make_set() { return make<HashSet>(); }
};

// ------------------------------------------------------------------------------------------- String.format
// %d %s %c %b %x %f %e %n %%, with flags (-, 0, +, ',', space) and width/precision. %f rounds half up on
// the shortest decimal digits, as java.util.Formatter does (not on the exact binary value, as printf does).
inline std::string fixed_half_up(double d, int prec) {
	if (d != d) return "NaN";
	if (std::isinf(d)) return d > 0 ? "Infinity" : "-Infinity";
	bool neg = std::signbit(d);
	double a = std::fabs(d);
	char buf[64];
	auto r = std::to_chars(buf, buf + sizeof buf, a, std::chars_format::scientific);
	std::string s(buf, r.ptr);
	size_t e = s.find('e');
	int exp = std::atoi(s.c_str() + e + 1);
	std::string dg;
	for (size_t i = 0; i < e; ++i) if (s[i] != '.') dg += s[i];
	// digits dg represent 0.dg * 10^(exp+1); the decimal point sits after (exp+1) digits
	int point = exp + 1;
	if (a == 0) { dg = "0"; point = 1; }
	int keep = point + prec;                       // how many digits of dg stay
	std::string kept;
	if (keep < 0) { kept = ""; }
	else {
		kept = dg.substr(0, std::min<size_t>(dg.size(), keep));
		while ((int)kept.size() < keep) kept += '0';
		if (keep < (int)dg.size() && dg[keep] >= '5') {   // half up
			int i = (int)kept.size() - 1;
			while (i >= 0 && kept[i] == '9') { kept[i] = '0'; --i; }
			if (i >= 0) kept[i]++; else { kept = "1" + kept; ++point; }
		}
	}
	if (keep == 0 && !dg.empty() && dg[0] >= '5') { kept = "1"; ++point; }
	std::string ip, fp;
	if (point <= 0) { ip = "0"; fp = std::string(-point, '0') + kept; }
	else { ip = kept.substr(0, std::min<size_t>(kept.size(), point)); while ((int)ip.size() < point) ip += '0'; fp = (int)kept.size() > point ? kept.substr(point) : ""; }
	while ((int)fp.size() < prec) fp += '0';
	if ((int)fp.size() > prec) fp = fp.substr(0, prec);
	size_t nz = ip.find_first_not_of('0');
	ip = nz == std::string::npos ? "0" : ip.substr(nz);
	std::string out = ip + (prec > 0 ? "." + fp : "");
	bool zero = out.find_first_not_of("0.") == std::string::npos;
	return (neg && !zero ? "-" : "") + out;
}
struct FmtArg {
	enum K { I, F, S, C, B } k; int64_t i = 0; double f = 0; std::u16string s; char16_t c = 0; bool b = false; bool isnull = false;
};
inline FmtArg fa(int32_t v) { FmtArg a; a.k = FmtArg::I; a.i = v; return a; }
inline FmtArg fa(int64_t v) { FmtArg a; a.k = FmtArg::I; a.i = v; return a; }
inline FmtArg fa(int16_t v) { return fa((int32_t)v); }
inline FmtArg fa(int8_t v) { return fa((int32_t)v); }
inline FmtArg fa(double v) { FmtArg a; a.k = FmtArg::F; a.f = v; return a; }
inline FmtArg fa(float v) { return fa((double)v); }
inline FmtArg fa(char16_t v) { FmtArg a; a.k = FmtArg::C; a.c = v; return a; }
inline FmtArg fa(bool v) { FmtArg a; a.k = FmtArg::B; a.b = v; return a; }
template <class T> FmtArg fa(const ref<T>& o) {
	FmtArg a; a.k = FmtArg::S; a.isnull = !o; a.s = sv(o);
	if (auto i = as<Integer>(o)) { a.k = FmtArg::I; a.i = i->value; }
	else if (auto l = as<Long>(o)) { a.k = FmtArg::I; a.i = l->value; }
	else if (auto d = as<Double>(o)) { a.k = FmtArg::F; a.f = d->value; }
	else if (auto fl = as<Float>(o)) { a.k = FmtArg::F; a.f = fl->value; }
	else if (auto c = as<Character>(o)) { a.k = FmtArg::C; a.c = c->value; }
	else if (auto b = as<Boolean>(o)) { a.k = FmtArg::B; a.b = b->value; }
	return a;
}
inline std::u16string format_args(const std::u16string& f, const std::vector<FmtArg>& args) {
	std::u16string out;
	size_t ai = 0;
	for (size_t i = 0; i < f.size(); ++i) {
		if (f[i] != '%') { out += f[i]; continue; }
		++i;
		std::string flags;
		while (i < f.size() && std::strchr("-0+, #", (char)f[i]) && f[i] < 128) flags += (char)f[i++];
		int width = 0, prec = -1;
		while (i < f.size() && f[i] >= '0' && f[i] <= '9') width = width * 10 + (f[i++] - '0');
		if (i < f.size() && f[i] == '.') { ++i; prec = 0; while (i < f.size() && f[i] >= '0' && f[i] <= '9') prec = prec * 10 + (f[i++] - '0'); }
		char conv = i < f.size() ? (char)f[i] : 's';
		if (conv == 'n') { out += u"\n"; continue; }
		if (conv == '%') { out += u"%"; continue; }
		FmtArg a = ai < args.size() ? args[ai++] : FmtArg();
		std::string body;
		bool upper = conv >= 'A' && conv <= 'Z';
		char lc = upper ? conv + 32 : conv;
		auto group = [&](std::string digits) {
			if (flags.find(',') == std::string::npos) return digits;
			std::string sign = (!digits.empty() && digits[0] == '-') ? "-" : "";
			if (!sign.empty()) digits = digits.substr(1);
			size_t dot = digits.find('.');
			std::string ip = digits.substr(0, dot), rest = dot == std::string::npos ? "" : digits.substr(dot);
			std::string g;
			for (size_t k = 0; k < ip.size(); ++k) { if (k && (ip.size() - k) % 3 == 0) g += ','; g += ip[k]; }
			return sign + g + rest;
		};
		if (lc == 'd') { body = group(std::to_string(a.i)); if (flags.find('+') != std::string::npos && a.i >= 0) body = "+" + body; else if (flags.find(' ') != std::string::npos && a.i >= 0) body = " " + body; }
		else if (lc == 'x') { body = utf8(to_radix(a.i < 0 && a.k == FmtArg::I && a.i >= IntegerS::MIN_VALUE ? (uint64_t)(uint32_t)a.i : (uint64_t)a.i, 4)); }
		else if (lc == 'o') { body = utf8(to_radix((uint64_t)(uint32_t)a.i, 3)); }
		else if (lc == 'f') { body = group(fixed_half_up(a.f, prec < 0 ? 6 : prec)); if (flags.find('+') != std::string::npos && !std::signbit(a.f)) body = "+" + body; }
		else if (lc == 'e') { char b[64]; std::snprintf(b, sizeof b, "%.*e", prec < 0 ? 6 : prec, a.f); body = b; }
		else if (lc == 'c') { body = utf8(std::u16string(1, a.c)); }
		else if (lc == 'b') { body = a.k == FmtArg::B ? (a.b ? "true" : "false") : (a.isnull ? "false" : "true"); }
		else { std::u16string s = a.k == FmtArg::I ? sv(a.i) : a.k == FmtArg::F ? sv(a.f) : a.k == FmtArg::C ? sv(a.c) : a.k == FmtArg::B ? sv(a.b) : a.s; if (prec >= 0 && (int)s.size() > prec) s = s.substr(0, prec); body = utf8(s); }
		if (upper) for (auto& ch : body) ch = (char)std::toupper((unsigned char)ch);
		int len = 0;
		for (unsigned char ch : body) if ((ch & 0xC0) != 0x80) ++len;
		if (len < width) {
			if (flags.find('-') != std::string::npos) body += std::string(width - len, ' ');
			else if (flags.find('0') != std::string::npos && lc != 's') { size_t at = (!body.empty() && (body[0] == '-' || body[0] == '+')) ? 1 : 0; body.insert(at, std::string(width - len, '0')); }
			else body = std::string(width - len, ' ') + body;
		}
		std::u16string u;
		for (size_t k = 0; k < body.size();) {         // back from UTF-8
			unsigned char c0 = body[k];
			if (c0 < 0x80) { u += c0; ++k; }
			else if (c0 < 0xE0) { u += (char16_t)(((c0 & 31) << 6) | (body[k + 1] & 63)); k += 2; }
			else { u += (char16_t)(((c0 & 15) << 12) | ((body[k + 1] & 63) << 6) | (body[k + 2] & 63)); k += 3; }
		}
		out += u;
	}
	return out;
}
template <class... A> ref<String> String::format(const ref<String>& f, A... a) {
	std::vector<FmtArg> args{fa(a)...};
	return str(format_args(nn(f)->s, args));
}

// String.split for the patterns that are literal text (a single character, or no regular-expression
// metacharacters); any other pattern is UnsupportedOperationException, said in the ledger
inline ref<Array<ref<String>>> String::split(const ref<String>& re) {
	std::u16string sep = nn(re)->s;
	if (sep.size() == 2 && sep[0] == '\\') sep = sep.substr(1);
	else for (char16_t c : sep) if (std::u16string(u".$|()[{^?*+\\").find(c) != std::u16string::npos && sep.size() > 1)
		raise_new<UnsupportedOperationException>(u"String.split with a regular expression");
	if (sep.size() == 1 && std::u16string(u".$|()[{^?*+\\").find(sep[0]) != std::u16string::npos && nn(re)->s.size() == 1)
		raise_new<UnsupportedOperationException>(u"String.split with a regular expression");
	std::vector<ref<String>> parts;
	if (s.empty()) { auto r = new_array<ref<String>>(1); (*r)[0] = str(u""); return r; }
	size_t i = 0;
	for (;;) {
		size_t p = sep.empty() ? (i + 1 <= s.size() ? i + 1 : std::u16string::npos) : s.find(sep, i);
		if (p == std::u16string::npos || (sep.empty() && p >= s.size())) { parts.push_back(str(s.substr(i))); break; }
		parts.push_back(str(s.substr(i, p - i)));
		i = p + sep.size();
	}
	while (!parts.empty() && parts.back()->s.empty()) parts.pop_back();   // trailing empty strings go
	auto r = new_array<ref<String>>((int32_t)parts.size());
	for (size_t k = 0; k < parts.size(); ++k) r->a[k] = parts[k];
	return r;
}

// ------------------------------------------------------------------------------------------- enums
struct Enum : virtual Object {
	ref<String> name_;
	int32_t ordinal_ = 0;
	const char16_t* jclass_() const override { return u"java.lang.Enum"; }
	void ctor_(ref<String> n, int32_t o) { name_ = n; ordinal_ = o; }
	ref<String> name() { return name_; }
	int32_t ordinal() { return ordinal_; }
	ref<String> toString() override { return name_; }
	int32_t compareTo(O o) { return ordinal_ - cast<Enum>(o)->ordinal_; }
	int32_t hashCode() override { return Object::hashCode(); }
};
inline int32_t natural_enum(const O& a, const O& b) { if (auto e = as<Enum>(a)) return e->compareTo(b); return INT32_MIN; }

// ------------------------------------------------------------------------------------------- finally
// try { ... } finally { f } where f cannot leave by a jump: f runs however the block is left
template <class F> struct Finally {
	F f;
	int pending = std::uncaught_exceptions();
	explicit Finally(F x) : f(std::move(x)) {}
	~Finally() noexcept(false) {
		if (std::uncaught_exceptions() > pending) { try { f(); } catch (...) {} }
		else f();
	}
};

inline int run_main(void (*m)(ref<Array<ref<String>>>), int argc, char** argv) {
	auto args = new_array<ref<String>>(argc > 1 ? argc - 1 : 0);
	for (int i = 1; i < argc; ++i) (*args)[i - 1] = str(u16(argv[i]));
	try {
		m(args);
	} catch (JavaThrow& t) {
		std::fflush(stdout);
		std::fprintf(stderr, "Exception in thread \"main\" %s\n", utf8(sv(t.t)).c_str());
		return 1;
	}
	std::fflush(stdout);
	return 0;
}

}  // namespace jrt
#endif
