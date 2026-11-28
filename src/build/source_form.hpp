// build/source_form.hpp -- a built module's source form: its sources alone, in one folder, with OpenWatcom's recipe.
//
//     SourceForm f = source_form::write(input);            <build folder>/source: every source the link used, m.lnk, makefile
//     std::string why = source_form::check(f, ow, scratch, image);   "" when `wmake` in a copy of it links `image`, byte for byte
//
// The build's folder is its working area: trial compiles, the error listings of what did not compile, the cache's objects, the
// probes. The source form holds only what the image is made from -- each object's source as it was when that object was made
// (the build compiles some functions more than once), the header, the definition and resource files, the DOS stub carried from
// the original -- and a `makefile` for wmake that makes each object with the tool and the options the build used, and links them
// with wlink. No path in it is absolute: the libraries are found under %WATCOM%.
//
// The names a person reads replace the build's placeholders where they are known and free: an import the module calls by number
// (`KERNEL_18`) by the name its exporting module gives it (`GlobalLock`), an internal function (`sub_3_0488`) by the name the
// corpus's symbol store gives it. A name changes no code: the image the form links is the build's image.
#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "build/functions.hpp"
#include "toolchain/watcom.hpp"

namespace cb::build::source_form {

struct Input {
    std::filesystem::path build_folder;   // where the build linked m.dll
    std::string module;                   // the original's file name ("ver.dll")
    std::vector<std::string> link_lines;  // m.lnk as linked
    // How the build made an object (by its path); nullopt for one it did not note.
    std::function<std::optional<functions::MadeObject>(const std::filesystem::path& object)> made;
    std::map<std::string, std::string> stem_function;   // object stem ("s03/f0000") -> the one function it holds
    std::map<std::string, std::string> names;           // build symbol (C spelling: KERNEL_18, sub_3_0488) -> the name to write
    // an exported function's parameters as the SDK declares it ({function: (type, name) in the header's order})
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> parameters;
    // the resources: the script, its compiled form the link binds, the files the script reads (relative to the build folder)
    std::optional<std::filesystem::path> rc, res;
    std::vector<std::filesystem::path> resource_files;
    std::vector<std::filesystem::path> carried;          // other files linked as they are (stub.exe), relative to the build folder
    std::vector<std::filesystem::path> documents;        // written beside the sources, not made (the .def, the public header)
    std::string libpath = "lib286\\win";                // the libraries under %WATCOM%
};

struct SourceForm {
    std::filesystem::path folder;
    std::size_t c_files = 0, asm_files = 0;
    std::map<std::string, std::string> renamed;   // build symbol -> written name
    std::size_t named_parameters = 0;             // a<N> placeholders given the SDK's parameter name
    std::vector<std::string> missing;             // objects the link uses whose making the build did not note
};

// Write the source form into <build folder>/source (made afresh).
SourceForm write(const Input& in);

// Make the form with wmake in `scratch` (a copy, made afresh and removed after) and compare what it links with `image`: "" when
// they are the same bytes, else what differed.
std::string check(const SourceForm& form, const toolchain::watcom::OpenWatcom& ow, const std::filesystem::path& scratch,
                  const std::filesystem::path& image);

// ---- the parts (shared with the tests) ---------------------------------------------------------------------------------------

// `text` with every identifier that `names` maps written as its new name, and its decorated forms (`_NAME`, as C's cdecl and
// stdcall spell a symbol in an object and the assembler and linker read it) likewise.
std::string rename_identifiers(std::string_view text, const std::map<std::string, std::string>& names);
// Every identifier of a text (for telling a name that is free from one that is taken).
void identifiers_of(std::string_view text, std::set<std::string>& out);
// The renames of `wanted` that can be made: the new name a C and assembler identifier, no other identifier of the texts
// (`taken`, compared without case: the assembler's names have none), no keyword, and no two to the same name.
std::map<std::string, std::string> free_renames(const std::map<std::string, std::string>& wanted, const std::set<std::string>& taken);
// The SDK's names for the a<N> placeholders of a far Pascal function's heading in `c_text` ({a<N>: name}), where the words its
// parameters take, read last first (Pascal pushes the first argument first, so the last lies lowest -- a1), are the words the
// header's parameters take: a parameter of the heading that is one of the header's whole is given its name; two words that are
// halves of one are `<name>_off` and `<name>_seg` (a far pointer) or `<name>_lo` and `<name>_hi`. Nothing when the words do not add
// up, a type's size is not known, or the function is not __stdcall16far.
std::map<std::string, std::string> sdk_parameter_names(std::string_view c_text, std::string_view function,
                                                       const std::vector<std::pair<std::string, std::string>>& declared);
// The words (2 bytes each) a 16-bit SDK type takes on the stack: 1 or 2, or nullopt when not known.
std::optional<int> sdk_words(std::string_view type);

// The makefile's text: a rule per object (tool, options, source), the resources', and the link's.
struct Rule {
    std::string object;              // relative, with backslashes
    std::vector<std::string> needs;  // its sources
    std::string command;
};
std::string makefile(std::string_view module, const std::vector<Rule>& rules, std::string_view libpath);

}  // namespace cb::build::source_form
