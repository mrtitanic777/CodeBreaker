#pragma once

#include <cstdint>

// CB_GPU_TRACE=<first>[-<last>]: writes the decoded GPU state of every draw in that frame range (frames counted by
// presented swaps, starting at 1) to gpu_trace.log in the working directory. Used to learn what the title actually
// uses before building the renderer (analysis/xenos/00_plan.md step 1).
namespace gpu
{
    struct Shader;

    struct DrawPacket
    {
        uint32_t opcode;        // 0x22 DRAW_INDX or 0x36 DRAW_INDX_2
        uint32_t vizToken;      // DRAW_INDX dword 0 ([5:0] query id, [8] conditional on it); 0 for DRAW_INDX_2
        uint32_t initiator;     // VGT_DRAW_INITIATOR
        uint32_t dmaBase;       // index buffer (DMA source only)
        uint32_t dmaSize;       // VGT_DMA_SIZE (DMA source only)
        bool surveyOnly;        // inside a visibility survey: not drawn (PA_SC_VIZ_QUERY)
    };

    // Whether the frame now being built (the one after `presentedFrames` swaps) is traced.
    bool TraceActive(uint64_t presentedFrames);
    void TraceDraw(const DrawPacket& draw, const Shader* vertexShader, const Shader* pixelShader);
    void TraceNote(const char* text);
    // Called at each swap with the number of frames presented so far (after this one).
    void TraceSwap(uint64_t presentedFrames);
}
