#include "cb/recompile/xenon_tools.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/formats/xex.hpp"
#include "cb/recompile/disassemble.hpp"
#include "cb/recompile/xenon.hpp"

namespace cb::recompile::xenon_tools {

namespace {

namespace stdfs = std::filesystem;
namespace pi = formats::powerpc_image;
namespace rcfg = cb::recompile::config;

// The xex program and where its unpacked images go: the settings `xex_bin` and the state folder.
formats::xex::Tools tools_of(const cb::config::Config& config) {
    formats::xex::Tools tools;
    tools.xex_program = config.path("xex_bin").value_or(stdfs::path{}) / "xex.exe";
    tools.unpacked_dir = config.state() / "unpacked" / "xex";
    return tools;
}

// A file written whole or not at all: <out>.part, then renamed.
void write_text_whole(const stdfs::path& out, std::string_view text) {
    if (out.has_parent_path()) fs::make_dirs(out.parent_path());
    stdfs::path part = out;
    part += ".part";
    fs::write_text(part, text);
    stdfs::rename(part, out);
}

}  // namespace

cli::Command xex_basic_command(const Environment& environment) {
    return cli::Command{
        "xex-basic",
        "the unencrypted, basic-compressed form of an .xex that XenonRecomp reads",
        {cli::Positional{"xex", "the .xex"}, cli::Positional{"out", "the basic .xex to write"}},
        [environment](const cli::Args& args) {
            const stdfs::path out = fs::from_text(args.required("out"));
            if (out.has_parent_path()) fs::make_dirs(out.parent_path());
            formats::xex::write_basic(tools_of(environment.config), fs::from_text(args.required("xex")), out);
            return 0;
        },
        {}, "Xbox 360", ""};
}

cli::Command xenon_config_command(const Environment& environment) {
    return cli::Command{
        "xenon-config",
        "the XenonRecomp configuration of a basic .xex",
        {cli::Positional{"xex", "the basic .xex"}, cli::Positional{"out", "the .toml to write"},
         cli::Option{"--out-dir", "", "the folder XenonRecomp writes its sources to (default ppc)", cli::Arg::Value, "DIR", "ppc"},
         cli::Option{"--switch-tables", "", "the switch tables file the configuration names (default switch_tables.toml)", cli::Arg::Value, "FILE", "switch_tables.toml"},
         cli::Option{"--file-path", "", "the .xex as the configuration names it (default: the .xex as given)", cli::Arg::Value, "FILE", ""}},
        [environment](const cli::Args& args) {
            const stdfs::path xex = fs::from_text(args.required("xex"));
            const pi::Image image = command::read_image(xex, environment);
            disassemble::Disassembler dis(command::vector_decoder());
            rcfg::MakeOptions options;
            options.out_dir = args.required("out-dir");
            options.switch_tables = args.required("switch-tables");
            if (const std::optional<std::string> file_path = args.value("file-path"); file_path && !file_path->empty()) options.file_path = *file_path;
            write_text_whole(fs::from_text(args.required("out")), rcfg::make(command::config_title(image), dis, xex, options));
            return 0;
        },
        {}, "Xbox 360", ""};
}

cli::Command switch_tables_command(const Environment& environment) {
    return cli::Command{
        "switch-tables",
        "the switch tables of a basic .xex: the finder's, with XenonAnalyse's for the rest",
        {cli::Positional{"xex", "the basic .xex"}, cli::Positional{"out", "the .toml to write"}},
        [environment](const cli::Args& args) {
            const stdfs::path xex = fs::from_text(args.required("xex"));
            const stdfs::path out = fs::from_text(args.required("out"));
            stdfs::path analysed = out;
            analysed.replace_extension(".analysed.toml");
            if (out.has_parent_path()) fs::make_dirs(out.parent_path());

            // XenonAnalyse's tables for the bctr the finder leaves unresolved; where it cannot be had the finder's alone are written
            std::optional<std::string> with_analysed;
            try {
                const cb::config::Config& config = environment.config;
                xenon::Config xc;
                xc.checkout_root = config.checkout_root();
                xc.state_dir = config.state();
                xc.visual_studio = config.path("visual_studio");
                xc.ppc_binutils = config.path("ppc_binutils");
                xenon::Xenon tool(std::move(xc));
                tool.build();
                process::run({.argv = {fs::to_text(tool.analyse_exe()), fs::to_text(xex), fs::to_text(analysed)},
                              .out = process::Capture::Discard,
                              .err = process::Capture::Discard});
                std::error_code ec;
                if (stdfs::is_regular_file(analysed, ec)) with_analysed = fs::read_text(analysed);
            } catch (const Error& e) {
                cb::err("NOTE XenonAnalyse did not run ({}); switch tables from the finder alone\n", e.what());
            }
            stdfs::path part = out;
            part += ".part";
            const pi::Image image = command::read_image(xex, environment);
            rcfg::write_switch_tables(part, command::code_image(image), with_analysed);
            stdfs::rename(part, out);
            return 0;
        },
        {}, "Xbox 360", ""};
}

std::vector<cli::Command> commands(const Environment& environment) {
    return {xex_basic_command(environment), xenon_config_command(environment), switch_tables_command(environment)};
}

}  // namespace cb::recompile::xenon_tools
