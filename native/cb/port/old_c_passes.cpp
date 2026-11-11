#include "cb/port/old_c_passes.hpp"

#include <algorithm>
#include <format>

#include "cb/base/ctext.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/port/csource.hpp"

namespace cb::port {

using base::ctext::blank;
using csource::apply_edits;
using csource::Edit;
using csource::LineIndex;

namespace {

bool is_word(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_';
}

void note(Notes& notes, std::string_view why, std::string where) {
    notes[std::string(why)].push_back(std::move(where));
}

// The pieces of `s` split at `sep`, trimmed, with the empty ones kept.
std::vector<std::string> split_trimmed(std::string_view s, char sep) {
    std::vector<std::string> out;
    for (std::string_view p : text::split(s, sep))
        out.emplace_back(text::trim(p));
    return out;
}

bool has(std::string_view s, std::string_view part) {
    return s.find(part) != std::string_view::npos;
}

bool contains(const std::vector<std::string>& v, std::string_view x) {
    return std::find(v.begin(), v.end(), x) != v.end();
}

} // namespace

// ---- Rules --------------------------------------------------------------------------------------------

Rules::Rules(std::string_view spec) {
    for (std::string_view piece : text::split(spec, ',')) {
        std::string w;
        for (char c : piece) {
            if (c != ' ')
                w += c;
        }
        items_.push_back(std::move(w));
    }
}

Rules Rules::from_environment() {
    return Rules(fs::get_env("CB_PORT_RULES").value_or(""));
}

bool Rules::on(std::string_view name) const {
    bool on = true;
    for (const std::string& w : items_) {
        if (w == name || w == "+all" ||
            (w.size() == name.size() + 1 && w[0] == '+' && w.compare(1, name.size(), name) == 0)) {
            on = true;
        } else if (w == "-all" ||
                   (w.size() == name.size() + 1 && w[0] == '-' && w.compare(1, name.size(), name) == 0)) {
            on = false;
        }
    }
    return on;
}

// ---- block_heads --------------------------------------------------------------------------------------

std::string block_heads(std::string_view t, Notes& notes, std::string_view fname, const Names& ptr2) {
    // (char *)<operand> + <n>, the operand a parenthesized expression or a chain of names
    static const Regex chained(R"(\(\s*char\s*\*\s*\)\s*((?:\((?:[^()]|\([^()]*\))*\)|[\w.\->]+))\s*\+\s*(\d+)\b)");
    static const Regex char_pointer_pointer(R"(\(\s*char\s*\*\s*\*\s*\))");
    static const Regex word(R"(\w+)");
    static const Regex row_re(R"(\{([^{}]*)\})"); // ({ (char **)g, 1016, g + 4, ... }: a pool's first block)
    static const Regex chain_head(R"(\(\s*char\s*\*\s*\*\s*\)\s*(\w+))");
    static const Regex allocation(R"(\b(?:malloc|calloc|alloc|getmem|sbrk)\s*\(([^;()]*\S)\s*\+\s*(\d+)\s*\))");

    const std::string code = blank(t);
    std::vector<Edit> edits;
    std::set<unsigned long long> heads;
    const auto head_edit = [&](std::size_t start, std::size_t end, unsigned long long n) {
        edits.push_back({start, end, std::format("DOS_HEAD({})", n)});
        heads.insert(n);
    };

    chained.for_each_match(code, [&](const RegexMatch& m) {
        const std::string_view operand = m.group(1);
        const auto n = text::parse_int<unsigned long long>(m.group(2));
        if (!n || *n < 2 || *n % 2 != 0)
            return true;
        const std::string squeezed = text::replace_all(csource::collapse_space(operand), "( char", "(char");
        const std::vector<std::string> names = word.find_all(char_pointer_pointer.replace_all(operand, ""));
        const bool chain_of_char_pointers = has(squeezed, "char **");
        const bool link_member = !names.empty() && ptr2.contains(names.back());
        if (!(chain_of_char_pointers || link_member || char_pointer_pointer.test(operand)))
            return true;
        head_edit(m.start(2), m.end(2), *n);
        return true;
    });
    row_re.for_each_match(code, [&](const RegexMatch& row_match) {
        const std::string_view row = row_match.group(1);
        chain_head.for_each_match(row, [&](const RegexMatch& c) {
            const std::string_view name = c.group(1);
            // <name> + <n>: the head of the block the name's pointer starts
            for (std::size_t at = row.find(name); at != std::string_view::npos; at = row.find(name, at + 1)) {
                if ((at > 0 && is_word(row[at - 1])) ||
                    (at + name.size() < row.size() && is_word(row[at + name.size()])))
                    continue;
                std::size_t i = at + name.size();
                while (i < row.size() && text::is_space(row[i]))
                    ++i;
                if (i >= row.size() || row[i] != '+')
                    continue;
                ++i;
                while (i < row.size() && text::is_space(row[i]))
                    ++i;
                std::size_t end = i;
                while (end < row.size() && std::isdigit(static_cast<unsigned char>(row[end])) != 0)
                    ++end;
                if (end == i || (end < row.size() && is_word(row[end])))
                    continue;
                const auto n = text::parse_int<unsigned long long>(row.substr(i, end - i));
                if (n && *n >= 2 && *n % 2 == 0)
                    head_edit(row_match.start(1) + i, row_match.start(1) + end, *n);
            }
            return true;
        });
        return true;
    });
    // (and the blocks given: their heads as long)
    allocation.for_each_match(code, [&](const RegexMatch& m) {
        const auto n = text::parse_int<unsigned long long>(m.group(2));
        if (n && heads.contains(*n) && m.group(2) == std::to_string(*n)) {
            edits.push_back({m.start(2), m.end(2), std::format("DOS_HEAD({})", *n)});
        }
        return true;
    });

    // the last edit at a place is the one made (the longer, by the sort); the rest at that place are not
    std::sort(edits.begin(), edits.end(), std::greater<>());
    std::string out(t);
    std::set<std::size_t> seen;
    std::size_t made = 0;
    for (const Edit& e : edits) {
        if (!seen.insert(e.start).second)
            continue;
        out.replace(e.start, e.end - e.start, e.text);
        ++made;
    }
    if (made != 0) {
        note(notes,
             "a block chained by its first word, its bytes after a head of the old size: the head DOS_HEAD (the link a "
             "pointer of today's)",
             std::format("{} ({})", fname, made));
    }
    return out;
}

// ---- whole_word_stores --------------------------------------------------------------------------------

std::string whole_word_stores(std::string_view t, Notes& notes, std::string_view fname) {
    static const Regex declared_param(R"(\bintptr_t\s+(\w+)\s*[,)=])");
    static const Regex declared_list(R"(\bintptr_t\s+([^;(){}]*);)");
    static const Regex store(R"(\*\s*\(\s*(?:u?int16_t|int|short|unsigned)\s*\*\s*\)\s*(\w+)\s*=\s*(\w+)\s*;)");

    std::vector<std::string> lines = csource::split_lines(t);
    const std::vector<std::string> code = csource::split_lines(blank(t));
    for (const csource::FunctionSpan& fn : csource::definitions_any(code)) {
        std::string body;
        for (std::size_t i = fn.first; i <= fn.last && i < code.size(); ++i)
            body += (i == fn.first ? "" : "\n") + code[i];
        std::set<std::string> wide;
        declared_param.for_each_match(body, [&](const RegexMatch& m) {
            wide.emplace(m.group(1));
            return true;
        });
        declared_list.for_each_match(body, [&](const RegexMatch& m) { // (intptr_t n, v, pool;)
            for (std::string_view declarator : text::split(m.group(1), ',')) {
                std::size_t i = 0;
                while (i < declarator.size() && text::is_space(declarator[i]))
                    ++i;
                while (i < declarator.size() && declarator[i] == '*')
                    ++i;
                while (i < declarator.size() && text::is_space(declarator[i]))
                    ++i;
                std::size_t end = i;
                while (end < declarator.size() && is_word(declarator[end]))
                    ++end;
                wide.emplace(declarator.substr(i, end - i));
            }
            return true;
        });
        if (wide.empty())
            continue;
        for (std::size_t x = fn.open; x <= fn.last && x < code.size(); ++x) {
            const auto m = store.search(code[x]);
            if (m && wide.contains(std::string(m->group(2)))) {
                lines[x] = lines[x].substr(0, m->start()) +
                           std::format("*(intptr_t *){} = {};", m->group(1), m->group(2)) + lines[x].substr(m->end());
                note(notes, "a word stored through (int *) of a value that may be a pointer (intptr_t): stored whole",
                     std::format("{}:{}", fname, x + 1));
            }
        }
    }
    return csource::join_lines(lines);
}

// ---- file_internals -----------------------------------------------------------------------------------

std::string file_internals(std::string_view t, Notes& notes, std::string_view fname) {
    static const Regex internal(
        R"(([A-Za-z_][\w.]*(?:->\w+|\[[^\]]*\])*)\s*->\s*_(file|flag)\b(?!\s*(?:[-+*/|&^]?=(?!=)|\+\+|--)))");
    const std::string code = blank(t);
    std::vector<Edit> edits;
    internal.for_each_match(code, [&](const RegexMatch& m) {
        edits.push_back(
            {m.start(), m.end(), std::format("{}({})", m.group(2) == "file" ? "fileno" : "dos_fflag", m.group(1))});
        return true;
    });
    if (!edits.empty()) {
        note(notes,
             "the old library's FILE read inside (today's keeps it to itself): fileno, its end-of-file and error flags "
             "(dos_fflag)",
             std::format("{} ({})", fname, edits.size()));
    }
    return apply_edits(t, std::move(edits));
}

// ---- stdio_inits --------------------------------------------------------------------------------------

std::string stdio_inits(std::string_view t, Notes& notes, std::string_view fname) {
    static const Regex definition(R"((?m)^((?:static\s+)?)(FILE\s*\*\s*)(\w+)\s*=\s*(stdin|stdout|stderr)\s*;(.*)$)");
    if (!fname.ends_with(".c"))
        return std::string(t);
    std::vector<std::pair<std::string, std::string>> sets;
    std::string out = definition.replace_all(t, [&](const RegexMatch& m) {
        sets.emplace_back(m.group(3), m.group(4));
        note(notes,
             "a variable given stdin, stdout or stderr where it is defined (not a constant now): set before main",
             std::format("{} ({})", fname, m.group(3)));
        return std::format("{}FILE *{};{}", m.group(1), m.group(3), m.group(5));
    });
    for (const auto& [var, value] : sets) {
        while (out.ends_with('\n'))
            out.pop_back();
        out += std::format("\nDOS_INIT(dos_init_{}) {{ {} = {}; }}\n", var, var, value);
    }
    return out;
}

// ---- word_copies --------------------------------------------------------------------------------------

std::string word_copies(std::string_view t, Notes& notes, std::string_view fname, const Names& copied) {
    static const Regex union_head(R"(\bunion\s*\w*\s*\{)");
    static const Regex nested(R"(\b(?:struct|union)\s*\w*\s*\{[^{}]*\})");
    static const Regex pointer_member(R"(\*\s*\w+\s*;)");
    static const Regex int_member(R"((?m)^([ \t]*)(?:u?int16_t|short|int|unsigned)\s+(\w+)\s*;)");
    if (copied.empty())
        return std::string(t);
    const std::string code = blank(t);
    std::vector<Edit> edits;
    union_head.for_each_match(code, [&](const RegexMatch& um) {
        int depth = 0;
        std::size_t k = um.end() - 1;
        while (k < code.size()) {
            depth += (code[k] == '{') - (code[k] == '}');
            if (depth == 0)
                break;
            ++k;
        }
        const std::string body = code.substr(um.end(), k - um.end());
        // (the members of a structure or union inside it are its own: blanked)
        const std::string flat =
            nested.replace_all(body, [](const RegexMatch& x) { return std::string(x.whole().size(), ' '); });
        if (!pointer_member.test(flat))
            return true; // (no pointer in it: a number is a number)
        int_member.for_each_match(flat, [&](const RegexMatch& st) {
            if (copied.contains(st.group(2))) {
                edits.push_back({um.end() + st.start(), um.end() + st.end(),
                                 std::format("{}intptr_t {};", st.group(1), st.group(2))});
                note(notes,
                     "an int of a union of pointers, copied from one to another (the word it was: a pointer too): "
                     "intptr_t",
                     std::format("{} ({})", fname, st.group(2)));
            }
            return true;
        });
        return true;
    });
    return apply_edits(t, std::move(edits));
}

// ---- word_unions --------------------------------------------------------------------------------------

std::string word_unions(std::string_view t, Notes& notes, std::string_view fname, const Names& olds) {
    static const Regex word_union(R"(\bunion(\s+\w+)?\s*\{([^{}]*)\})");
    static const Regex plain_int(R"((?:u?int16_t|short|int|unsigned)\s+\w+)");
    return word_union.replace_all(t, [&](const RegexMatch& m) {
        std::vector<std::string> pointers, ints, all;
        for (std::string_view d : text::split(m.group(2), ';')) {
            const std::string_view decl = text::trim(d);
            if (decl.empty())
                continue;
            all.emplace_back(decl);
            if (has(decl, "*"))
                pointers.emplace_back(decl);
            if (plain_int.full_match(decl))
                ints.emplace_back(decl);
        }
        const std::string tag(text::trim(m.group(1)));
        if (pointers.empty() || ints.size() != 1 || ints.size() + pointers.size() != all.size() || olds.contains(tag)) {
            return std::string(m.whole());
        }
        const std::string name = ints[0].substr(ints[0].find_last_of(" \t\r\n\v\f") + 1);
        note(notes, "a union of a number and pointers (one 16-bit word then): the number intptr_t",
             std::format("{} ({})", fname, name));
        return text::replace_first(m.whole(), ints[0], "intptr_t " + name);
    });
}

// ---- old_c_ways ---------------------------------------------------------------------------------------

void old_c_ways(Sources& out, Notes& notes) {
    // (*(int16_t *)&p->size = dos_ph(..): a handle kept in the word over two bytes, read as a pointer: what it stands
    // for)
    static const Regex handle_store(R"(\*\s*\(\s*int16_t\s*\*\s*\)\s*&\s*[^=;]*?->\s*(\w+)\s*=\s*dos_ph\s*\()");
    std::set<std::string> held;
    for (const auto& [name, t] : out) {
        const std::string code = blank(t);
        handle_store.for_each_match(code, [&](const RegexMatch& m) {
            held.emplace(m.group(1));
            return true;
        });
    }
    if (!held.empty()) {
        std::string names;
        for (const std::string& h : held)
            names += (names.empty() ? "" : "|") + csource::regex_escape(h);
        // The one pattern of this pass that is built from the program (the members found above), once per call.
        const Regex read_as_pointer(
            R"(\(\s*((?:struct|union)\s+\w+\s*\*|char\s*\*)\s*\)\s*\(\s*(\*\s*\(\s*int16_t\s*\*\s*\)\s*&\s*[\w>\-.\s()*]*?->\s*(?:)" +
            names + R"())\s*\))");
        for (auto& [name, t] : out) {
            if (t.empty() || !name.ends_with(".c"))
                continue;
            const std::string before = t;
            std::string t2 = base::ctext::sub_code(before, read_as_pointer, [&](const RegexMatch& m) {
                return std::format("({})dos_hp({})", m.group(1),
                                   std::string_view(before).substr(m.start(2), m.end(2) - m.start(2)));
            });
            if (t2 != before) {
                t = std::move(t2);
                note(notes, "a handle kept in a word over two bytes, read as a pointer: what it stands for (dos_hp)",
                     name);
            }
        }
    }

    static const Regex string_literal_pointer(
        R"((?m)^((?:static\s+)?(?:unsigned\s+)?char\s*\*\s*\w+\s*=\s*)("(?:[^"\\\n]|\\.)*")\s*;)");
    static const Regex time_call(R"(\btime\s*\(\s*(?:\(\s*(?:time_t|long|int32_t)\s*\*\s*\)\s*)?&\s*(\w+)\s*\))");
    for (auto& [name, text_ref] : out) {
        if (text_ref.empty() || !name.ends_with(".c"))
            continue;
        const std::string t = text_ref;
        const std::string code = blank(t);
        const std::vector<int> ds = csource::depths(csource::split_lines(code));
        const LineIndex lines(code);
        std::vector<Edit> edits;
        string_literal_pointer.for_each_match(code, [&](const RegexMatch& m) {
            if (ds[lines.line_of(m.start())] == 0) {
                edits.push_back(
                    {m.start(2), m.end(2), "(char []){" + t.substr(m.start(2), m.end(2) - m.start(2)) + "}"});
            }
            return true;
        });
        time_call.for_each_match(code, [&](const RegexMatch& m) {
            const std::string var(m.group(1));
            if (csource::name_regex(R"(\b(?:long|int32_t|uint32_t)\s+(?:\w+\s*,\s*)*@@\b)", var).test(code)) {
                edits.push_back({m.start(), m.end(), std::format("({} = (int32_t)time(NULL))", var)});
            }
            return true;
        });
        if (edits.empty())
            continue;
        const std::size_t count = edits.size();
        text_ref = apply_edits(t, std::move(edits));
        note(
            notes,
            "what old C allowed that today's does not: a string's text written to (an array of its own), time() into a "
            "long (today's time_t 8 bytes)",
            std::format("{} ({})", name, count));
    }
}

// ---- numbered_places ----------------------------------------------------------------------------------

void numbered_places(Sources& out, const std::vector<std::string>& extra, Notes& notes) {
    static const Regex global_def(
        R"(\s*(?:static\s+)?((?:(?:struct|union)\s+\w+|(?:unsigned\s+)?\w+)\s*\**\s*(g_[0-9a-f]{4})\s*(?:\[[^\]]*\])?)\s*(?:=|;))");
    // a place given as the number of the variable that is there: (struct hsym **)0x3a20 or (struct hsym **)(0x3a20)
    static const Regex numbered(
        R"(\(\s*(?:(?:struct|union)\s+\w+|\w+)\s*\*+\s*\)\s*(\((?:[^()]*)\)|0x[0-9a-fA-F]{4}\b))");
    static const Regex hex_place(R"(\b0x([0-9a-fA-F]{4})\b)");

    std::map<std::string, std::string, std::less<>> declarations; // name: its declaration, as the definition says it
    const std::string extra_text = text::join(extra, "\n");
    const auto read_declarations = [&](std::string_view t) {
        const std::vector<std::string> lines = csource::split_lines(blank(t));
        const std::vector<int> ds = csource::depths(lines);
        for (std::size_t i = 0; i < lines.size(); ++i) {
            const auto m = global_def.match(lines[i]);
            if (!m || ds[i] != 0)
                continue;
            const std::string_view lead = text::ltrim(lines[i]);
            if (lead.starts_with("extern") || lead.starts_with("typedef") || lead.starts_with('#'))
                continue;
            declarations.emplace(std::string(m->group(2)),
                                 std::string(text::trim(csource::collapse_space(m->group(1)))));
        }
    };
    for (const auto& entry : out)
        read_declarations(entry.second);
    read_declarations(extra_text);
    if (declarations.empty())
        return;

    for (auto& [name, text_ref] : out) {
        if (!name.ends_with(".c") || text_ref.empty())
            continue;
        std::string t = text_ref;
        const std::string code = blank(t);
        std::vector<Edit> edits;
        std::set<std::string> used;
        numbered.for_each_match(code, [&](const RegexMatch& m) {
            const std::size_t seg_at = m.start(1);
            hex_place.for_each_match(m.group(1), [&](const RegexMatch& h) {
                std::string variable = "g_" + text::lower(h.group(1));
                if (declarations.contains(variable)) {
                    edits.push_back({seg_at + h.start(), seg_at + h.end(), variable});
                    used.insert(std::move(variable));
                }
                return true;
            });
            return true;
        });
        if (edits.empty())
            continue;
        t = apply_edits(t, std::move(edits));
        std::vector<std::string> need;
        const std::string edited_code = blank(t);
        for (const std::string& variable : used) {
            if (!csource::name_regex(R"((?m)^[^\n(]*\b@@\b[^\n(]*;)", variable).test(edited_code))
                need.push_back(variable);
        }
        if (!need.empty()) {
            std::vector<std::string> lines = csource::split_lines(t);
            std::size_t at = 0;
            for (std::size_t i = 0; i < lines.size(); ++i) {
                if (lines[i].starts_with("#include"))
                    at = i + 1;
            }
            std::vector<std::string> externs;
            for (const std::string& variable : need)
                externs.push_back("extern " + declarations.find(variable)->second + ";");
            lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(at), externs.begin(), externs.end());
            t = csource::join_lines(lines);
        }
        text_ref = std::move(t);
        note(notes, "a variable's place given as its number (the old data segment's): the variable",
             std::format("{} ({})", name, text::join(used, ", ")));
    }
}

// ---- agree_wide ---------------------------------------------------------------------------------------

void agree_wide(Sources& out, const std::vector<std::string>& extra, Notes& notes, const Rules& rules) {
    static const Regex wide_declaration(R"(\s*(?:extern\s+|static\s+)?intptr_t\s+([^;()]*);)");
    static const Regex bare_name(R"(\s*\w+\s*)");
    static const Regex pointer_to_wide(R"(\(\s*intptr_t\s*\*\s*\)\s*&\s*(\w+)\b(?!\s*(?:\[|\.|->)))");
    static const Regex declaration(R"((?m)^([ \t]*(?:extern\s+)?)((?:u?int16_t|int|short|unsigned))\s+([\w\s,]+);)");
    static const Regex definition(
        R"((?m)^([ \t]*(?:static\s+)?)((?:u?int16_t|int|short|unsigned))\s+(\w+)(\s*=[^;{}]*;))");
    static const Regex plain_definition(R"((?m)^([ \t]*(?:static\s+)?)((?:u?int16_t|int|short|unsigned))\s+(\w+)\s*;)");
    static const Regex table_place(R"(([{,]\s*)&\s*(\w+)\s*(?=[,}]))");
    static const Regex table_start(R"(=\s*\{)");

    const bool more = rules.on("agree_wide_defs"); // (definitions and tables' places too)
    std::set<std::string> wide;
    const auto find_wide = [&](std::string_view t) {
        const std::string code = blank(t);
        const std::vector<std::string> lines = csource::split_lines(code);
        const std::vector<int> ds = csource::depths(lines);
        for (std::size_t i = 0; i < lines.size(); ++i) {
            const auto m = wide_declaration.match(lines[i]);
            if (ds[i] != 0 || !m)
                continue;
            for (std::string_view d : text::split(m->group(1), ',')) {
                if (bare_name.full_match(d))
                    wide.emplace(text::trim(d));
            }
        }
        if (more) { // (a table's { "-FPc", (int *)&g_0cbc }: the place a word of any row's width is written to)
            pointer_to_wide.for_each_match(code, [&](const RegexMatch& m) {
                wide.emplace(m.group(1));
                return true;
            });
        }
    };
    for (const auto& entry : out)
        find_wide(entry.second);
    find_wide(text::join(extra, "\n"));
    if (wide.empty())
        return;

    for (auto& [name, text_ref] : out) {
        if (text_ref.empty())
            continue;
        const std::string t = text_ref;
        const std::string code = blank(t);
        const std::vector<std::string> lines = csource::split_lines(code);
        const std::vector<int> ds = csource::depths(lines);
        const LineIndex index(code);
        std::set<Edit> edits;
        declaration.for_each_match(code, [&](const RegexMatch& m) {
            if (ds[index.line_of(m.start())] != 0 || !has(m.group(1), "extern"))
                return true;
            const std::vector<std::string> names = split_trimmed(m.group(3), ',');
            std::vector<std::string> hit, rest;
            for (const std::string& w : names)
                (wide.contains(w) ? hit : rest).push_back(w);
            if (hit.empty())
                return true;
            std::string replacement;
            if (!rest.empty()) {
                replacement = std::format("{}{} {}; ", m.group(1), m.group(2), text::join(rest, ", "));
                replacement += std::format("extern intptr_t {};", text::join(hit, ", "));
            } else {
                replacement = std::string(m.group(1)) + std::format("intptr_t {};", text::join(hit, ", "));
            }
            edits.insert({m.start(), m.end(), std::move(replacement)});
            return true;
        });
        if (more) {
            definition.for_each_match(code, [&](const RegexMatch& m) { // (its definition: int16_t g_0cbc = 0;)
                if (ds[index.line_of(m.start())] == 0 && wide.contains(std::string(m.group(3)))) {
                    edits.insert({m.start(2), m.end(2), "intptr_t"});
                }
                return true;
            });
            plain_definition.for_each_match(code, [&](const RegexMatch& m) { // (or without a value: int16_t g_0cbc;)
                if (ds[index.line_of(m.start())] == 0 && wide.contains(std::string(m.group(3))) &&
                    !has(m.group(1), "extern")) {
                    edits.insert({m.start(2), m.end(2), "intptr_t"});
                }
                return true;
            });
            // (and given as the place of a 16-bit one too, { 'a', 1, &g_0c92 }: a file's table's, not a call's)
            table_place.for_each_match(code, [&](const RegexMatch& m) {
                const std::size_t line = index.line_of(m.start());
                std::optional<std::size_t> top;
                for (std::size_t j = line + 1; j-- > 0;) {
                    if (ds[j] == 0) {
                        top = j;
                        break;
                    }
                }
                if (wide.contains(std::string(m.group(2))) && top && table_start.test(lines[*top])) {
                    const std::size_t at = m.start() + m.group(1).size();
                    edits.insert({at, at, "(void *)"});
                }
                return true;
            });
        }
        if (edits.empty())
            continue;
        text_ref = apply_edits(t, std::vector<Edit>(edits.begin(), edits.end()));
        note(notes,
             "a variable intptr_t in some files (it holds a pointer), a number of 16 bits in another: intptr_t in all",
             name);
    }
}

// ---- widen_member -------------------------------------------------------------------------------------

std::optional<std::string> widen_member(std::string_view text_in, std::string_view tag, std::string_view mem) {
    static const Regex integer_list(R"((?m)\b((?:u?int16_t|int|short|unsigned))\s+([\w\s,]+?)\s*;)");
    static const Regex plain_word(R"(\w+)");
    const std::string code = blank(text_in);
    const std::size_t body_at = csource::find_tag_body(code, "struct", tag);
    if (body_at == std::string::npos)
        return std::nullopt;
    const std::size_t end = code.find('}', body_at);
    if (end == std::string::npos)
        return std::nullopt;
    const std::string body = code.substr(body_at, end - body_at);
    std::optional<std::string> result;
    integer_list.for_each_match(body, [&](const RegexMatch& d) {
        const std::vector<std::string> names = split_trimmed(d.group(2), ',');
        if (!contains(names, mem) || !std::all_of(names.begin(), names.end(), [](const std::string& x) {
                return plain_word.full_match(x).has_value();
            })) {
            return true;
        }
        const std::size_t a = body_at + d.start(), b = body_at + d.end();
        const std::size_t line_end = text_in.find('\n', b);
        const std::string_view rest_of_line =
            text_in.substr(b, line_end == std::string_view::npos ? std::string_view::npos : line_end - b);
        if (has(rest_of_line, "(a handle"))
            return false; // (a pointer's handle: not a plain integer)
        const std::size_t k = static_cast<std::size_t>(std::find(names.begin(), names.end(), mem) - names.begin());
        std::vector<std::string> parts; // (in their order: an initializer gives them so)
        if (k != 0)
            parts.push_back(std::format(
                "{} {};", d.group(1),
                text::join(std::vector<std::string>(names.begin(), names.begin() + static_cast<std::ptrdiff_t>(k)),
                           ", ")));
        parts.push_back("intptr_t " + std::string(mem) + ";");
        if (k + 1 < names.size())
            parts.push_back(std::format(
                "{} {};", d.group(1),
                text::join(std::vector<std::string>(names.begin() + static_cast<std::ptrdiff_t>(k) + 1, names.end()),
                           ", ")));
        result = std::string(text_in.substr(0, a)) + text::join(parts, " ") + std::string(text_in.substr(b));
        return false;
    });
    return result;
}

} // namespace cb::port
