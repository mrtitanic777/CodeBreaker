// codebreaker/verify/gate16/setup16.hpp -- typed C++ of codebreaker.verify.gate16.setup16.
//
// setup16.py -- does Windows 95 Setup run with these modules? The real system's test of a rebuilt module.
//
// Setup's own CD is copied with WIN95\MINI.CAB rebuilt -- the original's files, with the ones
// given in their place -- and booted in QEMU from a disk Setup has already prepared and a floppy
// that starts `SETUP /IS`. At the Welcome dialog Enter is pressed (Continue); Setup then shows
// its License Agreement page. That page on the screen is a pass; Windows back at the DOS prompt,
// an error box, or Windows' screen with nothing on it is a fail.
//
// Everything a run copies (the CD, the disk -- 650 MB) is made in a folder of its own
// (base/runs.py: state/phase3/runs/) and gone when the run ends -- a killed run's, by the next
// run once it is a day old; only the last screen of each tag is kept (state/phase3/shots/<tag>.png),
// for whoever looks at a failure.
//
//     auto [verdict, png] = continue_test({{"USER.EXE", path}, ...}, tag);
//       verdict: "PASS" | "FAIL(dos)" | "FAIL(hang)" | "FAIL(error box)" | "FAIL(timeout)"
//                | "FAIL(error box before Welcome)"
//                | "SLOW" (Setup's DOS part never finished: the run says nothing)
//
// Typing notes -- what the runtime does not have, and how it is met:
//   * pycdlib (an ISO 9660 / Joliet reader and writer) and codebreaker.formats.cab (MSZIP cabinets: read, write, extract) have no typed
//     port and no runtime counterpart: make_cd() and _replace_on_cd() are written over the interfaces `Iso` and `CabTool` (the calls the Python makes
//     of them, nothing else), handed in by the caller; continue_test() takes the make_cd as a function (`MakeCd`), which is the Python's
//     module-level make_cd by default when an `Iso` and `CabTool` are given to set_iso_cab().
//   * PIL: `Image` is an RGB picture read from a PPM (what QEMU's screendump writes: P6, 8 bits), with getpixel, size, resize (Pillow's
//     default, bicubic, bit for bit: its convolution with 22-bit fixed-point coefficients) and save (a PNG: the pixels are what PIL writes, the
//     compressed bytes are zlib's own).  A file PIL cannot open is an OSError (PIL.UnidentifiedImageError is one).
//   * socket and subprocess.Popen: written here over Winsock and CreateProcessW (Popen.poll / kill / wait are `Process`); a connection that
//     fails is OSError, a receive that times out is the Python's socket.timeout (not an error there).
//   * `_Monitor.send(*cmds)` takes the commands as a list.
//   * `continue_test`'s `keep_shot` is std::optional<str> (None: the run's results folder).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::verify::gate16::setup16 {

using RGB = std::tuple<int64_t, int64_t, int64_t>;
inline constexpr RGB LICENSE_GREY{192, 199, 200};
extern const py::list<std::tuple<int64_t, int64_t>> LICENSE_POINTS;   // ((60, 350), (580, 350), (300, 395))

// A picture of the screen: what PIL's Image.open(ppm).convert("RGB") gives.
class Image {
  public:
    Image() = default;
    // a PPM (P6) file's bytes; OSError when they are not a picture
    static Image from_ppm(const py::bytes& data);
    static Image open(const py::str& path);     // Image.open(path).convert("RGB") of a PPM file
    int64_t width() const { return w_; }
    int64_t height() const { return h_; }
    std::tuple<int64_t, int64_t> size() const { return {w_, h_}; }
    RGB getpixel(int64_t x, int64_t y) const;   // IndexError outside the picture
    Image resize(int64_t w, int64_t h) const;   // Pillow's default (bicubic)
    py::list<RGB> getdata() const;
    void save(const py::str& png) const;        // a PNG
    // for harnesses
    static Image make(int64_t w, int64_t h, const py::bytes& rgb);
    const py::bytes& rgb() const { return rgb_; }

  private:
    int64_t w_ = 0, h_ = 0;
    py::bytes rgb_;   // 3 bytes a pixel, rows from the top
};

struct Paths {
    py::str qemu, iso, original, cab, hd, floppy;
};
Paths _paths();

int64_t _free_port();

// What make_cd asks of an ISO image (pycdlib.PyCdlib) ...
struct IsoChild {
    py::bytes file_identifier;
};
class Iso {
  public:
    virtual ~Iso() = default;
    virtual void open(const py::str& path) = 0;
    virtual bool has_joliet() = 0;
    virtual void rm_file(const py::str& iso_path, const std::optional<py::str>& joliet_path) = 0;
    virtual void add_fp(const py::bytes& data, const py::str& iso_path, const std::optional<py::str>& joliet_path) = 0;
    virtual py::list<IsoChild> list_children(const py::str& iso_path) = 0;
    virtual py::bytes get_file_from_iso(const py::str& iso_path) = 0;
    virtual void write(const py::str& path) = 0;
    virtual void close() = 0;
};
// ... and of formats.cab
struct CabEntry {
    py::str name;
    int64_t folder = 0;
};
struct CabInfo {                                   // c.info: .get("prev"), .get("next"), .get("set_id", 0), .get("index", 0)
    std::optional<py::str> prev, next;
    int64_t set_id = 0, index = 0;
};
struct Cab {
    py::list<CabEntry> files;
    CabInfo info;
};
class CabTool {
  public:
    virtual ~CabTool() = default;
    virtual Cab read(const py::str& path) = 0;
    virtual void write(const py::list<std::tuple<py::str, py::bytes>>& files, const py::str& out, const py::str& compress, int64_t set_id,
                       int64_t index = 0, const std::optional<py::str>& prev = std::nullopt, const std::optional<py::str>& next = std::nullopt) = 0;
    virtual py::dict<py::str, py::str> extract(const py::str& path, const py::str& dest) = 0;   // {name: path of the extracted file}
};
using IsoFactory = std::function<std::shared_ptr<Iso>()>;
// the Iso / CabTool make_cd uses (formats::cab and pycdlib have no port: whoever has them sets them)
void set_iso_cab(const IsoFactory& iso, const std::shared_ptr<CabTool>& cab);

// Setup's CD with MINI.CAB rebuilt from the original's files, `replace` {NAME: path} in place of some.
void make_cd(const py::str& out, const py::dict<py::str, py::str>& replace);

// Put the files of `rest` {NAME: path} in place on the open CD under /WIN95: a loose file replaced, a file in a cabinet by the cabinet written again.
void _replace_on_cd(Iso& iso, CabTool& cab, const py::dict<py::str, py::str>& rest, const py::str& out);

// A running QEMU (subprocess.Popen).
class Process {
  public:
    Process() = default;
    ~Process();
    Process(const Process&) = delete;
    Process& operator=(const Process&) = delete;
    // Popen(args, stdout=DEVNULL, stderr=DEVNULL); OSError when it cannot start
    explicit Process(const py::list<py::str>& args);
    std::optional<int64_t> poll();       // None: still running
    void kill();
    void wait(double timeout);           // TimeoutExpired is the caller's: false when the time ran out
    bool wait_timeout(double timeout);

  private:
    void* handle_ = nullptr;
};

class _Monitor {
  public:
    explicit _Monitor(int64_t port) : port(port) {}
    int64_t port;
    // connect, read what the monitor says first (2 s at most), send each command and wait 0.3 s after it
    void send(const py::list<py::str>& cmds);
    // screendump into a PPM beside `png`, 1.2 s later read it, save it as the PNG and remove the PPM
    Image shot(const py::str& png);
};

bool _license(const Image& img);
bool _error_box(const Image& img);
bool _welcome(const Image& img);
// the number of bright pixels of the small picture _welcome counts (what it compares with 2000)
int64_t _welcome_count(const Image& img);
bool _black(const Image& img);

using MakeCd = std::function<void(const py::str& out, const py::dict<py::str, py::str>& replace)>;

// Boot Setup with `replace` {NAME: path} and press Continue. -> (verdict, screenshot path)
std::tuple<py::str, py::str> continue_test(const py::dict<py::str, py::str>& replace, const py::str& tag, int64_t wait = 150,
                                           const std::optional<py::str>& keep_shot = std::nullopt, const MakeCd& make_cd_fn = nullptr);

}  // namespace codebreaker::verify::gate16::setup16
