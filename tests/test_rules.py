# -*- coding: utf-8 -*-
"""Tests for the structural crash-class rules (rules.py), focused on the two added for the Xbox
buildout: virtual-call-in-constructor and eager static-init-order."""

import unittest

from j2x import rules


def _fire(fn, fname, text, header=None):
    return list(fn(fname, text, header))


class TestVirtualCallInCtor(unittest.TestCase):
    HEADER = 'class Foo {\npublic:\n\tFoo();\n\tvirtual void setOpacity(int);\n\tvirtual int calc();\n};\n'

    def test_this_arrow_virtual_call_flagged(self):
        cpp = 'Foo::Foo()\n{\n\tthis->setOpacity(3);\n}\n'
        f = _fire(rules.rule_virtual_call_in_ctor, 'Foo.cpp', cpp, self.HEADER)
        self.assertEqual(len(f), 1)
        self.assertEqual(f[0][3], 'virtual-call-in-ctor')
        self.assertIn('setOpacity', f[0][4])

    def test_bare_virtual_call_flagged(self):
        cpp = 'Foo::Foo()\n{\n\tint n = calc();\n}\n'
        f = _fire(rules.rule_virtual_call_in_ctor, 'Foo.cpp', cpp, self.HEADER)
        self.assertEqual(len(f), 1)
        self.assertIn('calc', f[0][4])

    def test_call_on_other_object_not_flagged(self):
        cpp = 'Foo::Foo()\n{\n\tother->setOpacity(3);\n\tbar.calc();\n\tBase::calc();\n}\n'
        self.assertEqual(_fire(rules.rule_virtual_call_in_ctor, 'Foo.cpp', cpp, self.HEADER), [])

    def test_non_virtual_call_not_flagged(self):
        cpp = 'Foo::Foo()\n{\n\thelper(1);\n}\n'   # helper is not declared virtual in the header
        self.assertEqual(_fire(rules.rule_virtual_call_in_ctor, 'Foo.cpp', cpp, self.HEADER), [])

    def test_no_header_no_crash(self):
        self.assertEqual(_fire(rules.rule_virtual_call_in_ctor, 'Foo.cpp', 'Foo::Foo(){ calc(); }', None), [])


class TestStaticInitOrder(unittest.TestCase):
    def test_new_user_class_flagged(self):
        cpp = 'Bar* Foo::s_inst = new Bar();\n'
        f = _fire(rules.rule_static_init_order, 'Foo.cpp', cpp)
        self.assertEqual(len(f), 1)
        self.assertEqual(f[0][3], 'static-init-order')

    def test_cross_static_reference_flagged(self):
        cpp = 'Tessellator* Foo::s_t = Tessellator::s_instance;\n'
        f = _fire(rules.rule_static_init_order, 'Foo.cpp', cpp)
        self.assertEqual(len(f), 1)

    def test_compile_time_constant_not_flagged(self):
        cpp = 'int Foo::s_n = 5;\nconst float Foo::s_pi = 3.14f;\n'
        self.assertEqual(_fire(rules.rule_static_init_order, 'Foo.cpp', cpp), [])

    def test_runtime_value_type_new_not_flagged(self):
        # JavaString/JArray/JList/JObject have no cross-class static dependency -> safe eager.
        cpp = 'JavaString Foo::s_s = new JavaString("x");\n'
        self.assertEqual(_fire(rules.rule_static_init_order, 'Foo.cpp', cpp), [])

    def test_zero_init_def_not_flagged(self):
        cpp = 'Bar* Foo::s_inst;\n'   # deferred form (no initializer) -- the fixed shape
        self.assertEqual(_fire(rules.rule_static_init_order, 'Foo.cpp', cpp), [])


class TestEndiannessRawCast(unittest.TestCase):
    def test_reinterpret_in_nbt_file_flagged(self):
        cpp = 'void NBTTagInt::write() {\n\tint x = *(int*)ptr;\n}\n'
        f = _fire(rules.rule_endianness_raw_cast, 'NBTTagInt.cpp', cpp)
        self.assertEqual(len(f), 1)
        self.assertEqual(f[0][3], 'endianness')

    def test_memcpy_in_save_file_flagged(self):
        cpp = 'void RegionFile::read() {\n\tmemcpy(dst, src, 4);\n}\n'
        self.assertEqual(len(_fire(rules.rule_endianness_raw_cast, 'RegionFile.cpp', cpp)), 1)

    def test_raw_cast_in_non_io_file_not_flagged(self):
        cpp = 'void Foo::go() {\n\tint x = *(int*)ptr;\n}\n'   # Foo is not a binary-IO file
        self.assertEqual(_fire(rules.rule_endianness_raw_cast, 'Foo.cpp', cpp), [])


if __name__ == '__main__':
    unittest.main()
