// codebreaker/emulate/m68k.cpp -- typed C++ port of codebreaker.emulate.m68k. See m68k.hpp.

#include "codebreaker/emulate/m68k.hpp"

namespace codebreaker::emulate::m68k {

namespace {

const py::str B("B"), W("W"), L("L");

[[noreturn]] void bad_size(const py::str& s) { throw py::KeyError(py::repr(s).utf8()); }

// MASK[size], SIGN[size], BYTES[size]
int64_t mask_of(const py::str& s) {
    if (s == "B") return 0xFF;
    if (s == "W") return 0xFFFF;
    if (s == "L") return 0xFFFFFFFF;
    bad_size(s);
}

int64_t sign_of(const py::str& s) {
    if (s == "B") return 0x80;
    if (s == "W") return 0x8000;
    if (s == "L") return 0x80000000;
    bad_size(s);
}

int64_t bytes_of(const py::str& s) {
    if (s == "B") return 1;
    if (s == "W") return 2;
    if (s == "L") return 4;
    bad_size(s);
}

// an address that is None used as a number
int64_t need(const std::optional<int64_t>& a) {
    if (!a) throw py::TypeError("unsupported operand type(s) for &=: 'NoneType' and 'int'");
    return *a;
}

int64_t target_of(const Insn& insn) {
    if (!insn.target) throw py::TypeError("instruction has no target");
    return *insn.target;
}

// `a, b = ops`
std::tuple<const Op&, const Op&> unpack2(const py::list<Op>& ops) {
    int64_t n = ops.size();
    if (n < 2) throw py::ValueError("not enough values to unpack (expected 2, got " + std::to_string(n) + ")");
    if (n > 2) throw py::ValueError("too many values to unpack (expected 2)");
    return std::tie(ops.vec()[0], ops.vec()[1]);
}

// Python's % for a positive modulus
int64_t floormod(int64_t a, int64_t m) {
    int64_t r = a % m;
    return r < 0 ? r + m : r;
}

// Python's // (b != 0)
int64_t floordiv(int64_t a, int64_t b) {
    int64_t q = a / b;
    return ((a % b != 0) && ((a < 0) != (b < 0))) ? q - 1 : q;
}

// 1 << n (n >= 0; Python's ints are unbounded: a bit past the 63rd is 0 here)
int64_t bit_of(int64_t n) { return n >= 63 ? 0 : (int64_t(1) << n); }

py::list<int64_t> zeros8() {   // [0] * 8
    py::list<int64_t> out;
    for (int i = 0; i < 8; ++i) out.append(0);
    return out;
}

py::dict<py::str, int64_t> make_dict(int64_t b, int64_t w, int64_t l) {
    py::dict<py::str, int64_t> d;
    d.set(py::str("B"), b);
    d.set(py::str("W"), w);
    d.set(py::str("L"), l);
    return d;
}

}  // namespace

const py::dict<py::str, int64_t> MASK = make_dict(0xFF, 0xFFFF, 0xFFFFFFFF);
const py::dict<py::str, int64_t> SIGN = make_dict(0x80, 0x8000, 0x80000000);
const py::dict<py::str, int64_t> BYTES = make_dict(1, 2, 4);

// ---- Bus

Bus::Bus(const std::optional<py::bytearray>& memory, int64_t size) : mem(memory ? *memory : py::bytearray::zeros(size)) {}

void Bus::map(int64_t lo, int64_t hi, const IoRead& read_, const IoWrite& write_) { io.append(IoRange(lo, hi, read_, write_)); }

std::optional<std::tuple<IoRead, IoWrite>> Bus::_io(int64_t a) {
    for (const IoRange& r : io) {
        if (std::get<0>(r) <= a && a <= std::get<1>(r)) return std::tuple<IoRead, IoWrite>(std::get<2>(r), std::get<3>(r));
    }
    return std::nullopt;
}

int64_t Bus::read(int64_t a, int64_t n) {
    a &= 0xFFFFFF;
    std::optional<std::tuple<IoRead, IoWrite>> hit = !io.empty() ? _io(a) : std::nullopt;
    if (hit) {
        const IoRead& rd_ = std::get<0>(*hit);
        int64_t v = rd_ ? rd_(a, n) : 0;
        log.append(IoAccess(py::percent(py::str("r%d"), n), a, v));
        return v;
    }
    if (a + n > mem.size()) return 0;
    const std::string& raw = mem.raw();
    int64_t v = 0;
    for (int64_t i = 0; i < n; ++i) v = (v << 8) | static_cast<unsigned char>(raw[static_cast<size_t>(a + i)]);
    return v;
}

void Bus::write(int64_t a, int64_t n, int64_t v) {
    a &= 0xFFFFFF;
    std::optional<std::tuple<IoRead, IoWrite>> hit = !io.empty() ? _io(a) : std::nullopt;
    if (hit) {
        log.append(IoAccess(py::percent(py::str("w%d"), n), a, v));
        const IoWrite& wr_ = std::get<1>(*hit);
        if (wr_) wr_(a, n, v);
        return;
    }
    if (a + n <= mem.size()) {
        std::string& raw = mem.raw_mut();
        for (int64_t i = 0; i < n; ++i) raw[static_cast<size_t>(a + i)] = static_cast<char>((v >> (8 * (n - 1 - i))) & 0xFF);
    }
}

int64_t _sx(int64_t v, const py::str& size) {
    v &= mask_of(size);
    return (v & sign_of(size)) ? v - (mask_of(size) + 1) : v;
}

// ---- CPU

CPU::CPU(const py::ref<Bus>& bus_) : bus(bus_), d(zeros8()), a(zeros8()) {}

// ---- flags
int64_t CPU::flag(int64_t f) const { return (sr & f) ? 1 : 0; }

void CPU::setf(std::optional<bool> x, std::optional<bool> n, std::optional<bool> z, std::optional<bool> v, std::optional<bool> c) {
    int64_t s = sr;
    auto apply = [&](int64_t bit, const std::optional<bool>& val) {
        if (val) s = *val ? (s | bit) : (s & ~bit);
    };
    apply(FX, x);
    apply(FN, n);
    apply(FZ, z);
    apply(FV, v);
    apply(FC, c);
    sr = s;
}

void CPU::nz(int64_t r, const py::str& size) { setf(std::nullopt, (r & sign_of(size)) != 0, (r & mask_of(size)) == 0); }

void CPU::logic(int64_t r, const py::str& size) {
    setf(std::nullopt, (r & sign_of(size)) != 0, (r & mask_of(size)) == 0, false, false);
}

bool CPU::cond(const py::str& cc) const {
    bool n = flag(FN) != 0, z = flag(FZ) != 0, v = flag(FV) != 0, c = flag(FC) != 0;
    if (cc == "T") return true;
    if (cc == "F") return false;
    if (cc == "HI") return !c && !z;
    if (cc == "LS") return c || z;
    if (cc == "CC") return !c;
    if (cc == "CS") return c;
    if (cc == "NE") return !z;
    if (cc == "EQ") return z;
    if (cc == "VC") return !v;
    if (cc == "VS") return v;
    if (cc == "PL") return !n;
    if (cc == "MI") return n;
    if (cc == "GE") return n == v;
    if (cc == "LT") return n != v;
    if (cc == "GT") return !z && n == v;
    if (cc == "LE") return z || n != v;
    if (cc == "RA") return true;
    throw py::KeyError(py::repr(cc).utf8());
}

// ---- memory
int64_t CPU::rd(int64_t a_, const py::str& size) { return bus->read(a_, bytes_of(size)); }

void CPU::wr(int64_t a_, const py::str& size, int64_t v) { bus->write(a_, bytes_of(size), v & mask_of(size)); }

void CPU::push(int64_t v, const py::str& size) {
    a[7] = (a[7] - bytes_of(size)) & 0xFFFFFFFF;
    wr(a[7], size, v);
}

int64_t CPU::pop(const py::str& size) {
    int64_t v = rd(a[7], size);
    a[7] = (a[7] + bytes_of(size)) & 0xFFFFFFFF;
    return v;
}

// ---- operands
std::optional<int64_t> CPU::address(const Op& o, const py::str& size) {
    const py::str& k = o.kind;
    int64_t step_ = !(o.reg == 7 && size == "B") ? bytes_of(size) : 2;
    if (k == "ind") return a[o.reg];
    if (k == "post") {
        int64_t at = a[o.reg];
        a[o.reg] = (at + step_) & 0xFFFFFFFF;
        return at;
    }
    if (k == "pre") {
        a[o.reg] = (a[o.reg] - step_) & 0xFFFFFFFF;
        return a[o.reg];
    }
    if (k == "disp") return (a[o.reg] + o.value) & 0xFFFFFFFF;
    if (k == "index" || k == "pcindex") {
        if (!o.xreg) throw py::TypeError("cannot unpack non-iterable NoneType object");
        const py::str& xk = std::get<0>(*o.xreg);
        int64_t xn = std::get<1>(*o.xreg);
        int64_t x = xk == "D" ? d[xn] : a[xn];
        x = o.xsize == "W" ? _sx(x, W) : _sx(x, L);
        int64_t base = k == "index" ? a[o.reg] + o.value : o.value;
        return (base + x) & 0xFFFFFFFF;
    }
    if (k == "absw" || k == "absl" || k == "pcdisp") return o.value & 0xFFFFFFFF;
    return std::nullopt;
}

int64_t CPU::get(const Op& o, const py::str& size, const std::optional<int64_t>& at) {
    const py::str& k = o.kind;
    if (k == "D") return d[o.reg] & mask_of(size);
    if (k == "A") return a[o.reg] & mask_of(size);
    if (k == "imm" || k == "quick") return o.value & mask_of(size);
    if (k == "sr") return sr;
    if (k == "ccr") return sr & 0x1F;
    return rd(need(!at ? address(o, size) : at), size);
}

void CPU::put(const Op& o, const py::str& size, int64_t v, const std::optional<int64_t>& at) {
    const py::str& k = o.kind;
    if (k == "D") {
        int64_t m = mask_of(size);
        d[o.reg] = (d[o.reg] & ~m & 0xFFFFFFFF) | (v & m);
    } else if (k == "A") {
        a[o.reg] = size == "W" ? (_sx(v, size) & 0xFFFFFFFF) : (v & 0xFFFFFFFF);
    } else if (k == "sr") {
        sr = v & 0xA71F;
    } else if (k == "ccr") {
        sr = (sr & 0xFF00) | (v & 0x1F);
    } else {
        wr(need(!at ? address(o, size) : at), size, v);
    }
}

void CPU::modify(const Op& o, const py::str& size, const ModifyFn& fn) {
    std::optional<int64_t> at = (o.kind == "D" || o.kind == "A") ? std::nullopt : address(o, size);
    std::optional<int64_t> r = fn(get(o, size, at));
    if (r) put(o, size, *r, at);
}

// ---- one instruction
void CPU::step() {
    int64_t pc_ = pc;
    if (!_cache.contains(pc_)) {
        std::string code;   // bytes(bus.read(pc + i, 1) for i in range(10))
        for (int64_t i = 0; i < 10; ++i) {
            int64_t v = bus->read(pc_ + i, 1);
            if (v < 0 || v > 255) throw py::ValueError("bytes must be in range(0, 256)");
            code.push_back(static_cast<char>(v));
        }
        std::optional<Insn> decoded = table::decode(py::bytes(code), 0, pc_);
        if (!decoded) throw Stop(py::percent(py::str("no instruction at %06X"), pc_));
        _cache.set(pc_, *decoded);
    }
    const Insn& insn = _cache.at(pc_);
    pc = (pc_ + insn.length) & 0xFFFFFFFF;
    _execute(*this, insn);
    steps += 1;
}

int64_t _add(CPU& cpu, int64_t a, int64_t b, const py::str& size, int64_t x, bool keep_z) {
    int64_t m = mask_of(size);
    int64_t r = (a + b + x) & m;
    int64_t s = sign_of(size);
    bool c = (a + b + x) > m;
    bool v = ((~(a ^ b) & (a ^ r)) & s) != 0;
    bool z = (r == 0) && (keep_z ? cpu.flag(FZ) != 0 : true);
    cpu.setf(c, (r & s) != 0, z, v, c);
    return r;
}

int64_t _sub(CPU& cpu, int64_t a, int64_t b, const py::str& size, int64_t x, bool keep_z, bool set_x) {
    int64_t m = mask_of(size);
    int64_t r = (a - b - x) & m;
    int64_t s = sign_of(size);
    bool c = (b + x) > a;
    bool v = (((a ^ b) & (a ^ r)) & s) != 0;
    bool z = (r == 0) && (keep_z ? cpu.flag(FZ) != 0 : true);
    if (set_x) {
        cpu.setf(c, (r & s) != 0, z, v, c);
    } else {
        cpu.setf(std::nullopt, (r & s) != 0, z, v, c);
    }
    return r;
}

int64_t _bcd_add(CPU& cpu, int64_t a, int64_t b) {
    int64_t x = cpu.flag(FX);
    int64_t lo = (a & 0xF) + (b & 0xF) + x;
    int64_t hi = (a & 0xF0) + (b & 0xF0);
    int64_t r = hi + lo;
    if (lo > 9) r += 6;
    bool c = r > 0x99;
    if (c) r += 0x60;
    r &= 0xFF;
    cpu.setf(c, std::nullopt, (r == 0) && cpu.flag(FZ) != 0, std::nullopt, c);
    return r;
}

int64_t _bcd_sub(CPU& cpu, int64_t a, int64_t b) {
    int64_t x = cpu.flag(FX);
    int64_t lo = (a & 0xF) - (b & 0xF) - x;
    int64_t r = (a & 0xF0) - (b & 0xF0) + lo;
    if (lo < 0) r -= 6;
    bool c = r < 0;
    if (c) r -= 0x60;
    r &= 0xFF;
    cpu.setf(c, std::nullopt, (r == 0) && cpu.flag(FZ) != 0, std::nullopt, c);
    return r;
}

int64_t _shift(CPU& cpu, const py::str& name, int64_t v, int64_t count, const py::str& size) {
    int64_t m = mask_of(size), s = sign_of(size);
    std::string nm = name.utf8();
    std::string kind = nm.empty() ? std::string() : nm.substr(0, nm.size() - 1);   // name[:-1]
    bool left = !nm.empty() && nm.back() == 'L';
    int64_t x = cpu.flag(FX);
    int64_t c = 0, vflag = 0;
    if (count == 0) {
        cpu.setf(std::nullopt, (v & s) != 0, (v & m) == 0, false, kind == "ROX" ? x != 0 : false);
        return v;
    }
    for (int64_t i = 0; i < count; ++i) {
        if (kind == "AS" || kind == "LS") {
            int64_t nv;
            if (left) {
                c = (v & s) ? 1 : 0;
                nv = (v << 1) & m;
                if (kind == "AS" && ((nv ^ v) & s)) vflag = 1;
            } else {
                c = v & 1;
                nv = (v >> 1) | (kind == "AS" ? (v & s) : 0);
            }
            v = nv;
            x = c;
        } else if (kind == "RO") {
            if (left) {
                c = (v & s) ? 1 : 0;
                v = ((v << 1) | c) & m;
            } else {
                c = v & 1;
                v = (v >> 1) | (c ? s : 0);
            }
        } else {   // ROX
            if (left) {
                c = (v & s) ? 1 : 0;
                v = ((v << 1) | x) & m;
            } else {
                c = v & 1;
                v = (v >> 1) | (x ? s : 0);
            }
            x = c;
        }
    }
    if (kind == "RO") {
        cpu.setf(std::nullopt, (v & s) != 0, v == 0, false, c != 0);
    } else {
        cpu.setf(x != 0, (v & s) != 0, v == 0, vflag != 0, c != 0);
    }
    return v;
}

void _execute(CPU& cpu, const Insn& insn) {
    const py::str& mn = insn.mnemonic;
    py::str size = insn.size == "" ? W : insn.size;   // insn.size or "W"
    const py::list<Op>& ops = insn.operands;
    if (_HANDLERS.contains(mn)) {
        _HANDLERS.at(mn)(cpu, insn, size, ops);
        return;
    }
    for (const auto& entry : _PREFIXED) {
        const py::str& prefix = std::get<0>(entry);
        if (mn.startswith(prefix) && !_NOT_PREFIXED.contains(mn)) {
            std::get<1>(entry)(cpu, insn, mn.slice_from(prefix.size()), size, ops);
            return;
        }
    }
    throw Stop(py::percent(py::str("not run: %s"), mn));
}

void _move(CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
    auto [src, dst] = unpack2(ops);
    if (dst.kind == "sr" || dst.kind == "ccr" || dst.kind == "usp" || src.kind == "sr" || src.kind == "usp") {
        if (dst.kind == "usp") {
            cpu.usp = cpu.a[src.reg];
        } else if (src.kind == "usp") {
            cpu.a[dst.reg] = cpu.usp;
        } else {
            cpu.put(dst, W, cpu.get(src, W));
        }
        return;
    }
    int64_t v = cpu.get(src, size);
    cpu.put(dst, size, v);
    cpu.logic(v, size);
}

void _movea(CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
    const Op& dst = ops[1];   // (Python evaluates the arguments left to right: ops[1], then the get)
    int64_t v = cpu.get(ops[0], size);
    cpu.put(dst, size, v);
}

void _moveq(CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
    int64_t v = ops[0].value & 0xFFFFFFFF;
    cpu.d[ops[1].reg] = v;
    cpu.logic(v, L);
}

void _movem(CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
    int64_t n = bytes_of(size);
    if (ops[0].kind == "regs") {   // registers to memory
        int64_t mask = ops[0].value;
        const Op& dst = ops[1];
        std::vector<int64_t> regs;
        for (int64_t i = 0; i < 8; ++i) regs.push_back(cpu.d[i]);
        for (int64_t i = 0; i < 8; ++i) regs.push_back(cpu.a[i]);
        if (dst.kind == "pre") {
            int64_t addr = cpu.a[dst.reg];
            for (int64_t i = 15; i >= 0; --i) {   // the mask is reversed: bit 0 is A7
                if (mask & (int64_t(1) << (15 - i))) {
                    addr -= n;
                    cpu.wr(addr, size, regs[static_cast<size_t>(i)]);
                }
            }
            cpu.a[dst.reg] = addr & 0xFFFFFFFF;
        } else {
            std::optional<int64_t> addr = cpu.address(dst, size);
            for (int64_t i = 0; i < 16; ++i) {
                if (mask & (int64_t(1) << i)) {
                    cpu.wr(need(addr), size, regs[static_cast<size_t>(i)]);
                    addr = need(addr) + n;
                }
            }
        }
        return;
    }
    const Op& src = ops[0];
    int64_t mask = ops[1].value;
    std::optional<int64_t> addr = src.kind == "post" ? std::optional<int64_t>(cpu.a[src.reg]) : cpu.address(src, size);
    for (int64_t i = 0; i < 16; ++i) {
        if (mask & (int64_t(1) << i)) {
            int64_t v = cpu.rd(need(addr), size);
            if (size == "W") v = _sx(v, W) & 0xFFFFFFFF;
            if (i < 8) {
                cpu.d[i] = v;
            } else {
                cpu.a[i - 8] = v;
            }
            addr = need(addr) + n;
        }
    }
    if (src.kind == "post") cpu.a[src.reg] = need(addr) & 0xFFFFFFFF;   // (addr is still the int it began as)
}

void _movep(CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
    int64_t n = bytes_of(size);
    if (ops[0].kind == "disp") {
        std::optional<int64_t> addr = cpu.address(ops[0], size);
        int64_t v = 0;
        for (int64_t i = 0; i < n; ++i) v = (v << 8) | cpu.rd(need(addr) + 2 * i, B);
        cpu.put(ops[1], size, v);
    } else {
        int64_t v = cpu.get(ops[0], size);
        std::optional<int64_t> addr = cpu.address(ops[1], size);
        for (int64_t i = 0; i < n; ++i) cpu.wr(need(addr) + 2 * i, B, v >> (8 * (n - 1 - i)));
    }
}

// (LEA / PEA of an operand with no address -- a register or immediate -- is the TypeError of using None
// as a number; the 68000 has no such instruction, and decode does not produce one)
void _lea(CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) { cpu.a[ops[1].reg] = need(cpu.address(ops[0], L)); }

void _pea(CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) { cpu.push(need(cpu.address(ops[0], L))); }

void _clr(CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
    cpu.put(ops[0], size, 0);
    cpu.setf(std::nullopt, false, true, false, false);
}

void _tst(CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) { cpu.logic(cpu.get(ops[0], size), size); }

void _tas(CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
    cpu.modify(ops[0], B, [&](int64_t v) -> std::optional<int64_t> {
        cpu.logic(v, B);
        return v | 0x80;
    });
}

void _neg(CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
    cpu.modify(ops[0], size, [&](int64_t v) -> std::optional<int64_t> { return _sub(cpu, 0, v, size); });
}

void _negx(CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
    cpu.modify(ops[0], size, [&](int64_t v) -> std::optional<int64_t> { return _sub(cpu, 0, v, size, cpu.flag(FX), true); });
}

void _not(CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
    cpu.modify(ops[0], size, [&](int64_t v) -> std::optional<int64_t> {
        int64_t r = ~v & mask_of(size);
        cpu.logic(r, size);
        return r;
    });
}

void _ext(CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
    int64_t reg = ops[0].reg;
    int64_t r;
    if (size == "W") {
        r = _sx(cpu.d[reg], B) & 0xFFFF;
        cpu.d[reg] = (cpu.d[reg] & 0xFFFF0000) | r;
    } else {
        r = _sx(cpu.d[reg], W) & 0xFFFFFFFF;
        cpu.d[reg] = r;
    }
    cpu.logic(r, size);
}

void _swap(CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
    int64_t v = cpu.d[ops[0].reg];
    int64_t r = ((v << 16) | (v >> 16)) & 0xFFFFFFFF;
    cpu.d[ops[0].reg] = r;
    cpu.logic(r, L);
}

void _exg(CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
    auto [a, b] = unpack2(ops);
    int64_t va = cpu.get(a, L), vb = cpu.get(b, L);
    cpu.put(a, L, vb);
    cpu.put(b, L, va);
}

Handler _arith(const py::str& name) {
    return [name](CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
        auto [src, dst] = unpack2(ops);
        if (dst.kind == "ccr" || dst.kind == "sr") {   // ANDI/ORI/EORI to CCR/SR
            int64_t v = cpu.get(src, size);
            int64_t cur = cpu.get(dst, W);
            int64_t r = name == "AND" ? (cur & v) : name == "OR" ? (cur | v) : (cur ^ v);
            cpu.put(dst, W, r);
            return;
        }
        int64_t b = cpu.get(src, size);
        cpu.modify(dst, size, [&](int64_t a) -> std::optional<int64_t> {
            if (name == "ADD") return _add(cpu, a, b, size);
            if (name == "SUB") return _sub(cpu, a, b, size);
            if (name == "CMP") {
                _sub(cpu, a, b, size, 0, false, false);
                return std::nullopt;
            }
            int64_t r = name == "AND" ? (a & b) : name == "OR" ? (a | b) : (a ^ b);
            cpu.logic(r, size);
            return r;
        });
    };
}

Handler _addr_arith(const py::str& name) {
    return [name](CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
        int64_t v = _sx(cpu.get(ops[0], size), size);
        int64_t a = cpu.a[ops[1].reg];
        if (name == "CMPA") {
            _sub(cpu, a, v & 0xFFFFFFFF, L, 0, false, false);
            return;
        }
        cpu.a[ops[1].reg] = (name == "ADDA" ? a + v : a - v) & 0xFFFFFFFF;
    };
}

Handler _quick(const py::str& name) {
    return [name](CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
        int64_t q = ops[0].value;
        const Op& dst = ops[1];
        if (dst.kind == "A") {   // to an address register: all 32 bits, no flags
            cpu.a[dst.reg] = (cpu.a[dst.reg] + (name == "ADDQ" ? q : -q)) & 0xFFFFFFFF;
            return;
        }
        cpu.modify(dst, size, [&](int64_t a) -> std::optional<int64_t> {
            return name == "ADDQ" ? _add(cpu, a, q, size) : _sub(cpu, a, q, size);
        });
    };
}

Handler _extended(const py::str& name) {
    return [name](CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
        auto [src, dst] = unpack2(ops);
        int64_t b = cpu.get(src, size);
        cpu.modify(dst, size, [&](int64_t a) -> std::optional<int64_t> {
            return name == "ADDX" ? _add(cpu, a, b, size, cpu.flag(FX), true) : _sub(cpu, a, b, size, cpu.flag(FX), true);
        });
    };
}

void _cmpm(CPU& cpu, const Insn&, const py::str& size, const py::list<Op>& ops) {
    int64_t b = cpu.get(ops[0], size);
    int64_t a = cpu.get(ops[1], size);
    _sub(cpu, a, b, size, 0, false, false);
}

Handler _mul(bool signed_) {
    return [signed_](CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
        int64_t b = cpu.get(ops[0], W);
        int64_t a = cpu.d[ops[1].reg] & 0xFFFF;
        int64_t r = signed_ ? _sx(a, W) * _sx(b, W) : a * b;
        r &= 0xFFFFFFFF;
        cpu.d[ops[1].reg] = r;
        cpu.logic(r, L);
    };
}

Handler _div(bool signed_) {
    return [signed_](CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
        int64_t b = cpu.get(ops[0], W);
        int64_t a = cpu.d[ops[1].reg];
        if (b == 0) throw Stop(py::str("division by zero"));
        int64_t q, rem;
        if (signed_) {
            int64_t sa = _sx(a, L), sb = _sx(b, W);
            q = sa / sb;   // truncated toward zero, as abs(sa) // abs(sb) with the sign put back
            rem = sa - q * sb;
            if (!(-0x8000 <= q && q <= 0x7FFF)) {
                cpu.setf(std::nullopt, std::nullopt, std::nullopt, true, false);
                return;
            }
        } else {
            q = floordiv(a, b);   // (b is a device's raw answer when it comes from a mapped range: Python's floor semantics)
            rem = a - q * b;
            if (q > 0xFFFF) {
                cpu.setf(std::nullopt, std::nullopt, std::nullopt, true, false);
                return;
            }
        }
        cpu.d[ops[1].reg] = ((rem & 0xFFFF) << 16) | (q & 0xFFFF);
        cpu.logic(q & 0xFFFF, W);
    };
}

Handler _bcd(const py::str& name) {
    return [name](CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
        if (name == "NBCD") {
            cpu.modify(ops[0], B, [&](int64_t v) -> std::optional<int64_t> { return _bcd_sub(cpu, 0, v); });
            return;
        }
        int64_t b = cpu.get(ops[0], B);
        cpu.modify(ops[1], B, [&](int64_t a) -> std::optional<int64_t> {
            return name == "ABCD" ? _bcd_add(cpu, a, b) : _bcd_sub(cpu, a, b);
        });
    };
}

void _shift_handler(CPU& cpu, const Insn& insn, const py::str& size, const py::list<Op>& ops) {
    const py::str& name = insn.mnemonic;
    if (ops.size() == 1) {   // memory: one bit, word
        cpu.modify(ops[0], W, [&](int64_t v) -> std::optional<int64_t> { return _shift(cpu, name, v, 1, W); });
        return;
    }
    int64_t count = ops[0].kind == "quick" ? ops[0].value : (cpu.d[ops[0].reg] & 63);
    cpu.modify(ops[1], size, [&](int64_t v) -> std::optional<int64_t> { return _shift(cpu, name, v, count, size); });
}

Handler _bit(const py::str& name) {
    return [name](CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
        const Op& dst = ops[1];
        int64_t width = dst.kind == "D" ? 32 : 8;
        int64_t n = floormod(cpu.get(ops[0], L), width);
        const py::str& sz = dst.kind == "D" ? L : B;
        cpu.modify(dst, sz, [&](int64_t v) -> std::optional<int64_t> {
            cpu.setf(std::nullopt, std::nullopt, ((v >> n) & 1) == 0);
            if (name == "BTST") return std::nullopt;
            return name == "BCHG" ? (v ^ bit_of(n)) : name == "BCLR" ? (v & ~bit_of(n)) : (v | bit_of(n));
        });
    };
}

void _branch(CPU& cpu, const Insn& insn, const py::str& cc, const py::str&, const py::list<Op>&) {
    if (cc == "SR") {
        cpu.push(cpu.pc);
        cpu.pc = target_of(insn);
    } else if (cpu.cond(cc)) {
        cpu.pc = target_of(insn);
    }
}

void _dbcc(CPU& cpu, const Insn& insn, const py::str& cc, const py::str&, const py::list<Op>& ops) {
    if (cpu.cond(cc)) return;
    int64_t reg = ops[0].reg;
    int64_t v = (cpu.d[reg] - 1) & 0xFFFF;
    cpu.d[reg] = (cpu.d[reg] & 0xFFFF0000) | v;
    if (v != 0xFFFF) cpu.pc = target_of(insn);
}

void _scc(CPU& cpu, const Insn&, const py::str& cc, const py::str&, const py::list<Op>& ops) {
    const Op& dst = ops[0];   // (ops[0] is evaluated before the condition)
    int64_t v = cpu.cond(cc) ? 0xFF : 0;
    cpu.put(dst, B, v);
}

// (JMP / JSR of an operand with no address: the TypeError of using None as a number)
void _jmp(CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) { cpu.pc = need(cpu.address(ops[0], L)); }

void _jsr(CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
    int64_t t = need(cpu.address(ops[0], L));
    cpu.push(cpu.pc);
    cpu.pc = t;
}

void _rts(CPU& cpu, const Insn&, const py::str&, const py::list<Op>&) { cpu.pc = cpu.pop(); }

void _rtr(CPU& cpu, const Insn&, const py::str&, const py::list<Op>&) {
    cpu.sr = (cpu.sr & 0xFF00) | (cpu.pop(W) & 0x1F);
    cpu.pc = cpu.pop();
}

void _rte(CPU& cpu, const Insn&, const py::str&, const py::list<Op>&) {
    cpu.sr = cpu.pop(W) & 0xA71F;
    cpu.pc = cpu.pop();
}

void _link(CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
    int64_t reg = ops[0].reg;
    cpu.push(cpu.a[reg]);
    cpu.a[reg] = cpu.a[7];
    cpu.a[7] = (cpu.a[7] + _sx(ops[1].value, W)) & 0xFFFFFFFF;
}

void _unlk(CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
    int64_t reg = ops[0].reg;
    cpu.a[7] = cpu.a[reg];
    cpu.a[reg] = cpu.pop();
}

void _chk(CPU& cpu, const Insn&, const py::str&, const py::list<Op>& ops) {
    int64_t bound = _sx(cpu.get(ops[0], W), W);
    int64_t v = _sx(cpu.d[ops[1].reg], W);
    if (v < 0 || v > bound) {
        cpu.setf(std::nullopt, v < 0);
        throw Stop(py::str("CHK exception"));
    }
}

Handler _stop_run(const py::str& why) {
    return [why](CPU&, const Insn& insn, const py::str&, const py::list<Op>&) {
        throw Stop(why != "" ? why : insn.mnemonic);
    };
}

void _trapv(CPU& cpu, const Insn&, const py::str&, const py::list<Op>&) {
    if (cpu.flag(FV)) throw Stop(py::str("TRAPV exception"));
}

namespace {

py::dict<py::str, Handler> make_handlers() {
    py::dict<py::str, Handler> h;
    auto put = [&](const char* k, const Handler& f) { h.set(py::str(k), f); };
    put("MOVE", _move);
    put("MOVEA", _movea);
    put("MOVEQ", _moveq);
    put("MOVEM", _movem);
    put("MOVEP", _movep);
    put("LEA", _lea);
    put("PEA", _pea);
    put("CLR", _clr);
    put("TST", _tst);
    put("TAS", _tas);
    put("NEG", _neg);
    put("NEGX", _negx);
    put("NOT", _not);
    put("EXT", _ext);
    put("SWAP", _swap);
    put("EXG", _exg);
    put("ADD", _arith(py::str("ADD")));
    put("ADDI", _arith(py::str("ADD")));
    put("SUB", _arith(py::str("SUB")));
    put("SUBI", _arith(py::str("SUB")));
    put("CMP", _arith(py::str("CMP")));
    put("CMPI", _arith(py::str("CMP")));
    put("AND", _arith(py::str("AND")));
    put("ANDI", _arith(py::str("AND")));
    put("OR", _arith(py::str("OR")));
    put("ORI", _arith(py::str("OR")));
    put("EOR", _arith(py::str("EOR")));
    put("EORI", _arith(py::str("EOR")));
    put("ADDA", _addr_arith(py::str("ADDA")));
    put("SUBA", _addr_arith(py::str("SUBA")));
    put("CMPA", _addr_arith(py::str("CMPA")));
    put("ADDQ", _quick(py::str("ADDQ")));
    put("SUBQ", _quick(py::str("SUBQ")));
    put("ADDX", _extended(py::str("ADDX")));
    put("SUBX", _extended(py::str("SUBX")));
    put("CMPM", _cmpm);
    put("MULU", _mul(false));
    put("MULS", _mul(true));
    put("DIVU", _div(false));
    put("DIVS", _div(true));
    put("ABCD", _bcd(py::str("ABCD")));
    put("SBCD", _bcd(py::str("SBCD")));
    put("NBCD", _bcd(py::str("NBCD")));
    for (const char* k : {"ASL", "ASR", "LSL", "LSR", "ROL", "ROR", "ROXL", "ROXR"}) put(k, _shift_handler);
    put("BTST", _bit(py::str("BTST")));
    put("BCHG", _bit(py::str("BCHG")));
    put("BCLR", _bit(py::str("BCLR")));
    put("BSET", _bit(py::str("BSET")));
    put("JMP", _jmp);
    put("JSR", _jsr);
    put("RTS", _rts);
    put("RTR", _rtr);
    put("RTE", _rte);
    put("LINK", _link);
    put("UNLK", _unlk);
    put("CHK", _chk);
    put("NOP", [](CPU&, const Insn&, const py::str&, const py::list<Op>&) {});
    put("RESET", [](CPU&, const Insn&, const py::str&, const py::list<Op>&) {});
    put("TRAP", _stop_run(py::str("TRAP")));
    put("ILLEGAL", _stop_run(py::str("ILLEGAL")));
    put("STOP", _stop_run(py::str("STOP")));
    put("TRAPV", _trapv);
    return h;
}

py::list<std::tuple<py::str, PrefixedHandler>> make_prefixed() {
    py::list<std::tuple<py::str, PrefixedHandler>> out;
    out.append(std::tuple<py::str, PrefixedHandler>(py::str("DB"), _dbcc));
    out.append(std::tuple<py::str, PrefixedHandler>(py::str("B"), _branch));
    out.append(std::tuple<py::str, PrefixedHandler>(py::str("S"), _scc));
    return out;
}

py::set<py::str> make_not_prefixed() {
    py::set<py::str> out;
    for (const py::str& k : _HANDLERS) out.add(k);
    return out;
}

}  // namespace

const py::dict<py::str, Handler> _HANDLERS = make_handlers();
const py::list<std::tuple<py::str, PrefixedHandler>> _PREFIXED = make_prefixed();
const py::set<py::str> _NOT_PREFIXED = make_not_prefixed();

RunRecord run(const py::ref<Bus>& bus, int64_t entry, int64_t steps, const std::optional<py::list<int64_t>>& d,
              const std::optional<py::list<int64_t>>& a, int64_t sp, int64_t sr) {
    CPU cpu(bus);
    py::list<int64_t> dd, aa;
    if (d && !d->empty()) {
        for (int64_t v : *d) dd.append(v & 0xFFFFFFFF);
    } else {
        for (int i = 0; i < 8; ++i) dd.append(0);
    }
    if (a && !a->empty()) {
        for (int64_t v : *a) aa.append(v & 0xFFFFFFFF);
    } else {
        for (int i = 0; i < 8; ++i) aa.append(0);
    }
    cpu.d = dd;
    cpu.a = aa;
    cpu.a[7] = sp;
    cpu.sr = sr;
    cpu.push(RETURN);
    cpu.pc = entry;
    py::str end("limit");
    try {
        while (cpu.steps < steps) {
            if (cpu.pc == RETURN) {
                end = py::str("return");
                break;
            }
            cpu.step();
        }
    } catch (const Stop& e) {
        end = py::percent(py::str("stopped: %s"), py::str(e.what()));
    }
    RunRecord out;
    out.end = end;
    out.d = py::list<int64_t>(cpu.d.vec());
    out.a = py::list<int64_t>(cpu.a.vec());
    out.sr = cpu.sr;
    out.steps = cpu.steps;
    out.io = py::list<IoAccess>(bus->log.vec());   // list(getattr(bus, "log", []))
    return out;
}

emu_run::Run record(const RunRecord& out) {
    py::dict<py::str, emu_run::Register> results;
    for (int64_t i = 0; i < out.d.size(); ++i) results.set(py::percent(py::str("D%d"), i), emu_run::Register(out.d[i]));
    for (int64_t i = 0; i < out.a.size(); ++i) results.set(py::percent(py::str("A%d"), i), emu_run::Register(out.a[i]));
    results.set(py::str("SR"), emu_run::Register(out.sr));
    py::list<emu_run::IO> io;
    for (const IoAccess& t : out.io) {
        emu_run::IO x;
        x.kind = std::get<0>(t);
        x.port = std::get<1>(t);
        x.value = std::get<2>(t);
        io.append(x);
    }
    return emu_run::Run(out.end, results, py::list<emu_run::Write>(), false, py::list<emu_run::Call>(), io, std::nullopt, std::nullopt,
                        py::list<int64_t>(), out.steps, out.end == "limit", py::str("big"), std::make_shared<RunRecord>(out));
}

emu_run::Run recorded(const py::ref<Bus>& bus, int64_t entry, const RunOptions& kw) {
    return record(run(bus, entry, kw.steps, kw.d, kw.a, kw.sp, kw.sr));
}

}  // namespace codebreaker::emulate::m68k
