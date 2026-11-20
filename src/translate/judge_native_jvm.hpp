// translate/judge_native_jvm.hpp -- the judge for C/C++ -> Java (docs/TRANSLATOR_DESIGN.md, "The judge, for every pair").
//
// The original runs natively: clang compiles the C or C++ program (-O0, the front end's own target) and it runs. The translation runs on the
// JVM: the clang front end lowers each file to L2, the Java back end writes the program as Java, javac compiles it beside the C runtime (crt),
// and java runs it. Both get the same arguments and the same stdin; their stdout, stderr and exit code are compared. Windows text mode writes
// "\r\n" for "\n" on the native side, so line ends are compared as "\n".
//
// Every program gets a verdict:
//   equivalent  -- same stdout, stderr and exit code;
//   divergent   -- both ran, something differs (the first difference is the reason);
//   refused     -- the translator declined, and says why (front end Unsupported, back end Refused);
//   no-compile  -- the Java it wrote does not compile (a back end bug: javac's first error is the reason);
//   no-original -- the original itself does not build or run here.
//
// What the judge needs of the rest of CodeBreaker is given as a `Host`: the clang front end (finding clang, its options, lowering a file to L2),
// the JDK, the runtime's sources and the Java back end's library mapping, and for `roundtrip` the Java lane's two stages.
//
//     const Verdict v = judge(host, {"prog.c"}, work_folder);
//     v.verdict, v.reason, v.tier
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/arena.hpp"
#include "core/bytes.hpp"
#include "translate/backends/java.hpp"
#include "translate/ir.hpp"

namespace cb::translate::judge_native_jvm {

// A run takes at most this long.
inline constexpr std::chrono::seconds kRunTimeout{120};

// ---- what the judge reads of the rest of CodeBreaker ------------------------------------------------------------------------

struct Jdk {
    std::filesystem::path javac, java;
};

// One source file to lower to L2.
struct LoweringRequest {
    std::filesystem::path path;
    std::string language;                              // "c" or "c++"
    std::optional<std::string> standard;               // "c++14" ...
    std::vector<std::string> includes, quote_includes, defines;
    std::optional<std::vector<std::filesystem::path>> own;   // the files that are the program's own (a header beside the source)
};

struct Host {
    Jdk jdk;
    std::filesystem::path runtime_sources;   // the C runtime for Java: the folder of crt/*.java
    std::filesystem::path state_folder;      // where the compiled runtime is kept
    backends::java::Options java_options;    // the Java back end's library mapping (a back end without one refuses every library call)

    // The clang front end. `clang` is the executable, nullopt where none is found; `flags` the options for parsing (and for the judge's compiling) a file.
    std::function<std::optional<std::filesystem::path>()> clang;
    std::function<std::string(const std::filesystem::path&)> language_of;   // "c" for a .c file, else "c++"
    std::function<std::vector<std::string>(const std::string& language, const std::optional<std::string>& standard, const std::vector<std::string>& includes,
                                           const std::vector<std::string>& quote_includes, const std::vector<std::string>& defines)>
        flags;
    // One source file as an L2 unit made in `arena`: the declarations lowered, cached by the source and the lowering. A file the front end cannot read is a
    // ToolError carrying its message.
    std::function<ir::Node*(Arena& arena, const LoweringRequest&)> lower;

    // The two stages of the Java round trip. `java_to_cpp` writes the classes under `src` as C++ in `cpp_out` (the Java lane) and says what it declined, "" for
    // nothing; it throws on failure. `lift_java_runtime` maps the Java runtime in the C++ the front end read back onto Java's library, in place.
    std::function<std::string(const std::filesystem::path& src, const std::filesystem::path& cpp_out)> java_to_cpp;
    // The units are changed in place; any new nodes are made in `arena`.
    std::function<void(Arena& arena, std::span<ir::Node* const> units)> lift_java_runtime;

    // The Java back end, when it is not `backends::java::program` with `java_options` (a host whose library mapping or idiomatic tier is not the module's
    // own yet): the units of a program in, its Java out. `Refused` for a program it does not write.
    std::function<backends::java::Program(std::span<ir::Node* const> units, const std::string& name)> write_java;
};

// ---- runs and verdicts ------------------------------------------------------------------------------------------------------

// What a program did: its stdout and stderr, and how it ended.
struct Run {
    Bytes out, err;
    std::optional<std::int64_t> exit_code;   // none when it timed out
    bool timed_out() const { return !exit_code; }
};

enum class Kind { Equivalent, Divergent, Refused, NoCompile, NoOriginal };
std::string_view word(Kind kind);   // "equivalent", "divergent", "refused", "no-compile", "no-original"

struct Verdict {
    std::string name;
    Kind verdict = Kind::NoOriginal;
    std::string reason;
    std::optional<std::string> tier;     // the Java back end's tier, "idiomatic" or "faithful"
    std::optional<std::string> java;     // the Java text it wrote
    std::optional<Run> native, jvm;      // none: not run
    std::vector<std::string> notes;

    // `name: verdict -- reason`
    std::string describe() const;
};

// ---- the steps ----------------------------------------------------------------------------------------------------------------

// The C runtime for Java compiled once (cached by its sources and the JDK): the class folder. ToolError when it does not compile.
std::filesystem::path runtime_classes(const Host& host);

// Run `argv` in `cwd` with `input` on stdin. A run that takes longer than `timeout` is a Run with no exit code and "(timed out)" as its stderr.
Run run_program(const std::vector<std::string>& argv, ByteView input, const std::filesystem::path& cwd, std::chrono::seconds timeout = kRunTimeout);

struct Compiled {
    bool ok = false;
    std::string message;   // the end of what the compiler printed
};
// The original, built with clang at -O0.
Compiled build_native(const Host& host, std::span<const std::filesystem::path> files, const std::filesystem::path& out_exe, const std::string& language,
                      const std::optional<std::string>& standard, const std::vector<std::string>& defines, const std::vector<std::string>& includes);

// The program's files (.c, .cpp, .cc, .cxx; headers are read through the sources) as Java. Units are made in `arena`. `Refused` for a front end that
// leaves a construct unsupported, naming it and where; whatever the host's lowering or the back end refuses is as they say.
backends::java::Program translate(const Host& host, Arena& arena, std::span<const std::filesystem::path> files, const std::string& name,
                                  const std::optional<std::string>& language, const std::optional<std::string>& standard, const std::vector<std::string>& defines,
                                  const std::vector<std::string>& includes);

// None when the two runs agree, else what differs first: the first byte of stdout that differs (as a line and a few bytes around it), the exit code, then
// stderr; line ends are compared as "\n".
std::optional<std::string> compare(const Run& native, const Run& jvm);

// The first line that mentions an error or an exception, else the first line, at most 300 characters.
std::string first_error(std::string_view text);

// A Java class name from a file's stem: every byte outside [A-Za-z0-9_] becomes `_`, a leading digit gets a `P` in front, the first letter is a capital.
std::string class_name(std::string_view stem);

struct JudgeOptions {
    std::optional<std::string> name;       // the class name; default: from the first file
    std::optional<std::string> language;   // default: from the first file
    std::optional<std::string> standard;
    std::vector<std::string> defines, includes;
    Bytes stdin_data;
    std::vector<std::string> args;
};

// One C or C++ program, original against translation. Everything it makes goes under `work`.
Verdict judge(const Host& host, std::span<const std::filesystem::path> files, const std::filesystem::path& work, const JudgeOptions& options = {});

// Java -> C++ -> Java, judged: the Java lane writes the classes as C++, the clang front end reads that C++ back (the Java runtime in it lifted onto Java's
// library) and the Java back end writes Java again. The original classes and the round-tripped ones run on the JVM; their output is compared.
// `main_class` is the qualified class whose main runs ("sample.RoundTrip"). The Verdict's `java` is the Java written, its notes what was translated.
Verdict roundtrip(const Host& host, std::span<const std::filesystem::path> java_files, const std::string& main_class, const std::filesystem::path& work,
                  const std::string& name = "RoundTripped");

}  // namespace cb::translate::judge_native_jvm
