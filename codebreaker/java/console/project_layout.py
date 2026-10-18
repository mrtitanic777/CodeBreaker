# -*- coding: utf-8 -*-
r"""
project_layout.py -- give a converted tree the standard IDE solution layout.

The converter writes source files flat into a directory. A Visual Studio solution wants a
*solution root* that holds a `<Name>.sln`, with the actual project (sources + `.vcxproj`) one level
down in a same-named subfolder:

    <root>/<Name>.sln
    <root>/<Name>/<Name>.vcxproj   + all .cpp/.h + Content\ + ...

That is exactly how the reference Alpha port is arranged. Without it, pointing the converter at the
repo root dumps hundreds of files flat there -- and pointing it a second time one level down creates a
duplicate copy (the "duplicate directories" bug). This module makes the layout deterministic:

    layout(root)                    # root basename is the project name
      -> ensures <root>/<Name>/ holds the sources (moves a flat dump down if needed)
      -> writes <Name>.vcxproj from a template if absent, then refreshes its file list
      -> (re)writes <root>/<Name>.sln bound to the project's GUID

Idempotent: running it again on an already-structured root is a no-op beyond refreshing the file list.
The .sln/.vcxproj are Visual-Studio project *configuration* (build settings, not SDK headers/symbols);
the platform SDK gate is unchanged.
"""

import os
import re
import shutil
import hashlib

HERE = os.path.dirname(os.path.abspath(__file__))
_TEMPLATE = os.path.join(HERE, 'templates', 'xbox360.vcxproj.in')

# things that stay at the solution root (never moved down into the project subfolder)
_ROOT_KEEP_EXT = ('.sln', '.sdf', '.suo')

_SLN = '''
Microsoft Visual Studio Solution File, Format Version 11.00
# Visual Studio 2010
Project("{{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}}") = "{name}", "{name}\\{name}.vcxproj", "{guid}"
EndProject
Global
\tGlobalSection(SolutionConfigurationPlatforms) = preSolution
\t\tCodeAnalysis|Xbox 360 = CodeAnalysis|Xbox 360
\t\tDebug|Xbox 360 = Debug|Xbox 360
\t\tProfile_FastCap|Xbox 360 = Profile_FastCap|Xbox 360
\t\tProfile|Xbox 360 = Profile|Xbox 360
\t\tRelease_LTCG|Xbox 360 = Release_LTCG|Xbox 360
\t\tRelease|Xbox 360 = Release|Xbox 360
\tEndGlobalSection
\tGlobalSection(ProjectConfigurationPlatforms) = postSolution
{cfg}
\tEndGlobalSection
\tGlobalSection(SolutionProperties) = preSolution
\t\tHideSolutionNode = FALSE
\tEndGlobalSection
EndGlobal
'''

_CONFIGS = ['CodeAnalysis', 'Debug', 'Profile_FastCap', 'Profile', 'Release_LTCG', 'Release']


def project_guid(name):
    """A stable per-name GUID (so re-running yields the same .sln/.vcxproj binding)."""
    h = hashlib.md5(('j2ctool-project-' + name).encode('utf-8')).hexdigest().upper()
    return '{%s-%s-%s-%s-%s}' % (h[:8], h[8:12], h[12:16], h[16:20], h[20:32])


def _existing_guid(vcx_path):
    if not os.path.isfile(vcx_path):
        return None
    try:
        text = open(vcx_path, 'r', encoding='utf-8-sig', errors='replace').read()
    except (IOError, OSError):
        return None
    m = re.search(r'<ProjectGuid>(\{[^}]*\})</ProjectGuid>', text)
    return m.group(1) if m else None


def _sln_text(name, guid):
    cfg = '\n'.join('\t\t%s.%s|Xbox 360.%s = %s|Xbox 360' % (guid, c, kind, c)
                    for c in _CONFIGS for kind in ('ActiveCfg', 'Build.0'))
    return _SLN.format(name=name, guid=guid, cfg=cfg)


def write_solution(root, name, guid):
    path = os.path.join(root, name + '.sln')
    with open(path, 'w', encoding='utf-8') as f:
        f.write(_sln_text(name, guid))
    return path


def _write_vcxproj_from_template(vcx_path, guid):
    tmpl = open(_TEMPLATE, 'r', encoding='utf-8').read()
    with open(vcx_path, 'w', encoding='utf-8') as f:
        f.write(tmpl.replace('{{GUID}}', guid))


def _has_flat_sources(d):
    try:
        return any(f.endswith('.cpp') for f in os.listdir(d))
    except OSError:
        return False


def _move_flat_into(root, proj_dir, name):
    """Move a flat dump at `root` down into `proj_dir` (everything except the solution files, the
    project subfolder itself, and the .sln). Returns count moved."""
    os.makedirs(proj_dir, exist_ok=True)
    moved = 0
    for entry in os.listdir(root):
        if entry == name:                                     # the project subfolder
            continue
        if entry.lower().endswith(_ROOT_KEEP_EXT):            # keep .sln/.sdf/.suo at root
            continue
        src = os.path.join(root, entry)
        dst = os.path.join(proj_dir, entry)
        if os.path.abspath(src) == os.path.abspath(proj_dir):
            continue
        try:
            if os.path.exists(dst):
                if os.path.isdir(dst):
                    shutil.rmtree(dst, ignore_errors=True)
                else:
                    os.remove(dst)
            shutil.move(src, dst)
            moved += 1
        except (IOError, OSError):
            continue                                          # locked (e.g. an open log) -> leave at root
    return moved


def layout(root, name=None, regen=True):
    """Make `root` a solution root. `name` defaults to the root folder's basename. If sources are
    dumped flat in `root`, they are moved down into `root/<name>/`. Returns a summary dict."""
    root = os.path.abspath(root)
    name = name or os.path.basename(os.path.normpath(root))
    proj_dir = os.path.join(root, name)

    moved = 0
    if not _has_flat_sources(proj_dir) and _has_flat_sources(root):
        moved = _move_flat_into(root, proj_dir, name)
    os.makedirs(proj_dir, exist_ok=True)

    vcx = os.path.join(proj_dir, name + '.vcxproj')
    guid = _existing_guid(vcx) or project_guid(name)
    if not os.path.isfile(vcx):
        _write_vcxproj_from_template(vcx, guid)
    if regen:
        from codebreaker.java.console import vcxproj
        vcxproj.run(proj_dir, name=name)                      # refresh ClCompile/ClInclude + filters

    sln = write_solution(root, name, guid)
    return {'name': name, 'root': root, 'proj_dir': proj_dir,
            'sln': sln, 'vcxproj': vcx, 'guid': guid, 'moved': moved}
