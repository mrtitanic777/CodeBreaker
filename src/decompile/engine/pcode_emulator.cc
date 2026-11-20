// decompile/engine/pcode_emulator.cc -- see pcode_emulator.hh. Built with the core (its own flags), so the core's headers stay here.
//
// The interpreter is its own loop over the p-code the translator emits for each instruction (cached by address, dropped when a
// write lands on a page the code was read from): the core's EmulatePcodeCache decodes again for every instruction, fails half-way
// when the next bytes are no instruction, and its memory banks cannot take byte-exact hooks. The arithmetic itself is the core's
// own OpBehavior, so an operation means here what it means in the decompiler.

#include "decompile/engine/pcode_emulator.hh"

#include <array>
#include <cstring>
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

constexpr std::uint64_t kPageBits = 12;
constexpr std::uint64_t kPageSize = std::uint64_t{1} << kPageBits;

struct Page {
    std::array<std::uint8_t, kPageSize> d{};
    bool code = false;   // instructions were decoded from this page
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

struct Decoded {
    std::size_t length = 0;   // bytes
    std::vector<Op> ops;
    std::vector<VarnodeData> vn;
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
                if (p) p->code = true;
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

    std::unordered_map<std::uint64_t, Decoded> cache;
    bool dirty = false;                 // a write hit decoded code: flush before the next step
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
        if (p->code) dirty = true;
        p->d[a & (kPageSize - 1)] = v;
    }
    std::uint64_t getValueBytes(const SpaceMem& m, std::uint64_t a, unsigned size) const {
        std::uint64_t v = 0;
        const unsigned n = size > 8 ? 8 : size;
        if (m.big) {
            // the low bytes are the last ones
            for (unsigned i = 0; i < n; ++i) v = (v << 8) | getByte(m, a + size - n + i);
        } else {
            for (unsigned i = n; i-- > 0;) v = (v << 8) | getByte(m, a + i);
        }
        return v;
    }
    void putValueBytes(SpaceMem& m, std::uint64_t a, unsigned size, std::uint64_t v) {
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
    std::uint64_t get(const VarnodeData& v) {
        if (v.space == constSpace) return v.offset;
        return readSpace(mem[static_cast<std::size_t>(v.space->getIndex())], v.offset, v.size);
    }
    void put(const VarnodeData& v, std::uint64_t value) {
        writeSpace(mem[static_cast<std::size_t>(v.space->getIndex())], v.offset, v.size, value);
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
    void syncPcRegister() {
        if (pcReg) putValueBytes(mem[static_cast<std::size_t>(pcReg->space->getIndex())], pcReg->offset, pcReg->size, pcValue);
    }

    // ---- decoding
    void flush() {
        cache.clear();
        trans->forgetParses();
        for (SpaceMem& m : mem)
            for (auto& [k, p] : m.pages) p->code = false;
        dirty = false;
    }
    const Decoded* decode(std::uint64_t pc) {
        if (dirty) flush();
        auto it = cache.find(pc);
        if (it != cache.end()) return &it->second;
        Decoded d;
        try {
            Collect collect(d);
            d.length = static_cast<std::size_t>(trans->oneInstruction(collect, ghidra::Address(mem[static_cast<std::size_t>(codeIndex)].space, pc)));
        } catch (const ghidra::LowlevelError&) {
            return nullptr;
        }
        if (d.length == 0) return nullptr;
        if (cache.size() > (1u << 20)) cache.clear();
        return &cache.emplace(pc, std::move(d)).first->second;
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
    SpaceMem& m = impl_->space(space);
    for (std::size_t i = 0; i < size; ++i) impl_->putByte(m, addr + i, data[i]);
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
    o.syncPcRegister();
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
    impl_->trans->setContextDefault(name, static_cast<ghidra::uintm>(value));
    impl_->flush();
}

void Emulator::flushDecoded() { impl_->flush(); }

}  // namespace cb::decompile::sleigh
