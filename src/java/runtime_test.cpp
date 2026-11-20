#include <map>
#include <string>
#include <vector>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "java/runtime.hpp"
#include "java/textutil.hpp"

namespace {

using namespace cb::java;

bool rt_has(const std::string& text, std::string_view needle) { return text.find(needle) != std::string::npos; }
namespace stdfs = std::filesystem;

JavaData rt_real_data() { return JavaData(cb::testing::data_path("../config/java")); }

const runtime::Runtime& rt_runtime() {
    static const runtime::Runtime r(rt_real_data());
    return r;
}

std::string rt_digest(const std::string& text) {
    const cb::Bytes bytes(text.begin(), text.end());
    return cb::crypto::to_hex(cb::crypto::sha256(bytes));
}

// testdata/runtime.golden.txt: the runtime's recorded output, as
//   file|<config>|<name>|<size>|<sha256 of the text>      the generated file set of each configuration
//   members|<class>|<parent>|<method=type;...>            STUB_MEMBERS
//   unwired|<canonical headers that are not wired>
struct RtGolden {
    std::map<std::string, std::map<std::string, std::pair<std::size_t, std::string>>> files;   // config -> name -> (size, sha256)
    std::map<std::string, std::pair<std::string, std::string>> members;
    std::string unwired;
};

RtGolden rt_load_golden() {
    RtGolden g;
    const std::string text = cb::fs::read_text(cb::testing::data_path("java/testdata/runtime.golden.txt"));   // (the lines are views of it)
    for (const std::string_view line : cb::text::lines(text)) {
        if (line.empty()) continue;
        const auto parts = cb::text::split(line, '|');
        if (parts[0] == "file") {
            g.files[std::string(parts[1])][std::string(parts[2])] = {*cb::text::parse_int<std::size_t>(parts[3]), std::string(parts[4])};
        } else if (parts[0] == "members") {
            g.members[std::string(parts[1])] = {std::string(parts[2]), std::string(parts[3])};
        } else if (parts[0] == "unwired") {
            g.unwired = parts.size() > 1 ? std::string(parts[1]) : "";
        }
    }
    return g;
}

// (the configuration names of the golden file: its year and platform)
runtime::GenerateOptions rt_options(const std::string& name) {
    static const std::map<std::string, std::pair<int, std::string>> table = {
        {"none", {0, ""}},          {"xbox360-03", {2003, "xbox360"}}, {"pc_opengl-17", {2017, "pc_opengl"}}, {"ps3-11", {2011, "ps3"}},
        {"wii-14", {2014, "wii"}},  {"psp-03", {2003, "psp"}},         {"nds-11", {2011, "nds"}},            {"pc_windows-17", {2017, "pc_windows"}},
        {"generic-std-98", {1998, ""}}};
    const auto& [year, platform] = table.at(name);
    runtime::GenerateOptions o;
    if (year != 0) o.cpp_std = emit::header::CppStd::of_year(year);
    o.platform = platform;
    return o;
}

CB_TEST("java.runtime.file_sets_match_golden", [] {
    const RtGolden golden = rt_load_golden();
    CB_CHECK_EQ(golden.files.size(), std::size_t{9});
    for (const auto& [config, expected] : golden.files) {
        std::map<std::string, std::string, std::less<>> made;
        try {
            made = rt_runtime().generate_files(rt_options(config));
        } catch (const std::exception& e) {
            CB_CHECK_EQ(std::string(e.what()), config);   // names the configuration that threw
        }
        CB_CHECK_EQ(made.size(), expected.size());
        for (const auto& [name, want] : expected) {
            const auto it = made.find(name);
            CB_CHECK(it != made.end());
            if (it == made.end()) continue;
            CB_CHECK_EQ(it->second.size(), want.first);
            CB_CHECK_EQ(rt_digest(it->second), want.second);
        }
    }
});

CB_TEST("java.runtime.stub_members_match_golden", [] {
    const RtGolden golden = rt_load_golden();
    const auto& made = rt_runtime().stub_members();
    CB_CHECK_EQ(made.size(), golden.members.size());
    for (const auto& [cls, want] : golden.members) {
        const auto it = made.find(cls);
        CB_CHECK(it != made.end());
        if (it == made.end()) continue;
        CB_CHECK_EQ(it->second.parent.value_or(""), want.first);
        std::vector<std::string> methods;
        for (const auto& [m, type] : it->second.methods) methods.push_back(m + "=" + type);
        CB_CHECK_EQ(cb::text::join(methods, ";"), want.second);
    }
});

CB_TEST("java.runtime.every_canonical_header_is_wired", [] {
    CB_CHECK(rt_runtime().unwired_canonicals().empty());
    CB_CHECK_EQ(rt_load_golden().unwired, std::string());
});

CB_TEST("java.runtime.unwired_canonical_is_reported", [] {
    cb::fs::ScopedTempDir tmp("cbruntime");
    const stdfs::path java = tmp.path() / "java";
    cb::fs::copy_tree(cb::testing::data_path("../config/java/platform/canonical"), java / "platform" / "canonical");
    cb::fs::write_text(java / "platform" / "canonical" / "Zebra.h", "// a canonical header nothing wires\n");
    cb::fs::write_text(java / "platform" / "canonical" / "notes.txt", "not a header\n");
    const runtime::Runtime r{JavaData(java)};
    CB_CHECK(r.unwired_canonicals() == std::vector<std::string>{"Zebra.h"});
});

CB_TEST("java.runtime.missing_canonical_folder", [] {
    cb::fs::ScopedTempDir tmp("cbruntime");
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)runtime::Runtime(JavaData(tmp.path())));
});

CB_TEST("java.runtime.canonical_headers_are_taken_verbatim", [] {
    const auto& files = rt_runtime().files();
    CB_CHECK_EQ(files.at("Random.h"), textutil::read_text(cb::testing::data_path("../config/java/platform/canonical/Random.h")));
    CB_CHECK(files.contains("JavaRT.h"));
    CB_CHECK(files.contains("GL11.h"));
    CB_CHECK_EQ(files.at("Iterator.h"), "#ifndef ITERATOR_H\n#define ITERATOR_H\n#include \"JIterator.h\"\n#endif\n");
});

// ---- the stub vocabulary

CB_TEST("java.runtime.ns_stub_text", [] {
    runtime::NsStub spec;
    spec.name = "Widget";
    spec.constants = {{"WIDE", 7}};
    spec.void_fns = {"open"};
    spec.ret_fns = {{"count", "int", "0"}};
    spec.includes = {"Gadget"};
    spec.forward_decls = {"Panel"};
    const std::string text = runtime::ns_stub(spec);
    CB_CHECK(text.starts_with("#ifndef WIDGET_H\n#define WIDGET_H\n\n// ============================ EXTERNAL-LIBRARY STUB"));
    CB_CHECK(rt_has(text, "// Stands in for the external Java type Widget.\n"));
    CB_CHECK(rt_has(text, "#include <string>\n#include \"Gadget.h\"\nclass Panel;\n\nnamespace Widget {\n"));
    CB_CHECK(rt_has(text, "\tconst int WIDE = 7;\n"));
    CB_CHECK(rt_has(text, "\tinline void open(...) {}   // J2X-STUB[Widget]: no-op placeholder -- verify not load-bearing\n"));
    CB_CHECK(rt_has(text, "\tinline int count(...) { return 0; }   // J2X-STUB[Widget]: returns default -- verify\n"));
    CB_CHECK(text.ends_with("}\n#endif // WIDGET_H\n"));
});

CB_TEST("java.runtime.banner_names_the_real_package", [] {
    const auto banner = runtime::external_banner("GL15");
    CB_CHECK_EQ(banner.front(), std::string());
    CB_CHECK(rt_has(cb::text::join(banner, "\n"), "// Stands in for org.lwjgl.opengl.GL15.\n"));
    CB_CHECK(banner.back().starts_with("// ====="));
});

CB_TEST("java.runtime.class_stub_text_and_members", [] {
    runtime::ClassStub spec;
    spec.name = "Thing";
    spec.bases = {"Base"};
    spec.includes = {"Base"};
    spec.void_methods = {"close"};
    spec.ret_methods = {{"label", "std::string", "std::string()"}, {"size", "int", "0"}};
    spec.extra = {"static Thing* make(...) { return new Thing(); }"};
    spec.trailer = {"inline Thing* shared() { static Thing t; return &t; }"};
    const std::string text = runtime::class_stub(spec);
    CB_CHECK(rt_has(text, "#include <string>\n#include \"JavaString.h\"\n#include \"Base.h\"\n\nclass Thing : public Base {\npublic:\n\tThing(...) {}\n"));
    CB_CHECK(rt_has(text, "\tvirtual void close(...) {}   // J2X-STUB[Thing]"));
    // a Java String comes back as JavaString
    CB_CHECK(rt_has(text, "\tvirtual JavaString label(...) { return JavaString(); }"));
    CB_CHECK(rt_has(text, "\tvirtual int size(...) { return 0; }"));
    CB_CHECK(rt_has(text, "\tstatic Thing* make(...) { return new Thing(); }\n};\ninline Thing* shared()"));
    // the member types of the runtime's own stub classes
    const auto& members = rt_runtime().stub_members();
    CB_CHECK_EQ(members.at("Element").parent.value(), "Node");
    CB_CHECK_EQ(members.at("Element").methods.at("getAttribute"), "std::string");   // the member type keeps std::string
    CB_CHECK_EQ(members.at("DocumentBuilderFactory").methods.at("newInstance"), "DocumentBuilderFactory*");   // from a verbatim line
    CB_CHECK_EQ(members.at("Toolkit").methods.at("getDefaultToolkit"), "Toolkit*");
});

CB_TEST("java.runtime.buffer_stub_chains", [] {
    const std::string text = runtime::buffer_stub("IntBuffer");
    CB_CHECK(rt_has(text, "\tIntBuffer& flip() { return *this; }\n"));
    CB_CHECK(rt_has(text, "\tIntBuffer& position(int) { return *this; }\n"));
    CB_CHECK(rt_has(text, "\tstatic IntBuffer* allocateDirect(...) { return new IntBuffer(); }\n"));
    CB_CHECK(text.ends_with("};\n#endif // INTBUFFER_H\n"));
});

CB_TEST("java.runtime.keyboard_knows_key_names", [] {
    const std::string& text = rt_runtime().files().at("Keyboard.h");
    CB_CHECK(rt_has(text, "#include <string>\n#include \"JavaString.h\"\n"));
    CB_CHECK(rt_has(text, "\t\tcase 17: return JavaString(\"W\");\n"));
    CB_CHECK(rt_has(text, "\t\tif (name == \"ESCAPE\") return 1;\n"));
    CB_CHECK(!rt_has(text, "getKeyName(...)"));
});

// ---- platform and standard

CB_TEST("java.runtime.platform_and_standard_applied", [] {
    const auto& r = rt_runtime();
    // no platform: the generic stdafx.h and the host file layer; the C library's clocks replace the Windows timers
    const auto plain = r.generate_files();
    CB_CHECK(plain.contains("j2x_host_io.cpp"));
    CB_CHECK(!plain.contains("stdafx.cpp"));
    CB_CHECK(rt_has(plain.at("JavaRT.h"), "return (long long)time(0) * 1000LL;"));
    // the 360 keeps its Windows-style timers, takes stdafx.cpp for the precompiled header, and has no host file layer
    const auto x360 = r.generate_files({.platform = "xbox360"});
    CB_CHECK(x360.contains("stdafx.cpp"));
    CB_CHECK(!x360.contains("j2x_host_io.cpp"));
    // a desktop PC build is given the host file layer too
    CB_CHECK(r.generate_files({.platform = "pc_opengl"}).contains("j2x_host_io.cpp"));
    // a console that is not the 360 says so in every stub's banner and swaps in its own clock
    const auto ps3 = r.generate_files({.platform = "ps3"});
    CB_CHECK(rt_has(ps3.at("Keyboard.h"), "platform layer"));
    CB_CHECK(!rt_has(ps3.at("Keyboard.h"), "the Xbox 360 platform layer"));
    CB_CHECK(rt_has(x360.at("Keyboard.h"), "the Xbox 360 platform layer"));
    CB_CHECK(rt_has(ps3.at("JavaRT.h"), "sys_time_get_system_time"));
    // a standard rewrites the files: every file is generated again from the same runtime
    const auto modern = r.generate_files({.cpp_std = emit::header::CppStd::of_year(2017), .platform = "xbox360"});
    CB_CHECK_EQ(modern.size(), x360.size());
    // generate_files does not change what the runtime holds
    CB_CHECK(!rt_has(r.files().at("JavaRT.h"), "return (long long)time(0) * 1000LL;"));
});

}  // namespace
