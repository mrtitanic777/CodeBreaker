// sdk/sdk_cpp_test.cpp -- cb.hpp, the C++ wrappers: RAII, spans and ranges over the records, errors as exceptions. (The header is compiled
// here at the tree's warning level, so a wrapper that does not build cleanly fails the build.)
#include <algorithm>
#include <filesystem>
#include <string>
#include <utility>

#include "cb/cb.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

// A check the rest of the test cannot go on without.
#ifndef SDK_REQUIRE
#define SDK_REQUIRE(...)                                             \
    do {                                                             \
        const bool sdk_require_ok_ = static_cast<bool>(__VA_ARGS__); \
        CB_CHECK(sdk_require_ok_ && #__VA_ARGS__);                   \
        if (!sdk_require_ok_) return;                                \
    } while (0)
#endif

namespace {

namespace stdfs = std::filesystem;

std::string cpp_testdata(std::string_view name) { return cb::fs::to_text(stdfs::path(CB_SOURCE_DIR) / "formats" / "testdata" / name); }

// cb::sdk::Session over the checkout, the process's settings put back afterwards (sdk_test.cpp's SdkSession, for the wrapper).
struct CppSession {
    cb::fs::ScopedEnv keep[6] = {{"CB_CHECKOUT_ROOT", cb::fs::get_env("CB_CHECKOUT_ROOT")}, {"CB_STATE", cb::fs::get_env("CB_STATE")},
                                 {"CB_KNOWLEDGE_DB", cb::fs::get_env("CB_KNOWLEDGE_DB")},   {"CB_LANGUAGES_DIR", cb::fs::get_env("CB_LANGUAGES_DIR")},
                                 {"CB_DECOMPILER_BIN", cb::fs::get_env("CB_DECOMPILER_BIN")}, {"CB_THIRD_PARTY_BIN", cb::fs::get_env("CB_THIRD_PARTY_BIN")}};
    cb::fs::ScopedTempDir state{"cb-sdk-cpp"};
    cb::sdk::Session session{{cb::fs::to_text(stdfs::path(CB_SOURCE_DIR).parent_path()), cb::fs::to_text(state.path())}};
};

}  // namespace

CB_TEST("sdk.cpp.binary", [] {
    CppSession s;
    CB_CHECK(cb::sdk::compatible());
    CB_CHECK_EQ(cb::sdk::version_text(), std::string_view(CB_SDK_VERSION_STRING));
    cb::sdk::Binary b = cb::sdk::Binary::open(cpp_testdata("lz32.dll"));
    CB_CHECK_EQ(b.format(), std::string_view("PE"));
    CB_CHECK_EQ(b.arch(), std::string_view("x86-32"));
    CB_CHECK(std::any_of(b.sections().begin(), b.sections().end(), [](const cb_section& x) { return std::string_view(x.name) == ".text"; }));
    CB_CHECK(!b.imports().empty() && !b.exports().empty());
    std::size_t named = 0;
    for (const cb::sdk::Function f : b.functions()) named += f ? 1 : 0;
    CB_CHECK_EQ(named, b.functions().size());
    const cb::sdk::Function lz = b.function("LZCopy");
    CB_CHECK_EQ(lz.name(), std::string_view("LZCopy"));
    CB_CHECK_EQ(cb::sdk::address_text(lz.address()).substr(0, 2), std::string("0x"));
    CB_CHECK(b.function_at(lz.address()).get() == lz.get());
    CB_CHECK_EQ(b.function_at(cb::sdk::linear(0x10)).name(), std::string_view("sub_10"));

    cb::sdk::Binary moved = std::move(b);
    CB_CHECK(b.get() == nullptr && moved.get() != nullptr);   // NOLINT(bugprone-use-after-move): the moved-from handle is empty
    try {
        (void)moved.function("NoSuchFunction");
        CB_CHECK(false);
    } catch (const cb::sdk::Error& e) {
        CB_CHECK_EQ(e.status(), CB_ERROR_NOT_FOUND);
        CB_CHECK(std::string_view(e.what()).find("NoSuchFunction") != std::string_view::npos);
    }
    try {
        (void)cb::sdk::Binary::open(cpp_testdata("no_such_file.dll"));
        CB_CHECK(false);
    } catch (const cb::sdk::Error& e) {
        CB_CHECK_EQ(e.status(), CB_ERROR_NOT_FOUND);
    }
});

CB_TEST("sdk.cpp.disasm", [] {
    CppSession s;
    CB_CHECK(cb::sdk::archs().size() > 10);
    CB_CHECK_EQ(std::string_view(cb::sdk::arch("gbz80").name), std::string_view("sm83"));
    const std::uint8_t code[] = {0xA9, 0x12, 0x60};   // LDA #$12, RTS
    cb::sdk::Instructions list = cb::sdk::disasm("6502", code, 0x8000);
    SDK_REQUIRE(list.size() == 2);
    CB_CHECK_EQ(list[0].length, 2u);
    CB_CHECK_EQ(list[1].flow, static_cast<std::uint32_t>(CB_FLOW_RETURN));
    std::size_t bytes = 0;
    for (const cb_instruction& i : list) bytes += i.length;
    CB_CHECK_EQ(bytes, sizeof code);
    cb::sdk::Instructions other = std::move(list);
    CB_CHECK_EQ(other.size(), std::size_t{2});
    try {
        (void)cb::sdk::disasm("no-such-cpu", code, 0);
        CB_CHECK(false);
    } catch (const cb::sdk::Error& e) {
        CB_CHECK_EQ(e.status(), CB_ERROR_NOT_FOUND);
    }
    cb::sdk::check_decompiler();
    cb::sdk::String empty;
    CB_CHECK(empty.view().empty() && empty.size() == 0 && std::string(empty.c_str()).empty());
});
