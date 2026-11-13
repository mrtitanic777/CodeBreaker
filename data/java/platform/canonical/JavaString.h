#ifndef JAVASTRING_H
#define JAVASTRING_H
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include "JArray.h"
#include "Regex.h"

// Java's Double.toString / Float.toString -- what `"" + d`, StringBuilder.append(d) and String.valueOf(d)
// print: the SHORTEST decimal that reads back to the same double (float), always with a fraction digit,
// plain for 1e-3 <= |v| < 1e7 ("100.25", "0.001", "1000000.0"), else "d.dddE<exp>" ("1.0E7", "1.5E-4");
// "NaN", "Infinity", "-0.0". J2X-BUGFIX java-number-text: C++ streams print 0.0 as "0" and 1000000.0 as
// "1e+06" (Vec3D.toString(), every float in a debug/chat string); found by the behavioural judge.
#if defined(_MSC_VER)
#define J2X_FMT(buf, ...) sprintf_s(buf, sizeof(buf), __VA_ARGS__)     // MSVC: no C4996 for sprintf
#else
#define J2X_FMT(buf, ...) std::sprintf(buf, __VA_ARGS__)
#endif
inline std::string j2xNumberText(double v, bool single) {
	if (v != v) return "NaN";
	if (v - v != v - v) return v > 0 ? "Infinity" : "-Infinity";
	if (v == 0) return (1.0 / v < 0) ? "-0.0" : "0.0";
	char buf[40];
	int p = 1;
	for (; p <= 17; ++p) {                              // the fewest significant digits that read back the same
		J2X_FMT(buf, "%.*e", p - 1, v);
		double back = std::strtod(buf, 0);
		if (single ? ((float)back == (float)v) : (back == v)) break;
	}
	std::string s(buf);                                 // [-]d[.ddd]e[+-]XX
	bool neg = s[0] == '-';
	if (neg) s = s.substr(1);
	std::string::size_type e = s.find('e');
	int exp10 = std::atoi(s.c_str() + e + 1);
	std::string digits;
	for (std::string::size_type i = 0; i < e; ++i) if (s[i] != '.') digits += s[i];
	while (digits.size() > 1 && digits[digits.size() - 1] == '0') digits.erase(digits.size() - 1);
	std::string out;
	double a = neg ? -v : v;
	if (a >= 1e-3 && a < 1e7) {
		if (exp10 >= 0) {
			std::string whole = digits.substr(0, (std::string::size_type)exp10 + 1 < digits.size() ? exp10 + 1 : digits.size());
			while ((int)whole.size() < exp10 + 1) whole += '0';
			std::string frac = (std::string::size_type)exp10 + 1 < digits.size() ? digits.substr(exp10 + 1) : "0";
			out = whole + "." + frac;
		} else {
			out = "0." + std::string((std::string::size_type)(-exp10 - 1), '0') + digits;
		}
	} else {
		char e10[16];
		J2X_FMT(e10, "E%d", exp10);
		out = digits.substr(0, 1) + "." + (digits.size() > 1 ? digits.substr(1) : std::string("0")) + e10;
	}
	return neg ? "-" + out : out;
}
struct JObject;                                  // (String)o unboxes through the ctor below (JavaRT.h)

class JavaString : public std::string {
	static char _lc(char c) { return (char)((c >= 'A' && c <= 'Z') ? c + 32 : c); }
	static char _uc(char c) { return (char)((c >= 'a' && c <= 'z') ? c - 32 : c); }
	// Java null. A String that was never given a value (a field, a `new String[n]` slot) or was given
	// null is null; "" is not -- `s == null` tells them apart, as Java does (NBTTagString threw on every
	// "" because both were the empty string). Only an EMPTY string can be null, so any text written into
	// it through the inherited std::string mutators (+=, append) makes it non-null without a hook.
	bool _null;
public:
	JavaString() : _null(true) {}
	JavaString(const char* s) : std::string(s ? s : ""), _null(!s) {}
	JavaString(const std::string& s) : std::string(s), _null(false) {}
	JavaString(const JObject& o);                // defined in JavaRT.h, after JObject
	bool isNull() const { return _null && empty(); }
	// new String(byte[] [,charset]) / new String(byte[], off, len [,charset]): build from a byte array
	// (NBT keys, packet strings). Charset is ignored -- the game's text is UTF-8/ASCII and the bytes copy
	// verbatim. Trailing `const char* = 0` swallows the "UTF-8" argument without a separate overload.
	JavaString(const JArray<unsigned char>& b, const char* = 0) : _null(false) {
		for (int i = 0; i < b.length; ++i) push_back((char)b[i]);
	}
	JavaString(const JArray<unsigned char>& b, int off, int len, const char* = 0) : _null(false) {
		for (int i = 0; i < len; ++i) push_back((char)b[off + i]);
	}

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
	// Java's String is UTF-16 and these are defined on its chars: compareTo is the difference of the first
	// chars that differ (else of the lengths), hashCode is s[0]*31^(n-1) + ... FIXED BUG string-compare:
	// compareTo gave std::string's -1/0/1, and hashCode summed signed bytes, wrong past ASCII ("§cRed").
	// The text here is UTF-8, decoded to UTF-16 units; a byte that is not valid UTF-8 counts as itself.
	static void _utf16(const std::string& s, std::vector<unsigned short>& u) {
		u.clear();
		u.reserve(s.size());
		for (size_type i = 0; i < s.size(); ) {
			unsigned c = (unsigned char)s[i];
			int n = c >= 0xF0 && c < 0xF8 ? 3 : c >= 0xE0 && c < 0xF0 ? 2 : c >= 0xC0 && c < 0xE0 ? 1 : 0;
			bool good = n > 0;                               // the continuation bytes are checked below
			unsigned cp = c & (0x3F >> n);
			for (int k = 1; good && k <= n; ++k) {
				if (i + k >= s.size() || ((unsigned char)s[i + k] & 0xC0) != 0x80) good = false;
				else cp = (cp << 6) | ((unsigned char)s[i + k] & 0x3F);
			}
			if (!good) { u.push_back((unsigned short)c); ++i; continue; }
			if (cp >= 0x10000) {
				cp -= 0x10000;
				u.push_back((unsigned short)(0xD800 + (cp >> 10)));
				u.push_back((unsigned short)(0xDC00 + (cp & 0x3FF)));
			} else {
				u.push_back((unsigned short)cp);
			}
			i += n + 1;
		}
	}
	int compareTo(const std::string& o) const {
		std::vector<unsigned short> a, b;
		_utf16(*this, a);
		_utf16(o, b);
		size_type n = a.size() < b.size() ? a.size() : b.size();
		for (size_type i = 0; i < n; ++i) if (a[i] != b[i]) return (int)a[i] - (int)b[i];
		return (int)a.size() - (int)b.size();
	}
	int hashCode() const {
		std::vector<unsigned short> u;
		_utf16(*this, u);
		unsigned h = 0;                                      // unsigned: Java's int overflow wraps
		for (size_type i = 0; i < u.size(); ++i) h = h * 31u + u[i];
		return (int)h;
	}
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
	// Copy-out buffers for toCharArray/getBytes. JArray is NON-owning (shallow copy, no destructor), so a
	// fresh `new T[n]` per call leaked every call -- fatal for per-frame callers (FontRenderer). Instead the
	// copy lands in a bounded ring of owned slots: a slot is freed when the ring wraps, so memory stays
	// bounded at _RING slots. Java's copy semantics hold (each call gets its own buffer); a result retained
	// across more than _RING later calls would dangle -- callers iterate locally, so the ring is generous.
	// Each buffer carries a trailing NUL so a site that still treats the result as a C string stays safe.
	// Not thread-safe: string-to-array copies run on the game thread only.
	enum { _RING = 256 };
	template<class T> static JArray<T> _copyOut(const std::string& s) {
		static T* ring[_RING];                       // zero-initialized (static storage)
		static int next = 0;
		int slot = next; next = (next + 1) % _RING;
		delete[] ring[slot];
		int n = (int)s.size();
		ring[slot] = new T[n + 1]();
		for (int i = 0; i < n; ++i) ring[slot][i] = (T)s[(size_type)i];
		return JArray<T>(ring[slot], n);
	}
	// java.lang.String.toCharArray() -> char[] (JArray<unsigned char>: Java's char is unsigned), a COPY --
	// matches Java's return type so `char[] c = s.toCharArray()` (FontRenderer glyph iteration) assigns.
	JArray<unsigned char> toCharArray() const { return _copyOut<unsigned char>(*this); }
	// java.lang.String.getBytes([charset]) -> a COPY of the bytes as byte[] (JArray<unsigned char>),
	// matching Java's return type so `byte[] b = s.getBytes("UTF-8")` (NBT/packets) assigns + `b.length`.
	JArray<unsigned char> getBytes(...) const { return _copyOut<unsigned char>(*this); }
	// String.replaceAll / replaceFirst / matches / split take Java regular expressions (Regex.h). FIXED BUG
	// replaceall-noop: replaceAll returned the string unchanged, so SoundPool's sound names kept their '/'.
	// In the replacement, $n is group n and \x is x, as in Java.
	JavaString replaceAll(const std::string& re, const std::string& rep) const { return _replace(re, rep, true); }
	JavaString replaceFirst(const std::string& re, const std::string& rep) const { return _replace(re, rep, false); }
	bool matches(const std::string& re) const { std::vector<int> g; return J2xRegex(re).matchAll(*this, g); }
	// String.split(regex): as Java, trailing empty strings are dropped ("a:b:".split(":") is [a, b]) and a
	// zero-width match at the start makes no leading empty one; "".split(x) is [""].
	JArray<JavaString> split(const std::string& re) const {
		std::vector<JavaString> parts;
		if (empty()) {
			parts.push_back(JavaString(""));
		} else {
			J2xRegex rx(re);
			std::vector<int> g;
			size_type start = 0;
			int from = 0;
			while (from <= (int)size()) {
				int st = rx.find(*this, from, g);
				if (st < 0) break;
				int en = g[1];
				if (en == 0) { from = 1; continue; }
				parts.push_back(JavaString(substr(start, (size_type)st - start)));
				start = (size_type)en;
				from = en > st ? en : st + 1;
			}
			parts.push_back(JavaString(substr(start)));
			while (!parts.empty() && parts.back().empty()) parts.pop_back();
		}
		JArray<JavaString> r((int)parts.size());
		for (size_type i = 0; i < parts.size(); ++i) r[(int)i] = parts[i];
		return r;
	}
	JavaString _replace(const std::string& re, const std::string& rep, bool all) const {
		J2xRegex rx(re);
		std::vector<int> g;
		std::string out;
		size_type last = 0;
		int from = 0;
		while (from <= (int)size()) {
			int st = rx.find(*this, from, g);
			if (st < 0) break;
			out.append(*this, last, (size_type)st - last);
			for (size_type i = 0; i < rep.size(); ++i) {
				char c = rep[i];
				if (c == '\\' && i + 1 < rep.size()) {
					out += rep[++i];
				} else if (c == '$' && i + 1 < rep.size() && rep[i + 1] >= '0' && rep[i + 1] <= '9') {
					int n = rep[++i] - '0';
					if (n <= rx.ngroups && g[2 * n] >= 0) out.append(*this, (size_type)g[2 * n], (size_type)(g[2 * n + 1] - g[2 * n]));
				} else {
					out += c;
				}
			}
			if (g[1] == st) {                                // an empty match: keep the next char, step past it
				if (st < (int)size()) out += (*this)[(size_type)st];
				last = (size_type)st + 1;
				from = st + 1;
			} else {
				last = (size_type)g[1];
				from = g[1];
			}
			if (!all) break;
		}
		if (last < size()) out.append(*this, last, npos);
		return JavaString(out);
	}
	// Java `someString == null` / `!= null` (NULL is an integer 0 here, hence long long)
	bool operator==(long long v) const { return v == 0 && isNull(); }
	bool operator!=(long long v) const { return !(v == 0 && isNull()); }
	// Java string concatenation `s + <primitive>` (the JCat lowering can't reach every site, e.g. inside a
	// ternary). String-first only, and numeric-only -- no std::string/char*/JavaString overloads, so the
	// existing free operator+ still resolves string+string with no ambiguity (numbers can't convert to it).
	JavaString operator+(int v) const { std::ostringstream o; o << v; return JavaString((const std::string&)*this + o.str()); }
	JavaString operator+(long v) const { std::ostringstream o; o << v; return JavaString((const std::string&)*this + o.str()); }
	JavaString operator+(long long v) const { std::ostringstream o; o << v; return JavaString((const std::string&)*this + o.str()); }
	JavaString operator+(unsigned v) const { std::ostringstream o; o << v; return JavaString((const std::string&)*this + o.str()); }
	JavaString operator+(double v) const { return JavaString((const std::string&)*this + j2xNumberText(v, false)); }
	// a Java byte (signed char) prints its number, as Java does ("" + b), not a character
	JavaString operator+(signed char v) const { std::ostringstream o; o << (int)v; return JavaString((const std::string&)*this + o.str()); }
	JavaString operator+(float v) const { return JavaString((const std::string&)*this + j2xNumberText(v, true)); }
	JavaString operator+(char v) const { return JavaString((const std::string&)*this + std::string(1, v)); }
	// a Java char (unsigned char) appends the character, not its number (it would promote to operator+(int))
	JavaString operator+(unsigned char v) const { return JavaString((const std::string&)*this + std::string(1, (char)v)); }
	// NOTE: deliberately NO operator+(bool)/(const char*)/(std::string) -- a `const char*` literal converts
	// to bool, which would make `s + "literal"` ambiguous against the free std::operator+(string,const char*).
	// String concatenation with a bool is handled by the JCat lowering (which has its own bool overload).
};
// a null String prints "null", as Java's concatenation does ("" + s)
inline std::ostream& operator<<(std::ostream& o, const JavaString& s) {
	return s.isNull() ? (o << "null") : (o << (const std::string&)s);
}
#endif // JAVASTRING_H
