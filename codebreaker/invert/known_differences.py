r"""known_differences.py -- the divergence micro-pattern knowledge base (roadmap 2c).

Every time a candidate MISS is diagnosed and fixed, the (divergence signature -> cause
-> fix) mapping is recorded here so it is never re-derived by hand on another DLL. The
refine loop consults `diagnose()` on a miss: it matches the localized divergence against
known patterns and reports the likely cause + the fix that resolved it before, turning
"stare at the bytes" into "apply the known rule."

Entries are seeded from the fixes proven this project. `msc6dec_construct` names the
recognizer that now handles it (so the KB doubles as a map from divergence to engine
code); `example` is a real function it was proven on.
"""
import re

# Each pattern: a regex over a normalized divergence string (from refine.localize /
# the miss histogram), the human cause, the fix (engine construct or C transform), and
# whether msc6dec now handles it automatically or it's still a manual refine transform.
KB = [
    {"id": "seg-indexed-in-cond",
     "match": r"es:\[|ds:\[",                       # a raw <seg>:[..] leaked into C
     "cause": "segment-indexed memory used in a condition/expression, not resolved to a field",
     "fix": "_val() routes <seg>:[base+N] through _farmem() -> *(WORD far*)((char far*)p+N)",
     "msc6dec_construct": "_farmem in _val", "auto": True, "example": "OLECLI seg3:0x2a00"},
    {"id": "far-param-as-int",
     "match": r"ship\[les .*\] vs ours\[mov ",       # shipped les (far), ours mov (int)
     "cause": "a far-pointer param inferred as int (2-byte) -> wrong frame + param layout",
     "fix": "les [bp+off] marks off a far param (4 bytes); bound the count by retf N",
     "msc6dec_construct": "far-param inference (les + retf N)", "auto": True, "example": "OLECLI seg11:0x31c"},
    {"id": "trailing-pad",
     "match": r"len\+1$|trailing (90|00)",           # one extra alignment byte
     "cause": "compiler alignment nop after a function; not part of the semantic body",
     "fix": "strip trailing 0x90/0x00 from both sides before comparing (verify + _mask)",
     "msc6dec_construct": "verify_inverted padding-strip", "auto": True, "example": "LZEXPAND Cleanup"},
    {"id": "cdecl-arg-order",
     "match": r"push\[bp \+ N\].*vs.*push\[bp \+ N\]|arg.?order",
     "cause": "cdecl call pushes args right-to-left; args must be REVERSED from push order",
     "fix": "detect 9a+add sp -> cdecl -> reverse s.args when building the call expr",
     "msc6dec_construct": "cdecl arg reversal", "auto": True, "example": "OLECLI/seg10:0x23a"},
    {"id": "byte-store-mask",
     "match": r"c606|byte store.*offset",
     "cause": "a byte-global store (c6 06) DGROUP offset not masked -> never matches relink",
     "fix": "add 0x68/0xc6 to _mask's direct-[imm16] opcodes",
     "msc6dec_construct": "_mask opcode set", "auto": True, "example": "LZEXPAND FreeBuffers"},
    {"id": "recipe-Os",
     "match": r"len[-+]\d+.*frame|sub sp.*vs|83ec02",
     "cause": "function compiled with a different optimization flag (e.g. /Os omits the dummy frame)",
     "fix": "verify against MULTIPLE recipes (/Ox and /Os); the flag prober pins the exact one",
     "msc6dec_construct": "multi-recipe verify / detect_recipe", "auto": True, "example": "LZEXPAND return-0 (/Os)"},
    {"id": "addr-of-global-imm",
     "match": r"push (imm|N).*global|68 N",
     "cause": "a push imm16 that is a DGROUP offset is &global (near ptr), not a literal",
     "fix": "push imm16 whose value is also touched as [imm16] -> &gW_x; mask the 0x68 operand",
     "msc6dec_construct": "address-of-global push", "auto": True, "example": "OLECLI/seg10:0x23a"},
]


def diagnose(divergence_text):
    """Match a localized divergence against the KB; return the matching pattern(s)."""
    hits = []
    t = divergence_text or ""
    for p in KB:
        if re.search(p["match"], t, re.I):
            hits.append(p)
    return hits


def record(pattern):
    """Append a newly-solved divergence pattern (id, match, cause, fix, ...)."""
    KB.append(pattern)
    return pattern


if __name__ == "__main__":
    print("divergence KB: %d known patterns\n" % len(KB))
    for p in KB:
        print("  [%s] %s" % ("auto" if p["auto"] else "manual", p["id"]))
        print("      cause: %s" % p["cause"])
        print("      fix:   %s  (%s)" % (p["fix"], p["msc6dec_construct"]))
