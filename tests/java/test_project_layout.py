# -*- coding: utf-8 -*-
"""Tests for project_layout: flat converted tree -> VS solution layout (<root>/<Name>.sln + subdir)."""
import os
import tempfile
import shutil
import unittest

from codebreaker.java.console import project_layout as pl


def _touch(path, text='x'):
    with open(path, 'w', encoding='utf-8') as f:
        f.write(text)


class TestLayout(unittest.TestCase):
    def setUp(self):
        self.d = tempfile.mkdtemp()
        self.root = os.path.join(self.d, 'MyGame')
        os.makedirs(self.root)

    def tearDown(self):
        shutil.rmtree(self.d, ignore_errors=True)

    def _flat_dump(self):
        for n in ('Foo.cpp', 'Bar.cpp', 'stdafx.cpp', 'Foo.h', 'Bar.h', 'stdafx.h'):
            _touch(os.path.join(self.root, n), '// ' + n)
        os.makedirs(os.path.join(self.root, 'Content'))
        _touch(os.path.join(self.root, 'Content', 'terrain.png'), 'png')

    def test_flat_dump_gets_structured(self):
        self._flat_dump()
        r = pl.layout(self.root)
        self.assertEqual(r['name'], 'MyGame')
        self.assertTrue(r['moved'] >= 7)
        # root now holds ONLY the project subfolder + the .sln
        self.assertEqual(sorted(os.listdir(self.root)), ['MyGame', 'MyGame.sln'])
        proj = os.path.join(self.root, 'MyGame')
        self.assertTrue(os.path.isfile(os.path.join(proj, 'Foo.cpp')))
        self.assertTrue(os.path.isdir(os.path.join(proj, 'Content')))
        self.assertTrue(os.path.isfile(os.path.join(proj, 'MyGame.vcxproj')))

    def test_sln_binds_to_project_and_guid(self):
        self._flat_dump()
        r = pl.layout(self.root)
        sln = open(r['sln'], encoding='utf-8').read()
        self.assertIn('MyGame\\MyGame.vcxproj', sln)
        self.assertIn(r['guid'], sln)
        vcx = open(r['vcxproj'], encoding='utf-8').read()
        self.assertIn(r['guid'], vcx)
        self.assertIn('Foo.cpp', vcx)                         # file list refreshed
        self.assertIn('Create</PrecompiledHeader>', vcx)      # stdafx PCH block preserved

    def test_template_instantiates_to_valid_xml(self):
        # Guards the class of bug where a comment contained '--' (illegal in XML) -> VS "could not be
        # loaded / An XML comment cannot contain '--'". The template with {{GUID}} filled must parse.
        import xml.dom.minidom as minidom
        tmpl = open(pl._TEMPLATE, encoding='utf-8').read().replace(
            '{{GUID}}', '{AAAABBBB-CCCC-DDDD-EEEE-FFFF00001111}')
        minidom.parseString(tmpl)                             # raises if malformed

    def test_generated_project_files_are_valid_xml(self):
        import xml.dom.minidom as minidom
        self._flat_dump()
        r = pl.layout(self.root)
        for key in ('vcxproj',):
            minidom.parse(r[key])                             # .vcxproj parses
        filt = os.path.splitext(r['vcxproj'])[0] + '.vcxproj.filters'
        self.assertTrue(os.path.isfile(filt))
        minidom.parse(filt)                                   # .filters parses

    def test_idempotent_no_triple_nest(self):
        self._flat_dump()
        r1 = pl.layout(self.root)
        r2 = pl.layout(self.root)
        self.assertEqual(r2['moved'], 0)
        self.assertEqual(r1['guid'], r2['guid'])
        # never creates <root>/<Name>/<Name>/
        self.assertFalse(os.path.exists(os.path.join(r1['proj_dir'], 'MyGame')))
        self.assertEqual(sorted(os.listdir(self.root)), ['MyGame', 'MyGame.sln'])

    def test_guid_is_deterministic_per_name(self):
        self.assertEqual(pl.project_guid('MyGame'), pl.project_guid('MyGame'))
        self.assertNotEqual(pl.project_guid('MyGame'), pl.project_guid('Other'))

    def test_existing_vcxproj_guid_is_reused(self):
        # a pre-existing project keeps its own GUID (don't rebind an established solution)
        proj = os.path.join(self.root, 'MyGame')
        os.makedirs(proj)
        _touch(os.path.join(proj, 'Foo.cpp'))
        pre = '{AAAABBBB-CCCC-DDDD-EEEE-FFFF00001111}'
        tmpl = open(pl._TEMPLATE, encoding='utf-8').read().replace('{{GUID}}', pre)
        _touch(os.path.join(proj, 'MyGame.vcxproj'), tmpl)
        r = pl.layout(self.root)
        self.assertEqual(r['guid'], pre)
        self.assertIn(pre, open(r['sln'], encoding='utf-8').read())

    def test_already_structured_is_left_alone(self):
        # sources already in the subfolder -> nothing moves, just sln/vcxproj ensured
        proj = os.path.join(self.root, 'MyGame')
        os.makedirs(proj)
        for n in ('Foo.cpp', 'stdafx.cpp'):
            _touch(os.path.join(proj, n))
        r = pl.layout(self.root)
        self.assertEqual(r['moved'], 0)
        self.assertTrue(os.path.isfile(r['sln']))


if __name__ == '__main__':
    unittest.main()
