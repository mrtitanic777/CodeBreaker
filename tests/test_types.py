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
        for n in ('Block', 'World', 'Entity', 'ICamera'):
            self.assertEqual(self.r.ownership(T(n)), 'pointer', n)

    def test_enums_are_value(self):
        # enums map to int (value); constants are scoped static const ints (EnumOS::linux)
        self.assertEqual(self.r.ownership(T('EnumOS')), 'value')
        self.assertEqual(self.r.field_type(T('EnumOS')), 'int')

    def test_unknown_capitalized_is_pointer(self):
        self.assertEqual(self.r.ownership(T('SomeNewClass')), 'pointer')

    def test_arrays_are_value(self):
        # Java arrays -> JArray<T>, a value wrapper (held by value, '.' access, '.length')
        self.assertEqual(self.r.ownership(T('int', 1)), 'value')
        self.assertEqual(self.r.ownership(T('Block', 1)), 'value')


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
        # String -> JavaString (std::string subclass with Java's String methods)
        self.assertEqual(self.r.field_type(T('String')), 'JavaString')
        self.assertEqual(self.r.param_type(T('String')), 'const JavaString&')
        self.assertEqual(self.r.return_type(T('String')), 'JavaString')   # by value (no dangling)

    def test_primitive_arrays(self):
        self.assertEqual(self.r.field_type(T('int', 1)), 'JArray<int >')
        self.assertEqual(self.r.field_type(T('boolean', 1)), 'JArray<bool >')

    def test_reference_arrays_hold_pointers(self):
        # a reference-type element is itself a pointer -> JArray<T*>
        self.assertEqual(self.r.field_type(T('Block', 1)), 'JArray<Block* >')
        self.assertEqual(self.r.field_type(T('ItemStack', 1)), 'JArray<ItemStack* >')
        self.assertEqual(self.r.field_type(T('World', 1)), 'JArray<World* >')
        self.assertEqual(self.r.field_type(T('Mystery', 1)), 'JArray<Mystery* >')   # unknown cap = ref

    def test_value_type_array_holds_values(self):
        self.assertEqual(self.r.field_type(T('Vec3D', 1)), 'JArray<Vec3D >')
        self.assertEqual(self.r.field_type(T('String', 1)), 'JArray<JavaString >')

    def test_two_dimensional_arrays(self):
        self.assertEqual(self.r.field_type(T('int', 2)), 'JArray<JArray<int > >')
        self.assertEqual(self.r.field_type(T('Block', 2)), 'JArray<JArray<Block* > >')

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
