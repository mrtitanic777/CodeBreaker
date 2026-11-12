// cb/translate/backends/module_file.hpp -- L2 to the source file people read: a module's functions, each under its
// status, with the status of the whole at the head.
//
// The back end source/module_writer writes through. A module is one L2 tree (docs/IR_UNIFICATION.md): for C, a Module
// whose units are its functions in address order --
//
//     MethodDecl      a function with a reconstruction: `text` its C, `comments` [its status line]
//     Comment         a function with none: its status line alone
//
// -- and for a driver or a real-mode program a Module of MachineFragments (its whole source), which the assembly back
// end (backends/asm.hpp) writes. What is said at the head is not the tree's but the binary's: the per-binary Facts the
// function store and the build give.
//
//     CB_TRY_ASSIGN(auto parts, module_file::parts(module_node));         // the C module's functions
//     auto files = module_file::c(parts, facts, /*split=*/0);             // <stem>.c, or <stem>_1.c ... when split
//     CB_TRY_ASSIGN(auto source, module_file::source_units(asm_module));  // a driver's assembly
//     CB_TRY_ASSIGN(auto text, module_file::assembly_file(source, facts));
//
// A node of a kind this back end does not write is declined (assembly::Declined), as the assembly back end declines.
#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/translate/backends/asm.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::backends::module_file {

// The lane that produced the module; only PowerPC changes what the head says (its compiler and its interpreter).
enum class Lane { Pe, Ne, PowerPc, Le, Dos };

// A status class (verified, weak, unverified ...) in the order the head counts them: its title and its function count.
struct Status {
    std::string title;
    std::size_t count = 0;
};

// How the whole module built.
struct Whole {
    bool linked = false;
    std::size_t exports_same = 0;   // exports that match the original's
    std::size_t exports_total = 0;
    std::string error;              // why it did not link
    std::size_t rejected = 0;       // functions the compiler rejected (stand-ins that return 0)
    std::size_t absent = 0;         // calls to code with no reconstruction
    std::string built;              // the built image's path
};

// A function that is not verified, in the assembly file's head.
struct Row {
    std::string address;
    std::string name;
    std::string title;
    std::string what;
};

// The per-binary facts the head says.
struct Facts {
    std::string name;               // the module's file name
    std::string stem;               // and its stem
    std::string date;               // when it was generated
    std::string digest;             // under which verifier settings
    std::vector<Status> statuses;
    std::size_t total = 0;          // all the functions (the share each status is of)
    Lane lane = Lane::Pe;
    std::optional<Whole> whole;     // the whole module's build, when it was tried
    std::size_t real = 0;           // imports by number written with their real names
    bool header = false;            // whether <stem>.h is written (the C file includes it)
    std::string how;                // (assembly) what the judge said of the module as a whole
    std::vector<Row> rows;          // (assembly) each function not verified
};

// One function's part of the C file: a function with a reconstruction has its text; one with none is its status alone.
struct Part {
    std::string status;
    std::optional<std::string> text;
};

// A file of the module.
struct File {
    std::string name;
    std::string text;
};

// The functions of a C module (a Module node); Declined for another document or another kind of unit.
Result<std::vector<Part>> parts(const ir::Node* module);

// The head of the C file: the status, how the module builds, what the labels mean.
std::vector<std::string> banner(const Facts& facts);

// One function's part of the file: its status line as a comment, then its C.
std::string part_text(const Part& part);

// The module's C, as files: one, or parts of at most `split` functions each (the status in the first); `split` 0: one.
std::vector<File> c(std::span<const Part> parts, const Facts& facts, std::size_t split = 0);

// The units a driver's or real-mode program's assembly is written from (a Module node).
Result<std::vector<assembly::Unit>> source_units(const ir::Node* module);

// The head of the assembly file, as comment lines (without the comment lead).
std::vector<std::string> asm_banner(const Facts& facts);

// The one assembly file: the status at its head as comments, then the source (`; (no source could be written)` when it
// has none).
Result<std::string> assembly_file(std::span<const assembly::Unit> source, const Facts& facts);

}  // namespace cb::translate::backends::module_file
