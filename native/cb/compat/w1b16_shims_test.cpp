// The shims at the old header paths of invert/early_c_forward, formats/xbox/crypto/nand_tool, formats/xbox/god and java/probe keep the
// old API (py::bytes, py::list, the old IR's class handles) over the new modules. Builds at /W3 (the old py:: layer is not warning-clean).
// What reaches the native reader's device registry (SvodReader, the GDF-based operations of god's main) and the corpus settings is only
// compiled, by shim_check_w1b16.cpp.
#include "codebreaker/formats/xbox/crypto/nand_tool.hpp"
#include "codebreaker/formats/xbox/god.hpp"
#include "codebreaker/invert/early_c_forward.hpp"
#include "codebreaker/java/probe.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace old_forward = codebreaker::invert::early_c_forward;
namespace old_nand = codebreaker::formats::xbox::crypto::nand_tool;
namespace old_god = codebreaker::formats::xbox::god;
namespace old_probe = codebreaker::java::probe;

namespace {

py::bytes bytes_of(const cb::Bytes& b) { return cb::compat::to_py(cb::ByteView(b)); }

cb::Bytes testdata(const char* name) { return cb::fs::read_file(cb::testing::data_path(std::string("formats/testdata/god/") + name)); }

}  // namespace

CB_TEST("compat.w1b16.early_c_forward_shim", [] {
    // int f(a, b) { return a + b; }: FUNC-BEGIN (function 6), add(a=7, b=8) -> temp, RETURN temp
    auto node = [](int id) { return cb::Bytes{0x0a, 0, 0, static_cast<std::uint8_t>(id), 0, 2, 0, 0, 0, 0, 0}; };
    cb::Bytes temp = {0x0e, 0x40, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0};
    cb::Bytes add;
    for (const auto& part : {node(7), cb::Bytes{0}, node(8), cb::Bytes{0}, temp}) cb::append(add, part);
    py::list<old_forward::QuadRecord> function;
    function.append(old_forward::QuadRecord(0x23, bytes_of({0x0a, 0, 0, 6, 0, 0xff, 0xff, 0, 0, 0, 0})));
    function.append(old_forward::QuadRecord(0x13, bytes_of(add)));
    function.append(old_forward::QuadRecord(0x24, bytes_of(temp)));
    CB_CHECK_EQ(old_forward::parse_funcbegin(std::get<1>(function[0])), 6);
    const py::bytes code = old_forward::model(function);
    CB_CHECK_EQ(cb::compat::from_py(code).size(), std::size_t{26});
    CB_CHECK_EQ(cb::compat::from_py(code)[0], 0x55);
    // a function the model does not cover raises as the Python did (NotImplementedError)
    py::list<old_forward::QuadRecord> unmodelled;
    unmodelled.append(function[0]);
    unmodelled.append(old_forward::QuadRecord(0x00, bytes_of(cb::Bytes{0xa, 0, 0, 7, 0, 2, 0, 0, 0, 0, 0, 4, 0x80, 0, 1, 0, 0, 0})));
    CB_CHECK_THROWS_AS(py::NotImplementedError, (void)old_forward::model(unmodelled));
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_forward::parse_funcbegin(bytes_of(cb::Bytes{1})));
});

CB_TEST("compat.w1b16.nand_tool_shim", [] {
    cb::Bytes raw(512 * 3);
    for (std::size_t i = 0; i < raw.size(); ++i) raw[i] = static_cast<std::uint8_t>(i * 31 + i / 512);
    const py::bytes image = old_nand::add_spare(bytes_of(raw));
    CB_CHECK_EQ(image.size(), 3 * old_nand::FULL);
    CB_CHECK_EQ(old_nand::verify_ecc(image).size(), 0);
    CB_CHECK(old_nand::strip_spare(image) == bytes_of(raw));
    // a template's spare bytes are kept, an empty one is none
    CB_CHECK(old_nand::add_spare(bytes_of(raw), py::bytes()) == image);
    // damage, find, repair
    cb::Bytes bad = cb::compat::from_py(image);
    bad[528 + 5] ^= 1;
    const py::list<int64_t> found = old_nand::verify_ecc(bytes_of(bad));
    CB_CHECK_EQ(found.size(), 1);
    CB_CHECK_EQ(found[0], 1);
    CB_CHECK_EQ(old_nand::verify_ecc(old_nand::fix_all_ecc(bytes_of(bad))).size(), 0);
    // the page ECC of the first page, from the Python-side computation
    cb::Bytes page(512);
    for (std::size_t i = 0; i < page.size(); ++i) page[i] = static_cast<std::uint8_t>(i);
    CB_CHECK_EQ(old_nand::calc_page_ecc(bytes_of(page), bytes_of(cb::Bytes(16, 0))), 0x2ef3873);
    const auto [data, spare] = old_nand::fix_page_ecc(bytes_of(page), bytes_of(cb::Bytes(16, 0)));
    CB_CHECK_EQ(old_nand::_stored_ecc(py::bytes(spare)), 0x2ef3873);
    CB_CHECK_EQ(data.size(), 512);
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_nand::calc_page_ecc(bytes_of(cb::Bytes(10, 0)), bytes_of(cb::Bytes(16, 0))));
    CB_CHECK_EQ(old_nand::DESCRIPTION.utf8().substr(0, 25), "Xbox 360 NAND image build");
});

CB_TEST("compat.w1b16.god_shim", [] {
    const cb::Bytes tpl = testdata("empty_live.bin");
    cb::fs::ScopedTempDir tmp("w1b16god");
    // write_god forwards: the volume is read through the old-style callback
    const auto volume = [](int64_t offset, int64_t length) {
        cb::Bytes out(static_cast<std::size_t>(length));
        for (std::size_t i = 0; i < out.size(); ++i) out[i] = static_cast<std::uint8_t>((offset + static_cast<int64_t>(i)) * 3);
        return bytes_of(out);
    };
    const auto [header, data_dir, parts] = old_god::write_god(volume, 10 * old_god::BLOCK, cb::compat::to_py(cb::fs::to_text(tmp.path())),
                                                            std::optional<py::bytes>(bytes_of(tpl)), 0x4D530064, 0xCAFEF00D, 0x7000, py::str("Game"));
    CB_CHECK_EQ(parts, 1);
    CB_CHECK(cb::compat::from_py(header).ends_with("CAFEF00D"));
    CB_CHECK(cb::compat::from_py(data_dir).ends_with("CAFEF00D.data"));
    CB_CHECK_EQ(cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(header))).size(), tpl.size());
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_god::write_god(volume, 10, cb::compat::to_py(cb::fs::to_text(tmp.path())), std::nullopt, 1, 2));
    // the NXE2GOD transform
    cb::Bytes installed = tpl;
    installed[0x346] = 0x40;
    CB_CHECK(old_god::is_nxe_install(bytes_of(installed)));
    CB_CHECK(!old_god::is_nxe_install(bytes_of(tpl)));
    CB_CHECK_EQ(old_god::install_to_god(bytes_of(installed)).size(), installed.size());
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_god::install_to_god(bytes_of(cb::Bytes(100, 0))));
    CB_CHECK_EQ(old_god::BLOCKS_PER_PART, 0xA1C4);
});

CB_TEST("compat.w1b16.probe_shim_helpers", [] {
    // the pure helpers the judge's callers use
    py::list<py::str> types;
    types.append(py::str("boolean"));
    types.append(py::str("boolean"));
    const auto four = old_probe::cases(types);
    CB_CHECK_EQ(four.size(), 4);
    CB_CHECK_EQ(four[0][0].utf8(), "true");
    CB_CHECK_EQ(four[3][1].utf8(), "false");
    CB_CHECK_EQ(old_probe::_java_literal(py::str("String"), py::str("a\"b")).utf8(), "\"a\\u0022b\"");
    CB_CHECK_EQ(old_probe::_java_literal(py::str("byte"), py::str("-1")).utf8(), "(byte)-1");
    const auto results = old_probe::_results(py::str("0.0|1f\n0.1|throw\n\n"));
    CB_CHECK_EQ(results.size(), 2);
    CB_CHECK_EQ(results.at(py::str("0.1")).utf8(), "throw");
    CB_CHECK(old_probe::same(py::str("double"), py::str("7ff8000000000000"), py::str("7ff8000000000001")));
    CB_CHECK(!old_probe::same(py::str("int"), py::str("1"), py::str("2")));
    CB_CHECK_EQ(old_probe::REV, 11);
    old_probe::CXX_STD = py::str("c++17");
    CB_CHECK_EQ(old_probe::CXX_STD.utf8(), "c++17");
    old_probe::CXX_STD = py::str("c++03");
});
