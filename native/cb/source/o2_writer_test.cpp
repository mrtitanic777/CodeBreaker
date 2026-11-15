// cb-unity: alone
// The Odyssey 2 writer over a small cartridge: the source it writes, built again by CodeBreaker's 8048 assembler, is the cartridge.

#include "cb/source/o2_writer.hpp"

#include "cb/build/i8048_asm.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/o2_test_image.hpp"

namespace cb::source::o2_writer {

namespace ti = formats::o2::test_image;

CB_TEST("source.o2_writer.a_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_o2_writer");
    const image_writer::Written r = write(formats::o2::parse(ti::cartridge()), "demo.o2", dir.path());
    CB_CHECK_EQ(r.notes.empty() ? std::string() : r.notes.front(), std::string());
    CB_CHECK(r.linked && r.same);
    CB_CHECK((r.functions == image_writer::Functions{{0x400, "start"}, {0x420, "sub_0420"}}));
    CB_CHECK_EQ(r.image_bytes, 0x400u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"ORG $0400", "start:", "sub_0420:", "L_040B:", "CALL sub_0420", "JNZ L_040B", "MOVX @R0,A", "MOV R0,#$10", "FILL "}) {
        if (s.find(want) == std::string::npos) CB_CHECK_EQ(std::string("missing: ") + want, std::string());
    }
});

CB_TEST("source.o2_writer.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_o2_edit");
    const image_writer::Written r = write(formats::o2::parse(ti::cartridge()), "demo.o2", dir.path());
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("ADD A,#$03");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 10, "ADD A,#$04");
    Bytes expected = ti::cartridge();
    expected[0x23] = 0x04;
    CB_CHECK(build::i8048_asm::build(s).data == expected);
});

}  // namespace cb::source::o2_writer
