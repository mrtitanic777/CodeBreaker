// The model's text made the project's source: the engine's names for routines and data, and the declarations the text
// needs. Part of cb/exact/draft/candidates.hpp.
#include <algorithm>
#include <array>
#include <format>
#include <map>
#include <set>

#include "core/regex.hpp"
#include "core/text.hpp"
#include "exact/draft/candidates_internal.hpp"
#include "exact/draft/shapes.hpp"

namespace cb::exact::draft::candidates {

using namespace inner;

namespace {

std::string loc_join_names(const std::vector<std::string>& names, sv suffix = "") {
    std::vector<std::string> out;
    for (const std::string& n : names) out.push_back(n + str(suffix));
    return ct::join(out, ", ");
}

bool loc_in(const std::vector<std::string>& v, const std::string& x) { return std::ranges::find(v, x) != v.end(); }

std::vector<std::string> loc_without(const std::vector<std::string>& v, const std::vector<std::string>& drop) {
    std::vector<std::string> out;
    for (const std::string& x : v)
        if (!loc_in(drop, x)) out.push_back(x);
    return out;
}

// The first word of a text: a macro's name.
std::optional<std::string> loc_first_word(sv s) {
    std::size_t e = 0;
    while (e < s.size() && is_word(s[e])) ++e;
    if (e == 0) return std::nullopt;
    return str(s.substr(0, e));
}

// The name of the routine at `place` (the engine's names; else f<place>), where a near call wraps in its 64K.
std::string loc_routine(const std::map<std::uint32_t, std::string>& names, std::int64_t at, std::int64_t rel) {
    if (rel >= 0x80000000LL) rel -= 1LL << 32;
    const auto named = [&](std::int64_t t) -> const std::string* {
        if (t < 0 || t > 0xFFFFFFFFLL) return nullptr;
        const auto f = names.find(static_cast<std::uint32_t>(t));
        return f == names.end() ? nullptr : &f->second;
    };
    std::int64_t t = at + rel >= 0 ? (at + rel) & 0xFFFFF : at + rel + 0x10000;   // (a near call wraps in its 64K: the segment's)
    if (!named(t)) {   // (the 64K it wraps in: where a routine is)
        for (const std::int64_t x : {t - 0x10000, t + 0x10000}) {
            if (named(x)) {
                t = x;
                break;
            }
        }
    }
    if (const std::string* n = named(t); n && !n->empty()) return *n;
    return std::format("f{:05x}", t);
}

}  // namespace

Candidate localize(const Settings& settings, std::string_view code, std::uint32_t at, const std::map<std::uint32_t, std::string>& names, State& state,
                   text::ProjectCaches& caches) {
    auto& facts = state.facts;
    static const Regex sub_call(R"re(\bsub_(-?[0-9a-f]+)\()re");
    const auto call = [&](const RegexMatch& m) {
        const bool negative = m[1].starts_with("-");
        const std::int64_t magnitude = *ct::parse_int<std::int64_t>(negative ? m[1].substr(1) : m[1], 16);
        return loc_routine(names, at, negative ? -magnitude : magnitude) + "(";
    };
    std::set<std::string> long_funcs;   // (routines of a long value)
    for (const std::string& f : facts.longs.funcs) {
        const std::string named = sub_all(sub_call, f + "(", call);
        long_funcs.insert(named.substr(0, named.size() - 1));
    }
    std::string text = sub_all(sub_call, code, call);
    static const Regex data_place(R"re(\bg_([0-9a-f]{1,4})\b)re");
    text = sub_all(data_place, text, [](const RegexMatch& m) { return std::format("g_{:04x}", *ct::parse_int<std::int64_t>(m[1], 16)); });
    static const Regex label_last(R"re((?m)^([ \t]*)(L_\w+:)\n(?=[ \t]*\}))re");
    text = label_last.replace_all(text, "$1$2\n$1;\n");   // (a label last in its block: on an empty statement)
    static const Regex post_paren(R"re(\*\((\w+)(\+\+|--)\))re");
    text = post_paren.replace_all(text, "*$1$2");   // (*(p++): *p++ - the same, and mendable)
    text = fp_conversions(text, settings.fp_conversions, state);
    text = long_helpers(text, settings.long_helpers);
    text = forms::library_macros(text, settings.macros, facts);   // (the routines named: the macros they show)
    text = shapes::call_tests(forms::data_macros(settings.kit, caches, facts, text, settings.data_macros));   // (a macro's value tested, switched on)
    const bool has_streams = !facts.streams.empty() || facts.include || facts.include2;
    const auto normal = [](const std::string& n) {
        static const Regex data(R"re(g_[0-9a-f]{1,4})re");
        return data.full_match(n) ? std::format("g_{:04x}", *ct::parse_int<std::int64_t>(sv(n).substr(2), 16)) : n;
    };
    if (has_streams) {   // (the stream's fields the struct pass read: the macro's now)
        std::set<std::string> streams;
        for (const auto& kv : facts.streams) streams.insert(normal(kv.first));
        static const Regex stream_struct(R"re((?m)^struct s_(\w+) \{[^}]*\};\n)re");
        text = sub_all(stream_struct, text, [&](const RegexMatch& m) { return streams.contains(normal(str(m[1]))) ? std::string() : str(m.whole()); });
        text = shapes::fold_registers(shapes::stored_calls(text));
        text = forms::unused_locals(text);
    }
    std::set<std::string> macro_names;
    for (const forms::LibraryMacroSpec& m : settings.macros)
        if (const auto w = loc_first_word(m.as)) macro_names.insert(*w);   // (a macro that begins with no name has none)
    for (const forms::DataMacroSpec& sp : settings.data_macros)
        for (const auto& kv : sp.fields)
            if (const auto w = loc_first_word(kv.second)) macro_names.insert(*w);
    const auto body_of = [](const std::string& t) {
        const std::size_t b = t.find('{');
        return b == npos ? t : t.substr(b + 1);
    };
    std::string body = body_of(text);
    // registers the model left: a draft for a person, not for the compiler
    static const Regex string_literal(R"re("(?:[^"\\]|\\.)*")re");
    static const Regex register_word(R"re(\b(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh|es)\b)re");
    std::set<std::string> left_set;
    register_word.for_each_match(string_literal.replace_all(body, ""), [&](const RegexMatch& m) {
        left_set.insert(str(m[1]));
        return true;
    });
    const std::vector<std::string> left(left_set.begin(), left_set.end());
    static const Regex data_use(R"re(\bg_[0-9a-f]{4}\b)re");
    std::set<std::string> data_set;
    data_use.for_each_match(text, [&](const RegexMatch& m) {
        data_set.insert(str(m.whole()));
        return true;
    });
    std::vector<std::string> data(data_set.begin(), data_set.end());
    std::string own_name;   // (its own name: after a type)
    {
        const std::size_t paren = text.find('(');
        const std::vector<sv> words = ct::split_ws(sv(text).substr(0, paren == npos ? text.size() : paren));
        if (!words.empty()) own_name = str(words.back());
    }
    static const Regex called(R"re(\b([A-Za-z_]\w*)\()re");
    std::set<std::string> func_set;
    called.for_each_match(body, [&](const RegexMatch& m) {
        func_set.insert(str(m[1]));
        return true;
    });
    for (const std::string& skip : {own_name, std::string("if"), std::string("while"), std::string("for"), std::string("switch"), std::string("return"), std::string("sizeof")})
        func_set.erase(skip);
    for (const std::string& m : macro_names) func_set.erase(m);
    std::vector<std::string> funcs(func_set.begin(), func_set.end());
    std::vector<std::string> decl;
    std::vector<std::string> fptr;   // (data called through)
    for (const std::string& d : data) {
        const Regex through(R"re(\(\s*\*\s*)re" + d + R"re(\s*\)\s*\()re");
        if (through.search(body)) fptr.push_back(d);
    }
    for (const std::string& d : fptr) {   // (a number given it that is a function's place: it)
        const Regex given_number(R"re((?m)^(\s*)re" + d + R"re( = )(\d+);)re");
        text = sub_all(given_number, text, [&](const RegexMatch& m) {
            const std::int64_t place = to_int(m[2]);
            const auto f = place >= 0 && place <= 0xFFFFFFFFLL ? names.find(static_cast<std::uint32_t>(place)) : names.end();
            return str(m[1]) + (f != names.end() && !f->second.empty() ? f->second : std::format("f{:05x}", place)) + ";";
        });
        body = body_of(text);
    }
    {
        std::set<std::string> more(funcs.begin(), funcs.end());
        for (const std::string& d : fptr) {
            const Regex assigned(R"re((?m)^[ \t]*)re" + d + R"re( = ([A-Za-z_]\w*);)re");
            assigned.for_each_match(body, [&](const RegexMatch& m) {
                more.insert(str(m[1]));
                return true;
            });
        }
        funcs.assign(more.begin(), more.end());
    }
    for (const std::string& a : std::vector<std::string>(data)) {   // (an array of words named whole, and the words just after it by their own places: its elements)
        const auto array_type = facts.arrays.find(a);
        if (array_type == facts.arrays.end() || array_type->second != "int") continue;
        const std::int64_t base = *ct::parse_int<std::int64_t>(sv(a).substr(2), 16);
        std::vector<std::string> near;
        for (const std::string& d : data) {
            if (d == a || facts.arrays.contains(d)) continue;
            const std::int64_t delta = *ct::parse_int<std::int64_t>(sv(d).substr(2), 16) - base;
            if (0 < delta && delta < 0x40 && delta % 2 == 0) near.push_back(d);
        }
        const Regex whole(R"re(\b)re" + a + R"re(\b(?!\[))re");
        if (whole.search(body) || !near.empty()) {
            const std::size_t brace = text.find('{');
            const std::string hd = brace == npos ? text : text.substr(0, brace);
            std::string bd = brace == npos ? std::string() : text.substr(brace + 1);
            bd = whole.replace_all(bd, a + "[0]");
            for (const std::string& d : near) {
                const std::int64_t element = (*ct::parse_int<std::int64_t>(sv(d).substr(2), 16) - base) / 2;
                bd = replace_word(bd, d, a + "[" + std::to_string(element) + "]");
            }
            text = hd + (brace == npos ? "" : "{") + bd;
            body = bd;
            data = loc_without(data, near);
        }
    }
    std::vector<std::string> arrs;   // (indexed in place: an array, not a pointer)
    for (const std::string& d : data)
        if (facts.arrays.contains(d) && !loc_in(fptr, d)) arrs.push_back(d);
    static const Regex pointer_var(R"re((?m)^[ \t]*(?:register\s+)?(?:unsigned\s+)?(?:char|int)\s*\*\s*(\w+);)re");
    static const Regex struct_pointer_var(R"re((?m)^[ \t]*(?:register\s+)?(struct \w+)\s*\*\s*(\w+);)re");
    std::set<std::string> pvars;   // (pointers the draft declares)
    pointer_var.for_each_match(text, [&](const RegexMatch& m) {
        pvars.insert(str(m[1]));
        return true;
    });
    OrderedMap<std::string, std::string> sptrs;
    struct_pointer_var.for_each_match(text, [&](const RegexMatch& m) {
        sptrs.insert_or_assign(str(m[2]), str(m[1]));
        return true;
    });
    for (const auto& kv : sptrs) pvars.insert(kv.first);
    const auto flows = [&](const std::string& d) {
        return std::ranges::any_of(pvars, [&](const std::string& p) {
            const Regex flow(R"re((?m)^[ \t]*)re" + re_escape(p) + " = " + d + R"re(\b|^\s*)re" + d + " = " + re_escape(p) + R"re(\b)re");
            return flow.search(body).has_value();
        });
    };
    for (const std::string& d : data) {   // (data given a struct pointer's value: a pointer to it)
        std::optional<std::string> source;
        for (const auto& kv : sptrs) {
            const Regex copy(R"re((?m)^[ \t]*(?:\w+ = )*)re" + d + " = (?:\\w+ = )*" + re_escape(kv.first) + ";$");
            if (copy.search(body)) {
                source = kv.first;
                break;
            }
        }
        if (source && !state.struct_pointers.contains(d)) {
            const std::vector<sv> words = ct::split_ws(sptrs.at(*source));
            state.struct_pointers[d] = str(words.at(1));
        }
    }
    std::vector<std::string> ptrs, ints;
    for (const std::string& d : data) {
        if (loc_in(fptr, d) || loc_in(arrs, d)) continue;
        const Regex through(R"re(\*\s*\(?\s*(?:\+\+|--)?\s*)re" + d + R"re(\b|\b)re" + d + R"re(\s*(?:\+\+|--)?\s*(?:->|\[))re");
        if (through.search(body) || flows(d)) ptrs.push_back(d);   // (or given a pointer's value, or giving one)
    }
    for (const std::string& d : data)
        if (!loc_in(ptrs, d) && !loc_in(fptr, d) && !loc_in(arrs, d)) ints.push_back(d);
    static const Regex block_copy(R"re(\*\(struct _b(\d+) \*\)(\w+) = \*\(struct _b\1 \*\)(\w+);)re");
    std::vector<std::array<std::string, 3>> copies;
    block_copy.for_each_match(text, [&](const RegexMatch& m) {
        copies.push_back({str(m[1]), str(m[2]), str(m[3])});
        return true;
    });
    for (const auto& [nb, x, y] : copies) {
        for (const std::string& v : {x, y}) {   // (the pointers copied through: pointers to the block)
            const auto t2 = forms::redeclare(text, v, "struct _b" + nb + " *" + v);
            const Regex indexed(R"re(\b)re" + v + R"re(\s*(?:->|\[))re");
            if (t2 && !t2->empty() && !indexed.search(text)) text = ct::replace_all(*t2, "*(struct _b" + nb + " *)" + v, "*" + v);
        }
    }
    {
        static const Regex block(R"re(\bstruct _b(\d+) \*)re");   // (a block of bytes copied whole)
        std::set<std::int64_t> blocks;
        block.for_each_match(text, [&](const RegexMatch& m) {
            blocks.insert(to_int(m[1]));
            return true;
        });
        for (const std::int64_t nb : blocks) decl.insert(decl.begin(), std::format("struct _b{} {{ char _c[{}]; }};", nb, nb));
    }
    if (!arrs.empty()) {
        // (arrays of structs of one size whose places lie within one element: one array, its fields at the places'
        // distances from the first)
        std::vector<std::pair<std::int64_t, std::string>> sa;
        for (const std::string& d : arrs)
            if (facts.struct_arrays.contains(d)) sa.emplace_back(*ct::parse_int<std::int64_t>(sv(d).substr(2), 16), d);
        std::ranges::sort(sa);
        std::vector<std::pair<std::int64_t, std::vector<std::string>>> merged;   // (the base's place -> its arrays, in the order met)
        for (const auto& [b, d] : sa) {
            const std::uint32_t size = facts.struct_arrays.at(d).size;
            std::pair<std::int64_t, std::vector<std::string>>* base = nullptr;
            for (auto& entry : merged) {
                if (facts.struct_arrays.at(entry.second[0]).size == size && 0 < b - entry.first && b - entry.first < size) {
                    base = &entry;
                    break;
                }
            }
            if (base) base->second.push_back(d);
            else merged.push_back({b, {d}});
        }
        for (const auto& [base, ds] : merged) {
            if (ds.size() < 2) continue;
            const std::string& bn = ds[0];
            for (std::size_t i = 1; i < ds.size(); ++i) {
                const std::string& d = ds[i];
                const std::int64_t off = *ct::parse_int<std::int64_t>(sv(d).substr(2), 16) - base;
                const Regex first_field(R"re(\b)re" + d + R"re(\[([^\[\]]+)\]\.f0\b)re");
                text = first_field.replace_all(text, bn + "[$1].f" + std::to_string(off));
                arrs.erase(std::remove(arrs.begin(), arrs.end(), d), arrs.end());
            }
            std::vector<dt::StructField> elems;
            for (const std::string& d : ds) {
                const dt::StructArray& sd = facts.struct_arrays.at(d);
                const std::string type = sd.fields.empty() ? sd.first_type : sd.fields[0].type;   // (laid out before - by an earlier variant of this text: its first field's type)
                elems.push_back({static_cast<std::uint32_t>(*ct::parse_int<std::int64_t>(sv(d).substr(2), 16) - base), type.starts_with("struct") ? "int" : type});
            }
            facts.struct_arrays.at(bn).fields = elems;
        }
        for (const std::string& d : arrs) {   // (an array of structs: its element laid out - the field the code reads first, the rest of its size after)
            const auto found = facts.struct_arrays.find(d);
            if (found == facts.struct_arrays.end()) continue;
            const dt::StructArray& sa_d = found->second;
            std::vector<dt::StructField> layout = sa_d.fields;
            if (layout.empty()) layout.push_back({0, sa_d.first_type});
            std::vector<std::string> fl;
            std::int64_t pos = 0;
            for (dt::StructField field : layout) {
                if (field.offset > pos) fl.push_back(std::format("char _{}[{}];", pos, field.offset - pos));
                const Regex gone_through(R"re(\b)re" + d + R"re(\[[^\]]*\]\.f)re" + std::to_string(field.offset) + "->");
                const Regex field_struct(R"re((?m)^struct s_f)re" + std::to_string(field.offset) + R"re( \{)re");
                const Regex deref(R"re((?<![\w)\]])\*)re" + d + R"re(\[[^\]]*\]\.f)re" + std::to_string(field.offset) + R"re(\b(?!\s*(?:->|\[|\.)))re");
                if (field.type == "int" && gone_through.search(text) && field_struct.search(text)) field.type = "struct s_f" + std::to_string(field.offset) + " *";   // (a field gone through: a pointer to what it reads)
                else if (field.type == "int" && deref.search(text)) field.type = "int *";
                fl.push_back(std::format("{} f{};", field.type, field.offset));
                pos = field.offset + (field.type == "char" ? 1 : 2);
            }
            if (sa_d.size > pos) fl.push_back(std::format("char _{}[{}];", pos, sa_d.size - pos));
            decl.push_back(std::format("struct s_{} {{ {} }};", d, ct::join(fl, " ")));
        }
        for (const std::string& d : arrs) decl.push_back(std::format("extern {} {}[];", facts.arrays.at(d), d));
    }
    if (!fptr.empty()) {
        std::vector<std::string> f;
        for (const std::string& d : fptr) f.push_back("(*" + d + ")()");
        decl.push_back("extern int " + ct::join(f, ", ") + ";");
    }
    std::vector<std::string> streams;   // (a stream a library macro reads)
    for (const std::string& d : data)
        if (facts.streams.contains(d)) streams.push_back(d);
    ptrs = loc_without(ptrs, streams);
    ints = loc_without(ints, streams);
    for (const std::string& d : streams) decl.push_back("extern " + facts.streams.at(d) + " *" + d + ";");
    {
        const std::size_t brace = text.find('{');
        std::string head2 = brace == npos ? text : text.substr(0, brace);
        const std::string rest = brace == npos ? std::string() : text.substr(brace);
        for (const auto& kv : facts.streams) {   // (a parameter that is one)
            const std::string& p = kv.first;
            if (loc_in(data, p)) continue;
            const Regex plain(R"re((?m)^([ \t]*)(?:register )?(?:int|char \*|struct \w+ \*) ?)re" + re_escape(p) + ";$");
            head2 = plain.replace_all(head2, "$1" + kv.second + " *" + p + ";");
            head2 = map_lines(head2, [&](const std::string& line) {
                const auto parts = int_decl_parts(line, p);
                if (!parts) return line;
                const std::string others = parts->rest();
                return (others.empty() ? std::string() : parts->indent + "int " + others + ";\n") + parts->indent + kv.second + " *" + p + ";";
            });
        }
        text = head2 + rest;
    }
    std::vector<std::string> incs;
    for (const auto& inc : {facts.include, facts.include2})
        if (inc && !inc->empty() && !loc_in(incs, *inc)) incs.push_back(*inc);
    for (auto h = incs.rbegin(); h != incs.rend(); ++h) decl.insert(decl.begin(), "#include <" + *h + ">");
    if (!ptrs.empty()) {   // (data the draft goes through: pointers)
        std::map<std::string, std::string> sp_;
        for (const auto& [k, v] : state.struct_pointers) sp_[normal(k)] = v;
        const auto in_bytes = [&](const char* suffix) {
            return std::ranges::any_of(std::initializer_list<const char*>{"bx", "si", "di"}, [&](const char* r) { return has_site(facts.byte_sites, std::string(r) + suffix, 0); });
        };
        for (const std::string& d : ptrs) {   // (a word read through it, never a byte: an int *)
            const Regex star(R"re(\*)re" + d + R"re(\b)re");
            const Regex star_step(R"re(\*\+\+)re" + d + R"re(\b)re");
            const bool word = in_bytes("w") && !in_bytes("*") && star.search(body);
            std::string type;
            if (const auto s = sp_.find(d); s != sp_.end()) type = "struct " + s->second;
            else if (facts.word_pointers.contains(d) && star_step.search(body)) type = "char *";
            else if (word || facts.word_pointers.contains(d)) type = "int";
            else type = "char";
            decl.push_back("extern " + type + " *" + d + ";");
        }
    }
    std::vector<std::string> uch, chs, plain_ints;
    for (const std::string& d : ints) {
        if (facts.unsigned_globals.contains(d)) uch.push_back(d);
        else if (facts.byte_globals.contains(d)) chs.push_back(d);
        else plain_ints.push_back(d);
    }
    ints = plain_ints;
    if (!uch.empty()) decl.push_back("extern unsigned char " + ct::join(uch, ", ") + ";");
    if (!chs.empty()) decl.push_back("extern char " + ct::join(chs, ", ") + ";");
    std::vector<std::string> lngs;   // (a long in the data: its two words one)
    for (const std::string& d : ints)
        if (facts.longs.globals.contains(d)) lngs.push_back(d);
    ints = loc_without(ints, lngs);
    if (!lngs.empty()) decl.push_back("extern long " + ct::join(lngs, ", ") + ";");
    if (!ints.empty()) decl.push_back("extern int " + ct::join(ints, ", ") + ";");
    for (const std::string& h : incs) {   // (what an included header declares: its)
        const std::set<std::string> declared = forms::header_names(settings.kit, caches, h);
        std::vector<std::string> kept;
        for (const std::string& f : funcs)
            if (!declared.contains(f)) kept.push_back(f);
        funcs = kept;
    }
    std::vector<std::string> long_called, other_called;
    for (const std::string& f : funcs) (long_funcs.contains(f) ? long_called : other_called).push_back(f);
    if (!long_called.empty()) decl.push_back("extern long " + loc_join_names(long_called, "()") + ";");
    if (!other_called.empty()) decl.push_back("extern " + loc_join_names(other_called, "()") + ";");
    text = fp_types(text, state.fp);
    // (a byte through a char pointer compared with 128..255: the compiler compares bytes only of an unsigned char)
    static const Regex compared_high(R"re(\*\(?(?:\+\+|--)?(\w+)(?:\+\+|--)?\)? [!=]= (?:1(?:2[89]|[3-9]\d)|2[0-4]\d|25[0-5])\b)re");
    std::set<std::string> high_pointers;
    compared_high.for_each_match(text, [&](const RegexMatch& m) {
        high_pointers.insert(str(m[1]));
        return true;
    });
    for (const std::string& p : high_pointers) {
        const Regex char_pointer(R"re((?m)^([ \t]*)(register )?char \*)re" + re_escape(p) + ";$");
        text = char_pointer.replace_all(text, "$1$2unsigned char *" + p + ";");
    }
    {
        static const Regex scaled(R"re((?m)^[ \t]*(\w+) = \((\d+) \* ([^()]+?)\) \+ (\d+);$)re");
        std::vector<std::array<std::string, 4>> found;
        scaled.for_each_match(text, [&](const RegexMatch& m) {
            found.push_back({str(m[1]), str(m[2]), str(m[3]), str(m[4])});
            return true;
        });
        for (const auto& [p, k_text, e, n_text] : found) {   // (a struct pointer given K * E + N: the address of an element of an array there)
            const Regex tag_re(R"re((?m)^[ \t]*(?:register )?struct (\w+) \*)re" + p + ";$");
            const auto tag = tag_re.search(text);
            if (!tag) continue;
            const Regex struct_re(R"re((?m)^struct )re" + str((*tag)[1]) + R"re( \{([^}]*)\};$)re");
            const auto dm = struct_re.search(text);
            if (!dm || to_int(n_text) < 0x100) continue;
            const std::int64_t k = to_int(k_text), n = to_int(n_text);
            static const Regex field_re(R"re((char|int|long|\*) ?_?f?(\d+)(?:\[(\d+)\])?;)re");
            std::int64_t size = 0;
            field_re.for_each_match(dm->group(1), [&](const RegexMatch& f) {
                const std::int64_t width = f.has(3) && !f[3].empty() ? to_int(f[3]) : f[1] == "long" ? 4 : f[1] == "char" ? 1 : 2;
                size = std::max(size, to_int(f[2]) + width);
                return true;
            });
            if (size > k) continue;
            std::string body_ = str(dm->group(1)) + (size < k ? std::format(" char _{}[{}];", size, k - size) : std::string());
            while (body_.ends_with(" ") || body_.ends_with("\t") || body_.ends_with("\n")) body_.pop_back();
            const std::string name = std::format("g_{:04x}", n);
            const std::string tag_name = str((*tag)[1]);
            text = ct::replace_all(text, str(dm->whole()), "struct " + tag_name + " {" + body_ + " };\nextern struct " + tag_name + " " + name + "[];");
            const Regex assign(R"re((?m)^([ \t]*))re" + p + R"re( = \()re" + k_text + " \\* " + re_escape(e) + R"re(\) \+ )re" + n_text + ";$");
            text = assign.replace_all(text, "$1" + p + " = &" + name + "[" + e + "];");
        }
    }
    {
        static const Regex cast_read(R"re(\*\((char|int|unsigned char) \*\)(\w+)\b)re");   // (only ever read through one cast: a pointer to it)
        std::set<std::pair<std::string, std::string>> casts;
        cast_read.for_each_match(text, [&](const RegexMatch& m) {
            casts.insert({str(m[1]), str(m[2])});
            return true;
        });
        for (const auto& [ty, x] : casts) {
            static const Regex any_cast_start(R"re(\*\((?:char|int|unsigned char) \*\))re");
            const Regex others(R"re(\*\((?:char|int|unsigned char) \*\))re" + x + R"re(\b)re");
            bool all_same = true;
            others.for_each_match(text, [&](const RegexMatch& m) {
                all_same = all_same && m.whole() == "*(" + ty + " *)" + x;
                return true;
            });
            const Regex used_otherwise(R"re((?<![)\w])\*\s*)re" + x + R"re(\b|\b)re" + x + R"re(\s*(?:->|\[))re");
            if (!all_same || used_otherwise.search(text)) continue;
            const Regex int_decl(R"re((?m)^[ \t]*(?:register )?int (?:\w+, )*)re" + x + R"re(\b)re");
            if (!int_decl.search(text)) continue;
            if (const auto t2 = forms::redeclare(text, x, ty + " *" + x); t2 && !t2->empty()) text = ct::replace_all(*t2, "*(" + ty + " *)" + x, "*" + x);
        }
    }
    std::string out = (decl.empty() ? std::string() : ct::join(decl, "\n") + "\n\n") + text;
    if (!settings.data_names.empty()) {   // (data the program has names for: those, by place - a name of the form g_XXXX may be another place's)
        static const Regex place(R"re(\bg_([0-9a-f]{4})\b)re");
        out = sub_all(place, out, [&](const RegexMatch& m) {
            const auto f = settings.data_names.find(static_cast<std::uint32_t>(*ct::parse_int<std::int64_t>(m[1], 16)));
            return f == settings.data_names.end() ? str(m.whole()) : f->second;
        });
    }
    return Candidate{out, left};
}

}  // namespace cb::exact::draft::candidates
