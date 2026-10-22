#pragma once

// Rich presence as Xbox LIVE shows it to friends ("Playing Alone / Riding a pig"), formatted from the title's own
// calls as LIVE did: XUserSetContext(X_CONTEXT_PRESENCE 0x00008001, mode) picks a presence mode of the title's XLAST,
// whose string may name contexts and properties the title sets with XUserSetContext / XUserSetPropertyEx:
//   {c<context id, decimal>}   the string of that context's current value (XLAST ContextValue stringId)
//   {p0x<property id, hex>}    the property's value as text
// The XLAST is the SPA's metadata entry 'XSRC' (section 1, id 0x58535243): {'XSRC', u32 version, u32 size, u32 name
// length, name, u32 size, u32 compressed size} then a gzip stream of the UTF-16LE XLAST XML. The XML gives the mode
// and context-value string ids (Presence/PresenceMode @contextValue @stringId, Contexts/Context @id /ContextValue
// @value @stringId); the strings themselves come from the SPA's XSTR tables in the console's language (the caller's
// `string`). (Format checked on Minecraft TU46's SPA 584111F7 and the dashboard's FFFE07D1; Xenia Canary reads the
// same XML: util/xlast.cc, util/presence_string_builder.cc.)
//
// Needs native/x360/system/xui (xui/inflate.h) in the same target.

#include <cstdint>
#include <functional>
#include <map>
#include <string>

namespace live::presence
{
    constexpr uint32_t kContextPresence = 0x00008001; // X_CONTEXT_PRESENCE

    struct Definitions
    {
        std::map<uint32_t, uint32_t> modes;                             // presence mode -> string id
        std::map<uint32_t, std::map<uint32_t, uint32_t>> contextValues; // context id -> value -> string id
    };

    // Reads the XSRC entry's XLAST. Returns false (with `error`) when it cannot be read.
    bool LoadXsrc(const uint8_t* xsrc, size_t size, Definitions& out, std::string& error);

    // The presence text for the current values (`contexts` must hold X_CONTEXT_PRESENCE; empty text without it or for
    // a mode the title does not define). `string` gives a string id's text (UTF-8), "" if absent.
    std::string Format(const Definitions& definitions, const std::map<uint32_t, uint32_t>& contexts,
                       const std::map<uint32_t, std::string>& properties,
                       const std::function<std::string(uint32_t)>& string);

    // CB_PRESENCE_TEST=<SPA file>: prints every presence mode of the SPA formatted with context values 0..2 (English
    // strings). Returns the number of failures.
    int RunTest(const std::string& spaPath);
}
