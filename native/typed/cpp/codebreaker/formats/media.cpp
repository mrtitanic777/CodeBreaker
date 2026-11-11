// codebreaker/formats/media.cpp -- see media.hpp.

#include "codebreaker/formats/media.hpp"

#include <algorithm>

#include "codebreaker/formats/xbox/gdf.hpp"
#include "codebreaker/formats/xbox/volumes.hpp"

// a value read from an answer as it is (the volumes' list tells its kind first, and the kind says what the rest is)
namespace py::native {
template <> struct reader<Val> {
    static Val get(const Val& v) { return v; }
};
}  // namespace py::native

namespace codebreaker::formats::media {

namespace {

using xbox::gdf::native::ask;

// formats.dll's answer for one entry: (path, size, is_dir, start, when, short, attributes) -- an ISO entry's has its extents too
using EntryAnswer = std::tuple<py::str, int64_t, bool, int64_t, py::str, py::str, int64_t>;
using IsoEntryAnswer = std::tuple<py::str, int64_t, bool, int64_t, py::str, py::str, int64_t, py::list<std::tuple<int64_t, int64_t>>>;

Entry entry_of(const EntryAnswer& t) {
    return Entry(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t), std::get<5>(t), std::get<6>(t));
}
Entry entry_of(const IsoEntryAnswer& t) {
    Entry e(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t), std::get<5>(t), std::get<6>(t));
    e.extents = py::list<std::tuple<int64_t, int64_t>>::from(std::get<7>(t));
    return e;
}

}  // namespace

// ---- Entry -----------------------------------------------------------------------------------------------------------
Entry::Entry(const py::str& path_, int64_t size_, bool is_dir_, int64_t start_, const py::str& when_, const py::str& short__, int64_t attributes_)
    : path(path_), size(size_), is_dir(is_dir_), start(start_), when(when_), short_(short__.empty() ? path_ : short__), attributes(attributes_) {}

py::str Entry::repr() const { return py::percent(py::str("Entry(%r, %d)"), path, size); }

// ---- _Source ---------------------------------------------------------------------------------------------------------
py::bytes _Source::read(int64_t at, int64_t n) const {
    py::file f = py::file::open(path, py::str("rb"));
    // (a binary file's seek to before its start is the system's EINVAL, an OSError; the runtime's file::seek gives the text file's ValueError)
    if (offset + at < 0) throw py::OSError("[Errno 22] Invalid argument");
    f.seek(offset + at);
    py::bytes data = f.read(n);
    f.close();
    return data;
}

// ---- FatVolume -------------------------------------------------------------------------------------------------------
void FatVolume::take_(const FatAttrs& a) {
    where = a.where;
    old = a.old;
    bps = a.bps;
    spc = a.spc;
    roots = a.roots;
    fats = a.fats;
    spf = a.spf;
    fat_at = a.fat_at;
    root_at = a.root_at;
    data_at = a.data_at;
    clusters = a.clusters;
    bits = a.bits;
    kind = a.kind;
    root_cluster = a.root_cluster;
    boot = a.boot;
    fat = a.fat;
    label = a.label;
}

FatVolume::FatVolume(const _Source& source, const py::str& where_, std::optional<int64_t> size) : src(source) {
    take_(ask<FatAttrs>("media.fat", py::bytes(), src.path, src.offset, where_, size));
}

FatVolume::FatVolume(const _Source& source, const FatAttrs& attrs) : src(source) { take_(attrs); }

py::list<Entry> FatVolume::files() {
    if (!_entries) {
        auto answer = ask<std::tuple<py::list<EntryAnswer>, py::str>>("media.fat_files", fat, src.path, src.offset, bits, spc, bps, data_at,
                                                                      root_at, roots, root_cluster, label);
        label = std::get<1>(answer);
        py::list<Entry> out;
        for (const auto& t : std::get<0>(answer)) out.append(entry_of(t));
        _entries = out;
    }
    return *_entries;
}

int64_t FatVolume::_next(int64_t c) { return ask<int64_t>("media.fat_next", fat, bits, c); }

py::bytes FatVolume::_chain(int64_t cluster, std::optional<int64_t> size) {
    return ask<py::bytes>("media.fat_read", fat, src.path, src.offset, bits, spc, bps, data_at, cluster, size);
}

py::bytes FatVolume::read(const Entry& entry) { return entry.is_dir ? py::bytes() : _chain(entry.start, entry.size); }

// ---- IsoVolume -------------------------------------------------------------------------------------------------------
void IsoVolume::take_(const IsoAttrs& a) {
    sector = a.sector;
    skip = a.skip;
    where = a.where;
    boot_catalog = a.boot_catalog;
    long_names = a.long_names;
    block = a.block;
    label = a.label;
    root = a.root;
    boot = a.boot;
    kind = py::str("iso9660");
}

IsoVolume::IsoVolume(const _Source& source, int64_t sector_, int64_t skip_, bool long_names_) : src(source) {
    take_(ask<IsoAttrs>("media.iso", py::bytes(), src.path, src.offset, sector_, skip_, long_names_));
}

IsoVolume::IsoVolume(const _Source& source, const IsoAttrs& attrs) : src(source) { take_(attrs); }

py::list<Entry> IsoVolume::files() {
    if (!_entries) {
        py::list<Entry> out;
        for (const auto& t : ask<py::list<IsoEntryAnswer>>("media.iso_files", py::bytes(), src.path, src.offset, sector, skip, long_names,
                                                           std::get<0>(root), std::get<1>(root)))
            out.append(entry_of(t));
        _entries = out;
    }
    return *_entries;
}

py::bytes IsoVolume::read(const Entry& entry) {
    if (entry.is_dir) return py::bytes();
    if (!entry.extents) throw py::AttributeError("'Entry' object has no attribute 'extents'");
    return ask<py::bytes>("media.iso_read", py::bytes(), src.path, src.offset, sector, skip, *entry.extents);
}

std::optional<std::tuple<int64_t, int64_t, int64_t>> IsoVolume::boot_image() {
    return ask<std::optional<std::tuple<int64_t, int64_t, int64_t>>>("media.iso_boot_image", py::bytes(), src.path, src.offset, sector, skip,
                                                                     boot_catalog);
}

// ---- BundleVolume ----------------------------------------------------------------------------------------------------
BundleVolume::BundleVolume(const py::str& path) {
    kind = py::str("bundle");
    where = py::str("");
    modules = ne::open_boot_image(path);
    if (modules.size() < 2) throw py::ValueError("not a bundle of modules");
    label = py::os::path::basename(path);
    boot.set(py::str("modules"), BootValue(modules.size()));
}

py::list<Entry> BundleVolume::files() {
    py::list<Entry> out;
    for (const auto& kv : modules.items()) {
        const py::str& name = std::get<0>(kv);
        out.append(Entry(name + py::str(".EXE"), _bytes(name).size(), false, std::get<1>(kv).ne_off, py::str(""), name + py::str(".EXE")));
    }
    return out;
}

py::bytes BundleVolume::_bytes(const py::str& name) {
    if (!_built.contains(name)) _built.set(name, ne::standalone(modules.at(name)));
    return _built.at(name);
}

py::bytes BundleVolume::read(const Entry& entry) { return _bytes(entry.path.slice(0, entry.path.size() - 4)); }

// ---- any image -------------------------------------------------------------------------------------------------------
py::list<std::shared_ptr<Volume>> volumes(const py::str& path, bool long_names) {
    namespace nat = py::native;
    // (first: an Xbox 360 disc also carries an ISO-9660 video part)
    std::optional<xbox::volumes::VolumeList> found = xbox::volumes::volumes(path, long_names);
    if (found && !found->empty()) return *found;
    if (py::os::path::isdir(path))
        throw py::ValueError(py::os::path::basename(path).utf8() + ": a folder, and not a Games-on-Demand package");
    py::list<std::shared_ptr<Volume>> out;
    nat::Val answer = ask<nat::Val>("media.volumes", py::bytes(), path, long_names);
    for (const nat::Val& item : answer.items) {
        const std::string& kind = item.items.at(0).s;
        int64_t offset = item.items.at(1).i;
        const nat::Val& attrs = item.items.at(2);
        if (kind == "iso") out.append(std::make_shared<IsoVolume>(_Source(path, offset), nat::read<IsoAttrs>(attrs)));
        else out.append(std::make_shared<FatVolume>(_Source(path, offset), nat::read<FatAttrs>(attrs)));
    }
    if (!out.empty()) return out;
    try {
        py::list<std::shared_ptr<Volume>> bundle;
        bundle.append(std::make_shared<BundleVolume>(path));
        return bundle;
    } catch (const py::ValueError&) {
    } catch (const py::StructError&) {
    } catch (const py::IndexError&) {
    } catch (const py::KeyError&) {
    }
    throw py::ValueError(py::os::path::basename(path).utf8() + ": no FAT volume, partition table, ISO-9660 volume or module bundle found");
}

py::list<std::tuple<py::str, py::str>> put(const py::str& path, const py::dict<py::str, py::bytes>& files, int64_t volume) {
    std::shared_ptr<Volume> vol = volumes(path)[volume];
    auto fat = std::dynamic_pointer_cast<FatVolume>(vol);
    if (!fat || fat->old) throw py::ValueError("not a FAT volume with a parameter block");
    py::list<std::tuple<py::str, py::bytes>> given;
    for (const auto& kv : files.items()) given.append(std::make_tuple(std::get<0>(kv), std::get<1>(kv)));
    return ask<py::list<std::tuple<py::str, py::str>>>("media.put", py::bytes(), path, fat->src.offset, given);
}

py::str _safe(const py::str& path) {
    std::vector<std::u32string> parts;
    std::u32string cur;
    for (char32_t c : path.replace(py::str("\\"), py::str("/")).u32()) {
        if (c == U'/') {
            parts.push_back(cur);
            cur.clear();
        } else cur.push_back(c);
    }
    parts.push_back(cur);
    std::optional<py::str> joined;
    for (const std::u32string& p : parts) {
        if (p.empty() || p == U"." || p == U"..") continue;
        std::u32string clean;
        for (char32_t c : p) clean.push_back((c != U'<' && c != U'>' && c != U':' && c != U'"' && c != U'|' && c != U'?' && c != U'*' && c >= 32) ? c : U'_');
        joined = joined ? py::os::path::join(*joined, py::str(clean)) : py::str(clean);
    }
    if (!joined) throw py::TypeError("join() missing 1 required positional argument: 'path'");
    return *joined;
}

// ---- fnmatch (Python's, with the platform's case rules) -----------------------------------------------------------------
namespace {

struct GlobPart {
    enum Kind { Star, Any, Char, Set, Never } kind = Char;
    char32_t c = 0;
    bool negate = false;
    std::vector<std::pair<char32_t, char32_t>> ranges;
};

struct Tok {
    char32_t ch;
    bool plain;        // an escaped character (a literal), or the "-" that makes a range
};

// fnmatch.translate: the pattern as parts to match the whole name against
std::vector<GlobPart> translate(const std::u32string& pat) {
    std::vector<GlobPart> parts;
    size_t i = 0, n = pat.size();
    while (i < n) {
        char32_t c = pat[i++];
        GlobPart part;
        if (c == U'*') {
            if (parts.empty() || parts.back().kind != GlobPart::Star) {
                part.kind = GlobPart::Star;
                parts.push_back(part);
            }
        } else if (c == U'?') {
            part.kind = GlobPart::Any;
            parts.push_back(part);
        } else if (c == U'[') {
            size_t j = i;
            if (j < n && pat[j] == U'!') ++j;
            if (j < n && pat[j] == U']') ++j;
            while (j < n && pat[j] != U']') ++j;
            if (j >= n) {
                part.c = U'[';
                parts.push_back(part);
            } else {
                std::u32string stuff = pat.substr(i, j - i);
                std::vector<Tok> toks;
                if (stuff.find(U'-') == std::u32string::npos) {
                    for (char32_t ch : stuff) toks.push_back({ch, true});
                } else {
                    std::vector<std::u32string> chunks;
                    size_t k = pat[i] == U'!' ? i + 2 : i + 1;
                    while (true) {
                        size_t f = k < j ? pat.find(U'-', k) : std::u32string::npos;
                        if (f == std::u32string::npos || f >= j) break;
                        chunks.push_back(pat.substr(i, f - i));
                        i = f + 1;
                        k = f + 3;
                    }
                    std::u32string chunk = pat.substr(i, j - i);
                    if (!chunk.empty()) chunks.push_back(chunk);
                    else if (chunks.empty()) throw py::IndexError("list index out of range");
                    else chunks.back() += U'-';
                    // remove empty ranges -- invalid in RE
                    for (size_t k2 = chunks.size() - 1; k2 > 0; --k2) {
                        if (chunks[k2 - 1].empty() || chunks[k2].empty()) throw py::IndexError("string index out of range");
                        if (chunks[k2 - 1].back() > chunks[k2][0]) {
                            chunks[k2 - 1] = chunks[k2 - 1].substr(0, chunks[k2 - 1].size() - 1) + chunks[k2].substr(1);
                            chunks.erase(chunks.begin() + static_cast<long>(k2));
                        }
                    }
                    for (size_t k2 = 0; k2 < chunks.size(); ++k2) {
                        if (k2) toks.push_back({U'-', false});
                        for (char32_t ch : chunks[k2]) toks.push_back({ch, true});
                    }
                }
                i = j + 1;
                if (toks.empty()) {
                    part.kind = GlobPart::Never;
                } else if (toks.size() == 1 && toks[0].ch == U'!') {
                    part.kind = GlobPart::Any;          // a negated empty range: any character
                } else {
                    part.kind = GlobPart::Set;
                    size_t at = 0;
                    if (toks[0].ch == U'!') {
                        part.negate = true;
                        at = 1;
                    }
                    while (at < toks.size()) {
                        char32_t lo = toks[at].ch;
                        if (at + 2 < toks.size() && toks[at + 1].ch == U'-' && !toks[at + 1].plain) {
                            part.ranges.emplace_back(lo, toks[at + 2].ch);
                            at += 3;
                        } else {
                            part.ranges.emplace_back(lo, lo);
                            at += 1;
                        }
                    }
                }
                parts.push_back(part);
            }
        } else {
            part.c = c;
            parts.push_back(part);
        }
    }
    return parts;
}

bool match_one(const GlobPart& p, char32_t ch) {
    switch (p.kind) {
    case GlobPart::Any: return true;
    case GlobPart::Char: return p.c == ch;
    case GlobPart::Set: {
        bool in = false;
        for (const auto& r : p.ranges)
            if (r.first <= ch && ch <= r.second) in = true;
        return in != p.negate;
    }
    default: return false;
    }
}

// the whole name matches the parts
bool glob_match(const std::vector<GlobPart>& parts, const std::u32string& s) {
    std::vector<char> now(s.size() + 1, 0), next;
    now[0] = 1;
    for (const GlobPart& p : parts) {
        next.assign(s.size() + 1, 0);
        if (p.kind == GlobPart::Star) {
            bool open = false;
            for (size_t k = 0; k <= s.size(); ++k) {
                open = open || now[k];
                next[k] = open;
            }
        } else {
            for (size_t k = 0; k < s.size(); ++k)
                if (now[k] && match_one(p, s[k])) next[k + 1] = 1;
        }
        now.swap(next);
    }
    return now[s.size()] != 0;
}

}  // namespace

bool fnmatch(const py::str& name, const py::str& pat) {
    // os.path.normcase (Windows): lower case, "/" is "\"
    py::str n = name.lower().replace(py::str("/"), py::str("\\")), p = pat.lower().replace(py::str("/"), py::str("\\"));
    return glob_match(translate(p.u32()), n.u32());
}

int64_t extract(const py::str& path, const py::str& folder, const py::list<py::str>& names, bool long_names) {
    int64_t n = 0;
    int64_t i = 0;
    for (const std::shared_ptr<Volume>& v : volumes(path, long_names)) {
        py::str base = i == 0 ? folder : py::os::path::join(folder, py::percent(py::str("volume%d"), i + 1));
        ++i;
        for (const Entry& e : v->files()) {
            bool wanted = names.empty();
            if (!wanted) {
                int64_t slash = e.path.rfind(py::str("/"));
                py::str last = slash < 0 ? e.path : e.path.slice(slash + 1, e.path.size());
                for (const py::str& w : names)
                    if (fnmatch(e.path.lower(), w.lower()) || fnmatch(last.lower(), w.lower())) {
                        wanted = true;
                        break;
                    }
            }
            if (e.is_dir || !wanted) continue;
            py::str out = py::os::path::join(base, _safe(e.path));
            py::str dir = py::os::path::dirname(out);
            py::os::makedirs(dir.empty() ? py::str(".") : dir, 0777, true);
            py::file f = py::file::open(out, py::str("wb"));
            f.write(v->read(e));
            f.close();
            ++n;
        }
    }
    return n;
}

const py::str MAIN_DOC(
    "python -m codebreaker media list <image> [--boot] [--plain]\n"
    "python -m codebreaker media extract <image> <folder> [name ...] [--plain]\n"
    "\n"
    "List or extract the files of a floppy, hard-disk or CD image, or of a Windows 1.x boot\n"
    "image. --boot also prints each volume's boot sector fields; --plain uses 8.3 / primary names.");

int64_t main(const py::list<py::str>& argv) {
    py::list<py::str> flags, args;
    for (const py::str& a : argv) (a.startswith(py::str("--")) ? flags : args).append(a);
    if (args.size() < 2 || !(args[0] == py::str("list") || args[0] == py::str("extract")) || (args[0] == py::str("extract") && args.size() < 3)) {
        py::print(MAIN_DOC);
        return 2;
    }
    bool long_names = !flags.contains(py::str("--plain"));
    if (args[0] == py::str("extract")) {
        py::print(py::percent(py::str("%d files written to %s"), extract(args[1], args[2], args.slice_from(3), long_names), args[2]));
        return 0;
    }
    for (const std::shared_ptr<Volume>& v : volumes(args[1], long_names)) {
        py::list<Entry> files = v->files();
        int64_t nfiles = 0, nfolders = 0, nbytes = 0;
        for (const Entry& e : files) {
            if (!e.is_dir) {
                ++nfiles;
                nbytes += e.size;
            } else ++nfolders;
        }
        py::print(py::percent(py::str("%s volume %r%s: %d files, %d folders, %d bytes"), v->kind, v->label,
                              v->where.empty() ? py::str("") : py::percent(py::str(" (%s)"), v->where), nfiles, nfolders, nbytes));
        if (flags.contains(py::str("--boot"))) {
            for (const auto& kv : v->boot.items()) {
                const BootValue& value = std::get<1>(kv);
                py::str shown = std::holds_alternative<py::str>(value) ? std::get<py::str>(value) : py::to_str(std::get<int64_t>(value));
                py::print(py::percent(py::str("    %-20s %s"), std::get<0>(kv), shown));
            }
        }
        for (const Entry& e : files)
            py::print(py::percent(py::str("  %10s  %-16s  %s"), e.is_dir ? py::str("<folder>") : py::to_str(e.size), e.when,
                                  long_names ? e.path : e.short_));
    }
    return 0;
}

}  // namespace codebreaker::formats::media
