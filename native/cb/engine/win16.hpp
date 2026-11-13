// cb/engine/win16.hpp -- the 16-bit module build as steps of the engine: describe, decompile, compile, gate, link.
//
//     Win16 win16(steps, engine, {.builder = builder, .wcc = wcc, .options = platform_options, .stages = stages});
//     Built16 b = win16.build("USER.EXE");                // the image in the database's build folder, or `outdir`
//     auto db = engine.open(path); engine.get(db, "ne.image");
//
// The facts, and what each is made from (a key is a function's address in the original, "1:7cd4"):
//
//     binary                the file (given)
//     ne.segments           its segment table                                    <- binary
//     ne.module             the module decompiled and made compilable (describe, the decompiler, c_text: the lane's prepare) and
//                           its description, as a document                        <- binary
//     ne.description        the description's lines: what was said of the module before it was decompiled      <- ne.module
//     ne.functions          every function: address, name, segment, near, its object's stem                   <- ne.module
//     ne.header             the header every function compiles with               <- ne.module
//     ne.lines              the description's line for each function              <- ne.module
//     function.c <key>      the function's C                                      <- ne.module, ne.functions
//     function.notes <key>  what the decompiler said of it besides its C (its pointers' sizes, the globals it touches, the
//                           structures it recovered)                              <- ne.module, ne.functions
//     function.l2 <key>     its L2 document, the C is written from (or null)     <- function.notes
//     function.source <key> the C as the build compiles it: calls left as addresses named, its segment's code segment, the
//                           compiler's options                                    <- function.c, ne.functions
//     function.object <key> the object the compiler makes of it (or why it would not, and the stand-in's object)
//                                                                                 <- function.source, ne.header
//     function.never_set <key>  what it reads and never sets (the compiler's flow check)   <- function.source, ne.header
//     function.contract <key>   the description's lines for it and for each function it calls   <- function.c, ne.lines
//     function.gate <key>   run beside the original's: same, or how it differs    <- binary, ne.header, function.c, function.contract
//     ne.image              the linked module and its report: {name, report, rejected, gated, gate_set, served, image}
//                                                                                 <- all of the above
//
// The stages are the build's own, called as they are; the steps only say what each is made from. What is particular to 16-bit
// Windows they ask `Stages` for: the segments of the file (the loader), the lane's preparation, the gate, the contract check and
// the link; the compiler comes from the FunctionBuilder -- the steps of another platform would be the same steps asking its
// slots. So a function's C changed (by hand: Engine::put(db, "function.c", key, text)) makes again that function's source,
// object, flow check and gate verdict, and the link -- nothing else.
//
// The link is handed the compiler's work: while it runs, a function it compiles with the options and text a function.object
// fact was made from gets that fact's object (and its flow check the function.never_set fact's), and the compiler is not started
// (FunctionBuilder::serve). So if the options here ever drift from the link's, the link is slower, never wrong. The gate is
// run as the module build runs it: the verdicts already known first, the functions rebuilt from C then run beside the original's
// in the build just made, and the module linked once more when some differ.
//
// What a fact is, here: a document. The decompiled module is kept as the document of Module16 (everything the steps read of it:
// nothing else of the lane's preparation is needed after it), an object as its bytes in hex, and the linked file in the build
// folder, named by the fact.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/build/functions.hpp"
#include "cb/engine/engine.hpp"
#include "cb/knowledge/value.hpp"
#include "cb/verify/lanes/kind.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::engine::win16 {

inline constexpr std::string_view kPlatform = "win16";

// A row of the segment table.
struct Segment {
    std::int64_t idx = 0;
    std::string kind;                       // "CODE" | "DATA"
    std::int64_t file_off = 0, length = 0, flags = 0, minalloc = 0;
};

// Where a function is in the original.
struct Place {
    std::int64_t seg = 0, off = 0;
};

// The decompiled module as the lane prepares it: what the steps read of it. `key`, `order`, `texts`, `header`, `sig` and `all_keys` are the
// Prepared's.
struct Module16 : verify::lanes::Prepared {
    std::vector<std::string> description;                        // the description's lines (tab separated: F a function, X an import ...)
    std::map<std::int64_t, std::string> by_va;                   // function address -> name
    std::map<std::int64_t, Place> where;                         // function address -> (segment, offset); a function not placed is absent
    std::set<std::string> near;                                  // the functions called near (within their segment)
    std::map<std::string, knowledge::Value> notes;               // name -> what the decompiler said besides the C ("l2": its L2 document)
    std::set<std::string> wide;                                  // the functions that take or give 32-bit values
};

// The module as the fact ne.module keeps it, and back. FormatError for a document that is not one.
knowledge::Value to_value(const Module16& module);
Module16 module_of(const knowledge::Value& value);

// The compiler's options for a function of the module: the platform's with the medium model, the 8087, size, the code class first.
std::vector<std::string> cflags(const std::vector<std::string>& platform_options);

// What the link of a module reports.
struct LinkOutcome {
    std::optional<std::filesystem::path> image;                  // the linked file; none: nothing was linked
    std::string name;
    std::set<std::string> placed_c;                              // the functions whose C is in the image
    std::set<std::string> moved_c;                               // ... those moved to a segment of their own
    std::map<std::string, std::string> resident_overflow;        // functions to leave to the original because the resident segment overflowed
    std::map<std::string, std::string> gated;                    // the functions left to the original because the gate found them different: why
    std::map<std::string, std::string> rejected;                 // the functions the compiler turned down: why
    std::optional<std::string> gate_error, contract_error;
};

// The link of the module in `folder`. `gated` are functions to leave as the original's. It compiles through the builder the steps serve,
// and does not change `module`.
struct LinkRequest {
    const Module16& module;
    std::filesystem::path binary, folder;
    std::map<std::string, std::string> gated;
    std::size_t functions = 0;
};

// One answer of the gate.
struct GateAnswer {
    std::string name;
    verify::Verdict verdict = verify::Verdict::Inconclusive;
    std::string why;
};

// What the steps ask of the platform's parts that are not in the engine.
struct Stages {
    // The segments of the file (the platform's loader).
    std::function<std::vector<Segment>(const std::filesystem::path& binary)> segments;
    // The lane's prepare: the module decompiled and made compilable, in `work`.
    std::function<Module16(const std::filesystem::path& binary, const std::filesystem::path& work)> prepare;
    // The tables the gate is given of the module (the functions, the wide ones, where each is placed): a document the gate reads.
    std::function<knowledge::Value(const Module16&)> gate_tables;
    // The functions `names` of the build in `folder` run beside the original's.
    std::function<std::vector<GateAnswer>(const std::filesystem::path& binary, const std::filesystem::path& folder,
                                          const std::vector<std::string>& names, const knowledge::Value& tables)> gate;
    // The functions of `placed` whose calls are not the original's, with why (a throw is the build's contract_error).
    std::function<std::map<std::string, std::string>(const std::filesystem::path& binary, const std::filesystem::path& folder,
                                                     const std::filesystem::path& image, const std::vector<std::string>& placed)> contract;
    std::function<LinkOutcome(const LinkRequest&)> link;
    // The linked file made the whole file's image (resources and all), before the gate.
    std::function<void(const std::filesystem::path& binary, const std::filesystem::path& image)> whole_file_image;
    // The linked image checked against the original: the build's report (a build with no image: why it is not built).
    std::function<std::string(const LinkOutcome&)> check;
    // The identity of what each stage runs, digested into the versions of the steps it makes (empty: nothing).
    std::function<std::string()> module_stamp, gate_stamp, image_stamp;
};

struct Config {
    build::functions::FunctionBuilder& builder;
    build::functions::Compiler wcc;                       // the compiler and the environment it runs in
    std::vector<std::string> options;              // the platform's compiler options
    Stages stages;
    std::function<std::string()> compiler_stamp;   // the compiler's identity (its bytes)
    bool gate = true;                              // run the gate on the functions rebuilt from C (the build says CB_NO_GATE otherwise)
};

// What module build of a 16-bit module made.
struct Built16 {
    std::string name;
    std::string report;
    std::map<std::string, std::string> rejected, gated;
    std::vector<std::string> gate_set;
    std::int64_t served = 0, compiled = 0;           // objects the engine had made and the link took / the link compiled itself
    std::optional<std::filesystem::path> image;      // the linked file, in `outdir` or the database's build folder
};

class Win16 {
public:
    // Declares the steps above on `steps` (the platform "win16").
    Win16(Steps& steps, Engine& engine, Config config);
    ~Win16();
    Win16(const Win16&) = delete;
    Win16& operator=(const Win16&) = delete;

    // module build of a 16-bit module, made by the engine: the image in `outdir`, else the database's build folder. (Running the
    // built module is the caller's.)
    Built16 build(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir = std::nullopt,
                  const Run::Log& log = nullptr);

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

// A call the decompiler left as an address (`func_0x10000ec0`) goes to the function reconstructed there, when it can be reached
// from this segment: `by_va` is {address: (name, segment or none, near)}.
struct Callee {
    std::string name;
    std::optional<std::int64_t> seg;
    bool near = false;
};
std::string named(std::string_view text, std::int64_t seg, const std::map<std::int64_t, Callee>& by_va);

}  // namespace cb::engine::win16
