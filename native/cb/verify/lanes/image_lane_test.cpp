#include "cb/verify/lanes/image_lane.hpp"

#include <atomic>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

// DIVERGES from verify/lanes/image_lane.py:
//   * the profile digest is the first 12 hex digits of the SHA-1 of "<kind> rev <n> states [{a=0, ...}, ...]", not of
//     Python's repr of a tuple of dicts: a store written by the Python lanes is rebuilt, as the design says (3.8);
//   * the platform's writer is given to the lane (`ImageLaneSpec::writer`), not found through `platforms.of(kind)`;
//   * two runs that differ only in how many places they show (a lane's fault) are a Divergent verdict with the reason
//     "effects-differs"; the Python raised StopIteration out of `next()` there and the judge crashed;
//   * the header and the signature are over the source file's bytes as they are (the Python read them as latin-1 and
//     wrote them back, which is the same bytes).

using namespace cb;
using namespace cb::verify;
using namespace cb::verify::lanes;

namespace {

const std::vector<std::string> kWhat = {"end", "A", "RAM"};

// A program is a file of bytes; a run of it from a state shows the end, A, and the RAM = the file's bytes.
Effects effects_of(const std::filesystem::path& subject, std::uint32_t entry, const State& state) {
    const Bytes file = fs::read_file(subject);
    return {std::string("ret"), static_cast<std::int64_t>(file.at(entry)) + state.at("a"),
            Bytes(file.begin(), file.end())};
}

ImageLaneSpec spec_with(Writer writer) {
    ImageLaneSpec s;
    s.kind = "gb";
    s.rev = 1;
    s.states = {{{"a", 0}, {"fill", 0}}, {{"a", 7}, {"fill", 1}}};
    s.effects = effects_of;
    s.what = kWhat;
    s.start = [](const State& st) { return std::format("from A={:02X}", st.at("a")); };
    s.writer = std::move(writer);
    return s;
}

// A writer that writes `source` and names functions at 0x0 and 0x10 and 0x100; the "built" file is `built_bytes`.
Writer fake_writer(std::string source, bool linked, bool same, Bytes built_bytes, std::vector<std::string> notes = {}) {
    return [=](const std::filesystem::path&, const std::filesystem::path& out_dir) {
        Written w;
        w.source = out_dir / "prog.asm";
        fs::write_text(w.source, source);
        w.functions = {{0x100, "late"}, {0x0, "start"}, {0x10, "mid"}};
        w.linked = linked;
        w.same = same;
        w.notes = notes;
        w.built = out_dir / "prog.gb";
        fs::write_file(w.built, built_bytes);
        return w;
    };
}

Bytes prog(std::uint8_t a, std::uint8_t b) {
    Bytes p(0x200, 0);
    p[0] = a;
    p[0x10] = b;
    return p;
}

}  // namespace

CB_TEST("verify.lanes.image_lane.profile", [] {
    ImageLane lane(spec_with(fake_writer("", true, true, {})));
    const Profile p = lane.profile("any");
    CB_CHECK_EQ(p.digest.size(), 12u);
    CB_CHECK_EQ(p.settings, "gb rev 1 states [{a=0, fill=0}, {a=7, fill=1}]");
    CB_CHECK_EQ(lane.profile("other").digest, p.digest);   // the module is not part of it
    ImageLaneSpec changed = spec_with(fake_writer("", true, true, {}));
    changed.rev = 2;
    CB_CHECK_NE(ImageLane(changed).profile("").digest, p.digest);
    changed.rev = 1;
    changed.states.push_back({{"a", 1}, {"fill", 2}});
    CB_CHECK_NE(ImageLane(changed).profile("").digest, p.digest);   // the states are part of what a verdict depends on
    CB_CHECK_EQ(lane.name(), "gb");
});

CB_TEST("verify.lanes.image_lane.prepare", [] {
    fs::ScopedTempDir tmp("cb-lane");
    ImageLane lane(spec_with(fake_writer("; the source\r\nnop\r\n", true, true, prog(1, 2))));
    const auto m = lane.prepare("module.gb", tmp.path());
    CB_CHECK_EQ(m->header, "; the source\r\nnop\r\n");
    CB_CHECK(m->order == std::vector<std::string>{"start", "mid", "late"});   // by address
    CB_CHECK_EQ(m->key.at("start"), "0000");
    CB_CHECK_EQ(m->key.at("mid"), "0010");
    CB_CHECK_EQ(m->key.at("late"), "0100");
    CB_CHECK(m->all_keys == std::vector<std::string>{"0000", "0010", "0100"});
    CB_CHECK_EQ(m->texts.at("mid"), "; mid, in prog.asm");
    CB_CHECK(std::filesystem::exists(tmp.path() / "source" / "prog.asm"));   // the writer works in <work>/source
    // the signature follows the source and the revision
    ImageLane other(spec_with(fake_writer("; changed\r\n", true, true, prog(1, 2))));
    CB_CHECK_NE(other.prepare("module.gb", tmp.path())->sig, m->sig);
    CB_CHECK_EQ(lane.prepare("module.gb", tmp.path())->sig, m->sig);
    CB_CHECK_EQ(m->sig.size(), 40u);
});

CB_TEST("verify.lanes.image_lane.key_digits", [] {
    fs::ScopedTempDir tmp("cb-lane");
    ImageLaneSpec s = spec_with(fake_writer("x", true, true, prog(1, 2)));
    s.key_digits = 6;
    const auto m = ImageLane(s).prepare("module.gb", tmp.path());
    CB_CHECK_EQ(m->key.at("late"), "000100");
});

CB_TEST("verify.lanes.image_lane.not_built", [] {
    fs::ScopedTempDir tmp("cb-lane");
    ImageLane lane(spec_with(fake_writer("x", false, false, {}, {"first note", std::string(300, 'z')})));
    const auto m = lane.prepare("module.gb", tmp.path());
    const Built b = lane.build(*m, "module.gb", tmp.path());
    CB_CHECK(b.items.empty());
    CB_CHECK(!b.judge);
    CB_CHECK_EQ(b.missing.size(), 3u);
    CB_CHECK_EQ(b.missing.at("mid"), std::string(200, 'z'));   // the last note, cut to 200
    // no notes: a stated default
    ImageLane quiet(spec_with(fake_writer("x", false, false, {})));
    CB_CHECK_EQ(quiet.build(*quiet.prepare("m", tmp.path()), "m", tmp.path()).missing.at("start"), "did not build");
});

CB_TEST("verify.lanes.image_lane.identical_program_is_the_original", [] {
    fs::ScopedTempDir tmp("cb-lane");
    ImageLane lane(spec_with(fake_writer("x", true, true, prog(1, 2))));
    const auto m = lane.prepare("module.gb", tmp.path());
    const Built b = lane.build(*m, "module.gb", tmp.path());
    CB_CHECK(b.items == m->order);
    CB_CHECK(b.missing.empty());
    const Judgement j = b.judge("mid");
    CB_CHECK(j.verdict == Verdict::Equivalent);
    CB_CHECK_EQ(j.detail, std::string(kSameImage));
    CB_CHECK(j.limits.empty());
});

CB_TEST("verify.lanes.image_lane.edited_program_is_run_in_both", [] {
    fs::ScopedTempDir tmp("cb-lane");
    // the original file the lane runs; the rebuilt one differs at 0x10 only
    const std::filesystem::path original = tmp.path() / "orig.gb";
    fs::write_file(original, prog(1, 2));
    {   // the rebuilt program differs in "mid" (offset 0x10): its A and RAM show it, "start" is the same
        ImageLane lane(spec_with(fake_writer("x", true, false, prog(1, 9))));
        const auto m = lane.prepare(original, tmp.path());
        const Built b = lane.build(*m, original, tmp.path());
        const Judgement start = b.judge("start");
        CB_CHECK(start.verdict == Verdict::Divergent);   // RAM is the whole file: it differs for every function
        CB_CHECK_EQ(start.reason, "RAM-differs");
        const Judgement mid = b.judge("mid");
        CB_CHECK(mid.verdict == Verdict::Divergent);
        CB_CHECK_EQ(mid.reason, "A-differs");            // A differs before RAM does: the first place by the names given
        CB_CHECK_EQ(mid.detail, "from A=00");            // the first state it differs from
    }
    {   // same effects: the rebuilt file is the original's bytes under another name
        ImageLane lane(spec_with(fake_writer("x", true, false, prog(1, 2))));
        const auto m = lane.prepare(original, tmp.path());
        const Built b = lane.build(*m, original, tmp.path());
        const Judgement j = b.judge("mid");
        CB_CHECK(j.verdict == Verdict::Equivalent);
        CB_CHECK_EQ(j.detail, "the same effects from 2 register states");
    }
});

CB_TEST("verify.lanes.image_lane.first_differing_state_is_named", [] {
    fs::ScopedTempDir tmp("cb-lane");
    const std::filesystem::path original = tmp.path() / "orig.gb";
    fs::write_file(original, prog(1, 2));
    ImageLaneSpec s = spec_with(fake_writer("x", true, false, prog(1, 2)));
    // the lane's effects see a difference only from the second state (the first state does not read the byte)
    s.effects = [](const std::filesystem::path& subject, std::uint32_t entry, const State& st) {
        const Bytes f = fs::read_file(subject);
        return Effects{std::string("ret"), st.at("a") == 0 ? std::int64_t{0} : std::int64_t{f.at(entry)}, Bytes{}};
    };
    const ImageLane lane(s);
    const Judgement same = lane.judge_function(false, original, original, 0x10);
    CB_CHECK(same.verdict == Verdict::Equivalent);
    fs::write_file(tmp.path() / "b.gb", prog(1, 5));
    const Judgement j = lane.judge_function(false, original, tmp.path() / "b.gb", 0x10);
    CB_CHECK(j.verdict == Verdict::Divergent);
    CB_CHECK_EQ(j.reason, "A-differs");
    CB_CHECK_EQ(j.detail, "from A=07");
});

CB_TEST("verify.lanes.image_lane.effects_of_different_length", [] {
    fs::ScopedTempDir tmp("cb-lane");
    ImageLaneSpec s = spec_with(fake_writer("x", true, false, {}));
    int call = 0;
    s.effects = [&call](const std::filesystem::path&, std::uint32_t, const State&) {
        return ++call % 2 ? Effects{std::int64_t{1}} : Effects{std::int64_t{1}, std::string("extra")};
    };
    const Judgement j = ImageLane(s).judge_function(false, "a", "b", 0);
    CB_CHECK(j.verdict == Verdict::Divergent);
    CB_CHECK_EQ(j.reason, "effects-differs");
});

CB_TEST("verify.lanes.image_lane.subjects_override", [] {
    fs::ScopedTempDir tmp("cb-lane");
    const std::filesystem::path original = tmp.path() / "orig.gb";
    const std::filesystem::path other = tmp.path() / "other.gb";
    fs::write_file(original, prog(1, 2));
    fs::write_file(other, prog(1, 2));
    ImageLaneSpec s = spec_with(fake_writer("x", true, false, prog(7, 7)));   // the built file would differ ...
    s.subjects = [&](const std::filesystem::path&, const Written&) { return std::make_pair(original, other); };   // ... but is not what is run
    const ImageLane lane(s);
    const auto m = lane.prepare(original, tmp.path());
    const Built b = lane.build(*m, original, tmp.path());
    CB_CHECK(b.judge("start").verdict == Verdict::Equivalent);
});

CB_TEST("verify.lanes.image_lane.judge_is_usable_from_threads", [] {
    fs::ScopedTempDir tmp("cb-lane");
    ImageLane lane(spec_with(fake_writer("x", true, true, prog(1, 2))));
    const auto m = lane.prepare("module.gb", tmp.path());
    const Built b = lane.build(*m, "module.gb", tmp.path());
    std::vector<std::thread> ts;
    std::atomic<int> good{0};
    for (int i = 0; i < 4; ++i)
        ts.emplace_back([&] {
            for (const std::string& n : b.items)
                if (b.judge(n).verdict == Verdict::Equivalent)
                    ++good;
        });
    for (auto& t : ts)
        t.join();
    CB_CHECK_EQ(good.load(), 12);
});
