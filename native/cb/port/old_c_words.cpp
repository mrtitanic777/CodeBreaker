// cb/port/old_c_words.cpp -- word_stacks (old_c_passes.hpp): the words that held pointers.

#include <algorithm>
#include <format>
#include <set>

#include "cb/base/ctext.hpp"
#include "cb/core/text.hpp"
#include "cb/port/old_c_passes.hpp"
#include "cb/port/old_c_scan.hpp"

namespace cb::port::pre_ansi {

namespace {

using base::ctext::blank;

// An int variable of the program: an array at a file's top (no function) or a variable of one function.
struct Node {
    std::string file;
    std::optional<std::string> function;
    std::string name;
    friend auto operator<=>(const Node&, const Node&) = default;
};

// Which variables are one: a union-find over nodes.
class Equivalence {
public:
    // The representative of the node's set (the node itself when it has not been met).
    Node find(const Node& node) {
        parent_.try_emplace(node, node);
        Node at = node;
        while (parent_.at(at) != at) {
            const Node grandparent = parent_.at(parent_.at(at));
            parent_[at] = grandparent;   // (path halving)
            at = grandparent;
        }
        return at;
    }
    void unite(const Node& a, const Node& b) {
        const Node ra = find(a), rb = find(b);
        parent_[ra] = rb;
    }
    std::set<Node> members() const {
        std::set<Node> out;
        for (const auto& kv : parent_) out.insert(kv.first);
        return out;
    }

private:
    std::map<Node, Node> parent_;
};

constexpr const char* kInt = R"re((?:register\s+)?(?:unsigned\s+|signed\s+)?(?:int|short|unsigned))re";

}  // namespace

void word_stacks(ProgramState& program) {
    static const Regex star_name(R"re(\*\s*(\w+)\s*(?=[;,=\[)]))re");
    static const Regex int_array(std::string(R"re(\s*(?:extern\s+|static\s+)?)re") + kInt +
                                 R"re(\s+(\w+)\s*\[[^\]]*\]\s*(?:=[^;]*)?;)re");
    static const Regex int_declaration(std::string(R"re((?m)^\s*)re") + kInt + R"re(\s+([^;(){}]*);)re");
    static const Regex declared_word(R"re((?:^|,)\s*(?:\*\s*(\w+)|(\w+)\s*\[[^\]]*\])\s*(?=,|$))re");
    static const Regex local_pointer(R"re(\*\s*(\w+)\s*[;,=)\[])re");
    static const Regex declaration_line(R"re(\s*(?:register\s+)?(?:struct|union|char|int|unsigned|long|short)\b)re");
    static const Regex word_given(R"re((?:\b(\w+)\s*\[[^\]]*\]|\*\s*(\w+)\s*(?:\+\+|--)?)\s*=(?!=)\s*([^;]*);)re");
    static const Regex address_or_string(R"re(&\s*\w+.*|".*)re");
    static const Regex cast_then_word(R"re(\(\s*(?:struct\s+\w+|\w+)\s*\*+\s*\)\s*(\w+)\s*\[)re");
    static const Regex cast_to_pointers(R"re(\(\s*(?:struct\s+\w+|\w+)\s*\*\s*\*\s*\)\s*(\w+)\b(?!\s*\[))re");
    static const Regex given_or_compared(
        R"re(\b(\w+)\s*(?:=(?!=)|[<>]=?|==|!=)\s*(?:\(\s*\w+\s*\*\s*\)\s*)?(\w+)\b(?!\s*[\[(]))re");
    static const Regex pointer_from_word(
        R"re(\b(\w+)\s*=\s*\(\s*(?:unsigned\s+|signed\s+)?(?:int|short|unsigned)\s*\*\s*\)\s*\w+\s*\[)re");
    static const Regex declaration_head(R"re((\s*(?:register\s+)?)(?:unsigned\s+|signed\s+)?(?:int|short|unsigned)(\s+)(.*;.*)$)re");
    static const Regex array_head(std::string(R"re((?m)^(\s*(?:extern\s+|static\s+)?))re") + kInt + R"re((\s+(\w+)\s*\[))re");
    static const Regex cast_of_it(R"re((\b(\w+)\s*=\s*\(\s*)(?:unsigned\s+|signed\s+)?(?:int|short|unsigned)(\s*\*\s*\)))re");

    Files& files = program.files;
    std::set<std::string, std::less<>> pointer_names;          // names declared pointers anywhere, at the top
    std::map<std::string, std::vector<std::string>> arrays;    // an int array at a file's top: the files that declare it
    for (const auto& [n, t] : files) {
        const std::vector<std::string> code = split_lines(blank(t));
        const std::vector<int> depth = brace_depths(code);
        for (std::size_t i = 0; i < code.size(); ++i) {
            if (depth[i]) continue;
            star_name.for_each_match(code[i], [&](const RegexMatch& m) {
                const std::string_view before = std::string_view(code[i]).substr(0, m.start());
                if (before.find('(') == std::string_view::npos || code[i].find("(*") != std::string::npos)
                    pointer_names.insert(std::string(m.group(1)));
                return true;
            });
            if (const auto m = int_array.match(code[i])) arrays[std::string(m->group(1))].push_back(n);
        }
    }

    Equivalence same;
    std::set<Node> seeds, nodes;
    std::map<std::pair<std::string, std::size_t>, Node> params;   // (function, parameter index): the int pointer it is
    std::set<std::string, std::less<>> defined;                    // the program's functions
    for (const std::string& n : sorted_names(files)) {
        if (!n.ends_with(".c")) continue;
        const std::string& t = files.at(n);
        const std::vector<std::string> lines = split_lines(t), code = split_lines(blank(t));
        for (const Definition& d : definitions(lines, code)) {
            const std::string head = d.open > d.first ? join_range(code, d.first, d.open - 1) : std::string();
            const std::string body = join_range(code, d.open, d.last);
            std::map<std::string, Node> mine;   // an int pointer or array of the function's: its node
            int_declaration.for_each_match(head + "\n" + body, [&](const RegexMatch& decl) {
                declared_word.for_each_match(decl.group(1), [&](const RegexMatch& w) {
                    const std::string name(w.has(1) ? w.group(1) : w.group(2));
                    mine[name] = Node{n, d.fn.name, name};
                    return true;
                });
                return true;
            });
            defined.insert(d.fn.name);
            for (std::size_t k = 0; k < d.fn.params.size(); ++k)
                if (const auto it = mine.find(d.fn.params[k].name); it != mine.end())
                    params.insert_or_assign(std::make_pair(d.fn.name, k), it->second);
            std::set<std::string, std::less<>> local_pointers;
            {
                std::vector<std::string> declaration_lines;   // (the body's lines that declare something)
                for (const std::string& l : split_lines(body))
                    if (text::trim(l).ends_with(';') && declaration_line.match(l)) declaration_lines.push_back(l);
                const std::string declared = head + "\n" + text::join(declaration_lines, "\n");
                local_pointer.for_each_match(declared, [&](const RegexMatch& m) {
                    local_pointers.insert(std::string(m.group(1)));
                    return true;
                });
            }
            auto node = [&](std::string_view v) -> std::optional<Node> {
                if (const auto it = mine.find(std::string(v)); it != mine.end()) return it->second;
                if (arrays.contains(std::string(v))) return Node{"", std::nullopt, std::string(v)};
                return std::nullopt;
            };
            for (const auto& kv : mine) nodes.insert(*node(kv.first));
            for (const auto& kv : arrays)
                if (find_word(body, kv.first) != std::string::npos) nodes.insert(*node(kv.first));
            word_given.for_each_match(body, [&](const RegexMatch& m) {   // (a pointer kept in a word of it)
                const auto v = node(m.has(1) ? m.group(1) : m.group(2));
                const std::string_view e = text::trim(m.group(3));
                const bool plain = !e.empty() && std::all_of(e.begin(), e.end(), is_word_char);
                if (v && (address_or_string.full_match(e) ||
                          (plain && (local_pointers.contains(e) || mine.contains(std::string(e)) || pointer_names.contains(e)))))
                    seeds.insert(*v);
                return true;
            });
            cast_then_word.for_each_match(body, [&](const RegexMatch& m) {   // (a word of it read as a pointer)
                if (const auto v = node(m.group(1))) seeds.insert(*v);
                return true;
            });
            cast_to_pointers.for_each_match(body, [&](const RegexMatch& m) {   // (it, taken as pointers)
                if (const auto v = node(m.group(1))) seeds.insert(*v);
                return true;
            });
            given_or_compared.for_each_match(body, [&](const RegexMatch& m) {   // (one given the other, or compared)
                const auto a = node(m.group(1)), b = node(m.group(2));
                if (a && b) same.unite(*a, *b);
                return true;
            });
            pointer_from_word.for_each_match(body, [&](const RegexMatch& m) {   // (fp = (int *)sp[2]: a pointer kept in a word)
                if (const auto v = node(m.group(1))) seeds.insert(*v);
                return true;
            });
        }
    }
    for (const std::string& n : sorted_names(files)) {   // (a call: its arguments one with the parameters)
        if (!n.ends_with(".c")) continue;
        const std::string& t = files.at(n);
        const std::vector<std::string> lines = split_lines(t), code = split_lines(blank(t));
        for (const Definition& d : definitions(lines, code)) {
            const std::string body = join_range(code, d.open, d.last);
            std::map<std::string, Node> mine;
            int_declaration.for_each_match(join_range(code, d.first, d.last), [&](const RegexMatch& decl) {
                declared_word.for_each_match(decl.group(1), [&](const RegexMatch& w) {
                    const std::string name(w.has(1) ? w.group(1) : w.group(2));
                    mine[name] = Node{n, d.fn.name, name};
                    return true;
                });
                return true;
            });
            for (const CallSite& site : call_sites(body, [&](std::string_view w) { return defined.contains(w); }, /*not_member=*/true)) {
                const std::vector<Span> spans =
                    call_args(std::string_view(body).substr(site.at, 400), site.name).value_or(std::vector<Span>{});
                for (std::size_t k = 0; k < spans.size(); ++k) {
                    const std::string arg(text::trim(std::string_view(body).substr(site.at + spans[k].begin, spans[k].size())));
                    const auto formal = params.find({std::string(site.name), k});
                    if (formal == params.end() || arg.empty() || !std::all_of(arg.begin(), arg.end(), is_word_char)) continue;
                    if (const auto it = mine.find(arg); it != mine.end()) {
                        same.unite(it->second, formal->second);
                    } else if (arrays.contains(arg)) {
                        same.unite(Node{"", std::nullopt, arg}, formal->second);
                    }
                }
            }
        }
    }

    std::set<Node> wide;
    for (const Node& s : seeds) wide.insert(same.find(s));
    std::set<Node> everything = same.members();
    everything.insert(nodes.begin(), nodes.end());
    everything.insert(seeds.begin(), seeds.end());
    std::vector<Node> chosen;
    for (const Node& x : everything)
        if (wide.contains(same.find(x))) chosen.push_back(x);
    if (chosen.empty()) return;
    auto order = [](const Node& x) { return std::make_tuple(x.file, x.function.value_or(std::string()), x.name); };
    std::sort(chosen.begin(), chosen.end(), [&](const Node& a, const Node& b) { return order(a) < order(b); });

    NamedRegex declares_it(R"re(\*\s*@1@|@1@\s*\[[^\]]*\])re");
    static constexpr const char* kWhy = "words that held pointers too (a stack, a list of results): intptr_t";
    for (const Node& x : chosen) {
        const std::string& v = x.name;
        if (!x.function) {   // (an array: its definition and every extern of it)
            for (const std::string& f : arrays[v]) {
                files.insert_or_assign(f, array_head.replace_all(files.at(f), [&](const RegexMatch& m) {
                                           return m.group(3) == v ? std::string(m.group(1)) + "intptr_t" + std::string(m.group(2))
                                                                  : std::string(m.whole());
                                       }));
            }
            note(program.notes, kWhy, v);
            continue;
        }
        const std::string& n = x.file;
        std::vector<std::string> lines = split_lines(files.at(n));
        const std::vector<std::string> code = split_lines(blank(files.at(n)));
        for (const Definition& d : definitions(lines, code)) {
            if (d.fn.name != *x.function) continue;
            for (std::size_t line = d.first; line <= d.last; ++line) {
                const auto m = declaration_head.match(lines[line]);
                if (!m) continue;
                const std::string_view declarators = m->group(3).substr(0, m->group(3).find(';'));
                std::vector<std::string> decls, keep;
                std::optional<std::string> mine_decl;
                for (const std::string_view piece : split_top(declarators)) {
                    const std::string dcl(text::trim(piece));
                    decls.push_back(dcl);
                    if (declares_it.with(v).full_match(dcl)) {
                        if (!mine_decl) mine_decl = dcl;
                    } else {
                        keep.push_back(dcl);
                    }
                }
                if (!mine_decl) continue;
                const std::string rest = std::string(m->group(3).substr(declarators.size()));
                const std::size_t prefix = m->group(1).size();
                const std::string base = lines[line].substr(prefix, lines[line].find(m->group(2), prefix) - prefix);
                std::string rewritten = std::string(m->group(1)) + "intptr_t " + *mine_decl + ";";
                if (!keep.empty()) rewritten = std::string(m->group(1)) + base + " " + text::join(keep, ", ") + "; intptr_t " + *mine_decl + ";";
                lines[line] = rewritten + rest.substr(1);
                break;
            }
            // (fp = (int *)sp[2]: an intptr_t * taken out)
            const std::string body = cast_of_it.replace_all(join_range(lines, d.open, d.last), [&](const RegexMatch& m) {
                return m.group(2) == v ? std::string(m.group(1)) + "intptr_t" + std::string(m.group(3)) : std::string(m.whole());
            });
            const std::vector<std::string> body_lines = split_lines(body);
            for (std::size_t i = 0; i < body_lines.size(); ++i) lines[d.open + i] = body_lines[i];
        }
        files.insert_or_assign(n, join_lines(lines));
        note(program.notes, kWhy, std::format("{} ({}: {})", n, *x.function, v));
    }
}

}  // namespace cb::port::pre_ansi
