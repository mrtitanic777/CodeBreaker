# -*- mode: python ; coding: utf-8 -*-
# PyInstaller spec for the j2x converter.
#
# Build:  pyinstaller j2x.spec
# Output: dist/j2x.exe (single-file, console+GUI)

import os

HERE = os.path.abspath('.')

a = Analysis(
    ['j2x_entry.py'],
    pathex=[HERE],
    binaries=[],
    datas=[
        ('overrides', 'overrides'),
        ('profiles', 'profiles'),
    ],
    hiddenimports=[
        'j2x.gui',
        'j2x.pipeline',
        'j2x.signatures',
        'j2x.worklist',
        'j2x.vcxproj',
    ],
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
    name='j2x',
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
