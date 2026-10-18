# -*- coding: utf-8 -*-
"""The reseed-on-switch hook: profiles.use() must actually retarget the frozen profile globals in
typemap/rules/vcxproj/signatures/pipeline, and round-trip cleanly (no profile-object pollution)."""

import pytest

from j2ctool import profiles, typemap, rules, vcxproj, signatures, pipeline


@pytest.fixture(autouse=True)
def _restore_default():
    yield
    profiles.use('minecraft_xbox360')


def _globals_match(p):
    assert typemap.UNSIGNED_BYTE_SCALAR_FIELDS == set(p.unsigned_byte_scalar_fields)
    assert typemap.KNOWN_NULLABLE == set(p.nullable)
    assert typemap.KNOWN_NULLABLE_FIELDS == set(p.nullable_fields)
    assert typemap.KNOWN_NULLABLE_GETTERS == set(p.nullable_getters)
    assert typemap.NULLABLE_GETTER_RETURN_TYPE == dict(p.nullable_getter_return_type)
    assert typemap.DEFAULT_VALUE_TYPES == set(p.default_value_types)
    assert rules._HEAVY_TYPES == tuple(p.heavy_types)
    assert vcxproj._FILTER_RULES == list(p.vcxproj_filters)
    assert signatures._PACKAGE_ROLES == dict(p.package_roles)
    assert pipeline._STATIC_INIT_ORDER == list(p.static_init_order)


@pytest.mark.parametrize('name', [n for n in profiles.names()])
def test_use_reseeds_every_global(name):
    p = profiles.use(name)
    _globals_match(p)


def test_generic_empties_globals():
    profiles.use('generic')
    assert typemap.KNOWN_NULLABLE_FIELDS == set()
    assert vcxproj._FILTER_RULES == []
    assert rules._HEAVY_TYPES == ()
    assert signatures._PACKAGE_ROLES == {}
    assert pipeline._STATIC_INIT_ORDER == []


def test_round_trip_is_clean_no_pollution():
    # snapshot the default
    base_getters = set(typemap.KNOWN_NULLABLE_GETTERS)
    base_fields = set(typemap.KNOWN_NULLABLE_FIELDS)
    base_filters = list(vcxproj._FILTER_RULES)
    # switch away and back
    profiles.use('minecraft_rubydung')
    profiles.use('minecraft_xbox360')
    assert typemap.KNOWN_NULLABLE_GETTERS == base_getters
    assert typemap.KNOWN_NULLABLE_FIELDS == base_fields
    assert vcxproj._FILTER_RULES == base_filters


def test_runtime_mutation_does_not_pollute_profile():
    # emulate the pipeline's per-conversion corpus merge (KNOWN_NULLABLE_GETTERS |= ...) and prove it
    # does not leak into the shared Profile object (the reseed copies, never aliases).
    p = profiles.use('minecraft_alpha')
    before = set(p.nullable_getters)
    typemap.KNOWN_NULLABLE_GETTERS.add('__scratch_getter__')
    assert '__scratch_getter__' not in p.nullable_getters      # profile object untouched
    profiles.use('minecraft_alpha')                            # reseed drops the scratch entry
    assert typemap.KNOWN_NULLABLE_GETTERS == before
