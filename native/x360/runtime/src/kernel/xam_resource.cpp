// XAM's resource locators (XamBuild*ResourceLocator): XUI URLs naming a resource inside a module's section, which
// XUI opens through XexGetModuleSection (xboxkrnl_modules.cpp serves xam.xex's sections from the user's own xam.xex).
//
// XAM 17559: XamBuildResourceLocator (0x816C8A48) writes "section://%X,%s#%s" (module handle, section, resource) or,
// for module 0, "file://media:/%s.xzp#%s"; it clears the buffer first, a NULL resource is "", and it returns E_FAIL
// when the result is empty. The shared-system (0x816C8BF0), gamercard and XAM variants use XAM's own module handle
// with the sections "shrdres", "gamercrd" and "xam".

#include "kernel/kernel.h"
#include "pool.h"
#include "xex.h"
#include "xui/package.h"

#include <array>
#include <cstring>
#include <format>
#include <mutex>

namespace
{
    constexpr uint32_t kEFail = 0x80004005;

    std::string Narrow(const std::u16string& text)
    {
        std::string out;
        for (char16_t c : text)
        {
            if (c > 0x7F)
                Fatal("Resource locator text has a non-ASCII character {:04X}", uint32_t(c));
            out += char(c);
        }
        return out;
    }

    std::string WideArgument(uint32_t address)
    {
        return address ? Narrow(kernel::ReadGuestWideString(address, 0x400)) : std::string();
    }

    // _snwprintf into the caller's buffer of cch characters. XAM leaves a truncated locator unterminated; no caller
    // should hit that, so it stops here.
    uint32_t WriteLocator(uint32_t buffer, uint32_t cch, const std::string& text)
    {
        auto& mem = guest::g_memory;
        if (buffer == 0 || cch == 0)
            Fatal("XamBuild*ResourceLocator: no output buffer ({:08X}, {} characters)", buffer, cch);
        if (text.size() >= cch)
            Fatal("XamBuild*ResourceLocator: \"{}\" does not fit {} characters", text, cch);
        for (size_t i = 0; i < text.size(); i++)
            mem.Write16(buffer + uint32_t(i) * 2, uint8_t(text[i]));
        mem.Write16(buffer + uint32_t(text.size()) * 2, 0);
        return text.empty() ? kEFail : 0;
    }

    std::string Locator(uint32_t module, const std::string& section, const std::string& resource)
    {
        return module ? std::format("section://{:X},{}#{}", module, section, resource) :
            std::format("file://media:/{}.xzp#{}", section, resource);
    }

    uint32_t XamModule()
    {
        return kernel::SystemModuleHandle("xam.xex");
    }
}

// HRESULT XamBuildResourceLocator(HMODULE Module, LPCWSTR Section, LPCWSTR Resource, LPWSTR Buffer, DWORD cch)
GUEST_IMPORT(XamBuildResourceLocator)
{
    ctx.r3.u64 = WriteLocator(ctx.r6.u32, ctx.r7.u32,
        Locator(ctx.r3.u32, WideArgument(ctx.r4.u32), WideArgument(ctx.r5.u32)));
}

// HRESULT XamBuildSharedSystemResourceLocator(LPCWSTR Resource, LPWSTR Buffer, DWORD cch): XAM's "shrdres" section.
GUEST_IMPORT(XamBuildSharedSystemResourceLocator)
{
    ctx.r3.u64 = WriteLocator(ctx.r4.u32, ctx.r5.u32, Locator(XamModule(), "shrdres", WideArgument(ctx.r3.u32)));
}

// HRESULT XamBuildGamercardResourceLocator(LPCWSTR Resource, LPWSTR Buffer, DWORD cch): XAM's "gamercrd" section.
GUEST_IMPORT(XamBuildGamercardResourceLocator)
{
    ctx.r3.u64 = WriteLocator(ctx.r4.u32, ctx.r5.u32, Locator(XamModule(), "gamercrd", WideArgument(ctx.r3.u32)));
}

// HRESULT XamBuildDynamicResourceLocator(DWORD Index, HMODULE Module, LPCWSTR Section, LPCWSTR Resource,
//                                        LPWSTR Buffer, DWORD cch): Index -1 names the module by its handle, any
// other index a slot of XUI's dynamic module table ("section://@%X,...").
GUEST_IMPORT(XamBuildDynamicResourceLocator)
{
    const uint32_t index = ctx.r3.u32, module = ctx.r4.u32;
    const std::string section = WideArgument(ctx.r5.u32), resource = WideArgument(ctx.r6.u32);
    std::string text;
    if (module == 0 || index == 0xFFFFFFFF)
        text = Locator(module, section, resource);
    else
        text = std::format("section://@{:X},{}#{}", index, section, resource);
    ctx.r3.u64 = WriteLocator(ctx.r7.u32, ctx.r8.u32, text);
}

// HRESULT XamGetDefaultImage(DWORD Index, PBYTE* Image, PDWORD Size) (XAM 0x816C8E10): images XAM loads at start-up
// (0x816C8D68, table 0x81A82714: section, resource, buffer, size) from its own XUI sections: 0 "xam"
// defaultsystemimage.png, 1 "xam" dashicon.png, 2 "shrdres" ico_64x_licensestore.png. Index > 2 is E_INVALIDARG; an
// image that did not load, E_UNEXPECTED. Here each is read from the user's xam.xex on first use into kernel memory.
GUEST_IMPORT(XamGetDefaultImage)
{
    struct Image
    {
        const char* section;
        const char* resource;
        uint32_t address = 0;
        uint32_t size = 0;
        bool loaded = false;
    };
    static std::mutex mutex;
    static std::array<Image, 3> images{ { { "xam", "defaultsystemimage.png" }, { "xam", "dashicon.png" },
                                          { "shrdres", "ico_64x_licensestore.png" } } };
    auto& mem = guest::g_memory;
    const uint32_t index = ctx.r3.u32, imagePtr = ctx.r4.u32, sizePtr = ctx.r5.u32;
    if (index > 2)
    {
        ctx.r3.u64 = 0x80070057;
        return;
    }
    std::lock_guard lock(mutex);
    Image& image = images[index];
    if (!image.loaded)
    {
        image.loaded = true;
        const xex::HostImage& xam = kernel::XamImage();
        const auto section = xam.resources.find(image.section);
        if (section == xam.resources.end())
            Fatal("XamGetDefaultImage({}): xam.xex has no section \"{}\"", index, image.section);
        const uint8_t* bytes = xam.At(section->second.first, section->second.second);
        if (!bytes)
            Fatal("XamGetDefaultImage({}): section \"{}\" is outside xam.xex's image", index, image.section);
        xui::Package package;
        std::string error;
        if (!package.Load(std::vector<uint8_t>(bytes, bytes + section->second.second), error))
            Fatal("XamGetDefaultImage({}): section \"{}\": {}", index, image.section, error);
        if (const xui::Package::Entry* entry = package.Find(image.resource))
        {
            image.address = KernelPool().Allocate(uint32_t(entry->size));
            if (image.address == 0)
                Fatal("XamGetDefaultImage({}): kernel pool exhausted", index);
            std::memcpy(mem.Translate(image.address), entry->data, entry->size);
            image.size = uint32_t(entry->size);
        }
    }
    if (image.address == 0)
    {
        ctx.r3.u64 = 0x8000FFFF;
        return;
    }
    mem.Write32(imagePtr, image.address);
    if (sizePtr)
        mem.Write32(sizePtr, image.size);
    ctx.r3.u64 = 0;
}

// HRESULT XamGetDefaultSystemImage(PBYTE* Image, PDWORD Size) (XAM 0x816C8E70): XamGetDefaultImage(0, Image, Size),
// defaultsystemimage.png (the 6719 dash's default pictures).
GUEST_IMPORT(XamGetDefaultSystemImage)
{
    ctx.r5.u64 = ctx.r4.u32;
    ctx.r4.u64 = ctx.r3.u32;
    ctx.r3.u64 = 0;
    XamGetDefaultImage_impl(ctx, base);
}
