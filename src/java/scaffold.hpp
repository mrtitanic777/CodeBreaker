// java/scaffold.hpp -- skeleton output for the target languages that are not C++ (C, C#, Rust).
//
// C++ is the fully translated target. The others are scaffolds: for each Java class one file in the target language
// with the type or struct shape, the fields, and the method signatures as unimplemented stubs -- enough to SEE how the
// port would be structured in that language, not a working translation. This is what the language picker's "coming
// soon" options produce, so the whole flow is real.
//
//     const Scaffold made = scaffold::write(java_dir, out_dir, Language::Rust);
//     // made.written: the file names; made.skipped: the .java files that did not parse
//
// `render` is the pure part (one parsed class to the text of its file); `write` walks a folder of Java, parses each
// file, and writes what `render` gives plus a short SKELETON_<LANG>.md saying what the folder is.
#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "java/ir.hpp"

namespace cb::java::scaffold {

enum class Language { C, CSharp, Rust };

std::optional<Language> parse_language(std::string_view name);   // "c", "csharp", "rust"; nullopt for any other text
std::string_view label(Language language);                       // "C", "C#", "Rust"

// A generated skeleton file: its name and its text.
struct File {
    std::string name;
    std::string content;
};

// The skeleton of one class: `<Class>.h` (C), `<Class>.cs` (C#) or `<Class>.rs` (Rust). Types the target does not know
// stay as the Java name (C#, Rust) or become a pointer to a struct of that name (C).
File render(const ir::JClass& cls, Language language);

struct Scaffold {
    std::vector<std::string> written;   // the class files, in the order of the Java files
    std::vector<std::string> skipped;   // Java files that did not parse (their paths), or whose class has no name
};

// Every .java under `java_dir` (sorted by path), parsed and written to `out_dir` (made if absent), then the README.
// NotFoundError when `java_dir` is not a folder; IoError when the output cannot be written.
Scaffold write(const std::filesystem::path& java_dir, const std::filesystem::path& out_dir, Language language);

}  // namespace cb::java::scaffold
