#include "cb/java/init_driver.hpp"

#include <algorithm>
#include <format>
#include <map>

#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::init_driver {

namespace stdfs = std::filesystem;

namespace {

bool drv_is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }

// The words that start with a capital letter (`\b[A-Z]\w*\b`): class names, as the scan reads them
NameSet drv_capitalised_words(std::string_view text) {
    NameSet words;
    for (std::size_t i = 0; i < text.size();) {
        if (!drv_is_word(text[i])) { ++i; continue; }
        std::size_t j = i;
        while (j < text.size() && drv_is_word(text[j])) ++j;
        if (text[i] >= 'A' && text[i] <= 'Z') words.insert(std::string(text.substr(i, j - i)));
        i = j;
    }
    return words;
}

// The capitalised words that follow `new` (`\bnew\s+([A-Z]\w*)`): the classes an initialiser constructs
std::vector<std::string> drv_new_targets(std::string_view text) {
    std::vector<std::string> targets;
    for (std::size_t at = text.find("new"); at != std::string_view::npos; at = text.find("new", at + 1)) {
        if (at > 0 && drv_is_word(text[at - 1])) continue;
        std::size_t i = at + 3;
        const std::size_t blanks = i;
        while (i < text.size() && text::is_space(text[i])) ++i;
        if (i == blanks || i >= text.size() || text[i] < 'A' || text[i] > 'Z') continue;
        std::size_t j = i;
        while (j < text.size() && drv_is_word(text[j])) ++j;
        targets.emplace_back(text.substr(i, j - i));
    }
    return targets;
}

// The pieces of Java text that run when an object of the class is constructed: its constructor bodies, its instance field
// initialisers and its instance `{ }` blocks (all execute during construction), joined by spaces.
std::string drv_constructed_text(const ir::JClass& cls) {
    std::vector<std::string_view> parts;
    for (const ir::Method* m : cls.constructors()) {
        for (const ir::SourceLine& line : m->body) parts.push_back(line.text);
    }
    for (const ir::Field& f : cls.fields) {
        if (!f.is_static && f.initializer) parts.push_back(*f.initializer);
    }
    for (const ir::Body& block : cls.inst_inits) {
        for (const ir::SourceLine& line : block) parts.push_back(line.text);
    }
    std::erase_if(parts, [](std::string_view p) { return p.empty(); });
    return text::join(parts, " ");
}

// The initable classes a class's static initialisation reads (directly, or through the constructors it runs).
NameSet drv_direct_refs(const ir::JClass& c, const std::map<std::string, const ir::JClass*, std::less<>>& by_name, const NameSet& callable_init) {
    std::vector<std::string_view> parts;
    for (const ir::Field& f : c.fields) {
        if (f.is_static && f.initializer) parts.push_back(*f.initializer);
    }
    for (const ir::Body& block : c.static_inits) {
        for (const ir::SourceLine& line : block) parts.push_back(line.text);
    }
    for (const ir::EnumConst& ec : c.enum_consts) {   // typed-enum constant arguments are built in initStatics() (EnumCreatureType reads Material.air)
        for (const std::string& arg : ec.args) parts.push_back(arg);
    }
    std::erase_if(parts, [](std::string_view p) { return p.empty(); });
    const std::string joined = text::join(parts, " ");
    NameSet refs = drv_capitalised_words(joined);
    // follow `new X` transitively into the constructed classes' constructor, instance and ancestor bodies
    std::vector<std::string> todo = drv_new_targets(joined);
    NameSet seen;
    while (!todo.empty()) {
        const std::string x = std::move(todo.back());
        todo.pop_back();
        if (!seen.insert(x).second) continue;
        const auto found = by_name.find(x);
        const ir::JClass* anc = found == by_name.end() ? nullptr : found->second;
        while (anc != nullptr) {   // x and its superclass chain
            const std::string body = drv_constructed_text(*anc);
            const NameSet more = drv_capitalised_words(body);
            refs.insert(more.begin(), more.end());
            for (std::string& t : drv_new_targets(body)) todo.push_back(std::move(t));
            const auto next = anc->parent ? by_name.find(*anc->parent) : by_name.end();
            if (next == by_name.end() || seen.contains(next->second->name)) break;
            anc = next->second;
        }
    }
    NameSet out;
    for (const std::string& r : refs) {
        if (callable_init.contains(r) && r != c.name) out.insert(r);   // only OTHER initable classes are real dependencies
    }
    return out;
}

// A class's base name as the ancestry registry spells it: no type arguments
std::string drv_plain_name(std::string_view name) { return std::string(text::trim(name.substr(0, name.find('<')))); }

}  // namespace

std::vector<std::string> compute_static_init_order(std::span<const ir::JClass> classes, const NameSet& callable_init, const std::vector<std::string>& seed) {
    std::map<std::string, const ir::JClass*, std::less<>> by_name;
    for (const ir::JClass& c : classes) by_name[c.name] = &c;

    std::map<std::string, NameSet, std::less<>> deps;
    for (const std::string& n : callable_init) {
        if (const auto it = by_name.find(n); it != by_name.end()) deps[n] = drv_direct_refs(*it->second, by_name, callable_init);
    }
    // The seed is a HARD ordering chain, not just a tiebreaker: each seeded class that is present runs after the previous one, so the
    // orderings that are mediated by constructors (invisible to the scan) hold.
    std::vector<std::string> seed_present;
    for (const std::string& n : seed) {
        if (deps.contains(n)) seed_present.push_back(n);
    }
    for (std::size_t i = 1; i < seed_present.size(); ++i) deps[seed_present[i]].insert(seed_present[i - 1]);

    std::map<std::string, std::size_t, std::less<>> priority;
    for (std::size_t i = 0; i < seed.size(); ++i) priority[seed[i]] = i;
    const auto order_key = [&](const std::string& n) {
        const auto it = priority.find(n);
        return std::pair<std::size_t, const std::string&>(it == priority.end() ? seed.size() : it->second, n);
    };

    std::vector<std::string> ordered;
    NameSet placed;
    NameSet remaining;
    for (const auto& [n, unused] : deps) remaining.insert(n);
    while (!remaining.empty()) {
        std::vector<const std::string*> ready;
        for (const std::string& n : remaining) {
            if (std::ranges::all_of(deps.at(n), [&placed](const std::string& d) { return placed.contains(d); })) ready.push_back(&n);
        }
        if (ready.empty()) {   // a dependency cycle: broken by priority
            for (const std::string& n : remaining) ready.push_back(&n);
        }
        const std::string* pick = *std::ranges::min_element(ready, [&](const std::string* a, const std::string* b) { return order_key(*a) < order_key(*b); });
        const std::string chosen = *pick;   // (a copy: the erase below removes the element the pointer names)
        ordered.push_back(chosen);
        placed.insert(chosen);
        remaining.erase(chosen);
    }
    // a callable class the corpus does not hold (should not happen) is appended, in a stable order
    for (const std::string& c : callable_init) {
        if (!placed.contains(c)) ordered.push_back(c);
    }
    return ordered;
}

void write_init_driver(const stdfs::path& out_dir, const NameSet& init_classes, std::span<const ir::JClass> classes, const std::vector<std::string>& seed) {
    const std::vector<std::string> ordered = compute_static_init_order(classes, init_classes, seed);

    textutil::write_text(out_dir / "j2x_init.h",
                         "#ifndef J2X_INIT_H\n#define J2X_INIT_H\n"
                         "// Runs every class's static initializers (Java static{} + static field inits) in\n"
                         "// dependency order. CALL ONCE at startup, before any game code. Generated by CodeBreaker.\n"
                         "void j2x_initAll();\n#endif // J2X_INIT_H\n");

    // FIXED BUG 2026-08-13 instanceof-isa: C++ typeid is EXACT; Java instanceof is IS-A. is<T>() (JObject) therefore missed every
    // subclass (`x instanceof Block` was false for BlockOreStorage), silently breaking recipes, entities and the GUI. A class-ancestry
    // registry (each class -> its base and interfaces) that is<T>() walks fixes it.
    NameSet names;
    for (const ir::JClass& c : classes) names.insert(c.name);
    std::vector<std::pair<std::string, std::string>> edges;   // (class, ancestor)
    for (const ir::JClass& c : classes) {
        if (c.parent) {
            const std::string base = drv_plain_name(*c.parent);
            if (names.contains(base) && base != c.name) edges.emplace_back(c.name, base);
        }
        for (const std::string& iface : c.interfaces) {
            const std::string base = drv_plain_name(iface);
            if (names.contains(base) && base != c.name) edges.emplace_back(c.name, base);
        }
    }
    textutil::write_text(out_dir / "j2x_rtti.h",
                         "#ifndef J2X_RTTI_H\n#define J2X_RTTI_H\n"
                         "// Registers each class's base/interface ancestry so JObject::is<T>() (instanceof) matches\n"
                         "// subclasses, not just the exact dynamic type. Call ONCE, before any instanceof runs.\n"
                         "void j2xRegisterRtti();\n"
                         "void j2xPrepopulateClassReg();   // pre-fill _j2xClassReg at BOOT (thread-safe RTTI under MP net threads)\n"
                         "#endif // J2X_RTTI_H\n");
    std::vector<std::string> rtti = {"#include \"stdafx.h\"", "#include \"JavaRT.h\"", "#include \"j2x_rtti.h\"", "#include \"Class.h\"", ""};
    for (const std::string& n : names) rtti.push_back("#include \"" + n + ".h\"");
    // Java value equality: each concrete class that defines equals(Object) or hashCode() (a HashMap key such as ChunkPosition) --
    // JObject compares and orders by them (JavaRT.h j2xRegisterValue)
    std::vector<std::string> valued;
    for (const ir::JClass& c : classes) {
        if (c.is_interface || c.is_abstract) continue;
        bool equals = false, hash_code = false;
        for (const ir::Method& m : c.methods) {
            if (m.is_ctor || m.is_static || m.is_abstract) continue;
            equals = equals || (m.name == "equals" && m.params.size() == 1 && m.params[0].type.name == "Object");
            hash_code = hash_code || (m.name == "hashCode" && m.params.empty());
        }
        if (!equals && !hash_code) continue;
        valued.push_back(c.name);
        rtti.push_back(std::format("static bool j2xEq_{}(void* s, const JObject& o) {{ return {}; }}", c.name,
                                   equals ? std::format("(({}*)s)->equals(o)", c.name) : "s == o.p"));
        rtti.push_back(std::format("static int j2xHash_{}(void* s) {{ return {}; }}", c.name,
                                   hash_code ? std::format("(({}*)s)->hashCode()", c.name) : "(int)(std::size_t)s"));
    }
    for (const char* line : {"", "void j2xRegisterRtti()", "{"}) rtti.emplace_back(line);
    for (const auto& [derived, base] : edges) rtti.push_back(std::format("\tj2xRegisterBase(typeid({}).name(), typeid({}).name());", derived, base));
    for (const std::string& n : valued) rtti.push_back(std::format("\tj2xRegisterValue(typeid({}).name(), j2xEq_{}, j2xHash_{});", n, n, n));
    for (const char* line : {"}", ""}) rtti.emplace_back(line);
    // FIXED BUG classreg-multithread-race: every class is materialised into _j2xClassReg AT BOOT, before the multiplayer network reader
    // and writer threads start, so j2xClassOf never lazily push_backs the shared registry during play. That keeps it READ-ONLY under
    // concurrency: no reader-thread/main-thread use-after-free (the non-deterministic multiplayer heap-corruption hang). Alpha's runtime
    // had no such registry, which is why Alpha multiplayer never corrupted.
    for (const char* line : {"void j2xPrepopulateClassReg()", "{"}) rtti.emplace_back(line);
    for (const std::string& n : names) rtti.push_back(std::format("\tj2xClassOf(typeid({}));", n));
    for (const char* line : {"}", ""}) rtti.emplace_back(line);
    textutil::write_text(out_dir / "j2x_rtti.cpp", text::join(rtti, "\n"));

    std::vector<std::string> lines = {"#include \"stdafx.h\"", "#include \"JavaRT.h\"", "#include \"j2x_init.h\"", "#include \"j2x_rtti.h\"", ""};
    for (const std::string& c : ordered) lines.push_back("#include \"" + c + ".h\"");
    // FIXED BUG 2026-08-13 init-trace: a per-class boot trace pinpoints a crash INSIDE j2x_initAll (a class whose initStatics or
    // constructor dereferences a not-yet-initialised static): the last [INIT] line logged names the offender. One-shot at boot (sparse,
    // so safe by the DbgPrintF rule); J2X_INIT_TRACE 0 strips it from a release build. Raw DbgPrint (it surfaces once main() runs).
    for (const char* line : {"", "extern \"C\" unsigned long __cdecl DbgPrint(const char* Format, ...);", "#ifndef J2X_INIT_TRACE",
                             "#define J2X_INIT_TRACE 0   // default OFF (floods boot log). Set 1 to pinpoint a static-init crash.", "#endif", "",
                             "void j2x_initAll()", "{",
                             "\tj2xRegisterRtti();   // instanceof ancestry -- MUST precede any is<T>() (recipe/entity init)",
                             "\tj2xPrepopulateClassReg();   // pre-fill the class registry at BOOT so RTTI is read-only under MP net threads"}) {
        lines.emplace_back(line);
    }
    for (const std::string& c : ordered) {
        lines.emplace_back("#if J2X_INIT_TRACE");
        lines.push_back(std::format("\tDbgPrint(\"[INIT] {}\\n\");", c));
        lines.emplace_back("#endif");
        lines.push_back(std::format("\t{}::initStatics();", c));
    }
    for (const char* line : {"}", ""}) lines.emplace_back(line);
    textutil::write_text(out_dir / "j2x_init.cpp", text::join(lines, "\n"));
}

}  // namespace cb::java::init_driver
