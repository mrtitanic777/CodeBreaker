// cb/decompile/c_text_dos.cpp -- see c_text_dos.hpp.
#include "cb/decompile/c_text_dos.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <format>
#include <map>
#include <optional>
#include <set>
#include <vector>

#include "cb/core/text.hpp"

namespace cb::decompile::c_text_dos {

namespace {

bool ctd_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool ctd_hex(char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; }

std::string_view ctd_trim(std::string_view s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front())) != 0) s.remove_prefix(1);
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())) != 0) s.remove_suffix(1);
    return s;
}

// The index just past the ')' matching the '(' at `open` (the end of `s` when there is none).
// Past a comment or a string or character literal that starts at `k` (`/* ",..." */`: its commas and parentheses are not the
// code's); `k` itself when none starts there.
std::size_t ctd_skip(std::string_view s, std::size_t k) {
    if (s.substr(k, 2) == "/*") {
        const std::size_t e = s.find("*/", k + 2);
        return e == std::string_view::npos ? s.size() : e + 2;
    }
    if (s[k] != '"' && s[k] != '\'') return k;
    for (std::size_t e = k + 1; e < s.size(); ++e) {
        if (s[e] == '\\') ++e;
        else if (s[e] == s[k]) return e + 1;
    }
    return s.size();
}

std::size_t ctd_close(std::string_view s, std::size_t open) {
    int depth = 0;
    for (std::size_t k = open; k < s.size(); ++k) {
        if (const std::size_t past = ctd_skip(s, k); past != k) {
            k = past - 1;
            continue;
        }
        if (s[k] == '(') ++depth;
        else if (s[k] == ')' && --depth == 0) return k + 1;
    }
    return s.size();
}

// The arguments of a call (or the parameters of a declaration): split at the commas outside parentheses.
std::vector<std::string> ctd_args(std::string_view s) {
    std::vector<std::string> out;
    if (ctd_trim(s).empty()) return out;
    int depth = 0;
    std::string cur;
    for (std::size_t k = 0; k < s.size(); ++k) {
        if (const std::size_t past = ctd_skip(s, k); past != k) {
            cur += s.substr(k, past - k);
            k = past - 1;
            continue;
        }
        const char c = s[k];
        if (c == ',' && depth == 0) {
            out.emplace_back(ctd_trim(cur));
            cur.clear();
            continue;
        }
        if (c == '(') ++depth;
        if (c == ')') --depth;
        cur += c;
    }
    out.emplace_back(ctd_trim(cur));
    return out;
}

// `word` at `at` in `s` stands alone (not part of a longer identifier).
bool ctd_whole(std::string_view s, std::size_t at, std::size_t length) {
    return (at == 0 || !ctd_word(s[at - 1])) && (at + length >= s.size() || !ctd_word(s[at + length]));
}

std::size_t ctd_count(std::string_view s, std::string_view word) {
    std::size_t n = 0;
    for (std::size_t at = s.find(word); at != std::string_view::npos; at = s.find(word, at + word.size()))
        if (ctd_whole(s, at, word.size())) ++n;
    return n;
}

// Every identifier of `s`, in order.
std::vector<std::string_view> ctd_identifiers(std::string_view s) {
    std::vector<std::string_view> out;
    for (std::size_t k = 0; k < s.size();) {
        if (ctd_word(s[k]) && std::isdigit(static_cast<unsigned char>(s[k])) == 0 && (k == 0 || !ctd_word(s[k - 1]))) {
            std::size_t e = k;
            while (e < s.size() && ctd_word(s[e])) ++e;
            out.push_back(s.substr(k, e - k));
            k = e;
        } else {
            ++k;
        }
    }
    return out;
}

// A line that begins a function: its return type and convention, its name, its parameters, on one line of its own.
bool ctd_signature(std::string_view line) {
    if (line.empty() || std::isspace(static_cast<unsigned char>(line.front())) != 0) return false;
    if (line.front() == '#' || line.front() == '/' || line.front() == '{' || line.front() == '}') return false;
    const std::string_view t = ctd_trim(line);
    if (t.empty() || t.back() != ')' || t.find(';') != std::string_view::npos) return false;
    return t.find("__cdecl16") != std::string_view::npos || t.find("__stdcall16") != std::string_view::npos ||
           t.find("__fastcall16") != std::string_view::npos;
}

std::string ctd_name(std::string_view signature) {
    const std::size_t open = signature.find('(');
    std::size_t e = open;
    while (e > 0 && signature[e - 1] == ' ') --e;
    std::size_t b = e;
    while (b > 0 && ctd_word(signature[b - 1])) --b;
    return std::string(signature.substr(b, e - b));
}

std::vector<std::string> ctd_params(std::string_view signature) {
    const std::size_t open = signature.find('(');
    const std::size_t close = signature.rfind(')');
    const std::string_view inside = ctd_trim(signature.substr(open + 1, close - open - 1));
    if (inside.empty() || inside == "void") return {};
    return ctd_args(inside);
}

// The declared name of a parameter or local: its last identifier.
std::string ctd_declared(std::string_view declaration) {
    const std::vector<std::string_view> ids = ctd_identifiers(declaration);
    return ids.empty() ? std::string() : std::string(ids.back());
}

// The words a parameter of this type is (large model: a data pointer is far, two words), 0 for one that is not a type this
// knows the size of.
std::size_t ctd_words(std::string_view type) {
    type = ctd_trim(type);
    if (type.ends_with('*')) return 2;
    for (const std::string_view two : {"int4", "uint4", "xunknown4", "long", "unsigned long", "float4"})
        if (type == two) return 2;
    for (const std::string_view one : {"int2", "uint2", "xunknown2", "int", "unsigned", "short", "unsigned short", "int1", "uint1",
                                       "xunknown1", "char", "unsigned char"})
        if (type == one) return 1;
    return 0;
}

// Each call in `body` of a function of `merged` that passes its words one by one (as many arguments as the words of its
// parameters, more than its parameters) passes each two-word parameter's pair as one value.
std::string ctd_join_words(const std::string& body, const std::map<std::string, std::vector<std::string>>& merged) {
    std::string out = body;
    for (std::size_t k = 0; k < out.size(); ++k) {
        if (const std::size_t past = ctd_skip(out, k); past != k) {
            k = past - 1;
            continue;
        }
        if (!std::isalpha(static_cast<unsigned char>(out[k])) && out[k] != '_') continue;
        std::size_t e = k;
        while (e < out.size() && ctd_word(out[e])) ++e;
        if (k > 0 && ctd_word(out[k - 1])) { k = e - 1; continue; }
        const auto m = merged.find(out.substr(k, e - k));
        if (m == merged.end() || e >= out.size() || out[e] != '(') { k = e - 1; continue; }
        const std::size_t end = ctd_close(out, e);
        const std::vector<std::string> args = ctd_args(std::string_view(out).substr(e + 1, end - e - 2));
        std::size_t words = 0;
        for (const std::string& t : m->second) words += ctd_words(t);
        if (args.size() != words || args.size() == m->second.size()) { k = e - 1; continue; }
        std::vector<std::string> joined;
        std::size_t a = 0;
        for (const std::string& t : m->second) {
            if (ctd_words(t) == 2) joined.push_back(std::format("({})CONCAT22({},{})", t, args[a + 1], args[a])), a += 2;
            else joined.push_back(args[a++]);
        }
        const std::string call = "(" + text::join(joined, ",") + ")";
        out.replace(e, end - e, call);
        k = e;   // (the arguments' own calls next)
    }
    return out;
}

// Each call in `body` of a function of `reads` with more arguments than it has parameters, where every argument past them
// does nothing (no call, no store, no increment), written with as many as it has.
std::string ctd_drop_surplus(const std::string& body, const std::map<std::string, std::size_t>& reads) {
    std::string out = body;
    for (std::size_t k = 0; k < out.size(); ++k) {
        if (const std::size_t past = ctd_skip(out, k); past != k) {
            k = past - 1;
            continue;
        }
        if (!std::isalpha(static_cast<unsigned char>(out[k])) && out[k] != '_') continue;
        std::size_t e = k;
        while (e < out.size() && ctd_word(out[e])) ++e;
        if (k > 0 && ctd_word(out[k - 1])) { k = e - 1; continue; }
        const auto m = reads.find(out.substr(k, e - k));
        if (m == reads.end() || e >= out.size() || out[e] != '(') { k = e - 1; continue; }
        const std::size_t end = ctd_close(out, e);
        std::vector<std::string> args = ctd_args(std::string_view(out).substr(e + 1, end - e - 2));
        if (args.size() <= m->second) { k = e - 1; continue; }
        bool pure = true;
        for (std::size_t a = m->second; a < args.size(); ++a) {
            std::string_view x = args[a];
            for (std::size_t p = 0; p < x.size() && pure; ++p) {
                if (x[p] == '(' && p > 0 && ctd_word(x[p - 1])) {   // a call (but a far pointer made: no code)
                    std::size_t b = p;
                    while (b > 0 && ctd_word(x[b - 1])) --b;
                    const std::string_view callee = x.substr(b, p - b);
                    if (callee != "codebreaker_far" && !callee.starts_with("CONCAT") && !callee.starts_with("CODEBREAKER_PIECE")) pure = false;
                }
                if (x[p] == '=' && (p + 1 >= x.size() || x[p + 1] != '=') && (p == 0 || std::string_view("=!<>").find(x[p - 1]) == std::string_view::npos))
                    pure = false;   // a store
                if ((x[p] == '+' || x[p] == '-') && p + 1 < x.size() && x[p + 1] == x[p]) pure = false;
            }
        }
        if (!pure) { k = e - 1; continue; }
        args.resize(m->second);
        out.replace(e, end - e, "(" + text::join(args, ",") + ")");
        k = e;
    }
    return out;
}

// A number, with the comment the strings pass may have put beside it: `0x31a /* "MSC_CMD_FLAGS" */`, `0`.
bool ctd_literal(std::string_view s) {
    s = ctd_trim(s.substr(0, s.find("/*")));
    if (s.empty()) return false;
    for (const char c : s)
        if (!std::isxdigit(static_cast<unsigned char>(c)) && c != 'x' && c != 'X') return false;
    return std::isdigit(static_cast<unsigned char>(s[0])) != 0;
}

// `iVar4 = exit(...)`: a library function whose header says it returns nothing is called alone (the decompiler gave its
// result a name where the analysis saw AX read after it).
std::string ctd_library_void(const std::string& body, const std::set<std::string>& nothing) {
    if (nothing.empty()) return body;
    std::string out;
    for (const std::string_view line : text::lines(body)) {
        const std::size_t eq = line.find(" = ");
        if (eq != std::string_view::npos && line.ends_with(");")) {
            const std::string_view lhs = ctd_trim(line.substr(0, eq));
            const std::string_view rhs = line.substr(eq + 3);
            const std::size_t open = rhs.find('(');
            bool simple = !lhs.empty();
            for (const char c : lhs) simple = simple && ctd_word(c);
            if (simple && open != std::string_view::npos && nothing.contains(std::string(rhs.substr(0, open))) && ctd_close(rhs, open) == rhs.size() - 1) {
                out += std::string(line.substr(0, line.find_first_not_of(' '))) + std::string(rhs) + "\n";
                continue;
            }
        }
        out += line;
        out += '\n';
    }
    return out;
}

// Each call in `body` of a library function passes each pointer argument as the type its header declares (`(FILE *)pxVar3`):
// the decompiler typed the pointer by how this program uses it, the header by what the library does with it. A number stays
// as it is unless it was already a pointer cast (`(xunknown1 *)0x31a` becomes `(char *)0x31a`), and 0 is the null pointer.
std::string ctd_library_casts(const std::string& body, const std::map<std::string, std::vector<std::string>>& library) {
    if (library.empty()) return body;
    std::string out = body;
    for (std::size_t k = 0; k < out.size(); ++k) {
        if (const std::size_t past = ctd_skip(out, k); past != k) {
            k = past - 1;
            continue;
        }
        if (!std::isalpha(static_cast<unsigned char>(out[k])) && out[k] != '_') continue;
        std::size_t e = k;
        while (e < out.size() && ctd_word(out[e])) ++e;
        if (k > 0 && ctd_word(out[k - 1])) { k = e - 1; continue; }
        const auto m = library.find(out.substr(k, e - k));
        if (m == library.end() || e >= out.size() || out[e] != '(') { k = e - 1; continue; }
        const std::size_t end = ctd_close(out, e);
        std::vector<std::string> args = ctd_args(std::string_view(out).substr(e + 1, end - e - 2));
        if (args.size() != m->second.size()) { k = e - 1; continue; }   // (as many as the header says, or left alone)
        bool changed = false;
        for (std::size_t a = 0; a < args.size(); ++a) {
            const std::string& type = m->second[a];
            std::string& arg = args[a];
            if (type.empty() || arg.starts_with("(" + type + ")") || ctd_trim(arg) == "0") continue;
            if (ctd_literal(arg)) continue;
            const std::size_t close = arg.starts_with('(') ? ctd_close(arg, 0) : std::string::npos;
            if (close != std::string::npos && arg.substr(0, close).ends_with("*)") && ctd_literal(std::string_view(arg).substr(close)))
                arg = "(" + type + ")" + arg.substr(close);   // a pointer cast of a number: the header's
            else {
                bool word = true;
                for (const char c : arg) word = word && ctd_word(c);
                arg = "(" + type + ")" + (word ? arg : "(" + arg + ")");
            }
            changed = true;
        }
        if (changed) out.replace(e, end - e, "(" + text::join(args, ",") + ")");
        k = e;   // (the arguments' own calls next)
    }
    return out;
}

struct CtdFunction {
    std::string signature;   // its first line, as written
    std::string body;        // every line after it, to the next function
};

// ---- one function's text -------------------------------------------------------------------------------------------------

// The type `T *name;` declares `name` to point to, for each pointer declared in the function (its parameters too).
std::map<std::string, std::string> ctd_pointees(const CtdFunction& f) {
    std::map<std::string, std::string> out;
    auto add = [&](std::string_view decl) {
        decl = ctd_trim(decl);
        if (!decl.empty() && decl.back() == ';') decl.remove_suffix(1);
        const std::size_t star = decl.rfind('*');
        if (star == std::string_view::npos || decl.find('(') != std::string_view::npos) return;
        const std::string_view type = ctd_trim(decl.substr(0, star));
        const std::string_view name = ctd_trim(decl.substr(star + 1));
        if (type.empty() || name.empty() || type.find('*') != std::string_view::npos) return;
        for (const char c : name)
            if (!ctd_word(c)) return;
        out[std::string(name)] = std::string(type);
    };
    for (const std::string& p : ctd_params(f.signature)) add(p);
    for (const std::string_view line : text::lines(f.body))
        if (line.size() > 2 && line[0] == ' ' && line.back() == ';' && line.find('=') == std::string_view::npos) add(line);
    return out;
}

// MK_FP(segment, (T *)offset) as a far pointer to T made by codebreaker_far (DX:AX, no code); an offset with no cast points
// where the pointer it is made of points, else to a word.
std::string ctd_far(std::string_view s, const std::map<std::string, std::string>& pointees) {
    std::string out;
    std::size_t at = 0;
    for (std::size_t k = s.find("MK_FP("); k != std::string_view::npos; k = s.find("MK_FP(", at)) {
        if (!ctd_whole(s, k, 5)) {
            out += s.substr(at, k + 6 - at);
            at = k + 6;
            continue;
        }
        out += s.substr(at, k - at);
        const std::size_t end = ctd_close(s, k + 5);
        const std::string inner = ctd_far(s.substr(k + 6, end - k - 7), pointees);
        const std::vector<std::string> args = ctd_args(inner);
        if (args.size() != 2) {
            out += "MK_FP(" + inner + ")";
            at = end;
            continue;
        }
        const std::string& seg = args[0];
        std::string_view offset = args[1];
        std::string type = "xunknown2";
        if (offset.starts_with('(')) {
            const std::size_t close = offset.find(')');
            const std::string_view cast = close == std::string_view::npos ? std::string_view() : ctd_trim(offset.substr(1, close - 1));
            if (cast.ends_with('*') && cast.find('(') == std::string_view::npos && ctd_trim(cast.substr(0, cast.size() - 1)).find('*') == std::string_view::npos) {
                type = std::string(ctd_trim(cast.substr(0, cast.size() - 1)));
                offset = offset.substr(close + 1);
            }
        }
        if (type == "xunknown2" && !ctd_trim(offset).starts_with('&')) {   // (no cast: the pointer the offset is made of; &x->f a field's word)
            const std::vector<std::string_view> ids = ctd_identifiers(offset);
            if (!ids.empty()) {
                const auto p = pointees.find(std::string(ids.front()));
                if (p != pointees.end() && ctd_trim(offset).find(ids.front()) <= 1) type = p->second;
            }
        }
        out += "((" + type + " __far *)codebreaker_far(" + seg + ",(uint2)(" + std::string(ctd_trim(offset)) + ")))";
        at = end;
    }
    out += s.substr(at);
    return out;
}

// The function without the declarations of registers no statement uses (in_BX, unaff_SI, written for every register read
// before it is written, most of which the statements no longer read).
std::string ctd_drop_registers(const CtdFunction& f, std::size_t& dropped) {
    std::string out;
    const std::string whole = f.signature + "\n" + f.body;
    for (const std::string_view line : text::lines(f.body)) {
        const std::string_view t = ctd_trim(line);
        if (line.starts_with(' ') && t.ends_with(';') && t.find('=') == std::string_view::npos) {
            const std::string name = ctd_declared(t.substr(0, t.size() - 1));
            const bool reg = (name.starts_with("in_") || name.starts_with("unaff_")) && name.size() <= 9;
            if (reg && ctd_count(whole, name) == 1) {
                ++dropped;
                continue;
            }
        }
        out += line;
        out += '\n';
    }
    return out;
}

// A local the decompiler declared as four bytes (`xunknown4 xStack_6;`) that its statements dereference (`*xStack_6`) is a
// far pointer: declared as the pointer the statements most often store in it (`xStack_6 = (char *)CONCAT22(...)`).
std::string ctd_pointer_locals(const std::string& body, std::size_t& retyped) {
    std::map<std::string, std::size_t> wide;   // name -> its declaration's line start
    std::size_t at = 0;
    for (const std::string_view line : text::lines(body)) {
        const std::string_view t = ctd_trim(line);
        for (const std::string_view type : {"xunknown4 ", "int4 ", "uint4 "})
            if (line.starts_with("  ") && t.starts_with(type) && t.ends_with(';') && t.find_first_of("=*(,[") == std::string_view::npos)
                wide[std::string(ctd_trim(t.substr(type.size(), t.size() - type.size() - 1)))] = at;
        at += line.size() + 1;
    }
    std::string out = body;
    std::vector<std::pair<std::size_t, std::string>> edits;   // (declaration line start, its new line)
    for (const auto& [name, line_at] : wide) {
        bool deref = false;
        std::map<std::string, std::size_t> stored;
        for (std::size_t k = body.find(name); k != std::string::npos; k = body.find(name, k + name.size())) {
            if (!ctd_whole(body, k, name.size())) continue;
            std::size_t b = k;
            while (b > 0 && body[b - 1] == ' ') --b;
            if (b > 0 && body[b - 1] == '*' && (b < 2 || (body[b - 2] != '*' && body[b - 2] != ')' && !ctd_word(body[b - 2]))))
                deref = true;
            const std::size_t e = k + name.size();
            if (body.compare(e, 4, " = (") == 0) {
                const std::size_t close = body.find(')', e + 4);
                const std::string cast = close == std::string::npos ? "" : body.substr(e + 4, close - e - 4);
                if (cast.ends_with(" *") && cast.find_first_of("()") == std::string::npos && cast.find("__far") == std::string::npos)
                    ++stored[cast];
            }
        }
        if (!deref || stored.empty()) continue;
        const auto best = std::max_element(stored.begin(), stored.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
        edits.emplace_back(line_at, "  " + best->first + name + ";");
    }
    std::sort(edits.rbegin(), edits.rend());
    for (const auto& [line_at, line] : edits) {
        const std::size_t end = out.find('\n', line_at);
        out.replace(line_at, (end == std::string::npos ? out.size() : end) - line_at, line);
        ++retyped;
    }
    return out;
}

// The string the data segment holds at `offset`: printable text from the start of a string (the byte before it ends another,
// or it is the first) to its NUL, at least three characters; as a C literal.
std::optional<std::string> ctd_string_at(std::string_view data, std::uint32_t offset) {
    if (offset >= data.size() || (offset > 0 && data[offset - 1] != '\0')) return std::nullopt;
    std::string out = "\"";
    std::size_t k = offset;
    for (; k < data.size() && data[k] != '\0'; ++k) {
        const auto c = static_cast<unsigned char>(data[k]);
        if (c == '\n') out += "\\n";
        else if (c == '\t') out += "\\t";
        else if (c == '"' || c == '\\') out += std::string("\\") + static_cast<char>(c);
        else if (c == '/' && !out.empty() && out.back() == '*') out += "\\/";   // (no end of the comment it stands in)
        else if (c >= 0x20 && c < 0x7F) out += static_cast<char>(c);
        else return std::nullopt;
        if (out.size() > 70) {   // (a long one shortened)
            out += "...";
            while (k < data.size() && data[k] != '\0') ++k;
            break;
        }
    }
    if (k >= data.size() || k - offset < 3) return std::nullopt;
    return out + "\"";
}

// Each pointer constant of the text that points at a string of the data segment, with the string beside it: a near pointer
// `(xunknown1 *)0x31a`, and a far one passed as its two words, `0x22a,unaff_DS` / `0x22a,0x2a38` (the offset, then the segment).
std::string ctd_strings(std::string_view s, std::string_view data, std::string_view segment, std::size_t& named) {
    if (data.empty()) return std::string(s);
    std::string out;
    std::size_t at = 0;
    for (std::size_t k = s.find("0x"); k != std::string_view::npos; k = s.find("0x", k + 2)) {
        if (k > 0 && (ctd_word(s[k - 1]))) continue;
        std::size_t e = k + 2;
        const std::size_t digits = e;
        while (e < s.size() && ctd_hex(s[e])) ++e;
        if (e == digits || e - digits > 4 || (e < s.size() && ctd_word(s[e]))) continue;
        const bool cast = k >= 3 && s.substr(k - 3, 3) == " *)";
        const std::string_view rest = s.substr(e);
        const bool far = rest.starts_with(",unaff_DS") || (!segment.empty() && rest.starts_with(std::string(",") + std::string(segment)) &&
                                                           (rest.size() == segment.size() + 1 || !ctd_word(rest[segment.size() + 1])));
        if (!cast && !far) continue;
        const std::optional<std::string> text = ctd_string_at(data, static_cast<std::uint32_t>(std::stoul(std::string(s.substr(digits, e - digits)), nullptr, 16)));
        if (!text) continue;
        out += s.substr(at, e - at);
        out += " /* " + *text + " */";
        at = e;
        ++named;
    }
    out += s.substr(at);
    return out;
}

// A label the block ends with (`case 3:` and then `}`) given the empty statement C wants after it.
std::string ctd_label_statements(std::string_view body) {
    std::string out;
    const std::vector<std::string_view> lines = text::lines(body);
    for (std::size_t i = 0; i < lines.size(); ++i) {
        out += lines[i];
        out += '\n';
        const std::string_view t = ctd_trim(lines[i]);
        const bool label = t.ends_with(':') && (t.starts_with("case ") || t == "default:" || (t.find(' ') == std::string_view::npos && t.size() > 1));
        if (label && i + 1 < lines.size() && ctd_trim(lines[i + 1]).starts_with('}'))
            out += std::string(std::min(lines[i].find_first_not_of(' '), lines[i].size()) + 2, ' ') + ";\n";
    }
    return out;
}

// x._N_M_ (the M bytes of x from byte N) as CODEBREAKER_PIECEM(x, N).
std::string ctd_pieces(std::string_view s) {
    std::string out;
    std::size_t at = 0;
    for (std::size_t k = s.find("._"); k != std::string_view::npos; k = s.find("._", k + 2)) {
        std::size_t b = k;
        while (b > 0 && ctd_word(s[b - 1])) --b;
        std::size_t e = k + 2, n1 = e;
        while (e < s.size() && std::isdigit(static_cast<unsigned char>(s[e])) != 0) ++e;
        if (e == n1 || e >= s.size() || s[e] != '_') continue;
        const std::size_t n2 = ++e;
        while (e < s.size() && std::isdigit(static_cast<unsigned char>(s[e])) != 0) ++e;
        if (e == n2 || e >= s.size() || s[e] != '_' || (e + 1 < s.size() && ctd_word(s[e + 1])) || b == k) continue;
        out += s.substr(at, b - at);
        out += std::format("CODEBREAKER_PIECE{}({},{})", s.substr(n2, e - n2), s.substr(b, k - b), s.substr(n1, n2 - 1 - n1));
        at = e + 1;
    }
    out += s.substr(at);
    return out;
}

// *(T *)0x5cd4 -- a word of the data segment -- as g_5cd4, once for every place; the places and their types.
struct CtdGlobal {
    std::map<std::string, std::size_t> types;   // how often each type reads it
    std::vector<std::string> first_seen;         // the types in the order the program first reads it with each (a tie goes to the first)
};

std::string ctd_globals(std::string_view s, std::map<std::uint32_t, CtdGlobal>& globals) {
    std::string out;
    std::size_t at = 0;
    for (std::size_t k = s.find("*("); k != std::string_view::npos; k = s.find("*(", k + 2)) {
        const std::size_t close = s.find(')', k + 2);
        if (close == std::string_view::npos) break;
        const std::string_view cast = ctd_trim(s.substr(k + 2, close - k - 2));
        if (!cast.ends_with('*')) continue;
        const std::string_view type = ctd_trim(cast.substr(0, cast.size() - 1));
        if (type.empty() || type.find_first_of("*( ") != std::string_view::npos) continue;
        std::size_t e = close + 1;
        if (s.substr(e, 2) != "0x") continue;
        e += 2;
        const std::size_t digits = e;
        while (e < s.size() && ctd_hex(s[e])) ++e;
        if (e == digits || e - digits > 4 || (e < s.size() && (ctd_word(s[e]) || s[e] == '.'))) continue;
        if (k > 0 && (ctd_word(s[k - 1]) || s[k - 1] == ')')) continue;   // (a product, not a read)
        const std::uint32_t address = static_cast<std::uint32_t>(std::stoul(std::string(s.substr(digits, e - digits)), nullptr, 16));
        CtdGlobal& g = globals[address];
        if (g.types[std::string(type)]++ == 0) g.first_seen.emplace_back(type);
        out += s.substr(at, k - at);
        out += std::format("@G{:x}:{}@", address, type);   // (named below, once every use's type is known)
        at = e;
    }
    out += s.substr(at);
    return out;
}

}  // namespace

Result compilable(std::string_view text, std::string_view header, const Layout& layout) {
    Result result;
    Report& r = result.report;

    // the lines before the first function (the title, the #includes), and the functions
    std::string pre;
    std::vector<CtdFunction> fns;
    for (const std::string_view line : text::lines(text)) {
        if (ctd_signature(line)) {
            fns.push_back({std::string(ctd_trim(line)), {}});
        } else if (fns.empty()) {
            pre += line;
            pre += '\n';
        } else {
            fns.back().body += line;
            fns.back().body += '\n';
        }
    }
    r.functions = fns.size();

    // each function: its unused registers dropped, its far pointers and pieces written as C
    for (CtdFunction& f : fns) {
        const std::map<std::string, std::string> pointees = ctd_pointees(f);
        f.body = ctd_strings(ctd_label_statements(ctd_pieces(ctd_far(ctd_drop_registers(f, r.registers_dropped), pointees))), layout.data, layout.data_segment, r.strings);
        f.body = ctd_library_void(ctd_library_casts(ctd_pointer_locals(f.body, r.pointers), layout.library_pointers), layout.library_void);
        f.signature = ctd_pieces(f.signature);
    }

    // the structures the decompiler named: a word at each offset a field is read at
    std::map<std::string, std::set<std::uint32_t>> structures;
    std::map<std::pair<std::string, std::uint32_t>, std::string> field_names;
    for (const CtdFunction& f : fns) {
        const std::string whole = f.signature + "\n" + f.body;
        for (const std::string_view id : ctd_identifiers(whole))
            if (id.starts_with("s_")) structures[std::string(id)];
        // T *var; then var->f_8 / var->field_0x8; T var; then var.f_8
        std::map<std::string, std::string> typed;
        for (const auto& [var, type] : ctd_pointees(f))
            if (type.starts_with("s_")) typed[var] = type;
        for (const std::string_view line : text::lines(f.body)) {
            const std::string_view t = ctd_trim(line);
            if (t.starts_with("s_") && t.ends_with(';') && t.find_first_of("*=(") == std::string_view::npos) {
                const std::vector<std::string_view> ids = ctd_identifiers(t);
                if (ids.size() == 2) typed[std::string(ids[1])] = std::string(ids[0]);
            }
        }
        auto field_at = [&](std::string_view s, std::size_t k, const std::string& type) {
            const std::string_view rest = s.substr(k);
            for (const std::string_view prefix : {std::string_view("f_"), std::string_view("field_0x")}) {
                if (!rest.starts_with(prefix)) continue;
                std::size_t e = prefix.size();
                while (e < rest.size() && ctd_hex(rest[e])) ++e;
                if (e == prefix.size() || (e < rest.size() && ctd_word(rest[e]))) continue;
                const std::uint32_t off = static_cast<std::uint32_t>(std::stoul(std::string(rest.substr(prefix.size(), e - prefix.size())), nullptr, 16));
                structures[type].insert(off);
                field_names[{type, off}] = std::string(prefix);
            }
        };
        for (const auto& [var, type] : typed) {
            for (std::size_t k = whole.find(var); k != std::string::npos; k = whole.find(var, k + var.size())) {
                if (!ctd_whole(whole, k, var.size())) continue;
                const std::size_t after = k + var.size();
                if (whole.compare(after, 2, "->") == 0) field_at(whole, after + 2, type);
                else if (whole.compare(after, 1, ".") == 0) field_at(whole, after + 1, type);
            }
        }
        // ((s_X *)expr)->f_8, ((s_X __far *)codebreaker_far(...))->f_8: a cast to the structure, the whole cast in parentheses
        for (std::size_t k = whole.find("((s_"); k != std::string::npos; k = whole.find("((s_", k + 4)) {
            const std::size_t star = whole.find(" *)", k);
            if (star == std::string::npos) continue;
            std::string type = whole.substr(k + 2, star - k - 2);
            if (type.ends_with(" __far")) type.resize(type.size() - 6);
            if (type.find_first_of(" ()") != std::string::npos) continue;
            const std::size_t end = ctd_close(whole, k);
            if (whole.compare(end, 2, "->") == 0) field_at(whole, end + 2, type);
        }
    }
    std::string declarations;
    for (const auto& [name, offsets] : structures) {
        std::string fields;
        std::uint32_t at = 0;
        for (const std::uint32_t off : offsets) {
            if (off < at) continue;
            if (off > at) fields += std::format("uint1 pad_{:x}[{}]; ", at, off - at);
            const auto named = field_names.find({name, off});
            fields += std::format("xunknown2 {}{:x}; ", named == field_names.end() ? "f_" : named->second, off);
            at = off + 2;
        }
        if (fields.empty()) fields = "xunknown2 f_0; ";
        declarations += "typedef struct " + name + " { " + fields + "} " + name + ";\n";
    }
    r.structures = structures.size();

    // a function written void whose result a caller uses returns what it leaves in AX
    std::set<std::string> used;
    for (const CtdFunction& f : fns)
        for (std::size_t k = f.body.find("= "); k != std::string::npos; k = f.body.find("= ", k + 2)) {
            std::size_t b = k + 2;   // (`x = sub_...(`, or through a cast: `x = (xunknown2 *)sub_...(`)
            if (b < f.body.size() && f.body[b] == '(') {
                const std::size_t close = ctd_close(f.body, b);
                if (close <= b + 2 || f.body[close - 2] != '*') continue;
                b = close;
            }
            if (f.body.compare(b, 4, "sub_") != 0) continue;
            std::size_t e = b;
            while (e < f.body.size() && ctd_word(f.body[e])) ++e;
            if (e < f.body.size() && f.body[e] == '(') used.insert(f.body.substr(b, e - b));
        }
    for (CtdFunction& f : fns)
        if (f.signature.starts_with("void ") && used.contains(ctd_name(f.signature))) f.signature = "xunknown2 " + f.signature.substr(5);

    // a function whose parameters the clean-C pass made far pointers of (two words each: `void *a1` for the words a1 and a2)
    // is still called with the words, by callers decompiled on their own: such a call passes each pair as the one value
    // it is, `(void *)CONCAT22(high, low)` (cdecl pushes the low word last: it is the first of the two)
    std::map<std::string, std::vector<std::string>> merged;   // function -> each parameter's type, where a parameter is two words
    for (const CtdFunction& f : fns) {
        std::vector<std::string> types;
        bool wide = false;
        for (const std::string& p : ctd_params(f.signature)) {
            const std::string id = ctd_declared(p);
            std::string type(ctd_trim(std::string_view(p).substr(0, p.size() - id.size())));
            wide = wide || ctd_words(type) == 2;
            types.push_back(std::move(type));
        }
        if (wide) merged[ctd_name(f.signature)] = std::move(types);
    }
    for (CtdFunction& f : fns) f.body = ctd_join_words(f.body, merged);

    // a call passing more than its function reads (the decompiler guessed the words of a call it was told nothing of): the
    // callee never reads the rest, and the caller removes what it pushed, so an argument past them that does nothing (no call,
    // no store) is not written
    std::map<std::string, std::size_t> parameters;   // function -> its parameters, where none is `...`
    for (const CtdFunction& f : fns) {
        const std::vector<std::string> params = ctd_params(f.signature);
        if (std::none_of(params.begin(), params.end(), [](const std::string& p) { return ctd_trim(p) == "..."; }))
            parameters[ctd_name(f.signature)] = params.size();
    }
    for (CtdFunction& f : fns) f.body = ctd_drop_surplus(f.body, parameters);

    // the arguments each function is called with
    std::map<std::string, std::set<std::size_t>> calls;
    std::set<std::string> defined;
    for (const CtdFunction& f : fns) defined.insert(ctd_name(f.signature));
    for (const CtdFunction& f : fns) {
        for (const std::string_view id : ctd_identifiers(f.body)) {
            if (!defined.contains(std::string(id))) continue;
            const std::size_t k = static_cast<std::size_t>(id.data() - f.body.data());
            if (k + id.size() >= f.body.size() || f.body[k + id.size()] != '(') continue;
            const std::size_t end = ctd_close(f.body, k + id.size());
            calls[std::string(id)].insert(ctd_args(std::string_view(f.body).substr(k + id.size() + 1, end - k - id.size() - 2)).size());
        }
    }
    std::vector<std::string> prototypes;
    for (CtdFunction& f : fns) {
        const std::string name = ctd_name(f.signature);
        std::vector<std::string> params = ctd_params(f.signature);
        // a function whose callers pass different numbers of words is written `(int2 a1, ...)`, and where its body reads a word
        // past those by name, `(int2 a1, ..., xunknown2 param_3)`: that `...` is the word between, and one last is dropped --
        // such a function is defined the old way, which lets each caller pass what it passes
        bool variadic = false;
        std::size_t fixed = 0;   // the words every caller passes (each a word: the lane states them so)
        for (std::size_t k = 0; k < params.size(); ++k) {
            if (ctd_trim(params[k]) != "...") continue;
            if (!variadic) fixed = k;
            variadic = true;
            if (k + 1 == params.size()) params.pop_back();
            else params[k] = std::format("xunknown2 param_{}", k + 1);
        }
        const auto seen = calls.find(name);
        const std::string head = f.signature.substr(0, f.signature.find('(') + 1);
        if (variadic && fixed > 0) {
            // `T f(int2 a1, ...)`: a prototype every caller agrees with; a word the body reads past the fixed ones is the word
            // at its place beside the first argument (cdecl pushes the last first: they lie in order up the stack)
            const std::vector<std::string> named(params.begin(), params.begin() + static_cast<std::ptrdiff_t>(fixed));
            f.signature = head + text::join(named, ",") + ",...)";
            prototypes.push_back(f.signature + ";");
            const std::string first = ctd_declared(params[0]);
            std::string reads;
            for (std::size_t k = fixed; k < params.size(); ++k) {
                const std::string p(ctd_trim(params[k]));
                const std::string id = ctd_declared(p);
                if (ctd_count(f.body, id) == 0) continue;   // (a word between that the body never reads)
                const std::string type(ctd_trim(std::string_view(p).substr(0, p.size() - id.size())));
                reads += std::format("  {} = *({} *)((xunknown2 *)&{} + {});\n", p, type, first, k);
            }
            const std::size_t open = f.body.find("{\n");
            if (!reads.empty() && open != std::string::npos) f.body.insert(open + 2, reads);
            ++r.old_style;
            continue;
        }
        if (!variadic && (seen == calls.end() || (seen->second.size() == 1 && *seen->second.begin() <= params.size()))) {
            prototypes.push_back(f.signature + ";");
            continue;
        }
        const std::size_t most = std::max(seen == calls.end() ? 0 : *seen->second.rbegin(), params.size());
        for (std::size_t k = params.size(); k < most; ++k) params.push_back(std::format("xunknown2 param_{}", k + 1));
        if (!variadic && seen->second.size() == 1) {   // every caller passes the same number: the function takes them all
            f.signature = head + text::join(params, ",") + ")";
            prototypes.push_back(f.signature + ";");
            ++r.widened;
            continue;
        }
        // callers disagree: defined the old way (C as it was written in 1986), declared without a prototype
        std::vector<std::string> names;
        for (const std::string& p : params) names.push_back(ctd_declared(p));
        std::string declared;
        for (const std::string& p : params) declared += "  " + p + ";\n";
        f.signature = head + text::join(names, ",") + ")\n" + declared;
        if (f.signature.ends_with('\n')) f.signature.pop_back();
        prototypes.push_back(head + ");");
        ++r.old_style;
    }

    // the data segment's words, named; memory outside the program; the helpers nothing declares
    std::map<std::uint32_t, CtdGlobal> globals;
    for (CtdFunction& f : fns) f.body = ctd_globals(f.body, globals);

    // the source files: each code segment votes, with every data-segment word its functions read, for the file whose data
    // that is (between its version string and the next; the last file's data runs on into the shared data, so it gets no votes)
    std::map<std::string, std::string> file_of;   // function name -> its file
    if (layout.files.size() >= 2) {
        std::map<std::uint32_t, std::map<std::size_t, std::size_t>> votes;   // segment -> file -> words
        for (const CtdFunction& f : fns) {
            const auto seg = layout.segment_of.find(ctd_name(f.signature));
            if (seg == layout.segment_of.end()) continue;
            for (std::size_t k = f.body.find("@G"); k != std::string::npos; k = f.body.find("@G", k + 2)) {
                const std::uint32_t address = static_cast<std::uint32_t>(std::stoul(f.body.substr(k + 2, f.body.find(':', k) - k - 2), nullptr, 16));
                const auto after = std::upper_bound(layout.files.begin(), layout.files.end(), address,
                                                    [](std::uint32_t a, const auto& file) { return a < file.first; });
                if (after == layout.files.begin() || after == layout.files.end()) continue;
                ++votes[seg->second][static_cast<std::size_t>(after - layout.files.begin()) - 1];
            }
        }
        // a segment is a file's where that file has most of its words, twice the next
        std::vector<std::pair<std::uint32_t, std::size_t>> placed;   // (segment, file), segments ascending
        for (const auto& [segment, by_file] : votes) {
            std::size_t best = 0, first = 0, second = 0;
            for (const auto& [file, n] : by_file) {
                if (n > first) second = first, first = n, best = file;
                else if (n > second) second = n;
            }
            if (first >= 2 && first >= 2 * second) placed.emplace_back(segment, best);
        }
        // the files were linked in the order their data is: keep the longest run of placements in that order
        std::vector<std::size_t> length(placed.size(), 1), previous(placed.size(), SIZE_MAX);
        std::size_t end = SIZE_MAX;
        for (std::size_t i = 0; i < placed.size(); ++i) {
            for (std::size_t j = 0; j < i; ++j)
                if (placed[j].second <= placed[i].second && length[j] + 1 > length[i]) length[i] = length[j] + 1, previous[i] = j;
            if (end == SIZE_MAX || length[i] > length[end]) end = i;
        }
        std::map<std::uint32_t, std::size_t> file_of_segment;
        for (std::size_t i = end; i != SIZE_MAX; i = previous[i]) file_of_segment[placed[i].first] = placed[i].second;
        // (a segment no data places stays unplaced: the files that have no code -- the message tables, the keyword table -- leave
        // no count of files between two placed segments to go by)
        for (const auto& [name, segment] : layout.segment_of) {
            const auto file = file_of_segment.find(segment);
            if (file != file_of_segment.end()) file_of[name] = layout.files[file->second].second;
        }
    }
    std::string defines;
    std::map<std::uint32_t, std::string> global_type;
    for (const auto& [address, g] : globals) {
        std::string best;
        std::size_t most = 0;
        for (const std::string& type : g.first_seen)
            if (g.types.at(type) > most) best = type, most = g.types.at(type);
        global_type[address] = best;
        defines += std::format("#define g_{:x} (*({} __near *)0x{:x})\n", address, best, address);
    }
    r.globals = globals.size();
    for (CtdFunction& f : fns) {
        std::string out;
        std::size_t at = 0;
        for (std::size_t k = f.body.find("@G"); k != std::string::npos; k = f.body.find("@G", at)) {
            const std::size_t colon = f.body.find(':', k);
            const std::size_t end = f.body.find('@', colon);
            const std::uint32_t address = static_cast<std::uint32_t>(std::stoul(f.body.substr(k + 2, colon - k - 2), nullptr, 16));
            const std::string type = f.body.substr(colon + 1, end - colon - 1);
            out += f.body.substr(at, k - at);
            out += type == global_type[address] ? std::format("g_{:x}", address) : std::format("(*({} __near *)&g_{:x})", type, address);
            at = end + 1;
        }
        out += f.body.substr(at);
        f.body = std::move(out);
    }
    std::set<std::string> ram, helpers;
    for (const CtdFunction& f : fns) {
        const std::vector<std::string_view> ids = ctd_identifiers(f.body);
        for (std::size_t i = 0; i < ids.size(); ++i) {
            const std::string_view id = ids[i];
            const std::size_t ram_at = id.find("Ram");
            if (ram_at != std::string_view::npos && ram_at > 0 && id.size() == ram_at + 11 &&
                std::all_of(id.begin() + static_cast<std::ptrdiff_t>(ram_at + 3), id.end(), ctd_hex))
                ram.insert(std::string(id));
            const std::size_t k = static_cast<std::size_t>(id.data() - f.body.data());
            const bool called = k + id.size() < f.body.size() && f.body[k + id.size()] == '(';
            const bool interrupt = id.size() >= 5 && id.starts_with("int") && ctd_hex(id[3]) && ctd_hex(id[4]) && !defined.contains(std::string(id));
            if (called && !defined.contains(std::string(id)) && (interrupt || id.starts_with("func_0x") || id.starts_with("FUN_")))
                helpers.insert(std::string(id));
        }
    }
    for (const std::string& name : ram) {
        const std::size_t ram_at = name.find("Ram");
        const std::uint32_t linear = static_cast<std::uint32_t>(std::stoul(name.substr(ram_at + 3), nullptr, 16));
        const char kind = name.front();
        const std::string type = kind == 'b' ? "uint1" : kind == 'c' ? "char" : kind == 'i' ? "int2" : kind == 'u' ? "uint2" : "xunknown2";
        defines += std::format("#define {} (*({} __far *)codebreaker_far(0x{:x}, 0x{:x}))   /* {:05X}h */\n", name, type, linear >> 4, linear & 15, linear);
    }

    // the whole: the title, the header, the program's declarations, then the functions
    std::string out;
    const std::size_t title_end = pre.find('\n');
    out += pre.substr(0, title_end == std::string::npos ? pre.size() : title_end + 1);
    out += "\n";
    out += header;
    out += "/* a far pointer made of a segment and an offset: DX:AX, no code */\n"
           "void __far *codebreaker_far(uint2 seg, uint2 off);\n"
           "#pragma aux codebreaker_far = parm [dx] [ax] value [dx ax] modify exact [];\n"
           "/* the bytes of a variable from byte n, read or written in place */\n"
           "#define CODEBREAKER_PIECE1(x, n) (*(uint1 *)((uint1 *)&(x) + (n)))\n"
           "#define CODEBREAKER_PIECE2(x, n) (*(uint2 *)((uint1 *)&(x) + (n)))\n"
           "#define CODEBREAKER_PIECE4(x, n) (*(uint4 *)((uint1 *)&(x) + (n)))\n"
           "/* the stack pointer, where the decompiler names it as the register it is (a program's start-up code sets it) */\n"
           "uint2 codebreaker_sp(void);\n"
           "#pragma aux codebreaker_sp = \"mov ax,sp\" value [ax] modify exact [ax];\n"
           "#define register0x00000010 codebreaker_sp()\n";
    if (title_end != std::string::npos) out += pre.substr(title_end + 1);
    if (!declarations.empty()) out += "\n/* structures the functions read through (a word at each offset a field is read at) */\n" + declarations;
    if (!defines.empty()) out += "\n/* the data segment's words, and memory outside the program */\n" + defines;
    if (!helpers.empty()) {
        out += "\n";
        for (const std::string& h : helpers) out += "int2 " + h + "();\n";
    }
    out += "\n";
    for (const std::string& p : prototypes) out += p + "\n";
    std::optional<std::string> current;   // the source file the functions written last are from
    for (const CtdFunction& f : fns) {
        if (!file_of.empty()) {
            const auto file = file_of.find(ctd_name(f.signature));
            const std::string here = file == file_of.end() ? std::string() : file->second;
            if (!current || *current != here) {
                out += here.empty() ? "\n/* ---- (not placed in a source file) ---- */\n" : "\n/* ---- " + here + " ---- */\n";
                current = here;
            }
            if (!here.empty()) ++result.placed;
        }
        // _a1: the decompiler's name for a1 and the word after it read as one far pointer -- the two arguments sit side by side
        // on the stack, so the name is that place read as one
        // (a local the same way: _piStack_a, piStack_a and the word after it in the frame)
        std::set<std::string> joined, declared;
        for (const std::string& p : ctd_params(f.signature)) declared.insert(ctd_declared(p));
        for (const std::string_view line : text::lines(f.body)) {
            const std::string_view t = ctd_trim(line);
            if (line.starts_with(' ') && t.ends_with(';') && t.find_first_of("=(") == std::string_view::npos && !t.starts_with("return ") &&
                !t.starts_with("goto "))
                declared.insert(ctd_declared(t.substr(0, t.size() - 1)));
        }
        for (const std::string_view id : ctd_identifiers(f.body))
            if (id.size() >= 2 && id.starts_with('_') && declared.contains(std::string(id.substr(1))) && !declared.contains(std::string(id)))
                joined.insert(std::string(id));
        for (const std::string& j : joined) out += "\n#define " + j + " (*(xunknown2 **)&" + j.substr(1) + ")";
        out += "\n" + f.signature + "\n" + f.body;
        for (const std::string& j : joined) out += "#undef " + j + "\n";
    }
    while (!out.empty() && (out.back() == '\n' || out.back() == ' ')) out.pop_back();
    out += '\n';
    result.text = std::move(out);
    return result;
}

}  // namespace cb::decompile::c_text_dos
