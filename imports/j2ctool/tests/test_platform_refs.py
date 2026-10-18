# -*- coding: utf-8 -*-
"""Tests for detection-driven, game-agnostic platform emission (j2ctool.platform_refs)."""

import os
import unittest

from j2ctool import ir, platform_refs


def _cls_with_main(name='RubyDung'):
    c = ir.JClass(name)
    m = ir.Method('main', ir.JType('void'),
                  params=[ir.Param('args', ir.JType('String', array_dims=1))],
                  is_static=True, line=1)
    c.methods.append(m)
    return c


class DetectMainClassTests(unittest.TestCase):
    def test_detects_static_string_array_main(self):
        classes = [ir.JClass('Level'), _cls_with_main('RubyDung')]
        self.assertEqual(platform_refs.detect_main_class(classes), 'RubyDung')

    def test_no_main_returns_none(self):
        self.assertIsNone(platform_refs.detect_main_class([ir.JClass('Level')]))

    def test_instance_main_is_not_entry(self):
        c = ir.JClass('X')
        c.methods.append(ir.Method('main', ir.JType('void'),
                                   params=[ir.Param('a', ir.JType('String', array_dims=1))],
                                   is_static=False, line=1))
        self.assertIsNone(platform_refs.detect_main_class([c]))


class GenerateEntryPointTests(unittest.TestCase):
    def test_generic_entry_targets_detected_class(self):
        cpp = platform_refs.generate_entry_point('RubyDung')
        self.assertIn('RubyDung::main(args)', cpp)
        self.assertIn('j2x_initAll();', cpp)
        self.assertIn('#include "RubyDung.h"', cpp)
        self.assertNotIn('Minecraft', cpp)          # no game hardcoding


class EmitCollisionSkipTests(unittest.TestCase):
    def setUp(self):
        import tempfile
        self.ref = tempfile.mkdtemp(prefix='j2cref_')
        self.out = tempfile.mkdtemp(prefix='j2cout_')
        # a genuine platform reference (game never defines it) + a companion the game DOES define
        self._w(self.ref, 'GL11.reference.h', '#pragma once\nnamespace GL11 { void glBegin(int); }\n')
        self._w(self.ref, 'GL11.reference.cpp', '#include "GL11.h"\n')
        self._w(self.ref, 'Textures.ext.reference.cpp', '// uses Textures::genId\n')
        os.makedirs(os.path.join(os.path.dirname(self.ref), ''), exist_ok=True)
        comp = os.path.join(os.path.dirname(self.ref), 'companions')
        # companions dir is a sibling of reference dir; emit_references derives it that way
        self.comp = os.path.join(self.ref, '..', 'companions')

    def _w(self, d, fn, s):
        with open(os.path.join(d, fn), 'w', encoding='utf-8') as f:
            f.write(s)

    def test_platform_reference_emitted_but_game_collision_skipped(self):
        emitted = platform_refs.emit_references(
            self.out, self.ref, game_classes={'Textures'}, main_class='RubyDung')
        # GL11 is a platform API the game does not define -> emitted
        self.assertIn('GL11', emitted)
        self.assertTrue(os.path.isfile(os.path.join(self.out, 'GL11.h')))
        # Textures.ext collides with the game-defined Textures -> skipped
        self.assertFalse(os.path.isfile(os.path.join(self.out, 'Textures.ext.cpp')))
        self.assertIn('Textures.ext', platform_refs.emit_references.last_skipped)
        # generic entry generated for the detected main class
        with open(os.path.join(self.out, 'Main.cpp'), 'r', encoding='utf-8') as f:
            main = f.read()
        self.assertIn('RubyDung::main(args)', main)
