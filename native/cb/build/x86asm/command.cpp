// cb/build/x86asm/command.cpp -- `codebreaker nasm <args>` (command.hpp).
#include "cb/build/x86asm/command.hpp"

#include <filesystem>
#include <fstream>
#include <iterator>

#include "cb/build/x86asm/driver.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"

namespace cb::build::x86asm {

namespace stdfs = std::filesystem;

int run_nasm_command(std::span<const std::string> args) {
    Request request;
    request.args.assign(args.begin(), args.end());
    std::error_code ec;
    request.cwd = fs::to_text(stdfs::current_path(ec));
    request.read = [](const std::string& name) -> std::optional<Bytes> {
        std::error_code e;
        const stdfs::path p = fs::from_text(name);
        if (!stdfs::is_regular_file(p, e)) return std::nullopt;
        try {
            return fs::read_file(p);
        } catch (const cb::Error&) {
            return std::nullopt;
        }
    };

#if defined(_WIN32)
    constexpr bool kCrlf = true;   // text files on Windows have CR LF line ends, as nasm.exe wrote them
#else
    constexpr bool kCrlf = false;
#endif
    const RunResult result = run(request, kCrlf);

    int status = result.exit;
    for (const auto& [name, data] : result.files) {
        try {
            fs::write_file(fs::from_text(name), data);
        } catch (const cb::Error& e) {
            cb::err_text(std::string("nasm: unable to write ") + name + ": " + e.what() + "\n");
            status = status ? status : 1;
        }
    }
    cb::out_text(result.out);
    cb::flush_out();
    cb::err_text(result.err);
    return status;
}

}  // namespace cb::build::x86asm
