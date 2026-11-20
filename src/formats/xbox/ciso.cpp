#include "formats/xbox/ciso.hpp"

#include <algorithm>
#include <format>
#include <optional>
#include <unordered_map>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "formats/media.hpp"
#include "formats/xbox/gdf.hpp"

namespace cb::formats::xbox::ciso {

namespace {

constexpr std::string_view kReader = "ciso";
constexpr std::uint32_t kCsoHeader = 0x18, kCciHeader = 0x20;
constexpr std::uint32_t kCompressedBit = 0x80000000u;
// a block is kept compressed only when it saves enough: CSO needs 12 bytes and a length word, CCI needs it under 2040
constexpr std::size_t kCsoSaving = 12;
constexpr std::size_t kCciLimit = 2040;

std::uint32_t le32(ByteView b, std::size_t at) {
    return b[at] | (b[at + 1] << 8) | (b[at + 2] << 16) | (static_cast<std::uint32_t>(b[at + 3]) << 24);
}
std::uint64_t le64(ByteView b, std::size_t at) { return le32(b, at) | (static_cast<std::uint64_t>(le32(b, at + 4)) << 32); }

[[noreturn]] void damaged(std::string_view what) { throw FormatError(kReader, 0, std::string(what)); }

void emit_length(Bytes& out, std::size_t n) {
    while (n >= 255) {
        out.push_back(255);
        n -= 255;
    }
    out.push_back(static_cast<std::uint8_t>(n));
}

}  // namespace

// ---- LZ4 blocks ------------------------------------------------------------------------------------------------------------

Bytes lz4_decode_block(ByteView src, std::size_t uncompressed_size) {
    if (uncompressed_size == 0) return {};
    if (src.empty()) damaged("empty LZ4 block");
    Bytes dst;
    dst.reserve(uncompressed_size);
    const std::size_t n = src.size();
    std::size_t i = 0;
    const auto need = [&](std::size_t at) -> std::uint8_t {
        if (at >= n) damaged("truncated LZ4 block");
        return src[at];
    };
    // a length continued by bytes of 255 until a smaller one
    const auto extend = [&](std::size_t length) {
        for (;;) {
            const std::uint8_t b = need(i++);
            length += b;
            if (b != 255) return length;
        }
    };
    while (i < n) {
        const std::uint8_t token = need(i++);
        std::size_t literals = token >> 4;
        if (literals == 15) literals = extend(literals);
        if (literals != 0) {
            if (literals > n - i) damaged("truncated LZ4 block");
            dst.insert(dst.end(), src.begin() + static_cast<std::ptrdiff_t>(i), src.begin() + static_cast<std::ptrdiff_t>(i + literals));
            i += literals;
        }
        if (i == n) break;   // the last sequence has literals only
        const std::size_t offset = need(i) | (std::size_t{need(i + 1)} << 8);
        i += 2;
        if (offset == 0) damaged("invalid zero match offset");
        std::size_t length = (token & 0x0Fu) + 4;
        if ((token & 0x0F) == 15) length = extend(length);
        if (offset > dst.size()) damaged("match before start of output");
        const std::size_t start = dst.size() - offset;
        for (std::size_t k = 0; k < length; ++k) dst.push_back(dst[start + k]);   // (a match may overlap what it copies)
        if (dst.size() > uncompressed_size) damaged(std::format("LZ4 block decoded to more than the {} bytes expected", uncompressed_size));
    }
    if (dst.size() != uncompressed_size) damaged(std::format("LZ4 block decoded to {} bytes, expected {}", dst.size(), uncompressed_size));
    return dst;
}

Bytes lz4_compress_block(ByteView s) {
    const std::size_t n = s.size();
    Bytes out;
    if (n < 13) {   // too short to hold a match: one run of literals
        out.push_back(static_cast<std::uint8_t>(std::min<std::size_t>(n, 15) << 4));
        append(out, s);
        return out;
    }
    std::unordered_map<std::uint32_t, std::size_t> last;   // the latest place of each 4 bytes (looked up by key only)
    last.reserve(n);
    const auto word = [&](std::size_t at) { return le32(s, at); };
    std::size_t anchor = 0, i = 0;
    const std::size_t limit = n - 12;
    while (i < limit) {
        const std::uint32_t q = word(i);
        const auto it = last.find(q);
        const bool have = it != last.end();
        const std::size_t candidate = have ? it->second : 0;
        last[q] = i;
        if (have && i - candidate <= 0xFFFF && word(candidate) == q) {
            std::size_t length = 4;
            const std::size_t longest = n - 5 - i;   // (the last 5 bytes are literals)
            while (length < longest && s[candidate + length] == s[i + length]) ++length;
            const std::size_t literals = i - anchor;
            out.push_back(static_cast<std::uint8_t>((std::min<std::size_t>(literals, 15) << 4) | std::min<std::size_t>(length - 4, 15)));
            if (literals >= 15) emit_length(out, literals - 15);
            out.insert(out.end(), s.begin() + static_cast<std::ptrdiff_t>(anchor), s.begin() + static_cast<std::ptrdiff_t>(i));
            out.push_back(static_cast<std::uint8_t>(i - candidate));
            out.push_back(static_cast<std::uint8_t>((i - candidate) >> 8));
            if (length - 4 >= 15) emit_length(out, length - 4 - 15);
            for (std::size_t j = i + 1; j < std::min(i + length, limit); ++j) last.emplace(word(j), j);   // (keeps an earlier place)
            i += length;
            anchor = i;
        } else {
            ++i;
        }
    }
    const std::size_t literals = n - anchor;
    out.push_back(literals < 15 ? static_cast<std::uint8_t>(literals << 4) : 0xF0);
    if (literals >= 15) emit_length(out, literals - 15);
    out.insert(out.end(), s.begin() + static_cast<std::ptrdiff_t>(anchor), s.end());
    return out;
}

// ---- reading ---------------------------------------------------------------------------------------------------------------

std::optional<Kind> kind_of(const std::filesystem::path& path) {
    const Bytes magic = fs::RandomFile(path, fs::FileMode::Read).read_at(0, 4);
    if (as_text(magic) == "CISO") return Kind::Cso;
    if (as_text(magic) == "CCIM") return Kind::Cci;
    return std::nullopt;
}

Image Image::open(const std::filesystem::path& path) {
    const std::optional<Kind> kind = kind_of(path);
    if (!kind) damaged(std::format("{} is not a CSO or CCI image", fs::to_text(path)));
    Image image(path, *kind);
    const bool cci = *kind == Kind::Cci;
    const std::uint32_t header_size = cci ? kCciHeader : kCsoHeader;
    const Bytes head = image.file_.read_at(0, header_size);
    if (head.size() < header_size) throw TruncatedError(kReader, 0, header_size, head.size());
    image.size_ = le64(head, 8);
    const std::uint64_t index_at = cci ? le64(head, 16) : kCsoHeader;
    const std::uint32_t block_size = le32(head, cci ? 24 : 16);
    if (le32(head, 4) != header_size || block_size != kBlockSize) damaged(cci ? "not a CCI image" : "not a CSO image");
    image.block_count_ = image.size_ / kBlockSize;
    const std::uint64_t file_size = image.file_.size();
    if (image.block_count_ > file_size / 4 || index_at > file_size) damaged("truncated index");   // (a size no index could describe)
    const std::size_t index_bytes = static_cast<std::size_t>((image.block_count_ + 1) * 4);
    const Bytes raw = image.file_.read_at(index_at, index_bytes);
    if (raw.size() < index_bytes) damaged("truncated index");
    image.offsets_.reserve(image.block_count_ + 1);
    image.compressed_.reserve(image.block_count_ + 1);
    for (std::uint64_t k = 0; k <= image.block_count_; ++k) {
        const std::uint32_t v = le32(raw, static_cast<std::size_t>(k * 4));
        image.offsets_.push_back(std::uint64_t{v & ~kCompressedBit} << 2);
        image.compressed_.push_back((v & kCompressedBit) != 0);
    }
    if (image.compressed_.back()) damaged("index terminator has compressed bit set");
    return image;
}

std::uint64_t Image::compressed_blocks() const {
    std::uint64_t n = 0;
    for (std::uint64_t k = 0; k < block_count_; ++k) n += compressed_[k];
    return n;
}

Bytes Image::decode_block(std::uint64_t index) {
    if (kind_ == Kind::Cso) {
        const std::uint64_t at = offsets_[index];
        if (!compressed_[index]) return file_.read_at(at, kBlockSize);
        const Bytes length_word = file_.read_at(at, 4);
        if (length_word.size() != 4) throw TruncatedError(kReader, at, 4, length_word.size());
        const std::uint32_t word = le32(length_word, 0);
        Bytes data = file_.read_at(at + 4, word & ~kCompressedBit);
        if ((word & kCompressedBit) != 0) return data;   // stored inside a compressed-index block
        return lz4_decode_block(data, kBlockSize);
    }
    const std::uint64_t stored = offsets_[index + 1] > offsets_[index] ? offsets_[index + 1] - offsets_[index] : 0;
    if (!compressed_[index] && stored == kBlockSize) return file_.read_at(offsets_[index], kBlockSize);
    const Bytes record = file_.read_at(offsets_[index], static_cast<std::size_t>(std::min<std::uint64_t>(stored, 4 * kBlockSize)));
    if (record.empty()) damaged("an empty block");
    const std::size_t padding = record[0];   // the block ends with this many padding bytes
    if (1 + padding > record.size()) damaged("a block's padding is longer than the block");
    return lz4_decode_block(ByteView(record).subspan(1, record.size() - 1 - padding), kBlockSize);
}

Bytes Image::read_at(std::uint64_t offset, std::size_t length) {
    Bytes out;
    const std::uint64_t end = std::min<std::uint64_t>(offset + length, size_);
    while (offset < end) {
        const std::uint64_t index = offset / kBlockSize;
        if (index != cached_index_) {
            cached_ = decode_block(index);
            cached_index_ = index;
        }
        const std::uint64_t within = offset % kBlockSize;
        if (within >= cached_.size()) break;   // (a block the file cut short)
        const std::uint64_t take = std::min<std::uint64_t>({kBlockSize - within, end - offset, cached_.size() - within});
        out.insert(out.end(), cached_.begin() + static_cast<std::ptrdiff_t>(within), cached_.begin() + static_cast<std::ptrdiff_t>(within + take));
        offset += take;
    }
    return out;
}

// ---- writing ---------------------------------------------------------------------------------------------------------------

namespace {

// The next 2048-byte sector of the ISO, zero-padded.
Bytes next_sector(const fs::RandomFile& iso, std::uint64_t index) {
    Bytes raw = iso.read_at(index * kBlockSize, kBlockSize);
    raw.resize(kBlockSize, 0);
    return raw;
}

std::uint64_t padded_size(const fs::RandomFile& iso) { return (iso.size() + kBlockSize - 1) / kBlockSize * kBlockSize; }

Bytes index_bytes(const std::vector<std::uint32_t>& entries) {
    io::ByteWriter w;
    for (const std::uint32_t v : entries) w.u32(v);
    return w.take();
}

io::ByteWriter header(Kind kind, std::uint64_t uncompressed, std::uint64_t index_at) {
    io::ByteWriter h;
    if (kind == Kind::Cso) {
        h.bytes(as_bytes("CISO"));
        h.u32(kCsoHeader);
        h.u64(uncompressed);
        h.u32(kBlockSize);
        h.u8(2);   // version
        h.u8(2);   // index shift
        h.u16(0);
    } else {
        h.bytes(as_bytes("CCIM"));
        h.u32(kCciHeader);
        h.u64(uncompressed);
        h.u64(index_at);
        h.u32(kBlockSize);
        h.u8(1);
        h.u8(2);
        h.u16(0);
    }
    return h;
}

}  // namespace

std::uint64_t write_cso(const std::filesystem::path& iso_path, const std::filesystem::path& out_path) {
    const fs::RandomFile iso(iso_path, fs::FileMode::Read);
    const std::uint64_t uncompressed = padded_size(iso), blocks = uncompressed / kBlockSize;
    fs::RandomFile out(out_path, fs::FileMode::Create);
    out.write_at(0, header(Kind::Cso, uncompressed, 0).data());
    std::uint64_t pos = kCsoHeader + (blocks + 1) * 4;   // (the index is written when the blocks are)
    std::vector<std::uint32_t> entries;
    entries.reserve(blocks + 1);
    for (std::uint64_t b = 0; b < blocks; ++b) {
        const Bytes raw = next_sector(iso, b);
        const Bytes packed = lz4_compress_block(raw);
        const std::uint64_t at = pos;
        if (packed.size() + 4 < kBlockSize - kCsoSaving) {
            io::ByteWriter w;
            w.u32(static_cast<std::uint32_t>(packed.size()));
            w.bytes(packed);
            out.write_at(at, w.data());
            pos += w.size();
            entries.push_back(static_cast<std::uint32_t>(at >> 2) | kCompressedBit);
        } else {
            out.write_at(at, raw);
            pos += raw.size();
            entries.push_back(static_cast<std::uint32_t>(at >> 2));
        }
        if (pos % 4 != 0) {   // blocks start on a 4-byte boundary
            const Bytes pad(4 - pos % 4, 0);
            out.write_at(pos, pad);
            pos += pad.size();
        }
    }
    entries.push_back(static_cast<std::uint32_t>(pos >> 2));
    out.write_at(kCsoHeader, index_bytes(entries));
    out.flush();
    return uncompressed;
}

std::uint64_t write_cci(const std::filesystem::path& iso_path, const std::filesystem::path& out_path) {
    const fs::RandomFile iso(iso_path, fs::FileMode::Read);
    const std::uint64_t uncompressed = padded_size(iso), blocks = uncompressed / kBlockSize;
    fs::RandomFile out(out_path, fs::FileMode::Create);
    out.write_at(0, header(Kind::Cci, uncompressed, 0).data());   // (rewritten with the index place at the end)
    std::uint64_t pos = kCciHeader;
    std::vector<std::uint32_t> entries;
    entries.reserve(blocks + 1);
    for (std::uint64_t b = 0; b < blocks; ++b) {
        const Bytes raw = next_sector(iso, b);
        const Bytes packed = lz4_compress_block(raw);
        const std::uint64_t at = pos;
        if (packed.size() < kCciLimit) {
            const std::size_t total = (1 + packed.size() + 3) & ~std::size_t{3}, padding = total - 1 - packed.size();
            io::ByteWriter w;
            w.u8(static_cast<std::uint8_t>(padding));
            w.bytes(packed);
            w.zeros(padding);
            out.write_at(at, w.data());
            pos += w.size();
            entries.push_back(static_cast<std::uint32_t>(at >> 2) | kCompressedBit);
        } else {
            out.write_at(at, raw);
            pos += raw.size();
            entries.push_back(static_cast<std::uint32_t>(at >> 2));
        }
    }
    const std::uint64_t index_at = pos;
    entries.push_back(static_cast<std::uint32_t>(index_at >> 2));
    out.write_at(index_at, index_bytes(entries));
    out.write_at(0, header(Kind::Cci, uncompressed, index_at).data());
    out.flush();
    return uncompressed;
}

std::uint64_t to_iso(const std::filesystem::path& image_path, const std::filesystem::path& out_path) {
    Image image = Image::open(image_path);
    fs::RandomFile out(out_path, fs::FileMode::Create);
    constexpr std::size_t kChunk = 4 * 1024 * 1024;
    std::uint64_t offset = 0;
    while (offset < image.size()) {
        const Bytes chunk = image.read_at(offset, kChunk);
        if (chunk.empty()) break;
        out.write_at(offset, chunk);
        offset += chunk.size();
    }
    out.flush();
    return offset;
}

// ---- the command -----------------------------------------------------------------------------------------------------------

std::string_view usage() {
    return "usage:\n"
           "  ciso info    <image.cso|.cci>\n"
           "  ciso list    <image.cso|.cci>\n"
           "  ciso extract <image.cso|.cci> <outdir>\n"
           "  ciso toiso   <image.cso|.cci> <out.iso>\n"
           "  ciso tocso   <in.iso> <out.cso>\n"
           "  ciso tocci   <in.iso> <out.cci>\n";
}

int command(std::span<const std::string> args) {
    if (args.empty() || args[0] == "-h" || args[0] == "--help") {
        out_text(usage());
        return 0;
    }
    const std::string& op = args[0];
    if (args.size() < 2) {
        out_text(usage());
        return 2;
    }
    const std::filesystem::path first = fs::from_text(args[1]);
    if (op == "tocso" || op == "tocci" || op == "toiso") {
        if (args.size() < 3) {
            out_text(usage());
            return 2;
        }
        const std::filesystem::path second = fs::from_text(args[2]);
        if (op == "tocso") {
            out("wrote CSO ({} MB uncompressed) -> {}\n", write_cso(first, second) >> 20, args[2]);
        } else if (op == "tocci") {
            out("wrote CCI ({} MB uncompressed) -> {}\n", write_cci(first, second) >> 20, args[2]);
        } else {
            out("wrote {} bytes -> {}\n", to_iso(first, second), args[2]);
        }
        return 0;
    }
    if (op != "info" && op != "list" && op != "extract") {
        out_text(usage());
        return 2;
    }
    auto image = std::make_shared<Image>(Image::open(first));
    if (op == "info") {
        out("{} image: {}\n", image->kind() == Kind::Cso ? "CSO" : "CCI", args[1]);
        out("uncompressed: {} bytes ({} blocks); {} compressed, {} raw; on-disk {} bytes\n", image->size(), image->block_count(),
            image->compressed_blocks(), image->block_count() - image->compressed_blocks(), std::filesystem::file_size(first));
        return 0;
    }
    gdf::Disc disc = gdf::Disc::open(image);
    if (op == "list") {
        for (const gdf::Entry& e : disc.files()) out("  {:>12}  {}\n", e.size, e.path);
        return 0;
    }
    if (args.size() < 3) {
        out_text(usage());
        return 2;
    }
    for (const gdf::Entry& e : disc.files()) {
        const std::filesystem::path dest = fs::from_text(args[2]) / media::safe_relative_path(e.path);
        std::filesystem::create_directories(dest.parent_path());
        fs::write_file(dest, disc.read(e));
    }
    out("extracted -> {}\n", args[2]);
    return 0;
}

}  // namespace cb::formats::xbox::ciso
