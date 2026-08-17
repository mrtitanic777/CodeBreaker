# -*- coding: utf-8 -*-
"""
statements/constants.py -- pure data tables shared across the statements package: the container
receiver-type sets and the Java->C++ method-name maps. No imports, no logic -- a leaf both the
package __init__ and the transform modules import.
"""

# receiver categories
_LIST_TYPES = {'ArrayList', 'List', 'LinkedList'}
_MAP_TYPES = {'HashMap', 'Map', 'TreeMap', 'LinkedHashMap'}
_SET_TYPES = {'HashSet', 'Set'}

# simple renames (args preserved): (category, java method) -> C++ method
_METHOD_RENAME = {
    ('list', 'add'): 'push_back', ('list', 'isEmpty'): 'empty',
    ('map', 'isEmpty'): 'empty', ('map', 'remove'): 'erase',
    ('set', 'add'): 'insert', ('set', 'isEmpty'): 'empty', ('set', 'remove'): 'erase',
    # NOTE: containsKey/contains are NOT remapped to `count` -- JMap::containsKey / JSet::contains already
    # return bool. `count()` returns size_type (unsigned), which forces a C4800 in a bool context.
    ('string', 'isEmpty'): 'empty',
}
# accessor that becomes recv[arg]: category -> java method
# FIXED BUG map-get-inserts (world-gen infinite recursion): Java Map.get() must NOT be rewritten to
# recv[k]. On a std::map, operator[] READ *inserts* a default (null) entry when the key is absent, but
# Java's Map.get() never does. That silently poisoned containsKey(): the first `chunkMap.get(key)` in
# ChunkProvider.provideChunk inserted key->null, so chunkExists() then returned TRUE for an unloaded
# chunk -> World.getHeightValue() (called from generateSkylightMap during that very chunk's generation)
# took the "chunk exists" branch -> provideChunk() again -> func_538_d() again -> infinite regeneration
# of chunk (0,0). JMap has a non-inserting .get(); leave `map.get(k)` as-is (only put -> [k]=v below).
# list/string keep the subscript rewrite (JList::operator[] and JavaString::operator[] don't insert).
_INDEX_METHOD = {'list': 'get', 'string': 'charAt'}
