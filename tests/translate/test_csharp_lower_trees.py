# -*- coding: utf-8 -*-
"""The C# front end's lowering on small trees written as Roslyn writes them (no .NET SDK needed): the same trees and
reviewed documents native/cb/translate/frontends/csharp/lower_test.cpp checks the C++ lowering against
(native/cb/translate/frontends/csharp/testdata/). One case per construct that is refused, per hoist rule, per
parenthesis rule, and per shape the corpus programs do not reach; the real programs' dumps are checked the same way."""

import json
import os

import pytest

from codebreaker.translate import l2
from codebreaker.translate.frontends.csharp import lower as L

HERE = os.path.dirname(os.path.abspath(__file__))
DATA = os.path.join(HERE, '..', '..', 'native', 'cb', 'translate', 'frontends', 'csharp', 'testdata')


def _load(name):
    with open(os.path.join(DATA, name), encoding='utf-8') as f:
        return json.load(f)


CASES = _load('csharp_cases.json')
PROGRAMS = ['Shapes', 'Numbers', 'Records', 'Linq']


@pytest.mark.parametrize('case', CASES, ids=[c['name'] for c in CASES])
def test_a_made_tree_lowers_to_the_reviewed_document(case):
    unit, refused = L.lower_with_refusals(json.loads(json.dumps(case['tree'])), 'case')
    assert l2.to_json(unit) == case['l2']
    assert [list(r) for r in refused] == case['refused']


@pytest.mark.parametrize('program', PROGRAMS)
def test_a_corpus_dump_lowers_to_the_reviewed_document(program):
    unit, refused = L.lower_with_refusals(_load('csharp_%s.dump.json' % program), program)
    assert not refused
    assert l2.to_json(unit) == _load('csharp_%s.l2.json' % program)
