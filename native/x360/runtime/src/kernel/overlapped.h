#pragma once

#include "kernel/kernel.h"

// XOVERLAPPED (asynchronous XAM requests):
//   +0x00 InternalLow (result), +0x04 InternalHigh (length), +0x08 InternalContext (requesting thread handle),
//   +0x0C hEvent, +0x10 pCompletionRoutine, +0x14 dwCompletionContext, +0x18 dwExtendedError.
namespace kernel
{
    constexpr uint32_t kErrorSuccess = 0;
    constexpr uint32_t kErrorIoPending = 0x000003E5;     // ERROR_IO_PENDING
    constexpr uint32_t kErrorIoIncomplete = 0x000003E4;  // ERROR_IO_INCOMPLETE

    // Starts a request that completes later (from any thread): InternalLow = ERROR_IO_PENDING, hEvent reset. A request
    // with a completion routine must then complete on a guest thread (the routine runs as its user APC).
    void BeginOverlapped(uint32_t overlapped);

    // Completes an overlapped request: stores result/extended error/length, signals hEvent.
    void CompleteOverlapped(uint32_t overlapped, uint32_t result, uint32_t extendedError, uint32_t length);

    // Completes a XAM request the way the XDK documents for asynchronous calls: success stores InternalLow 0 and
    // `information` in InternalHigh; failure stores ERROR_FUNCTION_FAILED with HRESULT_FROM_WIN32(error) as the
    // extended error (titles test XGetOverlappedExtendedError, e.g. 0x80070012 for the end of an enumeration).
    void CompleteOverlappedWin32(uint32_t overlapped, uint32_t error, uint32_t information);

    // Cancels the pending system-UI request using this overlapped structure (xam_ui.cpp): it completes with
    // ERROR_CANCELLED. False if no open dialog uses it.
    bool CancelSystemUi(uint32_t overlapped);

    // Immediate completion as XAM does for synchronous work: length is 0 on success and 0xFFFFFFFF on failure.
    // (Xenia Canary notes titles that test the length as a success indicator; not verified on hardware.)
    void CompleteOverlappedImmediate(uint32_t overlapped, uint32_t result);
}
