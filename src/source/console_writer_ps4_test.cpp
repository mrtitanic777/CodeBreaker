// cb-unity: alone
// The PlayStation 4 / 5 writer over a small x86-64 ELF written byte by byte (formats/orbis_test_image.hpp). The tests that build run x86asm, in this process.

#include "formats/orbis.hpp"
#include "source/console_writer.hpp"
#include "source/x64_writer.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/orbis_test_image.hpp"
#include "source/x64_writer.hpp"

namespace cb::source::console_writer {
namespace {
// The bytes written to a file in `dir` and back as source through the console writer (config/platforms/ps4.json).
[[maybe_unused]] image_writer::Written write_file_of(const fs::ScopedTempDir& dir, const Bytes& bytes, const char* name) {
    fs::write_file(dir.path() / name, bytes);
    return reconstruct("ps4", dir.path() / name, dir.path());
}
}  // namespace

namespace ot = formats::orbis_test;

CB_TEST("source.console_writer.ps4.an_elf_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_orbis_writer");
    const image_writer::Written r = write_file_of(dir, ot::elf(), "demo.elf");
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.image_bytes, ot::kElfSize);
    // the entry, the call's target and the function a relocation's pointer names are functions
    CB_CHECK_EQ(r.functions.size(), 3u);
    CB_CHECK_EQ(r.functions.at(0x1000), "_start");
    CB_CHECK_EQ(r.functions.at(0x1020), "sub_001020");
    CB_CHECK_EQ(r.functions.at(0x1040), "sub_001040");
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"bits 64", "default rel", "_start:", "sub_001020:", "call sub_001020", "call qword [rel got_sceKernelUsleep]", "got_sceKernelUsleep:", "mov eax, dword [rel D_",
                             "ELF header", "program headers", "the dynamic section", "sceKernelUsleep", "libkernel.sprx", "jne short"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

CB_TEST("source.console_writer.ps4.a_fake_self_is_written_as_the_elf_it_holds", [] {
    fs::ScopedTempDir dir("cb_orbis_writer_self");
    const Bytes elf = ot::elf();
    fs::write_file(dir.path() / "eboot.bin", ot::fself(elf));
    const image_writer::Written r = reconstruct("ps4", dir.path() / "eboot.bin", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.image_bytes, ot::elf_of_self(elf).size());
    CB_CHECK_EQ(r.functions.size(), 3u);
});

CB_TEST("source.console_writer.ps4.the_source_is_written_without_an_assembler_too", [] {
    // the source is written beside the image the writer builds (with x86asm, CodeBreaker's own assembler: always)
    fs::ScopedTempDir dir("cb_orbis_writer_text");
    x64_writer::Image image;
    image.name = "demo";
    image.file = ot::elf();
    image.runs = {{0, 0x1000, 0, false, false}, {0x1000, 0x80, 0x1000, true, true}, {0x1080, static_cast<std::uint32_t>(ot::kElfSize - 0x1080), 0, false, false}};
    image.entries = {0x1000};
    image.head = "; test";
    const image_writer::Written r = x64_writer::write(image, dir.path());
    CB_CHECK(std::filesystem::is_regular_file(r.source));
    CB_CHECK_EQ(r.functions.size(), 2u);   // _start and its call's target
    CB_CHECK(r.same);
});

}  // namespace cb::source::console_writer
