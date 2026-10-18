# -*- coding: utf-8 -*-
"""Regression: a `this((X)arg, ...)` delegation whose cast arg is pointer-translated to `(X*)arg`
before inlining must still select the X-typed overload, so a TRANSITIVE delegation's arg is threaded.

ItemStack(Block) -> this((Block)var1, 1) -> ItemStack(Block,int) -> this(var1.blockID, var2)
                 -> ItemStack(int,int) { itemID = var1; }
The (Block*)var1 arg must pick ItemStack(Block,int) (not the same-arity ItemStack(int,int)), or var1
lands directly in `itemID = (Block*)var1` -- a Block*->int error (C2440), dropping `.blockID`."""

import os
import tempfile

from j2ctool import pipeline, profiles


def _convert(src_files):
    d = tempfile.mkdtemp(); out = tempfile.mkdtemp()
    for name, src in src_files.items():
        with open(os.path.join(d, name), 'w', encoding='utf-8') as f:
            f.write(src)
    try:
        pipeline.convert(d, out, platform=None)
        return {n[:-5]: open(os.path.join(out, n[:-5] + '.cpp'), encoding='utf-8').read()
                for n in src_files if n.endswith('.java')}
    finally:
        profiles.use('minecraft_xbox360')


ITEMSTACK = '''public class ItemStack {
   public int itemID;
   public int stackSize;
   public ItemStack(Block var1) { this((Block)var1, 1); }
   public ItemStack(Block var1, int var2) { this(var1.blockID, var2); }
   public ItemStack(int var1, int var2) { this.itemID = var1; this.stackSize = var2; }
}'''
BLOCK = 'public class Block { public int blockID; }'


def test_transitive_delegation_threads_blockid():
    cpp = _convert({'ItemStack.java': ITEMSTACK, 'Block.java': BLOCK})['ItemStack']
    # the Block(1-arg) ctor inlines the whole chain: itemID gets ((Block*)var1)->blockID, not (Block*)var1
    assert '->blockID' in cpp
    assert 'this->itemID = ((Block*)var1);' not in cpp        # the buggy Block*->int assignment is gone
