// translate/capability.hpp -- what the translator can do, asked of the translator: which languages it reads, which it writes, and for an input
// which targets it supports (docs/TRANSLATOR_DESIGN.md, "Asking the hub what it can do").
//
//     for (const capability::Target& t : capability::targets_for_file("Bank.cs"))
//         if (!t.supported) show_disabled(t.name, t.reason);      // "Java output is not supported yet"
//
// The answers come from three tables kept beside the code they describe (capability.cpp), not from anything a user interface writes:
//
//   languages   every language the hub has a front end for (it reads it: the extensions it takes) or a back end for (it writes it)
//   paths       the (source language, target) pairs that are wired end to end: a front end, a back end, and the judge that proves the result by
//               running both (translate_file.hpp runs a path; judge-cs-cpp and judge-py-cpp are the same proofs as commands)
//   tools       what a path needs on this computer (the .NET SDK, a C++ compiler, Python, the L2 library), found the way the judges find them
//
// A target is supported for a source when a path joins them and its tools are present. Otherwise the target is listed with the reason: the back
// end does not exist ("Python output is not supported yet"), no path joins the two ("C# to Java is not supported yet"), or a tool is missing
// ("needs the .NET SDK, which is not set up on this computer"). "Original" is the one target that is not a translation: the program's own
// decompiled source, as the window has always written it.
//
// Platform-neutral: no operating-system API here.
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::translate::capability {

// A language the hub knows. `extensions` are the file names its front end reads (empty: no front end, it is only written); `back_end` says the hub
// can write it. The ids are stable: "c", "cpp03", "cpp17", "csharp", "java", "python".
struct Language {
    std::string id, name;
    std::vector<std::string> extensions;   // lower case, with the dot: ".cs"
    bool back_end = false;
};

// Every language, in the order a list shows them.
const std::vector<Language>& languages();
const Language* find_language(std::string_view id);
// The language whose front end reads this file's extension; nullptr for any other file.
const Language* language_of_file(const std::filesystem::path& file);
// Every extension a front end reads, sorted (".c", ".cs", ".h", ".java", ".py"): for the file picker and the hint.
std::vector<std::string> source_extensions();

// A way from a source language to a target that is wired end to end and proven by running both.
struct Path {
    std::string from, to;                 // language ids: "csharp" -> "cpp17"
    std::string judge;                    // the command that makes the same proof: "judge-cs-cpp"
    std::string proof;                    // what the proof is, in words: "runs the C# program and the C++17 program and compares what they print and their exit code"
    std::vector<std::string> needs;       // tools: "dotnet", "cxx", "python", "l2"
};
const std::vector<Path>& paths();
const Path* find_path(std::string_view from, std::string_view to);

// The first tool the path needs that this computer lacks, as a phrase ("the .NET SDK"); empty when all are present. Looked for once per process.
std::string missing_tool(const Path& path);

// One entry of a target list.
struct Target {
    std::string id;           // "original", "cpp17"
    std::string name;         // "Original", "C++17"
    bool supported = false;
    std::string reason;       // when not supported: "Python output is not supported yet"
    std::string proof;        // when supported and a translation: how it is checked; empty for Original
};

// The target list with no input yet: Original, then every language; a language is supported when some path ends in it.
std::vector<Target> targets();
// For source code in the language `source_id` (a front end's id): Original is not supported (the source is already source code).
std::vector<Target> targets_for_source(std::string_view source_id);
// For a source file by its extension. A file with no front end gets the list of targets() with Original supported: a program, whose own route decides.
std::vector<Target> targets_for_file(const std::filesystem::path& file);
// For a program whose decompiled output is in `language` ("c": a Windows or DOS program's C; empty: it comes back as assembly, which has no front end).
// Original is supported; the rest follow the paths from that language.
std::vector<Target> targets_for_decompiled(std::string_view language);

// The target a person typed: its id or its name, in any case ("cpp17", "C++17", "c#", "cs", "py").
const Target* find_target(const std::vector<Target>& list, std::string_view text);
// "C++17, C#" -- the names of the supported targets, Original left out: for a message.
std::string supported_names(const std::vector<Target>& list);

}  // namespace cb::translate::capability
