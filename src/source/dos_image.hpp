// source/dos_image.hpp -- a real-mode program as assembly source that builds back into it.
//
// The real-mode base of the system was written in assembly, so assembly is what its source is. The text has one segment for every
// segment the program itself names (each word its relocation table marks holds one, and so do the entry point and the stack), and
// every such word is written as the segment it names (`dw seg S0605`), so the program can be assembled and linked at any address.
// Code is what analyze/dos_functions found by following the program; the rest is written as data, text as text. Interrupt calls
// carry the name of the service.
//
// What is NOT recovered, because a real-mode image does not state it: which data an offset inside a segment refers to.
// `mov ax, ds:[1234h]` stays a number, so data cannot be moved within its segment without changing those by hand.
//
// An instruction is written as its mnemonic only when the assembler turns that text back into the original's bytes (the probing of
// source/assembly.hpp checks each one); the loop that assembles, links and compares is dos_program.hpp.
//
//     DosImage image(program, "LZEXPAND.EXE", symbols::dos_interrupts::ServiceTable::standard(), decoder);
//     auto [text, counts] = image.source(&good);            // every instruction in `good` as its mnemonic, the rest as bytes
//
// Terms: an *offset* is a position in the load module (the image), and a *segment* here is the paragraph number it starts at.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "analyze/dos_functions.hpp"
#include "core/bytes.hpp"
#include "formats/dos.hpp"
#include "platforms/decoder.hpp"
#include "source/assembly.hpp"
#include "symbols/dos_interrupts.hpp"
#include "translate/l2.hpp"

namespace cb::source::dos_image {

// What stands in for a relocated segment word while an instruction is decoded (the assembler's own number never occurs there).
inline constexpr std::uint16_t kSegmentMark = 0x7AB3;

// How far into a segment's paragraph a stretch may run: a segment holds 64 KB less 16 bytes of the paragraph it is named by.
inline constexpr std::uint32_t kSegmentSpan = 0xFFF0;

// One instruction of the image as the decoder read it.
struct Instruction {
    Bytes bytes;
    std::string mnemonic;
    std::string operands;
    std::optional<std::uint32_t> target;   // the offset a near jump or call goes to inside its segment: written as a label
    std::optional<std::uint32_t> where;    // the offset of the relocated word inside it (written as the segment it names)
};

// How much of the image was written as what.
struct Counts {
    std::size_t instructions = 0;
    std::size_t raw = 0;     // instructions written as bytes
    std::size_t data = 0;    // bytes of data
};

struct Source {
    std::string text;
    Counts counts;
};

class DosImage {
public:
    // `name`: the program's file name (the head of the source says it). `decoder`: a 16-bit x86 decoder.
    DosImage(const formats::dos::Program& program, std::string name, const symbols::dos_interrupts::ServiceTable& services,
             const platforms::Decoder& decoder);

    const formats::dos::Program& program() const { return program_; }
    const analyze::dos_functions::Analysis& analysis() const { return analysis_; }
    ByteView image() const { return image_; }
    // A .COM file sits behind its program prefix: its bytes begin here.
    std::uint32_t origin() const { return origin_; }
    // Bytes as they load, or a file that has a header of its own?
    bool flat() const { return flat_; }
    const std::set<std::uint32_t>& relocated() const { return relocated_; }
    // The segments the source has, by the paragraph each is named by (the first is 0).
    const std::vector<std::uint32_t>& segments() const { return segments_; }
    const std::optional<std::uint32_t>& stack_segment() const { return stack_in_; }
    const std::map<std::uint32_t, Instruction>& instructions() const { return ins_; }
    const std::set<std::uint32_t>& labels() const { return labels_; }

    // The segment an offset is in.
    std::uint32_t segment_of(std::uint32_t at) const;
    // The program's name for a place, else L_<position>.
    std::string label(std::uint32_t at) const;
    // A relocated word's value (relative to where the program is loaded) as an expression: `seg S0605 + 4`.
    std::string seg_name(std::uint32_t value) const;
    // The instruction at `at` as the text the assembler turns back into its bytes, or nullopt where it is written as bytes (a jump
    // across a place the program was divided at, a relocated word the text does not show).
    std::optional<std::string> mnemonic(std::uint32_t at) const;

    // The whole source. `good`: the offsets of the instructions to write as mnemonics (null: all of them).
    Source source(const std::set<std::uint32_t>* good = nullptr) const;

    // The image as the pieces the source is made of, for assembly::culprits: each instruction written as its mnemonic, each word
    // that holds a segment, and the runs of bytes between.
    std::vector<assembly::Piece<std::uint32_t>> pieces(const std::set<std::uint32_t>& good) const;

    // What can be probed: {offset: the text, the instruction's bytes outside its relocated word, which bytes those are}. Jumps and
    // calls to a label are not probed (their label is not the assembler's to check).
    std::map<std::uint32_t, assembly::Candidate> candidates() const;

    // The linker's directives for `object`.
    std::vector<std::string> link_lines(std::string_view object) const;

private:
    void decode_instructions(const platforms::Decoder& decoder);
    void divide();
    std::string segment_at(std::uint32_t at) const;      // the word at `at` as a segment expression
    std::vector<std::string> raw_lines(std::uint32_t at) const;

    const formats::dos::Program& program_;
    std::string name_;
    Bytes image_;
    std::uint32_t origin_ = 0;
    std::set<std::uint32_t> relocated_;
    bool flat_ = true;
    std::optional<std::uint32_t> stack_in_;
    std::uint32_t top_ = 0;
    std::vector<std::uint32_t> segments_;
    analyze::dos_functions::Analysis analysis_;
    std::map<std::uint32_t, std::string> names_;
    std::optional<std::uint32_t> entry_;
    std::set<std::uint32_t> labels_;
    std::map<std::uint32_t, Instruction> ins_;
    std::map<std::uint32_t, std::string> calls_;         // an `int`'s offset -> the service it asks for
    std::set<std::uint32_t> crossing_;                    // jumps across a place the program was divided at: written as bytes
};

}  // namespace cb::source::dos_image
