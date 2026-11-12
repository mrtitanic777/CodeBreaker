// cb/formats/xbox/crypto/nand_tool.cpp -- see nand_tool.hpp.
#include "cb/formats/xbox/crypto/nand_tool.hpp"

#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"

namespace cb::formats::xbox::crypto::nand_tool {
namespace {

constexpr std::uint32_t kEccMask = 0x3ffffff;        // 26 bits
constexpr std::uint32_t kEccPolynomial = 0x6954559;
constexpr unsigned kEccBits = 0x1066;                // 4096 data bits, then the first 102 bits of the spare
constexpr unsigned kEccSpareStart = 0x1000;

[[noreturn]] void usage_error(const std::string& what) { throw UsageError("nand_tool: " + what); }

// Where each field lies in the 16 bytes, per layout.
struct SparePositions {
    std::size_t block_id_low, id_nibbles;       // block id's low byte; the byte holding the id's top 4 bits (low nibble) and fs_unused_0 (high)
    std::array<std::size_t, 4> sequence;
    std::size_t bad_block, size_low, size_high, page_count;
    std::array<std::size_t, 2> unused_1;
    std::size_t type_and_ecc;                   // block type (low 6 bits) and the two lowest ECC bits (top 2 bits)
    std::size_t ecc_1st, ecc_2nd, ecc_3rd;      // ECC bits 2..9, 10..17, 18..25
};

const SparePositions& positions(SpareLayout layout) {
    static const SparePositions small_block{0, 1, {2, 3, 4, 6}, 5, 7, 8, 9, {10, 11}, 12, 13, 14, 15};
    static const SparePositions big_on_small{1, 2, {0, 3, 4, 6}, 5, 7, 8, 9, {10, 11}, 12, 13, 14, 15};
    static const SparePositions big_block{1, 2, {5, 3, 4, 6}, 0, 7, 8, 9, {10, 11}, 12, 13, 14, 15};
    switch (layout) {
        case SpareLayout::SmallBlock: return small_block;
        case SpareLayout::BigOnSmall: return big_on_small;
        case SpareLayout::BigBlock: return big_block;
    }
    return small_block;
}

std::uint32_t load_u32(ByteView v, std::size_t at) {
    return static_cast<std::uint32_t>(v[at]) | static_cast<std::uint32_t>(v[at + 1]) << 8 | static_cast<std::uint32_t>(v[at + 2]) << 16 |
           static_cast<std::uint32_t>(v[at + 3]) << 24;
}

void check_page(ByteView data, ByteView spare) {
    if (data.size() != kPageSize) usage_error(std::format("a page is {} data bytes, got {}", kPageSize, data.size()));
    if (spare.size() != kSpareSize) usage_error(std::format("a spare is {} bytes, got {}", kSpareSize, spare.size()));
}

// "[1, 2, 3]"
std::string index_list(const std::vector<std::size_t>& v, std::size_t count) {
    std::string out = "[";
    for (std::size_t i = 0; i < std::min(count, v.size()); ++i) out += std::format("{}{}", i ? ", " : "", v[i]);
    return out + "]";
}

}  // namespace

// ---- the spare ---------------------------------------------------------------------------------------------------------

BlockSpare parse_spare(SpareLayout layout, ByteView spare) {
    if (spare.size() != kSpareSize) throw FormatError("nand_tool", 0, std::format("a spare is {} bytes, got {}", kSpareSize, spare.size()));
    const SparePositions& p = positions(layout);
    BlockSpare out;
    out.block_id = static_cast<std::uint16_t>(((spare[p.id_nibbles] & 0x0f) << 8) | spare[p.block_id_low]);
    out.fs_unused_0 = static_cast<std::uint8_t>(spare[p.id_nibbles] >> 4);
    for (std::size_t i = 0; i < 4; ++i) out.fs_sequence[i] = spare[p.sequence[i]];
    out.bad_block = spare[p.bad_block];
    out.fs_size = static_cast<std::uint16_t>((spare[p.size_high] << 8) | spare[p.size_low]);
    out.fs_page_count = spare[p.page_count];
    out.fs_unused_1 = {spare[p.unused_1[0]], spare[p.unused_1[1]]};
    out.fs_block_type = static_cast<std::uint8_t>(spare[p.type_and_ecc] & 0x3f);
    out.ecc = stored_ecc(spare);
    return out;
}

Bytes encode_spare(SpareLayout layout, const BlockSpare& f) {
    if (f.block_id > 0xfff) usage_error("block_id is 12 bits");
    if (f.fs_unused_0 > 0xf) usage_error("fs_unused_0 is 4 bits");
    if (f.fs_block_type > 0x3f) usage_error("fs_block_type is 6 bits");
    if (f.ecc > kEccMask) usage_error("ecc is 26 bits");
    const SparePositions& p = positions(layout);
    Bytes out(kSpareSize, 0);
    out[p.block_id_low] = static_cast<std::uint8_t>(f.block_id & 0xff);
    out[p.id_nibbles] = static_cast<std::uint8_t>((f.block_id >> 8) | (f.fs_unused_0 << 4));
    for (std::size_t i = 0; i < 4; ++i) out[p.sequence[i]] = f.fs_sequence[i];
    out[p.bad_block] = f.bad_block;
    out[p.size_low] = static_cast<std::uint8_t>(f.fs_size & 0xff);
    out[p.size_high] = static_cast<std::uint8_t>(f.fs_size >> 8);
    out[p.page_count] = f.fs_page_count;
    out[p.unused_1[0]] = f.fs_unused_1[0];
    out[p.unused_1[1]] = f.fs_unused_1[1];
    out[p.type_and_ecc] = static_cast<std::uint8_t>(f.fs_block_type | ((f.ecc & 3) << 6));
    out[p.ecc_1st] = static_cast<std::uint8_t>((f.ecc >> 2) & 0xff);
    out[p.ecc_2nd] = static_cast<std::uint8_t>((f.ecc >> 10) & 0xff);
    out[p.ecc_3rd] = static_cast<std::uint8_t>((f.ecc >> 18) & 0xff);
    return out;
}

// ---- page ECC ----------------------------------------------------------------------------------------------------------

std::uint32_t page_ecc(ByteView data, ByteView spare) {
    check_page(data, spare);
    // A bit-serial CRC-like sum over the page's data words and then the spare's, each word inverted, lowest bit first.
    std::uint32_t val = 0, word = 0;
    ByteView source = data;
    std::size_t at = 0;
    for (unsigned bit = 0; bit < kEccBits; ++bit) {
        if ((bit & 31) == 0) {
            if (bit == kEccSpareStart) {
                source = spare;
                at = 0;
            }
            word = ~load_u32(source, at);
            at += 4;
        }
        val ^= word & 1;
        word >>= 1;
        if (val & 1) val ^= kEccPolynomial;
        val >>= 1;
    }
    return ~val & kEccMask;
}

std::uint32_t stored_ecc(ByteView spare) {
    if (spare.size() < kSpareSize) usage_error("a spare is 16 bytes");
    return ((spare[12] >> 6) & 3u) | static_cast<std::uint32_t>(spare[13]) << 2 | static_cast<std::uint32_t>(spare[14]) << 10 |
           static_cast<std::uint32_t>(spare[15]) << 18;
}

void fix_page_ecc(ByteView data, ByteSpan spare) {
    const std::uint32_t ecc = page_ecc(data, spare);
    spare[12] = static_cast<std::uint8_t>((spare[12] & 0x3f) | ((ecc & 3) << 6));
    spare[13] = static_cast<std::uint8_t>((ecc >> 2) & 0xff);
    spare[14] = static_cast<std::uint8_t>((ecc >> 10) & 0xff);
    spare[15] = static_cast<std::uint8_t>((ecc >> 18) & 0xff);
}

// ---- the image ---------------------------------------------------------------------------------------------------------

NandImage NandImage::from_bytes(Bytes file) {
    NandImage image;
    image.file_ = std::move(file);
    return image;
}

NandImage NandImage::open(const std::filesystem::path& path) {
    NandImage image = from_bytes(fs::read_file(path));
    image.path_ = path;
    return image;
}

ByteView NandImage::page(std::size_t index) const {
    if (index >= page_count()) usage_error(std::format("page {} is outside the image ({} pages)", index, page_count()));
    return ByteView(file_).subspan(index * kFullPage, kPageSize);
}

ByteView NandImage::spare(std::size_t index) const {
    if (index >= page_count()) usage_error(std::format("page {} is outside the image ({} pages)", index, page_count()));
    return ByteView(file_).subspan(index * kFullPage + kPageSize, kSpareSize);
}

void NandImage::set_page(std::size_t index, ByteView data) {
    if (data.size() != kPageSize) usage_error(std::format("a page is {} data bytes, got {}", kPageSize, data.size()));
    (void)page(index);   // the range check
    std::uint8_t* at = file_.data() + index * kFullPage;
    std::copy(data.begin(), data.end(), at);
    fix_page_ecc(ByteView(at, kPageSize), ByteSpan(at + kPageSize, kSpareSize));
}

void NandImage::set_spare(std::size_t index, ByteView spare_bytes) {
    if (spare_bytes.size() != kSpareSize) usage_error(std::format("a spare is {} bytes, got {}", kSpareSize, spare_bytes.size()));
    (void)spare(index);
    std::copy(spare_bytes.begin(), spare_bytes.end(), file_.begin() + static_cast<std::ptrdiff_t>(index * kFullPage + kPageSize));
}

Bytes NandImage::read_flash(std::uint64_t offset, std::size_t length) const {
    if (offset > flash_size() || length > flash_size() - offset) usage_error("the flash range is outside the image");
    Bytes out;
    out.reserve(length);
    while (out.size() < length) {
        const std::uint64_t at = offset + out.size();
        const std::size_t in_page = static_cast<std::size_t>(at % kPageSize);
        const std::size_t take = std::min(kPageSize - in_page, length - out.size());
        append(out, page(static_cast<std::size_t>(at / kPageSize)).subspan(in_page, take));
    }
    return out;
}

void NandImage::write_flash(std::uint64_t offset, ByteView data) {
    if (offset > flash_size() || data.size() > flash_size() - offset) usage_error("the flash range is outside the image");
    std::size_t done = 0;
    while (done < data.size()) {
        const std::uint64_t at = offset + done;
        const std::size_t index = static_cast<std::size_t>(at / kPageSize);
        const std::size_t in_page = static_cast<std::size_t>(at % kPageSize);
        const std::size_t take = std::min(kPageSize - in_page, data.size() - done);
        Bytes merged(page(index).begin(), page(index).end());
        std::copy(data.begin() + static_cast<std::ptrdiff_t>(done), data.begin() + static_cast<std::ptrdiff_t>(done + take),
                  merged.begin() + static_cast<std::ptrdiff_t>(in_page));
        set_page(index, merged);
        done += take;
    }
}

void NandImage::save() const {
    if (!path_) usage_error("this image was not opened from a file; use save_as");
    fs::write_file(*path_, file_);
}

void NandImage::save_as(const std::filesystem::path& path) const { fs::write_file(path, file_); }

std::uint64_t file_to_flash_offset(std::uint64_t file_offset) {
    const std::uint64_t page = file_offset / kFullPage, in_page = file_offset % kFullPage;
    return in_page < kPageSize ? page * kPageSize + in_page : (page + 1) * kPageSize;
}

std::uint64_t flash_to_file_offset(std::uint64_t flash_offset) {
    return (flash_offset / kPageSize) * kFullPage + flash_offset % kPageSize;
}

// ---- batch operations --------------------------------------------------------------------------------------------------

Bytes strip_spare(ByteView image) {
    Bytes out;
    out.reserve(image.size() / kFullPage * kPageSize);
    for (std::size_t at = 0; at + kFullPage <= image.size(); at += kFullPage) append(out, image.subspan(at, kPageSize));
    return out;
}

Bytes add_spare(ByteView raw, std::optional<ByteView> meta) {
    const std::size_t pages = raw.size() / kPageSize;
    if (meta && meta->size() < pages * kSpareSize) usage_error("meta holds fewer spares than raw has pages");
    Bytes out;
    out.reserve(pages * kFullPage);
    for (std::size_t i = 0; i < pages; ++i) {
        const ByteView page = raw.subspan(i * kPageSize, kPageSize);
        Bytes spare(kSpareSize, 0);
        if (meta) std::copy_n(meta->begin() + static_cast<std::ptrdiff_t>(i * kSpareSize), kSpareSize, spare.begin());
        fix_page_ecc(page, spare);
        append(out, page);
        append(out, spare);
    }
    return out;
}

std::vector<std::size_t> verify_ecc(ByteView image) {
    std::vector<std::size_t> bad;
    for (std::size_t i = 0; (i + 1) * kFullPage <= image.size(); ++i) {
        const ByteView page = image.subspan(i * kFullPage, kPageSize), spare = image.subspan(i * kFullPage + kPageSize, kSpareSize);
        if (page_ecc(page, spare) != stored_ecc(spare)) bad.push_back(i);
    }
    return bad;
}

Bytes fix_all_ecc(ByteView image) {
    Bytes out(image.begin(), image.end());
    for (std::size_t at = 0; at + kFullPage <= out.size(); at += kFullPage) {
        fix_page_ecc(ByteView(out.data() + at, kPageSize), ByteSpan(out.data() + at + kPageSize, kSpareSize));
    }
    return out;
}

// ---- the command -------------------------------------------------------------------------------------------------------

std::string_view usage() {
    return "usage:\n"
           "  nandtool info   <img>              page/geometry summary\n"
           "  nandtool verify <img>              check every page's ECC\n"
           "  nandtool unecc  <img> <out>        strip spare -> raw 512-page data\n"
           "  nandtool ecc    <raw> <out>        add spare + ECC -> flash image\n"
           "  nandtool fix    <img> <out>        recompute all ECC in place\n";
}

int command(std::span<const std::string> args) {
    if (args.empty() || args[0] == "-h" || args[0] == "--help") {
        out_text(usage());
        return 0;
    }
    const std::string& op = args[0];
    if (op != "info" && op != "verify" && op != "unecc" && op != "ecc" && op != "fix") throw UsageError("nandtool: unknown operation '" + op + "'\n" + std::string(usage()));
    if (args.size() < 2) throw UsageError("usage: nandtool " + op + " <image>");
    const bool writes = op == "unecc" || op == "ecc" || op == "fix";
    if (writes && args.size() < 3) throw UsageError("usage: nandtool " + op + " <in> <out>");
    const Bytes data = fs::read_file(fs::from_text(args[1]));

    if (op == "info") {
        const std::size_t pages = data.size() / kFullPage;
        out("file size   : 0x{:X} ({} bytes)\n", data.size(), data.size());
        if (data.size() % kFullPage == 0) {
            out("layout      : {} pages x 528 (512 data + 16 spare)\n", pages);
            out("flash size  : 0x{:X} ({} MB)\n", pages * kPageSize, pages * kPageSize / (1024 * 1024));
            const std::vector<std::size_t> bad = verify_ecc(data);
            if (bad.empty()) out("ECC         : all {} pages OK\n", pages);
            else out("ECC         : {} bad pages (e.g. {})\n", bad.size(), index_list(bad, 8));
        } else if (data.size() % kPageSize == 0) {
            out("layout      : {} raw pages x 512 (no spare) \xE2\x80\x94 use `ecc` to build\n", data.size() / kPageSize);
        } else {
            out("layout      : not a whole number of 512/528 pages\n");
        }
        return 0;
    }
    if (op == "verify") {
        const std::vector<std::size_t> bad = verify_ecc(data);
        out("pages       : {}\n", data.size() / kFullPage);
        if (bad.empty()) out("ECC result  : all pages valid\n");
        else out("ECC result  : {} bad: {}{}\n", bad.size(), index_list(bad, 16), bad.size() > 16 ? " ..." : "");
        return bad.empty() ? 0 : 1;
    }
    const std::filesystem::path target = fs::from_text(args[2]);
    if (op == "unecc") {
        fs::write_file(target, strip_spare(data));
        out("stripped {} pages -> {} ({} bytes raw)\n", data.size() / kFullPage, args[2], data.size() / kFullPage * kPageSize);
    } else if (op == "ecc") {
        const Bytes made = add_spare(data);
        fs::write_file(target, made);
        out("built {} pages with ECC -> {} ({} bytes)\n", data.size() / kPageSize, args[2], made.size());
    } else {
        fs::write_file(target, fix_all_ecc(data));
        out("recomputed ECC for {} pages -> {}\n", data.size() / kFullPage, args[2]);
    }
    return 0;
}

}  // namespace cb::formats::xbox::crypto::nand_tool
