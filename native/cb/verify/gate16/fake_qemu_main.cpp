// fake_qemu_main.cpp -- TEST ONLY: stands in for qemu-system-i386 in setup16's tests (cb_fake_qemu.exe, built beside
// cb_tests). It takes `-monitor tcp:127.0.0.1:PORT,server,nowait` as QEMU does, listens there, says a banner to each
// connection and reads its lines: `screendump <path>` writes the next picture of the script to <path>; every command is
// logged. What it does is told by the environment:
//
//     FAKE_QEMU_SCRIPT  a text file with the path of a PPM on each line: the pictures `screendump` writes, in order (the
//                       last one repeats)
//     FAKE_QEMU_LOG     a file the arguments and each command are appended to
//     FAKE_QEMU_EXIT    1: the program ends at once with exit code 3 (a machine that never started)
//
// It runs until it is killed (the test's process object kills its job).
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/socket.hpp"

namespace {

std::string env(const char* name) {
    return cb::fs::get_env(name).value_or(std::string());
}

void log(const std::string& line) {
    const std::string path = env("FAKE_QEMU_LOG");
    if (path.empty())
        return;
    std::ofstream f(path, std::ios::app | std::ios::binary);
    f << line << "\n";
}

std::string slurp(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}

}  // namespace

int main(int argc, char** argv) {
    int port = 0;
    std::string args;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "-monitor" && i + 1 < argc) {
            const std::string m = argv[i + 1];
            port = std::atoi(m.c_str() + m.rfind(':') + 1);
        }
        if ((a == "-M" || a == "-m" || a == "-boot" || a == "-display" || a == "-cdrom" || a == "-drive") && i + 1 < argc)
            args += " " + a + " " + argv[i + 1];
    }
    log("args:" + args);
    if (env("FAKE_QEMU_EXIT") == "1")
        return 3;

    std::vector<std::string> script;
    {
        std::ifstream f(env("FAKE_QEMU_SCRIPT"));
        for (std::string line; std::getline(f, line);) {
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
                line.pop_back();
            if (!line.empty())
                script.push_back(line);
        }
    }

    auto listener = cb::net::Listener::listen("127.0.0.1", static_cast<std::uint16_t>(port), false);
    if (!listener)
        return 4;

    std::size_t shot = 0;
    for (;;) {
        auto accepted = listener->accept(std::chrono::hours(24));
        if (!accepted)
            continue;
        cb::net::Socket& client = *accepted;
        client.send("QEMU 8.2.0 monitor - type 'help' for more information\r\n(qemu) ");
        std::string pending;
        std::uint8_t buffer[1024];
        std::int64_t n;
        while ((n = client.receive(buffer)) > 0) {
            pending.append(reinterpret_cast<const char*>(buffer), static_cast<std::size_t>(n));
            for (std::size_t newline; (newline = pending.find('\n')) != std::string::npos;) {
                const std::string line = pending.substr(0, newline);
                pending.erase(0, newline + 1);
                if (line.rfind("screendump ", 0) == 0) {
                    log("cmd: screendump");
                    if (!script.empty()) {
                        const std::string& picture = script[shot < script.size() ? shot : script.size() - 1];
                        std::ofstream out(line.substr(11), std::ios::binary);
                        out << slurp(picture);
                    }
                    ++shot;
                } else {
                    log("cmd: " + line);
                }
                client.send("(qemu) ");
            }
        }
    }
    return 0;
}
