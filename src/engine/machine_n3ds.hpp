// engine/machine_n3ds.hpp -- the Nintendo 3DS lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_n3ds_lane();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A 3DS homebrew `.3dsx` or its ARM ELF (formats/n3ds) is written back as GNU as source (source/console_writer: config/platforms/n3ds.json) and rebuilt; one that comes back identical is
// the original, an edited one has each function run beside the original's in CodeBreaker's ARM interpreter (emulate/arm, `System::N3ds`: the ARM11's
// ARMv6K integer set; the VFP and Thumb-2 are decoded and written but not run). The memory is the process's as the loader leaves it: the segments at
// $00100000 (the absolute relocations applied), a heap at $08000000, the stack, and, as logged devices, the I/O and GPU register windows ($1EC00000), video
// memory ($1F000000) and the kernel's configuration, shared and thread-local pages ($1FF80000). The thread-local storage pointer (CP15 c13) is $1FF82000.
// A software interrupt is a Horizon kernel call: logged by its name and R0-R3 (`svcSendSyncRequest`), answered with 0, and the ones that end the program
// (ExitProcess, ExitThread, Break) end the run. The comparison is of how a function ended, R0 R1 R4-R11 SP, the memory it changed, every register access
// and the kernel calls.
#pragma once

#include <filesystem>

#include "emulate/arm.hpp"
#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec n3ds_lane_spec();

// The lane's machine for a program and a state: the bus with the program loaded, the hardware windows and the work memory seeded, and the run's options (the
// registers of the state, the stack, the kernel calls that end it) and the address the function is at. Used by the lane's comparison and by the tools that trace one.
void n3ds_prepare(const std::filesystem::path& path, std::uint32_t entry, const verify::lanes::State& state, emulate::arm::Bus& bus, emulate::arm::RunOptions& options,
                  std::uint32_t& address);

// Add "n3ds" to the machine lane table. Safe to call twice.
void register_n3ds_lane();

}  // namespace cb::engine
