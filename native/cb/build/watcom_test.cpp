#include "cb/build/watcom.hpp"

#include <atomic>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::build::watcom;

std::vector<ImportedName> sample_imports() {
    return {{"KERNEL32.dll", "HeapAlloc"},
            {"KERNEL32.dll", "_lread@12"},                // a decorated stdcall name
            {"INFOCOMM.dll", "_AtqReadFile@16"},
            {"USER32.dll", "?QueryStrA@STR@@QBEPADXZ"},   // a C++ name
            {"GDI32.dll", "#25"},                          // by ordinal: no C name
            {"ADVAPI32.dll", "HeapAlloc"},                 // the first DLL wins
            {"WS2_32.dll", "bad name"}};                   // no C spelling
}

}  // namespace

CB_TEST("build.watcom.tools_and_environment", [] {
    fs::ScopedTempDir dir("watcom");
    fs::make_dirs(dir.path() / "binnt64");
    fs::write_file(dir.path() / "binnt64" / "wcc386.exe", Bytes{1});
    fs::write_file(dir.path() / "binnt64" / "wlink.exe", Bytes{1});
    const OpenWatcom ow(dir.path());
    CB_CHECK(fs::same_path(ow.tool("wlink"), dir.path() / "binnt64" / "wlink.exe"));
    CB_CHECK(fs::same_path(ow.tool("wlink.exe"), dir.path() / "binnt64" / "wlink.exe"));
    CB_CHECK(fs::same_path(ow.cc(Target::Win32), dir.path() / "binnt64" / "wcc386.exe"));
    CB_CHECK(ow.cc(Target::Win16).empty());   // wcc is not installed: empty, not a bad path
    CB_CHECK(OpenWatcom(std::filesystem::path()).tool("wlink").empty());

    const auto inc32 = ow.include_dirs(Target::Win32);
    CB_CHECK_EQ(inc32.size(), 2u);
    CB_CHECK(fs::same_path(inc32[0], dir.path() / "h"));
    CB_CHECK(fs::same_path(inc32[1], dir.path() / "h" / "nt"));
    CB_CHECK(fs::same_path(ow.include_dirs(Target::Win16)[1], dir.path() / "h" / "win"));
    CB_CHECK(fs::same_path(ow.lib_dir(Target::Win16), dir.path() / "lib286" / "win"));
    CB_CHECK(fs::same_path(ow.lib_dir(Target::Win32), dir.path() / "lib386" / "nt"));

    const fs::ScopedEnv path("PATH", "C:\\somewhere");
    const auto env = ow.environment(Target::Win32);
    CB_CHECK(fs::same_path(env.at("WATCOM"), dir.path()));
    CB_CHECK_EQ(env.at("PATH"), fs::to_text(dir.path() / "binnt64") + ";C:\\somewhere");
    CB_CHECK(env.at("INCLUDE").find(';') != std::string::npos);
    CB_CHECK(fs::same_path(env.at("WLINK_LNK"), dir.path() / "binnt64" / "wlink.lnk"));
    CB_CHECK(env.contains("NoDefaultCurrentDirectoryInExePath"));
});

CB_TEST("build.watcom.root_from_settings_or_environment", [] {
    {
        const fs::ScopedEnv unset("CB_OPENWATCOM", std::nullopt);
        CB_CHECK(fs::same_path(OpenWatcom::from_settings("C:/ow/rel").root(), "C:/ow/rel"));
    }
    {
        const fs::ScopedEnv set("CB_OPENWATCOM", "D:/elsewhere");
        CB_CHECK(fs::same_path(OpenWatcom::from_settings("C:/ow/rel").root(), "D:/elsewhere"));
    }
    {
        const fs::ScopedEnv empty("CB_OPENWATCOM", "");   // an empty variable does not override
        CB_CHECK(fs::same_path(OpenWatcom::from_settings("C:/ow/rel").root(), "C:/ow/rel"));
    }
});

CB_TEST("build.watcom.scratch_lives_in_temp", [] {
    const std::string_view parts[] = {"lz32", "run1"};
    const auto p = scratch_dir(parts);
    CB_CHECK(p.string().find("bodyverify") != std::string::npos);
    CB_CHECK_EQ(p.filename().string(), "run1");
    CB_CHECK_EQ(p.parent_path().filename().string(), "lz32");
    CB_CHECK_EQ(scratch_dir().filename().string(), "bodyverify");
});

CB_TEST("build.watcom.import_maps", [] {
    int reads = 0;
    ImportMaps maps([&](const std::filesystem::path& module) -> std::vector<ImportedName> {
        ++reads;
        if (module == "broken.dll") throw FormatError("pe", 0, "not a PE");
        return sample_imports();
    });
    const auto m = maps.of("retail.dll");
    CB_CHECK_EQ(m->at("HeapAlloc"), "KERNEL32");                    // the first DLL wins
    CB_CHECK_EQ(m->at("_lread_12"), "KERNEL32");                    // the decorated name, in its C spelling
    CB_CHECK_EQ(m->at("_AtqReadFile_16"), "INFOCOMM");
    CB_CHECK_EQ(m->at("_QueryStrA_STR__QBEPADXZ"), "USER32");
    CB_CHECK_EQ(m->size(), 4u);                                     // no ordinal, no "bad name"
    CB_CHECK_EQ(maps.export_name("_lread_12"), "_lread@12");        // what the DLL exports it by
    CB_CHECK_EQ(maps.export_name("_QueryStrA_STR__QBEPADXZ"), "?QueryStrA@STR@@QBEPADXZ");
    CB_CHECK_EQ(maps.export_name("HeapAlloc"), "HeapAlloc");        // undecorated: itself
    CB_CHECK_EQ(maps.export_name("unknown"), "unknown");
    // read once per module
    (void)maps.of("retail.dll");
    CB_CHECK_EQ(reads, 1);
    // an unreadable module has no imports and does not stop the others
    CB_CHECK(maps.of("broken.dll")->empty());
    CB_CHECK_EQ(maps.of("retail.dll")->size(), 4u);
});

CB_TEST("build.watcom.import_maps_from_many_threads", [] {
    std::atomic<int> reads{0};
    ImportMaps maps([&](const std::filesystem::path&) {
        ++reads;
        return sample_imports();
    });
    std::atomic<bool> all_good{true};   // (checks are recorded on the test's own thread)
    std::vector<std::thread> threads;
    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&] {
            for (int i = 0; i < 50; ++i) {
                if (maps.of("shared.dll")->size() != 4u || maps.export_name("_lread_12") != "_lread@12") all_good = false;
            }
        });
    }
    for (auto& t : threads) t.join();
    CB_CHECK(all_good.load());
    CB_CHECK_EQ(reads.load(), 1);
});

CB_TEST("build.watcom.import_lines", [] {
    ImportMaps maps([](const std::filesystem::path&) { return sample_imports(); });
    const auto imports = maps.of("retail.dll");
    // the compiled object's external symbols: cdecl `_name`, stdcall `_name@N`
    const std::vector<std::string> externs{"_HeapAlloc@12", "__AtqReadFile_16@16", "_notimported"};
    const std::vector<std::string> wanted{"HeapAlloc", "notimported", "HeapAlloc", "missing"};   // duplicates and strangers
    // `notimported` is in the object but not imported by the retail module: no line; `missing` is not in the object
    CB_CHECK_EQ(import_lines(externs, wanted, *imports, maps), "import '_HeapAlloc@12' 'KERNEL32'.'HeapAlloc'\n");
    // the decoration adds ONE underscore: `__AtqReadFile_16@16` is `_AtqReadFile_16`, the C spelling of the
    // decorated export `_AtqReadFile@16`, which is what the DLL is asked for
    const std::vector<std::string> wanted2{"_AtqReadFile_16", "AtqReadFile_16"};
    CB_CHECK_EQ(import_lines(externs, wanted2, *imports, maps), "import '__AtqReadFile_16@16' 'INFOCOMM'.'_AtqReadFile@16'\n");
    // lines come out in name order
    const std::vector<std::string> both{"_AtqReadFile_16", "HeapAlloc"};
    CB_CHECK_EQ(import_lines(externs, both, *imports, maps),
                "import '_HeapAlloc@12' 'KERNEL32'.'HeapAlloc'\n"
                "import '__AtqReadFile_16@16' 'INFOCOMM'.'_AtqReadFile@16'\n");
});

CB_TEST("build.watcom.import_lines_by_number", [] {
    ImportMaps maps([](const std::filesystem::path&) { return sample_imports(); });
    const auto imports = maps.of("retail.dll");
    const std::vector<std::string> externs{"_GetStuff@8", "_HeapAlloc@12"};
    std::unordered_map<std::string, NumberedImport> numbered;
    numbered["GetStuff"] = {"SHELL32", 17, 8};
    const std::vector<std::string> wanted{"GetStuff", "HeapAlloc"};
    CB_CHECK_EQ(import_lines(externs, wanted, *imports, maps, &numbered),
                "import '_GetStuff@8' 'SHELL32'.17\n"
                "import '_HeapAlloc@12' 'KERNEL32'.'HeapAlloc'\n");
    // a name that is numbered and also imported by name is bound by number
    numbered["HeapAlloc"] = {"KERNEL32", 99, std::nullopt};
    CB_CHECK(import_lines(externs, wanted, *imports, maps, &numbered).find("'KERNEL32'.99\n") != std::string::npos);
    CB_CHECK_EQ(import_lines({}, wanted, *imports, maps), "");
});
