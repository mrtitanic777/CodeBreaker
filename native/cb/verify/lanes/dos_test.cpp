#include "cb/verify/lanes/dos.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

// DIVERGES from verify/lanes/dos.py:
//   * the profile digest is the first 12 hex digits of the SHA-1 of "dos rev 93 tools <name>:<size>:<mtime> ...", not
//     of Python's repr of a tuple (a store the Python lane wrote is rebuilt); a tool that cannot be found is
//     "<name>:missing" (the Python added an empty string, which made two missing tools indistinguishable);
//   * the packager's writer and tools are given to the lane, not found through `platforms.of("dos")`;
//   * a linked program with no source text has keys but no per-function texts, as before; the Python then failed with a
//     KeyError in build() (it indexed the empty key table), the C++ keeps the keys.

using namespace cb;
using namespace cb::verify;
using namespace cb::verify::lanes;

namespace {

Writer fake_writer(std::optional<std::string> source, bool linked, bool same, std::vector<std::string> notes = {}) {
    return [=](const std::filesystem::path&, const std::filesystem::path& out_dir) {
        Written w;
        if (source) {
            w.source = out_dir / "prog.asm";
            fs::write_text(w.source, *source);
        }
        w.functions = {{0x4F0, "b"}, {0x10, "a"}, {0x12345, "c"}};
        w.linked = linked;
        w.same = same;
        w.notes = notes;
        return w;
    };
}

DosLane::Tools tools_at(const std::filesystem::path& dir) {
    return [dir] {
        return std::map<std::string, std::filesystem::path>{{"wasm", dir / "wasm.exe"}, {"wlink", dir / "wlink.exe"}};
    };
}

}  // namespace

CB_TEST("verify.lanes.dos.profile_follows_the_tools", [] {
    fs::ScopedTempDir tmp("cb-dos");
    DosLane lane(fake_writer("x", true, true), tools_at(tmp.path()));
    CB_CHECK_EQ(lane.name(), "dos");
    const Profile missing = lane.profile("");
    CB_CHECK_EQ(missing.settings, "dos rev 93 tools wasm:missing wlink:missing");
    CB_CHECK_EQ(missing.digest.size(), 12u);
    fs::write_text(tmp.path() / "wasm.exe", "assembler");
    fs::write_text(tmp.path() / "wlink.exe", "linker");
    const Profile found = lane.profile("");
    CB_CHECK_NE(found.digest, missing.digest);
    CB_CHECK(found.settings.starts_with("dos rev 93 tools wasm:9:"));
    // a different assembler is a different profile
    fs::write_text(tmp.path() / "wasm.exe", "assembler, newer");
    CB_CHECK_NE(lane.profile("").digest, found.digest);
    CB_CHECK_EQ(lane.profile("").digest, lane.profile("another module").digest);
});

CB_TEST("verify.lanes.dos.prepare", [] {
    fs::ScopedTempDir tmp("cb-dos");
    DosLane lane(fake_writer("mov ax, 1\r\n", true, true), tools_at(tmp.path()));
    const auto m = lane.prepare("prog.exe", tmp.path());
    CB_CHECK_EQ(m->header, "mov ax, 1\r\n");
    CB_CHECK(m->order == std::vector<std::string>{"a", "b", "c"});
    CB_CHECK_EQ(m->key.at("a"), "00010");   // five hex digits: a real-mode address is 20 bits
    CB_CHECK_EQ(m->key.at("b"), "004f0");
    CB_CHECK_EQ(m->key.at("c"), "12345");
    CB_CHECK(m->all_keys == std::vector<std::string>{"00010", "004f0", "12345"});
    CB_CHECK_EQ(m->texts.at("b"), "; b, in prog.asm");
    CB_CHECK_EQ(m->sig.size(), 40u);
});

CB_TEST("verify.lanes.dos.prepare_without_source", [] {
    fs::ScopedTempDir tmp("cb-dos");
    DosLane lane(fake_writer(std::nullopt, false, false, {"the writer failed"}), tools_at(tmp.path()));
    const auto m = lane.prepare("prog.exe", tmp.path());
    CB_CHECK(m->header.empty());
    CB_CHECK(m->texts.empty());
    CB_CHECK_EQ(m->order.size(), 3u);
});

CB_TEST("verify.lanes.dos.identical_program", [] {
    fs::ScopedTempDir tmp("cb-dos");
    DosLane lane(fake_writer("x", true, true), tools_at(tmp.path()));
    const auto m = lane.prepare("prog.exe", tmp.path());
    const Built b = lane.build(*m, "prog.exe", tmp.path());
    CB_CHECK(b.items == m->order);
    CB_CHECK(b.missing.empty());
    for (const std::string& n : b.items) {
        const Judgement j = b.judge(n);
        CB_CHECK(j.verdict == Verdict::Equivalent);
        CB_CHECK_EQ(j.detail, std::string(kSameProgram));
    }
});

CB_TEST("verify.lanes.dos.different_program_says_how", [] {
    fs::ScopedTempDir tmp("cb-dos");
    DosLane lane(fake_writer("x", true, false, {"linked", "the relocation tables differ at entry 4"}), tools_at(tmp.path()));
    const auto m = lane.prepare("prog.exe", tmp.path());
    const Built b = lane.build(*m, "prog.exe", tmp.path());
    CB_CHECK_EQ(b.items.size(), 3u);
    const Judgement j = b.judge("a");
    CB_CHECK(j.verdict == Verdict::Divergent);
    CB_CHECK_EQ(j.reason, "image-differs");
    CB_CHECK_EQ(j.detail, "the relocation tables differ at entry 4");
    // no note: a stated default
    DosLane quiet(fake_writer("x", true, false), tools_at(tmp.path()));
    const Built q = quiet.build(*quiet.prepare("p", tmp.path()), "p", tmp.path());
    CB_CHECK_EQ(q.judge("b").detail, "the built program differs from the original");
});

CB_TEST("verify.lanes.dos.program_that_did_not_build", [] {
    fs::ScopedTempDir tmp("cb-dos");
    DosLane lane(fake_writer("x", false, false, {"wasm: error", "wlink: unresolved external _main"}), tools_at(tmp.path()));
    const auto m = lane.prepare("prog.exe", tmp.path());
    const Built b = lane.build(*m, "prog.exe", tmp.path());
    CB_CHECK(b.items.empty());
    CB_CHECK(!b.judge);
    CB_CHECK_EQ(b.missing.size(), 3u);
    CB_CHECK_EQ(b.missing.at("c"), "wlink: unresolved external _main");
    CB_CHECK_EQ(DosLane::judge_function(false, "why").reason, "image-differs");
});
