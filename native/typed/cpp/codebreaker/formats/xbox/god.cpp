// codebreaker/formats/xbox/god.cpp -- see god.hpp.

#include "codebreaker/formats/xbox/god.hpp"

#include <algorithm>

#include "codebreaker/formats/xex.hpp"
#include "py_settings.hpp"

namespace codebreaker::formats::xbox::god {

const py::str DESCRIPTION("Xbox 360 GoD/SVOD: read (de-interleave) and convert ISO -> GoD");

namespace {

using gdf::native::ask;

// print("text") (py::print of a char* would print a bool)
void say(const char* utf8) { py::print(py::str(utf8)); }

// a // b for b > 0 (Python's floor division)
int64_t floordiv(int64_t a, int64_t b) {
    int64_t q = a / b;
    if ((a % b != 0) && (a < 0)) --q;
    return q;
}

}  // namespace

// ---- SvodReader ------------------------------------------------------------------------------------------------------
SvodReader::SvodReader(const py::str& data_dir) {
    auto r = ask<std::tuple<int64_t, py::list<py::str>>>("xdev.open_svod", py::bytes(), data_dir);
    _id = std::get<0>(r);
    paths = std::get<1>(r);
}

// ---- writer ----------------------------------------------------------------------------------------------------------
std::tuple<py::str, py::str, int64_t> write_god(const std::function<py::bytes(int64_t, int64_t)>& volume_read, int64_t volume_size,
                                                const py::str& out_dir, const std::optional<py::bytes>& template_,
                                                int64_t title_id, int64_t media_id, int64_t content_type, const py::str& title) {
    py::str tid = py::percent(py::str("%08X"), title_id);
    py::str mid = py::percent(py::str("%08X"), media_id);
    py::str base = py::os::path::join(py::os::path::join(out_dir, tid), py::percent(py::str("%08X"), content_type));
    py::str data_dir = py::os::path::join(base, mid + py::str(".data"));
    py::os::makedirs(data_dir, 0777, true);

    int64_t total_blocks = floordiv(volume_size + BLOCK - 1, BLOCK);
    int64_t part_count = floordiv(total_blocks + BLOCKS_PER_PART - 1, BLOCKS_PER_PART);
    std::optional<py::bytes> first_master_hash;
    int64_t parts_total_size = 0;

    for (int64_t pidx = 0; pidx < part_count; ++pidx) {
        py::str part_path = py::os::path::join(data_dir, py::percent(py::str("Data%04d"), pidx));
        int64_t first = pidx * BLOCKS_PER_PART;
        std::string blocks;
        for (int64_t gblk = first; gblk < std::min(first + BLOCKS_PER_PART, total_blocks); ++gblk)
            blocks += volume_read(gblk * BLOCK, BLOCK).ljust(BLOCK, py::bytes(std::string(1, '\0'))).raw();
        auto part = ask<std::tuple<py::bytes, py::bytes>>("god.part", py::bytes(), py::bytes(blocks));
        {
            py::file f = py::file::open(part_path, py::str("wb"));
            f.write(std::get<0>(part));
            f.close();
        }
        parts_total_size += py::os::path::getsize(part_path);
        if (pidx == 0) first_master_hash = std::get<1>(part);
    }

    py::bytes header = ask<py::bytes>("god.con_header", py::bytes(), template_, title_id, media_id, content_type, total_blocks,
                                      part_count, parts_total_size, first_master_hash, title);
    py::str con_path = py::os::path::join(base, mid);
    {
        py::file f = py::file::open(con_path, py::str("wb"));
        f.write(header);
        f.close();
    }
    return std::make_tuple(con_path, data_dir, part_count);
}

std::optional<py::bytes> _template() {
    py::str here = py::os::path::join(py::os::path::join(py::os::path::join(py::settings::checkout_root(), py::str("codebreaker")), py::str("formats")),
                                      py::str("xbox"));
    py::str p = py::os::path::join(here, py::str("empty_live.bin"));
    if (py::os::path::exists(p)) {
        py::file f = py::file::open(p, py::str("rb"));
        py::bytes data = f.read();
        f.close();
        return data;
    }
    return std::nullopt;
}

// ---- NXE-installed STFS package -> GoD/LIVE header (NXE2GOD transform) ----
py::bytes install_to_god(const py::bytes& data) { return ask<py::bytes>("god.install_to_god", data); }

bool is_nxe_install(const py::bytes& data) {
    return data.size() >= 0x348 && data.slice(0x344, 0x348) == py::bytes(std::string("\x00\x00\x40\x00", 4));
}

// ------------------------------------------------------------------------ CLI
void main(const py::list<py::str>& a) {
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help")) {
        say("usage: god {info|list} <god-dir>  |  god2iso <god.data-dir> <out.iso>  |  "
                  "iso2god <image.iso> <out-dir>  |  install2god <con-header> <out-header>");
        return;
    }
    py::str op = a[0];
    if (op == py::str("install2god")) {
        if (a.size() < 3) {
            say("usage: god install2god <installed-STFS-header> <out-GoD-header>");
            throw py::SystemExit(2);
        }
        py::file in = py::file::open(a[1], py::str("rb"));
        py::bytes data = in.read();
        if (!is_nxe_install(data)) say("warning: header content-type is not 0x00004000 (NXE-installed); converting anyway");
        py::file out = py::file::open(a[2], py::str("wb"));      // (opened before the conversion is asked for, as `open(...).write(f(data))`)
        out.write(install_to_god(data));
        out.close();
        py::print(py::percent(py::str("GoD/LIVE header written -> %s (copy the sibling .data folder alongside)"), a[2]));
        return;
    }
    if (op == py::str("info") || op == py::str("list")) {
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
    } else if (op == py::str("god2iso")) {
        if (a.size() < 3) {
            say("usage: god2iso <god.data-dir> <out.iso>");
            throw py::SystemExit(2);
        }
        py::str data_dir = a[1], out = a[2];
        SvodReader sv(data_dir);
        // total logical size = data blocks across all parts
        int64_t total = 0;
        for (const py::str& path : sv.paths) {
            int64_t phys = py::os::path::getsize(path);
            // each part holds (phys - master) / (subpart_hash + subpart_data) subparts
            int64_t body = phys - BLOCK;
            int64_t n_sub = floordiv(body, BLOCK + SUBPART_SIZE);
            int64_t rem = body - n_sub * (BLOCK + SUBPART_SIZE);
            total += n_sub * SUBPART_SIZE + std::max<int64_t>(0, rem - BLOCK);
        }
        py::file f = py::file::open(out, py::str("wb"));
        int64_t off = 0;
        while (off < total) {
            py::bytes chunk = sv.read_at(off, 8 * 1024 * 1024);
            if (chunk.empty()) break;
            f.write(chunk);
            off += chunk.size();
        }
        f.close();
        py::print(py::percent(py::str("wrote %d bytes -> %s"), off, out));
    } else if (op == py::str("iso2god") || op == py::str("convert")) {
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
    } else if (op == py::str("xex2god")) {
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
