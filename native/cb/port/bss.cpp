// cb/port/bss.cpp -- see bss.hpp.

#include "cb/port/bss.hpp"

#include <algorithm>
#include <map>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_sizes.hpp"

namespace cb::port::pre_ansi {

namespace {

// `[ n ]` or `[]` -- the first bracket pair of a declarator, replaced by `[count]`
std::string with_first_dimension(const std::string& declarator, std::uint64_t count) {
    static const Regex bracket(R"(\[[^\]]*\])");
    return bracket.replace_first(declarator, "[" + std::to_string(count) + "]");
}

bool is_plain_integer_type(const std::string& base) {
    static const Regex integers(R"((?:unsigned\s+|signed\s+)?(?:int|short|unsigned))");
    return integers.full_match(base).has_value();
}

}  // namespace

BssPlan::BssPlan(OriginalProgram& original, std::vector<std::string> runtime_internals)
    : original_(original), runtime_internals_(std::move(runtime_internals)) {}

void BssPlan::set_sizes(BssSizes sizes) {
    sizes_ = std::move(sizes);
    decls_.reset();
}

const ExternDecls& BssPlan::extern_decls() {
    if (!decls_) {
        std::set<std::string> names;
        for (const auto& kv : sizes_) names.insert(kv.first);
        decls_ = original_.extern_decls(names);
    }
    return *decls_;
}

void BssPlan::bss_views(const Files& files, const OrderedMap<std::string, Fn>& fns, Aliases& aliases,
                        std::set<std::string>& view_headers, Notes& notes) {
    static const Regex struct_pointer_param(R"(\s*(?:register\s+)?((?:struct|union)\s+\w+)\s*\*\s*\w+\s*$)");
    static const Regex address_of(R"(&\s*(\w+))");
    if (sizes_.empty() || views_done_) return;
    views_done_ = true;
    const StructLayouts& old = original_.layouts();
    std::vector<std::string> names;                         // (each one's place from the first: the sizes in order)
    std::map<std::string, std::uint64_t> offs;
    std::uint64_t o = 0;
    for (const auto& kv : sizes_) {
        names.push_back(kv.first);
        offs[kv.first] = o;
        o += kv.second;
    }
    std::vector<std::string> codes;                         // (every file blanked once)
    for (const auto& kv : files) codes.push_back(base::ctext::blank(kv.second));

    std::map<std::string, std::string> found;               // variable -> the structure a function takes it for
    for (const auto& fkv : fns) {
        const Fn& fn = fkv.second;
        std::map<std::size_t, std::string> ks;              // parameter index -> the structure it points to
        for (std::size_t k = 0; k < fn.params.size(); ++k)
            if (const auto x = struct_pointer_param.match(fn.params[k].type)) ks[k] = collapse_space(x->group(1));
        if (ks.empty()) continue;
        for (const std::string& code : codes) {
            for (const std::size_t at : call_starts(code, fn.name)) {
                const std::string_view from_call = std::string_view(code).substr(at);
                const auto sp = call_args(from_call, fn.name).value_or(std::vector<Span>{});
                for (const auto& [k, st] : ks) {
                    if (k >= sp.size()) continue;
                    const std::string_view a = text::trim(from_call.substr(sp[k].begin, sp[k].size()));
                    const auto x = address_of.full_match(a);
                    if (!x) continue;
                    const std::string var(x->group(1));
                    const auto size = sizes_.find(var);
                    if (size != nullptr && old.contains(st) && *size < old.at(st).size) found[var] = st;
                }
            }
        }
    }
    for (const auto& [v, st] : found) {
        const StructLayout& lay = old.at(st);
        std::map<std::uint64_t, std::string> mem;           // the member at each old place (the last one a place has)
        for (const Member& m : lay.members) mem[m.offset] = m.name;
        const std::size_t k0 = static_cast<std::size_t>(std::find(names.begin(), names.end(), v) - names.begin());
        std::vector<std::pair<std::string, std::string>> run;
        bool ok = true;
        for (std::size_t i = k0; i < names.size(); ++i) {
            const std::uint64_t rel = offs[names[i]] - offs[v];
            if (rel >= lay.size) break;
            const auto m = mem.find(rel);
            if (m == mem.end()) {
                ok = false;
                break;
            }
            run.emplace_back(names[i], m->second);
        }
        if (!ok || run.empty()) continue;
        if (std::any_of(run.begin(), run.end(), [&](const auto& r) { return aliases.contains(r.first); })) continue;
        const std::string var = v + "_s";
        std::vector<std::string> run_names, run_notes;
        for (const auto& [nm, m] : run) {
            aliases.insert_or_assign(nm, Alias{"(" + var + "." + m + ")", st + " " + var + ";"});
            run_names.push_back(nm);
            run_notes.push_back(nm + "=." + m);
        }
        structs_.insert_or_assign(var, BssStruct{st, run_names});
        const Regex defines_struct(R"(\b)" + regex_quote(st, true) + R"(\s*\{)");   // (this structure's: compiled once)
        for (const auto& hf : files)
            if (hf.first.ends_with(".h") && defines_struct.test(base::ctext::blank(hf.second))) view_headers.insert(hf.first);
        note(notes,
             "variables of the BSS list one after another that a routine takes as a structure (its members' places): one variable of it",
             var + " (" + st + ": " + text::join(run_notes, ", ") + ")");
    }
}

bool BssPlan::join_bss(const std::string& var, Aliases& aliases, std::vector<DeclRule>& decl_rules) {
    static const Regex number_in_brackets(R"(\[(\d+)\])");
    std::vector<std::string> names;
    for (const auto& kv : sizes_) names.push_back(kv.first);
    std::string head = var;
    for (const auto& [h, rest] : joined_)
        if (std::find(rest.begin(), rest.end(), var) != rest.end()) {
            head = h;
            break;
        }
    std::vector<std::string> chain = {head};
    if (const auto* rest = joined_.find(head)) chain.insert(chain.end(), rest->begin(), rest->end());
    const auto last = std::find(names.begin(), names.end(), chain.back());
    if (last == names.end() || last + 1 == names.end()) return false;
    const std::string nxt = *(last + 1);
    const ExternDecls& decls = extern_decls();
    const auto* a = decls.find(head);
    const auto* b = decls.find(nxt);
    // (*v[64] and *w[64]: one shape)
    auto shape = [](const std::string& d) {
        static const Regex ident(R"([A-Za-z_]\w*)");
        static const Regex bracket(R"(\[[^\]]*\])");
        return bracket.replace_all(ident.replace_first(d, "@"), "[]");
    };
    if (a == nullptr || b == nullptr || a->first != b->first || shape(a->second) != shape(b->second) || structs_.contains(nxt))
        return false;
    {
        std::vector<std::string> now = joined_.contains(head) ? joined_.at(head) : std::vector<std::string>{};
        now.push_back(nxt);
        if (const auto* nxts = joined_.find(nxt)) now.insert(now.end(), nxts->begin(), nxts->end());
        joined_.erase(nxt);
        joined_.insert_or_assign(head, std::move(now));
    }
    definitions(aliases);   // (the places' names now: their externs dropped)
    const auto owner = aliases.find(nxt);
    if (owner != nullptr) {
        if (const auto n = number_in_brackets.search(owner->declaration))   // (and the array's own externs of its new size)
            decl_rules.emplace_back(R"((\bextern\b[^;\n]*?\b)" + regex_quote(head) + R"(\s*\[)\s*\d*\s*\])",
                                    "${1}" + std::string(n->group(1)) + "]");
    }
    return true;
}

std::vector<std::string> BssPlan::definitions(Aliases& aliases) {
    static const Regex storage_word(R"(^(?:extern|static)\s+)");
    static const Regex bracket_pairs(R"(\[\s*(\d+)\s*\])");
    static const Regex first_bracket(R"(\[[^\]]*\])");
    if (sizes_.empty()) return {};
    const StructLayouts& old = original_.layouts();
    std::set<std::string> defined;
    std::map<std::string, std::pair<std::string, std::string>> decl;   // name -> (base type, declarator)
    for (const auto& file : original_.files()) {
        if (!(file.first.ends_with(".c") || file.first.ends_with(".h"))) continue;
        const std::vector<std::string> code = split_lines(base::ctext::blank(file.second));
        const std::vector<int> ds = brace_depths(code);
        for (std::size_t i = 0; i < code.size(); ++i) {
            const std::string_view st = text::trim(code[i]);
            if (ds[i] != 0 || !st.ends_with(';') || text::replace_all(st, "(*", "").find('(') != std::string::npos) continue;
            const bool is_extern = st.starts_with("extern");
            std::string s = storage_word.replace_first(st, "");
            if (const std::size_t eq = s.find('='); eq != std::string::npos) s = s.substr(0, eq) + ";";
            while (s.ends_with(';')) s.pop_back();
            const auto r = declarators(s);
            if (!r) continue;
            for (const Declarator& dn : r->names) {
                if (!sizes_.contains(dn.name)) continue;
                if (is_extern) {   // (extern int g in one file, char *g in another: the pointer)
                    const auto was = decl.find(dn.name);
                    bool take = was == decl.end();
                    if (!take) {
                        const bool plain_int = is_plain_integer_type(was->second.first);
                        const bool name_alone_before = was->second.second == dn.name && dn.text != dn.name;
                        const bool pointer_over_non_pointer = dn.text.find('*') != std::string::npos &&
                                                              was->second.second.find('*') == std::string::npos && rule_enabled("bss_ptr_tables");
                        take = plain_int && (name_alone_before || pointer_over_non_pointer);
                    }
                    if (take) decl[dn.name] = {std::string(text::trim(r->base)), dn.text};
                } else {
                    defined.insert(dn.name);
                }
            }
        }
    }
    std::vector<std::string> out;
    std::set<std::string> members;
    for (const auto& [var, bs] : structs_) {
        out.push_back(bs.type + " " + var + ";\t/* (" + text::join(bs.names, ", ") + ": one place, as the old linker kept them) */");
        members.insert(bs.names.begin(), bs.names.end());
    }
    for (const auto& [nm, ignored] : sizes_) {
        (void)ignored;
        if (defined.contains(nm) || std::find(runtime_internals_.begin(), runtime_internals_.end(), nm) != runtime_internals_.end() ||
            nm.starts_with('_') || members.contains(nm))
            continue;
        bool is_place = false;
        for (const auto& kv : joined_)
            if (std::find(kv.second.begin(), kv.second.end(), nm) != kv.second.end()) is_place = true;
        if (is_place) continue;   // (a place in the array before it: joined)
        std::vector<std::string> parts = {nm};
        if (const auto* rest = joined_.find(nm)) parts.insert(parts.end(), rest->begin(), rest->end());
        std::uint64_t size = 0;
        for (const std::string& p : parts) size += sizes_.contains(p) ? sizes_.at(p) : 0;
        std::string base = "char", dcl = nm + "[]";
        if (const auto d = decl.find(nm); d != decl.end()) std::tie(base, dcl) = d->second;
        if (dcl.find('[') == std::string::npos) {
            out.push_back(base + " " + dcl + ";");
            continue;
        }
        std::string element = text::replace_all(first_bracket.replace_first(dcl, ""), nm, "");
        std::vector<std::uint64_t> inner;
        bracket_pairs.for_each_match(dcl, [&](const RegexMatch& m) {
            inner.push_back(text::parse_int<std::uint64_t>(m.group(1)).value_or(0));
            return true;
        });
        std::uint64_t esz = element.find('*') != std::string::npos ? 2 : old.contains(base) ? old.at(base).size : old_scalar_size(base).value_or(2);
        for (std::size_t i = 1; i < inner.size(); ++i) esz *= inner[i];
        const std::uint64_t room = base == "char" && esz == 1 ? 16 : 0;   // (bytes: a block's head may have grown)
        const std::uint64_t unit = std::max<std::uint64_t>(1, esz);
        if (parts.size() > 1) {   // (the names after it: places in it)
            std::uint64_t at = 0;
            const std::string full = base + " " + with_first_dimension(dcl, std::max<std::uint64_t>(1, size / unit) + (rule_enabled("bss_room") ? room : 0)) + ";";
            for (const std::string& p : parts) {
                if (p != nm) aliases.insert_or_assign(p, Alias{"(&" + nm + "[" + std::to_string(at / unit) + "])", full});
                at += sizes_.contains(p) ? sizes_.at(p) : 0;
            }
        }
        out.push_back(base + " " + with_first_dimension(dcl, std::max<std::uint64_t>(1, size / unit) + room) + ";\t/* (" +
                      std::to_string(size) + " bytes then" + (room ? ", and room" : "") + ") */");
    }
    return out;
}

}  // namespace cb::port::pre_ansi
