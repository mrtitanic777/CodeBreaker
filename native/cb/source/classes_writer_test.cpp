// Tests of the classes writer: the declarations it writes from the classes analysis found (each virtual function tied to the function that
// implements it, with its verdict), the virtual tables of a PowerPC image, and the functions of a 32-bit module that only jump through an
// import -- the last over a real file (testdata: lz32.dll).
//
// Behaviour pinned here: a class with the shape of a COM interface says so ("has the shape of a COM interface"), where the
// a boolean flag would print, "(True)".

#include "cb/source/classes_writer.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::source::classes_writer {
namespace {

class ByTable final : public FunctionNotes {
public:
    std::string name_at(Va address) const override {
        switch (address) {
            case 0x401000: return "GetRuntimeClass";
            case 0x401010: return "sub_00401010";
            case 0x401020: return "OnCreate";
            case 0x401500: return "CMainFrame_ctor";
            case 0x401600: return "CMainFrame_dtor";
            default: return std::format("sub_{:08X}", address);
        }
    }
    std::string status_at(Va address) const override {
        switch (address) {
            case 0x401000: return "VERIFIED";
            case 0x401010: return "UNVERIFIED";
            default: return "";
        }
    }
};

ClassRecord main_frame() {
    ClassRecord c;
    c.vtable = 0x403000;
    c.name = "CMainFrame";
    c.bases = {"CFrameWnd", "CWnd", "CCmdTarget"};
    c.methods = {0x401000, 0x401010, 0x401020};
    c.slots = 3;
    c.evidence = {"run-time type information names it"};
    c.size = 0x1a8;
    c.installed_by = {0x401500, 0x401600};
    c.message_map = 0x404000;
    return c;
}

CB_TEST("source.classes_writer.a_class_is_a_declaration_with_the_verdict_of_each_function", [] {
    ClassRecord com;
    com.vtable = 0x405000;
    com.name = "class_405000";
    com.methods.assign(12, 0x401000);
    com.size = 0x20;
    com.size_exact = false;
    com.com = true;
    ClassRecord odd;
    odd.vtable = 0x406000;
    odd.name = "CWeird";
    odd.bases = {"MFC42.800", "", "9lives", "ok_name"};
    odd.size = 0;   // a size of nothing is not said
    const std::vector<ClassRecord> classes = {main_frame(), com, odd};
    std::vector<std::string> want = {
        "// demo.exe -- the C++ classes this module shows, as class declarations.",
        "//",
        "// Read beside demo.c. Not compiled: each virtual function is the reconstructed function named",
        "// in its comment, where its real parameters are. A class or function name the binary does",
        "// not state is a placeholder (class_<address of its table>, vf_<slot>).",
        "//",
        "// 3 classes; 2 named by the binary itself.",
        "",
        "// 3 virtual functions; table at 403000; object of 0x1a8 bytes; message map at 404000",
        "//   run-time type information names it",
        "//   constructed or destroyed in: CMainFrame_ctor, CMainFrame_dtor",
        "class CMainFrame : public CFrameWnd {",
        "    // further up: CWnd : CCmdTarget",
        "public:",
        "    virtual int4 vf_0();     // GetRuntimeClass  [VERIFIED]",
        "    virtual int4 vf_1();     // sub_00401010  [UNVERIFIED]",
        "    virtual int4 vf_2();     // OnCreate",
        "};",
        "",
        "// 12 virtual functions; table at 405000; object of 0x20 bytes at least; has the shape of a COM interface"};
    want.push_back("class class_405000 {");
    want.push_back("public:");
    for (int slot = 0; slot < 12; ++slot) want.push_back(std::format("    virtual int4 vf_{}();{}// GetRuntimeClass  [VERIFIED]", slot, std::string(slot < 10 ? 5 : 4, ' ')));
    want.insert(want.end(), {"};", "", "// 0 virtual functions; table at 406000", "class CWeird : public imported_class_MFC42800 {",
                             "    // further up: imported_class_9lives : ok_name", "public:", "};", ""});
    CB_CHECK_EQ(classes_text("demo.exe", "demo", classes, ByTable()), text::join(want, "\n"));
});

CB_TEST("source.classes_writer.installed_by_names_at_most_eight_functions", [] {
    ClassRecord c = main_frame();
    c.installed_by.clear();
    for (Va a = 0x500000; a < 0x500000 + 12; ++a) c.installed_by.push_back(a);
    const std::string text = classes_text("demo.exe", "demo", std::vector<ClassRecord>{c}, ByTable());
    CB_CHECK(text.find("//   constructed or destroyed in: sub_00500000, sub_00500001, sub_00500002, sub_00500003, sub_00500004, sub_00500005, "
                       "sub_00500006, sub_00500007\n") != std::string::npos);
    CB_CHECK(text.find("sub_00500008") == std::string::npos);
});

CB_TEST("source.classes_writer.write_makes_the_file_beside_the_c_or_nothing", [] {
    fs::ScopedTempDir dir("cb_classes");
    CB_CHECK(!write({}, "demo.exe", "demo", dir.path(), ByTable()).has_value());
    CB_CHECK(!std::filesystem::exists(dir.path() / "demo_classes.hpp"));
    const std::vector<ClassRecord> classes = {main_frame()};
    const auto out = write(classes, "demo.exe", "demo", dir.path() / "src", ByTable());   // (the folder is made)
    CB_CHECK(out && *out == dir.path() / "src" / "demo_classes.hpp");
    CB_CHECK_EQ(fs::read_text(*out), classes_text("demo.exe", "demo", classes, ByTable()));
});

// ---- PowerPC ----------------------------------------------------------------------------------------------------------

class FakeImage final : public PowerPcSource {
public:
    std::set<std::uint32_t> functions_;
    std::optional<std::pair<std::uint32_t, std::uint32_t>> code_;
    std::vector<Section> sections_;
    std::map<std::uint32_t, Bytes> data_;

    const std::set<std::uint32_t>& functions() const override { return functions_; }
    std::optional<std::pair<std::uint32_t, std::uint32_t>> code() const override { return code_; }
    std::span<const Section> sections() const override { return sections_; }
    Bytes bytes_at(std::uint32_t va, std::uint32_t) const override { return data_.at(va); }

    void add(const std::string& name, std::uint32_t va, const std::vector<std::uint32_t>& words) {
        Bytes b;
        for (const std::uint32_t w : words) {
            for (int i = 0; i < 4; ++i) b.push_back(static_cast<std::uint8_t>(w >> (24 - 8 * i)));
        }
        sections_.push_back({name, va, static_cast<std::uint32_t>(b.size())});
        data_[va] = std::move(b);
    }
};

CB_TEST("source.classes_writer.a_table_of_the_images_own_functions_is_a_virtual_table", [] {
    FakeImage image;
    image.functions_ = {0x82000100, 0x82000200, 0x82000300, 0x82000400};
    image.code_ = {{0x82000000, 0x82001000}};
    image.add(".text", 0x82000000, {0x82000100, 0x82000200, 0x82000300});                          // code: never read as a table
    image.add(".rdata", 0x82100000, {0, 0x82000100, 0x82000200, 0x82000300, 0xDEAD, 0x82000400, 0, 0x82000100, 0x82000200});
    image.add(".reloc", 0x82200000, {0x82000100, 0x82000200});                                       // skipped by name
    CB_CHECK_EQ(image.functions().size(), 4u);
    const std::vector<ClassRecord> tables = powerpc_tables(image);
    CB_CHECK_EQ(tables.size(), 2u);   // [100 200 300] at word 1, and [100 200] at word 7 -- the run [400] alone is no table
    CB_CHECK_EQ(tables[0].vtable, 0x82100004);
    CB_CHECK_EQ(tables[0].slots, 3);
    CB_CHECK((tables[0].methods == std::vector<Va>{0x82000100, 0x82000200, 0x82000300}));
    CB_CHECK_EQ(tables[0].name, "class_82100004");
    CB_CHECK(tables[0].bases.empty() && !tables[0].size && !tables[0].com && !tables[0].message_map && tables[0].installed_by.empty());
    CB_CHECK_EQ(tables[0].evidence.size(), 1u);
    CB_CHECK(tables[0].evidence[0].starts_with("a table of 3 of the image's own functions, in .rdata: a virtual table, or another table of functions"));
    CB_CHECK_EQ(tables[1].vtable, 0x82100000 + 4 * 7);
    CB_CHECK_EQ(tables[1].slots, 2);
});

CB_TEST("source.classes_writer.the_pdata_table_says_how_the_addresses_in_the_data_are_shifted", [] {
    // The data holds addresses as linked (0x10000 lower than where the functions are); the table of function entries gives the shift away:
    // its first words are the functions' addresses as linked.
    FakeImage image;
    image.functions_ = {0x82000100, 0x82000200, 0x82000300, 0x82000400};
    image.code_ = {{0x82000000, 0x82001000}};
    const std::uint32_t shift = 0x10000;
    image.add(".pdata", 0x82300000, {0x82000100 - shift, 0x11111, 0x82000200 - shift, 0x22222, 0x82000300 - shift, 0x33333, 0x82000400 - shift, 0x44444});
    image.add(".rdata", 0x82100000, {0x82000300 - shift, 0x82000400 - shift, 0x82000100 - shift});
    const std::vector<ClassRecord> tables = powerpc_tables(image);
    CB_CHECK_EQ(tables.size(), 1u);
    if (tables.size() == 1) {
        CB_CHECK_EQ(tables[0].slots, 3);
        // the methods are the functions' own addresses, as the image has them
        CB_CHECK((tables[0].methods == std::vector<Va>{0x82000300, 0x82000400, 0x82000100}));
    }
});

CB_TEST("source.classes_writer.an_image_with_no_table_has_no_class", [] {
    FakeImage image;
    image.functions_ = {0x82000100};
    image.code_ = {{0x82000000, 0x82001000}};
    image.add(".rdata", 0x82100000, {0x82000100, 0x82000100, 0x82000100});   // three words that name one function is a table of three
    CB_CHECK_EQ(powerpc_tables(image).size(), 1u);
    FakeImage none;
    CB_CHECK(powerpc_tables(none).empty());
    FakeImage short_section;
    short_section.functions_ = {0x82000100};
    short_section.add(".data", 0x82100000, {0x82000100});   // under two words and a table is not read
    CB_CHECK(powerpc_tables(short_section).empty());
});

// ---- jumps through an import ------------------------------------------------------------------------------------------

class OneModule final : public symbols::ordinals::ModuleExports {
public:
    std::optional<Table> table(std::string_view stem) const override {
        if (text::upper(stem) != "COMCTL32") return std::nullopt;
        return Table{{{17, "InitCommonControls"}}, "comctl32.dll"};
    }
};

CB_TEST("source.classes_writer.a_function_that_only_jumps_through_an_import_is_named_for_it", [] {
    const formats::pe::Image pe = formats::pe::Image::open(testing::data_path("formats/testdata/lz32.dll"));
    const std::vector<formats::pe::Import> imports = pe.imports();
    // a `jmp [slot]` in the code, whose slot is an import
    std::optional<std::pair<Va, formats::pe::Import>> thunk;
    const std::uint64_t base = pe.optional_header().image_base;
    for (const formats::pe::Section& s : pe.sections()) {
        const ByteView d = pe.section_data(s);
        for (std::size_t i = 0; i + 6 <= d.size() && !thunk; ++i) {
            if (d[i] != 0xFF || d[i + 1] != 0x25) continue;
            const std::uint64_t slot = d[i + 2] | static_cast<std::uint64_t>(d[i + 3]) << 8 | static_cast<std::uint64_t>(d[i + 4]) << 16 | static_cast<std::uint64_t>(d[i + 5]) << 24;
            for (const formats::pe::Import& imp : imports) {
                if (imp.slot_va == slot) thunk = {static_cast<Va>(base + s.virtual_address + i), imp};
            }
        }
    }
    CB_CHECK(thunk.has_value());
    if (!thunk) return;
    const OneModule exports;
    const symbols::ordinals::Ordinals ordinals(exports, {});
    const ImportThunks thunks(pe);
    const std::string library = text::upper(std::filesystem::path(thunk->second.dll).stem().string());
    CB_CHECK_EQ(thunks.behind(thunk->first, ordinals).value_or(""), library + "." + thunk->second.name);
    CB_CHECK(!thunks.behind(static_cast<Va>(base) + 0x10, ordinals).has_value());     // the headers are not a jump
    CB_CHECK(!thunks.behind(static_cast<Va>(base) - 1, ordinals).has_value());        // below the image
    CB_CHECK(!thunks.behind(static_cast<Va>(base) + 0x7FFFFFF, ordinals).has_value()); // beyond it
    CB_CHECK(!thunks.behind(-5, ordinals).has_value());

    // an import by number is named where the tables know it, else by its number
    const auto named_as = [&](const char* dll, const char* symbol) {
        const formats::pe::Import imp{thunk->second.slot_va, dll, symbol};
        const ImportThunks other(pe, std::span<const formats::pe::Import>(&imp, 1));
        return other.behind(thunk->first, ordinals);
    };
    CB_CHECK_EQ(named_as("comctl32.dll", "#17").value_or(""), "COMCTL32.InitCommonControls");
    CB_CHECK_EQ(named_as("comctl32.dll", "#99").value_or(""), "COMCTL32.99");
    CB_CHECK_EQ(named_as("comctl32.dll", "#abc").value_or(""), "COMCTL32.#abc");   // not a number: a name that begins with #
    CB_CHECK_EQ(named_as("KERNEL32.DLL", "Sleep").value_or(""), "KERNEL32.Sleep");
    CB_CHECK(!named_as("", "Sleep").has_value());
});

}  // namespace
}  // namespace cb::source::classes_writer
