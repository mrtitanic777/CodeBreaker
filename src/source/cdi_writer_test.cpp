// cb-unity: alone
// The CD-i writer over a small module and a disc holding it: the source it writes, built again by CodeBreaker's assembler, puts the module's bytes back.

#include "source/cdi_writer.hpp"

#include "build/m68k_asm.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/cdi_test_image.hpp"
#include "source/isa.hpp"

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
    for (const char* want : {"org $010000", "dc.w $4AFC", "MCD212_CSR1", "F_Exit", "I_Write", "TRAP #0", "dc.w F_Exit", "main_main", "BSR"}) CB_CHECK(s.find(want) != std::string::npos);
});

CB_TEST("source.cdi_writer.an_edited_source_builds_a_different_module", [] {
    fs::ScopedTempDir dir("cb_cdi_edit");
    const image_writer::Written r = write(formats::cdi::parse(ti::module()), "main.mod", dir.path());
    CB_CHECK(r.linked && r.same);
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("#$1234");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 6, "#$1235");
    const build::m68k_asm::Assembler assembler(isa::m68k_encoder());
    CB_CHECK_EQ(assembler.build(s)[0x10043], 0x35);
});

}  // namespace cb::source::cdi_writer
