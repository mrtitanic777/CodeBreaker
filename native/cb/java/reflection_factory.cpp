#include "cb/java/reflection_factory.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::reflection_factory {

namespace {

namespace stdfs = std::filesystem;

// a receiver lvalue: var, var->field, this->field, var[i], var[i]->field, ...
constexpr const char* kRecv = R"RX(((?:this\s*->\s*)?[A-Za-z_]\w*(?:\s*->\s*[A-Za-z_]\w*|\s*\[[^\]]*\])*))RX";

// A reflection instantiation with a ctor arg (EntityList / SpawnerAnimals / MapStorage):
//   (Entity*)recv->getConstructor((JArrayInit<Class* >(), j2xClassOf(typeid(World)))).newInstance((JArrayInit<JObject >(), var1))
// groups: 1 the cast type, 2 the receiver, 3 the `, j2xClassOf(typeid(T))` list, 4 the constructor argument(s).
const Regex& world_arg_re() {
    static const Regex re(std::string(R"RX(\(\s*([A-Za-z_]\w*)\s*\*\s*\)\s*)RX") + kRecv +
                          R"RX(\s*->\s*getConstructor\s*\(\s*\(\s*JArrayInit<[^>]*>\(\)((?:\s*,\s*(?:Class|j2xClassOf)\(typeid\(\s*\w+\s*\)\))+)\s*\)\s*\)\s*\.\s*newInstance\s*\(\s*\(\s*JArrayInit<[^>]*>\(\)\s*,\s*(.+?)\)\s*\))RX");
    return re;
}

// A plain no-arg reflection instantiation (TileEntity / Packet / BlockSign):  (TileEntity*)recv->newInstance()
// groups: 1 the cast type, 2 the receiver.
const Regex& plain_re() {
    static const Regex re(std::string(R"RX(\(\s*([A-Za-z_]\w*)\s*\*\s*\)\s*)RX") + kRecv + R"RX(\s*->\s*newInstance\s*\(\s*\))RX");
    return re;
}

// X.class -> j2xClassOf(typeid(X)) (or the older Class(typeid(X))); the dynamic getClass() form ...(typeid(*(var)))
// is excluded by \w+. The generated factory compares `*c == Class(typeid(X))` (a runtime Class value), which is
// correct whichever spelling the registration table used.
const Regex& typeid_re() {
    static const Regex re(R"RX((?:Class|j2xClassOf)\(typeid\(\s*(\w+)\s*\)\))RX");
    return re;
}

// A factory definition line left by a previous run: `Entity* EntityList_createByReflection(Class* c, World* a0)`.
const Regex& definition_re() {
    static const Regex re(R"RX(^([A-Za-z_]\w*)\*\s+(\w+_createByReflection)\s*\(([^)]*)\)\s*$)RX", RegexFlags::Multiline);
    return re;
}

bool has(std::string_view hay, std::string_view needle) { return hay.find(needle) != std::string_view::npos; }

bool is_word(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }

std::vector<std::string> typeid_names(std::string_view text) {
    std::vector<std::string> out;
    typeid_re().for_each_match(text, [&](const RegexMatch& m) {
        out.emplace_back(m[1]);
        return true;
    });
    return out;
}

// Split a comma-separated arg blob at top-level commas (respecting nested parens/angles; the `>` of `->` is not a
// closing angle).
std::vector<std::string> split_top(std::string_view s) {
    std::vector<std::string> out;
    int depth = 0;
    std::string cur;
    for (std::size_t i = 0; i < s.size(); ++i) {
        const char ch = s[i];
        if (ch == '(' || ch == '<') {
            ++depth;
        } else if (ch == ')' || (ch == '>' && !(i > 0 && s[i - 1] == '-'))) {
            --depth;
        }
        if (ch == ',' && depth == 0) {
            out.emplace_back(text::trim(cur));
            cur.clear();
        } else {
            cur += ch;
        }
    }
    if (!text::trim(cur).empty()) out.emplace_back(text::trim(cur));
    return out;
}

// Concrete classes to switch over: every typeid(X) on a registration line, minus the ctor arg types (e.g. World).
std::vector<std::string> concrete_classes(std::string_view text, const std::set<std::string>& arg_types,
                                          const Registrars& registrars) {
    std::set<std::string> seen;
    std::vector<std::string> out;
    for (std::string_view line : text::split(text, '\n')) {
        if (!registrars.matches_line(line)) continue;
        for (const std::string& x : typeid_names(line)) {
            if (arg_types.contains(x) || !seen.insert(x).second) continue;
            out.push_back(x);
        }
    }
    return out;
}

// Drop `// J2X-REVIEW[reflection-unsupported]` lines whose next code line now calls a generated factory -- the
// concern is resolved, so the greppable tag should go.
std::string strip_resolved_comments(std::string_view text) {
    const std::vector<std::string_view> lines = text::split(text, '\n');
    std::vector<std::string_view> kept;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (has(lines[i], "J2X-REVIEW[reflection-unsupported]")) {
            const std::string_view next = i + 1 < lines.size() ? lines[i + 1] : std::string_view();
            if (has(next, "_createByReflection(")) continue;
        }
        kept.push_back(lines[i]);
    }
    return text::join(kept, "\n");
}

std::string cast_args(const std::vector<std::string>& arg_types, const std::vector<std::string>& values) {
    std::string out;
    for (std::size_t i = 0; i < arg_types.size() && i < values.size(); ++i) {
        if (i != 0) out += ", ";
        out += std::format("({}*){}", arg_types[i], values[i]);
    }
    return out;
}

// Insert `block` as one more line after the last `#include` line (after line 0 if there is none).
std::string insert_after_includes(std::string_view text, const std::string& block) {
    std::vector<std::string> lines;
    for (std::string_view l : text::split(text, '\n')) lines.emplace_back(l);
    std::size_t last = 0;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (text::ltrim(lines[i]).starts_with("#include")) last = i;
    }
    lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(last + 1), block);
    return text::join(lines, "\n");
}

// Pick the factory whose ctor-arg signature matches this site and whose return type is `cast` (or a base of it --
// e.g. EntityList's Entity* factory serving an (EntityLiving*) site).
const FactoryRef* find_factory(const std::vector<FactoryRef>& factories, const std::vector<std::string>& arg_types,
                               std::string_view cast) {
    std::vector<const FactoryRef*> same;
    for (const FactoryRef& f : factories) {
        if (f.args == arg_types) same.push_back(&f);
    }
    for (const FactoryRef* f : same) {
        if (f->ret == cast || cast.starts_with(f->ret)) return f;
    }
    return same.size() == 1 ? same[0] : nullptr;   // a unique arg-sig (e.g. the sole World-arg factory)
}

}  // namespace

// ---- Registrars -----------------------------------------------------------------------------------------

bool Registrars::matches_line(std::string_view line) const {
    for (std::size_t i = 0; i < line.size();) {
        if (!is_word(line[i])) {
            ++i;
            continue;
        }
        std::size_t j = i;
        while (j < line.size() && is_word(line[j])) ++j;
        const std::string_view ident = line.substr(i, j - i);
        std::size_t k = j;
        while (k < line.size() && text::is_space(line[k])) ++k;
        if (k < line.size() && line[k] == '(') {
            if (names.empty() ? ident.starts_with("register") : std::find(names.begin(), names.end(), ident) != names.end()) {
                return true;
            }
        }
        i = j;
    }
    return false;
}

// ---- the passes -----------------------------------------------------------------------------------------

ProcessResult process(std::string_view text_in, std::string_view registry, const Registrars& registrars) {
    const std::string text(text_in);
    const auto world = world_arg_re().search(text);
    const auto plain = plain_re().search(text);
    if (!world && !plain) return {strip_resolved_comments(text), 0, std::nullopt};

    std::string ret_type;
    std::vector<std::string> arg_types;
    if (world) {   // World-arg registry (entities)
        ret_type = std::string(world->group(1));
        arg_types = typeid_names(world->group(3));   // e.g. {World}
        const std::size_t n_args = std::min(arg_types.size(), split_top(world->group(4)).size());
        arg_types.resize(n_args);
    } else {       // no-arg registry (tile entities / packets)
        ret_type = std::string(plain->group(1));
    }

    const std::vector<std::string> concrete =
        concrete_classes(text, std::set<std::string>(arg_types.begin(), arg_types.end()), registrars);
    if (concrete.empty()) return {text, 0, std::nullopt};

    // ---- generate the factory ----
    std::string params = "Class* c";
    std::string callargs;
    for (std::size_t i = 0; i < arg_types.size(); ++i) {
        params += std::format(", {}* a{}", arg_types[i], i);
        callargs += std::format("{}a{}", i == 0 ? "" : ", ", i);
    }
    const std::string fname = std::format("{}_createByReflection", registry);
    std::vector<std::string> lines = {
        "",
        "// ==== CodeBreaker-generated reflection factory (replaces newInstance/getConstructor) ====",
        "// Built from the class registration table; returns a real `new` instead of the NULL stub.",
        std::format("{}* {}({})", ret_type, fname, params),
        "{",
        "\tif (!c) return NULL;",
    };
    for (const std::string& x : concrete) lines.push_back(std::format("\tif (*c == Class(typeid({}))) return new {}({});", x, x, callargs));
    lines.insert(lines.end(), {"\treturn NULL;", "}", ""});
    const std::string factory = text::join(lines, "\n");

    // ---- rewrite the call sites ----
    std::size_t sites = 0;
    std::string rewritten = world_arg_re().replace_all(text, [&](const RegexMatch& m) {
        ++sites;
        std::vector<std::string> vals = split_top(m.group(4));
        vals.resize(std::min(vals.size(), arg_types.size()));
        const std::string casts = cast_args(arg_types, vals);
        return std::format("{}({}{})", fname, m.group(2), casts.empty() ? "" : ", " + casts);
    });
    rewritten = plain_re().replace_all(rewritten, [&](const RegexMatch& m) {
        ++sites;
        return std::format("{}({})", fname, m.group(2));
    });

    // ---- insert the factory after the include block (before the first method uses it) ----
    return {strip_resolved_comments(insert_after_includes(rewritten, factory)), sites,
            Factory{std::string(registry), ret_type, arg_types, concrete}};
}

std::pair<std::string, std::size_t> reuse(std::string_view text_in, const std::vector<FactoryRef>& factories) {
    std::size_t sites = 0;
    std::string text = world_arg_re().replace_all(text_in, [&](const RegexMatch& m) -> std::string {
        const std::string cast(m.group(1)), recv(m.group(2));
        const std::vector<std::string> arg_types = typeid_names(m.group(3));
        std::vector<std::string> vals = split_top(m.group(4));
        vals.resize(std::min(vals.size(), arg_types.size()));
        const FactoryRef* f = find_factory(factories, arg_types, cast);
        if (f == nullptr) return std::string(m.whole());
        ++sites;
        const std::string casts = cast_args(arg_types, vals);
        return std::format("({}*){}({}{})", cast, f->name, recv, casts.empty() ? "" : ", " + casts);
    });
    text = plain_re().replace_all(text, [&](const RegexMatch& m) -> std::string {
        const std::string cast(m.group(1)), recv(m.group(2));
        const FactoryRef* f = find_factory(factories, {}, cast);
        if (f == nullptr) return std::string(m.whole());
        ++sites;
        return std::format("({}*){}({})", cast, f->name, recv);
    });
    return {strip_resolved_comments(text), sites};
}

std::string header_text(const std::vector<FactoryRef>& factories) {
    std::set<std::string> forward = {"Class"};
    for (const FactoryRef& f : factories) {
        forward.insert(f.ret);
        forward.insert(f.args.begin(), f.args.end());
    }
    std::string classes;
    for (const std::string& t : forward) classes += std::format("{}class {};", classes.empty() ? "" : " ", t);
    std::vector<std::string> lines = {
        "// ReflectionFactory.h -- CodeBreaker-generated. Declares the reflection factories so any file can",
        "// construct a registered class by its Class* (replacing Java newInstance()).",
        "#ifndef J2X_REFLECTION_FACTORY_H",
        "#define J2X_REFLECTION_FACTORY_H",
        "",
        classes,
    };
    std::set<std::string> seen;
    for (const FactoryRef& f : factories) {
        std::string params = "Class* c";
        for (std::size_t i = 0; i < f.args.size(); ++i) params += std::format(", {}* a{}", f.args[i], i);
        std::string decl = std::format("{}* {}({});", f.ret, f.name, params);
        if (seen.insert(decl).second) lines.push_back(std::move(decl));
    }
    lines.insert(lines.end(), {"", "#endif // J2X_REFLECTION_FACTORY_H", ""});
    return text::join(lines, "\n");
}

RunResult run(const stdfs::path& out_dir, const Registrars& registrars) {
    std::error_code ec;
    if (!stdfs::is_directory(out_dir, ec)) throw NotFoundError(out_dir, 0, "not a folder");
    std::vector<std::string> files;
    for (const auto& entry : stdfs::directory_iterator(out_dir)) {
        const std::string name = fs::to_text(entry.path().filename());
        if (entry.is_regular_file() && name.ends_with(".cpp")) files.push_back(name);
    }
    std::sort(files.begin(), files.end());

    auto read = [&](const std::string& f) { return textutil::try_read_text(out_dir / fs::from_text(f)); };
    auto write = [&](const std::string& f, const std::string& t) { fs::write_text(out_dir / fs::from_text(f), t); };

    RunResult result;
    std::vector<FactoryRef> factories;
    std::set<std::string> registry_files;

    // PASS 1: registry factories
    for (const std::string& f : files) {
        const auto text = read(f);
        if (!text || (!has(*text, "newInstance") && !has(*text, "_createByReflection("))) continue;
        ProcessResult r = process(*text, f.substr(0, f.size() - 4), registrars);
        if (r.text != *text) write(f, r.text);
        if (r.sites != 0 && r.factory) {
            factories.push_back({r.factory->registry + "_createByReflection", r.factory->ret, r.factory->args});
            result.registries.push_back({f, r.sites, *r.factory});
            registry_files.insert(f);
        }
    }

    // detect factories generated by a PRIOR run (so re-runs / cross-file reuse still work)
    std::set<std::string> known;
    for (const FactoryRef& f : factories) known.insert(f.name);
    for (const std::string& f : files) {
        const auto text = read(f);
        if (!text || !has(*text, "_createByReflection(")) continue;
        definition_re().for_each_match(*text, [&](const RegexMatch& m) {
            const std::string name(m[2]);
            if (!known.insert(name).second) return true;
            std::vector<std::string> params = split_top(m[3]);   // drop the leading `Class* c`
            std::vector<std::string> args;
            for (std::size_t i = 1; i < params.size(); ++i) {
                if (const auto star = params[i].find('*'); star != std::string::npos) {
                    args.emplace_back(text::trim(std::string_view(params[i]).substr(0, star)));
                }
            }
            factories.push_back({name, std::string(m[1]), std::move(args)});
            return true;
        });
    }

    // PASS 2: reuse factories in other files
    if (!factories.empty()) {
        write("ReflectionFactory.h", header_text(factories));
        for (const std::string& f : files) {
            if (registry_files.contains(f)) continue;
            const auto text = read(f);
            if (!text || !has(*text, "newInstance")) continue;
            auto [rewritten, sites] = reuse(*text, factories);
            if (sites == 0) continue;
            if (!has(rewritten, "#include \"ReflectionFactory.h\"")) {
                rewritten = insert_after_includes(rewritten, "#include \"ReflectionFactory.h\"");
            }
            write(f, rewritten);
            result.reused.push_back({f, sites});
        }
    }
    for (const RegistryResult& r : result.registries) {
        result.total_sites += r.sites;
        result.total_classes += r.factory.classes.size();
    }
    for (const ReusedResult& r : result.reused) result.total_sites += r.sites;
    return result;
}

}  // namespace cb::java::reflection_factory

