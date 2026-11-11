#include "cb/exact/recut.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/fixupp.hpp"
#include "cb/exact/omf.hpp"

using namespace cb;
using namespace cb::exact;

// DIVERGES from the Python module (recut.py), on purpose:
//   * a header whose relocation table lies outside the file is a FormatError (the Python sliced what was there and
//     failed in struct.iter_unpack unless the cut happened to fall on an entry)
//   * a FIXUPP record before any data record, a segment named by an index no LNAMES gave, a thread no record defined
//     and a record that ends inside a field are FormatError/TruncatedError (the Python raised TypeError, IndexError,
//     KeyError and IndexError)
//   * the fix-up's location is read as the 4 bits it is (the Python kept 3, so a location of 8 or more -- a 32-bit
//     form, which 16-bit records do not hold -- came back changed)

namespace {

constexpr std::int64_t kBase = 0x100;   // where the object's first public lies in the program

Bytes u16(std::uint32_t v) { return {static_cast<std::uint8_t>(v & 0xFF), static_cast<std::uint8_t>(v >> 8)}; }

Bytes cat(std::initializer_list<ByteView> parts) {
    Bytes out;
    for (const ByteView p : parts) append(out, p);
    return out;
}

// A fix-up of a segment word (location 2) or a pointer (3) at `at` that names segment 1.
fixupp::Fixup fix(std::uint32_t at, std::uint8_t location = 2) {
    fixupp::Fixup f;
    f.segment_relative = true;
    f.location = location;
    f.frame_method = 0;
    f.frame_index = std::uint16_t{1};
    f.target_method = 0;
    f.target_index = std::uint16_t{1};
    f.no_displacement = true;
    f.at = at;
    return f;
}

// One code segment _TEXT of `size` bytes (the byte at i is i & 0xFF) with a public `_main` at 0, in the compiler's way:
// data records of at most 1024 bytes, each followed by one FIXUPP record listing the fix-ups that lie in it in the order
// given (a fix-up's `at` is its place in the segment).
Bytes make_object(std::uint32_t size, const std::vector<fixupp::Fixup>& fixups) {
    const Bytes name{0, 5, '_', 'T', 'E', 'X', 'T', 4, 'C', 'O', 'D', 'E'};
    const Bytes segdef = cat({Bytes{0x28}, u16(size), Bytes{2, 3, 1}});
    const Bytes pubdef = {0, 1, 5, '_', 'm', 'a', 'i', 'n', 0, 0, 0};
    Bytes out;
    append(out, omf::record(0x80, Bytes{1, 't'}));
    append(out, omf::record(0x96, name));
    append(out, omf::record(0x98, segdef));
    append(out, omf::record(0x90, pubdef));
    for (std::uint32_t begin = 0; begin < size; begin += 1024) {
        const std::uint32_t end = std::min(size, begin + 1024);
        Bytes data = {1};   // the segment's index
        append(data, u16(begin));
        for (std::uint32_t i = begin; i < end; ++i) data.push_back(static_cast<std::uint8_t>(i & 0xFF));
        append(out, omf::record(0xA0, data));
        std::vector<fixupp::Fixup> here;
        for (const fixupp::Fixup& f : fixups) {
            if (f.at >= begin && f.at < end) here.push_back(f);
        }
        if (!here.empty()) append(out, fixupp::encode(here, begin));
    }
    append(out, omf::record(0x8A, Bytes{0}));
    return out;
}

// What a module's data records hold: where each begins, its bytes, and the fix-ups listed after it (places as offsets in
// the segment).
struct Piece {
    std::uint32_t begin = 0;
    Bytes data;
    std::vector<fixupp::Fixup> fixups;
};

std::vector<Piece> pieces_of(ByteView obj) {
    std::vector<Piece> out;
    fixupp::Threads threads;
    for (const omf::Record& r : omf::records(obj)) {
        if (r.type == 0xA0) {
            Piece p;
            const auto seg = fixupp::read_index(r.payload, 0);
            p.begin = r.payload[seg.next] | (r.payload[seg.next + 1] << 8);
            p.data.assign(r.payload.begin() + static_cast<std::ptrdiff_t>(seg.next) + 2, r.payload.end());
            out.push_back(std::move(p));
        } else if (r.type == 0x9C) {
            for (fixupp::Fixup f : fixupp::parse(r.payload, threads)) {
                f.at = out.back().begin + f.offset;
                out.back().fixups.push_back(f);
            }
        }
    }
    return out;
}

std::vector<std::uint32_t> places(const Piece& p) {
    std::vector<std::uint32_t> out;
    for (const fixupp::Fixup& f : p.fixups) out.push_back(f.at);
    return out;
}

const SymbolTable& symbols() {
    static const SymbolTable table{{"_main", SymPlace{SymKind::Code, kBase}}};
    return table;
}

recut::Original original(const SymbolTable& table, std::vector<std::uint32_t> relocs) {
    return {table, std::move(relocs), std::nullopt, {}};
}

}  // namespace

CB_TEST("exact.recut.cuts_where_the_relocations_run_up_again", [] {
    // The compiler wrote one record with its fix-ups last first (30, 12, 4). The program's relocations come 12, 4 then
    // 30: a run down, then up -- the original's first record ended before 30.
    const Bytes raw = make_object(40, {fix(30), fix(12), fix(4)});
    const Bytes out = recut::recut(original(symbols(), {kBase + 12, kBase + 4, kBase + 30}), raw);
    const std::vector<Piece> pieces = pieces_of(out);
    CB_CHECK_EQ(pieces.size(), std::size_t{2});
    CB_CHECK_EQ(pieces[0].begin, 0u);
    CB_CHECK_EQ(pieces[0].data.size(), std::size_t{30});   // the cut is as late as the next word allows
    CB_CHECK_EQ(pieces[1].begin, 30u);
    CB_CHECK_EQ(pieces[1].data.size(), std::size_t{10});
    CB_CHECK((places(pieces[0]) == std::vector<std::uint32_t>{12, 4}));   // in the original's order
    CB_CHECK((places(pieces[1]) == std::vector<std::uint32_t>{30}));
    // the bytes and everything but the cut are the module's own
    const omf::Object before = omf::read(raw), after = omf::read(out);
    CB_CHECK(before.segments.at("_TEXT") == after.segments.at("_TEXT"));
    CB_CHECK_EQ(after.publics.size(), std::size_t{1});
    CB_CHECK_EQ(after.fixups.size(), std::size_t{3});
    // the module's other records (THEADR, LNAMES, SEGDEF, PUBDEF, MODEND) are kept, in place
    const std::vector<omf::Record> recs = omf::records(out);
    CB_CHECK_EQ(recs.front().type, 0x80);
    CB_CHECK_EQ(recs.back().type, 0x8A);
    // cutting what is already cut that way changes nothing
    CB_CHECK(recut::recut(original(symbols(), {kBase + 12, kBase + 4, kBase + 30}), out) == out);
});

CB_TEST("exact.recut.a_pointer_counts_its_segment_word", [] {
    // a segment word at 4 and a pointer at 20 (its segment word is the one at 22), in the order they lie
    const Bytes raw = make_object(40, {fix(20, 3), fix(4)});
    const Bytes out = recut::recut(original(symbols(), {kBase + 4, kBase + 22}), raw);
    const std::vector<Piece> pieces = pieces_of(out);
    CB_CHECK_EQ(pieces.size(), std::size_t{2});
    // the cut goes as late as the pointer's offset word: not between its two words
    CB_CHECK_EQ(pieces[1].begin, 20u);
    CB_CHECK((places(pieces[0]) == std::vector<std::uint32_t>{4}));
    CB_CHECK((places(pieces[1]) == std::vector<std::uint32_t>{20}));
    CB_CHECK_EQ(pieces[1].fixups[0].location, 3);
});

CB_TEST("exact.recut.left_alone_when_no_cut_fits", [] {
    // the runs end at the word at 4 and begin at the word at 8, so the cut would lie at 6, 7 or 8 -- but fix-ups at 5, 6
    // and 7 (low bytes, overlapping) split a word at every one of them: the module is left as it is
    const Bytes raw = make_object(40, {fix(4), fix(5, 0), fix(6, 0), fix(7, 0), fix(8)});
    CB_CHECK(recut::recut(original(symbols(), {kBase + 4, kBase + 8}), raw) == raw);
    // without them the cut is made (at 8, the latest)
    const Bytes clear = make_object(40, {fix(4), fix(8)});
    const std::vector<Piece> pieces = pieces_of(recut::recut(original(symbols(), {kBase + 4, kBase + 8}), clear));
    CB_CHECK_EQ(pieces.size(), std::size_t{2});
    CB_CHECK_EQ(pieces[1].begin, 8u);
});

CB_TEST("exact.recut.left_alone_when_the_relocations_are_not_the_programs", [] {
    const Bytes raw = make_object(40, {fix(30), fix(12), fix(4)});
    // one of the module's words is missing from the program's relocations
    CB_CHECK(recut::recut(original(symbols(), {kBase + 12, kBase + 30}), raw) == raw);
    // a place the program relocates that the module has no fix-up at is not the module's: it is ignored
    CB_CHECK(recut::recut(original(symbols(), {kBase + 12, kBase + 4, kBase + 30, kBase + 8}), raw) != raw);
    // a program with none (an EXEPACK one read without its table)
    CB_CHECK(recut::recut(original(symbols(), {}), raw) == raw);
});

CB_TEST("exact.recut.left_alone_when_the_project_does_not_know_the_module", [] {
    const Bytes raw = make_object(40, {fix(30), fix(12), fix(4)});
    const SymbolTable none;
    CB_CHECK(recut::recut(original(none, {kBase + 12, kBase + 4, kBase + 30}), raw) == raw);
    // a name known as data is no place of the code
    const SymbolTable data{{"_main", SymPlace{SymKind::Data, kBase}}};
    CB_CHECK(recut::recut(original(data, {kBase + 12, kBase + 4, kBase + 30}), raw) == raw);
    // a place of no said kind will do
    const SymbolTable unknown{{"_main", SymPlace{SymKind::Unknown, kBase}}};
    CB_CHECK(recut::recut(original(unknown, {kBase + 12, kBase + 4, kBase + 30}), raw) != raw);
});

CB_TEST("exact.recut.a_segment_without_fixups_is_one_record_still", [] {
    const Bytes raw = make_object(40, {});
    // nothing to show a cut: the segment is written again as one record, which is what it was
    const Bytes out = recut::recut(original(symbols(), {}), raw);
    CB_CHECK(out == raw);
    CB_CHECK_EQ(pieces_of(out).size(), std::size_t{1});
});

CB_TEST("exact.recut.no_record_over_a_kilobyte", [] {
    // one run, 3000 bytes: the records are cut at 1024 and the fix-up at 1500 goes with the one that holds it
    const Bytes raw = make_object(3000, {fix(1500)});
    const Bytes out = recut::recut(original(symbols(), {kBase + 1500}), raw);
    const std::vector<Piece> pieces = pieces_of(out);
    CB_CHECK_EQ(pieces.size(), std::size_t{3});
    Bytes joined;
    for (const Piece& p : pieces) {
        CB_CHECK(p.data.size() <= 1024);
        CB_CHECK_EQ(p.begin, joined.size());
        append(joined, p.data);
    }
    CB_CHECK_EQ(joined.size(), std::size_t{3000});
    CB_CHECK(joined == omf::read(raw).segments.at("_TEXT"));
    CB_CHECK(pieces[0].fixups.empty());
    CB_CHECK((places(pieces[1]) == std::vector<std::uint32_t>{1500}));
    CB_CHECK(pieces[2].fixups.empty());
});

CB_TEST("exact.recut.a_hundred_fixups_to_a_record", [] {
    // 150 words in one record, listed as the program has them (down): two FIXUPP records, 100 and 50
    std::vector<fixupp::Fixup> listed;
    std::vector<std::uint32_t> program;
    for (int k = 149; k >= 0; --k) {
        listed.push_back(fix(static_cast<std::uint32_t>(2 * k)));
        program.push_back(static_cast<std::uint32_t>(kBase + 2 * k));
    }
    const Bytes raw = make_object(400, listed);
    const Bytes out = recut::recut(original(symbols(), program), raw);
    int fixup_records = 0;
    for (const omf::Record& r : omf::records(out)) fixup_records += r.type == 0x9C;
    CB_CHECK_EQ(fixup_records, 2);
    const std::vector<Piece> pieces = pieces_of(out);
    CB_CHECK_EQ(pieces.size(), std::size_t{1});
    CB_CHECK_EQ(pieces[0].fixups.size(), std::size_t{150});
    CB_CHECK_EQ(pieces[0].fixups.front().at, 298u);   // the original's order
    CB_CHECK_EQ(pieces[0].fixups.back().at, 0u);
});

CB_TEST("exact.recut.the_other_fixups_follow_the_relocated_ones", [] {
    // a low-byte fix-up (location 0) at 8 is not relocated: it goes after the relocated ones in its record
    fixupp::Fixup low = fix(8, 0);
    const Bytes raw = make_object(40, {low, fix(30), fix(12), fix(4)});
    const Bytes out = recut::recut(original(symbols(), {kBase + 12, kBase + 4, kBase + 30}), raw);
    const std::vector<Piece> pieces = pieces_of(out);
    CB_CHECK_EQ(pieces.size(), std::size_t{2});
    CB_CHECK((places(pieces[0]) == std::vector<std::uint32_t>{12, 4, 8}));
});

CB_TEST("exact.recut.relocation_order", [] {
    const SymbolTable none;
    // from the EXE header's table: offset word then segment word, address = segment * 16 + offset
    Bytes file(8, 0);
    append(file, u16(0x0C));
    append(file, u16(0x10));
    append(file, u16(0x0E));
    append(file, u16(0x11));
    recut::Original from_table{none, std::nullopt, recut::RelocTable{2, 8}, file};
    CB_CHECK((recut::relocation_order(from_table) == std::vector<std::uint32_t>{0x10C, 0x11E}));
    // EXEPACK's, when the program is packed (the header's is then not the program's)
    recut::Original packed{none, std::vector<std::uint32_t>{5, 4}, recut::RelocTable{2, 8}, file};
    CB_CHECK((recut::relocation_order(packed) == std::vector<std::uint32_t>{5, 4}));
    // no header, no relocations
    CB_CHECK(recut::relocation_order(recut::Original{none, std::nullopt, std::nullopt, file}).empty());
    // an empty table
    CB_CHECK(recut::relocation_order(recut::Original{none, std::nullopt, recut::RelocTable{0, 8}, file}).empty());
    // a table that lies past the file
    CB_CHECK_THROWS_AS(FormatError, recut::relocation_order(recut::Original{none, std::nullopt, recut::RelocTable{3, 8}, file}));
    CB_CHECK_THROWS_AS(FormatError, recut::relocation_order(recut::Original{none, std::nullopt, recut::RelocTable{1, 0xFFFFFFFFu}, file}));
    // the largest address a header can name
    Bytes big;
    append(big, u16(0xFFFF));
    append(big, u16(0xFFFF));
    CB_CHECK((recut::relocation_order(recut::Original{none, std::nullopt, recut::RelocTable{1, 0}, big}) ==
              std::vector<std::uint32_t>{0xFFFFu * 16 + 0xFFFFu}));
});

CB_TEST("exact.recut.damaged_modules", [] {
    const SymbolTable& table = symbols();
    const std::vector<std::uint32_t> program{kBase + 12, kBase + 4, kBase + 30};
    // a fix-up record before any data record
    Bytes early;
    append(early, omf::record(0x80, Bytes{1, 't'}));
    append(early, fixupp::encode(std::vector<fixupp::Fixup>{fix(4)}, 0));
    append(early, omf::record(0x8A, Bytes{0}));
    CB_CHECK_THROWS_AS(FormatError, recut::recut(original(table, program), early));
    // a segment named by an index no LNAMES record gave
    Bytes lost;
    append(lost, omf::record(0x80, Bytes{1, 't'}));
    append(lost, omf::record(0x98, cat({Bytes{0x28}, u16(4), Bytes{9, 3, 1}})));
    append(lost, omf::record(0x8A, Bytes{0}));
    CB_CHECK_THROWS_AS(FormatError, recut::recut(original(table, program), lost));
    // a fix-up that names a thread nobody defined
    Bytes raw = make_object(40, {});
    Bytes thread_user;
    append(thread_user, ByteView(raw).first(raw.size() - 5));   // up to the MODEND (5 bytes)
    append(thread_user, omf::record(0x9C, Bytes{0xC4, 0x04, 0x80, 0x01}));
    append(thread_user, omf::record(0x8A, Bytes{0}));
    CB_CHECK_THROWS_AS(FormatError, recut::recut(original(table, program), thread_user));
    // a record that ends inside a fix-up
    Bytes cut_off;
    append(cut_off, ByteView(raw).first(raw.size() - 5));
    append(cut_off, omf::record(0x9C, Bytes{0xC4, 0x04}));
    append(cut_off, omf::record(0x8A, Bytes{0}));
    CB_CHECK_THROWS_AS(Error, recut::recut(original(table, program), cut_off));
    // a module cut off in its first record has nothing to cut again
    CB_CHECK_NOTHROW(recut::recut(original(table, program), ByteView(raw).first(10)));
});

// The Python module's own answers, on objects the Python helpers built (testdata/recut: <case>.obj, the program's
// relocations in <case>.relocs, what recut.py made of them in <case>.golden.obj). Each golden file was read through
// and checked against the rule (cut as late as the next run's first word allows, relocations in the program's order
// within a record, no data record over 1024); "foreign" and "big" come out as they went in.
CB_TEST("exact.recut.the_python_modules_answers", [] {
    const std::filesystem::path dir = testing::data_path("exact/testdata/recut");
    for (const char* name : {"cut", "pointer", "mixed", "many", "big", "three", "foreign"}) {
        const Bytes raw = fs::read_file(dir / std::format("{}.obj", name));
        const Bytes golden = fs::read_file(dir / std::format("{}.golden.obj", name));
        std::vector<std::uint32_t> relocs;
        const std::string relocs_text = fs::read_text(dir / std::format("{}.relocs", name));   // (the lines point into it)
        for (const std::string_view line : text::lines(relocs_text)) {
            const std::optional<std::uint32_t> address = text::parse_int<std::uint32_t>(line, 16);
            CB_CHECK(address.has_value());
            if (address) relocs.push_back(*address);
        }
        const Bytes got = recut::recut(original(symbols(), relocs), raw);
        const auto diff = std::ranges::mismatch(got, golden);
        CB_CHECK_EQ(std::format("{}{}", name, got == golden ? "" : std::format(" differs from the first difference at {}", diff.in1 - got.begin())),
                    std::string(name));
    }
});

// Real object modules: members of the Microsoft C 5.0 medium and large model libraries (code with pointer and base
// fix-ups, as the real compiler wrote it) and hand-made ones with a fix-up across the 1000-byte cut, each "placed in a
// program" and given the program's relocations as a linker would list them (or damaged ways: ascending, shuffled, one
// missing, an extra word, a duplicate, none), as the packed original's list or as an EXE header and file. The answers
// are the Python module's: every scenario that changes the module, the damaged ones, and a sample of those that leave
// it alone (testdata/recut_real.json; the typed-port harness state/work/typed-port/exact.recut holds all 408).
CB_TEST("exact.recut.real_objects_as_the_python_recut_them", [] {
    const Json doc = json::load(testing::data_path("exact/testdata/recut_real.json"));
    std::vector<Bytes> raws;
    for (const Json& h : doc.at("raws")) raws.push_back(crypto::from_hex(h.get<std::string>()));
    std::size_t changed = 0, unchanged = 0, errors = 0;
    for (const Json& e : doc.at("entries")) {
        const std::string title = e.at("title").get<std::string>();
        const Bytes& raw = raws[e.at("raw").get<std::size_t>()];
        SymbolTable symbols;
        for (const Json& s : e.at("syms")) {
            const std::string kind = s[1].get<std::string>();
            symbols.insert_or_assign(s[0].get<std::string>(),
                                     SymPlace{kind == "C" ? SymKind::Code : kind == "D" ? SymKind::Data : kind == "S" ? SymKind::Segment : SymKind::Unknown,
                                              s[2].get<std::int64_t>()});
        }
        std::optional<std::vector<std::uint32_t>> packed;
        if (!e.at("packed").is_null()) {
            packed = std::vector<std::uint32_t>{};
            for (const Json& r : e.at("packed")) packed->push_back(r.get<std::uint32_t>());
        }
        const Bytes file = crypto::from_hex(e.at("file").get<std::string>());
        std::optional<recut::RelocTable> table;
        bool header_too_short = false;
        if (!e.at("header").is_null()) {
            const Json& h = e.at("header");
            if (h.size() > 12) {
                // (a negative table offset is not one: the Python sliced from the end of the file)
                if (h[12].get<std::int64_t>() < 0) continue;
                table = recut::RelocTable{h[3].get<std::uint32_t>(), h[12].get<std::uint32_t>()};
            } else if (!h.empty()) {
                header_too_short = true;   // the Python read h[12] of it: an IndexError
            }
        }
        const recut::Original original{symbols, packed, table, file};
        // the order of the program's relocations
        const std::string order_want = e.at("order").get<std::string>();
        if (!header_too_short && order_want.starts_with("order=")) {
            std::vector<std::uint32_t> order;
            try {
                order = recut::relocation_order(original);
            } catch (const FormatError&) {
                continue;   // a table that lies outside the file: the Python read what was there (see the DIVERGES block)
            }
            std::string text = "order=";
            if (order.size() < 12) {
                for (const std::uint32_t o : order) text += std::format("{},", o);
                if (text.back() == ',') text.pop_back();
            } else {
                text += std::to_string(order.size());
            }
            CB_CHECK_EQ(title + ": " + text, title + ": " + order_want);
        }
        // what recut makes of the module
        const std::string want = e.at("result").get<std::string>();
        try {
            if (header_too_short) throw FormatError("test", 0, "a header too short to name the table");
            const Bytes got = recut::recut(original, raw);
            std::string text;
            if (got == raw) {
                text = "same";
                ++unchanged;
            } else {
                text = std::format("len={} crc={:08x} sha1={}", got.size(), crypto::crc32(got), crypto::to_hex(crypto::sha1(got)));
                ++changed;
            }
            CB_CHECK_EQ(title + ": " + text, title + ": " + want);
        } catch (const Error&) {
            ++errors;
            CB_CHECK(want.starts_with("EXC"));
        }
    }
    CB_CHECK(changed > 100);
    CB_CHECK(unchanged > 30);
    CB_CHECK(errors >= 1);
});
