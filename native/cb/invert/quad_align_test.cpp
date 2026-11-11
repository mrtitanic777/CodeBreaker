// Tests of cb/invert/quad_align.hpp: the quad file of the oracle program FXU.C, the real Lattice C objects HELLO, CAND
// and PROBE (testdata/), and objects built with the OMF record writer for the 32-bit record forms and damage.
//
// DIVERGES from the Python tool: the 32-bit SEGDEF, LEDATA and PUBDEF records (0x99, 0xa1, 0x91) are read with their
// 32-bit fields where the Python read the 16-bit layout for all of them (it misplaced the segment name and placed the
// data at the wrong offset); data that would reach past 16 MiB of a segment is a FormatError where the Python grew
// a buffer without limit; a quad record promising more payload than the file holds is a TruncatedError where the
// Python clamped it; a damaged OMF record is a FormatError where the Python raised struct.error.
#include "cb/invert/quad_align.hpp"

#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/omf.hpp"

using namespace cb;
using namespace std::literals;
namespace qa = cb::invert::quad_align;
namespace omf = cb::formats::omf;

namespace {

Bytes testdata(const char* name) { return fs::read_file(testing::data_path(std::string("invert/testdata/") + name)); }

Bytes cat(std::initializer_list<Bytes> parts) {
    Bytes out;
    for (const Bytes& p : parts) append(out, p);
    return out;
}

Bytes lnames(std::initializer_list<std::string_view> names) {
    Bytes payload;
    for (const auto n : names) append(payload, omf::name(n));
    return omf::record(0x96, payload);
}

// A 16-bit object with one PROG segment holding `code` and publics {name, offset}.
Bytes object16(ByteView code, const std::vector<std::pair<std::string, int>>& publics) {
    Bytes out = lnames({"PROG", "CODE"});
    out = cat({out, omf::record(0x98, Bytes{0x20, static_cast<std::uint8_t>(code.size()), static_cast<std::uint8_t>(code.size() >> 8), 1, 2, 0})});
    Bytes led = {1, 0, 0};   // segment 1 at offset 0
    append(led, code);
    out = cat({out, omf::record(0xa0, led)});
    Bytes pub = {0, 1};      // no group, segment 1
    for (const auto& [name, off] : publics) {
        append(pub, omf::name(name));
        pub.push_back(static_cast<std::uint8_t>(off & 0xff));
        pub.push_back(static_cast<std::uint8_t>((off >> 8) & 0xff));
        pub.push_back(0);   // type index
    }
    out = cat({out, omf::record(0x90, pub), omf::record(0x8a, Bytes{0})});
    return out;
}

}  // namespace

CB_TEST("invert.quad_align.quads_of_the_real_file", [] {
    const auto fns = qa::parse_quads(testdata("FXU.Q"));
    CB_CHECK_EQ(fns.size(), std::size_t{1});   // FXU.C has one function, main
    CB_CHECK_EQ(fns[0].size(), std::size_t{451});
    CB_CHECK_EQ(fns[0][0].opcode, 0x23);
    CB_CHECK_EQ(fns[0][0].payload.size(), std::size_t{21});
    CB_CHECK_EQ(fns[0].back().opcode, 0x24);
});

CB_TEST("invert.quad_align.quads_split_at_func_begin", [] {
    Bytes f(cb::invert::quad_dump::kFirstRecord, 0);
    auto rec = [&](std::uint8_t op, Bytes payload) {
        f.push_back(op);
        f.push_back(static_cast<std::uint8_t>(payload.size()));
        append(f, payload);
    };
    rec(0x13, {1});            // before the first FUNC-BEGIN: belongs to no function
    rec(0x23, {2});
    rec(0x24, {});
    rec(0x23, {3, 3});
    rec(0x2e, {0x81});         // (a non-printable last byte ends the records)
    f.push_back(0xff);
    f.push_back(0);
    f.push_back(0x80);
    const auto fns = qa::parse_quads(f);
    CB_CHECK_EQ(fns.size(), std::size_t{2});
    CB_CHECK_EQ(fns[0].size(), std::size_t{2});
    CB_CHECK_EQ(fns[0][0].opcode, 0x23);
    CB_CHECK_EQ(fns[0][1].opcode, 0x24);
    CB_CHECK(fns[0][1].payload.empty());
    CB_CHECK_EQ(fns[1].size(), std::size_t{2});
    CB_CHECK(fns[1][0].payload == (Bytes{3, 3}));
});

CB_TEST("invert.quad_align.quads_of_odd_files", [] {
    CB_CHECK(qa::parse_quads(Bytes{}).empty());
    CB_CHECK(qa::parse_quads(Bytes(1, 0x23)).empty());
    CB_CHECK(qa::parse_quads(Bytes(0x5c, 0)).empty());        // a header and nothing else
    Bytes overrun(0x5c, 0);
    overrun.insert(overrun.end(), {0x23, 100, 1, 2, 0x80});   // the head promises 100 bytes
    CB_CHECK_THROWS_AS(TruncatedError, qa::parse_quads(overrun));
});

CB_TEST("invert.quad_align.functions_of_real_objects", [] {
    const auto hello = qa::parse_obj_funcs(testdata("HELLO.OBJ"));
    CB_CHECK_EQ(hello.size(), std::size_t{1});
    CB_CHECK_EQ(hello[0].name, "MAIN");
    CB_CHECK_EQ(hello[0].offset, 0u);
    CB_CHECK_EQ(hello[0].code.size(), std::size_t{14});
    CB_CHECK(hello[0].code.size() > 12 &&
             (Bytes(hello[0].code.begin(), hello[0].code.begin() + 12) ==
              Bytes{0x55, 0x8b, 0xec, 0xb8, 0x00, 0x00, 0x50, 0xe8, 0x00, 0x00, 0x8b, 0xe5}));   // push bp; mov bp,sp; mov ax,0; push ax; call; mov sp,bp

    const auto cand = qa::parse_obj_funcs(testdata("CAND.OBJ"));
    CB_CHECK_EQ(cand.size(), std::size_t{1});
    CB_CHECK_EQ(cand[0].name, "F00000");
    CB_CHECK_EQ(cand[0].code.size(), std::size_t{26});

    // PROBE.OBJ: 19 functions, each running to the next public, in address order
    const auto probe = qa::parse_obj_funcs(testdata("PROBE.OBJ"));
    CB_CHECK_EQ(probe.size(), std::size_t{19});
    CB_CHECK_EQ(probe[0].name, "T01");
    CB_CHECK_EQ(probe[0].code.size(), std::size_t{30});
    CB_CHECK_EQ(probe[4].name, "R09");
    CB_CHECK_EQ(probe[4].offset, 120u);
    CB_CHECK_EQ(probe[4].code.size(), std::size_t{31});
    CB_CHECK_EQ(probe[9].name, "R10");       // by address it follows R14
    CB_CHECK_EQ(probe[9].offset, 278u);
    CB_CHECK_EQ(probe[18].name, "C07");
    CB_CHECK_EQ(probe[18].code.size(), std::size_t{38});
    std::size_t total = 0;
    for (std::size_t i = 0; i < probe.size(); ++i) {
        total += probe[i].code.size();
        if (i) CB_CHECK(probe[i].offset >= probe[i - 1].offset);
        CB_CHECK_EQ(probe[i].code[0], 0x55);   // every one starts push bp
    }
    CB_CHECK_EQ(total, std::size_t{626});   // 588 + 38: the last public's offset plus its length
});

CB_TEST("invert.quad_align.object_with_data_and_equal_offsets", [] {
    const Bytes code = {0x55, 0x8b, 0xec, 0x5d, 0xc3, 0x01, 0x02, 0x03, 0x90, 0x90};
    // two publics at offset 5 (data), one at 0, one past the end of the code
    const Bytes obj = object16(code, {{"data1", 5}, {"fn", 0}, {"data2", 5}, {"far", 40}});
    const auto fns = qa::parse_obj_funcs(obj);
    CB_CHECK_EQ(fns.size(), std::size_t{4});
    CB_CHECK_EQ(fns[0].name, "fn");
    CB_CHECK_EQ(fns[0].code.size(), std::size_t{5});
    CB_CHECK_EQ(fns[1].name, "data1");   // equal offsets keep the order of the records
    CB_CHECK(fns[1].code.empty());       // ... and the first of them runs to the second, which starts at the same place
    CB_CHECK_EQ(fns[2].name, "data2");
    CB_CHECK_EQ(fns[2].code.size(), std::size_t{5});   // 5..10, the next public is past the code
    CB_CHECK_EQ(fns[3].name, "far");
    CB_CHECK_EQ(fns[3].offset, 40u);
    CB_CHECK(fns[3].code.empty());
});

CB_TEST("invert.quad_align.32bit_records", [] {
    // SEGDEF32 (length is 4 bytes), LEDATA32 (4-byte offset), PUBDEF32 (4-byte offset)
    Bytes out = lnames({"PROG", "CODE"});
    out = cat({out, omf::record(0x99, Bytes{0x20, 8, 0, 0, 0, 1, 2, 0})});
    Bytes led = {1, 4, 0, 0, 0};   // segment 1, offset 4
    append(led, Bytes{0x55, 0x8b, 0xec, 0xc3});
    out = cat({out, omf::record(0xa1, led)});
    Bytes pub = {0, 1};
    append(pub, omf::name("fn32"));
    append(pub, Bytes{4, 0, 0, 0, 0});
    out = cat({out, omf::record(0x91, pub), omf::record(0x8a, Bytes{0})});
    const auto fns = qa::parse_obj_funcs(out);
    CB_CHECK_EQ(fns.size(), std::size_t{1});
    CB_CHECK_EQ(fns[0].name, "fn32");
    CB_CHECK_EQ(fns[0].offset, 4u);
    CB_CHECK(fns[0].code == (Bytes{0x55, 0x8b, 0xec, 0xc3}));
});

CB_TEST("invert.quad_align.object_damage", [] {
    CB_CHECK(qa::parse_obj_funcs(Bytes{}).empty());
    Bytes truncated = object16(Bytes{0x55, 0xc3}, {{"f", 0}});
    truncated.resize(truncated.size() - 4);
    CB_CHECK_THROWS_AS(FormatError, qa::parse_obj_funcs(truncated));
    // a segment other than PROG is ignored
    Bytes other = lnames({"DATA"});
    other = cat({other, omf::record(0x98, Bytes{0x20, 2, 0, 1, 1, 0}), omf::record(0xa0, Bytes{1, 0, 0, 0x55, 0xc3}),
                 omf::record(0x90, cat({Bytes{0, 1}, omf::name("d"), Bytes{0, 0, 0}}))});
    const auto fns = qa::parse_obj_funcs(other);
    CB_CHECK_EQ(fns.size(), std::size_t{1});
    CB_CHECK(fns[0].code.empty());   // the public is there, the code of the other segment is not
    // a LEDATA that places bytes absurdly far away
    Bytes huge = lnames({"PROG"});
    huge = cat({huge, omf::record(0x98, Bytes{0x20, 0, 0, 1, 1, 0}), omf::record(0xa1, Bytes{1, 0, 0, 0, 0x40, 1, 2, 3})});
    CB_CHECK_THROWS_AS(FormatError, qa::parse_obj_funcs(huge));
});

CB_TEST("invert.quad_align.align_and_report", [] {
    // an object whose publics are two functions and a data symbol
    const Bytes code = {0x55, 0x8b, 0xec, 0x5d, 0xc3, 0xde, 0xad, 0x8b, 0xe5, 0xc3};
    const Bytes obj = object16(code, {{"f", 0}, {"tbl", 5}, {"g", 7}});
    std::vector<qa::QuadFunction> quads = {{{0x23, {1}}, {0x24, {}}}};   // the quad file has one function
    const auto aligned = qa::align(quads, qa::parse_obj_funcs(obj));
    CB_CHECK_EQ(aligned.size(), std::size_t{2});   // `tbl` starts 0xde: data, left out
    CB_CHECK_EQ(aligned[0].name, "f");
    CB_CHECK_EQ(aligned[0].quads.size(), std::size_t{2});
    CB_CHECK_EQ(aligned[1].name, "g");
    CB_CHECK(aligned[1].quads.empty());            // more functions than quad functions
    CB_CHECK_EQ(qa::format_alignment(1, aligned),
                "quad funcs: 1 | obj funcs: 2\n"
                "\n=== f  (5 bytes) ===\n  quads: 23 24\n  code : 55 8b ec 5d c3\n"
                "\n=== g  (3 bytes) ===\n  quads: \n  code : 8b e5 c3\n");

    // a whole report from the two files' bytes
    Bytes q(0x5c, 0);
    for (const std::uint8_t b : Bytes{0x23, 0, 0x24, 0}) q.push_back(b);
    q.push_back(0xff), q.push_back(0), q.push_back(0x80);
    CB_CHECK(qa::report(q, obj).starts_with("quad funcs: 1 | obj funcs: 2\n"));
});
