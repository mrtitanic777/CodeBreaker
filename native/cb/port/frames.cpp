// cb/port/frames.cpp -- see frames.hpp.

#include "cb/port/frames.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_sizes.hpp"
#include "cb/port/struct_layout.hpp"

namespace cb::port::pre_ansi {

OrderedMap<std::string, LocalDecl> local_decls(const std::vector<std::string>& code, std::size_t open, std::size_t last,
                                               unsigned ptr) {
    static const Regex tagged(R"(\b(?:struct|union)\s+\w+)");
    OrderedMap<std::string, LocalDecl> out;
    for (std::size_t x = open + 1; x < last && x < code.size(); ++x) {
        const std::string_view s = text::trim(code[x]);
        if (s.empty()) continue;
        std::optional<Declaration> r;
        if (s.ends_with(';') && s.find('=') == std::string_view::npos &&
            text::replace_all(s, "(*", "").find('(') == std::string::npos)
            r = declarators(s);
        if (!r || r->base.empty()) break;
        bool all_type_words = true;
        const std::string untagged = tagged.replace_all(r->base, "");
        for (std::string_view w : text::split_ws(untagged))
            if (!is_type_word(w)) all_type_words = false;
        if (!all_type_words) break;
        for (const Declarator& dn : r->names) {
            out.insert_or_assign(dn.name, LocalDecl{r->base, dn.text, size_of(r->base, dn.text, dn.name, ptr, StructLayouts{}), x,
                                                    r->base.find("register") != std::string::npos});
        }
    }
    return out;
}

namespace {

using Lines = std::vector<std::optional<std::string>>;

// Gathers the locals the call of `g` reaches past the array `a` (it needs `e` bytes) into `struct { } frame;`. The
// declarations stay where the array was; the others' lines lose their names. A function whose neighbours cannot all be
// sized is left alone (false).
bool overlay(Notes& notes, const std::string& fname, std::vector<std::string>& lines,
             const std::vector<std::string>& code, std::size_t k, const OrderedMap<std::string, LocalDecl>& locs,
             const std::string& a, std::uint64_t e, const std::string& g) {
    static const Regex register_word(R"(\bregister\s+)");
    std::vector<const std::string*> names;
    for (const auto& kv : locs) names.push_back(&kv.first);
    std::size_t i = 0;
    while (i < names.size() && *names[i] != a) ++i;
    const LocalDecl& arr = locs.at(a);
    struct Gathered {
        std::string name, base, decl;
    };
    std::vector<Gathered> members = {{a, arr.base, arr.declarator}};
    std::uint64_t off = arr.size.value_or(0);
    // the frame grows down: those declared before it lie above, the nearest first
    for (std::size_t n = i; n-- > 0;) {
        const LocalDecl& d = locs.at(*names[n]);
        if (d.is_register) continue;
        if (!d.size) return false;
        if (*d.size > 1 && off % 2) ++off;
        members.push_back({*names[n], d.base, d.declarator});
        off += *d.size;
        if (off >= e) break;
    }
    if (off < e) return false;

    std::set<std::string> gone;
    std::set<std::size_t, std::greater<>> decl_lines;   // the declarations: the rest of each line kept
    for (const Gathered& m : members) {
        gone.insert(m.name);
        decl_lines.insert(locs.at(m.name).line);
    }
    Lines edited(lines.begin(), lines.end());
    for (const std::size_t x : decl_lines) {
        const auto r = declarators(text::trim(code[x]));
        std::vector<std::string> keep;
        for (const Declarator& dn : r->names)
            if (!gone.contains(dn.name)) keep.push_back(dn.text);
        const std::string ind(indent_of(lines[x]));
        if (!keep.empty())
            edited[x] = ind + r->base + " " + text::join(keep, ", ") + ";";
        else
            edited[x].reset();
    }
    const std::string ind = "\t";
    std::vector<std::string> decl;
    decl.push_back(ind + "struct {\t\t\t/* side by side, as the original's frame had them: " + g + " reads past " + a + " */");
    for (const Gathered& m : members) decl.push_back(ind + "\t" + register_word.replace_all(m.base, "") + " " + m.decl + ";");
    decl.push_back(ind + "} frame;");
    const std::size_t first = locs.at(a).line;   // where the array was declared
    {
        std::vector<std::string> pieces;
        if (edited[first]) pieces.push_back(*edited[first]);
        pieces.insert(pieces.end(), decl.begin(), decl.end());
        edited[first] = text::join(pieces, "\n");
    }
    std::string alternatives;
    for (const std::string& n : gone) alternatives += (alternatives.empty() ? "" : "|") + regex_quote(n);
    const Regex name_rx("(?<![.>\\w])(" + alternatives + ")\\b");   // (the names of this one overlay: compiled once)
    for (std::size_t x = first + 1; x <= k && x < edited.size(); ++x) {
        if (edited[x])
            edited[x] = base::ctext::sub_code(*edited[x], name_rx, [](const RegexMatch& m) { return "frame." + std::string(m.group(1)); });
    }
    std::string joined;
    bool any = false;
    for (const auto& l : edited) {
        if (!l) continue;
        if (any) joined += "\n";
        joined += *l;
        any = true;
    }
    lines = split_lines(joined);
    std::vector<std::string> names_in_order;
    for (const Gathered& m : members) names_in_order.push_back(m.name);
    note(notes, "locals the program reaches past an array (as the original's frame had them): kept side by side in a structure",
         fname + " (" + g + ": " + text::join(names_in_order, ", ") + ")");
    return true;
}

}  // namespace

void frames(Files& out, const OrderedMap<std::string, Fn>& fns, unsigned pointer_bytes, const LayoutFn& layout, Notes& notes) {
    static const Regex struct_ptr(R"(\b(struct \w+)\s*\*)");
    const unsigned ptr = pointer_bytes != 0 ? pointer_bytes : 2;
    std::vector<std::string> texts;
    for (const auto& kv : out) texts.push_back(kv.second);
    const StructLayouts lay = layout ? layout(texts, ptr, ptr) : lay_out_structs(texts, ptr, ptr);
    std::unordered_map<std::string, std::string> owner;   // member -> the first structure that has it (lookup only)
    for (const auto& kv : lay)
        for (const Member& m : kv.second.members) owner.try_emplace(m.name, kv.first);

    // (function, parameter index) -> the bytes of its pointer the function reads
    std::map<std::pair<std::string, std::size_t>, std::uint64_t> need;
    for (const auto& fkv : fns) {
        const Fn& f = fkv.second;
        for (std::size_t i = 0; i < f.params.size(); ++i) {
            const Param& prm = f.params[i];
            if (prm.type.find('*') == std::string::npos) continue;
            if (find_word(f.body, prm.name) == std::string::npos) continue;
            const std::optional<RegexMatch> own = struct_ptr.search(prm.type);
            const std::string q = regex_quote(prm.name);
            // `((struct S *)p)->m` or `p->m` (a pattern of this parameter's name: compiled once for it)
            const Regex reads(R"((?:\(\s*\(\s*(struct \w+)\s*\*\s*\)\s*)" + q + R"(\s*\)|\b)" + q + R"()\s*->\s*(\w+))");
            std::uint64_t e = 0;
            reads.for_each_match(f.body, [&](const RegexMatch& m) {
                const std::string member(m.group(2));
                std::optional<std::string> s;
                if (m.has(1)) {
                    s = std::string(m.group(1));
                } else if (own && lay.contains(std::string(own->group(1)))) {
                    s = std::string(own->group(1));
                } else if (const auto o = owner.find(member); o != owner.end()) {
                    s = o->second;
                }
                if (s) {
                    if (const StructLayout* sl = lay.find(*s)) {
                        if (const Member* mem = sl->find(member)) e = std::max<std::uint64_t>(e, mem->offset + mem->size);
                    }
                }
                return true;
            });
            if (e != 0) need[{f.name, i}] = e;
        }
    }
    // through the functions that hand the pointer on
    for (int round = 0; round < 8; ++round) {
        bool grew = false;
        for (const auto& fkv : fns) {
            const Fn& f = fkv.second;
            std::map<std::string, std::vector<std::vector<Span>>> calls_cache;   // this body's calls, by callee
            for (std::size_t i = 0; i < f.params.size(); ++i) {
                const std::vector<std::pair<std::pair<std::string, std::size_t>, std::uint64_t>> snapshot(need.begin(), need.end());
                for (const auto& [key, e] : snapshot) {
                    const auto& [g, k] = key;
                    auto it = calls_cache.find(g);
                    if (it == calls_cache.end()) it = calls_cache.emplace(g, calls_of(f.body, g)).first;
                    for (const std::vector<Span>& spans : it->second) {
                        if (k < spans.size() && text::trim(std::string_view(f.body).substr(spans[k].begin, spans[k].size())) == f.params[i].name) {
                            std::uint64_t& have = need[{f.name, i}];
                            if (have < e) {
                                have = e;
                                grew = true;
                            }
                        }
                    }
                }
            }
        }
        if (!grew) break;
    }
    if (need.empty()) return;

    for (auto& file : out) {
        if (!file.first.ends_with(".c")) continue;
        std::vector<std::string> lines = split_lines(file.second);
        std::vector<std::string> code = split_lines(base::ctext::blank(file.second));
        const std::vector<NamedDefinition> defs = definitions_any(lines, code);
        for (auto def = defs.rbegin(); def != defs.rend(); ++def) {
            const std::size_t j = def->open;
            std::size_t k = def->last;
            auto body_of = [&] {
                const std::vector<std::string> piece(code.begin() + static_cast<std::ptrdiff_t>(j),
                                                     code.begin() + static_cast<std::ptrdiff_t>(std::min(k + 1, code.size())));
                return join_lines(piece);
            };
            std::string body = body_of();
            OrderedMap<std::string, LocalDecl> locs = local_decls(code, j, k);
            auto arrays_of = [](const OrderedMap<std::string, LocalDecl>& l) {
                std::map<std::string, std::uint64_t> arrays;
                for (const auto& kv : l)
                    if (kv.second.declarator.find('[') != std::string::npos && kv.second.size && *kv.second.size != 0)
                        arrays[kv.first] = *kv.second.size;
                return arrays;
            };
            std::map<std::string, std::uint64_t> arrays = arrays_of(locs);
            for (const auto& [key, e] : need) {
                const auto& [g, idx] = key;
                // After an overlay the body has changed: its calls are listed again (the spans of the old listing
                // mean nothing in the new text), until no call is left that reaches past an array.
                for (bool again = true; again;) {
                    again = false;
                    for (const std::vector<Span>& spans : calls_of(body, g)) {
                        if (idx >= spans.size()) continue;
                        const std::string a(text::trim(std::string_view(body).substr(spans[idx].begin, spans[idx].size())));
                        const auto have = arrays.find(a);
                        if (have == arrays.end() || have->second >= e) continue;
                        const std::size_t before = lines.size();
                        if (!overlay(notes, file.first, lines, code, k, locs, a, e, g)) continue;
                        k += lines.size() - before;   // (the frame structure's lines)
                        code = split_lines(base::ctext::blank(join_lines(lines)));
                        body = body_of();
                        locs = local_decls(code, j, k);
                        arrays = arrays_of(locs);
                        again = true;
                        break;
                    }
                }
            }
        }
        file.second = join_lines(lines);
    }
}

}  // namespace cb::port::pre_ansi
