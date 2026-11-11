// codebreaker/jobs/workers.cpp -- see the header.

#include "codebreaker/jobs/workers.hpp"

#include <atomic>
#include <fstream>
#include <mutex>

#include "codebreaker/base/parallel.hpp"
#include "codebreaker/build/module.hpp"
#include "codebreaker/engine/lanes_wiring.hpp"
#include "codebreaker/verify/gate16/isolated16.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py_stdlib.hpp"
#include "py_subprocess.hpp"

#include <windows.h>
#undef near
#undef far

namespace codebreaker::jobs::workers {

namespace {
std::mutex g_mu;
std::string g_exe;
std::atomic<int> g_serial{0};
}  // namespace

void enable(const std::string& exe) {
    std::lock_guard<std::mutex> g(g_mu);
    char buf[MAX_PATH * 4];
    const DWORD n = GetModuleFileNameA(nullptr, buf, sizeof buf);   // (argv[0] may be relative or without .exe)
    g_exe = n ? std::string(buf, n) : exe;
}

bool enabled() {
    std::lock_guard<std::mutex> g(g_mu);
    return !g_exe.empty() && !std::getenv("CB_NO_WORKERS");
}

int64_t count() { return std::max<int64_t>(1, base::parallel::workers()); }

int64_t run(const std::vector<std::string>& args) {
    std::string exe;
    {
        std::lock_guard<std::mutex> g(g_mu);
        exe = g_exe;
    }
    py::list<py::str> argv;
    argv.append(py::str(exe));
    argv.append(py::str("--cb-worker"));
    for (const std::string& a : args) argv.append(py::str(a));
    py::subprocess::RunOptions o;
    o.stdin_ = py::subprocess::Stream::Devnull;
    o.stdout_ = py::subprocess::Stream::Devnull;
    o.stderr_ = py::subprocess::Stream::Inherit;
    return py::subprocess::run(argv, o).returncode;
}

bool dispatch(int argc, char** argv, int& rc) {
    if (argc < 3 || std::string(argv[1]) != "--cb-worker") return false;
    std::vector<std::string> args(argv + 3, argv + argc);
    const std::string kind = argv[2];
    try {
        engine::lanes_wiring::register_core_platforms();
        if (kind == "gate") rc = verify::gate16::isolated16::worker_main(args);
        else if (kind == "judge") rc = verify::reconstruct::judge_worker(args);
        else if (kind == "build") rc = static_cast<int>(build::module::build_worker(args));
        else rc = 2;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "worker %s: %s\n", kind.c_str(), e.what());
        rc = 3;
    }
    return true;
}

std::string hex(const std::string& raw) {
    static const char* d = "0123456789abcdef";
    std::string o;
    for (unsigned char c : raw) {
        o += d[c >> 4];
        o += d[c & 15];
    }
    return o.empty() ? "-" : o;
}

std::string unhex(const std::string& text) {
    if (text == "-") return "";
    auto v = [](char c) { return c <= '9' ? c - '0' : c - 'a' + 10; };
    std::string o;
    for (size_t i = 0; i + 1 < text.size(); i += 2) o += static_cast<char>(v(text[i]) * 16 + v(text[i + 1]));
    return o;
}

Enc& Enc::i(int64_t v) {
    out_ += (out_.empty() ? "" : " ") + std::to_string(v);
    return *this;
}
Enc& Enc::s(const std::string& v) {
    out_ += (out_.empty() ? "" : " ") + hex(v);
    return *this;
}

Dec::Dec(const std::string& text) {
    size_t at = 0;
    while (at < text.size()) {
        size_t sp = text.find(' ', at);
        if (sp == std::string::npos) sp = text.size();
        if (sp > at) tokens_.push_back(text.substr(at, sp - at));
        at = sp + 1;
    }
}
int64_t Dec::i() { return std::stoll(tokens_.at(at_++)); }
std::string Dec::s() { return unhex(tokens_.at(at_++)); }

std::string read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}
void write_file(const std::string& path, const std::string& text) {
    std::ofstream out(path, std::ios::binary);
    out << text;
}

std::string scratch_folder() {
    const std::string base = py::tempfile::gettempdir().utf8() + "\\codebreaker_workers";
    CreateDirectoryA(base.c_str(), nullptr);
    const std::string d = base + "\\w" + std::to_string(GetCurrentProcessId()) + "_" + std::to_string(g_serial++);
    CreateDirectoryA(d.c_str(), nullptr);
    return d;
}

}  // namespace codebreaker::jobs::workers
