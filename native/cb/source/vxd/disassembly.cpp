// cb-unity: alone (file-local helpers named for this module)
#include "cb/source/vxd/disassembly.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>
#include <unordered_set>

#include "cb/core/text.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/source/vxd/symbols.hpp"

namespace cb::source::vxd {

namespace le = formats::le;

namespace {

// ---- the canonical device ids ------------------------------------------------------------------------------------------------

struct DeviceName {
    std::uint32_t id;
    std::string_view name;
};

constexpr std::array kDevices = std::to_array<DeviceName>({
    {0x0001, "VMM"}, {0x0002, "DEBUG"}, {0x0003, "VPICD"}, {0x0004, "VDMAD"}, {0x0005, "VTD"},
    {0x0006, "V86MMGR"}, {0x0007, "PAGESWAP"}, {0x0008, "PARITY"}, {0x0009, "REBOOT"}, {0x000A, "VDD"},
    {0x000B, "VSD"}, {0x000C, "VMD"}, {0x000D, "VKD"}, {0x000E, "VCD"}, {0x000F, "VPD"},
    {0x0010, "BLOCKDEV"}, {0x0011, "VMCPD"}, {0x0012, "EBIOS"}, {0x0013, "BIOSXLAT"}, {0x0014, "VNETBIOS"},
    {0x0015, "DOSMGR"}, {0x0016, "VMPOLL"}, {0x0017, "SHELL"}, {0x0018, "VMM_STRING"}, {0x0019, "VPROD"},
    {0x001A, "DOSNET"}, {0x001B, "VFD"}, {0x001C, "VDD2"}, {0x001D, "WINLOAD"}, {0x001E, "VXDLDR"},
    {0x0020, "INT13"}, {0x0021, "PAGEFILE"}, {0x0022, "SCSI"}, {0x0023, "MCA_POS"}, {0x0024, "SCSIFD"},
    {0x0026, "VPEND"}, {0x0027, "APM"}, {0x0028, "VPOWERD"}, {0x0029, "VXDLDR"}, {0x002A, "NDIS"},
    {0x002B, "BIOS"}, {0x002C, "VWIN32"}, {0x002D, "VCOMM"}, {0x002E, "SPOOLER"}, {0x0032, "VCOMM"},
    {0x0033, "CONFIGMG"}, {0x0036, "VFBACKUP"}, {0x0037, "ENABLE"}, {0x0038, "VCOND"}, {0x003D, "BIOS"},
    {0x0040, "IFSMgr"}, {0x0041, "VCDFSD"}, {0x0442, "VTDAPI"}, {0x0448, "PERF"}, {0x0449, "VWIN32"},
    {0x044A, "VFAT"}, {0x0450, "MRCI2"}, {0x0454, "NWLINK"}, {0x0480, "NDIS2SUP"},
});

// ---- the recovery's bookkeeping -----------------------------------------------------------------------------------------------

constexpr std::uint8_t kIsCode = 1;    // an instruction covers the byte
constexpr std::uint8_t kIsData = 2;    // known to be data (tables, the DDB): never code
constexpr std::uint8_t kIsEntry = 4;   // an authoritative entry point (a DDB procedure, a service handler): no instruction may span it

constexpr std::uint32_t kBigSegment = 0x2000;        // object flag: a 32-bit segment
constexpr std::int64_t kAliasMask = 0x7FFFFFFF;      // a fix-up target may carry the high linear alias bit
constexpr std::size_t kWindow = 16;                  // the bytes handed to the decoder for one instruction
constexpr std::size_t kServiceCallSize = 6;          // `int 20h` and its DWORD
constexpr std::uint8_t kInt = 0xCD, kVxdService = 0x20;
constexpr std::size_t kMaxJumpTable = 1024;          // entries looked for after the base of a dispatch table
constexpr std::size_t kMaxServices = 4096;           // service-table entries looked for when the DDB gives no size
constexpr std::uint32_t kMaxKnownServices = 8192;    // a DDB size past this is not believed
constexpr int kMaxGapRounds = 8;                     // gap recovery: reach code the control graph does not
constexpr int kMaxRemarkRounds = 32;                 // reclassifying one span can expose targets a longer mis-decode shadowed
constexpr int kMaxSplitRounds = 8;
constexpr std::size_t kPointerTableRun = 3;          // stride-4 internal fix-ups in a row that make a pointer table
constexpr std::size_t kLongestInstruction = 15;
constexpr int kControlProcInstructions = 10;         // how far into the control procedure its dispatch is looked for

const le::InternalFixup* internal_of(const le::Fixup& f) { return std::get_if<le::InternalFixup>(&f); }
const le::InternalFixup* internal_of(const std::optional<le::Fixup>& f) { return f ? internal_of(*f) : nullptr; }

// The place an internal fix-up points at; nullopt for a 16-bit selector fix-up, which has no offset.
std::optional<Place> target_place(const le::InternalFixup& f) {
    if (!f.target) return std::nullopt;
    return Place{f.object, static_cast<std::int64_t>(*f.target)};
}

bool is_terminator(std::string_view m) { return m == "ret" || m == "retn" || m == "retf" || m == "iret" || m == "iretd" || m == "iretw"; }

bool is_transfer(std::string_view m) { return m == "call" || m.starts_with('j') || m.starts_with("loop"); }


std::uint64_t little_endian_value(ByteView bytes) {
    std::uint64_t v = 0;
    for (std::size_t i = 0; i < bytes.size() && i < 8; ++i) v |= static_cast<std::uint64_t>(bytes[i]) << (8 * i);
    return v;
}

class Engine {
public:
    Engine(const le::Module& driver, const DdkNames& names) : driver_(driver), names_(names) {}

    Recovered run() {
        const std::span<const le::Object> objects = driver_.objects();
        for (const le::Object& o : objects) {
            r_.code.push_back(driver_.object_data(o));
            bits_.emplace_back(r_.code.back().size(), std::uint8_t{0});
        }
        for (const auto& [place, fixup] : driver_.fixups()) r_.fixups.insert_or_assign(place, fixup);   // a place met twice keeps its first position, the later fix-up
        r_.ddb = driver_.ddb();

        const std::vector<Place> seeds = find_seeds();   // the entry points (control, API, service) first
        // Carve the ASCII string data out of every executable object BEFORE the descent, so that the gap seeder's
        // fix-up-to-string targets cannot decode strings as bogus code and cascade past them. Never across an authoritative entry point.
        for (std::size_t i = 0; i < objects.size(); ++i) {
            if (!objects[i].executable()) continue;
            for (const auto& [from, to] : string_ranges(r_.code[i])) {
                const std::uint32_t oi = static_cast<std::uint32_t>(i + 1);
                if (!has_bits(oi, from, to, kIsEntry)) mark_data(oi, static_cast<std::int64_t>(from), static_cast<std::int64_t>(to - from));
            }
        }
        descend(seeds);
        for (int round = 0; round < kMaxGapRounds; ++round) {
            const std::vector<Place> more = gap_seeds();
            if (more.empty()) break;
            descend(more);
        }
        remark_data_from_fixups();      // reclassify data the sweep decoded as code
        remark_pointer_tables();        // reclassify mis-swept pointer tables as data
        remark_split_fixup_sources();   // reclassify pointers that straddle a mis-decode
        collect_labels();
        apply_ddk_names();

        r_.covered.clear();
        for (const Bytes& code : r_.code) r_.covered.emplace_back(code.size(), false);
        for (const auto& [at, insn] : r_.instructions) {
            for (std::uint32_t k = 0; k < insn.size; ++k) set_covered(at.object, at.offset + k);
        }
        return std::move(r_);
    }

private:
    // ---- the byte states ---------------------------------------------------------------------------------------------------

    std::uint8_t* state_of(std::uint32_t object, std::int64_t offset) {
        if (object < 1 || object > bits_.size() || offset < 0 || static_cast<std::uint64_t>(offset) >= bits_[object - 1].size()) return nullptr;
        return &bits_[object - 1][static_cast<std::size_t>(offset)];
    }
    bool has_bit(std::uint32_t object, std::int64_t offset, std::uint8_t bit) {
        const std::uint8_t* s = state_of(object, offset);
        return s != nullptr && (*s & bit) != 0;
    }
    // Whether any byte of [from, to) has one of the bits.
    bool has_bits(std::uint32_t object, std::uint64_t from, std::uint64_t to, std::uint8_t bit) {
        for (std::uint64_t k = from; k < to; ++k) {
            if (has_bit(object, static_cast<std::int64_t>(k), bit)) return true;
        }
        return false;
    }
    void add_bit(std::uint32_t object, std::int64_t start, std::int64_t length, std::uint8_t bit) {
        for (std::int64_t k = start; k < start + length; ++k) {
            if (std::uint8_t* s = state_of(object, k)) *s = static_cast<std::uint8_t>(*s | bit);
        }
    }
    void clear_bit(std::uint32_t object, std::int64_t start, std::int64_t length, std::uint8_t bit) {
        for (std::int64_t k = start; k < start + length; ++k) {
            if (std::uint8_t* s = state_of(object, k)) *s = static_cast<std::uint8_t>(*s & ~bit);
        }
    }
    void mark_data(std::uint32_t object, std::int64_t start, std::int64_t length) { add_bit(object, start, length, kIsData); }
    // True iff [at, at+size) is entirely unclaimed: decoding there creates no overlap.
    bool is_free(std::uint32_t object, std::int64_t at, std::int64_t size) {
        for (std::int64_t k = at; k < at + size; ++k) {
            if (has_bit(object, k, kIsCode)) return false;
        }
        return true;
    }
    void set_covered(std::uint32_t object, std::int64_t offset) {
        if (object >= 1 && object <= r_.covered.size() && offset >= 0 && static_cast<std::uint64_t>(offset) < r_.covered[object - 1].size()) {
            r_.covered[object - 1][static_cast<std::size_t>(offset)] = true;
        }
    }

    const le::Object* object_of(std::uint32_t object) const {
        const std::span<const le::Object> all = driver_.objects();
        return object >= 1 && object <= all.size() ? &all[object - 1] : nullptr;
    }
    bool is_executable(std::uint32_t object) const {
        const le::Object* o = object_of(object);
        return o != nullptr && o->executable();
    }
    bool is_32bit(std::uint32_t object) const {
        const le::Object* o = object_of(object);
        return o == nullptr || (o->flags & kBigSegment) != 0;   // a USE16 segment decodes as 16-bit code
    }
    // The fix-up at a place, nullptr when there is none; it lives as long as the recovery's table of fix-ups (which no step adds to).
    const le::Fixup* fixup_at(std::uint32_t object, std::int64_t offset) const { return r_.fixups.find(Place{object, offset}); }
    // The same when it is an internal one.
    const le::InternalFixup* internal_at(std::uint32_t object, std::int64_t offset) const {
        const le::Fixup* f = fixup_at(object, offset);
        return f != nullptr ? std::get_if<le::InternalFixup>(f) : nullptr;
    }

    // The first instruction of `window` read as code of the object's width at `address`, nullopt when nothing decodes.
    std::optional<platforms::Insn> decode(std::uint32_t object, std::uint64_t address) {
        const Bytes& code = r_.code[object - 1];
        const ByteView window = ByteView(code).subspan(address, std::min(kWindow, code.size() - address));
        std::vector<platforms::Insn> insns = decoders_.get(platforms::Arch::X86, is_32bit(object) ? 32 : 16).disasm(window, address, 1);
        if (insns.empty()) return std::nullopt;
        return std::move(insns.front());
    }

    // ---- the entry points -------------------------------------------------------------------------------------------------

    std::vector<Place> find_seeds() {
        std::vector<Place> seeds;
        const std::span<const le::Object> objects = driver_.objects();
        if (!r_.ddb) {   // no DDB: fall back to the first byte of each executable object
            for (const le::Object& o : objects) {
                if (o.executable()) seeds.push_back(Place{o.index, 0});
            }
            return seeds;
        }
        const le::Ddb& db = *r_.ddb;
        const std::uint32_t oi = db.object;
        mark_data(oi, db.rva, kDdbSize);   // the DDB itself is data, not code
        std::string device = "Device";
        if (!db.name.empty()) device = db.name;
        struct EntryProcedure {
            std::uint32_t field;
            const char* suffix;   // what the label adds to the device's name
        };
        const EntryProcedure procedures[] = {{kDdbControlProc, "_Control"}, {kDdbV86ApiProc, "_V86_API"}, {kDdbPmApiProc, "_PM_API"}};
        for (const EntryProcedure& procedure : procedures) {
            const le::InternalFixup* f = internal_at(oi, db.rva + procedure.field);
            const auto target = f ? target_place(*f) : std::nullopt;
            if (target && is_executable(target->object)) {
                seeds.push_back(*target);
                add_bit(target->object, target->offset, 1, kIsEntry);
                std::string label = device;
                label += procedure.suffix;
                r_.labels[*target] = std::move(label);
            }
        }
        // the service table: DDB_Service_Table_Ptr points at DDB_Service_Size dword procedures
        const le::InternalFixup* table = internal_at(oi, db.rva + kDdbServiceTable);
        const auto base_place = table ? target_place(*table) : std::nullopt;
        if (base_place) {
            const std::uint32_t table_object = base_place->object;
            const std::int64_t base = base_place->offset;
            const Bytes& ddb_code = r_.code[oi - 1];
            const std::uint64_t size_at = static_cast<std::uint64_t>(db.rva + kDdbServiceSize);
            const std::uint64_t size = size_at < ddb_code.size() ? little_endian_value(ByteView(ddb_code).subspan(size_at, std::min<std::size_t>(4, ddb_code.size() - size_at))) : 0;
            const bool known = size > 0 && size <= kMaxKnownServices;   // DDB_Service_Table_Size is authoritative
            const std::size_t slots = known ? static_cast<std::size_t>(size) : kMaxServices;   // (VMM has 406: a fixed 256 cap dropped the rest)
            std::size_t count = 0;
            for (std::size_t j = 0; j < slots; ++j) {
                const le::InternalFixup* entry = internal_at(table_object, base + static_cast<std::int64_t>(4 * j));
                const auto handler = entry ? target_place(*entry) : std::nullopt;
                if (!handler) {   // a reserved slot (dd 0, no fix-up): with a known size skip it and go on, else the first gap ends the run
                    if (known) {
                        ++count;
                        continue;
                    }
                    break;
                }
                if (is_executable(handler->object)) {
                    seeds.push_back(*handler);
                    add_bit(handler->object, handler->offset, 1, kIsEntry);   // a service handler always starts an instruction
                    // this driver's own DDK service name, else its number
                    std::string label;
                    if (const std::optional<std::string> known_name = names_.services().name(db.name, static_cast<std::uint32_t>(j)); known_name && !known_name->empty()) {
                        label = *known_name;
                    } else {
                        std::string owner = "Dev";
                        if (!db.name.empty()) owner = db.name;
                        label = std::format("{}_Service_{:02}", owner, j);
                    }
                    r_.labels.try_emplace(*handler, std::move(label));
                }
                ++count;
            }
            if (count != 0) {
                r_.service_table = Recovered::ServiceTable{table_object, base, count};
                mark_data(table_object, base, static_cast<std::int64_t>(count * 4));   // the service table is a pointer array
            }
        }
        return seeds;
    }

    // A fix-up target or a function prologue (55 8B EC) in an unreached, non-DDB gap of an executable object: plausible code
    // entry points the descent missed.
    std::vector<Place> gap_seeds() {
        std::vector<Place> seeds;
        const std::span<const le::Object> objects = driver_.objects();
        for (std::size_t i = 0; i < objects.size(); ++i) {
            if (!objects[i].executable()) continue;
            const std::uint32_t oi = objects[i].index;
            const Bytes& code = r_.code[i];
            const std::int64_t n = static_cast<std::int64_t>(code.size());
            for (const auto& [at, fixup] : r_.fixups) {   // internal fix-up targets that are unclaimed and look like an instruction start
                const le::InternalFixup* f = internal_of(fixup);
                if (f == nullptr || f->object != oi || !f->target) continue;
                const std::int64_t t = static_cast<std::int64_t>(*f->target) & kAliasMask;   // mask the alias bit
                if (t < 0 || t >= n || has_bit(oi, t, kIsCode | kIsData)) continue;
                if (looks_like_code(oi, static_cast<std::uint64_t>(t))) seeds.push_back(Place{oi, t});
            }
            for (std::int64_t k = 0; k + 2 < n;) {   // a bare `push ebp; mov ebp, esp` in a gap
                if (code[static_cast<std::size_t>(k)] == 0x55 && code[static_cast<std::size_t>(k) + 1] == 0x8B && code[static_cast<std::size_t>(k) + 2] == 0xEC &&
                    !has_bit(oi, k, kIsCode | kIsData)) {
                    seeds.push_back(Place{oi, k});
                    k += 3;
                } else {
                    ++k;
                }
            }
        }
        return seeds;
    }

    // A cheap gate for a gap seed: a valid instruction decodes there and the bytes are not a run of NUL padding (which decodes
    // as `add [eax],al` forever).
    bool looks_like_code(std::uint32_t object, std::uint64_t at) {
        if (r_.code[object - 1][at] == 0x00) return false;
        return decode(object, at).has_value();
    }

    // ---- the descent -------------------------------------------------------------------------------------------------------

    void descend(const std::vector<Place>& seeds) {
        std::vector<Place> work = seeds;
        while (!work.empty()) {
            const Place start = work.back();
            work.pop_back();
            if (start.object < 1 || start.object > r_.code.size()) continue;
            const std::uint32_t oi = start.object;
            const Bytes& code = r_.code[oi - 1];
            const std::int64_t n = static_cast<std::int64_t>(code.size());
            std::int64_t a = start.offset;
            if (a < 0 || a >= n) continue;
            while (a < n) {
                const Place here{oi, a};
                if (r_.instructions.contains(here)) break;
                if (has_bit(oi, a, kIsCode | kIsData)) break;   // mid-instruction of earlier code, or a known data table: abandon this walk
                const std::size_t at = static_cast<std::size_t>(a);

                // a VxD service call: CD 20 and an inline DWORD
                if (code[at] == kInt && a + 5 < n && code[at + 1] == kVxdService) {
                    if (!is_free(oi, a, kServiceCallSize)) break;
                    if (has_bits(oi, static_cast<std::uint64_t>(a + 1), static_cast<std::uint64_t>(a + 6), kIsEntry)) break;   // must not span a service or DDB entry point
                    ServiceCall call;
                    const le::Fixup* site = fixup_at(oi, a + 2);
                    if (const auto* imported = site != nullptr ? std::get_if<le::ImportFixup>(site) : nullptr) {
                        call = ServiceCall{imported->module, imported->ordinal, true};
                    } else {
                        const std::uint32_t dword = static_cast<std::uint32_t>(little_endian_value(ByteView(code).subspan(at + 2, 4)));
                        call = ServiceCall{dword >> 16, dword & 0xFFFF, false};
                    }
                    Instruction insn;
                    insn.size = kServiceCallSize;
                    insn.mnemonic = "int";
                    insn.operands = "20h";
                    insn.raw.assign(code.begin() + static_cast<std::ptrdiff_t>(at), code.begin() + static_cast<std::ptrdiff_t>(at + kServiceCallSize));
                    insn.service = call;
                    r_.instructions[here] = std::move(insn);
                    add_bit(oi, a, kServiceCallSize, kIsCode);
                    a += kServiceCallSize;
                    continue;
                }

                const std::optional<platforms::Insn> decoded = decode(oi, at);
                if (!decoded) break;
                const std::int64_t size = decoded->size;
                if (!is_free(oi, a, size)) break;   // would overlap authoritative code
                if (has_bits(oi, static_cast<std::uint64_t>(a + 1), static_cast<std::uint64_t>(a + size), kIsEntry)) break;   // would swallow an authoritative entry point: let it start fresh
                Instruction insn;
                insn.size = decoded->size;
                insn.mnemonic = decoded->mnemonic;
                insn.operands = decoded->op_str;
                insn.raw.assign(code.begin() + static_cast<std::ptrdiff_t>(at), code.begin() + static_cast<std::ptrdiff_t>(at) + size);
                for (std::int64_t k = a + 1; k < a + size; ++k) {   // the first fix-up inside the instruction
                    if (const le::Fixup* f = r_.fixups.find(Place{oi, k})) {
                        insn.fixup = *f;
                        break;
                    }
                }
                const std::string mnemonic = insn.mnemonic;
                const std::string operands = insn.operands;
                const std::optional<le::Fixup> fixup = insn.fixup;
                r_.instructions[here] = std::move(insn);
                add_bit(oi, a, size, kIsCode);
                if (mnemonic == "jmp" && operands.find('[') != std::string::npos && operands.find("*4") != std::string::npos) {   // an indirect jump through a table: jmp dword ptr [reg*4 (+ base)]
                    jump_table(oi, a, size, work);
                    break;
                }
                if (is_terminator(mnemonic)) break;
                if (mnemonic == "jmp") {   // a direct unconditional jump is a tail
                    follow(oi, operands, fixup, work);
                    break;
                }
                if (is_transfer(mnemonic)) follow(oi, operands, fixup, work);   // a call or conditional jump falls through to the next instruction
                a += size;
            }
        }
    }

    // Queue a branch target. A relocated (cross-object) branch has rel32 = 0 and a fix-up that names the real destination; a plain
    // intra-object branch reads its operand.
    void follow(std::uint32_t object, const std::string& operands, const std::optional<le::Fixup>& fixup, std::vector<Place>& work) {
        if (const le::InternalFixup* f = internal_of(fixup)) {
            if (f->target) work.push_back(Place{f->object, static_cast<std::int64_t>(*f->target) & kAliasMask});   // (a selector fix-up has no offset to follow)
            return;
        }
        if (const auto t = direct_operand(operands)) work.push_back(Place{object, *t});
    }

    void jump_table(std::uint32_t object, std::int64_t at, std::int64_t size, std::vector<Place>& work) {
        std::optional<Place> base;   // the table's base is the fix-up inside this instruction
        for (std::int64_t k = at + 1; k < at + size; ++k) {
            if (const le::InternalFixup* f = internal_at(object, k)) {
                base = target_place(*f);
                break;
            }
        }
        if (!base) return;
        std::vector<Place> targets;
        for (std::size_t j = 0; j < kMaxJumpTable; ++j) {
            const le::InternalFixup* entry = internal_at(base->object, base->offset + static_cast<std::int64_t>(4 * j));
            const auto target = entry ? target_place(*entry) : std::nullopt;
            if (!target) break;
            targets.push_back(*target);
            if (is_executable(target->object)) work.push_back(*target);
        }
        if (targets.empty()) return;
        const std::int64_t bytes = static_cast<std::int64_t>(targets.size() * 4);
        r_.jump_tables[*base] = std::move(targets);
        mark_data(base->object, base->offset, bytes);   // the table's bytes are data, not code
    }

    // ---- reclassifying what the descent got wrong -------------------------------------------------------------------------------

    // The start of the instruction that covers each byte, per object (-1: none).
    std::vector<std::vector<std::int64_t>> cover_map() const {
        std::vector<std::vector<std::int64_t>> cover;
        for (const Bytes& code : r_.code) cover.emplace_back(code.size(), -1);
        for (const auto& [at, insn] : r_.instructions) {
            for (std::uint32_t k = 0; k < insn.size; ++k) {
                const std::int64_t byte = at.offset + k;
                if (byte >= 0 && static_cast<std::uint64_t>(byte) < cover[at.object - 1].size()) cover[at.object - 1][static_cast<std::size_t>(byte)] = at.offset;
            }
        }
        return cover;
    }
    static std::optional<std::int64_t> covering(const std::vector<std::vector<std::int64_t>>& cover, std::uint32_t object, std::int64_t offset) {
        if (object < 1 || object > cover.size() || offset < 0 || static_cast<std::uint64_t>(offset) >= cover[object - 1].size()) return std::nullopt;
        const std::int64_t start = cover[object - 1][static_cast<std::size_t>(offset)];
        return start < 0 ? std::nullopt : std::optional<std::int64_t>(start);
    }
    // Drop the instruction at `at` and make its bytes data.
    void demote_to_data(const Place& at) {
        const auto found = r_.instructions.find(at);
        if (found == r_.instructions.end()) return;
        const std::int64_t size = found->second.size;
        r_.instructions.erase(found);
        clear_bit(at.object, at.offset, size, kIsCode);
        mark_data(at.object, at.offset, size);
    }

    // A relocation always targets a clean address (a label the linker resolves), never the middle of a real instruction. So an
    // internal fix-up target that lands INSIDE a decoded instruction proves that "instruction" is data the sweep mis-decoded (a
    // pointer or variable in a data region). The covering instruction becomes data, so that a label can be placed at the target and the
    // loadable emit keeps the bytes or writes `dd offset` for the pointers instead of re-encoding them as an instruction (which can change the
    // bytes and leave a pointer holding a garbage address). Repeated: reclassifying one span can expose targets a longer mis-decode shadowed.
    void remark_data_from_fixups() {
        for (int round = 0; round < kMaxRemarkRounds; ++round) {
            const auto cover = cover_map();
            std::set<Place> victims;
            for (const auto& [at, fixup] : r_.fixups) {
                const le::InternalFixup* f = internal_of(fixup);
                if (f == nullptr || !f->target) continue;
                const std::int64_t target = *f->target;
                const auto start = covering(cover, f->object, target);
                if (start && *start != target) victims.insert(Place{f->object, *start});   // the target sits mid-instruction
            }
            if (victims.empty()) break;
            for (const Place& v : victims) demote_to_data(v);
        }
    }

    // A run of internal fix-up SOURCES at 4-byte stride is a pointer table (a dispatch table, a vtable, a handler array): each source
    // dword is a relocated pointer, i.e. data. The sweep can mis-decode such a table as code (it is reached only through an indirect
    // `[base+i*4]` load, never by the control graph); the loadable emit then relocates only the entries that happen to fall on an imm32
    // operand and drops the rest -- an unrelocated pointer that faults the moment the dispatcher jumps through it. The whole run becomes
    // data, so each entry is a `dd offset` and one relocation. Nothing changes when the table is already data.
    void remark_pointer_tables() {
        std::map<std::uint32_t, std::set<std::int64_t>> sources;   // object -> sorted source offsets
        for (const auto& [at, fixup] : r_.fixups) {
            const le::InternalFixup* f = internal_of(fixup);
            if (f != nullptr && f->target) sources[at.object].insert(at.offset);
        }
        for (const auto& [oi, offsets] : sources) {
            const std::vector<std::int64_t> offs(offsets.begin(), offsets.end());
            for (std::size_t i = 0; i < offs.size();) {
                std::size_t j = i;
                while (j + 1 < offs.size() && offs[j + 1] == offs[j] + 4) ++j;
                if (j - i + 1 >= kPointerTableRun) {
                    const std::int64_t lo = offs[i], hi = offs[j] + 4;
                    // an instruction that overlaps the table is a mis-decode (instructions are at most 15 bytes, so the scan starts that far before)
                    std::vector<Place> overlapping;
                    for (auto it = r_.instructions.lower_bound(Place{oi, lo - static_cast<std::int64_t>(kLongestInstruction)});
                         it != r_.instructions.end() && it->first.object == oi && it->first.offset < hi; ++it) {
                        if (it->first.offset + static_cast<std::int64_t>(it->second.size) > lo) overlapping.push_back(it->first);
                    }
                    for (const Place& p : overlapping) demote_to_data(p);
                }
                i = j + 1;
            }
        }
    }

    // A single internal fix-up SOURCE dword [S, S+4) is a relocated pointer: four contiguous data bytes. If the sweep decoded code that
    // overlaps it only PARTLY (the dword straddles a gap and code, or two instructions -- a pointer whose high bytes the sweep took for the
    // start of the next function), neither the per-instruction emit nor the data emit can place the relocation: a `dd offset` would cross
    // the boundary, the pointer is kept as bytes and loses its fix-up. Every instruction that straddles such a dword becomes data and the
    // dword is marked data, so one clean `dd offset` is written. A dword wholly inside ONE instruction's operand (mov/push offset X) is the
    // normal relocation and is left alone.
    void remark_split_fixup_sources() {
        for (int round = 0; round < kMaxSplitRounds; ++round) {
            const auto cover = cover_map();
            bool changed = false;
            for (const auto& [at, fixup] : r_.fixups) {
                const le::InternalFixup* f = internal_of(fixup);
                if (f == nullptr || !f->target) continue;
                std::set<std::int64_t> starts;
                for (std::int64_t d = 0; d < 4; ++d) {
                    if (const auto s = covering(cover, at.object, at.offset + d)) starts.insert(*s);
                }
                if (starts.empty()) continue;   // the dword is wholly in a gap: the data emit handles it
                if (starts.size() == 1) {
                    const std::int64_t st = *starts.begin();
                    const auto row = r_.instructions.find(Place{at.object, st});
                    if (row != r_.instructions.end() && st <= at.offset && at.offset + 4 <= st + static_cast<std::int64_t>(row->second.size)) continue;   // inside one instruction's operand
                }
                for (const std::int64_t st : starts) demote_to_data(Place{at.object, st});   // straddles a boundary
                mark_data(at.object, at.offset, 4);
                changed = true;
            }
            if (!changed) break;
        }
    }

    // ---- labels -------------------------------------------------------------------------------------------------------------

    // Object-qualified (two objects often both start at offset 0): sub_<object>_<offset> for a call target, loc_ for the rest.
    static std::string label_name(const Place& at, bool is_call_target) {
        return std::format("{}_{}_{:04X}", is_call_target ? "sub" : "loc", at.object, at.offset);
    }

    void collect_labels() {
        std::set<Place> call_targets;
        for (const auto& [at, insn] : r_.instructions) {   // a call destination reads as a procedure
            if (insn.mnemonic != "call") continue;
            if (const le::InternalFixup* f = internal_of(insn.fixup)) {
                if (const auto t = target_place(*f)) call_targets.insert(*t);
            } else if (const auto t = insn.direct_target()) {
                call_targets.insert(Place{at.object, *t});
            }
        }
        const auto name_it = [&](const Place& at) {
            r_.labels.try_emplace(at, label_name(at, call_targets.contains(at)));
        };
        for (const auto& [at, insn] : r_.instructions) {   // branch and call targets
            if (const auto t = insn.direct_target(); t && r_.instructions.contains(Place{at.object, *t})) name_it(Place{at.object, *t});
            if (const le::InternalFixup* f = internal_of(insn.fixup)) {   // a cross-object branch or reference
                if (const auto t = target_place(*f); t && r_.instructions.contains(*t)) name_it(*t);
            }
        }
        for (const auto& [base, targets] : r_.jump_tables) {   // jump-table destinations
            for (const Place& t : targets) {
                if (r_.instructions.contains(t)) name_it(t);
            }
        }
        for (const auto& [at, fixup] : r_.fixups) {   // fix-up targets that are code
            const le::InternalFixup* f = internal_of(fixup);
            if (f == nullptr) continue;
            if (const auto t = target_place(*f); t && r_.instructions.contains(*t)) name_it(*t);
        }
    }

    // Real names from the DDK over the anonymous sub_ and loc_ labels: the control-dispatch handlers get the message they service
    // (DEVICE_INIT, SYS_VM_INIT ...), and the 12 core drivers that have a debug .SYM get their internal function names where a
    // symbol's offset lands on a recovered function start.
    void apply_ddk_names() {
        if (!r_.ddb) return;
        const std::string& device = r_.ddb->name;
        std::set<std::string> used;   // never introduce a duplicate label
        for (const auto& [at, name] : r_.labels) used.insert(name);
        const auto assign = [&](const Place& key, const std::string& name) {
            if (used.contains(name) || !r_.instructions.contains(key)) return;
            const auto current = r_.labels.find(key);
            if (current == r_.labels.end() || !(current->second.starts_with("sub_") || current->second.starts_with("loc_"))) return;   // keep the semantic names
            current->second = name;
            used.insert(name);
        };

        const std::string control = device + "_Control";
        const auto control_label = std::find_if(r_.labels.begin(), r_.labels.end(), [&](const auto& kv) { return kv.second == control; });
        if (control_label != r_.labels.end()) {   // the control handlers are named for their message
            const std::map<std::uint32_t, std::string>& messages = names_.control_messages();
            const Place entry = control_label->first;
            std::int64_t at = entry.offset, base = 0;
            for (int i = 0; i < kControlProcInstructions; ++i) {
                const auto row = r_.instructions.find(Place{entry.object, at});
                if (row == r_.instructions.end()) break;
                const Instruction& insn = row->second;
                if (insn.mnemonic == "sub" && text::replace_all(insn.operands, " ", "").starts_with("eax,")) {
                    const std::size_t comma = insn.operands.find(',');
                    if (const auto value = text::parse_int<std::int64_t>(text::trim(insn.operands.substr(comma + 1)), 0)) base = *value;   // the first message number
                }
                if (insn.mnemonic == "jmp" && insn.operands.find("*4") != std::string::npos) {
                    const std::vector<Place>* table = nullptr;
                    for (std::int64_t k = at + 1; k < at + static_cast<std::int64_t>(insn.size); ++k) {
                        if (const le::InternalFixup* f = internal_at(entry.object, k)) {
                            if (const auto t = target_place(*f)) {
                                const auto found = r_.jump_tables.find(*t);
                                if (found != r_.jump_tables.end()) table = &found->second;
                            }
                            break;
                        }
                    }
                    if (table != nullptr) {
                        for (std::size_t index = 0; index < table->size(); ++index) {
                            const std::int64_t message = static_cast<std::int64_t>(index) + base;
                            const auto name = message >= 0 ? messages.find(static_cast<std::uint32_t>(message)) : messages.end();
                            if (name != messages.end() && std::count(table->begin(), table->end(), (*table)[index]) == 1) assign((*table)[index], device + "_" + name->second);   // a unique handler only
                        }
                    }
                    break;
                }
                at += insn.size;
            }
        }
        for (const auto& [offset, name] : names_.symbols(device)) assign(Place{1, offset}, name);   // where the offset lands on a function start
    }

    const le::Module& driver_;
    const DdkNames& names_;
    platforms::DecoderSet decoders_;
    Recovered r_;
    std::vector<std::vector<std::uint8_t>> bits_;   // by object, by byte: kIsCode | kIsData | kIsEntry
};

}  // namespace

// ---- vocabulary -----------------------------------------------------------------------------------------------------------------

std::optional<std::string_view> device_name(std::uint32_t id) {
    const auto it = std::lower_bound(kDevices.begin(), kDevices.end(), id, [](const DeviceName& d, std::uint32_t v) { return d.id < v; });
    if (it == kDevices.end() || it->id != id) return std::nullopt;
    return it->name;
}

// The number a direct branch writes as its operand ("0x401000"); nullopt for any other operand.
std::optional<std::int64_t> direct_operand(std::string_view operands) {
    const std::string_view t = text::trim(operands);
    if (!t.starts_with("0x")) return std::nullopt;
    return text::parse_int<std::int64_t>(t, 16);
}

std::optional<std::int64_t> Instruction::direct_target() const {
    if (!is_transfer(mnemonic)) return std::nullopt;
    return direct_operand(operands);
}

std::vector<std::pair<std::size_t, std::size_t>> string_ranges(ByteView code) {
    const auto printable = [](std::uint8_t b) { return b >= 0x20 && b <= 0x7E; };
    const auto letter = [](std::uint8_t b) { return (b >= 0x41 && b <= 0x5A) || (b >= 0x61 && b <= 0x7A); };
    const auto vowel = [](std::uint8_t b) { return b == 'A' || b == 'E' || b == 'I' || b == 'O' || b == 'U' || b == 'a' || b == 'e' || b == 'i' || b == 'o' || b == 'u'; };
    constexpr std::size_t kShort = 8, kLong = 20;   // a run is a string if it is long, or NUL-terminated and not too short
    constexpr double kMostlyLetters = 0.60;
    std::vector<std::pair<std::size_t, std::size_t>> out;
    const std::size_t n = code.size();
    for (std::size_t i = 0; i < n;) {
        if (!printable(code[i])) {
            ++i;
            continue;
        }
        std::size_t j = i;
        while (j < n && printable(code[j])) ++j;
        const std::size_t length = j - i;
        const ByteView run = code.subspan(i, length);
        const std::size_t letters = static_cast<std::size_t>(std::count_if(run.begin(), run.end(), letter));
        if (length >= kShort && ((j < n && code[j] == 0) || length >= kLong) && static_cast<double>(letters) >= kMostlyLetters * static_cast<double>(length) &&
            std::any_of(run.begin(), run.end(), vowel)) {
            out.emplace_back(i, j);
        }
        i = j;
    }
    return out;
}

DdkNames DdkNames::standard(const DdkFolders& ddk, const std::optional<std::filesystem::path>& design_dir) {
    return DdkNames(ServiceTables::standard(ddk, design_dir), vxd::control_messages(ddk), ddk);
}

OrderedMap<std::uint16_t, std::string> DdkNames::symbols(std::string_view device) const { return sym_names(folders_, device); }

// ---- the result -------------------------------------------------------------------------------------------------------------------

Disassembly Disassembly::run(const le::Module& driver, const DdkNames& names) {
    Recovered recovered = Engine(driver, names).run();
    return Disassembly(driver, std::move(recovered));
}

std::optional<le::Fixup> Disassembly::fixup_at(const Place& at) const {
    const le::Fixup* f = r_.fixups.find(at);
    return f ? std::optional<le::Fixup>(*f) : std::nullopt;
}

ByteView Disassembly::code(std::uint32_t object) const {
    return object >= 1 && object <= r_.code.size() ? ByteView(r_.code[object - 1]) : ByteView();
}

std::size_t Disassembly::code_bytes(std::uint32_t object) const {
    if (object < 1 || object > r_.covered.size()) return 0;
    const std::vector<bool>& bits = r_.covered[object - 1];
    return static_cast<std::size_t>(std::count(bits.begin(), bits.end(), true));
}

bool Disassembly::is_code(const Place& at) const {
    if (at.object < 1 || at.object > r_.covered.size() || at.offset < 0 || static_cast<std::uint64_t>(at.offset) >= r_.covered[at.object - 1].size()) return false;
    return r_.covered[at.object - 1][static_cast<std::size_t>(at.offset)];
}

}  // namespace cb::source::vxd
