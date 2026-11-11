// codebreaker/verify/gate16/setup16.hpp -- COMPAT SHIM over cb/verify/gate16/setup16.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/gate16/setup16.{hpp,cpp} now (does Windows 95 Setup run with these modules? Setup's CD
// copied with MINI.CAB rebuilt, booted in QEMU, Enter pressed at the Welcome dialog; the License page on the screen is a
// pass). This header keeps the old py:: API: the screen recognisers over `Image`, `_Monitor`, `Process`, `_paths()` (the
// settings, read as before), `make_cd` and `continue_test`, converting to and from the new module. No logic of its own.
//
// The old `Iso` and `CabTool` interfaces (which stood in for pycdlib and formats.cab, neither of which had a port) are
// kept so that callers compile, and are not used: the new make_cd reads and writes the CD and the cabinets itself.
// `set_iso_cab` accepts and ignores what it is given.
#pragma once

#include <cstdint>
#include <chrono>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/core/process.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/verify/gate16/setup16.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::verify::gate16::setup16 {

using RGB = std::tuple<int64_t, int64_t, int64_t>;
inline constexpr RGB LICENSE_GREY{192, 199, 200};
inline const py::list<std::tuple<int64_t, int64_t>> LICENSE_POINTS = {{60, 350}, {580, 350}, {300, 395}};

// A picture of the screen: what PIL's Image.open(ppm).convert("RGB") gives.
class Image {
  public:
    Image() = default;
    // a PPM (P6) file's bytes; OSError when they are not a picture
    static Image from_ppm(const py::bytes& data) {
        auto p = cb::verify::gate16::Picture::try_from_ppm(cb::compat::from_py(data));
        if (!p) throw py::OSError("cannot identify image file");
        return Image(std::move(*p));
    }
    static Image open(const py::str& path) {
        py::file f = py::open(path, py::str("rb"));
        py::bytes data = f.read();
        f.close();
        return from_ppm(data);
    }
    int64_t width() const { return pic_.width(); }
    int64_t height() const { return pic_.height(); }
    std::tuple<int64_t, int64_t> size() const { return {width(), height()}; }
    RGB getpixel(int64_t x, int64_t y) const {   // IndexError outside the picture
        const auto p = x >= 0 && y >= 0 ? pic_.at(static_cast<std::uint32_t>(x), static_cast<std::uint32_t>(y)) : std::nullopt;
        if (!p) throw py::IndexError("image index out of range");
        return {p->r, p->g, p->b};
    }
    Image resize(int64_t w, int64_t h) const { return Image(pic_.resized(static_cast<std::uint32_t>(w), static_cast<std::uint32_t>(h))); }
    py::list<RGB> getdata() const {
        py::list<RGB> out;
        const cb::Bytes& px = pic_.rgb();
        for (std::size_t i = 0; i + 2 < px.size(); i += 3) out.append(RGB{px[i], px[i + 1], px[i + 2]});
        return out;
    }
    void save(const py::str& png) const {   // a PNG
        const cb::Bytes data = pic_.to_png();
        py::file f = py::open(png, py::str("wb"));
        f.write(cb::compat::to_py(cb::ByteView(data)));
        f.close();
    }
    // for harnesses
    static Image make(int64_t w, int64_t h, const py::bytes& rgb) {
        return Image(cb::verify::gate16::Picture(static_cast<std::uint32_t>(w), static_cast<std::uint32_t>(h), cb::compat::from_py(rgb)));
    }
    py::bytes rgb() const { return cb::compat::to_py(cb::ByteView(pic_.rgb())); }
    const cb::verify::gate16::Picture& picture() const { return pic_; }

  private:
    explicit Image(cb::verify::gate16::Picture p) : pic_(std::move(p)) {}
    cb::verify::gate16::Picture pic_;
};

struct Paths {
    py::str qemu, iso, original, cab, hd, floppy;
};
inline Paths _paths() {
    const py::str mini = py::os::path::join(py::settings::get_str(py::str("state")), py::str("mini"));
    py::os::makedirs(py::os::path::dirname(mini), 0777, true);   // (settings.state makes the folder of the path it joins, not the path itself)
    Paths p;
    const py::str q = py::settings::get_str(py::str("qemu"));
    p.qemu = q.empty() ? py::str("C:\\Program Files\\qemu\\qemu-system-i386.exe") : q;
    p.iso = py::settings::get_str(py::str("win95_setup_iso"));
    p.original = py::os::path::join(mini, py::str("original"));
    p.cab = py::os::path::join(mini, py::str("MINI.CAB"));
    p.hd = py::os::path::join(mini, py::str("prepared_hd.img"));
    const py::str floppy = py::os::path::join(py::settings::get_str(py::str("state")), py::str("bootfloppy"), py::str("boot95_is.img"));
    py::os::makedirs(py::os::path::dirname(floppy), 0777, true);
    p.floppy = floppy;
    return p;
}

inline int64_t _free_port() { return cb::compat::guard([] { return static_cast<int64_t>(cb::verify::gate16::setup16::free_port()); }); }

// What the old make_cd asked of an ISO image (pycdlib.PyCdlib) and of formats.cab: kept for callers that compile against
// them; unused.
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
struct CabEntry {
    py::str name;
    int64_t folder = 0;
};
struct CabInfo {
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
    virtual py::dict<py::str, py::str> extract(const py::str& path, const py::str& dest) = 0;
};
using IsoFactory = std::function<std::shared_ptr<Iso>()>;
inline void set_iso_cab(const IsoFactory&, const std::shared_ptr<CabTool>&) {}   // (the new make_cd needs neither)

namespace detail {
inline cb::verify::gate16::setup16::Config config() {
    const Paths p = _paths();
    cb::verify::gate16::setup16::Config c;
    c.state = py::settings::get_str(py::str("state")).utf8();
    c.qemu = p.qemu.utf8();
    c.setup_iso = p.iso.utf8();
    c.mini_dir = std::filesystem::path(p.cab.utf8()).parent_path();
    c.boot_floppy = p.floppy.utf8();
    return c;
}
inline cb::verify::gate16::setup16::Replace replace_of(const py::dict<py::str, py::str>& replace) {
    cb::verify::gate16::setup16::Replace out;
    for (const auto& kv : replace.items()) out[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
    return out;
}
}  // namespace detail

// Setup's CD with MINI.CAB rebuilt from the original's files, `replace` {NAME: path} in place of some.
inline void make_cd(const py::str& out, const py::dict<py::str, py::str>& replace) {
    cb::compat::guard([&] { cb::verify::gate16::setup16::make_cd(detail::config(), out.utf8(), detail::replace_of(replace)); });
}

// A running QEMU (subprocess.Popen).
class Process {
  public:
    Process() = default;
    // Popen(args, stdout=DEVNULL, stderr=DEVNULL); OSError when it cannot start
    explicit Process(const py::list<py::str>& args) {
        cb::process::ProcessSpec spec;
        for (const py::str& a : args) spec.argv.push_back(a.utf8());
        spec.out = cb::process::Capture::Discard;
        spec.err = cb::process::Capture::Discard;
        try {
            p_ = std::make_unique<cb::process::Process>(cb::process::start(spec));
        } catch (const cb::Error& e) {
            throw py::FileNotFoundError(e.what());
        }
    }
    std::optional<int64_t> poll() {   // None: still running
        if (!p_ || p_->running()) return std::nullopt;
        const auto code = p_->wait(std::chrono::milliseconds(0));
        return code ? std::optional<int64_t>(*code) : std::nullopt;
    }
    void kill() { if (p_) p_->kill(); }
    void wait(double timeout) { wait_timeout(timeout); }
    bool wait_timeout(double timeout) { return !p_ || p_->wait(std::chrono::milliseconds(static_cast<int64_t>(timeout * 1000))).has_value(); }

  private:
    std::unique_ptr<cb::process::Process> p_;
};

class _Monitor {
  public:
    explicit _Monitor(int64_t port) : port(port) {}
    int64_t port;
    // connect, read what the monitor says first (2 s at most), send each command and wait 0.3 s after it
    void send(const py::list<py::str>& cmds) {
        std::vector<std::string> commands;
        for (const py::str& c : cmds) commands.push_back(c.utf8());
        if (!cb::verify::gate16::setup16::Monitor(static_cast<std::uint16_t>(port)).send(commands))
            throw py::OSError("the monitor cannot be reached");
    }
    // screendump into a PPM beside `png`, 1.2 s later read it, save it as the PNG and remove the PPM
    Image shot(const py::str& png) {
        const auto p = cb::verify::gate16::setup16::Monitor(static_cast<std::uint16_t>(port)).shot(png.utf8());
        if (!p) throw py::OSError("no screen came from the monitor");
        return Image::make(p->width(), p->height(), cb::compat::to_py(cb::ByteView(p->rgb())));
    }
};

inline bool _license(const Image& img) { return cb::verify::gate16::setup16::is_license(img.picture()); }
inline bool _error_box(const Image& img) { return cb::verify::gate16::setup16::is_error_box(img.picture()); }
inline bool _welcome(const Image& img) { return cb::verify::gate16::setup16::is_welcome(img.picture()); }
// the number of bright pixels of the small picture _welcome counts (what it compares with 2000)
inline int64_t _welcome_count(const Image& img) { return cb::verify::gate16::setup16::white_pixels(img.picture()); }
inline bool _black(const Image& img) { return cb::verify::gate16::setup16::is_black(img.picture()); }

using MakeCd = std::function<void(const py::str& out, const py::dict<py::str, py::str>& replace)>;

// Boot Setup with `replace` {NAME: path} and press Continue. -> (verdict, screenshot path)
inline std::tuple<py::str, py::str> continue_test(const py::dict<py::str, py::str>& replace, const py::str& tag, int64_t wait = 150,
                                                  const std::optional<py::str>& keep_shot = std::nullopt, const MakeCd& make_cd_fn = nullptr) {
    return cb::compat::guard([&] {
        namespace nv = cb::verify::gate16::setup16;
        nv::Options options;
        options.timing.wait = std::chrono::seconds(wait);
        if (keep_shot && !keep_shot->empty()) options.keep_shot = std::filesystem::path(keep_shot->utf8());
        if (make_cd_fn) {
            options.make_cd = [make_cd_fn](const std::filesystem::path& out, const nv::Replace& r) {
                py::dict<py::str, py::str> d;
                for (const auto& [name, path] : r) d.set(cb::compat::to_py(name), cb::compat::to_py(path.generic_string()));
                make_cd_fn(cb::compat::to_py(out.generic_string()), d);
            };
        }
        const nv::Outcome o = nv::continue_test(detail::config(), detail::replace_of(replace), tag.utf8(), options);
        return std::tuple<py::str, py::str>{cb::compat::to_py(nv::word(o.verdict)), cb::compat::to_py(o.screenshot.generic_string())};
    });
}

}  // namespace codebreaker::verify::gate16::setup16
