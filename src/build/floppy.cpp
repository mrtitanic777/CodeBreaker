#include "build/floppy.hpp"

#include <algorithm>
#include <format>

#include "build/x86asm/x86asm.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "formats/dos.hpp"

namespace cb::build::floppy {

namespace {

namespace stdfs = std::filesystem;

constexpr std::uint8_t kLabel = 0x08, kFolder = 0x10, kLongName = 0x0F;   // directory entry attributes
constexpr std::uint32_t kFat12End = 0xFF8;                                // a chain value from here on ends the chain
constexpr std::size_t kBpbAt = 11;                                         // the parameter block's place in the boot sector
constexpr std::size_t kMediaByteAt = 21;

// The clusters of a file, from its first: the chain the FAT states (a chain that loops is cut where it comes round).
std::vector<std::uint32_t> chain(ByteView fat, std::uint32_t first) {
    std::vector<std::uint32_t> clusters;
    for (std::uint32_t c = first; c >= 2 && c < kFat12End && clusters.size() <= fat.size(); ) {
        clusters.push_back(c);
        const std::size_t at = static_cast<std::size_t>(c) * 3 / 2;
        if (at + 2 > fat.size()) break;
        const std::uint32_t word = fat[at] | (fat[at + 1] << 8);
        c = (c & 1) != 0 ? word >> 4 : word & 0xFFF;
    }
    return clusters;
}

// A 12-bit FAT entry.
void put_fat(Bytes& fat, std::uint32_t cluster, std::uint32_t value) {
    const std::size_t at = static_cast<std::size_t>(cluster) * 3 / 2;
    if (at + 2 > fat.size()) throw DoNotFitError("the files do not fit on the floppy (its FAT is full)");
    std::uint32_t word = fat[at] | (fat[at + 1] << 8);
    word = (cluster & 1) != 0 ? (word & 0x000F) | (value << 4) : (word & 0xF000) | value;
    fat[at] = static_cast<std::uint8_t>(word);
    fat[at + 1] = static_cast<std::uint8_t>(word >> 8);
}

void put_entry_fields(std::array<std::uint8_t, kEntrySize>& raw, std::uint16_t first, std::uint32_t size) {
    raw[26] = static_cast<std::uint8_t>(first);
    raw[27] = static_cast<std::uint8_t>(first >> 8);
    for (int k = 0; k < 4; ++k) raw[28 + k] = static_cast<std::uint8_t>(size >> (8 * k));
}

}  // namespace

bool Bpb::same_layout(const Bpb& other) const {
    return bytes_per_sector == other.bytes_per_sector && sectors_per_cluster == other.sectors_per_cluster && reserved == other.reserved &&
           fats == other.fats && root_entries == other.root_entries && total_sectors == other.total_sectors &&
           sectors_per_fat == other.sectors_per_fat;
}

Bpb bpb_of(ByteView boot) {
    io::ByteReader r(boot, "floppy boot sector");
    r.seek(kBpbAt);
    Bpb b;
    b.bytes_per_sector = r.u16();
    b.sectors_per_cluster = r.u8();
    b.reserved = r.u16();
    b.fats = r.u8();
    b.root_entries = r.u16();
    b.total_sectors = r.u16();
    b.media = r.u8();
    b.sectors_per_fat = r.u16();
    b.root_at = (std::size_t{b.reserved} + std::size_t{b.fats} * b.sectors_per_fat) * b.bytes_per_sector;
    b.data_at = b.root_at + std::size_t{b.root_entries} * kEntrySize;
    return b;
}

std::string Entry::name() const {
    const auto field = [&](std::size_t from, std::size_t count) {
        return std::string(text::trim(text::from_latin1(ByteView(raw).subspan(from, count))));
    };
    const std::string base = field(0, 8), extension = field(8, 3);
    return base + (extension.empty() ? "" : "." + extension);
}

bool Entry::is_file() const { return (raw[11] & (kLabel | kFolder)) == 0 && raw[11] != kLongName; }

Disk read(ByteView image) {
    Disk disk;
    disk.bpb = bpb_of(image);
    const Bpb& b = disk.bpb;
    if (image.size() < kSectorSize) throw FormatError("floppy", 0, "the image is shorter than a boot sector");
    disk.boot.assign(image.begin(), image.begin() + kSectorSize);
    const auto slice = [&](std::size_t from, std::size_t size) {
        from = std::min(from, image.size());
        return image.subspan(from, std::min(size, image.size() - from));
    };
    const ByteView fat = slice(std::size_t{b.reserved} * b.bytes_per_sector, std::size_t{b.sectors_per_fat} * b.bytes_per_sector);
    const std::size_t cluster = std::size_t{b.sectors_per_cluster} * b.bytes_per_sector;
    for (std::size_t i = 0; i < b.root_entries; ++i) {
        const ByteView raw = slice(b.root_at + kEntrySize * i, kEntrySize);
        if (raw.size() < kEntrySize || raw[0] == 0) break;
        if (raw[0] == 0xE5) continue;
        Entry entry;
        std::copy(raw.begin(), raw.end(), entry.raw.begin());
        if (entry.is_file()) {
            const std::uint32_t first = raw[26] | (raw[27] << 8);
            const std::uint32_t size = raw[28] | (raw[29] << 8) | (raw[30] << 16) | (std::uint32_t{raw[31]} << 24);
            Bytes data;
            for (const std::uint32_t c : chain(fat, first)) append(data, slice(b.data_at + (std::size_t{c} - 2) * cluster, cluster));
            data.resize(std::min<std::size_t>(data.size(), size));
            entry.data = std::move(data);
        }
        disk.entries.push_back(std::move(entry));
    }
    return disk;
}

Bytes make(const Disk& disk, ByteView boot, const std::map<std::string, Bytes>& contents) {
    const Bpb& b = disk.bpb;
    if (!bpb_of(boot).same_layout(b)) throw UsageError("the boot sector's parameters are not the original floppy's");
    Bytes img(std::size_t{b.total_sectors} * b.bytes_per_sector, 0);
    if (boot.size() < kSectorSize || img.size() < kSectorSize) throw UsageError("the boot sector is shorter than a sector");
    std::copy(boot.begin(), boot.begin() + kSectorSize, img.begin());
    const std::size_t cluster = std::size_t{b.sectors_per_cluster} * b.bytes_per_sector;
    Bytes fat(std::size_t{b.sectors_per_fat} * b.bytes_per_sector, 0);
    put_fat(fat, 0, 0xF00 | boot[kMediaByteAt]);   // the media byte
    put_fat(fat, 1, 0xFFF);
    std::size_t next = 2;
    for (std::size_t i = 0; i < disk.entries.size(); ++i) {
        const Entry& entry = disk.entries[i];
        std::array<std::uint8_t, kEntrySize> raw = entry.raw;
        if (entry.is_file()) {
            const auto given = contents.find(text::upper(entry.name()));
            const Bytes& data = given != contents.end() ? given->second : entry.data.value_or(Bytes{});
            const std::size_t count = (data.size() + cluster - 1) / cluster;
            if (next + count - 2 > (img.size() - b.data_at) / cluster) throw DoNotFitError("the files do not fit on the floppy");
            const std::size_t first = count != 0 ? next : 0;
            for (std::size_t k = 0; k < count; ++k) put_fat(fat, static_cast<std::uint32_t>(next + k), k + 1 < count ? static_cast<std::uint32_t>(next + k + 1) : 0xFFF);
            const std::size_t at = b.data_at + (next - 2) * cluster;
            std::copy(data.begin(), data.end(), img.begin() + static_cast<std::ptrdiff_t>(at));
            next += count;
            put_entry_fields(raw, static_cast<std::uint16_t>(first), static_cast<std::uint32_t>(data.size()));
        }
        std::copy(raw.begin(), raw.end(), img.begin() + static_cast<std::ptrdiff_t>(b.root_at + kEntrySize * i));
    }
    for (unsigned k = 0; k < b.fats; ++k) {
        const std::size_t at = (std::size_t{b.reserved} + std::size_t{k} * b.sectors_per_fat) * b.bytes_per_sector;
        std::copy(fat.begin(), fat.end(), img.begin() + static_cast<std::ptrdiff_t>(at));
    }
    return img;
}

namespace {

// The boot sector assembled from its source by x86asm (CodeBreaker's own assembler): (its bytes, the source's path).
std::pair<Bytes, stdfs::path> boot_sector(const Environment& env) {
    const stdfs::path source = env.design_dir / "recon" / "bootdisk" / "msboot.asm";
    fs::make_dirs(env.state / "bootfloppy");
    const stdfs::path out = env.state / "bootfloppy" / "msboot.bin";
    x86asm::Request request;
    request.cwd = stdfs::current_path().generic_string();
    request.args = {"-f", "bin", fs::to_text(source), "-o", fs::to_text(out)};
    request.read = [](const std::string& path) -> std::optional<Bytes> {   // (the source, and anything it includes, from the disk)
        std::error_code ec;
        if (!stdfs::is_regular_file(fs::from_text(path), ec)) return std::nullopt;
        return fs::read_file(fs::from_text(path));
    };
    const x86asm::Result result = x86asm::assemble(request);
    if (result.exit != 0) throw ToolError("x86asm", static_cast<std::uint32_t>(result.exit), result.messages, "the boot sector did not assemble");
    for (const auto& [file, bytes] : result.files) fs::write_file(fs::from_text(file), bytes);
    return {fs::read_file(out), source};
}

// The file built from a program's source in <src_dir>/<NAME>, or nothing: the program laid out as the original file is (dos_writer: file/<NAME>),
// else as built.
std::optional<stdfs::path> rebuilt(const stdfs::path& src_dir, const std::string& name) {
    const stdfs::path folder = src_dir / name;
    if (!stdfs::is_directory(folder)) return std::nullopt;
    const std::string stem = text::lower(fs::to_text(stdfs::path(name).stem()));
    for (const stdfs::path& candidate : {stdfs::path("file") / text::upper(name), stdfs::path(text::lower(name)), stdfs::path(stem + ".exe"),
                                         stdfs::path(stem + ".com"), stdfs::path(stem + ".bin")}) {
        std::error_code ec;
        if (stdfs::is_regular_file(folder / candidate, ec)) return folder / candidate;
    }
    return std::nullopt;
}

bool is_text(ByteView data) {
    return std::all_of(data.begin(), data.end(), [](std::uint8_t c) { return (c >= 32 && c < 127) || c == 9 || c == 10 || c == 13 || c == 26; });
}

// How an MZ program's load module is stored compressed, or none.
std::optional<std::string> packer_of_file(const stdfs::path& file) {
    const Bytes data = fs::read_file(file);
    const auto header = formats::dos::mz_header(data);
    if (!header) return std::nullopt;
    const auto packer = formats::dos::packer_of(data, *header);
    if (!packer) return std::nullopt;
    return formats::dos::packer_name(*packer);
}

std::string quiet_relative(const stdfs::path& path, const stdfs::path& base) { return stdfs::relative(path, base).generic_string(); }

}  // namespace

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "boot-floppy";
    c.summary = "the Win95 Setup boot floppy, made from the rebuilt files";
    c.area = "Build";
    c.params = {
        cli::Option{"--image", "", "the original floppy (default: setup/media/boot.img of the corpus)", cli::Arg::Value, "ORIGINAL.img"},
        cli::Option{"--out", "", "where to write the floppy (default: <state>/bootfloppy/boot95.img)", cli::Arg::Value, "FILE.img"},
        cli::Option{"--extract", "", "write the original floppy's files to DIR and stop", cli::Arg::Value, "DIR"},
    };
    c.handler = [environment](const cli::Args& a) -> int {
        const stdfs::path image = a.value("image") ? fs::from_text(*a.value("image")) : environment.corpus_root / "setup" / "media" / "boot.img";
        const Disk disk = read(fs::read_file(image));
        if (const auto target = a.value("extract")) {
            const stdfs::path folder = fs::from_text(*target);
            fs::make_dirs(folder);
            std::size_t count = 0;
            for (const Entry& e : disk.entries) {
                if (!e.is_file()) continue;
                fs::write_file(folder / e.name(), *e.data);
                ++count;
            }
            out("{} files written to {}\n", count, *target);
            return kExitOk;
        }
        const stdfs::path bootfloppy = environment.state / "bootfloppy";
        const stdfs::path original = bootfloppy / "original", src_dir = bootfloppy / "src";
        fs::make_dirs(original);
        const auto [boot, boot_source] = boot_sector(environment);
        out("boot sector: {} ({} the original's)\n", fs::to_text(boot_source.filename()), boot == disk.boot ? "the same as" : "DIFFERENT from");
        std::map<std::string, Bytes> contents;
        std::vector<std::string> rebuilt_order;   // the names in `contents`, in the directory's order
        std::vector<std::string> kept;
        for (const Entry& e : disk.entries) {
            if (!e.is_file()) continue;
            const std::string name = e.name();
            const stdfs::path path = original / name;
            if (!stdfs::is_regular_file(path)) fs::write_file(path, *e.data);
            if (!formats::dos::kind_of(*e.data, name)) {
                const bool plain = is_text(*e.data);
                out("  {:<13} {}\n", name, plain ? "text: as it is" : "not a real-mode program: the original's");
                if (!plain) kept.push_back(name);
                continue;
            }
            std::optional<stdfs::path> built = rebuilt(src_dir, name);
            if (!built) {
                const source::dos_writer::Result r = source::dos_writer::reconstruct(path, src_dir / name, environment.writer);
                if (r.same || r.equivalent) built = r.file ? r.file : r.image;
            }
            if (!built) {
                out("  {:<13} does not build back: the original's\n", name);
                kept.push_back(name);
                continue;
            }
            contents[text::upper(name)] = fs::read_file(*built);
            rebuilt_order.push_back(text::upper(name));
            out("  {:<13} rebuilt from source ({})\n", name, quiet_relative(*built, src_dir));
        }
        // a program the original stores packed by a packer CodeBreaker does not write (PKLITE ...) is rebuilt unpacked, and bigger: when the disk
        // is too small for them all, the biggest of them go on as the original stores them, until everything fits
        std::vector<std::string> unpacked;
        for (const std::string& name : rebuilt_order) {
            const auto packer = packer_of_file(original / name);
            if (packer && *packer != "EXEPACK" && *packer != "moves itself" && *packer != "PKLITE") unpacked.push_back(name);
        }
        std::stable_sort(unpacked.begin(), unpacked.end(), [&](const std::string& x, const std::string& y) { return contents.at(x).size() > contents.at(y).size(); });
        Bytes img;
        for (;;) {
            try {
                img = make(disk, boot, contents);
                break;
            } catch (const DoNotFitError&) {
                if (unpacked.empty()) throw;
                const std::string name = unpacked.front();
                unpacked.erase(unpacked.begin());
                contents.erase(name);
                kept.push_back(std::format("{} (stored packed by {}; rebuilt unpacked it does not fit)", name, packer_of_file(original / name).value_or("?")));
            }
        }
        const std::string target = a.value("out") ? *a.value("out") : fs::to_text(bootfloppy / "boot95.img");
        fs::write_file(fs::from_text(target), img);
        out("{}: {} programs rebuilt from source; the original's: {}\n", target, contents.size(), kept.empty() ? "none" : text::join(kept, ", "));
        return kExitOk;
    };
    return c;
}

}  // namespace cb::build::floppy
