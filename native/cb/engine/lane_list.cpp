// cb/engine/lane_list.cpp -- THE list of platform lanes: one line to add a lane (its header above, its call below).
//
// Each lane defines its registration object in its own file (engine/machine_<name>.cpp: `register_<name>_lane()` calls `register_lane({...})`, see
// lane_registry.hpp) and is named here once. The order of the calls is the order the platforms are tried in when a file is identified (the first
// platform whose probe takes it is its platform) and the order the lane kit asks the lanes' probes; put a lane after the ones whose files it must
// not take. This list is explicit on purpose: a lane that registered itself from a static initialiser would be dropped, with its object file, by
// the linker of a static library that nothing else refers to.
// cb-unity: alone
#include "cb/engine/lane_registry.hpp"

#include <mutex>

#include "cb/engine/machine_atari2600.hpp"
#include "cb/engine/machine_atari5200.hpp"
#include "cb/engine/machine_atari7800.hpp"
#include "cb/engine/machine_builtin.hpp"
#include "cb/engine/machine_cdi.hpp"
#include "cb/engine/machine_chf.hpp"
#include "cb/engine/machine_dreamcast.hpp"
#include "cb/engine/machine_gekko.hpp"
#include "cb/engine/machine_intv.hpp"
#include "cb/engine/machine_jaguar.hpp"
#include "cb/engine/machine_lynx.hpp"
#include "cb/engine/machine_m68k.hpp"
#include "cb/engine/machine_n3ds.hpp"
#include "cb/engine/machine_n64.hpp"
#include "cb/engine/machine_nds.hpp"
#include "cb/engine/machine_ngp.hpp"
#include "cb/engine/machine_o2.hpp"
#include "cb/engine/machine_pce.hpp"
#include "cb/engine/machine_ps2.hpp"
#include "cb/engine/machine_ps3.hpp"
#include "cb/engine/machine_psp.hpp"
#include "cb/engine/machine_sh2.hpp"
#include "cb/engine/machine_snes.hpp"
#include "cb/engine/machine_switch.hpp"
#include "cb/engine/machine_threedo.hpp"
#include "cb/engine/machine_vb.hpp"
#include "cb/engine/machine_vectrex.hpp"
#include "cb/engine/machine_vita.hpp"
#include "cb/engine/machine_wonderswan.hpp"
#include "cb/engine/machine_x64.hpp"

namespace cb::engine {

void register_all_lanes() {
    static std::once_flag once;
    std::call_once(once, [] {
        register_nes_lane();         // (the lane kit's own machines: engine/machine_builtin.cpp)
        register_sms_lane();
        register_gb_lane();
        register_genesis_lane();
        register_ps1_lane();
        register_gba_lane();
        register_nds_lane();
        register_n64_lane();
        register_zx_lane();
        register_msx_lane();
        register_coleco_lane();
        register_cpc_lane();
        register_atari5200_lane();
        register_atari2600_lane();
        register_atari7800_lane();
        register_lynx_lane();
        register_pce_lane();
        register_snes_lane();
        register_m68k_lanes();       // neogeo, segacd
        register_sh2_lanes();        // sega32x, saturn
        register_dreamcast_lane();
        register_threedo_lane();
        register_jaguar_lane();
        register_wonderswan_lane();
        register_cdi_lane();
        register_vectrex_lane();
        register_gekko_lanes();      // gamecube, wii, wiiu
        register_ps2_lane();
        register_psp_lane();
        register_n3ds_lane();
        register_vita_lane();
        register_ps3_lane();
        register_switch_lane();
        register_vb_lane();
        register_intv_lane();
        register_o2_lane();
        register_chf_lane();
        register_ngp_lane();
        register_ps4_lane();
        register_ps5_lane();
        register_xboxone_lane();
    });
}

}  // namespace cb::engine
