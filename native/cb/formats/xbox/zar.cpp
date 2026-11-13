#include "cb/formats/xbox/zar.hpp"

#include <algorithm>
#include <cstdlib>
#include <format>

#include "cb/core/dynlib.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"

namespace cb::formats::xbox::zar {

namespace {

constexpr std::string_view kReader = "zar";
constexpr std::size_t kFooterSize = 144;
constexpr std::uint32_t kFooterMagic = 0x169F52D6u;
constexpr std::uint32_t kFooterVersion = 0x61BF3A01u;
constexpr std::size_t kEntriesPerRecord = 16;
constexpr std::size_t kRecordSize = 8 + 2 * kEntriesPerRecord;   // 40
constexpr std::size_t kEntrySize = 16;
constexpr std::uint32_t kRootNameOffset = 0x7FFFFFFFu;
constexpr std::size_t kCacheBlocks = 64;

[[noreturn]] void damaged(std::string_view what) { throw FormatError(kReader, 0, std::string(what)); }

std::uint64_t be64(ByteView b, std::size_t at) {
    std::uint64_t v = 0;
    for (std::size_t i = 0; i < 8; ++i) v = (v << 8) | b[at + i];
    return v;
}
std::uint32_t be32(ByteView b, std::size_t at) {
    return (std::uint32_t{b[at]} << 24) | (std::uint32_t{b[at + 1]} << 16) | (std::uint32_t{b[at + 2]} << 8) | b[at + 3];
}
std::uint16_t be16(ByteView b, std::size_t at) { return static_cast<std::uint16_t>((b[at] << 8) | b[at + 1]); }

// The reference implementation folds ASCII A-Z only.
std::string fold(std::string_view s) {
    std::string out(s);
    for (char& c : out) {
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
    }
    return out;
}

constexpr bool is_file(const std::array<std::uint32_t, 4>& e) { return (e[0] & 0x80000000u) != 0; }
constexpr std::uint64_t file_offset(const std::array<std::uint32_t, 4>& e) { return e[1] | (std::uint64_t{e[3] & 0xFFFFu} << 32); }
constexpr std::uint64_t file_size(const std::array<std::uint32_t, 4>& e) { return e[2] | (std::uint64_t{e[3] >> 16} << 32); }

// libzstd through DynLib.
class LibZstd final : public ZstdCodec {
public:
    explicit LibZstd(DynLib lib) : lib_(std::move(lib)) {
        decompress_ = lib_.function<DecompressFn>("ZSTD_decompress");
        is_error_ = lib_.function<IsErrorFn>("ZSTD_isError");
        error_name_ = lib_.function<ErrorNameFn>("ZSTD_getErrorName");
    }
    Bytes decompress(ByteView frame, std::size_t expected_size) override {
        Bytes out(expected_size);
        const std::size_t n = decompress_(out.data(), out.size(), frame.data(), frame.size());
        if (is_error_(n) != 0) damaged(std::format("zstd decompression failed: {}", error_name_(n)));
        if (n != expected_size) damaged(std::format("a block decompressed to {} bytes, expected {}", n, expected_size));
        return out;
    }

private:
    using DecompressFn = std::size_t (*)(void*, std::size_t, const void*, std::size_t);
    using IsErrorFn = unsigned (*)(std::size_t);
    using ErrorNameFn = const char* (*)(std::size_t);
    DynLib lib_;
    DecompressFn decompress_ = nullptr;
    IsErrorFn is_error_ = nullptr;
    ErrorNameFn error_name_ = nullptr;
};

}  // namespace

std::shared_ptr<ZstdCodec> load_zstd() {
    try {
        if (const std::optional<std::string> named = fs::get_env("CODEBREAKER_ZSTD_DLL"); named && !named->empty()) {
            return std::make_shared<LibZstd>(DynLib::load(fs::from_text(*named)));
        }
        for (const char* name : {"libzstd.dll", "zstd.dll"}) {
            try {
                return std::make_shared<LibZstd>(DynLib::load_beside_exe(name));
            } catch (const NotFoundError&) {   // (the next name)
            }
        }
        return std::make_shared<LibZstd>(DynLib::load(std::filesystem::path("libzstd.dll")));   // on the search path
    } catch (const Error&) {
        return nullptr;   // not found, or not loadable: the caller says what it could not read
    }
}

Reader Reader::open(const std::filesystem::path& path, std::shared_ptr<ZstdCodec> codec) {
    Reader r(fs::RandomFile(path, fs::FileMode::Read));
    r.codec_ = std::move(codec);
    r.tried_codec_ = r.codec_ != nullptr;
    const std::uint64_t size = r.file_.size();
    if (size <= kFooterSize) damaged("file too small to be a ZArchive");
    const Bytes footer = r.file_.read_at(size - kFooterSize, kFooterSize);
    if (footer.size() != kFooterSize) damaged("short read on the footer");
    struct Section {
        std::uint64_t offset, size;
    } sections[6];
    for (std::size_t i = 0; i < 6; ++i) sections[i] = {be64(footer, i * 16), be64(footer, i * 16 + 8)};
    const std::uint64_t total = be64(footer, 12 * 8 + 32);
    const std::uint32_t version = be32(footer, 12 * 8 + 32 + 8), magic = be32(footer, 12 * 8 + 32 + 12);
    if (magic != kFooterMagic) throw FormatError(kReader, size - 4, std::format("bad footer magic {:#010x}", magic));
    if (version != kFooterVersion) throw UnsupportedError(kReader, size - 8, std::format("unsupported version {:#010x}", version));
    if (total != size) damaged(std::format("the footer's total size {} does not match the file size {}", total, size));
    for (const Section& s : sections) {
        if (s.offset > size || s.size > size - s.offset) damaged("a section exceeds the file");
    }
    r.data_offset_ = sections[0].offset;
    r.data_size_ = sections[0].size;
    const Section records = sections[1], names = sections[2], tree = sections[3];
    if (records.size > 0xFFFFFFFFull || names.size > 0x7FFFFFFFull || tree.size > 0xFFFFFFFFull) damaged("a section size is out of range");
    if (records.size == 0 || records.size % kRecordSize != 0) damaged(std::format("invalid offset-record section size {}", records.size));
    if (tree.size == 0 || tree.size % kEntrySize != 0) damaged(std::format("invalid file-tree section size {}", tree.size));

    // the offset records: each block's place within the compressed data and its size
    const Bytes raw_records = r.file_.read_at(records.offset, static_cast<std::size_t>(records.size));
    if (raw_records.size() != records.size) damaged("short read on the offset records");
    for (std::size_t rec = 0; rec < raw_records.size(); rec += kRecordSize) {
        std::uint64_t offset = be64(raw_records, rec);
        for (std::size_t k = 0; k < kEntriesPerRecord; ++k) {
            const std::uint64_t compressed = std::uint64_t{be16(raw_records, rec + 8 + k * 2)} + 1;
            r.block_offset_.push_back(offset);
            r.block_size_.push_back(compressed);
            offset += compressed;
        }
    }
    r.names_ = r.file_.read_at(names.offset, static_cast<std::size_t>(names.size));
    if (r.names_.size() != names.size) damaged("short read on the name table");
    const Bytes raw_tree = r.file_.read_at(tree.offset, static_cast<std::size_t>(tree.size));
    if (raw_tree.size() != tree.size) damaged("short read on the file tree");
    for (std::size_t at = 0; at < raw_tree.size(); at += kEntrySize) {
        r.tree_.push_back({be32(raw_tree, at), be32(raw_tree, at + 4), be32(raw_tree, at + 8), be32(raw_tree, at + 12)});
    }
    const std::uint32_t root = r.tree_[0][0];
    if ((root & 0x80000000u) != 0) damaged("the first file-tree entry is not a directory");
    if ((root & 0x7FFFFFFFu) != kRootNameOffset && !r.name_at(root & 0x7FFFFFFFu).empty()) damaged("the root node must not have a name");
    return r;
}

std::string Reader::name_at(std::uint32_t offset) const {
    if (offset == kRootNameOffset || offset >= names_.size()) return {};
    std::size_t at = offset;
    const std::uint8_t header = names_[at];
    std::size_t length = header & 0x7F;
    if ((header & 0x80) != 0) {   // a 2-byte header: the second byte carries bits 7 up of the length
        if (at + 1 >= names_.size()) return {};
        length |= std::size_t{names_[at + 1]} << 7;
        at += 2;
    } else {
        at += 1;
    }
    if (at + length > names_.size()) return {};
    return std::string(reinterpret_cast<const char*>(names_.data()) + at, length);   // (the bytes as stored)
}

std::vector<Reader::Node> Reader::walk() const {
    std::vector<Node> out;
    std::vector<std::pair<std::string, std::size_t>> stack = {{"", 0}};
    std::vector<bool> entered(tree_.size(), false);
    while (!stack.empty()) {
        auto [prefix, index] = std::move(stack.back());
        stack.pop_back();
        if (entered[index]) damaged("the file tree leads back into a folder it has entered");
        entered[index] = true;
        const auto& entry = tree_[index];
        const std::uint64_t first = entry[1], count = entry[2];
        if (first + count > tree_.size()) damaged("a directory's child range is out of bounds");
        std::vector<std::pair<std::string, std::size_t>> pending;
        for (std::uint64_t i = first; i < first + count; ++i) {
            const auto& child = tree_[static_cast<std::size_t>(i)];
            const std::string name = name_at(child[0] & 0x7FFFFFFFu);
            if (name.empty()) damaged(std::format("file-tree entry {} has an invalid name", i));
            std::string child_path = prefix.empty() ? name : prefix + "/" + name;
            out.push_back({child_path, static_cast<std::size_t>(i), is_file(child)});
            if (!is_file(child)) pending.emplace_back(std::move(child_path), static_cast<std::size_t>(i));
        }
        for (auto it = pending.rbegin(); it != pending.rend(); ++it) stack.push_back(std::move(*it));   // (so they come out in stored order)
    }
    return out;
}

std::vector<FileInfo> Reader::files() {
    std::vector<FileInfo> out;
    for (const Node& n : walk()) {
        if (n.is_file) out.push_back({n.path, file_size(tree_[n.index])});
    }
    return out;
}

std::vector<std::string> Reader::directories() {
    std::vector<std::string> out;
    for (const Node& n : walk()) {
        if (!n.is_file) out.push_back(n.path);
    }
    return out;
}

// A path resolved to a tree index the way the reference reader's LookUp does: '/' or '\' between names, empty parts skipped, names
// compared without regard to ASCII case.
std::size_t Reader::lookup(std::string_view path) const {
    const auto not_found = [&] { return NotFoundError(std::filesystem::path(std::string(path)), 0, "not found in the archive"); };
    std::string slashed(path);
    std::ranges::replace(slashed, '\\', '/');
    std::size_t current = 0;
    std::size_t at = 0;
    while (at <= slashed.size()) {
        const std::size_t end = std::min(slashed.find('/', at), slashed.size());
        const std::string part = slashed.substr(at, end - at);
        at = end + 1;
        if (part.empty()) continue;
        const auto& entry = tree_[current];
        if (is_file(entry)) throw not_found();
        const std::string want = fold(part);
        bool found = false;
        for (std::uint64_t i = entry[1]; i < std::uint64_t{entry[1]} + entry[2] && i < tree_.size(); ++i) {
            if (fold(name_at(tree_[static_cast<std::size_t>(i)][0] & 0x7FFFFFFFu)) == want) {
                current = static_cast<std::size_t>(i);
                found = true;
                break;
            }
        }
        if (!found) throw not_found();
    }
    return current;
}

std::uint64_t Reader::size(std::string_view path) {
    const auto& entry = tree_[lookup(path)];
    if (!is_file(entry)) throw NotFoundError(std::filesystem::path(std::string(path)), 0, "not a file");
    return file_size(entry);
}

const Bytes& Reader::block(std::uint64_t index) {
    if (const auto it = cached_.find(index); it != cached_.end()) {
        cache_.splice(cache_.begin(), cache_, it->second);   // the newest
        return it->second->second;
    }
    if (index >= block_offset_.size()) damaged(std::format("block index {} is out of range", index));
    const std::uint64_t offset = block_offset_[index], stored = block_size_[index];
    if (offset > data_size_ || stored > data_size_ - offset) damaged(std::format("block {} is outside the compressed data", index));
    const Bytes raw = file_.read_at(data_offset_ + offset, static_cast<std::size_t>(stored));
    if (raw.size() != stored) damaged(std::format("short read on block {}", index));
    Bytes data;
    if (stored == kBlockSize) {
        data = raw;   // stored: compressing did not help
    } else {
        if (!tried_codec_) {
            codec_ = load_zstd();
            tried_codec_ = true;
        }
        if (!codec_) {
            throw UnsupportedError(kReader, offset, "a compressed block needs libzstd, which was not found (libzstd.dll beside the program, "
                                                    "or the file CODEBREAKER_ZSTD_DLL names)");
        }
        data = codec_->decompress(raw, kBlockSize);
    }
    cache_.emplace_front(index, std::move(data));
    cached_[index] = cache_.begin();
    if (cache_.size() > kCacheBlocks) {
        cached_.erase(cache_.back().first);
        cache_.pop_back();
    }
    return cache_.front().second;
}

Bytes Reader::read_range(std::uint64_t offset, std::uint64_t length) {
    const std::uint64_t available = block_offset_.size() * std::uint64_t{kBlockSize};
    if (offset > available || length > available - offset) damaged("a file runs past the archive's data");
    Bytes out;
    while (length > 0) {
        const std::uint64_t index = offset / kBlockSize, within = offset % kBlockSize;
        const std::uint64_t step = std::min<std::uint64_t>(length, kBlockSize - within);
        const Bytes& b = block(index);
        out.insert(out.end(), b.begin() + static_cast<std::ptrdiff_t>(within), b.begin() + static_cast<std::ptrdiff_t>(within + step));
        offset += step;
        length -= step;
    }
    return out;
}

Bytes Reader::read(std::string_view path) {
    const auto& entry = tree_[lookup(path)];
    if (!is_file(entry)) throw NotFoundError(std::filesystem::path(std::string(path)), 0, "not a file");
    return read_range(file_offset(entry), file_size(entry));
}

}  // namespace cb::formats::xbox::zar
