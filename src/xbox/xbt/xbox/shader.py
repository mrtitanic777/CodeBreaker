"""Xenos (Xbox 360 GPU) shader microcode disassembler.

Decodes the ATI Xenos VLIW shader ISA: the control-flow program (exec / loop /
jmp / alloc …) and, for every exec block, the ALU (co-issued vector + scalar)
and vertex/texture fetch instructions it runs. Opcode tables and the bitfield
layout are clean-room from the documented ISA (Xenia's ucode.h / shader
translator).

Encoding: microcode is a big-endian u32 array. Control-flow instructions are
48 bits, packed two per three dwords; ALU and fetch instructions are three
dwords each. An `exec` names a slot `address` and an instruction `count`; each
instruction's type comes from 2 bits of the exec `sequence` (bit0 = fetch).

Commands:
  shader <shader.bin> [--le]     disassemble raw shader microcode
"""

import struct
import sys

CF = {0: "nop", 1: "exec", 2: "exec_end", 3: "cond_exec", 4: "cond_exec_end",
      5: "cond_exec_pred", 6: "cond_exec_pred_end", 7: "loop_start",
      8: "loop_end", 9: "cond_call", 10: "return", 11: "cond_jmp", 12: "alloc",
      13: "cond_exec_pred_clean", 14: "cond_exec_pred_clean_end",
      15: "mark_vs_fetch_done"}
EXEC_OPS = {1, 2, 3, 4, 5, 6, 13, 14}
FETCH = {0: "vfetch_full", 1: "tfetch", 16: "getBCF", 17: "getCompTexLOD",
         18: "getGradients", 19: "getWeights", 24: "setTexLOD",
         25: "setGradientH", 26: "setGradientV"}
SCALAR = {0: "adds", 1: "adds_prev", 2: "muls", 3: "muls_prev", 4: "muls_prev2",
          5: "maxs", 6: "mins", 7: "seqs", 8: "sgts", 9: "sges", 10: "snes",
          11: "frcs", 12: "truncs", 13: "floors", 14: "exp", 15: "logc",
          16: "log", 17: "rcpc", 18: "rcpf", 19: "rcp", 20: "rsqc", 21: "rsqf",
          22: "rsq", 23: "maxAs", 24: "maxAsf", 25: "subs", 26: "subs_prev",
          27: "setpEq", 28: "setpNe", 29: "setpGt", 30: "setpGe", 31: "setpInv",
          32: "setpPop", 33: "setpClr", 34: "setpRstr", 35: "killsEq",
          36: "killsGt", 37: "killsGe", 38: "killsNe", 39: "killsOne",
          40: "sqrt", 42: "mulsc0", 43: "mulsc1", 44: "addsc0", 45: "addsc1",
          46: "subsc0", 47: "subsc1", 48: "sin", 49: "cos", 50: "retain_prev"}
VECTOR = {0: "add", 1: "mul", 2: "max", 3: "min", 4: "seq", 5: "sgt", 6: "sge",
          7: "sne", 8: "frc", 9: "trunc", 10: "floor", 11: "mad", 12: "cndEq",
          13: "cndGe", 14: "cndGt", 15: "dp4", 16: "dp3", 17: "dp2add",
          18: "cube", 19: "max4", 20: "setpEqPush", 21: "setpNePush",
          22: "setpGtPush", 23: "setpGePush", 24: "killEq", 25: "killGt",
          26: "killGe", 27: "killNe", 28: "dst", 29: "maxA"}
_WM = "xyzw"


def _dwords(data, little):
    fmt = ("<" if little else ">") + "I"
    return [struct.unpack_from(fmt, data, i)[0] for i in range(0, len(data) & ~3, 4)]


def _cf_pair(w0, w1, w2):
    return (w0 | ((w1 & 0xFFFF) << 32),
            (w1 >> 16) | (w2 << 16))


def decode_cf(cf):
    op = (cf >> 44) & 0xF
    d = {"op": op, "mn": CF.get(op, "op%d" % op)}
    if op in EXEC_OPS:
        d["address"] = cf & 0xFFF
        d["count"] = (cf >> 12) & 0x7
        d["sequence"] = (cf >> 16) & 0xFFF
    elif op in (7, 8):                                   # loop_start / loop_end
        d["address"] = cf & 0xFFF
    elif op in (9, 11):                                  # cond_call / cond_jmp
        d["address"] = cf & 0xFFF
    return d


def decode_alu(w0, w1, w2):
    return {
        "vector": VECTOR.get((w2 >> 24) & 0x1F, "v%d" % ((w2 >> 24) & 0x1F)),
        "scalar": SCALAR.get((w0 >> 26) & 0x3F, "s%d" % ((w0 >> 26) & 0x3F)),
        "vector_dest": w0 & 0x3F,
        "scalar_dest": (w0 >> 8) & 0x3F,
        "vector_mask": (w0 >> 16) & 0xF,
        "scalar_mask": (w0 >> 20) & 0xF,
        "export": (w0 >> 15) & 1,
        "src1": (w2 >> 16) & 0xFF, "src2": (w2 >> 8) & 0xFF, "src3": w2 & 0xFF,
    }


def decode_fetch(w0, w1, w2):
    op = w0 & 0x1F
    return {"op": op, "mn": FETCH.get(op, "fetch%d" % op),
            "src_reg": (w0 >> 5) & 0x3F, "dst_reg": (w0 >> 12) & 0x3F}


def _mask(m):
    return "".join(_WM[i] for i in range(4) if m & (1 << i)) or "____"


def disassemble(data, little=False):
    dwords = _dwords(data, little)
    cf_list = []
    min_addr = None
    slot = 0
    while (slot + 1) * 3 <= len(dwords):
        if min_addr is not None and slot >= min_addr:
            break
        w0, w1, w2 = dwords[slot * 3:slot * 3 + 3]
        for cf in _cf_pair(w0, w1, w2):
            d = decode_cf(cf)
            d["slot"] = slot
            cf_list.append(d)
            if d.get("count") and "address" in d:
                min_addr = d["address"] if min_addr is None else min(min_addr, d["address"])
        slot += 1

    lines = []
    for d in cf_list:
        if d["mn"] == "nop" and "address" not in d:
            continue
        if d["op"] in EXEC_OPS:
            lines.append("%-22s addr=%d count=%d" % (d["mn"], d["address"], d["count"]))
            seq = d["sequence"]
            for i in range(d["count"]):
                s = seq >> (2 * i)
                idx = d["address"] + i
                base = idx * 3
                if base + 3 > len(dwords):
                    break
                w0, w1, w2 = dwords[base:base + 3]
                if s & 0b01:                              # fetch
                    f = decode_fetch(w0, w1, w2)
                    lines.append("    %-14s r%d, r%d" % (f["mn"], f["dst_reg"], f["src_reg"]))
                else:                                     # ALU
                    a = decode_alu(w0, w1, w2)
                    tag = "export" if a["export"] else "r%d/r%d" % (a["vector_dest"], a["scalar_dest"])
                    lines.append("    %-8s + %-10s %s  v=%s s=%s" % (
                        a["vector"], a["scalar"], tag, _mask(a["vector_mask"]), _mask(a["scalar_mask"])))
        else:
            extra = (" addr=%d" % d["address"]) if "address" in d else ""
            lines.append("%s%s" % (d["mn"], extra))
    return lines, cf_list


DESCRIPTION = "disassemble Xbox 360 Xenos GPU shader microcode (control flow + ALU/fetch)"

# fxc (D3DX9 Shader Compiler, Xbox 360) wraps microcode in a container whose
# first three bytes are 10 2A 11; the 4th byte is the shader type (0=pixel,
# 1=vertex) and the big-endian dword at +0x04 is the offset to the raw microcode.
_FXC_MAGIC = b"\x10\x2a\x11"
_FXC_TYPE = {0: "pixel", 1: "vertex"}


def unwrap_container(data):
    """If `data` is an fxc-compiled shader container, return (microcode_bytes,
    shader_type_str); otherwise (data, None). The microcode is disassembled by
    `disassemble()` exactly as a raw blob would be."""
    if len(data) >= 8 and data[:3] == _FXC_MAGIC:
        off = struct.unpack_from(">I", data, 4)[0]
        if 8 <= off < len(data):
            return data[off:], _FXC_TYPE.get(data[3], "type%d" % data[3])
    return data, None


def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: shader <shader.bin | fxc.bin> [--le] [--raw]\n"
              "       fxc-compiled containers (10 2A 11 ..) are auto-detected;\n"
              "       --raw forces treating the whole file as raw microcode")
        return
    little = "--le" in a
    force_raw = "--raw" in a
    path = [x for x in a if not x.startswith("--")][0]
    data = open(path, "rb").read()
    if not force_raw:
        data, kind = unwrap_container(data)
        if kind:
            print("[fxc container: %s shader, microcode @0x%X]" % (
                kind, struct.unpack_from(">I", open(path, "rb").read(), 4)[0]))
    lines, cf = disassemble(data, little)
    for ln in lines:
        print(ln)
    print("--- %d control-flow instructions ---" % len([c for c in cf if c["mn"] != "nop" or "address" in c]))


if __name__ == "__main__":
    main()
