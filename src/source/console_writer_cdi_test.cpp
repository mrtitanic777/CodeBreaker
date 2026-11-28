// cb-unity: alone
// The CD-i writer over a small module and a disc holding it: the source it writes, built again by CodeBreaker's assembler, puts the module's bytes back.

#include "formats/cdi.hpp"
#include "source/console_writer.hpp"

#include "source/dialect.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/cdi_test_image.hpp"
#include "source/risc_source.hpp"

namespace cb::source::console_writer {
namespace {
// The bytes written to a file in `dir` and back as source through the console writer (config/platforms/cdi.json).
[[maybe_unused]] image_writer::Written write_file_of(const fs::ScopedTempDir& dir, const Bytes& bytes, const char* name) {
    fs::write_file(dir.path() / name, bytes);
    return reconstruct("cdi", dir.path() / name, dir.path());
}
}  // namespace

namespace ti = formats::cdi::test_image;

CB_TEST("source.console_writer.cdi.a_disc_comes_back_as_source_that_builds_the_same_disc", [] {
    fs::ScopedTempDir dir("cb_cdi_disc");
    const image_writer::Written r = write_file_of(dir, ti::disc(), "game.iso");
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x10040, "main_main"}, {0x10050, "sub_010050"}}));
    CB_CHECK(r.built.has_value() && fs::read_file(*r.built) == ti::disc());
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"org $010000", "dc.w $4AFC", "MCD212_CSR1", "F_Exit", "I_Write", "TRAP #0", "dc.w F_Exit", "main_main", "BSR"}) CB_CHECK(s.find(want) != std::string::npos);
});

CB_TEST("source.console_writer.cdi.an_edited_source_builds_a_different_module", [] {
    fs::ScopedTempDir dir("cb_cdi_edit");
    const image_writer::Written r = write_file_of(dir, ti::module(), "main.mod");
    CB_CHECK(r.linked && r.same);
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("#$1234");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 6, "#$1235");
    CB_CHECK_EQ(risc_source::assemble("m68k", dialect::find("vasm-m68k").parse(s), ".")[0x43], 0x35);   // (the module at 0x10000 is the first bytes built: 0x10043 - 0x10000)
});

}  // namespace cb::source::console_writer
