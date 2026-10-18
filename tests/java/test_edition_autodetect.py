# -*- coding: utf-8 -*-
"""Source auto-detection: detect_edition() over tiny synthetic corpora, conservative when uncertain."""

from codebreaker.java import edition, parse


def _corpus(*sources):
    return [parse.parse_source(s) for s in sources]


def test_rubydung_by_package():
    c = _corpus('package com.mojang.rubydung;\npublic class RubyDung {}',
                'package com.mojang.rubydung.level;\npublic class Level {}')
    assert edition.detect_edition(c) == 'rubydung'


def test_indev_finite_world_no_chunks():
    # Real Indev: net.minecraft.game.level.World (a FINITE world, class named World) + LevelLoader,
    # Block/Material registries, and NO Chunk/ChunkProvider. "World present" must NOT mean Alpha here.
    c = _corpus('package net.minecraft.game.level;\npublic class World {}',
                'package net.minecraft.game.level;\npublic class LevelLoader {}',
                'package net.minecraft.game.level.block;\npublic class Block {}',
                'package net.minecraft.game.level.material;\npublic class Material {}')
    assert edition.detect_edition(c) == 'indev'


def test_infdev_chunked_game_package():
    # Chunk streaming appears but still the early net.minecraft.game layout, no biome markers.
    c = _corpus('package net.minecraft.game.level;\npublic class World {}',
                'package net.minecraft.game.level;\npublic class Chunk {}',
                'package net.minecraft.game.level;\npublic class ChunkProvider {}')
    assert edition.detect_edition(c) == 'infdev'


def test_alpha_flat_src_with_chunks():
    c = _corpus('package net.minecraft.src;\npublic class World {}',
                'package net.minecraft.src;\npublic class Chunk {}')
    assert edition.detect_edition(c) == 'alpha'


def test_beta_world_with_biome_markers():
    c = _corpus('package net.minecraft.src;\npublic class World {}',
                'public class BiomeGenBase {}',
                'public class RegionFile {}')
    assert edition.detect_edition(c) == 'beta'


def test_release_by_server_package():
    c = _corpus('package net.minecraft.server;\npublic class MinecraftServer {}',
                'public class World {}')
    assert edition.detect_edition(c) == 'release'


def test_ambiguous_returns_none():
    c = _corpus('public class Foo {}')
    assert edition.detect_edition(c) is None


def test_empty_returns_none():
    assert edition.detect_edition([]) is None
    assert edition.detect_edition(None) is None
