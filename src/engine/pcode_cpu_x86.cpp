// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_cpu_x86.cpp -- an x86 engine on the one emulator, as the core16::Uc Unicorn's engine is (pcode_cpu_x86.hpp).
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
// lanes' comparison; for the WonderSwan lane, which compares the flags, it is why that lane is not on this engine.
#include "engine/pcode_cpu_x86.hpp"

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
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_internal.hpp"

#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

namespace cb::engine::pcode_cpu {

namespace sl = cb::decompile::sleigh;
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
                const RegRef x = emu.regRef("XMM" + std::to_string(i));
                xmm[2 * i] = {x.space, x.offset, 8};
                xmm[2 * i + 1] = {x.space, x.offset + 8, 8};
            }
        fs_base = emu.regRef("FS_OFFSET");
        gs_base = emu.regRef("GS_OFFSET");
    }
    int bits;
    sl::Emulator emu;
    RegRef flag[21];
    RegRef flag_words[3];
    RegRef gpr[16];
    RegRef seg[6];
    RegRef xmm[32];
    RegRef fs_base, gs_base;
    int gprs = 0;
    bool busy = false;
};

// The bit of EFLAGS each of the description's flag registers is (IOPL is a field, bits 12 and 13).
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
        emu_.onRead([this](std::string_view space, u64 address, unsigned size, u64& value) {
            if (!is_ram(space)) return false;
            if (!mapped(address, size)) {
                if (fault_ == 0) fault_ = kErrReadUnmapped;
                value = 0;
                return true;
            }
            last_read_address_ = address;
            if (af_.kind != AfOp::Kind::None && !e_captured_) {
                e_captured_ = true;
                e_value_ = emu_.readValue(address, size);
            }
            return false;
        });
        emu_.onWrite([this](std::string_view space, u64 address, unsigned size, u64) {
            if (!is_ram(space)) return false;
            if (fault_ != 0) return true;   // (an instruction that faulted stores nothing more)
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
            return false;
        });
        emu_.onUserOp([this](sl::Emulator&, const std::string& name, const std::vector<u64>& args, u64& result) { return user_op(name, args, result); });
    }
    ~PcodeUc() override {
        syscall_guard_.reset();
        twin_.reset();
        emu_.onRead({});
        emu_.onWrite({});
        emu_.onUserOp({});
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
        const std::vector<std::uint8_t> zeros(static_cast<std::size_t>(std::min<u64>(size, 1u << 20)), 0);
        for (u64 at = 0; at < size; at += zeros.size()) emu_.write(address + at, zeros.data(), static_cast<std::size_t>(std::min<u64>(zeros.size(), size - at)));
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
        touch(address, data.size());
        emu_.write(address, data.data(), data.size());
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
            if (const char* why = machine_instruction(pc)) return hand_off(why, until, count, executed);
            save(before);
            fault_ = 0;
            journal_size_ = 0;
            unserved_ = nullptr;
            e_captured_ = false;
            const sl::Emulator::Status status = emu_.step();
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
                return hand_off("division fault", until, count, executed);
            }
            if (status == sl::Emulator::Status::Ok && fault_ == 0) {
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
            const char* why = fault_ == kErrReadUnmapped ? "read outside the regions" : fault_ == kErrWriteUnmapped ? "write outside the regions"
                              : status == sl::Emulator::Status::BadInstruction ? "no instruction in the description" : "the description failed";
            if (status == sl::Emulator::Status::BadInstruction) ++g_stats.opcodes[opcode_key(pc)];
            return hand_off(why, until, count, executed);
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
    // The registers an instruction can change, kept to put back before an instruction goes to Unicorn.
    struct Snapshot {
        u64 gpr[16];
        u64 flags[3];  // the flag registers' bytes, eight at a time
        u64 seg[6];
        u64 xmm[32];   // XMM0..15, low and high halves (64-bit machine)
        u64 pc;
    };
    void save(Snapshot& s) const {
        for (int i = 0; i < m_->gprs; ++i) s.gpr[i] = emu_.reg(m_->gpr[i]);
        for (int i = 0; i < 3; ++i) s.flags[i] = emu_.reg(m_->flag_words[i]);
        if (bits_ == 16)
            for (int i = 0; i < 6; ++i) s.seg[i] = emu_.reg(m_->seg[i]);
        if (vector_ && bits_ == 64)
            for (int i = 0; i < 32; ++i) s.xmm[i] = emu_.reg(m_->xmm[i]);
        s.pc = emu_.pc();
    }
    void restore(const Snapshot& s) {
        for (std::size_t i = journal_size_; i-- > 0;) emu_.write(journal_[i].address, journal_[i].old, journal_[i].size);
        journal_size_ = 0;
        for (int i = 0; i < m_->gprs; ++i) emu_.setReg(m_->gpr[i], s.gpr[i]);
        for (int i = 0; i < 3; ++i) emu_.setReg(m_->flag_words[i], s.flags[i]);
        if (bits_ == 16)
            for (int i = 0; i < 6; ++i) emu_.setReg(m_->seg[i], s.seg[i]);
        if (vector_ && bits_ == 64)
            for (int i = 0; i < 32; ++i) emu_.setReg(m_->xmm[i], s.xmm[i]);
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
    const char* machine_instruction(u64 pc) {
        const u64 bytes = emu_.readValue(pc, 8);
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
        vector_ = op == 0x0F;
        zext_ = 0;
        const bool override16 = operand_16_override(bytes, i);
        for (unsigned a = 0; a < i; ++a)   // (a prefix said twice: the description counts each, the processor does not)
            for (unsigned b = a + 1; b < i; ++b)
                if (((bytes >> (8 * a)) & 0xFF) == ((bytes >> (8 * b)) & 0xFF) && !(wide && ((bytes >> (8 * a)) & 0xF0) == 0x40)) return "a prefix repeated";
        if (wide && override16) {   // (Intel's 16-bit branch in long mode cuts the instruction pointer to 16 bits: the description does not)
            if ((op >= 0x70 && op <= 0x7F) || (op >= 0xE0 && op <= 0xE3) || op == 0xE8 || op == 0xE9 || op == 0xEB || op == 0xC2 || op == 0xC3 || (op == 0x0F && next >= 0x80 && next <= 0x8F) ||
                (op == 0xFF && (reg == 2 || reg == 4)))
                return "branch with an operand-size prefix";
        }
        switch (op) {
            case 0xF4: return "HLT";
            case 0xCC: case 0xCD: return "INT";
            case 0xCE: return wide ? "invalid in 64-bit mode" : "INT";
            case 0x62: return wide ? "EVEX" : "BOUND";
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
            case 0x37: case 0x3F: return wide ? "invalid in 64-bit mode" : "AAA and AAS (AX + 0x106)";
            case 0x27: case 0x2F: return wide ? "invalid in 64-bit mode" : "DAA and DAS (OF)";   // (OF after them is whatever the machine chooses)
            case 0x69: case 0x6B:
                if (wide) return (rex & 8) != 0 ? "64-bit IMUL (a 128-bit product)" : nullptr;
                return "IMUL by an immediate (SF, ZF, PF are whatever the machine chooses)";
            case 0xD1:   // RCR of a 64-bit register: the description rotates a 128-bit value, which the emulator cannot hold
            case 0xC1: case 0xD3:
                if (wide && reg == 3 && (rex & 8) != 0) return "64-bit RCR (a 128-bit rotate)";
                return op == 0xD1 || wide ? nullptr : "shift or rotate by a count (OF is whatever the machine chooses)";
            case 0xC0: case 0xD2: return wide ? nullptr : "shift or rotate by a count (OF is whatever the machine chooses)";   // (the description adds 1 to AH, not the carry out of AL too)
            case 0xC4: case 0xC5: return wide ? "VEX" : (next >> 6) == 3 ? "LES or LDS of a register (the description reads a VEX prefix)" : nullptr;
            case 0x63: return wide ? nullptr : "ARPL (invalid in real mode)";
            case 0x8E: return wide ? "MOV to a segment register" : reg == 1 ? "MOV to CS" : nullptr;
            case 0x9A:
                if (wide) return "invalid in 64-bit mode";
                if (override16) return "far call with an operand-size prefix";
                far_call_ = true;   // CALLF ptr16:16: the description leaves CS as it is
                far_segment_ = static_cast<unsigned>(bytes >> (8 * (i + 3))) & 0xFFFF;
                far_offset_ = static_cast<unsigned>(bytes >> (8 * (i + 1))) & 0xFFFF;
                return nullptr;
            case 0x06: case 0x07: case 0x0E: case 0x16: case 0x17: case 0x1E: case 0x1F: case 0x60: case 0x61: case 0x82:
            case 0xD5: case 0xD6: case 0xEA: return wide ? "invalid in 64-bit mode" : op == 0xD5 ? "AAM and AAD (the flags the machine chooses)" : nullptr;
            case 0xD4: return wide ? "invalid in 64-bit mode" : next == 0 ? "division fault" : "AAM and AAD (the flags the machine chooses)";
            case 0xF1: return "INT1";
            case 0x99:
                if (wide && (rex & 8) != 0) cqo_ = true;   // CQO: the description sign-extends through a 128-bit value, which the emulator cannot hold: RDX is set after it
                return nullptr;
            case 0xC8: return override16 || ((bytes >> (8 * (i + 3))) & 0xFF) != 0 ? "ENTER with a 16-bit frame or nesting" : nullptr;
            case 0x9B: return "FWAIT";
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
            case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD: case 0xDE: case 0xDF: return "x87";   // (the description's float is the host's double: 80-bit precision and the status word are Unicorn's)
            case 0xE4: case 0xE5: case 0xE6: case 0xE7: case 0xEC: case 0xED: case 0xEE: case 0xEF: case 0x6C: case 0x6D: case 0x6E: case 0x6F:
                return wide ? "IN/OUT" : nullptr;
            case 0x8F: case 0xC6: case 0xC7: return reg != 0 ? "undefined opcode extension" : nullptr;
            case 0xFE: return reg > 1 ? "undefined opcode extension" : nullptr;
            case 0xFF: return reg == 7 ? "undefined opcode extension" : reg == 3 || reg == 5 ? "far transfer" : nullptr;
            case 0xF6: case 0xF7:
                if (reg == 1) return "undefined opcode extension";
                if (!wide && (reg == 4 || reg == 5)) return "MUL and IMUL (SF, ZF, PF, AF are whatever the machine chooses)";
                if (wide && (reg == 4 || reg == 5) && (rex & 8) != 0) return "64-bit MUL and IMUL (a 128-bit product: the description's arithmetic is a word wide)";
                if (reg >= 6) {
                    const unsigned size = op == 0xF6 ? 1u : bits_ == 16 ? (override16 ? 4u : 2u) : (rex & 8) != 0 ? 8u : override16 ? 2u : 4u;
                    division_ = {true, size, reg == 7, next >> 6, next & 7, (rex & 1) != 0, rex != 0};
                    if (size == 8) division_.on = true;
                }
                return nullptr;
            case 0x0F: {
                const unsigned third = static_cast<unsigned>(bytes >> (8 * (i + 2))) & 0xFF;
                const unsigned mandatory = prefix_present(bytes, i, 0xF3) ? 2 : prefix_present(bytes, i, 0xF2) ? 3 : override16 ? 1 : 0;   // none, 66, F3, F2
                switch (next) {
                    case 0x00: case 0x01: case 0x02: case 0x03: case 0x06: case 0x07: case 0x08: case 0x09: case 0x0B: case 0xFF: case 0xB9:
                    case 0x20: case 0x21: case 0x22: case 0x23: case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x37:
                    case 0xA2: case 0x78: case 0x79: case 0x0E: return "system instruction";
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
                    case 0xAF: return wide && (rex & 8) != 0 ? "64-bit IMUL (a 128-bit product)" : nullptr;
                    case 0x1A: case 0x1B: return "MPX instruction or hint NOP";   // (the description stores bounds registers; Unicorn does nothing)
                    case 0xC7: return "RDRAND and the 8- and 16-byte compare-exchanges";
                    case 0xA0: case 0xA1: case 0xA8: case 0xA9: return "PUSH or POP of FS or GS";
                    case 0xBC: case 0xBD:   // BSF, BSR (TZCNT, LZCNT with F3): a zero source leaves the destination undefined, as the processors and Unicorn each leave it
                        if (override16) return "16-bit double shift or bit scan";
                        if (mandatory != 2) scan_ = {true, (rex & 8) != 0 ? 8u : 4u, false, third >> 6, third & 7, (rex & 1) != 0, rex != 0};
                        return nullptr;
                    case 0xA4: case 0xA5: case 0xAC: case 0xAD: return override16 ? "16-bit double shift or bit scan" : nullptr;
                    case 0x77: return mandatory != 0 ? "undefined prefix" : "MMX";
                    case 0x2C: case 0x2D: return mandatory >= 2 ? "conversion to an integer" : nullptr;
                    default:
                        if (next >= 0xC8 && next <= 0xCF && override16) return "BSWAP of 16 bits";
                        if (next == 0xC5 && mandatory <= 1) return "PEXTRW";
                        if (mandatory == 0 && ((next >= 0x60 && next <= 0x7F) || (next >= 0xD0 && next <= 0xFF) || next == 0xC4)) return "MMX";   // (the 64-bit registers share the x87 stack)
                        if (wide_vector(mandatory, next)) return "128-bit vector instruction";
                        return nullptr;
                }
            }
            default: return nullptr;
        }
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
            if (a.size == 1) return n < 4 ? before.gpr[hardware_to_index(n)] & 0xFF : (before.gpr[hardware_to_index(n - 4)] >> 8) & 0xFF;
            return before.gpr[hardware_to_index(n)] & 0xFFFF;
        };
        u64 x = a.a, y = a.b;
        if ((a.kind == K::Cmps || a.kind == K::Scas) && a.rep && (before.gpr[hardware_to_index(1)] & 0xFFFF) == 0) return (before.flags[0] >> 32) & 1;   // (nothing was compared)
        if (a.kind == K::Cmps || a.kind == K::Scas) {
            const u64 si = before.gpr[hardware_to_index(6)] & 0xFFFF, di = before.gpr[hardware_to_index(7)] & 0xFFFF;
            const u64 es = before.seg[2], source = before.seg[a.segment];
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
        const u64 carry = a.kind == K::Adc || a.kind == K::Sbb ? before.flags[0] & 1 : 0;
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
            if (d.bytes == 1 && !d.has_rex && n >= 4 && n < 8) value = (before.gpr[hardware_to_index(n - 4)] >> 8) & 0xFF;   // AH, CH, DH, BH
            else value = before.gpr[hardware_to_index(n)];
        } else {
            value = emu_.readValue(last_read_address_, d.bytes);
        }
        return d.bytes >= 8 ? value : value & ((u64{1} << (8 * d.bytes)) - 1);
    }
    bool source_is_zero(const Division& d, const Snapshot& before) const { return operand_value(d, before) == 0; }
    // After a DIV or IDIV the description has run: whether the processor would have faulted (a zero divisor the description fails on itself; a quotient too big it does not know).
    // A dividend of two words at the largest width is Unicorn's: the description's arithmetic is a word wide.
    bool division_faults(const Snapshot& before) {
        const Division d = division_;
        division_.on = false;
        u64 divisor = operand_value(d, before);
        if (divisor == 0) return true;
        const unsigned bits = d.bytes * 8;
        const u64 mask = bits == 64 ? ~u64{0} : ((u64{1} << bits) - 1);
        const u64 rax = before.gpr[0], rdx = before.gpr[3];
        u64 dividend = 0;   // the dividend, as a word (for 64 bits it must be one: the high half nothing, or the sign of the low)
        switch (d.bytes) {
            case 1: dividend = rax & 0xFFFF; break;
            case 2: dividend = (rax & 0xFFFF) | ((rdx & 0xFFFF) << 16); break;
            case 4: dividend = (rax & 0xFFFFFFFFull) | ((rdx & 0xFFFFFFFFull) << 32); break;
            default:
                if (d.is_signed ? rdx != (((rax >> 63) & 1) != 0 ? ~u64{0} : u64{0}) : rdx != 0) return true;
                dividend = rax;
                break;
        }
        const unsigned width = d.bytes == 8 ? 64 : bits * 2;   // the dividend's bits
        bool negative_dividend = false, negative_divisor = false;
        if (d.is_signed) {   // (the magnitudes of both)
            negative_divisor = ((divisor >> (bits - 1)) & 1) != 0;
            negative_dividend = ((dividend >> (width - 1)) & 1) != 0;
            if (negative_divisor) divisor = (~divisor + 1) & mask;
            if (negative_dividend) dividend = (~dividend + 1) & (width == 64 ? ~u64{0} : ((u64{1} << width) - 1));
        }
        const u64 quotient = dividend / divisor;
        if (!d.is_signed) return quotient > mask;
        const u64 limit = u64{1} << (bits - 1);   // (a negative quotient may be -limit)
        return negative_dividend != negative_divisor ? quotient > limit : quotient >= limit;
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
    std::unordered_set<u64> touched_;   // the pages written so far: what a Unicorn engine is handed
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
    if (enabled_when_asked() && (bits == 16 || bits == 64)) return open_x86_emulator(bits);
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

void write_xmm(core16::Uc& uc, unsigned index, ByteView bytes) {
    if (auto* mine = dynamic_cast<PcodeUc*>(&uc)) return mine->write_xmm(index, bytes);
    write_unicorn_xmm(uc, index, bytes);
}

X86Stats& x86_stats() { return g_stats; }

}  // namespace cb::engine::pcode_cpu
