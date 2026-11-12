#include "cb/build/dos_tools.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::build;

DosRunSpec sample_spec(const std::filesystem::path& tools) {
    DosRunSpec spec;
    spec.mounts = {{"c", tools}};
    spec.commands = {"c:\\ml.exe /c t.asm", "dir"};
    spec.artifacts = {"T.OBJ"};
    return spec;
}

}  // namespace

CB_TEST("build.dos_tools.batch_text_ends_lines_the_dos_way", [] {
    const DosRunSpec spec = sample_spec("C:/tools");
    CB_CHECK_EQ(dos_tools::batch_text(spec), "@echo off\r\nc:\\ml.exe /c t.asm\r\ndir\r\necho _DONE_ > done.txt\r\n");
    DosRunSpec none;
    CB_CHECK_EQ(dos_tools::batch_text(none), "@echo off\r\necho _DONE_ > done.txt\r\n");
});

CB_TEST("build.dos_tools.conf_mounts_work_as_d_and_runs_in_the_last_drive", [] {
    DosRunSpec spec = sample_spec("C:/tools/ms");
    const std::string conf = dos_tools::dosbox_conf(spec, "C:/work/one");
    CB_CHECK_EQ(conf,
                "[cpu]\r\ncycles=20000\r\n[autoexec]\r\nmount c \"C:\\tools\\ms\"\r\nmount d \"C:\\work\\one\"\r\n"
                "d:\r\ncall BLD.BAT\r\nexit\r\n");
});

CB_TEST("build.dos_tools.conf_keeps_the_order_of_settings_and_a_named_d", [] {
    DosRunSpec spec;
    spec.mounts = {{"d", "C:/mine"}, {"c", "C:/tc"}};
    spec.cpu.insert_or_assign("cputype", "386");
    spec.cpu.insert_or_assign("cycles", "max");
    spec.dos.insert_or_assign("ver", "6.22");
    spec.variables.insert_or_assign("PATH", "C:\\BIN");
    spec.variables.insert_or_assign("INCLUDE", "C:\\INCLUDE");
    const std::string conf = dos_tools::dosbox_conf(spec, "C:/unused");
    CB_CHECK_EQ(conf,
                "[cpu]\r\ncputype=386\r\ncycles=max\r\n[dos]\r\nver=6.22\r\n[autoexec]\r\n"
                "mount d \"C:\\mine\"\r\nmount c \"C:\\tc\"\r\nset PATH=C:\\BIN\r\nset INCLUDE=C:\\INCLUDE\r\n"
                "c:\r\ncall BLD.BAT\r\nexit\r\n");
});

CB_TEST("build.dos_tools.no_emulator_makes_nothing_and_says_so", [] {
    dos_tools::DosboxRunner runner(dos_tools::Emulator{});
    const DosRunResult r = runner.run(sample_spec("C:/tools"));
    CB_CHECK(!r.ok);
    CB_CHECK(r.made.empty());
    CB_CHECK(!r.work.has_value());
});

CB_TEST("build.dos_tools.a_forbidden_emulator_is_never_started", [] {
    fs::ScopedTempDir dir("dostools");
    fs::write_file(dir.path() / "dosbox-x.exe", Bytes{1});
    dos_tools::Emulator e;
    e.dosbox_x = dir.path() / "dosbox-x.exe";
    CB_CHECK(e.program().has_value());
    e.forbidden = true;
    CB_CHECK(!e.program().has_value());
    CB_CHECK(!dos_tools::DosboxRunner(e).run(sample_spec(dir.path())).work.has_value());
});

CB_TEST("build.dos_tools.an_emulator_that_cannot_start_leaves_the_prepared_folder", [] {
    fs::ScopedTempDir dir("dostools");
    fs::write_file(dir.path() / "dosbox-x.exe", Bytes{1, 2, 3});   // a file, but not a program
    dos_tools::Emulator e;
    e.dosbox_x = dir.path() / "dosbox-x.exe";
    DosRunSpec spec = sample_spec(dir.path());
    spec.work = dir.path() / "work";
    spec.files["T.ASM"] = std::string("mov ax,1\nret\n");
    spec.files["T.BIN"] = Bytes{0x0A, 0x00};
    const DosRunResult r = dos_tools::DosboxRunner(e).run(spec);
    CB_CHECK(!r.ok);
    CB_CHECK(r.made.empty());
    CB_CHECK(r.work.has_value());
    // the work folder holds what the emulator would have read: the files (text with DOS line ends), the batch, the conf
    CB_CHECK_EQ(fs::read_text(*r.work / "T.ASM"), "mov ax,1\r\nret\r\n");
    CB_CHECK(fs::read_file(*r.work / "T.BIN") == (Bytes{0x0A, 0x00}));
    CB_CHECK_EQ(fs::read_text(*r.work / "BLD.BAT"), dos_tools::batch_text(spec));
    CB_CHECK_EQ(fs::read_text(*r.work / "r.conf"), dos_tools::dosbox_conf(spec, *r.work));
});

CB_TEST("build.dos_tools.a_text_file_keeps_a_line_end_that_is_already_dos", [] {
    fs::ScopedTempDir dir("dostools");
    fs::write_file(dir.path() / "dosbox-x.exe", Bytes{1});
    dos_tools::Emulator e;
    e.dosbox_x = dir.path() / "dosbox-x.exe";
    DosRunSpec spec;
    spec.work = dir.path() / "w";
    spec.files["A.TXT"] = std::string("a\r\nb\nc");
    (void)dos_tools::DosboxRunner(e).run(spec);
    CB_CHECK_EQ(fs::read_text(*spec.work / "A.TXT"), "a\r\nb\r\nc");
});
