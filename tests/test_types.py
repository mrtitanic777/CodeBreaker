# -*- coding: utf-8 -*-
"""Module 3 tests: type + ownership resolution."""

import unittest

from j2x import ir, types


def T(name, dims=0, args=None):
    return ir.JType(name, dims, args)


class TestOwnership(unittest.TestCase):
    def setUp(self):
        self.r = types.TypeResolver(known_classes={'Block', 'World', 'Entity', 'ItemStack'},
                                    value_types={'Vec3D', 'AxisAlignedBB'},
                                    interface_types={'ICamera'},
                                    enum_types={'EnumOS'})

    def test_primitives_are_value(self):
        for n in ('int', 'float', 'boolean', 'long', 'char', 'double'):
            self.assertEqual(self.r.ownership(T(n)), 'value', n)

    def test_value_types(self):
        self.assertEqual(self.r.ownership(T('Vec3D')), 'value')
        self.assertEqual(self.r.ownership(T('AxisAlignedBB')), 'value')
        self.assertEqual(self.r.ownership(T('String')), 'value')

    def test_reference_types_are_pointer(self):
        for n in ('Block', 'World', 'Entity', 'ICamera', 'EnumOS'):
            self.assertEqual(self.r.ownership(T(n)), 'pointer', n)

    def test_unknown_capitalized_is_pointer(self):
        self.assertEqual(self.r.ownership(T('SomeNewClass')), 'pointer')

    def test_arrays_are_pointer(self):
        self.assertEqual(self.r.ownership(T('int', 1)), 'pointer')
        self.assertEqual(self.r.ownership(T('Block', 1)), 'pointer')


class TestCppSpelling(unittest.TestCase):
    def setUp(self):
        self.r = types.TypeResolver(known_classes={'Block', 'Item', 'ItemStack', 'World'},
                                    value_types={'Vec3D'})

    def test_primitive_mapping(self):
        self.assertEqual(self.r.field_type(T('boolean')), 'bool')
        self.assertEqual(self.r.field_type(T('byte')), 'unsigned char')
        self.assertEqual(self.r.field_type(T('long')), 'long long')
        self.assertEqual(self.r.field_type(T('int')), 'int')

    def test_reference_field_gets_pointer(self):
        self.assertEqual(self.r.field_type(T('Block')), 'Block*')
        self.assertEqual(self.r.field_type(T('World')), 'World*')

    def test_value_field_no_pointer(self):
        self.assertEqual(self.r.field_type(T('Vec3D')), 'Vec3D')

    def test_string_contexts(self):
        self.assertEqual(self.r.field_type(T('String')), 'std::string')
        self.assertEqual(self.r.param_type(T('String')), 'const std::string&')
        self.assertEqual(self.r.return_type(T('String')), 'const std::string&')

    def test_primitive_arrays(self):
        self.assertEqual(self.r.field_type(T('int', 1)), 'int*')
        self.assertEqual(self.r.field_type(T('boolean', 1)), 'bool*')

    def test_nullable_reference_arrays_are_double_pointer(self):
        # Block[] / Item[] / ItemStack[] -> T** (NULL slots + polymorphism)
        self.assertEqual(self.r.field_type(T('Block', 1)), 'Block**')
        self.assertEqual(self.r.field_type(T('ItemStack', 1)), 'ItemStack**')

    def test_generic_vector_of_reference(self):
        self.assertEqual(self.r.field_type(T('List', args=[T('ItemStack')])),
                         'std::vector<ItemStack*>')

    def test_generic_vector_of_value(self):
        self.assertEqual(self.r.field_type(T('List', args=[T('int')])),
                         'std::vector<int>')

    def test_raw_container_defaults(self):
        self.assertEqual(self.r.field_type(T('List')), 'std::vector<void*>')
        self.assertEqual(self.r.field_type(T('HashMap')), 'std::map<std::string, void*>')
        self.assertEqual(self.r.field_type(T('HashSet')), 'std::set<void*>')

    def test_object_maps_to_voidstar_with_note(self):
        self.assertEqual(self.r.field_type(T('Object')), 'void*')
        self.assertIsNotNone(self.r.port_note(T('Object')))


if __name__ == '__main__':
    unittest.main()
