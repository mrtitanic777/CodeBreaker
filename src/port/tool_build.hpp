// port/tool_build.hpp -- the port built and checked by a compiler of today, each file's result kept by what it depends on.
//
//   check    one C file checked (`-fsyntax-only`): what the compiler said of it is kept under the compiler, its command, the
//            file and every header it reaches, so a file as it was the last time is not checked again
//   build    the port built a C file at a time, side by side (at most four), each object kept the same way; then the objects
//            linked. A compiler of another form (MSVC's, which takes all the files at once) is run once
//
// Both keep their results in a `cache::Cache` (kinds "port-checks" and "port-objects": shared, named by the bytes of what went
// in, so any checkout may use them).
#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "base/cache.hpp"
#include "core/ordered_map.hpp"
#include "port/tool_compilers.hpp"

namespace cb::port::tool {

namespace cache = ::cb::base::cache;

// An object file whole: not empty, its first bytes not nothing (a build cut short leaves either).
bool object_is_whole(const std::filesystem::path& file);

// What one file's check depends on: the compiler (its path) and its command, the file, every header it reaches (as the port's
// text has them).
std::string check_key(const Compiler& spec, const std::filesystem::path& cc, const std::string& file,
                      const OrderedMap<std::string, std::string>& texts);

// The C files to check again: those whose text is not what it was, and those that include (themselves or through another
// header) a header whose text is not. With nothing before, every C file.
std::vector<std::string> changed_files(const OrderedMap<std::string, std::string>& texts,
                                       const OrderedMap<std::string, std::string>& before, const std::vector<std::string>& c_files);

// The compiler's check of the C file `file` (in `out`), kept: its complaints. `ran` is set when the compiler was run (not found in
// the cache).
std::vector<Diagnostic> check_file(cache::Cache& cache, const Compiler& spec, const std::filesystem::path& cc, const std::filesystem::path& out,
                                   const std::string& file, const OrderedMap<std::string, std::string>& texts, bool& ran);

struct BuildResult {
    bool ok = false;
    std::string log;
};

// The program: `tpl` is the compiler's build command (`{cc} -O1 -I{inc} -o {out} {srcs}`), `sources` the C files (and objects)
// to build in `out`, `exe` the program, `objects` a folder for the objects made on the way (removed after). `jobs`: how many
// files at once (at most four).
BuildResult build(cache::Cache& cache, const Compiler& spec, const std::filesystem::path& cc, const std::vector<std::string>& tpl,
                  const std::filesystem::path& out, const std::vector<std::string>& sources, const std::filesystem::path& exe, int jobs);

}  // namespace cb::port::tool
