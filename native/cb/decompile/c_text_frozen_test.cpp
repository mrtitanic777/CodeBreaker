// The recorded cases of the C text's private passes (testdata/frozen/c_text): each pass was given real inputs once, and what it
// answered was kept; the same inputs go to the same passes here and the same answers are wanted. The
// five here are the ones the other C text tests do not replay (the interrupt passes are checked in c_text_test.cpp).
//
//   _ram_type      the C type of a 32-bit / PowerPC global from the decompiler's prefix
//   _global_type   the same for a 16-bit module (c_text's global_type16)
//   _far_loads     the argument words a 16-bit function loads as far pointers
//   _loads_dgroup  whether a 16-bit function's prologue loads DS with the module's own data
//   _more_helpers  the joins, pieces and widenings written for every pair of sizes
//
// A window `{"$w": [start, hex]}` is a large buffer read only there (zeros elsewhere).
#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/decompile/c_text_internal.hpp"

namespace cti = cb::decompile::c_text::internal;
using cb::Json;

namespace {

Json frozen_cases(const char* pass) {
    return cb::json::load(cb::testing::data_path(std::string("decompile/testdata/frozen/c_text/") + pass + ".json"));
}

// A recorded window as the code buffer: zeros up to the window's start, then its bytes.
cb::Bytes frozen_window(const Json& w) {
    const Json& p = w.at("$w");
    const std::size_t start = p.at(0).get<std::size_t>();
    cb::Bytes bytes(start, 0);
    const std::string hex = p.at(1).get<std::string>();
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) bytes.push_back(static_cast<std::uint8_t>(std::stoi(hex.substr(i, 2), nullptr, 16)));
    return bytes;
}

}  // namespace

CB_TEST("decompile.c_text.frozen_ram_type", [] {
    const Json g = frozen_cases("_ram_type");
    CB_CHECK_EQ(g["cases"].size(), std::size_t{10});
    for (const Json& c : g["cases"]) {
        const auto& a = c["args"];
        CB_CHECK_EQ(cti::ram_type(a[0].get<std::string>(), a[1].get<std::int64_t>(), a[2].get<bool>()), c["result"].get<std::string>());
    }
});

CB_TEST("decompile.c_text.frozen_global_type", [] {
    const Json g = frozen_cases("_global_type");
    CB_CHECK_EQ(g["cases"].size(), std::size_t{9});
    for (const Json& c : g["cases"]) {
        const auto& a = c["args"];
        CB_CHECK_EQ(cti::global_type16(a[0].get<std::string>(), a[1].get<std::int64_t>(), std::nullopt), c["result"].get<std::string>());
    }
});

CB_TEST("decompile.c_text.frozen_far_loads", [] {
    const Json g = frozen_cases("_far_loads");
    CB_CHECK_EQ(g["cases"].size(), std::size_t{10});
    std::size_t found = 0;
    for (const Json& c : g["cases"]) {
        const auto& a = c["args"];
        const cb::Bytes code = frozen_window(a[0]);
        const std::set<std::int64_t> got = cti::far_loads16(code, a[1].get<std::int64_t>(), a[2].get<std::int64_t>());
        std::set<std::int64_t> want;
        for (const Json& x : c["result"].at("$s")) want.insert(x.get<std::int64_t>());
        CB_CHECK(got == want);
        found += got.size();
    }
    CB_CHECK_EQ(found, std::size_t{1});   // (one function of the ten loads a far pointer from its arguments: `les bx, [bp+6]`)
});

CB_TEST("decompile.c_text.frozen_loads_dgroup", [] {
    const Json g = frozen_cases("_loads_dgroup");
    CB_CHECK_EQ(g["cases"].size(), std::size_t{10});
    std::size_t loads = 0;
    for (const Json& c : g["cases"]) {
        const auto& a = c["args"];
        const cb::Bytes code = frozen_window(a[0]);
        const bool got = cti::loads_dgroup16(code, a[1].get<std::int64_t>(), a[2].get<std::int64_t>());
        CB_CHECK_EQ(got, c["result"].get<bool>());
        loads += got;
    }
    CB_CHECK_EQ(loads, std::size_t{2});
});

// What the core writes beyond the helpers spelled out in the header text: x86_helpers() takes
// the header's own helpers_text(), which must be that text.
CB_TEST("decompile.c_text.frozen_more_helpers", [] {
    const Json g = frozen_cases("_more_helpers");
    CB_CHECK_EQ(g["cases"].size(), std::size_t{1});
    const Json& c = g["cases"][0];
    const std::string have = c["args"][0].get<std::string>();
    CB_CHECK_EQ(std::string(cti::helpers_text()), have);
    const std::string want = c["result"].get<std::string>();
    const std::string got = cti::x86_helpers();
    CB_CHECK(got.find("#define CONCAT14(a,b)") != std::string::npos);
    CB_CHECK_EQ(got.size(), want.size());
    CB_CHECK_EQ(got, want);
});
