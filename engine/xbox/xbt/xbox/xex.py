#!/usr/bin/env python3
"""
xex.py — an XEX (Xbox 360 executable) reader, the fourth format in this lineage.

The lineage so far parsed three PC formats: NE (16-bit, ne.py), PE (32-bit,
pe.py) and LE/LX (VxD, le.py). The Xbox 360 is a different machine entirely — a
big-endian **PowerPC** console (the "Xenon" CPU, three PPC cores) — and it ships
its executables in its own container: the **XEX** ("Xbox EXecutable").

An XEX is a wrapper. Peel it and what is inside is an ordinary **PE image**, but
one whose code is **PowerPC**, not x86. So the job here mirrors the PC loaders
with one extra front stage:

    raw .xex  --[parse headers]-->  basefile region
              --[decrypt (AES)]-->  cleartext
              --[decompress]------>  the embedded PE image
              --[parse PE]-------->  sections; .text is PowerPC code

The header/section parsing below is exact and complete, and the DECRYPT +
DECOMPRESS front stage is now PROVEN end-to-end against ground truth: a retail
`default.xex` (AES-128 + LZX) decodes to byte-for-byte the same PE image as a
separately-produced uncompressed copy (all 18 PE sections match; see README).
The AES step uses the public retail key (same as the Xenia emulator); LZX lives
in `_lzx.py`. Per project doctrine, nothing here guesses: an image we cannot
fully decode raises a clear, specific error rather than returning plausible-
looking bytes.

Everything multi-byte in the XEX header is BIG-ENDIAN (the console is BE).

    python xex.py default.xex                 # header summary
    python xex.py default.xex --sections      # the embedded PE section map
    python xex.py default.xex --dump out.exe  # write the decoded basefile
    python xex.py default.xex --resources     # embedded resource sections (media/SPA)
    python xex.py default.xex --extract media # pull one section out (media -> media.xzp)
    python xex.py default.xex --extract-all out/   # dump every resource section
"""
import hashlib
import struct
import sys

MAGICS = (b"XEX2", b"XEX1", b"XEX%", b"XEX-", b"XEX?")

# The retail image key, public and identical to the one the Xenia emulator uses
# to unwrap retail titles. The devkit key is 16 zero bytes. We try retail first,
# then devkit, and keep whichever yields a valid PE — never guessing between them.
RETAIL_KEY = bytes.fromhex("20b185a59d28fdc340583fbb0896bf91")
DEVKIT_KEY = b"\x00" * 16
XEX1_KEY   = bytes.fromhex("a26c10f71fd935e98b99922ce9321572")  # older XEX1 KEK

# Optional-header keys we care about (see the XEX2 header directory).
K_RESOURCE_INFO   = 0x000002FF
K_FILE_FORMAT     = 0x000003FF   # encryption + compression descriptor
K_ENTRY_POINT     = 0x00010100
K_IMAGE_BASE      = 0x00010201
K_ORIGINAL_BASE   = 0x00010001
K_IMPORT_LIBS     = 0x000103FF
K_ORIGINAL_PENAME = 0x000183FF
K_EXECUTION_INFO  = 0x00040006
K_DELTA_PATCH_DESC = 0x000005FF  # delta-patch descriptor (title-update .xexp)
K_BOUNDING_PATH   = 0x000080FF

# encryption / compression enums from the file-format descriptor
ENC_NONE, ENC_NORMAL = 0, 1
CMP_NONE, CMP_BASIC, CMP_NORMAL, CMP_DELTA = 0, 1, 2, 3

# module_flags (header +0x04) bit meanings
_MODULE_FLAGS = [
    (0x01, "title"), (0x02, "exports-to-title"), (0x04, "system-debugger"),
    (0x08, "dll"), (0x10, "module-patch"), (0x20, "patch-full"),
    (0x40, "patch-delta"), (0x80, "user-mode"),
]

_BE32 = ">I"
_BE16 = ">H"


def _u32(d, o):
    return struct.unpack_from(_BE32, d, o)[0]


def _u16(d, o):
    return struct.unpack_from(_BE16, d, o)[0]


class XEXError(Exception):
    """A specific, honest failure — never a silent wrong answer."""


class XEXFile:
    def __init__(self, path):
        if isinstance(path, (bytes, bytearray)):
            self.path = "<bytes>"
            self.data = bytes(path)
        else:
            self.path = path
            with open(path, "rb") as f:
                self.data = f.read()
        d = self.data
        self.magic = d[:4]
        if self.magic not in MAGICS:
            raise XEXError(f"{path}: not an XEX (magic {self.magic!r})")
        if self.magic != b"XEX2":
            # XEX1/XEX% differ mainly in the security-info layout. We parse the
            # header directory the same way, but flag the unknown so the crypto
            # offsets are not trusted blindly.
            pass
        self._parse_header()

    # -- XEX header directory ------------------------------------------------
    def _parse_header(self):
        d = self.data
        self.module_flags       = _u32(d, 0x04)
        self.data_offset        = _u32(d, 0x08)   # where the basefile begins
        self.security_offset    = _u32(d, 0x10)
        self.opt_header_count   = _u32(d, 0x14)

        # optional-header directory: opt_header_count * (key, value)
        self.opt = {}          # key -> raw value (inline dword or file offset)
        p = 0x18
        for _ in range(self.opt_header_count):
            if p + 8 > len(d):
                break
            key, val = struct.unpack_from(">II", d, p)
            self.opt[key] = val
            p += 8

        # entry point / image base are stored inline in the directory.
        self.entry_point = self.opt.get(K_ENTRY_POINT)
        self.image_base  = self.opt.get(K_IMAGE_BASE)
        self.original_base = self.opt.get(K_ORIGINAL_BASE)

        self._parse_file_format()
        self._parse_execution_info()
        self._parse_delta_descriptor()

    # -- identity / patch metadata ------------------------------------------
    @staticmethod
    def decode_version(dword):
        """Xbox 360 version dword -> 'vMaj.Min.Build.Qfe'."""
        if dword is None:
            return None
        return "v%d.%d.%d.%d" % ((dword >> 28) & 0xF, (dword >> 24) & 0xF,
                                 (dword >> 8) & 0xFFFF, dword & 0xFF)

    def module_flag_names(self):
        return [name for bit, name in _MODULE_FLAGS if self.module_flags & bit]

    @property
    def module_type(self):
        f = self.module_flags
        if f & 0x40:
            return "delta-patch"
        if f & 0x20:
            return "full-patch"
        if f & 0x10:
            return "module-patch"
        if f & 0x08:
            return "dll"
        return "title"

    @property
    def is_patch(self):
        return bool(self.module_flags & 0x70)

    def _parse_execution_info(self):
        """Execution ID (opt 0x00040006): title/media/version identity (24 B)."""
        self.execution_info = None
        off = self.opt.get(K_EXECUTION_INFO)
        if off is None or off + 24 > len(self.data):
            return
        d = self.data
        media_id, version, base_version, title_id = struct.unpack_from(">IIII", d, off)
        title_hex = "%08X" % title_id
        pub = bytes([(title_id >> 24) & 0xFF, (title_id >> 16) & 0xFF])
        self.execution_info = {
            "media_id": "%08X" % media_id,
            "version": version,
            "version_str": self.decode_version(version),
            "base_version": base_version,
            "base_version_str": self.decode_version(base_version),
            "title_id": title_hex,
            "publisher": pub.decode("latin-1", "replace") if all(32 <= b < 127 for b in pub) else pub.hex(),
            "platform": d[off + 0x10],
            "executable_type": d[off + 0x11],
            "disc_number": d[off + 0x12],
            "disc_count": d[off + 0x13],
            "savegame_id": _u32(d, off + 0x14),
        }

    def _parse_delta_descriptor(self):
        """Delta-patch descriptor (opt 0x000005FF): source->target for a title
        update. Present only on delta-patch modules; records the SHA-1 of the
        base image the patch must be applied against. Layout matches XEX2's
        XEX_DELTA_PATCH_DESCRIPTOR (as documented by XEXLoaderWV)."""
        self.delta_descriptor = None
        off = self.opt.get(K_DELTA_PATCH_DESC)
        if off is None or off + 76 > len(self.data):
            return
        d = self.data
        dh = struct.unpack_from(">III", d, off + 0x34)   # delta headers
        di = struct.unpack_from(">III", d, off + 0x40)   # delta image
        self.delta_descriptor = {
            "size": _u32(d, off),
            "target_version": self.decode_version(_u32(d, off + 0x04)),
            "source_version": self.decode_version(_u32(d, off + 0x08)),
            "source_digest": d[off + 0x0C:off + 0x20].hex(),
            "image_key": d[off + 0x20:off + 0x30].hex(),
            "target_header_size": _u32(d, off + 0x30),
            "delta_headers": {"src_off": dh[0], "src_size": dh[1], "tgt_off": dh[2]},
            "delta_image": {"src_off": di[0], "src_size": di[1], "tgt_off": di[2]},
        }

    # -- security info / resources / bounding path --------------------------
    def _img(self):
        """Image-info offset (past the 0x100-byte signature in security info)."""
        return self.security_offset + 8 + 256

    def security(self):
        d, img = self.data, self._img()
        return {
            "load_address": _u32(d, img + 8),
            "media_id": d[img + 56:img + 72],
            "aes_key": d[img + 72:img + 88],
            "export_table": _u32(d, img + 88),
            "region": _u32(d, img + 112),
            "allowed_media": _u32(d, img + 116),
        }

    def resources(self):
        """[(name, address, size)] from the resource-info header (0x000002FF)."""
        blob = self._opt_blob(K_RESOURCE_INFO)
        if not blob:
            return []
        size = _u32(blob, 0)
        n = (size - 4) // 16 if size >= 4 else 0
        out = []
        for i in range(n):
            e = 4 + i * 16
            name = blob[e:e + 8].split(b"\x00", 1)[0].decode("latin-1", "replace")
            out.append((name, _u32(blob, e + 8), _u32(blob, e + 12)))
        return out

    def resource_bytes(self, name):
        """Raw bytes of the embedded resource section `name` (e.g. 'media' — the
        packed .xzp media archive — or the title-id, which names the .spa).

        A resource is mapped into the loaded image by virtual address, so its
        bytes live in the decoded basefile at (addr - image_base). Raises
        XEXError if there is no such section or its range falls outside the
        decoded image — never returns a wrong slice."""
        base = self.image_base or 0
        for rname, addr, size in self.resources():
            if rname != name:
                continue
            img = self.basefile()
            off = addr - base
            if off < 0 or off + size > len(img):
                raise XEXError(
                    "resource %r range [0x%X,+%d] falls outside the decoded "
                    "image (%d bytes)" % (name, off, size, len(img)))
            return img[off:off + size]
        have = ", ".join(r[0] for r in self.resources()) or "(none)"
        raise XEXError("no resource section named %r; present: %s" % (name, have))

    def bounding_path(self):
        blob = self._opt_blob(K_BOUNDING_PATH)
        if not blob:
            return None
        return blob[4:4 + _u32(blob, 0)].split(b"\x00", 1)[0].decode("latin-1", "replace")

    def _opt_blob(self, key):
        """Bytes of a variable-length optional header (value is a file offset;
        first dword at that offset is the blob size)."""
        off = self.opt.get(key)
        if off is None or off + 4 > len(self.data):
            return None
        size = _u32(self.data, off)
        return self.data[off:off + size]

    def _parse_file_format(self):
        """Encryption + compression descriptor (key 0x000003FF)."""
        self.encryption = ENC_NONE
        self.compression = CMP_NONE
        self.basic_blocks = []      # [(data_size, zero_size)] for BASIC
        self.lzx_window = 0
        self.lzx_first_block = None  # (block_size, hash20)
        off = self.opt.get(K_FILE_FORMAT)
        if off is None:
            return
        d = self.data
        # struct: u32 info_size, u16 encryption, u16 compression, then variable.
        self.encryption  = _u16(d, off + 4)
        self.compression = _u16(d, off + 6)
        q = off + 8
        if self.compression == CMP_BASIC:
            # array of (data_size, zero_size) up to info_size
            info_size = _u32(d, off)
            end = off + info_size
            while q + 8 <= end:
                ds, zs = struct.unpack_from(">II", d, q)
                self.basic_blocks.append((ds, zs))
                q += 8
        elif self.compression == CMP_NORMAL:
            self.lzx_window = _u32(d, q)
            block_size = _u32(d, q + 4)
            block_hash = d[q + 8:q + 28]
            self.lzx_first_block = (block_size, block_hash)

    # -- security info (key material) ---------------------------------------
    def image_size(self):
        """Uncompressed image size (security_info + 0x04) — the LZX output length."""
        o = self.security_offset + 0x04
        if o + 4 > len(self.data):
            return None
        return _u32(self.data, o)

    def _encrypted_file_key(self):
        """The 16-byte AES session key, still wrapped by the console key.

        The offset into the security-info block is format-specific (confirmed
        against the Xenia-derived layout): XEX2 stores it at +0x150, XEX1 at
        +0x134. Other magics are not assumed — we return None rather than read a
        wrong offset."""
        if self.magic == b"XEX2":
            key_off = 0x150
        elif self.magic == b"XEX1":
            key_off = 0x134
        else:
            return None
        o = self.security_offset + key_off
        if o + 16 > len(self.data):
            return None
        return self.data[o:o + 16]

    def _session_keys(self):
        """Candidate cleartext session keys: retail first, then devkit. We do
        not choose blindly — the caller keeps whichever decrypts to a valid PE."""
        wrapped = self._encrypted_file_key()
        if wrapped is None:
            return []
        out = []
        for base_key in (RETAIL_KEY, DEVKIT_KEY, XEX1_KEY):
            out.append(_aes_cbc_decrypt(base_key, wrapped, iv=b"\x00" * 16))
        return out

    # -- the payload ---------------------------------------------------------
    def basefile(self):
        """The decoded embedded image (an ordinary PE, but PowerPC inside).

        Runs the full front stage: decrypt (if ENC_NORMAL) then decompress (per
        the file-format descriptor). Raises XEXError with a specific reason when
        a stage is not yet validated for this image, rather than returning
        bytes we cannot stand behind.
        """
        raw = self.data[self.data_offset:]

        # 1) decrypt --------------------------------------------------------
        candidates = []                                  # (payload, title_key)
        if self.encryption == ENC_NORMAL:
            keys = self._session_keys()
            if not keys:
                raise XEXError("encrypted image but no session key found "
                               "(security-info offset unknown for this XEX kind)")
            for k in keys:
                candidates.append((_aes_cbc_decrypt(k, raw, iv=b"\x00" * 16), k))
        else:
            candidates.append((raw, None))

        # 2) decompress + accept the candidate that yields a real PE --------
        last_err = None
        for cand, key in candidates:
            try:
                img = self._decompress(cand)
            except (XEXError, ValueError) as e:          # wrong key -> try next
                last_err = e
                continue
            if img[:2] == b"MZ" or _looks_like_pe(img):
                self._title_key = key                    # remember the working key
                return img
            last_err = XEXError("decoded payload is not a PE image "
                                "(wrong key, or LZX path needs validation)")
        raise last_err or XEXError("could not decode basefile")

    def _decompress(self, buf):
        if self.compression in (CMP_NONE,):
            return buf
        if self.compression == CMP_BASIC:
            out = bytearray()
            p = 0
            for ds, zs in self.basic_blocks:
                out += buf[p:p + ds]
                out += b"\x00" * zs
                p += ds
            return bytes(out)
        if self.compression == CMP_NORMAL:
            # LZX decode lives in _lzx.py. The XEX block framing (u32 size +
            # SHA-1 per block, u16 chunk lengths) is exact and self-checking;
            # the LZXD codec is validated end-to-end against the Tetris ground
            # truth (see README). Guarded so header-only callers never trip it.
            try:
                from _lzx import lzx_decompress_xex
            except Exception:
                raise XEXError("LZX-compressed image: decompressor not wired "
                               "(_lzx.py missing) — see README 'front stage'")
            out_len = self.image_size() or 0
            return lzx_decompress_xex(buf, self.lzx_window,
                                      self.lzx_first_block, out_len)
        if self.compression == CMP_DELTA:
            raise XEXError("delta-compressed image (title update patch) — "
                           "apply against the base title first; not supported")
        raise XEXError(f"unknown compression type {self.compression}")

    # -- convenience over the embedded PE -----------------------------------
    def sections(self):
        """The embedded PE's section table: [{name, vaddr, vsize, raw_off,
        raw_size, code}]. This is where .text (PowerPC) is found."""
        img = self.basefile()
        return _pe_sections(img)

    def code_bytes(self):
        try:
            return sum(s["raw_size"] for s in self.sections() if s["code"])
        except XEXError:
            return 0

    def imports(self):
        """Parse the XEX import libraries (opt header 0x000103FF).

        Returns {library_name: [(guest_addr, ordinal, kind)]} where kind is
        'thunk' (a called function — becomes a host shim) or 'slot' (a data/
        variable import). The game calls into the thunk addresses, so those are
        exactly the functions the runtime must provide."""
        off = self.opt.get(K_IMPORT_LIBS)
        if off is None or off + 12 > len(self.data):
            return {}
        d = self.data
        str_table_size = _u32(d, off + 4)
        lib_count = _u32(d, off + 8)
        st = d[off + 12: off + 12 + str_table_size]
        names = [n.decode("latin1") for n in st.split(b"\x00") if n]

        # each import address points into the loaded image; read the encoded
        # (kind<<24 | ordinal) value there (flat/virtual layout).
        try:
            img = self.basefile()
        except XEXError:
            img = None
        base = self.image_base or 0

        out = {}
        p = off + 12 + str_table_size
        for i in range(lib_count):
            if p + 40 > len(d):
                break
            lib_size = _u32(d, p)
            name_index = _u16(d, p + 36)
            count = _u16(d, p + 38)
            libname = names[name_index] if name_index < len(names) else \
                (names[i] if i < len(names) else "library%d" % i)
            recs = []
            for j in range(count):
                ao = p + 40 + j * 4
                if ao + 4 > len(d):
                    break
                addr = _u32(d, ao)
                ordv = kind = None
                if img is not None and base <= addr < base + len(img):
                    val = struct.unpack_from(">I", img, addr - base)[0]
                    kind = "thunk" if (val >> 24) == 1 else "slot"
                    ordv = val & 0xFFFF
                recs.append((addr, ordv, kind))
            out.setdefault(libname, []).extend(recs)
            p += lib_size if lib_size else (40 + count * 4)
        return out

    def module_name(self):
        blob = self._opt_blob(K_ORIGINAL_PENAME)
        if blob and len(blob) > 4:
            name = blob[4:].split(b"\x00", 1)[0]
            try:
                return name.decode("ascii", "replace")
            except Exception:
                pass
        import os
        return os.path.basename(self.path)

    def summary(self):
        enc = {ENC_NONE: "none", ENC_NORMAL: "AES-128"}.get(self.encryption, "?")
        cmp = {CMP_NONE: "none", CMP_BASIC: "basic", CMP_NORMAL: "LZX",
               CMP_DELTA: "delta"}.get(self.compression, "?")
        s = {
            "magic": self.magic.decode("ascii", "replace"),
            "entry_point": self.entry_point,
            "image_base": self.image_base,
            "encryption": enc,
            "compression": cmp,
            "data_offset": self.data_offset,
            "opt_headers": len(self.opt),
            "module_type": self.module_type,
            "module_flags": self.module_flag_names(),
        }
        if self.execution_info:
            s["title_id"] = self.execution_info["title_id"]
            s["version"] = self.execution_info["version_str"]
            s["media_id"] = self.execution_info["media_id"]
        if self.delta_descriptor:
            s["delta"] = self.delta_descriptor
        return s


def open_xex(path):
    return XEXFile(path)


# --------------------------------------------------------------------------
# Embedded-PE helpers (the payload is a normal PE; machine is PowerPC BE).
# --------------------------------------------------------------------------
_IMAGE_SCN_CNT_CODE = 0x00000020
_IMAGE_SCN_MEM_EXECUTE = 0x20000000
_MACHINE_POWERPCBE = 0x01F2   # IMAGE_FILE_MACHINE_POWERPCBE (Xbox 360 code)


def _resource_ext(data):
    """Best-effort file extension for an extracted resource, by leading magic.
    Only well-known, verified magics get a real extension; everything else is
    left as .bin rather than guessed. XUIZ = the packed XUI media archive (.xzp);
    XDBF = the Xbox Data Base Format container the SPA game-config uses (.spa)."""
    head = bytes(data[:4])
    if head == b"XUIZ":
        return ".xzp"
    if head == b"XDBF":
        return ".spa"
    return ".bin"


def _looks_like_pe(img):
    if len(img) < 0x40 or img[:2] != b"MZ":
        return False
    try:
        e = struct.unpack_from("<I", img, 0x3C)[0]
        return img[e:e + 4] == b"PE\0\0"
    except Exception:
        return False


def _pe_sections(img):
    if not _looks_like_pe(img):
        raise XEXError("embedded payload is not a parseable PE")
    e = struct.unpack_from("<I", img, 0x3C)[0]
    coff = e + 4
    machine, nsec = struct.unpack_from("<HH", img, coff)
    opt_size = struct.unpack_from("<H", img, coff + 16)[0]
    sec_off = coff + 20 + opt_size
    out = []
    for i in range(nsec):
        rec = img[sec_off + i * 40: sec_off + i * 40 + 40]
        if len(rec) < 40:
            break
        name = rec[:8].split(b"\x00", 1)[0].decode("ascii", "replace")
        vsize, vaddr, raw_size, raw_off = struct.unpack_from("<IIII", rec, 8)
        chars = struct.unpack_from("<I", rec, 36)[0]
        code = bool(chars & (_IMAGE_SCN_CNT_CODE | _IMAGE_SCN_MEM_EXECUTE))
        out.append({"name": name, "vaddr": vaddr, "vsize": vsize,
                    "raw_off": raw_off, "raw_size": raw_size, "code": code,
                    "machine": machine})
    return out


# --------------------------------------------------------------------------
# AES-128-CBC decrypt. Prefer a real crypto lib; fall back to a small,
# self-contained pure-Python implementation so the frozen EXE needs no wheel.
# --------------------------------------------------------------------------
def _aes_cbc_decrypt(key, data, iv):
    # trim to a block multiple; XEX payloads are block-aligned but be safe.
    data = data[:len(data) - (len(data) % 16)]
    try:
        from Crypto.Cipher import AES
        return AES.new(key, AES.MODE_CBC, iv).decrypt(data)
    except Exception:
        pass
    try:
        from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
        c = Cipher(algorithms.AES(key), modes.CBC(iv))
        dec = c.decryptor()
        return dec.update(data) + dec.finalize()
    except Exception:
        pass
    return _PurePyAES(key).cbc_decrypt(data, iv)


def _aes_ecb_decrypt_block(key, block):
    """Decrypt a single 16-byte block (ECB) — used for XEX key un-wrapping."""
    try:
        from Crypto.Cipher import AES
        return AES.new(key, AES.MODE_ECB).decrypt(block)
    except Exception:
        pass
    try:
        from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
        dec = Cipher(algorithms.AES(key), modes.ECB()).decryptor()
        return dec.update(block) + dec.finalize()
    except Exception:
        pass
    return bytes(_PurePyAES(key)._dec_block(bytearray(block)))


def _aes_cbc_encrypt(key, data, iv):
    data = data[:len(data) - (len(data) % 16)]
    try:
        from Crypto.Cipher import AES
        return AES.new(key, AES.MODE_CBC, iv).encrypt(data)
    except Exception:
        pass
    from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
    enc = Cipher(algorithms.AES(key), modes.CBC(iv)).encryptor()
    return enc.update(data) + enc.finalize()


def _aes_ecb_encrypt_block(key, block):
    try:
        from Crypto.Cipher import AES
        return AES.new(key, AES.MODE_ECB).encrypt(block)
    except Exception:
        pass
    from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
    enc = Cipher(algorithms.AES(key), modes.ECB()).encryptor()
    return enc.update(block) + enc.finalize()


# --------------------------------------------------------------------------
# Delta-patch application (.xexp title updates)
#
# Reconstruct a decrypted, uncompressed standalone XEX from a base XEX plus a
# delta patch. Clean-room port of the documented algorithm (Xenia / XenonRecomp
# xex_patcher), using this project's own LZX (with the reference-window mode),
# AES and SHA-1. Verified byte-identical against XexTool-RE's `patch`.
# --------------------------------------------------------------------------
_K_FILE_FORMAT = 0x000003FF
_K_DELTA_PATCH = 0x000005FF


def _opt_off(m, key):
    """File offset of a size-prefixed optional header's payload, or None."""
    count = _u32(m, 0x14)
    for i in range(count):
        if _u32(m, 0x18 + i * 8) == key:
            return _u32(m, 0x18 + i * 8 + 4)
    return None


def _lzx_delta_apply(patch, patch_len, window_size, dst, dst_base=0):
    """Apply one stream of delta-patch records into bytearray `dst`. Records
    address the destination relative to `dst_base`."""
    from _lzx import lzxd_decompress
    wbits = window_size.bit_length() - 1
    cur, end = 0, patch_len
    while cur < end:
        old_addr = dst_base + _u32(patch, cur)
        new_addr = dst_base + _u32(patch, cur + 4)
        unc_len = _u16(patch, cur + 8)
        cmp_len = _u16(patch, cur + 10)
        if cmp_len == 0 and unc_len == 0 and new_addr == dst_base and old_addr == dst_base:
            break
        if cmp_len == 0:
            dst[new_addr:new_addr + unc_len] = b"\x00" * unc_len
            cur += 12
        elif cmp_len == 1:
            dst[new_addr:new_addr + unc_len] = bytes(dst[old_addr:old_addr + unc_len])
            cur += 12
        else:
            ref = bytes(dst[old_addr:old_addr + unc_len])
            block = bytes(patch[cur + 12:cur + 12 + cmp_len])
            out = lzxd_decompress(block, wbits, unc_len, ref_data=ref)
            dst[new_addr:new_addr + unc_len] = out
            cur += 12 + cmp_len


def apply_delta_patch(base, patch):
    """Apply a .xexp delta `patch` (bytes) to a `base` XEX (bytes); return the
    reconstructed decrypted+uncompressed standalone XEX (bytes)."""
    b, p = bytes(base), bytes(patch)
    if len(b) < 0x18 or b[:4] != b"XEX2":
        raise XEXError("base is not a XEX2 file")
    if len(p) < 0x18 or p[:4] != b"XEX2":
        raise XEXError("patch is not a XEX2 file")
    if (_u32(p, 4) & 0x70) == 0:
        raise XEXError("not a patch file (module flags lack a patch bit)")

    pd_off = _opt_off(p, _K_DELTA_PATCH)
    pffi = _opt_off(p, _K_FILE_FORMAT)
    if pd_off is None or pffi is None:
        raise XEXError("patch missing delta/file-format descriptors")
    if _u16(p, pffi + 6) != 3:
        raise XEXError("patch is not delta-compressed")

    base_hdr_size = _u32(b, 8)
    size_tgt_hdr = _u32(p, pd_off + 0x30)
    dh_src_off = _u32(p, pd_off + 0x34)
    dh_src_sz = _u32(p, pd_off + 0x38)
    dh_tgt_off = _u32(p, pd_off + 0x3C)
    window = _u32(p, pffi + 8)

    if dh_src_off > base_hdr_size or dh_src_sz > base_hdr_size - dh_src_off:
        raise XEXError("patch incompatible (header source out of range)")

    hdr_tgt = size_tgt_hdr or (dh_tgt_off + dh_src_sz)

    # --- rebuild the headers ---
    new_hdr = max(hdr_tgt, base_hdr_size)
    out = bytearray(new_hdr)
    out[:min(base_hdr_size, new_hdr)] = b[:min(base_hdr_size, new_hdr)]
    if dh_src_off > 0:
        out[dh_tgt_off:dh_tgt_off + dh_src_sz] = bytes(out[dh_src_off:dh_src_off + dh_src_sz])
    _lzx_delta_apply(p[pd_off + 0x4C:], _u32(p, pd_off), window, out)
    del out[hdr_tgt:]

    # --- append the base data region after the new headers ---
    new_sec_off = _u32(out, 0x10)
    new_img_size = _u32(out, new_sec_off + 4)
    base_img_size0 = _u32(b, _u32(b, 0x10) + 4)
    work = hdr_tgt + max(new_img_size, base_img_size0) + 0x1000
    out.extend(b"\x00" * (work - len(out)))
    tail = b[base_hdr_size:]
    out[hdr_tgt:hdr_tgt + len(tail)] = tail

    # --- derive & validate the image key (try retail / devkit / xex1 KEKs) ---
    base_sec = _u32(b, 0x10)
    patch_sec = _u32(p, 0x10)
    orig_key = patch_key = None
    for kek in (RETAIL_KEY, DEVKIT_KEY, XEX1_KEY):
        ok = _aes_ecb_decrypt_block(kek, b[base_sec + 0x150:base_sec + 0x160])
        nk = _aes_ecb_decrypt_block(kek, bytes(out[new_sec_off + 0x150:new_sec_off + 0x160]))
        pk = _aes_ecb_decrypt_block(nk, p[patch_sec + 0x150:patch_sec + 0x160])
        img_key_src = _aes_ecb_decrypt_block(nk, p[pd_off + 0x20:pd_off + 0x30])
        if img_key_src == ok:
            orig_key, patch_key = ok, pk
            break
    if orig_key is None:
        raise XEXError("patch incompatible with this base XEX (no key match)")

    # --- decrypt + decompress the base image in place ---
    bffi = _opt_off(b, _K_FILE_FORMAT)
    if bffi is None:
        raise XEXError("base missing file-format info")
    base_enc = _u16(b, bffi + 4)
    base_cmp = _u16(b, bffi + 6)
    if base_enc == 1:
        n = (len(tail)) & ~15
        out[hdr_tgt:hdr_tgt + n] = _aes_cbc_decrypt(orig_key, bytes(out[hdr_tgt:hdr_tgt + n]), b"\x00" * 16)
    elif base_enc != 0:
        raise XEXError("unsupported base encryption")

    base_img_size = _u32(b, base_sec + 4)
    if base_cmp == 1:                                     # basic: reverse blocks
        bi = bffi + 8
        nblocks = _u32(b, bffi) // 8 - 1
        comp = img = 0
        for i in range(nblocks):
            comp += _u32(b, bi + i * 8)
            img += _u32(b, bi + i * 8) + _u32(b, bi + i * 8 + 4)
        src = hdr_tgt + comp
        dst = hdr_tgt + img
        for i in range(nblocks - 1, -1, -1):
            ds = _u32(b, bi + i * 8)
            zs = _u32(b, bi + i * 8 + 4)
            dst -= zs
            out[dst:dst + zs] = b"\x00" * zs
            dst -= ds
            src -= ds
            out[dst:dst + ds] = bytes(out[src:src + ds])
    elif base_cmp == 2:                                   # normal LZX
        wbits = _u32(b, bffi + 8).bit_length() - 1
        stream = bytearray()
        pos = hdr_tgt
        block_size = _u32(b, bffi + 12)
        while block_size:
            nxt = _u32(out, pos)
            bp = pos + 24
            block_end = pos + block_size
            while bp + 2 <= block_end:
                chunk = (out[bp] << 8) | out[bp + 1]
                bp += 2
                if not chunk:
                    break
                stream += out[bp:bp + chunk]
                bp += chunk
            pos += block_size
            block_size = nxt
        from _lzx import lzxd_decompress
        image = lzxd_decompress(bytes(stream), wbits, base_img_size)
        out[hdr_tgt:hdr_tgt + len(image)] = image
    elif base_cmp != 0:
        raise XEXError("unsupported base compression")

    # the new headers now describe an uncompressed, unencrypted image
    nffi = _opt_off(out, _K_FILE_FORMAT)
    if nffi is not None:
        struct.pack_into(">HH", out, nffi + 4, 0, 0)

    # --- decrypt patch data, then apply the image delta blocks ---
    patch_data = bytearray(p[_u32(p, 8):])
    if _u16(p, pffi + 4) == 1:
        n = len(patch_data) & ~15
        patch_data[:n] = _aes_cbc_decrypt(patch_key, bytes(patch_data[:n]), b"\x00" * 16)
    elif _u16(p, pffi + 4) != 0:
        raise XEXError("unsupported patch encryption")

    new_header_size = _u32(out, 8)
    di_src_off = _u32(p, pd_off + 0x40)
    di_src_sz = _u32(p, pd_off + 0x44)
    di_tgt_off = _u32(p, pd_off + 0x48)
    if di_src_off > 0:
        s = new_header_size
        out[s + di_tgt_off:s + di_tgt_off + di_src_sz] = bytes(
            out[s + di_src_off:s + di_src_off + di_src_sz])

    # image is a view into `out` starting at new_header_size; apply blocks there
    cursor = 0
    cur_block = _u32(p, pffi + 12)
    cur_hash = p[pffi + 16:pffi + 16 + 20]
    while cur_block > 0:
        if hashlib.sha1(bytes(patch_data[cursor:cursor + cur_block])).digest() != cur_hash:
            raise XEXError("patch block hash mismatch")
        nxt = _u32(patch_data, cursor)
        nxt_hash = bytes(patch_data[cursor + 4:cursor + 24])
        cursor += 24
        # delta records address the image relative to new_header_size
        _lzx_delta_apply(patch_data[cursor:cursor + cur_block - 24], cur_block - 24,
                         window, out, dst_base=new_header_size)
        cursor += cur_block - 24
        cur_block = nxt
        cur_hash = nxt_hash

    del out[hdr_tgt + new_img_size:]
    return bytes(out)


class _PurePyAES:
    """Minimal AES-128 decryptor (no external deps). Correctness-first, not
    speed — only used when neither pycryptodome nor cryptography is present."""
    _SBOX = None
    _INV_SBOX = None
    _RCON = [0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36]

    def __init__(self, key):
        if _PurePyAES._SBOX is None:
            _PurePyAES._build_tables()
        self.round_keys = self._expand(key)

    @classmethod
    def _build_tables(cls):
        p = q = 1
        sbox = [0] * 256
        # generate S-box via the standard multiplicative-inverse + affine method
        inv = [0] * 256
        # multiplicative inverse in GF(2^8)
        def xtime_mul(a, b):
            r = 0
            for _ in range(8):
                if b & 1:
                    r ^= a
                hi = a & 0x80
                a = (a << 1) & 0xFF
                if hi:
                    a ^= 0x1B
                b >>= 1
            return r
        # build inverse table by brute force
        for a in range(256):
            for b in range(256):
                if xtime_mul(a, b) == 1:
                    inv[a] = b
                    break
        for a in range(256):
            x = inv[a]
            s = x ^ ((x << 1) | (x >> 7)) ^ ((x << 2) | (x >> 6)) ^ \
                ((x << 3) | (x >> 5)) ^ ((x << 4) | (x >> 4)) ^ 0x63
            sbox[a] = s & 0xFF
        inv_sbox = [0] * 256
        for i, v in enumerate(sbox):
            inv_sbox[v] = i
        cls._SBOX = sbox
        cls._INV_SBOX = inv_sbox
        cls._mul = staticmethod(xtime_mul)

    def _expand(self, key):
        sbox = self._SBOX
        w = [list(key[i:i + 4]) for i in range(0, 16, 4)]
        for i in range(4, 44):
            t = list(w[i - 1])
            if i % 4 == 0:
                t = t[1:] + t[:1]
                t = [sbox[b] for b in t]
                t[0] ^= self._RCON[i // 4 - 1]
            w.append([w[i - 4][j] ^ t[j] for j in range(4)])
        return w

    def _dec_block(self, block):
        s = [list(block[i:i + 4]) for i in range(0, 16, 4)]
        s = self._add_rk(s, 10)
        for rnd in range(9, 0, -1):
            s = self._inv_shift(s)
            s = self._inv_sub(s)
            s = self._add_rk(s, rnd)
            s = self._inv_mix(s)
        s = self._inv_shift(s)
        s = self._inv_sub(s)
        s = self._add_rk(s, 0)
        return bytes(s[i][j] for i in range(4) for j in range(4))

    def _add_rk(self, s, rnd):
        w = self.round_keys
        return [[s[i][j] ^ w[rnd * 4 + i][j] for j in range(4)] for i in range(4)]

    def _inv_sub(self, s):
        ib = self._INV_SBOX
        return [[ib[s[i][j]] for j in range(4)] for i in range(4)]

    def _inv_shift(self, s):
        out = [[0] * 4 for _ in range(4)]
        for r in range(4):
            for c in range(4):
                out[(c + r) % 4][r] = s[c][r]
        return out

    def _inv_mix(self, s):
        m = self._mul
        out = [[0] * 4 for _ in range(4)]
        for c in range(4):
            col = [s[c][r] for r in range(4)]
            out[c][0] = m(col[0], 14) ^ m(col[1], 11) ^ m(col[2], 13) ^ m(col[3], 9)
            out[c][1] = m(col[0], 9) ^ m(col[1], 14) ^ m(col[2], 11) ^ m(col[3], 13)
            out[c][2] = m(col[0], 13) ^ m(col[1], 9) ^ m(col[2], 14) ^ m(col[3], 11)
            out[c][3] = m(col[0], 11) ^ m(col[1], 13) ^ m(col[2], 9) ^ m(col[3], 14)
        return out

    def cbc_decrypt(self, data, iv):
        out = bytearray()
        prev = iv
        for i in range(0, len(data), 16):
            block = data[i:i + 16]
            dec = self._dec_block(block)
            out += bytes(a ^ b for a, b in zip(dec, prev))
            prev = block
        return bytes(out)


_CMP_BASIC = 1


def _header_entries(x):
    """[[key, val, is_blob, body(bytearray)]] for every optional header."""
    src, data_off = x.data, x.data_offset
    ents = []
    for j in range(_u32(src, 0x14)):
        e = 0x18 + j * 8
        key, val = _u32(src, e), _u32(src, e + 4)
        lo = key & 0xFF
        is_blob, body = False, b""
        if lo != 0 and val != 0 and val < data_off:
            sz = _u32(src, val) if lo == 0xFF else lo * 4
            if sz and val + sz <= data_off:
                is_blob, body = True, src[val:val + sz]
        ents.append([key, val, is_blob, bytearray(body)])
    return ents


def _build_header(x, ents, repack=False):
    """Re-serialize the XEX header/directory from `ents`. Returns (hdr, data_off,
    sec). Mirrors XexTool-RE build_header byte-for-byte."""
    src = x.data
    old_sec = _u32(src, 0x10)
    secsz = _u32(src, old_sec)
    ents.sort(key=lambda e: e[0])
    count = len(ents)
    sec = 0x18 + count * 8 + 0x80                      # always a 0x80 zero gap
    import_size = sum(len(e[3]) for e in ents if e[0] == K_IMPORT_LIBS and e[2])
    pos = sec + secsz
    for e in ents:
        if not e[2] or e[0] == K_IMPORT_LIBS:
            continue
        if repack:
            e[1] = pos
        pos = max(pos, e[1] + len(e[3]))
    data_offset = (pos + import_size + 0xFFF) & ~0xFFF
    import_off = data_offset - import_size
    for e in ents:
        if e[0] == K_IMPORT_LIBS and e[2]:
            e[1] = import_off
    hdr = bytearray(data_offset)
    hdr[:0x18] = src[:0x18]
    struct.pack_into(">I", hdr, 0x08, data_offset)
    struct.pack_into(">I", hdr, 0x10, sec)
    struct.pack_into(">I", hdr, 0x14, count)
    hdr[sec:sec + secsz] = src[old_sec:old_sec + secsz]
    for j, (key, val, is_blob, body) in enumerate(ents):
        e = 0x18 + j * 8
        struct.pack_into(">I", hdr, e, key)
        struct.pack_into(">I", hdr, e + 4, val)
        if is_blob:
            hdr[val:val + len(body)] = body
    return hdr, data_offset, sec


def _source_do_sign(x):
    sig = x.data[x.security_offset + 8:x.security_offset + 8 + 256]
    import xex_sign
    return xex_sign.verify_devkit_sig(sig)


def _title_key(x):
    """Cleartext AES title key (unwrap the stored key under the source KEK)."""
    wrapped = x._encrypted_file_key()
    kek = DEVKIT_KEY if _source_do_sign(x) else RETAIL_KEY
    return _aes_cbc_decrypt(kek, wrapped, b"\x00" * 16)


def decompress_to_basic(x, binary):
    """Reconstruct a decrypted XEX in 'basic' compression (-c u), or flat binary
    (-c b, a single data run). Byte-identical to XexTool-RE."""
    import xex_sign
    image = x.basefile()
    G = 0x8000
    if binary:
        runs = [(len(image), 0)]
    else:
        ng = len(image) // G
        def zero_g(g):
            return not any(image[g * G:g * G + G])
        runs = []
        g = 0
        while g < ng:
            d = z = 0
            while g < ng and not zero_g(g):
                d += 1; g += 1
            while g < ng and zero_g(g):
                z += 1; g += 1
            if d or z:
                runs.append((d * G, z * G))
        if not runs:
            runs = [(len(image), 0)]
        tail = len(image) - ng * G
        if tail:
            if runs[-1][1] == 0:
                runs[-1] = (runs[-1][0] + tail, 0)
            else:
                runs.append((tail, 0))
    data = bytearray()
    pos = 0
    for dsz, zsz in runs:
        data += image[pos:pos + dsz]
        pos += dsz + zsz
    enc = x.encryption
    if enc != ENC_NONE:
        n = len(data) & ~15
        data[:n] = _aes_cbc_encrypt(x._title_key, bytes(data[:n]), b"\x00" * 16)
    if len(data) % G:
        data += b"\x00" * (G - (len(data) % G))
    ents = _header_entries(x)
    for e in ents:
        if e[0] != K_FILE_FORMAT:
            continue
        body = bytearray(8 + len(runs) * 8)
        struct.pack_into(">I", body, 0, len(body))
        struct.pack_into(">HH", body, 4, enc, _CMP_BASIC)
        for r, (dsz, zsz) in enumerate(runs):
            struct.pack_into(">II", body, 8 + r * 8, dsz, zsz)
        e[2], e[3] = True, body
    hdr, data_off, sec = _build_header(x, ents, repack=True)
    xex_sign.devkit_sign(hdr, data_off, _source_do_sign(x), sec)
    return bytes(hdr) + bytes(data)


def _strip_min_library_versions(body):
    """Drop each import library's minimum-version requirement and rebuild the
    back-to-front import digest chain. Returns (changed, head_digest) or (False,
    None). Mirrors XexTool-RE strip_min_library_versions."""
    if len(body) < 12:
        return False, None
    str_size = _u32(body, 4)
    count = _u32(body, 8)
    libs = []
    p = 12 + str_size
    for _ in range(count):
        if p + 4 > len(body):
            break
        bsz = _u32(body, p)
        if bsz < 0x28 or p + bsz > len(body):
            return False, None
        libs.append((p, bsz))
        p += bsz
    if not libs:
        return False, None
    for off, _bsz in libs:                              # zero minVersion build+QFE
        struct.pack_into(">I", body, off + 0x20, _u32(body, off + 0x20) & 0xFF000000)
    for i in range(len(libs) - 1, -1, -1):
        off = libs[i][0]
        if i + 1 < len(libs):
            noff, nsz = libs[i + 1]
            body[off + 4:off + 24] = hashlib.sha1(bytes(body[noff + 4:noff + nsz])).digest()
        else:
            body[off + 4:off + 24] = b"\x00" * 20
    head = hashlib.sha1(bytes(body[libs[0][0] + 4:libs[0][0] + libs[0][1]])).digest()
    return True, head


def remove_limitations(x, opts):
    """Strip region/media/limit fields (XexTool -r). opts is a subset of
    a(all)/b(bounding)/d(deviceid)/l(min-lib-versions)/m(media)/r(region)/z(mediaid).

    Like -m/-c, -r re-serializes the basefile before editing: a normal-compressed
    source is recompressed (staying compressed), a basic source stays basic."""
    import xex_sign
    ser = (compress_to_normal(x, x.encryption != ENC_NONE)
           if x.compression == CMP_NORMAL else decompress_to_basic(x, False))
    y = XEXFile(ser)
    src = y.data
    allf = "a" in opts
    has = lambda c: allf or c in opts

    ents = _header_entries(y)
    dropped = [False]
    import_digest = [None]

    def drop(key):
        before = len(ents)
        ents[:] = [e for e in ents if e[0] != key]
        if len(ents) != before:
            dropped[0] = True
    if has("b"):
        drop(K_BOUNDING_PATH)
    if has("d"):
        drop(0x00008105)
    if has("l"):
        for e in ents:
            if e[0] == K_IMPORT_LIBS and e[2]:
                ch, dig = _strip_min_library_versions(e[3])
                if ch:
                    import_digest[0] = dig

    hdr, data_off, sec = _build_header(y, ents, repack=dropped[0])
    img = sec + 8 + 256
    dirty = dropped[0]

    def set32(o, v):
        nonlocal dirty
        if _u32(hdr, o) != v:
            struct.pack_into(">I", hdr, o, v)
            dirty = True
    if has("m"):
        set32(img + 4, _u32(hdr, img + 4) & ~0x08)
        set32(img + 116, 0xFFFFFFFF)
    if has("r"):
        set32(img + 112, 0xFFFFFFFF)
    if has("z"):
        for k in range(16):
            if hdr[img + 56 + k]:
                hdr[img + 56 + k] = 0
                dirty = True
    if import_digest[0] is not None:
        hdr[img + 0x24:img + 0x24 + 20] = import_digest[0]
        dirty = True
    if not dirty:
        return bytes(x.data)
    xex_sign.devkit_sign(hdr, data_off, _source_do_sign(y), sec)
    return bytes(hdr) + ser[y.data_offset:]


def add_bounding_path(x, path):
    """Add a bounding-path optional header (XexTool -a). Distinct code path: it
    pads the file-format-info END to 0x10 and keeps the source data_offset."""
    import xex_sign
    src = x.data
    data_off = x.data_offset
    old_sec = _u32(src, 0x10)
    old_count = _u32(src, 0x14)
    sec_size = _u32(src, old_sec)
    if x._opt_blob(K_BOUNDING_PATH) is not None:
        return bytes(src)                               # already present
    new_count = old_count + 1
    new_sec = 0x18 + new_count * 8 + 0x80
    if new_sec + sec_size > data_off:
        return bytes(src)
    import_off = x.opt.get(K_IMPORT_LIBS, data_off)
    pb = bytearray((4 + len(path) + 1 + 3) & ~3)
    struct.pack_into(">I", pb, 0, len(pb))
    pb[4:4 + len(path)] = path.encode("latin-1")

    ents = []                                           # [key, val, body, is_blob]
    for j in range(old_count):
        e = 0x18 + j * 8
        key, val = _u32(src, e), _u32(src, e + 4)
        is_blob = (key & 0xFF) != 0 and val != 0 and val < data_off and val != import_off
        body = b""
        if is_blob:
            lo = key & 0xFF
            sz = _u32(src, val) if lo == 0xFF else lo * 4
            if val + sz > data_off:
                return bytes(src)
            body = src[val:val + sz]
        ents.append([key, val, bytearray(body), is_blob])
    ents.append([K_BOUNDING_PATH, 0, pb, True])
    ents.sort(key=lambda e: e[0])

    out = bytearray(data_off)
    out[:0x18] = src[:0x18]
    struct.pack_into(">I", out, 0x10, new_sec)
    struct.pack_into(">I", out, 0x14, new_count)
    out[new_sec:new_sec + sec_size] = src[old_sec:old_sec + sec_size]
    out[import_off:data_off] = src[import_off:data_off]
    pos = new_sec + sec_size
    for j, e in enumerate(ents):
        key, val, body, is_blob = e
        if is_blob:
            if pos + len(body) > import_off:
                return bytes(src)
            out[pos:pos + len(body)] = body
            val = pos
            pos += len(body)
            if key == K_FILE_FORMAT:
                pos = (pos + 0xF) & ~0xF
        ee = 0x18 + j * 8
        struct.pack_into(">II", out, ee, key, val)
    do_sign = xex_sign.verify_devkit_sig(src[old_sec + 8:old_sec + 8 + 256])
    xex_sign.devkit_sign(out, data_off, do_sign, new_sec)
    out += src[data_off:]
    return bytes(out)


def _source_kek(x):
    """The KEK the source's title key is wrapped under (retail / devkit-zero / xex1)."""
    if x.encryption != ENC_NONE:
        x.basefile()
        wrapped = x._encrypted_file_key()
        for kek in (RETAIL_KEY, DEVKIT_KEY, XEX1_KEY):
            if _aes_cbc_decrypt(kek, wrapped, b"\x00" * 16) == x._title_key:
                return kek
    return DEVKIT_KEY if _source_do_sign(x) else RETAIL_KEY


def convert_machine(x, to_devkit):
    """Convert between devkit and retail machine type (XexTool -m d / -m r):
    re-serialize, rewrap the title key under the target KEK, re-sign. The
    compression format is preserved -- a normal-compressed source is recompressed
    (byte-identical to -c c on the same file), a basic source stays basic."""
    import xex_sign
    out = bytearray(compress_to_normal(x, x.encryption != ENC_NONE)
                    if x.compression == CMP_NORMAL else decompress_to_basic(x, False))
    data_off = _u32(out, 8)
    sec = _u32(out, 0x10)
    img = sec + 8 + 256
    title = _aes_ecb_decrypt_block(_source_kek(x), x.security()["aes_key"])
    wrapped = _aes_ecb_encrypt_block(DEVKIT_KEY if to_devkit else RETAIL_KEY, title)
    out[img + 0x48:img + 0x48 + 16] = wrapped              # image-info aes_key
    xex_sign.devkit_sign(out, data_off, to_devkit, sec)
    return bytes(out)


def set_encryption(x, encrypt):
    """Toggle the stored payload's AES encryption (XexTool -e e / -e u), rebuild
    the header (file-format-info encryption field) and re-sign. Byte-identical."""
    import xex_sign
    if encrypt == (x.encryption != ENC_NONE):
        return bytes(x.data)                            # already in target state
    if x.encryption != ENC_NONE:
        x.basefile()                                    # sets x._title_key
        title_key = x._title_key
    else:
        kek = DEVKIT_KEY if _source_do_sign(x) else RETAIL_KEY
        title_key = _aes_cbc_decrypt(kek, x._encrypted_file_key(), b"\x00" * 16)
    data = bytearray(x.data[x.data_offset:])
    n = len(data) & ~15
    iv = b"\x00" * 16
    data[:n] = (_aes_cbc_encrypt if encrypt else _aes_cbc_decrypt)(title_key, bytes(data[:n]), iv)
    ents = _header_entries(x)
    for e in ents:
        if e[0] == K_FILE_FORMAT and len(e[3]) >= 6:
            struct.pack_into(">H", e[3], 4, ENC_NORMAL if encrypt else ENC_NONE)
    hdr, data_off, sec = _build_header(x, ents, repack=False)
    xex_sign.devkit_sign(hdr, data_off, _source_do_sign(x), sec)
    return bytes(hdr) + bytes(data)


def compress_to_normal(x, encrypt, image_in=None):
    """Inverse of decompress_to_basic: LZX-compress the raw image into the XEX
    'normal' (block-framed) format (XexTool -c c). Byte-identical to xextool for
    the data region + file-format-info. `image_in` overrides the source image."""
    import xex_sign
    from lzx_encode import lzx_compress
    image = image_in if image_in is not None else x.basefile()

    # 1. LZX-compress -> u16-BE chunk-framed stream (window 0x8000, no E8).
    stream = lzx_compress(image, 0x8000, 0)

    # 2. Pack whole chunks into storage blocks (each < 0x10000-24 payload bytes).
    CAP = 0x10000 - 24
    contents = []
    cur = bytearray()
    o = 0
    while o + 2 <= len(stream):
        clen = (stream[o] << 8) | stream[o + 1]
        whole = 2 + clen
        if cur and len(cur) + whole > CAP:
            contents.append(cur)
            cur = bytearray()
        cur += stream[o:o + whole]
        o += whole
    if cur:
        contents.append(cur)

    # 3. Build blocks back-to-front, chaining SHA-1(next block) into each header.
    nb = len(contents)
    blocks = [None] * nb
    next_size = 0
    next_hash = b"\x00" * 20
    for i in range(nb - 1, -1, -1):
        used = len(contents[i])
        bsize = (used + 24 + 0x7FF) & ~0x7FF
        blk = bytearray(bsize)
        struct.pack_into(">I", blk, 0, next_size)
        blk[4:24] = next_hash
        blk[24:24 + used] = contents[i]
        next_size = bsize
        next_hash = hashlib.sha1(bytes(blk)).digest()
        blocks[i] = blk
    first_block_size = len(blocks[0]) if nb else 0
    first_hash = hashlib.sha1(bytes(blocks[0])).digest() if nb else b"\x00" * 20

    # 4. Optional AES-CBC encryption of the whole compressed data stream.
    data = bytearray()
    for blk in blocks:
        data += blk
    if encrypt:
        # Title key = security-info AES key unwrapped under the source KEK
        # (works whether or not the source itself was encrypted).
        title_key = _aes_ecb_decrypt_block(_source_kek(x), x.security()["aes_key"])
        n = len(data) & ~15
        data[:n] = _aes_cbc_encrypt(title_key, bytes(data[:n]), b"\x00" * 16)

    # 5. Rebuild the 0x24-byte file-format-info describing the new stream.
    ents = _header_entries(x)
    for e in ents:
        if e[0] != K_FILE_FORMAT:
            continue
        body = bytearray(0x24)
        struct.pack_into(">I", body, 0, 0x24)
        struct.pack_into(">HH", body, 4, ENC_NORMAL if encrypt else ENC_NONE, CMP_NORMAL)
        struct.pack_into(">I", body, 8, 0x8000)
        struct.pack_into(">I", body, 12, first_block_size)
        body[16:36] = first_hash
        e[2], e[3] = True, body

    # -c c does NOT repack: the FFI is rewritten in place. Only if the new 0x24
    # FFI would overlap the following blob is the tail relaid from
    # round_up(ffi_end, 0x10) (matches xextool's -a padding).
    ffi_off = next((e[1] for e in ents if e[0] == K_FILE_FORMAT), 0)
    if ffi_off:
        ffi_end = ffi_off + 0x24
        overlap = any(e[2] and e[0] != K_IMPORT_LIBS and ffi_off < e[1] < ffi_end
                      for e in ents)
        if overlap:
            tail = sorted((e for e in ents if e[2] and e[0] != K_IMPORT_LIBS and e[1] > ffi_off),
                          key=lambda e: e[1])
            pos = (ffi_end + 0xF) & ~0xF
            for e in tail:
                e[1] = pos
                pos += len(e[3])

    hdr, data_off, sec = _build_header(x, ents, repack=False)

    # 6. HeaderHash + signature (devkit key only when the source was devkit-signed).
    devkit = xex_sign.verify_devkit_sig(bytes(hdr[sec + 8:sec + 8 + 256]))
    xex_sign.devkit_sign(hdr, data_off, devkit, sec)
    return bytes(hdr) + bytes(data)


def _pe_sections_idc(img, base):
    """[(name, start, end, code, perms)] with IDA perms (exec1/write2/read4).
    code is the MEM_EXECUTE bit only, matching XexTool -i."""
    out = []
    if len(img) < 0x40 or img[:2] != b"MZ":
        return out
    pe = struct.unpack_from("<I", img, 0x3C)[0]
    if pe + 24 > len(img):
        return out
    nsec = struct.unpack_from("<H", img, pe + 6)[0]
    optsz = struct.unpack_from("<H", img, pe + 20)[0]
    st = pe + 24 + optsz
    for i in range(nsec):
        e = st + i * 40
        if e + 40 > len(img):
            break
        name = img[e:e + 8].split(b"\x00", 1)[0].decode("latin-1", "replace")
        vsize, vaddr = struct.unpack_from("<II", img, e + 8)
        ch = struct.unpack_from("<I", img, e + 36)[0]
        perms = (1 if ch & 0x20000000 else 0) | (2 if ch & 0x80000000 else 0) | \
                (4 if ch & 0x40000000 else 0)
        out.append((name, base + vaddr, base + vaddr + vsize, bool(ch & 0x20000000), perms))
    return out


def _mangle(s):
    return "".join(c if c.isalnum() else "_" for c in s)


def make_idc(x):
    """Generate an IDA IDC script for the basefile (XexTool -i). Byte-identical
    (CRLF), boilerplate verbatim + derived sections/resources/imports/exports."""
    import idc_boilerplate as ib
    img = x.basefile()
    base = x.security()["load_address"]
    res = x.resources()
    first_res = min((r[1] for r in res), default=0xFFFFFFFF)

    body = "static SetupSections()\n{\n    auto addr;\n\n"
    idx = 1
    for name, start, end, code, perms in _pe_sections_idc(img, base):
        if start >= first_res:
            continue
        body += '    SetupSection(0x%08X, 0x%08X, "%s", %d, "%s", %d);\n' % (
            start, end, "CODE" if code else "DATA", perms, name, idx)
        idx += 1
    body += ('\n    // remove unused "leftovers" of the original binary segment\n'
             "    while( (addr = SegByBase(0)) != BADADDR )\n"
             "        DelSeg(addr, SEGMOD_KILL|SEGMOD_SILENT);\n}\n\n")
    body += "static SetupResources()\n{\n"
    for name, addr, size in res:
        body += '    SetupSection(0x%08X, 0x%08X, "DATA", 4, "%s", %d);\n' % (
            addr, addr + size, name, idx)
        idx += 1
    body += "}\n" + ib.kIdcMid1

    raw = x.data
    chunk_names = []
    imp = x.opt.get(K_IMPORT_LIBS)
    if imp is not None:
        str_size = _u32(raw, imp + 4)
        nlibs = _u32(raw, imp + 8)
        st = raw[imp + 12:imp + 12 + str_size]
        names = [n.decode("latin-1") for n in st.split(b"\x00") if n]
        p = imp + 12 + str_size

        def word_at(va):
            o = va - base
            return _u32(img, o) if 0 <= o and o + 4 <= len(img) else 0
        for L in range(nlibs):
            if p + 0x28 > len(raw):
                break
            bsz = _u32(raw, p)
            ncount = _u16(raw, p + 0x26)
            name_index = _u16(raw, p + 0x24)
            lib = names[name_index] if name_index < len(names) else ""
            recs = [_u32(raw, p + 0x28 + i * 4) for i in range(ncount)]
            for c0 in range(0, ncount, 256):
                c1 = min(c0 + 256, ncount)
                fname = "setupImports_%u_%s_%u" % (L, _mangle(lib), c0 // 256)
                chunk_names.append(fname)
                body += "static " + fname + "()\n{\n"
                i = c0
                while i < c1:
                    v = word_at(recs[i])
                    ordv = v & 0xFFFF
                    is_func = (i + 1 < c1) and ((word_at(recs[i + 1]) >> 24) == 1) and \
                              ((word_at(recs[i + 1]) & 0xFFFF) == ordv)
                    if is_func:
                        body += '    SetupImportFunc(0x%08X, 0x%08X, 0x%03X, "%s");\n' % (
                            recs[i], recs[i + 1], ordv, lib)
                        i += 2
                    else:
                        body += '    SetupImportData(0x%08X,             0x%03X, "%s");\n' % (
                            recs[i], ordv, lib)
                        i += 1
                body += "}\n\n"
            p += bsz
    body += "static SetupImports()\n{\n"
    for n in chunk_names:
        body += "    " + n + "();\n"
    body += "}\n" + ib.kIdcMid2

    entry = x.entry_point or 0
    body += ("static SetupExports()\n{\n    auto name;\n    name = GetInputFile();\n\n\n"
             "    // set start entry point\n")
    body += '    SetupExportFunc(0x%08X, 0x%08X, "start");\n}\n' % (entry, entry)

    out = ib.kIdcHead + body + ib.kIdcTail
    return out.replace("\n", "\r\n").encode("latin-1", "replace")


def dump_resources(x, outdir):
    """Extract embedded resources (from the resource-info header) out of the
    reconstructed basefile into `outdir`. Returns the count. Mirrors XexTool -d."""
    import os
    res = x.resources()
    if not res:
        return 0
    base = x.basefile()
    load = x.security()["load_address"]
    n = 0
    for name, addr, size in res:
        if addr < load:
            continue
        off = addr - load
        if off > len(base):
            continue
        end = min(off + size, len(base))
        os.makedirs(outdir, exist_ok=True)
        with open(os.path.join(outdir, name), "wb") as f:
            f.write(base[off:end])
        n += 1
    return n


_XML_MEDIA = [(0x01, "Hard Disk"), (0x02, "DVD-X2 (Xbox1 Original Disc)"),
              (0x04, "DVD / CD"), (0x08, "DVD5"), (0x10, "DVD9"),
              (0x20, "System Flash"), (0x80, "Memory Unit"),
              (0x100, "Usb Mass Storage"), (0x02000000, 'Savegame Package ("CONS")')]
_XML_REGION = [(0xFF, "North America"), (0x100, "Japan"), (0x200, "China"),
               (0xFC00, "Rest of Asia"), (0x10000, "Australia / New Zealand"),
               (0xFE0000, "Rest of Europe"), (0xFF000000, "Rest of the World")]


def print_xml(x):
    """XexTool -x machine-readable <XexInfo> XML (all fields). Byte-identical."""
    import xex_sign
    mf = x.module_flags
    bt = ("Patch" if mf & 0x70 else "Exe" if mf & 0x01 else "Dll" if mf & 0x08 else "Other")
    sec = x.security()
    L = ["<XexInfo>", '    <BasefileType format="string">%s</BasefileType>' % bt,
         '    <MediaId format="hex">%s</MediaId>' % sec["media_id"].hex(),
         "    <GameMedias>"]
    for mask, name in _XML_MEDIA:
        if sec["allowed_media"] & mask:
            L.append('        <Media format="string">%s</Media>' % name)
    L.append("    </GameMedias>")
    bp = x.bounding_path()
    if bp:
        L.append("    <BoundingPath>%s</BoundingPath>" % bp)
    L.append("    <GameRegions>")
    if sec["region"] == 0xFFFFFFFF:
        L.append('        <Region format="string">All</Region>')
    else:
        for mask, name in _XML_REGION:
            if (sec["region"] & mask) == mask:
                L.append('        <Region format="string">%s</Region>' % name)
    L.append("    </GameRegions>")
    tid = x.execution_info["title_id"].lower() if x.execution_info else "00000000"
    L.append('    <TitleId format="hex">%s</TitleId>' % tid)
    sig = x.data[x.security_offset + 8:x.security_offset + 8 + 256]
    L.append('    <MachineFormat format="string">%s</MachineFormat>'
             % ("Devkit" if xex_sign.verify_devkit_sig(sig) else "Retail"))
    L.append("</XexInfo>")
    return "\n".join(L)


def main():
    # `xex info <file.xex> [flags]` — alias for the default read (so the GUI can
    # offer a uniform Operation selector); just drop the token and fall through.
    if len(sys.argv) >= 2 and sys.argv[1] == "info":
        del sys.argv[1]
    # `xex idc <file.xex> <out.idc>` — IDA IDC script (XexTool -i).
    if len(sys.argv) >= 2 and sys.argv[1] == "idc":
        if len(sys.argv) < 4:
            print("usage: xex idc <file.xex> <out.idc>"); sys.exit(2)
        with open(sys.argv[3], "wb") as f:
            f.write(make_idc(open_xex(sys.argv[2])))
        print("idc -> %s" % sys.argv[3])
        return
    # `xex xml <file.xex>` — machine-readable XML (XexTool -x).
    if len(sys.argv) >= 2 and sys.argv[1] == "xml":
        if len(sys.argv) < 3:
            print("usage: xex xml <file.xex>"); sys.exit(2)
        print(print_xml(open_xex(sys.argv[2])))
        return
    # `xex remove-limits <in.xex> <opts> <out.xex>` — strip limits (XexTool -r).
    if len(sys.argv) >= 2 and sys.argv[1] in ("remove-limits", "removelimits"):
        if len(sys.argv) < 5:
            print("usage: xex remove-limits <in.xex> <opts a/b/d/l/m/r/z> <out.xex>"); sys.exit(2)
        out = remove_limitations(open_xex(sys.argv[2]), sys.argv[3])
        with open(sys.argv[4], "wb") as f:
            f.write(out)
        print("remove-limits %s -> %s (%d bytes)" % (sys.argv[3], sys.argv[4], len(out)))
        return
    # `xex bounding <in.xex> <path> <out.xex>` — add a bounding path (XexTool -a).
    if len(sys.argv) >= 2 and sys.argv[1] == "bounding":
        if len(sys.argv) < 5:
            print("usage: xex bounding <in.xex> <path> <out.xex>"); sys.exit(2)
        out = add_bounding_path(open_xex(sys.argv[2]), sys.argv[3])
        with open(sys.argv[4], "wb") as f:
            f.write(out)
        print("bounding -> %s (%d bytes)" % (sys.argv[4], len(out)))
        return
    # `xex machine <in.xex> <d|r> <out.xex>` — devkit/retail convert (XexTool -m).
    if len(sys.argv) >= 2 and sys.argv[1] == "machine":
        if len(sys.argv) < 5 or sys.argv[3] not in ("d", "r"):
            print("usage: xex machine <in.xex> <d|r> <out.xex>"); sys.exit(2)
        out = convert_machine(open_xex(sys.argv[2]), to_devkit=(sys.argv[3] == "d"))
        with open(sys.argv[4], "wb") as f:
            f.write(out)
        print("machine %s -> %s (%d bytes)" % (sys.argv[3], sys.argv[4], len(out)))
        return
    # `xex encrypt|decrypt <in.xex> <out.xex>` — toggle payload AES (XexTool -e).
    if len(sys.argv) >= 2 and sys.argv[1] in ("encrypt", "decrypt"):
        if len(sys.argv) < 4:
            print("usage: xex %s <in.xex> <out.xex>" % sys.argv[1]); sys.exit(2)
        out = set_encryption(open_xex(sys.argv[2]), encrypt=(sys.argv[1] == "encrypt"))
        with open(sys.argv[3], "wb") as f:
            f.write(out)
        print("%s -> %s (%d bytes)" % (sys.argv[1], sys.argv[3], len(out)))
        return
    # `xex flat|decompress <in.xex> <out.xex>` — basic/flat re-serialization.
    if len(sys.argv) >= 2 and sys.argv[1] in ("flat", "decompress"):
        if len(sys.argv) < 4:
            print("usage: xex %s <in.xex> <out.xex>" % sys.argv[1]); sys.exit(2)
        out = decompress_to_basic(open_xex(sys.argv[2]), binary=(sys.argv[1] == "flat"))
        with open(sys.argv[3], "wb") as f:
            f.write(out)
        print("%s -> %s (%d bytes)" % (sys.argv[1], sys.argv[3], len(out)))
        return
    # `xex compress <in.xex> <out.xex> [-e]` — LZX 'normal' compression (XexTool -c c).
    if len(sys.argv) >= 2 and sys.argv[1] == "compress":
        if len(sys.argv) < 4:
            print("usage: xex compress <in.xex> <out.xex> [-e]"); sys.exit(2)
        encrypt = "-e" in sys.argv[4:]
        out = compress_to_normal(open_xex(sys.argv[2]), encrypt=encrypt)
        with open(sys.argv[3], "wb") as f:
            f.write(out)
        print("compress -> %s (%d bytes)" % (sys.argv[3], len(out)))
        return
    # `xex resources <file.xex> [outdir]` — dump embedded resources (XexTool -d).
    if len(sys.argv) >= 2 and sys.argv[1] == "resources":
        if len(sys.argv) < 3:
            print("usage: xex resources <file.xex> [outdir]"); sys.exit(2)
        outdir = sys.argv[3] if len(sys.argv) > 3 else "."
        print("dumped %d resources -> %s" % (dump_resources(open_xex(sys.argv[2]), outdir), outdir))
        return
    # `xex patch <base.xex> <patch.xexp> <out.xex>` — apply a delta patch.
    if len(sys.argv) >= 2 and sys.argv[1] == "patch":
        if len(sys.argv) < 5:
            print("usage: xex patch <base.xex> <patch.xexp> <out.xex>"); sys.exit(2)
        base = open(sys.argv[2], "rb").read()
        patch = open(sys.argv[3], "rb").read()
        out = apply_delta_patch(base, patch)
        with open(sys.argv[4], "wb") as f:
            f.write(out)
        print("applied patch -> %s (%d bytes)" % (sys.argv[4], len(out)))
        return
    import argparse
    ap = argparse.ArgumentParser(description="XEX (Xbox 360 executable) reader")
    ap.add_argument("path")
    ap.add_argument("--sections", action="store_true")
    ap.add_argument("--imports", action="store_true", help="list imported libraries/functions")
    ap.add_argument("--dump", metavar="OUT", help="write decoded basefile to OUT")
    ap.add_argument("--resources", action="store_true",
                    help="list embedded resource sections (media .xzp, title-id .spa, etc.)")
    ap.add_argument("--extract", metavar="NAME",
                    help="extract one resource section by name to --out (default ./NAME<ext>)")
    ap.add_argument("--extract-all", metavar="DIR",
                    help="extract every resource section into DIR")
    ap.add_argument("--out", metavar="FILE", help="output path for --extract")
    args = ap.parse_args()
    x = open_xex(args.path)
    s = x.summary()
    print(f"{args.path}")
    print(f"  magic        {s['magic']}")
    ep = s["entry_point"]
    ib = s["image_base"]
    print(f"  entry point  {('0x%08X' % ep) if ep else '?'}")
    print(f"  image base   {('0x%08X' % ib) if ib else '?'}")
    print(f"  encryption   {s['encryption']}")
    print(f"  compression  {s['compression']}")
    print(f"  module type  {s['module_type']}  [{', '.join(s['module_flags']) or 'none'}]")
    if "title_id" in s:
        print(f"  title id     {s['title_id']}  media {s['media_id']}  version {s['version']}")
    if "delta" in s:
        dd = s["delta"]
        print(f"  delta patch  {dd['source_version']} -> {dd['target_version']}")
        print(f"    base image SHA-1  {dd['source_digest']}")
    print(f"  opt headers  {s['opt_headers']}")
    if args.imports:
        imp = x.imports()
        print("  imports:")
        for lib, recs in imp.items():
            thunks = [r for r in recs if r[2] == "thunk"]
            print("    %-16s %d records (%d functions)" % (lib, len(recs), len(thunks)))
            for addr, ordv, kind in recs[:8]:
                if kind == "thunk":
                    print("      0x%08X  ordinal %-4s  (function)"
                          % (addr, ordv if ordv is not None else "?"))
        return
    if args.resources or args.extract or args.extract_all:
        res = x.resources()
        try:
            if args.resources:
                if not res:
                    print("  resources:  (none)")
                for name, addr, size in res:
                    print("  resource  %-10s addr=0x%08X  size=%-10d (0x%X)"
                          % (name, addr, size, size))
            if args.extract:
                data = x.resource_bytes(args.extract)
                out = args.out or (args.extract + _resource_ext(data))
                with open(out, "wb") as f:
                    f.write(data)
                print("  extracted %r  %d bytes -> %s" % (args.extract, len(data), out))
            if args.extract_all:
                import os
                os.makedirs(args.extract_all, exist_ok=True)
                for name, addr, size in res:
                    data = x.resource_bytes(name)
                    out = os.path.join(args.extract_all, name + _resource_ext(data))
                    with open(out, "wb") as f:
                        f.write(data)
                    print("  extracted %r  %d bytes -> %s" % (name, len(data), out))
                if not res:
                    print("  (no resource sections to extract)")
        except XEXError as e:
            print(f"  [resources] {e}")
            sys.exit(3)
        return
    if args.sections or args.dump:
        try:
            if args.dump:
                img = x.basefile()
                with open(args.dump, "wb") as f:
                    f.write(img)
                print(f"  wrote {len(img):,} bytes -> {args.dump}")
            if args.sections:
                print("  embedded PE sections (code = PowerPC):")
                for sec in x.sections():
                    print("    %-8s va=0x%08X  vsize=%-8d raw=%-8d %s" %
                          (sec["name"], sec["vaddr"], sec["vsize"],
                           sec["raw_size"], "CODE" if sec["code"] else ""))
        except XEXError as e:
            print(f"  [front stage] {e}")
            sys.exit(3)


if __name__ == "__main__":
    main()
