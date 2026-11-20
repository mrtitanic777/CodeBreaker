#include "recompile/title.hpp"

#include <algorithm>
#include <array>
#include <nlohmann/json.hpp>
#include <string_view>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/random_file.hpp"
#include "core/text.hpp"
#include "formats/xbox/volumes.hpp"

namespace cb::recompile::title {
namespace {

namespace stdfs = std::filesystem;
namespace media = formats::media;

constexpr std::array<std::string_view, 3> kPackageMagics = {"CON ", "LIVE", "PIRS"};   // the STFS packages
constexpr std::array<std::string_view, 4> kDiscExtensions = {".iso", ".xiso", ".cso", ".cci"};
constexpr std::string_view kDefaultXex = "default.xex";

bool has_disc_extension(std::string_view name) {
    const std::string lower = text::lower(name);
    return std::ranges::any_of(kDiscExtensions, [&](std::string_view e) { return lower.ends_with(e); });
}

// The name of a path's last part ("C:/games/halo/" is "halo").
std::string base_name(const stdfs::path& path) {
    stdfs::path p = path;
    if (!p.has_filename()) p = p.parent_path();
    return fs::to_text(p.filename());
}

stdfs::path absolute_path(const stdfs::path& path) { return stdfs::absolute(path).lexically_normal(); }

// A folder's entries: the names of its subfolders and of everything else, each sorted; what cannot be listed is not there.
struct Listing {
    std::vector<std::string> folders, files;
};

Listing list_folder(const stdfs::path& folder) {
    Listing out;
    std::error_code ec;
    for (stdfs::directory_iterator it(folder, stdfs::directory_options::skip_permission_denied, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code status_ec;
        const bool dir = it->is_directory(status_ec);
        (dir ? out.folders : out.files).push_back(fs::to_text(it->path().filename()));
    }
    std::ranges::sort(out.folders);
    std::ranges::sort(out.files);
    return out;
}

// The first four bytes of a file, or nothing when it cannot be read (a file met while scanning a folder, not one asked for).
std::optional<Bytes> head_of(const stdfs::path& file) {
    try {
        const fs::RandomFile f(file, fs::FileMode::Read);
        return f.read_at(0, 4);
    } catch (const IoError&) {
        return std::nullopt;
    }
}

bool is_marker_of(const stdfs::path& marker, const std::string& source, std::uint64_t size) {
    std::string content;
    try {
        content = fs::read_text(marker);
    } catch (const IoError&) {
        return false;   // no marker yet
    }
    Json got;
    if (!json::try_parse(content, got) || !got.is_object()) return false;
    const auto field = [&](const char* key) { return got.contains(key) ? &got.at(key) : nullptr; };
    const Json* s = field("source");
    const Json* n = field("size");
    const Json* f = field("format");
    return s && n && f && s->is_string() && n->is_number_integer() && f->is_number_integer() && s->get<std::string>() == source &&
           n->get<std::int64_t>() == static_cast<std::int64_t>(size) && f->get<int>() == kMarkerFormat;
}

void write_marker(const stdfs::path& marker, const std::string& source, std::uint64_t size) {
    Json stamp = Json::object();
    stamp["source"] = source;
    stamp["size"] = size;
    stamp["format"] = kMarkerFormat;
    json::save(marker, stamp, json::Style::PythonCompatible);   // the stamp's established JSON style
}

// A file copied with its modification time.
void copy_with_time(const stdfs::path& from, const stdfs::path& to) {
    fs::make_dirs(to.parent_path());
    stdfs::copy_file(from, to, stdfs::copy_options::overwrite_existing);
    std::error_code ec;
    cb::fs::set_last_write_time(to, cb::fs::last_write_time(from, ec), ec);   // (best effort: a copy without its time is still the copy)
}

// The label a volume's boot facts give the title: its "display name", when it has one.
std::string display_name(const media::Volume& volume) {
    for (const media::BootField& f : volume.boot()) {
        if (f.name == "display name") return f.text;
    }
    return {};
}

}  // namespace

Bytes Title::read_xex() const {
    if (folder_) {
        const std::optional<std::string> name = default_xex(*folder_);
        if (!name) throw NotFoundError(*folder_ / std::string(kDefaultXex), 0, "no default.xex in the title's folder");
        return fs::read_file(*folder_ / *name);
    }
    if (!xex_) throw NotFoundError(source_, 0, "no default.xex in the title");
    return volume_->read(volume_->files()[*xex_]);
}

std::size_t Title::unpack(const stdfs::path& folder) const {
    const stdfs::path marker = folder / kMarkerName;
    const std::string source = fs::to_text(absolute_path(source_));
    const std::uint64_t size = size_of(source_);
    if (is_marker_of(marker, source, size)) return 0;

    std::size_t written = 0;
    if (folder_) {
        std::error_code ec;
        for (stdfs::recursive_directory_iterator it(*folder_, stdfs::directory_options::skip_permission_denied, ec), end; !ec && it != end;
             it.increment(ec)) {
            std::error_code status_ec;
            if (it->is_directory(status_ec)) continue;
            copy_with_time(it->path(), folder / it->path().lexically_relative(*folder_));
            ++written;
        }
    } else {
        for (const media::Entry& e : volume_->files()) {
            if (e.is_dir) continue;
            fs::write_file(folder / media::safe_relative_path(e.path), volume_->read(e));
            ++written;
        }
    }
    write_marker(marker, source, size);
    return written;
}

std::uint64_t size_of(const stdfs::path& path) {
    if (stdfs::is_regular_file(path)) return fs::file_size(path);
    std::uint64_t total = 0;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(path, stdfs::directory_options::skip_permission_denied, ec), end; !ec && it != end;
         it.increment(ec)) {
        std::error_code status_ec;
        if (it->is_regular_file(status_ec)) total += it->file_size(status_ec);
    }
    return total;
}

std::optional<std::string> default_xex(const stdfs::path& folder) {
    for (const std::string& name : list_folder(folder).files) {
        if (text::lower(name) != kDefaultXex) continue;
        const std::optional<Bytes> head = head_of(folder / fs::from_text(name));
        if (head && std::string_view(reinterpret_cast<const char*>(head->data()), head->size()) == "XEX2") return name;
    }
    return std::nullopt;
}

std::optional<Title> from_volumes(const stdfs::path& path, std::optional<Volumes> volumes) {
    if (!volumes) return std::nullopt;
    for (std::unique_ptr<media::Volume>& v : *volumes) {
        std::optional<std::size_t> xex;
        const std::vector<media::Entry>& files = v->files();
        for (std::size_t i = 0; i < files.size() && !xex; ++i) {
            if (files[i].is_dir) continue;
            std::string p = text::replace_all(files[i].path, "\\", "/");
            if (text::lower(text::trim(p, "/")) == kDefaultXex) xex = i;
        }
        if (!xex) continue;

        // the display name, else the volume's label, else the file's name; a disc image's extension goes
        std::string name = display_name(*v);
        if (name.empty()) name = v->label();
        if (name.empty()) name = base_name(path);
        if (has_disc_extension(name) && name.find_last_of('.') != 0) name.erase(name.find_last_of('.'));
        std::string kept;
        for (const char c : name) {
            if (c >= 0x20 && c < 0x7F) kept.push_back(c);   // (what is ASCII and printable)
        }
        const std::string_view trimmed = text::trim(kept);

        Title t(path, trimmed.empty() ? "title" : std::string(trimmed));
        t.volume_ = std::move(v);
        t.xex_ = xex;
        return t;
    }
    return std::nullopt;
}

namespace {

// A title when the file `file` is a package or disc image holding one.
std::optional<Title> container(const stdfs::path& file, const FindVolumes& find_volumes) {
    const std::optional<Bytes> magic = head_of(file);
    if (!magic) return std::nullopt;
    const std::string_view head(reinterpret_cast<const char*>(magic->data()), magic->size());
    if (!std::ranges::any_of(kPackageMagics, [&](std::string_view m) { return m == head; }) && !has_disc_extension(fs::to_text(file))) return std::nullopt;
    try {
        return from_volumes(file, find_volumes(file));
    } catch (const Error&) {   // a damaged or foreign file: not this title
        return std::nullopt;
    }
}

std::optional<Title> search(const stdfs::path& folder, const FindVolumes& find_volumes,
                            const std::function<Title(const stdfs::path&, const std::string&)>& folder_title) {
    const Listing listing = list_folder(folder);
    for (const std::string& name : listing.files) {
        if (std::optional<Title> t = container(folder / fs::from_text(name), find_volumes)) return t;
    }
    for (const std::string& name : listing.folders) {
        const stdfs::path sub = folder / fs::from_text(name);
        if (default_xex(sub)) return folder_title(sub, name);
    }
    for (const std::string& name : listing.folders) {
        if (std::optional<Title> t = search(folder / fs::from_text(name), find_volumes, folder_title)) return t;
    }
    return std::nullopt;
}

}  // namespace

std::optional<Title> locate(const stdfs::path& given, const FindVolumes& find_volumes) {
    const stdfs::path path = absolute_path(given);
    std::error_code ec;
    if (stdfs::is_regular_file(path, ec)) return container(path, find_volumes);
    if (!stdfs::is_directory(path, ec)) return std::nullopt;

    const auto folder_title = [](const stdfs::path& folder, const std::string& name) {
        Title t(folder, name);
        t.folder_ = folder;
        return t;
    };
    if (default_xex(path)) return folder_title(path, base_name(path));
    try {
        std::optional<Title> t = from_volumes(path, find_volumes(path));   // Games on Demand
        if (t) {
            if (t->name_ == "GoD") t->name_ = base_name(path);
            return t;
        }
    } catch (const Error&) {   // (not a container: look inside)
    }
    return search(path, find_volumes, folder_title);
}

std::optional<Title> locate(const stdfs::path& path) {
    return locate(path, [](const stdfs::path& p) { return formats::xbox::volumes::open(p); });
}

}  // namespace cb::recompile::title
