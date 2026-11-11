// cb/port/old_c_pipeline.cpp -- the steps of Port::modernize that are rewriting in their own right
// (old_c_passes.hpp): the report's notes, pascal names, callers' return types, headers, allocator sizes, loose
// calls, array sizes, header guards, and the one-name definitions of a file.

#include <algorithm>
#include <format>
#include <set>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_c_passes.hpp"
#include "cb/port/old_c_scan.hpp"

namespace cb::port::pre_ansi {

namespace {

using base::ctext::blank;

void add_all(Notes& notes, const char* why, const std::vector<std::string>& places) {
    for (const std::string& place : places) note(notes, why, place);
}

}  // namespace

// ================================================================ the report's notes
Notes gather_notes(const ReportInputs& in) {
    Notes notes = in.mended;
    add_all(notes,
            "a variable another file declares a pointer, declared a number here (the old linker did not mind): the pointer; "
            "0xffff, the top of the segment, the top of memory",
            in.lists.agreed);
    add_all(notes, "a structure's first word copied as an int (its chain's link then): the first member, as wide as it is now",
            in.lists.word_copies);
    add_all(notes, "a block of one structure's old size made for another, cast to it elsewhere: the larger of the two now",
            in.lists.shared_blocks);
    add_all(notes, "a view's pointer where the other has a number (one thing cast to both): the number a pointer's width", in.view_wide);
    add_all(notes,
            "a view only one thing cast to both gives, that would read a byte of the other's word: not taken (the other "
            "then a record of bytes)",
            in.view_puns_refused);
    add_all(notes, "a structure's bytes reached through a char * by their old places: the members that were there", in.lists.byte_places);
    add_all(notes,
            "a file's own copy of the program's structure, its words where the other has pointers (the old layout the "
            "same): the program's, by its names",
            in.lists.local_copies);
    add_all(notes,
            "a pooled allocator's blocks of a size for the old pieces: so many times the size (today's pointers), its "
            "first, static blocks as big",
            in.lists.pools);
    add_all(notes,
            "a table of a structure's old byte counts (its first members, by kind): the same parts of today's structure "
            "(offsetof, sizeof)",
            in.lists.size_tables);
    add_all(notes, "a long the other files declare, defined as the two words it was in the old data: the long", in.lists.longs);
    add_all(notes, "the program's own allocator of blocks of a structure's old size: the structure's size now", in.lists.allocators);
    for (const auto& [why, places] : in.union_notes)
        for (const std::string& place : places) note(notes, why, place);
    add_all(notes, "member retyped: the source declared it otherwise than the program uses it (--types)", in.member_fixes);
    add_all(notes, "a table read past its end into the variables after it: one table, their names its elements", in.lists.tables);
    for (const auto& [file, count] : in.far_near_dropped)
        note(notes, "far / near / pascal and their kind dropped (one flat memory, one way of calling)", std::format("{} ({})", file, count));
    return notes;
}

// ================================================================ pascal names
void rename_pascal_calls(const Files& files, const std::vector<std::string>& function_names,
                         const std::set<std::string, std::less<>>& known, std::map<std::string, std::string>& renames, Notes& notes) {
    // (pascal names were made capitals for the linker: a call in capitals and a definition in small letters were one function)
    std::map<std::string, std::string> lower;
    for (const std::string& f : function_names) lower.try_emplace(text::lower(f), f);
    for (const auto& [n, t] : files) {
        std::set<std::string> called;
        const std::string code = blank(t);   // (the call sites' names are views into it)
        for (const CallSite& site : call_sites(code, [](std::string_view w) { return !(w[0] >= '0' && w[0] <= '9'); }))
            called.insert(std::string(site.name));
        for (const std::string& c : called) {
            const auto d = lower.find(text::lower(c));
            if (d != lower.end() && d->second != c && !known.contains(c) && !renames.contains(c)) {
                renames[c] = d->second;
                note(notes, "a pascal name called in other letters (the linker made both capitals): the definition's", std::format("{} ({})", n, c));
            }
        }
    }
}

// ================================================================ the callers' declarations
void declared_return_types(const Files& files, OrderedMap<std::string, Fn>& functions, Notes& notes) {
    static const Regex declaration(
        R"re((?m)^\s*(?:extern\s+)?((?:unsigned\s+|signed\s+)?(?:struct\s+\w+|union\s+\w+|\w+)(?:\s*\*+\s*|\s+))(\w+)\s*\(\s*\)\s*;)re");
    // (the callers' declarations say what a function gives, where its definition left it old C's int)
    for (const std::string& n : sorted_names(files)) {
        declaration.for_each_match(blank(files.at(n)), [&](const RegexMatch& m) {
            const std::string type(text::trim(collapse_space(m.group(1))));
            Fn* f = functions.find(std::string(m.group(2)));
            if (!f || !f->ret.empty() || type == "int" || type == "extern" || type.starts_with("extern") || type.starts_with("static")) return true;
            const std::vector<std::string_view> words = text::split_ws(type);
            const std::string_view first_word = words.empty() ? std::string_view() : words.front();
            if (!is_type_word(first_word) && first_word != "struct" && first_word != "union") return true;
            f->ret = type;
            f->ret_set = true;
            note(notes, "what a function of old C's int gives, as its callers declared it", std::format("{} ({}: {})", n, f->name, f->ret));
            return true;
        });
    }
}

// ================================================================ headers
std::map<std::string, std::string> add_missing_includes(Files& out, const std::map<std::string, std::vector<std::string>>& need_headers,
                                                        const ExtraDecls& extra_decls, Notes& notes) {
    static const Regex tag_definition(R"re(\b(struct|union)\s+(\w+)\s*\{)re");
    static const Regex tag_use(R"re(\b(?:struct|union)\s+\w+)re");
    static const Regex include_line(R"re(#\s*include\s*"([^"]+)")re");
    static const Regex is_include(R"re(\s*#\s*include\b)re");

    // a structure: the header that defines it -- one alone; a tag two headers define is either's: none added
    std::map<std::string, std::string> tags;
    std::set<std::string> twice;
    for (const auto& [h, t] : out) {
        if (!h.ends_with(".h")) continue;
        tag_definition.for_each_match(blank(t), [&](const RegexMatch& m) {
            const std::string key = std::string(m.group(1)) + " " + std::string(m.group(2));
            const auto [it, fresh] = tags.try_emplace(key, h);
            if (!fresh && it->second != h) twice.insert(key);
            return true;
        });
    }
    for (const std::string& k : twice) tags.erase(k);

    for (const std::string& n : file_names(out)) {
        if (!n.ends_with(".c")) continue;
        std::string t = out.at(n);
        const std::string code = blank(t);
        std::set<std::string> own;
        tag_definition.for_each_match(code, [&](const RegexMatch& m) {
            own.insert(std::string(m.group(1)) + " " + std::string(m.group(2)));
            return true;
        });
        std::set<std::string> included;   // (and what those include, in turn)
        std::vector<std::string> todo;
        include_line.for_each_match(t, [&](const RegexMatch& m) {
            todo.emplace_back(m.group(1));
            return true;
        });
        while (!todo.empty()) {
            const std::string h = todo.back();
            todo.pop_back();
            if (!included.insert(h).second) continue;
            if (const std::string* text = out.find(h))
                include_line.for_each_match(*text, [&](const RegexMatch& m) {
                    todo.emplace_back(m.group(1));
                    return true;
                });
        }
        std::set<std::string> uses, need;
        tag_use.for_each_match(code, [&](const RegexMatch& m) {
            uses.insert(std::string(m.whole()));
            return true;
        });
        for (const std::string& u : uses)
            if (!own.contains(u))
                if (const auto it = tags.find(u); it != tags.end() && !included.contains(it->second)) need.insert(it->second);
        if (const auto it = need_headers.find(n); it != need_headers.end())
            for (const std::string& h : it->second)
                if (!included.contains(h)) need.insert(h);
        if (!need.empty()) {   // (the old build gave it the header some other way: after its own -- what those need comes first)
            std::vector<std::string> lines = split_lines(t);
            std::size_t at = 0;   // (after the last #include; at the top when the file has none)
            for (std::size_t i = 0; i < lines.size(); ++i)
                if (is_include.match(lines[i])) at = i + 1;
            std::vector<std::string> added;
            for (const std::string& h : need) added.push_back("#include \"" + h + "\"");
            lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(at), added.begin(), added.end());
            t = join_lines(lines);
            out.insert_or_assign(n, t);
            note(notes, "header of the structures a file uses, which it did not include: included",
                 std::format("{} ({})", n, text::join(std::vector<std::string>(need.begin(), need.end()), ", ")));
        }
        if (const auto it = extra_decls.find(n); it != extra_decls.end()) {   // (a variable a mend reads there: declared)
            std::vector<std::string> more;
            for (const std::string& d : it->second)
                if (t.find(d) == std::string::npos) more.push_back(d);
            if (!more.empty()) {
                std::vector<std::string> lines = split_lines(out.at(n));
                std::size_t at = 0;
                for (std::size_t i = 0; i < lines.size(); ++i)
                    if (is_include.match(lines[i])) at = i + 1;
                lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(at), more.begin(), more.end());
                out.insert_or_assign(n, join_lines(lines));
            }
        }
    }
    return tags;
}

// ================================================================ allocators that keep their blocks by an old size
void allocator_block_sizes(Files& out, const OrderedMap<std::string, Fn>& functions, const AllocatorSizes& allocators,
                           const std::map<std::string, std::string>& header_of, Notes& notes) {
    // (the program's allocators: the old sizes they keep their blocks by, the structures' sizes now)
    NamedRegex head(R"re((?m)^[\w\s*]*\b@1@\s*\([^;]*\)\s*$)re");
    NamedRegex number(R"re((?<![\w.])@1@(?![\w.]))re");
    for (const std::string& n : file_names(out)) {
        std::string t = out.at(n);
        for (const auto& [fn, sizes] : allocators) {
            const Fn* f = functions.find(fn);
            if (!f || !f->file || *f->file != n) continue;
            const std::string code = blank(t);
            const auto dm = head.with(fn).search(code);
            if (!dm) continue;
            const std::size_t after_head = dm->end();
            std::size_t body_end = code.find("\n}", after_head);
            if (body_end == std::string::npos) body_end = t.empty() ? 0 : t.size() - 1;
            body_end = std::max(body_end, after_head);
            for (const auto& [st, old_n] : sizes) {
                const std::string seg = t.substr(after_head, body_end - after_head);
                const std::string replaced = number.with(std::to_string(old_n)).replace_all(seg, "sizeof(" + st + ")");
                if (replaced == seg) continue;
                t = t.substr(0, after_head) + replaced + t.substr(body_end);
                body_end += replaced.size() - seg.size();
                const auto h = header_of.find(st);   // (the structure's header, for its size)
                if (h != header_of.end() && t.find("#include \"" + h->second + "\"") == std::string::npos) {
                    t = text::replace_first(t, "#include \"port.h\"", "#include \"port.h\"\n#include \"" + h->second + "\"");
                    body_end += std::string("\n#include \"" + h->second + "\"").size();
                }
                note(notes, "the program's own allocator keeps its blocks by a structure's old size: its size now",
                     std::format("{} ({}: {})", n, fn, old_n));
            }
        }
        out.insert_or_assign(n, std::move(t));
    }
}

// ================================================================ loose calls
void loose_calls(Files& out, const OrderedMap<std::string, Fn>& functions, const std::map<std::string, std::string>& renames, Notes& notes) {
    static const Regex integer_parameter(
        R"re((?:register\s+)?(?:unsigned\s+int|unsigned|short|int|signed|intptr_t)\s+\w+)re");
    static const Regex definition_head(R"re(^\s*(?:static\s+)?[\w\s*]*\b\w+\s*\([\w\s,]*\)\s*$)re");
    static const Regex char_pointer(R"re(\bchar\s*\*)re");

    // the calls of an old-style definition kept (its calls unchecked, its integer parameters intptr_t): each integer
    // given it made an intptr_t -- unprototyped, an int 0 leaves the upper half of the 64-bit place what it was
    std::map<std::string, std::vector<bool>> loose;   // function (as renamed) -> for each place, whether an integer goes there
    for (const auto& [name, f] : functions) {
        const auto renamed = renames.find(name);
        const std::string key = renamed == renames.end() ? name : renamed->second;
        if (f.loose) {
            std::vector<bool>& ints = loose[key];
            ints.clear();
            for (const Param& p : f.params) ints.push_back(integer_parameter.full_match(text::trim(p.type)).has_value());
        } else if (f.va_slots) {   // (its variable arguments a slot each: every one of them an intptr_t)
            std::vector<bool>& ints = loose[key];
            ints.assign(f.params.size(), false);
            ints.insert(ints.end(), 32, true);
        }
    }
    if (loose.empty()) return;
    for (const std::string& n : file_names(out)) {
        if (!n.ends_with(".c")) continue;
        std::vector<std::string> lines = split_lines(out.at(n));
        const std::vector<std::string> code = split_lines(blank(out.at(n)));
        bool changed = false;
        for (std::size_t i = 0; i < code.size(); ++i) {
            const std::vector<CallSite> sites = call_sites(code[i], [&](std::string_view w) { return loose.contains(std::string(w)); });
            if (sites.empty() || definition_head.match(code[i])) continue;   // (the definition's own head)
            std::string l = lines[i];
            for (auto site = sites.rbegin(); site != sites.rend(); ++site) {
                const std::string name(site->name);
                const auto spans = call_args(std::string_view(l).substr(site->at), name);
                if (!spans || spans->empty()) continue;
                const std::vector<bool>& ints = loose.at(name);
                for (std::size_t k = spans->size(); k-- > 0;) {
                    const Span& sp = (*spans)[k];
                    const std::string arg = l.substr(site->at + sp.begin, sp.size());
                    const std::string_view stripped = text::trim(arg);
                    if (k >= ints.size() || !ints[k] || stripped.empty() || stripped.starts_with("(intptr_t)") || stripped == "..." ||
                        char_pointer.test(arg))   // (a head's own: f(char *s, ...))
                        continue;
                    const std::string lead = arg.substr(0, arg.size() - text::ltrim(arg).size());
                    l = l.substr(0, site->at + sp.begin) + lead + "(intptr_t)(" + std::string(stripped) + ")" + l.substr(site->at + sp.end);
                    changed = true;
                }
            }
            if (l != lines[i]) {
                lines[i] = l;
                note(notes, "an integer given to an old-style definition kept (unchecked calls): made its intptr_t", std::format("{}:{}", n, i + 1));
            }
        }
        if (changed) out.insert_or_assign(n, join_lines(lines));
    }
}

// ================================================================ array sizes
OrderedMap<std::string, std::uint64_t> array_sizes_of(const Files& texts, const std::vector<DeclRule>& decl_rules) {
    static const Regex sized(R"re(\s*(?:static\s+)?[\w\s*]*?\b(\w+)\s*\[\s*(\d+)\s*\]\s*(?:=|;))re");
    static const Regex initialized(R"re(\s*(?:static\s+)?[\w\s*]*?\b(\w+)\s*\[\s*\]\s*=\s*\{)re");

    OrderedMap<std::string, std::uint64_t> sizes;
    for (const auto& kv : texts) {
        std::string t = kv.second;
        for (const DeclRule& rule : decl_rules) t = rule.apply(t);
        const std::string c = blank(t);
        const std::vector<std::string> lines = split_lines(c);
        const std::vector<int> depth = brace_depths(lines);
        std::size_t line_start = 0;   // the offset of line i in c
        for (std::size_t i = 0; i < lines.size(); line_start += lines[i].size() + 1, ++i) {
            const std::string& l = lines[i];
            const std::string_view trimmed = text::ltrim(l);
            if (depth[i] || trimmed.starts_with("extern") || trimmed.starts_with("#") || trimmed.starts_with("typedef")) continue;
            const bool call_before_bracket = l.substr(0, l.find('[')).find('(') != std::string::npos;
            if (const auto m = sized.match(l); m && !call_before_bracket) {
                sizes.try_emplace(std::string(m->group(1)), text::parse_int<std::uint64_t>(m->group(2), 10).value_or(UINT64_MAX));
                continue;
            }
            if (const auto m = initialized.match(l); m && !call_before_bracket) {
                const std::size_t open = line_start + m->end() - 1;
                int depth_now = 0;
                std::size_t close = open;
                while (close < c.size()) {
                    depth_now += c[close] == '{' ? 1 : c[close] == '}' ? -1 : 0;
                    if (depth_now == 0) break;
                    ++close;
                }
                std::uint64_t rows = 0;   // (its rows: commas outside braces)
                int nesting = 0;
                bool row_has_text = false;
                for (std::size_t k = open + 1; k < close && k < c.size(); ++k) {
                    const char ch = c[k];
                    nesting += (ch == '(' || ch == '{' || ch == '[') - (ch == ')' || ch == '}' || ch == ']');
                    if (ch == ',' && nesting == 0) {
                        rows += row_has_text;
                        row_has_text = false;
                    } else if (!text::is_space(ch)) {
                        row_has_text = true;
                    }
                }
                rows += row_has_text;
                if (rows) sizes.try_emplace(std::string(m->group(1)), rows);
            }
        }
    }
    return sizes;
}

// ================================================================ header guards
void guard_headers(Files& out) {
    for (const std::string& h : file_names(out)) {   // (every header of the program's: once -- port.h and data.c include some of them too)
        if (!h.ends_with(".h")) continue;
        const std::string& text = out.at(h);
        if (text.find("DOSRT_ONCE_") != std::string::npos) continue;
        const std::string guard = "DOSRT_ONCE_" + text::upper(text::sanitize_identifier(h));
        std::string body = text;
        while (!body.empty() && body.back() == '\n') body.pop_back();
        out.insert_or_assign(h, "#ifndef " + guard + "\n#define " + guard + "\n" + body + "\n#endif\n");
    }
}

// ================================================================ the one-name definitions of a file
std::vector<GlobalDefinition> global_definitions(std::string_view text) {
    static const Regex statement(R"re(([^=]*?)(?:=(.*))?;$)re");
    static const Regex static_word(R"re(\bstatic\s+)re");
    const std::string code = blank(text);
    const std::vector<std::string> code_lines = split_lines(code);
    const std::vector<std::string> raw_lines = split_lines(text);
    const std::vector<int> depth = brace_depths(code_lines);
    std::vector<GlobalDefinition> out;
    for (std::size_t i = 0; i < code_lines.size(); ++i) {
        const std::string_view c = code_lines[i];
        const std::string st(text::trim(c));
        const std::string without_function_pointers = text::replace_all(st, "(*", "");
        if (depth[i] || !st.ends_with(';') || st.starts_with("extern") || st.starts_with("typedef") || st.starts_with("#") ||
            (without_function_pointers.find('(') != std::string::npos && st.find('=') == std::string::npos))
            continue;
        const auto m = statement.match(st);
        if (!m) continue;
        const auto declared = declarators(text::trim(m->group(1)));
        if (!declared || declared->names.size() != 1) continue;
        const Declarator& d = declared->names.front();
        // (static left out; the words one space apart)
        const std::string base(text::trim(collapse_space(static_word.replace_all(declared->base, ""))));
        // the declarator with the name as `@` and no spaces
        std::string shape;
        for (std::size_t k = 0; k < d.text.size();) {
            if (is_word_char(d.text[k])) {
                std::size_t end = k;
                while (end < d.text.size() && is_word_char(d.text[end])) ++end;
                shape += d.text.substr(k, end - k) == d.name ? "@" : d.text.substr(k, end - k);
                k = end;
            } else {
                if (d.text[k] != ' ') shape.push_back(d.text[k]);
                ++k;
            }
        }
        GlobalDefinition g{i, d.name, base, shape, std::nullopt};
        if (m->has(2)) {
            const std::size_t eq = c.find('='), semi = c.rfind(';');
            g.initializer = std::string(text::trim(std::string_view(raw_lines[i]).substr(eq + 1, semi - (eq + 1))));
        }
        out.push_back(std::move(g));
    }
    return out;
}

}  // namespace cb::port::pre_ansi
