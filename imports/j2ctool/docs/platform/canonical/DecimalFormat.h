#ifndef DECIMALFORMAT_H
#define DECIMALFORMAT_H
// Canonical java.text.DecimalFormat -- REAL pattern formatter (was a stub -> blank stats numbers).
// Reads the fixed-fraction count from the pattern (e.g. "########0.00" -> 2 decimals) and whether the
// pattern groups (contains ','). Fixed decimals: no trailing-zero trim (Java DecimalFormat keeps them).
#include <cstdio>
#include <string>
#include "JavaString.h"
#include "NumberFormat.h"

class DecimalFormat : public NumberFormat {
public:
	DecimalFormat(...) { _maxFrac = 2; _grouping = false; }
	DecimalFormat(const JavaString& pattern, ...) {
		std::string p = (std::string)pattern;
		std::string::size_type dot = p.find('.');
		_maxFrac = (dot == std::string::npos) ? 0 : (int)(p.size() - dot - 1);
		_grouping = (p.find(',') != std::string::npos);
	}
	virtual JavaString format(double v) {                // fixed fraction count (keep trailing zeros)
		char buf[64];
		sprintf(buf, "%.*f", _maxFrac, v);
		std::string s(buf);
		if (_grouping) _group(s);
		return JavaString(s);
	}
	virtual JavaString format(long long v) { return format((double)v); }
	virtual JavaString format(int v) { return format((double)v); }
};
#endif // DECIMALFORMAT_H
