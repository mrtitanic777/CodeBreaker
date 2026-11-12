// XAM content (XContent): storage devices and content packages (saved games, DLC) mounted under a root name.
//
// The console has one storage device, the hard drive (device ID 1, type HDD), backed by <data directory>/content in
// Xenia's layout, so save folders move between the two unchanged:
//   content/<XUID>/<TitleID>/<ContentType>/<FileName>/            the package's files
//   content/<XUID>/<TitleID>/Headers/<ContentType>/<FileName>.header
// XUID 0 holds common content (user index NONE/ANY, and all marketplace content). The .header file is the package's
// STFS header and metadata exactly as a console package stores them (magic, licenses at 0x22C, header size 0x340,
// metadata from 0x344: content type, title ID at 0x360, creator XUID at 0x371, display names at 0x411, attribute
// flags at 0x1711, thumbnail size/data at 0x1712/0x171A, title thumbnail at 0x571A), followed by Xenia's 42-byte
// file-name field at 0x971A, padded to 0xA000 bytes. File names that Windows cannot store are %XX-escaped.
//
// Structures (XDK xbox.h): XCONTENT_DATA (0x134): DeviceID, dwContentType, szDisplayName WCHAR[128] +8, szFileName
// CHAR[42] +0x108. XDEVICE_DATA (0x50): DeviceID, DeviceType, ulDeviceBytes +8, ulDeviceFreeBytes +0x10,
// wszFriendlyName WCHAR[27] +0x18. Behaviour follows the XDK documentation where it is specific (see
// analysis/xcontent_spec.md); where it is silent, Xenia's codes are used and marked.
//
// Packages stored as STFS container files (real console DLC) are not readable yet and stop with a message. The
// dump's DLC (res/DLC) is not XContent: the title reads it directly from UPDATE:.

#include "kernel/enumerator.h"
#include "kernel/game_import.h"
#include "kernel/objects.h"
#include "kernel/overlapped.h"
#include "kernel/profile_import.h"
#include "kernel/profile_location.h"
#include "kernel/spa.h"
#include "kernel/stfs_mount.h"
#include "kernel/storage.h"
#include "kernel/users.h"
#include "settings.h"
#include "vfs.h"
#include "xui/system_ui.h"

#include <array>
#include <windows.h>

#include <algorithm>
#include <atomic>
#include <charconv>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <map>
#include <mutex>
#include <optional>

namespace fs = std::filesystem;

namespace
{
    constexpr uint32_t kHardDriveId = kernel::storage::kHardDriveId;
    constexpr uint32_t kContentDataSize = 0x134;
    constexpr uint32_t kTypeMarketplace = 2;
    constexpr uint32_t kTypeProfile = 0x10000;

    constexpr uint32_t kErrorFileNotFound = 2;
    constexpr uint32_t kErrorPathNotFound = 3;
    constexpr uint32_t kErrorAccessDenied = 5;
    constexpr uint32_t kErrorInvalidParameter = 0x57;
    constexpr uint32_t kErrorInsufficientBuffer = 0x7A;
    constexpr uint32_t kErrorInvalidName = 0x7B;
    constexpr uint32_t kErrorAlreadyExists = 0xB7;
    constexpr uint32_t kErrorDeviceNotConnected = 0x48F;
    constexpr uint32_t kErrorNoSuchUser = 0x525;
    constexpr uint32_t kErrorFileCorrupt = 0x570;
    constexpr uint32_t kHresultInvalidArg = 0x80070057;

    // XCONTENTFLAG_*
    constexpr uint32_t kCreateNew = 1, kCreateAlways = 2, kOpenExisting = 3, kOpenAlways = 4, kTruncateExisting = 5;
    constexpr uint32_t kNoProfileTransfer = 0x10;
    constexpr uint32_t kForceShowUi = 0x200;
    constexpr uint32_t kEnumExcludeCommon = 0x1000;
    // XCONTENT_*_DISPOSITION
    constexpr uint32_t kCreatedNew = 1, kOpenedExisting = 2;

    // Package header (file offsets).
    constexpr size_t kHeaderFileSize = 0xA000;
    constexpr uint32_t kMagicCon = 0x434F4E20; // "CON "
    constexpr uint32_t kMagicLive = 0x4C495645, kMagicPirs = 0x50495253; // "LIVE", "PIRS"
    constexpr size_t kLicenses = 0x22C, kLicenseCount = 16, kHeaderSizeField = 0x340;
    constexpr size_t kMetadata = 0x344, kMetadataSize = 0x93D6;
    constexpr size_t kContentType = kMetadata + 0x00, kMetadataVersion = kMetadata + 0x04;
    constexpr size_t kContentSize = kMetadata + 0x08, kTitleIdField = kMetadata + 0x1C;
    constexpr size_t kProfileId = kMetadata + 0x2D, kDataFileCount = kMetadata + 0x59, kVolumeType = kMetadata + 0x65;
    constexpr size_t kDisplayNames = kMetadata + 0xCD, kDisplayNamesEx = kMetadata + 0x50D6;
    constexpr size_t kFlags = kMetadata + 0x13CD;
    constexpr size_t kThumbnailSize = kMetadata + 0x13CE, kTitleThumbnailSize = kMetadata + 0x13D2;
    constexpr size_t kThumbnail = kMetadata + 0x13D6, kTitleThumbnail = kMetadata + 0x53D6;
    constexpr size_t kFileNameField = kMetadata + kMetadataSize, kFileNameLength = 42;
    constexpr uint32_t kThumbnailMax = 0x3D00;
    // Attribute flags byte: bit 0 is kept set for packages created with XCONTENTFLAG_NOPROFILE_TRANSFER.
    constexpr uint8_t kAttributeNoProfileTransfer = 0x01;

    // Completes a synchronous or overlapped request with a Win32 result.
    uint32_t Finish(uint32_t overlapped, uint32_t error, uint32_t information = 0)
    {
        if (!overlapped)
            return error;
        kernel::CompleteOverlappedWin32(overlapped, error, information);
        return kernel::kErrorIoPending;
    }

    fs::path ContentRoot()
    {
        return DataDirectory() / "content";
    }

    // The content tree of a device (kernel/storage.h): content/ for the hard drive, <content root>\Content on the
    // others (a console stick's own tree). Empty: no such device.
    fs::path DeviceRoot(uint32_t device)
    {
        if (device == 0 || device == kHardDriveId)
            return ContentRoot();
        const auto dev = kernel::storage::Find(device);
        return dev ? dev->contentRoot / "Content" : fs::path();
    }

    std::string EscapeName(const std::string& name);

    // A package on a device: <root>\<XUID>\<title>\<type>\<file name>; a folder (the hard drive's own format) or an
    // STFS package file (any device: kernel/stfs_mount.h).
    fs::path PackageOn(uint32_t device, uint64_t xuid, uint32_t title, uint32_t type, const std::string& fileName)
    {
        return DeviceRoot(device) / std::format("{:016X}", xuid) / std::format("{:08X}", title) /
            std::format("{:08X}", type) / EscapeName(fileName);
    }

    bool IsStfsPackage(const fs::path& package)
    {
        std::error_code ec;
        return fs::is_regular_file(package, ec);
    }

    // title 0: the running title's.
    fs::path ContentTypeDirectory(uint64_t xuid, uint32_t type, uint32_t device = kHardDriveId, uint32_t title = 0)
    {
        return DeviceRoot(device) / std::format("{:016X}", xuid) / std::format("{:08X}", title ? title : kernel::TitleId()) /
            std::format("{:08X}", type);
    }

    // Package names are the title's szFileName; characters Windows cannot store become %XX.
    std::string EscapeName(const std::string& name)
    {
        std::string out;
        for (size_t i = 0; i < name.size(); i++)
        {
            const unsigned char c = static_cast<unsigned char>(name[i]);
            const bool trailing = i == name.size() - 1 && (c == '.' || c == ' ');
            if (c < 0x20 || c >= 0x7F || std::strchr("<>:\"/\\|?*%", c) || trailing)
                out += std::format("%{:02X}", c);
            else
                out += char(c);
        }
        return out;
    }

    std::string UnescapeName(const std::string& name)
    {
        std::string out;
        for (size_t i = 0; i < name.size(); i++)
        {
            if (name[i] == '%' && i + 2 < name.size() && std::isxdigit(uint8_t(name[i + 1])) &&
                std::isxdigit(uint8_t(name[i + 2])))
            {
                out += char(std::stoi(name.substr(i + 1, 2), nullptr, 16));
                i += 2;
            }
            else
                out += name[i];
        }
        return out;
    }

    fs::path HeaderPath(const fs::path& package)
    {
        return package.parent_path().parent_path() / "Headers" / package.parent_path().filename() /
            (package.filename().string() + ".header");
    }

    // ---- Header access --------------------------------------------------------------------------------------------

    uint32_t GetBe32(const std::vector<uint8_t>& h, size_t at)
    {
        return uint32_t(h[at]) << 24 | uint32_t(h[at + 1]) << 16 | uint32_t(h[at + 2]) << 8 | h[at + 3];
    }

    uint64_t GetBe64(const std::vector<uint8_t>& h, size_t at)
    {
        return uint64_t(GetBe32(h, at)) << 32 | GetBe32(h, at + 4);
    }

    void PutBe32(std::vector<uint8_t>& h, size_t at, uint32_t v)
    {
        for (int i = 0; i < 4; i++)
            h[at + i] = uint8_t(v >> (24 - 8 * i));
    }

    void PutBe64(std::vector<uint8_t>& h, size_t at, uint64_t v)
    {
        PutBe32(h, at, uint32_t(v >> 32));
        PutBe32(h, at + 4, uint32_t(v));
    }

    std::optional<std::vector<uint8_t>> ReadHeader(const fs::path& package)
    {
        kernel::stfs_mount::WaitForWrites(package); // a package being written back after XamContentClose
        // An STFS package file carries its header (the same bytes, 0x971A of them) at its start.
        std::ifstream in(IsStfsPackage(package) ? package : HeaderPath(package), std::ios::binary);
        if (!in)
            return std::nullopt;
        std::vector<uint8_t> h(kHeaderFileSize, 0);
        in.read(reinterpret_cast<char*>(h.data()), std::streamsize(h.size()));
        // XAM (0x81680128) accepts the three package signatures: CON (console-signed), LIVE and PIRS (Microsoft).
        const uint32_t magic = GetBe32(h, 0);
        if (in.gcount() < std::streamsize(kFileNameField) ||
            (magic != kMagicCon && magic != kMagicLive && magic != kMagicPirs))
            return std::nullopt;
        return h;
    }

    void WriteHeader(const fs::path& package, const std::vector<uint8_t>& h)
    {
        if (IsStfsPackage(package))
        {
            // Through the STFS library: the header is part of the package file and its hash chain.
            std::string error;
            if (!kernel::stfs_mount::RewriteHeader(package, h, &error))
                Fatal("Content header of STFS package {}: {}", package.string(), error);
            return;
        }
        const fs::path path = HeaderPath(package);
        fs::create_directories(path.parent_path());
        std::ofstream out(path, std::ios::binary | std::ios::trunc);
        out.write(reinterpret_cast<const char*>(h.data()), std::streamsize(h.size()));
        if (!out)
            Fatal("Cannot write content header {}", path.string());
    }

    // Display-name slot of a console language (XC_LANGUAGE, 1-based): 9 slots, then 3 more (metadata version 2).
    std::optional<size_t> DisplayNameSlot(uint32_t language)
    {
        if (language >= 1 && language <= 9)
            return kDisplayNames + (language - 1) * 256;
        if (language >= 10 && language <= 12)
            return kDisplayNamesEx + (language - 10) * 256;
        return std::nullopt;
    }

    std::u16string ReadName(const std::vector<uint8_t>& h, size_t at)
    {
        std::u16string s;
        for (size_t i = 0; i < 128; i++)
        {
            const char16_t c = char16_t(h[at + i * 2] << 8 | h[at + i * 2 + 1]);
            if (c == 0)
                break;
            s += c;
        }
        return s;
    }

    // The console language's name, else the first non-empty one.
    std::u16string DisplayName(const std::vector<uint8_t>& h)
    {
        if (const auto slot = DisplayNameSlot(Settings().language))
            if (auto name = ReadName(h, *slot); !name.empty())
                return name;
        const uint32_t languages = GetBe32(h, kMetadataVersion) >= 2 ? 12 : 9;
        for (uint32_t language = 1; language <= languages; language++)
            if (auto name = ReadName(h, *DisplayNameSlot(language)); !name.empty())
                return name;
        return u"";
    }

    std::string StoredFileName(const std::vector<uint8_t>& h)
    {
        const char* p = reinterpret_cast<const char*>(h.data() + kFileNameField);
        return std::string(p, strnlen(p, kFileNameLength));
    }

    // A new package's header, as XAM writes it at creation.
    std::vector<uint8_t> NewHeader(uint32_t type, uint32_t title, uint64_t creator, const std::u16string& displayName,
        const std::string& fileName, uint32_t flags)
    {
        std::vector<uint8_t> h(kHeaderFileSize, 0);
        PutBe32(h, 0, kMagicCon);
        PutBe32(h, kHeaderSizeField, uint32_t(kMetadata + kMetadataSize));
        PutBe32(h, kContentType, type);
        PutBe32(h, kMetadataVersion, 2);
        PutBe32(h, kTitleIdField, title);
        PutBe64(h, kProfileId, creator);
        PutBe32(h, kDataFileCount, 1);
        PutBe32(h, kVolumeType, 0); // STFS
        const size_t slot = DisplayNameSlot(Settings().language).value_or(kDisplayNames); // else English
        for (size_t i = 0; i < displayName.size() && i < 127; i++)
        {
            h[slot + i * 2] = uint8_t(displayName[i] >> 8);
            h[slot + i * 2 + 1] = uint8_t(displayName[i]);
        }
        h[kFlags] = (flags & kNoProfileTransfer) ? kAttributeNoProfileTransfer : 0;
        // Thumbnails: the title's image from its SPA, as the dashboard shows for new content.
        const auto image = kernel::spa::Image(kernel::spa::kTitleImageId);
        if (!image.empty() && image.size() <= kThumbnailMax)
        {
            std::memcpy(h.data() + kThumbnail, image.data(), image.size());
            std::memcpy(h.data() + kTitleThumbnail, image.data(), image.size());
            PutBe32(h, kThumbnailSize, uint32_t(image.size()));
            PutBe32(h, kTitleThumbnailSize, uint32_t(image.size()));
        }
        std::memcpy(h.data() + kFileNameField, fileName.data(), std::min(fileName.size(), kFileNameLength));
        return h;
    }

    uint32_t LicenseMask(const std::vector<uint8_t>& h)
    {
        uint32_t mask = 0;
        for (size_t i = 0; i < kLicenseCount; i++)
            if (GetBe64(h, kLicenses + i * 16) != 0)
                mask |= GetBe32(h, kLicenses + i * 16 + 8);
        return mask;
    }

    uint64_t PackageBytes(const fs::path& package)
    {
        kernel::stfs_mount::WaitForWrites(package);
        uint64_t total = 0;
        std::error_code ec;
        if (IsStfsPackage(package))
        {
            // The package file and its data files (<package>.data\Data0000..., content bigger than FAT32 allows).
            total = fs::file_size(package, ec);
            for (const auto& entry : fs::recursive_directory_iterator(package.string() + ".data", ec))
                if (entry.is_regular_file(ec))
                    total += entry.file_size(ec);
            return total;
        }
        for (const auto& entry : fs::recursive_directory_iterator(package, ec))
            if (entry.is_regular_file(ec))
                total += entry.file_size(ec);
        return total;
    }

    // ---- Package identity -----------------------------------------------------------------------------------------

    struct ContentData
    {
        uint32_t deviceId = 0;
        uint32_t type = 0;
        std::u16string displayName;
        std::string fileName;
    };

    ContentData ReadContentData(uint32_t data)
    {
        auto& mem = guest::g_memory;
        ContentData d;
        d.deviceId = mem.Read32(data);
        d.type = mem.Read32(data + 4);
        for (uint32_t i = 0; i < 128; i++)
        {
            const char16_t c = char16_t(mem.Read16(data + 8 + i * 2));
            if (c == 0)
                break;
            d.displayName += c;
        }
        const char* name = reinterpret_cast<const char*>(mem.Translate(data + 0x108));
        d.fileName.assign(name, strnlen(name, kFileNameLength));
        return d;
    }

    // Owner XUID of content a user creates or opens: the profile's for users 0-3, 0 (common) for NONE/ANY and for
    // marketplace content. Returns an error for a user who is not signed in.
    uint32_t Owner(uint32_t user, uint32_t type, uint64_t& xuid)
    {
        if (type == kTypeMarketplace || user == 0xFE || user == 0xFF)
        {
            xuid = 0;
            return kernel::kErrorSuccess;
        }
        if (user >= 4)
            return kErrorInvalidParameter;
        if (!kernel::UserSignedIn(user))
            return kErrorAccessDenied; // XDK: creating content for a user who is not signed in
        xuid = kernel::users::Xuid(user);
        return kernel::kErrorSuccess;
    }

    fs::path PackagePath(uint64_t xuid, const ContentData& d)
    {
        return ContentTypeDirectory(xuid, d.type, d.deviceId) / EscapeName(d.fileName);
    }

    // ---- Mounted packages -----------------------------------------------------------------------------------------

    struct Mounted
    {
        std::string root;   // as the title named it
        std::string device; // runtime device the root links to
        fs::path package;
        bool stfs = false;     // an STFS package file mounted through its mirror (kernel/stfs_mount.h)
        bool writable = false; // written back into the package on flush / close
        fs::path mirror;
        uint64_t mirrorSignature = 0; // stfs_mount::TreeSignature when mounted / last written back
        bool closing = false;         // CommitMount from XamContentClose (asynchronous write-back)
    };

    std::mutex g_contentMutex;
    std::map<std::string, Mounted> g_mounted; // lower-case root name -> package
    uint32_t g_nextDevice = 1;

    std::string Lower(std::string s)
    {
        for (char& c : s)
            c = char(std::tolower(uint8_t(c)));
        return s;
    }

    void UpdateContentSize(const fs::path& package);

    // XamContentFlush / XamContentClose: a folder package records its size; a writable STFS package gets its mirror's
    // files written back into it.
    void CommitMount(Mounted& m)
    {
        if (!m.stfs)
        {
            UpdateContentSize(m.package);
            return;
        }
        if (!m.writable)
            return;
        if (m.closing)
        {
            // XamContentClose: rebuilt on a background thread (stfs_mount::CommitAsync) when its files changed.
            if (kernel::stfs_mount::TreeSignature(m.mirror) != m.mirrorSignature)
                kernel::stfs_mount::CommitAsync(m.package, m.mirror);
            return;
        }
        // Rebuilding a package rewrites the whole file (on the title's thread): only when its files changed. The dash
        // opens the content cache on every device after sign-in and closes it unchanged.
        const uint64_t signature = kernel::stfs_mount::TreeSignature(m.mirror);
        if (signature == m.mirrorSignature)
            return;
        std::string error;
        if (!kernel::stfs_mount::Commit(m.package, m.mirror, &error))
            Fatal("XamContentClose(\"{}\"): cannot write STFS package {}: {}", m.root, m.package.string(), error);
        m.mirrorSignature = signature;
    }

    bool IsMounted(const fs::path& package)
    {
        for (const auto& [root, m] : g_mounted)
            if (std::error_code ec; fs::equivalent(m.package, package, ec))
                return true;
        return false;
    }

    // Records the package's current size in its header (the console's content size).
    void UpdateContentSize(const fs::path& package)
    {
        if (IsStfsPackage(package))
            return; // the STFS library records the size when it writes the package
        if (auto h = ReadHeader(package))
        {
            PutBe64(*h, kContentSize, PackageBytes(package));
            WriteHeader(package, *h);
        }
    }

    // Packages of one owner and type, as XCONTENT_DATA items.
    void CollectPackages(uint64_t xuid, uint32_t type, std::vector<std::vector<uint8_t>>& items,
                         uint32_t device = kHardDriveId, uint32_t title = 0)
    {
        const fs::path directory = ContentTypeDirectory(xuid, type, device, title);
        std::error_code ec;
        if (!fs::is_directory(directory, ec))
            return;
        for (const auto& entry : fs::directory_iterator(directory, ec))
        {
            if (!entry.is_directory() && !kernel::stfs_mount::IsPackageFile(entry.path()))
                continue; // a package's .data folder's sibling or a stray file
            const auto h = ReadHeader(entry.path());
            if (!h)
                continue; // no valid header: not listed (as Xenia skips invalid packages)
            std::vector<uint8_t> item(kContentDataSize, 0);
            PutBe32(item, 0, device);
            PutBe32(item, 4, type);
            const std::u16string name = DisplayName(*h);
            for (size_t i = 0; i < name.size() && i < 127; i++)
            {
                item[8 + i * 2] = uint8_t(name[i] >> 8);
                item[8 + i * 2 + 1] = uint8_t(name[i]);
            }
            std::string fileName = StoredFileName(*h);
            if (fileName.empty())
                fileName = UnescapeName(entry.path().filename().string());
            std::memcpy(item.data() + 0x108, fileName.data(), std::min(fileName.size(), kFileNameLength));
            items.push_back(std::move(item));
        }
    }
}

namespace
{
    constexpr uint32_t kDeviceDataSize = 0x50;

    // A device's XDEVICE_DATA (kernel/storage.h): capacity and free space are its PC volume's, so a full disk is
    // reported truthfully.
    std::vector<uint8_t> DeviceDataOf(uint32_t id)
    {
        const auto device = kernel::storage::Find(id);
        return device ? kernel::storage::DeviceData(*device) : std::vector<uint8_t>{};
    }
}

// DWORD XamContentGetDeviceData(XCONTENTDEVICEID DeviceID, PXDEVICE_DATA DeviceData)
GUEST_IMPORT(XamContentGetDeviceData)
{
    auto& mem = guest::g_memory;
    const uint32_t device = ctx.r3.u32, data = ctx.r4.u32;
    const std::vector<uint8_t> bytes = DeviceDataOf(device);
    if (bytes.empty())
    {
        ctx.r3.u64 = kErrorDeviceNotConnected;
        return;
    }
    std::memcpy(mem.Translate(data), bytes.data(), bytes.size());
    ctx.r3.u64 = kernel::kErrorSuccess;
}

// XAM's content server keeps a table of 20 storage devices (0x81A9CB30, 0xA0 bytes each: +0x00 the volume path the
// kernel's DrvGetContentStorageNotification reported, +0x78 device ID, +0x80 state, +0x8C a 0x14-byte serial number).
// The device queries below read it (message handlers in the switch at 0x81687C00). This runtime has one device, the
// hard drive: its volume path is the console's, "\Device\Harddisk0\Partition1\" (the dash appends "Content" to it,
// "%sContent" at 0x9202DDD0); the other devices are kernel/storage.h's. A device the table lacks is 0x8007048F, which
// the client wrappers turn into ERROR_DEVICE_NOT_CONNECTED.
// DWORD XamContentGetDefaultDevice(PXCONTENTDEVICEID DeviceID) (XAM 0x816EF1B0, message 0x2002B -> 0x8167A810): the
// first device in the table whose state is 3; none: E_FAIL, which the wrapper turns into ERROR_FUNCTION_FAILED.
GUEST_IMPORT(XamContentGetDefaultDevice)
{
    guest::g_memory.Write32(ctx.r3.u32, kHardDriveId);
    ctx.r3.u64 = kernel::kErrorSuccess;
}

// DWORD XamContentGetDeviceSerialNumber(XCONTENTDEVICEID DeviceID, PBYTE Serial, DWORD Size) (XAM 0x816EF130, message
// 0x2002A -> 0x8167ADC0): copies Size bytes from the device's serial field (memcpy, no terminator). The dash asks for
// 0x14 (the field's size).
GUEST_IMPORT(XamContentGetDeviceSerialNumber)
{
    const uint32_t device = ctx.r3.u32, buffer = ctx.r4.u32, size = ctx.r5.u32;
    const auto dev = kernel::storage::Find(device);
    if (!dev)
    {
        ctx.r3.u64 = kErrorDeviceNotConnected;
        return;
    }
    if (size > 0x14)
        Fatal("XamContentGetDeviceSerialNumber: {} bytes asked; XAM would copy past the 0x14-byte field", size);
    std::memcpy(guest::g_memory.Translate(buffer), dev->serial.data(), size);
    ctx.r3.u64 = kernel::kErrorSuccess;
}

// DWORD XamContentGetDeviceVolumePath(XCONTENTDEVICEID DeviceID, LPSTR Path, DWORD Size, BOOL TrimLast) (XAM
// 0x816EF218, message 0x20024 -> 0x8167B0F0): strncpy of the volume path with Size, then Path[Size - 1] = 0; with
// TrimLast, Size becomes the path's length when it fits, so the last character (the trailing '\') is dropped.
GUEST_IMPORT(XamContentGetDeviceVolumePath)
{
    auto& mem = guest::g_memory;
    const uint32_t device = ctx.r3.u32, buffer = ctx.r4.u32, trimLast = ctx.r6.u32;
    uint32_t size = ctx.r5.u32;
    const auto dev = kernel::storage::Find(device);
    if (!dev)
    {
        ctx.r3.u64 = kErrorDeviceNotConnected;
        return;
    }
    if (size == 0)
        Fatal("XamContentGetDeviceVolumePath: size 0 (XAM writes Path[-1])");
    const std::string path = dev->volumePath;
    if (trimLast && path.size() <= size)
        size = uint32_t(path.size());
    char* out = reinterpret_cast<char*>(mem.Translate(buffer));
    std::strncpy(out, path.c_str(), size);
    out[size - 1] = 0;
    ctx.r3.u64 = kernel::kErrorSuccess;
}

// DWORD XamContentGetLocalizedDeviceData(XCONTENTDEVICEID DeviceID, PXDEVICE_DATA DeviceData) (XAM 0x816EF020,
// message 0x20031 -> 0x8167DC90): XamContentGetDeviceData, then a device still carrying its type's default English
// name ("Hard Drive" for the hard drive) gets XAM's common string for the type instead (0xA3 hard drive, 0xA4 memory
// unit, 0xA5 USB, 0xA6 type 0x10).
GUEST_IMPORT(XamContentGetLocalizedDeviceData)
{
    auto& mem = guest::g_memory;
    const uint32_t device = ctx.r3.u32, data = ctx.r4.u32;
    const auto dev = kernel::storage::Find(device);
    if (!dev)
    {
        ctx.r3.u64 = kErrorDeviceNotConnected;
        return;
    }
    std::vector<uint8_t> bytes = kernel::storage::DeviceData(*dev);
    if (!dev->defaultName)
    {
        std::memcpy(mem.Translate(data), bytes.data(), bytes.size());
        ctx.r3.u64 = kernel::kErrorSuccess;
        return;
    }
    std::fill(bytes.begin() + 0x18, bytes.end(), uint8_t(0));
    const std::u16string name = dev->kind == kernel::storage::kKindUsb ? xui::CommonString(0xA5, dev->name.c_str())
                                                                        : xui::CommonString(0xA3, dev->name.c_str());
    for (size_t i = 0; i < name.size() && i < 26; i++) // WCHAR[27], terminated at +0x4C
    {
        bytes[0x18 + i * 2] = uint8_t(name[i] >> 8);
        bytes[0x18 + i * 2 + 1] = uint8_t(name[i]);
    }
    std::memcpy(mem.Translate(data), bytes.data(), bytes.size());
    ctx.r3.u64 = kernel::kErrorSuccess;
}

namespace
{
    // XAM's license check of a package (0x8167F8F8): the entries at 0x22C up to the first empty ID; an entry applies
    // when its ID is 0xFFFFFFFFFFFFFFFF (any console, any user). Console (0xF000), XUID (9) and other entry types
    // depend on this console's keys or LIVE users: not implemented.
    uint32_t ArcadeLicenseMask(const std::vector<uint8_t>& h, const fs::path& package)
    {
        uint32_t mask = 0;
        for (size_t i = 0; i < kLicenseCount; i++)
        {
            const uint64_t id = GetBe64(h, kLicenses + i * 16);
            if (id == 0)
                break;
            if (id != ~0ull)
                Fatal("Package {}: license entry {} ({:016X}) is not an any-console license", package.string(), i, id);
            mask |= GetBe32(h, kLicenses + i * 16 + 8);
        }
        return mask;
    }

    // XAM's content item for a package (0x81684950 / 0x81684B58 / 0x81684BF0 / 0x816831D8, as the dash's
    // XamEnumerate flags 0xE1E select): the aggregate enumerator's items and the content-change callback's argument.
    std::vector<uint8_t> ContentItem(const fs::path& package, const std::vector<uint8_t>& h, uint64_t xuid,
                                     uint32_t title, uint32_t type)
    {
        if (type == 0x9000 || type >= 0x100000)
            Fatal("Content item: package {} (type {:X}): item fields +0x1D8..+0x1FA are not implemented",
                  package.string(), type);
        std::vector<uint8_t> item(0x200, 0);
        PutBe32(item, 0x000, kHardDriveId);
        PutBe32(item, 0x004, type);
        const std::u16string name = DisplayName(h);
        for (size_t i = 0; i < name.size() && i < 127; i++)
        {
            item[8 + i * 2] = uint8_t(name[i] >> 8);
            item[8 + i * 2 + 1] = uint8_t(name[i]);
        }
        std::string fileName = StoredFileName(h);
        if (fileName.empty())
            fileName = UnescapeName(package.filename().string());
        std::memcpy(item.data() + 0x108, fileName.data(), std::min(fileName.size(), kFileNameLength));
        const bool gameType = type == 2 || type == 0x7000 || type == 0x80000 || type == 0xD0000;
        if (gameType)
            std::memcpy(item.data() + 0x134, h.data() + 0x3B5, 4);
        PutBe64(item, 0x138, xuid);
        PutBe32(item, 0x140, title);
        if (type == 0xD0000 || (type == 2 && (title == 0x584E07D2 || (title >> 16) == 0x584A)))
            PutBe32(item, 0x144, ArcadeLicenseMask(h, package));
        uint64_t size = PackageBytes(package);
        if (gameType || type == 0x4000 || type == 0x5000)
            size += GetBe64(h, 0x3A1);
        PutBe64(item, 0x148, size);
        WIN32_FILE_ATTRIBUTE_DATA attributes{};
        if (GetFileAttributesExW(package.c_str(), GetFileExInfoStandard, &attributes))
            PutBe64(item, 0x150, uint64_t(attributes.ftCreationTime.dwHighDateTime) << 32 |
                                     attributes.ftCreationTime.dwLowDateTime);
        std::memcpy(item.data() + 0x158, h.data() + 0x1691, 0x80); // UTF-16BE as stored
        const uint8_t transfer = h[0x1711];
        PutBe32(item, 0x1FC, (transfer & 0x04 ? 0x08u : 0u) | (transfer & 0x08 ? 0x10u : 0u) |
                                 (transfer & 0x10 ? 0x04u : 0u) | (transfer & 0xE0u));
        return item;
    }
}

// DWORD XamContentGetMetaDataInternal(const XCONTENT_DATA_INTERNAL* Content, PVOID Metadata, PXOVERLAPPED Overlapped)
// (XAM 0x816EF338 -> message 0x20011 -> 0x81684E10): opens the item's package and copies its metadata (header
// 0x344, 0x93D6 bytes: XCONTENT_METADATA) into Metadata, then ends each string with a 0: the nine display names
// (+0x1CB + i * 0x100), the nine descriptions (+0xACB + i * 0x100), the publisher (+0x134B) and the title name
// (+0x13CB). Failures: their low 16 bits for facility WIN32, else 0x65B. The dash's Storage reads it for each item.
// Here: hard drive packages from their .header; a package not on the drive -> ERROR_FILE_NOT_FOUND (the code XAM's
// open returns was not traced).
GUEST_IMPORT(XamContentGetMetaDataInternal)
{
    auto& mem = guest::g_memory;
    const uint32_t item = ctx.r3.u32, metadata = ctx.r4.u32, overlapped = ctx.r5.u32;
    if (item == 0 || metadata == 0)
    {
        ctx.r3.u64 = Finish(overlapped, kErrorInvalidParameter);
        return;
    }
    const uint32_t device = mem.Read32(item), type = mem.Read32(item + 4);
    const char* name = reinterpret_cast<const char*>(mem.Translate(item + 0x108));
    const std::string fileName(name, strnlen(name, kFileNameLength));
    const uint64_t xuid = uint64_t(mem.Read32(item + 0x138)) << 32 | mem.Read32(item + 0x13C);
    const uint32_t title = mem.Read32(item + 0x140);
    if (!kernel::storage::Find(device))
        Fatal("XamContentGetMetaDataInternal: no device {} (type {:X}, title {:08X}, \"{}\")", device, type, title,
            fileName);
    const fs::path package = PackageOn(device, xuid, title, type, fileName);
    const auto h = ReadHeader(package);
    if (!h)
    {
        Log("XamContentGetMetaDataInternal({}): no package header: 2", package.string());
        ctx.r3.u64 = Finish(overlapped, 2);
        return;
    }
    uint8_t* out = mem.Translate(metadata);
    std::memcpy(out, h->data() + kMetadata, kMetadataSize);
    for (uint32_t i = 0; i < 9; i++)
    {
        out[0x1CB + i * 0x100] = out[0x1CC + i * 0x100] = 0;
        out[0xACB + i * 0x100] = out[0xACC + i * 0x100] = 0;
    }
    out[0x134B] = out[0x134C] = 0;
    out[0x13CB] = out[0x13CC] = 0;
    ctx.r3.u64 = Finish(overlapped, kernel::kErrorSuccess);
}

// DWORD XamContentGetLocalizedString(const XCONTENT_METADATA* Metadata, LPWSTR DisplayName, DWORD DisplayNameChars,
// LPWSTR Description, DWORD DescriptionChars) (XAM 0x816EE090): the metadata's display name (0x8167CAF8) and description
// (0x8167CBD8) in the console's language (0x8167B8E8: XTLGetLanguageV2 L 1-9 -> slot L - 1 of the nine at +0xCD /
// +0x9CD; L of 10 or more with metadata version 2 or later -> slot 11 - (L - 1) of the three at +0x50D6 / +0x90D6;
// else slot 0), each 0x100 bytes; an empty slot falls back to the first non-empty of the nine, then (version 2 or
// later) of the three. Copied up to the buffer's size and ended with a 0. Metadata NULL: 0x57.
GUEST_IMPORT(XamContentGetLocalizedString)
{
    auto& mem = guest::g_memory;
    const uint32_t metadata = ctx.r3.u32;
    if (metadata == 0)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    const uint32_t version = mem.Read32(metadata + 4);
    const uint32_t language = kernel::XConfigRead32(3, 0x09); // XTLGetLanguageV2: the console's language
    auto pick = [&](uint32_t base, uint32_t extended) {
        uint32_t slot = language - 1, at = base;
        if (slot > 8)
        {
            if (version > 1)
                at = extended + ((0xB - slot) & 0xFFFFFF) * 0x100;
            else
                slot = 0;
        }
        if (at == base)
            at = base + slot * 0x100;
        if (mem.Read16(metadata + at) == 0)
        {
            for (uint32_t i = 0; i < 9 && mem.Read16(metadata + at) == 0; i++)
                at = base + i * 0x100;
            for (uint32_t i = 0; version > 1 && i < 3 && mem.Read16(metadata + at) == 0; i++)
                at = extended + i * 0x100;
        }
        return metadata + at;
    };
    auto copy = [&](uint32_t to, uint32_t chars, uint32_t from) {
        if (to == 0 || chars == 0)
            return;
        bool ended = false;
        for (uint32_t i = 0; i < chars; i++)
        {
            const uint16_t c = ended || i >= 0x80 ? 0 : mem.Read16(from + i * 2);
            ended = ended || c == 0;
            mem.Write16(to + i * 2, c);
        }
        mem.Write16(to + (chars - 1) * 2, 0);
    };
    copy(ctx.r4.u32, ctx.r5.u32, pick(0xCD, 0x50D6));
    copy(ctx.r6.u32, ctx.r7.u32, pick(0x9CD, 0x90D6));
    ctx.r3.u64 = 0;
}

// DWORD XamContentResolveInternal(const XCONTENT_DATA_INTERNAL* Content, LPSTR Path, DWORD PathSize, ...) (XAM
// 0x816EE8E0 -> 0x816EE7B0, user 0xFE, the 0x200-byte item): XAM message 0x20016 (0x8167F988) looks up the item's
// device (none: E_FAIL), finds the package on it (0x8167F648) and writes its device path; HRESULT failures return
// their low 16 bits for facility WIN32, else 0x65B. The dash calls it (0x9218D750) for the theme a profile names in
// its ThematicSkin setting before mounting it as "theme"; a failure keeps the default theme.
// Here: device 0 or any device but the hard drive -> 0x65B; a package not on the drive -> ERROR_FILE_NOT_FOUND (the
// code 0x8167F648 returns was not traced). A package that exists stops: its device path is not implemented.
GUEST_IMPORT(XamContentResolveInternal)
{
    auto& mem = guest::g_memory;
    const uint32_t item = ctx.r3.u32;
    if (item == 0)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    const uint32_t device = mem.Read32(item), type = mem.Read32(item + 4);
    const char* name = reinterpret_cast<const char*>(mem.Translate(item + 0x108));
    const std::string fileName(name, strnlen(name, kFileNameLength));
    const uint64_t xuid = uint64_t(mem.Read32(item + 0x138)) << 32 | mem.Read32(item + 0x13C);
    const uint32_t title = mem.Read32(item + 0x140);
    if (device != kHardDriveId)
    {
        // Other devices' guest paths are not mounted as volumes (XamContentGetDeviceVolumePath only names them).
        Log("XamContentResolveInternal(device {}, type {:X}, title {:08X}, \"{}\"): not the hard drive: 65B", device,
            type, title, fileName);
        ctx.r3.u64 = 0x65B;
        return;
    }
    const fs::path package = PackageOn(device, xuid, title, type, fileName);
    std::error_code ec;
    if (!fs::exists(package, ec))
    {
        Log("XamContentResolveInternal(type {:X}, title {:08X}, \"{}\"): not on the hard drive: 2", type, title,
            fileName);
        ctx.r3.u64 = 2;
        return;
    }
    const uint32_t pathPtr = ctx.r4.u32, pathSize = ctx.r5.u32, checkExists = ctx.r6.u32, linkName = ctx.r7.u32,
                   overlapped = ctx.r8.u32;
    if (linkName != 0 || overlapped != 0)
        Fatal("XamContentResolveInternal({}): link name {:08X} / overlapped {:08X} are not implemented",
              package.string(), linkName, overlapped);
    const std::string path = std::format("\\Device\\Harddisk0\\Partition1\\Content\\{:016X}\\{:08X}\\{:08X}\\{}", xuid,
                                         title, type, fileName);
    Log("XamContentResolveInternal({}, {:X}, check {}): {} (from {})", pathPtr == 0 ? "NULL" : "buffer", pathSize,
        checkExists, path, kernel::GuestBacktrace(ctx));
    if (pathPtr == 0 || path.size() + 1 > pathSize)
        Fatal("XamContentResolveInternal: path buffer {:08X} of {} bytes for {}", pathPtr, pathSize, path);
    std::memcpy(mem.Translate(pathPtr), path.c_str(), path.size() + 1);
    ctx.r3.u64 = 0;
}

// DWORD XamContentAggregateCreateEnumerator(XUID Xuid, XCONTENTDEVICEID DeviceID, DWORD ContentType, DWORD TitleId,
//     PHANDLE Enumerator) (XAM 0x816EEDB8): every package on a device, across owners and titles; one 0x200-byte
// XCONTENT_DATA_INTERNAL per XamEnumerate (app 0xFE, enumerate 0x2000E -> 0x816875B8, close 0x20010). XAM walks
// Content\<XUID>\<TitleID>\<Type>\<file> (directory names parsed as hex): Xuid 0 = every owner including the common
// 0000000000000000, -1 = common only, else that owner; TitleId 0 = every title; ContentType 0xFFFFFFFF = every type.
// It reads each package's header (unreadable packages are skipped) and fills (0x81685588, for the dash's XamEnumerate
// flags 0xE1E): device +0, type +4, display name +8, file name +0x108, XUID +0x138, title ID +0x140; +0x134 the header
// dword at 0x3B5 for types 2 / 0x7000 / 0x80000 / 0xD0000; +0x144 the license mask (0x816831D8: OR of the bits of the
// license entries at 0x22C that apply, up to the first empty one) for type 0xD0000 and type 2 of title 584E07D2 /
// 584Axxxx; +0x148 size (package file + the data-file total at 0x3A1 for game types) and +0x150 creation time
// (0x81684688); +0x158 title name (0x1691); +0x1FC bits 2-7 from the transfer flags at 0x1711 (bit 2 -> 0x08, 3 -> 0x10,
// 4 -> 0x04, 5-7 kept) for types below 0x100000; bit 1 / bit 0 mark corrupt / incomplete headers. XamEnumerate flags
// 0x40 / 0x100 filter types (not implemented: stop). Types 0x9000 and >= 0x100000 also fill +0x1D8..+0x1FA (stop).
// Deviations: imported profiles have no package header, so they are not listed (a console lists them, type 0x10000);
// packages are folders, so the "file" size is the folder's total and the creation time the folder's; signatures are
// not verified (a bad one would set the corrupt bit); XAM's ExExpansionCall('PV03') on an Arcade license mask is not
// made.
GUEST_IMPORT(XamContentAggregateCreateEnumerator)
{
    auto& mem = guest::g_memory;
    const uint64_t xuidFilter = ctx.r3.u64;
    const uint32_t device = ctx.r4.u32, typeFilter = ctx.r5.u32, titleFilter = ctx.r6.u32, handlePtr = ctx.r7.u32;
    if (handlePtr == 0)
        Fatal("XamContentAggregateCreateEnumerator: no handle pointer");

    auto parseHex = [](const std::string& name, size_t digits, uint64_t& value)
        {
            if (name.size() != digits)
                return false;
            value = 0;
            for (char c : name)
            {
                const int d = c >= '0' && c <= '9' ? c - '0' : c >= 'A' && c <= 'F' ? c - 'A' + 10 :
                    c >= 'a' && c <= 'f' ? c - 'a' + 10 : -1;
                if (d < 0)
                    return false;
                value = value << 4 | uint64_t(d);
            }
            return true;
        };
    auto dirs = [](const fs::path& p)
        {
            std::vector<fs::path> out;
            std::error_code ec;
            if (fs::is_directory(p, ec))
                for (const auto& entry : fs::directory_iterator(p, ec))
                    if (entry.is_directory())
                        out.push_back(entry.path());
            std::sort(out.begin(), out.end());
            return out;
        };

    std::vector<std::vector<uint8_t>> items;
    if (kernel::storage::Find(device))
    {
        std::lock_guard lock(g_contentMutex);
        for (const fs::path& owner : dirs(DeviceRoot(device)))
        {
            uint64_t xuid = 0;
            if (!parseHex(owner.filename().string(), 16, xuid))
                continue;
            if (xuidFilter == ~0ull ? xuid != 0 : xuidFilter != 0 && xuid != xuidFilter)
                continue;
            for (const fs::path& titleDir : dirs(owner))
            {
                uint64_t title = 0;
                if (!parseHex(titleDir.filename().string(), 8, title) || (titleFilter && title != titleFilter))
                    continue;
                for (const fs::path& typeDir : dirs(titleDir))
                {
                    uint64_t type = 0;
                    if (!parseHex(typeDir.filename().string(), 8, type) ||
                        (typeFilter != 0xFFFFFFFF && type != typeFilter))
                        continue;
                    std::error_code ec;
                    for (const auto& entry : fs::directory_iterator(typeDir, ec))
                    {
                        if (!entry.is_directory() && !kernel::stfs_mount::IsPackageFile(entry.path()))
                            continue; // data folders (<package>.data) and other files
                        const auto h = ReadHeader(entry.path());
                        if (!h)
                            continue; // unreadable: skipped, as XAM does without flag 0x80
                        items.push_back(ContentItem(entry.path(), *h, xuid, uint32_t(title), uint32_t(type)));
                        PutBe32(items.back(), 0, device);
                    }
                }
            }
        }
    }
    Log("XamContentAggregateCreateEnumerator(XUID {:016X}, device {}, type {:X}, title {:08X}): {} packages",
        xuidFilter, device, typeFilter, titleFilter, items.size());

    kernel::EnumeratorInfo info;
    info.appId = 0xFE;
    info.message = 0x2000E;
    info.closeMessage = 0x20010;
    info.userIndex = 0xFF;
    info.itemsPerEnumerate = 1;
    info.unsupportedEnumerateFlags = 0x140;
    info.wholeItems = true;
    mem.Write32(handlePtr, kernel::CreateHostEnumerator(info, 0x200, std::move(items)));
    ctx.r3.u64 = kernel::kErrorSuccess;
}

// DWORD XamContentCreateDeviceEnumerator(DWORD ContentTypes, DWORD Flags, DWORD ItemCount, PDWORD BufferSize,
//                                        PHANDLE Enumerator) (XAM 0x816EEE58): an enumerator of XDEVICE_DATA items
// (app 0xFE, enumerate message 0x2000A, close 0x20009; ContentTypes is not used). Its enumeration (0x81681B20) walks
// XAM's device table and lists every mounted device; the Flags bits add devices in other states (0x80000000
// unformatted, 0x04000000 / 0x02000000 two more states, 0x01000000 mounted type-8 devices; 0x10000000 ORs extra
// type bits into memory units). Every device here is mounted (kernel/storage.h), so all are listed for any Flags.
// *BufferSize = ItemCount * sizeof(XDEVICE_DATA).
GUEST_IMPORT(XamContentCreateDeviceEnumerator)
{
    auto& mem = guest::g_memory;
    const uint32_t itemCount = ctx.r5.u32, sizePtr = ctx.r6.u32, handlePtr = ctx.r7.u32;
    if (handlePtr == 0)
        Fatal("XamContentCreateDeviceEnumerator: no handle pointer");
    std::vector<std::vector<uint8_t>> items;
    for (const kernel::storage::Device& dev : kernel::storage::Devices())
        items.push_back(kernel::storage::DeviceData(dev));
    if (sizePtr)
        mem.Write32(sizePtr, itemCount * kDeviceDataSize);
    kernel::EnumeratorInfo info;
    info.appId = 0xFE;
    info.message = 0x2000A;
    info.closeMessage = 0x20009;
    info.userIndex = 0xFE;
    info.itemsPerEnumerate = itemCount;
    mem.Write32(handlePtr, kernel::CreateHostEnumerator(info, kDeviceDataSize, std::move(items)));
    ctx.r3.u64 = kernel::kErrorSuccess;
}

// DWORD XamContentGetDeviceState(XCONTENTDEVICEID DeviceID, PXOVERLAPPED Overlapped)
GUEST_IMPORT(XamContentGetDeviceState)
{
    ctx.r3.u64 = Finish(ctx.r4.u32, kernel::storage::Find(ctx.r3.u32) ? kernel::kErrorSuccess : kErrorDeviceNotConnected);
}

// DWORD XamShowDeviceSelectorUI(DWORD UserIndex, DWORD ContentType, DWORD Flags, ULARGE_INTEGER BytesRequested,
//                               PXCONTENTDEVICEID DeviceID, PXOVERLAPPED Overlapped)
// The selector is the deviceselector.xex app (XAM 0x816C51E8 starts it). Which devices it offers (FUN_916D3B60,
// SHOWS): every device from XamContentCreateDeviceEnumerator, minus optical drives; Flags 0x2000 keeps those with
// BytesRequested free; 0x10000 only ready USB devices; 0x20000 adds type-8 devices (enumerator flag 0x1000000);
// 0x40000 hides cloud storage (here never shown); 0x8000 asks for memory-unit kinds (not handled: stops). The
// current device is not removed (the dash's Storage transfer rejects the same device itself). The XDK: with exactly
// one device and no XCONTENTFLAG_FORCE_SHOW_UI, it is chosen without UI.
// When it would show, the selector UI (native/x360/system/xui ShowDeviceSelector, XAM's own scene) lets the player pick;
// CB_DEVICE_SELECTOR=<device ID> (tests) picks without it.
GUEST_IMPORT(XamShowDeviceSelectorUI)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, flags = ctx.r5.u32, deviceId = ctx.r7.u32, overlapped = ctx.r8.u32;
    const uint64_t bytes = ctx.r6.u64;
    if (overlapped == 0 || deviceId == 0 || (flags & 0x83F00008) || (user >= 4 && user != 0xFF))
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    if (flags & 0x8000)
        Fatal("XamShowDeviceSelectorUI: flags {:X} (0x8000: memory-unit kinds, not handled)", flags);
    std::vector<uint32_t> offered;
    for (const kernel::storage::Device& dev : kernel::storage::Devices())
    {
        if ((flags & 0x10000) && dev.kind != kernel::storage::kKindUsb)
            continue;
        if (flags & 0x2000)
        {
            ULARGE_INTEGER freeBytes{};
            GetDiskFreeSpaceExW(dev.volume.c_str(), &freeBytes, nullptr, nullptr);
            if (freeBytes.QuadPart < bytes)
                continue;
        }
        offered.push_back(dev.id);
    }
    if (offered.empty())
        Fatal("XamShowDeviceSelectorUI(flags {:X}, {} bytes): no device to offer (the selector's empty list is not "
              "implemented)", flags, bytes);
    uint32_t chosen = offered[0];
    if ((flags & kForceShowUi) || offered.size() > 1)
    {
        if (const char* pick = std::getenv("CB_DEVICE_SELECTOR"); pick && *pick)
        {
            chosen = uint32_t(std::strtoul(pick, nullptr, 0));
            if (std::find(offered.begin(), offered.end(), chosen) == offered.end())
                Fatal("CB_DEVICE_SELECTOR={}: not among the offered devices", pick);
            Log("XamShowDeviceSelectorUI(flags {:X}): CB_DEVICE_SELECTOR picks device {}", flags, chosen);
        }
        else
        {
            // The selector UI (deviceselector.xex's DeviceSelector.xur): the player picks; B cancels with
            // ERROR_CANCELLED as the other system UIs do.
            std::vector<kernel::DeviceChoice> choices;
            for (const uint32_t id : offered)
                if (const auto dev = kernel::storage::Find(id))
                {
                    ULARGE_INTEGER freeBytes{}, totalBytes{};
                    GetDiskFreeSpaceExW(dev->volume.c_str(), &freeBytes, &totalBytes, nullptr);
                    choices.push_back({ dev->name, dev->kind, freeBytes.QuadPart, totalBytes.QuadPart });
                }
            kernel::BeginOverlapped(overlapped);
            const bool shown = kernel::ShowDeviceSelectorUi(user, choices, [offered, deviceId, overlapped](int row) {
                if (row < 0 || size_t(row) >= offered.size())
                {
                    kernel::CompleteOverlapped(overlapped, 0x4C7, 0x800704C7, 0);
                    return;
                }
                guest::g_memory.Write32(deviceId, offered[size_t(row)]);
                Log("XamShowDeviceSelectorUI: the player chose device {}", offered[size_t(row)]);
                kernel::CompleteOverlapped(overlapped, kernel::kErrorSuccess, 0, 0);
            });
            if (shown)
            {
                ctx.r3.u64 = kernel::kErrorIoPending;
                return;
            }
            Log("XamShowDeviceSelectorUI(flags {:X}): no system UI to show the selector: device {}", flags, chosen);
        }
    }
    mem.Write32(deviceId, chosen);
    ctx.r3.u64 = Finish(overlapped, kernel::kErrorSuccess);
}

// DWORD XamContentGetLicenseMask(PDWORD LicenseMask, PXOVERLAPPED Overlapped)
// The title runs as an installed Xbox LIVE Arcade package; its license bits come from console_settings.ini
// (bit 0 = full game; TU46 checks only that bit).
GUEST_IMPORT(XamContentGetLicenseMask)
{
    const uint32_t mask = ctx.r3.u32, overlapped = ctx.r4.u32;
    if (mask == 0)
    {
        ctx.r3.u64 = Finish(overlapped, kErrorInvalidParameter);
        return;
    }
    guest::g_memory.Write32(mask, Settings().licenseMask);
    ctx.r3.u64 = Finish(overlapped, kernel::kErrorSuccess);
}

namespace
{
    // The enumerator both forms create: the packages of `xuid` (0: none) and the common ones (unless
    // XCONTENTFLAG_ENUM_EXCLUDECOMMON) of `title` (0: the running title), snapshotted now; *BufferSize = ItemCount *
    // sizeof(XCONTENT_DATA).
    uint32_t CreateContentEnumerator(uint32_t user, uint64_t xuid, uint32_t device, uint32_t type, uint32_t title,
                                     uint32_t flags, uint32_t itemCount, uint32_t sizePtr, uint32_t handlePtr)
    {
        auto& mem = guest::g_memory;
        if (device != 0 && !kernel::storage::Find(device))
            return kHresultInvalidArg;
        if (handlePtr == 0)
            return kErrorInvalidParameter;
        // Device 0: every device (XDK: XCONTENTDEVICE_ANY).
        std::vector<std::vector<uint8_t>> items;
        for (const kernel::storage::Device& dev : kernel::storage::Devices())
        {
            if (device != 0 && dev.id != device)
                continue;
            std::lock_guard lock(g_contentMutex);
            if (xuid && type != kTypeMarketplace)
                CollectPackages(xuid, type, items, dev.id, title);
            if (!(flags & kEnumExcludeCommon))
                CollectPackages(0, type, items, dev.id, title);
        }
        if (sizePtr)
            mem.Write32(sizePtr, itemCount * kContentDataSize);
        kernel::EnumeratorInfo info;
        info.appId = 0xFE; // XAM; enumerate/close messages as Xenia uses for content enumerators
        info.message = 0x20005;
        info.closeMessage = 0x20007;
        info.userIndex = user;
        info.itemsPerEnumerate = itemCount;
        mem.Write32(handlePtr, kernel::CreateHostEnumerator(info, kContentDataSize, std::move(items)));
        return kernel::kErrorSuccess;
    }
}

// DWORD XamContentCreateEnumerator(DWORD UserIndex, XCONTENTDEVICEID DeviceID, DWORD ContentType, DWORD Flags,
//                                  DWORD ItemCount, PDWORD BufferSize, PHANDLE Enumerator)
// Lists the user's packages (none for user index NONE/ANY) and common ones of the running title (XAM 17559
// 0x816EFE38: XamContentCreateEnumeratorInternal with the user's XUID and title 0).
GUEST_IMPORT(XamContentCreateEnumerator)
{
    const uint32_t user = ctx.r3.u32;
    if (user < 4 && !kernel::UserSignedIn(user))
    {
        ctx.r3.u64 = kErrorNoSuchUser;
        return;
    }
    if (user >= 4 && user != 0xFE && user != 0xFF)
        Fatal("XamContentCreateEnumerator: user index {:X}", user);
    ctx.r3.u64 = CreateContentEnumerator(user, user < 4 ? kernel::users::Xuid(user) : 0, ctx.r4.u32, ctx.r5.u32, 0,
        ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32);
}

// DWORD XamContentCreateEnumeratorInternal(QWORD Xuid, XCONTENTDEVICEID DeviceID, DWORD ContentType, DWORD TitleId,
//                                          DWORD Flags, DWORD ItemCount, PDWORD BufferSize, PHANDLE Enumerator)
// (XAM 17559 0x816EECF8, its enumerator state 0x8167CD38: device +0x10, XUID +0x60, type +0x68, title +0x6C, flags
// +0x70): a title's content by XUID, the 6719 dash's game details (its demos, title 0x584307D4 ...). Xuid 0: the
// common packages only; title 0: the running title.
GUEST_IMPORT(XamContentCreateEnumeratorInternal)
{
    ctx.r3.u64 = CreateContentEnumerator(0xFF, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
        ctx.r8.u32, ctx.r9.u32, ctx.r10.u32);
}

namespace
{
    std::atomic<uint32_t> g_contentChangeCallback = 0;

    std::vector<uint8_t> ContentItem(const fs::path& package, const std::vector<uint8_t>& h, uint64_t xuid,
                                     uint32_t title, uint32_t type);

    // XAM's content-change notification (0x81684E90): Callback(type, item) with the package's content item, on the
    // thread of the operation (0 added: after a package is created, 0x81686DA0; 1 deleted; 2 updated). The item is
    // passed on the guest stack below the caller's frame.
    void NotifyContentChange(PPCContext& ctx, uint32_t type, const std::vector<uint8_t>& item)
    {
        const uint32_t callback = g_contentChangeCallback.load();
        if (callback == 0)
            return;
        PPCContext call = ctx;
        const uint32_t frame = (ctx.r1.u32 - 0x400) & ~0xFu;
        const uint32_t data = frame + 0x100;
        std::memcpy(guest::g_memory.Translate(data), item.data(), item.size());
        call.r1.u64 = frame;
        call.r3.u64 = type;
        call.r4.u64 = data;
        kernel::CallGuest(call, callback);
    }

    // XamContentCreateEx / XamContentCreateInternal after their argument checks: creates or opens the package of
    // `xuid` and `title` and links "<root>:" to it. Returns the import's result.
    uint32_t CreateOrOpen(const std::string& root, const ContentData& data, uint64_t xuid, uint32_t title,
                          uint32_t flags, uint32_t dispositionPtr, uint32_t licensePtr, uint32_t overlapped,
                          std::optional<std::vector<uint8_t>>& createdItem)
    {
        auto& mem = guest::g_memory;
        auto fail = [&](uint32_t error) { return Finish(overlapped, error); };
        const uint32_t disposition = flags & 0xF;
        if (disposition < kCreateNew || disposition > kTruncateExisting)
            return fail(kErrorInvalidParameter);

        std::lock_guard lock(g_contentMutex);
        if (g_mounted.contains(Lower(root)))
            Fatal("XamContentCreate: root \"{}\" is already mounted (behaviour not established)", root);

        const uint32_t deviceId = data.deviceId ? data.deviceId : kHardDriveId;
        const fs::path package = PackageOn(deviceId, xuid, title, data.type, data.fileName);
        const bool exists = fs::exists(package);
        // New packages: folders on the hard drive (its own format), STFS package files on the other devices.
        const bool newAsStfs = deviceId != kHardDriveId;
        std::optional<std::vector<uint8_t>> header;
        uint32_t result = kOpenedExisting;
        auto create = [&]
            {
                header = NewHeader(data.type, title, xuid, data.displayName, data.fileName, flags);
                if (newAsStfs)
                {
                    std::string error;
                    if (!kernel::stfs_mount::CreateEmpty(package, *header, &error))
                        Fatal("XamContentCreate: cannot create STFS package {}: {}", package.string(), error);
                }
                else
                {
                    fs::create_directories(package);
                    WriteHeader(package, *header);
                }
                result = kCreatedNew;
                createdItem = ContentItem(package, *header, xuid, title, data.type);
                PutBe32(*createdItem, 0, deviceId);
            };
        switch (disposition)
        {
        case kCreateNew:
            if (exists)
                return fail(kErrorAlreadyExists); // Xenia's code; the XDK does not name it
            create();
            break;
        case kCreateAlways:
            if (exists)
            {
                fs::remove_all(package);
                fs::remove(HeaderPath(package));
                std::error_code ec;
                fs::remove_all(package.string() + ".data", ec);
            }
            create();
            break;
        case kOpenExisting:
        case kTruncateExisting:
            if (!exists)
                return fail(kErrorPathNotFound); // Xenia's code
            break;
        case kOpenAlways:
            if (!exists)
                create();
            break;
        }
        if (!header)
        {
            header = ReadHeader(package);
            if (!header)
                return fail(kErrorFileCorrupt);
            // XCONTENTFLAG_NOPROFILE_TRANSFER: only the creating profile may open it.
            if (((*header)[kFlags] & kAttributeNoProfileTransfer) && xuid != 0 && GetBe64(*header, kProfileId) != xuid)
                return fail(kErrorAccessDenied);
        }

        // An STFS package mounts through its mirror; CON packages are writable (written back on flush / close),
        // LIVE / PIRS (Microsoft's) read-only.
        Mounted mount{ root, std::format("Content{}", g_nextDevice++), package };
        fs::path mountRoot = package;
        if (IsStfsPackage(package))
        {
            std::string error;
            const auto mirror = kernel::stfs_mount::Mirror(package, &error);
            if (!mirror)
            {
                Log("XamContentCreate(\"{}\"): {}: {}", root, package.string(), error);
                return fail(kErrorFileCorrupt);
            }
            mount.stfs = true;
            mount.writable = GetBe32(*header, 0) == kMagicCon;
            mount.mirror = *mirror;
            mount.mirrorSignature = kernel::stfs_mount::TreeSignature(*mirror);
            mountRoot = *mirror;
        }
        if (disposition == kTruncateExisting && result == kOpenedExisting)
        {
            if (mount.stfs && !mount.writable)
                return fail(kErrorAccessDenied);
            for (const auto& entry : fs::directory_iterator(mountRoot))
                fs::remove_all(entry.path());
            result = kCreatedNew;
        }
        vfs::MountDevice(mount.device, mountRoot, mount.stfs && !mount.writable);
        vfs::CreateLink("\\??\\" + root + ":", mount.device + ":");
        g_mounted[Lower(root)] = mount;
        const uint32_t license = LicenseMask(*header);
        if (licensePtr)
            mem.Write32(licensePtr, license);
        if (overlapped)
            return Finish(overlapped, kernel::kErrorSuccess, result);
        if (dispositionPtr)
            mem.Write32(dispositionPtr, result);
        return kernel::kErrorSuccess;
    }
}

namespace
{
    // XamContentCreateEx's body with its overlapped argument passed directly (XamContentCreate has it in r9).
    void ContentCreateEx(PPCContext& ctx, uint32_t overlapped);
}

// DWORD XamContentCreateEx(DWORD UserIndex, LPCSTR RootName, const XCONTENT_DATA* ContentData, DWORD Flags,
//     PDWORD Disposition, PDWORD LicenseMask, DWORD FileCacheSize, ULARGE_INTEGER ContentSize, PXOVERLAPPED Overlapped)
// Creates or opens a package and links "<RootName>:" to it. Asynchronously, the disposition goes to InternalHigh
// (the Disposition pointer is not used) and *LicenseMask is written on completion.
GUEST_IMPORT(XamContentCreateEx)
{
    ContentCreateEx(ctx, kernel::StackArg32(ctx, 8));
}

// DWORD XamContentCreate(DWORD UserIndex, LPCSTR RootName, const XCONTENT_DATA* ContentData, DWORD Flags,
//     PDWORD Disposition, PDWORD LicenseMask, PXOVERLAPPED Overlapped) (XAM 0x816EFD48): XamContentCreateEx with
// FileCacheSize 0 and ContentSize 0 (r9 = r10 = 0, the overlapped moved to the ninth argument's stack slot).
GUEST_IMPORT(XamContentCreate)
{
    const uint32_t overlapped = ctx.r9.u32;
    ctx.r9.u64 = 0;
    ctx.r10.u64 = 0;
    ContentCreateEx(ctx, overlapped);
}

// BOOL XamContentExistsOnDeviceInternal(const XCONTENT_DATA_INTERNAL* Data, DWORD DeviceId) (XAM 0x816EFDA0): copies
// the 0x200-byte descriptor, puts DeviceId in its first word (the device ID), and tries XamContentCreateInternal(
// "XamContentExists...", copy, OPEN_EXISTING, NULL, NULL, 0, 0, NULL); on success XamContentClose(root, NULL), TRUE.
// No content-change notification: opening an existing package creates nothing.
GUEST_IMPORT(XamContentExistsOnDeviceInternal)
{
    auto& mem = guest::g_memory;
    const uint32_t dataPtr = ctx.r3.u32, deviceId = ctx.r4.u32;
    if (dataPtr == 0)
        Fatal("XamContentExistsOnDeviceInternal: no content data (XAM copies 0x200 bytes from it)");
    std::vector<uint8_t> copy(mem.Translate(dataPtr), mem.Translate(dataPtr) + 0x200);
    PutBe32(copy, 0x00, deviceId);
    const uint32_t scratch = mem.SystemAlloc(0x200);
    std::memcpy(mem.Translate(scratch), copy.data(), copy.size());
    ContentData data = ReadContentData(scratch);
    mem.SystemFree(scratch);

    bool exists = false;
    if (!data.fileName.empty() && kernel::storage::Find(data.deviceId))
    {
        // A root name of our own: the guest cannot hold it, so XAM's "already mounted" case cannot arise.
        const std::string root = "XamContentExistsOnDeviceInternal";
        std::optional<std::vector<uint8_t>> created;
        if (CreateOrOpen(root, data, GetBe64(copy, 0x138), GetBe32(copy, 0x140), kOpenExisting, 0, 0, 0, created) ==
            kernel::kErrorSuccess)
        {
            std::lock_guard lock(g_contentMutex);
            const auto it = g_mounted.find(Lower(root));
            vfs::DeleteLink("\\??\\" + it->second.root + ":");
            vfs::UnmountDevice(it->second.device);
            g_mounted.erase(it);
            exists = true;
        }
    }
    ctx.r3.u64 = exists ? 1 : 0;
}

namespace
{
void ContentCreateEx(PPCContext& ctx, uint32_t overlapped)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, rootPtr = ctx.r4.u32, dataPtr = ctx.r5.u32, flags = ctx.r6.u32;
    const uint32_t dispositionPtr = ctx.r7.u32, licensePtr = ctx.r8.u32;

    auto fail = [&](uint32_t error) { ctx.r3.u64 = Finish(overlapped, error); };
    if (dataPtr == 0)
        return fail(kErrorInvalidParameter);
    const std::string root = rootPtr ? std::string(reinterpret_cast<const char*>(mem.Translate(rootPtr))) : "";
    const ContentData data = ReadContentData(dataPtr);
    if (root.empty() || data.fileName.empty())
        return fail(kErrorInvalidName);
    if (!kernel::storage::Find(data.deviceId))
        return fail(kErrorDeviceNotConnected);
    uint64_t xuid = 0;
    if (const uint32_t error = Owner(user, data.type, xuid))
        return fail(error);
    std::optional<std::vector<uint8_t>> created;
    ctx.r3.u64 = CreateOrOpen(root, data, xuid, kernel::TitleId(), flags, dispositionPtr, licensePtr, overlapped,
                              created);
    if (created)
        NotifyContentChange(ctx, 0, *created);
}
}

// DWORD XamContentCreateInternal(LPCSTR RootName, const XCONTENT_DATA_INTERNAL* Data, DWORD Flags, PDWORD Disposition,
//     PDWORD LicenseMask, DWORD FileCacheSize, ULARGE_INTEGER ContentSize, PXOVERLAPPED Overlapped) (XAM 0x816EE478):
// XamContentCreateEx for system code, for any owner and title: the 0x200-byte XCONTENT_DATA_INTERNAL names them
// (XUID at 0x138, title ID at 0x140); an empty or invalid file name fails with 0x57. The dash's content manager keeps
// its content display cache in content/0/FFFE07DF/00040000/ContentCache.pkg (CDIs.bin).
GUEST_IMPORT(XamContentCreateInternal)
{
    auto& mem = guest::g_memory;
    const uint32_t rootPtr = ctx.r3.u32, dataPtr = ctx.r4.u32, flags = ctx.r5.u32;
    const uint32_t dispositionPtr = ctx.r6.u32, licensePtr = ctx.r7.u32, overlapped = ctx.r10.u32;
    if (dataPtr == 0)
        Fatal("XamContentCreateInternal: no content data");
    const ContentData data = ReadContentData(dataPtr);
    if (data.fileName.empty())
    {
        ctx.r3.u64 = Finish(overlapped, kErrorInvalidParameter);
        return;
    }
    const std::string root = rootPtr ? std::string(reinterpret_cast<const char*>(mem.Translate(rootPtr))) : "";
    if (root.empty())
    {
        ctx.r3.u64 = Finish(overlapped, kErrorInvalidName);
        return;
    }
    if (!kernel::storage::Find(data.deviceId))
    {
        ctx.r3.u64 = Finish(overlapped, kErrorDeviceNotConnected);
        return;
    }
    std::optional<std::vector<uint8_t>> created;
    ctx.r3.u64 = CreateOrOpen(root, data, mem.Read64(dataPtr + 0x138), mem.Read32(dataPtr + 0x140), flags,
                              dispositionPtr, licensePtr, overlapped, created);
    if (created)
        NotifyContentChange(ctx, 0, *created);
}

// DWORD XamProfileOpen(XUID Xuid, LPCSTR RootName, DWORD Flags, PXCONTENT_DATA_INTERNAL Data): mounts a profile's
// package (content/<XUID>/FFFE07D1/00010000/<XUID>) as RootName. XAM 17559: 0x57 unless the XUID is offline (E...);
// XamProfileFindAccount's error for a profile the console does not have (2 here); then XamContentCreateInternal(Root,
// data, OPEN_EXISTING, ..., (Flags & 1) << 16) with a zeroed 0x200-byte XCONTENT_DATA_INTERNAL {device ID, type
// 0x10000, file name "%16I64X" at 0x108, XUID at 0x138, title FFFE07D1 at 0x140}, copied to Data on success.
// Imported profiles have no package header, so the folder is mounted as it is. XamProfileClose = XamContentClose.
GUEST_IMPORT(XamProfileOpen)
{
    auto& mem = guest::g_memory;
    const uint64_t xuid = ctx.r3.u64;
    const uint32_t rootPtr = ctx.r4.u32, flags = ctx.r5.u32, dataPtr = ctx.r6.u32;
    if ((xuid >> 60) != 0xE)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    if (flags & ~1u)
        Fatal("XamProfileOpen({:016X}): flags {:X} (only bit 0 is known)", xuid, flags);
    const auto& profiles = kernel::users::Profiles();
    if (std::none_of(profiles.begin(), profiles.end(), [&](const kernel::users::Profile& p) { return p.xuid == xuid; }))
    {
        ctx.r3.u64 = kErrorFileNotFound;
        return;
    }
    const std::string root = rootPtr ? std::string(reinterpret_cast<const char*>(mem.Translate(rootPtr))) : "";
    if (root.empty())
        Fatal("XamProfileOpen({:016X}): no root name", xuid);

    std::lock_guard lock(g_contentMutex);
    if (g_mounted.contains(Lower(root)))
        Fatal("XamProfileOpen: root \"{}\" is already mounted (behaviour not established)", root);
    const std::string name = std::format("{:016X}", xuid);
    // On whichever device holds it (kernel/profile_location.h): a folder, or an STFS package's mirror (written back
    // on XamProfileClose).
    const kernel::profile_location::Location where = kernel::profile_location::Find(xuid);
    const fs::path folder = kernel::profile_location::Folder(xuid);
    fs::create_directories(folder); // a configured profile's package exists from its creation on a console
    Mounted mount{ root, std::format("Content{}", g_nextDevice++), where.device ? where.package : folder };
    mount.stfs = where.stfs;
    mount.writable = where.stfs;
    mount.mirror = where.stfs ? folder : fs::path();
    if (where.stfs)
        mount.mirrorSignature = kernel::stfs_mount::TreeSignature(folder);
    vfs::MountDevice(mount.device, folder, false);
    vfs::CreateLink("\\??\\" + root + ":", mount.device + ":");
    g_mounted[Lower(root)] = mount;
    Log("XamProfileOpen({:016X}, \"{}\", {:X})", xuid, root, flags);
    if (dataPtr)
    {
        std::memset(mem.Translate(dataPtr), 0, 0x200);
        mem.Write32(dataPtr + 0x000, where.device ? where.device : kHardDriveId);
        mem.Write32(dataPtr + 0x004, 0x10000);
        std::memcpy(mem.Translate(dataPtr + 0x108), name.data(), name.size());
        mem.Write64(dataPtr + 0x138, xuid);
        mem.Write32(dataPtr + 0x140, 0xFFFE07D1);
    }
    ctx.r3.u64 = kernel::kErrorSuccess;
}

// DWORD XamContentOpenFileInternal(DWORD UserIndex, LPCSTR RootName, LPCSTR PackagePath, DWORD Flags, DWORD Unknown5,
//     PXCONTENT_DATA_INTERNAL ContentData, PDWORD Unknown7, PXOVERLAPPED Overlapped) (XAM 0x816EE500): a package named
// by its file path (not by XCONTENT_DATA) mounted as "<RootName>:". The wrapper fails a root its resolver rejects
// (0x816EDED8) or a guest user, zeroes *Unknown7, and sends message 0x20002 (0x15C bytes: user, root 0x40, path 0x104,
// Flags, Unknown5, ContentData, Unknown7, overlapped) to the content server, FUN_81682BB8:
//  - the link is "\??\<root>:"; a root not starting with "\??\" / "\System??\" is mounted read-only (FUN_81682860's
//    mount flag 1; INFER from its use there: the dash only reads these packages);
//  - with ContentData: +0 the path's device, +4 the package's content type, +8 its display name (0x80 bytes),
//    +0x108 the path's last component (0x2A bytes), +0x138 the caller's XUID (user < 4, else 0), +0x140 the package's
//    title ID (type 0x9000 adds +0x1D8..+0x1F0).
// Packages here are folders (content/<XUID>/<title>/<type>/<name>, XamContentResolveInternal's paths); other paths and
// a missing package stop with specifics (XAM's errors for them come from its STFS open, FUN_81680128: not read yet).
// The dash: NielsenSound / NUIPACK / PAMFETCH (only after XamPackageManagerFindPackageContainingIndexedXEX finds one:
// never here), the current theme, NXEART on a disc.
namespace
{
    void ContentOpenFile(PPCContext& ctx, uint32_t user, uint32_t rootPtr, uint32_t pathPtr, uint32_t flags,
                         uint32_t dataPtr, uint32_t overlapped)
    {
        auto& mem = guest::g_memory;
        const std::string root = rootPtr ? std::string(reinterpret_cast<const char*>(mem.Translate(rootPtr))) : "";
        const std::string path = pathPtr ? std::string(reinterpret_cast<const char*>(mem.Translate(pathPtr))) : "";
        if (root.empty() || root.size() >= 0x40)
        {
            ctx.r3.u64 = Finish(overlapped, kErrorInvalidName); // the wrapper's root check (0x816EDED8)
            return;
        }
        // The wrapper also fails a guest user with ERROR_ACCESS_DENIED; this runtime signs in no guests.
        if (Lower(root).starts_with("\\??\\") || Lower(root).starts_with("\\system??\\"))
            Fatal("XamContentOpenFile: root \"{}\" with a prefix (writable mounts are not established)", root);
        if (flags & 0x0C000000)
            Fatal("XamContentOpenFile(\"{}\"): flags {:X} (0x4000000 / 0x8000000 change FUN_81682860's path)", root,
                  flags);

        const std::string prefix = "\\device\\harddisk0\\partition1\\content\\";
        if (!Lower(path).starts_with(prefix))
            Fatal("XamContentOpenFile(\"{}\", \"{}\"): only hard drive content packages are supported", root, path);
        std::vector<std::string> parts;
        for (size_t at = prefix.size(), next; at <= path.size(); at = next + 1)
        {
            next = path.find('\\', at);
            if (next == std::string::npos)
                next = path.size();
            parts.push_back(path.substr(at, next - at));
        }
        if (parts.size() != 4 || parts[3].empty())
            Fatal("XamContentOpenFile(\"{}\", \"{}\"): not <XUID>\\<title>\\<type>\\<name>", root, path);
        const fs::path package = ContentRoot() / parts[0] / parts[1] / parts[2] / EscapeName(parts[3]);
        if (!fs::is_directory(package))
            Fatal("XamContentOpenFile(\"{}\", \"{}\"): no package {} (XAM's error not established)", root, path,
                  package.string());
        const auto header = ReadHeader(package);
        if (!header)
            Fatal("XamContentOpenFile(\"{}\"): package {} has no readable header", root, package.string());

        std::lock_guard lock(g_contentMutex);
        if (g_mounted.contains(Lower(root)))
            Fatal("XamContentOpenFile: root \"{}\" is already mounted (behaviour not established)", root);
        const std::string device = std::format("Content{}", g_nextDevice++);
        vfs::MountDevice(device, package, true);
        vfs::CreateLink("\\??\\" + root + ":", device + ":");
        g_mounted[Lower(root)] = { root, device, package };
        Log("XamContentOpenFile(\"{}\", {}): {}", root, path, package.string());

        if (dataPtr)
        {
            const uint32_t type = GetBe32(*header, 0x344);
            if (type == 0x9000)
                Fatal("XamContentOpenFile(\"{}\"): type 0x9000 fields +0x1D8..+0x1F0 are not implemented", root);
            std::vector<uint8_t> out(mem.Translate(dataPtr), mem.Translate(dataPtr) + 0x200);
            PutBe32(out, 0x000, kHardDriveId);
            PutBe32(out, 0x004, type);
            std::fill(out.begin() + 8, out.begin() + 0x88, uint8_t(0));
            const std::u16string name = DisplayName(*header);
            for (size_t i = 0; i < name.size() && i < 63; i++)
            {
                out[8 + i * 2] = uint8_t(name[i] >> 8);
                out[8 + i * 2 + 1] = uint8_t(name[i]);
            }
            std::fill(out.begin() + 0x108, out.begin() + 0x108 + 0x2A, uint8_t(0));
            std::memcpy(out.data() + 0x108, parts[3].data(), std::min<size_t>(parts[3].size(), 0x2A));
            PutBe64(out, 0x138, user < 4 && kernel::UserSignedIn(user) ? kernel::users::Xuid(user) : 0);
            PutBe32(out, 0x140, GetBe32(*header, 0x360));
            std::memcpy(mem.Translate(dataPtr), out.data(), out.size());
        }
        ctx.r3.u64 = Finish(overlapped, kernel::kErrorSuccess);
    }
}

GUEST_IMPORT(XamContentOpenFileInternal)
{
    if (ctx.r9.u32)
        guest::g_memory.Write32(ctx.r9.u32, 0);
    ContentOpenFile(ctx, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r8.u32, ctx.r10.u32);
}

// DWORD XamContentOpenFile(DWORD UserIndex, LPCSTR RootName, LPCSTR PackagePath, DWORD Flags, DWORD Unknown5,
//     PDWORD Unknown6, PXOVERLAPPED Overlapped) (XAM 0x816EFD80): XamContentOpenFileInternal(r3..r7, NULL, r8, r9).
GUEST_IMPORT(XamContentOpenFile)
{
    if (ctx.r8.u32)
        guest::g_memory.Write32(ctx.r8.u32, 0);
    ContentOpenFile(ctx, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, 0, ctx.r9.u32);
}

// DWORD XamContentClose(LPCSTR RootName, PXOVERLAPPED Overlapped): commits and unmounts. An unknown root fails with
// ERROR_FILE_NOT_FOUND (Xenia's code; TU46's storage manager ignores the result).
GUEST_IMPORT(XamContentClose)
{
    auto& mem = guest::g_memory;
    const std::string root = ctx.r3.u32 ? std::string(reinterpret_cast<const char*>(mem.Translate(ctx.r3.u32))) : "";
    std::lock_guard lock(g_contentMutex);
    const auto it = g_mounted.find(Lower(root));
    if (it == g_mounted.end())
    {
        ctx.r3.u64 = Finish(ctx.r4.u32, kErrorFileNotFound);
        return;
    }
    it->second.closing = true;
    CommitMount(it->second);
    vfs::DeleteLink("\\??\\" + it->second.root + ":");
    vfs::UnmountDevice(it->second.device);
    g_mounted.erase(it);
    ctx.r3.u64 = Finish(ctx.r4.u32, kernel::kErrorSuccess);
}

// DWORD XamProfileClose(LPCSTR RootName): XamContentClose(RootName, NULL).
GUEST_IMPORT(XamProfileClose)
{
    ctx.r4.u64 = 0;
    XamContentClose_impl(ctx, base);
}

// DWORD XamContentFlush(LPCSTR RootName, PXOVERLAPPED Overlapped): commits without unmounting. Files are written
// through to the host, so this records the package size.
GUEST_IMPORT(XamContentFlush)
{
    auto& mem = guest::g_memory;
    const std::string root = ctx.r3.u32 ? std::string(reinterpret_cast<const char*>(mem.Translate(ctx.r3.u32))) : "";
    std::lock_guard lock(g_contentMutex);
    const auto it = g_mounted.find(Lower(root));
    if (it == g_mounted.end())
    {
        ctx.r3.u64 = Finish(ctx.r4.u32, kErrorFileNotFound);
        return;
    }
    CommitMount(it->second);
    ctx.r3.u64 = Finish(ctx.r4.u32, kernel::kErrorSuccess);
}

namespace
{
    // The item XAM's change callback gets for a deleted package: 0x8167C4C0 parses it from the package's path into
    // a zeroed 0x200-byte item (device, type +4, file name +0x108, XUID +0x138, title +0x140; nothing from the header,
    // which is gone).
    std::vector<uint8_t> DeletedItem(uint32_t device, uint64_t xuid, uint32_t title, uint32_t type,
                                     const std::string& fileName)
    {
        std::vector<uint8_t> item(0x200, 0);
        PutBe32(item, 0x000, device);
        PutBe32(item, 0x004, type);
        std::memcpy(item.data() + 0x108, fileName.data(), std::min(fileName.size(), kFileNameLength));
        PutBe64(item, 0x138, xuid);
        PutBe32(item, 0x140, title);
        return item;
    }

    // XAM's package delete (0x81685D18): XamDeleteFile on Content\<XUID>\<Title>\<Type>\<FileName> (its error, e.g.
    // 2 when there is no such package), then the change callback with 1 (deleted), then the package's ".data" folder
    // (none here). A package in use cannot be deleted (Xenia's code 5). A package generated from the games folder
    // also takes its game's folder with it (kernel/game_import.h). Returns the Win32 error; `deleted` gets the item
    // to notify with once the content lock is released (the callback may call content functions).
    uint32_t DeletePackage(uint32_t device, uint64_t xuid, uint32_t title, uint32_t type, const std::string& fileName,
                           std::optional<std::vector<uint8_t>>& deleted)
    {
        if (fileName.empty())
            return kErrorInvalidParameter;
        // 0x8167EFC8: a file name starting with a byte >= 0x80 is common content; profiles and 0x40000 content
        // belong to the dashboard.
        if (uint8_t(fileName[0]) >= 0x80)
            xuid = 0;
        if (type == kTypeProfile || type == 0x40000)
            title = 0xFFFE07D1;
        const fs::path package = PackageOn(device, xuid, title, type, fileName);
        std::lock_guard lock(g_contentMutex);
        std::error_code ec;
        if (!fs::exists(package, ec))
            return kErrorFileNotFound;
        const bool stfs = IsStfsPackage(package);
        if (IsMounted(package))
            return kErrorAccessDenied;
        kernel::stfs_mount::WaitForWrites(package);
        fs::path marker = HeaderPath(package);
        marker.replace_extension(".recomp");
        if (fs::is_regular_file(marker, ec))
            kernel::games::RemoveGameFolder(marker);
        fs::remove_all(package, ec);
        if (ec)
        {
            Log("XamContentDelete: {}: {}", package.string(), ec.message());
            return kErrorAccessDenied;
        }
        fs::remove(HeaderPath(package), ec);
        fs::remove(marker, ec);
        if (stfs)
            fs::remove_all(package.string() + ".data", ec); // XAM removes the data folder after the package (0x81685D18)
        Log("Content: deleted {}", package.string());
        deleted = DeletedItem(device, xuid, title, type, fileName);
        return kernel::kErrorSuccess;
    }

    // A deleted profile leaves the console's profile list (XAM enumerates profile packages; the runtime keeps a list,
    // kernel/users.h) and is not copied again from the profiles folder at the next start.
    void ForgetProfile(uint64_t xuid)
    {
        std::string gamertag;
        for (const auto& p : kernel::users::Profiles())
            if (p.xuid == xuid)
                gamertag = p.gamertag;
        kernel::profile::RecordDeleted(xuid, gamertag);
        kernel::users::RemoveProfile(xuid);
    }

    // After DeletePackage: the change callback, and for a profile package (System Settings > Storage > Delete Profile
    // Only) the profile's removal from the list.
    void Deleted(PPCContext& ctx, const std::vector<uint8_t>& item)
    {
        if (GetBe32(item, 4) == kTypeProfile && GetBe32(item, 0x140) == 0xFFFE07D1)
        {
            const std::string name(reinterpret_cast<const char*>(item.data() + 0x108),
                                   strnlen(reinterpret_cast<const char*>(item.data() + 0x108), kFileNameLength));
            uint64_t xuid = 0;
            const auto [end, error] = std::from_chars(name.data(), name.data() + name.size(), xuid, 16);
            // Still on another device (a move copies before it deletes): the profile stays, now there.
            kernel::profile_location::Forget(xuid);
            if (name.size() == 16 && error == std::errc() && end == name.data() + name.size() &&
                kernel::profile_location::Find(xuid).device == 0)
                ForgetProfile(xuid);
        }
        NotifyContentChange(ctx, 1, item);
    }
}

// DWORD XamContentDelete(DWORD UserIndex, const XCONTENT_DATA* ContentData, PXOVERLAPPED Overlapped) (XAM 0x816EE788
// -> 0x816EE6D0, message 0x20004 with the 0x134-byte item): the package of the user's XUID and the running title.
GUEST_IMPORT(XamContentDelete)
{
    const uint32_t user = ctx.r3.u32, overlapped = ctx.r5.u32;
    if (ctx.r4.u32 == 0)
    {
        ctx.r3.u64 = Finish(overlapped, kErrorInvalidParameter);
        return;
    }
    const ContentData data = ReadContentData(ctx.r4.u32);
    uint64_t xuid = 0;
    uint32_t error = kernel::storage::Find(data.deviceId) ? Owner(user, data.type, xuid) : kErrorDeviceNotConnected;
    std::optional<std::vector<uint8_t>> deleted;
    if (error == kernel::kErrorSuccess)
        error = DeletePackage(data.deviceId, xuid, kernel::TitleId(), data.type, data.fileName, deleted);
    if (deleted)
        Deleted(ctx, *deleted);
    ctx.r3.u64 = Finish(overlapped, error);
}

// DWORD XamContentDeleteInternal(const XCONTENT_DATA_INTERNAL* ContentData, PXOVERLAPPED Overlapped) (XAM 0x816EE798
// -> 0x816EE6D0 with user 0xFE and the 0x200-byte item): any owner's package, XUID at 0x138, title at 0x140 (0: the
// running title's). System Settings > Storage deletes with it.
GUEST_IMPORT(XamContentDeleteInternal)
{
    auto& mem = guest::g_memory;
    const uint32_t dataPtr = ctx.r3.u32, overlapped = ctx.r4.u32;
    if (dataPtr == 0)
    {
        ctx.r3.u64 = Finish(overlapped, kErrorInvalidParameter);
        return;
    }
    const ContentData data = ReadContentData(dataPtr);
    uint32_t title = mem.Read32(dataPtr + 0x140);
    if (title == 0)
        title = kernel::TitleId();
    std::optional<std::vector<uint8_t>> deleted;
    const uint32_t error = !kernel::storage::Find(data.deviceId) ? kErrorDeviceNotConnected
        : DeletePackage(data.deviceId, mem.Read64(dataPtr + 0x138), title, data.type, data.fileName, deleted);
    if (deleted)
        Deleted(ctx, *deleted);
    ctx.r3.u64 = Finish(overlapped, error);
}

// HRESULT XamProfileDelete(DWORD DeviceId, XUID Xuid) (XAM 0x816DB078 -> message 0x23006 -> 0x816DC598,
// synchronous): clears XCONFIG_USER_DEFAULT_PROFILE if it names the profile, then deletes everything the profile owns
// on the device (DeviceId 0: every device): the whole Content\<XUID>\ tree (0x81686870 -> 0x81685D18 with flag
// 0x200, 0x816855F0), each deleted file reported to the change callback with 1; a missing tree is no error. XAM
// does not sign the profile out. Here each package (folder) is one notification, and the deletion is remembered so
// the profile is not copied again from the profiles folder at the next start (kernel/profile_import.h).
GUEST_IMPORT(XamProfileDelete)
{
    const uint32_t device = ctx.r3.u32;
    const uint64_t xuid = ctx.r4.u64;
    if (device != 0 && !kernel::storage::Find(device))
    {
        ctx.r3.u64 = 0x80070000 | kErrorDeviceNotConnected; // 0x8167A560: no such device
        return;
    }
    if (kernel::XConfigRead(3, 0x08) == std::vector<uint8_t>{ uint8_t(xuid >> 56), uint8_t(xuid >> 48),
            uint8_t(xuid >> 40), uint8_t(xuid >> 32), uint8_t(xuid >> 24), uint8_t(xuid >> 16), uint8_t(xuid >> 8),
            uint8_t(xuid) })
        kernel::XConfigWrite(3, 0x08, std::vector<uint8_t>(8, 0));
    std::vector<std::vector<uint8_t>> deleted;
    for (const kernel::storage::Device& dev : kernel::storage::Devices())
    {
        if (device != 0 && dev.id != device)
            continue;
        std::lock_guard lock(g_contentMutex);
        const fs::path owner = DeviceRoot(dev.id) / std::format("{:016X}", xuid);
        std::error_code ec;
        if (!fs::is_directory(owner, ec))
            continue;
        for (const auto& [root, m] : g_mounted)
            if (const fs::path rel = m.package.lexically_relative(owner); !rel.empty() && *rel.begin() != "..")
                Fatal("XamProfileDelete({:016X}): its package {} is in use (behaviour not established)", xuid,
                      m.package.string());
        auto hex = [](const fs::path& name) -> std::optional<uint32_t> {
            const std::string s = name.filename().string();
            uint32_t value = 0;
            const auto [end, error] = std::from_chars(s.data(), s.data() + s.size(), value, 16);
            if (s.size() != 8 || error != std::errc() || end != s.data() + s.size())
                return std::nullopt;
            return value;
        };
        for (const auto& title : fs::directory_iterator(owner, ec))
            for (const auto& type : fs::directory_iterator(title.path(), ec))
            {
                const auto titleId = hex(title.path()), typeId = hex(type.path()); // not Headers
                if (!titleId || !typeId)
                    continue;
                for (const auto& package : fs::directory_iterator(type.path(), ec))
                    if (package.path().extension() != ".data")
                        deleted.push_back(DeletedItem(dev.id, xuid, *titleId, *typeId,
                                                      UnescapeName(package.path().filename().string())));
            }
        const auto removed = fs::remove_all(owner, ec);
        if (ec)
            Fatal("XamProfileDelete({:016X}): removing {} failed: {}", xuid, owner.string(), ec.message());
        Log("Profiles: {:016X} deleted with its items: {} files and folders in {}", xuid, removed, owner.string());
    }
    kernel::profile_location::Forget(xuid);
    if (kernel::profile_location::Find(xuid).device == 0)
        ForgetProfile(xuid); // gone from every device
    for (const auto& item : deleted)
        NotifyContentChange(ctx, 1, item);
    ctx.r3.u64 = 0;
}

// DWORD XamContentGetCreator(DWORD UserIndex, const XCONTENT_DATA* ContentData, PBOOL IsCreator, PXUID Creator,
//                            PXOVERLAPPED Overlapped): from the creator XUID stored in the package header.
GUEST_IMPORT(XamContentGetCreator)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, isCreatorPtr = ctx.r5.u32, creatorPtr = ctx.r6.u32, overlapped = ctx.r7.u32;
    if (ctx.r4.u32 == 0)
    {
        ctx.r3.u64 = Finish(overlapped, kErrorInvalidParameter);
        return;
    }
    const ContentData data = ReadContentData(ctx.r4.u32);
    uint64_t xuid = 0;
    if (const uint32_t error = kernel::storage::Find(data.deviceId) ? Owner(user, data.type, xuid) : kErrorDeviceNotConnected)
    {
        ctx.r3.u64 = Finish(overlapped, error);
        return;
    }
    std::lock_guard lock(g_contentMutex);
    const auto header = ReadHeader(PackagePath(xuid, data));
    if (!header)
    {
        ctx.r3.u64 = Finish(overlapped, kErrorPathNotFound); // Xenia's code
        return;
    }
    const uint64_t creator = GetBe64(*header, kProfileId);
    const uint64_t caller = user < 4 ? kernel::users::Xuid(user) : 0;
    if (isCreatorPtr)
        mem.Write32(isCreatorPtr, creator == caller ? 1 : 0);
    if (creatorPtr)
        mem.Write64(creatorPtr, creator);
    ctx.r3.u64 = Finish(overlapped, kernel::kErrorSuccess);
}

// DWORD XamContentGetThumbnail(DWORD UserIndex, const XCONTENT_DATA* ContentData, PBYTE Thumbnail,
//                              PDWORD ThumbnailBytes, PXOVERLAPPED Overlapped)
// A NULL buffer asks for the size.
GUEST_IMPORT(XamContentGetThumbnail)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, buffer = ctx.r5.u32, sizePtr = ctx.r6.u32, overlapped = ctx.r7.u32;
    if (ctx.r4.u32 == 0 || sizePtr == 0)
    {
        ctx.r3.u64 = Finish(overlapped, kErrorInvalidParameter);
        return;
    }
    const ContentData data = ReadContentData(ctx.r4.u32);
    uint64_t xuid = 0;
    if (const uint32_t error = kernel::storage::Find(data.deviceId) ? Owner(user, data.type, xuid) : kErrorDeviceNotConnected)
    {
        ctx.r3.u64 = Finish(overlapped, error);
        return;
    }
    std::lock_guard lock(g_contentMutex);
    const auto header = ReadHeader(PackagePath(xuid, data));
    if (!header)
    {
        ctx.r3.u64 = Finish(overlapped, kErrorFileNotFound);
        return;
    }
    const uint32_t size = std::min(GetBe32(*header, kThumbnailSize), kThumbnailMax);
    if (size == 0)
    {
        ctx.r3.u64 = Finish(overlapped, kErrorFileNotFound);
        return;
    }
    if (buffer == 0)
    {
        mem.Write32(sizePtr, size);
        ctx.r3.u64 = Finish(overlapped, kernel::kErrorSuccess);
        return;
    }
    if (mem.Read32(sizePtr) < size)
    {
        mem.Write32(sizePtr, size);
        ctx.r3.u64 = Finish(overlapped, kErrorInsufficientBuffer);
        return;
    }
    std::memcpy(mem.Translate(buffer), header->data() + kThumbnail, size);
    mem.Write32(sizePtr, size);
    ctx.r3.u64 = Finish(overlapped, kernel::kErrorSuccess);
}

namespace
{
    // Replaces a package's thumbnail (header 0x1712 / 0x171A). Returns the Win32 result.
    uint32_t SetThumbnail(const fs::path& package, uint32_t image, uint32_t size)
    {
        std::lock_guard lock(g_contentMutex);
        auto header = ReadHeader(package);
        if (!header)
            return kErrorFileNotFound;
        std::memset(header->data() + kThumbnail, 0, kThumbnailMax);
        if (size)
            std::memcpy(header->data() + kThumbnail, guest::g_memory.Translate(image), size);
        PutBe32(*header, kThumbnailSize, size);
        WriteHeader(package, *header);
        return kernel::kErrorSuccess;
    }
}

// DWORD XamContentSetThumbnail(DWORD UserIndex, const XCONTENT_DATA* ContentData, const BYTE* Thumbnail,
//                              DWORD ThumbnailBytes, PXOVERLAPPED Overlapped): at most 15,616 bytes (0x3D00).
GUEST_IMPORT(XamContentSetThumbnail)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, image = ctx.r5.u32, size = ctx.r6.u32, overlapped = ctx.r7.u32;
    if (ctx.r4.u32 == 0 || (image == 0 && size != 0) || size > kThumbnailMax)
    {
        ctx.r3.u64 = Finish(overlapped, kErrorInvalidParameter);
        return;
    }
    const ContentData data = ReadContentData(ctx.r4.u32);
    uint64_t xuid = 0;
    if (const uint32_t error = kernel::storage::Find(data.deviceId) ? Owner(user, data.type, xuid) : kErrorDeviceNotConnected)
    {
        ctx.r3.u64 = Finish(overlapped, error);
        return;
    }
    ctx.r3.u64 = Finish(overlapped, SetThumbnail(PackagePath(xuid, data), image, size));
}

// DWORD XamContentGetAttributesInternal(const XCONTENT_DATA_INTERNAL* Data, WIN32_FILE_ATTRIBUTE_DATA* Attributes,
//     PXOVERLAPPED Overlapped) (XAM 0x816EE9D0 -> message 0x2001A -> 0x81684688(.., 1)): the package file's
// NtQueryFullAttributesFile data as WIN32_FILE_ATTRIBUTE_DATA (attributes +0, creation +4, last access +0xC, last
// write +0x14, size high/low +0x1C / +0x20), the size plus the header's data-file total (0x3A1) for game types. A
// missing package is ERROR_FILE_NOT_FOUND. Packages are folders here: attributes FILE_ATTRIBUTE_NORMAL as for files
// (xboxkrnl_io.cpp), the folder's times, the folder's file total.
GUEST_IMPORT(XamContentGetAttributesInternal)
{
    auto& mem = guest::g_memory;
    const uint32_t dataPtr = ctx.r3.u32, out = ctx.r4.u32, overlapped = ctx.r5.u32;
    if (dataPtr == 0 || out == 0)
        Fatal("XamContentGetAttributesInternal: NULL argument");
    const ContentData data = ReadContentData(dataPtr);
    if (!kernel::storage::Find(data.deviceId))
    {
        ctx.r3.u64 = Finish(overlapped, kErrorDeviceNotConnected);
        return;
    }
    const fs::path package = PackageOn(data.deviceId, mem.Read64(dataPtr + 0x138), mem.Read32(dataPtr + 0x140),
                                       data.type, data.fileName);
    std::lock_guard lock(g_contentMutex);
    WIN32_FILE_ATTRIBUTE_DATA attributes{};
    if (!GetFileAttributesExW(package.c_str(), GetFileExInfoStandard, &attributes))
    {
        ctx.r3.u64 = Finish(overlapped, kErrorFileNotFound);
        return;
    }
    uint64_t size = PackageBytes(package);
    const uint32_t type = data.type;
    if ((type & 0xFFF00000) || type == 0x7000 || type == 0x4000 || type == 0xD0000 || type == 0x80000 || type == 2 ||
        type == 0x5000)
    {
        const auto header = ReadHeader(package);
        if (header)
            size += GetBe64(*header, 0x3A1);
        else
            Fatal("XamContentGetAttributesInternal: {} has no readable header (XAM then sums Data#### files)",
                  package.string());
    }
    auto time = [](const FILETIME& t) { return uint64_t(t.dwHighDateTime) << 32 | t.dwLowDateTime; };
    mem.Write32(out + 0x00, 0x80);
    mem.Write64(out + 0x04, time(attributes.ftCreationTime));
    mem.Write64(out + 0x0C, time(attributes.ftLastAccessTime));
    mem.Write64(out + 0x14, time(attributes.ftLastWriteTime));
    mem.Write32(out + 0x1C, uint32_t(size >> 32));
    mem.Write32(out + 0x20, uint32_t(size));
    ctx.r3.u64 = Finish(overlapped, kernel::kErrorSuccess);
}

// DWORD XamContentSetThumbnailInternal(const XCONTENT_DATA_INTERNAL* Data, const BYTE* Thumbnail, DWORD ThumbnailBytes)
// (XAM 0x816EF548 -> 0x816EF470(0xFE, Data, 0x200, ...)): XamContentSetThumbnail for the package Data names (XUID at
// 0x138, title at 0x140), synchronously.
GUEST_IMPORT(XamContentSetThumbnailInternal)
{
    auto& mem = guest::g_memory;
    const uint32_t dataPtr = ctx.r3.u32, image = ctx.r4.u32, size = ctx.r5.u32;
    if (dataPtr == 0 || (image == 0 && size != 0) || size > kThumbnailMax)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    const ContentData data = ReadContentData(dataPtr);
    if (!kernel::storage::Find(data.deviceId))
    {
        ctx.r3.u64 = kErrorDeviceNotConnected;
        return;
    }
    const fs::path package = PackageOn(data.deviceId, mem.Read64(dataPtr + 0x138), mem.Read32(dataPtr + 0x140),
                                       data.type, data.fileName);
    ctx.r3.u64 = SetThumbnail(package, image, size);
}

// VOID XamContentRegisterChangeCallback(PFN Callback) (XAM 0x816EE080 -> 0x8167CCC0): one global callback (NULL
// unregisters), called as Callback(type, item) by XAM's content operations: 0 content added (create, copy, move), 1 deleted, 2
// package updated (NotifyContentChange). The runtime calls it after creating a package; deleting (XamContentDelete)
// must call it with 1 once the dashboard deletes content.
GUEST_IMPORT(XamContentRegisterChangeCallback)
{
    g_contentChangeCallback = ctx.r3.u32;
}

// DWORD XamContentCopyInternal / XamContentMoveInternal(const XUID* NewOwner, const XCONTENTDEVICEID* DestDevice,
//     const XCONTENT_DATA_INTERNAL* Item, LPPROGRESS_ROUTINE Progress, PVOID Context, DWORD Unknown6,
//     PXOVERLAPPED Overlapped) (XAM 0x816EEB68 / 0x816EEB70 -> worker 0x816EEA78 with flags 0 / 1, message 0x2000C,
//     handler FUN_81686248). What the handler does, in order (SHOWS):
//  - the source device must exist; the package's transfer flags (header 0x1711) decide: a destination device needs
//    flag 0x40 (device transfer), or 0x20 (move only) for a move; a new owner needs 0x80 (profile transfer); else
//    E_ACCESSDENIED (0x80070005). (ExExpansionCall 0x50563033 can also allow it: not on a retail console's dash.)
//  - the destination needs room for the package (else 0x80070070, ERROR_DISK_FULL);
//  - the package file is copied in 64 KiB chunks (0x8171BA28) with Progress(Total, Done, Total, Done, 1, Reason,
//    hSource, hDest, Context) first with Reason 1 (CALLBACK_STREAM_SWITCH), then 0 after each chunk: 0 continues,
//    3 stops the calls, anything else aborts (ERROR_REQUEST_ABORTED, the copy deleted); then <package>.data\DataNNNN
//    when the metadata counts data files (0x39D); the content-change callback "added"; a move deletes the source.
// Packages copy as they are (a byte copy: signed packages keep their signatures). A folder package (the hard drive's
// own format) becomes an STFS package file on the other device (unsigned CON: kernel/stfs_mount.h).
// Not implemented (stop with specifics): a new owner other than the item's, move-only content (XAM records the
// destination's serial in the header), a copy onto the same device.
namespace
{
    constexpr uint32_t kTransferProfile = 0x80, kTransferDevice = 0x40, kTransferMoveOnly = 0x20;
    constexpr uint32_t kHresultAccessDenied = 0x80070005, kHresultDiskFull = 0x80070070;
    constexpr uint32_t kHresultDeviceNotConnected = 0x8007048F, kHresultFileNotFound = 0x80070002;
    constexpr uint32_t kHresultRequestAborted = 0x800704D3;

    // The guest's progress routine; returns its answer.
    uint32_t CallProgress(PPCContext& ctx, uint32_t routine, uint32_t context, uint64_t total, uint64_t done,
                          uint32_t reason)
    {
        PPCContext call = ctx;
        const uint32_t frame = (ctx.r1.u32 - 0x400) & ~0xFu;
        call.r1.u64 = frame;
        call.r3.u64 = total;
        call.r4.u64 = done;
        call.r5.u64 = total;
        call.r6.u64 = done;
        call.r7.u64 = 1;
        call.r8.u64 = reason;
        call.r9.u64 = 0;  // hSourceFile: not a guest handle here
        call.r10.u64 = 0; // hDestinationFile
        guest::g_memory.Write32(frame + 0x54, context); // ninth argument
        kernel::CallGuest(call, routine);
        return call.r3.u32;
    }

    // One file copied as 0x8171BA28 does, with its progress calls. Returns an HRESULT.
    uint32_t CopyWithProgress(PPCContext& ctx, const fs::path& from, const fs::path& to, uint32_t& routine,
                              uint32_t context)
    {
        kernel::stfs_mount::WaitForWrites(from);
        kernel::stfs_mount::WaitForWrites(to);
        std::ifstream in(from, std::ios::binary);
        std::error_code ec;
        const uint64_t total = fs::file_size(from, ec);
        if (!in || ec)
            return kHresultFileNotFound;
        fs::create_directories(to.parent_path(), ec);
        std::ofstream out(to, std::ios::binary | std::ios::trunc);
        if (!out)
            return kHresultAccessDenied;
        auto ask = [&](uint64_t done, uint32_t reason) {
            if (!routine)
                return true;
            const uint32_t answer = CallProgress(ctx, routine, context, total, done, reason);
            if (answer == 3)
                routine = 0; // PROGRESS_QUIET
            return answer == 0 || answer == 3;
        };
        bool ok = ask(0, 1);
        std::vector<char> chunk(0x10000);
        uint64_t done = 0;
        while (ok && done < total)
        {
            in.read(chunk.data(), std::streamsize(chunk.size()));
            const auto got = in.gcount();
            if (got <= 0)
                break;
            out.write(chunk.data(), got);
            done += uint64_t(got);
            ok = ask(done, 0);
        }
        out.close();
        if (!ok || done != total || !out)
        {
            fs::remove(to, ec);
            return ok ? kHresultAccessDenied : kHresultRequestAborted;
        }
        return 0;
    }

    void ContentCopyMove(PPCContext& ctx, bool move)
    {
        auto& mem = guest::g_memory;
        const uint32_t ownerPtr = ctx.r3.u32, destPtr = ctx.r4.u32, itemPtr = ctx.r5.u32, routineArg = ctx.r6.u32;
        const uint32_t context = ctx.r7.u32, unknown6 = ctx.r8.u32, overlapped = ctx.r9.u32;
        const char* op = move ? "XamContentMoveInternal" : "XamContentCopyInternal";
        if (ownerPtr == 0 && destPtr == 0)
            Fatal("{}: neither a new owner nor a destination (XAM sends nothing: its result is not established)", op);
        if (unknown6 != 0)
            Fatal("{}: sixth argument {:X} (its use is not established)", op, unknown6);
        if (itemPtr == 0)
            Fatal("{}: no content item", op);
        const ContentData data = ReadContentData(itemPtr);
        const uint64_t xuid = mem.Read64(itemPtr + 0x138);
        const uint32_t title = mem.Read32(itemPtr + 0x140);

        auto finish = [&](uint32_t hr) {
            if (overlapped)
            {
                kernel::CompleteOverlapped(overlapped, int32_t(hr) < 0 ? 0x65B : 0, hr, 0);
                ctx.r3.u64 = kernel::kErrorIoPending;
                return;
            }
            ctx.r3.u64 = int32_t(hr) >= 0 ? 0 : (hr & 0x1FFF0000) == 0x70000 ? (hr & 0xFFFF) : 0x65B;
        };

        if (!kernel::storage::Find(data.deviceId))
            return finish(kHresultDeviceNotConnected);
        const fs::path source = PackageOn(data.deviceId, xuid, title, data.type, data.fileName);
        const auto header = ReadHeader(source);
        if (!header)
            return finish(kHresultFileNotFound);
        const uint8_t transfer = (*header)[kFlags];
        const uint32_t destId = destPtr ? mem.Read32(destPtr) : data.deviceId;
        if (destPtr && !(transfer & kTransferDevice) && !(move && (transfer & kTransferMoveOnly)))
        {
            Log("{}: {}: transfer flags {:02X} do not allow another device", op, source.string(), transfer);
            return finish(kHresultAccessDenied);
        }
        if (ownerPtr)
        {
            if (!(transfer & kTransferProfile))
                return finish(kHresultAccessDenied);
            if (mem.Read64(ownerPtr) != xuid)
                Fatal("{}: new owner {:016X} for {:016X}'s package (profile transfer is not implemented)", op,
                      mem.Read64(ownerPtr), xuid);
        }
        if (move && (transfer & kTransferMoveOnly) && !(transfer & kTransferDevice))
            Fatal("{}: move-only package {} (XAM writes the destination's serial into its header: not implemented)",
                  op, source.string());
        const auto dest = kernel::storage::Find(destId);
        if (!dest)
            return finish(kHresultDeviceNotConnected);
        if (destId == data.deviceId)
            Fatal("{}: {} onto its own device {} (not established)", op, source.string(), destId);
        const uint64_t bytes = PackageBytes(source);
        ULARGE_INTEGER freeBytes{};
        GetDiskFreeSpaceExW(dest->volume.c_str(), &freeBytes, nullptr, nullptr);
        if (bytes >= freeBytes.QuadPart)
            return finish(kHresultDiskFull);

        const fs::path target = PackageOn(destId, xuid, title, data.type, data.fileName);
        uint32_t routine = routineArg;
        uint32_t hr = 0;
        std::error_code ec;
        if (IsStfsPackage(source))
        {
            hr = CopyWithProgress(ctx, source, target, routine, context);
            const fs::path dataFolder = source.string() + ".data";
            if (hr == 0 && GetBe32(*header, kDataFileCount) != 0 && fs::is_directory(dataFolder, ec))
                for (const auto& entry : fs::directory_iterator(dataFolder, ec))
                    if (hr == 0)
                        hr = CopyWithProgress(ctx, entry.path(), fs::path(target.string() + ".data") /
                                              entry.path().filename(), routine, context);
        }
        else
        {
            // The hard drive's folder package written as an STFS package file on the destination, with the
            // progress routine told at the start and the end.
            if (routine)
            {
                const uint32_t answer = CallProgress(ctx, routine, context, bytes, 0, 1);
                if (answer == 3)
                    routine = 0;
                else if (answer != 0)
                    hr = kHresultRequestAborted;
            }
            std::string error;
            if (hr == 0 && !kernel::stfs_mount::FromFolder(source, *header, target, &error))
            {
                Log("{}: {} -> {}: {}", op, source.string(), target.string(), error);
                hr = kHresultAccessDenied;
            }
            if (hr == 0 && routine)
                CallProgress(ctx, routine, context, bytes, bytes, 0);
        }
        if (hr != 0)
        {
            fs::remove(target, ec);
            fs::remove_all(target.string() + ".data", ec);
            Log("{}: {} -> device {}: {:08X}", op, source.string(), destId, hr);
            return finish(hr);
        }
        Log("{}: {} -> {} (device {})", op, source.string(), target.string(), destId);
        if (const auto copied = ReadHeader(target))
        {
            std::vector<uint8_t> item = ContentItem(target, *copied, xuid, title, data.type);
            PutBe32(item, 0, destId);
            NotifyContentChange(ctx, 0, item);
        }
        if (move)
        {
            std::optional<std::vector<uint8_t>> deleted;
            if (const uint32_t error = DeletePackage(data.deviceId, xuid, title, data.type, data.fileName, deleted))
                Log("{}: the source {} stays (delete: {:X})", op, source.string(), error);
            if (deleted)
                Deleted(ctx, *deleted);
        }
        finish(0);
    }
}

GUEST_IMPORT(XamContentCopyInternal)
{
    ContentCopyMove(ctx, false);
}

GUEST_IMPORT(XamContentMoveInternal)
{
    ContentCopyMove(ctx, true);
}

// XAM exports both under ordinals 625 and 633 (the dash imports them; a title may look them up with
// XexGetProcedureAddress): the same functions, the dynamic lookup binds the imports above.
PPC_FUNC(__imp__XamContentCreateInternal);
PPC_FUNC(__imp__XamContentAggregateCreateEnumerator);

namespace
{
    kernel::DynamicExportRegistrar g_contentCreateInternal("xam.xex", 625, __imp__XamContentCreateInternal);
    kernel::DynamicExportRegistrar g_contentAggregateCreateEnumerator("xam.xex", 633, __imp__XamContentAggregateCreateEnumerator);
}
