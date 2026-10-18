# -*- coding: utf-8 -*-
"""Warning-cleanup emission passes: C4554 (precedence parens), C4566 (unicode literals),
C4101 (unused catch vars), C4102 (unused labels)."""

import unittest
from codebreaker.java.emit import statements as st


class TestClarifyPrecedence(unittest.TestCase):
    def test_shift_then_and(self):
        self.assertEqual(st._clarify_precedence('int x = a[i] >> 24 & 255;'),
                         'int x = (a[i] >> 24) & 255;')

    def test_sub_then_shift(self):
        self.assertEqual(st._clarify_precedence('var4 = var1 - 1 >> 1;'),
                         'var4 = (var1 - 1) >> 1;')

    def test_shift_or_chain(self):
        self.assertEqual(st._clarify_precedence('int v = 255 - x << 16 | x << 8;'),
                         'int v = ((255 - x) << 16) | (x << 8);')

    def test_xor_with_comparison_in_if(self):
        self.assertEqual(st._clarify_precedence('if((a == 0 || a == 2) ^ b <= 3) {'),
                         'if((a == 0 || a == 2) ^ (b <= 3)) {')

    def test_no_warn_op_untouched(self):
        self.assertEqual(st._clarify_precedence('return a + b * c;'), 'return a + b * c;')

    def test_cast_bails(self):
        self.assertEqual(st._clarify_precedence('x = (Type*)foo & 3;'), 'x = (Type*)foo & 3;')

    def test_ternary_bails(self):
        self.assertEqual(st._clarify_precedence('x = a & b ? c : d;'), 'x = a & b ? c : d;')

    def test_equivalent_value(self):
        # the reparenthesized form must evaluate identically
        env = {'a': 0x1234, 'b': 5, 'i': 0}
        src = 'r = a >> 8 & 15 | b << 4;'
        out = st._clarify_precedence(src)
        lhs = eval(src.split(' = ')[1][:-1], {}, dict(env))
        rhs = eval(out.split(' = ')[1][:-1], {}, dict(env))
        self.assertEqual(lhs, rhs)
        self.assertNotEqual(src, out)


class TestUnicodeLiterals(unittest.TestCase):
    def test_char_mask_to_int(self):
        self.assertEqual(st._fix_unicode_literals("x & '\\uffff'"), 'x & 0xffff')

    def test_house_glyph_to_cp437_byte(self):
        self.assertEqual(st._fix_unicode_literals('"~\\u2302\\u00c7"'), '"~\\x7f\\u00c7"')

    def test_house_glyph_char(self):
        self.assertEqual(st._fix_unicode_literals("c = '\\u2302';"), "c = '\\x7f';")


class TestUnusedCatchVars(unittest.TestCase):
    def test_drops_unused(self):
        self.assertEqual(st._drop_unused_catch_vars(['} catch (Exception* var2) {', '}']),
                         ['} catch (Exception*) {', '}'])

    def test_keeps_used(self):
        self.assertEqual(st._drop_unused_catch_vars(['} catch (IOException* var3) {', 'log(var3);', '}']),
                         ['} catch (IOException* var3) {', 'log(var3);', '}'])


class TestLabelTargets(unittest.TestCase):
    def test_only_referenced_target_emitted(self):
        # break-only labeled loop must NOT emit a _cont: target (would be C4102)
        lines = ['L: for(int i = 0; i < 3; ++i) {', 'if(x) break L;', '}']
        out = st._translate_labels(lines)
        joined = '\n'.join(out)
        self.assertIn('L_brk:;', joined)
        self.assertNotIn('L_cont:;', joined)


if __name__ == '__main__':
    unittest.main()
