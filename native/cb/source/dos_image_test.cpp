// Tests of cb/source/dos_image: small real-mode programs written byte by byte (a .COM file with a call, an MZ program with a
// relocated segment word, a program too long for one segment) and what source each becomes.
//
// Design notes (class Writer):
//   * the program's name is given (the C++ Program has none); the source's head says it;
//   * which jumps and calls capstone resolved is read from the instruction's bytes (the relative forms E8, E9, EB, 70-7F, E0-E3,
//     0F 80-8F, with a 66 prefix making the displacement 32 bits wide), not from the decoder's operand records: the answer is the
//     same where the decoder resolves them in 16-bit mode, and no detail is asked of the decoder;
//   * a relocated word in the last byte of the image is not marked.

#include "cb/source/dos_image.hpp"

#include <algorithm>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::source::dos_image;
namespace dos = cb::formats::dos;

namespace {

constexpr std::uint32_t kImageBase = dos::kLoadSegment * 16;

const symbols::dos_interrupts::ServiceTable& dos_image_services() {
    static const auto table = symbols::dos_interrupts::ServiceTable::load(
        std::filesystem::path(CB_SOURCE_DIR).parent_path() / "config" / "symbols" / "dos_interrupts.json");
    return table;
}

const platforms::Decoder& dos_image_decoder() {
    static const platforms::Decoder d = platforms::Decoder::open(platforms::Arch::X86, 16, false);
    return d;
}

bool has_line(const std::string& text, std::string_view line) {
    std::size_t at = 0;
    while (at < text.size()) {
        const std::size_t end = text.find('\n', at);
        const std::string_view l = std::string_view(text).substr(at, (end == std::string::npos ? text.size() : end) - at);
        if (l == line) return true;
        if (end == std::string::npos) break;
        at = end + 1;
    }
    return false;
}

// A .COM file: 256 bytes of program prefix, then the code. call 110h / mov ah,4Ch / int 21h, and a function at 110h.
dos::Program com_program() {
    Bytes image(0x120, 0);
    const Bytes code = {0xE8, 0x0D, 0x00, 0xB4, 0x4C, 0xCD, 0x21};
    std::copy(code.begin(), code.end(), image.begin() + 0x100);
    const Bytes function = {0x55, 0x8B, 0xEC, 0xB4, 0x09, 0xCD, 0x21, 0x5D, 0xC3};
    std::copy(function.begin(), function.end(), image.begin() + 0x110);
    dos::Program p;
    p.kind = dos::Kind::Com;
    p.base = kImageBase;
    p.spans.push_back({kImageBase, static_cast<std::uint32_t>(image.size()), "image"});
    p.entries.push_back({kImageBase + 0x100, "start", dos::Entry::Call::Near});
    p.image = std::move(image);
    return p;
}

}  // namespace

CB_TEST("source.dos_image.a_com_file_becomes_labels_and_mnemonics", [] {
    const dos::Program p = com_program();
    const DosImage w(p, "HELLO.COM", dos_image_services(), dos_image_decoder());
    CB_CHECK(w.flat());
    CB_CHECK_EQ(w.origin(), std::uint32_t{0x100});
    CB_CHECK((w.segments() == std::vector<std::uint32_t>{0}));
    CB_CHECK(w.labels().contains(0x100) && w.labels().contains(0x110));
    CB_CHECK_EQ(w.label(0x100), "start");
    CB_CHECK_EQ(w.label(0x110), "sub_00110");
    CB_CHECK_EQ(w.label(0x105), "L_00105");
    CB_CHECK_EQ(w.instructions().size(), std::size_t{9});
    CB_CHECK(w.instructions().at(0x100).target == std::optional<std::uint32_t>(0x110));   // the call goes to its label
    CB_CHECK_EQ(*w.mnemonic(0x100), "call   near ptr sub_00110");
    CB_CHECK_EQ(*w.mnemonic(0x103), "mov    ah, 04Ch");
    CB_CHECK_EQ(*w.mnemonic(0x110), "push   bp");

    const Source s = w.source();
    CB_CHECK_EQ(s.counts.instructions, std::size_t{9});
    CB_CHECK_EQ(s.counts.raw, std::size_t{0});
    CB_CHECK_EQ(s.counts.data, std::size_t{16});   // 9 bytes between the programs and 7 behind the function
    CB_CHECK(s.text.starts_with("; HELLO.COM -- reconstructed from the program image by CodeBreaker.\n;\n"));
    CB_CHECK(has_line(s.text, "S0000 segment para public use16 'C0000'"));
    CB_CHECK(has_line(s.text, "        assume cs:S0000, ds:nothing, es:nothing, ss:nothing"));
    CB_CHECK(has_line(s.text, "        org  256"));
    CB_CHECK(has_line(s.text, "start:"));
    CB_CHECK(has_line(s.text, "        call   near ptr sub_00110"));
    CB_CHECK(has_line(s.text, "sub_00110:"));
    CB_CHECK(has_line(s.text, "S0000 ends"));
    CB_CHECK(s.text.ends_with("\nend start\n"));   // where it starts: the linker leaves out what is before
    // an interrupt carries the name of its service
    bool named = false;
    for (std::size_t at = 0; (at = s.text.find("int    021h", at)) != std::string::npos; ++at) named = named || s.text.find("; ", at) != std::string::npos;
    CB_CHECK(named);
});

CB_TEST("source.dos_image.an_instruction_with_prefixes_after_a_66_is_read_to_its_end", [] {
    // 66 66 90 C3: the constructor once stepped over the first prefix only, and looped for ever on the second (found on IO.SYS)
    Bytes image(0x108, 0);
    const Bytes code = {0x66, 0x66, 0x90, 0xC3};
    std::copy(code.begin(), code.end(), image.begin() + 0x100);
    dos::Program p;
    p.kind = dos::Kind::Com;
    p.base = kImageBase;
    p.spans.push_back({kImageBase, static_cast<std::uint32_t>(image.size()), "image"});
    p.entries.push_back({kImageBase + 0x100, "start", dos::Entry::Call::Near});
    p.image = std::move(image);
    const DosImage w(p, "PREFIX.COM", dos_image_services(), dos_image_decoder());
    CB_CHECK(w.instructions().contains(0x100));
    CB_CHECK(!w.instructions().at(0x100).target.has_value());
    CB_CHECK(w.instructions().contains(0x103));
});

CB_TEST("source.dos_image.only_the_instructions_said_to_be_good_are_mnemonics", [] {
    const dos::Program p = com_program();
    const DosImage w(p, "HELLO.COM", dos_image_services(), dos_image_decoder());
    std::set<std::uint32_t> good = {0x100, 0x103};
    const Source s = w.source(&good);
    CB_CHECK_EQ(s.counts.instructions, std::size_t{9});
    CB_CHECK_EQ(s.counts.raw, std::size_t{7});   // the others as bytes (the mnemonic in the comment)
    CB_CHECK(has_line(s.text, "        call   near ptr sub_00110"));
    bool bytes_with_comment = false;
    for (std::size_t at = 0; (at = s.text.find("        db   055h", at)) != std::string::npos; ++at) bytes_with_comment = true;
    CB_CHECK(bytes_with_comment);
    CB_CHECK(s.text.find("push   bp") != std::string::npos);   // in the comment of its bytes
    // an instruction that has nothing to be written as is bytes whatever `good` says
    CB_CHECK_EQ(w.source().counts.raw, std::size_t{0});
});

CB_TEST("source.dos_image.what_the_probe_and_the_culprit_search_are_given", [] {
    const dos::Program p = com_program();
    const DosImage w(p, "HELLO.COM", dos_image_services(), dos_image_decoder());
    const auto candidates = w.candidates();
    CB_CHECK_EQ(candidates.size(), std::size_t{8});   // the call to a label is not probed
    CB_CHECK(!candidates.contains(0x100));
    CB_CHECK_EQ(candidates.at(0x103).text, "mov    ah, 04Ch");
    CB_CHECK((candidates.at(0x103).original == Bytes{0xB4, 0x4C}));
    CB_CHECK((candidates.at(0x103).mask == Bytes{0, 0}));

    const std::set<std::uint32_t> good = {0x100, 0x110};
    const auto pieces = w.pieces(good);
    std::size_t covered = 0;
    for (const auto& piece : pieces) covered += piece.length;
    CB_CHECK_EQ(covered, std::size_t{0x120 - 0x100});   // the whole image from its origin, in order
    CB_CHECK_EQ(pieces.front().offset, std::size_t{0});
    CB_CHECK(pieces.front().key == std::optional<std::uint32_t>(0x100));
    CB_CHECK(pieces.front().inner_jump);
    CB_CHECK(!pieces[1].inner_jump);
    CB_CHECK(!pieces[1].key.has_value());   // not in `good`
    CB_CHECK((w.link_lines("hello.obj") == std::vector<std::string>{"format dos com", "option quiet, nodefaultlibs", "file hello.obj"}));
});

namespace {

// An MZ program with a header of its own: `mov ax, <the segment 10h>` (a relocated word), `ret`, then zeros to 0x110.
dos::Program mz_program_with_a_segment_word() {
    Bytes image(0x110, 0);
    image[0] = 0xB8;
    image[1] = 0x10;                 // 1000h + 10h: loaded at 1000h, it names paragraph 10h of the module
    image[2] = 0x10;
    image[3] = 0xC3;
    dos::Program p;
    p.kind = dos::Kind::Mz;
    p.base = kImageBase;
    p.spans.push_back({kImageBase, static_cast<std::uint32_t>(image.size()), "program"});
    p.entries.push_back({kImageBase, "start", dos::Entry::Call::Near});
    p.relocations = {kImageBase + 1};
    p.cs = dos::kLoadSegment;
    p.ip = 0;
    p.ss = dos::kLoadSegment + 0x10;
    p.sp = 0x40;
    p.image = std::move(image);
    return p;
}

}  // namespace

CB_TEST("source.dos_image.a_word_that_holds_a_segment_is_written_as_the_segment", [] {
    const dos::Program p = mz_program_with_a_segment_word();
    const DosImage w(p, "SEG.EXE", dos_image_services(), dos_image_decoder());
    CB_CHECK(!w.flat());
    CB_CHECK((w.segments() == std::vector<std::uint32_t>{0, 0x10}));   // the segment the word names, and the stack's (the same)
    CB_CHECK(w.stack_segment() == std::optional<std::uint32_t>(0x10));
    CB_CHECK(w.instructions().at(0).where == std::optional<std::uint32_t>(1));
    CB_CHECK_EQ(*w.mnemonic(0), "mov    ax, seg S0010");
    CB_CHECK_EQ(w.seg_name(0x10), "seg S0010");
    CB_CHECK_EQ(w.seg_name(0x14), "seg S0010 + 4");
    CB_CHECK_EQ(w.seg_name(3), "seg S0000 + 3");
    CB_CHECK_EQ(w.segment_of(0xFF), std::uint32_t{0});
    CB_CHECK_EQ(w.segment_of(0x100), std::uint32_t{0x10});
    const Source s = w.source();
    CB_CHECK(has_line(s.text, "S0000 segment para public use16 'C0000'"));
    CB_CHECK(has_line(s.text, "S0010 segment para stack use16 'STACK'"));   // the stack, in the program's last segment
    CB_CHECK(has_line(s.text, "        mov    ax, seg S0010"));
    CB_CHECK(has_line(s.text, "S0010 ends"));
    CB_CHECK(s.text.ends_with("end start\n"));   // the entry point has a label
    // the instruction written as bytes keeps the word as the segment it names
    std::set<std::uint32_t> none;
    const Source raw = w.source(&none);
    CB_CHECK_EQ(raw.counts.raw, std::size_t{2});
    CB_CHECK(raw.text.find("        db   0B8h") != std::string::npos);
    CB_CHECK(raw.text.find("        dw   seg S0010") != std::string::npos);
    CB_CHECK((w.link_lines("seg.obj") ==
              std::vector<std::string>{"format dos", "option quiet, nodefaultlibs, packcode=0, packdata=0, stack=64", "file seg.obj"}));
    // and its pieces: the instruction with the relocated word masked, the word it holds
    const auto pieces = w.pieces({0});
    CB_CHECK(pieces.front().mask.has_value());
    CB_CHECK((*pieces.front().mask == Bytes{0, 1, 1}));
});

CB_TEST("source.dos_image.a_stretch_longer_than_a_segment_is_divided_where_nothing_crosses", [] {
    Bytes image(0x12000, 0x90);   // nops
    image.back() = 0xC3;
    dos::Program p;
    p.kind = dos::Kind::Mz;
    p.base = kImageBase;
    p.spans.push_back({kImageBase, static_cast<std::uint32_t>(image.size()), "program"});
    p.entries.push_back({kImageBase, "start", dos::Entry::Call::Near});
    p.cs = dos::kLoadSegment;
    p.image = std::move(image);
    const DosImage w(p, "BIG.EXE", dos_image_services(), dos_image_decoder());
    CB_CHECK((w.segments() == std::vector<std::uint32_t>{0, 0xFFF}));   // cut at the paragraph FFF0h
    const Source s = w.source();
    CB_CHECK(has_line(s.text, "S0FFF segment para public use16 'C0FFF'"));
});

CB_TEST("source.dos_image.a_jump_across_the_division_is_written_as_bytes", [] {
    Bytes image(0x12000, 0x90);
    // a short jump forward cannot cross 0xFFF0: make a chain of long ones that reach across every candidate place
    // jmp near at 0x0FF00 to 0x10100 (E9 rel16 = 0x01FD): crosses the cut at 0xFFF0
    image[0xFF00] = 0xE9;
    image[0xFF01] = 0xFD;
    image[0xFF02] = 0x01;
    // every paragraph from 0xF000 to 0xFFF0 is jumped across by another such jump, so each cut crosses one
    for (std::uint32_t at = 0xF000; at < 0xFFF0; at += 0x300) {
        image[at] = 0xE9;
        image[at + 1] = 0xFD;
        image[at + 2] = 0x1B;   // rel16 = 0x1BFD: from at+3 reaches beyond 0xFFF0 in every case
    }
    dos::Program p;
    p.kind = dos::Kind::Mz;
    p.base = kImageBase;
    p.spans.push_back({kImageBase, static_cast<std::uint32_t>(image.size()), "program"});
    p.entries.push_back({kImageBase, "start", dos::Entry::Call::Near});
    p.cs = dos::kLoadSegment;
    p.image = std::move(image);
    const DosImage w(p, "BIGJ.EXE", dos_image_services(), dos_image_decoder());
    // whatever place was chosen, no jump that remains a jump crosses a segment boundary
    for (const auto& [at, i] : w.instructions()) {
        if (!i.target) continue;
        CB_CHECK_EQ(w.segment_of(at), w.segment_of(*i.target));
    }
    CB_CHECK(w.segments().size() >= 2);
});
