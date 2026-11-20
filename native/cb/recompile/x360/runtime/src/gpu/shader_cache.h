#pragma once

#include "gpu/ucode.h"

#include <cstdint>
#include <string>
#include <vector>

namespace gpu
{
    // A shader as loaded by IM_LOAD / IM_LOAD_IMMEDIATE, identified by the hash of its microcode (the title compiles
    // some shaders at run time and D3D patches vertex fetches per declaration, so content is the only stable key).
    struct Shader
    {
        ucode::Stage stage;
        uint64_t hash;
        std::vector<uint32_t> words;  // host order
        ucode::Analysis analysis;
        std::string disassembly;
    };

    // The shader with these microcode words, analysed on first sight. Undefined encodings are fatal (after the
    // shader is dumped). CB_GPU_DUMP_SHADERS=1 writes every new shader to analysis/shaders/<stage>_<hash>.{bin,txt}
    // (or to the directory named by the variable). Called from the command-processor thread only.
    const Shader* LoadShader(ucode::Stage stage, std::vector<uint32_t> words);

    size_t ShaderCount();

    struct TranslationOptions;
    struct TextureBinding;
    struct SamplerBinding;

    // A translated and compiled shader: host bytecode and the texture/sampler bindings it declares.
    struct HostShader
    {
        std::vector<uint8_t> bytecode;
        std::vector<TextureBinding> textures;
        std::vector<SamplerBinding> samplers;
    };

    // `shader` translated with `options` (shader_translator.h), compiled on first use and cached. A compile failure is
    // fatal (the HLSL is dumped next to the shader dumps). The reference stays valid for the process lifetime.
    const HostShader& CompiledShader(const Shader& shader, const TranslationOptions& options);
    // Non-blocking: the compiled shader if it is ready, else nullptr (it is queued for the compile workers; the draw
    // that needs it is skipped meanwhile). CB_ASYNC_SHADERS=0: compiles at once, never nullptr (pixel-exact tests).
    const HostShader* TryCompiledShader(const Shader& shader, const TranslationOptions& options);
    // Queues the compile without waiting (the command processor's first look at a draw's shaders).
    void RequestCompiledShader(const Shader& shader, const TranslationOptions& options);
    size_t CompiledShaderCount();
    // Every (shader, options) compiled is listed in shader_cache/manifest.bin (this PC's own cache; CB_SHADER_CACHE=0:
    // none). At start-up, before the boot animation, PrefetchShaders queues them all for the compile workers, so a
    // draw seldom waits for a compile (an update that changes the translated HLSL recompiles them here, in the
    // background, instead of stuttering at each first draw).
    void PrefetchShaders();
    // Waits until every shader PrefetchShaders queued is compiled (start-up: nothing plays before, so no first draw
    // waits for a compile and the boot animation's picture keeps up with its sound).
    void WaitForPrefetchedShaders();
}
