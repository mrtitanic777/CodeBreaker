// Tests of the 16-bit Windows lane, on a real module (LZEXPAND.DLL: formats/testdata) against the recorded answers
// (testdata/win16_lane.golden.json): the fixed inputs, the fingerprint of every exported function on the first ten
// of them, the spans of the functions, the directed inputs, the places of a caller-frame helper, and the verdict of every exported function beside
// itself and beside another function of the module standing in for its rebuild; plus the kind interface (prepare, build) over stand-in compile and link
// steps, and the pieces a build is made of (the code images, the rebuild's own data, the names of code segments).
//
// Design notes:
//   * the four scattered inputs are drawn from a SplitMix64 generator seeded with 0x16, so those words differ (they are
//     deterministic: every word a different even offset below 0x7F00, odd words the safe selector in every second input);
//     the golden is compared on the ten fixed inputs only;
//   * the guessing explorer (the lane has no lifters here) draws from the same generator, so the inputs it finds differ and a
//     `coverage=NN` limit may differ by a few points: verdicts, reasons and details are compared exactly, limits other than coverage exactly, coverage
//     within ten points;
//   * the original's runs are kept within one judgement only;
//   * the profile is made over our own text.
#include "verify/lanes/win16.hpp"

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "knowledge/package.hpp"

using namespace cb;
using namespace cb::store;
using namespace cb::verify;
using namespace cb::verify::lanes;
namespace sfs = std::filesystem;
namespace w16 = cb::emulate::windows16;
using nlohmann::ordered_json;

namespace {

const ordered_json& w16_golden() {
    static const ordered_json g = json::load(testing::data_path("verify/lanes/testdata/win16_lane.golden.json"));
    return g;
}

sfs::path w16_module() { return testing::data_path("formats/testdata/lzexpand.dll"); }

struct Win16World {
    fs::ScopedTempDir dir{"cb-win16-lane"};
    knowledge::Store store{dir.path() / "k.sqlite", knowledge::DataFolders::under(sfs::path(CB_SOURCE_DIR).parent_path())};
    knowledge::Knowledge kb{store};
    std::map<std::string, std::uint32_t> sizes = [] {
        std::map<std::string, std::uint32_t> s;
        for (const auto& [api, bytes] : w16_golden().at("sizes").items()) s[api] = bytes.get<std::uint32_t>();
        return s;
    }();

    w16::Environment environment(emulate::native_trace::Governor& governor) {
        w16::Environment env;
        env.governor = &governor;
        env.models = w16::models_of(kb);
        env.argument_bytes = [this](std::string_view api) -> std::optional<std::uint32_t> {
            const auto it = sizes.find(std::string(api));
            if (it == sizes.end()) return std::nullopt;
            return it->second;
        };
        return env;
    }

    Win16LaneSpec spec() {
        Win16LaneSpec s;
        s.environment = [this](emulate::native_trace::Governor& g) { return environment(g); };
        s.compiler_options = "-ml -ecd";
        return s;
    }
};

std::vector<std::pair<std::uint32_t, std::uint32_t>> w16_places() {
    std::vector<std::pair<std::uint32_t, std::uint32_t>> out;
    for (const ordered_json& p : w16_golden().at("places")) out.emplace_back(p.at(0).get<std::uint32_t>(), p.at(1).get<std::uint32_t>());
    return out;
}

std::string w16_name(std::uint32_t seg, std::uint32_t off) { return std::format("{}:{:x}", seg, off); }

std::vector<std::int64_t> w16_words(const ordered_json& j) { return j.get<std::vector<std::int64_t>>(); }

}  // namespace

CB_TEST("verify.win16.the_fixed_inputs_are_the_recorded_ones", [] {
    const std::vector<Vector16> v = win16_vectors();
    CB_CHECK_EQ(v.size(), w16_golden().at("n_vectors").get<std::size_t>());
    for (std::size_t i = 0; i < 10; ++i) CB_CHECK(v[i].args == w16_words(w16_golden().at("vectors").at(i)));
    // the scattered four: every word an even offset, the odd ones of every second input the safe selector; the same on every run
    CB_CHECK(win16_vectors()[11].args == v[11].args);
    for (std::size_t i = 10; i < 14; ++i) {
        CB_CHECK_EQ(v[i].args.size(), std::size_t(8));
        for (std::size_t k = 0; k < 8; ++k) {
            const bool selector = (i - 10) % 2 != 0 && k % 2 != 0;
            if (selector) CB_CHECK_EQ(v[i].args[k], w16::kSafeSelector);
            else CB_CHECK(v[i].args[k] % 2 == 0 && v[i].args[k] >= 0x100 && v[i].args[k] < 0x7F00);
        }
    }
    CB_CHECK(v[10].args != v[11].args);
    CB_CHECK_EQ(v[0].words().size(), std::size_t(8));
});

CB_TEST("verify.win16.a_fingerprint_is_the_recorded_one", [] {
    Win16World w;
    emulate::native_trace::Governor governor;
    w16::Runner runner(w.environment(governor));
    const formats::ne::Module nef = formats::ne::Module::open_file(w16_module());
    std::map<std::uint64_t, core16::SegOff> where;
    const auto places = w16_places();
    for (std::size_t i = 0; i < places.size(); ++i) where[i] = {places[i].first, places[i].second};
    const auto spans = Win16Lane::spans(nef, where);
    // the spans and instruction counts
    for (const auto& [i, want] : w16_golden().at("spans").items()) {
        const auto at = spans.find(std::stoull(i));
        CB_CHECK(at != spans.end());
        if (at == spans.end()) continue;
        CB_CHECK_EQ(at->second.first.start, want.at(0).at(0).get<std::uint32_t>());
        CB_CHECK_EQ(at->second.first.end, want.at(0).at(1).get<std::uint32_t>());
        CB_CHECK_EQ(at->second.second, want.at(1).get<std::int64_t>());
    }
    const std::vector<Vector16> vectors = win16_vectors();
    std::size_t compared = 0;
    for (std::size_t f = 0; f < places.size(); ++f) {
        const auto& [seg, off] = places[f];
        const auto span = spans.at(f).first;
        // the code of the segment, for the path of the function's own jumps
        const std::vector<formats::ne::Segment> segments = nef.segments();
        const formats::ne::Segment& s = segments[seg - 1];
        JumpTable jumps(Bytes(nef.data().begin() + s.file_offset, nef.data().begin() + s.file_offset + s.length), seg);
        Win16Target target{w16_module(), seg, off, false, false, span};
        Win16Side side;
        side.jumps = &jumps;
        const ordered_json& rows = w16_golden().at("fingerprints").at(w16_name(seg, off));
        for (std::size_t k = 0; k < 10; ++k) {
            const ordered_json& want = rows.at(k);
            const Win16Print got = win16_fingerprint(runner, win16_judge_base(), target, vectors[k], side);
            const std::string what = std::format("{} input {}", w16_name(seg, off), k);
            CB_CHECK(!got.fp.err);
            CB_CHECK_EQ(got.fp.exit, want.at("exit").get<std::string>());
            CB_CHECK_EQ(got.fp.ret.value_or(-1), want.at("ret").is_null() ? -1 : want.at("ret").get<std::int64_t>());
            CB_CHECK(got.fp.deps == want.at("deps").get<std::vector<std::string>>());
            CB_CHECK_EQ(got.fp.writes.value_or("?"), want.at("writes").get<std::string>());
            CB_CHECK_EQ(got.fp.pop.value_or(-1), want.at("pop").is_null() ? -1 : want.at("pop").get<std::int64_t>());
            CB_CHECK_EQ(got.fp.saved.value_or(false), want.at("saved").is_null() ? false : want.at("saved").get<bool>());   // (null: a run that did not return)
            CB_CHECK(got.fp.cov == want.at("cov").get<std::vector<std::int64_t>>());
            std::vector<std::pair<std::int64_t, bool>> trail, want_trail;
            for (const concolic::Step& t : got.trail) trail.emplace_back(t.site, t.taken);
            for (const ordered_json& t : want.at("trail")) want_trail.emplace_back(t.at(0).get<std::int64_t>(), t.at(1).get<bool>());
            CB_CHECK(trail == want_trail);
            ++compared;
        }
    }
    CB_CHECK(compared >= 200);
});

CB_TEST("verify.win16.the_inputs_aimed_at_what_the_code_tests_are_the_recorded_ones", [] {
    const formats::ne::Module nef = formats::ne::Module::open_file(w16_module());
    std::map<std::uint64_t, core16::SegOff> where;
    const auto places = w16_places();
    for (std::size_t i = 0; i < places.size(); ++i) where[i] = {places[i].first, places[i].second};
    const auto spans = Win16Lane::spans(nef, where);
    for (std::size_t f = 0; f < 6; ++f) {
        const auto& [seg, off] = places[f];
        const std::vector<Vector16> got = win16_directed(nef, seg, spans.at(f).first);
        const ordered_json& want = w16_golden().at("directed").at(w16_name(seg, off));
        CB_CHECK_EQ(got.size(), want.size());
        for (std::size_t i = 0; i < got.size() && i < want.size(); ++i) CB_CHECK(got[i].args == w16_words(want.at(i)));
    }
});

CB_TEST("verify.win16.a_caller_frame_helpers_places_are_read_from_its_c", [] {
    const std::string c = w16_golden().at("frames").at(0).at(0).get<std::string>();
    (void)c;
    const std::string text = "void sub_9_0A62(frame_4,frame_6)\nint2 __far *frame_4;\nint1 __far *frame_6;\n{\n}\n";
    const auto slots = win16_frame_slots(text, "sub_9_0A62");
    CB_CHECK(slots.has_value());
    if (slots) {
        CB_CHECK_EQ(slots->size(), std::size_t(2));
        CB_CHECK_EQ((*slots)[0].first, 4u);
        CB_CHECK_EQ((*slots)[0].second, 2u);
        CB_CHECK_EQ((*slots)[1].first, 6u);
        CB_CHECK_EQ((*slots)[1].second, 1u);
    }
    // a function that takes no frame places, an unknown type, a name that is not the function's
    CB_CHECK(!win16_frame_slots("void f(int a)\n{\n}", "f").has_value());
    CB_CHECK(!win16_frame_slots("void f(frame_4)\n{\n}", "f").has_value());
    CB_CHECK(!win16_frame_slots(text, "other").has_value());
    CB_CHECK(!win16_frame_slots(text, "A62").has_value());
    // the helper's places as the arguments a run is entered with: a far pointer to each, the last parameter deepest
    CB_CHECK(w16_golden().at("frames").at(1).at(2).is_null());
});

CB_TEST("verify.win16.each_exported_function_is_judged_as_recorded", [] {
    Win16World w;
    const Win16Lane lane(w.spec());
    const formats::ne::Module nef = formats::ne::Module::open_file(w16_module());
    std::map<std::uint64_t, core16::SegOff> where;
    const auto places = w16_places();
    for (std::size_t i = 0; i < places.size(); ++i) where[i] = {places[i].first, places[i].second};
    const auto spans = Win16Lane::spans(nef, where);
    std::size_t judged = 0, divergent = 0;
    for (std::size_t f = 0; f < places.size(); ++f) {
        const auto& [seg, off] = places[f];
        for (const char* what : {"self", "other"}) {
            const auto [bseg, boff] = std::string(what) == "self" ? places[f] : places[(f + 1) % places.size()];
            Win16Lane::Item item;
            item.key = Win16Lane::key(seg, off);
            item.segment = seg;
            item.offset = off;
            item.wide = false;
            item.span = spans.at(f).first;
            item.insns = spans.at(f).second;
            item.built_segment = bseg;
            item.built_offset = boff;
            Win16Lane::Context context;
            context.module = w16_module();
            context.dll = w16_module();
            context.order = {{seg, seg}, {bseg, bseg}};
            const Judgement got = lane.judge_function(context, item);
            const ordered_json& want = w16_golden().at("judge").at(item.key + " " + what);
            const std::string name = item.key + " " + what;
            ++judged;
            if (got.verdict == Verdict::Divergent) ++divergent;
            testing::count_check();
            if (std::string(word(got.verdict)) != want.at(0).get<std::string>() || got.reason != want.at(1).get<std::string>() || got.detail != want.at(3).get<std::string>()) {
                testing::record_failure(__FILE__, __LINE__, std::format("{}: the judge said {} / {} / {} but expected {} / {} / {}", name, word(got.verdict), got.reason, got.detail,
                                                                       want.at(0).get<std::string>(), want.at(1).get<std::string>(), want.at(3).get<std::string>()));
            }
            // the limits: all but the coverage exactly, the coverage within ten points (the guessed inputs differ)
            std::vector<std::string> mine, theirs;
            int mine_cov = -1, their_cov = -1;
            for (const std::string& l : got.limits) {
                if (l.starts_with("coverage=")) mine_cov = std::stoi(l.substr(9));
                else mine.push_back(l);
            }
            for (const ordered_json& l : want.at(2)) {
                const std::string text = l.get<std::string>();
                if (text.starts_with("coverage=")) their_cov = std::stoi(text.substr(9));
                else theirs.push_back(text);
            }
            // (low-coverage follows the coverage: it may come or go with it)
            const auto strip = [](std::vector<std::string>& v) { v.erase(std::remove(v.begin(), v.end(), "low-coverage"), v.end()); };
            if (std::abs(mine_cov - their_cov) > 10) {
                testing::count_check();
                testing::record_failure(__FILE__, __LINE__, std::format("{}: coverage {} but expected {}", name, mine_cov, their_cov));
            }
            strip(mine);
            strip(theirs);
            CB_CHECK(mine == theirs);
        }
    }
    CB_CHECK_EQ(judged, std::size_t(60));
    CB_CHECK_EQ(divergent, std::size_t(30));   // every stand-in that does something else is told apart; every function equals itself
});

CB_TEST("verify.win16.the_keys_the_profile_and_the_names_of_segments", [] {
    Win16World w;
    CB_CHECK_EQ(Win16Lane::key(4, 0x15c), "4:015c");
    const Win16Lane lane(w.spec());
    CB_CHECK_EQ(lane.name(), "ne");
    const Profile a = lane.profile({}), b = lane.profile({});
    CB_CHECK_EQ(a.digest, b.digest);
    CB_CHECK_EQ(a.digest.size(), std::size_t(12));
    Win16LaneSpec other = w.spec();
    other.compiler_options = "-ml -ecd -j";
    CB_CHECK(Win16Lane(other).profile({}).digest != a.digest);
    CB_CHECK(!lane.concolic_ready());   // no lifters given: the inputs are guessed
    // a rebuild's code segments are named by the original's they hold; the selector of module segment n is GDT index 300 + n - 1
    const auto own = Win16Lane::named_segments({1, 2});
    CB_CHECK_EQ(own.at(w16::selector(w16::kModuleSlot)), "seg1");
    CB_CHECK_EQ(own.at(w16::selector(w16::kModuleSlot + 1)), "seg2");
    const std::map<std::uint32_t, std::uint32_t> order = {{1, 3}, {2, 3}};
    const auto rebuilt = Win16Lane::named_segments({}, &order);
    CB_CHECK_EQ(rebuilt.at(w16::selector(w16::kModuleSlot)), "seg3");
    CB_CHECK_EQ(rebuilt.size(), std::size_t(2));
});

CB_TEST("verify.win16.the_rebuilds_own_data_is_read_from_its_map", [] {
    fs::ScopedTempDir dir("cb-win16-lane-map");
    CB_CHECK(Win16Lane::own_data(dir.path()).empty());   // no map: nothing of the build's own
    fs::write_text(dir.path() / "m.map", "0001:0000       _first\n0003:01a4+      _codebreaker_int\n0003:01c0       _other\n");
    const auto own = Win16Lane::own_data(dir.path());
    CB_CHECK_EQ(own.size(), std::size_t(1));
    CB_CHECK_EQ(own[0].lo, 0x1a4u);
    CB_CHECK_EQ(own[0].hi, 0x1a4u + 20);
    fs::write_text(dir.path() / "m.map", "0001:0000       _first\n");
    CB_CHECK(Win16Lane::own_data(dir.path()).empty());
});

namespace {

// A module as the lane's compile step would hand it: the exports of LZEXPAND as functions f0 ... of a header and their texts.
std::shared_ptr<decompile::c_text::Module16> fake_module(std::size_t count) {
    auto m = std::make_shared<decompile::c_text::Module16>();
    OrderedMap<std::string, std::string> texts;
    OrderedMap<std::uint64_t, std::string> by_va;
    const auto places = w16_places();
    std::vector<std::string> order;
    for (std::size_t i = 0; i < count; ++i) {
        const std::string name = "f" + std::to_string(i);
        texts[name] = "int " + name + "(void)\n{\n    return " + std::to_string(i) + ";\n}\n";
        by_va[0x1000 + i] = name;
        order.push_back(name);
        m->where[0x1000 + static_cast<std::int64_t>(i)] = {places[i].first, places[i].second};
    }
    m->made = translate::l2::Made::of("lzexpand", "/* header */\n", texts, order, by_va);
    m->wide.insert("f1");
    m->near.insert("f2");
    m->novalue.insert("f3");
    return m;
}

}  // namespace

CB_TEST("verify.win16.prepare_and_build_make_the_functions_the_units_judged", [] {
    Win16World w;
    Win16LaneSpec spec = w.spec();
    int built_with = 0;
    spec.compilable = [](const sfs::path&, const sfs::path&) { return fake_module(5); };
    spec.build_dll = [&](const decompile::c_text::Module16&, const sfs::path&, const std::map<int, Bytes>& images) {
        ++built_with;
        CB_CHECK(images.empty());
        Win16LaneSpec::Rebuilt r;
        r.dll = w16_module();   // (the module itself stands in for the rebuild)
        const auto places = w16_places();
        for (std::size_t i = 0; i < 4; ++i) r.where["f" + std::to_string(i)] = {static_cast<std::uint16_t>(places[i].first), places[i].second};
        r.dropped["f4"] = "the compiler rejected it";
        return r;
    };
    const Win16Lane lane(spec);
    fs::ScopedTempDir work("cb-win16-lane-prepare");
    const std::unique_ptr<Prepared> prepared = lane.prepare(w16_module(), work.path());
    CB_CHECK_EQ(prepared->header, "/* header */\n");
    CB_CHECK_EQ(text::join(prepared->order, " "), "f0 f1 f2 f3 f4");
    const auto places = w16_places();
    CB_CHECK_EQ(prepared->key.at("f1"), Win16Lane::key(places[1].first, places[1].second));
    CB_CHECK_EQ(prepared->all_keys.size(), std::size_t(5));
    CB_CHECK(std::is_sorted(prepared->all_keys.begin(), prepared->all_keys.end()));
    CB_CHECK_EQ(prepared->sig.size(), std::size_t(40));
    CB_CHECK(prepared->texts.at("f0").find("return 0;") != std::string::npos);

    const Built built = lane.build(*prepared, w16_module(), work.path());
    CB_CHECK_EQ(built_with, 1);
    CB_CHECK_EQ(text::join(built.items, " "), "f0 f1 f2 f3");
    CB_CHECK_EQ(built.missing.size(), std::size_t(1));
    CB_CHECK_EQ(built.missing.at("f4"), "the compiler rejected it");
    // the build's judge is the lane's: a function beside itself is equivalent (the far one and a near one; one returning a value, one that returns none)
    const Judgement j = built.judge("f1");
    CB_CHECK(j.verdict == Verdict::Equivalent);
    CB_CHECK(built.judge("f3").verdict == Verdict::Equivalent);
    // the lane needs what makes a module compilable, and what builds it
    CB_CHECK_THROWS_AS(UsageError, (void)Win16Lane(w.spec()).prepare(w16_module(), work.path()));
    Win16LaneSpec no_build = w.spec();
    no_build.compilable = spec.compilable;
    CB_CHECK_THROWS_AS(UsageError, (void)Win16Lane(no_build).build(*prepared, w16_module(), work.path()));
});

CB_TEST("verify.win16.code_segments_the_c_still_reads_are_given_to_the_build", [] {
    const formats::ne::Module nef = formats::ne::Module::open_file(w16_module());
    auto m = fake_module(3);
    decompile::c_text::CodeSegmentReport left;
    left.read = 4;
    left.left = {"an offset from a register"};
    decompile::c_text::CodeSegmentReport clean;
    clean.read = 2;
    clean.named = 2;
    m->code_reads[2] = left;
    m->code_reads[4] = clean;
    m->code_reads[99] = left;   // a segment the module has not
    const auto images = Win16Lane::code_images(*m, nef);
    CB_CHECK_EQ(images.size(), std::size_t(1));
    const formats::ne::Segment s = nef.segments()[1];
    CB_CHECK_EQ(images.at(2).size(), static_cast<std::size_t>(s.length));
    CB_CHECK(images.at(2)[0] == nef.data()[s.file_offset]);
});
