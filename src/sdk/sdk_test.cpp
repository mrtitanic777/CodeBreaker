// sdk/sdk_test.cpp -- every call of the C interface (include/cb), on the repository's own test files: what it returns, what it says
// when it fails, and that a record points into the handle that gave it.
#include <cstring>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/binary.h"
#include "cb/cb.h"
#include "cb/decompile.h"
#include "cb/disasm.h"
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

stdfs::path sdk_checkout() { return stdfs::path(CB_SOURCE_DIR).parent_path(); }
std::string sdk_testdata(std::string_view name) { return cb::fs::to_text(stdfs::path(CB_SOURCE_DIR) / "formats" / "testdata" / name); }

// cb_init over the checkout with a state folder of its own; the settings cb_init sets for the process are put back afterwards, so the
// tests after this one see the environment they had.
class SdkSession {
public:
    SdkSession()
        : state_("cb-sdk-test"),
          root_(cb::fs::to_text(sdk_checkout())),
          keep_{{"CB_CHECKOUT_ROOT"}, {"CB_STATE"}, {"CB_KNOWLEDGE_DB"}, {"CB_LANGUAGES_DIR"}, {"CB_DECOMPILER_BIN"}, {"CB_THIRD_PARTY_BIN"}} {
        const std::string state = cb::fs::to_text(state_.path());
        cb_init_options o = CB_INIT_OPTIONS_INIT;
        o.data_root = root_.c_str();
        o.state_dir = state.c_str();
        status_ = cb_init(&o);
    }
    ~SdkSession() {
        if (status_ == CB_OK) cb_shutdown();
    }
    cb_status status() const { return status_; }
    const stdfs::path& state() const { return state_.path(); }

private:
    struct Kept {
        Kept(std::string_view name) : env(name, cb::fs::get_env(name)) {}   // (not explicit: the array above is made from names)
        cb::fs::ScopedEnv env;
    };
    cb::fs::ScopedTempDir state_;
    std::string root_;
    Kept keep_[6];
    cb_status status_ = CB_ERROR_INTERNAL;
};

bool sdk_says(std::string_view words) { return std::string_view(cb_last_error()).find(words) != std::string_view::npos; }

}  // namespace

CB_TEST("sdk.version", [] {
    CB_CHECK_EQ(cb_version(), static_cast<std::uint32_t>(CB_SDK_VERSION));
    CB_CHECK_EQ(std::string(cb_version_string()), std::string(CB_SDK_VERSION_STRING));
    CB_CHECK_EQ(CB_SDK_MAKE_VERSION(1, 2, 3), 0x010203);
    CB_CHECK_EQ(std::string(cb_status_name(CB_OK)), std::string("CB_OK"));
    CB_CHECK_EQ(std::string(cb_status_name(CB_ERROR_BUFFER_TOO_SMALL)), std::string("CB_ERROR_BUFFER_TOO_SMALL"));
    for (int s = CB_OK; s <= CB_ERROR_INTERNAL; ++s) CB_CHECK(std::string(cb_status_name(static_cast<cb_status>(s))).starts_with("CB_"));
    CB_CHECK_EQ(std::string(cb_status_name(static_cast<cb_status>(99))), std::string("CB_ERROR_UNKNOWN"));
});

CB_TEST("sdk.init", [] {
    // before cb_init: the calls that do work say to call it
    cb_binary* b = reinterpret_cast<cb_binary*>(1);
    CB_CHECK_EQ(cb_binary_open(sdk_testdata("lz32.dll").c_str(), &b), CB_ERROR_NOT_INITIALIZED);
    CB_CHECK(b == nullptr);
    CB_CHECK(sdk_says("cb_binary_open: the library is not set up: call cb_init(NULL)"));
    cb_instructions* none = nullptr;
    CB_CHECK_EQ(cb_disasm("z80", "\xC9", 1, 0, 0, &none), CB_ERROR_NOT_INITIALIZED);
    CB_CHECK_EQ(cb_decompiler_check(), CB_ERROR_NOT_INITIALIZED);
    cb_shutdown();   // (no cb_init: nothing happens)

    cb_init_options wrong = CB_INIT_OPTIONS_INIT;
    wrong.struct_size = 4;
    CB_CHECK_EQ(cb_init(&wrong), CB_ERROR_INVALID_ARGUMENT);
    CB_CHECK(sdk_says("struct_size is 4"));
    cb::fs::ScopedTempDir empty("cb-sdk-empty");
    const std::string empty_text = cb::fs::to_text(empty.path());
    cb_init_options nowhere = CB_INIT_OPTIONS_INIT;
    nowhere.data_root = empty_text.c_str();
    CB_CHECK_EQ(cb_init(&nowhere), CB_ERROR_NOT_FOUND);
    CB_CHECK(sdk_says("has no config folder"));

    SdkSession session;
    CB_CHECK_EQ(session.status(), CB_OK);
    CB_CHECK_EQ(cb_init(nullptr), CB_OK);   // a second user, the same set-up
    cb_shutdown();
    CB_CHECK_EQ(cb_init(&nowhere), CB_ERROR_INVALID_ARGUMENT);   // another data root than the first
    CB_CHECK(sdk_says("already set up"));
    CB_CHECK_EQ(cb::fs::get_env("CB_STATE").value_or(""), cb::fs::to_text(session.state()));

    cb_string s{nullptr, 0};
    cb_string_free(&s);
    cb_string_free(nullptr);
});

CB_TEST("sdk.binary.pe", [] {
    SdkSession session;
    SDK_REQUIRE(session.status() == CB_OK);
    const std::string path = sdk_testdata("lz32.dll");
    cb_binary* b = nullptr;
    SDK_REQUIRE(cb_binary_open(path.c_str(), &b) == CB_OK);
    CB_CHECK_EQ(std::string(cb_binary_path(b)), path);
    CB_CHECK_EQ(std::string(cb_binary_format(b)), std::string("PE"));
    CB_CHECK_EQ(std::string(cb_binary_kind(b)), std::string("pe"));
    CB_CHECK_EQ(std::string(cb_binary_arch(b)), std::string("x86-32"));
    CB_CHECK_EQ(cb_binary_size(b), static_cast<std::uint64_t>(stdfs::file_size(cb::fs::from_text(path))));
    CB_CHECK(cb_binary_image_base(b) != 0);
    const cb_address entry = cb_binary_entry(b);
    CB_CHECK_EQ(entry.style, static_cast<std::uint32_t>(CB_ADDRESS_LINEAR));
    CB_CHECK(entry.offset > cb_binary_image_base(b));

    std::size_t n = 0;
    const cb_section* sections = cb_binary_sections(b, &n);
    SDK_REQUIRE(n > 0 && sections != nullptr);
    bool text = false;
    for (std::size_t i = 0; i < n; ++i)
        if (std::string(sections[i].name) == ".text") text = sections[i].code == 1 && sections[i].address.style == CB_ADDRESS_LINEAR;
    CB_CHECK(text);

    const cb_import* imports = cb_binary_imports(b, &n);
    SDK_REQUIRE(n > 0 && imports != nullptr);
    bool kernel32 = false;
    for (std::size_t i = 0; i < n; ++i) kernel32 |= std::string_view(imports[i].module).starts_with("KERNEL32") && imports[i].name != nullptr;
    CB_CHECK(kernel32);

    const cb_export* exports = cb_binary_exports(b, &n);
    SDK_REQUIRE(n > 0 && exports != nullptr);
    bool lzcopy = false;
    for (std::size_t i = 0; i < n; ++i) lzcopy |= exports[i].name != nullptr && std::string(exports[i].name) == "LZCopy" && exports[i].forward == nullptr;
    CB_CHECK(lzcopy);

    // the functions the file names, in address order, each pointing back at its binary
    const std::size_t count = cb_binary_function_count(b);
    SDK_REQUIRE(count >= n);
    for (std::size_t i = 0; i + 1 < count; ++i) CB_CHECK(cb_function_address(cb_binary_function(b, i)).offset < cb_function_address(cb_binary_function(b, i + 1)).offset);
    CB_CHECK(cb_binary_function(b, count) == nullptr);
    const cb_function* f = nullptr;
    SDK_REQUIRE(cb_binary_function_find(b, "LZCopy", &f) == CB_OK);
    CB_CHECK_EQ(std::string(cb_function_name(f)), std::string("LZCopy"));
    CB_CHECK(cb_function_binary(f) == b);
    const cb_function* same = nullptr;
    SDK_REQUIRE(cb_binary_function_at(b, cb_function_address(f), &same) == CB_OK);
    CB_CHECK(same == f);
    const cb_function* entry_fn = nullptr;
    CB_CHECK_EQ(cb_binary_function_find(b, "entry", &entry_fn), CB_OK);
    CB_CHECK_EQ(cb_binary_function_find(b, "NoSuchFunction", &entry_fn), CB_ERROR_NOT_FOUND);
    CB_CHECK(entry_fn == nullptr);
    CB_CHECK(sdk_says("names no function NoSuchFunction"));

    const cb_function* made = nullptr;
    SDK_REQUIRE(cb_binary_function_at(b, cb_address{0x12345678, 0, CB_ADDRESS_LINEAR}, &made) == CB_OK);
    CB_CHECK_EQ(std::string(cb_function_name(made)), std::string("sub_12345678"));
    CB_CHECK_EQ(cb_binary_function_count(b), count);   // (made, not named: not counted)
    CB_CHECK_EQ(cb_binary_function_at(b, cb_address{0, 0, CB_ADDRESS_NONE}, &made), CB_ERROR_INVALID_ARGUMENT);
    cb_binary_close(b);
    cb_binary_close(nullptr);
});

CB_TEST("sdk.binary.ne", [] {
    SdkSession session;
    SDK_REQUIRE(session.status() == CB_OK);
    cb_binary* b = nullptr;
    SDK_REQUIRE(cb_binary_open(sdk_testdata("lzexpand.dll").c_str(), &b) == CB_OK);
    CB_CHECK_EQ(std::string(cb_binary_format(b)), std::string("NE"));
    CB_CHECK_EQ(std::string(cb_binary_arch(b)), std::string("x86-16"));
    std::size_t n = 0;
    const cb_section* sections = cb_binary_sections(b, &n);
    SDK_REQUIRE(n > 0);
    CB_CHECK_EQ(sections[0].address.style, static_cast<std::uint32_t>(CB_ADDRESS_NE_SEGMENT));
    CB_CHECK_EQ(sections[0].address.segment, 1u);
    const cb_function* f = nullptr;
    SDK_REQUIRE(cb_binary_function_find(b, "LZCOPY", &f) == CB_OK);
    CB_CHECK_EQ(cb_function_address(f).style, static_cast<std::uint32_t>(CB_ADDRESS_NE_SEGMENT));
    const cb_function* made = nullptr;
    SDK_REQUIRE(cb_binary_function_at(b, cb_address{0x116, 1, CB_ADDRESS_NE_SEGMENT}, &made) == CB_OK);
    CB_CHECK(std::string(cb_function_name(made)) == "sub_1_0116" || cb_function_address(made).offset == 0x116);
    cb_binary_close(b);

    // a file that is not an executable: identified (or not), with no parts
    cb_binary* text = nullptr;
    SDK_REQUIRE(cb_binary_open(cb::fs::to_text(sdk_checkout() / "LICENSE").c_str(), &text) == CB_OK);
    CB_CHECK_EQ(std::string(cb_binary_arch(text)), std::string(""));
    CB_CHECK(cb_binary_sections(text, &n) == nullptr && n == 0);
    CB_CHECK(cb_binary_imports(text, &n) == nullptr && n == 0);
    CB_CHECK(cb_binary_exports(text, &n) == nullptr && n == 0);
    CB_CHECK_EQ(cb_binary_function_count(text), std::size_t{0});
    CB_CHECK_EQ(cb_binary_entry(text).style, static_cast<std::uint32_t>(CB_ADDRESS_NONE));
    cb_binary_close(text);
});

CB_TEST("sdk.binary.errors", [] {
    SdkSession session;
    SDK_REQUIRE(session.status() == CB_OK);
    cb_binary* b = nullptr;
    CB_CHECK_EQ(cb_binary_open(cb::fs::to_text(sdk_checkout() / "no_such_file.dll").c_str(), &b), CB_ERROR_NOT_FOUND);
    CB_CHECK(sdk_says("no_such_file.dll: no such file"));
    CB_CHECK_EQ(cb_binary_open(cb::fs::to_text(sdk_checkout() / "src").c_str(), &b), CB_ERROR_INVALID_ARGUMENT);
    CB_CHECK(sdk_says("is a folder"));
    CB_CHECK_EQ(cb_binary_open(nullptr, &b), CB_ERROR_INVALID_ARGUMENT);
    CB_CHECK_EQ(cb_binary_open("x", nullptr), CB_ERROR_INVALID_ARGUMENT);
    // a NULL binary reads as empty
    std::size_t n = 7;
    CB_CHECK(cb_binary_sections(nullptr, &n) == nullptr && n == 0);
    CB_CHECK_EQ(std::string(cb_binary_format(nullptr)) + cb_binary_kind(nullptr) + cb_binary_arch(nullptr) + cb_binary_path(nullptr), std::string(""));
    CB_CHECK_EQ(cb_binary_size(nullptr) + cb_binary_image_base(nullptr), std::uint64_t{0});
    CB_CHECK(cb_binary_function(nullptr, 0) == nullptr);
    CB_CHECK_EQ(std::string(cb_function_name(nullptr)), std::string(""));
    CB_CHECK_EQ(cb_function_address(nullptr).style, static_cast<std::uint32_t>(CB_ADDRESS_NONE));
    CB_CHECK(cb_function_binary(nullptr) == nullptr);
    const cb_function* f = nullptr;
    CB_CHECK_EQ(cb_binary_function_find(nullptr, "x", &f), CB_ERROR_INVALID_ARGUMENT);
    CB_CHECK_EQ(cb_binary_function_at(nullptr, cb_address{1, 0, CB_ADDRESS_LINEAR}, &f), CB_ERROR_INVALID_ARGUMENT);
});

CB_TEST("sdk.address_format", [] {
    char buffer[32];
    std::size_t needed = 0;
    auto text = [&](cb_address a) {
        CB_CHECK_EQ(cb_address_format(&a, buffer, sizeof buffer, &needed), CB_OK);
        CB_CHECK_EQ(needed, std::strlen(buffer) + 1);
        return std::string(buffer);
    };
    CB_CHECK_EQ(text({0x10001000, 0, CB_ADDRESS_LINEAR}), std::string("0x10001000"));
    CB_CHECK_EQ(text({0x1a4, 3, CB_ADDRESS_NE_SEGMENT}), std::string("3:01a4"));
    CB_CHECK_EQ(text({0x10, 0x1234, CB_ADDRESS_REAL_MODE}), std::string("1234:0010"));
    CB_CHECK_EQ(text({0x1000, 1, CB_ADDRESS_LE_OBJECT}), std::string("1:00001000"));
    CB_CHECK_EQ(text({0, 0, CB_ADDRESS_NONE}), std::string(""));
    const cb_address a{0xFFFFFFFFFFFFFFFFull, 0, CB_ADDRESS_LINEAR};
    char small[4];
    CB_CHECK_EQ(cb_address_format(&a, small, sizeof small, &needed), CB_ERROR_BUFFER_TOO_SMALL);
    CB_CHECK_EQ(needed, std::size_t{19});
    CB_CHECK(sdk_says("needs 19 bytes"));
    CB_CHECK_EQ(cb_address_format(&a, nullptr, 0, &needed), CB_ERROR_BUFFER_TOO_SMALL);
    CB_CHECK_EQ(cb_address_format(nullptr, buffer, sizeof buffer, nullptr), CB_ERROR_INVALID_ARGUMENT);
    CB_CHECK_EQ(cb_address_format(&a, nullptr, 8, nullptr), CB_ERROR_INVALID_ARGUMENT);
});

CB_TEST("sdk.disasm", [] {
    // the processor table needs no cb_init
    std::size_t n = 0;
    const cb_arch* archs = cb_arch_list(&n);
    SDK_REQUIRE(n > 10 && archs != nullptr);
    const cb_arch* z80 = nullptr;
    SDK_REQUIRE(cb_arch_find("Z80", &z80) == CB_OK);
    CB_CHECK_EQ(std::string(z80->name), std::string("z80"));
    CB_CHECK_EQ(z80->described, 1u);
    CB_CHECK_EQ(z80->big_endian, 0u);
    const cb_arch* i386 = nullptr;
    SDK_REQUIRE(cb_arch_find("i386", &i386) == CB_OK);
    CB_CHECK_EQ(std::string(i386->name), std::string("x86-32"));
    CB_CHECK(std::string(i386->aliases).find("i386") != std::string::npos);
    CB_CHECK_EQ(cb_arch_find("no-such-cpu", &i386), CB_ERROR_NOT_FOUND);
    CB_CHECK(i386 == nullptr);
    CB_CHECK(sdk_says("the names are: "));
    CB_CHECK_EQ(cb_arch_find(nullptr, &i386), CB_ERROR_INVALID_ARGUMENT);

    SdkSession session;
    SDK_REQUIRE(session.status() == CB_OK);
    const std::uint8_t z80_code[] = {0x3E, 0x12, 0xCD, 0x34, 0x12, 0xC9};
    cb_instructions* list = nullptr;
    SDK_REQUIRE(cb_disasm("z80", z80_code, sizeof z80_code, 0x100, 0, &list) == CB_OK);
    const cb_instruction* ins = cb_instructions_data(list, &n);
    SDK_REQUIRE(n == 3);
    CB_CHECK_EQ(ins[0].address, std::uint64_t{0x100});
    CB_CHECK_EQ(ins[0].length, 2u);
    CB_CHECK_EQ(std::string(ins[0].mnemonic) + " " + ins[0].operands, std::string("LD A,0x12"));
    CB_CHECK_EQ(ins[1].flow, static_cast<std::uint32_t>(CB_FLOW_CALL));
    CB_CHECK_EQ(ins[1].has_target, 1u);
    CB_CHECK_EQ(ins[1].target, std::uint64_t{0x1234});
    CB_CHECK_EQ(ins[2].flow, static_cast<std::uint32_t>(CB_FLOW_RETURN));
    cb_instructions_free(list);

    // count, and bytes that are no instruction
    const std::uint8_t x86[] = {0x55, 0x8B, 0xEC, 0x0F, 0xFF, 0xC3};
    SDK_REQUIRE(cb_disasm("x86-32", x86, sizeof x86, 0x401000, 2, &list) == CB_OK);
    cb_instructions_data(list, &n);
    CB_CHECK_EQ(n, std::size_t{2});
    cb_instructions_free(list);
    SDK_REQUIRE(cb_disasm("x86-32", x86 + 3, 2, 0, 0, &list) == CB_OK);
    ins = cb_instructions_data(list, &n);
    SDK_REQUIRE(n >= 1);
    CB_CHECK_EQ(ins[0].flow, static_cast<std::uint32_t>(CB_FLOW_INVALID));
    CB_CHECK(ins[0].length >= 1);
    cb_instructions_free(list);
    SDK_REQUIRE(cb_disasm("z80", nullptr, 0, 0, 0, &list) == CB_OK);
    CB_CHECK(cb_instructions_data(list, &n) == nullptr && n == 0);
    cb_instructions_free(list);
    cb_instructions_free(nullptr);

    CB_CHECK_EQ(cb_disasm("no-such-cpu", x86, sizeof x86, 0, 0, &list), CB_ERROR_NOT_FOUND);
    CB_CHECK(list == nullptr);
    CB_CHECK_EQ(cb_disasm("z80", nullptr, 4, 0, 0, &list), CB_ERROR_INVALID_ARGUMENT);
    archs = cb_arch_list(&n);
    for (std::size_t i = 0; i < n; ++i)
        if (archs[i].described == 0) {
            CB_CHECK_EQ(cb_disasm(archs[i].name, x86, sizeof x86, 0, 0, &list), CB_ERROR_UNSUPPORTED);
            CB_CHECK(sdk_says("has no SLEIGH description"));
            break;
        }
});

CB_TEST("sdk.decompile", [] {
    SdkSession session;
    SDK_REQUIRE(session.status() == CB_OK);
    SDK_REQUIRE(cb_decompiler_check() == CB_OK);
    cb_binary* b = nullptr;
    SDK_REQUIRE(cb_binary_open(sdk_testdata("lz32.dll").c_str(), &b) == CB_OK);
    const cb_function* f = nullptr;
    SDK_REQUIRE(cb_binary_function_find(b, "LZCopy", &f) == CB_OK);
    cb_string c{nullptr, 0};
    SDK_REQUIRE(cb_function_decompile(f, nullptr, &c) == CB_OK);
    CB_CHECK(c.data != nullptr && c.size == std::strlen(c.data));
    CB_CHECK(std::string_view(c.data).find("LZCopy") != std::string_view::npos);
    cb_string_free(&c);
    CB_CHECK(c.data == nullptr && c.size == 0);

    cb_decompile_options options = CB_DECOMPILE_OPTIONS_INIT;
    options.language = "C";
    CB_CHECK_EQ(cb_function_decompile(f, &options, &c), CB_OK);
    cb_string_free(&c);
    options.language = "pascal";
    CB_CHECK_EQ(cb_function_decompile(f, &options, &c), CB_ERROR_UNSUPPORTED);
    CB_CHECK(sdk_says("\"pascal\" is not written"));
    options.struct_size = 1;
    CB_CHECK_EQ(cb_module_decompile(b, &options, &c, nullptr), CB_ERROR_INVALID_ARGUMENT);

    const cb_function* nowhere = nullptr;
    SDK_REQUIRE(cb_binary_function_at(b, cb_address{0x10, 0, CB_ADDRESS_LINEAR}, &nowhere) == CB_OK);
    CB_CHECK_EQ(cb_function_decompile(nowhere, nullptr, &c), CB_ERROR_NOT_FOUND);

    cb_module_info info{};
    SDK_REQUIRE(cb_module_decompile(b, nullptr, &c, &info) == CB_OK);
    CB_CHECK(info.found > 0 && info.decompiled > 0 && info.decompiled <= info.found);
    CB_CHECK(std::string_view(c.data).find("LZCopy") != std::string_view::npos);
    cb_string_free(&c);
    CB_CHECK_EQ(cb_function_decompile(nullptr, nullptr, &c), CB_ERROR_INVALID_ARGUMENT);
    CB_CHECK_EQ(cb_module_decompile(nullptr, nullptr, &c, nullptr), CB_ERROR_INVALID_ARGUMENT);
    cb_binary_close(b);

    // a file that is not an executable
    cb_binary* text = nullptr;
    SDK_REQUIRE(cb_binary_open(cb::fs::to_text(sdk_checkout() / "LICENSE").c_str(), &text) == CB_OK);
    CB_CHECK_EQ(cb_module_decompile(text, nullptr, &c, nullptr), CB_ERROR_UNSUPPORTED);
    cb_binary_close(text);
});
