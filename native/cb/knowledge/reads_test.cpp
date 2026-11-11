#include "cb/knowledge/reads.hpp"

#include <set>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

// DIVERGES from codebreaker/knowledge/reads.py and its port:
//   * a read is a typed Read (platform, kind, key); split() of text with fewer than two colons was a ValueError and is
//     Read::parse() returning nullopt;
//   * the reads of a worker process cross as bytes (encode_reads/decode_reads), not as a pickle;
//   * digest() takes the base's answer and each kind's freshener from its owner (ReadDigests::on_kind) instead of
//     importing the symbols modules; a freshener that throws is logged and the kind hashed as it stands (the Python
//     swallowed every Exception silently).

using namespace cb;
using namespace cb::knowledge;

namespace {
std::vector<std::string> names(const std::vector<Read>& reads) {
    std::vector<std::string> out;
    for (const Read& r : reads) out.push_back(r.text());
    return out;
}
}  // namespace

CB_TEST("knowledge.reads.constants_and_text", [] {
    CB_CHECK_EQ(std::string(kReadInputKind), "kb");
    CB_CHECK_EQ(std::string(kAbsent), "absent");
    CB_CHECK_EQ((Read{"win32", "prototype", "GetDlgItem"}).text(), "win32:prototype:GetDlgItem");
    CB_CHECK_EQ((Read{"", "", ""}).text(), "::");
    CB_CHECK((Read{"win32", "signature", "*"}).whole_kind());
    CB_CHECK(!(Read{"win32", "signature", "x"}).whole_kind());
});

CB_TEST("knowledge.reads.parse", [] {
    // harvested from the old harness: the first two colons separate, the key keeps the rest
    const Read a = *Read::parse("win32:prototype:GetDlgItem");
    CB_CHECK_EQ(a.platform, "win32");
    CB_CHECK_EQ(a.kind, "prototype");
    CB_CHECK_EQ(a.key, "GetDlgItem");
    CB_CHECK_EQ(Read::parse("a:b:c:d")->key, "c:d");
    CB_CHECK_EQ(Read::parse("a:b:")->key, "");
    CB_CHECK_EQ(Read::parse("::")->platform, "");
    CB_CHECK_EQ(Read::parse("x:y:z:with:many:colons")->key, "z:with:many:colons");
    CB_CHECK_EQ(Read::parse("p:k:\xC3\xA9")->key, "\xC3\xA9");
    CB_CHECK(!Read::parse("a:b"));      // the old split raised ValueError
    CB_CHECK(!Read::parse("a"));
    CB_CHECK(!Read::parse(""));
    for (const char* t : {"a:b:c", "::", "x:y:z:w"}) CB_CHECK_EQ(Read::parse(t)->text(), t);
});

CB_TEST("knowledge.reads.nothing_outside_a_recording", [] {
    CB_CHECK(!recording_active());
    record("win32", "prototype", "outside");
    record_kind("win32", "prototype");
    merge_reads(std::vector<Read>{{"x", "y", "z"}});
    CB_CHECK(!recording_active());
    CB_CHECK(current_recording() == nullptr);
});

CB_TEST("knowledge.reads.recording_collects_sorted_unique", [] {
    Recording r;
    CB_CHECK(recording_active());
    record("win32", "prototype", "GetDlgItem");
    record("win32", "prototype", "GetDlgItem");
    record("win16", "struct", "RECT");
    record_kind("win32", "signature");
    merge_reads(std::vector<Read>{{"bodies", "name", "abc"}, {"win32", "prototype", "GetDlgItem"}});
    const std::vector<std::string> expected = {"bodies:name:abc", "win16:struct:RECT", "win32:prototype:GetDlgItem",
                                               "win32:signature:*"};
    CB_CHECK(names(r.reads()->sorted()) == expected);
    CB_CHECK_EQ(r.reads()->size(), 4u);
    CB_CHECK(r.reads()->contains({"win16", "struct", "RECT"}));
    CB_CHECK(!r.reads()->contains({"win16", "struct", "POINT"}));
});

CB_TEST("knowledge.reads.inner_recording_keeps_its_own", [] {
    Recording outer;
    record("a", "k", "outer1");
    {
        Recording inner;
        record("a", "k", "inner");
        CB_CHECK_EQ(inner.reads()->size(), 1u);
    }
    record("a", "k", "outer2");
    CB_CHECK(names(outer.reads()->sorted()) == (std::vector<std::string>{"a:k:outer1", "a:k:outer2"}));   // not "inner"
    CB_CHECK(current_recording() == outer.reads());
});

CB_TEST("knowledge.reads.adopted_on_another_thread", [] {
    Recording r;
    const std::shared_ptr<Reads> asker = current_recording();
    bool had_one = true;
    std::thread t([asker, &had_one] {
        had_one = recording_active();           // a new thread has no recording of its own
        AdoptRecording adopt(asker);
        record("win32", "prototype", "FromThread");
    });
    t.join();
    CB_CHECK(!had_one);
    CB_CHECK(r.reads()->contains({"win32", "prototype", "FromThread"}));
    // many threads recording at once lose nothing
    std::vector<std::thread> pool;
    for (int i = 0; i < 8; ++i) {
        pool.emplace_back([asker, i] {
            AdoptRecording adopt(asker);
            for (int k = 0; k < 200; ++k) record("p", "k", std::to_string(i * 1000 + k));
        });
    }
    for (std::thread& th : pool) th.join();
    CB_CHECK_EQ(r.reads()->size(), 1601u);
});

CB_TEST("knowledge.reads.carried_through_a_worker_boundary", [] {
    // The worker runs the item under run_recorded, the reads travel as bytes, and the asker merges them.
    Carried<int> carried = run_recorded([] {
        record("win16", "api_model", "GLOBALLOCK");
        return 42;
    });
    CB_CHECK_EQ(carried.value, 42);
    CB_CHECK_EQ(carried.reads.size(), 1u);
    const Bytes wire = encode_reads(carried.reads);
    Carried<int> arrived{carried.value, decode_reads(wire)};
    Recording asker;
    CB_CHECK_EQ(unwrap(std::move(arrived)), 42);
    CB_CHECK(asker.reads()->contains({"win16", "api_model", "GLOBALLOCK"}));
    // no recording was left in force by the worker's run
    CB_CHECK(current_recording() == asker.reads());
});

CB_TEST("knowledge.reads.wire_format", [] {
    CB_CHECK(decode_reads(encode_reads({})).empty());
    const std::vector<Read> reads = {{"a", "b", ""}, {"", "", "caf\xC3\xA9:with:colons"}};
    CB_CHECK(decode_reads(encode_reads(reads)) == reads);
    const Bytes good = encode_reads(reads);
    for (std::size_t n = 0; n < good.size(); ++n) {
        const Bytes cut(good.begin(), good.begin() + static_cast<std::ptrdiff_t>(n));
        CB_CHECK_THROWS_AS(FormatError, (void)decode_reads(cut));
    }
    Bytes trailing = good;
    trailing.push_back(7);
    CB_CHECK_THROWS_AS(FormatError, (void)decode_reads(trailing));
    CB_CHECK_THROWS_AS(FormatError, (void)decode_reads(Bytes{0xFF, 0xFF, 0xFF, 0xFF}));
});

CB_TEST("knowledge.reads.digests_freshen_once_per_kind", [] {
    std::vector<std::string> asked;
    ReadDigests digests([&asked](const Read& r) {
        asked.push_back(r.text());
        return r.key == "gone" ? std::string(kAbsent) : "h(" + r.text() + ")";
    });
    std::vector<std::string> freshened;
    digests.on_kind({"win32", "prototype"}, [&](const KindId& k) { freshened.push_back(k.platform + ":" + k.kind); });
    CB_CHECK_EQ(digests.digest({"win32", "prototype", "A"}), "h(win32:prototype:A)");
    CB_CHECK_EQ(digests.digest({"win32", "prototype", "B"}), "h(win32:prototype:B)");
    CB_CHECK_EQ(digests.digest({"win32", "prototype", "gone"}), "absent");       // a piece that is not there
    CB_CHECK_EQ(digests.digest({"win32", "prototype", "*"}), "h(win32:prototype:*)");
    CB_CHECK_EQ(freshened.size(), 1u);                                           // once for the kind
    CB_CHECK_EQ(digests.digest({"bodies", "name", "x"}), "h(bodies:name:x)");    // a kind with no freshener is read as is
    CB_CHECK_EQ(freshened.size(), 1u);
    CB_CHECK_EQ(asked.size(), 5u);
});

CB_TEST("knowledge.reads.digests_survive_a_failing_freshener", [] {
    ReadDigests digests([](const Read&) { return std::string("h"); });
    int calls = 0;
    digests.on_kind({"win32", "struct"}, [&](const KindId&) {
        ++calls;
        throw IoError("headers", 3, "gone");
    });
    testing::CapturedStd captured;
    CB_CHECK_EQ(digests.digest({"win32", "struct", "RECT"}), "h");               // hashed as it stands, and said
    CB_CHECK_EQ(digests.digest({"win32", "struct", "POINT"}), "h");
    CB_CHECK_EQ(calls, 1);                                                       // not retried on every ask
    CB_CHECK(captured.err().find("win32:struct") != std::string::npos);
});
