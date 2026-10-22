# Storage devices: what an Xbox 360 USB stick looks like, and the template for other PC drives

Owner's plan (2026-10-15): every PC drive is a storage device in System Settings > Storage, like a console's hard
drive and USB sticks; moving content to another drive gives that drive a hidden folder with the console's own layout;
a USB stick formatted by a real console, plugged into the PC, shows its content as it does on the console.

## A console-formatted USB stick (observed: the user's stick, formatted by a 17559 console, 2026-10-15)
FAT32 (volume label empty), Windows reports it as a removable drive. At the root:

```
G:\name.txt                                  the device's name: UTF-16BE with a BOM (FE FF), no terminator, no
                                             newline: "USB Storage Device" (Storage > device > Rename changes it)
G:\Content\                                  attribute Hidden (the only hidden entry; name.txt is not)
   <Owner XUID, 16 hex>\<Title ID, 8 hex>\<Content type, 8 hex>\<package file>
```

That is the hard drive's `Content\` tree (XAM 0x8167EFC8 builds `Content\<XUID>\<Title>\<Type>\<FileName>` the same on
every device), with these rules:
- **Owner**: `0000000000000000` for content shared by every profile (marketplace content, DLC, caches); a profile's
  ID (offline XUID, E...) for that profile's content (its profile package, saved games).
- **Package = one file**: an STFS container (magic `CON ` for console-signed, `LIVE` / `PIRS` for Microsoft-signed)
  named by its XCONTENT_DATA file name (up to 42 characters; may contain spaces: Minecraft names saves after the
  world, e.g. `Minigame World By HijackAssasin v2.bin`). Marketplace packages are named by a 42-hex-digit hash.
  There is **no `Headers\` folder**: the package's header is the first 0x971A bytes of the file itself.
- Content bigger than FAT32 allows (Games on Demand, installed discs) keeps its data files in a folder beside the
  package: `<package>.data\Data0000, Data0001...` (XAM's delete removes `<package>.data\` after the package,
  0x81685D18).
- Directory names are upper-case hex; the console wrote no other files (Windows added `System Volume Information`).

Observed tree (owner's stick; types: 0x10000 profile, 0x1 saved game, 0x2 marketplace content (skin pack),
0x20000 avatar/marketplace item, 0x40000 cache):
```
Content\0000000000000000\FFFE07DF\00040000\ContentCache.pkg          CON  "Content Cache" (the dash's content cache)
Content\0000000000000000\584D07D1\00040000\asset_refresh_cookie      CON  "Avatar Asset Refresh Cache"
Content\0000000000000000\584111F7\00000002\B3B1A316...8A458          LIVE "Skin Pack 4"
Content\0000000000000000\584111F7\00020000\BF92FD1B...DF858          LIVE "Minecraft Spawn Eggs"
Content\E0000016529FAD89\FFFE07D1\00010000\E0000016529FAD89          CON  profile package (display name = the ID)
Content\E0000016529FAD89\584111F7\00000001\_MinecraftSaveInfo        CON  saved game
Content\E0000016529FAD89\584111F7\00000001\Minigame World By ...bin  CON  saved game (a world)
Content\E9F766DBEDEF9C37\FFFE07D1\00010000\E9F766DBEDEF9C37          CON  profile package
```
Header fields worth knowing when moving packages between devices: 0x344 content type, 0x360 title ID, 0x36C the
console ID (5 bytes; the console that made it), 0x371 profile ID (owner), 0x3FD device ID (20 bytes; zeros for
marketplace packages, other values for console-made ones: not yet understood, check before writing packages to a
new device), 0x411 display name (UTF-16BE).

## The hard drive here (content/) differs, for now
The runtime's hard drive is `content/` beside the data files with the same path rules, but each package is an
**extracted folder** plus a header file in `<Title>\Headers\<Type>\<FileName>.header` (and a `.recomp` marker for
games from the games folder, `content_drive_games.md`). The runtime cannot open STFS package files yet ("STFS
container files are not supported yet"): reading a console USB stick needs STFS read support (mount a package file as
a read-only device, header straight from the file) and write support for saving to it.

## Design: every PC drive and USB stick a device, full STFS

Owner's decision: PC drives **and** USB sticks are storage devices, and packages are **real STFS files** where a
console would have them, so a console's packages (CON / LIVE / PIRS, signed) brought to the PC work, and a stick works
in both machines. Pieces (each its own job):

1. **STFS library** (`native/x360/system/stfs/`, shared by both ports): read a package file (header, metadata, the volume
   descriptor, hash tables, file table, block chains) as a read-only file system; write: create a package, add /
   replace / delete files, rehash (SHA-1 hash tables up the levels, the header hash at 0x32C..), keep LIVE / PIRS
   packages byte-identical when only copied. **Signing**: copying never re-signs. A CON package created or changed on
   the PC is signed only with a key the user points the runtime at (their own console's; never shipped, never in
   git); without one it is left unsigned (works on the PC and on a modded console, not on a stock one). Reference
   implementations: `codebreaker tools: stfs_extract.py` (ours, read), `../NexGen/nexgen/tools/stfs2.py`
   (read / write / rehash), XboxTranspiler's `docs/STFS-CON-Resigning.md`.
2. **Devices** (dashboard runtime): XAM's rules (SHOWS, XAM 17559): device IDs come from a counter starting at 1,
   skipping 0 and 2 (2 = "\Device\NetworkStorageCache"), wrapping 499 -> 3 (0x8167B3C8); so the hard drive is 1 and
   later devices 3, 4, ... in arrival order. The kind (XDEVICE_DATA type, device table +0x7C) picks the default name
   (0x815F37C0): 1 "Hard Drive", 2 "Memory Unit", 3 "System", 4 "Optical Drive", 8 "Transfer Cable", 0xF "Sapphire",
   0x10 "USB Storage", else "Storage Device". State (+0x80) 3 = ready; a USB device that is not configured is 5,
   unformatted 7 (XDEVICE_DATA type gets 0x4000000 / 0x2000000 for the dash, 0x80000000 for state 4).
   Ours: device 1 = the drive holding `content/` (the hard drive, as today); every other fixed or removable PC volume
   is one more device: kind 0x10 when its bus type is USB (IOCTL_STORAGE_QUERY_PROPERTY BusType), else kind 1 (an
   internal disk shows as a hard drive, owner TODO 7). Its content root: a console-formatted stick's own root
   (`name.txt` + hidden `Content\`, used as it is) or `<Drive>:\xbox360recomped\` (created hidden when content first
   goes there). Name: `name.txt` (UTF-16BE, BOM), else the volume label, else the kind's default.
3. **Packages on every device as the console stores them**: STFS files under `Content\<XUID>\<Title>\<Type>\`, header
   in the file. `content/` (extracted folders + `Headers\`) stays readable for the existing profiles and games and is
   migrated later or kept as the hard drive's own format (decided when the STFS library lands).
4. **Copy / Move** (XamContentCopyInternal / MoveInternal, protocol in the dashboard's NOTES session 27) between
   devices: a byte copy of the package file (+ its `.data\` folder); folder packages are written as STFS on the way.

## Template for other PC drives (proposal, superseded by the design above)
A PC drive used as a storage device gets the console's layout under one hidden folder at its root, so nothing else
on the drive is touched:
```
<Drive>:\xbox360recomped\            Hidden
    name.txt                         UTF-16BE with BOM, the device's name (default: the Windows volume label, else
                                     the console's default name for its kind of device)
    Content\<XUID>\<Title>\<Type>\<package>   as on a console USB stick (package files, `.data` folders)
```
A drive whose root already has a console's `name.txt` + hidden `Content\` (a stick formatted by a console) is used as
it is, at the root, so plugging that stick into the PC shows the same content the console shows, and content moved
to it on the PC shows on the console. Icons: a USB bus type shows the USB icon, internal drives the hard drive icon
(TODO 7 in the dashboard; IOCTL_STORAGE_QUERY_PROPERTY BusType).

Open questions: whether to store packages on other drives as STFS files (console-compatible, needs STFS writing) or
as extracted folders like content/ (simple, not readable by a console); FAT32's 4 GB file limit (the `.data` folders
solve it the console's way); the device ID field (0x3FD) for packages written to a new device.
