// cb/source/java_writer.hpp -- a Java program's C++ source in a folder, every class labelled with what is known.
//
//     Written w = write_java("minecraft", store, module, profile, out_dir, state_dir, convert, log);
//
// Reached through module_writer for a module of the java kind. The C++ is transpiled again into the folder (`convert`: the Java
// converter is deterministic, so the same source gives the same files the judge compiled), and each class's .cpp (or its .h, for a class
// with no .cpp) gets one status line at its head, read from the function store. STATUS.md lists every class with its status and the
// reason.
//
// A file in the folder that CodeBreaker did not write is never replaced: the whole tree then goes to the state folder instead.
//
// Line ends: a class file's line ends are left as the converter wrote them (no text-mode rewrite, which would make them
// CRLF on Windows); a file's head is judged on its first 600 bytes, not its first 600 characters.
#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/engine/function_store.hpp"
#include "cb/verify/verdicts.hpp"

namespace cb::source::java_writer {

// Transpile the program into a folder.
using Conversion = std::function<void(const std::filesystem::path& out_dir)>;
// One line of what was done (nothing given: silent).
using Log = std::function<void(std::string_view)>;

// What the store says of one class.
struct ClassRow {
    std::string name;
    verify::Class status = verify::Class::NoSource;
    std::string said;   // its first line of why, at most 200 bytes
};

struct Written {
    std::filesystem::path folder;           // where the C++ went
    std::vector<ClassRow> rows;             // every class the store knows, by name
    engine::ClassCount counts;              // by status; every class of verify::kClassOrder is a key
};

// The two lines stamped at the head of a class's file: `// Block.cpp -- transpiled from Java by CodeBreaker.` and its status.
std::string banner(std::string_view file, verify::Class status, std::string_view said);

// What the store says of each class of `module` under `profile`, by name: the status of its best candidate (no source for a class it
// has none for) and the first line of why -- the reason where it did not compile, else the detail, else the reason.
std::vector<ClassRow> class_rows(engine::FunctionStore& store, std::string_view module, std::string_view profile);

// STATUS.md: the classes with their status and the reason.
std::string status_report(std::string_view name, const std::vector<ClassRow>& rows, const engine::ClassCount& counts);

// Transpile into `out_dir` (`state_dir` when it holds .h or .cpp files nobody here wrote), stamp each class's file and write STATUS.md.
Written write_java(std::string_view name, engine::FunctionStore& store, std::string_view module, std::string_view profile,
                   const std::filesystem::path& out_dir, const std::filesystem::path& state_dir, const Conversion& convert, const Log& log = nullptr);

}  // namespace cb::source::java_writer
