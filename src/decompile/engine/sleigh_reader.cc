// decompile/engine/sleigh_reader.cc -- see sleigh_reader.hh. Built with the core (its own flags), so the core's headers stay here.

#include "decompile/engine/sleigh_reader.hh"

#include <algorithm>
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

std::size_t Reader::alignment() const { return static_cast<std::size_t>(impl_->trans->getAlignment()); }

std::vector<Instruction> Reader::read(const std::uint8_t* code, std::size_t size, std::uint64_t base, std::size_t count) const {
    std::lock_guard hold(impl_->lock);
    impl_->window.set(code, size, base);
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
