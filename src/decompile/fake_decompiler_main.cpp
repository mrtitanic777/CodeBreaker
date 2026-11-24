// fake_decompiler_main.cpp -- TEST ONLY: stands in for decompile/engine/decompiler.exe in decompiler_test.cpp (cb_fake_decompiler.exe, built
// beside the tests). It reads a description the way the real program does (`F <hex> <name> ...` lines; `--at <hex>...` limits it to those),
// writes a block for each function -- the line `/*@@ <name> <hex> */`, notes, a C body -- and can be told, by environment variables, to
// behave as the real one does at its worst:
//
//     FAKE_DECOMPILER_LOG=<file>      each start appends one line: the arguments and the switches it was given (CB_*)
//     FAKE_DECOMPILER_DIE=<hex,...>   dies (exit 3) as it begins those functions, after writing the marker line
//     FAKE_DECOMPILER_NOTES=1         writes a global, a pointer and a structure note before each body
//     FAKE_DECOMPILER_L2=<file>       writes the file's text as the function's L2 note
//     FAKE_DECOMPILER_FAIL=<hex>      writes a `decompile failed` body for that function
//     FAKE_DECOMPILER_LONG=<hex>      writes the "too many instructions" failure for that function unless CB_MAX_INSTRUCTIONS gives room
//     FAKE_DECOMPILER_SLEEP_MS=<n>    waits that long on each function (to see progress while it works)
// It notes each function it finishes in CB_PROGRESS_FILE, as the real one does.
#if defined(_WIN32)
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace {

#if !defined(_WIN32)
std::string env(const char* name) {
    const char* value = std::getenv(name);
    return value ? std::string(value) : std::string();
}
#else
std::string env(const char* name) {
    const DWORD need = GetEnvironmentVariableA(name, nullptr, 0);   // (the Win32 call: every Windows compiler's CRT has it)
    if (need == 0) return {};
    std::string out(need, '\0');
    out.resize(GetEnvironmentVariableA(name, out.data(), need));
    return out;
}
#endif

std::vector<std::string> split(const std::string& s, char sep) {
    std::vector<std::string> out;
    std::string cur;
    std::istringstream in(s);
    while (std::getline(in, cur, sep)) out.push_back(cur);
    return out;
}

unsigned long long hex(const std::string& s) { return std::strtoull(s.c_str(), nullptr, 16); }

}  // namespace

int main(int argc, char** argv) {
    // (stdout stays in text mode, as the real program's is: every line it writes ends in CR LF)
    if (argc < 2) return 2;
    const std::string description = argv[1];
    std::vector<std::string> at;
    bool have_at = false;
    for (int i = 2; i < argc; ++i) {
        if (std::string(argv[i]) == "--at") have_at = true;
        else if (have_at) at.push_back(argv[i]);
    }
    if (const std::string log = env("FAKE_DECOMPILER_LOG"); !log.empty()) {
        std::string line = "run";
        for (int i = 2; i < argc; ++i) line += std::string(" ") + argv[i];
        line += " | threads=" + env("CB_THREADS") + " structs=" + env("CB_STRUCTS") + " l2=" + env("CB_L2") + " max=" + env("CB_MAX_INSTRUCTIONS") + "\n";
        // (one append of the whole line: processes running at once do not lose each other's lines)
#if defined(_WIN32)
        HANDLE file = CreateFileA(log.c_str(), FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (file != INVALID_HANDLE_VALUE) {
            DWORD written = 0;
            WriteFile(file, line.data(), static_cast<DWORD>(line.size()), &written, nullptr);
            CloseHandle(file);
        }
#else
        if (const int file = ::open(log.c_str(), O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644); file >= 0) {
            [[maybe_unused]] const ssize_t written = ::write(file, line.data(), line.size());
            ::close(file);
        }
#endif
    }
    std::vector<std::pair<unsigned long long, std::string>> functions;   // in the description's order
    std::ifstream in(description);
    std::string line;
    while (std::getline(in, line)) {
        const std::vector<std::string> p = split(line, '\t');
        if (p.size() > 2 && p[0] == "F") functions.emplace_back(hex(p[1]), p[2]);
    }
    std::vector<std::pair<unsigned long long, std::string>> todo;
    if (have_at) {
        for (const std::string& a : at)
            for (const auto& f : functions)
                if (f.first == hex(a)) todo.push_back(f);
    } else {
        todo = functions;
    }
    std::set<unsigned long long> die;
    for (const std::string& d : split(env("FAKE_DECOMPILER_DIE"), ',')) die.insert(hex(d));
    const std::string l2 = env("FAKE_DECOMPILER_L2");
    std::string l2_text;
    if (!l2.empty()) {
        std::ifstream f(l2);
        std::stringstream all;
        all << f.rdbuf();
        l2_text = all.str();
    }
    const unsigned long long fail = env("FAKE_DECOMPILER_FAIL").empty() ? ~0ULL : hex(env("FAKE_DECOMPILER_FAIL"));
    const unsigned long long long_one = env("FAKE_DECOMPILER_LONG").empty() || !env("CB_MAX_INSTRUCTIONS").empty() ? ~0ULL : hex(env("FAKE_DECOMPILER_LONG"));
    const int sleep_ms = std::atoi(env("FAKE_DECOMPILER_SLEEP_MS").c_str());
    const std::string progress_file = env("CB_PROGRESS_FILE");
    const auto note_finished = [&] {
        if (progress_file.empty()) return;
        std::ofstream(progress_file, std::ios::binary | std::ios::app) << '\n';   // (opened and closed each time: the caller reads it meanwhile)
    };
    for (const auto& [va, name] : todo) {
        std::printf("/*@@ %s %llx */\n", name.c_str(), va);
        std::fflush(stdout);
        if (die.count(va) != 0) return 3;
        if (sleep_ms > 0) std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
        if (va == fail) {
            std::printf("/* decompile failed: nothing here */\n");
            note_finished();
            continue;
        }
        if (va == long_one) {
            std::printf("/* decompile failed: Flow exceeded maximum allowable instructions */\n");
            note_finished();
            continue;
        }
        if (env("FAKE_DECOMPILER_NOTES") == "1") std::printf("/*@G %llx 4 4 */\n/*@P %s_p 4 */\n/*@S struct_%s 8 0:4:int4 4:4:int4 */\n", va, name.c_str(), name.c_str());
        if (!l2_text.empty()) std::printf("/*@L2 %s */\n", l2_text.c_str());
        std::printf("int %s(void)\n{\n  return %llu;\n}\n", name.c_str(), va);
        std::fflush(stdout);
        note_finished();
    }
    return 0;
}
