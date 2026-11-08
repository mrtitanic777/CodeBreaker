// codebreaker/emulate/powerpc.cpp -- typed C++ port of codebreaker.emulate.powerpc (see powerpc.hpp).

#include "codebreaker/emulate/powerpc.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>

namespace codebreaker::emulate::powerpc {

namespace {

constexpr uint64_t M32U = 0xFFFFFFFFULL;

// bits 0..n-1 set (n may be 64)
uint64_t ones(int64_t n) { return n >= 64 ? M64 : ((uint64_t(1) << n) - 1); }

uint32_t float_bits(float x) {
    uint32_t u;
    std::memcpy(&u, &x, 4);
    return u;
}
float bits_float(uint32_t u) {
    float x;
    std::memcpy(&x, &u, 4);
    return x;
}

// struct.pack(">f", x) as the 32 bits. (CPython keeps a signalling NaN signalling, and raises OverflowError
// for a finite x too large for a float -- the callers answer that with an infinity of x's sign, which is
// the float it rounds to.)
uint32_t pack_f32(double x) {
    float y = static_cast<float>(x);
    uint32_t u = float_bits(y);
    if (std::isnan(x)) {
        uint64_t v;
        std::memcpy(&v, &x, 8);
        if ((v & (uint64_t(1) << 51)) == 0) u &= ~(uint32_t(1) << 22);
    }
    return u;
}

// struct.unpack(">f", ...) as a double (a signalling NaN float stays signalling)
double unpack_f32(uint32_t u) {
    float x = bits_float(u);
    if (std::isnan(x) && (u & (uint32_t(1) << 22)) == 0) {
        double y = static_cast<double>(x);
        uint64_t u64;
        std::memcpy(&u64, &y, 8);
        u64 &= ~(uint64_t(1) << 51);
        std::memcpy(&y, &u64, 8);
        return y;
    }
    return static_cast<double>(x);
}

// the high 64 bits of the 128-bit product of two unsigned numbers
uint64_t mulhu(uint64_t a, uint64_t b) {
    uint64_t a0 = a & M32U, a1 = a >> 32, b0 = b & M32U, b1 = b >> 32;
    uint64_t p00 = a0 * b0, p01 = a0 * b1, p10 = a1 * b0, p11 = a1 * b1;
    uint64_t mid = (p00 >> 32) + (p01 & M32U) + (p10 & M32U);
    return p11 + (p01 >> 32) + (p10 >> 32) + (mid >> 32);
}

// the high 64 bits of the 128-bit product of two signed numbers
uint64_t mulhs(uint64_t a, uint64_t b) {
    uint64_t hi = mulhu(a, b);
    if (_s64(a) < 0) hi -= b;
    if (_s64(b) < 0) hi -= a;
    return hi;
}

// int.bit_length of an unsigned number
int64_t bit_length(uint64_t v) {
    int64_t n = 0;
    while (v) {
        ++n;
        v >>= 1;
    }
    return n;
}

py::dict<py::str, Pages>& filled_cache() {
    static py::dict<py::str, Pages> cache;     // _FILLED
    return cache;
}

py::list<uint64_t> eight_regs(Machine& m) {
    py::list<uint64_t> out;
    for (int64_t i = 0; i < 8; ++i) out.append(m.r[3 + i]);
    return out;
}

}  // namespace

int64_t _s64(uint64_t v) { return static_cast<int64_t>(v); }

int64_t _s32(uint64_t v) { return static_cast<int64_t>(static_cast<int32_t>(static_cast<uint32_t>(v & M32U))); }

uint64_t _sext(uint64_t v, int64_t bits) {
    uint64_t mask = ones(bits);
    v &= mask;
    return ((v >> (bits - 1)) & 1) ? (v | ~mask) : v;
}

uint64_t _mask(int64_t mb, int64_t me) {
    if (mb <= me) return ones(64 - mb) & ~ones(63 - me) & M64;
    return (ones(64 - mb) | ~ones(63 - me)) & M64;
}

uint64_t _rotl64(uint64_t v, int64_t n) {
    n &= 63;
    return n ? ((v << n) | (v >> (64 - n))) & M64 : v & M64;
}

uint64_t _rotl32(uint64_t v, int64_t n) {
    v &= M32U;
    n &= 31;
    uint64_t r = n ? ((v << n) | (v >> (32 - n))) & M32U : v;
    return r | (r << 32);                                    // the 32-bit result, doubled (as the hardware does)
}

double _f(uint64_t bits) {
    double x;
    std::memcpy(&x, &bits, 8);
    return x;
}

uint64_t _b(double x) {
    uint64_t u;
    std::memcpy(&u, &x, 8);
    return u;
}

double _single(double x) { return unpack_f32(pack_f32(x)); }

Pages pages(const py::list<Region>& regions) {
    py::dict<int64_t, py::bytearray> out;
    for (const Region& rg : regions) {
        int64_t va = std::get<0>(rg);
        const py::bytes& data = std::get<1>(rg);
        int64_t pos = 0;
        while (pos < data.size()) {
            int64_t a = (va + pos) & M32;
            int64_t off = a & (PAGE - 1);
            int64_t n = py::min(PAGE - off, data.size() - pos);
            py::bytearray page = out.setdefault(a >> 12, py::bytearray::zeros(PAGE));
            page.raw_mut().replace(static_cast<size_t>(off), static_cast<size_t>(n), data.raw(), static_cast<size_t>(pos),
                                   static_cast<size_t>(n));
            pos += n;
        }
    }
    Pages res;
    for (const auto& kv : out.items()) res.set(std::get<0>(kv), std::get<1>(kv).view());
    return res;
}

// ---- Machine

Machine::Machine(const py::list<Region>& regions, const std::optional<Pages>& shared_)
    : shared(shared_ ? *shared_ : Pages()) {
    for (int i = 0; i < 32; ++i) r.append(0);
    for (int i = 0; i < 32; ++i) f.append(0);                // floating registers, as 64 raw bits
    for (const Region& rg : regions) load(std::get<0>(rg), std::get<1>(rg));
}

void Machine::load(int64_t va, const py::bytes& data) {
    int64_t pos = 0;
    while (pos < data.size()) {
        int64_t a = (va + pos) & M32;
        std::string& page = _page(a);
        int64_t off = a & (PAGE - 1);
        int64_t n = py::min(PAGE - off, data.size() - pos);
        page.replace(static_cast<size_t>(off), static_cast<size_t>(n), data.raw(), static_cast<size_t>(pos),
                     static_cast<size_t>(n));
        pos += n;
    }
}

std::string& Machine::_page(int64_t a) {
    int64_t key = a >> 12;
    if (!pages.contains(key)) {
        bool has_base = shared.contains(key);
        if (static_cast<int64_t>(pages.size()) >= MAX_PAGES) throw Stop("memory");
        pages.set(key, has_base ? py::bytearray(shared.at(key)) : py::bytearray::zeros(PAGE));
    }
    return pages.at(key).raw_mut();
}

const std::string& Machine::_page_read(int64_t a) {
    int64_t key = a >> 12;
    if (pages.contains(key)) return pages.at(key).raw();
    if (shared.contains(key)) return shared.at(key).raw();
    return _page(a);
}

uint64_t Machine::read(int64_t a, int64_t n) {
    a &= M32;
    int64_t off = a & (PAGE - 1);
    uint64_t v = 0;
    if (off + n <= PAGE) {
        const std::string& page = _page_read(a);
        for (int64_t i = 0; i < n; ++i) v = (v << 8) | static_cast<unsigned char>(page[static_cast<size_t>(off + i)]);
        return v;
    }
    for (int64_t i = 0; i < n; ++i) {
        int64_t x = (a + i) & M32;
        v = (v << 8) | static_cast<unsigned char>(_page_read(x)[static_cast<size_t>(x & (PAGE - 1))]);
    }
    return v;
}

void Machine::write(int64_t a, int64_t n, uint64_t v) {
    a &= M32;
    bool track = !(STACK_LOW <= a && a < STACK_TOP);
    for (int64_t i = 0; i < n; ++i) {
        int64_t shift = 8 * (n - 1 - i);
        int64_t byte = shift >= 64 ? 0 : static_cast<int64_t>((v >> shift) & 0xFF);
        int64_t x = (a + i) & M32;
        std::string& page = _page(x);
        size_t off = static_cast<size_t>(x & (PAGE - 1));
        if (track) {
            int64_t first = writes.contains(x) ? std::get<0>(writes.at(x)) : static_cast<unsigned char>(page[off]);
            writes.set(x, std::make_tuple(first, byte));
        }
        page[off] = static_cast<char>(byte);
    }
}

void Machine::set_cr(int64_t field, int64_t bits) {
    int64_t shift = 28 - 4 * field;
    cr = ((cr & ~(int64_t(0xF) << shift)) | (bits << shift)) & M32;
}

void Machine::record(uint64_t v) {
    int64_t s = _s64(v & M64);
    set_cr(0, s < 0 ? 8 : s > 0 ? 4 : 2);
}

void Machine::compare(int64_t field, int64_t a, int64_t b) { set_cr(field, a < b ? 8 : a > b ? 4 : 2); }

void Machine::compare(int64_t field, uint64_t a, uint64_t b) { set_cr(field, a < b ? 8 : a > b ? 4 : 2); }

int64_t Machine::bit(int64_t i) { return (cr >> (31 - i)) & 1; }

std::tuple<uint64_t, int64_t> _add(Machine& m, uint64_t a, uint64_t b, int64_t carry) {
    (void)m;
    uint64_t sum = a + b;
    int64_t out = sum < a ? 1 : 0;
    uint64_t total = sum + static_cast<uint64_t>(carry);
    if (total < sum) out = 1;
    return {total, out};
}

// ---- tables

const py::dict<int64_t, std::function<int64_t(int64_t, int64_t)>> _CR_OPS = {
    {257, [](int64_t a, int64_t b) { return a & b; }},   {449, [](int64_t a, int64_t b) { return a | b; }},
    {193, [](int64_t a, int64_t b) { return a ^ b; }},   {225, [](int64_t a, int64_t b) { return ~(a & b); }},
    {33, [](int64_t a, int64_t b) { return ~(a | b); }}, {289, [](int64_t a, int64_t b) { return ~(a ^ b); }},
    {129, [](int64_t a, int64_t b) { return a & ~b; }},  {417, [](int64_t a, int64_t b) { return a | ~b; }}};

const py::dict<int64_t, LoadSpec> _LOADS = {{32, {4, 0, 0}}, {33, {4, 0, 1}}, {34, {1, 0, 0}}, {35, {1, 0, 1}},
                                            {40, {2, 0, 0}}, {41, {2, 0, 1}}, {42, {2, 1, 0}}, {43, {2, 1, 1}}};
const py::dict<int64_t, StoreSpec> _STORES = {{36, {4, 0}}, {37, {4, 1}}, {38, {1, 0}},
                                              {39, {1, 1}}, {44, {2, 0}}, {45, {2, 1}}};

// indexed loads and stores of opcode 31: xo -> (size, signed, update, byte-reversed)
const py::dict<int64_t, XLoadSpec> _XLOAD = {
    {23, {4, 0, 0, 0}},  {55, {4, 0, 1, 0}},  {87, {1, 0, 0, 0}},  {119, {1, 0, 1, 0}}, {279, {2, 0, 0, 0}},
    {311, {2, 0, 1, 0}}, {343, {2, 1, 0, 0}}, {375, {2, 1, 1, 0}}, {341, {4, 1, 0, 0}}, {373, {4, 1, 1, 0}},
    {21, {8, 0, 0, 0}},  {53, {8, 0, 1, 0}},  {534, {4, 0, 0, 1}}, {790, {2, 0, 0, 1}}, {20, {4, 0, 0, 0}},
    {84, {8, 0, 0, 0}}};
const py::dict<int64_t, XStoreSpec> _XSTORE = {
    {151, {4, 0, 0}}, {183, {4, 1, 0}}, {215, {1, 0, 0}}, {247, {1, 1, 0}}, {407, {2, 0, 0}},
    {439, {2, 1, 0}}, {149, {8, 0, 0}}, {181, {8, 1, 0}}, {662, {4, 0, 1}}, {918, {2, 0, 1}}};
const py::dict<int64_t, std::function<uint64_t(uint64_t, uint64_t)>> _LOGIC = {
    {28, [](uint64_t a, uint64_t b) { return a & b; }},     {60, [](uint64_t a, uint64_t b) { return a & ~b; }},
    {444, [](uint64_t a, uint64_t b) { return a | b; }},    {412, [](uint64_t a, uint64_t b) { return a | ~b; }},
    {316, [](uint64_t a, uint64_t b) { return a ^ b; }},    {124, [](uint64_t a, uint64_t b) { return ~(a | b); }},
    {476, [](uint64_t a, uint64_t b) { return ~(a & b); }}, {284, [](uint64_t a, uint64_t b) { return ~(a ^ b); }}};

// ---- branches

bool _taken(Machine& m, int64_t bo, int64_t bi) {
    bool ok = true;
    if (!(bo & 4)) {
        m.ctr = (m.ctr - 1) & M64;
        ok = (m.ctr != 0) != static_cast<bool>(bo & 2);
    }
    if (ok && !(bo & 16)) ok = m.bit(bi) == ((bo >> 3) & 1);
    return ok;
}

// ---- one instruction

void _step(Machine& m, int64_t w) {
    int64_t op = w >> 26;
    int64_t rt = (w >> 21) & 31, ra = (w >> 16) & 31, rb = (w >> 11) & 31;
    py::list<uint64_t>& r = m.r;
    int64_t nxt = (m.pc + 4) & M32;
    uint64_t simm = _sext(static_cast<uint64_t>(w & 0xFFFF), 16);
    uint64_t uimm = static_cast<uint64_t>(w & 0xFFFF);

    if (op == 14) {                                          // addi
        r[rt] = ((ra ? r[ra] : 0) + simm) & M64;
    } else if (op == 15) {                                   // addis
        r[rt] = ((ra ? r[ra] : 0) + _sext(uimm << 16, 32)) & M64;
    } else if ((32 <= op && op <= 55) || op == 58 || op == 62) {
        _memory(m, op, w, rt, ra, simm);
    } else if (op == 18) {                                   // b
        uint64_t d = _sext(static_cast<uint64_t>(w & 0x03FFFFFC), 26);
        if (w & 1) m.lr = nxt;
        nxt = static_cast<int64_t>(((w & 2) ? d : static_cast<uint64_t>(m.pc) + d) & M32U);
    } else if (op == 16) {                                   // bc
        if (_taken(m, rt, ra)) {
            uint64_t d = _sext(static_cast<uint64_t>(w & 0xFFFC), 16);
            if (w & 1) m.lr = nxt;
            nxt = static_cast<int64_t>(((w & 2) ? d : static_cast<uint64_t>(m.pc) + d) & M32U);
        } else if (w & 1) {
            m.lr = nxt;
        }
    } else if (op == 19) {
        int64_t xo = (w >> 1) & 0x3FF;
        if (xo == 16) {                                      // bclr
            int64_t target = m.lr;
            if (_taken(m, rt, ra)) {
                if (w & 1) m.lr = nxt;
                nxt = target & 0xFFFFFFFC;
            } else if (w & 1) {
                m.lr = nxt;
            }
        } else if (xo == 528) {                              // bcctr
            if (_taken(m, rt | 4, ra)) {
                if (w & 1) m.lr = nxt;
                nxt = static_cast<int64_t>(m.ctr & 0xFFFFFFFCULL);
            } else if (w & 1) {
                m.lr = nxt;
            }
        } else if (xo == 0) {                                // mcrf
            m.set_cr(rt >> 2, (m.cr >> (28 - 4 * (ra >> 2))) & 0xF);
        } else if (_CR_OPS.contains(xo)) {
            int64_t a = m.bit(ra), b = m.bit(rb);
            int64_t v = _CR_OPS.at(xo)(a, b) & 1;
            m.cr = ((m.cr & ~(int64_t(1) << (31 - rt))) | (v << (31 - rt))) & M32;
        } else if (xo == 150) {                              // isync
        } else {
            throw Stop("unsupported");
        }
    } else if (op == 31) {
        _extended(m, w, rt, ra, rb);
    } else if (op == 24) {
        r[ra] = r[rt] | uimm;
    } else if (op == 25) {
        r[ra] = r[rt] | (uimm << 16);
    } else if (op == 26) {
        r[ra] = r[rt] ^ uimm;
    } else if (op == 27) {
        r[ra] = r[rt] ^ (uimm << 16);
    } else if (op == 28) {
        r[ra] = r[rt] & uimm;
        m.record(r[ra]);
    } else if (op == 29) {
        r[ra] = r[rt] & (uimm << 16);
        m.record(r[ra]);
    } else if (op == 11) {                                   // cmpi
        if (rt & 1) {
            m.compare(rt >> 2, _s64(r[ra]), _s64(simm));
        } else {
            m.compare(rt >> 2, _s32(r[ra]), _s64(simm));
        }
    } else if (op == 10) {                                   // cmpli
        uint64_t a = (rt & 1) ? r[ra] : (r[ra] & M32U);
        m.compare(rt >> 2, a, uimm);
    } else if (op == 12 || op == 13) {                       // addic, addic.
        auto [sum, ca] = _add(m, r[ra], simm);
        r[rt] = sum;
        m.ca = ca;
        if (op == 13) m.record(r[rt]);
    } else if (op == 8) {                                    // subfic
        auto [sum, ca] = _add(m, ~r[ra] & M64, simm, 1);
        r[rt] = sum;
        m.ca = ca;
    } else if (op == 7) {                                    // mulli
        r[rt] = r[ra] * simm;                                // (the low 64 bits of the signed product)
    } else if (op == 21) {                                   // rlwinm
        int64_t mb = (w >> 6) & 31, me = (w >> 1) & 31;
        r[ra] = _rotl32(r[rt], rb) & _mask(mb + 32, me + 32);
        if (w & 1) m.record(r[ra]);
    } else if (op == 20) {                                   // rlwimi
        int64_t mb = (w >> 6) & 31, me = (w >> 1) & 31;
        uint64_t k = _mask(mb + 32, me + 32);
        r[ra] = (_rotl32(r[rt], rb) & k) | (r[ra] & ~k & M64);
        if (w & 1) m.record(r[ra]);
    } else if (op == 23) {                                   // rlwnm
        int64_t mb = (w >> 6) & 31, me = (w >> 1) & 31;
        r[ra] = _rotl32(r[rt], static_cast<int64_t>(r[rb] & 31)) & _mask(mb + 32, me + 32);
        if (w & 1) m.record(r[ra]);
    } else if (op == 30) {                                   // 64-bit rotates
        int64_t sh = rb | (((w >> 1) & 1) << 5);
        int64_t mbe = ((w >> 6) & 31) | (((w >> 5) & 1) << 5);
        int64_t kind = (w >> 2) & 7;
        uint64_t v;
        if (kind == 0) {                                     // rldicl
            v = _rotl64(r[rt], sh) & _mask(mbe, 63);
        } else if (kind == 1) {                              // rldicr
            v = _rotl64(r[rt], sh) & _mask(0, mbe);
        } else if (kind == 2) {                              // rldic
            v = _rotl64(r[rt], sh) & _mask(mbe, 63 - sh);
        } else if (kind == 3) {                              // rldimi
            uint64_t k = _mask(mbe, 63 - sh);
            v = (_rotl64(r[rt], sh) & k) | (r[ra] & ~k & M64);
        } else {
            int64_t n = static_cast<int64_t>(r[rb] & 63);
            if (((w >> 1) & 0xF) == 8) {                     // rldcl
                v = _rotl64(r[rt], n) & _mask(mbe, 63);
            } else if (((w >> 1) & 0xF) == 9) {              // rldcr
                v = _rotl64(r[rt], n) & _mask(0, mbe);
            } else {
                throw Stop("unsupported");
            }
        }
        r[ra] = v;
        if (w & 1) m.record(v);
    } else if (op == 59 || op == 63) {
        _float(m, op, w, rt, ra, rb);
    } else if (op == 3 || op == 2) {                         // twi, tdi: a trap is taken only if its test holds
        int64_t a = op == 3 ? _s32(r[ra]) : _s64(r[ra]);
        int64_t b = _s64(simm);
        uint64_t ua = op == 3 ? (r[ra] & M32U) : r[ra];
        uint64_t ub = op == 3 ? (simm & M32U) : simm;
        if (((rt & 16) && a < b) || ((rt & 8) && a > b) || ((rt & 4) && a == b) || ((rt & 2) && ua < ub) ||
            ((rt & 1) && ua > ub)) {
            throw Stop("trap");
        }
    } else if (op == 17) {                                   // sc
        throw Stop("syscall");
    } else {
        throw Stop("unsupported");
    }
    m.pc = nxt;
}

void _memory(Machine& m, int64_t op, int64_t w, int64_t rt, int64_t ra, uint64_t simm) {
    py::list<uint64_t>& r = m.r;
    if (op == 58 || op == 62) simm = _sext(static_cast<uint64_t>(w & 0xFFFC), 16);
    uint64_t base = ra ? r[ra] : 0;
    int64_t ea = static_cast<int64_t>((base + simm) & M32U);
    if (_LOADS.contains(op)) {
        LoadSpec s = _LOADS.at(op);
        uint64_t v = m.read(ea, s.size);
        r[rt] = s.signed_ ? _sext(v, 8 * s.size) : v;
        if (s.update) r[ra] = (base + simm) & M64;
    } else if (_STORES.contains(op)) {
        StoreSpec s = _STORES.at(op);
        m.write(ea, s.size, r[rt]);
        if (s.update) r[ra] = (base + simm) & M64;
    } else if (op == 58) {
        int64_t kind = w & 3;
        if (kind == 2) {                                     // lwa
            r[rt] = _sext(m.read(ea, 4), 32);
        } else {                                             // ld, ldu
            r[rt] = m.read(ea, 8);
            if (kind == 1) r[ra] = (base + simm) & M64;
        }
    } else if (op == 62) {                                   // std, stdu
        m.write(ea, 8, r[rt]);
        if ((w & 3) == 1) r[ra] = (base + simm) & M64;
    } else if (op == 46) {                                   // lmw
        for (int64_t i = rt; i < 32; ++i) r[i] = m.read(ea + 4 * (i - rt), 4);
    } else if (op == 47) {                                   // stmw
        for (int64_t i = rt; i < 32; ++i) m.write(ea + 4 * (i - rt), 4, r[i]);
    } else if (op == 48 || op == 49) {                       // lfs, lfsu
        m.f[rt] = _b(unpack_f32(static_cast<uint32_t>(m.read(ea, 4))));
        if (op == 49) r[ra] = (base + simm) & M64;
    } else if (op == 50 || op == 51) {                       // lfd, lfdu
        m.f[rt] = m.read(ea, 8);
        if (op == 51) r[ra] = (base + simm) & M64;
    } else if (op == 52 || op == 53) {                       // stfs, stfsu
        m.write(ea, 4, pack_f32(_f(m.f[rt])));
        if (op == 53) r[ra] = (base + simm) & M64;
    } else if (op == 54 || op == 55) {                       // stfd, stfdu
        m.write(ea, 8, m.f[rt]);
        if (op == 55) r[ra] = (base + simm) & M64;
    } else {
        throw Stop("unsupported");
    }
}

py::bytes _pack_single(double x) {
    uint32_t u = pack_f32(x);
    char b[4] = {static_cast<char>(u >> 24), static_cast<char>(u >> 16), static_cast<char>(u >> 8), static_cast<char>(u)};
    return py::bytes(b, 4);
}

void _extended(Machine& m, int64_t w, int64_t rt, int64_t ra, int64_t rb) {
    py::list<uint64_t>& r = m.r;
    int64_t xo = (w >> 1) & 0x3FF;
    int64_t rc = w & 1;
    uint64_t base = ra ? r[ra] : 0;
    if (_XLOAD.contains(xo)) {
        XLoadSpec s = _XLOAD.at(xo);
        int64_t ea = static_cast<int64_t>((base + r[rb]) & M32U);
        uint64_t v = m.read(ea, s.size);
        if (s.swap) {
            uint64_t sw = 0;
            for (int64_t i = 0; i < s.size; ++i) sw |= ((v >> (8 * i)) & 0xFF) << (8 * (s.size - 1 - i));
            v = sw;
        }
        r[rt] = s.signed_ ? _sext(v, 8 * s.size) : v;
        if (s.update) r[ra] = (base + r[rb]) & M64;
        if (xo == 20 || xo == 84) m.reserve = ea;            // lwarx, ldarx: a reservation
        return;
    }
    if (_XSTORE.contains(xo)) {
        XStoreSpec s = _XSTORE.at(xo);
        int64_t ea = static_cast<int64_t>((base + r[rb]) & M32U);
        uint64_t v = r[rt] & ones(8 * s.size);
        if (s.swap) {
            uint64_t sw = 0;
            for (int64_t i = 0; i < s.size; ++i) sw |= ((v >> (8 * i)) & 0xFF) << (8 * (s.size - 1 - i));
            v = sw;
        }
        m.write(ea, s.size, v);
        if (s.update) r[ra] = (base + r[rb]) & M64;
        return;
    }
    if (_LOGIC.contains(xo)) {
        r[ra] = _LOGIC.at(xo)(r[rt], r[rb]) & M64;
        if (rc) m.record(r[ra]);
        return;
    }
    if (xo == 150 || xo == 214) {                            // stwcx., stdcx.: stored while the reservation
        if (m.reserve.has_value()) {                         // lwarx/ldarx made holds (one processor: it
            m.write(static_cast<int64_t>((base + r[rb]) & M32U), xo == 150 ? 4 : 8, r[rt]);   // does, until this)
            m.set_cr(0, 2);
        } else {
            m.set_cr(0, 0);
        }
        m.reserve = std::nullopt;
        return;
    }
    if (xo == 0) {                                           // cmp
        if (rt & 1) {
            m.compare(rt >> 2, _s64(r[ra]), _s64(r[rb]));
        } else {
            m.compare(rt >> 2, _s32(r[ra]), _s32(r[rb]));
        }
        return;
    }
    if (xo == 32) {                                          // cmpl
        if (rt & 1) {
            m.compare(rt >> 2, r[ra], r[rb]);
        } else {
            m.compare(rt >> 2, r[ra] & M32U, r[rb] & M32U);
        }
        return;
    }
    if (xo == 339) {                                         // mfspr
        int64_t spr = ((w >> 16) & 31) | (((w >> 11) & 31) << 5);
        if (spr == 8) {
            r[rt] = static_cast<uint64_t>(m.lr);
        } else if (spr == 9) {
            r[rt] = m.ctr;
        } else if (spr == 1) {
            r[rt] = static_cast<uint64_t>(m.ca) << 29;
        } else {
            r[rt] = 0;                                       // time base and the like read as zero
        }
        return;
    }
    if (xo == 467) {                                         // mtspr
        int64_t spr = ((w >> 16) & 31) | (((w >> 11) & 31) << 5);
        if (spr == 8) {
            m.lr = static_cast<int64_t>(r[rt] & M32U);
        } else if (spr == 9) {
            m.ctr = r[rt];
        } else if (spr == 1) {
            m.ca = static_cast<int64_t>((r[rt] >> 29) & 1);
        }
        return;
    }
    if (xo == 19) {                                          // mfcr
        r[rt] = static_cast<uint64_t>(m.cr);
        return;
    }
    if (xo == 144) {                                         // mtcrf
        int64_t fxm = (w >> 12) & 0xFF;
        for (int64_t i = 0; i < 8; ++i) {
            if (fxm & (0x80 >> i)) m.set_cr(i, static_cast<int64_t>((r[rt] >> (28 - 4 * i)) & 0xF));
        }
        return;
    }
    if (xo == 371 || xo == 83) {                             // mftb, mfmsr
        r[rt] = 0;
        return;
    }
    if (xo == 278 || xo == 246 || xo == 86 || xo == 54 || xo == 598 || xo == 982 || xo == 854) {   // cache hints, sync, eieio
        return;
    }
    if (xo == 1014) {                                        // dcbz: the 32-byte block is cleared
        int64_t ea = static_cast<int64_t>((base + r[rb]) & M32U) & ~int64_t(31);
        for (int64_t i = 0; i < 32; i += 8) m.write(ea + i, 8, 0);
        return;
    }
    if (xo == 535 || xo == 567 || xo == 599 || xo == 631 || xo == 663 || xo == 695 || xo == 727 || xo == 759 ||
        xo == 983) {
        int64_t ea = static_cast<int64_t>((base + r[rb]) & M32U);
        if (xo == 535 || xo == 567) {                        // lfsx, lfsux
            m.f[rt] = _b(unpack_f32(static_cast<uint32_t>(m.read(ea, 4))));
        } else if (xo == 599 || xo == 631) {                 // lfdx, lfdux
            m.f[rt] = m.read(ea, 8);
        } else if (xo == 663 || xo == 695) {                 // stfsx, stfsux
            m.write(ea, 4, pack_f32(_f(m.f[rt])));
        } else if (xo == 727 || xo == 759) {                 // stfdx, stfdux
            m.write(ea, 8, m.f[rt]);
        } else {                                             // stfiwx
            m.write(ea, 4, m.f[rt] & M32U);
        }
        if (xo == 567 || xo == 631 || xo == 695 || xo == 759) r[ra] = (base + r[rb]) & M64;
        return;
    }
    if (xo == 4 || xo == 68) {                               // tw, td
        bool wide = xo == 68;
        int64_t a = wide ? _s64(r[ra]) : _s32(r[ra]);
        int64_t b = wide ? _s64(r[rb]) : _s32(r[rb]);
        uint64_t ua = wide ? r[ra] : (r[ra] & M32U);
        uint64_t ub = wide ? r[rb] : (r[rb] & M32U);
        if (((rt & 16) && a < b) || ((rt & 8) && a > b) || ((rt & 4) && a == b) || ((rt & 2) && ua < ub) ||
            ((rt & 1) && ua > ub)) {
            throw Stop("trap");
        }
        return;
    }
    // shifts
    uint64_t v;
    if (xo == 24) {                                          // slw
        int64_t n = static_cast<int64_t>(r[rb] & 63);
        v = n < 32 ? (r[rt] << n) & M32U : 0;
    } else if (xo == 536) {                                  // srw
        int64_t n = static_cast<int64_t>(r[rb] & 63);
        v = n < 32 ? (r[rt] & M32U) >> n : 0;
    } else if (xo == 792 || xo == 824) {                     // sraw, srawi
        int64_t n = xo == 792 ? static_cast<int64_t>(r[rb] & 63) : rb;
        int64_t s = _s32(r[rt]);
        if (n > 31) {
            v = s < 0 ? M64 : 0;
            m.ca = s < 0 ? 1 : 0;
        } else {
            v = static_cast<uint64_t>(s >> n);
            m.ca = (s < 0 && (r[rt] & ones(n))) ? 1 : 0;
        }
    } else if (xo == 27) {                                   // sld
        int64_t n = static_cast<int64_t>(r[rb] & 127);
        v = n < 64 ? (r[rt] << n) & M64 : 0;
    } else if (xo == 539) {                                  // srd
        int64_t n = static_cast<int64_t>(r[rb] & 127);
        v = n < 64 ? r[rt] >> n : 0;
    } else if (xo == 794 || (xo >> 1) == 413) {              // srad, sradi
        int64_t n = xo == 794 ? static_cast<int64_t>(r[rb] & 127) : (rb | (((w >> 1) & 1) << 5));
        int64_t s = _s64(r[rt]);
        if (n > 63) {
            v = s < 0 ? M64 : 0;
            m.ca = s < 0 ? 1 : 0;
        } else {
            v = static_cast<uint64_t>(s >> n);
            m.ca = (s < 0 && (r[rt] & ones(n))) ? 1 : 0;
        }
    } else if (xo == 26) {                                   // cntlzw
        v = static_cast<uint64_t>(32 - bit_length(r[rt] & M32U));
    } else if (xo == 58) {                                   // cntlzd
        v = static_cast<uint64_t>(64 - bit_length(r[rt]));
    } else if (xo == 954) {
        v = _sext(r[rt], 8);
    } else if (xo == 922) {
        v = _sext(r[rt], 16);
    } else if (xo == 986) {
        v = _sext(r[rt], 32);
    } else {
        _arithmetic(m, w, xo & 0x1FF, rt, ra, rb, rc);
        return;
    }
    r[ra] = v;
    if (rc) m.record(v);
}

void _arithmetic(Machine& m, int64_t w, int64_t xo, int64_t rt, int64_t ra, int64_t rb, int64_t rc) {
    (void)w;
    py::list<uint64_t>& r = m.r;
    uint64_t a = r[ra], b = r[rb];
    uint64_t v;
    if (xo == 266) {                                         // add
        v = (a + b) & M64;
    } else if (xo == 40) {                                   // subf
        v = (b - a) & M64;
    } else if (xo == 10) {                                   // addc
        auto [s, ca] = _add(m, a, b);
        v = s;
        m.ca = ca;
    } else if (xo == 8) {                                    // subfc
        auto [s, ca] = _add(m, ~a & M64, b, 1);
        v = s;
        m.ca = ca;
    } else if (xo == 138) {                                  // adde
        auto [s, ca] = _add(m, a, b, m.ca);
        v = s;
        m.ca = ca;
    } else if (xo == 136) {                                  // subfe
        auto [s, ca] = _add(m, ~a & M64, b, m.ca);
        v = s;
        m.ca = ca;
    } else if (xo == 202) {                                  // addze
        auto [s, ca] = _add(m, a, 0, m.ca);
        v = s;
        m.ca = ca;
    } else if (xo == 200) {                                  // subfze
        auto [s, ca] = _add(m, ~a & M64, 0, m.ca);
        v = s;
        m.ca = ca;
    } else if (xo == 234) {                                  // addme
        auto [s, ca] = _add(m, a, M64, m.ca);
        v = s;
        m.ca = ca;
    } else if (xo == 232) {                                  // subfme
        auto [s, ca] = _add(m, ~a & M64, M64, m.ca);
        v = s;
        m.ca = ca;
    } else if (xo == 104) {                                  // neg
        v = (0 - a) & M64;
    } else if (xo == 235) {                                  // mullw
        v = static_cast<uint64_t>(_s32(a) * _s32(b));
    } else if (xo == 75) {                                   // mulhw
        v = static_cast<uint64_t>((_s32(a) * _s32(b)) >> 32) & M32U;
    } else if (xo == 11) {                                   // mulhwu
        v = ((a & M32U) * (b & M32U)) >> 32;
    } else if (xo == 233) {                                  // mulld
        v = a * b;
    } else if (xo == 73) {                                   // mulhd
        v = mulhs(a, b);
    } else if (xo == 9) {                                    // mulhdu
        v = mulhu(a, b);
    } else if (xo == 491) {                                  // divw
        int64_t x = _s32(a), y = _s32(b);
        v = (y == 0 || (x == -0x80000000LL && y == -1)) ? 0 : static_cast<uint64_t>(x / y) & M32U;
    } else if (xo == 459) {                                  // divwu
        v = (b & M32U) == 0 ? 0 : (a & M32U) / (b & M32U);
    } else if (xo == 489) {                                  // divd
        int64_t x = _s64(a), y = _s64(b);
        if (y == 0 || (x == INT64_MIN && y == -1)) {
            v = 0;
        } else {
            uint64_t ux = x < 0 ? 0 - a : a, uy = y < 0 ? 0 - b : b;   // |x| and |y| (exact for the most negative)
            uint64_t q = ux / uy;
            v = ((x < 0) == (y < 0) ? q : 0 - q) & M64;
        }
    } else if (xo == 457) {                                  // divdu
        v = b == 0 ? 0 : a / b;
    } else {
        throw Stop("unsupported");
    }
    r[rt] = v;
    if (rc) m.record(v);
}

void _float(Machine& m, int64_t op, int64_t w, int64_t rt, int64_t ra, int64_t rb) {
    py::list<uint64_t>& f = m.f;
    int64_t rc5 = (w >> 6) & 31;
    int64_t xo5 = (w >> 1) & 31;
    int64_t xo = (w >> 1) & 0x3FF;
    bool single = op == 59;
    double a = _f(f[ra]), b = _f(f[rb]), c = _f(f[rc5]);
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();
    double v;
    // (Python guards this with `except (OverflowError, ValueError): v = nan`; float arithmetic there
    // raises neither, and the divisions and roots are guarded, so there is nothing to catch)
    if (xo5 == 21) {
        v = a + b;
    } else if (xo5 == 20) {
        v = a - b;
    } else if (xo5 == 25) {
        v = a * c;
    } else if (xo5 == 18) {
        v = b != 0.0 ? a / b : ((a == 0 || a != a) ? nan : std::copysign(inf, a) * std::copysign(1.0, b));
    } else if (xo5 == 29) {
        double t = a * c;
        v = t + b;
    } else if (xo5 == 28) {
        double t = a * c;
        v = t - b;
    } else if (xo5 == 31) {
        double t = a * c;
        v = -(t + b);
    } else if (xo5 == 30) {
        double t = a * c;
        v = -(t - b);
    } else if (xo5 == 22) {
        v = b >= 0 ? std::sqrt(b) : nan;
    } else if (xo5 == 24) {
        v = b != 0.0 ? 1.0 / b : std::copysign(inf, b);
    } else if (xo5 == 26) {
        v = b > 0 ? 1.0 / std::sqrt(b) : (b == 0 ? inf : nan);
    } else if (xo5 == 23 && !single) {                       // fsel
        f[rt] = a >= 0.0 ? f[rc5] : f[rb];
        return;
    } else if (single) {
        throw Stop("unsupported");
    } else {
        _float_other(m, w, xo, rt, ra, rb);
        return;
    }
    f[rt] = _b(single ? _single(v) : v);
}

void _float_other(Machine& m, int64_t w, int64_t xo, int64_t rt, int64_t ra, int64_t rb) {
    (void)w;
    py::list<uint64_t>& f = m.f;
    double b = _f(f[rb]);
    if (xo == 72) {                                          // fmr
        f[rt] = f[rb];
    } else if (xo == 40) {                                   // fneg
        f[rt] = f[rb] ^ (uint64_t(1) << 63);
    } else if (xo == 264) {                                  // fabs
        f[rt] = f[rb] & ~(uint64_t(1) << 63) & M64;
    } else if (xo == 136) {                                  // fnabs
        f[rt] = f[rb] | (uint64_t(1) << 63);
    } else if (xo == 12) {                                   // frsp
        f[rt] = _b(_single(b));
    } else if (xo == 0 || xo == 32) {                        // fcmpu, fcmpo
        double a = _f(f[ra]);
        m.set_cr(rt >> 2, (a != a || b != b) ? 1 : a < b ? 8 : a > b ? 4 : 2);
    } else if (xo == 14 || xo == 15) {                       // fctiw, fctiwz
        f[rt] = static_cast<uint64_t>(_to_int(b, 32, xo == 15)) & M32U;
    } else if (xo == 814 || xo == 815) {                     // fctid, fctidz
        f[rt] = static_cast<uint64_t>(_to_int(b, 64, xo == 815)) & M64;
    } else if (xo == 846) {                                  // fcfid
        f[rt] = _b(static_cast<double>(_s64(f[rb])));
    } else if (xo == 583 || xo == 711 || xo == 134 || xo == 70 || xo == 38 || xo == 64) {   // status register moves: no status is kept
        if (xo == 583) f[rt] = 0;
    } else {
        throw Stop("unsupported");
    }
}

int64_t _to_int(double x, int64_t bits, bool toward_zero) {
    // lo = -(1 << (bits-1)), hi = (1 << (bits-1)) - 1. Python compares a float with these ints exactly;
    // for 64 bits hi is not a double, and the doubles >= hi are exactly those >= 2**63.
    int64_t lo = bits == 64 ? INT64_MIN : -(int64_t(1) << (bits - 1));
    int64_t hi = bits == 64 ? INT64_MAX : (int64_t(1) << (bits - 1)) - 1;
    double hi_d = bits == 64 ? 9223372036854775808.0 : static_cast<double>(hi);
    if (x != x) return lo;
    if (x >= hi_d) return hi;
    if (x <= static_cast<double>(lo)) return lo;
    return toward_zero ? static_cast<int64_t>(x) : static_cast<int64_t>(std::nearbyint(x));
}

py::bytes filled(const py::str& kind, int64_t size) {
    std::string out;
    auto word = [&out](int64_t v) {
        if (v < 0 || v > 0xFFFFFFFFLL) throw py::StructError("'I' format requires 0 <= number <= 4294967295");
        out.push_back(static_cast<char>(v >> 24));
        out.push_back(static_cast<char>(v >> 16));
        out.push_back(static_cast<char>(v >> 8));
        out.push_back(static_cast<char>(v));
    };
    if (kind == py::str("pointers")) {
        for (int64_t a = 0; a < size; a += 4) word(ARGS_BASE + a + 0x100);
        return py::bytes(out);
    }
    if (kind == py::str("counts")) {
        for (int64_t a = 0; a < size; a += 4) word(a / 4 + 1);
        return py::bytes(out);
    }
    if (kind == py::str("text")) {
        for (int64_t a = 0; a < size; ++a) out.push_back(static_cast<char>(a % 24 == 23 ? 0 : 0x41 + (a % 24)));
        return py::bytes(out);
    }
    return py::bytes();
}

// ---- run

Out run(const py::list<Region>& regions, int64_t entry, const py::list<uint64_t>& args, const RunOptions& kw) {
    py::ref<Machine> mp = std::make_shared<Machine>(regions, kw.shared);
    Machine& m = *mp;
    if (kw.fill.has_value() && !kw.fill->empty()) {
        const py::str& fill = *kw.fill;
        py::dict<py::str, Pages>& cache = filled_cache();
        if (!cache.contains(fill)) cache.set(fill, pages(py::list<Region>{Region(ARGS_BASE, filled(fill))}));
        if (!m.shared.empty()) {
            Pages merged;                                    // {**_FILLED[fill], **m.shared}
            merged.update(cache.at(fill));
            merged.update(m.shared);
            m.shared = merged;
        } else {
            m.shared = cache.at(fill);
        }
    }
    py::dict<int64_t, py::str> externs = kw.externs ? *kw.externs : py::dict<int64_t, py::str>();
    int64_t na = std::min<int64_t>(args.size(), 8);
    for (int64_t i = 0; i < na; ++i) m.r[3 + i] = args[i] & M64;
    int64_t nf = std::min<int64_t>(kw.fargs.size(), 13);
    for (int64_t i = 0; i < nf; ++i) m.f[1 + i] = _b(kw.fargs[i]);
    for (int64_t i = 14; i < 32; ++i) m.r[i] = 0xA5A50000ULL + static_cast<uint64_t>(i);   // values a function must hand back unchanged
    m.r[1] = static_cast<uint64_t>(STACK_TOP - 0x4000);
    m.r[13] = 0x7E000000;                                    // per-thread data pointer of these targets
    m.r[2] = 0;
    m.lr = RETURN;
    m.pc = entry & M32;
    py::list<uint64_t> saved_regs;
    for (int64_t i = 14; i < 32; ++i) saved_regs.append(m.r[i]);
    uint64_t saved_sp = m.r[1];
    Out out;
    py::list<std::tuple<py::str, py::list<uint64_t>>>& calls = out.calls;
    py::set<int64_t>& path = out.path;
    int64_t lo = 0, hi = 0;
    if (kw.span.has_value()) {
        lo = std::get<0>(*kw.span);
        hi = std::get<1>(*kw.span);
    }
    std::optional<_Branches> log;
    if (kw.branches) log.emplace();
    std::optional<int64_t> last, linked;                     // the last instruction; where a call from it went
    int64_t count = 0;
    try {
        while (true) {
            int64_t pc = m.pc;
            if (pc == RETURN) {
                out.exit = py::str("ret");
                break;
            }
            auto name = externs.get(pc);
            if (name.has_value()) {
                if (static_cast<int64_t>(calls.size()) >= MAX_CALLS) {
                    out.truncated = true;
                    break;
                }
                calls.append(std::make_tuple(*name, eight_regs(m)));
                m.r[3] = static_cast<uint64_t>(kw.stub_ret);
                m.pc = m.lr & M32;
                if (kw.shadow != nullptr) kw.shadow->after_call(m, true, static_cast<uint64_t>(kw.stub_ret));
                continue;
            }
            if (count >= kw.steps) {
                out.truncated = true;
                break;
            }
            count += 1;
            if (lo <= pc && pc < hi) path.add(pc);
            if (pc & 3) throw Stop("alignment");
            int64_t w = static_cast<int64_t>(m.read(pc, 4));
            if (w == 0) {
                if (kw.absent.has_value() && std::get<0>(*kw.absent) <= pc && pc < std::get<1>(*kw.absent) &&
                    linked.has_value() && *linked == pc && m.lr == ((*last + 4) & M32)) {
                    // Called, and nothing is there: code outside what was given (a partial image
                    // calls into the rest of its title). A call out, like one to `externs`.
                    if (static_cast<int64_t>(calls.size()) >= MAX_CALLS) {
                        out.truncated = true;
                        break;
                    }
                    char buf[32];
                    std::snprintf(buf, sizeof buf, "@%08llx", static_cast<unsigned long long>(pc));
                    calls.append(std::make_tuple(py::str(buf), eight_regs(m)));
                    m.r[3] = static_cast<uint64_t>(kw.stub_ret);
                    m.pc = m.lr & M32;
                    if (kw.shadow != nullptr) kw.shadow->after_call(m, true, static_cast<uint64_t>(kw.stub_ret));
                    continue;
                }
                throw Stop("invalid");
            }
            last = pc;
            linked = std::nullopt;
            if (kw.shadow != nullptr) kw.shadow->step(m, pc);
            if (log.has_value() && lo <= pc && pc < hi) {
                log->step(m, pc, w);
            } else {
                _step(m, w);
            }
            if ((w & 1) && ((w >> 26) == 16 || (w >> 26) == 18 || (w >> 26) == 19)) {   // a branch that links: where it went
                linked = m.pc;
            }
        }
    } catch (const Stop& e) {
        out.exit = py::str("exception:") + py::str(std::string(e.what()));
    }
    out.ret = m.r[3];
    out.fret = m.f[1];
    out.steps = count;
    out.pc = m.pc;
    for (const auto& kv : m.writes.items()) {
        const std::tuple<int64_t, int64_t>& v = std::get<1>(kv);
        if (std::get<0>(v) != std::get<1>(v)) out.writes.set(std::get<0>(kv), v);
    }
    bool same = m.r[1] == saved_sp;
    for (int64_t i = 14; i < 32 && same; ++i) same = m.r[i] == saved_regs[i - 14];
    out.saved = same;
    out.machine = mp;
    if (log.has_value()) out.branches = log->events;
    return out;
}

emu_run::Run record(const Out& out) {
    std::optional<emu_run::Preserved> preserved;
    if (out.machine) {
        const Machine& m = *out.machine;
        emu_run::Preserved p;
        using Pair = std::tuple<std::optional<int64_t>, std::optional<int64_t>>;
        for (int64_t i = 14; i < 32; ++i) {
            p.set(py::str("r") + py::str(std::to_string(i)), Pair(0xA5A50000LL + i, static_cast<int64_t>(m.r[i])));
        }
        p.set(py::str("r1"), Pair(STACK_TOP - 0x4000, static_cast<int64_t>(m.r[1])));
        preserved = p;
    }
    py::dict<py::str, emu_run::Register> results;
    results.set(py::str("r3"), emu_run::Register(static_cast<int64_t>(out.ret)));
    results.set(py::str("f1"), emu_run::Register(static_cast<int64_t>(out.fret)));
    py::list<emu_run::Write> writes;
    for (const auto& kv : out.writes.items()) {
        const std::tuple<int64_t, int64_t>& v = std::get<1>(kv);
        writes.append(emu_run::Write{std::get<0>(kv), 1, std::get<1>(v), std::get<0>(v), false});
    }
    py::list<emu_run::Call> calls;
    for (const auto& c : out.calls) {
        py::list<int64_t> args;
        for (uint64_t a : std::get<1>(c)) args.append(static_cast<int64_t>(a));
        emu_run::Call call;
        call.name = std::get<0>(c);
        call.args = args;
        calls.append(call);
    }
    py::list<int64_t> path;
    for (int64_t a : out.path) path.append(a);
    return emu_run::Run(std::optional<py::str>(out.exit), results, writes, false, calls, py::list<emu_run::IO>(), std::nullopt,
                        preserved, path, out.steps, out.truncated, py::str("big"), std::make_shared<Out>(out));
}

emu_run::Run recorded(const py::list<Region>& regions, int64_t entry, const py::list<uint64_t>& args, const RunOptions& kw) {
    return record(run(regions, entry, args, kw));
}

// ---- _Branches

void _Branches::step(Machine& m, int64_t pc, int64_t w) {
    int64_t op = w >> 26;
    int64_t rt = (w >> 21) & 31, ra = (w >> 16) & 31, rb = (w >> 11) & 31;
    int64_t xo = (w >> 1) & 0x3FF;
    py::list<uint64_t>& r = m.r;
    std::optional<std::tuple<int64_t, Compare>> compare;
    std::optional<int64_t> branch;
    std::optional<std::tuple<int64_t, int64_t, int64_t>> load;
    if (op == 10 || op == 11 || (op == 31 && (xo == 0 || xo == 32))) {
        int64_t size = (rt & 1) ? 8 : 4;
        uint64_t mask = size == 8 ? M64 : M32U;
        uint64_t b;
        std::optional<int64_t> at_b;
        if (op == 11) {
            b = _sext(static_cast<uint64_t>(w & 0xFFFF), 16) & mask;
        } else if (op == 10) {
            b = static_cast<uint64_t>(w & 0xFFFF);
        } else {
            b = r[rb] & mask;
            at_b = _where(rb, size);
        }
        Compare c;
        c.size = size;
        c.a = r[ra] & mask;
        c.b = b;
        c.a_from = _where(ra, size);
        c.b_from = at_b;
        compare = std::make_tuple(rt >> 2, c);
    } else if ((op == 16 || (op == 19 && xo == 16)) && (rt & 0x14) != 0x14) {
        branch = (rt & 16) ? std::nullopt : std::optional<int64_t>(ra >> 2);   // (the field it tests; None: the counter alone)
    } else if (_LOADS.contains(op) || (op == 58 && ((w & 3) == 0 || (w & 3) == 2))) {
        int64_t size = _LOADS.contains(op) ? _LOADS.at(op).size : ((w & 3) == 0 ? 8 : 4);
        uint64_t d = _sext(static_cast<uint64_t>(w & (op == 58 ? 0xFFFC : 0xFFFF)), 16);
        load = std::make_tuple(rt, static_cast<int64_t>(((ra ? r[ra] : 0) + d) & M32U), size);
    }
    int64_t cr = m.cr;
    _step(m, w);
    if (compare.has_value()) {
        field.set(std::get<0>(*compare), std::get<1>(*compare));
    } else if (cr != m.cr) {                                 // a field set by something else
        for (int64_t f = 0; f < 8; ++f) {
            if (((cr ^ m.cr) >> (28 - 4 * f)) & 0xF) field.set(f, std::nullopt);
        }
    }
    if (op == 16 || (op == 19 && xo == 16)) {
        if ((rt & 0x14) != 0x14 && static_cast<int64_t>(events.size()) < CAP) {
            std::optional<Compare> seen;
            if (branch.has_value() && field.contains(*branch)) seen = field.at(*branch);
            events.append(std::make_tuple(pc, m.pc != ((pc + 4) & M32), seen));
        }
    } else if (!compare.has_value()) {
        const std::tuple<int64_t, int64_t> none_loaded;
        if (!(_STORES.contains(op) && !_STORES.at(op).update)) loaded.pop(rt, none_loaded);   // (a plain store writes no register)
        loaded.pop(ra, none_loaded);
        if (load.has_value() && std::get<0>(*load) != ra) {
            loaded.set(std::get<0>(*load), std::make_tuple(std::get<1>(*load), std::get<2>(*load)));
        }
    }
}

std::optional<int64_t> _Branches::_where(int64_t reg, int64_t size) {
    if (!loaded.contains(reg)) return std::nullopt;
    const std::tuple<int64_t, int64_t>& at = loaded.at(reg);
    if (std::get<1>(at) >= size) {
        return std::get<0>(at) + (std::get<1>(at) > size ? std::get<1>(at) - size : 0);
    }
    return std::nullopt;
}

}  // namespace codebreaker::emulate::powerpc
