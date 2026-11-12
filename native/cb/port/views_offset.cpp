#include <algorithm>
#include <cctype>
#include <format>
#include <tuple>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_c_passes.hpp"
#include "cb/port/old_c_scan.hpp"
#include "cb/port/views.hpp"
#include "cb/port/views_internal.hpp"

namespace cb::port::pre_ansi {

using base::ctext::blank;
using namespace views_detail;

namespace {

// a member of the other structure at some place: its offset, size and name (a member of a structure in it: "a.b")
struct VoMember {
    std::uint32_t offset = 0, size = 0;
    std::string name;
    const Member* member = nullptr;
};

// The pointer's type in a layout is written with the pointer apart: the text of a type as the passes compare them.
std::string vo_type(const Member& m) { return squeeze(m.type); }

std::optional<std::string> vo_small_c_type(std::string_view type, bool with_long) {
    static const std::map<std::string, std::string, std::less<>> basic = {
        {"char", "char"}, {"unsigned char", "unsigned char"}, {"int", "int16_t"},          {"short", "int16_t"},
        {"unsigned", "uint16_t"}, {"unsigned int", "uint16_t"}, {"unsigned short", "uint16_t"}};
    const std::string t(text::trim(type));
    if (const auto it = basic.find(t); it != basic.end()) return it->second;
    if (with_long) {
        if (t == "long") return "int32_t";
        if (t == "unsigned long") return "uint32_t";
    }
    return std::nullopt;
}

// re.sub(r"\s*\*.*", "", declaration).strip(): the type before the first star
std::string vo_before_star(std::string_view declaration) {
    const std::size_t star = declaration.find('*');
    if (star == std::string_view::npos) return std::string(text::trim(declaration));
    return std::string(text::trim(text::rtrim(declaration.substr(0, star))));
}

// pt.split("*")[-1].strip(): the declaration's last word after its last star
std::string vo_after_star(std::string_view declaration) {
    const std::size_t star = declaration.rfind('*');
    return std::string(text::trim(star == std::string_view::npos ? declaration : declaration.substr(star + 1)));
}

// The first non-blank byte at or after / before a position (what lstrip / rstrip leave at the edge).
char vo_next_char(std::string_view s, std::size_t from) {
    while (from < s.size() && text::is_space(s[from])) ++from;
    return from < s.size() ? s[from] : '\0';
}
char vo_prev_char(std::string_view s, std::size_t to) {
    while (to > 0 && text::is_space(s[to - 1])) --to;
    return to > 0 ? s[to - 1] : '\0';
}

// The views a program's files have of other structures (one pass over the whole program: see ViewOf).
class ViewBuilder {
public:
    ViewBuilder(ViewContext& cx, bool views_cast) : cx_(cx), views_cast_(views_cast) {}
    ViewTable build(std::set<std::string>& weak_out);

private:
    ViewContext& cx_;
    bool views_cast_;
};

ViewTable ViewBuilder::build(std::set<std::string>& weak_out) {
    ViewContext& cx = cx_;
    ViewTable found;
    // the old layouts of the port's texts: headers first (tags it gave anonymous ones known)
    StructLayouts old;
    {
        std::vector<std::string> texts;
        for (const auto& kv : cx.files)
            if (kv.first.ends_with(".h")) texts.push_back(kv.second);
        for (const auto& kv : cx.files)
            if (!kv.first.ends_with(".h")) texts.push_back(kv.second);
        old = lay_out_structs(texts, 2, 4);
    }
    // a member's name: the structures it points to
    std::map<std::string, std::set<std::string>> ptr_mem;
    for (const auto& [tag, lay] : old) {
        if (is_nested(tag)) continue;
        for (const Member& m : lay.members)
            if (m.pointer && (m.type.starts_with("struct") || m.type.starts_with("union"))) ptr_mem[m.name].insert(vo_type(m));
    }
    static const Regex struct_open(R"(\bstruct\s+(\w+)\s*\{)");
    OrderedMap<std::string, std::string> local;   // (a .c file's own structure: its file)
    for (const auto& [n, t] : cx.files) {
        if (!n.ends_with(".c")) continue;
        struct_open.for_each_match(blank(t), [&](const RegexMatch& m) {
            local.try_emplace("struct " + std::string(m.group(1)), n);
            return true;
        });
    }
    // (#define L(s) ((struct lsym *)(s)): its uses are the casts)
    static const Regex cast_macro(R"((?m)^\s*#\s*define\s+(\w+)\s*\(\s*(\w+)\s*\)\s*\(\s*\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*\(?\s*\2\s*\)?\s*\)\s*$)");
    OrderedMap<std::string, std::string> expanded;
    for (const auto& [n, t] : cx.files) {
        std::string b = blank(t);
        std::vector<std::pair<std::string, std::string>> macros;
        cast_macro.for_each_match(b, [&](const RegexMatch& mm) {
            macros.emplace_back(std::string(mm.group(1)), std::string(mm.group(3)));
            return true;
        });
        for (const auto& [name, type] : macros)
            b = rx_name(R"(\b@@\s*\(\s*(\w+)\s*\))", name).replace_all(b, [&](const RegexMatch& u) { return "((" + type + " *)" + std::string(u.group(1)) + ")"; });
        expanded.insert_or_assign(n, std::move(b));
    }
    std::string alltext;
    {
        std::vector<std::string> all;
        for (const auto& kv : expanded) all.push_back(kv.second);
        alltext = text::join(all, "\n");
    }
    OrderedMap<std::string, std::string> heads;   // (a header's structure that never has an object of its own: its header)
    std::set<std::string> weak;
    for (const auto& [n, t] : cx.files) {   // (no variable, no sizeof: every pointer to it came from another's: a view too, if the evidence names one)
        if (!n.ends_with(".h")) continue;
        struct_open.for_each_match(blank(t), [&](const RegexMatch& m) {
            const std::string x = "struct " + std::string(m.group(1));
            if (local.contains(x)) return true;
            const std::string tag(m.group(1));
            const std::string no_pointers = rx_name(R"(\bstruct\s+@@\s*\*)", tag).replace_all(alltext, "");
            const bool own = rx_name(R"(\bstruct\s+@@\s+(?!\{)\**\s*\w+\s*(?:\[[^\]]*\]\s*)?[;=,])", tag).search(no_pointers) ||
                             rx_name(R"(sizeof\s*\(\s*struct\s+@@\s*\))", tag).search(alltext);
            if (!views_cast_) {
                if (own) return true;
            } else if (rx_name(R"((?m)^(?:static\s+|extern\s+)?struct\s+@@\s+(?!\{)\**\s*\w+\s*(?:\[[^\]]*\]\s*)?[;=,])", tag).search(no_pointers) ||
                       rx_name(R"((?m)^[ \t]+(?:register\s+|static\s+)?struct\s+@@\s+\w+\s*\[)", tag).search(alltext) ||
                       rx_name(R"(\bstruct\s+@@\s+\w+\s*[,)])", tag).search(alltext) ||
                       rx_name(R"(sizeof\s*\(\s*struct\s+@@\s*\))", tag).search(alltext)) {
                return true;   // (an object of its own at a file's top; a function's own, a parameter, an array)
            }
            if (own) weak.insert(x);   // (a function's own object of it, struct targ t;: a view only where one thing is cast to both)
            heads.try_emplace(x, n);
            return true;
        });
    }
    // where a function definition's head is, for the callers of a call: (end of its head, its name)
    struct Head {
        std::size_t end;
        std::string name;
    };
    std::vector<Head> definition_heads;
    {
        static const Regex head(R"((?m)^[\w\s*]*?\b(\w+)\s*\([^;{]*\)\s*$)");
        head.for_each_match(alltext, [&](const RegexMatch& m) {
            definition_heads.push_back({m.end(), std::string(m.group(1))});
            return true;
        });
    }
    auto caller_before = [&](std::size_t cut) -> std::optional<std::string> {   // (the nearest definition before a call)
        for (auto it = definition_heads.rbegin(); it != definition_heads.rend(); ++it)
            if (it->end <= cut && cx.fns.contains(it->name)) return it->name;
        return std::nullopt;
    };
    // (a variable of a parameter: as the loop typed it there)
    auto parameter_type_of = [&](const std::string& caller, std::string_view var) -> std::optional<std::string> {
        for (const auto& [key, pt] : cx.param_types)
            if (key.first == caller && vo_after_star(pt) == var) return vo_before_star(pt);
        return std::nullopt;
    };
    static const Regex dotted_arrow_tail(R"((?:->|\.)\s*\w+$)");
    static const Regex identifier(R"(\w+)");
    static const Regex plain_path(R"([\w.>\[\]-]+)");
    static const Regex word(R"(\w+)");
    std::vector<std::pair<std::string, std::string>> candidates(local.begin(), local.end());
    candidates.insert(candidates.end(), heads.begin(), heads.end());
    for (const auto& [x, n] : candidates) {
        const StructLayout* lx = old.find(x);
        if (!lx) continue;
        std::set<std::string> strong;   // (one thing cast to both: the surest evidence)
        const bool is_head = heads.contains(x);
        std::map<std::string, std::set<std::string>> pmx;   // (a header's: a member's name that one structure has, alone)
        if (is_head) {
            for (const auto& [k, v] : ptr_mem)
                if (v.size() == 1) pmx.emplace(k, v);
        }
        const std::map<std::string, std::set<std::string>>& members_to = is_head ? pmx : ptr_mem;
        auto pointed_by = [&](std::string_view member) -> const std::set<std::string>* {
            const auto it = members_to.find(std::string(member));
            return it == members_to.end() ? nullptr : &it->second;
        };
        std::set<std::string> ys;
        const std::string flex_x = regex_quote(x, true);
        auto add_returned = [&](std::string_view function, std::string_view search_in) {
            const auto rm = rx_name(R"(\b((?:struct|union)\s+\w+)\s*\*\s*@@\s*\()", function).search(search_in);
            if (rm && squeeze(rm->group(1)) != x) ys.insert(squeeze(rm->group(1)));
        };
        const Regex cast_to_x(R"(\(\s*)" + flex_x + R"(\s*\*\s*\)\s*([\w.>\[\]-]+))");
        cast_to_x.for_each_match(alltext, [&](const RegexMatch& m) {
            std::vector<std::string_view> words;
            identifier.for_each_match(m.group(1), [&](const RegexMatch& w) {
                words.push_back(w.whole());
                return true;
            });
            if (!words.empty()) {
                if (const auto* s = pointed_by(words.back())) ys.insert(s->begin(), s->end());
            }
            if (views_cast_ && is_head && dotted_arrow_tail.search(m.group(1))) {   // ((struct targ *)p->op and (struct psym *)p->op: one thing, two views: the one with objects of its own)
                Regex(R"(\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)\s*\(?\s*)" + regex_quote(m.group(1)) + R"((?![\w\[(]))")
                    .for_each_match(alltext, [&](const RegexMatch& o) {
                        const std::string oy = squeeze(o.group(1));
                        if (oy != x && !heads.contains(oy)) {
                            ys.insert(oy);
                            strong.insert(oy);
                        }
                        return true;
                    });
            }
            if (!words.empty() && word.full_match(m.group(1))) {   // (a variable's: as it is declared)
                std::optional<std::string> n_file;
                for (const auto& [file, text_of_file] : expanded)
                    if (Regex(R"(\(\s*)" + flex_x + R"(\s*\*\s*\)\s*)" + regex_quote(m.group(1)) + R"(\b)").search(text_of_file)) {
                        n_file = file;
                        break;
                    }
                std::optional<std::string> py;
                for (const auto& [key, pt] : cx.param_types) {
                    const Fn* f = cx.fns.find(key.first);
                    if (vo_after_star(pt) == m.group(1) && f && n_file && f->file.value_or("") == *n_file) py = vo_before_star(pt);
                }
                if (!py && n_file) py = pointee_of(cx.files.at(*n_file), m.group(1));
                if (py) ys = {*py};
            }
            return true;
        });
        Regex(R"(\(\s*)" + flex_x + R"(\s*\*\s*\)\s*\(?\s*[\w.>\[\]-]*?(?:->|\.)\s*(\w+)\s*\)?)")
            .for_each_match(alltext, [&](const RegexMatch& m) {
                rx_name(R"((?:->|\.)\s*@@\s*=\s*([A-Za-z_]\w*)\s*\()", m.group(1)).for_each_match(alltext, [&](const RegexMatch& am) {   // ((struct func *)node->i2, and n->i2 = f0b6cc(): a struct isym *: what the member holds)
                    add_returned(am.group(1), alltext);
                    return true;
                });
                return true;
            });
        const Regex variable_of_x(R"(\b)" + flex_x + R"(\s*\*\s*(\w+)\s*[;,=)])");
        for (const auto& [file, t] : expanded) {   // (or a variable of it given what a function of another's gives: p = f0b67a(sh), struct isym *)
            std::set<std::string> vars;
            variable_of_x.for_each_match(t, [&](const RegexMatch& m) {
                vars.emplace(m.group(1));
                return true;
            });
            for (const std::string& vv : vars) {
                const std::string both = t + "\n" + alltext;
                rx_name(R"((?<![\w.>])@@\s*=\s*([A-Za-z_]\w*)\s*\()", vv).for_each_match(t, [&](const RegexMatch& am) {
                    add_returned(am.group(1), both);
                    return true;
                });
            }
        }
        const Regex x_star(flex_x + R"(\s*\*)");
        for (const auto& [fname_, fn] : cx.fns) {   // (or what the callers give a parameter of it)
            static_cast<void>(fname_);
            for (std::size_t k = 0; k < fn.params.size(); ++k) {
                if (!x_star.search(fn.params[k].type)) continue;
                rx_name(R"(\b@@\s*\()", fn.name).for_each_match(alltext, [&](const RegexMatch& m) {
                    const auto sp = call_args(std::string_view(alltext).substr(m.start(), 400), fn.name);
                    if (!sp || k >= sp->size()) return true;
                    const std::string a(text::trim(std::string_view(alltext).substr(m.start() + (*sp)[k].begin, (*sp)[k].size())));
                    std::vector<std::string_view> words;
                    identifier.for_each_match(a, [&](const RegexMatch& w) {
                        words.push_back(w.whole());
                        return true;
                    });
                    if (!words.empty() && word.full_match(a)) {   // (a variable: as declared there, or as the loop typed a parameter)
                        const std::optional<std::string> caller = caller_before(m.start());
                        std::optional<std::string> py = caller ? parameter_type_of(*caller, a) : std::nullopt;
                        std::optional<std::string> cf;
                        if (caller)
                            if (const Fn* f = cx.fns.find(*caller)) cf = f->file;
                        if (!py && cf && cx.files.contains(*cf)) py = pointee_of(cx.files.at(*cf), a);
                        if (py) {
                            ys.insert(*py);
                            return true;
                        }
                    }
                    if (!words.empty() && plain_path.full_match(a)) {
                        if (const auto* s = pointed_by(words.back())) ys.insert(s->begin(), s->end());
                        if (rx_name(R"((?:->|\.)\s*@@$)", words.back()).search(a))   // (f(text, node->i2), n->i2 = f0b6cc(): a word holding a struct isym *)
                            rx_name(R"((?:->|\.)\s*@@\s*=\s*([A-Za-z_]\w*)\s*\()", words.back()).for_each_match(alltext, [&](const RegexMatch& am) {
                                add_returned(am.group(1), alltext);
                                return true;
                            });
                    }
                    return true;
                });
            }
        }
        std::erase_if(ys, [&](const std::string& y) { return local.contains(y) || y == x; });
        if (ys.size() > 1 && strong.size() == 1) ys = strong;   // (the other a header's: two files' own copies of one structure are each as it is)
        if (ys.size() != 1) continue;
        const std::string y = *ys.begin();
        const StructLayout* ly = old.find(y);
        if (!ly || y == x || (weak.contains(x) && strong != std::set<std::string>{y})) continue;
        // (the members of the other, and the members of a structure in it: c.op)
        std::vector<VoMember> ymem;
        for (const Member& v : ly->members) {
            ymem.push_back({v.offset, v.size, v.name, &v});
            const StructLayout* sub = nullptr;
            if (!v.pointer) {
                sub = old.find(vo_type(v));
                if (!sub) sub = old.find(y + "." + v.name);
            }
            if (sub && !has(v.declarator, '['))
                for (const Member& v2 : sub->members) ymem.push_back({v.offset + v2.offset, v2.size, v.name + "." + v2.name, &v2});
        }
        std::stable_sort(ymem.begin(), ymem.end(), [](const VoMember& a, const VoMember& b) {
            return std::make_pair(a.offset, -static_cast<long>(a.size)) < std::make_pair(b.offset, -static_cast<long>(b.size));
        });
        if (is_head) {   // (a header's: only when its link to its own kind is where the other's own link is: one list, two names)
            std::set<std::uint32_t> selfx, selfy;
            for (const Member& v : lx->members)
                if (v.pointer && vo_type(v) == x) selfx.insert(v.offset);
            for (const VoMember& ym : ymem)
                if (ym.member->pointer && vo_type(*ym.member) == y) selfy.insert(ym.offset);
            if (!selfx.empty() && !std::includes(selfy.begin(), selfy.end(), selfx.begin(), selfx.end())) continue;
            if (selfx.empty()) {   // (no link of its own: struct rseg over struct seg: its pointers each on one of the other's)
                std::vector<std::uint32_t> xptr;
                std::set<std::uint32_t> yptr;
                for (const Member& v : lx->members)
                    if (v.pointer) xptr.push_back(v.offset);
                for (const VoMember& ym : ymem)
                    if (ym.member->pointer) yptr.insert(ym.offset);
                const bool covered = !xptr.empty() && std::all_of(xptr.begin(), xptr.end(), [&](std::uint32_t o) { return yptr.contains(o); });
                if (!views_cast_ || (strong != std::set<std::string>{y} && !covered)) continue;   // (one thing cast to both)
            }
        }
        std::optional<std::map<std::string, ViewMember>> mapping = std::map<std::string, ViewMember>{};
        for (const Member& v : lx->members) {
            const std::uint32_t lo = v.offset, hi = v.offset + v.size;
            std::vector<const VoMember*> same;   // (one member just there: of its type the best)
            for (const VoMember& ym : ymem)
                if (ym.offset == lo && ym.size == v.size) same.push_back(&ym);
            if (!same.empty()) {
                const VoMember* best = nullptr;
                for (const VoMember* ym : same)
                    if (ym->member->type == v.type && ym->member->pointer == v.pointer) {
                        best = ym;
                        break;
                    }
                if (!best)
                    for (const VoMember* ym : same)
                        if (ym->member->pointer == v.pointer) {
                            best = ym;
                            break;
                        }
                if (!best) best = same.front();
                const auto ct0 = vo_small_c_type(vo_type(v), false);
                if (!v.pointer && best->member->pointer && ct0 && is_head) {   // (a number kept in the other's pointer's place: hi in left: the word there, read as a number)
                    ViewMember pun;
                    pun.pun = true;
                    pun.base = best->name;
                    pun.type = *ct0;
                    (*mapping)[v.name] = pun;
                    continue;
                }
                ViewMember names;
                names.names = {best->name};
                (*mapping)[v.name] = names;
                continue;
            }
            std::vector<const VoMember*> top;
            for (const VoMember& ym : ymem)
                if (!has(ym.name, '.')) top.push_back(&ym);
            std::vector<const VoMember*> cover;
            for (const VoMember* ym : top)
                if (lo <= ym->offset && ym->offset < hi) cover.push_back(ym);
            const auto ct = vo_small_c_type(vo_type(v), true);
            const VoMember* inside = nullptr;
            for (const VoMember* ym : top)
                if (ym->offset <= lo && hi <= ym->offset + ym->size && ym->size > v.size) {
                    inside = ym;
                    break;
                }
            if (inside && ct && !v.pointer && y.starts_with("struct ")) {   // (a union's all at 0: none)
                ViewMember pun;   // (a byte of a word of the other's: that byte)
                pun.pun = true;
                pun.base = inside->name;
                pun.offset = lo - inside->offset;
                pun.type = *ct;
                (*mapping)[v.name] = pun;
                continue;
            }
            if (cover.empty() || cover.front()->offset != lo || cover.back()->offset + cover.back()->size != hi) {
                mapping.reset();
                break;
            }
            if (cover.size() > 1 && v.size <= 2 && ct && !v.pointer && y.starts_with("struct ") &&
                std::all_of(cover.begin(), cover.end(), [](const VoMember* c) { return !c->member->pointer && !has(c->member->declarator, '['); })) {
                ViewMember pun;   // (a word over two bytes of the other's: the word there)
                pun.pun = true;
                pun.base = cover.front()->name;
                pun.type = *ct;
                (*mapping)[v.name] = pun;
                continue;
            }
            ViewMember names;
            for (const VoMember* c : cover) names.names.push_back(c->name);
            (*mapping)[v.name] = names;
        }
        if (mapping && !mapping->empty() && is_head) {   // (a header's: only a view whose link is the other's under another name: u6.n for next: two structures sharing a head, each its own, are not one)
            std::vector<std::string> links;
            for (const Member& v : lx->members)
                if (v.pointer && vo_type(v) == x) links.push_back(v.name);
            if (!links.empty() || !views_cast_) {
                bool all_self = true;
                for (const std::string& k : links) {
                    const auto it = mapping->find(k);
                    all_self = all_self && it != mapping->end() && !it->second.pun && it->second.names == std::vector<std::string>{k};
                }
                if (all_self) mapping.reset();
            }
            std::map<std::string, const Member*> ytop;
            for (const VoMember& ym : ymem) ytop[ym.name] = ym.member;
            std::vector<std::string> on_num;   // (the words of the other's that are numbers where this view has a pointer)
            if (mapping)
                for (const Member& v : lx->members) {
                    if (!v.pointer) continue;
                    const auto it = mapping->find(v.name);
                    if (it == mapping->end() || it->second.pun) continue;
                    for (const std::string& w : it->second.names) {
                        const auto yt = ytop.find(w);
                        if (yt == ytop.end() || !yt->second->pointer) on_num.push_back(w);
                    }
                }
            static const Regex int_word(R"((?:unsigned\s+)?(?:int|short|unsigned))");
            const bool two_byte_words = std::all_of(on_num.begin(), on_num.end(), [&](const std::string& w) {
                const auto yt = ytop.find(w);
                return yt != ytop.end() && yt->second->size == 2 && int_word.full_match(text::trim(yt->second->type));
            });
            if (!on_num.empty() && strong == std::set<std::string>{y} && two_byte_words) {   // (struct lab over struct psym, its refs where psym's cls is: the number a pointer's width, to hold what the other view keeps there)
                for (const std::string& w : on_num)
                    for (std::size_t f = 0; f < cx.files.size(); ++f) {
                        const std::string h = cx.files.entries()[f].first;
                        if (const auto nw = widen_member(cx.files.at(h), y.substr(y.rfind(' ') + 1), w)) cx.files.insert_or_assign(h, *nw);
                    }
                cx.view_wide.push_back(std::format("{} ({} over {}: {})", n, x, y, text::join(on_num, ", ")));
            } else if (!on_num.empty()) {   // (nor one whose pointer lies on the other's number)
                mapping.reset();
            }
            // (a union of the other's whose members are all pointers)
            auto ptr_union = [&](const std::string& w) {
                const std::string first = w.substr(0, w.find('.'));
                const auto yt = ytop.find(first);
                if (yt == ytop.end() || !yt->second->type.starts_with("union") ||
                    (yt->second->type.size() > 5 && (std::isalnum(static_cast<unsigned char>(yt->second->type[5])) || yt->second->type[5] == '_')))
                    return false;
                const StructLayout* sub = old.find(y + "." + first);
                return sub && !sub->members.empty() && std::all_of(sub->members.begin(), sub->members.end(), [](const Member& m) { return m.pointer; });
            };
            if (mapping) {   // (nor one with a number on the other's union of pointers: two kinds, not one)
                bool refused = false;
                for (const auto& [k_, m_] : *mapping) {
                    const Member* mx_member = lx->find(k_);
                    if (!mx_member || mx_member->pointer) continue;
                    const std::vector<std::string> ws = m_.pun ? std::vector<std::string>{m_.base} : m_.names;
                    for (const std::string& w : ws) refused = refused || ptr_union(w);
                }
                if (refused) mapping.reset();
            }
        }
        if (mapping && !mapping->empty()) found.insert_or_assign(x, ViewOf{y, *mapping, n});
    }
    if (views_cast_) weak_out = weak;
    return found;
}

}  // namespace

// ================================================================ offset_views

std::string offset_views(ViewContext& cx, std::string t, std::string_view fname) {
    const bool views_cast = rule_enabled("views_cast");   // (one thing cast to both views: the surest evidence)
    ViewCaches& caches = cx.caches;
    if (!caches.views) {
        std::set<std::string> ignored;
        caches.views = ViewBuilder(cx, false).build(ignored);
        if (views_cast) {   // (the views one thing cast to both adds, or changes: none that reads a byte of the other's word: a record then taken as bytes, MASM 4.00's opitem over its node)
            ViewTable plain = *caches.views;
            ViewTable cast_views = ViewBuilder(cx, true).build(caches.weak);
            for (const auto& [x, v] : cast_views) {
                const ViewOf* before = plain.find(x);
                const bool changed = !before || !(*before == v);
                const bool reads_a_byte = std::any_of(v.mapping.begin(), v.mapping.end(), [](const auto& kv) { return kv.second.pun; });
                if (changed && reads_a_byte) {
                    cx.view_puns_refused.push_back(std::format("{} over {}", x, v.other));
                    continue;
                }
                if (changed) caches.fresh.insert(x);
                caches.views->insert_or_assign(x, v);
            }
        }
    }
    std::vector<std::pair<std::string, ViewOf>> mine;
    for (const auto& [x, v] : *caches.views)
        if (v.file == fname || v.file.ends_with(".h")) mine.emplace_back(x, v);
    if (mine.empty()) return t;
    for (const auto& [x, view] : mine) {
        const std::string& y = view.other;
        if (caches.weak.contains(x)) {   // (a view whose functions have objects of it: struct targ t; tp = &t: the room of the other's, whose members the view reads)
            t = rx_tag(R"((?m)^([ \t]+(?:register\s+)?)@T@(\s+\w+\s*;))", x).replace_all(t, [&](const RegexMatch& m) { return std::string(m.group(1)) + y + std::string(m.group(2)); });
        }
        const std::string code = blank(t);
        std::set<std::string> vars;
        const Regex var_of_x = rx_tag(R"(@T@\s*\*\s*(\w+))", x);
        var_of_x.for_each_match(code, [&](const RegexMatch& m) {
            vars.emplace(m.group(1));
            return true;
        });
        std::vector<std::string> macros;
        rx_tag(R"((?m)^\s*#\s*define\s+(\w+)\s*\(\s*(\w+)\s*\)\s*\(\s*\(\s*@T@\s*\*\s*\)\s*\(?\s*\2\s*\)?\s*\)\s*$)", x).for_each_match(code, [&](const RegexMatch& m) {
            macros.emplace_back(m.group(1));
            return true;
        });
        std::vector<std::string> patterns;
        if (!vars.empty()) {
            std::vector<std::string> alt;
            for (const std::string& v : vars) alt.push_back(regex_quote(v));
            patterns.push_back(R"(\b(?<v>)" + text::join(alt, "|") + R"()\s*->\s*(?<m>\w+)\b)");
        }
        if (!macros.empty()) {   // (L(s)->num: the macro's cast, the other's member)
            std::vector<std::string> alt;
            for (const std::string& m : macros) alt.push_back(regex_quote(m));
            patterns.push_back(R"(\b(?:)" + text::join(alt, "|") + R"()\s*\(\s*(?<v>\w+)\s*\)\s*->\s*(?<m>\w+)\b)");
        }
        if (caches.fresh.contains(x))   // (the cast's too, for the views the casts gave: ((struct targ *)g_2622->sym)->kind)
            patterns.push_back(text::replace_all(R"(\(\s*\(\s*@T@\s*\*\s*\)\s*(?<v>[\w.>\[\]-]+?|\((?:[^()]|\([^()]*\))*\))\s*\)\s*->\s*(?<m>\w+)\b)", "@T@", regex_quote(x, true)));
        struct Use {
            std::size_t begin, end;
            std::string var, member;
            bool operator<(const Use& o) const { return std::tie(begin, end, var, member) < std::tie(o.begin, o.end, o.var, o.member); }
        };
        std::vector<Use> uses;
        for (const std::string& pattern : patterns)
            Regex(pattern).for_each_match(code, [&](const RegexMatch& m) {
                uses.push_back({m.start(), m.end(), std::string(*m.named("v")), std::string(*m.named("m"))});
                return true;
            });
        std::sort(uses.begin(), uses.end());
        struct Change {
            std::size_t begin, end;
            std::string text;
        };
        std::vector<Change> edits;
        for (const Use& u : uses) {
            const auto it = view.mapping.find(u.member);
            if (it == view.mapping.end()) continue;
            const ViewMember& ms = it->second;
            if (!ms.pun && ms.names.empty()) continue;
            const char before = vo_prev_char(code, u.begin), after = vo_next_char(code, u.end);
            const bool as_arg = (before == '(' || before == ',') && (after == ',' || after == ')');
            if (ms.pun) {   // (a part of a member of the other's, or a word over two)
                edits.push_back({u.begin, u.end, std::format("(*({} *)((char *)&(({} *){})->{} + {}))", ms.type, y, u.var, ms.base, ms.offset)});
                continue;
            }
            if (ms.names.size() == 1) {
                edits.push_back({u.begin, u.end, std::format("(({} *){})->{}", y, u.var, ms.names[0])});
            } else if (as_arg) {
                std::vector<std::string> each;
                for (const std::string& w : ms.names) each.push_back(std::format("(({} *){})->{}", y, u.var, w));
                edits.push_back({u.begin, u.end, text::join(each, ", ")});
            }
        }
        std::sort(edits.begin(), edits.end(), [](const Change& a, const Change& b) { return std::tie(a.begin, a.end, a.text) > std::tie(b.begin, b.end, b.text); });
        for (const Change& e : edits) t.replace(e.begin, e.end - e.begin, e.text);
        if (!edits.empty()) {
            std::vector<std::string> headers;   // (the other's members: its header)
            const Regex defines_y = rx_tag(R"(\b@T@\s*\{)", y);
            for (const auto& [h, ht] : cx.files)
                if (h.ends_with(".h") && defines_y.search(blank(ht))) headers.push_back(h);
            const std::string tag = y.substr(y.rfind(' ') + 1);
            std::optional<std::string> chosen;
            for (const std::string& h : headers)
                if (stem_of(h) == tag) {
                    chosen = h;
                    break;
                }
            if (!chosen && !headers.empty()) chosen = headers.front();
            if (chosen) cx.need_headers[std::string(fname)].insert(*chosen);
            note(cx.notes, "a file's own view of another structure by its old places: its members the other's (a long over two of them, given, the two)",
                 std::format("{} ({} as {})", fname, x, y));
        }
    }
    return t;
}

}  // namespace cb::port::pre_ansi
