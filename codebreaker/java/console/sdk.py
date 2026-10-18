# -*- coding: utf-8 -*-
r"""
sdk.py -- platform SDK detection & gatekeeping.

A console platform's SDK (Xbox 360 XDK, PS3 SDK, Wii RVL SDK) is proprietary. The converter must NOT
ship or redistribute those headers/signatures, and must NOT emit platform-specific code for a target
the user has no SDK for. So this module:

  * DETECTS the SDK on the user's own machine (env var first, then the standard install paths), or
    accepts an explicit `--sdk <path>` override.
  * GATEKEEPS: converting/emitting for a console platform is refused unless its SDK is located. PC is
    ungated; `--platform none` (portable only) is always allowed.
  * VERIFIES symbols against the located SDK headers, so the code we emit calls only APIs that actually
    exist in THAT installed SDK -- we pull the truth from the user's SDK instead of trusting a baked-in
    signature list. (The real declarations come from the user's own `<xtl.h>` at compile time; the tool
    ships none of them.)

`require(platform)` is the one call the pipeline/config use to gate. It returns an SdkInfo or raises
SdkNotFound with actionable instructions.
"""

import os
import re


class SdkNotFound(Exception):
    pass


# Per-platform: the environment variable(s) that point at the SDK root, the standard install paths to
# probe, the include subdir, and header files that must exist for the root to count as a real SDK.
PLATFORM_SDKS = {
    'xbox360': {
        'name': 'Xbox 360 SDK (XDK)',
        'env': ['XEDK'],
        'default_paths': [
            r'C:\Program Files (x86)\Microsoft Xbox 360 SDK',
            r'C:\Program Files\Microsoft Xbox 360 SDK',
            r'C:\XDK',
        ],
        'include_subdirs': [os.path.join('include', 'xbox'), 'include'],
        'probe_headers': ['xtl.h'],
        'get_env': 'XEDK',
        'setting': 'xbox360_sdk',
    },
    'ps3': {
        'name': 'PlayStation 3 SDK (Cell)',
        'env': ['SCE_PS3_ROOT', 'CELL_SDK'],
        'default_paths': [r'C:\usr\local\cell', r'C:\ProgramData\SCE\PS3'],
        'include_subdirs': [os.path.join('target', 'ppu', 'include'), 'include'],
        'probe_headers': ['cell', 'sys'],           # directories/headers present in the Cell SDK
        'get_env': 'CELL_SDK',
    },
    'wii': {
        'name': 'Wii SDK (Revolution)',
        'env': ['REVOLUTION_SDK_ROOT', 'RVL_ROOT'],
        'default_paths': [r'C:\projects\RVL', r'C:\RVL_SDK'],
        'include_subdirs': ['include'],
        'probe_headers': ['revolution.h'],
        'get_env': 'REVOLUTION_SDK_ROOT',
    },
}

# Platforms that need NO SDK gate (portable / ubiquitous toolchain).
UNGATED = {None, 'none', 'pc_windows', 'pc', 'windows'}


class SdkInfo(object):
    def __init__(self, platform, name, root, include_dirs, source):
        self.platform = platform
        self.name = name
        self.root = root
        self.include_dirs = include_dirs      # list of existing include dirs to search for symbols
        self.source = source                  # 'env:XEDK' / 'path' / 'override' -- how we found it
        self._symbol_cache = None

    def __repr__(self):
        return 'SdkInfo(%s @ %s via %s)' % (self.platform, self.root, self.source)

    def _all_header_text(self):
        if self._symbol_cache is not None:
            return self._symbol_cache
        blob = []
        for d in self.include_dirs:
            if not os.path.isdir(d):
                continue
            for dp, _dirs, files in os.walk(d):
                for f in files:
                    if f.lower().endswith(('.h', '.hpp', '.hxx')):
                        try:
                            blob.append(open(os.path.join(dp, f), 'r', encoding='utf-8',
                                             errors='replace').read())
                        except (IOError, OSError):
                            pass
        self._symbol_cache = '\n'.join(blob)
        return self._symbol_cache

    def has_symbol(self, symbol):
        """True if `symbol` is declared anywhere in the located SDK's headers. Lets the emitter verify
        an API exists in THIS install before emitting a call to it (pull-from-SDK, not baked-in)."""
        return re.search(r'\b%s\b' % re.escape(symbol), self._all_header_text()) is not None


def requires_sdk(platform):
    return platform not in UNGATED and platform in PLATFORM_SDKS


def _valid_root(spec, root):
    """Does `root` look like a real SDK (has an include dir with a probe header)?  Returns the list of
    existing include dirs if valid, else None."""
    if not root or not os.path.isdir(root):
        return None
    inc_dirs = [os.path.join(root, s) for s in spec['include_subdirs']]
    inc_dirs = [d for d in inc_dirs if os.path.isdir(d)]
    if not inc_dirs:
        # some SDKs point env straight at the include dir
        if os.path.isdir(root):
            inc_dirs = [root]
    for probe in spec['probe_headers']:
        for d in inc_dirs + [root]:
            if os.path.exists(os.path.join(d, probe)):
                return inc_dirs or [root]
    return None


def detect(platform, override=None):
    """Locate the SDK for `platform`. Order: explicit override -> the platform's settings.json key
    (xbox360_sdk) -> env var(s) -> default install paths. Returns SdkInfo or None. Ungated platforms
    return a trivial SdkInfo."""
    if platform in UNGATED or platform not in PLATFORM_SDKS:
        return SdkInfo(platform, 'no SDK required', '', [], 'ungated')
    spec = PLATFORM_SDKS[platform]
    # 1. explicit override
    if override:
        inc = _valid_root(spec, override)
        if inc:
            return SdkInfo(platform, spec['name'], override, inc, 'override')
        return None
    # 1b. settings.json
    if spec.get('setting'):
        from codebreaker.base import settings
        root = settings.get(spec['setting'])
        inc = _valid_root(spec, root) if root else None
        if inc:
            return SdkInfo(platform, spec['name'], root, inc, 'setting:%s' % spec['setting'])
    # 2. environment variables
    for ev in spec['env']:
        root = os.environ.get(ev)
        inc = _valid_root(spec, root) if root else None
        if inc:
            return SdkInfo(platform, spec['name'], root, inc, 'env:%s' % ev)
    # 3. standard install paths
    for root in spec['default_paths']:
        inc = _valid_root(spec, root)
        if inc:
            return SdkInfo(platform, spec['name'], root, inc, 'path')
    return None


def require(platform, override=None):
    """Return the SdkInfo for `platform`, or raise SdkNotFound with actionable guidance. This is the
    gate: no console SDK -> no platform-specific conversion/emission."""
    info = detect(platform, override=override)
    if info is not None:
        return info
    spec = PLATFORM_SDKS[platform]
    envs = ' or '.join('%%%s%%' % e for e in spec['env'])
    raise SdkNotFound(
        "No %s found -- cannot convert/emit for platform '%s'.\n"
        "  The converter never ships SDK headers or signatures; it pulls them from your installed SDK.\n"
        "  Provide it one of these ways:\n"
        "    * set the environment variable %s to the SDK root, or\n"
        "    * pass --sdk <path-to-sdk-root>, or\n"
        "    * install it to one of: %s\n"
        "  (Ungated targets still work: use --platform none for a portable conversion with no %s code.)"
        % (spec['name'], platform, envs, ', '.join(spec['default_paths']), spec['name']))


def describe(platform, override=None):
    """Human-readable one-liner about SDK status for `platform` (for CLI `j2ctool sdk detect`)."""
    if platform in UNGATED:
        return "platform '%s': no SDK required (ungated)." % platform
    if platform not in PLATFORM_SDKS:
        return "platform '%s': unknown." % platform
    info = detect(platform, override=override)
    if info is None:
        spec = PLATFORM_SDKS[platform]
        return ("platform '%s': %s NOT found (checked %s + %s). Set %s or pass --sdk."
                % (platform, spec['name'], ', '.join('%%%s%%' % e for e in spec['env']),
                   ', '.join(spec['default_paths']),
                   ' / '.join(spec['env'])))
    return "platform '%s': %s found at %s (via %s)." % (platform, info.name, info.root, info.source)
