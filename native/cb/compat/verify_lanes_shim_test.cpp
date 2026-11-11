// The shims at the old header paths for verify/lanes/image_lane and verify/lanes/dos: the old py:: API over the new lanes.
// (Built at /W3 without /WX like the other compat tests: the old layer is not warning-clean.)
#include "codebreaker/verify/lanes/dos.hpp"
#include "codebreaker/verify/lanes/image_lane.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace il = codebreaker::verify::lanes::image_lane;
namespace dos = codebreaker::verify::lanes::dos;

namespace {

il::Written image_written(const py::str& out_dir, bool linked, bool same) {
    const std::string dir = out_dir.utf8();
    fs::write_text(std::filesystem::path(dir) / "prog.asm", "; source\r\n");
    fs::write_file(std::filesystem::path(dir) / "prog.gb", Bytes{1, 2, 3, 4});
    il::Written w;
    w.source = py::str(dir + "/prog.asm");
    py::dict<int64_t, il::Function> fns;
    fns.set(0x20, il::Function{py::str("second")});
    fns.set(0x10, il::Function{py::str("first")});
    w.functions = fns;
    w.linked = linked;
    w.same = same;
    w.notes = py::list<py::str>{py::str("note one"), py::str("note two")};
    w.built = py::str(dir + "/prog.gb");
    return w;
}

il::Lane make_lane(bool linked, bool same) {
    il::State s0, s1;
    s0.set(py::str("a"), 0);
    s1.set(py::str("a"), 5);
    return il::Lane(
        py::str("gb"), 3, py::list<il::State>{s0, s1},
        [](const py::str& subject, int64_t entry, const il::State& state) {
            // a run shows how it ended, and the byte at `entry` plus A: the files are 4 bytes, so entry is read mod 4
            (void)subject;
            return il::Effects{il::Effect(py::str("ret")), il::Effect(entry + state.at(py::str("a")))};
        },
        py::list<py::str>{py::str("end"), py::str("A")},
        [](const il::State& st) { return py::str("from A=") + py::percent(py::str("%02X"), st.at(py::str("a"))); },
        [linked, same]() -> il::Writer {
            return [linked, same](const py::str&, const py::str& out_dir) { return image_written(out_dir, linked, same); };
        });
}

}  // namespace

CB_TEST("compat.verify_lanes.image_lane_old_api", [] {
    fs::ScopedTempDir tmp("cb-shim-lane");
    const il::Lane lane = make_lane(true, true);
    CB_CHECK_EQ(lane.KIND.utf8(), "gb");
    CB_CHECK_EQ(lane.REV, 3);
    const auto [digest, settings] = lane.profile();
    CB_CHECK_EQ(digest.utf8().size(), 12u);
    CB_CHECK(settings.utf8().starts_with("gb rev 3 states"));

    const py::str work = py::str(tmp.path().generic_string());
    const il::Prepared m = lane.prepare(py::str("module.gb"), work);
    CB_CHECK_EQ(py::len(m.order), 2);
    CB_CHECK_EQ(m.order[0].utf8(), "first");              // by address
    CB_CHECK_EQ(m.key.at(py::str("second")).utf8(), "0020");
    CB_CHECK_EQ(m.by_name.at(py::str("second")), 0x20);
    CB_CHECK_EQ(m.texts.at(py::str("first")).utf8(), "; first, in prog.asm");
    CB_CHECK_EQ(m.header.utf8(), "; source\r\n");
    CB_CHECK_EQ(m.sig.utf8().size(), 40u);
    CB_CHECK(m.written.linked.value_or(false));

    const il::Built b = lane.build(m, py::str("module.gb"), work);
    CB_CHECK(b.context.has_value());
    CB_CHECK(b.context->same);
    CB_CHECK_EQ(py::len(b.items), 2);
    CB_CHECK_EQ(std::get<0>(b.items.at(py::str("first"))).utf8(), "0010");
    CB_CHECK_EQ(std::get<1>(b.items.at(py::str("first"))), 0x10);
    CB_CHECK(b.missing.empty());
    const il::Verdict v = lane.judge(b.context, b.items.at(py::str("first")));
    CB_CHECK_EQ(std::get<0>(v).utf8(), "equivalent");
    CB_CHECK_EQ(std::get<3>(v).utf8(), il::SAME.utf8());
});

CB_TEST("compat.verify_lanes.image_lane_old_api_when_the_program_did_not_build_or_differs", [] {
    fs::ScopedTempDir tmp("cb-shim-lane");
    const py::str work = py::str(tmp.path().generic_string());
    {
        const il::Lane lane = make_lane(false, false);
        const il::Prepared m = lane.prepare(py::str("module.gb"), work);
        const il::Built b = lane.build(m, py::str("module.gb"), work);
        CB_CHECK(!b.context.has_value());
        CB_CHECK_EQ(py::len(b.missing), 2);
        CB_CHECK_EQ(b.missing.at(py::str("first")).utf8(), "note two");
        CB_CHECK_THROWS_AS(py::TypeError, lane.judge(b.context, il::Item(py::str("0010"), 0x10)));
    }
    {
        // edited: the same effects everywhere (entry and A are what the fake effects show) -> equivalent, with the old detail
        const il::Lane lane = make_lane(true, false);
        const il::Prepared m = lane.prepare(py::str("module.gb"), work);
        const il::Built b = lane.build(m, py::str("module.gb"), work);
        const il::Verdict v = lane.judge(b.context, b.items.at(py::str("second")));
        CB_CHECK_EQ(std::get<0>(v).utf8(), "equivalent");
        CB_CHECK_EQ(std::get<3>(v).utf8(), "the same effects from 2 register states");
    }
});

namespace {

dos::Written dos_written(const py::str& out_dir, bool linked, bool same) {
    const std::string dir = out_dir.utf8();
    fs::write_text(std::filesystem::path(dir) / "prog.asm", "mov ax, 1\r\n");
    dos::Written w;
    w.source = py::str(dir + "/prog.asm");
    py::dict<int64_t, dos::Function> fns;
    fns.set(0x4F0, dos::Function{py::str("b")});
    fns.set(0x10, dos::Function{py::str("a")});
    w.functions = fns;
    w.linked = linked;
    w.same = same;
    w.notes = py::list<py::str>{py::str("linked"), py::str("the relocation tables differ")};
    return w;
}

void install(bool linked, bool same) {
    dos::Packager p;
    p.tools = [] { return py::dict<py::str, py::str>(); };
    p.writer = [linked, same](const py::str&, const py::str& out) { return dos_written(out, linked, same); };
    dos::set_packager(p);
}

}  // namespace

CB_TEST("compat.verify_lanes.dos_old_api", [] {
    fs::ScopedTempDir tmp("cb-shim-dos");
    const py::str work = py::str(tmp.path().generic_string());
    install(true, true);
    CB_CHECK_EQ(dos::KIND.utf8(), "dos");
    CB_CHECK_EQ(dos::REV, 93);
    CB_CHECK_EQ(std::get<1>(dos::profile()).utf8(), "dos rev 93 tools");
    const dos::Prepared m = dos::prepare(py::str("prog.exe"), work);
    CB_CHECK_EQ(m.order[0].utf8(), "a");
    CB_CHECK_EQ(m.key.at(py::str("b")).utf8(), "004f0");
    CB_CHECK_EQ(m.texts.at(py::str("a")).utf8(), "; a, in prog.asm");
    const dos::Built b = dos::build(m, py::str("prog.exe"), work);
    CB_CHECK(std::get<0>(b.context));
    CB_CHECK_EQ(py::len(b.items), 2);
    CB_CHECK_EQ(std::get<0>(dos::judge(b.context, b.items.at(py::str("a")))).utf8(), "equivalent");

    install(true, false);   // linked, but not the original
    const dos::Built d = dos::build(dos::prepare(py::str("prog.exe"), work), py::str("prog.exe"), work);
    const dos::Verdict v = dos::judge(d.context, d.items.at(py::str("b")));
    CB_CHECK_EQ(std::get<0>(v).utf8(), "divergent");
    CB_CHECK_EQ(std::get<1>(v).utf8(), "image-differs");
    CB_CHECK_EQ(std::get<3>(v).utf8(), "the relocation tables differ");

    install(false, false);  // not built
    const dos::Built n = dos::build(dos::prepare(py::str("prog.exe"), work), py::str("prog.exe"), work);
    CB_CHECK(n.items.empty());
    CB_CHECK_EQ(n.missing.at(py::str("a")).utf8(), "the relocation tables differ");
});

CB_TEST("compat.verify_lanes.dos_asking_before_a_packager_is_set", [] {
    dos::set_packager(dos::Packager{});
    CB_CHECK_THROWS_AS(py::RuntimeError, dos::profile());
});
