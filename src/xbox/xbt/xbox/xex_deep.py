"""Deep XEX2 file parser.

Ported from XexParser and Xbox360Toolkit.
Parses all optional header entries, execution ID, library versions,
resource sections, compression info, and security info.
"""

import struct
from typing import Optional
from dataclasses import dataclass, field


XEX2_MAGIC = b"XEX2"

# ── Optional header IDs and descriptions ────────────────

OPTIONAL_HEADERS = {
    0x000002FF: "Resource Section",
    0x000003FF: "Compression Info",
    0x000080FF: "Bound Pathname",
    0x000103FF: "Other Import Libraries",
    0x000183FF: "Unknown [2]",
    0x000200FF: "Library Versions",
    0x00010001: "Original Base Address",
    0x00010100: "Entry Point",
    0x00010201: "Image Base Address",
    0x00018002: "Checksum & Timestamp",
    0x00018102: "Callcap Enabled",
    0x00018200: "Fastcap Enabled",
    0x00020104: "TLS Info",
    0x00020200: "Default Stack Size",
    0x00020301: "Default FS Cache Size",
    0x00020401: "Default Heap Size",
    0x00030000: "System Flags",
    0x00040006: "Execution ID",
    0x00040201: "Title Workspace Size",
    0x00040310: "Game Ratings",
    0x00040404: "LAN Key",
    0x000405FF: "Unknown [1]",
    0x00E10402: "Export By Name",
}

# Module flag bits
MODULE_FLAGS = {
    0x0001: "TITLE_MODULE",
    0x0002: "EXPORTS_TO_TITLE",
    0x0004: "SYSTEM_DEBUGGER",
    0x0008: "DLL_MODULE",
    0x0010: "MODULE_PATCH",
    0x0020: "PATCH_FULL",
    0x0040: "PATCH_DELTA",
    0x0080: "USER_MODE",
}

# Image flag bits
IMAGE_FLAGS = {
    0x00000001: "MANUFACTURING_UTILITY",
    0x00000002: "MANUFACTURING_SUPPORT_TOOLS",
    0x00000004: "XGD2_MEDIA_ONLY",
    0x00000008: "CARDEA_KEY",
    0x00000010: "XEIKA_KEY",
    0x00000020: "USERMODE_TITLE",
    0x00000040: "USERMODE_SYSTEM",
    0x00000100: "ORANGE0",
    0x00000200: "ORANGE1",
    0x00000400: "ORANGE2",
    0x00002000: "IPTV_SIGNUP_APPLICATION",
    0x00004000: "IPTV_TITLE_APPLICATION",
    0x04000000: "KEYVAULT_PRIVILEGES_REQUIRED",
    0x08000000: "ONLINE_ACTIVATION_REQUIRED",
    0x10000000: "PAGE_SIZE_4KB",
    0x20000000: "NO_GAME_REGION",
    0x40000000: "REVOCATION_CHECK_OPTIONAL",
    0x80000000: "REVOCATION_CHECK_REQUIRED",
}

# Media types
MEDIA_TYPES = {
    0x00000001: "HDD",
    0x00000002: "DVD_X2",
    0x00000004: "DVD_CD",
    0x00000008: "DVD_5",
    0x00000010: "DVD_9",
    0x00000020: "SYSTEM_FLASH",
    0x00000080: "MEMORY_UNIT",
    0x00000100: "USB_MASS_STORAGE",
    0x00000200: "NETWORK",
    0x00000400: "DIRECT_FROM_MEMORY",
    0x00001000: "INSECURE_PACKAGE",
    0x00002000: "SAVEGAME_PACKAGE",
    0x00004000: "LOCALLY_SIGNED_PACKAGE",
    0x00008000: "LIVE_SIGNED_PACKAGE",
    0x00010000: "XBOX_PLATFORM_PACKAGE",
}

# Game regions
GAME_REGIONS = {
    0x000000FF: "REGION_FREE",
    0x00000001: "NTSCU",
    0x00000002: "JAPAN",
    0x00000004: "CHINA",
    0x00000008: "ASIA_EXTENDED",
    0x00000010: "NTSCJ_OTHER",
    0x00000100: "PAL_AU_NZ",
    0x00000200: "PAL_EUROPE",
    0x00000400: "PAL_AFRICA",
    0x00000800: "PAL_MIDDLE_EAST",
    0x01000000: "OTHER",
    0xFF000000: "ALL",
}


# ── Data structures ─────────────────────────────────────

@dataclass
class ExecutionId:
    media_id: int = 0
    version: int = 0
    base_version: int = 0
    title_id: int = 0
    platform: int = 0
    executable_type: int = 0
    disc_num: int = 0
    discs_in_set: int = 0
    save_game_id: int = 0

@dataclass
class LibraryVersion:
    name: str = ""
    major: int = 0
    minor: int = 0
    build: int = 0
    qfe: int = 0
    is_approved: bool = False

@dataclass
class XexResource:
    name: str = ""
    address: int = 0
    size: int = 0

@dataclass
class OptionalHeader:
    id: int = 0
    name: str = ""
    data: int = 0
    decoded: str = ""

@dataclass
class XexDeepInfo:
    magic: str = ""
    module_flags: int = 0
    module_flag_names: list = field(default_factory=list)
    data_offset: int = 0
    header_size: int = 0
    entry_point: int = 0
    image_base: int = 0
    image_size: int = 0
    image_flags: int = 0
    image_flag_names: list = field(default_factory=list)
    load_address: int = 0
    game_region: int = 0
    game_region_names: list = field(default_factory=list)
    allowed_media: int = 0
    media_type_names: list = field(default_factory=list)
    title_id: int = 0
    execution_id: Optional[ExecutionId] = None
    libraries: list = field(default_factory=list)
    resources: list = field(default_factory=list)
    optional_headers: list = field(default_factory=list)
    bound_path: str = ""
    default_stack_size: int = 0
    lan_key: bytes = b""
    session_key: bytes = b""


def _be32(data: bytes, off: int) -> int:
    return struct.unpack_from(">I", data, off)[0]

def _be16(data: bytes, off: int) -> int:
    return struct.unpack_from(">H", data, off)[0]

def _decode_flags(value: int, flag_table: dict) -> list:
    return [name for mask, name in flag_table.items() if value & mask]


# ── Parser ──────────────────────────────────────────────

def parse_xex2(data: bytes) -> XexDeepInfo:
    """Parse a XEX2 file with full optional header decoding."""
    if len(data) < 0x18 or data[:4] != XEX2_MAGIC:
        raise ValueError("Not a XEX2 file")

    info = XexDeepInfo()
    info.magic = "XEX2"
    info.module_flags = _be32(data, 4)
    info.module_flag_names = _decode_flags(info.module_flags, MODULE_FLAGS)
    info.data_offset = _be32(data, 8)
    info.header_size = _be32(data, 16)
    dir_count = _be32(data, 20)

    # Parse file header fields at header_size offset
    fh_off = info.header_size if info.header_size < len(data) else 0
    if fh_off > 0 and fh_off + 0x180 <= len(data):
        info.image_size = _be32(data, fh_off + 4)
        info.image_flags = _be32(data, fh_off + 0x0C)
        info.image_flag_names = _decode_flags(info.image_flags, IMAGE_FLAGS)
        info.load_address = _be32(data, fh_off + 0x10)
        info.game_region = _be32(data, fh_off + 0x78)
        info.game_region_names = _decode_flags(info.game_region, GAME_REGIONS)
        info.allowed_media = _be32(data, fh_off + 0x7C)
        info.media_type_names = _decode_flags(info.allowed_media, MEDIA_TYPES)

        # Session key (encrypted) at fh_off + 0x150
        if fh_off + 0x160 <= len(data):
            info.session_key = bytes(data[fh_off + 0x150:fh_off + 0x160])

    # Parse optional headers
    off = 0x18
    for _ in range(dir_count):
        if off + 8 > len(data):
            break
        hdr_id = _be32(data, off)
        hdr_data = _be32(data, off + 4)

        oh = OptionalHeader()
        oh.id = hdr_id
        oh.name = OPTIONAL_HEADERS.get(hdr_id, f"Unknown(0x{hdr_id:08X})")
        oh.data = hdr_data

        # Decode specific headers
        if hdr_id == 0x00010100:  # Entry Point
            info.entry_point = hdr_data
            oh.decoded = f"0x{hdr_data:08X}"
        elif hdr_id == 0x00010201:  # Image Base
            info.image_base = hdr_data
            oh.decoded = f"0x{hdr_data:08X}"
        elif hdr_id == 0x00010001:  # Original Base
            oh.decoded = f"0x{hdr_data:08X}"
        elif hdr_id == 0x00020200:  # Default Stack Size
            info.default_stack_size = hdr_data
            oh.decoded = f"0x{hdr_data:X} ({hdr_data:,} bytes)"
        elif hdr_id == 0x00040006:  # Execution ID
            if hdr_data + 24 <= len(data):
                eid = _parse_execution_id(data, hdr_data)
                info.execution_id = eid
                info.title_id = eid.title_id
                oh.decoded = f"Title ID: 0x{eid.title_id:08X}"
        elif hdr_id == 0x000200FF:  # Library Versions
            if hdr_data + 4 <= len(data):
                info.libraries = _parse_libraries(data, hdr_data)
                oh.decoded = f"{len(info.libraries)} libraries"
        elif hdr_id == 0x000080FF:  # Bound Pathname
            if hdr_data + 4 <= len(data):
                path_len = _be32(data, hdr_data)
                if hdr_data + 4 + path_len <= len(data):
                    info.bound_path = data[hdr_data+4:hdr_data+4+path_len].decode("ascii", errors="replace").rstrip("\x00")
                    oh.decoded = info.bound_path
        elif hdr_id == 0x00040404:  # LAN Key
            if hdr_data + 16 <= len(data):
                info.lan_key = bytes(data[hdr_data:hdr_data+16])
                oh.decoded = info.lan_key.hex().upper()
        elif hdr_id == 0x000002FF:  # Resource Section
            if hdr_data + 4 <= len(data):
                info.resources = _parse_resources(data, hdr_data)
                oh.decoded = f"{len(info.resources)} resources"
        elif hdr_id == 0x00030000:  # System Flags
            oh.decoded = f"0x{hdr_data:08X}"

        info.optional_headers.append(oh)
        off += 8

    return info


def _parse_execution_id(data: bytes, offset: int) -> ExecutionId:
    eid = ExecutionId()
    eid.media_id = _be32(data, offset)
    eid.version = _be32(data, offset + 4)
    eid.base_version = _be32(data, offset + 8)
    eid.title_id = _be32(data, offset + 12)
    eid.platform = data[offset + 16]
    eid.executable_type = data[offset + 17]
    eid.disc_num = data[offset + 18]
    eid.discs_in_set = data[offset + 19]
    eid.save_game_id = _be32(data, offset + 20)
    return eid


def _parse_libraries(data: bytes, offset: int) -> list:
    size = _be32(data, offset)
    libs = []
    pos = offset + 4
    entry_size = 16  # 8 name + 4x uint16 versions
    count = size // entry_size
    for _ in range(count):
        if pos + entry_size > len(data):
            break
        name = data[pos:pos+8].decode("ascii", errors="replace").rstrip("\x00")
        major = _be16(data, pos + 8)
        minor = _be16(data, pos + 10)
        build = _be16(data, pos + 12)
        qfe = _be16(data, pos + 14)
        approved = bool(qfe & 0x8000)
        libs.append(LibraryVersion(name=name, major=major, minor=minor,
                                    build=build, qfe=qfe & 0x7FFF,
                                    is_approved=approved))
        pos += entry_size
    return libs


def _parse_resources(data: bytes, offset: int) -> list:
    size = _be32(data, offset)
    resources = []
    pos = offset + 4
    entry_size = 16  # 8 name + 4 address + 4 size
    count = (size - 4) // entry_size if size > 4 else 0
    for _ in range(count):
        if pos + entry_size > len(data):
            break
        name = data[pos:pos+8].decode("ascii", errors="replace").rstrip("\x00")
        addr = _be32(data, pos + 8)
        sz = _be32(data, pos + 12)
        resources.append(XexResource(name=name, address=addr, size=sz))
        pos += entry_size
    return resources
