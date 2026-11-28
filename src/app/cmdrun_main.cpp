// cmdrun: runs the command layers of the formats, build and analyze areas on a command line, with the surroundings the integrator's table
// will give them (a corpus, the xex program, the console ordinals, OpenWatcom), so each can be run on
// real inputs. Not built by default (`cmake --build <dir> --target cb_cmdrun`).
//
//     cmdrun [--corpus DIR] [--xex-bin DIR] [--state DIR] [--ordinals FILE] [--openwatcom DIR] [--design DIR] <command> [args...]
#include <algorithm>
#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "analyze/ppc_calls.hpp"
#include "build/disc_image.hpp"
#include "build/floppy.hpp"
#include "toolchain/watcom.hpp"
#include "build/xex_edit.hpp"
#include "core/cli.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "formats/fonts.hpp"
#include "formats/gfx.hpp"
#include "formats/media.hpp"
#include "formats/xbox/crypto/tools.hpp"
#include "formats/xbox/system_ui.hpp"
#include "formats/xex_imports.hpp"
#include "source/dos_writer.hpp"

namespace {

namespace stdfs = std::filesystem;
using namespace cb;

// A file of the corpus by name (case ignored), found by walking it once.
std::optional<stdfs::path> corpus_file(const stdfs::path& root, std::string_view name) {
    std::error_code ec;
    if (root.empty() || !stdfs::is_directory(root, ec)) return std::nullopt;
    const std::string wanted = text::lower(name);
    for (const auto& entry : stdfs::recursive_directory_iterator(root, stdfs::directory_options::skip_permission_denied, ec)) {
        if (entry.is_regular_file(ec) && text::lower(fs::to_text(entry.path().filename())) == wanted) return entry.path();
    }
    return std::nullopt;
}

}  // namespace

int main(int argc, char** argv) {
    stdfs::path corpus, state = "state", openwatcom, design, ordinals;
    int at = 1;
    for (; at + 1 < argc; at += 2) {
        const std::string flag = argv[at];
        const stdfs::path value = fs::from_text(argv[at + 1]);
        if (flag == "--corpus") corpus = value;
        else if (flag == "--state") state = value;
        else if (flag == "--openwatcom") openwatcom = value;
        else if (flag == "--design") design = value;
        else if (flag == "--ordinals") ordinals = value;
        else break;
    }
    if (design.empty() && !corpus.empty()) design = corpus.parent_path().parent_path();

    formats::xex::Tools tools;
    tools.unpacked_dir = state / "unpacked" / "xex";
    formats::xex_imports::OrdinalNames names;
    if (!ordinals.empty()) {
        auto table = std::make_shared<Json>(json::load(ordinals));
        names = [table](std::string_view library, std::uint32_t ordinal) -> std::optional<std::string> {
            const auto lib = table->find(std::string(library));
            if (lib == table->end() || !lib->is_object()) return std::nullopt;
            const auto name = lib->find(std::to_string(ordinal));
            if (name == lib->end() || !name->is_string()) return std::nullopt;
            return name->get<std::string>();
        };
    }

    formats::fonts::Environment fonts_env;
    fonts_env.retail = [corpus](std::string_view name) { return corpus_file(corpus, name); };
    formats::xex_imports::Environment imports_env{tools, names};
    build::xex_edit::Environment edit_env{tools};
    analyze::ppc_calls::Environment calls_env{tools, names};
    formats::xbox::system_ui::Environment system_ui_env;
    build::floppy::Environment floppy_env;
    floppy_env.corpus_root = corpus;
    floppy_env.design_dir = design;
    floppy_env.state = state;
    if (!openwatcom.empty()) {
        const toolchain::watcom::OpenWatcom watcom(openwatcom);
        floppy_env.writer = source::dos_writer::options_for(watcom, state / "assembly_probes.json");
    }

    cli::Cli program("cmdrun", {formats::fonts::command(fonts_env), formats::gfx::command(), formats::xbox::system_ui::command(system_ui_env), formats::xbox::crypto::command(formats::xbox::crypto::Environment{}), formats::media::command(), formats::xex_imports::command(imports_env),
                                build::xex_edit::command(edit_env), analyze::ppc_calls::command(calls_env), build::disc_image::command(),
                                build::floppy::command(floppy_env)});
    std::vector<std::string> args(argv + at, argv + argc);
    try {
        return program.run(args);
    } catch (const UsageError& e) {
        err("error: {}\n", e.what());
        return kExitUsage;
    } catch (const Error& e) {
        err("error: {}\n", e.what());
        return e.exit_code();
    }
}
