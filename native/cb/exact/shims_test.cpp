// The shims at the old header paths (native/typed/cpp/codebreaker/exact/{aim,standin,modules,pascal_join}.hpp) over the
// modules of this folder: they must compile with the old layer and forward to the new code. Built at /W3 (the old
// layer is not warning-clean). The new modules' own tests have the real cases; these check the conversions.
#include <string>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "codebreaker/exact/aim.hpp"
#include "codebreaker/exact/modules.hpp"
#include "codebreaker/exact/pascal_join.hpp"
#include "codebreaker/exact/standin.hpp"

namespace {

namespace old = codebreaker::exact;
using cb::compat::from_py;
using cb::compat::to_py;

py::bytes py_bytes(std::initializer_list<int> values) {
    cb::Bytes b;
    for (const int v : values) b.push_back(static_cast<std::uint8_t>(v));
    return to_py(cb::ByteView(b));
}

// The old layer's decoder: Capstone's, as the old record.
old::compare::Decoder decoder() {
    return [](const py::bytes& code, int64_t address) {
        py::list<old::compare::Insn> out;
        const cb::Bytes bytes = from_py(code);
        for (const auto& i : cb::exact::x86::capstone_decoder().all(bytes, static_cast<std::uint32_t>(address))) {
            old::compare::Insn x;
            x.address = i.address;
            x.size = static_cast<int64_t>(i.size());
            x.op_str = to_py(i.op_str);
            x.bytes = to_py(cb::ByteView(i.bytes));
            out.append(x);
        }
        return out;
    };
}

std::filesystem::path data(std::string_view relative) { return cb::testing::data_path(std::string("exact/testdata/") + std::string(relative)); }

CB_TEST("exact.shims.aim", [] {
    const std::string listing = "_f\tPROC NEAR\n; Line 4\n*** 000000\t55 \t\t\tpush\tbp\n*** 000001\t8b ec \t\t\tmov\tbp,sp\n; Line 5\n*** 000003\t5d \t\t\tpop\tbp\n*** 000004\tc3 \t\t\tret\n_f\tENDP\n";
    const auto code = old::aim::listing_code(py::str(listing));
    CB_CHECK_EQ(code.size(), 4);
    CB_CHECK_EQ(std::get<0>(code[2]), 3);
    CB_CHECK_EQ(*std::get<2>(code[2]), 5);
    const py::bytes mine = old::aim::code_of(code);
    CB_CHECK(from_py(mine) == (cb::Bytes{0x55, 0x8B, 0xEC, 0x5D, 0xC3}));
    const auto same = old::aim::diagnose(mine, mine, code);
    CB_CHECK(same.kinds.empty() && !same.first);
    const auto d = old::aim::diagnose(py_bytes({0x55, 0x8B, 0xEC, 0x90, 0xC3}), mine, code);
    CB_CHECK_EQ(d.kinds.size(), 1);
    CB_CHECK(d.first == std::optional<int64_t>(5));
    CB_CHECK_EQ(from_py(old::aim::_kind(py::list<py::str>{py::str("jl T")}, py::list<py::str>{py::str("jb T")})), "sign");
    const auto ops = old::aim::_get_opcodes(py::list<py::str>{py::str("a"), py::str("b")}, py::list<py::str>{py::str("a"), py::str("c")});
    CB_CHECK_EQ(ops.size(), 2);
    CB_CHECK_EQ(from_py(std::get<0>(ops[1])), "replace");
});

CB_TEST("exact.shims.standin_records", [] {
    const py::bytes ix = old::standin::_ix(300);
    CB_CHECK(from_py(ix) == (cb::Bytes{0x81, 0x2C}));
    const auto [index, next] = old::standin::_idx(ix, 0);
    CB_CHECK_EQ(index, 300);
    CB_CHECK_EQ(next, 2);
    old::standin::Fixup f;
    f.M = 1; f.loc = 1; f.fm = 5; f.tm = 2; f.ti = 3; f.P = 1; f.at = 0x104;
    py::list<old::standin::Fixup> list;
    list.append(f);
    const py::bytes record = old::standin::fixrec(list, 0x100);
    old::standin::Threads threads;
    const cb::Bytes whole = from_py(record);   // type, length, payload, checksum
    const py::bytes payload = to_py(cb::ByteView(whole).subspan(3, whole.size() - 4));
    const auto back = old::standin::fixups(payload, threads);
    CB_CHECK_EQ(back.size(), 1);
    CB_CHECK_EQ(back[0].off, 4);
    CB_CHECK_EQ(back[0].tm, 2);
    CB_CHECK_EQ(*back[0].ti, 3);
});

struct OldStandinProject final : old::standin::Project {
    py::bytes img = to_py(cb::ByteView(cb::crypto::from_hex(cb::text::trim(cb::fs::read_text(data("standin/program.hex"))))));
    py::set<int64_t> rel{int64_t(0x10B), int64_t(0x10E)};
    py::bytes image() const override { return img; }
    old::compare::Symbols symbols() const override {
        old::compare::Symbols s;
        s.set(py::str("_ext"), std::make_tuple(py::str("C"), int64_t(0x180)));
        s.set(py::str("_far_fn"), std::make_tuple(py::str("C"), int64_t(0x150)));
        s.set(py::str("_f1"), std::make_tuple(py::str("C"), int64_t(0x100)));
        return s;
    }
    int64_t loadseg() const override { return 0x10; }
    int64_t dgroup() const override { return 0x1A0; }
    int64_t code_base(int64_t) const override { return 0; }
    const py::set<int64_t>& relocs() const override { return rel; }
    std::optional<py::list<int64_t>> packed_relocs() const override { return std::nullopt; }
};

CB_TEST("exact.shims.standin_rebuild", [] {
    const OldStandinProject prj;
    const py::bytes base = to_py(cb::ByteView(cb::fs::read_file(data("standin/base.obj"))));
    py::list<int64_t> addrs;
    addrs.append(0x100); addrs.append(0x108); addrs.append(0x111);
    const auto [raw, stand] = old::standin::rebuild(prj, base, addrs, 0x112, py::str("MODX"), py::list<py::bytes>(), decoder());
    CB_CHECK(from_py(raw) == cb::fs::read_file(data("standin/standins.obj")));
    CB_CHECK_EQ(stand.size(), 1);
    CB_CHECK_EQ(from_py(stand[0]), "_f2");
});

struct OldToolchain final : old::modules::Toolchain {
    std::optional<py::str> p_get(const py::str& key) const override {
        if (key == py::str("file_switches")) return py::str("switches");
        if (key == py::str("segment_switch")) return py::str("-NT {name}_TEXT");
        if (key == py::str("switches")) return py::str("-Zi");
        return std::nullopt;
    }
    std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const py::dict<py::str, py::bytes>&, const py::list<py::str>&,
                                                          const std::optional<py::str>&) const override {
        last = from_py(src);
        return {std::nullopt, py::str("fatal error")};
    }
    mutable cb::Bytes last;
};

struct OldModulesProject final : old::modules::Project {
    std::shared_ptr<OldToolchain> tc = std::make_shared<OldToolchain>();
    py::set<int64_t> rel;
    py::str src() const override { return to_py(cb::fs::to_text(data("modules/src"))); }
    py::str path(const py::str& name) const override { return to_py(cb::fs::to_text(data("modules/src") / from_py(name))); }
    int64_t code_end() const override { return 0; }
    int64_t own_end() const override { return 0x2000; }
    std::optional<py::str> switches() const override { return std::nullopt; }
    std::optional<py::str> cfg_str(const py::str& key) const override {
        if (key == py::str("first_segment")) return py::str("SEGONE");
        return std::nullopt;
    }
    py::list<int64_t> code_segments() const override { py::list<int64_t> l; l.append(0); l.append(0x1100); return l; }
    std::shared_ptr<old::modules::Toolchain> toolchain() const override { return tc; }
    py::dict<py::str, py::bytes> headers() const override { return {}; }
    py::list<py::str> include() const override { return {}; }
    py::list<std::shared_ptr<const old::modules::Project>> also_sets() const override { return {}; }
    py::bytes image() const override { return py::bytes(""); }
    old::compare::Symbols symbols() const override { return {}; }
    int64_t dgroup() const override { return 0; }
    int64_t loadseg() const override { return 0; }
    int64_t code_base(int64_t) const override { return 0; }
    const py::set<int64_t>& relocs() const override { return rel; }
};

CB_TEST("exact.shims.modules", [] {
    const OldModulesProject prj;
    const auto rows = old::modules::rows(prj);
    CB_CHECK_EQ(rows.size(), 6);
    CB_CHECK_EQ(from_py(std::get<0>(rows[0])), "main.c");
    CB_CHECK_EQ(*std::get<1>(rows[0]), 0x1000);
    CB_CHECK_EQ(*std::get<2>(rows[0]), 0x1200);
    CB_CHECK_EQ(*std::get<3>(rows[0]), 0x100);
    CB_CHECK_EQ(from_py(*std::get<4>(rows[0])), "-Ox");
    CB_CHECK_EQ(*old::modules::lib_start(prj), 0x1800);
    CB_CHECK_EQ(old::modules::parts(prj, py::str("mix.c")).size(), 1);
    CB_CHECK_EQ(old::modules::starts(prj).size(), 5);
    const auto [text, addrs, flags] = *old::modules::source(prj, rows[0]);
    CB_CHECK_EQ(from_py(text), cb::fs::read_text(data("modules/golden_main.c.txt")));
    CB_CHECK_EQ(addrs.size(), 2);
    CB_CHECK(flags.contains(py::str("-Ox")));
    CB_CHECK(!old::modules::source(prj, rows[2]).has_value());   // kit.obj
    CB_CHECK_EQ(from_py(old::modules::deftypes(prj.src()).at(py::str("f01010"))), "char *");
    const auto [object, console, where] = old::modules::compile_module(prj, rows[0]);
    CB_CHECK(!object.has_value());
    CB_CHECK_EQ(from_py(console), "fatal error");
    CB_CHECK_EQ(where.size(), 2);
    CB_CHECK_EQ(std::string(prj.tc->last.begin(), prj.tc->last.end()), cb::fs::read_text(data("modules/golden_main.c.txt")));
    const auto checked = old::modules::check(prj, rows[0], decoder());
    CB_CHECK(checked.error.has_value());
    CB_CHECK(old::modules::check(prj, rows[2], decoder()).obj);
    CB_CHECK(!old::modules::first_segment(prj, 0x1200).has_value());
    CB_CHECK_EQ(from_py(*old::modules::first_segment(prj, 0x1000)), "SEGONE");
});

CB_TEST("exact.shims.pascal_join", [] {
    old::pascal_join::Project prj;
    prj.dir = to_py(cb::fs::to_text(data("pascal")));
    prj.cfg.kinds = py::str("kinds.json");
    prj.cfg.procedures = py::str("procedures.json");
    prj.cfg.units.set(py::str("MAIN"), 1);
    old::pascal_join::CompilandCfg u;
    u.dir = py::str("main");
    u.kind = py::str("PROGRAM");
    u.main = py::str("0x1100");   // (a text: int(text, 0))
    u.parts = 2;
    prj.cfg.compilands.set(py::str("MAIN"), u);
    prj.toolchain = [] {
        old::pascal_join::Toolchain t;
        t.compile = [](const py::bytes&, const py::str&) { return std::make_tuple(std::optional<py::bytes>(), py::str("error 1\nfatal")); };
        return t;
    };
    prj.symbols = [] { return old::compare::Symbols(); };
    prj.compare.code_base = [](int64_t) { return int64_t(0); };
    prj.compare.relocs = []() -> const py::set<int64_t>& { static const py::set<int64_t> none; return none; };
    prj.decode = decoder();
    const old::pascal_join::Compiland c(prj, py::str("MAIN"), u);
    CB_CHECK_EQ(c.dirs.size(), 1);
    const auto parts = c.parts();
    CB_CHECK_EQ(parts.size(), 2);
    CB_CHECK_EQ(from_py(c.source(parts[0])), cb::fs::read_text(data("pascal/golden_part0.pas")));
    CB_CHECK_EQ(c.names_of(parts[0]).size(), 3);
    CB_CHECK_EQ(c.addr_of(py::str("01010")), 0x1010);
    const auto procs = old::pascal_join::_procedures(prj);
    CB_CHECK_EQ(procs.size(), 3);
    CB_CHECK_EQ(*std::get<0>(procs.at(0x1000)), 6);
    // (`run` resolves comparer names to the old habits ports, which cb_tests cannot link: compat/shim_check_exact.cpp
    // keeps it compiling)
});

}  // namespace
