// cb-unity: alone (its helpers share names with old_c_passes.cpp's)
// The passes of mend.hpp that work on the whole program rather than on a complaint: parameters and members that
// carried a pointer, the union a value belongs in, the tags a file sees.
#include <algorithm>
#include <format>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/csource.hpp"
#include "cb/port/mend.hpp"

namespace cb::port {

using base::ctext::blank;
using csource::definitions_any;
using csource::FunctionSpan;

namespace {

bool is_word(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_';
}

std::string lines_joined(const std::vector<std::string>& lines, std::size_t first, std::size_t last,
                         std::string_view sep) {
    std::string out;
    for (std::size_t i = first; i <= last && i < lines.size(); ++i) {
        if (i != first)
            out += sep;
        out += lines[i];
    }
    return out;
}

// The tags of the structures a variable is declared a pointer to: `struct tag *var`. `strict` takes the variable
// right after the star; `listed` also the later ones of a list (struct tag *a, *b, c;), as the old text read them.
struct PointerVariables {
    std::map<std::string, std::set<std::string>> strict;
    std::map<std::string, std::set<std::string>> listed;
};

PointerVariables pointer_variables(std::string_view code) {
    static const Regex struct_star(R"(\bstruct\s+(\w+)\s*\*\s*)");
    PointerVariables out;
    struct_star.for_each_match(code, [&](const RegexMatch& m) {
        const std::string tag(m.group(1));
        std::size_t i = m.end();
        bool first = true;
        for (;;) {
            std::size_t j = i;
            while (j < code.size() && is_word(code[j]))
                ++j;
            if (j == i)
                break;
            const std::string name(code.substr(i, j - i));
            while (j < code.size() && text::is_space(code[j]))
                ++j;
            if (j >= code.size() || std::string_view(";,=)").find(code[j]) == std::string_view::npos)
                break;
            out.listed[name].insert(tag);
            if (first)
                out.strict[name].insert(tag);
            if (code[j] != ',')
                break;
            first = false;
            ++j;
            while (j < code.size() && text::is_space(code[j]))
                ++j;
            if (j < code.size() && code[j] == '*')
                ++j;
            while (j < code.size() && text::is_space(code[j]))
                ++j;
            i = j;
        }
        return true;
    });
    return out;
}

void note(Notes& notes, std::string_view why, std::string where) {
    notes[std::string(why)].push_back(std::move(where));
}

} // namespace

// ---- passed_on ----------------------------------------------------------------------------------------

bool passed_on(MendState& st, Sources& out) {
    static const Regex intptr_param(R"(intptr_t\s+\w+)");
    static const Regex int_param(R"((?:register\s+)?(?:u?int16_t|int|unsigned|short)\s+(\w+))");

    std::map<std::string, std::set<std::size_t>> wide; // function: the parameters that hold a pointer whole
    for (const auto& [name, fn] : st.functions) {
        for (std::size_t k = 0; k < fn.params.size(); ++k) {
            if (intptr_param.full_match(text::trim(fn.params[k].type)))
                wide[name].insert(k);
        }
    }
    if (wide.empty())
        return false;
    // The one pattern built from the program: the names of those functions, once per call.
    std::string names;
    for (const auto& entry : wide)
        names += (names.empty() ? "" : "|") + csource::regex_escape(entry.first);
    const Regex call(R"(\b()" + names + R"()\s*\()");

    bool changed = false;
    for (const auto& [file, text] : out) {
        if (!file.ends_with(".c"))
            continue;
        const std::vector<std::string> code = csource::split_lines(blank(text));
        for (const FunctionSpan& def : definitions_any(code)) {
            const auto it = st.functions.find(def.name);
            if (it == st.functions.end())
                continue;
            Function& me = it->second;
            std::map<std::string, std::size_t> mine; // an integer parameter: its place
            for (std::size_t i = 0; i < me.params.size(); ++i) {
                const auto m = int_param.full_match(text::trim(me.params[i].type));
                if (m && m->group(1) == me.params[i].name)
                    mine[me.params[i].name] = i;
            }
            if (mine.empty())
                continue;
            for (std::size_t x = def.open; x <= def.last && x < code.size(); ++x) {
                call.for_each_match(code[x], [&](const RegexMatch& m) {
                    const std::string callee(m.group(1));
                    const auto spans = csource::call_args(std::string_view(code[x]).substr(m.start()), callee);
                    if (!spans)
                        return true;
                    for (std::size_t k = 0; k < spans->size(); ++k) {
                        const std::string arg(text::trim(std::string_view(code[x]).substr(
                            m.start() + (*spans)[k].start, (*spans)[k].end - (*spans)[k].start)));
                        const auto place = mine.find(arg);
                        if (wide[callee].contains(k) && place != mine.end()) {
                            const std::size_t i = place->second;
                            mine.erase(place);
                            me.params[i] = {arg, "intptr_t " + arg};
                            st.param_types[{me.name, i}] = me.params[i].type;
                            note(st.mended,
                                 "integer parameter given on to one that holds a pointer (it carried one): intptr_t",
                                 std::format("{}:{} ({}, {})", file, x + 1, me.name, arg));
                            changed = true;
                        }
                    }
                    return true;
                });
            }
        }
    }
    return changed;
}

// ---- stored_wide --------------------------------------------------------------------------------------

bool stored_wide(MendState& st, Sources& out, const Rules& rules) {
    static const Regex intptr_param(R"(intptr_t\s+\w+)");
    static const Regex declared_in_head(R"(\bintptr_t\s+(\w+)\s*[,);])");
    static const Regex member_store(R"(\b(\w+)\s*->\s*(\w+)\s*=\s*(\w+)\s*;)");
    static const Regex cast_store(R"((?<![\w.>])(\w+)\s*->\s*(\w+)\s*=\s*\(\s*intptr_t\s*\))");
    static const Regex return_name(R"(\breturn\s+(\w+)\s*;)");
    static const Regex odd_return(R"(\breturn\s+[^\w\s;]|\breturn\s+\w+\s*[^\w\s;])");
    static const Regex int_return(R"(\bint\b)");

    std::set<std::pair<std::string, std::string>> wide; // (tag, member) given an intptr_t parameter
    for (const auto& [f, t] : out) {
        if (!f.ends_with(".c"))
            continue;
        const std::string code = blank(t);
        const std::vector<std::string> code_lines = csource::split_lines(code);
        const PointerVariables pointers = pointer_variables(code);
        const auto tag_of = [](const std::map<std::string, std::set<std::string>>& table,
                               std::string_view var) -> std::optional<std::string> {
            const auto it = table.find(std::string(var));
            if (it == table.end() || it->second.size() != 1)
                return std::nullopt;
            return *it->second.begin();
        };
        for (const FunctionSpan& def : definitions_any(code_lines)) {
            const auto it = st.functions.find(def.name);
            if (it == st.functions.end())
                continue;
            const Function& me = it->second;
            const std::string head = lines_joined(code_lines, def.first, def.open, " "); // (the definition's own say)
            std::set<std::string> carried;
            for (const Param& p : me.params) {
                if (intptr_param.full_match(text::trim(p.type)))
                    carried.insert(p.name);
            }
            declared_in_head.for_each_match(head, [&](const RegexMatch& m) {
                carried.emplace(m.group(1));
                return true;
            });
            const std::string body = lines_joined(code_lines, def.open, def.last, "\n");
            member_store.for_each_match(body, [&](const RegexMatch& m) {
                if (carried.contains(std::string(m.group(3)))) {
                    if (auto tag = tag_of(pointers.strict, m.group(1)))
                        wide.emplace(*tag, std::string(m.group(2)));
                }
                return true;
            });
        }
        if (rules.on("stored_wide_cast")) { // (p->s6 = (int)name: a pointer the old code kept in a member: its width)
            cast_store.for_each_match(code, [&](const RegexMatch& m) {
                if (auto tag = tag_of(pointers.listed, m.group(1)))
                    wide.emplace(*tag, std::string(m.group(2)));
                return true;
            });
        }
    }

    bool changed = false;
    for (const auto& [tag, member] : wide) { // (in the port's own text: each round is made from it)
        for (auto& [f, t] : st.files) {
            if (t.empty())
                continue;
            const auto widened = widen_member(t, tag, member);
            if (!widened)
                continue;
            t = *widened;
            if (const std::string* current = out.find(f); current != nullptr && !current->empty()) {
                if (auto widened_out = widen_member(*current, tag, member))
                    out[f] = *widened_out;
            }
            note(st.mended, "a member given an intptr_t parameter (the pointer it carried kept there): intptr_t",
                 std::format("{} (struct {}: {})", f, tag, member));
            changed = true;
        }
    }

    std::vector<std::string> members_taken; // (and taken out again: v = s->v; return v;)
    for (const auto& entry : wide)
        members_taken.push_back(csource::regex_escape(entry.second));
    std::sort(members_taken.begin(), members_taken.end());
    members_taken.erase(std::unique(members_taken.begin(), members_taken.end()), members_taken.end());
    if (members_taken.empty())
        return changed;
    const std::string member_alternatives = text::join(members_taken, "|");

    std::vector<std::string> files;
    for (const auto& entry : out)
        files.push_back(entry.first);
    for (const std::string& f : files) {
        if (!f.ends_with(".c"))
            continue;
        // (read afresh: an earlier file's mending may have changed this one's declarations)
        std::vector<std::string> lines = csource::split_lines(out.at(f));
        const std::vector<std::string> code = csource::split_lines(blank(out.at(f)));
        std::vector<FunctionSpan> defs = definitions_any(code);
        std::stable_sort(defs.begin(), defs.end(),
                         [](const FunctionSpan& a, const FunctionSpan& b) { return a.first > b.first; });
        for (const FunctionSpan& def : defs) {
            const auto it = st.functions.find(def.name);
            if (it == st.functions.end())
                continue;
            Function& me = it->second;
            std::string ret(text::trim(me.ret.empty() ? std::string_view("int") : std::string_view(me.ret)));
            if (ret != "int" && ret != "int16_t" && ret != "short" && ret != "unsigned" && ret != "uint16_t")
                continue;
            const std::string body = lines_joined(code, def.open, def.last, "\n");
            std::set<std::string> returned;
            return_name.for_each_match(body, [&](const RegexMatch& m) {
                returned.emplace(m.group(1));
                return true;
            });
            if (returned.size() != 1 || odd_return.test(body))
                continue;
            const std::string v = *returned.begin();
            if (!csource::name_regex(R"(\b@@\s*=\s*\w+\s*->\s*(?:)" + member_alternatives + R"()\s*;)", v).test(body))
                continue;
            const Regex local_decl = csource::name_regex(R"(^(\s*)(?:u?int16_t|int|short|unsigned)(\s+@@\s*;))", v);
            for (std::size_t x = def.open; x <= def.last && x < lines.size(); ++x)
                lines[x] = local_decl.replace_all(lines[x], "$1intptr_t$2");
            me.ret = "intptr_t";
            me.ret_set = true;
            st.local_retypes[{f, def.name, v}] = "intptr_t";
            const Regex head_decl =
                csource::name_regex(R"(^(\s*(?:static\s+)?)(?:u?int16_t|int|short|unsigned)(\s+@@\s*\())", def.name);
            for (std::size_t x = def.first; x <= def.open && x < lines.size(); ++x)
                lines[x] = head_decl.replace_all(lines[x], "$1intptr_t$2");
            out[f] = csource::join_lines(lines);
            // (its declarations)
            const Regex declaration = csource::name_regex(
                R"((?m)^(\s*(?:extern\s+|static\s+)?)(?:u?int16_t|int|short|unsigned)(\s+@@\s*\())", def.name);
            for (auto& [g, tg] : out) {
                if (tg.empty() || g == f)
                    continue;
                std::string ng = declaration.replace_all(tg, "$1intptr_t$2");
                if (ng != tg)
                    tg = std::move(ng);
            }
            lines = csource::split_lines(out.at(f));
            note(st.mended, "a function giving back what a member kept of an intptr_t parameter: intptr_t",
                 std::format("{} ({}: {})", f, def.name, v));
            changed = true;
        }
    }
    return changed;
}

// ---- union_member_for ---------------------------------------------------------------------------------

std::optional<std::string> union_member_for(const Sources& out, std::string_view union_type, std::string_view type) {
    static const Regex nested_braces(R"(\{[^{}]*\})");
    static const Regex tagged(R"((?:struct|union)\s+\w+$)");

    const std::vector<std::string_view> words = text::split_ws(union_type);
    if (words.empty())
        return std::nullopt;
    const std::string_view tag = words.back();
    std::optional<std::string> body;
    for (const auto& entry : out) {
        const std::string code = blank(entry.second);
        const std::size_t open = csource::find_tag_body(code, "union", tag);
        if (open == std::string::npos)
            continue;
        int depth = 1;
        std::size_t k = open;
        while (k < code.size() && depth != 0) {
            depth += (code[k] == '{') - (code[k] == '}');
            ++k;
        }
        body = code.substr(open, (k > open ? k - 1 : open) - open);
        break;
    }
    if (!body)
        return std::nullopt;
    const std::string flat = nested_braces.replace_all(*body, ""); // (its own members: not those of a structure in it)
    std::vector<std::pair<std::string, std::string>> members;      // (name, its type with the spaces gone)
    for (std::string_view decl : text::split(flat, ';')) {
        if (text::trim(decl).empty())
            continue;
        const auto r = csource::declarators(text::trim(decl));
        if (!r || r->declarators.size() != 1)
            continue;
        const std::string base(text::trim(r->base));
        if (base.empty())
            continue;
        const bool plain = !base.starts_with("struct") && !base.starts_with("union");
        if (!(plain || tagged.match(base) || r->declarators[0].text.find('*') != std::string::npos))
            continue;
        const std::string& name = r->declarators[0].name;
        std::string type_text = csource::collapse_space(r->base);
        type_text = std::string(text::trim(type_text)) + " " + text::replace_all(r->declarators[0].text, name, "");
        members.emplace_back(name, text::replace_all(type_text, " ", ""));
    }
    std::string t = text::replace_all(text::replace_all(type, " ", ""), "const", "");
    static const std::map<std::string, std::string, std::less<>> compilers_words = {
        {"long", "int32_t"},         {"longint", "int32_t"},           {"int", "int32_t"},
        {"unsignedint", "uint32_t"}, {"unsigned", "uint32_t"},         {"short", "int16_t"},
        {"shortint", "int16_t"},     {"shortunsignedint", "uint16_t"}, {"unsignedshort", "uint16_t"},
        {"float", "double"},         {"unsignedlong", "uint32_t"}}; // (today's int is 32 bits)
    if (const auto it = compilers_words.find(t); it != compilers_words.end())
        t = it->second;
    for (const auto& [name, member_type] : members) {
        if (member_type == t)
            return name;
    }
    const auto first_of = [&](auto&& wanted) -> std::optional<std::string> {
        for (const auto& [name, member_type] : members) {
            if (wanted(member_type))
                return name;
        }
        return std::nullopt;
    };
    if (t.find('*') != std::string::npos) {
        if (auto m = first_of([](const std::string& mt) { return mt == "char*"; }))
            return m;
        return first_of([](const std::string& mt) { return mt.find('*') != std::string::npos; });
    }
    if (t == "double" || t == "float" || t == "longdouble") {
        return first_of([](const std::string& mt) { return mt == "double" || mt == "float"; });
    }
    if (t == "int32_t" || t == "uint32_t" || t == "longlong" || t == "int64_t" || t == "intptr_t") {
        return first_of([](const std::string& mt) { return mt == "int32_t" || mt == "uint32_t"; });
    }
    return first_of([](const std::string& mt) {
        return mt == "int16_t" || mt == "uint16_t" || mt == "char" || mt == "unsignedchar";
    });
}

// ---- visible_tags -------------------------------------------------------------------------------------

std::set<std::string, std::less<>> visible_tags(const Sources& out, std::string_view file) {
    static const Regex tag_definition(R"(\b(?:struct|union)\s+(\w+)\s*\{)");
    static const Regex include(R"re(#\s*include\s*"([^"]+)")re");
    std::set<std::string> seen;
    std::vector<std::string> todo{std::string(file)};
    std::set<std::string, std::less<>> tags;
    while (!todo.empty()) {
        const std::string n = std::move(todo.back());
        todo.pop_back();
        const std::string* text = out.find(n);
        if (seen.contains(n) || text == nullptr)
            continue;
        seen.insert(n);
        tag_definition.for_each_match(blank(*text), [&](const RegexMatch& m) {
            tags.emplace(m.group(1));
            return true;
        });
        include.for_each_match(*text, [&](const RegexMatch& m) {
            todo.emplace_back(m.group(1));
            return true;
        });
    }
    return tags;
}

} // namespace cb::port
