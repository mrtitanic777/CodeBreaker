// Tests of cb/invert/msvc2.hpp, the Microsoft C/C++ 2.0 (32-bit) code generator's model: the forward model (construct -> bytes, to_c), the inverse
// (bytes -> construct, accepted only when the forward model gives the bytes back), the register-dataflow walk and the frame layout -- against the recorded
// answers (testdata/msvc2.json).
//
// The golden holds the forward model's battery (71 constructs, two of them shapes that must be declined), what each one's bytes read back as, what
// `interp` makes of them, the frame-layout battery, and framed functions cut out of the real code of lz32.dll and crtdll.dll (the model's class is
// small leaves; real functions are mostly outside it, so these are mostly proofs that it declines).
//
// A construct with a part missing or of the wrong kind is a FormatError; frame_layout
// returns its offsets in layout order.
#include "invert/msvc2.hpp"
#include "invert/msvc2_battery.hpp"

#include <nlohmann/json.hpp>

#include "core/json.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

using namespace cb;
namespace m2 = cb::invert::msvc2;
using cb::invert::Term;

namespace {

Bytes from_hex(std::string_view hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(*text::parse_int<std::uint8_t>(hex.substr(i, 2), 16));
    return out;
}

std::string hex_of(ByteView b) {
    static constexpr char kDigits[] = "0123456789abcdef";
    std::string out;
    for (const std::uint8_t x : b) out += {kDigits[x >> 4], kDigits[x & 15]};
    return out;
}

// A node as the golden writes it: {"t": [...]} a tuple, [...] a list, {"d": {...}} a dictionary.
Term term_of(const Json& j) {
    if (j.is_null()) return Term();
    if (j.is_boolean()) return Term::boolean(j.get<bool>());
    if (j.is_number_integer()) return Term::integer(j.get<long long>());
    if (j.is_string()) return Term::string(j.get<std::string>());
    if (j.is_array()) {
        std::vector<Term> items;
        for (const Json& x : j) items.push_back(term_of(x));
        return Term::list(std::move(items));
    }
    if (j.contains("t")) {
        std::vector<Term> items;
        for (const Json& x : j.at("t")) items.push_back(term_of(x));
        return Term::tuple(std::move(items));
    }
    Term d = Term::dict();
    for (const auto& [k, v] : j.at("d").items()) d.set(k, term_of(v));
    return d;
}

const Json& golden() {
    static const Json g = json::load(testing::data_path("invert/testdata/msvc2.json"));
    return g;
}

void check_reading(const std::string& where, const std::optional<m2::Reading>& got, const Json& want) {
    if (want.is_null()) {
        if (got) testing::record_failure(__FILE__, __LINE__, where + ": read, where a decline is expected: " + got->c);
        return;
    }
    if (!got) {
        testing::record_failure(__FILE__, __LINE__, where + ": declined, where it should read as " + want.at("c").get<std::string>());
        return;
    }
    if (got->c != want.at("c").get<std::string>()) testing::record_failure(__FILE__, __LINE__, where + ": read as '" + got->c + "', expected '" + want.at("c").get<std::string>() + "'");
    testing::count_check();
    CB_CHECK_EQ(got->nargs, want.at("nargs").get<long long>());
    CB_CHECK(got->expr == term_of(want.at("expr")));
    CB_CHECK(got->node == term_of(want.at("node")));
}

void check_interp(const std::string& where, const std::optional<Term>& got, const Json& want) {
    if (want.is_null()) {
        if (got) testing::record_failure(__FILE__, __LINE__, where + ": the walk answered, where none is expected");
        return;
    }
    if (!got) {
        testing::record_failure(__FILE__, __LINE__, where + ": the walk declined, where an answer is expected");
        return;
    }
    // the walk's answer: a list of constructs
    Term wanted = term_of(want);
    CB_CHECK(got->items().size() == wanted.items().size());
    for (std::size_t i = 0; i < wanted.items().size() && i < got->items().size(); ++i)
        if (!(got->items()[i] == wanted.items()[i])) testing::record_failure(__FILE__, __LINE__, where + ": construct " + std::to_string(i) + " differs");
}

}  // namespace

CB_TEST("invert.msvc2.the_forward_battery_as_recorded", [] {
    std::size_t emitted = 0, declined = 0, read_back = 0;
    for (const Json& c : golden().at("cases")) {
        const std::string name = c.at("name").get<std::string>();
        const Term node = term_of(c.at("node"));
        const auto bytes = m2::emit(node);
        if (c.at("emit").is_null()) {
            CB_CHECK(!bytes.has_value());
            ++declined;
            continue;
        }
        ++emitted;
        CB_CHECK(bytes.has_value());
        if (!bytes) continue;
        CB_CHECK_EQ(hex_of(*bytes), c.at("emit").get<std::string>());
        if (c.at("c").is_null()) continue;
        if (m2::to_c(node) != c.at("c").get<std::string>()) testing::record_failure(__FILE__, __LINE__, name + ": to_c gave '" + m2::to_c(node) + "', expected '" + c.at("c").get<std::string>() + "'");
        testing::count_check();
        check_reading(name, m2::invert(*bytes), c.at("invert"));
        read_back += c.at("invert").is_null() ? 0 : 1;
        if (c.contains("interp") && !c.at("interp").is_object()) check_interp(name, m2::interp(*bytes, term_of(c.at("node")).at("nargs").number()), c.at("interp"));
    }
    CB_CHECK_EQ(emitted, std::size_t{69});
    CB_CHECK_EQ(declined, std::size_t{2});
    CB_CHECK_EQ(read_back, std::size_t{69});
});

CB_TEST("invert.msvc2.real_functions_are_declined_as_recorded", [] {
    std::size_t rows = 0, read = 0;
    for (const Json& r : golden().at("real")) {
        const Bytes code = from_hex(r.at("code").get<std::string>());
        const std::string where = r.at("module").get<std::string>() + " " + r.at("code").get<std::string>().substr(0, 24);
        const Json& inv = r.at("invert");
        try {
            const auto got = m2::invert(code);
            if (inv.is_object() && inv.contains("exc")) testing::record_failure(__FILE__, __LINE__, where + ": the golden expects the error " + inv.at("exc").get<std::string>());
            else check_reading(where, got, inv);
            read += got ? 1 : 0;
        } catch (const Error&) {
            if (!(inv.is_object() && inv.contains("exc"))) testing::record_failure(__FILE__, __LINE__, where + ": threw where an answer is expected");
        }
        try {
            const auto it = m2::interp(code, r.at("nargs").get<long long>());
            if (r.at("interp").is_object() && r.at("interp").contains("exc")) testing::record_failure(__FILE__, __LINE__, where + ": interp: the golden expects an error");
            else check_interp(where + " interp", it, r.at("interp"));
        } catch (const Error&) {
            if (!(r.at("interp").is_object() && r.at("interp").contains("exc"))) testing::record_failure(__FILE__, __LINE__, where + ": interp threw where an answer is expected");
        }
        ++rows;
        testing::count_check();
    }
    CB_CHECK(rows > 100);
});

CB_TEST("invert.msvc2.the_batteries_are_the_recorded_ones", [] {
    const auto cases = m2::battery();
    const auto declined = m2::declined_battery();
    const Json& golden_cases = golden().at("cases");
    CB_CHECK_EQ(cases.size() + declined.size(), golden_cases.size());
    for (std::size_t i = 0; i < golden_cases.size(); ++i) {
        const m2::Case& c = i < cases.size() ? cases[i] : declined[i - cases.size()];
        CB_CHECK_EQ(c.name, golden_cases.at(i).at("name").get<std::string>());
        CB_CHECK(c.node == term_of(golden_cases.at(i).at("node")));
    }
    const auto frames = m2::frame_battery();
    CB_CHECK_EQ(frames.size(), golden().at("frames").size());
    for (std::size_t i = 0; i < frames.size() && i < golden().at("frames").size(); ++i) {
        CB_CHECK_EQ(frames[i].name, golden().at("frames").at(i).at("name").get<std::string>());
        CB_CHECK_EQ(frames[i].source, golden().at("frames").at(i).at("source").get<std::string>());
        CB_CHECK_EQ(frames[i].locals.size(), golden().at("frames").at(i).at("locals").size());
    }
});

CB_TEST("invert.msvc2.frame_layout", [] {
    for (const Json& f : golden().at("frames")) {
        std::vector<m2::Local> locals;
        for (const Json& l : f.at("locals")) locals.push_back({l.at(0).get<std::uint32_t>(), l.at(1).get<bool>()});
        const m2::FrameLayout layout = m2::frame_layout(locals);
        CB_CHECK_EQ(layout.frame_size, f.at("frame").get<std::uint32_t>());
        std::map<std::size_t, std::int64_t> by_index(layout.offsets.begin(), layout.offsets.end());
        CB_CHECK_EQ(by_index.size(), f.at("offsets").size());
        for (const Json& o : f.at("offsets")) CB_CHECK_EQ(by_index[o.at(0).get<std::size_t>()], o.at(1).get<std::int64_t>());
    }
    // scalars first (nearest ebp), then arrays; each rounded up to 4
    const std::vector<m2::Local> locals{{6, true}, {4, false}};
    const m2::FrameLayout layout = m2::frame_layout(locals);
    CB_CHECK_EQ(layout.frame_size, 12u);
    CB_CHECK_EQ(layout.offsets.at(0).first, std::size_t{1});
    CB_CHECK_EQ(layout.offsets.at(0).second, std::int64_t{-4});
    CB_CHECK_EQ(layout.offsets.at(1).second, std::int64_t{-12});
    CB_CHECK_EQ(m2::frame_layout({}).frame_size, 0u);
});

CB_TEST("invert.msvc2.malformed_constructs_and_functions_are_errors_or_declined", [] {
    CB_CHECK(!m2::emit(Term::dict()).has_value());                                           // no parts
    CB_CHECK(!m2::emit(term_of(Json::parse(R"({"d":{"nargs":0,"expr":null}})"))).has_value());   // nothing to return
    CB_CHECK(!m2::invert(Bytes{}).has_value());
    CB_CHECK(!m2::invert(from_hex("90909090")).has_value());
    CB_CHECK(!m2::interp(Bytes{}, 0).has_value());
    const auto zero = m2::invert(from_hex("33c0c3"));
    CB_CHECK(zero.has_value());
    if (zero) CB_CHECK_EQ(zero->c, std::string("int __stdcall f(void) { return 0; }"));
    const auto big = m2::invert(from_hex("b834120000c3"));
    CB_CHECK(big.has_value());
    if (big) CB_CHECK_EQ(big->c, std::string("int __stdcall f(void) { return 4660; }"));
});
