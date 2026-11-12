// cb/java/probe.hpp -- run a Java class and its C++ translation on the same inputs and compare what they do.
//
//     probe::Toolchain tools{.cxx = "g++", .javac = "javac", .java = "java"};
//     probe::stage_sources(java_dir, staged_dir);
//     probe::SourceTree tree(staged_dir);
//     probe::RunResult result = probe::run(tree, cpp_dir, *tree.lookup("MathHelper"), work, tools);
//
// The behavioural half of the java kind's judge (verify/lanes/java). For a class, every method it can call is called on a fixed,
// deterministic set of inputs, in the JVM and natively, and what comes back compared:
//
//   * what it can pass: primitives, Strings, and objects of classes it can make -- a class is made with its first non-private
//     constructor, else a static factory of the class returning it, whose parameters are primitives or Strings (or the implicit
//     constructor). The maker's arguments come from the same input set.
//   * what it compares: a primitive or String result; an object result by its fields (the non-private, non-static primitive and
//     String fields of the declared class and its ancestors); and for a void instance method, the object's fields after the call.
//     Integers by value, floats and doubles by their bits (any NaN equals any NaN), Strings by their bytes. A null String in Java
//     is the empty string here -- the port's model of a String is a value -- and they compare alike.
//   * the original: the class's Java source compiled by javac (-sourcepath: it pulls in only what the harness needs) and run on the
//     JVM -- twice: a method any of whose results changes between the two runs (a Random seeded from the clock, the time) is left
//     out, it says nothing about the translation.
//   * the translation: a generated harness linked against the whole emitted program (every .cpp compiled once per module, into an
//     archive the linker takes what it needs from), run natively after the linked classes' static initialisers in the order
//     j2x_init.cpp gives.
//
// An input on which the Java side throws is not run on the C++ side (Java's exception is C++'s undefined behaviour) and not counted.
// A class is judged on the methods it could run and says how many those were.
//
// What a run could not do is an answer (RunResult::why), not an exception: a class whose harness does not compile or link is a
// verdict of its own. Errors that stay errors: IoError for a folder or file that cannot be read or written, ToolError if a tool
// cannot be started at all.
#pragma once

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#include "cb/java/ir.hpp"

namespace cb::java::probe {

// The judge's cache keys carry this: it changes when what a probe says about a class can change.
inline constexpr int kRevision = 11;

// The tools and the settings a probe runs with.
struct Toolchain {
    std::filesystem::path cxx;                       // the C++ compiler; its sibling `ar` makes the archive
    std::filesystem::path javac;
    std::filesystem::path java;
    std::string cxx_std = "c++03";                   // the dialect the program and harnesses compile as ("c++17" for the C++17 back end)
    std::optional<std::filesystem::path> prefix;     // a header every compiled unit includes first
    std::vector<std::filesystem::path> libraries;    // the jars the game is compiled and run against (LWJGL, the sound system)
    std::filesystem::path shim_sources;              // stand-ins for classes no library download has (`probe_shims/`); empty for none
    std::chrono::seconds run_timeout{60};            // one run of a harness
    std::chrono::seconds step_timeout{180};          // one compile or link
};

// The `.jar` files of a folder, sorted by name (none if the folder does not exist).
std::vector<std::filesystem::path> list_libraries(const std::filesystem::path& folder);

// ---- the sources -----------------------------------------------------------------------------------------------------------

// The .java files of a tree laid out by package: javac's -sourcepath needs net/minecraft/src/X.java where a decompiled tree is often
// flat. Files are copied (the text made valid UTF-8), one per class name (the first of a name wins, in name order). Returns `dest`.
std::filesystem::path stage_sources(const std::filesystem::path& java_dir, const std::filesystem::path& dest);

// A staged source tree: which file declares each class, and each class parsed once. Safe to share between threads.
class SourceTree {
public:
    explicit SourceTree(std::filesystem::path dir);   // indexes the folder (a class name declared twice keeps the first path in path order)

    const std::filesystem::path& dir() const { return dir_; }
    // The parsed class, or nullptr if the tree has no such class or its source does not parse. The pointer stays valid as long
    // as the tree does.
    const ir::JClass* lookup(std::string_view name) const;

private:
    std::filesystem::path dir_;
    std::map<std::string, std::filesystem::path, std::less<>> index_;
    mutable std::mutex mutex_;
    mutable std::unordered_map<std::string, std::unique_ptr<ir::JClass>> parsed_;   // nullptr: tried and failed
};

// ---- what can be run ------------------------------------------------------------------------------------------------------

// How an instance of a class is made: `new C(params)` when `factory` is empty, else `C.factory(params)`; the parameters are
// primitive or String type names.
struct Maker {
    std::optional<std::string> factory;
    std::vector<std::string> params;
};
// How an instance of `cls` is made, or nullopt if it cannot be (abstract, an interface, an enum, or constructors needing objects).
std::optional<Maker> maker(const ir::JClass* cls);

// A parameter: a primitive or String type name, or an object of a class and how it is made.
struct ObjectSlot {
    std::string cls;
    Maker made;
};
using Slot = std::variant<std::string, ObjectSlot>;

// [(name, java type)]: the state a class is compared by: its own and its ancestors' non-static, non-private primitive and String
// fields that the emitted header declares under that name.
using FieldList = std::vector<std::pair<std::string, std::string>>;
FieldList comparable_fields(const SourceTree& tree, const ir::JClass& cls, const std::filesystem::path& cpp_dir);

// How a call's outcome is compared.
struct Outcome {
    enum class Kind {
        Primitive,   // a primitive or String result: `type` names it
        Object,      // an object result: by `fields` of class `cls`
        Self,        // a void instance method: the object's `fields` afterwards
    };
    Kind kind = Kind::Primitive;
    std::string type;
    std::string cls;
    FieldList fields;
};

// One method the harness can call: its name, input slots, how its outcome is compared and how its object is made (nullopt for a
// static method).
struct Probeable {
    std::string name;
    std::vector<Slot> slots;
    Outcome outcome;
    std::optional<Maker> made;
};

// The methods of `cls` a harness can call: present under their name in the emitted header (`header_text`), every parameter passable,
// the outcome comparable. A method that reads the clock is left out (its result differs between any two runs).
std::vector<Probeable> probeable(const SourceTree& tree, const ir::JClass& cls, std::string_view header_text, const std::filesystem::path& cpp_dir);

// The methods that read the clock or the system clipboard (what the machine holds, not what the code does), or call one that does;
// and whether the constructors read it.
struct ClockReads {
    std::vector<std::string> methods;
    bool constructors = false;
    bool any() const { return constructors || !methods.empty(); }
};
ClockReads reads_clock(const ir::JClass& cls);

// How many methods a full judgement would have to run (every non-abstract method and constructor).
std::size_t counted(const ir::JClass& cls);

// ---- inputs -----------------------------------------------------------------------------------------------------------------

// The deterministic input tuples for a list of primitive/String type names: every combination, or -- for several -- a fixed sample of
// at most 48 of them, each input of each type used. Each input is a Java/C++ literal text (see java_literal and cpp_literal).
std::vector<std::vector<std::string>> cases(const std::vector<std::string>& types);

// An input as a literal of the language: `type` is a primitive or String name from the input table.
std::string java_literal(std::string_view type, std::string_view value);
std::string cpp_literal(std::string_view type, std::string_view value);

// ---- comparing --------------------------------------------------------------------------------------------------------------

// The "key|value" lines a harness prints, as {key: value}.
std::map<std::string, std::string> parse_results(std::string_view text);
// Do two printed results agree? Floats and doubles by bits, any NaN equal to any NaN.
bool same(std::string_view type, std::string_view a, std::string_view b);

// ---- harnesses --------------------------------------------------------------------------------------------------------------

// The Java harness class `harness_name`, in the class's own package (so package-private members can be reached). `only`: the case
// keys ("method.case") to run (all of them when null).
std::string java_harness(std::string_view cls_name, const std::vector<Probeable>& methods, const std::optional<std::string>& package,
                         std::string_view harness_name, const std::set<std::string>* only = nullptr);

// The C++ side for the cases in `keep` (the keys the JVM answered on). `init_order`: classes whose initStatics() run first, in order.
// `ancestry`: (class, base) pairs registered for instanceof first.
std::string cpp_harness(std::string_view cls_name, const std::vector<Probeable>& methods, const std::set<std::string>& keep,
                        const std::vector<std::string>& init_order, const std::set<std::string>& extra_headers,
                        const std::vector<std::pair<std::string, std::string>>& ancestry);

// (class, base) for every class of the program, from j2x_rtti.cpp; the classes in the order j2x_init.cpp runs their static initialisers.
std::vector<std::pair<std::string, std::string>> read_ancestry(const std::filesystem::path& cpp_dir);
std::vector<std::string> read_init_order(const std::filesystem::path& cpp_dir);

// ---- building and running --------------------------------------------------------------------------------------------------

// The whole emitted program as one archive in `work/probe/libj2x.a`, every .cpp compiled once (side by side): the harnesses link
// against it and the linker takes what each needs. A unit that does not compile is left out (its class is "does not compile"
// anyway). nullopt if the archive cannot be made. `jobs` 0 means cb::parallel::workers().
std::optional<std::filesystem::path> library(const Toolchain& tools, const std::filesystem::path& cpp_dir, const std::filesystem::path& work,
                                             std::size_t jobs = 0);

struct NativeBuild {
    bool ok = false;
    std::string why;                    // what went wrong, short
    std::vector<std::string> linked;    // the classes the link took from the archive
};
// Compile the harness and link it against the program's archive.
NativeBuild build_native(const Toolchain& tools, const std::filesystem::path& cpp_dir, const std::filesystem::path& harness,
                         const std::filesystem::path& exe, const std::filesystem::path& lib);

// Compile the stand-ins of `tools.shim_sources` (classes the game was built against in a form no library download has) against the
// libraries into `work/probe/_shims`, once. The folder, or nullopt when there are no libraries or stand-ins, or they did not compile.
std::optional<std::filesystem::path> compile_shims(const std::filesystem::path& work, const Toolchain& tools);

// The classes a probe has something to run for.
struct ClassPlan {
    const ir::JClass* cls = nullptr;
    std::vector<Probeable> methods;
};
std::vector<ClassPlan> plan(const SourceTree& tree, const std::filesystem::path& cpp_dir, const std::vector<std::string>& names);

// A class's Java harness once compiled: the folder of class files and the class to run -- or why it did not compile.
struct CompiledHarness {
    std::filesystem::path classes_dir;
    std::string main_class;
    std::string why;
    bool ok() const { return why.empty(); }
};
using Compiled = std::map<std::string, CompiledHarness, std::less<>>;
// Compile the Java harnesses of `todo` in batches of `batch` (javac's start and the classes the harnesses share are paid once a batch,
// not once a class), the batches side by side. A batch javac rejects is split in half and tried again, so a class whose source cannot
// compile (LWJGL not on the classpath, say) fails alone. `jobs` 0 means cb::parallel::workers().
Compiled compile_java(const SourceTree& tree, const std::vector<ClassPlan>& todo, const std::filesystem::path& work, const Toolchain& tools,
                      std::size_t batch = 40, std::size_t jobs = 0, std::string_view tag = "batch");

struct Difference {
    std::string method;
    std::vector<std::string> inputs;
    std::string java;
    std::string native;
};
struct RunResult {
    std::vector<std::string> methods;   // the methods it could call
    std::size_t judged = 0;             // how many of them ran on at least one input
    std::size_t total = 0;              // how many a full judgement would have to run (counted())
    std::size_t cases = 0;
    std::vector<Difference> differs;
    std::string why;                    // why nothing was judged, when that is so
    std::size_t unsteady = 0;           // cases left out because Java gave different results between two runs
};
// Judge one class. `compiled`: compile_java's answer for the module, when the harnesses were compiled in batches beforehand.
RunResult run(const SourceTree& tree, const std::filesystem::path& cpp_dir, const ir::JClass& cls, const std::filesystem::path& work,
              const Toolchain& tools, const Compiled* compiled = nullptr);

}  // namespace cb::java::probe
