// decompile/engine/pcode_emulator.cc -- see pcode_emulator.hh. Built with the core (its own flags), so the core's headers stay here.
//
// The interpreter is its own loop over the p-code the translator emits for each instruction (cached by address, dropped when a
// write lands on a page the code was read from): the core's EmulatePcodeCache decodes again for every instruction, fails half-way
// when the next bytes are no instruction, and its memory banks cannot take byte-exact hooks. The arithmetic itself is the core's
// own OpBehavior, so an operation means here what it means in the decompiler.

#include "decompile/engine/pcode_emulator.hh"

#include <algorithm>
#include <array>
#include <bitset>
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

constexpr std::uint64_t kPageBits = 12;
constexpr std::uint64_t kPageSize = std::uint64_t{1} << kPageBits;

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
    std::unordered_map<std::uint64_t, std::unique_ptr<Page>> pages;
    mutable const Page* last = nullptr;
    mutable std::uint64_t lastKey = ~std::uint64_t{0};

    const Page* find(std::uint64_t key) const {
        if (key == lastKey) return last;
        auto it = pages.find(key);
        const Page* p = it == pages.end() ? nullptr : it->second.get();
        if (p) lastKey = key, last = p;
        return p;
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

}  // namespace

struct Emulator::Impl {
    // The bytes the translator decodes: the code space's pages.
    class Image : public ghidra::LoadImage {
    public:
        explicit Image(Impl& owner) : LoadImage("codebreaker"), o(owner) {}
        void loadFill(ghidra::uint1* ptr, ghidra::int4 size, const ghidra::Address& addr) override {
            SpaceMem& m = o.mem[o.codeIndex];
            const std::uint64_t start = addr.getOffset() * m.word;
            for (ghidra::int4 i = 0; i < size; ++i) {
                const std::uint64_t a = start + static_cast<std::uint64_t>(i);
                Page* p = const_cast<Page*>(m.find(a >> kPageBits));
                if (p) {
                    p->code = true;
                    p->decoded.set(a & (kPageSize - 1));
                }
                ptr[i] = p ? p->d[a & (kPageSize - 1)] : 0;
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
            if (readHook(m.space->getName(), a, size, value)) return size >= 8 ? value : value & ((std::uint64_t{1} << (8 * size)) - 1);
        }
        return getValueBytes(m, a, size);
    }
    void writeSpace(SpaceMem& m, std::uint64_t unitOff, unsigned size, std::uint64_t v) {
        const std::uint64_t a = m.space->wrapOffset(unitOff) * m.word;
        if (m.hooked && writeHook && writeHook(m.space->getName(), a, size, size >= 8 ? v : v & ((std::uint64_t{1} << (8 * size)) - 1))) return;
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
                    if (it != decoded.end() && s + it->second.length / unit > at) decoded.erase(it);
                }
        }
        stale.clear();
        trans->forgetParses();
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
        auto it = cache->find(pc);
        if (it != cache->end()) return &it->second;
        Decoded d;
        try {
            Collect collect(d);
            d.length = static_cast<std::size_t>(trans->oneInstruction(collect, ghidra::Address(mem[static_cast<std::size_t>(codeIndex)].space, pc)));
        } catch (const ghidra::LowlevelError&) {
            if (!addressContextKept) dropAddressContext();
            return nullptr;
        }
        if (!addressContextKept) dropAddressContext();
        if (d.length == 0) return nullptr;
        resolve(d);
        if (cache->size() > (1u << 20)) cache->clear();
        return &cache->emplace(pc, std::move(d)).first->second;
    }

    // ---- one user operation
    void callOther(const Decoded& d, const Op& op) {
        const VarnodeData* v = &d.vn[op.first];
        const VarnodeData* out = op.hasOut ? v++ : nullptr;
        const std::size_t index = static_cast<std::size_t>(v[0].offset);
        const std::string name = index < userOps.size() ? userOps[index] : "userop" + std::to_string(index);
        std::vector<std::uint64_t> args;
        for (unsigned i = 1; i < op.nIn; ++i) args.push_back(get(v[i]));
        std::uint64_t result = 0;
        bool served = false;
        if (name == "segment" && args.size() == 2) {
            result = segmentShift ? ((args[0] & 0xffff) << segmentShift) + (args[1] & 0xffff) : args[1];
            served = true;
        } else if (userHook) {
            served = userHook(*self, name, args, result);
        }
        if (!served) ++unimpl[name];
        if (out) put(*out, served ? result : 0);
    }

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
                        else next = v[0].offset, leave = true;
                        break;
                    case ghidra::CPUI_CBRANCH:
                        if (get(v[1]) != 0) {
                            if (v[0].space == constSpace) relative(v[0]);
                            else next = v[0].offset, leave = true;
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
    for (std::size_t i = 0; i < size; ++i) data[i] = Impl::getByte(m, addr + i);
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
        Text t;
        o.trans->printAssembly(t, ghidra::Address(o.mem[static_cast<std::size_t>(o.codeIndex)].space, pc));
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
    o.trans->forgetParses();
    // the tag of the contexts decoding now runs under: the variables set so far and their values
    o.contextSet[name] = value;
    std::uint64_t tag = 1469598103934665603ull;
    for (const auto& [variable, set_to] : o.contextSet) {
        for (const char ch : variable) tag = (tag ^ static_cast<unsigned char>(ch)) * 1099511628211ull;
        tag = (tag ^ static_cast<std::uint32_t>(set_to)) * 1099511628211ull;
    }
    o.contextTag = tag;
    o.cache = &o.caches[tag];
}

void Emulator::keepAddressContext(bool keep) {
    impl_->addressContextKept = keep;
    if (!keep) impl_->dropAddressContext();
    impl_->flush();
}

void Emulator::flushDecoded() { impl_->flush(); }

}  // namespace cb::decompile::sleigh
