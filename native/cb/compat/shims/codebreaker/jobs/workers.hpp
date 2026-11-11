// codebreaker/jobs/workers.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/jobs/workers.hpp for the ports that still use
// the old API. No logic: forwarding. Staged under native/cb/compat/shims/ until the exe's build is wired for C++20.
//
// Old API: enable, enabled, dispatch, run, count, hex, unhex, Enc, Dec, read_file, write_file, scratch_folder.
// Two things are the old world's and stay in the old workers.cpp until their modules are refactored:
//   * dispatch(): it registers the three kinds the old program's workers do (gate, judge, build) by calling
//     verify/gate16/isolated16, verify/reconstruct and build/module -- COMPAT(py): remove when those are refactored,
//     each registers its own kind with a jobs::WorkerKinds then.
//   * count(): base/parallel's workers() reads the old settings.
// This header gives the rest, and `kinds()`/`launcher()` for the old workers.cpp to use.
#pragma once

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/jobs/workers.hpp"

namespace codebreaker::jobs::workers {

// The process's launcher and kinds (the old API had a global for each).
inline cb::jobs::WorkerLauncher& launcher() {
    static cb::jobs::WorkerLauncher l{std::filesystem::path()};
    return l;
}
inline cb::jobs::WorkerKinds& kinds() {
    static cb::jobs::WorkerKinds k;
    return k;
}

// This program's own path is `exe`: from now on workers may be started. (The old one asked the OS for the path too.)
inline void enable(const std::string&) { launcher() = cb::jobs::WorkerLauncher::for_this_program(); }
inline bool enabled() { return launcher().enabled(); }

// Is this process a worker (argv[1] == "--cb-worker")? Then it ran its share: `rc` is its exit code.
// (Defined in the old workers.cpp: it registers the old modules' kinds first.)
bool dispatch(int argc, char** argv, int& rc);

// Run the program as a worker with these arguments (after "--cb-worker"); its exit code. Blocks.
inline int64_t run(const std::vector<std::string>& args) {
    if (args.empty()) return cb::jobs::kWorkerUnknownKind;
    return launcher()
        .run(args.front(), cb::jobs::Args(args.begin() + 1, args.end()))
        .exit_code;
}

// How many at once (base/parallel workers()). (Defined in the old workers.cpp.)
int64_t count();

// ---- the files' text ----
inline std::string hex(const std::string& raw) {
    cb::jobs::TokenWriter w;
    w.text(raw);
    return w.str();
}
inline std::string unhex(const std::string& text) { return cb::jobs::TokenReader(text).text(); }

class Enc {
  public:
    Enc& i(int64_t v) {
        w_.integer(v);
        return *this;
    }
    Enc& s(const std::string& v) {
        w_.text(v);
        return *this;
    }
    const std::string& text() const { return w_.str(); }

  private:
    cb::jobs::TokenWriter w_;
};

class Dec {
  public:
    explicit Dec(const std::string& text) : r_(text) {}
    int64_t i() { return r_.integer(); }
    std::string s() { return r_.text(); }

  private:
    cb::jobs::TokenReader r_;
};

inline std::string read_file(const std::string& path) { return cb::fs::read_text(cb::fs::from_text(path)); }
inline void write_file(const std::string& path, const std::string& text) { cb::fs::write_text(cb::fs::from_text(path), text); }

// A folder for one hand-over (removed by the caller).
inline std::string scratch_folder() {
    const std::filesystem::path base = std::filesystem::temp_directory_path() / "codebreaker_workers";
    cb::fs::make_dirs(base);
    return cb::fs::ScopedTempDir("w", base).release().string();   // (released: the caller removes it, as it always did)
}

}  // namespace codebreaker::jobs::workers
