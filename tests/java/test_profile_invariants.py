# -*- coding: utf-8 -*-
"""Structural invariants every registered Minecraft edition profile must satisfy."""

import re

import pytest

from codebreaker.java import profiles

_MC_PROFILES = [n for n in profiles.names() if n.startswith('minecraft_')]


@pytest.mark.parametrize('name', _MC_PROFILES)
def test_filters_are_wellformed(name):
    p = profiles.get(name)
    assert p.vcxproj_filters, '%s has no vcxproj filters' % name
    for rule in p.vcxproj_filters:
        assert isinstance(rule, tuple) and len(rule) == 2, 'bad filter rule in %s: %r' % (name, rule)
        regex, path = rule
        re.compile(regex)                 # must compile
        assert isinstance(path, str) and path


@pytest.mark.parametrize('name', _MC_PROFILES)
def test_nullable_getter_return_types_are_subset(name):
    p = profiles.get(name)
    # every getter with a declared return type must also be in the nullable-getter set (else the type
    # would never be consulted). getSmeltingResultItem-style value getters are simply absent from both.
    extra = set(p.nullable_getter_return_type) - set(p.nullable_getters)
    assert not extra, '%s: return-type keys not in nullable_getters: %s' % (name, sorted(extra))


@pytest.mark.parametrize('name', _MC_PROFILES)
def test_static_init_order_has_no_dupes(name):
    p = profiles.get(name)
    assert len(p.static_init_order) == len(set(p.static_init_order)), '%s: dup in static_init_order' % name


@pytest.mark.parametrize('name', _MC_PROFILES)
def test_render_rule_precedes_entity_block_fallbacks(name):
    # first-match-wins: a Rendering rule must be tried before the generic Entity/Block fallbacks, or
    # RenderX/ModelX would be misfiled as an entity/block.
    paths = [path for _rx, path in profiles.get(name).vcxproj_filters]
    render_idx = next((i for i, pth in enumerate(paths) if pth.startswith('Rendering')), None)
    entity_idx = next((i for i, pth in enumerate(paths) if pth == 'Entities\\Types'), None)
    if render_idx is not None and entity_idx is not None:
        assert render_idx < entity_idx, '%s: Rendering rule after Entities\\Types fallback' % name


def test_all_six_editions_registered():
    for era in ['rubydung', 'indev', 'infdev', 'alpha', 'beta', 'release']:
        assert ('minecraft_' + era) in profiles.names()
