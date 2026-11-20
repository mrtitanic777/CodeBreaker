// cb-unity: alone
// The Xbox One / Series writer over a small x64 PE32+ written byte by byte (formats/xboxone_test_image.hpp). The tests that build run NASM (settings: nasm, or CB_NASM) and are
// skipped where there is none.

#include "cb/source/xboxone_writer.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xboxone_test_image.hpp"
#include "cb/source/x64_writer.hpp"

namespace cb::source::xboxone_writer {

namespace xt = formats::xboxone_test;

CB_TEST("source.xboxone_writer.a_pe_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_xboxone_writer");
    const image_writer::Written r = write(formats::xboxone::parse(xt::pe()), "app.exe", dir.path());
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

CB_TEST("source.xboxone_writer.a_package_member_is_written_from_inside_its_package", [] {
    fs::ScopedTempDir dir("cb_xboxone_writer_pkg");
    fs::write_file(dir.path() / "demo.msix", xt::msix());
    const image_writer::Written r = reconstruct(dir.path() / "demo.msix", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.functions.size(), 2u);
    const std::string s = fs::read_text(r.source);
    CB_CHECK(s.find("MSIX package") != std::string::npos);
    CB_CHECK(s.find("Demo.App") != std::string::npos);
    const image_writer::Written dll = reconstruct(dir.path() / "demo.msix", dir.path() / "out_dll", "lib.dll");
    CB_CHECK(dll.same);
});

}  // namespace cb::source::xboxone_writer
