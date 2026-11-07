// inspect.cc -- the byte-level parts of codebreaker/formats/inspect.py: the runs of printable
// characters in a file with what each looks like, the classic hex listing, and categorize().
// The description of a file (describe, markdown, render, the command) stays with the readers it
// assembles, in the shim.
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

#include "../core/questions.h"
#include "../core/text.h"

namespace cbf::inspect {
namespace {

bool printable(uint8_t b) { return b == '\t' || b == '\r' || b == '\n' || (b >= 0x20 && b <= 0x7e); }

bool ends_with_ci(const std::string &t, const char *ext) {
    size_t n = strlen(ext);
    if (t.size() < n) return false;
    for (size_t i = 0; i < n; ++i) {
        char c = t[t.size() - n + i];
        if (c >= 'A' && c <= 'Z') c = char(c + 32);
        if (c != ext[i]) return false;
    }
    return true;
}

bool ci_contains(const std::string &t, const char *word) {
    size_t n = strlen(word);
    for (size_t i = 0; i + n <= t.size(); ++i) {
        size_t k = 0;
        for (; k < n; ++k) {
            char c = t[i + k];
            if (c >= 'A' && c <= 'Z') c = char(c + 32);
            if (c != word[k]) break;
        }
        if (k == n) return true;
    }
    return false;
}

// str.strip(): Unicode white space, on UTF-8
std::string strip(const std::string &s) {
    bool ascii = true;
    for (unsigned char ch : s)
        if (ch >= 0x80) {
            ascii = false;
            break;
        }
    if (ascii) {                                       // the common case: no code points to make
        auto sp = [](unsigned char ch) { return (ch >= 9 && ch <= 13) || (ch >= 0x1c && ch <= 0x20); };
        size_t a = 0, b = s.size();
        while (a < b && sp(s[a])) ++a;
        while (b > a && sp(s[b - 1])) --b;
        return s.substr(a, b - a);
    }
    std::vector<uint32_t> cp = codepoints(s);
    auto space = [](uint32_t c) {
        return (c >= 9 && c <= 13) || (c >= 0x1c && c <= 0x20) || c == 0x85 || c == 0xa0 || c == 0x1680 ||
               (c >= 0x2000 && c <= 0x200a) || c == 0x2028 || c == 0x2029 || c == 0x202f || c == 0x205f || c == 0x3000;
    };
    size_t a = 0, b = cp.size();
    while (a < b && space(cp[a])) ++a;
    while (b > a && space(cp[b - 1])) --b;
    return from_codepoints(std::vector<uint32_t>(cp.begin() + a, cp.begin() + b));
}

bool format_spec(const std::string &t) {      // %[-+ 0-9.*#hlLIw]*[sdiouxXeEfgGcpn]
    const std::string A = "-+ 0123456789.*#hlLIw", B = "sdiouxXeEfgGcpn";
    for (size_t i = 0; i < t.size(); ++i) {
        if (t[i] != '%') continue;
        size_t j = i + 1;
        while (j < t.size() && A.find(t[j]) != std::string::npos) ++j;
        if (j < t.size() && B.find(t[j]) != std::string::npos) return true;
    }
    return false;
}

bool constant_form(const std::string &t) {    // [A-Z][A-Z0-9]*(?:_[A-Z0-9]+)+
    auto up = [](char c) { return c >= 'A' && c <= 'Z'; };
    auto upd = [&](char c) { return up(c) || (c >= '0' && c <= '9'); };
    if (t.empty() || !up(t[0])) return false;
    size_t i = 1;
    while (i < t.size() && upd(t[i])) ++i;
    int groups = 0;
    while (i < t.size() && t[i] == '_') {
        size_t j = i + 1;
        while (j < t.size() && upd(t[j])) ++j;
        if (j == i + 1) return false;
        ++groups;
        i = j;
    }
    return i == t.size() && groups >= 1;
}

const char *categorize_text(const std::string &text) {
    std::string t = strip(text);
    if (format_spec(t)) return "format";
    for (const char *e : {".dll", ".exe", ".sys", ".drv", ".vxd", ".386", ".ocx", ".cpl", ".scr", ".inf", ".ini", ".hlp"})
        if (ends_with_ci(t, e)) return "module";
    if (t.find('\\') != std::string::npos || t.find('/') != std::string::npos) return "path";
    if (constant_form(t)) return "constant";
    for (const char *w : {"error", "fail", "invalid", "cannot", "unexpected", "exception"})
        if (ci_contains(t, w)) return "error";
    return "other";
}

void categorize(Call &c) { c.out.text(categorize_text(c.str_arg(0))); }

struct Found {
    int64_t off;
    bool wide;
    std::string text;
};

// inspect.strings(data, min_len, wide, limit) -> [(offset, kind, category, text)]
void strings(Call &c) {
    Bytes d = c.data;
    int64_t min_len = c.int_arg(0, 4);
    bool wide = c.bool_arg(1, true);
    bool has_limit = !c.arg(2).is_none();
    int64_t limit = has_limit ? c.int_arg(2) : 0;
    std::vector<Found> found;
    int64_t n = d.size();
    for (int64_t i = 0; i < n;) {                 // [printable]{min,}
        int64_t j = i;
        while (j < n && printable(d.data()[j])) ++j;
        if (j - i >= min_len && j > i) {
            found.push_back({i, false, latin1(d.slice(i, j))});
            i = j;
        } else {
            i = j > i ? j : i + 1;
        }
    }
    if (wide) {
        for (int64_t i = 0; i < n;) {             // (?:[printable]\x00){min,}
            int64_t j = i;
            while (j + 1 < n && printable(d.data()[j]) && d.data()[j + 1] == 0) j += 2;
            if (j > i && (j - i) / 2 >= min_len) {
                found.push_back({i, true, utf16le(d.slice(i, j))});
                i = j;
            } else {
                ++i;
            }
        }
    }
    std::stable_sort(found.begin(), found.end(), [](const Found &a, const Found &b) {
        if (a.off != b.off) return a.off < b.off;
        if (a.wide != b.wide) return !a.wide;
        return a.text < b.text;
    });
    size_t count = found.size();
    if (has_limit) {
        int64_t k = limit < 0 ? std::max<int64_t>(0, int64_t(count) + limit) : std::min<int64_t>(limit, count);
        count = static_cast<size_t>(k);
    }
    Out &o = c.out;
    o.begin_list();
    for (size_t k = 0; k < count; ++k) {
        o.begin_tuple();
        o.integer(found[k].off);
        o.text(found[k].wide ? "utf16" : "ascii");
        o.text(categorize_text(found[k].text));
        o.text(found[k].text);
        o.end_tuple();
    }
    o.end_list();
}

// inspect.hex_lines(data, base) -> ["%08X  %-47s  %s"]
void hex_lines(Call &c) {
    Bytes d = c.data;
    int64_t base = c.int_arg(0);
    Out &o = c.out;
    o.begin_list();
    for (int64_t i = 0; i < d.size(); i += 16) {
        std::string hx, txt;
        for (int64_t k = i; k < std::min<int64_t>(i + 16, d.size()); ++k) {
            uint8_t b = d.data()[k];
            if (k > i) hx += ' ';
            static const char *H = "0123456789ABCDEF";
            hx += H[b >> 4];
            hx += H[b & 15];
            txt += (b >= 32 && b < 127) ? char(b) : '.';
        }
        hx.resize(std::max<size_t>(hx.size(), 47), ' ');
        std::string a = hex(static_cast<uint64_t>(base + i), true);
        if (a.size() < 8) a = std::string(8 - a.size(), '0') + a;
        o.text(a + "  " + hx + "  " + txt);
    }
    o.end_list();
}

}  // namespace

CBF_QUESTIONS(inspect) {
    add("inspect.categorize", categorize, "str");
    add("inspect.strings", strings);
    add("inspect.hex_lines", hex_lines);
}

}  // namespace cbf::inspect
