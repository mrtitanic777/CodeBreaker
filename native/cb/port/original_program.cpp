// cb/port/original_program.cpp -- see original_program.hpp.

#include "cb/port/original_program.hpp"

#include <algorithm>
#include <deque>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_sizes.hpp"
#include "cb/port/struct_layout.hpp"

namespace cb::port::pre_ansi {

namespace {

// The statement on a line at the top level of a file that declares an array or variable: `st` is the stripped line;
// the declaration without its initializer or `far`/`near`/`huge` words, ready for `declarators`.
std::string without_initializer_and_distance(std::string_view st) {
    static const Regex distance(R"(\b(far|near|huge)\b)");
    std::string s(st);
    if (const std::size_t eq = s.find('='); eq != std::string::npos && s.ends_with(';')) s = s.substr(0, eq) + ";";
    s = distance.replace_all(s, "");
    while (s.ends_with(';')) s.pop_back();
    return s;
}

bool has_far(std::string_view s) {
    static const Regex far_word(R"(\b(far|huge)\b)");
    return far_word.test(s);
}

std::string base_type(std::string_view base, const Regex& storage) {
    return std::string(text::trim(collapse_space(storage.replace_all(base, ""))));
}

// `[n]` removed once (the first dimension): the element's declarator
std::string without_first_dimension(const std::string& declarator) {
    const std::size_t open = declarator.find('[');
    if (open == std::string::npos) return declarator;
    const std::size_t close = declarator.find(']', open);
    if (close == std::string::npos) return declarator;
    return declarator.substr(0, open) + declarator.substr(close + 1);
}

bool declares_numbered_array(std::string_view declarator) {
    static const Regex numbered(R"(\[\s*\d+\s*\])");
    return numbered.test(declarator);
}

bool has_paren_besides_function_pointer(std::string_view st) {
    return text::replace_all(st, "(*", "").find('(') != std::string::npos;
}

}  // namespace

OriginalProgram::OriginalProgram(Files originals, unsigned pointer_bytes, LayoutFn layout)
    : originals_(std::move(originals)), pointer_bytes_(pointer_bytes), layout_(layout ? std::move(layout) : LayoutFn(lay_out_structs)) {}

const StructLayouts& OriginalProgram::layouts() {
    if (!layouts_) {
        std::vector<std::string> texts;
        for (const auto& kv : originals_) texts.push_back(kv.second);
        layouts_ = layout_(texts, pointer_bytes_, 4);
    }
    return *layouts_;
}

OriginalProgram::LayoutInput OriginalProgram::layout_input(const std::string& file) {
    static const Regex tag_definition(R"(\b((?:struct|union)\s+\w+)\s*\{)");
    static const Regex include_directive(R"re(#\s*include\s*"([^"]+)")re");
    if (!twice_tags_) {
        std::map<std::string, std::string> first_file;
        std::set<std::string> twice;
        for (const auto& kv : originals_) {
            tag_definition.for_each_match(base::ctext::blank(kv.second), [&](const RegexMatch& m) {
                const std::string tag = collapse_space(m.group(1));
                if (first_file.try_emplace(tag, kv.first).first->second != kv.first) twice.insert(tag);
                return true;
            });
        }
        twice_tags_ = std::move(twice);
    }
    LayoutInput in;
    if (twice_tags_->empty()) {
        for (const auto& kv : originals_) in.texts.push_back(kv.second);
        return in;
    }
    // its headers, depth first, in the order included
    std::deque<std::string> todo = {file};
    while (!todo.empty()) {
        const std::string n = todo.front();
        todo.pop_front();
        if (std::find(in.order.begin(), in.order.end(), n) != in.order.end() || !originals_.contains(n)) continue;
        in.order.push_back(n);
        std::vector<std::string> includes;
        include_directive.for_each_match(originals_.at(n), [&](const RegexMatch& m) {
            includes.emplace_back(m.group(1));
            return true;
        });
        todo.insert(todo.begin(), includes.begin(), includes.end());
    }
    for (auto n = in.order.rbegin(); n != in.order.rend(); ++n) in.texts.push_back(originals_.at(*n));
    for (const auto& kv : originals_)
        if (std::find(in.order.begin(), in.order.end(), kv.first) == in.order.end()) in.texts.push_back(kv.second);
    return in;
}

const StructLayouts& OriginalProgram::layouts_for(const std::string& file) {
    LayoutInput in = layout_input(file);
    if (in.order.empty()) return layouts();
    auto it = layouts_by_include_order_.find(in.order);
    if (it == layouts_by_include_order_.end()) it = layouts_by_include_order_.emplace(in.order, layout_(in.texts, pointer_bytes_, 4)).first;
    return it->second;
}

const OrderedMap<std::string, std::uint64_t>& OriginalProgram::elem_sizes() {
    static const Regex storage(R"(\b(extern|static|register|const)\b)");
    if (!elem_sizes_) {
        const StructLayouts& old = layouts();
        OrderedMap<std::string, std::uint64_t> out;
        for (const auto& file : originals_) {
            const std::vector<std::string> code = split_lines(base::ctext::blank(file.second));
            const std::vector<int> ds = brace_depths(code);
            for (std::size_t i = 0; i < code.size(); ++i) {
                const std::string_view st = text::trim(code[i]);
                if (ds[i] != 0 || st.find('[') == std::string_view::npos || !st.ends_with(';') || has_paren_besides_function_pointer(st)) continue;
                const bool is_far = has_far(st);
                const auto r = declarators(without_initializer_and_distance(st));
                if (!r) continue;
                const std::string base = base_type(r->base, storage);
                for (const Declarator& dn : r->names) {
                    std::string element = without_first_dimension(dn.text);
                    element = text::replace_all(element, dn.name, "");
                    if (element.find('*') != std::string::npos) {
                        out.try_emplace(dn.name, is_far ? 4 : 2);
                    } else if ((base.starts_with("struct ") || base.starts_with("union ")) && old.contains(base)) {
                        out.try_emplace(dn.name, old.at(base).size);
                    } else if (const auto s = old_scalar_size(base)) {
                        out.try_emplace(dn.name, *s);
                    }
                }
            }
        }
        elem_sizes_ = std::move(out);
    }
    return *elem_sizes_;
}

OrderedMap<std::string, std::uint64_t> OriginalProgram::var_sizes(const OrderedMap<std::string, std::uint64_t>& asm_sizes) {
    static const Regex storage(R"(\b(extern|static|register|const)\b)");
    static const Regex extern_static(R"(\b(extern|static)\b)");
    if (!var_sizes_) {
        const StructLayouts& old = layouts();
        OrderedMap<std::string, std::uint64_t> out;
        for (const auto& file : originals_) {
            const std::vector<std::string> code = split_lines(base::ctext::blank(file.second));
            const std::vector<int> ds = brace_depths(code);
            for (std::size_t i = 0; i < code.size(); ++i) {
                const std::string_view st = text::trim(code[i]);
                if (ds[i] != 0 || st.find('[') == std::string_view::npos || st.find('(') != std::string_view::npos || !st.ends_with(';')) continue;
                const bool is_far = has_far(st);
                const auto r = declarators(without_initializer_and_distance(st));
                if (!r) continue;
                for (const Declarator& dn : r->names) {
                    if (!declares_numbered_array(dn.text)) continue;
                    auto size = size_of(text::trim(extern_static.replace_all(r->base, "")), dn.text, dn.name, is_far ? 4 : 2, old);
                    if (!size) {
                        const auto unit = old_scalar_size(base_type(r->base, storage));
                        const auto count = array_count(dn.text);
                        if (unit && count && dn.text.find('*') == std::string::npos) size = *unit * *count;
                    }
                    if (size && *size != 0) out.try_emplace(dn.name, *size);
                }
            }
        }
        var_sizes_ = std::move(out);
    }
    OrderedMap<std::string, std::uint64_t> merged = *var_sizes_;
    for (const auto& kv : asm_sizes) merged.insert_or_assign(kv.first, kv.second);   // (and what the assembler's data placed)
    return merged;
}

const Allocators& OriginalProgram::allocators(const Files& ported) {
    static const Regex pointer_decl(R"(\b((?:struct|union)\s+\w+)\s*\*\s*(\w+)\s*[;,)=])");
    static const Regex allocating_call(
        R"((?:\(\s*((?:struct|union)\s+\w+)\s*\*\s*\)|\b(\w+)\s*=)\s*(?:\w+\s*\(\s*)?([A-Za-z_]\w*)\s*\(([^()]*)\))");
    static const Regex size_literal(R"(\s*(?:0[xX][0-9a-fA-F]+|[1-9]\d*)\s*)");
    static const Regex any_call(R"(\b(\w+)\s*\()");
    if (allocators_) return *allocators_;
    const StructLayouts& old = layouts();
    Allocators out;   // name: the places of its arguments the size was at
    for (const auto& file : originals_) {
        const std::string code = base::ctext::blank(file.second);
        std::map<std::string, std::string> ptr_of;   // (a pointer variable's structure: the last declaration says)
        pointer_decl.for_each_match(code, [&](const RegexMatch& m) {
            ptr_of[std::string(m.group(2))] = collapse_space(m.group(1));
            return true;
        });
        allocating_call.for_each_match(code, [&](const RegexMatch& m) {
            std::string st;
            if (m.has(1)) {
                st = collapse_space(m.group(1));
            } else if (const auto p = ptr_of.find(std::string(m.group(2))); p != ptr_of.end()) {
                st = p->second;
            }
            if (st.empty() || !old.contains(st) || is_keyword(m.group(3))) return true;
            const std::uint64_t whole = old.at(st).size;
            std::size_t k = 0;
            for (std::string_view arg : text::split(m.group(4), ',')) {
                if (size_literal.full_match(arg)) {
                    const auto v = text::parse_int<std::uint64_t>(text::trim(arg), 0);
                    if (v && *v == whole) out[std::string(m.group(3))].insert(k);
                }
                ++k;
            }
            return true;
        });
    }
    if (!rule_enabled("alloc_places")) {   // (the names alone: no place of the size known)
        for (auto& kv : out) kv.second.clear();
        allocators_ = std::move(out);
        return *allocators_;
    }
    // and what one gives on to another: f03f02(n, v, pool) { f03e86(pool, n); } -- f03e86's size its second
    std::map<std::string, Fn> defs;
    std::map<std::string, std::string> bodies;
    for (const auto& file : ported) {
        if (!file.first.ends_with(".c")) continue;
        const std::vector<std::string> lines = split_lines(file.second);
        const std::vector<std::string> code = split_lines(base::ctext::blank(file.second));
        for (Definition& d : definitions(lines, code)) {
            std::vector<std::string> piece(code.begin() + static_cast<std::ptrdiff_t>(d.open),
                                           code.begin() + static_cast<std::ptrdiff_t>(std::min(d.last + 1, code.size())));
            bodies.try_emplace(d.fn.name, join_lines(piece));
            defs.try_emplace(d.fn.name, std::move(d.fn));
        }
    }
    for (int round = 0; round < 3; ++round) {
        const std::vector<std::pair<std::string, std::set<std::size_t>>> snapshot(out.begin(), out.end());
        for (const auto& [a, places] : snapshot) {
            const auto def = defs.find(a);
            if (places.size() != 1 || def == defs.end() || *places.begin() >= def->second.params.size()) continue;
            const std::string& pa = def->second.params[*places.begin()].name;
            const std::string body = bodies.contains(a) ? bodies.at(a) : std::string();
            any_call.for_each_match(body, [&](const RegexMatch& m) {
                const std::string callee(m.group(1));
                if (is_keyword(callee) || callee == a || !defs.contains(callee)) return true;
                const std::string_view window = std::string_view(body).substr(m.start(), 400);
                const auto spans = call_args(window, callee).value_or(std::vector<Span>{});
                std::vector<std::size_t> js;
                for (std::size_t j = 0; j < spans.size(); ++j)
                    if (text::trim(window.substr(spans[j].begin, spans[j].size())) == pa) js.push_back(j);
                if (js.size() == 1) out[callee] = {js[0]};
                return true;
            });
        }
    }
    allocators_ = std::move(out);
    return *allocators_;
}

ExternDecls OriginalProgram::extern_decls(const std::set<std::string>& names) const {
    static const Regex extern_word(R"(^extern\s+)");
    ExternDecls decl;
    for (const auto& file : originals_) {
        if (!(file.first.ends_with(".c") || file.first.ends_with(".h"))) continue;
        const std::vector<std::string> code = split_lines(base::ctext::blank(file.second));
        const std::vector<int> ds = brace_depths(code);
        for (std::size_t i = 0; i < code.size(); ++i) {
            std::string_view st = text::trim(code[i]);
            if (ds[i] != 0 || !st.starts_with("extern") || !st.ends_with(';') || has_paren_besides_function_pointer(st)) continue;
            std::string rest = extern_word.replace_first(st, "");
            while (rest.ends_with(';')) rest.pop_back();
            const auto r = declarators(rest);
            if (!r) continue;
            for (const Declarator& dn : r->names)
                if (names.contains(dn.name) && dn.text.find('[') != std::string::npos)
                    decl.try_emplace(dn.name, std::make_pair(std::string(text::trim(collapse_space(r->base))), dn.text));
        }
    }
    return decl;
}

}  // namespace cb::port::pre_ansi
