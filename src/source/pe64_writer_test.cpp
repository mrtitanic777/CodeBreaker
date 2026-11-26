// cb-unity: alone
// The 64-bit Windows writer over a small x64 PE32+ written byte by byte (formats/xboxone_test_image.hpp, without the app-sandbox flag). The tests that build
// run x86asm, in this process.

#include "source/pe64_writer.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/xboxone_test_image.hpp"

namespace cb::source::pe64_writer {

namespace xt = formats::xboxone_test;

CB_TEST("source.pe64_writer.a_64_bit_windows_program_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_pe64_writer");
    fs::write_file(dir.path() / "tool.exe", xt::pe(false));
    const image_writer::Written r = reconstruct(dir.path() / "tool.exe", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK_EQ(r.image_bytes, xt::kPeSize);
    CB_CHECK_EQ(r.functions.size(), 2u);   // the entry point, and the function only the exception directory begins
    CB_CHECK_EQ(r.source.filename().string(), "tool.asm");
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"bits 64", "entry:", "call qword [rel __imp_Sleep]", "64-bit Windows program", "PE32+ image", "KERNEL32.dll"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

CB_TEST("source.pe64_writer.the_name_and_the_kind_are_the_callers", [] {
    fs::ScopedTempDir dir("cb_pe64_writer_named");
    Options o;
    o.name = "tool.exe";
    o.kind = "a test program";
    const image_writer::Written r = write(xt::pe(false), "tool.exe", dir.path(), o);
    CB_CHECK(r.same);
    CB_CHECK_EQ(r.source.filename().string(), "tool.exe.asm");
    CB_CHECK(fs::read_text(r.source).find("; a test program,") != std::string::npos);
});

CB_TEST("source.pe64_writer.a_32_bit_program_is_refused_with_a_note", [] {
    fs::ScopedTempDir dir("cb_pe64_writer_32");
    const image_writer::Written r = reconstruct(testing::data_path("formats/testdata/lz32.dll"), dir.path());
    CB_CHECK(!r.linked && !r.same);
    CB_CHECK(r.source.empty());
    CB_CHECK(!r.notes.empty());
});

}  // namespace cb::source::pe64_writer
