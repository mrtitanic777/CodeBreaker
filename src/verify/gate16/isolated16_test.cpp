// Tests of the isolated 16-bit gate, on a real module (LZEXPAND.DLL: formats/testdata) used as its own rebuild, against the recorded answers of check()
// (testdata/isolated16.golden.json): every function equals itself, and
// beside the next function standing in for its rebuild each is told apart -- the first input and the first thing that differs -- with the callees answered
// or run for real (chained), with the inputs given, and for a name the tables do not know; plus the parts of the comparison (what a difference says), the
// tables as a document, the linker map and the threads the work is shared over.
//
// Design notes:
//   * a difference is worded plainly (`calls: original [a, b], ours [c]`; a byte result `byte 5`): the golden is compared with the quotes taken out;
//   * the scattered inputs and the guessed ones are drawn from our own generator (verify/lanes/win16), so a function that passes is `equivalent` for the same
//     reason and a function that differs is found on the same first input (the fixed ones come first);
//   * the shares of a big check are threads.
#include "verify/gate16/isolated16.hpp"

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "knowledge/package.hpp"

using namespace cb;
using namespace cb::store;
using namespace cb::verify;
using namespace cb::verify::gate16;
namespace sfs = std::filesystem;
namespace w16 = cb::emulate::windows16;
using nlohmann::ordered_json;

namespace {

const ordered_json& iso_golden() {
    static const ordered_json g = json::load(testing::data_path("verify/gate16/testdata/isolated16.golden.json"));
    return g;
}

sfs::path iso_module() { return testing::data_path("formats/testdata/lzexpand.dll"); }

struct IsoWorld {
    fs::ScopedTempDir dir{"cb-isolated16"};
    knowledge::Store store{dir.path() / "k.sqlite", knowledge::DataFolders::under(sfs::path(CB_SOURCE_DIR).parent_path())};
    knowledge::Knowledge kb{store};
    std::map<std::string, std::uint32_t> sizes = {{"KERNEL.@15", 6}, {"KERNEL.@84", 8}, {"KERNEL.@82", 8}, {"KERNEL.@17", 2},
                                                  {"KERNEL.@18", 2}, {"KERNEL.@81", 2}, {"KERNEL.@86", 8}, {"KERNEL.@85", 6}};

    Isolated16Spec spec() {
        Isolated16Spec s;
        s.environment = [this](emulate::native_trace::Governor& governor) {
            w16::Environment env;
            env.governor = &governor;
            env.models = w16::models_of(kb);
            env.argument_bytes = [this](std::string_view api) -> std::optional<std::uint32_t> {
                const auto it = sizes.find(std::string(api));
                if (it == sizes.end()) return std::nullopt;
                return it->second;
            };
            return env;
        };
        return s;
    }

    IsolatedTables tables() const {
        IsolatedTables t;
        for (const auto& [name, row] : iso_golden().at("funcs").items()) t.funcs.insert_or_assign(name, IsolatedTables::Function{row.at(0).get<std::int64_t>(), row.at(1).get<bool>()});
        const ordered_json& places = iso_golden().at("places");
        for (std::size_t i = 0; i < places.size(); ++i) t.place["f" + std::to_string(i)] = {places.at(i).at(0).get<std::uint32_t>(), places.at(i).at(1).get<std::uint32_t>()};
        return t;
    }

    // A build folder whose linker map puts f0 ... where `where` says.
    sfs::path folder(const std::string& label, const ordered_json& where) {
        const sfs::path d = dir.path() / label;
        std::string map = "Segment Offset Symbol\n";
        for (std::size_t i = 0; i < where.size(); ++i) map += std::format("{:04x}:{:08x}  _f{}\n", where.at(i).at(0).get<std::uint32_t>(), where.at(i).at(1).get<std::uint32_t>(), i);
        fs::write_text(d / "m.map", map);
        return d;
    }
};

std::string iso_unquoted(std::string s) {
    s.erase(std::remove(s.begin(), s.end(), '\''), s.end());
    return s;
}

void iso_compare(const std::string& label, const std::vector<Isolated16Answer>& got, const ordered_json& want) {
    CB_CHECK_EQ(got.size(), want.size());
    for (std::size_t i = 0; i < got.size() && i < want.size(); ++i) {
        const ordered_json& w = want.at(i);
        testing::count_check();
        if (got[i].name != w.at(0).get<std::string>() || std::string(word(got[i].verdict)) != w.at(1).get<std::string>() ||
            got[i].why != iso_unquoted(w.at(2).get<std::string>())) {
            testing::record_failure(__FILE__, __LINE__, std::format("{} {}: the gate said {} / {} but expected {} / {}", label, got[i].name, word(got[i].verdict), got[i].why,
                                                                   w.at(1).get<std::string>(), iso_unquoted(w.at(2).get<std::string>())));
        }
    }
}

std::vector<std::string> iso_names(std::size_t from, std::size_t to) {
    std::vector<std::string> out;
    for (std::size_t i = from; i < to; ++i) out.push_back("f" + std::to_string(i));
    return out;
}

}  // namespace

CB_TEST("verify.isolated16.every_function_beside_itself_and_beside_another_is_the_recorded_verdict", [] {
    IsoWorld w;
    const Isolated16 gate(w.spec());
    const IsolatedTables tables = w.tables();
    const std::size_t n = iso_golden().at("names").size();
    const sfs::path own = w.folder("own", iso_golden().at("places"));
    const sfs::path swapped = w.folder("swapped", iso_golden().at("swapped_places"));
    Isolated16Options options;
    options.image = iso_module();
    options.tables = tables;
    options.workers = 1;
    iso_compare("own", gate.check(iso_module(), own, iso_names(0, n), options), iso_golden().at("cases").at("own"));
    iso_compare("swapped", gate.check(iso_module(), swapped, iso_names(0, n), options), iso_golden().at("cases").at("swapped"));
    Isolated16Options chained = options;
    chained.chain = true;
    iso_compare("own chained", gate.check(iso_module(), own, iso_names(0, 4), chained), iso_golden().at("cases").at("own_chain"));
    iso_compare("swapped chained", gate.check(iso_module(), swapped, iso_names(0, 4), chained), iso_golden().at("cases").at("swapped_chain"));
    Isolated16Options given = options;
    const std::vector<lanes::Vector16> fixed = lanes::win16_vectors();
    given.inputs = std::vector<lanes::Vector16>{fixed[0], fixed[3], fixed[8]};
    iso_compare("swapped, inputs given", gate.check(iso_module(), swapped, iso_names(0, 5), given), iso_golden().at("cases").at("swapped_given"));
    std::vector<std::string> odd = {"nope", "f0"};
    iso_compare("unknown name", gate.check(iso_module(), own, odd, options), iso_golden().at("cases").at("unknown"));
});

CB_TEST("verify.isolated16.the_work_is_shared_over_threads_and_the_answers_come_in_the_order_asked", [] {
    IsoWorld w;
    const Isolated16 gate(w.spec());
    const std::size_t n = iso_golden().at("names").size();
    const sfs::path swapped = w.folder("swapped", iso_golden().at("swapped_places"));
    Isolated16Options options;
    options.image = iso_module();
    options.tables = w.tables();
    options.workers = 2;   // more names than twice the workers: shared out
    std::vector<std::string> names = iso_names(0, n);
    std::reverse(names.begin(), names.end());
    const std::vector<Isolated16Answer> got = gate.check(iso_module(), swapped, names, options);
    CB_CHECK_EQ(got.size(), n);
    options.workers = 1;
    const std::vector<Isolated16Answer> serial = gate.check(iso_module(), swapped, names, options);
    for (std::size_t i = 0; i < n; ++i) {
        CB_CHECK_EQ(got[i].name, names[i]);
        CB_CHECK(got[i].verdict == serial[i].verdict);
        CB_CHECK_EQ(got[i].why, serial[i].why);
    }
});

CB_TEST("verify.isolated16.the_image_defaults_to_the_file_named_as_the_original_and_the_tables_to_the_spec", [] {
    IsoWorld w;
    Isolated16Spec spec = w.spec();
    bool asked = false;
    spec.tables = [&](const sfs::path&) {
        asked = true;
        return w.tables();
    };
    const Isolated16 gate(spec);
    const sfs::path own = w.folder("own", iso_golden().at("places"));
    sfs::copy_file(iso_module(), own / "LZEXPAND.DLL");   // (the file is found by name, in any case)
    const auto got = gate.check(iso_module(), own, {"f0"});
    CB_CHECK(asked);
    CB_CHECK_EQ(got.size(), std::size_t(1));
    CB_CHECK(got[0].verdict == Verdict::Equivalent);
    // no such file in the folder, no tables to ask: said
    const sfs::path empty = w.folder("empty", ordered_json::array());
    CB_CHECK_THROWS_AS(NotFoundError, (void)gate.check(iso_module(), empty, {"f0"}));
    CB_CHECK_THROWS_AS(UsageError, (void)Isolated16(w.spec()).check(iso_module(), own, {"f0"}));
});

CB_TEST("verify.isolated16.the_linker_map_gives_each_name_its_first_place", [] {
    fs::ScopedTempDir dir("cb-isolated16-map");
    fs::write_text(dir.path() / "m.map",
                   "Address        Symbol\n"
                   "0001:0000049e  _Alpha@8\n"
                   "0002:00000010+ Beta\n"
                   "0003:0010*     _gamma_one\n"
                   "0001:00000000  _Alpha\n"
                   "junk line\n"
                   "0004:zz        _bad\n"
                   "0005:00000020  _delta   \n");
    const auto places = built_places(dir.path());
    CB_CHECK_EQ(places.size(), std::size_t(4));
    CB_CHECK_EQ(places.at("Alpha").seg, 1u);
    CB_CHECK_EQ(places.at("Alpha").off, 0x49eu);   // the first place a name is given
    CB_CHECK_EQ(places.at("Beta").seg, 2u);
    CB_CHECK_EQ(places.at("gamma_one").off, 0x10u);
    CB_CHECK_EQ(places.at("delta").seg, 5u);
    CB_CHECK_THROWS_AS(NotFoundError, (void)built_places(dir.path() / "none"));
});

CB_TEST("verify.isolated16.the_tables_are_a_document_and_back", [] {
    IsolatedTables t;
    t.funcs.insert_or_assign("b", IsolatedTables::Function{8, true});
    t.funcs.insert_or_assign("a", IsolatedTables::Function{-1, false});
    t.wide = {"b"};
    t.place["a"] = {2, 0x10};
    t.place["b"] = {3, 0x20};
    const knowledge::Value doc = t.to_value();
    const IsolatedTables back = IsolatedTables::from_value(doc);
    CB_CHECK_EQ(back.funcs.size(), std::size_t(2));
    CB_CHECK(back.funcs.at("b") == t.funcs.at("b"));
    CB_CHECK(back.funcs.at("a") == t.funcs.at("a"));
    CB_CHECK_EQ(back.funcs.begin()->first, "b");   // (in the order given)
    CB_CHECK(back.wide == t.wide);
    CB_CHECK_EQ(back.place.at("b").off, 0x20u);
    CB_CHECK_THROWS_AS(FormatError, (void)IsolatedTables::from_value(knowledge::Value("not a document")));
    CB_CHECK_THROWS_AS(FormatError, (void)IsolatedTables::from_value(knowledge::Value(knowledge::Value::Map{{"funcs", knowledge::Value(knowledge::Value::Map{{"x", knowledge::Value("y")}})},
                                                                                                           {"wide", knowledge::Value(knowledge::Value::List{})},
                                                                                                           {"place", knowledge::Value(knowledge::Value::Map{})}})));
});

CB_TEST("verify.isolated16.what_a_difference_says", [] {
    GatePrint a, b;
    a.exit = b.exit = "ret";
    a.result_kind = b.result_kind = GatePrint::ResultKind::Number;
    a.result = b.result = 5;
    a.pop = b.pop = 8;
    a.kept = b.kept = std::array<bool, 3>{true, true, true};
    CB_CHECK(!gate_difference(a, b).has_value());
    GatePrint c = b;
    c.exit = "fault";
    CB_CHECK_EQ(gate_difference(a, c).value(), "ends: original ret, ours fault");
    c = b;
    a.calls = {"x()", "y()", "z()"};
    c.calls = {"x()", "w()"};
    CB_CHECK_EQ(gate_difference(a, c).value(), "calls: original [y(), z()], ours [w()]");
    // a run cut off at the step limit says nothing past where it stopped: the calls up to the shorter run's last are what compares
    c.exit = "truncated";
    c.calls = {"x()", "y()"};
    CB_CHECK(!gate_difference(a, c).has_value());
    c.calls = {"x()", "q()"};
    CB_CHECK_EQ(gate_difference(a, c).value(), "calls: original [y(), z()], ours [q()]");
    a.calls.clear();
    // what it leaves in the data segment and through a far pointer
    c = b;
    a.data = {{4, std::uint8_t{1}}, {6, std::uint8_t{2}}};
    c.data = {{4, std::uint8_t{1}}, {6, std::uint8_t{3}}, {8, std::uint8_t{4}}};
    CB_CHECK_EQ(gate_difference(a, c).value(), "leaves in the data segment: original [0006=2, 0008=none], ours [0006=3, 0008=4]");
    a.data.clear();
    c.data.clear();
    a.far = {{0x5000, std::uint8_t{7}}};
    CB_CHECK_EQ(gate_difference(a, c).value(), "leaves through a far pointer: original [5000=7], ours [5000=none]");
    a.far.clear();
    // the result: a number, a function that sets none matches anything, a byte matches by its low byte
    c.result = 6;
    CB_CHECK_EQ(gate_difference(a, c).value(), "returns: original 5, ours 6");
    a.result_kind = GatePrint::ResultKind::NoResult;
    CB_CHECK(!gate_difference(a, c).has_value());
    a.result_kind = GatePrint::ResultKind::Byte;
    a.result = 0x12;
    c.result = 0x3412;
    c.result_kind = GatePrint::ResultKind::Number;
    CB_CHECK(!gate_difference(a, c).has_value());
    c.result = 0x3413;
    CB_CHECK_EQ(gate_difference(a, c).value(), "returns: original byte 12, ours 3413");
    c.result_kind = GatePrint::ResultKind::DataSelector;
    CB_CHECK(gate_difference(a, c).has_value());
    // the stack and the registers
    a = b;
    c = b;
    c.pop = 10;
    CB_CHECK_EQ(gate_difference(a, c).value(), "removes argument bytes: original 8, ours 10");
    c = b;
    c.kept = std::array<bool, 3>{true, false, true};
    CB_CHECK_EQ(gate_difference(a, c).value(), "keeps SI, DI, DS: original kept/kept/kept, ours kept/changed/kept");
});

CB_TEST("verify.isolated16.an_input_as_a_verdict_names_it", [] {
    lanes::Vector16 v;
    v.args = {0x10, 0x20, 0xFFFF, 0x4, 0x99};
    CB_CHECK_EQ(gate_input_text(v), "args 10 20 ffff 4");
    v.guided = true;
    v.stub_ret = 0x1234;
    v.mem.push_back({w16::DataOffset{0x1c}, Bytes{1, 2}});
    v.mem.push_back({std::uint32_t{0xB00010}, Bytes{0xAB}});
    v.mem.push_back({w16::DataOffset{0x99}, Bytes{9}});   // (the first two only)
    CB_CHECK_EQ(gate_input_text(v), "guided args 10 20 ffff 4, DS:001c=0102, b00010=ab, windows returns 1234");
});
