// The shim at the old header path (native/typed/cpp/codebreaker/exact/habits/newcomp.hpp) over cb::exact::habits::newcomp:
// it must compile with the old layer, forward, and keep the old layer's module tables (AMAP, GWALK). Built at /W3 (the old
// layer is not warning-clean).
#include "cb/core/testing.hpp"
#include "codebreaker/exact/habits/newcomp.hpp"

CB_TEST("exact.habits.newcomp.shim.forwards_and_sets_the_tables", [] {
    namespace old_nc = codebreaker::exact::habits::newcomp;
    using cb::compat::from_py;
    using cb::compat::to_py;
    CB_CHECK_EQ(from_py(old_nc::mask(py::str("add ax, 12"))), "add ax, N");
    CB_CHECK(!old_nc::rel(to_py(cb::ByteView(cb::Bytes{0x90}))).has_value());
    CB_CHECK(old_nc::rename(py::str("mov dl, 1"), py::dict<py::str, py::str>{}).has_value());
    // mov ax,[bp+4] / mov dx,[bp+6] / add ax,dx / push ax ... against the later compiler's registers
    const cb::Bytes want = {0x8b, 0x46, 0x04, 0x8b, 0x56, 0x06, 0x01, 0xd0, 0x50, 0xc7, 0x46, 0xfe, 0x07, 0x00, 0xc3};
    const cb::Bytes got = {0x8b, 0x56, 0x04, 0x8b, 0x4e, 0x06, 0x01, 0xd1, 0x51, 0xc7, 0x46, 0xfe, 0x07, 0x00, 0xc3};
    old_nc::AMAP.clear();
    const auto r = old_nc::equivalent(to_py(cb::ByteView(want)), to_py(cb::ByteView(got)), py::set<int64_t>{});
    CB_CHECK(std::get<0>(r));
    CB_CHECK_EQ(from_py(std::get<1>(r)[0]), "registers used otherwise (AX for CX) from +0000");
    CB_CHECK_EQ(old_nc::AMAP.size(), 6);   // (each instruction of the original and where it is in the compiled code)
    CB_CHECK_EQ(old_nc::GWALK.size(), 6);
});
