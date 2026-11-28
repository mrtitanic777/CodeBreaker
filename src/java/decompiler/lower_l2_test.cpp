// Tests of java/decompiler/lower_l2: Fernflower's tree lowered into L2 is the tree the javac front end makes of the text Fernflower writes for it.
//
// Each program is compiled with javac (--release 8, as the corpus was), its classes decompiled with lower_l2::decompile, and the Java text of that
// run read back through the javac front end (java/frontend, TreeDump). The two units of each class must be the same document, apart from what
// only the text has (source positions, javac's Paren nodes) and what only the bytecode has (a node's bytecode span). The programs are the
// translator's Java corpus (tests/translate/corpus/java) and the kits the Java -> C# capability test runs. A run without a JDK has nothing to
// compare and passes.
#include "java/decompiler/lower_l2.hpp"

#include <cstdio>
#include <map>
#include <set>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/process.hpp"
#include "core/testing.hpp"
#include "java/frontend.hpp"
#include "java/jar.hpp"
#include "translate/l2.hpp"

namespace {

namespace ir = cb::translate::ir;
namespace stdfs = std::filesystem;
namespace ll = cb::java::decompiler::lower_l2;

struct LlJdk {
    stdfs::path java, javac;
};

std::optional<LlJdk> ll_jdk() {
    const std::optional<stdfs::path> java = cb::java::jar::find_java({});
    if (!java) return std::nullopt;
    stdfs::path javac = java->parent_path() / "javac.exe";
    std::error_code ec;
    if (!stdfs::is_regular_file(javac, ec)) javac = java->parent_path() / "javac";
    if (!stdfs::is_regular_file(javac, ec)) return std::nullopt;
    return LlJdk{*java, javac};
}

// What only the text or only the bytecode has, taken out: positions, spans, javac's parentheses (a Paren is its expression).
// A type as its erasure: a type variable is Object, a class has no arguments.
void ll_erase(cb::Json& type) {
    if (type.is_array()) {
        for (cb::Json& t : type) ll_erase(t);
        return;
    }
    if (!type.is_object()) return;
    if (type.contains("kind") && type["kind"] == "typevar") type = cb::Json{{"kind", "class"}, {"name", "java.lang.Object"}};
    type.erase("args");
    for (auto& [key, value] : type.items()) ll_erase(value);
}

// What only the text or only the bytecode has, taken out: positions, spans, javac's parentheses (a Paren is its expression), javac's mark of
// a default constructor (set only where javac's position for it is the class's). And what neither has the same: a library member's declared
// generic types, owner and varargs (javac reads them from the JDK; the decompiler has the erased descriptor and the class the bytecode names),
// so every type is compared as its erasure (the decompiler's text is raw: what generics javac sees in it come from the JDK), and an enum
// constant's owner (javac's dump leaves it out).
void ll_normalise(cb::Json& doc) {
    if (doc.is_object()) {
        if (doc.contains("node") && doc["node"] == "Paren" && doc.contains("x")) {
            cb::Json inner = doc["x"];
            doc = std::move(inner);
            ll_normalise(doc);
            return;
        }
        for (const char* key : {"line", "col", "origin", "lang"}) doc.erase(key);
        for (const char* key : {"type", "test", "of", "extends", "implements", "elem", "throws"})   // (the places a type is)
            if (doc.contains(key)) ll_erase(doc[key]);
        if (doc.contains("node") && doc["node"] == "Catch" && doc.contains("types")) ll_erase(doc["types"]);
        if (doc.contains("node") && doc["node"] == "EnumConst" && doc.contains("ctor") && doc["ctor"].is_object()) doc["ctor"].erase("owner");
        if (doc.contains("kind") && doc["kind"] == "enumconst") doc.erase("owner");
        if (doc.contains("sym") && doc["sym"].is_object()) {
            for (const char* key : {"params", "ret", "type"})
                if (doc["sym"].contains(key)) ll_erase(doc["sym"][key]);
            // a JDK member's owner: javac's is the class that declares it, the decompiler's the class the bytecode names (it reads no JDK)
            if (doc["sym"].contains("owner") && doc["sym"]["owner"].is_string()) {
                const std::string owner = doc["sym"]["owner"].get<std::string>();
                if (owner.starts_with("java.") || owner.starts_with("javax.")) {
                    doc["sym"].erase("owner");
                    doc["sym"].erase("varargs");
                }
            }
        }
        if (doc.contains("mods") && doc["mods"].is_array()) {
            cb::Json kept = cb::Json::array();
            for (const cb::Json& m : doc["mods"])
                if (m != "IMPLICIT") kept.push_back(m);
            if (kept.empty()) doc.erase("mods");
            else doc["mods"] = std::move(kept);
        }
        for (const char* flag : {"postfix", "default", "implicit"})
            if (doc.contains(flag) && doc[flag] == false) doc.erase(flag);
        for (auto& [key, value] : doc.items()) ll_normalise(value);
    } else if (doc.is_array()) {
        for (cb::Json& item : doc) ll_normalise(item);
    }
}

// The first places two documents differ (at most `limit` of them).
void ll_differences(const cb::Json& a, const cb::Json& b, const std::string& path, std::vector<std::string>& out, std::size_t limit) {
    if (out.size() >= limit || a == b) return;
    if (a.is_object() && b.is_object()) {
        for (const auto& [key, value] : a.items()) {
            if (!b.contains(key)) out.push_back(path + "/" + key + " only in the lowering: " + value.dump().substr(0, 160));
            else ll_differences(value, b.at(key), path + "/" + key, out, limit);
        }
        for (const auto& [key, value] : b.items())
            if (!a.contains(key)) out.push_back(path + "/" + key + " only in javac's: " + value.dump().substr(0, 160));
        return;
    }
    if (a.is_array() && b.is_array() && a.size() == b.size()) {
        for (std::size_t i = 0; i < a.size(); ++i) ll_differences(a[i], b[i], path + "/" + std::to_string(i), out, limit);
        return;
    }
    out.push_back(path + ": " + a.dump().substr(0, 200) + "\n        against " + b.dump().substr(0, 200));
}

// One program: its sources compiled, decompiled and lowered, the decompiled text lowered by javac; the number of units that differ.
std::size_t ll_compare(const LlJdk& jdk, const std::string& name, const std::vector<stdfs::path>& sources) {
    cb::fs::ScopedTempDir dir{"cb-lower-l2"};
    const stdfs::path classes = dir.path() / "classes";
    cb::fs::make_dirs(classes);
    // Java 8's class files; a program of a later Java's language as Java 17's, its string concatenation inline (no invokedynamic)
    cb::process::ProcessResult compiled;
    for (const char* release : {"8", "17"}) {
        std::vector<std::string> argv = {cb::fs::to_text(jdk.javac), "--release", release, "-XDstringConcat=inline", "-nowarn", "-encoding", "UTF-8", "-d", cb::fs::to_text(classes)};
        for (const stdfs::path& s : sources) argv.push_back(cb::fs::to_text(s));
        cb::process::ProcessSpec compile;
        compile.argv = argv;
        compile.err = cb::process::Capture::Merge;
        compiled = cb::process::run(compile);
        if (compiled.exit_code == 0) break;
    }
    if (compiled.exit_code != 0) std::fprintf(stderr, "%s: javac: %.400s\n", name.c_str(), std::string(compiled.out.begin(), compiled.out.end()).c_str());
    CB_CHECK_EQ(compiled.exit_code, 0u);
    if (compiled.exit_code != 0) return 1;

    cb::Arena arena;
    cb::java::decompiler::DecompileRequest request;
    request.sources = {classes};
    const ll::Lowered lowered = ll::decompile(request, arena);
    const stdfs::path text_dir = dir.path() / "text";
    for (const auto& [path, text] : lowered.text.java_texts()) {
        const stdfs::path file = text_dir / cb::fs::from_text(path.substr(path.find("!/") == std::string::npos ? 0 : path.find("!/") + 2));
        cb::fs::make_dirs(file.parent_path());
        cb::fs::write_text(file, text);
    }
    cb::java::frontend::DumpTools tools;
    tools.java = jdk.java;
    tools.javac = jdk.javac;
    tools.treedump_source = cb::testing::data_path("../config/translate/frontends/java/TreeDump.java");
    tools.state = dir.path() / "state";
    const std::vector<ir::Node*> theirs = cb::java::frontend::lower_program(arena, text_dir, tools);

    std::map<std::string, cb::Json> by_path;
    for (ir::Node* u : theirs) {
        cb::Json doc = cb::translate::l2::to_json(*u);
        ll_normalise(doc);
        by_path[ir::get<ir::Unit>(u)->path.value_or("")] = std::move(doc);
    }
    std::size_t differ = 0;
    for (ir::Node* u : lowered.units) {
        const std::string path = ir::get<ir::Unit>(u)->path.value_or("");
        cb::Json ours = cb::translate::l2::to_json(*u);
        ll_normalise(ours);
        const auto it = by_path.find(path);
        if (it == by_path.end()) {
            ++differ;
            std::fprintf(stderr, "%s %s: no unit of javac's\n", name.c_str(), path.c_str());
            continue;
        }
        std::vector<std::string> diffs;
        ll_differences(ours, it->second, "", diffs, 6);
        if (diffs.empty()) continue;
        ++differ;
        std::fprintf(stderr, "%s %s:\n", name.c_str(), path.c_str());
        for (const std::string& d : diffs) std::fprintf(stderr, "    %s\n", d.c_str());
        if (const auto out = cb::fs::get_env("CB_LOWER_L2_OUT")) {
            cb::fs::make_dirs(stdfs::path(*out) / name);
            cb::json::save(stdfs::path(*out) / name / (cb::fs::to_text(cb::fs::from_text(path).filename()) + ".ours.json"), ours);
            cb::json::save(stdfs::path(*out) / name / (cb::fs::to_text(cb::fs::from_text(path).filename()) + ".javac.json"), it->second);
        }
    }
    CB_CHECK_EQ(lowered.units.size(), theirs.size());
    std::fprintf(stderr, "%s: %zu classes, %zu differ\n", name.c_str(), lowered.units.size(), differ);
    return differ;
}

std::vector<stdfs::path> ll_java_files(const stdfs::path& dir) { return cb::java::frontend::java_files(dir); }

}  // namespace

// calc and fixtures are javac's tree of their text, class for class. modern is not, and cannot be: its lambdas are passed where the bytecode's
// interfaces are raw (Predicate), so javac types their parameters of the text as Object and cannot resolve what they read (`var0.price()`),
// where the lowering has the types the compiled lambdas take (an Item). translate.csharp.pairs.decompiled_* runs it instead.
CB_TEST("java.decompiler.lower_l2.the_corpus_lowers_as_javac_lowers_its_text", [] {
    const std::optional<LlJdk> jdk = ll_jdk();
    if (!jdk) return;
    const stdfs::path corpus = cb::testing::data_path("../tests/translate/corpus/java");
    for (const char* name : {"calc", "fixtures"}) CB_CHECK_EQ(ll_compare(*jdk, name, ll_java_files(corpus / name)), std::size_t{0});
    (void)ll_compare(*jdk, "modern", ll_java_files(corpus / "modern"));
});

// The kits the Java -> C# capability test runs, each compiled alone (Java 8's class files, or Java 17's). The ones listed are javac's tree of
// their text; the others are printed with where they differ: a method Fernflower cannot decompile (an Unsupported node, where javac has the
// text's empty body), a lambda's parameters where the text is raw (as modern's), the JDK's own knowledge of its generic methods' results
// (javac's `Objects.requireNonNull(x)` is of x's type), javac's names for what it writes itself (an enum body's constructor's parameters)
// and the method a class in a class in a method captures a local of (the outermost: the lowering names the one around it).
CB_TEST("java.decompiler.lower_l2.the_kits_lower_as_javac_lowers_their_text", [] {
    const std::optional<LlJdk> jdk = ll_jdk();
    if (!jdk) return;
    const std::set<std::string> same = {"AnonLibKit", "AsListKit", "BoxKit", "BoxStep", "ClientKit", "DataKit",
                                        "ExtKit", "FormatKit", "LibKit", "SwitchTry", "ToArrayKit"};
    std::vector<stdfs::path> kits;
    for (const char* dir : {"translate/testdata/csharp_java/kit", "translate/testdata/c_java/trykit", "translate/testdata/c_java/streamkit"})
        for (const stdfs::path& f : ll_java_files(cb::testing::data_path(dir))) kits.push_back(f);
    std::size_t differ = 0;
    for (const stdfs::path& kit : kits) {
        const std::string name = cb::fs::to_text(kit.stem());
        const std::size_t d = ll_compare(*jdk, name, {kit});
        differ += d;
        if (same.contains(name)) CB_CHECK_EQ(d, std::size_t{0});
    }
    std::fprintf(stderr, "kits: %zu, %zu differ\n", kits.size(), differ);
    CB_CHECK(kits.size() >= 20);
});
