// cb-unity: alone
// The CD-i writer over a small module and a disc holding it: the source it writes, built again by the one assembler (the 68000 SLEIGH description), puts the module's bytes back.

#include "source/cdi_writer.hpp"

#include "source/risc_source.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/cdi_test_image.hpp"

namespace cb::source::cdi_writer {

namespace ti = formats::cdi::test_image;

CB_TEST("source.cdi_writer.a_disc_comes_back_as_source_that_builds_the_same_disc", [] {
    fs::ScopedTempDir dir("cb_cdi_disc");
    const image_writer::Written r = write(formats::cdi::parse(ti::disc()), "game.iso", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x10040, "main_main"}, {0x10050, "sub_010050"}}));
    CB_CHECK(r.built.has_value() && fs::read_file(*r.built) == ti::disc());
    const std::string s = fs::read_text(r.source);
    for (const char* want : {".org 0x010000", ".half 0x4afc", "MCD212_CSR1", "F_Exit", "I_Write", "trap #0x0", ".half F_Exit", "main_main", "bsr"}) CB_CHECK(s.find(want) != std::string::npos);
});

CB_TEST("source.cdi_writer.an_edited_source_builds_a_different_module", [] {
    fs::ScopedTempDir dir("cb_cdi_edit");
    const image_writer::Written r = write(formats::cdi::parse(ti::module()), "main.mod", dir.path());
    CB_CHECK(r.linked && r.same);
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("#0x1234");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 7, "#0x1235");
    CB_CHECK_EQ(risc_source::assemble("m68k", s, ".")[0x43], 0x35);   // (the module at 0x10000 is the first bytes built: 0x10043 - 0x10000)
});

}  // namespace cb::source::cdi_writer
