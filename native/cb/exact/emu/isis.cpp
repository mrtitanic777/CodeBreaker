#include "cb/exact/emu/isis.hpp"

#include <algorithm>
#include <bit>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::emu::isis {

namespace {

namespace stdfs = std::filesystem;

constexpr std::uint16_t kIsisEntry = 0x40;       // CALL 40h is an ISIS call
constexpr std::uint16_t kMonitorBase = 0xF800;   // the monitor's entries are from here up
constexpr std::uint16_t kMonitorCi = 0xF803;
constexpr std::uint16_t kMonitorCo = 0xF809;
constexpr std::uint16_t kMonitorMemck = 0xF81B;
constexpr std::uint16_t kInitialSp = 0xF6C0;
constexpr std::uint16_t kTopOfMemory = 0xF6C0;   // MEMCK says so (ASM86 wants at least this much)
constexpr int kMaxOpenFile = 98;                 // file numbers handed out: 2..98 (0 and 1 are the console)
constexpr std::size_t kBlock = 128;              // ISIS seeks in blocks of 128 bytes

// The statuses an ISIS call reports.
constexpr std::uint16_t kStatusOk = 0;
constexpr std::uint16_t kStatusBadFile = 2;      // no such open file
constexpr std::uint16_t kStatusNotFound = 13;    // no such file on the drive

// Object file records (OMF-80): the content record loads bytes, the entry record names the start.
constexpr std::uint8_t kRecordContent = 0x06;
constexpr std::uint8_t kRecordEntry = 0x04;
constexpr std::uint8_t kRecordEnd = 0x0E;

// The 8080's parity flag is set when the result has an even number of ones.
int bit(bool b) { return b ? 1 : 0; }

bool even_parity(std::uint8_t v) { return (std::popcount(v) & 1) == 0; }

bool is_file(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_regular_file(p, ec);
}

enum class Call : std::uint8_t {
    Open = 0, Close, Delete, Read, Write, Seek, Load, Rename, Attrib, Exit, Consol, Rescan, Error, Whocon, Spath
};

}  // namespace

Machine::Machine(Drives drives, std::string_view tail, Options options) : drives_(std::move(drives)), options_(std::move(options)) {
    if (!drives_.contains(0)) throw UsageError("ISIS: drive 0 is required");
    cpu_.sp = kInitialSp;
    console_input_ = to_bytes("PROG");
    append(console_input_, text::to_latin1(tail));
    append(console_input_, as_bytes("\r\n"));
}

std::uint32_t Machine::load(const stdfs::path& object, std::uint32_t bias) {
    const Bytes file = fs::read_file(object);
    std::uint32_t entry = 0;
    std::size_t p = 0;
    while (p + 3 <= file.size()) {
        const std::uint8_t type = file[p];
        const std::size_t length = file[p + 1] | (file[p + 2] << 8);   // the record's length counts its checksum byte
        if (p + 3 + length > file.size()) throw TruncatedError("ISIS object", p, 3 + length, file.size() - p);
        const ByteView body(file.data() + p + 3, length == 0 ? 0 : length - 1);
        p += 3 + length;
        if (type == kRecordContent) {
            if (body.size() < 3) throw FormatError("ISIS object", p, "content record without an address");
            const std::uint32_t address = static_cast<std::uint32_t>(body[1] | (body[2] << 8)) + bias;
            // what does not fit in the 64 KiB is cut off
            for (std::size_t i = 3; i < body.size() && address + (i - 3) < memory_.size(); ++i) memory_[address + (i - 3)] = body[i];
        } else if (type == kRecordEntry) {
            if (body.size() < 4) throw FormatError("ISIS object", p, "entry record without an address");
            entry = static_cast<std::uint32_t>(body[2] | (body[3] << 8)) + bias;
        } else if (type == kRecordEnd) {
            break;
        }
    }
    return entry;
}

void Machine::start(const stdfs::path& object) { cpu_.pc = static_cast<std::uint16_t>(load(object)); }

// ---- the 8080 -----------------------------------------------------------------------------------------

namespace {

// The 8080 on a 64 KiB memory. Every address wraps at 16 bits.
class Core {
  public:
    Core(Cpu& cpu, std::array<std::uint8_t, 65536>& memory) : cpu_(cpu), m_(memory) {}

    std::uint8_t byte_at(std::uint32_t a) const { return m_[a & 0xFFFF]; }
    std::uint16_t word_at(std::uint32_t a) const { return static_cast<std::uint16_t>(byte_at(a) | (byte_at(a + 1) << 8)); }
    void set_byte(std::uint32_t a, std::uint8_t v) { m_[a & 0xFFFF] = v; }
    void set_word(std::uint32_t a, std::uint32_t v) {
        set_byte(a, static_cast<std::uint8_t>(v));
        set_byte(a + 1, static_cast<std::uint8_t>(v >> 8));
    }

    void push(std::uint16_t v) {
        cpu_.sp = static_cast<std::uint16_t>(cpu_.sp - 2);
        set_word(cpu_.sp, v);
    }
    std::uint16_t pop() {
        const std::uint16_t v = word_at(cpu_.sp);
        cpu_.sp = static_cast<std::uint16_t>(cpu_.sp + 2);
        return v;
    }

    // Run the instruction `op` (its first byte is read and the program counter is already past it). False for HLT.
    bool execute(std::uint8_t op) {
        const int hi = op >> 6;
        const int d = (op >> 3) & 7;   // the destination register / condition / operation
        const int sr = op & 7;         // the source register / sub-opcode
        Cpu& c = cpu_;
        if (op == 0x76) return false;   // HLT
        if (hi == 1) {
            put(d, get(sr));   // MOV
            return true;
        }
        if (hi == 2) {
            alu(d, get(sr));
            return true;
        }
        if (hi == 0) {
            const int k = d >> 1;   // register pair
            switch (sr) {
                case 0: break;      // NOP (and its undocumented duplicates)
                case 1:
                    if ((d & 1) == 0) {
                        set_pair(k, imm16());   // LXI
                    } else {
                        const std::uint32_t t = hl() + pair(k);   // DAD
                        c.carry = (t >> 16) != 0;
                        c.h = static_cast<std::uint8_t>(t >> 8);
                        c.l = static_cast<std::uint8_t>(t);
                    }
                    break;
                case 2:
                    switch (d) {
                        case 0: set_byte(bc(), c.a); break;                 // STAX B
                        case 1: c.a = byte_at(bc()); break;                 // LDAX B
                        case 2: set_byte(de(), c.a); break;                 // STAX D
                        case 3: c.a = byte_at(de()); break;                 // LDAX D
                        case 4: set_word(imm16(), hl()); break;             // SHLD
                        case 5: set_pair(2, word_at(imm16())); break;       // LHLD
                        case 6: set_byte(imm16(), c.a); break;              // STA
                        default: c.a = byte_at(imm16()); break;             // LDA
                    }
                    break;
                case 3: set_pair(k, static_cast<std::uint16_t>(pair(k) + ((d & 1) == 0 ? 1 : -1))); break;   // INX / DCX
                case 4: {                                                                                    // INR
                    const std::uint8_t v = static_cast<std::uint8_t>(get(d) + 1);
                    put(d, v);
                    c.aux_carry = (v & 15) == 0;
                    szp(v);
                    break;
                }
                case 5: {   // DCR
                    const std::uint8_t v = static_cast<std::uint8_t>(get(d) - 1);
                    put(d, v);
                    c.aux_carry = (v & 15) != 15;
                    szp(v);
                    break;
                }
                case 6: put(d, imm8()); break;   // MVI
                default: accumulator_operation(d); break;
            }
            return true;
        }
        // the 0xC0 block
        const int k = d >> 1;
        switch (sr) {
            case 0:   // Rcc
                if (condition(d)) c.pc = pop();
                break;
            case 1:
                if ((d & 1) == 0) {   // POP
                    if (k == 3) {
                        const std::uint16_t v = pop();
                        c.a = static_cast<std::uint8_t>(v >> 8);
                        c.sign = (v >> 7) & 1;
                        c.zero = (v >> 6) & 1;
                        c.aux_carry = (v >> 4) & 1;
                        c.parity = (v >> 2) & 1;
                        c.carry = v & 1;
                    } else {
                        set_pair(k, pop());
                    }
                } else if (d == 5) {
                    c.pc = hl();   // PCHL
                } else if (d == 7) {
                    c.sp = hl();   // SPHL
                } else {
                    c.pc = pop();   // RET (and its undocumented duplicate)
                }
                break;
            case 2: {   // Jcc
                const std::uint16_t target = imm16();
                if (condition(d)) c.pc = target;
                break;
            }
            case 3:
                if (d == 0 || d == 1) {
                    c.pc = imm16();   // JMP (and its undocumented duplicate)
                } else if (d == 2 || d == 3) {
                    imm8();   // OUT / IN: the port is read and nothing happens
                } else if (d == 4) {   // XTHL
                    const std::uint16_t top = word_at(c.sp);
                    set_word(c.sp, hl());
                    c.l = static_cast<std::uint8_t>(top);
                    c.h = static_cast<std::uint8_t>(top >> 8);
                } else if (d == 5) {   // XCHG
                    std::swap(c.d, c.h);
                    std::swap(c.e, c.l);
                }   // DI, EI: nothing
                break;
            case 4: {   // Ccc
                const std::uint16_t target = imm16();
                if (condition(d)) {
                    push(c.pc);
                    c.pc = target;
                }
                break;
            }
            case 5:
                if ((d & 1) == 0) {   // PUSH
                    push(k == 3 ? psw() : pair(k));
                } else {   // CALL (and its undocumented duplicates)
                    const std::uint16_t target = imm16();
                    push(c.pc);
                    c.pc = target;
                }
                break;
            case 6: alu(d, imm8()); break;   // ADI ACI SUI SBI ANI XRI ORI CPI
            default:                          // RST
                push(c.pc);
                c.pc = static_cast<std::uint16_t>(d * 8);
                break;
        }
        return true;
    }

    std::uint16_t bc() const { return static_cast<std::uint16_t>((cpu_.b << 8) | cpu_.c); }
    std::uint16_t de() const { return static_cast<std::uint16_t>((cpu_.d << 8) | cpu_.e); }
    std::uint16_t hl() const { return static_cast<std::uint16_t>((cpu_.h << 8) | cpu_.l); }

  private:
    // register number 0..7 = B C D E H L M A (M is the byte HL addresses)
    std::uint8_t get(int i) const {
        switch (i) {
            case 0: return cpu_.b;
            case 1: return cpu_.c;
            case 2: return cpu_.d;
            case 3: return cpu_.e;
            case 4: return cpu_.h;
            case 5: return cpu_.l;
            case 6: return byte_at(hl());
            default: return cpu_.a;
        }
    }
    void put(int i, std::uint8_t v) {
        switch (i) {
            case 0: cpu_.b = v; break;
            case 1: cpu_.c = v; break;
            case 2: cpu_.d = v; break;
            case 3: cpu_.e = v; break;
            case 4: cpu_.h = v; break;
            case 5: cpu_.l = v; break;
            case 6: set_byte(hl(), v); break;
            default: cpu_.a = v; break;
        }
    }
    std::uint16_t pair(int k) const {
        switch (k) {
            case 0: return bc();
            case 1: return de();
            case 2: return hl();
            default: return cpu_.sp;
        }
    }
    void set_pair(int k, std::uint16_t v) {
        const std::uint8_t hi = static_cast<std::uint8_t>(v >> 8);
        const std::uint8_t lo = static_cast<std::uint8_t>(v);
        switch (k) {
            case 0: cpu_.b = hi; cpu_.c = lo; break;
            case 1: cpu_.d = hi; cpu_.e = lo; break;
            case 2: cpu_.h = hi; cpu_.l = lo; break;
            default: cpu_.sp = v; break;
        }
    }

    std::uint8_t imm8() {
        const std::uint8_t v = byte_at(cpu_.pc);
        cpu_.pc = static_cast<std::uint16_t>(cpu_.pc + 1);
        return v;
    }
    std::uint16_t imm16() {
        const std::uint16_t v = word_at(cpu_.pc);
        cpu_.pc = static_cast<std::uint16_t>(cpu_.pc + 2);
        return v;
    }

    void szp(std::uint8_t v) {
        cpu_.zero = v == 0;
        cpu_.sign = (v >> 7) != 0;
        cpu_.parity = even_parity(v);
    }

    std::uint16_t psw() const {
        return static_cast<std::uint16_t>((cpu_.a << 8) | (bit(cpu_.sign) << 7) | (bit(cpu_.zero) << 6) | (bit(cpu_.aux_carry) << 4) | (bit(cpu_.parity) << 2) | 2 | bit(cpu_.carry));
    }

    // the condition of Jcc/Ccc/Rcc: NZ Z NC C PO PE P M
    bool condition(int k) const {
        switch (k) {
            case 0: return !cpu_.zero;
            case 1: return cpu_.zero;
            case 2: return !cpu_.carry;
            case 3: return cpu_.carry;
            case 4: return !cpu_.parity;
            case 5: return cpu_.parity;
            case 6: return !cpu_.sign;
            default: return cpu_.sign;
        }
    }

    // ADD ADC SUB SBB ANA XRA ORA CMP with `v` (a register, memory or an immediate)
    void alu(int operation, std::uint8_t v) {
        Cpu& c = cpu_;
        const int a = c.a;
        switch (operation) {
            case 0:
            case 1: {   // ADD, ADC
                const int carry_in = operation == 1 ? bit(c.carry) : 0;
                const int t = a + v + carry_in;
                c.aux_carry = (((a & 15) + (v & 15) + carry_in) >> 4) != 0;
                c.carry = (t >> 8) != 0;
                c.a = static_cast<std::uint8_t>(t);
                szp(c.a);
                break;
            }
            case 2:
            case 3:
            case 7: {   // SUB, SBB, CMP (CMP leaves the accumulator)
                const int borrow_in = operation == 3 ? bit(c.carry) : 0;
                const int t = a - v - borrow_in;
                c.aux_carry = ((a & 15) - (v & 15) - borrow_in) >= 0;
                c.carry = t < 0;
                szp(static_cast<std::uint8_t>(t));
                if (operation != 7) c.a = static_cast<std::uint8_t>(t);
                break;
            }
            case 4: {   // ANA: the auxiliary carry is bit 3 of the OR of the operands
                c.aux_carry = (((a | v) >> 3) & 1) != 0;
                c.a = static_cast<std::uint8_t>(a & v);
                c.carry = false;
                szp(c.a);
                break;
            }
            case 5:
            case 6:   // XRA, ORA
                c.a = static_cast<std::uint8_t>(operation == 5 ? (a ^ v) : (a | v));
                c.carry = false;
                c.aux_carry = false;
                szp(c.a);
                break;
        }
    }

    // RLC RRC RAL RAR DAA CMA STC CMC
    void accumulator_operation(int which) {
        Cpu& c = cpu_;
        const int a = c.a;
        switch (which) {
            case 0:
                c.carry = (a >> 7) != 0;
                c.a = static_cast<std::uint8_t>((a << 1) | bit(c.carry));
                break;
            case 1:
                c.carry = (a & 1) != 0;
                c.a = static_cast<std::uint8_t>((a >> 1) | (bit(c.carry) << 7));
                break;
            case 2: {
                const bool carry_in = c.carry;
                c.carry = (a >> 7) != 0;
                c.a = static_cast<std::uint8_t>((a << 1) | bit(carry_in));
                break;
            }
            case 3: {
                const bool carry_in = c.carry;
                c.carry = (a & 1) != 0;
                c.a = static_cast<std::uint8_t>((a >> 1) | (bit(carry_in) << 7));
                break;
            }
            case 4: {   // DAA
                bool carry = c.carry;
                int adjust = 0;
                if ((a & 15) > 9 || c.aux_carry) adjust = 6;
                if (a > 0x99 || carry) {
                    adjust |= 0x60;
                    carry = true;
                }
                c.aux_carry = (a & 15) + (adjust & 15) > 15;
                c.a = static_cast<std::uint8_t>(a + adjust);
                szp(c.a);
                c.carry = carry;
                break;
            }
            case 5: c.a = static_cast<std::uint8_t>(~a); break;   // CMA
            case 6: c.carry = true; break;                        // STC
            default: c.carry = !c.carry; break;                   // CMC
        }
    }

    Cpu& cpu_;
    std::array<std::uint8_t, 65536>& m_;
};

}  // namespace

void Machine::step() {
    Core core(cpu_, memory_);
    const std::uint8_t op = memory_[cpu_.pc];
    cpu_.pc = static_cast<std::uint16_t>(cpu_.pc + 1);
    if (!core.execute(op)) finished_ = true;   // HLT
}

void Machine::monitor_entry() {
    Core core(cpu_, memory_);
    switch (cpu_.pc) {
        case kMonitorMemck:   // top of memory in B:A
            cpu_.a = static_cast<std::uint8_t>(kTopOfMemory);
            cpu_.b = static_cast<std::uint8_t>(kTopOfMemory >> 8);
            break;
        case kMonitorCo: console_.push_back(cpu_.c); break;   // CO
        case kMonitorCi: cpu_.a = 0x0D; break;                // CI: a carriage return
        default: break;
    }
    cpu_.pc = core.pop();
}

std::uint64_t Machine::run(std::uint64_t limit) {
    std::uint64_t n = 0;
    finished_ = false;
    while (n < limit) {
        const std::uint16_t pc = cpu_.pc;
        if (pc == kIsisEntry) {
            Core core(cpu_, memory_);
            if (!isis_call() && !finished_) cpu_.pc = core.pop();   // (back to the caller, unless the call went to a program)
        } else if (pc >= kMonitorBase) {
            monitor_entry();
        } else {
            step();
        }
        if (finished_) break;   // the instruction that ends the program is not counted
        ++n;
    }
    return n;
}

// ---- ISIS ---------------------------------------------------------------------------------------------

namespace {

// Where a name goes: a file of a drive's host directory, or a device.
struct Resolved {
    std::optional<stdfs::path> host;
    std::optional<std::string> device;
};

std::uint8_t device_type(std::string_view device) {
    if (device == "CO") return 0x0D;
    if (device == "CI") return 0x0C;
    if (device == "LP") return 0x15;
    if (device == "TO") return 0x09;
    if (device == "TI") return 0x08;
    return 0x16;   // BB and anything else
}

bool is_name_byte(std::uint8_t b) {
    return (b >= 0x30 && b <= 0x39) || (b >= 0x41 && b <= 0x5A) || (b >= 0x61 && b <= 0x7A) || b == 0x3A || b == 0x2E;
}

}  // namespace

bool Machine::isis_call() {
    Core core(cpu_, memory_);
    const int function = cpu_.c;
    const std::uint16_t block = static_cast<std::uint16_t>((cpu_.d << 8) | cpu_.e);   // the parameter block
    // parameter k of the block: a word
    const auto param = [&](int k) { return core.word_at(block + 2 * k); };
    // the status a call reports goes to the address in parameter k
    const auto status = [&](int k, std::uint16_t v) { core.set_word(param(k), v); };
    // a name in memory: after any blanks, as long as it holds letters, digits, ':' and '.'
    const auto name_at = [&](std::uint16_t address) {
        std::uint32_t a = address;
        while (a < 0x10000 && memory_[a] == 0x20) ++a;
        std::uint32_t e = a;
        while (e < 0x10000 && is_name_byte(memory_[e])) ++e;
        return text::from_latin1(ByteView(memory_.data() + std::min<std::uint32_t>(a, 0x10000), e - std::min<std::uint32_t>(a, 0x10000)));
    };
    const auto resolve = [&](const std::string& written) {
        const std::string name = text::upper(written);
        Resolved r;
        if (name.size() >= 4 && name[0] == ':' && name[3] == ':') {
            const std::string device = name.substr(1, 2);
            if (device[0] == 'F' && device[1] >= '0' && device[1] <= '9') {
                const auto drive = drives_.find(device[1] - '0');
                r.host = (drive != drives_.end() ? drive->second : drives_.at(0)) / name.substr(4);
            } else {
                r.device = device;
            }
            return r;
        }
        r.host = drives_.at(0) / name;
        return r;
    };
    // store bytes at an address, as much as fits in the memory
    const auto poke = [&](std::uint32_t address, ByteView bytes) {
        for (std::size_t i = 0; i < bytes.size() && address + i < memory_.size(); ++i) memory_[address + i] = bytes[i];
    };
    const auto next_file_number = [&]() {
        for (int n = 2; n <= kMaxOpenFile; ++n) {
            if (!files_.contains(n)) return n;
        }
        throw EmulationError(std::format("ISIS: more than {} files open", kMaxOpenFile - 1));
    };
    const auto write_back = [&](const OpenFile& f) {
        if (f.dirty && f.host) fs::write_file(*f.host, f.data);
    };

    switch (static_cast<Call>(function)) {
        case Call::Open: {   // OPEN(aftn^, name, access, echo, status^)
            const Resolved r = resolve(name_at(param(1)));
            const std::uint16_t access = param(2);   // 1 read, 2 write, 3 update
            if (r.device) {
                int number = 0;
                if (*r.device == "CO") {
                    number = 0;
                } else if (*r.device == "CI") {
                    number = 1;
                } else {
                    number = next_file_number();
                    files_[number] = OpenFile{std::nullopt, 0, r.device, {}, false};
                }
                core.set_word(param(0), static_cast<std::uint16_t>(number));
                status(4, kStatusOk);
                break;
            }
            const bool exists = is_file(*r.host);
            if (access == 1 && !exists) {
                status(4, kStatusNotFound);
                break;
            }
            OpenFile f{r.host, 0, std::nullopt, {}, access != 1};
            if (access != 2 && exists) f.data = fs::read_file(*r.host);
            const int number = next_file_number();
            files_[number] = std::move(f);
            core.set_word(param(0), static_cast<std::uint16_t>(number));
            status(4, kStatusOk);
            break;
        }
        case Call::Close: {   // CLOSE(aftn, status^)
            if (const auto it = files_.find(param(0)); it != files_.end()) {
                write_back(it->second);
                files_.erase(it);
            }
            status(1, kStatusOk);
            break;
        }
        case Call::Delete: {   // DELETE(name, status^)
            const Resolved r = resolve(name_at(param(0)));
            std::error_code ec;
            if (r.host && is_file(*r.host) && stdfs::remove(*r.host, ec)) status(1, kStatusOk);
            else status(1, kStatusNotFound);
            break;
        }
        case Call::Read: {   // READ(aftn, buf, count, actual^, status^)
            const std::uint16_t aftn = param(0);
            const std::uint16_t buffer = param(1);
            const std::size_t count = param(2);
            Bytes chunk;
            if (aftn == 1) {   // the console: a line at a time
                std::size_t end = console_input_.size();
                for (std::size_t i = input_position_; i < console_input_.size(); ++i) {
                    if (console_input_[i] == '\n') {
                        end = i + 1;
                        break;
                    }
                }
                const std::size_t from = std::min(input_position_, console_input_.size());
                const std::size_t to = std::min(end, input_position_ + count);
                if (to > from) chunk.assign(console_input_.begin() + static_cast<std::ptrdiff_t>(from), console_input_.begin() + static_cast<std::ptrdiff_t>(to));
                input_position_ += chunk.size();
            } else {
                const auto it = files_.find(aftn);
                if (it == files_.end()) {
                    status(4, kStatusBadFile);
                    break;
                }
                OpenFile& f = it->second;
                if (!f.device) {
                    const std::size_t from = std::min<std::size_t>(f.position, f.data.size());
                    const std::size_t to = std::min<std::size_t>(f.position + count, f.data.size());
                    chunk.assign(f.data.begin() + static_cast<std::ptrdiff_t>(from), f.data.begin() + static_cast<std::ptrdiff_t>(to));
                }
                f.position += chunk.size();
            }
            poke(buffer, chunk);
            core.set_word(param(3), static_cast<std::uint16_t>(chunk.size()));
            status(4, kStatusOk);
            break;
        }
        case Call::Write: {   // WRITE(aftn, buf, count, status^)
            const std::uint16_t aftn = param(0);
            const std::uint32_t buffer = param(1);
            const std::size_t count = param(2);
            const std::size_t available = buffer < memory_.size() ? std::min<std::size_t>(count, memory_.size() - buffer) : 0;
            const ByteView chunk(memory_.data() + std::min<std::size_t>(buffer, memory_.size()), available);
            if (aftn == 0) {
                append(console_, chunk);
                if (options_.echo) options_.echo(text::from_latin1(chunk));
            } else {
                const auto it = files_.find(aftn);
                if (it == files_.end()) {
                    status(3, kStatusBadFile);
                    break;
                }
                OpenFile& f = it->second;
                if (!f.device) {
                    if (f.position > f.data.size()) f.data.resize(f.position);   // a gap reads as zeros
                    if (f.position + chunk.size() > f.data.size()) f.data.resize(f.position + chunk.size());
                    std::copy(chunk.begin(), chunk.end(), f.data.begin() + static_cast<std::ptrdiff_t>(f.position));
                    f.position += count;
                    f.dirty = true;
                }
            }
            status(3, kStatusOk);
            break;
        }
        case Call::Seek: {   // SEEK(aftn, mode, block^, byte^, status^)
            const auto it = files_.find(param(0));
            if (it == files_.end()) {
                status(4, kStatusBadFile);
                break;
            }
            OpenFile& f = it->second;
            const std::uint16_t mode = param(1);
            const std::size_t offset = static_cast<std::size_t>(core.word_at(param(2))) * kBlock + core.word_at(param(3));
            switch (mode) {
                case 0:   // where it is
                    core.set_word(param(2), static_cast<std::uint16_t>(f.position / kBlock));
                    core.set_word(param(3), static_cast<std::uint16_t>(f.position % kBlock));
                    break;
                case 1: f.position = f.position > offset ? f.position - offset : 0; break;   // back
                case 2: f.position = offset; break;                                            // to
                case 3: f.position += offset; break;                                           // forward
                case 4: f.position = f.data.size(); break;                                     // to the end
                default: break;
            }
            if (f.position > f.data.size() && f.dirty) f.data.resize(f.position);
            status(4, kStatusOk);
            break;
        }
        case Call::Load: {   // LOAD(name, bias, retsw, entry^, status^)
            const Resolved r = resolve(name_at(param(0)));
            if (!r.host || !is_file(*r.host)) {
                status(4, kStatusNotFound);
                break;
            }
            // (the parameter block may lie where the program loads: read it first)
            const std::uint16_t bias = param(1);
            const std::uint16_t return_switch = param(2);
            const std::uint16_t entry_address = param(3);
            const std::uint16_t status_address = param(4);
            core.set_word(status_address, kStatusOk);
            const std::uint32_t entry = load(*r.host, bias);
            if (return_switch != 0) {   // 1: control goes to the program loaded; 0: back to the caller
                cpu_.pc = static_cast<std::uint16_t>(entry);
                return true;
            }
            core.set_word(entry_address, entry);
            break;
        }
        case Call::Rename: {   // RENAME(old, new, status^)
            const Resolved from = resolve(name_at(param(0)));
            const Resolved to = resolve(name_at(param(1)));
            std::error_code ec;
            if (from.host && to.host && is_file(*from.host)) stdfs::rename(*from.host, *to.host, ec);
            else ec = std::make_error_code(std::errc::no_such_file_or_directory);
            status(2, ec ? kStatusNotFound : kStatusOk);
            break;
        }
        case Call::Exit:   // EXIT: what is still open is written back first
            for (const auto& [number, f] : files_) write_back(f);
            finished_ = true;
            break;
        case Call::Rescan:   // RESCAN: the command line is read again from the program's name
            input_position_ = 0;
            status(1, kStatusOk);
            break;
        case Call::Error: {   // ERROR(errnum, status^)
            const std::string line = std::format("ERROR {} USER PC {:04X}\r\n", param(0) & 255, core.word_at(cpu_.sp));
            append(console_, as_bytes(line));
            status(1, kStatusOk);
            break;
        }
        case Call::Spath: {   // SPATH(name, info^, status^)
            const std::string name = text::upper(name_at(param(0)));
            const Resolved r = resolve(name);
            const std::uint16_t info = param(1);
            Bytes entry(12, 0);
            if (r.device) {
                entry[0] = device_type(*r.device);
            } else {
                const std::string base = r.host->filename().string();
                const auto dot = base.find('.');
                const std::string stem = base.substr(0, dot);
                const std::string extension = dot == std::string::npos ? "" : base.substr(dot + 1);
                entry[0] = static_cast<std::uint8_t>(name.starts_with(":F") && name.size() > 2 ? name[2] - '0' : 0);   // the drive
                for (std::size_t i = 0; i < 6 && i < stem.size(); ++i) entry[1 + i] = static_cast<std::uint8_t>(stem[i]);
                for (std::size_t i = 0; i < 3 && i < extension.size(); ++i) entry[7 + i] = static_cast<std::uint8_t>(extension[i]);
                entry[10] = 3;
                entry[11] = 0;
            }
            poke(info, entry);
            status(2, kStatusOk);
            break;
        }
        case Call::Attrib: status(2, kStatusOk); break;   // ATTRIB(name, attrib, status^): accepted, nothing kept
        case Call::Consol: status(3, kStatusOk); break;   // CONSOL(...): the console is the console
        case Call::Whocon: status(1, kStatusOk); break;   // WHOCON(...): likewise
        default: throw EmulationError(std::format("ISIS call {} is not provided", function));
    }
    return false;
}

Result run(const stdfs::path& program, std::string_view tail, const Drives& drives, const Options& options) {
    Machine machine(drives, tail, options);
    machine.start(program);
    Result r;
    r.steps = machine.run(options.limit);
    r.finished = machine.finished();
    r.console = text::from_latin1(machine.console());
    return r;
}

}  // namespace cb::exact::emu::isis
