// decompile/engine/sleigh_assembler.hh -- the inverse of the SLEIGH reader: CodeBreaker's one assembler engine, driven by a processor's
// SLEIGH description (languages/<cpu>.sla). It assembles a line written in the description's own syntax -- exactly the text sleigh::Reader
// prints -- into bytes, for any description. Nothing of the core shows through this header.
//
//     sleigh::Assembler z80(languages / "z80.sla", {});
//     for (const auto& bytes : z80.assemble("LD A,0x12", 0x0000)) ...    // {3e 12}
//
// How: the line is parsed against each constructor's print pieces (a packrat parse over the description's subtables), giving constructor
// trees; each tree is laid out the way the translator lays an instruction out, and every operand's value is solved back into token and
// context fields (inverting field, field*k, field<<k, inst_next+field*2, sign extension ...; small fields otherwise by enumeration). Each
// candidate is verified by disassembling it with the same translator and comparing the text, so partial solving is always safe.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cb::decompile::sleigh {

class Assembler {
public:
    // `sla`: a compiled description; `context`: the description's context variables to start from (x86's "addrsize", ARM's "TMode" ...).
    // Error (std::runtime_error) for a file that is not a description.
    Assembler(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context);
    ~Assembler();
    Assembler(const Assembler&) = delete;
    Assembler& operator=(const Assembler&) = delete;

    // All encodings of one instruction at `address` (in the description's address units), shortest first; empty when the text is no
    // instruction of the description's.
    std::vector<std::vector<std::uint8_t>> assemble(std::string_view line, std::uint64_t address) const;

    // What the last assemble() did, for diagnosis: constructor trees the line parsed to, candidates solved, candidates that round-tripped.
    struct Diagnostics {
        std::size_t parses = 0, candidates = 0, verified = 0;
    };
    Diagnostics last() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::decompile::sleigh
