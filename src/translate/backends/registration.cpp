// translate/backends/registration.cpp -- every back end registers itself with the hub: the language it writes, the module that writes it, and a rule
// set for each source language it writes, with the proof of that pair. The file-level rule sets are the ones translate_file.hpp runs; the others are
// the routes the plan's survey lists (docs: work/unify/LANGUAGE_PLAN.md, section 1) and are judged by their own commands.
#include "translate/registry.hpp"

namespace cb::translate::registry {

// Java as C11 on the C runtime (backends/c_java).
void register_c_back_end(Registry& r) {
    r.add(BackEnd{"c",
                  "backends/c_java",
                  {{"java", "translate_file", "builds the C translation, runs it beside the Java program (javac and the JVM), and compares what they print and their exit code",
                    {"jdk", "cc"}, true}}});
}

// The C++03 back end writes a Java conversion's bodies; C# reaches it through the Java declaration skeleton (dotnet/cs2cpp).
void register_cpp03_back_end(Registry& r) {
    r.add(BackEnd{"cpp03",
                  "backends/cpp03",
                  {{"java", "javajudge", "Alpha and Beta classes judged alike, one by one, the Alpha 360 build warning-free", {"jdk", "cxx"}, false}}});
    r.add(BackEnd{"cpp03",
                  "dotnet/cs2cpp with backends/cpp03",
                  {{"csharp", "judge-cs-cpp", "the Shapes program judged equivalent; the rest is refused", {"dotnet", "cxx"}, false}}});
    r.add(BackEnd{"cpp17",
                  "backends/cpp03_cpp17 (Bodies17)",
                  {{"java", "javajudge", "the transpiler's Cpp17 dialect judged on the Alpha classes", {"jdk", "cxx"}, false}}});
}

// The L2 core library's C++17 writer: one emitter, a rule set per source language.
void register_cpp17_back_end(Registry& r) {
    r.add(BackEnd{"cpp17",
                  "l2 core cpp17",
                  {{"java", "javajudge", "five modern programs, 635 cases, each run as Java and as C++17", {"jdk", "cxx", "l2"}, false},
                   {"python", "judge-py-cpp",
                    "builds the C++17 translation, runs it beside the Python module (CPython), and compares the answer of every run_* function",
                    {"python", "cxx", "l2"}, true}}});
}

// C# as C# (and Java as C#): backends/csharp, with csharp_java for Java's library.
void register_csharp_back_end(Registry& r) {
    r.add(BackEnd{"csharp",
                  "backends/csharp",
                  {{"csharp", "judge-cs-cpp", "writes the program as C# again, runs both (csc and the installed .NET), and compares what they print and their exit code",
                    {"dotnet"}, true},
                   {"java", "javajudge", "four of four programs run alike as Java and as C#", {"jdk", "dotnet"}, false}}});
}

// C# as C++17 on its own runtime (csrt.hpp): the emitter the plan folds into the L2 core's C++17 writer.
void register_csharp_cpp17_back_end(Registry& r) {
    r.add(BackEnd{"cpp17",
                  "backends/csharp_cpp17",
                  {{"csharp", "judge-cs-cpp",
                    "builds the C++17 translation, runs it beside the C# program (csc and the installed .NET), and compares what they print and their exit code",
                    {"dotnet", "cxx"}, true}}});
}

// C (and C++, through the same clang front end) as Java 17.
void register_java_back_end(Registry& r) {
    r.add(BackEnd{"java",
                  "backends/java",
                  {{"c", "judge-c-java", "each corpus program run as C and as Java: 184 of 187 C programs equivalent; the C++ programs 7 of 9, two refused", {"cxx", "jdk"}, false}}});
}

}  // namespace cb::translate::registry
