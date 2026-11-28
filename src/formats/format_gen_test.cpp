// Tests of the generated readers (formats/gen: format_gen's output, docs/FORMAT_CODEGEN.md): each must say what the engine
// says reading the same description, on the files here and on cut and damaged copies of them (the try rules, the error
// classes); and the typed members of a few readers on small images (a name resolved from either encloser, a part, a
// variable, a field of either type).
#include "formats/format_gen_check.hpp"

#include <filesystem>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/format_builtin.hpp"
#include "formats/gen/cab.hpp"
#include "formats/gen/mz.hpp"
#include "formats/gen/ps3.hpp"
#include "formats/gen/xur.hpp"
#include "formats/gen/xzp.hpp"

using namespace cb;
namespace eng = cb::formats::engine;
namespace gen = cb::formats::gen;

namespace {

struct GenW {
    Bytes b;
    bool big = false;
    GenW& u8(std::uint64_t v) { return b.push_back(static_cast<std::uint8_t>(v)), *this; }
    GenW& n(std::uint64_t v, unsigned width) {
        for (unsigned k = 0; k < width; ++k) u8(v >> (8 * (big ? width - 1 - k : k)));
        return *this;
    }
    GenW& u16(std::uint64_t v) { return n(v, 2); }
    GenW& u32(std::uint64_t v) { return n(v, 4); }
    GenW& u64(std::uint64_t v) { return n(v, 8); }
    GenW& str(std::string_view s) { return append(b, as_bytes(s)), *this; }
    GenW& pad_to(std::size_t size) { return b.resize(std::max(b.size(), size), 0), *this; }
};

// The generated readers and the engine on `data` and on copies of it cut short and damaged.
std::size_t gen_expect_agreement(const eng::Registry& reg, ByteView data, const std::string& what) {
    std::vector<Bytes> inputs{Bytes(data.begin(), data.end())};
    for (std::size_t cut : {data.size() / 3, data.size() / 2, data.size() * 2 / 3, data.size() - std::min<std::size_t>(data.size(), 1)})
        inputs.emplace_back(data.begin(), data.begin() + static_cast<std::ptrdiff_t>(cut));
    for (std::size_t at : {std::size_t{4}, std::size_t{0x10}, std::size_t{0x18}, std::size_t{0x3C}, data.size() / 2}) {
        if (at >= data.size()) continue;
        Bytes damaged(data.begin(), data.end());
        damaged[at] ^= 0xFF;
        if (at + 1 < damaged.size()) damaged[at + 1] ^= 0x7F;
        inputs.push_back(std::move(damaged));
    }
    std::size_t compared = 0;
    for (const Bytes& in : inputs)
        for (const eng::GenComparison& c : eng::compare_generated(reg, in)) {
            ++compared;
            if (!c.agree) ::cb::testing::record_failure(__FILE__, __LINE__, std::format("{} ({} bytes): {}: engine {}, generated {} {}", what, in.size(), c.format, c.engine, c.generated, c.difference));
        }
    return compared;
}

}  // namespace

CB_TEST("formats.format_gen.agrees_with_engine", [] {
    const eng::Registry& reg = eng::builtin();
    std::size_t compared = 0, files = 0;
    for (const char* dir : {"formats/testdata", "formats/xbox/testdata"}) {
        std::error_code ec;
        for (std::filesystem::recursive_directory_iterator it(testing::data_path(dir), ec), end; !ec && it != end; it.increment(ec)) {
            if (!it->is_regular_file(ec) || it->file_size(ec) > (std::uintmax_t{4} << 20)) continue;
            const Bytes data = fs::read_file(it->path());
            compared += gen_expect_agreement(reg, data, fs::to_text(it->path().filename()));
            ++files;
        }
    }
    CB_CHECK(files > 20);
    CB_CHECK(compared > 1000);
});

CB_TEST("formats.format_gen.typed_members", [] {
    const eng::Registry& reg = eng::builtin();
    // xur: big-endian numbers, an array of structs, a second root struct read at an offset
    GenW x;
    x.big = true;
    x.u32(0x58554942).u32(5).u32(0).u16(0x0102).u32(0x40).u16(1).u32(0x5354524E).u32(0x20).u32(10);
    x.pad_to(0x20).u16(2).u16('h').u16('i').u16(1).u16('!');
    const gen::xur::File scene = gen::xur::read(x.b);
    CB_CHECK_EQ(scene.version, 5u);
    CB_CHECK_EQ(scene.tool_version, 0x0102u);
    CB_CHECK_EQ(scene.sections.size(), 1u);
    CB_CHECK_EQ(scene.sections[0].offset, 0x20u);
    const gen::xur::Strings strings = gen::xur::read_Strings(ByteView(x.b).subspan(0x20, 10));
    CB_CHECK_EQ(strings.strings.size(), 2u);
    CB_CHECK_EQ(strings.strings[1].units, 1u);
    CB_CHECK_THROWS_AS(TruncatedError, gen::xur::read(ByteView(x.b).first(10)));
    Bytes wrong = x.b;
    wrong[0] = 'Y';
    try {
        (void)gen::xur::read(wrong);
        ::cb::testing::record_failure(__FILE__, __LINE__, "a wrong magic reads");
    } catch (const TruncatedError&) {
        ::cb::testing::record_failure(__FILE__, __LINE__, "a wrong magic is not a file cut short");
    } catch (const FormatError&) {
    }
    gen_expect_agreement(reg, x.b, "xur");

    // mz: a part (the relocation table), read only when asked, tolerantly (a table the file cuts holds what is there)
    GenW m;
    m.str("MZ").u16(0x90).u16(3).u16(3).u16(4).u16(0).u16(0xFFFF).u16(0).u16(0xB8).u16(0).u16(0).u16(0).u16(0x40).u16(0);
    m.pad_to(0x40).u16(0x10).u16(0).u16(0x22).u16(1).u16(0x30);
    CB_CHECK(!gen::mz::read(m.b).header.relocs.has_value());
    gen::mz::Options parts;
    parts.parts.dos_relocs = true;
    const gen::mz::MzHeader h = gen::mz::read(m.b, parts).header;
    CB_CHECK_EQ(h.relocs->size(), 2u);   // (the third is cut short)
    CB_CHECK_EQ((*h.relocs)[1].offset, 0x22u);
    CB_CHECK_EQ(h.header_size, 0x40);
    CB_CHECK_EQ(h.image_size, 3 * 512 - (512 - 0x90));
    gen_expect_agreement(reg, m.b, "mz");

    // xzp: a name of two types (a v3 table or a v1 table) and a let of the encloser read by the entries
    GenW z;
    z.big = true;
    z.str("XUIZ").u32(1).u32(0x40).pad_to(0x10).u32(0x1A).pad_to(0x17);
    z.n(4, 3).u32(0).u8(2).u8(0).u8('a').u8(0).u8('b').u8(0);
    z.pad_to(0x30).str("DATA");
    const gen::xzp::File pack = gen::xzp::read(z.b);
    CB_CHECK(!pack.entries_V3Entry.has_value());
    CB_CHECK_EQ(pack.entries_V1Entry->size(), 2u);   // (and an empty one that ends at the payload base)
    CB_CHECK_EQ((*pack.entries_V1Entry)[0].name.size(), 4u);
    CB_CHECK_EQ((*pack.entries_V1Entry)[0].name_end, 0x17 + 9 + 4);
    gen_expect_agreement(reg, z.b, "xzp");

    // ps3: elf.fmt's Header reads is64 from the struct that reads it (a SELF or a plain ELF)
    GenW e;
    e.big = true;
    e.str("\x7f" "ELF").u8(2).u8(2).u8(1).u8(0).u8(0).pad_to(16);
    e.u16(2).u16(21).u32(1).u64(0x10200).u64(0x40).u64(0).u32(0).u16(64).u16(56).u16(1).u16(64).u16(0).u16(0);
    e.u32(1).u32(5).u64(0).u64(0x10000).u64(0x10000).u64(0x78).u64(0x78).u64(0x10000);
    const gen::ps3::File p = gen::ps3::read(e.b);
    CB_CHECK(p.elf.has_value() && !p.self_.has_value());
    CB_CHECK_EQ(p.elf->header.entry, 0x10200u);
    CB_CHECK_EQ(p.elf->segments->size(), 1u);
    CB_CHECK_EQ((*p.elf->segments)[0].filesz, 0x78u);
    gen_expect_agreement(reg, e.b, "ps3");

    // cab: a cstr with its terminator, a field inside an `if` (optional)
    GenW c;
    c.str("MSCF").u32(0).u32(0x60).u32(0).u32(0x2C).u32(0).u8(3).u8(1).u16(1).u16(1).u16(0).u16(7).u16(0);
    c.u32(0x40).u16(1).u16(1);
    c.u32(5).u32(0).u16(0).u16(0).u16(0).u16(0x20).str("a.txt").u8(0);
    c.pad_to(0x60);
    const gen::cab::File cab = gen::cab::read(c.b);
    CB_CHECK(!cab.header_reserve.has_value());
    CB_CHECK_EQ(cab.files.size(), 1u);
    CB_CHECK_EQ(cab.files[0].name, std::string_view("a.txt", 6));
    CB_CHECK_EQ(cab.index_, 0u);
    gen_expect_agreement(reg, c.b, "cab");
});
