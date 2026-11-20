#include "formats/xbox/crypto/xell_unpack.hpp"

#include <algorithm>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "zlib.h"

namespace cb::formats::xbox::crypto::xell_unpack {

namespace {

constexpr int kGzipWindowBits = 16 + 15;   // zlib's flag for a gzip stream

}  // namespace

Bytes gunzip(ByteView stream) {
    z_stream z{};
    if (inflateInit2(&z, kGzipWindowBits) != Z_OK) throw InternalError("zlib: cannot start inflating");
    struct End {
        z_stream& z;
        ~End() { inflateEnd(&z); }
    } end{z};
    z.next_in = const_cast<Bytef*>(stream.data());
    z.avail_in = static_cast<uInt>(stream.size());
    Bytes out;
    Bytes chunk(1 << 16);
    int status = Z_OK;
    while (status == Z_OK) {
        z.next_out = chunk.data();
        z.avail_out = static_cast<uInt>(chunk.size());
        status = inflate(&z, Z_NO_FLUSH);
        out.insert(out.end(), chunk.begin(), chunk.begin() + static_cast<std::ptrdiff_t>(chunk.size() - z.avail_out));
        if (status == Z_BUF_ERROR && z.avail_in == 0) status = Z_DATA_ERROR;   // out of input before the end of the stream
    }
    if (status != Z_STREAM_END) throw FormatError("xell", stream.size() - z.avail_in, "the second stage is not a whole gzip stream");
    return out;
}

Parts split(ByteView image) {
    if (image.size() < kStage1Size + kStage2Size) throw FormatError("xell", image.size(), "an XeLL image is 256 KB: too short for its stages");
    Parts parts;
    parts.stage1.assign(image.begin(), image.begin() + static_cast<std::ptrdiff_t>(kStage1Size));
    const ByteView second = image.subspan(kStage1Size, kStage2Size);
    parts.stage2_compressed.assign(second.begin(), second.end());
    parts.stage2 = gunzip(second);
    return parts;
}

std::vector<std::filesystem::path> unpack_files(const std::filesystem::path& folder, const std::vector<std::string>& names) {
    std::vector<std::filesystem::path> written;
    for (const std::string& name : names) {
        const Parts parts = split(fs::read_file(folder / (name + ".bin")));
        const std::pair<std::string, const Bytes*> outputs[] = {{"-stage-1.bin", &parts.stage1},
                                                                {"-stage-2-compressed.elf", &parts.stage2_compressed},
                                                                {"-stage-2-decompressed.elf", &parts.stage2}};
        for (const auto& [suffix, bytes] : outputs) {
            const std::filesystem::path file = folder / (name + suffix);
            fs::write_file(file, *bytes);
            written.push_back(file);
        }
    }
    return written;
}

cli::Command command() {
    return cli::Command{
        "xell_unpack",
        "split XeLL images into their stages and inflate the second",
        {cli::Positional{"names", "the images to unpack, without .bin (default xell-1f xell-2f xell-gggggg)", false, true},
         cli::Option{"--dir", "-d", "the folder of the images (default XeLL)", cli::Arg::Value, "DIR", "XeLL"}},
        [](const cli::Args& args) {
            std::vector<std::string> names = args.values("names");
            if (names.empty()) names = {"xell-1f", "xell-2f", "xell-gggggg"};
            for (const std::filesystem::path& file : unpack_files(fs::from_text(args.required("dir")), names)) cb::out("{}\n", fs::to_text(file));
            return 0;
        },
        {},
        "",
        ""};
}

}  // namespace cb::formats::xbox::crypto::xell_unpack
