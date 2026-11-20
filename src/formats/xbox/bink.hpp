// formats/xbox/bink.hpp -- Bink video (.bik), the most common Xbox 360 cutscene and video format.
//
//     bink::Header h = bink::read_header(first_64_bytes);     generation, codec tag, frames, size, frame rate, audio tracks
//     bink::decode(bik, out_wav_or_mp4, ffmpeg);              extract and decode through ffmpeg
//
// The container header is read here, with no tool: little-endian, the magic ("BIK" + b, d, f, g, h, i or k for Bink 1; "KB2" + a letter for
// Bink 2), then the file size less 8, frame count, largest frame, a second frame count, width, height, frame-rate numerator and denominator,
// video flags, audio track count. The Bink 1 / 2 video and DCT audio codecs are RAD's and ffmpeg maintains the decoders, so decoding is driven,
// not embedded (the policy chdman and the XMA decoder follow): ffmpeg is named by the caller.
//
// Errors: FormatError (TruncatedError) for bytes that are not a Bink header; ToolError for a missing ffmpeg or a failed decode.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

#include "core/bytes.hpp"
#include "core/cli.hpp"

namespace cb::formats::xbox::bink {

// Does the file begin with a Bink magic?
bool is_bink(ByteView head);

struct Header {
    int generation = 1;               // 1 or 2
    std::string codec;                // the four-character magic ("BIKi", "KB2j")
    std::uint64_t file_size = 0;      // the whole file (the field plus its own 8 bytes)
    std::uint32_t frames = 0, largest_frame = 0, width = 0, height = 0;
    double fps = 0;                   // numerator over denominator, to three places; 0 when the denominator is 0
    std::uint32_t video_flags = 0, audio_tracks = 0;
};

// `head` is the start of the file (44 bytes are read).
Header read_header(ByteView head);
Header read_header_file(const std::filesystem::path& file);

// A .bik decoded through ffmpeg: the output container and codec are chosen by the extension of `out` (.wav, .mp4, ...). -> the size written.
std::uint64_t decode(const std::filesystem::path& bik, const std::filesystem::path& out, const std::optional<std::filesystem::path>& ffmpeg,
                     bool audio_only = false, bool video_only = false);

struct Environment {
    std::optional<std::filesystem::path> ffmpeg;   // the ffmpeg to drive (settings: ffmpeg, else on PATH)
};

// `bink info|decode ...`: the module's command line.
cli::Command command(const Environment& environment);

}  // namespace cb::formats::xbox::bink
