// cb-unity: alone
#include "cb/formats/cdi.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/cdi_test_image.hpp"

namespace cb::formats::cdi {

namespace ti = test_image;

CB_TEST("formats.cdi.a_disc_is_searched_for_its_modules", [] {
    const Program p = parse(ti::disc());
    CB_CHECK(p.disc);
    CB_CHECK_EQ(p.modules.size(), 1u);
    const Module& m = p.modules[0];
    CB_CHECK_EQ(m.name, "main");
    CB_CHECK_EQ(m.file_offset, 4096u);
    CB_CHECK_EQ(m.size, 0x5Cu);
    CB_CHECK_EQ(m.type, 1);
    CB_CHECK_EQ(m.exec_offset, 0x40u);
    CB_CHECK_EQ(m.address, kFirstBase);
    CB_CHECK_EQ(m.entry(), 0x10040u);
    CB_CHECK_EQ(p.offset(0x10050).value(), 4096u + 0x50u);
    CB_CHECK_EQ(p.memory()[0x10000], 0x4A);
    CB_CHECK((p.entries() == std::vector<std::uint32_t>{0x10040}));
});

CB_TEST("formats.cdi.modules_one_after_another_get_addresses_of_their_own", [] {
    Bytes file = ti::module();
    const Bytes second = ti::module(3);
    file.insert(file.end(), second.begin(), second.end());
    const Program p = parse(file);
    CB_CHECK(!p.disc);
    CB_CHECK_EQ(p.modules.size(), 2u);
    CB_CHECK_EQ(p.modules[0].address, 0x10000u);
    CB_CHECK_EQ(p.modules[1].address, 0x10100u);
    CB_CHECK_EQ(p.modules[1].file_offset, 0x5Cu);
});

CB_TEST("formats.cdi.a_data_module_is_found_and_not_run", [] {
    const Program p = parse(ti::module(4));
    CB_CHECK_EQ(p.modules.size(), 1u);
    CB_CHECK(!p.modules[0].runs());
    CB_CHECK(p.placed().empty());
});

CB_TEST("formats.cdi.refuses_what_holds_no_module", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(4096, 0)));
    Bytes bad = ti::module();
    bad[0x2E] ^= 0xFF;   // the parity no longer fits
    CB_CHECK(!module_at(bad, 0).has_value());
});

CB_TEST("formats.cdi.probe", [] {
    fs::ScopedTempDir dir("cb_cdi_probe");
    fs::write_file(dir.path() / "a.iso", ti::disc());
    fs::write_file(dir.path() / "b.mod", ti::module());
    fs::write_file(dir.path() / "c.iso", Bytes(8192, 0));
    fs::write_file(dir.path() / "d.txt", ti::module());
    CB_CHECK(probe(dir.path() / "a.iso"));
    CB_CHECK(probe(dir.path() / "b.mod"));
    CB_CHECK(!probe(dir.path() / "c.iso"));
    CB_CHECK(!probe(dir.path() / "d.txt"));
    CB_CHECK(!probe(dir.path() / "missing.iso"));
});

}  // namespace cb::formats::cdi
