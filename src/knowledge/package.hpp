// knowledge/package.hpp -- one knowledge base: what CodeBreaker knows about each platform, kept in one place.
//
//     Knowledge kb(store, HeaderFolders::standard(openwatcom, reference));
//     kb.prototype("win16", "KERNEL.GLOBALALLOC")     how the headers declare it: {returns, convention, parameters, header}
//     kb.declaration("win16", "LZRead")               "int WINAPI LZRead(HFILE, void FAR *, int)"
//     kb.spelling("GETDLGITEM")                       "GetDlgItem"
//     kb.name(body_digest)                            (name, where it is from) of a function's body, in any module
//     kb.name_body(body_digest, "Name")               ... given once, seen in every module with that body
//     kb.signature_match("win32", code)               the runtime-library function `code` is
//     kb.api_model("win16", "GLOBALLOCK")             what the emulator's stand-in for it does
//     kb.known(platform, kind, key) / kb.learn(platform, kind, key, value, source)   anything else
//
// The base is one SQLite file keyed by platform (store.hpp). Its platforms:
//
//     win16     16-bit Windows: prototypes (the 16-bit headers), argument bytes read from exports' code, runtime-library
//               signatures, the emulator's stand-ins, KERNEL's numbers
//     win32     32-bit Windows: prototypes, argument bytes (the headers' table, and exports' code), runtime-library
//               signatures, the stand-ins' argument counts
//     windows   both: numbered exports' names from their makers' definition files
//     vxd       ring-0 drivers: devices, their services in order, control messages, stand-ins
//     dos       DOS and BIOS interrupts
//     xbox360   the console's system libraries' numbered exports, the XDK's library signatures
//     (bodies)  the name store: a body's digest and the names given to it, whatever its platform
//
// Where each kind comes from, and how a change reaches it, is sources.hpp (the files people write: kept where they are,
// loaded again when they change) and the module that makes it (symbols/prototypes from the headers, symbols/api_arguments
// from an export's code). Platform names may be given as the lanes say them: ne, pe, le, mz, powerpc, 16, 32.
//
// What a step of the engine reads here is an input of the fact it makes (reads.hpp): a changed prototype or argument size
// makes stale exactly the facts made from it.
//
// `Knowledge` is the one object that brings the base and the symbol readers together; the symbol readers themselves
// (symbols/api_arguments, symbols/ordinals, symbols/sdk_structs) take the memories this gives (`win32_memory`,
// `win16_memory`) instead of reaching for a process-wide base.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "knowledge/describe.hpp"
#include "knowledge/sources.hpp"
#include "knowledge/store.hpp"
#include "knowledge/value.hpp"
#include "symbols/api_arguments.hpp"
#include "symbols/prototypes.hpp"
#include "symbols/runtime_library.hpp"

namespace cb::knowledge {




// A platform of the base and its title.
struct PlatformInfo {
    std::string_view name;
    std::string_view title;
};

// Every platform, in the order the listing shows them.
std::span<const PlatformInfo> known_platforms();

// The base's name for a platform, from the lane's (ne -> win16, pe -> win32, le -> vxd, mz -> dos, powerpc -> xbox360,
// "16" -> win16, "32" -> win32); a name that is already the base's, or none of them, as it is.
std::string platform(std::string_view name);
// 16 -> "win16", 32 -> "win32"; any other number as its text.
std::string platform(int bits);

// `KERNEL.GLOBALALLOC`, `USER32!FindWindowA` -> the function's own name; anything else as it is.
std::string bare_name(std::string_view name);

class Knowledge;

// A hardware register of a console, as its `registers` description says.
struct HardwareRegister {
    std::string name;       // the hardware documentation's name
    std::string table;      // the group it is in ("ports", "io", "tia_write")
    std::string space;      // what the address means: "cpu" (the processor's memory) or "io" (a port, the Z80's io space)
    std::uint32_t address = 0;
    int width = 1;          // bytes
    std::map<std::string, std::string> names;   // spellings by dialect
};

// A block of a machine's address space, as its `registers` description says (kind region): [address, address + size), named.
struct HardwareRegion {
    std::string name;       // the prefix of the registers in it ("CP", "VIF0")
    std::string table;
    std::string space;
    std::uint32_t address = 0;
    std::uint32_t size = 0;
};

// The checkout's own knowledge, opened at the first ask (the base the engine's steps read when none is handed to them: the
// checkout is $CB_CHECKOUT_ROOT, else the one the running program belongs to; the base file is $CB_KNOWLEDGE_DB, else
// build/knowledge.v2.sqlite).
Knowledge& standard();

class Knowledge {
public:
    // `headers`: the header folders the prototypes are read from when they are asked for and the base has none made from
    // the folders as they are now.
    explicit Knowledge(Store& store, symbols::prototypes::HeaderFolders headers = {});
    ~Knowledge();
    Knowledge(const Knowledge&) = delete;
    Knowledge& operator=(const Knowledge&) = delete;

    Store& store() { return store_; }

    // ---- anything -------------------------------------------------------------------------------------------------
    // A piece (the ask is recorded as a read of the running step).
    std::optional<Value> known(std::string_view platform, std::string_view kind, std::string_view key);
    // Keep something found: written once, seen by every later ask in any process.
    void learn(std::string_view platform, std::string_view kind, std::string_view key, const Value& value,
               std::string_view source = {});
    // {key: value} of a kind (an authored kind is loaded from its file first when the file changed). A step reading it
    // reads the whole kind.
    Rows table(std::string_view platform, std::string_view kind);
    // What the emulator's stand-in for a system function does ({ax, dx, pop} for win16, with "arg<N>" and "heap" as
    // knowledge/models/win16.json says; {argc} for win32; {answers_in} for a ring-0 service), as a map without the keys
    // that start with "_"; nullopt when it is not modelled.
    std::optional<Value> api_model(std::string_view platform, std::string_view api);

    // ---- description files (describe.hpp: config/knowledge/<platform>/<domain>.json) -------------------------------
    // One entry of a domain: its object, as the file writes it (the ask is recorded as a read of platform:domain:id).
    std::optional<Value> entry(std::string_view platform, std::string_view domain, std::string_view id);
    // Every entry of a domain by id, in the file's order (a step reading it reads the whole domain).
    Rows entries(std::string_view platform, std::string_view domain);
    // The hardware registers of a platform (its `registers` domain): the entries of kind register, in the file's order. `table`
    // picks one group of them (a register entry's `table`: "ports", "io", "tia_write"; empty: all).
    std::vector<HardwareRegister> hardware_registers(std::string_view platform, std::string_view table = {});
    // The same as {address: name}. `dialect` asks for a spelling (the entry's `names`, {"rgbds": "rLCDC"}); the register's own
    // name when it has none for it.
    std::map<std::uint32_t, std::string> hardware_names(std::string_view platform, std::string_view table = {}, std::string_view dialect = {});
    // The blocks of a platform's address space (its `registers` domain): the entries of kind region, in the file's order (the first that
    // holds an address names it). `table` as for hardware_registers.
    std::vector<HardwareRegion> hardware_regions(std::string_view platform, std::string_view table = {});
    // The name an `ordinals` description (config/knowledge/<platform>/ordinals.json) gives to export `ordinal` of `module` ("xam.xex",
    // 1 -> "NetDll_WSAStartup"); nullopt when it names none. The module is as the description writes it (lower case).
    std::optional<std::string> export_name(std::string_view platform, std::string_view module, std::uint32_t ordinal);
    // The names of every export of `module` by number, as text ({"1": "NetDll_WSAStartup", ...}); a step reading it reads the domain.
    std::map<std::string, std::string> export_names(std::string_view platform, std::string_view module);
    // The names a `registers` description gives to places that are not registers: the entries of one `table` of any kind (a BIOS
    // routine's entry point, an OS call's code), as {address: name}. The place is the entry's `address`, else the first value of its
    // `select`. `dialect` as for hardware_names.
    std::map<std::uint32_t, std::string> table_names(std::string_view platform, std::string_view table, std::string_view dialect = {});
    // The domains a platform has description files for, in order of name.
    std::vector<std::string> domains(std::string_view platform);
    // Every description file checked against the schema: header, kinds, ids, shapes.
    ValidationReport validate();

    // ---- prototypes -----------------------------------------------------------------------------------------------
    // The headers' declarations, read once and kept in the base (made again when the header folders change).
    const symbols::prototypes::PrototypeTable& prototypes();
    // How the headers declare a Windows function, or nullptr. Capitals do not matter.
    const symbols::prototypes::Prototype* prototype(std::string_view platform, std::string_view name);
    std::optional<std::string> declaration(std::string_view platform, std::string_view name);
    std::string spelling(std::string_view name);

    // ---- names of bodies ------------------------------------------------------------------------------------------
    std::optional<BodyName> name(std::string_view body_digest);
    void name_body(std::string_view body_digest, std::string_view name, std::string_view source = "named by hand");

    // ---- signatures -----------------------------------------------------------------------------------------------
    // The runtime-library functions' signatures of a platform.
    std::vector<symbols::runtime_library::SignatureRow> signatures(std::string_view platform);
    // The runtime-library function `code` starts with, or nullopt (win16 or win32).
    std::optional<symbols::runtime_library::Identified> signature_match(std::string_view platform, ByteView code);

    // ---- what the argument-size readers remember ------------------------------------------------------------------
    // The headers' sizes and what scans of exports found, as the base keeps them.
    std::unique_ptr<symbols::api_arguments::Win32Memory> win32_memory();
    // The sizes found for 16-bit exports; `scope` is the product and way of reading the sizes belong to (their keys start
    // with it, so another product's sizes are never taken for these).
    std::unique_ptr<symbols::api_arguments::Win16Memory> win16_memory(std::string_view scope = {});

private:
    struct Impl;
    Store& store_;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::knowledge
