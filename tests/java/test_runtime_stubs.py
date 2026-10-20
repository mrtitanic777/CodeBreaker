# -*- coding: utf-8 -*-
"""The runtime's stub headers and member environment (kept from test_emit_statements when the old
text translator was deleted: these test the runtime, not a translator)."""

import unittest

from codebreaker.java import ir


class TestStubMemberEnv(unittest.TestCase):
    """Runtime-stub method return types are adopted into the member environment so a chain THROUGH a
    stub result resolves '.' vs '->'. Pointer returns become the pointee JType; std::string -> String;
    void*/JArray/primitives are omitted (chain resolver stays conservative)."""
    def setUp(self):
        from codebreaker.java import transpile as pipeline
        self.env = pipeline.build_member_env([])          # no corpus -- only the stub classes

    def test_pointer_return_registered(self):
        self.assertEqual(self.env['File']['methods']['getParentFile'].name, 'File')
        self.assertEqual(self.env['Applet']['methods']['getDocumentBase'].name, 'URL')
        self.assertEqual(self.env['Node']['methods']['getChildNodes'].name, 'NodeList')

    def test_static_factory_from_extra_line(self):
        # `static Toolkit* getDefaultToolkit()` lives in `extra`, not ret_methods. (Runtime is no longer a
        # member-env stub -- it has a real baked implementation now, so it isn't in the stub env.)
        self.assertEqual(self.env['Toolkit']['methods']['getDefaultToolkit'].name, 'Toolkit')
        self.assertNotIn('Runtime', self.env)

    def test_string_return_mapped_to_String(self):
        self.assertEqual(self.env['File']['methods']['getName'].name, 'String')

    def test_ambiguous_returns_omitted(self):
        # void* (Toolkit.getSystemClipboard) and JArray<> (File.listFiles) are not usable pointees
        self.assertNotIn('getSystemClipboard', self.env['Toolkit']['methods'])
        self.assertNotIn('listFiles', self.env['File']['methods'])

    def test_corpus_class_inherits_stub_methods(self):
        # a parsed class extending a stub base picks up the stub's methods via the flatten
        sub = ir.JClass('MyApplet', parent='Applet')
        env = __import__('codebreaker.java.transpile', fromlist=['build_member_env']).build_member_env([sub])
        self.assertEqual(env['MyApplet']['methods']['getDocumentBase'].name, 'URL')


class TestJMapRuntimeHeader(unittest.TestCase):
    """The emitted JMap.h exposes the full Java Map surface as real members so calls bind on any
    receiver (chained/foreign), mirroring the JList/JSet pattern."""
    def setUp(self):
        from codebreaker.java import runtime
        self.files = runtime.generate_files()

    def test_jmap_header_emitted(self):
        self.assertIn('JMap.h', self.files)
        self.assertIn('JIterator.h', self.files)                  # Iterator extracted so JMap can reuse it

    def test_jmap_has_java_map_surface(self):
        h = self.files['JMap.h']
        self.assertIn('class JMap : public std::map<K, V>', h)
        for member in ('put', 'get', 'containsKey', 'remove', 'isEmpty',
                       'values', 'keySet', 'entrySet', 'operator==', 'operator!='):
            self.assertIn(member, h, member)

    def test_list_and_set_expose_iterator_member(self):
        # chained/temporary receivers (map.values().iterator(), (JSet<..>(..)).iterator()) can't be
        # reached by the text rewrite, so JList/JSet carry a real iterator() member.
        self.assertIn('Iterator* iterator()', self.files['JList.h'])
        self.assertIn('Iterator* iterator()', self.files['JSet.h'])

    def test_entry_carries_key_value(self):
        e = self.files['Entry.h']
        self.assertIn('Entry(void* k, void* v)', e)
        self.assertIn('getKey', e)
        self.assertIn('getValue', e)
