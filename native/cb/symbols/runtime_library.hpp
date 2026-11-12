// cb/symbols/runtime_library.hpp -- which of a module's functions came with the compiler, and which compiler.
//
// Nobody wrote memcpy, the 64-bit multiply helper, the stack probe or the start-up code for the module they turn up in:
// they are linked in from the compiler's libraries. Such a function is IDENTIFIED -- named, with the library it is from
// -- not reconstructed as if the module's author had written it.
//
// How: the libraries themselves are the evidence. Each function of a period library (the C runtime, MFC's static
// library) is reduced to a signature -- its length, the places the linker fills in (listed by the library's own
// relocation records, so nothing is guessed: libraries/coff, libraries/omf), and a digest of every other byte. A function
// of a module is that library function when all of those bytes are the same. Two more things are known without
// signatures: a module that IS a runtime library (CRTDLL, MSVCRT20, an MFC DLL) -- its exports are that library's
// functions -- and the thunk compiler's stubs.
//
//     const SignatureTable table = SignatureTable::from_rows(rows32);          // the rows the knowledge base holds
//     table.match(code)                                                         // {name, library} or nullopt
//     identify_pe(table, Pe32Module{...}, decoders)                             // {address: {name, library}}
//     identify_ne(table, Ne16Module{...})
//     describe_compiler(facts, libraries_of_the_identified_functions)           // linker, guess, evidence
//     learn(sources, decoders, log)                                             // make the rows from library files
//
// This module reads no file of the corpus and asks no store: the caller gives it the module (what the format readers
// found), the signature rows, and the decoders; the one thing it opens is a library file, in learn().
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/platforms/decoder.hpp"

namespace cb::symbols::runtime_library {

// Which x86 mode a module's code is in.
enum class Bits : std::uint8_t { X16 = 16, X32 = 32 };

// How many bytes of a function are compared (a longer function is compared by its first kLongest).
inline constexpr std::size_t kLongest = 1024;
// Bytes the linker does not touch that a signature must have.
inline constexpr std::size_t kFewest = 10;
// Signatures are looked up by this many leading bytes (fewer when the linker touches the early ones).
inline constexpr std::size_t kLead = 6;

// ---- signatures ---------------------------------------------------------------------------------------------------

// A function's bytes reduced to what tells it from others.
struct Signature {
    std::size_t size = 0;        // bytes compared
    std::string mask_text;       // the places the linker fills in, as runs "offset.length" in hex, comma-separated
    std::string digest;          // the first 16 hex digits of the SHA-1 of the bytes with those places zeroed
    friend bool operator==(const Signature&, const Signature&) = default;
};

// The signature of `code` with what its byte mask leaves out (non-zero = left out, as libraries::Function::mask) taken
// out, or nullopt when too little is left to tell it from anything else (fewer than kFewest bytes the linker leaves
// alone, or fewer than four distinct values: padding, a table of zeros).
std::optional<Signature> signature(ByteView code, ByteView mask);

// The offsets a mask text names. ParseError for text that is not runs of "hex.hex".
std::vector<std::uint32_t> mask_offsets(std::string_view mask_text);

struct Identified {
    std::string name;      // as the C name
    std::string library;   // "<compiler>: <library>" as it was learned
    friend bool operator==(const Identified&, const Identified&) = default;
};

// One signature as the knowledge base keeps it (a row of the authored runtime-signature kind).
struct SignatureRow {
    std::string lead_hex;       // the first bytes of the function, in hex
    std::size_t size = 0;
    std::string mask_text;
    std::string digest;
    std::string name;
    std::string library;
};

// The signatures of one bit width, indexed by their leading bytes.
class SignatureTable {
public:
    // FormatError for a lead that is not hex or a mask that is not runs.
    static SignatureTable from_rows(std::span<const SignatureRow> rows);

    // The library function `code` starts with: the longest leading bytes first, rows of one lead in the order given.
    std::optional<Identified> match(ByteView code) const;
    std::size_t size() const { return rows_; }

private:
    struct Entry {
        std::size_t size = 0;
        std::vector<std::uint32_t> masked;
        std::string digest;
        Identified what;
    };
    std::unordered_map<std::string, std::vector<Entry>> by_lead_;   // lookup only: the leading bytes
    std::size_t rows_ = 0;
};

// ---- identifying ----------------------------------------------------------------------------------------------------

// A linker symbol as the C name: `_memcpy` -> memcpy, `__allmul` -> _allmul, `_f@8` -> f; a C++ name stays as it is.
std::string c_name(std::string_view symbol);

// The modules that are a runtime library themselves, by stem ("CRTDLL", "MFC30"): what their exports are.
const std::map<std::string, std::string, std::less<>>& runtime_libraries();
// What a runtime-library module's exports are; nullptr for any other module (the stem is in capitals).
const std::string* runtime_library_of(std::string_view stem);

// What the stubs of the thunk compiler are said to be from.
inline constexpr std::string_view kThunkCompiler = "Microsoft thunk compiler: a 32-to-16-bit flat thunk";

// A run of a PE module's code in memory.
struct CodeRegion {
    std::uint64_t va = 0;
    ByteView bytes;
    bool contains(std::uint64_t address) const { return address >= va && address - va < bytes.size(); }
};

// An export of a PE module.
struct PeExport {
    std::uint32_t ordinal = 0;
    std::optional<std::string> name;
    std::uint32_t rva = 0;
};

// What identify_pe reads of a 32-bit PE module.
struct Pe32Module {
    std::string stem;                                   // the file name in capitals without the extension
    std::uint64_t image_base = 0;
    std::vector<CodeRegion> code;                       // the code sections
    std::vector<std::uint64_t> function_starts;         // ascending
    std::vector<PeExport> exports;
    // A module the thunk compiler made (build/flat_thunk classifies it): its stubs are found as well.
    bool flat_thunk = false;
    // For a runtime-library module (runtime_library_of(stem)): the names the tables give to exports that have none.
    std::map<std::uint32_t, std::string> ordinal_names;
};

// {address: what it is} of the functions of a 32-bit module that belong to a runtime library: by signature, the thunk
// compiler's stubs (flat_thunk), and, for a module that is itself a runtime library, its exports.
std::map<std::uint64_t, Identified> identify_pe(const SignatureTable& table, const Pe32Module& module,
                                                platforms::DecoderSet& decoders);

// The starts of the functions of a flat-thunk module that the thunk compiler generated: each reaches the 16-bit side
// through a pointer kept in the code section itself (QT_Thunk's, filled in by ThunkConnect32 when the DLL is loaded),
// which no compiled C does. They are its output, rebuilt from the .thk script, not C -- and cannot run here.
std::vector<std::uint64_t> flat_thunk_stubs(std::span<const CodeRegion> code, std::span<const std::uint64_t> starts,
                                            platforms::DecoderSet& decoders);

// A place in a 16-bit module.
struct SegOff {
    std::uint16_t segment = 0;     // 1-based
    std::uint16_t offset = 0;
    friend auto operator<=>(const SegOff&, const SegOff&) = default;
};

struct Ne16Segment {
    std::uint16_t index = 0;       // 1-based
    bool code = false;
    ByteView bytes;                // what the file holds of it
};

// An exported entry of a 16-bit module (fixed or movable).
struct Ne16Entry {
    std::uint16_t ordinal = 0;
    std::uint16_t segment = 0;
    std::uint16_t offset = 0;
};

struct Ne16Module {
    std::string module_name;                    // as the module names itself
    std::vector<Ne16Segment> segments;
    std::vector<SegOff> function_starts;        // ascending
    std::vector<Ne16Entry> entries;
    std::map<std::uint32_t, std::string> ordinal_names;   // as Pe32Module's
};

// The same for a 16-bit module: by signature, and for a module that is itself a runtime library, its exports.
std::map<SegOff, Identified> identify_ne(const SignatureTable& table, const Ne16Module& module);

// ---- which compiler ---------------------------------------------------------------------------------------------------

// What the file itself says of its tools.
struct CompilerFacts {
    enum class Kind { Pe, Ne, Other } kind = Kind::Other;
    ByteView file;                              // the whole file, for the marks the tools leave in it
    std::uint32_t linker_major = 0, linker_minor = 0;   // the header's (Pe, Ne)
    std::uint32_t pe_header_offset = 0;         // Pe: where the PE header starts (the tool block is before it)
    std::vector<std::string> imported_modules;  // Pe: the file names of the modules it imports from, as written
    std::vector<std::string> module_references; // Ne: the modules it refers to
};

struct CompilerInfo {
    std::string linker;                         // "2.50", empty when the file says none
    std::string compiler;                       // the guess, or "not established"
    std::vector<std::string> evidence;
};

// What built a module: the linker's version in the header, marks the runtime library leaves in the image, the runtime
// DLLs it imports, and which library its identified functions came from (`libraries`: one entry per function).
CompilerInfo describe_compiler(const CompilerFacts& facts, std::span<const std::string> libraries);

// The tool block Visual C++ 6 and later leave before the PE header: (product id, build, count), oldest first. Empty for
// a file with none, or one whose block runs outside the file.
struct ToolRecord {
    std::uint32_t product = 0, build = 0, count = 0;
};
std::vector<ToolRecord> tool_block(ByteView file, std::size_t header_offset);

// ---- making the signatures ------------------------------------------------------------------------------------------

// Whether an instruction is part of the frame every function sets up and takes down, or a trivial `return 0`.
bool is_frame_instruction(std::string_view mnemonic, std::string_view operands);

// How many instructions of the first `size` bytes of a function do something of their own. An empty function is the same
// bytes in every program and every library; only one with substance can be told to be a particular library function.
std::size_t substance(platforms::DecoderSet& decoders, Bits bits, ByteView code, std::size_t size);

struct LearnSource {
    std::string what;                  // what the library is: "Visual C++ 2.0 C runtime"
    std::filesystem::path file;        // an `!<arch>` archive, a COFF object, or a 16-bit library
};

// One learned signature.
struct LearnedRow {
    Bits bits = Bits::X32;
    SignatureRow row;
    std::size_t library = 0;           // index into Learned::libraries
};

struct Learned {
    std::vector<std::string> libraries;   // "what (FILE.LIB)" of each library a row came from
    std::vector<LearnedRow> rows;         // in the order learned
    std::size_t count(Bits bits) const;
};

// Make the runtime signatures from [(what the library is, file)], earlier ones first: when two libraries hold the same
// function, it is reported as the earlier one's.
//
// Kept out: a function with too little substance to be told from anybody's (fewer than 4 instructions of its own; 8 for a
// class library, whose thousands of small methods look like everyone's small methods), and any signature two
// differently named functions share once the linker's bytes are left out -- which of them a match would be is not known.
// A library that cannot be read, or is not x86 code, is reported to `log` and passed over.
Learned learn(std::span<const LearnSource> sources, platforms::DecoderSet& decoders,
              const std::function<void(const std::string&)>& log);

// The text of runtime_signatures.json for what was learned (the layout the knowledge base's loader reads).
std::string format_signature_file(const Learned& learned);

}  // namespace cb::symbols::runtime_library
