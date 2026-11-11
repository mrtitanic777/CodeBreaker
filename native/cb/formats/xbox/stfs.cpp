#include "cb/formats/xbox/stfs.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <format>

#include <windows.h>   // CNG (bcrypt.h) needs it; confined to this file
#include <bcrypt.h>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xbox::stfs {

namespace {

constexpr std::string_view kReader = "stfs";
constexpr std::uint32_t kEndOfChain = 0xFFFFFF;
constexpr std::size_t kEntrySize = 0x40;       // one file-table entry
constexpr std::size_t kHashEntry = 0x18;       // one hash-table entry: SHA-1, status, next block (24 bits)
constexpr std::uint32_t kPerL0 = 0xAA;
constexpr std::uint32_t kPerL1 = 0x70E4;
constexpr std::size_t kNameField = 0x28;

[[noreturn]] void bad(std::string what, std::uint64_t offset = 0) { throw FormatError(kReader, offset, std::move(what)); }

std::uint32_t u24_le(ByteView d, std::size_t at) {
    io::ByteReader r(d, kReader);
    r.seek(at);
    const std::uint32_t lo = r.u8(), mid = r.u8(), hi = r.u8();
    return lo | (mid << 8) | (hi << 16);
}

std::uint32_t u24_be(ByteView d, std::size_t at) {
    io::ByteReader r(d, kReader);
    r.seek(at);
    const std::uint32_t hi = r.u8(), mid = r.u8(), lo = r.u8();
    return (hi << 16) | (mid << 8) | lo;
}

// A UTF-16BE string up to its first NUL, from the (possibly cut-off) bytes at `at`.
std::string wide_string(ByteView d, std::size_t at, std::size_t max_bytes = 0x80) {
    if (at >= d.size()) return {};
    Bytes b(d.begin() + static_cast<std::ptrdiff_t>(at), d.begin() + static_cast<std::ptrdiff_t>(std::min(d.size(), at + max_bytes)));
    for (std::size_t i = 0; i + 1 < b.size(); i += 2) std::swap(b[i], b[i + 1]);
    const std::string s = text::from_utf16le(b);
    return s.substr(0, s.find('\0'));
}

}  // namespace

std::string_view to_string(Magic m) {
    switch (m) {
        case Magic::Con: return "CON ";
        case Magic::Live: return "LIVE";
        case Magic::Pirs: return "PIRS";
    }
    return "CON ";
}

std::optional<Magic> magic_of(ByteView four) {
    if (four.size() < 4) return std::nullopt;
    const std::string_view s = as_text(four.first(4));
    if (s == "CON ") return Magic::Con;
    if (s == "LIVE") return Magic::Live;
    if (s == "PIRS") return Magic::Pirs;
    return std::nullopt;
}

// ---- the block arithmetic --------------------------------------------------------------------------------

// ComputeBackingDataBlockNumber: a data block's number among the blocks with the hash blocks counted in.
std::uint64_t Package::backing_block(std::uint32_t b) const {
    std::uint64_t r = ((std::uint64_t{b} + kPerL0) / kPerL0 << shift_) + b;
    if (b < kPerL0) return r;
    r += (std::uint64_t{b} + kPerL1) / kPerL1 << shift_;
    return b < kPerL1 ? r : r + (std::uint64_t{1} << shift_);
}

std::uint64_t Package::block_address(std::uint32_t b) const { return (backing_block(b) << 12) + first_hash_; }

// ComputeLevel0BackingHashBlockNumber
std::uint64_t Package::level0_backing(std::uint32_t b) const {
    if (b < kPerL0) return 0;
    std::uint64_t num = std::uint64_t{b / kPerL0} * step_[0];
    num += std::uint64_t{b / kPerL1 + 1} << shift_;
    return b / kPerL1 == 0 ? num : num + (std::uint64_t{1} << shift_);
}

std::uint64_t Package::level1_backing(std::uint32_t b) const {
    if (b < kPerL1) return step_[0];
    return (std::uint64_t{1} << shift_) + std::uint64_t{b / kPerL1} * step_[1];
}

// Cache the top hash table's per-entry status bytes; `hash_address` reads their 0x40 bit to choose which shadow table a
// lower level lives in.
void Package::load_top_table() {
    const std::uint64_t true_block = top_level_ == 0 ? level0_backing(0) : top_level_ == 1 ? level1_backing(0) : step_[1];
    const std::uint64_t base = (true_block << 12) + first_hash_;
    const std::uint64_t addr = base + (std::uint64_t{block_sep_ & 2u} << 11);
    const std::uint32_t per = top_level_ == 0 ? 1 : top_level_ == 1 ? kPerL0 : kPerL1;
    std::uint64_t count = alloc_blocks_ / per;
    if (alloc_blocks_ > kPerL1 && alloc_blocks_ % kPerL1) ++count;
    else if (alloc_blocks_ > kPerL0 && alloc_blocks_ % kPerL0) ++count;
    top_entries_.clear();
    for (std::uint64_t i = 0; i < count + 1; ++i) {
        const std::uint64_t at = addr + i * kHashEntry;
        if (at + kHashEntry > data_.size()) break;
        top_entries_.push_back(data_[at + 0x14]);   // the status byte
    }
}

std::uint64_t Package::hash_address(std::uint32_t b) const {
    auto top_entry = [&](std::uint32_t index) {
        if (index >= top_entries_.size()) bad(std::format("block {} is beyond the top hash table", b));
        return top_entries_[index];
    };
    std::uint64_t a = (level0_backing(b) << 12) + first_hash_ + std::uint64_t{b % kPerL0} * kHashEntry;
    if (top_level_ == 0) {
        a += std::uint64_t{block_sep_ & 2u} << 11;
    } else if (top_level_ == 1) {
        a += std::uint64_t{top_entry(b / kPerL0) & 0x40u} << 6;
    } else {
        const std::uint64_t l1 = std::uint64_t{top_entry(b / kPerL1) & 0x40u} << 6;
        const std::uint64_t pos = (level1_backing(b) << 12) + first_hash_ + l1 + std::uint64_t{b % kPerL0} * kHashEntry;
        if (pos + 0x15 > data_.size()) throw TruncatedError(kReader, pos, 0x15, data_.size() > pos ? data_.size() - pos : 0);
        a += std::uint64_t{data_[pos + 0x14] & 0x40u} << 6;
    }
    return a;
}

std::uint32_t Package::next_block(std::uint32_t b) const { return u24_be(data_, hash_address(b) + 0x15); }

Bytes Package::read_chain(std::uint32_t start, std::uint32_t blocks, std::uint32_t size) const {
    Bytes out;
    std::uint32_t cur = start;
    // A chain of n blocks (or, when the entry does not say, as long as it goes -- bounded by the package's blocks).
    const std::uint64_t limit = blocks ? blocks : std::uint64_t{alloc_blocks_} + 1;
    for (std::uint64_t i = 0; i < limit; ++i) {
        const ByteView block = io::checked_range(data_, block_address(cur), kBlock);
        append(out, block);
        cur = next_block(cur);
        if (cur == kEndOfChain || cur == 0) break;
        if (i + 1 == limit && !blocks) bad("a block chain that does not end", block_address(cur));
    }
    if (size) out.resize(std::min<std::size_t>(out.size(), size));
    return out;
}

void Package::read_file_listing() {
    struct Raw {
        std::string name;
        bool folder;
        int parent;
        std::uint32_t start, blocks, size;
        bool present;
    };
    std::vector<Raw> entries;
    std::uint32_t cur = ft_block_number_;
    for (std::uint32_t k = 0; k < ft_block_count_; ++k) {
        const ByteView block = io::checked_range(data_, block_address(cur), kBlock);
        for (std::size_t i = 0; i < kBlock / kEntrySize; ++i) {
            const ByteView e = block.subspan(i * kEntrySize, kEntrySize);
            const std::size_t name_length = e[0x28] & 0x3F;
            if (name_length == 0) {
                entries.push_back({{}, false, -1, 0, 0, 0, false});   // (indices count every slot)
                continue;
            }
            io::ByteReader r(e, kReader);
            r.seek(0x32);
            const std::int16_t parent = r.i16_be();
            const std::uint32_t size = r.u32_be();
            // a name longer than its 40-byte field is cut at the field
            entries.push_back({text::from_latin1(e.first(std::min(name_length, kNameField))), (e[0x28] & 0x80) != 0, parent,
                               u24_le(e, 0x2F), u24_le(e, 0x29), size, true});
        }
        cur = next_block(cur);
        if (cur == kEndOfChain || cur == 0) break;
    }
    // an entry names its folder by the folder entry's index in the listing (-1: the root)
    auto path_of = [&](auto&& self, int index, int depth) -> std::string {
        if (index < 0 || static_cast<std::size_t>(index) >= entries.size() || !entries[static_cast<std::size_t>(index)].present || depth > 64) return {};
        const Raw& e = entries[static_cast<std::size_t>(index)];
        const std::string parent = e.parent >= 0 ? self(self, e.parent, depth + 1) : std::string();
        return parent.empty() ? e.name : parent + "/" + e.name;
    };
    for (std::size_t i = 0; i < entries.size(); ++i) {
        const Raw& e = entries[i];
        if (!e.present || e.folder) continue;
        FileEntry f{path_of(path_of, static_cast<int>(i), 0), e.start, e.blocks, e.size};
        const auto known = index_.find(f.path);
        if (known != index_.end()) {
            files_[known->second] = std::move(f);   // met twice: the first place, the later data
        } else {
            index_[f.path] = files_.size();
            files_.push_back(std::move(f));
        }
    }
}

Package Package::parse(Bytes data) {
    Package p;
    p.data_ = std::move(data);
    const ByteView d = p.data_;
    const std::optional<Magic> magic = magic_of(d);
    if (!magic) bad("not an STFS package (no CON / LIVE / PIRS magic)");
    p.magic_ = *magic;
    io::ByteReader r(d, kReader);
    r.seek(0x340);
    p.header_size_ = r.u32_be();
    p.first_hash_ = static_cast<std::uint32_t>((std::uint64_t{p.header_size_} + 0xFFF) & 0xFFFFF000u);

    // the volume descriptor
    r.seek(0x37B);
    p.block_sep_ = r.u8();
    p.ft_block_count_ = r.u16();
    p.ft_block_number_ = u24_le(d, 0x37E);
    r.seek(0x395);
    p.alloc_blocks_ = r.u32_be();
    p.unalloc_blocks_ = r.u32_be();

    p.male_ = ((~p.block_sep_) & 1) != 0;
    p.shift_ = p.male_ ? 1 : 0;
    p.step_[0] = p.male_ ? 0xAC : 0xAB;
    p.step_[1] = p.male_ ? 0x723A : 0x718F;
    p.top_level_ = p.alloc_blocks_ <= kPerL0 ? 0 : p.alloc_blocks_ <= kPerL1 ? 1 : 2;

    r.seek(0x344);
    p.content_type_ = r.u32_be();
    const auto hex = [&](std::size_t at, std::size_t n) { return text::upper(crypto::to_hex(io::checked_range(d, at, n))); };
    p.title_id_ = hex(0x360, 4);
    p.profile_id_ = hex(0x371, 8);
    p.display_name_ = wide_string(d, 0x411);
    p.title_name_ = wide_string(d, 0x1691);

    p.load_top_table();
    p.read_file_listing();
    return p;
}

const FileEntry* Package::find(std::string_view path) const {
    const auto it = index_.find(std::string(path));
    return it == index_.end() ? nullptr : &files_[it->second];
}

Bytes Package::read_file(const FileEntry& f) const {
    if (f.size == 0) return {};   // an empty file's start block is the next file's: there is nothing to read
    return read_chain(f.start_block, f.block_count, f.size);
}

Bytes Package::read_file(std::string_view path) const {
    const FileEntry* f = find(path);
    if (!f) throw FormatError(kReader, 0, "no such file in the package: " + std::string(path));
    return read_file(*f);
}

std::size_t Package::extract_all(const std::filesystem::path& outdir) const {
    std::size_t n = 0;
    for (const FileEntry& f : files_) {
        const std::string slashes = text::replace_all(f.path, "\\", "/");
        std::filesystem::path dest = outdir;
        bool unsafe = f.path.empty();
        for (std::string_view part : text::split(slashes, '/')) {
            if (part.empty() || part == ".") continue;
            if (part == ".." || part.find(':') != std::string_view::npos) unsafe = true;
            dest /= fs::from_text(part);
        }
        if (unsafe || dest == outdir) bad("refusing to extract the unsafe name '" + f.path + "'");
        fs::write_file(dest, read_file(f));
        ++n;
    }
    return n;
}

// ---- packing ---------------------------------------------------------------------------------------------

namespace {

// The block geometry of the 'male' layout, which fixes the table spacing at [0xAC, 0x723A].
struct WriteGeometry {
    static constexpr unsigned kShift = 1;
    static constexpr std::uint64_t kStep0 = 0xAC;
    std::uint32_t alloc_blocks;
    std::uint32_t first_hash;
    unsigned top_level;

    WriteGeometry(std::uint32_t alloc, std::uint32_t first) : alloc_blocks(alloc), first_hash(first), top_level(alloc <= kPerL0 ? 0 : 1) {}

    std::uint64_t backing(std::uint32_t b) const {
        std::uint64_t r = ((std::uint64_t{b} + kPerL0) / kPerL0 << kShift) + b;
        if (b < kPerL0) return r;
        r += (std::uint64_t{b} + kPerL1) / kPerL1 << kShift;
        return b < kPerL1 ? r : r + (std::uint64_t{1} << kShift);
    }
    std::uint64_t block_addr(std::uint32_t b) const { return (backing(b) << 12) + first_hash; }
    std::uint64_t l0_backing(std::uint32_t b) const { return b < kPerL0 ? 0 : std::uint64_t{b / kPerL0} * kStep0 + 2; }
    std::uint64_t l1_backing() const { return kStep0; }
};

// UTF-8 to Latin-1, dropping what Latin-1 cannot hold (the name field is Latin-1)
std::string latin1_dropping(std::string_view utf8) {
    std::string out;
    for (std::size_t i = 0; i < utf8.size();) {
        const unsigned char c = static_cast<unsigned char>(utf8[i]);
        std::size_t len = c < 0x80 ? 1 : (c >> 5) == 6 ? 2 : (c >> 4) == 14 ? 3 : (c >> 3) == 30 ? 4 : 1;
        if (i + len > utf8.size()) len = 1;
        std::uint32_t cp = len == 1 ? c : c & (0xFFu >> (len + 1));
        for (std::size_t k = 1; k < len; ++k) cp = (cp << 6) | (static_cast<unsigned char>(utf8[i + k]) & 0x3F);
        if (len > 1 || c < 0x80) {
            if (cp <= 0xFF) out.push_back(static_cast<char>(cp));
        }
        i += len;
    }
    return out;
}

void put_u24_le(Bytes& b, std::size_t at, std::uint32_t v) {
    b[at] = static_cast<std::uint8_t>(v);
    b[at + 1] = static_cast<std::uint8_t>(v >> 8);
    b[at + 2] = static_cast<std::uint8_t>(v >> 16);
}
void put_u32_be(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}
void put_u64_be(Bytes& b, std::size_t at, std::uint64_t v) {
    for (int i = 0; i < 8; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (56 - 8 * i));
}

// Master hash at 0x32C = SHA1(0x344 .. first hash table).
void header_hash(Bytes& buf, std::size_t first_hash) {
    const crypto::Sha1Digest h = crypto::sha1(ByteView(buf).subspan(0x344, first_hash - 0x344));
    std::copy(h.begin(), h.end(), buf.begin() + 0x32C);
}

}  // namespace

Bytes build(std::span<const SourceFile> files, ByteView tpl) {
    if (!magic_of(tpl)) bad("the template is not an STFS package");
    std::uint32_t header_size;
    {
        io::ByteReader r(tpl, kReader);
        r.seek(0x340);
        header_size = r.u32_be();
    }
    const std::uint64_t first_hash64 = (std::uint64_t{header_size} + 0xFFF) & 0xFFFFF000u;
    if (first_hash64 > tpl.size()) throw TruncatedError(kReader, 0, first_hash64, tpl.size());
    const std::uint32_t first_hash = static_cast<std::uint32_t>(first_hash64);
    if (files.empty()) throw UsageError("no files to pack");

    // data blocks: the file table at block 0.., then each file's content
    const std::size_t ft_blocks = std::max<std::size_t>(1, (files.size() + 0x3F) / 0x40);
    struct Placed {
        std::string name;
        std::uint32_t start, blocks;
        std::uint64_t size;
    };
    std::vector<Placed> placed;
    std::vector<Bytes> blocks(ft_blocks);
    std::uint32_t cur = static_cast<std::uint32_t>(ft_blocks);
    for (const SourceFile& f : files) {
        const std::uint32_t nb = static_cast<std::uint32_t>(std::max<std::size_t>(1, (f.data.size() + 0xFFF) / 0x1000));
        placed.push_back({latin1_dropping(f.name).substr(0, kNameField), cur, nb, f.data.size()});
        for (std::uint32_t i = 0; i < nb; ++i) {
            Bytes b(kBlock, 0);
            const std::size_t from = std::size_t{i} * kBlock;
            if (from < f.data.size()) std::copy_n(f.data.begin() + static_cast<std::ptrdiff_t>(from), std::min(kBlock, f.data.size() - from), b.begin());
            blocks.push_back(std::move(b));
        }
        cur += nb;
    }
    const std::uint32_t alloc_blocks = cur;

    // the file-table block(s)
    Bytes ftbuf(ft_blocks * kBlock, 0);
    for (std::size_t i = 0; i < placed.size(); ++i) {
        const Placed& p = placed[i];
        const std::size_t at = i * kEntrySize;
        std::copy(p.name.begin(), p.name.end(), ftbuf.begin() + static_cast<std::ptrdiff_t>(at));
        ftbuf[at + 0x28] = static_cast<std::uint8_t>(p.name.size());
        put_u24_le(ftbuf, at + 0x29, p.blocks);
        put_u24_le(ftbuf, at + 0x2C, p.blocks);
        put_u24_le(ftbuf, at + 0x2F, p.start);
        ftbuf[at + 0x32] = ftbuf[at + 0x33] = 0xFF;   // the root folder
        put_u32_be(ftbuf, at + 0x34, static_cast<std::uint32_t>(p.size));
    }
    for (std::size_t i = 0; i < ft_blocks; ++i) blocks[i].assign(ftbuf.begin() + static_cast<std::ptrdiff_t>(i * kBlock), ftbuf.begin() + static_cast<std::ptrdiff_t>((i + 1) * kBlock));

    // next-block chains
    std::vector<std::uint32_t> next(alloc_blocks, kEndOfChain);
    for (std::size_t j = 0; j + 1 < ft_blocks; ++j) next[j] = static_cast<std::uint32_t>(j + 1);
    for (const Placed& p : placed) {
        for (std::uint32_t i = 0; i + 1 < p.blocks; ++i) next[p.start + i] = p.start + i + 1;
    }

    const WriteGeometry g(alloc_blocks, first_hash);
    std::uint64_t max_back = 0;
    for (std::uint32_t b = 0; b < alloc_blocks; ++b) max_back = std::max({max_back, g.backing(b), g.l0_backing(b) + 1});
    if (g.top_level == 1) max_back = std::max(max_back, g.l1_backing() + 1);
    Bytes buf(first_hash + (max_back + 2) * kBlock, 0);
    std::copy_n(tpl.begin(), first_hash, buf.begin());
    for (std::uint32_t b = 0; b < alloc_blocks; ++b) std::copy(blocks[b].begin(), blocks[b].end(), buf.begin() + static_cast<std::ptrdiff_t>(g.block_addr(b)));

    // L0 hash tables (+ the male layout's backup copy)
    const std::uint32_t groups = (alloc_blocks + 0xA9) / kPerL0;
    std::vector<crypto::Sha1Digest> l0_hashes;
    for (std::uint32_t grp = 0; grp < groups; ++grp) {
        const std::uint64_t base = (g.l0_backing(grp * kPerL0) << 12) + first_hash;
        for (std::uint32_t k = 0; k < kPerL0; ++k) {
            const std::uint32_t b = grp * kPerL0 + k;
            if (b >= alloc_blocks) break;
            const std::uint64_t ea = base + std::uint64_t{k} * kHashEntry;
            const crypto::Sha1Digest h = crypto::sha1(blocks[b]);
            std::copy(h.begin(), h.end(), buf.begin() + static_cast<std::ptrdiff_t>(ea));
            buf[ea + 0x14] = 0xC0;
            buf[ea + 0x15] = static_cast<std::uint8_t>(next[b] >> 16);
            buf[ea + 0x16] = static_cast<std::uint8_t>(next[b] >> 8);
            buf[ea + 0x17] = static_cast<std::uint8_t>(next[b]);
        }
        const ByteView table = ByteView(buf).subspan(base, kBlock);
        std::copy(table.begin(), table.end(), buf.begin() + static_cast<std::ptrdiff_t>(base + kBlock));
        l0_hashes.push_back(crypto::sha1(ByteView(buf).subspan(base, kBlock)));
    }
    crypto::Sha1Digest top_hash;
    if (g.top_level == 1) {
        const std::uint64_t l1 = (g.l1_backing() << 12) + first_hash;
        for (std::size_t i = 0; i < l0_hashes.size(); ++i) {
            const std::uint64_t ea = l1 + i * kHashEntry;
            std::copy(l0_hashes[i].begin(), l0_hashes[i].end(), buf.begin() + static_cast<std::ptrdiff_t>(ea));
            buf[ea + 0x14] = 0x80;
        }
        const ByteView table = ByteView(buf).subspan(l1, kBlock);
        std::copy(table.begin(), table.end(), buf.begin() + static_cast<std::ptrdiff_t>(l1 + kBlock));
        top_hash = crypto::sha1(ByteView(buf).subspan(l1, kBlock));
    } else {
        top_hash = l0_hashes[0];
    }

    // the volume descriptor and sizes (male layout: block separation 0)
    buf[0x379] = 0x24;
    buf[0x37B] = 0x00;
    buf[0x37C] = static_cast<std::uint8_t>(ft_blocks);
    buf[0x37D] = static_cast<std::uint8_t>(ft_blocks >> 8);
    buf[0x37E] = buf[0x37F] = buf[0x380] = 0;
    std::copy(top_hash.begin(), top_hash.end(), buf.begin() + 0x381);
    put_u32_be(buf, 0x395, alloc_blocks);
    put_u32_be(buf, 0x399, 0);
    put_u64_be(buf, 0x34C, std::uint64_t{alloc_blocks} * kBlock);
    header_hash(buf, first_hash);
    return buf;
}

std::filesystem::path build(const std::filesystem::path& folder, const std::filesystem::path& out_path,
                            const std::filesystem::path& template_path) {
    std::vector<std::pair<std::string, Bytes>> inner;
    if (std::filesystem::is_directory(folder)) {
        std::vector<std::filesystem::path> names;
        for (const auto& e : std::filesystem::directory_iterator(folder)) {
            if (e.is_regular_file()) names.push_back(e.path());
        }
        std::sort(names.begin(), names.end(), [](const auto& a, const auto& b) { return fs::to_text(a.filename()) < fs::to_text(b.filename()); });
        for (const auto& p : names) inner.emplace_back(fs::to_text(p.filename()), fs::read_file(p));
    } else {
        inner.emplace_back(fs::to_text(folder.filename()), fs::read_file(folder));
    }
    std::vector<SourceFile> files;
    for (const auto& [name, data] : inner) files.push_back({name, data});
    const Bytes tpl = fs::read_file(template_path);
    fs::write_file(out_path, build(files, tpl));
    return out_path;
}

// ---- resigning ---------------------------------------------------------------------------------------------

namespace {

// A XeCrypt big number (an array of big-endian qwords, least significant first) as the big-endian bytes of its value.
Bytes bn_big_endian(ByteView b) {
    Bytes out;
    out.reserve(b.size());
    for (std::size_t g = b.size() / 8; g-- > 0;) out.insert(out.end(), b.begin() + static_cast<std::ptrdiff_t>(g * 8), b.begin() + static_cast<std::ptrdiff_t>(g * 8 + 8));
    return out;
}

ByteView minimal(ByteView v) {   // without leading zeros (but never empty)
    std::size_t i = 0;
    while (i + 1 < v.size() && v[i] == 0) ++i;
    return v.subspan(i);
}

constexpr std::string_view kSha1DigestInfo = "\x30\x21\x30\x09\x06\x05\x2b\x0e\x03\x02\x1a\x05\x00\x04\x14";

struct Provider {
    BCRYPT_ALG_HANDLE h = nullptr;
    Provider() {
        if (BCryptOpenAlgorithmProvider(&h, BCRYPT_RSA_ALGORITHM, nullptr, 0) != 0) throw InternalError("CNG: no RSA provider");
    }
    ~Provider() { BCryptCloseAlgorithmProvider(h, 0); }
};
struct KeyHandle {
    BCRYPT_KEY_HANDLE h = nullptr;
    ~KeyHandle() {
        if (h) BCryptDestroyKey(h);
    }
};

// A BCRYPT_RSAKEY_BLOB: the header, then the public exponent, modulus (and primes), big-endian.
Bytes rsa_blob(ULONG magic, ByteView e, ByteView n, ByteView p, ByteView q) {
    BCRYPT_RSAKEY_BLOB h{};
    h.Magic = magic;
    h.BitLength = static_cast<ULONG>(n.size() * 8);
    h.cbPublicExp = static_cast<ULONG>(e.size());
    h.cbModulus = static_cast<ULONG>(n.size());
    h.cbPrime1 = static_cast<ULONG>(p.size());
    h.cbPrime2 = static_cast<ULONG>(q.size());
    Bytes out(sizeof h);
    std::memcpy(out.data(), &h, sizeof h);
    append(out, e);
    append(out, n);
    append(out, p);
    append(out, q);
    return out;
}

// PKCS#1 v1.5 RSA-SHA1 over `region` with the keyvault's console key: the 0x80-byte signature in the byte-reversed form
// STFS stores.
Bytes con_sign(ByteView region, ByteView kv) {
    io::ByteReader r(kv, kReader);
    r.seek(kKvPrivateKeyOffset + 4);
    const std::uint32_t exponent = r.u32_be();
    const Bytes e = {static_cast<std::uint8_t>(exponent >> 24), static_cast<std::uint8_t>(exponent >> 16), static_cast<std::uint8_t>(exponent >> 8), static_cast<std::uint8_t>(exponent)};
    const Bytes n = bn_big_endian(io::checked_range(kv, kKvPrivateKeyOffset + 0x10, 0x80));
    const Bytes p = bn_big_endian(io::checked_range(kv, kKvPrivateKeyOffset + 0x90, 0x40));
    const Bytes q = bn_big_endian(io::checked_range(kv, kKvPrivateKeyOffset + 0xD0, 0x40));
    const Bytes blob = rsa_blob(BCRYPT_RSAPRIVATE_MAGIC, minimal(e), n, p, q);
    Provider prov;
    KeyHandle key;
    if (BCryptImportKeyPair(prov.h, nullptr, BCRYPT_RSAPRIVATE_BLOB, &key.h, const_cast<PUCHAR>(blob.data()), static_cast<ULONG>(blob.size()), 0) != 0) {
        bad("the keyvault's console key is not a valid RSA key");
    }
    const crypto::Sha1Digest digest = crypto::sha1(region);
    BCRYPT_PKCS1_PADDING_INFO pad{BCRYPT_SHA1_ALGORITHM};
    Bytes sig(n.size());
    ULONG written = 0;
    if (BCryptSignHash(key.h, &pad, const_cast<PUCHAR>(digest.data()), static_cast<ULONG>(digest.size()), sig.data(), static_cast<ULONG>(sig.size()), &written, BCRYPT_PAD_PKCS1) != 0 ||
        written != sig.size()) {
        bad("the keyvault's console key cannot sign");
    }
    std::reverse(sig.begin(), sig.end());
    return sig;
}

// Verify a byte-reversed STFS signature against the certificate's public modulus (a self-check). The modulus is a
// XeCrypt BnQw big number, like the key.
bool con_verify(ByteView region, ByteView sig, ByteView modulus_bnqw, std::uint32_t exponent) {
    const Bytes n = bn_big_endian(modulus_bnqw);
    const Bytes e = {static_cast<std::uint8_t>(exponent >> 24), static_cast<std::uint8_t>(exponent >> 16), static_cast<std::uint8_t>(exponent >> 8), static_cast<std::uint8_t>(exponent)};
    const Bytes blob = rsa_blob(BCRYPT_RSAPUBLIC_MAGIC, minimal(e), n, {}, {});
    Provider prov;
    KeyHandle key;
    if (BCryptImportKeyPair(prov.h, nullptr, BCRYPT_RSAPUBLIC_BLOB, &key.h, const_cast<PUCHAR>(blob.data()), static_cast<ULONG>(blob.size()), 0) != 0) return false;
    Bytes forward(sig.rbegin(), sig.rend());
    const crypto::Sha1Digest digest = crypto::sha1(region);
    BCRYPT_PKCS1_PADDING_INFO pad{BCRYPT_SHA1_ALGORITHM};
    return BCryptVerifySignature(key.h, &pad, const_cast<PUCHAR>(digest.data()), static_cast<ULONG>(digest.size()), forward.data(),
                                 static_cast<ULONG>(forward.size()), BCRYPT_PAD_PKCS1) == 0;
}

}  // namespace

std::string console_id(ByteView kv) {
    return text::upper(crypto::to_hex(io::checked_range(kv, kKvCertOffset + 2, 5)));
}

Bytes resign(ByteView package, ByteView kv, const ResignOptions& options) {
    const std::optional<Magic> magic = magic_of(package);
    if (!magic) bad("not an STFS package (no CON / LIVE / PIRS magic)");
    if (kv.size() < kKvSize) bad(std::format("keyvault too small ({} bytes); expected a decrypted KV_dec.bin of 0x4000 bytes", kv.size()));
    Bytes data(package.begin(), package.end());
    if (*magic != Magic::Con) {
        // LIVE/PIRS -> CON: flip the magic and reset the license descriptor to a single Unrestricted entry (the state
        // console content uses). The cert copy below fills 0x04..0x1AC and the signature fills 0x1AC..0x22C, so the whole
        // Microsoft-signature block is overwritten.
        std::memcpy(data.data(), "CON ", 4);
        io::checked_range(data, 0x22C, 0x100);
        std::fill(data.begin() + 0x22C, data.begin() + 0x32C, std::uint8_t{0});   // 0x10 license entries
        std::fill(data.begin() + 0x22C, data.begin() + 0x234, std::uint8_t{0xFF});   // license[0] = Unrestricted
    }
    // 1) the console certificate -> header 0x04 (also sets the cert size 0x1A8 at 0x04)
    const ByteView cert = io::checked_range(kv, kKvCertOffset, kKvCertLength);
    io::checked_range(data, 0x04, kKvCertLength);
    std::copy(cert.begin(), cert.end(), data.begin() + 0x04);
    // optionally bind the content to this console (the metadata's console id at 0x36C)
    if (options.bind_console) {
        const ByteView id = io::checked_range(kv, kKvCertOffset + 2, 5);
        io::checked_range(data, 0x36C, 5);
        std::copy(id.begin(), id.end(), data.begin() + 0x36C);
    }
    // 2) the header hash at 0x32C over the current metadata (inside the signed region): 0x344 up to the first hash
    //    table, but never past the end of the file
    io::ByteReader r(data, kReader);
    r.seek(0x340);
    const std::uint64_t first_hash = std::min<std::uint64_t>((std::uint64_t{r.u32_be()} + 0xFFF) & 0xFFFFF000u, data.size());
    if (first_hash < 0x344) bad("the header size is smaller than the header", 0x340);
    const crypto::Sha1Digest header_digest = crypto::sha1(ByteView(data).subspan(0x344, first_hash - 0x344));
    std::copy(header_digest.begin(), header_digest.end(), data.begin() + 0x32C);
    // 3) sign header[0x22C:0x344] with the console private key
    const Bytes region(data.begin() + 0x22C, data.begin() + 0x344);
    const Bytes sig = con_sign(region, kv);
    io::checked_range(data, 0x1AC, sig.size());
    std::copy(sig.begin(), sig.end(), data.begin() + 0x1AC);
    // 4) self-check: the signature must verify against the certificate's own modulus
    io::ByteReader cr(data, kReader);
    cr.seek(0x28);
    const std::uint32_t cert_exponent = cr.u32_be();
    if (!con_verify(region, sig, ByteView(data).subspan(0x2C, 0x80), cert_exponent)) {
        bad("the signature does not verify against the keyvault's certificate (the key and certificate do not match)");
    }
    return data;
}

void resign_file(const std::filesystem::path& package, const std::filesystem::path& keyvault,
                 const std::optional<std::filesystem::path>& out_path, const ResignOptions& options) {
    const Bytes in = fs::read_file(package);
    const Bytes kv = fs::read_file(keyvault);
    // (the whole result exists before the destination is touched, which may be the package itself)
    fs::write_file_atomic(out_path.value_or(package), resign(in, kv, options));
}

}  // namespace cb::formats::xbox::stfs
