#include "cb/verify/gate16/install16.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::verify::gate16::install16 {

namespace stdfs = std::filesystem;
using std::chrono::milliseconds;
using std::chrono::seconds;

namespace {

constexpr std::uint32_t kGraphicsWidth = 640;   // Windows' screen
constexpr std::uint32_t kTextWidth = 720;       // DOS's and the BIOS's text screen

bool near_grey(const Rgb& p, unsigned lowest, unsigned highest) {
    const unsigned lo = std::min({p.r, p.g, p.b}), hi = std::max({p.r, p.g, p.b});
    return lo >= lowest && hi <= highest && hi - lo <= 16;
}

// The default desktop's teal. The 16-colour driver's DAC has six bits a channel, so the 128 comes out a few levels either side.
bool is_teal(const Rgb& p) { return p.r < 40 && p.g >= 100 && p.g <= 160 && p.b >= 100 && p.b <= 160 && (p.g > p.b ? p.g - p.b : p.b - p.g) < 16; }

std::string key_for(char c) {
    switch (c) {
    case ' ': return "spc";
    case '.': return "dot";
    case '-': return "minus";
    case ',': return "comma";
    case '/': return "slash";
    case '\\': return "backslash";
    case ':': return "shift-semicolon";
    default: break;
    }
    if (c >= 'A' && c <= 'Z') return std::string("shift-") + static_cast<char>(c - 'A' + 'a');
    return std::string(1, c);
}

// The run: what it has done and the clock.
class Driver {
public:
    Driver(Console& console, const Timing& timing, const std::filesystem::path& fix_floppy)
        : c_(console), t_(timing), fix_(fix_floppy) {}

    Outcome run(const Entries& entries);

private:
    seconds scaled(seconds s) const { return seconds(static_cast<std::int64_t>(std::llround(static_cast<double>(s.count()) * t_.scale))); }
    milliseconds scaled_ms(milliseconds s) const { return milliseconds(static_cast<std::int64_t>(std::llround(static_cast<double>(s.count()) * t_.scale))); }
    void nap(seconds s) { c_.sleep(std::max(milliseconds(1), scaled_ms(std::chrono::duration_cast<milliseconds>(s)))); }
    bool out_of_time() { return c_.clock() > std::chrono::duration_cast<milliseconds>(scaled(t_.everything_at_most)); }

    // The screen now, remembered as the last one.
    const std::optional<Picture>& look() {
        if (std::optional<Picture> p = c_.look()) last_ = std::move(p);
        return last_;
    }
    bool still(const Picture& a, const Picture& b) { return changed_share(a, b) < 0.003; }

    // Wait for a page: half of `at_most` as it is, then until the screen stops changing (two looks in a row alike), then no longer than `at_most`.
    void settle(seconds at_most, bool early_ok = true) {
        const milliseconds start = c_.clock();
        const milliseconds limit = std::chrono::duration_cast<milliseconds>(scaled(at_most));
        if (early_ok) {
            c_.sleep(std::max(milliseconds(1), limit / 2));
            int alike = 0;
            std::optional<Picture> before = look();
            while (c_.clock() - start < limit) {
                nap(seconds(3));
                const std::optional<Picture>& now = look();
                if (before && now && still(*before, *now)) {
                    if (++alike >= 2) return;
                } else {
                    alike = 0;
                }
                before = now;
            }
        } else {
            c_.sleep(std::max(milliseconds(1), limit));
        }
    }

    void press(std::string_view k) { c_.key(k); }
    void type(std::string_view text) {
        for (const char ch : text) c_.key(key_for(ch));
    }
    // Press a key; when the screen did not answer in `answer`, press it again (a DOS program that clears the keyboard before it asks
    // eats a key that came early). False when it never did.
    bool press_until_it_answers(std::string_view k, int tries = 4, seconds answer = seconds(20)) {
        for (int i = 0; i < tries; ++i) {
            const std::optional<Picture> before = look();
            c_.key(k);
            const milliseconds start = c_.clock();
            while (c_.clock() - start < std::chrono::duration_cast<milliseconds>(scaled(answer))) {
                nap(seconds(4));
                const std::optional<Picture>& now = look();
                if (before && now && changed_share(*before, *now) > 0.01) return true;
                if (!c_.alive()) return false;
            }
        }
        return false;
    }
    // Wait until the screen has not changed for `quiet` (at most `at_most`). False when `at_most` passed first.
    bool wait_quiet(seconds quiet, seconds at_most) {
        const milliseconds start = c_.clock();
        std::optional<Picture> before = look();
        milliseconds since = c_.clock();
        while (c_.clock() - start < std::chrono::duration_cast<milliseconds>(scaled(at_most))) {
            nap(seconds(4));
            const std::optional<Picture>& now = look();
            if (!now) continue;
            if (before && still(*before, *now)) {
                if (c_.clock() - since >= std::chrono::duration_cast<milliseconds>(scaled(quiet))) return true;
            } else {
                since = c_.clock();
            }
            before = now;
            if (!c_.alive()) return false;
        }
        return false;
    }

    void stage_ends(std::string_view stage, std::string_view what) {
        const auto s = std::chrono::duration_cast<seconds>(c_.clock());
        out_.stage = std::string(stage);
        out_.log.push_back(std::format("{:>6}s  {}: {}", s.count(), stage, what));
        c_.keep(stage);
    }
    Outcome& fail(Verdict v, std::string_view stage, std::string_view what) {
        out_.verdict = v;
        stage_ends(stage, what);
        return out_;
    }

    bool welcome();
    void wizard(const Entries& entries);
    bool copy();
    bool put_in_fix_disk();
    bool restart_into_dos();
    bool fix95cpu();
    void windows();

    Console& c_;
    Timing t_;
    std::filesystem::path fix_;
    std::optional<Picture> last_;
    Outcome out_;
    bool concluded_ = false;
};

// Setup's DOS part runs until the Welcome dialog shows; no key goes to it (it asks for none from a prepared disk).
bool Driver::welcome() {
    for (int i = 0; i < t_.welcome_polls; ++i) {
        nap(t_.welcome_poll);
        if (!c_.alive()) {
            fail(Verdict::Slow, "welcome", "the machine ended before the Welcome dialog");
            return false;
        }
        const std::optional<Picture>& p = look();
        if (!p) continue;
        if (setup16::is_error_box(*p)) {
            fail(Verdict::FailErrorBox, "welcome", "an error box before the Welcome dialog");
            return false;
        }
        if (setup16::is_welcome(*p)) {
            nap(t_.welcome_settle);
            stage_ends("welcome", "the Welcome dialog");
            return true;
        }
    }
    fail(Verdict::Slow, "welcome", "Setup's DOS part never finished");
    return false;
}

// The wizard's pages, each answered as the first full install answered it (the wait after is that run's).
void Driver::wizard(const Entries& entries) {
    press("ret");                                  settle(seconds(60));    // Welcome: Continue
    press("alt-y");                                settle(seconds(90));    // License: Yes
    press("ret");                                  settle(seconds(60));    // Setup Wizard: Next
    press("ret");                                  settle(seconds(90));    // Directory: Next
    press("ret");                                  settle(seconds(60));    // Setup options: Next
    for (const std::string& group : entries.product_id_groups) {           // Product identification
        type(group);
        nap(seconds(1));
    }
    nap(seconds(1));
    press("ret");                                  settle(seconds(30));    // Next
    type(entries.name);
    press("ret");                                  settle(seconds(60));    // User information: Next
    press("ret");                                  settle(seconds(240), false);   // Analyzing your computer: Next (the detection runs)
    press("ret");                                  settle(seconds(40));    // Windows components: Next
    press("alt-n");                                nap(seconds(2));        // Startup disk: No
    press("ret");                                  settle(seconds(30));    // Next
    stage_ends("wizard", "to Start Copying");
}

// Start Copying, and the copy: the Finish page is the screen that stops changing.
bool Driver::copy() {
    press("ret");   // Start copying: Next
    const milliseconds start = c_.clock();
    c_.sleep(std::max(milliseconds(1), std::chrono::duration_cast<milliseconds>(scaled(t_.copy_at_least))));
    std::optional<Picture> before = look();
    milliseconds since = c_.clock();
    while (c_.clock() - start < std::chrono::duration_cast<milliseconds>(scaled(t_.copy_at_most))) {
        if (!c_.alive()) {
            fail(Verdict::Slow, "copy", "the machine ended during the copy");
            return false;
        }
        if (out_of_time()) break;
        nap(seconds(10));
        const std::optional<Picture>& now = look();
        if (!now) continue;
        // (no error-box test here: the copy's slides -- "Register now" -- are white where an error box is, and stay for half a minute)
        if (before && still(*before, *now)) {
            if (c_.clock() - since >= std::chrono::duration_cast<milliseconds>(scaled(t_.copy_still))) {
                stage_ends("copy", std::format("the copy is done after {}s: Finish", std::chrono::duration_cast<seconds>(c_.clock() - start).count()));
                return true;
            }
        } else {
            since = c_.clock();
        }
        before = now;
    }
    fail(Verdict::FailTimeout, "copy", "the copy did not finish");
    return false;
}

// FIX95CPU's disk in the floppy drive: the machine says so when it is (a `change` made while the guest holds the drive can be refused).
bool Driver::put_in_fix_disk() {
    const std::string change = std::format("change floppy0 {} raw", text::replace_all(fs::to_text(fix_), "\\", "/"));
    for (int tries = 0; tries < 6; ++tries) {
        c_.command(change);
        nap(seconds(2));
        if (text::lower(c_.ask("info block")).find("fix95cpu") != std::string::npos) return true;
        nap(seconds(5));
    }
    fail(Verdict::FailStuck, "finish", "FIX95CPU's disk could not be put in the floppy drive");
    return false;
}

// FIX95CPU's disk in the drive (before Finish), Finish, and the machine goes down: the first text screen after Windows' is the BIOS's.
bool Driver::restart_into_dos() {
    if (!put_in_fix_disk()) return false;
    press("ret");   // Finish
    const milliseconds start = c_.clock();
    std::optional<Picture> before;
    milliseconds since = c_.clock();
    int oks = 0;
    while (c_.clock() - start < std::chrono::duration_cast<milliseconds>(scaled(t_.restart_at_most))) {
        nap(seconds(5));
        if (!c_.alive()) {
            fail(Verdict::Slow, "finish", "the machine ended at Finish");
            return false;
        }
        const std::optional<Picture>& p = look();
        if (!p) continue;
        if (p->width() == kTextWidth) {
            // Whatever the machine booted -- FIX95CPU's disk, or the floppy Setup started from (its DOS part again: an "X:\WIN95>" prompt that
            // waits for nothing), which a restart can fall back to -- it is made to boot FIX95CPU's now: the disk in, the machine reset.
            nap(seconds(15));
            if (!put_in_fix_disk()) return false;
            c_.command("system_reset");
            nap(seconds(5));
            stage_ends("finish", std::format("Windows restarted into DOS{}; the machine reset to boot FIX95CPU's disk", oks ? " (after OK to the floppy-disk box)" : ""));
            return true;
        }
        // Setup sees the disk in the drive and asks for it out ("Setup detected a disk in your floppy disk drive... Click OK to restart"):
        // the disk is the one meant to be there, OK restarts all the same. A screen that waits this long is that box.
        if (before && still(*before, *p)) {
            if (c_.clock() - since >= std::chrono::duration_cast<milliseconds>(scaled(seconds(20))) && oks < 3) {
                c_.keep("finish-box");
                press("ret");
                ++oks;
                since = c_.clock();
            }
        } else {
            since = c_.clock();
        }
        before = p;
    }
    fail(Verdict::FailStuck, "finish", "Finish did not restart the machine");
    return false;
}

// FIX95CPU's disk boots to its batch file: a key at the logo, N to "view the README?", a key to start; then the patch runs to the green
// SUCCESS (or the yellow ERROR); the disk comes out, the boot goes to the hard disk, a key restarts.
bool Driver::fix95cpu() {
    if (!wait_quiet(seconds(15), t_.fix_boot_at_most)) {
        fail(Verdict::FailStuck, "fix95cpu", "the FIX95CPU disk's first screen never settled");
        return false;
    }
    if (!press_until_it_answers("spc")) {   // Press any key to continue
        fail(Verdict::FailStuck, "fix95cpu", "the first screen did not answer a key");
        return false;
    }
    wait_quiet(seconds(8), seconds(120));
    if (!press_until_it_answers("n")) {     // View the README file? No
        fail(Verdict::FailStuck, "fix95cpu", "the README question did not answer N");
        return false;
    }
    wait_quiet(seconds(8), seconds(120));
    if (!press_until_it_answers("spc")) {   // Ready to install: any key
        fail(Verdict::FailStuck, "fix95cpu", "the patch did not start");
        return false;
    }
    const milliseconds start = c_.clock();
    while (c_.clock() - start < std::chrono::duration_cast<milliseconds>(scaled(t_.fix_at_most))) {
        nap(seconds(5));
        const std::optional<Picture>& p = look();
        if (!c_.alive()) {
            fail(Verdict::Slow, "fix95cpu", "the machine ended during the patch");
            return false;
        }
        if (!p) continue;
        if (has_text_colour(*p, TextColour::BrightYellow)) {
            fail(Verdict::FailFix, "fix95cpu", "FIX95CPU reported an error");
            return false;
        }
        if (has_text_colour(*p, TextColour::BrightGreen)) {
            nap(seconds(5));
            stage_ends("fix95cpu", "patched");
            // The machine boots the hard disk from now on. The disk stays in until the machine has gone down: the batch file's last
            // step, RESTART, is read from it ("General failure reading drive A" when it is out before the key).
            c_.command("boot_set c");
            nap(seconds(2));
            press("spc");                  // Press any key to restart
            nap(seconds(1));
            c_.command("eject floppy0");
            return true;
        }
    }
    fail(Verdict::FailTimeout, "fix95cpu", "the patch did not finish");
    return false;
}

// Windows 95 finishes its setup: hardware, control panel, time zone, restarts; each screen that waits for an answer gets Enter. The desktop
// (twice in a row) is the pass.
void Driver::windows() {
    const milliseconds start = c_.clock();
    std::optional<Picture> before;
    milliseconds since = c_.clock();      // the screen has been the same since
    milliseconds text_since{-1};
    int enters = 0, errors = 0, desktops = 0;
    while (c_.clock() - start < std::chrono::duration_cast<milliseconds>(scaled(t_.windows_at_most)) && !out_of_time()) {
        nap(seconds(10));
        if (!c_.alive()) {
            fail(Verdict::Slow, "windows", "the machine ended");
            return;
        }
        const std::optional<Picture>& now = look();
        if (!now) continue;
        desktops = is_desktop(*now) ? desktops + 1 : 0;
        if (desktops >= 2) {
            out_.verdict = Verdict::Pass;
            stage_ends("windows", "the desktop");
            c_.keep("desktop");
            return;
        }
        if (now->width() == kTextWidth) {
            if (text_since < milliseconds(0)) text_since = c_.clock();
            if (c_.clock() - text_since > std::chrono::duration_cast<milliseconds>(scaled(t_.text_at_most))) {
                fail(Verdict::FailDos, "windows", "a text screen where Windows should be");
                return;
            }
        } else {
            text_since = milliseconds(-1);
        }
        errors = setup16::is_error_box(*now) ? errors + 1 : 0;
        if (errors >= 3 && before && still(*before, *now)) {
            fail(Verdict::FailErrorBox, "windows", "an error box");
            return;
        }
        if (before && still(*before, *now)) {
            if (c_.clock() - since >= std::chrono::duration_cast<milliseconds>(scaled(t_.windows_still)) && now->width() == kGraphicsWidth) {
                if (enters >= t_.enters_at_most) {
                    fail(Verdict::FailStuck, "windows", std::format("the screen waits and {} Enters did not move it", enters));
                    return;
                }
                ++enters;
                c_.keep(std::format("windows-{}", enters));
                if (is_copy_from_box(*now)) {
                    // "The file ... could not be found: copy files from X:\WIN95". X: was the CD in Setup's DOS part; Windows has it as D:
                    type(t_.cd_path);
                    press("ret");
                } else {
                    // A question that waits: Esc closes what can be closed (the time zone; the Add Printer Wizard, which would copy the
                    // printer's driver from the CD -- an install does not need one), Enter answers what cannot (every other press).
                    press(enters % 2 == 1 ? "esc" : "ret");
                }
                since = c_.clock();
            }
        } else {
            since = c_.clock();
        }
        before = now;
    }
    fail(Verdict::FailTimeout, "windows", "no desktop in time");
}

Outcome Driver::run(const Entries& entries) {
    out_ = Outcome{};
    if (!welcome()) return out_;
    wizard(entries);
    if (!copy()) return out_;
    if (!restart_into_dos()) return out_;
    if (!fix95cpu()) return out_;
    windows();
    return out_;
}

}  // namespace

std::string_view word(Verdict v) {
    switch (v) {
    case Verdict::Pass: return "PASS";
    case Verdict::FailDos: return "FAIL(dos)";
    case Verdict::FailErrorBox: return "FAIL(error box)";
    case Verdict::FailFix: return "FAIL(fix95cpu)";
    case Verdict::FailStuck: return "FAIL(stuck)";
    case Verdict::FailTimeout: return "FAIL(timeout)";
    case Verdict::Slow: return "SLOW";
    }
    return "SLOW";
}

bool is_desktop(const Picture& screen) {
    if (screen.width() != kGraphicsWidth || screen.height() < 400) return false;
    const Picture small = screen.resized(160, 120);
    const Bytes& px = small.rgb();
    unsigned teal = 0;
    for (std::size_t i = 0; i + 2 < px.size(); i += 3) teal += is_teal(Rgb{px[i], px[i + 1], px[i + 2]});
    if (teal < 160 * 120 * 35 / 100) return false;
    // the taskbar: light grey along the bottom (the Start button's face and the bar beside it)
    const std::uint32_t y = screen.height() - 8;
    int grey = 0;
    for (const std::uint32_t x : {300u, 420u, 520u, 600u}) {
        const std::optional<Rgb> p = screen.at(x, y);
        grey += p && near_grey(*p, 150, 230);
    }
    return grey >= 3;
}

bool is_copy_from_box(const Picture& screen) {
    if (screen.width() != kGraphicsWidth) return false;
    // the box's "Copy files from" field holds its text selected: dark blue (not the bright blue of the desktop's pattern) at three points
    const auto navy = [](const std::optional<Rgb>& p) { return p && p->r < 48 && p->g < 48 && p->b >= 100 && p->b <= 200; };
    return navy(screen.at(135, 265)) && navy(screen.at(250, 265)) && navy(screen.at(310, 265));
}

bool has_text_colour(const Picture& screen, TextColour colour) {
    if (screen.width() != kTextWidth) return false;
    const Bytes& px = screen.rgb();
    unsigned n = 0;
    for (std::size_t i = 0; i + 2 < px.size(); i += 3) {
        const unsigned r = px[i], g = px[i + 1], b = px[i + 2];
        switch (colour) {
        case TextColour::BrightGreen: n += g > 220 && r < 130 && b < 130; break;
        case TextColour::BrightYellow: n += r > 220 && g > 220 && b < 130; break;
        }
    }
    return n > 150;
}

double changed_share(const Picture& before, const Picture& after) {
    if (before.width() != after.width() || before.height() != after.height() || before.width() == 0) return 1.0;
    const Picture a = before.resized(160, 120), b = after.resized(160, 120);
    const Bytes& pa = a.rgb();
    const Bytes& pb = b.rgb();
    unsigned changed = 0;
    for (std::size_t i = 0; i + 2 < pa.size(); i += 3) {
        const int d = std::abs(int{pa[i]} - pb[i]) + std::abs(int{pa[i + 1]} - pb[i + 1]) + std::abs(int{pa[i + 2]} - pb[i + 2]);
        changed += d > 24;
    }
    return static_cast<double>(changed) / (160.0 * 120.0);
}

Outcome drive(Console& console, const std::filesystem::path& fix_floppy, const Timing& timing, const Entries& entries) {
    Driver driver(console, timing, fix_floppy);
    return driver.run(entries);
}

// ---- over QEMU -----------------------------------------------------------------------------------------------------------

namespace {

class MonitorConsole final : public Console {
public:
    MonitorConsole(setup16::Machine& machine, std::string tag)
        : machine_(machine), tag_(std::move(tag)), shots_(machine.screenshot().parent_path()), start_(std::chrono::steady_clock::now()) {}

    bool alive() override { return machine_.running(); }
    std::optional<Picture> look() override { return machine_.monitor().shot(machine_.screenshot()); }
    void key(std::string_view k) override {
        const std::string command = std::format("sendkey {}", k);
        (void)machine_.monitor().send(std::span<const std::string>(&command, 1));
    }
    void command(std::string_view c) override {
        const std::string text(c);
        (void)machine_.monitor().send(std::span<const std::string>(&text, 1));
    }
    std::string ask(std::string_view c) override { return machine_.monitor().ask(c); }
    void sleep(milliseconds time) override { std::this_thread::sleep_for(time); }
    milliseconds clock() override { return std::chrono::duration_cast<milliseconds>(std::chrono::steady_clock::now() - start_); }
    void keep(std::string_view stage) override {
        std::error_code ec;
        stdfs::copy_file(machine_.screenshot(), shots_ / std::format("{}-{}.png", tag_, stage), stdfs::copy_options::overwrite_existing, ec);
    }

private:
    setup16::Machine& machine_;
    std::string tag_;
    stdfs::path shots_;
    std::chrono::steady_clock::time_point start_;
};

}  // namespace

Outcome install(const setup16::Config& config, const setup16::Replace& replace, std::string_view tag, const stdfs::path& fix95cpu,
                const Options& options) {
    // FIX95CPU's disk is a copy in the run's own folder: the machine may write to a floppy, never the original
    setup16::Machine machine(config, replace, tag, options.machine);
    const stdfs::path floppy = machine.folder() / "fix95cpu.ima";
    std::error_code ec;
    stdfs::copy_file(fix95cpu, floppy, stdfs::copy_options::overwrite_existing, ec);
    if (ec) throw IoError(fix95cpu, ec.value(), "FIX95CPU's disk cannot be copied: " + ec.message());
    MonitorConsole console(machine, std::string(tag));
    Outcome outcome = drive(console, floppy, options.timing, options.entries);
    outcome.screenshot = machine.screenshot();
    return outcome;
}

}  // namespace cb::verify::gate16::install16
