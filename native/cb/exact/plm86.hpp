// cb/exact/plm86.hpp -- Intel's ISIS-II tools for the 8086 (PL/M-86 V1.1-2.1, ASM86 2.0/2.1; 8080 programs), as toolchains.
//
// A profile names this module (toolchains/plm86.json, asm86.json: "plugin": "codebreaker.exact.plm86:PLM86"). The tool runs under the
// ISIS-II emulator (exact/emu/isis); the object is Intel's OMF-86 (exact/omf reads it). The profile says which:
//
//     "tool": folder of the kit (plm86, asm86)   "program": its file (PLM86, ASM86)
//     "source": the name the source is given (T.PLM, T.A86)
//     "tail": the command line after the program, {controls} the variant's
//     "ok": a regular expression the console and listing must match (group 1 "0": no errors), or none
//     "limit": the instructions the tool may run (400 million by default)
//
// and the variants the version and the controls: {"version": "2.1", "controls": "MEDIUM OPTIMIZE(2) CODE"}. A source's $INCLUDEd files are
// the project's headers (the profile's "headers" extensions: .LIT).
//
//     toolchain::Toolchains toolchains(locations, &runner, &cache);
//     plm86::register_plugin(toolchains);
//     auto tc = toolchains.load("plm86");
//     modules::CompileResult r = tc->compile(source, {});     // r.object: the OMF-86 object, or none; r.console: what the tool printed
#pragma once

#include <memory>
#include <optional>
#include <string>

#include "cb/core/json_fwd.hpp"
#include "cb/core/regex.hpp"
#include "cb/exact/toolchain.hpp"

namespace cb::exact::plm86 {

inline constexpr std::string_view kCacheVersion = "p2";
inline constexpr std::string_view kPluginName = "codebreaker.exact.plm86:PLM86";

class Plm86 final : public toolchain::Toolchain {
public:
    // UsageError for a variant the profile does not have; NotFoundError when the profile's file is not there.
    Plm86(toolchain::Profile profile, std::optional<std::string> variant, const toolchain::Context& context);
    ~Plm86() override;

    const std::string& version() const { return version_; }
    const std::string& controls() const { return controls_; }
    // sha1(name + version + controls + kCacheVersion), 12 hex digits: names this toolchain's compiles in the cache.
    const std::string& plugin_key() const { return plugin_key_; }

    // The object module and what the tool printed. The headers are written to the tool's drive as they are named, in upper case, with
    // CR LF line ends; `switches` are the controls (the variant's by default). The include folders are not used (a source's own
    // $INCLUDEs are its headers). Cached under the source, the headers, the controls and plugin_key.
    modules::CompileResult compile(ByteView source, const modules::CompileOptions& options) const override;

    // What compile() keeps its answer under, and the compile itself without the cache (a caller with a cache of its own).
    std::string cache_key(ByteView source, const modules::CompileOptions& options) const;
    modules::CompileResult build(ByteView source, const modules::CompileOptions& options) const;

private:
    modules::CompileResult run_tool(const Bytes& source, const std::vector<std::pair<std::string, Bytes>>& headers, const std::string& controls) const;

    toolchain::Context ctx_;
    std::string version_, controls_, plugin_key_;
    std::unique_ptr<Json> raw_;     // the profile's own JSON
    std::shared_ptr<Regex> ok_;     // the profile's "ok", compiled once (none: any run that made an object is good)
};

// Registers the factory under kPluginName.
void register_plugin(toolchain::Toolchains& toolchains);

}  // namespace cb::exact::plm86
