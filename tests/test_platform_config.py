# -*- coding: utf-8 -*-
"""Tests for SDK gatekeeping (j2ctool.sdk), platform choices (j2ctool.platform_config), debug (j2ctool.debug)."""

import os
import pytest

from j2ctool import sdk, platform_config, debug


def _fake_xdk(tmp_path, with_symbols=True):
    """Build a minimal fake XDK: <root>/include/xbox/xtl.h declaring the kernel symbols."""
    root = tmp_path / 'FakeXDK'
    inc = root / 'include' / 'xbox'
    inc.mkdir(parents=True)
    decls = ('DWORD XShowKeyboardUI();\nDWORD XShowDeviceSelectorUI();\n'
             'DWORD XContentCreate();\nDWORD XContentClose();\n') if with_symbols else '// empty\n'
    (inc / 'xtl.h').write_text(decls, encoding='utf-8')
    return str(root)


# ---- SDK detection / gate --------------------------------------------------
def test_ungated_platform_needs_no_sdk():
    assert not sdk.requires_sdk('pc_windows')
    assert not sdk.requires_sdk(None)
    info = sdk.detect('pc_windows')
    assert info is not None and info.source == 'ungated'


def test_detect_via_override(tmp_path):
    root = _fake_xdk(tmp_path)
    info = sdk.detect('xbox360', override=root)
    assert info is not None
    assert info.source == 'override'
    assert info.has_symbol('XShowKeyboardUI')
    assert not info.has_symbol('XNoSuchSymbol')


def test_require_raises_when_absent(tmp_path):
    with pytest.raises(sdk.SdkNotFound):
        sdk.require('xbox360', override=str(tmp_path / 'does-not-exist'))


def test_detect_bad_root_returns_none(tmp_path):
    empty = tmp_path / 'empty'
    empty.mkdir()
    assert sdk.detect('xbox360', override=str(empty)) is None    # no xtl.h probe


# ---- controller mapping ----------------------------------------------------
def test_default_config_binds_all_actions():
    cfg = platform_config.default_config()
    for key, _label, _default in platform_config.CONTROLLER_ACTIONS:
        assert key in cfg['controller']['binds']


def test_southpaw_layout_swaps_sticks():
    cfg = platform_config.default_config()
    cfg['controller']['layout'] = 'southpaw'
    binds = platform_config._apply_layout(cfg)
    assert binds['move'] == 'RSTICK'
    assert binds['look'] == 'LSTICK'


def test_legacy_layout_moves_attack_to_bumpers():
    cfg = platform_config.default_config()
    cfg['controller']['layout'] = 'legacy'
    binds = platform_config._apply_layout(cfg)
    assert binds['attack'] == 'RBUMPER'
    assert binds['use'] == 'LBUMPER'


# ---- emission + SDK gate together ------------------------------------------
def test_gated_medium_refused_without_sdk(tmp_path):
    out = str(tmp_path / 'out'); os.makedirs(out)
    with pytest.raises(sdk.SdkNotFound):
        platform_config.run(out, updates={'storage': 'stfs'}, platform='xbox360',
                            sdk_override=str(tmp_path / 'no-sdk'))


def test_gated_medium_allowed_with_sdk(tmp_path):
    out = str(tmp_path / 'out'); os.makedirs(out)
    root = _fake_xdk(tmp_path)
    cfg, written, info, unverified = platform_config.run(
        out, updates={'storage': 'stfs', 'keyboard': 'xshowkeyboard'},
        platform='xbox360', sdk_override=root)
    assert info is not None and info.source == 'override'
    assert unverified == []                               # all symbols present in the fake SDK
    names = set(os.path.basename(p) for p in written)
    assert {'ControllerConfig.h', 'KernelConfig.h', 'Kernel.cpp', 'PLATFORM_CHOICES.md'} <= names
    kernel = open(os.path.join(out, 'Kernel.cpp'), encoding='utf-8').read()
    assert 'XShowKeyboardUI' in kernel and 'XContentCreate' in kernel


def test_ungated_mediums_emit_without_sdk(tmp_path):
    out = str(tmp_path / 'out'); os.makedirs(out)
    # osk + memory need no XDK symbols -> no SDK required even for xbox360
    cfg, written, info, unverified = platform_config.run(
        out, updates={'keyboard': 'osk', 'storage': 'memory'}, platform='xbox360')
    assert info is None
    assert os.path.isfile(os.path.join(out, 'Kernel.cpp'))
    kernel = open(os.path.join(out, 'Kernel.cpp'), encoding='utf-8').read()
    assert 'J2X-STUB[kernel-storage]' in kernel           # memory-only marked


def test_unverified_symbol_flagged(tmp_path):
    out = str(tmp_path / 'out'); os.makedirs(out)
    root = _fake_xdk(tmp_path, with_symbols=False)         # xtl.h present but no symbol decls
    cfg, written, info, unverified = platform_config.run(
        out, updates={'keyboard': 'xshowkeyboard'}, platform='xbox360', sdk_override=root)
    assert 'XShowKeyboardUI' in unverified


# ---- debug system ----------------------------------------------------------
def test_debug_emits_three_files(tmp_path):
    out = str(tmp_path)
    paths = debug.write_debug(out)
    assert {os.path.basename(p) for p in paths} == {'Debug.h', 'Debug.cpp', 'DEBUG.md'}
    h = open(os.path.join(out, 'Debug.h'), encoding='utf-8').read()
    assert '#define DEBUG_ENABLED 1' in h
    # master-off path defines every macro to a no-op
    assert '#define DBG_ONCE(cat, catflag, ...)         ((void)0)' in h
    assert 'DBG_SUMMARY' in h
