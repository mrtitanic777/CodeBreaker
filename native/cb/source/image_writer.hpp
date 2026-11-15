// cb/source/image_writer.hpp -- what the writers of a program image share: a cartridge, a ROM, a tape block.
//
// Every one of these programs comes back the same way. Code is what the program's own flow reaches from its entry points; every
// byte it reaches is written as an instruction, every other byte as data. Function starts keep their entry point's name or are
// `sub_<address>`, jump targets `L_<address>`, and an operand naming the start of a line gets that line's label. The source is then
// assembled and the result compared with the original byte for byte.
//
// What differs from one machine to the next -- its processor, the assembler's spelling, how the image lies in memory, which hardware
// has names -- is what a writer says, through two interfaces it implements for its instruction type `Insn`:
//
//     CodeModel<Insn>   how flow runs through the program: decode, where code may stand, what an instruction jumps to
//     Listing<Insn>     how one stretch of the image is written: its bytes, where the processor sees them, the text of an
//                       instruction, what stands at a place of its own (a header field, the vectors)
//
// and the shared steps are then
//
//     Walked<Insn> w = walk(model, entries);                 the code flow reaches, its targets, the function starts
//     Functions functions = name_functions(w.starts, names, kSub);
//     Labels labels = make_labels(w.targets, names, line_start, kLocal);
//     Assembly out({"; head"}, "ca65", module);              the source as one L2 Module, built as it is written
//     emit(out, first, last, labels, functions, listing);    appends the lines of [first, last)
//     Written r = finish(folder, "game.s", text(out) + "\n", functions, code_bytes(w.code, model), image_size);
//     assembled(r, [&] { return assemble(...); }, original, built_path);
//
// The source is built as one L2 tree as it is written (translate/l2.hpp's Assembly: emit() makes each function a MachineFragment,
// a writer's part() each bank or segment); the assembly back end writes the fragments joined by a line break, so the text is the
// lines as they were laid out.
//
// The x86 files written whole (a 32-bit Windows program, a 16-bit one, a driver) are found out in rounds against NASM instead: see
// source/flat_rounds.hpp.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/translate/l2.hpp"

namespace cb::source::image_writer {

// Names by address: what an entry point is called, a label's name. Looked up, and walked in address order when labels are made.
using Names = std::map<std::uint32_t, std::string>;
// The function starts and their names, in address order (what a Written result lists).
using Functions = std::map<std::uint32_t, std::string>;
// Whether a line of the source can begin at an address.
using LineStart = std::function<bool(std::uint32_t)>;
// Writes out the data bytes gathered so far as lines; handed to Listing::special.
using Flush = std::function<void()>;

// How an address is spelled in a made-up name: a prefix and a fixed number of upper-case hex digits (`sub_0150`, `L_0200`).
struct Spelling {
    std::string_view prefix;
    unsigned digits = 4;
    std::string operator()(std::uint32_t address) const;
};

// ---- how flow runs through the program -----------------------------------------------------------------------------

template <class Insn>
class CodeModel {
public:
    virtual ~CodeModel() = default;

    // The instruction at `pc`, or nothing where the bytes are not one.
    virtual std::optional<Insn> decode(std::uint32_t pc) const = 0;
    virtual std::uint32_t size(const Insn& insn) const = 0;
    // Where a jump, call or branch goes.
    virtual std::optional<std::uint32_t> target(const Insn& insn) const = 0;
    // May code stand at `pc`?
    virtual bool inside(std::uint32_t pc) const = 0;
    // Does the whole instruction lie in the program?
    virtual bool fits(std::uint32_t pc, const Insn& insn) const = 0;
    // Is its target a place flow goes?
    virtual bool follows(const Insn& insn) const = 0;
    // Does flow not go on to the next instruction?
    virtual bool stops(const Insn& insn) const = 0;
    // Where the instruction at `pc` is filed in the code (a ROM with banks files it by its place in the file).
    virtual std::uint32_t key(std::uint32_t pc) const { return pc; }
    // A model that finds functions by what is called: is the target of this instruction a function's start?
    virtual bool finds_functions() const { return false; }
    virtual bool starts_function(const Insn&) const { return false; }
};

template <class Insn>
struct Walked {
    std::map<std::uint32_t, Insn> code;    // by key
    std::set<std::uint32_t> targets;       // every place a jump goes
    std::vector<std::uint32_t> starts;     // the entries, and with finds_functions() every called target too, ascending
};

// What flow reaches from `entries`.
template <class Insn>
Walked<Insn> walk(const CodeModel<Insn>& model, std::span<const std::uint32_t> entries) {
    std::set<std::uint32_t> found(entries.begin(), entries.end());
    Walked<Insn> out;
    std::vector<std::uint32_t> todo(found.begin(), found.end());
    std::set<std::uint32_t> seen;
    while (!todo.empty()) {
        std::uint32_t pc = todo.back();
        todo.pop_back();
        while (model.inside(pc) && !seen.contains(pc)) {
            seen.insert(pc);
            std::optional<Insn> insn = model.decode(pc);
            if (!insn || !model.fits(pc, *insn)) break;
            const std::optional<std::uint32_t> to = model.target(*insn);
            if (to && model.follows(*insn)) {
                out.targets.insert(*to);
                todo.push_back(*to);
                if (model.finds_functions() && model.starts_function(*insn)) found.insert(*to);
            }
            const bool stop = model.stops(*insn);
            const std::uint32_t size = model.size(*insn);
            out.code.insert_or_assign(model.key(pc), std::move(*insn));
            if (stop) break;
            pc += size;
        }
    }
    if (model.finds_functions()) out.starts.assign(found.begin(), found.end());
    else out.starts.assign(entries.begin(), entries.end());
    return out;
}

// The positions inside an instruction, past its first byte: no line starts there.
template <class Insn>
std::set<std::uint32_t> covered(const std::map<std::uint32_t, Insn>& code, const CodeModel<Insn>& model) {
    std::set<std::uint32_t> inside;
    for (const auto& [at, insn] : code) {
        const std::uint32_t size = model.size(insn);
        for (std::uint32_t a = at + 1; a < at + size; ++a) inside.insert(a);
    }
    return inside;
}

// The bytes the code takes up.
template <class Insn>
std::size_t code_bytes(const std::map<std::uint32_t, Insn>& code, const CodeModel<Insn>& model) {
    std::size_t total = 0;
    for (const auto& entry : code) total += model.size(entry.second);
    return total;
}

// ---- names ------------------------------------------------------------------------------------------------------

// {start: name}, each function named from `names` or by `spelling` (a name made up is added to `names`).
Functions name_functions(std::span<const std::uint32_t> starts, Names& names, const Spelling& spelling);

// {address: label} for each flow target, and each named address, where a line starts.
Names make_labels(const std::set<std::uint32_t>& targets, const Names& names, const LineStart& line_start, const Spelling& spelling);

// ---- the source -------------------------------------------------------------------------------------------------

// How a machine's assemblers say "bytes" and "this many of one value".
struct DataSyntax {
    std::string bytes = ".DB";
    std::string fill = ".DSB";        // `.DSB 32,$00`
    std::string fill_separator = ",";
};

// Bytes as source lines: rows of 16, runs of four or more printable characters as a string, and a run of 16 or more of one value
// as one fill line.
std::vector<std::string> data_lines(ByteView data, const DataSyntax& syntax = {});

// `lines` with `what` as a comment on the first.
std::vector<std::string> commented(std::vector<std::string> lines, std::string_view what);

// The source `out` holds, as the assembly back end writes it.
std::string text(const translate::l2::Assembly& out);

// ---- one stretch of the image, written --------------------------------------------------------------------------

template <class Insn>
class Listing {
public:
    virtual ~Listing() = default;

    // The byte at a file offset, and the address the processor sees it at (labels and functions are by address).
    virtual std::uint8_t byte(std::uint32_t offset) const = 0;
    virtual std::uint32_t address(std::uint32_t offset) const = 0;
    // The instruction to write at the offset, or nullptr for data.
    virtual const Insn* code_at(std::uint32_t offset, std::uint32_t address) const = 0;
    virtual std::uint32_t size(const Insn& insn) const = 0;
    virtual std::string text(const Insn& insn) const = 0;
    virtual std::vector<std::string> data_lines(ByteView data) const = 0;
    // What stands at a place of its own (a header field, the vectors): write it (after `flush`) and say how many bytes it took,
    // or answer 0.
    virtual std::uint32_t special(std::uint32_t /*offset*/, std::uint32_t /*address*/, const Flush& /*flush*/) { return 0; }
    // The line that defines a label: `name:` unless the assembler wants another spelling (dasm: the name alone).
    virtual std::string label_line(const std::string& name) const { return name + ":"; }
};

// Append the lines for the image's bytes [start, end) to `out`.
template <class Insn>
void emit(translate::l2::Assembly& out, std::uint32_t start, std::uint32_t end, const Names& labels, const Functions& functions,
          Listing<Insn>& listing) {
    std::vector<std::uint8_t> data;
    const Flush flush = [&] {
        if (data.empty()) return;
        out.extend(listing.data_lines(data));
        data.clear();
    };
    std::uint32_t offset = start;
    while (offset < end) {
        const std::uint32_t addr = listing.address(offset);
        if (const std::uint32_t took = listing.special(offset, addr, flush)) {
            offset += took;
            continue;
        }
        if (const auto label = labels.find(addr); label != labels.end()) {
            flush();
            if (const auto fn = functions.find(addr); fn != functions.end()) {
                out.part(fn->second, "function", translate::ir::OriginVa{out.module().value_or(""), addr});   // each function a fragment
                out.append("");
            }
            out.append(listing.label_line(label->second));
        }
        if (const Insn* insn = listing.code_at(offset, addr)) {
            flush();
            out.append("        " + listing.text(*insn));
            offset += listing.size(*insn);
        } else {
            data.push_back(listing.byte(offset));
            if (labels.contains(addr + 1)) flush();
            ++offset;
        }
    }
    flush();
}

// ---- the result -------------------------------------------------------------------------------------------------

// What every writer gives back.
struct Written {
    std::filesystem::path source;                  // the file written
    Functions functions;
    std::vector<std::string> notes;                // why it did not rebuild, if it did not
    bool linked = false;                           // the source assembled
    bool same = false;                             // ... and the image it built is the original's
    std::size_t code_bytes = 0;                    // bytes written as instructions
    std::size_t image_bytes = 0;                   // the image's size (ROM, PRG, the block)
    std::optional<std::filesystem::path> built;    // where the rebuilt image was kept
    std::optional<std::filesystem::path> config;   // a linker configuration written beside the source (NES)
    std::optional<Bytes> built_image;              // the rebuilt block, for a writer whose caller wants it (Spectrum, MSX)
};

// Write the source to <folder>/<name> (line ends as they are) and begin the result.
Written finish(const std::filesystem::path& folder, std::string_view name, std::string_view source, Functions functions,
               std::size_t code_bytes, std::size_t image_bytes);

// Assemble with `build` (it throws a ParseError, as the assemblers do, for source that does not assemble), keep what came out at
// `built_path`, and say in `r` whether it is the original; `same`, when given, is that test. Gives the bytes built, or nothing.
std::optional<Bytes> assembled(Written& r, const std::function<Bytes()>& build, ByteView original,
                               const std::optional<std::filesystem::path>& built_path = std::nullopt, std::string_view what = "image",
                               const std::function<bool(ByteView)>& same = nullptr);

// What the command line of a writer prints about its result, and the exit code: 0 when the rebuilt image is the original.
//
//     game.nes: 3 functions, 120 of 32768 PRG bytes reached as code -> game.s
//       rebuilt: identical to the original
struct Report {
    std::string text;
    int exit_code = 0;
};
Report report(const std::filesystem::path& program, const Written& r, std::string_view unit = "ROM bytes");

}  // namespace cb::source::image_writer
