// cb/build/asm16.hpp -- a 16-bit function the C lane cannot give as assembly, written so that it links into the rebuild.
//
//     asm16::Function f(module, {.seg = 3, .start = 0x1c0, .end = 0x262, .publics = {{0x1c0, "_f3_1c0"}}}, symbol_for, decoders);
//     if (f.text()) write_file(f.assembly());       // else f.why() says why this function stays C
//     std::optional<std::string> bad = asm16::check(object_bytes, f);   // nullopt: the assembled object is the original's bytes
//
// The 16-bit system modules were largely written in assembly, and some of their functions say things C cannot: they
// move the stack, they take a software interrupt with the registers set up before it, they read a segment the caller
// left in ES. When the decompiler's C for one does not compile, this writes it instead as what it is -- its own
// instructions -- in a form that links into the rebuilt module wherever the linker puts it:
//
//   * every place the loader fixes (the segment's relocations: a selector, an offset, a far pointer; to a function of
//     the module, its data segment, or an import) is written as the symbol it refers to, so the linker fixes it for
//     where things are now;
//   * a near call or jump that leaves the function (no relocation exists for those: the target was at a known
//     distance in the original segment) is written as `call`/`jmp` to the target's symbol;
//   * everything else is the original's bytes.
//
// A function that refers to its own segment by number (a jump table read through CS, a short jump out of itself)
// depends on where it sits, and is refused: its C stand-in remains -- unless what it reads is the segment's lead-in
// (the bytes before its first function, often a variable such as the data segment's selector), which the build
// writes the same way and links first into the segment, so it stays where it was.
//
// A Function is made whole by its constructor and is read afterwards; `write` and `raw_calls_into_holes` are the two
// calls that go on working (they may name more symbols), so they are not const. Maps here are ordered by offset or
// name: the order is what makes the text the same from one run to the next.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/ne.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/source/assembly.hpp"

namespace cb::build::asm16 {

// An import is named by its ordinal or its name.
using Which = std::variant<std::uint16_t, std::string>;

// What the build is asked to name (the answers of `SymbolFor`).
struct FunctionRef {   // the function that begins at seg:off (a segment number counts from 1)
    std::uint16_t seg = 0;
    std::uint32_t off = 0;
};
struct SegmentRef {    // any symbol in that code segment
    std::uint16_t seg = 0;
};
struct DataRef {       // a data segment's first byte
    std::uint16_t seg = 0;
};
struct ImportRef {     // an exported ordinal or name of another module (nullopt module: the reference is to none the file names)
    std::optional<std::string> module;
    Which which = std::uint16_t{0};
};
using Reference = std::variant<FunctionRef, SegmentRef, DataRef, ImportRef>;
// The symbol for a reference, or nullopt when nothing has one (the function is then refused).
using SymbolFor = std::function<std::optional<std::string>(const Reference&)>;

struct PublicSymbol {   // a function of the run: where it begins and the symbol that is defined there
    std::uint32_t offset = 0;
    std::string symbol;
    friend auto operator<=>(const PublicSymbol&, const PublicSymbol&) = default;
};
struct Hole {           // a function inside the run that is rebuilt from C: the build places it after the run
    std::uint32_t start = 0, end = 0;
    std::string symbol;
    friend auto operator<=>(const Hole&, const Hole&) = default;
};

// What to write. [start, end) of segment `seg` (counted from 1) is one function, or a run of functions written
// together as their source file had them; `publics` gives the symbol of each (at least one).
struct Request {
    std::uint16_t seg = 0;
    std::uint32_t start = 0, end = 0;
    std::vector<PublicSymbol> publics;
    // Offsets of the segment below this may be read through CS: the segment's lead-in, which the build keeps where it was.
    std::uint32_t cs_ok = 0;
    // Other places of the run that are called into from outside (each must also be among `publics`).
    std::vector<std::uint32_t> extra_entries;
    // The run is a whole segment, which the build keeps where it was: any place of it may be read through CS by its number.
    bool whole = false;
    // Functions inside it rebuilt from C: their bytes here are filler, and a call to one is a call by its name.
    std::vector<Hole> holes;
};

enum class Linkage { Near, Far, Byte };   // how an external symbol is declared

struct Import {
    std::string module;
    Which which = std::uint16_t{0};
};

// A piece of the source that stands for some of the original's bytes: `size` bytes, written as `text`.
struct Directive {
    std::uint32_t size = 0;
    std::string text;
};

// One instruction of the original, as the decoder reads it.
struct Insn {
    Bytes bytes;
    std::string mnemonic;
    std::string operands;
    std::optional<std::uint32_t> target;   // where a jump inside the run goes
};

// A far or near call or jump in a not-followed place that goes into a function rebuilt from C: its symbol and its opcode.
struct HoleCall {
    std::string symbol;
    std::uint8_t opcode = 0;
};

class Function {
public:
    // Reads the code from the entry points and works out the source. A function that cannot be written this way
    // leaves text() empty and says why. UsageError when `seg` is not a segment of the module or `publics` is empty.
    // `decoders` is the calling thread's (an engine is not shared between threads).
    Function(const formats::ne::Module& module, const Request& request, const SymbolFor& symbol_for,
             platforms::DecoderSet& decoders);

    // ---- the result
    const std::optional<std::string>& text() const { return text_; }   // the assembly; empty when it cannot be written so
    const std::string& why() const { return why_; }                    // why text() is empty
    const std::map<std::string, Import>& imports() const { return imports_; }   // symbol -> the import it stands for
    const std::map<std::string, Linkage>& externs() const { return externs_; }
    const std::set<std::uint32_t>& missing() const { return missing_; }    // near-call targets without a symbol
    const std::set<std::uint32_t>& tables() const { return tables_; }      // places that are entries of a jump table
    const std::set<std::pair<std::uint16_t, std::uint32_t>>& far_missing() const { return far_missing_; }   // a far call or jump reaches mid-run
    const std::optional<std::uint32_t>& reaches() const { return reaches_; }   // refused for jumping into the middle of this offset's function
    const std::vector<Hole>& holes() const { return holes_; }              // sorted
    const std::map<std::uint32_t, HoleCall>& raw_hole_calls() const { return raw_hole_calls_; }   // offset -> the call (see raw_calls_into_holes)
    const std::set<std::pair<std::uint16_t, std::uint32_t>>& raw_far_entries() const { return raw_far_entries_; }   // (segment, offset) a raw far call or pushed pointer goes to
    const std::vector<PublicSymbol>& publics() const { return publics_; }   // sorted
    const std::map<std::uint32_t, std::uint32_t>& cs_labels() const { return cs_labels_; }   // instruction -> the place in the run it reads through CS

    // ---- what the build and check read (set once the code is followed; empty for a function refused before that)
    unsigned bits() const { return bits_; }   // 32 for a use32 segment (NE flag 0x2000)
    std::uint32_t start() const { return start_; }
    std::uint32_t end() const { return end_; }
    ByteView blob() const { return blob_; }   // the segment's bytes as the loader has them
    const std::map<std::uint16_t, std::uint32_t>& code_segments() const { return code_segments_; }   // segment -> bytes of code
    const std::map<std::uint32_t, Directive>& sites() const { return sites_; }       // each loader fix-up, written as its symbol
    const std::map<std::uint32_t, Insn>& insns() const { return ins_; }
    const std::map<std::uint32_t, Directive>& symbolic() const { return symbolic_; }   // an instruction whose fix-up has a name
    const std::map<std::uint32_t, Directive>& calls() const { return calls_; }         // near calls and jumps that leave the run
    const std::set<std::uint32_t>& labels() const { return labels_; }
    const std::string& module_name() const { return module_; }
    std::uint16_t seg() const { return seg_; }
    const std::string& first_public() const { return public_; }

    // The instruction at `at` as the assembler reads it. `at` must be in insns().
    std::string mnemonic(std::uint32_t at) const;
    // {offset: the text, the original's bytes, a mask of none} of the instructions the assembler's spelling is probed for.
    std::map<std::uint32_t, source::assembly::Candidate> candidates() const;
    // {symbol: offset} of each function rebuilt from C (a hole) that a near call or jump in the part of the frame NOT
    // followed as code -- written out as its bytes -- goes to (also fills raw_hole_calls). The bytes hold the
    // displacement to where the function was, and it is placed elsewhere (USER's 25:2460 `E8 59 FB`, call 1FBC, in
    // code reached only through a table). Opens a decoder of its own: it is called once per function, and only for one
    // with holes.
    std::map<std::string, std::uint32_t> raw_calls_into_holes();

    // The offsets of the instructions to write as mnemonics; the rest are written as bytes. nullopt: every instruction
    // (a symbolic one then stays as its bytes and its fix-up sites).
    using Good = std::optional<std::set<std::uint32_t>>;
    // The source. With `piece` only that part of it: one between two functions rebuilt from C, in a frame whose places
    // stay where they were -- what points out of the piece (a jump, a place read through CS, a table's entry) is
    // written as the original's bytes, which stay right.
    std::string write(const Good& good, std::optional<std::pair<std::uint32_t, std::uint32_t>> piece = std::nullopt);

private:
    void build(const formats::ne::Module& module, const Request& request, const SymbolFor& symbol_for, platforms::DecoderSet& decoders);
    void far_pointer_offsets(const platforms::Decoder& md, const SymbolFor& symbol_for,
                             const std::map<std::uint32_t, std::uint16_t>& selectors, std::uint16_t seg);
    void raw_far_pointers(const SymbolFor& symbol_for, const std::map<std::uint32_t, std::uint16_t>& internal_selector);
    std::optional<std::uint16_t> pointer_selector(const platforms::Decoder& md, std::uint32_t at, const Insn& ins,
                                                  const std::map<std::uint32_t, std::uint16_t>& selectors) const;
    bool stays_inside(std::uint32_t at, std::uint32_t lo, std::uint32_t hi) const;
    bool used(std::uint32_t label, std::uint32_t lo, std::uint32_t hi);
    std::uint16_t blob_u16(std::uint32_t at) const;

    std::optional<std::string> text_;
    std::string why_;
    std::map<std::string, Import> imports_;
    std::map<std::string, Linkage> externs_;
    std::set<std::uint32_t> missing_, tables_;
    std::set<std::pair<std::uint16_t, std::uint32_t>> far_missing_, raw_far_entries_;
    std::optional<std::uint32_t> reaches_;
    std::vector<Hole> holes_;
    std::map<std::uint32_t, HoleCall> raw_hole_calls_;
    std::vector<PublicSymbol> publics_;
    std::map<std::uint32_t, std::uint32_t> cs_labels_;
    unsigned bits_ = 16;
    std::uint32_t start_ = 0, end_ = 0;
    Bytes blob_;
    std::map<std::uint16_t, std::uint32_t> code_segments_;
    std::map<std::uint32_t, Directive> sites_, symbolic_, calls_;
    std::map<std::uint32_t, Insn> ins_;
    std::set<std::uint32_t> labels_;
    std::string module_, public_;
    std::uint16_t seg_ = 0;
    bool raw_scanned_ = false;
    std::optional<std::map<std::uint32_t, std::vector<std::uint32_t>>> refs_;   // who refers to each label (made once)
};

// A piece [lo, hi) of a frame (see Function::write) against the original: nullopt when every byte is the same but the
// ones the loader or the linker sets, else where they differ.
std::optional<std::string> check_piece(ByteView object, const Function& f, std::uint32_t lo, std::uint32_t hi);
// The assembled object against the original.
std::optional<std::string> check(ByteView object, const Function& f);
// The offsets (in the segment) where the assembled code is not the original's, apart from the places the loader or
// the linker sets aside. `mask` is non-zero at the bytes a fix-up record of the object names.
std::vector<std::uint32_t> differences(ByteView code, ByteView mask, const Function& f);

}  // namespace cb::build::asm16
