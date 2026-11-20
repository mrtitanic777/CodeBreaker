// What a title's program answers where the console's system shell would: the dashboard's own parts (its online
// home page and spotlight pictures) and the console's recompiled XAM are not linked into a title's program, so XAM's
// exports are the runtime's own (kernel/xam_*.cpp) and the dashboard's content is absent.

#include "kernel/kernel.h"

namespace kernel
{
    bool RecompiledXam()
    {
        return false;
    }

    PPCFunc* XamForward(const char*)
    {
        return nullptr;
    }

    std::optional<std::string> EpixOnlineManifest(std::string& error)
    {
        error = "the dashboard's home page is not part of a title's program";
        return std::nullopt;
    }

    std::optional<std::vector<uint8_t>> SpotlightPicture(const std::string&)
    {
        return std::nullopt;
    }
}
