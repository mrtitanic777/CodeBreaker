#ifndef NUMBERFORMAT_H
#define NUMBERFORMAT_H
// Canonical java.text.NumberFormat -- REAL number formatting (was a stub whose format() returned "" ->
// blank values on the Statistics screen). getIntegerInstance() = no decimals + thousands grouping.
#include <cstdio>
#include <string>
#include "JavaString.h"

class NumberFormat {
protected:
	int _maxFrac;
	bool _grouping;
	static void _group(std::string& s) {                 // insert ',' every 3 digits in the integer part
		std::string::size_type dot = s.find('.');
		std::string::size_type end = (dot == std::string::npos) ? s.size() : dot;
		std::string::size_type start = (s.size() && (s[0] == '-' || s[0] == '+')) ? 1 : 0;
		for (std::string::size_type i = (end >= 3 ? end - 3 : start); i > start; i -= 3) {
			s.insert(i, ",");
			if (i < 3) break;
		}
	}
public:
	NumberFormat(...) : _maxFrac(3), _grouping(false) {}
	virtual ~NumberFormat() {}
	virtual JavaString format(double v) {
		char buf[64];
		sprintf(buf, "%.*f", _maxFrac, v);
		std::string s(buf);
		if (_maxFrac > 0) {                              // trim trailing zeros / dot (Java NumberFormat default)
			std::string::size_type dot = s.find('.');
			if (dot != std::string::npos) {
				std::string::size_type last = s.find_last_not_of('0');
				if (last == dot) --last;
				s.erase(last + 1);
			}
		}
		if (_grouping) _group(s);
		return JavaString(s);
	}
	virtual JavaString format(long long v) { char b[32]; sprintf(b, "%lld", v); std::string s(b); if (_grouping) _group(s); return JavaString(s); }
	virtual JavaString format(int v) { return format((long long)v); }
	static NumberFormat* getIntegerInstance(...) { NumberFormat* f = new NumberFormat(); f->_maxFrac = 0; f->_grouping = true; return f; }
	static NumberFormat* getInstance(...)        { return new NumberFormat(); }
	static NumberFormat* getNumberInstance(...)  { return new NumberFormat(); }
};
#endif // NUMBERFORMAT_H
