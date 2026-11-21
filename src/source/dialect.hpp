// source/dialect.hpp -- the period assembler dialects a console's source is written in, over the one engine underneath.
//
// A processor's SLEIGH description writes an instruction as `LD HL,0xc000` and the directive layer (build/mips_asm) reads `.org`, `.equ`,
// `.byte`, `.space`, `.ascii`. A program for a Master System is not written so: it is WLA-DX (`LD HL,$C000`, `.DEFINE`, `.BANK 0 SLOT 0`, `.DSB`);
// a Game Boy's is RGBDS, an NES's ca65, an Atari 2600's DASM, a Mega Drive's vasm, a PlayStation's GNU as. CodeBreaker's source reads in its
// platform's own dialect and builds through the same engine: a dialect is DATA (config/dialects/<name>.json) and there is ONE printer and ONE
// parser here.
//
//     const dialect::Dialect& d = dialect::find("wla-dx");
//     d.line("        .space 50, 0xff")           -> "        .DSB 50,$FF"           (the printer: neutral text to the dialect)
//     d.instruction("LD HL,0xc000")               -> "LD HL,$C000"
//     d.parse(dialect_source)                     -> the neutral text the description's assembler and the directive layer read
//
// What a dialect says (see config/dialects/wla-dx.json): the comment character; how a number is written (prefix, suffix, case, padding);
// `directives`, rows of {neutral template, dialect template} with {v} a number, {list} a list of numbers and strings, {n} a count, {name}, {s}
// a string, {rest}; how a label line reads; `blocks`, the lines a writer asks for by name (the memory map ahead of the code, a bank's head)
// with {placeholders} filled by the writer; and `layout`, how a banked source says where a line lands (WLA-DX's slots).
#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core/bytes.hpp"

namespace cb::source::dialect {

using Vars = std::vector<std::pair<std::string, std::string>>;

class Dialect {
public:
    struct Impl;
    explicit Dialect(std::unique_ptr<Impl> impl);
    ~Dialect();
    Dialect(const Dialect&) = delete;
    Dialect& operator=(const Dialect&) = delete;

    const std::string& name() const;
    // True for the neutral dialect (the description's own text under the directive layer): nothing is converted.
    bool neutral() const;

    // ---- the printer ----
    // A number as the dialect writes it: `digits` hex digits at least (rounded up to an even count when the dialect pads that way).
    std::string number(std::uint64_t value, unsigned digits) const;
    // One line of neutral source (an instruction, a directive of the directive layer, a label, a comment; its indent and trailing comment kept).
    std::string line(std::string_view neutral, int name_width = 0) const;   // (name_width: the width an `.equ`'s names are lined up to, when the writer's is not the dialect's)
    // Neutral source of many lines (a writer that wrote its whole source in the neutral text), every line said in the dialect.
    std::string print(std::string_view neutral_source) const;
    // A `.byte` line a writer made for an instruction the description cannot say back (`.byte 0x8d,0x02,0x00  ; STA 0x2`): its comment said in the dialect.
    std::string fallback(std::string_view code) const;
    // `STA.w x`: an absolute form of a zero-page address, for a dialect that has one (DASM); empty when it has none for the mnemonic.
    std::string forced_absolute(std::string_view mnemonic, std::string_view operand) const;
    // One instruction text (no indent) as the dialect writes it.
    std::string instruction(std::string_view text) const;
    // `name:` as the dialect defines a label.
    std::string label_line(std::string_view name) const;
    // Bytes as lines: rows of 16 with runs of four or more printable characters as strings, a run of 16 or more of one value as one fill line.
    std::vector<std::string> data_lines(ByteView data) const;
    // The lines of a named block with `vars` put in ({pages}); empty when the dialect has none.
    std::vector<std::string> block(std::string_view name, const Vars& vars = {}) const;
    bool has_block(std::string_view name) const;

    // ---- the parser ----
    // Dialect source as the neutral text the directive layer and the description's assembler read. ParseError for a line it cannot read.
    // `aux`: what a dialect keeps beside its source -- ca65's linker configuration (where each `.segment` loads) and the folder an `.incbin` is read from.
    struct Aux {
        std::string linker_config;
        std::filesystem::path folder;
    };
    std::string parse(std::string_view source) const { return parse(source, Aux{}); }
    std::string parse(std::string_view source, const Aux& aux) const;

private:
    std::unique_ptr<Impl> impl_;
};

// The dialect config/dialects/<name>.json describes ("sleigh" is the neutral one). UsageError for a name with no file.
const Dialect& find(std::string_view name);

}  // namespace cb::source::dialect
