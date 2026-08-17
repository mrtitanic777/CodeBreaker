#ifndef JAVASTRING_H
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
