// cb/recompile/disassemble.hpp -- Xbox 360 PowerPC as (mnemonic, operands) text, for the recompiler.
//
//     Disassembler dis(vmx128_decode);                         one per thread
//     dis.listing(code, base_va)                               every word: address, word, mnemonic, operands
//     dis.classify(code, base_va, entries)                     the bytes flow reaches from `entries`: code
//     dis.data_spans(code, base_va, entries)                   the runs flow never reaches
//     codebreaker disassemble <image or .xex> [--limit N]      the command
//
// A word is read as VMX128 first (the `VectorDecoder`, decompile/vmx128), the Xenon's vector set: Capstone reads most
// VMX128 words as other, later instructions (ISA 2.07/3.0, which the Xenon does not have). Everything else is Capstone's.
// An engine is not safe to share between threads (and the whole-image recompile runs on a thread pool), so a
// Disassembler belongs to one thread and owns its engine. The vector builders read VMX128 operands in that decoder's
// order. Every instruction is one word, four-byte aligned, so there is no boundary to recover: code is what flow
// reaches, the rest is data.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/platforms/decoder.hpp"

namespace cb::recompile::disassemble {

inline constexpr std::uint32_t kWord = 4;

// A word in VMX128's primary opcodes (4, 5, 6): a vector instruction, not data.
bool is_vmx128(std::uint32_t word);

// The Xenon's vector instructions: (mnemonic, operands) for a VMX128 word, nullopt for any other. Without one, vector
// words are read by Capstone, wrongly (the decoder of decompile/vmx128 is what the recompiler passes).
using VectorDecoder = std::function<std::optional<std::pair<std::string, std::string>>(std::uint32_t word)>;

// One word of the listing.
struct Row {
    std::uint32_t address = 0;
    std::uint32_t word = 0;      // as the big-endian bytes read
    std::string mnemonic;        // ".long" for a word neither decoder reads
    std::string operands;        // then "0x%08X" of the word
    std::array<std::uint8_t, 4> raw() const {
        return {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8),
                static_cast<std::uint8_t>(word)};
    }
};

// Which bytes of `code` flow reaches: one flag per byte offset.
using Coverage = std::vector<bool>;

// A run of bytes [begin, end) in `code`.
struct Span {
    std::size_t begin = 0;
    std::size_t end = 0;
};

class Disassembler {
public:
    // Throws NotFoundError when Capstone cannot be loaded.
    explicit Disassembler(VectorDecoder vector = {});

    // The bytes of `code` that flow reaches from `entries` (addresses; one outside the code is ignored): an
    // instruction's four bytes are covered when it is reached; flow goes on past everything but b, ba, blr, bctr, bctrl,
    // rfi and rfid, and follows the immediate target of a jump or call into the code. A VMX128 word Capstone does not read
    // is reached too (vector code falls through, never branches).
    Coverage classify(ByteView code, std::uint32_t base_va, std::span<const std::uint32_t> entries);

    // The runs of at least `min_run` bytes flow never reaches.
    std::vector<Span> data_spans(ByteView code, std::uint32_t base_va, std::span<const std::uint32_t> entries, std::size_t min_run = kWord);

    // Every word from offset `start` (rounded down to a word), `count` at most (none: all), as rows. A word neither
    // decoder reads is ".long 0x....". Words are read big-endian at base_va + offset.
    std::vector<Row> listing(ByteView code, std::uint32_t base_va, std::size_t start = 0, std::optional<std::size_t> count = std::nullopt);

    // The same, one row at a time (an image's code is millions of words).
    void each_row(ByteView code, std::uint32_t base_va, std::size_t start, std::optional<std::size_t> count,
                  const std::function<void(const Row&)>& visit);

private:
    // One instruction as Capstone reads the word at `off`: a decode that fails is nullopt.
    struct Decoded {
        std::string mnemonic;
        std::string operands;
        bool jump = false, call = false;
        std::vector<std::int64_t> immediates;   // the operands of immediate type, in order
    };
    std::optional<Decoded> decode_one(ByteView code, std::size_t off, std::uint32_t base_va);

    VectorDecoder vector_;
    platforms::DecoderSet decoders_;
};

// ---- an image's code -------------------------------------------------------------------------------------------------

// What a PowerPC image says about itself (cb/formats/powerpc_image reads it; a .xex is read through its basefile).
struct ImageFacts {
    struct Chunk {                  // `size` bytes of the file at `file_offset` live at `va`
        std::uint32_t va = 0;
        std::uint32_t file_offset = 0;
        std::uint32_t size = 0;
    };
    struct Section {
        std::string name;
        std::uint32_t va = 0, size = 0, characteristics = 0;
    };
    Bytes data;                     // the file
    std::uint32_t base = 0;         // where it loads
    std::uint32_t entry = 0;        // the entry point, 0 for none
    std::uint32_t code_begin = 0, code_end = 0;
    std::vector<Chunk> chunks;
    std::vector<Section> sections;
};

struct CodeSection {
    Bytes code;
    std::uint32_t address = 0;
    std::uint32_t entry = 0;        // the image's entry, else the start of the code
};

// The image's code section.
CodeSection code_section(const ImageFacts& image);

// The image laid out as loaded: [base, the end of the last chunk), each chunk at its address, the gaps zero.
struct FlatImage {
    struct Section {
        std::string name;
        std::uint32_t va = 0, size = 0;
        bool executable = false;       // marked executable (0x20000000), or named .text
    };
    Bytes bytes;
    std::uint32_t base = 0;
    std::uint32_t entry = 0;           // the image's entry, else the start of the code
    std::vector<Section> sections;
};
// FormatError when the image has no chunk or a chunk lies outside the file.
FlatImage flatten(const ImageFacts& image);

// ---- the command ------------------------------------------------------------------------------------------------------

using ImageReader = std::function<ImageFacts(const std::filesystem::path&)>;

// `codebreaker disassemble <image or .xex> [--limit N]`: the first N words (60) of the code with an `d` mark on those flow
// does not reach, and how much of the code reads as data. Returns the exit code.
int main(const ImageReader& read_image, const VectorDecoder& vector, std::span<const std::string> args);

}  // namespace cb::recompile::disassemble
