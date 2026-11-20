// decompile/engine/sleigh_reader.cc -- see sleigh_reader.hh. Built with the core (its own flags), so the core's headers stay here.

#include "decompile/engine/sleigh_reader.hh"

#include <algorithm>
#include <array>
#include <unordered_map>
#include <mutex>
#include <sstream>
#include <stdexcept>

#include "decompile/engine/core/cpp/loadimage.hh"
#include "decompile/engine/core/cpp/sleigh.hh"

namespace cb::decompile::sleigh {
namespace {

// The bytes being read, as the translator asks for them (zero outside the window).
class Window : public ghidra::LoadImage {
public:
    Window() : LoadImage("codebreaker") {}
    void set(const std::uint8_t* data, std::size_t size, std::uint64_t base) {
        data_ = data;
        size_ = size;
        base_ = base;
    }
    void loadFill(ghidra::uint1* ptr, ghidra::int4 size, const ghidra::Address& addr) override {
        const std::uint64_t start = addr.getOffset();
        for (ghidra::int4 i = 0; i < size; ++i) {
            const std::uint64_t at = start + static_cast<std::uint64_t>(i);
            ptr[i] = at >= base_ && at - base_ < size_ ? data_[at - base_] : 0;
        }
    }
    std::string getArchType() const override { return "codebreaker"; }
    void adjustVma(long) override {}

private:
    const std::uint8_t* data_ = nullptr;
    std::size_t size_ = 0;
    std::uint64_t base_ = 0;
};

class Text : public ghidra::AssemblyEmit {
public:
    void dump(const ghidra::Address&, const std::string& mnem, const std::string& body) override {
        mnemonic = mnem;
        operands = body;
    }
    std::string mnemonic, operands;
};

// The way out of an instruction, from its p-code.
class FlowOf : public ghidra::PcodeEmit {
public:
    FlowOf(std::uint64_t here, std::uint64_t next) : here_(here), next_(next) {}
    void dump(const ghidra::Address&, ghidra::OpCode op, ghidra::VarnodeData*, ghidra::VarnodeData* vars, ghidra::int4) override {
        // A conditional branch to the instruction itself (a repeat: LDIR, REP MOVS) or to the next one, with more p-code after it (ARM's
        // condition on any instruction, a conditional RET's test), is no way out: it makes the way out after it conditional. With
        // nothing after it, it is the instruction's own branch (JR NZ to the next instruction). A branch to a p-code label is inside.
        if (pending_) {   // (something follows the conditional branch to here or next: it was a test)
            pending_ = false;
            conditional = true;
            flow = Flow::Next, target.reset();
        }
        const bool label = vars != nullptr && vars[0].space->getType() == ghidra::IPTR_CONSTANT;
        switch (op) {
            case ghidra::CPUI_BRANCH:
                if (!label) set(Flow::Jump, vars[0].offset);
                break;
            case ghidra::CPUI_CBRANCH:
                if (label) conditional = true;
                else {
                    set(Flow::Branch, vars[0].offset);
                    if (vars[0].offset == here_ || vars[0].offset == next_) pending_ = true;
                }
                break;
            case ghidra::CPUI_CALL: set(Flow::Call, vars[0].offset); break;
            case ghidra::CPUI_CALLIND: set(Flow::CallIndirect, std::nullopt); break;
            case ghidra::CPUI_RETURN: set(Flow::Return, std::nullopt); break;
            case ghidra::CPUI_BRANCHIND: set(Flow::Indirect, std::nullopt); break;
            default: break;
        }
    }
    Flow result() const {
        // a return or jump that a test inside the instruction may skip falls through to the next instruction
        if (conditional && (flow == Flow::Return || flow == Flow::Indirect)) return Flow::Next;
        if (conditional && flow == Flow::Jump) return Flow::Branch;
        return flow;
    }
    Flow flow = Flow::Next;
    std::optional<std::uint64_t> target;
    bool conditional = false;

private:
    void set(Flow f, std::optional<std::uint64_t> t) {
        flow = f;
        target = t;
    }
    std::uint64_t here_, next_;
    bool pending_ = false;
};

void initialize_once() {
    static std::once_flag once;
    std::call_once(once, [] {
        ghidra::AttributeId::initialize();
        ghidra::ElementId::initialize();
    });
}

}  // namespace

struct Reader::Impl {
    mutable Window window;
    // at(): the bytes each kept parse was made from (an instruction is at most 16 bytes), so a parse is used again only for the same
    // bytes -- a new image in a recycled buffer at the same place must not get the old image's parses
    std::unordered_map<std::uint64_t, std::array<std::uint8_t, 16>> parsed;
    const std::uint8_t* seen_code = nullptr;   // and the buffer they were made in (another buffer: none of them is kept)
    std::size_t seen_size = 0;
    std::uint64_t seen_base = 0;
    ghidra::ContextInternal context;
    std::unique_ptr<ghidra::Sleigh> trans;
    ghidra::DocumentStorage store;
    mutable std::mutex lock;   // (a translator keeps state between instructions: one read at a time)
};

Reader::Reader(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context) : impl_(std::make_unique<Impl>()) {
    initialize_once();
    if (!std::filesystem::is_regular_file(sla)) throw std::runtime_error("no processor description " + sla.string());
    impl_->trans = std::make_unique<ghidra::Sleigh>(&impl_->window, &impl_->context);
    try {
        std::istringstream name("<sleigh>" + sla.generic_string() + "</sleigh>");
        ghidra::Element* root = impl_->store.parseDocument(name)->getRoot();
        impl_->store.registerTag(root);
        impl_->trans->initialize(impl_->store);
        for (const auto& [variable, value] : context) impl_->context.setVariableDefault(variable, static_cast<ghidra::uintm>(value));
    } catch (const ghidra::LowlevelError& e) {
        throw std::runtime_error(sla.string() + ": " + e.explain);
    }
}

Reader::~Reader() = default;

Instruction Reader::at(const std::uint8_t* code, std::size_t size, std::uint64_t base, std::uint64_t address) const {
    std::lock_guard hold(impl_->lock);
    impl_->window.set(code, size, base);
    if (impl_->seen_code != code || impl_->seen_size != size || impl_->seen_base != base) {
        impl_->trans->forgetParses();
        impl_->parsed.clear();
        impl_->seen_code = code, impl_->seen_size = size, impl_->seen_base = base;
    }
    ghidra::AddrSpace* space = impl_->trans->getDefaultCodeSpace();
    const std::uint64_t unit = static_cast<std::uint64_t>(space->getWordSize());
    Instruction ins;
    ins.address = address;
    const std::uint64_t at = (address - base) * unit;
    if (address < base || at >= size) return ins;
    std::array<std::uint8_t, 16> here{};
    for (std::size_t k = 0; k < here.size() && at + k < size; ++k) here[k] = code[at + k];
    if (const auto kept = impl_->parsed.find(address); kept != impl_->parsed.end() && kept->second != here) {
        impl_->trans->forgetParses();   // (other bytes than the kept parse was made of)
        impl_->parsed.clear();
    }
    impl_->parsed[address] = here;
    Text text;
    try {
        const ghidra::Address addr(space, address);
        ins.length = static_cast<std::size_t>(impl_->trans->printAssembly(text, addr));
        ins.mnemonic = text.mnemonic;
        ins.operands = text.operands;
        if (at + ins.length > size) return Instruction{address};
        FlowOf flow(address, address + ins.length / unit);
        impl_->trans->oneInstruction(flow, addr);
        ins.flow = flow.result();
        ins.target = flow.target;
    } catch (const ghidra::LowlevelError&) {
        return Instruction{address};
    }
    return ins;
}

std::size_t Reader::alignment() const { return static_cast<std::size_t>(impl_->trans->getAlignment()); }

std::vector<Instruction> Reader::read(const std::uint8_t* code, std::size_t size, std::uint64_t base, std::size_t count) const {
    std::lock_guard hold(impl_->lock);
    impl_->window.set(code, size, base);
    impl_->trans->forgetParses();   // (other bytes may have been read at these addresses before)
    impl_->parsed.clear();
    ghidra::AddrSpace* space = impl_->trans->getDefaultCodeSpace();
    // an address in the space's units (a word-addressed processor counts words, not bytes)
    const std::uint64_t unit = static_cast<std::uint64_t>(space->getWordSize());
    std::vector<Instruction> out;
    for (std::uint64_t at = 0; at < size && (count == 0 || out.size() < count);) {
        Instruction ins;
        ins.address = base + at / unit;
        Text text;
        try {
            const ghidra::Address addr(space, base + at / unit);
            ins.length = static_cast<std::size_t>(impl_->trans->printAssembly(text, addr));   // (in bytes, whatever the space's unit)
            ins.mnemonic = text.mnemonic;
            ins.operands = text.operands;
            FlowOf flow(base + at / unit, base + (at + ins.length) / unit);
            impl_->trans->oneInstruction(flow, addr);
            ins.flow = flow.result();
            ins.target = flow.target;
        } catch (const ghidra::LowlevelError&) {
            ins.length = 0;
        }
        if (ins.length != 0 && at + ins.length > size) ins.length = 0;   // cut off by the end of the bytes
        const std::uint64_t step = ins.length != 0 ? ins.length : std::max<std::uint64_t>(alignment() * unit, 1);
        out.push_back(std::move(ins));
        at += step;
    }
    return out;
}

}  // namespace cb::decompile::sleigh
