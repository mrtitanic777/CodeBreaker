#include "cb/exact/linkemu.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::exact;
using namespace cb::exact::linkemu;

namespace {

Bytes hex(std::string_view h) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < h.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(std::string(h.substr(i, 2)), nullptr, 16)));
    return out;
}

std::string hex_of(ByteView b) {
    std::string out;
    for (std::uint8_t c : b) out += std::format("{:02x}", c);
    return out;
}

// The two objects of the old harness's "a" program (an OMF crt0 and a module of C), as omf.read returns them.
LinkInput crt0(bool odd = false) {
    LinkInput in;
    in.name = "a_crt0.obj";
    in.odd = odd;
    ObjectFile& o = in.object;
    o.segs = {{"_TEXT", "CODE", 48, 2}, {"_DATA", "DATA", 16, 2}, {"_BSS", "BSS", 32, 2}, {"c_common", "BSS", 0, 2}, {"STACK", "STACK", 256, 3}};
    o.groups.insert_or_assign("DGROUP", std::vector<std::string>{"_DATA", "_BSS", "c_common", "STACK"});
    o.seg.insert_or_assign("_TEXT", hex("81a6657b623a95f3794a018f0efe5861c65da9a43c2c85f0687362242966e027336ca82c30ec28368be33eb31cce7e70"));
    o.seg.insert_or_assign("_DATA", hex("f1ce46c406111afd14b7dd3c83fa1cf1"));
    o.seg.insert_or_assign("_BSS", Bytes(32));
    o.seg.insert_or_assign("c_common", Bytes{});
    o.seg.insert_or_assign("STACK", Bytes(256));
    o.filled.insert_or_assign("_TEXT", 48u);
    o.filled.insert_or_assign("_DATA", 16u);
    o.pubs = {{"__start", "_TEXT", 0}, {"__astart", "_TEXT", 8}};
    o.ext = {"_main"};
    o.fix = {{"_TEXT", 3, FixKind::Off, {TargetKind::External, "_main"}, 0},
             {"_TEXT", 9, FixKind::Rel, {TargetKind::External, "_main"}, 0},
             {"_TEXT", 16, FixKind::Base, {TargetKind::Group, "DGROUP"}, 0},
             {"_TEXT", 20, FixKind::Off, {TargetKind::Segment, "_DATA"}, 4}};
    o.start = StartAddress{"_TEXT", 0};
    for (auto [name, how] : {std::pair<const char*, int>{"_TEXT", 2}, {"_DATA", 2}, {"_BSS", 2}, {"c_common", 6}, {"STACK", 5}})
        o.combine.insert_or_assign(name, static_cast<std::uint8_t>(how));
    return in;
}

LinkInput module0(std::string_view name = "a_m0.obj") {
    LinkInput in;
    in.name = std::string(name);
    ObjectFile& o = in.object;
    o.segs = {{"_TEXT", "CODE", 61, 2}, {"_DATA", "DATA", 18, 2}, {"_BSS", "BSS", 16, 2}};
    o.groups.insert_or_assign("DGROUP", std::vector<std::string>{"_DATA", "_BSS"});
    o.seg.insert_or_assign("_TEXT", hex("304b982f08f8b4d4001ff64a0d047a2dee3ac7f334457a856b2c52544c0c1977c2d5fa8934f1d4978d93e86009f924120948042e37682ad95437c491eb"));
    o.seg.insert_or_assign("_DATA", hex("cd63db80912ca5a1638fbcf4c9cd82c19e28"));
    o.seg.insert_or_assign("_BSS", Bytes(16));
    o.filled.insert_or_assign("_TEXT", 61u);
    o.filled.insert_or_assign("_DATA", 18u);
    o.pubs = {{"_main", "_TEXT", 0}, {"_ad0", "_DATA", 0}};
    o.ext = {"_av0_0", "_av0_1"};
    o.comm.insert_or_assign("_av0_0", CommonVar{2, false});
    o.comm.insert_or_assign("_av0_1", CommonVar{4, false});
    o.fix = {{"_TEXT", 10, FixKind::Off, {TargetKind::External, "_av0_0"}, 0},
             {"_TEXT", 12, FixKind::Off, {TargetKind::Segment, "_DATA"}, 0},
             {"_TEXT", 16, FixKind::Off, {TargetKind::Segment, "_TEXT"}, 0},
             {"_DATA", 0, FixKind::Off, {TargetKind::Segment, "_DATA"}, 2},
             {"_DATA", 2, FixKind::Base, {TargetKind::Segment, "_BSS"}, 0}};
    return in;
}

SymbolTable communal_places(std::int64_t first = 0x52) {
    SymbolTable s;
    s.insert_or_assign("_av0_0", SymPlace{SymKind::Data, first});
    s.insert_or_assign("_av0_1", SymPlace{SymKind::Data, first + 2});
    return s;
}

std::uint16_t word(const Bytes& b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | (b[at + 1] << 8)); }

}  // namespace

// The recorded result of the Python linker on these two objects (exact.linkemu harness, first case), whose header and
// fix-ups were checked word by word: 658 bytes, DGROUP at 0x70, relocations (0x10, seg 0) and (0x12, seg 7), the
// stack at paragraph 0xd with 0x100 bytes, the checksum making the words add up to 0xFFFF.
CB_TEST("exact.linkemu.two_modules_of_a_small_model_program", [] {
    std::vector<std::string> log;
    const LinkResult r = link24({crt0(), module0()}, communal_places(), [&](std::string_view s) { log.emplace_back(s); });
    CB_CHECK_EQ(r.exe.size(), 658u);
    CB_CHECK_EQ(hex_of(ByteView(r.exe).first(42)), "4d5a92000200020020001400ffff0d000001ff13000000001e0000000100100000001200070000000000");
    CB_CHECK_EQ(hex_of(ByteView(r.exe).subspan(512)),
                "81a665ab623a95f3796f018f0efe58610700a9a4402c85f0687362242966e027336ca82c30ec28368be33eb31cce7e70304b982f08f8b4d4001f484b1d047a2d1e3bc7f33"
                "4457a856b2c52544c0c1977c2d5fa8934f1d4978d93e86009f924120948042e37682ad95437c491eb000000f1ce46c406111afd14b7dd3c83fa1cf1df630700912ca5a1638f"
                "bcf4c9cd82c19e28");
    // the words of the file add up to FFFF
    std::uint32_t sum = 0;
    for (std::size_t i = 0; i < r.exe.size(); i += 2) sum += word(r.exe, i);
    CB_CHECK_EQ(sum & 0xFFFF, 0xFFFFu);
    CB_CHECK_EQ(r.dgroup, 0x70u);
    // a fix-up by hand: crt0's text word at 3 is an offset of _main (module 0's text begins at 48)
    CB_CHECK_EQ(word(r.exe, 512 + 3), static_cast<std::uint16_t>(0x627B + 48));
    CB_CHECK_EQ(log.size(), 1u);
    CB_CHECK_EQ(log[0], "  the text ends at 0006d, DGROUP is at 00070; the image is 0092 bytes, the program ends at 001d0");
    // where each module's pieces went
    CB_CHECK_EQ(r.modules.size(), 2u);
    const auto at = [&](std::size_t module, std::string_view seg) {
        for (const PlacedPiece& p : r.modules[module].pieces)
            if (p.segment == seg) return static_cast<std::int64_t>(p.at);
        return std::int64_t{-1};
    };
    CB_CHECK_EQ(at(0, "_TEXT"), 0);
    CB_CHECK_EQ(at(0, "_DATA"), 112);
    CB_CHECK_EQ(at(0, "STACK"), 208);
    CB_CHECK_EQ(at(1, "_TEXT"), 48);
    CB_CHECK_EQ(at(1, "_DATA"), 128);
    CB_CHECK_EQ(at(1, "_BSS"), 178);
});

CB_TEST("exact.linkemu.communal_variables_follow_symbols", [] {
    std::vector<std::string> log;
    // SYMBOLS has the first variable somewhere else: the linker places them from where the previous ended, and says so
    link24({crt0(), module0()}, communal_places(0x60), [&](std::string_view s) { log.emplace_back(s); });
    CB_CHECK_EQ(log.size(), 3u);
    CB_CHECK_EQ(log[0], "  _av0_0: SYMBOLS has it at 0060; the variables before it end at 0052");
    CB_CHECK_EQ(log[1], "  _av0_1: SYMBOLS has it at 0062; the variables before it end at 0054");
});

CB_TEST("exact.linkemu.a_module_whose_code_ends_on_an_odd_byte", [] {
    // the compiler's filler is not there: the module's text is a byte shorter and the one after it is byte aligned
    LinkInput bytewise = module0();
    bytewise.object.segs[0].align = 1;   // (an assembler's byte-aligned segment: it can follow an odd end)
    const LinkResult even = link24({crt0(), bytewise}, communal_places());
    const LinkResult odd = link24({crt0(true), bytewise}, communal_places());
    CB_CHECK_EQ(even.modules[1].pieces[0].at, 48u);
    CB_CHECK_EQ(odd.modules[1].pieces[0].at, 47u);
    CB_CHECK(odd.exe != even.exe);
    // a word-aligned module after it is moved on to the even place
    CB_CHECK_EQ(link24({crt0(true), module0()}, communal_places()).modules[1].pieces[0].at, 48u);
});

CB_TEST("exact.linkemu.the_link_itself_can_fail", [] {
    // _main defined twice
    CB_CHECK_THROWS_AS(ToolError, (void)link24({crt0(), module0("m0.obj"), module0("m1.obj")}, communal_places()));
    // nothing defines _main
    CB_CHECK_THROWS_AS(ToolError, (void)link24({crt0()}, SymbolTable{}));
    // a communal variable SYMBOLS has no place for
    CB_CHECK_THROWS_AS(ToolError, (void)link24({crt0(), module0()}, SymbolTable{}));
    try {
        (void)link24({crt0(), module0()}, SymbolTable{});
    } catch (const cb::ToolError& e) {
        CB_CHECK_EQ(e.tool(), "link24");
        CB_CHECK_EQ(e.output(), "communal variables SYMBOLS has no place for: _av0_0 _av0_1");
    }
    // no segment holds data
    LinkInput empty;
    empty.name = "empty.obj";
    empty.object.segs = {{"_TEXT", "CODE", 0, 2}};
    empty.object.seg.insert_or_assign("_TEXT", Bytes{});
    CB_CHECK_THROWS_AS(ToolError, (void)link24({empty}, SymbolTable{}));
});
