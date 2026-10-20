# -*- coding: utf-8 -*-
"""The Minecraft project: RubyDung, Indev, Infdev, Alpha, Beta and release.

What only Minecraft needs, behind the project hooks (codebreaker/java/projects):
  * its target profiles (profiles/), one per era, chosen by edition.py (--edition, --mc-version, or the
    source's own marks);
  * the console fixes to the game's own logic -- on the IR (sound_fix, applet_ping, render_fix) and on
    the written program (controller_nav, keybind_mark, raycast);
  * its reports: the asset manifest (content), achievements, platform-layer coverage;
  * data: the font's house glyph, the class registrars its reflection uses.
"""
import os

from codebreaker.java.projects import Project
# the era profiles register themselves with codebreaker.java.profiles on import
from codebreaker.java.projects.minecraft.profiles import (minecraft_xbox360, minecraft_rubydung,  # noqa: F401
                                                          minecraft_indev, minecraft_infdev, minecraft_alpha,
                                                          minecraft_beta, minecraft_release)


# the Tessellator holds an MB-scale vertex buffer: a by-value copy of it is HIGH (rules.py)
from codebreaker.java import profiles as _profiles  # noqa: E402
for _n in _profiles.names():
    if _n.startswith('minecraft_'):
        _profiles.get(_n).heavy_types_high = ('Tesselator', 'Tessellator')


class Minecraft(Project):
    name = 'minecraft'
    char_map = {0x2302: 127}             # U+2302 (house): the font's tile 127, its CP437 byte
    registrars = (r'addMapping', r'addIdClassMapping', r'register\w*', r'addClassMapping')   # EntityList,
    #                                                                     Packet, TileEntity
    default_profile = 'minecraft_xbox360'

    def profile_for(self, edition=None, mc_version=None, classes=None):
        from codebreaker.java.projects.minecraft import edition as _edition
        return _edition.resolve(edition, mc_version=mc_version, classes=classes) or self.default_profile

    def ir_patches(self, units, platform):
        if platform != 'xbox360':
            return {}
        from codebreaker.java.projects.minecraft import sound_fix, applet_ping, render_fix
        return {'sound': sound_fix.patch(units), 'applet': applet_ping.patch(units),
                'render': render_fix.patch(units)}

    def finalize_text(self, text, platform):
        if platform != 'xbox360':
            return text
        from codebreaker.java.projects.minecraft import raycast
        return raycast.convert_gl_pick_to_raycast(text)

    def after_emit(self, out_dir, platform, patched, prog):
        if platform != 'xbox360':
            return
        from codebreaker.java.projects.minecraft import sound_fix, applet_ping, render_fix, controller_nav
        # SFX BYPASS: SoundManager gates every sound effect on the OGG pool, EMPTY on the console (sound
        # comes from XACT banks) -> all SFX silent. The guard is dropped and the name routed to the bridge.
        sf = dict(patched.get('sound', {'sites': 0, 'files': []}), stranded=sound_fix.stranded(out_dir))
        if sf['sites']:
            prog('[sound] SFX+music bypass: %d site(s) routed to the XACT bridge across %d file(s)'
                 % (sf['sites'], len(sf['files'])))
        if sf['stranded']:
            prog('[sound] WARNING: a soundPool*->getRandomSound null-guard survived in %s -- sound may be '
                 'silent there (guard shape changed; update sound_fix.py)' % ', '.join(sf['stranded']))
        # CONTROLLER MENU NAVIGATION: the D-pad adapter into GuiScreen + the per-frame call (the algorithm
        # lives in Input.reference). Before overlays (an overlay wins).
        cn = controller_nav.run(out_dir)
        if cn['guiscreen_cpp'] and cn['callsite']:
            prog('[controller] menu nav injected into GuiScreen (+ per-frame call)')
        if cn['missing']:
            prog('[controller] WARNING: menu-nav anchor(s) not found -> menus may be un-navigable: %s'
                 % '; '.join(cn['missing']))
        # APPLET ANTI-PIRACY PING: setLevel's getDocumentBase()->openStream() faults on console.
        ap = dict(patched.get('applet', {'sites': 0, 'files': []}), stranded=applet_ping.stranded(out_dir))
        if ap['sites']:
            prog('[applet] anti-piracy URL ping neutralized: %d site(s) across %d file(s)'
                 % (ap['sites'], len(ap['files'])))
        if ap['stranded']:
            prog('[applet] WARNING: a getDocumentBase()->openStream() ping survived in %s -- may crash on '
                 'setLevel (ping shape changed; update applet_ping.py)' % ', '.join(ap['stranded']))
        # RENDER: the per-frame chunk-rebuild budget and the hurt-camera 0/0 NaN guard.
        rf = dict(patched.get('render', {'update': 0, 'hurt': 0, 'files': []}), stranded=render_fix.stranded(out_dir))
        if rf['update']:
            prog('[render] chunk-rebuild budget applied to updateRenderers (%d site(s))' % rf['update'])
        if rf['hurt']:
            prog('[render] hurt-camera 0/0 NaN guard applied (%d site(s))' % rf['hurt'])
        if rf['stranded']:
            prog('[render] WARNING: a render-fix trigger survived in %s -- first in-game frame may hang '
                 'or be invisible (shape changed; update render_fix.py)' % ', '.join(rf['stranded']))

    def after_construction(self, out_dir, platform, prog):
        if platform != 'xbox360':
            return
        # KEYBIND MARKING: F1/F3/ESC/hotbar... are keys the console does not have; tag each site.
        from codebreaker.java.projects.minecraft import keybind_mark
        kb = keybind_mark.run(out_dir)
        prog('[keybind] tagged %d keyboard key-press site(s) -> KEYBIND_INDEX.md (route to controller/menu)'
             % len(kb['sites']))

    def reports(self, java_dir, out_dir, runtime_files, prog):
        from codebreaker.java.projects.minecraft import content, achievements, platform_coverage
        from codebreaker.java._paths import data_path
        # asset manifest: what textures/sounds/music/lang files must ship in the content folder
        cm = content.write_content_md(java_dir, out_dir)
        prog('[content] asset manifest written to %s' % cm)
        # achievements -> the console achievement system (title config + networked profile write)
        found = achievements.scan_achievements(out_dir)
        if found:
            achievements.run(out_dir, out_dir)
            prog('[achievements] %d found -> ACHIEVEMENTS.md + achievements.xlast + Achievements.h/.cpp'
                 % len(found))
        # platform-layer coverage against the Alpha reference layer
        ref_dir = data_path('platform', 'reference')
        pc_path, _pc = platform_coverage.write_report(
            out_dir, reference_dir=(ref_dir if os.path.isdir(ref_dir) else None),
            runtime_files=set(runtime_files))
        prog('[platform] coverage report written to %s' % pc_path)


PROJECT = Minecraft()
