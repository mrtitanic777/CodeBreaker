// cb/port/struct_layout.cpp -- see struct_layout.hpp.

#include "cb/port/struct_layout.hpp"

#include <algorithm>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/c_source.hpp"
#include "cb/port/old_sizes.hpp"

namespace cb::port::pre_ansi {

namespace {

struct Pending {
    std::string body;
    bool is_union;
};

void set_member(StructLayout& lay, const std::string& name, std::uint32_t offset, std::uint32_t size,
                const std::string& declarator, bool pointer, const std::string& type) {
    for (Member& m : lay.members)
        if (m.name == name) {
            m.offset = offset;
            m.size = size;
            m.declarator = declarator;
            m.pointer = pointer;
            m.type = type;
            return;
        }
    lay.members.push_back({name, offset, size, declarator, pointer, type});
}

// The layout of a structure's (or union's) members.
StructLayout lay_out_body(std::string_view body, bool is_union, unsigned ptr, unsigned far_ptr, StructLayouts& out, const std::string& path) {
    static const Regex nested(R"((?s)\s*(struct|union)\s*\w*\s*\{(.*)\}\s*(.*)$)");
    static const Regex array_tail(R"(\s*\[.*)");
    static const Regex distance(R"(\b(far|near|huge)\b)");
    static const Regex far_word(R"(\bfar\b)");

    // the members: ; at the body's own level
    std::vector<std::string> members;
    {
        int depth = 0;
        std::string cur;
        for (const char c : body) {
            depth += (c == '{') - (c == '}');
            if (c == ';' && depth == 0) {
                members.push_back(std::move(cur));
                cur.clear();
            } else {
                cur.push_back(c);
            }
        }
    }
    StructLayout lay;
    std::uint32_t off = 0;
    lay.is_union = is_union;
    auto place = [&](const std::string& name, std::uint32_t size, const std::string& declarator, bool pointer,
                     const std::string& type) {
        if (size > 1 && off % 2) ++off;
        set_member(lay, name, is_union ? 0 : off, size, declarator, pointer, type);
        off = is_union ? std::max(off, size) : off + size;
    };
    for (const std::string& s : members) {
        if (text::trim(s).empty()) continue;
        if (const auto nest = nested.match(s)) {   // (struct { ... } name: its own layout, one member)
            std::vector<std::string> names;
            for (std::string_view x : text::split(nest->group(3), ',')) {
                x = text::trim(x);
                if (!x.empty()) names.emplace_back(x);
            }
            const bool inner_union = nest->group(1) == "union";
            const StructLayout inner = lay_out_body(nest->group(2), inner_union, ptr, far_ptr, out,
                                                    path + "." + (names.empty() ? "?" : names[0]));
            for (std::string& nm : names) nm = array_tail.replace_all(nm, "");
            // (its members, for what is inside it; the latest round's layout, once the sizes have settled)
            for (const std::string& nm : names) out.insert_or_assign(path + "." + nm, inner);
            const bool all_pointers = std::all_of(inner.members.begin(), inner.members.end(), [](const Member& m) { return m.pointer; });
            for (const std::string& nm : names) place(nm, inner.size, nm, all_pointers, std::string(nest->group(1)));
            continue;
        }
        const bool is_far = far_word.test(s);
        const auto r = declarators(distance.replace_all(s, ""));
        if (!r) continue;
        for (const Declarator& dn : r->names) {
            const auto sz = size_of(r->base, dn.text, dn.name, is_far ? far_ptr : ptr, out);
            std::string without_name = dn.text;   // (a pointer: a '*' left once the member's own name is taken out)
            if (const std::size_t at = without_name.find(dn.name); at != std::string::npos) without_name.erase(at, dn.name.size());
            place(dn.name, static_cast<std::uint32_t>(sz.value_or(2)), dn.text, without_name.find('*') != std::string::npos,
                  std::string(text::trim(r->base)));
        }
    }
    lay.size = off + (off > 1 ? off % 2 : 0);
    return lay;
}

}  // namespace

StructLayouts lay_out_structs(std::span<const std::string> texts, unsigned ptr, unsigned far_ptr) {
    static const Regex tag_head(R"(\b(struct|union)\s+(\w+)\s*\{)");
    StructLayouts out;
    OrderedMap<std::string, Pending> defs;   // each tag's first definition: its body
    for (const std::string& t : texts) {
        const std::string code = base::ctext::blank(t);
        tag_head.for_each_match(code, [&](const RegexMatch& m) {
            const std::size_t close = match_brace(code, m.end() - 1);
            defs.try_emplace(std::string(m.group(1)) + " " + std::string(m.group(2)),
                             Pending{code.substr(m.end(), close - m.end()), m.group(1) == "union"});
            return true;
        });
    }
    for (int round = 0; round < 6; ++round) {   // (again until the sizes settle: a structure of one defined later is known the next time)
        const StructLayouts before = out;
        for (const auto& [name, def] : defs) out.insert_or_assign(name, lay_out_body(def.body, def.is_union, ptr, far_ptr, out, name));
        if (out == before) break;
    }
    return out;
}

}  // namespace cb::port::pre_ansi
