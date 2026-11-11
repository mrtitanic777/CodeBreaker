#include "cb/knowledge/value.hpp"

#include <cmath>
#include <limits>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

// DIVERGES from the Python base's values (pickled rows, engine/facts):
//   * a tuple and a "shared constant" str were values of their own in the old Json (what a pickle kept); a Value has
//     lists and texts only, and the encoding is the base's own versioned format -- no pickle is read or written;
//   * an integer wider than 64 bits was refused (OverflowError); it is a FormatError now, as it never fit;
//   * the NaN / Infinity / -Infinity literals json.load accepted are not JSON and are a ParseError.

using namespace cb;
using namespace cb::knowledge;

namespace {

Value sample() {
    Value::Map inner;
    inner.insert_or_assign("ax", Value(0));
    inner.insert_or_assign("dx", Value("arg0"));
    inner.insert_or_assign("pop", Value(std::int64_t{2}));
    Value::Map top;
    top.insert_or_assign("zeta", Value(inner));          // keys deliberately not sorted: order is part of the value
    top.insert_or_assign("alpha", Value(Value::List{Value(), Value(true), Value(false), Value(-7), Value(1.5), Value("caf\xC3\xA9")}));
    top.insert_or_assign("empty_list", Value(Value::List{}));
    top.insert_or_assign("empty_map", Value(Value::Map{}));
    return Value(std::move(top));
}

}  // namespace

CB_TEST("knowledge.value.kinds_and_views", [] {
    CB_CHECK(Value().is_null());
    CB_CHECK_EQ(*Value(true).boolean(), true);
    CB_CHECK(!Value(1).boolean());                          // an integer is not a boolean
    CB_CHECK_EQ(*Value(std::int64_t{-5}).integer(), -5);
    CB_CHECK(!Value(1.0).integer());
    CB_CHECK_EQ(*Value(3).number(), 3.0);                   // an integer widens to a number
    CB_CHECK_EQ(*Value(2.5).number(), 2.5);
    CB_CHECK_EQ(*Value("x").text(), "x");
    CB_CHECK(Value("x").list() == nullptr);
    const Value v = sample();
    CB_CHECK(v.get("zeta") != nullptr);
    CB_CHECK_EQ(*v.get("zeta")->get("dx")->text(), "arg0");
    CB_CHECK(v.get("absent") == nullptr);
    CB_CHECK(Value(1).get("a") == nullptr);                 // not a map: absent, not an error
    CB_CHECK_EQ(v.map()->entries()[0].first, "zeta");       // insertion order kept
    CB_CHECK_EQ(v.get("alpha")->list()->size(), 6u);
});

CB_TEST("knowledge.value.equality", [] {
    CB_CHECK(sample() == sample());
    CB_CHECK(Value(1) != Value(1.0));                       // kinds differ
    CB_CHECK(Value("a") != Value("b"));
    Value::Map a, b;
    a.insert_or_assign("x", Value(1));
    a.insert_or_assign("y", Value(2));
    b.insert_or_assign("y", Value(2));
    b.insert_or_assign("x", Value(1));
    CB_CHECK(Value(a) != Value(b));                         // key order is part of a map's identity
    const Value nan(std::numeric_limits<double>::quiet_NaN());
    CB_CHECK(nan == nan);                                   // bitwise: a value equals its own decoded copy
});

CB_TEST("knowledge.value.encoding_layout", [] {
    // The format is part of the contract: magic, version, tagged items (integers little-endian, text length-first).
    const Bytes b = Value(Value::List{Value(), Value(true), Value(std::int64_t{-2}), Value("hi")}).encode();
    const Bytes expected = {'C', 'B', 'K', 'V', 1,        // magic and version
                            6, 4, 0, 0, 0,                 // list of four
                            0,                             // null
                            2,                             // true
                            3, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // int -2
                            5, 2, 0, 0, 0, 'h', 'i'};      // text "hi"
    CB_CHECK(b == expected);
});

CB_TEST("knowledge.value.round_trip", [] {
    const Value v = sample();
    const Value back = Value::decode(v.encode());
    CB_CHECK(back == v);
    CB_CHECK_EQ(v.digest(), back.digest());
    CB_CHECK_EQ(v.digest().size(), 40u);
    CB_CHECK_NE(v.digest(), Value(1).digest());
    for (double d : {0.0, -0.0, 1e300, -1e-300, std::numeric_limits<double>::infinity()})
        CB_CHECK(Value::decode(Value(d).encode()) == Value(d));
    CB_CHECK(std::isnan(*Value::decode(Value(std::numeric_limits<double>::quiet_NaN()).encode()).number()));
    // the extremes of the integer
    for (std::int64_t i : {std::numeric_limits<std::int64_t>::min(), std::numeric_limits<std::int64_t>::max(), std::int64_t{0}})
        CB_CHECK(*Value::decode(Value(i).encode()).integer() == i);
});

CB_TEST("knowledge.value.digest_changes_with_content", [] {
    // The digest is what the engine compares: any change of a piece changes it, a copy does not.
    Value::Map m1, m2;
    m1.insert_or_assign("argc", Value(1));
    m2.insert_or_assign("argc", Value(2));
    CB_CHECK_NE(Value(m1).digest(), Value(m2).digest());
    CB_CHECK_EQ(Value(m1).digest(), Value(m1).digest());
    CB_CHECK_NE(Value("12").digest(), Value(12).digest());   // text "12" is not the integer 12
});

CB_TEST("knowledge.value.decode_damage", [] {
    const Bytes good = sample().encode();
    // every proper prefix is truncated: a FormatError, never a crash or a wrong value
    for (std::size_t n = 0; n < good.size(); ++n) {
        const Bytes cut(good.begin(), good.begin() + static_cast<std::ptrdiff_t>(n));
        CB_CHECK_THROWS_AS(FormatError, (void)Value::decode(cut));
    }
    Bytes bad_magic = good;
    bad_magic[0] = 'X';
    CB_CHECK_THROWS_AS(FormatError, (void)Value::decode(bad_magic));
    Bytes bad_version = good;
    bad_version[4] = 2;
    CB_CHECK_THROWS_AS(FormatError, (void)Value::decode(bad_version));
    Bytes trailing = good;
    trailing.push_back(0);
    CB_CHECK_THROWS_AS(FormatError, (void)Value::decode(trailing));
    Bytes bad_tag = {'C', 'B', 'K', 'V', 1, 99};
    CB_CHECK_THROWS_AS(FormatError, (void)Value::decode(bad_tag));
    // a list that claims four billion items in a handful of bytes
    Bytes huge = {'C', 'B', 'K', 'V', 1, 6, 0xFF, 0xFF, 0xFF, 0xFF};
    CB_CHECK_THROWS_AS(FormatError, (void)Value::decode(huge));
    Bytes huge_map = {'C', 'B', 'K', 'V', 1, 7, 0xFF, 0xFF, 0xFF, 0xFF};
    CB_CHECK_THROWS_AS(FormatError, (void)Value::decode(huge_map));
    // a text longer than the data
    Bytes long_text = {'C', 'B', 'K', 'V', 1, 5, 100, 0, 0, 0, 'a'};
    CB_CHECK_THROWS_AS(FormatError, (void)Value::decode(long_text));
});

CB_TEST("knowledge.value.decode_depth_is_bounded", [] {
    Bytes deep = {'C', 'B', 'K', 'V', 1};
    for (int i = 0; i < 100000; ++i) {
        deep.insert(deep.end(), {6, 1, 0, 0, 0});   // a list of one list of one list ...
    }
    deep.push_back(0);
    CB_CHECK_THROWS_AS(FormatError, (void)Value::decode(deep));
});

CB_TEST("knowledge.value.from_json", [] {
    const Value v = Value::from_json_text(R"({"b": [1, -2, 3.5, "xé", null, true], "a": {"n": 0}})");
    CB_CHECK_EQ(v.map()->entries()[0].first, "b");           // the file's key order, not sorted
    const Value::List& l = *v.get("b")->list();
    CB_CHECK_EQ(*l[0].integer(), 1);
    CB_CHECK_EQ(*l[1].integer(), -2);
    CB_CHECK_EQ(*l[2].number(), 3.5);
    CB_CHECK_EQ(*l[3].text(), "x\xC3\xA9");
    CB_CHECK(l[4].is_null());
    CB_CHECK_EQ(*l[5].boolean(), true);
    CB_CHECK_EQ(*v.get("a")->get("n")->integer(), 0);
    // an integer that needs the 64th bit is refused, one that fits is kept
    CB_CHECK_EQ(*Value::from_json_text("9223372036854775807").integer(), std::numeric_limits<std::int64_t>::max());
    CB_CHECK_THROWS_AS(FormatError, (void)Value::from_json_text("9223372036854775808"));
    CB_CHECK_THROWS_AS(ParseError, (void)Value::from_json_text("{\"a\": "));
    CB_CHECK_THROWS_AS(ParseError, (void)Value::from_json_text(""));
    CB_CHECK_THROWS_AS(ParseError, (void)Value::from_json_text("[1, 2] x"));          // nothing may follow the document
    CB_CHECK_THROWS_AS(ParseError, (void)Value::from_json_text("NaN"));               // not JSON
});

CB_TEST("knowledge.value.from_json_duplicate_keys_and_depth", [] {
    // a repeated key keeps its first place and takes its last value (what the base has always done)
    const Value v = Value::from_json_text(R"({"a": 1, "b": 2, "a": 3})");
    CB_CHECK_EQ(v.map()->size(), 2u);
    CB_CHECK_EQ(v.map()->entries()[0].first, "a");
    CB_CHECK_EQ(*v.get("a")->integer(), 3);
    // a document nested past the bound is refused as a FormatError, not a stack overflow
    const std::string deep = std::string(300, '[') + std::string(300, ']');
    CB_CHECK_THROWS_AS(FormatError, (void)Value::from_json_text(deep));
    const std::string ok = std::string(200, '[') + std::string(200, ']');
    CB_CHECK(Value::from_json_text(ok).list() != nullptr);
    CB_CHECK(Value::from_json_text("[]").list()->empty());
    CB_CHECK(Value::from_json_text("{}").map()->empty());
});
