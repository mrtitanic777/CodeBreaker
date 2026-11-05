// prototypes.cc -- the Windows API's prototypes, read from the C headers (what
// codebreaker/symbols/prototypes.py's read_header and _build did): `<result> <CONVENTION> name(...);`
// and COM's `STDAPI_(type) name(...);`, every header of every folder, the first folder's
// declaration kept and a later (Microsoft's) header naming its unnamed parameters.
//
// A header is read as Latin-1 with Python's universal newlines, so each byte is one character and
// the character classes (\w, \s) are Python's for Latin-1.  The scanners below are the regular
// expressions the Python used, written out by hand with the same matching order.
#include "symbols.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <map>
#include <memory>
#include <set>
#include <vector>

namespace cbs {
namespace {

using std::string;
typedef unsigned char uc;

bool isw(uc c) {                          // Python's \w on a Latin-1 character (str.isalnum() or _)
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') return true;
    return c == 0xaa || c == 0xb2 || c == 0xb3 || c == 0xb5 || c == 0xb9 || c == 0xba || (c >= 0xbc && c <= 0xbe) ||
           (c >= 0xc0 && c != 0xd7 && c != 0xf7);
}
bool iss(uc c) { return (c >= 9 && c <= 13) || (c >= 0x1c && c <= 0x20) || c == 0x85 || c == 0xa0; }
bool isfirst(uc c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool isAZ(uc c) { return c >= 'A' && c <= 'Z'; }
bool isAZ09(uc c) { return isAZ(c) || (c >= '0' && c <= '9'); }
bool ends(const string& s, const char* t) { size_t n = strlen(t); return s.size() >= n && s.compare(s.size() - n, n, t) == 0; }
bool starts(const string& s, const char* t) { size_t n = strlen(t); return s.size() >= n && s.compare(0, n, t) == 0; }
string lower(string s) {                  // str.lower() on Latin-1
    for (auto& ch : s) { uc c = (uc)ch; if (isAZ(c) || (c >= 0xc0 && c <= 0xde && c != 0xd7)) ch = (char)(c + 0x20); }
    return s;
}
string upper_ascii(string s) { for (auto& ch : s) if (ch >= 'a' && ch <= 'z') ch = (char)(ch - 32); return s; }
string strip(const string& s) {
    size_t a = 0, b = s.size();
    while (a < b && iss((uc)s[a])) ++a;
    while (b > a && iss((uc)s[b - 1])) --b;
    return s.substr(a, b - a);
}
string collapse(const string& s) {        // re.sub(r"\s+", " ", s)
    string out; bool sp = false;
    for (char ch : s) { if (iss((uc)ch)) { if (!sp) out.push_back(' '); sp = true; } else { out.push_back(ch); sp = false; } }
    return out;
}
bool isupper_py(const string& s) {        // str.isupper() on Latin-1
    bool cased = false;
    for (char ch : s) {
        uc c = (uc)ch;
        bool up = isAZ(c) || (c >= 0xc0 && c <= 0xde && c != 0xd7);
        bool low = (c >= 'a' && c <= 'z') || c == 0xaa || c == 0xb5 || c == 0xba || (c >= 0xdf && c != 0xf7);
        if (low) return false;
        if (up) cased = true;
    }
    return cased;
}

const std::map<string, const char*>& CONVENTIONS() {
    static const std::map<string, const char*> m = {
        {"WINAPI", "stdcall"}, {"APIENTRY", "stdcall"}, {"PASCAL", "stdcall"}, {"CALLBACK", "stdcall"},
        {"STDAPICALLTYPE", "stdcall"}, {"STDCALL", "stdcall"}, {"__stdcall", "stdcall"}, {"_stdcall", "stdcall"},
        {"STDAPI", "stdcall"}, {"NTAPI", "stdcall"}, {"API", "stdcall"}, {"__pascal", "stdcall"},
        {"_pascal", "stdcall"}, {"WINAPIV", "cdecl"}, {"CDECL", "cdecl"}, {"cdecl", "cdecl"},
        {"__cdecl", "cdecl"}, {"_cdecl", "cdecl"}, {"STDAPIVCALLTYPE", "cdecl"}};
    return m;
}
bool is_com(const string& w) { return w == "STDAPI" || w == "WINOLEAPI" || w == "WINOLEAUTAPI"; }

bool all_of(const string& s, bool (*f)(uc)) { for (char ch : s) if (!f((uc)ch)) return false; return true; }
bool az09_(uc c) { return isAZ09(c) || c == '_'; }

bool not_a_type(const string& w) {        // _NOT_A_TYPE: a word that is no part of the result's type
    static const std::set<string> plain = {"extern", "C", "__declspec", "_CRTIMP", "static", "inline", "__inline",
                                           "dllimport", "dllexport", "_loadds", "__loadds", "EXPORTDBG"};
    if (plain.count(w)) return true;
    if (starts(w, "EXTERN_C")) return true;
    if (starts(w, "DECLSPEC_") && w.size() > 9) return true;
    if (w.size() >= 4 && isAZ((uc)w[0]) && all_of(w, az09_) && ends(w, "API")) return true;
    if (w.size() > 8 && ends(w, "_LINKAGE")) return true;
    return false;
}

// _split: the parameters at the top level of a list
std::vector<string> split(const string& params) {
    std::vector<string> out, keep;
    int depth = 0;
    string cur;
    for (char ch : params) {
        depth += ch == '(';
        depth -= ch == ')';
        if (ch == ',' && depth == 0) { out.push_back(cur); cur.clear(); }
        else cur.push_back(ch);
    }
    out.push_back(cur);
    for (auto& p : out) { string t = strip(p); if (!t.empty()) keep.push_back(t); }
    return keep;
}

struct Param { string type, name; };

// _parameter: ^(.*[\w*\]])\s+([A-Za-z_]\w*)((?:\s*\[\w*\])*)$ on the collapsed text, greedy group 1
Param parameter(const string& raw) {
    string text = strip(collapse(raw));
    size_t n = text.size();
    for (size_t k = n; k >= 1; --k) {
        uc last = (uc)text[k - 1];
        if (!(isw(last) || last == '*' || last == ']')) continue;
        size_t q = k;
        while (q < n && iss((uc)text[q])) ++q;
        if (q == k || q >= n || !isfirst((uc)text[q])) continue;
        size_t r = q + 1;
        while (r < n && isw((uc)text[r])) ++r;
        size_t t = r;
        for (;;) {                        // (?:\s*\[\w*\])*
            size_t u = t;
            while (u < n && iss((uc)text[u])) ++u;
            if (u >= n || text[u] != '[') break;
            ++u;
            while (u < n && isw((uc)text[u])) ++u;
            if (u >= n || text[u] != ']') break;
            t = u + 1;
        }
        if (t != n) continue;
        string g1 = text.substr(0, k), g2 = text.substr(q, r - q), g3 = text.substr(r, n - r);
        static const std::set<string> type_words = {"int", "long", "short", "char", "unsigned", "signed", "void", "float",
            "double", "const", "volatile", "far", "near", "struct", "union", "enum", "huge", "_far", "__far", "_near", "__near"};
        if (type_words.count(lower(g2)) || isupper_py(g2)) break;
        bool any = false;                 // a word of the type that is not just a qualifier
        for (size_t a = 0; a < g1.size();) {
            if (!isw((uc)g1[a])) { ++a; continue; }
            size_t b = a;
            while (b < g1.size() && isw((uc)g1[b])) ++b;
            string w = lower(g1.substr(a, b - a));
            if (w != "const" && w != "volatile" && w != "far" && w != "near") any = true;
            a = b;
        }
        if (!any) break;
        return {strip(g1 + g3), g2};
    }
    return {text, ""};
}

std::vector<Param> parameters(const string& inside) {
    std::vector<Param> out;
    string l = lower(inside);
    if (l.empty() || l == "void") return out;
    for (auto& p : split(inside)) out.push_back(parameter(p));
    return out;
}

struct Proto {
    string returns, convention, marker, header;
    std::vector<Param> params;
};
typedef std::vector<std::pair<string, std::shared_ptr<Proto>>> Ordered;   // a dict, in insertion order

struct Table {
    Ordered items;
    std::map<string, size_t> at;
    std::shared_ptr<Proto> get(const string& k) const { auto i = at.find(k); return i == at.end() ? nullptr : items[i->second].second; }
    void setdefault(const string& k, std::shared_ptr<Proto> p) { if (!at.count(k)) { at[k] = items.size(); items.push_back({k, p}); } }
};

// Python's text mode: Latin-1, \r\n and \r as \n
string read_text(const std::filesystem::path& path, bool& ok) {
    ok = false;
    FILE* f = _wfopen(path.c_str(), L"rb");
    if (!f) return string();
    string raw;
    char buf[65536];
    size_t got;
    while ((got = fread(buf, 1, sizeof buf, f)) > 0) raw.append(buf, got);
    fclose(f);
    string out;
    out.reserve(raw.size());
    for (size_t i = 0; i < raw.size(); ++i) {
        if (raw[i] == '\r') { out.push_back('\n'); if (i + 1 < raw.size() && raw[i + 1] == '\n') ++i; }
        else out.push_back(raw[i]);
    }
    ok = true;
    return out;
}

string strip_comments(const string& s) {  // _COMMENT.sub(" ", s): //[^\n]* | /\*.*?\*/
    string out;
    out.reserve(s.size());
    size_t i = 0, n = s.size();
    while (i < n) {
        if (s[i] == '/' && i + 1 < n && s[i + 1] == '/') {
            size_t e = s.find('\n', i);
            i = e == string::npos ? n : e;
            out.push_back(' ');
            continue;
        }
        if (s[i] == '/' && i + 1 < n && s[i + 1] == '*') {
            size_t e = s.find("*/", i + 2);
            if (e != string::npos) { i = e + 2; out.push_back(' '); continue; }
        }
        out.push_back(s[i++]);
    }
    return out;
}

string strip_directives(const string& s) {   // (?m)^[ \t]*#(?:[^\n]*\\\n)*[^\n]*$
    string out;
    out.reserve(s.size());
    size_t i = 0, n = s.size();
    while (i < n) {
        if (i == 0 || s[i - 1] == '\n') {
            size_t p = i;
            while (p < n && (s[p] == ' ' || s[p] == '\t')) ++p;
            if (p < n && s[p] == '#') {
                ++p;
                for (;;) {                // whole lines ending in a backslash, then the rest of the line
                    size_t e = s.find('\n', p);
                    if (e == string::npos) { p = n; break; }
                    if (e > p && s[e - 1] == '\\') { p = e + 1; continue; }
                    p = e;
                    break;
                }
                out.push_back(' ');
                i = p;
                continue;
            }
        }
        out.push_back(s[i++]);
    }
    return out;
}

void read_header(const std::filesystem::path& path, const string& header, Table& out) {
    bool ok;
    string text = read_text(path, ok);
    if (!ok) return;
    text = strip_directives(strip_comments(text));
    size_t n = text.size();
    // \b([A-Za-z_]\w*)\s*\(
    size_t pos = 0;
    while (pos < n) {
        size_t start = pos;
        if (!isfirst((uc)text[start]) || (start > 0 && isw((uc)text[start - 1]))) { ++pos; continue; }
        size_t e = start + 1;
        while (e < n && isw((uc)text[e])) ++e;
        size_t f = e;
        while (f < n && iss((uc)text[f])) ++f;
        if (f >= n || text[f] != '(') { ++pos; continue; }
        size_t mend = f + 1;
        pos = mend;
        string name = text.substr(start, e - start);
        // the words before: [A-Za-z_]\w*|\*|[;{}()=,] over the 200 characters before
        size_t b0 = start >= 200 ? start - 200 : 0;
        std::vector<string> words;
        for (size_t a = b0; a < start;) {
            uc c = (uc)text[a];
            if (isfirst(c)) {
                size_t b = a + 1;
                while (b < start && isw((uc)text[b])) ++b;
                words.push_back(text.substr(a, b - a));
                a = b;
            } else {
                if (c == '*' || c == ';' || c == '{' || c == '}' || c == '(' || c == ')' || c == '=' || c == ',')
                    words.push_back(string(1, (char)c));
                ++a;
            }
        }
        if (words.empty() || CONVENTIONS().count(name)) continue;
        const string& marker = words.back();
        string convention;
        auto ci = CONVENTIONS().find(marker);
        if (ci != CONVENTIONS().end()) convention = ci->second;
        else if (marker.size() >= 4 && isAZ((uc)marker[0]) && all_of(marker, [](uc c) { return isAZ09(c); }) && ends(marker, "API"))
            convention = "stdcall";
        else if (marker.size() >= 5 && isAZ((uc)marker[0]) && all_of(marker, [](uc c) { return isAZ09(c); }) && ends(marker, "APIV"))
            convention = "cdecl";
        if (convention.empty()) continue;
        std::vector<string> result;
        for (size_t k = words.size() - 1; k-- > 0;) {
            const string& w = words[k];
            if (w.size() == 1 && string(";{}()=,").find(w[0]) != string::npos) break;
            result.push_back(w);
        }
        std::reverse(result.begin(), result.end());
        std::vector<string> kept;
        bool typedef_ = false;
        for (auto& w : result) {
            string up = upper_ascii(w);
            if (not_a_type(w) || CONVENTIONS().count(w) || up == "FAR" || up == "NEAR") continue;
            kept.push_back(w);
            if (w == "typedef") typedef_ = true;
        }
        int depth = 0;
        size_t j = mend - 1;
        while (j < n) {
            depth += text[j] == '(';
            depth -= text[j] == ')';
            if (depth == 0) break;
            ++j;
        }
        if (depth) continue;
        {                                 // re.match(r"\s*;", text[j+1:j+12])
            size_t a = j + 1, lim = std::min(n, j + 12);
            while (a < lim && iss((uc)text[a])) ++a;
            if (a >= lim || text[a] != ';') continue;
        }
        string inside = strip(collapse(text.substr(mend, j - mend)));
        std::vector<Param> params = parameters(inside);
        if (typedef_) continue;
        auto p = std::make_shared<Proto>();
        string returns;
        for (size_t k = 0; k < kept.size(); ++k) { if (k) returns += " "; returns += kept[k]; }
        p->returns = !returns.empty() ? returns : is_com(marker) ? "HRESULT" : "void";
        p->convention = convention;
        p->marker = marker;
        p->params = params;
        p->header = header;
        out.setdefault(name, p);
    }
    // \b(?:STDAPI|WINOLEAPI|WINOLEAUTAPI)_\(\s*([^)]*?)\s*\)\s+([A-Za-z_]\w*)\s*\(([^;{}]*?)\)\s*;
    static const char* coms[] = {"STDAPI", "WINOLEAPI", "WINOLEAUTAPI"};
    pos = 0;
    while (pos < n) {
        size_t start = pos;
        if (start > 0 && isw((uc)text[start - 1])) { ++pos; continue; }
        size_t p = string::npos;
        for (const char* c : coms) {
            size_t l = strlen(c);
            if (text.compare(start, l, c) == 0 && start + l + 1 < n && text[start + l] == '_' && text[start + l + 1] == '(') {
                p = start + l + 2;
                break;
            }
        }
        if (p == string::npos) { ++pos; continue; }
        size_t close = text.find(')', p);
        if (close == string::npos) { ++pos; continue; }
        string g1 = strip(text.substr(p, close - p));
        size_t q = close + 1, q0 = q;
        while (q < n && iss((uc)text[q])) ++q;
        if (q == q0 || q >= n || !isfirst((uc)text[q])) { ++pos; continue; }
        size_t r = q + 1;
        while (r < n && isw((uc)text[r])) ++r;
        string g2 = text.substr(q, r - q);
        size_t s = r;
        while (s < n && iss((uc)text[s])) ++s;
        if (s >= n || text[s] != '(') { ++pos; continue; }
        size_t g3s = s + 1, t = g3s, mend = string::npos;
        for (; t < n; ++t) {
            char ch = text[t];
            if (ch == ')') {
                size_t u = t + 1;
                while (u < n && iss((uc)text[u])) ++u;
                if (u < n && text[u] == ';') { mend = u + 1; break; }
            }
            if (ch == ';' || ch == '{' || ch == '}') break;
        }
        if (mend == string::npos) { ++pos; continue; }
        string inside = strip(collapse(text.substr(g3s, t - g3s)));
        auto proto = std::make_shared<Proto>();
        proto->returns = g1;
        proto->convention = "stdcall";
        proto->marker = "STDAPI";
        proto->params = parameters(inside);
        proto->header = header;
        out.setdefault(g2, proto);
        pos = mend;
    }
}

void jstr(string& o, const string& s) {
    static const char* hex = "0123456789abcdef";
    o.push_back('"');
    for (char ch : s) {
        uc c = (uc)ch;
        if (c == '"') o += "\\\"";
        else if (c == '\\') o += "\\\\";
        else if (c < 0x20 || c >= 0x7f) { o += "\\u00"; o.push_back(hex[c >> 4]); o.push_back(hex[c & 15]); }
        else o.push_back(ch);
    }
    o.push_back('"');
}

string lower_name(const std::wstring& w) {   // a file name, lower-cased, as Latin-1-or-wider text for the JSON
    string out;
    for (wchar_t c : w) {
        unsigned v = (unsigned)c;
        if ((v >= 'A' && v <= 'Z') || (v >= 0xc0 && v <= 0xde && v != 0xd7)) v += 0x20;
        if (v < 0x100) out.push_back((char)v);
        else out.push_back('?');            // (never so in the headers read; see the gate)
    }
    return out;
}

}  // namespace

// The table for bits "32" and "16": `folders` is "<bits>\t<folder>\n" lines, in rank order.
bool prototypes_build(const std::u32string& folders, std::string& json) {
    std::vector<std::pair<string, std::vector<std::wstring>>> order;   // bits -> folders
    std::wstring line;
    string bits;
    auto flush = [&]() {
        if (line.empty() && bits.empty()) return;
        auto it = std::find_if(order.begin(), order.end(), [&](auto& p) { return p.first == bits; });
        if (it == order.end()) { order.push_back({bits, {}}); it = order.end() - 1; }
        it->second.push_back(line);
        line.clear(); bits.clear();
    };
    bool inbits = true;
    for (char32_t c : folders) {
        if (c == '\n') { flush(); inbits = true; continue; }
        if (inbits) { if (c == '\t') inbits = false; else bits.push_back((char)c); continue; }
        if (c >= 0x10000) { unsigned v = (unsigned)c - 0x10000; line.push_back((wchar_t)(0xd800 + (v >> 10))); line.push_back((wchar_t)(0xdc00 + (v & 0x3ff))); }
        else line.push_back((wchar_t)c);
    }
    flush();
    string o = "{";
    bool firstbits = true;
    for (const char* want : {"32", "16"}) {
        Table table;
        for (auto& bd : order) {
            if (bd.first != want) continue;
            for (size_t rank = 0; rank < bd.second.size(); ++rank) {
                std::filesystem::path dir(bd.second[rank]);
                std::vector<std::wstring> names;
                std::error_code ec;
                for (auto& ent : std::filesystem::directory_iterator(dir, ec)) names.push_back(ent.path().filename().wstring());
                std::sort(names.begin(), names.end());
                for (auto& fn : names) {
                    string l = lower_name(fn);
                    if (!ends(l, ".h") || l.find("win386") != string::npos) continue;
                    Table one;
                    read_header(dir / fn, l, one);
                    for (auto& kv : one.items) {
                        auto known = table.get(kv.first);
                        if (!known) { table.setdefault(kv.first, kv.second); continue; }
                        if (rank && known->params.size() == kv.second->params.size()) {
                            bool named = false;
                            for (auto& pr : known->params) if (!pr.name.empty()) named = true;
                            if (!named)
                                for (size_t k = 0; k < known->params.size(); ++k) known->params[k].name = kv.second->params[k].name;
                        }
                    }
                }
            }
        }
        if (!firstbits) o += ",";
        firstbits = false;
        o += "\"" + string(want) + "\":{";
        for (size_t k = 0; k < table.items.size(); ++k) {
            const Proto& p = *table.items[k].second;
            if (k) o += ",";
            jstr(o, table.items[k].first);
            o += ":{\"returns\":"; jstr(o, p.returns);
            o += ",\"convention\":"; jstr(o, p.convention);
            o += ",\"marker\":"; jstr(o, p.marker);
            o += ",\"parameters\":[";
            for (size_t m = 0; m < p.params.size(); ++m) {
                if (m) o += ",";
                o += "["; jstr(o, p.params[m].type); o += ","; jstr(o, p.params[m].name); o += "]";
            }
            o += "],\"header\":"; jstr(o, p.header); o += "}";
        }
        o += "}";
    }
    json = o + "}";
    return true;
}

}  // namespace cbs
