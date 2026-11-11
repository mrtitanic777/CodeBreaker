// Tests of the verdict vocabulary: the words, the older spellings read as the words now, classification with
// limits, and the provenance index. The expected words were checked against verify/verdicts.py by hand.

#include "cb/verify/verdicts.hpp"

#include <set>

#include "cb/core/testing.hpp"

using namespace cb::verify;

CB_TEST("verify.verdicts.judge_words", [] {
    const char* expected[] = {"equivalent", "consistent", "divergent", "inconclusive", "no-compile", "no-source"};
    int i = 0;
    for (Verdict v : kVerdicts) {
        CB_CHECK_EQ(std::string(word(v)), std::string(expected[i]));
        CB_CHECK(parse_verdict(expected[i]) == v); // a stored word reads back as itself
        ++i;
    }
    CB_CHECK_EQ(std::string(said(Verdict::Equivalent)), "same");
    CB_CHECK_EQ(std::string(said(Verdict::Consistent)), "same (both fail)");
    CB_CHECK_EQ(std::string(said(Verdict::Divergent)), "differs");
    CB_CHECK_EQ(std::string(said(Verdict::Inconclusive)), "not run");
    CB_CHECK_EQ(std::string(said(Verdict::NoCompile)), "does not compile");
    CB_CHECK_EQ(std::string(said(Verdict::NoSource)), "no source");
});

CB_TEST("verify.verdicts.older_spellings", [] {
    CB_CHECK(parse_verdict("no-synth") == Verdict::NoSource);
    CB_CHECK(parse_verdict("same") == Verdict::Equivalent);
    CB_CHECK(parse_verdict("alike") == Verdict::Equivalent);
    CB_CHECK(parse_verdict("differs") == Verdict::Divergent);
    CB_CHECK(parse_verdict("different") == Verdict::Divergent);
    for (const char* w : {"not run", "not-run", "not judged", "untested"})
        CB_CHECK(parse_verdict(w) == Verdict::Inconclusive);
    CB_CHECK(parse_verdict("does not compile") == Verdict::NoCompile);
    CB_CHECK(parse_verdict("not compiled") == Verdict::NoCompile);
    // a word that is not a verdict is not guessed at; the words are exact (no case folding, no trimming)
    CB_CHECK(!parse_verdict("verified"));
    CB_CHECK(!parse_verdict("Equivalent"));
    CB_CHECK(!parse_verdict(" equivalent"));
    CB_CHECK(!parse_verdict(""));
});

CB_TEST("verify.verdicts.good_and_pair", [] {
    CB_CHECK(is_good(Verdict::Equivalent));
    CB_CHECK(is_good(Verdict::Consistent));
    CB_CHECK(!is_good(Verdict::Divergent));
    CB_CHECK(!is_good(Verdict::Inconclusive));
    CB_CHECK(!is_good(std::optional<Verdict>()));
    CB_CHECK(is_good(parse_verdict("same")));
    CB_CHECK(pair_verdict(true) == Verdict::Equivalent);
    CB_CHECK(pair_verdict(false) == Verdict::Divergent);
    CB_CHECK(pair_verdict(true, false) == Verdict::Inconclusive);
    CB_CHECK(pair_verdict(true, true, false) == Verdict::NoCompile); // not built wins over not run
    CB_CHECK(pair_verdict(true, false, false) == Verdict::NoCompile);
});

CB_TEST("verify.verdicts.class_words_and_order", [] {
    CB_CHECK_EQ(std::string(word(Class::NotJudged)), "not-judged");
    CB_CHECK_EQ(std::string(title(Class::Weak)), "VERIFIED (weak)");
    CB_CHECK_EQ(std::string(title(Class::Identified)), "IDENTIFIED (runtime library)");
    CB_CHECK_EQ(std::string(title(Class::NoCompile)), "DOES NOT COMPILE");
    CB_CHECK_EQ(std::string(label(Class::Identified)), "runtime library");
    CB_CHECK_EQ(std::string(label(Class::NotJudged)), "not judged");
    // ORDER is best first, SCORED puts Identified third, BUILT stops before the classes that did not run
    std::string order;
    for (Class c : kClassOrder)
        order += std::string(word(c)) + " ";
    CB_CHECK_EQ(order, "verified weak unverified not-judged no-compile no-source ");
    std::string scored;
    for (Class c : kClassScored)
        scored += std::string(word(c)) + " ";
    CB_CHECK_EQ(scored, "verified weak identified unverified not-judged no-compile no-source ");
    std::string built;
    for (Class c : kClassBuilt)
        built += std::string(word(c)) + " ";
    CB_CHECK_EQ(built, "verified weak unverified not-judged ");
    CB_CHECK(Class::Verified < Class::Weak); // the enum is ordered best first
    CB_CHECK(Class::Unverified < Class::NoSource);
    // each class's verdict
    CB_CHECK(class_of(Verdict::Equivalent) == Class::Verified);
    CB_CHECK(class_of(Verdict::Consistent) == Class::Weak);
    CB_CHECK(class_of(Verdict::Divergent) == Class::Unverified);
    CB_CHECK(class_of(Verdict::Inconclusive) == Class::NotJudged);
});

CB_TEST("verify.verdicts.parse_class", [] {
    for (Class c : kClassScored) {
        CB_CHECK(parse_class(word(c)) == c);
        CB_CHECK(parse_class(title(c)) == c); // the title reads back (case folded)
        CB_CHECK(parse_class(label(c)) == c); // and so does the terminal's label
    }
    CB_CHECK(parse_class("  Verified (weak) ") == Class::Weak);
    CB_CHECK(parse_class("run-time library") == Class::Identified);
    CB_CHECK(parse_class("runtime library") == Class::Identified);
    CB_CHECK(parse_class("Identified (runtime library)") == Class::Identified);
    CB_CHECK(!parse_class("equivalent")); // a judge's word is not a class
    CB_CHECK(!parse_class(""));
});

CB_TEST("verify.verdicts.weak_limits", [] {
    CB_CHECK_EQ(std::string(word(WeakLimit::LowCoverage)), "low-coverage");
    CB_CHECK_EQ(std::string(explanation(WeakLimit::Truncated)), "a run was cut off at the step limit");
    CB_CHECK_EQ(std::string(explanation(WeakLimit::MethodsPartial)),
                "only some of the class's methods could be run beside the original (java)");
    std::set<std::string> seen;
    for (unsigned i = 0; i < kWeakLimitCount; ++i) {
        auto l = static_cast<WeakLimit>(i);
        seen.insert(std::string(word(l)));
        CB_CHECK(parse_weak_limit(word(l)) == l);
    }
    CB_CHECK_EQ(seen.size(), std::size_t{9});

    WeakLimits s = parse_limits("truncated,layout-differs,fault-only,truncated,,low-coverage");
    CB_CHECK_EQ(s.size(), std::size_t{3}); // a repeat counts once; a lane's own note is not a weak limit
    CB_CHECK(s.contains(WeakLimit::Truncated) && s.contains(WeakLimit::FaultOnly) &&
             s.contains(WeakLimit::LowCoverage));
    CB_CHECK(!s.contains(WeakLimit::StubCallee));
    CB_CHECK_EQ(limits_text(s), "fault-only,low-coverage,truncated"); // enum order
    CB_CHECK(parse_limits("").empty());
    CB_CHECK(parse_limits(",,").empty());
    CB_CHECK(parse_limits("layout-differs").empty());
    int n = 0;
    for (WeakLimit l : s) {
        (void)l;
        ++n;
    }
    CB_CHECK_EQ(n, 3);
    CB_CHECK(WeakLimits{WeakLimit::FaultOnly, WeakLimit::MethodsPartial}.contains(WeakLimit::MethodsPartial));
});

CB_TEST("verify.verdicts.classify", [] {
    // an equivalent verdict is verified unless a weak limit says otherwise
    CB_CHECK(classify(Verdict::Equivalent, "") == (Classification{Class::Verified, {}}));
    CB_CHECK(classify(Verdict::Equivalent, "layout-differs") == (Classification{Class::Verified, {}}));
    Classification c = classify(Verdict::Equivalent, "low-coverage,stub-callee");
    CB_CHECK(c.cls == Class::Weak);
    CB_CHECK(c.reasons == (WeakLimits{WeakLimit::LowCoverage, WeakLimit::StubCallee}));
    // a consistent verdict is always weak; with no limit named the reason is that both sides only failed
    c = classify(Verdict::Consistent, "");
    CB_CHECK(c.cls == Class::Weak && c.reasons == WeakLimits{WeakLimit::FaultOnly});
    c = classify(Verdict::Consistent, "truncated");
    CB_CHECK(c.cls == Class::Weak && c.reasons == WeakLimits{WeakLimit::Truncated});
    // every other verdict is its class, limits ignored
    CB_CHECK(classify(Verdict::Divergent, "truncated") == (Classification{Class::Unverified, {}}));
    CB_CHECK(classify(Verdict::Inconclusive) == (Classification{Class::NotJudged, {}}));
    CB_CHECK(classify(Verdict::NoCompile) == (Classification{Class::NoCompile, {}}));
    CB_CHECK(classify(Verdict::NoSource) == (Classification{Class::NoSource, {}}));
    // no verdict at all (a missing word, or one that is not a verdict) is no source
    CB_CHECK(classify(std::nullopt, "") == (Classification{Class::NoSource, {}}));
    CB_CHECK(classify(parse_verdict("garbage"), "truncated") == (Classification{Class::NoSource, {}}));
    // an older spelling classifies as the word now
    CB_CHECK(classify(parse_verdict("same"), "") == (Classification{Class::Verified, {}}));
});

CB_TEST("verify.verdicts.byte_match", [] {
    const char* words[] = {"identical", "same size", "different", "no original extent", "no-compile"};
    int i = 0;
    for (ByteMatch m : kByteMatches) {
        CB_CHECK_EQ(std::string(word(m)), std::string(words[i++]));
        CB_CHECK(parse_byte_match(word(m)) == m);
    }
    CB_CHECK_EQ(std::string(label(ByteMatch::NoCompile)), "does not compile");
    CB_CHECK_EQ(std::string(label(ByteMatch::Identified)), "runtime library");
    CB_CHECK(parse_byte_match("does not compile") == ByteMatch::NoCompile);
    CB_CHECK(parse_byte_match("same length, different") == ByteMatch::SameSize);
    CB_CHECK(parse_byte_match("different length") == ByteMatch::Different);
    CB_CHECK(parse_byte_match("run-time library") == ByteMatch::Identified);
    CB_CHECK(parse_byte_match("runtime library") == ByteMatch::Identified);
    CB_CHECK(parse_byte_match("identified") == ByteMatch::Identified);
    CB_CHECK(!parse_byte_match("equivalent"));
});

CB_TEST("verify.verdicts.judgement", [] {
    Judgement j = not_run();
    CB_CHECK(j.verdict == Verdict::Inconclusive);
    CB_CHECK_EQ(j.reason, "judge: the emulator crashed on this function");
    CB_CHECK(j.limits.empty() && j.detail.empty());
    CB_CHECK(j.classify() == (Classification{Class::NotJudged, {}}));

    Judgement k;
    k.verdict = Verdict::Equivalent;
    k.limits = {"layout-differs", "truncated"};
    CB_CHECK_EQ(k.limits_joined(), "layout-differs,truncated"); // the stored form keeps the lane's own note
    CB_CHECK(k.classify() == (Classification{Class::Weak, WeakLimits{WeakLimit::Truncated}}));
});

CB_TEST("verify.verdicts.of_row_and_index", [] {
    StoreRow row;
    row.addr = "0x401000";
    row.verdict = "same"; // written by an older store
    row.reason = "ok";
    FunctionVerdict f = of_row("lz32.dll", row);
    CB_CHECK_EQ(f.module, "lz32.dll");
    CB_CHECK(f.verdict == Verdict::Equivalent);
    CB_CHECK(f.addr && *f.addr == "0x401000");
    CB_CHECK_EQ(f.reason, "ok");
    CB_CHECK(f.limits.empty() && f.detail.empty());
    StoreRow bare; // a row with no verdict and no address
    FunctionVerdict g = of_row("m", bare);
    CB_CHECK(!g.verdict && !g.addr);
    StoreRow odd;
    odd.verdict = "frobnicated";
    CB_CHECK(!of_row("m", odd).verdict);

    std::unordered_map<std::string, StoreRow> rows;
    StoreRow a;
    a.addr = "0x10";
    a.verdict = "divergent";
    rows["0x10"] = a;
    Index index("lz32.dll", rows);
    CB_CHECK_EQ(index.module(), "lz32.dll");
    const StoreRow* hit = index.at(Origin{"lz32.dll", "0x10"});
    CB_CHECK(hit != nullptr && hit->verdict == std::optional<std::string>("divergent"));
    CB_CHECK(index.at(Origin{"lz32.dll", "0x20"}) == nullptr);  // no such function
    CB_CHECK(index.at(Origin{"other.dll", "0x10"}) == nullptr); // another module's node
    CB_CHECK(index.at(Origin{}) == nullptr);                    // no origin at all
    Origin o = index.origin("0x10");
    CB_CHECK(o.module == "lz32.dll" && o.va == "0x10");
    CB_CHECK(index.at(o) == hit);
});
