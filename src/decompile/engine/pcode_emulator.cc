// decompile/engine/pcode_emulator.cc -- see pcode_emulator.hh. Built with the core (its own flags), so the core's headers stay here.
//
// The interpreter is its own loop over the p-code the translator emits for each instruction (cached by address, dropped when a
// write lands on a page the code was read from): the core's EmulatePcodeCache decodes again for every instruction, fails half-way
// when the next bytes are no instruction, and its memory banks cannot take byte-exact hooks. The arithmetic itself is the core's
// own OpBehavior, so an operation means here what it means in the decompiler.

#include "decompile/engine/pcode_emulator.hh"

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <cstdlib>
#include <functional>
#include <cmath>
#include <cstring>
#include <type_traits>
#include <set>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "decompile/engine/core/cpp/loadimage.hh"
#include "decompile/engine/core/cpp/opbehavior.hh"
#include "decompile/engine/core/cpp/pcoderaw.hh"
#include "decompile/engine/core/cpp/sleigh.hh"

namespace cb::decompile::sleigh {
namespace {

using ghidra::AddrSpace;
using ghidra::OpCode;
using ghidra::VarnodeData;

// The bits of a value as another type (this file builds as C++17: no std::bit_cast).
template <class To, class From>
To bits_as(const From& from) {
    static_assert(sizeof(To) == sizeof(From));
    To to;
    std::memcpy(&to, &from, sizeof to);
    return to;
}

// A NaN operand gives that NaN, quiet, the first operand's before the second's: written out, because the compiler may commute a host a * b and the host keeps whichever
// it was handed first.
template <class F>
F first_nan(F a, F c, F result) {
    using Bits = std::conditional_t<sizeof(F) == 8, std::uint64_t, std::uint32_t>;
    constexpr Bits quiet = Bits{1} << (sizeof(F) == 8 ? 51 : 22);
    if (a != a) return bits_as<F>(static_cast<Bits>(bits_as<Bits>(a) | quiet));
    if (c != c) return bits_as<F>(static_cast<Bits>(bits_as<Bits>(c) | quiet));
    return result;
}

// The common operations of at most 8 bytes, written out where the behaviour table would be called: each is the formula of its OpBehavior, line for line (the test
// compares them over every size and edge value). Returns false for an operation not written out here.
inline bool fast_unary(OpCode opc, int sizeout, int sizein, std::uint64_t a, std::uint64_t& r) {
    switch (opc) {
        case ghidra::CPUI_COPY: r = a; return true;
        case ghidra::CPUI_INT_ZEXT: r = a; return true;
        case ghidra::CPUI_INT_SEXT: r = ghidra::sign_extend(a, sizein, sizeout); return true;
        case ghidra::CPUI_INT_NEGATE: r = ~a & ghidra::calc_mask(sizein); return true;
        case ghidra::CPUI_INT_2COMP: r = ~(a - 1) & ghidra::calc_mask(sizein); return true;
        case ghidra::CPUI_BOOL_NEGATE: r = a ^ 1; return true;
        default: return false;
    }
}

inline bool fast_binary(OpCode opc, int sizeout, int sizein, std::uint64_t a, std::uint64_t c, std::uint64_t& r) {
    switch (opc) {
        case ghidra::CPUI_INT_ADD: r = (a + c) & ghidra::calc_mask(sizeout); return true;
        case ghidra::CPUI_INT_SUB: r = (a - c) & ghidra::calc_mask(sizeout); return true;
        case ghidra::CPUI_INT_MULT: r = (a * c) & ghidra::calc_mask(sizeout); return true;
        case ghidra::CPUI_INT_AND: r = a & c; return true;
        case ghidra::CPUI_INT_OR: r = a | c; return true;
        case ghidra::CPUI_INT_XOR: r = a ^ c; return true;
        case ghidra::CPUI_INT_EQUAL: r = a == c ? 1 : 0; return true;
        case ghidra::CPUI_INT_NOTEQUAL: r = a != c ? 1 : 0; return true;
        case ghidra::CPUI_INT_LESS: r = a < c ? 1 : 0; return true;
        case ghidra::CPUI_INT_LESSEQUAL: r = a <= c ? 1 : 0; return true;
        case ghidra::CPUI_INT_SLESS:
        case ghidra::CPUI_INT_SLESSEQUAL: {
            if (sizein <= 0) {
                r = 0;
                return true;
            }
            std::uint64_t mask = 0x80;
            mask <<= 8 * (sizein - 1);
            const std::uint64_t bit1 = a & mask, bit2 = c & mask;
            if (bit1 != bit2) r = bit1 != 0 ? 1 : 0;
            else r = opc == ghidra::CPUI_INT_SLESS ? (a < c ? 1 : 0) : (a <= c ? 1 : 0);
            return true;
        }
        case ghidra::CPUI_INT_LEFT: r = c >= static_cast<std::uint64_t>(sizeout) * 8 ? 0 : (a << c) & ghidra::calc_mask(sizeout); return true;
        case ghidra::CPUI_INT_RIGHT: r = c >= static_cast<std::uint64_t>(sizeout) * 8 ? 0 : (a & ghidra::calc_mask(sizeout)) >> c; return true;
        case ghidra::CPUI_SUBPIECE: r = c >= sizeof(std::uint64_t) ? 0 : (a >> (c * 8)) & ghidra::calc_mask(sizeout); return true;
        case ghidra::CPUI_INT_CARRY: r = a > ((a + c) & ghidra::calc_mask(sizein)) ? 1 : 0; return true;
        case ghidra::CPUI_INT_SCARRY: {
            const std::uint64_t sum = a + c;
            const unsigned top = static_cast<unsigned>(sizein) * 8 - 1;   // (the sign bit's place)
            const unsigned sa = static_cast<unsigned>(a >> top) & 1, sb = static_cast<unsigned>(c >> top) & 1, sr = static_cast<unsigned>(sum >> top) & 1;
            r = (sr ^ sa) & (sa ^ sb ^ 1);
            return true;
        }
        case ghidra::CPUI_INT_SBORROW: {
            const std::uint64_t diff = a - c;
            const unsigned top = static_cast<unsigned>(sizein) * 8 - 1;
            const unsigned sa = static_cast<unsigned>(a >> top) & 1, sb = static_cast<unsigned>(c >> top) & 1, sr = static_cast<unsigned>(diff >> top) & 1;
            r = (sa ^ sr) & (sr ^ sb ^ 1);
            return true;
        }
        case ghidra::CPUI_BOOL_AND: r = a & c; return true;
        case ghidra::CPUI_BOOL_OR: r = a | c; return true;
        case ghidra::CPUI_BOOL_XOR: r = a ^ c; return true;
        default: return false;
    }
}

// Whether fast_unary / fast_binary write the operation out (they return false for the others).
constexpr bool isFastUnary(OpCode opc) {
    return opc == ghidra::CPUI_COPY || opc == ghidra::CPUI_INT_ZEXT || opc == ghidra::CPUI_INT_SEXT || opc == ghidra::CPUI_INT_NEGATE || opc == ghidra::CPUI_INT_2COMP ||
           opc == ghidra::CPUI_BOOL_NEGATE;
}
constexpr bool isFastBinary(OpCode opc) {
    switch (opc) {
        case ghidra::CPUI_INT_ADD: case ghidra::CPUI_INT_SUB: case ghidra::CPUI_INT_MULT: case ghidra::CPUI_INT_AND: case ghidra::CPUI_INT_OR: case ghidra::CPUI_INT_XOR:
        case ghidra::CPUI_INT_EQUAL: case ghidra::CPUI_INT_NOTEQUAL: case ghidra::CPUI_INT_LESS: case ghidra::CPUI_INT_LESSEQUAL: case ghidra::CPUI_INT_SLESS:
        case ghidra::CPUI_INT_SLESSEQUAL: case ghidra::CPUI_INT_LEFT: case ghidra::CPUI_INT_RIGHT: case ghidra::CPUI_SUBPIECE: case ghidra::CPUI_INT_CARRY:
        case ghidra::CPUI_INT_SCARRY: case ghidra::CPUI_INT_SBORROW: case ghidra::CPUI_BOOL_AND: case ghidra::CPUI_BOOL_OR: case ghidra::CPUI_BOOL_XOR: return true;
        default: return false;
    }
}

constexpr std::size_t kRetiredLimit = 1u << 16;   // decoded instructions set aside at most
constexpr std::uint64_t kPageBits = 12;
constexpr std::uint64_t kPageSize = std::uint64_t{1} << kPageBits;

#if defined(_MSC_VER) && !defined(__clang__)
#define CB_PAGE_NOINLINE __declspec(noinline)
#else
#define CB_PAGE_NOINLINE __attribute__((noinline))
#endif

struct Page {
    std::array<std::uint8_t, kPageSize> d{};
    bool code = false;   // instructions were decoded from this page
    std::bitset<kPageSize> decoded;   // ... and these are the bytes they were decoded from: a write that changes one of them drops the instructions that read it
};

struct SpaceMem {
    AddrSpace* space = nullptr;
    unsigned word = 1;     // bytes per addressable unit
    bool big = false;
    bool hooked = false;   // memory-mapped I/O hooks see this space
    bool isCode = false;
    mutable std::unordered_map<std::uint64_t, std::unique_ptr<Page>> pages;
    mutable const Page* last = nullptr;
    mutable std::uint64_t lastKey = ~std::uint64_t{0};
    // Where the bytes of a page the map does not hold come from (Emulator::setPageSource): the page is made the first time it is looked up.
    std::function<bool(std::uint64_t address, std::uint8_t* out, std::size_t size)> source;

    // The page, without asking the source: null when the map does not hold it.
    const Page* findHeld(std::uint64_t key) const {
        if (key == lastKey) return last;
        auto it = pages.find(key);
        const Page* p = it == pages.end() ? nullptr : it->second.get();
        if (p) lastKey = key, last = p;
        return p;
    }
    const Page* find(std::uint64_t key) const {
        if (key == lastKey) return last;
        auto it = pages.find(key);
        const Page* p = it == pages.end() ? nullptr : it->second.get();
        if (!p && source) p = fromSource(key);
        if (p) lastKey = key, last = p;
        return p;
    }
    // The page the source has bytes for, made (null: it has none).
    CB_PAGE_NOINLINE const Page* fromSource(std::uint64_t key) const {
        const std::uint64_t address = key << kPageBits;
        if (!source(address, nullptr, static_cast<std::size_t>(kPageSize))) return nullptr;   // (nothing there: the page reads zeros and is not made)
        auto made = std::make_unique<Page>();
        source(address, made->d.data(), static_cast<std::size_t>(kPageSize));
        return pages.emplace(key, std::move(made)).first->second.get();
    }
    Page* make(std::uint64_t key) {
        std::unique_ptr<Page>& slot = pages[key];
        if (!slot) slot = std::make_unique<Page>();
        lastKey = key;
        last = slot.get();
        return slot.get();
    }
};

struct Op {
    OpCode opc;
    bool hasOut;
    unsigned nIn;
    std::size_t first;   // into Decoded::vn: the output (if any), then the inputs
};

// How to reach a varnode without looking anything up: a constant, or the bytes of a register or temporary inside one page (the pointers hold while the page does: the
// decoded instructions are dropped whenever a memory is cleared).
struct Slot {
    std::uint8_t* ptr = nullptr;   // direct: the bytes
    std::uint64_t value = 0;       // constant: its value
    std::uint8_t size = 0;
    std::uint8_t kind = 0;         // 0 look it up, 1 constant, 2 direct
    bool big = false;
};

struct Decoded {
    std::size_t length = 0;   // bytes
    std::vector<Op> ops;
    std::vector<VarnodeData> vn;
    std::vector<Slot> slots;  // by vn
    std::vector<std::uint8_t> raw;   // the bytes it was decoded from (kept for a decode set aside by a change of them: it is used again if they come back)
    std::uint64_t tag = 0;           // the context it was decoded under
};

class Collect : public ghidra::PcodeEmit {
public:
    explicit Collect(Decoded& d) : d_(d) {}
    void dump(const ghidra::Address&, OpCode opc, VarnodeData* outvar, VarnodeData* vars, ghidra::int4 isize) override {
        Op op{opc, outvar != nullptr, static_cast<unsigned>(isize), d_.vn.size()};
        if (outvar) d_.vn.push_back(*outvar);
        for (ghidra::int4 i = 0; i < isize; ++i) d_.vn.push_back(vars[i]);
        d_.ops.push_back(op);
    }

private:
    Decoded& d_;
};

class Text : public ghidra::AssemblyEmit {
public:
    void dump(const ghidra::Address&, const std::string& mnem, const std::string& body) override { text = body.empty() ? mnem : mnem + " " + body; }
    std::string text;
};

void initialize_once() {
    static const bool done = [] {
        ghidra::AttributeId::initialize();
        ghidra::ElementId::initialize();
        return true;
    }();
    (void)done;
}

std::atomic<int> g_blocks_override{-1};   // -1: the environment (CB_PCODE_BLOCKS=0 turns blocks off); 0 / 1: forced

}  // namespace

struct Emulator::Impl {
    // The bytes the translator decodes: the code space's pages.
    class Image : public ghidra::LoadImage {
    public:
        explicit Image(Impl& owner) : LoadImage("codebreaker"), o(owner) {}
        void loadFill(ghidra::uint1* ptr, ghidra::int4 size, const ghidra::Address& addr) override {
            SpaceMem& m = o.mem[o.codeIndex];
            const std::uint64_t start = addr.getOffset();   // (the translator's addresses are bytes, in a word-addressed space too)
            for (ghidra::int4 i = 0; i < size; ++i) {
                const std::uint64_t a = start + static_cast<std::uint64_t>(i);
                Page* p = const_cast<Page*>(m.find(a >> kPageBits));
                if (!p) p = m.make(a >> kPageBits);   // (zeros decoded here are code too: a later write to them must drop what was decoded)
                p->code = true;
                p->decoded.set(a & (kPageSize - 1));
                ptr[i] = p->d[a & (kPageSize - 1)];
            }
        }
        std::string getArchType() const override { return "codebreaker"; }
        void adjustVma(long) override {}
        Impl& o;
    };

    Image image{*this};
    ghidra::ContextInternal context;
    std::unique_ptr<ghidra::Sleigh> trans;
    ghidra::DocumentStorage store;
    std::vector<ghidra::OpBehavior*> behave;
    std::vector<SpaceMem> mem;          // by AddrSpace::getIndex()
    int codeIndex = 0, regIndex = -1;
    const AddrSpace* constSpace = nullptr;
    std::vector<std::string> userOps;
    std::string unnamedOp;                  // the name given to a user operation the description does not name
    std::size_t segmentOp = ~std::size_t{0};   // the index of the built-in "segment" operation
    unsigned segmentShift = 0;
    std::unordered_map<std::string, VarnodeData> regCache;
    std::vector<Register> topRegs, allRegs;
    bool topRegsDone = false;
    const VarnodeData* pcReg = nullptr;
    VarnodeData pcRegData{};

    // The decoded instructions, by the context they were decoded under (ARM's Thumb state), then by address: a change of context (a BX) picks another set rather
    // than throwing them away.
    std::unordered_map<std::uint64_t, std::unordered_map<std::uint64_t, Decoded>> caches;
    std::unordered_map<std::uint64_t, Decoded>* cache = &caches[0];
    std::uint64_t contextTag = 0;
    // The instructions run most recently, found without a lookup in the cache: by the low bits of the address (an entry is cleared when its instruction is dropped, and all of
    // them when the cache or its context changes).
    static constexpr std::size_t kHot = 1024;
    std::array<const Decoded*, kHot> hotEntry{};
    std::array<std::uint64_t, kHot> hotPc{};
    const void* hotCache = nullptr;
    void clearHot() { hotEntry.fill(nullptr); }
    // Decoded instructions set aside because bytes they were decoded from changed, by address: code that is written over and over (a routine run from RAM that changes, a
    // device that answers a fetch differently each time) comes back to bytes it had before, and decoding from the description is the costly step.
    std::unordered_map<std::uint64_t, std::vector<Decoded>> retired;
    std::size_t retiredCount = 0;
    bool parsesStale = false;   // a context default changed since the translator's parse cache was last cleared: it is cleared when an instruction is next decoded
    std::map<std::string, int> contextSet;   // the variables setContext changed, and to what
    bool addressContextKept = true;     // the context a decoded instruction sets for other addresses (SLEIGH's globalset) stays; false: dropped after each decode
    bool dirty = false;                // too much decoded code changed: flush before the next step
    std::vector<std::uint64_t> stale;  // byte addresses of decoded code that changed: the instructions that read them are dropped before the next step
    std::uint64_t pcValue = 0;
    std::uint64_t stepCount = 0;
    std::set<std::uint64_t> stops;
    std::string error;
    std::map<std::string, std::uint64_t> unimpl;
    ReadHook readHook;
    WriteHook writeHook;
    Trace trace;
    UserOpHook userHook;
    BadInstructionHook badHook;

    Impl() = default;
    ~Impl() {
        for (ghidra::OpBehavior* b : behave) delete b;
    }

    SpaceMem& space(std::string_view name) {
        if (name.empty()) return mem[codeIndex];
        for (SpaceMem& m : mem)
            if (m.space && m.space->getName() == name) return m;
        throw std::runtime_error("no address space " + std::string(name));
    }
    const SpaceMem& space(std::string_view name) const { return const_cast<Impl*>(this)->space(name); }

    // ---- bytes
    static std::uint8_t getByte(const SpaceMem& m, std::uint64_t a) {
        const Page* p = m.find(a >> kPageBits);
        return p ? p->d[a & (kPageSize - 1)] : 0;
    }
    void putByte(SpaceMem& m, std::uint64_t a, std::uint8_t v) {
        const std::uint64_t key = a >> kPageBits;
        Page* p = const_cast<Page*>(m.find(key));
        if (!p) {
            if (v == 0) return;
            p = m.make(key);
        }
        const std::size_t off = static_cast<std::size_t>(a & (kPageSize - 1));
        if (p->code && p->d[off] != v && p->decoded[off]) noteCodeWrite(a);
        p->d[off] = v;
    }
    std::uint64_t getValueBytes(const SpaceMem& m, std::uint64_t a, unsigned size) const {
        std::uint64_t v = 0;
        const unsigned n = size > 8 ? 8 : size;
        if (size <= 8) {   // (the common case: the value is inside one page: its bytes are put together without looking the page up for each)
            const std::uint64_t off = a & (kPageSize - 1);
            if (off + size <= kPageSize) {
                const Page* p = m.find(a >> kPageBits);
                if (!p) return 0;
                const std::uint8_t* b = p->d.data() + off;
                if (size == 4) {   // (the common widths written out: the compiler turns these into one load and a byte swap)
                    return m.big ? (std::uint64_t{b[0]} << 24 | std::uint64_t{b[1]} << 16 | std::uint64_t{b[2]} << 8 | b[3])
                                 : (std::uint64_t{b[3]} << 24 | std::uint64_t{b[2]} << 16 | std::uint64_t{b[1]} << 8 | b[0]);
                }
                if (size == 8) {
                    return m.big ? (std::uint64_t{b[0]} << 56 | std::uint64_t{b[1]} << 48 | std::uint64_t{b[2]} << 40 | std::uint64_t{b[3]} << 32 | std::uint64_t{b[4]} << 24 |
                                    std::uint64_t{b[5]} << 16 | std::uint64_t{b[6]} << 8 | b[7])
                                 : (std::uint64_t{b[7]} << 56 | std::uint64_t{b[6]} << 48 | std::uint64_t{b[5]} << 40 | std::uint64_t{b[4]} << 32 | std::uint64_t{b[3]} << 24 |
                                    std::uint64_t{b[2]} << 16 | std::uint64_t{b[1]} << 8 | b[0]);
                }
                if (m.big)
                    for (unsigned i = 0; i < size; ++i) v = (v << 8) | b[i];
                else
                    for (unsigned i = size; i-- > 0;) v = (v << 8) | b[i];
                return v;
            }
        }
        if (m.big) {
            // the low bytes are the last ones
            for (unsigned i = 0; i < n; ++i) v = (v << 8) | getByte(m, a + size - n + i);
        } else {
            for (unsigned i = n; i-- > 0;) v = (v << 8) | getByte(m, a + i);
        }
        return v;
    }
    void putValueBytes(SpaceMem& m, std::uint64_t a, unsigned size, std::uint64_t v) {
        if (size <= 8) {   // (inside one page: one lookup)
            const std::uint64_t off = a & (kPageSize - 1);
            if (off + size <= kPageSize) {
                const std::uint64_t key = a >> kPageBits;
                Page* p = const_cast<Page*>(m.find(key));
                if (!p) {
                    if (v == 0) return;   // (a page that does not exist reads zeros)
                    p = m.make(key);
                }
                std::uint8_t* b = p->d.data() + off;
                if (p->code) {   // (a write that changes a byte an instruction was decoded from drops that instruction: the others stay)
                    std::uint64_t w = v;
                    for (unsigned i = 0; i < size; ++i) {
                        const unsigned at = m.big ? size - 1 - i : i;
                        const std::uint8_t nb = static_cast<std::uint8_t>(w);
                        w >>= 8;
                        if (b[at] != nb && p->decoded[static_cast<std::size_t>(off) + at]) noteCodeWrite(a + at);
                    }
                }
                if (m.big)
                    for (unsigned i = size; i-- > 0; v >>= 8) b[i] = static_cast<std::uint8_t>(v);
                else
                    for (unsigned i = 0; i < size; ++i, v >>= 8) b[i] = static_cast<std::uint8_t>(v);
                return;
            }
        }
        if (m.big) {
            for (unsigned i = size; i-- > 0;) {
                putByte(m, a + i, static_cast<std::uint8_t>(v));
                v >>= 8;   // (a varnode wider than 8 bytes gets zeros above its low 8)
            }
        } else {
            for (unsigned i = 0; i < size; ++i) {
                putByte(m, a + i, i < 8 ? static_cast<std::uint8_t>(v >> (8 * i)) : 0);
            }
        }
    }

    // ---- p-code accesses (hooks apply)
    std::uint64_t readSpace(SpaceMem& m, std::uint64_t unitOff, unsigned size) {
        const std::uint64_t a = m.space->wrapOffset(unitOff) * m.word;
        if (m.hooked && readHook) {
            std::uint64_t value = 0;
            ++hookCalls;
            if (readHook(m.space->getName(), a, size, value)) return size >= 8 ? value : value & ((std::uint64_t{1} << (8 * size)) - 1);
        }
        return getValueBytes(m, a, size);
    }
    void writeSpace(SpaceMem& m, std::uint64_t unitOff, unsigned size, std::uint64_t v) {
        const std::uint64_t a = m.space->wrapOffset(unitOff) * m.word;
        if (m.hooked && writeHook && ++hookCalls && writeHook(m.space->getName(), a, size, size >= 8 ? v : v & ((std::uint64_t{1} << (8 * size)) - 1))) return;
        putValueBytes(m, a, size, v);
    }
    // The varnodes handed to get and put are elements of the instruction being run: its slots say how to reach each.
    const VarnodeData* vnBase = nullptr;
    const Slot* slotBase = nullptr;
    std::uint64_t get(const VarnodeData& v) {
        const Slot& s = slotBase[&v - vnBase];
        if (s.kind == 2) {
            const std::uint8_t* b = s.ptr;
            switch (s.size) {
                case 1: return b[0];
                case 4: return s.big ? (std::uint64_t{b[0]} << 24 | std::uint64_t{b[1]} << 16 | std::uint64_t{b[2]} << 8 | b[3])
                                     : (std::uint64_t{b[3]} << 24 | std::uint64_t{b[2]} << 16 | std::uint64_t{b[1]} << 8 | b[0]);
                default: {
                    std::uint64_t r = 0;
                    if (s.big)
                        for (unsigned i = 0; i < s.size; ++i) r = (r << 8) | b[i];
                    else
                        for (unsigned i = s.size; i-- > 0;) r = (r << 8) | b[i];
                    return r;
                }
            }
        }
        if (s.kind == 1) return s.value;
        return readSpace(mem[static_cast<std::size_t>(v.space->getIndex())], v.offset, v.size);
    }
    void put(const VarnodeData& v, std::uint64_t value) {
        const Slot& s = slotBase[&v - vnBase];
        if (s.kind == 2) {
            std::uint8_t* b = s.ptr;
            if (s.big)
                for (unsigned i = s.size; i-- > 0; value >>= 8) b[i] = static_cast<std::uint8_t>(value);
            else
                for (unsigned i = 0; i < s.size; ++i, value >>= 8) b[i] = static_cast<std::uint8_t>(value);
            return;
        }
        writeSpace(mem[static_cast<std::size_t>(v.space->getIndex())], v.offset, v.size, value);
    }
    // Slots for a decoded instruction.
    void resolve(Decoded& d) {
        d.slots.assign(d.vn.size(), Slot{});
        for (std::size_t i = 0; i < d.vn.size(); ++i) {
            const VarnodeData& v = d.vn[i];
            Slot& s = d.slots[i];
            if (v.space == constSpace) {
                s.kind = 1;
                s.value = v.offset;
                continue;
            }
            SpaceMem& m = mem[static_cast<std::size_t>(v.space->getIndex())];
            if (m.hooked || m.isCode || v.size == 0 || v.size > 8) continue;
            const std::uint64_t a = m.space->wrapOffset(v.offset) * m.word;
            const std::uint64_t off = a & (kPageSize - 1);
            if (off + v.size > kPageSize) continue;
            Page* p = const_cast<Page*>(m.find(a >> kPageBits));
            if (!p) p = m.make(a >> kPageBits);
            s.kind = 2;
            s.ptr = p->d.data() + off;
            s.size = static_cast<std::uint8_t>(v.size);
            s.big = m.big;
        }
    }

    // ---- registers
    const VarnodeData& regData(const std::string& name) const {
        auto it = regCache.find(name);
        if (it != regCache.end()) return it->second;
        try {
            const VarnodeData& v = trans->getRegister(name);
            return const_cast<Impl*>(this)->regCache.emplace(name, v).first->second;
        } catch (const ghidra::LowlevelError&) {
            throw std::runtime_error("no register " + name);
        }
    }
    Slot pcSlot;   // where the pc register is, found on first use and again after a flush (a memory may have been cleared)
    bool pcSlotValid = false;
    void syncPcRegister() {
        if (!pcReg) return;
        if (!pcSlotValid) {
            pcSlot = Slot{};
            SpaceMem& m = mem[static_cast<std::size_t>(pcReg->space->getIndex())];
            const std::uint64_t a = m.space->wrapOffset(pcReg->offset) * m.word;
            const std::uint64_t off = a & (kPageSize - 1);
            if (!m.hooked && !m.isCode && pcReg->size <= 8 && off + pcReg->size <= kPageSize) {
                Page* p = const_cast<Page*>(m.find(a >> kPageBits));
                if (!p) p = m.make(a >> kPageBits);
                pcSlot.kind = 2;
                pcSlot.ptr = p->d.data() + off;
                pcSlot.size = static_cast<std::uint8_t>(pcReg->size);
                pcSlot.big = m.big;
            }
            pcSlotValid = true;
        }
        if (pcSlot.kind == 2) {
            std::uint64_t value = pcValue;
            std::uint8_t* b = pcSlot.ptr;
            if (pcSlot.big)
                for (unsigned i = pcSlot.size; i-- > 0; value >>= 8) b[i] = static_cast<std::uint8_t>(value);
            else
                for (unsigned i = 0; i < pcSlot.size; ++i, value >>= 8) b[i] = static_cast<std::uint8_t>(value);
            return;
        }
        putValueBytes(mem[static_cast<std::size_t>(pcReg->space->getIndex())], pcReg->offset, pcReg->size, pcValue);
    }

    // ---- decoding
    void flush() {
        caches.clear();
        clearHot();
        retired.clear();
        retiredCount = 0;
        cache = &caches[contextTag];
        trans->forgetParses();
        for (SpaceMem& m : mem)
            for (auto& [k, p] : m.pages) {
                if (!p->code) continue;
                p->code = false;
                p->decoded.reset();
            }
        dirty = false;
        stale.clear();
        pcSlotValid = false;
        if (pinned) pinSlot = directSlot(pinVn);   // (a memory may have been cleared under them)
        if (counterSet) counterSlot = directSlot(counterVn);
        blocks.clear();
    }
    // A byte an instruction was decoded from has changed: that instruction (and any other that reads the byte) is decoded again, and nothing else is. Many changes at
    // once are the same as a flush.
    void noteCodeWrite(std::uint64_t byteAddress) {
        if (stale.size() >= 64) {
            dirty = true;
            return;
        }
        stale.push_back(byteAddress);
    }
    void dropStale() {
        const std::uint64_t unit = mem[static_cast<std::size_t>(codeIndex)].word;
        for (const std::uint64_t byteAddress : stale) {
            const std::uint64_t at = byteAddress / unit;
            for (std::uint64_t s = at >= 16 ? at - 16 : 0; s <= at; ++s)
                for (auto& [tag, decoded] : caches) {
                    const auto it = decoded.find(s);
                    if (it != decoded.end() && s + it->second.length / unit > at) {
                        if (!it->second.raw.empty() && retiredCount < kRetiredLimit) {
                            std::vector<Decoded>& variants = retired[s];
                            if (variants.size() >= 8) {
                                variants.erase(variants.begin());
                                --retiredCount;
                            }
                            variants.push_back(std::move(it->second));
                            ++retiredCount;
                        }
                        if (hotPc[s % kHot] == s) hotEntry[s % kHot] = nullptr;
                        decoded.erase(it);
                    }
                }
        }
        // (no instruction decoded from these bytes is held now: a write to one is of no concern until an instruction is decoded from it again)
        SpaceMem& code = mem[static_cast<std::size_t>(codeIndex)];
        for (const std::uint64_t byteAddress : stale)
            if (Page* p = const_cast<Page*>(code.find(byteAddress >> kPageBits))) p->decoded.reset(static_cast<std::size_t>(byteAddress & (kPageSize - 1)));
        stale.clear();
        if (!blocks.empty()) blocks.clear();
        parsesStale = true;   // (the translator's parse cache is cleared when an instruction is next decoded, if one is)
    }
    // The translator's context cache points into the entries cleared here, so it is told to read them again.
    void dropAddressContext() {
        context.clearChangePoints();
        trans->forgetContextRange();
    }
    const Decoded* decode(std::uint64_t pc) {
        if (dirty) {
            flush();
        } else if (!stale.empty()) {
            dropStale();
        }
        if (hotCache != cache) {
            clearHot();
            hotCache = cache;
        }
        if (const Decoded* hot = hotEntry[pc % kHot]; hot && hotPc[pc % kHot] == pc) return hot;
        auto it = cache->find(pc);
        if (it != cache->end()) {
            hotEntry[pc % kHot] = &it->second;
            hotPc[pc % kHot] = pc;
            return &it->second;
        }
        if (const auto back = retired.find(pc); back != retired.end()) {   // (decoded before from these very bytes)
            const SpaceMem& code = mem[static_cast<std::size_t>(codeIndex)];
            std::vector<Decoded>& variants = back->second;
            for (std::size_t v = variants.size(); v-- > 0;) {
                Decoded& old = variants[v];
                if (old.tag != contextTag) continue;
                bool same = true;
                for (std::size_t i = 0; i < old.raw.size() && same; ++i) same = getByte(code, pc * code.word + i) == old.raw[i];
                if (!same) continue;
                const auto placed = cache->emplace(pc, std::move(old));
                variants.erase(variants.begin() + static_cast<std::ptrdiff_t>(v));
                --retiredCount;
                for (std::size_t i = 0; i < placed.first->second.raw.size(); ++i) {   // (the bytes it reads are decoded-from again)
                    const std::uint64_t a = pc * code.word + i;
                    if (Page* page = const_cast<Page*>(code.find(a >> kPageBits))) {
                        page->code = true;
                        page->decoded.set(static_cast<std::size_t>(a & (kPageSize - 1)));
                    }
                }
                return &placed.first->second;
            }
        }
        if (parsesStale) {
            trans->forgetParses();
            parsesStale = false;
        }
        Decoded d;
        try {
            Collect collect(d);
            d.length = static_cast<std::size_t>(trans->oneInstruction(collect, ghidra::Address(mem[static_cast<std::size_t>(codeIndex)].space, pc * mem[static_cast<std::size_t>(codeIndex)].word)));
        } catch (const ghidra::LowlevelError&) {
            if (!addressContextKept) dropAddressContext();
            return nullptr;
        }
        if (!addressContextKept) dropAddressContext();
        if (d.length == 0) return nullptr;
        resolve(d);
        {
            const SpaceMem& code = mem[static_cast<std::size_t>(codeIndex)];
            d.raw.resize(d.length);
            for (std::size_t i = 0; i < d.length; ++i) d.raw[i] = getByte(code, pc * code.word + i);
            d.tag = contextTag;
        }
        if (cache->size() > (1u << 20)) {
            clearHot();
            cache->clear();
            blocks.clear();
        }
        return &cache->emplace(pc, std::move(d)).first->second;
    }

    // ---- one user operation
    void callOther(const Decoded& d, const Op& op) {
        const VarnodeData* v = &d.vn[op.first];
        const VarnodeData* out = op.hasOut ? v++ : nullptr;
        const std::size_t index = static_cast<std::size_t>(v[0].offset);
        if (index == segmentOp && op.nIn == 3) {   // (x86 real mode does this for every memory operand: no name, no argument list)
            const std::uint64_t segment = get(v[1]), offset = get(v[2]);
            if (out) put(*out, segmentShift ? ((segment & 0xffff) << segmentShift) + (offset & 0xffff) : offset);
            return;
        }
        const std::string& name = index < userOps.size() ? userOps[index] : (unnamedOp = "userop" + std::to_string(index));
        std::vector<std::uint64_t> args;
        for (unsigned i = 1; i < op.nIn; ++i) args.push_back(get(v[i]));
        std::uint64_t result = 0;
        bool served = false;
        if (name == "segment" && args.size() == 2) {
            result = segmentShift ? ((args[0] & 0xffff) << segmentShift) + (args[1] & 0xffff) : args[1];
            served = true;
        } else if (userHook) {
            ++hookCalls;
            served = userHook(*self, name, args, result);
        }
        if (!served) ++unimpl[name];
        if (out) put(*out, served ? result : 0);
    }

    // ---- blocks: a straight run of instructions translated once. The instructions whose p-code is only arithmetic on registers and temporaries become one flat array of
    // operations with their operands already resolved (no cache lookup, no hook check and no pc register write between them); every other instruction is run by execute()
    // as step() runs it. A block ends after an instruction that branches or is run by execute(), and runBlocks() leaves it early whenever something outside could have
    // seen or changed the state (a hook was called, code bytes changed, a stop address was reached), so what the caller observes is what single steps give.
    struct FOp {
        OpCode opc = ghidra::CPUI_COPY;
        std::uint8_t outSize = 0, inSize = 0;
        bool unary = false, branch = false, cbranch = false, absolute = false;
        Slot out, a, c;
        std::int32_t rel = 0;      // a relative branch: the op index it goes to (the count of ops: the instruction's end)
        std::uint64_t target = 0;  // an absolute branch: the pc it goes to
    };
    struct BItem {
        const Decoded* d = nullptr;
        std::uint64_t next = 0;    // the pc after the instruction
        std::uint64_t amount = 0;  // the instruction counter's addition for it
        bool flat = false;
        std::vector<FOp> ops;
    };
    struct Block {
        std::vector<BItem> items;
    };
    std::unordered_map<std::uint64_t, Block> blocks;
    std::uint64_t hookCalls = 0;   // hooks (memory, user operations) called so far: a block ends where one was
    bool pinned = false;           // a register set back to a value after every instruction (an adapter fixing a description's width)
    Slot pinSlot;
    VarnodeData pinVn, counterVn;
    std::uint64_t pinValue = 0;
    bool counterSet = false;       // a register that counts instructions (the Z80 R): added to before each instruction
    Slot counterSlot;
    std::uint64_t counterMask = 0;
    std::function<std::uint64_t(std::uint64_t)> counterAmount;
    static std::uint64_t slotGet(const Slot& s) {
        if (s.kind == 1) return s.value;
        const std::uint8_t* b = s.ptr;
        switch (s.size) {
            case 1: return b[0];
            default: {
                std::uint64_t r = 0;
                if (s.big)
                    for (unsigned i = 0; i < s.size; ++i) r = (r << 8) | b[i];
                else
                    for (unsigned i = s.size; i-- > 0;) r = (r << 8) | b[i];
                return r;
            }
        }
    }
    static void slotPut(const Slot& s, std::uint64_t v) {
        std::uint8_t* b = s.ptr;
        if (s.big)
            for (unsigned i = s.size; i-- > 0; v >>= 8) b[i] = static_cast<std::uint8_t>(v);
        else
            for (unsigned i = 0; i < s.size; ++i, v >>= 8) b[i] = static_cast<std::uint8_t>(v);
    }
    Slot directSlot(const VarnodeData& v) {
        Slot s;
        SpaceMem& m = mem[static_cast<std::size_t>(v.space->getIndex())];
        const std::uint64_t a = m.space->wrapOffset(v.offset) * m.word;
        const std::uint64_t off = a & (kPageSize - 1);
        if (m.hooked || m.isCode || v.size == 0 || v.size > 8 || off + v.size > kPageSize) return s;
        Page* p = const_cast<Page*>(m.find(a >> kPageBits));
        if (!p) p = m.make(a >> kPageBits);
        s.kind = 2;
        s.ptr = p->d.data() + off;
        s.size = static_cast<std::uint8_t>(v.size);
        s.big = m.big;
        return s;
    }
    void applyCounter(std::uint64_t amount) {
        const std::uint64_t old = slotGet(counterSlot);
        slotPut(counterSlot, (old & ~counterMask) | ((old + amount) & counterMask));
    }
    void applyPin() {
        if (pinned && slotGet(pinSlot) != pinValue) slotPut(pinSlot, pinValue);
    }
    bool overlapsPc(const VarnodeData& v) const {
        return pcReg && v.space == pcReg->space && v.offset < pcReg->offset + pcReg->size && pcReg->offset < v.offset + v.size;
    }
    // Translates the instruction into a flat item when all of its p-code is simple; false: execute() runs it.
    bool flatten(const Decoded& d, BItem& item) {
        item.ops.clear();
        for (std::size_t i = 0; i < d.ops.size(); ++i) {
            const Op& op = d.ops[i];
            const std::size_t in0 = op.first + (op.hasOut ? 1 : 0);
            FOp f;
            f.opc = op.opc;
            if (op.opc == ghidra::CPUI_BRANCH || op.opc == ghidra::CPUI_CBRANCH || op.opc == ghidra::CPUI_CALL) {
                if (op.hasOut) return false;
                f.branch = true;
                f.cbranch = op.opc == ghidra::CPUI_CBRANCH;
                if (op.nIn != (f.cbranch ? 2u : 1u)) return false;
                const VarnodeData& t = d.vn[in0];
                if (t.space == constSpace) {
                    const std::int64_t target = static_cast<std::int64_t>(i) + static_cast<std::int32_t>(static_cast<std::uint32_t>(t.offset));
                    if (target < 0 || target > static_cast<std::int64_t>(d.ops.size())) return false;
                    f.rel = static_cast<std::int32_t>(target);
                } else {
                    f.absolute = true;
                    f.target = t.offset / mem[static_cast<std::size_t>(codeIndex)].word;   // (a fixed address in a word-addressed space comes as bytes)
                }
                if (f.cbranch) {
                    f.c = d.slots[in0 + 1];
                    if (f.c.kind == 0 || overlapsPc(d.vn[in0 + 1])) return false;
                }
                item.ops.push_back(f);
                continue;
            }
            std::uint64_t probe;
            const bool unary = op.nIn == 1, binary = op.nIn == 2;
            if (!op.hasOut || !(unary || binary)) return false;
            if (unary ? !fast_unary(op.opc, 1, 1, 0, probe) : !fast_binary(op.opc, 1, 1, 0, 0, probe)) return false;
            f.unary = unary;
            f.out = d.slots[op.first];
            f.a = d.slots[in0];
            f.outSize = static_cast<std::uint8_t>(d.vn[op.first].size);
            f.inSize = static_cast<std::uint8_t>(d.vn[in0].size);
            if (f.out.kind != 2 || f.a.kind == 0 || overlapsPc(d.vn[op.first]) || overlapsPc(d.vn[in0])) return false;
            if (binary) {
                f.c = d.slots[in0 + 1];
                if (f.c.kind == 0 || d.vn[in0 + 1].size > 8 || overlapsPc(d.vn[in0 + 1])) return false;
            }
            item.ops.push_back(f);
        }
        return true;
    }
    static bool controlFlow(const Decoded& d) {
        for (const Op& op : d.ops)
            if (op.opc == ghidra::CPUI_BRANCH || op.opc == ghidra::CPUI_CBRANCH || op.opc == ghidra::CPUI_CALL || op.opc == ghidra::CPUI_BRANCHIND || op.opc == ghidra::CPUI_CALLIND ||
                op.opc == ghidra::CPUI_RETURN)
                return true;
        return false;
    }
    Block* blockAt(std::uint64_t pc) {
        const auto found = blocks.find(pc);
        if (found != blocks.end()) return &found->second;
        if (cache->size() + 32 > (1u << 20)) {   // (decoding below must not clear the instructions this block is pointing at)
            clearHot();
            cache->clear();
            blocks.clear();
        }
        Block block;
        const std::uint64_t unit = mem[static_cast<std::size_t>(codeIndex)].word;
        std::uint64_t at = pc;
        for (int n = 0; n < 32; ++n) {
            const Decoded* d = decode(at);
            if (!d) break;
            BItem item;
            item.d = d;
            const std::uint64_t start = at;
            at += d->length / unit;
            item.next = at;
            item.flat = flatten(*d, item);
            if (counterSet) item.amount = counterAmount(start);
            const bool last = !item.flat || controlFlow(*d);
            block.items.push_back(std::move(item));
            if (last) break;
        }
        return &(blocks[pc] = std::move(block));
    }
    static bool blocksEnabledByEnvironment() {
        static const bool on = [] {
            const char* e = std::getenv("CB_PCODE_BLOCKS");
            return !(e != nullptr && (e[0] == '0' || e[0] == 'n' || e[0] == 'f'));
        }();
        return on;
    }
    // Runs up to maxSteps instructions, as that many step() calls would, but returning early (after the instruction) when a hook was called, code bytes changed or the pc
    // reached stopPc, so the caller sees the state wherever it could have looked.
    Emulator::BlockResult runBlocks(Emulator& self_, std::uint64_t maxSteps, bool useStop, std::uint64_t stopPc);
    bool blocksOnForRun = true;

    Emulator* self = nullptr;

    // ---- one instruction; returns false with `error` set on failure
    bool execute(const Decoded& d) {
        const std::uint64_t unit = mem[static_cast<std::size_t>(codeIndex)].word;
        std::uint64_t next = pcValue + d.length / unit;
        std::size_t i = 0;
        struct RestoreBases {   // (a hook may step the emulator: the instruction it interrupted goes on reading its own operands)
            Impl& o;
            const VarnodeData* vn;
            const Slot* slot;
            ~RestoreBases() {
                o.vnBase = vn;
                o.slotBase = slot;
            }
        } restoreBases{*this, vnBase, slotBase};
        vnBase = d.vn.data();
        slotBase = d.slots.data();
        try {
            while (i < d.ops.size()) {
                const Op& op = d.ops[i];
                const VarnodeData* v = &d.vn[op.first];
                const VarnodeData* out = op.hasOut ? v++ : nullptr;
                std::size_t follow = i + 1;
                bool leave = false;
                auto relative = [&](const VarnodeData& t) {
                    const std::int32_t delta = static_cast<std::int32_t>(static_cast<std::uint32_t>(t.offset));
                    const std::int64_t target = static_cast<std::int64_t>(i) + delta;
                    if (target < 0 || target > static_cast<std::int64_t>(d.ops.size())) throw ghidra::LowlevelError("bad intra-instruction branch");
                    follow = static_cast<std::size_t>(target);
                };
                switch (op.opc) {
                    case ghidra::CPUI_LOAD: {
                        SpaceMem& m = mem[static_cast<std::size_t>(v[0].getSpaceFromConst()->getIndex())];
                        put(*out, readSpace(m, get(v[1]), out->size));
                        break;
                    }
                    case ghidra::CPUI_STORE: {
                        SpaceMem& m = mem[static_cast<std::size_t>(v[0].getSpaceFromConst()->getIndex())];
                        writeSpace(m, get(v[1]), v[2].size, get(v[2]));
                        break;
                    }
                    case ghidra::CPUI_BRANCH:
                    case ghidra::CPUI_CALL:
                        if (v[0].space == constSpace) relative(v[0]);
                        else next = v[0].offset / unit, leave = true;   // (a fixed address in a word-addressed space comes as bytes)
                        break;
                    case ghidra::CPUI_CBRANCH:
                        if (get(v[1]) != 0) {
                            if (v[0].space == constSpace) relative(v[0]);
                            else next = v[0].offset / unit, leave = true;   // (a fixed address in a word-addressed space comes as bytes)
                        }
                        break;
                    case ghidra::CPUI_BRANCHIND:
                    case ghidra::CPUI_CALLIND:
                    case ghidra::CPUI_RETURN:
                        next = mem[static_cast<std::size_t>(codeIndex)].space->wrapOffset(get(v[0]));
                        leave = true;
                        break;
                    case ghidra::CPUI_CALLOTHER:
                        callOther(d, op);
                        break;
                    case ghidra::CPUI_FLOAT_FLOAT2FLOAT:
                        if (out && out->size == 4 && v[0].size == 8) {   // (the host's own conversion: a NaN keeps its payload)
                            put(*out, bits_as<std::uint32_t>(static_cast<float>(bits_as<double>(static_cast<std::uint64_t>(get(v[0]))))));
                            break;
                        }
                        if (out && out->size == 8 && v[0].size == 4) {
                            put(*out, bits_as<std::uint64_t>(static_cast<double>(bits_as<float>(static_cast<std::uint32_t>(get(v[0]))))));
                            break;
                        }
                        {
                            const ghidra::OpBehavior* b = static_cast<std::size_t>(op.opc) < behave.size() ? behave[static_cast<std::size_t>(op.opc)] : nullptr;
                            if (!b || b->isSpecial() || !out) throw ghidra::LowlevelError(std::string("p-code operation without a behaviour: ") + ghidra::get_opname(op.opc));
                            put(*out, b->evaluateUnary(static_cast<ghidra::int4>(out->size), static_cast<ghidra::int4>(v[0].size), get(v[0])));
                        }
                        break;
                    case ghidra::CPUI_FLOAT_ADD: case ghidra::CPUI_FLOAT_SUB: case ghidra::CPUI_FLOAT_MULT: case ghidra::CPUI_FLOAT_DIV:
                    case ghidra::CPUI_FLOAT_NEG: case ghidra::CPUI_FLOAT_ABS: case ghidra::CPUI_FLOAT_SQRT: {
                        // single and double precision on the host's own arithmetic, bit for bit: the description's float model turns every NaN into the default one, a
                        // processor keeps the quiet form of the operand's
                        const bool unary = op.opc == ghidra::CPUI_FLOAT_NEG || op.opc == ghidra::CPUI_FLOAT_ABS || op.opc == ghidra::CPUI_FLOAT_SQRT;
                        const auto compute = [&](auto a, auto c) {
                            switch (op.opc) {
                                case ghidra::CPUI_FLOAT_ADD: return a + c;
                                case ghidra::CPUI_FLOAT_SUB: return a - c;
                                case ghidra::CPUI_FLOAT_MULT: return a * c;
                                case ghidra::CPUI_FLOAT_DIV: return a / c;
                                case ghidra::CPUI_FLOAT_NEG: return -a;
                                case ghidra::CPUI_FLOAT_ABS: {
                                    using Bits = std::conditional_t<sizeof(a) == 8, std::uint64_t, std::uint32_t>;
                                    return bits_as<decltype(a)>(static_cast<Bits>(bits_as<Bits>(a) & ~(Bits{1} << (8 * sizeof(a) - 1))));
                                }
                                default: return static_cast<decltype(a)>(std::sqrt(a));
                            }
                        };
                        if (out && (out->size == 4 || out->size == 8) && v[0].size == out->size) {
                            if (out->size == 8) {
                                const double a = bits_as<double>(static_cast<std::uint64_t>(get(v[0]))), c = unary ? 0.0 : bits_as<double>(static_cast<std::uint64_t>(get(v[1])));
                                put(*out, bits_as<std::uint64_t>(unary ? compute(a, c) : first_nan(a, c, compute(a, c))));
                            } else {
                                const float a = bits_as<float>(static_cast<std::uint32_t>(get(v[0]))), c = unary ? 0.0f : bits_as<float>(static_cast<std::uint32_t>(get(v[1])));
                                put(*out, bits_as<std::uint32_t>(unary ? compute(a, c) : first_nan(a, c, compute(a, c))));
                            }
                            break;
                        }
                        const ghidra::OpBehavior* b = static_cast<std::size_t>(op.opc) < behave.size() ? behave[static_cast<std::size_t>(op.opc)] : nullptr;
                        if (!b || b->isSpecial() || !out) throw ghidra::LowlevelError(std::string("p-code operation without a behaviour: ") + ghidra::get_opname(op.opc));
                        if (b->isUnary()) put(*out, b->evaluateUnary(static_cast<ghidra::int4>(out->size), static_cast<ghidra::int4>(v[0].size), get(v[0])));
                        else put(*out, b->evaluateBinary(static_cast<ghidra::int4>(out->size), static_cast<ghidra::int4>(v[0].size), get(v[0]), get(v[1])));
                        break;
                    }
                    default: {
                        if (out && out->size <= 8 && v[0].size <= 8) {   // (the common operations are written out, as blocks run them, rather than reached through the behaviour table)
                            std::uint64_t fast;
                            if (op.nIn == 1 && isFastUnary(op.opc)) {
                                fast_unary(op.opc, static_cast<int>(out->size), static_cast<int>(v[0].size), get(v[0]), fast);
                                put(*out, fast);
                                break;
                            }
                            if (op.nIn == 2 && v[1].size <= 8 && isFastBinary(op.opc)) {
                                const std::uint64_t a = get(v[0]);
                                const std::uint64_t c = get(v[1]);
                                fast_binary(op.opc, static_cast<int>(out->size), static_cast<int>(v[0].size), a, c, fast);
                                put(*out, fast);
                                break;
                            }
                        }
                        const ghidra::OpBehavior* b = static_cast<std::size_t>(op.opc) < behave.size() ? behave[static_cast<std::size_t>(op.opc)] : nullptr;
                        if (!b || b->isSpecial() || !out) throw ghidra::LowlevelError(std::string("p-code operation without a behaviour: ") + ghidra::get_opname(op.opc));
                        if (b->isUnary()) {
                            put(*out, b->evaluateUnary(static_cast<ghidra::int4>(out->size), static_cast<ghidra::int4>(v[0].size), get(v[0])));
                        } else {
                            const std::uint64_t a = get(v[0]);
                            const std::uint64_t c = get(v[1]);
                            put(*out, b->evaluateBinary(static_cast<ghidra::int4>(out->size), static_cast<ghidra::int4>(v[0].size), a, c));
                        }
                        break;
                    }
                }
                if (leave) break;
                i = follow;
            }
        } catch (const ghidra::LowlevelError& e) {
            error = e.explain;
            return false;
        } catch (const std::exception& e) {
            error = e.what();
            return false;
        }
        pcValue = next;
        return true;
    }
};

Emulator::Emulator(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context) : impl_(std::make_unique<Impl>()) {
    initialize_once();
    Impl& o = *impl_;
    o.self = this;
    if (!std::filesystem::is_regular_file(sla)) throw std::runtime_error("no processor description " + sla.string());
    o.trans = std::make_unique<ghidra::Sleigh>(&o.image, &o.context);
    try {
        std::istringstream name("<sleigh>" + sla.generic_string() + "</sleigh>");
        ghidra::Element* root = o.store.parseDocument(name)->getRoot();
        o.store.registerTag(root);
        o.trans->initialize(o.store);
        for (const auto& [variable, value] : context) o.context.setVariableDefault(variable, static_cast<ghidra::uintm>(value));
        o.trans->setDefaultFloatFormats();
        ghidra::OpBehavior::registerInstructions(o.behave, o.trans.get());
        o.trans->getUserOpNames(o.userOps);
        for (std::size_t i = 0; i < o.userOps.size(); ++i)
            if (o.userOps[i] == "segment") o.segmentOp = i;
    } catch (const ghidra::LowlevelError& e) {
        throw std::runtime_error(sla.string() + ": " + e.explain);
    }
    for (const auto& [variable, value] : context)
        if (variable == "addrsize" && value == 0) o.segmentShift = 4;   // x86 real mode: segment * 16 + offset
    o.constSpace = o.trans->getConstantSpace();
    const int n = o.trans->numSpaces();
    o.mem = std::vector<SpaceMem>(static_cast<std::size_t>(n));   // (not resize(): a space's page map is moved only by copying)
    o.codeIndex = o.trans->getDefaultCodeSpace()->getIndex();
    for (int i = 0; i < n; ++i) {
        AddrSpace* s = o.trans->getSpace(i);
        if (!s) continue;
        SpaceMem& m = o.mem[static_cast<std::size_t>(i)];
        m.space = s;
        m.word = s->getType() == ghidra::IPTR_PROCESSOR ? static_cast<unsigned>(s->getWordSize()) : 1;
        m.big = s->isBigEndian();
        m.isCode = i == o.codeIndex;
        if (s->getName() == "register") o.regIndex = i;
        m.hooked = s->getType() == ghidra::IPTR_PROCESSOR && s->getName() != "register";
    }
    for (const char* name : {"PC", "pc", "EIP", "RIP"}) {
        try {
            o.pcRegData = o.trans->getRegister(name);
            o.pcReg = &o.pcRegData;
            break;
        } catch (const ghidra::LowlevelError&) {
        }
    }
}

Emulator::~Emulator() = default;

void Emulator::load(std::uint64_t addr, const std::uint8_t* data, std::size_t size, std::string_view space) { write(addr, data, size, space); }

void Emulator::write(std::uint64_t addr, const std::uint8_t* data, std::size_t size, std::string_view space) {
    Impl& o = *impl_;
    SpaceMem& m = o.space(space);
    // a page at a time: a page that does not exist and would only get zeros is not made (a 16 MB memory image of a console is mostly that)
    for (std::size_t i = 0; i < size;) {
        const std::uint64_t at = addr + i;
        const std::size_t in = static_cast<std::size_t>(at & (kPageSize - 1));
        const std::size_t n = std::min<std::size_t>(size - i, static_cast<std::size_t>(kPageSize) - in);
        Page* p = const_cast<Page*>(m.find(at >> kPageBits));
        if (!p) {
            std::size_t z = 0;
            while (z < n && data[i + z] == 0) ++z;
            if (z == n) {
                i += n;
                continue;
            }
            p = m.make(at >> kPageBits);
        }
        if (p->code)   // (only a byte that changes, in an instruction decoded from it, costs that instruction)
            for (std::size_t k = 0; k < n; ++k)
                if (p->d[in + k] != data[i + k] && p->decoded[in + k]) o.noteCodeWrite(at + k);
        std::memcpy(p->d.data() + in, data + i, n);
        i += n;
    }
}

void Emulator::clearMemory(std::string_view space) {
    SpaceMem& m = impl_->space(space);
    m.pages.clear();
    m.last = nullptr;
    m.lastKey = ~std::uint64_t{0};
    impl_->flush();
}

void Emulator::read(std::uint64_t addr, std::uint8_t* data, std::size_t size, std::string_view space) const {
    const SpaceMem& m = impl_->space(space);
    // a page at a time; a page the map does not hold is the source's (not made) or zeros
    for (std::size_t i = 0; i < size;) {
        const std::uint64_t at = addr + i;
        const std::size_t in = static_cast<std::size_t>(at & (kPageSize - 1));
        const std::size_t n = std::min<std::size_t>(size - i, static_cast<std::size_t>(kPageSize) - in);
        if (const Page* p = m.findHeld(at >> kPageBits)) std::memcpy(data + i, p->d.data() + in, n);
        else if (!m.source || !m.source(at, data + i, n)) std::memset(data + i, 0, n);
        i += n;
    }
}

void Emulator::setPageSource(PageSource source, std::string_view space) {
    SpaceMem& m = impl_->space(space);
    m.source = std::move(source);
    m.last = nullptr;
    m.lastKey = ~std::uint64_t{0};
}

void Emulator::writeHeld(std::uint64_t addr, const std::uint8_t* data, std::size_t size, std::string_view space) {
    Impl& o = *impl_;
    SpaceMem& m = o.space(space);
    for (std::size_t i = 0; i < size;) {
        const std::uint64_t at = addr + i;
        const std::size_t in = static_cast<std::size_t>(at & (kPageSize - 1));
        const std::size_t n = std::min<std::size_t>(size - i, static_cast<std::size_t>(kPageSize) - in);
        if (Page* p = const_cast<Page*>(m.findHeld(at >> kPageBits))) {
            if (p->code)
                for (std::size_t k = 0; k < n; ++k)
                    if (p->d[in + k] != data[i + k] && p->decoded[in + k]) o.noteCodeWrite(at + k);
            std::memcpy(p->d.data() + in, data + i, n);
        }
        i += n;
    }
}

std::uint64_t Emulator::readValue(std::uint64_t addr, unsigned size, std::string_view space) const {
    return impl_->getValueBytes(impl_->space(space), addr, size);
}

void Emulator::writeValue(std::uint64_t addr, unsigned size, std::uint64_t value, std::string_view space) {
    impl_->putValueBytes(impl_->space(space), addr, size, value);
}

void Emulator::onRead(ReadHook hook) { impl_->readHook = std::move(hook); }
void Emulator::onWrite(WriteHook hook) { impl_->writeHook = std::move(hook); }

bool Emulator::hasReg(const std::string& name) const {
    try {
        impl_->regData(name);
        return true;
    } catch (const std::runtime_error&) {
        return false;
    }
}

std::uint64_t Emulator::reg(const std::string& name) const {
    const VarnodeData& v = impl_->regData(name);
    return impl_->getValueBytes(impl_->mem[static_cast<std::size_t>(v.space->getIndex())], v.offset, v.size);
}

void Emulator::setReg(const std::string& name, std::uint64_t value) {
    const VarnodeData& v = impl_->regData(name);
    impl_->putValueBytes(impl_->mem[static_cast<std::size_t>(v.space->getIndex())], v.offset, v.size, value);
}

Emulator::RegRef Emulator::regRef(const std::string& name) const {
    const VarnodeData& v = impl_->regData(name);
    return {static_cast<std::size_t>(v.space->getIndex()), v.offset, static_cast<unsigned>(v.size)};
}

std::uint64_t Emulator::reg(const RegRef& ref) const { return impl_->getValueBytes(impl_->mem[ref.space], ref.offset, ref.size); }

void Emulator::setReg(const RegRef& ref, std::uint64_t value) { impl_->putValueBytes(impl_->mem[ref.space], ref.offset, ref.size, value); }

void Emulator::readRegs(const RegRef& from, std::uint8_t* out, std::size_t size) const {
    const SpaceMem& m = impl_->mem[from.space];
    for (std::size_t i = 0; i < size;) {
        const std::uint64_t at = from.offset + i;
        const std::size_t in = static_cast<std::size_t>(at & (kPageSize - 1));
        const std::size_t n = std::min<std::size_t>(size - i, static_cast<std::size_t>(kPageSize) - in);
        if (const Page* p = m.findHeld(at >> kPageBits)) std::memcpy(out + i, p->d.data() + in, n);
        else std::memset(out + i, 0, n);
        i += n;
    }
}

void Emulator::writeRegs(const RegRef& to, const std::uint8_t* data, std::size_t size) {
    SpaceMem& m = impl_->mem[to.space];
    for (std::size_t i = 0; i < size;) {
        const std::uint64_t at = to.offset + i;
        const std::size_t in = static_cast<std::size_t>(at & (kPageSize - 1));
        const std::size_t n = std::min<std::size_t>(size - i, static_cast<std::size_t>(kPageSize) - in);
        Page* p = const_cast<Page*>(m.findHeld(at >> kPageBits));
        if (!p) p = m.make(at >> kPageBits);
        std::memcpy(p->d.data() + in, data + i, n);
        i += n;
    }
}

const std::vector<Emulator::Register>& Emulator::registers(bool parts) const {
    Impl& o = *impl_;
    if (parts) {
        if (o.allRegs.empty()) {
            std::map<VarnodeData, std::string> every;
            o.trans->getAllRegisters(every);
            for (const auto& [v, name] : every)
                if (v.size <= 8 && name.rfind("contextreg", 0) != 0) o.allRegs.push_back({name, v.size});
        }
        return o.allRegs;
    }
    if (o.topRegsDone) return o.topRegs;
    o.topRegsDone = true;
    std::map<VarnodeData, std::string> all;
    o.trans->getAllRegisters(all);
    for (const auto& [v, name] : all) {
        if (v.size > 8 || name.rfind("contextreg", 0) == 0) continue;
        bool part = false;
        for (const auto& [w, other] : all) {
            if (w.space != v.space || w.size <= v.size) continue;
            if (w.offset <= v.offset && v.offset + v.size <= w.offset + w.size) {
                part = true;
                break;
            }
        }
        if (!part) o.topRegs.push_back({name, v.size});
    }
    return o.topRegs;
}

std::uint64_t Emulator::pc() const { return impl_->pcValue; }
void Emulator::setPc(std::uint64_t pc) {
    impl_->pcValue = pc;
    impl_->syncPcRegister();
}


Emulator::Status Emulator::step() {
    Impl& o = *impl_;
    const Decoded* d = o.decode(o.pcValue);
    if (!d) {
        const std::uint64_t at = o.pcValue;
        if (o.badHook) {
            o.syncPcRegister();
            if (o.badHook(*this, at)) {
                ++o.stepCount;
                o.syncPcRegister();
                if (o.trace) o.trace(at, *this);
                return Status::Ok;
            }
        }
        o.error = "no instruction at " + std::to_string(o.pcValue);
        return Status::BadInstruction;
    }
    o.syncPcRegister();   // (a caller may have cleared or set the registers since the last step)
    const std::uint64_t at = o.pcValue;
    if (!o.execute(*d)) return Status::Error;
    ++o.stepCount;
    o.syncPcRegister();
    if (o.trace) o.trace(at, *this);
    return Status::Ok;
}

void Emulator::setBlocksOverride(int mode) { g_blocks_override = mode; }

bool Emulator::evaluateOp(unsigned opcode, int sizeOut, int sizeIn, std::uint64_t a, std::uint64_t c, std::uint64_t& result, bool writtenOut) const {
    const OpCode opc = static_cast<OpCode>(opcode);
    const bool unary = isFastUnary(opc);
    if (writtenOut) return unary ? fast_unary(opc, sizeOut, sizeIn, a, result) : fast_binary(opc, sizeOut, sizeIn, a, c, result);
    const ghidra::OpBehavior* b = opcode < impl_->behave.size() ? impl_->behave[opcode] : nullptr;
    if (!b) return false;
    result = unary ? b->evaluateUnary(sizeOut, sizeIn, a) : b->evaluateBinary(sizeOut, sizeIn, a, c);
    return true;
}

void Emulator::setPin(const RegRef& ref, std::uint64_t value) {
    Impl& o = *impl_;
    ghidra::VarnodeData v;
    v.space = o.mem[ref.space].space;
    v.offset = ref.offset;
    v.size = ref.size;
    o.pinVn = v;
    o.pinSlot = o.directSlot(v);
    o.pinned = o.pinSlot.kind == 2;
    o.pinValue = value;
}

void Emulator::clearPin() { impl_->pinned = false; }

void Emulator::setCounter(const RegRef& ref, std::uint64_t mask, std::function<std::uint64_t(std::uint64_t pc)> amount) {
    Impl& o = *impl_;
    ghidra::VarnodeData v;
    v.space = o.mem[ref.space].space;
    v.offset = ref.offset;
    v.size = ref.size;
    o.counterVn = v;
    o.counterSlot = o.directSlot(v);
    o.counterSet = o.counterSlot.kind == 2;
    o.counterMask = mask;
    o.counterAmount = std::move(amount);
    o.blocks.clear();   // (the amounts are part of the blocks)
}

void Emulator::clearCounter() {
    impl_->counterSet = false;
    impl_->blocks.clear();
}

Emulator::BlockResult Emulator::runBlocks(std::uint64_t maxSteps, bool useStop, std::uint64_t stopPc) {
    Impl& o = *impl_;
    BlockResult r;
    const bool blocksOn = !o.trace && o.stops.empty() && (g_blocks_override.load() < 0 ? Impl::blocksEnabledByEnvironment() : g_blocks_override.load() != 0);
    // one instruction, as the caller would have run it with step()
    const auto single = [&]() -> bool {
        if (o.counterSet) o.applyCounter(o.counterAmount(o.pcValue));
        const std::uint64_t hooks = o.hookCalls;
        const Status status = step();
        if (status != Status::Ok) {
            r.status = status;
            return false;
        }
        ++r.executed;
        o.applyPin();
        return !(useStop && o.pcValue == stopPc) && o.hookCalls == hooks;
    };
    if (!blocksOn) {
        if (r.executed < maxSteps) single();
        return r;
    }
    while (r.executed < maxSteps) {
        if (o.dirty) o.flush();
        else if (!o.stale.empty()) o.dropStale();
        Impl::Block* block = o.blockAt(o.pcValue);
        if (block->items.empty()) {   // (nothing decodes here: step() says what happens, including a bad-instruction hook)
            single();
            return r;
        }
        o.syncPcRegister();   // (a caller may have cleared or set the registers since the last step)
        for (const Impl::BItem& item : block->items) {
            if (r.executed >= maxSteps) {
                o.syncPcRegister();
                return r;
            }
            if (o.counterSet) o.applyCounter(item.amount);
            const std::uint64_t hooks = o.hookCalls;
            if (item.flat) {
                std::uint64_t next = item.next;
                const Impl::FOp* ops = item.ops.data();
                const std::size_t count = item.ops.size();
                std::size_t i = 0;
                while (i < count) {
                    const Impl::FOp& f = ops[i];
                    if (f.branch) {
                        if (f.cbranch && Impl::slotGet(f.c) == 0) {
                            ++i;
                            continue;
                        }
                        if (f.absolute) {
                            next = f.target;
                            break;
                        }
                        i = static_cast<std::size_t>(f.rel);
                        continue;
                    }
                    std::uint64_t result = 0;
                    if (f.unary) fast_unary(f.opc, f.outSize, f.inSize, Impl::slotGet(f.a), result);
                    else fast_binary(f.opc, f.outSize, f.inSize, Impl::slotGet(f.a), Impl::slotGet(f.c), result);
                    Impl::slotPut(f.out, result);
                    ++i;
                }
                o.pcValue = next;
                ++o.stepCount;
            } else {
                o.syncPcRegister();
                if (!o.execute(*item.d)) {
                    r.status = Status::Error;
                    return r;
                }
                ++o.stepCount;
                o.syncPcRegister();
            }
            ++r.executed;
            o.applyPin();
            if ((useStop && o.pcValue == stopPc) || o.hookCalls != hooks || o.dirty || !o.stale.empty()) {
                o.syncPcRegister();
                return r;
            }
        }
        o.syncPcRegister();
    }
    return r;
}

Emulator::RunResult Emulator::run(std::uint64_t n) {
    RunResult r;
    for (; r.steps < n; ++r.steps) {
        if (r.steps != 0 && impl_->stops.count(impl_->pcValue)) {
            r.status = Status::Stopped;
            return r;
        }
        const Status s = step();
        if (s != Status::Ok) {
            r.status = s;
            return r;
        }
    }
    return r;
}

void Emulator::addStop(std::uint64_t pc) { impl_->stops.insert(pc); }
void Emulator::clearStops() { impl_->stops.clear(); }
std::uint64_t Emulator::steps() const { return impl_->stepCount; }
const std::string& Emulator::lastError() const { return impl_->error; }
void Emulator::onTrace(Trace trace) { impl_->trace = std::move(trace); }
void Emulator::onUserOp(UserOpHook hook) { impl_->userHook = std::move(hook); }
void Emulator::onBadInstruction(BadInstructionHook hook) { impl_->badHook = std::move(hook); }
std::uint64_t Emulator::busRead(std::uint64_t addr, unsigned size, std::string_view space) {
    SpaceMem& m = impl_->space(space);
    return impl_->readSpace(m, addr / m.word, size);
}
void Emulator::busWrite(std::uint64_t addr, unsigned size, std::uint64_t value, std::string_view space) {
    SpaceMem& m = impl_->space(space);
    impl_->writeSpace(m, addr / m.word, size, value);
}
const std::map<std::string, std::uint64_t>& Emulator::unimplementedOps() const { return impl_->unimpl; }

std::string Emulator::text(std::uint64_t pc) const {
    Impl& o = *impl_;
    try {
        if (o.parsesStale) {
            o.trans->forgetParses();
            o.parsesStale = false;
        }
        Text t;
        o.trans->printAssembly(t, ghidra::Address(o.mem[static_cast<std::size_t>(o.codeIndex)].space, pc * o.mem[static_cast<std::size_t>(o.codeIndex)].word));
        return t.text;
    } catch (const ghidra::LowlevelError&) {
        return {};
    }
}

std::size_t Emulator::length(std::uint64_t pc) const {
    const Decoded* d = impl_->decode(pc);
    return d ? d->length : 0;
}

void Emulator::setContext(const std::string& name, int value) {
    Impl& o = *impl_;
    o.trans->setContextDefault(name, static_cast<ghidra::uintm>(value));
    o.parsesStale = true;   // (an instruction already decoded is in the emulator's own cache, per context: only a decode that misses needs the translator's cache clear)
    // the tag of the contexts decoding now runs under: the variables set so far and their values
    o.contextSet[name] = value;
    std::uint64_t tag = 1469598103934665603ull;
    for (const auto& [variable, set_to] : o.contextSet) {
        for (const char ch : variable) tag = (tag ^ static_cast<unsigned char>(ch)) * 1099511628211ull;
        tag = (tag ^ static_cast<std::uint32_t>(set_to)) * 1099511628211ull;
    }
    if (tag != o.contextTag) o.blocks.clear();   // (a block holds instructions decoded under one context)
    o.contextTag = tag;
    o.cache = &o.caches[tag];
}

void Emulator::keepAddressContext(bool keep) {
    if (impl_->addressContextKept == keep) return;   // (what was decoded was decoded under this setting already)
    impl_->addressContextKept = keep;
    if (!keep) impl_->dropAddressContext();
    impl_->flush();
}

void Emulator::flushDecoded() { impl_->flush(); }

}  // namespace cb::decompile::sleigh
