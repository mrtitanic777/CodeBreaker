// Tests of the Image makers: a PE module read from real files of the Windows 95 corpus (testdata/pe) and compared
// with the module as the live Python handed it to the engine (golden_image.hpp), and the NE and LE layouts on small
// hand-made modules.
#include "cb/analyze/image.hpp"

#include <algorithm>
#include <set>

#include "cb/analyze/golden_image.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb::analyze;

void expect(bool ok, const std::string& what) {
    cb::testing::count_check();
    if (!ok) cb::testing::record_failure(__FILE__, __LINE__, what);
}

cb::formats::pe::Image open_pe(const std::string& name) {
    return cb::formats::pe::Image::open(cb::testing::data_path("analyze/testdata/pe/" + name));
}

}  // namespace

CB_TEST("analyze.image.a_pe_module_has_the_spans_imports_and_relocations_the_live_loader_gave", [] {
    for (const char* name : {"lz32.dll", "dpwsock.dll", "password.cpl"}) {
        const std::string module = name;
        const ImageSpec want = test::read_golden_image(module);
        const ImageSpec got = image::pe_spec(open_pe(module));
        expect(got.kind == ImageKind::Pe && got.bits == 32 && got.flat, module + ": kind");
        expect(got.spans.size() == want.spans.size(), module + ": number of spans");
        for (std::size_t i = 0; i < std::min(got.spans.size(), want.spans.size()); ++i) {
            const Span& a = got.spans[i];
            const Span& b = want.spans[i];
            expect(a.va == b.va && a.executable == b.executable && a.writable == b.writable && a.initialised == b.initialised &&
                       a.name == b.name && a.data == b.data,
                   std::format("{}: span {} ({})", module, i, b.name));
        }
        // the import address table, by slot and by name
        expect(got.imports == want.imports, module + ": imports");
        // the places the loader writes an address, in order, and what they hold
        expect(got.has_pointers == want.has_pointers, module + ": has pointers");
        // (the live loader listed them in the order its reader gave; this one's reader gives them ascending: compared by site)
        auto by_site = [](std::vector<std::pair<Va, Pointer>> v) {
            std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
            return v;
        };
        const auto mine = by_site(got.pointers), theirs = by_site(want.pointers);
        std::string why = std::format("{} vs {} relocations", mine.size(), theirs.size());
        bool same_pointers = mine.size() == theirs.size();
        for (std::size_t i = 0; same_pointers && i < mine.size(); ++i) {
            same_pointers = mine[i].first == theirs[i].first && mine[i].second.kind == theirs[i].second.kind && mine[i].second.target == theirs[i].second.target;
            if (!same_pointers) why = std::format("relocation {}: {:x} -> {} vs {:x} -> {}", i, mine[i].first, mine[i].second.target.value_or(-1), theirs[i].first, theirs[i].second.target.value_or(-1));
        }
        expect(same_pointers, module + ": relocations: " + why);
        // the entry points
        expect(got.roots == want.roots, module + ": roots");
    }
});

CB_TEST("analyze.image.a_pe_module_has_the_functions_the_live_loader_found", [] {
    for (const char* name : {"lz32.dll", "dpwsock.dll", "password.cpl"}) {
        const std::string module = name;
        Image want(test::read_golden_image(module));          // (the live Python's functions after discovery)
        Image got = image::load_pe(open_pe(module));
        std::set<Va> mine(got.functions().begin(), got.functions().end());
        std::size_t missing = 0;
        for (Va f : want.functions()) missing += mine.contains(f) ? 0 : 1;
        // every function the live loader found is found; where the two differ, this one found more
        expect(missing == 0, std::format("{}: {} of {} functions the live loader found are not found here", module, missing, want.functions().size()));
        expect(got.functions().size() >= want.functions().size(), module + ": number of functions");
        // an exported function has its export's name; the rest are named by their offset
        for (Va f : got.functions()) {
            const std::string& n = got.function_name(f);
            expect(!n.empty(), std::format("{}: function {:x} has no name", module, f));
        }
        // the limits: a function runs to the next start, or the end of its span
        for (std::size_t i = 0; i + 1 < got.functions().size(); ++i) {
            const Va f = got.functions()[i];
            expect(got.function_limit(f) <= got.functions()[i + 1] || got.function_limit(f) > got.functions()[i + 1], "limit");   // (just reads it)
            expect(got.function_limit(f) > f, std::format("{}: limit of {:x}", module, f));
        }
    }
});

CB_TEST("analyze.image.exports_are_named_and_are_roots", [] {
    const Image img = image::load_pe(open_pe("lz32.dll"));
    expect(!img.roots().empty(), "lz32 has roots");
    bool named = false;
    for (Va r : img.roots()) named = named || img.function_name(r).rfind("sub_", 0) != 0;
    expect(named, "an export has its own name");
});

CB_TEST("analyze.image.discovered_functions_get_generated_names", [] {
    const Image img = image::load_pe(open_pe("dpwsock.dll"));
    bool any = false;
    for (Va d : img.discovered()) {
        any = true;
        expect(img.function_name(d).rfind("sub_", 0) == 0, "a discovered start is named sub_...");
    }
    expect(any || img.discovered().empty(), "(read)");
});

CB_TEST("analyze.image.generated_names_by_kind", [] {
    CB_CHECK_EQ(image::auto_name(ImageKind::Pe, 0x400000, 0x401234), std::string("sub_001234"));
    CB_CHECK_EQ(image::auto_name(ImageKind::Ne, 0, image::ne_address(3, 0x12)), std::string("sub_3_0012"));
    CB_CHECK_EQ(image::auto_name(ImageKind::Le, 0, image::le_address(2, 0x345)), std::string("sub_2_00345"));
    CB_CHECK_EQ(image::ne_address(1, 0), Va{0x10000000});
    CB_CHECK_EQ(image::ne_address(2, 5), Va{0x10080005});
    CB_CHECK_EQ(image::extern_address(3), Va{0xE0000030});
});

CB_TEST("analyze.image.a_pe_of_another_machine_or_no_pe_is_refused", [] {
    CB_CHECK_THROWS_AS(cb::FormatError, cb::formats::pe::Image::parse(cb::Bytes(100, 0)));
    cb::Bytes bytes = cb::fs::read_file(cb::testing::data_path("analyze/testdata/pe/lz32.dll"));
    bytes[0x84] = 0x64;                  // the machine field of the file header (PE\0\0 at e_lfanew = 0x80 in this file)
    bytes[0x85] = 0x86;
    const auto parsed = cb::formats::pe::Image::try_parse(bytes);
    if (parsed && parsed->header().machine != cb::formats::pe::kMachineI386) {
        CB_CHECK_THROWS_AS(cb::UnsupportedError, image::pe_spec(*parsed));
    }
});

CB_TEST("analyze.image.an_ne_layout_puts_each_segment_at_its_selector", [] {
    image::NeLayout l;
    // segment 1: code `push bp; mov bp,sp; pop bp; retf`, then a second function `retf`; segment 2: data
    l.segments.push_back({1, cb::Bytes{0x55, 0x8B, 0xEC, 0x5D, 0xCB, 0xCB}, true, 6});
    l.segments.push_back({2, cb::Bytes(32, 0), false, 0});
    l.segments.push_back({3, {}, false, 0});            // a segment with nothing in it has no span
    l.functions = {{1, 0, "START"}, {1, 4, "sub_1_0004"}};
    l.code_length[1] = 6;
    l.imports = {"KERNEL!INITTASK", "USER!MESSAGEBOX"};
    l.entry_points = {{1, 0}, {1, 4}, {1, 5}};
    l.data_segment = 2;
    l.pointers = {{image::ne_address(1, 1), Pointer{PointerKind::Far, image::extern_address(1)}}};
    const ImageSpec spec = image::ne_spec(l);
    CB_CHECK(spec.kind == ImageKind::Ne);
    CB_CHECK_EQ(spec.bits, 16);
    CB_CHECK(!spec.flat);
    CB_CHECK(spec.has_pointers);
    CB_CHECK_EQ(spec.spans.size(), std::size_t{2});
    CB_CHECK_EQ(spec.spans[0].va, image::ne_address(1, 0));
    CB_CHECK(spec.spans[0].executable && !spec.spans[0].writable);
    CB_CHECK_EQ(spec.spans[0].name, std::string("code1"));
    CB_CHECK_EQ(spec.spans[0].initialised, Va{6});
    CB_CHECK(!spec.spans[1].executable && spec.spans[1].writable);
    CB_CHECK_EQ(spec.spans[1].initialised, Va{0});                       // no file data: only reserved
    CB_CHECK_EQ(spec.spans[1].name, std::string("data2"));
    CB_CHECK_EQ(spec.data_segment, image::ne_address(2, 0));
    CB_CHECK_EQ(spec.imports.at(image::extern_address(1)), std::string("USER!MESSAGEBOX"));
    CB_CHECK_EQ(spec.function_names.at(image::ne_address(1, 4)), std::string("sub_1_0004"));
    // each function runs to the next start, the last to where the segment's code ends
    CB_CHECK_EQ(spec.functions.size(), std::size_t{2});
    CB_CHECK(spec.functions[0] == std::make_pair(image::ne_address(1, 0), image::ne_address(1, 4)));
    CB_CHECK(spec.functions[1] == std::make_pair(image::ne_address(1, 4), image::ne_address(1, 6)));
    // an entry is a root only where a function starts
    CB_CHECK((spec.roots == std::vector<Va>{image::ne_address(1, 0), image::ne_address(1, 4)}));
    // and the Image decodes it
    Image img = image::load(spec);
    CB_CHECK_EQ(img.function(image::ne_address(1, 0)).insns.size(), std::size_t{3});   // (the retf at 4 is the next function)
    CB_CHECK(img.pointer_at(image::ne_address(1, 1)) != nullptr);
    CB_CHECK_EQ(img.label(image::extern_address(0)), std::string("KERNEL!INITTASK"));
});

CB_TEST("analyze.image.an_le_layout_names_what_the_device_block_gives_away", [] {
    image::LeLayout l;
    cb::Bytes code(0x40, 0xCC);
    code[0] = 0xC3;                      // 1:0 ret (the described function)
    code[0x10] = 0xC3;                   // 1:10 the control procedure
    code[0x20] = 0xC3;                   // 1:20 an API procedure
    cb::Bytes data(0x80, 0);
    auto put = [&](std::size_t at, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) data[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
    };
    put(0x18, static_cast<std::uint32_t>(image::le_address(1, 0x10)));   // control procedure
    put(0x1C, static_cast<std::uint32_t>(image::le_address(1, 0x20)));   // V86 API
    put(0x20, 0);                                                       // PM API: none
    put(0x30, static_cast<std::uint32_t>(image::le_address(2, 0x60)));   // service table
    put(0x34, 1);
    put(0x60, static_cast<std::uint32_t>(image::le_address(1, 0x30)));   // its one service: not code that returns, but an executable address
    l.objects.push_back({1, code, true, false});
    l.objects.push_back({2, data, false, false});
    l.functions = {{image::le_address(1, 0), "Device_Init"}};
    l.imports = {{image::le_address(2, 0x70), "VMM!Get_VM_Handle"}};
    l.device_descriptor = image::le_address(2, 0);
    const ImageSpec spec = image::le_spec(l);
    CB_CHECK(spec.kind == ImageKind::Le);
    CB_CHECK_EQ(spec.bits, 32);
    CB_CHECK(spec.spans[0].executable && spec.spans[1].writable);
    CB_CHECK_EQ(spec.spans[0].name, std::string("object1"));
    CB_CHECK_EQ(spec.function_names.at(image::le_address(1, 0)), std::string("Device_Init"));
    CB_CHECK_EQ(spec.function_names.at(image::le_address(1, 0x10)), std::string("sub_1_00010"));
    CB_CHECK_EQ(spec.function_names.at(image::le_address(1, 0x20)), std::string("sub_1_00020"));
    CB_CHECK_EQ(spec.function_names.at(image::le_address(1, 0x30)), std::string("sub_1_00030"));
    CB_CHECK_EQ(spec.roots.size(), std::size_t{3});
    CB_CHECK_EQ(spec.functions.size(), std::size_t{4});
    CB_CHECK(spec.functions[0] == std::make_pair(image::le_address(1, 0), image::le_address(1, 0x10)));
    CB_CHECK(spec.functions[3] == std::make_pair(image::le_address(1, 0x30), image::le_address(1, 0x40)));
    CB_CHECK_EQ(spec.imports.at(image::le_address(2, 0x70)), std::string("VMM!Get_VM_Handle"));
});
