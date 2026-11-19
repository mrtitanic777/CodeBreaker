// Tests of the install driver (verify/gate16/install16): the screens it reads, and the whole run against a machine that answers as Windows 95's
// Setup does -- made up here, with a clock of its own, so a run that takes an hour on the real machine takes no time. The real run is
// `codebreaker regress install`, which these tests do not start.
#include "cb/verify/gate16/install16.hpp"

#include <algorithm>

#include "cb/core/testing.hpp"
#include "cb/verify/gate16/page_prints.hpp"

using namespace cb;
using namespace cb::verify::gate16;
using namespace cb::verify::gate16::install16;

namespace {

using std::chrono::milliseconds;

Picture solid(std::uint32_t w, std::uint32_t h, Rgb c) {
    Bytes px(static_cast<std::size_t>(w) * h * 3);
    for (std::size_t i = 0; i < px.size(); i += 3) {
        px[i] = c.r;
        px[i + 1] = c.g;
        px[i + 2] = c.b;
    }
    return Picture(w, h, std::move(px));
}

void paint(Picture& p, std::uint32_t x0, std::uint32_t y0, std::uint32_t x1, std::uint32_t y1, Rgb c) {
    Bytes px = p.rgb();
    for (std::uint32_t y = y0; y < y1 && y < p.height(); ++y)
        for (std::uint32_t x = x0; x < x1 && x < p.width(); ++x) {
            const std::size_t at = (static_cast<std::size_t>(y) * p.width() + x) * 3;
            px[at] = c.r;
            px[at + 1] = c.g;
            px[at + 2] = c.b;
        }
    p = Picture(p.width(), p.height(), std::move(px));
}

constexpr Rgb kTeal{0, 130, 130}, kGrey{192, 199, 200}, kWhite{255, 255, 255}, kBlack{0, 0, 0}, kGreen{85, 255, 85}, kYellow{255, 255, 85};

Picture desktop() {
    Picture p = solid(640, 480, kTeal);
    paint(p, 0, 452, 640, 480, kGrey);
    return p;
}
// Setup's Welcome dialog: white over a part of a grey screen, and not at the three points an error box stands on.
Picture welcome() {
    Picture p = solid(640, 480, kGrey);
    paint(p, 150, 180, 490, 305, kWhite);
    return p;
}
// A page of the wizard: the grey screen, a bar `n` wide (it changes with n).
Picture page(unsigned n) {
    Picture p = solid(640, 480, kGrey);
    paint(p, 100, 60, 100 + 8 * (n % 50), 200, Rgb{0, 0, 128});
    return p;
}
// A page with the text of `print` where the real screen has it (the heading is dark on the grey page; a title is white on the blue bar).
Picture with_print(Picture p, const PagePrint& print) {
    if (!print.dark_text) paint(p, print.x - 2, print.y - 3, print.x + 120, print.y + 12, Rgb{0, 0, 168});
    Bytes px = p.rgb();
    for (std::size_t j = 0; j < print.rows.size(); ++j)
        for (std::size_t i = 0; i < print.rows[j].size(); ++i) {
            if (print.rows[j][i] != '#') continue;
            const std::size_t at = ((print.y + j) * p.width() + print.x + i) * 3;
            px[at] = px[at + 1] = px[at + 2] = print.dark_text ? 0 : 255;
        }
    return Picture(p.width(), p.height(), std::move(px));
}
Picture text_screen(unsigned lines, std::optional<Rgb> colour) {
    Picture p = solid(720, 400, kBlack);
    for (unsigned i = 0; i < lines; ++i) paint(p, 40, 20 + 16 * i, 360, 30 + 16 * i, colour.value_or(Rgb{170, 0, 0}));
    return p;
}

// Windows 95's Setup as the driver meets it: a clock of its own (a sleep or a look costs time), a screen that answers the keys of the wizard,
// the long copy, the restart into FIX95CPU's disk and the Windows that comes up after.
class FakeSetup final : public Console {
public:
    enum class Ends { Desktop, FloppyBox, SetupFloppyAgain, FixError, StaysDos, CopyNeverEnds, WindowsErrorBox };
    explicit FakeSetup(Ends ends) : ends_(ends) {}

    std::vector<std::string> keys;
    std::vector<std::string> commands;
    std::vector<std::string> kept;

    bool alive() override { return true; }
    std::optional<Picture> look() override {
        advance(milliseconds(1000));
        ++looks_;
        return screen();
    }
    void key(std::string_view k) override {
        keys.emplace_back(k);
        press(k);
    }
    void command(std::string_view c) override {
        commands.emplace_back(c);
        if (c.rfind("change floppy0", 0) == 0) fix_in_ = true;
        if (c == "system_reset" && phase_ == Phase::Restart) {
            reset_ = true;
            go(Phase::Restart);   // (the clock of the boot begins again)
        }
        if (c == "eject floppy0") fix_in_ = false;
    }
    std::string ask(std::string_view c) override {
        commands.emplace_back(std::string("ask ") + std::string(c));
        if (c == "info block") return fix_in_ ? "floppy0 (#block1): C:/x/fix95cpu.ima (raw)" : "floppy0: boot95_is.img";
        return {};
    }
    void sleep(milliseconds t) override { advance(t); }
    milliseconds clock() override { return now_; }
    void keep(std::string_view stage) override { kept.emplace_back(stage); }

private:
    enum class Phase { SetupDos, Wizard, Copy, Finish, Box, Restart, Fix, Windows } phase_ = Phase::SetupDos;
    Ends ends_;
    milliseconds now_{0}, since_{0};   // the clock; when the phase began
    unsigned page_ = 0;                // the wizard's page; the FIX95CPU disk's screen
    unsigned looks_ = 0;
    bool fix_in_ = false;
    bool reset_ = false;
    bool enter_for_question_ = false;  // Windows' last question answered

    void go(Phase p) {
        phase_ = p;
        since_ = now_;
        page_ = 0;
    }
    milliseconds in_phase() const { return now_ - since_; }
    void advance(milliseconds t) {
        now_ += t;
        if (phase_ == Phase::SetupDos && in_phase() > std::chrono::seconds(30)) go(Phase::Wizard);
        if (phase_ == Phase::Copy && ends_ != Ends::CopyNeverEnds && in_phase() > std::chrono::seconds(900)) go(Phase::Finish);
        // (Setup's own floppy boots again and waits at its prompt until the machine is reset)
        if (phase_ == Phase::Restart && in_phase() > std::chrono::seconds(reset_ ? 20 : 35) && (ends_ != Ends::SetupFloppyAgain || reset_)) go(Phase::Fix);
    }
    void press(std::string_view k) {
        switch (phase_) {
        case Phase::Wizard:
            if (k == "ret" || k == "alt-y" || k == "alt-n") ++page_;   // (typed characters leave the page where it is)
            if (page_ == 12) go(Phase::Copy);                           // Welcome..Start Copying: twelve pages answered
            break;
        case Phase::Finish:
            if (k == "ret") go(ends_ == Ends::FloppyBox ? Phase::Box : Phase::Restart);   // (Setup asks for the floppy disk out first)
            break;
        case Phase::Box:
            if (k == "ret") go(Phase::Restart);
            break;
        case Phase::Fix:
            if (page_ < 3 && ((page_ == 1) == (k == "n"))) ++page_;      // logo, README question, start
            else if (page_ == 4 && k == "spc") go(Phase::Windows);       // "press any key to restart"
            break;
        case Phase::Windows:
            if ((k == "ret" || k == "esc") && page_ == 1) enter_for_question_ = true;
            break;
        default: break;
        }
    }
    Picture screen() {
        switch (phase_) {
        case Phase::SetupDos: return text_screen(3, std::nullopt);
        case Phase::Wizard: return page_ == 0 && in_phase() < std::chrono::seconds(60) ? welcome() : page(page_ * 7);
        case Phase::Copy: return page(looks_);   // the bar moves with every look
        case Phase::Finish: return with_print(page(3), kFinishPrint);
        case Phase::Box: return page(9);
        case Phase::Restart:
            if (in_phase() < std::chrono::seconds(8)) return solid(640, 480, kBlack);
            return text_screen(2, ends_ == Ends::SetupFloppyAgain && !reset_ ? std::optional<Rgb>(Rgb{170, 170, 170}) : std::nullopt);
        case Phase::Fix:
            if (page_ < 3) return text_screen(4 + 2 * page_, Rgb{170, 0, 0});
            if (page_ == 3) {
                if (in_phase() < std::chrono::seconds(60)) return text_screen(5 + looks_ % 7, Rgb{170, 170, 170});   // the files being copied
                if (ends_ == Ends::FixError) return text_screen(10, kYellow);
                page_ = 4;
            }
            return text_screen(12, kGreen);
        case Phase::Windows:
            if (ends_ == Ends::StaysDos) return text_screen(6, Rgb{170, 170, 170});
            if (ends_ == Ends::WindowsErrorBox) return welcome_error_box();
            if (in_phase() < std::chrono::seconds(30)) return text_screen(1, std::nullopt);
            if (!enter_for_question_) {
                page_ = 1;
                return page(40);   // a question that waits for Enter (the time zone)
            }
            if (in_phase() < std::chrono::seconds(400)) return page(looks_);
            return desktop();
        }
        return solid(640, 480, kBlack);
    }
    static Picture welcome_error_box() {
        Picture p = solid(640, 480, kWhite);
        return p;
    }
};

// What the first full install typed through the wizard, key by key: Enter and Alt-Y through the pages, the product identification (three
// groups), the name, Alt-N on the startup disk, the last Enter on Start Copying.
// What the first full install typed through the wizard, key by key: Enter and Alt-Y through the pages, the product identification (three
// groups), the name, Alt-N on the startup disk, the last Enter on Start Copying.
const std::vector<std::string> kWizardKeys = {"ret", "alt-y", "ret", "ret", "ret", "3", "4", "6", "9", "8", "0", "0", "3", "9", "6", "8", "2",
                                              "7", "2", "1", "3", "5", "ret", "shift-l", "o", "g", "a", "n", "ret", "ret", "ret", "alt-n", "ret", "ret"};

}  // namespace

CB_TEST("verify.gate16.install16.words", [] {
    CB_CHECK_EQ(word(Verdict::Pass), "PASS");
    CB_CHECK_EQ(word(Verdict::FailFix), "FAIL(fix95cpu)");
    CB_CHECK_EQ(word(Verdict::FailStuck), "FAIL(stuck)");
    CB_CHECK_EQ(word(Verdict::Slow), "SLOW");
});

CB_TEST("verify.gate16.install16.the_desktop_is_teal_with_a_taskbar", [] {
    CB_CHECK(is_desktop(desktop()));
    Picture real = solid(640, 480, Rgb{87, 168, 168});          // (the desktop as the real machine shows it)
    paint(real, 0, 452, 640, 480, kGrey);
    CB_CHECK(is_desktop(real));
    CB_CHECK(!is_desktop(solid(640, 480, kTeal)));            // (Windows' own teal before the shell: no taskbar)
    CB_CHECK(!is_desktop(page(3)));                            // (a wizard page)
    CB_CHECK(!is_desktop(solid(720, 400, kTeal)));             // (a text screen is never the desktop)
    Picture partly = desktop();
    paint(partly, 0, 0, 640, 400, kGrey);                      // (a window over most of it)
    CB_CHECK(!is_desktop(partly));
});

CB_TEST("verify.gate16.install16.fault_box", [] {
    const Rgb navy{0, 0, 168};
    Picture fault = solid(640, 480, kGrey);
    paint(fault, 92, 160, 548, 166, navy);                     // (the system-modal frame: top edge, then the sides)
    paint(fault, 92, 160, 98, 320, navy);
    paint(fault, 542, 160, 548, 320, navy);
    CB_CHECK(is_fault_box(fault));
    Picture message = solid(640, 480, kGrey);
    paint(message, 92, 160, 548, 162, Rgb{255, 255, 255});     // (a message box: no navy frame)
    paint(message, 120, 165, 520, 185, navy);                  // (and its dark-blue title bar)
    CB_CHECK(!is_fault_box(message));
    CB_CHECK(!is_fault_box(solid(720, 400, navy)));            // (a text screen)
});

CB_TEST("verify.gate16.install16.text_colours", [] {
    CB_CHECK(has_text_colour(text_screen(12, kGreen), TextColour::BrightGreen));
    CB_CHECK(!has_text_colour(text_screen(12, kGreen), TextColour::BrightYellow));
    CB_CHECK(has_text_colour(text_screen(12, kYellow), TextColour::BrightYellow));
    CB_CHECK(!has_text_colour(text_screen(12, Rgb{170, 0, 0}), TextColour::BrightGreen));
    CB_CHECK(!has_text_colour(solid(640, 480, kGreen), TextColour::BrightGreen));   // (not a text screen)
});

CB_TEST("verify.gate16.install16.changed_share", [] {
    CB_CHECK_EQ(changed_share(page(3), page(3)), 0.0);
    CB_CHECK(changed_share(page(3), page(20)) > 0.003);
    CB_CHECK_EQ(changed_share(page(3), text_screen(2, std::nullopt)), 1.0);   // (two sizes)
});

CB_TEST("verify.gate16.install16.the_install_reaches_the_desktop", [] {
    FakeSetup machine(FakeSetup::Ends::Desktop);
    const Outcome o = drive(machine, "fix95cpu.ima");
    CB_CHECK_EQ(std::string(word(o.verdict)), "PASS");
    CB_CHECK_EQ(o.stage, "windows");
    // the wizard's keys, as the first full install pressed them; then Finish, FIX95CPU's three keys (any, N, any), the key that restarts
    // it, and the Enter of Windows' last question
    std::vector<std::string> expected = kWizardKeys;
    for (const char* k : {"ret", "spc", "n", "spc", "spc", "esc"}) expected.emplace_back(k);
    const auto joined = [](const std::vector<std::string>& v) { std::string t; for (const std::string& k : v) t += k + " "; return t; };
    CB_CHECK_EQ(joined(machine.keys), joined(expected));
    // FIX95CPU's disk goes in before Finish and comes out before the restart; the machine boots from the hard disk
    CB_CHECK_EQ(machine.commands.size(), std::size_t{4});
    CB_CHECK(machine.commands[0].rfind("change floppy0 fix95cpu.ima", 0) == 0);
    CB_CHECK_EQ(machine.commands[1], "ask info block");   // (the disk is in when the machine says so)
    CB_CHECK_EQ(machine.commands[2], "boot_set c");
    CB_CHECK_EQ(machine.commands[3], "eject floppy0");    // (no reset: the machine booted FIX95CPU's disk by itself)
    // every stage is kept for whoever looks, the desktop last
    CB_CHECK(std::find(machine.kept.begin(), machine.kept.end(), "copy") != machine.kept.end());
    CB_CHECK(std::find(machine.kept.begin(), machine.kept.end(), "fix95cpu") != machine.kept.end());
    CB_CHECK_EQ(machine.kept.back(), "desktop");
    CB_CHECK(o.log.size() >= 5);
});

CB_TEST("verify.gate16.install16.the_patch_that_fails_fails_the_run", [] {
    FakeSetup machine(FakeSetup::Ends::FixError);
    const Outcome o = drive(machine, "fix95cpu.ima");
    CB_CHECK_EQ(std::string(word(o.verdict)), "FAIL(fix95cpu)");
    CB_CHECK_EQ(o.stage, "fix95cpu");
    CB_CHECK(std::find(machine.commands.begin(), machine.commands.end(), "eject floppy0") == machine.commands.end());   // (the disk stays)
});

CB_TEST("verify.gate16.install16.windows_that_stays_in_dos_fails", [] {
    FakeSetup machine(FakeSetup::Ends::StaysDos);
    const Outcome o = drive(machine, "fix95cpu.ima");
    CB_CHECK_EQ(std::string(word(o.verdict)), "FAIL(dos)");
    CB_CHECK_EQ(o.stage, "windows");
});

CB_TEST("verify.gate16.install16.an_error_box_in_windows_fails", [] {
    FakeSetup machine(FakeSetup::Ends::WindowsErrorBox);
    const Outcome o = drive(machine, "fix95cpu.ima");
    CB_CHECK_EQ(std::string(word(o.verdict)), "FAIL(error box)");
});

CB_TEST("verify.gate16.install16.a_copy_that_never_ends_times_out", [] {
    FakeSetup machine(FakeSetup::Ends::CopyNeverEnds);
    Timing timing;
    timing.copy_at_most = std::chrono::seconds(30 * 60);
    const Outcome o = drive(machine, "fix95cpu.ima", timing);
    CB_CHECK_EQ(std::string(word(o.verdict)), "FAIL(timeout)");
    CB_CHECK_EQ(o.stage, "copy");
    CB_CHECK(machine.commands.empty());   // (Finish was never reached: the floppy was not touched)
});

CB_TEST("verify.gate16.install16.the_floppy_disk_box_is_answered_ok", [] {
    FakeSetup machine(FakeSetup::Ends::FloppyBox);
    const Outcome o = drive(machine, "fix95cpu.ima");
    CB_CHECK_EQ(std::string(word(o.verdict)), "PASS");
    CB_CHECK(std::find(machine.kept.begin(), machine.kept.end(), "finish-box") != machine.kept.end());
});

CB_TEST("verify.gate16.install16.the_copy_from_box_has_its_path_selected", [] {
    Picture box = solid(640, 480, Rgb{0, 0, 255});   // (the desktop pattern bright blue is not the selection)
    CB_CHECK(!is_copy_from_box(box));
    paint(box, 130, 258, 325, 272, Rgb{0, 0, 168});
    CB_CHECK(is_copy_from_box(box));
    CB_CHECK(!is_copy_from_box(page(3)));
});

CB_TEST("verify.gate16.install16.setups_own_floppy_booting_again_is_swapped_and_reset", [] {
    FakeSetup machine(FakeSetup::Ends::SetupFloppyAgain);
    const Outcome o = drive(machine, "fix95cpu.ima");
    CB_CHECK_EQ(std::string(word(o.verdict)), "PASS");
    const auto reset = std::find(machine.commands.begin(), machine.commands.end(), "system_reset");
    CB_CHECK(reset != machine.commands.end());
    // the disk is put in again, and found in, before the reset (change, ask), and the reset is not the first thing done
    CB_CHECK(reset - machine.commands.begin() >= 4);
});

CB_TEST("verify.gate16.install16.pages_are_known_by_their_text", [] {
    CB_CHECK(is_finish_page(with_print(page(3), kFinishPrint)));
    CB_CHECK(!is_finish_page(page(3)));                   // the grey page without its heading
    CB_CHECK(!is_finish_page(welcome()));
    CB_CHECK(!is_finish_page(desktop()));
    CB_CHECK(!is_finish_page(text_screen(5, std::nullopt)));
    for (const PagePrint* print : {&kTimeZonePrint, &kPrinterPrint, &kRestartPrint}) {
        CB_CHECK(is_known_question(with_print(solid(640, 480, kGrey), *print)));
        CB_CHECK(!is_finish_page(with_print(solid(640, 480, kGrey), *print)));
    }
    CB_CHECK(!is_known_question(page(40)));
    CB_CHECK(!is_known_question(with_print(page(3), kFinishPrint)));
});

// The recognising wait gives the verdict the fixed waits give, on every ending the machine can have -- the same stage, the same keys -- and
// gets there sooner when the screens are the real ones.
CB_TEST("verify.gate16.install16.recognising_screens_changes_the_time_not_the_verdict", [] {
    using Ends = FakeSetup::Ends;
    for (const Ends ends : {Ends::Desktop, Ends::FloppyBox, Ends::SetupFloppyAgain, Ends::FixError, Ends::StaysDos, Ends::WindowsErrorBox,
                            Ends::CopyNeverEnds}) {
        Timing slow, fast;
        fast.recognise = true;
        slow.copy_at_most = fast.copy_at_most = std::chrono::seconds(30 * 60);
        FakeSetup a(ends), b(ends);
        const Outcome oa = drive(a, "fix95cpu.ima", slow);
        const Outcome ob = drive(b, "fix95cpu.ima", fast);
        CB_CHECK_EQ(std::string(word(ob.verdict)), std::string(word(oa.verdict)));
        CB_CHECK_EQ(ob.stage, oa.stage);
        CB_CHECK(b.clock() <= a.clock());
        if (ends == Ends::Desktop) {
            CB_CHECK(b.clock() + std::chrono::minutes(5) < a.clock());
            // the same keys, in the same order (the wizard's pages, Finish, FIX95CPU's disk, the question)
            CB_CHECK_EQ(b.keys.size(), a.keys.size());
            for (std::size_t i = 0; i < a.keys.size() && i < b.keys.size(); ++i) CB_CHECK_EQ(b.keys[i], a.keys[i]);
        }
    }
});
