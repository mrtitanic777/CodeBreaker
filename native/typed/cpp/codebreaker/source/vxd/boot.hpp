// codebreaker/source/vxd/boot.hpp -- typed C++ of codebreaker.source.vxd.boot.
//
// boot.py -- does Windows 95 start with these drivers? A rebuilt driver's real test.
//
//     python -m codebreaker vxd boot [--drivers NAME,...] [--vmm32] [--retail] [--bisect]
//                                    [--disk IMAGE] [--wait SECONDS] [--tag NAME]
//
// (what the module says of itself is DOC below, as the Python's `__doc__` prints it for -h)
//
// Typing and runtime notes:
//   * A PIL image (the screen: classify() asks it its size, converts it to RGB, resizes it with NEAREST, crops it, takes the pixel at a
//     point) is the struct Image: 8-bit RGB, row by row. The screens QEMU writes are PPM files (P6), read into it; an image PIL would
//     hold in another mode is not met (the PPM is RGB, and the Python converts to RGB first thing).
//   * The Python's setup16._free_port / _Monitor (verify.gate16.setup16, which has no C++ port yet) are Winsock calls here; the monitor's
//     screendump is read as a PPM and saved as a PNG (a PNG writer over py::zlib stands for PIL's img.save(png)).
//   * QEMU is started with CreateProcessW (the Python's subprocess.Popen: no Popen in the runtime) at below-normal priority, its
//     output to NUL; poll / kill / wait are GetExitCodeProcess / TerminateProcess / WaitForSingleObject.
//   * `log` (the Python's `print` by default) is a std::function taking the line; an empty one prints to std::cout.
//   * BOOT_OPTIONS is an ordered list of (key, value): the Python's dict, whose items() order is the order the lines are put in.
//   * boot()'s (verdict, png path, bootlog text) is the struct BootResult.
//   * main(argv, out, err): argv is sys.argv[1:]; print() goes to `out`; the return is the exit status.
#pragma once

#include <cstdint>
#include <functional>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::vxd::boot {

extern const py::str DEFAULT_DISK;
extern const py::str DOC;

using Log = std::function<void(const py::str&)>;
using Pixel = std::tuple<int64_t, int64_t, int64_t>;

// the GREYS of the taskbar: the palette's grey, and on QEMU's VGA
extern const py::list<Pixel> GREYS;

// -- what goes where
// The corpus original of a driver name ("vtd", "vtd.vxd", "vdhcp", "drvspacx") -> its path. ValueError when the corpus has none.
py::str corpus_file(const py::str& name);

// Where on an installed disk a corpus driver goes: WINDOWS/SYSTEM/x.vxd into VMM32/, any other under the corpus' own folder.
py::str guest_path(const py::str& path);

// A driver rebuilt from its recovered disassembly: build_le, or build_w3 for a W3 bundle.
py::bytes build(const py::str& path);

py::bytes _original(const py::str& path);

// The driver names inside an installed VMM32.VXD (W4 or W3).
py::list<py::str> _bundle_members(const py::bytes& data);

extern const py::list<std::tuple<py::str, py::str>> BOOT_OPTIONS;   // BootLog=1: C:\BOOTLOG.TXT; AutoScan=0: no ScanDisk first

// MSDOS.SYS with `options` (default BOOT_OPTIONS) set under [Options].
py::str boot_options(const py::str& text, const std::optional<py::list<std::tuple<py::str, py::str>>>& options = std::nullopt);

// {path on the disk: bytes} for a run: the drivers (rebuilt, or the originals with `retail`), VMM32.VXD with `vmm32` and the bundle's other
// drivers it then needs, MSDOS.SYS with BOOT_OPTIONS.
py::dict<py::str, py::bytes> plan(const py::str& disk, const py::list<py::str>& drivers = py::list<py::str>(), bool vmm32 = false, bool retail = false,
                                  const Log& log = Log());

// -- the machine
struct Image {
    int64_t width = 0, height = 0;
    std::string rgb;   // width * height * 3 bytes, row by row
};

Image read_ppm(const py::bytes& data);          // PIL's Image.open of a P6 file (ValueError when it is none)
py::bytes png_bytes(const Image& img);          // img.save(png): a PNG of the image
Image resize_nearest(const Image& img, int64_t w, int64_t h);   // img.resize((w, h), 0)
Image crop(const Image& img, int64_t left, int64_t top, int64_t right, int64_t bottom);
Pixel getpixel(const Image& img, int64_t x, int64_t y);

// Windows 95's desktop colour: (0, 128, 128) in the palette, (87, 168, 168) on QEMU's VGA.
bool _teal(const Pixel& p);

py::list<Pixel> _pixels(const Image& img);

// What a screen shows: "desktop", "blue" (a fatal exception, or ScanDisk), "text" (the text mode: a protection error, "Press a key", DOS),
// "black" (a graphics mode with nothing on it), "other" (Windows' logo, a box with no desktop behind it ...).
py::str classify(const Image& img);

struct BootResult {
    py::str verdict, png, bootlog;
};

// Copy `disk`, write `files` into the copy, boot it. verdict: "PASS" | "FAIL(blue screen)" | "FAIL(text screen)" | "FAIL(black screen)" |
// "FAIL(timeout)" | "FAIL(qemu exited)"
BootResult boot(const py::str& disk, const py::dict<py::str, py::bytes>& files, const py::str& tag, double wait = 300, const Log& log = Log());

py::str _read(const py::str& image, const py::str& name);

// What BOOTLOG.TXT says of a driver file put in: its load and the phases of its start-up ("LoadSuccess SYSCRITINITSUCCESS DEVICEINITSUCCESS
// ..."), the last one where it stopped; "" when the log never names it (not a driver this machine loads).
py::str loaded(const py::str& bootlog, const py::str& path);

py::str run(const py::str& disk, const py::list<py::str>& drivers, bool vmm32, bool retail, const py::str& tag, double wait, const Log& log = Log());

// The units (driver names, "VMM32") of a failing set that fail by themselves, found by booting halves: trial(units, tag) -> verdict.
py::list<py::str> bisect(const py::list<py::str>& units, const std::function<py::str(const py::list<py::str>&, const py::str&)>& trial, const py::str& tag,
                         const Log& log = Log());

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err);

}  // namespace codebreaker::source::vxd::boot
