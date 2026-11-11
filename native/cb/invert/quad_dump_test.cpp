// Tests of cb/invert/quad_dump.hpp on a real Lattice C 2.12 quad file (testdata/FXU.Q, the compiler's output for the
// oracle program FXU.C) and on small constructed files for the framing rules and damaged input.
//
// DIVERGES from the Python tool: a record whose head promises more payload than the file holds is reported (and the
// walk stops) where the Python clamped the payload to the end of the file and went on; a file shorter than the 0x50
// bytes of header fields is a TruncatedError before anything is printed, where the Python printed its first line and
// then raised struct.error. The name list is printed with double quotes, not Python's list repr.
#include "cb/invert/quad_dump.hpp"

#include <array>
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace std::literals;
namespace qd = cb::invert::quad_dump;

namespace {

Bytes testdata(const char* name) { return fs::read_file(testing::data_path(std::string("invert/testdata/") + name)); }

struct Rec {
    std::uint8_t op;
    Bytes payload;
};

// A quad file: 0x5c bytes of header (the magic, the type-size table and the symbol count filled in), the records, an
// optional 0xff terminator with `trailer` bytes, then the name pool.
Bytes make_file(const std::vector<Rec>& records, bool terminator, std::string_view pool, std::size_t trailer = 2) {
    Bytes f(qd::kFirstRecord, 0);
    f[4] = 2, f[6] = 1, f[0x4e] = 7;
    for (const Rec& r : records) {
        f.push_back(r.op);
        f.push_back(static_cast<std::uint8_t>(r.payload.size()));
        f.insert(f.end(), r.payload.begin(), r.payload.end());
    }
    if (terminator) {
        f.push_back(0xff);
        f.push_back(3);
        f.insert(f.end(), trailer, std::uint8_t{0x80});   // symbol trailer bytes: not printable, so not the pool
    }
    f.insert(f.end(), pool.begin(), pool.end());
    return f;
}

}  // namespace

CB_TEST("invert.quad_dump.real_file_header", [] {
    const Bytes q = testdata("FXU.Q");
    CB_CHECK_EQ(q.size(), std::size_t{10522});
    const qd::File f = qd::parse(q);
    CB_CHECK_EQ(f.magic, 0);
    const std::array<std::uint16_t, 10> sizes = {2, 1, 2, 1, 2, 1, 2, 4, 4, 8};   // char, int, long, float, double ...
    CB_CHECK(f.type_sizes == sizes);
    CB_CHECK_EQ(f.symbol_count, 30);
    CB_CHECK_EQ(f.scan.pool_offset, std::size_t{0x289e});
    const std::vector<std::string> names = {"main",  "argc",   "argv",   "c",      "brace",  "cnest",  "nc",     "i",
                                            "ns",    "copy",   "inlit",  "delim",  "pc",     "sfp",    "symbol", "text",
                                            "fputs", "_iob",   "exit",   "fopen",  "_filbf", "_flsbf", "_ctype", "strcmp"};
    CB_CHECK(f.names == names);
    CB_CHECK_EQ(f.scan.records.size(), std::size_t{451});
    CB_CHECK(f.scan.terminator.has_value());
    CB_CHECK_EQ(f.scan.terminator->offset, std::size_t{0x272e});
    CB_CHECK_EQ(f.scan.terminator->length, 3);
    CB_CHECK(!f.scan.overrun);
    CB_CHECK_EQ(f.scan.records.front().opcode, qd::kFuncBegin);
    CB_CHECK_EQ(f.scan.records.front().offset, std::size_t{0x5c});
    CB_CHECK_EQ(f.scan.records.front().payload.size(), std::size_t{21});
    CB_CHECK_EQ(f.scan.records.back().opcode, 0x24);   // RETURN
});

CB_TEST("invert.quad_dump.real_file_dump", [] {
    const Bytes q = testdata("FXU.Q");
    const std::string text = qd::dump(qd::parse(q), "FXU.Q");
    const std::vector<std::string_view> head = cb::text::lines(text);
    CB_CHECK_EQ(std::string(head[0]), "== FXU.Q (10522 bytes) ==");
    CB_CHECK_EQ(std::string(head[1]), "0x00 magic=0  0x04 type-size-tbl=[2, 1, 2, 1, 2, 1, 2, 4, 4, 8]");
    CB_CHECK(std::string(head[2]).starts_with("0x4e symbolcount=30  name-pool@0x289e=[\"main\", \"argc\", \"argv\", \"c\""));
    // From the blank line before the record heading on, the text is what the Python tool printed for the same file.
    const std::size_t at = text.find("\n-- quad records");
    CB_CHECK(at != std::string::npos);
    CB_CHECK_EQ(text.substr(at), fs::read_text(testing::data_path("invert/testdata/FXU.records.golden.txt")));
});

CB_TEST("invert.quad_dump.framing", [] {
    const Bytes f = make_file({{0x23, {1, 2}}, {0x13, {}}, {0x99, {0xab}}}, true, "\0main\0x\0"sv);
    const qd::File p = qd::parse(f);
    CB_CHECK_EQ(p.scan.records.size(), std::size_t{3});
    CB_CHECK_EQ(p.scan.records[1].offset, std::size_t{0x5c + 4});
    CB_CHECK(p.scan.records[1].payload.empty());
    CB_CHECK(p.scan.terminator.has_value());
    CB_CHECK_EQ(p.scan.terminator->offset, std::size_t{0x5c + 4 + 2 + 3});
    CB_CHECK_EQ(p.magic, 0);
    CB_CHECK_EQ(p.type_sizes[0], 2);
    CB_CHECK_EQ(p.type_sizes[1], 1);
    CB_CHECK_EQ(p.symbol_count, 7);
    CB_CHECK_EQ(p.names.size(), std::size_t{2});   // empty names between NULs are dropped
    CB_CHECK_EQ(p.names[0], "main");
    CB_CHECK_EQ(p.names[1], "x");
    const std::string text = qd::dump(p, "t.q");
    CB_CHECK(text.find("  0x05c: FUNC-BEGIN  ( 2) : 01 02\n") != std::string::npos);
    CB_CHECK(text.find("  0x060: add         ( 0) : \n") != std::string::npos);
    CB_CHECK(text.find("  0x062: op99        ( 1) : ab\n") != std::string::npos);   // an opcode the map does not know
    CB_CHECK(text.find("0xff TERMINATOR (len=3); ") != std::string::npos);
});

CB_TEST("invert.quad_dump.no_terminator_and_overrun", [] {
    // records run into the name pool without a terminator
    const Bytes a = make_file({{0x23, {1}}, {0x84, {0x90}}}, false, "name\0"sv);
    const qd::File pa = qd::parse(a);
    CB_CHECK(!pa.scan.terminator.has_value());
    CB_CHECK(!pa.scan.overrun.has_value());
    CB_CHECK_EQ(pa.scan.records.size(), std::size_t{2});
    CB_CHECK_EQ(pa.scan.end_offset, std::size_t{0x5c + 6});
    CB_CHECK(qd::dump(pa, "a.q").find("  (reached 0x62; name pool at 0x62)\n") != std::string::npos);
    // The name pool is found by its bytes, so records whose last bytes are printable are taken for pool (the Python
    // tool did the same): here the RETURN record's head and the pool's first NUL are all that is printable.
    const qd::File swallowed = qd::parse(make_file({{0x23, {1}}, {0x24, {}}}, false, "name\0"sv));
    CB_CHECK_EQ(swallowed.scan.records.size(), std::size_t{1});
    CB_CHECK_EQ(swallowed.scan.pool_offset, std::size_t{0x5c + 3});

    // a record that promises 200 payload bytes in a file that has 3 more
    Bytes b = make_file({{0x23, {1}}}, false, ""sv);
    b.insert(b.end(), {0x13, 200, 1, 2, 3});
    b.push_back(0x80);   // not printable: the pool is empty and the record is "inside" the walkable area
    const qd::File pb = qd::parse(b);
    CB_CHECK_EQ(pb.scan.records.size(), std::size_t{1});
    CB_CHECK(pb.scan.overrun.has_value());
    CB_CHECK_EQ(*pb.scan.overrun, std::size_t{0x5c + 3});
    CB_CHECK(qd::dump(pb, "b.q").find("a record runs past the end of the file") != std::string::npos);
});

CB_TEST("invert.quad_dump.short_and_odd_files", [] {
    CB_CHECK_THROWS_AS(TruncatedError, qd::parse(Bytes{}));
    CB_CHECK_THROWS_AS(TruncatedError, qd::parse(Bytes(0x4f, 0)));
    const qd::File exact = qd::parse(Bytes(0x50, 0));   // just the header fields: no records, all of it is pool
    CB_CHECK(exact.scan.records.empty());
    CB_CHECK_EQ(exact.scan.pool_offset, std::size_t{0});
    // scan never throws, whatever the size
    CB_CHECK(qd::scan(Bytes{}).records.empty());
    CB_CHECK(qd::scan(Bytes(1, 0x80)).records.empty());
    CB_CHECK(qd::scan(Bytes(qd::kFirstRecord, 0x80)).records.empty());   // the pool is empty, the records start at the end
    // a terminator first
    const qd::File t = qd::parse(make_file({}, true, ""sv));
    CB_CHECK(t.scan.records.empty());
    CB_CHECK(t.scan.terminator.has_value());
});

CB_TEST("invert.quad_dump.opcode_map_and_usage", [] {
    CB_CHECK_EQ(std::string(qd::opcode_name(0x0a)), "neg");
    CB_CHECK_EQ(std::string(qd::opcode_name(0x1f)), "or");
    CB_CHECK_EQ(std::string(qd::opcode_name(0x3f)), "LABEL(named)");
    CB_CHECK_EQ(std::string(qd::opcode_name(0x40)), "GOTO");
    CB_CHECK(qd::opcode_name(0x21).empty());
    CB_CHECK_EQ(qd::opcode_label(0x21), "op21");
    CB_CHECK_EQ(qd::opcode_label(0x23), "FUNC-BEGIN");
    CB_CHECK_EQ(qd::format_payload(Bytes{0, 0xab, 0x7f}), "00 ab 7f");
    CB_CHECK_EQ(qd::format_payload(Bytes{}), "");
    const std::string u = qd::usage();
    CB_CHECK(u.starts_with("quad_dump - structural parser for Lattice C 2.12 quad files"));
    CB_CHECK(u.ends_with("0x2e=LABEL, 0x2f=LOOP, 0x3f=LABEL(named), 0x40=GOTO"));
    CB_CHECK(u.find("  0x0a=neg, 0x0b=lnot, 0x0c=compl, 0x10=mul") != std::string::npos);
});
