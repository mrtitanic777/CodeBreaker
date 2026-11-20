// Tests of cb/build/disc_image.hpp: the images the writer makes from the test trees (testdata/disc: real DOS and Windows files and some
// awkward names) are the ones `codebreaker disc` made, byte for byte but for the date the volume was written (testdata/disc_golden:
// the command's output, and the size and SHA-1 of each image with those two dates zeroed). The trees get fixed modification times
// first.
//
// A character that is not ASCII becomes one '_' however many characters its upper-case form has; the
// usage lines are ours.
#include "build/disc_image.hpp"

#include <chrono>
#include <format>
#include <string>
#include <vector>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/command_test_kit.hpp"
#include "formats/media.hpp"

using namespace cb;
namespace disc_image = cb::build::disc_image;

namespace {

std::string disc_golden(const char* name) { return fs::read_text(testing::data_path(std::string("build/testdata/disc_golden/") + name)); }

// 2026-01-02 03:04:05 UTC on every file and folder of the tree.
void disc_fix_times(const std::filesystem::path& root) {
    using namespace std::chrono;
    const auto when = clock_cast<std::filesystem::file_time_type::clock>(sys_days{year{2026} / 1 / 2} + hours{3} + minutes{4} + seconds{5});
    for (const auto& entry : std::filesystem::recursive_directory_iterator(root)) cb::fs::set_last_write_time(entry.path(), when);
    cb::fs::set_last_write_time(root, when);
}

struct DiscWorld {
    fs::ScopedTempDir dir{"cb-disc"};
    std::string w = fs::to_text(dir.path());

    DiscWorld() {
        for (const char* name : {"tree", "clash", "deep"}) {
            fs::copy_tree(testing::data_path(std::string("build/testdata/disc/") + name), dir.path() / name);
        }
        fs::make_dirs(dir.path() / "tree" / "empty");   // (an empty folder is not in the repository)
        for (const char* name : {"tree", "clash", "deep"}) disc_fix_times(dir.path() / name);
        fs::write_file(dir.path() / "boot_floppy.img", Bytes(1474560, 0));
        std::filesystem::copy_file(testing::data_path("build/testdata/disc/boot_small.img"), dir.path() / "boot_small.img");
    }

    // The command's output and `exit=N`, with the folder as <w> and '/' for every separator.
    std::string run(std::vector<std::string> args) const {
        cli::Cli cli("codebreaker", {disc_image::command()});
        testing::CapturedStd cap;
        args.insert(args.begin(), "disc");
        const int code = cli.run(args);
        const std::string out = cap.out() + cap.err() + "exit=" + std::to_string(code) + "\n";   // (the refusals go to stderr; the goldens have both)
        return text::replace_all(text::replace_all(out, "\\", "/"), w + "/", "<w>/");
    }

    // "name size sha1" of an image with the two volume dates (when it was written) zeroed.
    std::string line(const std::string& name) const {
        Bytes image = fs::read_file(dir.path() / name);
        const std::size_t at = 16 * disc_image::kSectorSize + 7 + 806;
        std::fill(image.begin() + static_cast<std::ptrdiff_t>(at), image.begin() + static_cast<std::ptrdiff_t>(at + 34), std::uint8_t{0});
        return std::format("{} {} {}\n", name, image.size(), crypto::to_hex(crypto::sha1(image)));
    }
};

// The golden's section `name` ("== name\n<stdout>exit=N\n"), and a listing with its creation date hidden.
std::string disc_case(const std::string& name) {
    const std::string log = disc_golden("disc.txt");
    std::string out;
    bool inside = false;
    for (const std::string_view line : text::lines(log)) {
        if (line.starts_with("== ")) inside = line.substr(3) == name;
        else if (inside) out += std::string(line) + "\n";
    }
    return text::replace_all(out, "\\", "/");
}
std::string disc_hide_created(const std::string& listing) {
    std::string out;
    for (const std::string_view line : text::lines(listing)) out += (line.starts_with("    created ") ? "    created -" : std::string(line)) + "\n";
    return out;
}

}  // namespace

CB_TEST("build.disc_image.names_are_cut_to_level_one", [] {
    using Name = std::pair<std::string, std::string>;
    CB_CHECK((disc_image::level1_name("Hello World.txt", false) == Name{"HELLO_WO", "TXT"}));
    CB_CHECK((disc_image::level1_name("LongFileName.Document.exe", false) == Name{"LONGFILE", "EXE"}));   // the extension is after the last dot
    CB_CHECK((disc_image::level1_name(".hidden", false) == Name{"_", "HID"}));
    CB_CHECK((disc_image::level1_name("noext", false) == Name{"NOEXT", ""}));
    CB_CHECK((disc_image::level1_name("trailing.", false) == Name{"TRAILING", ""}));
    CB_CHECK((disc_image::level1_name("weird name!.c", false) == Name{"WEIRD_NA", "C"}));
    CB_CHECK((disc_image::level1_name("Setup", true) == Name{"SETUP", ""}));
    CB_CHECK((disc_image::level1_name("my.folder", true) == Name{"MY_FOLDE", ""}));   // a folder keeps its dot as '_'
    CB_CHECK((disc_image::level1_name("\xC3\xA9t\xC3\xA9", false) == Name{"_T_", ""}));
    CB_CHECK((disc_image::level1_name("", true) == Name{"_", ""}));
});

CB_TEST("build.disc_image.the_images_are_the_recorded_ones", [] {
    DiscWorld w;
    CB_CHECK_EQ(w.run({w.w + "/tree", w.w + "/plain.iso"}), disc_case("plain"));
    CB_CHECK_EQ(w.run({w.w + "/tree", w.w + "/vol.iso", "--volume", "My Disc! v1.0"}), disc_case("volume"));
    CB_CHECK_EQ(w.run({w.w + "/tree", w.w + "/boot144.iso", "--boot", w.w + "/boot_floppy.img", "--volume", "boot"}), disc_case("boot_floppy"));
    CB_CHECK_EQ(w.run({w.w + "/tree", w.w + "/bootsm.iso", "--boot", w.w + "/boot_small.img"}), disc_case("boot_small"));
    const std::string images = disc_golden("images.txt");
    std::string expected;
    for (const std::string_view line : text::lines(images)) expected += std::string(line) + "\n";
    const std::string ours = w.line("plain.iso") + w.line("vol.iso") + w.line("boot144.iso") + w.line("bootsm.iso");
    CB_CHECK_EQ(ours, expected);
});

CB_TEST("build.disc_image.refusals_say_what_is_wrong_and_exit_one", [] {
    DiscWorld w;
    CB_CHECK_EQ(w.run({w.w + "/clash", w.w + "/clash.iso"}), disc_case("clash"));
    CB_CHECK_EQ(w.run({w.w + "/deep", w.w + "/deep.iso"}), disc_case("deep"));
    CB_CHECK_EQ(w.run({w.w + "/nothing", w.w + "/x.iso"}), disc_case("not_a_folder"));
    CB_CHECK(!std::filesystem::exists(w.dir.path() / "x.iso"));
    // names that clash: a file and a folder of one name do not (they are told apart)
    fs::make_dirs(w.dir.path() / "mixed" / "abcdefghi");
    fs::write_file(w.dir.path() / "mixed" / "abcdefgh", Bytes{1});
    CB_CHECK_NOTHROW((void)disc_image::write(w.dir.path() / "mixed", w.dir.path() / "mixed.iso"));
    CB_CHECK_THROWS_AS(disc_image::DiscError, disc_image::write(w.dir.path() / "clash", w.dir.path() / "clash.iso"));
});

CB_TEST("build.disc_image.an_image_is_listed_and_extracted_through_the_media_readers", [] {
    DiscWorld w;
    (void)w.run({w.w + "/tree", w.w + "/plain.iso"});
    (void)w.run({w.w + "/tree", w.w + "/bootsm.iso", "--boot", w.w + "/boot_small.img"});
    CB_CHECK_EQ(disc_hide_created(w.run({"--list", w.w + "/plain.iso"})), disc_hide_created(disc_case("list_plain")));
    CB_CHECK_EQ(disc_hide_created(w.run({"--list", w.w + "/bootsm.iso"})), disc_hide_created(disc_case("list_boot")));
    CB_CHECK_EQ(w.run({"--extract", w.w + "/plain.iso", w.w + "/out"}), disc_case("extract"));
    CB_CHECK_EQ(testing::tree_manifest(w.dir.path() / "out"), disc_golden("extract_manifest.txt"));
    // the boot image is a file of the disc: El Torito finds it
    auto volumes = formats::media::volumes(w.dir.path() / "bootsm.iso");
    CB_CHECK(!volumes.empty());
});

CB_TEST("build.disc_image.one_input_writes_a_folder_beside_itself_or_takes_an_image_apart_beside_itself", [] {
    DiscWorld w;
    const std::string wrote = w.run({w.w + "/tree"});   // a folder: <folder>.iso beside it, named after it
    CB_CHECK(wrote.find("wrote <w>/tree.iso") != std::string::npos && wrote.find("Output: ") != std::string::npos && wrote.ends_with("exit=0\n"));
    CB_CHECK(std::filesystem::is_regular_file(w.dir.path() / "tree.iso"));
    const std::string taken = w.run({w.w + "/tree.iso"});   // an image: its files in a folder beside it
    CB_CHECK(taken.find("to <w>/tree") != std::string::npos && taken.ends_with("exit=0\n"));
    CB_CHECK(std::filesystem::is_directory(w.dir.path() / "tree"));
    CB_CHECK_EQ(disc_image::volume_name("My Disc! v1.0"), "MY_DISC__V1_0");
    CB_CHECK_EQ(disc_image::volume_name(""), "CODEBREAKER");
    CB_CHECK_EQ(disc_image::volume_name(std::string(40, 'a')).size(), 32u);
});

CB_TEST("build.disc_image.misuse_is_a_usage_error", [] {
    DiscWorld w;
    // (a UsageError from the handler is mapped to exit 2 by the application)
    CB_CHECK_THROWS_AS(UsageError, w.run({w.w + "/tree", w.w + "/a", "--list"}));
    CB_CHECK_THROWS_AS(UsageError, w.run({"--list", w.w + "/tree", w.w + "/b"}));
    CB_CHECK_THROWS_AS(UsageError, w.run({"--extract", w.w + "/plain.iso"}));
    CB_CHECK(w.run({}).ends_with("exit=2\n"));   // (no folder at all is a parse error)
});
