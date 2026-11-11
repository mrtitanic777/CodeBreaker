// Tests of cb/recompile/gpu: the generated graphics/ subsystem is the original's text, byte for byte (the SHA-256 of
// each file is that of the original module's string), and the design note names the project.

#include "cb/recompile/gpu.hpp"

#include <algorithm>

#include "cb/core/crypto.hpp"
#include "cb/core/testing.hpp"

using namespace cb::recompile::gpu;

namespace {

std::string sha(std::string_view text) { return cb::crypto::to_hex(cb::crypto::sha256(cb::as_bytes(text))); }

// the braces of C++ text balance (outside nothing fancy: the templates have no braces in strings or comments that
// do not pair)
bool has(std::string_view text, std::string_view needle) { return text.find(needle) != std::string_view::npos; }

bool braces_balance(std::string_view text) {
    long depth = 0;
    for (const char c : text) {
        if (c == '{') ++depth;
        if (c == '}' && --depth < 0) return false;
    }
    return depth == 0;
}

}  // namespace

CB_TEST("recompile.gpu.the_files_are_the_originals", [] {
    // SHA-256 of codebreaker.recompile.gpu.GPU_H, GPU_CPP, D3D12_CPP and TRANSLATORS_CPP (computed from the Python
    // module's strings when this port was made)
    const auto files = graphics_files();
    CB_CHECK_EQ(files.size(), 4u);
    CB_CHECK_EQ(files[0].path, "graphics/xe_gpu.h");
    CB_CHECK_EQ(sha(files[0].text), "86cc2bc895560a1a8e350b294609dbe372c4139bd84a6b970ee9be778361200c");
    CB_CHECK_EQ(files[1].path, "graphics/xe_gpu.cpp");
    CB_CHECK_EQ(sha(files[1].text), "8760a91ae0a2eeb66d6e6b3843a8ba069d12b56a2761bcf1464885b6af59c97b");
    CB_CHECK_EQ(files[2].path, "graphics/xe_d3d12.cpp");
    CB_CHECK_EQ(sha(files[2].text), "f54adbfed0cd147704f8b362bee6a60201911a2b0322cea88c8af002bb5b1941");
    CB_CHECK_EQ(files[3].path, "graphics/xe_translators.cpp");
    CB_CHECK_EQ(sha(files[3].text), "8dbc2c582692f2268d898f2fa5fe776e50c49c0295ac49d3092b060922b5ec3f");
});

CB_TEST("recompile.gpu.the_templates_are_whole_cpp_sources", [] {
    for (const GeneratedFile& f : graphics_files()) {
        CB_CHECK(!f.text.empty());
        CB_CHECK(braces_balance(f.text));
        CB_CHECK(f.text.find('\r') == std::string_view::npos);
        CB_CHECK(f.text.find("{name}") == std::string_view::npos);   // no placeholder left to fill
        CB_CHECK(f.text.ends_with("\n"));
    }
    const auto files = graphics_files();
    CB_CHECK(has(files[0].text, "#pragma once"));
    CB_CHECK(has(files[0].text, "class CommandProcessor"));
    CB_CHECK(has(files[1].text, "#include \"xe_gpu.h\""));
    CB_CHECK(has(files[2].text, "class D3D12Backend"));
    CB_CHECK(has(files[2].text, "class NullBackend"));
    CB_CHECK(has(files[3].text, "CreateShaderTranslator"));
});

CB_TEST("recompile.gpu.the_design_note_names_the_project", [] {
    const std::string note = design_md("Minecraft");
    // SHA-256 of codebreaker.recompile.gpu.design_md("Minecraft")
    CB_CHECK_EQ(sha(note), "03ed043d05781014d2464663dc7774937594fdbb36a919306ef0074149b62e2d");
    CB_CHECK(note.starts_with("# Minecraft GPU subsystem - native D3D12 design\n"));
    CB_CHECK(has(note, "`Vd*` kernel shims (in `Minecraft_imports.cpp`)"));
    CB_CHECK(has(note, "`graphics/xe_gpu.{h,cpp}`"));   // the template's own braces stay
    CB_CHECK(!has(design_md("X"), "{name}"));
    CB_CHECK(design_md("").starts_with("#  GPU subsystem"));
});
