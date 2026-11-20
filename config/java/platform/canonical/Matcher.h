#ifndef MATCHER_H
#define MATCHER_H
// java.util.regex.Matcher over J2xRegex (Regex.h). Was a stub whose matches() was always false, so every
// JsonNumber failed to construct and chunk-file names never parsed.
#include "JavaString.h"
#include "Regex.h"

class Matcher {
	J2xRegex _re;
	JavaString _s;
	std::vector<int> _g;
	int _from;
	bool _hit;
public:
	Matcher(const J2xRegex& re, const JavaString& s) : _re(re), _s(s), _from(0), _hit(false) {}
	bool matches() { _hit = _re.matchAll(_s, _g); return _hit; }
	bool lookingAt() { _hit = _re.prefix(_s, _g) >= 0; return _hit; }
	bool find() {
		if (_from > (int)_s.size()) return _hit = false;
		int st = _re.find(_s, _from, _g);
		_hit = st >= 0;
		if (_hit) _from = _g[1] > st ? _g[1] : st + 1;     // past an empty match, as Java does
		return _hit;
	}
	int groupCount() const { return _re.ngroups; }
	JavaString group(int i) const {
		if (!_hit || i < 0 || i > _re.ngroups || _g[2 * i] < 0) return JavaString();   // Java: null
		return JavaString(_s.substr((size_t)_g[2 * i], (size_t)(_g[2 * i + 1] - _g[2 * i])));
	}
	JavaString group() const { return group(0); }
	int start() const { return _hit ? _g[0] : -1; }
	int end() const { return _hit ? _g[1] : -1; }
	Matcher* reset() { _from = 0; _hit = false; return this; }
};
#endif // MATCHER_H
