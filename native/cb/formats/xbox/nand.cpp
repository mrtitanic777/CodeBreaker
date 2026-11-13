#include "cb/formats/xbox/nand.hpp"

#include <algorithm>
#include <format>
#include <tuple>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/random_file.hpp"

namespace cb::formats::xbox::nand {

namespace {

constexpr std::string_view kReader = "nand";
constexpr std::uint8_t kRootType = 0x30;
constexpr std::size_t kMinimumDump = 0x1000000;
constexpr std::size_t kEntrySize = 0x20, kNameSize = 22;
constexpr std::size_t kAllocWordsPerPage = 256;
constexpr std::uint32_t kLastChainBlock = 0x1FF0;   // a block number from here up is not a data block

}  // namespace

bool is_nand(const std::filesystem::path& path) {
    try {
        fs::RandomFile file(path, fs::FileMode::Read);
        const std::uint64_t size = file.size();
        const Bytes head = file.read_at(0, 0x40);
        return size % (kPage + kSpare) == 0 && size >= kMinimumDump && head.size() >= 2 && head[0] == 0xFF && head[1] == 0x4F &&
               as_text(head).find("Microsoft") != std::string_view::npos;
    } catch (const IoError&) {
        return false;
    }
}

ByteView Flash::page(std::size_t number) const {
    const std::size_t at = number * (kPage + kSpare);
    return io::checked_range(raw_, at, kPage);
}

ByteView Flash::spare(std::size_t block) const {
    return io::checked_range(raw_, block * kPagesPerBlock * (kPage + kSpare) + kPage, kSpare);
}

Flash Flash::parse(Bytes raw) {
    if (raw.size() % (kPage + kSpare) != 0) {
        throw FormatError(kReader, 0, std::format("not a raw NAND dump with spare bytes (size {:#x})", raw.size()));
    }
    Flash f;
    f.raw_ = std::move(raw);
    const std::size_t blocks = f.raw_.size() / (kPage + kSpare) / kPagesPerBlock;

    // the newest root: by sequence number, then by block
    std::vector<std::tuple<std::uint32_t, std::uint32_t>> roots;
    for (std::size_t b = 0; b < blocks; ++b) {
        const ByteView s = f.spare(b);
        if (std::ranges::all_of(s, [](std::uint8_t x) { return x == 0xFF; })) continue;   // unused
        if ((s[12] & 0x3F) == kRootType) {
            const std::uint32_t sequence = (std::uint32_t{s[0]} << 24) | (std::uint32_t{s[3]} << 16) | (std::uint32_t{s[4]} << 8) | s[6];
            roots.emplace_back(sequence, static_cast<std::uint32_t>(b));
        } else {
            f.by_id_.emplace((std::uint32_t{s[2]} << 8) | s[1], static_cast<std::uint32_t>(b));   // the first block that claims a number
        }
    }
    if (roots.empty()) throw FormatError(kReader, 0, "no flash file system root block");
    f.root_ = std::get<1>(*std::ranges::max_element(roots));

    for (std::size_t i = 0; i < kPagesPerBlock; ++i) {
        const ByteView p = f.page(std::size_t{f.root_} * kPagesPerBlock + i);
        if (i % 2 == 0) {   // the allocation table
            for (std::size_t k = 0; k < kAllocWordsPerPage; ++k) f.alloc_.push_back(static_cast<std::uint16_t>((p[k * 2] << 8) | p[k * 2 + 1]));
            continue;
        }
        for (std::size_t o = 0; o < kPage; o += kEntrySize) {   // the directory
            const ByteView e = p.subspan(o, kEntrySize);
            if (e[0] == 0 || e[0] == 0xFF) continue;
            std::size_t name_end = kNameSize;
            while (name_end > 0 && e[name_end - 1] == 0) --name_end;
            std::string name;
            for (std::size_t k = 0; k < name_end; ++k) {
                if (e[k] < 0x80) {
                    name.push_back(static_cast<char>(e[k]));
                } else {
                    name += "\xEF\xBF\xBD";   // (a byte that is not ASCII is U+FFFD)
                }
            }
            File file;
            file.name = name;
            file.start_block = (std::uint32_t{e[22]} << 8) | e[23];
            file.size = (std::uint32_t{e[24]} << 24) | (std::uint32_t{e[25]} << 16) | (std::uint32_t{e[26]} << 8) | e[27];
            file.time = (std::uint32_t{e[28]} << 24) | (std::uint32_t{e[29]} << 16) | (std::uint32_t{e[30]} << 8) | e[31];
            const auto [it, added] = f.index_.emplace(name, f.files_.size());
            if (added) {
                f.files_.push_back(std::move(file));
            } else {
                f.files_[it->second] = std::move(file);   // a name met again keeps its place and takes the later entry
            }
        }
    }
    return f;
}

const File* Flash::find(std::string_view name) const {
    const auto it = index_.find(std::string(name));
    return it == index_.end() ? nullptr : &files_[it->second];
}

std::vector<std::uint32_t> Flash::chain(const File& file) const {
    std::vector<std::uint32_t> out;
    std::uint32_t block = file.start_block;
    while (block != kChainEnd && out.size() * kBlock < file.size) {
        if (block >= alloc_.size() || block >= kLastChainBlock) {
            throw FormatError(kReader, 0, std::format("{}: a bad block number in its chain ({:#x})", file.name, block));
        }
        out.push_back(block);
        block = alloc_[block];
    }
    if (out.size() * kBlock < file.size) {
        throw FormatError(kReader, 0, std::format("{}: its chain is short ({} blocks for {:#x} bytes)", file.name, out.size(), file.size));
    }
    return out;
}

Bytes Flash::read(std::string_view name) const {
    const File* file = find(name);
    if (file == nullptr) throw FormatError(kReader, 0, std::format("no file named '{}'", name));
    const std::vector<std::uint32_t> blocks = chain(*file);
    Bytes data;
    data.reserve(blocks.size() * kBlock);
    for (const std::uint32_t logical : blocks) {
        const auto it = by_id_.find(logical);
        const std::size_t physical = it != by_id_.end() ? it->second : logical;
        for (std::size_t i = 0; i < kPagesPerBlock; ++i) append(data, page(physical * kPagesPerBlock + i));
    }
    data.resize(file->size);
    return data;
}

}  // namespace cb::formats::xbox::nand
