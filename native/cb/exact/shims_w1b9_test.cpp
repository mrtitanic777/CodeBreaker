// The shims at the old header paths (native/typed/cpp/codebreaker/exact/*.hpp) over the modules of this folder: they
// must compile with the old layer and forward to the new code. Built at /W3 (the old layer is not warning-clean).
#include "cb/core/testing.hpp"
#include "cb/exact/fixupp.hpp"
#include "cb/exact/omf.hpp"
#include "codebreaker/exact/recut.hpp"

namespace {

using cb::compat::from_py;
using cb::compat::to_py;

// The object of recut_test: a 40-byte code segment with segment words at 30, 12 and 4, listed last first.
cb::Bytes small_object() {
    namespace omf = cb::exact::omf;
    cb::Bytes data = {1, 0, 0};
    for (int i = 0; i < 40; ++i) data.push_back(static_cast<std::uint8_t>(i));
    std::vector<cb::exact::fixupp::Fixup> fixups;
    for (const std::uint32_t at : {30u, 12u, 4u}) {
        cb::exact::fixupp::Fixup f;
        f.segment_relative = true;
        f.location = 2;
        f.frame_index = std::uint16_t{1};
        f.target_index = std::uint16_t{1};
        f.at = at;
        fixups.push_back(f);
    }
    cb::Bytes out;
    cb::append(out, omf::record(0x80, cb::Bytes{1, 't'}));
    cb::append(out, omf::record(0x96, cb::Bytes{0, 5, '_', 'T', 'E', 'X', 'T', 4, 'C', 'O', 'D', 'E'}));
    cb::append(out, omf::record(0x98, cb::Bytes{0x28, 40, 0, 2, 3, 1}));
    cb::append(out, omf::record(0x90, cb::Bytes{0, 1, 5, '_', 'm', 'a', 'i', 'n', 0, 0, 0}));
    cb::append(out, omf::record(0xA0, data));
    cb::append(out, cb::exact::fixupp::encode(fixups, 0));
    cb::append(out, omf::record(0x8A, cb::Bytes{0}));
    return out;
}

// An old-layer project: _main at 0x100; the original's relocations are the packed list.
class OldProject final : public codebreaker::exact::recut::Project {
  public:
    codebreaker::exact::compare::Symbols symbols() const override {
        codebreaker::exact::compare::Symbols s;
        s.set(py::str("_main"), std::make_tuple(py::str("C"), int64_t{0x100}));
        return s;
    }
    std::optional<py::list<int64_t>> packed_relocs() const override { return py::list<int64_t>{0x10C, 0x104, 0x11E}; }
    py::list<int64_t> header() const override { return py::list<int64_t>(); }
    py::bytes file() const override { return py::bytes(); }
};

CB_TEST("exact.shims.recut", [] {
    const OldProject project;
    const cb::Bytes raw = small_object();
    const py::bytes out = codebreaker::exact::recut::recut(project, to_py(cb::ByteView(raw)));
    CB_CHECK(from_py(out) != raw);   // cut where the relocations show
    CB_CHECK_EQ(cb::exact::omf::read(from_py(out)).fixups.size(), std::size_t{3});
    const py::list<int64_t> order = codebreaker::exact::recut::order(project);
    CB_CHECK_EQ(order.size(), int64_t{3});
    CB_CHECK_EQ(order[0], int64_t{0x10C});
    // a Python-side caller sees the new code's errors as the old exceptions
    cb::Bytes early;   // a fix-up record before any data record
    cb::append(early, cb::exact::omf::record(0x80, cb::Bytes{1, 't'}));
    cb::append(early, cb::exact::omf::record(0x9C, cb::Bytes{0xC4, 0x04, 0x00, 0x01, 0x01}));
    cb::append(early, cb::exact::omf::record(0x8A, cb::Bytes{0}));
    CB_CHECK_THROWS_AS(py::Exception, codebreaker::exact::recut::recut(project, to_py(cb::ByteView(early))));
});

}  // namespace
