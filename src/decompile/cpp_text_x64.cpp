// decompile/cpp_text_x64.cpp -- see cpp_text_x64.hpp.
#include "decompile/cpp_text_x64.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <optional>
#include <regex>
#include <set>
#include <vector>

#include "core/text.hpp"

namespace cb::decompile::cpp_text_x64 {

namespace {

namespace cx = symbols::cpp_classes::x64;

bool cx_word(char c) { return c == '_' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

std::string cx_squash(std::string_view s) { return text::join(text::split_ws(s), " "); }

// A function's text taken apart: what comes before its heading, its result, name and parameters, and its body (from `{`).
struct Cx64Parsed {
    std::string lead;
    std::string ret;
    std::string name;
    std::vector<std::pair<std::string, std::string>> params;   // (type, name)
    std::string body;
};

std::optional<Cx64Parsed> cx_parse(std::string_view text) {
    Cx64Parsed p;
    std::string_view rest = text::trim(text);
    while (rest.starts_with("/*")) {
        const std::size_t end = rest.find("*/");
        if (end == std::string_view::npos) return std::nullopt;
        p.lead += std::string(rest.substr(0, end + 2)) + "\n";
        rest = text::trim(rest.substr(end + 2));
    }
    const std::size_t brace = rest.find('{');
    if (brace == std::string_view::npos) return std::nullopt;
    const std::string_view head = rest.substr(0, brace);
    const std::size_t open = head.find('('), close = head.rfind(')');
    if (open == std::string_view::npos || close == std::string_view::npos || close < open) return std::nullopt;
    const std::string_view before = text::rtrim(head.substr(0, open));
    std::size_t n = before.size();
    while (n > 0 && cx_word(before[n - 1])) --n;
    p.name = std::string(before.substr(n));
    p.ret = cx_squash(before.substr(0, n));
    if (p.name.empty() || p.ret.empty()) return std::nullopt;
    const std::string_view list = head.substr(open + 1, close - open - 1);
    int depth = 0;
    std::size_t from = 0;
    auto take = [&](std::string_view one) -> bool {
        one = text::trim(one);
        if (one.empty() || one == "void") return true;
        std::size_t e = one.size();
        while (e > 0 && cx_word(one[e - 1])) --e;
        if (e == one.size() || e == 0) return false;
        p.params.emplace_back(cx_squash(one.substr(0, e)), std::string(one.substr(e)));
        return true;
    };
    for (std::size_t k = 0; k < list.size(); ++k) {
        if (list[k] == '(') ++depth;
        if (list[k] == ')') --depth;
        if (list[k] == ',' && depth == 0) {
            if (!take(list.substr(from, k - from))) return std::nullopt;
            from = k + 1;
        }
    }
    if (!take(list.substr(from))) return std::nullopt;
    p.body = std::string(rest.substr(brace));
    return p;
}

bool cx_is_float(std::string_view type) { return type == "float8" || type == "float4" || type == "double" || type == "float"; }
bool cx_is_pointer(std::string_view type) { return type.ends_with("*"); }

// `type` with the space the decompiler leaves before a pointer's stars: `xunknown8 *`.
std::string cx_decl(std::string_view type, std::string_view name) {
    if (!cx_is_pointer(type)) return std::string(type) + " " + std::string(name);
    const std::size_t stars = type.find_last_not_of('*') + 1;
    return std::string(text::rtrim(type.substr(0, stars))) + " " + std::string(type.substr(stars)) + std::string(name);
}

// Every whole-word `from` in `s` replaced by what `to` makes of the text after it (which it may consume: it returns how much).
template <class F>
std::string cx_each_word(std::string_view s, std::string_view word, F to) {
    std::string out;
    std::size_t k = 0;
    while (k < s.size()) {
        const std::size_t at = s.find(word, k);
        if (at == std::string_view::npos) break;
        const std::size_t end = at + word.size();
        if ((at > 0 && cx_word(s[at - 1])) || (end < s.size() && cx_word(s[end]))) {
            out.append(s.substr(k, end - k));
            k = end;
            continue;
        }
        out.append(s.substr(k, at - k));
        std::size_t used = 0;
        out += to(s.substr(end), used);
        k = end + used;
    }
    out.append(s.substr(k));
    return out;
}

std::string cx_hex(std::int64_t v) { return std::format("{:x}", v); }

// A field type the header declares (the decompiler's sized words, `code`, the structures); another is the bytes it takes.
bool cx_known_type(std::string_view word, const std::set<std::string>& structs) {
    static const std::regex sized(R"((x?unknown|xunknown|u?int|float|unkbyte|unkint|unkuint)\d+|code|bool|char)");
    return structs.contains(std::string(word)) || std::regex_match(word.begin(), word.end(), sized);
}

struct Cx64Field {
    std::int64_t offset = 0;
    std::int64_t bytes = 0;
    std::string type;
};

struct Cx64Sig {
    std::string ret = "uint8";
    std::vector<std::string> params;   // after `this`
};

struct Cx64Decl {
    std::size_t slot = 0;
    bool overrides = false;
    bool pure = false;
    std::uint64_t function = 0;
    std::pair<std::size_t, std::size_t> family;   // (class that introduces it, slot)
};

struct Cx64Cls {
    const cx::Class* rtti = nullptr;
    std::vector<std::string> scope;   // the namespaces it is declared in
    std::string leaf;
    std::string cpp;                  // scope::leaf
    std::optional<std::size_t> base;  // the emitted class its primary table extends
    std::vector<std::uint64_t> slots;
    std::vector<std::pair<std::size_t, std::size_t>> family;   // per slot
    std::vector<Cx64Decl> decls;
    std::map<std::int64_t, Cx64Field> fields;
};

// A name part as an identifier: `anonymous namespace` and template arguments become words.
std::string cx_ident(std::string_view part) {
    std::string s(part);
    if (s == "`anonymous namespace'") return "anonymous_namespace";
    std::string out;
    for (const char c : s) out.push_back(cx_word(c) ? c : '_');
    while (!out.empty() && out.back() == '_') out.pop_back();
    while (out.find("__") != std::string::npos) out = text::replace_all(out, "__", "_");
    if (!out.empty() && out[0] >= '0' && out[0] <= '9') out = "_" + out;
    return out;
}

// `a::b<c::d>::e` as its parts {a, b<c::d>, e}.
std::vector<std::string> cx_parts(std::string_view name) {
    std::vector<std::string> out;
    int depth = 0;
    std::size_t from = 0;
    for (std::size_t k = 0; k < name.size(); ++k) {
        if (name[k] == '<') ++depth;
        if (name[k] == '>') --depth;
        if (depth == 0 && name.substr(k, 2) == "::") {
            out.emplace_back(name.substr(from, k - from));
            from = k + 2;
            ++k;
        }
    }
    out.emplace_back(name.substr(from));
    return out;
}

// A class that is a library's: the C++ library's, the runtime's, and the template libraries a Windows program is built with
// (WRL, WIL, wistd); every template instance (its code is the template's, written once by the library); a name the demangler
// cannot read (a lambda's closure).
bool cx_library_class(const cx::Class& c) {
    const std::string_view name = c.name;
    if (name == c.decorated || name.find('<') != std::string_view::npos || name.find('`') != std::string_view::npos) return true;
    for (const std::string_view p : {"std::", "stdext::", "Concurrency::", "type_info", "__", "Microsoft::WRL::", "wil::", "wistd::", "ATL::"})
        if (name.starts_with(p)) return true;
    return false;
}

}  // namespace

Recovered recover(const Input& in) {
    Recovered out;
    if (in.classes == nullptr || in.texts == nullptr || in.by_va == nullptr) return out;
    // ---- the classes declared, bases first ----
    std::vector<Cx64Cls> classes;
    std::map<std::string, std::size_t> by_decorated;
    {
        std::vector<const cx::Class*> chosen;
        for (const auto& [decorated, c] : in.classes->classes)
            if (c.primary() != nullptr && !cx_library_class(c)) chosen.push_back(&c);
        // bases first: a class after the classes its primary chain names
        std::set<std::string> placed;
        std::map<std::string, const cx::Class*> pending;
        for (const cx::Class* c : chosen) pending.emplace(c->decorated, c);
        std::vector<const cx::Class*> ordered;
        std::function<void(const cx::Class*)> place = [&](const cx::Class* c) {
            if (placed.contains(c->decorated)) return;
            placed.insert(c->decorated);
            if (const cx::BaseClass* b = c->primary_base(); b != nullptr)
                if (const auto it = pending.find(b->decorated); it != pending.end()) place(it->second);
            ordered.push_back(c);
        };
        for (const cx::Class* c : chosen) place(c);
        std::set<std::string> full;
        for (const cx::Class* c : ordered) {
            Cx64Cls k;
            k.rtti = c;
            std::vector<std::string> parts;
            for (const std::string& p : cx_parts(c->name)) parts.push_back(cx_ident(p));
            k.leaf = parts.back();
            parts.pop_back();
            k.scope = parts;
            classes.push_back(std::move(k));
        }
        // (a class nested in another class this declares is named whole: a namespace and a class cannot share a name)
        for (Cx64Cls& k : classes) full.insert(text::join(k.scope, "::") + (k.scope.empty() ? "" : "::") + k.leaf);
        std::set<std::string> taken;
        for (Cx64Cls& k : classes) {
            std::string prefix;
            bool clash = false;
            for (const std::string& s : k.scope) {
                prefix += (prefix.empty() ? "" : "::") + s;
                if (full.contains(prefix)) clash = true;
            }
            if (clash) {
                std::vector<std::string> all = k.scope;
                all.push_back(k.leaf);
                k.leaf = text::join(all, "_");
                k.scope.clear();
            }
            k.cpp = text::join(k.scope, "::") + (k.scope.empty() ? "" : "::") + k.leaf;
            for (int n = 2; k.leaf.empty() || taken.contains(k.cpp); ++n) {
                k.leaf = (k.leaf.empty() ? "Class" : k.leaf) + "_" + std::to_string(n);
                k.cpp = text::join(k.scope, "::") + (k.scope.empty() ? "" : "::") + k.leaf;
            }
            taken.insert(k.cpp);
        }
        for (std::size_t i = 0; i < classes.size(); ++i) by_decorated[classes[i].rtti->decorated] = i;
    }
    if (classes.empty()) return out;
    const std::map<Va, std::string>& names = in.names != nullptr ? *in.names : *in.by_va;
    auto name_at = [&](std::uint64_t va) -> std::string {
        const auto it = names.find(static_cast<Va>(va));
        return it == names.end() ? std::string() : it->second;
    };

    // (a pure virtual function's slot holds _purecall, or a stub that only calls it)
    auto is_purecall = [&](std::uint64_t va) {
        const std::string n = name_at(va);
        if (n == "_purecall") return true;
        const auto own = in.by_va->find(static_cast<Va>(va));
        const std::string* t = own == in.by_va->end() ? nullptr : in.texts->find(own->second);
        return t != nullptr && t->find("_purecall(") != std::string::npos && std::count(t->begin(), t->end(), ';') <= 2;
    };
    // ---- slots: what each class declares ----
    for (std::size_t i = 0; i < classes.size(); ++i) {
        Cx64Cls& k = classes[i];
        k.slots = k.rtti->primary()->slots;
        if (const cx::BaseClass* b = k.rtti->primary_base(); b != nullptr)
            if (const auto it = by_decorated.find(b->decorated); it != by_decorated.end() && classes[it->second].slots.size() <= k.slots.size())
                k.base = it->second;
        const std::size_t inherited = k.base ? classes[*k.base].slots.size() : 0;
        for (std::size_t s = 0; s < k.slots.size(); ++s) {
            if (s < inherited) {
                k.family.push_back(classes[*k.base].family[s]);
                if (k.slots[s] == classes[*k.base].slots[s]) continue;
                k.decls.push_back({s, true, false, k.slots[s], k.family.back()});
            } else {
                k.family.emplace_back(i, s);
                k.decls.push_back({s, false, false, k.slots[s], k.family.back()});
            }
            k.decls.back().pure = is_purecall(k.slots[s]);
        }
    }

    // ---- which functions are methods ----
    std::map<std::uint64_t, std::string> c_name;   // address -> the function's C name, where there is C
    for (const auto& [va, n] : *in.by_va)
        if (in.texts->find(n) != nullptr) c_name[static_cast<std::uint64_t>(va)] = n;
    // every place a function is in any table
    std::map<std::uint64_t, std::vector<std::tuple<const cx::Class*, std::uint32_t, std::size_t>>> places;
    for (const auto& [decorated, c] : in.classes->classes)
        for (const cx::VTable& t : c.vtables)
            for (std::size_t s = 0; s < t.slots.size(); ++s) places[t.slots[s]].emplace_back(&c, t.offset, s);
    auto derives = [&](const cx::Class* x, const cx::Class* from) {
        if (x == from) return true;
        for (const cx::BaseClass& b : x->bases)
            if (b.decorated == from->decorated && b.mdisp == 0 && !b.is_virtual()) return true;
        return false;
    };
    std::map<std::uint64_t, std::vector<std::pair<std::size_t, std::size_t>>> declared_at;   // function -> (class, decl)
    for (std::size_t i = 0; i < classes.size(); ++i)
        for (std::size_t d = 0; d < classes[i].decls.size(); ++d) declared_at[classes[i].decls[d].function].emplace_back(i, d);
    struct Cx64Owner {
        std::size_t cls = 0;
        bool is_virtual = false;
        std::size_t decl = 0;
        bool ctor = false;
    };
    std::map<std::string, Cx64Owner> owners;   // C name -> its class
    for (const auto& [function, at] : declared_at) {
        if (at.size() != 1 || !c_name.contains(function)) continue;
        const auto [i, d] = at.front();
        const Cx64Cls& k = classes[i];
        bool alone = true;
        for (const auto& [c, offset, slot] : places[function])
            if (offset != 0 || slot != k.decls[d].slot || !derives(c, k.rtti)) alone = false;
        if (alone && !k.decls[d].pure) owners[c_name.at(function)] = {i, true, d, false};
    }
    // constructors and destructors: the functions that store one class's own table into their first argument
    std::map<std::uint64_t, std::size_t> by_table;
    for (std::size_t i = 0; i < classes.size(); ++i) by_table[classes[i].rtti->primary()->address] = i;
    static const std::regex store(R"((?:\bparam_1->f_0|\*param_1|\*\(\w+ \*\)param_1) = 0x([0-9a-f]+);)");
    auto ancestors = [&](std::size_t i) {
        std::set<std::size_t> a;
        for (std::optional<std::size_t> at = i; at; at = classes[*at].base) a.insert(*at);
        return a;
    };
    for (const auto& [va, n] : c_name) {
        if (owners.contains(n) || places.contains(va)) continue;
        const std::string& t = *in.texts->find(n);
        std::vector<std::size_t> stored;
        for (auto it = std::sregex_iterator(t.begin(), t.end(), store); it != std::sregex_iterator(); ++it)
            if (const auto table = text::parse_int<std::uint64_t>((*it)[1].str(), 16))
                if (const auto c = by_table.find(*table); c != by_table.end()) stored.push_back(c->second);
        if (stored.empty()) continue;
        std::optional<std::size_t> most;
        for (const std::size_t s : stored) {
            const std::set<std::size_t> a = ancestors(s);
            if (std::all_of(stored.begin(), stored.end(), [&](std::size_t o) { return a.contains(o); })) most = s;
        }
        if (!most) continue;
        if (stored.back() == *most) owners[n] = {*most, false, 0, true};
        else if (stored.front() == *most) owners[n] = {*most, false, 0, false};
    }

    // ---- signatures: one per slot, the introducing class's ----
    std::map<std::string, Cx64Parsed> parsed;
    for (const auto& [n, o] : owners)
        if (std::optional<Cx64Parsed> p = cx_parse(*in.texts->find(n))) parsed.emplace(n, std::move(*p));
    for (auto it = owners.begin(); it != owners.end();)   // (a text this cannot take apart stays a function)
        it = parsed.contains(it->first) ? std::next(it) : owners.erase(it);
    std::map<std::pair<std::size_t, std::size_t>, Cx64Sig> sigs;
    {
        std::map<std::pair<std::size_t, std::size_t>, std::vector<const Cx64Parsed*>> impls;
        for (const auto& [n, o] : owners)
            if (o.is_virtual) impls[classes[o.cls].decls[o.decl].family].push_back(&parsed.at(n));
        for (const Cx64Cls& k : classes)
            for (const Cx64Decl& d : k.decls) {
                if (sigs.contains(d.family)) continue;
                Cx64Sig s;
                bool all_void = true, any = false;
                for (const Cx64Parsed* p : impls[d.family]) {
                    any = true;
                    if (p->ret != "void") all_void = false;
                    if (cx_is_float(p->ret)) s.ret = p->ret;
                    for (std::size_t a = 1; a < p->params.size(); ++a) {
                        if (s.params.size() < a) s.params.push_back("uint8");
                        if (cx_is_float(p->params[a].first)) s.params[a - 1] = p->params[a].first;
                    }
                }
                if (any && all_void) s.ret = "void";
                sigs[d.family] = s;
            }
    }

    // ---- data members: the fields the methods read through `this` ----
    std::map<std::string, const core16::StructNote*> notes;
    std::set<std::string> struct_names;
    for (const core16::StructNote& s : in.structs) {
        notes.try_emplace(s.name, &s);
        struct_names.insert(s.name);
    }
    auto this_note = [&](const Cx64Parsed& p) -> const core16::StructNote* {
        if (p.params.empty() || !cx_is_pointer(p.params[0].first)) return nullptr;
        const std::string word = std::string(text::trim(p.params[0].first.substr(0, p.params[0].first.size() - 1)));
        const auto it = notes.find(word);
        return it == notes.end() ? nullptr : it->second;
    };
    auto add_field = [](std::map<std::int64_t, Cx64Field>& fields, const Cx64Field& f) {
        const auto next = fields.lower_bound(f.offset);
        if (next != fields.end() && next->first < f.offset + f.bytes) return false;
        if (next != fields.begin() && std::prev(next)->second.offset + std::prev(next)->second.bytes > f.offset) return false;
        fields.emplace(f.offset, f);
        return true;
    };
    for (const auto& [n, o] : owners) {
        const core16::StructNote* s = this_note(parsed.at(n));
        if (s == nullptr) continue;
        for (const core16::StructField& f : s->fields) {
            if (f.offset < 8 || f.bytes <= 0) continue;   // (the table pointer)
            const std::size_t stars = f.type.find_last_not_of('*') + 1;
            std::string type = f.type;
            const bool whole = f.bytes == 1 || f.bytes == 2 || f.bytes == 4 || f.bytes == 8;
            if (f.type.find('^') != std::string::npos || !cx_known_type(f.type.substr(0, stars), struct_names) ||
                (stars == f.type.size() && struct_names.contains(f.type)))
                type = whole ? "xunknown" + std::to_string(f.bytes) : "";
            if (type.empty()) continue;
            add_field(classes[o.cls].fields, {f.offset, f.bytes, type});
        }
    }
    auto extent = [&](std::size_t i) {
        std::int64_t e = 8;
        for (std::optional<std::size_t> at = i; at; at = classes[*at].base)
            for (const auto& [off, f] : classes[*at].fields) e = std::max(e, f.offset + f.bytes);
        return e;
    };
    for (bool moved = true; moved;) {   // (a field a class reads below the end of its base's members is the base's)
        moved = false;
        for (Cx64Cls& k : classes) {
            if (!k.base) continue;
            const std::int64_t e = extent(*k.base);
            for (auto it = k.fields.begin(); it != k.fields.end();) {
                if (it->first >= e) {
                    ++it;
                    continue;
                }
                (void)add_field(classes[*k.base].fields, it->second);
                it = k.fields.erase(it);
                moved = true;
            }
        }
    }
    auto field_of = [&](std::size_t i, std::int64_t offset) -> const Cx64Field* {
        for (std::optional<std::size_t> at = i; at; at = classes[*at].base)
            if (const auto it = classes[*at].fields.find(offset); it != classes[*at].fields.end()) return &it->second;
        return nullptr;
    };

    // ---- the declarations ----
    std::vector<std::string> lines = {"#pragma pack(push, 1)"};
    for (std::size_t i = 0; i < classes.size(); ++i) {
        const Cx64Cls& k = classes[i];
        std::vector<std::string> bases;
        for (const cx::BaseClass& b : k.rtti->bases)
            if (b.direct) bases.push_back(b.name + (b.mdisp != 0 ? std::format(" at 0x{:x}", b.mdisp) : "") + (b.is_virtual() ? " (virtual)" : ""));
        lines.push_back("");
        lines.push_back(std::format("/* {}: run-time type information {}, its table at 0x{:x} ({} slots){} */", k.rtti->name, k.rtti->decorated,
                                    k.rtti->primary()->address, k.slots.size(), bases.empty() ? "" : "; bases " + text::join(bases, ", ")));
        std::string open;
        for (const std::string& s : k.scope) open += "namespace " + s + " { ";
        lines.push_back(open + "class " + k.leaf + (k.base ? " : public ::" + classes[*k.base].cpp : "") + " {");
        lines.push_back("public:");
        for (const Cx64Decl& d : k.decls) {
            const Cx64Sig& s = sigs.at(d.family);
            std::vector<std::string> params;
            for (std::size_t a = 0; a < s.params.size(); ++a) params.push_back(s.params[a] + " arg" + std::to_string(a + 2));
            std::string note;
            const std::string fn = name_at(d.function);
            if (!d.pure && !fn.empty()) note = "   /* " + fn + " */";
            lines.push_back(std::format("    virtual {} vf{}({}){}{};{}", s.ret, d.slot, text::join(params, ", "), d.overrides ? " override" : "",
                                        d.pure ? " = 0" : "", note));
            ++out.virtuals;
        }
        for (const auto& [n, o] : owners) {
            if (o.cls != i || o.is_virtual) continue;
            const Cx64Parsed& p = parsed.at(n);
            const std::string member = (o.ctor ? "ctor_" : "dtor_") + n.substr(n.find('_') + 1);
            std::vector<std::string> params;
            for (std::size_t a = 1; a < p.params.size(); ++a) params.push_back(cx_decl(p.params[a].first, p.params[a].second));
            const bool returns_this = !p.params.empty() && p.ret == p.params[0].first;
            lines.push_back(std::format("    {} {}({});", returns_this ? k.leaf + " *" : p.ret, member, text::join(params, ", ")));
        }
        std::int64_t at = k.base ? extent(*k.base) : 8;
        std::map<std::int64_t, std::string> sub_objects;   // the offsets of the tables of the bases after the first
        for (const cx::BaseClass& b : k.rtti->bases)
            if (b.direct && b.mdisp != 0 && !b.is_virtual()) sub_objects.emplace(b.mdisp, b.name);
        for (const auto& [off, f] : k.fields) {
            if (off > at) lines.push_back(std::format("    uint1 _pad_{}[{}];", cx_hex(at), off - at));
            const auto sub = sub_objects.find(off);
            lines.push_back("    " + cx_decl(f.type, "f_" + cx_hex(off)) + ";" +
                            (sub != sub_objects.end() ? "   /* the table pointer of the " + sub->second + " sub-object */" : ""));
            at = off + f.bytes;
            ++out.fields;
        }
        std::string close = "};";
        for (std::size_t s = 0; s < k.scope.size(); ++s) close += " }";
        lines.push_back(close);
        ++out.classes;
    }
    lines.push_back("#pragma pack(pop)");
    out.declarations = text::join(lines, "\n") + "\n";

    // ---- the methods' definitions ----
    static const std::regex returns(R"(\breturn\b([^;]*);)");
    for (const auto& [n, o] : owners) {
        const Cx64Parsed& p = parsed.at(n);
        const Cx64Cls& k = classes[o.cls];
        Method m;
        m.cls = k.cpp;
        m.va = 0;
        for (const auto& [va, name] : *in.by_va)
            if (name == n) m.va = va;
        m.is_virtual = o.is_virtual;
        std::string ret = p.ret;
        std::vector<std::string> params;
        std::vector<std::string> locals;
        if (o.is_virtual) {
            const Cx64Decl& d = k.decls[o.decl];
            const Cx64Sig& s = sigs.at(d.family);
            m.member = "vf" + std::to_string(d.slot);
            m.slot = d.slot;
            ret = s.ret;
            for (std::size_t a = 0; a < s.params.size(); ++a) {
                if (a + 1 < p.params.size()) {
                    const auto& [type, pname] = p.params[a + 1];
                    if (type == s.params[a]) {
                        params.push_back(cx_decl(type, pname));
                        continue;
                    }
                    locals.push_back(std::format("  {} = ({})arg{};", cx_decl(type, pname), type, a + 2));
                }
                params.push_back(s.params[a] + " arg" + std::to_string(a + 2));
            }
        } else {
            m.member = (o.ctor ? "ctor_" : "dtor_") + n.substr(n.find('_') + 1);
            for (std::size_t a = 1; a < p.params.size(); ++a) params.push_back(cx_decl(p.params[a].first, p.params[a].second));
            if (!p.params.empty() && p.ret == p.params[0].first) ret = "::" + k.cpp + " *";
        }
        m.qualified = k.cpp + "::" + m.member;
        // `this`: the fields as members where every use of the first parameter is a field or the pointer passed on
        std::string body = p.body;
        if (!p.params.empty()) {
            const std::string& self = p.params[0].second;
            const core16::StructNote* note = this_note(p);
            std::map<std::int64_t, const core16::StructField*> own;
            if (note != nullptr)
                for (const core16::StructField& f : note->fields) own.try_emplace(f.offset, &f);
            bool members = note != nullptr;
            const std::string tried = cx_each_word(body, self, [&](std::string_view after, std::size_t& used) -> std::string {
                if (after.starts_with("->f_")) {
                    std::size_t e = 4;
                    while (e < after.size() && cx_word(after[e])) ++e;
                    const auto offset = text::parse_int<std::int64_t>(after.substr(4, e - 4), 16);
                    const auto f = offset ? own.find(*offset) : own.end();
                    if (f == own.end()) {
                        members = false;
                        return self;
                    }
                    used = e;
                    const Cx64Field* member = field_of(o.cls, *offset);
                    if (member != nullptr && member->bytes == f->second->bytes && member->type == f->second->type) return "this->f_" + cx_hex(*offset);
                    const std::string type = member != nullptr ? member->type : (f->second->type.find('^') == std::string::npos ? f->second->type : "xunknown8");
                    if (*offset == 0) return "*(" + type + (cx_is_pointer(type) ? "*)this" : " *)this");
                    return "(*(" + type + (cx_is_pointer(type) ? "*" : " *") + ")((uint1 *)this + 0x" + cx_hex(*offset) + "))";
                }
                std::size_t s = 0;
                while (s < after.size() && after[s] == ' ') ++s;
                const std::string_view next = after.substr(s);
                const bool value = next.starts_with(",") || next.starts_with(")") || next.starts_with(";") || next.starts_with("==") || next.starts_with("!=");
                if (!value) members = false;
                return "this";
            });
            // (a field outside the structure, or the pointer used for anything else: the parameter is `this` as its own type)
            if (members) body = tried;
            else locals.insert(locals.begin(), std::format("  {} = ({})this;", cx_decl(p.params[0].first, self), p.params[0].first));
        }
        // every `return` gives the method's result type
        body = [&] {
            std::string b;
            std::size_t at = 0;
            for (auto it = std::sregex_iterator(body.begin(), body.end(), returns); it != std::sregex_iterator(); ++it) {
                b.append(body, at, static_cast<std::size_t>(it->position()) - at);
                const std::string e(text::trim((*it)[1].str()));
                if (ret == "void") b += "return;";
                else if (e.empty()) b += "return 0;";
                else if (e == "0" || (ret == p.ret && !cx_is_pointer(ret))) b += "return " + e + ";";
                else b += "return (" + ret + ")(" + e + ");";
                at = static_cast<std::size_t>(it->position() + it->length());
            }
            b.append(body, at, std::string::npos);
            return b;
        }();
        const std::size_t brace = body.find('{');
        std::string head = std::format("{}/* {} at 0x{:x}{} */\n{} {}::{}({})\n", p.lead, n, m.va, o.is_virtual ? std::format(", slot {} of {}", m.slot, k.cpp) : "",
                                       ret, k.cpp, m.member, text::join(params, ", "));
        std::string inside = "\n#pragma comment(linker, \"/ALTERNATENAME:" + n + "=\" __FUNCDNAME__)\n";
        for (const std::string& l : locals) inside += l + "\n";
        body = body.substr(0, brace + 1) + inside + std::string(text::trim(body.substr(brace + 1), "\n"));
        out.texts[n] = head + body;
        out.methods[n] = m;
    }
    return out;
}

}  // namespace cb::decompile::cpp_text_x64
