// cb/port/headers.cpp -- see headers.hpp.

#include "cb/port/headers.hpp"

#include <algorithm>
#include <cctype>
#include <unordered_map>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"

namespace cb::port::pre_ansi {

namespace {

// What the headers of the program say of each other: which defines a tag, which includes which. Each answer is
// worked out once for the run of `make_headers`.
class HeaderIndex {
public:
    explicit HeaderIndex(const Files& files) : files_(files) {}

    const std::string& blanked(const std::string& file) {
        auto it = blanked_.find(file);
        if (it == blanked_.end()) it = blanked_.emplace(file, base::ctext::blank(files_.at(file))).first;
        return it->second;
    }

    // The first `.h` file of the program that defines the tag (`struct s {`).
    std::optional<std::string> first_defining(std::string_view tag) {
        const Regex rx(R"(\b)" + regex_quote(tag, true) + R"(\s*\{)");
        for (const auto& kv : files_)
            if (kv.first.ends_with(".h") && rx.test(blanked(kv.first))) return kv.first;
        return std::nullopt;
    }

    // The `.h` file that defines a tag -- of two that do, the one of its name.
    std::optional<std::string> defining(const std::string& tag) {
        if (const auto it = defining_.find(tag); it != defining_.end()) return it->second;
        const Regex rx(R"(\b)" + regex_quote(tag, true) + R"(\s*\{)");
        std::vector<std::string> hs;
        for (const auto& kv : files_)
            if (kv.first.ends_with(".h") && rx.test(blanked(kv.first))) hs.push_back(kv.first);
        std::optional<std::string> result;
        if (!hs.empty()) result = hs.front();
        const std::string wanted = tag.substr(tag.find_last_of(' ') == std::string::npos ? 0 : tag.find_last_of(' ') + 1);
        for (const std::string& h : hs)
            if (stem(h) == wanted) {
                result = h;
                break;
            }
        defining_.emplace(tag, result);
        return result;
    }

    // The tags a header defines, itself and through its includes.
    const std::set<std::string>& tags_of(const std::string& h) {
        auto it = tags_.find(h);
        if (it == tags_.end()) {
            std::set<std::string> seen;
            it = tags_.emplace(h, collect(h, seen)).first;
        }
        return it->second;
    }

    bool same_def(const std::string& tag, const std::string& h1, const std::string& h2) {
        const Regex rx(R"(\b)" + regex_quote(tag, true) + R"(\s*\{)");
        return where(rx, h1) == where(rx, h2);
    }

private:
    static std::string stem(const std::string& h) {
        const std::size_t slash = h.find_last_of("/\\");
        const std::size_t dot = h.find_last_of('.');
        return dot == std::string::npos || (slash != std::string::npos && dot < slash) ? h : h.substr(0, dot);
    }

    std::set<std::string> collect(const std::string& h, std::set<std::string>& seen) {
        static const Regex tag_definition(R"(\b((?:struct|union)\s+\w+)\s*\{)");
        static const Regex include_directive(R"re(#\s*include\s*"([^"]+)")re");
        if (seen.contains(h) || !files_.contains(h)) return {};
        seen.insert(h);
        std::set<std::string> out;
        tag_definition.for_each_match(blanked(h), [&](const RegexMatch& m) {
            out.insert(collapse_space(m.group(1)));
            return true;
        });
        std::vector<std::string> includes;
        include_directive.for_each_match(files_.at(h), [&](const RegexMatch& m) {
            includes.emplace_back(m.group(1));
            return true;
        });
        for (const std::string& i : includes) {
            const std::set<std::string> more = collect(i, seen);
            out.insert(more.begin(), more.end());
        }
        return out;
    }

    // The header that defines the tag: `h` itself, or the first of its includes (depth first) that does.
    std::optional<std::string> where(const Regex& defines_tag, const std::string& h) {
        std::set<std::string> seen;
        return where_in(defines_tag, h, seen);
    }
    std::optional<std::string> where_in(const Regex& defines_tag, const std::string& h, std::set<std::string>& seen) {
        static const Regex include_directive(R"re(#\s*include\s*"([^"]+)")re");
        if (seen.contains(h) || !files_.contains(h)) return std::nullopt;
        seen.insert(h);
        if (defines_tag.test(blanked(h))) return h;
        std::vector<std::string> includes;
        include_directive.for_each_match(files_.at(h), [&](const RegexMatch& m) {
            includes.emplace_back(m.group(1));
            return true;
        });
        for (const std::string& i : includes)
            if (auto w = where_in(defines_tag, i, seen)) return w;
        return std::nullopt;
    }

    const Files& files_;
    std::unordered_map<std::string, std::string> blanked_;
    std::unordered_map<std::string, std::optional<std::string>> defining_;
    std::unordered_map<std::string, std::set<std::string>> tags_;
};

std::string include_line(std::string_view header) { return "#include \"" + std::string(header) + "\""; }

std::string first_identifier(std::string_view s) {
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (std::isalpha(static_cast<unsigned char>(s[i])) || s[i] == '_') {
            std::size_t j = i + 1;
            while (j < s.size() && is_word_char(s[j])) ++j;
            return std::string(s.substr(i, j - i));
        }
    }
    return {};
}

}  // namespace

bool same_definition(const Files& files, std::string_view tag, std::string_view h1, std::string_view h2) {
    HeaderIndex index(files);
    return index.same_def(std::string(tag), std::string(h1), std::string(h2));
}

Headers make_headers(const HeaderInputs& in, Aliases& aliases, BssPlan& bss, const FinishRules& rules, const WidthsFn& widths) {
    static const Regex tag_word(R"(\b(?:struct|union)\s+\w+)");
    static const Regex array_pointer_param(R"(\b((?:struct|union)\s+\w+)\s*\(\s*\*\s*\w+\s*\)\s*\[)");
    static const Regex register_word(R"(\bregister\s+)");
    static const Regex member_by_value(R"(\b((?:struct|union)\s+\w+)\s+\w+\s*(?:\[[^\]]*\])?\s*;)");
    static const Regex bss_struct_var(R"(\b((?:struct|union)\s+\w+)\s+\w+\s*[\[;])");
    HeaderIndex index(in.files);
    Headers result;

    // ---- proto.h
    std::set<std::string> tag_set;
    for (const auto& fkv : in.fns) {
        const Fn& f = fkv.second;
        for (const Param& p : f.params)
            tag_word.for_each_match(p.type, [&](const RegexMatch& m) {
                tag_set.emplace(m.whole());
                return true;
            });
        tag_word.for_each_match(f.ret, [&](const RegexMatch& m) {
            tag_set.emplace(m.whole());
            return true;
        });
    }
    std::set<std::string> whole;   // (T (*a)[3]: an array of T -- T must be complete)
    for (const auto& fkv : in.fns)
        for (const Param& p : fkv.second.params) {
            const std::string pointer = array_param(p.type, p.name);
            array_pointer_param.for_each_match(pointer, [&](const RegexMatch& m) {
                whole.insert(collapse_space(m.group(1)));
                return true;
            });
        }
    std::vector<std::string> incs;
    for (const std::string& t : whole) {
        const std::optional<std::string> h = index.first_defining(t);
        if (h && std::find(incs.begin(), incs.end(), *h) == incs.end()) incs.push_back(*h);
    }
    std::vector<std::string> proto = {"/* proto.h -- every function of the program, declared (made by CodeBreaker's port) */",
                                      "#ifndef PROTO_H", "#define PROTO_H"};
    for (const std::string& h : incs) proto.push_back(include_line(h));
    for (const std::string& t : tag_set) proto.push_back(t + ";");
    proto.push_back("");
    std::vector<const Fn*> by_place;
    for (const auto& fkv : in.fns) by_place.push_back(&fkv.second);
    std::stable_sort(by_place.begin(), by_place.end(), [](const Fn* a, const Fn* b) {
        const std::string fa = a->file.value_or(""), fb = b->file.value_or("");
        return std::tie(fa, a->line) < std::tie(fb, b->line);
    });
    for (const Fn* f : by_place)
        if (!f->is_static) proto.push_back(register_word.replace_all(f->proto(), "") + ";");
    if (!in.asm_protos.empty()) {   // (the program's own assembly routines, called old-style: their arguments pushed as they were)
        proto.push_back("");
        proto.push_back("/* the program's routines in assembly language (their *64.asm) */");
        for (const auto& kv : in.asm_protos) proto.push_back(kv.second);
    }
    proto.push_back("");
    proto.push_back("#endif");
    const std::string proto_text = widths(text::join(proto, "\n"), "proto.h", in.int16);

    // ---- port.h
    std::vector<std::string> port = {"/* port.h -- what every file of the port includes first (made by CodeBreaker's port) */",
                                     "#ifndef PORT_H", "#define PORT_H", "#include <stdio.h>", "#include <stdlib.h>",
                                     "#include <string.h>", "#include <stdint.h>", "#include <ctype.h>", "#include <signal.h>",
                                     "#include <errno.h>", "#include <time.h>", include_line(in.profile.platform_header)};
    if (const auto align = in.profile.struct_align; align && *align != 0) {   // (the program's structures laid out as its compiler did)
        port.push_back("/* the program's structures laid out as its compiler did: members on " + std::to_string(*align) +
                       "-byte boundaries at most */");
        port.push_back("#pragma pack(push, " + std::to_string(*align) + ")");
    }
    for (const std::string& h : in.view_headers) port.push_back(include_line(h));   // (a structure some names are members of: known here)
    if (!aliases.empty()) {   // (variables the old linker gave one place: the names of its elements are the elements)
        std::map<std::string, std::string> owners;   // the variable that owns the places -> its declaration
        std::vector<std::pair<std::string, Alias>> sorted_by_name(aliases.begin(), aliases.end());
        std::sort(sorted_by_name.begin(), sorted_by_name.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        for (const auto& [name, alias] : sorted_by_name) {
            const std::string owner = first_identifier(alias.where);
            if (!owner.empty()) owners.emplace(owner, alias.declaration);
        }
        port.push_back("");
        port.push_back("/* the names the old linker gave the places of an array's elements */");
        for (const auto& [owner, declaration] : owners)
            if (!declaration.empty()) port.push_back(widths("extern " + declaration, "port.h", in.int16));
        std::vector<std::pair<std::string, Alias>> alias_order(aliases.begin(), aliases.end());   // (in the order the places are)
        std::stable_sort(alias_order.begin(), alias_order.end(), [](const auto& a, const auto& b) { return a.second.where < b.second.where; });
        for (const auto& [name, alias] : alias_order) port.push_back("#define " + name + "\t" + alias.where);
    }
    port.push_back(include_line("proto.h"));
    port.push_back("#endif");

    // ---- which headers go into which data file: headers that define one tag two ways are not in one file
    std::vector<std::vector<std::string>> groups;
    {
        std::set<std::string> data_headers;
        for (const auto& [h, d] : in.data)
            if (h.ends_with(".h")) data_headers.insert(h);
        for (const std::string& h : data_headers) {
            std::vector<std::string>* home = nullptr;
            for (auto& g : groups) {
                bool conflict = false;
                for (const std::string& t : index.tags_of(h)) {
                    bool in_group = false, alike = false;
                    for (const std::string& x : g) {
                        if (!index.tags_of(x).contains(t)) continue;
                        in_group = true;
                        if (index.same_def(t, x, h)) alike = true;
                    }
                    if (in_group && !alike) conflict = true;
                }
                if (!conflict) {
                    home = &g;
                    break;
                }
            }
            if (home == nullptr) {
                groups.push_back({h});
            } else {
                home->push_back(h);
            }
        }
    }
    if (!in.asm_data.empty() || groups.empty()) {
        std::set<std::string> all;
        for (const auto& [h, d] : in.data)
            if (h.ends_with(".h")) all.insert(h);
        if (!in.asm_data.empty())
            for (const auto& kv : in.files)
                if (kv.first.ends_with(".h")) all.insert(kv.first);
        groups = {std::vector<std::string>(all.begin(), all.end())};
    }
    std::set<std::string> in_any_group;
    for (const auto& g : groups) in_any_group.insert(g.begin(), g.end());
    std::vector<std::pair<std::string, std::vector<std::string>>> datas;
    for (std::size_t k = 0; k < groups.size(); ++k) {
        const std::string name = k == 0 ? "data.c" : "data" + std::to_string(k + 1) + ".c";
        const auto& g = groups[k];
        std::vector<std::string> lines = {"/* " + name + " -- the variables the headers share, defined once (made by CodeBreaker's port) */",
                                          include_line("port.h")};
        for (const std::string& h : g) lines.push_back(include_line(h));
        lines.push_back("");
        for (const auto& [h, d] : in.data)
            if (std::find(g.begin(), g.end(), h) != g.end() || (k == 0 && !in_any_group.contains(h))) lines.push_back(d);
        if (!in.asm_data.empty() && k == 0) {
            lines.push_back("");
            lines.push_back("/* the variables an assembler module of data alone placed */");
            for (const auto& [h, d] : in.asm_data) lines.push_back(d);
        }
        datas.emplace_back(name, std::move(lines));
    }

    // ---- bss.c (made after port.h's text: the BSS definitions may give names places of their own)
    const std::vector<std::string> bss_defs = bss.definitions(aliases);
    std::vector<std::string> bdata = {
        "/* bss.c -- the variables the old build defined beginning without a value (its BSS list), as the program declares them "
        "(made by CodeBreaker's port) */",
        include_line("port.h")};
    std::set<std::string> seen_headers;
    // (and first what its members' structures need: by value -- a complete type)
    std::function<void(const std::string&)> add = [&](const std::string& h) {
        if (seen_headers.contains(h)) return;
        seen_headers.insert(h);
        std::vector<std::string> needed;
        member_by_value.for_each_match(index.blanked(h), [&](const RegexMatch& m) {
            needed.push_back(collapse_space(m.group(1)));
            return true;
        });
        for (const std::string& u : needed) {
            const auto d = index.defining(u);
            if (d && *d != h) add(*d);
        }
        bdata.push_back(include_line(h));
    };
    {
        std::set<std::string> raw_tags;
        const std::string joined = text::join(bss_defs, "\n");
        bss_struct_var.for_each_match(joined, [&](const RegexMatch& m) {
            raw_tags.emplace(m.group(1));
            return true;
        });
        for (const std::string& u : raw_tags) {   // (a structure of its own, not a pointer: its header)
            if (const auto h = index.defining(collapse_space(u))) add(*h);
        }
    }
    std::vector<std::string> bss_lines = bdata;
    bss_lines.push_back("");
    bss_lines.insert(bss_lines.end(), bss_defs.begin(), bss_defs.end());
    const std::string bss_text = widths(text::join(bss_lines, "\n") + "\n", "bss.c", in.int16);

    auto finish_into = [&](const std::string& name, const std::string& text) {
        Finished f = finish(rules, name, text);
        result.origins.insert_or_assign(name, std::move(f.origin));
        result.files.insert_or_assign(name, std::move(f.text));
    };
    finish_into("port.h", text::join(port, "\n") + "\n");
    finish_into("proto.h", proto_text + "\n");
    if (!bss_defs.empty()) {
        finish_into("bss.c", bss_text);
    } else {
        result.files.insert_or_assign("bss.c", std::nullopt);
    }
    for (const auto& [name, lines] : datas) {
        const std::string text = widths(text::join(lines, "\n") + "\n", name, in.int16);
        if (!in.data.empty() || !in.asm_data.empty()) {
            finish_into(name, text);
        } else {
            result.files.insert_or_assign(name, std::nullopt);
        }
    }
    return result;
}

}  // namespace cb::port::pre_ansi
