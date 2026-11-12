// codebreaker/formats/xbox/god.hpp -- SHIM over cb/formats/xbox/god.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumers
// (xbox/gdf and xbox/volumes, which read a package through the native reader's device registry as a gdf::Device).
//
//   * SvodReader is still the old gdf::Device: its id is the native reader's (xdev.open_svod), which is what the old GDF reader
//     walks. cb::formats::xbox::god::SvodReader is the typed reader; nothing old can hold it as a Device.
//   * write_god, install_to_god, is_nxe_install and the operations of main() that need no disc image forward to the new module.
//   * COMPAT(xbox/gdf): info, list, iso2god and xex2god read and build XDVDFS volumes through the old gdf port; they stay in
//     main() until gdf is refactored (the new command() leaves them out).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/xbox/god.hpp"
#include "codebreaker/formats/xbox/gdf.hpp"
#include "codebreaker/formats/xex.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::formats::xbox::god {

namespace cbg = cb::formats::xbox::god;

inline const py::str DESCRIPTION = py::str("Xbox 360 GoD/SVOD: read (de-interleave) and convert ISO -> GoD");

inline constexpr int64_t BLOCK = 0x1000;
inline constexpr int64_t BLOCKS_PER_SUBPART = 0xCC;          // 204 data blocks per subpart
inline constexpr int64_t BLOCKS_PER_PART = 0xA1C4;           // = 0xCB * 0xCC data blocks per part
inline constexpr int64_t SUBPART_SIZE = BLOCK * BLOCKS_PER_SUBPART;

// De-interleaves a GoD `.data` directory into the logical disc volume, as the native reader's device.
class SvodReader : public gdf::Device {
  public:
    py::list<py::str> paths;          // the part files, in order
    explicit SvodReader(const py::str& data_dir) {
        auto r = gdf::native::ask<std::tuple<int64_t, py::list<py::str>>>("xdev.open_svod", py::bytes(), data_dir);
        _id = std::get<0>(r);
        paths = std::get<1>(r);
    }
};

// -> (the header file's path, the .data directory, the number of parts)
inline std::tuple<py::str, py::str, int64_t> write_god(const std::function<py::bytes(int64_t, int64_t)>& volume_read, int64_t volume_size,
                                                       const py::str& out_dir, const std::optional<py::bytes>& template_,
                                                       int64_t title_id, int64_t media_id, int64_t content_type = 0x7000,
                                                       const py::str& title = py::str("")) {
    return cb::compat::guard([&] {
        if (!template_) throw py::ValueError("write_god needs the header template");
        const cb::Bytes tpl = cb::compat::from_py(*template_);
        const cbg::WriteResult done = cbg::write_god(
            [&](std::uint64_t offset, std::size_t length) {
                return cb::compat::from_py(volume_read(static_cast<int64_t>(offset), static_cast<int64_t>(length)));
            },
            static_cast<std::uint64_t>(volume_size), cb::fs::from_text(cb::compat::from_py(out_dir)), tpl,
            cbg::Title{static_cast<std::uint32_t>(title_id), static_cast<std::uint32_t>(media_id), static_cast<std::uint32_t>(content_type),
                       cb::compat::from_py(title)});
        return std::make_tuple(cb::compat::to_py(cb::fs::to_text(done.header)), cb::compat::to_py(cb::fs::to_text(done.data_dir)),
                               static_cast<int64_t>(done.part_count));
    });
}

// The empty LIVE header template (from iso2god-rs, MIT), beside the Python sources: codebreaker/formats/xbox/ in the checkout.
inline std::optional<py::bytes> _template() {
    const py::str p = py::os::path::join(py::os::path::join(py::os::path::join(py::settings::checkout_root(), py::str("codebreaker")),
                                                            py::str("formats")), py::str("xbox"));
    const py::str file = py::os::path::join(p, py::str("empty_live.bin"));
    if (!py::os::path::exists(file)) return std::nullopt;
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(file))))); });
}

inline py::bytes install_to_god(const py::bytes& data) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cbg::install_to_god(cb::compat::from_py(data)))); });
}

inline bool is_nxe_install(const py::bytes& data) { return cbg::is_nxe_install(cb::compat::from_py(data)); }

// the command: `a` is sys.argv[1:]
inline void main(const py::list<py::str>& a) {
    auto say = [](const char* utf8) { py::print(py::str(utf8)); };
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help")) {
        say("usage: god {info|list} <god-dir>  |  god2iso <god.data-dir> <out.iso>  |  "
            "iso2god <image.iso> <out-dir>  |  install2god <con-header> <out-header>");
        return;
    }
    const py::str op = a[0];
    if (op == py::str("install2god") || op == py::str("god2iso")) {   // (forwards to the new tool; its usage errors exit 2 as the script did)
        try {
            const int code = cbg::command(cb::compat::from_py(a));
            if (code != 0) throw py::SystemExit(code);
        } catch (const cb::UsageError& e) {
            py::print(cb::compat::to_py(std::string(e.what())));
            throw py::SystemExit(2);
        } catch (const cb::Error& e) {
            cb::compat::rethrow_as_py(e);
        }
        return;
    }
    if (op == py::str("info") || op == py::str("list")) {   // COMPAT(xbox/gdf)
        py::str data_dir = a[1];
        if (!py::os::path::isdir(data_dir)) {
            say("give the .data directory of a GoD package");
            throw py::SystemExit(2);
        }
        gdf::GDF g(std::make_shared<SvodReader>(data_dir), 0);
        auto fs = g.files();
        int64_t total = 0;
        for (const auto& f : fs) total += std::get<1>(f);
        if (op == py::str("info")) {
            py::print(py::percent(py::str("GoD .data : %s"), data_dir));
            py::print(py::percent(py::str("root sector: %d  files: %d  total: %d bytes"), g.root_sector, fs.size(), total));
        } else {
            for (const auto& f : fs) py::print(py::percent(py::str("  %12d  %s"), std::get<1>(f), std::get<0>(f)));
            py::print(py::percent(py::str("total: %d files"), fs.size()));
        }
    } else if (op == py::str("iso2god") || op == py::str("convert")) {   // COMPAT(xbox/gdf)
        if (a.size() < 3) {
            say("usage: iso2god <image.iso> <out-dir> [--title NAME]");
            throw py::SystemExit(2);
        }
        py::str iso = a[1], outdir = a[2];
        py::str title("");
        if (a.contains(py::str("--title"))) title = a[a.index(py::str("--title")) + 1];
        std::optional<py::bytes> tpl = _template();
        if (!tpl) {
            say("error: empty_live.bin template not found next to god.py");
            throw py::SystemExit(1);
        }
        gdf::GDF disc(std::make_shared<gdf::FileDevice>(iso));          // locate the XDVDFS volume
        int64_t base = disc.base;
        int64_t size = py::os::path::getsize(iso) - base;
        gdf::FileDevice dev(iso);
        auto vol = [&](int64_t off, int64_t length) { return dev.read_at(base + off, length); };
        // title/media id: read from the disc's default.xex if present, else 0
        int64_t title_id = 0, media_id = 0;
        try {
            auto ids = xex::ids(disc.read(py::str("DEFAULT.XEX")));
            media_id = std::get<0>(ids);
            title_id = std::get<1>(ids);
        } catch (const py::Exception&) {
        }
        py::print(py::percent(py::str("  title_id=%08X  media_id=%08X"), title_id, media_id));
        auto done = write_god(vol, size, outdir, tpl, title_id, media_id, 0x7000, title);
        py::print(py::percent(py::str("wrote GoD: %d part(s)"), std::get<2>(done)));
        py::print(py::percent(py::str("  con header: %s"), std::get<0>(done)));
        py::print(py::percent(py::str("  data dir  : %s"), std::get<1>(done)));
    } else if (op == py::str("xex2god")) {   // COMPAT(xbox/gdf)
        if (a.size() < 3) {
            say("usage: xex2god <default.xex> <out-dir> [--title NAME]");
            throw py::SystemExit(2);
        }
        py::str xexpath = a[1], outdir = a[2];
        py::str title = a.contains(py::str("--title")) ? a[a.index(py::str("--title")) + 1] : py::str("");
        py::file in = py::file::open(xexpath, py::str("rb"));
        py::bytes data = in.read();
        auto ids = xex::ids(data);
        int64_t media_id = std::get<0>(ids), title_id = std::get<1>(ids);
        py::dict<py::str, py::bytes> one;
        one.set(py::str("default.xex"), data);
        py::bytes vol = gdf::build_xdvdfs(one);
        auto done = write_god([&](int64_t o, int64_t l) { return vol.slice(o, o + l); }, vol.size(), outdir, _template(), title_id,
                              media_id, 0x7000, title);
        py::print(py::percent(py::str("wrote GoD from XEX: title_id=%08X media_id=%08X, %d part(s)"), title_id, media_id, std::get<2>(done)));
        py::print(py::percent(py::str("  con header: %s"), std::get<0>(done)));
    } else {
        py::print(py::percent(py::str("unknown op %r"), op));
        throw py::SystemExit(2);
    }
}

}  // namespace codebreaker::formats::xbox::god
