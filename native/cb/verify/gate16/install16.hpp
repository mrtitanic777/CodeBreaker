// cb/verify/gate16/install16.hpp -- the whole Windows 95 install, on the modules we rebuilt, to the desktop.
//
// continue_test (setup16) stops at Setup's License page. The milestone goes on: Setup's wizard to Start Copying, the copy of every file,
// Finish and Windows' restart, the FIX95CPU disk (the first restart fails at "While initializing device IOS" on a host this fast: the
// classic timing-loop fault of Windows 95, not one of our modules), the restart again, and Windows 95 finishing its setup until the
// desktop is on the screen. That screen is the pass.
//
//     install16::Outcome o = install16::install(config, replace, "install-1", fix95cpu_floppy);
//     if (o.verdict == install16::Verdict::Pass) ...
//
// The keystrokes are what drove the first full install (2026-10-17..20): Enter and Alt-Y through the wizard's pages, the product
// identification and the user's name typed in, Alt-N on the startup disk, and the long wait of the copy. What waits for what is read off
// the screen -- a page is left when the screen stops changing, or after the time the first run measured -- because the copy alone takes
// from ten minutes to well over an hour on a busy machine. The DOS part (FIX95CPU's boot floppy: a key at its first screen, N to the
// README question, a key to start) is driven the same way, a key pressed again when the screen did not answer it.
//
// The machine is a `Console`: a screen to look at, keys to press, monitor commands, a clock. The production one is QEMU's monitor
// (setup16::Machine); a test gives one that answers like Windows does.
#pragma once

#include <chrono>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/verify/gate16/screen.hpp"
#include "cb/verify/gate16/setup16.hpp"

namespace cb::verify::gate16::install16 {

enum class Verdict {
    Pass,           // the desktop is on the screen (the taskbar, and the teal of Windows' default desktop)
    FailDos,        // a text screen where Windows should be: a protection error, or DOS prompt, that stayed
    FailErrorBox,   // an error box, twice in a row
    FailFix,        // FIX95CPU's disk said it could not patch the installed system
    FailStuck,      // the screen did not change and the keys that could move it did not
    FailTimeout,    // the time for the whole install, or for one of its parts, ran out
    Slow,           // Setup never got as far as its Welcome dialog (or the machine ended): the run says nothing
};

// "PASS", "FAIL(dos)", "FAIL(error box)", "FAIL(fix95cpu)", "FAIL(stuck)", "FAIL(timeout)", "SLOW".
std::string_view word(Verdict v);

struct Outcome {
    Verdict verdict = Verdict::Slow;
    std::string stage;                      // where the run ended: welcome, wizard, copy, finish, fix95cpu, windows
    std::filesystem::path screenshot;       // the last screen (<shots>/<tag>.png); the stages' screens are <tag>-<stage>.png beside it
    std::vector<std::string> log;           // one line a stage: how long it took and what came of it
};

// ---- what is on the screen -------------------------------------------------------------------------------------------

// Windows' desktop: a 640-wide screen with the default desktop's teal over most of it and the taskbar's grey along the bottom.
bool is_desktop(const Picture& screen);
// Windows' "The file ... could not be found ... Copy files from:" box, with its path selected.
bool is_copy_from_box(const Picture& screen);
// A 720-wide text screen with more than 150 pixels of the given text colour (DOS's bright green SUCCESS and bright yellow ERROR of
// FIX95CPU's batch file, and its red text): the colour is matched by sign, not by value (the DAC's rounding).
enum class TextColour { BrightGreen, BrightYellow, Red };
bool has_text_colour(const Picture& screen, TextColour colour);
// The share (0..1) of the pictures' pixels, on a 160x120 copy of each, that differ by more than a few levels; 1 for pictures of two sizes.
double changed_share(const Picture& before, const Picture& after);

// ---- the machine as the driver sees it ---------------------------------------------------------------------------------

class Console {
public:
    virtual ~Console() = default;
    virtual bool alive() = 0;                                   // the machine is still running
    virtual std::optional<Picture> look() = 0;                  // the screen now (nothing when no picture came)
    virtual void key(std::string_view qemu_key) = 0;            // `sendkey`: "ret", "alt-y", "spc", "5", "shift-l"
    virtual void command(std::string_view monitor_command) = 0; // any other monitor command (`change floppy0 ...`)
    virtual std::string ask(std::string_view monitor_command) = 0;   // a monitor command and what the monitor answered
    virtual void sleep(std::chrono::milliseconds time) = 0;
    virtual std::chrono::milliseconds clock() = 0;              // time since the run began (the console's own: a test's is made up)
    virtual void keep(std::string_view stage) = 0;              // the last screen kept as <tag>-<stage>.png
};

// What the run types in, and how long it waits for what. `scale` multiplies every wait (a test runs at a thousandth).
struct Timing {
    double scale = 1.0;
    int welcome_polls = 60;                                     // looks for the Welcome dialog
    std::chrono::seconds welcome_poll{4};
    std::chrono::seconds welcome_settle{8};                     // after the dialog shows, before Enter
    std::chrono::seconds copy_at_least{300};                    // Start Copying is not finished before this
    std::chrono::seconds copy_still{90};                        // the Finish page is the screen that has not changed for this long
    std::chrono::seconds copy_at_most{90 * 60};
    std::chrono::seconds restart_at_most{10 * 60};              // Finish to the first text screen
    std::chrono::seconds fix_boot_at_most{5 * 60};              // to the FIX95CPU disk's first screen
    std::chrono::seconds fix_at_most{15 * 60};                  // the patch itself
    std::chrono::seconds windows_at_most{45 * 60};              // the restart to the desktop
    std::chrono::seconds windows_still{45};                     // a screen this long unchanged is a question waiting: Enter
    std::chrono::seconds text_at_most{4 * 60};                  // a text screen that stays this long, after the patch, is a fault
    int enters_at_most = 25;
    std::string cd_path = "d:\\win95";                          // where Windows has the Setup CD (Setup's DOS part had it as X:)
    std::chrono::seconds everything_at_most{150 * 60};
};

// The product identification and the name typed in the wizard (the key is the OEM one every Windows 95 disk of that kind takes).
struct Entries {
    std::string product_id_groups[3] = {"34698", "0039682", "72135"};
    std::string name = "Logan";
};

// Drive the machine from Setup's Welcome dialog to the desktop. `fix_floppy` is the image of FIX95CPU's disk (put in the drive before Finish).
Outcome drive(Console& console, const std::filesystem::path& fix_floppy, const Timing& timing = {}, const Entries& entries = {});

// ---- over QEMU -----------------------------------------------------------------------------------------------------------

struct Options {
    Timing timing;
    Entries entries;
    setup16::Options machine;   // (the CD's maker and the shots' folder)
};

// Boot Setup on `replace` (the CD with those files in place, the prepared disk) and drive it to the desktop. `fix95cpu` is FIX95CPU.IMA.
// IoError when it is not there.
Outcome install(const setup16::Config& config, const setup16::Replace& replace, std::string_view tag,
                const std::filesystem::path& fix95cpu, const Options& options = {});

}  // namespace cb::verify::gate16::install16
