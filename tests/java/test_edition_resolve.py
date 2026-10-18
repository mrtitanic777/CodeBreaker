# -*- coding: utf-8 -*-
"""Edition resolution: the version-id map and the resolve() precedence (explicit > version > detect)."""

import pytest

from codebreaker.java import edition


@pytest.mark.parametrize('vid,expected', [
    ('a1.2.6', 'alpha'),
    ('b1.6.6', 'beta'),
    ('c0.30_01c', 'rubydung'),
    ('rd-132211', 'rubydung'),
    ('inf-20100618', 'infdev'),
    ('infdev', 'infdev'),
    ('in-20100223', 'indev'),
    ('indev', 'indev'),
    ('1.2.5', 'release'),
    ('garbage', None),
    ('', None),
    (None, None),
])
def test_edition_for_version(vid, expected):
    assert edition.edition_for_version(vid) == expected


def test_infdev_wins_over_indev_prefix():
    # 'inf-' must be tried before 'in-' so infdev is not mis-mapped to indev.
    assert edition.edition_for_version('inf-20100618') == 'infdev'


def test_profile_name_mapping():
    assert edition.profile_name('alpha') == 'minecraft_alpha'
    assert edition.profile_name(None) is None
    assert edition.profile_name('bogus') is None


def test_resolve_precedence_explicit_over_version():
    assert edition.resolve('beta', mc_version='a1.2.6', classes=None) == 'minecraft_beta'


def test_resolve_version_when_no_explicit():
    assert edition.resolve(None, mc_version='a1.2.6') == 'minecraft_alpha'


def test_resolve_accepts_qualified_profile_name():
    assert edition.resolve('minecraft_release') == 'minecraft_release'


def test_resolve_none_when_nothing_conclusive():
    assert edition.resolve(None, None, classes=[]) is None


def test_resolve_rejects_unknown_edition():
    with pytest.raises(ValueError):
        edition.resolve('nonsense')
