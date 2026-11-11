// cb/port/old_c_agree.cpp -- agree_globals, joins_next (old_c_data.hpp).

#include <algorithm>
#include <format>
#include <set>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_c_data.hpp"
#include "cb/port/old_c_scan.hpp"

namespace cb::port::pre_ansi {

namespace {

using base::ctext::blank;

// `extern int g;` -- the declaration of a variable whose type another file may say differently.
const Regex& extern_number_decl() {
    static const Regex re(R"re((?m)^(\s*extern\s+)((?:unsigned|int|short|unsigned\s+int)\s+)(\w+)\s*;)re");
    return re;
}

// The type a pointer variable is declared with somewhere: `extern struct s *g;`, `extern char *a, *b;`, `FILE *g = 0;`.
std::map<std::string, std::string, std::less<>> pointer_variables(const Files& files, std::set<std::string, std::less<>>& declared_extern) {
    static const Regex extern_ptr(R"re((?m)^\s*extern\s+((?:const\s+)?(?:struct\s+\w+|union\s+\w+|\w+)\s*\*+)\s*(\w+)\s*;)re");
    static const Regex extern_ptr_list(
        R"re((?m)^\s*extern\s+((?:struct\s+\w+|union\s+\w+|(?:unsigned\s+)?char|int))\s+(\*\s*\w+(?:\s*,\s*\*\s*\w+)+)\s*;)re");
    static const Regex defined_ptr(
        R"re((?m)^((?:const\s+)?(?:struct\s+\w+|union\s+\w+|(?!static\b|extern\b|return\b)\w+)\s*\*+)\s*(\w+)\s*(?:=[^;]*)?;)re");

    std::map<std::string, std::string, std::less<>> ptrs;   // lookup only
    for (const auto& [name, text] : files) {
        const std::string code = blank(text);
        extern_ptr.for_each_match(code, [&](const RegexMatch& m) {
            ptrs.try_emplace(std::string(m.group(2)), pointer_type_text(m.group(1)));
            return true;
        });
        if (rule_enabled("agree_ptr_lists")) {   // (extern char *a, *b;)
            extern_ptr_list.for_each_match(code, [&](const RegexMatch& m) {
                for (const std::string_view piece : text::split(m.group(2), ',')) {
                    std::string var(piece);
                    var.erase(std::remove(var.begin(), var.end(), '*'), var.end());
                    ptrs.try_emplace(std::string(text::trim(var)), collapse_space(m.group(1)) + " *");
                }
                return true;
            });
        }
    }
    for (const auto& kv : ptrs) declared_extern.insert(kv.first);   // (what files declare extern: the program's variables)
    for (const auto& [name, text] : files) {                        // (and its definition's: FILE *g = 0;)
        defined_ptr.for_each_match(blank(text), [&](const RegexMatch& m) {
            ptrs.try_emplace(std::string(m.group(2)), pointer_type_text(m.group(1)));
            return true;
        });
    }
    return ptrs;
}

std::string declaration_of(const std::string& type, const std::string& name) {
    return type + (type.ends_with('*') ? "" : " ") + name;
}

// `extern int f, g;` where g is a pointer: a declaration of its own for each pointer.
void split_extern_lists(Files& files, const std::map<std::string, std::string, std::less<>>& ptrs) {
    static const Regex many(
        R"re((?m)(?:^|(?<=;))([ \t]*extern\s+)((?:unsigned|int|short|unsigned\s+int)\s+)(\w+(?:\s*,\s*\w+)+)\s*;)re");
    for (const std::string& n : sorted_names(files)) {
        std::string t = files.at(n);
        std::vector<Edit> edits;
        const std::string code = blank(t);
        many.for_each_match(code, [&](const RegexMatch& m) {
            std::vector<std::string> names, rest, pointers;
            for (const std::string_view w : text::split(m.group(3), ',')) names.emplace_back(text::trim(w));
            for (const std::string& w : names) (ptrs.contains(w) ? pointers : rest).push_back(w);
            if (pointers.empty()) return true;
            std::vector<std::string> lines;
            if (!rest.empty()) lines.push_back(std::string(m.group(1)) + std::string(m.group(2)) + text::join(rest, ", "));
            for (const std::string& w : names)
                if (ptrs.contains(w)) lines.push_back(std::string(m.group(1)) + std::string(m.group(2)) + w);
            edits.push_back({m.start(), m.end(), text::join(lines, ";\n") + ";"});
            return true;
        });
        files.insert_or_assign(n, apply_edits(std::move(t), std::move(edits)));
    }
}

// A number a variable of the program that other files declare a pointer is given as: 0xffff (the top of the segment).
std::string top_of_memory(std::string text, const std::map<std::string, std::string>& chosen) {
    static const Regex top(R"re(\b(\w+)(\s*=\s*)(?:0[xX][fF]{4}|65535)\b)re");
    return top.replace_all(text, [&](const RegexMatch& m) {
        const auto it = chosen.find(std::string(m.group(1)));
        if (it == chosen.end()) return std::string(m.whole());
        return std::string(m.group(1)) + std::string(m.group(2)) + "(" + it->second + ")UINTPTR_MAX";
    });
}

void agree_pointers(ProgramState& program, const std::map<std::string, std::string, std::less<>>& ptrs,
                    const std::set<std::string, std::less<>>& declared_extern) {
    static const Regex int_definition(R"re((?m)^(?:unsigned\s+int|unsigned|int|short)\s+(\w+)\s*(=\s*0\s*)?;)re");
    Files& files = program.files;
    for (const std::string& n : sorted_names(files)) {
        std::string t = files.at(n);
        std::map<std::string, std::string> chosen;
        std::vector<std::string> names;
        extern_number_decl().for_each_match(blank(t), [&](const RegexMatch& m) {
            const std::string name(m.group(3));
            if (const auto it = ptrs.find(name); it != ptrs.end()) {
                names.push_back(name);
                chosen[name] = it->second;
            }
            return true;
        });
        if (!names.empty()) {
            t = extern_number_decl().replace_all(t, [&](const RegexMatch& m) {
                const auto it = chosen.find(std::string(m.group(3)));
                if (it == chosen.end()) return std::string(m.whole());
                return std::string(m.group(1)) + declaration_of(it->second, it->first) + ";";
            });
            t = top_of_memory(std::move(t), chosen);
            for (const std::string& name : names) program.lists.agreed.push_back(std::format("{} ({})", n, name));
        }

        // an old-style definition's parameter lines (`int v;` between `f(v)` and its `{`) are no definitions
        std::set<std::size_t> head_lines;
        if (rule_enabled("agree_ptr_defs") && n.ends_with(".c"))
            for (const Definition& d : definitions(split_lines(t), split_lines(blank(t))))
                for (std::size_t line = d.first; line <= d.open; ++line) head_lines.insert(line);
        if (rule_enabled("agree_ptr_defs")) {   // (and its definition: `int g_0c6a = 0;` -- `extern char *g_0c6a;` elsewhere)
            const std::string code = blank(t);
            std::vector<Edit> edits;
            std::vector<std::string> placed;
            int_definition.for_each_match(code, [&](const RegexMatch& m) {
                const std::string name(m.group(1));
                if (!declared_extern.contains(name) || head_lines.contains(line_of(code, m.start()))) return true;
                edits.push_back({m.start(), m.end(),
                                 declaration_of(ptrs.find(name)->second, name) + ";" + (m.has(2) ? " /* (= 0) */" : "")});
                placed.push_back(name);
                return true;
            });
            t = apply_edits(std::move(t), std::move(edits));
            std::reverse(placed.begin(), placed.end());   // (the notes are in the order the edits were made: last first)
            for (const std::string& name : placed) program.lists.agreed.push_back(std::format("{} ({})", n, name));
        }
        files.insert_or_assign(n, std::move(t));
    }
}

// The value of a constant in a definition (`0x1f`, `-5`): its low 16 bits, and the number as it reads in decimal.
struct Constant {
    std::uint16_t low16 = 0;
    std::string decimal;
};
std::optional<Constant> read_constant(std::string_view text) {
    const bool negative = text.starts_with('-');
    const auto magnitude = c_integer(negative ? text.substr(1) : text);
    if (!magnitude) return std::nullopt;
    const std::uint64_t value = negative ? 0 - *magnitude : *magnitude;
    return Constant{static_cast<std::uint16_t>(value & 0xffff), (negative ? "-" : "") + std::to_string(*magnitude)};
}

void agree_longs(ProgramState& program) {
    static const Regex extern_long(R"re((?m)^\s*extern\s+((?:unsigned\s+)?long)\s+([\w\s,]+);)re");
    static const Regex low_word(
        R"re((?m)^((?:unsigned\s+int|unsigned|int|short))\s+(\w+)\s*(=\s*([^;,]+))?;([^\n]*))re");
    static const Regex high_word(
        R"re(\n(?:unsigned\s+int|unsigned|int|short)\s+(g_[0-9a-f]{4})\s*(?:=\s*([^;,]+))?;[^\n]*)re");
    static const Regex place_name(R"re(g_[0-9a-f]{4})re");

    Files& files = program.files;
    std::map<std::string, std::string, std::less<>> longs;   // (extern long g_0cb0; -- its definition `int g_0cb0 = 0;`
    if (rule_enabled("agree_longs")) {                       //  and `int g_0cb2 = 0;`, the two words of the long)
        for (const auto& kv : files) {
            extern_long.for_each_match(blank(kv.second), [&](const RegexMatch& m) {
                for (const std::string_view w : text::split(m.group(2), ',')) {
                    const std::string_view name = text::trim(w);
                    if (!name.empty() && std::all_of(name.begin(), name.end(), is_word_char)) longs.try_emplace(std::string(name), std::string(m.group(1)));
                }
                return true;
            });
        }
    }
    std::map<std::string, std::string> blanked;   // (the code of each file, as it is now)
    auto code_of = [&](const std::string& file) -> const std::string& {
        auto it = blanked.find(file);
        if (it == blanked.end()) it = blanked.emplace(file, blank(files.at(file))).first;
        return it->second;
    };
    for (const std::string& n : sorted_names(files)) {
        std::string t = files.at(n);
        const std::string code = blank(t);
        std::vector<Edit> edits;
        std::vector<std::string> names;
        low_word.for_each_match(code, [&](const RegexMatch& m) {
            const std::string name(m.group(2));
            if (!longs.contains(name)) return true;
            const std::string_view lo_text = m.has(4) ? text::trim(m.group(4)) : std::string_view("0");
            const auto lo = read_constant(lo_text);
            if (!lo) return true;
            const std::string after = std::string(m.group(1)).find("unsigned") != std::string::npos ? "unsigned long" : longs.find(name)->second;
            std::uint64_t value = lo->low16;
            std::size_t end = m.end();
            std::string next_name;
            if (place_name.full_match(name)) next_name = std::format("g_{:04x}", *text::parse_int<std::uint32_t>(name.substr(2), 16) + 2);
            if (const auto hi = high_word.match(code, m.end()); hi && hi->group(1) == next_name) {
                bool elsewhere = false;   // (the high word, no other file's: in the long now)
                for (const auto& kv : files)
                    if (kv.first != n && find_word(code_of(kv.first), next_name) != std::string::npos) elsewhere = true;
                if (!elsewhere) {
                    const auto hv = read_constant(hi->has(2) ? text::trim(hi->group(2)) : std::string_view("0"));
                    if (hv) {
                        value |= static_cast<std::uint64_t>(hv->low16) << 16;
                        end = hi->end();
                    }
                }
            }
            edits.push_back({m.start(), end,
                             std::format("{} {} = {};{}", after, name, value > 0xffff ? std::format("0x{:x}L", value) : lo->decimal, m.group(5))});
            names.push_back(name);
            return true;
        });
        if (edits.empty()) continue;
        files.insert_or_assign(n, apply_edits(std::move(t), std::move(edits)));
        blanked.erase(n);
        program.lists.longs.push_back(std::format("{} ({})", n, text::join(names, ", ")));
    }
}

}  // namespace

void agree_globals(ProgramState& program) {
    std::set<std::string, std::less<>> declared_extern;
    const auto ptrs = pointer_variables(program.files, declared_extern);
    program.lists.agreed.clear();
    split_extern_lists(program.files, ptrs);
    agree_pointers(program, ptrs, declared_extern);
    agree_longs(program);
}

bool joins_next(const Files& files, std::string_view table) {
    // a table defined with an initializer, followed by a definition of the same type that has one (a variable or table)
    static const Regex definition(
        R"re((?m)^((?:unsigned\s+|signed\s+)?(?:char|short|int|long|unsigned))\s+(\w+)\s*\[\s*\d*\s*\]\s*=[^;]*;[^\n]*\n\1\s+\w+\s*(?:\[\s*\d*\s*\])?\s*=)re");
    for (const auto& kv : files) {
        const std::string& text = kv.second;
        if (find_word(text, table) == std::string::npos) continue;
        // the definition must be at the start of a line and be the table's: try each line start (a search over any
        // table's would let another definition's match hide this one)
        std::size_t line = 0;
        while (line <= text.size()) {
            if (const auto m = definition.match(text, line); m && m->group(2) == table) return true;
            const std::size_t next = text.find('\n', line);
            if (next == std::string::npos) break;
            line = next + 1;
        }
    }
    return false;
}

}  // namespace cb::port::pre_ansi
