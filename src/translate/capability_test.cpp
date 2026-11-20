// cb-unity: alone (test helpers named for this module)
// Tests of the translator's capability API (capability.hpp): the tables are consistent (every path has a front end, a back end and a runner), the target
// lists say what is supported and why not, and, where the machine has the tools, whole files go through translate_file.hpp and are proved by running both.
#include "translate/capability.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "translate/translate_file.hpp"

namespace {

namespace cap = cb::translate::capability;
namespace tf = cb::translate::translate_file;
namespace stdfs = std::filesystem;

const cap::Target* capt_find(const std::vector<cap::Target>& list, const std::string& id) {
    for (const cap::Target& t : list)
        if (t.id == id) return &t;
    return nullptr;
}

stdfs::path capt_work(const std::string& name) {
    const stdfs::path p = cb::testing::data_path("../state/work/translate-file-tests") / name;
    std::error_code ec;
    stdfs::remove_all(p, ec);
    cb::fs::make_dirs(p);
    return p;
}

tf::Result capt_run(const std::string& from, const std::string& to, const stdfs::path& input, const stdfs::path& out, const stdfs::path& work,
                    cb::translate::judge_cs_cpp::Observer observer = {}) {
    cb::translate::judge_cs_cpp::Environment environment{cb::config::Config::load(cb::testing::data_path("..")), observer};
    return tf::run(*cap::find_path(from, to), input, out, work, environment);
}

}  // namespace

CB_TEST("translate.capability.every_path_joins_a_front_end_to_a_back_end_and_has_a_runner", [] {
    CB_CHECK(!cap::paths().empty());
    for (const cap::Path& p : cap::paths()) {
        const cap::Language* from = cap::find_language(p.from);
        const cap::Language* to = cap::find_language(p.to);
        CB_CHECK(from != nullptr && !from->extensions.empty());   // a front end reads it
        CB_CHECK(to != nullptr && to->back_end);                  // a back end writes it
        CB_CHECK(tf::can_run(p));
        CB_CHECK(!p.judge.empty() && !p.proof.empty() && !p.needs.empty());
    }
});

CB_TEST("translate.capability.a_file_is_known_by_its_extension", [] {
    CB_CHECK(cap::language_of_file("Bank.cs") != nullptr && cap::language_of_file("Bank.cs")->id == "csharp");
    CB_CHECK(cap::language_of_file("TOOL.PY")->id == "python");
    CB_CHECK(cap::language_of_file("Calc.java")->id == "java");
    CB_CHECK(cap::language_of_file("lz32.c")->id == "c");
    CB_CHECK(cap::language_of_file("lz32.dll") == nullptr);
    CB_CHECK(cap::language_of_file("noextension") == nullptr);
    const std::vector<std::string> ext = cap::source_extensions();
    CB_CHECK(std::is_sorted(ext.begin(), ext.end()));
    CB_CHECK(std::find(ext.begin(), ext.end(), ".cs") != ext.end());
});

CB_TEST("translate.capability.csharp_source_can_become_cpp17_and_csharp_and_the_rest_say_why_not", [] {
    const std::vector<cap::Target> list = cap::targets_for_file("Bank.cs");
    // Original is not a translation: the source is source already
    const cap::Target* original = capt_find(list, "original");
    CB_CHECK(original != nullptr && !original->supported);
    CB_CHECK(!original->reason.empty());
    // Python has no back end at all
    const cap::Target* python = capt_find(list, "python");
    CB_CHECK(python != nullptr && !python->supported);
    CB_CHECK_EQ(python->reason, "Python output is not supported yet");
    // Java has a back end but no path from C# is wired
    const cap::Target* java = capt_find(list, "java");
    CB_CHECK(java != nullptr && !java->supported);
    CB_CHECK_EQ(java->reason, "C# to Java is not supported yet");
    // C++17 and C# have a path: supported when the tools are there, otherwise the reason names the missing tool
    for (const char* id : {"cpp17", "csharp"}) {
        const cap::Target* t = capt_find(list, id);
        CB_CHECK(t != nullptr);
        const std::string missing = cap::missing_tool(*cap::find_path("csharp", id));
        CB_CHECK_EQ(t->supported, missing.empty());
        if (t->supported) CB_CHECK(!t->proof.empty());
        else CB_CHECK(t->reason.starts_with("needs ") && t->reason.find(missing) != std::string::npos);
    }
    // every target of the languages table is listed once, Original first
    CB_CHECK_EQ(list.size(), cap::languages().size() + 1);
    CB_CHECK(list.front().id == "original");
});

CB_TEST("translate.capability.python_source_goes_to_cpp17_only", [] {
    const std::vector<cap::Target> list = cap::targets_for_file("tool.py");
    const std::string missing = cap::missing_tool(*cap::find_path("python", "cpp17"));
    CB_CHECK_EQ(capt_find(list, "cpp17")->supported, missing.empty());
    CB_CHECK_EQ(capt_find(list, "csharp")->reason, "Python to C# is not supported yet");
    CB_CHECK_EQ(capt_find(list, "python")->reason, "Python output is not supported yet");   // (no back end: said before any pair)
});

CB_TEST("translate.capability.with_no_input_every_language_a_path_ends_in_is_offered", [] {
    const std::vector<cap::Target> list = cap::targets();
    CB_CHECK(capt_find(list, "original")->supported);
    CB_CHECK(capt_find(list, "cpp17")->supported);
    CB_CHECK(capt_find(list, "csharp")->supported);
    CB_CHECK(!capt_find(list, "java")->supported);
    CB_CHECK_EQ(capt_find(list, "python")->reason, "Python output is not supported yet");
    // a file with no front end gets this list too (a program: its own route decides)
    CB_CHECK(cap::targets_for_file("setup.exe").size() == list.size());
});

CB_TEST("translate.capability.a_decompiled_program_keeps_its_original_and_nothing_translates_c_yet", [] {
    const std::vector<cap::Target> c = cap::targets_for_decompiled("c");
    CB_CHECK(capt_find(c, "original")->supported);
    CB_CHECK_EQ(capt_find(c, "cpp17")->reason, "C to C++17 is not supported yet");
    CB_CHECK_EQ(capt_find(c, "java")->reason, "C to Java is not supported yet");
    CB_CHECK(cap::supported_names(c).empty());
    const std::vector<cap::Target> assembly = cap::targets_for_decompiled("");   // a console program comes back as assembly
    CB_CHECK(capt_find(assembly, "original")->supported);
    CB_CHECK(!capt_find(assembly, "cpp17")->supported);
    CB_CHECK(capt_find(assembly, "cpp17")->reason.find("assembly") != std::string::npos);
});

CB_TEST("translate.capability.a_target_is_found_by_id_name_or_the_short_forms", [] {
    const std::vector<cap::Target> list = cap::targets();
    CB_CHECK_EQ(cap::find_target(list, "cpp17")->id, "cpp17");
    CB_CHECK_EQ(cap::find_target(list, "C++17")->id, "cpp17");
    CB_CHECK_EQ(cap::find_target(list, "c++17")->id, "cpp17");
    CB_CHECK_EQ(cap::find_target(list, "C#")->id, "csharp");
    CB_CHECK_EQ(cap::find_target(list, "cs")->id, "csharp");
    CB_CHECK_EQ(cap::find_target(list, "Original")->id, "original");
    CB_CHECK_EQ(cap::find_target(list, "py")->id, "python");
    CB_CHECK(cap::find_target(list, "cobol") == nullptr);
    CB_CHECK(cap::find_target(list, "") == nullptr);
    const std::vector<cap::Target> from_cs = cap::targets_for_file("Bank.cs");
    const std::string names = cap::supported_names(from_cs);
    CB_CHECK(names.find("Original") == std::string::npos);
    CB_CHECK(names.empty() || names.find("C++17") != std::string::npos);
});

// ---- whole files, where the machine has the tools --------------------------------------------------------------------------------------

CB_TEST("translate.translate_file.csharp_to_cpp17_is_written_and_proved_by_running_both", [] {
    if (!cap::missing_tool(*cap::find_path("csharp", "cpp17")).empty()) return;   // (no .NET SDK or C++ compiler here)
    const stdfs::path work = capt_work("cs-cpp17");
    std::vector<std::string> translating, checking;
    cb::translate::judge_cs_cpp::Observer observer;
    observer.translating = [&](std::uint64_t done, std::uint64_t total, std::string_view) { translating.push_back(std::to_string(done) + "/" + std::to_string(total)); };
    observer.checking = [&](std::uint64_t done, std::uint64_t total, std::string_view) { checking.push_back(std::to_string(done) + "/" + std::to_string(total)); };
    const tf::Result r = capt_run("csharp", "cpp17", cb::testing::data_path("../tests/translate/corpus/csharp/Sieve.cs"), work / "out", work / "scratch", observer);
    CB_CHECK(r.verdict == tf::Verdict::Agrees);
    CB_CHECK(!r.checked.empty());
    CB_CHECK(stdfs::exists(work / "out" / "Sieve.cpp"));
    CB_CHECK(stdfs::exists(work / "out" / "runtime" / "csrt.hpp"));
    CB_CHECK(std::find(r.files.begin(), r.files.end(), stdfs::path("Sieve.cpp")) != r.files.end());
    // real counts: the front end said how many functions there are, then they were written; the check went through its four steps
    CB_CHECK(translating.size() >= 3 && translating.front() == "0/0");
    CB_CHECK(translating.back() != "0/0" && translating.back().substr(0, translating.back().find('/')) == translating.back().substr(translating.back().find('/') + 1));
    const std::vector<std::string> steps = {"0/4", "1/4", "2/4", "3/4", "4/4"};
    CB_CHECK(checking == steps);
});

CB_TEST("translate.translate_file.csharp_to_csharp_is_proved_by_running_both", [] {
    if (!cap::missing_tool(*cap::find_path("csharp", "csharp")).empty()) return;
    const stdfs::path work = capt_work("cs-cs");
    const tf::Result r = capt_run("csharp", "csharp", cb::testing::data_path("../tests/translate/corpus/csharp/Sieve.cs"), work / "out", work / "scratch");
    CB_CHECK(r.verdict == tf::Verdict::Agrees);
    CB_CHECK(stdfs::exists(work / "out" / "Sieve.cs"));
});

CB_TEST("translate.translate_file.python_to_cpp17_is_proved_by_running_both", [] {
    if (!cap::missing_tool(*cap::find_path("python", "cpp17")).empty()) return;   // (no python, C++ compiler or L2 library here)
    const stdfs::path work = capt_work("py-cpp17");
    const tf::Result r = capt_run("python", "cpp17", cb::testing::data_path("../tests/translate/corpus/python/Generators.py"), work / "out", work / "scratch");
    CB_CHECK(r.verdict == tf::Verdict::Agrees);
    CB_CHECK(stdfs::exists(work / "out" / "Generators.cpp"));
    CB_CHECK(stdfs::exists(work / "out" / "Generators.hpp"));
    CB_CHECK(stdfs::exists(work / "out" / "runtime" / "py.hpp"));
});

CB_TEST("translate.translate_file.a_python_module_with_nothing_to_run_is_refused_and_says_what_the_check_needs", [] {
    if (!cap::missing_tool(*cap::find_path("python", "cpp17")).empty()) return;
    const stdfs::path work = capt_work("py-none");
    cb::fs::write_text(work / "plain.py", "def double(x):\n    return x * 2\n");
    const tf::Result r = capt_run("python", "cpp17", work / "plain.py", work / "out", work / "scratch");
    CB_CHECK(r.verdict == tf::Verdict::Refused);
    CB_CHECK(r.detail.find("run_*") != std::string::npos);
    CB_CHECK(r.files.empty());
});

CB_TEST("translate.translate_file.a_csharp_file_that_does_not_compile_is_bad_input_and_leaves_nothing", [] {
    if (!cap::missing_tool(*cap::find_path("csharp", "cpp17")).empty()) return;
    const stdfs::path work = capt_work("cs-bad");
    cb::fs::write_text(work / "Broken.cs", "class Broken { static void Main() { int x = \"text\"; } }\n");
    const tf::Result r = capt_run("csharp", "cpp17", work / "Broken.cs", work / "out", work / "scratch");
    CB_CHECK(r.verdict == tf::Verdict::BadInput);
    CB_CHECK(r.files.empty());
    CB_CHECK(!stdfs::exists(work / "out" / "Broken.cpp"));
});
