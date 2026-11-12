// cb/exact/xenix_c.hpp -- the C compiler of XENIX for the 8086 (cpp, c0, c1, c2, as; 1982-83), as a toolchain.
//
// The profile (toolchains/xenixc.json) names this module ("plugin"); what cannot be data is here. The compiler that survives is the Altos
// ACS 8600 XENIX 2.3 diskette's; the programs Microsoft built with it (LINK 2.40/2.44, LIB 2.20/2.40, 1983-84) were made by another
// revision. The variant "microsoft" is that revision, as far as their bytes tell:
//
//   - c1 without its stack probe (the limit word it keeps: 32767)
//   - c1's assembly revised before c2 reads it (rules U D M B: where a value is made)
//   - c2: at an unconditional jump its two `mov reg,reg` passes give up (and one of them also at the note that ends a function); a jump
//     or call through a pointer counts 6 more to its reckoning of how far a jump goes; a constant is not taken from a register a
//     variable lives in; a call through a pointer uses every register
//   - c2's block moves: a loop's block is left where it is, a block a conditional jump leads to over one other changes places with it
//     (hooks at two places of codemove)
//
// Run under the XENIX emulator (exact/emu/xenix). The object is a Version 7 a.out (exact/aout).
//
//     toolchain::Toolchains toolchains(locations, &runner, &cache);
//     xenix_c::register_plugin(toolchains);
//     auto tc = toolchains.load("xenixc");                    // the "microsoft" variant by default
//     modules::CompileResult r = tc->compile(source, {.headers = {{"tree.h", bytes}}});
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/json_fwd.hpp"
#include "cb/exact/emu/xenix.hpp"
#include "cb/exact/toolchain.hpp"

namespace cb::exact::xenix_c {

inline constexpr std::string_view kCacheVersion = "x1";
inline constexpr std::string_view kPluginName = "codebreaker.exact.xenix_c:XenixC";

// ---- c1's assembly, revised (Microsoft's c1) -----------------------------------------------------------------------------

// An unsigned word made a long: this c1 loads the low register of the pair and clears the high one; Microsoft's made the long in AX:DX
// and moved it to the pair if it was wanted in one.
std::string rule_u(std::string_view assembly);
// A ++ or -- put off until its byte is fetched: Microsoft's c1 does it when the byte is the value wanted.
std::string rule_d(std::string_view assembly);
// op (R),M with M in memory (no 8086 instruction): Microsoft's c1 brought M to AX first.
std::string rule_m(std::string_view assembly);
// A char made an int OR'ed with a constant: Microsoft's c1 did it on the word.
std::string rule_b(std::string_view assembly);
// The rules named by their letters, in that order (U D M B). UsageError for a letter that is no rule.
std::string apply_rules(std::string_view letters, std::string_view assembly);

// ---- the passes, changed as Microsoft's --------------------------------------------------------------------------------------

// c1 without its stack probe: the limit word it keeps (100) is 32767. FormatError when this is not the c1 the patch was written for.
Bytes patched_c1(Bytes c1);
// c2 with the changes listed above (code added at the end of its text, a few jumps into it). FormatError when this is not the c2 the patch
// was written for.
Bytes patched_c2(Bytes c2);

// c2's block moves, from outside: hooks at two places of codemove, by offset in the text. `mode` has R (a loop's block stays) and S (a
// block a conditional jump leads to over one other changes places with it).
std::map<std::uint32_t, emu::xenix::Hook> c2_hooks(std::string_view mode = "RS");

class XenixC final : public toolchain::Toolchain {
public:
    // UsageError for a variant the profile does not have; NotFoundError when the profile's file is not there. The variant's "rules" (the
    // letters of the assembly rules), "patch" (c1 and c2 patched) and "c2move" (the block-move hooks' mode) say what it is.
    XenixC(toolchain::Profile profile, std::optional<std::string> variant, const toolchain::Context& context);
    ~XenixC() override;

    const std::string& rules() const { return rules_; }
    bool patch() const { return patch_; }
    const std::string& c2move() const { return c2move_; }
    const std::string& plugin_key() const { return plugin_key_; }

    // The object (the a.out `as` wrote) and what the passes printed. A pass that stops, or says why, ends the compile there: the object is
    // none and the console says which pass. The headers (and the .h files of the include folders) are written beside the source with LF
    // line ends. Cached under the source, the headers, the include files and plugin_key.
    modules::CompileResult compile(ByteView source, const modules::CompileOptions& options) const override;

    // What compile() keeps its answer under, and the compile itself without the cache (a caller with a cache of its own).
    std::string cache_key(ByteView source, const modules::CompileOptions& options) const;
    modules::CompileResult build(ByteView source, const modules::CompileOptions& options) const;

private:
    modules::CompileResult run_passes(const Bytes& source, const std::vector<std::pair<std::string, Bytes>>& headers) const;

    toolchain::Context ctx_;
    std::string rules_, c2move_, plugin_key_;
    bool patch_ = false;
    std::unique_ptr<Json> raw_;   // the profile's own JSON
};

void register_plugin(toolchain::Toolchains& toolchains);

}  // namespace cb::exact::xenix_c
