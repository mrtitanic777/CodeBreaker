// cb/port/asm16_ctypes.cpp -- the C program's types and functions as the assembly's port needs them. (See asm16.hpp.)
// cb-unity: alone

#include <algorithm>

#include "cb/base/ctext.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/port/asm16.hpp"
#include "cb/port/c_source.hpp"
#include "cb/port/struct_layout.hpp"

namespace cb::port::asm16 {

namespace {

using pre_ansi::StructLayout;

// The words of a declaration that say where a variable lives, not what it is.
bool is_storage_word(std::string_view w) {
    return w == "extern" || w == "static" || w == "register" || w == "const" || w == "volatile";
}

// `text` with every whole word `drop` names taken out (the spaces around them stay).
template <class Pred>
std::string without_words(std::string_view text, Pred drop) {
    std::string out;
    for (std::size_t i = 0; i < text.size();) {
        if (pre_ansi::is_word_char(text[i])) {
            std::size_t j = i;
            while (j < text.size() && pre_ansi::is_word_char(text[j])) ++j;
            if (!drop(text.substr(i, j - i))) out.append(text.substr(i, j - i));
            i = j;
        } else {
            out.push_back(text[i++]);
        }
    }
    return out;
}

std::string storage_free(std::string_view base) { return std::string(text::trim(without_words(base, is_storage_word))); }

// `text` with each whole-word `word` made `with`.
std::string replace_word(std::string text, std::string_view word, std::string_view with) {
    for (std::size_t at = pre_ansi::find_word(text, word); at != std::string::npos; at = pre_ansi::find_word(text, word, at + with.size())) {
        text.replace(at, word.size(), with);
    }
    return text;
}

// The count of the first occurrence of `name` taken out of `declarator`, then the `*` left (a member that is a pointer).
int stars_after_name(std::string_view declarator, std::string_view name) {
    std::string d(declarator);
    if (const std::size_t at = d.find(name); at != std::string::npos) d.erase(at, name.size());
    return static_cast<int>(std::count(d.begin(), d.end(), '*'));
}

std::string repeat_stars(std::int64_t n) { return std::string(static_cast<std::size_t>(std::max<std::int64_t>(n, 0)), '*'); }

}  // namespace

// ---- CTypes ----------------------------------------------------------------------------------------------------------

CTypes::CTypes(std::span<const std::string> port_texts, std::span<const std::string> original_texts) {
    for (const std::string& t : port_texts) {
        const std::string code = base::ctext::blank(t);
        const std::vector<std::string> lines = pre_ansi::split_lines(code);
        const std::vector<int> depth = pre_ansi::brace_depths(lines);
        for (std::size_t i = 0; i < lines.size(); ++i) {
            std::string s(text::trim(lines[i]));
            if (depth[i] || !s.ends_with(";") || s.starts_with("typedef") || s.starts_with("#")) continue;
            if (text::replace_all(s, "(*", "").find('(') != std::string::npos) continue;
            if (const std::size_t eq = s.find('='); eq != std::string::npos) s = s.substr(0, eq) + ";";   // (an initialiser goes)
            const auto decl = pre_ansi::declarators(s);
            if (!decl || decl->base.empty()) continue;
            for (const pre_ansi::Declarator& d : decl->names)
                vars.try_emplace(d.name, VarType{storage_free(decl->base), d.text});   // (the first definition of a name counts)
        }
    }
    old_layouts = pre_ansi::lay_out_structs(original_texts, 2, 4);
    new_layouts = pre_ansi::lay_out_structs(port_texts, 8, 8);
    // globals whose pointers were far: `char far *g;`
    static const Regex far_global(R"(^[^;(){}#]*\bfar\s*\*[^;(){}]*?\b(\w+)\s*(?:\[[^\]]*\])?\s*(?:=[^;]*)?;)", RegexFlags::Multiline);
    for (const std::string& t : original_texts) {
        const std::string code = base::ctext::blank(t);
        far_global.for_each_match(code, [&](const RegexMatch& m) {
            far_ptrs.insert(std::string(m.group(1)));
            return true;
        });
    }
}

const VarType* CTypes::var(std::string_view name) const { return vars.find(std::string(name)); }

std::optional<std::string> CTypes::pointee(std::string_view name) const {
    const VarType* v = var(name);
    if (!v || v->declarator.find('*') == std::string::npos) return std::nullopt;
    return std::string(text::trim(pre_ansi::collapse_space(v->base)));
}

bool CTypes::is_pointer(std::string_view name) const {
    const VarType* v = var(name);
    return v && v->declarator.find('*') != std::string::npos;
}

std::optional<ElementSizes> CTypes::elem_sizes(std::string_view name) const {
    const VarType* v = var(name);
    if (!v) return std::nullopt;
    const std::string b(text::trim(pre_ansi::collapse_space(storage_free(v->base))));
    if (v->declarator.find('*') != std::string::npos) return ElementSizes{far_ptrs.contains(std::string(name)) ? 4 : 2, 8};
    if (b.starts_with("struct") || b.starts_with("union")) {
        const StructLayout* o = old_layouts.find(b);
        const StructLayout* n = new_layouts.find(b);
        if (o && n) return ElementSizes{o->size, n->size};
        return std::nullopt;
    }
    static const std::map<std::string, int, std::less<>> kScalars = {
        {"char", 1},      {"signed char", 1}, {"unsigned char", 1}, {"int16_t", 2}, {"uint16_t", 2}, {"short", 2},
        {"int32_t", 4},   {"uint32_t", 4},    {"long", 4},          {"int8_t", 1},  {"uint8_t", 1}};
    if (const auto it = kScalars.find(b); it != kScalars.end()) return ElementSizes{it->second, it->second};
    return std::nullopt;
}

std::optional<std::string> CTypes::member_type(std::string_view structure, std::int64_t old_offset) const {
    const StructLayout* o = old_layouts.find(std::string(structure));
    if (!o) return std::nullopt;
    for (const pre_ansi::Member& m : o->members) {
        if (m.offset != old_offset) continue;
        const std::string sub = std::string(structure) + "." + m.name;
        if (old_layouts.contains(sub)) return sub;
        const int stars = stars_after_name(m.declarator, m.name);
        return m.type + (stars ? " " + repeat_stars(stars) : std::string());
    }
    return std::nullopt;
}

std::optional<Field> CTypes::field(std::string_view structure, std::int64_t old_offset) const {
    const StructLayout* o = old_layouts.find(std::string(structure));
    const StructLayout* n = new_layouts.find(std::string(structure));
    if (!o || !n) return std::nullopt;
    return field_in(std::string(structure), *o, *n, old_offset);
}

std::optional<Field> CTypes::field_in(const std::string& path, const StructLayout& o, const StructLayout& n,
                                      std::int64_t old_offset) const {
    for (const bool exact : {true, false}) {   // (a member that begins there, else one around it)
        for (const pre_ansi::Member& v : o.members) {
            const std::int64_t fo = v.offset, fs = v.size;
            const pre_ansi::Member* nm = n.find(v.name);
            const std::string sub = path + "." + v.name;
            const StructLayout* old_inner = old_layouts.find(sub);
            const StructLayout* new_inner = new_layouts.find(sub);
            if (!(fo <= old_offset && old_offset < fo + fs && nm) || (exact && fo != old_offset && !old_inner)) continue;
            if (old_inner && new_inner) {   // (a structure or union inside: within it)
                if (old_inner->is_union) {  // (a union: its views, the one with a member there)
                    for (const pre_ansi::Member& vv : old_inner->members) {
                        const pre_ansi::Member* nv = new_inner->find(vv.name);
                        if (!nv) continue;
                        const std::string view_path = sub + "." + vv.name;
                        const StructLayout* vo = old_layouts.find(view_path);
                        const StructLayout* vn = new_layouts.find(view_path);
                        if (!vo || !vn) continue;
                        const std::int64_t inside = old_offset - fo - vv.offset;
                        const auto f = field_in(view_path, *vo, *vn, inside);
                        if (f && (f->old_offset == inside || !exact))
                            return Field{f->name,      fo + vv.offset + f->old_offset, f->old_size,
                                         nm->offset + nv->offset + f->new_offset, f->new_size, f->is_pointer, f->type};
                    }
                    continue;
                }
                const auto f = field_in(sub, *old_inner, *new_inner, old_offset - fo);
                if (f)
                    return Field{f->name, fo + f->old_offset, f->old_size, nm->offset + f->new_offset, f->new_size, f->is_pointer, f->type};
                continue;
            }
            const int stars = stars_after_name(v.declarator, v.name);
            return Field{v.name, fo, fs, nm->offset, nm->size, v.pointer,
                         v.type + (stars > 1 ? " " + repeat_stars(stars - 1) : std::string())};
        }
    }
    return std::nullopt;
}

// ---- the C program's functions -------------------------------------------------------------------------------------

const RuntimeCall* runtime_call(std::string_view old_name) {
    static const std::map<std::string, RuntimeCall, std::less<>> kTable = {
        {"__ffree", {"dos_ffree", CSig{false, {{4, CArg::Kind::Ptr}}, CSig::Ret::Void, std::nullopt}}},
        {"__fmalloc", {"dos_fmalloc", CSig{false, {{2, CArg::Kind::Val}}, CSig::Ret::Ptr, std::nullopt}}},
        {"_free", {"free", CSig{false, {{2, CArg::Kind::Ptr}}, CSig::Ret::Void, std::nullopt}}},
        {"_malloc", {"malloc", CSig{false, {{2, CArg::Kind::Val}}, CSig::Ret::Ptr, std::nullopt}}}};
    const auto it = kTable.find(old_name);
    return it == kTable.end() ? nullptr : &it->second;
}

OrderedMap<std::string, CSig> c_functions(std::span<const std::string> texts) {
    static const Regex define(R"(^\s*#\s*define\s+(\w+)[ \t]+([\w \t]+?)\s*$)", RegexFlags::Multiline);
    static const Regex head(R"(^([\w\s*]*?)\b([A-Za-z_]\w*)\s*\(([\w\s,]*)\)\s*$)");
    static const Regex base_words(
        R"(^((?:\s*\b(?:register|unsigned|signed|long|short|int|char|float|double|struct\s+\w+|union\s+\w+|far|near|huge)\b)+))");

    // the macros made of calling words (#define NEAR near pascal), as they were
    OrderedMap<std::string, std::string> macros;   // (a later definition of a name replaces the earlier)
    for (const std::string& t : texts) {
        define.for_each_match(t, [&](const RegexMatch& m) {
            bool calling = true;
            for (const std::string_view w : text::split_ws(m.group(2)))
                if (!is_calling_word(w)) calling = false;
            if (calling) macros.insert_or_assign(std::string(m.group(1)), std::string(m.group(2)));
            return true;
        });
    }

    OrderedMap<std::string, CSig> out;
    for (const std::string& t : texts) {
        std::string code = base::ctext::blank(t);
        for (const auto& [k, v] : macros) code = replace_word(std::move(code), k, v);
        const std::vector<std::string> lines = pre_ansi::split_lines(code);
        const std::vector<int> depth = pre_ansi::brace_depths(lines);
        for (std::size_t i = 0; i < lines.size(); ++i) {
            const auto m = head.match(lines[i]);
            if (!m || depth[i] != 0 || pre_ansi::is_keyword(m->group(2))) continue;
            // the arguments' types, declared after the head
            std::size_t j = i + 1;
            std::map<std::string, std::pair<std::string, std::string>> decls;   // name -> (base, declarator)
            while (j < lines.size() && !text::trim(lines[j]).starts_with("{")) {
                const std::string_view d = text::trim(lines[j]);
                if (!d.ends_with(";")) break;
                if (const auto base = base_words.match(d)) {
                    std::string_view tail = d.substr(base->end());
                    while (tail.ends_with(";")) tail.remove_suffix(1);
                    for (const std::string_view dcl : pre_ansi::split_top(tail)) {
                        std::size_t a = 0;
                        while (a < dcl.size() && !pre_ansi::is_word_char(dcl[a])) ++a;
                        std::size_t b = a;
                        while (b < dcl.size() && pre_ansi::is_word_char(dcl[b])) ++b;
                        if (b > a) decls[std::string(dcl.substr(a, b - a))] = {std::string(base->group(1)), std::string(dcl)};
                    }
                }
                ++j;
            }
            if (j >= lines.size() || !text::trim(lines[j]).starts_with("{")) continue;
            const std::string ret(m->group(1));
            std::vector<std::string> words;
            for (const std::string_view w : text::split_ws(ret)) words.emplace_back(w);
            const bool pascal = std::find(words.begin(), words.end(), "pascal") != words.end();
            CSig sig;
            bool skip = false;
            for (const std::string_view arg_raw : text::split(m->group(3), ',')) {
                const std::string a(text::trim(arg_raw));
                if (a.empty()) continue;
                const auto found = decls.find(a);
                const std::string base = found == decls.end() ? "int" : found->second.first;
                const std::string dcl = found == decls.end() ? a : found->second.second;
                if (dcl.find('*') != std::string::npos || dcl.find('[') != std::string::npos) {
                    const std::string before_name = dcl.substr(0, dcl.find(a));   // (what comes before the name: far?)
                    const std::string both = base + before_name;
                    const bool far = pre_ansi::find_word(both, "far") != std::string::npos || pre_ansi::find_word(both, "huge") != std::string::npos;
                    sig.args.push_back({far ? 4 : 2, CArg::Kind::Ptr});
                } else if (base.find("double") != std::string::npos || base.find("float") != std::string::npos) {
                    sig.args.push_back({8, CArg::Kind::Double});
                } else if (base.find("long") != std::string::npos) {
                    sig.args.push_back({4, CArg::Kind::Long});
                } else if (pre_ansi::find_word(base, "struct") != std::string::npos || pre_ansi::find_word(base, "union") != std::string::npos) {
                    skip = true;   // (a structure passed whole: not a call this port makes)
                    break;
                } else {
                    sig.args.push_back({2, CArg::Kind::Val});
                }
            }
            if (skip) continue;
            sig.pascal = pascal;
            const bool has_star = ret.find('*') != std::string::npos;
            const bool is_void = std::find(words.begin(), words.end(), "void") != words.end();
            if (has_star) sig.ret = CSig::Ret::Ptr;
            else if (std::find(words.begin(), words.end(), "long") != words.end()) sig.ret = CSig::Ret::Long;
            else if (is_void) sig.ret = CSig::Ret::Void;
            else sig.ret = CSig::Ret::Val;
            if (sig.ret == CSig::Ret::Ptr) {   // (what the address it gives points to)
                std::string base_type;
                const std::string before_star = ret.substr(0, ret.find('*'));
                for (std::size_t p = 0; p < before_star.size();) {
                    if (!pre_ansi::is_word_char(before_star[p])) {
                        ++p;
                        continue;
                    }
                    std::size_t q = p;
                    while (q < before_star.size() && pre_ansi::is_word_char(before_star[q])) ++q;
                    const std::string_view w = std::string_view(before_star).substr(p, q - p);
                    if (!is_calling_word(w) && w != "static" && w != "extern" && w != "register") base_type += (base_type.empty() ? "" : " ") + std::string(w);
                    p = q;
                }
                const std::int64_t stars = std::count(ret.begin(), ret.end(), '*');
                if (!base_type.empty()) sig.to = base_type + (stars > 1 ? " " + repeat_stars(stars - 1) : std::string());
            }
            const std::string name(m->group(2));
            const CSig* existing = out.find(name);
            if (!existing || sig.args.size() >= existing->args.size()) out.insert_or_assign(name, std::move(sig));
        }
    }
    return out;
}

std::string inner_name(std::string_view label, const std::set<std::string>& c_names) {
    const std::string n(label.starts_with('_') ? label.substr(1) : label);
    const std::string n_lower = text::lower(n);
    std::string c = n;
    for (const std::string& x : c_names)
        if (text::lower(x) == n_lower) c = x;
    return c == label ? "@" + std::string(label) : std::string(label);
}

}  // namespace cb::port::asm16
