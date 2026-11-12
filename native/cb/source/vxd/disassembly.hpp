// cb/source/vxd/disassembly.hpp -- a recursive-descent 32-bit disassembler and MASM-style listing for Win95 VxDs.
//
// Takes the LE module of a Virtual Device Driver and reconstructs it as annotated 32-bit assembly. A VxD's executable
// object is a MIX of code and data (the DDB, jump tables, the service table, string and pointer tables all live inside it),
// so a linear sweep disassembles the data as garbage. This uses recursive descent instead: it starts from the real entry
// points the DDB records (control procedure, API procedures, service table) and follows control flow, finding the
// jump-table dispatchers as it goes. Whatever the descent never reaches as code is data.
//
// Three VxD-specific things are decoded:
//
//   1. The DDB (Device Descriptor Block), the driver's identity, becomes a Declare_Virtual_Device with its fields (name,
//      version, device id, control and API procedures, service table).
//   2. INT 20h service calls, `int 20h` and an inline DWORD (the service number in the low word, the device id in the high
//      word), become `VMMCall` / `VxDCall DEVICE.Service`; the DWORD belongs to the instruction, it is not disassembled.
//   3. Fix-ups resolve every zeroed operand to its real target; the control dispatch `jmp [eax*4 + table]` and its table
//      are recovered as a labelled jump.
//
//     const le::Module driver = le::Module::open("vkd.vxd");
//     const Disassembly code = Disassembly::run(driver, DdkNames::standard(ddk, design_dir));
//     std::string listing = emit_asm(code);
//
// What the DDK names (the services, the control messages, the debug symbols) is given as `DdkNames`; without it a service
// stays `VMM.Service_003Fh` and a handler `sub_1_0123`.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/base/corpus.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/formats/le.hpp"
#include "cb/source/vxd/ddk.hpp"
#include "cb/source/vxd/services.hpp"

namespace cb::source::vxd {

using Place = formats::le::Place;   // (object, offset in the object)

struct PlaceHash {
    std::size_t operator()(const Place& p) const noexcept {
        return static_cast<std::size_t>(p.object) * 0x9E3779B97F4A7C15ull ^ (static_cast<std::size_t>(p.offset) + 0x7F4A7C15u);
    }
};

// ---- the DDK's names ---------------------------------------------------------------------------------------------------------

// What the DDK says about a driver's insides: the service names by device (VMM.Get_System_Time), the system control messages
// (DEVICE_INIT, SYS_VM_INIT ...), and the debug symbol files of the 12 core drivers.
class DdkNames {
public:
    DdkNames() = default;   // nothing is named
    DdkNames(ServiceTables services, std::map<std::uint32_t, std::string> control_messages, DdkFolders folders)
        : services_(std::move(services)), control_messages_(std::move(control_messages)), folders_(std::move(folders)) {}
    // The DDK found as the folders say; what it does not have is left out.
    static DdkNames standard(const DdkFolders& ddk, const std::optional<std::filesystem::path>& design_dir);

    const ServiceTables& services() const { return services_; }
    const std::map<std::uint32_t, std::string>& control_messages() const { return control_messages_; }
    // {offset: name} of DEBUG/<device>.SYM, in the file's order; empty when there is none.
    OrderedMap<std::uint16_t, std::string> symbols(std::string_view device) const;

private:
    ServiceTables services_;
    std::map<std::uint32_t, std::string> control_messages_;
    DdkFolders folders_;
};

// ---- vocabulary --------------------------------------------------------------------------------------------------------------

// The canonical Win95 VxD device ids: the high word of an INT 20h service DWORD names the callee. nullopt for an id not in the table.
std::optional<std::string_view> device_name(std::uint32_t id);

// Where the fields of the DDB lie, from its start (the DDB_Next dword).
inline constexpr std::uint32_t kDdbControlProc = 0x18, kDdbV86ApiProc = 0x1C, kDdbPmApiProc = 0x20, kDdbServiceTable = 0x30,
                               kDdbServiceSize = 0x34;
inline constexpr std::uint32_t kDdbSize = 0x54;   // what the recovery treats as the DDB's bytes

// An INT 20h service call.
struct ServiceCall {
    std::uint32_t device = 0, service = 0;
    bool from_import = false;   // the DWORD is an import fix-up (device and service come from it), else it is inline
};

// One recovered instruction.
struct Instruction {
    std::uint32_t size = 0;
    std::string mnemonic, operands;
    Bytes raw;
    std::optional<formats::le::Fixup> fixup;   // the first fix-up inside the instruction, past its first byte
    std::optional<ServiceCall> service;        // set for a service call (mnemonic "int", operands "20h")

    // The target of a direct call, jump or loop ("0x401000" as its operand), nullopt for the others.
    std::optional<std::int64_t> direct_target() const;
};

// The number a direct branch writes as its operand ("0x401000"); nullopt for any other operand.
std::optional<std::int64_t> direct_operand(std::string_view operands);

// [start, end) byte ranges of `code` that are ASCII string DATA the disassembler would take for instructions. A run of 8 or more
// printable bytes qualifies only if it is NUL-terminated or long (20 or more), mostly letters (60%), and has a vowel: three
// gates that reject real code whose bytes happen to be printable (an `[esp+disp]` compare chain reads `D$ ;D$$u`). Nearly
// every string of the Win95 corpus passes and no code does; short vowel-less tags (`CD-XA001`) are missed.
std::vector<std::pair<std::size_t, std::size_t>> string_ranges(ByteView code);

// ---- the recovery -------------------------------------------------------------------------------------------------------------

// What the recovery found, as the engine hands it over (see Disassembly for what the parts mean).
struct Recovered {
    using FixupMap = OrderedMap<Place, formats::le::Fixup, PlaceHash>;
    struct ServiceTable {
        std::uint32_t object = 0;
        std::int64_t base = 0;
        std::size_t count = 0;
    };
    std::optional<formats::le::Ddb> ddb;
    FixupMap fixups;
    std::vector<Bytes> code;                              // by object (object 1 is code[0])
    std::vector<std::vector<bool>> covered;               // by object, by byte: an instruction covers it
    std::map<Place, Instruction> instructions;
    std::map<Place, std::string> labels;
    std::map<Place, std::vector<Place>> jump_tables;
    std::optional<ServiceTable> service_table;
};

class Disassembly {
public:
    // Recover the driver. `driver` must outlive the result.
    static Disassembly run(const formats::le::Module& driver, const DdkNames& names);
    Disassembly(const formats::le::Module& driver, Recovered recovered) : driver_(&driver), r_(std::move(recovered)) {}

    const formats::le::Module& driver() const { return *driver_; }
    // The device descriptor block, nullopt when the module has none.
    const std::optional<formats::le::Ddb>& ddb() const { return r_.ddb; }
    // Every fix-up, in the order the module lists them (the order in which the recovery looks for entry points).
    using FixupMap = Recovered::FixupMap;
    const FixupMap& fixups() const { return r_.fixups; }
    std::optional<formats::le::Fixup> fixup_at(const Place& at) const;
    // The bytes of object `object` (1-based) as loaded; empty for an object that is not there.
    ByteView code(std::uint32_t object) const;
    // Instruction starts, sorted by place.
    const std::map<Place, Instruction>& instructions() const { return r_.instructions; }
    // Names of the places that are branch targets, call targets, entry points ...
    const std::map<Place, std::string>& labels() const { return r_.labels; }
    // Jump and pointer tables: (object, table base) -> the places its entries point at.
    const std::map<Place, std::vector<Place>>& jump_tables() const { return r_.jump_tables; }
    using ServiceTable = Recovered::ServiceTable;
    const std::optional<ServiceTable>& service_table() const { return r_.service_table; }
    // How many bytes of the object the instructions cover.
    std::size_t code_bytes(std::uint32_t object) const;
    // Whether an instruction covers the byte.
    bool is_code(const Place& at) const;

private:
    const formats::le::Module* driver_;
    Recovered r_;
};

// ---- the listing pieces ----------------------------------------------------------------------------------------------------

// `VMMCall VMM.Get_System_Time`, `VxDCall VKD.Service_0003h` (a service the DDK does not name), with `  ; (import-fixup)` when the
// DWORD of the call is an import fix-up.
std::string service_text(const ServiceCall& call, const DdkNames& names);

// The label a place has, nullopt when it has none.
using LabelLookup = std::function<std::optional<std::string>(const Place&)>;
// The operand text of an instruction and whether it resolved to a label: a branch to a labelled place reads as that label (for a
// cross-object branch, rel32 = 0 and a fix-up, the label comes from the fix-up's target), so the listing reads as symbolic control flow.
std::pair<std::string, bool> operand_text(const Instruction& insn, std::uint32_t object, const LabelLookup& label_of);

// ---- the listing --------------------------------------------------------------------------------------------------------------

// Annotated 32-bit assembly for the whole driver; `names` gives the DDK's service names.
std::string emit_asm(const Disassembly& code, const DdkNames& names);
// The same from the module (the recovery is run).
std::string emit_asm(const formats::le::Module& driver, const DdkNames& names);

// Faithfulness of the recovery: it is a VIEW of the real bytes, so every executable byte must be claimed by exactly one
// instruction or by none (data). Two instructions over one byte mean a wrongly seeded entry point; an instruction past the
// end of the object means an over-read. No overlap and no overrun anywhere is the proof.
struct ObjectCheck {
    std::uint32_t object = 0;
    std::size_t size = 0, code = 0, data = 0, overlap = 0, overrun = 0;
};
std::vector<ObjectCheck> verify(const Disassembly& code);

// What `disassemble_file` says of one module.
struct ModuleStats {
    std::string name;
    std::size_t instructions = 0;
    double coverage_percent = 0;   // of the executable objects' bytes that are code
    std::size_t labels = 0, jump_tables = 0;
    std::optional<std::string> device;
    std::optional<std::uint32_t> device_id;
};
// Disassemble a VxD file (LE, LX or a W3 bundle of several modules); with `out_folder`, one <module>.asm per module is written there.
std::vector<ModuleStats> disassemble_file(const std::filesystem::path& file, const DdkNames& names, const std::optional<std::filesystem::path>& out_folder = std::nullopt);

// ---- the command --------------------------------------------------------------------------------------------------------------

struct Environment {
    const base::Corpus& corpus;                // the folder `--verify` and `--all` take by default
    const DdkNames& names;
    std::filesystem::path listing_folder;      // where `--all` writes by default (the state folder's vxd/listing)
};

// `FILE ...` the listing of each file | `--verify [FOLDER]` the faithfulness check over a folder | `--all [FOLDER [OUT]]` every
// driver of a folder to <module>.asm. Returns the exit code.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::vxd
