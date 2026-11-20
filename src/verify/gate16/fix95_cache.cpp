#include "verify/gate16/fix95_cache.hpp"

#include <algorithm>
#include <format>
#include <set>
#include <sstream>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/media.hpp"

namespace cb::verify::gate16::fix95 {
namespace {

namespace stdfs = std::filesystem;
namespace media = cb::formats::media;

constexpr std::string_view kManifestHead = "fix95cache 1";

std::string hash_bytes(ByteView data) { return crypto::to_hex(crypto::sha256(data)); }

std::vector<std::string> split_path(std::string_view path) {
    std::vector<std::string> parts;
    std::string part;
    for (const char c : path) {
        if (c == '/' || c == '\\') {
            if (!part.empty()) parts.push_back(part);
            part.clear();
        } else {
            part += c;
        }
    }
    if (!part.empty()) parts.push_back(part);
    return parts;
}

bool is_fat(const media::Volume& v) {
    return v.kind() == media::VolumeKind::Fat12 || v.kind() == media::VolumeKind::Fat16 || v.kind() == media::VolumeKind::Fat32;
}

}  // namespace

std::string hash_file(const stdfs::path& file) { return hash_bytes(fs::read_file(file)); }

std::string key(std::string_view fix_hash, std::string_view iso_hash) {
    const std::string text = std::format("fix95cache v1\nfix {}\niso {}\n", fix_hash, iso_hash);
    return hash_bytes(ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()));
}

std::vector<Wanted> files_of(const stdfs::path& ima) {
    std::vector<Wanted> out;
    std::vector<std::unique_ptr<media::Volume>> volumes = media::volumes(ima);
    if (volumes.empty() || !is_fat(*volumes.front())) throw FormatError("fix95_cache", 0, "the FIX95CPU disk image holds no FAT volume");
    media::Volume& v = *volumes.front();
    for (const media::Entry& e : v.files()) {
        if (e.is_dir) continue;
        std::vector<std::string> parts = split_path(e.path);
        if (parts.size() < 2) continue;
        const std::string edition = text::upper(parts.front());
        if (edition != "W95BOTH" && edition != "W95RTM" && edition != "W95OSR2") continue;
        parts.erase(parts.begin());
        // (the POST folder only says "after WININIT": the file lands beside the others)
        parts.erase(std::remove_if(parts.begin(), parts.end(), [](const std::string& p) { return text::upper(p) == "POST"; }), parts.end());
        std::string under;
        for (const std::string& p : parts) under += (under.empty() ? "" : "/") + text::upper(p);
        out.push_back({text::upper(parts.back()), under, hash_bytes(v.read(e))});
    }
    return out;
}

Capture capture(const stdfs::path& disk_image, const stdfs::path& ima) {
    Capture out;
    const std::vector<Wanted> wanted = files_of(ima);
    std::vector<std::unique_ptr<media::Volume>> volumes = media::volumes(disk_image);
    // C: is the first FAT volume with a MSDOS.SYS in its root
    media::Volume* disk = nullptr;
    const media::Entry* msdos = nullptr;
    for (auto& v : volumes) {
        if (!is_fat(*v)) continue;
        for (const media::Entry& e : v->files())
            if (!e.is_dir && text::upper(e.path) == "MSDOS.SYS") {
                disk = v.get();
                msdos = &e;
                break;
            }
        if (disk) break;
    }
    if (!disk) {
        out.notes.push_back("the installed disk has no FAT volume with an MSDOS.SYS");
        return out;
    }
    // the Windows folder, as MSDOS.SYS names it (WinDir=C:\WINDOWS)
    std::string windir = "WINDOWS";
    {
        const Bytes raw = disk->read(*msdos);
        std::istringstream in(std::string(raw.begin(), raw.end()));
        std::string line;
        while (std::getline(in, line)) {
            const std::string low = text::lower(line);
            if (!low.starts_with("windir=")) continue;
            std::string value = line.substr(7);
            while (!value.empty() && (value.back() == '\r' || value.back() == ' ')) value.pop_back();
            if (value.size() >= 2 && value[1] == ':') value.erase(0, 2);
            std::string joined;
            for (const std::string& p : split_path(value)) joined += (joined.empty() ? "" : "/") + p;
            if (!joined.empty()) windir = joined;
            break;
        }
    }
    std::map<std::string, const media::Entry*> by_path;   // lower-case path (long and short names)
    for (const media::Entry& e : disk->files()) {
        if (e.is_dir) continue;
        by_path[text::lower(e.path)] = &e;
        by_path.emplace(text::lower(e.short_name), &e);
    }
    std::map<std::string, std::string> hash_of;   // captured name -> hash
    std::set<std::string> conflicts;
    for (const Wanted& w : wanted) {
        const std::string where = windir + "/" + w.under;
        const auto at = by_path.find(text::lower(where));
        if (at == by_path.end()) {
            out.notes.push_back(std::format("{}: not on the disk (the other edition's copy)", where));
            continue;
        }
        Bytes data = disk->read(*at->second);
        const std::string got = hash_bytes(data);
        if (got != w.hash) {
            out.notes.push_back(std::format("{}: the disk's file is not the one on the floppy", where));
            continue;
        }
        const auto seen = hash_of.find(w.name);
        if (seen != hash_of.end()) {
            const bool same_place = std::any_of(out.files.begin(), out.files.end(), [&](const Captured& c) {
                return c.name == w.name && text::lower(c.disk_path) == text::lower(at->second->path);
            });
            if (seen->second != got || !same_place) conflicts.insert(w.name);
            continue;
        }
        hash_of[w.name] = got;
        out.files.push_back({w.name, at->second->path, std::move(data)});
    }
    out.conflicts.assign(conflicts.begin(), conflicts.end());
    return out;
}

Entry store(const stdfs::path& folder, const Capture& cap, const std::map<std::string, std::string>& originals) {
    std::error_code ec;
    stdfs::remove_all(folder, ec);
    fs::make_dirs(folder / "files");
    Entry entry;
    entry.folder = folder;
    std::ostringstream manifest;
    manifest << kManifestHead << "\n";
    std::set<std::string> bad(cap.conflicts.begin(), cap.conflicts.end());
    for (const Captured& c : cap.files) {
        const auto original = originals.find(c.name);
        if (original == originals.end()) {   // (in no cabinet: Setup installs no file of that name)
            entry.additions.push_back(c.name);
            manifest << "addition " << c.name << "\n";
            continue;
        }
        File f{c.name, c.disk_path, hash_bytes(c.data), original->second};
        fs::write_file(folder / "files" / c.name, c.data);
        manifest << "file " << f.name << " " << f.disk_path << " " << f.patched_hash << " " << f.original_hash << "\n";
        entry.files.push_back(std::move(f));
    }
    for (const std::string& n : bad) {
        entry.unreplaceable.push_back(n);
        manifest << "unreplaceable " << n << "\n";
    }
    const std::string text = manifest.str();
    fs::write_file_atomic(folder / "manifest.txt", ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()));   // (last)
    return entry;
}

std::optional<Entry> load(const stdfs::path& folder) {
    std::error_code ec;
    if (!stdfs::is_regular_file(folder / "manifest.txt", ec)) return std::nullopt;
    try {
        const Bytes raw = fs::read_file(folder / "manifest.txt");
        std::istringstream in(std::string(raw.begin(), raw.end()));
        std::string line;
        if (!std::getline(in, line)) return std::nullopt;
        while (!line.empty() && line.back() == '\r') line.pop_back();
        if (line != kManifestHead) return std::nullopt;
        Entry entry;
        entry.folder = folder;
        while (std::getline(in, line)) {
            while (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) continue;
            std::istringstream words(line);
            std::string kind;
            words >> kind;
            if (kind == "file") {
                File f;
                words >> f.name >> f.disk_path >> f.patched_hash >> f.original_hash;
                if (f.name.empty() || f.patched_hash.empty() || f.original_hash.empty()) return std::nullopt;
                if (hash_file(entry.path_of(f.name)) != f.patched_hash) return std::nullopt;   // (a file changed or gone)
                entry.files.push_back(std::move(f));
            } else if (kind == "addition") {
                std::string n;
                words >> n;
                entry.additions.push_back(n);
            } else if (kind == "unreplaceable") {
                std::string n;
                words >> n;
                entry.unreplaceable.push_back(n);
            } else {
                return std::nullopt;
            }
        }
        return entry;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

Plan plan(const setup16::Replace& replace, const std::optional<Entry>& entry) {
    Plan out;
    if (!entry) {
        out.note = "fix95cpu cache: none for this FIX95CPU disk and Setup CD yet; the FIX95CPU stage runs";
        return out;
    }
    if (!entry->usable()) {
        out.note = entry->files.empty() ? "fix95cpu cache: the entry holds no files; the FIX95CPU stage runs"
                                        : std::format("fix95cpu cache: {} file(s) cannot be put in place by name; the FIX95CPU stage runs", entry->unreplaceable.size());
        return out;
    }
    std::set<std::string> mine;
    for (const auto& [name, path] : replace) mine.insert(text::upper(name));
    for (const File& f : entry->files)
        if (mine.contains(text::upper(f.name))) {
            out.note = std::format("fix95cpu cache: the run replaces {} itself; the FIX95CPU stage runs", f.name);
            return out;
        }
    setup16::Replace merged = replace;
    for (const File& f : entry->files) merged[f.name] = entry->path_of(f.name);
    out.replace = std::move(merged);
    out.note = std::format("fix95cpu cache: {} patched file(s) installed by Setup; no FIX95CPU stage", entry->files.size());
    if (!entry->additions.empty()) {
        out.note += " (not installed, in no cabinet:";
        for (const std::string& n : entry->additions) out.note += " " + n;
        out.note += ")";
    }
    return out;
}

}  // namespace cb::verify::gate16::fix95
