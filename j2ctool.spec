# -*- mode: python ; coding: utf-8 -*-
# PyInstaller spec for the j2ctool converter.
#
# Build:  pyinstaller j2ctool.spec
# Output: dist/j2ctool.exe (single-file, console+GUI)

import os

HERE = os.path.abspath('.')

a = Analysis(
    ['j2ctool_entry.py'],
    pathex=[HERE],
    binaries=[],
    datas=[
        ('profiles', 'profiles'),
    ],
    hiddenimports=[
        'j2ctool.gui',
        'j2ctool.pipeline',
        'j2ctool.signatures',
        'j2ctool.worklist',
        'j2ctool.vcxproj',
        'j2ctool.platforms',
        'j2ctool.stubs',
        'j2ctool.emit.std_transforms',
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
