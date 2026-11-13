#!/usr/bin/env python3
"""Makes testdata/tools_goldens.txt: what `codebreaker.exe crypto <tool> ...` (the translated Python) prints and writes for the inputs the tests
of the native crypto tools (tools_*_test.cpp) build, so the native tools are compared with the Python on the same bytes.

    python make_tools_goldens.py <codebreaker.exe> <work folder> [case ...]

Each case is a folder of inputs (made here, and by the tests in C++ the same way), one command, and a record of its stdout, exit code and the
files it wrote (size and SHA-1; text files with CRLF made LF, as the exe writes text the Windows way). The record is "== name" then the stdout
lines, "exit=N", and one "file <path> <size> <sha1>" line per output.

The inputs are deterministic: pattern(n, seed) is byte i = (i * 131 + seed * 17 + (i >> 8) * 7) & 0xFF.
"""
import hashlib
import os
import random
import shutil
import hmac
import struct
import subprocess
import sys
import textwrap


HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.abspath(os.path.join(HERE, "..", "..", "..", "..", "..")))


def td(name):
    """A file of this folder."""
    return open(os.path.join(HERE, name), "rb").read()


def valid_cpu_key(seed):
    from codebreaker.formats.xbox.crypto import cpukey_ecd
    rnd = random.Random(seed)
    while True:
        key = bytearray(rnd.randbytes(16))
        if cpukey_ecd.compute_hamming_weight(bytes(key)) == cpukey_ecd.VALID_HAMMING_WEIGHT:
            return bytes(cpukey_ecd.compute_ecd(key))


def pattern(n, seed):
    return bytes(((i * 131 + seed * 17 + (i >> 8) * 7) & 0xFF) for i in range(n))


def stage(image, at, magic, build, pairing, flags, entry, size, key=None):
    """A bootloader stage header (and the key after it) written into `image`."""
    image[at:at + 16] = magic + struct.pack(">HHHII", build, pairing, flags, entry, size)
    if key is not None:
        image[at + 16:at + 32] = key


def make_nand():
    image = bytearray(0xA1000)
    stage(image, 0x8000, b"CB", 0x1D69, 0x8000, 0x0801, 0x03000000, 0x2000, pattern(16, 3))     # CB: a devkit, manufacturing, CB_A
    stage(image, 0xA000, b"CB", 0x1D6A, 0x0000, 0x0000, 0x03000010, 0x1000)                      # CB_B: no key
    stage(image, 0xB000, b"CD", 0x1D6B, 0x0000, 0x0000, 0x03000020, 0x1000, pattern(16, 4))      # CD
    stage(image, 0x70000, b"CE", 0x2A5F, 0x0000, 0x0001, 0x80000030, 0x3000, pattern(16, 5))     # CE at its usual place
    stage(image, 0x90010, b"CF", 0x2A60, 0x0000, 0x0000, 0x03000040, 0x1000, pattern(16, 6))     # CF found by the scan
    stage(image, 0x91000, b"CG", 0x2A60, 0x0000, 0x0000, 0x03000050, 0x1000, pattern(16, 7))     # CG: the same build as CF, another kind
    stage(image, 0xA0000, b"HV", 0x1000, 0x0000, 0x0000, 0x00000000, 0x40000)                    # HV
    stage(image, 0x80000, b"CD", 0x1D6B, 0x0000, 0x0000, 0x03000060, 0x1000, pattern(16, 8))     # a second CD of the same build: not another stage
    stage(image, 0x82000, b"CB", 0x1D6C, 0x0000, 0x0000, 0x03000070, 0x0, pattern(16, 9))        # a size of 0: not a stage
    return bytes(image)


def xval_encrypt(serial, high, low):
    from Crypto.Cipher import DES
    key = hmac.new(serial + b"\0", b"XBOX360SSB", hashlib.sha1).digest()[:8]
    return DES.new(key, DES.MODE_ECB).encrypt(struct.pack(">LL", high, low)).hex().upper()


def be(fmt, *values):
    return struct.pack(">" + fmt, *values)


def make_keyvault(console_type, region, serial=b"123456789012"):
    kv = bytearray(pattern(0x4000, 11))
    kv[0x18] = 1
    kv[0x1C:0x1E] = be("H", 0x00A5)
    kv[0x1E:0x20] = be("H", 0x0102)
    kv[0x9C:0xA8] = serial
    kv[0xB4:0xB6] = be("H", region)
    kv[0x9E4:0x9E8] = be("I", console_type)
    kv[0x9F0:0x9F4] = be("I", 0x10001)
    return bytes(kv)


def make_smc_plain():
    """A decrypted SMC image with its version record in the tail."""
    smc = bytearray(pattern(0x1800, 12))
    for i in range(len(smc) - 0x400, len(smc)):         # nothing in the tail looks like a version record
        smc[i] = 0xF0 | (smc[i] & 0x0F)
    at = len(smc) - 0x100
    smc[at:at + 4] = bytes([2, 3, 0x11, 4])             # major 2, Jasper, build 0x11, revision 4
    return bytes(smc)


def smc_encrypt(data):
    key = [0x42, 0x75, 0x4E, 0x79]
    out = bytearray(len(data))
    for i in range(len(data)):
        j = data[i] ^ (key[i & 3] & 0xFF)
        mod = (j * 0xFB) & 0xFFFFFFFF
        out[i] = j
        key[(i + 1) & 3] = (key[(i + 1) & 3] + mod) & 0xFFFFFFFF
        key[(i + 2) & 3] = (key[(i + 2) & 3] + (mod >> 8)) & 0xFFFFFFFF
    return bytes(out)


def make_flash_header(config_at, smc_len, smc_at, total):
    nand = bytearray(pattern(total, 13))
    nand[0:2] = b"\xff\x4f"
    nand[0x74:0x78] = be("I", config_at)
    nand[0x78:0x7C] = be("I", smc_len)
    nand[0x7C:0x80] = be("I", smc_at)
    return bytes(nand)


def make_hypervisor(salt):
    hv = bytearray(pattern(0x40000, salt))
    hv[0x10100:0x10130] = bytes([0x55]) * 0x30
    return bytes(hv)


def make_patch_table(patches):
    out = b""
    for address, words in patches:
        out += be("II", address, len(words) // 4) + words
    return out + be("I", 0xFFFFFFFF)


def make_xell(seed, size):
    """An XeLL image: stage 1, the gzip stream of an ELF-like payload (xell_stage2_<seed>.gz, made once here: Python's zlib at level 9) and padding."""
    import gzip
    path = os.path.join(HERE, "xell_stage2_%d.gz" % seed)
    if not os.path.exists(path):
        open(path, "wb").write(gzip.compress(pattern(size, seed), 9, mtime=0))
    gz = td("xell_stage2_%d.gz" % seed)
    stage1 = pattern(16 * 1024, seed + 40)
    stage2_size = 256 * 1024 - 16 * 1024 - 16 - 10 - 8
    assert len(gz) <= stage2_size
    return stage1 + gz + bytes(stage2_size - len(gz)) + pattern(16, seed + 50) + bytes(18)


def make_pe(cabinets, sections=2, machine=0x14C):
    """A minimal i386 PE with section data and the cabinets (a magic and a size in a header) appended."""
    dos = bytearray(0x40)
    dos[0:2] = b"MZ"
    dos[0x3C:0x40] = struct.pack("<I", 0x40)
    optional = bytes(224)
    headers = b"PE\0\0" + struct.pack("<HHIIIHH", machine, sections, 0, 0, 0, len(optional), 0x102)
    table = b""
    raw_at = 0x200
    raw = b""
    for i in range(sections):
        data = pattern(0x100 * (i + 1), 20 + i)
        table += (b".sec%d" % i).ljust(8, b"\0") + struct.pack("<IIIIIIHHI", len(data), 0x1000 * (i + 1), len(data), raw_at, 0, 0, 0, 0, 0x60000020)
        raw += data
        raw_at += len(data)
    head = bytes(dos) + headers + optional + table
    head += bytes(0x200 - len(head))
    exe = head + raw
    for i, size in enumerate(cabinets):
        cab = b"MSCF" + struct.pack("<II", 0, size) + pattern(size - 12, 30 + i)
        exe += cab
    return exe + b"trailing"[:0]


# ---- the cases -------------------------------------------------------------------------------------------------------------------------

def cases():
    out = []

    def case(name, tool, args, files=None, outputs=(), text=(), py=False):
        """py: run with the Python source (python -m codebreaker) where the translated exe cannot (its ctypes key structures)."""
        out.append(dict(name=name, tool=tool, args=args, files=files or {}, outputs=list(outputs), text=set(text), py=py))

    data37 = pattern(37, 1)
    for lang, var in (("python", "Data"), ("c", "Buf"), ("c++", "ExpPubKey"), ("csharp", "Blob"), ("php", "payload"), ("php-old", "payload"), ("py", "x")):
        slug = lang.replace("+", "p").replace("-", "_")
        case("bin2lang_" + slug, "bin2lang", ["{w}/in.bin", "{w}/out.txt", "-l", lang, "-b", "8", "-v", var], {"in.bin": data37}, ["out.txt"], ["out.txt"])
    case("bin2lang_defaults", "bin2lang", ["{w}/in.bin", "{w}/out.txt"], {"in.bin": pattern(40, 2)}, ["out.txt"], ["out.txt"])

    case("bootchain_full", "bootchain", ["{w}/nand.bin"], {"nand.bin": make_nand()})
    case("bootchain_empty", "bootchain", ["{w}/nand.bin"], {"nand.bin": bytes(0x1000)})
    case("bootchain_short", "bootchain", ["{w}/nand.bin"], {"nand.bin": bytes(0x20)})

    for name, key in (("zero_key", "00" * 16), ("not_hex", "zz" * 16), ("short", "abcd"), ("spaced", "00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00")):
        case("cpukey_ecd_" + name, "cpukey_ecd", [key])

    for seed in (1, 2, 3):
        case("cpukey_ecd_valid_%d" % seed, "cpukey_ecd", [valid_cpu_key(seed).hex()])
    flipped = bytearray(valid_cpu_key(4))
    flipped[14] ^= 1
    case("cpukey_ecd_bad_ecd", "cpukey_ecd", [bytes(flipped).hex().upper()])
    changed = bytearray(valid_cpu_key(5))
    changed[0] ^= 0x01
    case("cpukey_ecd_bad_weight", "cpukey_ecd", [bytes(changed).hex()])

    case("fuses_pattern", "fuses", ["{w}/fuses.bin"], {"fuses.bin": pattern(0x60, 2)})
    case("fuses_clean", "fuses", ["{w}/fuses.bin"], {"fuses.bin": bytes(0x60)})
    case("fuses_longer", "fuses", ["{w}/fuses.bin"], {"fuses.bin": pattern(0x80, 3)})

    for name, ctype, region in (("retail", 2, 0x00FF), ("devkit", 1, 0xFF00), ("recovery", 0x40000001, 0x0000), ("unknown", 0x77, 0x0001), ("test_retail", 0x40000002, 0xFFFF)):
        case("keyvault_" + name, "keyvault", ["{w}/kv.bin"], {"kv.bin": make_keyvault(ctype, region)})

    case("flashplan_plan", "flashplan", ["plan", "Corona 4G", "RGH 3"])
    case("flashplan_plan_jasper", "flashplan", ["plan", "Jasper 256MB", "RGH 1.2", "--class", "FreeBoot"])
    case("flashplan_plan_retail", "flashplan", ["plan", "Falcon", "None", "--class", "Retail"])
    case("flashplan_plan_devkit", "flashplan", ["plan", "Trinity", "DevRGL", "--class", "Devkit"])
    case("flashplan_plan_nandmu", "flashplan", ["plan", "Corona", "S-RGH", "--nandmu"])
    case("flashplan_plan_unknown", "flashplan", ["plan", "Mystery Box", "Argon Data", "--class", "JTAG"])
    case("flashplan_image_type", "flashplan", ["image-type", "FreeBoot", "RGH 1"])
    case("flashplan_image_type_devgl", "flashplan", ["image-type", "Devkit", "DevGL"])
    case("flashplan_secure_boot", "flashplan", ["secure-boot", "Corona 4G"])
    case("flashplan_secure_boot_trinity", "flashplan", ["secure-boot", "Trinity"])
    case("flashplan_secure_boot_xenon", "flashplan", ["secure-boot", "Xenon"])
    case("flashplan_cpld", "flashplan", ["cpld", "06E5E093"])
    case("flashplan_cpld_other", "flashplan", ["cpld", "12345678"])
    case("flashplan_motherboards", "flashplan", ["motherboards"])
    case("flashplan_hacks", "flashplan", ["hacks"])

    smc_plain = make_smc_plain()
    smc_enc = smc_encrypt(smc_plain)
    nand = make_flash_header(0x1000, 0x1800, 0x2000, 0x4000)
    nand = nand[:0x2000] + smc_enc + nand[0x2000 + len(smc_enc):]
    case("smc_decrypt", "smc", ["decrypt", "{w}/smc.bin", "{w}/dec.bin"], {"smc.bin": smc_enc}, ["dec.bin"])
    case("smc_encrypt", "smc", ["encrypt", "{w}/smc.bin", "{w}/enc.bin"], {"smc.bin": smc_plain}, ["enc.bin"])
    case("smc_info", "smc", ["info", "{w}/smc.bin"], {"smc.bin": smc_enc})
    case("smc_info_no_version", "smc", ["info", "{w}/smc.bin"], {"smc.bin": pattern(0x500, 14)})
    case("smc_extract", "smc", ["extract", "{w}/nand.bin", "{w}/smc.bin"], {"nand.bin": nand}, ["smc.bin"])
    case("smc_config_nand", "smc", ["config", "{w}/nand.bin"], {"nand.bin": nand})
    case("smc_config_raw", "smc", ["config", "{w}/cfg.bin"], {"cfg.bin": pattern(0x2A0, 15)})
    case("smc_config_short", "smc", ["config", "{w}/cfg.bin"], {"cfg.bin": pattern(10, 16)})

    case("nand_info_small", "nand", ["info", "{w}/dump.bin"], {"dump.bin": pattern(0x200, 17)})
    case("nand_info_tiny", "nand", ["info", "{w}/dump.bin"], {"dump.bin": pattern(0x40, 18)})
    case("nand_extract_hv", "nand", ["extract-hv", "{w}/dump.bin", "{w}/hv.bin"], {"dump.bin": pattern(0x40100, 19)}, ["hv.bin"])
    case("nand_hv_alias", "nand", ["hv", "{w}/dump.bin", "{w}/hv.bin"], {"dump.bin": pattern(0x40000, 20)}, ["hv.bin"])

    hv = make_hypervisor(21)
    case("hypervisor_info", "hypervisor", ["info", "{w}/hv.bin"], {"hv.bin": hv})
    case("hypervisor_clean", "hypervisor", ["clean", "{w}/dumped.bin", "{w}/base.bin", "{w}/clean.bin"], {"dumped.bin": hv, "base.bin": make_hypervisor(22)}, ["clean.bin"])
    case("hypervisor_clean_short_base", "hypervisor", ["clean", "{w}/dumped.bin", "{w}/base.bin", "{w}/clean.bin"],
         {"dumped.bin": hv, "base.bin": pattern(0x12000, 23)}, ["clean.bin"])

    for name, high, low in (("clean", 0, 0), ("invalid", 0xFFFFFFFF, 0xFFFFFFFF), ("some_flags", 0, 0x189), ("all_known_flags", 0, 0x1FFFF),
                            ("unknown_bits", 0, 0x00028000), ("high_word_only", 0x10, 0), ("both_words", 0x10, 0x20)):
        case("xval_" + name, "xval", ["123456789012", xval_encrypt(b"123456789012", high, low)])
    case("xval_dashes", "xval", ["123456789012", "-".join(textwrap.wrap(xval_encrypt(b"123456789012", 0, 0x81), 4))])
    case("xval_other_serial", "xval", ["YYYYYYYYYYYY", xval_encrypt(b"YYYYYYYYYYYY", 0, 0x1100)])

    table_a = make_patch_table([(0x1000, pattern(16, 24)), (0x1008, pattern(16, 25)), (0x2000, pattern(8, 26))])
    table_b = make_patch_table([(0x100, pattern(8, 27)), (0x200, pattern(8, 28))])
    case("patch_checker_default", "patch_checker", [],
         {"Output/NonZero/RGL_flash.bin": table_a, "Output/NonZero/RGL_hdd.bin": table_b, "Output/Zero/VRGL_flash.bin": table_b, "Output/Zero/VRGL_hdd.bin": table_a})

    prv = td("test_rsa_prv.bin")
    case("exp_signer_plain", "exp_signer", ["{w}/payload.bin", "-o", "{w}/out.bin", "-k", "{w}/prv.bin"], {"payload.bin": pattern(100, 31), "prv.bin": prv}, ["out.bin"], py=True)
    case("exp_signer_xbonline", "exp_signer", ["{w}/payload.bin", "-o", "{w}/out.bin", "-k", "{w}/prv.bin", "-i", "0x48564050"],
         {"payload.bin": pattern(0x1000, 32), "prv.bin": prv}, ["out.bin"], py=True)
    case("exp_signer_in_place", "exp_signer", ["{w}/payload.bin", "-k", "{w}/prv.bin"], {"payload.bin": pattern(48, 33), "prv.bin": prv}, ["payload.bin"], py=True)
    case("format_key", "format_key", [], {"Keys/custom_pub.bin": td("test_rsa_pub.bin"), "Output/Zero/.keep": b""}, ["Output/Zero/ExpPubKey.h"], ["Output/Zero/ExpPubKey.h"])

    case("xell_unpack", "xell_unpack", [],
         {"XeLL/xell-1f.bin": make_xell(1, 3000), "XeLL/xell-2f.bin": make_xell(2, 70000), "XeLL/xell-gggggg.bin": make_xell(3, 10)},
         ["XeLL/xell-1f-stage-1.bin", "XeLL/xell-1f-stage-2-compressed.elf", "XeLL/xell-1f-stage-2-decompressed.elf",
          "XeLL/xell-2f-stage-1.bin", "XeLL/xell-2f-stage-2-compressed.elf", "XeLL/xell-2f-stage-2-decompressed.elf",
          "XeLL/xell-gggggg-stage-1.bin", "XeLL/xell-gggggg-stage-2-compressed.elf", "XeLL/xell-gggggg-stage-2-decompressed.elf"])

    case("xdk_extract", "xdk_extract", ["{w}/setup.exe", "{w}/cabs", "-p", "xdk"], {"setup.exe": make_pe([300, 5000, 64])},
         ["cabs/xdk_0.cab", "cabs/xdk_1.cab", "cabs/xdk_2.cab"])
    case("xdk_extract_no_cabs", "xdk_extract", ["{w}/setup.exe", "{w}/cabs"], {"setup.exe": make_pe([])})
    return out


def sha1_of(path, text):
    data = open(path, "rb").read()
    if text:
        data = data.replace(b"\r\n", b"\n")
    return len(data), hashlib.sha1(data).hexdigest()


def run_case(exe, work, case):
    folder = os.path.join(work, case["name"])
    if os.path.isdir(folder):
        shutil.rmtree(folder)
    os.makedirs(folder)
    for rel, data in case["files"].items():
        path = os.path.join(folder, rel)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        open(path, "wb").write(data)
    w = folder.replace("\\", "/")
    args = [a.replace("{w}", w) for a in case["args"]]
    if case["py"]:
        env = dict(os.environ, PYTHONPATH=os.path.abspath(os.path.join(HERE, "..", "..", "..", "..", "..")))
        result = subprocess.run([sys.executable, "-m", "codebreaker", "crypto", case["tool"]] + args, cwd=folder, capture_output=True, env=env)
    else:
        result = subprocess.run([exe, "crypto", case["tool"]] + args, cwd=folder, capture_output=True)
    stdout = result.stdout.decode("utf-8", "replace").replace("\r\n", "\n").replace(w, "<w>")
    lines = ["== " + case["name"]]
    lines += stdout.splitlines()
    lines.append("exit=%d" % result.returncode)
    for rel in case["outputs"]:
        path = os.path.join(folder, rel)
        if os.path.exists(path):
            size, digest = sha1_of(path, rel in case["text"])
            lines.append("file %s %d %s" % (rel, size, digest))
        else:
            lines.append("file %s missing" % rel)
    return "\n".join(lines) + "\n"


def main():
    exe, work = sys.argv[1], sys.argv[2]
    if exe.startswith("@"):   # a file holding the path
        exe = open(exe[1:]).read().strip()
    only = set(sys.argv[3:])
    text = ""
    for c in cases():
        if only and c["name"] not in only:
            continue
        text += run_case(exe, work, c)
    out = os.path.join(os.path.dirname(os.path.abspath(__file__)), "tools_goldens.txt")
    if only:
        sys.stdout.write(text)
    else:
        open(out, "w", newline="\n").write(text)


if __name__ == "__main__":
    main()
