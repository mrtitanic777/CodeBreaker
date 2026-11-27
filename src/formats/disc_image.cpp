#include "formats/disc_image.hpp"

#include <algorithm>
#include <array>
#include <cstring>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/random_file.hpp"
#include "core/text.hpp"
#include "formats/probe.hpp"

namespace cb::formats::disc {

namespace {

constexpr std::string_view kReader = "disc";
constexpr std::array<std::uint8_t, 12> kSync = {0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0};

}  // namespace

bool raw_sync(ByteView sector) { return sector.size() >= kSync.size() && std::memcmp(sector.data(), kSync.data(), kSync.size()) == 0; }

std::size_t data_offset(ByteView raw_sector) { return raw_sector.size() > 15 && raw_sector[15] == 2 ? kRawHeader + kSubheader : kRawHeader; }

std::optional<Cue> read_cue(const std::filesystem::path& sheet) {
    std::string text_of;
    try {
        text_of = fs::read_text(sheet);
    } catch (const Error&) {
        return std::nullopt;
    }
    Cue cue;
    for (const std::string_view raw : text::lines(text_of)) {
        const std::string_view line = text::trim(raw);
        if (line.size() > 5 && text::iequals(line.substr(0, 5), "FILE ") && cue.file.empty()) {
            const std::size_t open = line.find('"');
            const std::size_t close = line.rfind('"');
            if (open == std::string_view::npos || close <= open) return std::nullopt;
            cue.file = sheet.parent_path() / std::string(line.substr(open + 1, close - open - 1));
        } else if (line.size() > 6 && text::iequals(line.substr(0, 6), "TRACK ") && cue.sector_size == 0) {
            const std::string mode = text::upper(line.substr(line.rfind(' ') + 1));
            cue.sector_size = mode.ends_with("/2048") ? kSector : mode.ends_with("/2352") ? kRawSector : 0;
        }
    }
    if (cue.file.empty()) return std::nullopt;
    return cue;
}

bool is_image_name(const std::filesystem::path& path) { return has_extension(path, {".iso", ".bin", ".img", ".cue"}); }

// What the sectors are read from: bytes in memory, or a file.
struct Sectors::Impl {
    ByteView memory;
    std::optional<fs::RandomFile> file;
    std::filesystem::path path;
    std::size_t stride = kSector;
    std::uint64_t count = 0;

    Bytes bytes_at(std::uint64_t offset, std::size_t n) const {
        if (file) return file->read_at(offset, n);
        if (offset >= memory.size()) return {};
        const std::size_t take = static_cast<std::size_t>(std::min<std::uint64_t>(n, memory.size() - offset));
        return Bytes(memory.begin() + static_cast<std::ptrdiff_t>(offset), memory.begin() + static_cast<std::ptrdiff_t>(offset + take));
    }

    Bytes sector(std::uint64_t lba) const {
        if (lba >= count) return {};
        if (stride == kSector) return bytes_at(lba * kSector, kSector);
        Bytes raw = bytes_at(lba * kRawSector, kRawSector);
        if (raw.size() < kRawSector) return {};
        const std::size_t at = data_offset(raw);
        return Bytes(raw.begin() + static_cast<std::ptrdiff_t>(at), raw.begin() + static_cast<std::ptrdiff_t>(at + kSector));
    }

    // How big a sector is, from what the caller said or the first bytes.
    void lay_out(std::size_t sector_size, std::uint64_t size) {
        if (sector_size == kRawSector || (sector_size == 0 && raw_sync(bytes_at(0, kSync.size())))) stride = kRawSector;
        count = size / stride;
    }
};

Sectors::Sectors(std::shared_ptr<const Impl> impl) : impl_(std::move(impl)) {}
Sectors::Sectors(const Sectors&) = default;
Sectors& Sectors::operator=(const Sectors&) = default;
Sectors::Sectors(Sectors&&) noexcept = default;
Sectors& Sectors::operator=(Sectors&&) noexcept = default;
Sectors::~Sectors() = default;

Sectors Sectors::of(ByteView image, std::size_t sector_size) {
    auto impl = std::make_shared<Impl>();
    impl->memory = image;
    impl->lay_out(sector_size, image.size());
    return Sectors(std::move(impl));
}

Sectors Sectors::open(const std::filesystem::path& path) {
    std::filesystem::path image = path;
    std::size_t sector_size = 0;
    if (has_extension(path, {".cue"})) {
        const std::optional<Cue> cue = read_cue(path);
        if (!cue) throw FormatError(kReader, 0, "a .cue sheet that names no file");
        image = cue->file;
        sector_size = cue->sector_size;
    }
    auto impl = std::make_shared<Impl>();
    impl->file.emplace(image, fs::FileMode::Read);
    impl->path = image;
    impl->lay_out(sector_size, impl->file->size());
    return Sectors(std::move(impl));
}

bool Sectors::raw() const { return impl_->stride == kRawSector; }
std::uint64_t Sectors::count() const { return impl_->count; }
const std::filesystem::path& Sectors::file() const { return impl_->path; }
Bytes Sectors::sector(std::uint64_t lba) const { return impl_->sector(lba); }

Bytes Sectors::read(std::uint64_t lba, std::size_t length) const {
    if (!raw()) {
        // cooked: one read, cut at the image's last whole sector
        const std::uint64_t room = lba < count() ? (count() - lba) * kSector : 0;
        return impl_->bytes_at(lba * kSector, static_cast<std::size_t>(std::min<std::uint64_t>(length, room)));
    }
    Bytes out;
    out.reserve(length);
    for (std::uint64_t s = lba; out.size() < length && has(s); ++s) {
        const Bytes data = impl_->sector(s);
        out.insert(out.end(), data.begin(), data.begin() + static_cast<std::ptrdiff_t>(std::min(kSector, length - out.size())));
    }
    return out;
}

Bytes Sectors::read_at(std::uint64_t offset, std::size_t count) const {
    const std::uint64_t lba = offset / kSector;
    const std::size_t skip = static_cast<std::size_t>(offset % kSector);
    Bytes out = read(lba, skip + count);
    if (out.size() <= skip) return {};
    out.erase(out.begin(), out.begin() + static_cast<std::ptrdiff_t>(skip));
    return out;
}

}  // namespace cb::formats::disc
