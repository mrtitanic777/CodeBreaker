// cb-unity: alone
// The Xbox One / Series writer over a small x64 PE32+ written byte by byte (formats/xboxone_test_image.hpp). The tests that build run x86asm, in this process.

#include "formats/xboxone.hpp"
#include "source/console_writer.hpp"
#include "source/x64_writer.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/xboxone_test_image.hpp"
#include "source/x64_writer.hpp"

namespace cb::source::console_writer {
namespace {
// The bytes written to a file in `dir` and back as source through the console writer (config/platforms/xboxone.json).
[[maybe_unused]] image_writer::Written write_file_of(const fs::ScopedTempDir& dir, const Bytes& bytes, const char* name) {
    fs::write_file(dir.path() / name, bytes);
    return reconstruct("xboxone", dir.path() / name, dir.path());
}
}  // namespace

namespace xt = formats::xboxone_test;

CB_TEST("source.console_writer.xboxone.a_pe_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_xboxone_writer");
    const image_writer::Written r = write_file_of(dir, xt::pe(), "app.exe");
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.image_bytes, xt::kPeSize);
    // the entry point, and the function only the exception directory begins
    CB_CHECK_EQ(r.functions.size(), 2u);
    CB_CHECK_EQ(r.functions.at(0x1000), "entry");
    CB_CHECK_EQ(r.functions.at(0x1020), "sub_001020");
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"bits 64", "entry:", "call qword [rel __imp_Sleep]", "__imp_Sleep:", "mov eax, dword [rel D_", "lea eax, [rcx + rdx]", "PE32+ image", "KERNEL32.dll", "section .s1"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

CB_TEST("source.console_writer.xboxone.a_package_member_is_written_from_inside_its_package", [] {
    fs::ScopedTempDir dir("cb_xboxone_writer_pkg");
    fs::write_file(dir.path() / "demo.msix", xt::msix());
    const image_writer::Written r = reconstruct("xboxone", dir.path() / "demo.msix", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.functions.size(), 2u);
    const std::string s = fs::read_text(r.source);
    CB_CHECK(s.find("MSIX package") != std::string::npos);
    CB_CHECK(s.find("Demo.App") != std::string::npos);
    const image_writer::Written dll = write(describe("xboxone"), load_xboxone(dir.path() / "demo.msix", "lib.dll"), "demo.msix", dir.path() / "out_dll");
    CB_CHECK(dll.same);
});

}  // namespace cb::source::console_writer
