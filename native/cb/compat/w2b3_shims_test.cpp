// The shims at the old header paths of analyze/facts and build/resources (W2-B3): the old py:: surface over cb/analyze/facts and
// cb/build/resources. Conversion and forwarding only; the modules' own tests are native/cb/analyze/facts_test.cpp and
// native/cb/build/resources_test.cpp. (rebuild() and main() reach OpenWatcom and the old corpus: they compile here and run in
// the exe.) Builds at /W3.
#include "codebreaker/analyze/facts.hpp"
#include "codebreaker/build/resources.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace facts = codebreaker::analyze::facts;
namespace res = codebreaker::formats::resources;
namespace bres = codebreaker::build::resources;

CB_TEST("compat.analyze.facts_put_get_and_tally_through_the_old_api", [] {
    cb::fs::ScopedTempDir dir("cb-facts-shim");
    const auto store = facts::open(py::str((dir.path() / "f.sqlite").generic_string()));
    facts::Json value;
    value.kind = facts::Json::Dict;
    facts::Json n;
    n.kind = facts::Json::Int;
    n.i = 4;
    value.dict.set(py::str("args"), n);
    facts::Json name;
    name.kind = facts::Json::Str;
    name.s = py::str("a");
    value.dict.set(py::str("name"), name);
    py::list<facts::Row> rows;
    rows.append(facts::Row(py::str("401000"), py::str("convention"), value));
    rows.append(facts::Row(py::str(""), py::str("duplicates"), facts::Json()));
    store->put(py::str("m"), py::str("conventions"), rows);
    const py::list<facts::Fact> got = store->get(py::str("m"), py::str("conventions"), std::nullopt, py::str("convention"));
    CB_CHECK_EQ(got.vec().size(), 1u);
    CB_CHECK_EQ(got.vec()[0].addr.utf8(), "401000");
    CB_CHECK_EQ(got.vec()[0].value.dict.at(py::str("args")).i, 4);
    CB_CHECK_EQ(facts::dumps(value).utf8(), "{\"args\": 4, \"name\": \"a\"}");
    CB_CHECK_EQ(facts::loads(py::str("[1, null]")).list.size(), 2u);
    CB_CHECK_EQ(store->tally(py::str("m")).at(py::str("conventions")).at(py::str("duplicates")), 1);
    CB_CHECK_EQ(facts::key(py::str("ne"), 0x10080020).utf8(), "2:0020");
    CB_CHECK_EQ(facts::key(py::str("pe"), 0x401000).utf8(), "401000");
    CB_CHECK_EQ(facts::key(py::str("pe"), std::nullopt).utf8(), "");
});

CB_TEST("compat.build.resources_result_and_compare_keep_their_old_shapes", [] {
    py::list<res::Resource> original, rebuilt;
    original.append(res::Resource(res::Ident(int64_t(10)), res::Ident(py::str("DATA")), 1033, 0, py::bytes("abc")));
    original.append(res::Resource(res::Ident(int64_t(10)), res::Ident(py::str("OTHER")), 1033, 0, py::bytes("abc")));
    original.append(res::Resource(res::Ident(int64_t(10)), res::Ident(py::str("GONE")), 1033, 0, py::bytes("abc")));
    rebuilt.append(res::Resource(res::Ident(int64_t(10)), res::Ident(py::str("data")), 1033, 0, py::bytes("abc")));   // (case aside)
    rebuilt.append(res::Resource(res::Ident(int64_t(10)), res::Ident(py::str("OTHER")), 1033, 0, py::bytes("xyz")));
    const bres::Result r = bres::compare(original, rebuilt, bres::WIDE);
    CB_CHECK_EQ(r.total, 3);
    CB_CHECK_EQ(r.identical, 1);
    CB_CHECK_EQ(r.different.vec().size(), 1u);
    CB_CHECK_EQ(r.missing.vec().size(), 1u);
    CB_CHECK(!r.same());
    CB_CHECK_EQ(std::get<py::str>(std::get<1>(r.missing.vec()[0])).utf8(), "GONE");
    CB_CHECK_EQ(r.line().utf8(),
                "3 resources: 1 identical, 0 equivalent, 1 different, 1 missing  (written as text 0, as image files 0, opaque "
                "payload 0, fallback payload 0)");
    // compare() continues the Result it is given
    bres::Result done;
    done.as_text = 5;
    CB_CHECK_EQ(bres::compare(original, original, 0, done).as_text, 5);
    CB_CHECK(bres::compare(original, original, 0, done).same());
});

}  // namespace
