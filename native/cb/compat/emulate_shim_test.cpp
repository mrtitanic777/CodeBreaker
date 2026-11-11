// The shims at codebreaker/emulate/{run,ports16,capstone_x86,symbolic,pcode}.hpp answer what the old layer's callers
// expect, through the new code.
#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/emulate/ports16.hpp"
#include "codebreaker/emulate/run.hpp"
#include "codebreaker/emulate/symbolic.hpp"
#include "cb/core/testing.hpp"

namespace old = codebreaker::emulate;

CB_TEST("compat.emulate_shim.run", [] {
    old::run::Run r(py::str("exception: UC_ERR_READ_UNMAPPED"));
    CB_CHECK(r.ended() == py::str("fault"));
    CB_CHECK(old::run::ended(std::nullopt) == py::str("none"));
    CB_CHECK(old::run::ended(py::str("int:21")) == py::str("int"));
    CB_CHECK(!r.kept());                                         // preserved not read
    old::run::Preserved p;
    p.set(py::str("EBX"), std::make_tuple(std::optional<int64_t>(1), std::optional<int64_t>(1)));
    p.set(py::str("ESI"), std::make_tuple(std::optional<int64_t>(2), std::optional<int64_t>(3)));
    old::run::Run q(py::str("ret"), std::nullopt, py::list<old::run::Write>(), false, py::list<old::run::Call>(),
                    py::list<old::run::IO>(), std::nullopt, p, py::list<int64_t>(), 5);
    CB_CHECK(!q.kept());
    CB_CHECK(q.kept(py::list<py::str>{py::str("EBX")}));
    CB_CHECK(!q.kept(py::list<py::str>{py::str("EBX"), py::str("ESI")}));
    CB_CHECK(q.repr() == py::str("<Run ret steps=5 calls=0 writes=0>"));
});

CB_TEST("compat.emulate_shim.ports16", [] {
    int64_t step = 7;
    old::ports16::Devices d([&] { return step++; });
    CB_CHECK_EQ(d.read(0x3FD), 0x61);
    d.write(0x3FB, 0x80);
    py::list<old::ports16::LogEntry> log = d.log;
    CB_CHECK_EQ(log.size(), 2);
    CB_CHECK_EQ(std::get<0>(log[0]), 7);
    CB_CHECK(std::get<1>(log[0]) == py::str("in"));
    CB_CHECK(std::get<1>(log[1]) == py::str("out"));
    d.log.clear();
    py::list<old::ports16::LogEntry> after = d.log;
    CB_CHECK_EQ(after.size(), 0);
    // the hooks' callbacks
    struct Hooks : old::ports16::InsnHooks {
        std::function<int64_t(int64_t, int64_t)> in;
        std::function<void(int64_t, int64_t, int64_t)> out;
        void hook_in(std::function<int64_t(int64_t, int64_t)> f) override { in = f; }
        void hook_out(std::function<void(int64_t, int64_t, int64_t)> f) override { out = f; }
    } hooks;
    old::ports16::hook(hooks, d);
    hooks.out(0x3F8, 2, 0x0201);                                 // DLAB is on: the divisor
    CB_CHECK_EQ(hooks.in(0x3F8, 2), 0x0201);
});

CB_TEST("compat.emulate_shim.capstone_x86", [] {
    csh h = old::capstone_x86::handle(32, false);
    CB_CHECK_EQ(h, old::capstone_x86::handle(32, false));
    CB_CHECK_EQ(old::capstone_x86::handle(0, false), h);        // any width but 16 and 64 is 32
    CB_CHECK(old::capstone_x86::handle(16, false) != h);
    CB_CHECK(old::capstone_x86::api().disasm != nullptr);
    CB_CHECK(old::capstone_x86::dll().utf8().find("capstone") != std::string::npos);
});

CB_TEST("compat.emulate_shim.symbolic", [] {
    namespace s = old::symbolic;
    s::ExprP x = s::var(py::str("x"), 32);
    s::ExprP lo = s::extract(x, 0, 16);
    s::ExprP hi = s::extract(x, 16, 16);
    CB_CHECK(s::concat(hi, lo).get() == x.get());                // folded through the pool, identity kept
    CB_CHECK(s::zext(x, 32).get() == x.get());
    s::ExprP flag = s::to_bool(s::compare(py::str("ult"), x, s::const_(9, 32)));
    CB_CHECK(s::nonzero(flag)->op == py::str("ult"));
    CB_CHECK(flag->bits == 8);
    CB_CHECK(s::const_(0x1FF, 8)->value == 0xFF);
    CB_CHECK(s::TRUE->op == py::str("true"));
    s::InputsMemo memo;
    py::set<py::str> names = s::inputs(s::binary(py::str("add"), x, s::var(py::str("y"), 32)), memo);
    CB_CHECK_EQ(names.size(), 2);
    // a record made by hand (native_shadow does) joins the pool when it is used
    auto by_hand = std::make_shared<s::Expr>();
    by_hand->op = py::str("neg");
    by_hand->bits = 32;
    by_hand->args.push_back(x);
    s::ExprP folded = s::extract(by_hand, 0, 32);
    CB_CHECK(folded.get() == by_hand.get());
});

CB_TEST("compat.emulate_shim.pcode_records", [] {
    namespace p = old::pcode;
    p::Op add;
    add.opcode = py::str("INT_ADD");
    add.out = p::Varnode{py::str("r"), 0, 4};
    add.inputs.append(p::Input(p::Varnode{py::str("r"), 0, 4}));
    add.inputs.append(p::Input(p::Varnode{py::str("c"), 1, 4}));
    add.inputs.append(p::Input(py::str("name")));
    add.inputs.append(p::Input(py::none_t{}));
    p::Lifted l(1, py::list<p::Op>{add});
    CB_CHECK(!l.full);
    CB_CHECK(l.reads.contains(3));
    CB_CHECK(l.writes.contains(0));
    CB_CHECK(l.ops.size() == 1);
});
