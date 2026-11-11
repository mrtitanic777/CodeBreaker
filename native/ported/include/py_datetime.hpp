// py_datetime.hpp -- the subset of Python's datetime module the ports use (naive values, local time):
//   py::datetime::date(y, m, d)                 .isoformat() .strftime() .weekday() .isoweekday() .toordinal()
//   py::datetime::datetime(y, m, d, h, mi, s, us)  the same, + .date() .timestamp() .replace(...)
//   datetime::now() / utcnow() / fromtimestamp(ts) / today()  isoformat(sep, timespec)  str()
//   comparison, and ValueError texts as CPython 3.14's.
// strftime reads: %a %A %b %B %c %d %f %H %I %j %m %M %p %S %U %w %W %x %X %y %Y %Z %z %%
// (the C locale; %Z and %z are empty for a naive value).
#pragma once

#include <chrono>
#include <ctime>
#include <tuple>

#include "py.hpp"

namespace py {
namespace datetime {

namespace detail_ {
inline bool leap(int64_t y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }
inline int64_t days_in_month(int64_t y, int64_t m) {
    static const int64_t d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return d[m - 1] + (m == 2 && leap(y) ? 1 : 0);
}
inline int64_t days_before_year(int64_t y) {
    int64_t p = y - 1;
    return p * 365 + p / 4 - p / 100 + p / 400;
}
inline int64_t days_before_month(int64_t y, int64_t m) {
    int64_t n = 0;
    for (int64_t i = 1; i < m; i++) n += days_in_month(y, i);
    return n;
}
inline int64_t ymd_to_ord(int64_t y, int64_t m, int64_t d) { return days_before_year(y) + days_before_month(y, m) + d; }
inline void check_date(int64_t y, int64_t m, int64_t d) {
    if (y < 1 || y > 9999) throw ValueError("year must be in 1..9999, not " + std::to_string(y));
    if (m < 1 || m > 12) throw ValueError("month must be in 1..12, not " + std::to_string(m));
    int64_t dim = days_in_month(y, m);
    if (d < 1 || d > dim)
        throw ValueError("day " + std::to_string(d) + " must be in range 1.." + std::to_string(dim) + " for month " + std::to_string(m) + " in year " + std::to_string(y));
}
inline const char* const DAY[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
inline const char* const MONTH[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
inline std::string pad(int64_t v, int w) {
    std::string s = std::to_string(v);
    return std::string(s.size() < static_cast<size_t>(w) ? static_cast<size_t>(w) - s.size() : 0, '0') + s;
}
}  // namespace detail_

class datetime;

class date {
  public:
    int64_t year, month, day;
    date(int64_t y, int64_t m, int64_t d) : year(y), month(m), day(d) { detail_::check_date(y, m, d); }
    int64_t toordinal() const { return detail_::ymd_to_ord(year, month, day); }
    int64_t weekday() const { return (toordinal() + 6) % 7; }
    int64_t isoweekday() const { return weekday() + 1; }
    str isoformat() const { return str(detail_::pad(year, 4) + "-" + detail_::pad(month, 2) + "-" + detail_::pad(day, 2)); }
    str strftime(const str& fmt) const;
    friend bool operator==(const date& a, const date& b) { return a.toordinal() == b.toordinal(); }
    friend bool operator<(const date& a, const date& b) { return a.toordinal() < b.toordinal(); }
};

class datetime {
  public:
    int64_t year, month, day, hour, minute, second, microsecond;
    datetime(int64_t y, int64_t m, int64_t d, int64_t h = 0, int64_t mi = 0, int64_t s = 0, int64_t us = 0)
        : year(y), month(m), day(d), hour(h), minute(mi), second(s), microsecond(us) {
        detail_::check_date(y, m, d);
        if (h < 0 || h > 23) throw ValueError("hour must be in 0..23, not " + std::to_string(h));
        if (mi < 0 || mi > 59) throw ValueError("minute must be in 0..59, not " + std::to_string(mi));
        if (s < 0 || s > 59) throw ValueError("second must be in 0..59, not " + std::to_string(s));
        if (us < 0 || us > 999999) throw ValueError("microsecond must be in 0..999999, not " + std::to_string(us));
    }
    static datetime fromtimestamp(double ts) {
        // CPython: the fraction rounded half-even to microseconds, then the local time of the whole seconds
        double frac, whole;
        frac = std::modf(ts, &whole);
        int64_t us = static_cast<int64_t>(std::nearbyint(frac * 1e6));
        int64_t secs = static_cast<int64_t>(whole);
        if (us < 0) { us += 1000000; secs -= 1; }
        if (us >= 1000000) { us -= 1000000; secs += 1; }
        std::time_t t = static_cast<std::time_t>(secs);
        std::tm tm{};
        if (localtime_s(&tm, &t) != 0) throw OSError("[Errno 22] Invalid argument");
        if (tm.tm_year < 70) throw OSError("[Errno 22] Invalid argument");   // (the system's localtime refuses a local time before 1970)
        return datetime(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, std::min(tm.tm_sec, 59), us);
    }
    static datetime now() {
        auto n = std::chrono::system_clock::now().time_since_epoch();
        int64_t us = std::chrono::duration_cast<std::chrono::microseconds>(n).count();
        std::time_t t = static_cast<std::time_t>(us / 1000000);
        std::tm tm{};
        localtime_s(&tm, &t);
        return datetime(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, std::min(tm.tm_sec, 59), us % 1000000);
    }
    static datetime today() { return now(); }
    static datetime utcnow() {
        auto n = std::chrono::system_clock::now().time_since_epoch();
        int64_t us = std::chrono::duration_cast<std::chrono::microseconds>(n).count();
        std::time_t t = static_cast<std::time_t>(us / 1000000);
        std::tm tm{};
        gmtime_s(&tm, &t);
        return datetime(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, std::min(tm.tm_sec, 59), us % 1000000);
    }
    datetime replace(std::optional<int64_t> y = std::nullopt, std::optional<int64_t> mo = std::nullopt, std::optional<int64_t> d = std::nullopt,
                     std::optional<int64_t> h = std::nullopt, std::optional<int64_t> mi = std::nullopt, std::optional<int64_t> s = std::nullopt,
                     std::optional<int64_t> us = std::nullopt) const {
        return datetime(y.value_or(year), mo.value_or(month), d.value_or(day), h.value_or(hour), mi.value_or(minute), s.value_or(second), us.value_or(microsecond));
    }
    class date date() const { return class date(year, month, day); }
    int64_t toordinal() const { return detail_::ymd_to_ord(year, month, day); }
    int64_t weekday() const { return (toordinal() + 6) % 7; }
    int64_t isoweekday() const { return weekday() + 1; }
    // naive local time as a POSIX timestamp
    double timestamp() const {
        std::tm tm{};
        tm.tm_year = static_cast<int>(year - 1900);
        tm.tm_mon = static_cast<int>(month - 1);
        tm.tm_mday = static_cast<int>(day);
        tm.tm_hour = static_cast<int>(hour);
        tm.tm_min = static_cast<int>(minute);
        tm.tm_sec = static_cast<int>(second);
        tm.tm_isdst = -1;
        std::time_t t = std::mktime(&tm);
        if (t == static_cast<std::time_t>(-1)) throw OSError("[Errno 22] Invalid argument");
        return static_cast<double>(t) + static_cast<double>(microsecond) / 1e6;
    }
    // isoformat(sep="T", timespec="auto")
    str isoformat(char sep = 'T', const char* timespec = "auto") const {
        std::string ts = timespec;
        std::string out = detail_::pad(year, 4) + "-" + detail_::pad(month, 2) + "-" + detail_::pad(day, 2) + sep;
        if (ts == "auto") ts = microsecond ? "microseconds" : "seconds";
        if (ts == "hours") return str(out + detail_::pad(hour, 2));
        if (ts == "minutes") return str(out + detail_::pad(hour, 2) + ":" + detail_::pad(minute, 2));
        std::string hms = detail_::pad(hour, 2) + ":" + detail_::pad(minute, 2) + ":" + detail_::pad(second, 2);
        if (ts == "seconds") return str(out + hms);
        if (ts == "milliseconds") return str(out + hms + "." + detail_::pad(microsecond / 1000, 3));
        if (ts == "microseconds") return str(out + hms + "." + detail_::pad(microsecond, 6));
        throw ValueError("Unknown timespec value");
    }
    str to_str() const { return isoformat(' '); }
    str strftime(const str& fmt) const;
    friend bool operator==(const datetime& a, const datetime& b) { return a.key_() == b.key_(); }
    friend bool operator!=(const datetime& a, const datetime& b) { return !(a == b); }
    friend bool operator<(const datetime& a, const datetime& b) { return a.key_() < b.key_(); }
    friend bool operator>(const datetime& a, const datetime& b) { return b < a; }
    friend bool operator<=(const datetime& a, const datetime& b) { return !(b < a); }
    friend bool operator>=(const datetime& a, const datetime& b) { return !(a < b); }

  private:
    std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t> key_() const {
        return {year, month, day, hour, minute, second, microsecond};
    }
};

namespace detail_ {
// ISO year and week of a date (%G %V %g)
inline std::pair<int64_t, int64_t> iso_week(int64_t y, int64_t mo, int64_t d) {
    int64_t ord = ymd_to_ord(y, mo, d), wd = (ord + 6) % 7;   // Monday = 0
    int64_t thursday = ord - wd + 3;                           // the Thursday of this week decides the ISO year
    int64_t yy = y;
    if (thursday < ymd_to_ord(y, 1, 1)) yy = y - 1;
    else if (thursday > ymd_to_ord(y, 12, 31)) yy = y + 1;
    return {yy, (thursday - ymd_to_ord(yy, 1, 1)) / 7 + 1};
}
inline str format_(const str& fmt, int64_t y, int64_t mo, int64_t d, int64_t h, int64_t mi, int64_t s, int64_t us) {
    int64_t wd = (ymd_to_ord(y, mo, d) + 6) % 7;                         // Monday = 0
    int64_t yday = days_before_month(y, mo) + d;                          // 1..366
    std::u32string out;
    auto add = [&](const std::string& t) { for (char c : t) out.push_back(static_cast<char32_t>(static_cast<unsigned char>(c))); };
    const std::u32string& f = fmt.u32();
    auto spec = [&](char32_t c, bool strip_zeros, auto&& self) -> void {
        auto num = [&](int64_t v, int w) { add(strip_zeros ? std::to_string(v) : pad(v, w)); };
        switch (c) {
            case U'a': add(std::string(DAY[wd]).substr(0, 3)); break;
            case U'A': add(DAY[wd]); break;
            case U'b': case U'h': add(std::string(MONTH[mo - 1]).substr(0, 3)); break;
            case U'B': add(MONTH[mo - 1]); break;
            case U'c':   // "%a %b %e %H:%M:%S %Y"
                for (char32_t part : std::u32string(U"a b e H:M:S Y")) {
                    if (part == U' ' || part == U':') out.push_back(part);
                    else self(part, false, self);
                }
                break;
            case U'C': num(y / 100, 2); break;
            case U'd': num(d, 2); break;
            case U'D': self(U'm', false, self); out.push_back(U'/'); self(U'd', false, self); out.push_back(U'/'); self(U'y', false, self); break;
            case U'e': add(d < 10 ? " " + std::to_string(d) : std::to_string(d)); break;
            case U'f': add(pad(us, 6)); break;
            case U'F': add(pad(y, 4) + "-" + pad(mo, 2) + "-" + pad(d, 2)); break;
            case U'g': num(iso_week(y, mo, d).first % 100, 2); break;
            case U'G': add(pad(iso_week(y, mo, d).first, 4)); break;
            case U'H': num(h, 2); break;
            case U'I': num(h % 12 == 0 ? 12 : h % 12, 2); break;
            case U'j': num(yday, 3); break;
            case U'm': num(mo, 2); break;
            case U'M': num(mi, 2); break;
            case U'n': out.push_back(U'\n'); break;
            case U'p': add(h < 12 ? "AM" : "PM"); break;
            case U'r': self(U'I', false, self); out.push_back(U':'); self(U'M', false, self); out.push_back(U':'); self(U'S', false, self); out.push_back(U' '); self(U'p', false, self); break;
            case U'R': self(U'H', false, self); out.push_back(U':'); self(U'M', false, self); break;
            case U'S': num(s, 2); break;
            case U't': out.push_back(U'\t'); break;
            case U'T': self(U'H', false, self); out.push_back(U':'); self(U'M', false, self); out.push_back(U':'); self(U'S', false, self); break;
            case U'u': add(std::to_string(wd + 1)); break;
            case U'U': num((yday - 1 + 7 - (wd + 1) % 7) / 7, 2); break;   // week of the year, Sunday first
            case U'V': num(iso_week(y, mo, d).second, 2); break;
            case U'w': add(std::to_string((wd + 1) % 7)); break;
            case U'W': num((yday - 1 + 7 - wd) / 7, 2); break;            // week of the year, Monday first
            case U'x': self(U'm', false, self); out.push_back(U'/'); self(U'd', false, self); out.push_back(U'/'); self(U'y', false, self); break;
            case U'X': self(U'H', false, self); out.push_back(U':'); self(U'M', false, self); out.push_back(U':'); self(U'S', false, self); break;
            case U'y': num(y % 100, 2); break;
            case U'Y': add(strip_zeros ? std::to_string(y) : pad(y, 4)); break;
            case U'Z': case U'z': break;   // (a naive value: nothing)
            case U'%': out.push_back(U'%'); break;
            default: throw ValueError("Invalid format string");
        }
    };
    for (size_t i = 0; i < f.size(); i++) {
        if (f[i] != U'%') { out.push_back(f[i]); continue; }
        if (++i >= f.size()) throw ValueError("Invalid format string");
        char32_t c = f[i];
        bool strip = false;
        if (c == U'#') {   // the Windows flag: no leading zeros
            if (++i >= f.size()) throw ValueError("Invalid format string");
            c = f[i];
            strip = true;
        } else if (c == U'E' || c == U'O') {   // %Ec %EC %Ex %EX %Ey %EY, %Od %Oe %OH %OI %Om %OM %OS %Ou %OU %OV %Ow %OW %Oy
            if (++i >= f.size()) throw ValueError("Invalid format string");
            char32_t n = f[i];
            bool ok = c == U'E' ? std::u32string(U"cCxXyY").find(n) != std::u32string::npos : std::u32string(U"deHImMSuUVwWy").find(n) != std::u32string::npos;
            if (!ok) throw ValueError("Invalid format string");
            c = n;
        }
        spec(c, strip, spec);
    }
    return str(out);
}
}  // namespace detail_

inline str date::strftime(const str& fmt) const { return detail_::format_(fmt, year, month, day, 0, 0, 0, 0); }
inline str datetime::strftime(const str& fmt) const { return detail_::format_(fmt, year, month, day, hour, minute, second, microsecond); }

}  // namespace datetime
}  // namespace py
