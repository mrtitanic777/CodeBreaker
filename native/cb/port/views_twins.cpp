#include <algorithm>
#include <format>
#include <tuple>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_c_data.hpp"
#include "cb/port/old_c_passes.hpp"
#include "cb/port/old_c_scan.hpp"
#include "cb/port/views.hpp"
#include "cb/port/views_internal.hpp"

namespace cb::port::pre_ansi {

using base::ctext::blank;
using namespace views_detail;

namespace {

struct TwMember {
    std::string name, base, declarator;
};
struct TwBody {
    std::size_t begin = 0, end = 0;   // between the braces
    std::vector<TwMember> members;
};

// The members of `struct tag { ... }` in a text (size_t is a type word there).
std::optional<TwBody> tw_members_in(const std::string& text, std::string_view tag) {
    const std::string code = blank(text);
    const auto m = rx_name(R"(\bstruct\s+@@\s*\{([^{}]*)\})", tag).search(code);
    if (!m) return std::nullopt;
    TwBody out;
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

using TwSig = std::vector<std::pair<std::uint32_t, std::uint32_t>>;   // (offset, size) of each member
TwSig tw_sig(const StructLayout& l) {
    TwSig s;
    for (const Member& m : l.members) s.emplace_back(m.offset, m.size);
    return s;
}

std::vector<std::string> tw_sorted_names(const Files& files) {
    std::vector<std::string> names;
    for (const auto& kv : files) names.push_back(kv.first);
    std::sort(names.begin(), names.end());
    return names;
}

// The end of the `{ ... }` table whose '{' stands just before `from`, plus one: where the scan stopped (the text's end when it never closes).
std::size_t tw_table_scan_end(std::string_view code, std::size_t from) {
    int d = 1;
    std::size_t k = from;
    while (k < code.size() && d != 0) {
        d += (code[k] == '{') - (code[k] == '}');
        ++k;
    }
    return k;
}

}  // namespace

// ================================================================ cast_twins

void cast_twins(ViewContext& cx) {
    const StructLayouts lays = old_layouts_of(cx.originals);
    std::map<std::string, std::string> blanked;   // (the code of each file; dropped when a file changes)
    auto code_of = [&](const std::string& file) -> const std::string& {
        auto it = blanked.find(file);
        if (it == blanked.end()) it = blanked.emplace(file, blank(cx.files.at(file))).first;
        return it->second;
    };
    std::map<std::string, std::vector<std::string>> where;   // (a tag: the files that define it)
    static const Regex struct_open(R"(\bstruct\s+(\w+)\s*\{)");
    for (const auto& kv : cx.files)
        struct_open.for_each_match(code_of(kv.first), [&](const RegexMatch& m) {
            where[std::string(m.group(1))].push_back(kv.first);
            return true;
        });
    static const Regex old_definition(R"((?m)^(?:\w+\s+\**\s*|\w+\s*\*+\s*)?(\w+)\s*\(([\w\s,]*)\)\s*\n((?:[^{;]*;\s*)*)\{)");
    static const Regex struct_param(R"(\bstruct\s+(\w+)\s*\*\s*(\w+)\s*[;,])");
    static const Regex outer_struct(R"(\bstruct\s+(\w+)\s+\**\s*(\w+)\s*[\[;=,])");
    FirstSeenMap<std::pair<std::string, std::size_t>, std::pair<std::string, std::string>> params;   // ((function, index): (file, tag)) of struct T * ones
    std::map<std::string, std::set<std::string>> glob;   // (variable: tag) of the files' outer struct ones
    for (const auto& kv : cx.files) {
        const std::string& code = code_of(kv.first);
        old_definition.for_each_match(code, [&](const RegexMatch& m) {
            std::vector<std::string> names;
            for (std::string_view w : text::split(m.group(2), ',')) {
                const std::string_view s = text::trim(w);
                if (!s.empty()) names.emplace_back(s);
            }
            struct_param.for_each_match(m.group(3), [&](const RegexMatch& d) {
                const auto at = std::find(names.begin(), names.end(), std::string(d.group(2)));
                if (at != names.end())
                    params.assign({std::string(m.group(1)), static_cast<std::size_t>(at - names.begin())},
                                            std::make_pair(kv.first, std::string(d.group(1))));
                return true;
            });
            return true;
        });
        const std::string outer = [&] {   // (outside every function)
            std::string out;
            int depth = 0;
            for (char c : code) {
                depth += c == '{';
                depth -= c == '}';
                out.push_back(depth == 0 || c == '{' || c == '}' ? c : ' ');
            }
            return out;
        }();
        outer_struct.for_each_match(outer, [&](const RegexMatch& d) {
            glob[std::string(d.group(2))].emplace(d.group(1));
            return true;
        });
    }
    std::map<std::string, std::set<std::string>> fvars;   // (a function: the function pointers given it)
    static const Regex fp_decl(R"(\(\s*\*\s*(\w+)\s*\)\s*\(\s*\))");
    static const Regex fp_init(R"(\(\s*\*\s*(\w+)\s*\)\s*\(\s*\)\s*=\s*(\w+)\s*[;,])");
    static const Regex fp_assign(R"((?<![\w.>])(\w+)\s*=\s*&?\s*(\w+)\s*;)");
    for (const auto& kv : cx.files) {
        const std::string& code = code_of(kv.first);
        std::set<std::string> fps;
        fp_decl.for_each_match(code, [&](const RegexMatch& m) {
            fps.emplace(m.group(1));
            return true;
        });
        fp_init.for_each_match(code, [&](const RegexMatch& m) {
            fvars[std::string(m.group(2))].emplace(m.group(1));
            return true;
        });
        fp_assign.for_each_match(code, [&](const RegexMatch& m) {
            if (fps.contains(std::string(m.group(1)))) fvars[std::string(m.group(2))].emplace(m.group(1));
            return true;
        });
    }
    struct Call {
        std::string line;
        std::optional<std::vector<Span>> args;
    };
    std::map<std::pair<std::string, std::string>, std::vector<Call>> call_memo;   // ((file, function): its calls; dropped when a file changes)
    // each call of fn in a file (a call through one of its pointers too: the line, as a call of the name, and its arguments)
    auto calls = [&](const std::string& file, const std::string& fn) -> const std::vector<Call>& {
        const auto key = std::make_pair(file, fn);
        if (const auto it = call_memo.find(key); it != call_memo.end()) return it->second;
        std::vector<Call> out;
        const std::string& code = code_of(file);
        rx_name(R"((?<![\w.>])@@\s*\()", fn).for_each_match(code, [&](const RegexMatch& cm) {
            const std::string ln(line_at(code, cm.start()));
            out.push_back({ln, call_args(ln, fn)});
            return true;
        });
        if (const auto fv = fvars.find(fn); fv != fvars.end())
            for (const std::string& v : fv->second)
                rx_name(R"(\(\s*\*\s*@@\s*\)\s*\()", v).for_each_match(code, [&](const RegexMatch& cm) {
                    std::string ln(line_at(code, cm.start()));
                    ln = rx_name(R"(\(\s*\*\s*@@\s*\)\s*\()", v).replace_all(ln, [&](const RegexMatch&) { return v + "("; });
                    out.push_back({ln, call_args(ln, v)});
                    return true;
                });
        return call_memo.emplace(key, std::move(out)).first->second;
    };
    // the structure a variable of a file points to: as it is declared there, or the file-level one of that name
    auto kinds = [&](const std::string& file, std::string_view var) -> std::optional<std::string> {
        std::set<std::string> tags;
        std::string first;
        rx_name(R"(\bstruct\s+(\w+)\s*\*\s*@@\s*[;,=)])", var).for_each_match(code_of(file), [&](const RegexMatch& m) {
            if (tags.empty()) first = std::string(m.group(1));
            tags.emplace(m.group(1));
            return true;
        });
        if (tags.size() == 1) return first;
        const auto g = glob.find(std::string(var));
        if (g != glob.end() && g->second.size() == 1) return *g->second.begin();
        return std::nullopt;
    };
    auto changed = [&](const std::string& file) {
        blanked.erase(file);
        call_memo.clear();
    };
    static const Regex ampersand_name(R"(&?\w+)");
    static const Regex struct_or_union(R"((?:struct|union)\s+(\w+))");
    auto pointer = [](const TwMember& d) { return has(d.declarator, '*') || has(d.base, '*'); };
    auto local_ok = [&](const TwMember& d) {
        const auto m = struct_or_union.full_match(d.base);
        if (!m || has(d.declarator, '*')) return true;
        const std::string tag = d.base.substr(d.base.rfind(' ') + 1);
        const auto w = where.find(tag);
        if (w == where.end()) return false;
        return std::any_of(w->second.begin(), w->second.end(), [](const std::string& h) { return h.ends_with(".h"); });
    };
    // each word: the pointer, of whichever has it
    auto merged = [&](const std::vector<TwMember>& mine, const std::vector<TwMember>& other, const std::string& me_tag, const std::string& other_tag) {
        std::vector<std::string> lines;
        for (std::size_t k = 0; k < mine.size(); ++k) {
            const TwMember& a = mine[k];
            if (k < other.size() && pointer(other[k]) && !pointer(a)) {
                const TwMember& o = other[k];
                lines.push_back("    " + text::replace_all(o.base, "struct " + other_tag, "struct " + me_tag) + " " +
                                rx_name(R"(\b@@\b)", o.name).replace_all(o.declarator, [&](const RegexMatch&) { return a.name; }) + ";");
            } else {
                lines.push_back("    " + a.base + " " + a.declarator + ";");
            }
        }
        return text::join(lines, "\n");
    };
    for (const std::string& n : tw_sorted_names(cx.files)) {
        if (!n.ends_with(".c")) continue;
        std::vector<std::string> xs;   // (the structures the file defines, as it was when the pass reached it)
        struct_open.for_each_match(code_of(n), [&](const RegexMatch& xm) {
            xs.emplace_back(xm.group(1));
            return true;
        });
        for (const std::string& x : xs) {
            const auto wx = where.find(x);
            if (wx == where.end() || wx->second.size() != 1) continue;
            std::set<std::string> ys;
            for (const auto& [key, owner] : params) {   // (a struct X * parameter given a Y, or a struct Y * parameter given an X)
                const auto& [fn, i] = key;
                const auto& [pf, tag] = owner;
                for (const auto& file : cx.files) {
                    for (const Call& call : calls(file.first, fn)) {
                        if (!call.args || i >= call.args->size()) continue;
                        const std::string_view arg = text::trim(std::string_view(call.line).substr((*call.args)[i].begin, (*call.args)[i].size()));
                        if (!ampersand_name.full_match(arg)) continue;
                        const auto at = kinds(file.first, arg.starts_with('&') ? arg.substr(arg.find_first_not_of('&')) : arg);
                        if (tag == x && at && *at != x && pf == n) {
                            ys.insert(*at);
                        } else if (at && *at == x && tag != x && file.first == n) {
                            ys.insert(tag);
                        }
                    }
                }
            }
            std::erase_if(ys, [&](const std::string& y) {   // (one the other's first words)
                const StructLayout *ly = lays.find("struct " + y), *lx = lays.find("struct " + x);
                if (!ly || !lx) return true;
                const TwSig a = tw_sig(*ly), b = tw_sig(*lx);
                const std::size_t k = std::min(a.size(), b.size());
                const bool prefix = !a.empty() && !b.empty() && std::equal(a.begin(), a.begin() + static_cast<std::ptrdiff_t>(k), b.begin());
                return !prefix || !where.contains(y);
            });
            if (ys.size() != 1) continue;
            const std::string y = *ys.begin();
            const auto mx = tw_members_in(cx.files.at(n), x);
            std::vector<std::string> y_files = where.at(y);
            std::stable_sort(y_files.begin(), y_files.end(), [](const std::string& a, const std::string& b) {
                return !a.ends_with(".h") < !b.ends_with(".h");
            });
            const auto my = tw_members_in(cx.files.at(y_files.front()), y);
            if (!mx || !my) continue;
            const std::size_t common = std::min(mx->members.size(), my->members.size());
            auto shape = [](const TwMember& d) { return std::make_pair(d.base, text::replace_all(d.declarator, d.name, "@")); };
            bool alike = true, differs = false, all_local = true, mine_ptr = false, theirs_ptr = false;
            for (std::size_t k = 0; k < common; ++k) {
                const TwMember &a = mx->members[k], &b = my->members[k];
                alike = alike && shape(a) == shape(b);
                differs = differs || pointer(a) != pointer(b);
                all_local = all_local && local_ok(a) && local_ok(b);
                theirs_ptr = theirs_ptr || (pointer(b) && !pointer(a));
                mine_ptr = mine_ptr || (pointer(a) && !pointer(b));
            }
            if (alike) continue;           // (alike already)
            if (!differs) continue;        // (no word a pointer in one, a number in the other)
            if (!all_local) continue;      // (a whole structure of one file's own: not moved)
            const std::string bx = merged(mx->members, my->members, x, y);
            if (theirs_ptr) {
                const std::string& t = cx.files.at(n);
                cx.files.insert_or_assign(n, t.substr(0, mx->begin) + std::format("\t/* (as struct {} is: one structure, two names) */\n{}\n", y, bx) + t.substr(mx->end));
                changed(n);
                note(cx.notes, "a structure that is another's, under its own name (cast to it, the old bytes alike): its members of the other's types",
                     std::format("{} (struct {} as struct {})", n, x, y));
            }
            if (mine_ptr) {
                for (const std::string& w : where.at(y)) {
                    const auto my_now = tw_members_in(cx.files.at(w), y);
                    if (!my_now) continue;
                    const std::string& tw = cx.files.at(w);
                    cx.files.insert_or_assign(w, tw.substr(0, my_now->begin) +
                                                     std::format("\t/* (as struct {} is: one structure, two names) */\n{}\n", x, merged(my_now->members, mx->members, y, x)) +
                                                     tw.substr(my_now->end));
                    changed(w);
                    note(cx.notes, "a structure that is another's, under its own name (cast to it, the old bytes alike): its members of the other's types",
                         std::format("{} (struct {} as struct {})", w, y, x));
                }
            }
        }
    }
}

// ================================================================ twin_structs

void twin_structs(ViewContext& cx) {
    const StructLayouts lays = old_layouts_of(cx.originals);
    OrderedMap<std::string, std::string> hdr;   // (a structure: the header of the original that defines it)
    static const Regex struct_open(R"(\bstruct\s+(\w+)\s*\{)");
    for (const auto& [h, t] : cx.originals) {
        if (!h.ends_with(".h")) continue;
        struct_open.for_each_match(blank(t), [&](const RegexMatch& m) {
            hdr.try_emplace("struct " + std::string(m.group(1)), h);
            return true;
        });
    }
    static const Regex struct_body(R"(\bstruct\s+(\w+)\s*\{([^{}]*)\})");
    std::vector<std::pair<std::string, std::string>> snapshot(cx.files.begin(), cx.files.end());
    for (const auto& [n, t] : snapshot) {
        if (!n.ends_with(".c")) continue;
        const std::string code = blank(t);
        std::size_t from = 0;
        while (const auto m = struct_body.search(code, from)) {
            from = m->end();
            const std::string x = "struct " + std::string(m->group(1));
            const StructLayout* lx = lays.find(x);
            if (!lx || hdr.contains(x)) continue;
            std::vector<std::string> twins;
            for (const auto& [y, ly] : lays) {
                const std::string* header = hdr.find(y);
                if (y == x || !header || ly.members.size() < 3 || tw_sig(ly) != tw_sig(*lx)) continue;
                if (Regex(R"(#\s*include\s*")" + regex_quote(*header) + R"(")").search(t)) twins.push_back(y);
            }
            if (twins.size() != 1) continue;
            const std::string y_tag = twins[0].substr(twins[0].rfind(' ') + 1);   // (and the file takes one for the other: a function it declares giving an X that gives a Y, or a cast of a Y)
            bool evident = false;
            rx_name(R"(\bstruct\s+@@\s*\*\s*(\w+)\s*\(\s*\))", m->group(1)).for_each_match(code, [&](const RegexMatch& fm) {
                for (const auto& [n2, t2] : cx.originals)
                    if (n2 != n && Regex(R"((?m)^\s*(?:static\s+)?struct\s+)" + regex_quote(y_tag) + R"(\s*\*\s*\n?\s*)" + regex_quote(fm.group(1)) + R"(\s*\()").search(blank(t2)))
                        evident = true;
                return true;
            });
            if (!evident) continue;
            const StructLayout& ly = lays.at(twins[0]);
            const std::size_t common = std::min(lx->members.size(), ly.members.size());
            bool wanted = false;   // (no int where the other has a pointer: as it is)
            for (std::size_t k = 0; k < common; ++k) wanted = wanted || (!lx->members[k].pointer && ly.members[k].pointer);
            if (!wanted) continue;
            std::vector<std::string> body;
            for (std::size_t k = 0; k < common; ++k) {
                const Member &mx = lx->members[k], &my = ly.members[k];
                body.push_back("    " + my.type + " " + rx_name(R"(\b@@\b)", my.name).replace_all(my.declarator, [&](const RegexMatch&) { return mx.name; }) + ";");
            }
            const std::string now = cx.files.at(n);
            cx.files.insert_or_assign(n, now.substr(0, m->start(2)) + std::format("\t/* (as {} is: its bytes) */\n{}\n", twins[0], text::join(body, "\n")) + now.substr(m->end(2)));
            note(cx.notes, "a file's own structure that is another's bytes, ints where it has pointers: its members of the other's types",
                 std::format("{} ({} as {})", n, x, twins[0]));
            break;
        }
    }
}

// ================================================================ struct_views

void struct_views(ViewContext& cx) {
    const StructLayouts lays = old_layouts_of(cx.originals);
    struct Shape {
        std::string name, type, declarator;   // a member: its name, its type collapsed, its declarator with the name as @ and no spaces
    };
    OrderedMap<std::string, std::vector<Shape>> shapes;
    for (const auto& [tag, lay] : lays) {
        if (is_nested(tag) || !tag.starts_with("struct ") || lay.members.size() < 2) continue;
        if (!std::any_of(lay.members.begin(), lay.members.end(), [](const Member& m) { return has(m.declarator, '['); })) continue;
        std::vector<Shape> mem;
        for (const Member& m : lay.members) {
            std::string decl = rx_name(R"(\b@@\b)", m.name).replace_all(m.declarator, [](const RegexMatch&) { return std::string("@"); });
            decl.erase(std::remove(decl.begin(), decl.end(), ' '), decl.end());
            mem.push_back({m.name, squeeze(m.type), decl});
        }
        shapes.insert_or_assign(tag, std::move(mem));
    }
    if (shapes.empty()) return;
    std::string alltext;
    {
        std::vector<std::string> blanked;
        for (const auto& kv : cx.files) blanked.push_back(blank(kv.second));
        alltext = text::join(blanked, "\n");
    }
    for (const std::string& n : tw_sorted_names(cx.files)) {
        if (!n.ends_with(".c")) continue;
        const std::string t = cx.files.at(n);
        const std::vector<GlobalDefinition> gl = global_definitions(t);
        std::vector<std::string> lines = split_lines(t);
        std::set<std::size_t> done;
        for (std::size_t i = 0; i < gl.size(); ++i) {
            if (done.contains(i) || !rx_name(R"(&\s*@@\b)", gl[i].name).search(alltext)) continue;
            for (const auto& [st, mem] : shapes) {
                if (i + mem.size() > gl.size()) continue;
                bool fits = true;
                for (std::size_t k = 0; k < mem.size(); ++k) {
                    if (k > 0 && gl[i + k].line - gl[i + k - 1].line != 1) fits = false;
                    if (gl[i + k].base != mem[k].type || gl[i + k].shape != mem[k].declarator) fits = false;
                }
                if (!fits) continue;
                const std::string var = gl[i].name + "_s";
                std::vector<std::string> inits, names;
                for (std::size_t k = 0; k < mem.size(); ++k) {
                    const GlobalDefinition& g = gl[i + k];
                    inits.push_back(g.initializer ? *g.initializer : has(g.shape, '[') ? "{ 0 }" : "0");
                    names.push_back(g.name);
                }
                lines[gl[i].line] = std::format("{} {} = {{ {} }};\t/* ({}: one place, as the old linker kept them) */", st, var,
                                                text::join(inits, ", "), text::join(names, ", "));
                for (std::size_t k = 1; k < mem.size(); ++k) lines[gl[i + k].line] = "";
                for (std::size_t k = 0; k < mem.size(); ++k) {
                    cx.aliases.insert_or_assign(gl[i + k].name, Alias{"(" + var + "." + mem[k].name + ")", st + " " + var + ";"});
                    cx.view_vars.insert(gl[i + k].name);
                }
                for (const auto& [h, ht] : cx.files)
                    if (h.ends_with(".h") && rx_name(R"(\b@@\s*\{)", st).search(blank(ht))) cx.view_headers.insert(h);
                note(cx.notes, "globals one after another that are a structure's members, the first's place taken: one variable of it",
                     std::format("{} ({}: {})", n, st, text::join(names, ", ")));
                for (std::size_t k = 0; k < mem.size(); ++k) done.insert(i + k);
                break;
            }
        }
        cx.files.insert_or_assign(n, join_lines(lines));
    }
}

// ================================================================ word_columns

namespace {

// int a, v = 1, b;  ->  intptr_t v = 1; int a, b;
std::string tw_widen(const RegexMatch& m, std::string_view v) {
    const std::string pre(m.group(1)), init(m.group(3));
    const std::string_view end = m.group(4);
    std::string rest(text::trim(m.group(2)));
    while (!rest.empty() && rest.back() == ',') rest.pop_back();
    rest = std::string(text::trim(rest));
    const std::string own = std::format("{}intptr_t {}{};", pre, v, init);
    if (end == ";" && rest.empty()) return own;
    std::string tail = rest.empty() ? "" : rest + (end == "," ? ", " : "");
    const std::string_view lead = text::trim(pre);
    return own + " " + (lead.empty() ? "" : std::string(lead) + " ") + "int " + tail + (end == "," ? "" : ";");
}

}  // namespace

void word_columns(ViewContext& cx) {
    const StructLayouts lays = old_layouts_of(cx.originals);
    std::set<std::string> ptrs, ints;
    for (const auto& kv : cx.files)
        for (const GlobalDefinition& g : global_definitions(kv.second)) {
            if (has(g.shape, '*') || g.base.starts_with("struct") || g.base.starts_with("union")) {
                ptrs.insert(g.name);
            } else if ((g.base == "int" || g.base == "unsigned" || g.base == "short" || g.base == "unsigned int") && g.shape == "@") {
                ints.insert(g.name);
            }
        }
    static const Regex table_head(R"(\bstruct\s+(\w+)\s+\w+\s*\[[^\]]*\]\s*=\s*\{)");
    static const Regex row_rx(R"(\{([^{}]*)\})");
    static const Regex ptr_cast_of(R"(\(\s*(?:int|unsigned|short)\s*\*\s*\)\s*&\s*(\w+))");
    static const Regex ptr_cast(R"(\(\s*(?:int|unsigned|short)\s*\*\s*\))");
    static const Regex address_of(R"(&\s*(\w+))");
    static const Regex int_type(R"((?:int|unsigned|short|unsigned int))");
    OrderedMap<std::string, std::set<std::size_t>> cols;   // (struct: the member columns given pointers' places)
    for (const auto& kv : cx.files) {
        const std::string code = blank(kv.second);
        table_head.for_each_match(code, [&](const RegexMatch& m) {
            const StructLayout* lay = lays.find("struct " + std::string(m.group(1)));
            if (!lay) return true;
            const std::size_t k = tw_table_scan_end(code, m.end());
            const std::size_t stop = k > 0 ? std::max(m.end(), k - 1) : m.end();
            row_rx.for_each_match(std::string_view(code).substr(m.end(), stop - m.end()), [&](const RegexMatch& row) {
                std::size_t c = 0;
                for (std::string_view raw : split_top(row.group(1), ',')) {
                    const std::string e(text::trim(raw));
                    const auto x = ptr_cast_of.full_match(e);
                    if (x && ptrs.contains(std::string(x->group(1))) && c < lay->members.size() &&
                        int_type.full_match(text::trim(lay->members[c].type)) &&
                        [&] {
                            std::string d = lay->members[c].declarator;
                            d.erase(std::remove(d.begin(), d.end(), ' '), d.end());
                            return d == "*" + lay->members[c].name;
                        }())
                        cols[std::string(m.group(1))].insert(c);
                    ++c;
                }
                return true;
            });
            return true;
        });
    }
    if (cols.empty()) return;
    std::set<std::string> widen;
    for (std::size_t f = 0; f < cx.files.size(); ++f) {
        const std::string n = cx.files.entries()[f].first;
        std::string t = cx.files.at(n);
        const std::string code = blank(t);
        struct Change {
            std::size_t begin, end;
            std::string text;
        };
        std::vector<Change> changes;
        table_head.for_each_match(code, [&](const RegexMatch& m) {
            const auto* col = cols.find(std::string(m.group(1)));
            if (!col) return true;
            const std::size_t k = tw_table_scan_end(code, m.end());
            const std::size_t stop = k > 0 ? std::max(m.end(), k - 1) : m.end();
            row_rx.for_each_match(std::string_view(code).substr(m.end(), stop - m.end()), [&](const RegexMatch& row) {
                const std::size_t base_at = m.end() + row.start(1);
                std::size_t pos = 0, c = 0;
                for (std::string_view e : split_top(row.group(1), ',')) {
                    const std::size_t at = base_at + pos + (e.size() - text::ltrim(e).size());
                    pos += e.size() + 1;
                    const std::string_view es = text::trim(e);
                    const std::size_t column = c++;
                    if (!col->contains(column)) continue;
                    const auto x = ptr_cast_of.full_match(es);
                    const auto y = address_of.full_match(es);
                    const auto z = ptr_cast.match(es);
                    if (x || (y && cx.view_vars.contains(std::string(y->group(1))))) {
                        changes.push_back({at, at + es.size(), "(intptr_t *)&" + std::string((x ? *x : *y).group(1))});
                    } else if (z) {   // ((int *)table: another thing's place: so cast)
                        changes.push_back({at, at + z->end(), "(intptr_t *)"});
                    } else if (y && ints.contains(std::string(y->group(1)))) {
                        widen.insert(std::string(y->group(1)));
                    }
                }
                return true;
            });
            return true;
        });
        for (auto it = changes.rbegin(); it != changes.rend(); ++it) t.replace(it->begin, it->end - it->begin, it->text);
        cx.files.insert_or_assign(n, t);
    }
    for (const auto& [tag, columns] : cols) {
        const StructLayout& lay = lays.at("struct " + tag);
        for (std::size_t f = 0; f < cx.files.size(); ++f) {
            const std::string n = cx.files.entries()[f].first;
            const std::string t = cx.files.at(n);
            const auto sm = rx_name(R"(\bstruct\s+@@\s*\{([^{}]*)\})", tag).search(t);
            if (!sm) continue;
            std::string body(sm->group(1));
            std::vector<std::string> names;
            for (const std::size_t c : columns) {
                const std::string& member = lay.members.at(c).name;
                body = rx_name(R"((?m)^([ \t]*)(?:int|unsigned|short|unsigned int)\s*\*\s*@@\s*;)", member)
                           .replace_all(body, "${1}intptr_t *" + member + ";\t/* (a word's place - a pointer's in some rows) */");
                names.push_back(member);
            }
            cx.files.insert_or_assign(n, t.substr(0, sm->start(1)) + body + t.substr(sm->end(1)));
            note(cx.notes, "a table's int * column given pointers' places too (a word was either): intptr_t *, the ints placed in it intptr_t",
                 std::format("{} (struct {}: {})", n, tag, text::join(names, ", ")));
        }
    }
    for (std::size_t f = 0; f < cx.files.size(); ++f) {   // (the ints: intptr_t, wherever declared)
        const std::string n = cx.files.entries()[f].first;
        std::string t = cx.files.at(n);
        for (const std::string& v : widen)
            t = rx_name(R"((?m)^([ \t]*(?:extern\s+|static\s+)?)(?:int|unsigned|short|unsigned int)\s+([^;(){}]*)\b@@\b(\s*(?:=[^,;]*)?)\s*([,;]))", v)
                    .replace_all(t, [&](const RegexMatch& m) { return tw_widen(m, v); });
        cx.files.insert_or_assign(n, t);
    }
    if (!widen.empty())
        note(cx.notes, "an int whose place a column of pointers' places is given: intptr_t", text::join(widen, ", "));
}

// ================================================================ bss_ends

std::string bss_ends(ViewContext& cx, std::string text, std::string_view fname) {
    if (cx.bss.empty()) return text;
    std::vector<std::string> names;
    for (const auto& kv : cx.bss) names.push_back(kv.first);
    std::set<std::string> chars;   // (the char arrays of the originals)
    static const Regex char_array(R"(\bchar\s+(\w+)\s*\[)");
    for (const auto& kv : cx.originals)
        char_array.for_each_match(blank(kv.second), [&](const RegexMatch& m) {
            chars.emplace(m.group(1));
            return true;
        });
    std::set<std::string> used;
    static const Regex address(R"(\(\s*char\s*\*\s*\)\s*&\s*(\w+)\b(?!\s*(?:\[|->|\.)))");
    text = base::ctext::sub_code(text, address, [&](const RegexMatch& m) -> std::string {
        const std::string v(m.group(1));
        if (!cx.bss.contains(v) || chars.contains(v)) return std::string(m.whole());
        const std::size_t k = static_cast<std::size_t>(std::find(names.begin(), names.end(), v) - names.begin());
        if (k == 0 || !chars.contains(names[k - 1])) return std::string(m.whole());
        const std::string& prev = names[k - 1];
        const std::uint64_t bytes = cx.bss.at(prev);
        note(cx.notes, "the place of the variable after a char array (the old linker's order): the array's end",
             std::format("{} (&{}: {} + {})", fname, v, prev, bytes));
        used.insert(prev);
        return std::format("({} + {})", prev, bytes);
    });
    for (const std::string& prev : used) {   // (the array declared where its end is taken)
        if (rx_name(R"(\b@@\s*\[)", prev).search(blank(text))) continue;
        const std::size_t at = text.find("#include \"port.h\"");
        if (at != std::string::npos) text.insert(at + 17, "\nextern char " + prev + "[];");
    }
    return text;
}

// ================================================================ what the program does to members and pointers

const std::set<std::string>& copied_members(ViewContext& cx) {
    if (!cx.caches.copied) {
        std::set<std::string> out;
        static const Regex copy(R"(\.\s*(\w+)\s*=\s*[\w.>()\[\] -]*?\.\s*(\w+)\s*;)");
        for (const auto& kv : cx.originals)
            copy.for_each_match(blank(kv.second), [&](const RegexMatch& m) {
                if (m.group(1) == m.group(2)) out.emplace(m.group(1));
                return true;
            });
        cx.caches.copied = std::move(out);
    }
    return *cx.caches.copied;
}

const std::set<std::string>& ptr2_names(ViewContext& cx) {
    if (!cx.caches.ptr2) {
        std::set<std::string> out;
        static const Regex pointer_to_pointer(R"(\bchar\s*\*\s*\*\s*(\w+)\s*[;,=)\[])");
        for (const auto& kv : cx.originals)
            pointer_to_pointer.for_each_match(blank(kv.second), [&](const RegexMatch& m) {
                out.emplace(m.group(1));
                return true;
            });
        cx.caches.ptr2 = std::move(out);
    }
    return *cx.caches.ptr2;
}

std::optional<std::string> pointee_of(std::string_view text, std::string_view var) {
    const std::string c = blank(text);
    auto m = rx_name(R"(\b((?:struct|union)\s+\w+)\s*\*\s*(?:\w+\s*,\s*\*?\s*)*@@\b\s*[;,)=\[])", var).search(c);
    if (!m) m = rx_name(R"(\b((?:struct|union)\s+\w+)\s+@@\s*\[)", var).search(c);
    if (!m) return std::nullopt;
    return squeeze(m->group(1));
}

// ================================================================ pass_through_params

void pass_through_params(ViewContext& cx) {
    if (cx.caches.pass_done) return;
    cx.caches.pass_done = true;
    std::vector<std::pair<std::string, std::string>> texts;   // (the C files)
    for (const auto& kv : cx.files)
        if (kv.first.ends_with(".c")) texts.push_back(kv);
    std::map<std::string, std::vector<NamedDefinition>> spans;   // (the functions of each file, found once)
    auto functions_of = [&](const std::string& file, const std::string& text) -> const std::vector<NamedDefinition>& {
        auto it = spans.find(file);
        if (it == spans.end()) {
            const std::vector<std::string> lines = split_lines(text), code = split_lines(blank(text));
            std::vector<NamedDefinition> all = definitions_any(lines, code);
            for (const Definition& d : definitions(lines, code)) all.push_back({d.fn.name, d.first, d.open, d.last});
            it = spans.emplace(file, std::move(all)).first;
        }
        return it->second;
    };
    static const Regex number_or_null(R"(0|\(\s*\w+\s*\*\s*\)\s*0)");
    static const Regex before_use(R"([(,]\s*(?:\([^()]*\)\s*)?$)");
    static const Regex after_use(R"(\s*[,)])");
    static const Regex argument_var(R"(&?\s*(\w+)(?:\s*\[[^\]]*\])?)");
    std::vector<Fn> fns;
    for (const auto& kv : cx.fns) fns.push_back(kv.second);
    for (const Fn& fn : fns) {
        std::string body = fn.body;
        if (body.empty()) {
            const std::string file = fn.file.value_or("");
            const auto src = std::find_if(texts.begin(), texts.end(), [&](const auto& t) { return t.first == file; });
            if (src != texts.end()) {
                for (const NamedDefinition& d : functions_of(src->first, src->second))
                    if (d.name == fn.name) {
                        const std::vector<std::string> code = split_lines(blank(src->second));
                        body = join_range(code, d.open, d.last);
                        break;
                    }
            }
        }
        for (std::size_t k = 0; k < fn.params.size(); ++k) {
            const auto& [pn, pt] = fn.params[k];
            const auto m = rx_name(R"(\s*(?:register\s+)?((?:struct|union)\s+\w+)\s*\*\s*@@\s*)", pn).full_match(pt);
            if (!m || body.empty()) continue;
            const Regex use = rx_name(R"(\b@@\b)", pn);
            std::size_t uses = 0;
            bool only_given_on = true;
            use.for_each_match(body, [&](const RegexMatch& u) {
                ++uses;
                if (!before_use.search(std::string_view(body).substr(0, u.start())) || !after_use.match(std::string_view(body).substr(u.end())))
                    only_given_on = false;
                return only_given_on;
            });
            if (uses == 0 || !only_given_on) continue;   // (used otherwise than given on: its own type)
            std::set<std::optional<std::string>> given;
            for (const auto& [n, t] : texts) {
                const std::string code = blank(t);
                rx_name(R"(\b@@\s*\()", fn.name).for_each_match(code, [&](const RegexMatch& c) {
                    const std::string_view line = line_at(code, c.start());
                    if (rx_name(R"([\w\s*]*\b@@\s*\([^;]*\)\s*$)", fn.name).match(line)) return true;   // (its definition)
                    const auto sp = call_args(std::string_view(code).substr(c.start(), 400), fn.name);
                    if (!sp || k >= sp->size()) return true;
                    const std::string_view a = text::trim(std::string_view(code).substr(c.start() + (*sp)[k].begin, (*sp)[k].size()));
                    if (a.empty() || number_or_null.full_match(a)) return true;   // (none given; a declaration's f())
                    const auto x = argument_var.full_match(a);
                    given.insert(x ? pointee_of(t, x->group(1)) : std::nullopt);
                    return true;
                });
            }
            if (given.size() == 1 && given.begin()->has_value() && **given.begin() != m->group(1)) {
                const std::string y = **given.begin();
                cx.param_types[{fn.name, k}] = y + " *" + pn;
                note(cx.mended, "a pointer parameter only given on, its callers all giving another structure's: that one's (any pointer two bytes then)",
                     std::format("{} ({}: {})", fn.name, pn, y));
            }
        }
    }
}

}  // namespace cb::port::pre_ansi
