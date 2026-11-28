// source/console_writer.hpp -- the one source writer for the consoles: a platform is a description (config/platforms/<name>.json), the
// writer reads it and writes the program through the shape it names (docs/SOURCE_WRITER.md).
//
//     image_writer::Written r = console_writer::reconstruct("coleco", "game.col", "out");
//     r.source                  out/game.asm, in the platform's dialect (the description's `dialect`: sjasmplus for the Z80 machines)
//     r.same                    the source, built, is the program byte for byte
//
// What a platform says, as data: its title, its shape (how the program lies in memory: `z80-block`, one block at one address; `banked`, a cartridge of banks; `cart6502`; `segmented`, ca65 segments an ld65 configuration places), its dialect,
// which knowledge tables name its hardware (config/knowledge/<platform>/registers.json: `ports` a hardware section for IN/OUT ports, `fixed`
// a table of fixed addresses outside the program -- BIOS or firmware entry points, system variables) and the head comment, one template per
// line over the values the platform's loader read from the file's header.
//
// What stays code is the loader (source/console_platforms.cpp): which bytes of the file are the program, where they sit, its entry points and
// the header's values by name. The file formats themselves are formats/<x>'s.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "core/bytes.hpp"
#include "source/image_writer.hpp"

namespace cb::formats::lynx {
struct Program;
}
namespace cb::formats::genesis {
struct Rom;
}

namespace cb::source::console_writer {

// A value a head template says: a number, a text, or a list of texts (a line with `each` is written once per element).
using Value = std::variant<std::int64_t, std::string, std::vector<std::string>>;
using Values = std::map<std::string, Value, std::less<>>;

// One line of the head comment: written when `when` is empty or names a value that is set, once per element of `each` when given (as `{it}`).
struct HeadLine {
    std::string text;
    std::string when;
    std::string each;
};

// ---- the banked shape's part of a description (docs/SOURCE_WRITER.md) ----
struct Range {
    std::uint32_t from = 0, to = 0;   // [from, to): a range of the address space that is data (a header), not code
};
struct HeaderField {
    std::uint32_t at = 0, size = 0;
    std::string what;                 // its name, said beside its data rows
};
struct VectorName {
    std::uint32_t at = 0;
    std::string name;
    bool always = false;              // named even when flow finds no function there
};
struct HardwareTable {
    std::string section;              // the knowledge hardware section (config/knowledge/<platform>/registers.json)
    std::string spelling;             // the dialect spelling of its names ("" the knowledge base's own)
    bool port = false;                // I/O ports (IN/OUT), else addresses the code reads and writes
    unsigned digits = 4;              // the hex digits of its `.equ` values
};
struct Variant {
    std::string when;                 // the value of `variant_by` it applies to ("Game Gear")
    std::vector<HardwareTable> hardware;   // more tables
    std::vector<std::uint32_t> drop_ports;
    image_writer::Names add_ports;
    bool constants = true;            // false: the constant tables do not apply (no such hardware)
};
struct Banked {
    std::string cpu;                  // "z80", "sm83"
    std::string assembler;            // the L2 assembly's syntax ("rgbds", "wla-z80")
    std::uint32_t bank_size = 0x4000;
    std::vector<std::uint32_t> slots; // the address each bank is seen at: bank i at slots[min(i, last)]; the first slots.size() banks are mapped at power-on
    std::uint32_t power_on = 0x8000;  // flow runs below this address
    std::uint32_t data_floor = 0;     // an operand below this is not a pointer into the program
    std::vector<Range> data;
    std::vector<HeaderField> header_fields;
    std::vector<VectorName> vectors;
    std::vector<std::string> no_data_labels;   // instruction patterns whose operand is never a data label ("LD (nn)": a write into ROM goes to the mapper)
    std::vector<HardwareTable> hardware;
    std::string prologue;             // the dialect block ahead of the banks ({pages})
    std::vector<std::string> bank_blocks;      // the dialect blocks that open a bank: the first bank's, then the rest's ({bank}, {slot})
    std::string variant_by;           // the loader's value that picks a variant ("system")
    std::vector<Variant> variants;
};

// ---- the cart6502 shape's part of a description ----
struct ChipTable {
    std::string read, write;          // the knowledge hardware sections of a chip's read and write names (the same one for a chip that has one)
};
struct CartShape {
    std::string ext;                  // the cartridge's extension, for the prologue ({ext}) and the rebuilt file
    std::string prologue;             // the dialect block ahead of the code ({stem}, {ext})
    std::string bank_block;           // the dialect block that opens a bank ({offset}, {window}); "" an `.org` of the window
    std::string part = "BANK{bank}";  // the L2 part a bank is
    std::string part_single;          // ... when there is one bank ("" the same)
    std::string header_part;          // the L2 part of the file header, and what its data rows are called
    std::string header_what;
    std::uint32_t tail = 0;           // the bytes at the end of each bank that are data
    bool tail_last_only = false;      // ... of the last bank only
    std::vector<std::string> tail_lines;   // how they are said: {wN} the word at N (by its label), {bN} the byte at N
    unsigned equ_digits = 0;          // the digits of an `.equ` (0: 2 for a zero-page address, else 4)
    std::uint32_t data_floor = 0;     // an operand below this is never a data label
    bool whole = false;               // the source is written in the neutral text and said in the dialect at the end (else line by line)
    std::vector<ChipTable> chips;
};

// ---- the segmented shape's part of a description: ca65 segments placed by an ld65 configuration ----
struct SegmentRole {
    std::string name;                 // the role a loader gives a piece ("header", "chr")
    std::string as = "rows";          // how its bytes are written: "rows" (the dialect's data rows), "row" (one `.byte` line), "incbin" (a file of their own)
    std::string file;                 // (incbin) the file's name, a template ({stem})
    std::string note;                 // a comment ahead of the bytes, a template over the loader's values ("" none)
    std::string what;                 // a comment on the first data row ("the copier header")
    bool neutral = false;             // (rows) the neutral text's rows, said with the rest at the end (`whole`), not the dialect's
};
struct Segmented {
    std::string cpu;                  // "6502", "65sc02", "huc6280", "65816"
    std::string assembler;            // the L2 assembly's syntax ("ca65")
    std::string source = "s";         // the source's extension
    std::string built;                // the rebuilt file's name, a template ({stem} and the loader's values)
    std::vector<std::string> prologue;   // lines ahead of the register names
    bool whole = false;               // the source is written in the neutral text and said in the dialect at the end (else line by line)
    std::string registers;            // the knowledge hardware section of the registers the code names
    std::uint32_t data_floor = 0;     // an absolute operand below this is never a data label
    std::uint32_t tail_at = 0, tail = 0;   // bytes at a fixed address of a code piece that are data (the vectors), and how they are said
    std::vector<std::string> tail_lines;   // ({wN} the word at N, by its label; {bN} the byte at N)
    std::vector<SegmentRole> roles;
    unsigned address_digits = 4;      // the hex digits of a memory area's start in the ld65 configuration
};

// ---- the vectored shape's part of a description: a 68000 image with a vector table or a header, its pieces each at an `.org` ----
struct VectorField {
    std::uint32_t at = 0, size = 0;   // the image offset of a field of the header, and its length
    std::string what;                 // its name, said beside it
    std::string as = "text";          // "text" (an `.ascii` line when it is plain text, else rows), "rows", "long" (`.long 0x...`), "pointer" (`.long` the label);
                                      // (MIPS) "ascii", "zeros" (`.space` or bytes), "word", "address" (a word, by its label), "bytes" (`.space`, `.ascii` or bytes),
                                      // "byte" (a byte row), "items" (data rows) -- each but the first two with `what` (a template) as its comment
    std::string when;                 // a loader value that must be set for the field to stand ("" always)
};
struct Vectored {
    std::string cpu;                  // "m68k"; "mips" (the R3000A), "r4300" (console_mips.cpp)
    std::string assembler;            // the L2 assembly's syntax ("asm68k", "gas")
    std::string source = "asm";       // the source's extension
    std::string built;                // the rebuilt file's name, a template
    bool whole = false;               // the source is written in the neutral text and said in the dialect at the end (else line by line)
    bool neutral_rows = false;        // data rows in the neutral text (else the dialect's)
    std::string registers;            // the knowledge hardware section of the registers the code names
    std::uint32_t immediate_floor = 0;   // a long immediate below this names data only when it is code
    bool calls_start_functions = false;  // every JSR and BSR target is a function (else the loader's starts are)
    unsigned org_digits = 6;          // the hex digits of a piece's `.org`
    bool blank_after_piece = false;   // an empty line after each piece
    std::vector<VectorField> fields;
    std::string vector_names;         // (a "vectors" piece) the loader's list of what each long of the table is
    unsigned vectors_plain = 0;       // ... the first longs of it that are numbers, never labels (the initial stack pointer)
    // (MIPS)
    std::vector<std::string> prologue;   // lines ahead of the names (`.set noreorder`)
    bool big_endian = false;          // the words' byte order
    std::uint32_t bare_region = 0;    // the segment whose view of a register is written with its bare name (KSEG1 or KUSEG); KSEG0 is `_K0`
    std::string other_suffix;         // ... and the other views' suffix
    std::uint32_t ram_size = 0;       // a KSEG0/KSEG1 address below this (physical) is a variable
    std::vector<Range> variables;     // other physical ranges whose addresses are variables
    std::string built_what = "image"; // what the rebuilt file is called in a note
};

// A platform's description (config/platforms/<name>.json).
struct Description {
    std::string name;        // the file's name: what reconstruct() is asked for
    std::string platform;    // the knowledge base's platform (config/knowledge/<platform>)
    std::string title;
    std::string shape;       // "z80-block"
    std::string dialect;     // config/dialects/<dialect>.json
    std::string ports;       // the knowledge hardware section of the I/O ports ("" none)
    std::string fixed;       // the knowledge table of the fixed addresses outside the program ("" none)
    std::vector<HeadLine> head;
    Banked banked;           // (the banked shape's)
    CartShape cart;          // (the cart6502 shape's)
    Segmented segmented;     // (the segmented shape's)
    Vectored vectored;       // (the vectored shape's)
};

// The description of `name`. UsageError when there is none.
const Description& describe(std::string_view name);

// (cart6502) The banks of a 6502 cartridge as flow sees them: flow addresses are bank << 16 | the address in the bank's window.
struct CartBank {
    std::uint32_t offset = 0, size = 0;   // in the image
    std::uint32_t window = 0;             // the address its first byte shows at
    Bytes memory;                         // the address space with the bank in its window, as flow decodes it
};
struct Cart {
    std::vector<CartBank> banks;
    std::function<bool(std::size_t bank, std::uint32_t address)> inside;                      // an address the bank shows
    std::function<bool(std::size_t bank, std::uint32_t address, std::uint32_t size)> fits;    // an instruction there lies in the bank
    std::function<std::optional<std::uint32_t>(std::size_t bank, std::uint32_t address)> target;    // the flow address a branch or call from the bank goes to
    std::function<std::optional<std::uint32_t>(std::size_t bank, std::uint32_t address)> data_at;   // the flow address an absolute operand names, if it is one
    std::function<std::uint32_t(std::size_t bank, std::uint32_t address)> offset;             // the image offset of an address
    Bytes header;                         // a file header ahead of the image (the A78's), written as data ahead of the banks
};

// (segmented) One stretch of the image: a ca65 segment in its own ld65 memory area, in image order. A code piece is linked at the address its
// bytes are seen at; the others are bytes as the role says.
struct Piece {
    std::string segment, area;            // the ca65 segment and the ld65 memory area it loads into
    std::string role;                     // the description's role for it ("" plain data rows)
    std::uint32_t offset = 0, size = 0;   // in the image
    std::uint32_t address = 0;            // the start of its memory area: where code is linked
    bool code = false;
    bool org = false;                     // (vectored) an `.org` of its address opens it
};
// (segmented) The code of a program whose flow keeps a state the plain one does not -- the PC Engine's bank mapping, the SNES's register widths --
// as its loader gives it (console_pce.cpp, console_snes.cpp): it follows the code and writes the code pieces; the shape writes the rest. A
// program without one is followed through one address space by the shape itself.
class SegmentCode {
public:
    virtual ~SegmentCode() = default;
    // Follow the code (asked first, once).
    virtual void follow(const Description& d) = 0;
    virtual image_writer::Functions functions() const = 0;
    virtual std::size_t code_bytes() const = 0;
    // The lines that name the registers and the like, ahead of the pieces (with the empty lines between their groups).
    virtual std::vector<std::string> names() const = 0;
    // The address a code piece is linked at (flow may have decided it: the page a bank was found mapped at).
    virtual std::uint32_t linked_at(const Piece& piece) const { return piece.address; }
    // A code piece's lines.
    virtual void write(translate::l2::Assembly& out, const Piece& piece) = 0;
};

struct Segments {
    std::vector<Piece> pieces;
    std::shared_ptr<SegmentCode> code;    // the loader's own flow ("" the shape's)
    std::function<std::optional<std::uint32_t>(std::uint32_t address)> offset_of;   // the image offset the processor sees at an address, if any
    std::size_t measured = 0;             // the bytes the result says the program is (the PRG, the file)
    std::vector<Range> code_ranges;       // (vectored) the address ranges code may stand in
    std::function<Bytes(const Bytes&)> rebuilt;   // (vectored) the file the assembled image makes ("" the image itself)
    Bytes original;                       // (vectored) what the rebuilt file is compared with ("" the image)
};

// What a loader read from a file: the program's bytes, the address of the first, its entry points (address -> name) and the header's values.
struct Loaded {
    Bytes image;
    std::uint32_t origin = 0;
    image_writer::Names entries;
    Values values;
    std::vector<std::uint32_t> starts;   // (banked) where flow starts, in the order the format found them
    Bytes memory;                        // (banked) the address space at power-on that flow decodes
    Cart cart;                           // (cart6502)
    Segments segments;                   // (segmented: `image` is the whole file the pieces lie in, `memory` what flow decodes)
};

// The program in `file`, as the platform's loader reads it (IoError when the file cannot be read, FormatError when it is not the platform's).
Loaded load(std::string_view platform, const std::filesystem::path& file);

// The Lynx's loader for a program read already: a cartridge's loaded segments are set by the caller, who can play its boot ROM
// (engine/lynx_boot, which the writer does not link). load("lynx", file) reads one without them: a cartridge is then all data.
Loaded load_lynx(const formats::lynx::Program& program);

// The Genesis's loader for a cartridge read already (the 32X's 68000 side is one: source/sega32x_writer).
Loaded load_genesis(const formats::genesis::Rom& rom);

// `text` with each {name} (a value), {name:spec} (a number in std::format's spec: 04X) and {name?yes|no} (the branch for whether the value is
// set: non-zero, non-empty; the branches are templates too) put in. UsageError for a name `values` does not have.
std::string fill(std::string_view text, const Values& values);

// The program written as source to `folder` (<stem>.asm for a file named `file_name`) and rebuilt. The writer adds the values every program
// has: `file` (file_name), `stem` (its name without the extension), `size` and `kb` (the image's), `origin`. A head template that is
// empty is an empty line; the others are comment lines.
image_writer::Written write(const Description& description, const Loaded& program, std::string_view file_name, const std::filesystem::path& folder);

// load() and write() for a file.
image_writer::Written reconstruct(std::string_view platform, const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::console_writer
