# -*- mode: python ; coding: utf-8 -*-
# PyInstaller spec for the j2ctool converter.
#
#   Build:  pyinstaller j2ctool.spec
#   Output: dist/j2ctool.exe  (single-file, windowed GUI; the CLI works too)
#
# Data bundled below is unpacked at runtime under sys._MEIPASS and located via j2ctool._paths.data_path.
# Generated output is written next to the .exe (j2ctool._paths.output_root), never into _MEIPASS.

import os
from PyInstaller.utils.hooks import collect_submodules

HERE = os.path.abspath('.')

# Collect EVERY j2ctool submodule -- most tool modules are imported lazily (per CLI subcommand / GUI
# step) and would otherwise be missed by PyInstaller's static import analysis.
hidden = collect_submodules('j2ctool')

a = Analysis(
    ['j2ctool_entry.py'],
    pathex=[HERE],
    binaries=[],
    datas=[
        # Read-only data the converter reads at runtime (the emitted Java-stdlib headers, the
        # hand-written platform reference layer, and its companions), plus conversion profiles.
        ('docs/platform/canonical',  'docs/platform/canonical'),
        ('docs/platform/reference',  'docs/platform/reference'),
        ('docs/platform/companions', 'docs/platform/companions'),
        ('profiles',                 'profiles'),
        # The bundled RetroMCP/Vineflower decompiler backend (Minecraft version -> Java source).
        ('backends',                 'backends'),
    ],
    hiddenimports=hidden,
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    noarchive=False,
    optimize=0,
)
pyz = PYZ(a.pure)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name='j2ctool',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
)
