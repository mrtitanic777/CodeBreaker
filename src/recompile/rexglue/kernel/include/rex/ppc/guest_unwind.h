/**
 * @file        ppc/guest_unwind.h
 * @brief       Host-exception carrier for guest C++/SEH stack unwinding.
 *
 * When recompiled guest code invokes the kernel's RtlUnwind (to unwind the
 * stack to a catch/__except handler), we cannot walk native x64 frames the way
 * the console kernel walks the guest stack. Instead RtlUnwind THROWS this host
 * exception; it propagates up the native call stack (unwinding the recompiled
 * frames), and the recompiled function that owns the target catch handler
 * (identified by targetIp falling within its address range) catches it, restores
 * its frame, and resumes execution at targetIp. Functions in between simply let
 * it propagate. See generate_exception_handlers codegen.
 */
#pragma once

#include <cstdint>

namespace rex::ppc {

struct GuestUnwind {
  uint32_t targetFrame = 0;   // RtlUnwind TargetFrame (establisher frame)
  uint32_t targetIp = 0;      // RtlUnwind TargetIp (guest address to resume at)
  uint32_t returnValue = 0;   // RtlUnwind ReturnValue (placed in r3 on resume)
};

}  // namespace rex::ppc
