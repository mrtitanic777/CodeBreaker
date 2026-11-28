// cb-unity: alone (file-local names that other files in a batch also use)
// The recompiler's input: an Xbox 360 instruction's p-code, as data, from the description the decompiler lifts and the emulator runs
// (ppc_64_xenon_be.sla), through sleigh::Reader::pcode. Step 1 of docs/RECOMPILER.md: the one translator writes C++ from these
// operations, so what they say for each kind of instruction (arithmetic, a load, a store with update, a compare, the branches and calls,
// a VMX128 operation) is checked here, and each is checked against the emulator running the same word.
#include <cstdint>
#include <filesystem>
#include <format>
#include <optional>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/engine/pcode_emulator.hh"
#include "decompile/engine/sleigh_reader.hh"

namespace cb {
namespace {

namespace sl = decompile::sleigh;

std::filesystem::path xenon_sla() {
    std::filesystem::path dir = config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
    if (const std::optional<std::string> env = fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) dir = fs::from_text(*env);
    return dir / "ppc_64_xenon_be.sla";
}

constexpr std::uint64_t kBase = 0x82000000;

std::vector<std::uint8_t> be_words(std::initializer_list<std::uint32_t> words) {
    std::vector<std::uint8_t> out;
    for (const std::uint32_t w : words) {
        out.push_back(static_cast<std::uint8_t>(w >> 24));
        out.push_back(static_cast<std::uint8_t>(w >> 16));
        out.push_back(static_cast<std::uint8_t>(w >> 8));
        out.push_back(static_cast<std::uint8_t>(w));
    }
    return out;
}

// A varnode as text: a register by its name, a temporary as uN, a constant as #value, memory as space[address].
std::string show(const sl::Reader& r, const sl::Varnode& v) {
    switch (v.kind) {
        case sl::Space::Constant: return std::format("#{:x}:{}", v.offset, v.size);
        case sl::Space::Register: {
            const std::string name = r.register_name(v.offset, v.size);
            return name.empty() ? std::format("reg[{:x}]:{}", v.offset, v.size) : name;
        }
        case sl::Space::Unique: return std::format("u{:x}:{}", v.offset, v.size);
        case sl::Space::Memory: return std::format("{}[{:x}]:{}", r.space_name(v.space), v.offset, v.size);
    }
    return "?";
}

// The operations, one a line: "out = OP in, in".
std::string show(const sl::Reader& r, const sl::Lifted& l) {
    std::string text;
    for (const sl::PcodeOp& op : l.ops) {
        if (op.output) text += show(r, *op.output) + " = ";
        text += sl::op_name(op.op);
        for (std::size_t i = 0; i < op.inputs.size(); ++i) {
            text += i == 0 ? " " : ", ";
            text += op.op == sl::Op::CallOther && i == 0 ? r.user_op_name(op.inputs[0].offset) : show(r, op.inputs[i]);
        }
        text += "\n";
    }
    return text;
}

sl::Lifted lift(const sl::Reader& r, std::uint32_t word) {
    const std::vector<std::uint8_t> code = be_words({word});
    return r.pcode(code.data(), code.size(), kBase, kBase);
}

bool has_op(const sl::Lifted& l, sl::Op op) {
    for (const sl::PcodeOp& o : l.ops) {
        if (o.op == op) return true;
    }
    return false;
}

}  // namespace

CB_TEST("recompile.pcode.an_instruction_is_its_operations_on_the_description_registers", [] {
    const sl::Reader r(xenon_sla(), {});
    // add r3,r4,r5: one addition of the two registers into the third
    const sl::Lifted add = lift(r, 0x7C642A14);
    CB_CHECK_EQ(add.length, std::size_t{4});
    CB_CHECK_EQ(show(r, add), std::string("r3 = INT_ADD r4, r5\n"));
    // the register table the translator maps to the context comes from the same description
    CB_CHECK(r.register_named("r3").has_value());
    CB_CHECK_EQ(r.register_named("r3")->size, 8u);
    CB_CHECK(!r.register_named("no_such_register").has_value());
    // a part of a register is named by the register: cmpwi compares r3's low word (big-endian: 4 bytes at r3's offset + 4)
    CB_CHECK_EQ(r.register_name(r.register_named("r3")->offset + 4, 4), std::string("r3"));
    const sl::Lifted cmpwi = lift(r, 0x2F030000);   // cmpwi cr6,r3,0
    CB_CHECK_EQ(cmpwi.ops.front().op, sl::Op::IntSext);
    CB_CHECK_EQ(cmpwi.ops.front().inputs[0].offset, r.register_named("r3")->offset + 4);
    CB_CHECK_EQ(cmpwi.ops.front().inputs[0].size, 4u);
    CB_CHECK_EQ(r.register_name(cmpwi.ops.back().output->offset, cmpwi.ops.back().output->size), std::string("cr6"));
});

CB_TEST("recompile.pcode.loads_and_stores_name_their_space_and_address", [] {
    const sl::Reader r(xenon_sla(), {});
    const sl::Lifted lwz = lift(r, 0x80640008);   // lwz r3,8(r4)
    CB_CHECK(has_op(lwz, sl::Op::Load));
    for (const sl::PcodeOp& op : lwz.ops) {
        if (op.op != sl::Op::Load) continue;
        CB_CHECK_EQ(r.space_name(static_cast<unsigned>(op.inputs[0].offset)), std::string("ram"));
        CB_CHECK_EQ(op.output->size, 4u);
    }
    CB_CHECK_EQ(lwz.ops.back().output->kind, sl::Space::Register);
    CB_CHECK_EQ(r.register_name(lwz.ops.back().output->offset, lwz.ops.back().output->size), std::string("r3"));
    const sl::Lifted stwu = lift(r, 0x9421FFF0);   // stwu r1,-16(r1): a store, and r1 updated
    CB_CHECK(has_op(stwu, sl::Op::Store));
    CB_CHECK_EQ(r.register_name(stwu.ops.back().output->offset, stwu.ops.back().output->size), std::string("r1"));
});

CB_TEST("recompile.pcode.branches_and_calls_name_their_targets", [] {
    const sl::Reader r(xenon_sla(), {});
    const sl::Lifted bl = lift(r, 0x48000101);   // bl +0x100
    CB_CHECK(has_op(bl, sl::Op::Call));
    for (const sl::PcodeOp& op : bl.ops) {
        if (op.op != sl::Op::Call) continue;
        CB_CHECK_EQ(op.inputs[0].kind, sl::Space::Memory);
        CB_CHECK_EQ(op.inputs[0].offset, kBase + 0x100);
    }
    CB_CHECK(has_op(lift(r, 0x4E800020), sl::Op::Return));      // blr
    CB_CHECK(has_op(lift(r, 0x4E800420), sl::Op::BranchInd));   // bctr
    CB_CHECK(has_op(lift(r, 0x419A0010), sl::Op::CBranch));     // beq cr6,+0x10
});

CB_TEST("recompile.pcode.vmx128_and_system_operations_are_user_operations", [] {
    const sl::Reader r(xenon_sla(), {});
    const sl::Lifted vmadd = lift(r, 0x140000D0);   // vmaddfp128 v0,v0,v0,v0
    CB_CHECK_EQ(vmadd.length, std::size_t{4});
    CB_CHECK(!vmadd.ops.empty());
    const sl::Lifted sync = lift(r, 0x7C0004AC);    // sync
    CB_CHECK(has_op(sync, sl::Op::CallOther));
    // a word that is no instruction: no length, no operations
    const sl::Lifted none = lift(r, 0x00000000);
    CB_CHECK_EQ(none.length, std::size_t{0});
    CB_CHECK(none.ops.empty());
});

// The operations say what the emulator does with the same word: add, a load, a store with update and a compare run on it give the
// results the operations name.
CB_TEST("recompile.pcode.the_operations_agree_with_the_emulator", [] {
    sl::Emulator emu(xenon_sla(), {});
    const std::vector<std::uint8_t> code = be_words({0x7C642A14, 0x80640008, 0x9421FFF0, 0x2F030000});
    emu.load(kBase, code.data(), code.size());
    const std::uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    emu.load(0x1008, data, 4);
    emu.setPc(kBase);
    emu.setReg("r4", 0x1000);
    emu.setReg("r5", 0x20);
    emu.setReg("r1", 0x8000);
    CB_CHECK(emu.step() == sl::Emulator::Status::Ok);
    CB_CHECK_EQ(emu.reg("r3"), std::uint64_t{0x1020});      // r3 = INT_ADD r4, r5
    CB_CHECK(emu.step() == sl::Emulator::Status::Ok);
    CB_CHECK_EQ(emu.reg("r3"), std::uint64_t{0x12345678});  // the LOAD of ram at r4 + 8, zero-extended into r3
    CB_CHECK(emu.step() == sl::Emulator::Status::Ok);
    CB_CHECK_EQ(emu.reg("r1"), std::uint64_t{0x7FF0});      // the STORE at r1 - 16, and r1 its address
    CB_CHECK_EQ(emu.readValue(0x7FF0, 4), std::uint64_t{0x8000});
});

}  // namespace cb
