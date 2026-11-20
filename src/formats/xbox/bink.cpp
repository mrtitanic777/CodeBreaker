#include "formats/xbox/bink.hpp"

#include <algorithm>
#include <cmath>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/text.hpp"

namespace cb::formats::xbox::bink {

namespace {

constexpr std::string_view kBinkReader = "bink";
constexpr std::size_t kBinkHeaderBytes = 44;   // the magic and ten 32-bit fields
constexpr std::string_view kBink1Letters = "bdfghik";

// 1 for BIK + b d f g h i k, 2 for KB2 + a letter, 0 for neither.
int bink_generation(ByteView head) {
    if (head.size() < 4) return 0;
    const std::string_view magic = as_text(head.first(4));
    if (magic.starts_with("BIK") && kBink1Letters.find(magic[3]) != std::string_view::npos) return 1;
    if (magic.starts_with("KB2") && magic[3] >= 'a' && magic[3] <= 'z') return 2;
    return 0;
}

}  // namespace

bool is_bink(ByteView head) { return bink_generation(head) != 0; }

Header read_header(ByteView head) {
    const int generation = bink_generation(head);
    if (!generation) throw FormatError(kBinkReader, 0, "not a Bink file (bad magic)");
    io::ByteReader r(head.first(std::min(head.size(), kBinkHeaderBytes)), kBinkReader);
    Header h;
    h.generation = generation;
    h.codec = std::string(as_text(r.bytes(4)));
    h.file_size = std::uint64_t{r.u32()} + 8;
    h.frames = r.u32();
    h.largest_frame = r.u32();
    r.skip(4);   // the frame count again
    h.width = r.u32();
    h.height = r.u32();
    const std::uint32_t numerator = r.u32(), denominator = r.u32();
    h.video_flags = r.u32();
    h.audio_tracks = r.u32();
    h.fps = denominator ? std::round(static_cast<double>(numerator) / denominator * 1000.0) / 1000.0 : 0.0;
    return h;
}

Header read_header_file(const std::filesystem::path& file) {
    const fs::MappedFile f(file);
    return read_header(f.view().first(std::min<std::size_t>(f.view().size(), 64)));
}

std::uint64_t decode(const std::filesystem::path& bik, const std::filesystem::path& out, const std::optional<std::filesystem::path>& ffmpeg,
                     bool audio_only, bool video_only) {
    if (!ffmpeg) throw ToolError("ffmpeg", 0, "", "not found: install it and/or name it in settings.json (ffmpeg)");
    std::vector<std::string> argv = {fs::to_text(*ffmpeg), "-y", "-hide_banner", "-loglevel", "error", "-i", fs::to_text(bik)};
    if (audio_only) argv.emplace_back("-vn");
    if (video_only) argv.emplace_back("-an");
    argv.push_back(fs::to_text(out));
    const process::ProcessResult r = process::run({.argv = argv, .err = process::Capture::Merge});
    if (!r.ok()) throw ToolError("ffmpeg", r.exit_code, as_text(r.out), "Bink decode failed");
    return std::filesystem::exists(out) ? fs::file_size(out) : 0;
}

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "bink";
    c.summary = "Xbox 360 Bink video (.bik): header info, and ffmpeg-driven decode";
    c.area = "Xbox 360";
    c.subcommands.push_back(cli::Command{"info", "the native header summary", {cli::Positional{"file", "the .bik"}}, [](const cli::Args& a) {
        const std::string path = a.required("file");
        const Header h = read_header_file(fs::from_text(path));
        out("Bink{} ({}): {}\n", h.generation, h.codec, path);
        out("  {} x {}  @ {:.3f} fps   {} frames   {} audio track(s)\n", h.width, h.height, h.fps, h.frames, h.audio_tracks);
        out("  file size {} bytes  (largest frame {})  flags 0x{:08X}\n", h.file_size, h.largest_frame, h.video_flags);
        return kExitOk;
    }});
    c.subcommands.push_back(cli::Command{"decode", "decode through ffmpeg (the output's extension picks the codec)",
                                         {cli::Positional{"file", "the .bik"}, cli::Positional{"out", "the .mp4, .wav, ... to write"},
                                          cli::Option{"--audio", "", "the audio track only"}, cli::Option{"--video", "", "the video only"}},
                                         [environment](const cli::Args& a) {
        const std::string target = a.required("out");
        const std::uint64_t n = decode(fs::from_text(a.required("file")), fs::from_text(target), environment.ffmpeg, a.flag("audio"), a.flag("video"));
        out("decoded -> {} ({} bytes)\n", target, n);
        return kExitOk;
    }});
    return c;
}

}  // namespace cb::formats::xbox::bink
