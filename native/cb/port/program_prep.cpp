#include "cb/port/program_prep.hpp"

#include <algorithm>
#include <format>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/asm16.hpp"
#include "cb/port/old_c_scan.hpp"
#include "cb/port/struct_layout.hpp"
#include "cb/port/union_views.hpp"
#include "cb/port/views.hpp"
#include "cb/port/views_internal.hpp"

namespace cb::port::pre_ansi {

using base::ctext::blank;
using namespace views_detail;

std::optional<std::string> runtime_left_out(std::string_view name, std::string_view text, const Profile& profile) {
    if (name.ends_with(".h") && std::find(profile.runtime_headers.begin(), profile.runtime_headers.end(), name) != profile.runtime_headers.end())
        return "the old C library's header: the C library of today has its own";
    if (name.ends_with(".c")) {
        std::vector<std::string> got;   // (what the program defines that the library defines)
        for (const Definition& d : definitions(split_lines(text), split_lines(blank(text))))
            if (std::find(profile.runtime_defines.begin(), profile.runtime_defines.end(), d.fn.name) != profile.runtime_defines.end())
                got.push_back(d.fn.name);
        if (!got.empty()) return "the old C library (it defines " + text::join(got, ", ") + "): the C library of today stands in";
    }
    return std::nullopt;
}

namespace {

// The members' corrections: {"struct.member": type} -- `struct s { ... x; ... }` gets the member's declaration written
// with the type the program uses it as.
void pp_correct_members(Files& files, const OrderedMap<std::string, std::string>& members, std::vector<std::string>& member_fixes) {
    for (const auto& [key, type] : members) {
        const std::size_t dot = key.find('.');
        if (dot == std::string::npos) continue;
        const std::string st = key.substr(0, dot), mem = key.substr(dot + 1);
        for (std::size_t f = 0; f < files.size(); ++f) {
            const std::string n = files.entries()[f].first;
            const std::string t = files.at(n);
            const auto m = rx_name(R"(\bstruct\s+@@\s*\{)", st).search(t);
            if (!m) continue;
            const std::size_t end = t.find("};", m->end());
            if (end == std::string::npos) continue;   // (no closing of it: nothing to correct)
            const std::string body = t.substr(m->end(), end - m->end());
            const std::string neu = replace_first_with(rx_name(R"((?m)^([ \t]*)[^;{}\n]*?\b@@\s*;)", mem), body, [&](const RegexMatch& x) {
                return std::string(x.group(1)) + type + (type.ends_with('*') ? "" : " ") + mem + ";";
            });
            if (neu != body) {
                files.insert_or_assign(n, t.substr(0, m->end()) + neu + t.substr(end));
                member_fixes.push_back(std::format("{}: struct {}'s {} is {} (as the program uses it)", n, st, mem, type));
            }
        }
    }
}

}  // namespace

PreparedProgram prepare_program(const Files& input, const Profile& profile, const OrderedMap<std::string, std::string>& members) {
    PreparedProgram p;
    Files all = input;
    pp_correct_members(all, members, p.member_fixes);
    p.originals = all;   // (the source as it was: the old layouts, far ones)
    for (const auto& [n, t] : all) {
        if (const auto why = runtime_left_out(n, t, profile))
            p.left_out.insert_or_assign(n, *why);
        else
            p.files.insert_or_assign(n, t);
    }
    static const Regex stand_in(R"(/\* SAMEFILE \*/\n([^\n{;]*\([^)]*\))\s*\{\s*\})");
    static const Regex parameters(R"(\(.*\))");
    for (std::size_t f = 0; f < p.files.size(); ++f) {   // (empty stand-ins of functions of the original's same file, made for the byte-exact build: declarations)
        const std::string n = p.files.entries()[f].first;
        const std::string t = p.files.at(n);
        const std::string neu = stand_in.replace_all(t, [&](const RegexMatch& m) {
            return "extern " + std::string(text::trim(parameters.replace_first(m.group(1), "()"))) + ";";
        });
        if (neu != t) {
            p.files.insert_or_assign(n, neu);
            note(p.notes, "an empty stand-in of a function defined elsewhere (for the byte-exact build): declared instead", n);
        }
    }
    const std::set<std::string> internals(profile.runtime_internals.begin(), profile.runtime_internals.end());
    static const Regex library_structure(R"((?m)^[ \t]*struct\s+(\w+)\s*\{[^{}]*\}\s*;[^\n]*\n?)");
    for (std::size_t f = 0; f < p.files.size(); ++f) {   // (the old library's own structures, copied into the program's header: FILE's: today's library's)
        const std::string n = p.files.entries()[f].first;
        const std::string t = p.files.at(n);
        const std::string neu = library_structure.replace_all(t, [&](const RegexMatch& m) {
            return internals.contains(std::string(m.group(1))) ? std::string() : std::string(m.whole());
        });
        if (neu != t) {
            p.files.insert_or_assign(n, neu);
            note(p.union_notes, "the old library's own structure, copied into the program: today's library's instead", n);
        }
    }
    {
        const StructLayouts old_layouts = old_layouts_of(p.originals);
        std::vector<std::string> texts;
        for (const auto& kv : p.files) texts.push_back(kv.second);
        const StructLayouts new_layouts = lay_out_structs(texts, 8, 8);
        SizeChanges sizes;
        for (const auto& [k, lay] : old_layouts)
            if (!is_nested(k))
                if (const StructLayout* now = new_layouts.find(k)) sizes[k] = {lay.size, now->size};
        std::vector<std::string> names;
        for (const auto& kv : p.files) names.push_back(kv.first);
        std::sort(names.begin(), names.end());
        for (const std::string& n : names)   // (unions whose views meet at the old places: so still)
            p.files.insert_or_assign(n, union_views(p.files.at(n), p.union_notes, n, sizes));
    }
    // the structure passes of views.hpp, over this program (what the later passes need of them is not kept here)
    Notes mended;
    std::vector<std::string> view_wide, view_puns_refused;
    std::map<std::string, std::set<std::string>> need_headers;
    OrderedMap<std::string, Fn> fns;
    ParamTypes param_types;
    const BssSizes bss;
    ViewCaches caches;
    ViewContext cx{p.files, p.originals, p.notes, mended, p.aliases, p.view_vars, p.view_headers, view_wide, view_puns_refused,
                   need_headers, fns, param_types, bss, caches};
    anon_tags(cx);
    p.view_headers.clear();
    struct_views(cx);
    word_columns(cx);
    twin_structs(cx);
    cast_twins(cx);
    fnptr_returns(cx);
    link_words(cx);
    array_tails(cx);

    // (struct rfld *)&sym->u.rfl: a structure of its own laid over a view of the union: laid out as the view is
    static const Regex cast_over_view(R"(\(\s*struct\s+(\w+)\s*(?:far\s*|near\s*|huge\s*)?\*\s*\)\s*&[^;,)]*?\bu\.(\w+))");
    std::set<std::pair<std::string, std::string>> casts;
    for (const auto& kv : p.files)
        cast_over_view.for_each_match(blank(kv.second), [&](const RegexMatch& m) {
            casts.emplace(std::string(m.group(1)), std::string(m.group(2)));
            return true;
        });
    for (const auto& [tag, view] : casts) {
        std::optional<std::string> view_body;
        for (const auto& kv : p.files) {   // (tagged or not)
            const auto vm = rx_name(R"(\bstruct\s*(?:\w+\s*)?\{([^{}]*)\}\s*@@\s*;)", view).search(kv.second);
            if (vm && has(vm->group(1), "_old")) view_body = std::string(vm->group(1));
        }
        if (!view_body || view_body->empty()) continue;
        for (std::size_t f = 0; f < p.files.size(); ++f) {
            const std::string n = p.files.entries()[f].first;
            const std::string t = p.files.at(n);
            const auto sm = rx_name(R"(\bstruct\s+@@\s*\{[^{}]*\})", tag).search(t);
            if (!sm) continue;
            p.files.insert_or_assign(n, t.substr(0, sm->start()) + "struct " + tag + " {" + *view_body + "}" + t.substr(sm->end()));
            note(p.union_notes, "a structure laid over a view of a union (by a cast): laid out as the view is", std::format("{} (struct {}, u.{})", n, tag, view));
        }
    }
    // ((struct comsym *)sym)->count: a structure that is another's first bytes and then members of it
    static const Regex cast_member(R"(\(\s*\(\s*struct\s+(\w+)\s*(?:far\s*|near\s*|huge\s*)?\*\s*\)\s*(\w+)\s*\)\s*->)");
    std::set<std::pair<std::string, std::string>> over;
    for (const auto& kv : p.files) {
        const std::string code = blank(kv.second);
        cast_member.for_each_match(code, [&](const RegexMatch& m) {
            const auto d = rx_name(R"(\bstruct\s+(\w+)\s*(?:far\s*|near\s*|huge\s*)?\*\s*@@\b)", m.group(2)).search(code);
            if (d) over.emplace(std::string(m.group(1)), "struct " + std::string(d->group(1)));
            return true;
        });
    }
    if (!over.empty()) {
        asm16::CTypes types;
        types.old_layouts = old_layouts_of(p.originals);
        {
            std::vector<std::string> texts;
            for (const auto& kv : p.files) texts.push_back(kv.second);
            types.new_layouts = lay_out_structs(texts, 8, 8);
        }
        for (const auto& [tag, under] : over) {
            for (std::size_t f = 0; f < p.files.size(); ++f) {
                const std::string n = p.files.entries()[f].first;
                const std::string t = p.files.at(n);
                const std::string code_of_t = blank(t);
                const auto sm = rx_name(R"(\bstruct\s+@@\s*\{\s*char\s+(\w+)\s*\[\s*(0[xX][0-9a-fA-F]+|\d+)\s*\]\s*;)", tag).search(code_of_t);   // (its comments aside: the places are the text's)
                if (!sm) continue;
                const std::uint64_t k = c_integer(sm->group(2)).value_or(0);
                std::optional<asm16::Field> field;   // (the member of the other there)
                if (types.old_layouts.contains(under)) field = types.field(under, static_cast<std::int64_t>(k));
                if (field && static_cast<std::uint64_t>(field->old_offset) == k && static_cast<std::uint64_t>(field->new_offset) != k) {
                    p.files.insert_or_assign(n, t.substr(0, sm->start(2)) + std::to_string(field->new_offset) + t.substr(sm->end(2)));
                    note(p.union_notes, "a structure that is another's first bytes and then members of it: the bytes as many as the other's now",
                         std::format("{} (struct {} over {}: {})", n, tag, under, field->new_offset));
                }
            }
        }
    }
    static const Regex far_pointer(R"(FP_SEG\s*\(\s*(\w+)\s*\)\s*=\s*([^;]+);\s*FP_OFF\s*\(\s*\1\s*\)\s*=\s*([^;]+);)");
    for (std::size_t f = 0; f < p.files.size(); ++f) {   // (a far pointer made of a segment and an offset: the platform layer gives the place, as DOS had it)
        const std::string n = p.files.entries()[f].first;
        const std::string t = p.files.at(n);
        const std::string neu = far_pointer.replace_all(t, [](const RegexMatch& m) {
            return std::string(m.group(1)) + " = dos_far(" + std::string(text::trim(m.group(2))) + ", " + std::string(text::trim(m.group(3))) + ");";
        });
        if (neu != t) p.files.insert_or_assign(n, neu);
    }
    if (profile.keywords_removed && !profile.keywords_removed->empty()) {   // (far, near, pascal...: the 8086's segments and calling conventions, which a flat memory has not)
        const std::vector<std::string>& kw = *profile.keywords_removed;
        const std::set<std::string> kwset(kw.begin(), kw.end());
        std::set<std::string> macros;   // (a macro of those words alone: #define NEAR near pascal: gone too, and the word where it was used)
        static const Regex define_words(R"((?m)^[ \t]*#[ \t]*define[ \t]+(\w+)[ \t]+([\w \t]+?)[ \t]*$)");
        for (const auto& kv : p.files)
            define_words.for_each_match(kv.second, [&](const RegexMatch& m) {
                bool all_words = true;
                for (std::string_view w : text::split_ws(m.group(2))) all_words = all_words && kwset.contains(std::string(w));
                if (all_words) macros.emplace(m.group(1));
                return true;
            });
        const std::vector<std::string> sorted_macros(macros.begin(), macros.end());
        if (!macros.empty()) {
            const Regex define_line(R"((?m)^[ \t]*#[ \t]*define[ \t]+(?:)" + text::join(sorted_macros, "|") + R"()[ \t]+[\w \t]*\n)");
            for (std::size_t f = 0; f < p.files.size(); ++f) {
                const std::string n = p.files.entries()[f].first;
                p.files.insert_or_assign(n, define_line.replace_all(p.files.at(n), ""));
            }
        }
        std::vector<std::string> words;
        for (const std::string& w : kw) words.push_back(regex_quote(w));
        for (const std::string& w : sorted_macros) words.push_back(regex_quote(w));
        // (a qualifier: not a name of its own: char near, x3; p->far)
        const Regex qualifier(R"((?<!->)(?<!\.)\b(?:)" + text::join(words, "|") + R"()\b(?!\s*[,;=\[)])[ \t]*)");
        for (std::size_t f = 0; f < p.files.size(); ++f) {
            const std::string n = p.files.entries()[f].first;
            const std::string t = p.files.at(n);
            std::size_t count = 0;
            const std::string neu = base::ctext::sub_code(t, qualifier, [&](const RegexMatch&) {
                ++count;
                return std::string();
            });
            if (neu != t) {
                p.files.insert_or_assign(n, neu);
                p.far_near_dropped[n] += count;
            }
        }
    }
    p.renames = profile.platform_renames;   // (a function whose name the C library has: renamed; the platform's own)
    p.int16 = profile.int_storage && *profile.int_storage == 16;   // (every int 16 bits; else only where bytes are laid out)
    return p;
}

}  // namespace cb::port::pre_ansi
