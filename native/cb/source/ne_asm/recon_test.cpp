// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/ne_asm/recon: the MASM text of an instruction, the db and data lines, the body of a function, the listings the divergers are found
// from, and the loop that pins what the assembler will not encode. The assembler is given by the test (it takes a source and answers with what MASM
// would), over LZEXPAND.DLL's first code segment with the listing of scenario_a (a canned objdump). The expected text was worked out from the module's
// rules; the run on a real MASM, byte for byte, is in the report of the change.
#include "cb/source/ne_asm/recon.hpp"

#include <format>
#include <set>
#include <sstream>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace rc = cb::source::ne_asm::recon;
namespace sg = cb::source::ne_asm::segment;
namespace fx = cb::source::ne_asm::fixups;
namespace sd = cb::source::ne_asm::segdiff;

namespace {

using RcRows = std::vector<std::pair<std::uint32_t, std::string>>;

RcRows rc_rows(std::string_view name) {
    RcRows rows;
    std::istringstream in(cb::text::replace_all(cb::fs::read_text(cb::testing::data_path("source/ne_asm/testdata/" + std::string(name))), "\r\n", "\n"));
    for (std::string line; std::getline(in, line);) {
        if (line.empty()) continue;
        const std::size_t tab = line.find('\t');
        rows.emplace_back(*cb::text::parse_int<std::uint32_t>(line.substr(0, tab), 16), line.substr(tab + 1));
    }
    return rows;
}

fx::Objdump rc_objdump(RcRows rows) {
    return [rows = std::move(rows)](cb::ByteView, std::uint32_t) {
        std::string out = "\r\nsegment.bin:     file format binary\r\n\r\n\r\nDisassembly of section .data:\r\n\r\n00000000 <.data>:\r\n";
        for (const auto& [offset, text] : rows) out += std::format("{:4x}:\t00 00\t{}\r\n", offset, text);
        return out;
    };
}

sg::Seg rc_segment() {
    const cb::formats::ne::Module module = fx::open_module(cb::testing::data_path("formats/testdata/lzexpand.dll"));
    const auto ordinals = std::make_shared<const sg::Ordinals>(sg::load_ordinals(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md")));
    return sg::Seg(module, 1, ordinals, {}, rc_objdump(rc_rows("scenario_a.rows")));
}

bool rc_has(std::string_view text, std::string_view part) { return text.find(part) != std::string_view::npos; }

const rc::LabelOf rc_no_label = [](std::uint32_t) -> std::optional<std::string> { return std::nullopt; };

}  // namespace

CB_TEST("source.ne_asm.recon.numbers_as_masm_writes_them", [] {
    CB_CHECK_EQ(rc::masm_hex(0x1f), "1fh");
    CB_CHECK_EQ(rc::masm_hex(0xab), "0abh");   // a number that starts with a letter takes a leading zero
    CB_CHECK_EQ(rc::masm_hex(0xa), "0ah");
    CB_CHECK_EQ(rc::masm_hex(0), "0h");
    CB_CHECK_EQ(rc::masm_hex(0x100), "100h");
    CB_CHECK_EQ(rc::masm_hex(0xfffff), "0fffffh");
});

CB_TEST("source.ne_asm.recon.an_instruction_in_masm_syntax", [] {
    CB_CHECK_EQ(rc::to_masm("push   bp", rc_no_label), "push bp");
    CB_CHECK_EQ(rc::to_masm("mov    ax,WORD PTR [bp+0xc]", rc_no_label), "mov ax,word ptr [bp+0ch]");   // lower-case size words, MASM's numbers
    CB_CHECK_EQ(rc::to_masm("cmp    ax,0x10", rc_no_label), "cmp ax,10h");
    CB_CHECK_EQ(rc::to_masm("mov    eax,DWORD PTR [bx+0x10]", rc_no_label), "mov eax,dword ptr [bx+10h]");
    CB_CHECK_EQ(rc::to_masm("mov    al,BYTE PTR [bx+0xf0]", rc_no_label), "mov al,byte ptr [bx+0f0h]");
    //
    CB_CHECK_EQ(rc::to_masm("fld    FWORD PTR [bx]", rc_no_label), "fld Fword ptr [bx]");
    CB_CHECK_EQ(rc::to_masm("nop", rc_no_label), "nop");
    CB_CHECK_EQ(rc::to_masm("retf   0xa", rc_no_label), "retf 0ah");
    CB_CHECK_EQ(rc::to_masm("call   0x0:0x0", rc_no_label), "call 0h:0h");   // a far pointer is not a branch to a label
    // string instructions take MASM's size suffix, by the operand size objdump shows
    CB_CHECK_EQ(rc::to_masm("rep stos WORD PTR es:[di],ax", rc_no_label), "rep stosw");
    CB_CHECK_EQ(rc::to_masm("movs   BYTE PTR es:[di],BYTE PTR ds:[si]", rc_no_label), "movsb");
    CB_CHECK_EQ(rc::to_masm("repz cmps BYTE PTR ds:[si],BYTE PTR es:[di]", rc_no_label), "repz cmpsb");
    CB_CHECK_EQ(rc::to_masm("lods   WORD PTR ds:[si]", rc_no_label), "lodsw");
    // a direct branch goes to its label where it has one
    const rc::LabelOf labelled = [](std::uint32_t at) -> std::optional<std::string> {
        return at == 0x20 ? std::optional<std::string>("L_0020") : std::nullopt;
    };
    CB_CHECK_EQ(rc::to_masm("jne    0x20", labelled), "jne L_0020");
    CB_CHECK_EQ(rc::to_masm("jne    0x30", labelled), "jne 30h");
    CB_CHECK_EQ(rc::to_masm("jmp    0x20", labelled), "jmp L_0020");
    CB_CHECK_EQ(rc::to_masm("call   0x20", labelled), "call L_0020");
});

CB_TEST("source.ne_asm.recon.bytes_as_db_sixteen_to_a_line", [] {
    std::vector<std::string> lines;
    const cb::Bytes raw = {0x00, 0xab, 0x0a, 0x5f, 0xff};
    rc::emit_db(lines, raw, 0x1234, "note");
    CB_CHECK_EQ(lines.size(), std::size_t{1});
    CB_CHECK_EQ(lines[0], "        db      00h, 0abh, 00ah, 5fh, 0ffh   ; 1234 note");
    lines.clear();
    cb::Bytes seventeen(17, 0x41);
    rc::emit_db(lines, seventeen, 0x10, "n");
    CB_CHECK_EQ(lines.size(), std::size_t{2});
    CB_CHECK(rc_has(lines[0], "; 0010 n"));
    CB_CHECK(lines[1].ends_with("; 0020 "));   // the note goes on the first line only
    lines.clear();
    rc::emit_db(lines, {}, 0, "none");
    CB_CHECK(lines.empty());
});

CB_TEST("source.ne_asm.recon.data_as_readable_text", [] {
    // "Hi there" at 0, a NUL and a byte that is not text, then 50 characters of text, then a quote
    cb::Bytes code;
    for (const char c : std::string_view("Hi there")) code.push_back(static_cast<std::uint8_t>(c));
    code.push_back(0);
    code.push_back(0x80);
    for (int i = 0; i < 50; ++i) code.push_back('a');
    code.push_back('\'');
    code.push_back('x');
    std::vector<std::string> lines;
    rc::emit_data(lines, code, 0, static_cast<std::uint32_t>(code.size()), cb::source::ne_asm::datacode::Kind::String);
    CB_CHECK_EQ(lines[0], "        db      'Hi there'               ; 0000 string");
    CB_CHECK(rc_has(lines[1], "db      00h, 80h"));                                                 // the NUL and the byte that is not text, as hex
    CB_CHECK(rc_has(lines[2], "'" + std::string(40, 'a') + "'"));                                    // at most 40 characters a line
    CB_CHECK(rc_has(lines[3], "'" + std::string(10, 'a') + "'"));
    CB_CHECK(rc_has(lines[4], "27h"));                                                              // the quote is not text: it would end the string
    CB_CHECK(rc_has(lines.back(), "string") || rc_has(lines.back(), "'x'") || rc_has(lines.back(), "data"));
    // data of the other kind is hex all the way
    std::vector<std::string> hex;
    rc::emit_data(hex, code, 0, 4, cb::source::ne_asm::datacode::Kind::Data);
    CB_CHECK_EQ(hex.size(), std::size_t{1});
    CB_CHECK(rc_has(hex[0], "48h, 69h, 20h, 74h") && rc_has(hex[0], "; 0000 data"));
});

CB_TEST("source.ne_asm.recon.the_body_of_a_function", [] {
    const sg::Seg seg = rc_segment();
    // [0, 0x20): its branches to 0x20 and beyond leave it; `je 0x40` goes out too
    const std::vector<std::string> body = rc::emit_function(seg, 0, 0x20, {});
    const std::string text = cb::text::join(body, "\n");
    CB_CHECK(rc_has(text, "push bp"));
    CB_CHECK(rc_has(text, "mov bp,sp"));
    CB_CHECK(rc_has(text, "; 0000 "));
    // a branch to a place outside the range is db: the label is not there
    CB_CHECK(rc_has(text, "db      "));
    CB_CHECK(!rc_has(text, "L_0040"));
    // every offset of the range is written: 0x20 is the end of the range, so each row below it, once
    for (const std::uint32_t at : {0x0u, 0x1u, 0x3u, 0x4u, 0x7u, 0xau, 0xcu, 0xfu, 0x11u, 0x14u, 0x17u, 0x19u, 0x1au}) CB_CHECK(rc_has(text, std::format("; {:04x}", at)));
    CB_CHECK(!rc_has(text, "; 0020"));
    // a branch inside the range gets its label: [0x90, 0xa0) has none; [0x0, 0x60) branches to 0x20, 0x40 inside
    const std::string wide = cb::text::join(rc::emit_function(seg, 0, 0x60, {}), "\n");
    CB_CHECK(rc_has(wide, "L_0020:\n"));
    CB_CHECK(rc_has(wide, "L_0040:\n"));
    CB_CHECK(rc_has(wide, "jne L_0020"));
    CB_CHECK(rc_has(wide, "je L_0040"));
    // a row in the force set is db whole, with its text in the note
    const std::string forced = cb::text::join(rc::emit_function(seg, 0, 0x20, {0x4}), "\n");
    CB_CHECK(rc_has(forced, "; 0004 mov ax,word ptr [bp+0ch]"));
    CB_CHECK(rc_has(forced, "db      "));
    // far calls and jumps (a pointer, always outside) and the unfinished rows past the end are db
    const std::string far = cb::text::join(rc::emit_function(seg, 0x40, 0x50, {}), "\n");
    CB_CHECK(rc_has(far, "call 0h:0h"));   // (the canned rows are not the segment's bytes: this one is not a 9A far call, so it is written as an instruction)
    CB_CHECK(rc_has(far, "; 0044"));
});

CB_TEST("source.ne_asm.recon.the_wrapped_source", [] {
    const std::vector<std::string> body = {"        nop", "L:", "        ret"};
    const std::string source = rc::wrap(body);
    CB_CHECK(source.starts_with("        title   asmrecon\nretf    macro   n\n"));
    CB_CHECK(rc_has(source, "_TEXT   segment byte public 'CODE'\n        assume  cs:_TEXT\n        nop\nL:\n        ret\n_TEXT   ends\n        end\n"));
    CB_CHECK(source.ends_with("        end\n"));
    CB_CHECK(rc_has(source, "        db      0cbh\n        else\n        db      0cah\n        dw      n\n"));   // the retf macro
});

CB_TEST("source.ne_asm.recon.the_divergers_come_from_the_listing", [] {
    // three instructions of the original at 0 (1 byte), 1 (2 bytes) and 3 (3 bytes); MASM wrote the second in 3 bytes and the third at 4
    const std::map<std::uint32_t, std::uint32_t> spans = {{0, 1}, {1, 3}, {3, 6}};
    const std::string listing =
        "Microsoft (R) Macro Assembler\r\n"
        "\r\n"
        " 0000 55                      push bp                 ; 0000\r\n"
        " 0001 8B EC 90                mov bp,sp               ; 0001\r\n"
        " 0004 E8 0000 E               call  near_thing        ; 0003\r\n"
        "L1:\r\n"
        " 0007                         label_only\r\n";
    const cb::Bytes ours = {0x55, 0x8B, 0xEC, 0x90, 0xE8, 0x00, 0x00};
    const cb::Bytes shipped = {0x55, 0x8B, 0xEC, 0xE8, 0x12, 0x34};
    const sd::Mask none;
    const rc::Divergers d = rc::divergers(listing, ours, none, shipped, none, spans, 0);
    CB_CHECK(d.length == (std::set<std::uint32_t>{1}));   // the second is three bytes of MASM's, two of the original's
    // the third: three and three bytes; the displacement the loader fills in is masked on both sides, so it is not a difference
    CB_CHECK(d.re_encode == (std::set<std::uint32_t>{3}));   // unmasked, the displacement differs too
    const sd::Mask ours_mask = {5, 6}, shipped_mask = {4, 5};   // the field after E8 (ours at 4: bytes 5 and 6; the original's at 3: 4 and 5)
    CB_CHECK(rc::divergers(listing, ours, ours_mask, shipped, shipped_mask, spans, 0).re_encode.empty());
    // the same length, the opcode another: a re-encoding
    const cb::Bytes other_ours = {0x55, 0x8B, 0xEC, 0x90, 0xE9, 0x00, 0x00};   // jmp where the original has call
    const rc::Divergers e = rc::divergers(listing, other_ours, ours_mask, shipped, shipped_mask, spans, 0);
    CB_CHECK(e.re_encode == (std::set<std::uint32_t>{3}));
    // the pairs themselves: a label has no byte field, a heading no offset
    const std::vector<rc::ListedPair> pairs = rc::masm_listing_pairs(listing);
    CB_CHECK_EQ(pairs.size(), std::size_t{3});
    CB_CHECK_EQ(pairs[1].first, std::uint32_t{1});
    CB_CHECK(pairs[2].second == std::optional<std::uint32_t>(3));
    // a comment MASM wrapped onto the next line is found there
    const std::vector<rc::ListedPair> wrapped = rc::masm_listing_pairs(" 0010 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10\r\n                  ; 0042\r\n");
    CB_CHECK_EQ(wrapped.size(), std::size_t{1});
    CB_CHECK(wrapped[0].second == std::optional<std::uint32_t>(0x42));
    // a line past the original's table of spans (an uncovered db line) says nothing
    CB_CHECK(rc::divergers(" 0000 55   db 55h ; 0999\r\n", ours, none, shipped, none, spans, 0).length.empty());
});

CB_TEST("source.ne_asm.recon.the_listing_of_nasm", [] {
    const std::string listing =
        "     1                                  ; a comment\n"
        "     2 00000000 55                      push bp                ; 0000\n"
        "     3 00000001 8BEC                    mov bp,sp              ; 0001\n"
        "     4                                  <1> a macro line with no offset\n"
        "     5 00000003 E8                      call  x                ; 0003\n";
    const std::vector<rc::ListedPair> pairs = rc::nasm_listing_pairs(listing);
    CB_CHECK_EQ(pairs.size(), std::size_t{3});
    CB_CHECK_EQ(pairs[0].first, std::uint32_t{0});
    CB_CHECK_EQ(pairs[1].first, std::uint32_t{1});
    CB_CHECK(pairs[1].second == std::optional<std::uint32_t>(1));
    CB_CHECK_EQ(pairs[2].first, std::uint32_t{3});
    CB_CHECK(pairs[2].second == std::optional<std::uint32_t>(3));
    CB_CHECK(rc::nasm_listing_pairs("").empty());
});

CB_TEST("source.ne_asm.recon.error_lines_of_each_assembler", [] {
    CB_CHECK(rc::masm_error_lines("t.asm(12): error A2009: syntax error\nt.asm(7) : fatal error A1000: x\nt.asm(3): warning A4001\nfoo(1)") == (std::vector<std::size_t>{12, 7}));
    CB_CHECK(rc::masm_error_lines("").empty());
    CB_CHECK(rc::nasm_error_lines("m.asm:12: error: parser\nm.asm:5: Fatal: x\nm.asm:9: warning: y\n") == (std::vector<std::size_t>{12, 5}));
    CB_CHECK(rc::nasm_error_lines("m.asm(12): error").empty());
    CB_CHECK(rc::comment_offset("        nop ; 0123") == std::optional<std::uint32_t>(0x123));
    CB_CHECK(!rc::comment_offset("        nop ; 01234"));    // (five digits is not an offset of four)
    CB_CHECK(!rc::comment_offset("        nop ; 01G3"));
    CB_CHECK(!rc::comment_offset("        nop"));
    CB_CHECK(rc::comment_offset("; 12345", 5) == std::optional<std::uint32_t>(0x12345));
});

CB_TEST("source.ne_asm.recon.an_instruction_the_assembler_refuses_is_data", [] {
    const sg::Seg seg = rc_segment();
    std::vector<std::string> sources;
    // an assembler that refuses the instruction at 0x0003 (the first time it is a live instruction) and makes nothing else
    const rc::Assemble refuses = [&](std::string_view source) {
        sources.emplace_back(source);
        const std::vector<std::string_view> lines = cb::text::lines(source);
        rc::Assembled made;
        for (std::size_t i = 0; i < lines.size(); ++i)
            if (lines[i].find("        push si") == 0 && lines[i].find("; 0003") != std::string_view::npos) made.log = std::format("t.asm({}): error A2009: syntax error\n", i + 1);
        return made;
    };
    const rc::Reconciled r = rc::reconcile(seg, 0, 0x20, {}, refuses);
    CB_CHECK(!r.verdict.has_value());   // it never made an object
    CB_CHECK(!r.exact());
    CB_CHECK(sources.size() >= 3);      // refused; then no object and no error: everything db; then the end
    CB_CHECK(rc_has(sources.front(), "        push si"));
    CB_CHECK(!rc_has(sources[1], "        push si"));   // the refused instruction is db from the second time on
    std::size_t rows = 0;
    for (const sg::Row& row : seg.rows()) rows += row.offset < 0x20;
    CB_CHECK_EQ(r.instructions, rows - 1);          // what the assembler refused is data, and not counted
    CB_CHECK(!r.force_db.contains(3));              // ... nor as a pinned instruction
    CB_CHECK(r.force_db.contains(0));               // the others were all pinned by the last resort
    CB_CHECK_EQ(r.force_db.size(), rows - 1);
    // the all-db source is what was made at the end
    CB_CHECK(!rc_has(r.source, "        push bp"));
    CB_CHECK(rc_has(r.source, "db      "));
});

CB_TEST("source.ne_asm.recon.the_selftest_needs_a_module", [] {
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    cb::fs::ScopedTempDir dir("recontest");
    cb::fs::ScopedEnv root{"CB_CORPUS_ROOT", cb::fs::to_text(dir.path())};
    cb::base::Products products;
    products.target = "win95";
    products.target_folder = dir.path();
    products.checkout_root = dir.path() / "CodeBreaker";
    products.state = dir.path() / "state";
    const cb::base::Corpus corpus(products);
    const rc::Environment env{corpus, [](std::string_view) { return rc::Assembled{}; }, rc_objdump(rc_rows("scenario_a.rows"))};
    {
        cb::testing::CapturedStd cap;   // none of the five modules the selftest looks for is in an empty corpus
        CB_CHECK_EQ(rc::main(env, std::vector<std::string>{"--selftest"}), cb::kExitFailed);
        CB_CHECK(cap.err().find("no suitable NE function found for selftest") != std::string::npos);
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_THROWS_AS(cb::UsageError, rc::main(env, std::vector<std::string>{}));   // no module, no --selftest
    }
    {
        cb::testing::CapturedStd cap;   // a function nobody carved there
        const std::string lz = cb::fs::to_text(cb::testing::data_path("formats/testdata/lzexpand.dll"));
        const std::string ords = cb::fs::to_text(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md"));
        CB_CHECK_EQ(rc::main(env, std::vector<std::string>{lz, "--seg", "1", "--func", "0x7777", "--ords", ords}), cb::kExitFailed);
        CB_CHECK(cap.err().find("no function carved at ") != std::string::npos);
    }
});
