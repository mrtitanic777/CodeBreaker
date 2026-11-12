#include "cb/port/views.hpp"

#include <algorithm>
#include <format>
#include <tuple>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_c_data.hpp"
#include "cb/port/old_c_passes.hpp"
#include "cb/port/old_c_scan.hpp"
#include "cb/port/views_internal.hpp"

namespace cb::port::pre_ansi {

using base::ctext::blank;
using namespace views_detail;

namespace {

// ---- reading ----------------------------------------------------------------------------------------------

// `int n, f4;` is `int n; int f4;`: the members of a structure's body, each as "type declarator", for comparing two
// structures' bodies.
std::string vw_normal_body(std::string_view body) {
    std::vector<std::string> members;
    const std::string code = blank(body);
    for (std::string_view piece : text::split(code, ';')) {
        const std::string_view d = text::trim(piece);
        if (d.empty()) continue;
        if (const auto r = declarators(d)) {
            for (const Declarator& dn : r->names) {
                std::string decl;
                for (char c : dn.text)
                    if (c != ' ') decl.push_back(c);
                members.push_back(squeeze(r->base) + " " + decl);
            }
        } else {
            std::string squashed;
            for (char c : piece)
                if (!text::is_space(c)) squashed.push_back(c);
            members.push_back(std::move(squashed));
        }
    }
    return text::join(members, ";");
}

// (struct tag, the member name) of a structure body, as the layout engine or a declaration reads them.
struct VwMember {
    std::string name, base, declarator;
};

struct VwBody {
    std::size_t begin = 0, end = 0;   // the body between the braces, in the file's text
    std::vector<VwMember> members;
};

// The members of `struct tag { ... }` in a file (size_t is a type word there): nullopt when the file has no such structure.
std::optional<VwBody> vw_members_in(const std::string& text, std::string_view tag) {
    const std::string code = blank(text);
    const auto m = rx_name(R"(\bstruct\s+@@\s*\{([^{}]*)\})", tag).search(code);
    if (!m) return std::nullopt;
    VwBody out;
    out.begin = m->start(1);
    out.end = m->end(1);
    for (std::string_view piece : text::split(std::string_view(code).substr(out.begin, out.end - out.begin), ';')) {
        const std::string_view d = text::trim(piece);
        if (d.empty()) continue;
        if (const auto r = declarators(d, true))
            for (const Declarator& dn : r->names) out.members.push_back({dn.name, squeeze(r->base), dn.text});
    }
    return out;
}

using VwSig = std::vector<std::pair<std::uint32_t, std::uint32_t>>;   // (offset, size) of each member

VwSig vw_offset_sig(const StructLayout& l) {
    VwSig s;
    for (const Member& m : l.members) s.emplace_back(m.offset, m.size);
    return s;
}

// The text with the brace-depth-zero code only (every byte inside a brace pair but the braces themselves a space).
std::string vw_outer_code(std::string_view code) {
    std::string out;
    int depth = 0;
    for (char c : code) {
        depth += c == '{';
        depth -= c == '}';
        out.push_back(depth == 0 || c == '{' || c == '}' ? c : ' ');
    }
    return out;
}

// The lines of the text (split at '\n') and back.
std::string vw_join(const std::vector<std::string>& lines) { return join_lines(lines); }

}  // namespace

// ================================================================ anon_tags

void anon_tags(ViewContext& cx) {
    struct Named {
        std::string tag, header;
    };
    std::map<std::string, Named> named;   // (a lookup only)
    static const Regex header_member(R"(\bstruct\s*\{([^{}]*)\}\s*(\w+)\s*;)");
    for (const auto& [h, t] : cx.files) {
        if (!h.ends_with(".h")) continue;
        std::vector<std::pair<std::string, std::string>> found;   // (the body, the member's name)
        header_member.for_each_match(t, [&](const RegexMatch& m) {
            found.emplace_back(std::string(m.group(1)), std::string(m.group(2)));
            return true;
        });
        for (auto it = found.rbegin(); it != found.rend(); ++it)   // (the last of a file first: it stands for the body)
            named.emplace(vw_normal_body(it->first), Named{stem_of(h) + "_" + it->second, h});
    }
    static const Regex tagged_member(R"(\bstruct\s*\{([^{}]*)\}(\s*)(\w+)\s*;)");
    for (auto& [h, t] : cx.files) {   // (the header's: tagged)
        if (!h.ends_with(".h")) continue;
        t = tagged_member.replace_all(t, [&](const RegexMatch& m) -> std::string {
            const auto it = named.find(vw_normal_body(m.group(1)));
            if (it == named.end() || it->second.header != h) return std::string(m.whole());
            return std::format("struct {} {{{}}}{}{};", it->second.tag, m.group(1), m.group(2), m.group(3));
        });
    }
    static const Regex anonymous(R"(\bstruct\s*\{([^{}]*)\}\s*(\w+)\s*;)");
    static const Regex anonymous_pointer(R"((?m)^(\s*(?:register\s+)?)struct\s*\{([^{}]*)\}\s*(\*+)\s*(\w+)\s*;)");
    for (auto& [n, t] : cx.files) {   // (a C file's parameter of the same: the tag)
        if (!n.ends_with(".c")) continue;
        t = anonymous.replace_all(t, [&](const RegexMatch& m) -> std::string {   // (a parameter's, a local's)
            const auto it = named.find(vw_normal_body(m.group(1)));
            if (it == named.end()) return std::string(m.whole());
            note(cx.notes, "an anonymous structure given by value, a header's member's too: one type, by a tag",
                 std::format("{} ({})", n, it->second.tag));
            return std::format("struct {} {};", it->second.tag, m.group(2));
        });
        std::vector<std::string> own;
        t = anonymous_pointer.replace_all(t, [&](const RegexMatch& m) -> std::string {   // (register struct { ... } *p; of a definition's parameters: a tag of the header's or its own)
            const auto it = named.find(vw_normal_body(m.group(2)));
            const std::string tag = it != named.end() ? it->second.tag : stem_of(n) + "_" + std::string(m.group(4));
            if (it == named.end()) own.push_back(std::format("struct {} {{{}}};", tag, m.group(2)));
            note(cx.notes, "an anonymous structure of a parameter: a tag (a prototype names it)", std::format("{} ({})", n, tag));
            return std::format("{}struct {} {}{};", m.group(1), tag, m.group(3), m.group(4));
        });
        if (!own.empty()) {
            std::vector<std::string> lines = split_lines(t);
            std::size_t at = 0;   // (after the last #include)
            for (std::size_t i = 0; i < lines.size(); ++i)
                if (lines[i].starts_with("#include")) at = i + 1;
            lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(at), own.begin(), own.end());
            t = vw_join(lines);
        }
    }
}

// ================================================================ array_tails

void array_tails(ViewContext& cx) {
    const StructLayouts lays = old_layouts_of(cx.originals);
    std::string allcode;
    {
        std::vector<std::string> blanked;
        for (const auto& kv : cx.files) blanked.push_back(blank(kv.second));
        allcode = text::join(blanked, "\n");
    }
    static const Regex structure(R"(\bstruct\s+(\w+)\s*\{([^{}]*)\})");
    static const Regex declaration(R"([^;{}]+;)");
    std::vector<std::string> names;
    for (const auto& kv : cx.files) names.push_back(kv.first);
    std::sort(names.begin(), names.end());
    for (const std::string& n : names) {
        std::string t = cx.files.at(n);
        std::size_t pos = 0;   // (each structure is read from the file as it is now: an earlier one may have changed its length)
        for (;;) {
            const std::string code = blank(t);
            const auto sm = structure.search(code, pos);
            if (!sm) break;
            pos = sm->end();
            const std::string tag(sm->group(1));
            const StructLayout* lay = lays.find("struct " + tag);
            if (!lay) continue;
            const std::string body = t.substr(sm->start(2), sm->end(2) - sm->start(2));
            struct Decl {
                std::size_t begin, end;
                std::string text;
            };
            std::vector<Decl> decls;
            declaration.for_each_match(body, [&](const RegexMatch& m) {
                decls.push_back({m.start(), m.end(), std::string(m.whole())});
                return true;
            });
            for (std::size_t k = 0; k + 1 < decls.size(); ++k) {
                const Decl &d1 = decls[k], &d2 = decls[k + 1];
                const std::string c1 = blank(d1.text), c2 = blank(d2.text);
                const auto r1 = declarators(text::trim(c1), true), r2 = declarators(text::trim(c2), true);
                if (!r1 || !r2 || r1->names.size() != 1 || r2->names.size() != 1) continue;
                const Declarator &array = r1->names[0], &word = r2->names[0];
                const auto shape = rx_name(R"((\**)\s*@@\s*\[\s*(\d+)\s*\])", array.name).full_match(text::trim(array.text));
                if (!shape || text::trim(word.text) != word.name) continue;
                const Member *ma = lay->find(array.name), *mw = lay->find(word.name);
                if (!ma || !mw) continue;
                const std::uint32_t count = text::parse_int<std::uint32_t>(shape->group(2)).value_or(0);
                const std::uint32_t esz = ma->size / std::max<std::uint32_t>(1, count);
                if (mw->offset != ma->offset + ma->size || mw->size != esz) continue;   // (not where, nor of the size, the next element is)
                if (rx_name(R"((?:->|\.)\s*@@\b)", word.name).search(allcode) ||
                    rx_name(R"((?m)^\s*#\s*define\s+\w+.*\b@@\b)", word.name).search(allcode))
                    continue;   // (the word named somewhere: a member of its own)
                const std::string only_within = std::format(R"((?:->|\.)\s*@@\s*\[\s*(?![0-{}]\s*\])[^\]]+\])", count == 0 ? 0 : count - 1);
                if (!rx_name(only_within, array.name).search(allcode)) continue;   // (the array only ever indexed within it)
                const std::string new_d1 =
                    Regex(std::format(R"(\[\s*{}\s*\])", count)).replace_first(d1.text, std::format("[{}]", count + 1));
                const std::string body2 = body.substr(0, d1.begin) + new_d1 + body.substr(d1.end, d2.begin - d1.end) +
                                          std::format("\t/* ({}: the word after it, its element {}) */", word.name, count) + body.substr(d2.end);
                t = t.substr(0, sm->start(2)) + body2 + t.substr(sm->end(2));
                cx.files.insert_or_assign(n, t);
                note(cx.notes, "a word after an array that its next element was (indexed past it, never named): the array one longer",
                     std::format("{} (struct {}: {}[{}], {})", n, tag, array.name, count, word.name));
                pos = sm->start(2) + body2.size() + 1;   // (the structure's end in the new text)
                break;
            }
        }
    }
}

// ================================================================ link_words

void link_words(ViewContext& cx) {
    static const Regex macro_def(R"((?m)^\s*#\s*define\s+(\w+)\s*\(\s*(\w+)\s*\)\s*\(?\s*\*\s*\(\s*struct\s+(\w+)\s*\*\s*\*\s*\)\s*\(?\s*\2\s*\)?\s*\)?\s*$)");
    static const Regex cast_use(R"(\*\s*\(\s*struct\s+(\w+)\s*\*\s*\*\s*\)\s*(\w+)\b)");
    OrderedMap<std::string, std::string> macros;   // (macro: the structure its link points to)
    for (const auto& kv : cx.files)
        macro_def.for_each_match(kv.second, [&](const RegexMatch& m) {
            macros.insert_or_assign(std::string(m.group(1)), std::string(m.group(3)));
            return true;
        });
    OrderedMap<std::string, std::set<std::string>> uses;   // (structure: the link's structure)
    for (const auto& kv : cx.files) {
        const std::string code = blank(kv.second);
        std::vector<std::pair<std::string, std::string>> found;   // (the variable, the macro or structure)
        for (const auto& mac : macros)
            rx_name(R"(\b(@@)\s*\(\s*(\w+)\s*\))", mac.first).for_each_match(code, [&](const RegexMatch& mm) {
                found.emplace_back(std::string(mm.group(2)), mac.first);
                return true;
            });
        cast_use.for_each_match(code, [&](const RegexMatch& mm) {
            found.emplace_back(std::string(mm.group(2)), std::string(mm.group(1)));
            return true;
        });
        for (const auto& [v, x] : found) {
            std::set<std::string> tags;
            rx_name(R"(\bstruct\s+(\w+)\s*\*\s*@@\s*[;,=)])", v).for_each_match(code, [&](const RegexMatch& m) {
                tags.emplace(m.group(1));
                return true;
            });
            if (tags.size() != 1) continue;
            const std::string* via = macros.find(x);
            uses[*tags.begin()].insert(via ? *via : x);
        }
    }
    const StructLayouts lays = old_layouts_of(cx.originals);
    using Sig = std::vector<std::tuple<std::string, std::uint32_t, std::uint32_t>>;
    auto sig = [](const StructLayout& l) {
        Sig s;
        for (const Member& m : l.members) s.emplace_back(m.name, m.offset, m.size);
        return s;
    };
    std::vector<std::pair<std::string, std::set<std::string>>> snapshot(uses.begin(), uses.end());   // (an isym4 { int s0; char kind, s3; int s4; }: an isym's first fields: its link alike)
    for (const auto& [y, xs] : snapshot) {
        const StructLayout* ylay = lays.find("struct " + y);
        if (xs.size() != 1 || !ylay) continue;
        const Sig whole = sig(*ylay);
        for (const auto& [k, lay] : lays) {
            if (is_nested(k) || !k.starts_with("struct ")) continue;
            const std::string z = k.substr(k.rfind(' ') + 1);
            const Sig part = sig(lay);
            if (z != y && !uses.contains(z) && !part.empty() && part.size() < whole.size() &&
                std::equal(part.begin(), part.end(), whole.begin()))
                uses.insert_or_assign(z, xs);
        }
    }
    std::vector<std::string> names;
    for (const auto& kv : cx.files) names.push_back(kv.first);
    std::sort(names.begin(), names.end());
    for (const auto& [y, xs] : uses) {
        if (xs.size() != 1) continue;
        const std::string& x = *xs.begin();
        for (const std::string& n : names) {
            std::string t = cx.files.at(n);
            const std::string code_of_t = blank(t);
            const auto m = rx_name(R"(\bstruct\s+@@\s*\{\s*((?:unsigned\s+|signed\s+)?(?:int|short|unsigned|int16_t|uint16_t))\s+(\w+)\s*;)", y).search(code_of_t);
            if (!m) continue;
            t = t.substr(0, m->start(1)) + std::format("struct {} *{};", x, m->group(2)) + "\t/* (the chain's link: NEXT) */" + t.substr(m->end());
            cx.files.insert_or_assign(n, t);
            note(cx.notes, "a structure's first word its chain's link (read as a pointer): a pointer",
                 std::format("{} (struct {}: {})", n, y, m->group(2)));
        }
    }
}

// ================================================================ fnptr_returns

void fnptr_returns(ViewContext& cx) {
    static const Regex pointer_function(
        R"((?m)^(?:static\s+)?((?:struct\s+|unsigned\s+)?\w+)\s*(\*+)\s*(\w+)\s*\(([\w\s,]*)\)\s*\n((?:[^{;]*;\s*)*)\{)");
    static const Regex old_definition(R"((?m)^(?:\w+\s+\**\s*|\w+\s*\*+\s*)?(\w+)\s*\(([\w\s,]*)\)\s*\n((?:[^{;]*;\s*)*)\{)");
    static const Regex function_parameter(R"((?m)^[ \t]*(?:register\s+)?(?:int\s+)?\(\s*\*\s*(\w+)\s*\)\s*\(\s*\)\s*;)");
    static const Regex word(R"(\w+)");
    std::map<std::string, std::set<std::string>> rets;   // (function: its return type, a pointer one) -- a lookup only
    struct Def {
        std::string file;
        std::size_t begin, end;
        std::string name;
    };
    FirstSeenMap<std::pair<std::string, std::size_t>, Def> defs;   // ((function, index): where its declaration is)
    for (const auto& [n, t] : cx.files) {
        const std::string code = blank(t);
        pointer_function.for_each_match(code, [&](const RegexMatch& m) {
            rets[std::string(m.group(3))].insert(std::string(m.group(1)) + " " + std::string(m.group(2)));
            return true;
        });
        old_definition.for_each_match(code, [&](const RegexMatch& m) {
            std::vector<std::string> names;
            for (std::string_view w : text::split(m.group(2), ',')) {
                const std::string_view s = text::trim(w);
                if (!s.empty()) names.emplace_back(s);
            }
            function_parameter.for_each_match(m.group(3), [&](const RegexMatch& d) {
                const auto at = std::find(names.begin(), names.end(), std::string(d.group(1)));
                if (at != names.end())
                    defs.assign({std::string(m.group(1)), static_cast<std::size_t>(at - names.begin())},
                                          Def{n, m.start(3) + d.start(), m.start(3) + d.end(), std::string(d.group(1))});
                return true;
            });
            return true;
        });
    }
    std::map<std::string, std::string> one_return;   // (a function that has one return type)
    for (const auto& [f, r] : rets)
        if (r.size() == 1) one_return[f] = *r.begin();
    std::vector<std::pair<std::pair<std::string, std::size_t>, Def>> order(defs.begin(), defs.end());
    std::stable_sort(order.begin(), order.end(), [](const auto& x, const auto& y) { return x.second.begin > y.second.begin; });
    for (const auto& [key, def] : order) {
        const auto& [fn, i] = key;
        std::set<std::optional<std::string>> given;
        for (const auto& kv : cx.files) {
            const std::string code = blank(kv.second);
            rx_name(R"((?<![\w.>])@@\s*\()", fn).for_each_match(code, [&](const RegexMatch& cm) {
                // (its definition: the file's first line is blank and the call is followed by parameters alone)
                const std::string_view before = std::string_view(code).substr(0, cm.start());
                const std::string_view first_line = before.substr(0, before.find('\n'));
                static const Regex parameters_then_newline(R"([\w\s,]*\)\s*\n)");
                if (text::trim(first_line).empty() && parameters_then_newline.match(std::string_view(code).substr(cm.end()))) return true;
                const std::string_view ln = line_at(code, cm.start());
                const auto sp = call_args(ln, fn);
                if (sp && sp->size() == 1 && text::trim(ln.substr((*sp)[0].begin, (*sp)[0].size())).empty()) return true;   // (extern f(), a declaration)
                if (!sp || i >= sp->size()) {
                    given.insert(std::nullopt);
                    return true;
                }
                std::string_view arg = text::trim(ln.substr((*sp)[i].begin, (*sp)[i].size()));
                while (!arg.empty() && arg.front() == '&') arg.remove_prefix(1);
                const auto r = one_return.find(std::string(arg));
                given.insert(word.full_match(arg) && r != one_return.end() ? std::optional<std::string>(r->second) : std::nullopt);
                return true;
            });
        }
        if (given.size() != 1 || !given.begin()->has_value()) continue;
        const std::string rt = **given.begin();
        std::string t = cx.files.at(def.file);
        const std::string piece = t.substr(def.begin, def.end - def.begin);
        const std::string neu = replace_first_with(rx_name(R"((?:int\s+)?\(\s*\*\s*@@)", def.name), piece, [&](const RegexMatch&) { return rt + "(*" + def.name; });
        t = t.substr(0, def.begin) + neu + t.substr(def.end);
        cx.files.insert_or_assign(def.file, t);
        note(cx.notes, "a function parameter given functions of pointers (int was a pointer's width): a function of that pointer",
             std::format("{} ({} of {}: {})", def.file, def.name, fn, rt));
    }
}

}  // namespace cb::port::pre_ansi
