// cb/source/vxd/boot.hpp -- does Windows 95 start with these drivers? A rebuilt driver's real test.
//
//     codebreaker vxd boot [--drivers NAME,...] [--vmm32] [--retail] [--bisect] [--disk IMAGE] [--wait SECONDS] [--tag NAME]
//
// Each driver named (vtd, vpicd, drvspacx ...: a file of the corpus) is built from the recovered disassembly with the LE writer, and with
// --vmm32 Windows' own VMM32.VXD with the bundle writer; they are put into a copy of an installed Windows 95 disk, and the copy is booted
// in QEMU with no window. Windows reaching its desktop (or a logon box over it) is a pass; a blue screen, a screen that stays black or in
// text (a "Windows protection error" is one), or no desktop in --wait seconds (default 300) is a fail. --retail puts the corpus originals in
// instead of the rebuilt files: the baseline a rebuilt run is measured against. --bisect, when the run fails, boots halves of the set until
// it finds the drivers that fail by themselves.
//
// Where each file goes: a driver kept in WINDOWS\SYSTEM of the corpus goes into WINDOWS\SYSTEM\VMM32 -- Windows loads a driver found there
// in place of the copy inside VMM32.VXD -- and one kept in a folder below it (IOSUBSYS) or with another extension (.386) where the corpus
// has it. VMM32.VXD goes into WINDOWS\SYSTEM. The corpus VMM32.VXD holds VMM alone, where the installed one (W4, compressed) holds 43
// drivers; so with --vmm32 every other driver of the installed bundle the disk has no file of in VMM32 is put there from the corpus too
// (Windows takes a driver it does not find in the bundle from there).
//
// The disk is never changed: --disk, or the setting "win95_installed_hd" (a raw image of a hard disk Windows 95 is installed on), is copied
// into a run folder of its own (base/runs), which is deleted when the run ends -- and a killed run's, by the next run, once it is a day old.
// BootLog=1 is set in the copy's MSDOS.SYS; after the run the copy's BOOTLOG.TXT says which of the files Windows loaded, and it is kept with
// the last screen, one of each per tag (<state>/vxdboot/shots/<tag>.png and <tag>.bootlog.txt; the tag is "rebuilt" or "retail" unless --tag
// names another, and a run overwrites the last of its tag). That image has FIX95CPU's drivers in VMM32 already: without them a fast host fails
// at IOS, whatever the drivers (the classic timing-loop fault of Windows 95), and those files are left alone.
//
// QEMU runs at below-normal priority, one machine at a time.
#pragma once

#include <chrono>
#include <filesystem>
#include <functional>
#include <map>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/base/corpus.hpp"
#include "cb/core/bytes.hpp"
#include "cb/source/vxd/disassembly.hpp"
#include "cb/verify/gate16/screen.hpp"

namespace cb::source::vxd::boot {

using Log = std::function<void(std::string_view)>;

// What a screen of the machine shows.
enum class Screen {
    Desktop,   // mostly teal with whatever box is on it (the logon, "Welcome", a clock message), and the taskbar once Explorer has it
    Blue,      // a fatal exception, or ScanDisk
    Text,      // the text mode: a protection error, "Press a key", DOS
    Black,     // a graphics mode with nothing on it
    Other,     // Windows' logo, a box with no desktop behind it ...
};
std::string_view screen_word(Screen screen);
Screen classify(const verify::gate16::Picture& picture);

// What QEMU's boot of a disk came to.
struct Outcome {
    std::string verdict;                // "PASS" | "FAIL(blue screen)" | "FAIL(text screen)" | "FAIL(black screen)" | "FAIL(timeout)" | "FAIL(qemu exited)"
    std::filesystem::path screenshot;
    std::string boot_log;               // the copy's BOOTLOG.TXT
};

struct Environment {
    const DdkNames& names;
    const base::Corpus& corpus;
    std::filesystem::path state;        // runs and screenshots go under <state>/vxdboot
    std::filesystem::path qemu;
    double min_free_gb = 6;             // free space a run keeps beyond the copy of the disk
    std::filesystem::path default_disk; // the setting "win95_installed_hd"
};

// The corpus original of a driver name ("vtd", "vtd.vxd", "vdhcp", "drvspacx"). UsageError when there is none.
std::filesystem::path corpus_file(const Environment& env, std::string_view name);
// Where on an installed disk a corpus driver goes (upper case, '/' between folders).
std::string guest_path(const Environment& env, const std::filesystem::path& path);
// A driver rebuilt from its recovered disassembly: the LE writer, or the bundle writer for a W3 bundle of one.
Bytes build(const Environment& env, const std::filesystem::path& path);
// The driver names inside an installed VMM32.VXD (W4 or W3).
std::vector<std::string> bundle_members(ByteView data);
// MSDOS.SYS with BootLog=1 and AutoScan=0 set under [Options] (`options` replaces them): a log of what loaded, and no ScanDisk first.
std::string boot_options(std::string_view text);
// {path on the disk: bytes} for a run: the drivers (rebuilt, or the originals with `retail`), VMM32.VXD with `vmm32` and the bundle's other
// drivers it then needs, and MSDOS.SYS with the boot options.
std::map<std::string, Bytes> plan(const Environment& env, const std::filesystem::path& disk, std::span<const std::string> drivers, bool vmm32,
                                  bool retail, const Log& log);
// Copy `disk`, write `files` into the copy, boot it. The copy is gone when this returns; the last screen and the log are kept under `tag`.
Outcome run_machine(const Environment& env, const std::filesystem::path& disk, const std::map<std::string, Bytes>& files, std::string_view tag,
                    std::chrono::seconds wait, const Log& log);
// What the boot log says of a driver file put in: its load and the phases of its start-up ("LoadSuccess SYSCRITINITSUCCESS ..."), the last
// one where it stopped; "" when the log never names it (a driver this machine does not load).
std::string loaded(std::string_view boot_log, std::string_view path);
// One run: plan, boot, report. -> the verdict.
std::string run(const Environment& env, const std::filesystem::path& disk, std::span<const std::string> drivers, bool vmm32, bool retail,
                std::string_view tag, std::chrono::seconds wait, const Log& log);
// The units (driver names, "VMM32") of a failing set that fail by themselves, found by booting halves. trial(units, tag) -> its verdict.
std::vector<std::string> bisect(std::span<const std::string> units,
                                const std::function<std::string(std::span<const std::string>, const std::string&)>& trial,
                                const std::string& tag, const Log& log);

// The command's help text.
std::string_view usage();

// `[--drivers NAME,...] [--vmm32] [--retail] [--bisect] [--disk IMAGE] [--wait SECONDS] [--tag NAME]`. Returns 0 for a pass, 1 for a fail or no
// disk, 2 for no arguments.
int main(const Environment& env, std::span<const std::string> args);

}  // namespace cb::source::vxd::boot
