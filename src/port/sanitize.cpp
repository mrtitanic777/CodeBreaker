// cb-unity: alone (helpers named for what they do here)
#include "port/sanitize.hpp"

#include <algorithm>
#include <cmath>
#include <format>

#include "base/ctext.hpp"
#include "core/fs.hpp"
#include "core/intmath.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"

namespace cb::port::sanitize {

namespace pa = cb::port::pre_ansi;
using pa::collapse_space;
using pa::regex_quote;
using pa::rule_enabled;
using pa::split_lines;

namespace {

// ---- the sanitizer's words ------------------------------------------------------------------------------------------------

constexpr std::string_view kNoteArrayPast =
    "array reached past its end (as the original did, harmlessly there): room made for it";
constexpr std::string_view kNoteBeforeTable =
    "a table read before its start (as the original did: its linker's variable there): that variable read";
constexpr std::string_view kNoteReadIntoNext =
    "a table read past its end into the variables after it (a run showed it): one table";
constexpr std::string_view kNoteBssJoined =
    "a BSS array gone past into the one after it (the old layout's): one array, their names places in it";
constexpr std::string_view kNoteSmallWord =
    "a table's word read as a pointer, a small number at times (DOS read the null segment there): dos_np";
constexpr std::string_view kNoteNullRead = "a pointer read through when NULL (DOS read the null segment there): dos_np";

// An ASan "global-buffer-overflow ... READ|WRITE of size N ... M bytes after global variable 'v' ... of size S".
const Regex& past_end_report() {
    static const Regex re(R"(AddressSanitizer: global-buffer-overflow.*?\n(?<rw>READ|WRITE) of size (?<sz>\d+).*?)"
                          R"(is located (?<after>\d+) bytes after global variable '(?<var>\w+)'.*?of size (?<size>\d+))",
                          RegexFlags::DotAll);
    return re;
}
// ... a READ whose first frame says where (file:line).
const Regex& before_start_report() {
    static const Regex re(R"(AddressSanitizer: global-buffer-overflow.*?\n(?<rw>READ) of size (?<sz>\d+).*?\n\s*#0 0x[0-9a-fA-F]+ )"
                          R"(in \w+ (?<path>[^\n]*?):(?<line>\d+))",
                          RegexFlags::DotAll);
    return re;
}
const Regex& before_variable() {
    static const Regex re(R"(is located (?<before>\d+) bytes before global variable '(?<var>\w+)'.*?of size (?<size>\d+))",
                          RegexFlags::DotAll);
    return re;
}
// The bounds check's report: file:line:col: runtime error: index -N out of bounds for type '...'.
const Regex& bounds_report() {
    static const Regex re(R"((?<path>[^\s:]+(?::[\\/][^\s:]+)?):(?<line>\d+):(?<col>\d+): runtime error: index (?<idx>-\d+) )"
                          R"(out of bounds for type '(?<type>[^']*)')");
    return re;
}
// A fault at a small address, with the first frame that names a file.
const Regex& small_address_report() {
    static const Regex re(R"(access-violation on unknown address 0x0*(?<addr>[0-9a-fA-F]{1,2})\b.*?\n(?:.*\n)*?\s*#\d+ )"
                          R"(0x[0-9a-fA-F]+ in (?<fn>\w+) (?<path>[^\n]*?):(?<line>\d+))");
    return re;
}

std::optional<RegexMatch> find_in(const Regex& re, std::string_view text) {
    // a report the step budget cannot get through is no report
    auto found = re.try_search(text);
    if (!found.ok()) return std::nullopt;
    return found.value();
}

std::uint64_t number_of(std::string_view digits) { return text::parse_int<std::uint64_t>(digits, 10).value_or(0); }
std::int64_t signed_number_of(std::string_view digits) { return text::parse_int<std::int64_t>(digits, 10).value_or(0); }

std::string base_name(std::string_view path) {
    const std::size_t at = path.find_last_of("\\/");
    return std::string(at == std::string_view::npos ? path : path.substr(at + 1));
}

// The 1-based line of a file's text; nullopt when the file has no such line.
std::optional<std::string> line_of(const Files& texts, const std::string& file, std::int64_t number) {
    const std::string* t = texts.find(file);
    if (t == nullptr || number < 1) return std::nullopt;
    const std::vector<std::string> lines = split_lines(*t);
    if (static_cast<std::size_t>(number) > lines.size()) return std::nullopt;
    return lines[static_cast<std::size_t>(number) - 1];
}

// A regex made from a name found in the program (the variable the sanitizer named): one per call, not per loop turn.
Regex named(const std::string& pattern_with_name_slot, std::string_view name, RegexFlags flags = RegexFlags::None) {
    std::string pattern = pattern_with_name_slot;
    const std::size_t at = pattern.find("@NAME@");
    pattern.replace(at, 6, regex_quote(name));
    return Regex(pattern, flags);
}

std::string flex_quote(std::string_view s) { return regex_quote(s, true); }   // `struct  s` and `struct s` are one

// ---- rules of the port ----------------------------------------------------------------------------------------------------

bool has_null_read(const Target& t, const std::string& file, const std::string& var) {
    return std::any_of(t.null_reads.begin(), t.null_reads.end(),
                       [&](const pa::NullRead& r) { return r.file == file && r.var == var; });
}
bool has_null_read_of_kind(const Target& t, const std::string& file, const std::string& var, pa::NullRead::Kind kind) {
    return std::any_of(t.null_reads.begin(), t.null_reads.end(), [&](const pa::NullRead& r) {
        return r.file == file && r.var == var && r.kind == kind;
    });
}
void add_null_read(Target& t, std::string file, std::string var, std::string type, pa::NullRead::Kind kind) {
    t.null_reads.push_back({std::move(file), std::move(var), std::move(type), kind});
}

}  // namespace

// ---- c_layout --------------------------------------------------------------------------------------------------------------

namespace {

struct ScalarSize {
    std::uint64_t size, align;
};

// The sizes of today's C types as the port writes them (x64).
std::optional<ScalarSize> scalar_size(std::string_view type) {
    static const std::map<std::string, ScalarSize, std::less<>> kSizes = {
        {"char", {1, 1}},     {"signed char", {1, 1}}, {"unsigned char", {1, 1}}, {"int16_t", {2, 2}}, {"uint16_t", {2, 2}},
        {"short", {2, 2}},    {"int32_t", {4, 4}},     {"uint32_t", {4, 4}},      {"int", {4, 4}},      {"unsigned", {4, 4}},
        {"long", {4, 4}},     {"float", {4, 4}},       {"double", {8, 8}},        {"intptr_t", {8, 8}}, {"int64_t", {8, 8}}};
    const auto it = kSizes.find(type);
    if (it == kSizes.end()) return std::nullopt;
    return it->second;
}

}  // namespace

std::optional<CLayout> c_layout(std::string_view body) {
    static const Regex qualifiers(R"(\b(?:register|const|volatile|signed)\b(?= \w))");
    static const Regex dimension(R"(\[\s*(\d+)\s*\])");
    CLayout layout;
    std::uint64_t offset = 0, align = 1;
    const std::string code = base::ctext::blank(body);   // (the pieces are views into it)
    for (const std::string_view piece : text::split(code, ';')) {
        const std::string_view declaration = text::trim(piece);
        if (declaration.empty()) continue;
        const std::optional<pa::Declaration> parsed = pa::declarators(declaration);
        if (!parsed) continue;
        const std::string base = std::string(text::trim(collapse_space(qualifiers.replace_all(parsed->base, ""))));
        for (const pa::Declarator& d : parsed->names) {
            std::uint64_t count = 1;
            dimension.for_each_match(d.text, [&](const RegexMatch& m) {
                count *= number_of(m[1]);
                return true;
            });
            std::string without_name = d.text;
            if (const std::size_t at = without_name.find(d.name); at != std::string::npos) without_name.erase(at, d.name.size());
            std::uint64_t size = 0, alignment = 0;
            if (without_name.find('*') != std::string::npos) {
                size = alignment = 8;
            } else if (const std::optional<ScalarSize> s = scalar_size(base)) {
                size = s->size;
                alignment = s->align;
            } else {
                return std::nullopt;
            }
            offset = (offset + alignment - 1) / alignment * alignment;
            layout.members[d.name] = {offset, size * count};
            offset += size * count;
            align = std::max(align, alignment);
        }
    }
    layout.size = (offset + align - 1) / align * align;
    return layout;
}

// ---- before_start ----------------------------------------------------------------------------------------------------------

namespace {

// `var[<index>].<field>` in a line: the first such place (the index has no brackets of its own).
struct IndexedMember {
    std::size_t start = 0, end = 0;
    std::string whole, index, field;
};

std::optional<IndexedMember> find_indexed_member(std::string_view line, const std::string& var) {
    const Regex re = named(R"(\b@NAME@\s*\[([^\[\]]+)\]\s*\.\s*(\w+))", var);
    const std::optional<RegexMatch> m = re.search(line);
    if (!m) return std::nullopt;
    return IndexedMember{m->start(), m->end(), std::string(m->whole()), std::string(m->group(1)), std::string(m->group(2))};
}

// Has the index text an assignment or an increment in it (a read that does more than read is left alone)?
bool index_has_side_effect(std::string_view index) {
    static const Regex re(R"(\+\+|--|[^=!<>]=[^=])");
    return re.test(index);
}

struct TableDecl {
    std::string type;   // `struct row`
};

// The structure type of the table `var` (defined with rows, or only declared) in any of the files.
std::optional<TableDecl> table_decl(const Files& out, const std::string& var) {
    const Regex defined = named(R"(\b(struct\s+\w+)\s+@NAME@\s*\[[^\]]*\]\s*=\s*\{)", var);
    for (const auto& [name, t] : out) {
        if (const auto d = defined.search(base::ctext::blank(t))) return TableDecl{std::string(d->group(1))};
    }
    const Regex declared = named(R"(\b(struct\s+\w+)\s+@NAME@\s*\[)", var);
    for (const auto& [name, t] : out) {
        if (const auto d = declared.search(base::ctext::blank(t))) return TableDecl{std::string(d->group(1))};
    }
    return std::nullopt;
}

// The text of the body of `struct tag { ... }` in any file.
std::optional<std::string> struct_body(const Files& out, std::string_view type) {
    const Regex body_re(std::string(R"(\b)") + flex_quote(type) + R"(\s*\{([^{}]*)\})");
    for (const auto& [name, t] : out) {
        if (const auto b = body_re.search(base::ctext::blank(t))) return t.substr(b->start(1), b->end(1) - b->start(1));
    }
    return std::nullopt;
}

}  // namespace

std::optional<std::string> before_start(Target& target, std::string_view case_name, std::string_view text) {
    std::string var;
    std::string file;
    std::string line;
    std::size_t line_number = 0;
    std::optional<std::int64_t> given_index;
    std::uint64_t before = 0;

    if (const std::optional<RegexMatch> ix = find_in(bounds_report(), text)) {
        // the bounds check's report: the row itself
        file = base_name(*ix->named("path"));
        line_number = static_cast<std::size_t>(number_of(*ix->named("line")));
        const std::optional<std::string> l = line_of(target.out, file, static_cast<std::int64_t>(line_number));
        if (!l) return std::nullopt;
        line = *l;
        // each name followed by `[` that is a variable of the data segment; the one nearest the reported column
        static const Regex subscripted(R"(\b(\w+)\s*\[)");
        const std::size_t column = static_cast<std::size_t>(number_of(*ix->named("col"))) - 1;
        std::optional<std::string> nearest;
        std::size_t nearest_distance = 0;
        subscripted.for_each_match(line, [&](const RegexMatch& m) {
            if (target.ds.contains(std::string(m[1]))) {
                const std::size_t d = m.start() > column ? m.start() - column : column - m.start();
                if (!nearest || d < nearest_distance) {
                    nearest = std::string(m[1]);
                    nearest_distance = d;
                }
            }
            return true;
        });
        if (!nearest) return std::nullopt;
        var = *nearest;
        given_index = signed_number_of(*ix->named("idx"));
        before = 0;
    } else {
        const std::optional<RegexMatch> m = find_in(before_start_report(), text);
        const std::optional<RegexMatch> v = find_in(before_variable(), text);
        if (!m || !v || !target.ds.contains(std::string(*v->named("var")))) return std::nullopt;
        var = std::string(*v->named("var"));
        before = number_of(*v->named("before"));
        file = base_name(*m->named("path"));
        line_number = static_cast<std::size_t>(number_of(*m->named("line")));
        const std::optional<std::string> l = line_of(target.out, file, static_cast<std::int64_t>(line_number));
        if (!l) return std::nullopt;
        line = *l;
    }

    const std::optional<IndexedMember> use = find_indexed_member(line, var);
    if (!use || index_has_side_effect(use->index)) return std::nullopt;
    const std::string& field = use->field;

    // the table's structure and its rows
    const std::optional<TableDecl> decl = table_decl(target.out, var);
    if (!decl) return std::nullopt;
    const std::string st = collapse_space(decl->type);
    const std::optional<std::string> body = struct_body(target.out, st);
    std::optional<CLayout> layout;
    if (body) layout = c_layout(*body);
    const pa::StructLayouts& old = target.old_layouts_for(file);
    const pa::StructLayout* old_layout = old.find(st);
    if (!layout || !layout->members.contains(field) || old_layout == nullptr || old_layout->find(field) == nullptr) return std::nullopt;
    const auto [field_offset, field_size] = layout->members.at(field);
    (void)field_size;

    std::int64_t idx = 0;
    if (given_index) {
        idx = *given_index;
    } else {
        // the place read, from the table's start, today: the row is below the first
        const std::int64_t position = -static_cast<std::int64_t>(before);
        const std::int64_t row_size = static_cast<std::int64_t>(layout->size);
        if (row_size == 0) return std::nullopt;
        idx = cb::floor_div(position - static_cast<std::int64_t>(field_offset), row_size);
        if (idx * row_size + static_cast<std::int64_t>(field_offset) != position) return std::nullopt;   // not that member of a row
    }

    // that row's member in the old data segment
    const std::int64_t address = static_cast<std::int64_t>(*target.ds.find(var)) + idx * static_cast<std::int64_t>(old_layout->size) +
                                 static_cast<std::int64_t>(old_layout->find(field)->offset);
    // the variable at or below that address (before every variable the program has: none to read)
    const std::string* owner_name = nullptr;
    std::uint64_t owner_address = 0;
    for (const auto& [name, a] : target.ds) {
        if (static_cast<std::int64_t>(a) <= address && (owner_name == nullptr || std::pair(a, name) > std::pair(owner_address, *owner_name))) {
            owner_name = &name;
            owner_address = a;
        }
    }
    if (owner_name == nullptr) return std::nullopt;
    const std::string& owner = *owner_name;
    const std::int64_t offset_in_owner = address - static_cast<std::int64_t>(owner_address);

    // what is there: of the variable's own type
    std::optional<std::string> what;
    const Regex owner_decl = named(R"(\b(struct\s+\w+)\s+@NAME@\s*\[)", owner);
    for (const auto& [name, t] : target.out) {
        const std::optional<RegexMatch> d = owner_decl.search(base::ctext::blank(t));
        if (!d) continue;
        const std::string owner_type = collapse_space(d->group(1));   // declared where it is read now, its header too
        target.extra_decls[file].insert(std::format("extern {} {}[];", owner_type, owner));
        const Regex defines = Regex(std::string(R"(\b)") + flex_quote(owner_type) + R"(\s*\{)");
        for (const auto& [h, ht] : target.out) {
            if (h.ends_with(".h") && defines.test(base::ctext::blank(ht))) {
                target.need_headers[file].insert(h);
                break;
            }
        }
        if (const pa::StructLayout* owner_layout = old.find(owner_type); owner_layout != nullptr && owner_layout->size != 0) {
            const std::int64_t size = owner_layout->size;
            const std::int64_t element = cb::floor_div(offset_in_owner, size);
            const std::int64_t rest = offset_in_owner - element * size;
            for (const pa::Member& m : owner_layout->members) {
                if (static_cast<std::int64_t>(m.offset) == rest) {
                    what = std::format("{}[{}].{}", owner, element, m.name);
                    break;
                }
            }
        }
        break;
    }
    if (!what && offset_in_owner == 0) what = owner;
    if (!what) return std::nullopt;

    const auto origin = [&]() -> const pa::LineOrigin* {
        const auto f = target.line_origin.find(file);
        if (f == target.line_origin.end()) return nullptr;
        const auto l = f->second.find(line_number);
        return l == f->second.end() ? nullptr : &l->second;
    }();
    const std::string replaced = line.substr(0, use->start) +
                                 std::format("(({}) == {} ? {} : {})", text::trim(use->index), idx, *what, use->whole) + line.substr(use->end);
    pa::LineFixKey key{file, origin != nullptr ? origin->text : line, origin != nullptr ? std::optional<std::size_t>(origin->nth) : std::nullopt};
    target.line_fixes[key] = replaced;
    const std::string why = std::format("{}:{} {}[{}].{} (case '{}'): before the table - the original read {} there", file, line_number,
                                        var, idx, field, case_name, *what);
    pa::note(target.mended, std::string(kNoteBeforeTable), why);
    return why;
}

// ---- low_reads -------------------------------------------------------------------------------------------------------------

namespace {

// The "struct tag" / "union tag" of a declaration `T *name` in a text.
std::optional<std::string> declared_pointer_type(std::string_view text, const std::string& name) {
    const Regex re = named(R"(\b((?:struct|union)\s+\w+)\s*\*\s*@NAME@\s*[;,=)])", name);
    if (const auto m = re.search(text)) return std::string(m->group(1));
    return std::nullopt;
}

}  // namespace

std::optional<std::string> low_reads(Target& target, std::string_view case_name, std::string_view text) {
    const std::optional<RegexMatch> m = find_in(small_address_report(), text);
    if (!m) return std::nullopt;
    const std::string file = base_name(*m->named("path"));
    const std::string address(*m->named("addr"));
    const std::string* file_text = target.out.find(file);
    if (file_text == nullptr) return std::nullopt;
    const std::size_t line_number = static_cast<std::size_t>(number_of(*m->named("line")));
    const std::vector<std::string> file_lines = split_lines(*file_text);
    const std::string line = line_number >= 1 && line_number <= file_lines.size() ? file_lines[line_number - 1] : std::string();

    // p[...]-> on the line: p's words are small numbers at times
    static const Regex indexed_arrow(R"((?<![\w.>])(\w+)\s*\[[^\[\]]*\]\s*->)");
    std::vector<std::string> indexed;
    indexed_arrow.for_each_match(line, [&](const RegexMatch& r) {
        indexed.emplace_back(r[1]);
        return true;
    });
    for (const std::string& v : indexed) {
        if (has_null_read(target, file, v)) continue;
        const Regex d = named(R"(\b((?:struct|union)\s+\w+)\s*\*\s*\*\s*@NAME@\b)", v);
        const Regex e = named(R"(\b((?:struct|union)\s+\w+)\s*\*\s*@NAME@\s*\[)", v);
        std::optional<std::string> tag;
        if (const auto a = d.search(*file_text)) tag = std::string(a->group(1));
        else if (const auto b = e.search(*file_text)) tag = std::string(b->group(1));
        if (!tag) continue;
        add_null_read(target, file, v, *tag + " *", pa::NullRead::Kind::Plain);
        const std::string why = std::format("{}: {}[...] a small number at times (read at {}h, case '{}'): read through the null segment there",
                                            file, v, address, case_name);
        pa::note(target.mended, std::string(kNoteSmallWord), why);
        return why;
    }

    if (rule_enabled("np_chains")) {
        // p->next->prev = ..: p was read through -- the NULL its link, p->next, the list's last's
        static const Regex chain(R"((?<![\w.>])(?:\(\s*\([^()]*\)\s*dos_np\(\s*(\w+)\s*\)\s*\)|(\w+))\s*->\s*(\w+)\s*->)");
        std::vector<std::pair<std::string, std::string>> chains;
        chain.for_each_match(line, [&](const RegexMatch& r) {
            chains.emplace_back(r.has(1) ? std::string(r[1]) : std::string(r[2]), std::string(r[3]));
            return true;
        });
        for (const auto& [v, member] : chains) {
            if (has_null_read_of_kind(target, file, member, pa::NullRead::Kind::Mem)) continue;
            const std::optional<std::string> tag = declared_pointer_type(*file_text, v);
            if (!tag) continue;
            // the definition the file sees: its own, its headers' (theirs, in turn)
            Files texts;
            for (const auto& [n, t] : target.files) texts.insert_or_assign(n, t);
            for (const auto& [n, t] : target.out)
                if (!t.empty()) texts.insert_or_assign(n, t);
            std::set<std::string> seen;
            std::vector<std::string> todo{file};
            std::string mine;
            static const Regex includes(R"re(#\s*include\s*"([^"]+)")re");
            while (!todo.empty()) {
                const std::string x = todo.front();
                todo.erase(todo.begin());
                if (seen.contains(x) || texts.find(x) == nullptr) continue;
                seen.insert(x);
                const std::string& t = texts.at(x);
                mine += (mine.empty() ? "" : "\n") + t;
                includes.for_each_match(t, [&](const RegexMatch& r) {
                    todo.emplace_back(r[1]);
                    return true;
                });
            }
            std::string everything;
            for (const auto& [n, t] : texts) everything += (everything.empty() ? "" : "\n") + t;
            const Regex body_re(std::string(R"(\b)") + flex_quote(*tag) + R"(\s*\{([^{}]*)\})");
            std::optional<std::string> body;
            if (const auto b = body_re.search(mine)) body = std::string(b->group(1));
            else if (const auto b2 = body_re.search(everything)) body = std::string(b2->group(1));
            if (!body) continue;
            const Regex member_re = named(R"(\b((?:struct|union)\s+\w+)\s*\*\s*(?:\w+\s*,\s*\*\s*)*@NAME@\s*[;,])", member);
            const auto mt = member_re.search(*body);
            if (!mt) continue;
            add_null_read(target, file, member, std::string(mt->group(1)) + " *", pa::NullRead::Kind::Mem);
            const std::string why = std::format("{}: {}->{} a NULL at times (at {}h, case '{}'): through the null segment there", file, v,
                                                member, address, case_name);
            pa::note(target.mended, std::string(kNoteNullRead), why);
            return why;
        }
        // ((q = f06df2(p))->kind: the value given back NULL at the list's end
        static const Regex assigned(R"(\(\s*(\w+)\s*=\s*\w+\s*\([^()]*\)\s*\)\s*->)");
        std::vector<std::string> assigned_to;
        assigned.for_each_match(line, [&](const RegexMatch& r) {
            assigned_to.emplace_back(r[1]);
            return true;
        });
        for (const std::string& v : assigned_to) {
            if (has_null_read_of_kind(target, file, v, pa::NullRead::Kind::Assign)) continue;
            const Regex d = named(R"(\b((?:struct|union)\s+\w+)\s*\*\s*(?:\w+\s*,\s*\*\s*)*@NAME@\s*[;,=)])", v);
            const auto found = d.search(*file_text);
            if (!found) continue;
            const std::string type = std::string(found->group(1)) + " *";
            add_null_read(target, file, v, type, pa::NullRead::Kind::Assign);
            if (!has_null_read_of_kind(target, file, v, pa::NullRead::Kind::Var))   // (and q->op after it)
                add_null_read(target, file, v, type, pa::NullRead::Kind::Var);
            const std::string why = std::format("{}: {} as given a NULL at times (at {}h, case '{}'): read through the null segment there",
                                                file, v, address, case_name);
            pa::note(target.mended, std::string(kNoteNullRead), why);
            return why;
        }
    }

    // l->op, l a NULL there: as DOS read it
    static const Regex arrow(R"((?<![\w.>])(\w+)\s*->)");
    std::vector<std::string> arrows;
    arrow.for_each_match(line, [&](const RegexMatch& r) {
        arrows.emplace_back(r[1]);
        return true;
    });
    for (const std::string& v : arrows) {
        if (has_null_read(target, file, v)) continue;
        const std::optional<std::string> tag = declared_pointer_type(*file_text, v);
        if (!tag) continue;
        add_null_read(target, file, v, *tag + " *", pa::NullRead::Kind::Var);
        const std::string why = std::format("{}: {} a NULL at times (read at {}h, case '{}'): read through the null segment there", file, v,
                                            address, case_name);
        pa::note(target.mended, std::string(kNoteNullRead), why);
        return why;
    }
    return std::nullopt;
}

// ---- mend ------------------------------------------------------------------------------------------------------------------

namespace {

// The rule that makes the array `var` (declared with `declared` elements, `extern`ed or defined) `now` elements.
void grow_declaration(Target& target, const std::string& var, std::uint64_t declared, std::uint64_t now, bool sized) {
    static const std::string kTypes =
        R"((?:char|short|int|long|unsigned|signed|float|double|struct\s+\w+|union\s+\w+|\w+_t)\b)";
    if (sized) {
        target.decl_rules.emplace_back(
            R"((?m)^(\s*(?:extern\s+|static\s+)?(?:[\w ]*?\b)?)" + kTypes + R"([\s*]+)" + regex_quote(var) + R"(\s*\[)\s*)" +
                std::to_string(declared) + R"(\s*\])",
            "${1}" + std::to_string(now) + "]");
    } else {
        target.decl_rules.emplace_back(R"((?m)^(\s*(?:static\s+)?(?:unsigned\s+|signed\s+)?char\s+)" + regex_quote(var) + R"(\s*\[)\s*\])",
                                       "${1}" + std::to_string(now) + "]");
    }
}

std::optional<std::string> mend_past_end(Target& target, const RegexMatch& m, std::string_view case_name, std::vector<Mended>& done) {
    const std::string var(*m.named("var"));
    const std::uint64_t size = number_of(*m.named("size")), after = number_of(*m.named("after")), sz = number_of(*m.named("sz"));
    const bool wrote = *m.named("rw") == "WRITE";

    // read into the variable after it: one table, as the original's linker made them
    if (target.joins_next && !target.reach.contains(var) && target.joins_next(var)) {
        std::string everything;
        for (const auto& [n, t] : target.files) everything += (everything.empty() ? "" : "\n") + t;
        const Regex elements = named(R"((?m)^\s*(?:unsigned\s+|signed\s+)?(?:char|short|int|long|unsigned)\s+@NAME@\s*\[[^\]]*\]\s*=\s*\{([^}]*)\})", var);
        std::uint64_t n = 0;
        if (const auto e = elements.search(everything)) {
            for (const std::string_view item : text::split(e->group(1), ','))
                if (!text::trim(item).empty()) ++n;
        }
        if (n != 0) {
            const std::uint64_t element = std::max<std::uint64_t>(1, size / n);
            target.reach[var] = (size + after + sz - 1) / element;
            const std::string why = std::format("{}[{}] {} (case '{}'): the table went on into the variables after it - one table", var,
                                                target.reach[var], wrote ? "write" : "read", case_name);
            pa::note(target.mended, std::string(kNoteReadIntoNext), why);
            done.push_back({var, why});
            return why;
        }
    }
    // a BSS array gone into the one after it
    if (target.bss.contains(var) && target.join_bss && target.join_bss(var)) {
        const std::string why = std::format("{} {} past its end (case '{}'): the BSS variable after it, one array with it", var,
                                            wrote ? "write" : "read", case_name);
        pa::note(target.mended, std::string(kNoteBssJoined), why);
        done.push_back({var, why});
        return why;
    }

    // its definition: not a declaration of it (extern)
    std::optional<std::uint64_t> declared;
    const Regex defined = named(R"((?m)^\s*(?!extern\b)(?:static\s+)?(?:[\w ]*?\b)?(?:char|short|int|long|unsigned|signed|float|double|struct\s+\w+|union\s+\w+|\w+_t)\b[\s*]+@NAME@\s*\[\s*(\d+)\s*\])", var);
    for (const auto& [f, t] : target.out) {
        if (const auto d = defined.search(t)) {
            declared = number_of(d->group(1));
            break;
        }
    }
    bool sized = true;
    if (!declared || *declared == 0) {   // char s[] = "...": its size the string's
        const Regex string_defined = named(R"((?m)^\s*(?:static\s+)?(?:unsigned\s+|signed\s+)?char\s+@NAME@\s*\[\s*\]\s*=)", var);
        declared.reset();
        for (const auto& [f, t] : target.out) {
            if (string_defined.test(t)) {
                declared = size;
                sized = false;
                break;
            }
        }
    }
    if (!declared || *declared == 0) return std::nullopt;
    const auto grown = target.grown.find(var);
    if (grown != target.grown.end() && grown->second > *declared) return std::nullopt;   // grown already, this round

    const std::uint64_t element = std::max<std::uint64_t>(1, size / *declared);   // a declaration larger than the size reported: one byte an element
    const std::uint64_t extra = (after + sz + element - 1) / element;
    target.grown[var] = *declared + extra;
    grow_declaration(target, var, *declared, *declared + extra, sized);
    const std::string why = std::format("{}[{}] {} (case '{}'): the original reached into the variable its linker put after it", var,
                                        *declared + extra - 1, wrote ? "written" : "read", case_name);
    pa::note(target.mended, std::string(kNoteArrayPast), why);
    done.push_back({var, why});
    return why;
}

}  // namespace

std::vector<Mended> mend(Target& target, const std::vector<Report>& reports) {
    std::vector<Mended> done;
    const auto already = [&](const std::string& why) {
        return std::any_of(done.begin(), done.end(), [&](const Mended& d) { return d.why == why; });
    };
    for (const Report& report : reports) {
        if (const std::optional<std::string> why = low_reads(target, report.case_name, report.text)) {
            done.push_back({report.case_name, *why});
            continue;
        }
        bool found = false;
        // each index the bounds check reported: one a read before a table's start, perhaps
        bounds_report().for_each_match(report.text, [&](const RegexMatch& ix) {
            if (const std::optional<std::string> why = before_start(target, report.case_name, ix.whole())) {
                if (!already(*why)) {
                    done.push_back({report.case_name, *why});
                    found = true;
                }
            }
            return true;
        });
        if (found) continue;
        constexpr std::string_view kBefore = "bytes before global variable";
        constexpr std::string_view kAfter = "bytes after global variable";
        if (const std::size_t at = report.text.find(kBefore); at != std::string::npos) {
            // a read before, unless the 400 bytes ahead of it already say "after"
            const std::string_view head = std::string_view(report.text).substr(0, at);
            const std::string_view tail = head.substr(head.size() > 400 ? head.size() - 400 : 0);
            if (tail.find(kAfter) == std::string_view::npos) {
                if (const std::optional<std::string> why = before_start(target, report.case_name, report.text)) {
                    done.push_back({report.case_name, *why});
                    continue;
                }
            }
        }
        past_end_report().for_each_match(report.text, [&](const RegexMatch& m) {
            (void)mend_past_end(target, m, report.case_name, done);
            return true;
        });
    }
    return done;
}

// ---- the sanitizer's runtime -----------------------------------------------------------------------------------------------

std::optional<std::filesystem::path> runtime_dir(const std::filesystem::path& compiler) {
    namespace stdfs = std::filesystem;
    const stdfs::path root = stdfs::absolute(compiler).parent_path().parent_path();
    const stdfs::path clang_lib = root / "lib" / "clang";
    std::error_code ec;
    if (!stdfs::is_directory(clang_lib, ec)) return std::nullopt;
    std::vector<std::string> versions;
    for (const stdfs::directory_entry& e : stdfs::directory_iterator(clang_lib, ec))
        if (e.is_directory(ec)) versions.push_back(fs::to_text(e.path().filename()));
    std::sort(versions.begin(), versions.end(), std::greater<>());   // the newest first
    for (const std::string& v : versions) {
        const stdfs::path dir = clang_lib / v / "lib" / "windows";
        if (!stdfs::is_directory(dir, ec)) continue;
        for (const stdfs::directory_entry& f : stdfs::directory_iterator(dir, ec)) {
            const std::string name = fs::to_text(f.path().filename());
            if (name.starts_with("clang_rt.asan_dynamic") && name.ends_with(".dll")) return dir;
        }
    }
    return std::nullopt;
}

}  // namespace cb::port::sanitize
