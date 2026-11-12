#include "cb/recompile/discover.hpp"

#include <map>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
namespace dc = cb::recompile::discover;

namespace {

constexpr std::uint32_t kSliceBase = 0x822B6E00;   // where the 16 KB of Minecraft code in ppc_slice.bin loads
constexpr std::uint32_t kDataBase = 0x82900000;    // the made data section (discover_data.bin)

// What testdata/gen_discover_golden.py wrote: the image's facts, then the reference's answers as "[name]" sections.
struct DiscoverFacts {
    std::uint32_t entry = 0;
    dc::Functions extents;
    std::set<std::uint32_t> imports;
    std::map<std::string, std::vector<std::string>> sections;   // lookup by name only
};

DiscoverFacts read_discover_facts() {
    DiscoverFacts facts;
    std::vector<std::string>* current = nullptr;
    const std::string body = fs::read_text(testing::data_path("recompile/testdata/discover.golden.txt"));
    for (const std::string_view line : text::lines(body)) {
        if (line.empty() || line.starts_with('#')) continue;
        const auto words = text::split_ws(line);
        if (line.starts_with('[')) {
            current = &facts.sections[std::string(line.substr(1, line.size() - 2))];
        } else if (words[0] == "entry") {
            facts.entry = *text::parse_int<std::uint32_t>(words[1], 16);
        } else if (words[0] == "extent") {
            facts.extents[*text::parse_int<std::uint32_t>(words[1], 16)] = *text::parse_int<std::uint32_t>(words[2], 16);
        } else if (words[0] == "import") {
            facts.imports.insert(*text::parse_int<std::uint32_t>(words[1], 16));
        } else if (current) {
            current->emplace_back(line);
        }
    }
    return facts;
}

// The Xenon's vector words of the slice, as the reference decoded them (ppc_slice.listing.txt); Capstone reads the rest.
recompile::disassemble::VectorDecoder slice_vector_decoder() {
    std::map<std::uint32_t, std::pair<std::string, std::string>> table;   // lookup only
    const std::string body = fs::read_text(testing::data_path("recompile/testdata/ppc_slice.listing.txt"));
    for (const std::string_view line : text::lines(body)) {
        if (line.empty()) continue;
        const auto parts = text::split_n(line, ' ', 3);
        if (parts[2] != "V") continue;
        const std::size_t bar = parts[3].find('|');
        table[*text::parse_int<std::uint32_t>(parts[1], 16)] = {std::string(parts[3].substr(0, bar)), std::string(parts[3].substr(bar + 1))};
    }
    return [table](std::uint32_t word) -> std::optional<std::pair<std::string, std::string>> {
        const auto it = table.find(word);
        if (it == table.end()) return std::nullopt;
        return it->second;
    };
}

// The image the golden file describes: the real code, and a data section of pointers into it.
dc::Image slice_image(const DiscoverFacts& facts) {
    dc::Image image;
    image.code = fs::read_file(testing::data_path("recompile/testdata/ppc_slice.bin"));
    image.code_begin = kSliceBase;
    image.code_end = kSliceBase + static_cast<std::uint32_t>(image.code.size());
    image.entry = facts.entry;
    image.extents = facts.extents;
    image.imports = facts.imports;
    image.sections.push_back({".text", kSliceBase, 0x60000020, image.code});
    image.sections.push_back({".rdata", kDataBase, 0x40000040, fs::read_file(testing::data_path("recompile/testdata/discover_data.bin"))});
    return image;
}

std::vector<std::string> hex_lines(const std::vector<std::uint32_t>& addresses) {
    std::vector<std::string> out;
    for (const std::uint32_t a : addresses) out.push_back(std::format("{:08X}", a));
    return out;
}

// "ADDR SIZE" lines and the sources line of a Discovery, as the golden file writes them.
std::vector<std::string> discovery_lines(const dc::Discovery& found) {
    const dc::Sources& s = found.sources;
    std::vector<std::string> out = {std::format("sources pdata={} entry={} calls={} pointers={} constants={} seed={}", s.pdata, s.entry, s.calls,
                                                s.pointers, s.constants, s.seed)};
    for (const auto& [address, size] : found.functions) out.push_back(std::format("{:08X} {}", address, size));
    return out;
}

}  // namespace

CB_TEST("recompile.discover.pointers_in_data_sections", [] {
    const DiscoverFacts facts = read_discover_facts();
    const dc::Image image = slice_image(facts);
    recompile::disassemble::Disassembler dis(slice_vector_decoder());
    const std::vector<std::uint32_t> found = dc::from_pointers(image, dis, facts.extents);
    CB_CHECK(hex_lines(found) == facts.sections.at("from_pointers"));
    CB_CHECK_EQ(found.size(), 28u);
    // the refusals of the made section: a pointer is found once, and never one inside a known function, outside the
    // code, unaligned, or already a function's start
    for (const std::uint32_t refused : {0u, 0xFFFFFFFFu, kSliceBase + 1, 0x82000000u, 0x822B6EA8u + 0x10, 0x822B7000u + 4, 0x822B6EA8u, 0x822B7000u}) {
        CB_CHECK(std::find(found.begin(), found.end(), refused) == found.end());
    }
});

CB_TEST("recompile.discover.constants_built_by_lis_and_addi_or_ori", [] {
    const DiscoverFacts facts = read_discover_facts();
    const dc::Image image = slice_image(facts);
    recompile::disassemble::Disassembler dis(slice_vector_decoder());
    CB_CHECK(hex_lines(dc::from_constants(image, dis, facts.extents)) == facts.sections.at("from_constants"));
});

CB_TEST("recompile.discover.a_functions_first_instruction", [] {
    const DiscoverFacts facts = read_discover_facts();
    const dc::Image image = slice_image(facts);
    recompile::disassemble::Disassembler dis(slice_vector_decoder());
    std::size_t plausible = 0;
    for (const std::string& line : facts.sections.at("plausible")) {
        const auto words = text::split_ws(line);
        const std::uint32_t address = *text::parse_int<std::uint32_t>(words[0], 16);
        CB_CHECK_EQ(dc::plausible_start(dis, image, address), words[1] == "1");
        if (words[1] == "1") ++plausible;
    }
    CB_CHECK(plausible > 50 && plausible < 400);   // (real code: prologues, and the words after each blr)
    // outside the code there is nothing to read
    CB_CHECK(!dc::plausible_start(dis, image, kSliceBase - 4));
    CB_CHECK(!dc::plausible_start(dis, image, image.code_end));
});

CB_TEST("recompile.discover.every_function_with_and_without_the_scan_and_seeds", [] {
    const DiscoverFacts facts = read_discover_facts();
    const dc::Image image = slice_image(facts);
    recompile::disassemble::Disassembler dis(slice_vector_decoder());
    const std::vector<std::uint32_t> seeds = {0x822B7800, 0x822B9E00, 0x10000000, 0x822B7000};   // (the generator's SEEDS)

    CB_CHECK(discovery_lines(dc::discover(image, dis)) == facts.sections.at("discover_default"));
    CB_CHECK(discovery_lines(dc::discover(image, dis, {.pointer_scan = true, .seeds = seeds})) == facts.sections.at("discover_seeds"));
    CB_CHECK(discovery_lines(dc::discover(image, dis, {.pointer_scan = false})) == facts.sections.at("discover_no_scan"));
    CB_CHECK(discovery_lines(dc::discover(image, dis, {.pointer_scan = false, .seeds = seeds})) == facts.sections.at("discover_no_scan_seeds"));

    // the known sizes survive, and what was found by calls follows the bl targets inside the code
    const dc::Discovery found = dc::discover(image, dis, {.pointer_scan = false});
    CB_CHECK_EQ(found.functions.at(0x822B6EA8), 0x100u);
    CB_CHECK_EQ(found.sources.pdata, 2u);
    CB_CHECK(found.sources.calls > 0);
    for (const auto& [address, size] : found.functions) {
        CB_CHECK(address >= image.code_begin && address < image.code_end);
        (void)size;
    }
    // an import is never a function, even when a bl reaches it
    CB_CHECK(!found.functions.contains(0x822B7400));
});

CB_TEST("recompile.discover.the_entry_point_is_a_function", [] {
    dc::Image image;
    image.code_begin = 0x82000000;
    image.code_end = 0x82000010;
    image.code = {0x4E, 0x80, 0x00, 0x20, 0x4E, 0x80, 0x00, 0x20, 0x4E, 0x80, 0x00, 0x20, 0x4E, 0x80, 0x00, 0x20};   // four blr
    image.entry = 0x82000008;
    recompile::disassemble::Disassembler dis;
    const dc::Discovery found = dc::discover(image, dis, {.pointer_scan = false});
    CB_CHECK_EQ(found.functions.size(), 1u);
    CB_CHECK_EQ(found.functions.at(0x82000008), 0u);
    CB_CHECK_EQ(found.sources.entry, 1u);
    // no entry, no extents, no seeds: nothing
    image.entry = 0;
    CB_CHECK(dc::discover(image, dis, {.pointer_scan = false}).functions.empty());
});

CB_TEST("recompile.discover.a_bl_chain_is_followed_to_its_end", [] {
    // 0x82000000: bl +8 ; blr ; (0x82000008) bl +8 ; blr ; (0x82000010) blr
    const auto bl = [](std::int32_t offset) { return 0x48000001u | (static_cast<std::uint32_t>(offset) & 0x03FFFFFC); };
    constexpr std::uint32_t kBlr = 0x4E800020;
    const std::uint32_t words[] = {bl(8), kBlr, bl(8), kBlr, kBlr};
    dc::Image image;
    image.code_begin = 0x82000000;
    image.code_end = 0x82000014;
    for (const std::uint32_t w : words) {
        for (int shift = 24; shift >= 0; shift -= 8) image.code.push_back(static_cast<std::uint8_t>(w >> shift));
    }
    image.extents = {{0x82000000, 8}};   // the first function is known (a bl and a blr); the others are found by calls
    recompile::disassemble::Disassembler dis;
    const dc::Discovery found = dc::discover(image, dis, {.pointer_scan = false});
    CB_CHECK_EQ(found.functions.size(), 3u);
    CB_CHECK(found.functions.contains(0x82000008));
    CB_CHECK(found.functions.contains(0x82000010));
    CB_CHECK_EQ(found.sources.calls, 2u);
    // a bl into an import, or out of the code, is not a function
    image.imports = {0x82000008};
    image.code_end = 0x82000010;
    const dc::Discovery fewer = dc::discover(image, dis, {.pointer_scan = false});
    CB_CHECK_EQ(fewer.functions.size(), 1u);
});

CB_TEST("recompile.discover.the_command_prints_the_counts_and_lists_on_request", [] {
    const DiscoverFacts facts = read_discover_facts();
    const dc::ImageReader reader = [&](const std::filesystem::path&) { return slice_image(facts); };
    const std::vector<std::string> plain = {"minecraft.xex"};
    {
        testing::CapturedStd captured;
        CB_CHECK_EQ(dc::main(reader, slice_vector_decoder(), plain), 0);
        const std::string out = captured.out();
        const std::size_t count = facts.sections.at("discover_default").size() - 1;   // (the sources line is not a function)
        CB_CHECK(out.starts_with(std::format("{} functions: pdata 2, entry 0, calls 6, pointers 28, constants 1, seed 0\n", count)));
        CB_CHECK(out.find("  822B6EA8") == std::string::npos);   // not listed without --list
    }
    {
        testing::CapturedStd captured;
        const std::vector<std::string> listed = {"minecraft.xex", "--list"};
        CB_CHECK_EQ(dc::main(reader, slice_vector_decoder(), listed), 0);
        CB_CHECK(captured.out().find("  822B6EA8  256\n") != std::string::npos);
    }
    testing::CapturedStd captured;
    CB_CHECK_EQ(dc::main(reader, slice_vector_decoder(), {}), 2);   // no image: a usage error
});
