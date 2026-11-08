// codebreaker/java/console/assets.cpp -- C++ port of codebreaker.java.console.assets (see the header for what is
// not ported: the Pillow PNG re-encode).
//
// assets.py -- extract the game's shipped assets (textures, language files, fonts, misc resources)
// out of the original Minecraft jar and lay them down in the converted project's Content folder with
// the EXACT same relative paths and names.
//
// Why exact paths matter: the port resolves a Java resource path `"/gui/gui.png"` to
// `game:\Content\gui\gui.png` (FileIO / Class::getResourceAsStream). If we drop the jar entry
// `gui/gui.png` at `Content\gui\gui.png`, the texture loader finds it with ZERO code changes -- the
// folder layout IS the contract. So this tool never renames or reorganizes; it mirrors the jar.
//
// Two jobs:
//   * extract(jar, content_dir)        -- mirror every asset entry into Content\<same-path>.
//   * coverage(jar, code_dir)          -- audit: which resource paths the CODE loads vs which the jar
//                                         provides. A path the code loads but the jar lacks becomes a
//                                         NULL image at runtime -> the loader crashes. This finds it
//                                         before the game runs, the same spirit as PLATFORM_COVERAGE.
//
// `coverage` scans either the Java corpus (.java) or the converted C++ tree (.cpp/.h) -- the resource
// path string literal `"/gui/gui.png"` is identical in both, so the audit works pre- or post-convert.

#include "codebreaker/java/console/assets.hpp"

#include <cstring>

#include "codebreaker/formats/xbox/crypto/xell_unpack.hpp"
#include "codebreaker/java/_paths.hpp"

namespace codebreaker::java::console::assets {

namespace {

// ------------------------------------------------------------------------------------------ zipfile
// A reader for the part of Python's zipfile.ZipFile the module uses: ZipFile(path), namelist(), read(name).
// Stored (0) and deflated (8) entries; zip64 and the other compression methods are not read.
struct ZipInfo {
    py::str orig_filename;     // as in the central directory (before the os.sep replacement)
    py::str filename;
    int64_t flag_bits = 0;
    int64_t compress_type = 0;
    int64_t crc = 0;
    int64_t compress_size = 0;
    int64_t file_size = 0;
    int64_t header_offset = 0;
};

int64_t u16(const std::string& d, size_t at) {
    return static_cast<unsigned char>(d[at]) | (static_cast<int64_t>(static_cast<unsigned char>(d[at + 1])) << 8);
}
int64_t u32(const std::string& d, size_t at) { return u16(d, at) | (u16(d, at + 2) << 16); }

py::str decode_name(const std::string& raw, int64_t flag_bits) {
    // zipfile: bit 11 = UTF-8, else cp437
    return py::bytes(raw).decode(flag_bits & 0x800 ? "utf-8" : "cp437");
}

class ZipFile {
    std::string data_;
    std::vector<ZipInfo> infos_;
    py::dict<py::str, int64_t> by_name_;    // NameToInfo: the last entry of a name wins

  public:
    explicit ZipFile(const py::str& path) {
        data_ = py::open_rb(path).read().raw();
        // _EndRecData: the end-of-central-directory record, in the last 64K + 22 bytes
        const char sig[4] = {'P', 'K', 5, 6};
        int64_t size = static_cast<int64_t>(data_.size());
        int64_t pos = -1;
        if (size >= 22 && std::memcmp(data_.data() + size - 22, sig, 4) == 0 && u16(data_, size - 22 + 20) == 0) {
            pos = size - 22;
        } else {
            int64_t lo = std::max<int64_t>(0, size - 65535 - 22);
            for (int64_t i = size - 22; i >= lo; --i) {
                if (std::memcmp(data_.data() + i, sig, 4) == 0) {
                    pos = i;
                    break;
                }
            }
        }
        if (pos < 0) throw BadZipFile("File is not a zip file");
        int64_t entries = u16(data_, pos + 10), size_cd = u32(data_, pos + 12), offset_cd = u32(data_, pos + 16);
        if (offset_cd == 0xFFFFFFFF || size_cd == 0xFFFFFFFF || entries == 0xFFFF) {
            throw py::NotImplementedError("zip64 archives are not read by this port");
        }
        int64_t concat = pos - size_cd - offset_cd;      // data prepended to the archive
        if (concat < 0) throw BadZipFile("Bad offset for central directory");
        offset_cd += concat;
        int64_t at = offset_cd, total = 0;
        while (total < size_cd) {
            if (at + 46 > size || std::memcmp(data_.data() + at, "PK\x01\x02", 4) != 0) {
                throw BadZipFile("Bad magic number for central directory");
            }
            ZipInfo x;
            x.flag_bits = u16(data_, at + 8);
            x.compress_type = u16(data_, at + 10);
            x.crc = u32(data_, at + 16);
            x.compress_size = u32(data_, at + 20);
            x.file_size = u32(data_, at + 24);
            int64_t fn = u16(data_, at + 28), ex = u16(data_, at + 30), cm = u16(data_, at + 32);
            x.header_offset = u32(data_, at + 42);
            if (at + 46 + fn + ex + cm > size) throw BadZipFile("Truncated central directory");
            std::string rawname = data_.substr(at + 46, fn);
            x.orig_filename = decode_name(rawname, x.flag_bits);
            py::str name = x.orig_filename;
            int64_t nul = name.find(py::str(std::u32string(1, 0)));
            if (nul >= 0) name = name.slice(0, nul);
            name = name.replace(py::os::sep, py::str("/"));    // ZipInfo: os.sep -> "/"
            x.filename = name;
            if (x.compress_size == 0xFFFFFFFF || x.file_size == 0xFFFFFFFF || x.header_offset == 0xFFFFFFFF) {
                throw py::NotImplementedError("zip64 entries are not read by this port");
            }
            x.header_offset += concat;
            by_name_[x.filename] = static_cast<int64_t>(infos_.size());
            infos_.push_back(x);
            total += 46 + fn + ex + cm;
            at += 46 + fn + ex + cm;
        }
    }

    py::list<py::str> namelist() const {
        py::list<py::str> out;
        for (const ZipInfo& x : infos_) out.append(x.filename);
        return out;
    }

    py::bytes read(const py::str& name) const {
        if (!by_name_.contains(name)) {
            throw py::KeyError(py::percent(py::str("There is no item named %r in the archive"), name).utf8());
        }
        const ZipInfo& z = infos_[static_cast<size_t>(by_name_.get(name, 0))];
        if (z.flag_bits & 0x1) {
            throw py::RuntimeError(py::percent(py::str("File %r is encrypted, password required for extraction"), z.orig_filename).utf8());
        }
        if (z.compress_type != 0 && z.compress_type != 8) {
            throw py::NotImplementedError(py::percent(py::str("That compression method is not supported (%d)"), z.compress_type).utf8());
        }
        int64_t at = z.header_offset, size = static_cast<int64_t>(data_.size());
        if (at + 30 > size || std::memcmp(data_.data() + at, "PK\x03\x04", 4) != 0) {
            throw BadZipFile("Bad magic number for file header");
        }
        int64_t fn = u16(data_, at + 26), ex = u16(data_, at + 28);
        if (at + 30 + fn > size) throw BadZipFile("Truncated file header");
        py::str fname = decode_name(data_.substr(at + 30, fn), u16(data_, at + 6));
        if (fname != z.orig_filename) {
            throw BadZipFile(py::percent(py::str("File name in directory %r and header %r differ."), z.orig_filename, fname).utf8());
        }
        int64_t start = at + 30 + fn + ex;
        if (start + z.compress_size > size) throw py::EOFError("Compressed file ended before the end-of-stream marker was reached");
        std::string body = data_.substr(start, z.compress_size);
        std::string out;
        if (z.compress_type == 0) {
            out = body;
        } else {
            // zipfile hands the entry to zlib as a raw deflate stream; the gzip reader the runtime has takes
            // the same stream with a gzip header and its CRC-32 / size trailer built from the directory entry
            std::string gz("\x1f\x8b\x08\x00\x00\x00\x00\x00\x00\xff", 10);
            gz += body;
            for (int k = 0; k < 4; ++k) gz.push_back(static_cast<char>((z.crc >> (8 * k)) & 0xFF));
            for (int k = 0; k < 4; ++k) gz.push_back(static_cast<char>((z.file_size >> (8 * k)) & 0xFF));
            try {
                out = codebreaker::formats::xbox::crypto::xell_unpack::decompress(py::bytes(gz)).raw();
            } catch (const codebreaker::formats::xbox::crypto::xell_unpack::BadGzipFile&) {
                throw BadZipFile(py::percent(py::str("Bad CRC-32 for file %r"), z.orig_filename).utf8());
            }
        }
        py::bytes result(out);
        if (z.compress_type == 0 && py::crc32(result) != z.crc) {
            throw BadZipFile(py::percent(py::str("Bad CRC-32 for file %r"), z.orig_filename).utf8());
        }
        return result;
    }
};

// name.lower().endswith(_ASSET_EXTS)
bool has_asset_ext(const py::str& name) {
    py::str low = name.lower();
    for (const py::str& e : _ASSET_EXTS) {
        if (low.endswith(e)) return true;
    }
    return false;
}

bool has_code_ext(const py::str& name) {
    py::str low = name.lower();
    for (const py::str& e : _CODE_EXTS) {
        if (low.endswith(e)) return true;
    }
    return false;
}

}  // namespace

const py::list<py::str> _ASSET_EXTS = {py::str(".png"), py::str(".jpg"), py::str(".jpeg"), py::str(".txt"),
                                       py::str(".lang"), py::str(".properties"), py::str(".fsh"), py::str(".vsh")};

const py::re::Pattern _RES_PATH = py::re::compile(py::str(R"rx("(/[\w./-]+\.(?:png|jpg|jpeg|txt|lang|properties|fsh|vsh))")rx"));

const py::list<py::str> _CODE_EXTS = {py::str(".java"), py::str(".cpp"), py::str(".h"), py::str(".hpp"), py::str(".cc"), py::str(".cxx")};

const py::re::Pattern _LINE_COMMENT = py::re::compile(py::str(R"rx(//[^\n]*)rx"));
const py::re::Pattern _BLOCK_COMMENT = py::re::compile(py::str(R"rx(/\*.*?\*/)rx"), py::re::DOTALL);

py::str _strip_comments(const py::str& text) {
    return _LINE_COMMENT.sub(py::str(""), _BLOCK_COMMENT.sub(py::str(""), text));
}

// --- PNG normalization: force to 8-bit straight-alpha RGBA (colortype 6) ------------------------
// WHY: the Xbox 360 texture loader expects 32-bit RGBA. A palettized (indexed, colortype 3) or
// grayscale PNG loaded raw is decoded as garbage/wrong colors -- the "RGB problem" from the Alpha
// port. Empirically (diffing Alpha's shipped Content vs the stock jar) the ONLY transform the Alpha
// pipeline applied was: convert every non-RGBA PNG to RGBA8888, honoring palette transparency (tRNS).
// No alpha-bleed, no channel swap, no premultiply -- those were all confirmed absent. So we replicate
// exactly that: convert colortype != 6 PNGs to RGBA; RGBA PNGs are copied byte-for-byte.

std::optional<int64_t> _png_colortype(const py::bytes& raw) {
    // PNG IHDR color type (0 gray, 2 rgb, 3 indexed, 4 gray+alpha, 6 rgba), or None if not a PNG.
    if (raw.size() < 26 || raw.slice(0, 8) != py::bytes("\x89PNG\r\n\x1a\n", 8)) {
        return std::nullopt;
    }
    return raw[25];
}

bool png_needs_rgba(const py::bytes& raw) {
    // True if this PNG is not already 8-bit straight RGBA (i.e. would misrender on the 360).
    std::optional<int64_t> ct = _png_colortype(raw);
    return ct.has_value() && *ct != 6;
}

std::tuple<py::bytes, bool> normalize_png_to_rgba(const py::bytes& raw) {
    // Return (bytes, changed). If the PNG isn't already RGBA, decode it (honoring palette tRNS) and
    // re-encode as 8-bit RGBA; otherwise return it unchanged. Requires Pillow to actually convert.
    if (!png_needs_rgba(raw)) {
        return {raw, false};
    }
    if (!_HAS_PIL) {
        return {raw, false};                              // can't convert here; run() warns + lists it
    }
    return {raw, false};     // (unreachable: _HAS_PIL is false; the Pillow re-encode is not ported)
}

py::list<py::str> list_jar_assets(const py::str& jar_path) {
    // Return the sorted list of asset entry paths (png/txt/lang/...) inside the jar, using forward
    // slashes and no leading slash -- exactly how they map under Content\.
    ZipFile z(jar_path);
    py::list<py::str> names;
    for (const py::str& n : z.namelist()) {
        if (!n.endswith(py::str("/")) && has_asset_ext(n)) {
            names.append(n);
        }
    }
    return py::sorted(names);
}

py::dict<py::str, py::list<py::str>> code_resource_refs(const py::str& code_dir) {
    // Every resource path the code loads, as a set WITHOUT the leading slash (so it lines up with
    // jar entry names). Scans .java and/or converted .cpp/.h -- the literal is the same in both.
    py::dict<py::str, py::set<py::str>> refs;
    for (const auto& walked : py::os::walk(code_dir)) {
        const py::str& dp = std::get<0>(walked);
        for (const py::str& f : std::get<2>(walked)) {
            if (!has_code_ext(f)) {
                continue;
            }
            py::str text;
            try {
                text = py::open(py::os::path::join(dp, f), py::str("r"), py::str("utf-8"), py::str("replace")).read_text();
            } catch (const py::OSError&) {                 // (IOError, OSError)
                continue;
            }
            for (const py::re::Match& m : _RES_PATH.finditer(_strip_comments(text))) {
                refs.setdefault(m.group(1).lstrip(py::str("/")), py::set<py::str>()).add(f);
            }
        }
    }
    py::dict<py::str, py::list<py::str>> out;
    for (const auto& kv : refs.items()) {
        out[std::get<0>(kv)] = py::sorted(std::get<1>(kv));
    }
    return out;
}

const py::str& _CONSOLE_ASSETS_DIR() {
    static const py::str dir = codebreaker::java::_paths::data_path({py::str("platform"), py::str("assets")});
    return dir;
}

py::list<py::str> place_console_assets(const py::str& content_dir) {
    // Mirror the bundled console-only assets into content_dir (same relative paths). Returns the list of
    // relative paths written. Skips silently if the bundle dir is absent.
    if (!py::os::path::isdir(_CONSOLE_ASSETS_DIR())) {
        return py::list<py::str>();
    }
    py::list<py::str> placed;
    for (const auto& walked : py::os::walk(_CONSOLE_ASSETS_DIR())) {
        const py::str& root = std::get<0>(walked);
        for (const py::str& fn : std::get<2>(walked)) {
            py::str src = py::os::path::join(root, fn);
            py::str rel = py::os::path::relpath(src, _CONSOLE_ASSETS_DIR());
            py::str dst = py::os::path::join(content_dir, rel);
            py::str d = py::os::path::dirname(dst);
            if (py::truthy(d) && !py::os::path::isdir(d)) {
                py::os::makedirs(d);
            }
            py::file fh = py::open(src, py::str("rb"));
            py::bytes data = fh.read();
            fh.close();
            py::file out = py::open(dst, py::str("wb"));
            out.write(data);
            out.close();
            placed.append(rel.replace(py::os::sep, py::str("/")));
        }
    }
    return py::sorted(placed);
}

py::list<py::str> extract(const py::str& jar_path, const py::str& content_dir, const std::optional<py::set<py::str>>& only,
                          bool normalize_png, const std::optional<py::list<py::str>>& converted) {
    // Mirror the jar's asset entries into content_dir, preserving each entry's relative path and
    // name (jar `gui/gui.png` -> content_dir/gui/gui.png). If `only` (a set of slash-less paths) is
    // given, extract just those; otherwise extract every asset. When normalize_png is on, any non-RGBA
    // PNG is converted to RGBA8888 on the way out (the 360 render fix). If `converted` is a list, the
    // names of PNGs that were normalized are appended to it. Returns the sorted list of paths written.
    py::list<py::str> written;
    ZipFile z(jar_path);
    for (const py::str& name : z.namelist()) {
        if (name.endswith(py::str("/")) || !has_asset_ext(name)) {
            continue;
        }
        if (only.has_value() && !only->contains(name)) {
            continue;
        }
        py::bytes data = z.read(name);
        if (normalize_png && name.lower().endswith(py::str(".png"))) {
            auto [new_data, changed] = normalize_png_to_rgba(data);
            data = new_data;
            if (changed && converted.has_value()) {
                converted->append(name);
            }
        }
        py::str rel = name.replace(py::str("/"), py::os::sep);
        py::str dst = py::os::path::join(content_dir, rel);
        py::str d = py::os::path::dirname(dst);
        if (py::truthy(d) && !py::os::path::isdir(d)) {
            py::os::makedirs(d);
        }
        py::file out = py::open(dst, py::str("wb"));
        out.write(data);
        out.close();
        written.append(name);
    }
    return py::sorted(written);
}

py::list<py::str> list_console_assets() {
    // Bundled console-only asset rel-paths (forward slash, no leading slash). Part of the coverage
    // audit's SUPPLY, since these ship from the converter rather than the jar.
    if (!py::os::path::isdir(_CONSOLE_ASSETS_DIR())) {
        return py::list<py::str>();
    }
    py::list<py::str> out;
    for (const auto& walked : py::os::walk(_CONSOLE_ASSETS_DIR())) {
        const py::str& root = std::get<0>(walked);
        for (const py::str& fn : std::get<2>(walked)) {
            py::str rel = py::os::path::relpath(py::os::path::join(root, fn), _CONSOLE_ASSETS_DIR());
            out.append(rel.replace(py::os::sep, py::str("/")));
        }
    }
    return py::sorted(out);
}

Coverage coverage(const py::str& jar_path, const py::str& code_dir) {
    // Audit the code's texture/resource demand against the jar's supply. Returns:
    //   needed   {path: [files that reference it]}   (slash-less)
    //   present  sorted[paths the code needs AND the jar has]
    //   missing  sorted[paths the code needs but the jar LACKS]  <- runtime NULL-image crash
    //   extra    sorted[asset paths in the jar the code never references]  (safe to ship anyway)
    py::dict<py::str, py::list<py::str>> needed = code_resource_refs(code_dir);
    // supply = jar + console bundle
    py::set<py::str> jar = py::set<py::str>::from(list_jar_assets(jar_path)) | py::set<py::str>::from(list_console_assets());
    py::set<py::str> jar_lower;                  // dict((n.lower(), n) for n in jar): only its keys are asked
    for (const py::str& n : jar) jar_lower.add(n.lower());
    py::set<py::str> needset = py::set<py::str>::from(needed.keys());
    py::list<py::str> present, missing;
    for (const py::str& p : py::sorted(needset)) {
        if (jar.contains(p) || jar_lower.contains(p.lower())) {   // tolerate case drift (Windows FS is case-insensitive)
            present.append(p);
        } else {
            missing.append(p);
        }
    }
    py::list<py::str> extra = py::sorted(jar - py::set<py::str>::from(present) - needset);
    Coverage cov;
    cov.needed = needed;
    cov.present = present;
    cov.missing = missing;
    cov.extra = extra;
    cov.jar_count = jar.size();
    return cov;
}

py::str render_coverage(const Coverage& cov, const py::list<py::str>& converted, const py::list<py::str>& unconverted) {
    py::list<py::str> L = {
        py::str("# TEXTURE_COVERAGE.md -- shipped-asset audit"), py::str(""),
        py::str("Resource paths the game code loads vs assets present in the jar. A **MISSING** path is one "
                "the code opens but the jar does not contain -> at runtime the loader gets a NULL image and "
                "crashes. Everything here is copied into `Content\\` with its exact jar path so the loader "
                "needs no changes."),
        py::str(""),
        py::percent(py::str("- code needs: %d   jar provides: %d   **present: %d   MISSING: %d**   (jar extras: %d)"),
                    py::len(cov.needed), cov.jar_count, py::len(cov.present), py::len(cov.missing), py::len(cov.extra)),
        py::str("")};
    auto extend = [&L](std::initializer_list<py::str> xs) {
        for (const py::str& x : xs) L.append(x);
    };
    if (py::truthy(cov.missing)) {
        extend({py::str("## MISSING -- code loads these but the jar lacks them (runtime crash)"), py::str(""),
                py::str("| Resource path | Loaded by |"), py::str("|---|---|")});
        for (const py::str& p : cov.missing) {
            py::list<py::str> files = cov.needed.get(p, py::list<py::str>());
            L.append(py::percent(py::str("| `%s` | %s |"), p, py::join(py::str(", "), files.slice(0, 4))));
        }
        L.append(py::str(""));
    } else {
        extend({py::str("> No missing textures: the jar covers every resource path the code loads."), py::str("")});
    }
    if (py::truthy(converted)) {
        extend({py::str("## PNGs normalized to RGBA (the 360 render fix)"), py::str(""),
                py::str("These were palettized/grayscale in the jar and would misrender as garbage colors if "
                        "copied raw (the 360 texture loader expects 32-bit RGBA). They were decoded to RGBA8888 "
                        "(honoring palette transparency) on extraction:"),
                py::str("")});
        for (const py::str& p : converted) {
            L.append(py::percent(py::str("- `%s`"), p));
        }
        L.append(py::str(""));
    }
    if (py::truthy(unconverted)) {
        extend({py::str("## WARNING -- non-RGBA PNGs that could NOT be converted (Pillow not installed)"), py::str(""),
                py::str("These will misrender on the 360 until converted to RGBA. Install Pillow (`pip install "
                        "Pillow`) and re-run, or convert them by hand:"),
                py::str("")});
        for (const py::str& p : unconverted) {
            L.append(py::percent(py::str("- `%s`"), p));
        }
        L.append(py::str(""));
    }
    if (py::truthy(cov.extra)) {
        extend({py::str("## Jar extras (shipped anyway, not referenced by code)"), py::str("")});
        for (const py::str& p : cov.extra) {
            L.append(py::percent(py::str("- `%s`"), p));
        }
        L.append(py::str(""));
    }
    return py::join(py::str("\n"), L) + py::str("\n");
}

py::list<py::str> indexed_pngs(const py::str& jar_path) {
    // Names of PNG entries in the jar that are NOT already RGBA (would misrender raw on the 360).
    py::list<py::str> out;
    ZipFile z(jar_path);
    for (const py::str& name : z.namelist()) {
        if (name.lower().endswith(py::str(".png")) && png_needs_rgba(z.read(name))) {
            out.append(name);
        }
    }
    return py::sorted(out);
}

RunResult run(const py::str& jar_path, const py::str& content_dir, const std::optional<py::str>& code_dir, bool only_needed,
              bool normalize_png) {
    std::optional<Coverage> cov;
    if (code_dir.has_value() && py::truthy(*code_dir)) {
        cov = coverage(jar_path, *code_dir);
    }
    std::optional<py::set<py::str>> only;
    if (only_needed && cov.has_value()) {
        only = py::set<py::str>::from(cov->present);
    }
    py::list<py::str> converted;
    py::list<py::str> need_convert = normalize_png ? indexed_pngs(jar_path) : py::list<py::str>();
    py::list<py::str> written = extract(jar_path, content_dir, only, normalize_png, converted);
    // console-only assets (controller cursor, etc.) ship from the converter's bundle, not the jar.
    py::list<py::str> console = place_console_assets(content_dir);
    // PNGs that needed conversion but couldn't be (Pillow absent) -> flag them, they will misrender.
    py::list<py::str> unconverted;
    if (normalize_png) {
        py::set<py::str> nc = py::set<py::str>::from(need_convert);
        unconverted = py::sorted(nc - py::set<py::str>::from(converted) - (nc - py::set<py::str>::from(written)));
    }
    std::optional<py::str> report_path;
    if (cov.has_value()) {
        py::str d = py::os::path::dirname(content_dir.rstrip(py::os::sep));
        report_path = py::os::path::join(py::truthy(d) ? d : py::str("."), py::str("TEXTURE_COVERAGE.md"));
        py::file f = py::open(*report_path, py::str("w"), py::str("utf-8"));
        f.write(render_coverage(*cov, converted, unconverted));
        f.close();
    }
    RunResult r;
    r.written = written;
    r.coverage = cov;
    r.content_dir = content_dir;
    r.report = report_path;
    r.converted = py::sorted(converted);
    r.unconverted = unconverted;
    r.has_pil = _HAS_PIL;
    r.console = console;
    return r;
}

}  // namespace codebreaker::java::console::assets
