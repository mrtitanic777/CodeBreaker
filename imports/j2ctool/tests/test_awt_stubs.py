# -*- coding: utf-8 -*-
"""Regression: the AWT types Indev's MouseHelper / level-save dialogs use (java.awt Point, MouseInfo,
PointerInfo, Dialog) must get no-op EXTERNAL-LIBRARY stubs -- the same way Beta's dead AWT (Graphics,
Color, Robot, Component) is handled -- so the reachable game code COMPILES on the console. Beta never
hit these because its MouseHelper uses LWJGL Mouse, not raw java.awt."""

from j2ctool import runtime, typemap


def test_awt_mouse_stubs_are_generated():
    files = runtime.generate_files(platform_id='xbox360')
    for name in ('Point.h', 'MouseInfo.h', 'PointerInfo.h', 'Dialog.h'):
        assert name in files, '%s stub not generated' % name
        assert 'EXTERNAL-LIBRARY STUB' in files[name]


def test_point_has_x_y_fields():
    p = runtime.generate_files(platform_id='xbox360')['Point.h']
    assert 'int x;' in p and 'int y;' in p


def test_pointerinfo_getlocation_returns_valid_point():
    pi = runtime.generate_files(platform_id='xbox360')['PointerInfo.h']
    assert 'Point* getLocation()' in pi
    assert 'return &' in pi                       # a valid (static) Point, never NULL -> no crash on deref


def test_mouseinfo_getpointerinfo_returns_pointerinfo():
    mi = runtime.generate_files(platform_id='xbox360')['MouseInfo.h']
    assert 'namespace MouseInfo' in mi
    assert 'PointerInfo getPointerInfo' in mi


def test_component_getlocationonscreen_returns_point():
    comp = runtime.generate_files(platform_id='xbox360')['Component.h']
    assert 'Point* getLocationOnScreen()' in comp   # was void* -> couldn't assign to Point*


def test_awt_types_registered_for_static_and_include():
    # RUNTIME_HEADERS -> `#include "X.h"` + static-access set; MouseInfo is static (MouseInfo.getPointerInfo())
    for name in ('Point', 'PointerInfo', 'Dialog', 'MouseInfo'):
        assert name in typemap.RUNTIME_HEADERS
    assert 'MouseInfo' in typemap.RUNTIME_STATIC_HOLDERS
