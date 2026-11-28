#include "formats/xbox/xbe.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "formats/gen/xbe.hpp"

namespace cb::formats::xbox::xbe {

namespace {

constexpr std::string_view kXbeReader = "xbe";

// The keys that scramble the entry point and the kernel thunk address: retail, debug, Chihiro (tried in this order).
constexpr std::array<std::uint32_t, 3> kXbeEntryKeys = {0xA8FC57AB, 0x94859D4B, 0x40B5C16E};
constexpr std::array<std::uint32_t, 3> kXbeThunkKeys = {0x5B6D40B6, 0xEFB1F152, 0x2290059D};
constexpr KeySet kXbeKeySets[3] = {KeySet::Retail, KeySet::Debug, KeySet::Chihiro};

constexpr std::uint64_t kXbeSlack = 0x100000;        // how far past the image an unscrambled address may land
constexpr std::uint64_t kXbeMaxImage = 1ull << 30;   // a PE image larger than this is a damaged header, not a program

struct XbeFlag {
    std::uint32_t bit;
    std::string_view name;
};
constexpr XbeFlag kXbeRegions[] = {{0x1, "North America"}, {0x2, "Japan"}, {0x4, "Rest of World"}, {0x80000000, "Manufacturing/Debug"}};
constexpr XbeFlag kXbeMedia[] = {{0x1, "HDD"},        {0x2, "DVD-X2"},       {0x4, "DVD-CD"},    {0x8, "CD"},
                                 {0x10, "DVD-5-RO"},  {0x20, "DVD-9-RO"},    {0x40, "DVD-5-RW"}, {0x80, "DVD-9-RW"},
                                 {0x100, "Dongle"},   {0x200, "Media Board"}, {0x40000000, "Nonsecure Hard Disk"}, {0x80000000, "Nonsecure Mode"}};

template <std::size_t N>
std::string xbe_flag_text(std::uint32_t value, const XbeFlag (&table)[N]) {
    std::string out;
    for (const XbeFlag& f : table) {
        if (!(value & f.bit)) continue;
        if (!out.empty()) out += '|';
        out += f.name;
    }
    return out.empty() ? std::format("0x{:X}", value) : out;
}

Unscrambled xbe_unscramble(std::uint32_t raw, const Xbe& x, const std::array<std::uint32_t, 3>& keys) {
    for (std::size_t i = 0; i < keys.size(); ++i) {
        const std::uint32_t v = raw ^ keys[i];
        if (v >= x.base && std::uint64_t{v} < std::uint64_t{x.base} + x.size_of_image + kXbeSlack) return {v, kXbeKeySets[i]};
    }
    return {raw ^ keys[0], KeySet::Unknown};
}

// The virtual address `va` as an offset into the file, which maps the image from `base`.
std::uint64_t xbe_file_offset(std::uint32_t va, std::uint32_t base, std::string_view what) {
    if (va < base) throw FormatError(kXbeReader, va, std::string(what) + " lies below the image base");
    return std::uint64_t{va} - base;
}

}  // namespace

std::string_view key_set_name(KeySet keys) {
    switch (keys) {
        case KeySet::Retail: return "retail";
        case KeySet::Debug: return "debug";
        case KeySet::Chihiro: return "chihiro";
        case KeySet::Unknown: break;
    }
    return "unknown";
}

// The layout is xbe.fmt's (its generated reader, formats/gen/xbe).
Xbe Xbe::parse(ByteView file) {
    if (file.size() >= 4 && as_text(file.first(4)) != kMagic) throw FormatError(kXbeReader, 0, "not an XBE (bad magic)");   // (shorter: the read says it is cut short)
    const gen::xbe::File h = gen::xbe::read(file);
    Xbe x;
    x.file_.assign(file.begin(), file.end());
    x.base = h.base;
    x.size_of_headers = h.size_of_headers;
    x.size_of_image = h.size_of_image;
    x.cert_addr = h.cert_addr;
    x.section_count = h.section_count;
    x.section_header_addr = h.section_header_addr;
    x.init_flags = h.init_flags;
    x.entry_raw = h.entry_raw;
    x.thunk_raw = h.thunk_raw;   // the kernel image thunk address (0x148 is the PE time stamp: 2369d3631)
    const gen::xbe::Certificate& c = h.certificate;
    x.cert_size = c.size;
    x.title_id = c.title_id;
    x.title_name = text::from_utf16le(c.title_name);   // 40 units, ended by the first zero
    x.title_name.resize(std::min(x.title_name.find(std::string_view("", 1)), x.title_name.size()));
    x.allowed_media = c.allowed_media;
    x.region = c.region;
    x.ratings = c.ratings;
    x.disk_number = c.disk_number;
    x.version = c.version;
    return x;
}

Unscrambled Xbe::entry_point() const { return xbe_unscramble(entry_raw, *this, kXbeEntryKeys); }
Unscrambled Xbe::kernel_thunk() const { return xbe_unscramble(thunk_raw, *this, kXbeThunkKeys); }

std::vector<Section> Xbe::sections() const {
    std::vector<Section> out;
    (void)xbe_file_offset(section_header_addr, base, "the section table");
    gen::xbe::Options options;
    options.parts.sections = true;
    const gen::xbe::File h = gen::xbe::read(file_, options);
    if (!h.sections) return out;
    out.reserve(h.sections->size());
    for (const gen::xbe::Section& n : *h.sections) {
        Section s;
        s.flags = n.flags;
        s.vaddr = n.vaddr;
        s.vsize = n.vsize;
        s.raw_addr = n.raw_addr;
        s.raw_size = n.raw_size;
        if (n.name) {   // a name the file cuts off ends with the file
            const std::string_view raw = n.name->substr(0, n.name->find(std::string_view("", 1)));
            s.name = text::from_latin1(as_bytes(raw));
        }
        out.push_back(std::move(s));
    }
    return out;
}

std::string Xbe::title_id_text() const {
    const std::uint32_t publisher = (title_id >> 16) & 0xFFFF, game = title_id & 0xFFFF;
    const std::uint32_t a = (publisher >> 8) & 0xFF, b = publisher & 0xFF;
    const bool letters = a >= 0x20 && a < 0x7F && b >= 0x20 && b < 0x7F;
    const std::string pub = letters ? std::string{static_cast<char>(a), static_cast<char>(b)} : std::format("{:04X}", publisher);
    return std::format("{}-{:03}", pub, game);
}

std::string Xbe::region_text() const { return xbe_flag_text(region, kXbeRegions); }
std::string Xbe::media_text() const { return xbe_flag_text(allowed_media, kXbeMedia); }

Bytes Xbe::pe_image() const {
    std::vector<Section> present;
    for (Section& s : sections()) {
        if (s.vsize || s.raw_size) present.push_back(std::move(s));
    }
    const std::uint32_t entry = entry_point().address, thunk = kernel_thunk().address;

    std::uint64_t want = size_of_image;
    for (const Section& s : present) want = std::max(want, xbe_file_offset(s.vaddr, base, "a section") + std::max(s.vsize, s.raw_size));
    if (want > kXbeMaxImage) throw FormatError(kXbeReader, 0, std::format("the image would be {} bytes", want));

    Bytes image(static_cast<std::size_t>(want), 0);
    // the XBE's own header first (the PE headers take its place below)
    const std::size_t head = std::min<std::size_t>({file_.size(), size_of_headers, image.size()});
    std::copy_n(file_.begin(), head, image.begin());
    for (const Section& s : present) {
        const std::size_t at = static_cast<std::size_t>(xbe_file_offset(s.vaddr, base, "a section"));
        const std::size_t n = std::min(s.raw_size, s.vsize ? s.vsize : s.raw_size);
        // a section whose bytes the file cuts short is as long as the file holds
        const std::size_t have = s.raw_addr < file_.size() ? std::min<std::size_t>(n, file_.size() - s.raw_addr) : 0;
        if (have) std::copy_n(file_.begin() + static_cast<std::ptrdiff_t>(s.raw_addr), have, image.begin() + static_cast<std::ptrdiff_t>(at));
    }

    // the kernel thunk table is the import address table of xboxkrnl.exe: one import descriptor, appended after the image
    const std::uint32_t thunk_rva = static_cast<std::uint32_t>(xbe_file_offset(thunk, base, "the kernel thunk table"));
    const std::uint32_t imp = static_cast<std::uint32_t>((image.size() + 0xFFF) & ~std::size_t{0xFFF});
    std::uint32_t count = 0;
    while (std::uint64_t{thunk_rva} + 4 * std::uint64_t{count} + 4 <= image.size()) {
        const std::size_t at = thunk_rva + 4 * std::size_t{count};
        if (!(image[at] | image[at + 1] | image[at + 2] | image[at + 3])) break;
        ++count;
    }
    io::ByteWriter directory;
    directory.u32(thunk_rva);        // OriginalFirstThunk
    directory.u32(0);                // TimeDateStamp
    directory.u32(0);                // ForwarderChain
    directory.u32(imp + 40);         // Name: the string after this descriptor and its terminator
    directory.u32(thunk_rva);        // FirstThunk
    directory.zeros(20);             // the terminating descriptor
    directory.cstr("xboxkrnl.exe");
    image.resize(imp, 0);
    append(image, directory.data());
    image.resize((image.size() + 0xFFF) & ~std::size_t{0xFFF}, 0);

    struct Span {
        std::string name;   // PE section names are 8 bytes at most
        std::uint32_t rva, size, flags;
    };
    std::vector<Span> spans;
    for (const Section& s : present) {
        Bytes raw_name = text::to_latin1(s.name);
        raw_name.resize(std::min<std::size_t>(raw_name.size(), 8));
        spans.push_back({std::string(as_text(raw_name)), static_cast<std::uint32_t>(xbe_file_offset(s.vaddr, base, "a section")),
                         std::max(s.vsize, s.raw_size), s.flags});
    }
    const std::size_t sections_before_import = spans.size();
    spans.push_back({".cbimp", imp, static_cast<std::uint32_t>(directory.size()), 0});

    const std::size_t headers = 0x40 + 4 + 20 + 224 + 40 * spans.size();
    if (sections_before_import) {
        std::uint32_t lowest = 0xFFFFFFFF;
        for (const Span& s : spans) lowest = std::min(lowest, s.rva);
        if (headers > lowest) throw FormatError(kXbeReader, 0, "the XBE's first section starts too low for PE headers");
    }
    if (entry < base) throw FormatError(kXbeReader, entry, "the entry point lies below the image base");

    io::ByteWriter pe;
    pe.u8('M');
    pe.u8('Z');
    pe.zeros(0x3A);
    pe.u32(0x40);   // where the PE signature is
    pe.u8('P');
    pe.u8('E');
    pe.u16(0);
    // COFF file header: x86, the sections, no symbols, a 224-byte optional header, executable | 32-bit machine
    pe.u16(0x014C);
    pe.u16(static_cast<std::uint16_t>(spans.size()));
    pe.zeros(12);
    pe.u16(224);
    pe.u16(0x0102);
    // optional header (PE32)
    pe.u16(0x10B);
    pe.u16(0);                       // linker version
    pe.zeros(12);                    // sizes of code, initialised data, uninitialised data
    pe.u32(entry - base);            // entry point (RVA)
    pe.zeros(8);                     // base of code, base of data
    pe.u32(base);
    pe.u32(0x20);                    // section alignment
    pe.u32(0x20);                    // file alignment
    pe.u16(4); pe.u16(0);            // operating system version
    pe.u16(0); pe.u16(0);            // image version
    pe.u16(4); pe.u16(0);            // subsystem version
    pe.u32(0);                       // Win32 version
    pe.u32(static_cast<std::uint32_t>(image.size()));
    pe.u32(static_cast<std::uint32_t>(headers));
    pe.u32(0);                       // checksum
    pe.u16(14);                      // subsystem: Xbox
    pe.u16(0);                       // DLL characteristics
    pe.u32(0x100000); pe.u32(0x1000);   // stack reserve, commit
    pe.u32(0x100000); pe.u32(0x1000);   // heap reserve, commit
    pe.u32(0);                       // loader flags
    pe.u32(16);                      // data directories
    pe.zeros(8);                                                  // 0 export
    pe.u32(imp); pe.u32(static_cast<std::uint32_t>(directory.size()));   // 1 import
    pe.zeros(8 * 10);                                             // 2..11
    pe.u32(thunk_rva); pe.u32(4 * (count + 1));                   // 12 import address table
    pe.zeros(8 * 3);                                              // 13..15
    for (const Span& s : spans) {
        Bytes name(8, 0);
        std::copy_n(s.name.begin(), std::min<std::size_t>(s.name.size(), 8), name.begin());
        pe.bytes(name);
        pe.u32(s.size);              // virtual size
        pe.u32(s.rva);
        pe.u32(s.size);              // size of raw data: the image is flat
        pe.u32(s.rva);               // pointer to raw data: the same
        pe.zeros(12);                // relocations, line numbers, their counts
        std::uint32_t characteristics = 0x40000040;   // readable | initialised data
        if (s.name != ".cbimp") {
            if (s.flags & kSectionExecutable) characteristics |= 0x20000020;   // code | executable
            if (s.flags & kSectionWritable) characteristics |= 0x80000000;     // writable
        }
        pe.u32(characteristics);
    }
    CB_ASSERT(pe.size() == headers, "the PE headers are the size they were counted to be");
    std::copy(pe.data().begin(), pe.data().end(), image.begin());
    return image;
}

std::filesystem::path to_pe(const std::filesystem::path& path, const std::filesystem::path& folder) {
    const Bytes data = fs::read_file(path);
    const std::string digest = crypto::to_hex(crypto::sha1(data)).substr(0, 12);
    const std::filesystem::path out = folder / (text::lower(fs::to_text(path.stem())) + "-" + digest + ".exe");
    if (!std::filesystem::is_regular_file(out)) fs::write_file_atomic(out, Xbe::parse(data).pe_image());
    return out;
}

// ---- the command line ----------------------------------------------------------------------------------------------------------

namespace {

Xbe xbe_open(const cli::Args& a) { return Xbe::parse(fs::read_file(fs::from_text(a.required("file")))); }

int xbe_print_info(const cli::Args& a) {
    const Xbe x = xbe_open(a);
    const Unscrambled entry = x.entry_point(), thunk = x.kernel_thunk();
    out("magic        : XBEH\n");
    out("title        : '{}'\n", x.title_name);
    out("title id     : 0x{:08X}  ({})\n", x.title_id, x.title_id_text());
    out("base / size  : 0x{:08X} / {} bytes\n", x.base, x.size_of_image);
    out("entry point  : 0x{:08X}  ({} key)\n", entry.address, key_set_name(entry.keys));
    out("kernel thunk : 0x{:08X}  ({} key)\n", thunk.address, key_set_name(thunk.keys));
    out("region       : {}\n", x.region_text());
    out("media        : {}\n", x.media_text());
    out("ratings      : 0x{:X}   version: {}   disk: {}\n", x.ratings, x.version, x.disk_number);
    out("sections     : {}\n", x.section_count);
    return kExitOk;
}

}  // namespace

cli::Command command() {
    cli::Command c;
    c.name = "xbe";
    c.summary = "read an original Xbox XBE executable: header, certificate, sections, entry point";
    c.area = "Xbox";
    const cli::Positional file{"file", "the default.xbe"};
    c.subcommands.push_back(cli::Command{"info", "the header and certificate", {file}, xbe_print_info});
    c.subcommands.push_back(cli::Command{"sections", "the section table", {file}, [](const cli::Args& a) {
        const Xbe x = xbe_open(a);
        for (const Section& s : x.sections()) {
            out("  {:<12} va=0x{:08X} vsize={:<8} raw=0x{:06X} size={:<8} flags=0x{:X}\n", s.name, s.vaddr, s.vsize, s.raw_addr, s.raw_size, s.flags);
        }
        out("total: {} sections\n", x.section_count);
        return kExitOk;
    }});
    return c;
}

}  // namespace cb::formats::xbox::xbe
