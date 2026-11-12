// cb-unity: alone (its file-local helpers are named for what they do here, not for a unity neighbour)
#include "cb/java/twophase.hpp"

#include <algorithm>
#include <map>
#include <unordered_set>

#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/java/strview.hpp"
#include "cb/core/text.hpp"

namespace cb::java::twophase {

namespace {

namespace stdfs = std::filesystem;

bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }
bool is_word_start(char c) { return is_word(c) && !(c >= '0' && c <= '9'); }

bool is_keyword(std::string_view name) {
    static const std::unordered_set<std::string_view> kKeywords = {"if",  "for",  "while", "switch", "catch",
                                                                   "return", "new", "super", "this", "synchronized"};
    return kKeywords.contains(name);
}

// The text a constructor is made of: its statements, the initialisers of the instance fields and the instance
// initialiser blocks -- everything that runs while the base class is still being built.
std::string constructor_text(const ir::JClass& cls) {
    std::vector<std::string_view> parts;
    for (const ir::Method* ctor : cls.constructors()) {
        for (const ir::SourceLine& line : ctor->body) parts.push_back(line.text);
    }
    for (const ir::Field* f : cls.instance_fields()) {
        if (f->initializer && !f->initializer->empty()) parts.push_back(*f->initializer);
    }
    for (const ir::Body& block : cls.inst_inits) {
        for (const ir::SourceLine& line : block) parts.push_back(line.text);
    }
    return text::join(parts, "\n");
}

// The names called in a constructor: `name(` or `this.name(`, not as a member of something else.
NameSet called_names(const std::string& ctor_text) {
    static const Regex call(R"((?<![\w.>:])(?:this\s*\.\s*)?([A-Za-z_]\w*)\s*\()");
    NameSet out;
    call.for_each_match(ctor_text, [&](const RegexMatch& m) {
        const std::string_view name = m.group(1);
        if (!is_keyword(name)) out.emplace(name);
        return true;
    });
    return out;
}

NameSet instance_method_names(const ir::JClass& c) {
    NameSet out;
    for (const ir::Method& m : c.methods) {
        if (!m.is_ctor && !m.is_static) out.insert(m.name);
    }
    return out;
}

// Does one of the constructors start with `super(<args>);` with arguments?
bool super_call_with_args(const ir::JClass& c) {
    static const Regex super_args(R"(^\s*super\s*\((.*)\)\s*;)");
    for (const ir::Method* ctor : c.constructors()) {
        for (const ir::SourceLine& line : ctor->body) {
            const std::optional<RegexMatch> m = super_args.match(line.text);
            if (m && !text::trim(m->group(1)).empty()) return true;
        }
    }
    return false;
}

// Index just past the parenthesis that closes the one opened before `i`, or npos.
std::size_t close_paren(std::string_view text, std::size_t i) {
    std::size_t depth = 1;
    char quote = 0;
    for (; i < text.size(); ++i) {
        const char ch = text[i];
        if (quote != 0) {
            if (ch == '\\') {
                ++i;
            } else if (ch == quote) {
                quote = 0;
            }
        } else if (ch == '"' || ch == '\'') {
            quote = ch;
        } else if (ch == '(') {
            ++depth;
        } else if (ch == ')') {
            if (--depth == 0) return i + 1;
        }
    }
    return std::string_view::npos;
}

std::size_t skip_blanks(std::string_view text, std::size_t i) {
    while (i < text.size() && text::is_space(text[i])) ++i;
    return i;
}

}  // namespace

NameSet compute(std::span<const ir::JClass> classes, const NameSet& values) {
    std::map<std::string, const ir::JClass*, std::less<>> by_name;   // a name met twice keeps the later class
    for (const ir::JClass& c : classes) by_name[c.name] = &c;
    std::map<std::string, std::vector<std::string>, std::less<>> children;
    for (const ir::JClass& c : classes) {
        if (c.parent && by_name.contains(*c.parent)) children[*c.parent].push_back(c.name);
    }

    auto descendants = [&](const std::string& name) {
        std::vector<std::string> out;
        NameSet seen;
        std::vector<std::string> todo;
        if (const auto it = children.find(name); it != children.end()) todo = it->second;
        while (!todo.empty()) {
            const std::string n = std::move(todo.back());
            todo.pop_back();
            if (!seen.insert(n).second) continue;   // (a cycle in the parents must not loop)
            out.push_back(n);
            if (const auto it = children.find(n); it != children.end()) todo.insert(todo.end(), it->second.begin(), it->second.end());
        }
        return out;
    };

    NameSet roots;
    for (const ir::JClass& c : classes) {
        if (c.is_interface || c.is_enum || (c.constructors().empty() && c.instance_fields().empty())) continue;
        const NameSet called = called_names(constructor_text(c));
        if (called.empty()) continue;
        NameSet own;   // its own and inherited instance methods
        NameSet visited;
        for (const ir::JClass* k = &c; k != nullptr;) {
            if (!visited.insert(k->name).second) break;
            for (const std::string& n : instance_method_names(*k)) own.insert(n);
            const auto parent = k->parent ? by_name.find(*k->parent) : by_name.end();
            k = parent == by_name.end() ? nullptr : parent->second;
        }
        NameSet abstract_or_overridden;
        for (const ir::Method& m : c.methods) {
            if (m.is_abstract) abstract_or_overridden.insert(m.name);
        }
        for (const std::string& d : descendants(c.name)) {
            for (const std::string& n : instance_method_names(*by_name.at(d))) abstract_or_overridden.insert(n);
        }
        const bool virtual_call = std::any_of(called.begin(), called.end(), [&](const std::string& n) {
            return own.contains(n) && abstract_or_overridden.contains(n);
        });
        if (virtual_call) roots.insert(c.name);
    }

    NameSet picked;
    for (const std::string& r : roots) {
        picked.insert(r);
        for (const std::string& d : descendants(r)) picked.insert(d);
    }
    auto drop = [&](const std::string& name) {
        picked.erase(name);
        for (const std::string& d : descendants(name)) picked.erase(d);
    };

    bool changed = true;
    while (changed) {
        changed = false;
        for (const std::string& name : NameSet(picked)) {
            const ir::JClass& c = *by_name.at(name);
            const bool parent_is_user_class = c.parent && by_name.contains(*c.parent);
            if (values.contains(name) || (parent_is_user_class && !picked.contains(*c.parent) && super_call_with_args(c))) {
                drop(name);
                changed = true;
                break;
            }
        }
    }
    return picked;
}

Rewritten rewrite_news(std::string_view text, const NameSet& names) {
    if (names.empty() || !contains(text, "new ")) return {std::string(text), 0};
    Rewritten out;
    std::size_t pos = 0;   // what has been copied to the output so far
    for (std::size_t at = text.find("new"); at != std::string_view::npos; at = text.find("new", at + 1)) {
        if (at < pos) continue;   // inside the arguments of a construction just rewritten
        if (at > 0 && is_word(text[at - 1])) continue;   // `renew`, `anew`: not the keyword
        std::size_t i = at + 3;
        if (i >= text.size() || !text::is_space(text[i])) continue;
        i = skip_blanks(text, i);
        std::size_t id_end = i;
        while (id_end < text.size() && is_word(text[id_end])) ++id_end;
        const std::string_view name = text.substr(i, id_end - i);
        if (name.empty() || !names.contains(name)) continue;   // (`new C[n]` stops here too: no parenthesis)
        const std::size_t open = skip_blanks(text, id_end);
        if (open >= text.size() || text[open] != '(') continue;
        const std::size_t end = close_paren(text, open + 1);
        if (end == std::string_view::npos) continue;
        const std::string_view args = text.substr(open + 1, end - open - 2);
        if (text::trim(args).empty() && text.substr(end).starts_with(")->j2xInit(")) continue;   // already in that form
        const Rewritten inner = rewrite_news(args, names);
        out.text.append(text.substr(pos, at - pos));
        out.text += "(new ";
        out.text.append(name);
        out.text += "())->j2xInit(";
        out.text += inner.text;
        out.text += ')';
        pos = end;
        out.count += 1 + inner.count;
    }
    out.text.append(text.substr(pos));
    return out;
}

Rewritten rewrite_stack(std::string_view text, const NameSet& names) {
    if (names.empty()) return {std::string(text), 0};
    Rewritten out;
    std::size_t pos = 0;
    std::size_t i = 0;
    while (i < text.size()) {
        if (!is_word(text[i])) {
            ++i;
            continue;
        }
        const std::size_t start = i;
        std::size_t id_end = i;
        while (id_end < text.size() && is_word(text[id_end])) ++id_end;
        const std::string_view type_name = text.substr(start, id_end - start);
        // a type name that does not follow a word, a scope, a template bracket, a pointer or a reference
        const bool boundary = start == 0 || !(is_word(text[start - 1]) || std::string_view(":<*&").find(text[start - 1]) != std::string_view::npos);
        const std::size_t next = id_end;
        i = id_end;
        if (!boundary || !names.contains(type_name)) continue;
        // `C _j2xleakN` ...
        if (next >= text.size() || !text::is_space(text[next])) continue;
        std::size_t v = skip_blanks(text, next);
        static constexpr std::string_view kLeak = "_j2xleak";
        if (!text.substr(v).starts_with(kLeak)) continue;
        std::size_t v_end = v + kLeak.size();
        const std::size_t digits = v_end;
        while (v_end < text.size() && text[v_end] >= '0' && text[v_end] <= '9') ++v_end;
        if (v_end == digits) continue;
        const std::string_view var = text.substr(v, v_end - v);
        // ... then `;` or `(args);` (the arguments on one line, ending at the first `)` that is followed by `;`)
        std::size_t k = skip_blanks(text, v_end);
        std::string_view args;
        std::size_t match_end = std::string_view::npos;
        if (k < text.size() && text[k] == ';') {
            match_end = k + 1;
        } else if (k < text.size() && text[k] == '(') {
            for (std::size_t j = k + 1; j < text.size() && text[j] != '\n'; ++j) {
                if (text[j] != ')') continue;
                const std::size_t semi = skip_blanks(text, j + 1);
                if (semi < text.size() && text[semi] == ';') {
                    args = text.substr(k + 1, j - k - 1);
                    match_end = semi + 1;
                    break;
                }
            }
        }
        if (match_end == std::string_view::npos) continue;
        out.text.append(text.substr(pos, start - pos));
        out.text += std::string(type_name) + " " + std::string(var) + "; " + std::string(var) + ".j2xInit(" + std::string(args) + ");";
        pos = match_end;
        i = match_end;
        ++out.count;
    }
    out.text.append(text.substr(pos));
    return out;
}

RunResult run(const stdfs::path& out_dir, const NameSet& names) {
    RunResult result;
    if (names.empty()) return result;
    std::vector<std::string> files;
    for (const stdfs::directory_entry& e : stdfs::directory_iterator(out_dir)) {
        const std::string name = fs::to_text(e.path().filename());
        if (e.is_regular_file() && (name.ends_with(".cpp") || name.ends_with(".h"))) files.push_back(name);
    }
    std::sort(files.begin(), files.end());
    for (const std::string& name : files) {
        const stdfs::path path = out_dir / fs::from_text(name);
        const std::string original = fs::read_text(path);
        const Rewritten news = rewrite_news(original, names);
        const Rewritten stack = rewrite_stack(news.text, names);
        const std::size_t sites = news.count + stack.count;
        if (sites == 0) continue;
        fs::write_text(path, stack.text);
        result.sites += sites;
        result.files.push_back(name);
    }
    return result;
}

}  // namespace cb::java::twophase
