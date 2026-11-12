#include "cb/port/old_c_text.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <tuple>

#include "cb/base/ctext.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/port/c_source.hpp"
#include "cb/port/csource.hpp"
#include "cb/port/old_c_scan.hpp"

namespace cb::port::pre_ansi {

using base::ctext::blank;

namespace {

using ArgSpan = std::pair<std::size_t, std::size_t>;

// "file:line" of the byte `at` of the code.
std::string oct_place(std::string_view fname, std::string_view code, std::size_t at) {
    return std::format("{}:{}", fname, line_of(code, at) + 1);
}

// The arguments of the call whose '(' ends just before `from`: the spans between its top-level commas. (Square brackets
// count as parentheses; a call that never closes ends at the end of the code.)
std::vector<ArgSpan> oct_call_spans(std::string_view code, std::size_t from) {
    std::vector<ArgSpan> spans;
    int depth = 1;
    std::size_t start = from;
    for (std::size_t i = from; i < code.size() && depth != 0; ++i) {
        const char c = code[i];
        depth += (c == '(' || c == '[') - (c == ')' || c == ']');
        if ((c == ',' && depth == 1) || depth == 0) {
            spans.emplace_back(start, i);
            start = i + 1;
        }
    }
    return spans;
}

bool oct_has(std::string_view s, std::string_view part) { return s.find(part) != std::string_view::npos; }
bool oct_has(std::string_view s, char c) { return s.find(c) != std::string_view::npos; }

std::string_view oct_slice(std::string_view s, ArgSpan span) { return s.substr(span.first, span.second - span.first); }

// A number the program wrote: hex, or decimal, or octal by C's rule (old_c_scan.hpp).
std::optional<std::uint64_t> oct_number(std::string_view s) { return c_integer(text::trim(s)); }

std::size_t oct_leading_space(std::string_view s) { return s.size() - text::ltrim(s).size(); }

std::string oct_squeeze(std::string_view s) { return csource::collapse_space(s); }

// The edits of one pass, made from the last to the first.
struct Replacement {
    std::size_t begin, end;
    std::string text;
};
std::string oct_apply(std::string text, std::vector<Replacement> edits) {
    std::sort(edits.begin(), edits.end(), [](const Replacement& x, const Replacement& y) {
        return std::tie(x.begin, x.end, x.text) > std::tie(y.begin, y.end, y.text);
    });
    for (const Replacement& e : edits) text.replace(e.begin, e.end - e.begin, e.text);
    return text;
}

}  // namespace

// ================================================================ widths

std::string widths(std::string text, Notes& notes, std::string_view fname, bool int16) {
    std::string code = blank(text);
    struct Change {
        std::size_t begin, end;
        std::optional<std::string> text;   // nullopt: a mark that keeps other changes off its stretch
    };
    std::vector<Change> edits;
    auto sub = [&](const Regex& rx, std::string_view neu, std::size_t lo, std::size_t hi, const char* why) {
        rx.for_each_match(std::string_view(code).substr(lo, hi - lo), [&](const RegexMatch& m) {
            const std::size_t a = lo + m.start(), b = lo + m.end();
            const bool clash = std::any_of(edits.begin(), edits.end(), [&](const Change& e) { return a < e.end && e.begin < b; });
            if (clash) return true;
            edits.push_back({a, b, std::string(neu)});
            if (why) note(notes, why, oct_place(fname, code, a));
            return true;
        });
    };
    static const Regex struct_head(R"(\b(?:struct|union)\s*\w*\s*\{)");
    static const Regex unsigned_word(R"(\bunsigned\s+short(?:\s+int)?\b|\bunsigned(?!\s+(?:char|long|short))(?:\s+int)?\b)");
    static const Regex signed_word(R"((?<!unsigned )(?<!signed )\b(?:short\s+int|short|int)\b(?!\w))");
    static const Regex unsigned_ptr_cast(R"(\(\s*unsigned(?:\s+int)?\s*(?=\*+\s*\)))");
    static const Regex int_ptr_cast(R"(\(\s*int\s*(?=\*+\s*\)))");
    static const Regex unsigned_long(R"(\bunsigned\s+long(?:\s+int)?\b|\blong\s+unsigned(?:\s+int)?\b)");
    static const Regex long_word(R"(\blong(?:\s+int)?\b)");
    static const Regex unsigned_short(R"(\bunsigned\s+short(?:\s+int)?\b)");
    static const Regex short_word(R"(\bshort(?:\s+int)?\b)");
    static const Regex main_line(R"((?m)^.*\bmain\s*\(.*$)");
    static const Regex unsigned_any(R"(\bunsigned(?!\s+(?:char|long|short))(?:\s+int)?\b)");
    static const Regex int_any(R"((?<!unsigned )(?<!signed )\bint\b)");

    struct_head.for_each_match(code, [&](const RegexMatch& m) {   // (structures: the bytes' layout)
        const std::size_t close = match_brace(code, m.end() - 1);
        sub(unsigned_word, "uint16_t", m.end(), close, "struct member unsigned -> uint16_t (the structure's bytes as they were)");
        sub(signed_word, "int16_t", m.end(), close, "struct member int -> int16_t (the structure's bytes as they were)");
        return true;
    });
    const std::size_t all = code.size();
    sub(unsigned_ptr_cast, "(uint16_t ", 0, all, "pointer cast (unsigned *) -> (uint16_t *): a word of a buffer");
    sub(int_ptr_cast, "(int16_t ", 0, all, "pointer cast (int *) -> (int16_t *): a word of a buffer");
    sub(unsigned_long, "uint32_t", 0, all, "unsigned long -> uint32_t");
    sub(long_word, "int32_t", 0, all, "long -> int32_t (32 bits, as it was)");
    sub(unsigned_short, "uint16_t", 0, all, "short -> int16_t");
    sub(short_word, "int16_t", 0, all, "short -> int16_t");
    if (int16) {   // (every int as wide as it was: a store truncates as the 8086's did; main's own are the system's)
        main_line.for_each_match(code, [&](const RegexMatch& m) {
            edits.push_back({m.start(), m.start(), std::nullopt});
            edits.push_back({m.start(), m.end(), std::nullopt});
            return true;
        });
        sub(unsigned_any, "uint16_t", 0, all, "unsigned -> uint16_t (16 bits, as it was)");
        sub(int_any, "int16_t", 0, all, "int -> int16_t (16 bits, as it was)");
        std::erase_if(edits, [](const Change& e) { return !e.text; });
    }
    std::vector<Replacement> out;
    for (const Change& e : edits) out.push_back({e.begin, e.end, *e.text});
    return oct_apply(std::move(text), std::move(out));
}

// ================================================================ formats

std::string formats(std::string text, Notes& notes, std::string_view fname, bool int16, bool hex_upper) {
    const std::string code = blank(text);   // (of the text as it is now: an earlier pass may have moved everything)
    std::vector<Replacement> edits;
    static const Regex printf_call(R"(\b(?:printf|fprintf|sprintf)\s*\()");
    static const Regex far_near(R"((%[-+ #0]*(?:\d+|\*)?(?:\.\d+)?)[FN](?=l?[spndiouxXc]))");
    static const Regex hex(R"((%[-+ #0]*(?:\d+|\*)?(?:\.\d+)?l?)x)");
    static const Regex conversion(R"(%[-+ #0]*(?:\d+|\*)?(?:\.\d+)?(l?)([diouxXcsp%]))");
    static const Regex plain_arg(R"([\w.>\[\]-]+|\w+\([^()]*\))");
    printf_call.for_each_match(code, [&](const RegexMatch& m) {
        const std::vector<ArgSpan> spans = oct_call_spans(code, m.end());
        std::optional<std::size_t> fmt;
        for (std::size_t k = 0; k < spans.size() && !fmt; ++k)
            if (text::trim(oct_slice(code, spans[k])).starts_with('"')) fmt = k;
        if (!fmt) return true;
        const auto [a, b] = spans[*fmt];
        const std::string original(oct_slice(text, spans[*fmt]));
        std::string lit = far_near.replace_all(original, "$1");
        if (lit != original)   // (%Fs, %Np: Microsoft's far and near: one memory now)
            note(notes, "%Fs / %Np -> %s / %p (Microsoft's printf: a far or near pointer; one flat memory now)", oct_place(fname, code, a));
        if (hex_upper) {   // (the old printf's %x was in capitals)
            const std::string up = hex.replace_all(lit, "${1}X");
            if (up != lit) note(notes, "%x -> %X (the old printf gave its hexadecimal in capitals)", oct_place(fname, code, a));
            lit = up;
        }
        if (lit != original) edits.push_back({a, b, lit});
        std::vector<std::pair<bool, char>> convs;   // (long?, the conversion letter)
        conversion.for_each_match(lit, [&](const RegexMatch& c) {
            convs.emplace_back(!c.group(1).empty(), c.group(2)[0]);
            return true;
        });
        std::size_t k = *fmt;
        for (const auto& [is_long, conv] : convs) {
            if (conv == '%') continue;
            if (++k >= spans.size()) break;
            const auto [x, y] = spans[k];
            const std::string_view raw = oct_slice(text, spans[k]);
            const std::string arg(text::trim(raw));
            const std::string lead(raw.substr(0, oct_leading_space(raw)));
            const std::string simple = plain_arg.full_match(arg) ? arg : "(" + arg + ")";
            if (is_long && !arg.starts_with("(long)")) {
                edits.push_back({x, y, lead + "(long)" + simple});
                note(notes, "argument of a %l. format given as (long) (a long is int32_t here)", oct_place(fname, code, x));
            } else if (int16 && !is_long && std::string_view("ouxX").find(conv) != std::string_view::npos &&
                       !arg.starts_with("(uint16_t)")) {
                edits.push_back({x, y, lead + "(uint16_t)" + simple});   // (16 bits shown, as its printf showed them)
                note(notes, "argument of a %x / %u format given as (uint16_t) (16 bits, as the old printf showed)",
                     oct_place(fname, code, x));
            }
        }
        return true;
    });
    return oct_apply(std::move(text), std::move(edits));
}

// ================================================================ mem_sizes

std::string mem_sizes(std::string text, const OrderedMap<std::string, std::uint64_t>& olds,
                      const OrderedMap<std::string, std::uint64_t>& elems, Notes& notes, std::string_view fname) {
    const std::string code = blank(text);
    std::vector<Replacement> edits;
    static const Regex mem_call(R"(\b(?:memset|memcpy|memmove|memcmp|movbytes|fillb|far_memcpy|_fmemcpy|_fmemset)\s*\()");
    static const Regex array_name(R"(\s*&?\s*(\w+)\s*)");
    static const Regex literal(R"(\s*(0[xX][0-9a-fA-F]+|\d+)\s*)");
    mem_call.for_each_match(code, [&](const RegexMatch& m) {
        const std::vector<ArgSpan> args = oct_call_spans(code, m.end());
        std::vector<std::string> names;   // (the arrays among the arguments)
        for (const ArgSpan& s : args)
            if (const auto n = array_name.full_match(oct_slice(code, s)); n && olds.contains(std::string(n->group(1))))
                names.emplace_back(n->group(1));
        for (const ArgSpan& s : args) {
            const auto lit = literal.full_match(oct_slice(code, s));
            if (!lit) continue;
            const std::uint64_t n = oct_number(lit->group(1)).value_or(0);
            for (const std::string& nm : names) {
                if (olds.at(nm) != n || n <= 1) continue;
                const std::uint64_t* e = elems.find(nm);   // (as many elements as it had, of today's size: the declaration may say no size)
                const std::string neu = e && *e != 0 && n % *e == 0 ? std::format("{} * sizeof({}[0])", n / *e, nm) : std::format("sizeof({})", nm);
                edits.push_back({s.first + lit->start(1), s.first + lit->end(1), neu});
                note(notes, "the bytes of a global array given as the number it had: sizeof it now",
                     std::format("{}:{} ({}: {})", fname, line_of(code, s.first) + 1, nm, n));
                break;
            }
        }
        return true;
    });
    return oct_apply(std::move(text), std::move(edits));
}

// ================================================================ header_offsets

std::string header_offsets(std::string text, const StructLayouts& old, Notes& notes, std::string_view fname) {
    std::vector<std::string> lines = split_lines(text);
    std::vector<std::string> code = split_lines(blank(text));
    static const Regex cast_head(R"(\(\s*(struct\s+\w+)\s*\*\s*\)\s*(\w+)\b(?!\s*(?:\(|->|\[)))");
    static const Regex pointer_decl(R"(\b(struct\s+\w+)\s*\*\s*(\w+)\s*[;,])");
    for (const NamedDefinition& def : definitions_any(lines, code)) {
        const std::string body = join_lines(std::vector<std::string>(code.begin() + static_cast<std::ptrdiff_t>(def.first),
                                                                      code.begin() + static_cast<std::ptrdiff_t>(def.last) + 1));
        std::vector<std::pair<std::string, std::string>> heads;   // (structure, the variable that is a head of it)
        cast_head.for_each_match(body, [&](const RegexMatch& m) {
            heads.emplace_back(m.group(1), m.group(2));
            return true;
        });
        pointer_decl.for_each_match(body, [&](const RegexMatch& m) {   // (b = p, b a structure's pointer)
            csource::name_regex(R"(\b@@\s*=\s*(\w+)\s*;)", m.group(2)).for_each_match(body, [&](const RegexMatch& a) {
                heads.emplace_back(m.group(1), a.group(1));
                return true;
            });
            return true;
        });
        for (const auto& [raw_struct, var] : heads) {
            const std::string st = oct_squeeze(raw_struct);
            const StructLayout* lay = old.find(st);
            if (!lay) continue;
            std::optional<std::pair<std::uint32_t, std::string>> trailing;   // (the last array member: its offset, its name)
            std::uint32_t last_offset = 0;
            for (const Member& mem : lay->members) {
                last_offset = std::max(last_offset, mem.offset);
                if (oct_has(mem.declarator, '[')) {
                    const std::pair<std::uint32_t, std::string> key{mem.offset, mem.name};
                    if (!trailing || key > *trailing) trailing = key;
                }
            }
            if (!trailing) continue;
            const auto& [off, mem] = *trailing;
            if (off < 4 || off < last_offset) continue;   // (its last member, the bytes after the head)
            const std::string off_text = std::to_string(off);
            const std::string number = R"re((?<![\w.]))re" + off_text + R"re((?![\w.]))re";
            const std::string what = std::format("offsetof({}, {})", st, mem);
            const std::string patterns[] = {R"(\b@@\s*\[[^\]\n]*)" + number, R"(\b@@\s*=\s*(?:\([^()]*\)\s*)?\w+\s*\([^;\n]*)" + number,
                                            R"(\b@@\s*\+\s*)" + number};
            for (std::size_t x = def.first; x <= def.last; ++x) {
                std::string c = code[x], l = lines[x];
                for (const std::string& pattern : patterns) {
                    std::vector<std::size_t> ends;
                    csource::name_regex(pattern, var).for_each_match(c, [&](const RegexMatch& mm) {
                        ends.push_back(mm.end());
                        return true;
                    });
                    for (auto it = ends.rbegin(); it != ends.rend(); ++it) {
                        const std::size_t b = *it - off_text.size();
                        if (std::string_view(c).substr(b, off_text.size()) == off_text) {
                            l.replace(b, off_text.size(), what);
                            c.replace(b, off_text.size(), what);
                        }
                    }
                }
                if (l != lines[x]) {
                    lines[x] = std::move(l);
                    code[x] = std::move(c);
                    note(notes, "a structure's head given as the bytes it was (its array after it): offsetof",
                         std::format("{}:{} ({})", fname, x + 1, what));
                }
            }
        }
    }
    return join_lines(lines);
}

// ================================================================ struct_sizes

std::optional<std::string> prefix_member(const StructLayout& layout, std::uint64_t n) {
    if (n == 0 || n >= layout.size) return std::nullopt;
    for (const Member& m : layout.members)
        if (m.offset == n) return m.name;
    return std::nullopt;
}

namespace {

bool oct_is_keyword(std::string_view w) { return is_keyword(w); }

// The integer literals of a comma-separated argument list (`0x1f` or `[1-9]\d*`, nothing else in the argument).
std::vector<std::uint64_t> oct_literal_args(std::string_view args) {
    static const Regex number(R"(\s*(?:0[xX][0-9a-fA-F]+|[1-9]\d*)\s*)");
    std::vector<std::uint64_t> out;
    for (std::string_view a : text::split(args, ','))
        if (number.full_match(a)) out.push_back(oct_number(a).value_or(0));
    return out;
}

}  // namespace

std::string struct_sizes(std::string text, const StructLayouts& old, Notes& notes, std::string_view fname,
                         const Allocators& allocs, AllocatorSizes& pools) {
    std::string code = blank(text);
    static const Regex pointer_var(R"(\b((?:struct|union)\s+\w+)\s*\*\s*(\w+)\s*[;,)=])");
    static const Regex call_assign(R"(\b(\w+)\s*=\s*([A-Za-z_]\w*)\s*\(([^()]*)\))");
    static const Regex nested_call(R"(\b(\w+)\s*=\s*([A-Za-z_]\w*)\s*\(\s*([A-Za-z_]\w*)\s*\(([^()]*)\)\s*\))");
    static const Regex table_pointer_cast(R"(\(\s*((?:struct|union)\s+\w+|\w+)\s*\*\s*\*\s*\)\s*([A-Za-z_]\w*)\s*\()");
    static const Regex cast_call(R"(\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*([A-Za-z_]\w*)\s*\()");
    static const Regex plain_name(R"(\s*[A-Za-z_]\w*\s*)");
    static const Regex positive_number(R"(\s*[1-9]\d*\s*)");
    static const Regex literal_number(R"(\s*(?:0[xX][0-9a-fA-F]+|[1-9]\d*)\s*)");
    static const Regex number_in(R"((?<![\w.])(0[xX][0-9a-fA-F]+|[1-9]\d*)(?![\w.]))");
    static const Regex number_any(R"((?<![\w.])(0[xX][0-9a-fA-F]+|\d+)(?![\w.]))");
    static const Regex compared_before(R"((?:[=!<>]=|[<>])\s*$)");
    static const Regex compared_after(R"(\s*(?:[=!<>]=|[<>]))");
    static const Regex char_cast(R"(\s*\(\s*(?:unsigned\s+|signed\s+)?char\s*\))");

    // (p = f04fb5(0, 10); p a struct tcell *: as if the call were cast to it)
    std::map<std::string, std::string, std::less<>> ptr_of;
    pointer_var.for_each_match(code, [&](const RegexMatch& m) {
        ptr_of.emplace(std::string(m.group(2)), oct_squeeze(m.group(1)));
        return true;
    });
    auto table_read = [&](std::string_view name) {   // (sz = g_11a8[kind].n;: a size a table of old byte counts gives)
        return csource::name_regex(R"(\b@@\s*=\s*\w+\s*\[[^\]]*\]\s*\.\s*\w+\s*;)", text::trim(name)).search(code).has_value();
    };
    std::vector<std::pair<std::size_t, std::string>> casts;   // (where a cast goes in, the cast)
    call_assign.for_each_match(code, [&](const RegexMatch& m) {
        const auto owner = ptr_of.find(std::string(m.group(1)));
        if (owner == ptr_of.end()) return true;
        const std::string& st = owner->second;
        const StructLayout* lay = old.find(st);
        if (!lay || oct_is_keyword(m.group(2)) || oct_has(m.group(2), "alloc")) return true;
        const std::vector<std::uint64_t> lits = oct_literal_args(m.group(3));
        std::size_t tabled = 0;
        for (std::string_view a : text::split(m.group(3), ','))
            if (plain_name.full_match(a) && table_read(a)) ++tabled;
        const bool whole = std::find(lits.begin(), lits.end(), lay->size) != lits.end();
        // (or, to an allocator, a part's size: s = f(1, 10), s = f(far, sz))
        const bool part = allocs.contains(std::string(m.group(2))) &&
                          (std::count_if(lits.begin(), lits.end(), [&](std::uint64_t n) { return prefix_member(*lay, n).has_value(); }) == 1 ||
                           (tabled == 1 && lits.empty()));
        if (whole || part) casts.emplace_back(m.start(2), "(" + st + " *)");   // (its size, or a prefix's: 10 where a member of it began)
        return true;
    });
    nested_call.for_each_match(code, [&](const RegexMatch& m) {   // (t = f08e60(f0a42b(3, 22)); t a struct pnode *: the block made, then filled)
        const auto owner = ptr_of.find(std::string(m.group(1)));
        if (owner == ptr_of.end()) return true;
        const StructLayout* lay = old.find(owner->second);
        if (!lay || oct_is_keyword(m.group(3)) || oct_has(m.group(3), "alloc")) return true;
        for (std::uint64_t n : oct_literal_args(m.group(4)))
            if (n == lay->size) {
                casts.emplace_back(m.start(3), "(" + owner->second + " *)");
                break;
            }
        return true;
    });
    std::stable_sort(casts.begin(), casts.end(), [](const auto& x, const auto& y) { return x.first < y.first; });
    for (auto it = casts.rbegin(); it != casts.rend(); ++it) text.insert(it->first, it->second);
    code = blank(text);

    std::vector<Replacement> pointer_edits;   // ((struct sym **)f(1, 22): a table of pointers by its bytes then, two each: its pointers' now)
    table_pointer_cast.for_each_match(code, [&](const RegexMatch& m) {
        if (oct_is_keyword(m.group(2)) || m.group(2) == "memset" || m.group(2) == "memcpy" || m.group(2) == "memmove" ||
            m.group(2) == "memcmp" || m.group(2) == "movbytes" || m.group(2) == "fillb" || m.group(2) == "far_memcpy" ||
            m.group(2) == "_fmemcpy" || m.group(2) == "_fmemset")
            return true;
        const std::vector<ArgSpan> args = oct_call_spans(code, m.end());
        const ArgSpan* last = nullptr;
        for (const ArgSpan& s : args)
            if (positive_number.full_match(oct_slice(code, s))) last = &s;
        if (!last) return true;
        const std::uint64_t n = text::parse_int<std::uint64_t>(text::trim(oct_slice(code, *last))).value_or(0);
        if (n % 2 == 0) {
            pointer_edits.push_back({last->first, last->second, std::format(" {} * sizeof({} *)", n / 2, m.group(1))});
            note(notes, "a table of pointers given by its bytes then (two a pointer): as many of today's",
                 std::format("{}:{} ({} **: {})", fname, line_of(code, last->first) + 1, m.group(1), n));
        }
        return true;
    });
    text = oct_apply(std::move(text), std::move(pointer_edits));
    code = blank(text);

    std::vector<Replacement> edits;
    cast_call.for_each_match(code, [&](const RegexMatch& m) {
        const std::string s = oct_squeeze(m.group(1));
        const StructLayout* lay = old.find(s);
        if (!lay) return true;
        std::vector<ArgSpan> args = oct_call_spans(code, m.end());
        if (args.empty()) return true;
        const std::string callee(m.group(2));
        if (!oct_has(callee, "alloc")) {   // (a routine of the program's that gives it: one of its arguments the old size itself)
            std::vector<ArgSpan> whole;
            for (const ArgSpan& a : args)
                if (literal_number.full_match(oct_slice(code, a)) && oct_number(oct_slice(code, a)) == lay->size) whole.push_back(a);
            const std::set<std::size_t>* known = nullptr;   // (the argument places its size was given at, where known)
            if (const auto it = allocs.find(callee)) known = it;
            if (whole.size() > 1 && known && known->size() == 1 && *known->begin() < args.size() &&
                std::find(whole.begin(), whole.end(), args[*known->begin()]) != whole.end())
                whole = {args[*known->begin()]};   // (f03e86(4, 4): the one its size is given at)
            if (whole.size() != 1 && !allocs.contains(callee)) return true;   // (a part's size: only to an allocator's call)
            if (whole.size() != 1) {
                std::vector<ArgSpan> tabled;   // (s = f(far, sz), sz = g_11a8[kind].n: the structure whole now: no part of it is read past the kind's own part)
                for (const ArgSpan& a : args)
                    if (plain_name.full_match(oct_slice(code, a)) && table_read(oct_slice(code, a))) tabled.push_back(a);
                const bool any_literal = std::any_of(args.begin(), args.end(), [&](const ArgSpan& a) { return literal_number.full_match(oct_slice(code, a)); });
                if (tabled.size() == 1 && !any_literal) {
                    const ArgSpan a = tabled[0];
                    const std::string_view arg = oct_slice(code, a);
                    edits.push_back({a.first, a.second, std::string(arg.substr(0, oct_leading_space(arg))) + "sizeof(" + s + ")"});
                    note(notes, "a structure's block of a size a table of old byte counts gives (a part of it, by kind): the whole structure now",
                         std::format("{}:{} ({}: {})", fname, line_of(code, a.first) + 1, s, text::trim(arg)));
                    return true;
                }
                const std::set<std::size_t>* at = known;   // (the place its size was given at, where known)
                const ArgSpan chosen = at && at->size() == 1 && *at->begin() < args.size() ? args[*at->begin()] : args.back();
                const std::string_view arg = oct_slice(code, chosen);
                struct Literal {
                    std::size_t begin, end;
                    std::uint64_t value;
                };
                std::vector<Literal> lits;   // (not a compared one: no size)
                number_in.for_each_match(arg, [&](const RegexMatch& x) {
                    if (compared_before.search(arg.substr(0, x.start())) || compared_after.match(arg.substr(x.end()))) return true;
                    lits.push_back({x.start(), x.end(), oct_number(x.group(1)).value_or(0)});
                    return true;
                });
                const bool all_sizes = !lits.empty() && std::all_of(lits.begin(), lits.end(), [&](const Literal& x) {
                    return x.value == lay->size || prefix_member(*lay, x.value).has_value();
                });
                if (all_sizes && std::any_of(lits.begin(), lits.end(), [&](const Literal& x) { return prefix_member(*lay, x.value).has_value(); })) {
                    // (a block of a structure's first members, as many bytes as its members up to the one there: the structure whole now)
                    for (auto it = lits.rbegin(); it != lits.rend(); ++it)
                        edits.push_back({chosen.first + it->begin, chosen.first + it->end, "sizeof(" + s + ")"});
                    if (const auto cm = char_cast.match(arg)) {   // ((char)(...): a byte held the old size: not now)
                        const std::size_t lead = oct_leading_space(arg);
                        edits.push_back({chosen.first + lead, chosen.first + cm->end(), ""});
                    }
                    note(notes, "a block of a structure's first members, given as the bytes they were: the whole structure",
                         std::format("{}:{} ({}: {})", fname, line_of(code, chosen.first) + 1, s, text::trim(arg)));
                }
                return true;
            }
            args = whole;
        }
        const ArgSpan span = callee != "realloc" ? args.front() : args.back();
        const std::string_view arg = oct_slice(code, span);
        struct Number {
            std::size_t begin, end;
            std::uint64_t value;
        };
        std::optional<Number> biggest;   // (the largest number in the argument; the first of equals)
        number_any.for_each_match(arg, [&](const RegexMatch& x) {
            const std::uint64_t v = oct_number(x.group(1)).value_or(0);
            if (!biggest || v > biggest->value) biggest = Number{x.start(), x.end(), v};
            return true;
        });
        if (!biggest) return true;
        const std::uint64_t n = biggest->value, size = lay->size;
        if (n < size / 2) return true;   // (a count, not the structure)
        std::string neu = "sizeof(" + s + ")";
        if (n > size) neu += std::format(" + {}", n - size);
        if (n < size) neu += std::format(" - {}", size - n);
        edits.push_back({span.first + biggest->begin, span.first + biggest->end, neu});
        if (callee != "malloc" && callee != "calloc" && callee != "realloc" && callee != "_fmalloc" && n == size && oct_has(callee, "alloc"))
            pools[callee].insert({s, n});   // (the program's own allocator: given this size)
        note(notes, "a structure's size given as the number it had (16-bit layout): sizeof and the rest",
             std::format("{}:{} ({}: {})", fname, line_of(code, span.first) + 1, s, n));
        return true;
    });
    text = oct_apply(std::move(text), std::move(edits));
    code = blank(text);

    // (a record carved from a pool: p = (struct S *)pool; pool += size: its size the old one's)
    static const Regex pool_cast(R"(\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*(\w+)\s*;)");
    std::map<std::size_t, std::pair<std::size_t, std::string>> pool_edits;
    pool_cast.for_each_match(code, [&](const RegexMatch& m) {
        const std::string s = oct_squeeze(m.group(1));
        const StructLayout* lay = old.find(s);
        if (!lay) return true;
        const std::string_view after = std::string_view(code).substr(m.end(), 400);
        const auto bump = csource::name_regex(R"(\b@@\s*\+=\s*(\w+)\s*;)", m.group(2)).search(after);
        if (!bump) return true;
        const std::string var(bump->group(1));
        const std::size_t brace = code.rfind("\n{", m.start());
        if (brace == std::string::npos) return true;   // (no function head before it: nothing to read the size from)
        const std::string_view region = std::string_view(code).substr(brace, m.start() - brace);
        bool found = false;
        csource::name_regex(R"(\b@@\s*=\s*(\d+)\s*;)", var).for_each_match(region, [&](const RegexMatch& a) {
            const std::uint64_t n = text::parse_int<std::uint64_t>(a.group(1)).value_or(0);
            if (n < 2 || n > lay->size) return true;
            const std::size_t x = brace + a.start(1);
            pool_edits[x] = {x + a.group(1).size(), "sizeof(" + s + ")"};
            note(notes, "a record carved from a pool by the old size of its structure (part of it): the whole structure's size now, so that none reaches into the next",
                 std::format("{}:{} ({}: {})", fname, line_of(code, x) + 1, s, n));
            found = true;
            return false;
        });
        if (found) return true;
        // (size = extra[kind] + n + 0x1c: the old common part, what the kind had)
        std::set<std::uint64_t> offsets{lay->size};
        for (const Member& mem : lay->members) offsets.insert(mem.offset);
        static const Regex terms_in(R"(0[xX][0-9a-fA-F]+|\d+)");
        static const Regex array_term(R"(\w+\s*\[[^\]]*\])");
        csource::name_regex(R"(\b@@\s*=\s*([^;=]+);)", var).for_each_match(region, [&](const RegexMatch& a) {
            std::vector<std::string> terms;
            for (std::string_view t : text::split(a.group(1), '+')) terms.emplace_back(text::trim(t));
            std::vector<std::string> lits;
            for (const std::string& t : terms)
                if (terms_in.full_match(t)) lits.push_back(t);
            if (lits.size() != 1 || oct_has(a.group(1), '-')) return true;
            const std::uint64_t value = oct_number(lits[0]).value_or(0);
            if (!offsets.contains(value) || value < 2) return true;
            std::vector<std::string> keep;
            for (const std::string& t : terms)
                if (std::find(lits.begin(), lits.end(), t) == lits.end() && !array_term.full_match(t)) keep.push_back(t);
            const std::size_t x = brace + a.start(1), e = brace + a.end(1);
            std::string neu = "sizeof(" + s + ")";
            for (const std::string& k : keep) neu += " + " + k;
            pool_edits[x] = {e, neu};
            note(notes, "a record carved from a pool by the old sizes of its parts (the common part, what its kind had past it): the whole structure now, so that none reaches into the next",
                 std::format("{}:{} ({}: {})", fname, line_of(code, x) + 1, s, text::trim(a.group(1))));
            return false;
        });
        return true;
    });
    for (auto it = pool_edits.rbegin(); it != pool_edits.rend(); ++it)
        text.replace(it->first, it->second.first - it->first, it->second.second);
    return text;
}

// ================================================================ old_declarations

std::string old_declarations(std::string_view text, const std::set<std::string, std::less<>>& known, Notes& notes,
                             std::string_view fname, const std::vector<std::string>& platform) {
    std::vector<std::string> lines = split_lines(text), clines = split_lines(blank(text));
    // (extern f(), g(),\n h(); is one declaration: one line)
    std::vector<std::string> joined_lines, joined_code;
    for (std::size_t i = 0; i < lines.size();) {
        const std::string_view s = text::trim(clines[i]);
        if (s.starts_with("extern") && !s.ends_with(';') && !oct_has(s, '{')) {
            std::size_t k = i + 1;
            while (k < lines.size() && k - i < 12 && !text::trim(clines[k]).ends_with(';') && !oct_has(clines[k], '{')) ++k;
            bool has_hash = false;
            for (std::size_t j = i; k < lines.size() && j <= k; ++j) has_hash = has_hash || oct_has(clines[j], '#');
            if (k < lines.size() && k - i < 12 && !oct_has(clines[k], '{') && !has_hash) {
                std::string l(text::rtrim(lines[i])), c(text::rtrim(clines[i]));
                for (std::size_t j = i + 1; j <= k; ++j) {
                    l += " " + std::string(text::trim(lines[j]));
                    c += " " + std::string(text::trim(clines[j]));
                }
                joined_lines.push_back(std::move(l));
                joined_code.push_back(std::move(c));
                i = k + 1;
                continue;
            }
        }
        joined_lines.push_back(lines[i]);
        joined_code.push_back(clines[i]);
        ++i;
    }
    std::vector<std::string> out;
    static const Regex struct_name(R"(\b(?:struct|union)\s+\w+)");
    for (std::size_t n = 0; n < joined_lines.size(); ++n) {
        const std::string& l = joined_lines[n];
        const std::string_view s = text::trim(joined_code[n]);
        bool plat = false;   // (the platform layer declares it: what the old runtime's assembler defined)
        if (s.starts_with("extern"))
            for (const std::string& p : platform) plat = plat || (!p.empty() && find_word(s, p) != std::string_view::npos);
        std::optional<Declaration> r;
        if (s.ends_with(';') && (oct_has(s, "()") || plat) && !oct_has(s, '=')) r = declarators(s);
        bool all_types = r && !r->base.empty();
        if (all_types) {
            const std::string plain_words = struct_name.replace_all(r->base, "");
            for (std::string_view w : text::split_ws(plain_words)) all_types = all_types && is_type_word(w);
        }
        if (!all_types) {
            out.push_back(l);
            continue;
        }
        std::vector<std::string> keep;
        for (const Declarator& d : r->names) {
            const bool known_function = d.text.ends_with("()") && known.contains(d.name);
            const bool platform_name = std::find(platform.begin(), platform.end(), d.name) != platform.end();
            if (!known_function && !platform_name) keep.push_back(d.text);
        }
        if (keep.size() == r->names.size()) {
            out.push_back(l);
            continue;
        }
        note(notes, "old declaration removed (proto.h / the C library declare it)", std::format("{}:{}", fname, out.size() + 1));
        if (!keep.empty()) out.push_back(std::string(indent_of(l)) + r->base + " " + text::join(keep, ", ") + ";");
    }
    return join_lines(out);
}

// ================================================================ header_data

HeaderData header_data(std::string_view text, std::string_view fname, Notes& notes) {
    std::vector<std::string> lines = split_lines(text);
    const std::vector<std::string> clines = split_lines(blank(text));
    const std::vector<int> ds = brace_depths(clines);
    static const Regex star_paren(R"(\(\s*\*)");
    HeaderData out;
    for (std::size_t i = 0; i < clines.size(); ++i) {
        const std::string_view s = text::trim(clines[i]);
        if (ds[i] != 0 || !s.ends_with(';') || s.starts_with('#') || s.starts_with("extern") || s.starts_with("typedef") ||
            s.starts_with("static") || (oct_has(s, '(') && !star_paren.search(s)) || oct_has(s, '{') || oct_has(s, '}') || oct_has(s, '='))
            continue;
        const auto r = declarators(s);
        if (!r || r->base.empty()) continue;
        out.definitions.emplace_back(text::trim(lines[i]));
        lines[i] = std::string(indent_of(lines[i])) + "extern " + lines[i].substr(indent_of(lines[i]).size());
        note(notes, "shared variable of a header: extern there, defined once in data.c", std::format("{}:{}", fname, i + 1));
    }
    out.text = join_lines(lines);
    return out;
}

}  // namespace cb::port::pre_ansi
