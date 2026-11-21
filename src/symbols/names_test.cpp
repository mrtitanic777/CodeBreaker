// cb-unity: alone (test helpers named for this module)
// Tests of cb/symbols/names on real modules (LZEXPAND.DLL, 16-bit, and LZ32.DLL, 32-bit, from formats/testdata), and on symbol files made here a
// byte at a time so that each field the reader trusts is one this file chose. The expected values were worked out from the file formats and from
// the listings the other modules' goldens carry (LZEXPAND's exports: LZINIT at segment 2 offset 0, LZSEEK at 2:02ee ...).
//
// Addresses are unsigned 64-bit and records are structs; the caches (the .SYM index, the debug names of each module) belong to the
// Namer; the .SYM files of a name are tried in the order of their paths; a damaged .SYM or CodeView block gives no names.
#include "symbols/names.hpp"

#include "analyze/function_evidence.hpp"
#include "knowledge/store.hpp"
#include "store/knowledge_bodies.hpp"
#include "store/knowledge_proposals.hpp"
#include "symbols/proposals.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "decompile/describe.hpp"
#include "formats/pe.hpp"

namespace nm = cb::symbols::names;
namespace ord = cb::symbols::ordinals;
namespace stdfs = std::filesystem;
using cb::Bytes;

namespace {

// ---- little-endian bytes ------------------------------------------------------------------------------------------------------------
void put8(Bytes& b, unsigned v) { b.push_back(static_cast<std::uint8_t>(v)); }
void put16(Bytes& b, unsigned v) {
    put8(b, v & 0xFF);
    put8(b, v >> 8);
}
void put32(Bytes& b, std::uint32_t v) {
    put16(b, v & 0xFFFF);
    put16(b, v >> 16);
}
void put_text(Bytes& b, std::string_view s) { b.insert(b.end(), s.begin(), s.end()); }
void put_pascal(Bytes& b, std::string_view s) {
    put8(b, static_cast<unsigned>(s.size()));
    put_text(b, s);
}

// ---- a CodeView NB09 block with one subsection of public symbols ---------------------------------------------------------------------------
struct Public {
    unsigned segment;
    std::uint32_t offset;
    std::string name;
};

// What follows `lead` bytes of padding: the marker, the directory's place, the symbols, then the directory with one entry. `kind` is the
// subsection's kind (0x129 public symbols, which carry a 16-byte header before their records).
Bytes codeview_block(std::size_t lead, const std::vector<Public>& symbols, unsigned kind = 0x129) {
    Bytes b(lead, 0xAA);
    const std::size_t base = b.size();
    put_text(b, "NB09");
    put32(b, 0);   // the directory's place, patched below
    const std::size_t subsection = b.size() - base;
    if (kind != 0x134) b.insert(b.end(), 16, 0);
    for (const Public& s : symbols) {
        put16(b, static_cast<unsigned>(11 + s.name.size()));   // what follows the length: record, offset, segment, type, name
        put16(b, 0x0203);
        put32(b, s.offset);
        put16(b, s.segment);
        put16(b, 0);
        put_pascal(b, s.name);
    }
    const std::size_t size = b.size() - base - subsection;
    const std::uint32_t directory = static_cast<std::uint32_t>(b.size() - base);
    for (int k = 0; k < 4; ++k) b[base + 4 + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(directory >> (8 * k));
    put16(b, 12);   // the directory's header (12 bytes): its own size, the size of an entry, how many, and room for more
    put16(b, 12);
    put32(b, 1);
    put32(b, 0);
    put16(b, kind);
    put16(b, 0);
    put32(b, static_cast<std::uint32_t>(subsection));
    put32(b, static_cast<std::uint32_t>(size));
    return b;
}

// ---- a .SYM file -----------------------------------------------------------------------------------------------------------------------------
struct SymbolSeg {
    unsigned number;
    std::string object;                                    // the name the linker gave the segment
    std::vector<std::pair<std::uint32_t, std::string>> symbols;
    bool wide = false;                                     // 32-bit values
};

Bytes sym_file(const std::vector<SymbolSeg>& segments) {
    Bytes file(0x20, 0);   // the header: the number of segments at 10, the first one's paragraph at 12
    file[10] = static_cast<std::uint8_t>(segments.size());
    file[12] = 2;          // the first segment is at 0x20
    for (std::size_t i = 0; i < segments.size(); ++i) {
        const SymbolSeg& s = segments[i];
        Bytes seg(0x40, 0);   // the segment's header, its name, then the table of symbol places at 0x40
        auto at16 = [&](std::size_t where, unsigned v) {
            seg[where] = static_cast<std::uint8_t>(v);
            seg[where + 1] = static_cast<std::uint8_t>(v >> 8);
        };
        at16(2, static_cast<unsigned>(s.symbols.size()));
        at16(4, 0x40);
        at16(6, s.number);
        seg[14] = s.wide ? 1 : 0;
        seg[20] = static_cast<std::uint8_t>(s.object.size());
        std::copy(s.object.begin(), s.object.end(), seg.begin() + 21);
        std::size_t place = 0x40 + 2 * s.symbols.size();
        Bytes table, bodies;
        for (const auto& [value, name] : s.symbols) {
            put16(table, static_cast<unsigned>(place + bodies.size()));
            if (s.wide) put32(bodies, value);
            else put16(bodies, value);
            put_pascal(bodies, name);
        }
        seg.insert(seg.end(), table.begin(), table.end());
        seg.insert(seg.end(), bodies.begin(), bodies.end());
        while (seg.size() % 16) seg.push_back(0);
        const std::size_t next = i + 1 < segments.size() ? file.size() / 16 + seg.size() / 16 : 0;
        at16(0, static_cast<unsigned>(next));
        file.insert(file.end(), seg.begin(), seg.end());
    }
    return file;
}

// ---- modules ---------------------------------------------------------------------------------------------------------------------------------
stdfs::path testdata(std::string_view name) { return cb::testing::data_path("formats/testdata/" + std::string(name)); }

// No other module's names: nothing is exported by anyone else.
class NoExports : public ord::ModuleExports {
public:
    std::optional<Table> table(std::string_view) const override { return std::nullopt; }
};

// A body store in memory; names_from_bodies answers what was set.
class MemoryBodies : public cb::store::BodyStore {
public:
    std::map<std::string, std::map<std::string, cb::store::Body>> held;   // by module sha
    std::map<std::string, cb::store::SharedName> shared;                    // answered for any module
    std::vector<cb::store::ModuleRef> modules;
    std::vector<std::pair<std::string, std::string>> named;                // (digest, name)

    void set_bodies(const cb::store::ModuleRef& module, const std::map<std::string, cb::store::Body>& by_key) override {
        modules.push_back(module);
        held[module.sha] = by_key;
    }
    std::map<std::string, cb::store::Body> bodies(std::string_view module) override {
        const auto at = held.find(std::string(module));
        return at == held.end() ? std::map<std::string, cb::store::Body>{} : at->second;
    }
    void name_body(std::string_view digest, std::string_view name, std::string_view) override { named.emplace_back(std::string(digest), std::string(name)); }
    std::map<std::string, cb::store::SharedName> names_from_bodies(std::string_view) override { return shared; }
    cb::store::BodyCounts shared_bodies() override { return {7, 5, 2, 4}; }
};

struct World {
    cb::fs::ScopedTempDir dir{"cbnames"};
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    std::optional<cb::fs::ScopedEnv> root_env;
    std::unique_ptr<cb::base::Corpus> corpus;
    NoExports no_exports;
    ord::ShippedTables shipped;
    std::unique_ptr<ord::Ordinals> ordinals;
    MemoryBodies bodies;
    stdfs::path lz16, lz32;
    explicit World(ord::ShippedTables tables = {}) : shipped(std::move(tables)) {
        cb::fs::make_dirs(dir.path() / "corpus");
        cb::fs::make_dirs(dir.path() / "reference");
        root_env.emplace("CB_CORPUS_ROOT", cb::fs::to_text(dir.path() / "corpus"));
        cb::base::Products products;
        products.target = "win95";
        products.target_folder = dir.path() / "corpus";
        products.checkout_root = dir.path() / "CodeBreaker";
        products.state = dir.path() / "state";
        products.reference = dir.path() / "reference";
        cb::base::FormatProbes probes;
        probes.lane = [](const stdfs::path& p) -> std::optional<cb::base::Lane> {
            const std::string name = cb::text::lower(cb::fs::to_text(p.filename()));
            if (name == "lzexpand.dll") return cb::base::Lane::Ne;
            if (name == "lz32.dll") return cb::base::Lane::Pe;
            return std::nullopt;
        };
        corpus = std::make_unique<cb::base::Corpus>(products, probes);
        ordinals = std::make_unique<ord::Ordinals>(no_exports, shipped);
        lz16 = dir.path() / "corpus" / "lzexpand.dll";
        lz32 = dir.path() / "corpus" / "lz32.dll";
        cb::fs::write_file(lz16, cb::fs::read_file(testdata("lzexpand.dll")));
        cb::fs::write_file(lz32, cb::fs::read_file(testdata("lz32.dll")));
    }
    nm::Namer::Services services(bool with_bodies = true) {
        nm::Namer::Services s{*corpus, *ordinals};
        if (with_bodies) s.bodies = &bodies;
        return s;
    }
    void put_reference(std::string_view file, const Bytes& content) { cb::fs::write_file(dir.path() / "reference" / std::string(file), content); }
};

}  // namespace

// ---- keys and digests ----------------------------------------------------------------------------------------------------------------------

CB_TEST("symbols.names.a_function_is_keyed_as_the_store_keys_it", [] {
    CB_CHECK_EQ(nm::function_key(cb::base::Lane::Pe, 0x7FE0183A), "7fe0183a");
    CB_CHECK_EQ(nm::function_key(cb::base::Lane::Pe, 0x10A3C), "10a3c");
    CB_CHECK_EQ(nm::function_key(cb::base::Lane::Ne, static_cast<nm::Address>(cb::decompile::describe::address(1, 0x4A2))), "1:04a2");
    CB_CHECK_EQ(nm::function_key(cb::base::Lane::Ne, static_cast<nm::Address>(cb::decompile::describe::address(12, 0x10))), "12:0010");
    CB_CHECK_EQ(nm::function_key(cb::base::Lane::Le, 0x400), "400");
});

CB_TEST("symbols.names.a_body_is_digested_without_what_the_linker_fills_in", [] {
    Bytes code(40);
    for (std::size_t i = 0; i < code.size(); ++i) code[i] = static_cast<std::uint8_t>(i);
    // expected from the definition: sha1 of "<last>:" and the bytes up to last with the masked ones zeroed, its first 16 hex digits
    CB_CHECK(nm::body_digest(code, std::vector<std::uint32_t>{5, 6, 7, 8}, 30) == std::optional<std::string>("0cae8a818ed7cd34"));
    CB_CHECK(nm::body_digest(code, {}, 30) == std::optional<std::string>("395776cb3e54ebc5"));
    // a masked place past the end of the body is not part of it
    CB_CHECK(nm::body_digest(code, std::vector<std::uint32_t>{5, 6, 7, 8, 35}, 30) == std::optional<std::string>("0cae8a818ed7cd34"));
    CB_CHECK(nm::body_digest(code, {}, 24) == std::optional<std::string>("895bccfe1f78b7e7"));
    // too short to tell from anyone's: no digest
    CB_CHECK(!nm::body_digest(code, {}, 23));
    CB_CHECK(!nm::body_digest(code, {}, 0));
    CB_CHECK(!nm::body_digest(cb::ByteView(code).first(20), {}, 20));
    // a body that claims to be longer than the bytes there are is digested as far as they go, under the length it claims
    CB_CHECK(nm::body_digest(code, {}, 60) == std::optional<std::string>("9efae3ed733485e6"));
});

// ---- debug symbols -------------------------------------------------------------------------------------------------------------------------

CB_TEST("symbols.names.codeview_symbols_are_read_from_the_end_of_an_executable", [] {
    const Bytes block = codeview_block(10, {{1, 0x10, "_Alpha"}, {2, 0x20, "Beta"}, {1, 0x10, "Again"}, {0, 0x30, "NoSegment"}, {3, 0x40, ""}, {3, 0x44, "Last"}});
    const auto got = nm::codeview(block);
    CB_CHECK_EQ(got.size(), 3U);
    CB_CHECK_EQ(got.at({1, 0x10}), "_Alpha");   // the first at a place stays
    CB_CHECK_EQ(got.at({2, 0x20}), "Beta");
    CB_CHECK_EQ(got.at({3, 0x44}), "Last");     // a symbol with no segment or no name is no symbol
    // the global symbols' subsection has no header of its own to skip (0x134)
    const auto global = nm::codeview(codeview_block(0, {{1, 0x5, "G"}}, 0x134));
    CB_CHECK_EQ(global.size(), 1U);
    CB_CHECK_EQ(global.at({1, 5}), "G");
    // another kind of subsection is not looked into
    CB_CHECK(nm::codeview(codeview_block(0, {{1, 0x5, "G"}}, 0x120)).empty());
    // none, or damaged: nothing
    CB_CHECK(nm::codeview(Bytes(100, 0)).empty());
    CB_CHECK(nm::codeview({}).empty());
    for (std::size_t cut = 0; cut < block.size(); cut += 7) {
        const Bytes shorter(block.begin(), block.begin() + static_cast<std::ptrdiff_t>(cut));
        for (const auto& [place, name] : nm::codeview(shorter)) CB_CHECK(!name.empty());   // (whatever is read is whole: it never throws)
    }
    // the first block is damaged and the second is not: the second is the one
    Bytes two = codeview_block(0, {{1, 1, "Bad"}});
    for (std::size_t k = 4; k < 8; ++k) two[k] = 0xFF;   // the directory is said to be four gigabytes in
    const Bytes good = codeview_block(0, {{2, 0x22, "Good"}});
    two.insert(two.end(), good.begin(), good.end());
    const auto second = nm::codeview(two);
    CB_CHECK_EQ(second.size(), 1U);
    CB_CHECK_EQ(second.at({2, 0x22}), "Good");
});

CB_TEST("symbols.names.a_sym_file_lists_the_names_of_each_segment", [] {
    const Bytes file = sym_file({{1, "_MSGBOX", {{0x10, "Alpha"}, {0x2E0, "Beta"}}, false}, {2, "_TEXT", {{0x70000, "Wide"}}, true}, {3, "", {}, false}});
    const auto segments = nm::read_sym(file);
    CB_CHECK(segments.has_value());
    CB_CHECK_EQ(segments->size(), 3U);
    CB_CHECK_EQ((*segments)[0].number, 1U);
    CB_CHECK_EQ((*segments)[0].symbols.size(), 2U);
    CB_CHECK_EQ((*segments)[0].symbols.at(0x10), "Alpha");
    CB_CHECK_EQ((*segments)[0].symbols.at(0x2E0), "Beta");
    CB_CHECK_EQ((*segments)[1].number, 2U);
    CB_CHECK_EQ((*segments)[1].symbols.at(0x70000), "Wide");   // a 32-bit symbol file has 32-bit values
    CB_CHECK_EQ((*segments)[2].number, 3U);
    CB_CHECK((*segments)[2].symbols.empty());
    // the names the linker gave the segments: the objects it was made from; a segment with none has none
    const auto objects = nm::sym_objects(file);
    CB_CHECK_EQ(objects.size(), 2U);
    CB_CHECK_EQ(objects.at(1), "_MSGBOX");
    CB_CHECK_EQ(objects.at(2), "_TEXT");
    // not a symbol file, or damaged: none (a file that is too short to hold its own header is damaged)
    CB_CHECK(nm::read_sym(Bytes(8, 0)) == std::nullopt);
    CB_CHECK(nm::read_sym({}) == std::nullopt);
    CB_CHECK(nm::sym_objects({}).empty());
    CB_CHECK(nm::read_sym(Bytes(0x40, 0))->empty());   // no segments
    Bytes cut = file;
    cut.resize(0x20 + 0x45);   // inside the table of the first segment
    CB_CHECK(nm::read_sym(cut) == std::nullopt || nm::read_sym(cut)->size() <= 1U);
    // a chain of segments that loops back on itself ends where it would repeat
    Bytes loop = file;
    loop[0x20] = 2;   // the first segment's next is itself
    loop[10] = 9;
    CB_CHECK_EQ(nm::read_sym(loop)->size(), 1U);
});

// ---- the bodies of real modules --------------------------------------------------------------------------------------------------------------

CB_TEST("symbols.names.the_bodies_of_a_32_bit_module", [] {
    World w;
    nm::Namer namer(w.services());
    const auto bodies = namer.bodies(w.lz32);
    CB_CHECK(!bodies.empty());
    const auto carve = cb::formats::pe_functions::PeCarve::open(w.lz32);
    for (const auto& [address, digest] : bodies) {
        CB_CHECK(carve.functions().contains(static_cast<cb::formats::pe_functions::Va>(address)));   // only a function the carve found
        CB_CHECK_EQ(digest.size(), 16U);
        CB_CHECK(digest.find_first_not_of("0123456789abcdef") == std::string::npos);
    }
    CB_CHECK(bodies == nm::bodies32(carve));
    CB_CHECK(bodies == namer.bodies(w.lz32));   // the same twice
    CB_CHECK(bodies.size() <= carve.functions().size());
});

CB_TEST("symbols.names.the_bodies_of_a_16_bit_module", [] {
    World w;
    nm::Namer namer(w.services());
    const auto bodies = namer.bodies(w.lz16);
    CB_CHECK(!bodies.empty());
    for (const auto& [address, digest] : bodies) {
        const cb::core16::SegOff place = cb::decompile::describe::where(static_cast<cb::decompile::describe::Va>(address));
        CB_CHECK(place.seg >= 1 && place.seg <= 12);
        CB_CHECK_EQ(digest.size(), 16U);
    }
    // and none for a module that is neither (a file the corpus does not know the kind of)
    const stdfs::path other = w.dir.path() / "corpus" / "readme.txt";
    cb::fs::write_file(other, cb::to_bytes("not a module"));
    CB_CHECK(namer.bodies(other).empty());
});

CB_TEST("symbols.names.the_digests_are_the_ones_the_original_module_made", [] {
    // Golden: the recorded digests over the same two files (39 bodies of LZEXPAND, 15 of
    // LZ32), checked here for their count and for the first and last of each.
    World w;
    nm::Namer namer(w.services());
    const auto sixteen = namer.bodies(w.lz16);
    CB_CHECK_EQ(sixteen.size(), 39U);
    CB_CHECK_EQ(sixteen.at(0x10000000), "781e270f7e8b9f48");   // segment 1, offset 0 (selector 0x1000)
    CB_CHECK_EQ(sixteen.at(0x100000B2), "26ce7fb56b27c0a3");
    CB_CHECK_EQ(sixteen.at(0x10000116), "cac4b674a92170e1");
    CB_CHECK_EQ(sixteen.rbegin()->first, 0x10500319U);
    CB_CHECK_EQ(sixteen.rbegin()->second, "691a2ef9acdcface");
    const auto thirty_two = namer.bodies(w.lz32);
    CB_CHECK_EQ(thirty_two.size(), 15U);
    CB_CHECK_EQ(thirty_two.at(0xBFEA1000), "ee8148db58b3a42e");
    CB_CHECK_EQ(thirty_two.at(0xBFEA1057), "8b7ac35b3ea12584");
    CB_CHECK_EQ(thirty_two.at(0xBFEA1077), "e79cefadba5fb930");
    CB_CHECK_EQ(thirty_two.at(0xBFEA10BF), "a0de0e15da5f1a1f");
    CB_CHECK_EQ(thirty_two.rbegin()->first, 0xBFEA13D4U);
    CB_CHECK_EQ(thirty_two.rbegin()->second, "7e0e140fd5838550");
});

// ---- names -----------------------------------------------------------------------------------------------------------------------------------

CB_TEST("symbols.names.a_16_bit_module_names_what_it_exports", [] {
    World w;
    nm::Namer namer(w.services());
    const auto own = namer.own_names(w.lz16);
    const auto at = [](unsigned seg, unsigned off) { return static_cast<nm::Address>(cb::decompile::describe::address(seg, off)); };
    CB_CHECK_EQ(own.size(), 12U);   // the twelve exports that have names (the other eighteen are by number only)
    CB_CHECK(own.at(at(2, 0)) == (nm::NameSource{"LZINIT", "export"}));
    CB_CHECK(own.at(at(2, 0x2EE)) == (nm::NameSource{"LZSEEK", "export"}));
    CB_CHECK(own.at(at(4, 0x15C)) == (nm::NameSource{"LZCOPY", "export"}));
    CB_CHECK(own.at(at(3, 0)) == (nm::NameSource{"WEP", "export"}));
    CB_CHECK(own.at(at(11, 0x35E)) == (nm::NameSource{"___EXPORTEDSTUB", "export"}));
});

CB_TEST("symbols.names.an_export_the_module_leaves_unnamed_is_named_by_the_ordinal_table", [] {
    ord::ShippedTables shipped;
    shipped["LZEXPAND"] = {"lzexpand.def", 2, 0, {{3, "NotLzinit"}, {13, "Thirteen"}, {14, "Fourteen"}, {99, "NoSuchExport"}}};
    World w(shipped);
    nm::Namer namer(w.services());
    const auto own = namer.own_names(w.lz16);
    const auto at = [](unsigned seg, unsigned off) { return static_cast<nm::Address>(cb::decompile::describe::address(seg, off)); };
    CB_CHECK_EQ(own.size(), 14U);
    CB_CHECK(own.at(at(10, 0x124)) == (nm::NameSource{"Thirteen", "ordinal table (lzexpand.def)"}));   // @13
    CB_CHECK(own.at(at(10, 0x1B6)) == (nm::NameSource{"Fourteen", "ordinal table (lzexpand.def)"}));   // @14
    CB_CHECK(own.at(at(2, 0)).name == "LZINIT");   // the module's own name for @3 stands
});

CB_TEST("symbols.names.debug_symbols_are_used_only_for_the_build_they_are_of", [] {
    World w;
    const auto at = [](unsigned seg, unsigned off) { return static_cast<nm::Address>(cb::decompile::describe::address(seg, off)); };
    // LZEXPAND.SYM, as the linker would have written it for this build: its exports sit where the module's own table puts them (LZINIT 2:0000, LZSEEK
    // 2:02ee, LZREAD 2:03d6), and the build's internal functions are named too
    w.put_reference("LZEXPAND.SYM", sym_file({{2, "_LZCODE", {{0x0, "LZINIT"}, {0x2EE, "LZSEEK"}, {0x3D6, "LZREAD"}, {0x100, "_Helper"}, {0x180, "_Other"}}, false},
                                              {4, "_TEXT", {{0x0, "LZSTART"}, {0x40, "_Stub"}}, false}}));
    nm::Namer namer(w.services());
    const auto& debug = namer.debug_names(w.lz16);
    CB_CHECK_EQ(debug.at(at(2, 0x100)).name, "Helper");   // (the C decoration of a linker name is taken off)
    CB_CHECK_EQ(debug.at(at(2, 0x100)).source, "LZEXPAND.SYM");
    CB_CHECK_EQ(debug.at(at(2, 0x180)).name, "Other");
    CB_CHECK(debug.contains(at(2, 0)));   // (the exports are named too; the export table is the better source, see own_names)
    CB_CHECK(!debug.contains(at(4, 0x40)));   // segment 4: LZSTART sits at 4:0000 in the module too, but one is not two: not enough to show it is the build's
    // the file names the object each segment came from: only where the segment is named after one, and it is not a plain segment
    const auto& files = namer.source_files(w.lz16);
    CB_CHECK_EQ(files.at(at(2, 0x100)), "lzcode");
    CB_CHECK(!files.contains(at(4, 0x40)));   // _TEXT is nobody's object
    // the names the module gets: exports first, then the symbols
    const auto own = namer.own_names(w.lz16);
    CB_CHECK(own.at(at(2, 0)) == (nm::NameSource{"LZINIT", "export"}));
    CB_CHECK(own.at(at(2, 0x100)) == (nm::NameSource{"Helper", "debug symbols (LZEXPAND.SYM)"}));
});

CB_TEST("symbols.names.symbols_of_another_build_name_nothing", [] {
    World w;
    const auto at = [](unsigned seg, unsigned off) { return static_cast<nm::Address>(cb::decompile::describe::address(seg, off)); };
    // the same exports at other places: another build of the module (offsets move between builds)
    w.put_reference("LZEXPAND.SYM", sym_file({{2, "_LZCODE", {{0x10, "LZINIT"}, {0x300, "LZSEEK"}, {0x3E0, "LZREAD"}, {0x100, "_Helper"}}, false}}));
    nm::Namer namer(w.services());
    CB_CHECK(namer.debug_names(w.lz16).empty());
    CB_CHECK(namer.source_files(w.lz16).empty());
    CB_CHECK(!namer.own_names(w.lz16).contains(at(2, 0x100)));
    // two thirds agree and no fewer than two: the build's (one in three may be elsewhere)
    w.put_reference("LZEXPAND.SYM", sym_file({{2, "_LZCODE", {{0x0, "LZINIT"}, {0x2EE, "LZSEEK"}, {0x3E0, "LZREAD"}, {0x100, "_Helper"}}, false}}));
    nm::Namer another(w.services());
    CB_CHECK(another.debug_names(w.lz16).contains(at(2, 0x100)));
    // a damaged symbol file is passed over
    w.put_reference("LZEXPAND.SYM", Bytes(30, 0xFF));
    nm::Namer damaged(w.services());
    CB_CHECK(damaged.debug_names(w.lz16).empty());
});

CB_TEST("symbols.names.codeview_symbols_in_the_file_name_a_module", [] {
    // a 32-bit module with CodeView symbols appended: the module's exports are where they say, so the rest are named
    World w;
    const cb::formats::pe::Image image = cb::formats::pe::Image::open(w.lz32);
    const auto exports = image.exports();
    CB_CHECK(exports.size() >= 2);
    std::vector<Public> symbols;
    // the symbols' segments are the image's sections, one-based: the exports sit in the code section (the first)
    const auto& sections = image.sections();
    for (const auto& e : exports) {
        if (!e.name) continue;
        for (std::size_t s = 0; s < sections.size(); ++s)
            if (e.rva >= sections[s].virtual_address && e.rva < sections[s].virtual_address + sections[s].virtual_size) {
                symbols.push_back({static_cast<unsigned>(s + 1), e.rva - sections[s].virtual_address, *e.name});
                break;
            }
    }
    CB_CHECK(symbols.size() >= 2);
    symbols.push_back({1, 0x4, "_InternalOne"});
    Bytes file = cb::fs::read_file(w.lz32);
    const Bytes block = codeview_block(0, symbols);
    file.insert(file.end(), block.begin(), block.end());
    cb::fs::write_file(w.lz32, file);
    nm::Namer namer(w.services());
    const auto& debug = namer.debug_names(w.lz32);
    const nm::Address internal = image.header().image_base + sections[0].virtual_address + 4;
    CB_CHECK_EQ(debug.at(internal).name, "InternalOne");   // (the C decoration of the linker name is taken off)
    CB_CHECK_EQ(debug.at(internal).source, "CodeView symbols in the file");
    CB_CHECK_EQ(namer.own_names(w.lz32).at(internal).source, "debug symbols (CodeView symbols in the file)");
});

CB_TEST("symbols.names.a_powerpc_image_is_named_by_its_run_time_type_information", [] {
    World w;
    auto services = w.services();
    services.powerpc = [](const stdfs::path&) -> std::optional<nm::PowerPcNames> {
        nm::PowerPcNames n;
        n.entry = 0x82000100;
        n.methods = {{0x82001000, "CFoo", 0}, {0x82001010, "CFoo", 1}, {0x82001010 + 8, "ns::CBar<int>", 3}, {0x82002000, "CFoo", 0}, {0x82000100, "CEntry", 2}};
        return n;
    };
    // (the module is a 32-bit PE for the corpus; the hook is what says it is a PowerPC image, by the machine its header names)
    auto bytes = cb::fs::read_file(w.lz32);
    const std::uint32_t lfanew = static_cast<std::uint32_t>(bytes[0x3C] | (bytes[0x3D] << 8));
    bytes[lfanew + 4] = 0xF2;   // IMAGE_FILE_MACHINE_POWERPC
    bytes[lfanew + 5] = 0x01;
    const stdfs::path ppc = w.dir.path() / "corpus" / "lz32.dll";
    cb::fs::write_file(ppc, bytes);
    nm::Namer namer(services);
    const auto got = namer.own_names(ppc);
    CB_CHECK_EQ(got.size(), 4U);
    CB_CHECK(got.at(0x82000100) == (nm::NameSource{"entry", "entry point"}));   // the entry point keeps the entry's name over a virtual function's
    CB_CHECK(got.at(0x82001000) == (nm::NameSource{"CFoo_vf0", "run-time type information (CFoo, virtual function 0)"}));
    CB_CHECK(got.at(0x82001010).name == "CFoo_vf1");
    CB_CHECK(got.at(0x82001018) == (nm::NameSource{"ns_CBar_int__vf3", "run-time type information (ns::CBar<int>, virtual function 3)"}));   // each run of odd characters is one underscore
    CB_CHECK(!got.contains(0x82002000));   // CFoo_vf0 is taken
    // without a reader for such images nothing is named, and no function is found
    nm::Namer without(w.services());
    CB_CHECK(without.own_names(ppc).empty());
    CB_CHECK(without.of_module(ppc).functions.empty());
});

// ---- the best name of each function ----------------------------------------------------------------------------------------------------------

CB_TEST("symbols.names.every_function_has_a_name_from_the_best_source_that_has_one", [] {
    ord::ShippedTables shipped;
    shipped["LZEXPAND"] = {"lzexpand.def", 2, 0, {{13, "Thirteen"}}};
    World w(shipped);
    const auto at = [](unsigned seg, unsigned off) { return static_cast<nm::Address>(cb::decompile::describe::address(seg, off)); };
    const auto key = [&](unsigned seg, unsigned off) { return nm::function_key(cb::base::Lane::Ne, at(seg, off)); };
    // the body store knows a name for the function at 2:0000 (an export: the module's name wins), at 4:0000 (a name given by hand beats the export of
    // another... no: the export is the module's own, so it wins too), and for two functions the module names no other way
    w.bodies.shared = {{key(2, 0), {"FromBody", "named by hand"}},
                       {key(10, 0x124), {"HandNamed", "named by hand"}},
                       {key(8, 0xAA), {"SameBodyName", "the same body in other.dll"}}};
    nm::Namer namer(w.services());
    const nm::ModuleNames got = namer.of_module(w.lz16);
    CB_CHECK(!got.functions.empty());
    const auto find = [&](const std::string& k) -> const nm::FunctionName* {
        for (const nm::FunctionName& f : got.functions)
            if (f.key == k) return &f;
        return nullptr;
    };
    // the module's own export first
    const auto* init = find(key(2, 0));
    CB_CHECK(init != nullptr);
    CB_CHECK_EQ(init->name, "LZINIT");
    CB_CHECK_EQ(init->source, "export");
    CB_CHECK_EQ(init->address, at(2, 0));
    // a name given by hand beats the ordinal table
    const auto* hand = find(key(10, 0x124));
    CB_CHECK(hand != nullptr);
    CB_CHECK_EQ(hand->name, "HandNamed");
    CB_CHECK_EQ(hand->source, "named body (named by hand)");
    // the same body in another module is the last resort before sub_
    if (const auto* same = find(key(8, 0xAA))) {
        CB_CHECK_EQ(same->name, "SameBodyName");
        CB_CHECK_EQ(same->source, "the same body in other.dll");
    }
    // everything else is a placeholder
    std::size_t none = 0;
    for (const nm::FunctionName& f : got.functions) {
        CB_CHECK(!f.name.empty());
        if (f.source == "(none)") {
            ++none;
            CB_CHECK_EQ(f.name, "sub_" + cb::text::replace_all(f.key, ":", "_"));
        }
    }
    CB_CHECK(none > 0);
    // in address order, once each
    for (std::size_t i = 1; i < got.functions.size(); ++i) CB_CHECK(got.functions[i - 1].address < got.functions[i].address);
});

CB_TEST("symbols.names.a_runtime_library_function_is_named_for_its_library", [] {
    World w;
    nm::Namer first(w.services());
    const auto functions = first.of_module(w.lz16).functions;
    const auto target = std::find_if(functions.begin(), functions.end(), [](const nm::FunctionName& f) { return f.source == "(none)"; });
    CB_CHECK(target != functions.end());
    auto services = w.services();
    const nm::Address address = target->address;
    services.runtime_library = [address](const stdfs::path&) {
        return std::map<nm::Address, cb::symbols::runtime_library::Identified>{{address, {"_memcpy", "Microsoft C 6.0: SLIBCE"}}};
    };
    nm::Namer namer(services);
    for (const nm::FunctionName& f : namer.of_module(w.lz16).functions)
        if (f.address == address) {
            CB_CHECK_EQ(f.name, "_memcpy");
            CB_CHECK_EQ(f.source, "runtime library: Microsoft C 6.0: SLIBCE");
        }
});

CB_TEST("symbols.names.imports_by_name_and_by_number", [] {
    ord::ShippedTables shipped;
    shipped["KERNEL"] = {"kernel.def", 2, 0, {{91, "GetVersion"}}};
    World w(shipped);
    nm::Namer namer(w.services());
    const nm::ModuleNames got = namer.of_module(w.lz16);
    CB_CHECK(!got.imports.empty());
    bool by_number = false;
    for (const nm::ImportName& i : got.imports) {
        CB_CHECK(!i.module.empty());
        CB_CHECK(!i.name.empty());
        if (i.ordinal) {
            by_number = true;
            if (i.module == "KERNEL" && *i.ordinal == 91) {
                CB_CHECK_EQ(i.name, "GetVersion");
                CB_CHECK_EQ(i.source, "KERNEL's exports (kernel.def)");
            } else if (i.source == "(none)") {
                CB_CHECK_EQ(i.name, std::format("{}.{}", i.module, *i.ordinal));
            }
        } else {
            CB_CHECK_EQ(i.source, "imported by name");
        }
    }
    CB_CHECK(by_number);
    // sorted: the imports by name first (in the order met), then by number (by module and number)
    bool numbers_started = false;
    for (std::size_t k = 0; k < got.imports.size(); ++k) {
        if (got.imports[k].ordinal) numbers_started = true;
        else CB_CHECK(!numbers_started);
    }
});

CB_TEST("symbols.names.a_32_bit_module_names_its_exports_and_its_imports", [] {
    World w;
    nm::Namer namer(w.services());
    const nm::ModuleNames got = namer.of_module(w.lz32);
    CB_CHECK(!got.functions.empty());
    const cb::formats::pe::Image image = cb::formats::pe::Image::open(w.lz32);
    std::size_t exported = 0;
    for (const auto& e : image.exports()) {
        if (!e.name || e.name->empty()) continue;
        for (const nm::FunctionName& f : got.functions)
            if (f.address == image.header().image_base + e.rva) {
                CB_CHECK_EQ(f.name, *e.name);
                CB_CHECK_EQ(f.source, "export");
                CB_CHECK_EQ(f.key, std::format("{:x}", f.address));
                ++exported;
            }
    }
    CB_CHECK(exported > 0);
    CB_CHECK(!got.imports.empty());
    for (const nm::ImportName& i : got.imports) {
        CB_CHECK(!i.module.empty() && i.module == cb::text::upper(i.module));   // KERNEL32, not kernel32.dll
        if (!i.ordinal) CB_CHECK_EQ(i.source, "imported by name");
    }
    // sorted by module and then name, each once
    for (std::size_t k = 1; k < got.imports.size(); ++k) CB_CHECK(!(got.imports[k - 1].module == got.imports[k].module && got.imports[k - 1].name == got.imports[k].name));
});

CB_TEST("symbols.names.a_console_images_imports_are_named_by_the_hook", [] {
    World w;
    auto services = w.services();
    services.console_import_names = [](const stdfs::path&) {
        return std::map<std::string, std::string>{{"import3_ord_17", "XamFoo"}, {"import12_ord_5", "XamBar"}, {"strange", "Odd"}};
    };
    services.powerpc = [](const stdfs::path&) -> std::optional<nm::PowerPcNames> { return nm::PowerPcNames{}; };
    auto bytes = cb::fs::read_file(w.lz32);
    const std::uint32_t lfanew = static_cast<std::uint32_t>(bytes[0x3C] | (bytes[0x3D] << 8));
    bytes[lfanew + 4] = 0xF2;
    bytes[lfanew + 5] = 0x01;
    cb::fs::write_file(w.lz32, bytes);
    nm::Namer namer(services);
    const nm::ModuleNames got = namer.of_module(w.lz32);
    CB_CHECK_EQ(got.imports.size(), 3U);
    CB_CHECK_EQ(got.imports[0].module, "library 12");   // by symbol: import12_ord_5 sorts before import3_ord_17 as text
    CB_CHECK(got.imports[0].ordinal == std::optional<std::uint32_t>(5));
    CB_CHECK_EQ(got.imports[0].name, "XamBar");
    CB_CHECK_EQ(got.imports[1].module, "library 3");
    CB_CHECK(got.imports[1].ordinal == std::optional<std::uint32_t>(17));
    CB_CHECK_EQ(got.imports[2].module, "?");   // a symbol that is not of that form has no library
    CB_CHECK(!got.imports[2].ordinal.has_value());
    CB_CHECK_EQ(got.imports[2].source, "the console's system libraries (config/knowledge/xbox360/ordinals.json)");
});

// ---- the body store --------------------------------------------------------------------------------------------------------------------------

CB_TEST("symbols.names.a_modules_bodies_are_recorded_with_the_names_it_gives_them", [] {
    World w;
    nm::Namer namer(w.services());
    const std::size_t n = namer.index(w.lz16);
    CB_CHECK(n > 0);
    CB_CHECK_EQ(w.bodies.modules.size(), 1U);
    const cb::store::ModuleRef& m = w.bodies.modules[0];
    CB_CHECK_EQ(m.name, "lzexpand.dll");
    CB_CHECK_EQ(m.kind, "ne");
    CB_CHECK(m.sha == w.corpus->sha(w.lz16).value());
    CB_CHECK(m.path == cb::fs::to_text(w.lz16));
    const auto& rows = w.bodies.held.at(m.sha);
    CB_CHECK_EQ(rows.size(), n);
    for (const auto& [key, body] : rows) {
        CB_CHECK_EQ(body.digest.size(), 16U);
        CB_CHECK(key.find(':') != std::string::npos);   // a 16-bit module's keys are segment:offset
    }
    // a body of an export carries the export's name and says so
    for (const auto& [key, body] : rows)
        if (body.name) CB_CHECK(body.source.has_value());
    // asking for the names of the module records them when none are there, and not again
    nm::Namer second(w.services());
    w.bodies.modules.clear();
    (void)second.of_module(w.lz16);
    CB_CHECK(w.bodies.modules.empty());   // already recorded
    // without a body store there is nothing to record
    nm::Namer without(w.services(false));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)without.index(w.lz16));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)without.set_name(w.lz16, 0, "X"));
    CB_CHECK(!without.of_module(w.lz16).functions.empty());   // but the names are still found
});

CB_TEST("symbols.names.naming_a_body_names_it_everywhere", [] {
    World w;
    nm::Namer namer(w.services());
    const auto bodies = namer.bodies(w.lz16);
    CB_CHECK(!bodies.empty());
    const auto& [address, digest] = *bodies.begin();
    const auto got = namer.set_name(w.lz16, address, "StrChrHelper");
    CB_CHECK(got == std::optional<std::string>(digest));
    CB_CHECK_EQ(w.bodies.named.size(), 1U);
    CB_CHECK(w.bodies.named[0] == std::make_pair(digest, std::string("StrChrHelper")));
    // the bodies were recorded by the way
    CB_CHECK(!w.bodies.held.empty());
    // a function with no body worth naming gives nothing
    CB_CHECK(!namer.set_name(w.lz16, 12345, "Nothing").has_value());
    CB_CHECK_EQ(w.bodies.named.size(), 1U);
});

// ---- the command -----------------------------------------------------------------------------------------------------------------------------

CB_TEST("symbols.names.the_command_lists_every_function_and_import", [] {
    World w;
    auto services = w.services();
    services.compiler = [](const stdfs::path&) { return cb::symbols::runtime_library::CompilerInfo{"1.0", "Microsoft C 6.0", {}}; };
    nm::Namer namer(services);
    cb::testing::CapturedStd out;
    CB_CHECK_EQ(namer.run(std::vector<std::string>{"lzexpand.dll"}), 0);
    const std::string text = out.out();
    CB_CHECK(text.find("2:0000     LZINIT") != std::string::npos);
    CB_CHECK(text.find(" export\n") != std::string::npos);
    CB_CHECK(text.find("lzexpand.dll: ") != std::string::npos);
    CB_CHECK(text.find(", export 12\n") != std::string::npos || text.find("functions -- export 12") != std::string::npos);   // twelve are the module's own exports
    CB_CHECK(text.find(" imports -- ") != std::string::npos);
    CB_CHECK(text.find("built with: Microsoft C 6.0 (linker 1.0)\n") != std::string::npos);
    CB_CHECK(text.find("import ") != std::string::npos);
});

CB_TEST("symbols.names.the_command_names_a_body_and_counts_the_shared_ones", [] {
    World w;
    nm::Namer namer(w.services());
    cb::testing::CapturedStd out;
    CB_CHECK_EQ(namer.run(std::vector<std::string>{"--shared"}), 0);
    CB_CHECK_EQ(out.out(), "7 bodies recorded, 5 distinct; 2 bodies are in two or more modules (4 functions are such copies)\n");
    const auto bodies = namer.bodies(w.lz16);
    const auto& [address, digest] = *bodies.begin();
    const cb::core16::SegOff place = cb::decompile::describe::where(static_cast<cb::decompile::describe::Va>(address));
    const std::string spelled = std::format("{}:{:x}", place.seg, place.off);
    cb::testing::CapturedStd named;
    CB_CHECK_EQ(namer.run(std::vector<std::string>{"lzexpand.dll", "--set", spelled, "Renamed"}), 0);
    CB_CHECK_EQ(named.out(), std::format("body {} is now Renamed\n", digest));
    cb::testing::CapturedStd none;
    CB_CHECK_EQ(namer.run(std::vector<std::string>{"lzexpand.dll", "--set", "1:ffff", "Nothing"}), 1);
    CB_CHECK_EQ(none.out(), "that function has no body worth naming\n");
});

CB_TEST("symbols.names.the_command_refuses_what_it_cannot_do", [] {
    World w;
    nm::Namer namer(w.services());
    cb::testing::CapturedStd out;
    CB_CHECK_THROWS_AS(cb::UsageError, (void)namer.run(std::vector<std::string>{}));
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)namer.run(std::vector<std::string>{"nosuchmodule.dll"}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)namer.run(std::vector<std::string>{"lzexpand.dll", "--set", "1:0"}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)namer.run(std::vector<std::string>{"lzexpand.dll", "--set", "zz", "Name"}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)namer.run(std::vector<std::string>{"--ordinals", "a.def"}));   // nobody remakes the table here
    // when someone does, the files are handed over as given
    std::vector<stdfs::path> got;
    auto services = w.services();
    services.collect_ordinals = [&](std::span<const stdfs::path> files) { got.assign(files.begin(), files.end()); };
    nm::Namer with(services);
    CB_CHECK_EQ(with.run(std::vector<std::string>{"--ordinals", "a.def", "b.lib"}), 0);
    CB_CHECK_EQ(got.size(), 2U);
    CB_CHECK(got[1] == stdfs::path("b.lib"));
});

// ---- proposed names ---------------------------------------------------------------------------------------------------------------------------

namespace {

// The knowledge base's body store and proposal store over a folder of their own, and the real facts of a module (analyze/function_evidence).
struct ProposingWorld {
    World w;
    cb::knowledge::Store kb{w.dir.path() / "db" / "knowledge.v2.sqlite", cb::knowledge::DataFolders::under(stdfs::path(CB_SOURCE_DIR).parent_path())};
    cb::store::KnowledgeBodies bodies{kb};
    cb::store::KnowledgeProposals proposals{kb};

    nm::Namer::Services services(bool shared = true) {
        nm::Namer::Services s = w.services(false);
        if (shared) {
            s.bodies = &bodies;
            s.proposals = &proposals;
        }
        return s;
    }
    static std::vector<cb::symbols::FunctionEvidence> evidence(const stdfs::path& module) {
        static const cb::fs::ScopedTempDir state("names-evidence");   // (the 16-bit description's work files go under it)
        cb::analyze::report::Environment environment;
        environment.state = state.path();
        return cb::analyze::function_evidence::of_module(module, environment);
    }
    cb::symbols::proposals::Proposals::Services proposing(nm::Namer& namer) {
        return {namer, proposals, *w.corpus, [](const stdfs::path& module) { return evidence(module); }};
    }
};

std::set<std::string> names_of(const nm::ModuleNames& m) {
    std::set<std::string> out;
    for (const nm::FunctionName& f : m.functions) out.insert(f.name);
    return out;
}

}  // namespace

CB_TEST("symbols.names.proposals_over_the_real_modules_are_valid_unique_and_not_names", [] {
    ProposingWorld p;
    nm::Namer namer(p.services());
    cb::symbols::proposals::Proposals proposing(p.proposing(namer));
    for (const stdfs::path& module : {p.w.lz16, p.w.lz32}) {
        const nm::ModuleNames before = namer.of_module(module);
        const auto made = proposing.propose(module);
        std::set<std::string> seen;
        for (const auto& proposal : made) {
            CB_CHECK(cb::symbols::proposals::valid_name(proposal.name));
            CB_CHECK(seen.insert(proposal.name).second);              // one name to a function
            CB_CHECK(!names_of(before).contains(proposal.name));      // and not one a function has
            CB_CHECK_EQ(proposal.source, "proposal");
            CB_CHECK(!proposal.description.empty());
            CB_CHECK(!proposal.evidence.empty());
            CB_CHECK(proposal.confidence >= 0 && proposal.confidence <= 100);
        }
        // not names: the listing is the same but for what it says is proposed
        const nm::ModuleNames after = namer.of_module(module);
        CB_CHECK_EQ(after.functions.size(), before.functions.size());
        std::size_t shown = 0;
        for (std::size_t i = 0; i < after.functions.size(); ++i) {
            CB_CHECK_EQ(after.functions[i].name, before.functions[i].name);
            CB_CHECK_EQ(after.functions[i].source, before.functions[i].source);
            shown += after.functions[i].proposed.has_value() ? 1 : 0;
        }
        CB_CHECK_EQ(shown, made.size());
        CB_CHECK_EQ(proposing.propose(module).size(), made.size());   // proposing again finds the same
    }
});

CB_TEST("symbols.names.accepting_a_name_changes_what_names_shows_and_nothing_the_judge_reads", [] {
    ProposingWorld p;
    nm::Namer namer(p.services());
    cb::symbols::proposals::Proposals proposing(p.proposing(namer));
    // What the judge's side reads: the decompiler's namer has no body store (decompile/environment.cpp), and the facts the analyses read of the code.
    nm::Namer decompilers(p.services(false));
    for (const stdfs::path& module : {p.w.lz16, p.w.lz32}) {
        const nm::ModuleNames judge_before = decompilers.of_module(module);
        const auto evidence_before = ProposingWorld::evidence(module);
        const auto bodies_before = namer.bodies(module);
        const nm::ModuleNames shown_before = namer.of_module(module);

        const auto made = proposing.propose(module);
        const std::size_t taken = proposing.accept_all(module, 0);   // every one, whatever its confidence
        CB_CHECK_EQ(taken, made.size());

        // unchanged: the bytes' digests, the facts the analyses read, the decompiler's names
        CB_CHECK(bodies_before == namer.bodies(module));
        CB_CHECK(evidence_before == ProposingWorld::evidence(module));
        const nm::ModuleNames judge_after = decompilers.of_module(module);
        CB_CHECK_EQ(judge_before.functions.size(), judge_after.functions.size());
        for (std::size_t i = 0; i < judge_before.functions.size(); ++i) {
            CB_CHECK_EQ(judge_before.functions[i].name, judge_after.functions[i].name);
            CB_CHECK_EQ(judge_before.functions[i].source, judge_after.functions[i].source);
        }

        // changed: the names of exactly the functions that were proposed a name, and where they come from
        const nm::ModuleNames shown_after = namer.of_module(module);
        std::set<std::string> proposed_keys;
        for (const auto& proposal : made) proposed_keys.insert(proposal.key);
        std::size_t renamed = 0;
        for (std::size_t i = 0; i < shown_after.functions.size(); ++i) {
            const nm::FunctionName& was = shown_before.functions[i];
            const nm::FunctionName& is = shown_after.functions[i];
            if (!proposed_keys.contains(is.key)) {
                CB_CHECK_EQ(is.name, was.name);
                continue;
            }
            ++renamed;
            CB_CHECK(is.name != was.name);
            CB_CHECK(is.source.find("accepted proposal") != std::string::npos);
            CB_CHECK(!is.proposed.has_value());
        }
        CB_CHECK_EQ(renamed, made.size());
    }
});

CB_TEST("symbols.names.a_rejected_proposal_stays_rejected_and_is_not_made_again", [] {
    ProposingWorld p;
    nm::Namer namer(p.services());
    cb::symbols::proposals::Proposals proposing(p.proposing(namer));
    const auto made = proposing.propose(p.w.lz32);
    CB_CHECK(!made.empty());
    const std::string key = made.front().key;
    const auto address = *nm::parse_address(key);
    CB_CHECK(proposing.reject(p.w.lz32, address));
    CB_CHECK(!proposing.reject(p.w.lz32, address));   // (once)
    CB_CHECK(!proposing.accept(p.w.lz32, address));
    CB_CHECK_EQ(proposing.propose(p.w.lz32).size(), made.size() - 1);
    CB_CHECK(p.proposals.get(namer.module_sha(p.w.lz32), key)->state == cb::store::ProposalState::Rejected);
});

CB_TEST("symbols.names.an_assistants_suggestion_is_stored_apart_and_checked", [] {
    ProposingWorld p;
    nm::Namer namer(p.services());
    cb::symbols::proposals::Proposals proposing(p.proposing(namer));
    // a function nothing names (LZEXPAND's 1:0000)
    const auto address = static_cast<nm::Address>(cb::decompile::describe::address(1, 0));
    const auto stored = proposing.suggest(p.w.lz16, address, "InitHeader", "Reads the file header.", 70);
    CB_CHECK_EQ(stored.rule, "assistant");
    CB_CHECK_EQ(stored.confidence, 70);
    CB_CHECK(stored.state == cb::store::ProposalState::Proposed);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)proposing.suggest(p.w.lz16, address, "bad name", "", 70));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)proposing.suggest(p.w.lz16, address, "Fine", "", 101));
    // an exported function has its name already
    const auto exported = static_cast<nm::Address>(cb::decompile::describe::address(2, 0));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)proposing.suggest(p.w.lz16, exported, "Other", "", 50));
    // a name another function has
    const auto second = static_cast<nm::Address>(cb::decompile::describe::address(1, 0xb2));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)proposing.suggest(p.w.lz16, second, "InitHeader", "", 50));
    // the rules leave an assistant's proposal alone
    (void)proposing.propose(p.w.lz16);
    CB_CHECK_EQ(p.proposals.get(namer.module_sha(p.w.lz16), namer.key_of(p.w.lz16, address))->rule, "assistant");
    CB_CHECK(proposing.accept(p.w.lz16, address));
    CB_CHECK_EQ(namer.of_module(p.w.lz16).functions.front().name, "InitHeader");
});

CB_TEST("symbols.names.the_proposal_options_need_what_they_are_given", [] {
    World w;
    nm::Namer namer(w.services());
    cb::testing::CapturedStd out;
    CB_CHECK_THROWS_AS(cb::UsageError, (void)namer.run(std::vector<std::string>{"lzexpand.dll", "--propose"}));
});
