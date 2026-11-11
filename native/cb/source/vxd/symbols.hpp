// cb/source/vxd/symbols.hpp -- recover VxD function and handler names from the Win95 DDK.
//
// Two DDK sources name things the raw disassembly can only call sub_XXXX:
//
//   * control_messages()  -- the system control messages (SYS_CRITICAL_INIT=0, DEVICE_INIT=1, ..., from VMM.INC), so a
//     control-dispatch handler can be named after the message it services.
//   * sym_names()         -- the DDK debug .SYM files (DEBUG/<DEV>.SYM, MAPSYM output) carry the REAL internal function
//     names (VKD_Filter_Keyboard_Input, VKD_Put_Byte, ...) for 12 core VxDs. Record format:
//     <u16 offset><u8 namelen><name>. NOTE these are the DEBUG build's offsets; the retail oracle differs, so a caller
//     applies a name only where the offset lands on a real function start.
//
// The service names live in services.hpp (the exported interface); this adds the control handlers and the internal
// functions.
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/source/vxd/ddk.hpp"

namespace cb::source::vxd {

// {message number: MESSAGE_NAME} of the system control messages declared in VMM.INC's text: the `NAME EQU nnH` lines
// from SYS_CRITICAL_INIT to SYSTEM_EXIT2 whose value is below 40h (flag, bit, id and DIOC constants left out). The
// first name of a number wins.
std::map<std::uint32_t, std::string> parse_control_messages(std::string_view vmm_inc_text);

// The same, from the DDK's VMM.INC; empty when there is none.
std::map<std::uint32_t, std::string> control_messages(const DdkFolders& ddk);

// {offset: name} from the text of a .SYM file, in the order the file gives them: the names of 3 to 40 letters, digits
// and underscores, the first name at an offset winning; segment and group markers (one leading underscore and no
// other) left out.
OrderedMap<std::uint16_t, std::string> parse_sym_names(ByteView sym_file);

// {offset: name} from DEBUG/<device>.SYM (the file's name matched without regard to case), or empty if there is no
// symbol file for the device.
OrderedMap<std::uint16_t, std::string> sym_names(const DdkFolders& ddk, std::string_view device);

}  // namespace cb::source::vxd
