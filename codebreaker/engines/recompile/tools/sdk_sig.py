#!/usr/bin/env python3
r"""sdk_sig.py -- Tier-1 type inference: stamp the EXACT Microsoft signature on a deregisterized
export. For a function whose export name is a known Win32 API (reports/sdk_api.json), we don't
infer -- we apply the real return type, calling convention, and typed+named parameters straight
from the SDK. `int fn(u32 arg1, u32 arg2, ...)` becomes e.g.
    BOOL GetVolumeInformationA(LPCSTR lpRootPathName, LPSTR lpVolumeNameBuffer, DWORD n..., ...)
This is behavior-PRESERVING (types are 32-bit-ABI-identical labels; argN->name is an alpha-rename),
so an oracle-verified clean function stays verified. Returns None if the SDK arity is inconsistent
with what the lift recovered (then the generic signature is kept -- honest).
"""
import os, re, json

_MACROS = {"UNICODE", "WINBASEAPI", "WINUSERAPI", "WINGDIAPI", "WINADVAPI", "WINAPI", "WINOLEAPI",
           "WINOLEAUTAPI", "WINCOMMCTRLAPI", "APIENTRY", "CALLBACK", "STDAPI", "STDMETHODIMP",
           "EXTERN_C", "DECLSPEC_IMPORT", "WINOLEAUTAPI_", "SHSTDAPI", "LWSTDAPI", "const"}


def clean_type(t):
    """Strip API/linkage macros from an SDK type string -> the real C type (e.g.
    'UNICODE WINBASEAPI LPSTR' -> 'LPSTR')."""
    toks = [x for x in (t or "").split() if x not in _MACROS]
    return " ".join(toks) if toks else "int"


_SDK = None


def sdk_api(reports_dir):
    global _SDK
    if _SDK is None:
        p = os.path.join(reports_dir, "sdk_api.json")
        _SDK = json.load(open(p)) if os.path.exists(p) else {}
    return _SDK


def apply_api_signature(func_c, sdk, fname):
    """Rewrite `func_c`'s signature to the exact SDK signature and rename argN -> the SDK positional
    parameter name. `sdk` = {ret, cc, argc, args:[...], varargs}. Returns the rewritten C, or None
    if it can't be applied faithfully (varargs, register-arg present, or an argN beyond SDK arity)."""
    if sdk.get("varargs"):
        return None
    args = sdk.get("args") or []
    m = re.search(r"\b" + re.escape(fname) + r"\s*\(([^)]*)\)", func_c)
    if not m:
        return None
    cur = [p.strip() for p in m.group(1).split(",") if p.strip() and p.strip() != "void"]
    names = [p.split()[-1].lstrip("*") for p in cur]
    if any(not re.match(r"arg\d+$", n) for n in names):      # a register-param present -> not a plain stdcall API
        return None
    sdk_names = [a.split()[-1].lstrip("*") for a in args]
    ren = {}
    for n in names:
        i = int(n[3:]) - 1                                    # argN = Nth stack dword = SDK args[N-1]
        if i < 0 or i >= len(args):
            return None                                       # lift references an arg beyond SDK arity
        ren[n] = sdk_names[i]
    ret = clean_type(sdk.get("ret", "int"))
    new_sig = "%s %s(%s)" % (ret, fname, ", ".join(args) if args else "void")
    out = re.sub(r"^[^\n]*\b" + re.escape(fname) + r"\s*\([^)]*\)", new_sig, func_c, count=1, flags=re.M)
    for n in sorted(ren, key=lambda s: -len(s)):              # longest first: arg10 before arg1
        out = re.sub(r"\b" + n + r"\b", ren[n], out)
    return out


if __name__ == "__main__":     # demo: apply GetVolumeInformationA's signature to a generic clean fn
    import sys
    reports = os.path.join(os.environ.get("WT_DATA_ROOT", "."), "reports")
    api = sdk_api(reports)
    name = sys.argv[1] if len(sys.argv) > 1 else "GetVolumeInformationA"
    sample = ("int %s(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8)\n"
              "{\n    u32 eax;\n    eax = arg2;\n    return arg1;\n}") % name
    print("=== generic (deregisterized) ===\n" + sample)
    r = apply_api_signature(sample, api.get(name, {}), name)
    print("\n=== Tier-1 (exact SDK signature) ===\n" + (r or "(no SDK match / inconsistent)"))
