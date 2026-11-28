// translate/registry.hpp -- the registry of the translator's pieces: every front end and every back end says what it is, and the table of translation
// routes is computed from what is registered (language unification plan, step 1).
//
// The hub is the typed tree (translate/ir.hpp): every front end builds it, every back end reads it (or reads its L2 document through native_l2). A
// translation is a chain of registered pieces:
//
//     front end (a language, the tool it reads it with)  ->  typed tree  ->  back end (a target language) with a rule set for the source language
//
//     registry::Registry r;
//     r.add(registry::FrontEnd{"csharp", "Roslyn through the .NET SDK", {".cs"}});
//     r.add(registry::BackEnd{"cpp17", "backends/csharp_cpp17", {{"csharp", "judge-cs-cpp", "builds ...", {"dotnet", "cxx"}, true}}});
//     for (const registry::Route& route : r.routes()) ...      // csharp -> cpp17, computed
//
// A rule set is the back end's way of writing one source language ("one emitter, a rule set per source language"): the proof that checks it (the judge
// command that runs both programs), the tools that proof needs, and whether it is wired end to end for one file (`file_level`: translate_file.hpp runs it,
// and capability.hpp offers it). A route exists when a front end for the rule's source language and the back end are both registered, so a route cannot be
// listed whose pieces are missing, and a piece cannot be added without its routes appearing. Nothing here is a table of pairs.
//
// Each module registers itself with a function of its own (`register_...` in the module's registration file); `standard()` calls them. A module added to
// the build is added to the one list of modules in registry.cpp, not to a list of paths.
//
// Platform-neutral: no operating-system API here.
#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace cb::translate::registry {

// A language the hub knows by name. `id` is stable ("c", "cpp03", "cpp17", "csharp", "java", "python"); the order is the order a list shows them in.
struct LanguageName {
    std::string id, name;
};

// A front end: reads a language into the typed tree.
struct FrontEnd {
    std::string language;                     // id of the language it reads
    std::string tool;                         // what does the parsing: "clang's AST", "javac's tree", "Roslyn", "CPython's ast and the translator's inference"
    std::vector<std::string> extensions;      // the file names it reads, lower case with the dot (empty: no file-level reader)
};

// How a back end writes one source language's programs.
struct RuleSet {
    std::string from;                         // the source language (a front end for it must be registered)
    std::string judge;                        // the command that proves it by running both: "judge-cs-cpp"
    std::string proof;                        // what the proof is, in words
    std::vector<std::string> needs;           // tools the proof needs: "dotnet", "cxx", "python", "l2"
    bool file_level = false;                  // wired end to end for one file (translate_file.hpp runs it)
};

// A back end: writes the typed tree as a target language.
struct BackEnd {
    std::string language;                     // id of the language it writes
    std::string emitter;                      // the module that writes it: "backends/csharp_cpp17", "l2 core cpp17"
    std::vector<RuleSet> rules;               // one per source language it writes (an emitter can appear twice for one language, in two back ends)
};

// A route the registry computed: a front end, the typed tree, a back end with its rule set for the source.
struct Route {
    std::string from, to, emitter, judge, proof;
    std::vector<std::string> needs;
    std::string front_end;                    // the front end's tool
    bool file_level = false;
};

class Registry {
public:
    Registry();                               // holds the language names, nothing else
    void add(FrontEnd front_end);
    void add(BackEnd back_end);

    const std::vector<LanguageName>& language_names() const { return names_; }
    const std::vector<FrontEnd>& front_ends() const { return front_ends_; }
    const std::vector<BackEnd>& back_ends() const { return back_ends_; }

    const FrontEnd* front_end_for(std::string_view language) const;     // the first registered for it, or nullptr
    bool has_back_end(std::string_view language) const;
    // Every route: for each rule set of each back end whose source language has a front end. Ordered by source language, then target (the order of
    // language_names()), then registration. Computed on each call.
    std::vector<Route> routes() const;
    // The routes that are wired end to end for a file.
    std::vector<Route> file_routes() const;

private:
    std::vector<LanguageName> names_;
    std::vector<FrontEnd> front_ends_;
    std::vector<BackEnd> back_ends_;
    int index_of(std::string_view language) const;
};

// The registry of this program: every module's registration run once.
const Registry& standard();

// The modules' registration functions (each in the module's own registration file); `standard()` calls them all.
void register_clang_front_end(Registry&);
void register_csharp_front_end(Registry&);
void register_java_front_end(Registry&);
void register_python_front_end(Registry&);
void register_c_back_end(Registry&);          // backends/c_java: Java as C11
void register_cpp03_back_end(Registry&);
void register_cpp17_back_end(Registry&);      // the L2 core's C++17 writer
void register_csharp_back_end(Registry&);     // dotnet/decompiler/csharp/l2_syntax_builder (+ the rule set java)
void register_csharp_cpp17_back_end(Registry&);
void register_java_back_end(Registry&);       // backends/java: C and C++ as Java
void register_rust_back_end(Registry&);       // backends/rust_java: Java as Rust
void register_go_back_end(Registry&);         // backends/go_java: Java as Go

}  // namespace cb::translate::registry
