// lib_re.cc -- the re module over native/l2's pyre (a backtracking matcher with Python's syntax and
// semantics): compile with a cache, Pattern and Match objects, match / fullmatch / search / sub /
// subn (templates with \1 \g<name>, or a function) / split / findall / finditer / escape, the flags
// I M S X A, positions in code points, bytes patterns over bytes (each byte one code point).
#include <map>
#include <memory>

#include "pyre.hpp"
#include "rt.hpp"

namespace py {

static TypeObj *T_pattern, *T_match;
static var E_re_error;

struct PatternObj : DataObj {
    std::shared_ptr<re::Regex> rx;
    var pattern;            // as given (str or bytes)
    int flags = 0;
    bool bytes = false;
};
struct MatchObj : DataObj {
    re::Match m;
    var string;             // the subject as given
    var pattern;            // the PatternObj
    bool bytes = false;
    bool ascii = true;      // byte offsets are code point offsets
    int64_t pos = 0, endpos = 0;
};

// bytes are matched as Latin-1 text: each byte a code point
static std::string latin1_to_utf8(const std::string& b) {
    std::string o;
    for (unsigned char c : b) append_utf8(o, c);
    return o;
}
static std::string utf8_to_latin1(const std::string& s) {
    std::string o;
    for (char32_t c : to_u32(s)) o.push_back((char)c);
    return o;
}

// re.VERBOSE: whitespace and # comments outside classes and escapes go
static std::string strip_verbose(const std::string& p) {
    std::string o;
    bool cls = false;
    for (size_t i = 0; i < p.size(); i++) {
        char c = p[i];
        if (c == '\\' && i + 1 < p.size()) { o.push_back(c); o.push_back(p[++i]); continue; }
        if (cls) { if (c == ']') cls = false; o.push_back(c); continue; }
        if (c == '[') { cls = true; o.push_back(c); if (i + 1 < p.size() && p[i + 1] == ']') o.push_back(p[++i]); continue; }
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v') continue;
        if (c == '#') { while (i < p.size() && p[i] != '\n') i++; continue; }
        o.push_back(c);
    }
    return o;
}

static var compile_pattern(const var& pattern, int flags) {
    if (pattern.is_ref() && pattern.ref()->type == T_pattern) return pattern;
    static std::map<std::pair<std::string, int>, var>* cache = new std::map<std::pair<std::string, int>, var>();
    bool bytes = is_bytes(pattern);
    std::string src = bytes ? latin1_to_utf8(raw_of(pattern)) : str_utf8(pattern);
    auto key = std::make_pair(std::string(bytes ? "b:" : "s:") + src, flags);
    auto it = cache->find(key);
    if (it != cache->end()) return it->second;
    std::string p = src;
    // inline (?x) at the start
    if ((flags & 64) || p.rfind("(?x)", 0) == 0) {
        if (p.rfind("(?x)", 0) == 0) p = p.substr(4);
        p = strip_verbose(p);
    }
    auto* po = new PatternObj();
    po->type = T_pattern;
    po->pattern = pattern;
    po->flags = flags;
    po->bytes = bytes;
    try {
        po->rx = std::make_shared<re::Regex>(p, flags & (2 | 8 | 16));
    } catch (std::exception& e) {
        delete po;
        raise(make_exc_args(T(E_re_error), {new_str(e.what())}));
    }
    var v(static_cast<Obj*>(po));
    if (cache->size() > 2048) cache->clear();
    (*cache)[key] = v;
    return v;
}
static PatternObj* P(const var& v) { return static_cast<PatternObj*>(v.ref()); }
static MatchObj* MO(const var& v) { return static_cast<MatchObj*>(v.ref()); }

static std::string subject(const PatternObj* p, const var& s) {
    if (p->bytes) {
        if (!is_bytes(s)) raise(E_TypeError, "cannot use a bytes pattern on a string-like object");
        return latin1_to_utf8(raw_of(s));
    }
    if (!is_str(s)) raise(E_TypeError, "cannot use a string pattern on a bytes-like object");
    return str_utf8(s);
}
static var out_text(bool bytes, const std::string& s) { return bytes ? new_bytes(utf8_to_latin1(s)) : new_str(s); }

static var make_match(const var& pat, const re::Match& m, const var& string, bool ascii, int64_t pos, int64_t endpos) {
    auto* mo = new MatchObj();
    mo->type = T_match;
    mo->m = m;
    mo->string = string;
    mo->pattern = pat;
    mo->bytes = P(pat)->bytes;
    mo->ascii = ascii;
    mo->pos = pos;
    mo->endpos = endpos;
    return var(static_cast<Obj*>(mo));
}
static bool is_ascii(const std::string& s) {
    for (unsigned char c : s) if (c >= 0x80) return false;
    return true;
}
// code point index of a byte offset
static int64_t cp_at(const std::string& s, long byte, bool ascii) {
    if (byte < 0) return -1;
    if (ascii) return byte;
    int64_t n = 0;
    for (long k = 0; k < byte && k < (long)s.size(); k++) n += ((unsigned char)s[(size_t)k] & 0xC0) != 0x80;
    return n;
}
static size_t byte_at(const std::string& s, int64_t cp, bool ascii) {
    if (ascii) return (size_t)std::min<int64_t>(cp, (int64_t)s.size());
    size_t b = 0;
    for (int64_t k = 0; k < cp && b < s.size(); k++) {
        b++;
        while (b < s.size() && ((unsigned char)s[b] & 0xC0) == 0x80) b++;
    }
    return b;
}

static int group_index(const MatchObj* mo, const var& g) {
    if (is_int(g)) {
        int64_t i = g.as_int64();
        if (i < 0 || i > mo->m.ngroups()) raise(E_IndexError, "no such group");
        return (int)i;
    }
    if (is_str(g)) {
        const std::string& n = str_utf8(g);
        for (auto& [name, idx] : P(mo->pattern)->rx->names())
            if (name == n) return idx;
    }
    raise(E_IndexError, "no such group");
}
static var group_value(const MatchObj* mo, int g, const var& dflt = None) {
    if (!mo->m.has(g)) return dflt;
    return out_text(mo->bytes, mo->m.group(g));
}

// a replacement template: \1 .. \99, \g<n>, \g<name>, and the escapes
static std::string expand_template(const MatchObj* mo, const std::string& t) {
    std::string o;
    for (size_t i = 0; i < t.size(); i++) {
        char c = t[i];
        if (c != '\\' || i + 1 >= t.size()) { o.push_back(c); continue; }
        char d = t[++i];
        if (d == 'g' && i + 1 < t.size() && t[i + 1] == '<') {
            size_t e = t.find('>', i + 2);
            if (e == std::string::npos) raise(make_exc_args(T(E_re_error), {"missing >, unterminated name"_s}));
            std::string name = t.substr(i + 2, e - i - 2);
            var g = !name.empty() && isdigit((unsigned char)name[0]) ? var((int64_t)std::stoll(name)) : new_str(name);
            int gi = group_index(mo, g);
            if (mo->m.has(gi)) o += mo->m.group(gi);
            i = e;
        } else if (d >= '0' && d <= '9') {
            int n = d - '0';
            if (i + 1 < t.size() && isdigit((unsigned char)t[i + 1]) && n * 10 + (t[i + 1] - '0') <= mo->m.ngroups()) n = n * 10 + (t[++i] - '0');
            if (n > mo->m.ngroups()) raise(make_exc_args(T(E_re_error), {new_str("invalid group reference " + std::to_string(n))}));
            if (mo->m.has(n)) o += mo->m.group(n);
        } else {
            switch (d) {
            case 'n': o.push_back('\n'); break;
            case 't': o.push_back('\t'); break;
            case 'r': o.push_back('\r'); break;
            case 'f': o.push_back('\f'); break;
            case 'v': o.push_back('\v'); break;
            case 'a': o.push_back('\a'); break;
            case 'b': o.push_back('\b'); break;
            case '\\': o.push_back('\\'); break;
            default:
                if (isalpha((unsigned char)d)) raise(make_exc_args(T(E_re_error), {new_str(std::string("bad escape \\") + d)}));
                o.push_back('\\');
                o.push_back(d);
            }
        }
    }
    return o;
}

// every match of p in s from pos, as Python's finditer (an empty match just after a match is allowed,
// two empty matches at one place are not)
static std::vector<re::Match> all_matches(const PatternObj* p, const std::string& s, size_t pos, size_t endpos, int64_t limit) {
    std::vector<re::Match> out;
    if (pos == 0 && endpos >= s.size() && limit < 0) return p->rx->finditer(s);
    for (auto& m : p->rx->finditer(s)) {
        if ((size_t)m.start() < pos) continue;
        if ((size_t)m.end() > endpos) break;
        out.push_back(m);
        if (limit >= 0 && (int64_t)out.size() >= limit) break;
    }
    return out;
}

static var do_sub(const var& pat, const var& repl, const var& string, int64_t count, bool subn) {
    PatternObj* p = P(pat);
    std::string s = subject(p, string);
    bool ascii = is_ascii(s);
    bool fn = callable(repl).truthy();
    std::string tmpl;
    bool literal = false;
    if (!fn) {
        tmpl = p->bytes ? latin1_to_utf8(raw_of(repl)) : str_utf8(repl);
        literal = tmpl.find('\\') == std::string::npos;
    }
    std::vector<re::Match> ms = all_matches(p, s, 0, s.size(), count > 0 ? count : -1);
    std::string out;
    size_t last = 0;
    int64_t n = 0;
    for (auto& m : ms) {
        out.append(s, last, (size_t)m.start() - last);
        if (fn) {
            var mv = make_match(pat, m, string, ascii, 0, (int64_t)s.size());
            var r = call1(repl, mv);
            out += p->bytes ? latin1_to_utf8(raw_of(r)) : str_utf8(r);
        } else if (literal) {
            out += tmpl;
        } else {
            MatchObj tmp;
            tmp.m = m;
            tmp.pattern = pat;
            tmp.bytes = p->bytes;
            out += expand_template(&tmp, tmpl);
            tmp.pattern = var();
        }
        last = (size_t)m.end();
        n++;
    }
    out.append(s, last, std::string::npos);
    var r = out_text(p->bytes, out);
    if (subn) return new_tuple({r, var(n)});
    return r;
}

static var do_split(const var& pat, const var& string, int64_t maxsplit) {
    PatternObj* p = P(pat);
    std::string s = subject(p, string);
    std::vector<var> out;
    size_t last = 0;
    int64_t n = 0;
    for (auto& m : all_matches(p, s, 0, s.size(), maxsplit > 0 ? maxsplit : -1)) {
        out.push_back(out_text(p->bytes, s.substr(last, (size_t)m.start() - last)));
        for (int g = 1; g <= m.ngroups(); g++) out.push_back(m.has(g) ? out_text(p->bytes, m.group(g)) : var(None));
        last = (size_t)m.end();
        n++;
    }
    out.push_back(out_text(p->bytes, s.substr(last)));
    return new_list(out);
}

static var do_findall(const var& pat, const var& string, int64_t pos, int64_t endpos) {
    PatternObj* p = P(pat);
    std::string s = subject(p, string);
    bool ascii = is_ascii(s);
    std::vector<var> out;
    size_t bp = byte_at(s, pos, ascii), be = endpos < 0 ? s.size() : byte_at(s, endpos, ascii);
    for (auto& m : all_matches(p, s, bp, be, -1)) {
        int ng = m.ngroups();
        if (ng == 0) out.push_back(out_text(p->bytes, m.group(0)));
        else if (ng == 1) out.push_back(out_text(p->bytes, m.has(1) ? m.group(1) : ""));
        else {
            std::vector<var> row;
            for (int g = 1; g <= ng; g++) row.push_back(out_text(p->bytes, m.has(g) ? m.group(g) : ""));
            out.push_back(new_tuple(row));
        }
    }
    return new_list(out);
}

static var do_match(const var& pat, const var& string, int64_t pos, int64_t endpos, int mode) {
    PatternObj* p = P(pat);
    std::string s = subject(p, string);
    bool ascii = is_ascii(s);
    int64_t n = ascii ? (int64_t)s.size() : cp_count(s);
    if (pos < 0) pos = 0;
    if (endpos < 0 || endpos > n) endpos = n;
    if (pos > endpos) return None;
    size_t bp = byte_at(s, pos, ascii), be = byte_at(s, endpos, ascii);
    re::Match m = mode == 0 ? p->rx->match(s, bp, be) : mode == 1 ? p->rx->fullmatch(s, bp, be) : p->rx->search(s, bp, be);
    if (!m) return None;
    return make_match(pat, m, string, ascii, pos, endpos);
}

static int flags_of(const CallArgs& a, size_t i) {
    var f = arg(a, i, "flags", 0);
    return (int)index_(f);
}
static int64_t opt_int(const CallArgs& a, size_t i, const char* name, int64_t dflt) {
    var v = arg(a, i, name, None);
    return v.is_none() ? dflt : v.as_int64();
}

std::string re_escape(const std::string& s) {
    std::string o;
    for (char32_t c : to_u32(s)) {
        if (c < 0x80 && !isalnum((int)c) && c != '_') o.push_back('\\');
        append_utf8(o, c);
    }
    return o;
}

void init_re_module(const var& mod) {
    T_pattern = new_type("Pattern", {}, Layout::NativeData, true, "re");
    T_match = new_type("Match", {}, Layout::NativeData, true, "re");
    TypeObj* err = new_type("error", {E_Exception}, Layout::Exception, true, "re");
    E_re_error = tv(err);
    mod_set(mod, "error", E_re_error);
    mod_set(mod, "PatternError", E_re_error);
    mod_set(mod, "Pattern", tv(T_pattern));
    mod_set(mod, "Match", tv(T_match));
    for (auto& [n, v] : std::vector<std::pair<const char*, int>>{{"I", 2}, {"IGNORECASE", 2}, {"L", 4}, {"LOCALE", 4}, {"M", 8},
                                                                {"MULTILINE", 8}, {"S", 16}, {"DOTALL", 16}, {"U", 32}, {"UNICODE", 32},
                                                                {"X", 64}, {"VERBOSE", 64}, {"A", 256}, {"ASCII", 256}, {"NOFLAG", 0}})
        mod_set(mod, n, v);
    mod_def(mod, "compile", [](const CallArgs& a) { return compile_pattern(arg(a, 0, "pattern"), flags_of(a, 1)); });
    mod_def(mod, "match", [](const CallArgs& a) { return do_match(compile_pattern(arg(a, 0, "pattern"), flags_of(a, 2)), arg(a, 1, "string"), 0, -1, 0); });
    mod_def(mod, "fullmatch", [](const CallArgs& a) { return do_match(compile_pattern(arg(a, 0, "pattern"), flags_of(a, 2)), arg(a, 1, "string"), 0, -1, 1); });
    mod_def(mod, "search", [](const CallArgs& a) { return do_match(compile_pattern(arg(a, 0, "pattern"), flags_of(a, 2)), arg(a, 1, "string"), 0, -1, 2); });
    mod_def(mod, "sub", [](const CallArgs& a) {
        return do_sub(compile_pattern(arg(a, 0, "pattern"), flags_of(a, 4)), arg(a, 1, "repl"), arg(a, 2, "string"), arg(a, 3, "count", 0).as_int64(), false);
    });
    mod_def(mod, "subn", [](const CallArgs& a) {
        return do_sub(compile_pattern(arg(a, 0, "pattern"), flags_of(a, 4)), arg(a, 1, "repl"), arg(a, 2, "string"), arg(a, 3, "count", 0).as_int64(), true);
    });
    mod_def(mod, "split", [](const CallArgs& a) {
        return do_split(compile_pattern(arg(a, 0, "pattern"), flags_of(a, 3)), arg(a, 1, "string"), arg(a, 2, "maxsplit", 0).as_int64());
    });
    mod_def(mod, "findall", [](const CallArgs& a) { return do_findall(compile_pattern(arg(a, 0, "pattern"), flags_of(a, 2)), arg(a, 1, "string"), 0, -1); });
    mod_def(mod, "finditer", [](const CallArgs& a) {
        var pat = compile_pattern(arg(a, 0, "pattern"), flags_of(a, 2));
        return pat.call("finditer", arg(a, 1, "string"));
    });
    mod_def(mod, "escape", [](const CallArgs& a) {
        var s = a.pos.at(0);
        if (is_bytes(s)) return new_bytes(utf8_to_latin1(re_escape(latin1_to_utf8(raw_of(s)))));
        return new_str(re_escape(str_utf8(s)));
    });
    mod_def(mod, "purge", [](const CallArgs&) { return None; });

    TypeObj* t = T_pattern;
    add_method(t, "match", [](const CallArgs& a) { return do_match(a.pos[0], arg(a, 1, "string"), opt_int(a, 2, "pos", 0), opt_int(a, 3, "endpos", -1), 0); });
    add_method(t, "fullmatch", [](const CallArgs& a) { return do_match(a.pos[0], arg(a, 1, "string"), opt_int(a, 2, "pos", 0), opt_int(a, 3, "endpos", -1), 1); });
    add_method(t, "search", [](const CallArgs& a) { return do_match(a.pos[0], arg(a, 1, "string"), opt_int(a, 2, "pos", 0), opt_int(a, 3, "endpos", -1), 2); });
    add_method(t, "sub", [](const CallArgs& a) { return do_sub(a.pos[0], arg(a, 1, "repl"), arg(a, 2, "string"), arg(a, 3, "count", 0).as_int64(), false); });
    add_method(t, "subn", [](const CallArgs& a) { return do_sub(a.pos[0], arg(a, 1, "repl"), arg(a, 2, "string"), arg(a, 3, "count", 0).as_int64(), true); });
    add_method(t, "split", [](const CallArgs& a) { return do_split(a.pos[0], arg(a, 1, "string"), arg(a, 2, "maxsplit", 0).as_int64()); });
    add_method(t, "findall", [](const CallArgs& a) { return do_findall(a.pos[0], arg(a, 1, "string"), opt_int(a, 2, "pos", 0), opt_int(a, 3, "endpos", -1)); });
    add_method(t, "finditer", [](const CallArgs& a) {
        var pat = a.pos[0], string = arg(a, 1, "string");
        PatternObj* p = P(pat);
        std::string s = subject(p, string);
        bool ascii = is_ascii(s);
        int64_t pos = opt_int(a, 2, "pos", 0), endpos = opt_int(a, 3, "endpos", -1);
        size_t bp = byte_at(s, pos, ascii), be = endpos < 0 ? s.size() : byte_at(s, endpos, ascii);
        auto ms = std::make_shared<std::vector<re::Match>>(all_matches(p, s, bp, be, -1));
        auto i = std::make_shared<size_t>(0);
        int64_t n = ascii ? (int64_t)s.size() : cp_count(s);
        return new_iter([pat, string, ms, i, ascii, n](var& out) {
            if (*i >= ms->size()) return false;
            out = make_match(pat, (*ms)[(*i)++], string, ascii, 0, n);
            return true;
        });
    });
    add_getter(t, "pattern", [](const var& s) { return P(s)->pattern; });
    add_getter(t, "flags", [](const var& s) { return var((int64_t)(P(s)->flags | 32)); });
    add_getter(t, "groups", [](const var& s) { return var((int64_t)P(s)->rx->groups()); });
    add_getter(t, "groupindex", [](const var& s) {
        var d = new_dict_obj();
        for (auto& [n, i] : P(s)->rx->names()) d.setitem(new_str(n), var((int64_t)i));
        return d;
    });
    add_method(t, "__repr__", [](const CallArgs& a) { return new_str("re.compile(" + repr_(P(a.pos[0])->pattern) + ")"); });
    add_method(t, "__eq__", [](const CallArgs& a) {
        if (!a.pos.at(1).is_ref() || a.pos[1].ref()->type != T_pattern) return var(false);
        return var(P(a.pos[0])->pattern == P(a.pos[1])->pattern && P(a.pos[0])->flags == P(a.pos[1])->flags);
    });
    add_method(t, "__hash__", [](const CallArgs& a) { return var(hash_(P(a.pos[0])->pattern)); });

    t = T_match;
    add_method(t, "group", [](const CallArgs& a) {
        MatchObj* mo = MO(a.pos[0]);
        if (a.pos.size() <= 1) return group_value(mo, 0);
        if (a.pos.size() == 2) return group_value(mo, group_index(mo, a.pos[1]));
        std::vector<var> v;
        for (size_t i = 1; i < a.pos.size(); i++) v.push_back(group_value(mo, group_index(mo, a.pos[i])));
        return new_tuple(v);
    });
    add_method(t, "__getitem__", [](const CallArgs& a) { MatchObj* mo = MO(a.pos[0]); return group_value(mo, group_index(mo, a.pos.at(1))); });
    add_method(t, "groups", [](const CallArgs& a) {
        MatchObj* mo = MO(a.pos[0]);
        var dflt = arg(a, 1, "default", None);
        std::vector<var> v;
        for (int g = 1; g <= mo->m.ngroups(); g++) v.push_back(group_value(mo, g, dflt));
        return new_tuple(v);
    });
    add_method(t, "groupdict", [](const CallArgs& a) {
        MatchObj* mo = MO(a.pos[0]);
        var dflt = arg(a, 1, "default", None);
        var d = new_dict_obj();
        for (auto& [n, i] : P(mo->pattern)->rx->names()) d.setitem(new_str(n), group_value(mo, i, dflt));
        return d;
    });
    auto pos_of = [](const CallArgs& a, int which) -> var {
        MatchObj* mo = MO(a.pos[0]);
        int g = a.pos.size() > 1 ? group_index(mo, a.pos[1]) : 0;
        const std::string& s = mo->m.s ? *mo->m.s : std::string();
        int64_t st = mo->m.has(g) ? cp_at(s, mo->m.start(g), mo->ascii) : -1;
        int64_t en = mo->m.has(g) ? cp_at(s, mo->m.end(g), mo->ascii) : -1;
        if (which == 0) return var(st);
        if (which == 1) return var(en);
        return new_tuple({var(st), var(en)});
    };
    add_method(t, "start", [pos_of](const CallArgs& a) { return pos_of(a, 0); });
    add_method(t, "end", [pos_of](const CallArgs& a) { return pos_of(a, 1); });
    add_method(t, "span", [pos_of](const CallArgs& a) { return pos_of(a, 2); });
    add_method(t, "expand", [](const CallArgs& a) {
        MatchObj* mo = MO(a.pos[0]);
        std::string tm = mo->bytes ? latin1_to_utf8(raw_of(a.pos.at(1))) : str_utf8(a.pos.at(1));
        return out_text(mo->bytes, expand_template(mo, tm));
    });
    add_method(t, "__bool__", [](const CallArgs&) { return var(true); });
    add_method(t, "__repr__", [](const CallArgs& a) {
        MatchObj* mo = MO(a.pos[0]);
        const std::string& s = *mo->m.s;
        int64_t st = cp_at(s, mo->m.start(0), mo->ascii), en = cp_at(s, mo->m.end(0), mo->ascii);
        return new_str("<re.Match object; span=(" + std::to_string(st) + ", " + std::to_string(en) + "), match=" +
                       from_u32(to_u32(repr_(group_value(mo, 0))).substr(0, 50)) + ">");   // (%.50R, as CPython)
    });
    add_getter(t, "string", [](const var& s) { return MO(s)->string; });
    add_getter(t, "re", [](const var& s) { return MO(s)->pattern; });
    add_getter(t, "pos", [](const var& s) { return var(MO(s)->pos); });
    add_getter(t, "endpos", [](const var& s) { return var(MO(s)->endpos); });
    add_getter(t, "lastindex", [](const var& s) {
        MatchObj* mo = MO(s);
        // the last group to close: the one with the greatest end (the outer one on a tie)
        int best = -1;
        long be = -1;
        for (int g = 1; g <= mo->m.ngroups(); g++)
            if (mo->m.has(g) && mo->m.end(g) > be) { best = g; be = mo->m.end(g); }
        return best < 0 ? var(None) : var((int64_t)best);
    });
    add_getter(t, "lastgroup", [](const var& s) {
        var li = s.getattr("lastindex");
        if (li.is_none()) return var(None);
        for (auto& [n, i] : P(MO(s)->pattern)->rx->names())
            if (i == li.as_int64()) return new_str(n);
        return var(None);
    });
}

}  // namespace py
