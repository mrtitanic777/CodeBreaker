#include "cb/formats/probe.hpp"

#include <algorithm>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::formats {

std::string extension_of(const std::filesystem::path& path) {
    return text::lower(fs::to_text(path.extension()));
}

bool has_extension(const std::filesystem::path& path, std::initializer_list<std::string_view> extensions) {
    const std::string ext = extension_of(path);
    for (std::string_view e : extensions) {
        if (ext == e) return true;
    }
    return false;
}

std::optional<Bytes> read_prefix(const std::filesystem::path& path, std::uint64_t offset, std::size_t count) {
    try {
        // Mapped, so that a probe of an ISO or a big ROM touches only the pages it reads.
        const fs::MappedFile file(path);
        const ByteView all = file.view();
        if (offset >= all.size()) return Bytes{};
        const std::size_t n = std::min<std::uint64_t>(count, all.size() - offset);
        return Bytes(all.begin() + static_cast<std::ptrdiff_t>(offset),
                     all.begin() + static_cast<std::ptrdiff_t>(offset + n));
    } catch (const IoError&) {
        return std::nullopt;
    }
}

std::optional<std::uint64_t> size_of(const std::filesystem::path& path) {
    try {
        return fs::file_size(path);
    } catch (const IoError&) {
        return std::nullopt;
    }
}

}  // namespace cb::formats
