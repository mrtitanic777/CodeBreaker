#ifndef DATE_H
#define DATE_H
// java.util.Date: milliseconds since the epoch (the C library's clock, to the second).
#include <ctime>
#include <string>
#include "JavaString.h"

class Date {
public:
	long long ms;
	Date() : ms((long long)std::time(0) * 1000LL) {}
	explicit Date(long long t) : ms(t) {}
	virtual ~Date() {}
	virtual long long getTime() { return ms; }
	virtual JavaString toString() {
		std::time_t t = (std::time_t)(ms / 1000);
		char b[64];
		std::strftime(b, sizeof(b), "%a %b %d %H:%M:%S %Y", std::localtime(&t));
		return JavaString(b);
	}
};
#endif // DATE_H
