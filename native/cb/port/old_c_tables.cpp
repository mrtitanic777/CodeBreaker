// cb/port/old_c_tables.cpp -- array_bounds, merge_tables, numbered_tables, size_tables, pool_blocks (old_c_passes.hpp):
// the passes about tables and where the old linker put them.

#include <algorithm>
#include <format>
#include <set>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_c_passes.hpp"
#include "cb/port/old_c_scan.hpp"
#include "cb/port/old_sizes.hpp"

namespace cb::port::pre_ansi {

namespace {

using base::ctext::blank;

// The structure a variable of the text points to (`struct S *v`, `S v[]`), or nothing. `code` is the text blanked.
std::optional<std::string> pointee_of(std::string_view code, std::string_view var, NamedRegex& pointer, NamedRegex& array) {
    if (const auto m = pointer.with(var).search(code)) return collapse_space(m->group(1));
    if (const auto m = array.with(var).search(code)) return collapse_space(m->group(1));
    return std::nullopt;
}

// The type words as one family: what the compilers called the same thing.
std::string same_type(std::string_view type) {
    const std::string t(text::trim(collapse_space(type)));
    static const std::map<std::string, std::string, std::less<>> family = {
        {"int16_t", "int"}, {"short", "int"}, {"uint16_t", "unsigned"}, {"unsigned int", "unsigned"}, {"int8_t", "char"}};
    const auto it = family.find(t);
    return it == family.end() ? t : it->second;
}

// The digits in a declarator's brackets (`[ 12 ]`), or nothing.
std::optional<std::uint64_t> digits_in(std::string_view text) {
    std::size_t i = 0;
    while (i < text.size() && !(text[i] >= '0' && text[i] <= '9')) ++i;
    std::size_t j = i;
    while (j < text.size() && text[j] >= '0' && text[j] <= '9') ++j;
    if (i == j) return std::nullopt;
    return text::parse_int<std::uint64_t>(text.substr(i, j - i), 10);
}

// The place of a member among a layout's members, or nothing.
std::optional<std::size_t> member_index(const StructLayout& layout, std::string_view name) {
    for (std::size_t i = 0; i < layout.members.size(); ++i)
        if (layout.members[i].name == name) return i;
    return std::nullopt;
}

// Where a table's initializer ends: the `}` that closes the `{` just before `begin`; its rows are what lies between. A
// table that never closes has none. (The Python took the next `};` there was, or with none the file's last byte: the
// rows of what came after a table written `}` and `;` apart were its rows.)
std::optional<std::size_t> initializer_end(std::string_view code, std::size_t begin) {
    int depth = 1;
    for (std::size_t i = begin; i < code.size(); ++i) {
        depth += (code[i] == '{') - (code[i] == '}');
        if (depth == 0) return i;
    }
    return std::nullopt;
}

}  // namespace

// ================================================================ array_bounds
void array_bounds(ProgramState& program) {
    static const Regex place_name(R"re(\bg_([0-9a-f]{4})\b)re");
    static const Regex array_definition(
        R"re(\s*(?:static\s+)?((?:struct|union)\s+\w+|(?:unsigned\s+)?\w+)\s+(\*?)\s*(\w+)\s*\[\s*(\d+)\s*\]\s*[;=])re");
    static const Regex extern_line(R"re(\s*extern\s+((?:unsigned\s+)?\w+)\s+(.*);)re");
    static const Regex empty_brackets(R"re((?:^|,)\s*(\w+)\s*\[\s*\])re");

    OrderedMap<std::string, std::uint64_t> ds = program.ds;   // (what the program names by its place, and what its text does)
    if (ds.empty()) return;
    std::set<std::string> named_by_place;
    for (const auto& kv : program.originals)
        place_name.for_each_match(kv.second, [&](const RegexMatch& m) {
            named_by_place.insert(std::string(m.group(1)));
            return true;
        });
    for (const std::string& nm : named_by_place) ds.try_emplace("g_" + nm, *text::parse_int<std::uint64_t>(nm, 16));

    const StructLayouts& old = program.original.layouts();
    struct Table {
        std::string base;        // the element type (`struct s`, `int`, `char *`)
        std::uint64_t count = 0;
        std::uint64_t element_size = 0;
    };
    std::map<std::string, Table> arrays;   // a global array of the program's with a place: its type, elements and element size
    for (const auto& kv : program.originals) {
        const std::vector<std::string> code = split_lines(blank(kv.second));
        const std::vector<int> depth = brace_depths(code);
        for (std::size_t i = 0; i < code.size(); ++i) {
            const std::string_view line = text::ltrim(code[i]);
            if (depth[i] || line.starts_with("extern") || line.starts_with("#") || line.starts_with("typedef")) continue;
            const auto m = array_definition.match(code[i]);
            if (!m || !ds.contains(std::string(m->group(3)))) continue;
            const std::string base = collapse_space(m->group(1));
            std::uint64_t esz = 0;
            if (!m->group(2).empty()) {
                esz = 2;
            } else if (const StructLayout* layout = old.find(base)) {
                esz = layout->size;
            } else {
                esz = old_scalar_size(base).value_or(0);
            }
            if (esz) arrays[std::string(m->group(3))] = {base + (m->group(2).empty() ? "" : " *"), *c_integer(m->group(4)), esz};
        }
    }
    std::map<std::uint64_t, std::vector<std::string>> starts;
    for (const auto& [nm, at] : ds) starts[at].push_back(nm);

    std::map<std::string, Table> tables = arrays;   // (and a table only declared, `extern int g_748e[]`: as many
    for (const auto& kv : program.originals) {      //  elements as up to the next variable)
        for (const std::string& line : split_lines(blank(kv.second))) {
            const auto m = extern_line.match(line);
            if (!m || m->group(1) == "struct" || m->group(1) == "union") continue;
            const std::string base = collapse_space(m->group(1));
            const std::uint64_t esz = old_scalar_size(base).value_or(0);
            empty_brackets.for_each_match(m->group(2), [&](const RegexMatch& e) {
                const std::string nm(e.group(1));
                if (!esz || tables.contains(nm) || !ds.contains(nm)) return true;
                const std::uint64_t at = ds.at(nm);
                std::optional<std::uint64_t> next;
                for (const auto& d : ds)
                    if (d.second > at && (!next || d.second < *next)) next = d.second;
                if (next && (*next - at) % esz == 0) tables[nm] = {base, (*next - at) / esz, esz};
                return true;
            });
        }
    }

    // a comparison of a pointer with the variable after the table, either way round
    NamedRegex compared(
        R"re((?<![\w.>])([\w.>-]+)\s*(>=|<=|<|>)\s*@1@\b(?!\s*[\[.(])|(?<![\w.>])@1@\b(?!\s*[\[.(])\s*(>=|<=|<|>)\s*([\w.>-]+))re");
    NamedRegex pointer_decl(R"re(\b((?:struct|union)\s+\w+)\s*\*\s*(?:\w+\s*,\s*\*?\s*)*@1@\b\s*[;,)=\[])re");
    NamedRegex array_decl(R"re(\b((?:struct|union)\s+\w+)\s+@1@\s*\[)re");
    NamedRegex number_decl(
        R"re(\b((?:unsigned\s+)?(?:int|short|char|long|unsigned|u?int(?:8|16|32)_t))\s*\*\s*(?:\w+\s*,\s*\*?\s*)*@1@\b\s*[;,)=])re");

    for (const auto& [a, table] : tables) {
        std::vector<std::string> after;
        if (const auto it = starts.find(ds.at(a) + table.count * table.element_size); it != starts.end())
            for (const std::string& b : it->second)
                if (b != a) after.push_back(b);
        for (const std::string& b : after) {
            const auto bt = arrays.find(b);
            if (bt != arrays.end() && bt->second.base == table.base) continue;   // (one type: the tables' join does it)
            for (const std::string& n : sorted_names(program.files)) {
                std::string t = program.files.at(n);
                const std::string code = blank(t);
                std::vector<Edit> edits;
                compared.with(b).for_each_match(code, [&](const RegexMatch& m) {
                    const bool left = m.has(1);   // (the variable is on the right of the comparison)
                    const std::string_view v = left ? m.group(1) : m.group(4);
                    std::size_t word_end = v.size();
                    while (word_end > 0 && !is_word_char(v[word_end - 1])) --word_end;
                    if (word_end == 0) return true;   // (a -> or . alone: no name)
                    std::size_t word_begin = word_end;
                    while (word_begin > 0 && is_word_char(v[word_begin - 1])) --word_begin;
                    const bool plain = std::all_of(v.begin(), v.end(), is_word_char);
                    std::optional<std::string> pointee;
                    if (plain) {
                        pointee = pointee_of(code, v.substr(word_begin, word_end - word_begin), pointer_decl, array_decl);
                        if (!pointee)   // (or a number's: int *g_0524)
                            if (const auto sm = number_decl.with(v).search(code)) pointee = std::string(sm->group(1));
                    }
                    if (pointee && same_type(*pointee) == same_type(text::replace_all(table.base, " *", ""))) {
                        const std::string_view seg = std::string_view(code).substr(m.start(), m.end() - m.start());
                        const std::size_t k = left ? seg.rfind(b) : seg.find(b);
                        edits.push_back({m.start() + k, m.start() + k + b.size(), std::format("({} + {})", a, table.count)});
                    }
                    return true;
                });
                if (edits.empty()) continue;
                t = apply_edits(std::move(t), std::move(edits));
                if (find_word(code, a) == std::string::npos)   // (the table named here: declared)
                    program.extra_decls[n].insert(std::format("extern {}{}{}[{}];", text::replace_all(table.base, " *", ""),
                                                              table.base.ends_with('*') ? " *" : " ", a, table.count));
                program.files.insert_or_assign(n, std::move(t));
                note(program.notes, "a pointer into a table compared with the variable the old linker put after it: the table's end",
                     std::format("{} ({}: {} + {})", n, b, a, table.count));
            }
        }
    }
}

// ================================================================ merge_tables
namespace {

// A one-name definition at a file's top: `int t[3] = { 1, 2 };`, `char x;`
struct Definition1 {
    std::string base, name;
    std::optional<std::string> array;      // `[3]`, `[]`
    std::optional<std::string> value;      // what follows '=' (without the braces for an array)
    std::string rest;
};

const Regex& one_name_definition() {
    static const Regex re(
        R"re(^((?:unsigned\s+|signed\s+)?(?:char|short|int|long|unsigned|intptr_t))\s+(\w+)\s*(\[\s*\d*\s*\])?\s*(?:=\s*(\{[^;{}]*\}|[^;{}]*?)\s*)?;(.*)$)re");
    return re;
}

std::optional<Definition1> read_definition(std::string_view line) {
    const auto m = one_name_definition().match(line);
    if (!m) return std::nullopt;
    Definition1 d;
    d.base = m->group(1);
    d.name = m->group(2);
    if (m->has(3)) d.array = std::string(m->group(3));
    if (m->has(4)) d.value = std::string(m->group(4));
    d.rest = m->group(5);
    return d;
}

// An array without an initializer: its size in zeros (a scalar: one); nothing for `[]` and for a definition with a value.
std::optional<std::vector<std::string>> zero_elements(const Definition1& d) {
    if (d.value) return std::nullopt;
    const auto size = d.array ? digits_in(*d.array) : std::nullopt;
    if (!size && d.array) return std::nullopt;
    return std::vector<std::string>(size ? static_cast<std::size_t>(*size) : 1, "0");
}

std::vector<std::string> split_values(std::string_view value) {
    std::vector<std::string> out;
    for (const std::string_view v : text::split(text::trim(value, "{} "), ',')) {
        const std::string_view piece = text::trim(v);
        if (!piece.empty()) out.emplace_back(piece);
    }
    return out;
}

void pad_with_zeros(std::vector<std::string>& values, const std::optional<std::string>& array) {
    if (const auto size = array ? digits_in(*array) : std::nullopt)
        if (values.size() < *size) values.resize(static_cast<std::size_t>(*size), "0");
}

}  // namespace

void merge_tables(ProgramState& program) {
    static const Regex subscripted(R"re(\b([A-Za-z_]\w*)\s*\[\s*(\d+)\s*\])re");
    std::map<std::string, std::int64_t> uses;   // table -> the largest constant place it is indexed at
    for (const auto& kv : program.files)
        subscripted.for_each_match(blank(kv.second), [&](const RegexMatch& m) {
            const std::int64_t place = static_cast<std::int64_t>(std::min<std::uint64_t>(*c_integer(m.group(2)), 1ull << 40));
            auto [it, fresh] = uses.try_emplace(std::string(m.group(1)), place);
            if (!fresh) it->second = std::max(it->second, place);
            return true;
        });
    for (const auto& [name, elements] : program.reach) {   // (and where a run read one past its end: sanitize)
        auto [it, fresh] = uses.try_emplace(name, static_cast<std::int64_t>(elements));
        if (!fresh) it->second = std::max(it->second, static_cast<std::int64_t>(elements));
    }
    auto used_at = [&](const std::string& name) {
        const auto it = uses.find(name);
        return it == uses.end() ? std::int64_t{-1} : it->second;
    };

    struct Part {
        std::string name;
        std::size_t first_element, count;
        bool is_array;
    };
    for (const std::string& n : sorted_names(program.files)) {
        if (!n.ends_with(".c")) continue;
        std::vector<std::string> lines = split_lines(program.files.at(n));
        std::size_t i = 0;
        while (i < lines.size()) {
            const std::optional<Definition1> d = read_definition(lines[i]);
            std::optional<std::vector<std::string>> values;
            if (d && d->array && !d->value) {
                values = zero_elements(*d);
            } else if (d && d->array) {
                values = split_values(*d->value);   // (int t[2691] = { 0 }: its 2691, the rest 0)
                const bool is_string = text::ltrim(*d->value).starts_with('"');
                if (!is_string) pad_with_zeros(*values, d->array);
                if (is_string) {   // (a string: its bytes, and the 0 after)
                    values.reset();
                    const std::string literal(text::trim(*d->value));
                    const auto given = digits_in(*d->array);
                    const std::uint64_t size = given ? *given : c_string_bytes(literal).size() + 1;
                    if (used_at(d->name) >= static_cast<std::int64_t>(size))
                        note(program.notes, "a string read past its end at a constant place (not joined: check)",
                             std::format("{} ({})", n, d->name));
                }
            }
            if (!d || !values || values->empty() || used_at(d->name) < static_cast<std::int64_t>(values->size())) {
                ++i;
                continue;
            }
            std::vector<std::string>& vals = *values;
            std::vector<Part> parts = {{d->name, 0, vals.size(), true}};
            std::size_t j = i + 1;
            while (j < lines.size()) {   // (what follows, of the same type: the table's rest)
                const std::optional<Definition1> d2 = read_definition(lines[j]);
                if (!d2 || d2->base != d->base) break;
                std::vector<std::string> v2;
                if (!d2->value) {
                    auto zeros = zero_elements(*d2);
                    if (!zeros) break;
                    v2 = std::move(*zeros);
                } else if (d2->array && text::ltrim(*d2->value).starts_with('"')) {   // (a string among them: its bytes as elements)
                    const Bytes bytes = c_string_bytes(text::trim(*d2->value));
                    const std::size_t size = static_cast<std::size_t>(digits_in(*d2->array).value_or(bytes.size() + 1));
                    for (const std::uint8_t b : bytes) v2.push_back(std::to_string(b));
                    if (size > v2.size()) v2.resize(size, "0");
                } else {
                    v2 = d2->array ? split_values(*d2->value) : std::vector<std::string>{std::string(text::trim(*d2->value))};
                    pad_with_zeros(v2, d2->array);
                }
                parts.push_back({d2->name, vals.size(), v2.size(), d2->array.has_value()});
                vals.insert(vals.end(), v2.begin(), v2.end());
                ++j;
            }
            if (parts.size() == 1) {
                ++i;
                continue;
            }
            std::vector<std::string> names;
            for (const Part& p : parts) names.push_back(p.name);
            const bool any_value = std::any_of(vals.begin(), vals.end(), [](const std::string& v) { return v != "0"; });
            const std::string joined_names = text::join(names, ", ");
            const std::string merged = any_value
                ? std::format("{} {}[{}] = {{ {} }};\t/* (one table: {}) */", d->base, d->name, vals.size(), text::join(vals, ", "), joined_names)
                : std::format("{} {}[{}];\t/* (one table: {}) */", d->base, d->name, vals.size(), joined_names);   // (none given a value: one block of them)
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(i), lines.begin() + static_cast<std::ptrdiff_t>(j));
            lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(i), merged);
            for (std::size_t k = 1; k < parts.size(); ++k)
                program.aliases.insert_or_assign(
                    parts[k].name,
                    Alias{parts[k].is_array ? std::format("(&{}[{}])", d->name, parts[k].first_element)
                                            : std::format("{}[{}]", d->name, parts[k].first_element),
                          std::format("{} {}[{}];", d->base, d->name, vals.size())});
            names.erase(names.begin());
            program.lists.tables.push_back(std::format("{} ({}: {})", n, d->name, text::join(names, ", ")));
            ++i;
        }
        program.files.insert_or_assign(n, join_lines(lines));
    }
}

// ================================================================ numbered_tables
void numbered_tables(ProgramState& program) {
    static const Regex table_decl(
        R"re((?m)^\s*(?:extern\s+|static\s+)?((?:unsigned\s+)?(?:int|short|unsigned|char))\s+(g_[0-9a-f]{4})\s*\[\s*(\d*)\s*\]\s*[;=,])re");
    static const Regex hex_place(R"re(\b0x([0-9a-fA-F]{4})\b)re");
    static const Regex place_argument(
        R"re(\s*(?:[^?:]*\?\s*)?0x[0-9a-fA-F]{4}\s*(?::\s*0x[0-9a-fA-F]{4}\s*)?)re");
    static const Regex ptr_to_ptr(R"re(\*\s*\*)re");
    static const Regex last_in_choice(R"re((?:^|[?:]\s*)(g_[0-9a-f]{4})\s*$)re");
    static const Regex first_in_choice(R"re(^\s*(g_[0-9a-f]{4})\b\s*(?::|$))re");
    static const Regex cast_to_ptr_ptr(R"re(\(\s*(?:(?:struct|union)\s+\w+|\w+)\s*\*\s*\*\s*\)\s*\(?\s*)re");
    // what may follow such a cast: a table, or a choice (`c ? a : b`) between tables
    static const Regex places[] = {Regex(R"re((g_[0-9a-f]{4})\b)re"), Regex(R"re([^()?:]*\?\s*(g_[0-9a-f]{4})\b)re"),
                                   Regex(R"re(\w+\s*:\s*(g_[0-9a-f]{4})\b)re"),
                                   Regex(R"re([^()?:]*\?\s*\w+\s*:\s*(g_[0-9a-f]{4})\b)re")};

    Files& files = program.files;
    std::map<std::string, std::pair<std::string, std::string>> decls;   // g_3a20: ("int", "64"), as declared somewhere
    for (const auto& kv : program.originals)
        table_decl.for_each_match(blank(kv.second), [&](const RegexMatch& m) {
            const std::string name(m.group(2));
            if (!m.group(3).empty() || !decls.contains(name)) decls[name] = {std::string(m.group(1)), std::string(m.group(3))};
            return true;
        });
    if (decls.empty()) return;

    // the definitions' parameters (the program's functions are not made yet), in the order the files have them
    OrderedMap<std::string, Fn> defs;
    std::map<std::string, std::string> bodies;
    for (const auto& [n, t] : files) {
        if (!n.ends_with(".c")) continue;
        const std::vector<std::string> code = split_lines(blank(t));
        for (const Definition& d : definitions(split_lines(t), code)) {
            defs.try_emplace(d.fn.name, d.fn);
            bodies.try_emplace(d.fn.name, join_range(code, d.open, d.last));
        }
    }
    std::map<std::string, std::vector<bool>> pointer_params;   // function -> for each parameter, whether it is a pointer
    for (const auto& [name, f] : defs) {
        std::vector<bool>& flags = pointer_params[name];
        for (const Param& p : f.params) flags.push_back(p.type.find('*') != std::string::npos);
    }
    auto args_of = [](const std::string& text, std::size_t at, std::string_view fn, std::size_t reach) {
        std::vector<Span> spans = call_args(std::string_view(text).substr(at, reach), fn).value_or(std::vector<Span>{});
        return spans;
    };
    for (int round = 0; round < 3; ++round) {   // (f067e8(num, tab) { return f067b5(num, tab); } -- a parameter given on
        for (const auto& [name, f] : defs) {    //  to a pointer one: a pointer too)
            const std::string& body = bodies[name];
            for (const CallSite& site : call_sites(body, [](std::string_view) { return true; })) {
                const std::string callee(site.name);
                if (!pointer_params.contains(callee) || callee == name) continue;
                const std::vector<Span> spans = args_of(body, site.at, callee, 400);
                for (std::size_t k = 0; k < spans.size(); ++k) {
                    const std::string arg(text::trim(std::string_view(body).substr(site.at + spans[k].begin, spans[k].size())));
                    if (k < pointer_params[callee].size() && pointer_params[callee][k])
                        for (std::size_t i = 0; i < f.params.size(); ++i)
                            if (f.params[i].name == arg) pointer_params[name][i] = true;
                }
            }
        }
    }

    // the places given as numbers, where a table's number is wanted
    NamedRegex declared(R"re(\b@1@\b\s*\[[^\]]*\]\s*[;,=])re");
    for (const std::string& n : file_names(files)) {
        if (!n.ends_with(".c")) continue;
        std::string t = files.at(n);
        const std::string code = blank(t);
        std::vector<Edit> edits;
        std::set<std::string> used;
        for (const CallSite& site : call_sites(code, [&](std::string_view w) { return pointer_params.contains(std::string(w)); })) {
            const std::string callee(site.name);
            const std::vector<bool>& flags = pointer_params[callee];
            const std::vector<Span> spans = args_of(code, site.at, callee, 600);
            for (std::size_t k = 0; k < spans.size(); ++k) {
                if (k >= flags.size() || !flags[k]) continue;
                const std::string_view arg = std::string_view(code).substr(site.at + spans[k].begin, spans[k].size());
                hex_place.for_each_match(arg, [&](const RegexMatch& h) {
                    const std::string name = "g_" + text::lower(h.group(1));
                    if (decls.contains(name) && place_argument.full_match(arg)) {
                        const std::size_t at = site.at + spans[k].begin + h.start();
                        edits.push_back({at, at + h.whole().size(), name});
                        used.insert(name);
                    }
                    return true;
                });
            }
        }
        if (edits.empty()) continue;
        t = apply_edits(std::move(t), std::move(edits), /*unique=*/true);
        const std::string code_now = blank(t);
        for (const std::string& name : used)
            if (!declared.with(name).test(code_now))
                program.extra_decls[n].insert(std::format("extern {} {}[{}];", decls[name].first, name, decls[name].second));
        note(program.notes, "a table's place given as its number (the old data segment's): the table, by name",
             std::format("{} ({})", n, text::join(std::vector<std::string>(used.begin(), used.end()), ", ")));
        files.insert_or_assign(n, std::move(t));
    }

    // an int table where a table of pointers is wanted: cast to a pointer to pointers, or given to a parameter that is one
    std::set<std::string> wide;
    for (const auto& [n, t] : files) {
        const std::string code = blank(t);
        std::set<std::string> present;
        for (const auto& kv : decls)
            if (find_word(code, kv.first) != std::string::npos) present.insert(kv.first);
        if (present.empty()) continue;
        cast_to_ptr_ptr.for_each_match(code, [&](const RegexMatch& cast) {   // (a cast to a pointer to pointers of a table, or of a choice of two)
            const std::string_view rest = std::string_view(code).substr(cast.end());
            for (const Regex& form : places)
                if (const auto m = form.match(rest))
                    if (present.contains(std::string(m->group(1)))) wide.insert(std::string(m->group(1)));
            return true;
        });
        for (const CallSite& site : call_sites(code, [&](std::string_view w) { return defs.contains(std::string(w)); })) {
            const Fn& f = defs.at(std::string(site.name));
            const std::vector<Span> spans = args_of(code, site.at, site.name, 600);
            for (std::size_t k = 0; k < spans.size() && k < f.params.size(); ++k) {
                if (!ptr_to_ptr.test(f.params[k].type)) continue;
                const std::string_view arg = text::trim(std::string_view(code).substr(site.at + spans[k].begin, spans[k].size()));
                if (const auto m = last_in_choice.search(arg); m && present.contains(std::string(m->group(1)))) wide.insert(std::string(m->group(1)));
                if (const auto m = first_in_choice.search(arg); m && present.contains(std::string(m->group(1)))) wide.insert(std::string(m->group(1)));
            }
        }
    }
    for (const std::string& name : wide) {
        const std::string& type = decls.at(name).first;
        if (type != "int" && type != "short" && type != "unsigned" && type != "unsigned int") continue;
        program.decl_rules.emplace_back(R"re(\b(?:unsigned\s+int|unsigned|short|u?int16_t|int)(\s+)re" + name + R"re(\s*\[))re", "intptr_t${1}");
        for (auto& [file, declarations] : program.extra_decls) {
            std::set<std::string> rewritten;
            for (const std::string& d : declarations)
                rewritten.insert(text::replace_all(d, type + " " + name + "[", "intptr_t " + name + "["));
            declarations = std::move(rewritten);
        }
        note(program.notes, "an int table holding pointers (given where a table of them is wanted): intptr_t", name);
    }
}

// ================================================================ size_tables
void size_tables(ProgramState& program) {
    static const Regex struct_pointer_assign(R"re(\b(\w+)\s*=\s*(\w+)\s*\[[^\]]*\]\s*\.\s*(\w+)\s*;)re");
    static const Regex alloc_call(R"re(\b(\w+)\s*=\s*(?:\([^()]*\)\s*)?(\w+)\s*\()re");
    static const Regex table_field(R"re((\w+)\s*\[[^\]]*\]\s*\.\s*(\w+))re");
    static const Regex row(R"re(\{([^{}]*)\})re");

    Files& files = program.files;
    const StructLayouts& old = program.original.layouts();
    const Allocators& allocs = program.original.allocators(files);
    std::map<std::pair<std::string, std::string>, std::set<std::string>> uses;   // (table, field): the structures it gives sizes of
    for (const auto& [n, t] : files) {
        if (!n.ends_with(".c")) continue;
        const std::string code = blank(t);
        const auto ptr_of = pointer_targets(code);
        std::map<std::string, std::pair<std::string, std::string>> through;   // n = g_2096[s->kind].n;
        struct_pointer_assign.for_each_match(code, [&](const RegexMatch& m) {
            through[std::string(m.group(1))] = {std::string(m.group(2)), std::string(m.group(3))};
            return true;
        });
        alloc_call.for_each_match(code, [&](const RegexMatch& m) {
            const auto st = ptr_of.find(m.group(1));
            const auto* at = allocs.find(std::string(m.group(2)));
            if (st == ptr_of.end() || !old.contains(st->second) || !at || at->size() != 1) return true;
            const std::vector<Span> sp = call_args(std::string_view(code).substr(m.start(2), 400), m.group(2)).value_or(std::vector<Span>{});
            const std::size_t k = *at->begin();
            if (k >= sp.size()) return true;
            const std::string a(text::trim(std::string_view(code).substr(m.start(2) + sp[k].begin, sp[k].size())));
            if (const auto d = table_field.full_match(a)) {
                uses[{std::string(d->group(1)), std::string(d->group(2))}].insert(st->second);
            } else if (std::all_of(a.begin(), a.end(), is_word_char) && !a.empty()) {
                if (const auto via = through.find(a); via != through.end()) uses[via->second].insert(st->second);
            }
            return true;
        });
    }

    NamedRegex table_definition(R"re((?m)^(?:static\s+)?(struct\s+\w+)\s+@1@\s*\[[^\]]*\]\s*=\s*\{)re");
    NamedRegex extern_table(R"re(\bextern\s+((?:struct|union)\s+\w+)\s+@1@\s*\[)re");
    for (const auto& [key, sts] : uses) {
        const auto& [tab, field] = key;
        if (sts.size() != 1) continue;
        const std::string& st = *sts.begin();
        const StructLayout& lay = old.at(st);
        std::map<std::uint32_t, std::string> offs;   // the member that begins at each old offset (the last, in a union)
        for (const Member& m : lay.members) offs[m.offset] = m.name;
        for (const std::string& n : file_names(files)) {
            std::string t = files.at(n);
            const std::string code = blank(t);
            const auto dm = table_definition.with(tab).search(code);
            if (!dm) continue;
            const StructLayout* tl = old.find(collapse_space(dm->group(1)));
            if (!tl) continue;
            std::size_t k = 0;
            if (const auto at = member_index(*tl, field)) {
                k = *at;
            } else {   // (struct okind g_1010[] defined, extern struct recfmt g_1010[] used: the place by order)
                std::optional<std::string> used_tag;
                for (const auto& other : files) {
                    const std::string other_code = blank(other.second);   // (the match's views are into it)
                    if (const auto x = extern_table.with(tab).search(other_code)) {
                        used_tag = collapse_space(x->group(1));
                        break;
                    }
                }
                const StructLayout* ul = used_tag ? old.find(*used_tag) : nullptr;
                const auto at_used = ul ? member_index(*ul, field) : std::nullopt;
                if (!at_used) continue;
                k = *at_used;
                if (k >= tl->members.size()) continue;
            }
            const std::size_t body_begin = dm->end();
            const auto body_end = initializer_end(code, body_begin);
            if (!body_end) continue;
            std::vector<Edit> edits;
            row.for_each_match(std::string_view(code).substr(body_begin, *body_end - body_begin), [&](const RegexMatch& e) {
                const std::vector<std::string_view> parts = split_top(e.group(1));
                if (k >= parts.size()) return true;
                const std::string_view v = text::trim(parts[k]);
                const auto nv = c_integer(v);
                if (!nv || *nv == 0) return true;
                std::string replacement;
                if (*nv == lay.size) {
                    replacement = std::format("sizeof({})", st);
                } else if (const auto it = *nv <= UINT32_MAX ? offs.find(static_cast<std::uint32_t>(*nv)) : offs.end(); it != offs.end()) {
                    replacement = std::format("offsetof({}, {})", st, it->second);
                } else {
                    return true;
                }
                std::size_t at = body_begin + e.start(1);
                for (std::size_t p = 0; p < k; ++p) at += parts[p].size() + 1;
                const std::size_t lead = parts[k].size() - text::ltrim(parts[k]).size();
                edits.push_back({at + lead, at + lead + v.size(), replacement});
                return true;
            });
            if (edits.empty()) continue;
            t = apply_edits(std::move(t), std::move(edits));
            if (t.find("<stddef.h>") == std::string::npos) t = "#include <stddef.h>\n" + t;
            files.insert_or_assign(n, std::move(t));
            program.lists.size_tables.push_back(std::format("{} ({}[].{}: {})", n, tab, field, st));
        }
    }
}

// ================================================================ pool_blocks
void pool_blocks(ProgramState& program) {
    static const Regex malloc_of_member(R"re(\bmalloc\s*\(\s*(\w+)\s*->\s*(\w+))re");
    static const Regex row(R"re(\{([^{}]*)\})re");
    static const Regex place_name(R"re(\b(g_[0-9a-f]{4})\b)re");

    Files& files = program.files;
    const StructLayouts& old = program.original.layouts();
    const std::uint64_t ratio = 8 / std::max(1u, program.pointer_bytes);
    NamedRegex size_kept(R"re(\b@1@\s*->\s*(\w+)\s*=\s*@1@\s*->\s*@2@\b)re");
    NamedRegex pool_pointer(R"re(\b(struct\s+\w+)\s*\*\s*@1@\s*[;,)])re");
    NamedRegex compared(R"re(>\s*@1@\s*->\s*@2@\b)re");

    OrderedMap<std::string, std::pair<std::string, std::string>> found;   // struct pool: (its block size member, what is left)
    for (const auto& [n, t] : files) {
        if (!n.ends_with(".c")) continue;
        const std::vector<std::string> code = split_lines(blank(t));
        for (const Definition& d : definitions(split_lines(t), code)) {
            const std::string body = join_range(code, d.open, d.last);
            const std::string whole = join_range(code, d.first, d.last);
            malloc_of_member.for_each_match(body, [&](const RegexMatch& m) {
                const std::string_view pool = m.group(1), size_member = m.group(2);
                const auto kept = size_kept.with(pool, size_member).search(body);
                const auto st = pool_pointer.with(pool).search(whole);
                if (kept && st && compared.with(pool, kept->group(1)).test(body))
                    found.insert_or_assign(collapse_space(st->group(1)), std::pair<std::string, std::string>{std::string(size_member), std::string(kept->group(1))});
                return true;
            });
        }
    }
    for (const auto& [st, members] : found) {
        const auto& [size_member, left_member] = members;
        const StructLayout* lay = old.find(st);
        if (!lay) continue;
        const auto ia = member_index(*lay, size_member), ib = member_index(*lay, left_member);
        if (!ia || !ib) continue;
        // the table of pools: `struct pool` with any white space between its words
        const Regex table(R"re((?m)^(?:static\s+)?)re" + regex_quote(st, /*spaces_flex=*/true) + R"re(\s+\w+\s*\[[^\]]*\]\s*=\s*\{)re");
        for (const std::string& n : file_names(files)) {
            std::string t = files.at(n);
            const std::string code = blank(t);
            const auto dm = table.search(code);
            if (!dm) continue;
            const std::size_t body_begin = dm->end();
            const auto body_end = initializer_end(code, body_begin);
            if (!body_end) continue;
            std::vector<Edit> edits;
            row.for_each_match(std::string_view(code).substr(body_begin, *body_end - body_begin), [&](const RegexMatch& e) {
                const std::vector<std::string_view> parts = split_top(e.group(1));
                const std::string_view first_block = *ia < parts.size() ? text::trim(parts[*ia]) : std::string_view();
                if (const auto first = c_integer(first_block)) {
                    // (its first block, a BSS array of the old size: as big as the blocks now, a head's room more)
                    std::set<std::string> places;
                    place_name.for_each_match(e.group(1), [&](const RegexMatch& g) {
                        places.insert(std::string(g.group(1)));
                        return true;
                    });
                    for (const std::string& g : places)
                        if (const auto it = program.bss.find(g); it != program.bss.end()) it->second = std::max(it->second, *first * ratio + 16);
                }
                for (const std::size_t index : {*ia, *ib}) {
                    if (index >= parts.size()) continue;
                    const std::string_view v = text::trim(parts[index]);
                    const auto value = c_integer(v);
                    if (!value) continue;
                    std::size_t at = body_begin + e.start(1);
                    for (std::size_t p = 0; p < index; ++p) at += parts[p].size() + 1;
                    const std::size_t lead = parts[index].size() - text::ltrim(parts[index]).size();
                    edits.push_back({at + lead, at + lead + v.size(), std::to_string(*value * ratio)});
                }
                return true;
            });
            if (edits.empty()) continue;
            files.insert_or_assign(n, apply_edits(std::move(t), std::move(edits)));
            program.lists.pools.push_back(std::format("{} ({}: {} and {} x{})", n, st, size_member, left_member, ratio));
        }
    }
}

}  // namespace cb::port::pre_ansi
