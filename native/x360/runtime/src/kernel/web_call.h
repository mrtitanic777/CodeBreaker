#pragma once

// XLiveBase 0x5806A..0x5806F, the web-service calls (xam_web_call.cpp).

#include "kernel/kernel.h"
#include "live/client.h"

namespace kernel
{
    // One call, read from the caller's message block (on its thread: the block is on its stack).
    struct WebCall
    {
        uint32_t message = 0;
        live::WsCallRequest call;
        uint32_t handle = 0, status = 0, resp = 0, size = 0, out = 0, taskBuffer = 0;
        bool sync = false; // Close: runs at once
    };
    WebCall WebCallPrepare(PPCContext& ctx, uint32_t message, uint32_t buffer);
    // The network part and the results (any thread); the HRESULT the call completes with.
    uint32_t WebCallRun(const WebCall& call);
}
