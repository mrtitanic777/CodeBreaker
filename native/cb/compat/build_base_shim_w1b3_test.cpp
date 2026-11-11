// The shims at codebreaker/base/converted.hpp and codebreaker/build/flat_thunk.hpp answer what the old layer's callers expect,
// through the new code (cb/base/converted, cb/build/flat_thunk). The parts of the flat_thunk shim that need the old corpus and
// the old prototypes table (recover_flat_thunk, the builds, main) are compiled, not run: see shim_check_w1b3.cpp in cb_shim_check.
#include "codebreaker/base/converted.hpp"
#include "codebreaker/build/flat_thunk.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

#include <cstdlib>

namespace stdfs = std::filesystem;
namespace old_conv = codebreaker::base::converted;
namespace old_ft = codebreaker::build::flat_thunk;

namespace {

// The old layer reads settings through the C runtime's environment, which only _putenv_s changes (see base_shim_test.cpp).
struct CrtEnv {
    std::string name;
    std::string before;
    explicit CrtEnv(std::string n, const std::string& value) : name(std::move(n)) {
        char* old = nullptr;
        size_t len = 0;
        if (_dupenv_s(&old, &len, name.c_str()) == 0 && old) {
            before = old;
            std::free(old);
        }
        _putenv_s(name.c_str(), value.c_str());
    }
    ~CrtEnv() { _putenv_s(name.c_str(), before.c_str()); }
};

struct ReloadOnExit {
    ~ReloadOnExit() { py::settings::reload(); }
};

struct Settings {
    ReloadOnExit reload;
    cb::fs::ScopedTempDir dir{"cbshimc"};
    CrtEnv root{"CB_CHECKOUT_ROOT", cb::fs::to_text(dir.path())};
    Settings() { py::settings::reload(); }
};

}  // namespace

CB_TEST("compat.build_base_shim.converted_root_is_the_setting_or_the_checkouts_Converted_folder", [] {
    Settings s;
    CB_CHECK(cb::fs::same_path(cb::compat::legacy::path_of(old_conv::root()), s.dir.path() / "Converted"));
    CB_CHECK(stdfs::is_directory(s.dir.path() / "Converted"));
    {
        const CrtEnv other("CB_CONVERTED", cb::fs::to_text(s.dir.path() / "Elsewhere"));
        py::settings::reload();
        CB_CHECK(cb::fs::same_path(cb::compat::legacy::path_of(old_conv::root()), s.dir.path() / "Elsewhere"));
    }
    py::settings::reload();
});

CB_TEST("compat.build_base_shim.a_project_has_its_folders_as_text_and_records_steps", [] {
    Settings s;
    const stdfs::path binary = s.dir.path() / "hello.bin";
    cb::fs::write_text(binary, "hello world");
    const py::str path = cb::compat::legacy::native(binary);
    old_conv::Project p = old_conv::project(path);
    CB_CHECK_EQ(p.name().utf8(), "hello");
    CB_CHECK(p.src.utf8().ends_with("\\Converted\\hello\\src"));
    CB_CHECK(p.manifest.utf8().ends_with("\\Converted\\hello\\project.json"));
    CB_CHECK(stdfs::is_directory(cb::compat::legacy::path_of(p.reports)));
    p.record(py::str("decompile"), py::list<py::str>{py::str(p.src.utf8() + "\\a.c")}, py::str("a note"));
    const old_conv::ProjectData d = p.data();
    CB_CHECK_EQ(d.name->utf8(), "hello");
    CB_CHECK_EQ(d.sha1->utf8(), "2aae6c35c94fcfb415dbe95f408b9ce91ee846ed");
    CB_CHECK_EQ(*d.size, int64_t{11});
    CB_CHECK_EQ(d.steps->size(), int64_t{1});
    CB_CHECK_EQ((*d.steps)[0].command->utf8(), "decompile");
    CB_CHECK_EQ((*d.steps)[0].outputs->size(), int64_t{1});
    CB_CHECK_EQ((*(*d.steps)[0].outputs)[0].utf8(), "src\\a.c");
    CB_CHECK_EQ((*d.steps)[0].notes->utf8(), "a note");
    CB_CHECK(p.original_file().has_value());
    CB_CHECK(p.original_file()->utf8().ends_with("\\original\\hello.bin"));
    // found again, by the bytes
    const old_conv::Project again = old_conv::project(path);
    CB_CHECK_EQ(again.root.utf8(), p.root.utf8());
    // a named project in another base folder
    const old_conv::Project named = old_conv::project(path, py::str("My Name!"), cb::compat::legacy::native(s.dir.path() / "base"));
    CB_CHECK(named.root.utf8().ends_with("\\base\\My_Name"));
    // the helpers
    CB_CHECK_EQ(old_conv::_clean(py::str("a b")).utf8(), "a_b");
    CB_CHECK_EQ(old_conv::default_name(path).utf8(), "hello");
    CB_CHECK_EQ(old_conv::_kind(path).utf8(), "");
    CB_CHECK_EQ(old_conv::_sha1(path).utf8(), "2aae6c35c94fcfb415dbe95f408b9ce91ee846ed");
    CB_CHECK_EQ(old_conv::FOLDERS.size(), int64_t{5});
    // an error reaches the old code as the Python's
    bool oserror = false;
    try {
        old_conv::project(py::str((s.dir.path() / "missing.bin").string()));
    } catch (const py::OSError&) {
        oserror = true;
    }
    CB_CHECK(oserror);
});

CB_TEST("compat.build_base_shim.flat_thunk_classify_and_bindiff", [] {
    const auto data = [](const char* name) { return cb::compat::legacy::native(cb::testing::data_path(std::string("formats/testdata/") + name)); };
    const old_ft::Classification c = old_ft::classify(data("lz32.dll"));
    CB_CHECK_EQ(c.format.utf8(), "PE");
    CB_CHECK_EQ(c.subtype.utf8(), "flat-thunk");
    CB_CHECK_EQ(c.recipe->utf8(), "thunk-ml-cl-link");
    CB_CHECK_EQ(*c.n_exports, int64_t{12});
    CB_CHECK_EQ(*c.n_imports, int64_t{17});
    const auto items = c.items();
    CB_CHECK_EQ(items.size(), int64_t{6});
    CB_CHECK_EQ(std::get<0>(items[0]).utf8(), "format");
    CB_CHECK_EQ(std::get<1>(items[5]).utf8(), "imports flat-thunk helpers (SMapLS,SUnMapLS)");
    const old_ft::Classification unknown = old_ft::classify(data("choice.com"));
    CB_CHECK_EQ(unknown.subtype.utf8(), "unknown");
    CB_CHECK(!unknown.recipe.has_value());
    CB_CHECK_EQ(unknown.items().size(), int64_t{4});

    const old_ft::BinDiff d = old_ft::bindiff(data("lz32.dll"), data("lz32.dll"));
    CB_CHECK(!d.error.has_value());
    CB_CHECK_EQ(d.verdict->utf8(), "byte-surface-identical");
    CB_CHECK_EQ(d.kind->utf8(), "PE");
    CB_CHECK_EQ(std::get<0>(d.entry).utf8(), "0x1000");
    CB_CHECK_EQ(std::get<0>(d.n_exports), int64_t{12});
    CB_CHECK_EQ(d.sections_built.size(), int64_t{6});
    const old_ft::BinDiff ne = old_ft::bindiff(data("lzexpand.dll"), data("lzexpand.dll"));
    CB_CHECK_EQ(ne.kind->utf8(), "NE");
    CB_CHECK_EQ(ne.seg_kinds_built.size(), int64_t{12});
    CB_CHECK_EQ(ne.seg_kinds_built[11].utf8(), "DATA");
    // the Python's answer for a pair it cannot compare: an entry named "error"
    const old_ft::BinDiff bad = old_ft::bindiff(data("lzexpand.dll"), data("lz32.dll"));
    CB_CHECK(bad.error.has_value());
    CB_CHECK_EQ(bad.error->utf8(), "format mismatch: built=NE shipped=PE");
});
