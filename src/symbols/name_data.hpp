// symbols/name_data.hpp -- names as data: what a function is called and where the name comes from, the store's key of a
// function's address, the digests that tell function bodies apart, the names debug symbols and PowerPC images give.
//
// This is the part of naming every layer can use: it reads bytes and formats, and asks nothing of the decompiler. The namer
// that puts the sources together (exports, ordinals, debug symbols, the body store) is symbols/names.hpp.
//
//     Named named = powerpc_names(facts);                  // what an image's entry point and run-time type information name
//     std::string key = function_key(base::Lane::Ne, a);   // "segment:offset" of a 16-bit module, hex otherwise
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "base/corpus.hpp"
#include "core/bytes.hpp"
#include "core16_types.hpp"
#include "formats/ne.hpp"
#include "formats/pe_functions.hpp"
#include "platforms/decoder.hpp"

namespace cb::symbols::names {

// A function's address: a 32-bit one as it is, a 16-bit module's as core16::address lays it out ((selector << 16) + offset).
using Address = std::uint64_t;

// A name and where it comes from ("export", "debug symbols (KERNEL32.SYM)", "ordinal table (user.exe)").
struct NameSource {
    std::string name, source;
    friend bool operator==(const NameSource&, const NameSource&) = default;
};
using Named = std::map<Address, NameSource>;

// What is proposed for a function nothing names (store/proposals.hpp, symbols/proposals.hpp). It is not the function's name.
struct ProposedName {
    std::string name, description, rule;
    int confidence = 0;   // percent
};
struct FunctionName {
    Address address = 0;
    std::string key;     // the function store's text for the address
    std::string name, source;
    std::optional<ProposedName> proposed;   // only for a function with no name and a proposal nobody has decided
};
struct ImportName {
    std::string module;
    std::optional<std::uint32_t> ordinal;   // none: imported by name
    std::string name, source;
};
struct ModuleNames {
    std::vector<FunctionName> functions;   // in address order
    std::vector<ImportName> imports;
};

// The address a person writes: "segment:offset" (decimal segment, hex offset) of a 16-bit module, or the address in hex; nullopt for anything else.
std::optional<Address> parse_address(std::string_view text);

// The function store's text for an address:"segment:offset" (the offset in four hex digits) for a 16-bit module, the address in hex for any other.
std::string function_key(base::Lane lane, Address address);

// ---- bodies -----------------------------------------------------------------------------------------------------------------------
// The smallest body (bytes up to its last return or jump) that is told apart: a shorter one is everybody's.
inline constexpr std::size_t kSmallestBody = 24;

// The digest of a function's bytes up to its last return or jump (what follows is padding) with the offsets in `masked` left out: 16 hex digits.
// nullopt for a body with nothing of its own in it (shorter than kSmallestBody).
std::optional<std::string> body_digest(ByteView code, std::span<const std::uint32_t> masked, std::size_t last);

// {address: digest} for the functions of a 32-bit x86 module that have a body worth telling apart (six instructions of substance and
// kSmallestBody bytes). The places the linker fills in -- addresses inside the module, calls and jumps out of the function -- are left out.
std::map<Address, std::string> bodies32(const formats::pe_functions::PeCarve& carve);

// The same for a 16-bit module, by the function starts given: the places the loader writes (its relocations), near calls and jumps out of the
// function.
std::map<Address, std::string> bodies16(const formats::ne::Module& module, std::span<const core16::SegOff> starts, platforms::DecoderSet& decoders);

// ---- debug symbols ----------------------------------------------------------------------------------------------------------------
// {(segment, offset): name} from CodeView symbols appended to an executable (the NB09 form: public symbols and procedures); empty when there are
// none or they are damaged.
std::map<core16::SegOff, std::string> codeview(ByteView data);

// One segment of a .SYM file: its number and each public name at its offset.
struct SymSegment {
    std::uint32_t number = 0;
    std::map<std::uint32_t, std::string> symbols;
};
// The segments of a .SYM file (the symbol file MAPSYM makes from a linker map); nullopt for a file that is not one or is damaged.
std::optional<std::vector<SymSegment>> read_sym(ByteView data);
// {segment number: name} from a .SYM file. The linker names a segment after the object file its code came from (_MSGBOX, _WINUTIL, ...), so
// where a module was linked one segment per object these are the names of its maker's source files. Empty when it cannot be read.
std::map<std::uint32_t, std::string> sym_objects(ByteView data);

// ---- PowerPC images -----------------------------------------------------------------------------------------------------------------
// What a PowerPC image says that names or finds functions: its entry point, the table of where its functions start, and the virtual functions of
// the classes its run-time type information names.
struct PowerPcNames {
    std::optional<Address> entry;
    std::vector<Address> functions;   // ascending
    struct VirtualMethod {
        Address address = 0;
        std::string class_name;
        std::uint32_t slot = 0;
    };
    std::vector<VirtualMethod> methods;   // in address order
};

// The names those facts give for certain: "entry" at the entry point, and <class>_vf<slot> for each virtual function (the class name's runs
// of non-word characters as one underscore each; the first function at an address and the first use of a name win).
Named powerpc_names(const PowerPcNames& facts);

}  // namespace cb::symbols::names
