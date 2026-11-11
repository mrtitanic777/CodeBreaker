#include "cb/formats/cab.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <chrono>
#include <ctime>
#include <format>
#include <map>
#include <memory>

#include "zlib.h"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/intmath.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::cab {

namespace {

constexpr std::string_view kReader = "cab";
constexpr std::size_t kHeaderSize = 36;
constexpr std::uint16_t kFlagPrev = 1;
constexpr std::uint16_t kFlagNext = 2;
constexpr std::uint16_t kFlagReserve = 4;
constexpr std::uint16_t kAttrUtf8Name = 0x80;
constexpr std::uint16_t kAttrArchive = 0x20;

[[noreturn]] void bad(std::string what, std::uint64_t offset = 0) { throw FormatError(kReader, offset, std::move(what)); }

std::string text_at(io::ByteReader& r, bool utf8 = false) {
    const ByteView s = r.cstr();
    return utf8 ? text::sanitize_utf8(as_text(s)) : text::from_latin1(s);
}

}  // namespace

std::string compression_name(std::uint16_t type) {
    switch (type) {
        case 0: return "NONE";
        case 1: return "MSZIP";
        case 2: return "QUANTUM";
        case 3: return "LZX";
        default: return std::format("?{}", type);
    }
}

std::size_t Cabinet::folder_of(const FileEntry& file) const {
    if (file.folder == kFromPrev || file.folder == kPrevAndNext) return 0;
    return file.folder == kToNext ? folders.size() - 1 : file.folder;
}

std::string Cabinet::compression_of(const FileEntry& file) const {
    if (folders.empty()) return "?";
    const std::size_t k = folder_of(file);
    return k < folders.size() ? folders[k].compression() : "?";
}

Cabinet parse(ByteView data, const std::filesystem::path& path) {
    if (data.size() < kHeaderSize || std::string_view(reinterpret_cast<const char*>(data.data()), 4) != "MSCF") {
        bad("not a cabinet (no MSCF signature)");
    }
    io::ByteReader r(data, kReader);
    Cabinet c;
    c.path = path;
    r.seek(8);
    c.size = r.u32();
    r.seek(16);
    const std::uint32_t files_at = r.u32();
    r.seek(24);
    c.version_minor = r.u8();
    c.version_major = r.u8();
    const std::uint16_t folder_count = r.u16();
    const std::uint16_t file_count = r.u16();
    c.flags = r.u16();
    c.set_id = r.u16();
    c.index = r.u16();
    std::uint8_t folder_reserve = 0;
    if (c.flags & kFlagReserve) {
        const std::uint16_t header_reserve = r.u16();
        folder_reserve = r.u8();
        c.block_reserve = r.u8();
        r.skip(header_reserve);
    }
    if (c.flags & kFlagPrev) {
        SetLink link;
        link.cabinet = text_at(r);
        link.disk = text_at(r);
        c.prev = std::move(link);
    }
    if (c.flags & kFlagNext) {
        SetLink link;
        link.cabinet = text_at(r);
        link.disk = text_at(r);
        c.next = std::move(link);
    }
    for (std::uint16_t k = 0; k < folder_count; ++k) {
        FolderEntry f;
        f.data_offset = r.u32();
        f.block_count = r.u16();
        f.type = r.u16();
        c.folders.push_back(f);
        r.skip(folder_reserve);
    }
    r.seek(files_at);
    for (std::uint16_t k = 0; k < file_count; ++k) {
        FileEntry e;
        e.size = r.u32();
        e.offset = r.u32();
        e.folder = r.u16();
        e.date = r.u16();
        e.time = r.u16();
        e.attributes = r.u16();
        e.name = text_at(r, (e.attributes & kAttrUtf8Name) != 0);
        c.files.push_back(std::move(e));
    }
    return c;
}

Cabinet read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view(), path);
}

std::uint32_t checksum(ByteView payload, std::uint16_t uncompressed) {
    // The block header's two sizes, then the payload, as one run of bytes.
    Bytes d;
    d.reserve(payload.size() + 4);
    const std::uint16_t compressed = static_cast<std::uint16_t>(payload.size());
    d.push_back(static_cast<std::uint8_t>(compressed & 0xFF));
    d.push_back(static_cast<std::uint8_t>(compressed >> 8));
    d.push_back(static_cast<std::uint8_t>(uncompressed & 0xFF));
    d.push_back(static_cast<std::uint8_t>(uncompressed >> 8));
    append(d, payload);
    const std::size_t whole = d.size() & ~std::size_t{3};
    std::uint32_t v = 0;
    for (std::size_t i = whole; i < d.size(); ++i) v = (v << 8) | d[i];   // the odd bytes, most significant first
    for (std::size_t i = 0; i < whole; i += 4) {
        v ^= d[i] | (d[i + 1] << 8) | (d[i + 2] << 16) | (static_cast<std::uint32_t>(d[i + 3]) << 24);
    }
    return v;
}

namespace {

// ---- the neighbours of a set --------------------------------------------------------------------------

Cabinet neighbour(const Cabinet& cab, bool next) {
    const std::optional<SetLink>& link = next ? cab.next : cab.prev;
    const std::string which = next ? "next" : "prev";
    if (!link) bad(path_text(cab.path) + ": the " + which + " cabinet of the set is not named");
    const std::filesystem::path folder = std::filesystem::absolute(cab.path).parent_path();
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(folder, ec)) {
        if (text::iequals(fs::to_text(entry.path().filename()), link->cabinet)) return read(entry.path());
    }
    throw NotFoundError(folder / link->cabinet, 0,
                        "the " + which + " cabinet of the set (" + link->cabinet + ") is not beside " + path_text(cab.path));
}

bool continued_from(const Cabinet& c) {
    return std::any_of(c.files.begin(), c.files.end(),
                       [](const FileEntry& e) { return e.folder == kFromPrev || e.folder == kPrevAndNext; });
}
bool continues_into(const Cabinet& c) {
    return std::any_of(c.files.begin(), c.files.end(),
                       [](const FileEntry& e) { return e.folder == kToNext || e.folder == kPrevAndNext; });
}

// ---- decoders: one per folder, fed block by block -----------------------------------------------------

class Decoder {
public:
    virtual ~Decoder() = default;
    // One CFDATA block's payload; appends `uncompressed` bytes to `out` (the whole stream so far).
    virtual void block(ByteView payload, std::uint16_t uncompressed, Bytes& out) = 0;
};

class Stored final : public Decoder {
public:
    void block(ByteView payload, std::uint16_t, Bytes& out) override { append(out, payload); }
};

class MsZip final : public Decoder {
public:
    void block(ByteView payload, std::uint16_t uncompressed, Bytes& out) override {
        if (payload.size() < 2 || payload[0] != 'C' || payload[1] != 'K') bad("MSZIP block without the CK signature");
        const Bytes got = inflate(payload.subspan(2));
        if (got.size() != uncompressed) {
            bad(std::format("MSZIP block inflated to {} bytes, header says {}", got.size(), uncompressed));
        }
        append(out, got);
        // The window now holds this block at its start and what was there before from then on; it is the next block's
        // dictionary (a zero-filled window to begin with).
        Bytes w = got;
        if (got.size() < window_.size()) w.insert(w.end(), window_.begin() + static_cast<std::ptrdiff_t>(got.size()), window_.end());
        if (w.size() > kBlock) w.erase(w.begin(), w.end() - static_cast<std::ptrdiff_t>(kBlock));
        w.resize(std::max(kBlock, w.size()), 0);
        window_ = std::move(w);
    }

private:
    Bytes inflate(ByteView in) const {
        z_stream z{};
        if (inflateInit2(&z, -15) != Z_OK) throw InternalError("zlib: cannot start inflating");
        struct End { z_stream& z; ~End() { inflateEnd(&z); } } end{z};
        if (!window_.empty() &&
            inflateSetDictionary(&z, window_.data(), static_cast<uInt>(window_.size())) != Z_OK) {
            throw InternalError("zlib: cannot set the MSZIP dictionary");
        }
        Bytes out;
        std::uint8_t chunk[65536];
        z.next_in = const_cast<Bytef*>(in.data());
        z.avail_in = static_cast<uInt>(in.size());
        int rc = Z_OK;
        do {
            z.next_out = chunk;
            z.avail_out = sizeof chunk;
            rc = ::inflate(&z, Z_SYNC_FLUSH);
            out.insert(out.end(), chunk, chunk + (sizeof chunk - z.avail_out));
            if (rc == Z_BUF_ERROR && z.avail_out != 0) {   // the input ran out: what was inflated is what there is
                rc = Z_OK;
                z.avail_in = 0;
            }
        } while (rc == Z_OK && (z.avail_in > 0 || z.avail_out == 0));
        if (rc != Z_OK && rc != Z_STREAM_END && rc != Z_BUF_ERROR) {
            bad(std::format("MSZIP block does not inflate ({}: {})", rc, z.msg ? z.msg : ""));
        }
        return out;
    }

    Bytes window_;
};

// Quantum: an arithmetic coder over adaptive frequency models and an LZ77 window, from the published description of the
// format. Its arithmetic is signed (a corrupt stream can drive the intermediate values negative); the divisions
// that matter are floor divisions.
class Quantum final : public Decoder {
public:
    explicit Quantum(std::uint16_t type) {
        window_bits_ = (type >> 8) & 0x1F;
        if (window_bits_ < 10 || window_bits_ > 21) bad(std::format("Quantum window of 2^{} bytes is not valid", window_bits_));
        for (int i = 0; i < 4; ++i) literals_.emplace_back(64 * i, 64);
        positions_.emplace_back(0, std::min(window_bits_ * 2, 24));
        positions_.emplace_back(0, std::min(window_bits_ * 2, 36));
        positions_.emplace_back(0, window_bits_ * 2);
    }

    void block(ByteView payload, std::uint16_t uncompressed, Bytes& out) override;

private:
    // An adaptive frequency model: symbols in order of frequency and their cumulative frequencies (descending, with a
    // final zero).
    struct Model {
        std::vector<int> symbols;
        std::vector<int> cum;
        int countdown = 4;
        Model(int first, int count) {
            for (int i = 0; i < count; ++i) symbols.push_back(first + i);
            for (int i = count; i >= 0; --i) cum.push_back(i);
        }
    };

    struct Tables {
        std::array<int, 42> pos_extra{}, pos_base{};
        std::array<int, 27> len_extra{}, len_base{};
        Tables() {
            for (int i = 0; i < 40; ++i) pos_extra[2 + i] = i / 2;
            for (int i = 0; i < 42; ++i) {
                for (int k = 0; k < i; ++k) pos_base[i] += 1 << pos_extra[k];
            }
            int k = 0;
            for (int i = 0; i < 6; ++i) len_extra[k++] = 0;
            for (int n = 1; n <= 5; ++n) {
                for (int r = 0; r < 4; ++r) len_extra[k++] = n;
            }
            len_extra[k++] = 0;
            for (int i = 0; i < 27; ++i) {
                for (int j = 0; j < i; ++j) len_base[i] += 1 << len_extra[j];
            }
        }
    };

    static void rescale(Model& m);

    int window_bits_ = 0;
    std::vector<Model> literals_, positions_;
    Model lengths_{0, 27}, selector_{0, 7};
    std::uint64_t produced_ = 0;
};

void Quantum::rescale(Model& m) {
    const int count = static_cast<int>(m.symbols.size());
    if (--m.countdown != 0) {
        // halve the frequencies, keeping them strictly descending
        for (int i = count - 1; i >= 0; --i) {
            m.cum[i] >>= 1;
            if (m.cum[i] <= m.cum[i + 1]) m.cum[i] = m.cum[i + 1] + 1;
        }
        return;
    }
    // every 50th rescale: also reorder the symbols by frequency (a selection pass, as the format describes)
    m.countdown = 50;
    std::vector<int> freq(count);
    for (int i = 0; i < count; ++i) freq[i] = (m.cum[i] - m.cum[i + 1] + 1) >> 1;
    for (int i = 0; i < count - 1; ++i) {
        int fi = freq[i];
        for (int j = i + 1; j < count; ++j) {
            if (fi < freq[j]) {
                freq[i] = freq[j];
                freq[j] = fi;
                std::swap(m.symbols[i], m.symbols[j]);
                fi = freq[i];
            }
        }
    }
    for (int i = count - 1; i >= 0; --i) m.cum[i] = m.cum[i + 1] + freq[i];
}

void Quantum::block(ByteView payload, std::uint16_t uncompressed, Bytes& out) {
    static const Tables T;
    Bytes data(payload.begin(), payload.end());
    data.insert(data.end(), 8, 0);
    if (data.size() < 10) bad("Quantum block too short");
    std::size_t pos = 2;
    int have = 0;
    std::uint64_t buf = 0;
    std::int64_t high = 0xFFFF, low = 0, code = (std::int64_t{data[0]} << 8) | data[1];
    produced_ += uncompressed;
    const std::uint64_t goal = produced_;

    // n (at most 20) bits from the stream; zeros past its end for a little way, then an error
    auto bits = [&](int n) -> int {
        if (have < n) {
            std::uint32_t word = 0;
            for (std::size_t i = 0; i < 4; ++i) word = (word << 8) | (pos + i < data.size() ? data[pos + i] : 0);
            buf = ((buf & ((std::uint64_t{1} << have) - 1)) << 32) | word;
            pos += 4;
            have += 32;
            if (pos > data.size() + 64) bad("Quantum block ran out of data", pos);
        }
        have -= n;
        return static_cast<int>((buf >> have) & ((std::uint64_t{1} << n) - 1));
    };
    auto symbol = [&](Model& m) -> int {
        const std::int64_t total = m.cum[0];
        const std::int64_t span = high - low + 1;
        if (span <= 0 || total <= 0) bad("Quantum arithmetic state is corrupt", pos);
        const std::int64_t target = floor_div((code - low + 1) * total - 1, span);
        std::size_t i = 1;
        while (i < m.cum.size() && m.cum[i] > target) ++i;
        if (i >= m.cum.size()) bad("Quantum symbol out of range", pos);
        const int value = m.symbols[i - 1];
        high = low + floor_div(std::int64_t{m.cum[i - 1]} * span, total) - 1;
        low += floor_div(std::int64_t{m.cum[i]} * span, total);
        for (std::size_t j = 0; j < i; ++j) m.cum[j] += 8;
        if (m.cum[0] > 3800) rescale(m);
        while (true) {
            const std::int64_t differ = high ^ low;
            if (differ < 0 || differ > 0xFFFF) bad("Quantum arithmetic state is corrupt", pos);
            int n;
            if (differ & 0x8000) {
                if (!((low & 0x4000) && !(high & 0x4000))) break;
                code ^= 0x4000;
                low &= 0x3FFF;
                high |= 0x4000;
                n = 1;
            } else {
                n = 16 - std::bit_width(static_cast<std::uint64_t>(differ));
            }
            low = (low << n) & 0xFFFF;
            high = ((high << n) | ((std::int64_t{1} << n) - 1)) & 0xFFFF;
            code = ((code << n) | bits(n)) & 0xFFFF;
        }
        return value;
    };

    while (out.size() < goal) {
        const int which = symbol(selector_);
        if (which < 4) {
            out.push_back(static_cast<std::uint8_t>(symbol(literals_[static_cast<std::size_t>(which)])));
            continue;
        }
        int length;
        int slot;
        if (which == 6) {
            slot = symbol(lengths_);
            length = T.len_base[static_cast<std::size_t>(slot)] + bits(T.len_extra[static_cast<std::size_t>(slot)]) + 5;
        } else {
            length = which - 1;
        }
        slot = symbol(positions_[static_cast<std::size_t>(which - 4)]);
        const std::size_t distance = static_cast<std::size_t>(T.pos_base[static_cast<std::size_t>(slot)] +
                                                              bits(T.pos_extra[static_cast<std::size_t>(slot)]) + 1);
        if (distance > out.size()) bad("Quantum match reaches before the start of the folder", pos);
        for (int k = 0; k < length; ++k) out.push_back(out[out.size() - distance]);   // may overlap its own output
    }
}

// ---- reading a folder ----------------------------------------------------------------------------------

std::unique_ptr<Decoder> decoder_for(const Cabinet& cab, const FolderEntry& folder) {
    switch (folder.type & 15) {
        case 0: return std::make_unique<Stored>();
        case 1: return std::make_unique<MsZip>();
        case 2: return std::make_unique<Quantum>(folder.type);
        case 3: throw UnsupportedError(kReader, 0, "LZX-compressed cabinets are not supported");
        default: bad(std::format("{}: unknown compression type {}", path_text(cab.path), folder.type & 15));
    }
}

}  // namespace

Bytes folder_stream(const Cabinet& start, std::size_t k, std::optional<std::uint64_t> upto) {
    Cabinet cab = start;
    while (k == 0 && continued_from(cab)) {   // a folder that began in an earlier cabinet is read from its start
        cab = neighbour(cab, false);
        CB_ASSERT(!cab.folders.empty(), "a cabinet that continues a folder has none");
        k = cab.folders.size() - 1;
    }
    if (k >= cab.folders.size()) bad(std::format("{}: no folder {}", path_text(cab.path), k));
    std::unique_ptr<Decoder> decoder = decoder_for(cab, cab.folders[k]);
    Bytes out;
    Bytes held;   // the payload so far of a block whose data runs on into the next cabinet's first block
    while (true) {
        const FolderEntry& f = cab.folders[k];
        const fs::MappedFile file(cab.path);
        io::ByteReader r(file.view(), kReader);
        r.seek(f.data_offset);
        for (std::uint16_t i = 0; i < f.block_count; ++i) {
            const std::uint32_t stored_checksum = r.u32();
            const std::uint16_t compressed = r.u16();
            const std::uint16_t uncompressed = r.u16();
            r.skip(cab.block_reserve);
            const ByteView payload = r.bytes(compressed);
            if (stored_checksum != 0 && stored_checksum != checksum(payload, uncompressed)) {
                bad(path_text(cab.path) + ": data block checksum mismatch", r.pos());
            }
            if (uncompressed == 0 && compressed != 0) {   // the first part of a block continued in the next cabinet
                append(held, payload);
                continue;
            }
            if (uncompressed != 0) {
                if (held.empty()) {
                    decoder->block(payload, uncompressed, out);
                } else {
                    append(held, payload);
                    decoder->block(held, uncompressed, out);
                    held.clear();
                }
            } else {
                held.clear();
            }
            if (upto && out.size() >= *upto) return out;
        }
        if (k != cab.folders.size() - 1 || !continues_into(cab)) return out;
        cab = neighbour(cab, true);
        k = 0;
    }
}

namespace {

// Where a stored name lands under `outdir`: backslashes are separators, empty and "." parts are skipped; a name that
// would leave the folder is refused.
std::filesystem::path safe_path(const std::filesystem::path& outdir, const std::string& name) {
    const std::string slashes = text::replace_all(name, "\\", "/");
    std::vector<std::string_view> parts;
    for (std::string_view part : text::split(slashes, '/')) {
        if (!part.empty() && part != ".") parts.push_back(part);
    }
    const bool unsafe = parts.empty() || name.find(':') != std::string::npos ||
                        std::any_of(parts.begin(), parts.end(), [](std::string_view p) { return p == ".."; });
    if (unsafe) bad("refusing to extract the unsafe name '" + name + "'");
    std::filesystem::path out = outdir;
    for (std::string_view p : parts) out /= fs::from_text(p);
    return out;
}

void set_file_time(const std::filesystem::path& target, const FileEntry& e) {
    std::tm tm{};
    tm.tm_year = 1980 + (e.date >> 9) - 1900;
    tm.tm_mon = ((e.date >> 5) & 15) - 1;
    tm.tm_mday = e.date & 31;
    tm.tm_hour = e.time >> 11;
    tm.tm_min = (e.time >> 5) & 63;
    tm.tm_sec = (e.time & 31) * 2;
    tm.tm_isdst = -1;
    const std::time_t stamp = std::mktime(&tm);
    if (stamp == static_cast<std::time_t>(-1)) return;   // not a representable time: the file keeps its own
    std::error_code ec;
    std::filesystem::last_write_time(
        target, std::chrono::clock_cast<std::filesystem::file_time_type::clock>(std::chrono::system_clock::from_time_t(stamp)), ec);
}

}  // namespace

std::vector<Extracted> extract(const std::filesystem::path& cabinet, const std::filesystem::path& outdir,
                               const std::optional<std::vector<std::string>>& names) {
    const Cabinet cab = read(cabinet);
    std::vector<std::string> wanted;
    if (names) {
        for (const std::string& n : *names) wanted.push_back(text::lower(n));
    }
    // the files to take, grouped by the folder they live in (one decode of a folder serves them all)
    std::map<std::size_t, std::vector<const FileEntry*>> groups;
    for (const FileEntry& e : cab.files) {
        if (!names || std::find(wanted.begin(), wanted.end(), text::lower(e.name)) != wanted.end()) {
            groups[cab.folder_of(e)].push_back(&e);
        }
    }
    std::vector<Extracted> written;
    for (const auto& [folder, entries] : groups) {
        std::uint64_t upto = 0;
        for (const FileEntry* e : entries) upto = std::max<std::uint64_t>(upto, std::uint64_t{e->offset} + e->size);
        const Bytes stream = folder_stream(cab, folder, upto);
        for (const FileEntry* e : entries) {
            const std::uint64_t end = std::uint64_t{e->offset} + e->size;
            if (end > stream.size()) bad(path_text(cabinet) + ": " + e->name + " is cut short");
            const std::filesystem::path target = safe_path(outdir, e->name);
            fs::write_file(target, ByteView(stream).subspan(e->offset, e->size));
            set_file_time(target, *e);
            auto same = std::find_if(written.begin(), written.end(), [&](const Extracted& w) { return w.name == e->name; });
            if (same != written.end()) same->path = target;
            else written.push_back({e->name, target});
        }
    }
    return written;
}

namespace {

// raw deflate of one block at the strongest setting, primed with the block before it when there is one
Bytes deflate_raw(ByteView chunk, ByteView dictionary) {
    z_stream z{};
    if (deflateInit2(&z, 9, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) != Z_OK) throw InternalError("zlib: cannot start deflating");
    struct End { z_stream& z; ~End() { deflateEnd(&z); } } end{z};
    if (!dictionary.empty()) deflateSetDictionary(&z, dictionary.data(), static_cast<uInt>(dictionary.size()));
    Bytes out(deflateBound(&z, static_cast<uLong>(chunk.size())) + 64);
    z.next_in = const_cast<Bytef*>(chunk.data());
    z.avail_in = static_cast<uInt>(chunk.size());
    z.next_out = out.data();
    z.avail_out = static_cast<uInt>(out.size());
    if (deflate(&z, Z_FINISH) != Z_STREAM_END) throw InternalError("zlib: deflate did not finish");
    out.resize(out.size() - z.avail_out);
    return out;
}

struct Block {
    Bytes payload;
    std::uint16_t uncompressed = 0;
};

std::vector<Block> mszip_blocks(ByteView stream, bool short_blocks) {
    std::vector<Block> blocks;
    Bytes previous;
    for (std::size_t at = 0; at < stream.size();) {
        ByteView chunk = stream.subspan(at, std::min(kBlock, stream.size() - at));
        Bytes body = deflate_raw(chunk, previous);
        previous.assign(chunk.begin(), chunk.end());
        if (body.size() + 2 > kBlock && short_blocks) {
            // Will not fit a 32768-byte payload: cut the block short and store it, and start the next block with no
            // dictionary so that every decoder reads it the same.
            chunk = stream.subspan(at, std::min(kShortBlock, stream.size() - at));
            const std::uint16_t n = static_cast<std::uint16_t>(chunk.size());
            io::ByteWriter w;
            w.u8(1);   // BFINAL, stored
            w.u16(n);
            w.u16(static_cast<std::uint16_t>(n ^ 0xFFFF));
            w.bytes(chunk);
            body = w.take();
            previous.clear();
        }
        Block b;
        b.payload = {'C', 'K'};
        append(b.payload, body);
        b.uncompressed = static_cast<std::uint16_t>(chunk.size());
        blocks.push_back(std::move(b));
        at += chunk.size();
    }
    return blocks;
}

// A neighbour as the header stores it: name and disk label, each NUL-terminated (latin-1).
void put_link(io::ByteWriter& w, const std::optional<SetLink>& link) {
    if (!link) return;
    w.bytes(text::to_latin1(link->cabinet));
    w.u8(0);
    w.bytes(text::to_latin1(link->disk));
    w.u8(0);
}

}  // namespace

DosTimestamp dos_timestamp_now() {
    const std::time_t now = std::time(nullptr);
    std::tm st{};
    localtime_s(&st, &now);
    DosTimestamp t;
    t.date = static_cast<std::uint16_t>(((st.tm_year + 1900 - 1980) << 9) | ((st.tm_mon + 1) << 5) | st.tm_mday);
    t.time = static_cast<std::uint16_t>((st.tm_hour << 11) | (st.tm_min << 5) | (st.tm_sec / 2));
    return t;
}

Bytes build(std::span<const SourceFile> files, const WriteOptions& options) {
    if (options.compression != Compression::MsZip && options.compression != Compression::None) {
        throw UsageError("a cabinet is written MSZIP-compressed or stored");
    }
    Bytes stream;
    for (const SourceFile& f : files) append(stream, f.data);
    std::vector<Block> blocks;
    if (options.compression == Compression::MsZip) {
        blocks = mszip_blocks(stream, options.short_blocks);
    } else {
        for (std::size_t at = 0; at < stream.size(); at += kBlock) {
            const std::size_t n = std::min(kBlock, stream.size() - at);
            blocks.push_back({Bytes(stream.begin() + static_cast<std::ptrdiff_t>(at), stream.begin() + static_cast<std::ptrdiff_t>(at + n)),
                              static_cast<std::uint16_t>(n)});
        }
    }
    if (blocks.size() > 0xFFFF || files.size() > 0xFFFF) {
        throw UnsupportedError(kReader, 0, std::format("too much for one cabinet folder ({} blocks, {} files)", blocks.size(), files.size()));
    }
    if (stream.size() > 0xFFFFFFFFull) throw UnsupportedError(kReader, 0, "more than 4 GB in one folder");

    const DosTimestamp stamp = options.timestamp.value_or(dos_timestamp_now());
    io::ByteWriter chain;
    put_link(chain, options.prev);
    put_link(chain, options.next);
    const std::uint16_t flags = static_cast<std::uint16_t>((options.prev ? kFlagPrev : 0) | (options.next ? kFlagNext : 0));

    io::ByteWriter entries;
    std::uint32_t offset = 0;
    for (const SourceFile& f : files) {
        const bool ascii_only = std::all_of(f.name.begin(), f.name.end(), [](char c) { return static_cast<unsigned char>(c) < 0x80; });
        entries.u32(static_cast<std::uint32_t>(f.data.size()));
        entries.u32(offset);
        entries.u16(0);   // folder 0
        entries.u16(stamp.date);
        entries.u16(stamp.time);
        entries.u16(ascii_only ? kAttrArchive : (kAttrArchive | kAttrUtf8Name));
        entries.cstr(f.name);
        offset += static_cast<std::uint32_t>(f.data.size());
    }
    const std::uint32_t files_at = static_cast<std::uint32_t>(kHeaderSize + chain.size() + 8);
    const std::uint32_t data_at = files_at + static_cast<std::uint32_t>(entries.size());
    io::ByteWriter body;
    for (const Block& b : blocks) {
        body.u32(0);   // no checksum
        body.u16(static_cast<std::uint16_t>(b.payload.size()));
        body.u16(b.uncompressed);
        body.bytes(b.payload);
    }
    const std::uint32_t total = data_at + static_cast<std::uint32_t>(body.size());

    io::ByteWriter out;
    out.bytes(as_bytes("MSCF"));
    out.u32(0);
    out.u32(total);
    out.u32(0);
    out.u32(files_at);
    out.u32(0);
    out.u8(3);   // version 1.3
    out.u8(1);
    out.u16(1);   // one folder
    out.u16(static_cast<std::uint16_t>(files.size()));
    out.u16(flags);
    out.u16(options.set_id);
    out.u16(options.index);
    out.bytes(chain.data());
    out.u32(data_at);   // the folder
    out.u16(static_cast<std::uint16_t>(blocks.size()));
    out.u16(options.compression == Compression::MsZip ? 1 : 0);
    out.bytes(entries.data());
    out.bytes(body.data());
    return out.take();
}

std::uint64_t write(std::span<const SourceFile> files, const std::filesystem::path& out_path, const WriteOptions& options) {
    const Bytes cabinet = build(files, options);
    fs::write_file(out_path, cabinet);
    return cabinet.size();
}

}  // namespace cb::formats::cab
