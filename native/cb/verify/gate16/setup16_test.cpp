#include "cb/verify/gate16/setup16.hpp"

#include <algorithm>
#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/cab.hpp"
#include "cb/formats/iso9660.hpp"
#include "cb/jobs/workers.hpp"

// DIVERGES from verify/gate16/setup16.py:
//   * the CD is patched in place (cb/formats/iso9660.hpp), not written again by pycdlib: every file that was not
//     replaced stays where it was on the image;
//   * the verdicts are an enum (`word()` gives the strings the Python returned, byte for byte);
//   * a screen too short to hold one of the three pixels the License and error-box tests look at is "not that screen"; the
//     Python raised IndexError there, which ended the run;
//   * a picture QEMU wrote only in part, a monitor that does not answer yet, and a PPM that cannot be read are all "no
//     picture this time" and the run looks again, as the Python did for OSError/ValueError; the Enter that cannot be sent
//     is not an error either (the wait that follows ends as FAIL(timeout)); the Python raised;
//   * the CD is Setup's with a file replaced -- the test of continue_test starts cb_fake_qemu.exe (verify/gate16/
//     fake_qemu_main.cpp), which speaks QEMU's monitor, in QEMU's place, and does not boot anything.

using namespace cb;
using namespace cb::verify::gate16;
using namespace cb::verify::gate16::setup16;

namespace {

namespace stdfs = std::filesystem;

Picture solid(std::uint32_t w, std::uint32_t h, Rgb c) {
    Bytes px;
    for (std::size_t i = 0; i < std::size_t{w} * h; ++i) {
        px.push_back(c.r);
        px.push_back(c.g);
        px.push_back(c.b);
    }
    return Picture(w, h, std::move(px));
}

Picture with_pixel(Picture p, std::uint32_t x, std::uint32_t y, Rgb c) {
    Bytes px = p.rgb();
    const std::size_t i = (std::size_t{y} * p.width() + x) * 3;
    px[i] = c.r;
    px[i + 1] = c.g;
    px[i + 2] = c.b;
    return Picture(p.width(), p.height(), std::move(px));
}

bool has(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

constexpr Rgb kWhite{255, 255, 255};
constexpr Rgb kBlack{0, 0, 0};
constexpr Rgb kBlue{0, 0, 170};
constexpr Rgb kGrey{192, 199, 200};

// The License page: blue Windows backdrop, the dialog's grey at the three points.
Picture license_page() {
    Picture p = solid(640, 480, kBlue);
    for (auto [x, y] : {std::pair{60u, 350u}, std::pair{580u, 350u}, std::pair{300u, 395u}})
        p = with_pixel(std::move(p), x, y, kGrey);
    return p;
}

}  // namespace

CB_TEST("verify.gate16.setup16.words", [] {
    CB_CHECK_EQ(word(Verdict::Pass), "PASS");
    CB_CHECK_EQ(word(Verdict::FailDos), "FAIL(dos)");
    CB_CHECK_EQ(word(Verdict::FailHang), "FAIL(hang)");
    CB_CHECK_EQ(word(Verdict::FailErrorBox), "FAIL(error box)");
    CB_CHECK_EQ(word(Verdict::FailTimeout), "FAIL(timeout)");
    CB_CHECK_EQ(word(Verdict::FailErrorBoxBeforeWelcome), "FAIL(error box before Welcome)");
    CB_CHECK_EQ(word(Verdict::Slow), "SLOW");
});

CB_TEST("verify.gate16.setup16.what_the_screen_shows", [] {
    // the License page
    CB_CHECK(is_license(license_page()));
    CB_CHECK(!is_license(solid(640, 480, kBlue)));
    CB_CHECK(!is_license(with_pixel(license_page(), 580, 350, kBlack)));   // one point wrong
    CB_CHECK(!is_license([] { return solid(720, 480, kGrey); }()));        // the DOS text screen is not it
    CB_CHECK(!is_license(solid(640, 300, kGrey)));                         // a short screen has no third point
    // an error box is white at its three points
    CB_CHECK(is_error_box(solid(640, 480, kWhite)));
    CB_CHECK(!is_error_box(solid(640, 480, kBlue)));
    CB_CHECK(!is_error_box(solid(640, 200, kWhite)));                      // short: not it, and no crash
    CB_CHECK(!is_error_box(solid(720, 480, kWhite)));
    CB_CHECK(!is_error_box(with_pixel(solid(640, 480, kWhite), 540, 310, kBlue)));
    // the Welcome dialog: mostly white, 640 wide; the License page is not white enough, a blank white 720 screen is DOS's
    CB_CHECK(is_welcome(solid(640, 480, kWhite)));
    CB_CHECK(!is_welcome(solid(720, 480, kWhite)));
    CB_CHECK(!is_welcome(license_page()));
    CB_CHECK_EQ(white_pixels(solid(640, 480, kWhite)), 160u * 120u);
    CB_CHECK_EQ(white_pixels(solid(640, 480, kBlue)), 0u);
    // a screen with the left half white: the half of the shrunk copy's pixels (the filter blurs only the seam)
    Bytes half(640 * 480 * 3, 0);
    for (std::size_t y = 0; y < 480; ++y)
        std::fill_n(half.begin() + static_cast<std::ptrdiff_t>(y * 640 * 3), 320 * 3, std::uint8_t{255});
    const unsigned whites = white_pixels(Picture(640, 480, std::move(half)));
    CB_CHECK(whites >= 9000 && whites <= 9700);
    CB_CHECK(!is_welcome(Picture(640, 480, Bytes(640 * 480 * 3, 0))));
    // a black screen
    CB_CHECK(is_black(solid(640, 480, kBlack)));
    CB_CHECK(!is_black(solid(640, 480, kBlue)));
    CB_CHECK(!is_black(solid(640, 480, kWhite)));
});

// ---- the monitor and the whole run, with a machine that is not one ----------------------------------------------------

namespace {

stdfs::path testdata(std::string_view name) { return testing::data_path(std::format("verify/gate16/testdata/setup16/{}", name)); }

stdfs::path fake_qemu() { return jobs::WorkerLauncher::for_this_program().exe().parent_path() / "cb_fake_qemu.exe"; }

void write_ppm(const stdfs::path& path, const Picture& p) {
    Bytes out = to_bytes(std::format("P6\n{} {}\n255\n", p.width(), p.height()));
    append(out, p.rgb());
    fs::write_file(path, out);
}

// A fast timing: the run polls every 20 ms.
Timing quick() {
    Timing t;
    t.welcome_polls = 12;
    t.welcome_poll = std::chrono::milliseconds(20);
    t.welcome_settle = std::chrono::milliseconds(20);
    t.continue_poll = std::chrono::milliseconds(20);
    t.wait = std::chrono::seconds(2);
    t.monitor.after_command = std::chrono::milliseconds(5);
    t.monitor.after_screendump = std::chrono::milliseconds(60);
    t.monitor.banner_timeout = std::chrono::milliseconds(500);
    return t;
}

// One run of continue_test against the fake machine, whose screens are `screens` in order (the last repeats).
struct Run {
    fs::ScopedTempDir dir{"cb-setup16"};
    Outcome outcome;
    std::string log;
    stdfs::path state = dir.path() / "state";

    explicit Run(const std::vector<Picture>& screens, bool machine_ends_at_once = false, Timing timing = quick()) {
        std::string script;
        for (std::size_t i = 0; i < screens.size(); ++i) {
            const stdfs::path ppm = dir.path() / std::format("screen{}.ppm", i);
            write_ppm(ppm, screens[i]);
            script += fs::to_text(ppm) + "\n";
        }
        fs::write_text(dir.path() / "script.txt", script);
        const fs::ScopedEnv s("FAKE_QEMU_SCRIPT", fs::to_text(dir.path() / "script.txt"));
        const fs::ScopedEnv l("FAKE_QEMU_LOG", fs::to_text(dir.path() / "log.txt"));
        const fs::ScopedEnv e("FAKE_QEMU_EXIT", machine_ends_at_once ? "1" : "0");

        // the disk Setup prepared (a small file stands in for 650 MB), and a CD the fake makes
        fs::write_text(dir.path() / "mini" / "prepared_hd.img", "disk");
        Config config = Config::in_state(state, fake_qemu(), dir.path() / "none.iso");
        config.mini_dir = dir.path() / "mini";
        Options options;
        options.timing = timing;
        options.make_cd = [](const stdfs::path& out, const Replace& replace) {
            std::string what = "cd:";
            for (const auto& [name, path] : replace)
                what += " " + name;
            fs::write_text(out, what);
        };
        outcome = continue_test(config, {{"USER.EXE", "user.exe"}, {"GDI.EXE", "gdi.exe"}}, "run-1", options);
        if (stdfs::exists(dir.path() / "log.txt"))
            log = fs::read_text(dir.path() / "log.txt");
    }
};

// white, but for one of the three points an error box is white at: the Welcome dialog is not an error box
const Picture kWelcome = with_pixel(solid(640, 480, kWhite), 320, 175, kBlue);
const Picture kErrorBox = solid(640, 480, kWhite);

}  // namespace

CB_TEST("verify.gate16.setup16.pass_when_the_license_page_comes_up", [] {
    const Run run({kWelcome, license_page()});
    CB_CHECK(run.outcome.verdict == Verdict::Pass);
    CB_CHECK(run.outcome.screenshot == run.state / "phase3" / "shots" / "run-1.png");
    CB_CHECK(has(run.log, "cmd: sendkey ret"));        // Continue was pressed once the Welcome dialog was up
    CB_CHECK(has(run.log, "-cdrom"));                  // the machine's arguments
    CB_CHECK(has(run.log, "-M pc -m 64"));
    CB_CHECK(has(run.log, "-display none"));
    CB_CHECK(has(run.log, "snapshot=on"));             // the floppy is not written
    // the last screen was kept as a PNG; the PPM is gone; the run's own folder (the CD and the disk copies) is gone
    CB_CHECK(stdfs::is_regular_file(run.outcome.screenshot));
    CB_CHECK(!stdfs::exists(stdfs::path(run.outcome.screenshot).replace_extension(".ppm")));
    const stdfs::path runs = run.state / "phase3" / "runs";
    CB_CHECK(!stdfs::exists(runs) || stdfs::is_empty(runs));
    const Bytes png = fs::read_file(run.outcome.screenshot);
    CB_CHECK_EQ(png[1], 'P');
    CB_CHECK_EQ(png[18], 0x02);   // width 640 = 0x00000280: IHDR's width field
    CB_CHECK_EQ(png[19], 0x80);
});

CB_TEST("verify.gate16.setup16.the_enter_is_pressed_only_once_the_welcome_dialog_shows", [] {
    // DOS-ish blue screens first: no key. Then Welcome, one Enter, then the page.
    const Run run({solid(640, 480, kBlue), solid(640, 480, kBlue), kWelcome, license_page()});
    CB_CHECK(run.outcome.verdict == Verdict::Pass);
    std::size_t enters = 0, at = 0;
    while ((at = run.log.find("sendkey ret", at)) != std::string::npos) {
        ++enters;
        ++at;
    }
    CB_CHECK_EQ(enters, 1u);
    CB_CHECK(run.log.find("screendump") < run.log.find("sendkey ret"));
});

CB_TEST("verify.gate16.setup16.an_error_box_before_the_welcome_dialog", [] {
    // (an error box is white at its three points, like the Welcome dialog: the test for the box comes first)
    const Run run({kErrorBox});   // white everywhere: both tests hold: the error box is what it is called
    CB_CHECK(run.outcome.verdict == Verdict::FailErrorBoxBeforeWelcome);
    CB_CHECK(!has(run.log, "sendkey"));
});

CB_TEST("verify.gate16.setup16.windows_back_at_the_dos_prompt_after_continue", [] {
    // a Welcome dialog that is not an error box: white but for one of the box's points
    const Picture& welcome = kWelcome;
    const Run run({welcome, solid(720, 400, kBlack)});
    CB_CHECK(run.outcome.verdict == Verdict::FailDos);
    CB_CHECK(has(run.log, "sendkey ret"));
});

CB_TEST("verify.gate16.setup16.an_error_box_twice_after_continue", [] {
    const Picture& welcome = kWelcome;
    const Run run({welcome, kErrorBox});   // after Enter: error boxes, two looks running
    CB_CHECK(run.outcome.verdict == Verdict::FailErrorBox);
});

CB_TEST("verify.gate16.setup16.nothing_known_comes_up_after_continue", [] {
    const Picture& welcome = kWelcome;
    Timing t = quick();
    t.wait = std::chrono::seconds(1);
    const Run run({welcome, solid(640, 480, kBlue)}, false, t);   // Windows' blue screen: not the page, not an error
    CB_CHECK(run.outcome.verdict == Verdict::FailTimeout);
});

CB_TEST("verify.gate16.setup16.the_welcome_dialog_never_comes", [] {
    Timing t = quick();
    t.welcome_polls = 4;
    // Windows' screen with nothing on it that the gate knows: hung
    CB_CHECK(Run({solid(640, 480, kBlue)}, false, t).outcome.verdict == Verdict::FailHang);
    // a black screen: back at DOS
    CB_CHECK(Run({solid(640, 480, kBlack)}, false, t).outcome.verdict == Verdict::FailDos);
    // a text screen that is not black: Setup's DOS part is still going
    CB_CHECK(Run({solid(720, 400, kBlue)}, false, t).outcome.verdict == Verdict::Slow);
});

CB_TEST("verify.gate16.setup16.a_machine_that_ends_by_itself_says_nothing", [] {
    const Run run({kWelcome}, true);
    CB_CHECK(run.outcome.verdict == Verdict::Slow);
});

CB_TEST("verify.gate16.setup16.a_machine_that_cannot_be_started", [] {
    fs::ScopedTempDir dir("cb-setup16");
    fs::write_text(dir.path() / "mini" / "prepared_hd.img", "disk");
    Config config = Config::in_state(dir.path() / "state", dir.path() / "no-such-qemu.exe", dir.path() / "none.iso");
    config.mini_dir = dir.path() / "mini";
    Options options;
    options.timing = quick();
    options.make_cd = [](const stdfs::path& out, const Replace&) { fs::write_text(out, "cd"); };
    CB_CHECK_THROWS_AS(ToolError, continue_test(config, {}, "t", options));
    // and the run's folder is gone
    const stdfs::path runs = dir.path() / "state" / "phase3" / "runs";
    CB_CHECK(!stdfs::exists(runs) || stdfs::is_empty(runs));
});

CB_TEST("verify.gate16.setup16.the_monitor", [] {
    // no one listening: the monitor cannot be reached, and says so
    const Monitor nobody(free_port());
    const std::string cmd = "sendkey ret";
    CB_CHECK(!nobody.send(std::span<const std::string>(&cmd, 1)));
    fs::ScopedTempDir dir("cb-setup16");
    CB_CHECK(!nobody.shot(dir.path() / "s.png").has_value());
    // two free ports in a row are valid ports
    CB_CHECK(free_port() > 0);
});

// ---- the CD -------------------------------------------------------------------------------------------------------------

namespace {

// A miniature of Setup's CD (testdata/setup16/setup_like.iso: /WIN95 with MINI.CAB, the set PRECOPY1.CAB -> PRECOPY2.CAB
// and SETUP.EXE), with the modules MINI.CAB is built from.
struct Cd {
    fs::ScopedTempDir dir{"cb-cd"};
    Config config;
    Cd() {
        config.state = dir.path() / "state";
        config.setup_iso = testdata("setup_like.iso");
        config.mini_dir = dir.path() / "mini";
        fs::make_dirs(config.mini_dir);
        fs::write_file(config.cabinet(), fs::read_file(testdata("MINI.CAB")));
        fs::write_file(config.original() / "MINI1.DLL", fs::read_file(testdata("original/MINI1.DLL")));
        fs::write_file(config.original() / "MINI2.DLL", fs::read_file(testdata("original/MINI2.DLL")));
    }
    stdfs::path file(std::string_view name, std::string_view content) const {
        const stdfs::path p = dir.path() / "ours" / name;
        fs::write_text(p, content);
        return p;
    }
    // the files of a cabinet on the CD, by name
    static std::map<std::string, Bytes> files_of(const stdfs::path& iso, std::string_view cabinet, const stdfs::path& scratch) {
        const formats::iso9660::Volume cd(iso);
        fs::write_file(scratch / std::string(cabinet), cd.read(std::format("/WIN95/{}", cabinet)));
        std::map<std::string, Bytes> out;
        for (const formats::cab::Extracted& e : formats::cab::extract(scratch / std::string(cabinet), scratch / (std::string(cabinet) + ".d")))
            out[e.name] = fs::read_file(e.path);
        return out;
    }
};

std::string text_of(const Bytes& b) { return std::string(as_text(b)); }

}  // namespace

CB_TEST("verify.gate16.setup16.make_cd_replaces_minis_modules", [] {
    Cd cd;
    const stdfs::path out = cd.dir.path() / "out.iso";
    make_cd(cd.config, out, {{"mini1.dll", cd.file("mini1.dll", "OURS ONE")}});   // (the name's case does not matter)
    const auto mini = Cd::files_of(out, "MINI.CAB", cd.dir.path());
    CB_CHECK_EQ(mini.size(), 2u);
    CB_CHECK_EQ(text_of(mini.at("MINI1.DLL")), "OURS ONE");
    CB_CHECK(mini.at("MINI2.DLL") == fs::read_file(testdata("original/MINI2.DLL")));   // the original's, from the folder
    // the rest of the CD is as it was
    const formats::iso9660::Volume now(out), before(testdata("setup_like.iso"));
    CB_CHECK(now.read("/WIN95/SETUP.EXE") == before.read("/WIN95/SETUP.EXE"));
    CB_CHECK(now.read("/WIN95/PRECOPY1.CAB") == before.read("/WIN95/PRECOPY1.CAB"));
    CB_CHECK(now.read("/Win95/Mini.cab", formats::iso9660::Names::Joliet) == now.read("/WIN95/MINI.CAB"));
    // the set id MINI.CAB is written with
    const formats::cab::Cabinet c = formats::cab::parse(now.read("/WIN95/MINI.CAB"));
    CB_CHECK_EQ(c.set_id, 16261);
    CB_CHECK_EQ(c.files.size(), 2u);
    // a second run of the same replaces the same: the CD is made from the original each time
    const stdfs::path again = cd.dir.path() / "again.iso";
    make_cd(cd.config, again, {{"MINI1.DLL", cd.file("mini1.dll", "OURS ONE")}});
    CB_CHECK(Cd::files_of(again, "MINI.CAB", cd.dir.path()) == mini);
});

CB_TEST("verify.gate16.setup16.make_cd_with_nothing_replaced_keeps_the_originals_files", [] {
    Cd cd;
    const stdfs::path out = cd.dir.path() / "out.iso";
    make_cd(cd.config, out, {});
    const auto mini = Cd::files_of(out, "MINI.CAB", cd.dir.path());
    CB_CHECK(mini.at("MINI1.DLL") == fs::read_file(testdata("original/MINI1.DLL")));
    CB_CHECK(mini.at("MINI2.DLL") == fs::read_file(testdata("original/MINI2.DLL")));
});

CB_TEST("verify.gate16.setup16.make_cd_replaces_a_file_of_another_cabinet_and_keeps_the_set_together", [] {
    Cd cd;
    const stdfs::path out = cd.dir.path() / "out.iso";
    make_cd(cd.config, out, {{"B.EXE", cd.file("b.exe", "OUR B")}});
    const stdfs::path scratch = cd.dir.path() / "scratch";
    fs::make_dirs(scratch);
    // PRECOPY1 holds ours, and its other file as it was; PRECOPY2, the other cabinet of the set, is written again too
    const auto pre1 = Cd::files_of(out, "PRECOPY1.CAB", scratch);
    CB_CHECK_EQ(pre1.size(), 2u);
    CB_CHECK_EQ(text_of(pre1.at("B.EXE")), "OUR B");
    CB_CHECK(text_of(pre1.at("A.EXE")).starts_with("a-original-"));
    CB_CHECK(text_of(Cd::files_of(out, "PRECOPY2.CAB", scratch).at("C.EXE")).starts_with("c-original-"));
    // the fields that tie the cabinets into a set are carried over
    const formats::iso9660::Volume now(out);
    const formats::cab::Cabinet c1 = formats::cab::parse(now.read("/WIN95/PRECOPY1.CAB"));
    const formats::cab::Cabinet c2 = formats::cab::parse(now.read("/WIN95/PRECOPY2.CAB"));
    CB_CHECK_EQ(c1.set_id, 77);
    CB_CHECK_EQ(c2.set_id, 77);
    CB_CHECK_EQ(c1.index, 0);
    CB_CHECK_EQ(c2.index, 1);
    CB_CHECK(c1.next.has_value() && c1.next->cabinet == "PRECOPY2.CAB" && c1.next->disk == "Disk 2");
    CB_CHECK(c2.prev.has_value() && c2.prev->cabinet == "PRECOPY1.CAB" && c2.prev->disk == "Disk 1");
    // MINI.CAB is rebuilt from the original's files either way
    CB_CHECK_EQ(Cd::files_of(out, "MINI.CAB", scratch).size(), 2u);
});

CB_TEST("verify.gate16.setup16.make_cd_replaces_a_loose_file", [] {
    Cd cd;
    const stdfs::path out = cd.dir.path() / "out.iso";
    make_cd(cd.config, out, {{"setup.exe", cd.file("setup.exe", "OUR SETUP")}, {"C.EXE", cd.file("c.exe", "OUR C")}});
    const formats::iso9660::Volume now(out);
    CB_CHECK_EQ(text_of(now.read("/WIN95/SETUP.EXE")), "OUR SETUP");
    CB_CHECK_EQ(text_of(Cd::files_of(out, "PRECOPY2.CAB", cd.dir.path()).at("C.EXE")), "OUR C");
    // PRECOPY1 is of the same set: written again too, with its files as they were
    CB_CHECK_EQ(Cd::files_of(out, "PRECOPY1.CAB", cd.dir.path()).size(), 2u);
});

CB_TEST("verify.gate16.setup16.make_cd_errors", [] {
    Cd cd;
    const stdfs::path out = cd.dir.path() / "out.iso";
    CB_CHECK_THROWS_AS(NotFoundError, make_cd(cd.config, out, {{"MINI1.DLL", cd.dir.path() / "no" / "such.dll"}}));   // a file to put in is not there
    Config no_cabinet = cd.config;
    no_cabinet.mini_dir = cd.dir.path() / "nowhere";
    CB_CHECK_THROWS_AS(IoError, make_cd(no_cabinet, out, {}));
    Config no_cd = cd.config;
    no_cd.setup_iso = cd.dir.path() / "no.iso";
    CB_CHECK_THROWS_AS(IoError, make_cd(no_cd, out, {}));
});

CB_TEST("verify.gate16.setup16.config_in_state", [] {
    const Config c = Config::in_state("S", "qemu.exe", "setup.iso");
    CB_CHECK(c.mini_dir == stdfs::path("S") / "mini");
    CB_CHECK(c.boot_floppy == stdfs::path("S") / "bootfloppy" / "boot95_is.img");
    CB_CHECK(c.original() == stdfs::path("S") / "mini" / "original");
    CB_CHECK(c.cabinet() == stdfs::path("S") / "mini" / "MINI.CAB");
    CB_CHECK(c.prepared_disk() == stdfs::path("S") / "mini" / "prepared_hd.img");
});
