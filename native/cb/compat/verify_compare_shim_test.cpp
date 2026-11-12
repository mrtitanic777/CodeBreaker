// The shim at codebreaker/verify/compare.hpp: the old record shapes over cb/verify/compare. Builds at /W3: the old layer
// is not warning-clean.
#include "codebreaker/verify/compare.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace old = codebreaker::verify::compare;

struct OldWorld {
    cb::fs::ScopedTempDir dir{"cb-shim-compare"};
    cb::fs::ScopedEnv off{"CB_NO_CACHE", std::nullopt};
    OldWorld() {
        py::settings::hook() = [root = dir.path()](const py::str& key) -> py::str {
            if (key == py::str("state")) return py::str(cb::fs::to_text(root / "state"));
            if (key == py::str("build_cache")) return py::str(cb::fs::to_text(root / "bc"));
            if (key == py::str("cache_max_gb")) return py::str("2");
            return py::str("");
        };
        codebreaker::base::cache::_close();
    }
    ~OldWorld() {
        codebreaker::base::cache::_close();
        py::settings::hook() = nullptr;
    }
};

old::Fingerprint returned(int64_t ret) {
    old::Fingerprint f;
    f.ret = ret;
    f.exit = py::str("ret");
    f.writes = py::str("none");
    f.pop = 8;
    f.saved = true;
    return f;
}

struct LaneInput : old::Input {
    int id = 0;
};

}  // namespace

CB_TEST("compat.verify_compare_shim.differences_in_the_old_words", [] {
    old::Fingerprint a = returned(1), b = returned(2);
    CB_CHECK(old::first_difference(a, b) == py::str("ret-differs"));
    CB_CHECK(old::first_difference(a, b, 0xFFFFFFFF) == py::str(""));
    CB_CHECK(old::difference_detail(a, b) == py::str("ret 1 vs 2"));
    old::Input v;
    v.args.append(16);
    CB_CHECK(old::difference_detail(a, b, v) == py::str("ret 1 vs 2  [input 10]"));
});

CB_TEST("compat.verify_compare_shim.compare_hands_the_lanes_own_input_back", [] {
    py::list<LaneInput> vectors;
    for (int i = 1; i <= 3; ++i) {
        LaneInput v;
        v.id = i;
        vectors.append(v);
    }
    int seen_ids = 0;
    old::FingerprintOf<LaneInput> original = [&](const LaneInput& v) {
        seen_ids += v.id;
        return returned(v.id);
    };
    old::FingerprintOf<LaneInput> rebuilt = [](const LaneInput& v) { return returned(v.id); };
    old::Info info;
    auto [verdict, reason, limits] = old::compare<LaneInput>(original, rebuilt, vectors, py::list<LaneInput>(), 0, nullptr, &info);
    CB_CHECK(verdict == py::str("equivalent"));
    CB_CHECK(reason == py::str(""));
    CB_CHECK_EQ(limits.size(), 0);
    CB_CHECK_EQ(seen_ids, 6);

    old::FingerprintOf<LaneInput> wrong = [](const LaneInput& v) { return returned(v.id == 2 ? 99 : v.id); };
    auto [v2, r2, l2] = old::compare<LaneInput>(original, wrong, vectors, py::list<LaneInput>(), 0, nullptr, &info);
    CB_CHECK(v2 == py::str("divergent"));
    CB_CHECK(r2 == py::str("ret-differs"));
    CB_CHECK(info.detail && *info.detail == py::str("ret 2 vs 63  [input ]"));
});

CB_TEST("compat.verify_compare_shim.limits_are_the_sorted_codes", [] {
    py::list<LaneInput> vectors;
    vectors.append(LaneInput());
    old::FingerprintOf<LaneInput> run = [](const LaneInput&) {
        old::Fingerprint f = returned(1);
        f.deps.append(py::str("Api"));
        f.cov.append(1);
        return f;
    };
    auto [verdict, reason, limits] = old::compare<LaneInput>(run, run, vectors, py::list<LaneInput>(), 4);
    CB_CHECK(verdict == py::str("equivalent"));
    CB_CHECK_EQ(limits.size(), 3);   // argc-unknown, coverage=25, low-coverage
    CB_CHECK(limits[0] == py::str("argc-unknown"));
    CB_CHECK(limits[1] == py::str("coverage=25"));
    CB_CHECK(limits[2] == py::str("low-coverage"));
});

CB_TEST("compat.verify_compare_shim.fingerprints_are_cached", [] {
    OldWorld w;
    const py::str key = old::cache_key({codebreaker::base::cache::Part(py::str("module")), codebreaker::base::cache::Part(int64_t(3))});
    CB_CHECK(key.utf8().starts_with("r25:"));
    CB_CHECK(!old::cache_get(key).has_value());
    old::Fingerprint f = returned(7);
    f.deps.append(py::str("A(1)"));
    f.cov.append(5);
    old::cache_put(key, f);
    const std::optional<old::Fingerprint> back = old::cache_get(key);
    CB_CHECK(back.has_value());
    CB_CHECK(back && *back == f);
    CB_CHECK(old::cache_key_repr(py::str("('a', 1)")).utf8().starts_with("r25:"));
});
