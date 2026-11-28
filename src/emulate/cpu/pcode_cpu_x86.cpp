// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_cpu_x86.cpp -- an x86 engine on the one emulator, as the core16::Uc Unicorn's engine is (pcode_cpu_x86.hpp).
//
// The machine is Unicorn's as far as a lane can see: regions that are mapped, one flat memory behind them (the emulator's page map, which the description reads and writes by its
// own p-code, a hook on every access checking it against the regions), registers by Unicorn's numbers, an instruction count, the `until` address, hooks, faults as numbers.
// An instruction runs on the description (x86.sla for the 16-bit machine, x86-64.sla for the 64-bit one) one at a time, so that what comes before and after it is the adapter's
// to see. What the description leaves to the machine, or does not carry, is Unicorn's: an instruction that faults (an access outside the regions, a division the description
// fails on), one the description cannot decode, and the instructions that talk to the machine itself (HLT, INT, CPUID, the time stamp, the descriptor tables, the model registers,
// IN and OUT of the 64-bit machine) are put back as they were and the rest of the run goes to a Unicorn engine given the machine as it stands: the regions, every page that was
// written, the registers (the vector ones too), the hooks. From then on the engine is that one's, as one engine to the lane.
//
// What goes to Unicorn, with the reason (machine_instruction and wide_vector below are the lists; pcode_cpu_x86_test.cpp finds each by running the instruction on both):
//   - the emulator's values are at most 64 bits, so every instruction that works on a 128-bit value (the SSE moves, logic and arithmetic, 55 or so of the two-byte opcodes),
//     the x87 float (also the host's double, not 80 bits), MMX (which shares the x87 registers), and a 128-bit dividend, are Unicorn's;
//   - Unicorn is a 386-class machine without AVX: VEX and EVEX, undefined opcode extensions, opcodes invalid in 64-bit mode and doubled prefixes give its fault;
//   - the machine's own instructions: HLT, INT, IN and OUT of the 64-bit machine, CPUID, RDTSC and the system opcodes, far transfers, MOV to a segment register, BOUND;
//   - where Unicorn and the description choose differently: a division that faults, BSF and BSR of zero, a 16-bit branch in long mode, RET imm16's stack pointer;
//   - any user operation the emulator does not carry (SSE's saturating and averaging arithmetic, packs, x87's sines and BCD, ...) and any access outside the mapped regions.
// What the adapter puts right after the description ran: the zero-extension of a 32-bit register the description leaves whole (XCHG and XADD of memory, LODSD, CMPXCHG), the 16-bit
// machine's CALLF (the description does not load CS) and RET imm16, RETF imm16 and IRET (it leaves the segment out), and OUTS (its operands come the other way round).
// What stays unmatched: the flag AF, which the description never computes (it keeps the value it had; Unicorn computes it for every arithmetic instruction), and the flags the
// processor leaves undefined, which Unicorn and the description each choose (shifts' OF, multiplies' SF ZF PF, the BCD instructions'). For the 64-bit lanes that is no part of the
// lanes' comparison; the WonderSwan lane compares the flags, so the adapter works AF out itself (decode_af) and hands the instructions with undefined flags to Unicorn.
//
// Speed (the lanes run hundreds of short runs, most of which end on Unicorn): a lane's mem_write is kept as written and the emulator's pages are made from it when a run first
// looks at them (Emulator::setPageSource), where it was a page made and copied for every page of the image, and again to hand them to Unicorn; a Unicorn engine is handed what was
// written in one write each and only the pages the run itself wrote. The registers saved around an instruction are a few copies of bytes, not one read for each.
#include "emulate/cpu/pcode_cpu_x86.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <format>
#include <unordered_map>
#include <unordered_set>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/unicorn_api.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "emulate/unicorn_uc.hpp"
#include "emulate/cpu/pcode_cpu.hpp"
#include "emulate/cpu/pcode_cpu_internal.hpp"
#include "emulate/cpu/pcode_cpu_x86_cpuid.hpp"
#include "emulate/cpu/pcode_ops_x86.hpp"
#include "emulate/cpu/pcode_vector.hpp"

#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

namespace cb::emulate::pcode_cpu {

namespace sl = cb::decompile::sleigh;
namespace vec = cb::emulate::vec;
namespace uu = cb::emulate::unicorn_uc;

namespace {

using u32 = std::uint32_t;
using u64 = std::uint64_t;
using RegRef = sl::Emulator::RegRef;

constexpr u64 kPage = 0x1000;
constexpr int kErrMap = 11;
constexpr int kErrArg = 15;
constexpr int kErrReadUnmapped = 6;
constexpr int kErrFetchUnmapped = 8;
constexpr int kErrWriteUnmapped = 7;
constexpr int kErrInsnInvalid = 10;
constexpr int kErrException = 21;

std::filesystem::path languages() {
    if (const std::optional<std::string> env = cb::fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) return cb::fs::from_text(*env);
    return config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
}

[[noreturn]] void fail(int code, std::string_view what) { throw uu::UcFault(code, std::format("{}: {}", what, uu::describe_error(code))); }

thread_local X86Stats g_stats;

constexpr const char* kGpr64[16] = {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"};
constexpr const char* kGpr16[8] = {"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP"};
constexpr const char* kSeg[6] = {"CS", "DS", "ES", "SS", "FS", "GS"};

// One description, read once per thread and mode, with the registers the engine reads and writes found.
struct Machine {
    explicit Machine(int bits_)
        : bits(bits_),
          emu(languages() / (bits_ == 64 ? "x86-64.sla" : "x86.sla"),
              bits_ == 64 ? std::vector<std::pair<std::string, int>>{{"longMode", 1}, {"addrsize", 2}, {"bit64", 1}, {"opsize", 1}}
                          : std::vector<std::pair<std::string, int>>{{"addrsize", 0}, {"opsize", 0}, {"protectedMode", 0}}) {
        const char* const names[21] = {"CF", "F1", "PF", "F3", "AF", "F5", "ZF", "SF", "TF", "IF", "DF", "OF", "IOPL", "NT", "F15", "RF", "VM", "AC", "VIF", "VIP", "ID"};
        for (int i = 0; i < 21; ++i) flag[i] = emu.regRef(names[i]);
        for (int i = 0; i < 3; ++i) flag_words[i] = {flag[0].space, flag[0].offset + 8u * static_cast<unsigned>(i), 8};
        gprs = bits == 64 ? 16 : 8;
        for (int i = 0; i < gprs; ++i) gpr[i] = emu.regRef(bits == 64 ? kGpr64[i] : kGpr16[i]);
        for (int i = 0; i < 6; ++i) seg[i] = emu.regRef(kSeg[i]);
        if (bits == 64)
            for (int i = 0; i < 16; ++i) {
                const RegRef x = emu.regRef("YMM" + std::to_string(i));   // (the whole register: the XMM one is its low half)
                for (int k = 0; k < 4; ++k) xmm[4 * i + k] = {x.space, x.offset + 8u * static_cast<unsigned>(k), 8};
            }
        fs_base = emu.regRef("FS_OFFSET");
        gs_base = emu.regRef("GS_OFFSET");
        // where each register the snapshot keeps sits in it: a group of registers is one copy of the bytes from its first to its last
        unsigned next = 0;
        const auto group = [&](Group& g, const RegRef* refs, int n, Slot* slots) {
            std::uint64_t lo = ~std::uint64_t{0}, hi = 0;
            for (int i = 0; i < n; ++i) {
                lo = std::min<std::uint64_t>(lo, refs[i].offset);
                hi = std::max<std::uint64_t>(hi, refs[i].offset + refs[i].size);
            }
            g.at = {refs[0].space, lo, 0};
            g.len = static_cast<unsigned>(hi - lo);
            g.dst = next;
            next += (g.len + 7) & ~7u;
            if (next > kSnapshotBytes) throw UsageError("the p-code x86 engine's registers are spread too far apart to keep");
            for (int i = 0; i < n; ++i) slots[i] = {g.dst + static_cast<unsigned>(refs[i].offset - lo), refs[i].size};
        };
        ops_context = pcode_ops_x86::make_context(emu);
        ops_binding = pcode_ops_x86::bind(emu, ops_context);
        group(gpr_group, gpr, gprs, gpr_slot);
        group(flag_group, flag_words, 3, flag_slot);
        if (bits == 16) group(seg_group, seg, 6, seg_slot);
        if (bits == 64) group(xmm_group, xmm, 64, xmm_slot);
    }
    struct Group {
        RegRef at;
        unsigned len = 0, dst = 0;
    };
    struct Slot {
        unsigned dst = 0, size = 0;
    };
    static constexpr unsigned kSnapshotBytes = 4096;
    Group gpr_group, flag_group, seg_group, xmm_group;
    Slot gpr_slot[16], flag_slot[3], seg_slot[6], xmm_slot[64];
    int bits;
    sl::Emulator emu;
    RegRef flag[21];
    RegRef flag_words[3];
    RegRef gpr[16];
    RegRef seg[6];
    RegRef xmm[64];
    pcode_ops_x86::Context ops_context;
    pcode_ops::Binding ops_binding;
    RegRef fs_base, gs_base;
    int gprs = 0;
    bool busy = false;
};

// The bit of EFLAGS each of the description's flag registers is (IOPL is a field, bits 12 and 13).
constexpr unsigned kPhysicalBits = 52;
constexpr u64 kPhysicalMask = (u64{1} << kPhysicalBits) - 1;
constexpr unsigned kFlagBit[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18, 19, 20, 21};

Machine* acquire(int bits, std::unique_ptr<Machine>& own) {
    thread_local std::unique_ptr<Machine> shared[2];
    std::unique_ptr<Machine>& slot = shared[bits == 64 ? 1 : 0];
    if (!slot) slot = std::make_unique<Machine>(bits);
    if (!slot->busy) {
        slot->busy = true;
        return slot.get();
    }
    own = std::make_unique<Machine>(bits);   // (a second engine alive at once: its own copy of the description)
    own->busy = true;
    return own.get();
}

std::unordered_map<int, std::string_view> register_names() {
    std::unordered_map<int, std::string_view> names;
    for (const auto& [name, id] : uu::x86_registers()) names.emplace(id, name);
    return names;
}

// Unicorn's syscall hook, removed when the guard goes.
struct UnicornSyscallHook {
    core16::Uc* uc = nullptr;
    std::function<void(core16::Uc&)> hook;
    uc_engine* engine = nullptr;
    uc_hook handle = 0;
    ~UnicornSyscallHook() {
        if (handle != 0) emu::unicorn::api().uc_hook_del(engine, handle);
    }
};

void on_unicorn_syscall(uc_engine*, void* user) {
    UnicornSyscallHook& h = *static_cast<UnicornSyscallHook*>(user);
    h.hook(*h.uc);
}

std::shared_ptr<UnicornSyscallHook> add_unicorn_syscall_hook(core16::Uc& uc, std::function<void(core16::Uc&)> hook) {
    auto guard = std::make_shared<UnicornSyscallHook>();
    guard->uc = &uc;
    guard->hook = std::move(hook);
    guard->engine = static_cast<uc_engine*>(uc.handle());
    const emu::unicorn::Api& api = emu::unicorn::api();
    emu::unicorn::check(api.uc_hook_add(guard->engine, &guard->handle, UC_HOOK_INSN, reinterpret_cast<void*>(&on_unicorn_syscall), guard.get(), 1, 0, UC_X86_INS_SYSCALL),
                        "uc_hook_add(syscall)");
    return guard;
}

Bytes read_unicorn_xmm(core16::Uc& uc, unsigned index) {
    Bytes out(16);
    emu::unicorn::check(emu::unicorn::api().uc_reg_read(static_cast<uc_engine*>(uc.handle()), UC_X86_REG_XMM0 + static_cast<int>(index), out.data()), "uc_reg_read(xmm)");
    return out;
}
void write_unicorn_xmm(core16::Uc& uc, unsigned index, ByteView bytes) {
    std::uint8_t buffer[16] = {};
    std::memcpy(buffer, bytes.data(), std::min<std::size_t>(16, bytes.size()));
    emu::unicorn::check(emu::unicorn::api().uc_reg_write(static_cast<uc_engine*>(uc.handle()), UC_X86_REG_XMM0 + static_cast<int>(index), buffer), "uc_reg_write(xmm)");
}

class PcodeUc final : public core16::Uc {
public:
    explicit PcodeUc(int bits) : m_(acquire(bits, own_)), emu_(m_->emu), bits_(bits) {
        static const std::unordered_map<int, std::string_view> names = register_names();
        names_ = &names;
        // a fresh machine: every register zero but the flags' bit 1, and what the processor starts its floating point and vector units with
        emu_.clearMemory();
        const std::vector<std::uint8_t> zeros(0x2000, 0);
        emu_.write(0, zeros.data(), zeros.size(), "register");
        emu_.setPc(0);
        set_flag_bits(0x2);
        // (Unicorn's own start: the x87 control word and MXCSR are zero, not the processor's reset values)
        emu_.setPageSource([this](u64 address, std::uint8_t* out, std::size_t size) { return image_bytes(address, out, size); });
        emu_.onRead([this](std::string_view space, u64 address, unsigned size, u64& value) {
            if (!is_ram(space)) return false;
            const bool beyond = (address >> kPhysicalBits) != 0;   // (Unicorn's memory has 52 address bits: an address above them reads what its low 52 bits do)
            if (beyond) address &= kPhysicalMask;
            if (!mapped(address, size)) {
                if (fault_ == 0) fault_ = kErrReadUnmapped;
                value = 0;
                return true;
            }
            last_read_address_ = address;
            if (!read_seen_) {
                read_seen_ = true;
                first_read_address_ = address;
            }
            if (af_.kind != AfOp::Kind::None && !e_captured_) {
                e_captured_ = true;
                e_value_ = emu_.readValue(address, size);
            }
            if (beyond) value = emu_.readValue(address, size);
            return beyond;
        });
        emu_.onWrite([this](std::string_view space, u64 address, unsigned size, u64 value) {
            if (!is_ram(space)) return false;
            if (fault_ != 0) return true;   // (an instruction that faulted stores nothing more)
            const bool beyond = (address >> kPhysicalBits) != 0;
            if (beyond) address &= kPhysicalMask;
            if (!mapped(address, size)) {
                fault_ = kErrWriteUnmapped;
                return true;
            }
            touch(address, size);
            if (size <= sizeof(Stored::old)) {
                Stored& st = journal_[journal_size_ < kJournal ? journal_size_++ : kJournal - 1];
                st.address = address;
                st.size = size;
                emu_.read(address, st.old, size);
            }
            if (beyond) emu_.writeValue(address, size, value);
            return beyond;
        });
        emu_.onBadInstruction([this](sl::Emulator&, u64 pc) { return bad_instruction(pc); });
        emu_.onUserOp([this](sl::Emulator&, const std::string& name, const std::vector<u64>& args, u64& result) { return user_op(name, args, result); });
    }
    ~PcodeUc() override {
        syscall_guard_.reset();
        twin_.reset();
        emu_.onRead({});
        emu_.onWrite({});
        emu_.onUserOp({});
        emu_.onBadInstruction({});
        emu_.setPageSource({});
        emu_.clearMemory();
        m_->busy = false;
    }

    void* handle() const override { return twin_ ? twin_->handle() : nullptr; }

    // ---- memory
    void mem_map(u64 address, u64 size) override {
        if (twin_) return twin_->mem_map(address, size);
        if (size == 0 || (address & (kPage - 1)) != 0 || (size & (kPage - 1)) != 0) fail(kErrArg, "uc_mem_map");
        for (const Region& r : regions_)
            if (address < r.end && r.begin < address + size) fail(kErrMap, "uc_mem_map");
        regions_.push_back({address, address + size});
    }
    void mem_unmap(u64 address, u64 size) override {
        if (twin_) return twin_->mem_unmap(address, size);
        if (size == 0 || (address & (kPage - 1)) != 0 || (size & (kPage - 1)) != 0 || !mapped(address, size)) fail(kErrArg, "uc_mem_unmap");
        std::vector<Region> kept;
        for (const Region& r : regions_) {
            if (address >= r.end || r.begin >= address + size) {
                kept.push_back(r);
                continue;
            }
            if (r.begin < address) kept.push_back({r.begin, address});
            if (r.end > address + size) kept.push_back({address + size, r.end});
        }
        regions_ = std::move(kept);
        forget_image(address, address + size);
        const std::vector<std::uint8_t> zeros(static_cast<std::size_t>(std::min<u64>(size, 1u << 20)), 0);
        for (u64 at = 0; at < size; at += zeros.size()) emu_.writeHeld(address + at, zeros.data(), static_cast<std::size_t>(std::min<u64>(zeros.size(), size - at)));
        for (u64 page = address / kPage; page < (address + size) / kPage; ++page) touched_.erase(page);
        last_page_ = ~u64{0};
    }
    Bytes mem_read(u64 address, u64 size) override {
        if (twin_) return twin_->mem_read(address, size);
        if (!mapped(address, size)) fail(kErrReadUnmapped, "uc_mem_read");
        Bytes out(static_cast<std::size_t>(size));
        emu_.read(address, out.data(), out.size());
        return out;
    }
    void mem_write(u64 address, ByteView data) override {
        if (twin_) return twin_->mem_write(address, data);
        if (!mapped(address, data.size())) fail(kErrWriteUnmapped, "uc_mem_write");
        if (data.empty()) return;
        // (kept as written: the emulator's pages are made from it when a run first looks at them, and a Unicorn engine is handed it as it was written)
        image_.push_back({address, Bytes(data.begin(), data.end())});
        emu_.writeHeld(address, data.data(), data.size());   // (a page the emulator has made already is changed as well)
    }

    // ---- registers
    u64 reg_read(int reg) override {
        if (twin_) return twin_->reg_read(reg);
        const std::string_view name = name_of(reg);
        if (name == "EFLAGS" || name == "RFLAGS") return flag_bits();
        if (name == "RIP" || name == "EIP" || name == "IP") return ip_value();
        if (name == "FS_BASE") return emu_.reg("FS_OFFSET");
        if (name == "GS_BASE") return emu_.reg("GS_OFFSET");
        return emu_.reg(std::string(name));
    }
    double reg_read_float(int reg) override { return twin_ ? twin_->reg_read_float(reg) : static_cast<double>(reg_read(reg)); }   // (as the Unicorn engine: ST0 as its first eight bytes)
    void reg_write(int reg, u64 value) override {
        if (twin_) return twin_->reg_write(reg, value);
        const std::string_view name = name_of(reg);
        if (name == "EFLAGS" || name == "RFLAGS") {
            set_flag_bits(value);
        } else if (name == "RIP" || name == "EIP" || name == "IP") {
            set_ip(value);
        } else if (name == "FS_BASE") {
            emu_.setReg(m_->fs_base, value);
        } else if (name == "GS_BASE") {
            emu_.setReg(m_->gs_base, value);
        } else if (name == "CS" && bits_ == 16) {
            const u64 ip = (emu_.pc() - (emu_.reg(m_->seg[0]) << 4)) & 0xFFFF;   // (the instruction pointer stays: the segment moves under it)
            emu_.setReg(m_->seg[0], value);
            emu_.setPc((value << 4) + ip);
        } else if (bits_ == 64 && is_dword_register(name)) {
            // a 32-bit write in long mode zero-extends into the 64-bit register
            const std::string parent = name[0] == 'E' ? "R" + std::string(name.substr(1)) : std::string(name.substr(0, name.size() - 1));
            emu_.setReg(parent, value & 0xFFFFFFFFull);
        } else {
            emu_.setReg(std::string(name), value);
        }
    }
    void reg_write_descriptor(int reg, const Descriptor& value) override {
        if (twin_) return twin_->reg_write_descriptor(reg, value);
        throw UsageError("a descriptor register of the p-code x86 engine");
    }
    struct Saved final : Context {
        std::array<std::uint8_t, 0x2000> registers{};
        u64 pc = 0;
    };
    std::unique_ptr<Context> context_save() override {
        if (twin_) return twin_->context_save();
        auto saved = std::make_unique<Saved>();
        emu_.read(0, saved->registers.data(), saved->registers.size(), "register");
        saved->pc = emu_.pc();
        return saved;
    }
    void context_restore(const Context& context) override {
        if (twin_) return twin_->context_restore(context);
        const Saved& saved = static_cast<const Saved&>(context);
        emu_.write(0, saved.registers.data(), saved.registers.size(), "register");
        emu_.setPc(saved.pc);
    }
    void ctl_flush_tb() override {
        if (twin_) twin_->ctl_flush_tb();
    }

    // ---- running
    void emu_start(u64 begin, u64 until, u64 timeout_us, u64 count) override {
        if (twin_) return twin_->emu_start(begin, until, timeout_us, count);
        stop_ = false;
        fault_ = 0;
        if (bits_ == 16) {
            const u64 cs = emu_.reg(m_->seg[0]);
            emu_.setPc((cs << 4) + ((begin - (cs << 4)) & 0xFFFF));   // (a real-mode start: the instruction pointer is what is left of the linear address)
        } else {
            emu_.setPc(begin);
        }
        ++g_stats.runs;
        u64 executed = 0;
        Snapshot before;
        while (!stop_) {
            const u64 pc = emu_.pc();
            if (pc == until) break;
            if (!mapped(pc, 1)) fail(kErrFetchUnmapped, "uc_emu_start");   // (Unicorn finds the next instruction even when the count is spent)
            if (count != 0 && executed >= count) break;
            for (std::size_t i = 0; i < code_hooks_.size() && !stop_; ++i)
                if (pc >= code_hooks_[i].begin && pc <= code_hooks_[i].end) code_hooks_[i].callback(*this, pc, 1);
            if (stop_) break;
            if (bits_ == 16 && (emu_.reg(m_->seg[0]) & 0xFFF) != 0) return hand_off("code segment not on a 64 KB boundary", until, count, executed);   // (the description's near branches wrap in a 64 KB window)
            trail_[2] = trail_[1];
            trail_[1] = trail_[0];
            trail_[0] = pc;
            special_ = Special::None;
            if (const char* why = machine_instruction(pc)) return hand_off(why, until, count, executed);
            if (special_ != Special::None) {   // the machine's own instructions, which the adapter carries out as Unicorn does
                switch (special_) {
                    case Special::Invalid: ++g_stats.opcodes["refused " + bytes_text(pc)]; fail(kErrInsnInvalid, "uc_emu_start");
                    case Special::Halt:   // the run ends after the instruction
                        emu_.setPc(pc + special_len_);
                        ++g_stats.steps;
                        return;
                    case Special::Nop:
                        emu_.setPc(pc + special_len_);
                        if (bits_ == 16) wrap_ip();
                        ++executed;
                        ++g_stats.steps;
                        continue;
                    case Special::Exception: fail(kErrException, "uc_emu_start");
                    case Special::Cpuid: {   // the answers are Unicorn's CPU's (pcode_cpu_x86_cpuid.hpp)
                        u64 leaf = emu_.reg(m_->gpr[0]) & 0xFFFFFFFFull;
                        const u64 sub = emu_.reg(m_->gpr[2]) & 0xFFFFFFFFull;
                        if (leaf < 0x80000000ull ? leaf > 0xD : leaf > 0x80000008ull) leaf = 0xD;
                        u64 answer[4] = {0, 0, 0, 0};
                        for (const CpuidRow& row : kCpuidRows)
                            if (row.leaf == leaf && (row.sub < 0 || static_cast<u64>(row.sub) == sub)) {
                                answer[0] = row.eax, answer[1] = row.ebx, answer[2] = row.ecx, answer[3] = row.edx;
                                break;
                            }
                        for (int k = 0; k < 4; ++k) emu_.setReg(m_->gpr[k], answer[k]);   // (RAX, RBX, RCX, RDX: the 32-bit answers zero-extended)
                        emu_.setPc(pc + special_len_);
                        if (bits_ == 16) wrap_ip();
                        ++executed;
                        ++g_stats.steps;
                        continue;
                    }
                    case Special::Interrupt:   // no hook for it: an unhandled exception, the instruction pointer past the instruction
                        emu_.setPc(pc + special_len_);
                        ++g_stats.steps;
                        fail(kErrException, "uc_emu_start");
                    default: break;
                }
            }
            save(before);
            fault_ = 0;
            journal_size_ = 0;
            unserved_ = nullptr;
            e_captured_ = false;
            read_seen_ = false;
            bad_hand_off_ = nullptr;
            const u64 declined_before = declined_total();
            sl::Emulator::Status status = step_patched();
            if (declined_total() != declined_before && unserved_ == nullptr) {   // (an operation of the table that declined its operands: a shape it does not take)
                unserved_ = "an operation the emulator does not carry";
                ++g_stats.unserved["(declined) " + declined_name(before_names_)];
            }
            if (status == sl::Emulator::Status::BadInstruction && lock_at_ != 0) {   // (LOCK on an instruction that has no locked form: Unicorn runs it without)
                std::uint8_t lock = 0;
                emu_.read(lock_at_, &lock, 1);
                add_patch(lock_at_, 0xF0, 0x2E);
                lock_at_ = 0;
                fault_ = 0;
                status = step_patched();
            }
            if (status == sl::Emulator::Status::BadInstruction && addr32_at_ != 0 && bits_ == 64) {   // (an address-size prefix the description does not take with this instruction: run without it)
                const u64 around = emu_.readValue(pc, 8);
                unsigned chosen = 0x2E;
                for (const unsigned candidate : {0x2Eu, 0x3Eu, 0x26u, 0x36u}) {
                    bool used = prefix_present(around, 8, candidate);
                    for (unsigned k = 0; k < patch_count_; ++k) used = used || patch_[k].inert == candidate;
                    if (!used) {
                        chosen = candidate;
                        break;
                    }
                }
                add_patch(addr32_at_, 0x67, chosen);
                addr32_at_ = 0;
                fault_ = 0;
                status = step_patched();
            }
            if (status == sl::Emulator::Status::Ok && fault_ == 0 && unserved_ != nullptr) {
                restore(before);
                return hand_off(unserved_, until, count, executed);
            }
            if (status == sl::Emulator::Status::Ok && fault_ == 0 && scan_.on && source_is_zero(scan_, before)) {
                restore(before);
                return hand_off("bit scan of zero", until, count, executed);
            }
            if (status == sl::Emulator::Status::Ok && fault_ == 0 && division_.on && division_faults(before)) {
                restore(before);
                fail(kErrException, "uc_emu_start");   // (#DE: an unhandled exception, the state as it was before the instruction)
            }
            if (special_ == Special::InvalidAfterLoad) ++g_stats.opcodes["after-load " + bytes_text(pc) + (fault_ != 0 ? " faulted" : " ran")];
            if (status == sl::Emulator::Status::Ok && fault_ == 0 && special_ == Special::InvalidAfterLoad) {
                restore(before);
                fail(kErrInsnInvalid, "uc_emu_start");
            }
            if (status == sl::Emulator::Status::Ok && fault_ == 0) {
                if (m_->ops_context.invalid_instruction) {   // (an operation of the description reached an instruction the machine refuses)
                    m_->ops_context.invalid_instruction = false;
                    restore(before);
                    fail(kErrInsnInvalid, "uc_emu_start");
                }
                if (conv_.kind != Conversion::Kind::None) convert(before);
                if (fma_.on) fused_multiply_add(before);
                if (extract_memory_ && journal_size_ > 0 && journal_[journal_size_ - 1].size == 8) emu_.write(journal_[journal_size_ - 1].address + 4, journal_[journal_size_ - 1].old + 4, 4);
                if (af_.kind != AfOp::Kind::None) emu_.setReg(m_->flag[4], af_value(before));
                if (cqo_) emu_.setReg(m_->gpr[3], (emu_.reg(m_->gpr[0]) >> 63) != 0 ? ~u64{0} : 0);
                if (linear_ip_) emu_.setPc((emu_.reg(m_->seg[0]) << 4) + (emu_.pc() & 0xFFFF));
                if (far_call_) {   // (CALLF through the description pushes the return address and goes to the target, but does not load CS)
                    emu_.setReg(m_->seg[0], far_segment_);
                    emu_.setPc((static_cast<u64>(far_segment_) << 4) + far_offset_);
                }
                for (unsigned i = 0; zext_ != 0 && i < 16; ++i)
                    if ((zext_ >> i) & 1) emu_.setReg(m_->gpr[i], emu_.reg(m_->gpr[i]) & 0xFFFFFFFFull);
                ++executed;
                ++g_stats.steps;
                if (bits_ == 16) wrap_ip();
                continue;
            }
            restore(before);
            if (fault_ != 0) ++g_stats.opcodes["fault " + std::to_string(fault_) + " at " + bytes_text(pc) + " after " + bytes_text(trail_[0]) + " and " + bytes_text(trail_[1]) + " and " + bytes_text(trail_[2])];
            if (fault_ != 0) {   // (an access outside the regions: the state as it was before the instruction)
                if (fault_pc_after_ != 0) emu_.setPc(pc + fault_pc_after_);
                fail(fault_, "uc_emu_start");
            }
            if (division_.on && operand_value(division_, before) == 0) fail(kErrException, "uc_emu_start");   // (a zero divisor the description fails on)
            if (status == sl::Emulator::Status::BadInstruction) {
                ++g_stats.opcodes[opcode_key(pc)];
                if (bits_ == 64 && bad_hand_off_ == nullptr) fail(kErrInsnInvalid, "uc_emu_start");
                return hand_off(bad_hand_off_ != nullptr ? bad_hand_off_ : "no instruction in the description", until, count, executed);
            }
            return hand_off("the description failed", until, count, executed);
        }
    }
    void emu_stop() override {
        if (twin_) return twin_->emu_stop();
        stop_ = true;
    }

    // ---- hooks
    void hook_add_code(std::function<void(core16::Uc&, u64, u32)> callback, u64 begin, u64 end) override {
        if (twin_) return twin_->hook_add_code(std::move(callback), begin, end);
        code_hooks_.push_back({std::move(callback), begin, end});
    }
    void hook_add_insn_in(std::function<u32(core16::Uc&, u32, u32)> callback) override {
        if (twin_) return twin_->hook_add_insn_in(std::move(callback));
        in_ = std::move(callback);
    }
    void hook_add_insn_out(std::function<void(core16::Uc&, u32, u32, u32)> callback) override {
        if (twin_) return twin_->hook_add_insn_out(std::move(callback));
        out_ = std::move(callback);
    }
    void set_syscall(std::function<void(core16::Uc&)> hook) {
        syscall_ = std::move(hook);
        if (twin_) syscall_guard_ = syscall_ ? add_unicorn_syscall_hook(*twin_, syscall_) : nullptr;
    }
    bool on_twin() const { return twin_ != nullptr; }
    // A vector register (the 128 bits of XMM0..15), as the machine has it now.
    Bytes read_xmm(unsigned index) {
        if (twin_) return read_unicorn_xmm(*twin_, index);
        Bytes out(16);
        emu_.read(emu_.regRef("XMM" + std::to_string(index)).offset, out.data(), 16, "register");
        return out;
    }
    Bytes read_ymm(unsigned index) {
        if (twin_) {
            Bytes out = read_unicorn_xmm(*twin_, index);
            out.resize(32, 0);
            return out;
        }
        Bytes out(32);
        sl::Emulator::RegRef r = m_->xmm[4 * index];
        r.size = 32;
        emu_.readRegWide(r, out.data());
        return out;
    }
    void write_ymm(unsigned index, ByteView bytes) {
        if (twin_) return write_unicorn_xmm(*twin_, index, bytes);
        std::uint8_t buffer[32] = {};
        std::memcpy(buffer, bytes.data(), std::min<std::size_t>(32, bytes.size()));
        sl::Emulator::RegRef r = m_->xmm[4 * index];
        r.size = 32;
        emu_.writeRegWide(r, buffer);
    }
    void write_xmm(unsigned index, ByteView bytes) {
        if (twin_) return write_unicorn_xmm(*twin_, index, bytes);
        emu_.write(emu_.regRef("XMM" + std::to_string(index)).offset, bytes.data(), 16, "register");
    }

private:
    struct Region {
        u64 begin, end;
    };
    struct CodeHook {
        std::function<void(core16::Uc&, u64, u32)> callback;
        u64 begin, end;
    };
    struct Division {
        bool on = false;
        unsigned bytes = 0;
        bool is_signed = false;
        unsigned mod = 0, rm = 0;
        bool rex_b = false, has_rex = false;
    };
    // The registers an instruction can change, kept to put back before an instruction goes to Unicorn: the groups' bytes, copied whole (Machine::group).
    struct Snapshot {
        const Machine* m = nullptr;
        alignas(8) std::uint8_t raw[Machine::kSnapshotBytes];
        u64 pc = 0;
        u64 word(const Machine::Slot& s) const {
            u64 v = 0;
            std::memcpy(&v, raw + s.dst, s.size);   // (the description's registers are little-endian, as the host)
            return v;
        }
        u64 gpr(int i) const { return word(m->gpr_slot[i]); }
        u64 flags(int i) const { return word(m->flag_slot[i]); }   // the flag registers' bytes, eight at a time
        u64 seg(int i) const { return word(m->seg_slot[i]); }
    };
    void save(Snapshot& s) const {
        s.m = m_;
        emu_.readRegs(m_->gpr_group.at, s.raw + m_->gpr_group.dst, m_->gpr_group.len);
        emu_.readRegs(m_->flag_group.at, s.raw + m_->flag_group.dst, m_->flag_group.len);
        if (bits_ == 16) emu_.readRegs(m_->seg_group.at, s.raw + m_->seg_group.dst, m_->seg_group.len);
        if (vector_ && bits_ == 64) emu_.readRegs(m_->xmm_group.at, s.raw + m_->xmm_group.dst, m_->xmm_group.len);
        s.pc = emu_.pc();
    }
    void restore(const Snapshot& s) {
        for (std::size_t i = journal_size_; i-- > 0;) emu_.write(journal_[i].address, journal_[i].old, journal_[i].size);
        journal_size_ = 0;
        emu_.writeRegs(m_->gpr_group.at, s.raw + m_->gpr_group.dst, m_->gpr_group.len);
        emu_.writeRegs(m_->flag_group.at, s.raw + m_->flag_group.dst, m_->flag_group.len);
        if (bits_ == 16) emu_.writeRegs(m_->seg_group.at, s.raw + m_->seg_group.dst, m_->seg_group.len);
        if (vector_ && bits_ == 64) emu_.writeRegs(m_->xmm_group.at, s.raw + m_->xmm_group.dst, m_->xmm_group.len);
        emu_.setPc(s.pc);
    }

    std::string_view name_of(int reg) const {
        const auto it = names_->find(reg);
        if (it == names_->end()) throw UsageError(std::format("x86 register {} in the p-code engine", reg));
        return it->second;
    }
    static bool is_dword_register(std::string_view name) {
        if (name.size() == 3 && name[0] == 'E') return name != "EIP" && name != "ES";   // EAX .. EDI
        return name.size() >= 3 && name[0] == 'R' && name.back() == 'D';                 // R8D .. R15D
    }

    bool mapped(u64 address, u64 size) const {
        if (size == 0) return true;
        u64 at = address;
        const u64 end = address + size;
        if (end < address) return false;
        while (at < end) {
            bool found = false;
            for (const Region& r : regions_)
                if (at >= r.begin && at < r.end) {
                    at = r.end;
                    found = true;
                    break;
                }
            if (!found) return false;
        }
        return true;
    }
    // The image the lane wrote with mem_write, as the emulator's pages are made from it: [address, address + size) of it (zeros where nothing was written), and whether any byte was.
    bool image_bytes(u64 address, std::uint8_t* out, std::size_t size) const {
        if (out != nullptr) std::memset(out, 0, size);
        bool any = false;
        const u64 end = address + size;
        for (const Image& w : image_) {
            const u64 w_end = w.address + w.data.size();
            if (w.address >= end || w_end <= address) continue;
            if (out == nullptr) return true;
            any = true;
            const u64 from = std::max(w.address, address), to = std::min(w_end, end);
            std::memcpy(out + (from - address), w.data.data() + (from - w.address), static_cast<std::size_t>(to - from));
        }
        return any;
    }
    // [begin, end) is no longer mapped: what was written there is forgotten.
    void forget_image(u64 begin, u64 end) {
        std::vector<Image> kept;
        for (Image& w : image_) {
            const u64 w_end = w.address + w.data.size();
            if (w_end <= begin || w.address >= end) {
                kept.push_back(std::move(w));
                continue;
            }
            if (w.address < begin) kept.push_back({w.address, Bytes(w.data.begin(), w.data.begin() + static_cast<std::ptrdiff_t>(begin - w.address))});
            if (w_end > end) kept.push_back({end, Bytes(w.data.begin() + static_cast<std::ptrdiff_t>(end - w.address), w.data.end())});
        }
        image_ = std::move(kept);
    }
    void touch(u64 address, u64 size) {
        for (u64 page = address / kPage; page <= (address + size - 1) / kPage; ++page) {
            if (page == last_page_) continue;
            last_page_ = page;
            touched_.insert(page);
        }
    }

    u64 flag_bits() const {
        u64 bits = 0;
        for (int i = 0; i < 21; ++i) {
            const u64 v = emu_.reg(m_->flag[i]);
            bits |= i == 12 ? (v & 3) << 12 : (v & 1) << kFlagBit[i];
        }
        return bits | 0x2;
    }
    void set_flag_bits(u64 bits) {
        for (int i = 0; i < 21; ++i) emu_.setReg(m_->flag[i], i == 12 ? (bits >> 12) & 3 : (bits >> kFlagBit[i]) & 1);
        emu_.setReg(m_->flag[1], 1);
    }

    u64 ip_value() const { return bits_ == 64 ? emu_.pc() : (emu_.pc() - (emu_.reg(m_->seg[0]) << 4)) & 0xFFFF; }
    void set_ip(u64 value) {
        if (bits_ == 64) emu_.setPc(value);
        else emu_.setPc((emu_.reg(m_->seg[0]) << 4) + (value & 0xFFFF));
    }
    // The instruction pointer of the 16-bit machine stays in its segment: past the end it comes round.
    void wrap_ip() {
        const u64 base = emu_.reg(m_->seg[0]) << 4;
        const u64 ip = emu_.pc() - base;
        if (ip > 0xFFFF) emu_.setPc(base + (ip & 0xFFFF));
    }

    // ---- what the description leaves to the machine
    // The instruction at `pc` (after its prefixes): which of those the machine does is Unicorn's. Null: the description's.
    const char* machine_instruction(u64 pc, u64 given = 0, bool again = false) {
        u64 bytes = again ? given : emu_.readValue(pc, 8);
        unsigned i = 0;
        unsigned rex = 0;
        for (; i < 7; ++i) {
            const unsigned b = static_cast<unsigned>(bytes >> (8 * i)) & 0xFF;
            const bool legacy = b == 0x66 || b == 0x67 || b == 0xF2 || b == 0xF3 || b == 0xF0 || b == 0x26 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x64 || b == 0x65;
            if (bits_ == 64 && b >= 0x40 && b <= 0x4F) rex = b;
            else if (!legacy) break;
            else rex = 0;   // (a REX not last in front of the opcode counts for nothing)
        }
        const bool wide = bits_ == 64;
        if (!again) patch_count_ = 0;
        if (wide)   // (a REX that is followed by another prefix counts for nothing on Unicorn, and the description does not decode it: the byte is run as a prefix that does nothing)
            for (unsigned k = 0; k + 1 < i; ++k) {
                const unsigned b = static_cast<unsigned>(bytes >> (8 * k)) & 0xFF;
                if (b < 0x40 || b > 0x4F) continue;
                static constexpr std::uint8_t kInert[4] = {0x2E, 0x3E, 0x26, 0x36};   // (the segment overrides the 64-bit machine ignores)
                unsigned chosen = 0;
                for (const std::uint8_t candidate : kInert)
                    if (!prefix_present(bytes, i, candidate)) {
                        chosen = candidate;
                        break;
                    }
                if (chosen == 0) return "REX prefixes among four overrides";
                add_patch(pc + k, b, chosen);
                bytes = (bytes & ~(u64{0xFF} << (8 * k))) | (static_cast<u64>(chosen) << (8 * k));
            }
        const unsigned op = static_cast<unsigned>(bytes >> (8 * i)) & 0xFF;
        const unsigned next = static_cast<unsigned>(bytes >> (8 * (i + 1))) & 0xFF;
        const unsigned reg = (next >> 3) & 7;
        division_.on = false;
        scan_.on = false;
        af_ = AfOp{};
        if (!wide) decode_af(bytes, i);
        far_call_ = false;
        linear_ip_ = false;
        cqo_ = false;
        conv_.kind = Conversion::Kind::None;
        fma_.on = false;
        extract_memory_ = false;
        fault_pc_after_ = 0;
        vector_ = op == 0x0F || op == 0xC4 || op == 0xC5;
        zext_ = 0;
        const bool override16 = operand_16_override(bytes, i);
        if (!again) {
            lock_at_ = 0;
            addr32_at_ = 0;
        }
        for (unsigned k = 0; k < i; ++k)
            if (((bytes >> (8 * k)) & 0xFF) == 0x67) addr32_at_ = pc + k;
        for (unsigned k = 0; k < i; ++k)
            if (((bytes >> (8 * k)) & 0xFF) == 0xF0) lock_at_ = pc + k;
        for (unsigned a = 0; a < i; ++a)   // (a prefix said twice: the description counts each, the processor does not: the second is run as a prefix that does nothing)
            for (unsigned b = a + 1; b < i; ++b) {
                const unsigned x = static_cast<unsigned>(bytes >> (8 * a)) & 0xFF;
                if (x != ((bytes >> (8 * b)) & 0xFF) || (wide && (x & 0xF0) == 0x40)) continue;
                static constexpr std::uint8_t kInert[4] = {0x2E, 0x3E, 0x26, 0x36};
                unsigned chosen = 0;
                for (const std::uint8_t candidate : kInert)
                    if (!prefix_present(bytes, i, candidate)) {
                        chosen = candidate;
                        break;
                    }
                if (chosen == 0) return "a prefix repeated";
                add_patch(pc + b, x, chosen);
                bytes = (bytes & ~(u64{0xFF} << (8 * b))) | (static_cast<u64>(chosen) << (8 * b));
                if (x == 0xF0) lock_at_ = 0;
            }
        if (wide && lock_at_ != 0 && lock_refused(op, next, op == 0x0F ? static_cast<unsigned>(bytes >> (8 * (i + 2))) & 0xFF : next)) {
            // (Unicorn loads the memory operand first: an access outside the regions is the fault then; the instruction runs without the prefix and is refused after it)
            const bool has_modrm = (op < 0x40 && (op & 7) < 4) || op == 0x80 || op == 0x81 || op == 0x83 || op == 0xFE || op == 0xFF;
            const bool string_op = op == 0xA6 || op == 0xA7 || op == 0xAE || op == 0xAF;
            unsigned chosen = 0;
            for (const unsigned candidate : {0x2Eu, 0x3Eu, 0x26u, 0x36u})
                if (!prefix_present(bytes, i, candidate)) {
                    chosen = candidate;
                    break;
                }
            if (chosen != 0 && ((has_modrm && (next >> 6) != 3) || string_op)) {
                add_patch(lock_at_, 0xF0, chosen);
                lock_at_ = 0;
                special_ = Special::InvalidAfterLoad;
                return nullptr;
            }
            return invalid_insn();
        }
        if (wide && override16) {   // (Intel's 16-bit branch in long mode cuts the instruction pointer to 16 bits: the description does not)
            if ((op >= 0x70 && op <= 0x7F) || (op >= 0xE0 && op <= 0xE3) || op == 0xE8 || op == 0xE9 || op == 0xEB || op == 0xC2 || op == 0xC3 || (op == 0x0F && next >= 0x80 && next <= 0x8F) ||
                (op == 0xFF && (reg == 2 || reg == 4)))
                return "branch with an operand-size prefix";
        }
        switch (op) {
            case 0xF4: special_ = Special::Halt; special_len_ = i + 1; return nullptr;
            case 0xCC: special_ = Special::Interrupt; special_len_ = i + 1; return nullptr;
            case 0xCD:
                if (next == 6) return invalid_insn();   // (vector 6 is the invalid opcode exception)
                special_ = Special::Interrupt;
                special_len_ = i + 2;
                return nullptr;
            case 0xCE:
                if (wide) return invalid_insn();
                return "INT";
            case 0x62: return wide ? invalid_insn() : "BOUND";   // (EVEX: the processors of these lanes have no AVX-512, and Unicorn's has not either)
            case 0x9D:   // POPF and IRET: a trap flag in what they load makes Unicorn raise a single-step exception after the next instruction, which the description has no flag for
            case 0xCF:
                if (!wide && !override16) {
                    const u64 sp = emu_.reg(m_->gpr[7]) & 0xFFFF;
                    const u64 word = emu_.readValue((emu_.reg(m_->seg[3]) << 4) + ((sp + (op == 0xCF ? 4 : 0)) & 0xFFFF), 2);
                    if ((word & 0x3100) != 0) return "flags with the trap flag or IOPL";   // (Unicorn keeps both and acts on them: the description has no TF, and no IOPL in what PUSHF pushes)
                    if (op == 0xCF) linear_ip_ = true;
                }
                if (op == 0xCF && wide) return "IRET";
                if (op == 0x9D && wide) {   // POPF in the 64-bit machine: the same
                    const u64 flags = emu_.readValue(emu_.reg(m_->gpr[7]), override16 ? 2 : 8);
                    if ((flags & 0x3100) != 0) return "flags with the trap flag or IOPL";
                }
                return nullptr;
            case 0x37: case 0x3F: return wide ? invalid_insn() : "AAA and AAS (AX + 0x106)";
            case 0x27: case 0x2F: return wide ? invalid_insn() : "DAA and DAS (OF)";   // (OF after them is whatever the machine chooses)
            case 0x69: case 0x6B:
                return wide ? nullptr : "IMUL by an immediate (SF, ZF, PF are whatever the machine chooses)";
            case 0xD1:
            case 0xC1: case 0xD3:
                return op == 0xD1 || wide ? nullptr : "shift or rotate by a count (OF is whatever the machine chooses)";
            case 0xC0: case 0xD2: return wide ? nullptr : "shift or rotate by a count (OF is whatever the machine chooses)";   // (the description adds 1 to AH, not the carry out of AL too)
            case 0xC4: case 0xC5:
                if (wide) {
                    ++g_stats.vex;
                    if (!x86_options().vex_as_unicorn) {
                        {   // VCVTTPS2DQ: the integer indefinite for a lane out of range, as the legacy one (the description's operation saturates)
                            const unsigned vb2 = static_cast<unsigned>(bytes >> (8 * (i + 2))) & 0xFF, vb3 = static_cast<unsigned>(bytes >> (8 * (i + 3))) & 0xFF, vb4 = static_cast<unsigned>(bytes >> (8 * (i + 4))) & 0xFF;
                            const bool three = op == 0xC4;
                            const unsigned map_bits = three ? next & 0x1F : 1, pp_bits = three ? vb2 & 3 : next & 3, opcode_byte = three ? vb3 : vb2, modrm_byte = three ? vb4 : vb3;
                            const bool long_form = ((three ? vb2 : next) & 4) != 0, r_ext = (next & 0x80) == 0, b_ext = three && (next & 0x20) == 0;
                            if (map_bits == 1 && opcode_byte == 0x5B && pp_bits == 2) {
                                conv_ = {Conversion::Kind::PsToDq, true, false, (modrm_byte >> 6) == 3, modrm_byte & 7, (modrm_byte >> 3) & 7, b_ext, r_ext, false, long_form ? 32u : 16u};
                            }
                            if (map_bits == 1 && opcode_byte == 0xE6 && pp_bits == 1) {   // VCVTTPD2DQ
                                conv_ = {Conversion::Kind::PdToDq, true, true, (modrm_byte >> 6) == 3, modrm_byte & 7, (modrm_byte >> 3) & 7, b_ext, r_ext, false, long_form ? 32u : 16u};
                            }
                        }
                        const unsigned b1 = next, b2n = static_cast<unsigned>(bytes >> (8 * (i + 2))) & 0xFF, opc = static_cast<unsigned>(bytes >> (8 * (i + 3))) & 0xFF, modrm = static_cast<unsigned>(bytes >> (8 * (i + 4))) & 0xFF;
                        // FMA: the description multiplies and adds (two roundings); the processor fuses them. The adapter works the result out again from the registers as they were.
                        if (op == 0xC4 && (b1 & 0x1F) == 2 && (opc & 0x0F) >= 6 && (opc >> 4) >= 9 && (opc >> 4) <= 0xB) {
                            fma_ = {true, opc, b2n >> 7, (b2n >> 2) & 1, (~(b2n >> 3)) & 15u, ((modrm >> 3) & 7) + ((b1 & 0x80) == 0 ? 8u : 0u), (modrm >> 6) == 3, (modrm & 7) + ((b1 & 0x20) == 0 ? 8u : 0u)};
                        }
                        return nullptr;
                    }
                    // Unicorn takes the VEX prefix of an instruction of the 0F map as no prefix at all (the vvvv and mmmmm bits count for nothing) and refuses one with the length bit
                    // set (256 bits); the instructions of the other maps are, but for BMI's (opcodes F2 F3 F5 F6 F7 of the 0F 38 map, F0 of 0F 3A, the processor's own), a thing
                    // the adapter hands to it
                    const unsigned b2 = static_cast<unsigned>(bytes >> (8 * (i + 2))) & 0xFF, b3 = static_cast<unsigned>(bytes >> (8 * (i + 3))) & 0xFF;
                    const unsigned map = op == 0xC5 ? 1 : next & 0x1F;
                    const unsigned opcode = op == 0xC5 ? b2 : b3;
                    const bool length256 = ((op == 0xC5 ? next : b2) & 4) != 0;
                    for (unsigned k = 0; k < i; ++k) {   // (a VEX prefix after a REX, a LOCK, a 66, F2 or F3 is refused; one after a segment override or 67 is not)
                        const unsigned before_byte = static_cast<unsigned>(bytes >> (8 * k)) & 0xFF;
                        if (before_byte == 0x66 || before_byte == 0xF2 || before_byte == 0xF3 || before_byte == 0xF0 || (before_byte >= 0x40 && before_byte <= 0x4F)) return invalid_insn();
                    }
                    if (map == 2 && (opcode == 0xF2 || opcode == 0xF3 || opcode == 0xF5 || opcode == 0xF6 || opcode == 0xF7)) return nullptr;
                    if (map == 3 && opcode == 0xF0) return nullptr;
                    if (map != 1) return "VEX that is not a legacy instruction";
                    const bool sse_opcode = (opcode >= 0x10 && opcode <= 0x17) || (opcode >= 0x28 && opcode <= 0x2F) || (opcode >= 0x50 && opcode <= 0x7F) || opcode == 0xC2 || (opcode >= 0xC4 && opcode <= 0xC6) || (opcode >= 0xD0 && opcode <= 0xFE);
                    if (length256 && sse_opcode) return invalid_insn();
                    // another opcode of the 0F map: the legacy instruction (the prefix bytes become a mandatory prefix that does nothing, and a REX byte)
                    const unsigned pp = op == 0xC5 ? next & 3 : b2 & 3;
                    const bool r_ext = (next & 0x80) == 0, x_ext = op == 0xC4 && (next & 0x40) == 0, b_ext = op == 0xC4 && (next & 0x20) == 0, w = op == 0xC4 && (b2 & 0x80) != 0;
                    const unsigned rex_byte = 0x40 | (w ? 8 : 0) | (r_ext ? 4 : 0) | (x_ext ? 2 : 0) | (b_ext ? 1 : 0);   // (the register bits of a VEX prefix are inverted)
                    const unsigned prefix = pp == 1 ? 0x66 : pp == 2 ? 0xF3 : pp == 3 ? 0xF2 : 0;
                    if (op == 0xC5) {   // (two bytes: a mandatory prefix or a REX byte, then 0F)
                        if (rex_byte != 0x40 && prefix != 0) return "VEX that is not a legacy instruction";
                        add_patch(pc + i, 0xC5, rex_byte != 0x40 ? rex_byte : prefix != 0 ? prefix : 0x2E);
                        add_patch(pc + i + 1, next, 0x0F);
                    } else {   // (three: the prefix, the REX byte, 0F)
                        add_patch(pc + i, 0xC4, prefix != 0 ? prefix : 0x2E);
                        add_patch(pc + i + 1, next, rex_byte);
                        add_patch(pc + i + 2, b2, 0x0F);
                    }
                    for (unsigned k = 0; k < patch_count_; ++k)
                        if (patch_[k].address >= pc && patch_[k].address < pc + 8) bytes = (bytes & ~(u64{0xFF} << (8 * (patch_[k].address - pc)))) | (static_cast<u64>(patch_[k].inert) << (8 * (patch_[k].address - pc)));
                    return machine_instruction(pc, bytes, true);
                }
                return (next >> 6) == 3 ? "LES or LDS of a register (the description reads a VEX prefix)" : nullptr;
            case 0x63: return wide ? nullptr : "ARPL (invalid in real mode)";
            case 0x8E: return wide ? "MOV to a segment register" : reg == 1 ? "MOV to CS" : nullptr;
            case 0x9A:
                if (wide) return invalid_insn();
                if (override16) return "far call with an operand-size prefix";
                far_call_ = true;   // CALLF ptr16:16: the description leaves CS as it is
                far_segment_ = static_cast<unsigned>(bytes >> (8 * (i + 3))) & 0xFFFF;
                far_offset_ = static_cast<unsigned>(bytes >> (8 * (i + 1))) & 0xFFFF;
                return nullptr;
            case 0x06: case 0x07: case 0x0E: case 0x16: case 0x17: case 0x1E: case 0x1F: case 0x60: case 0x61: case 0x82:
            case 0xD5: case 0xD6: case 0xEA: return wide ? invalid_insn() : op == 0xD5 ? "AAM and AAD (the flags the machine chooses)" : nullptr;
            case 0xD4: return wide ? invalid_insn() : next == 0 ? "division fault" : "AAM and AAD (the flags the machine chooses)";
            case 0xF1: return invalid_insn();
            case 0x99: return nullptr;
            case 0xC8: return override16 || ((bytes >> (8 * (i + 3))) & 0xFF) != 0 ? "ENTER with a 16-bit frame or nesting" : nullptr;
            case 0x9B: special_ = Special::Nop; special_len_ = i + 1; return nullptr;   // (no exception is ever pending: FWAIT does nothing)
            case 0xC2:
                if (wide) return "RET imm16 (Unicorn's RSP)";
                if (override16) return "RET with an operand-size prefix";
                linear_ip_ = true;   // (the description returns to the offset, without the segment's base)
                return nullptr;
            case 0xCA: case 0xCB:
                if (wide) return "far return";
                if (override16) return "far return with an operand-size prefix";
                linear_ip_ = op != 0xCB;   // (RETF forms its own linear address)
                return nullptr;
            case 0xD7: return wide ? "XLAT" : nullptr;
            case 0x9E: case 0x9F: return wide ? "SAHF/LAHF" : nullptr;
            case 0x90: return wide && rex != 0 && (rex & 1) == 0 ? "NOP with REX" : nullptr;
            case 0x87:
                if (wide && (next >> 6) != 3 && (rex & 8) == 0 && !override16) zext_ = 1u << hardware_to_index(((next >> 3) & 7) + ((rex & 4) != 0 ? 8 : 0));
                return nullptr;
            case 0xE0: case 0xE1: case 0xE2: case 0xE3: return wide && prefix_present(bytes, i, 0x67) ? "loop with a 32-bit counter" : nullptr;
            case 0xA4: case 0xA5: case 0xA6: case 0xA7: case 0xAC: case 0xAD:
                if (wide && (prefix_present(bytes, i, 0x64) || prefix_present(bytes, i, 0x65))) return "string instruction with a segment override";
                if (op == 0xAD && wide && (rex & 8) == 0 && !override16) {
                    if (prefix_present(bytes, i, 0xF3) || prefix_present(bytes, i, 0xF2)) return "REP LODSD";
                    zext_ = 1;
                }
                return nullptr;
            case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD: case 0xDE: case 0xDF: return x86_options().x87_on_description ? nullptr : "x87";   // (the description's float is the host's double: 80-bit precision and the status word are Unicorn's)
            case 0xE4: case 0xE5: case 0xE6: case 0xE7: case 0xEC: case 0xED: case 0xEE: case 0xEF: case 0x6C: case 0x6D: case 0x6E: case 0x6F:
                if (wide && (((op == 0xE4 || op == 0xE5 || op == 0xEC || op == 0xED) && !in_) || ((op == 0xE6 || op == 0xE7 || op == 0xEE || op == 0xEF) && !out_))) return "IN or OUT with no hook";   // (Unicorn's own answer)
                if (wide && (op == 0x6C || op == 0x6D || op == 0x6E || op == 0x6F)) return "INS and OUTS";   // (the string forms: the order of the port and the value differs from Unicorn's)
                if (wide && (op == 0xE5 || op == 0xED) && !override16) zext_ |= 1;   // (IN EAX: the description leaves the upper half of RAX)
                return nullptr;
            case 0x8F: if (reg != 0) add_patch(pc + i + 1, next, next & ~0x38u); return nullptr;   // (Unicorn runs it as POP whatever the extension)
            case 0xC6: case 0xC7:
                if (reg == 7 && (next >> 6) == 3) add_patch(pc + i + 1, next, next & ~0x38u);   // (and /7 of a register, XABORT and XBEGIN to the description, as a MOV)
                return reg != 0 && (reg != 7 || (next >> 6) != 3) ? invalid_insn() : nullptr;
            case 0xFE: return reg > 1 ? invalid_insn() : nullptr;
            case 0xFF:
                if (reg == 7) {   // (Unicorn loads the operand before it finds the extension undefined: a fault of the load comes first)
                    if ((next >> 6) == 3) return invalid_insn();
                    add_patch(pc + i + 1, next, (next & ~0x38u) | 0x20);   // (as JMP r/m)
                    special_ = Special::InvalidAfterLoad;
                    return nullptr;
                }
                return reg == 3 || reg == 5 ? "far transfer" : nullptr;
            case 0xF6: case 0xF7:
                if (reg == 1) {
                    if ((next >> 6) == 3) return invalid_insn();
                    add_patch(pc + i + 1, next, next & ~0x38u);   // (as TEST)
                    special_ = Special::InvalidAfterLoad;
                    return nullptr;
                }
                if (!wide && (reg == 4 || reg == 5)) return "MUL and IMUL (SF, ZF, PF, AF are whatever the machine chooses)";
                if (reg >= 6) {
                    const unsigned size = op == 0xF6 ? 1u : bits_ == 16 ? (override16 ? 4u : 2u) : (rex & 8) != 0 ? 8u : override16 ? 2u : 4u;
                    division_ = {true, size, reg == 7, next >> 6, next & 7, (rex & 1) != 0, rex != 0};
                }
                return nullptr;
            case 0x0F: {
                const unsigned third = static_cast<unsigned>(bytes >> (8 * (i + 2))) & 0xFF;
                const unsigned mandatory = prefix_present(bytes, i, 0xF3) ? 2 : prefix_present(bytes, i, 0xF2) ? 3 : override16 ? 1 : 0;   // none, 66, F3, F2
                switch (next) {
                    case 0x0B: case 0xFF: case 0xB9: case 0xAA: return invalid_insn();   // (UD2, the undefined opcodes, UD1 and RSM)
                    case 0x00: case 0x01: case 0x02: case 0x03: case 0x06: case 0x07: case 0x08: case 0x09:
                    case 0x20: case 0x21: case 0x22: case 0x23: case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x37:
                    case 0x78: case 0x79: case 0x0E: return "system instruction";
                    case 0xA2: special_ = Special::Cpuid; special_len_ = i + 2; return nullptr;
                    case 0x05:
                        if (!syscall_) return "SYSCALL";
                        syscall_rcx_ = emu_.reg(m_->gpr[2]);   // (the description loads RCX and R11 before the operation; Unicorn's hook runs instead of the instruction and leaves both)
                        syscall_r11_ = emu_.reg(m_->gpr[11]);
                        return nullptr;
                    case 0xB1:   // CMPXCHG: the 32-bit registers it writes are zero-extended
                        if (wide && (rex & 8) == 0 && !override16) zext_ = 1u | ((third & 0xC0) == 0xC0 ? 1u << hardware_to_index((third & 7) + ((rex & 1) != 0 ? 8 : 0)) : 0u);
                        return nullptr;
                    case 0xC1:   // XADD: the 32-bit register of the memory form gets the old value, zero-extended
                        if (wide && (third & 0xC0) != 0xC0 && (rex & 8) == 0 && !override16) zext_ = 1u << hardware_to_index(((third >> 3) & 7) + ((rex & 4) != 0 ? 8 : 0));
                        return nullptr;
                    case 0x18: return (third & 0xC0) == 0xC0 || ((third >> 3) & 7) > 3 ? "undefined hint" : nullptr;
                    case 0xB2: case 0xB4: case 0xB5: return "far pointer load";
                    case 0xAF: return nullptr;
                    case 0x38: {   // the three-byte map: what Unicorn's processor has not got, and the register forms of MOVBE (which it runs as a load from the register)
                        const unsigned fourth = static_cast<unsigned>(bytes >> (8 * (i + 3))) & 0xFF;
                        if (third == 0xF0 && mandatory != 3 && wide && (rex & 8) == 0 && !override16) zext_ |= 1u << hardware_to_index(((fourth >> 3) & 7) + ((rex & 4) != 0 ? 8 : 0));   // (MOVBE of 32 bits)
                        if (third == 0xFC || (third == 0xF6 && (mandatory == 1 || mandatory == 2))) return invalid_insn();   // (the atomic adds, ADCX and ADOX)
                        if ((third == 0xF0 || third == 0xF1) && mandatory != 3 && (fourth >> 6) == 3 && (fourth & 7) != 4 && (fourth & 7) != 5) add_patch(pc + i + 3, fourth, fourth & 0x3F);
                        return nullptr;
                    }
                    case 0x3A:
                        if (third == 0x61 || third == 0x63) zext_ |= 4;   // PCMPESTRI and PCMPISTRI write ECX, which the description leaves zero-extended
                        if (third == 0x17 && ((static_cast<unsigned>(bytes >> (8 * (i + 3))) & 0xC0) != 0xC0)) extract_memory_ = true;
                        return nullptr;
                    case 0x1A: case 0x1B: return "MPX instruction or hint NOP";   // (the description stores bounds registers; Unicorn does nothing)
                    case 0xC7: return "RDRAND and the 8- and 16-byte compare-exchanges";
                    case 0xA0: case 0xA1: case 0xA8: case 0xA9: return "PUSH or POP of FS or GS";
                    case 0xBC: case 0xBD:   // BSF, BSR (TZCNT, LZCNT with F3): a zero source leaves the destination undefined, as the processors and Unicorn each leave it
                        if (override16) return "16-bit double shift or bit scan";
                        if (mandatory != 2) scan_ = {true, (rex & 8) != 0 ? 8u : 4u, false, third >> 6, third & 7, (rex & 1) != 0, rex != 0};
                        return nullptr;
                    case 0xA4: case 0xA5: case 0xAC: case 0xAD: return override16 ? "16-bit double shift or bit scan" : nullptr;
                    case 0x77: return mandatory != 0 ? "undefined prefix" : nullptr;
                    case 0x2C: case 0x2D:
                        if (mandatory < 2) return nullptr;
                        conv_ = {Conversion::Kind::ToInteger, next == 0x2C, mandatory == 3, (third >> 6) == 3, third & 7, (third >> 3) & 7, (rex & 1) != 0, (rex & 4) != 0, (rex & 8) != 0};
                        return nullptr;
                    case 0x5B:   // CVTPS2DQ, CVTTPS2DQ: the processor gives the "integer indefinite" for a lane out of range, the description a saturated value
                        if (mandatory == 1 || mandatory == 2) conv_ = {Conversion::Kind::PsToDq, mandatory == 2, false, (third >> 6) == 3, third & 7, (third >> 3) & 7, (rex & 1) != 0, (rex & 4) != 0, false};
                        return nullptr;
                    case 0xE6:
                        if (mandatory == 1 || mandatory == 3) conv_ = {Conversion::Kind::PdToDq, mandatory == 1, true, (third >> 6) == 3, third & 7, (third >> 3) & 7, (rex & 1) != 0, (rex & 4) != 0, false};
                        return nullptr;
                    case 0xF7:   // MASKMOVDQU and MASKMOVQ: the register form only; its helper faults without restoring the instruction pointer
                        if ((third & 0xC0) != 0xC0) return invalid_insn();
                        fault_pc_after_ = i + 3;
                        return nullptr;
                    case 0xAE:   // (with a REPZ prefix and a memory operand Unicorn runs FXSAVE and the rest as if there were none; the description reads RDGSBASE and the others)
                        if (mandatory == 2 && (third & 0xC0) != 0xC0) {
                            unsigned chosen = 0;
                            for (const unsigned candidate : {0x2Eu, 0x3Eu, 0x26u, 0x36u})
                                if (!prefix_present(bytes, i, candidate)) {
                                    chosen = candidate;
                                    break;
                                }
                            if (chosen == 0) return "a prefix repeated";
                            for (unsigned k = 0; k < i; ++k)
                                if (((bytes >> (8 * k)) & 0xFF) == 0xF3) add_patch(pc + k, 0xF3, chosen);
                        }
                        return nullptr;
                    case 0x50:   // MOVMSKPS, MOVMSKPD: the description leaves the upper half of the 64-bit register
                        zext_ |= 1u << hardware_to_index(((third >> 3) & 7) + ((rex & 4) != 0 ? 8 : 0));
                        return nullptr;
                    default:
                        if (next >= 0xC8 && next <= 0xCF && override16) return "BSWAP of 16 bits";
                        if (next == 0xC5 && mandatory <= 1) return "PEXTRW";
                        return nullptr;
                }
            }
            default: return nullptr;
        }
    }
    // Bytes of a ModRM operand: the ModRM byte, the SIB byte and the displacement (64-bit machine).
    static unsigned modrm_bytes(unsigned modrm, unsigned sib, bool address32) {
        (void)address32;
        const unsigned mod = modrm >> 6, rm = modrm & 7;
        if (mod == 3) return 1;
        unsigned n = 1;
        if (rm == 4) {
            ++n;
            if (mod == 0 && (sib & 7) == 5) n += 4;
        }
        if (mod == 0 && rm == 5) n += 4;
        else if (mod == 1) n += 1;
        else if (mod == 2) n += 4;
        return n;
    }
    // The bytes at `pc` are no instruction of the description. Unicorn runs some of them anyway; the adapter does what it does (the hint no-ops of 0F 18 to 0F 1F whatever their
    // ModRM byte, the register forms of MOVLPD and MOVHPD, which are MOVHLPS and MOVLHPS there), and says false for the rest: the instruction is then a #UD, or goes to Unicorn.
    bool bad_instruction(u64 pc) {
        if (bits_ != 64) return false;
        const u64 low = emu_.readValue(pc, 8), high = emu_.readValue(pc + 8, 8);
        const auto at = [&](unsigned k) { return static_cast<unsigned>((k < 8 ? low >> (8 * k) : high >> (8 * (k - 8))) & 0xFF); };
        unsigned i = 0;
        bool operand16 = false;
        unsigned mandatory = 0;
        for (; i < 14; ++i) {
            const unsigned b = at(i);
            if (b == 0x66) { operand16 = true; if (mandatory == 0) mandatory = 1; }
            else if (b == 0xF3) mandatory = 2;
            else if (b == 0xF2) mandatory = 3;
            else if (!(b == 0x67 || b == 0xF0 || b == 0x26 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x64 || b == 0x65 || (b >= 0x40 && b <= 0x4F))) break;
        }
        (void)operand16;
        if (at(i) != 0x0F) return false;
        const unsigned op = at(i + 1), modrm = at(i + 2);
        if (op >= 0x18 && op <= 0x1F) {   // (the hint no-ops: a ModRM operand, no immediate)
            emu_.setPc(pc + i + 2 + modrm_bytes(modrm, at(i + 3), false));
            return true;
        }
        if ((op == 0x12 || op == 0x16) && mandatory == 1 && (modrm >> 6) == 3) {
            unsigned rex = 0;
            for (unsigned k = 0; k < i; ++k)
                if (at(k) >= 0x40 && at(k) <= 0x4F) rex = at(k);
            const unsigned reg = ((modrm >> 3) & 7) + ((rex & 4) != 0 ? 8 : 0), rm = (modrm & 7) + ((rex & 1) != 0 ? 8 : 0);
            if (op == 0x12) emu_.setReg(m_->xmm[4 * reg], emu_.reg(m_->xmm[4 * rm + 1]));   // MOVHLPS: the low half of the destination is the high half of the source
            else emu_.setReg(m_->xmm[4 * reg + 1], emu_.reg(m_->xmm[4 * rm]));               // MOVLHPS
            emu_.setPc(pc + i + 3);
            return true;
        }
        if (op == 0xC3 && mandatory == 1) bad_hand_off_ = "MOVNTI with an operand-size prefix";
        return false;
    }
    // Whether Unicorn refuses `op` (one-byte map, or the second byte of a 0F opcode) with a LOCK prefix in front: found by running every opcode with and without it (the table is
    // pcode_cpu_x86_probe_test.cpp's, CB_X86_LOCK_DUMP). `next` is the byte after the opcode (the ModRM byte of the one-byte map), `modrm` the one of a 0F opcode.
    static bool lock_refused(unsigned op, unsigned next, unsigned modrm) {
        static const std::array<std::array<bool, 256>, 3> tables = [] {
            std::array<std::array<bool, 256>, 3> t{};
            const char* const lists[3] = {
                "02 03 04 05 0a 0b 0c 0d 12 13 14 15 1a 1b 1c 1d 22 23 24 25 2a 2b 2c 2d 32 33 34 35 38 39 3a 3b 3c 3d 90 a6 a7 ae af c5",   // one-byte map, a memory operand
                "00 01 02 03 04 05 08 09 0a 0b 0c 0d 10 11 12 13 14 15 18 19 1a 1b 1c 1d 20 21 22 23 24 25 28 29 2a 2b 2c 2d 30 31 32 33 34 35 38 39 3a 3b 3c 3d 80 81 83 90 a6 a7 ae af c4 c5 fe ff",   // a register
                "ae"};   // the 0F map (and b0 b1 with a register)
            for (int k = 0; k < 3; ++k)
                for (const char* c = lists[k]; *c != 0;) {
                    if (*c == ' ') {
                        ++c;
                        continue;
                    }
                    t[static_cast<std::size_t>(k)][static_cast<std::size_t>(std::strtoul(c, nullptr, 16))] = true;
                    c += 2;
                }
            return t;
        }();
        if (op == 0x0F) return tables[2][next] || ((next == 0xB0 || next == 0xB1) && (modrm >> 6) == 3);
        const bool register_operand = (next >> 6) == 3;
        if (op == 0xC4 || op == 0xC5 || (op >= 0x04 && (op & 7) >= 4 && (op & 7) <= 5 && op < 0x40) || op == 0x90 || (op >= 0xA6 && op <= 0xAF)) return tables[0][op];   // (no ModRM byte)
        if ((op == 0x80 || op == 0x81 || op == 0x83) && ((next >> 3) & 7) == 7) return true;   // (CMP cannot be locked)
        return tables[register_operand ? 1 : 0][op];
    }
    // An instruction the machine refuses (#UD): the adapter's own fault.
    const char* invalid_insn() {
        special_ = Special::Invalid;
        return nullptr;
    }
    // The index in Machine::gpr (RAX RBX RCX RDX RSI RDI RBP RSP R8 ..) of the processor's register number (RAX RCX RDX RBX RSP RBP RSI RDI R8 ..).
    static int hardware_to_index(unsigned hardware) {
        static constexpr int kIndex[16] = {0, 2, 3, 1, 7, 6, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15};
        return kIndex[hardware & 15];
    }
    // The two-byte opcodes (by the mandatory prefix: none, 66, F3, F2) that work on a 128-bit value, which the emulator's 64-bit varnodes cannot hold: found by running each opcode on
    // random vector registers against Unicorn (the two-byte sweep of pcode_cpu_x86_test.cpp).
    static bool wide_vector(unsigned mandatory, unsigned op) {
        static const std::array<std::array<bool, 256>, 4> table = [] {
            std::array<std::array<bool, 256>, 4> t{};
            const char* const lists[4] = {
                "10 11 15 28 29 2b 54 55 56 57 58 59 5b 5c c2",
                "10 11 28 29 2b 54 55 56 57 59 5a 5b 5c 64 65 66 67 68 69 6a 6f 74 7c 7d 7f c2 c4 d0 d4 d5 d7 da db de df e6 e7 ea eb ee ef f8 f9 fa fb fc fd fe",
                "12 16 5b 6f 7f",
                "7c 7d d0 e6"};
            for (int k = 0; k < 4; ++k)
                for (const char* c = lists[k]; *c != 0;) {
                    if (*c == ' ') {
                        ++c;
                        continue;
                    }
                    t[static_cast<std::size_t>(k)][static_cast<std::size_t>(std::strtoul(c, nullptr, 16))] = true;
                    c += 2;
                }
            return t;
        }();
        return table[mandatory][op];
    }
    // The 16-bit instruction at `i` (after its prefixes) as far as AF goes: which operation it is and where its operands are.
    void decode_af(u64 bytes, unsigned i) {
        const auto byte = [&](unsigned k) { return static_cast<unsigned>(bytes >> (8 * (i + k))) & 0xFF; };
        AfOp& a = af_;
        const unsigned op = byte(0);
        const auto modrm = [&](unsigned at) {
            const unsigned m = byte(at);
            a.mod = m >> 6;
            a.reg = (m >> 3) & 7;
            a.rm = m & 7;
            return a.mod == 0 && a.rm == 6 ? 2u : a.mod == 1 ? 1u : a.mod == 2 ? 2u : 0u;   // the displacement's length
        };
        const auto immediate = [&](unsigned at, unsigned size) { return static_cast<u64>(size == 1 ? byte(at) : byte(at) | (byte(at + 1) << 8)); };
        using K = AfOp::Kind;
        static constexpr K kKinds[8] = {K::Add, K::Clear, K::Adc, K::Sbb, K::Clear, K::Sub, K::Clear, K::Sub};   // ADD OR ADC SBB AND SUB XOR CMP
        if (op < 0x40 && (op & 7) < 6 && op != 0x0F) {
            a.kind = kKinds[(op >> 3) & 7];
            a.size = (op & 1) != 0 ? 2 : 1;
            const unsigned form = op & 7;
            if (form < 4) {
                modrm(1);
                a.a_is_e = form < 2;
                a.b_is_e = form >= 2;
                (form < 2 ? a.b_reg : a.a_reg) = a.reg;
            } else {
                a.a_reg = 0;
                a.b = immediate(1, a.size);
            }
        } else if (op >= 0x80 && op <= 0x83) {
            const unsigned len = modrm(1);
            a.size = (op & 1) != 0 ? 2 : 1;
            a.a_is_e = true;
            a.kind = kKinds[a.reg];
            const unsigned at = 2 + len;
            a.b = op == 0x83 ? static_cast<u64>(static_cast<std::int64_t>(static_cast<std::int8_t>(byte(at)))) & 0xFFFF : immediate(at, a.size);
        } else if (op >= 0x40 && op <= 0x4F) {
            a.kind = op < 0x48 ? K::Add : K::Sub;
            a.size = 2;
            a.a_reg = op & 7;
            a.b = 1;
        } else if (op == 0xFE || op == 0xFF) {
            modrm(1);
            if (a.reg > 1) return;
            a.kind = a.reg == 0 ? K::Add : K::Sub;
            a.size = op == 0xFE ? 1 : 2;
            a.a_is_e = true;
            a.b = 1;
        } else if (op == 0xF6 || op == 0xF7) {
            modrm(1);
            a.size = op == 0xF6 ? 1 : 2;
            if (a.reg == 3) {   // NEG: 0 - the operand
                a.kind = K::Sub;
                a.a = 0;
                a.b_is_e = true;
            } else if (a.reg == 0 || a.reg == 1) {   // TEST
                a.kind = K::Clear;
            }
        } else if (op == 0x84 || op == 0x85 || op == 0xA8 || op == 0xA9) {
            a.kind = K::Clear;
        } else if (op == 0xD0 || op == 0xD1) {
            modrm(1);
            if (a.reg >= 4 && a.reg != 6) a.kind = K::Clear;   // (shifts by one; SAL's alias /6 is Unicorn's SHL too; the rotates leave AF as it was)
            if (a.reg == 6) a.kind = K::Clear;
        } else if (op == 0xA6 || op == 0xA7 || op == 0xAE || op == 0xAF) {
            a.kind = (op & 8) != 0 ? K::Scas : K::Cmps;
            a.size = (op & 1) != 0 ? 2 : 1;
            // the source segment of CMPS: DS unless an override says
            a.segment = 1;
            a.rep = prefix_present(bytes, i, 0xF3) || prefix_present(bytes, i, 0xF2);
            for (unsigned k = 0; k < i; ++k) {
                switch (static_cast<unsigned>(bytes >> (8 * k)) & 0xFF) {
                    case 0x26: a.segment = 2; break;
                    case 0x2E: a.segment = 0; break;
                    case 0x36: a.segment = 3; break;
                    case 0x3E: a.segment = 1; break;
                    default: break;
                }
            }
        }
    }
    // AF after the instruction decode_af found: the carry out of bit 3 as Unicorn works it out, (a ^ b ^ result) & 0x10 for an addition or a subtraction, 0 for the rest.
    u64 af_value(const Snapshot& before) const {
        using K = AfOp::Kind;
        const AfOp& a = af_;
        if (a.kind == K::Clear) return 0;
        const u64 mask = a.size == 1 ? 0xFF : 0xFFFF;
        const auto register_value = [&](unsigned n) -> u64 {
            if (a.size == 1) return n < 4 ? before.gpr(hardware_to_index(n)) & 0xFF : (before.gpr(hardware_to_index(n - 4)) >> 8) & 0xFF;
            return before.gpr(hardware_to_index(n)) & 0xFFFF;
        };
        u64 x = a.a, y = a.b;
        if ((a.kind == K::Cmps || a.kind == K::Scas) && a.rep && (before.gpr(hardware_to_index(1)) & 0xFFFF) == 0) return (before.flags(0) >> 32) & 1;   // (nothing was compared)
        if (a.kind == K::Cmps || a.kind == K::Scas) {
            const u64 si = before.gpr(hardware_to_index(6)) & 0xFFFF, di = before.gpr(hardware_to_index(7)) & 0xFFFF;
            const u64 es = before.seg(2), source = before.seg(a.segment);
            const u64 dest_value = emu_.readValue((es << 4) + di, a.size);
            if (a.kind == K::Scas) {
                x = register_value(0);
                y = dest_value;
            } else {
                x = emu_.readValue((source << 4) + si, a.size);
                y = dest_value;
            }
        } else {
            if (a.a_is_e) x = a.mod == 3 ? register_value(a.rm) : e_value_;
            else if (a.a_reg != 99) x = register_value(a.a_reg);
            if (a.b_is_e) y = a.mod == 3 ? register_value(a.rm) : e_value_;
            else if (a.b_reg != 99) y = register_value(a.b_reg);
        }
        x &= mask;
        y &= mask;
        const u64 carry = a.kind == K::Adc || a.kind == K::Sbb ? before.flags(0) & 1 : 0;
        const u64 result = (a.kind == K::Add || a.kind == K::Adc ? x + y + carry : x - y - carry) & mask;
        return ((x ^ y ^ result) >> 4) & 1;
    }
    static bool prefix_present(u64 bytes, unsigned prefixes, unsigned value) {
        for (unsigned k = 0; k < prefixes; ++k)
            if (((bytes >> (8 * k)) & 0xFF) == value) return true;
        return false;
    }
    static bool operand_16_override(u64 bytes, unsigned prefixes) {
        for (unsigned k = 0; k < prefixes; ++k)
            if (((bytes >> (8 * k)) & 0xFF) == 0x66) return true;
        return false;
    }
    // The value of the instruction's ModRM operand (a register of the width given, or the memory the description read last), as it was before the instruction.
    u64 operand_value(const Division& d, const Snapshot& before) const {
        u64 value = 0;
        if (d.mod == 3) {
            const unsigned n = d.rm + (d.rex_b ? 8 : 0);
            if (d.bytes == 1 && !d.has_rex && n >= 4 && n < 8) value = (before.gpr(hardware_to_index(n - 4)) >> 8) & 0xFF;   // AH, CH, DH, BH
            else value = before.gpr(hardware_to_index(n));
        } else {
            value = emu_.readValue(last_read_address_, d.bytes);
        }
        return d.bytes >= 8 ? value : value & ((u64{1} << (8 * d.bytes)) - 1);
    }
    bool source_is_zero(const Division& d, const Snapshot& before) const { return operand_value(d, before) == 0; }
    u64 declined_total() const {
        const auto& m = emu_.unimplementedOps();
        if (m.empty()) return 0;
        u64 t = 0;
        for (const auto& [name, count] : m) t += count;
        return t;
    }
    static std::string declined_name(const int) { return "bound operation"; }
    int before_names_ = 0;
    // The 128-bit dividend hi:lo over a 64-bit divisor, whose quotient is known to fit 64 bits (hi < divisor): the quotient, by the long division of the bits.
    static u64 divide_wide(u64 hi, u64 lo, u64 divisor) {
        u64 remainder = hi, quotient = 0;
        for (int bit = 63; bit >= 0; --bit) {
            const bool carry = (remainder >> 63) != 0;
            remainder = (remainder << 1) | ((lo >> bit) & 1);
            if (carry || remainder >= divisor) {
                remainder -= divisor;
                quotient |= u64{1} << bit;
            }
        }
        return quotient;
    }
    // After a DIV or IDIV the description has run: whether the processor would have faulted (a zero divisor the description fails on itself; a quotient too big it does not know).
    bool division_faults(const Snapshot& before) {
        const Division d = division_;
        division_.on = false;
        u64 divisor = operand_value(d, before);
        if (divisor == 0) return true;
        const unsigned bits = d.bytes * 8;
        const u64 mask = bits == 64 ? ~u64{0} : ((u64{1} << bits) - 1);
        const u64 rax = before.gpr(0), rdx = before.gpr(3);
        u64 quotient = 0;
        bool negative_dividend = false, negative_divisor = false;
        if (d.bytes == 8) {   // rdx:rax, a 128-bit dividend
            u64 hi = rdx, lo = rax;
            if (d.is_signed) {
                negative_dividend = (hi >> 63) != 0;
                negative_divisor = (divisor >> 63) != 0;
                if (negative_dividend) {
                    lo = ~lo + 1;
                    hi = ~hi + (lo == 0 ? 1 : 0);
                }
                if (negative_divisor) divisor = ~divisor + 1;
            }
            if (hi >= divisor) return true;
            quotient = divide_wide(hi, lo, divisor);
        } else {
            u64 dividend = 0;   // the dividend, as a word
            switch (d.bytes) {
                case 1: dividend = rax & 0xFFFF; break;
                case 2: dividend = (rax & 0xFFFF) | ((rdx & 0xFFFF) << 16); break;
                default: dividend = (rax & 0xFFFFFFFFull) | ((rdx & 0xFFFFFFFFull) << 32); break;
            }
            const unsigned width = bits * 2;   // the dividend's bits
            if (d.is_signed) {   // (the magnitudes of both)
                negative_divisor = ((divisor >> (bits - 1)) & 1) != 0;
                negative_dividend = ((dividend >> (width - 1)) & 1) != 0;
                if (negative_divisor) divisor = (~divisor + 1) & mask;
                if (negative_dividend) dividend = (~dividend + 1) & (width == 64 ? ~u64{0} : ((u64{1} << width) - 1));
            }
            quotient = dividend / divisor;
        }
        if (!d.is_signed) return d.bytes == 8 ? false : quotient > mask;
        const u64 limit = u64{1} << (bits - 1);   // (a negative quotient may be -limit)
        return negative_dividend != negative_divisor ? quotient > limit : quotient >= limit;
    }
    // The result of the float to integer conversion in conv_, from the operand as it was before the instruction.
    void convert(const Snapshot& before) {
        const Conversion c = conv_;
        namespace ops = pcode_ops_x86;
        const unsigned source_register = c.rm + (c.rex_b ? 8 : 0), dest_register = c.reg + (c.rex_r ? 8 : 0);
        const unsigned ew = c.kind == Conversion::Kind::PdToDq || c.is_double ? 8 : 4;
        const unsigned bytes = c.kind == Conversion::Kind::ToInteger ? ew : c.vector_bytes;
        std::uint8_t source[32] = {};
        if (c.register_source) std::memcpy(source, before.raw + m_->xmm_slot[4 * source_register].dst, bytes);
        else emu_.read(first_read_address_, source, bytes);
        vec::FpEnv env = ops::environment(m_->ops_context.has_mxcsr ? emu_.reg(m_->ops_context.mxcsr) : 0);
        if (c.truncate) env.round = vec::Round::TowardZero;
        unsigned flags = 0;
        std::uint8_t out[32] = {};
        if (c.kind == Conversion::Kind::ToInteger) {
            const unsigned iew = c.rex_w ? 8 : 4;
            vec::floatToInt(out, source, ew, ew, iew, vec::Sign::Signed, 0, vec::IntOverflow::X86Indefinite, env, flags);
            u64 value = 0;
            std::memcpy(&value, out, iew);
            emu_.setReg(m_->gpr[hardware_to_index(dest_register)], value);
        } else {
            vec::floatToInt(out, source, bytes, ew, 4, vec::Sign::Signed, 0, vec::IntOverflow::X86Indefinite, env, flags);
            sl::Emulator::RegRef r = m_->xmm[4 * dest_register];
            r.size = c.kind == Conversion::Kind::PdToDq ? 16 : bytes;   // (four doubles make four dwords: the destination is always an XMM register)
            emu_.writeRegWide(r, out);
        }
        if (m_->ops_context.has_mxcsr && flags != 0) emu_.setReg(m_->ops_context.mxcsr, emu_.reg(m_->ops_context.mxcsr) | ops::exception_bits(flags));
    }
    // The FMA instruction in fma_: opcodes 96..9F, A6..AF, B6..BF (the form in the high nibble: 132, 213, 231; the operation in the low one).
    void fused_multiply_add(const Snapshot& before) {
        const Fma f = fma_;
        namespace ops = pcode_ops_x86;
        const unsigned form = f.opcode >> 4, kind = f.opcode & 15;
        const unsigned ew = f.wide != 0 ? 8 : 4;
        const bool scalar = kind >= 9 && (kind & 1) != 0;   // (9, B, D, F)
        const std::size_t bytes = scalar ? ew : f.length != 0 ? 32 : 16;
        std::uint8_t x[32] = {}, y[32] = {}, z[32] = {}, result[32] = {};
        std::memcpy(x, before.raw + m_->xmm_slot[4 * f.reg].dst, 32);
        std::memcpy(y, before.raw + m_->xmm_slot[4 * f.vvvv].dst, 32);
        if (f.register_source) std::memcpy(z, before.raw + m_->xmm_slot[4 * f.rm].dst, 32);
        else emu_.read(first_read_address_, z, bytes);
        std::uint8_t *a = y, *b = z, *c = x;   // 231
        if (form == 9) { a = x; b = z; c = y; }            // 132: dest * src3 + src2
        else if (form == 0xA) { a = y; b = x; c = z; }     // 213: src2 * dest + src3
        vec::Fp3 op = vec::Fp3::MulAdd;
        switch (kind) {
            case 8: case 9: case 6: case 7: op = vec::Fp3::MulAdd; break;
            case 0xA: case 0xB: op = vec::Fp3::MulSub; break;
            case 0xC: case 0xD: op = vec::Fp3::NegMulAdd; break;
            default: op = vec::Fp3::NegMulSub; break;
        }
        vec::FpEnv env = ops::environment(m_->ops_context.has_mxcsr ? emu_.reg(m_->ops_context.mxcsr) : 0);
        unsigned flags = 0;
        if (kind == 6 || kind == 7) {   // FMADDSUB (even lanes subtract) and FMSUBADD (even lanes add)
            std::uint8_t added[32], subtracted[32];
            vec::fp3(added, a, b, c, bytes, ew, vec::Fp3::MulAdd, env, flags);
            vec::fp3(subtracted, a, b, c, bytes, ew, vec::Fp3::MulSub, env, flags);
            for (std::size_t i = 0; i < bytes; i += ew)
                std::memcpy(result + i, ((i / ew) % 2 == 0) == (kind == 6) ? subtracted + i : added + i, ew);
        } else {
            vec::fp3(result, a, b, c, bytes, ew, op, env, flags);
        }
        if (scalar) {   // (the other lanes of the destination stay, the register above 128 bits is cleared)
            std::uint8_t whole[32] = {};
            std::memcpy(whole, x, 16);
            std::memcpy(whole, result, ew);
            std::memcpy(result, whole, 32);
        }
        sl::Emulator::RegRef r = m_->xmm[4 * f.reg];
        r.size = 32;
        emu_.writeRegWide(r, result);
        if (m_->ops_context.has_mxcsr && flags != 0) emu_.setReg(m_->ops_context.mxcsr, emu_.reg(m_->ops_context.mxcsr) | ops::exception_bits(flags));
    }
    std::string bytes_text(u64 pc) const {
        const u64 v = emu_.readValue(pc, 8);
        std::string t;
        for (int k = 0; k < 6; ++k) t += std::format("{:02x}", static_cast<unsigned>(v >> (8 * k)) & 0xFF);
        return t;
    }
    std::string opcode_key(u64 pc) const {
        const u64 bytes = emu_.readValue(pc, 8);
        unsigned i = 0;
        while (i < 6) {
            const unsigned b = static_cast<unsigned>(bytes >> (8 * i)) & 0xFF;
            if (b == 0x66 || b == 0x67 || b == 0xF2 || b == 0xF3 || b == 0xF0 || b == 0x26 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x64 || b == 0x65 || (bits_ == 64 && b >= 0x40 && b <= 0x4F)) ++i;
            else break;
        }
        const unsigned op = static_cast<unsigned>(bytes >> (8 * i)) & 0xFF;
        if (op == 0x0F) return std::format("0f {:02x}", static_cast<unsigned>(bytes >> (8 * (i + 1))) & 0xFF);
        return std::format("{:02x}", op);
    }

    // The rest of the run on Unicorn: a engine made to be the machine as it is now.
    void hand_off(const char* why, u64 until, u64 count, u64 executed) {
        ++g_stats.handoffs[why];
        const u64 pc = emu_.pc();
        twin_ = uu::open_x86(bits_);
        for (const Region& r : regions_) twin_->mem_map(r.begin, r.end - r.begin);
        for (const Image& w : image_) twin_->mem_write(w.address, w.data);
        Bytes page(kPage);
        for (const u64 index : touched_) {
            emu_.read(index * kPage, page.data(), page.size());
            twin_->mem_write(index * kPage, page);
        }
        const auto id = [](const char* name) { return *uu::x86_register(name); };
        for (int i = 0; i < m_->gprs; ++i) twin_->reg_write(id(bits_ == 64 ? kGpr64[i] : kGpr16[i]), emu_.reg(m_->gpr[i]));
        if (bits_ == 16)   // (the 64-bit machine's segment registers are Unicorn's own: writing them would take it out of long mode)
            for (int i = 0; i < 6; ++i) twin_->reg_write(id(kSeg[i]), emu_.reg(m_->seg[i]));
        twin_->reg_write(id("EFLAGS"), flag_bits());
        if (bits_ == 64) {
            twin_->reg_write(id("FS_BASE"), emu_.reg(m_->fs_base));
            twin_->reg_write(id("GS_BASE"), emu_.reg(m_->gs_base));
            const emu::unicorn::Api& api = emu::unicorn::api();
            auto engine = static_cast<uc_engine*>(twin_->handle());
            for (int i = 0; i < 16; ++i) {
                std::uint8_t buffer[16];
                emu_.read(emu_.regRef("XMM" + std::to_string(i)).offset, buffer, 16, "register");
                api.uc_reg_write(engine, UC_X86_REG_XMM0 + i, buffer);
            }
            twin_->reg_write(id("MXCSR"), emu_.reg("MXCSR"));
        }
        if (g_stats.count_tail) twin_->hook_add_code([](core16::Uc&, u64, u32) { ++g_stats.tail_steps; }, 0, ~u64{0});   // (the instructions Unicorn runs: counted for the report, slow)
        for (CodeHook& h : code_hooks_) twin_->hook_add_code(h.callback, h.begin, h.end);
        if (in_) twin_->hook_add_insn_in(in_);
        if (out_) twin_->hook_add_insn_out(out_);
        if (syscall_) syscall_guard_ = add_unicorn_syscall_hook(*twin_, syscall_);
        twin_->emu_start(pc, until, 0, count == 0 ? 0 : count - executed);
    }

    bool user_op(const std::string& name, const std::vector<u64>& args, u64& result) {
        if (name == "in" && in_) {
            result = in_(*this, static_cast<u32>(args.empty() ? 0 : args[0]), io_size());
            return true;
        }
        if (name == "out" && out_) {
            const u32 size = io_size();
            const bool string_form = io_opcode_ == 0x6E || io_opcode_ == 0x6F;   // (OUTS gives the description's operation its value first, then the port)
            const u32 port = static_cast<u32>(args.size() > 1 && string_form ? args[1] : args.empty() ? 0 : args[0]);
            const u32 value = static_cast<u32>(args.size() > 1 ? (string_form ? args[0] : args[1]) : 0);
            out_(*this, port, size, value);
            return true;
        }
        if (name == "syscall" && syscall_) {
            emu_.setReg(m_->gpr[2], syscall_rcx_);
            emu_.setReg(m_->gpr[11], syscall_r11_);
            syscall_(*this);
            return true;
        }
        if (name == "LOCK" || name == "UNLOCK" || name == "XACQUIRE" || name == "XRELEASE") return true;   // (prefixes: nothing for one processor to do)
        if (name == "pause" || name == "mfence" || name == "lfence" || name == "sfence" || name == "clflush" || name == "clflushopt" || name == "clwb" || name == "cldemote" ||
            name.rfind("prefetch", 0) == 0)
            return true;   // (no effect on the machine's registers or memory)
        ++g_stats.unserved[name];
        if (unserved_ == nullptr) unserved_ = "an operation the emulator does not carry";
        return false;
    }
    // The width of the IN or OUT being run: the opcode at the instruction's address (a 66 prefix turns the machine's word round).
    u32 io_size() {
        u64 at = emu_.pc();
        bool operand_override = false;
        for (int i = 0; i < 6; ++i) {
            const unsigned b = static_cast<unsigned>(emu_.readValue(at, 1));
            if (b == 0x66) {
                operand_override = true;
            } else if (b == 0x67 || b == 0xF2 || b == 0xF3 || b == 0x26 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x64 || b == 0x65 || (bits_ == 64 && b >= 0x40 && b <= 0x4F)) {
            } else {
                io_opcode_ = b;
                if (b == 0xE4 || b == 0xE6 || b == 0xEC || b == 0xEE || b == 0x6C || b == 0x6E) return 1;
                const bool wide = bits_ == 16 ? operand_override : !operand_override;
                return wide ? 4 : 2;
            }
            ++at;
        }
        return 1;
    }

    Machine* m_;
    std::unique_ptr<Machine> own_;
    sl::Emulator& emu_;
    int bits_;
    const std::unordered_map<int, std::string_view>* names_ = nullptr;
    std::vector<Region> regions_;
    std::vector<CodeHook> code_hooks_;
    std::function<u32(core16::Uc&, u32, u32)> in_;
    std::function<void(core16::Uc&, u32, u32, u32)> out_;
    std::function<void(core16::Uc&)> syscall_;
    struct Image {
        u64 address;
        Bytes data;
    };
    std::vector<Image> image_;          // what mem_write wrote, in order (a Unicorn engine is handed it)
    std::unordered_set<u64> touched_;   // the pages the run has written since: a Unicorn engine is handed them as they are
    u64 last_page_ = ~u64{0};
    struct Stored {
        u64 address = 0;
        unsigned size = 0;
        std::uint8_t old[32];
    };
    static constexpr std::size_t kJournal = 16;
    Stored journal_[kJournal];     // what the instruction being run has stored: the memory it was before
    std::size_t journal_size_ = 0;
    const char* unserved_ = nullptr;   // set by a user operation nothing served during the instruction being run
    unsigned io_opcode_ = 0;       // the opcode of the IN or OUT last served
    // AF of the 16-bit machine: the description never computes it, Unicorn does for every arithmetic instruction; the adapter works it out from the operands.
    struct AfOp {
        enum class Kind : std::uint8_t { None, Clear, Add, Adc, Sub, Sbb, Cmps, Scas } kind = Kind::None;   // (INC and ADD are one: an addition; DEC, NEG and CMP are subtractions)
        unsigned size = 0;               // 1 or 2 bytes
        bool a_is_e = false, b_is_e = false;   // an operand is the ModRM byte's register or memory
        unsigned mod = 0, rm = 0, reg = 0;
        u64 a = 0, b = 0;                 // an operand that is not a ModRM one: the value (fixed at decoding, a register's is filled from the snapshot by af_value)
        unsigned a_reg = 99, b_reg = 99;  // or a register number (the processor's, byte or word as `size` says), 99: none
        unsigned segment = 1;             // CMPS and SCAS: the segment register of the source (index in Machine::seg: DS)
        bool rep = false;                 // CMPS and SCAS: with a REP prefix (a count of zero compares nothing)
    } af_;
    u64 e_value_ = 0;                     // the memory operand the description read, as it was (the first read of the instruction)
    bool e_captured_ = false;
    enum class Special : std::uint8_t { None, Invalid, Halt, Interrupt, Exception, InvalidAfterLoad, Cpuid, Nop } special_ = Special::None;   // what the adapter does itself for the instruction about to run
    unsigned special_len_ = 0;     // its bytes
    struct Patch {
        u64 address;
        std::uint8_t original, inert;
    };
    Patch patch_[16];               // the REX bytes the instruction about to run has that count for nothing, and what they are run as
    unsigned patch_count_ = 0;
    u64 trail_[3] = {0, 0, 0};
    u64 addr32_at_ = 0;            // the address of a 67 prefix of the instruction about to run (0: none)
    u64 lock_at_ = 0;              // the address of a LOCK prefix of the instruction about to run (0: none)
    void add_patch(u64 address, unsigned original, unsigned inert) {
        if (patch_count_ < 16) patch_[patch_count_++] = {address, static_cast<std::uint8_t>(original), static_cast<std::uint8_t>(inert)};
    }
    sl::Emulator::Status step_patched() {
        for (unsigned k = 0; k < patch_count_; ++k) emu_.write(patch_[k].address, &patch_[k].inert, 1);
        const sl::Emulator::Status status = emu_.step();
        for (unsigned k = patch_count_; k-- > 0;) emu_.write(patch_[k].address, &patch_[k].original, 1);
        return status;
    }
    struct Fma {
        bool on = false;
        unsigned opcode = 0, wide = 0, length = 0, vvvv = 0, reg = 0;
        bool register_source = false;
        unsigned rm = 0;
    } fma_;
    bool extract_memory_ = false;  // EXTRACTPS to memory: the description stores a quadword, the processor a doubleword
    unsigned fault_pc_after_ = 0;  // an access outside the regions leaves the instruction pointer this many bytes on (MASKMOVDQU)
    // A float to integer conversion the processor and Unicorn give the "integer indefinite" (the minimum signed value) for a value out of range; the description's own operation
    // saturates, so the adapter works the result out from the operand as it was.
    struct Conversion {
        enum class Kind : std::uint8_t { None, ToInteger, PsToDq, PdToDq } kind = Kind::None;
        bool truncate = false, is_double = false, register_source = false;
        unsigned rm = 0, reg = 0;
        bool rex_b = false, rex_r = false, rex_w = false;
        unsigned vector_bytes = 16;   // the width of a packed conversion (a VEX one may be 32)
    } conv_;
    bool cqo_ = false;             // the instruction is CQO
    bool linear_ip_ = false;       // the instruction (RET imm16, RETF, IRET of the 16-bit machine) leaves the new instruction pointer as an offset
    bool far_call_ = false;        // the instruction is CALLF ptr16:16 (16-bit machine), whose segment and offset are far_segment_ and far_offset_
    unsigned far_segment_ = 0, far_offset_ = 0;
    bool vector_ = false;          // the instruction being run can change the vector registers
    u64 syscall_rcx_ = 0, syscall_r11_ = 0;   // RCX and R11 from before a SYSCALL
    unsigned zext_ = 0;            // the registers (a bit each, by index into Machine::gpr) the instruction wrote 32 bits of, which the processor zero-extends and the description leaves
    Division division_;
    Division scan_;                // the source of a BSF or BSR (bytes: its width)
    u64 last_read_address_ = 0;   // where the description's last read was (a division's memory divisor)
    u64 first_read_address_ = 0;  // and its first (a vector operand is read a word at a time: where it begins)
    bool read_seen_ = false;
    const char* bad_hand_off_ = nullptr;   // an instruction the description cannot decode and the adapter cannot carry out: the reason it goes to Unicorn
    int fault_ = 0;
    bool stop_ = false;
    std::unique_ptr<core16::Uc> twin_;
    std::shared_ptr<UnicornSyscallHook> syscall_guard_;
};

}  // namespace

std::unique_ptr<core16::Uc> open_x86_emulator(int bits) {
    if (bits != 16 && bits != 64) throw UsageError("the p-code x86 engine is 16 or 64 bits");
    return std::make_unique<PcodeUc>(bits);
}

std::unique_ptr<core16::Uc> open_x86(int bits) {
    if (enabled() && (bits == 16 || bits == 64)) return open_x86_emulator(bits);
    return uu::open_x86(bits);
}

bool is_emulator_engine(const core16::Uc& uc) { return dynamic_cast<const PcodeUc*>(&uc) != nullptr; }

bool ran_on_unicorn(const core16::Uc& uc) {
    const auto* mine = dynamic_cast<const PcodeUc*>(&uc);
    return mine != nullptr && mine->on_twin();
}

std::shared_ptr<void> hook_syscall(core16::Uc& uc, std::function<void(core16::Uc&)> hook) {
    if (auto* mine = dynamic_cast<PcodeUc*>(&uc)) {
        mine->set_syscall(std::move(hook));
        return std::shared_ptr<void>(mine, [](PcodeUc* p) { p->set_syscall({}); });
    }
    return add_unicorn_syscall_hook(uc, std::move(hook));
}

Bytes read_xmm(core16::Uc& uc, unsigned index) {
    if (auto* mine = dynamic_cast<PcodeUc*>(&uc)) return mine->read_xmm(index);
    return read_unicorn_xmm(uc, index);
}

Bytes read_ymm(core16::Uc& uc, unsigned index) {
    if (auto* mine = dynamic_cast<PcodeUc*>(&uc)) return mine->read_ymm(index);
    Bytes out = read_unicorn_xmm(uc, index);
    out.resize(32, 0);
    return out;
}

void write_ymm(core16::Uc& uc, unsigned index, ByteView bytes) {
    if (auto* mine = dynamic_cast<PcodeUc*>(&uc)) return mine->write_ymm(index, bytes);
    write_unicorn_xmm(uc, index, bytes);
}

void write_xmm(core16::Uc& uc, unsigned index, ByteView bytes) {
    if (auto* mine = dynamic_cast<PcodeUc*>(&uc)) return mine->write_xmm(index, bytes);
    write_unicorn_xmm(uc, index, bytes);
}

X86Stats& x86_stats() { return g_stats; }

X86Options& x86_options() {
    thread_local X86Options options;
    return options;
}

}  // namespace cb::emulate::pcode_cpu
