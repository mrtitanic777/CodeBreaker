# -*- coding: utf-8 -*-
"""Module 5 tests: end-to-end pipeline (discover -> parse -> emit -> verify -> report)."""

import os
import shutil
import tempfile
import unittest

from codebreaker.java import ir, transpile as pipeline
from codebreaker.java.projects.minecraft import raycast as _raycast


class TestInheritanceResolution(unittest.TestCase):
    def test_inherited_fields_populated(self):
        a = ir.JClass('A')
        a.fields.append(ir.Field('x', ir.JType('int'), access=ir.PUBLIC))
        a.fields.append(ir.Field('secret', ir.JType('int'), access=ir.PRIVATE))   # not inherited
        b = ir.JClass('B', parent='A')
        c = ir.JClass('C', parent='B')
        pipeline.resolve_inheritance([a, b, c])
        self.assertEqual([f.name for f in b.inherited_fields], ['x'])     # private skipped
        self.assertEqual([f.name for f in c.inherited_fields], ['x'])     # transitive through B
        self.assertEqual(a.inherited_fields, [])

FOO = """package a;
public class Foo {
    public int x;
    public Foo(int var1) { this.x = var1; }
    public int getX() { return this.x; }
}
"""
IBAR = """package a;
public interface IBar {
    void doIt();
}
"""


class TestMethodRenameCollision(unittest.TestCase):
    def test_field_method_name_collision_renamed(self):
        # Java allows field 'isRunning' + method 'isRunning(...)'; C++ does not -> rename method.
        from codebreaker.java import types
        from codebreaker.java.emit import header
        c = ir.JClass('NetworkManager')
        c.fields.append(ir.Field('isRunning', ir.JType('boolean')))
        m = ir.Method('isRunning', ir.JType('boolean'),
                      params=[ir.Param('var0', ir.JType('NetworkManager'))], is_static=True, line=1)
        m.body = [(1, 'boolean isRunning(NetworkManager var0) {'), (2, '\treturn var0.isRunning;'), (3, '}')]
        c.methods.append(m)
        renames = pipeline.build_method_renames([c])
        self.assertEqual(renames['NetworkManager'], {'isRunning'})
        r = types.TypeResolver(known_classes={'NetworkManager'}, method_renames=renames)
        h = header.render_header(c, r)
        self.assertIn('bool isRunning;', h)                       # field unchanged
        self.assertIn('static bool isRunning_(NetworkManager* var0);', h)   # method renamed
        # the call site: the back end's x_Call (tests/translate/test_port_semantics.py, Gen.total)


class TestPipeline(unittest.TestCase):
    def setUp(self):
        self.d = tempfile.mkdtemp()
        self.src = os.path.join(self.d, 'src')
        self.out = os.path.join(self.d, 'out')
        os.makedirs(self.src)
        with open(os.path.join(self.src, 'Foo.java'), 'w', encoding='utf-8') as f:
            f.write(FOO)
        with open(os.path.join(self.src, 'IBar.java'), 'w', encoding='utf-8') as f:
            f.write(IBAR)
        self.summary = pipeline.convert(self.src, self.out)

    def tearDown(self):
        shutil.rmtree(self.d, ignore_errors=True)

    def test_summary(self):
        self.assertEqual(self.summary['parsed'], 2)
        self.assertEqual(self.summary['headers'], 2)
        self.assertEqual(self.summary['sources'], 1)        # interface is header-only

    def test_files_written(self):
        self.assertTrue(os.path.isfile(os.path.join(self.out, 'Foo.h')))
        self.assertTrue(os.path.isfile(os.path.join(self.out, 'Foo.cpp')))
        self.assertTrue(os.path.isfile(os.path.join(self.out, 'IBar.h')))
        self.assertFalse(os.path.isfile(os.path.join(self.out, 'IBar.cpp')))

    def test_report_written(self):
        self.assertTrue(os.path.isfile(os.path.join(self.out, 'report', 'index.md')))

    def test_runtime_stub_headers_emitted(self):
        # external base types our corpus extends/implements get emitted stubs so a
        # missing base header never aborts a translation unit
        for h in ('Random.h', 'Runnable.h', 'Thread.h', 'Comparator.h', 'File.h',
                  'FilenameFilter.h', 'Canvas.h', 'Applet.h'):
            self.assertTrue(os.path.isfile(os.path.join(self.out, h)), h)

    def test_translation_applied(self):
        with open(os.path.join(self.out, 'Foo.cpp'), encoding='utf-8') as f:
            cpp = f.read()
        self.assertIn('this->x = var1;', cpp)
        self.assertIn('int Foo::getX()', cpp)


class TestGlSelectPickToRaycast(unittest.TestCase):
    """The GL_SELECT picking idiom (glRenderMode/gluPickMatrix/glPushName name stack) has no D3D
    equivalent; the xbox360 finalizer rewrites it to a screen-center voxel raycast."""

    DRIVER = (
        "void RubyDung::pick(float a)\n"
        "{\n"
        "\td3d9::glSelectBuffer(this->selectBuffer);\n"
        "\td3d9::glRenderMode(d3d9::GL_SELECT);\n"
        "\tthis->setupPickCamera(a, this->width / 2, this->height / 2);\n"
        "\tthis->levelRenderer->pick(this->player);\n"
        "\tint hits = d3d9::glRenderMode(d3d9::GL_RENDER);\n"
        "\tif (this->player != NULL) { this->hitResult = new HitResult(1,2,3,0,1); }\n"
        "\tthis->level->setTile(0,0,0,0);\n"   # forces this->level presence
        "}\n"
    )
    CAMERA = (
        "void RubyDung::setupPickCamera(float a, int x, int y)\n"
        "{\n"
        "\td3d9::gluPickMatrix(x, y, 5.0F, 5.0F, this->viewportBuffer);\n"
        "}\n"
    )
    RENDERER = (
        "void LevelRenderer::pick(Player* player)\n"
        "{\n"
        "\td3d9::glInitNames();\n"
        "\td3d9::glPushName(0);\n"
        "}\n"
    )

    def test_driver_becomes_raycast(self):
        out = _raycast.convert_gl_pick_to_raycast(self.DRIVER)
        self.assertNotIn('GL_SELECT', out)
        self.assertNotIn('glRenderMode', out)
        self.assertNotIn('glSelectBuffer', out)
        self.assertIn('isSolidTile', out)                 # raycast body installed
        self.assertIn('new HitResult(bx, by, bz, 0, face)', out)
        self.assertEqual(out.count('void RubyDung::pick(float a)'), 1)   # header preserved

    def test_helpers_emptied(self):
        cam = _raycast.convert_gl_pick_to_raycast(self.CAMERA)
        self.assertNotIn('gluPickMatrix', cam)
        ren = _raycast.convert_gl_pick_to_raycast(self.RENDERER)
        self.assertNotIn('glPushName', ren)
        self.assertNotIn('glInitNames', ren)

    def test_guard_leaves_non_minecraft_driver_untouched(self):
        # No this->player / this->level / this->hitResult -> cannot raycast -> driver stays.
        alien = (
            "void Widget::pick(float a)\n"
            "{\n"
            "\td3d9::glRenderMode(d3d9::GL_SELECT);\n"
            "\tdoStuff();\n"
            "}\n"
        )
        out = _raycast.convert_gl_pick_to_raycast(alien)
        self.assertIn('GL_SELECT', out)                   # left intact (no MC fields to raycast against)

    def test_no_op_without_idiom(self):
        plain = "void Foo::bar() {\n\treturn;\n}\n"
        self.assertEqual(_raycast.convert_gl_pick_to_raycast(plain), plain)


class TestPruneUnreferenced(unittest.TestCase):
    """Full-product #include-reachability prune: a platform/stub file the game never reaches (audio, net,
    unused libs, dead desktop types) is cut; reachable ones + link-critical J2X-ALWAYS-LINK files stay.
    See pipeline._prune_unreferenced."""

    def setUp(self):
        self.out = tempfile.mkdtemp()

    def tearDown(self):
        shutil.rmtree(self.out, ignore_errors=True)

    def _w(self, name, text):
        with open(os.path.join(self.out, name), 'w', encoding='utf-8') as f:
            f.write(text)

    def test_cuts_unreached_keeps_reachable_and_linkcritical(self):
        # game corpus root: Level. It uses the texture path (Textures.h) which pulls BufferedImage.
        self._w('Level.h', '#include "Textures.h"\nstruct Level{};\n')
        self._w('Level.cpp', '#include "Level.h"\nvoid f(){}\n')
        self._w('Textures.h', '#include "BufferedImage.h"\nstruct Textures{};\n')   # reached via Level
        self._w('BufferedImage.h', 'struct BufferedImage{};\n')                     # reached via Textures
        self._w('Audio.h', 'struct Audio{};\n')            # island -> CUT
        self._w('Audio.cpp', '#include "Audio.h"\n')       # island -> CUT
        self._w('Socket.h', 'struct Socket{};\n')          # island -> CUT
        self._w('HeapTrack.cpp', '// J2X-ALWAYS-LINK\nvoid* operator new(unsigned){return 0;}\n')  # link-critical -> KEEP
        self._w('stdafx.h', '// pch\n')
        dropped = pipeline._prune_unreferenced(self.out, {'Level'}, lambda _s: None)
        self.assertIn('Audio.h', dropped)
        self.assertIn('Audio.cpp', dropped)
        self.assertIn('Socket.h', dropped)
        self.assertNotIn('Textures.h', dropped)        # reachable from Level
        self.assertNotIn('BufferedImage.h', dropped)   # reachable via Textures
        self.assertNotIn('HeapTrack.cpp', dropped)     # J2X-ALWAYS-LINK root
        self.assertFalse(os.path.exists(os.path.join(self.out, 'Audio.h')))
        self.assertTrue(os.path.exists(os.path.join(self.out, 'HeapTrack.cpp')))
        self.assertTrue(os.path.exists(os.path.join(self.out, 'BufferedImage.h')))

    def test_overlay_written_files_are_roots(self):
        """GUARANTEE for overlays.py: a file a project overlay wrote is a prune ROOT even when NOTHING
        #includes it and it defines no referenced symbol. Regression -- the prune runs AFTER
        apply_overlays, so every overlay-ADDED file (Kernel.cpp + KernelConfig.h + ControllerConfig.h,
        and the whole Udp* scaffold that UDP_MIGRATION.md says nothing includes yet BY DESIGN) used to be
        silently deleted on every convert, defeating overlays.py's 'ADDED .cpp are compiled' contract."""
        self._w('Level.h', 'struct Level{};\n')
        self._w('Level.cpp', '#include "Level.h"\nvoid f(){}\n')
        self._w('stdafx.h', '// pch\n')
        # overlay-ADDED: unreachable by include AND by symbol reference -- only the overlay makes them live
        self._w('Kernel.cpp', '#include "KernelConfig.h"\nvoid Kernel_Boot(){}\n')
        self._w('KernelConfig.h', '#define J2X_KERNEL 1\n')
        self._w('UdpSocket.h', 'struct UdpSocket{};\n')
        self._w('Orphan.h', 'struct Orphan{};\n')          # NOT overlaid -> still CUT
        applied = ['Kernel.cpp', 'KernelConfig.h', 'UdpSocket.h']
        dropped = pipeline._prune_unreferenced(self.out, {'Level'}, lambda _s: None,
                                              overlay_files=applied)
        for f in applied:
            self.assertNotIn(f, dropped)
            self.assertTrue(os.path.exists(os.path.join(self.out, f)), '%s was pruned' % f)
        self.assertIn('Orphan.h', dropped)                 # control: prune still cuts real islands

    def test_overlay_roots_accept_nested_relpaths(self):
        """apply_overlays returns RELPATHS (it walks subdirs); the prune works on a flat listing, so a
        nested overlay relpath must still root by basename."""
        self._w('Level.h', 'struct Level{};\n')
        self._w('stdafx.h', '// pch\n')
        self._w('ControllerConfig.h', 'struct Pad{};\n')
        dropped = pipeline._prune_unreferenced(self.out, {'Level'}, lambda _s: None,
                                              overlay_files=[os.path.join('sub', 'ControllerConfig.h')])
        self.assertNotIn('ControllerConfig.h', dropped)

    def test_h_drags_cpp_partner_and_its_includes(self):
        # Level -> Display.h; Display.cpp (partner) pulls Input.h. Input must survive via the .cpp partner.
        self._w('Level.h', '#include "Display.h"\nstruct Level{};\n')
        self._w('Display.h', 'struct Display{};\n')
        self._w('Display.cpp', '#include "Display.h"\n#include "Input.h"\n')
        self._w('Input.h', 'struct Input{};\n')
        self._w('Orphan.h', 'struct Orphan{};\n')      # nothing reaches -> CUT
        dropped = pipeline._prune_unreferenced(self.out, {'Level'}, lambda _s: None)
        self.assertIn('Orphan.h', dropped)
        self.assertNotIn('Input.h', dropped)           # reached through Display.cpp partner
        self.assertNotIn('Display.cpp', dropped)

    def test_cross_named_implementation_kept_by_symbol_defense(self):
        # The real LNK bug: FileInputStream's ctor + a free function are DEFINED in FileIO.cpp but the
        # game references them by NAME without including FileIO.h. Symbol-defense must keep FileIO.cpp.
        self._w('Level.h', '#include "FileInputStream.h"\nstruct Level{};\n')
        self._w('Level.cpp', '#include "Level.h"\nvoid f(){ FileInputStream* s = new FileInputStream(0);'
                             ' InputStream* r = J2X_getResourceAsStream("x"); }\n')
        self._w('FileInputStream.h', 'struct FileInputStream { FileInputStream(void*); };\n')  # decl only
        self._w('FileIO.h', 'struct FileIO{};\n')
        self._w('FileIO.cpp', '#include "FileIO.h"\n'
                              'FileInputStream::FileInputStream(void*) {}\n'
                              'InputStream* J2X_getResourceAsStream(const char* n) { return 0; }\n')
        self._w('Audio.h', 'struct Audio{};\n')
        self._w('Audio.cpp', '#include "Audio.h"\nvoid Audio::play() {}\n')   # symbol unreferenced -> CUT
        dropped = pipeline._prune_unreferenced(self.out, {'Level'}, lambda _s: None)
        self.assertNotIn('FileIO.cpp', dropped)        # defines FileInputStream:: + J2X_getResourceAsStream
        self.assertNotIn('FileIO.h', dropped)          # dragged in via FileIO.cpp
        self.assertIn('Audio.cpp', dropped)            # defines only Audio::, referenced nowhere live
        self.assertIn('Audio.h', dropped)


if __name__ == "__main__":
    unittest.main()


class TestCoalesceForwarders(unittest.TestCase):
    """Pure redirect headers (single #include, no body) are folded into their targets. See
    pipeline._coalesce_forwarders."""

    def setUp(self):
        self.out = tempfile.mkdtemp()

    def tearDown(self):
        shutil.rmtree(self.out, ignore_errors=True)

    def _w(self, name, text):
        with open(os.path.join(self.out, name), 'w', encoding='utf-8') as f:
            f.write(text)

    def test_folds_forwarder_and_rewrites_includes(self):
        self._w('NIOBuffers.h', 'struct ByteBuffer{};\n')                       # real definition
        self._w('ByteBuffer.h', '#ifndef BYTEBUFFER_H\n#define BYTEBUFFER_H\n#include "NIOBuffers.h"\n#endif\n')  # forwarder
        self._w('Level.cpp', '#include "ByteBuffer.h"\nvoid f(){ ByteBuffer b; }\n')
        removed = pipeline._coalesce_forwarders(self.out, lambda _s: None)
        self.assertIn('ByteBuffer.h', removed)
        self.assertFalse(os.path.exists(os.path.join(self.out, 'ByteBuffer.h')))  # forwarder gone
        lvl = open(os.path.join(self.out, 'Level.cpp')).read()
        self.assertIn('#include "NIOBuffers.h"', lvl)   # rewritten to target
        self.assertNotIn('#include "ByteBuffer.h"', lvl)

    def test_resolves_forwarder_chain(self):
        self._w('Real.h', 'struct T{};\n')
        self._w('Mid.h', '#include "Real.h"\n')          # forwarder -> forwarder
        self._w('Top.h', '#include "Mid.h"\n')           # forwarder -> Mid -> Real
        self._w('U.cpp', '#include "Top.h"\n')
        removed = pipeline._coalesce_forwarders(self.out, lambda _s: None)
        self.assertEqual(set(removed), {'Mid.h', 'Top.h'})
        self.assertIn('#include "Real.h"', open(os.path.join(self.out, 'U.cpp')).read())

    def test_real_header_not_touched(self):
        self._w('Real.h', '#include "Other.h"\nstruct Real{};\n')   # has a body -> NOT a forwarder
        self._w('Other.h', 'struct Other{};\n')
        removed = pipeline._coalesce_forwarders(self.out, lambda _s: None)
        self.assertEqual(removed, [])
        self.assertTrue(os.path.exists(os.path.join(self.out, 'Real.h')))
