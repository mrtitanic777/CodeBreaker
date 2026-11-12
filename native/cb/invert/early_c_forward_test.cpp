// Tests of cb/invert/early_c_forward.hpp. The expected bytes of each modelled function were produced by the original
// Python model on the same quad records and then READ: every one is a plausible Microsoft C 3.00 small-model function
// (push bp; sub sp,N with the stack probe; mov bp,sp; body; add sp,N; pop bp; ret) whose body matches the statement
// the quads describe. The real quad file of the oracle program (testdata/FXU.Q) is covered by the failure it must give:
// its calls and string operands are outside what the model covers.
//
// DIVERGES from the Python model: a short jump whose distance does not fit rel8 is an UnsupportedError where the Python
// wrapped the displacement into a wrong byte; the Python's KeyError/IndexError/TypeError/NotImplementedError ways of
// refusing a function are FormatError (a malformed quad) and UnsupportedError (a shape not modelled) in a Result; the
// printed report is the same text.
#include "cb/invert/early_c_forward.hpp"

#include <string>
#include <utility>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/omf.hpp"
#include "cb/invert/quad_dump.hpp"

using namespace cb;
using namespace std::literals;
namespace ecf = cb::invert::early_c_forward;
namespace qa = cb::invert::quad_align;
namespace omf = cb::formats::omf;

namespace ecf_test {

using Quads = qa::QuadFunction;

Bytes konst(unsigned v) { return {4, 0x80, 0, static_cast<std::uint8_t>(v), static_cast<std::uint8_t>(v >> 8), 0, 0}; }
Bytes var(unsigned id, std::uint8_t type = 2) {
    return {0x0a, 0, 0, static_cast<std::uint8_t>(id), static_cast<std::uint8_t>(id >> 8), type, 0, 0, 0, 0, 0};
}
Bytes field(unsigned id, std::uint8_t offset) {
    return {0x0a, 0x20, 0, static_cast<std::uint8_t>(id), static_cast<std::uint8_t>(id >> 8), 0, 0, 0, 0, offset, 0};
}
Bytes temp() { return {0x0e, 0x40, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0}; }

// operands with the one-byte separator between them
Bytes ops(std::initializer_list<Bytes> list) {
    Bytes out;
    for (const Bytes& o : list) {
        if (!out.empty()) out.push_back(0);
        append(out, o);
    }
    return out;
}

qa::Quad quad(std::uint8_t opcode, Bytes payload = {}) { return {opcode, std::move(payload)}; }
qa::Quad begin(unsigned fn) {
    return quad(0x23, {0x0a, 0, 0, static_cast<std::uint8_t>(fn), static_cast<std::uint8_t>(fn >> 8), 0xff, 0xff, 0, 0, 0, 0});
}
const Bytes kNoPayload = {0, 0, 0};

std::string modelled(const Quads& f) {
    const Result<Bytes> r = ecf::model(f);
    return r.ok() ? crypto::to_hex(r.value()) : "";
}

// A quad file holding these functions.
Bytes quad_file(const std::vector<Quads>& functions) {
    Bytes f(cb::invert::quad_dump::kFirstRecord, 0);
    for (const Quads& fn : functions) {
        for (const qa::Quad& q : fn) {
            f.push_back(q.opcode);
            f.push_back(static_cast<std::uint8_t>(q.payload.size()));
            append(f, q.payload);
        }
    }
    f.push_back(0xff);
    f.push_back(0);
    return f;
}

// An object with one PROG segment of `code` and publics {name, offset}.
Bytes object_of(ByteView code, const std::vector<std::pair<std::string, int>>& publics) {
    Bytes names;
    append(names, omf::name("PROG"));
    append(names, omf::name("CODE"));
    Bytes out = omf::record(0x96, names);
    append(out, omf::record(0x98, Bytes{0x20, static_cast<std::uint8_t>(code.size()), static_cast<std::uint8_t>(code.size() >> 8), 1, 2, 0}));
    Bytes data = {1, 0, 0};
    append(data, code);
    append(out, omf::record(0xa0, data));
    Bytes pub = {0, 1};
    for (const auto& [name, offset] : publics) {
        append(pub, omf::name(name));
        pub.push_back(static_cast<std::uint8_t>(offset));
        pub.push_back(static_cast<std::uint8_t>(offset >> 8));
        pub.push_back(0);
    }
    append(out, omf::record(0x90, pub));
    append(out, omf::record(0x8a, Bytes{0}));
    return out;
}

CB_TEST("invert.early_c_forward.operands", [] {
    // one of each kind, the separator between them
    const auto parsed = ecf::parse_operands(ops({konst(300), var(7, 4), field(9, 6), temp()}));
    CB_CHECK_EQ(parsed.size(), std::size_t{4});
    CB_CHECK(parsed[0].kind == ecf::Operand::Kind::Const);
    CB_CHECK_EQ(parsed[0].value, 300);
    CB_CHECK(parsed[1].kind == ecf::Operand::Kind::Node);
    CB_CHECK_EQ(parsed[1].id, 7);
    CB_CHECK_EQ(parsed[1].type, 4);
    CB_CHECK(parsed[2].kind == ecf::Operand::Kind::Deref);
    CB_CHECK_EQ(parsed[2].id, 9);
    CB_CHECK_EQ(parsed[2].field, 6);
    CB_CHECK(parsed[3].kind == ecf::Operand::Kind::Temp);

    CB_CHECK(ecf::parse_operands(Bytes{}).empty());
    // the list stops at the first byte that starts no operand (the quad's trailer)
    Bytes with_trailer = var(7);
    append(with_trailer, Bytes{0x55, 0x66, 0x77});
    CB_CHECK_EQ(ecf::parse_operands(with_trailer).size(), std::size_t{1});
    // a variable, dereference or temporary that would run past the end is part of the trailer; a cut constant is damage
    const Bytes one_var = var(7), one_temp = temp();
    CB_CHECK(ecf::parse_operands(Bytes(one_var.begin(), one_var.begin() + 10)).empty());
    CB_CHECK(ecf::parse_operands(Bytes(one_temp.begin(), one_temp.begin() + 14)).empty());
    CB_CHECK_THROWS_AS(FormatError, ecf::parse_operands(Bytes{4, 0x80, 0, 1}));
    // a constant may end right after its value (the 7-byte form is not required to be complete)
    CB_CHECK_EQ(ecf::parse_operands(Bytes{4, 0x80, 0, 5, 0}).at(0).value, 5);
});

CB_TEST("invert.early_c_forward.funcbegin", [] {
    CB_CHECK_EQ(ecf::parse_funcbegin(begin(0x0106).payload), 0x0106);
    CB_CHECK_THROWS_AS(FormatError, ecf::parse_funcbegin(Bytes{0, 0, 0, 1}));
});

CB_TEST("invert.early_c_forward.arithmetic", [] {
    // int f(a, b) { return a + b; }   (parameters are ids 7 and 8 of the function 6)
    CB_CHECK_EQ(modelled({begin(6), quad(0x13, ops({var(7), var(8), temp()})), quad(0x24, temp())}),
                "5583ec023b2600007703e900008bec8b460603460883c4025dc3");
    // a leaf function: no arithmetic, no frame, no stack probe
    CB_CHECK_EQ(modelled({begin(6), quad(0x24, ops({konst(0)}))}), "558bec33c05dc3");
    CB_CHECK_EQ(modelled({begin(6), quad(0x24, ops({konst(300)}))}), "558becb82c015dc3");
    // mul, div, mod, shl, shr on two parameters
    CB_CHECK_EQ(modelled({begin(6), quad(0x10, ops({var(7), var(8), temp()})), quad(0x11, ops({var(7), var(8), temp()})),
                          quad(0x12, ops({var(7), var(8), temp()})), quad(0x15, ops({var(7), var(8), temp()})),
                          quad(0x16, ops({var(7), var(8), temp()})), quad(0x24, temp())}),
                "5583ec023b2600007703e900008bec8b4606f76e088b460699f77e088b460699f77e088bc28b4e088b4606d3e08b4e088b4606d3f8"
                "83c4025dc3");
    // and/or with a global and a constant: ax, [global] needs a relocation (zeros); ax, imm
    CB_CHECK_EQ(modelled({begin(6), quad(0x1d, ops({var(3), konst(255), temp()})), quad(0x1f, ops({temp(), var(8), temp()})),
                          quad(0x24, temp())}),
                "5583ec023b2600007703e900008beca1000025ff000b460883c4025dc3");
    // a relational used as a value: compare, then the 0/1 sequence
    CB_CHECK_EQ(modelled({begin(6), quad(0x1b, ops({var(7), var(8), temp()})), quad(0x24, temp())}),
                "5583ec023b2600007703e900008bec8b46063b4608740433dbeb0333db438bc383c4025dc3");
});

CB_TEST("invert.early_c_forward.long_arithmetic", [] {
    // longs: 4-byte frame and parameters; add carries through bx, multiply goes through the helper call
    CB_CHECK_EQ(modelled({begin(6), quad(0x13, ops({var(7, 4), var(9, 4), temp()})), quad(0x24, temp())}),
                "5583ec043b2600007703e900008bec8b46080346108b5e0a135e1283c4045dc3");
    CB_CHECK_EQ(modelled({begin(6), quad(0x10, ops({var(7, 4), var(9, 4), temp()})), quad(0x24, temp())}),
                "5583ec043b2600007703e900008bec8b460a8b5e088b4e128b5610e800008bc383c4045dc3");
    // a relational on longs is not modelled
    const Result<Bytes> r = ecf::model({begin(6), quad(0x17, ops({var(7, 4), var(9, 4), temp()})), quad(0x24, temp())});
    CB_CHECK(!r.ok());
    CB_CHECK_EQ(std::string(r.error().kind()), "UnsupportedError");
});

CB_TEST("invert.early_c_forward.assignment_and_memory", [] {
    // g = 5; p = g; return g;   (3 is a global, 7 a parameter)
    CB_CHECK_EQ(modelled({begin(6), quad(0x00, ops({konst(5), var(3)})), quad(0x00, ops({var(3), var(7)})), quad(0x24, ops({var(3)}))}),
                "558becb80500a30000a10000894604a100005dc3");
    // x = p->a; p->b = 7;   (through si, at field offset 0 and 6)
    CB_CHECK_EQ(modelled({begin(6), quad(0x00, ops({field(7, 0), var(8)})), quad(0x00, ops({konst(7), field(7, 6)})),
                          quad(0x24, ops({konst(0)}))}),
                "558bec8b76048b04894606b807008b760489440633c05dc3");
    // x = x + 1; x = x - 1: inc/dec [x] (the peephole)
    CB_CHECK_EQ(modelled({begin(6), quad(0x13, ops({var(7), konst(1), temp()})), quad(0x00, ops({temp(), var(7)})), quad(0x24, ops({var(7)}))}),
                "5583ec023b2600007703e900008becff46068b460683c4025dc3");
    CB_CHECK_EQ(modelled({begin(6), quad(0x14, ops({var(7), konst(1), temp()})), quad(0x00, ops({temp(), var(7)})), quad(0x24, ops({var(7)}))}),
                "5583ec023b2600007703e900008becff4e068b460683c4025dc3");
});

CB_TEST("invert.early_c_forward.control_flow", [] {
    // if (a < b) c = 1; return 0;   the compare feeds the CBRANCH and the jump skips the body when it does not hold
    CB_CHECK_EQ(modelled({begin(6), quad(0x17, ops({var(7), var(8), temp()})), quad(0x2c, temp()), quad(0x00, ops({konst(1), var(9)})),
                          quad(0x2e, kNoPayload), quad(0x24, ops({konst(0)}))}),
                "5583ec023b2600007703e900008bec8b46063b46087d06b8010089460a33c083c4025dc3");
    // while (a) b = 0; return 0;
    CB_CHECK_EQ(modelled({begin(6), quad(0x2f, kNoPayload), quad(0x30, ops({var(7)})), quad(0x00, ops({konst(0), var(8)})),
                          quad(0x31, kNoPayload), quad(0x24, ops({konst(0)}))}),
                "558bec8b460485c0740733c0894606ebf233c05dc3");
});

CB_TEST("invert.early_c_forward.refusals", [] {
    auto kind_of = [](const Quads& f) {
        const Result<Bytes> r = ecf::model(f);
        return r.ok() ? std::string("ok") : std::string(r.error().kind());
    };
    CB_CHECK_EQ(kind_of({begin(6), quad(0x00, ops({var(7), konst(1)}))}), "UnsupportedError");                    // store to a constant
    CB_CHECK_EQ(kind_of({begin(6), quad(0x17, ops({var(3), var(8), temp()})), quad(0x24, temp())}), "UnsupportedError");   // relational on a global
    CB_CHECK_EQ(kind_of({begin(6), quad(0x2e, kNoPayload), quad(0x24, ops({konst(0)}))}), "FormatError");         // a LABEL with no `if`
    CB_CHECK_EQ(kind_of({begin(6), quad(0x31, kNoPayload)}), "FormatError");                                        // a loop end with no loop
    CB_CHECK_EQ(kind_of({begin(6), quad(0x13, ops({var(7)}))}), "FormatError");                                     // a binary quad with one operand
    CB_CHECK_EQ(kind_of({}), "FormatError");                                                                       // no FUNC-BEGIN
    CB_CHECK_EQ(kind_of({quad(0x23, Bytes{1})}), "FormatError");                                                   // a FUNC-BEGIN too short
    CB_CHECK_EQ(kind_of({begin(6), quad(0x2c, temp()), quad(0x2e, kNoPayload)}), "FormatError");                  // (no open `if` for the label)
    // quads the model has no code for are skipped, as in the Python (a call generates nothing here)
    CB_CHECK_EQ(kind_of({begin(6), quad(0x20, Bytes{0xaa}), quad(0x24, ops({konst(0)}))}), "ok");
    // a frame offset that does not fit a byte: parameter 200 is past [bp+255]
    CB_CHECK_EQ(kind_of({begin(6), quad(0x00, ops({var(300), var(7)})), quad(0x24, ops({konst(0)}))}), "UnsupportedError");
    // an `if` body longer than a short jump reaches
    Quads far_if = {begin(6), quad(0x17, ops({var(7), var(8), temp()})), quad(0x2c, temp())};
    for (int i = 0; i < 30; ++i) far_if.push_back(quad(0x00, ops({konst(1), var(9)})));   // 6 bytes each
    far_if.push_back(quad(0x2e, kNoPayload));
    far_if.push_back(quad(0x24, ops({konst(0)})));
    CB_CHECK_EQ(kind_of(far_if), "UnsupportedError");
});

CB_TEST("invert.early_c_forward.real_quad_file", [] {
    // The oracle program's one function calls library routines and passes strings: not a shape the model covers.
    const auto fns = qa::parse_quads(fs::read_file(testing::data_path("invert/testdata/FXU.Q")));
    CB_CHECK_EQ(fns.size(), std::size_t{1});
    CB_CHECK(!ecf::model(fns[0]).ok());
});

CB_TEST("invert.early_c_forward.check_and_report", [] {
    const Quads add = {begin(6), quad(0x13, ops({var(7), var(8), temp()})), quad(0x24, temp())};
    const Bytes add_code = crypto::from_hex("5583ec023b2600007703e900008bec8b460603460883c4025dc3");
    // two functions in the object (the second not in the quad file, a third that is data and not a function)
    Bytes code = add_code;
    const Bytes other = {0x55, 0x8b, 0xec, 0x5d, 0xc3};
    append(code, other);
    append(code, Bytes{0x01, 0x02, 0x03});
    const Bytes object = object_of(code, {{"_add", 0}, {"_other", static_cast<int>(add_code.size())},
                                          {"_data", static_cast<int>(add_code.size() + other.size())}});
    const std::vector<ecf::Check> checks = ecf::check(quad_file({add}), object);
    CB_CHECK_EQ(checks.size(), std::size_t{2});
    CB_CHECK_EQ(checks[0].name, "_add");
    CB_CHECK(checks[0].exact());
    CB_CHECK_EQ(checks[1].name, "_other");
    CB_CHECK(!checks[1].predicted.has_value());
    CB_CHECK(!checks[1].exact());
    CB_CHECK_EQ(ecf::format_checks(checks),
                "  _add    OK   \n"
                "  _other  FAIL   pred \n"
                "            orig 55 8b ec 5d c3\n"
                "\nMC2 FORWARD MODEL: 1/2 BYTE-EXACT\n");

    // a prediction that is made and wrong shows both byte strings
    Bytes wrong = add_code;
    wrong[wrong.size() - 3] ^= 1;
    const std::vector<ecf::Check> mismatch = ecf::check(quad_file({add}), object_of(wrong, {{"_add", 0}}));
    CB_CHECK_EQ(mismatch.size(), std::size_t{1});
    CB_CHECK(mismatch[0].predicted.has_value() && !mismatch[0].exact());
    CB_CHECK_EQ(ecf::format_checks(mismatch).find("pred 55 83 ec 02"), std::size_t{17});

    CB_CHECK_EQ(ecf::format_checks({}), "\nMC2 FORWARD MODEL: 0/0 BYTE-EXACT\n");
});

CB_TEST("invert.early_c_forward.command", [] {
    fs::ScopedTempDir tmp("ecfwd");
    const Quads add = {begin(6), quad(0x13, ops({var(7), var(8), temp()})), quad(0x24, temp())};
    const Bytes add_code = crypto::from_hex("5583ec023b2600007703e900008bec8b460603460883c4025dc3");
    fs::write_file(tmp.path() / "a.q", quad_file({add}));
    fs::write_file(tmp.path() / "a.obj", object_of(add_code, {{"_add", 0}}));
    const std::vector<std::string> args = {fs::to_text(tmp.path() / "a.q"), fs::to_text(tmp.path() / "a.obj")};
    testing::CapturedStd captured;
    CB_CHECK_EQ(ecf::command(args), 0);
    CB_CHECK_EQ(captured.out(), "  _add    OK   \n\nMC2 FORWARD MODEL: 1/1 BYTE-EXACT\n");
    CB_CHECK_THROWS_AS(UsageError, ecf::command(std::span<const std::string>(args).first(1)));
    CB_CHECK_THROWS_AS(NotFoundError, ecf::command(std::vector<std::string>{fs::to_text(tmp.path() / "none.q"), args[1]}));
});

}  // namespace ecf_test
