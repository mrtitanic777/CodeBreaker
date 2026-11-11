// cb/port/union_views.cpp -- see union_views.hpp.

#include "cb/port/union_views.hpp"

#include <algorithm>
#include <set>
#include <tuple>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/c_source.hpp"
#include "cb/port/old_sizes.hpp"

namespace cb::port::pre_ansi {

namespace {

struct Edit {
    std::size_t begin, end;
    std::string text;
    auto operator<=>(const Edit&) const = default;
};

// Applies the edits from the last place to the first (equal places: the later text first, so the earlier ends up
// first in the result).
std::string apply_edits(std::string text, std::vector<Edit> edits) {
    std::sort(edits.begin(), edits.end(), [](const Edit& a, const Edit& b) { return a > b; });
    for (const Edit& e : edits) text = text.substr(0, e.begin) + e.text + text.substr(e.end);
    return text;
}

// The regex matches inside code[from, to) -- as if that piece of text stood alone (a word boundary at its ends).
template <class Fn>
void each_in(const Regex& rx, std::string_view code, std::size_t from, std::size_t to, Fn&& fn) {
    rx.for_each_match(code.substr(from, to - from), [&](const RegexMatch& m) {
        fn(m);
        return true;
    });
}

}  // namespace

std::string split_view_lists(std::string_view text) {
    static const Regex union_head(R"(\bunion\s*\w*\s*\{)");
    static const Regex view_struct(R"(\bstruct\s*\{([^{}]*)\})");
    static const Regex statement(R"([^;]*;)");
    const std::string code = base::ctext::blank(text);
    std::vector<Edit> edits;
    each_in(union_head, code, 0, code.size(), [&](const RegexMatch& um) {
        const std::size_t close = match_brace(code, um.end() - 1);
        each_in(view_struct, code, um.end(), close, [&](const RegexMatch& vm) {
            const std::size_t a0 = um.end() + vm.start(1);
            each_in(statement, code, a0, um.end() + vm.end(1), [&](const RegexMatch& st) {
                const auto r = declarators(text::trim(st.group(0)));
                if (!r || r->names.size() < 2 || st.group(0).find('(') != std::string_view::npos) return;
                const std::size_t a = a0 + st.start(), b = a0 + st.end();
                const std::string_view piece = text.substr(a, b - a);
                const std::string_view lead = indent_of(piece);
                const std::string ind = lead.find('\n') != std::string_view::npos
                                            ? std::string(lead.substr(lead.rfind('\n') + 1))
                                            : std::string(8, ' ');
                std::string joined;
                for (std::size_t i = 0; i < r->names.size(); ++i) {
                    if (i) joined += "\n" + ind;
                    joined += std::string(text::trim(r->base)) + " " + r->names[i].text + ";";
                }
                edits.push_back({a, b, std::string(lead) + joined});
            });
        });
    });
    return apply_edits(std::string(text), std::move(edits));
}

namespace {

struct ViewMember {
    std::size_t begin = 0, end = 0;   // the declaration's text (npos when the member is a plain member of the union)
    std::uint64_t off = 0, old_size = 0, new_size = 0;
    std::string name;
    bool bytes = false;               // a byte array over other views' members: it takes whatever room they leave
};

struct View {
    bool plain = false;               // a plain member of the union: a one-member view at its start
    std::vector<ViewMember> members;
};

// Whether the declarator makes a pointer: a `*` anywhere besides in its name's place (`*p`, `**v[64]`, `(*f)()`).
bool declarator_is_pointer(const std::string& declarator, const std::string& name) {
    std::string without = declarator;
    if (const std::size_t at = without.find(name); at != std::string::npos) without.erase(at, name.size());
    return without.find('*') != std::string::npos;
}

std::size_t start_of_line(const std::string& text, std::size_t pos) {
    if (pos == 0) return 0;
    const std::size_t nl = text.rfind('\n', pos - 1);
    return nl == std::string::npos ? 0 : nl + 1;
}

bool has_far(std::string_view s) {
    static const Regex far_word(R"(\b(far|huge)\b)");
    return far_word.test(s);
}

}  // namespace

std::string union_views(std::string_view original, Notes& notes, std::string_view fname, const SizeChanges& sizes) {
    static const Regex union_head(R"(\bunion\s*\w*\s*\{)");
    static const Regex view_head(R"(\bstruct\s*\{)");
    static const Regex statement(R"([^;{}]*;)");
    static const Regex near_far(R"(\b(far|near|huge)\b)");
    static const Regex qualifiers(R"(\b(register|const|volatile)\b)");
    static const Regex nested_struct(R"(\bstruct\s*\w*\s*\{[^{}]*\}[^;]*;)");
    static const Regex char_type(R"((?:unsigned\s+|signed\s+)?char)");
    static const Regex count_bracket(R"(\[\s*\d+\s*\])");

    const std::string text = split_view_lists(original);
    const std::string code = base::ctext::blank(text);
    std::vector<Edit> edits;
    each_in(union_head, code, 0, code.size(), [&](const RegexMatch& um) {
        const std::size_t close = match_brace(code, um.end() - 1);
        const std::size_t body_at = um.end();
        std::vector<View> views;
        bool views_ok = true;
        each_in(view_head, code, body_at, close, [&](const RegexMatch& vm) {
            if (!views_ok) return;
            const std::size_t vs = body_at + vm.end();
            int d2 = 1;
            std::size_t e = vs;
            while (e < close && d2 != 0) {
                d2 += (code[e] == '{') - (code[e] == '}');
                ++e;
            }
            View view;
            std::uint64_t off = 0;
            bool members_ok = true;
            each_in(statement, code, vs, e - 1, [&](const RegexMatch& stmt) {
                if (!members_ok) return;
                const std::string_view s = stmt.group(0);
                if (text::trim(text::trim(s, ";")).empty()) return;
                const bool isfar = has_far(s);
                const auto r = declarators(text::trim(near_far.replace_all(s, "")));
                if (!r || r->names.size() != 1) {
                    members_ok = false;
                    return;
                }
                const Declarator& dn = r->names[0];
                auto old_sz = size_of(r->base, dn.text, dn.name, isfar ? 4 : 2, StructLayouts{});
                if (!old_sz) {
                    const auto unit = old_scalar_size(collapse_space(text::trim(qualifiers.replace_all(r->base, ""))));
                    const auto cnt = array_count(dn.text);
                    if (unit && cnt) old_sz = *unit * *cnt;
                }
                if (!old_sz) {
                    members_ok = false;
                    return;
                }
                const bool ptr = declarator_is_pointer(dn.text, dn.name);
                const std::uint64_t cnt = array_count(dn.text).value_or(1);
                const std::uint64_t new_sz = ptr ? 8 * cnt : *old_sz;
                if (*old_sz > 1 && off % 2) ++off;
                const bool bytes = char_type.full_match(text::trim(r->base)) && dn.text.find('[') != std::string::npos && !ptr;
                view.members.push_back({vs + stmt.start(), vs + stmt.end(), off, *old_sz, new_sz, dn.name, bytes});
                off += *old_sz;
            });
            if (!members_ok) {
                views_ok = false;
                return;
            }
            views.push_back(std::move(view));
            // (the end of the view's braces is kept with it: the edit offsets are absolute)
        });
        if (!views_ok) views.clear();
        if (!views.empty()) {   // its plain members: one-member views at its start
            std::string flat(code.substr(body_at, close - body_at));
            std::string blanked;
            std::size_t last = 0;
            nested_struct.for_each_match(flat, [&](const RegexMatch& x) {
                blanked.append(flat, last, x.start() - last);
                blanked.append(x.end() - x.start(), ' ');
                last = x.end();
                return true;
            });
            blanked.append(flat, last, std::string::npos);
            each_in(statement, blanked, 0, blanked.size(), [&](const RegexMatch& st) {
                const auto r = declarators(text::trim(st.group(0)));
                if (!r || r->names.size() != 1 || text::trim(text::trim(text::trim(st.group(0)), ";")).empty()) return;
                const Declarator& dn = r->names[0];
                const std::string base(collapse_space(text::trim(r->base)));
                const bool ptr = declarator_is_pointer(dn.text, dn.name);
                std::uint64_t old_sz = 0, new_sz = 0;
                if (ptr) {
                    old_sz = has_far(st.group(0)) ? 4 : 2;
                    new_sz = 8;
                } else if (const auto it = sizes.find(base); it != sizes.end()) {
                    old_sz = it->second.first;
                    new_sz = it->second.second;
                } else {
                    old_sz = old_scalar_size(base).value_or(0);
                    new_sz = old_sz;
                }
                if (old_sz == 0 || dn.text.find('[') != std::string::npos) return;
                View v;
                v.plain = true;
                v.members.push_back({0, 0, 0, old_sz, new_sz, dn.name, false});
                views.push_back(std::move(v));
            });
        }
        if (views.size() < 2) return;
        const bool any_size_change = std::any_of(views.begin(), views.end(), [](const View& v) {
            return std::any_of(v.members.begin(), v.members.end(), [](const ViewMember& m) { return m.new_size != m.old_size; });
        });
        if (!any_size_change) return;
        // each old place: the least new one all views allow
        std::set<std::uint64_t> bounds = {0};
        for (const View& v : views)
            for (const ViewMember& m : v.members) {
                bounds.insert(m.off);
                bounds.insert(m.off + m.old_size);
            }
        std::map<std::uint64_t, std::uint64_t> placed;
        std::optional<std::uint64_t> prev;
        for (const std::uint64_t b : bounds) {
            std::uint64_t n = prev ? placed[*prev] + (b - *prev) : 0;
            for (const View& v : views)
                for (const ViewMember& m : v.members)
                    if (m.old_size != 0 && m.off + m.old_size == b && !m.bytes) n = std::max(n, placed[m.off] + m.new_size);
            placed[b] = n + (b % 2 == 0 ? n % 2 : 0);
            prev = b;
        }
        bool changed = false;
        for (const View& v : views) {
            if (v.plain) continue;   // (a plain member: at the start, as it is)
            std::uint64_t cur = 0;
            int pad = 0;
            for (const ViewMember& m : v.members) {
                const std::uint64_t at = placed[m.off];
                const std::string_view decl_text = std::string_view(code).substr(m.begin, m.end - m.begin);
                const std::size_t ds = m.begin + (decl_text.size() - text::ltrim(decl_text).size());   // (the declaration itself)
                const std::size_t line_start = start_of_line(text, ds);
                const std::string_view before = std::string_view(text).substr(line_start, ds - line_start);
                const std::string indent = text::trim(before).empty() ? std::string(before) : std::string(12, ' ');
                if (at > cur) {   // a line of its own before it: the room
                    edits.push_back({line_start, line_start,
                                     indent + "char _old" + std::to_string(pad) + "[" + std::to_string(at - cur) +
                                         "];\t\t/* (where the other views have members) */\n"});
                    ++pad;
                    changed = true;
                }
                if (m.bytes) {   // a byte array over other views' members: as long as the room is
                    const std::uint64_t size = placed[m.off + m.old_size] - at;
                    const std::string stmt = text.substr(ds, m.end - ds);
                    const std::string new_stmt = count_bracket.replace_first(stmt, "[" + std::to_string(size) + "]");
                    if (new_stmt != stmt) {
                        edits.push_back({ds, m.end, new_stmt});
                        changed = true;
                    }
                    cur = at + size;
                } else {
                    cur = at + m.new_size;
                }
            }
        }
        if (changed)
            note(notes, "a union whose views meet at the same old places: laid out again, each old place one new place in all of them",
                 std::string(fname));
    });
    return apply_edits(text, std::move(edits));
}

}  // namespace cb::port::pre_ansi
