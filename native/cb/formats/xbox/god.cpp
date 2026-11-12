// cb/formats/xbox/god.cpp -- see god.hpp.
#include "cb/formats/xbox/god.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <map>
#include <memory>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/random_file.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xbox::god {
namespace {

// The header file's layout (offsets into the CON/LIVE header).
constexpr std::size_t kHeaderHash = 0x32C;          // SHA-1 over [kHashedFrom, kHashedFrom + kHashedLength)
constexpr std::size_t kHashedFrom = 0x344;
constexpr std::size_t kHashedLength = 0xACBC;
constexpr std::size_t kContentType = 0x344;         // u32, big-endian
constexpr std::size_t kMediaId = 0x354;             // u32 BE
constexpr std::size_t kTitleId = 0x360;             // u32 BE
constexpr std::size_t kDescriptorFlags[] = {0x35B, 0x35F, 0x391};   // cleared in a GoD header
constexpr std::size_t kFirstMasterHash = 0x37D;     // 20 bytes
constexpr std::size_t kBlocksAllocated = 0x392;     // u24 BE
constexpr std::size_t kBlocksFree = 0x395;          // u16 BE, zero
constexpr std::size_t kPartCount = 0x3A0;           // u32 LE
constexpr std::size_t kPartsSize = 0x3A4;           // u32 BE, in units of 0x100 bytes
constexpr std::size_t kDisplayName = 0x411;         // UTF-16BE, up to 0x7E bytes
constexpr std::size_t kTitleName = 0x1691;          // the same text
constexpr std::size_t kNameBytes = 0x7E;
constexpr std::size_t kMinHeader = kTitleName + kNameBytes;

// What the NXE2GOD transform sets in an installed game's header.
constexpr std::size_t kInstallMinHeader = 0x413;
constexpr std::size_t kInstallClearFrom = 0x3FD, kInstallClearTo = 0x412;
constexpr std::size_t kInstallFfFrom = 0x22C, kInstallFfTo = 0x234;

void put_be32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}
void put_le32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
}

[[noreturn]] void usage_error(const std::string& what) { throw UsageError("god: " + what); }

// The header's hash: SHA-1 of the span from kHashedFrom to its end (or kHashedLength, whichever is less).
void rehash(Bytes& header) {
    const std::size_t length = std::min(kHashedLength, header.size() - kHashedFrom);
    const cb::crypto::Sha1Digest digest = cb::crypto::sha1(ByteView(header).subspan(kHashedFrom, length));
    std::copy(digest.begin(), digest.end(), header.begin() + static_cast<std::ptrdiff_t>(kHeaderHash));
}

std::filesystem::path numbered(const std::filesystem::path& dir, std::size_t index) {
    return dir / std::format("Data{:04d}", index);
}

}  // namespace

// ---- SvodReader ----------------------------------------------------------------------------------------------------------

struct SvodReader::Impl {
    mutable std::map<std::size_t, fs::RandomFile> open_files;   // the parts read so far
    fs::RandomFile& file(const std::filesystem::path& path, std::size_t index) const {
        auto it = open_files.find(index);
        if (it == open_files.end()) it = open_files.emplace(index, fs::RandomFile(path, fs::FileMode::Read)).first;
        return it->second;
    }
};

SvodReader::SvodReader() : impl_(std::make_unique<Impl>()) {}
SvodReader::SvodReader(SvodReader&&) noexcept = default;
SvodReader& SvodReader::operator=(SvodReader&&) noexcept = default;
SvodReader::~SvodReader() = default;

SvodReader SvodReader::open(const std::filesystem::path& data_dir) {
    std::error_code ec;
    std::filesystem::directory_iterator it(data_dir, ec);
    if (ec) throw NotFoundError(data_dir, ec.value(), "cannot list the GoD data folder");
    SvodReader reader;
    for (const auto& entry : it) {
        const std::string name = fs::to_text(entry.path().filename());
        if (name.size() >= 4 && text::lower(name.substr(0, 4)) == "data" && entry.is_regular_file(ec)) reader.parts_.push_back(entry.path());
    }
    // the part order is the name order (Data0000, Data0001, ...)
    std::sort(reader.parts_.begin(), reader.parts_.end(),
              [](const std::filesystem::path& a, const std::filesystem::path& b) { return fs::to_text(a.filename()) < fs::to_text(b.filename()); });
    if (reader.parts_.empty()) throw FormatError("god", 0, "no Data#### part files in " + fs::to_text(data_dir));
    return reader;
}

std::uint64_t SvodReader::size() const {
    std::uint64_t total = 0;
    for (const auto& part : parts_) {
        // a part is a master block, then subparts of (hash block + data blocks); a last subpart may be cut short
        const std::uint64_t physical = fs::file_size(part);
        if (physical < kBlock) continue;
        const std::uint64_t body = physical - kBlock;
        const std::uint64_t subparts = body / (kBlock + kSubpartSize);
        const std::uint64_t rest = body - subparts * (kBlock + kSubpartSize);
        total += subparts * kSubpartSize + (rest > kBlock ? rest - kBlock : 0);
    }
    return total;
}

Bytes SvodReader::read_at(std::uint64_t offset, std::size_t length) const {
    Bytes out;
    while (out.size() < length) {
        const std::uint64_t at = offset + out.size();
        const std::uint64_t block = at / kBlock, in_block = at % kBlock;
        const std::uint64_t part = block / kBlocksPerPart, in_part = block % kBlocksPerPart;
        if (part >= parts_.size()) break;
        const std::uint64_t subpart = in_part / kBlocksPerSubpart, in_subpart = in_part % kBlocksPerSubpart;
        // where the block lies in the part file: the master block, the subparts before it, this subpart's hash block
        const std::uint64_t physical = kBlock + subpart * (kBlock + kSubpartSize) + kBlock + in_subpart * kBlock + in_block;
        // the blocks of a subpart are contiguous on disk: take what is left of this one in a single read
        const std::uint64_t run = (kBlocksPerSubpart - in_subpart) * kBlock - in_block;
        const std::size_t take = static_cast<std::size_t>(std::min<std::uint64_t>(run, length - out.size()));
        const Bytes chunk = impl_->file(parts_[static_cast<std::size_t>(part)], static_cast<std::size_t>(part)).read_at(physical, take);
        append(out, chunk);
        if (chunk.size() < take) break;   // the part ends here
    }
    return out;
}

// ---- writing -------------------------------------------------------------------------------------------------------------

namespace {

struct PartResult {
    std::uint64_t size = 0;                // the part file's bytes
    cb::crypto::Sha1Digest master_hash{};      // SHA-1 of its master hash block
};

// One part: the volume blocks [first_block, first_block + count) as hash blocks and data, written to `path`.
PartResult write_part(const VolumeReader& read, std::uint64_t first_block, std::uint64_t count, const std::filesystem::path& path) {
    fs::RandomFile out(path, fs::FileMode::Create);
    Bytes master(kBlock, 0);
    std::uint64_t written = kBlock;   // the master block is written last, once its subparts are known
    for (std::uint64_t subpart = 0; subpart < kSubpartsPerPart; ++subpart) {
        const std::uint64_t base = subpart * kBlocksPerSubpart;
        if (base >= count) break;
        const std::uint64_t filled = std::min(kBlocksPerSubpart, count - base);
        Bytes data = read((first_block + base) * kBlock, static_cast<std::size_t>(filled * kBlock));
        data.resize(static_cast<std::size_t>(filled * kBlock), 0);   // (a short answer reads as zeros)
        Bytes hashes(kBlock, 0);
        for (std::uint64_t k = 0; k < filled; ++k) {
            const cb::crypto::Sha1Digest h = cb::crypto::sha1(ByteView(data).subspan(static_cast<std::size_t>(k * kBlock), kBlock));
            std::memcpy(hashes.data() + k * 20, h.data(), 20);
        }
        const cb::crypto::Sha1Digest sub = cb::crypto::sha1(hashes);
        std::memcpy(master.data() + subpart * 20, sub.data(), 20);
        out.write_at(written, hashes);
        out.write_at(written + kBlock, data);
        written += kBlock + data.size();
        if (filled < kBlocksPerSubpart) break;
    }
    out.write_at(0, master);
    out.flush();
    return {written, cb::crypto::sha1(master)};
}

}  // namespace

Bytes make_header(ByteView header_template, const Title& title, std::uint64_t blocks_allocated, std::uint64_t part_count,
                  std::uint64_t parts_total_size, ByteView first_master_hash) {
    if (header_template.size() < kMinHeader) throw FormatError("god", header_template.size(), "the header template is too short");
    if (first_master_hash.size() != 20) usage_error("the master hash is a 20-byte SHA-1");
    if (blocks_allocated > 0xffffff) usage_error("the volume has more blocks than the header counts (24 bits)");
    if (part_count > 0xffffffffu) usage_error("too many parts");
    if (parts_total_size / 0x100 > 0xffffffffu) usage_error("the parts are bigger than the header counts");

    Bytes b(header_template.begin(), header_template.end());
    put_be32(b, kContentType, title.content_type);
    put_be32(b, kMediaId, title.media_id);
    put_be32(b, kTitleId, title.title_id);
    b[kBlocksAllocated] = static_cast<std::uint8_t>(blocks_allocated >> 16);
    b[kBlocksAllocated + 1] = static_cast<std::uint8_t>(blocks_allocated >> 8);
    b[kBlocksAllocated + 2] = static_cast<std::uint8_t>(blocks_allocated);
    b[kBlocksFree] = 0;
    b[kBlocksFree + 1] = 0;
    put_le32(b, kPartCount, static_cast<std::uint32_t>(part_count));
    put_be32(b, kPartsSize, static_cast<std::uint32_t>(parts_total_size / 0x100));
    std::copy(first_master_hash.begin(), first_master_hash.end(), b.begin() + static_cast<std::ptrdiff_t>(kFirstMasterHash));
    if (!title.title.empty()) {
        // UTF-16BE, as many whole code units as fit (a surrogate pair is not cut in half)
        Bytes le = text::to_utf16le(title.title);
        std::size_t units = std::min<std::size_t>(le.size() / 2, kNameBytes / 2);
        if (units > 0 && units < le.size() / 2) {
            const std::uint8_t high_byte = le[(units - 1) * 2 + 1];
            if (high_byte >= 0xd8 && high_byte < 0xdc) --units;   // the last unit kept is a high surrogate whose partner is cut off
        }
        Bytes be(units * 2);
        for (std::size_t i = 0; i < units; ++i) {
            be[i * 2] = le[i * 2 + 1];
            be[i * 2 + 1] = le[i * 2];
        }
        std::copy(be.begin(), be.end(), b.begin() + static_cast<std::ptrdiff_t>(kDisplayName));
        std::copy(be.begin(), be.end(), b.begin() + static_cast<std::ptrdiff_t>(kTitleName));
    }
    for (const std::size_t at : kDescriptorFlags) b[at] = 0;
    rehash(b);
    return b;
}

WriteResult write_god(const VolumeReader& read, std::uint64_t volume_size, const std::filesystem::path& out_dir,
                      ByteView header_template, const Title& title) {
    if (volume_size == 0) usage_error("the volume is empty");
    const std::uint64_t total_blocks = (volume_size + kBlock - 1) / kBlock;
    const std::uint64_t part_count = (total_blocks + kBlocksPerPart - 1) / kBlocksPerPart;

    const std::filesystem::path base = out_dir / std::format("{:08X}", title.title_id) / std::format("{:08X}", title.content_type);
    const std::string media = std::format("{:08X}", title.media_id);
    WriteResult result;
    result.header = base / media;
    result.data_dir = base / (media + ".data");
    fs::make_dirs(result.data_dir);

    std::uint64_t parts_total_size = 0;
    cb::crypto::Sha1Digest first_master{};
    for (std::uint64_t index = 0; index < part_count; ++index) {
        const std::uint64_t first = index * kBlocksPerPart;
        const PartResult part = write_part(read, first, std::min(kBlocksPerPart, total_blocks - first), numbered(result.data_dir, static_cast<std::size_t>(index)));
        parts_total_size += part.size;
        if (index == 0) first_master = part.master_hash;
    }
    fs::write_file(result.header, make_header(header_template, title, total_blocks, part_count, parts_total_size, first_master));
    result.part_count = static_cast<std::size_t>(part_count);
    return result;
}

// ---- NXE-installed games -------------------------------------------------------------------------------------------------

Bytes install_to_god(ByteView header) {
    if (header.size() < kInstallMinHeader) throw FormatError("god", header.size(), "not an STFS package header (need >= 0x413 bytes)");
    Bytes b(header.begin(), header.end());
    std::memcpy(b.data(), "LIVE", 4);
    std::fill(b.begin() + kInstallFfFrom, b.begin() + kInstallFfTo, std::uint8_t{0xff});
    // the content type is 0x00007000; the neighbouring descriptor bytes are the NXE2GOD constants
    b[0x340] = 0x00;
    b[0x341] = 0x00;
    b[0x342] = 0xad;
    b[0x343] = 0x0e;
    b[0x346] = 0x70;
    b[0x379] = 0x24;
    b[0x37A] = 0x05;
    b[0x37B] = 0x05;
    b[0x37C] = 0x11;
    std::fill(b.begin() + kInstallClearFrom, b.begin() + kInstallClearTo, std::uint8_t{0});
    rehash(b);
    return b;
}

bool is_nxe_install(ByteView header) {
    return header.size() >= 0x348 && header[0x344] == 0 && header[0x345] == 0 && header[0x346] == 0x40 && header[0x347] == 0;
}

// ---- the command ---------------------------------------------------------------------------------------------------------

std::string_view usage() {
    return "usage: god god2iso <god.data-dir> <out.iso>  |  install2god <con-header> <out-header>\n";
}

int command(std::span<const std::string> args) {
    if (args.empty() || args[0] == "-h" || args[0] == "--help") {
        out_text(usage());
        return 0;
    }
    const std::string& op = args[0];
    if (op == "install2god") {
        if (args.size() < 3) throw UsageError("usage: god install2god <installed-STFS-header> <out-GoD-header>");
        const Bytes header = fs::read_file(fs::from_text(args[1]));
        if (!is_nxe_install(header)) out("warning: header content-type is not 0x00004000 (NXE-installed); converting anyway\n");
        fs::write_file(fs::from_text(args[2]), install_to_god(header));
        out("GoD/LIVE header written -> {} (copy the sibling .data folder alongside)\n", args[2]);
        return 0;
    }
    if (op == "god2iso") {
        if (args.size() < 3) throw UsageError("usage: god2iso <god.data-dir> <out.iso>");
        const SvodReader volume = SvodReader::open(fs::from_text(args[1]));
        const std::uint64_t total = volume.size();
        // the volume goes out 8 MiB at a time
        constexpr std::size_t kChunk = 8 * 1024 * 1024;
        fs::RandomFile iso(fs::from_text(args[2]), fs::FileMode::Create);
        std::uint64_t written = 0;
        while (written < total) {
            const Bytes chunk = volume.read_at(written, static_cast<std::size_t>(std::min<std::uint64_t>(kChunk, total - written)));
            if (chunk.empty()) break;
            iso.write_at(written, chunk);
            written += chunk.size();
        }
        iso.flush();
        out("wrote {} bytes -> {}\n", written, args[2]);
        return 0;
    }
    throw UsageError("god: '" + op + "' is not an operation of this tool (god2iso, install2god)\n" + std::string(usage()));
}

}  // namespace cb::formats::xbox::god
