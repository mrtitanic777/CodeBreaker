// Driver-level kernel imports that xam.xex uses (CB_RECOMP_XAM, kernel/xam_recompiled.cpp): the callbacks XAM
// registers with the kernel's drivers, and hardware this PC does not have. Semantics from how xam.xex 17559 calls them
// (analysis/xam_all_ann.c); Xenia and rexglue only stub these.

#include "kernel/kernel.h"
#include "kernel/ani.h"
#include "kernel/objects.h"

#include <map>
#include <mutex>
#include <set>
#include <string>

#include <array>
#include <atomic>

namespace
{
    // XAM's startup (0x816968A8) registers one function per driver event: a controller binding to a user, a device
    // configuration change, the Kinect mic array starting, the audio latency changing, an input device failing to
    // connect or bind. Kept for the drivers that will raise those events (none does yet).
    enum Callback { kUserBinding, kDeviceConfigChange, kMicArrayStart, kAudioLatency, kFailedConnectionOrBind, kCount };
    std::array<std::atomic<uint32_t>, kCount> g_callbacks{};

    void Register(Callback which, const char* name, uint32_t function)
    {
        g_callbacks[which] = function;
        Log("{}: callback {:08X}", name, function);
    }
}

namespace kernel
{
    uint32_t DriverCallback(int which)
    {
        return which >= 0 && which < kCount ? g_callbacks[which].load() : 0;
    }
}

GUEST_IMPORT(DrvSetUserBindingCallback)
{
    Register(kUserBinding, "DrvSetUserBindingCallback", ctx.r3.u32);
    kernel::SetXInputdBindingCallback(ctx.r3.u32); // the controller driver binds the pads (kernel/xboxkrnl_xinputd.cpp)
}
GUEST_IMPORT(DrvSetDeviceConfigChangeCallback) { Register(kDeviceConfigChange, "DrvSetDeviceConfigChangeCallback", ctx.r3.u32); }
GUEST_IMPORT(DrvSetMicArrayStartCallback) { Register(kMicArrayStart, "DrvSetMicArrayStartCallback", ctx.r3.u32); }
GUEST_IMPORT(DrvSetAudioLatencyCallback) { Register(kAudioLatency, "DrvSetAudioLatencyCallback", ctx.r3.u32); }
GUEST_IMPORT(XInputdSetFailedConnectionOrBindCallback) { Register(kFailedConnectionOrBind, "XInputdSetFailedConnectionOrBindCallback", ctx.r3.u32); }

// VOID KeSetPriorityClassThread(PKTHREAD Thread, ULONG PriorityClass): the scheduler's priority class of a thread (XAM
// raises its worker threads to class 1). The host scheduler gives every guest thread the same class.
GUEST_IMPORT(KeSetPriorityClassThread) {}

// NTSTATUS HalRegisterHdDvdRomNotification(PVOID Registration, BOOLEAN Register): for the HD DVD add-on drive's
// arrival; this console has none attached, so the notification never fires.
GUEST_IMPORT(HalRegisterHdDvdRomNotification) { ctx.r3.u64 = kernel::kStatusSuccess; }

// VOID XeKeysGetStatus(PULONG Status): the key vault's state. Bit 3 is a fault, despite the name of the XAM export
// that reads it (XamIsKeyVaultValid, 0x816E0D28): xam.xex's start-up takes its normal path, with the system UI, only
// when it is clear (0x81696558; set, it boots into an error path). The ports have no key vault faults: 0.
GUEST_IMPORT(XeKeysGetStatus)
{
    if (ctx.r3.u32)
        guest::g_memory.Write32(ctx.r3.u32, 0);
}

// NTSTATUS DumpRegisterDedicatedDataBlock(ULONG Id, PVOID Data, ULONG Size, ...): a block the kernel's crash dump
// writer saves (XAM registers its state for crash reports, 0x816A16F0). No crash dumps are written here.
GUEST_IMPORT(DumpRegisterDedicatedDataBlock) { ctx.r3.u64 = kernel::kStatusSuccess; }

// NTSTATUS DrvGetContentStorageNotification(PNOTIFICATION Notification): the next storage device arrival / removal
// (XAM's storage thread, 0x8167EB60, reads +0xC name, +0x10 1 arrival / 2 removal, +0x14 device type, until the call
// fails). None queued: the ports' storage devices are known to the runtime's own device layer (kernel/storage), not
// reported through this queue yet.
GUEST_IMPORT(DrvGetContentStorageNotification) { ctx.r3.u64 = 0x8000001A; } // STATUS_NO_MORE_ENTRIES

// VOID DrvSetContentStorageCallback(PVOID Callback, ...): called when a storage notification is queued.
GUEST_IMPORT(DrvSetContentStorageCallback) { Log("DrvSetContentStorageCallback: callback {:08X}", ctx.r3.u32); }

// NTSTATUS IoDismountVolumeByName(PANSI_STRING Name): XAM dismounts a removed device's volume; nothing is mounted
// through the guest's I/O manager here.
GUEST_IMPORT(IoDismountVolumeByName) { ctx.r3.u64 = kernel::kStatusSuccess; }

// VOID HalGetPowerUpCause(PHAL_POWER_UP_CAUSE Cause): why the console turned on, from the SMC (xam.xex keeps it at
// 0x81AA0B08 and reads byte +1, the SMC reason, and bit 0 of +4). Here always the power button (SMC reason 0x11).
GUEST_IMPORT(HalGetPowerUpCause)
{
    auto& mem = guest::g_memory;
    if (!ctx.r3.u32)
        return;
    for (uint32_t i = 0; i < 8; i++)
        mem.Write8(ctx.r3.u32 + i, 0);
    mem.Write8(ctx.r3.u32 + 1, 0x11);
}

// VOID XexShimLock(VOID): closes the kernel's title shim table to changes once XAM has set it up (startup,
// 0x816968A8). The runtime applies no title shims.
GUEST_IMPORT(XexShimLock) {}

// The network adapter (Nic*): kernel/xboxkrnl_nic.cpp.

// NTSTATUS HidReadKeys(DWORD User, PVOID Keys, PDWORD Count): the controller buttons held while the console starts
// (xam.xex 0x816BD290 checks them for its start-up key combinations). None held.
GUEST_IMPORT(HidReadKeys)
{
    if (ctx.r5.u32)
        guest::g_memory.Write32(ctx.r5.u32, 0);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// ---- Objects of types XAM defines itself (ObCreateObject with its own OBJECT_TYPE, e.g. 0x81A821E0) ----
namespace
{
    std::mutex g_typedMutex;
    std::map<uint32_t, uint32_t> g_objectTypes; // object -> its OBJECT_TYPE descriptor
    std::set<uint32_t> g_types;
}

namespace kernel
{
    // The OBJECT_TYPE an ObCreateObject object was made with (0 for the runtime's own objects), and whether a
    // descriptor is one such a type.
    uint32_t CreatedObjectType(uint32_t object)
    {
        std::lock_guard lock(g_typedMutex);
        auto it = g_objectTypes.find(object);
        return it == g_objectTypes.end() ? 0 : it->second;
    }

    bool IsCreatedObjectType(uint32_t type)
    {
        std::lock_guard lock(g_typedMutex);
        return g_types.contains(type);
    }
}

// NTSTATUS ObCreateObject(POBJECT_TYPE Type, POBJECT_ATTRIBUTES Attributes, ULONG BodySize, PVOID* Object): a zeroed,
// reference-counted object body of the type (the caller fills it, then ObInsertObject gives it a handle). The type's
// delete procedure is not called when the last reference goes (not seen needed yet).
GUEST_IMPORT(ObCreateObject)
{
    const uint32_t type = ctx.r3.u32, size = ctx.r5.u32, out = ctx.r6.u32;
    if (!out || size == 0)
    {
        ctx.r3.u64 = kernel::kStatusInvalidParameter;
        return;
    }
    const uint32_t object = kernel::CreateOwnedObject(size);
    kernel::ReferenceObject(object); // the creator's reference (owned objects start with none)
    {
        std::lock_guard lock(g_typedMutex);
        g_objectTypes[object] = type;
        g_types.insert(type);
    }
    kernel::SetObjectCleanup(object, [object] {
        std::lock_guard lock(g_typedMutex);
        g_objectTypes.erase(object);
    });
    guest::g_memory.Write32(out, object);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS ObInsertObject(PVOID Object, POBJECT_ATTRIBUTES Attributes, ULONG ObjectPointerBias, PHANDLE Handle): a
// handle to the object (named when the attributes name it). The creation reference passes to the handle.
GUEST_IMPORT(ObInsertObject)
{
    const uint32_t object = ctx.r3.u32, attributes = ctx.r4.u32, out = ctx.r6.u32;
    if (attributes)
        if (const std::string name = kernel::ReadObjectName(attributes); !name.empty())
            kernel::RegisterNamedObject(name, object);
    const uint32_t handle = kernel::CreateHandle(object);
    // The creation reference passes to the handle; ObjectPointerBias asks for that many more for the caller.
    for (uint32_t i = 0; i < ctx.r5.u32 && i < 16; i++)
        kernel::ReferenceObject(object);
    kernel::DereferenceObject(object);
    if (out)
        guest::g_memory.Write32(out, handle);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// VOID XAudioSetDuckerLevel(FLOAT Level): how far other sound drops while party chat talks (from XConfig 7/0x0A).
// The runtime's own voice path does no ducking yet; kept for it.
GUEST_IMPORT(XAudioSetDuckerLevel) { Log("XAudioSetDuckerLevel({})", ctx.f1.f64); }

// PVOID ObGetWaitableObject(PVOID Object): what a wait on the object waits on. For an object of a type XAM defined,
// its OBJECT_TYPE's DefaultObject (+0x14, after the Allocate / Free / Close / Delete / Parse procedures): an address in
// the kernel's space is a shared object to wait on, anything smaller the offset of the dispatcher object inside the
// body. The runtime's own objects are their own dispatcher objects (file objects included).
uint32_t kernel::WaitableObject(uint32_t object)
{
    if (const uint32_t type = CreatedObjectType(object))
    {
        const uint32_t defaultObject = guest::g_memory.Read32(type + 0x14);
        return defaultObject >= 0x80000000u ? defaultObject : object + defaultObject;
    }
    return object;
}

GUEST_IMPORT(ObGetWaitableObject) { ctx.r3.u64 = kernel::WaitableObject(ctx.r3.u32); }

// The XAudio ducker (party chat lowering other sound; xam.xex's audio queries it): level, threshold, attack, hold.
// The runtime's mixer does not duck yet; these report the console's no-ducking values.
GUEST_IMPORT(XAudioGetDuckerLevel) { ctx.f1.f64 = 1.0; }
GUEST_IMPORT(XAudioGetDuckerThreshold) { ctx.f1.f64 = 0.0; }
GUEST_IMPORT(XAudioGetDuckerAttackTime) { ctx.f1.f64 = 0.0; }
GUEST_IMPORT(XAudioGetDuckerHoldTime) { ctx.f1.f64 = 0.0; }

// HRESULT XAudioSetVoiceCategoryVolume(DWORD Category, FLOAT Volume): the console's per-category volumes (the
// runtime's native XAudioGetVoiceCategoryVolume reports full volume for all).
GUEST_IMPORT(XAudioSetVoiceCategoryVolume) { ctx.r3.u64 = 0; }

// ---- Kinect and cameras: the Kinect hub (Natal), its camera (Detroit) and mic array (MCA), the Xbox Live Vision
// camera (PsCam). None is attached: requests find no device. ----
constexpr uint32_t kStatusDeviceNotConnected = 0xC000009D;

// NTSTATUS UsbdNatalHubRegisterNotificationCallback(PVOID Callback, ...): told when the Kinect hub arrives; never.
GUEST_IMPORT(UsbdNatalHubRegisterNotificationCallback) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(DetroitDeviceRequest) { ctx.r3.u64 = kStatusDeviceNotConnected; }
GUEST_IMPORT(McaDeviceRequest) { ctx.r3.u64 = kStatusDeviceNotConnected; }
GUEST_IMPORT(PsCamDeviceRequest) { ctx.r3.u64 = kStatusDeviceNotConnected; }

// VOID XInputdSetMinMaxAuthDelay(ULONGLONG Min, ULONGLONG Max): how long a wireless controller's authentication may
// take (XConfig 10/3). The runtime's controllers need none.
GUEST_IMPORT(XInputdSetMinMaxAuthDelay) {}

// VOID HalRegisterSMCNotification(PVOID Registration, BOOLEAN Register): the SMC's events (the tray, the power and
// eject buttons) for XAM (0x816BB478). Kept for when the runtime raises them.
GUEST_IMPORT(HalRegisterSMCNotification) { Log("HalRegisterSMCNotification({:08X}, {})", ctx.r3.u32, ctx.r4.u32); }
GUEST_IMPORT(UsbdGetNatalHardwareVersion) { ctx.r3.u64 = kStatusDeviceNotConnected; }

// NTSTATUS HalReadArgonEeprom(DWORD Index, PVOID Buffer, DWORD Size, ...): the wireless controller radio (Argon)
// module's EEPROM (xam.xex reads 8 bytes of index 1 into its console description, 0x816B2900). Blank.
GUEST_IMPORT(HalReadArgonEeprom)
{
    auto& mem = guest::g_memory;
    for (uint32_t i = 0; ctx.r4.u32 && i < ctx.r5.u32 && i < 0x100; i++)
        mem.Write8(ctx.r4.u32 + i, 0);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// VOID XeKeysSetProtectedFlag(DWORD Flag, BOOL Value): a key vault protection flag the hypervisor keeps for the
// running title (xam.xex sets it per title, 0x8169ED78). Nothing here reads it.
GUEST_IMPORT(XeKeysSetProtectedFlag) {}

// NTSTATUS HalFinalizePowerLossRecovery(VOID): the end of recovery after a power loss (nothing to recover here).
GUEST_IMPORT(HalFinalizePowerLossRecovery) { ctx.r3.u64 = kernel::kStatusSuccess; }

// NTSTATUS XexVerifyImageHeaders(PVOID Header, DWORD Size, ...): the hypervisor's signature check of a module's
// headers. The ports run the user's own modules, which are not re-signed after their conversion; they verify.
GUEST_IMPORT(XexVerifyImageHeaders) { ctx.r3.u64 = kernel::kStatusSuccess; }

// VOID AniBlockOnAnimation(VOID): waits until the boot animation has played to its end (XAM's title start, 0x816A01A8,
// before the title's entry point). The native start-up calls the same wait itself.
GUEST_IMPORT(AniBlockOnAnimation) { kernel::BlockOnBootAnimation(); }

// VOID XAudioSetSpeakerConfig(DWORD Config, ...): the speaker configuration for the audio driver (XAM passes
// XGetAudioFlags() at each title start). The runtime's mixer reads XConfig itself.
GUEST_IMPORT(XAudioSetSpeakerConfig) { Log("XAudioSetSpeakerConfig({:08X})", ctx.r3.u32); }

// NTSTATUS XexSendDeferredNotifications(VOID): system notifications held while the title started go out now; the
// runtime holds none.
GUEST_IMPORT(XexSendDeferredNotifications) { ctx.r3.u64 = kernel::kStatusSuccess; }

// NTSTATUS SataCdRomRecordReset(...): the DVD drive's reset record (XAM's console description, 0x816B2A08).
GUEST_IMPORT(SataCdRomRecordReset) { ctx.r3.u64 = kernel::kStatusSuccess; }

// NTSTATUS XeKeysGetMediaID(PBYTE MediaId, ...): the inserted disc's media ID; there is no disc.
GUEST_IMPORT(XeKeysGetMediaID) { ctx.r3.u64 = 0xC0000013; } // STATUS_NO_MEDIA_IN_DEVICE

// NTSTATUS NicGetStats(HANDLE Nic, PVOID Stats, ULONG Size, ...): the adapter's counters; it has no link and has moved
// no frames.
GUEST_IMPORT(NicGetStats)
{
    auto& mem = guest::g_memory;
    for (uint32_t i = 0; ctx.r4.u32 && i < ctx.r5.u32 * 4 && i < 0x200; i++)
        mem.Write8(ctx.r4.u32 + i, 0);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS MtpdGetCurrentDevices(DWORD Max, PVOID Devices, PDWORD Count): portable media players attached over USB
// (MTP; xam.xex's media library, 0x8187B080). None.
GUEST_IMPORT(MtpdGetCurrentDevices)
{
    if (ctx.r5.u32)
        guest::g_memory.Write32(ctx.r5.u32, 0);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS XInputdGetLastTextInputTime(DWORD User, PULONGLONG Time, ...): when a keyboard last typed (xam.xex's idle
// timer, 0x816F2980, polls it). Never: no text input device.
GUEST_IMPORT(XInputdGetLastTextInputTime)
{
    if (ctx.r4.u32)
        guest::g_memory.Write64(ctx.r4.u32, 0);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// NTSTATUS HidGetLastInputTime(DWORD User, PULONGLONG Time): when a controller was last used (xam.xex's idle timer).
// The runtime's input layer keeps no such time yet: never.
GUEST_IMPORT(HidGetLastInputTime)
{
    if (ctx.r4.u32)
        guest::g_memory.Write64(ctx.r4.u32, 0);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// ---- Event tracing (Etx*): XAM's own logging consumers. Nothing is traced here; consumers find no logs. ----
GUEST_IMPORT(EtxConsumerRegister) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(EtxConsumerUnregister) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(EtxConsumerEnableEventType) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(EtxConsumerDisableEventType) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(EtxConsumerProcessLogs) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(EtxConsumerFlushBuffers) { ctx.r3.u64 = kernel::kStatusSuccess; }

// BOOL DumpGetRawDumpInfo(PVOID Info, ...): whether a crash dump from the previous boot is waiting (xam.xex's
// start-up shows a crash screen for one, 0x81696558). None.
GUEST_IMPORT(DumpGetRawDumpInfo) { ctx.r3.u64 = 0; }

// ULONG HalGetNotedArgonErrors(VOID): the wireless controller radio's errors (bits 0-2 make XAM report one). None.
GUEST_IMPORT(HalGetNotedArgonErrors) { ctx.r3.u64 = 0; }

// VOID HalRegisterXamPowerDownCallback(PVOID, ...) / HalRegisterBackgroundModeTransitionCallback(PVOID, ...): XAM's
// handlers for powering down and for the instant-on background mode; the runtime raises neither.
GUEST_IMPORT(HalRegisterXamPowerDownCallback) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(HalRegisterBackgroundModeTransitionCallback) { ctx.r3.u64 = kernel::kStatusSuccess; }

// BOOL XeKeysSecurityGetDetected(...) / XeKeysSecurityGetActivated(...): the hypervisor's tamper detections and the
// security measures it has activated (xam.xex reports them to LIVE, 0x816BAEA0). None.
GUEST_IMPORT(XeKeysSecurityGetDetected) { ctx.r3.u64 = 0; }
GUEST_IMPORT(XeKeysSecurityGetActivated) { ctx.r3.u64 = 0; }

// NTSTATUS VdRegisterXamGraphicsNotification(PVOID Routine, ...): XAM's handler for the title's graphics events
// (scaler and display mode changes; xam.xex's UI start-up, 0x816CE8B0). Kept: kernel::XamGraphicsNotification.
namespace
{
    std::atomic<uint32_t> g_graphicsNotification = 0;
}

uint32_t kernel::XamGraphicsNotification() { return g_graphicsNotification.load(); }

GUEST_IMPORT(VdRegisterXamGraphicsNotification)
{
    g_graphicsNotification = ctx.r3.u32;
    Log("VdRegisterXamGraphicsNotification({:08X})", ctx.r3.u32);
    ctx.r3.u64 = kernel::kStatusSuccess;
}

// The type procedures of an XAM-defined type (OBJECT_TYPE: +0 Allocate, +4 Free, +8 Close, +0xC Delete, +0x10
// Parse, +0x14 DefaultObject, +0x18 PoolTag): XAM's notification listeners unlink themselves from its broadcast list
// in theirs (a listener freed without it left the list pointing at freed memory). Called on the current guest
// thread, nested in the import that closed or released the object.
namespace
{
    void CallTypeProcedure(uint32_t object, uint32_t offset, uint32_t argument)
    {
        const uint32_t type = kernel::CreatedObjectType(object);
        if (!type)
            return;
        const uint32_t procedure = guest::g_memory.Read32(type + offset);
        kernel::GuestThread* thread = kernel::CurrentThread();
        if (!procedure || !thread || !thread->context)
            return;
        PPCContext call = *thread->context;
        call.r3.u64 = object;
        call.r4.u64 = argument;
        kernel::CallGuest(call, procedure);
    }
}

void kernel::ObjectHandleClosing(uint32_t object) { CallTypeProcedure(object, 0x8, 1); }
void kernel::ObjectDeleting(uint32_t object) { CallTypeProcedure(object, 0xC, 0); }

// NTSTATUS XInputdFFDeviceControl(HANDLE Device, DWORD Command, ...): force-feedback commands (XAM's rumble when the
// Guide opens, 0x816F2560). The runtime's controllers do not rumble yet.
GUEST_IMPORT(XInputdFFDeviceControl) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(XInputdFFSetRumble) { ctx.r3.u64 = kernel::kStatusSuccess; }
GUEST_IMPORT(XInputdFFSetDeviceGain) { ctx.r3.u64 = kernel::kStatusSuccess; }

// NTSTATUS ExExpansionCall(DWORD ExpansionId, DWORD Code, PVOID Data, ...): a call into a hypervisor expansion the
// kernel installed. XAM's 'PV03' (content licenses; xam.xex 0x8167F6E0 passes a package's 0x10-byte license entry
// with code 3 and reads it back, then checks the license type; 1 asks whether the expansion is there, 2 / 4 report a
// verdict and a header to it): the license comes back as it went in (INFER: what the expansion changes for a retail
// console was not traced; an unrestricted license, all ones, is what this runtime's packages carry).
GUEST_IMPORT(ExExpansionCall)
{
    if (ctx.r3.u32 != 0x50563033) // 'PV03'
        Fatal("ExExpansionCall({:08X}, {}): an expansion the runtime does not have", ctx.r3.u32, ctx.r4.u32);
    if (ctx.r4.u32 < 1 || ctx.r4.u32 > 4)
        Fatal("ExExpansionCall('PV03', {}): code not implemented", ctx.r4.u32);
    ctx.r3.u64 = kernel::kStatusSuccess;
}
