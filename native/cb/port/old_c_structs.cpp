// cb/port/old_c_structs.cpp -- fixed_allocators, local_copies, byte_places, shared_blocks, first_word_copies
// (old_c_passes.hpp): the passes about structures' old sizes and layouts.

#include <algorithm>
#include <format>
#include <set>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_c_passes.hpp"
#include "cb/port/old_c_scan.hpp"
#include "cb/port/struct_layout.hpp"

namespace cb::port::pre_ansi {

namespace {

using base::ctext::blank;

// The member of a layout that begins first (the lowest name among those at the same place), with its place.
const Member* first_member(const StructLayout& layout) {
    const Member* first = nullptr;
    for (const Member& m : layout.members)
        if (!first || std::tie(m.offset, m.name) < std::tie(first->offset, first->name)) first = &m;
    return first;
}

// The old size of the largest of the structures.
std::uint32_t largest_size(const StructLayouts& old, const std::vector<std::string>& structs) {
    std::uint32_t largest = 0;
    for (const std::string& s : structs) largest = std::max(largest, old.at(s).size);
    return largest;
}

}  // namespace

// ================================================================ fixed_allocators
void fixed_allocators(ProgramState& program) {
    static const Regex cast_call(R"re(\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*(\w+)\s*\(\s*\))re");
    static const Regex alloc_of_size(R"re(\b\w*alloc\w*\s*\(\s*(0[xX][0-9a-fA-F]+|\d+)\s*\))re");

    Files& files = program.files;
    const StructLayouts& old = program.original.layouts();
    std::map<std::string, std::set<std::string>> users;   // allocator -> the structures its callers cast it to
    for (const auto& kv : files)
        cast_call.for_each_match(blank(kv.second), [&](const RegexMatch& m) {
            users[std::string(m.group(2))].insert(collapse_space(m.group(1)));
            return true;
        });
    for (const std::string& n : sorted_names(files)) {
        std::vector<std::string> lines = split_lines(files.at(n));
        const std::vector<std::string> code = split_lines(blank(files.at(n)));
        for (const NamedDefinition& d : definitions_any(lines, code)) {
            const auto it = users.find(d.name);
            if (it == users.end()) continue;
            std::vector<std::string> structs;
            for (const std::string& s : it->second)
                if (old.contains(s)) structs.push_back(s);
            if (structs.empty()) continue;
            for (std::size_t x = d.open; x <= d.last && x < lines.size(); ++x) {
                const auto m = alloc_of_size.search(code[x]);
                if (!m) continue;
                const std::uint64_t size = *c_integer(m->group(1));
                const std::uint32_t largest = largest_size(old, structs);
                if (size < largest) continue;   // (not the structures' blocks)
                std::string expr = "sizeof(" + structs[0] + ")";
                for (std::size_t i = 1; i < structs.size(); ++i)
                    expr = std::format("(sizeof({0}) > {1} ? sizeof({0}) : {1})", structs[i], expr);
                if (const std::uint64_t rest = size - largest) expr += " + " + std::to_string(rest);
                lines[x].replace(m->start(1), m->end(1) - m->start(1), expr);
                program.lists.allocators.push_back(std::format("{}:{} ({}: {} for {})", n, x + 1, d.name, size, text::join(structs, ", ")));
            }
        }
        files.insert_or_assign(n, join_lines(lines));
    }
}

// ================================================================ local_copies
void local_copies(ProgramState& program) {
    static const Regex struct_head(R"re(\bstruct\s+(\w+)\s*\{)re");
    static const Regex local_definition(R"re((?m)^struct\s+(\w+)\s*\{[^{}]*\}\s*;[ \t]*\n?)re");

    Files& files = program.files;
    std::map<std::string, std::string> heads;   // a structure's tag: the header that defines it (the first)
    for (const auto& [h, t] : files)
        if (h.ends_with(".h"))
            struct_head.for_each_match(blank(t), [&](const RegexMatch& m) {
                heads.try_emplace(std::string(m.group(1)), h);
                return true;
            });
    if (heads.empty()) return;
    std::vector<std::string> header_texts;
    for (const std::string& h : sorted_names(files))
        if (h.ends_with(".h")) header_texts.push_back(files.at(h));
    const StructLayouts header_layouts = lay_out_structs(header_texts, 2, 4);

    NamedRegex member_use(R"re(((?:->|\.)\s*)@1@\b)re");
    NamedRegex own_variable(R"re(\bstruct\s+@1@\s*\*\s*(\w+))re");
    NamedRegex extern_variable(R"re((?m)^\s*extern\s+struct\s+@1@\s*\*\s*(\w+))re");
    NamedRegex included(R"re(#\s*include\s*"@1@")re");

    for (const std::string& n : sorted_names(files)) {
        if (!n.ends_with(".c")) continue;
        std::string t = files.at(n);
        std::string code = blank(t);
        std::vector<int> depth = brace_depths(split_lines(code));
        std::set<std::string> done_tags;
        while (true) {   // (one at a time: the text moves under each)
            std::optional<RegexMatch> found;
            local_definition.for_each_match(code, [&](const RegexMatch& m) {
                if (done_tags.contains(std::string(m.group(1)))) return true;
                found = m;
                return false;
            });
            if (!found) break;
            const RegexMatch& m = *found;
            const std::string tag(m.group(1));
            done_tags.insert(tag);
            const auto home = heads.find(tag);
            if (home == heads.end() || depth[line_of(code, m.start())]) continue;
            const std::string definition(m.whole());
            const StructLayouts mine_layouts = lay_out_structs(std::vector<std::string>{definition}, 2, 4);
            const StructLayout* mine = mine_layouts.find("struct " + tag);
            const StructLayout* theirs = header_layouts.find("struct " + tag);
            if (!mine || !theirs || mine->size != theirs->size) continue;
            std::map<std::pair<std::uint32_t, std::uint32_t>, std::string> at_theirs;   // (offset, size) -> the header's member there
            for (const Member& w : theirs->members) at_theirs[{w.offset, w.size}] = w.name;
            struct Pun {
                std::string member, base, type;
            };
            std::vector<std::pair<std::string, std::string>> renamed;   // the file's member name -> the header's
            std::vector<Pun> puns;                                      // a file's int that is the header's two words
            bool ok = true;
            for (const Member& v : mine->members) {
                const auto o = at_theirs.find({v.offset, v.size});
                if (o == at_theirs.end()) {   // (int size; over char size, i9: the word there)
                    std::vector<std::tuple<std::uint32_t, std::uint32_t, std::string>> cover;
                    for (const Member& w : theirs->members)
                        if (v.offset <= w.offset && w.offset < v.offset + v.size) cover.emplace_back(w.offset, w.size, w.name);
                    std::sort(cover.begin(), cover.end());
                    const char* type = v.size == 2 ? "int16_t" : v.size == 4 ? "int32_t" : nullptr;
                    if (type && cover.size() > 1 && std::get<0>(cover.front()) == v.offset &&
                        std::get<0>(cover.back()) + std::get<1>(cover.back()) == v.offset + v.size && !v.pointer) {
                        puns.push_back({v.name, std::get<2>(cover.front()), type});
                        continue;
                    }
                    ok = false;
                    break;
                }
                if (o->second != v.name) renamed.emplace_back(v.name, o->second);
            }
            if (!ok) continue;
            std::string updated = t.substr(0, m.start()) + t.substr(m.end());
            for (const auto& [from, to] : renamed)   // (its uses: ->i0 the header's ->next)
                updated = base::ctext::sub_code(updated, member_use.with(from), [&](const RegexMatch& x) { return std::string(x.group(1)) + to; });
            std::set<std::string> mine_vars;
            own_variable.with(tag).for_each_match(blank(updated), [&](const RegexMatch& x) {
                mine_vars.insert(std::string(x.group(1)));
                return true;
            });
            for (const auto& other : files)   // (and the program's variables of it, the header's)
                extern_variable.with(tag).for_each_match(blank(other.second), [&](const RegexMatch& x) {
                    mine_vars.insert(std::string(x.group(1)));
                    return true;
                });
            for (const Pun& pun : puns) {   // (p->size on a struct ins * alone: other structures' ->size are their own)
                if (mine_vars.empty()) break;
                std::vector<std::string> quoted;
                for (const std::string& v : mine_vars) quoted.push_back(regex_quote(v));
                const Regex use(R"re((?<![\w.>])()re" + text::join(quoted, "|") + R"re()\s*->\s*)re" + regex_quote(pun.member) + R"re(\b)re");
                updated = base::ctext::sub_code(updated, use, [&](const RegexMatch& x) {
                    return std::format("(*({} *)&{}->{})", pun.type, x.group(1), pun.base);
                });
            }
            if (!included.with(home->second).test(updated)) {
                std::vector<std::string> lines = split_lines(updated);
                std::size_t at = 0;
                for (std::size_t i = 0; i < lines.size(); ++i)
                    if (lines[i].starts_with("#include")) at = i + 1;
                lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(at), "#include \"" + home->second + "\"");
                updated = join_lines(lines);
            }
            t = updated;
            files.insert_or_assign(n, t);
            code = blank(t);
            depth = brace_depths(split_lines(code));
            std::string renames;
            for (const auto& [from, to] : renamed) renames += (renames.empty() ? "" : ", ") + from + " as " + to;
            program.lists.local_copies.push_back(std::format("{} (struct {}{})", n, tag, renamed.empty() ? "" : ": " + renames));
        }
    }
}

// ================================================================ byte_places
void byte_places(ProgramState& program) {
    static const Regex byte_pointer_param(R"re((?:register\s+)?(?:unsigned\s+)?char\s*\*\s*\w+)re");
    static const Regex byte_cast(R"re(^\(\s*(?:unsigned\s+)?char\s*\*\s*\)\s*)re");

    Files& files = program.files;
    const StructLayouts& old = program.original.layouts();
    struct Where {
        std::string file;
        std::size_t first, last;
    };
    std::map<std::string, Fn> defs;   // the program's functions (the first definition of a name)
    std::map<std::string, Where> where;
    for (const auto& [n, t] : files) {
        if (!n.ends_with(".c")) continue;
        for (const Definition& d : definitions(split_lines(t), split_lines(blank(t)))) {
            defs.try_emplace(d.fn.name, d.fn);
            where.try_emplace(d.fn.name, Where{n, d.first, d.last});
        }
    }
    std::map<std::string, std::vector<std::size_t>> candidates;   // function -> the parameters that are `char *p`
    for (const auto& [name, f] : defs)
        for (std::size_t i = 0; i < f.params.size(); ++i)
            if (byte_pointer_param.full_match(text::trim(f.params[i].type))) candidates[name].push_back(i);
    if (candidates.empty()) return;

    std::map<std::pair<std::string, std::size_t>, std::set<std::optional<std::string>>> given;   // (function, place): the structures given there
    for (const auto& [n, t] : files) {
        if (!n.ends_with(".c")) continue;
        const std::string code = blank(t);
        const auto ptr_of = pointer_targets(code);
        for (const CallSite& site : call_sites(code, [&](std::string_view w) { return candidates.contains(std::string(w)); })) {
            const std::string callee(site.name);
            const Where& w = where.at(callee);
            const auto line = static_cast<std::int64_t>(line_of(code, site.at));
            if (w.file == n && line <= static_cast<std::int64_t>(w.first) + 1 && line >= static_cast<std::int64_t>(w.first) - 1)
                continue;   // (its own definition's head)
            const std::vector<Span> spans = call_args(std::string_view(code).substr(site.at, 400), callee).value_or(std::vector<Span>{});
            for (const std::size_t k : candidates.at(callee)) {
                if (k >= spans.size()) continue;
                const std::string arg =
                    byte_cast.replace_first(text::trim(std::string_view(code).substr(site.at + spans[k].begin, spans[k].size())), "");
                const bool plain = !arg.empty() && std::all_of(arg.begin(), arg.end(), is_word_char);
                const auto target = plain ? ptr_of.find(arg) : ptr_of.end();
                given[{callee, k}].insert(target != ptr_of.end() ? std::optional<std::string>(target->second) : std::nullopt);
            }
        }
    }
    NamedRegex byte_index(R"re(\b@1@\s*\[\s*(0[xX][0-9a-fA-F]+|\d+)\s*\])re");
    NamedRegex word_at(R"re(\*\s*\(\s*(?:int|short|unsigned|int16_t)\s*\*\s*\)\s*\(\s*@1@\s*\+\s*(0[xX][0-9a-fA-F]+|\d+)\s*\))re");
    for (const auto& [place, structs] : given) {
        const auto& [fn, k] = place;
        if (structs.size() != 1 || !structs.begin()->has_value()) continue;
        const std::string& st = **structs.begin();
        const StructLayout* lay = old.find(st);
        if (!lay) continue;
        std::map<std::pair<std::uint32_t, std::uint32_t>, std::string> at;   // (old offset, size) -> the member there
        for (const Member& m : lay->members)
            if (m.declarator.find('[') == std::string::npos) at[{m.offset, m.size}] = m.name;
        const Where& w = where.at(fn);
        const std::string& pn = defs.at(fn).params[k].name;
        std::vector<std::string> lines = split_lines(files.at(w.file));
        const std::string body = join_range(lines, w.first, w.last);
        std::string updated = byte_index.with(pn).replace_all(body, [&](const RegexMatch& m) {
            const auto it = at.find({static_cast<std::uint32_t>(*c_integer(m.group(1))), 1u});
            if (it == at.end()) return std::string(m.whole());
            return std::format("(({} *){})->{}", st, pn, it->second);
        });
        updated = word_at.with(pn).replace_all(updated, [&](const RegexMatch& m) {
            const std::uint64_t o = *c_integer(m.group(1));
            if (const auto it = at.find({static_cast<std::uint32_t>(o), 2u}); it != at.end())
                return std::format("(({} *){})->{}", st, pn, it->second);
            for (const Member& v : lay->members)   // (the high word of a long there: its upper half)
                if (v.size == 4 && v.offset < o && o < v.offset + 4 && !v.pointer)
                    return std::format("(((int16_t *)&(({} *){})->{})[{}])", st, pn, v.name, (o - v.offset) / 2);
            return std::string(m.whole());
        });
        if (updated == body) continue;
        std::vector<std::string> out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(w.first));
        for (const std::string& l : split_lines(updated)) out.push_back(l);
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(std::min(w.last + 1, lines.size())), lines.end());
        files.insert_or_assign(w.file, join_lines(out));
        program.lists.byte_places.push_back(std::format("{} ({}'s {}: {})", w.file, fn, pn, st));
    }
}

// ================================================================ shared_blocks
void shared_blocks(ProgramState& program) {
    static const Regex cast_to(R"re(\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*(\w+)\b(?!\s*\())re");
    static const Regex returns_pointer(R"re(\b((?:struct|union)\s+\w+)\s*\*\s*(\w+)\s*\()re");
    static const Regex assignment(R"re(\b(\w+)\s*=\s*(\w+)\s*\()re");
    static const Regex alloc_call(R"re(\b(\w+)\s*=\s*(?:\([^()]*\)\s*)?(\w+)\s*\()re");

    Files& files = program.files;
    const StructLayouts& old = program.original.layouts();
    const Allocators& allocs = program.original.allocators(files);
    std::map<std::string, std::set<std::string>> casts;   // struct X: the structures its objects are cast to
    for (const auto& kv : files) {
        const std::string code = blank(kv.second);
        const auto ptr_of = pointer_targets(code);
        cast_to.for_each_match(code, [&](const RegexMatch& m) {
            const auto x = ptr_of.find(m.group(2));
            const std::string s = collapse_space(m.group(1));
            if (x != ptr_of.end() && x->second != s) casts[x->second].insert(s);
            return true;
        });
        std::map<std::string, std::string, std::less<>> returns;   // f = f03fec(); struct fix *f, struct cell *f03fec(): the same
        returns_pointer.for_each_match(code, [&](const RegexMatch& m) {
            returns[std::string(m.group(2))] = collapse_space(m.group(1));
            return true;
        });
        assignment.for_each_match(code, [&](const RegexMatch& m) {
            const auto s = ptr_of.find(m.group(1));
            const auto x = returns.find(m.group(2));
            if (s != ptr_of.end() && x != returns.end() && s->second != x->second) casts[x->second].insert(s->second);
            return true;
        });
    }
    for (const std::string& n : file_names(files)) {
        if (!n.ends_with(".c")) continue;
        std::string t = files.at(n);
        const std::string code = blank(t);
        const auto ptr_of = pointer_targets(code);
        std::vector<Edit> edits;
        alloc_call.for_each_match(code, [&](const RegexMatch& m) {
            const auto xs = ptr_of.find(m.group(1));
            const auto* places = allocs.find(std::string(m.group(2)));
            if (xs == ptr_of.end() || !old.contains(xs->second) || !places || places->size() != 1) return true;
            const std::string& x = xs->second;
            const std::vector<Span> sp = call_args(std::string_view(code).substr(m.start(2), 400), m.group(2)).value_or(std::vector<Span>{});
            const std::size_t k = *places->begin();
            if (k >= sp.size()) return true;
            const std::size_t a = m.start(2) + sp[k].begin, b = m.start(2) + sp[k].end;
            const std::string_view v = text::trim(std::string_view(code).substr(a, b - a));
            const auto size = c_integer(v);
            if (!size || *size <= old.at(x).size) return true;
            std::vector<std::string> larger;   // (the largest it was made to hold: 18 of 20)
            if (const auto c = casts.find(x); c != casts.end())
                for (const std::string& s : c->second)
                    if (old.contains(s) && old.at(x).size < old.at(s).size && old.at(s).size <= *size) larger.push_back(s);
            if (larger.empty()) return true;
            std::stable_sort(larger.begin(), larger.end(), [&](const std::string& p, const std::string& q) { return old.at(p).size > old.at(q).size; });
            const std::size_t lead = (b - a) - text::ltrim(std::string_view(code).substr(a, b - a)).size();
            edits.push_back({a + lead, a + lead + v.size(), std::format("(sizeof({0}) > sizeof({1}) ? sizeof({0}) : sizeof({1}))", larger[0], x)});
            program.lists.shared_blocks.push_back(std::format("{} ({}, {}: {})", n, x, larger[0], *size));
            return true;
        });
        if (!edits.empty()) files.insert_or_assign(n, apply_edits(std::move(t), std::move(edits)));
    }
}

// ================================================================ first_word_copies
void first_word_copies(ProgramState& program) {
    static const Regex word_copy(
        R"re(\*\s*\(\s*(?:unsigned\s+)?(?:int|short|unsigned)\s*\*\s*\)\s*(\w+)\s*=\s*\*\s*\(\s*(?:unsigned\s+)?(?:int|short|unsigned)\s*\*\s*\)\s*(\w+)\s*;)re");

    Files& files = program.files;
    const StructLayouts& old = program.original.layouts();
    for (const std::string& n : file_names(files)) {
        if (!n.ends_with(".c")) continue;
        std::string t = files.at(n);
        const std::string code = blank(t);
        const auto ptr_of = pointer_targets(code);
        std::vector<Edit> edits;
        word_copy.for_each_match(code, [&](const RegexMatch& m) {
            const auto a = ptr_of.find(m.group(1)), b = ptr_of.find(m.group(2));
            if (a == ptr_of.end() || b == ptr_of.end() || a->second != b->second) return true;
            const StructLayout* lay = old.find(a->second);
            if (!lay) return true;
            const Member* first = first_member(*lay);
            if (!first || first->offset != 0 || first->size != 2) return true;
            edits.push_back({m.start(), m.end(), std::format("{0}->{2} = {1}->{2};", m.group(1), m.group(2), first->name)});
            return true;
        });
        if (edits.empty()) continue;
        const std::size_t count = edits.size();
        files.insert_or_assign(n, apply_edits(std::move(t), std::move(edits)));
        program.lists.word_copies.push_back(std::format("{} ({})", n, count));
    }
}

}  // namespace cb::port::pre_ansi
