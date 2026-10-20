# -*- coding: utf-8 -*-
"""projects -- what belongs to one program family (a game, an application), kept out of the core.

The converter's core knows Java, C++ and the target platform. Everything that knows a particular
program -- its classes, its era, the fixes its own logic needs on a console, the reports only it makes
sense for -- lives in a project package under here, and the core reaches it only through the hooks of
`Project` (docs/TRANSLATOR_DESIGN.md, phase 3: the project layer).

A project is found by its `manifest.json`, read as data: the packages and class names that mark the
program, and the convert options it claims. Only a project that matches (or is asked for) is
imported, so converting any other program loads none of its code. With no match the conversion is
`generic`: no project, no profile data beyond the empty defaults.
"""
import importlib
import json
import os

_HERE = os.path.dirname(os.path.abspath(__file__))


class Project(object):
    """The generic project, and the base every project extends. Each hook is a no-op here."""
    name = 'generic'
    char_map = {}                          # source char code -> the 8-bit code the program's font draws
    registrars = (r'register\w*',)         # calls whose arguments map ids/names to classes (reflection)

    def profile_for(self, edition=None, mc_version=None, classes=None):
        """The target-profile name for this conversion, or None to keep the active one."""
        if edition or mc_version:
            raise ValueError('the generic project takes no edition or version (%r, %r)' % (edition, mc_version))
        return None

    def ir_patches(self, units, platform):
        """Change the lowered program before the back end writes it. -> {report name: report}."""
        return {}

    def finalize_text(self, text, platform):
        """A last change to one emitted C++ file (before the GL namespace rename)."""
        return text

    def after_emit(self, out_dir, platform, patched, prog):
        """After every class is written and before overlays: fixes and checks on the written program."""

    def after_construction(self, out_dir, platform, prog):
        """After two-phase construction (the last rewrite): markers over the final code."""

    def reports(self, java_dir, out_dir, runtime_files, prog):
        """The project's own reports (assets, achievements, platform coverage)."""


GENERIC = Project()
_LOADED = {}


def manifests():
    """[manifest dict] of every project package, by name."""
    out = []
    for d in sorted(os.listdir(_HERE)):
        p = os.path.join(_HERE, d, 'manifest.json')
        if os.path.isfile(p):
            with open(p, encoding='utf-8') as fh:
                m = json.load(fh)
            m.setdefault('name', d)
            out.append(m)
    return out


def load(name):
    """The project called `name` ('generic' or a project package)."""
    if name in (None, '', 'generic'):
        return GENERIC
    if name not in _LOADED:
        m = next((x for x in manifests() if x['name'] == name), None)
        if m is None:
            raise ValueError('unknown project %r (choices: generic, %s)'
                             % (name, ', '.join(x['name'] for x in manifests())))
        _LOADED[name] = importlib.import_module('codebreaker.java.projects.%s' % name).PROJECT
    return _LOADED[name]


def for_profile(profile_name):
    """Load the project that registers target profile `profile_name` (by its manifest's prefix)."""
    for m in manifests():
        if any(profile_name.startswith(p) for p in m.get('profile_prefixes', ())):
            return load(m['name'])
    return None


def matches(manifest, classes):
    """Do the parsed classes carry the manifest's marks (a package prefix or a class name)?"""
    pkgs = tuple(manifest.get('packages', ()))
    names = set(manifest.get('classes', ()))
    for c in classes or ():
        pkg = getattr(c, 'package', None) or ''
        if (pkgs and pkg.startswith(pkgs)) or getattr(c, 'name', None) in names:
            return True
    return False


def choose(classes, project=None, **options):
    """The project for this conversion: the one asked for; else the one claiming a given option (an
    `edition`, an `mc_version`); else the one whose marks the program carries; else generic."""
    if project:
        return load(project)
    given = [k for k, v in options.items() if v]
    for m in manifests():
        if given and set(given) <= set(m.get('options', ())):
            return load(m['name'])
    for m in manifests():
        if matches(m, classes):
            return load(m['name'])
    return GENERIC
