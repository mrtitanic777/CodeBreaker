// cb/translate/javajudge.hpp -- the Java corpus judged on each back end, beside the JVM (docs/TRANSLATOR_DESIGN.md, phases 4
// and 6).
//
// Two ways a Java program reaches native code, and a judge for each:
//
//   * through the transpiler (the java lane): declarations from its front end, every body from the IR by a body back end --
//     cpp03 (the 360's) or C++17. Judged class by class by the java lane and its probe: each class compiled on its own, its
//     methods run on the same inputs in the JVM and natively.                      judge_tree(lane, src, work, backend)
//
//   * whole programs from L2 (declarations and bodies), with a runtime of their own: the C++17 back end's program and the C
//     back end's. Judged per program: every static method with primitive or String parameters and a primitive or String
//     result is run on the probe's inputs in the JVM and natively, and the program's main is run on both sides and its
//     output compared.                                                           judge_program(host, src, work, backend)
//
// Verdicts: Equivalent (every result alike), Divergent (one differs, with the case), Refused (the back end declined -- a
// J2X-GAP body, a decline, or what it wrote does not compile), NotRun (nothing it could compare).
//
// What the judge needs of the rest of the program it is given, not reached for: the Java front end (javac's tree into L2), the
// probe's inputs and result reader, the whole-program builds, and the java lane. Each is a small interface of the host's
// (`ProgramHost`, `ProbeInputs`, `TreeLane`); the judge holds no state of its own, so two judgements may run at once.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/translate/dotnet/judge.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::javajudge {

enum class Verdict { Equivalent, Divergent, Refused, NotRun };

// "equivalent", "divergent", "refused", "not-run": the words reports use.
std::string_view word(Verdict verdict);

// What the java lane says of one class it judged.
enum class LaneVerdict {
    NoCompile,   // what the back end wrote does not compile
    Divergent,   // it ran and a result differs
    Good,        // it ran and every result is alike (verified, or as good as the lane counts)
    Other,       // anything else: inconclusive, not judged
};

// The verdict of a class from the lane's: no compile is a refusal, divergent is divergent, good is equivalent, the rest is
// not run.
Verdict verdict_of(LaneVerdict lane);

// ---- through the transpiler -----------------------------------------------------------------------------------------

// The body back ends the tree judge is run with.
enum class Backend { Cpp03, Cpp17 };
std::string_view backend_name(Backend backend);   // "cpp03", "cpp17"

// One class the lane judged: its verdict word, why, and the detail the lane gives.
struct LaneResult {
    LaneVerdict verdict = LaneVerdict::Other;
    std::string reason;
    std::string detail;
};

// The java lane, as the tree judge uses it. `judge` and `gaps` are called from several threads at once, each on another class.
class TreeLane {
public:
    virtual ~TreeLane() = default;
    // Convert the Java tree `src` with the body back end, as that language standard (the lane builds with it), and build what
    // judging needs; the classes it can judge, in the order the conversion made them.
    virtual std::vector<std::string> prepare(const std::filesystem::path& src, const std::filesystem::path& work, Backend backend) = 0;
    // The class judged. May throw: the judge says so and goes on.
    virtual LaneResult judge(const std::string& cls) = 0;
    // How many `J2X-GAP:` markers the written text of the class holds (the places the back end said it could not).
    virtual std::size_t gaps(const std::string& cls) = 0;
};

struct ClassJudgement {
    Verdict verdict = Verdict::NotRun;
    std::size_t gaps = 0;
    std::string detail;   // "<reason>: <detail, to 400 bytes>"
};

struct TreeOptions {
    std::optional<std::vector<std::string>> only;   // just these classes
    unsigned workers = 2;                           // classes judged at once
    std::function<void(std::string_view)> log;      // one line per step and per class; none: silent
};

// Every class of the Java tree `src` converted with `backend` and judged by `lane`: {class: its judgement}, in the order the
// lane made them.
OrderedMap<std::string, ClassJudgement> judge_tree(TreeLane& lane, const std::filesystem::path& src, const std::filesystem::path& work,
                                                   Backend backend, const TreeOptions& options = {});

// ---- whole programs from L2 -----------------------------------------------------------------------------------------

// The class the harness is: a program's own classes may not be called this.
inline constexpr std::string_view kHarness = "J2XProbe";

// The Java name of a primitive or String type ("int", "String" ...), else nothing.
std::optional<std::string_view> java_name(const ir::TypeP& type);

// A static method the harness can call: the class reference in Java ("Outer.Inner"), its name, its parameter and result types as
// Java names.
struct ProbeMethod {
    std::string owner;
    std::string name;
    std::vector<std::string> params;
    std::string result;
};

// The one package of the program's units ("" for the default package, or when they are in several).
std::string package_of(std::span<ir::Node* const> units);

// Every static method of a class of the program's package that is not private, is not main, is not generic, and whose
// parameters are primitives or Strings and whose result is one.
std::vector<ProbeMethod> probe_methods(std::span<ir::Node* const> units);

// The classes that have a static `main`.
std::vector<std::string> main_classes(std::span<ir::Node* const> units);

// The probe's inputs and how its output reads (the Java lane's probe): `cases` gives the argument values to try for these
// parameter types (each case one value per parameter, as text); `literal` writes a value as a Java literal of a type;
// `results` reads the harness's `key|value` lines; `same` says whether two results of a type are alike (floats by bits ...).
struct ProbeInputs {
    std::function<std::vector<std::vector<std::string>>(const std::vector<std::string>& param_types)> cases;
    std::function<std::string(const std::string& type, const std::string& value)> literal;
    std::function<OrderedMap<std::string, std::string>(std::string_view output)> results;
    std::function<bool(const std::string& type, const std::string& a, const std::string& b)> same;
};

// One call the harness makes: the method, the values it is given, the type of its result.
struct ProbeCase {
    std::string method;   // "Owner.name"
    std::vector<std::string> values;
    std::string result_type;
};

struct Harness {
    std::string source;                         // the Java source of J2XProbe
    OrderedMap<std::string, ProbeCase> cases;   // by key ("<method number>.<case number>"), as the harness prints it
};

// The harness for `methods`, on the probe's inputs.
Harness harness(std::span<const ProbeMethod> methods, std::string_view package, const ProbeInputs& probe);

// The Java sources under `src` made into one flat folder `dest` (made new).
void stage_sources(const std::filesystem::path& src, const std::filesystem::path& dest);

// A whole program built into a program that runs: `exe`, or why not.
struct Built {
    std::optional<std::filesystem::path> exe;
    std::string why;
};

// What judging a whole program needs of the rest of CodeBreaker.
struct ProgramHost {
    std::optional<dotnet::judge::Jdk> jdk;
    ProbeInputs probe;
    // The Java sources under `src` through javac's tree into L2 units made in `arena`; ToolError when javac reports an error
    // (its first message in the error). `work` is where its trees go.
    std::function<std::vector<ir::Node*>(Arena& arena, const std::filesystem::path& src, const std::filesystem::path& work)> lower;
    // The whole-program back end named `backend` ("cpp17" or "c") writing `units` and compiling them under `work`. A back end
    // that declines the program is a Built with no exe and the decline as why.
    std::function<Built(std::span<ir::Node* const> units, const std::filesystem::path& work, std::string_view backend)> build;
};

// One result of the JVM that the translation's does not match.
struct Difference {
    std::string method;
    std::vector<std::string> values;
    std::string java;     // the JVM's result
    std::string native;   // the translation's (or "no result (crashed?)")
};

// What a whole program's judgement found.
struct ProgramVerdict {
    Verdict verdict = Verdict::NotRun;
    std::size_t methods = 0;                           // distinct methods compared
    std::size_t cases = 0;                             // calls compared
    std::vector<Difference> differs;
    std::optional<OrderedMap<std::string, std::string>> main;   // per main class: "alike" or how it differs; none when the translation did not run
    std::string why;                                   // why there is no verdict
    std::string backend;
};

// A whole Java program (a folder of sources, default package) beside the JVM, translated by the whole-program back end
// `backend` ("cpp17" or "c"). Everything it makes goes under `work`.
ProgramVerdict judge_program(const ProgramHost& host, const std::filesystem::path& src, const std::filesystem::path& work,
                             std::string_view backend = "cpp17");

}  // namespace cb::translate::javajudge
