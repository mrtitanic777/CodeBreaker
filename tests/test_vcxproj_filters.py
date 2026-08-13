# -*- coding: utf-8 -*-
"""Tests for the vcxproj filter classification, incl. the Debug filter for the debug file."""

import os
from j2x import vcxproj


def test_debug_file_goes_to_debug_filter():
    assert vcxproj._classify('Debug') == 'Debug'
    # the j2x platform glue is grouped too
    assert vcxproj._classify('Kernel') == 'Platform\\Config'
    assert vcxproj._classify('KernelConfig') == 'Platform\\Config'
    assert vcxproj._classify('ControllerConfig') == 'Platform\\Config'
    # game code still classifies as before
    assert vcxproj._classify('Block') == 'Blocks'
    assert vcxproj._classify('GuiMainMenu') == 'GUI'


def test_write_filters_emits_debug_folder(tmp_path):
    path, n = vcxproj.write_filters(
        str(tmp_path), 'Proj',
        ['Debug.cpp', 'Block.cpp', 'Kernel.cpp'],
        ['Debug.h', 'Block.h', 'KernelConfig.h'])
    x = open(path, encoding='utf-8').read()
    assert '<Filter Include="Debug">' in x
    assert '<ClCompile Include="Debug.cpp"><Filter>Debug</Filter></ClCompile>' in x
    assert '<ClInclude Include="Debug.h"><Filter>Debug</Filter></ClInclude>' in x
    assert '<Filter Include="Platform\\Config">' in x
