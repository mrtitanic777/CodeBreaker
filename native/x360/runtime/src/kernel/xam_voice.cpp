// XAM voice (headset) and Kinect (NUI) exports. No headset and no Kinect are attached.

#include "kernel/kernel.h"
#include "pool.h"

#include <cstring>
#include <mutex>
#include <set>

namespace
{
    // Voice objects (XamVoiceCreate): opaque to the title, a zeroed kernel-pool block per object.
    constexpr uint32_t kVoiceObjectSize = 0x40;
    std::mutex g_voiceMutex;
    std::set<uint32_t> g_voices;

    bool IsVoice(uint32_t voice)
    {
        std::lock_guard lock(g_voiceMutex);
        return g_voices.count(voice) != 0;
    }
}

// HRESULT XamVoiceCreate(DWORD UserIndex, DWORD MaxAttachedPackets, PVOID* Voice)
// XHV2's LocalTalkerRegister calls it for a user's controller port. The XDK says IsHeadsetPresent becomes valid only
// some time after RegisterLocalTalker succeeds, so creating the voice object does not depend on a headset; headset
// presence is asked separately (XamVoiceHeadsetPresent). The title tests the result as an HRESULT.
GUEST_IMPORT(XamVoiceCreate)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32;
    const uint32_t voicePtr = ctx.r5.u32;
    if (voicePtr == 0 || user >= 4 || !kernel::UserSignedIn(user))
        Fatal("XamVoiceCreate(user {}, {} packets, {:08X}): unexpected arguments", user, ctx.r4.u32, voicePtr);
    const uint32_t voice = KernelPool().Allocate(kVoiceObjectSize);
    if (voice == 0)
        Fatal("XamVoiceCreate: kernel pool exhausted");
    {
        std::lock_guard lock(g_voiceMutex);
        g_voices.insert(voice);
    }
    mem.Write32(voicePtr, voice);
    ctx.r3.u64 = 0; // S_OK
}

// BOOL XamVoiceHeadsetPresent(PVOID Voice): no headset is attached.
GUEST_IMPORT(XamVoiceHeadsetPresent)
{
    if (!IsVoice(ctx.r3.u32))
        Fatal("XamVoiceHeadsetPresent({:08X}): not a voice object", ctx.r3.u32);
    ctx.r3.u64 = 0;
}

// HRESULT XamVoiceSubmitPacket(PVOID Voice, DWORD Kind, PVOID Packet)
// XHV2 hands XAM packets to fill from the headset's microphone (MPpSubmitMicPacket, Kind 1) or to play on its speaker
// (MPpSubmitCodecPacket). No headset is attached, so there is no device to queue them on. On a failing HRESULT XHV2
// marks the packet failed itself (status STATUS_UNSUCCESSFUL); it tests only the sign, so the exact code (here
// HRESULT_FROM_WIN32(ERROR_DEVICE_NOT_CONNECTED); Xenia returns HRESULT_FROM_WIN32(ERROR_BUSY)) is unverified.
GUEST_IMPORT(XamVoiceSubmitPacket)
{
    if (!IsVoice(ctx.r3.u32) || ctx.r5.u32 == 0)
        Fatal("XamVoiceSubmitPacket({:08X}, {}, {:08X}): not a voice object", ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
    ctx.r3.u64 = 0x8007048F;
}

// HRESULT XamVoiceClose(PVOID Voice)
GUEST_IMPORT(XamVoiceClose)
{
    const uint32_t voice = ctx.r3.u32;
    {
        std::lock_guard lock(g_voiceMutex);
        if (!g_voices.erase(voice))
            Fatal("XamVoiceClose({:08X}): not a voice object", voice);
    }
    KernelPool().Free(voice);
    ctx.r3.u64 = 0;
}

// BOOL XamVoiceIsActiveProcess(): whether this process owns voice chat. Without Xbox LIVE party chat nothing takes
// voice away from the title, so it does. (Xenia's netplay fork answers FALSE to skip XHV2's voice paths; in TU46
// the answer only routes talker output: XHVEngineSetLocalTalkerOutput, RemoteTalkerSetOutputVolumes,
// LTpUpdateVoiceSettings.)
GUEST_IMPORT(XamVoiceIsActiveProcess)
{
    ctx.r3.u64 = 1;
}

// DWORD XamVoiceGetMicArrayStatus() (ordinal 792; XAM 0x816FBBE8): the Kinect microphone array's state. XAM answers 2
// while the array was never initialised (its status word 0x81AACBF0 is 0), else 1 or 0 from a second status word
// (0x81AACBF8, set while the array runs). Without a Kinect nothing initialises it: 2, "no array".
GUEST_IMPORT(XamVoiceGetMicArrayStatus)
{
    ctx.r3.u64 = 2;
}

// HRESULT XamVoiceGetDirectionalData(PVOID Voice, float* Direction) (ordinal 794; XAM 0x816FB450 hands the voice
// device to the XVoiced driver): the direction a talker's voice comes from, which only the Kinect's array can tell.
// XHV2 (0x81A47380) tests the result for a negative value and, then, reports no direction (a NULL pointer to its
// callback), so the failing answer is an HRESULT: the one XamVoiceSubmitPacket gives without a device, HRESULT_FROM_WIN32(
// ERROR_DEVICE_NOT_CONNECTED), whose exact value is unverified (XVoiced is not among the references). *Direction is
// left alone.
GUEST_IMPORT(XamVoiceGetDirectionalData)
{
    if (!IsVoice(ctx.r3.u32))
        Fatal("XamVoiceGetDirectionalData({:08X}, {:08X}): not a voice object", ctx.r3.u32, ctx.r4.u32);
    ctx.r3.u64 = 0x8007048F;
}

PPC_FUNC(__imp__XamVoiceGetMicArrayStatus);
PPC_FUNC(__imp__XamVoiceGetDirectionalData);
namespace
{
    kernel::DynamicExportRegistrar g_voiceGetMicArrayStatus("xam.xex", 792, __imp__XamVoiceGetMicArrayStatus);
    kernel::DynamicExportRegistrar g_voiceGetDirectionalData("xam.xex", 794, __imp__XamVoiceGetDirectionalData);
}

// DWORD XamNuiGetDeviceStatus(PNUI_DEVICE_STATUS Status): 24-byte status, zeroed first. No Kinect sensor: every
// field stays 0. The title (XNuiGetHardwareStatus) reads only the status word at +0xC and ignores the result;
// 0xC0050006 is what Xenia's netplay fork returns without a sensor.
GUEST_IMPORT(XamNuiGetDeviceStatus)
{
    std::memset(guest::g_memory.Translate(ctx.r3.u32), 0, 24);
    ctx.r3.u64 = 0xC0050006;
}
