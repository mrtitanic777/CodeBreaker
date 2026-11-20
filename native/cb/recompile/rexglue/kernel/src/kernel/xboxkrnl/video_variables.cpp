// video_variables.cpp -- CodeBreaker's: the part of the kernel's video exports the kernel module
// needs when xboxkrnl_video.cpp is left out of the build (it brings the GPU and UI layers; a
// recompiled project routes the Vd* functions to its own GPU subsystem instead). The video
// variables a title imports, as xboxkrnl_video.cpp creates them, the video mode, the Vd*
// functions that only answer the title (taken from xboxkrnl_video.cpp), and the vertical-blank
// interrupt: a host thread that calls the title's graphics interrupt callback 60 times a second,
// as GraphicsSystem's vsync worker does.
#include <atomic>
#include <chrono>
#include <cstring>

#include <rex/kernel/xboxkrnl/rtl.h>
#include <rex/kernel/xboxkrnl/video.h>
#include <rex/memory.h>
#include <rex/ppc/function.h>
#include <rex/ppc/types.h>
#include <rex/system/export_resolver.h>
#include <rex/system/function_dispatcher.h>
#include <rex/system/kernel_state.h>
#include <rex/system/xthread.h>
#include <rex/system/xtypes.h>
#include <rex/thread.h>

namespace rex::kernel::xboxkrnl {

using system::KernelState;

void VdQueryVideoMode(system::X_VIDEO_MODE* video_mode) {
  std::memset(video_mode, 0, sizeof(system::X_VIDEO_MODE));
  video_mode->display_width = 1280;               // 720p, 60 Hz, progressive, widescreen, NTSC
  video_mode->display_height = 720;
  video_mode->is_interlaced = 0;
  video_mode->is_widescreen = 1;
  video_mode->is_hi_def = 1;
  video_mode->refresh_rate = 60.0f;
  video_mode->video_standard = 1;
  video_mode->unknown_0x8a = 0x4A;
  video_mode->unknown_0x01 = 0x01;
}

void RegisterVideoExports(rex::runtime::ExportResolver* export_resolver, KernelState* kernel_state) {
  auto memory = kernel_state->memory();
  // VdGlobalDevice, VdGlobalXamDevice: pointers to D3D devices a title sets; read as 0
  uint32_t pVdGlobalDevice = memory->SystemHeapAlloc(4, 32, memory::kSystemHeapPhysical);
  export_resolver->SetVariableMapping("xboxkrnl.exe", 0x01BE, pVdGlobalDevice);
  memory::store_and_swap<uint32_t>(memory->TranslateVirtual(pVdGlobalDevice), 0);
  uint32_t pVdGlobalXamDevice = memory->SystemHeapAlloc(4, 32, memory::kSystemHeapPhysical);
  export_resolver->SetVariableMapping("xboxkrnl.exe", 0x01BF, pVdGlobalXamDevice);
  memory::store_and_swap<uint32_t>(memory->TranslateVirtual(pVdGlobalXamDevice), 0);
  // VdGpuClockInMHz: Xenos runs at 500 MHz
  uint32_t pVdGpuClockInMHz = memory->SystemHeapAlloc(4, 32, memory::kSystemHeapPhysical);
  export_resolver->SetVariableMapping("xboxkrnl.exe", 0x01C0, pVdGpuClockInMHz);
  memory::store_and_swap<uint32_t>(memory->TranslateVirtual(pVdGpuClockInMHz), 500);
  // VdHSIOCalibrationLock: a critical section
  uint32_t pVdHSIOCalibrationLock = memory->SystemHeapAlloc(28, 32, memory::kSystemHeapPhysical);
  export_resolver->SetVariableMapping("xboxkrnl.exe", 0x01C1, pVdHSIOCalibrationLock);
  auto hsio_lock = memory->TranslateVirtual<X_RTL_CRITICAL_SECTION*>(pVdHSIOCalibrationLock);
  xeRtlInitializeCriticalSectionAndSpinCount(hsio_lock, pVdHSIOCalibrationLock, 10000);
}

void VdQueryVideoMode_entry(ppc_ptr_t<system::X_VIDEO_MODE> video_mode) {
  VdQueryVideoMode(video_mode);
}

void VdGetCurrentDisplayGamma_entry(ppc_pu32_t type_ptr, ppc_pf32_t power_ptr) {
  *type_ptr = 2;                                  // TV (BT.709)
  *power_ptr = 2.22222233f;
}

ppc_u32_result_t VdInitializeEngines_entry(ppc_unknown_t unk0, ppc_fn_t callback, ppc_pvoid_t arg,
                                           ppc_pu32_t pfp_ptr, ppc_pu32_t me_ptr) {
  return 1;
}

void VdSetSystemCommandBufferGpuIdentifierAddress_entry(ppc_pvoid_t unk) {}

ppc_u32_result_t VdCallGraphicsNotificationRoutines_entry(ppc_unknown_t unk0, ppc_pvoid_t args) {
  return 0;
}

ppc_u32_result_t VdRetrainEDRAMWorker_entry(ppc_unknown_t unk0) { return 0; }

ppc_u32_result_t VdRetrainEDRAM_entry(ppc_unknown_t unk0, ppc_unknown_t unk1, ppc_unknown_t unk2,
                                      ppc_unknown_t unk3, ppc_unknown_t unk4, ppc_unknown_t unk5) {
  return 0;
}

// The graphics interrupt: the title's callback gets (source, user data); source 0 is the
// vertical blank.
static std::atomic<uint32_t> interrupt_callback{0};
static std::atomic<uint32_t> interrupt_callback_data{0};
static system::object_ref<system::XHostThread> vsync_thread;

static void StartVsync(KernelState* kernel_state) {
  vsync_thread = system::object_ref<system::XHostThread>(
      new system::XHostThread(kernel_state, 128 * 1024, 0, [kernel_state]() {
        auto next = std::chrono::steady_clock::now();
        for (;;) {
          next += std::chrono::microseconds(16667);
          std::this_thread::sleep_until(next);
          uint32_t callback = interrupt_callback.load();
          if (!callback) {
            continue;
          }
          auto thread = system::XThread::GetCurrentThread();
          thread->SetActiveCpu(2);
          uint64_t args[] = {0, interrupt_callback_data.load()};
          kernel_state->function_dispatcher()->ExecuteInterrupt(thread->thread_state(), callback,
                                                                args, 2);
        }
        return 0;
      }));
  vsync_thread->set_name("GPU VSync");
  vsync_thread->Create();
}

void VdSetGraphicsInterruptCallback_entry(ppc_fn_t callback, ppc_pvoid_t user_data) {
  interrupt_callback_data = user_data.guest_address();
  interrupt_callback = callback.value();
  if (!vsync_thread) {
    StartVsync(REX_KERNEL_STATE());
  }
}

}  // namespace rex::kernel::xboxkrnl

XBOXKRNL_EXPORT(__imp__VdQueryVideoMode, rex::kernel::xboxkrnl::VdQueryVideoMode_entry)
XBOXKRNL_EXPORT(__imp__VdGetCurrentDisplayGamma,
                rex::kernel::xboxkrnl::VdGetCurrentDisplayGamma_entry)
XBOXKRNL_EXPORT(__imp__VdInitializeEngines, rex::kernel::xboxkrnl::VdInitializeEngines_entry)
XBOXKRNL_EXPORT(__imp__VdSetSystemCommandBufferGpuIdentifierAddress,
                rex::kernel::xboxkrnl::VdSetSystemCommandBufferGpuIdentifierAddress_entry)
XBOXKRNL_EXPORT(__imp__VdCallGraphicsNotificationRoutines,
                rex::kernel::xboxkrnl::VdCallGraphicsNotificationRoutines_entry)
XBOXKRNL_EXPORT(__imp__VdRetrainEDRAMWorker, rex::kernel::xboxkrnl::VdRetrainEDRAMWorker_entry)
XBOXKRNL_EXPORT(__imp__VdRetrainEDRAM, rex::kernel::xboxkrnl::VdRetrainEDRAM_entry)
XBOXKRNL_EXPORT(__imp__VdSetGraphicsInterruptCallback,
                rex::kernel::xboxkrnl::VdSetGraphicsInterruptCallback_entry)
