# -*- coding: utf-8 -*-
r"""
xdk_scan.py -- AUTO-DETECT platform capabilities & requirements from the INSTALLED Xbox 360 SDK.

The converter knows the target platform, and the user supplies the SDK -- so what the platform offers
(and mandates) must be READ FROM THE SDK'S OWN HEADERS, never hand-typed from memory. This walks the SDK
include tree, enumerates the exported function families, and maps the ones that are PRESENT to the
subsystems the port needs.

The honest split:
  * DETECTION  (is this API family in THIS SDK? which exact functions?) -> 100% from the scanned headers.
  * INTERPRETATION (which family serves which subsystem, and why it matters) -> the converter's platform
    knowledge, encoded as PATTERNS below -- but every pattern is GATED on its family being detected, and
    the functions it reports are the ones actually found. Against an SDK that lacks XContent*, the
    "saves need an STFS package" requirement is simply NOT emitted.

Consumed by platform_coverage.py to render SDK-derived requirements/opportunities/equivalents.
"""
import os
import re

# Exported-looking XDK entry points: XFoo(, XamFoo(, D3DXFoo(, MmFoo(, KeFoo(, NtFoo(, VdFoo(.
_FUNC_RE = re.compile(r'\b((?:Xam[A-Z]|X[A-Z]|D3DX|Mm[A-Z]|Ke[A-Z]|Nt[A-Z]|Vd[A-Z])\w+)\s*\(')


def scan_functions(include_dir):
    """{function_name: header_basename} for every exported-looking function in the SDK headers."""
    funcs = {}
    if not include_dir or not os.path.isdir(include_dir):
        return funcs
    for root, _dirs, files in os.walk(include_dir):
        for fn in files:
            if not fn.endswith('.h'):
                continue
            try:
                txt = open(os.path.join(root, fn), encoding='utf-8', errors='replace').read()
            except Exception:
                continue
            for m in _FUNC_RE.finditer(txt):
                funcs.setdefault(m.group(1), fn)
    return funcs


def sdk_include_dir(sdk_root):
    """The 360 header dir inside an SDK root (include/xbox), else the root itself if headers live there."""
    if not sdk_root:
        return None
    for cand in (os.path.join(sdk_root, 'include', 'xbox'), os.path.join(sdk_root, 'include'), sdk_root):
        if os.path.isdir(cand):
            return cand
    return None


# PATTERNS: platform knowledge, each GATED on an SDK family being present. `needs` = the function (or
# family prefix) that MUST exist in the scanned SDK for this to be claimed; `kind` = requirement (the
# platform mandates it) / equivalent (the API to resolve a stub to) / opportunity (a native win). The
# reported functions come from the scan; only `why`/`title` are interpretation.
PATTERNS = [
  {'needs': 'XContentCreate', 'family': 'XContent', 'kind': 'requirement', 'sub': 'save', 'sev': 'correctness',
   'title': 'World saves must go through an STFS content package (not a raw file write)',
   'why': 'This SDK exports the XContent* content-package family -- the durable per-profile storage mechanism on '
          'the 360. CreateFile/WriteFile to a plain path is NOT committed to the device until the package is '
          'flushed (XContentFlush) after a save batch.'},
  {'needs': 'XShowDeviceSelectorUI', 'family': 'XShowDeviceSelectorUI', 'kind': 'requirement', 'sub': 'save', 'sev': 'cert',
   'title': 'Storage device must be selected before saving',
   'why': 'The SDK provides the device-selector UI; the chosen XCONTENTDEVICEID scopes XContentCreate. Cert '
          'requires picking a device and handling no-device / not-enough-space.'},
  {'needs': 'XNotifyGetNext', 'family': 'XNotify', 'kind': 'requirement', 'sub': 'system', 'sev': 'cert',
   'title': 'Pause/yield when the Guide overlay opens; handle sign-in & controller changes',
   'why': 'The SDK exposes the notification family (XNotifyGetNext). A title that keeps simulating/playing audio '
          'under the Guide overlay is a cert failure; also handle XN_SYS_SIGNINCHANGED and controller unplug.'},
  {'needs': 'XNetStartup', 'family': 'XNet', 'kind': 'requirement', 'sub': 'network', 'sev': 'compat',
   'title': 'Talking to a PC/Java server needs XNet security bypassed',
   'why': 'XNetStartup(XNET_STARTUP_BYPASS_SECURITY) + insecure setsockopt -- otherwise XNet encrypts all traffic '
          'and refuses non-Xbox peers, so a standard Minecraft (PC) server sees garbage.'},
  {'needs': 'XPhysicalAlloc', 'family': 'XPhysical', 'kind': 'requirement', 'sub': 'video', 'sev': 'correctness',
   'title': 'GPU resources should come from physical memory',
   'why': 'The SDK exposes XPhysicalAlloc/XPhysicalFree. The GPU reads physical (not paged) memory; allocating '
          'textures/vertex buffers from the general heap is the wrong path and feeds heap fragmentation.'},

  {'needs': 'XShowKeyboardUI', 'family': 'XShowKeyboardUI', 'kind': 'equivalent', 'sub': 'input',
   'title': 'On-screen keyboard for text input',
   'why': 'XShowKeyboardUI (async) -- the OSK for server address / chat, since there is no PC keyboard.'},

  {'needs': 'XInputSetState', 'family': 'XInputSetState', 'kind': 'opportunity', 'sub': 'input', 'effort': 'S',
   'title': 'Controller rumble', 'why': 'XInputSetState(XINPUT_VIBRATION) -- haptics on damage/explosions/mining.'},
  {'needs': 'XMPGetStatus', 'family': 'XMP', 'kind': 'opportunity', 'sub': 'audio', 'effort': 'M',
   'title': 'Custom soundtracks', 'why': "The XMP family lets the player's own dashboard music play over the game."},
  {'needs': 'XMemCompress', 'family': 'XMemCompress', 'kind': 'opportunity', 'sub': 'save', 'effort': 'M',
   'title': 'LZX save compression', 'why': 'XMemCompress/XMemDecompress (LZX) for our own saves -- smaller/faster '
          'than zlib. NOTE: MP server chunks stay zlib/puff; LZX is a different format.'},
  {'needs': 'XShowMessageBoxUI', 'family': 'XShowMessageBoxUI', 'kind': 'opportunity', 'sub': 'system', 'effort': 'S',
   'title': 'Native system dialogs', 'why': 'XShowMessageBoxUI -- connect-failed/kicked prompts as the real 360 box.'},
  {'needs': 'XGetLanguage', 'family': 'XGetLanguage', 'kind': 'opportunity', 'sub': 'i18n', 'effort': 'S',
   'title': 'Console-language i18n', 'why': 'XGetLanguage -> load the matching .lang instead of hardcoded en_US.'},
  {'needs': 'XGetVideoMode', 'family': 'XGetVideoMode', 'kind': 'opportunity', 'sub': 'video', 'effort': 'S',
   'title': 'Native display resolution', 'why': 'XGetVideoMode -> size the render target to 480p/720p/1080p + widescreen.'},
  {'needs': 'XJSONCreateReader', 'family': 'XJSON', 'kind': 'opportunity', 'sub': 'io', 'effort': 'S',
   'title': 'Native JSON parser', 'why': 'The XJSON family replaces the argo-JSON stub cluster (StringReader/J_*).'},
]


def detect(sdk_root):
    """Scan the installed SDK and return the capabilities/requirements that are ACTUALLY present in it.
    Returns {'include': dir, 'funcs': {name:header}, 'found': [pattern+found-functions...], 'missing': [...]}."""
    inc = sdk_include_dir(sdk_root)
    funcs = scan_functions(inc)
    found, missing = [], []
    for p in PATTERNS:
        present = p['needs'] in funcs
        fam = sorted(f for f in funcs if f.startswith(p['family']))
        rec = dict(p)
        rec['present'] = present
        rec['functions'] = fam[:12]                    # the actual functions detected (capped for display)
        rec['func_count'] = len(fam)
        rec['header'] = funcs.get(p['needs'], '')
        (found if present else missing).append(rec)
    return {'include': inc, 'funcs': funcs, 'found': found, 'missing': missing}
