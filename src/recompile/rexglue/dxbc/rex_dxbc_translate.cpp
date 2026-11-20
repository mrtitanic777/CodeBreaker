// rex_dxbc_translate.cpp - implementation of the Xenos-ucode -> DXBC entry point.
// Wraps the vendored ReXGlue/Xenia DxbcShaderTranslator behind a tiny, dependency-
// free API (rex_dxbc_translate.h) so the rest of the project needn't see the SDK
// headers. Provenance: Xenia (Copyright Ben Vanik, BSD-3), adapted for ReXGlue by
// Tom Clay - see LICENSE. Only this glue is ours.
#include "rex_dxbc_translate.h"

#include <rex/graphics/pipeline/shader/dxbc_translator.h>
#include <rex/graphics/pipeline/shader/shader.h>
#include <rex/string/buffer.h>

namespace rex_dxbc {

std::vector<uint8_t> Translate(const uint32_t* ucode, uint32_t dwords,
                               bool pixel_shader) {
    using namespace rex::graphics;
    if (!ucode || !dwords) return {};

    DxbcShaderTranslator translator(
        rex::ui::GraphicsProvider::GpuVendorID::kNvidia,
        /*bindless_resources_used=*/false, /*edram_rov_used=*/false);
    xenos::ShaderType type =
        pixel_shader ? xenos::ShaderType::kPixel : xenos::ShaderType::kVertex;

    Shader shader(type, /*ucode_data_hash=*/0, ucode, dwords);
    rex::string::StringBuffer disasm;
    shader.AnalyzeUcode(disasm);

    Shader::Translation* translation = shader.GetOrCreateTranslation(0);
    if (!translation) return {};
    translator.TranslateAnalyzedShader(*translation);
    if (!translation->is_translated()) return {};
    return translation->translated_binary();
}

}  // namespace rex_dxbc
