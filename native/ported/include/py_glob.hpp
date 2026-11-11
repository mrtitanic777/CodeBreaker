// py_glob.hpp -- Python's fnmatch and glob modules (CPython 3.14 semantics).
//
//   py::fnmatch::fnmatch(name, pat) / fnmatchcase / filter(names, pat)
//   py::glob::glob(pathname, recursive=false, include_hidden=false, root_dir=none) -> list<str>
//   py::glob::has_magic / escape
//
// The pattern is read as fnmatch.translate does: * ? [seq] [!seq], ranges, the same handling of an unclosed [,
// an empty set, a reversed range and a backslash in a set. A name is matched whole (re.DOTALL: * and ? take newlines).
// Names are in the order the folder lists them (os.scandir's).
#pragma once

#include <optional>
#include <vector>

#include "py.hpp"

namespace py {
namespace fnmatch {

namespace detail_ {

struct Tok {
    enum Kind { Lit, Any, Star, Set, Never } kind = Lit;
    char32_t c = 0;
    bool neg = false;
    std::vector<char32_t> lits;
    std::vector<std::pair<char32_t, char32_t>> ranges;
    bool matches_set(char32_t x) const {
        bool hit = false;
        for (char32_t l : lits) if (l == x) hit = true;
        for (const auto& r : ranges) if (r.first <= x && x <= r.second) hit = true;
        return hit != neg;
    }
};

// fnmatch.translate, as tokens
inline std::vector<Tok> compile(const std::u32string& pat) {
    std::vector<Tok> out;
    size_t i = 0, n = pat.size();
    while (i < n) {
        char32_t c = pat[i++];
        if (c == U'*') {
            if (out.empty() || out.back().kind != Tok::Star) { Tok t; t.kind = Tok::Star; out.push_back(t); }
        } else if (c == U'?') {
            Tok t; t.kind = Tok::Any; out.push_back(t);
        } else if (c == U'[') {
            size_t j = i;
            if (j < n && pat[j] == U'!') j++;
            if (j < n && pat[j] == U']') j++;
            while (j < n && pat[j] != U']') j++;
            if (j >= n) {
                Tok t; t.kind = Tok::Lit; t.c = U'['; out.push_back(t);
                continue;
            }
            std::u32string stuff = pat.substr(i, j - i);
            Tok t; t.kind = Tok::Set;
            std::vector<std::u32string> chunks;
            if (stuff.find(U'-') == std::u32string::npos) {
                chunks.push_back(stuff);
            } else {
                size_t k = pat[i] == U'!' ? i + 2 : i + 1, start = i;
                for (;;) {
                    size_t f = pat.find(U'-', k);
                    if (f == std::u32string::npos || f >= j) break;
                    chunks.push_back(pat.substr(start, f - start));
                    start = f + 1;
                    k = f + 3;
                }
                std::u32string chunk = pat.substr(start, j - start);
                if (!chunk.empty()) chunks.push_back(chunk);
                else chunks.back() += U'-';
                // remove empty ranges -- invalid in a regex
                for (size_t q = chunks.size() - 1; q >= 1; q--) {
                    if (!chunks[q - 1].empty() && !chunks[q].empty() && chunks[q - 1].back() > chunks[q][0]) {
                        chunks[q - 1] = chunks[q - 1].substr(0, chunks[q - 1].size() - 1) + chunks[q].substr(1);
                        chunks.erase(chunks.begin() + static_cast<long>(q));
                    }
                }
            }
            i = j + 1;
            // the class text is the chunks joined by an (unescaped) '-': read as a regex class is: item '-' item is a range
            std::vector<std::pair<char32_t, bool>> el;   // (char, true for the joining '-')
            for (size_t q = 0; q < chunks.size(); q++) {
                if (q) el.push_back({U'-', true});
                for (char32_t ch : chunks[q]) el.push_back({ch, false});
            }
            if (el.empty()) { t.kind = Tok::Never; out.push_back(t); continue; }                       // an empty set never matches
            if (el.size() == 1 && el[0].first == U'!' && !el[0].second) { t.kind = Tok::Any; out.push_back(t); continue; }   // [!]
            size_t at = 0;
            if (el[0].first == U'!' && !el[0].second) { t.neg = true; at = 1; }
            for (size_t q = at; q < el.size(); q++) {
                if (!el[q].second && q + 2 < el.size() + 0 && el[q + 1].second && !el[q + 2].second) {
                    t.ranges.push_back({el[q].first, el[q + 2].first});
                    q += 2;
                } else {
                    t.lits.push_back(el[q].first);
                }
            }
            out.push_back(t);
        } else {
            Tok t; t.kind = Tok::Lit; t.c = c; out.push_back(t);
        }
    }
    return out;
}

inline bool one_(const Tok& t, char32_t x) {
    switch (t.kind) {
        case Tok::Lit: return t.c == x;
        case Tok::Any: return true;
        case Tok::Set: return t.matches_set(x);
        default: return false;
    }
}

inline bool match(const std::vector<Tok>& toks, const std::u32string& name) {
    size_t p = 0, s = 0, star_p = SIZE_MAX, star_s = 0;
    while (s < name.size()) {
        if (p < toks.size() && toks[p].kind == Tok::Star) { star_p = p++; star_s = s; continue; }
        if (p < toks.size() && one_(toks[p], name[s])) { p++; s++; continue; }
        if (star_p != SIZE_MAX) { p = star_p + 1; s = ++star_s; continue; }
        return false;
    }
    while (p < toks.size() && toks[p].kind == Tok::Star) p++;
    return p == toks.size();
}

}  // namespace detail_

inline bool fnmatchcase(const str& name, const str& pat) { return detail_::match(detail_::compile(pat.u32()), name.u32()); }
inline bool fnmatch(const str& name, const str& pat) { return fnmatchcase(os::path::normcase(name), os::path::normcase(pat)); }
inline list<str> filter(const list<str>& names, const str& pat) {
    std::vector<detail_::Tok> toks = detail_::compile(os::path::normcase(pat).u32());
    list<str> out;
    for (const str& n : names)
        if (detail_::match(toks, os::path::normcase(n).u32())) out.append(n);
    return out;
}

}  // namespace fnmatch

namespace glob {

inline bool has_magic(const str& s) {
    for (char32_t c : s.u32())
        if (c == U'*' || c == U'?' || c == U'[') return true;
    return false;
}
// glob.escape(pathname): the drive stays as it is, the magic characters of the rest are put in brackets
inline str escape(const str& pathname) {
    auto [drive, rest] = os::path::splitdrive(pathname);
    std::u32string out;
    for (char32_t c : rest.u32()) {
        if (c == U'*' || c == U'?' || c == U'[') { out.push_back(U'['); out.push_back(c); out.push_back(U']'); }
        else out.push_back(c);
    }
    return drive + str(out);
}

namespace detail_ {
inline bool ishidden(const str& s) { return !s.empty() && s.u32()[0] == U'.'; }
inline bool isrecursive(const str& s) { return s.u32() == U"**"; }
inline str join_(const std::optional<str>& dirname, const str& basename) {
    if (!dirname || dirname->empty() || basename.empty()) return (dirname && !dirname->empty()) ? *dirname : basename;
    return os::path::join(*dirname, basename);
}
inline bool lexists(const str& p) {
    if (p.u32().find(U'\0') != std::u32string::npos) return false;
    std::error_code ec;
    std::filesystem::file_status st = std::filesystem::symlink_status(std::filesystem::u8path(p.utf8()), ec);
    return !ec && st.type() != std::filesystem::file_type::not_found;
}
// the entries of dirname, as os.scandir lists them (only the directories with dironly); an unreadable folder: none
inline list<str> listdir(const str& dirname, bool dironly) {
    list<str> out;
    std::error_code ec;
    std::filesystem::directory_iterator it(std::filesystem::u8path((dirname.empty() ? str(".") : dirname).utf8()), ec);
    if (ec) return out;
    for (; it != std::filesystem::directory_iterator(); it.increment(ec)) {
        if (ec) break;
        std::error_code e2;
        if (!dironly || it->is_directory(e2)) out.append(str(it->path().filename().u8string()));
    }
    return out;
}
inline list<str> glob1(const str& dirname, const str& pattern, bool dironly, bool include_hidden) {
    list<str> names = listdir(dirname, dironly);
    if (include_hidden || !ishidden(pattern)) {
        list<str> kept;
        for (const str& x : names)
            if (include_hidden || !ishidden(x)) kept.append(x);
        names = kept;
    }
    return fnmatch::filter(names, pattern);
}
inline list<str> glob0(const str& dirname, const str& basename) {
    list<str> out;
    if (!basename.empty()) {
        if (lexists(join_(dirname, basename))) out.append(basename);
    } else if (os::path::isdir(dirname)) {
        out.append(basename);
    }
    return out;
}
inline void rlistdir(const str& dirname, bool dironly, bool include_hidden, const str& prefix, list<str>& out) {
    for (const str& x : listdir(dirname, dironly)) {
        if (!include_hidden && ishidden(x)) continue;
        out.append(prefix.empty() ? x : os::path::join(prefix, x));
        str path = dirname.empty() ? x : join_(dirname, x);
        rlistdir(path, dironly, include_hidden, prefix.empty() ? x : os::path::join(prefix, x), out);
    }
}
inline list<str> glob2(const str& dirname, bool dironly, bool include_hidden) {
    list<str> out;
    if (dirname.empty() || os::path::isdir(dirname)) out.append(str(""));
    rlistdir(dirname, dironly, include_hidden, str(""), out);
    return out;
}
inline list<str> iglob(const str& pathname, const std::optional<str>& root_dir, bool recursive, bool dironly, bool include_hidden) {
    list<str> out;
    auto [dirname, basename] = os::path::split(pathname);
    if (!has_magic(pathname)) {
        if (!basename.empty()) {
            if (lexists(join_(root_dir, pathname))) out.append(pathname);
        } else if (os::path::isdir(join_(root_dir, dirname))) {
            out.append(pathname);   // (a pattern ending with a separator matches only directories)
        }
        return out;
    }
    if (dirname.empty()) {
        str base = root_dir ? *root_dir : str("");
        if (recursive && isrecursive(basename)) return glob2(base, dironly, include_hidden);
        return glob1(base, basename, dironly, include_hidden);
    }
    list<str> dirs;
    if (dirname != pathname && has_magic(dirname)) dirs = iglob(dirname, root_dir, recursive, true, include_hidden);
    else dirs = list<str>{dirname};
    for (const str& d : dirs) {
        str where = join_(root_dir, d);
        list<str> names;
        if (has_magic(basename)) names = (recursive && isrecursive(basename)) ? glob2(where, dironly, include_hidden) : glob1(where, basename, dironly, include_hidden);
        else names = glob0(where, basename);
        for (const str& n : names) out.append(os::path::join(d, n));
    }
    return out;
}
}  // namespace detail_

// glob.glob(pathname, *, root_dir=None, recursive=False, include_hidden=False)
inline list<str> glob(const str& pathname, bool recursive = false, bool include_hidden = false, const std::optional<str>& root_dir = std::nullopt) {
    return detail_::iglob(pathname, root_dir, recursive, false, include_hidden);
}

}  // namespace glob
}  // namespace py
