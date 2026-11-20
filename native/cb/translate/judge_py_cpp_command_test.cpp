// cb-unity: alone (test helpers named for this module)
// Tests of `judge-py-cpp`: the command's shape, and, where the machine has a C++ compiler and a `python`, every module of
// the Python corpus (tests/translate/corpus/python) judged by running CPython beside its C++17 translation.
#include "cb/translate/judge_py_cpp_command.hpp"

#include <algorithm>
#include <iostream>

#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/translate/native_l2.hpp"

namespace {

namespace jpc = cb::translate::judge_py_cpp;
namespace stdfs = std::filesystem;

jpc::Environment jpct_environment() { return {cb::config::Config::load(cb::testing::data_path("../.."))}; }

bool jpct_python_present() {
    try {
        cb::process::ProcessSpec spec;
        spec.argv = {cb::fs::get_env("CB_PYTHON").value_or("python"), "--version"};
        spec.timeout = std::chrono::seconds(30);
        return cb::process::run(spec).exit_code == 0;
    } catch (const std::exception&) {
        return false;
    }
}

}  // namespace

CB_TEST("translate.judge_py_cpp.the_command_is_named_and_documented", [] {
    const cb::cli::Command c = jpc::command(jpct_environment());
    CB_CHECK_EQ(c.name, std::string("judge-py-cpp"));
    CB_CHECK_EQ(c.area, std::string("Translate"));
    CB_CHECK(static_cast<bool>(c.handler));
});

// Each corpus module: every run_* function lowers, the module is written, compiled, and each answer equals CPython's.
CB_TEST("translate.judge_py_cpp.the_python_corpus_agrees", [] {
    const auto cxx = cb::translate::judge_cs_cpp::find_cxx("");
    if (!cxx || !jpct_python_present()) {
        std::cerr << "judge-py-cpp tests: no C++ compiler or python; skipped\n";
        return;
    }
    const stdfs::path corpus = cb::testing::data_path("../../tests/translate/corpus/python");
    std::vector<stdfs::path> files;
    for (const auto& e : stdfs::directory_iterator(corpus)) {
        if (e.path().extension() == ".py") files.push_back(e.path());
    }
    std::sort(files.begin(), files.end());
    CB_CHECK(!files.empty());
    for (const stdfs::path& f : files) {
        const std::string name = f.stem().string();
        const stdfs::path work = cb::testing::data_path("../../state/work/judge-py-cpp-tests") / name;
        const jpc::Score s = jpc::judge_module(jpct_environment(), *cxx, f, work);
        if (!s.agrees.value_or(false)) std::cerr << name << ": " << s.detail << "\n";
        CB_CHECK(s.agrees.value_or(false));
        std::error_code ec;
        stdfs::remove_all(work, ec);
    }
});
