#include "cb/formats/xbox/crypto/lzx_dll.hpp"

#include <algorithm>
#include <format>

#include "cb/core/dynlib.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"

namespace cb::formats::xbox::crypto::lzx_dll {

namespace {

namespace stdfs = std::filesystem;

// The library's own types (windows.h's LONG and ULONG are 32 bits; the context handle is 64).
using Handle = std::int64_t;
using OutputCallback = std::int32_t (*)(void* user, std::uint8_t* compressed, std::int32_t compressed_size, std::int32_t uncompressed_size);

#pragma pack(push, 2)
struct CompressConfig {
    std::int32_t window_size;
    std::int32_t second_partition_size;
};
struct DecompressConfig {
    std::int32_t window_size;
    std::int32_t cpu_type;
};
#pragma pack(pop)

constexpr std::int32_t kSecondPartitionSize = 32 * 1024;
constexpr std::int32_t kCpuTypeIntel = 1;

using CreateCompression = std::int32_t (*)(std::uint32_t* block_max, void* config, std::uint32_t* dst_buffer_min, Handle* handle, OutputCallback callback, void* user);
using Compress = std::int32_t (*)(Handle handle, void* src, std::int32_t src_size, void* dst, std::int32_t dst_size, std::uint32_t* result);
using HandleCall = std::int32_t (*)(Handle handle);
using CreateDecompression = std::int32_t (*)(std::uint32_t* block_max, void* config, std::uint32_t* src_buffer_min, Handle* handle);
using Decompress = std::int32_t (*)(Handle handle, void* src, std::int32_t src_size, void* dst, std::uint32_t* result);

// Each chunk the compressor hands over: its two sizes, big-endian, then its bytes.
std::int32_t collect_chunk(void* user, std::uint8_t* compressed, std::int32_t compressed_size, std::int32_t uncompressed_size) {
    Bytes& stream = *static_cast<Bytes*>(user);
    stream.push_back(static_cast<std::uint8_t>(compressed_size >> 8));
    stream.push_back(static_cast<std::uint8_t>(compressed_size));
    stream.push_back(static_cast<std::uint8_t>(uncompressed_size >> 8));
    stream.push_back(static_cast<std::uint8_t>(uncompressed_size));
    stream.insert(stream.end(), compressed, compressed + compressed_size);
    return 0;
}

void check_call(std::int32_t code, std::string_view call) {
    if (code != 0) throw ToolError("LZX library", static_cast<std::uint32_t>(code), "", std::format("{} failed with code 0x{:X}", call, static_cast<std::uint32_t>(code)));
}

class DllCodec : public build_lib::LzxCodec {
public:
    DllCodec(DynLib compression, DynLib decompression) : compression_(std::move(compression)), decompression_(std::move(decompression)) {}

    Bytes compress_continuous(ByteView data) override {
        const auto create = compression_.function<CreateCompression>("LCICreateCompression");
        const auto compress = compression_.function<Compress>("LCICompress");
        const auto flush = compression_.function<HandleCall>("LCIFlushCompressorOutput");
        const auto destroy = compression_.function<HandleCall>("LCIDestroyCompression");

        Bytes stream;
        std::uint32_t block_max = static_cast<std::uint32_t>(kChunkSize), dst_min = 0;
        CompressConfig config{static_cast<std::int32_t>(kWindowSize), kSecondPartitionSize};
        Handle handle = 0;
        check_call(create(&block_max, &config, &dst_min, &handle, &collect_chunk, &stream), "LCICreateCompression");
        struct Destroy {
            HandleCall destroy;
            Handle handle;
            ~Destroy() { destroy(handle); }
        } guard{destroy, handle};

        Bytes chunk;
        for (std::size_t at = 0; at < data.size(); at += kChunkSize) {
            const std::size_t n = std::min(kChunkSize, data.size() - at);
            chunk.assign(data.begin() + static_cast<std::ptrdiff_t>(at), data.begin() + static_cast<std::ptrdiff_t>(at + n));
            std::uint32_t produced = 0;
            check_call(compress(handle, chunk.data(), static_cast<std::int32_t>(n), nullptr, static_cast<std::int32_t>(n + kMaxGrowth), &produced), "LCICompress");
        }
        check_call(flush(handle), "LCIFlushCompressorOutput");
        return stream;
    }

    Bytes decompress_continuous(ByteView data) override {
        const auto create = decompression_.function<CreateDecompression>("LDICreateDecompression");
        const auto decompress = decompression_.function<Decompress>("LDIDecompress");
        const auto destroy = decompression_.function<HandleCall>("LDIDestroyDecompression");

        std::uint32_t block_max = static_cast<std::uint32_t>(kChunkSize), src_min = 0;
        DecompressConfig config{static_cast<std::int32_t>(kWindowSize), kCpuTypeIntel};
        Handle handle = 0;
        check_call(create(&block_max, &config, &src_min, &handle), "LDICreateDecompression");
        struct Destroy {
            HandleCall destroy;
            Handle handle;
            ~Destroy() { destroy(handle); }
        } guard{destroy, handle};

        io::ByteReader r(data, "lzx stream");
        Bytes out;
        while (!r.eof()) {
            const std::uint16_t compressed_size = r.u16_be();
            const std::uint16_t uncompressed_size = r.u16_be();
            Bytes block = r.bytes_copy(compressed_size);
            Bytes plain(uncompressed_size);
            std::uint32_t produced = uncompressed_size;
            check_call(decompress(handle, block.data(), compressed_size, plain.data(), &produced), "LDIDecompress");
            if (produced != uncompressed_size) throw FormatError("lzx stream", r.pos(), "decompressed size mismatch");
            append(out, plain);
        }
        return out;
    }

private:
    DynLib compression_;
    DynLib decompression_;
};

}  // namespace

std::shared_ptr<build_lib::LzxCodec> load(const stdfs::path& bin_dir) {
    const stdfs::path folder = bin_dir / "LZX" / "Windows";
    const stdfs::path compression = folder / "LZXCompression.dll";
    const stdfs::path decompression = folder / "LZXDecompression.dll";
    std::error_code ec;
    if (!stdfs::is_regular_file(compression, ec) || !stdfs::is_regular_file(decompression, ec)) return nullptr;
    return std::make_shared<DllCodec>(DynLib::load(compression), DynLib::load(decompression));
}

namespace {

class LazyCodec : public build_lib::LzxCodec {
public:
    explicit LazyCodec(stdfs::path bin_dir) : bin_dir_(std::move(bin_dir)) {}
    Bytes compress_continuous(ByteView data) override { return codec().compress_continuous(data); }
    Bytes decompress_continuous(ByteView data) override { return codec().decompress_continuous(data); }

private:
    build_lib::LzxCodec& codec() {
        if (!codec_) codec_ = load(bin_dir_);
        if (!codec_) throw UnsupportedError("lzx", 0, "the LZX libraries aren't in the bin folder (" + fs::to_text(bin_dir_ / "LZX" / "Windows") + "; setting xbox360_crypto_bin)");
        return *codec_;
    }
    stdfs::path bin_dir_;
    std::shared_ptr<build_lib::LzxCodec> codec_;
};

}  // namespace

std::shared_ptr<build_lib::LzxCodec> lazy(const stdfs::path& bin_dir) { return std::make_shared<LazyCodec>(bin_dir); }

cli::Command command(const stdfs::path& bin_dir) {
    return cli::Command{
        "lzx",
        "the LZX library's self-test: compress a file, decompress it and compare",
        {cli::Option{"--input", "-i", "the file to compress (default data/bootloader/uncompressed.bin)", cli::Arg::Value, "FILE", "data/bootloader/uncompressed.bin"}},
        [bin_dir](const cli::Args& args) {
            const std::shared_ptr<build_lib::LzxCodec> codec = load(bin_dir);
            if (!codec) throw NotFoundError(bin_dir / "LZX" / "Windows" / "LZXCompression.dll", 0, "the LZX libraries aren't in the bin folder (setting xbox360_crypto_bin)");
            const Bytes original = fs::read_file(fs::from_text(args.required("input")));
            const Bytes packed = codec->compress_continuous(original);
            if (!equal_bytes(codec->decompress_continuous(packed), original)) {
                cb::err("the files aren't the same\n");
                return 1;
            }
            cb::out("Done!\n");
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::lzx_dll
