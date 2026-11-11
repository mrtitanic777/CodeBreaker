#include "cb/base/corpus.hpp"

#include <atomic>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::base;
namespace stdfs = std::filesystem;

namespace {

// A corpus on disk. The files' first bytes say what they are, so a probe that reads them can stand in for the
// formats' header detection: "MZ" + 'P' / 'N' / 'L' / 'D' is a PE, NE, LE or DOS executable.
struct Tree {
    fs::ScopedTempDir dir{"cbcorpus"};
    stdfs::path win95, win98;
    std::atomic<int> lane_probes{0};
    // clear every variable that would pick a product or a root for us
    fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    std::optional<fs::ScopedEnv> root_env;   // CB_CORPUS_ROOT: the default product's folder is found through it
    fs::ScopedEnv no_data{"WT_DATA_ROOT", std::nullopt};

    Tree() {
        win95 = dir.path() / "corpus95";
        win98 = dir.path() / "corpus98";
        fs::make_dirs(win95);
        root_env.emplace("CB_CORPUS_ROOT", fs::to_text(win95));
    }

    void put(const stdfs::path& relative_to_win95, std::string_view content) {
        fs::write_text(win95 / relative_to_win95, std::string(content));
    }

    Products products() const {
        Products p;
        p.target = "win95";
        p.target_folder = win95;
        p.listed["win98"] = win98;
        p.checkout_root = dir.path() / "CodeBreaker";
        p.state = dir.path() / "state";
        return p;
    }

    FormatProbes probes() {
        FormatProbes f;
        f.lane = [this](const stdfs::path& p) -> std::optional<Lane> {
            ++lane_probes;
            const std::string head = fs::read_text(p).substr(0, 3);
            if (head.size() < 3 || head[0] != 'M' || head[1] != 'Z') return std::nullopt;
            switch (head[2]) {
                case 'P': return Lane::Pe;
                case 'N': return Lane::Ne;
                case 'L': return Lane::Le;
                case 'D': return Lane::Dos;
            }
            return std::nullopt;
        };
        return f;
    }
};

std::vector<std::string> tails(const std::vector<stdfs::path>& paths, const stdfs::path& root) {
    std::vector<std::string> out;
    for (const stdfs::path& p : paths) out.push_back(fs::to_text(stdfs::relative(p, root)));
    return out;
}

}  // namespace

CB_TEST("base.corpus.the_copy_the_os_loads_comes_first", [] {
    Tree t;
    t.put("setup/kernel32.dll", "MZP-setup");
    t.put("drivers/a/b/kernel32.dll", "MZP-deep");
    t.put("WINDOWS/kernel32.dll", "MZP-windows");
    t.put("WINDOWS/SYSTEM/kernel32.dll", "MZP-system");
    t.put("zzz/KERNEL32.DLL", "MZP-upper");   // another copy, same name in another case
    Corpus c(t.products(), t.probes());
    const auto got = tails(c.candidates("kernel32.dll"), t.win95);
    CB_CHECK_EQ(got.size(), 5u);
    CB_CHECK_EQ(got.at(0), "WINDOWS/SYSTEM/kernel32.dll");
    CB_CHECK_EQ(got.at(1), "WINDOWS/kernel32.dll");
    // then the shorter path first: zzz/ and setup/ before drivers/a/b/; among those two, by name
    CB_CHECK_EQ(got.at(2), "setup/kernel32.dll");
    CB_CHECK_EQ(got.at(3), "zzz/KERNEL32.DLL");
    CB_CHECK_EQ(got.at(4), "drivers/a/b/kernel32.dll");
    // a name is matched in any case
    CB_CHECK_EQ(c.candidates("Kernel32.DLL").size(), 5u);
    CB_CHECK_EQ(tails({c.retail("kernel32").value()}, t.win95)[0], "WINDOWS/SYSTEM/kernel32.dll");
});

CB_TEST("base.corpus.a_bare_name_tries_the_usual_extensions", [] {
    Tree t;
    t.put("a/lzexpand.dll", "MZN");
    t.put("WINDOWS/lzexpand.exe", "MZN");
    t.put("b/shell.vxd", "MZL");
    Corpus c(t.products(), t.probes());
    const auto lz = tails(c.candidates("lzexpand"), t.win95);
    CB_CHECK_EQ(lz.size(), 2u);
    CB_CHECK_EQ(lz.at(0), "WINDOWS/lzexpand.exe");   // best first across extensions
    CB_CHECK_EQ(c.candidates("shell").size(), 1u);
    CB_CHECK(c.candidates("shell.dll").empty());   // a name with an extension is only itself
    CB_CHECK(c.candidates("nothing").empty());
    CB_CHECK(c.candidates("c:/some/dir/lzexpand.dll").size() == 1);   // only the file name counts
});

CB_TEST("base.corpus.retail_resolve_and_paths", [] {
    Tree t;
    t.put("WINDOWS/SYSTEM/user.exe", "MZN");
    fs::write_text(t.dir.path() / "mine.dll", "MZP");
    Corpus c(t.products(), t.probes());
    CB_CHECK(c.retail("user").has_value());
    CB_CHECK(!c.retail("nosuchmodule").has_value());
    CB_CHECK_THROWS_AS(NotFoundError, (void)c.resolve("nosuchmodule"));
    // an existing path is returned as it is, whatever the corpus holds
    CB_CHECK(c.retail(fs::to_text(t.dir.path() / "mine.dll")).value() == t.dir.path() / "mine.dll");
    CB_CHECK(c.resolve(fs::to_text(t.dir.path() / "mine.dll")) == t.dir.path() / "mine.dll");
    // a container is handed over as the image inside it
    FormatProbes probes = t.probes();
    probes.readable = [&](const stdfs::path& p) { return p.filename() == "user.exe" ? t.dir.path() / "inner.bin" : p; };
    Corpus unwrapping(t.products(), probes);
    CB_CHECK(unwrapping.retail("user").value() == t.dir.path() / "inner.bin");
    CB_CHECK(unwrapping.retail(fs::to_text(t.dir.path() / "mine.dll")).value() == t.dir.path() / "mine.dll");
});

CB_TEST("base.corpus.lanes_are_asked_once_per_file", [] {
    Tree t;
    t.put("a.dll", "MZP");
    t.put("b.exe", "MZN");
    t.put("c.drv", "garbage");
    Corpus c(t.products(), t.probes());
    CB_CHECK(c.lane(t.win95 / "a.dll") == Lane::Pe);
    CB_CHECK(c.lane(t.win95 / "b.exe") == Lane::Ne);
    CB_CHECK(!c.lane(t.win95 / "c.drv").has_value());
    const int asked = t.lane_probes;
    CB_CHECK_EQ(asked, 3);
    for (int i = 0; i < 3; ++i) (void)c.lane(t.win95 / "a.dll");
    (void)c.lane(t.win95 / "c.drv");   // "not an executable" is remembered too
    CB_CHECK_EQ(t.lane_probes.load(), asked);
    // no probe given: nothing is an executable
    Corpus blind(t.products());
    CB_CHECK(!blind.lane(t.win95 / "a.dll").has_value());
});

CB_TEST("base.corpus.modules_smallest_first_by_lane_and_extension", [] {
    Tree t;
    t.put("WINDOWS/SYSTEM/big.dll", "MZP" + std::string(500, 'x'));
    t.put("elsewhere/big.dll", "MZP" + std::string(50, 'x'));   // not the best copy: not listed
    t.put("small.exe", "MZN" + std::string(10, 'x'));
    t.put("mid.dll", "MZN" + std::string(100, 'x'));
    t.put("data.txt", "MZP");                                   // wrong extension
    t.put("font.fon", "MZN");                                   // wrong extension (but an executable: see executables())
    Corpus c(t.products(), t.probes());
    const auto all = tails(c.modules(), t.win95);
    CB_CHECK_EQ(all.size(), 3u);
    CB_CHECK_EQ(all.at(0), "small.exe");
    CB_CHECK_EQ(all.at(1), "mid.dll");
    CB_CHECK_EQ(all.at(2), "WINDOWS/SYSTEM/big.dll");
    CB_CHECK_EQ(tails(c.modules(Lane::Ne), t.win95).size(), 2u);
    CB_CHECK_EQ(tails(c.modules(Lane::Pe), t.win95).size(), 1u);
    CB_CHECK(c.modules(Lane::Le).empty());
    CB_CHECK_EQ(c.modules(std::nullopt, {".fon"}).size(), 1u);

    // executables: every copy, whatever the name, sorted by path
    const auto exes = c.executables();
    CB_CHECK_EQ(exes.size(), 6u);   // both copies of big, small, mid, font -- and data.txt, which starts with MZP
    CB_CHECK(std::is_sorted(exes.begin(), exes.end(), [](const auto& a, const auto& b) { return fs::to_text(a.path) < fs::to_text(b.path); }));
});

CB_TEST("base.corpus.sha_is_sha256_and_remembered", [] {
    Tree t;
    t.put("abc.dll", "abc");
    Corpus c(t.products(), t.probes());
    CB_CHECK_EQ(c.sha(t.win95 / "abc.dll").value(), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    CB_CHECK(!c.sha(t.win95 / "missing.dll").has_value());
    t.put("empty.dll", "");
    CB_CHECK_EQ(c.sha(t.win95 / "empty.dll").value(), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    // changed content (a different size) is a different file for the memo
    t.put("abc.dll", "abcd");
    CB_CHECK_EQ(c.sha(t.win95 / "abc.dll").value(), "88d4266fd4e6338d13b845fcf289579d209c897823b9217da3e161936f031589");
});

CB_TEST("base.corpus.products_and_selection", [] {
    Tree t;
    fs::make_dirs(t.win98);
    fs::write_text(t.win98 / "only98.dll", "MZP");
    t.put("only95.dll", "MZP");
    Corpus c(t.products(), t.probes());
    CB_CHECK(c.names() == (std::vector<std::string>{"win95", "win98"}));
    CB_CHECK_EQ(c.current(), "win95");
    CB_CHECK(c.root("win98") == t.win98);
    CB_CHECK(c.root("win95") != t.win98);
    CB_CHECK_EQ(c.candidates("only95.dll").size(), 1u);
    CB_CHECK(c.candidates("only98.dll").empty());

    CB_CHECK_THROWS_AS(UsageError, c.use("os2"));
    CB_CHECK_THROWS_AS(UsageError, (void)c.root("os2"));
    CB_CHECK_EQ(c.current(), "win95");   // a refused name changed nothing

    CB_CHECK_EQ(c.use("win98"), "win95");   // returns the one before
    CB_CHECK_EQ(c.current(), "win98");
    CB_CHECK(c.root() == t.win98);
    CB_CHECK_EQ(c.candidates("only98.dll").size(), 1u);
    CB_CHECK(c.candidates("only95.dll").empty());
    CB_CHECK_EQ(fs::get_env("CB_CORPUS_NAME").value_or(""), "win98");   // the workers it starts see it
    CB_CHECK_EQ(c.use(std::nullopt), "win98");
    CB_CHECK_EQ(c.current(), "win95");
    CB_CHECK(!fs::get_env("CB_CORPUS_NAME").has_value());
});

CB_TEST("base.corpus.selected_is_for_the_length_of_a_scope", [] {
    Tree t;
    fs::make_dirs(t.win98);
    Corpus c(t.products(), t.probes());
    {
        Corpus::Selected scope(c, "win98");
        CB_CHECK_EQ(c.current(), "win98");
        CB_CHECK_EQ(fs::get_env("CB_CORPUS_NAME").value_or(""), "win98");
    }
    CB_CHECK_EQ(c.current(), "win95");
    CB_CHECK(!fs::get_env("CB_CORPUS_NAME").has_value());
    // a selection made by the environment before the scope stands again after it
    fs::ScopedEnv outside("CB_CORPUS_NAME", "win98");
    {
        Corpus::Selected scope(c, "win95");
        CB_CHECK_EQ(c.current(), "win95");
    }
    CB_CHECK_EQ(fs::get_env("CB_CORPUS_NAME").value_or(""), "win98");
    CB_CHECK_EQ(c.current(), "win98");
    CB_CHECK_THROWS_AS(UsageError, Corpus::Selected(c, "nonsense"));
});

CB_TEST("base.corpus.the_corpus_option_is_taken_out_of_a_command_line", [] {
    Tree t;
    fs::make_dirs(t.win98);
    Corpus c(t.products(), t.probes());
    const auto rest = c.take_option({"decompile", "--corpus", "win98", "lz32.dll", "--judge"});
    CB_CHECK(rest == (std::vector<std::string>{"decompile", "lz32.dll", "--judge"}));
    CB_CHECK_EQ(c.current(), "win98");
    CB_CHECK((c.take_option({"a", "b"}) == std::vector<std::string>{"a", "b"}));
    CB_CHECK_THROWS_AS(UsageError, c.take_option({"x", "--corpus"}));
    CB_CHECK_THROWS_AS(UsageError, c.take_option({"--corpus", "nope"}));
    (void)c.use(std::nullopt);
});

CB_TEST("base.corpus.root_of_the_default_product", [] {
    Tree t;
    Products p = t.products();
    Corpus c(p, t.probes());
    t.root_env.reset();   // (the other tests find their folder through it)
    // nothing exists: the sibling Windows95 repo's corpus is reported
    const stdfs::path sibling = t.dir.path() / "Windows95" / "design" / "corpus" / "files";
    CB_CHECK(c.root() == sibling);
    {
        fs::ScopedTempDir data("cbdata");
        fs::ScopedEnv env("WT_DATA_ROOT", fs::to_text(data.path()));
        CB_CHECK(c.root() == data.path());
        fs::make_dirs(sibling);   // the sibling repo wins over WT_DATA_ROOT
        CB_CHECK(c.root() == sibling);
        fs::ScopedTempDir mine("cbmine");
        fs::ScopedEnv explicit_root("CB_CORPUS_ROOT", fs::to_text(mine.path()));   // and CB_CORPUS_ROOT over both
        CB_CHECK(c.root() == mine.path());
        fs::ScopedEnv not_a_dir("CB_CORPUS_ROOT", fs::to_text(mine.path() / "nope"));   // a name that is no folder is ignored
        CB_CHECK(c.root() == sibling);
    }
    CB_CHECK(!c.names().empty());
});

CB_TEST("base.corpus.design_dir_reference_and_owner", [] {
    Tree t;
    fs::make_dirs(t.win98);
    t.root_env.reset();
    Products p = t.products();
    p.listed["win95"] = t.dir.path() / "Windows95" / "design" / "corpus" / "files";
    Corpus c(p, t.probes());
    CB_CHECK(c.design_dir() == t.dir.path() / "Windows95" / "design");
    CB_CHECK(c.reference({"Microsoft Windows 95 DDK", "INC32"}) ==
             t.dir.path() / "Windows95" / "design" / "reference" / "Microsoft Windows 95 DDK" / "INC32");
    p.reference = t.dir.path() / "refs";
    Corpus with_reference(p, t.probes());
    CB_CHECK(with_reference.reference({"x"}) == t.dir.path() / "refs" / "x");
    CB_CHECK(with_reference.reference().filename() == "refs");
    {
        Corpus::Selected scope(c, "win98");   // another product keeps what it derives in the state folder
        CB_CHECK(c.design_dir() == t.dir.path() / "state" / "products" / "win98" / "design");
    }
    CB_CHECK(c.owner(t.win98 / "sub" / "x.dll") == "win98");
    CB_CHECK(c.owner(t.dir.path() / "Windows95" / "design" / "corpus" / "files" / "a.dll") == "win95");
    CB_CHECK(!c.owner(t.dir.path() / "elsewhere" / "a.dll").has_value());
    CB_CHECK(!c.owner(t.win98).has_value());   // the folder itself is not inside itself
    // the deepest product folder wins
    Products nested = p;
    nested.listed["inner"] = t.win98 / "inner";
    Corpus n(nested, t.probes());
    CB_CHECK(n.owner(t.win98 / "inner" / "a.dll") == "inner");
    CB_CHECK(n.owner(t.win98 / "other" / "a.dll") == "win98");
});

CB_TEST("base.corpus.boot_images_are_unpacked_once_into_the_state_folder", [] {
    Tree t;
    t.put("DISK/WIN.BIN", "MZNimage");
    t.put("DISK/WIN.OVL", "overlay");
    t.put("DISK/MSDOS.EXE", "x");   // a 1-byte stub of the same name as a module is not an executable
    FormatProbes probes = t.probes();
    int reads = 0;
    probes.boot_image_modules = [&](const stdfs::path&, const stdfs::path&) {
        ++reads;
        return std::vector<BootModule>{{"KERNEL", Bytes{'M', 'Z', 'N', 1}}, {"MSDOS", Bytes{'M', 'Z', 'N', 2}}};
    };
    Corpus c(t.products(), probes);
    const auto kernel = c.candidates("kernel.exe");
    CB_CHECK_EQ(kernel.size(), 1u);
    CB_CHECK(kernel.at(0).parent_path().parent_path() == t.dir.path() / "state" / "unpacked");
    CB_CHECK(kernel.at(0).parent_path().filename().string().starts_with("win-"));
    CB_CHECK(stdfs::exists(kernel.at(0)));
    CB_CHECK_EQ(fs::read_file(kernel.at(0)).size(), 4u);
    const auto dos = c.candidates("msdos.exe");   // the unpacked module goes first: the stub on the disk is no executable
    CB_CHECK_EQ(dos.size(), 2u);
    CB_CHECK(dos.at(0).parent_path().parent_path() == t.dir.path() / "state" / "unpacked");
    CB_CHECK_EQ(reads, 1);
    // a second corpus finds the files written and does not rewrite them
    const auto written = stdfs::last_write_time(kernel.at(0));
    Corpus again(t.products(), probes);
    CB_CHECK_EQ(again.candidates("kernel.exe").size(), 1u);
    CB_CHECK(stdfs::last_write_time(kernel.at(0)) == written);
});

CB_TEST("base.corpus.a_bin_that_is_not_a_boot_image_is_left_alone", [] {
    Tree t;
    t.put("A.BIN", "MZNx");
    t.put("A.OVL", "y");
    t.put("B.BIN", "MZNx");
    t.put("B.OVL", "y");
    FormatProbes probes = t.probes();
    probes.boot_image_modules = [&](const stdfs::path& image, const stdfs::path&) -> std::vector<BootModule> {
        if (image.filename() == "A.BIN") throw FormatError("boot image", 0, "not a boot image");
        return {{"GDI", Bytes{'M', 'Z', 'N'}}};
    };
    Corpus c(t.products(), probes);
    CB_CHECK(c.candidates("gdi.exe").size() == 1u);
    CB_CHECK_EQ(c.candidates("a.bin").size(), 1u);
});

CB_TEST("base.corpus.index_is_cached_until_refreshed", [] {
    Tree t;
    t.put("a.dll", "MZP");
    Corpus c(t.products(), t.probes());
    CB_CHECK_EQ(c.index()->size(), 1u);
    t.put("b.dll", "MZP");
    CB_CHECK_EQ(c.index()->size(), 1u);          // walked once
    CB_CHECK_EQ(c.index(true)->size(), 2u);      // until asked again
    CB_CHECK_EQ(c.index()->size(), 2u);
    // a folder that does not exist is an empty corpus, not an error
    t.root_env.emplace("CB_CORPUS_ROOT", fs::to_text(t.dir.path() / "no such folder"));
    Products p = t.products();
    p.target_folder = t.dir.path() / "no such folder";
    p.listed["win95"] = t.dir.path() / "no such folder";
    Corpus empty(p, t.probes());
    CB_CHECK(empty.index()->empty());
    CB_CHECK(empty.modules().empty());
});
