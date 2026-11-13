#include "cb/formats/xbox/crypto/xcp_dumper.hpp"

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/cab.hpp"
#include "cb/formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::xcp_dumper {

namespace {

namespace stdfs = std::filesystem;

constexpr std::size_t kCabHeaderSize = 0x28;       // the cabinet header as the tool reads it
constexpr std::size_t kCabHeaderBytes = 0x60;      // the part of the file that is the encrypted header
constexpr std::size_t kRecordSize = 0x1C;          // 20-byte checksum, 8-byte confounder
constexpr std::size_t kFolderRecordSize = 8;
constexpr std::size_t kXenonDataSize = 0x1C;
constexpr std::size_t kFolderTableAt = 0x180;
constexpr std::size_t kFileEntrySize = 16;
constexpr std::uint64_t kHeaderRecordAt = 0x60;
constexpr std::uint64_t kFolderTableRecordAt = 0x28;
constexpr std::uint64_t kNamesRecordAt = 0x44;
constexpr std::size_t kMaxRenamed = 1000;           // the new names are three digits

struct CabHead {
    std::uint32_t magic = 0;
    std::uint32_t files_at = 0;
    std::uint16_t folder_count = 0;
    std::uint16_t file_count = 0;
};

CabHead parse_head(ByteView data) {
    if (data.size() < kCabHeaderSize) throw TruncatedError("xcp", 0, kCabHeaderSize, data.size());
    io::ByteReader r(data, "xcp");
    CabHead head;
    head.magic = r.u32();
    r.seek(0x10);
    head.files_at = r.u32();
    r.seek(0x1A);
    head.folder_count = r.u16();
    head.file_count = r.u16();
    return head;
}

bool is_cabinet(std::uint32_t magic) { return magic == kCabMagic || magic == kCabMagicSwapped; }

// The RC4 whose key is made from the record at `at`; the confounder has already gone through it.
xecrypt::Rc4 cipher_from_record(StreamIO& stream, ByteView key, std::uint64_t at) {
    const Bytes record = stream.read_at(at, kRecordSize);
    if (record.size() < kRecordSize) throw TruncatedError("xcp", at, kRecordSize, record.size());
    const Bytes rc4_key = xecrypt::hmac_sha(key, ByteView(record).first(0x14));
    xecrypt::Rc4 cipher(rc4_key);
    (void)cipher.decrypt(ByteView(record).subspan(0x14, 8));
    return cipher;
}

// Decrypt `size` bytes at `data_at` in place with the record at `record_at`'s cipher; the plain bytes.
Bytes decrypt_piece(StreamIO& stream, ByteView key, std::uint64_t record_at, std::uint64_t data_at, std::size_t size) {
    xecrypt::Rc4 cipher = cipher_from_record(stream, key, record_at);
    return stream.perform_function_at(data_at, size, [&cipher](ByteView in) { return cipher.decrypt(in); });
}

struct Folder {
    std::uint32_t data_at = 0;
    std::uint64_t record_at = 0;
};

}  // namespace

bool is_encrypted(ByteView head) {
    if (head.size() < 4) return true;
    return !is_cabinet(io::ByteReader(head, "xcp").u32());
}

bool decrypt_in_place(StreamIO& stream, const std::optional<Bytes>& key) {
    const bool encrypted = is_encrypted(stream.read_at(0, 4));
    if (encrypted && (!key || key->empty())) throw UsageError("the package is encrypted: a key is needed (--key)");
    const ByteView key_view = key ? ByteView(*key) : ByteView{};

    CabHead head = parse_head(stream.read_at(0, kCabHeaderSize));
    if (encrypted) {
        cb::out("Decrypting CAB header...\n");
        // the header is decrypted in memory first: a key that does not give a cabinet leaves the file as it was
        xecrypt::Rc4 header_cipher = cipher_from_record(stream, key_view, kHeaderRecordAt);
        const Bytes header = header_cipher.decrypt(stream.read_at(0, kCabHeaderBytes));
        head = parse_head(header);
        if (!is_cabinet(head.magic)) throw FormatError("xcp", 0, "invalid key: the decrypted header is not a cabinet");
        stream.write_at(0, header);
        cb::out("Key appears to be OK!\n");

        cb::out("Decrypting folder data...\n");
        decrypt_piece(stream, key_view, kFolderTableRecordAt, kFolderTableAt, std::size_t{head.folder_count} * (kFolderRecordSize + kXenonDataSize));
    }

    cb::out("Processing filenames...\n");
    if (head.file_count > kMaxRenamed) throw UsageError(std::format("a package of {} files: the names can be renumbered for {} at most", head.file_count, kMaxRenamed));
    std::optional<xecrypt::Rc4> names;
    if (encrypted) names = cipher_from_record(stream, key_view, kNamesRecordAt);
    std::uint64_t at = head.files_at;
    for (std::size_t i = 0; i < head.file_count; ++i) {
        if (names) stream.perform_function_at(at, kFileEntrySize, [&names](ByteView in) { return names->decrypt(in); });
        at += kFileEntrySize;
        const std::uint64_t name_start = at;
        std::uint8_t byte = 0;
        do {
            const Bytes one = names ? stream.perform_function_at(at, 1, [&names](ByteView in) { return names->decrypt(in); }) : stream.read_at(at, 1);
            if (one.empty()) throw TruncatedError("xcp", at, 1, 0);
            byte = one[0];
            ++at;
        } while (byte != 0);
        if (at - name_start < 4) throw FormatError("xcp", name_start, "a file name too short to be renumbered");
        const std::string index = std::format("{:03}", i);
        stream.write_at(at - 4, as_bytes(index));
    }

    if (encrypted) {
        cb::out("Preprocessing folders...\n");
        std::vector<Folder> folders;
        std::uint64_t pos = kFolderTableAt;
        for (std::size_t i = 0; i < head.folder_count; ++i) {
            const Bytes record = stream.read_at(pos, kFolderRecordSize);
            if (record.size() < kFolderRecordSize) throw TruncatedError("xcp", pos, kFolderRecordSize, record.size());
            folders.push_back({io::ByteReader(record, "xcp folder").u32(), pos});
            pos += kFolderRecordSize + kXenonDataSize;
        }
        cb::out("Processing folders...\n");
        for (std::size_t i = 0; i < folders.size(); ++i) {
            const std::uint64_t end = i + 1 < folders.size() ? folders[i + 1].data_at : stream.length();
            if (end < folders[i].data_at) throw FormatError("xcp", folders[i].record_at, "folders out of order");
            decrypt_piece(stream, key_view, folders[i].record_at + kFolderRecordSize, folders[i].data_at, static_cast<std::size_t>(end - folders[i].data_at));
        }
    }
    return encrypted;
}

namespace {

// A file moved: renamed when it can be, copied when the folders are on different volumes.
void move_file(const stdfs::path& from, const stdfs::path& to) {
    std::error_code ec;
    stdfs::rename(from, to, ec);
    if (!ec) return;
    stdfs::copy_file(from, to, stdfs::copy_options::overwrite_existing);
    stdfs::remove(from);
}

// `parts` appended to `out`, in 1 MiB pieces.
void join_files(const std::vector<stdfs::path>& parts, const stdfs::path& out) {
    std::ofstream writer(out, std::ios::binary | std::ios::trunc);
    if (!writer) throw IoError(out, 0, "cannot write");
    Bytes chunk(1 << 20);
    for (const stdfs::path& part : parts) {
        std::ifstream reader(part, std::ios::binary);
        if (!reader) throw IoError(part, 0, "cannot read");
        while (reader) {
            reader.read(reinterpret_cast<char*>(chunk.data()), static_cast<std::streamsize>(chunk.size()));
            writer.write(reinterpret_cast<const char*>(chunk.data()), reader.gcount());
        }
    }
    if (!writer) throw IoError(out, 0, "cannot write");
}

}  // namespace

stdfs::path unpack(const stdfs::path& xcp, const std::optional<Bytes>& key, const Options& options) {
    if (!stdfs::is_regular_file(xcp)) throw NotFoundError(xcp, 0, "the specified input file doesn't exist");
    const std::string stem = fs::to_text(xcp.stem());
    const fs::ScopedTempDir work("cb-xcp-" + text::upper(stem));
    const stdfs::path out_dir = xcp.has_parent_path() ? xcp.parent_path() : stdfs::path(".");

    if (options.backup) {
        cb::out("Backing up the input file...\n");
        stdfs::copy_file(xcp, out_dir / (stem + ".bak"), stdfs::copy_options::overwrite_existing);
    } else {
        cb::out("Skipping backup...\n");
    }

    cb::out("Extracting XCP file...\n");
    {
        StreamIO stream = StreamIO::open(xcp);
        cb::out("Checking CAB header...\n");
        cb::out("XCP file is {}\n", is_encrypted(stream.read_at(0, 4)) ? "encrypted" : "not encrypted");
        decrypt_in_place(stream, key);
    }

    cb::out("Creating output directory...\n");
    cb::out("Renaming XCP file...\n");
    const stdfs::path cabinet = work.path() / "tmp.cab";
    move_file(xcp, cabinet);

    cb::out("Extracting CAB file...\n");
    const stdfs::path cache = work.path() / "cache";
    fs::make_dirs(cache);
    std::vector<stdfs::path> parts;
    for (const formats::cab::Extracted& file : formats::cab::extract(cabinet, cache)) parts.push_back(file.path);
    std::sort(parts.begin(), parts.end(), [](const stdfs::path& a, const stdfs::path& b) { return a.filename() < b.filename(); });

    cb::out("Merging the extracted files...\n");
    const std::string live_name = text::upper(stem);
    const stdfs::path merged = work.path() / live_name;
    join_files(parts, merged);
    cb::out("Converting to a LIVE package...\n");
    {
        StreamIO live = StreamIO::open(merged);
        live.write_at(0, as_bytes("LIVE"));
    }

    cb::out("Moving file...\n");
    const stdfs::path result = out_dir / live_name;
    move_file(merged, result);
    cb::out("Done!\n");
    return result;
}

cli::Command command() {
    return cli::Command{
        "xcp_dumper",
        "decrypt an XCP package and convert it to a LIVE package (the input file is consumed)",
        {cli::Positional{"input", "the XCP file"},
         cli::Option{"--key", "-k", "the key (hex) the package is encrypted with", cli::Arg::Value, "HEX", ""},
         cli::Option{"--ignore", "", "don't ask before the file is overwritten and deleted", cli::Arg::Flag, "", ""},
         cli::Option{"--no-backup", "", "don't keep a .bak copy of the input", cli::Arg::Flag, "", ""}},
        [](const cli::Args& args) {
            const stdfs::path input = fs::from_text(args.required("input"));
            if (!stdfs::is_regular_file(input)) throw NotFoundError(input, 0, "the specified input file doesn't exist");
            std::optional<Bytes> key;
            if (const std::optional<std::string> hex = args.value("key"); hex && !hex->empty()) {
                if (hex->size() % 2 != 0) throw UsageError("the key is an even number of hex digits");
                key = cb::crypto::from_hex(*hex);
            }
            if (!args.flag("ignore")) {
                cb::out("This will OVERWRITE and DELETE the original XCP file, press ENTER if you want to continue...\n");
                cb::flush_out();
                std::string answer;
                if (!std::getline(std::cin, answer)) throw UsageError("not confirmed: nothing was read (use --ignore to go on without asking)");
            }
            Options options;
            options.backup = !args.flag("no-backup");
            unpack(input, key, options);
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::xcp_dumper
