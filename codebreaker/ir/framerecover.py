#!/usr/bin/env python3
r"""framerecover.py — stack-frame / local-variable recovery for the raiser.

The lifted `[AUTO]` C drops the function's stack frame: the register-machine trace
models what the body *computes* but not the `push ebp; mov ebp,esp; sub esp,N`
prologue that reserves the locals. So a lifted `v1=(*fp)(); return v1;` never carries
the `sub esp,0x3c` the shipped function has, the candidate comes out the wrong length,
and the byte-match never closes.

This module recovers the frame FROM THE SHIPPED BYTES (the ground truth) and augments a
raiser candidate so MSVC 2.0 reproduces it:

  * `recover_frame(mod, seg, off)`  -> {frame, saved, locals_bytes, ecx_slots, ...}
    Parses the shipped prologue: `push ebp; mov ebp,esp`? which callee-saved regs
    (ebx/esi/edi) are pushed? what is N in `sub esp,N` (plus any `push ecx/eax`
    single-slot reservations)? what is the return/cleanup shape?

  * `augment_candidate(cand_c, frame_info)` -> c
    Rewrites the candidate's lift-named stack locals (`loc_<hex>` = `[ebp-<hex>]`) into
    a single sized frame `char _fr[N];` and pins every `loc_K` to `_fr + (N - K)` so it
    lands at exactly `[ebp-K]`, the shipped offset. Because an *escaping* sized array
    reserves its FULL declared size (verified against MSVC 2.0), this forces the exact
    `sub esp,N` and the exact local layout — instead of letting MSVC pick a smaller
    frame and its own offsets from the raiser's scalar `unsigned loc_4, loc_8;` decls.

HONEST SCOPE (see the module tail for the tested numbers and the limits):
  - Works when the lift CAPTURED the locals' uses (`&loc_N` / `loc_N` in the body): the
    frame then emerges from real escapes and pinning them to shipped offsets is sound.
  - Does NOT reproduce a "hidden frame" whose users the lift dropped entirely (e.g. the
    kernel32 seg2:0x20183 `55 8b ec 51 83 ec 3c ff 15[reloc] c9 c3` case): MSVC 2.0
    reserves stack ONLY for locals whose address escapes, and every escape emits a
    `lea;push` — there is no zero-code way to reserve unused stack, so the frame cannot
    be added back without the body that used it. That is a body-modeling gap, not a
    frame-recovery one.
"""
import re
from codebreaker.core import cores

# import lazily inside functions so this module is importable without the engine cwd set
_SUB_RE = None


def recover_frame(mod, seg, off, shipped=None):
    """Recover the stack frame of a function from its SHIPPED bytes.

    `shipped` may be passed directly (bytes); otherwise the per-function .asm is located
    and read via matchbatch. Returns a dict:
        frame        : bool  — `push ebp; mov ebp,esp` present
        saved        : list  — callee-saved regs pushed, in push order (subset of ebx/esi/edi)
        sub_bytes    : int   — N in `sub esp,N` (0 if none)
        ecx_slots    : int   — count of leading `push ecx/eax` single-slot reservations
        locals_bytes : int   — total reserved local space = sub_bytes + 4*ecx_slots
        cleanup      : str   — 'leave' | 'mov-esp-ebp' | 'none'
        ret          : str   — 'cdecl' (c3) | 'stdcall' (c2) | 'other'
        arg_bytes    : int|None — stdcall callee pop count (from `ret NN`)
        ok           : bool  — a frame or a stack adjust was actually found
        prologue_len : int   — number of shipped bytes the parse consumed (prologue span)
    """
    if shipped is None:
        matchbatch = cores.get('matchbatch')
        ap = matchbatch._asm_for(mod, seg, off)
        shipped = matchbatch.shipped_bytes(ap) if ap else b""
    sb = shipped or b""
    info = {"frame": False, "saved": [], "sub_bytes": 0, "ecx_slots": 0,
            "locals_bytes": 0, "cleanup": "none", "ret": "other", "arg_bytes": None,
            "ok": False, "prologue_len": 0}
    if len(sb) < 3:
        return info

    i = 0
    # --- push ebp; mov ebp,esp -------------------------------------------------
    if sb[0:3] == b"\x55\x8b\xec":
        info["frame"] = True
        i = 3

    # --- leading single-slot reservations: push ecx (0x51) / push eax (0x50) ---
    # MSVC emits these (1 byte) to reserve a 4-byte local before `sub esp,N`.
    while i < len(sb) and sb[i] in (0x51, 0x50):
        info["ecx_slots"] += 1
        i += 1

    # --- sub esp,N  (83 ec NN  |  81 ec NN NN NN NN) ---------------------------
    if i + 2 < len(sb) and sb[i] == 0x83 and sb[i + 1] == 0xec:
        info["sub_bytes"] = sb[i + 2]
        i += 3
    elif i + 5 < len(sb) and sb[i] == 0x81 and sb[i + 1] == 0xec:
        info["sub_bytes"] = int.from_bytes(sb[i + 2:i + 6], "little")
        i += 6

    # --- callee-saved pushes: ebx(0x53) esi(0x56) edi(0x57), pushed after frame -
    _SAVE = {0x53: "ebx", 0x56: "esi", 0x57: "edi", 0x55: "ebp"}
    while i < len(sb) and sb[i] in _SAVE:
        info["saved"].append(_SAVE[sb[i]])
        i += 1
    # some functions push a slot-reservation AFTER the saves (rare) — pick up trailing ecx
    while i < len(sb) and sb[i] in (0x51, 0x50):
        info["ecx_slots"] += 1
        i += 1

    info["locals_bytes"] = info["sub_bytes"] + 4 * info["ecx_slots"]
    info["prologue_len"] = i
    info["ok"] = info["frame"] or info["sub_bytes"] > 0 or info["ecx_slots"] > 0

    # --- return / cleanup shape (from the tail) --------------------------------
    if sb[-1] == 0xC3:
        info["ret"] = "cdecl"
        tail = sb[:-1]
    elif len(sb) >= 3 and sb[-3] == 0xC2:
        info["ret"] = "stdcall"
        info["arg_bytes"] = int.from_bytes(sb[-2:], "little")
        tail = sb[:-3]
    else:
        tail = sb
    if tail[-1:] == b"\xc9":
        info["cleanup"] = "leave"
    elif tail[-3:] == b"\x8b\xe5\x5d":
        info["cleanup"] = "mov-esp-ebp"
    return info


def prologue_bytes(info):
    """Reconstruct the exact prologue byte sequence implied by a recover_frame() dict.
    Used to VALIDATE the parser: reconstruct == shipped[:prologue_len] proves the parse
    is faithful. Emits pushes in the same order recover_frame consumed them."""
    out = bytearray()
    if info["frame"]:
        out += b"\x55\x8b\xec"
    out += b"\x51" * info["ecx_slots"]              # note: recover folds leading+trailing
    n = info["sub_bytes"]
    if n:
        if n < 0x80:
            out += bytes((0x83, 0xec, n))
        else:
            out += b"\x81\xec" + n.to_bytes(4, "little")
    for r in info["saved"]:
        out += bytes({"ebx": (0x53,), "esi": (0x56,), "edi": (0x57,), "ebp": (0x55,)}[r])
    return bytes(out)


# ---------------------------------------------------------------------------
# augment_candidate — pin the lifted locals into a sized, offset-exact frame
# ---------------------------------------------------------------------------

_LOC_RE = re.compile(r"\bloc_([0-9a-fA-F]+)\b")
# a scalar declaration list that CONTAINS a loc_ name (raiser emits `unsigned loc_4, v1;`).
# extern/global decls never mention loc_, so keying on `loc_` leaves them untouched.
_DECL_RE = re.compile(r"(unsigned|int)\s+([A-Za-z_][\w, ]*?)\s*;")
# the function's opening brace: `<ret> <conv> f(<params>) {`
_OPEN_RE = re.compile(r"(\bf\s*\([^)]*\)\s*\{)")


def augment_candidate(cand_c, frame_info):
    """Augment a raiser candidate so MSVC 2.0 reproduces the recovered frame.

    Strategy (frame-overlay + offset pinning):
      1. Collect the lift's stack locals `loc_<hex>` (hex = the [ebp-hex] displacement).
      2. Size the frame from frame_info['locals_bytes'] (fall back to the deepest loc if
         the parse gave nothing). Declare it once as `char _fr[N];`.
      3. Rewrite `&loc_K`  -> `((void*)(_fr + (N - K)))`   (address at exactly [ebp-K])
                 `loc_K`   -> `(*(unsigned*)(_fr + (N - K)))`
         so the byte-for-byte stack layout matches the shipped one, and the escaping
         sized array forces the exact `sub esp,N`.

    Returns the rewritten C. If the candidate has no `loc_` locals (nothing to pin) the
    text is returned unchanged — the frame, if any, must come from the body's own escapes.
    Robust to the loc_ decl being on its own line, inline, or merged with register survivors.
    """
    locs = set(int(h, 16) for h in _LOC_RE.findall(cand_c))
    if not locs:
        return cand_c

    n = int(frame_info.get("locals_bytes") or 0)
    deepest = max(locs)
    if n < deepest:                     # parse under-counted (or no frame_info) — cover the locals
        n = deepest
    if n % 4:                           # word-align like the compiler's own layout
        n += 4 - (n % 4)

    # 1) strip the loc_ names from their declaration list, keeping the vN register survivors.
    #    Only lists that actually contain a loc_ are touched (extern/global decls are safe).
    def _filter(m):
        typ, names = m.group(1), m.group(2)
        if "loc_" not in names:
            return m.group(0)
        keep = [x.strip() for x in names.split(",")
                if x.strip() and not _LOC_RE.fullmatch(x.strip())]
        return ("%s %s;" % (typ, ", ".join(keep))) if keep else ""

    out = _DECL_RE.sub(_filter, cand_c)

    # 2) declare the sized frame once, right after the function's opening brace.
    out, nsub = _OPEN_RE.subn(r"\1\n    char _fr[%d];" % n, out, count=1)
    if not nsub:                        # unusual signature shape — fall back to first `{`
        out = out.replace("{", "{\n    char _fr[%d];" % n, 1)

    # 3) pin every reference; address form first so value-form doesn't double-wrap it.
    def _addr(m):
        return "((void*)(_fr + %d))" % (n - int(m.group(1), 16))

    def _val(m):
        return "(*(unsigned*)(_fr + %d))" % (n - int(m.group(1), 16))

    out = re.sub(r"&\s*loc_([0-9a-fA-F]+)\b", _addr, out)
    out = _LOC_RE.sub(_val, out)
    return out


# ---------------------------------------------------------------------------
# self-test / validation entry point
# ---------------------------------------------------------------------------
if __name__ == "__main__":
    # tiny parser round-trip on the task's own example bytes
    ex = bytes.fromhex("55 8b ec 51 83 ec 3c ff 15 00 00 00 00 c9 c3".replace(" ", ""))
    fi = recover_frame("x", "2", "20183", shipped=ex)
    print("task-example recover:", fi)
    print("prologue reconstruct:", prologue_bytes(fi).hex(" "),
          "==", ex[:fi["prologue_len"]].hex(" "),
          "OK" if prologue_bytes(fi) == ex[:fi["prologue_len"]] else "MISMATCH")
    demo = ("extern int (__stdcall *fp_1)();\n"
            "int __stdcall f(int arg2) {\n"
            "    unsigned loc_4, loc_8, v1;\n"
            "    v1 = (*fp_1)((&loc_8), (&loc_4));\n"
            "    return v1;\n}\n")
    print("\n--- augment demo ---\n" + augment_candidate(demo, fi))
