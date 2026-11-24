// translate/backends/c_java.cpp -- the entry points of the C back end for Java: the program's C files, and building them (see c_java.hpp).

#include "translate/backends/c_java.hpp"

#include <algorithm>
#include <atomic>
#include <cctype>
#include <format>
#include <mutex>

#include "core/fs.hpp"
#include "core/parallel.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "java/paths.hpp"
#include "translate/backends/c_java_internal.hpp"

namespace cb::translate::backends::c_java {

namespace stdfs = std::filesystem;

stdfs::path default_runtime_dir() {
    // the Java lane's folder is codebreaker/java; the C runtime is beside it, under translate
    return java::JavaData::locate().root().parent_path() / "translate" / "runtime" / "c";
}

std::vector<File> files(Arena& arena, std::span<ir::Node* const> units, const std::optional<std::string>& main) {
    detail::Program prog(arena, units, main);
    return prog.write();
}

std::string program(Arena& arena, std::span<ir::Node* const> units, const std::optional<std::string>& main) {
    std::string text;
    for (const File& f : files(arena, units, main)) {
        text += "// ==== " + f.path + " ====\n" + f.text;
        if (!f.text.ends_with('\n')) text += '\n';
    }
    return text;
}

namespace {

// The first line of a compiler's log that says what went wrong.
std::string first_error(const std::string& log) {
    for (const std::string_view line : text::lines(log)) {
        if (line.find("error") != std::string_view::npos) return std::string(line).substr(0, 400);
    }
    return log.substr(0, 300);
}

}  // namespace

Built build(Arena& arena, std::span<ir::Node* const> units, const stdfs::path& out, const BuildOptions& options) {
    std::vector<File> written;
    try {
        written = files(arena, units);
    } catch (const Decline& declined) {   // a program the back end does not write is an answer, not a failure
        return {std::nullopt, std::string("Decline: ") + declined.what(), {}, {}};
    }
    const stdfs::path runtime = options.runtime_dir.empty() ? default_runtime_dir() : options.runtime_dir;
    const stdfs::path src = out / "src";
    const stdfs::path obj = out / "obj";
    const stdfs::path exe = out / "program.exe";
    Built built;
    std::vector<std::string> sources;   // the .c files, relative to src
    for (const File& f : written) {
        fs::write_text(src / fs::from_text(f.path), f.text);
        built.files.push_back(fs::from_text(f.path));
        if (f.path.ends_with(".c")) sources.push_back(f.path);
    }
    built.sources = src;
    const stdfs::path clang = fs::from_text(std::string(kClang));
    const std::string compiler = options.compiler ? fs::to_text(*options.compiler) : (stdfs::is_regular_file(clang) ? std::string(kClang) : "gcc");
    const auto command = [&](const std::string& source, const std::string& object) {
        process::ProcessSpec spec;
        spec.argv.push_back(compiler);
        for (const std::string_view flag : kFlags) spec.argv.emplace_back(flag);
        for (const std::string& arg : {std::string("-I"), fs::to_text(runtime), std::string("-I"), fs::to_text(src), std::string("-c"), source,
                                       std::string("-o"), object})
            spec.argv.push_back(arg);
        spec.err = process::Capture::Merge;
        spec.timeout = options.timeout;
        return spec;
    };

    // each source is compiled on its own, side by side: the program's files and then the runtime's
    std::vector<std::pair<std::string, std::string>> jobs;   // source, object
    for (const std::string& s : sources) jobs.emplace_back(fs::to_text(src / fs::from_text(s)), fs::to_text(obj / fs::from_text(s + ".o")));
    jobs.emplace_back(fs::to_text(runtime / "jrt.c"), fs::to_text(obj / "runtime" / "jrt.c.o"));
    for (const auto& job : jobs) fs::make_dirs(fs::from_text(job.second).parent_path());
    std::mutex lock;
    std::string failure, failed;   // the first error, and the source it came from (the first by name, so a failure reads the same each time)
    std::atomic<bool> timed_out = false;
    const std::size_t threads = std::min<std::size_t>({jobs.size(), static_cast<std::size_t>(options.jobs > 0 ? options.jobs : std::min(parallel::workers(), 8))});
    parallel::ThreadPool pool(std::max<std::size_t>(threads, 1));
    parallel::parallel_for(pool, jobs.size(), [&](std::size_t i) {
        if (timed_out.load()) return;
        process::ProcessResult ran;
        try {
            ran = process::run(command(jobs[i].first, jobs[i].second));
        } catch (const ToolError& e) {   // no compiler to run
            const std::lock_guard guard(lock);
            if (failure.empty()) failure = std::string("does not compile: ") + e.what();
            timed_out = true;
            return;
        }
        if (ran.timed_out) {
            timed_out = true;
            const std::lock_guard guard(lock);
            if (failure.empty()) failure = "does not compile: the C compiler did not finish";
        } else if (ran.exit_code != 0) {
            const std::lock_guard guard(lock);
            const std::string line = first_error(text::sanitize_utf8(as_text(ran.out)));
            if (failure.empty() || jobs[i].first < failed) {
                failure = "does not compile: " + line;
                failed = jobs[i].first;
            }
        }
    });
    if (!failure.empty()) {
        built.why = failure;
        return built;
    }

    // the link: a response file holds the objects (a program of hundreds of classes has more names than a command line holds)
    std::string objects;
    for (const auto& job : jobs) objects += "\"" + job.second + "\"\n";
    const stdfs::path response = out / "objects.rsp";
    fs::write_text(response, objects);
    process::ProcessSpec link;
    link.argv.push_back(compiler);
    for (const std::string& arg : {"@" + fs::to_text(response), std::string("-o"), fs::to_text(exe)}) link.argv.push_back(arg);
    link.err = process::Capture::Merge;
    link.timeout = options.timeout;
    process::ProcessResult linked;
    try {
        linked = process::run(link);
    } catch (const ToolError& e) {
        built.why = std::string("does not compile: ") + e.what();
        return built;
    }
    if (linked.timed_out || linked.exit_code != 0) {
        built.why = "does not compile: " + (linked.timed_out ? std::string("the linker did not finish") : first_error(text::sanitize_utf8(as_text(linked.out))));
        return built;
    }
    built.exe = exe;
    return built;
}

std::vector<stdfs::path> install(const Built& built, const std::string& name, const stdfs::path& out, const stdfs::path& runtime_dir) {
    std::vector<stdfs::path> written;
    const auto copy = [&](const stdfs::path& from, const stdfs::path& to) {
        fs::make_dirs((out / to).parent_path());
        fs::write_file(out / to, fs::read_file(from));
        written.push_back(to);
    };
    std::vector<std::string> sources;
    for (const stdfs::path& file : built.files) {
        copy(built.sources / file, file);
        if (file.extension() == ".c") sources.push_back(fs::to_text(file));
    }
    const stdfs::path runtime = runtime_dir.empty() ? default_runtime_dir() : runtime_dir;
    for (const char* file : {"jrt.h", "jrt.c"}) copy(runtime / file, stdfs::path("runtime") / file);

    const auto note = [&](const stdfs::path& to, const std::string& text) {
        fs::write_text(out / to, text);
        written.push_back(to);
    };
    note("BUILD.txt",
         "The C translation of " + name + ".java.\n\n"
         "Each Java class has its own header and source file (in the folder of its package), as the Java program has its own files; jprogram.h\n"
         "holds what they share, jmain.c the entry point, and the runtime they need is in runtime/. It builds with a C11 compiler.\n\n"
         "With CMake (compiles the files side by side):\n\n"
         "  cmake -S . -B build\n"
         "  cmake --build build -j\n\n"
         "With gcc or clang in one command (compiles the files one after the other):\n\n"
         "  gcc -std=c11 -fwrapv -O1 -w -I runtime -I . $(find . -name '*.c') -o " + name + " -lm\n"
         "  clang -std=c11 -fwrapv -O1 -w -I runtime -I . $(find . -name '*.c') -o " + name + "\n\n"
         "With a shell, side by side (8 compilers at a time):\n\n"
         "  find . -name '*.c' | xargs -P 8 -I{} gcc -std=c11 -fwrapv -O1 -w -I runtime -I . -c {} -o {}.o\n"
         "  gcc $(find . -name '*.o') -o " + name + " -lm\n");
    std::string project;
    for (const char c : name) project += std::isalnum(static_cast<unsigned char>(c)) != 0 ? c : '_';
    std::string cmake = "cmake_minimum_required(VERSION 3.16)\nproject(" + project + " C)\n\nset(CMAKE_C_STANDARD 11)\nset(CMAKE_C_STANDARD_REQUIRED ON)\n"
                        "if(NOT MSVC)\n  add_compile_options(-fwrapv -O1 -w)\nendif()\n\nadd_executable(" + project + "\n";
    for (const std::string& source : sources) cmake += "  " + source + "\n";
    cmake += "  runtime/jrt.c)\ntarget_include_directories(" + project + " PRIVATE . runtime)\nif(UNIX)\n  target_link_libraries(" + project + " m)\nendif()\n";
    note("CMakeLists.txt", cmake);
    return written;
}

}  // namespace cb::translate::backends::c_java
