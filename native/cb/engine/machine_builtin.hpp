// cb/engine/machine_builtin.hpp -- the registrations of the lane kit's own machines (the NES, Master System, Game Boy, Genesis, PlayStation, Game Boy Advance,
// ZX Spectrum, MSX, ColecoVision and Amstrad CPC): the platform, the probe, the text `info` prints and the extensions `reach` takes (engine/lane_registry.hpp).
// Their lanes themselves are made by the lane kit (lane_kit_machines.cpp), which is why a registration here has no `make_spec`.
#pragma once

namespace cb::engine {

void register_nes_lane();
void register_sms_lane();
void register_gb_lane();
void register_genesis_lane();
void register_ps1_lane();
void register_gba_lane();
void register_zx_lane();
void register_msx_lane();
void register_coleco_lane();
void register_cpc_lane();

}  // namespace cb::engine
