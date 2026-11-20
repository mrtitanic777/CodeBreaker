#include "exact/project.hpp"

#include <algorithm>


#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

using namespace cb;
using namespace cb::exact;
namespace stdfs = std::filesystem;

// testdata/project is a small real-mode program (an MZ file of two relocations, a 96-byte image) and the folders the
// stages read of it: project.json, SYMBOLS, one-function files, whole files placed two ways, an "also" set of assembler
// files. The expected values below were read off the project's output for the same folder, and
// checked against the file by hand: the image is the file less its 64-byte header; both relocations hold the word 4,
// so DGROUP is paragraph 4, offset 0x40).
//
// Design notes:
//   * SYMBOLS lines "name ? address" (what save_symbols writes for a place of no kind) are read back as such, so
//     a save does not lose a name whose kind was unsaid
//   * a SYMBOLS or "starts" line whose address is not hexadecimal is a ParseError
//   * a "sources" match whose group 1 is not a number is left out
//   * the "data_pads" are in address order
//   * "code_segments" as any text but "relocs" is refused

namespace {

std::string base_name(const stdfs::path& p) { return fs::to_text(p.filename()); }

// A project folder made of the checked-in fixture, writable.
struct Folder {
    fs::ScopedTempDir temp{"cb-project"};
    stdfs::path dir() const { return temp.path() / "p"; }
    Folder() { fs::copy_tree(testing::data_path("exact/testdata/project"), dir()); }
};

// A profiles folder with the toolchains the fixture names, and a kit folder for each kit they refer to.
struct Profiles {
    fs::ScopedTempDir temp{"cb-project-profiles"};
    toolchain::Locations locations;
    Profiles() {
        const stdfs::path profiles = temp.path() / "profiles";
        fs::write_text(profiles / "fake.json",
                       R"({"name": "fake", "kit": "kitx", "variants": {"v1": []}, "headers": [".h", ".inc"],
                           "libraries": {"M": [{"tag": "T", "path": "lib/m.lib"}], "S": [{"path": "lib/s.lib"}]},
                           "objects": {"M": ["o1.obj", "o2.obj"]}, "data": {"order": "before_next_function"}, "inverse": "x86",
                           "carve": {"prologues": ["55 8b ec"], "frames": false}, "link": {"run": "link.exe", "libs": {"S": ["a.lib"]}, "psp": "0x20"},
                           "communal": {"segment": "c_common", "align": "para"}})");
        fs::write_text(profiles / "fake2.json", R"({"name": "fake2", "variants": {"v1": []}})");
        fs::write_text(profiles / "pas.json", R"({"name": "pas", "plugin": "fakepascal"})");
        locations.profiles = profiles;
        locations.state = temp.path() / "state";
        locations.checkout_root = temp.path();
        locations.kits = {{"kitx", temp.path() / "kit-x"}, {"KIT", temp.path() / "kit-other"}};
    }
};

// A toolchain that compiles Pascal, as the Pascal project asks.
class FakePascal final : public toolchain::Toolchain, public pascal_join::Toolchain {
public:
    FakePascal(const toolchain::Profile& profile, const std::optional<std::string>& variant, const toolchain::Context& context)
        : toolchain::Toolchain(profile, variant, context) {}
    pascal_join::CompileResult compile(ByteView, std::string_view) const override { return {}; }
    modules::CompileResult compile(ByteView source, const modules::CompileOptions& options) const override {
        return toolchain::Toolchain::compile(source, options);
    }
    std::uint32_t helper_data() const override { return 5; }
};

std::shared_ptr<project::Services> services_of(toolchain::Toolchains& tcs) {
    auto s = std::make_shared<project::Services>();
    s->toolchains = &tcs;
    return s;
}

// An MZ program: `image` after a header of `header_paragraphs`, relocations (offset, segment) listed at 0x1C.
struct Reloc {
    std::uint16_t offset, segment;
};

Bytes mz_file(const Bytes& image, const std::vector<Reloc>& relocs, std::uint16_t header_paragraphs = 4) {
    Bytes out(static_cast<std::size_t>(header_paragraphs) * 16, 0);
    const auto put = [&](std::size_t at, std::uint16_t v) {
        out[at] = static_cast<std::uint8_t>(v);
        out[at + 1] = static_cast<std::uint8_t>(v >> 8);
    };
    const std::size_t size = out.size() + image.size();
    put(0, 0x5A4D);
    put(2, static_cast<std::uint16_t>(size % 512));
    put(4, static_cast<std::uint16_t>((size + 511) / 512));
    put(6, static_cast<std::uint16_t>(relocs.size()));
    put(8, header_paragraphs);
    put(24, 0x1C);
    for (std::size_t i = 0; i < relocs.size(); ++i) {
        put(0x1C + 4 * i, relocs[i].offset);
        put(0x1C + 4 * i + 2, relocs[i].segment);
    }
    out.insert(out.end(), image.begin(), image.end());
    return out;
}

void put_word(Bytes& b, std::size_t at, std::uint16_t v) {
    b[at] = static_cast<std::uint8_t>(v);
    b[at + 1] = static_cast<std::uint8_t>(v >> 8);
}

}  // namespace

CB_TEST("exact.project.program_and_numbers", [] {
    Folder f;
    const auto p = project::Project::open(f.dir());
    CB_CHECK_EQ(p->image().size(), 96u);
    CB_CHECK_EQ(p->file().size(), 160u);
    CB_CHECK_EQ(p->image()[0], 3);
    CB_CHECK_EQ(p->image()[0x10], 4);   // (the word both relocations stand at)
    CB_CHECK_EQ(p->dgroup(), 0x40);     // (from the relocations: no "dgroup" in the file)
    CB_CHECK(p->code_segments() == (std::vector<std::uint32_t>{0, 0x10}));   // (written "0x10" and 0: sorted)
    CB_CHECK_EQ(p->loadseg(), 0x10u);
    CB_CHECK_EQ(p->code_end(), 0x50u);
    CB_CHECK(p->switches() == std::optional<std::string>("/Ox"));
    CB_CHECK(p->relocs() == (std::unordered_set<std::uint32_t>{0x10, 0x20}));
    CB_CHECK(p->header().has_value());
    CB_CHECK_EQ((*p->header())[1], 160);
    CB_CHECK_EQ((*p->header())[3], 2);
    CB_CHECK(!p->packed().has_value());
    CB_CHECK_EQ(p->code_base(0), 0u);
    CB_CHECK_EQ(p->code_base(5), 0u);
    CB_CHECK_EQ(p->code_base(0x10), 0x10u);
    CB_CHECK_EQ(p->code_base(0x3000), 0x10u);
    CB_CHECK_EQ(base_name(p->exe()), "PROG.EXE");
    CB_CHECK_EQ(base_name(p->src()), "src");
    CB_CHECK_EQ(p->include().size(), 2u);
    CB_CHECK_EQ(base_name(p->include()[0]), "inc");
    CB_CHECK_EQ(base_name(p->include()[1]), "up");   // ("../up": the folder beside the project's)
    CB_CHECK(p->include()[1].parent_path() == p->dir().parent_path());
}

);

CB_TEST("exact.project.config", [] {
    Folder f;
    const auto p = project::Project::open(f.dir());
    const project::Config& c = p->config();
    CB_CHECK_EQ(c.image, "PROG.EXE");
    CB_CHECK_EQ(c.model, "M");
    CB_CHECK_EQ(c.library_gap, 100u);
    CB_CHECK(c.library_start == std::optional<std::uint32_t>(0x30));
    CB_CHECK_EQ(c.libraries.size(), 2u);
    CB_CHECK_EQ(c.libraries[0].tag, "EM");
    CB_CHECK_EQ(c.libraries[1].tag, "");
    CB_CHECK_EQ(c.libraries[1].path, "KIT:x.lib");
    CB_CHECK(c.first_objects == (std::vector<std::string>{"BIN.OBJ"}));
    CB_CHECK_EQ(c.data_sizes.at("_iob"), 32u);
    CB_CHECK_EQ(c.data_pads.size(), 2u);   // ({"0100": 16, "00a0": 4}: by address)
    CB_CHECK_EQ(c.data_pads[0].address, 0xA0u);
    CB_CHECK_EQ(c.data_pads[0].length, 4u);
    CB_CHECK_EQ(c.data_pads[1].address, 0x100u);
    CB_CHECK(c.link.linker == std::optional<std::string>("LINK.EXE"));
    CB_CHECK_EQ(c.link.switches, "/B");
    CB_CHECK_EQ(c.also.size(), 1u);
    CB_CHECK(c.toolchains.empty());
    CB_CHECK(c.toolchain == std::optional<std::string>("fake"));
    CB_CHECK_EQ(c.strings.at("named_by"), "x");
    CB_CHECK(!c.strings.contains("dgroup"));
    CB_CHECK(!c.dgroup.has_value());

    const project::Config d = project::parse_config(R"({"image": "a.exe", "dgroup": "0x61F0", "code_segments": "relocs", "loadseg": 16,
        "toolchains": [{"toolchain": "msc300", "variant": "own"}, {"label": "two", "toolchain": "msc400"}], "whole": true, "starts": ""})");
    CB_CHECK(d.dgroup == std::optional<std::int64_t>(0x61F0));
    CB_CHECK(d.code_segments_from_relocs);
    CB_CHECK_EQ(d.loadseg, 16u);
    CB_CHECK_EQ(d.toolchains.size(), 2u);
    CB_CHECK_EQ(d.toolchains[0].label, "msc300");   // (no label: the toolchain's name)
    CB_CHECK(d.toolchains[0].variant == std::optional<std::string>("own"));
    CB_CHECK_EQ(d.toolchains[1].label, "two");
    CB_CHECK(d.whole);
    CB_CHECK(!d.starts.has_value());   // (the empty name of a file is none)
    CB_CHECK_EQ(d.src, "src");
    CB_CHECK_EQ(d.sources, R"(f([0-9a-f]{5})\.c)");

    CB_CHECK_THROWS_AS(FormatError, project::parse_config(R"({"toolchain": "x"})"));
    CB_CHECK_THROWS_AS(ParseError, project::parse_config("{not json"));
    CB_CHECK_THROWS_AS(ParseError, project::parse_config(R"({"image": "a", "dgroup": "far"})"));
    CB_CHECK_THROWS_AS(ParseError, project::parse_config(R"({"image": "a", "code_segments": "all"})"));
    CB_CHECK_THROWS_AS(ParseError, project::parse_config(R"({"image": "a", "loadseg": -1})"));
    CB_CHECK_THROWS_AS(ParseError, project::parse_config(R"({"image": "a", "libraries": [{"tag": "x"}]})"));
}

);

CB_TEST("exact.project.symbols", [] {
    Folder f;
    const auto p = project::Project::open(f.dir());
    const SymbolTable s = p->symbols();
    CB_CHECK_EQ(s.size(), 6u);
    const auto at = [&](const char* name) { return s.at(name); };
    CB_CHECK(at("_main") == (SymPlace{SymKind::Code, 0x10}));
    CB_CHECK(at("_buf") == (SymPlace{SymKind::Data, 0x100}));
    CB_CHECK(at("seg@x") == (SymPlace{SymKind::Segment, 4}));
    CB_CHECK(at("bare") == (SymPlace{SymKind::Unknown, 0x20}));   // (name address: the kind unsaid)
    CB_CHECK(at("xx") == (SymPlace{SymKind::Unknown, 0xC}));      // ("xx C": the C is read as the number it also is)
    CB_CHECK(at("dup") == (SymPlace{SymKind::Data, 0x200}));      // (a name met again takes the last place)
    CB_CHECK_EQ(s.begin()->first, "_main");                       // (and keeps its first position)
    CB_CHECK_EQ(s.begin()[5].first, "dup");

    // SYMBOLS written back: by kind ('?', C, D, S), address, name
    p->save_symbols(s);
    CB_CHECK_EQ(fs::read_text(p->path("SYMBOLS")),
                "# name, C (code: image offset) or D (data: offset in the data segment), address\n"
                "xx ? 000c\nbare ? 0020\n_main C 0010\n_buf D 0100\ndup D 0200\nseg@x S 0004\n");
    CB_CHECK(p->symbols().size() == 6u);   // (and read again, the same)
    CB_CHECK(p->symbols().at("xx") == (SymPlace{SymKind::Unknown, 0xC}));

    SymbolTable learnt = s;
    learnt.insert_or_assign("neg", SymPlace{SymKind::Code, -5});
    p->save_symbols(learnt);
    CB_CHECK(fs::read_text(p->path("SYMBOLS")).find("neg C -0005\n") != std::string::npos);

    fs::write_text(p->path("SYMBOLS"), "a C xyz\n");
    CB_CHECK_THROWS_AS(ParseError, p->symbols());
    stdfs::remove(p->path("SYMBOLS"));
    CB_CHECK(p->symbols().empty());   // (no SYMBOLS: none)
}

);

CB_TEST("exact.project.sources", [] {
    Folder f;
    const auto p = project::Project::open(f.dir());
    // one-function files (any case), whole files placed by the names of src/PLACES (whole.c holds MAIN, which is at
    // f0020), files with no place (utility.c matches "unplaced"), and src/ORDER's: its entry for a file that is not
    // there, and for an address that is not hex, are passed over
    const auto sources = p->sources();
    CB_CHECK_EQ(sources.size(), 6u);
    const std::vector<std::pair<std::int64_t, std::string>> want = {
        {-1, "utility.c"}, {0x10, "f00010.c"}, {0x20, "F00020.C"}, {0x20, "whole.c"}, {0x40, "whole.c"}, {0x60, "f00010.c"}};
    for (std::size_t i = 0; i < want.size() && i < sources.size(); ++i) {
        CB_CHECK_EQ(sources[i].at, want[i].first);
        CB_CHECK_EQ(base_name(sources[i].path), want[i].second);
    }
    CB_CHECK(!p->whole(p->path("f00010.c")));
    CB_CHECK(p->whole(p->path("fx0003.c")));   // (a name that is not an address: whole)
    CB_CHECK(p->whole(p->path("whole.c")));
    CB_CHECK(p->whole(p->path("x.asm")));

    const auto starts = p->starts();
    CB_CHECK(starts.has_value());
    CB_CHECK(*starts == (std::vector<std::uint32_t>{0x10, 0x20, 0x30}));   // (comments and blank lines left out, sorted)

    // the "also" set: its own folder and file pattern; what it does not say it has of the project (here, the switches)
    const auto sets = p->sets();
    CB_CHECK_EQ(sets.size(), 2u);
    CB_CHECK(sets[0].get() == p.get());
    CB_CHECK_EQ(base_name(sets[1]->src()), "asm");
    CB_CHECK(sets[1]->switches() == std::optional<std::string>("/Ox"));
    CB_CHECK(sets[1]->config().also.empty());
    CB_CHECK(sets[1]->config().toolchain == std::optional<std::string>("fake2"));
    const auto also = sets[1]->sources();
    CB_CHECK_EQ(also.size(), 1u);
    CB_CHECK_EQ(also[0].at, 0x40);
    CB_CHECK_EQ(base_name(also[0].path), "f00040.asm");

    fs::write_text(f.dir() / "starts.txt", "zz\n");
    CB_CHECK_THROWS_AS(ParseError, p->starts());
    CB_CHECK_THROWS_AS(NotFoundError, project::Project::open(f.dir() / "nope"));
}

);

CB_TEST("exact.project.sources_are_stable", [] {
    // (the lines of src/PLACES and src/ORDER are read from a text that has to outlive them: once they were not, and one
    // call in ten lost whole.c)
    Folder f;
    const auto p = project::Project::open(f.dir());
    const auto first = p->sources();
    for (int i = 0; i < 100; ++i) {
        const auto again = p->sources();
        CB_CHECK_EQ(again.size(), first.size());
        if (again.size() != first.size()) break;
    }
}

);

CB_TEST("exact.project.errors", [] {
    Folder f;
    stdfs::remove(f.dir() / "PROG.EXE");
    CB_CHECK_THROWS_AS(NotFoundError, project::Project::open(f.dir()));
    fs::write_file(f.dir() / "PROG.EXE", Bytes{'M', 'Z', 1, 0});   // (an MZ without its header)
    CB_CHECK_THROWS_AS(TruncatedError, project::Project::open(f.dir()));
    Bytes bad = mz_file(Bytes(16, 0), {{0, 0}, {2, 0}});
    bad.resize(0x20);   // (the relocation table is cut off)
    fs::write_file(f.dir() / "PROG.EXE", bad);
    CB_CHECK_THROWS_AS(TruncatedError, project::Project::open(f.dir()));
    fs::write_text(f.dir() / "project.json", "[1]");
    CB_CHECK_THROWS_AS(ParseError, project::Project::open(f.dir()));
    stdfs::remove(f.dir() / "project.json");
    CB_CHECK_THROWS_AS(IoError, project::Project::open(f.dir()));

    // what needs the services says so
    Folder g;
    const auto p = project::Project::open(g.dir());
    CB_CHECK_THROWS_AS(UsageError, p->toolchain());
    CB_CHECK_THROWS_AS(UsageError, p->headers());
    CB_CHECK_THROWS_AS(UsageError, p->own_end());
    CB_CHECK_THROWS_AS(UsageError, p->identify_services());
    CB_CHECK_THROWS_AS(UsageError, p->pascal_project());
}

);

CB_TEST("exact.project.far_code_and_flat_programs", [] {
    Folder f;
    // a far-code program: three segment words, one of them DGROUP's (0x10 paragraphs: image offset 0x100)
    Bytes image(0x300, 0);
    put_word(image, 0x04, 0x0020);   // a segment of code (offset 0x200)
    put_word(image, 0x08, 0x0010);   // DGROUP's
    put_word(image, 0x0C, 0x0020);   // the same code segment again
    put_word(image, 0x10, 0x0100);   // past the image: not a segment of it
    fs::write_file(f.dir() / "PROG.EXE", mz_file(image, {{0x04, 0}, {0x08, 0}, {0x0C, 0}, {0x10, 0}}));
    fs::write_text(f.dir() / "project.json", R"({"image": "PROG.EXE", "dgroup": 256, "code": "far"})");
    auto p = project::Project::open(f.dir());
    CB_CHECK(p->code_segments() == (std::vector<std::uint32_t>{0, 0x200}));
    CB_CHECK_EQ(p->dgroup(), 256);

    fs::write_text(f.dir() / "project.json", R"({"image": "PROG.EXE", "dgroup": 256, "code": "far", "code_segments": [0, 64]})");
    CB_CHECK(project::Project::open(f.dir())->code_segments() == (std::vector<std::uint32_t>{0, 64}));   // (given: not worked out)
    fs::write_text(f.dir() / "project.json", R"({"image": "PROG.EXE", "code_segments": "relocs", "data_end": "0x120", "dgroup": 256})");
    CB_CHECK(project::Project::open(f.dir())->code_segments() == (std::vector<std::uint32_t>{0, 0x200}));

    // no "dgroup": the words the relocations stand at hold two different paragraphs, so none is DGROUP's
    fs::write_text(f.dir() / "project.json", R"({"image": "PROG.EXE"})");
    CB_CHECK_EQ(project::Project::open(f.dir())->dgroup(), 0);

    // a program that is not an MZ file is its own image
    fs::write_file(f.dir() / "PROG.EXE", Bytes{1, 2, 3, 4});
    p = project::Project::open(f.dir());
    CB_CHECK_EQ(p->image().size(), 4u);
    CB_CHECK(!p->header().has_value());
    CB_CHECK(p->relocs().empty());
    CB_CHECK_EQ(p->dgroup(), 0);
    CB_CHECK(p->code_segments() == (std::vector<std::uint32_t>{0}));
}

);

CB_TEST("exact.project.packed_original", [] {
    Folder f;
    // EXEPACK's header: the program's entry and stack, the unpacked length, "RB", then (after the unpacker's code and its
    // message) sixteen counted groups of relocations
    Bytes image(0x60, 0);
    put_word(image, 0, 0x00);
    const char* message = "Packed file is corrupt";
    Bytes pk;
    const auto word = [&](std::uint16_t v) { pk.push_back(static_cast<std::uint8_t>(v)); pk.push_back(static_cast<std::uint8_t>(v >> 8)); };
    word(0x0010); word(0x0001); word(0x0000); word(0x0100); word(0x0020); word(0x0002); word(0x0040); word(0x4252);   // (ip cs mem size sp ss dest, "RB")
    pk.insert(pk.end(), message, message + 22);
    for (int group = 0; group < 16; ++group) {
        if (group == 0) { word(2); word(0x0004); word(0x0008); }
        else word(0);
    }
    fs::write_file(f.dir() / "PACKED.EXE", mz_file(pk, {}));
    fs::write_file(f.dir() / "PROG.EXE", mz_file(image, {{0x20, 0}, {0x10, 0}}));
    fs::write_text(f.dir() / "project.json", R"({"image": "PROG.EXE", "packed": "PACKED.EXE"})");
    const auto p = project::Project::open(f.dir());
    CB_CHECK(p->packed().has_value());
    CB_CHECK_EQ(p->packed()->ip, 0x10);
    CB_CHECK_EQ(p->packed()->sp, 0x20);
    CB_CHECK(p->packed()->relocs == (std::vector<std::uint32_t>{4, 8}));
    const project::StandinInput in = p->standin_input();
    CB_CHECK(in.relocations == (std::vector<std::uint32_t>{4, 8}));   // (the packed table's: the program's own would be 0x10, 0x20)
    CB_CHECK_EQ(in.program().relocations.size(), 2u);
    CB_CHECK_EQ(in.program().image.size(), 0x60u);
    CB_CHECK(in.compare.relocs != nullptr && in.compare.relocs->size() == 2u);   // (comparing reads the program's own table)
    const project::RecutInput cut = p->recut_input();
    CB_CHECK(cut.original().packed_relocs.has_value() && cut.original().packed_relocs->size() == 2u);
    CB_CHECK(cut.original().table.has_value());

    fs::write_text(f.dir() / "project.json", R"({"image": "PROG.EXE", "packed": "NOPE.EXE"})");
    CB_CHECK_THROWS_AS(NotFoundError, project::Project::open(f.dir()));
    fs::write_file(f.dir() / "NOPE.EXE", Bytes{1, 2, 3});   // (a file that is not packed: no original's table)
    CB_CHECK(!project::Project::open(f.dir())->packed().has_value());
}

);

CB_TEST("exact.project.views_of_the_stages", [] {
    Folder f;
    Profiles prof;
    toolchain::Toolchains tcs(prof.locations, nullptr);
    const auto services = services_of(tcs);
    const auto p = project::Project::open(f.dir(), services);

    // the toolchain, its profile's parts, the headers beside the sources (by the profile's extensions: ".H" is ".h")
    CB_CHECK_EQ(p->toolchain()->name(), "fake");
    CB_CHECK_EQ(p->toolchain()->variant(), "v1");
    CB_CHECK_EQ(p->toolchains().size(), 1u);
    CB_CHECK_EQ(p->toolchains()[0].label, "fake");
    const auto parts = p->profile_parts();
    CB_CHECK(parts->kit == std::optional<std::string>("kitx"));
    CB_CHECK_EQ(parts->data_order, "before_next_function");
    CB_CHECK(parts->inverse == std::optional<std::string>("x86"));
    CB_CHECK(!parts->carve.frames && parts->carve.calls);
    CB_CHECK(parts->carve.prologues == (std::vector<std::string>{"55 8b ec"}));
    CB_CHECK(parts->link.has_value() && parts->link->psp == 0x20 && parts->link->run == std::optional<std::string>("link.exe"));
    CB_CHECK(parts->communal.has_value() && parts->communal->para && parts->communal->segment == "c_common");
    CB_CHECK(p->profile_parts().get() == parts.get());   // (read once)
    const auto headers = p->headers();
    CB_CHECK_EQ(headers.size(), 2u);
    CB_CHECK(headers.contains("types.h") && headers.contains("TYPES2.H"));
    CB_CHECK_EQ(headers.at("types.h").size(), 15u);
    CB_CHECK_EQ(p->headers_with({".c"}).size(), 5u);   // (f00010.c F00020.C f0003.c whole.c utility.c: the extension is read in lower case)
    const modules::CompileOptions opts = p->compile_options();
    CB_CHECK_EQ(opts.headers.size(), 2u);
    CB_CHECK_EQ(opts.include.size(), 2u);
    CB_CHECK(opts.switches == std::optional<std::string>("/Ox"));
    CB_CHECK_EQ(p->batch_options().include.size(), 2u);

    // modules
    const auto modules_view = p->as_modules();
    CB_CHECK_EQ(base_name(modules_view->src()), "src");
    CB_CHECK_EQ(base_name(modules_view->path("MODULES")), "MODULES");
    CB_CHECK_EQ(modules_view->code_end(), 0x50u);
    CB_CHECK(modules_view->config_text("named_by") == std::optional<std::string>("x"));
    CB_CHECK(!modules_view->config_text("dgroup").has_value());
    CB_CHECK(!modules_view->config_text("nothing").has_value());
    CB_CHECK(modules_view->code_segments() == (std::vector<std::uint32_t>{0, 0x10}));
    CB_CHECK(modules_view->toolchain() != nullptr);
    CB_CHECK_EQ(modules_view->headers().size(), 2u);
    CB_CHECK_EQ(modules_view->include().size(), 2u);
    CB_CHECK_EQ(modules_view->image().size(), 96u);
    CB_CHECK_EQ(modules_view->symbols().size(), 6u);
    CB_CHECK_EQ(modules_view->also_sets().size(), 1u);
    CB_CHECK_EQ(base_name(modules_view->also_sets()[0]->src()), "asm");
    CB_CHECK(modules_view->also_sets()[0]->toolchain()->profile("name") == std::optional<std::string>("fake2"));
    const compare::Program program = modules_view->program();
    CB_CHECK_EQ(program.dgroup, 0x40);
    CB_CHECK_EQ(program.loadseg, 0x10u);
    CB_CHECK_EQ(program.code_base(0x25), 0x10u);
    CB_CHECK(program.relocs != nullptr && program.relocs->size() == 2u);

    // library: the profile's libraries for the model "M" (the kit's), then the project's (its own folder, or "KIT:path")
    const auto library_view = p->as_library();
    const auto libs = library_view->libraries();
    CB_CHECK_EQ(libs.size(), 3u);
    CB_CHECK_EQ(libs[0].tag, "T");
    CB_CHECK(libs[0].file == prof.locations.kits.at("kitx") / "lib" / "m.lib");
    CB_CHECK_EQ(libs[1].tag, "EM");
    CB_CHECK(libs[1].file == f.dir() / "em.lib");
    CB_CHECK_EQ(libs[2].tag, "");
    CB_CHECK(libs[2].file == prof.locations.kits.at("KIT") / "x.lib");
    const auto objects = library_view->kit_objects();
    CB_CHECK_EQ(objects.size(), 2u);
    CB_CHECK(objects[1] == prof.locations.kits.at("kitx") / "o2.obj");
    CB_CHECK_EQ(library_view->first_objects().size(), 1u);
    CB_CHECK(library_view->first_objects()[0] == prof.locations.kits.at("kitx") / "BIN.OBJ");
    CB_CHECK_EQ(library_view->library_gap(), 100u);
    CB_CHECK(library_view->library_start() == std::optional<std::uint32_t>(0x30));
    CB_CHECK_EQ(library_view->data_sizes().at("_iob"), 32u);
    CB_CHECK_EQ(library_view->data_pads().size(), 2u);
    CB_CHECK_EQ(library_view->src().filename().string(), "src");   // (as modules)

    CB_CHECK_EQ(p->as_data()->data_order(), "before_next_function");

    // ledger (identify reads the same): sets, sources (a file with no place has -1), toolchains with their labels
    const auto ledger_view = p->as_ledger();
    CB_CHECK_EQ(ledger_view->sets().size(), 2u);
    CB_CHECK_EQ(base_name(ledger_view->sets()[1]->src()), "asm");
    CB_CHECK_EQ(base_name(stdfs::path(ledger_view->exe())), "PROG.EXE");
    CB_CHECK(ledger_view->starts() == (std::vector<std::uint32_t>{0x10, 0x20, 0x30}));
    CB_CHECK(ledger_view->whole(p->path("whole.c")));
    CB_CHECK_EQ(ledger_view->sources().size(), 6u);
    CB_CHECK(ledger_view->sources()[0].at == std::optional<std::int64_t>(-1));
    CB_CHECK_EQ(ledger_view->toolchains().size(), 1u);
    CB_CHECK_EQ(ledger_view->toolchains()[0].label, "fake");
    CB_CHECK(ledger_view->compilands() == nullptr);
    CB_CHECK(ledger_view->config_text("toolchain") == std::optional<std::string>("fake"));
    ledger_view->save_symbols(ledger_view->symbols());   // (through the project)
    CB_CHECK(fs::read_text(p->path("SYMBOLS")).starts_with("# name, C"));
    const identify::Services id = p->identify_services();
    CB_CHECK(id.locations.profiles == prof.locations.profiles);
    id.set_switches("/Od");   // (identify's candidates are tried this way: every view reads the project's switches at the time)
    CB_CHECK(ledger_view->switches() == std::optional<std::string>("/Od"));
    CB_CHECK(p->as_modules()->switches() == std::optional<std::string>("/Od"));
    id.set_switches(std::nullopt);
    CB_CHECK(!ledger_view->switches().has_value());
    CB_CHECK(id.load("fake2", std::string("v1")) != nullptr);
    CB_CHECK(id.load("fake2", std::string("v1")).get() == p->sets()[1]->toolchain().get());   // (the one toolchain)

    // linkwhole: files with no place are not linked by place; "KIT:" and plain names
    const auto link_view = p->as_linkwhole();
    CB_CHECK_EQ(link_view->sources().size(), 5u);
    CB_CHECK_EQ(link_view->sets().size(), 2u);
    CB_CHECK(link_view->link().linker == std::optional<std::string>("LINK.EXE"));
    CB_CHECK(link_view->resolve("LINK.EXE") == f.dir() / "LINK.EXE");
    CB_CHECK(link_view->resolve("KIT:O.OBJ") == prof.locations.kits.at("KIT") / "O.OBJ");
    CB_CHECK(link_view->load_toolchain("fake2", std::string("v1")) != nullptr);
    CB_CHECK_EQ(link_view->exe(), fs::to_text(p->exe()));
    CB_CHECK_EQ(link_view->file().size(), 160u);

    // readable
    const auto readable_view = p->as_readable();
    CB_CHECK_EQ(readable_view->symbols().size(), 6u);
    CB_CHECK_EQ(base_name(readable_view->source_dir()), "src");
    fs::write_text(f.dir() / "asm" / "PLACES", "");   // (the project's "place_by" file, in the other folder)
    const auto other = readable_view->with_source_dir(f.dir() / "asm");
    CB_CHECK_EQ(base_name(other->source_dir()), "asm");
    CB_CHECK_EQ(other->sources().size(), 1u);   // (the project's pattern of file names, the other folder's files: asm/f00010.c)
    CB_CHECK_EQ(other->sources()[0].address, 0x10u);
}

);

CB_TEST("exact.project.draft_and_services", [] {
    Folder f;
    Profiles prof;
    toolchain::Toolchains tcs(prof.locations, nullptr);
    auto services = services_of(tcs);
    int carved = 0;
    services->carve = [&](project::Project&) {
        ++carved;
        return std::vector<project::CodeFunction>{{0x10, 0x10}, {0x30, 0x1C}, {0x20, 0x8}};
    };
    const auto p = project::Project::open(f.dir(), services);
    CB_CHECK_EQ(p->own_end(), 0x4Cu);   // (the end of the last of them, not the highest address)
    CB_CHECK_EQ(p->own_end(), 0x4Cu);
    CB_CHECK_EQ(carved, 1);             // (kept)
    CB_CHECK_EQ(p->functions().size(), 3u);
    CB_CHECK_EQ(p->as_modules()->own_end(), 0x4Cu);

    const auto d = p->as_draft();
    CB_CHECK_EQ(d->toolchain_name(), "fake");
    CB_CHECK(d->inverse_model_name() == std::optional<std::string>("x86"));
    CB_CHECK_EQ(d->functions().size(), 3u);
    CB_CHECK_EQ(d->functions()[1].first, 0x30u);
    CB_CHECK(d->source_addresses() == (std::set<std::uint32_t>{0x10, 0x20, 0x40, 0x60}));   // (-1 is no address)
    CB_CHECK_EQ(d->symbols().size(), 6u);
    CB_CHECK_EQ(d->image().size(), 96u);
    CB_CHECK_EQ(base_name(d->path("SYMBOLS")), "SYMBOLS");
    CB_CHECK_THROWS_AS(UsageError, d->load_inverse_model("x86"));

    services = services_of(tcs);
    std::string asked;
    services->inverse_model = [&](std::string_view name) {
        asked = std::string(name);
        return std::shared_ptr<draft::InverseModel>();
    };
    services->prove_texts = [&](draft::InverseModel&, const draft::ModelTexts&, std::uint32_t at, std::uint32_t length,
                                const std::map<std::uint32_t, std::string>& names) {
        draft::ProveOutcome out;
        out.source = std::format("{:x}/{:x}/{}", at, length, names.size());
        out.verdict = "identical";
        return out;
    };
    const auto d2 = project::Project::open(f.dir(), services)->as_draft();
    CB_CHECK(d2->load_inverse_model("x86") == nullptr);
    CB_CHECK_EQ(asked, "x86");
}

);

CB_TEST("exact.project.recut_and_set_entry", [] {
    Folder f;
    const auto p = project::Project::open(f.dir());
    const project::RecutInput in = p->recut_input();
    CB_CHECK(in.original().table.has_value());
    CB_CHECK_EQ(in.original().table->count, 2u);
    CB_CHECK_EQ(in.original().table->offset, 0x1Cu);
    CB_CHECK(!in.original().packed_relocs.has_value());
    CB_CHECK_EQ(in.original().file.size(), 160u);
    CB_CHECK_EQ(in.original().symbols.size(), 6u);
    const project::StandinInput st = p->standin_input();
    CB_CHECK(st.relocations == (std::vector<std::uint32_t>{0x10, 0x20}));
    CB_CHECK_EQ(st.program().compare.loadseg, 0x10u);
    CB_CHECK_EQ(st.program().image.size(), 96u);

    // what the link's fit does to project.json: one more entry of "data_sizes", the rest of the file as it was
    p->set_entry("data_sizes", "_buf", 64);
    CB_CHECK_EQ(p->config().data_sizes.at("_buf"), 64u);
    CB_CHECK_EQ(p->config().data_sizes.at("_iob"), 32u);
    p->set_entry("data_pads", "0200", 8);   // (a new key of an entry that exists)
    CB_CHECK_EQ(p->config().data_pads.size(), 3u);
    CB_CHECK_EQ(p->config().data_pads[2].address, 0x200u);
    p->set_entry("fresh", "k", 1);          // (an entry that does not: made)
    const auto again = project::Project::open(f.dir());
    CB_CHECK_EQ(again->config().data_sizes.at("_buf"), 64u);
    CB_CHECK_EQ(again->config().toolchain.value_or(""), "fake");
    CB_CHECK_EQ(again->config().link.switches, "/B");   // (the other entries are as they were)
    CB_CHECK_EQ(again->config().also.size(), 1u);
}

);

CB_TEST("exact.project.pascal_in_pieces", [] {
    Folder f;
    Profiles prof;
    toolchain::Toolchains tcs(prof.locations, nullptr);
    tcs.register_plugin("fakepascal", [](const toolchain::Profile& pr, const std::optional<std::string>& v, const toolchain::Context& c) {
        return std::static_pointer_cast<toolchain::Toolchain>(std::make_shared<FakePascal>(pr, v, c));
    });
    fs::write_text(f.dir() / "project.json", R"({"image": "PROG.EXE", "toolchain": "pas", "root": "pieces", "over": ["ov"], "map": "map.json",
        "procedures": "procs.json", "units": {"MAIN": 3}, "equivalent": "habits:one",
        "compilands": {"MAIN": {"dir": "main", "kind": "PROGRAM", "main": "0x20", "parts": 2, "unit": 3, "stackck": true},
                       "LIB": {"dir": "lib", "kind": "MODULE", "dirs": ["a", "b"], "equivalent": "habits:two"}}})");
    auto services = services_of(tcs);
    services->equivalents["habits:one"] = [](ByteView, ByteView, const std::set<std::uint32_t>&, std::uint32_t) { return pascal_join::Equivalence{true, {}}; };
    const auto p = project::Project::open(f.dir(), services);
    const project::Config& c = p->config();
    CB_CHECK_EQ(c.pascal.root, "pieces");
    CB_CHECK(c.pascal.over == (std::vector<std::string>{"ov"}));
    CB_CHECK(c.pascal.map == std::optional<std::string>("map.json"));
    CB_CHECK(!c.pascal.kinds.has_value());
    CB_CHECK_EQ(c.pascal.procedures, "procs.json");
    CB_CHECK_EQ(c.pascal.units.at("MAIN"), 3);
    CB_CHECK_EQ(c.pascal.compilands.size(), 2u);
    CB_CHECK_EQ(c.pascal.compilands.begin()->first, "MAIN");   // (in the file's order: they are run in it)
    const pascal_join::CompilandConfig& m = c.pascal.compilands.at("MAIN");
    CB_CHECK(m.kind == pascal_join::CompilandKind::Program);
    CB_CHECK_EQ(m.main_at, 0x20u);
    CB_CHECK_EQ(m.parts, 2);
    CB_CHECK(m.unit == std::optional<std::int64_t>(3));
    CB_CHECK(m.stackck && !m.dirs && !m.over);
    const pascal_join::CompilandConfig& l = c.pascal.compilands.at("LIB");
    CB_CHECK(l.kind == pascal_join::CompilandKind::Module);
    CB_CHECK(l.dirs == std::optional<std::vector<std::string>>(std::vector<std::string>{"a", "b"}));
    CB_CHECK(l.equivalent == std::optional<std::string>("habits:two"));

    const pascal_join::Project q = p->pascal_project();
    CB_CHECK(q.dir == p->dir());
    CB_CHECK(q.toolchain != nullptr);
    CB_CHECK_EQ(q.toolchain->helper_data(), 5u);
    CB_CHECK_EQ(q.image.size(), 96u);
    CB_CHECK_EQ(q.program.dgroup, 0x40);
    CB_CHECK_EQ(q.equivalents.size(), 1u);
    CB_CHECK_EQ(q.config.compilands.size(), 2u);
    const auto ledger_view = p->as_ledger();
    CB_CHECK(ledger_view->compilands() != nullptr);
    CB_CHECK_EQ(ledger_view->compilands()->config.compilands.size(), 2u);

    // a compiland needs its kind; a project whose toolchain does not compile Pascal has no Pascal view
    CB_CHECK_THROWS_AS(ParseError, project::parse_config(R"({"image": "a", "compilands": {"X": {"dir": "x"}}})"));
    fs::write_text(f.dir() / "project.json", R"({"image": "PROG.EXE", "toolchain": "fake2", "compilands": {"X": {"dir": "x", "kind": "PROGRAM"}}})");
    CB_CHECK_THROWS_AS(UsageError, project::Project::open(f.dir(), services)->pascal_project());
}

);
