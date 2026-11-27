// app/function_source.hpp -- the functions of a finished conversion, as the Source card's Functions, Check and Compare views show them: the list (what
// the route recorded, else what the output folder's source holds), each function's verdict where a judge has filed one, and one function's original
// disassembly beside its rebuilt source with the lines that correspond. No operating-system API: tested on every system (function_source_test.cpp).
//
//     auto list = collect(folder, input_file, outcome.functions, state);   // every function; verdicts only where something judged them
//     Pair pair = load_pair(folder, list[3]);                              // its .asm body, its .c body, the links between their lines
//     const Link* link = link_at(pair.links, Side::C, 2);                  // the original's lines that match the C line clicked
//
// Honesty. A function is "Not checked" unless a record or the function store says otherwise: nothing here guesses a verdict. The links between lines are
// only those that can be read from the two texts: a C line that calls a function by name corresponds to the assembly line that calls it. Where there
// are none the views show both whole bodies and only the clicked line.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "app/convert.hpp"

namespace cb::app::fnsrc {

// "0x00024" from "sub_00024" (the decompiler's naming of a function by its address); empty for any other name.
std::string address_of_name(std::string_view name);
// An address as the store and the records spell it, compared: "0x0011F0", "11f0" and "0x11F0" are one address ("0x" and leading zeros dropped, lower case).
std::string normal_address(std::string_view address);

// The functions the C source files of `folder` define (a definition at the start of a line, its `{` on that line or the next), in file order, every one
// NotChecked, its address taken from its name when that names one. Empty when the folder has no such source.
std::vector<convert::FunctionRecord> scan_folder(const std::filesystem::path& folder);

// The latest verdicts the function store holds for the program `input` (found by the SHA-256 of the file), by normal_address; Verified and Weak are
// Matches, Unverified is Differs, anything else is left out (not checked). Empty when there is no store, no such module or the file cannot be read.
std::map<std::string, convert::FunctionVerdict> stored_verdicts(const std::filesystem::path& input, const std::filesystem::path& state);

// The list the window shows: `recorded` when a route named the functions, else scan_folder(folder); a function that is NotChecked and whose address has
// a verdict in `verdicts` takes it. Names with no source file recorded get the file and line scan_folder finds.
std::vector<convert::FunctionRecord> collect(const std::filesystem::path& folder, const std::vector<convert::FunctionRecord>& recorded,
                                             const std::map<std::string, convert::FunctionVerdict>& verdicts = {});

// One function a judge ran (source/pipeline's judge): the lane's key (its address, as hex), the name it built it by, and the class it came out.
struct Judged {
    std::string key, name;
    verify::Class cls = verify::Class::NoSource;
};
// The list after a judge ran: each function of `found` (scan_folder of the C written) takes the class of the judged function of the same name, else of
// the same address (its verdict verdict_of that class, its address the judged one's when it had none); the judged functions none of `found` names are
// added after them, with no source file. A function of `found` the judge never saw stays NotChecked.
std::vector<convert::FunctionRecord> with_judged(std::vector<convert::FunctionRecord> found, const std::vector<Judged>& judged);

// The sentence a conversion's judging stage leaves (Outcome::checked) from the module's `total` functions and their counts by class (reconstruct's
// Outcome::scores): "205 functions judged in the win64 lane: 120 verified, 30 weak, 40 unverified, 10 do not compile, 5 no source" (also "N not run"
// and "N runtime library" when there are any). The total is the module's functions, as `check` gives it, not the sum of the classes: the runtime
// library's are filed apart from the classes and may overlap them.
std::string judged_counts(const std::map<verify::Class, std::int64_t>& scores, std::int64_t total, std::string_view lane);

// How many of each, and the share of all the functions that behave like the original (functions nobody checked count against it: the figure is what is
// known, not what is hoped).
struct Counts {
    int total = 0, matches = 0, differs = 0, unchecked = 0;
    std::map<verify::Class, int> classes;   // the functions a judge gave a class (FunctionRecord::judged), by class
    int classed() const;                    // how many of them
    int judged() const { return matches + differs; }
    // 0..100 of all the functions; -1 when nothing was judged
    int percent_alike() const { return judged() == 0 || total == 0 ? -1 : (matches * 100 + total / 2) / total; }
};
Counts count(const std::vector<convert::FunctionRecord>& list);

// Which functions the filter lets through: 0 all, 1 Differs, 2 Not checked. The indexes into `list`, in order.
std::vector<int> filtered(const std::vector<convert::FunctionRecord>& list, int filter);

// The lines of the function `name` in a C source: the signature to the closing brace; empty when it is not defined there. `first_line` (0-based, in the
// file) is set when found.
std::vector<std::string> c_body(const std::vector<std::string>& file_lines, std::string_view name, int* first_line = nullptr);
// The lines of the function `name` in an assembly source: from its label (`name:` or `name proc`) to the next label that is not a local one (L_..., .x),
// or its `endp`; empty when there is none.
std::vector<std::string> asm_body(const std::vector<std::string>& file_lines, std::string_view name);

// Lines that correspond: an inclusive range of the assembly and one of the C.
struct Link {
    int asm_first = 0, asm_last = 0, c_first = 0, c_last = 0;
    friend bool operator==(const Link&, const Link&) = default;
};
// A C line that calls `name(` and the assembly line `call name`, matched in order of appearance.
std::vector<Link> link_by_calls(const std::vector<std::string>& asm_lines, const std::vector<std::string>& c_lines);

enum class Side { Asm, C };
// The link that holds `line` on `side`; null when none does.
const Link* link_at(const std::vector<Link>& links, Side side, int line);

// One function, both sides.
struct Pair {
    std::vector<std::string> asm_lines, c_lines;
    std::vector<Link> links;
    std::string asm_note, c_note;      // why a side is empty ("No assembly for this function in the output folder."); empty when it is shown
};
// Reads the folder's .asm and the record's source file (else every .c) for the function. Never throws.
Pair load_pair(const std::filesystem::path& folder, const convert::FunctionRecord& record);

}  // namespace cb::app::fnsrc
