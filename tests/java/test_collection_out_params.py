# -*- coding: utf-8 -*-
"""compute_collection_out_params: a Java collection param a method MUTATES is an out-param and must lower
to a C++ reference (JList<T>&), else the value-struct copy drops the callee's writes -- the
getCollidingBoundingBoxes 'player falls through the world' bug. Covers all three signals + the read-only
negative case + the emitter rendering."""

import unittest

from codebreaker.java import ir, transpile as pipeline
from codebreaker.java.emit import ownership


def _m(name, params, body_lines):
    m = ir.Method(name, ir.JType('void'), params=params, line=1)
    m.body = [(i + 1, t) for i, t in enumerate(body_lines)]
    return m


class TestCollectionOutParams(unittest.TestCase):
    def _run(self):
        # Base.fill DIRECTLY mutates its list param (var1.add) -> out-param.
        base = ir.JClass('Base')
        base.methods.append(_m('fill',
            [ir.Param('var0', ir.JType('World')), ir.Param('var1', ir.JType('ArrayList'))],
            ['void fill(World var0, ArrayList var1) {', '\tvar1.add(var0);', '}']))
        # Sub overrides fill and ONLY forwards to super -- never touches var1 directly. Must still be `&`
        # (virtual override signatures must match + the forward would otherwise mutate a dead copy).
        sub = ir.JClass('Sub', parent='Base')
        sub.methods.append(_m('fill',
            [ir.Param('var0', ir.JType('World')), ir.Param('var1', ir.JType('ArrayList'))],
            ['void fill(World var0, ArrayList var1) {', '\tsuper.fill(var0, var1);', '}']))
        # sink mutates a Map param; Forwarder.forward passes its Map into sink (non-override forwarding).
        holder = ir.JClass('Holder')
        holder.methods.append(_m('sink', [ir.Param('var1', ir.JType('Map'))],
            ['void sink(Map var1) {', '\tvar1.put(1, 2);', '}']))
        holder.methods.append(_m('forward', [ir.Param('var1', ir.JType('Map'))],
            ['void forward(Map var1) {', '\tthis.sink(var1);', '}']))
        # readOnly only ITERATES its list -> stays by value (a safe copy; not an out-param).
        reader = ir.JClass('Reader')
        reader.methods.append(_m('readOnly', [ir.Param('var1', ir.JType('List'))],
            ['void readOnly(List var1) {', '\tIterator var2 = var1.iterator();', '}']))

        classes = [base, sub, holder, reader]
        pipeline.resolve_inheritance(classes)
        resolver = pipeline.build_resolver(classes)
        n = pipeline.compute_collection_out_params(classes, resolver)
        by = dict((c.name, c) for c in classes)
        return by, resolver, n

    def test_direct_mutation_promoted(self):
        by, _r, _n = self._run()
        self.assertTrue(by['Base'].methods[0].params[1].by_ref)     # var1 mutated
        self.assertFalse(by['Base'].methods[0].params[0].by_ref)    # World var0 untouched

    def test_override_forward_matches_base(self):
        by, _r, _n = self._run()
        # Sub.fill only calls super -- promoted so its signature matches Base's virtual `&`.
        self.assertTrue(by['Sub'].methods[0].params[1].by_ref)

    def test_nonoverride_forwarding_promoted(self):
        by, _r, _n = self._run()
        self.assertTrue(by['Holder'].methods[1].params[0].by_ref)   # forward() -> sink() out slot

    def test_readonly_stays_by_value(self):
        by, _r, _n = self._run()
        self.assertFalse(by['Reader'].methods[0].params[0].by_ref)  # iterate-only: safe copy

    def test_emitter_renders_reference(self):
        by, resolver, _n = self._run()
        sig = ownership.params_str(by['Base'].methods[0], resolver)
        self.assertIn('& var1', sig)                                # promoted -> reference in the C++ signature
        self.assertNotIn('& var0', sig)                             # non-collection param unaffected


if __name__ == '__main__':
    unittest.main()
