# -*- coding: utf-8 -*-
"""Tests for the vcxproj filter classification, incl. the Debug filter for the debug file."""

from codebreaker.java.console import vcxproj


def test_debug_file_goes_to_debug_filter():
    assert vcxproj._classify('Debug') == 'Debug'
    # the j2ctool platform glue is grouped too
    assert vcxproj._classify('Kernel') == 'Platform\\Config'
    assert vcxproj._classify('KernelConfig') == 'Platform\\Config'
    assert vcxproj._classify('ControllerConfig') == 'Platform\\Config'
    # game code classifies into the deep subsystem hierarchy (Blocks\Base, GUI\Screens, ...)
    assert vcxproj._classify('Block') == 'Blocks\\Base'
    assert vcxproj._classify('GuiMainMenu') == 'GUI\\Screens'


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


def test_classify_uses_inheritance_for_unconventional_names():
    # A class whose NAME matches no rule but that (transitively) extends a known base is filed with
    # that base -- driven by the recovered inheritance graph, not the name.
    parent_of = {'Fluid': 'LiquidBlock', 'LiquidBlock': 'Block'}
    # no rule matches 'Fluid' or 'LiquidBlock' by name -> without inheritance they'd be 'Game (misc)'
    assert vcxproj._classify('Fluid') == 'Game (misc)'
    # with the inheritance chain Fluid -> LiquidBlock -> Block, it lands in the Block filter
    assert vcxproj._classify('Fluid', parent_of) == vcxproj._classify('Block')
    assert vcxproj._classify('LiquidBlock', parent_of) == vcxproj._classify('Block')


def test_build_parent_map_reads_headers(tmp_path):
    (tmp_path / 'Fluid.h').write_text('#ifndef FLUID_H\nclass Fluid : public Block {\n};\n#endif\n',
                                      encoding='utf-8')
    (tmp_path / 'Loose.h').write_text('class Loose {\n};\n', encoding='utf-8')      # root, no base
    (tmp_path / 'Fwd.h').write_text('class Block;\nclass Foo : public Bar {};\n', encoding='utf-8')
    pm = vcxproj._build_parent_map(str(tmp_path))
    assert pm['Fluid'] == 'Block'
    assert pm['Foo'] == 'Bar'
    assert 'Loose' not in pm                                    # no base -> not recorded
    assert 'Block' not in pm                                    # forward decl (no `:`) -> skipped
