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
std::size_t ctd_close(std::string_view s, std::size_t open) {
    int depth = 0;
    for (std::size_t k = open; k < s.size(); ++k) {
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
    for (const char c : s) {
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
        for (std::size_t k = f.body.find("= sub_"); k != std::string::npos; k = f.body.find("= sub_", k + 2)) {
            std::size_t e = k + 2;
            while (e < f.body.size() && ctd_word(f.body[e])) ++e;
            if (e < f.body.size() && f.body[e] == '(') used.insert(f.body.substr(k + 2, e - k - 2));
        }
    for (CtdFunction& f : fns)
        if (f.signature.starts_with("void ") && used.contains(ctd_name(f.signature))) f.signature = "xunknown2 " + f.signature.substr(5);

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
        const auto seen = calls.find(name);
        const std::string head = f.signature.substr(0, f.signature.find('(') + 1);
        if (seen == calls.end() || (seen->second.size() == 1 && *seen->second.begin() <= params.size())) {
            prototypes.push_back(f.signature + ";");
            continue;
        }
        const std::size_t most = std::max(*seen->second.rbegin(), params.size());
        for (std::size_t k = params.size(); k < most; ++k) params.push_back(std::format("xunknown2 param_{}", k + 1));
        if (seen->second.size() == 1) {   // every caller passes the same number: the function takes them all
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
            if (line.starts_with(' ') && t.ends_with(';') && t.find_first_of("=(") == std::string_view::npos) declared.insert(ctd_declared(t.substr(0, t.size() - 1)));
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
