# -*- coding: utf-8 -*-
"""Regression: the `->length` -> `.length` finalize flip must be type-aware.

JArray's `length` is a value member (`.length`), so a missed array FIELD reached through a pointer
(`var2->imageData->length`) must be corrected to `.length`. But a real scalar `length` FIELD on a
class pointer -- Indev's `World` has `int length`, accessed as `this->length` / `worldObj->length` --
must KEEP its `->`. A blanket flip clobbered the latter (dozens of C2228 errors on the Indev port)."""

from codebreaker.java import transpile as pipeline


def _finalize(text, array_fields=()):
    saved = pipeline._LENGTH_ARRAY_FIELDS
    pipeline._LENGTH_ARRAY_FIELDS = set(array_fields)
    try:
        return pipeline._finalize_emitted(text)
    finally:
        pipeline._LENGTH_ARRAY_FIELDS = saved


def test_scalar_length_field_via_pointer_keeps_arrow():
    assert _finalize('int a = this->length;') == 'int a = this->length;'
    assert _finalize('n = worldObj->length - 1;') == 'n = worldObj->length - 1;'
    # even when it also has an array field elsewhere, `this`/`worldObj` are not array fields
    assert 'this->length' in _finalize('this->width * this->length;', array_fields={'heightMap'})


def test_array_field_via_pointer_is_corrected_to_dot():
    # the documented miss: a byte[] FIELD reached through a typed pointer left as ->length
    assert _finalize('n = var2->imageData->length;', array_fields={'imageData'}) == 'n = var2->imageData.length;'
    assert _finalize('n = this->heightMap->length;', array_fields={'heightMap'}) == 'n = this->heightMap.length;'


def test_call_and_index_results_flip():
    # a call/index result of `length` is (almost always) a JArray -> value member
    assert _finalize('n = getData()->length;') == 'n = getData().length;'
    assert _finalize('n = rows[i]->length;') == 'n = rows[i].length;'


def test_length_method_and_lengthy_names_untouched():
    assert _finalize('n = file->length();') == 'n = file->length();'      # a METHOD, not the field
    assert _finalize('x = a->lengthSquared;') == 'x = a->lengthSquared;'  # different member
