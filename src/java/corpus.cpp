#include "java/corpus.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "core/fs.hpp"
#include "core/ordered_map.hpp"
#include "core/text.hpp"
#include "java/emit/ownership.hpp"

namespace cb::java::corpus {

namespace stdfs = std::filesystem;

namespace {

bool cor_is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }

template <class Classes>
auto cor_index(Classes& classes) {
    std::map<std::string, decltype(&classes[0]), std::less<>> by_name;
    for (auto& c : classes) by_name[c.name] = &c;   // a name met twice keeps the later class, as a dict would
    return by_name;
}

// The raw text of a method's body: its lines, one per line.
std::string cor_body_text(const ir::Method& m) {
    std::string out;
    for (std::size_t i = 0; i < m.body.size(); ++i) {
        if (i != 0) out += '\n';
        out += m.body[i].text;
    }
    return out;
}

// Does the text contain the whole word `word` (not part of a longer identifier)?
std::vector<std::size_t> cor_word_positions(std::string_view text, std::string_view word) {
    std::vector<std::size_t> found;
    for (std::size_t at = text.find(word); at != std::string_view::npos; at = text.find(word, at + 1)) {
        const bool starts = at == 0 || !cor_is_word(text[at - 1]);
        const bool ends = at + word.size() >= text.size() || !cor_is_word(text[at + word.size()]);
        if (starts && ends) found.push_back(at);
    }
    return found;
}

std::size_t cor_skip_space(std::string_view text, std::size_t i) {
    while (i < text.size() && text::is_space(text[i])) ++i;
    return i;
}

// `return null;` -- the statement, with whitespace between and before the semicolon
bool cor_returns_null(std::string_view body) {
    for (const std::size_t at : cor_word_positions(body, "return")) {
        std::size_t i = at + 6;
        const std::size_t after_return = i;
        i = cor_skip_space(body, i);
        if (i == after_return || !body.substr(i).starts_with("null")) continue;
        i = cor_skip_space(body, i + 4);
        if (i < body.size() && body[i] == ';') return true;
    }
    return false;
}

// `this` used as a value: not followed by `.`, `->` or `(`
bool cor_uses_this_as_value(std::string_view text) {
    for (const std::size_t at : cor_word_positions(text, "this")) {
        const std::size_t i = cor_skip_space(text, at + 4);
        if (i < text.size() && (text[i] == '.' || text[i] == '(' || text.substr(i).starts_with("->"))) continue;
        return true;
    }
    return false;
}

}  // namespace

// ---------------------------------------------------------------------------------------------- discover

namespace {

void cor_walk(const stdfs::path& dir, std::map<std::string, stdfs::path>& found) {
    std::error_code ec;
    std::vector<stdfs::path> files, folders;
    for (const auto& entry : stdfs::directory_iterator(dir, ec)) {
        if (entry.is_directory(ec)) folders.push_back(entry.path());
        else if (entry.is_regular_file(ec)) files.push_back(entry.path());
    }
    const auto by_name = [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a.filename()) < fs::to_text(b.filename()); };
    std::ranges::sort(files, by_name);
    std::ranges::sort(folders, by_name);
    for (const stdfs::path& f : files) {
        const std::string name = fs::to_text(f.filename());
        if (name.ends_with(".java")) found.emplace(name, f);   // the first found keeps the name
    }
    for (const stdfs::path& d : folders) cor_walk(d, found);
}

}  // namespace

std::vector<stdfs::path> discover(const stdfs::path& java_dir) {
    std::map<std::string, stdfs::path> found;
    cor_walk(java_dir, found);
    std::vector<stdfs::path> out;
    for (auto& [name, path] : found) out.push_back(std::move(path));
    return out;
}

// ---------------------------------------------------------------------------------------------- the passes

Dropped drop_synthetic_artifacts(std::vector<ir::JClass>& classes, ledger::Ledger* ledger) {
    // names used as a base are never droppable
    NameSet used_as_base;
    for (const ir::JClass& c : classes) {
        if (c.parent) used_as_base.insert(*c.parent);
        used_as_base.insert(c.interfaces.begin(), c.interfaces.end());
    }
    // the sole-parameter constructor types are the bridge-constructor signal
    NameSet bridge_param_types;
    for (const ir::JClass& c : classes) {
        for (const ir::Method& m : c.methods) {
            if (m.is_ctor && m.params.size() == 1) bridge_param_types.insert(m.params[0].type.name);
        }
    }
    // a marker is EMPTY (no members, parent or interfaces), never a base, AND used only as a bridge parameter
    Dropped dropped;
    for (const ir::JClass& c : classes) {
        if (c.is_enum || c.is_interface || !c.fields.empty() || !c.methods.empty() || c.parent || !c.interfaces.empty()) continue;
        if (!used_as_base.contains(c.name) && bridge_param_types.contains(c.name)) dropped.markers.insert(c.name);
    }
    if (dropped.markers.empty()) return dropped;
    for (ir::JClass& c : classes) {
        const std::size_t before = c.methods.size();
        std::erase_if(c.methods, [&dropped](const ir::Method& m) {
            return m.is_ctor && m.params.size() == 1 && dropped.markers.contains(m.params[0].type.name);   // a synthetic bridge constructor
        });
        dropped.bridge_constructors += before - c.methods.size();
    }
    std::erase_if(classes, [&dropped](const ir::JClass& c) { return dropped.markers.contains(c.name); });
    if (ledger != nullptr) {
        const std::vector<std::string> names(dropped.markers.begin(), dropped.markers.end());
        ledger->add("(pipeline)", ledger::Category::Ported,
                    std::format("dropped {} empty marker class(es) [{}] + {} synthetic bridge constructor(s)", dropped.markers.size(),
                                text::join(names, ", "), dropped.bridge_constructors),
                    {.source = "pipeline", .rule = "drop-synthetic"});
    }
    return dropped;
}

void resolve_inheritance(std::vector<ir::JClass>& classes) {
    const auto by_name = cor_index(classes);
    for (ir::JClass& cls : classes) {
        std::set<std::string, std::less<>> seen;
        for (const ir::Field& f : cls.fields) seen.insert(f.name);
        std::vector<const ir::Field*> inherited;
        std::set<std::string, std::less<>> guard = {cls.name};
        std::optional<std::string> parent = cls.parent;
        while (parent) {
            const auto it = by_name.find(*parent);
            if (it == by_name.end() || guard.contains(*parent)) break;
            guard.insert(*parent);
            const ir::JClass& pc = *it->second;
            for (const ir::Field& f : pc.fields) {
                if (!f.is_static && f.access != ir::Access::Private && !seen.contains(f.name)) {
                    inherited.push_back(&f);
                    seen.insert(f.name);
                }
            }
            parent = pc.parent;
        }
        cls.inherited_fields = std::move(inherited);
    }
}

void compute_unhidden_overloads(std::vector<ir::JClass>& classes) {
    const auto by_name = cor_index(classes);
    const auto signature = [](const ir::Method& m) {
        std::string s;
        for (const ir::Param& p : m.params) s += p.type.name + std::string(p.type.array_dims, '[') + ",";
        return s;
    };
    for (ir::JClass& cls : classes) {
        cls.unhidden_overloads.clear();
        if (cls.is_interface || !cls.parent || by_name.find(*cls.parent) == by_name.end()) continue;
        std::map<std::string, std::set<std::string>, std::less<>> own;   // name -> the parameter lists this class declares
        for (const ir::Method& m : cls.methods)
            if (!m.is_ctor) own[m.name].insert(signature(m));
        std::set<std::string, std::less<>> needed;
        std::set<std::string, std::less<>> guard;
        auto it = by_name.find(*cls.parent);
        while (it != by_name.end() && guard.insert(it->second->name).second) {
            const ir::JClass& anc = *it->second;
            if (anc.is_interface) break;
            for (const ir::Method& m : anc.methods) {
                if (m.is_ctor) continue;
                const auto mine = own.find(m.name);
                if (mine != own.end() && !mine->second.contains(signature(m))) needed.insert(m.name);
            }
            if (!anc.parent) break;
            it = by_name.find(*anc.parent);
        }
        cls.unhidden_overloads.assign(needed.begin(), needed.end());
    }
}

void compute_mi_forwards(std::vector<ir::JClass>& classes) {
    const auto by_name = cor_index(classes);
    // all methods from the implemented interfaces, transitively, in the order met (the first declaration of a name wins)
    const auto interface_methods = [&by_name](const ir::JClass& cls) {
        OrderedMap<std::string, const ir::Method*> out;
        std::vector<std::string> stack(cls.interfaces.begin(), cls.interfaces.end());
        std::set<std::string, std::less<>> seen;
        while (!stack.empty()) {
            const std::string name = std::move(stack.back());
            stack.pop_back();
            if (!seen.insert(name).second) continue;
            const auto it = by_name.find(name);
            if (it == by_name.end()) continue;
            const ir::JClass& ic = *it->second;
            for (const ir::Method& m : ic.methods) {
                if (!m.is_ctor && !out.contains(m.name)) out.insert_or_assign(m.name, &m);
            }
            stack.insert(stack.end(), ic.interfaces.begin(), ic.interfaces.end());
            if (ic.parent) stack.push_back(*ic.parent);
        }
        return out;
    };

    for (ir::JClass& cls : classes) {
        cls.mi_forwards.clear();
        if (cls.is_interface || cls.is_enum || cls.interfaces.empty()) continue;
        std::set<std::string, std::less<>> own;
        for (const ir::Method& m : cls.methods) {
            if (!m.is_ctor) own.insert(m.name);
        }
        // the nearest concrete implementation in the (non-interface) base-class chain: name -> (class, arity)
        std::map<std::string, std::pair<std::string, std::size_t>, std::less<>> base_concrete;
        std::set<std::string, std::less<>> guard;
        const ir::JClass* cur = nullptr;
        if (cls.parent) {
            const auto it = by_name.find(*cls.parent);
            if (it != by_name.end()) cur = it->second;
        }
        while (cur != nullptr && !guard.contains(cur->name)) {
            guard.insert(cur->name);
            if (!cur->is_interface) {
                for (const ir::Method& m : cur->methods) {
                    if (!m.is_ctor && !m.is_abstract) base_concrete.try_emplace(m.name, cur->name, m.arity());
                }
            }
            const auto next = cur->parent ? by_name.find(*cur->parent) : by_name.end();
            cur = next == by_name.end() ? nullptr : next->second;
        }
        for (const auto& [name, im] : interface_methods(cls)) {
            if (own.contains(name)) continue;   // the class overrides it
            const auto concrete = base_concrete.find(name);
            if (concrete == base_concrete.end()) continue;   // no inherited concrete implementation
            if (concrete->second.second != im->arity()) continue;   // same name, another signature: not the diamond
            cls.mi_forwards.push_back({im, concrete->second.first});
        }
    }
}

namespace {

// The JType of a runtime stub method's C++ return spelling, only when it is unambiguous enough to drive chain inference: a single
// reference-type pointer (`File*` -> File) or `std::string` (-> String, a value).
std::optional<ir::JType> cor_stub_return_type(std::string_view cpp) {
    cpp = text::trim(cpp);
    if (cpp == "std::string") return ir::JType{.name = "String"};
    if (!cpp.ends_with('*')) return std::nullopt;
    const std::string_view name = text::rtrim(cpp.substr(0, cpp.size() - 1));
    if (!text::is_identifier(name) || name == "void") return std::nullopt;
    return ir::JType{.name = std::string(name)};
}

// One class as the flatten walks it: a corpus class or a runtime stub.
struct CorNode {
    std::optional<std::string> parent;
    std::vector<std::pair<std::string, ir::JType>> fields;
    std::vector<std::pair<std::string, ir::JType>> methods;
};

}  // namespace

std::map<std::string, types::MemberEnv, std::less<>> build_member_env(std::span<const ir::JClass> classes, const runtime::Runtime& runtime) {
    std::map<std::string, CorNode, std::less<>> nodes;
    std::vector<std::string> order;   // the corpus first, then the stubs
    for (const ir::JClass& c : classes) {
        CorNode n;
        n.parent = c.parent;
        for (const ir::Field& f : c.fields) n.fields.emplace_back(f.name, f.type);
        for (const ir::Method& m : c.methods) {
            if (!m.is_ctor && m.return_type) n.methods.emplace_back(m.name, *m.return_type);
        }
        nodes[c.name] = std::move(n);
        order.push_back(c.name);
    }
    // runtime-stub classes adopt the same record, so one flatten handles corpus->stub inheritance and stub->stub bases alike
    for (const auto& [name, rec] : runtime.stub_members()) {
        if (nodes.contains(name)) continue;   // corpus names win
        CorNode n;
        n.parent = rec.parent;
        for (const auto& [method, cpp] : rec.methods) {
            if (auto jt = cor_stub_return_type(cpp)) n.methods.emplace_back(method, std::move(*jt));
        }
        nodes[name] = std::move(n);
        order.push_back(name);
    }
    std::map<std::string, types::MemberEnv, std::less<>> env;
    for (const std::string& name : order) {
        types::MemberEnv e;
        e.parent = nodes.at(name).parent;
        std::set<std::string, std::less<>> guard;
        const CorNode* cur = &nodes.at(name);
        std::string cur_name = name;
        while (cur != nullptr && !guard.contains(cur_name)) {   // most-derived first; closer definitions are not overwritten
            guard.insert(cur_name);
            for (const auto& [f, t] : cur->fields) e.fields.try_emplace(f, t);
            for (const auto& [m, t] : cur->methods) e.methods.try_emplace(m, t);
            if (!cur->parent) break;
            const auto next = nodes.find(*cur->parent);
            if (next == nodes.end()) break;
            cur_name = *cur->parent;
            cur = &next->second;
        }
        env[name] = std::move(e);
    }
    // StringBuilder is a runtime type that is in no header the member environment reads: its chained methods must resolve
    // `.` -> `->` (append() returns the builder)
    types::MemberEnv builder;
    for (const char* m : {"append", "insert", "deleteCharAt", "reverse"}) builder.methods.emplace(m, ir::JType{.name = "StringBuilder"});
    builder.methods.emplace("toString", ir::JType{.name = "String"});
    builder.methods.emplace("charAt", ir::JType{.name = "char"});
    builder.methods.emplace("length", ir::JType{.name = "int"});
    env["StringBuilder"] = std::move(builder);
    return env;
}

std::map<std::string, NameSet, std::less<>> build_method_renames(std::span<const ir::JClass> classes) {
    const auto by_name = cor_index(classes);
    std::map<std::string, NameSet, std::less<>> own;
    for (const ir::JClass& c : classes) {
        NameSet fields;
        for (const ir::Field& f : c.fields) fields.insert(f.name);
        NameSet names;
        for (const ir::Method& m : c.methods) {
            if (!m.is_ctor && fields.contains(m.name)) names.insert(m.name);
        }
        own[c.name] = std::move(names);
    }
    // A field that shares its name with an INHERITED method (Alpha 1.0.6's EntityBoat field `d` against Entity's method d(double,
    // double, double): unmapped names) hides the method in C++. The method is renamed in every class, so its overrides and every call
    // of it, whatever the receiver's class, keep agreeing.
    NameSet inherited_clash;
    for (const ir::JClass& c : classes) {
        NameSet fields;
        for (const ir::Field& f : c.fields) fields.insert(f.name);
        if (fields.empty()) continue;
        std::set<std::string, std::less<>> seen;
        auto it = c.parent ? by_name.find(*c.parent) : by_name.end();
        while (it != by_name.end() && seen.insert(it->second->name).second) {
            for (const ir::Method& m : it->second->methods)
                if (!m.is_ctor && fields.contains(m.name)) inherited_clash.insert(m.name);
            it = it->second->parent ? by_name.find(*it->second->parent) : by_name.end();
        }
    }
    if (!inherited_clash.empty())
        for (auto& [cls, names] : own) names.insert(inherited_clash.begin(), inherited_clash.end());
    // A STATIC method with the SAME NAME AND SIGNATURE as an inherited INSTANCE method is C4526 (the static cannot override the
    // base virtual): BlockFlowing's private static flow(World,int,int,int) against BlockFluid's instance flow(World,int,int,int).
    // Only the signature matches count (a static helper with another signature legally coexists), and only when the class has no
    // instance method of the name.
    const auto signature = [](const ir::Method& m) {
        std::vector<std::string> types;
        for (const ir::Param& p : m.params) types.push_back(p.type.name);
        return std::pair{m.name, std::move(types)};
    };
    for (const ir::JClass& c : classes) {
        NameSet instance_names;
        for (const ir::Method& m : c.methods) {
            if (!m.is_ctor && !m.is_static) instance_names.insert(m.name);
        }
        std::vector<const ir::Method*> statics;
        for (const ir::Method& m : c.methods) {
            if (!m.is_ctor && m.is_static && !instance_names.contains(m.name)) statics.push_back(&m);
        }
        if (statics.empty()) continue;
        std::set<std::pair<std::string, std::vector<std::string>>> inherited;
        std::set<std::string, std::less<>> guard;
        const auto start = c.parent ? by_name.find(*c.parent) : by_name.end();
        const ir::JClass* cur = start == by_name.end() ? nullptr : start->second;
        while (cur != nullptr && !guard.contains(cur->name)) {
            guard.insert(cur->name);
            for (const ir::Method& m : cur->methods) {
                if (!m.is_ctor && !m.is_static) inherited.insert(signature(m));
            }
            const auto next = cur->parent ? by_name.find(*cur->parent) : by_name.end();
            cur = next == by_name.end() ? nullptr : next->second;
        }
        for (const ir::Method* m : statics) {
            if (inherited.contains(signature(*m))) own[c.name].insert(m->name);
        }
    }
    // flattened over the parent chain, so a subclass call to an inherited colliding method is renamed too
    std::map<std::string, NameSet, std::less<>> renames;
    for (const ir::JClass& c : classes) {
        NameSet all;
        std::set<std::string, std::less<>> guard;
        const ir::JClass* cur = &c;
        while (cur != nullptr && !guard.contains(cur->name)) {
            guard.insert(cur->name);
            if (const auto it = own.find(cur->name); it != own.end()) all.insert(it->second.begin(), it->second.end());
            const auto next = cur->parent ? by_name.find(*cur->parent) : by_name.end();
            cur = next == by_name.end() ? nullptr : next->second;
        }
        renames[c.name] = std::move(all);
    }
    return renames;
}

NameSet compute_thread_like(std::span<const ir::JClass> classes) {
    const auto by_name = cor_index(classes);
    NameSet thread_like;
    const auto is_root = [](const ir::JClass& c) {
        const auto has = [&c](std::string_view n) { return std::ranges::find(c.interfaces, n) != c.interfaces.end(); };
        return (c.parent && (*c.parent == "Thread" || *c.parent == "Runnable")) || has("Runnable") || has("Thread");
    };
    for (const ir::JClass& cls : classes) {
        std::set<std::string, std::less<>> guard;
        const ir::JClass* cur = &cls;
        while (cur != nullptr && !guard.contains(cur->name)) {
            guard.insert(cur->name);
            if (is_root(*cur)) {
                thread_like.insert(cls.name);
                break;
            }
            const auto next = cur->parent ? by_name.find(*cur->parent) : by_name.end();
            cur = next == by_name.end() ? nullptr : next->second;
        }
    }
    return thread_like;
}

// ---------------------------------------------------------------------------------------------- collection out-params

std::vector<CallArg> call_args(std::string_view body, std::string_view name, std::size_t arity) {
    std::vector<CallArg> out;
    for (std::size_t at = body.find(name); at != std::string_view::npos; at = body.find(name, at + 1)) {
        if (at > 0 && cor_is_word(body[at - 1])) continue;                          // `\b` before the name
        const std::size_t paren = cor_skip_space(body, at + name.size());
        if (paren >= body.size() || body[paren] != '(') continue;
        std::size_t depth = 0;
        std::vector<std::string> args;
        std::string cur;
        for (std::size_t k = paren; k < body.size();) {
            const char ch = body[k];
            if (ch == '(') {
                ++depth;
                if (depth == 1) { ++k; continue; }
            } else if (ch == ')') {
                --depth;
                if (depth == 0) {
                    args.push_back(cur);
                    break;
                }
            }
            if (depth == 1 && ch == ',') {
                args.push_back(cur);
                cur.clear();
                ++k;
                continue;
            }
            if (depth >= 1) cur += ch;
            ++k;
        }
        for (std::string& a : args) a = std::string(text::trim(a));
        if (args.size() == 1 && args[0].empty()) args.clear();   // a call with no arguments
        if (args.size() == arity) {
            for (std::size_t i = 0; i < args.size(); ++i) out.push_back({i, args[i]});
        }
    }
    return out;
}

namespace {

// Java collection mutators. Calling one of these ON a collection PARAM mutates the CALLER's collection (Java passes the
// reference), so that parameter is an out or in-out parameter.
const NameSet& cor_collection_mutators() {
    static const NameSet names = {"add",           "addAll",       "addElement",     "addFirst",          "addLast",
                                  "insertElementAt", "set",        "setElementAt",   "replaceAll",        "sort",
                                  "remove",        "removeAll",    "removeElement",  "removeElementAt",   "removeAllElements",
                                  "removeFirst",   "removeLast",   "retainAll",      "clear",             "put",
                                  "putAll",        "putIfAbsent",  "merge",          "replace",           "push",
                                  "pop",           "offer",        "poll"};
    return names;
}

// `name.mutator(` in the body, the name standing alone
bool cor_mutates(std::string_view body, std::string_view name) {
    for (std::size_t at = body.find(name); at != std::string_view::npos; at = body.find(name, at + 1)) {
        if (at > 0 && cor_is_word(body[at - 1])) continue;
        std::size_t i = at + name.size();
        if (i >= body.size() || body[i] != '.') continue;
        i = cor_skip_space(body, i + 1);
        std::size_t j = i;
        while (j < body.size() && cor_is_word(body[j])) ++j;
        if (!cor_collection_mutators().contains(body.substr(i, j - i))) continue;
        j = cor_skip_space(body, j);
        if (j < body.size() && body[j] == '(') return true;
    }
    return false;
}

struct CorSlot {
    const ir::JClass* cls = nullptr;
    ir::Method* method = nullptr;
    std::string body;
    std::set<std::size_t> out;   // the parameter positions found to be out parameters
};

bool cor_is_collection_param(const ir::Param& p) { return !p.type.is_array() && typemap::containers().contains(p.type.name); }

}  // namespace

std::size_t compute_collection_out_params(std::vector<ir::JClass>& classes) {
    std::vector<CorSlot> slots;
    for (ir::JClass& c : classes) {
        for (ir::Method& m : c.methods) slots.push_back({&c, &m, cor_body_text(m), {}});
    }
    // inheritance closure (parent chain + interfaces) relates the members of an override family
    const auto by_name = cor_index(classes);
    std::map<std::string, NameSet, std::less<>> ancestors;
    for (const ir::JClass& c : classes) {
        NameSet seen;
        std::vector<std::string> stack = {c.name};
        while (!stack.empty()) {
            const std::string name = std::move(stack.back());
            stack.pop_back();
            const auto it = by_name.find(name);
            if (it == by_name.end()) continue;
            std::vector<std::string> parents(it->second->interfaces.begin(), it->second->interfaces.end());
            if (it->second->parent) parents.insert(parents.begin(), *it->second->parent);
            for (const std::string& p : parents) {
                if (seen.insert(p).second) stack.push_back(p);
            }
        }
        ancestors[c.name] = std::move(seen);
    }
    const auto related = [&ancestors](const std::string& a, const std::string& b) {
        const auto contains = [&ancestors](const std::string& of, const std::string& n) {
            const auto it = ancestors.find(of);
            return it != ancestors.end() && it->second.contains(n);
        };
        return a == b || contains(a, b) || contains(b, a);
    };

    // (1) seed: a direct mutation of a collection parameter in the body
    for (CorSlot& s : slots) {
        for (std::size_t i = 0; i < s.method->params.size(); ++i) {
            if (cor_is_collection_param(s.method->params[i]) && cor_mutates(s.body, s.method->params[i].name)) s.out.insert(i);
        }
    }
    std::map<std::pair<std::string, std::size_t>, std::vector<std::size_t>> by_key;   // (name, arity) -> slots
    for (std::size_t k = 0; k < slots.size(); ++k) by_key[{slots[k].method->name, slots[k].method->arity()}].push_back(k);

    // (2) and (3): propagate to a fixpoint
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& [key, group] : by_key) {   // the override family: one member out at i makes all of them out
            if (group.size() < 2) continue;
            for (std::size_t i = 0; i < key.second; ++i) {
                std::vector<std::size_t> holders;
                for (const std::size_t k : group) {
                    if (i < slots[k].method->params.size() && cor_is_collection_param(slots[k].method->params[i])) holders.push_back(k);
                }
                if (holders.size() < 2) continue;
                std::vector<std::size_t> hot;
                for (const std::size_t k : holders) {
                    if (slots[k].out.contains(i)) hot.push_back(k);
                }
                for (const std::size_t h : hot) {
                    for (const std::size_t k : holders) {
                        if (related(slots[h].cls->name, slots[k].cls->name) && slots[k].out.insert(i).second) changed = true;
                    }
                }
            }
        }
        // forwarding: a parameter passed into a callee's out slot
        std::map<std::pair<std::string, std::size_t>, std::set<std::size_t>> name_out;   // (name, arity) -> positions, unioned
        for (const CorSlot& s : slots) {
            for (const std::size_t i : s.out) name_out[{s.method->name, s.method->arity()}].insert(i);
        }
        for (CorSlot& s : slots) {
            std::map<std::string, std::size_t, std::less<>> param_at;   // the collection parameters not yet out
            for (std::size_t i = 0; i < s.method->params.size(); ++i) {
                if (cor_is_collection_param(s.method->params[i]) && !s.out.contains(i)) param_at[s.method->params[i].name] = i;
            }
            if (param_at.empty()) continue;
            for (const auto& [key, positions] : name_out) {
                for (const CallArg& arg : call_args(s.body, key.first, key.second)) {
                    const auto p = param_at.find(arg.text);
                    if (positions.contains(arg.index) && p != param_at.end() && s.out.insert(p->second).second) changed = true;
                }
            }
        }
    }

    // finalise: the parameters' by_ref flags
    std::size_t count = 0;
    for (CorSlot& s : slots) {
        for (const std::size_t i : s.out) {
            if (i < s.method->params.size() && !s.method->params[i].by_ref) {
                s.method->params[i].by_ref = true;
                ++count;
            }
        }
    }
    return count;
}

// ---------------------------------------------------------------------------------------------- nullability, escapes, resolver

Nullability derive_nullability(std::span<const ir::JClass> classes, const types::TypeResolver& resolver) {
    std::map<std::string, std::set<std::pair<std::string, bool>>, std::less<>> return_types_by_name;
    for (const ir::JClass& c : classes) {
        for (const ir::Method& m : c.methods) {
            if (!m.is_ctor && m.return_type) return_types_by_name[m.name].insert({m.return_type->name, m.return_type->is_array()});
        }
    }
    Nullability out;
    for (const ir::JClass& c : classes) {
        for (const ir::Method& m : c.methods) {
            if (m.is_ctor || !m.return_type || m.return_type->is_array()) continue;
            const ir::JType& rt = *m.return_type;
            // a value return cannot form getter()->deref
            if (typemap::primitives().contains(rt.name) || rt.name == "String" || typemap::containers().contains(rt.name)) continue;
            if (!resolver.is_pointer(rt)) continue;
            if (return_types_by_name[m.name].size() > 1) continue;   // ambiguous across classes: do not guard globally
            if (cor_returns_null(cor_body_text(m))) {
                out.getters.insert(m.name);
                out.return_types.try_emplace(m.name, rt.name);
            }
        }
    }
    return out;
}

NameSet compute_this_escapes(std::span<const ir::JClass> classes) {
    const auto by_name = cor_index(classes);
    NameSet own;
    for (const ir::JClass& c : classes) {
        bool escapes = false;
        for (const ir::Method* k : c.constructors()) {
            for (const ir::SourceLine& line : k->body) escapes = escapes || cor_uses_this_as_value(line.text);
        }
        for (const ir::Field* f : c.instance_fields()) escapes = escapes || (f->initializer && cor_uses_this_as_value(*f->initializer));
        for (const ir::Body& block : c.inst_inits) {
            for (const ir::SourceLine& line : block) escapes = escapes || cor_uses_this_as_value(line.text);
        }
        if (escapes) own.insert(c.name);
        // a Thread is kept by its running thread: `new ThreadSleepForever(..)` starts itself in its constructor, so deleting it
        // frees a live thread
        if ((c.parent && (*c.parent == "Thread" || *c.parent == "TimerTask")) || std::ranges::find(c.interfaces, "Runnable") != c.interfaces.end()) {
            own.insert(c.name);
        }
    }
    NameSet out;
    for (const ir::JClass& c : classes) {
        const ir::JClass* k = &c;
        std::set<std::string, std::less<>> guard;   // (a cyclic parent chain would not end)
        while (k != nullptr && guard.insert(k->name).second) {
            if (own.contains(k->name)) {
                out.insert(c.name);
                break;
            }
            const auto next = k->parent ? by_name.find(*k->parent) : by_name.end();
            k = next == by_name.end() ? nullptr : next->second;
        }
    }
    return out;
}

types::TypeResolver build_resolver(std::span<const ir::JClass> classes, const ResolverInputs& inputs) {
    types::ResolverData data = inputs.profile != nullptr ? types::ResolverData::for_profile(*inputs.profile) : types::ResolverData{};
    if (inputs.value_types) data.values = *inputs.value_types;
    for (const ir::JClass& c : classes) {
        data.known.insert(c.name);
        if (c.is_enum) {
            data.enums.insert(c.name);
            if (c.is_typed_enum()) data.typed_enums.insert(c.name);
        }
        if (c.is_interface) data.interfaces.insert(c.name);
    }
    if (inputs.runtime != nullptr) data.members = build_member_env(classes, *inputs.runtime);
    data.method_renames = build_method_renames(classes);
    data.thread_like = compute_thread_like(classes);
    data.markers = inputs.markers;
    types::TypeResolver resolver(std::move(data));
    // The whole-program no-GC caller-frees catalog for the source emitter: owned containers (Level.getCubes -> new AABBs) get an
    // RAII element-deleter at their call sites; owned scalar returns used inline as arguments (getCubes(bb.expand(...))) get hoisted
    // and deleted, but only where the receiving method provably just reads the argument (retained_params and corpus_methods gate it).
    types::OwnershipCatalog catalog;
    catalog.return_methods = emit::ownership::owned_return_container_methods(classes, resolver);
    catalog.scalar_methods = emit::ownership::owned_scalar_return_methods(classes, resolver);
    catalog.retained_params = emit::ownership::retained_param_positions(classes);
    for (const ir::JClass& c : classes) {
        for (const ir::Method& m : c.methods) {
            if (!m.is_ctor) catalog.corpus_methods.insert(m.name);
        }
    }
    resolver.data().own_catalog = std::move(catalog);
    return resolver;
}

}  // namespace cb::java::corpus
