// build/x86asm/preproc_masm.cpp -- the MASM front end of the preprocessor (see preproc_masm.hpp): preproc.c's masm_* functions.
//
// Each source line is rewritten as text (MASM syntax into NASM's), a line at a time, with state for the open MACRO / IRP /
// REPT / STRUCT blocks, the constants and names seen, and a queue of extra lines. The code follows NASM's functions
// one for one (it is text surgery by nature, so it keeps their pointer-walking shape over the line's characters); the
// buffers are strings and have no length limits.
#include "build/x86asm/preproc_masm.hpp"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <format>
#include <unordered_set>
#include <vector>

#include "build/x86asm/preproc_util.hpp"

namespace cb::build::x86asm {

using namespace pp;

namespace {

bool mx_is_ws(char c) { return c == ' ' || c == '\t'; }
bool mx_eq(std::string_view a, std::string_view b) { return stricmp_(a, b) == 0; }

// One structure, union or record the source defined.
struct MxSMember {
    std::string name;
    const char* dir = nullptr;   // "db", "dw", "dd", "dq", "dt" (null: a record field, or a member of a structure type)
    std::string def;             // the default value as written
    int count = 0;
    int shift = 0;
};
struct MxSDef {
    std::string name;
    bool is_union = false;
    bool is_record = false;
    const char* rdir = nullptr;
    int usize = 0;
    std::vector<MxSMember> members;
};

// An open block: a MACRO body, a FOR/IRP body, a REPT body, or a nested macro definition being skipped.
struct MxBlk {
    bool is_macro = false, is_for = false, is_skip = false;
    std::vector<std::string> param;      // a macro's parameter names
    std::string forname;
    std::vector<std::string> item;       // the items a FOR goes through
    std::vector<std::string> oparam;     // the outer macro's parameters (for nesting)
    int oseq = 0;
    std::string forvar;
};

struct MxPending {
    std::string name, expr;
    std::vector<std::string> deps;
};

std::string mx_trim_right(std::string s, const char* set = " \t\r") {
    while (!s.empty() && std::strchr(set, s.back())) s.pop_back();
    return s;
}

bool mx_has_streg_form(const char* s) { return (s[0] == 's' || s[0] == 'S') && (s[1] == 't' || s[1] == 'T') && s[2] >= '0' && s[2] <= '7'; }

int mx_type_bytes(std::string_view t) {
    if (mx_eq(t, "byte") || mx_eq(t, "db") || mx_eq(t, "sbyte")) return 1;
    if (mx_eq(t, "word") || mx_eq(t, "dw") || mx_eq(t, "sword")) return 2;
    if (mx_eq(t, "dword") || mx_eq(t, "dd") || mx_eq(t, "sdword")) return 4;
    if (mx_eq(t, "fword") || mx_eq(t, "df")) return 6;
    if (mx_eq(t, "qword") || mx_eq(t, "dq")) return 8;
    if (mx_eq(t, "tbyte") || mx_eq(t, "dt")) return 10;
    return 0;
}
const char* mx_type_to_dd(std::string_view t) {
    if (mx_eq(t, "byte") || mx_eq(t, "db") || mx_eq(t, "sbyte")) return "db";
    if (mx_eq(t, "word") || mx_eq(t, "dw") || mx_eq(t, "sword")) return "dw";
    if (mx_eq(t, "dword") || mx_eq(t, "dd") || mx_eq(t, "sdword")) return "dd";
    if (mx_eq(t, "qword") || mx_eq(t, "dq")) return "dq";
    if (mx_eq(t, "tbyte") || mx_eq(t, "dt")) return "dt";
    return nullptr;
}
const char* mx_type_to_res(std::string_view t) {
    if (mx_eq(t, "byte") || mx_eq(t, "db") || mx_eq(t, "sbyte")) return "resb";
    if (mx_eq(t, "word") || mx_eq(t, "dw") || mx_eq(t, "sword")) return "resw";
    if (mx_eq(t, "dword") || mx_eq(t, "dd") || mx_eq(t, "sdword")) return "resd";
    if (mx_eq(t, "qword") || mx_eq(t, "dq")) return "resq";
    if (mx_eq(t, "tbyte") || mx_eq(t, "dt")) return "rest";
    return nullptr;
}
int mx_type_from_word(std::string_view w) {
    if (mx_eq(w, "byte")) return 1;
    if (mx_eq(w, "word")) return 2;
    if (mx_eq(w, "dword")) return 4;
    if (mx_eq(w, "fword")) return 6;
    if (mx_eq(w, "qword")) return 8;
    if (mx_eq(w, "tbyte")) return 10;
    return 0;
}
bool mx_is_cond(std::string_view w) {
    static const char* const names[] = {"if", "ife", "ifdef", "ifndef", "ifb", "ifnb", "ifidn", "ifdif", "else", "elseif", "endif", "if1", "if2"};
    for (const char* n : names)
        if (mx_eq(w, n)) return true;
    return false;
}
bool mx_is_wordop(std::string_view w) {
    static const char* const ops[] = {"and", "or", "xor", "not", "shl", "shr", "mod", "dup", "ptr", "eq", "ne", "lt", "le", "gt", "ge"};
    for (const char* o : ops)
        if (std::strlen(o) == w.size() && strnicmp_(w, o, w.size()) == 0) return true;
    return false;
}
bool mx_is_segreg2(const char* s) {
    static const char* const sr[] = {"es", "cs", "ss", "ds", "fs", "gs"};
    for (const char* r : sr)
        if (!strnicmp_(s, r, 2)) return true;
    return false;
}
int mx_farhalf(std::string_view s) {
    if (s.size() == 2 && !strnicmp_(s, "lo", 2)) return 0;
    if (s.size() == 3 && !strnicmp_(s, "off", 3)) return 0;
    if (s.size() == 2 && !strnicmp_(s, "hi", 2)) return 2;
    if (s.size() == 3 && !strnicmp_(s, "sel", 3)) return 2;
    if (s.size() == 3 && !strnicmp_(s, "seg", 3)) return 2;
    return -1;
}
bool mx_is_gpreg(std::string_view s) {
    static const char* const rg[] = {"ax", "bx", "cx", "dx", "si", "di", "bp", "sp", "eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp"};
    for (const char* r : rg)
        if (mx_eq(s, r)) return true;
    return false;
}
bool mx_ps_idchar(int c) { return isalnum_(static_cast<char>(c)) || c == '_' || c == '@' || c == '?' || c == '$'; }

// A word at *pp (identifier characters, and %[..] / %{..} groups), advancing *pp; white space before it is skipped.
std::string mx_word(const char*& p) {
    std::string out;
    while (*p == ' ' || *p == '\t') p++;
    while (isidchar(*p) || (*p == '%' && (p[1] == '[' || p[1] == '{'))) {
        if (*p == '%') {
            const char close = p[1] == '[' ? ']' : '}';
            out += *p++;
            out += *p++;
            while (*p && *p != close) out += *p++;
            if (*p == close) out += *p++;
        } else {
            out += *p++;
        }
    }
    return out;
}

// True if the line is only one identifier (and white space).
bool mx_ident_only(const char* s) {
    const char* p = s;
    if (!isidstart(*p) && *p != '?') return false;
    while (isidchar(*p) || *p == '.' || *p == '?' || *p == '$') p++;
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
    return *p == '\0';
}

bool mx_atom_ref(const char* s) {
    const char* p = s;
    while (*p == ' ' || *p == '\t') p++;
    if (mx_ident_only(p)) return true;
    if (*p == '%') {
        p++;
        if (*p == '{') {
            p++;
            while (*p && *p != '}') p++;
            if (*p != '}') return false;
            p++;
        } else {
            if (!isdigit_(*p)) return false;
            while (isdigit_(*p)) p++;
        }
        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
        return *p == '\0';
    }
    return false;
}

// "TYPE PTR x": the text after the PTR, or `s` itself when there is no type override.
const char* mx_skip_typeptr(const char* s) {
    const char* p = s;
    while (*p == ' ' || *p == '\t') p++;
    const std::string w1 = mx_word(p);
    if (!mx_type_bytes(w1)) return s;
    const std::string w2 = mx_word(p);
    if (!mx_eq(w2, "ptr")) return s;
    while (*p == ' ' || *p == '\t') p++;
    return p;
}

// "$st0" for the x87 register names, so they cannot be mistaken for symbols; true if any was changed.
bool mx_escape_streg(std::string& dst, const char* src) {
    const char* s = src;
    bool any = false;
    char prev = ' ';
    dst.clear();
    while (*s) {
        if ((prev == '\0' || !isidchar(prev)) && prev != '$' && (s[0] == 's' || s[0] == 'S') && (s[1] == 't' || s[1] == 'T') &&
            s[2] >= '0' && s[2] <= '7' && !isidchar(s[3])) {
            dst += '$';
            dst += s[0];
            dst += s[1];
            dst += s[2];
            prev = s[2];
            s += 3;
            any = true;
            continue;
        }
        prev = *s;
        dst += *s++;
    }
    return any;
}

bool mx_pack_char_literal(const char* s, std::string& out) {
    while (*s == ' ' || *s == '\t') s++;
    if (*s != '\'' && *s != '"') return false;
    const char q = *s++;
    const char* b = s;
    std::size_t n = 0;
    while (s[n] && s[n] != q) n++;
    if (s[n] != q || n < 2 || n > 8) return false;
    const char* t = s + n + 1;
    while (*t == ' ' || *t == '\t') t++;
    if (*t) return false;
    unsigned long long val = 0;
    for (std::size_t i = 0; i < n; i++) val = (val << 8) | static_cast<unsigned char>(b[i]);
    out = std::format("0{:X}h", val);
    return true;
}

int mx_db_field_bytes(const char* s) {
    int bytes = 0;
    while (*s == ' ' || *s == '\t') s++;
    if (!*s || *s == ';' || (*s == '?' && !isidchar(s[1]))) return 0;
    while (*s && *s != ';') {
        while (*s == ' ' || *s == '\t' || *s == ',') s++;
        if (!*s || *s == ';') break;
        if (*s == '"' || *s == '\'') {
            const char q = *s++;
            while (*s && *s != q) {
                bytes++;
                s++;
            }
            if (*s == q) s++;
        } else {
            bytes++;
            while (*s && *s != ',' && *s != ';') s++;
        }
    }
    return bytes;
}

bool mx_line_is_macro_def(const char* line) {
    const char* s = line;
    while (*s == ' ' || *s == '\t') s++;
    if (!*s) return false;
    while (*s && *s != ' ' && *s != '\t') s++;
    while (*s == ' ' || *s == '\t') s++;
    return !strnicmp_(s, "macro", 5) && (s[5] == '\0' || s[5] == ' ' || s[5] == '\t' || s[5] == '\r' || s[5] == '\n');
}

// "<a, b>" arguments of a line to "{a, b}" (the first level only); null when none or unbalanced.
bool mx_angle_args_to_brace(const std::string& line, std::string& out) {
    const char* p = line.c_str();
    int depth = 0;
    char q = 0;
    bool changed = false;
    out.clear();
    while (*p) {
        if (q) {
            if (*p == q) q = 0;
            out += *p++;
            continue;
        }
        if (*p == '\'' || *p == '"') {
            q = *p;
            out += *p++;
            continue;
        }
        if (*p == '<') {
            out += depth == 0 ? '{' : '<';
            if (depth == 0) changed = true;
            depth++;
            p++;
            continue;
        }
        if (*p == '>') {
            if (depth == 1) {
                out += '}';
                depth = 0;
            } else {
                if (depth > 1) depth--;
                out += *p;
            }
            p++;
            continue;
        }
        out += *p++;
    }
    return changed && depth == 0;
}

// Strings with doubled quotes ('it''s') into NASM's backquoted form; false when nothing changed.
bool mx_fix_dq_strings(const std::string& line, std::string& out) {
    const char* p = line.c_str();
    bool changed = false;
    out.clear();
    while (*p) {
        if (*p == '\'' || *p == '"') {
            const char qc = *p;
            const char* s = p + 1;
            bool doubled = false;
            while (*s) {
                if (*s == qc) {
                    if (s[1] == qc) {
                        s += 2;
                        doubled = true;
                        continue;
                    }
                    break;
                }
                s++;
            }
            if (!doubled) {
                const char* e = (*s == qc) ? s + 1 : s;
                while (p < e) out += *p++;
                continue;
            }
            out += '`';
            const char* r = p + 1;
            while (r < s) {
                if (*r == qc && r[1] == qc) {
                    out += qc;
                    r += 2;
                    continue;
                }
                if (*r == '`' || *r == '\\') out += '\\';
                out += *r++;
            }
            out += '`';
            p = (*s == qc) ? s + 1 : s;
            changed = true;
            continue;
        }
        out += *p++;
    }
    return changed;
}

// "x shl 2 + y shr 1": the shift operators in a sum get parentheses so they bind as MASM's do. (masm_paren_shifts)
std::string mx_paren_shifts(const char* src) {
    const char* p = src;
    std::string out;
    std::size_t term = 0;
    char q = 0;
    while (*p) {
        if (q) {
            if (*p == q) q = 0;
            out += *p++;
            continue;
        }
        if (*p == '\'' || *p == '"') {
            q = *p;
            out += *p++;
            continue;
        }
        if (*p == '+' || *p == '-') {
            out += *p++;
            while (*p == ' ' || *p == '\t') out += *p++;
            term = out.size();
            continue;
        }
        if (*p == '(') {
            int d = 0;
            do {
                if (*p == '(') d++;
                else if (*p == ')') d--;
                out += *p++;
            } while (*p && d > 0);
            continue;
        }
        if ((!strnicmp_(p, "shl", 3) || !strnicmp_(p, "shr", 3)) && !isidchar(p[3]) && (p == src || !isidchar(p[-1]))) {
            const char* r = p + 3;
            while (*r == ' ' || *r == '\t') r++;
            while (*r == '~' || *r == '-' || *r == '+') {
                r++;
                while (*r == ' ' || *r == '\t') r++;
            }
            if (*r == '(') {
                int d = 0;
                do {
                    if (*r == '(') d++;
                    else if (*r == ')') d--;
                    r++;
                } while (*r && d > 0);
            } else {
                while (isidchar(*r) || *r == '.') r++;
            }
            out.insert(term, "(");
            while (p < r) out += *p++;
            out += ')';
            term = out.size();
            continue;
        }
        out += *p++;
    }
    return out;
}

// MASM's word operators (EQ NE LE GE LT GT MOD SHL SHR AND OR XOR NOT) as symbols; a ";" comment is kept as it is.
std::string mx_xlat_ops(const char* src_in) {
    static const struct {
        const char* w;
        const char* op;
    } ops[] = {{"eq", "=="}, {"ne", "!="}, {"le", "<="}, {"ge", ">="}, {"lt", "<"}, {"gt", ">"}, {"mod", "%"},
               {"shl", "<<"}, {"shr", ">>"}, {"and", "&"}, {"or", "|"}, {"xor", "^"}, {"not", "~"}};
    std::string cbuf, sbuf;
    const char* comment = nullptr;
    const char* src = src_in;
    {
        const char* s = src;
        char cq = 0;
        while (*s) {
            if (cq) {
                if (*s == cq) cq = 0;
            } else if (*s == '\'' || *s == '"') {
                cq = *s;
            } else if (*s == ';') {
                comment = s;
                break;
            }
            cbuf += *s++;
        }
        if (comment) src = cbuf.c_str();
    }
    sbuf = mx_paren_shifts(src);
    src = sbuf.c_str();
    std::string out;
    const char* p = src;
    char q = 0;
    while (*p) {
        if (q) {
            if (*p == q) q = 0;
            out += *p++;
            continue;
        }
        if (*p == '\'' || *p == '"') {
            q = *p;
            out += *p++;
            continue;
        }
        if (isidstart(*p) && (p == src || (!isidchar(p[-1]) && p[-1] != '.'))) {
            std::size_t wl = 0;
            int hit = -1;
            while (isidchar(p[wl])) wl++;
            for (int i = 0; i < 13; i++)
                if (std::strlen(ops[i].w) == wl && !strnicmp_(p, ops[i].w, wl)) {
                    hit = i;
                    break;
                }
            if (hit >= 0) {
                out += ops[hit].op;
                p += wl;
                continue;
            }
            out.append(p, wl);
            p += wl;
            continue;
        }
        out += *p++;
    }
    if (comment) {
        out += ' ';
        out += comment;
    }
    return out;
}

}  // namespace

// ---- the state and the transformation ----

struct MasmFront::Impl {
    Assembly& as;
    std::function<bool()> skipping;

    std::deque<std::string> ppq;           // extra lines to hand out first
    std::vector<MxBlk> ppstk;                // open blocks (the back is the innermost)
    int for_seq = 0;
    std::vector<MxSDef> sdefs;               // every structure defined (search from the newest)
    MxSDef cur;                              // the one being defined
    bool have_cur = false;
    bool in_struct = false;
    char comment_delim = 0;
    std::optional<std::string> locif_cond;
    bool locif_ife = false;
    std::vector<std::string> lbufs;
    std::vector<std::pair<std::string, int>> lszs;
    std::unordered_set<std::string> defined_tab, const_tab;   // lower-cased names
    std::deque<MxPending> pendings;
    int anon_seq = 0;

    Impl(Assembly& a, std::function<bool()> s) : as(a), skipping(std::move(s)) {}

    void ppq_add(std::string t) { ppq.push_back(std::move(t)); }
    std::string ppq_get() {
        std::string t = std::move(ppq.front());
        ppq.pop_front();
        return t;
    }
    bool ppq_has() const { return !ppq.empty(); }
    // The next queued line as a result, or "" when nothing is queued (the C code returns NULL there).
    std::optional<std::string> ppq_get_opt() {
        if (ppq.empty()) return std::nullopt;
        return ppq_get();
    }

    MxSDef* sdef_find(std::string_view name) {
        for (auto it = sdefs.rbegin(); it != sdefs.rend(); ++it)
            if (mx_eq(it->name, name)) return &*it;
        return nullptr;
    }
    bool is_field(std::string_view name) {
        for (auto it = sdefs.rbegin(); it != sdefs.rend(); ++it) {
            if (it->is_record) continue;
            for (const MxSMember& m : it->members)
                if (!m.name.empty() && mx_eq(m.name, name)) return true;
        }
        return false;
    }
    int field_bytes(std::string_view name) {
        for (auto it = sdefs.rbegin(); it != sdefs.rend(); ++it) {
            if (it->is_record) continue;
            for (const MxSMember& m : it->members)
                if (!m.name.empty() && mx_eq(m.name, name)) {
                    if (!m.dir) return 0;
                    if (mx_eq(m.dir, "db")) return 1;
                    if (mx_eq(m.dir, "dw")) return 2;
                    if (mx_eq(m.dir, "dd")) return 4;
                    if (mx_eq(m.dir, "dq")) return 8;
                    if (mx_eq(m.dir, "dt")) return 10;
                    return 0;
                }
        }
        return 0;
    }
    bool lbuf_has(std::string_view name) const {
        for (const std::string& b : lbufs)
            if (mx_eq(b, name)) return true;
        return false;
    }
    void lbuf_add(const std::string& name) {
        if (name.empty() || lbuf_has(name)) return;
        lbufs.push_back(name);
    }
    int lsz_get(std::string_view name) const {
        for (const auto& v : lszs)
            if (mx_eq(v.first, name)) return v.second;
        return 0;
    }
    void lsz_add(const std::string& name, int bytes) {
        if (name.empty() || lsz_get(name)) return;
        lszs.emplace_back(name, bytes);
    }
    static bool set_has(const std::unordered_set<std::string>& s, std::string_view n) { return s.count(lcase(n)) != 0; }
    static void set_add(std::unordered_set<std::string>& s, std::string_view n) {
        if (!n.empty()) s.insert(lcase(n));
    }

    // A name got its definition: every pending constant that only waited for it is defined now.
    void note_defined(const std::string& name) {
        set_add(defined_tab, name);
        bool changed = true;
        while (changed) {
            changed = false;
            for (auto pit = pendings.begin(); pit != pendings.end();) {
                auto& deps = pit->deps;
                for (auto d = deps.begin(); d != deps.end();) {
                    if (set_has(defined_tab, *d)) d = deps.erase(d);
                    else ++d;
                }
                if (deps.empty()) {
                    ppq_add("%iassign " + pit->name + " " + pit->expr);
                    set_add(defined_tab, pit->name);
                    pit = pendings.erase(pit);
                    changed = true;
                } else {
                    ++pit;
                }
            }
        }
    }

    void prescan(std::string_view text) {
        std::size_t pos = 0;
        while (true) {
            std::string buf;
            bool eof = false;
            while (true) {
                if (pos >= text.size()) {
                    eof = true;
                    break;
                }
                const char c = text[pos++];
                if (c == '\n') break;
                if (c == '\r') continue;
                if (buf.size() < 1023) buf += c;
            }
            const std::size_t sc = buf.find(';');
            if (sc != std::string::npos) buf.resize(sc);
            const char* p = buf.c_str();
            while (*p == ' ' || *p == '\t') p++;
            if (mx_ps_idchar(static_cast<unsigned char>(*p)) && !isdigit_(*p)) {
                std::string name;
                while (mx_ps_idchar(static_cast<unsigned char>(*p))) name += *p++;
                while (*p == ' ' || *p == '\t') p++;
                if (*p == '=' && p[1] != '=') {
                    set_add(const_tab, name);
                } else if ((p[0] == 'e' || p[0] == 'E') && (p[1] == 'q' || p[1] == 'Q') && (p[2] == 'u' || p[2] == 'U') &&
                           (p[3] == ' ' || p[3] == '\t')) {
                    const char* v = p + 3;
                    while (*v == ' ' || *v == '\t') v++;
                    if (*v && *v != '<' && *v != '[' && !std::strchr(v, '$') && stricmp_(v, "near") && stricmp_(v, "far") &&
                        stricmp_(v, "short"))
                        set_add(const_tab, name);
                }
            }
            if (eof) break;
        }
    }

    // ---- name substitution inside macro bodies ----

    std::string subst_params(const std::string& line, const std::vector<std::string>& param) {
        const char* base = line.c_str();
        const char* p = base;
        std::string out;
        bool changed = false;
        char q = 0;
        while (*p) {
            if (q) {
                if (*p == '&' && isidstart(p[1])) {
                    const char* s = p + 1;
                    std::size_t wl = 0;
                    int hit = -1;
                    while (isidchar(s[wl])) wl++;
                    for (std::size_t i = 0; i < param.size(); i++)
                        if (param[i].size() == wl && !std::memcmp(s, param[i].data(), wl)) {
                            hit = static_cast<int>(i);
                            break;
                        }
                    if (hit >= 0) {
                        out += std::format("%{{{}}}", hit + 1);
                        p = s + wl;
                        if (*p == '&') p++;
                        changed = true;
                        continue;
                    }
                }
                if (*p == q) q = 0;
                out += *p++;
                continue;
            }
            if (*p == '\'' || *p == '"') {
                q = *p;
                out += *p++;
                continue;
            }
            if (*p == '%' && (isalpha_(p[1]) || p[1] == '_')) {
                bool leading = true;
                for (const char* b = base; b < p; b++)
                    if (*b != ' ' && *b != '\t') {
                        leading = false;
                        break;
                    }
                if (!leading) {
                    const char* s = p + 1;
                    while (isidchar(*s) || *s == '.') s++;
                    out += "%[";
                    out.append(p + 1, static_cast<std::size_t>(s - (p + 1)));
                    out += ']';
                    p = s;
                    changed = true;
                    continue;
                }
            }
            if (*p == '&' && ((p > base && isidchar(p[-1])) || isidchar(p[1])) && strnicmp_(p + 1, "macro", 5) &&
                strnicmp_(p + 1, "endm", 4)) {
                p++;
                changed = true;
                continue;
            }
            if (isidstart(*p) && (p == base || (!isidchar(p[-1]) && p[-1] != '%' && p[-1] != '.'))) {
                std::size_t wl = 0;
                int hit = -1;
                while (isidchar(p[wl])) wl++;
                for (std::size_t i = 0; i < param.size(); i++)
                    if (param[i].size() == wl && !std::memcmp(p, param[i].data(), wl)) {
                        hit = static_cast<int>(i);
                        break;
                    }
                if (hit >= 0) {
                    out += std::format("%{{{}}}", hit + 1);
                    p += wl;
                    changed = true;
                    continue;
                }
                out.append(p, wl);
                p += wl;
                continue;
            }
            out += *p++;
        }
        return changed ? out : line;
    }

    std::string subst_outer(const std::string& line, const std::vector<std::string>& oparam, int oseq) {
        if (oparam.empty()) return line;
        const char* base = line.c_str();
        const char* p = base;
        std::string out;
        bool changed = false;
        char q = 0;
        while (*p) {
            if (q) {
                if (*p == q) q = 0;
                out += *p++;
                continue;
            }
            if (*p == '\'' || *p == '"') {
                q = *p;
                out += *p++;
                continue;
            }
            if (isidstart(*p) && (p == base || (!isidchar(p[-1]) && p[-1] != '%' && p[-1] != '.'))) {
                std::size_t wl = 0;
                int hit = -1;
                while (isidchar(p[wl])) wl++;
                for (std::size_t i = 0; i < oparam.size(); i++)
                    if (oparam[i].size() == wl && !std::memcmp(p, oparam[i].data(), wl)) {
                        hit = static_cast<int>(i);
                        break;
                    }
                if (hit >= 0) {
                    out += std::format("%[_mo{}_{}]", oseq, hit + 1);
                    p += wl;
                    changed = true;
                    continue;
                }
                out.append(p, wl);
                p += wl;
                continue;
            }
            out += *p++;
        }
        return changed ? out : line;
    }

    // IRP / FOR: the loop variable pasted into tokens with "&".
    std::string for_paste(const std::string& line, const std::string& var) {
        const std::size_t vl = var.size();
        if (!vl) return line;
        const char* base = line.c_str();
        const char* p = base;
        std::string out;
        bool changed = false;
        char q = 0;
        while (*p) {
            if (q) {
                if (*p == q) q = 0;
                out += *p++;
                continue;
            }
            if (*p == '\'' || *p == '"') {
                q = *p;
                out += *p++;
                continue;
            }
            if (*p == '&' && p[1] == '&' && !strnicmp_(p + 2, var, vl) && !isidchar(p[2 + vl])) {
                out += "%[" + var + "]";
                p += 2 + vl;
                if (p[0] == '&' && p[1] == '&') p += 2;
                changed = true;
                continue;
            }
            if (*p == '&' && !strnicmp_(p + 1, var, vl) && !isidchar(p[1 + vl]) && strnicmp_(p + 1, "macro", 5) &&
                strnicmp_(p + 1, "endm", 4)) {
                out += "%[" + var + "]";
                p += 1 + vl;
                changed = true;
                continue;
            }
            if (isidstart(*p) && !strnicmp_(p, var, vl) && p[vl] == '&' && (p == base || !isidchar(p[-1]))) {
                out += "%[" + var + "]";
                p += vl + 1;
                if (*p == '&') p++;
                changed = true;
                continue;
            }
            out += *p++;
        }
        return changed ? out : line;
    }

    // ---- small rewrites ----

    // "es: dword ptr" -> "dword ptr es:" (the size before the segment override).
    bool reorder_segsize(const std::string& line, std::string& out) {
        const char* base = line.c_str();
        const char* p = base;
        out.clear();
        bool changed = false;
        char qc = 0;
        while (*p) {
            if (qc) {
                if (*p == qc) qc = 0;
                out += *p++;
                continue;
            }
            if (*p == '\'' || *p == '"') {
                qc = *p;
                out += *p++;
                continue;
            }
            if (*p == ';') {
                out += p;
                break;
            }
            if ((p == base || !isidchar(p[-1])) && mx_is_segreg2(p) && !isidchar(p[2]) && p[2] == ':') {
                const char* q = p + 3;
                std::string sz;
                while (*q == ' ' || *q == '\t') q++;
                const char* r = q;
                while (isidchar(*r) && sz.size() + 1 < 16) sz += *r++;
                if (!sz.empty() && mx_type_bytes(sz) > 0) {
                    const char* t = r;
                    while (*t == ' ' || *t == '\t') t++;
                    if (!strnicmp_(t, "ptr", 3) && !isidchar(t[3])) {
                        out += std::format("{} ptr {}{}:", sz, p[0], p[1]);
                        p = t + 3;
                        changed = true;
                        continue;
                    }
                }
            }
            out += *p++;
        }
        return changed;
    }

    // Parentheses around a whole operand are dropped ("mov ax, (1+2)" -> "mov ax, 1+2"); in place.
    void strip_operand_parens(std::string& linestr) {
        std::string& s0 = linestr;
        std::size_t s = 0;
        int bdepth = 0;
        bool opstart;
        auto at = [&](std::size_t i) -> char { return i < s0.size() ? s0[i] : '\0'; };
        while (at(s) == ' ' || at(s) == '\t') s++;
        {
            const std::size_t save = s;
            while (at(s) && at(s) != ' ' && at(s) != '\t' && at(s) != ';') s++;
            if (s > save && s0[s - 1] == ':') {
                while (at(s) == ' ' || at(s) == '\t') s++;
                while (at(s) && at(s) != ' ' && at(s) != '\t' && at(s) != ';') s++;
            }
        }
        opstart = true;
        for (; at(s); s++) {
            if (at(s) == ';') break;
            if (at(s) == '\'' || at(s) == '"') {
                const char q = at(s++);
                while (at(s) && at(s) != q) s++;
                if (!at(s)) break;
                opstart = false;
                continue;
            }
            if (at(s) == '[') { bdepth++; opstart = false; continue; }
            if (at(s) == ']') { if (bdepth) bdepth--; opstart = false; continue; }
            if (at(s) == ',' && bdepth == 0) { opstart = true; continue; }
            if (at(s) == ' ' || at(s) == '\t') continue;
            if (at(s) == ':' && bdepth == 0) { opstart = true; continue; }
            if (at(s) == '(' && bdepth == 0) {
                bool cand = opstart;
                if (!cand) {
                    std::size_t b = s;
                    while (b > 0 && (s0[b - 1] == ' ' || s0[b - 1] == '\t')) b--;
                    if (b >= 3 && !strnicmp_(std::string_view(s0).substr(b - 3), "ptr", 3) && (b == 3 || !isidchar(s0[b - 4]))) cand = true;
                }
                if (cand) {
                    int pd = 1;
                    std::size_t e = s + 1;
                    for (; at(e) && at(e) != ';'; e++) {
                        if (at(e) == '(') pd++;
                        else if (at(e) == ')') {
                            if (--pd == 0) break;
                        } else if (at(e) == '\'' || at(e) == '"') {
                            const char q = at(e++);
                            while (at(e) && at(e) != q) e++;
                            if (!at(e)) break;
                        }
                    }
                    if (pd == 0 && at(e) == ')') {
                        std::size_t n = e + 1;
                        while (at(n) == ' ' || at(n) == '\t') n++;
                        if (at(n) == '\0' || at(n) == ',' || at(n) == ';') {
                            s0.erase(e, 1);
                            s0.erase(s, 1);
                            s--;
                            opstart = true;
                            continue;
                        }
                    }
                }
                opstart = false;
                continue;
            }
            opstart = false;
        }
    }

    // ---- the expression and operand rewriter (masm_rewrite_line) ----

    std::string rewrite_line(std::string line) {
        const char* p;
        std::string out, reord;
        bool changed = false;
        int depth = 0;
        strip_operand_parens(line);
        if (reorder_segsize(line, reord)) line = reord;
        const char* const base = line.c_str();
        p = base;
        while (*p) {
            if (*p == '\'' || *p == '"') {
                const char qc = *p;
                out += *p++;
                while (*p && *p != qc) out += *p++;
                if (*p) out += *p++;
                continue;
            }
            if (*p == '%' && p[1] == '(') {
                const char* q = p + 2;
                int d = 1;
                char qc = 0;
                for (; *q && d > 0; q++) {
                    if (qc) {
                        if (*q == qc) qc = 0;
                    } else if (*q == '\'' || *q == '"') qc = *q;
                    else if (*q == '(') d++;
                    else if (*q == ')' && --d == 0) break;
                }
                out += "%[";
                for (const char* r = p + 2; r < q; r++) out += *r;
                out += ']';
                p = (*q == ')') ? q + 1 : q;
                changed = true;
                continue;
            }
            if (*p == ']' && p[1] == '[') {
                out += " + ";
                p += 2;
                changed = true;
                continue;
            }
            if ((p == base || !isidchar(p[-1])) && !strnicmp_(p, "this", 4) && !isidchar(p[4])) {
                const char* q = p + 4;
                while (*q == ' ' || *q == '\t') q++;
                if (isidstart(*q)) {
                    const char* e = q;
                    std::string kw;
                    while (isidchar(*e) && kw.size() + 1 < 16) kw += *e++;
                    if (mx_type_bytes(kw) > 0) q = e;
                }
                out += '$';
                p = q;
                changed = true;
                continue;
            }
            if (*p == '(' && (p == base || !isidchar(p[-1])) && isidstart(p[1])) {
                const char* q = p + 1;
                while (isidchar(*q)) q++;
                if (*q == ')') {
                    out.append(p + 1, static_cast<std::size_t>(q - (p + 1)));
                    p = q + 1;
                    changed = true;
                    continue;
                }
            }
            if (*p == ']' && p[1] == '.') {
                const char* q = p + 2;
                const char* ms = q;
                std::size_t mlen, d2, fc;
                int half = -1;
                while (isidchar(*q) || *q == '.') q++;
                mlen = static_cast<std::size_t>(q - ms);
                if (((mlen == 4 && !strnicmp_(ms, "size", 4)) || (mlen == 6 && !strnicmp_(ms, "sizeof", 6)) ||
                     (mlen == 4 && !strnicmp_(ms, "type", 4))) &&
                    (*q == ' ' || *q == '\t')) {
                    const char* a = q;
                    while (*a == ' ' || *a == '\t') a++;
                    if (isidstart(*a)) {
                        const char* ts = a;
                        while (isidchar(*a) || *a == '.') a++;
                        out += " + ";
                        out.append(ts, static_cast<std::size_t>(a - ts));
                        out += "_size]";
                        p = a;
                        if (depth > 0) depth--;
                        changed = true;
                        continue;
                    }
                }
                mlen = static_cast<std::size_t>(q - ms);
                fc = 0;
                while (fc < mlen && ms[fc] != '.') fc++;
                if (fc < mlen) {
                    if (fc < 128) {
                        MxSDef* td = sdef_find(std::string_view(ms, fc));
                        if (td && !td->is_union && !td->is_record) ms += fc + 1;
                    }
                }
                mlen = static_cast<std::size_t>(q - ms);
                fc = 0;
                while (fc < mlen && ms[fc] != '.') fc++;
                d2 = mlen;
                while (d2 > 0 && ms[d2 - 1] != '.') d2--;
                if (d2 > 0 && *q != '[') {
                    if (fc < 128) {
                        if (!sdef_find(std::string_view(ms, fc))) half = mx_farhalf(std::string_view(ms + d2, mlen - d2));
                    }
                }
                if (half >= 0) mlen = d2 - 1;
                out += " + ";
                out.append(ms, mlen);
                if (half == 2) out += " + 2";
                if (*q == '[') {
                    out += " + ";
                    p = q + 1;
                    changed = true;
                    continue;
                }
                out += ']';
                p = q;
                if (depth > 0) depth--;
                changed = true;
                continue;
            }
            if (isidstart(*p) && (p == base || (!isidchar(p[-1]) && p[-1] != '.'))) {
                std::size_t wl = 0;
                const char* pre = nullptr;
                const char* post = nullptr;
                while (isidchar(p[wl])) wl++;
                {
                    const char* sym = nullptr;
                    bool unary = false;
                    char shift = 0;
                    if (wl == 3 && !strnicmp_(p, "and", 3)) sym = "&";
                    else if (wl == 2 && !strnicmp_(p, "or", 2)) sym = "|";
                    else if (wl == 3 && !strnicmp_(p, "xor", 3)) sym = "^";
                    else if (wl == 3 && !strnicmp_(p, "shl", 3)) { sym = ""; shift = 'l'; }
                    else if (wl == 3 && !strnicmp_(p, "shr", 3)) { sym = ""; shift = 'r'; }
                    else if (wl == 3 && !strnicmp_(p, "not", 3)) { sym = "~"; unary = true; }
                    if (sym) {
                        char prevsig = 0;
                        std::size_t b = out.size();
                        while (b > 0 && (out[b - 1] == ' ' || out[b - 1] == '\t')) b--;
                        if (b > 0) prevsig = out[b - 1];
                        const bool vend = prevsig == ')' || prevsig == ']' || prevsig == '_' || prevsig == '?' || prevsig == '$' ||
                                          prevsig == '\'' || prevsig == '"' || (prevsig >= '0' && prevsig <= '9') ||
                                          (prevsig >= 'A' && prevsig <= 'Z') || (prevsig >= 'a' && prevsig <= 'z');
                        bool isop;
                        if (unary) isop = prevsig != 0 && prevsig != ':' && !vend;
                        else isop = vend;
                        if (isop && shift) {
                            const char* q = p + wl;
                            out += std::format(" {} (1 << ", shift == 'l' ? '*' : '/');
                            while (*q == ' ' || *q == '\t') q++;
                            while (*q == '+' || *q == '-' || *q == '~') out += *q++;
                            while (*q == ' ' || *q == '\t') q++;
                            if (*q == '(') {
                                int d = 0;
                                do {
                                    if (*q == '(') d++;
                                    else if (*q == ')') d--;
                                    out += *q++;
                                } while (*q && d > 0);
                            } else {
                                while (isidchar(*q) || *q == '.' || *q == '$') out += *q++;
                            }
                            out += ')';
                            p = q;
                            changed = true;
                            continue;
                        }
                        if (isop) {
                            out += sym;
                            p += wl;
                            changed = true;
                            continue;
                        }
                    }
                }
                {
                    const char* pr = nullptr;
                    if (wl == 6 && !strnicmp_(p, "sizeof", 6)) pr = "";
                    else if (wl == 4 && !strnicmp_(p, "size", 4)) pr = "";
                    else if (wl == 4 && !strnicmp_(p, "type", 4)) pr = "";
                    else if (wl == 4 && !strnicmp_(p, "mask", 4)) pr = "MASK_";
                    else if (wl == 5 && !strnicmp_(p, "width", 5)) pr = "WIDTH_";
                    if (pr) {
                        const char* a = p + wl;
                        bool paren = false;
                        while (*a == ' ' || *a == '\t') a++;
                        if (*a == '(') {
                            paren = true;
                            a++;
                            while (*a == ' ' || *a == '\t') a++;
                        }
                        if (isidstart(*a)) {
                            const char* ns = a;
                            while (isidchar(*a) || *a == '.') a++;
                            if (pr[0]) {
                                out += pr;
                                out.append(ns, static_cast<std::size_t>(a - ns));
                            } else {
                                out.append(ns, static_cast<std::size_t>(a - ns));
                                out += "_size";
                            }
                            if (paren) {
                                while (*a == ' ' || *a == '\t') a++;
                                if (*a == ')') a++;
                            }
                            p = a;
                            changed = true;
                            continue;
                        }
                    }
                }
                if (wl == 7 && !strnicmp_(p, "lowword", 7)) { pre = "(("; post = ") & 0FFFFh)"; }
                else if (wl == 8 && !strnicmp_(p, "highword", 8)) { pre = "(("; post = " >> 16) & 0FFFFh)"; }
                else if (wl == 3 && !strnicmp_(p, "low", 3)) { pre = "(("; post = ") & 0FFh)"; }
                else if (wl == 4 && !strnicmp_(p, "high", 4)) { pre = "(("; post = " >> 8) & 0FFh)"; }
                if (pre) {
                    const char* a = p + wl;
                    while (*a == ' ' || *a == '\t') a++;
                    const char* ts = a;
                    if (*a == '(') {
                        int d = 0;
                        do {
                            if (*a == '(') d++;
                            else if (*a == ')') d--;
                            a++;
                        } while (*a && d > 0);
                    } else {
                        while (isidchar(*a) || *a == '.' || *a == '$') a++;
                    }
                    if (a > ts) {
                        out += pre;
                        out.append(ts, static_cast<std::size_t>(a - ts));
                        out += post;
                        p = a;
                        changed = true;
                        continue;
                    }
                }
                if (p[0] != '.') {
                    std::size_t dp = 0;
                    while (dp < wl && p[dp] != '.') dp++;
                    if (dp > 0 && dp + 1 < wl && dp < 120) {
                        const char* fp = p + dp + 1;
                        std::size_t fl = 0;
                        std::string f1;
                        while (fp + fl < p + wl && fp[fl] != '.' && fl + 1 < 128) {
                            f1 += fp[fl];
                            fl++;
                        }
                        const std::string base_s(p, dp);
                        const std::string full = wl < 128 ? std::string(p, wl) : std::string();
                        {
                            MxSDef* bt = sdef_find(base_s);
                            if (depth == 0 && bt && !bt->is_union && !bt->is_record && is_field(f1) && as.masm_type_query(full) == 0) {
                                out.append(p + dp + 1, wl - dp - 1);
                                p += wl;
                                changed = true;
                                continue;
                            }
                        }
                        const bool reg_in_br = depth > 0 && mx_is_gpreg(base_s);
                        const bool numfield = !f1.empty() && f1.find_first_not_of("0123456789") == std::string::npos;
                        const bool constfield = reg_in_br && set_has(const_tab, f1);
                        if (((is_field(f1) && (as.masm_type_query(base_s) > 0 || lbuf_has(base_s) || reg_in_br)) ||
                             ((numfield || constfield) && reg_in_br)) &&
                            !sdef_find(base_s) && as.masm_type_query(full) == 0) {
                            const char* fld = p + dp + 1;
                            std::size_t fln = wl - dp - 1;
                            std::size_t d2 = fln;
                            int half = -1;
                            while (d2 > 0 && fld[d2 - 1] != '.') d2--;
                            if (d2 > 0) half = mx_farhalf(std::string_view(fld + d2, fln - d2));
                            if (half >= 0) fln = d2 - 1;
                            if (depth == 0) {
                                out += half >= 0 ? "word " : "";
                                out += '[';
                                out.append(p, dp);
                                out += " + ";
                                out.append(fld, fln);
                                if (half == 2) out += " + 2";
                                out += ']';
                            } else {
                                out.append(p, dp);
                                out += " + ";
                                out.append(fld, fln);
                                if (half == 2) out += " + 2";
                            }
                            p += wl;
                            changed = true;
                            continue;
                        }
                    }
                }
                if (depth == 0 && p[wl] == '[' && wl < 120) {
                    bool dotted = false;
                    for (std::size_t k = 0; k < wl; k++)
                        if (p[k] == '.') {
                            dotted = true;
                            break;
                        }
                    if (!dotted) {
                        const std::string nm(p, wl);
                        if (as.masm_type_query(nm) > 0) {
                            out += '[';
                            out.append(p, wl);
                            out += " + ";
                            p += wl + 1;
                            depth++;
                            changed = true;
                            continue;
                        }
                    }
                }
                out.append(p, wl);
                p += wl;
                continue;
            }
            if (*p == '[') depth++;
            else if (*p == ']' && depth > 0) depth--;
            out += *p++;
        }
        return changed ? out : line;
    }

    // ---- guards and extern lists ----

    void emit_opt_guards(const std::string& expr) {
        const char* base = expr.c_str();
        const char* p = base;
        std::vector<std::string> seen;
        while (*p) {
            if (*p == '?' && isidstart(p[1]) && (p == base || (!isidchar(p[-1]) && p[-1] != '.'))) {
                std::string nm;
                nm += *p++;
                while (nm.size() + 1 < 64 && isidchar(*p)) nm += *p++;
                bool found = false;
                for (const std::string& s : seen)
                    if (s == nm) found = true;
                if (!found && seen.size() < 16) {
                    seen.push_back(nm);
                    ppq_add("%ifndef " + nm);
                    ppq_add("%assign " + nm + " 0");
                    ppq_add("%endif");
                }
            } else {
                p++;
            }
        }
    }

    std::string extern_emit(const char* rest, int deftype) {
        const char* r = rest;
        bool any = false;
        while (*r && *r != ';') {
            std::string nm;
            int sz = deftype;
            while (*r == ' ' || *r == '\t' || *r == ',' || *r == '<' || *r == '>') r++;
            if (!*r || *r == ';') break;
            while (*r && *r != ',' && *r != ':' && *r != ';' && *r != ' ' && *r != '\t' && *r != '<' && *r != '>') {
                if (nm.size() + 1 < 128) nm += *r;
                r++;
            }
            while (*r == ' ' || *r == '\t') r++;
            if (*r == ':') {
                r++;
                while (*r == ' ' || *r == '\t') r++;
                const char* ts = r;
                while (*r && *r != ',' && *r != ';' && *r != ' ' && *r != '\t' && *r != '>') r++;
                sz = mx_type_from_word(std::string_view(ts, static_cast<std::size_t>(r - ts)));
            }
            if (!nm.empty()) {
                as.masm_type_note(nm, sz);
                ppq_add("extern " + nm);
                any = true;
            }
        }
        if (!any) ppq_add("");
        return ppq_get();
    }

    // "@@:" anonymous labels and "@F" / "@B" references.
    bool rewrite_anon(const std::string& line_s, std::string& out) {
        if (skipping && skipping()) return false;
        const char* base = line_s.c_str();
        const char* p = base;
        bool changed = false;
        char qc = 0;
        out.clear();
        while (*p) {
            if (qc) {
                if (*p == qc) qc = 0;
                out += *p++;
                continue;
            }
            if (*p == '\'' || *p == '"') {
                qc = *p;
                out += *p++;
                continue;
            }
            if (*p == ';') {
                out += p;
                p += std::strlen(p);
                break;
            }
            if (*p == '@' && p[1] == '@' && p[2] == ':' && (p == base || (!isidchar(p[-1]) && p[-1] != '@'))) {
                out += std::format("__masm_anon_{}:", anon_seq);
                anon_seq++;
                p += 3;
                changed = true;
                continue;
            }
            if (*p == '@' && (p[1] == 'F' || p[1] == 'f' || p[1] == 'B' || p[1] == 'b') && !isidchar(p[2]) && p[2] != '@' &&
                (p == base || (!isidchar(p[-1]) && p[-1] != '@'))) {
                const int n = (p[1] == 'B' || p[1] == 'b') ? anon_seq - 1 : anon_seq;
                out += std::format("__masm_anon_{}", n);
                p += 2;
                changed = true;
                continue;
            }
            out += *p++;
        }
        return changed;
    }

    // A numeric constant: "%iassign" now, or held until the constants it uses are defined. Returns the line to hand out.
    std::optional<std::string> emit_num_const(const std::string& name, const std::string& expr) {
        std::vector<std::string> fdeps;
        const char* s = expr.c_str();
        while (*s) {
            if (isidchar(*s)) {
                const char* st = s;
                const bool isnum = isdigit_(*s);
                while (isidchar(*s)) s++;
                if (!isnum && !mx_is_wordop(std::string_view(st, static_cast<std::size_t>(s - st)))) {
                    const std::string id(st, static_cast<std::size_t>(s - st));
                    if (id.size() < 128) {
                        if (mx_eq(id, name) || set_has(defined_tab, id)) {
                        } else if (set_has(const_tab, id)) {
                            bool dup = false;
                            for (const std::string& d : fdeps)
                                if (mx_eq(d, id)) dup = true;
                            if (!dup) fdeps.push_back(id);
                        } else {
                            return std::nullopt;
                        }
                    }
                }
            } else {
                s++;
            }
        }
        if (!fdeps.empty()) {
            pendings.push_front({name, expr, fdeps});
            return std::string();
        }
        ppq_add("%iassign " + name + " " + expr);
        note_defined(name);
        return ppq_get();
    }

    // ---- structure instances ----

    // The "<a, b, c>" initializer list at `ip` (after the opening <).
    static std::vector<std::string> split_inits(const char* ip) {
        std::vector<std::string> inits(1);
        int depth = 0;
        for (const char* s = ip; *s; s++) {
            if (*s == '<') depth++;
            else if (*s == '>') {
                if (depth == 0) break;
                depth--;
            }
            if (*s == ',' && depth == 0) {
                if (inits.size() < 32) inits.emplace_back();
                continue;
            }
            if (inits.back().size() + 1 < 192) inits.back() += *s;
        }
        return inits;
    }
    static std::string clean_init(const std::string& in) {
        std::size_t b = 0;
        while (b < in.size() && (in[b] == ' ' || in[b] == '\t')) b++;
        std::string v = in.substr(b);
        while (!v.empty() && (v.back() == ' ' || v.back() == '\t' || v.back() == '\r')) v.pop_back();
        return v;
    }

    std::string struct_instance(MxSDef& sd, const std::string& ilabel, const char* ip) {
        std::vector<std::string> inits;
        int ninit = 0;
        while (*ip == ' ' || *ip == '\t') ip++;
        if (*ip == '<') {
            inits = split_inits(ip + 1);
            ninit = static_cast<int>(inits.size());
        }
        if (sd.is_record) {
            std::string rhs;
            bool first = true;
            int mi = 0;
            for (const MxSMember& mb : sd.members) {
                std::string v = "0";
                if (mi < ninit) {
                    const std::string val = clean_init(inits[static_cast<std::size_t>(mi)]);
                    if (!val.empty()) v = val;
                }
                rhs += std::format("{}(({}) << {})", first ? "" : " | ", v, mb.shift);
                first = false;
                mi++;
            }
            if (first) rhs = "0";
            ppq_add(std::format("{}\t{} {}", ilabel, sd.rdir, rhs));
            return ppq_get();
        }
        ppq_add(ilabel + ":");
        int mi = 0;
        for (const MxSMember& mb : sd.members) {
            std::string v;
            bool have_v = false;
            if (mi < ninit) {
                const std::string val = clean_init(inits[static_cast<std::size_t>(mi)]);
                if (!val.empty()) {
                    v = val;
                    have_v = true;
                }
            }
            mi++;
            if (!have_v) {
                const std::string& dv = mb.def;
                if (dv.empty() || (dv[0] == '?' && dv.size() == 1)) v = "0";
                else v = rewrite_line(dv);
            }
            const char* dir = mb.dir ? mb.dir : "dd";
            if (!stricmp_(dir, "db") && (v[0] == '"' || v[0] == '\'')) {
                const std::size_t vl = v.size();
                if (vl >= 4 && v[vl - 1] == v[0] && v[1] == '%' && (v[2] == '{' || v[2] == '[')) {
                    const std::string pref = v.substr(1, vl - 2);
                    int W = 0;
                    if (!mb.def.empty()) {
                        const std::size_t q = mb.def.find_first_of("\"'");
                        if (q != std::string::npos) {
                            const std::size_t e = mb.def.find(mb.def[q], q + 1);
                            if (e != std::string::npos) W = static_cast<int>(e - (q + 1));
                        }
                    }
                    if (W > 0) {
                        ppq_add("%defstr _masm_pstr " + pref);
                        ppq_add("%strlen _masm_pslen _masm_pstr");
                        ppq_add(std::format("{}.{}\tdb _masm_pstr", ilabel, mb.name));
                        ppq_add(std::format("%if _masm_pslen < {}", W));
                        ppq_add(std::format("times ({} - _masm_pslen) db ' '", W));
                        ppq_add("%endif");
                        continue;
                    }
                }
            }
            {
                std::string pk;
                if (stricmp_(dir, "db") && mx_pack_char_literal(v.c_str(), pk)) v = pk;
            }
            if (mb.count > 1) ppq_add(std::format("{}.{}\ttimes {} {} {}", ilabel, mb.name, mb.count, dir, v));
            else ppq_add(std::format("{}.{}\t{} {}", ilabel, mb.name, dir, v));
        }
        return ppq_get();
    }

    std::optional<std::string> paste_struct_instance(const std::string& line) {
        const char* s = line.c_str();
        std::string pbuf, sn;
        while (*s == ' ' || *s == '\t') s++;
        while (*s && *s != ' ' && *s != '\t' && pbuf.size() + 1 < 256) pbuf += *s++;
        while (*s == ' ' || *s == '\t') s++;
        while (isidchar(*s) && sn.size() + 1 < 128) sn += *s++;
        while (*s == ' ' || *s == '\t') s++;
        if (pbuf.empty() || sn.empty() || *s != '<' || pbuf.find_first_of("%&") == std::string::npos) return std::nullopt;
        MxSDef* sd = sdef_find(sn);
        if (!sd) return std::nullopt;
        return struct_instance(*sd, pbuf, s);
    }

    std::string xform(std::string line);
};

}  // namespace cb::build::x86asm

#include "build/x86asm/preproc_masm_xform.inc"
