// Tests of cb/core16_types.hpp: the records have the defaults the modules rely on, the ordered pointer table keeps the
// decompiler's order, and the abstract Uc can be implemented and driven (a fake machine with flat memory).
#include "cb/core16_types.hpp"

#include <map>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace c16 = cb::core16;

namespace {

// A machine that is only memory and registers: enough to see the interface carries what the callers pass.
class FakeUc final : public c16::Uc {
public:
    struct Saved final : Context {
        std::map<int, std::uint64_t> regs;
    };
    void* handle() const override { return nullptr; }
    void mem_map(std::uint64_t address, std::uint64_t size) override { mapped.emplace(address, Bytes(size, 0)); }
    void mem_unmap(std::uint64_t address, std::uint64_t) override { mapped.erase(address); }
    Bytes mem_read(std::uint64_t address, std::uint64_t size) override {
        const Bytes& page = page_of(address);
        const std::uint64_t at = address - base_of(address);
        if (at + size > page.size()) throw EmulationError("read past the mapping");
        return Bytes(page.begin() + static_cast<std::ptrdiff_t>(at), page.begin() + static_cast<std::ptrdiff_t>(at + size));
    }
    void mem_write(std::uint64_t address, ByteView data) override {
        Bytes& page = mapped.at(base_of(address));
        const std::uint64_t at = address - base_of(address);
        if (at + data.size() > page.size()) throw EmulationError("write past the mapping");
        std::copy(data.begin(), data.end(), page.begin() + static_cast<std::ptrdiff_t>(at));
    }
    std::uint64_t reg_read(int reg) override { return regs[reg]; }
    double reg_read_float(int) override { return 1.5; }
    void reg_write(int reg, std::uint64_t value) override { regs[reg] = value; }
    void reg_write_descriptor(int reg, const Descriptor& d) override { regs[reg] = d.base; }
    std::unique_ptr<Context> context_save() override {
        auto s = std::make_unique<Saved>();
        s->regs = regs;
        return s;
    }
    void context_restore(const Context& context) override { regs = static_cast<const Saved&>(context).regs; }
    void ctl_flush_tb() override { ++flushes; }
    void emu_start(std::uint64_t begin, std::uint64_t until, std::uint64_t, std::uint64_t) override {
        // "runs" one instruction of size 1 at every address from begin up to until
        for (std::uint64_t a = begin; a < until && !stopped; ++a) {
            if (code_hook) code_hook(*this, a, 1);
            if (a == in_at && in_hook) regs[1] = in_hook(*this, 0x60, 1);
        }
    }
    void emu_stop() override { stopped = true; }
    void hook_add_code(std::function<void(c16::Uc&, std::uint64_t, std::uint32_t)> cb, std::uint64_t, std::uint64_t) override { code_hook = std::move(cb); }
    void hook_add_insn_in(std::function<std::uint32_t(c16::Uc&, std::uint32_t, std::uint32_t)> cb) override { in_hook = std::move(cb); }
    void hook_add_insn_out(std::function<void(c16::Uc&, std::uint32_t, std::uint32_t, std::uint32_t)>) override {}

    std::map<std::uint64_t, Bytes> mapped;
    std::map<int, std::uint64_t> regs;
    std::function<void(c16::Uc&, std::uint64_t, std::uint32_t)> code_hook;
    std::function<std::uint32_t(c16::Uc&, std::uint32_t, std::uint32_t)> in_hook;
    std::uint64_t in_at = ~0ull;
    bool stopped = false;
    int flushes = 0;

private:
    std::uint64_t base_of(std::uint64_t address) const {
        for (const auto& [base, page] : mapped) {
            if (address >= base && address < base + page.size()) return base;
        }
        throw EmulationError("unmapped");
    }
    const Bytes& page_of(std::uint64_t address) const { return mapped.at(base_of(address)); }
};

}  // namespace

CB_TEST("core16.records_start_empty", [] {
    const c16::ClassRecord c;
    CB_CHECK(c.vtable == 0 && c.slots == 0 && c.methods.empty() && c.name.empty() && !c.size && !c.size_exact && !c.com && !c.message_map);
    const c16::FunctionNotes n;
    CB_CHECK(n.globals.empty() && n.pointers.empty() && n.structs.empty() && n.l2 == nullptr && !n.l2_exact_json && !n.placed && !n.from_l2);
    const c16::StructNote s{"s_f_p", 8, {{0, 4, "int"}, {4, 4, "char*"}}};
    CB_CHECK(s.fields.size() == 2 && s.fields[1].type == "char*");
    const c16::GlobalNote g{0x403000, 4, 1};
    CB_CHECK(g.address == 0x403000 && g.elem == 1);
});

CB_TEST("core16.pointers_keep_the_order_they_were_written_in", [] {
    c16::FunctionNotes n;
    n.pointers.insert_or_assign("pz", 4);
    n.pointers.insert_or_assign("pa", 2);
    n.pointers.insert_or_assign("pm", 4);
    std::vector<std::string> names;
    for (const auto& [name, bytes] : n.pointers) names.push_back(name);
    CB_CHECK(names == (std::vector<std::string>{"pz", "pa", "pm"}));
    CB_CHECK_EQ(n.pointers.at("pa"), 2);
});

CB_TEST("core16.segoff_orders_by_segment_then_offset", [] {
    CB_CHECK((c16::SegOff{1, 5} < c16::SegOff{2, 0}) && (c16::SegOff{1, 5} < c16::SegOff{1, 6}) && (c16::SegOff{3, 3} == c16::SegOff{3, 3}));
});

CB_TEST("core16.uc_memory_registers_and_contexts", [] {
    FakeUc fake;
    c16::Uc& uc = fake;
    uc.mem_map(0x1000, 0x100);
    const Bytes data = {1, 2, 3, 4};
    uc.mem_write(0x1010, data);
    CB_CHECK(uc.mem_read(0x1010, 4) == data);
    CB_CHECK_THROWS_AS(EmulationError, (void)uc.mem_read(0x10FE, 8));
    uc.reg_write(7, 0xDEAD);
    const std::unique_ptr<c16::Uc::Context> saved = uc.context_save();
    uc.reg_write(7, 0xBEEF);
    CB_CHECK_EQ(uc.reg_read(7), std::uint64_t{0xBEEF});
    uc.context_restore(*saved);
    CB_CHECK_EQ(uc.reg_read(7), std::uint64_t{0xDEAD});
    CB_CHECK(uc.reg_read_float(0) == 1.5);
    uc.speed_up();      // (the default does nothing)
    uc.ctl_flush_tb();
    CB_CHECK_EQ(fake.flushes, 1);
    uc.mem_unmap(0x1000, 0x100);
    CB_CHECK_THROWS_AS(EmulationError, (void)uc.mem_read(0x1010, 1));
});

CB_TEST("core16.uc_hooks_see_the_instructions_and_ports", [] {
    FakeUc fake;
    c16::Uc& uc = fake;
    std::vector<std::uint64_t> seen;
    uc.hook_add_code([&](c16::Uc&, std::uint64_t address, std::uint32_t size) { seen.push_back(address + size); }, 0, ~0ull);
    uc.hook_add_insn_in([](c16::Uc&, std::uint32_t port, std::uint32_t size) { return port * 16 + size; });
    fake.in_at = 0x102;
    uc.emu_start(0x100, 0x104, 0, 0);
    CB_CHECK(seen == (std::vector<std::uint64_t>{0x101, 0x102, 0x103, 0x104}));
    CB_CHECK_EQ(uc.reg_read(1), std::uint64_t{0x601});
    uc.emu_stop();
    seen.clear();
    uc.emu_start(0x100, 0x104, 0, 0);
    CB_CHECK(seen.empty());
});
