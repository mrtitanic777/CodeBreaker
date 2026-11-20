// cb-unity: alone (helpers named for what they do here)
#include "port/tool_asm.hpp"

#include <algorithm>
#include <format>

#include "base/ctext.hpp"
#include <cctype>

#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "port/asmdata.hpp"
#include "port/c_source.hpp"
#include "port/old_sizes.hpp"
#include "port/tool_tree.hpp"

namespace cb::port::tool {

namespace pa = cb::port::pre_ansi;
using pa::collapse_space;
using pa::regex_quote;

namespace {

// The line without its `;` comment (the first `;`, quotes or not: a listing's own comment convention here).
std::string_view before_semicolon(std::string_view line) { return line.substr(0, line.find(';')); }

std::set<std::string> public_names_of(std::string_view text) {
    static const Regex public_line(R"(^\s*PUBLIC\s+(.+))", RegexFlags::IgnoreCase);
    std::set<std::string> out;
    for (const std::string_view line : text::lines(text)) {
        if (const auto m = public_line.match(before_semicolon(line))) {
            for (const std::string_view name : text::split(m->group(1), ',')) {
                const std::string_view t = text::trim(name);
                if (!t.empty()) out.emplace(t);
            }
        }
    }
    return out;
}

bool is_c_style_public(const std::string& p, const std::set<std::string>& known) {
    // `_x` is C's x
    if (p.size() >= 2 && p[0] == '_' && std::isalpha(static_cast<unsigned char>(p[1])) && !known.contains(p.substr(1))) return true;
    // CAPS is a pascal one
    const bool caps = !p.empty() && std::isupper(static_cast<unsigned char>(p[0])) &&
                      std::all_of(p.begin(), p.end(), [](unsigned char c) { return std::isupper(c) || std::isdigit(c) || c == '_'; });
    return caps && !known.contains(text::lower(p));
}

const std::map<std::string, std::string>& new_types() {
    static const std::map<std::string, std::string> kTypes = {
        {"", "int16_t"},          {"int", "int16_t"},      {"short", "int16_t"},   {"short int", "int16_t"},
        {"unsigned", "uint16_t"}, {"unsigned int", "uint16_t"}, {"unsigned short", "uint16_t"}, {"long", "int32_t"},
        {"long int", "int32_t"},  {"unsigned long", "uint32_t"}, {"char", "char"},  {"unsigned char", "unsigned char"},
        {"void", "void"}};
    return kTypes;
}

std::uint64_t new_size_of_scalar(const std::string& base) {
    static const std::map<std::string, std::uint64_t> kSizes = {
        {"char", 1},     {"signed char", 1}, {"unsigned char", 1}, {"int8_t", 1},  {"uint8_t", 1},  {"int16_t", 2}, {"uint16_t", 2},
        {"short", 2},    {"int32_t", 4},     {"uint32_t", 4},      {"long", 4},    {"int", 4},      {"unsigned", 4}};
    const auto it = kSizes.find(base);
    return it == kSizes.end() ? 1 : it->second;
}

bool is_identifier_text(std::string_view s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c) { return std::isalnum(c) || c == '_'; });
}

std::string stars(std::size_t n) { return std::string(n, '*'); }

}  // namespace

std::string_view asm_kind_name(AsmKind kind) {
    switch (kind) {
        case AsmKind::Runtime: return "runtime";
        case AsmKind::Data: return "data";
        case AsmKind::Code: return "code";
    }
    return "runtime";
}

// ---- roles ---------------------------------------------------------------------------------------------------------------

Role asm_role(std::string_view text, const pa::Profile& profile) {
    const std::set<std::string> pubs = public_names_of(text);
    Role role;
    role.publics.assign(pubs.begin(), pubs.end());   // a set: sorted
    const std::set<std::string> defined(profile.runtime_defines.begin(), profile.runtime_defines.end());
    std::set<std::string> known(profile.libc.begin(), profile.libc.end());
    known.insert(profile.platform_calls.begin(), profile.platform_calls.end());
    known.insert(defined.begin(), defined.end());

    // makes public what the runtime defines (fopen, fprintf and malloc the program may define itself), or has a main
    const std::set<std::string> kProgramsOwn = {"fopen", "fprintf", "malloc"};
    bool defines_runtime = pubs.contains("_main");
    for (const std::string& p : pubs) {
        const std::string name = p.starts_with("_") ? p.substr(1) : p;
        if (defined.contains(name) && !kProgramsOwn.contains(name)) defines_runtime = true;
    }
    if (defines_runtime) {
        role.kind = AsmKind::Runtime;
        return role;
    }
    static const Regex end_line(R"(^\s*END\s+\w+)", RegexFlags::IgnoreCase);
    static const Regex main_line(R"(^\s*_main\b)");
    bool has_end = false, has_main = false;
    for (const std::string_view line : text::lines(text)) {
        const std::string_view code = before_semicolon(line);
        has_end = has_end || end_line.match(code).has_value();
        has_main = has_main || main_line.match(code).has_value();
    }
    if (has_end && !has_main) {   // END start: the start-up code, the C library's now
        role.kind = AsmKind::Runtime;
        return role;
    }

    const std::vector<asm16::Line> lines = asm16::parse(text);
    const std::set<std::string> code_segments = asm16::code_segments(lines);
    bool code = false;
    std::optional<std::string> segment;
    for (const asm16::Line& l : lines) {
        if (l.directive && l.directive->kind == "SEGMENT") {
            segment = l.directive->name && code_segments.contains(*l.directive->name) ? l.directive->name : std::nullopt;
        } else if (l.directive && l.directive->kind == "ENDS") {
            segment.reset();
        } else if (segment && l.op) {
            code = true;
        }
    }
    if (!code) {
        role.kind = AsmKind::Data;
        return role;
    }
    // C names (_x is C's x, CAPS a pascal one; __x and cret the runtime's)
    const bool own = std::any_of(pubs.begin(), pubs.end(), [&](const std::string& p) { return is_c_style_public(p, known); });
    role.kind = own ? AsmKind::Code : AsmKind::Runtime;
    return role;
}

std::map<std::string, std::string> c_declared(std::span<const std::string> texts) {
    static const Regex define_re(R"((?m)^\s*#\s*define\s+(\w+)[ \t]+([\w \t]+?)\s*$)");
    static const Regex extern_re(R"(\bextern\s+([\w\s*]+?)\b([A-Za-z_]\w*)\s*\(\s*\)\s*;)");
    static const Regex word_re(R"(\w+|\*)");
    std::set<std::string> macros;   // made of calling words (`#define NEAR near pascal`)
    for (const std::string& t : texts) {
        define_re.for_each_match(t, [&](const RegexMatch& m) {
            const std::vector<std::string_view> words = text::split_ws(m.group(2));
            if (std::all_of(words.begin(), words.end(), [](std::string_view w) { return asm16::is_calling_word(w); })) macros.emplace(m.group(1));
            return true;
        });
    }
    std::map<std::string, std::string> out;
    for (const std::string& t : texts) {
        const std::string code = base::ctext::blank(t);
        extern_re.for_each_match(code, [&](const RegexMatch& m) {
            std::vector<std::string> words;
            word_re.for_each_match(m.group(1), [&](const RegexMatch& w) {
                const std::string word(w.whole());
                if (!asm16::is_calling_word(word) && !macros.contains(word)) words.push_back(word);
                return true;
            });
            const std::size_t star_count = static_cast<std::size_t>(std::count(words.begin(), words.end(), "*"));
            std::string base;
            for (const std::string& w : words) {
                if (w == "*") continue;
                base += (base.empty() ? "" : " ") + w;
            }
            const std::string name(m.group(2));
            if (star_count != 0) {
                out.try_emplace(name, base + " " + stars(star_count));
            } else if (const auto it = new_types().find(base); it != new_types().end()) {
                out.try_emplace(name, it->second);
            }
            return true;
        });
    }
    return out;
}

// ---- the old runtime's tails ---------------------------------------------------------------------------------------------

std::map<std::string, std::vector<std::string>> runtime_tails(const OrderedMap<std::string, std::string>& modules) {
    static const Regex label_re(R"(^\s*(\w+)(?:\s+PROC\b.*|\s*:\s*)$)", RegexFlags::IgnoreCase);
    static const Regex not_straight(R"(^(?:\w+\s*:|j\w+\b|call\b|loop\w*\b|\w+\s+(?:PROC|ENDP|ENDS)\b))", RegexFlags::IgnoreCase);
    static const Regex ret_re(R"(^\s*ret\b)", RegexFlags::IgnoreCase);
    std::map<std::string, std::vector<std::string>> out;
    for (const auto& [name, text] : modules) {
        const std::vector<std::string_view> lines = text::lines(text);
        const std::set<std::string> pubs = public_names_of(text);
        for (std::size_t i = 0; i < lines.size(); ++i) {
            const auto m = label_re.match(before_semicolon(lines[i]));
            if (!m || !pubs.contains(std::string(m->group(1)))) continue;
            std::vector<std::string> body;
            bool straight = true;
            for (std::size_t j = i + 1; j < lines.size(); ++j) {
                const std::string c(text::trim(before_semicolon(lines[j])));
                if (c.empty()) continue;
                if (not_straight.match(c)) {   // not straight: left to the C library's
                    straight = false;
                    break;
                }
                body.push_back("\t" + c);
                if (ret_re.match(c)) break;
            }
            if (straight && !body.empty() && ret_re.match(body.back())) out[std::string(m->group(1))] = std::move(body);
        }
    }
    return out;
}

std::string inline_tails(std::string_view text, const std::map<std::string, std::vector<std::string>>& tails,
                         std::vector<std::string>& notes, std::string_view module_name) {
    if (tails.empty()) return std::string(text);
    static const Regex jump_re(R"(^\s*jmp\s+(?:near\s+ptr\s+|short\s+)?(\w+)\s*$)", RegexFlags::IgnoreCase);
    std::vector<std::string> out;
    std::size_t jumps = 0;
    for (const std::string_view line : text::split(text, '\n')) {
        const auto m = jump_re.match(before_semicolon(line));
        if (m && tails.contains(std::string(m->group(1)))) {
            const std::string name(m->group(1));
            out.push_back(std::format("\t; (JMP {} - the old runtime's: its instructions here)", name));
            const std::vector<std::string>& body = tails.at(name);
            out.insert(out.end(), body.begin(), body.end());
            ++jumps;
            continue;
        }
        out.emplace_back(line);
    }
    if (jumps != 0) {
        std::vector<std::string> used;
        for (const auto& [name, body] : tails) {
            const Regex jumps_to(R"((?i)\bjmp\s+)" + regex_quote(name) + R"(\b)");
            if (jumps_to.test(text)) used.push_back(name);
        }
        notes.push_back(std::format("{}: {} jump(s) into the old runtime's own ends ({}): their instructions in place", module_name, jumps,
                                    text::join(used, ", ")));
    }
    // `"\n".join(lines)`: the text ended with a newline when it did, which split() made a last empty piece
    return text::join(out, "\n");
}

// ---- data alone ----------------------------------------------------------------------------------------------------------

namespace {

struct Place {
    std::string c_name;
    std::pair<std::string, std::uint64_t> at;   // segment, offset
    std::uint64_t own = 0;                      // bytes of its own
};

std::string remove_brackets(std::string_view s) {
    static const Regex re(R"(\[\s*\])");
    return re.replace_all(s, "");
}
bool has_empty_brackets(std::string_view s) {
    static const Regex re(R"(\[\s*\])");
    return re.test(s);
}
std::string squash_space(std::string_view s) {
    std::string out;
    for (const char c : s)
        if (!std::isspace(static_cast<unsigned char>(c))) out.push_back(c);
    return out;
}
std::string replace_first(std::string s, std::string_view from, std::string_view to) {
    if (const std::size_t at = s.find(from); at != std::string::npos) s.replace(at, from.size(), to);
    return s;
}

}  // namespace

DataDefinitions asm_data_defs(std::string_view text, const asm16::CTypes& ct) {
    const std::vector<asm16::Line> lines = asm16::parse(text);
    std::set<std::string> pubs;
    for (const asm16::Line& l : lines) {
        if (l.directive && l.directive->kind == "PUBLIC") {
            for (const std::string_view name : text::split(l.directive->rest, ','))
                if (!text::trim(name).empty()) pubs.emplace(text::trim(name));
        }
    }
    std::vector<Place> places;
    std::uint64_t off = 0;
    std::string seg;
    static const Regex dup_re(R"(^\s*(\d+)\s+DUP)", RegexFlags::IgnoreCase);
    for (const asm16::Line& l : lines) {
        if (!l.directive) continue;
        const asm16::Directive& d = *l.directive;
        if (d.kind == "SEGMENT") {
            seg = d.name.value_or("");
            off = 0;
            continue;
        }
        if (d.kind == "LABEL" && d.name && pubs.contains(*d.name)) {
            const std::string& n = *d.name;
            places.push_back({n.starts_with("_") ? n.substr(1) : n, {seg, off}, 0});
        } else if (d.kind == "DB" || d.kind == "DW" || d.kind == "DD") {
            const std::uint64_t width = d.kind == "DB" ? 1 : d.kind == "DW" ? 2 : 4;
            std::uint64_t count = 0;
            if (const auto m = dup_re.match(d.rest)) count = text::parse_int<std::uint64_t>(m->group(1), 10).value_or(0);
            else count = asm16::split_ops(d.rest).size();
            const std::uint64_t n = count * width;
            if (!places.empty() && places.back().at.first == seg) places.back().own += n;
            off += n;
        }
    }

    const auto old_size = [](const std::string& base, const std::string& dcl, const std::string& name) -> std::uint64_t {
        return pa::size_of(base, dcl, name, 2, pa::StructLayouts{}).value_or(2);
    };
    DataDefinitions out;
    std::set<std::string> done;
    // scalars of one place under two names: one variable, of the type that is a pointer if one is
    for (std::size_t i = 0; i < places.size(); ++i) {
        const Place& p = places[i];
        const asm16::VarType* v = ct.var(p.c_name);
        std::vector<const Place*> same;
        for (const Place& q : places)
            if (q.at == p.at) same.push_back(&q);
        if (same.size() < 2 || done.contains(p.c_name) || v == nullptr || v->declarator.find('[') != std::string::npos) continue;
        std::vector<std::pair<std::string, const asm16::VarType*>> typed;
        for (const Place* q : same) {
            const asm16::VarType* w = ct.var(q->c_name);
            if (w != nullptr && w->declarator.find('[') == std::string::npos) typed.emplace_back(q->c_name, w);
        }
        if (typed.size() < 2) continue;
        std::string canon = typed.back().first;
        for (const auto& [n, w] : typed) {
            if (w->declarator.find('*') != std::string::npos) {
                canon = n;
                break;
            }
        }
        for (const auto& [n, w] : typed) {
            if (n != canon) {
                out.one_place[n] = canon;
                done.insert(n);
            }
        }
    }

    for (std::size_t i = 0; i < places.size(); ++i) {
        const Place& p = places[i];
        if (done.contains(p.c_name)) continue;
        const asm16::VarType* v = ct.var(p.c_name);
        const asm16::VarType* nv = i + 1 < places.size() ? ct.var(places[i + 1].c_name) : nullptr;
        const bool same_next = v != nullptr && nv != nullptr && squash_space(nv->base) == squash_space(v->base) &&
                               (nv->declarator.find('*') != std::string::npos) == (remove_brackets(v->declarator).find('*') != std::string::npos);
        if (v != nullptr && has_empty_brackets(v->declarator) && (p.own == 0 || same_next)) {
            // an array that is the names after it: the old linker gave them one place
            const std::string& base = v->base;
            const std::string& dcl = v->declarator;
            const std::string element(text::trim(remove_brackets(dcl)));
            const std::uint64_t es = old_size(base, element, p.c_name);
            const std::string elt = squash_space(base + replace_first(element, p.c_name, ""));
            // (name, null: the same array by another name | {array-rest, element}: an array that is the rest from there |
            //  element: one of its elements)
            struct Next {
                std::string name;
                enum class How { Same, Rest, Element } how = How::Same;
                std::uint64_t index = 0;
            };
            std::vector<Next> next;
            std::optional<std::uint64_t> end;
            for (std::size_t j = i + 1; j < places.size(); ++j) {
                const Place& q = places[j];
                const asm16::VarType* v2 = ct.var(q.c_name);
                if (q.at.first != p.at.first) break;
                const std::optional<std::string> d2 =
                    v2 != nullptr ? std::optional<std::string>(squash_space(v2->base + replace_first(remove_brackets(v2->declarator), q.c_name, ""))) : std::nullopt;
                const std::int64_t rel = static_cast<std::int64_t>(q.at.second) - static_cast<std::int64_t>(p.at.second);
                const bool same_element = d2 && *d2 == elt && v2 != nullptr;
                if (same_element && has_empty_brackets(v2->declarator) && rel == 0) {
                    next.push_back({q.c_name, Next::How::Same, 0});
                } else if (same_element && es != 0 && rel % static_cast<std::int64_t>(es) == 0 && has_empty_brackets(v2->declarator)) {
                    next.push_back({q.c_name, Next::How::Rest, static_cast<std::uint64_t>(rel) / es});
                } else if (same_element && es != 0 && rel % static_cast<std::int64_t>(es) == 0) {
                    next.push_back({q.c_name, Next::How::Element, static_cast<std::uint64_t>(rel) / es});
                } else {
                    end = q.at.second;
                    break;
                }
            }
            std::uint64_t own_sum = 0;
            for (std::size_t k = i; k < std::min(places.size(), i + next.size() + 1); ++k) own_sum += places[k].own;
            const std::uint64_t extent = end ? *end - p.at.second : std::max(es, own_sum);
            std::uint64_t most = 0;
            for (const Next& n : next)
                if (n.how != Next::How::Same) most = std::max(most, n.index);
            const std::uint64_t count = std::max<std::uint64_t>({1, es != 0 ? extent / es : 1, most + 1});
            out.definitions.push_back(std::format("{} {};", base, text::trim(text::replace_all(dcl, "[]", std::format("[{}]", count)))));
            for (const Next& n : next) {
                switch (n.how) {
                    case Next::How::Same: out.aliases[n.name] = p.c_name; break;
                    case Next::How::Rest: out.aliases[n.name] = std::format("(&{}[{}])", p.c_name, n.index); break;
                    case Next::How::Element: out.aliases[n.name] = std::format("{}[{}]", p.c_name, n.index); break;
                }
                done.insert(n.name);
            }
            continue;
        }
        if (v != nullptr) {
            static const Regex brackets(R"(\[\s*\])");
            const std::uint64_t unit = std::max<std::uint64_t>(1, old_size(v->base, remove_brackets(v->declarator), p.c_name));
            const std::string dcl = brackets.replace_all(v->declarator, std::format("[{}]", std::max<std::uint64_t>(1, p.own / unit)));
            out.definitions.push_back(std::format("{} {};", v->base, text::trim(dcl)));
        } else {
            out.definitions.push_back(std::format("char {}[{}];\t\t/* (no C declaration says its type) */", p.c_name, std::max<std::uint64_t>(p.own, 1)));
        }
    }
    for (const Place& p : places)
        if (p.own != 0) out.sizes[p.c_name] = p.own;
    return out;
}

// ---- how the C program calls the assembly --------------------------------------------------------------------------------

std::map<std::string, asm16::Alias> alias_places(const pa::Aliases& aliases, const asm16::CTypes& ct) {
    static const Regex place_re(R"(\(?&?\s*(\w+)\s*(?:\[\s*(\d+)\s*\])?\)?)");
    static const Regex storage_re(R"(\b(?:extern|static)\b)");
    static const Regex pointer_declarator(R"(\(\s*\*)");
    std::map<std::string, asm16::Alias> out;
    for (const auto& [alias, a] : aliases) {
        const auto m = place_re.full_match(text::trim(a.where));
        if (!m) continue;
        const std::string owner(m->group(1));
        const std::int64_t k = m->has(2) ? text::parse_int<std::int64_t>(m->group(2), 10).value_or(0) : 0;
        const std::string& declaration = a.declaration;   // "" when the owner's declaration was not found
        const std::string before_owner = declaration.substr(0, declaration.find(owner));
        const std::string base(text::trim(storage_re.replace_all(before_owner, "")));
        std::int64_t size = 1;
        if (before_owner.find('*') != std::string::npos || pointer_declarator.test(declaration)) {
            size = 8;
        } else if (base.starts_with("struct") || base.starts_with("union")) {
            const pa::StructLayout* l = ct.new_layouts.find(collapse_space(base));
            size = l != nullptr ? static_cast<std::int64_t>(l->size) : 1;
        } else {
            size = static_cast<std::int64_t>(new_size_of_scalar(collapse_space(base)));
        }
        out[alias] = asm16::Alias{owner, k * size};
    }
    return out;
}

CallArgTypes::CallArgTypes(std::span<const std::string> originals, const asm16::CTypes& ctypes) : ctypes_(ctypes) {
    static const Regex far_re(R"((?m)^[^;(){}]*\bfar\b[^;(){}]*?\b(\w+)\s*(?:\[[^\]]*\])?\s*;)");
    for (const std::string& t : originals)
        far_re.for_each_match(base::ctext::blank(t), [&](const RegexMatch& m) {
            far_.emplace(m.group(1));
            return true;
        });
}

std::vector<asm16::ParamType> CallArgTypes::of(const std::string& fn, std::span<const std::string> texts) const {
    static const Regex amp_var(R"(&\s*(\w+))");
    static const Regex word_only(R"(\w+)");
    static const Regex long_literal(R"((?:0[xX][0-9a-fA-F]+|\d+)[lL])");
    static const Regex bracket_group(R"(\[[^\]]*\])");
    static const Regex long_type(R"(\b(?:long|int32_t|uint32_t)\b)");
    const Regex call_here = Regex(R"(\b)" + regex_quote(fn) + R"(\s*\()");
    const Regex declaration_line = Regex(R"(^\s*(?:extern\b|\w[\w\s*]*\b)" + regex_quote(fn) + R"(\s*\([^)]*\)\s*$))");
    std::vector<asm16::ParamType> best;
    const auto known = [](const std::vector<asm16::ParamType>& v) {
        return std::count_if(v.begin(), v.end(), [](const asm16::ParamType& p) { return p.to.has_value(); });
    };
    for (const std::string& t : texts) {
        const std::string code = base::ctext::blank(t);
        for (const std::string_view line : text::split(code, '\n')) {
            if (!call_here.test(line) || declaration_line.match(line)) continue;
            const auto spans = pa::call_args(line, fn);
            if (!spans) continue;
            std::vector<asm16::ParamType> out;
            for (const pa::Span& s : *spans) {
                const std::string arg(text::trim(line.substr(s.begin, s.end - s.begin)));
                const auto m = amp_var.full_match(arg);
                const asm16::VarType* v = m ? ctypes_.var(m->group(1)) : nullptr;
                if (v != nullptr) {
                    const std::string name(m->group(1));
                    const std::string without_name = replace_first(bracket_group.replace_all(v->declarator, ""), name, "");
                    const std::size_t star_count = static_cast<std::size_t>(std::count(without_name.begin(), without_name.end(), '*'));
                    out.push_back({far_.contains(name) ? 4 : 2, std::string(text::trim(v->base)) + (star_count != 0 ? " " + stars(star_count) : "")});
                    continue;
                }
                const bool plain_word = word_only.full_match(arg).has_value();
                // a variable of the calling file: a parameter, a local
                std::optional<RegexMatch> local;
                std::optional<Regex> local_re;
                if (plain_word) {
                    local_re.emplace(R"(\b((?:struct|union)\s+\w+|char|unsigned\s+char|int16_t|uint16_t)\s*(\*+)\s*)" + regex_quote(arg) +
                                     R"(\s*[;,)=])");
                    local = local_re->search(code);
                }
                if (local) {
                    const std::size_t star_count = local->group(2).size();
                    out.push_back({2, collapse_space(local->group(1)) + (star_count > 1 ? " " + stars(star_count - 1) : "")});
                    continue;
                }
                if (plain_word && ctypes_.is_pointer(arg)) {
                    out.push_back({far_.contains(arg) ? 4 : 2, ctypes_.pointee(arg)});
                    continue;
                }
                const asm16::VarType* w = plain_word ? ctypes_.var(arg) : nullptr;
                bool is_long = false;
                if (plain_word) {
                    const Regex long_decl(R"(\b(?:long|int32_t|uint32_t)\s+(?:[^;(){}]*,\s*)?)" + regex_quote(arg) + R"(\s*[;,=])");
                    is_long = long_decl.test(code);
                }
                const bool wide = (w != nullptr && long_type.test(w->base)) || is_long || long_literal.full_match(arg).has_value();
                out.push_back({wide ? 4 : 2, std::nullopt});
            }
            if (known(out) > known(best)) best = std::move(out);
        }
    }
    return best;
}

// ---- the program's own assembly language ---------------------------------------------------------------------------------

AsmPorting port_assembler_modules(AsmSink& sink, std::span<const std::string> c_files, std::span<const std::string> originals,
                                  const OrderedMap<std::string, std::string>& modules,
                                  const std::map<std::string, std::filesystem::path>& paths, const std::map<std::string, Role>& roles,
                                  const pa::Profile& profile, const AsmOptions& options) {
    namespace stdfs = std::filesystem;
    AsmPorting result;
    const asm16::CTypes ct(c_files, originals);
    // what the C program calls, and defines in the old way
    static const Regex call_re(R"(\b([A-Za-z_]\w*)\s*\()");
    static const Regex far_func_re(R"(\bfar\b[^;(){}]*?\b(\w+)\s*\()");
    std::set<std::string> call_set, far_set;
    for (const std::string& t : c_files)
        call_re.for_each_match(base::ctext::blank(t), [&](const RegexMatch& m) {
            call_set.emplace(m.group(1));
            return true;
        });
    for (const std::string& t : originals)
        far_func_re.for_each_match(base::ctext::blank(t), [&](const RegexMatch& m) {
            far_set.emplace(m.group(1));
            return true;
        });
    const std::vector<std::string> calls(call_set.begin(), call_set.end());
    const OrderedMap<std::string, asm16::CSig> cfuncs = asm16::c_functions(originals);
    const std::map<std::string, std::string> declared = c_declared(originals);

    OrderedMap<std::string, std::string> runtime_modules;
    for (const auto& [n, t] : modules)
        if (roles.at(n).kind == AsmKind::Runtime) runtime_modules.insert_or_assign(n, t);
    const auto tails = runtime_tails(runtime_modules);
    OrderedMap<std::string, std::string> texts;
    for (const auto& [n, t] : modules)
        texts.insert_or_assign(n, roles.at(n).kind == AsmKind::Code ? inline_tails(t, tails, result.notes, n) : t);

    // each module's public routines: the others call them
    std::map<std::string, std::set<std::string>> routines, externs;
    for (const auto& [n, t] : texts) {
        if (roles.at(n).kind != AsmKind::Code) continue;
        asm16::PortSettings s;
        s.c_names = calls;
        const asm16::Port m0(asm16::module(n, t), ct, s);
        for (const std::string& p : m0.public_names())
            if (m0.labels().contains(p)) routines[n].insert(p);
        for (const auto& [name, kind] : m0.externs()) externs[n].insert(name);
    }

    // the data first: the names it places
    std::vector<std::string> order;
    for (const auto& [n, t] : texts) order.push_back(n);
    std::stable_sort(order.begin(), order.end(), [&](const std::string& a, const std::string& b) {
        const bool a_data = roles.at(a).kind == AsmKind::Data, b_data = roles.at(b).kind == AsmKind::Data;
        if (a_data != b_data) return a_data;
        return a < b;
    });

    const CallArgTypes call_args(originals, ct);
    std::set<std::string> lib(profile.libc.begin(), profile.libc.end());
    lib.insert({"strcmp", "strlen", "strcpy", "strcat", "memcpy", "memset", "memcmp"});

    for (const std::string& n : order) {
        const std::string& t = texts.at(n);
        const AsmKind kind = roles.at(n).kind;
        if (kind == AsmKind::Data) {
            if (const auto msg = asmdata::messages(t)) {   // the C library's numbered messages: a table of C
                sink.asm_data.emplace_back(n, *msg);
                result.notes.push_back(std::format("{}: the numbered messages (__NMSG_TEXT, __FMSG_TEXT): a table in data.c", n));
                continue;
            }
            if (const auto typed = asmdata::typed(t, ct)) {   // tables with addresses in them: C, initialized
                sink.asm_data.emplace_back(n, typed->text);
                result.notes.push_back(std::format("{}: tables with addresses in them - C definitions with initializers, in data.c", n));
                continue;
            }
            const DataDefinitions defs = asm_data_defs(t, ct);
            for (const std::string& d : defs.definitions) sink.asm_data.emplace_back(n, d);
            for (const auto& [alias, canon] : defs.one_place) {   // one variable: its name and type everywhere
                const asm16::VarType* v = ct.var(canon);
                std::string typ = v != nullptr ? std::string(text::trim(v->base + " " + replace_first(v->declarator, canon, ""))) : std::string();
                sink.renames[alias] = canon;
                pa::DeclRule rule(R"((?m)^(\s*extern\s+)[^;(=]*?\b)" + regex_quote(canon) + R"(\s*;)",
                                  "${1}" + typ + (typ.ends_with("*") ? "" : " ") + canon + ";");
                const bool present = std::any_of(sink.decl_rules.begin(), sink.decl_rules.end(), [&](const pa::DeclRule& r) {
                    return r.pattern() == rule.pattern() && r.replacement() == rule.replacement();
                });
                if (!present) sink.decl_rules.push_back(std::move(rule));
                result.notes.push_back(std::format("{}: {} and {} are one place - one variable, {}", n, alias, canon, typ));
            }
            for (const auto& [name, bytes] : defs.sizes) sink.asm_sizes[name] = bytes;
            for (const auto& [alias, where] : defs.aliases) {   // names of one place: one variable
                static const Regex identifier(R"([A-Za-z_]\w*)");
                const auto owner = identifier.search(where);
                std::optional<std::string> owner_definition;
                if (owner) {
                    const Regex indexed(R"(\b)" + regex_quote(owner->whole()) + R"(\s*\[)");
                    for (const auto& [module, d] : sink.asm_data)
                        if (indexed.test(d)) {
                            owner_definition = d;
                            break;
                        }
                }
                sink.aliases.insert_or_assign(alias, pa::Alias{where, owner_definition.value_or("")});
            }
            result.notes.push_back(std::format("{}: data alone - its variables defined in data.c with the C program's types", n));
            continue;
        }
        if (kind != AsmKind::Code) continue;

        asm16::PortSettings s;
        s.c_names = calls;
        s.c_funcs = cfuncs;
        for (const auto& [other, r] : routines)
            if (other != n) s.asm_routines.insert(r.begin(), r.end());
        s.underscore = options.c_underscore;
        asm16::Port m(asm16::module(n, t), ct, s);
        for (const auto& [other, e] : externs)
            if (other != n) m.used_outside.insert(e.begin(), e.end());
        for (const auto& [p, c] : m.cname) m.param_types[p] = call_args.of(c, c_files);
        m.aliases = alias_places(sink.aliases, ct);
        m.data_map = data_places(paths.at(n));
        for (const auto& [p, c] : m.cname)
            if (far_set.contains(c)) m.far.insert(p);
        for (auto& [p, c] : m.cname) {
            if (lib.contains(c)) {   // the program's own routine of a C library name: kept under a name of its own, in the C too
                sink.renames[c] = c + "_";
                result.notes.push_back(std::format("{}: its own {} kept (not the C library's), as {}_", n, c, c));
                c += "_";
            }
        }
        if (options.old_assembler && stdfs::is_regular_file(*options.old_assembler) && options.run_old_assembler)
            m.offsets = asm16::listing_offsets(paths.at(n), *options.old_assembler, options.run_old_assembler);
        std::string ported;
        try {
            m.run();
            ported = m.emit();
        } catch (const asm16::Asm16Error& e) {
            result.notes.push_back(std::format("**{}: not ported - {}**", n, e.what()));
            continue;
        }
        const std::string base = fs::to_text(stdfs::path(n).stem()) + "64";
        fs::write_text(options.out / (base + ".asm"), ported);
        for (const auto& [p, c] : m.cname) {
            const std::optional<asm16::State> ex = m.exit_of(p);
            const asm16::K* ax = ex ? ex->reg("ax") : nullptr;
            std::string ret = ax != nullptr && ax->wide() && ax->kind != asm16::Kind::Frame ? "char *" : "int16_t";
            if (const auto d = declared.find(c); d != declared.end()) ret = d->second;   // the C program's own declaration says
            result.prototypes[c] = ret + (ret.ends_with("*") ? "" : " ") + c + "();";
        }
        if (!options.ml64) {
            result.notes.push_back(std::format("**{}: ported to {}.asm, but no ml64 to assemble it**", n, base));
            continue;
        }
        process::ProcessSpec assemble;
        assemble.argv = {fs::to_text(*options.ml64), "/nologo", "/c", "/Cp", "/Fo" + base + ".obj", base + ".asm"};
        assemble.cwd = options.out;
        const process::ProcessResult assembled = process::run(assemble);
        if (assembled.exit_code != 0) {
            std::string said = std::string(as_text(assembled.out)) + std::string(as_text(assembled.err));
            if (said.size() > 400) said = said.substr(said.size() - 400);
            result.notes.push_back(std::format("**{}: {}.asm does not assemble: {}**", n, base, said));
            continue;
        }
        result.objects.push_back(base + ".obj");
        result.notes.push_back(std::format("{}: ported as assembly language to {}.asm ({} routines the C program calls)", n, base, m.cname.size()));
        if (!m.unreached().empty()) {   // code no path was found to: kept as comments - said
            std::vector<std::string> first;
            for (std::size_t i = 0; i < std::min<std::size_t>(12, m.unreached().size()); ++i) first.push_back(std::to_string(m.unreached()[i]));
            result.notes.push_back(std::format("**{}: {} lines no path reaches were left as comments (lines {}) - look at them**", n,
                                               m.unreached().size(), text::join(first, ", ")));
        }
    }
    return result;
}

}  // namespace cb::port::tool
