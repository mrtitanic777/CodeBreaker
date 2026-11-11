// codebreaker/build/disc_image.cpp -- C++ from codebreaker.build.disc_image (see disc_image.hpp).

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>   // (GetFileAttributesExW: os.stat's size and time in one query)

#include "codebreaker/build/disc_image.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <map>
#include <memory>

#include "codebreaker/formats/media.hpp"
#include "py_os.hpp"

namespace codebreaker::build::disc_image {

const int64_t SECTOR = 2048;

namespace {

namespace fs = std::filesystem;
using Bytes = std::string;   // raw bytes while an image is being laid out

const char* const ALLOWED = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

fs::path as_path(const py::str& s) { return fs::u8path(s.utf8()); }

void put16le(Bytes& out, uint64_t v) {
    out.push_back(static_cast<char>(v & 0xFF));
    out.push_back(static_cast<char>((v >> 8) & 0xFF));
}
void put16be(Bytes& out, uint64_t v) {
    out.push_back(static_cast<char>((v >> 8) & 0xFF));
    out.push_back(static_cast<char>(v & 0xFF));
}
void put32le(Bytes& out, uint64_t v) {
    put16le(out, v & 0xFFFF);
    put16le(out, (v >> 16) & 0xFFFF);
}
void put32be(Bytes& out, uint64_t v) {
    put16be(out, (v >> 16) & 0xFFFF);
    put16be(out, v & 0xFFFF);
}

// bytearray slice assignment of equal length: buf[at:at + len(data)] = data
void assign(Bytes& buf, size_t at, const Bytes& data) { buf.replace(at, data.size(), data); }

// A file or folder of the tree to write.
struct Node {
    py::str source;
    std::tuple<py::str, py::str> key;
    py::str ident_text;
    Bytes ident;
    bool is_dir = false;
    std::vector<std::shared_ptr<Node>> children;
    Node* parent = nullptr;
    int64_t number = 0;
    int64_t lba = 0;
    int64_t size = 0;
    Bytes stamp;
};
using NodeP = std::shared_ptr<Node>;

NodeP make_node(const py::str& source, const std::tuple<py::str, py::str>& key, bool is_dir, Node* parent) {
    auto n = std::make_shared<Node>();
    n->source = source;
    n->key = key;
    n->is_dir = is_dir;
    n->parent = parent;
    // a file's identifier always has the dot and a version number: "README.;1"
    py::str ident = is_dir ? std::get<0>(key) : py::percent(py::str("%s.%s;1"), std::get<0>(key), std::get<1>(key));
    n->ident = ident.encode("ascii").raw();
    // os.stat(source): its size and modification time (one query of the file's attributes)
    WIN32_FILE_ATTRIBUTE_DATA attrs{};
    std::wstring wide = as_path(source).wstring();
    if (!GetFileAttributesExW(wide.c_str(), GetFileExInfoStandard, &attrs)) {
        py::os::stat(source);   // (raises what os.stat raises)
    }
    int64_t file_size = (static_cast<int64_t>(attrs.nFileSizeHigh) << 32) | attrs.nFileSizeLow;
    n->size = is_dir ? 0 : file_size;
    // time.gmtime(info.st_mtime): the whole seconds, rounded down (st_mtime counts from 1970, the file time from 1601)
    int64_t ticks = (static_cast<int64_t>(attrs.ftLastWriteTime.dwHighDateTime) << 32) | attrs.ftLastWriteTime.dwLowDateTime;
    __time64_t t = static_cast<__time64_t>(std::floor(static_cast<double>(ticks / 10000000 - 11644473600LL) + static_cast<double>(ticks % 10000000) * 1e-7));
    std::tm tm{};
    _gmtime64_s(&tm, &t);
    int64_t year = tm.tm_year;   // years since 1900, as t.tm_year - 1900
    n->stamp = Bytes{static_cast<char>(std::max<int64_t>(0, std::min<int64_t>(255, year))),
                     static_cast<char>(tm.tm_mon + 1), static_cast<char>(tm.tm_mday), static_cast<char>(tm.tm_hour),
                     static_cast<char>(tm.tm_min), static_cast<char>(tm.tm_sec), 0};
    return n;
}

struct Scan {
    NodeP root;
    py::list<py::str> clashes;
};

// os.scandir(path)'s entry.path: the folder, a separator and the name
py::str entry_path(const py::str& folder, const py::str& name) {
    py::str sep = (folder.endswith(py::str("\\")) || folder.endswith(py::str("/"))) ? py::str("") : py::str("\\");
    return folder + sep + name;
}

void fill(Node& node, int64_t depth, py::list<py::str>& clashes) {
    if (depth > 8) {
        throw DiscError(py::percent(py::str("%s: folders nest deeper than the 8 levels ISO-9660 allows"), node.source));
    }
    // by name, as sorted(os.scandir(...), key=name) (the folder's entries, code point order)
    py::list<py::str> names = py::sorted(py::os::listdir(node.source));
    // seen: (is_dir, key) -> the names that become it, in the order first seen
    std::vector<std::tuple<bool, std::tuple<py::str, py::str>, py::list<py::str>>> seen;
    for (const py::str& name : names) {
        py::str path = entry_path(node.source, name);
        bool is_dir = py::os::path::isdir(path);
        if (!is_dir && !py::os::path::isfile(path)) {
            continue;
        }
        std::tuple<py::str, py::str> key = level1_name(name, is_dir);
        bool found = false;
        for (auto& s : seen) {
            if (std::get<0>(s) == is_dir && std::get<1>(s) == key) {
                std::get<2>(s).append(name);
                found = true;
                break;
            }
        }
        if (!found) {
            py::list<py::str> one;
            one.append(name);
            seen.emplace_back(is_dir, key, one);
        }
        node.children.push_back(make_node(path, key, is_dir, &node));
    }
    for (auto& s : seen) {
        const py::list<py::str>& group = std::get<2>(s);
        if (py::len(group) > 1) {
            const auto& key = std::get<1>(s);
            clashes.append(py::percent(py::str("%s: %s all become %s"), node.source, py::join(py::str(", "), group),
                                       std::get<0>(s) ? std::get<0>(key) : py::percent(py::str("%s.%s"), std::get<0>(key), std::get<1>(key))));
        }
    }
    std::stable_sort(node.children.begin(), node.children.end(),
                     [](const NodeP& a, const NodeP& b) { return a->key < b->key; });
    for (auto& child : node.children) {
        if (child->is_dir) {
            fill(*child, depth + 1, clashes);
        }
    }
}

// The tree to write. Children of every folder are in the order the standard asks of
// directory records (by name, then extension). Returns (root, clashes).
Scan scan(const py::str& folder) {
    Scan out;
    out.root = make_node(folder, std::make_tuple(py::str(""), py::str("")), true, nullptr);
    out.root->parent = out.root.get();
    fill(*out.root, 1, out.clashes);
    return out;
}

// Folders in path-table order -- by level, then by parent's number, then by name -- each
// given its 1-based number.
std::vector<Node*> numbered(const NodeP& root) {
    std::vector<Node*> order, level{root.get()};
    while (!level.empty()) {
        order.insert(order.end(), level.begin(), level.end());
        std::vector<Node*> next;
        for (Node* node : level) {
            for (auto& c : node->children) {
                if (c->is_dir) {
                    next.push_back(c.get());
                }
            }
        }
        level = next;
    }
    if (order.size() > 0xFFFF) {
        throw DiscError(std::string("more than 65535 folders"));
    }
    int64_t number = 1;
    for (Node* node : order) {
        node->number = number++;
    }
    return order;
}

Bytes record(const Bytes& ident, int64_t lba, int64_t size, bool is_dir, const Bytes& stamp) {
    size_t length = 33 + ident.size() + (1 - ident.size() % 2);   // padded to an even length
    Bytes r(length, '\0');
    r[0] = static_cast<char>(length);
    Bytes b32;
    put32le(b32, lba);
    put32be(b32, lba);
    assign(r, 2, b32);
    b32.clear();
    put32le(b32, size);
    put32be(b32, size);
    assign(r, 10, b32);
    assign(r, 18, stamp);
    r[25] = static_cast<char>(is_dir ? 2 : 0);
    Bytes v;
    put16le(v, 1);
    put16be(v, 1);
    assign(r, 28, v);
    r[32] = static_cast<char>(ident.size());
    assign(r, 33, ident);
    return r;
}

Bytes zeros(int64_t n) { return Bytes(static_cast<size_t>(n), '\0'); }

// The bytes of one folder's directory: "." and "..", then its children. A record never
// crosses a sector boundary; the whole is padded to full sectors.
Bytes directory(const Node& node) {
    Bytes out = record(Bytes(1, '\0'), node.lba, node.size, true, node.stamp);
    out += record(Bytes(1, '\1'), node.parent->lba, node.parent->size, true, node.parent->stamp);
    for (const auto& child : node.children) {
        Bytes r = record(child->ident, child->lba, child->size, child->is_dir, child->stamp);
        if (static_cast<int64_t>(out.size()) % SECTOR + static_cast<int64_t>(r.size()) > SECTOR) {
            out += zeros(py::mod(-static_cast<int64_t>(out.size()), SECTOR));
        }
        out += r;
    }
    return out + zeros(py::mod(-static_cast<int64_t>(out.size()), SECTOR));
}

Bytes path_table(const std::vector<Node*>& folders, bool big_endian) {
    Bytes out;
    for (const Node* node : folders) {
        Bytes ident = node->ident.empty() ? Bytes(1, '\0') : node->ident;
        out.push_back(static_cast<char>(ident.size()));
        out.push_back('\0');
        if (big_endian) {
            put32be(out, node->lba);
            put16be(out, node->parent->number);
        } else {
            put32le(out, node->lba);
            put16le(out, node->parent->number);
        }
        out += ident + zeros(ident.size() % 2);
    }
    return out;
}

// El Torito boot catalog: a validation entry (its 16-bit words sum to zero) and the
// default entry naming the boot image.
Bytes boot_catalog(int64_t image_lba, int64_t image_size) {
    Bytes check = zeros(32);
    check[0] = 1;   // header; platform 0 = x86
    assign(check, 4, "CODEBREAKER");
    check[30] = '\x55';
    check[31] = '\xAA';
    int64_t total = 0;
    for (int i = 0; i < 16; ++i) {
        total += static_cast<uint8_t>(check[2 * i]) | (static_cast<uint8_t>(check[2 * i + 1]) << 8);
    }
    Bytes sum;
    put16le(sum, static_cast<uint64_t>(-total) & 0xFFFF);
    assign(check, 28, sum);
    int64_t media = _floppy(image_size);
    int64_t count = media ? 1 : std::min<int64_t>(0xFFFF, (image_size + 511) / 512);   // 512-byte sectors to load
    Bytes entry;
    entry.push_back('\x88');
    entry.push_back(static_cast<char>(media));
    put16le(entry, 0);
    entry.push_back(0);
    entry.push_back(0);
    put16le(entry, count);
    put32le(entry, image_lba);
    return check + entry + zeros(SECTOR - 32 - static_cast<int64_t>(entry.size()));
}

Bytes descriptor(int64_t kind, const Bytes& body = Bytes()) {
    return Bytes(1, static_cast<char>(kind)) + "CD001\x01" + body + zeros(SECTOR - 7 - static_cast<int64_t>(body.size()));
}

Bytes both16(int64_t v) {
    Bytes b;
    put16le(b, v);
    put16be(b, v);
    return b;
}
Bytes both32(int64_t v) {
    Bytes b;
    put32le(b, v);
    put32be(b, v);
    return b;
}

// A file to copy into the image: the folder's files, and the boot image.
struct Source {
    py::str source;
    int64_t lba = 0;
    int64_t size = 0;
};

}  // namespace

int64_t _floppy(int64_t image_size) {
    if (image_size == 1228800) return 1;
    if (image_size == 1474560) return 2;
    if (image_size == 2949120) return 3;
    return 0;
}

py::bytes _both16(int64_t v) { return py::bytes(both16(v)); }
py::bytes _both32(int64_t v) { return py::bytes(both32(v)); }

int64_t _sectors(int64_t size) { return py::floordiv(size + SECTOR - 1, SECTOR); }

py::str _clean(const py::str& text, int64_t limit) {
    std::u32string out;
    for (char32_t c : text.upper().u32()) {
        bool ok = false;
        for (const char* a = ALLOWED; *a; ++a) {
            if (static_cast<char32_t>(*a) == c) {
                ok = true;
                break;
            }
        }
        out.push_back(ok ? c : U'_');
    }
    if (static_cast<int64_t>(out.size()) > limit) {
        out.resize(static_cast<size_t>(limit));
    }
    return py::str(out);
}

std::tuple<py::str, py::str> level1_name(const py::str& name, bool is_dir) {
    if (is_dir) {
        py::str base = _clean(name, 8);
        return {py::truthy(base) ? base : py::str("_"), py::str("")};
    }
    auto [base, dot, ext] = name.rpartition(py::str("."));
    if (!py::truthy(dot)) {
        base = name;
        ext = py::str("");
    }
    py::str b = _clean(base, 8);
    return {py::truthy(b) ? b : py::str("_"), _clean(ext, 3)};
}

int64_t write(const py::str& folder, const py::str& out_path, const py::str& volume_id,
              const std::optional<py::str>& boot_image) {
    if (!py::os::path::isdir(folder)) {
        throw DiscError(py::percent(py::str("%s is not a folder"), folder));
    }
    Scan tree = scan(folder);
    if (py::truthy(tree.clashes)) {
        throw DiscError(py::str("names that clash once cut to 8.3:\n  ") + py::join(py::str("\n  "), tree.clashes));
    }
    NodeP root = tree.root;
    std::vector<Node*> folders = numbered(root);
    std::vector<Node*> files;
    for (Node* node : folders) {
        for (auto& c : node->children) {
            if (!c->is_dir) {
                files.push_back(c.get());
            }
        }
    }
    py::list<py::str> big;
    for (Node* f : files) {
        if (f->size >= (int64_t(1) << 32)) {
            big.append(f->source);
        }
    }
    if (py::truthy(big)) {
        throw DiscError(py::str("too large for one ISO-9660 file (4 GB): ") + py::join(py::str(", "), big));
    }

    // layout: 16 system sectors, descriptors, both path tables, boot, directories, files
    int64_t boot_size = boot_image ? py::os::path::getsize(*boot_image) : 0;
    int64_t lba = 17 + (boot_image ? 1 : 0) + 1;
    int64_t table_size = static_cast<int64_t>(path_table(folders, false).size());
    int64_t little_at = lba, big_at = lba + _sectors(table_size);
    lba = big_at + _sectors(table_size);
    int64_t catalog_at = lba, boot_at = lba + 1;
    if (boot_image) {
        lba += 1 + _sectors(boot_size);
    }
    for (Node* node : folders) {
        node->size = static_cast<int64_t>(directory(*node).size());
        node->lba = lba;
        lba += node->size / SECTOR;
    }
    for (Node* node : files) {
        node->lba = lba;
        lba += _sectors(node->size);
    }
    int64_t total = lba;

    std::time_t now_t = std::time(nullptr);
    std::tm now_tm{};
    gmtime_s(&now_tm, &now_t);
    char stamp[32];
    std::strftime(stamp, sizeof stamp, "%Y%m%d%H%M%S00", &now_tm);
    Bytes now = Bytes(stamp) + Bytes(1, '\0');
    Bytes unset = Bytes(16, '0') + Bytes(1, '\0');
    Bytes volume = zeros(SECTOR - 7);   // offsets below are sector offset - 7
    assign(volume, 1, Bytes(32, ' '));   // system identifier
    py::str cleaned = _clean(volume_id, 32);
    py::str vol = py::truthy(cleaned) ? cleaned : py::str("CODEBREAKER");
    assign(volume, 33, vol.ljust(32).encode().raw());
    assign(volume, 73, both32(total));
    assign(volume, 113, both16(1));
    assign(volume, 117, both16(1));
    assign(volume, 121, both16(SECTOR));
    assign(volume, 125, both32(table_size));
    Bytes at4;
    put32le(at4, little_at);
    assign(volume, 133, at4);
    at4.clear();
    put32be(at4, big_at);
    assign(volume, 141, at4);
    assign(volume, 149, record(Bytes(1, '\0'), root->lba, root->size, true, root->stamp));
    assign(volume, 183, Bytes(623, ' '));   // set, publisher, preparer, ... names
    assign(volume, 806, now);
    assign(volume, 823, now);
    assign(volume, 840, unset);
    assign(volume, 857, unset);
    volume[874] = 1;

    std::vector<Source> copies;
    // the output file: seek, write; the size is set at the end
    FILE* out = nullptr;
    {
        py::file probe = py::open(out_path, py::str("wb"));   // (raises what open() raises)
        probe.close();
    }
    out = _wfopen(as_path(out_path).wstring().c_str(), L"wb");
    if (!out) {
        throw py::OSError(py::percent(py::str("cannot open %s"), out_path));
    }
    struct Closer {
        FILE* f;
        ~Closer() { if (f) std::fclose(f); }
    } closer{out};
    auto put = [&](int64_t at, const Bytes& data) {
        _fseeki64(out, at * SECTOR, SEEK_SET);
        std::fwrite(data.data(), 1, data.size(), out);
    };
    put(16, descriptor(1, volume));
    if (boot_image) {
        Bytes body = Bytes("EL TORITO SPECIFICATION");
        body += zeros(64 - static_cast<int64_t>(body.size()));
        put32le(body, catalog_at);
        put(17, descriptor(0, body));
        put(catalog_at, boot_catalog(boot_at, boot_size));
        copies.push_back(Source{*boot_image, boot_at, boot_size});
    }
    for (Node* f : files) {
        copies.push_back(Source{f->source, f->lba, f->size});
    }
    put(17 + (boot_image ? 1 : 0), descriptor(255));
    put(little_at, path_table(folders, false));
    put(big_at, path_table(folders, true));
    for (Node* node : folders) {
        put(node->lba, directory(*node));
    }
    for (const Source& node : copies) {
        _fseeki64(out, node.lba * SECTOR, SEEK_SET);
        int64_t left = node.size;
        py::file source = py::open_rb(node.source);
        while (left) {
            py::bytes piece = source.read(std::min<int64_t>(left, int64_t(1) << 20));
            if (!py::truthy(piece)) {
                throw DiscError(py::percent(py::str("%s shrank while it was being copied"), node.source));
            }
            std::fwrite(piece.raw().data(), 1, piece.raw().size(), out);
            left -= py::len(piece);
        }
    }
    std::fflush(out);
    std::fclose(out);
    closer.f = nullptr;
    std::error_code ec;
    fs::resize_file(as_path(out_path), static_cast<uintmax_t>(total * SECTOR), ec);
    if (ec) {
        throw py::OSError(py::percent(py::str("cannot size %s"), out_path));
    }
    return total * SECTOR;
}

namespace {

void err_line(const py::str& text) {
    std::fflush(stdout);
    std::string s = text.utf8() + "\n";
    std::fwrite(s.data(), 1, s.size(), stderr);
    std::fflush(stderr);
}

const char* const DOC =
    "python -m codebreaker disc <folder> <out.iso> [--volume NAME] [--boot IMG]\n"
    "python -m codebreaker disc --list <image.iso>\n"
    "python -m codebreaker disc --extract <image.iso> <outdir>";

}  // namespace

int64_t main(const py::list<py::str>& argv) {
    py::list<py::str> args;
    py::dict<py::str, py::str> value_options;   // --volume / --boot with their values
    bool list = false, extract = false;
    py::list<py::str> rest = argv.copy();   // list(argv): the caller's list is kept
    while (py::truthy(rest)) {
        py::str a = rest.pop(0);
        if ((a == py::str("--volume") || a == py::str("--boot")) && py::truthy(rest)) {
            value_options[a] = rest.pop(0);
        } else if (a == py::str("--list") || a == py::str("--extract")) {
            (a == py::str("--list") ? list : extract) = true;
        } else {
            args.append(a);
        }
    }
    try {
        if (list && py::len(args) == 1) {
            py::list<py::str> m;
            m.append(py::str("list"));
            m.append(args[0]);
            m.append(py::str("--boot"));
            return formats::media::main(m);
        }
        if (extract && py::len(args) == 2) {
            py::print(py::percent(py::str("extracted %d files to %s"), formats::media::extract(args[0], args[1]), args[1]));
            return 0;
        }
        if (py::len(args) == 2 && !list && !extract) {
            py::str volume = value_options.contains(py::str("--volume")) ? value_options[py::str("--volume")] : py::str("CODEBREAKER");
            std::optional<py::str> boot;
            if (value_options.contains(py::str("--boot"))) {
                boot = value_options[py::str("--boot")];
            }
            int64_t size = write(args[0], args[1], volume, boot);
            py::print(py::percent(py::str("wrote %s: %d bytes%s"), args[1], size,
                                  value_options.contains(py::str("--boot")) ? py::str(" (bootable)") : py::str("")));
            return 0;
        }
    } catch (const DiscError& problem) {
        err_line(py::percent(py::str("disc: %s"), py::str(problem.what())));
        return 1;
    } catch (const py::OSError& problem) {
        err_line(py::percent(py::str("disc: %s"), py::str(problem.what())));
        return 1;
    }
    err_line(py::str(DOC));
    return 2;
}

}  // namespace codebreaker::build::disc_image
