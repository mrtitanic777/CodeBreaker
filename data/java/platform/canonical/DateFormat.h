#ifndef DATEFORMAT_H
#define DATEFORMAT_H
// java.text.DateFormat / SimpleDateFormat: the pattern letters a game uses for timestamps and file names
// (y M d H h m s S a E, quoted text, everything else literal), in local time.
#include <ctime>
#include <cstdio>
#include <string>
#include "JavaString.h"
#include "Date.h"

class DateFormat {
protected:
	std::string _pattern;
public:
	DateFormat() : _pattern("yyyy-MM-dd HH:mm:ss") {}
	explicit DateFormat(const std::string& p) : _pattern(p) {}
	virtual ~DateFormat() {}
	virtual JavaString format(Date* d) { return format(d ? d->getTime() : (long long)std::time(0) * 1000LL); }
	JavaString format(long long ms) {
		std::time_t t = (std::time_t)(ms / 1000);
		std::tm tm = *std::localtime(&t);
		static const char* months[] = {"January", "February", "March", "April", "May", "June", "July",
		                               "August", "September", "October", "November", "December"};
		static const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
		std::string out;
		const std::string& p = _pattern;
		for (std::size_t i = 0; i < p.size();) {
			char c = p[i];
			if (c == '\'') {                                       // quoted literal ('' is a quote)
				std::size_t j = p.find('\'', i + 1);
				if (j == i + 1) { out += '\''; i += 2; continue; }
				out += p.substr(i + 1, (j == std::string::npos ? p.size() : j) - i - 1);
				i = j == std::string::npos ? p.size() : j + 1;
				continue;
			}
			if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) { out += c; ++i; continue; }
			std::size_t n = 1;
			while (i + n < p.size() && p[i + n] == c) ++n;
			int v = -1;
			char b[16];
			switch (c) {
			case 'y': v = tm.tm_year + 1900; if (n == 2) v %= 100; break;
			case 'M':
				if (n >= 3) { out += n == 3 ? std::string(months[tm.tm_mon], 3) : months[tm.tm_mon]; }
				else v = tm.tm_mon + 1;
				break;
			case 'd': v = tm.tm_mday; break;
			case 'H': v = tm.tm_hour; break;
			case 'h': v = tm.tm_hour % 12 == 0 ? 12 : tm.tm_hour % 12; break;
			case 'm': v = tm.tm_min; break;
			case 's': v = tm.tm_sec; break;
			case 'S': v = (int)(ms % 1000); break;
			case 'a': out += tm.tm_hour < 12 ? "AM" : "PM"; break;
			case 'E': out += n >= 4 ? days[tm.tm_wday] : std::string(days[tm.tm_wday], 3); break;
			default: out.append(n, c); break;
			}
			if (v >= 0) {
				std::sprintf(b, "%0*d", (int)n, v);
				out += b;
			}
			i += n;
		}
		return JavaString(out);
	}
};

class SimpleDateFormat : public DateFormat {
public:
	SimpleDateFormat() {}
	SimpleDateFormat(const std::string& p) : DateFormat(p) {}
	template<class L> SimpleDateFormat(const std::string& p, L) : DateFormat(p) {}   // (pattern, Locale)
	virtual ~SimpleDateFormat() {}
};
#endif // DATEFORMAT_H
