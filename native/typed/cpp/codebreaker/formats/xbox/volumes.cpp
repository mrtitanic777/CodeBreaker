// codebreaker/formats/xbox/volumes.cpp -- see volumes.hpp.

#include "codebreaker/formats/xbox/volumes.hpp"

#include <algorithm>

#include "codebreaker/formats/xbox/god.hpp"
#include "codebreaker/formats/xbox/heavyiron.hpp"
#include "codebreaker/formats/xbox/nand.hpp"
#include "codebreaker/formats/xbox/pck.hpp"
#include "codebreaker/formats/xbox/stfs.hpp"
#include "codebreaker/formats/xbox/zar.hpp"

namespace codebreaker::formats::xbox::volumes {

namespace {

using media::Boot;
using media::BootValue;
using media::Entry;
using media::Volume;

BootValue text(const std::string& s) { return BootValue(py::str(s)); }
BootValue text(const py::str& s) { return BootValue(s); }
BootValue text(const char* s) { return BootValue(py::str(s)); }
BootValue number(int64_t v) { return BootValue(v); }

VolumeList one(const std::shared_ptr<Volume>& v) {
    VolumeList out;
    out.append(v);
    return out;
}

// the list of names in a folder, os.listdir's order
py::list<py::str> listdir(const py::str& folder) { return py::os::listdir(folder); }

// os.walk(top), top-down: `visit(dirpath, dirnames, filenames)` returns true to stop (a `break` of the loop)
template <class F> bool walk(const py::str& top, F& visit) {
    py::list<py::str> names;
    try {
        names = py::os::listdir(top);
    } catch (const py::OSError&) {
        return false;
    }
    py::list<py::str> dirs, nondirs;
    for (const py::str& n : names) (py::os::path::isdir(py::os::path::join(top, n)) ? dirs : nondirs).append(n);
    if (visit(top, dirs, nondirs)) return true;
    for (const py::str& d : dirs) {
        py::str p = py::os::path::join(top, d);
        if (walk(p, visit)) return true;
    }
    return false;
}

// os.path.getsize(os.path.join(d, n)) for a part
int64_t size_of(const py::str& folder, const py::str& n) { return py::os::path::getsize(py::os::path::join(folder, n)); }

}  // namespace

const py::bytes CSO_MAGIC("CISO");
const py::bytes CCI_MAGIC("CCIM");

std::function<std::shared_ptr<gdf::Device>(const py::str&)> ciso_open_image;
std::function<py::list<std::tuple<py::str, py::bytes>>(const py::str&)> xzp_payloads;

// ---- _Listed ---------------------------------------------------------------------------------------------------------
_Listed::_Listed(const py::str& kind_, const py::str& label_, const py::str& where_, const Boot& boot_,
                 std::function<py::list<std::tuple<py::str, int64_t>>()> listing, std::function<py::bytes(const py::str&)> reader)
    : _listing(std::move(listing)), _reader(std::move(reader)) {
    kind = kind_;
    label = label_;
    where = where_;
    boot = boot_;
}

py::list<Entry> _Listed::files() {
    py::list<Entry> out;
    for (const auto& ps : _listing()) out.append(Entry(std::get<0>(ps), std::get<1>(ps), false, 0));
    return out;
}

py::bytes _Listed::read(const Entry& entry) { return _reader(entry.path); }

// ---- the containers --------------------------------------------------------------------------------------------------
std::shared_ptr<Volume> _stfs(const py::str& path) {
    py::file f = py::file::open(path, py::str("rb"));
    auto p = std::make_shared<stfs::STFS>(f.read());
    f.close();
    Boot boot;
    boot.set(py::str("magic"), text(p->magic.decode("latin-1").strip()));
    boot.set(py::str("title id"), text(p->title_id));
    boot.set(py::str("content type"), text(py::percent(py::str("0x%X"), p->content_type)));
    boot.set(py::str("display name"), text(p->display_name));
    return std::make_shared<_Listed>(py::str("stfs"), p->title_name.empty() ? py::os::path::basename(path) : p->title_name, py::str(""), boot,
                                     [p] {
                                         py::list<std::tuple<py::str, int64_t>> out;
                                         for (const auto& kv : p->files.items()) out.append(std::make_tuple(std::get<0>(kv), std::get<2>(std::get<1>(kv))));
                                         return out;
                                     },
                                     [p](const py::str& name) { return p->read_file(name); });
}

std::shared_ptr<Volume> _xdvdfs(const std::shared_ptr<gdf::Device>& device, const py::str& label, const py::str& where, const py::str& kind,
                                std::optional<int64_t> base, std::optional<int64_t> descriptor) {
    auto g = std::make_shared<gdf::GDF>(device, base, descriptor);
    Boot boot;
    boot.set(py::str("volume at"), text(py::percent(py::str("0x%X"), g->base)));
    return std::make_shared<_Listed>(kind, label, where, boot, [g] { return g->files(); }, [g](const py::str& path) { return g->read(path); });
}

std::optional<VolumeList> _god(const py::str& folder, const std::optional<py::str>& header) {
    auto parts = [](const py::str& d) {
        for (const py::str& n : listdir(d))
            if (n.lower().startswith(py::str("data")) && py::os::path::isfile(py::os::path::join(d, n))) return true;
        return false;
    };
    std::optional<py::str> data;
    if (parts(folder)) data = folder;
    if (!data) {
        auto visit = [&](const py::str& dp, const py::list<py::str>& dn, const py::list<py::str>&) {
            for (const py::str& d : dn)
                if (d.lower().endswith(py::str(".data")) && parts(py::os::path::join(dp, d))) {
                    data = py::os::path::join(dp, d);
                    return true;
                }
            return false;
        };
        walk(folder, visit);
    }
    if (!data) return std::nullopt;
    std::optional<int64_t> base, descriptor;
    if (header) {
        py::file f = py::file::open(*header, py::str("rb"));
        f.seek(0x391);                                       // SVOD descriptor (0x379): features, blocks, first block
        py::bytes d = f.read(7);
        f.close();
        if (d.size() == 7) {
            int64_t first = d[4] | d[5] << 8 | d[6] << 16;   // (24-bit, low byte first)
            bool enhanced = (d[0] & 0x40) != 0;
            base = (enhanced ? 0x1000 : 0) - first * 0x1000;
            descriptor = 0;
        }
    }
    auto device = std::make_shared<god::SvodReader>(*data);
    py::str label = py::os::path::basename(py::os::path::dirname(*data));
    return one(_xdvdfs(device, label.empty() ? py::str("GoD") : label, py::str("Games on Demand"), py::str("god"), base, descriptor));
}

VolumeList _parts(const py::str& folder) {
    std::vector<py::str> names;
    for (const py::str& n : listdir(folder))
        if (py::os::path::isfile(py::os::path::join(folder, n))) names.push_back(n);
    std::sort(names.begin(), names.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
    Boot boot;
    boot.set(py::str("parts"), number(static_cast<int64_t>(names.size())));
    return one(std::make_shared<_Listed>(
        py::str("parts"), py::os::path::basename(folder), py::str(""), boot,
        [folder, names] {
            py::list<std::tuple<py::str, int64_t>> out;
            for (const py::str& n : names) out.append(std::make_tuple(n, size_of(folder, n)));
            return out;
        },
        [folder](const py::str& name) {
            py::file f = py::file::open(py::os::path::join(folder, name), py::str("rb"));
            py::bytes b = f.read();
            f.close();
            return b;
        }));
}

// ---- FatxVolume ------------------------------------------------------------------------------------------------------
FatxVolume::FatxVolume(const std::shared_ptr<fatx::FatxReader>& reader, const fatx::PartitionRef& partition) : _r(reader), _p(partition) {
    kind = py::str("fatx");
    label = partition->name;
    where = py::percent(py::str("partition at 0x%X"), partition->offset);
    boot.set(py::str("cluster"), text(py::percent(py::str("0x%X bytes"), partition->bytes_per_cluster)));
    boot.set(py::str("clusters"), number(partition->max_clusters));
    boot.set(py::str("byte order"), text(partition->endian == py::str(">") ? "big (Xbox 360)" : "little (original Xbox)"));
}

const py::dict<py::str, fatx::EntryRef>& FatxVolume::_all() {
    if (!_entries) {
        py::dict<py::str, fatx::EntryRef> all;
        for (const fatx::EntryRef& e : _r->_flatten(_r->list_directory(_p))) all.set(e->path, e);
        _entries = all;
    }
    return *_entries;
}

py::list<Entry> FatxVolume::files() {
    py::list<Entry> out;
    for (const fatx::EntryRef& e : _all().values())
        out.append(Entry(e->path, e->file_size, e->is_directory, e->first_cluster,
                         e->modified ? e->modified->strftime_minute() : py::str("")));
    return out;
}

py::bytes FatxVolume::read(const Entry& entry) {
    if (!_all().contains(entry.path)) throw py::KeyError(py::repr(entry.path).utf8());
    fatx::EntryRef e = _all().at(entry.path);
    std::string out;
    int64_t remaining = e->file_size;
    for (int64_t cl : _r->cluster_chain(_p, e->first_cluster)) {
        if (remaining <= 0) break;
        py::bytes chunk = _r->read_cluster(_p, cl).slice(0, std::min(remaining, _p->bytes_per_cluster));
        out += chunk.raw();
        remaining -= chunk.size();
    }
    return py::bytes(out);
}

std::optional<VolumeList> _fatx(const py::str& path) {
    auto r = std::make_shared<fatx::FatxReader>(path);
    VolumeList found;
    for (const fatx::PartitionRef& p : r->detect_partitions()) found.append(std::make_shared<FatxVolume>(r, p));
    if (found.empty()) {
        r->close();
        return std::nullopt;
    }
    return found;
}

VolumeList _nand(const py::str& path) {
    py::file f = py::file::open(path, py::str("rb"));
    auto fs = std::make_shared<nand::Flash>(f.read());
    f.close();
    Boot boot;
    boot.set(py::str("files"), number(fs->files.size()));
    return one(std::make_shared<_Listed>(
        py::str("nand"), py::str("flash"), py::percent(py::str("root block %#x"), fs->root), boot,
        [fs] {
            py::list<std::tuple<py::str, int64_t>> out;
            for (const auto& kv : fs->files.items()) out.append(std::make_tuple(std::get<0>(kv), std::get<1>(std::get<1>(kv))));
            return out;
        },
        [fs](const py::str& name) { return fs->read(name); }));
}

VolumeList _zar(const py::str& path) {
    auto z = std::make_shared<zar::ZarReader>(path);
    Boot boot;
    boot.set(py::str("files"), number(z->files().size()));
    return one(std::make_shared<_Listed>(
        py::str("zar"), py::os::path::basename(path), py::str(""), boot, [z] { return z->files(); },
        [z](const py::str& name) {
            std::string out;
            for (const py::bytes& chunk : z->read_iter(name)) out += chunk.raw();
            return py::bytes(out);
        }));
}

VolumeList _payloads(const py::str& kind, const py::str& path, const py::list<std::tuple<py::str, py::bytes>>& found) {
    py::dict<py::str, py::bytes> blobs;
    for (const auto& kv : found) blobs.set(std::get<0>(kv), std::get<1>(kv));
    Boot boot;
    boot.set(py::str("files"), number(blobs.size()));
    return one(std::make_shared<_Listed>(
        kind, py::os::path::basename(path), py::str(""), boot,
        [blobs] {
            py::list<std::tuple<py::str, int64_t>> out;
            for (const auto& kv : blobs.items()) out.append(std::make_tuple(std::get<0>(kv), std::get<1>(kv).size()));
            return out;
        },
        [blobs](const py::str& name) {
            if (!blobs.contains(name)) throw py::KeyError(py::repr(name).utf8());
            return blobs.at(name);
        }));
}

std::optional<VolumeList> volumes(const py::str& path, bool long_names) {
    (void)long_names;
    if (py::os::path::isdir(path)) return _god(path);
    py::bytes head;
    {
        py::file f = py::file::open(path, py::str("rb"));
        head = f.read(16);
        f.close();
    }
    py::bytes magic = head.slice(0, 4);
    if (stfs::is_magic(magic)) {
        // (a package whose contents are the Data#### parts in the `<header>.data` folder beside it: a Games-on-Demand title
        // when its volume is a disc (SVOD, volume type 1 at 0x3A9), else the parts themselves -- a video download is its
        // .wmv cut into them)
        if (py::os::path::isdir(path + py::str(".data"))) {
            bool svod;
            {
                py::file f = py::file::open(path, py::str("rb"));
                f.seek(0x3A9);
                py::bytes four = f.read(4);
                f.close();
                int64_t v = 0;
                for (int64_t i = 0; i < four.size(); ++i) v = (v << 8) | four[i];       // int.from_bytes(..., "big")
                svod = v == 1;
            }
            if (svod) return _god(path + py::str(".data"), path);
            return _parts(path + py::str(".data"));
        }
        return one(_stfs(path));
    }
    if (magic == CSO_MAGIC || magic == CCI_MAGIC) {
        if (!ciso_open_image) throw py::ImportError("No module named 'codebreaker.formats.xbox.ciso'");
        return one(_xdvdfs(ciso_open_image(path), py::os::path::basename(path), magic.decode("latin-1").strip() + py::str(" compressed disc")));
    }
    if (nand::is_nand(path)) return _nand(path);
    if (path.lower().endswith(py::str(".ho")) && heavyiron::is_archive(path)) {
        heavyiron::VolumeListing vl = heavyiron::volume_listing(path);
        Boot boot;
        boot.set(py::str("assets"), number(vl.entries.size()));
        auto listing = vl.entries;
        return one(std::make_shared<_Listed>(py::str("heavyiron"), py::os::path::basename(path), py::str(""), boot, [listing] { return listing; }, vl.read));
    }
    if (magic == py::bytes("XUIZ")) {
        if (!xzp_payloads) throw py::ImportError("No module named 'codebreaker.formats.xbox.xzp'");
        return _payloads(py::str("xzp"), path, xzp_payloads(path));
    }
    if (path.lower().endswith(py::str(".zar"))) return _zar(path);
    if (path.lower().endswith(py::str(".pck"))) return _payloads(py::str("pck"), path, std::get<0>(pck::payloads(path)));
    auto dev = std::make_shared<gdf::FileDevice>(path);
    try {
        return one(_xdvdfs(dev, py::os::path::basename(path), py::str("disc image")));
    } catch (const py::ValueError&) {
        dev->close();
    } catch (const py::AssertionError&) {
        dev->close();
    } catch (const py::StructError&) {
        dev->close();
    }
    if (magic == py::bytes("FATX") || py::os::path::getsize(path) >= 0x80000) {
        try {
            return _fatx(path);
        } catch (const py::OSError&) {
            return std::nullopt;
        } catch (const py::ValueError&) {
            return std::nullopt;
        } catch (const py::StructError&) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

}  // namespace codebreaker::formats::xbox::volumes
