// codebreaker/formats/xbox/stfs.hpp -- compat shim over cb/formats/xbox/stfs.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/xbox/stfs.{hpp,cpp}; this header keeps the old py:: surface for the ports that
// have not been refactored yet (volumes, system_ui) and is deleted with the last of them. Conversion and forwarding only,
// except `main`, the old command-line wrapper over the same functions. The helpers the old class used internally (the
// block arithmetic `_cbdbn` ..., `_WriteGeom`, `_header_hash`, `_bn`, `_con_sign`, `_con_verify`) are not carried over: they
// are private to cb::formats::xbox::stfs now.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/xbox/stfs.hpp"
#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::stfs {

using BigInt = codebreaker::formats::xbox::crypto::MemCrypto::BigInt;
using SystemExit = codebreaker::formats::xbox::crypto::MemCrypto::SystemExit;

inline const py::str DESCRIPTION = py::str("read/resign Xbox 360 STFS packages (CON/LIVE/PIRS)");

inline constexpr int64_t BLOCK = 0x1000;
inline constexpr int64_t KV_CERT_OFF = 0x9C8;
inline constexpr int64_t KV_CERT_LEN = 0x1A8;
inline constexpr int64_t KV_PRV_OFF = 0x298;

// MAGICS = (b"CON ", b"LIVE", b"PIRS")
inline bool is_magic(const py::bytes& four) { return cb::formats::xbox::stfs::magic_of(cb::compat::from_py(four)).has_value(); }

class STFS {
public:
    py::bytes d;
    py::bytes magic;

    int64_t header_size = 0;
    int64_t first_hash = 0;

    // STFS volume descriptor
    int64_t block_sep = 0;
    int64_t ft_block_count = 0;
    int64_t ft_block_number = 0;
    int64_t alloc_blocks = 0;
    int64_t unalloc_blocks = 0;

    int64_t sex = 0;               // 0 = female, 1 = male
    int64_t top_level = 0;

    // content metadata
    int64_t content_type = 0;
    py::str title_id;
    py::str profile_id;
    py::str display_name;
    py::str title_name;

    py::dict<py::str, std::tuple<int64_t, int64_t, int64_t>> files;   // {path: (start block, blocks, size)}

    explicit STFS(py::bytes data) : d(std::move(data)) {
        pkg_ = std::make_shared<cb::formats::xbox::stfs::Package>(cb::compat::guard([&] { return cb::formats::xbox::stfs::Package::parse(cb::compat::from_py(d)); }));
        const cb::formats::xbox::stfs::Package& p = *pkg_;
        magic = cb::compat::to_py(cb::as_bytes(cb::formats::xbox::stfs::to_string(p.magic())));
        header_size = p.header_size();
        first_hash = p.first_hash_offset();
        block_sep = p.block_separation();
        ft_block_count = p.file_table_block_count();
        ft_block_number = p.file_table_block_number();
        alloc_blocks = p.allocated_blocks();
        unalloc_blocks = p.unallocated_blocks();
        sex = p.male_layout() ? 1 : 0;
        top_level = p.top_hash_level();
        content_type = p.content_type();
        title_id = cb::compat::to_py(p.title_id());
        profile_id = cb::compat::to_py(p.profile_id());
        display_name = cb::compat::to_py(p.display_name());
        title_name = cb::compat::to_py(p.title_name());
        for (const cb::formats::xbox::stfs::FileEntry& f : p.files()) {
            files.set(cb::compat::to_py(f.path), std::make_tuple(int64_t(f.start_block), int64_t(f.block_count), int64_t(f.size)));
        }
    }

    py::bytes read_file(const py::str& name) {
        return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(pkg_->read_file(cb::compat::from_py(name)))); });
    }
    int64_t extract_all(const py::str& outdir) {
        return cb::compat::guard([&] { return int64_t(pkg_->extract_all(cb::fs::from_text(cb::compat::from_py(outdir)))); });
    }

private:
    std::shared_ptr<cb::formats::xbox::stfs::Package> pkg_;
};

// Pack a FOLDER of files into a loadable STFS package, cloning the header skeleton (console cert +
// metadata layout) from an existing `template` package and rebuilding the payload, hash tree, file table
// and volume descriptor. The package is rehashed here but keeps the template's console signature.
inline py::str build(const py::str& folder, const py::str& out_path, const py::str& template_path) {
    return cb::compat::guard([&] {
        cb::formats::xbox::stfs::build(cb::fs::from_text(cb::compat::from_py(folder)), cb::fs::from_text(cb::compat::from_py(out_path)),
                                       cb::fs::from_text(cb::compat::from_py(template_path)));
        return out_path;
    });
}
inline py::str build(const py::str& folder, const py::str& out_path, const py::bytes& template_bytes) {
    return cb::compat::guard([&] {
        // the folder's files, packed over the template bytes
        const std::filesystem::path dir = cb::fs::from_text(cb::compat::from_py(folder));
        std::vector<std::pair<std::string, cb::Bytes>> inner;
        if (std::filesystem::is_directory(dir)) {
            std::vector<std::filesystem::path> names;
            for (const auto& e : std::filesystem::directory_iterator(dir)) {
                if (e.is_regular_file()) names.push_back(e.path());
            }
            std::sort(names.begin(), names.end(), [](const auto& a, const auto& b) { return cb::fs::to_text(a.filename()) < cb::fs::to_text(b.filename()); });
            for (const auto& p : names) inner.emplace_back(cb::fs::to_text(p.filename()), cb::fs::read_file(p));
        } else {
            inner.emplace_back(cb::fs::to_text(dir.filename()), cb::fs::read_file(dir));
        }
        std::vector<cb::formats::xbox::stfs::SourceFile> sources;
        for (const auto& [n, data] : inner) sources.push_back({n, data});
        cb::fs::write_file(cb::fs::from_text(cb::compat::from_py(out_path)), cb::formats::xbox::stfs::build(sources, cb::compat::from_py(template_bytes)));
        return out_path;
    });
}

// Re-sign a package with a console keyvault you own so it will load on that console (CON as is; LIVE / PIRS converted to
// CON). `package` and `kv` are bytes or a path; the result goes to `out_path`, else back over `package` when that is a path.
inline py::bytes resign(const std::variant<py::bytes, py::str>& package, const std::variant<py::bytes, py::str>& kv,
                        const std::optional<py::str>& out_path = std::nullopt, bool bind_console = false) {
    return cb::compat::guard([&] {
        auto bytes_of = [](const std::variant<py::bytes, py::str>& v) {
            if (const py::bytes* b = std::get_if<py::bytes>(&v)) return cb::compat::from_py(*b);
            return cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(std::get<py::str>(v))));
        };
        cb::formats::xbox::stfs::ResignOptions o;
        o.bind_console = bind_console;
        const cb::Bytes made = cb::formats::xbox::stfs::resign(bytes_of(package), bytes_of(kv), o);
        const py::bytes out = cb::compat::to_py(cb::ByteView(made));
        if (out_path) {
            cb::fs::write_file(cb::fs::from_text(cb::compat::from_py(*out_path)), made);
        } else if (const py::str* path = std::get_if<py::str>(&package)) {
            cb::fs::write_file(cb::fs::from_text(cb::compat::from_py(*path)), made);
        }
        return out;
    });
}

// ---- CLI --------------------------------------------------------------------------------------------
// `argv` is sys.argv[1:]; sys.exit(n) is a SystemExit.
inline void main(const py::list<py::str>& argv) {
    auto read_all = [](const py::str& p) {
        return cb::compat::guard([&] { return cb::compat::to_py(cb::ByteView(cb::fs::read_file(cb::fs::from_text(cb::compat::from_py(p))))); });
    };
    std::vector<py::str> a;
    for (int64_t i = 0; i < argv.size(); ++i) a.push_back(argv[i]);
    auto say = [](const std::string& line) { py::print(py::str(line)); };
    auto u8 = [](const py::str& s) { return s.utf8(); };
    if (a.empty() || u8(a[0]) == "-h" || u8(a[0]) == "--help") {
        say("usage: stfs {info|list|extract} <package> [outdir]");
        say("       stfs pack   <folder> <out.stfs> <template>");
        say("       stfs resign <package> <KV_dec.bin> [out] [--bind]");
        say("              resigns CON, and converts LIVE/PIRS -> CON");
        say("              --bind also stamps the console id into the metadata");
        return;
    }
    const std::string op = u8(a[0]);
    if (op == "pack") {
        if (a.size() < 4) {
            say("usage: stfs pack <folder> <out.stfs> <template-package>");
            throw SystemExit(2);
        }
        const py::str out = build(a[1], a[2], a[3]);
        const STFS r(read_all(out));
        say("packed " + std::to_string(r.files.size()) + " files -> " + u8(out) + "  (reads back: " + std::to_string(r.files.size()) + " files)");
        return;
    }
    if (op == "resign") {
        bool bind = false;
        std::vector<py::str> b;
        for (const auto& x : a) {
            if (u8(x) == "--bind") bind = true;
            else b.push_back(x);
        }
        if (b.size() < 3) {
            say("usage: stfs resign <package> <KV_dec.bin> [out] [--bind]");
            throw SystemExit(2);
        }
        const py::str out = b.size() > 3 ? b[3] : b[1];
        const py::bytes src_magic = read_all(b[1]).slice(0, 4);
        resign(std::variant<py::bytes, py::str>(b[1]), std::variant<py::bytes, py::str>(b[2]), out, bind);
        const std::string cid = cb::formats::xbox::stfs::console_id(cb::compat::from_py(read_all(b[2])));
        say("resigned " + u8(py::os::path::basename(b[1])));
        if (src_magic != py::bytes("CON ")) say("  converted  : " + src_magic.decode("latin-1").utf8() + " -> CON");
        say("  console id : " + cid + "  (from " + u8(py::os::path::basename(b[2])) + ")" + (bind ? "  [bound @0x36C]" : ""));
        say("  signature  : verified against console certificate");
        say("  written    : " + u8(out));
        return;
    }
    if (a.size() < 2) {
        say("usage: stfs " + op + " <package> [args]");
        throw SystemExit(2);
    }
    STFS s(read_all(a[1]));
    if (op == "info") {
        say("package     : " + u8(py::os::path::basename(a[1])));
        say("magic       : " + s.magic.decode("latin-1").utf8());
        say("title       : " + u8(py::repr(s.title_name)));
        say("display name: " + u8(py::repr(s.display_name)));
        say("title id    : " + u8(s.title_id) + "   content type: 0x" + py::percent(py::str("%X"), s.content_type).utf8());
        say("profile id  : " + u8(s.profile_id));
        say("layout      : " + std::string(s.sex == 0 ? "female" : "male") + "  top-level=" + std::to_string(s.top_level) + "  alloc_blocks=" + std::to_string(s.alloc_blocks));
        say("files       : " + std::to_string(s.files.size()));
    } else if (op == "list") {
        py::list<py::str> names = py::sorted(py::list<py::str>::from(s.files.keys()));
        int64_t total = 0;
        for (int64_t i = 0; i < names.size(); ++i) {
            const std::string num = std::to_string(std::get<2>(s.files.at(names[i])));
            say("  " + std::string(num.size() < 10 ? 10 - num.size() : 0, ' ') + num + "  " + u8(names[i]));
        }
        for (const auto& v : s.files.values()) total += std::get<2>(v);
        say("total: " + std::to_string(s.files.size()) + " files, " + std::to_string(total) + " bytes");
    } else if (op == "extract") {
        const py::str outdir = a.size() > 2 ? a[2] : py::str(u8(std::get<0>(py::os::path::splitext(a[1]))) + "_extracted");
        say("extracted " + std::to_string(s.extract_all(outdir)) + " files -> " + u8(outdir));
    } else {
        say("unknown op " + u8(py::repr(a[0])) + " (info|list|extract)");
        throw SystemExit(2);
    }
}

}  // namespace codebreaker::formats::xbox::stfs
