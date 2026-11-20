// The Lynx writer: a cartridge and a BS93 object come back as source (the 65C02's SLEIGH description) that builds the same file.
#include "source/lynx_writer.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "emulate/lynx.hpp"
#include "formats/lynx_test_image.hpp"
#include "source/m6502_source.hpp"

namespace cb::source::lynx_writer {
namespace {

// LDA #1 / STA SPRCTL0 / JSR $040C / JMP $0408, a stray byte, STZ SPRGO / INC A / RTS: two functions and a register each way.
const Bytes kProgram = {0xA9, 0x01, 0x8D, 0x80, 0xFC, 0x20, 0x0C, 0x04, 0x4C, 0x08, 0x04, 0xEA, 0x9C, 0x91, 0xFC, 0x1A, 0x60};

const Booter kBoot = [](formats::lynx::Program& p) {
    const emulate::lynx::Boot b = emulate::lynx::boot(p);
    p.set_loaded(b.segments, b.entries);
};

bool has_line(const std::string& source, std::string_view line) {
    for (const std::string_view l : text::lines(source)) {
        if (text::trim(l) == line) return true;
    }
    return false;
}

CB_TEST("source.lynx_writer.a_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_lynx");
    const Bytes file = formats::lynx_test_image::cartridge(kProgram, "Demo");
    fs::write_file(dir.path() / "demo.lnx", file);
    const image_writer::Written r = reconstruct(dir.path() / "demo.lnx", dir.path() / "out", kBoot);
    CB_CHECK(r.notes.empty());
    CB_CHECK(r.linked && r.same);
    CB_CHECK((r.functions == image_writer::Functions{{0x0400, "start"}, {0x040C, "sub_040C"}}));
    CB_CHECK_EQ(r.code_bytes, 16u);   // LDA STA JSR JMP, then STZ INC RTS; the stray byte between is data
    CB_CHECK_EQ(r.image_bytes, file.size());
    const std::string source = fs::read_text(r.source);
    CB_CHECK(has_line(source, ".equ SPRCTL0, 0xfc80"));
    CB_CHECK(has_line(source, ".equ SPRGO, 0xfc91"));
    CB_CHECK(has_line(source, "STA SPRCTL0"));
    CB_CHECK(has_line(source, "JSR sub_040C"));
    CB_CHECK(has_line(source, "STZ SPRGO"));
    CB_CHECK(has_line(source, "INC A"));
    CB_CHECK(has_line(source, "start:"));
    CB_CHECK(has_line(source, ".org 0x0400"));   // (the segment, at the RAM address the loader copies it to)
    CB_CHECK(!r.config);
});

CB_TEST("source.lynx_writer.a_cartridge_without_a_booter_is_all_data", [] {
    fs::ScopedTempDir dir("cb_lynx_nob");
    const Bytes file = formats::lynx_test_image::cartridge(kProgram, "Demo");
    const image_writer::Written r = write(formats::lynx::parse(file), "demo.lnx", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.functions.empty());
    CB_CHECK_EQ(r.code_bytes, 0u);
});

CB_TEST("source.lynx_writer.a_bs93_object_is_its_header_its_body_and_what_follows", [] {
    fs::ScopedTempDir dir("cb_lynx_bs93");
    Bytes file = {0x80, 0x08, 0x04, 0x00, 0x00, static_cast<std::uint8_t>(kProgram.size() + 10), 'B', 'S', '9', '3'};
    file.insert(file.end(), kProgram.begin(), kProgram.end());
    file.insert(file.end(), {0xDE, 0xAD});   // bytes after the block, kept
    const image_writer::Written r = write(formats::lynx::parse(file), "demo.o", dir.path());
    CB_CHECK(r.notes.empty());
    CB_CHECK(r.linked && r.same);
    CB_CHECK((r.functions == image_writer::Functions{{0x0400, "start"}, {0x040C, "sub_040C"}}));
    const std::string source = fs::read_text(r.source);
    CB_CHECK(source.find("; TRAILER (file offset 0x") != std::string::npos);
});

CB_TEST("source.lynx_writer.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_lynx_edit");
    Bytes file = {0x80, 0x08, 0x04, 0x00, 0x00, static_cast<std::uint8_t>(kProgram.size() + 10), 'B', 'S', '9', '3'};
    file.insert(file.end(), kProgram.begin(), kProgram.end());
    const image_writer::Written r = write(formats::lynx::parse(file), "demo.o", dir.path());
    CB_CHECK(r.same);
    // the source can be edited and rebuilt: INC A becomes DEC A (a different byte, same size)
    std::string source = fs::read_text(r.source);
    const std::size_t at = source.find("INC A");
    CB_CHECK(at != std::string::npos);
    source.replace(at, 5, "DEC A");
    const Bytes rebuilt = m6502_source::assemble(m6502_source::Cpu::M65C02, source, dir.path());
    CB_CHECK_EQ(rebuilt.size(), file.size());
    CB_CHECK_EQ(rebuilt[10 + 15], 0x3A);   // DEC A
    CB_CHECK_EQ(rebuilt[10 + 16], 0x60);
});

}  // namespace
}  // namespace cb::source::lynx_writer
