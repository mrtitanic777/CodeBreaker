// translate/capability.cpp -- see capability.hpp: the tables of languages and paths, the tools the paths need, and the target lists.
#include "translate/capability.hpp"

#include <algorithm>
#include <map>
#include <mutex>

#include "base/config.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/jar.hpp"
#include "translate/backends/c_java.hpp"
#include "translate/backends/rust_java.hpp"
#include "translate/dotnet/package.hpp"
#include "translate/judge_cs_cpp_command.hpp"
#include "translate/native_l2.hpp"
#include "translate/registry.hpp"

namespace cb::translate::capability {

namespace stdfs = std::filesystem;

namespace {

// ---- the tables --------------------------------------------------------------------------------------------------------------------------

// Both tables are computed from the registry (translate/registry.hpp): each front end and back end registers itself, and nothing here lists a pair.
// A language reads (has extensions) when a front end for it is registered, and writes (`back_end`) when a back end is.
const std::vector<Language>& cap_languages() {
    static const std::vector<Language> list = [] {
        const registry::Registry& reg = registry::standard();
        std::vector<Language> out;
        for (const registry::LanguageName& n : reg.language_names()) {
            Language l{n.id, n.name, {}, reg.has_back_end(n.id)};
            for (const registry::FrontEnd& f : reg.front_ends())
                if (f.language == n.id) l.extensions.insert(l.extensions.end(), f.extensions.begin(), f.extensions.end());
            out.push_back(std::move(l));
        }
        return out;
    }();
    return list;
}

// The paths wired end to end: the registry's file-level routes. Each is run and proven by translate_file.cpp with the judge's own steps.
const std::vector<Path>& cap_paths() {
    static const std::vector<Path> list = [] {
        std::vector<Path> out;
        for (const registry::Route& r : registry::standard().file_routes()) out.push_back(Path{r.from, r.to, r.judge, r.proof, r.needs});
        return out;
    }();
    return list;
}

// ---- the tools -----------------------------------------------------------------------------------------------------------------------------

bool cap_is_file(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_regular_file(p, ec);
}

// `python` found the way judge-py-cpp finds it: CB_PYTHON, else the PATH.
bool cap_has_python() {
    if (const auto env = fs::get_env("CB_PYTHON")) return cap_is_file(fs::from_text(*env));
    const auto path_var = fs::get_env("PATH");
    if (!path_var) return false;
#ifdef _WIN32
    constexpr char kSeparator = ';';
    const std::vector<std::string> names = {"python.exe"};
#else
    constexpr char kSeparator = ':';
    const std::vector<std::string> names = {"python3", "python"};
#endif
    for (const std::string_view dir : text::split(*path_var, kSeparator)) {
        if (dir.empty()) continue;
        for (const std::string& n : names)
            if (cap_is_file(fs::from_text(dir) / n)) return true;
    }
    return false;
}

bool cap_on_path(std::string_view name) {
    const auto path_var = fs::get_env("PATH");
    if (!path_var) return false;
#ifdef _WIN32
    constexpr char kSeparator = ';';
    const std::string file = std::string(name) + ".exe";
#else
    constexpr char kSeparator = ':';
    const std::string file(name);
#endif
    for (const std::string_view dir : text::split(*path_var, kSeparator))
        if (!dir.empty() && cap_is_file(fs::from_text(dir) / file)) return true;
    return false;
}

// A JDK: a java with its javac beside it.
bool cap_has_jdk() {
    const auto java = decompile::jar::find_java({});
    return java && cap_is_file(java->parent_path() / (java->extension() == ".exe" ? "javac.exe" : "javac"));
}

// "" when the tool is there, else the phrase that names it.
std::string cap_check_tool(const std::string& tool) {
    if (tool == "jdk") return cap_has_jdk() ? std::string() : "a Java JDK (java and javac)";
    if (tool == "cc") return cap_is_file(fs::from_text(std::string(backends::c_java::kClang))) || cap_on_path("gcc") ? std::string() : "a C compiler (clang or gcc)";
    if (tool == "rustc") return backends::rust_java::find_rustc() ? std::string() : "the Rust compiler (rustc)";
    if (tool == "dotnet") {
        const auto sdk = dotnet::Sdk::find();
        return sdk && sdk->available() ? std::string() : "the .NET SDK";
    }
    if (tool == "cxx") return judge_cs_cpp::find_cxx("") ? std::string() : "a C++17 compiler (g++, clang++ or Visual Studio's cl)";
    if (tool == "python") return cap_has_python() ? std::string() : "Python";
    if (tool == "l2") return std::string();   // (the L2 library is linked in)
    return tool;
}

// ---- reasons and lists ---------------------------------------------------------------------------------------------------------------------

std::string cap_name_of(std::string_view id) {
    const Language* l = find_language(id);
    return l != nullptr ? l->name : std::string(id);
}

// Why `to` is not offered for source code in `from` (the name of the language the source is in), when it is not.
std::string cap_reason(std::string_view from, const Language& to) {
    if (!to.back_end) return to.name + " output is not supported yet";
    const Path* path = find_path(from, to.id);
    if (path == nullptr) return cap_name_of(from) + " to " + to.name + " is not supported yet";
    if (const std::string tool = missing_tool(*path); !tool.empty()) return "needs " + tool + ", which is not set up on this computer";
    return {};
}

Target cap_target_of(std::string_view from, const Language& to) {
    Target t{.id = to.id, .name = to.name};
    t.reason = cap_reason(from, to);
    t.supported = t.reason.empty();
    if (t.supported) t.proof = find_path(from, to.id)->proof;
    return t;
}

Target cap_original(bool supported, std::string reason) {
    Target t{.id = "original", .name = "Original", .supported = supported};
    if (!supported) t.reason = std::move(reason);
    return t;
}

std::vector<Target> cap_list_for(std::string_view from, Target original) {
    std::vector<Target> out;
    out.push_back(std::move(original));
    for (const Language& l : languages()) out.push_back(cap_target_of(from, l));
    return out;
}

}  // namespace

const std::vector<Language>& languages() { return cap_languages(); }
const std::vector<Path>& paths() { return cap_paths(); }

const Language* find_language(std::string_view id) {
    for (const Language& l : languages())
        if (l.id == id) return &l;
    return nullptr;
}

const Language* language_of_file(const stdfs::path& file) {
    const std::string ext = text::lower(fs::to_text(file.extension()));
    if (ext.empty()) return nullptr;
    for (const Language& l : languages())
        if (std::find(l.extensions.begin(), l.extensions.end(), ext) != l.extensions.end()) return &l;
    return nullptr;
}

std::vector<std::string> source_extensions() {
    std::vector<std::string> out;
    for (const Language& l : languages()) out.insert(out.end(), l.extensions.begin(), l.extensions.end());
    std::sort(out.begin(), out.end());
    return out;
}

const Path* find_path(std::string_view from, std::string_view to) {
    for (const Path& p : paths())
        if (p.from == from && p.to == to) return &p;
    return nullptr;
}

std::string missing_tool(const Path& path) {
    static std::mutex lock;
    static std::map<std::string, std::string> found;   // tool -> "" or the phrase naming it
    std::lock_guard hold(lock);
    for (const std::string& tool : path.needs) {
        auto it = found.find(tool);
        if (it == found.end()) it = found.emplace(tool, cap_check_tool(tool)).first;
        if (!it->second.empty()) return it->second;
    }
    return {};
}

std::vector<Target> targets() {
    std::vector<Target> out;
    out.push_back(cap_original(true, {}));
    for (const Language& l : languages()) {
        Target t{.id = l.id, .name = l.name};
        if (!l.back_end) {
            t.reason = l.name + " output is not supported yet";
        } else {
            t.supported = std::any_of(paths().begin(), paths().end(), [&](const Path& p) { return p.to == l.id; });
            if (!t.supported) t.reason = l.name + " output is not supported yet";
        }
        out.push_back(std::move(t));
    }
    return out;
}

std::vector<Target> targets_for_source(std::string_view source_id) {
    return cap_list_for(source_id, cap_original(false, "This is source code already: choose the language to translate it to."));
}

std::vector<Target> targets_for_file(const stdfs::path& file) {
    if (const Language* l = language_of_file(file)) return targets_for_source(l->id);
    return targets();
}

std::vector<Target> targets_for_decompiled(std::string_view language) {
    if (language.empty() || find_language(language) == nullptr) {
        std::vector<Target> out = targets();
        for (Target& t : out) {
            if (t.id == "original") continue;
            t.supported = false;
            t.reason = "This program comes back as assembly, which the translator does not read yet";
        }
        return out;
    }
    return cap_list_for(language, cap_original(true, {}));
}

const Target* find_target(const std::vector<Target>& list, std::string_view text) {
    std::string want = text::lower(text);
    static const std::map<std::string, std::string> aliases = {{"c++17", "cpp17"}, {"c++03", "cpp03"}, {"c#", "csharp"}, {"cs", "csharp"}, {"py", "python"}, {"rs", "rust"}};
    if (const auto it = aliases.find(want); it != aliases.end()) want = it->second;
    for (const Target& t : list)
        if (t.id == want || text::lower(t.name) == want) return &t;
    return nullptr;
}

std::string supported_names(const std::vector<Target>& list) {
    std::vector<std::string> names;
    for (const Target& t : list)
        if (t.supported && t.id != "original") names.push_back(t.name);
    return text::join(names, ", ");
}

}  // namespace cb::translate::capability
