// cb-unity: alone (the scanning helpers are named for what they do here)
#include "java/emit/ownership.hpp"

#include <algorithm>

#include "core/regex.hpp"
#include "core/text.hpp"
#include "java/typemap.hpp"

namespace cb::java::emit::ownership {

namespace {

// -- scanning: the method bodies are raw Java text, so these are small word-level scans -----------------------------

bool word_char(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }

std::size_t skip_space(std::string_view s, std::size_t i) {
    while (i < s.size() && text::is_space(s[i])) ++i;
    return i;
}

// The start of each place `word` stands as a whole word (not inside a longer identifier), from `from` on.
std::size_t find_word(std::string_view s, std::string_view word, std::size_t from) {
    for (std::size_t i = s.find(word, from); i != std::string_view::npos; i = s.find(word, i + 1)) {
        const bool left_ok = i == 0 || !word_char(s[i - 1]);
        const std::size_t end = i + word.size();
        const bool right_ok = end >= s.size() || !word_char(s[end]);
        if (left_ok && right_ok) return i;
    }
    return std::string_view::npos;
}

// `null` or `NULL` at s[i], ending at a word boundary.
bool null_word_at(std::string_view s, std::size_t i) {
    for (const std::string_view w : {std::string_view("null"), std::string_view("NULL")}) {
        if (s.substr(i).starts_with(w) && (i + w.size() >= s.size() || !word_char(s[i + w.size()]))) return true;
    }
    return false;
}

bool is_null_text(std::string_view s) { return s == "null" || s == "NULL" || s == "nullptr" || s == "0"; }

// A method's body as one string (the raw lines joined by newlines).
std::string joined_body(const ir::Method& method) {
    std::string out;
    for (std::size_t i = 0; i < method.body.size(); ++i) {
        if (i) out += '\n';
        out += method.body[i].text;
    }
    return out;
}

// The body with each `// ...` comment removed (the form every ownership scan wants).
std::string body_text(const ir::Method& method) {
    std::string text = joined_body(method);
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size();) {
        if (text.compare(i, 2, "//") == 0) {
            const std::size_t nl = text.find('\n', i);
            if (nl == std::string::npos) break;
            i = nl;   // keep the newline
        } else {
            out += text[i++];
        }
    }
    return out;
}

// -- the allocation shapes of a right-hand side ----------------------------------------------------------------------

struct NewShape {
    bool matched = false;
    bool is_array = false;
};

// `new T` / `new T[`: a fresh allocation.
NewShape new_shape(std::string_view rhs) {
    static const Regex rhs_new(R"(^new\s+[A-Za-z_][\w:.]*\s*(\[)?)");
    const std::optional<RegexMatch> m = rhs_new.match(rhs);
    if (!m) return {};
    return {true, m->has(1)};
}

// `X::createY(` / `X.makeY(` ...: an owning factory.
bool is_factory_call(std::string_view rhs) {
    static const Regex rhs_factory(R"(^(?:[A-Za-z_]\w*\s*(?:::|\.)\s*)+(?:create|make|alloc|clone|copyOf)[A-Za-z]\w*\s*\()");
    return rhs_factory.match(rhs).has_value();
}

bool is_alloc_rhs(std::string_view rhs) { return new_shape(rhs).matched || is_factory_call(rhs); }

// -- the statements of a body ----------------------------------------------------------------------------------------

struct Assign {
    std::string_view target;
    std::string_view value;
};

const Regex& assign_statement() {
    static const Regex re(R"((?:this\s*(?:->|\.)\s*)?([A-Za-z_]\w*)\s*=\s*([^;]+);)");
    return re;
}
// `c.push_back(EXPR)` / `c.add(EXPR)`: a container element add
const Regex& add_call() {
    static const Regex re(R"(([A-Za-z_]\w*)\s*(?:->|\.)\s*(?:push_back|add|addElement|append)\s*\(\s*(.*?)\)\s*;)");
    return re;
}
// `c[i] = EXPR`
const Regex& element_set() {
    static const Regex re(R"(([A-Za-z_]\w*)\s*\[[^\]]*\]\s*=\s*([^;]+);)");
    return re;
}

}  // namespace

// -- parameters --------------------------------------------------------------------------------------------------------

NameSet nullable_value_params(const ir::Method& method, const types::TypeResolver& resolver) {
    const std::string body = joined_body(method);
    NameSet out;
    for (const ir::Param& p : method.params) {
        const ir::JType& jt = p.type;
        // (an Object is a JObject, which is null itself and compares with NULL: it stays as it is, as its overrides have it -- NBTBase.equals
        // made `JObject*` while NBTTagCompound's override took `JObject`, and `instanceof` on the pointer did not compile, Beta 1.9-pre5)
        if (jt.is_array() || resolver.is_pointer(jt) || typemap::primitives().contains(jt.name) || jt.name == "Object" ||
            typemap::value_stdlib().contains(jt.name) || typemap::containers().contains(jt.name)) {
            continue;
        }
        // `p == null`, `p != null`, `null == p`, `null != p`, `p = null`
        for (std::size_t at = find_word(body, p.name, 0); at != std::string::npos; at = find_word(body, p.name, at + 1)) {
            std::size_t after = skip_space(body, at + p.name.size());
            const std::string_view rest = std::string_view(body).substr(after);
            bool nullable = false;
            if (rest.starts_with("==") || rest.starts_with("!=")) {
                nullable = null_word_at(body, skip_space(body, after + 2));
            } else if (rest.starts_with("=")) {
                nullable = null_word_at(body, skip_space(body, after + 1));
            }
            if (!nullable) {   // null on the left: `null == p`
                std::size_t q = at;
                while (q > 0 && text::is_space(body[q - 1])) --q;
                if (q >= 2 && (body.compare(q - 2, 2, "==") == 0 || body.compare(q - 2, 2, "!=") == 0)) {
                    q -= 2;
                    while (q > 0 && text::is_space(body[q - 1])) --q;
                    constexpr std::size_t kNullLen = 4;
                    nullable = q >= kNullLen && (q == kNullLen || !word_char(body[q - kNullLen - 1])) &&
                               (body.compare(q - kNullLen, kNullLen, "null") == 0 || body.compare(q - kNullLen, kNullLen, "NULL") == 0);
                }
            }
            if (nullable) {
                out.insert(p.name);
                break;
            }
        }
    }
    return out;
}

NameSet reassigned_params(const ir::Method& method) {
    const std::string body = joined_body(method);
    NameSet out;
    for (const ir::Param& p : method.params) {
        for (std::size_t at = find_word(body, p.name, 0); at != std::string::npos; at = find_word(body, p.name, at + 1)) {
            if (at > 0 && body[at - 1] == '.') continue;   // a member of something else, not the parameter
            const std::size_t after = skip_space(body, at + p.name.size());
            const std::string_view rest = std::string_view(body).substr(after);
            bool assigned = rest.starts_with("++") || rest.starts_with("--");
            // `=` or a compound `op=`, but not `==`
            std::size_t eq = after;
            if (!assigned && !rest.empty() && std::string_view("-+*/%|&^").find(rest[0]) != std::string_view::npos && rest.size() > 1 && rest[1] == '=') eq = after + 1;
            if (!assigned && eq < body.size() && body[eq] == '=' && !(eq + 1 < body.size() && body[eq + 1] == '=')) assigned = true;
            if (assigned) {
                out.insert(p.name);
                break;
            }
        }
    }
    return out;
}

std::string params_str(const ir::Method& method, const types::TypeResolver& resolver) {
    const NameSet nullable = nullable_value_params(method, resolver);
    const NameSet reassigned = reassigned_params(method);
    std::vector<std::string> parts;
    for (const ir::Param& p : method.params) {
        std::string t = resolver.param_type(p.type);
        if (nullable.contains(p.name) && !t.ends_with('*')) {
            t += '*';
        } else if (reassigned.contains(p.name) && t.starts_with("const ") && t.ends_with('&')) {
            // const X& -> X: a reassigned param is by-value and mutable
            t = std::string(text::trim(std::string_view(t).substr(6, t.size() - 6 - 1)));
        }
        // Java collection out-param (the pipeline's compute_collection_out_params): pass the value-struct container by
        // reference so the callee's add()/remove()/put() reach the caller's collection.
        if (p.by_ref && !t.ends_with('*') && !t.ends_with('&')) t += '&';
        parts.push_back(t + " " + p.name);
    }
    return text::join(parts, ", ");
}

std::optional<std::string> aggregate_array_body(const ir::Field& field) {
    static const Regex agg_array(R"(^new\s+[\w.]+\s*\[\s*\]\s*\{(.*)\}\s*$)", RegexFlags::DotAll);
    if (!field.initializer) return std::nullopt;
    const std::string_view init = text::trim(*field.initializer);
    const std::optional<RegexMatch> m = agg_array.match(init);
    if (!m) return std::nullopt;
    return std::string(m->group(1));
}

// -- what a class frees ------------------------------------------------------------------------------------------------

OrderedMap<std::string, bool> owned_pointers(const ir::JClass& cls, const types::TypeResolver& resolver) {
    OrderedMap<std::string, bool> alloc;
    std::set<std::string, std::less<>> borrowed;

    auto classify = [&](std::string_view name, std::string_view rhs) {
        const NewShape shape = new_shape(rhs);
        if (shape.matched) {
            bool& slot = alloc[std::string(name)];   // inserted false on first sight: an array allocation sticks
            slot = shape.is_array || slot;
        } else if (is_factory_call(rhs)) {
            alloc.try_emplace(std::string(name), false);
        } else if (is_null_text(rhs)) {
            // `= NULL`: neutral, delete of NULL is safe
        } else {
            borrowed.emplace(name);   // a param, an alias, an unknown getter: borrowed
        }
    };

    // Field-declaration initializers (`Timer timer = new Timer(60F);`, `= BufferUtils.createIntBuffer(16);`): the emitter
    // moves these into the constructor, but they live on the field, NOT in any scanned method body.
    for (const ir::Field& f : cls.fields) {
        if (!f.is_static && f.initializer && !f.initializer->empty() && resolver.is_pointer(f.type)) {
            classify(f.name, text::trim(*f.initializer));
        }
    }
    for (const ir::Method& m : cls.methods) {
        const std::string code = body_text(m);
        assign_statement().for_each_match(code, [&](const RegexMatch& am) {
            const ir::Field* fld = cls.field(am.group(1));
            if (fld != nullptr && !fld->is_static && resolver.is_pointer(fld->type)) classify(am.group(1), text::trim(am.group(2)));
            return true;
        });
    }
    OrderedMap<std::string, bool> out;
    for (const auto& [name, is_array] : alloc) {
        if (!borrowed.contains(name)) out.insert_or_assign(name, is_array);
    }
    return out;
}

OrderedMap<std::string, std::string> owned_containers(const ir::JClass& cls, const types::TypeResolver& resolver) {
    std::vector<std::string> allocated_in_order;   // first seen first: the output follows the source, not a hash
    std::set<std::string, std::less<>> add_alloc, add_borrow;
    for (const ir::Method& m : cls.methods) {
        const std::string code = body_text(m);
        for (const Regex* pat : {&add_call(), &element_set()}) {   // `c.add(x)` / `c[i] = x`: owned iff x is a fresh allocation
            pat->for_each_match(code, [&](const RegexMatch& hit) {
                const std::string name(hit.group(1));
                if (is_alloc_rhs(text::trim(hit.group(2)))) {
                    if (add_alloc.insert(name).second) allocated_in_order.push_back(name);
                } else {
                    add_borrow.insert(name);
                }
                return true;
            });
        }
    }
    OrderedMap<std::string, std::string> out;
    for (const std::string& name : allocated_in_order) {
        if (add_borrow.contains(name)) continue;
        const ir::Field* fld = cls.field(name);
        if (fld == nullptr || fld->is_static) continue;
        const ir::JType& jt = fld->type;
        if (jt.is_array() && resolver.is_pointer(jt.element())) {
            out.insert_or_assign(name, ".length");
        } else if (typemap::containers().contains(jt.name) && !jt.args.empty() && resolver.is_pointer(jt.args[0])) {
            out.insert_or_assign(name, ".size()");
        }
    }
    return out;
}

bool owns_heap(const ir::JClass& cls, const types::TypeResolver& resolver) {
    return !owned_pointers(cls, resolver).empty() || !owned_containers(cls, resolver).empty();
}

// -- what a method hands back ------------------------------------------------------------------------------------------

namespace {

// Pre-generics Minecraft returns RAW containers (`ArrayList`, no <T>), so the element type is NOT on the return
// signature -- it comes from the `new T` adds and the caller's `JList<T*>` declaration. So the gate is just "returns a
// container type"; ownership is proven by every add being a fresh allocation.
bool returns_container(const std::optional<ir::JType>& rt) {
    if (!rt || rt->name == "void") return false;
    return typemap::containers().contains(rt->name) || rt->is_array();
}

// The return is a single reference/pointer object -- not void, an array, or a container.
bool returns_scalar_pointer(const ir::Method& method, const types::TypeResolver& resolver) {
    const std::optional<ir::JType>& rt = method.return_type;
    if (!rt || rt->name == "void" || rt->is_array() || typemap::containers().contains(rt->name)) return false;
    return resolver.is_pointer(*rt) || returns_pointer(method, resolver);
}

// A param is RETAINED unless every occurrence in the body is a pure read (`name.` or `name[`). Anything else -- stored,
// returned, passed on, compared -- counts as retained, so only an argument the callee provably just reads is freed.
bool param_retained(std::string_view name, std::string_view body) {
    for (std::size_t at = find_word(body, name, 0); at != std::string::npos; at = find_word(body, name, at + 1)) {
        const std::size_t next = skip_space(body, at + name.size());
        if (next >= body.size() || (body[next] != '.' && body[next] != '[')) return true;
    }
    return false;
}

// Whether the body has `return ... new NAME` inside one statement (a return, incl. a ternary, that allocates the type).
bool returns_new_of(std::string_view body, std::string_view type_name) {
    for (std::size_t r = find_word(body, "return", 0); r != std::string::npos; r = find_word(body, "return", r + 1)) {
        const std::size_t begin = r + 6;
        const std::size_t semi = body.find(';', begin);
        const std::string_view stmt = body.substr(begin, semi == std::string::npos ? std::string::npos : semi - begin);
        for (std::size_t n = find_word(stmt, "new", 0); n != std::string::npos; n = find_word(stmt, "new", n + 1)) {
            const std::size_t after = n + 3;
            if (after >= stmt.size() || !text::is_space(stmt[after])) continue;
            const std::size_t name_at = skip_space(stmt, after);
            if (stmt.substr(name_at).starts_with(type_name) &&
                (name_at + type_name.size() >= stmt.size() || !word_char(stmt[name_at + type_name.size()]))) {
                return true;
            }
        }
    }
    return false;
}

bool returns_null(std::string_view body) {
    for (std::size_t r = find_word(body, "return", 0); r != std::string::npos; r = find_word(body, "return", r + 1)) {
        const std::size_t at = r + 6;
        if (at < body.size() && text::is_space(body[at]) && null_word_at(body, skip_space(body, at))) return true;
    }
    return false;
}

}  // namespace

bool returns_owned_container(const ir::Method& method, const types::TypeResolver&) {
    if (!returns_container(method.return_type)) return false;
    static const Regex returned_var(R"(\breturn\s+([A-Za-z_]\w*)\s*;)");
    // a freshly constructed local container: `var = new ArrayList()` or generic `new ArrayList<T>()`
    static const Regex fresh_container(R"(\b([A-Za-z_]\w*)\s*=\s*new\s+[A-Za-z_][\w.]*\s*(?:<[^>]*>)?\s*\()");
    static const Regex element_add(R"(\b([A-Za-z_]\w*)\s*\.\s*(?:add|addElement|push_back|append)\s*\(\s*(.*?)\)\s*;)");
    const std::string code = body_text(method);

    std::set<std::string, std::less<>> returned, fresh;
    returned_var.for_each_match(code, [&](const RegexMatch& m) { returned.emplace(m.group(1)); return true; });
    if (returned.empty()) return false;
    fresh_container.for_each_match(code, [&](const RegexMatch& m) { fresh.emplace(m.group(1)); return true; });
    std::map<std::string, std::vector<std::string>, std::less<>> adds;   // var -> the stripped element expressions
    element_add.for_each_match(code, [&](const RegexMatch& m) {
        adds[std::string(m.group(1))].emplace_back(text::trim(m.group(2)));
        return true;
    });
    for (const std::string& var : returned) {
        if (!fresh.contains(var)) return false;
        const auto it = adds.find(var);
        if (it == adds.end() || it->second.empty()) return false;   // no adds
        for (const std::string& a : it->second) {
            if (!is_alloc_rhs(a)) return false;                      // a borrowed element added
        }
    }
    return true;
}

NameSet owned_return_container_methods(std::span<const ir::JClass> classes, const types::TypeResolver& resolver) {
    NameSet owned, ambiguous;
    for (const ir::JClass& c : classes) {
        for (const ir::Method& m : c.methods) {
            if (m.is_ctor || !returns_container(m.return_type)) continue;
            (returns_owned_container(m, resolver) ? owned : ambiguous).insert(m.name);
        }
    }
    NameSet out;
    for (const std::string& name : owned) {
        if (!ambiguous.contains(name)) out.insert(name);
    }
    return out;
}

bool returns_owned_scalar(const ir::Method& method, const types::TypeResolver& resolver) {
    if (!returns_scalar_pointer(method, resolver)) return false;
    static const Regex return_expr(R"(\breturn\b([^;]*);)");
    const std::string code = body_text(method);
    bool any_return = false, saw_new = false, all_owned = true;
    return_expr.for_each_match(code, [&](const RegexMatch& m) {
        any_return = true;
        const std::string_view e = text::trim(m.group(1));
        if (e.empty() || is_null_text(e)) return true;   // a bare / null / 0 return: the temp may be null, delete NULL is safe
        if (new_shape(e).matched) {
            saw_new = true;
            return true;
        }
        all_owned = false;   // returns a field, a param, something else: not caller-owned
        return false;
    });
    return any_return && all_owned && saw_new;
}

std::map<std::string, std::string, std::less<>> owned_scalar_return_methods(std::span<const ir::JClass> classes,
                                                                            const types::TypeResolver& resolver) {
    std::map<std::string, std::string, std::less<>> owned;
    NameSet ambiguous;
    for (const ir::JClass& c : classes) {
        for (const ir::Method& m : c.methods) {
            if (m.is_ctor || !returns_scalar_pointer(m, resolver)) continue;
            if (returns_owned_scalar(m, resolver)) {
                std::string elem = return_cpp_type(m, resolver);
                while (elem.ends_with('*')) elem.pop_back();
                elem = std::string(text::trim(elem));
                const auto it = owned.find(m.name);
                if (it != owned.end() && it->second != elem) ambiguous.insert(m.name);
                owned[m.name] = elem;
            } else {
                ambiguous.insert(m.name);
            }
        }
    }
    std::map<std::string, std::string, std::less<>> out;
    for (const auto& [name, elem] : owned) {
        if (!ambiguous.contains(name)) out.emplace(name, elem);
    }
    return out;
}

std::map<std::string, std::set<std::size_t>, std::less<>> retained_param_positions(std::span<const ir::JClass> classes) {
    std::map<std::string, std::set<std::size_t>, std::less<>> retained;
    for (const ir::JClass& c : classes) {
        for (const ir::Method& m : c.methods) {
            if (m.is_ctor) continue;
            std::string code = body_text(m);
            // drop the signature line: the param name in `f(T p)` is a declaration, not a retention use
            const std::size_t brace = code.find('{');
            const std::string_view body = brace == std::string::npos ? std::string_view(code) : std::string_view(code).substr(brace + 1);
            for (std::size_t i = 0; i < m.params.size(); ++i) {
                if (param_retained(m.params[i].name, body)) retained[m.name].insert(i);
            }
        }
    }
    return retained;
}

bool returns_pointer(const ir::Method& method, const types::TypeResolver& resolver) {
    const std::optional<ir::JType>& rt = method.return_type;
    if (!rt || rt->name == "void") return false;
    // Arrays are by-value JArray<T> (their own null handling: `return null` -> an empty JArray). Never promote to a
    // pointer, or `return theField;` (a value JArray) cannot convert.
    if (rt->is_array()) return false;
    // Collections are by-value (JList/JMap/JSet) with their own null handling exactly like arrays. NEVER promote a
    // container return to a pointer: a sibling method, local or field of the SAME interface type is a value, and a
    // promoted pointer return cannot interconvert with it (the StatsSyncher/StatFileWriter JMap*<->JMap collisions).
    if (typemap::containers().contains(rt->name)) return false;
    if (resolver.is_pointer(*rt)) return true;
    // std::string (and other value-stdlib types) is NEVER a pointer: it has dedicated by-value handling (return by
    // value, `return null` -> an empty string). Promoting it reintroduces the dangling-pointer risk and breaks
    // virtual-override return unification (base `return null` -> string*, derived `return "x"` -> string).
    if (typemap::value_stdlib().contains(rt->name)) return false;
    // A value-type return is promoted to a pointer if the body new's it in a return (a ternary too) OR returns null --
    // both impossible for a by-value return.
    const std::string code = joined_body(method);
    return returns_new_of(code, rt->name) || returns_null(code);
}

std::string return_cpp_type(const ir::Method& method, const types::TypeResolver& resolver) {
    if (!method.return_type || method.return_type->name == "void") return "void";
    std::string rt = resolver.return_type(*method.return_type);
    if (returns_pointer(method, resolver) && !rt.ends_with('*') && !rt.ends_with('&')) rt += '*';
    return rt;
}

}  // namespace cb::java::emit::ownership
