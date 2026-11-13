#ifndef PATTERN_H
#define PATTERN_H
// java.util.regex.Pattern over J2xRegex (Regex.h); see there for what is supported.
#include "Matcher.h"

class Pattern {
	JavaString _p;
	J2xRegex _re;
public:
	explicit Pattern(const JavaString& p) : _p(p), _re(p) {}
	static Pattern* compile(const JavaString& p) { return new Pattern(p); }
	static Pattern* compile(const JavaString& p, int) { return new Pattern(p); }   // flags: not supported
	static bool matches(const JavaString& p, const JavaString& s) { return Pattern(p).matcher(s)->matches(); }
	Matcher* matcher(const JavaString& s) const { return new Matcher(_re, s); }
	JavaString pattern() const { return _p; }
	JavaString toString() const { return _p; }
	JArray<JavaString> split(const JavaString& s) const { return s.split(_p); }
};
#endif // PATTERN_H
