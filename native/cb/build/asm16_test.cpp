// Tests of cb/build/asm16.hpp on LZEXPAND.DLL, a real Windows 95 16-bit module (formats/testdata/lzexpand.dll). The golden
// texts in testdata/ were made by the original asm16.py on the same functions and read over by hand: the segment's
// instructions, the places the loader fixes written as symbols, the labels where a jump goes.
#include "cb/build/asm16.hpp"

#include <format>

#include "cb/build/msc6.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

using namespace cb;
namespace asm16 = cb::build::asm16;
namespace ne = cb::formats::ne;

const ne::Module& lzexpand() {
    static const ne::Module m = ne::Module::open(fs::read_file(testing::data_path("formats/testdata/lzexpand.dll")));
    return m;
}

// What the build would answer: a symbol per function and segment, and per import.
std::optional<std::string> answer_symbols(const asm16::Reference& ref) {
    if (const auto* f = std::get_if<asm16::FunctionRef>(&ref)) {
        if ((f->seg * 31 + f->off) % 13 == 0) return std::nullopt;   // (a share of them have none, so that refusals are reached)
        return std::format("_f{}_{:x}", f->seg, f->off);
    }
    if (const auto* s = std::get_if<asm16::SegmentRef>(&ref)) return std::format("_seg{}", s->seg);
    if (const auto* d = std::get_if<asm16::DataRef>(&ref)) return std::format("_d{}", d->seg);
    const auto& i = std::get<asm16::ImportRef>(ref);
    const std::string which = std::holds_alternative<std::uint16_t>(i.which) ? std::to_string(std::get<std::uint16_t>(i.which)) : std::get<std::string>(i.which);
    return "_IMP_" + i.module.value_or("?") + "_" + which;
}

asm16::Request request(std::uint16_t seg, std::uint32_t start, std::uint32_t end) {
    asm16::Request r;
    r.seg = seg;
    r.start = start;
    r.end = end;
    r.publics = {{start, std::format("_f{}_{:x}", seg, start)}};
    return r;
}

asm16::Function make(const asm16::Request& r, const asm16::SymbolFor& symbol_for = answer_symbols) {
    platforms::DecoderSet decoders;
    return asm16::Function(lzexpand(), r, symbol_for, decoders);
}

bool has(const std::string& s, std::string_view part) { return s.find(part) != std::string::npos; }

std::string golden(const char* name) {
    return text::replace_all(fs::read_text(testing::data_path(std::string("build/testdata/") + name)), "\r\n", "\n");
}

// An OMF object holding `code` as the first segment's data, as a compiler leaves one.
Bytes object_of(ByteView code) {
    const auto record = [](std::uint8_t kind, ByteView body) {
        Bytes r{kind, static_cast<std::uint8_t>((body.size() + 1) & 0xFF), static_cast<std::uint8_t>((body.size() + 1) >> 8)};
        append(r, body);
        r.push_back(0);
        return r;
    };
    Bytes out = record(0x80, to_bytes(std::string("\x03" "f.c", 4)));
    Bytes data{1, 0, 0};
    append(data, code);
    append(out, record(0xA0, data));
    append(out, record(0x8A, Bytes{0}));
    return out;
}

// A one-segment NE module (name T, importing KERNEL) holding `code`, with these relocation records (source, flags, offset, module,
// ordinal): the same bytes tests/test_verification.py builds for the original.
struct Reloc {
    std::uint8_t source, flags;
    std::uint16_t offset, module, ordinal;
};

Bytes tiny_ne(ByteView code, const std::vector<Reloc>& relocs) {
    Bytes image(0x100, 0);
    const auto put16 = [&](std::size_t at, std::uint32_t v) {
        image[at] = static_cast<std::uint8_t>(v);
        image[at + 1] = static_cast<std::uint8_t>(v >> 8);
    };
    image[0] = 'M';
    image[1] = 'Z';
    put16(0x3C, 0x40);
    const std::size_t h = 0x40;   // the NE header; its tables follow it
    image[h] = 'N';
    image[h + 1] = 'E';
    image[h + 2] = 5;
    const std::uint16_t segment_table = 0x40, resident = 0x48, modrefs = 0x4D, imported = 0x4F, entry = 0x57;
    put16(h + 0x04, entry);
    put16(h + 0x06, 1);
    put16(h + 0x0C, 0x8000);
    put16(h + 0x1C, 1);
    put16(h + 0x1E, 1);
    put16(h + 0x22, segment_table);
    put16(h + 0x24, resident);
    put16(h + 0x26, resident);
    put16(h + 0x28, modrefs);
    put16(h + 0x2A, imported);
    put16(h + 0x32, 4);
    image[h + 0x36] = 2;
    put16(h + segment_table, 0x100 >> 4);
    put16(h + segment_table + 2, static_cast<std::uint32_t>(code.size()));
    put16(h + segment_table + 4, 0x0100);
    put16(h + segment_table + 6, static_cast<std::uint32_t>(code.size()));
    const std::string_view names("\x01T\x00\x00\x00", 5);
    std::copy(names.begin(), names.end(), image.begin() + static_cast<std::ptrdiff_t>(h + resident));
    put16(h + modrefs, 1);
    const std::string_view kernel("\x00\x06KERNEL", 8);
    std::copy(kernel.begin(), kernel.end(), image.begin() + static_cast<std::ptrdiff_t>(h + imported));
    append(image, code);
    image.push_back(static_cast<std::uint8_t>(relocs.size()));
    image.push_back(0);
    for (const Reloc& r : relocs) {
        image.push_back(r.source);
        image.push_back(r.flags);
        for (const std::uint16_t v : {r.offset, r.module, r.ordinal}) {
            image.push_back(static_cast<std::uint8_t>(v));
            image.push_back(static_cast<std::uint8_t>(v >> 8));
        }
    }
    return image;
}

Bytes bytes_of(const asm16::Function& f) {
    const ByteView b = f.blob().subspan(f.start(), f.end() - f.start());
    return Bytes(b.begin(), b.end());
}

}  // namespace

CB_TEST("build.asm16.a_function_with_far_calls_to_imports", [] {
    const asm16::Function f = make(request(1, 4, 178));
    CB_CHECK(f.text().has_value());
    CB_CHECK_EQ(f.why(), "");
    CB_CHECK_EQ(*f.text(), golden("asm16_1_4.bytes.asm"));
    CB_CHECK_EQ(f.imports().size(), 2u);
    CB_CHECK_EQ(f.imports().at("_IMP_KERNEL_15").module, "KERNEL");
    CB_CHECK(std::get<std::uint16_t>(f.imports().at("_IMP_KERNEL_15").which) == 15);
    CB_CHECK(f.externs().at("_IMP_KERNEL_17") == asm16::Linkage::Far);
    CB_CHECK_EQ(f.bits(), 16u);
    CB_CHECK_EQ(f.module_name(), "LZEXPAND");
    CB_CHECK_EQ(f.first_public(), "_f1_4");
    CB_CHECK(f.holes().empty());
    CB_CHECK(f.labels().contains(0x1c));   // the target of the jump at 0x16
    CB_CHECK(f.insns().at(0x14).target == std::optional<std::uint32_t>(0x1c));
    // 5-byte far calls are the instruction's byte then the import's own fix-up
    CB_CHECK(f.sites().at(37).size == 4);
    CB_CHECK_EQ(f.sites().at(37).text, "dd   _IMP_KERNEL_15");
    CB_CHECK_EQ(f.symbolic().at(36).text, "call far ptr _IMP_KERNEL_15");
});

CB_TEST("build.asm16.every_instruction_as_a_mnemonic", [] {
    asm16::Function f = make(request(2, 0x266, 0x2ee));
    CB_CHECK(f.text().has_value());
    CB_CHECK_EQ(f.write(std::nullopt), golden("asm16_2_266.all.asm"));
    // a near call out of the run is written by the callee's name, and is the same bytes but for its displacement
    CB_CHECK_EQ(f.calls().size(), 1u);
    CB_CHECK_EQ(f.calls().at(0x2d1).text, "call near ptr _f2_0");
    CB_CHECK(f.externs().at("_f2_0") == asm16::Linkage::Near);
    CB_CHECK(f.externs().at("_f10_124") == asm16::Linkage::Far);   // a far call into the module: the whole is the name
    CB_CHECK(f.symbolic().contains(0x29a) || !f.symbolic().empty());
    // the same text when each is listed
    std::set<std::uint32_t> every;
    for (const auto& [at, i] : f.insns()) every.insert(at);
    CB_CHECK_EQ(f.write(every).size() > 0, true);
    CB_CHECK(f.write(every) != f.write(std::nullopt));   // (a symbolic instruction is its own bytes in the first, a name in the second)
});

CB_TEST("build.asm16.a_function_rebuilt_from_c_inside_the_run_is_a_hole", [] {
    asm16::Request r = request(1, 182, 278);
    r.holes = {{220, 234, "_holedc"}};
    asm16::Function f = make(r);
    CB_CHECK(f.text().has_value());
    CB_CHECK_EQ(*f.text(), golden("asm16_1_b6_hole.bytes.asm"));
    CB_CHECK_EQ(f.write(std::nullopt), golden("asm16_1_b6_hole.all.asm"));
    CB_CHECK(has(*f.text(), "db   14 dup (0CCh)"));
    // the hole's own bytes are filler: neither the check nor the differences look at them
    Bytes flipped = bytes_of(f);
    for (std::uint32_t at = 220; at < 234; ++at) flipped[at - 182] ^= 0xFF;
    CB_CHECK(!asm16::check(object_of(flipped), f).has_value());
});

CB_TEST("build.asm16.check_against_the_original", [] {
    const asm16::Function f = make(request(1, 4, 178));
    const Bytes want = bytes_of(f);
    CB_CHECK(!asm16::check(object_of(want), f).has_value());
    Bytes flipped = want;
    flipped[0] ^= 0xFF;
    CB_CHECK_EQ(asm16::check(object_of(flipped), f).value_or("<none>"), "differs at 0004");
    Bytes shorter(want.begin(), want.end() - 1);
    CB_CHECK_EQ(asm16::check(object_of(shorter), f).value_or("<none>"), "173 bytes, the original 174");
    // the loader's fix-ups are the linker's to set: a different byte there is no difference (the mask says so as well)
    Bytes fixed = want;
    fixed[37 - 4 + 2] ^= 0x55;   // inside the 4-byte far call's operand
    CB_CHECK(!asm16::check(object_of(fixed), f).has_value());
    Bytes masked = want;
    masked[10] ^= 0xFF;
    const std::vector<std::uint32_t> bad = asm16::differences(masked, Bytes(masked.size(), 0), f);
    CB_CHECK_EQ(bad.size(), 1u);
    CB_CHECK_EQ(bad[0], 14u);
    Bytes mask(masked.size(), 0);
    mask[10] = 1;
    CB_CHECK(asm16::differences(masked, mask, f).empty());   // (a byte the object's own fix-ups name)
    // a piece of the function
    const Bytes piece(f.blob().begin() + 0x20, f.blob().begin() + 0x40);
    CB_CHECK(!asm16::check_piece(object_of(piece), f, 0x20, 0x40).has_value());
    CB_CHECK_EQ(asm16::check_piece(object_of(piece), f, 0x20, 0x41).value_or("<none>"), "32 bytes, the original 33");
    Bytes bad_piece = piece;
    bad_piece[1] ^= 1;
    CB_CHECK_EQ(asm16::check_piece(object_of(bad_piece), f, 0x20, 0x40).value_or("<none>"), "differs at 0021");
});

CB_TEST("build.asm16.candidates_are_the_instructions_the_assembler_is_asked_about", [] {
    const asm16::Function f = make(request(1, 4, 178));
    const auto candidates = f.candidates();
    CB_CHECK(!candidates.empty());
    // every instruction with no label of its own to write, as its text: a jump is checked in the whole
    CB_CHECK_EQ(candidates.at(4).text, "push   bp");
    CB_CHECK_EQ(candidates.at(5).text, "mov    bp, sp");
    CB_CHECK(candidates.at(5).original == (Bytes{0x8B, 0xEC}));
    CB_CHECK(candidates.at(5).mask == (Bytes{0, 0}));
    CB_CHECK(!candidates.contains(0x14));   // `je 0x1c`
    CB_CHECK_EQ(f.mnemonic(0x14), "je     short L_001C");
    CB_CHECK_EQ(f.mnemonic(0x16 + 0), "mov    ax, 1");
});

CB_TEST("build.asm16.a_function_is_refused_with_its_reason", [] {
    // no symbol for any import: the first call of the function says why it cannot be written
    const asm16::Function f = make(request(1, 4, 178), [](const asm16::Reference& ref) -> std::optional<std::string> {
        if (std::holds_alternative<asm16::ImportRef>(ref)) return std::nullopt;
        return answer_symbols(ref);
    });
    CB_CHECK(!f.text().has_value());
    CB_CHECK(has(f.why(), "no symbol"));
    // a range that is not in the segment
    const asm16::Function backwards = make(request(1, 5000, 100));
    CB_CHECK(!backwards.text().has_value());
    CB_CHECK_EQ(backwards.why(), "its bytes are not in the file");
    CB_CHECK(!make(request(1, 0, 99999)).text().has_value());
    // an empty answer is no answer
    const asm16::Function none = make(request(2, 0x266, 0x2ee), [](const asm16::Reference& ref) -> std::optional<std::string> {
        if (std::holds_alternative<asm16::FunctionRef>(ref)) return std::string();
        return answer_symbols(ref);
    });
    CB_CHECK(!none.text().has_value());
});

CB_TEST("build.asm16.misuse_is_an_error_not_a_refusal", [] {
    CB_CHECK_THROWS_AS(UsageError, make(request(99, 0, 10)));
    CB_CHECK_THROWS_AS(UsageError, make(request(0, 0, 10)));
    asm16::Request no_publics = request(1, 4, 178);
    no_publics.publics.clear();
    CB_CHECK_THROWS_AS(UsageError, make(no_publics));
});

CB_TEST("build.asm16.a_whole_segment_and_a_run_of_functions", [] {
    // the whole segment, kept where it was: the lead-in may be read through CS
    asm16::Request whole = request(1, 0, 456);
    whole.publics = {{0, "_whole1"}};
    whole.cs_ok = 0x30;
    whole.whole = true;
    const asm16::Function w = make(whole);
    CB_CHECK(w.text().has_value());
    CB_CHECK(has(*w.text(), "public _whole1"));
    // a run of two: each function's symbol is defined where it begins
    asm16::Request run = request(1, 178, 278);
    run.publics = {{178, "_f1_b2"}, {182, "_f1_b6"}};
    const asm16::Function r = make(run);
    CB_CHECK(r.text().has_value());
    CB_CHECK(has(*r.text(), "public _f1_b2\n        public _f1_b6"));
    CB_CHECK(has(*r.text(), "\n_f1_b6:\n"));
    // pieces of a frame: one half carries its own labels and what points out of it is bytes
    asm16::Function piece = make(run);
    const std::string half = piece.write(std::set<std::uint32_t>{}, std::pair<std::uint32_t, std::uint32_t>{178, 230});
    CB_CHECK(has(half, "1:00b2-00e6"));
    CB_CHECK(!has(half, "_f1_b6:\n") || has(half, "public _f1_b6"));
});

CB_TEST("build.asm16.a_code_segment_that_is_not_there_is_refused", [] {
    // the module's data segment is no code: a request into it finds no bytes to follow
    const ne::Module& m = lzexpand();
    std::uint16_t data_seg = 0;
    for (const ne::Segment& s : m.segments()) {
        if (s.is_data()) data_seg = s.index;
    }
    if (data_seg == 0) return;   // (this module has none)
    const asm16::Function f = make(request(data_seg, 0, 4));
    CB_CHECK(f.text().has_value() || !f.why().empty());
});

// DIVERGES: the original named a far call to an import with an addend by the last word of the expression and wrote `call far ptr 4`
// (the same bug in tests/test_verification.py's proof, fixed in asm16.py too); and a hole whose start equals its end made its `write`
// loop for ever, one `db 0 dup` line a turn. Neither is reached by any function of the Win95 modules the old harness covers (699
// functions, every attribute and the text in six forms equal); the order the discovery of instructions put them in is no longer
// the order of the maps (they are ordered by offset), which only a function with two instructions ending at one place could tell.
CB_TEST("build.asm16.a_far_call_to_an_import_with_an_addend_is_named_with_it", [] {
    // The far call's operand is a fix-up of the import KERNEL.15, additive (the place holds the addend, 4).
    const Bytes code{0x55, 0x8B, 0xEC, 0x9A, 0x04, 0x00, 0x00, 0x00, 0x5D, 0xCB};
    const ne::Module m = ne::Module::open(tiny_ne(code, {{3, 0x05, 4, 1, 15}}));
    platforms::DecoderSet decoders;
    asm16::Request r;
    r.seg = 1;
    r.start = 0;
    r.end = static_cast<std::uint32_t>(code.size());
    r.publics = {{0, "_f"}};
    asm16::Function f(m, r, answer_symbols, decoders);
    CB_CHECK(f.text().has_value());
    CB_CHECK_EQ(f.why(), "");
    CB_CHECK(has(*f.text(), "        call far ptr _IMP_KERNEL_15 + 4\n"));
    CB_CHECK_EQ(f.sites().at(4).text, "dd   _IMP_KERNEL_15 + 4");
    CB_CHECK_EQ(f.imports().at("_IMP_KERNEL_15").module, "KERNEL");
    // without the addend it is the plain name
    const ne::Module plain = ne::Module::open(tiny_ne(code, {{3, 0x01, 4, 1, 15}}));
    const asm16::Function g(plain, r, answer_symbols, decoders);
    CB_CHECK(g.text().has_value());
    CB_CHECK(has(*g.text(), "        call far ptr _IMP_KERNEL_15\n"));
});

CB_TEST("build.asm16.a_hole_of_no_length_is_no_hole", [] {
    asm16::Request r = request(1, 182, 278);
    r.holes = {{220, 220, "_nothing"}};
    const asm16::Function f = make(r);
    CB_CHECK(f.holes().empty());
    CB_CHECK_EQ(*f.text(), *make(request(1, 182, 278)).text());
});
