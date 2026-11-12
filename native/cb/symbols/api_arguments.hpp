// cb/symbols/api_arguments.hpp -- how many bytes of arguments an imported function removes from the stack, read from the
// retail module that EXPORTS it.
//
//     Win32Arguments sizes(headers_and_memory, scanner);
//     sizes.stack_bytes("USER32", "FindWindowA")     8
//     sizes.stack_bytes("CRTDLL", "memchr")          0        (cdecl: the caller cleans up)
//     sizes.stack_bytes("KERNEL32", "nonesuch")      nullopt  (not found / not decidable)
//
// The oracle stops at an import and has to return as the real function would: a stdcall function ends in `ret N`, a cdecl
// one in a bare `ret`. N is in the exporting DLL's own code, so it is looked up there rather than in a header table (which
// knew about a quarter of the imports the Win95 modules use). The answer is what the binary does; nothing is guessed.
// Every path from a function's entry is walked to its `ret`, tracking the stack depth; direct jumps, calls, import thunks
// and forwarded exports are followed (StackScanner).
//
// Two sources, checked against each other over eight system DLLs (they agree on 1,669 of 1,695 functions): the Win32
// header prototype, and the exporting DLL's own code. The header wins where it applies -- the disagreements were all the
// code scan misreading a hand-written wrapper -- and the scan covers what the headers do not: undocumented functions, and
// functions taking a structure by value.
//
// This module opens no file and keeps nothing on disk. The corpus is behind `ModuleSource`; the table the headers give
// and what was found so far (the knowledge base's win32 "argument_bytes.header" / "argument_bytes.code" kinds, win16
// "argument_bytes") are behind `Win32Memory` and `Win16Memory`; how a 16-bit export's code returns is behind
// `Win16Exports` (decompile/describe reads it). The caller gives each of them.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/pe.hpp"
#include "cb/platforms/decoder.hpp"

namespace cb::symbols::api_arguments {

// Instructions walked per function.
inline constexpr std::size_t kMaxInstructions = 4000;
// How deep calls and forwarded exports are followed.
inline constexpr int kMaxHops = 6;

// The extensions of a module file: .dll .exe .drv .cpl .ocx.
std::span<const std::string_view> module_extensions();

// A module's stem: the file name in capitals without the extension of a module file ("c:\\x\\user32.DLL" -> "USER32").
std::string stem(std::string_view dll);

// A PE module as the scan reads it: what it exports and imports, and its bytes by address.
class ExportingModule {
public:
    explicit ExportingModule(formats::pe::Image image);

    std::uint64_t image_base() const { return base_; }
    // Up to `n` bytes at the RVA, as the file holds them (fewer at the end of the file); empty where the RVA is not in
    // a section.
    ByteView bytes_at(std::int64_t rva, std::size_t n) const;
    // The RVA of an export by name, or by ordinal.
    std::optional<std::uint32_t> export_by_name(std::string_view name) const;
    std::optional<std::uint32_t> export_by_ordinal(std::uint64_t ordinal) const;
    // The module and name an import slot (its address in memory) refers to: "#<ordinal>" for an import by number.
    struct ImportedName {
        std::string dll;
        std::string name;
    };
    const ImportedName* import_at(std::uint64_t slot_va) const;
    // "DLL.Name" when an export's address points into the export directory (the export is another module's).
    std::optional<std::string> forwarder(std::uint32_t rva) const;

private:
    friend class StackScanner;
    formats::pe::Image image_;
    std::uint64_t base_ = 0;
    std::unordered_map<std::string, std::uint32_t> exports_;         // lookup only
    std::unordered_map<std::uint64_t, std::uint32_t> ordinals_;      // lookup only
    std::unordered_map<std::uint64_t, ImportedName> imports_;        // lookup only
    std::uint32_t export_lo_ = 0, export_hi_ = 0;                    // the export directory's RVA range
    // What the scan found for each function of this module (also the guard against a function that calls itself); it lives
    // with the module so that it ends with it. A module is scanned by one thread at a time.
    mutable std::unordered_map<std::int64_t, std::optional<std::uint32_t>> scanned_;
};

// Where the modules of the corpus are.
class ModuleSource {
public:
    virtual ~ModuleSource() = default;
    // The module whose stem is given ("USER32"), or null when the corpus has none that reads as a PE.
    virtual std::shared_ptr<const ExportingModule> find(std::string_view stem) = 0;
};

// Walks the code of exports for what they pop. Not thread-safe: it keeps the decoders it made and the modules keep what was
// found in them; one per thread, or one behind a lock.
class StackScanner {
public:
    explicit StackScanner(ModuleSource& modules) : modules_(modules) {}

    // Bytes of arguments the export `dll!name` (`name` is "#<ordinal>" for a number) removes on return, following forwarded
    // exports and tail calls through imports up to `hops` modules deep; nullopt when it cannot be decided.
    std::optional<std::uint32_t> resolve(std::string_view dll, std::string_view name, int hops = kMaxHops);

    // The same for the function at `rva` of a module.
    //
    // Every path from the entry is walked with the stack depth tracked, so the answer is right for the hand-written
    // wrappers Windows 95 is full of as well as for compiled code:
    //
    //     FindWindowA:  pop ecx / push 0 / push 0 / push ecx / jmp FindWindowExA     -> 16 - 8 = 8
    //
    // A `ret N` reached with D bytes still pushed means the function itself took N - D. Calls are resolved the same way
    // (the callee's own answer is what it pops). Paths whose depth is lost (an indirect call) still report their `ret N`,
    // but only count if no exact path was found.
    std::optional<std::uint32_t> scan(const ExportingModule& module, std::int64_t rva, int hops);

private:
    struct Walk;
    ModuleSource& modules_;
    platforms::DecoderSet decoders_;
};

// A size that was looked for once: `bytes` is empty when the search could not decide (and is not made again).
struct Known {
    std::optional<std::uint32_t> bytes;
};

// What the headers and the knowledge base know of 32-bit exports.
class Win32Memory {
public:
    virtual ~Win32Memory() = default;
    // Argument bytes for `name` as the Win32 headers declare it, or nullopt when the headers do not settle it (no
    // prototype; variadic; caller-cleans; a wide by-value parameter).
    virtual std::optional<std::uint32_t> header_bytes(std::string_view name) = 0;
    // What an earlier scan of `stem!name` found; nullopt: never scanned.
    virtual std::optional<Known> scanned(std::string_view stem, std::string_view name) = 0;
    virtual void remember(std::string_view stem, std::string_view name, std::optional<std::uint32_t> bytes) = 0;
};

class Win32Arguments {
public:
    Win32Arguments(Win32Memory& memory, StackScanner& scanner) : memory_(memory), scanner_(scanner) {}

    // The headers' answer for `name`, or nullopt.
    std::optional<std::uint32_t> header_bytes(std::string_view name) { return memory_.header_bytes(name); }
    // Bytes of arguments `dll!name` pops on return (0 for cdecl), or nullopt if unknown. The headers' answer when they
    // have one; else what the code says, scanned once and remembered.
    std::optional<std::uint32_t> stack_bytes(std::string_view dll, std::string_view name);
    // The argument count for the oracle's import stub (stack bytes / 4), or nullopt if unknown or not a whole number of words.
    std::optional<std::uint32_t> argc(std::string_view dll, std::string_view name);

private:
    Win32Memory& memory_;
    StackScanner& scanner_;
};

// ---- 16-bit ---------------------------------------------------------------------------------------------------------

// How the code of a 16-bit export returns.
struct ExportReturn {
    // The export only goes on into another function that removes this many bytes less what it added itself (a thunk that
    // puts a word of its own under the return address and goes on: what its callers push is what the body removes less
    // what it added -- Win95 USER.471, lstrcmpi, read as its body's `retf 0Ah` gave the C two arguments too many).
    std::optional<std::uint32_t> forwarded;
    // The code ends in a far return that pops `pop` bytes (negative: it does not, or it is not a far return).
    bool far_return = false;
    std::int64_t pop = -1;
};

// The 16-bit modules of the corpus, as far as the table needs them.
class Win16Exports {
public:
    virtual ~Win16Exports() = default;
    // How the export `ordinal` of the 16-bit module `module` (capitals) returns; nullopt when the corpus has no such
    // module. `what` is empty when the module has no code for that export (no such entry, or not in a code segment with
    // bytes). `file` is the file the module was read from, for the note kept with the size.
    struct Found {
        std::optional<ExportReturn> what;
        std::string file;
    };
    virtual std::optional<Found> find(std::string_view module, std::uint32_t ordinal) = 0;
};

// The table of 16-bit export sizes found so far for this product and way of reading, keyed "MODULE.ordinal".
class Win16Memory {
public:
    virtual ~Win16Memory() = default;
    virtual std::optional<Known> remembered(std::string_view key) = 0;
    // Every entry, for table_text().
    virtual std::map<std::string, std::optional<std::uint32_t>> table() = 0;
    virtual void remember(std::string_view key, std::optional<std::uint32_t> bytes, std::string_view source) = 0;
};

class Win16Arguments {
public:
    Win16Arguments(Win16Memory& memory, Win16Exports& exports) : memory_(memory), exports_(exports) {}

    // Bytes of arguments the export removes on return; nullopt when unknown. Read from the code once, then remembered.
    std::optional<std::uint32_t> arg_bytes(std::string_view module, std::uint32_t ordinal);
    // arg_bytes for a name as the emulator reports it: "USER.@171" or "USER_171".
    std::optional<std::uint32_t> of(std::string_view api);
    // The table found so far, as JSON text with the keys sorted: what a cache of the checker's verdicts is keyed by, since
    // the checker answers calls by it.
    std::string table_text();

private:
    Win16Memory& memory_;
    Win16Exports& exports_;
};

// A name as the emulator reports an API: "USER.@171", "USER_171" -> {"USER", 171}; nullopt for any other shape.
struct ApiName {
    std::string module;
    std::uint32_t ordinal = 0;
};
std::optional<ApiName> parse_api_name(std::string_view api);

}  // namespace cb::symbols::api_arguments
