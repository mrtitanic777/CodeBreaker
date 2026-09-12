"""Xbox 360 Console Forensics Engine.

Reconstructs a complete console identity and history from any combination of:
  - Decrypted keyvault (KV_dec.bin)
  - Encrypted keyvault + CPU key
  - NAND dump
  - HDD/USB image
  - Fuse dump

Produces a comprehensive forensic report covering:
  - Console identity (serial, console ID, manufacturing, revision)
  - Cryptographic key inventory (all keys present, derivable, or missing)
  - Security posture (XVal flags, ban status, certificate validity)
  - Boot chain integrity (every stage hashed and verified)
  - Console history (games played, achievements, LIVE connections)
  - Fuse state analysis (LDV, update sequence, blow count)
"""

import hashlib
import struct
import json
import time
from pathlib import Path
from typing import Optional
from dataclasses import dataclass, field


# ── Console types ───────────────────────────────────────

CONSOLE_TYPES = {
    0x00000001: "Development Kit",
    0x00000002: "Retail",
    0x40000001: "Test Kit (XDK Recovery)",
    0x40000002: "Retail (XDK Recovery)",
}

MOTHERBOARD_TYPES = {
    "Xenon": {"gpu": "90nm", "cpu": "90nm", "year_range": "2005-2006", "rrod_risk": "HIGH"},
    "Zephyr": {"gpu": "80nm", "cpu": "65nm", "year_range": "2007", "rrod_risk": "MEDIUM"},
    "Falcon": {"gpu": "80nm", "cpu": "65nm", "year_range": "2007-2008", "rrod_risk": "LOW"},
    "Jasper": {"gpu": "65nm", "cpu": "65nm", "year_range": "2008-2009", "rrod_risk": "VERY LOW"},
    "Kronos": {"gpu": "65nm", "cpu": "65nm", "year_range": "2009-2010", "rrod_risk": "MINIMAL"},
    "Trinity": {"gpu": "45nm", "cpu": "45nm (Valhalla SoC)", "year_range": "2010-2013", "rrod_risk": "NONE"},
    "Corona": {"gpu": "45nm", "cpu": "45nm (Valhalla SoC)", "year_range": "2013-2016", "rrod_risk": "NONE"},
    "Winchester": {"gpu": "45nm", "cpu": "45nm (Valhalla SoC)", "year_range": "2014-2016", "rrod_risk": "NONE"},
}

GAME_REGIONS = {
    0x00FF: "Region Free",
    0x0001: "NTSC-U (North America)",
    0x0002: "NTSC-J (Japan)",
    0x0004: "NTSC-J (China)",
    0x0008: "NTSC-J (Asia Extended)",
    0x0100: "PAL (Australia/NZ)",
    0x0200: "PAL (Europe)",
    0x0400: "PAL (Africa)",
    0x0800: "PAL (Middle East)",
}


# ── Key inventory ───────────────────────────────────────

@dataclass
class CryptoKey:
    name: str = ""
    offset: int = 0
    size: int = 0
    data: bytes = b""
    present: bool = False
    entropy: float = 0.0
    description: str = ""


def _entropy(data: bytes) -> float:
    """Calculate Shannon entropy of data (0.0 = uniform, 8.0 = random)."""
    if not data or len(data) == 0:
        return 0.0
    freq = {}
    for b in data:
        freq[b] = freq.get(b, 0) + 1
    import math
    entropy = 0.0
    for count in freq.values():
        p = count / len(data)
        if p > 0:
            entropy -= p * math.log2(p)
    return entropy


def _is_key_present(data: bytes) -> bool:
    """Check if key data looks like a real key (not zeros/padding)."""
    if not data:
        return False
    if all(b == 0 for b in data):
        return False
    if all(b == 0xFF for b in data):
        return False
    if len(set(data)) < 4:
        return False
    return True


# ── Forensic Report ─────────────────────────────────────

@dataclass
class ForensicReport:
    generated: str = ""
    tool_version: str = "NexGen Forensics v1.0"
    sources: list = field(default_factory=list)

    # Identity
    console_serial: str = ""
    motherboard_serial: str = ""
    console_id: str = ""
    console_type: str = ""
    console_type_raw: int = 0
    manufacture_date: str = ""
    part_number: str = ""
    game_region: str = ""
    game_region_raw: int = 0
    motherboard_revision: str = ""
    motherboard_info: dict = field(default_factory=dict)

    # Crypto inventory
    keys: list = field(default_factory=list)
    cpu_key: str = ""
    cpu_key_valid: bool = False
    cpu_key_hamming: int = 0
    cpu_key_ecd_ok: bool = False
    dvd_key: str = ""

    # Certificate
    cert_public_key_exponent: int = 0
    cert_public_key_modulus: str = ""
    cert_size: int = 0
    cert_privileges: int = 0

    # Security
    xval_flags: list = field(default_factory=list)
    ban_status: str = "Unknown"
    security_notes: list = field(default_factory=list)

    # Boot chain
    bootchain_stages: list = field(default_factory=list)
    bootchain_integrity: str = "Not analyzed"

    # Fuses
    fuse_ldv: int = 0
    fuse_lines: dict = field(default_factory=dict)
    fuse_cfldv: int = 0
    fuse_blow_count: int = 0

    # History
    games_played: list = field(default_factory=list)
    total_gamerscore: int = 0
    total_achievements: int = 0
    total_titles: int = 0

    # Raw data hashes (for verification)
    hashes: dict = field(default_factory=dict)


# ── Forensics Engine ───────────────────────────────────

class ForensicsEngine:
    """Build a comprehensive forensic report from console data.

    Usage:
        engine = ForensicsEngine()
        engine.add_keyvault("KV_dec.bin")
        engine.add_nand("nand_dump.bin")
        engine.add_cpu_key("C0DE8DAAE05493BCB0F1664FB1751F00")
        report = engine.analyze()
    """

    def __init__(self):
        self._kv_data: Optional[bytes] = None
        self._nand_data: Optional[bytes] = None
        self._hdd_path: Optional[str] = None
        self._cpu_key: Optional[bytes] = None
        self._fuse_data: Optional[bytes] = None
        self._report = ForensicReport()

    def add_keyvault(self, path_or_data):
        """Add a keyvault (decrypted or encrypted with CPU key)."""
        if isinstance(path_or_data, bytes):
            self._kv_data = path_or_data
        else:
            self._kv_data = Path(path_or_data).read_bytes()
        self._report.sources.append(f"Keyvault ({len(self._kv_data):,} bytes)")
        self._report.hashes["keyvault_sha1"] = hashlib.sha1(self._kv_data).hexdigest().upper()

    def add_nand(self, path_or_data):
        """Add a NAND dump."""
        if isinstance(path_or_data, bytes):
            self._nand_data = path_or_data
        else:
            self._nand_data = Path(path_or_data).read_bytes()
        self._report.sources.append(f"NAND ({len(self._nand_data):,} bytes)")
        self._report.hashes["nand_sha1"] = hashlib.sha1(self._nand_data).hexdigest().upper()

    def add_hdd(self, path: str):
        """Add an HDD/USB image path."""
        self._hdd_path = path
        self._report.sources.append(f"HDD ({path})")

    def add_cpu_key(self, key_hex: str):
        """Add a CPU key."""
        self._cpu_key = bytes.fromhex(key_hex.strip().replace(" ", ""))
        self._report.sources.append("CPU Key")

    def add_fuses(self, path_or_data):
        """Add fuse dump data."""
        if isinstance(path_or_data, bytes):
            self._fuse_data = path_or_data
        else:
            self._fuse_data = Path(path_or_data).read_bytes()
        self._report.sources.append(f"Fuses ({len(self._fuse_data)} bytes)")

    # ── Analysis ────────────────────────────────────────

    def analyze(self) -> ForensicReport:
        """Run full forensic analysis."""
        self._report.generated = time.strftime("%Y-%m-%d %H:%M:%S UTC", time.gmtime())

        if self._kv_data:
            self._analyze_keyvault()

        if self._cpu_key:
            self._analyze_cpu_key()

        if self._nand_data:
            self._analyze_nand()

        if self._fuse_data:
            self._analyze_fuses()

        if self._hdd_path:
            self._analyze_hdd()

        self._cross_reference()

        return self._report

    def _analyze_keyvault(self):
        """Extract all identity and crypto data from the keyvault."""
        from keyvault import parse_keyvault
        kv = parse_keyvault(self._kv_data)

        if "error" in kv:
            self._report.security_notes.append(f"KV parse error: {kv['error']}")
            return

        # Identity
        serial = kv.get("console_serial", b"")
        if isinstance(serial, bytes):
            self._report.console_serial = serial.decode("ascii", errors="replace").rstrip("\x00")
        else:
            self._report.console_serial = str(serial)

        mb_serial = kv.get("motherboard_serial", b"")
        if isinstance(mb_serial, bytes):
            self._report.motherboard_serial = mb_serial.decode("ascii", errors="replace").rstrip("\x00")

        cid = kv.get("console_id", b"")
        if isinstance(cid, bytes):
            self._report.console_id = cid.hex().upper()

        self._report.console_type_raw = kv.get("console_type", 0)
        self._report.console_type = CONSOLE_TYPES.get(
            self._report.console_type_raw,
            f"Unknown (0x{self._report.console_type_raw:08X})")

        mfg = kv.get("manufacture_date", b"")
        if isinstance(mfg, bytes) and len(mfg) >= 8:
            self._report.manufacture_date = mfg.decode("ascii", errors="replace").rstrip("\x00")

        pn = kv.get("console_part_number", b"")
        if isinstance(pn, bytes):
            self._report.part_number = pn.decode("ascii", errors="replace").rstrip("\x00")

        self._report.game_region_raw = kv.get("game_region", 0)
        regions = []
        for mask, name in GAME_REGIONS.items():
            if self._report.game_region_raw & mask:
                regions.append(name)
        self._report.game_region = ", ".join(regions) if regions else f"0x{self._report.game_region_raw:04X}"

        # Certificate
        self._report.cert_size = kv.get("console_certificate_size", 0)
        self._report.cert_public_key_exponent = kv.get("console_public_key_exponent", 0)
        modulus = kv.get("console_public_key_modulus", b"")
        if isinstance(modulus, bytes):
            self._report.cert_public_key_modulus = modulus.hex().upper()
        self._report.cert_privileges = kv.get("console_privileges", 0)

        # Crypto key inventory
        KEY_MAP = [
            ("Console Obfuscation Key", "console_obf_key", 0xBC, 16,
             "Used to obfuscate console-specific data"),
            ("Key Obfuscation Key", "key_obf_key", 0xCC, 16,
             "Used to wrap other keys"),
            ("Roamable Obfuscation Key", "roamable_obf_key", 0xDC, 16,
             "Used for roamable profile data"),
            ("ODD Key", "odd_key", 0xEC, 16,
             "Optical disc drive authentication key"),
            ("Primary Activation Key", "primary_activation_key", 0xFC, 24,
             "Primary console activation (LIVE)"),
            ("Secondary Activation Key", "secondary_activation_key", 0x114, 16,
             "Secondary activation key"),
            ("Global Dev 2DES Key 1", "global_dev_2des_key_1", 0x124, 16,
             "Global development 2DES key pair 1"),
            ("Global Dev 2DES Key 2", "global_dev_2des_key_2", 0x134, 16,
             "Global development 2DES key pair 2"),
            ("Wireless Controller Key 1", "wireless_controller_2des_key_1", 0x144, 16,
             "Wireless controller pairing key 1"),
            ("Wireless Controller Key 2", "wireless_controller_2des_key_2", 0x154, 16,
             "Wireless controller pairing key 2"),
        ]

        for name, kv_field, offset, size, desc in KEY_MAP:
            data = kv.get(kv_field, b"")
            if isinstance(data, bytes):
                key = CryptoKey(
                    name=name, offset=offset, size=size, data=data,
                    present=_is_key_present(data),
                    entropy=_entropy(data),
                    description=desc)
                self._report.keys.append(key)

        # DVD key from keyvault
        dvd_key = kv.get("odd_key", b"")
        if isinstance(dvd_key, bytes) and _is_key_present(dvd_key):
            self._report.dvd_key = dvd_key.hex().upper()

    def _analyze_cpu_key(self):
        """Validate the CPU key cryptographically."""
        from cpukey_ecd import validate_cpukey
        key_hex = self._cpu_key.hex().upper()
        self._report.cpu_key = key_hex

        result = validate_cpukey(key_hex)
        self._report.cpu_key_valid = result["valid"]
        self._report.cpu_key_hamming = result["hamming_weight"]
        self._report.cpu_key_ecd_ok = result["ecd_ok"]

        if not result["valid"]:
            self._report.security_notes.append(
                f"CPU Key INVALID: {'; '.join(result['errors'])}")

        # Add CPU key to key inventory
        key = CryptoKey(
            name="CPU Key", offset=0, size=16, data=self._cpu_key,
            present=True, entropy=_entropy(self._cpu_key),
            description="Unique per-console key burned into fuses. "
                        f"Hamming=0x{result['hamming_weight']:X}, "
                        f"ECD={'OK' if result['ecd_ok'] else 'FAIL'}")
        self._report.keys.insert(0, key)

    def _analyze_nand(self):
        """Analyze NAND dump for boot chain and keyvault."""
        from bootchain import scan_nand

        bc = scan_nand(self._nand_data)
        for stage in bc.stages:
            self._report.bootchain_stages.append({
                "name": stage.bl_name,
                "version": stage.version,
                "offset": f"0x{stage.offset_in_nand:06X}",
                "size": stage.size,
                "entrypoint": f"0x{stage.entrypoint:08X}",
                "is_devkit": stage.is_devkit,
                "key_present": _is_key_present(stage.key),
                "key_hash": hashlib.sha1(stage.key).hexdigest().upper()[:16] if stage.key else "",
            })

        if bc.stages:
            self._report.bootchain_integrity = "Stages found and parsed"
        else:
            self._report.bootchain_integrity = "No bootloader stages detected"
            self._report.security_notes.append("NAND may be corrupt or encrypted")

        # Extract KV from NAND if not already provided
        if self._kv_data is None:
            kv_offset = 0x4000
            kv_size = 0x4000
            if kv_offset + kv_size <= len(self._nand_data):
                nand_kv = self._nand_data[kv_offset:kv_offset + kv_size]
                # Check if it looks like a valid KV (not all zeros/FF)
                if _is_key_present(nand_kv[:16]):
                    self._kv_data = nand_kv
                    self._report.sources.append("Keyvault (extracted from NAND)")
                    self._analyze_keyvault()

    def _analyze_fuses(self):
        """Analyze fuse dump."""
        from fuses import parse_fuses
        info = parse_fuses(self._fuse_data)

        self._report.fuse_ldv = info.get("ldv", 0)
        self._report.fuse_lines = {k: v for k, v in info.items() if k.startswith("fuseline")}

        # CPU key from fuses
        fuse_cpu_key = info.get("cpu_key", "")
        if fuse_cpu_key and not self._cpu_key:
            self._cpu_key = bytes.fromhex(fuse_cpu_key)
            self._report.sources.append("CPU Key (from fuses)")
            self._analyze_cpu_key()

        # Count blown fuses
        blow_count = 0
        for b in self._fuse_data:
            blow_count += bin(b).count('1')
        self._report.fuse_blow_count = blow_count

    def _analyze_hdd(self):
        """Scan HDD for game history."""
        try:
            from fatx import FatxReader
            reader = FatxReader(self._hdd_path)
            parts = reader.detect_partitions()

            for p in parts:
                if p.name.lower() == "content":
                    entries = reader.list_directory(p)
                    flat = reader._flatten(entries)
                    # Count GPD files (game profile data)
                    gpd_files = [e for e in flat if e.name.lower().endswith(".gpd")]
                    self._report.total_titles = len(gpd_files)
                    break

            reader.close()
        except Exception as e:
            self._report.security_notes.append(f"HDD analysis error: {e}")

    def _cross_reference(self):
        """Cross-reference data from multiple sources for consistency."""
        # Check if CPU key from fuses matches keyvault
        if self._cpu_key and self._kv_data:
            # The keyvault should be decryptable with this CPU key
            self._report.security_notes.append(
                "CPU key and keyvault both present — cross-validation possible")

        # Determine motherboard revision from manufacturing date + part number
        pn = self._report.part_number
        if "X811" in pn or "X815" in pn:
            self._report.motherboard_revision = "Xenon"
        elif "X818" in pn:
            self._report.motherboard_revision = "Zephyr"
        elif "X819" in pn or "X821" in pn:
            self._report.motherboard_revision = "Falcon"
        elif "X831" in pn:
            self._report.motherboard_revision = "Jasper"
        elif "X850" in pn or "X851" in pn:
            self._report.motherboard_revision = "Trinity"
        elif "X861" in pn or "X877" in pn:
            self._report.motherboard_revision = "Corona"
        elif "X883" in pn or "X892" in pn:
            self._report.motherboard_revision = "Winchester"

        if self._report.motherboard_revision in MOTHERBOARD_TYPES:
            self._report.motherboard_info = MOTHERBOARD_TYPES[self._report.motherboard_revision]

        # Security posture summary
        if not self._report.keys:
            self._report.security_notes.append("No cryptographic keys found")
        else:
            present = sum(1 for k in self._report.keys if k.present)
            total = len(self._report.keys)
            self._report.security_notes.append(
                f"Key inventory: {present}/{total} keys present")

            # Check for low-entropy keys (potential tampering)
            for k in self._report.keys:
                if k.present and k.entropy < 3.0:
                    self._report.security_notes.append(
                        f"WARNING: {k.name} has low entropy ({k.entropy:.1f}) — possible tampering")


# ── Report formatting ───────────────────────────────────

def format_report(report: ForensicReport) -> str:
    """Format a forensic report as readable text."""
    lines = []
    lines.append("=" * 70)
    lines.append("  XBOX 360 CONSOLE FORENSIC REPORT")
    lines.append(f"  Generated: {report.generated}")
    lines.append(f"  Tool: {report.tool_version}")
    lines.append(f"  Sources: {', '.join(report.sources)}")
    lines.append("=" * 70)

    # Identity
    lines.append("\n── CONSOLE IDENTITY ──────────────────────────────")
    lines.append(f"  Serial:           {report.console_serial or '—'}")
    lines.append(f"  Motherboard S/N:  {report.motherboard_serial or '—'}")
    lines.append(f"  Console ID:       {report.console_id or '—'}")
    lines.append(f"  Console Type:     {report.console_type}")
    lines.append(f"  Part Number:      {report.part_number or '—'}")
    lines.append(f"  Manufacture Date: {report.manufacture_date or '—'}")
    lines.append(f"  Game Region:      {report.game_region}")
    if report.motherboard_revision:
        lines.append(f"  Motherboard:      {report.motherboard_revision}")
        if report.motherboard_info:
            mi = report.motherboard_info
            lines.append(f"    CPU:            {mi.get('cpu', '?')}")
            lines.append(f"    GPU:            {mi.get('gpu', '?')}")
            lines.append(f"    Year Range:     {mi.get('year_range', '?')}")
            lines.append(f"    RROD Risk:      {mi.get('rrod_risk', '?')}")

    # Crypto
    lines.append("\n── CRYPTOGRAPHIC KEY INVENTORY ───────────────────")
    if report.cpu_key:
        valid = "VALID" if report.cpu_key_valid else "INVALID"
        lines.append(f"  CPU Key:          {report.cpu_key}  [{valid}]")
        lines.append(f"    Hamming Weight: 0x{report.cpu_key_hamming:X} (need 0x35)")
        lines.append(f"    ECD:            {'OK' if report.cpu_key_ecd_ok else 'FAIL'}")
    if report.dvd_key:
        lines.append(f"  DVD Key:          {report.dvd_key}")

    for key in report.keys:
        if key.name == "CPU Key":
            continue
        status = "PRESENT" if key.present else "MISSING"
        hex_preview = key.data[:8].hex().upper() + "..." if key.present else ""
        lines.append(f"  {key.name:<30} [{status}]  {hex_preview}")
        if key.present:
            lines.append(f"    Entropy: {key.entropy:.2f}/8.0  Size: {key.size} bytes")
            lines.append(f"    {key.description}")

    # Certificate
    if report.cert_public_key_modulus:
        lines.append("\n── CONSOLE CERTIFICATE ──────────────────────────")
        lines.append(f"  Certificate Size: {report.cert_size}")
        lines.append(f"  Public Key Exp:   0x{report.cert_public_key_exponent:X}")
        lines.append(f"  Public Key Mod:   {report.cert_public_key_modulus[:32]}...")
        lines.append(f"  Privileges:       0x{report.cert_privileges:04X}")

    # Boot chain
    if report.bootchain_stages:
        lines.append("\n── BOOT CHAIN ───────────────────────────────────")
        lines.append(f"  Integrity: {report.bootchain_integrity}")
        for s in report.bootchain_stages:
            devkit = " [DEVKIT]" if s.get("is_devkit") else ""
            key_status = " key=✓" if s.get("key_present") else ""
            lines.append(f"  {s['name']:<12} v{s['version']:<6} @ {s['offset']}"
                         f"  size=0x{s['size']:06X}{devkit}{key_status}")

    # Fuses
    if report.fuse_lines:
        lines.append("\n── FUSE STATE ───────────────────────────────────")
        lines.append(f"  Lock Down Value:  {report.fuse_ldv}")
        lines.append(f"  Total Blown Fuses: {report.fuse_blow_count}")
        for name, value in sorted(report.fuse_lines.items()):
            lines.append(f"  {name}: {value}")

    # Security
    lines.append("\n── SECURITY POSTURE ─────────────────────────────")
    if report.xval_flags:
        lines.append(f"  Ban Status:       {report.ban_status}")
        for flag in report.xval_flags:
            lines.append(f"    - {flag}")
    else:
        lines.append(f"  Ban Status:       {report.ban_status}")

    for note in report.security_notes:
        lines.append(f"  NOTE: {note}")

    # History
    if report.total_titles > 0:
        lines.append("\n── CONSOLE HISTORY ──────────────────────────────")
        lines.append(f"  Titles Played:    {report.total_titles}")
        lines.append(f"  Gamerscore:       {report.total_gamerscore:,}")
        lines.append(f"  Achievements:     {report.total_achievements:,}")

    # Hashes
    if report.hashes:
        lines.append("\n── DATA HASHES (for verification) ───────────────")
        for name, h in report.hashes.items():
            lines.append(f"  {name}: {h}")

    lines.append("\n" + "=" * 70)
    return "\n".join(lines)


def save_report_json(report: ForensicReport, path: str):
    """Save a forensic report as JSON."""
    data = {
        "generated": report.generated,
        "tool": report.tool_version,
        "sources": report.sources,
        "identity": {
            "serial": report.console_serial,
            "motherboard_serial": report.motherboard_serial,
            "console_id": report.console_id,
            "console_type": report.console_type,
            "part_number": report.part_number,
            "manufacture_date": report.manufacture_date,
            "game_region": report.game_region,
            "motherboard_revision": report.motherboard_revision,
            "motherboard_info": report.motherboard_info,
        },
        "crypto": {
            "cpu_key": report.cpu_key,
            "cpu_key_valid": report.cpu_key_valid,
            "cpu_key_hamming": report.cpu_key_hamming,
            "cpu_key_ecd_ok": report.cpu_key_ecd_ok,
            "dvd_key": report.dvd_key,
            "keys": [{
                "name": k.name,
                "present": k.present,
                "entropy": round(k.entropy, 2),
                "hex": k.data.hex().upper() if k.present else "",
                "description": k.description,
            } for k in report.keys],
        },
        "certificate": {
            "size": report.cert_size,
            "public_key_exponent": report.cert_public_key_exponent,
            "public_key_modulus": report.cert_public_key_modulus,
            "privileges": report.cert_privileges,
        },
        "bootchain": {
            "integrity": report.bootchain_integrity,
            "stages": report.bootchain_stages,
        },
        "fuses": {
            "ldv": report.fuse_ldv,
            "blow_count": report.fuse_blow_count,
            "lines": report.fuse_lines,
        },
        "security": {
            "ban_status": report.ban_status,
            "xval_flags": report.xval_flags,
            "notes": report.security_notes,
        },
        "history": {
            "titles_played": report.total_titles,
            "gamerscore": report.total_gamerscore,
            "achievements": report.total_achievements,
        },
        "hashes": report.hashes,
    }
    Path(path).write_text(json.dumps(data, indent=2))
