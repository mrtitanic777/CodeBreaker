#ifndef BIGINTEGER_H
#define BIGINTEGER_H
// java.math.BigInteger, what the games do with one: build it from a sign and big-endian magnitude bytes
// (new BigInteger(1, md5.digest())) and print it in a radix (toString(16) -- MD5String). Was a stub whose
// toString() was "". No arithmetic.
#include <string>
#include <vector>
#include "JArray.h"
#include "JavaString.h"

class BigInteger {
	int _sign;                                      // -1, 0 or 1
	std::vector<unsigned char> _mag;                // big-endian, no leading zero bytes
public:
	BigInteger(int signum, const JArray<unsigned char>& mag) : _sign(0) {
		int i = 0;
		while (i < mag.length && mag[i] == 0) ++i;
		for (; i < mag.length; ++i) _mag.push_back(mag[i]);
		_sign = _mag.empty() ? 0 : (signum < 0 ? -1 : 1);
	}
	int signum() const { return _sign; }
	BigInteger* abs() const { BigInteger* r = new BigInteger(*this); if (r->_sign < 0) r->_sign = 1; return r; }
	JavaString toString(int radix) const {
		if (radix < 2 || radix > 36) radix = 10;
		if (_sign == 0) return JavaString("0");
		std::vector<unsigned char> n(_mag);
		std::string digits;
		while (!n.empty()) {                            // n /= radix, keeping the remainder
			unsigned rem = 0;
			std::vector<unsigned char> q;
			for (size_t i = 0; i < n.size(); ++i) {
				unsigned cur = (rem << 8) | n[i];
				unsigned char d = (unsigned char)(cur / (unsigned)radix);
				rem = cur % (unsigned)radix;
				if (!q.empty() || d) q.push_back(d);
			}
			digits += "0123456789abcdefghijklmnopqrstuvwxyz"[rem];
			n.swap(q);
		}
		if (_sign < 0) digits += '-';
		return JavaString(std::string(digits.rbegin(), digits.rend()));
	}
	JavaString toString() const { return toString(10); }
};
#endif // BIGINTEGER_H
