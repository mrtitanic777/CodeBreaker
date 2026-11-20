// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/vxd/boot with no machine: what a screen shows, where each file goes on the installed disk, the boot options, what the boot log says of
// a driver, the bisection, and the plan of a run over a FAT image (no QEMU is started here; `vxd boot` itself needs an installed Windows 95 disk).
#include "source/vxd/boot.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/detect.hpp"
#include "formats/le.hpp"

namespace vx = cb::source::vxd;
namespace bt = cb::source::vxd::boot;
using cb::verify::gate16::Picture;

namespace {

// A picture of one colour, with rectangles of others.
struct BootRect {
    std::uint32_t x, y, w, h;
    std::uint8_t r, g, b;
};
Picture boot_picture(std::uint32_t w, std::uint32_t h, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::initializer_list<BootRect> rects = {}) {
    cb::Bytes rgb(static_cast<std::size_t>(w) * h * 3);
    for (std::size_t i = 0; i < static_cast<std::size_t>(w) * h; ++i) rgb[i * 3] = r, rgb[i * 3 + 1] = g, rgb[i * 3 + 2] = b;
    for (const BootRect& q : rects)
        for (std::uint32_t y = q.y; y < q.y + q.h; ++y)
            for (std::uint32_t x = q.x; x < q.x + q.w; ++x) {
                const std::size_t at = (static_cast<std::size_t>(y) * w + x) * 3;
                rgb[at] = q.r, rgb[at + 1] = q.g, rgb[at + 2] = q.b;
            }
    return Picture(w, h, std::move(rgb));
}

// A corpus folder with splitter.vxd where Windows keeps its drivers.
struct BootWorld {
    cb::fs::ScopedTempDir dir{"boottest"};
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    cb::fs::ScopedEnv root{"CB_CORPUS_ROOT", cb::fs::to_text(dir.path() / "corpus")};
    cb::fs::ScopedEnv no_data{"WT_DATA_ROOT", std::nullopt};
    std::unique_ptr<cb::base::Corpus> corpus;
    vx::DdkNames names;
    std::unique_ptr<bt::Environment> env;
    BootWorld() {
        const std::filesystem::path folder = dir.path() / "corpus";
        cb::fs::make_dirs(folder / "WINDOWS" / "SYSTEM" / "IOSUBSYS");
        std::filesystem::copy_file(cb::testing::data_path("formats/testdata/splitter.vxd"), folder / "WINDOWS" / "SYSTEM" / "splitter.vxd");
        std::filesystem::copy_file(cb::testing::data_path("formats/testdata/srammtd.vxd"), folder / "WINDOWS" / "SYSTEM" / "IOSUBSYS" / "srammtd.vxd");
        std::filesystem::copy_file(cb::testing::data_path("formats/testdata/bios.vxd"), folder / "WINDOWS" / "SYSTEM" / "bios.386");
        cb::base::Products products;
        products.target = "win95";
        products.target_folder = folder;
        products.checkout_root = dir.path() / "CodeBreaker";
        products.state = dir.path() / "state";
        cb::base::FormatProbes probes;
        probes.lane = [](const std::filesystem::path& p) -> std::optional<cb::base::Lane> {
            const auto lane = cb::formats::detect::lane(p);
            if (!lane) return std::nullopt;
            switch (*lane) {
            case cb::formats::detect::Lane::Pe: return cb::base::Lane::Pe;
            case cb::formats::detect::Lane::Ne: return cb::base::Lane::Ne;
            case cb::formats::detect::Lane::Le: return cb::base::Lane::Le;
            case cb::formats::detect::Lane::Dos: return cb::base::Lane::Dos;
            }
            return std::nullopt;
        };
        corpus = std::make_unique<cb::base::Corpus>(products, probes);
        env = std::make_unique<bt::Environment>(bt::Environment{names, *corpus, dir.path() / "state", "qemu-not-here", 6, dir.path() / "no-disk.img"});
    }
};
}  // namespace

CB_TEST("source.vxd.boot.what_a_screen_shows", [] {
    CB_CHECK(bt::classify(boot_picture(640, 480, 0, 128, 128)) == bt::Screen::Desktop);                  // the desktop's teal, in the palette
    CB_CHECK(bt::classify(boot_picture(1024, 768, 87, 168, 168)) == bt::Screen::Desktop);                // ... and on QEMU's VGA
    CB_CHECK(bt::classify(boot_picture(640, 480, 0, 0, 170)) == bt::Screen::Blue);                       // a fatal exception
    CB_CHECK(bt::classify(boot_picture(720, 400, 0, 0, 0)) == bt::Screen::Text);                         // the text mode
    CB_CHECK(bt::classify(boot_picture(720, 400, 170, 170, 170)) == bt::Screen::Text);
    CB_CHECK(bt::classify(boot_picture(640, 480, 0, 0, 0)) == bt::Screen::Black);                        // a graphics mode with nothing on it
    CB_CHECK(bt::classify(boot_picture(640, 480, 255, 255, 255)) == bt::Screen::Other);                  // Windows' own white screens
    CB_CHECK(bt::classify(boot_picture(320, 200, 0, 128, 128)) == bt::Screen::Other);                    // teal, but a boot-time mode
    // a wallpaper: not teal, with the taskbar's grey across the bottom and its grey at the left edge
    CB_CHECK(bt::classify(boot_picture(640, 480, 200, 50, 60, {{0, 456, 640, 24, 192, 192, 192}})) == bt::Screen::Desktop);
    CB_CHECK(bt::classify(boot_picture(640, 480, 200, 50, 60, {{0, 456, 640, 24, 192, 199, 200}})) == bt::Screen::Desktop);   // (the VGA's grey)
    CB_CHECK(bt::classify(boot_picture(640, 480, 200, 50, 60)) == bt::Screen::Other);                    // no taskbar
    CB_CHECK(bt::classify(Picture()) == bt::Screen::Other);
    // a mostly blue screen with a window on it is a blue one still
    CB_CHECK(bt::classify(boot_picture(640, 480, 0, 0, 170, {{100, 100, 100, 50, 255, 255, 255}})) == bt::Screen::Blue);
});

CB_TEST("source.vxd.boot.the_boot_options_are_set_in_msdos_sys", [] {
    // present: replaced where they are named, and (re)written first under [Options]
    const std::string original = "[Paths]\r\nWinDir=C:\\WINDOWS\r\n[Options]\r\nBootMulti=1\r\nbootlog=0\r\nAutoScan = 2\r\nLogo=0\r\n";
    CB_CHECK_EQ(bt::boot_options(original), "[Paths]\r\nWinDir=C:\\WINDOWS\r\n[Options]\r\nBootLog=1\r\nAutoScan=0\r\nBootMulti=1\r\nLogo=0\r\n");
    // no [Options]: the section is made at the top
    CB_CHECK_EQ(bt::boot_options("[Paths]\r\nHostWinBootDrv=C\r\n"), "[Options]\r\nBootLog=1\r\nAutoScan=0\r\n[Paths]\r\nHostWinBootDrv=C\r\n");
    // (the comment filler line a real MSDOS.SYS has after the options is kept)
    CB_CHECK(bt::boot_options("[Options]\r\n;xxxxxxxx\r\n").find(";xxxxxxxx") != std::string::npos);
    CB_CHECK_EQ(bt::boot_options(""), "[Options]\r\nBootLog=1\r\nAutoScan=0\r\n");
});

CB_TEST("source.vxd.boot.what_the_boot_log_says_of_a_driver", [] {
    const std::string log =
        "[0010F1A2] Loading Vxd = vmm\r\n"
        "[0010F1A3] LoadStart = vtd\r\n"
        "[0010F1A4] LoadSuccess = vtd\r\n"
        "[0010F2A4] Loading Vxd = C:\\WINDOWS\\SYSTEM\\VMM32\\VTD.VXD\r\n"
        "[0010F3A5] SYSCRITINIT = vtd\r\n"
        "[0010F3A6] SYSCRITINITSUCCESS = vtd\r\n"
        "[0010F4B0] DEVICEINIT = vtd\r\n"
        "[0010F4B1] LoadStart = C:\\WINDOWS\\SYSTEM\\VMM32\\VPICD.VXD\r\n"
        "[0010F4B2] LoadFailed = C:\\WINDOWS\\SYSTEM\\VMM32\\VPICD.VXD\r\n";
    // a phase that ended well is named once ("<phase>SUCCESS" follows it): the phases that did not are what is said; "Loading Vxd" is left out
    CB_CHECK_EQ(bt::loaded(log, "WINDOWS/SYSTEM/VMM32/VTD.VXD"), "LoadStart LoadSuccess SYSCRITINITSUCCESS DEVICEINIT");
    CB_CHECK_EQ(bt::loaded(log, "WINDOWS/SYSTEM/VMM32/VPICD.VXD"), "LoadStart LoadFailed");
    CB_CHECK_EQ(bt::loaded(log, "WINDOWS/SYSTEM/VMM32/VKD.VXD"), "");   // the log never names it
    CB_CHECK_EQ(bt::loaded("", "VTD.VXD"), "");
});

CB_TEST("source.vxd.boot.bisection_finds_the_units_that_fail_alone", [] {
    const std::vector<std::string> units = {"a", "b", "c", "d", "e"};
    std::vector<std::string> tried;
    std::vector<std::string> said;
    const auto trial_failing = [&](const std::set<std::string>& bad) {
        return [&, bad](std::span<const std::string> part, const std::string& tag) -> std::string {
            tried.push_back(tag);
            for (const std::string& u : part)
                if (bad.contains(u)) return "FAIL(blue screen)";
            return "PASS";
        };
    };
    const auto log = [&](std::string_view line) { said.emplace_back(line); };
    CB_CHECK(bt::bisect(units, trial_failing({"d"}), "t", log) == (std::vector<std::string>{"d"}));
    CB_CHECK(bt::bisect(units, trial_failing({"a", "e"}), "t", log) == (std::vector<std::string>{"a", "e"}));
    CB_CHECK(!tried.empty() && tried.front() == "t_b0");   // the first half of the set is tag_b0, the second tag_b1
    // two that fail only together: no half fails alone, and the group is what is reported
    const auto together = [&](std::span<const std::string> part, const std::string&) -> std::string {
        return std::find(part.begin(), part.end(), "b") != part.end() && std::find(part.begin(), part.end(), "c") != part.end() ? "FAIL(x)" : "PASS";
    };
    CB_CHECK(bt::bisect(std::vector<std::string>{"a", "b", "c", "d"}, together, "t", log) == (std::vector<std::string>{"a", "b", "c", "d"}));
    CB_CHECK(!said.empty() && said.back().find("fail together") != std::string::npos);
    CB_CHECK(bt::bisect(std::vector<std::string>{"only"}, trial_failing({"only"}), "t", log) == (std::vector<std::string>{"only"}));
});

CB_TEST("source.vxd.boot.where_each_driver_goes_on_the_disk", [] {
    const BootWorld world;
    // a driver of WINDOWS\SYSTEM goes into its VMM32 folder (a driver found there is loaded in place of the copy in VMM32.VXD)
    CB_CHECK_EQ(bt::guest_path(*world.env, world.corpus->root() / "WINDOWS" / "SYSTEM" / "splitter.vxd"), "WINDOWS/SYSTEM/VMM32/SPLITTER.VXD");
    // one below it, or with another extension, where it is
    CB_CHECK_EQ(bt::guest_path(*world.env, world.corpus->root() / "WINDOWS" / "SYSTEM" / "IOSUBSYS" / "srammtd.vxd"), "WINDOWS/SYSTEM/IOSUBSYS/SRAMMTD.VXD");
    CB_CHECK_EQ(bt::guest_path(*world.env, world.corpus->root() / "WINDOWS" / "SYSTEM" / "bios.386"), "WINDOWS/SYSTEM/BIOS.386");
    // VMM32.VXD itself goes into WINDOWS\SYSTEM
    CB_CHECK_EQ(bt::guest_path(*world.env, world.corpus->root() / "WINDOWS" / "SYSTEM" / "vmm32.vxd"), "WINDOWS/SYSTEM/VMM32.VXD");
    // a name without an extension: .vxd, .386, .pdr in turn
    CB_CHECK(bt::corpus_file(*world.env, "splitter") == *world.corpus->retail("splitter.vxd"));
    CB_CHECK(bt::corpus_file(*world.env, "bios") == *world.corpus->retail("bios.386"));
    CB_CHECK(bt::corpus_file(*world.env, "splitter.vxd") == *world.corpus->retail("splitter.vxd"));
    CB_CHECK_THROWS_AS(cb::UsageError, bt::corpus_file(*world.env, "nonesuch"));
});

CB_TEST("source.vxd.boot.a_driver_is_rebuilt_from_its_disassembly", [] {
    const BootWorld world;
    // the LE writer's file of splitter is the original (byte for byte with the DDK's names, which this test does not have: what is compared is the objects)
    const cb::Bytes rebuilt = bt::build(*world.env, *world.corpus->retail("splitter.vxd"));
    const cb::formats::le::Module original = cb::formats::le::Module::open(cb::testing::data_path("formats/testdata/splitter.vxd"));
    const cb::formats::le::Module made = cb::formats::le::Module::from_buffer(cb::Bytes(rebuilt));
    CB_CHECK_EQ(rebuilt.size(), cb::fs::file_size(cb::testing::data_path("formats/testdata/splitter.vxd")));
    CB_CHECK(made.object_data(made.objects().front()) == original.object_data(original.objects().front()));
    // a file that holds no driver
    cb::fs::write_text(world.dir.path() / "not-a-driver.vxd", "MZ nothing");
    CB_CHECK_THROWS_AS(cb::FormatError, bt::build(*world.env, world.dir.path() / "not-a-driver.vxd"));
});

CB_TEST("source.vxd.boot.a_bundle_names_its_members", [] {
    CB_CHECK(bt::bundle_members({}).empty());
    const cb::Bytes nothing(64, 0);
    CB_CHECK(bt::bundle_members(nothing).empty());
});

CB_TEST("source.vxd.boot.the_command_needs_a_disk", [] {
    const BootWorld world;
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(bt::main(*world.env, std::vector<std::string>{}), cb::kExitUsage);   // no arguments: the help, and a usage exit
        CB_CHECK(cap.out().starts_with("boot -- does Windows 95 start with these drivers?"));
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(bt::main(*world.env, std::vector<std::string>{"--help"}), cb::kExitOk);
        CB_CHECK(cap.out().find("--bisect") != std::string::npos);
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(bt::main(*world.env, std::vector<std::string>{"--drivers", "splitter"}), cb::kExitFailed);
        CB_CHECK(cap.out().find("no installed Windows 95 disk at ") != std::string::npos);
    }
});
