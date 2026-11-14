// Tests of the `xbox-disc` command (cb/formats/xbox/disc_command.hpp) on a disc image and on a Games on Demand package of the same volume, against what
// `codebreaker.exe xbox-disc ...` printed and wrote for them: testdata/disc_goldens.txt, made by
// testdata/make_disc_goldens.py from the files this test writes when CB_DISC_SAMPLE_OUT names a folder (the volume built by gdf::build from four files,
// as gdf_test.cpp builds it, and its package from god::write_god).
//
// The golden holds each command's stdout, exit code and the files it wrote (size and SHA-1). Every case below is run on the native command and compared.
//
// Behaviour pinned here: nothing the command prints or writes; `files` lists in gdf.hpp's order (lower-cased path), which is the
// same order as walking the directory tree for a flat root named like these four files (the golden says so).
#include "cb/formats/xbox/disc_command.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"
#include "cb/formats/xbox/god.hpp"

namespace disc_command_test {

using namespace cb;
using namespace cb::formats::xbox;
namespace stdfs = std::filesystem;

Bytes pattern(std::size_t n, std::uint8_t step = 1) {
    Bytes b(n);
    for (std::size_t i = 0; i < n; ++i) b[i] = static_cast<std::uint8_t>(i * step);
    return b;
}

// the four files of gdf_test.cpp
std::vector<gdf::SourceFile> four_files() {
    Bytes xex = {'X', 'E', 'X', '2'};
    xex.resize(5004, 0);
    Bytes text;
    const std::string_view sentence = "The quick brown fox jumps over the lazy dog. ";
    for (int i = 0; i < 90; ++i) append(text, as_bytes(sentence));
    return {{"default.xex", xex}, {"B.TXT", to_bytes("hello")}, {"a.bin", pattern(2560)}, {"Zed.txt", text}};
}

Bytes sample_volume() { return gdf::build(four_files()); }

// A GoD package of the volume: the `.data` folder under `out`.
stdfs::path write_package(const Bytes& volume, const stdfs::path& out) {
    const Bytes tpl = fs::read_file(testing::data_path("formats/testdata/god/empty_live.bin"));
    god::Title title;
    title.title_id = 0x12345678;
    title.media_id = 0x0000ABCD;
    title.title = "Sample";
    const god::WriteResult made = god::write_god(
        [&volume](std::uint64_t offset, std::size_t length) {
            if (offset >= volume.size()) return Bytes{};
            const std::size_t n = std::min<std::size_t>(length, volume.size() - static_cast<std::size_t>(offset));
            return Bytes(volume.begin() + static_cast<std::ptrdiff_t>(offset), volume.begin() + static_cast<std::ptrdiff_t>(offset + n));
        },
        volume.size(), out, tpl, title);
    return made.data_dir;
}

std::string forward(std::string s) { return text::replace_all(s, "\\", "/"); }

// The golden of a case: its stdout, `exit=N`, and "file <path> <size> <sha1>" for each file it wrote.
std::string golden(const std::string& name) {
    const std::string log = fs::read_text(testing::data_path("formats/xbox/testdata/disc_goldens.txt"));
    std::string out;
    bool inside = false;
    for (const std::string_view line : text::lines(log)) {
        if (line.starts_with("== ")) inside = line.substr(3) == name;
        else if (inside) out += std::string(line) + "\n";
    }
    CB_CHECK(!out.empty());
    return out;
}

std::string manifest(const stdfs::path& folder) {
    std::string out = testing::tree_manifest(folder);
    return text::replace_all(out, ".part ", ".part ");
}

// Runs `xbox-disc args...`; stdout, exit code, and the manifest of `written` (a folder or a file) when it exists.
std::string run(const std::vector<std::string>& args, const stdfs::path& written = {}) {
    std::string out;
    try {
        out = testing::run_command(disc_command::command(), args);
    } catch (const Error& e) {
        out = std::string("error ") + e.what() + "\n";
    }
    if (!written.empty()) {
        std::error_code ec;
        if (stdfs::is_directory(written, ec)) out += testing::tree_manifest(written);
        else if (stdfs::is_regular_file(written, ec)) {
            const Bytes data = fs::read_file(written);
            out += std::format("file {} {} {}\n", fs::to_text(written.filename()), data.size(), crypto::to_hex(crypto::sha1(data)));
        }
    }
    return out;
}

// Used once, by hand: writes the volume and its package where make_disc_goldens.py reads them.
CB_TEST("formats.xbox.disc_command.write_the_samples_for_the_goldens", [] {
    const auto out = fs::get_env("CB_DISC_SAMPLE_OUT");
    if (!out) return;
    const Bytes volume = sample_volume();
    fs::make_dirs(fs::from_text(*out));
    fs::write_file(fs::from_text(*out) / "four.iso", volume);
    write_package(volume, fs::from_text(*out) / "pkg");
});

CB_TEST("formats.xbox.disc_command.files_lists_every_file_with_its_size", [] {
    const fs::ScopedTempDir dir{"cb-disc"};
    fs::write_file(dir.path() / "four.iso", sample_volume());
    const std::string w = forward(fs::to_text(dir.path()));
    const std::string got = run({"files", w + "/four.iso"});
    CB_CHECK_EQ(got, "2560\ta.bin\n5\tB.TXT\n5004\tdefault.xex\n4050\tZed.txt\nexit=0\n");
    // the same volume as a package: its data folder is the image
    const stdfs::path data = write_package(sample_volume(), dir.path() / "pkg");
    CB_CHECK_EQ(run({"files", forward(fs::to_text(data))}), got);
});

CB_TEST("formats.xbox.disc_command.files_match_the_recorded_output", [] {
    const fs::ScopedTempDir dir{"cb-disc"};
    fs::write_file(dir.path() / "four.iso", sample_volume());
    const std::string w = forward(fs::to_text(dir.path()));
    CB_CHECK_EQ(text::replace_all(run({"files", w + "/four.iso"}), w, "<w>"), golden("files_iso"));
    const stdfs::path data = write_package(sample_volume(), dir.path() / "pkg");
    CB_CHECK_EQ(text::replace_all(run({"files", forward(fs::to_text(data))}), w, "<w>"), golden("files_package"));
});

CB_TEST("formats.xbox.disc_command.read_writes_one_file_and_a_missing_file_is_exit_3", [] {
    const fs::ScopedTempDir dir{"cb-disc"};
    fs::write_file(dir.path() / "four.iso", sample_volume());
    const std::string w = forward(fs::to_text(dir.path()));
    const std::string got = run({"read", w + "/four.iso", "default.xex", w + "/out/x.xex"}, dir.path() / "out" / "x.xex");
    CB_CHECK_EQ(text::replace_all(got, w, "<w>"), golden("read_iso"));
    CB_CHECK(!stdfs::exists(dir.path() / "out" / "x.xex.part"));
    // the path is matched without regard to case
    CB_CHECK_EQ(run({"read", w + "/four.iso", "DEFAULT.XEX", w + "/out/y.xex"}), "exit=0\n");
    CB_CHECK(equal_bytes(fs::read_file(dir.path() / "out" / "y.xex"), fs::read_file(dir.path() / "out" / "x.xex")));
    // the disc has no such file: nothing written, exit 3 (what the golden says)
    const std::string missing = run({"read", w + "/four.iso", "nothing.xex", w + "/out/z.xex"}, dir.path() / "out" / "z.xex");
    CB_CHECK_EQ(text::replace_all(missing, w, "<w>"), golden("read_missing"));
    CB_CHECK(!stdfs::exists(dir.path() / "out" / "z.xex"));
});

CB_TEST("formats.xbox.disc_command.unpack_writes_every_file_and_reports_progress", [] {
    const fs::ScopedTempDir dir{"cb-disc"};
    fs::write_file(dir.path() / "four.iso", sample_volume());
    const std::string w = forward(fs::to_text(dir.path()));
    const std::string got = run({"unpack", w + "/four.iso", w + "/out"}, dir.path() / "out");
    CB_CHECK_EQ(text::replace_all(got, w, "<w>"), golden("unpack_iso"));
    // run again: files at their full size are kept (nothing is read: the one progress line is the last), the files are the same
    const std::string again = text::replace_all(run({"unpack", w + "/four.iso", w + "/out"}, dir.path() / "out"), w, "<w>");
    CB_CHECK_EQ(again, "PROGRESS 11619 11619\nexit=0\n" + golden("unpack_iso").substr(golden("unpack_iso").find("B.TXT")));
    // from the package
    const stdfs::path data = write_package(sample_volume(), dir.path() / "pkg");
    CB_CHECK_EQ(text::replace_all(run({"unpack", forward(fs::to_text(data)), w + "/out2"}, dir.path() / "out2"), w, "<w>"), golden("unpack_package"));
});

CB_TEST("formats.xbox.disc_command.unpack_goes_on_where_an_interrupted_one_stopped", [] {
    const fs::ScopedTempDir dir{"cb-disc"};
    fs::write_file(dir.path() / "four.iso", sample_volume());
    const std::string w = forward(fs::to_text(dir.path()));
    fs::make_dirs(dir.path() / "out");
    fs::write_file(dir.path() / "out" / "a.bin", pattern(2560));                  // already whole: kept
    fs::write_file(dir.path() / "out" / "Zed.txt.part", Bytes{1, 2, 3});         // a piece of a file: made again
    fs::write_file(dir.path() / "out" / "B.TXT", Bytes{9, 9});                    // the wrong size: made again
    const std::string got = run({"unpack", w + "/four.iso", w + "/out"});
    CB_CHECK(got.ends_with("PROGRESS 11619 11619\nexit=0\n"));
    CB_CHECK(fs::read_file(dir.path() / "out" / "B.TXT") == to_bytes("hello"));
    CB_CHECK_EQ(fs::file_size(dir.path() / "out" / "Zed.txt"), std::uint64_t{4050});
    CB_CHECK(!stdfs::exists(dir.path() / "out" / "Zed.txt.part"));
});

CB_TEST("formats.xbox.disc_command.an_image_that_is_not_a_disc_is_an_error", [] {
    const fs::ScopedTempDir dir{"cb-disc"};
    const std::string w = forward(fs::to_text(dir.path()));
    fs::write_file(dir.path() / "junk.iso", pattern(0x20000));
    CB_CHECK_THROWS_AS(FormatError, (void)testing::run_command(disc_command::command(), {"files", w + "/junk.iso"}));
    CB_CHECK_THROWS_AS(NotFoundError, (void)testing::run_command(disc_command::command(), {"files", w + "/nothing.iso"}));
});

}  // namespace disc_command_test
