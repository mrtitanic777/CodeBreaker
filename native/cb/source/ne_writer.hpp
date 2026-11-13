// cb/source/ne_writer.hpp -- a 16-bit Windows (or OS/2) program as assembly source that builds back into it, byte for byte.
//
//     ne_writer::Result r = ne_writer::reconstruct("lzexpand.dll", "out", {.nasm = nasm});
//     r.source, r.image, r.same, r.instructions, r.raw, r.rounds
//
// The way pe_writer writes a 32-bit image, for the segmented kind: one NASM source assembled flat (-f bin), each segment a section at its
// place in the file (start=) whose addresses are its own offsets (vstart=0), followed by its relocation table. Whatever the linker made is
// written as what it refers to, not as a number: the segment table's sectors and lengths as expressions of the segments' places, every
// relocation chain (LINK writes, in each place a relocation patches, where the next place of the same target is) as the label of the next
// place, every record's first place as its label, and an internal reference's offset as the label in its segment. So the file is the
// linker's output as written down, and stands without the linker that made it -- whichever version that was.
//
// Code is what is reached from the entry table, from the program's start, and from every internal reference into a code segment; what is
// not reached is data. An instruction is written as its mnemonic where NASM gives back the original's bytes (checked every round from the
// assembler's map, source/flat_rounds); one that holds a place a relocation patches is written as its bytes, with those places as
// expressions and its mnemonic beside it. The DOS program in front (the stub) comes from its own source when that builds back; the
// reader of that source is the DOS writer's, so it is handed in (`Options::find_stub`).
#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/ne.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/source/flat_rounds.hpp"
#include "cb/source/stub.hpp"
#include "cb/source/x86_flat.hpp"
#include "cb/translate/l2.hpp"

namespace cb::source::ne_writer {

// The stub's types are the whole-file writers' common ones (source/stub.hpp).
using ::cb::source::FindStub;
using ::cb::source::KeepStub;
using ::cb::source::SearchStub;
using ::cb::source::Stub;

// A place in the program: segment (1-based) and offset.
struct Place {
    std::uint32_t segment = 0;
    std::uint32_t offset = 0;
    friend auto operator<=>(const Place&, const Place&) = default;
};

struct Options {
    std::filesystem::path nasm;                         // the assembler (settings: nasm); NotFoundError when it is not there
    Stub stub = SearchStub{};
    FindStub find_stub;                                 // used for SearchStub; none: the stub's bytes are kept
    std::function<void(std::string_view)> say;          // progress, one line a round
};

// A segment of the file with its relocations.
struct Segment {
    std::uint32_t index = 0;
    std::size_t file_offset = 0, length = 0;
    std::uint16_t flags = 0;
    std::vector<formats::ne::Relocation> relocations;
    std::size_t table = 0;       // bytes of its relocation table (0 when it has none)
    bool code = false;
};

// What a relocation patches at one place: its record, how many bytes, and whether it is a chain's link.
struct Site {
    std::size_t record = 0;
    std::uint32_t width = 0;
    bool chained = false;
    std::uint32_t next = 0;      // the next place of the chain (0xFFFF: the last)
};

// The program, read and decoded, as the file written whole. Its rounds are flat_rounds::flat's.
class Image final : public flat_rounds::FlatImage<Place> {
public:
    // `file_name` is how the source names the program. FormatError when `module` has an unreadable table.
    Image(formats::ne::Module module, std::string file_name);
    static Image open(const std::filesystem::path& program);

    // The DOS program in front of the first segment: assembled from its source at `stub` (included by NASM), instead of the bytes kept.
    void set_stub(std::optional<std::filesystem::path> stub) { stub_ = std::move(stub); }

    const formats::ne::Module& module() const { return module_; }
    std::size_t instruction_count() const { return insns_.size(); }
    // The instructions found as code, by place.
    const std::map<Place, x86_flat::Insn>& instructions() const { return insns_; }
    // Where each function begins: the entry points and every place a call lands.
    const std::set<Place>& function_starts() const { return starts_; }
    // The instructions the last writing had to put as bytes for a place inside them.
    const std::set<Place>& written_as_bytes() const { return written_raw_; }
    // The last writing, as one L2 tree (each function and segment a fragment); build the document with `assembly().document(arena)`.
    const translate::l2::Assembly& assembly() const { return assembly_; }

    // `exact`: the sections stand at their own places in the file (the last writing); else each follows the one before.
    flat_rounds::Source<Place> write(const std::set<Place>& raw, bool final) override;
    std::set<Place> differing(ByteView built, const flat_rounds::LabelMap& labels, const std::set<Place>& raw) override;

    // The file's text with `raw` written as bytes. `mark_all`: a label on every instruction (the rounds need them to find where each
    // went); `exact`: see write().
    flat_rounds::Source<Place> source(const std::set<Place>& raw, bool mark_all, bool exact);

private:
    bool has_segment(std::uint32_t index) const { return segment_at_.contains(index); }
    const Segment& segment(std::uint32_t index) const { return segments_[segment_at_.at(index)]; }
    ByteView data() const { return module_.data(); }
    ByteView bytes(std::size_t lo, std::size_t hi) const;       // [lo, hi) of the file, cut to its end

    void decode_code();
    std::set<Place> find_function_starts(const std::vector<Place>& entries) const;
    std::set<Place> wanted(bool mark_all) const;
    std::string label(Place at) const;                           // the name of a place
    std::string ref(std::uint32_t segment, std::uint32_t offset) const;   // how the source spells a place
    std::vector<std::string> text(std::uint32_t segment, const x86_flat::Insn& insn, bool raw) const;
    std::optional<std::string> nasm_text(std::uint32_t segment, const x86_flat::Insn& insn, const std::optional<std::uint32_t>& target) const;
    void data_outside_segments(translate::l2::Assembly& out, std::size_t from, std::size_t to) const;
    void write_segment(translate::l2::Assembly& out, const Segment& s, std::map<std::size_t, Place>& lines_of) const;
    void write_data(translate::l2::Assembly& out, std::uint32_t segment, std::size_t base, std::size_t from, std::size_t to) const;

    formats::ne::Module module_;
    std::string file_name_;
    std::uint32_t shift_ = 0;
    std::optional<std::filesystem::path> stub_;
    platforms::DecoderSet decoders_;
    std::vector<Segment> segments_;
    std::map<std::uint32_t, std::size_t> segment_at_;           // segment number -> its place in segments_
    std::map<Place, Site> sites_;                                // every place a relocation patches
    std::map<Place, x86_flat::Insn> insns_;
    std::map<Place, std::uint32_t> covered_;                     // every byte of an instruction -> where its instruction starts
    std::set<Place> starts_;
    std::map<Place, std::string> placed_;                        // the labelled places of the last writing
    std::set<Place> written_raw_;
    translate::l2::Assembly assembly_;
};

struct Result {
    std::filesystem::path source;
    std::optional<std::filesystem::path> image;   // the file built
    bool same = false;                            // it is the original
    std::size_t rounds = 0;
    std::size_t raw = 0;                          // instructions written as bytes
    std::string log;
    std::size_t instructions = 0;

    flat_rounds::FlatReport report() const;
};

// Write `program` as <outdir>/<name>.asm (name: its file name in lower case), assembled with NASM, rounds until nothing differs.
// NotFoundError when there is no assembler; FormatError for a program that is not an NE file; ToolError when NASM rejects what is not
// the writer's to blame.
Result reconstruct(const std::filesystem::path& program, const std::filesystem::path& outdir, const Options& options);

// What the command `ne-source <program> [--out DIR] [--keep-stub]` needs of its surroundings.
struct Environment {
    std::filesystem::path nasm;
    std::filesystem::path state;     // the default out folder is <state>/ne_source/<program name>
    FindStub find_stub;
};
// The command line without the command's name; `say` takes every line printed. -> the exit code (2: no program named).
int command(std::span<const std::string> args, const Environment& env, const std::function<void(std::string_view)>& say);

// The command's usage text.
std::string_view usage();

}  // namespace cb::source::ne_writer
