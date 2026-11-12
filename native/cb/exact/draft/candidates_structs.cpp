// The structs the pointers' fields make: `p->fN` (N the offset the model read) declared as a field of a struct p
// points to. Part of cb/exact/draft/candidates.hpp.
#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/draft/candidates_internal.hpp"

namespace cb::exact::draft::candidates {

using namespace inner;

namespace {

using Offsets = std::set<std::int64_t>;
using Uses = OrderedMap<std::string, Offsets>;   // pointer -> the offsets read through it, the pointers in the order met

std::vector<std::string> structs_keys(const Uses& uses) {
    std::vector<std::string> out;
    for (const auto& kv : uses) out.push_back(kv.first);
    return out;
}

// A pattern made of a literal name between two pieces of pattern, compiled for the name (one per pointer the pass
// found, not one per use).
Regex structs_name(sv before, sv name, sv after, RegexFlags flags = RegexFlags::None) {
    return Regex(str(before) + re_escape(name) + str(after), flags);
}

// `\bP->f(\d+)\b` replaced as the byte readings are: `*P` for f0, `P[N]` else.
std::string structs_byte_reads(sv text, sv p) {
    const Regex field = structs_name(R"re(\b)re", p, R"re(->f(\d+)\b)re");
    return sub_all(field, text, [&](const RegexMatch& m) { return m[1] == "0" ? "*" + str(p) : str(p) + "[" + str(m[1]) + "]"; });
}

// The text with the declaration of x as an int turned into a pointer to `type` (declare_pointer), or none.
std::optional<std::string> structs_declare(sv text, sv x, sv type) { return dt::declare_pointer(text, x, type); }

bool structs_present(const std::optional<std::string>& s) { return s.has_value() && !s->empty(); }

}  // namespace

std::string structs(std::string_view text_in, const std::set<text::Site>& widths, State& state) {
    std::string text = str(text_in);
    const auto w = [&](sv pointer, std::int64_t offset) { return has_site(widths, pointer, offset); };
    const std::set<std::int64_t>& long_fields = state.facts.longs.fields;
    const auto is_long_field = [&](std::int64_t x) { return long_fields.contains(x); };

    // (lea ax,[p+N] - the model's &p->fN - is p + N)
    static const Regex amp_field(R"re(&(\w+)->f(\d+)\b)re");
    text = amp_field.replace_all(text, "($1 + $2)");
    static const Regex amp_index(R"re(&(\w+)\[(-?)(\d+)\])re");
    text = sub_all(amp_index, text, [](const RegexMatch& m) { return "(" + str(m[1]) + (m[2].empty() ? " + " : " - ") + str(m[3]) + ")"; });

    // (a routine's value gone through to a field: a pointer to a struct it returns - cast)
    static const Regex call_field(R"re(\b((?!if\b|while\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\([^()]*\))*)\)->f(\d+)\b)re");
    std::map<std::string, Offsets> rets;
    call_field.for_each_match(text, [&](const RegexMatch& m) {
        rets[str(m[1])].insert(to_int(m[3]));
        return true;
    });
    std::vector<std::string> pre;
    for (const auto& [f, offs] : rets) {
        std::vector<std::string> fl;
        std::int64_t pos = 0;
        for (const std::int64_t o : offs) {
            if (o > pos) fl.push_back(std::format("char _{}[{}];", pos, o - pos));
            const bool byte = w("bx*", o) && !w("bxw", o);
            fl.push_back(std::format("{} f{};", byte ? "char" : "int", o));
            pos = o + (byte ? 1 : 2);
        }
        pre.push_back(std::format("struct s_r_{} {{ {} }};", f, ct::join(fl, " ")));
    }
    if (!rets.empty()) {
        text = sub_all(call_field, text, [](const RegexMatch& m) {
            return "((struct s_r_" + str(m[1]) + " *)" + str(m[1]) + "(" + str(m[2]) + "))->f" + str(m[3]);
        });
        text = ct::join(pre, "\n") + "\n" + text;
    }
    if (w("bx*", 0) && !w("bxw", 0)) {   // (*p->fA->fB read a byte: a char there)
        static const Regex deref_chain(R"re((?<![\w>)\]])\*(\w+(?:->f\d+){2,})\b(?!\s*(?:->|\[|\.|\+\+|--)))re");
        text = deref_chain.replace_all(text, "*(char *)$1");
    }

    Uses uses;
    static const Regex field_use(R"re(\b(\w+)->f(\d+)\b)re");
    field_use.for_each_match(text, [&](const RegexMatch& m) {
        if (!m[1].starts_with("s_r_")) uses.try_emplace(str(m[1])).first->second.insert(to_int(m[2]));
        return true;
    });
    // (a pointer read through twice: a struct whose first field points on - a list)
    static const Regex twice(R"re(\*\(\*(\w+)\)|\*\*(\w+))re");
    std::vector<std::string> twice_pointers;
    twice.for_each_match(text, [&](const RegexMatch& m) {
        twice_pointers.push_back(str(!m[1].empty() ? m[1] : m[2]));
        return true;
    });
    for (const std::string& p : twice_pointers) {
        const std::string e = re_escape(p);
        const Regex declared(R"re((?m)^[ \t]*(?:register )?int )re" + e + R"re(;|^[ \t]*int (?:\w+, )*)re" + e + R"re(\b)re");
        if (declared.search(text)) {
            const Regex through(R"re(\*\(\*)re" + e + R"re(\)|\*\*)re" + e + R"re(\b)re");
            text = through.replace_all(text, "(" + p + "->f0)->f0");
            uses.try_emplace(p).first->second.insert(0);
        }
    }
    if (uses.empty()) return text;

    // (p + N, the place of a field - lea - is &p->fN)
    for (const std::string& p : structs_keys(uses)) {
        const Regex plus = structs_name(R"re(\()re", p, R"re( \+ (\d+)\))re");
        std::set<std::string> numbers;
        plus.for_each_match(text, [&](const RegexMatch& m) {
            numbers.insert(str(m[1]));
            return true;
        });
        for (const std::string& n : numbers) {
            if (to_int(n) < 0x100) {
                uses.at(p).insert(to_int(n));
                text = ct::replace_all(text, "(" + p + " + " + n + ")", "&" + p + "->f" + n);
            }
        }
    }
    // (*p of a struct pointer: its first field)
    static const Regex declaration_start(R"re(\s*(?:extern\s+|register\s+|static\s+)*(?:unsigned\s+)?(?:char|int|long|short|struct\s+\w+)\b)re");
    for (const std::string& p : structs_keys(uses)) {
        const Regex whole = structs_name(R"re((?<![\w>)\]])\*)re", p, R"re(\b(?!\s*(?:->|\[|\+\+|--|\.)))re");
        Lines lines = split_nl(text);
        bool found = false;
        for (const std::string& l : lines)
            if (!declaration_start.match(l) && whole.search(l)) found = true;
        if (!found) continue;
        for (std::string& l : lines)
            if (!declaration_start.match(l)) l = whole.replace_all(l, p + "->f0");
        text = join_nl(lines);
        uses.at(p).insert(0);
    }

    // (what a register variable's bytes are: its register's)
    const auto byte_at = [&](const std::string& p, std::int64_t o) {
        return w(p, o) || w("*", o) || (p.starts_with("r_") && w(p.substr(2) + "*", o) && !w(p.substr(2) + "w", o));
    };

    // (a pointer stepped and read as *p++, its places all bytes: a char *, the places p[N])
    for (const auto& [p, offs] : std::vector<std::pair<std::string, Offsets>>(uses.entries().begin(), uses.entries().end())) {
        const std::string e = re_escape(p);
        const Regex stepped(R"re(\*(?:)re" + e + R"re((?:\+\+|--)|(?:\+\+|--))re" + e + R"re(\b))re");
        if (!(stepped.search(text) && !offs.empty() && *offs.rbegin() < 16 && std::ranges::all_of(offs, [&](std::int64_t o) { return byte_at(p, o); })))
            continue;
        const std::string t2 = structs_byte_reads(text, p);
        const Regex char_decl = structs_name(R"re((?m)^[ \t]*(?:register )?char \*)re", p, ";");
        const std::optional<std::string> t3 = char_decl.search(t2) ? std::optional<std::string>(t2) : structs_declare(t2, p, "char");
        if (structs_present(t3)) {
            text = *t3;
            uses.erase(p);
        }
    }
    // (a byte read inside a word read: no struct - the register variable's words through an int *, the bytes through a
    // char * of it)
    for (const auto& [p, offs] : std::vector<std::pair<std::string, Offsets>>(uses.entries().begin(), uses.entries().end())) {
        Offsets words;
        for (const std::int64_t o : offs)
            if (!byte_at(p, o)) words.insert(o);
        const bool odd_byte_in_word = std::ranges::any_of(offs, [&](std::int64_t o) { return byte_at(p, o) && (o % 2) != 0 && words.contains(o - 1); });
        if (odd_byte_in_word && std::ranges::all_of(words, [](std::int64_t o) { return o % 2 == 0; })) {
            const Regex field = structs_name(R"re(\b)re", p, R"re(->f(\d+)\b)re");
            std::string t2 = sub_all(field, text, [&](const RegexMatch& m) {
                const std::int64_t n = to_int(m[1]);
                if (byte_at(p, n) && n % 2) return "((char *)" + p + ")[" + str(m[1]) + "]";
                return m[1] == "0" ? "*" + p : p + "[" + std::to_string(n / 2) + "]";
            });
            const Regex char_ptr = structs_name(R"re((?m)^([ \t]*)(register )?char \*)re", p, ";$");
            const std::string t3 = char_ptr.replace_all(t2, "$1$2int *" + p + ";");
            const std::optional<std::string> t4 = t3 != t2 ? std::optional<std::string>(t3) : structs_declare(t2, p, "int");
            if (structs_present(t4)) {
                text = *t4;
                uses.erase(p);
                continue;
            }
        }
        // (stepped by one, bytes alone read through it: a char *, its bytes p[N])
        std::set<std::string> steps;
        const Regex step_by = structs_name(R"re(\b)re", p, " = " + re_escape(p) + R"re( [+-] (\d+);)re");
        step_by.for_each_match(text, [&](const RegexMatch& m) {
            steps.insert(str(m[1]));
            return true;
        });
        const Regex step_one(R"re((?<![\w>.]))re" + re_escape(p) + R"re((?:\+\+|--)|(?:\+\+|--))re" + re_escape(p) + R"re(\b)re");
        if (step_one.search(text)) steps.insert("1");
        const Regex through_field = structs_name(R"re(\b)re", p, R"re(->f\d+->)re");
        if (steps == std::set<std::string>{"1"} && std::ranges::all_of(offs, [&](std::int64_t o) { return byte_at(p, o); }) && *offs.rbegin() < 0x10 &&
            !through_field.search(text)) {
            std::string t2 = structs_byte_reads(text, p);
            const Regex char_decl = structs_name(R"re((?m)^[ \t]*(?:register\s+)?char \*)re", p, ";$");
            std::optional<std::string> t3 = t2;
            if (!char_decl.search(t2)) t3 = structs_declare(t2, p, "char");
            if (structs_present(t3)) {
                text = *t3;
                uses.erase(p);
            }
        }
    }

    // (a field through which the code goes on - p->fN->fM, (p->fN)->fM, *(p->fN): a pointer to a struct of its own;
    // the same struct when what it reaches are fields p has too - a list, a tree)
    std::map<std::pair<std::string, std::int64_t>, Offsets> inner_fields;
    static const Regex star_paren_field(R"re(\*\((\w+)->f(\d+)\)(?!\s*(?:->|\[)))re");
    text = star_paren_field.replace_all(text, "($1->f$2)->f0");
    static const Regex two_fields(R"re(\b(\w+)->f(\d+)\)?->f(\d+)\b)re");
    two_fields.for_each_match(text, [&](const RegexMatch& m) {
        if (uses.contains(str(m[1]))) inner_fields[{str(m[1]), to_int(m[2])}].insert(to_int(m[3]));
        return true;
    });
    static const Regex paren_field(R"re(\((\w+->f\d+)\)->)re");
    text = paren_field.replace_all(text, "$1->");
    // (a whole deref of a field chain that goes on as a pointer elsewhere - *p->fN, *p->fN->fM: its first field)
    for (int round = 0; round < 3; ++round) {
        static const Regex deref_chain(R"re((?<![\w>)\]])\*(\w+)((?:->f\d+)+)\b(?!\s*(?:->|\[|\.)))re");
        std::set<std::pair<std::string, std::string>> chains;
        deref_chain.for_each_match(text, [&](const RegexMatch& m) {
            chains.insert({str(m[1]), str(m[2])});
            return true;
        });
        for (const auto& [p, chain] : chains) {
            const std::size_t arrows = static_cast<std::size_t>(std::ranges::count(chain, '>'));   // ("->" is the only place a > stands)
            const std::int64_t n = to_int(chain.substr(chain.rfind("->f") + 3));
            const Regex goes_on(R"re(\b)re" + re_escape(p) + re_escape(chain) + R"re(->f\d+)re");
            if (uses.contains(p) && (goes_on.search(text) || arrows > 1)) {
                const Regex whole(R"re((?<![\w>)\]])\*)re" + re_escape(p) + re_escape(chain) + R"re(\b(?!\s*(?:->|\[|\.)))re");
                text = whole.replace_all(text, p + chain + "->f0");
                if (arrows == 1) inner_fields[{p, n}].insert(0);
            }
        }
    }
    std::map<std::pair<std::string, std::int64_t>, std::string> ptrfield;
    std::vector<std::string> defs;
    std::map<std::string, Offsets> deeper;   // (what chains reach further down: p->fA->fB->fC..)
    static const Regex long_chain(R"re(\b(\w+)((?:->f\d+){3,}))re");
    static const Regex chain_offset(R"re(->f(\d+))re");
    long_chain.for_each_match(text, [&](const RegexMatch& m) {
        if (!uses.contains(str(m[1]))) return true;
        std::size_t k = 0;
        chain_offset.for_each_match(m[2], [&](const RegexMatch& o) {
            if (k++ >= 2) deeper[str(m[1])].insert(to_int(o[1]));
            return true;
        });
        return true;
    });
    for (const auto& [key, subs] : inner_fields) {
        const auto& [p, n] = key;
        Offsets bytes_in_subs;   // (a byte the code reads where p has a word: another struct)
        for (const std::int64_t o : subs)
            if (w("*", o)) bytes_in_subs.insert(o);
        // (what bx - the register the model goes on through - reads at an offset, beside what the others read there:
        // a byte through one and a word through the other is two structs)
        const auto width = [&](std::initializer_list<const char*> regs, std::int64_t o) {
            std::set<std::string> out;
            for (const char* r : regs) {
                const std::string kind = std::string(w(std::string(r) + "*", o) ? "b" : "") + (w(std::string(r) + "w", o) ? "w" : "");
                if (!kind.empty()) out.insert(kind);
            }
            return out;
        };
        Offsets both;
        for (const std::int64_t o : subs) {
            const auto bx = width({"bx"}, o), others = width({"si", "di"}, o);
            if (!bx.empty() && !others.empty() && bx != others) both.insert(o);
        }
        Offsets bytes_in;
        for (const std::int64_t o : subs) {
            const auto bx = width({"bx"}, o);
            const std::string joined = ct::join(bx, "");
            if (joined.find('b') != npos || (bx.empty() && bytes_in_subs.contains(o))) bytes_in.insert(o);
        }
        Offsets siblings{n};   // ({n} | the offsets inner has for p)
        for (const auto& [other, ignored] : inner_fields)
            if (other.first == p) siblings.insert(other.second);
        const Offsets& used = uses.at(p);
        Offsets used_and_zero = used;
        used_and_zero.insert(0);
        const bool within = std::ranges::includes(used, subs) || (n == 0 && std::ranges::includes(used_and_zero, subs));
        const bool bytes_meet = std::ranges::any_of(bytes_in, [&](std::int64_t o) { return siblings.contains(o); });
        if (within && both.empty() && !bytes_meet) {
            ptrfield[key] = "struct s_" + p + " *";
            if (const auto d = deeper.find(p); d != deeper.end()) uses.at(p).insert(d->second.begin(), d->second.end());
            continue;
        }
        std::vector<std::string> fl;
        std::int64_t pos = 0;
        for (const std::int64_t o : subs) {
            if (o > pos) fl.push_back(std::format("char _{}[{}];", pos, o - pos));
            const bool byte = bytes_in.contains(o);   // (a byte read: a char - an unsigned one widened with zeros)
            const Regex given = Regex(R"re(\b)re" + re_escape(p) + "->f" + std::to_string(n) + "->f" + std::to_string(o) + R"re( = (\w+);)re");   // (given a pointer variable: a pointer)
            const auto pg = given.search(text);
            std::optional<RegexMatch> pty;
            std::optional<Regex> pointer_decl;
            if (pg) {
                pointer_decl.emplace(R"re((?m)^[ \t]*(?:register\s+)?((?:unsigned\s+)?(?:char|int)|struct \w+) \*)re" + re_escape(str((*pg)[1])) + ";$");
                pty = pointer_decl->search(text);
            }
            std::set<std::int64_t> deep;
            const Regex deeper_re(R"re(\b)re" + re_escape(p) + "->f" + std::to_string(n) + "->f" + std::to_string(o) + R"re(->f(\d+)\b)re");
            deeper_re.for_each_match(text, [&](const RegexMatch& m) {
                deep.insert(to_int(m[1]));
                return true;
            });
            if (!deep.empty() && !byte) {   // (gone through once more: a pointer to a struct of its own)
                std::vector<std::string> dl;
                std::int64_t dp = 0;
                for (const std::int64_t x : deep) {
                    if (x < dp) continue;
                    if (x > dp) dl.push_back(std::format("char _{}[{}];", dp, x - dp));
                    const bool is_long = is_long_field(x) && !deep.contains(x + 2);
                    dl.push_back(std::format("{} f{};", is_long ? "long" : "int", x));
                    dp = x + (is_long ? 4 : 2);
                }
                defs.push_back(std::format("struct s_{}_{}_{} {{ {} }};", p, n, o, ct::join(dl, " ")));
                pty.reset();
            }
            const bool lng = !byte && deep.empty() && !pty && is_long_field(o) && !subs.contains(o + 2);   // (a long one)
            std::string type;
            if (byte && unsigned_at(state.facts.unsigned_sites, o)) type = "unsigned char";
            else if (byte) type = "char";
            else if (!deep.empty()) type = std::format("struct s_{}_{}_{} *", p, n, o);
            else if (pty) type = str((*pty)[1]) + " *";
            else if (lng) type = "long";
            else type = "int";
            fl.push_back(std::format("{} f{};", type, o));
            pos = o + (byte ? 1 : lng ? 4 : 2);
        }
        defs.push_back(std::format("struct s_{}_{} {{ {} }};", p, n, ct::join(fl, " ")));
        ptrfield[key] = std::format("struct s_{}_{} *", p, n);
    }

    std::vector<std::pair<std::string, Offsets>> sorted_uses(uses.entries().begin(), uses.entries().end());
    std::ranges::sort(sorted_uses, [](const auto& a, const auto& b) { return a.first < b.first; });
    for (const auto& [p, offs] : sorted_uses) {
        std::vector<std::string> fields;
        std::int64_t pos = 0;
        const Regex self_ref(R"re(\b)re" + re_escape(p) + " = " + re_escape(p) + R"re(->f0;)re");   // (p = p->f0: a list - the field points on)
        const bool selfref = self_ref.search(text).has_value();
        for (const std::int64_t off : offs) {
            if (off < pos) continue;   // (overlaps the field before: not this way)
            if (off > pos) fields.push_back(std::format("char _{}[{}];", pos, off - pos));
            const std::string field = re_escape(p) + "->f" + std::to_string(off);
            bool uch = unsigned_at(state.facts.unsigned_sites, off);   // (widened with zeros: unsigned)
            bool byte = w(p, off) || w("*", off) || uch || (p.starts_with("r_") && w(p.substr(2) + "*", off) && !w(p.substr(2) + "w", off));
            if (byte && !uch && !w(p, off) && p.starts_with("g_") && w("bxw", off) && !w("bx*", off)) byte = false;   // (data's pointer, gone through in bx: bx's width there)
            if (!uch && byte) {   // (a byte compared with 128..255: unsigned)
                const Regex above(R"re(\b)re" + field + R"re( \S{1,2} (?:12[89]|1[3-9]\d|2[0-4]\d|25[0-5])\b)re");
                uch = above.search(text).has_value();
            }
            const Regex deref_re(R"re((?<![\w>)\]])\*)re" + field + R"re(\b(?!\s*(?:->|\[|\.)))re");   // (*p->fN: a pointer)
            bool deref = deref_re.search(text).has_value();
            const bool lng = is_long_field(off) && !offs.contains(off + 2);   // (a long field: two words given as one)
            if (lng && byte && !w(p, off) && !w(std::string(strip_left(p, "r_")) + "*", off)) byte = false;   // (a byte another pointer reads there: not this one's)
            const Regex given_value(R"re(\b)re" + field + R"re( = (\w+);)re");
            std::optional<std::string> given_to;
            if (const auto g = given_value.search(text)) {
                given_to = str((*g)[1]);
            } else {
                const Regex chained(R"re((?m)^[ \t]*(\w+) = )re" + field + " = ");   // (given with one in a chain: a pointer)
                if (const auto g2 = chained.search(text)) given_to = str((*g2)[1]);
            }
            std::optional<std::string> pointee;   // the declared type a pointer variable points to
            if (given_to) {
                const Regex decl(R"re((?m)^[ \t]*(?:register\s+)?((?:unsigned\s+)?(?:char|int)|struct \w+) \*)re" + re_escape(*given_to) + ";$");
                if (const auto d = decl.search(text)) pointee = str((*d)[1]);
            }
            if (pointee && byte && !w(p, off)) byte = false;   // (with one in a chain: a pointer - not another's byte)
            deref = deref || (pointee && *pointee == "int" && !byte && !lng);
            const std::pair<std::string, std::int64_t> key{p, off};
            if (pointee && *pointee != "int" && !byte && !lng && !ptrfield.contains(key) && !(off == 0 && selfref)) ptrfield[key] = *pointee + " *";
            std::string type;
            if (off == 0 && selfref) type = "struct s_" + p + " *";
            else if (const auto f = ptrfield.find(key); f != ptrfield.end()) type = f->second;
            else if (uch) type = "unsigned char";
            else if (byte) type = "char";
            else if (deref) type = w("bx*", 0) && !w("bxw", 0) ? "char *" : "int *";   // (*p->fN read a byte: a char *)
            else if (lng) type = "long";
            else type = "int";
            fields.push_back(std::format("{} f{};", type, off));
            pos = off + (ptrfield.contains(key) ? 2 : byte ? 1 : lng ? 4 : 2);
        }
        const std::string tag = "s_" + p;
        // (stepped through an array of them: the step is the struct's size, and in C a step of one)
        std::set<std::int64_t> steps;
        const Regex step_re(R"re(\b)re" + re_escape(p) + " = " + re_escape(p) + R"re( [+-] (\d+);)re");
        step_re.for_each_match(text, [&](const RegexMatch& m) {
            steps.insert(to_int(m[1]));
            return true;
        });
        if (steps.size() == 1) {
            const std::int64_t n = *steps.begin();
            if (n >= pos && n < 0x100) {
                if (n > pos) fields.push_back(std::format("char _{}[{}];", pos, n - pos));
                const Regex step_one(R"re(\b)re" + re_escape(p) + " = " + re_escape(p) + R"re( ([+-]) )re" + std::to_string(n) + ";");
                text = step_one.replace_all(text, p + " = " + p + " $1 1;");
            }
        }
        const std::string def = "struct " + tag + " { " + ct::join(fields, " ") + " };";
        defs.push_back(def);
        // the declarations of p: a parameter, a register variable, a local, or data (declared by localize)
        const Regex plain_decl(R"re((?m)^([ \t]*)(register )?(?:int|char \*) ?)re" + re_escape(p) + ";");
        text = plain_decl.replace_all(text, "$1$2struct " + tag + " *" + p + ";");
        text = map_lines(text, [&](const std::string& line) {
            const auto parts = int_decl_parts(line, p);
            if (!parts) return line;
            const std::string rest = parts->rest();
            const std::string declared = parts->indent + "struct " + tag + " *" + p + ";";
            return rest.empty() ? declared : parts->indent + "int " + rest + ";\n" + declared;
        });
        state.struct_pointers[p] = tag;
        for (const auto& [key, type] : ptrfield) {   // (a struct pointer field plus bytes: through a char *)
            if (key.first != p) continue;
            const Regex plus_bytes(R"re((?<![\w>)]))re" + re_escape(p) + "->f" + std::to_string(key.second) + R"re( \+ (\d+)\b)re");
            text = plus_bytes.replace_all(text, "(char *)" + p + "->f" + std::to_string(key.second) + " + $1");
        }
        static const Regex char_field(R"re(\bchar f(\d+);)re");
        char_field.for_each_match(def, [&](const RegexMatch& m) {   // (a byte field masked to a byte: itself)
            const Regex masked(R"re(\()re" + re_escape(p) + "->f" + str(m[1]) + R"re( & 0xff\))re");
            text = masked.replace_all(text, p + "->f" + str(m[1]));
            return true;
        });
        if (std::ranges::any_of(widths, [](const Site& s) { return s.pointer.ends_with("*"); })) {   // (indexed by bytes: through a char *)
            const Regex indexed(R"re((?<![\w>.]))re" + re_escape(p) + R"re(\[)re");
            text = indexed.replace_all(text, "((char *)" + p + ")[");
        }
    }
    return ct::join(defs, "\n") + "\n" + text;
}

}  // namespace cb::exact::draft::candidates
