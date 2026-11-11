"""candidates.py -- the model's text made the project's source: the engine's names, structures, longs, the frame's
chars and buffers, the library's helpers, and every rewrite applied to make the candidates a compiler is given."""
import itertools
import re

from codebreaker.exact.draft.text import (_balanced, _changed, _declare_pointer, _if_parts, _same_arm, _strip,
                                          _top_commas, _top_split, ARRAYS, BYTEG, BYTEPTR, FILEPTR, LONGINFO, PTRSIZE,
                                          RANGES, REGS, SIZES, split_body, STRUCTARR, UCHAR)
from codebreaker.exact.draft.shapes import (assign_loops, branch_returns, byte_heads, call_pairs, call_tests,
                                            case_guards, chain_returns, chain_tails, char_widens, common_tails,
                                            compare_values, conditional_values, const_tests, continue_loops,
                                            copy_props, copy_tests, empty_loops, fall_dups, fall_gotos, first_breaks,
                                            flat_returns, fold_registers, for_loops, goto_returns, goto_tails,
                                            heads_variant, high_zero, label_sinks, logical_values, long_tests,
                                            loop_collapse, loop_guards, loop_registers, loop_tests, lvalue_choices,
                                            pointer_index, post_increments, post_steps, push_returns, range_switches,
                                            register_chains, return_cases_first, selector_reads, self_updates,
                                            shared_exit, step_tests, store_joins, store_loops, stored_calls,
                                            stored_reads, stored_tests, stored_values, sunk_pairs, sunk_tails,
                                            switch_follow, switch_ifs, tail_continues, temporaries, ternary_joins,
                                            test_first_loops, tested_zero, two_case_switches, widen_bytes,
                                            word_offsets, zero_extends)
from codebreaker.exact.draft.forms import (_drop_decl, _redeclare, address_variants, and_returns, assign_in_args,
                                           assign_in_tests, bool_trees, case_falls, chain_assigns,
                                           char_param_variants, char_params, char_stores, compare_results,
                                           compound_assigns, data_macros, default_after, default_last,
                                           empty_cases_first, fn_pointers, frame_more, frame_names, guard_flat,
                                           guard_gotos, guard_returns, guard_shared, header_names, high_words,
                                           inner_registers, lead_filler, library_macros, loop_gotos, merge_conditions,
                                           no_default, or_loops, pointer_flow, pointer_widths, pre_increments,
                                           pre_steps, push_tails, register_copies, register_reads, register_temps,
                                           register_values, REGSLOTS, return_sources, scaled_index, scaled_places,
                                           shapes, shared_arms, shared_tails, split_ternaries, store_increments,
                                           store_reloads, switch_breaks, table_macros, unmasked, unsigned_globals,
                                           unsigned_registers, unused_locals, word_arrays, word_constants)


def structs(text, widths):
    """the pointers the text goes through to fields (`p->fN`, N its offset) declared pointers to a struct of
    those fields: a char where the code reads a byte (widths: {(pointer, offset)} of the byte ones), else an
    int, and a pad between"""
    text = re.sub(r"&(\w+)->f(\d+)\b", r"(\1 + \2)", text)  # (lea ax,[p+N] - the model's &p->fN - is p + N)
    text = re.sub(r"&(\w+)\[(-?)(\d+)\]", lambda m: "(%s %s %s)" % (m.group(1), "-" if m.group(2) else "+", m.group(3)), text)
    rets = {}                                               # (a routine's value gone through to a field: a pointer to a
    callrx = r"\b((?!if\b|while\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\([^()]*\))*)\)->f(\d+)\b"   # struct it returns - cast)
    for f_, a_, off in re.findall(callrx, text):
        rets.setdefault(f_, set()).add(int(off))
    pre = []
    for f_, offs in sorted(rets.items()):
        fl, pos = [], 0
        for o in sorted(offs):
            if o > pos:
                fl.append("char _%d[%d];" % (pos, o - pos))
            b_ = ("bx*", o) in widths and ("bxw", o) not in widths
            fl.append("%s f%d;" % ("char" if b_ else "int", o))
            pos = o + (1 if b_ else 2)
        pre.append("struct s_r_%s { %s };" % (f_, " ".join(fl)))
    if rets:
        text = re.sub(callrx, lambda m: "((struct s_r_%s *)%s(%s))->f%s" % (m.group(1), m.group(1), m.group(2), m.group(3)), text)
        text = "\n".join(pre) + "\n" + text
    if ("bx*", 0) in widths and ("bxw", 0) not in widths:  # (*p->fA->fB read a byte: a char there)
        text = re.sub(r"(?<![\w>)\]])\*(\w+(?:->f\d+){2,})\b(?!\s*(?:->|\[|\.|\+\+|--))", r"*(char *)\1", text)
    uses = {}
    for p, off in re.findall(r"\b(\w+)->f(\d+)\b", text):
        if p.startswith("s_r_"):
            continue
        uses.setdefault(p, set()).add(int(off))
    for p in re.findall(r"\*\(\*(\w+)\)|\*\*(\w+)", text):  # (a pointer read through twice: a struct whose first
        p = p[0] or p[1]                                    #  field points on - a list)
        if re.search(r"(?m)^[ \t]*(?:register )?int %s;|^[ \t]*int (?:\w+, )*%s\b" % (p, p), text):
            text = re.sub(r"\*\(\*%s\)|\*\*%s\b" % (p, p), "(%s->f0)->f0" % p, text)
            uses.setdefault(p, set()).add(0)
    if not uses:
        return text
    for p in list(uses):                                    # (p + N, the place of a field - lea - is &p->fN)
        for n in set(re.findall(r"\(%s \+ (\d+)\)" % p, text)):
            if int(n) < 0x100:
                uses[p].add(int(n))
                text = text.replace("(%s + %s)" % (p, n), "&%s->f%s" % (p, n))
    for p in uses:                                          # (*p of a struct pointer: its first field)
        whole = re.compile(r"(?<![\w>)\]])\*%s\b(?!\s*(?:->|\[|\+\+|--|\.))" % p)
        decl = re.compile(r"\s*(?:extern\s+|register\s+|static\s+)*(?:unsigned\s+)?(?:char|int|long|short|struct\s+\w+)\b")
        lines = text.split("\n")
        if any(whole.search(l) for l in lines if not decl.match(l)):
            text = "\n".join(l if decl.match(l) else whole.sub("%s->f0" % p, l) for l in lines)
            uses[p].add(0)
    for p, offs in list(uses.items()):                      # (a pointer stepped and read as *p++, its places all
        bytes_ = lambda o: (p, o) in widths or ("*", o) in widths or \
            p.startswith("r_") and (p[2:] + "*", o) in widths and (p[2:] + "w", o) not in widths
        if re.search(r"\*(?:%s(?:\+\+|--)|(?:\+\+|--)%s\b)" % (re.escape(p), re.escape(p)), text) and offs and \
           max(offs) < 16 and all(bytes_(o) for o in offs):  #  bytes: a char *, the places p[N])
            t2 = re.sub(r"\b%s->f(\d+)\b" % re.escape(p), lambda m: "*%s" % p if m.group(1) == "0" else "%s[%s]" % (p, m.group(1)), text)
            t3 = _declare_pointer(t2, p, "char") if not re.search(r"(?m)^[ \t]*(?:register )?char \*%s;" % re.escape(p), t2) else t2
            if t3:
                text = t3
                del uses[p]
                continue
    for p, offs in list(uses.items()):                      # (a byte read inside a word read: no struct - the
        byte = lambda o: (p, o) in widths or ("*", o) in widths or \
            p.startswith("r_") and (p[2:] + "*", o) in widths and (p[2:] + "w", o) not in widths   # (a register variable's
                                                                    #  bytes: its register's) words through an int *, the bytes through
        words = {o for o in offs if not byte(o)}                     # a char * of it)
        if any(o % 2 and o - 1 in words for o in offs if byte(o)) and all(o % 2 == 0 for o in words):
            t2 = re.sub(r"\b%s->f(\d+)\b" % p, lambda m: "((char *)%s)[%s]" % (p, m.group(1)) if byte(int(m.group(1))) and
                        int(m.group(1)) % 2 else ("*%s" % p if m.group(1) == "0" else "%s[%d]" % (p, int(m.group(1)) // 2)), text)
            t3 = re.sub(r"(?m)^([ \t]*)(register )?char \*%s;$" % p, r"\1\2int *%s;" % p, t2)
            t2 = t3 if t3 != t2 else _declare_pointer(t2, p, "int")
            if t2:
                text = t2
                del uses[p]
                continue
        steps = set(re.findall(r"\b%s = %s [+-] (\d+);" % (p, p), text)) | \
            ({"1"} if re.search(r"(?<![\w>.])%s(?:\+\+|--)|(?:\+\+|--)%s\b" % (p, p), text) else set())   # (stepped by one, bytes alone read through
        if steps == {"1"} and all(byte(o) for o in offs) and max(offs) < 0x10 and \
                not re.search(r"\b%s->f\d+->" % p, text):                 #  it: a char *, its bytes p[N])
            t2 = re.sub(r"\b%s->f(\d+)\b" % p, lambda m: "*%s" % p if m.group(1) == "0" else "%s[%s]" % (p, m.group(1)), text)
            if not re.search(r"(?m)^[ \t]*(?:register\s+)?char \*%s;$" % p, t2):
                t2 = _declare_pointer(t2, p, "char")
            if t2:
                text = t2
                del uses[p]
    # (a field through which the code goes on - p->fN->fM, (p->fN)->fM, *(p->fN): a pointer to a struct of its
    #  own; the same struct when what it reaches are fields p has too - a list, a tree)
    inner = {}
    text = re.sub(r"\*\((\w+)->f(\d+)\)(?!\s*(?:->|\[))", r"(\1->f\2)->f0", text)
    for p, n, m_ in re.findall(r"\b(\w+)->f(\d+)\)?->f(\d+)\b", text):
        if p in uses:
            inner.setdefault((p, int(n)), set()).add(int(m_))
    text = re.sub(r"\((\w+->f\d+)\)->", r"\1->", text)
    # (a whole deref of a field chain that goes on as a pointer elsewhere - *p->fN, *p->fN->fM: its first field)
    for _ in range(3):
        for p, chain in set(re.findall(r"(?<![\w>)\]])\*(\w+)((?:->f\d+)+)\b(?!\s*(?:->|\[|\.))", text)):
            n = int(re.findall(r"->f(\d+)", chain)[-1])
            if p in uses and (re.search(r"\b%s%s->f\d+" % (p, re.escape(chain)), text) or (len(chain.split("->")) > 2)):
                text = re.sub(r"(?<![\w>)\]])\*%s%s\b(?!\s*(?:->|\[|\.))" % (p, re.escape(chain)), "%s%s->f0" % (p, chain), text)
                if len(chain.split("->")) == 2:
                    inner.setdefault((p, n), set()).add(0)
    ptrfield = {}
    defs = []
    deeper = {}                                             # (what chains reach further down: p->fA->fB->fC..)
    for p, chain in re.findall(r"\b(\w+)((?:->f\d+){3,})", text):
        if p in uses:
            deeper.setdefault(p, set()).update(int(o) for o in re.findall(r"->f(\d+)", chain)[2:])
    for (p, n), subs in sorted(inner.items()):
        bytes_ = {o for o in subs if ("*", o) in widths}    # (a byte the code reads where p has a word: another struct)
        # (what bx - the register the model goes on through - reads at an offset, beside what the others read
        #  there: a byte through one and a word through the other is two structs)
        width = lambda regs, o: {("b" if (r + "*", o) in widths else "") + ("w" if (r + "w", o) in widths else "") for r in regs} - {""}
        both = {o for o in subs if width(["bx"], o) and width(["si", "di"], o) and width(["bx"], o) != width(["si", "di"], o)}
        bytes_ = {o for o in subs if "b" in "".join(width(["bx"], o)) or (not width(["bx"], o) and o in bytes_)}
        if (subs <= uses[p] or (p, n) == (p, 0) and subs <= uses[p] | {0}) and not both and \
                not (bytes_ & ({n} | set(k for (q, k) in inner if q == p))):
            ptrfield[(p, n)] = "struct s_%s *" % p
            uses[p] |= deeper.get(p, set())                 # (p's own struct: what the chains reach further too)
            continue
        fl, pos = [], 0
        for o in sorted(subs):
            if o > pos:
                fl.append("char _%d[%d];" % (pos, o - pos))
            b_ = o in bytes_                     # (a byte read: a char - an
            pg = re.search(r"\b%s->f%d->f%d = (\w+);" % (p, n, o), text)   # (given a pointer variable: a pointer)
            pty = re.search(r"(?m)^[ \t]*(?:register\s+)?((?:unsigned\s+)?(?:char|int)|struct \w+) \*%s;$" % pg.group(1), text) if pg else None
            deep = sorted(set(int(x) for x in re.findall(r"\b%s->f%d->f%d->f(\d+)\b" % (p, n, o), text)))
            if deep and not b_:                             # (gone through once more: a pointer to a struct of its own)
                dl, dp = [], 0
                for x in deep:
                    if x < dp:
                        continue
                    if x > dp:
                        dl.append("char _%d[%d];" % (dp, x - dp))
                    dl.append("%s f%d;" % ("long" if x in LONGINFO.get("fields", ()) and x + 2 not in deep else "int", x))
                    dp = x + (4 if dl[-1].startswith("long") else 2)
                defs.append("struct s_%s_%d_%d { %s };" % (p, n, o, " ".join(dl)))
                pty = None
            lng_ = not b_ and not deep and not pty and o in LONGINFO.get("fields", ()) and o + 2 not in subs   # (a long one)
            fl.append("%s f%d;" % ("unsigned char" if any(isinstance(u, tuple) and u[1] == o for u in UCHAR) and b_
                                   else "char" if b_ else "struct s_%s_%d_%d *" % (p, n, o) if deep else
                                   "%s *" % pty.group(1) if pty else "long" if lng_ else "int", o))   #  unsigned one widened with zeros)
            pos = o + (1 if b_ else 4 if lng_ else 2)
        defs.append("struct s_%s_%d { %s };" % (p, n, " ".join(fl)))
        ptrfield[(p, n)] = "struct s_%s_%d *" % (p, n)
    for p, offs in sorted(uses.items()):
        fields = []
        pos = 0
        selfref = re.search(r"\b%s = %s->f0;" % (p, p), text)  # (p = p->f0: a list - the field points on)
        for off in sorted(offs):
            if off < pos:
                continue                                    # (overlaps the field before: not this way)
            if off > pos:
                fields.append("char _%d[%d];" % (pos, off - pos))
            uch = any(isinstance(u, tuple) and u[1] == off for u in UCHAR)   # (widened with zeros: unsigned)
            byte = (p, off) in widths or ("*", off) in widths or uch or \
                p.startswith("r_") and (p[2:] + "*", off) in widths and (p[2:] + "w", off) not in widths   # (a register
                                                                                                        #  variable's)
            if byte and not uch and (p, off) not in widths and p.startswith("g_") and ("bxw", off) in widths and \
                    ("bx*", off) not in widths:
                byte = False                                # (data's pointer, gone through in bx: bx's width there)
            uch = uch or byte and re.search(r"\b%s->f%d \S{1,2} (?:12[89]|1[3-9]\d|2[0-4]\d|25[0-5])\b" % (p, off), text)   # (a byte
                                                                                    #  compared with 128..255: unsigned)
            deref = re.search(r"(?<![\w>)\]])\*%s->f%d\b(?!\s*(?:->|\[|\.))" % (p, off), text)   # (*p->fN: a pointer)
            lng = off in LONGINFO.get("fields", ()) and off + 2 not in offs   # (a long field: two words given as one)
            if lng and byte and (p, off) not in widths and (p.lstrip("r_") + "*", off) not in widths:
                byte = False                                # (a byte another pointer reads there: not this one's)
            pg = re.search(r"\b%s->f%d = (\w+);" % (p, off), text) or \
                re.search(r"(?m)^[ \t]*(\w+) = %s->f%d = " % (p, off), text)   # (given a pointer variable, or given
            pty = re.search(r"(?m)^[ \t]*(?:register\s+)?((?:unsigned\s+)?(?:char|int)|struct \w+) \*%s;$" % pg.group(1), text) if pg else None
            if pty and byte and (p, off) not in widths:     #  with one in a chain: a pointer - not another's byte)
                byte = False
            deref = deref or (pty and pty.group(1) == "int" and not byte and not lng)
            if pty and pty.group(1) != "int" and not byte and not lng and (p, off) not in ptrfield and not (off == 0 and selfref):
                ptrfield[(p, off)] = "%s *" % pty.group(1)
            fields.append("%s f%d;" % ("struct s_%s *" % p if off == 0 and selfref else ptrfield[(p, off)] if (p, off) in ptrfield
                                       else "unsigned char" if uch else
                                       "char" if byte else ("char *" if ("bx*", 0) in widths and ("bxw", 0) not in widths else "int *")
                                       if deref else "long" if lng else "int", off))   # (*p->fN read a byte: a char *)
            pos = off + (2 if (p, off) in ptrfield else 1 if byte else 4 if lng else 2)
        tag = "s_" + p
        steps = {int(n) for n in re.findall(r"\b%s = %s [+-] (\d+);" % (p, p), text)}
        if len(steps) == 1:                                 # (stepped through an array of them: the step is the
            n = steps.pop()                                 #  struct's size, and in C a step of one)
            if n >= pos and n < 0x100:
                if n > pos:
                    fields.append("char _%d[%d];" % (pos, n - pos))
                text = re.sub(r"\b%s = %s ([+-]) %d;" % (p, p, n), r"%s = %s \1 1;" % (p, p), text)
        defs.append("struct %s { %s };" % (tag, " ".join(fields)))
        # the declarations of p: a parameter, a register variable, a local, or data (declared by localize)
        text = re.sub(r"(?m)^([ \t]*)(register )?(?:int|char \*) ?%s;" % p, r"\1\2struct %s *%s;" % (tag, p), text)
        text = re.sub(r"(?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);" % p,
                      lambda m: "%sint %s;\n%sstruct %s *%s;" % (m.group(1), (m.group(2) + m.group(3).lstrip(", ")).strip(", "), m.group(1), tag, p)
                      if (m.group(2) + m.group(3)).strip(", ") else "%sstruct %s *%s;" % (m.group(1), tag, p), text)
        STRUCTPTR[p] = tag
        for (q, n_), ty_ in ptrfield.items():               # (a struct pointer field plus bytes: through a char *)
            if q == p:
                text = re.sub(r"(?<![\w>)])%s->f%d \+ (\d+)\b" % (p, n_), r"(char *)%s->f%d + \1" % (p, n_), text)
        for f_ in re.findall(r"\bchar f(\d+);", defs[-1]):  # (a byte field masked to a byte: itself)
            text = re.sub(r"\(%s->f%s & 0xff\)" % (p, f_), "%s->f%s" % (p, f_), text)
        if ("*", 0) in widths or any(w[0].endswith("*") for w in widths if isinstance(w, tuple) and len(w) == 2 and isinstance(w[0], str)):
            text = re.sub(r"(?<![\w>.])%s\[" % p, "((char *)%s)[" % p, text)   # (indexed by bytes: through a char *)
    return "\n".join(defs) + "\n" + text


STRUCTPTR = {}                                              # (data the draft declares a struct pointer: localize)


def candidates(prj, text, at, names, regs, regparam, widths=()):
    """[(source, registers left)] - the model's text as the project's source, then the variants the compiler
    is to decide between: as read, without its last return (a function of no value), with the locals whose
    address it takes as the buffers they are - and both"""
    base = [text] + [v for v in [void_variant(text)] if v]
    out = []
    REGSLOTS[0] = bool(prj.toolchain().p.get("frame_order"))
    for t in base + [v for v in (buffers(t) for t in base) if v]:
        STRUCTPTR.clear()
        PTRSIZE.clear()
        ARRAYS.clear()
        STRUCTARR.clear()
        UCHAR.clear()
        BYTEG.clear()
        BYTEG.update(n for k, n in widths if k == "g")
        FILEPTR.clear()
        t = re.sub(r"\btarg(\d+)\b", r"t9\1", t)              # (the model's temporaries of a shared tail: temporaries)
        t = zero_extends(high_zero(copy_props(table_macros(prj, case_falls(char_stores(char_params(long_pairs(long_returns(store_reloads(fall_gotos(t))))))), prj.toolchain().p.get("table_macros")))))
        t = re.sub(r"&(\w+)\[(-?)(\d+)\]", lambda m: "(%s %s %s)" % (m.group(1), "-" if m.group(2) else "+", m.group(3)), t)
        t = re.sub(r"&(\w+)->f(\d+)\b", r"(\1 + \2)", t)  # (lea: &p[N], &p->fN - the place, p + N)
        t = fold_registers(stored_calls(store_loops(post_increments(widen_bytes(compare_values(chain_returns(branch_returns(push_returns(conditional_values(logical_values(step_tests(bool_trees(range_switches(loop_gotos(empty_loops(tested_zero(continue_loops(first_breaks(loop_registers(fall_dups(fall_gotos(goto_tails(t) or t))))))))))))))))))))))
        t = stored_calls(fold_registers(push_returns(t)))   # (returns again, as the passes left the arms)
        t = stored_reads(scaled_index(register_chains(word_arrays(pointer_index(test_first_loops(copy_tests(post_steps(selector_reads(lvalue_choices(t))))))))))
        t = fold_registers(stored_values(conditional_values(store_joins(ternary_joins(switch_ifs(loop_collapse(two_case_switches(long_tests(t)))))))))     # (the ?: values again, the elements folded)
        t = fold_registers(stored_tests(push_returns(temporaries(compare_values(goto_returns(sunk_tails(sunk_pairs(empty_loops(shared_arms(merge_conditions(guard_gotos(self_updates(switch_follow(t))))))))))))))
        wd = set(widths)                                    # (a parameter kept in a register: the bytes read through
        for r_, i_ in (regparam or {}).items():             #  that register its own)
            if isinstance(i_, int) and 0 <= i_ < 26:
                wd |= {(chr(97 + i_), o) for (q, o) in widths if q in (r_, r_ + "*") and (r_ + "w", o) not in widths}
        t = structs(pointer_flow(local_pointers(register_variables(shapes(long_pairs(t)), regs, regparam))), wd)
        t = re.sub(r"(?m)^[ \t]*(\w+) = \1;\n", "", t)         # (a register variable given itself: nothing)
        t = switch_ifs(lvalue_choices(register_values(long_ladders(long_pairs(t)))))                     # (the longs again, as the passes left them)
        if re.search(r"\bbp - \d+\b", t):                   # (a frame place the passes left as bp - N: the local)
            t = buffers(t) or t
            t = re.sub(r"\bbp - (\d+)\b", lambda m: ("v%s" % m.group(1)) if re.search(r"(?m)^[ \t]*char v%s\[" % m.group(1), t)
                       else ("(char *)&v%s" % m.group(1)) if re.search(r"(?m)^[ \t]*\w[\w ]*\bv%s;" % m.group(1), t) else m.group(0), t)
        t = frame_chars(fn_pointers(t), widths)
        t0_ = t
        t = fill_frame(t0_, FRAME[0], bool(prj.toolchain().p.get("frame_order")))
        FILLNAME[0] = "_p%d"                                # (the filler named by its place: the frame's order
        tp_ = fill_frame(t0_, FRAME[0], bool(prj.toolchain().p.get("frame_order")))   #  lays it out - a variant)
        FILLNAME[0] = "_pf"
        variants = [t] + [v for v in [tp_ if tp_ != t else None,store_increments(t), pre_steps(t), pre_increments(t), assign_in_tests(t), and_returns(t), inner_registers(t), for_loops(t), register_temps(t), compound_assigns(t), return_sources(t), assign_in_args(t), split_ternaries(t), default_last(t), push_tails(t), high_words(t), unmasked(t), no_default(t), unsigned_registers(t), empty_cases_first(t), register_slots(t), register_copies(t), lead_filler(t), scaled_places(t), shared_tails(t), guard_returns(t), default_after(t), frame_more(t), loop_tests(t), switch_breaks(t), register_reads(t), compare_results(t), or_loops(t), loop_guards(t), const_tests(t), flat_returns(t), return_cases_first(t), registers_last(t), guard_shared(t), guard_flat(t), _changed(common_tails, t), _changed(byte_heads, t), _changed(word_offsets, t), chain_tails(t), heads_variant(t), shared_exit(t), call_pairs(t), label_sinks(t), assign_loops(t), char_widens(t), _changed(tail_continues, t)] + case_guards(t) if v]
        both = pre_steps(t)
        both = pre_increments(both) if both else None
        if both:
            variants.append(both)
        for v in list(variants)[:4]:                        # (a value given twice: chained)
            c = chain_assigns(v)
            if c:
                variants.insert(variants.index(v) + 1, c)
        for v in list(variants):                            # (a pointer register variable through which words
            for w in pointer_widths(v):                     #  are read: int *, the compiler decides)
                variants.append(w)
        if re.search(r"& 0xff\)|\b[a-z] [<>=!]=? (?:3[2-9]|[4-9]\d|1[01]\d|12[0-6])\b", t):   # (a byte compared, or
            variants += char_param_variants(t)                                              #  a char: a char param)
        rule = prj.toolchain().p.get("frame_order")
        for v in list(variants):                            # (the names the frame's order asks for)
            fn = frame_names(v, rule) if rule else None
            variants.insert(variants.index(v) + 1, fn) if fn else None
            fl = frame_names(v, rule, True) if rule and REGSLOTS[0] else None   # (the register locals' slots last)
            variants.append(fl) if fl and fl != fn else None
        dsize = 0x10000                                     # (the data and the BSS after it: any place a pointer is compared with)
        for v in list(variants)[:2]:                        # (a number that is a place in the data)
            for name, av in address_variants(v, dsize):
                ARRAYS.setdefault(name, "char")
                variants.append(av)
                variants += pointer_widths(av)[:1]          # (and its pointer stepping words through it)
        loc = [localize(prj, word_constants(v), at, names) for v in variants]
        for src, left in loc[:3]:                           # (a global compared: unsigned, a variant)
            if not left and re.search(r"[<>=]=?\s*-?\d", src):
                loc += [(u, left) for u in unsigned_globals(src)]
        out.append(loc)
    # (the bases' readings taken in turn - each base's first, then each one's second, .. - so that no base's
    #  many variants keep another's first readings from the compiler)
    return [x for group in itertools.zip_longest(*out) for x in group if x is not None]


def byte_fields(inv, code):
    """{(pointer, offset)} of the fields the code reads or writes a byte of: the model's checker mode marks
    byte-wide memory as BYTE(x)"""
    if not hasattr(inv, "MARK"):
        return set()
    old = inv.MARK
    try:
        inv.MARK = True
        fs, params, stmts, pt, loc = inv.recover(code)
        t = inv.to_c("f", fs, params, stmts, pt, loc)
    except Exception:
        return set()
    finally:
        inv.MARK = old
    out = {(p, int(o)) for p, o in re.findall(r"BYTE\(\s*\(?(\w+)->f(\d+)\)?\s*\)", t)}
    out |= {(p, 0) for p in re.findall(r"BYTE\(\*\(?(?:\+\+|--)?(\w+)(?:\+\+|--)?\)?\)", t)}   # (*p a byte: a char *)
    from codebreaker.exact import x86                       # (and from the code itself: a byte read or written
    FRAMEW.clear()
    RANGES.clear()
    insns = list(x86.disasm(code, 0, len(code), False))
    for k in range(len(insns) - 4):                         # (a register loaded and tested against a range: a switch's)
        a_, b_, c_, d_ = insns[k + 1:k + 5]
        lo_ = "0" if a_.mnemonic == "or" and a_.op_str == "ax, ax" else a_.op_str[4:] if a_.mnemonic == "cmp" else None
        if insns[k].mnemonic == "mov" and insns[k].op_str.startswith("ax, ") and lo_ is not None and \
                c_.mnemonic == "cmp" and (a_.op_str.startswith("ax, ")) and c_.op_str.startswith("ax, ") and \
                b_.mnemonic in ("jl", "jb") and d_.mnemonic in ("jg", "ja", "jle", "jbe"):
            try:                                            # (jg/ja to the default: the cases' code first)
                RANGES.add((int(lo_, 0), int(c_.op_str[4:], 0), d_.mnemonic in ("jg", "ja")))
            except ValueError:
                pass
    for k, i in enumerate(insns):                           # (a slot's byte loaded and widened with zeros: unsigned)
        z = re.fullmatch(r"al, byte ptr \[bp ([+-]) (0x[0-9a-f]+|\d+)\]", i.op_str) if i.mnemonic == "mov" else None
        if z and any(x.op_str in ("ah, ah", "ah, 0") and x.mnemonic in ("sub", "xor", "mov") for x in insns[k + 1:k + 4]):
            FRAMEW.add(((1 if z.group(1) == "+" else -1) * int(z.group(2), 0), "zx"))
    for i in insns:         #  through a register at an offset - whatever the
        m = re.search(r"byte ptr \[(?:bx|si|di) \+ (0x[0-9a-f]+|\d+)\]", i.op_str)    # draft names the pointer)
        if m and int(m.group(1), 0) < 0x100:
            out.add(("*", int(m.group(1), 0)))
        elif m:
            out.add(("data", int(m.group(1), 0)))           # (a byte in the data indexed by a register)
        for wd, r_, o_ in re.findall(r"(byte|word) ptr \[(bx|si|di)(?: \+ (0x[0-9a-f]+|\d+))?\]", i.op_str):
            o_ = int(o_ or "0", 0)                          # (each width at each offset through each register:
            if o_ < 0x100:                                  #  an offset read both ways is two structs')
                out.add((r_ + ("*" if wd == "byte" else "w"), o_))
        g = re.search(r"byte ptr \[(0x[0-9a-f]+|\d+)\]", i.op_str)     # (a global read or written a byte at a time -
        if g and not any("word ptr [%s]" % g.group(1) in x.op_str for x in insns):   #  never a word: a word's low byte)
            out.add(("g", "g_%04x" % int(g.group(1), 0)))
        f = re.search(r"(byte|word) ptr \[bp ([+-]) (0x[0-9a-f]+|\d+)\]", i.op_str)   # (a frame slot by its width:
        if f:                                                                        #  FRAMEW: (+-N, width))
            FRAMEW.add(((1 if f.group(2) == "+" else -1) * int(f.group(3), 0), f.group(1)))
    return out


FRAMEW = set()                                              # (the frame slots the code reads or writes, by width)


def frame_chars(text, widths):
    """the locals and parameters the code only ever reads or writes a byte of (its frame slots, by width): chars"""
    slots = {}
    for off, w in FRAMEW:
        slots.setdefault(off, set()).add(w)
    chars = []
    for off, ws in slots.items():
        if ws - {"zx"} != {"byte"}:
            continue
        name = "v%d" % -off if off < 0 else chr(ord("a") + (off - 4) // 2) if off >= 4 and off % 2 == 0 else None
        if name:                                            # (widened with zeros where it is read: unsigned)
            chars.append((name, "unsigned char" if "zx" in ws else "char"))
    for x, ty in chars:
        text = re.sub(r"(?m)^([ \t]*)int %s;$" % x, r"\1%s %s;" % (ty, x), text)
        text = re.sub(r"(?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);$" % x, lambda m: "%sint %s;\n%s%s %s;" % (
            m.group(1), (m.group(2) + m.group(3).lstrip(", ")).strip(", "), m.group(1), ty, x), text)
        text = re.sub(r"(?m)^([ \t]*)char %s;$" % x, r"\1%s %s;" % (ty, x), text) if ty != "char" else text
    return text


def buffers(text):
    """the locals whose address the text takes (`&vN`) as char arrays, each as long as the frame has room before
    the next local (vN is at bp-N); a store into one goes through an int"""
    for n in sorted({int(x) for x in re.findall(r"\bbp - (\d+)\b", text)}):   # (the frame's place bp-N: a local there,
        if not re.search(r"(?m)^[ \t]*int v%d;" % n, text):                   #  in place of the filler that stood for it)
            f = list(re.finditer(r"(?m)^([ \t]*)char (_p\w*)\[(\d+)\];\n", text))
            if not f:
                continue
            text = text[:f[-1].start()] + "%sint v%d;\n" % (f[-1].group(1), n) + text[f[-1].end():]
        text = re.sub(r"\bbp - %d\b" % n, "&v%d" % n, text)
    locs = sorted({int(n) for n in re.findall(r"(?m)^[ \t]*int v(\d+);", text)})
    taken = [n for n in locs if re.search(r"&v%d\b" % n, text)]
    if not taken:
        return None
    for n in taken:
        lower = [m for m in locs if m < n]
        size = n - (max(lower) if lower else 0)
        text = re.sub(r"(?m)^([ \t]*)int v%d;" % n, r"\1char v%d[%d];" % (n, size), text)
        text = re.sub(r"&v%d\b" % n, "v%d" % n, text)
        text = re.sub(r"(?m)^([ \t]*)v%d = " % n, r"\1*(int *)v%d = " % n, text)
    return text


def long_returns(text):
    """a function whose every return gives dx too (the high half: dx:ax) returns a long: `dx = vM; return vN;`
    (M = N - 2, the two halves of one local) is `return vN;` of a long vN; `dx = (0 - (E >> 15)); return E;` (the
    sign of E spread: cwd) and `dx = 0; return E;` the value widened - C does that itself"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    rets = list(re.finditer(r"(?m)^([ \t]*)return ([^;]+);$", body))
    if not rets or not all(re.search(r"(?m)^[ \t]*dx = [^;]+;\n\s*\Z", body[:r.start()]) for r in rets):
        return text
    longs = set()

    def one(m):
        ind, hi, lo = m.group(1), m.group(2), m.group(3)
        h, l = re.fullmatch(r"v(\d+)", hi), re.fullmatch(r"v(\d+)", lo)
        if h and l and int(h.group(1)) == int(l.group(1)) - 2:
            longs.add((lo, hi))
            return "%sreturn %s;" % (ind, lo)
        if hi == "(0 - (%s >> 15))" % lo or hi == "(0 - (%s >> 15))" % lo.strip("()") or hi == "0":
            return "%sreturn %s;" % (ind, lo)
        return m.group(0)
    body = re.sub(r"(?m)^([ \t]*)dx = ([^;\n]+);\n\s*return ([^;\n]+);$", one, body)
    for lo, hi in longs:
        body = re.sub(r"(?m)^([ \t]*)int %s;\n" % hi, "", body)
        body = re.sub(r"(?m)^([ \t]*)int %s;" % lo, r"\1long %s;" % lo, body)
    m = re.search(r"(?m)^(\w+\()", head)
    if m:
        head = head[:m.start()] + "long\n" + head[m.start():]
    return head + sep + body


def long_pairs(text):
    """two words the code takes as one long - `*(long *)&x`, `LONG(y, x)`, `(x | y)` tested - x the low word: a
    parameter and the next (a, b) or a local and the one above it (vN, vN-2): x declared long, y gone, the
    pair written x. A function whose code widens its value last (cwd: FPINFO longret) returns a long"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    pairs = set(re.findall(r"\*\(long \*\)&(\w+)\b(?!->|\[)", body))   # (a variable's own two words - not a field's)
    for hi, lo in re.findall(r"\bLONG\((\w+), (\w+)\)", body):
        pairs.add(lo)
    hi0 = lambda x: chr(ord(x) + 1) if re.fullmatch(r"[a-y]", x) else "v%d" % (int(x[1:]) - 2) if re.fullmatch(r"v\d+", x) else None
    for x, y in re.findall(r"\((\w+) \| (\w+)\)", body):
        if hi0(x) == y:                                     # (a pair's two words or-ed: tested as one long)
            pairs.add(x)
    hi_of = lambda x: chr(ord(x) + 1) if re.fullmatch(r"[a-y]", x) else "v%d" % (int(x[1:]) - 2) if re.fullmatch(r"v\d+", x) else \
        "g_%x" % (int(x[2:], 16) + 2) if re.fullmatch(r"g_[0-9a-f]+", x) else None   # (a word of the data: the one after)

    def lupdate(m):                                         # (a long field updated: t = LONG(hi, lo) op E; lo = LO(t);
        ind, t, p, nh, lo, nl, op, e = m.groups()          #  hi = HI(t) - one assignment, op=)
        nl = 0 if lo.startswith("*") else int(nl)
        if int(nh) != nl + 2:
            return m.group(0)
        LONGINFO.setdefault("fields", set()).add(nl)
        return "%s*(long *)&%s->f%d %s= %s;" % (ind, p, nl, op, e)
    body = re.sub(r"(?m)^([ \t]*)(t\d+) = LONG\((\w+)->f(\d+), (\*\3|\3->f(\d+))\) ([-+&|^]) (.+);\n[ \t]*\5 = LO\(\2\);\n"
                  r"[ \t]*\3->f\4 = HI\(\2\);$", lupdate, body)

    def long_eq(m):                                         # (a pair compared with a number the compiler's way, the high
        h, l, op, k = m.group("h"), m.group("l"), m.group("op"), int(m.group("k"))   # word first: the long compared)
        hk = m.group("hk")
        if hi0(l) != h or k >= 0x8000 or (hk and int(hk) != 0):
            return m.group(0)
        pairs.add(l)
        return "(%s %s %d)" % (l, op, k)
    body = re.sub(r"\(\(?(?P<h>\w+)(?: != (?P<hk>0))?\)? \|\| \((?P<l>\w+) (?P<op>!=) (?P<k>\d+)\)\)", long_eq, body)
    body = re.sub(r"\(\((?P<h>\w+) == (?P<hk>0)\) && \((?P<l>\w+) (?P<op>==) (?P<k>\d+)\)\)", long_eq, body)
    body = re.sub(r"\(!(?P<h>\w+)(?P<hk>) && \((?P<l>\w+) (?P<op>==) (?P<k>\d+)\)\)", long_eq, body)
    # (a pair given a pair: `p->fN = x; p->fN+2 = y;` - a long field given a long - written as one store)
    def pair_store(m):
        ind, p, n, x, p2, n2, y = m.groups()
        byte = any(o in BYTEPTR for o in (("*", int(n)), ("*", int(n2)), (p, int(n)), (p, int(n2))))
        alone = [l for l in body.split("\n") if re.search(r"\b%s->f%s\b" % (p, n), l) and
                 not re.search(r"\b%s->f%s\b" % (p, n2), l) and not re.fullmatch(r"[ \t]*%s->f%s = %s;" % (p, n, x), l)]
        if alone and not any(re.search(r"\b%s->f%s\b" % (p, n2), l) for l in body.split("\n")
                             if not re.fullmatch(r"[ \t]*%s->f%s = %s;" % (p, n2, y), l)):
            return m.group(0)                               # (the low word read alone, the high never: two ints)
        if p2 == p and int(n2) == int(n) + 2 and hi_of(x) == y and not byte:   # (words both: a byte is no long's)
            pairs.add(x)
            LONGINFO.setdefault("fields", set()).add(int(n))
            return "%s%s->f%s = %s;" % (ind, p, n, x)
        return m.group(0)
    body = re.sub(r"(?m)^([ \t]*)\*(\w+) = (\w+);\n(\s*)\2->f2 = (\w+);$",           # (*p the first word: p->f0)
                  lambda m: "%s%s->f0 = %s;\n%s%s->f2 = %s;" % (m.group(1), m.group(2), m.group(3), m.group(4), m.group(2), m.group(5))
                  if hi_of(m.group(3)) == m.group(5) else m.group(0), body)
    body = _line_pairs(body, r"([ \t]*)(\w+)->f(\d+) = (\w+);\n\s*(\w+)->f(\d+) = (\w+);", pair_store)

    def call_store(m):                                      # (a routine's long value, dx:ax, kept in a pair:
        ind, lo, call, hi = m.groups()[:4]                      #  the routine returns a long)
        if hi_of(lo) != hi:
            return m.group(0)
        pairs.add(lo)
        LONGINFO.setdefault("funcs", set()).add(re.match(r"\w+", call).group(0))
        nxt = m.group(5) or ""
        return "%s%s = %s;%s" % (ind, lo, call, nxt.replace("(ax | dx)", lo))   # (the pair tested at once: the long)
    def call_args(m):                                       # (a long passed: its two words pushed - the pair, or
        args = [x.strip() for x in _top_commas(m.group(2))] if m.group(2).strip() else []   # an int widened)
        out, i = [], 0
        while i < len(args):
            x = args[i]
            y = args[i + 1] if i + 1 < len(args) else None
            if y is not None and re.fullmatch(r"\w+", x) and y == hi_of(x) and x in pairs:   # (a long the code
                out.append(x)                                                            #  shows elsewhere)
                i += 2
                continue
            if y is not None and y in ("(0 - (%s >> 15))" % x, "(0 - ((%s) >> 15))" % x):
                out.append("(long)%s" % x if re.fullmatch(r"\w+", x) else "(long)(%s)" % x)
                i += 2
                continue
            out.append(x)
            i += 1
        return "%s(%s)" % (m.group(1), ", ".join(out))

    bl = body.split("\n")                                   # (the model's temporaries holding an int and its sign
    for k in range(len(bl)):                                #  spread, read once by a call a line or two on: in their
        m = re.fullmatch(r"[ \t]*(t\d+) = (\(0 - \((\w+) >> 15\)\)|(\w+));", bl[k] or "") or \
            re.fullmatch(r"[ \t]*(bx|cx|dx|di) = (\(0 - \((\w+) >> 15\)\))()();", bl[k] or "")   # places - the call
        if not m:                                                                        #  passes a long; the
            continue                                                                     #  sign in a register too)
        t, e, x = m.group(1), m.group(2), m.group(3) or m.group(4)
        uses = [j for j in range(k + 1, min(k + 4, len(bl))) if bl[j] is not None and re.search(r"\b%s\b" % t, bl[j])]
        if len(uses) != 1 or any(re.search(r"\b%s\b" % t, bl[j] or "") for j in range(uses[0] + 1, len(bl))) or \
                len(re.findall(r"\b%s\b" % t, bl[uses[0]])) != 1 or not re.search(r"\w\(", bl[uses[0]]) or \
                any(re.match(r"[ \t]*%s = " % x, bl[j] or "") for j in range(k + 1, uses[0])) or \
                any("{" in (bl[j] or "") or "}" in (bl[j] or "") for j in range(k + 1, uses[0] + 1)):   # (one block)
            continue
        bl[uses[0]] = re.sub(r"\b%s\b" % t, lambda _: e, bl[uses[0]])
        bl[k] = None
    body = "\n".join(x for x in bl if x is not None)
    for t in set(re.findall(r"(?m)^[ \t]*int ((?:t\d+, )*t\d+);$", body)):
        for t1 in t.split(", "):
            if not re.search(r"\b%s\b" % t1, body.split("int " + t, 1)[-1].split(";", 1)[-1]):
                body = _drop_decl(body, t1)
    body = re.sub(r"\bLONG\(\(0 - \((\w+) >> 15\)\), \1\)", r"(long)\1", body)   # (an int's sign spread: widened)
    body = re.sub(r"\bLONG\(0, (\w+)\)", r"(long)(unsigned)\1", body)
    body = re.sub(r"\bLONG\((\w+(?:->f\d+)*)->f(\d+), \1->f(\d+)\)", lambda m: "%s->f%s" % (m.group(1), m.group(3))
                  if int(m.group(2)) == int(m.group(3)) + 2 and LONGINFO.setdefault("fields", set()).add(int(m.group(3))) is None
                  else m.group(0), body)                    # (a long field's two words: the field)
    if LONGINFO.get("ret"):                                 # (a long function's value its long's low word: the long)
        body = re.sub(r"\bLO\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)", r"(\1)", body)

    def pair_copy(m):                                       # (a pair given a pair: x = y, the high words too)
        ind, x, y, hx, hy = m.groups()
        ghi = lambda g: "g_%x" % (int(g[2:], 16) + 2) if re.fullmatch(r"g_[0-9a-f]+", g) else None
        gb = lambda g: re.fullmatch(r"g_[0-9a-f]+", g) and "g_%04x" % int(g[2:], 16) in {"g_%04x" % int(b_[2:], 16) for b_ in BYTEG if re.fullmatch(r"g_[0-9a-f]+", b_)}
        if gb(x) or gb(hx) or gb(y) or gb(hy):
            return m.group(0)                               # (bytes of the data given: no long's words)
        if hi_of(x) == hx and hi_of(y) == hy:
            pairs.add(x)
            pairs.add(y)
            return "%s%s = %s;" % (ind, x, y)
        if (hi_of(x) == hx or ghi(x) and int(hx[2:] if hx.startswith("g_") else "0", 16) == int(x[2:], 16) + 2) and \
                re.fullmatch(r"g_[0-9a-f]+", y) and re.fullmatch(r"g_[0-9a-f]+", hy) and int(hy[2:], 16) == int(y[2:], 16) + 2:
            pairs.add(x)                                    # (a long in the data copied: one long)
            LONGINFO.setdefault("globals", set()).add("g_%04x" % int(y[2:], 16))
            if x.startswith("g_"):
                LONGINFO["globals"].add("g_%04x" % int(x[2:], 16))
            return "%s%s = %s;" % (ind, x, y)
        return m.group(0)
    body = _line_pairs(body, r"([ \t]*)(\w+) = (\w+);\n\s*(\w+) = (\w+);", pair_copy)

    def const_pair(m):                                      # (a pair given two numbers: one long number)
        ind, x, k1, hx, k2 = m.groups()
        if hi_of(x) != hx or x.startswith("g_"):            # (two words of the data given numbers: two ints)
            return m.group(0)
        pairs.add(x)
        v = (int(k2) & 0xFFFF) << 16 | (int(k1) & 0xFFFF)
        v = v - (1 << 32) if v & 0x80000000 else v
        return "%s%s = %s;" % (ind, x, "%dL" % v if abs(v) > 0x7FFF else str(v))
    body = _line_pairs(body, r"([ \t]*)(\w+) = (-?\d+);\n\s*(\w+) = (-?\d+);", const_pair)

    def long_of(lo, hi):                                    # (two words' expressions, the low and the high, as the
        lo, hi = _strip(lo.strip()), _strip(hi.strip())     #  one long expression they are, or None: names, fields
        def bare(x):                                        #  and data in pairs, ~, & | ^, a number masking a half)
            return x[1:-1] if x.startswith("(") and x.endswith(")") and _balanced(x[1:-1]) else x
        lo, hi = bare(lo), bare(hi)
        fl, fh = re.fullmatch(r"(\w+)->f(\d+)", lo), re.fullmatch(r"(\w+)->f(\d+)", hi)
        if fl and fh and fl.group(1) == fh.group(1) and int(fh.group(2)) == int(fl.group(2)) + 2:
            LONGINFO.setdefault("fields", set()).add(int(fl.group(2)))
            return lo
        if re.fullmatch(r"g_[0-9a-f]+", lo) and re.fullmatch(r"g_[0-9a-f]+", hi) and int(hi[2:], 16) == int(lo[2:], 16) + 2:
            LONGINFO.setdefault("globals", set()).add("g_%04x" % int(lo[2:], 16))
            return lo
        if re.fullmatch(r"\w+", lo) and hi_of(lo) == hi:
            pairs.add(lo)
            return lo
        if lo.startswith("~") and hi.startswith("~"):
            x = long_of(lo[1:], hi[1:])
            return "(~%s)" % x if x else None
        def flat(x, op):                                    # (a & (b & c): a, b, c)
            out = []
            for p_ in _top_split(x, op):
                p_ = p_.strip()
                b_ = bare(p_)
                out += flat(b_, op) if b_ != p_ and len(_top_split(b_, op)) > 1 else [p_]
            return out
        for op in ("&", "|", "^"):
            pl, ph = flat(lo, op), flat(hi, op)
            if len(ph) > 1 and len(ph) == len(pl) + 1 and re.fullmatch(r"\d+|0x[0-9a-f]+", ph[-1].strip()) and op == "&":
                x = long_of(" & ".join(pl) if len(pl) > 1 else pl[0], " & ".join(ph[:-1]) if len(ph) > 2 else ph[0])
                k = int(ph[-1].strip(), 0) & 0xFFFF             # (the high word masked alone: the low word whole)
                return "(%s & 0x%04xffffL)" % (x, k) if x else None
            if len(pl) > 1 and len(pl) == len(ph):
                xs = [long_of(a, b) for a, b in zip(pl, ph)]
                return "(%s)" % (" %s " % op).join(xs) if all(xs) else None
        return None

    def long_test(m):                                       # ((lo) | (hi)) != 0: a long tested
        inner = m.group(1)[1:-1]
        parts = _top_split(inner, "|") if _balanced(inner) else []
        if len(parts) != 2 or not all(_balanced(p) for p in parts):
            return m.group(0)
        x = long_of(parts[0], parts[1])
        return "(%s %s 0)" % (x, m.group(2)) if x else m.group(0)
    body = re.sub(r"\((\((?:[^()]|\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\))*\)) (!=|==) 0\)", long_test, body)

    def field_pair(m):                                      # (a long field given a long a word at a time - another
        ind, p, n, x, p2, n2, y = m.groups()                #  long field's, a long in the data, ..)
        if p2 != p or int(n2) != int(n) + 2 or re.fullmatch(r"[a-z]|v\d+", x.strip()):
            return m.group(0)                               # (two variables: pair_store's, with its guards)
        lx = long_of(x, y)
        if not lx:
            return m.group(0)
        LONGINFO.setdefault("fields", set()).add(int(n))
        return "%s%s->f%s = %s;" % (ind, p, n, lx)
    body = _line_pairs(body, r"([ \t]*)(\w+(?:->f\d+)*)->f(\d+) = ([^;\n]+);\n\s*(\w+(?:->f\d+)*)->f(\d+) = ([^;\n]+);", field_pair)

    def lfield(p, n):                                       # (a long field: offset n of what p points to)
        LONGINFO.setdefault("fields", set()).add(int(n))
        return "%s->f%s" % (p, n)
    body = re.sub(r"(?m)^([ \t]*)(\w+)->f(\d+) = \(~\2->f\3\);\n[ \t]*\2->f(\d+) = \(~\2->f\4\);$",
                  lambda m: "%s%s = ~%s;" % (m.group(1), lfield(m.group(2), m.group(3)), lfield(m.group(2), m.group(3)))
                  if int(m.group(4)) == int(m.group(3)) + 2 else m.group(0), body)        # (both words complemented)
    body = re.sub(r"(?m)^([ \t]*)(\w+)->f(\d+) = (\w+);\n[ \t]*\2->f(\d+) = \(0 - \(\4 >> 15\)\);$",
                  lambda m: "%s%s = %s;" % (m.group(1), lfield(m.group(2), m.group(3)), m.group(4))
                  if int(m.group(5)) == int(m.group(3)) + 2 else m.group(0), body)        # (an int widened into it)
    body = re.sub(r"\((\w+(?:->f\d+)*)->f(\d+) \| \1->f(\d+)\)", lambda m: lfield(m.group(1), m.group(2))
                  if int(m.group(3)) == int(m.group(2)) + 2 else m.group(0), body)        # (its words tested together)
    body = re.sub(r"\*\(long \*\)&(\w+(?:->f\d+)*)->f(\d+)\b", lambda m: lfield(m.group(1), m.group(2)), body)   # (read
                                                                                                             #  whole)

    def field_args(m):                                      # (a long field's two words pushed together: the field)
        args = [x.strip() for x in _top_commas(m.group(2))] if m.group(2).strip() else []
        out, i = [], 0
        while i < len(args):
            a1 = re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", args[i])
            a2 = re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", args[i + 1]) if i + 1 < len(args) else None
            together = a1 and i + 1 < len(args) and len(re.findall(r"%s\b(?!->)" % re.escape(args[i + 1]), body)) == \
                len(re.findall(r"%s, %s\b" % (re.escape(args[i]), re.escape(args[i + 1])), body))   # (the high word never alone)
            if a1 and a2 and a1.group(1) == a2.group(1) and int(a2.group(2)) == int(a1.group(2)) + 2 and together and \
                    (a1.group(1), int(a1.group(2))) not in BYTEPTR and int(a1.group(2)) in LONGINFO.get("fields", ()):
                out.append(lfield(a1.group(1), a1.group(2)))
                i += 2
                continue
            out.append(args[i])
            i += 1
        return "%s(%s)" % (m.group(1), ", ".join(out))
    body = re.sub(r"\b((?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)", field_args, body)

    def masked_test(m):                                     # (a long and-ed with a long field and tested: the high
        ind, hi, p, n2, rest, lo, n1 = m.groups()           #  words in dx first, then the low words or-ed in)
        if hi_of(lo) != hi or int(n2) != int(n1) + 2:
            return m.group(0)
        pairs.add(lo)
        LONGINFO.setdefault("fields", set()).add(int(n1))
        return rest.replace("((%s & %s->f%s) | dx)" % (lo, p, n1), "(%s & %s->f%s)" % (lo, p, n1))
    body = re.sub(r"(?m)^([ \t]*)dx = (\w+) & (\w+)->f(\d+);\n(.*\(\((\w+) & \3->f(\d+)\) \| dx\).*)$", masked_test, body)

    def gpair_op(m):                                        # (a long in the data and-ed, or-ed, xor-ed with a long:
        ind, g, op, x, g2, op2, x2 = m.groups()             #  a word at a time, the high words after - one op)
        if op != op2 or int(g2[2:], 16) != int(g[2:], 16) + 2:
            return m.group(0)
        lx = long_of(x, x2)
        if not lx:
            return m.group(0)
        LONGINFO.setdefault("globals", set()).add("g_%04x" % int(g[2:], 16))
        return "%s%s = %s %s %s;" % (ind, g, g, op, lx)
    body = _line_pairs(body, r"([ \t]*)(g_[0-9a-f]+) = \2 ([|&^]) ([^;\n]+);\n\s*(g_[0-9a-f]+) = \5 ([|&^]) ([^;\n]+);", gpair_op)

    def sign_pair(m):                                       # (a pair given an int and its sign spread: the int
        ind, x, e, hx, e2 = m.groups()                      #  widened to the long)
        if hi_of(x) != hx or e2.strip() not in (e, "(%s)" % e):
            return m.group(0)
        pairs.add(x)
        if re.fullmatch(r"[A-Za-z_]\w*", e):
            widened[x] = e
        return "%s%s = %s;" % (ind, x, e)
    widened = {}                                            # (a long given an int: its low word that int)
    body = re.sub(r"(?m)^([ \t]*)(\w+) = ([^;\n]+);\n[ \t]*(\w+) = \(0 - \((.+) >> 15\)\);$", sign_pair, body)
    for x, e in widened.items():                            # (its one word tested - the int's, still in a register)
        if not re.search(r"(?m)^[ \t]*(?:%s|%s) = " % (x, e), body.split("%s = %s;" % (x, e), 1)[-1]):
            body = re.sub(r"\(%s (==|!=) 0\)" % x, r"(%s \1 0)" % e, body)

    def zero_pair(m):                                       # (both halves given 0, or -1: the long 0, -1)
        ind, a, v, b = m.groups()
        lo, hi = (b, a) if hi_of(b) == a else (a, b) if hi_of(a) == b else (None, None)
        if not lo or lo.startswith("g_"):
            return m.group(0)
        pairs.add(lo)
        return "%s%s = %s;" % (ind, lo, v)
    body = re.sub(r"(?m)^([ \t]*)(\w+) = (0|-1);\n\s*(\w+) = \2;$", zero_pair, body)

    def call_long(m):                                       # (a routine's dx:ax read as one long by the next
        LONGINFO.setdefault("funcs", set()).add(re.match(r"\w+", m.group(2)).group(0))   # statement)
        return m.group(3).replace("LONG(dx, ax)", m.group(2))
    body = re.sub(r"(?m)^([ \t]*)ax = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n(.*\bLONG\(dx, ax\).*)$", call_long, body)

    def halves(m):                                          # (a long given to a pair a half at a time: one store)
        ind, t, e, lo, hi = m.groups()
        if hi_of(lo) != hi:
            return m.group(0)
        pairs.add(lo)
        return "%s%s = %s;" % (ind, lo, e)
    body = re.sub(r"(?m)^([ \t]*)(t\d+) = (.+);\n\s*(\w+) = LO\(\2\);\n\s*(\w+) = HI\(\2\);$", halves, body)
    body = re.sub(r"(?m)^([ \t]*)dx = HI\((.+)\);\n[ \t]*(\w+) = LO\(\2\);\n[ \t]*(\w+) = dx;$",   # (the high half
                  lambda m: "%s%s = %s;" % (m.group(1), m.group(3), m.group(2)) if hi_of(m.group(3)) == m.group(4) and
                  pairs.add(m.group(3)) is None else m.group(0), body)                              #  kept in dx first)
    body = re.sub(r"(?m)^([ \t]*)(\w+(?:->f\d+)*)->f(\d+) = LO\((.+)\);\n[ \t]*\2->f(\d+) = HI\(\4\);$",
                  lambda m: "%s%s = %s;" % (m.group(1), lfield(m.group(2), m.group(3)), m.group(4))
                  if int(m.group(5)) == int(m.group(3)) + 2 else m.group(0), body)   # (a long field given a long)
    body = re.sub(r"(?m)^([ \t]*)()(\w+) = LO\((.+)\);\n\s*(\w+) = HI\(\4\);$",
                  lambda m: halves(type("M", (), {"groups": lambda s: (m.group(1), "", m.group(4), m.group(3), m.group(5)),
                                                  "group": lambda s, i=0: m.group(0)})()), body)
    body = re.sub(r"(?m)^([ \t]*)ax = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n\s*(\w+) = ax;\n(?=\s*\w+ = dx;)",
                  lambda m: "%s%s = %s;\n" % (m.group(1), m.group(3), m.group(2)), body)   # (through ax: the same -
                                                                                           #  when dx goes with it)
    body = re.sub(r"(?m)^([ \t]*)(\w+) = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n\s*(\w+) = dx;(\n.*)?$", call_store, body)
    for g in [x for x in pairs if re.fullmatch(r"g_[0-9a-f]+", x)]:   # (data a pair is made of: a long there)
        LONGINFO.setdefault("globals", set()).add("g_%04x" % int(g[2:], 16))

    def data_args(m):                                       # (two words of the data pushed together, the high one
        args = [x.strip() for x in _top_commas(m.group(2))] if m.group(2).strip() else []   #  first: a long there -
        out, i = [], 0                                                                     #  the same pushes)
        while i < len(args):
            if i + 1 < len(args) and re.fullmatch(r"g_[0-9a-f]+", args[i]) and args[i + 1] == hi_of(args[i]):
                LONGINFO.setdefault("globals", set()).add("g_%04x" % int(args[i][2:], 16))
                out.append(args[i])
                i += 2
                continue
            out.append(args[i])
            i += 1
        return "%s(%s)" % (m.group(1), ", ".join(out))
    body = re.sub(r"\b((?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)", call_args, body)
    body = re.sub(r"\b((?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)", data_args, body)
    params = re.findall(r"(?m)^\w+\(([\w, ]*)\)", head)
    plist = [p.strip() for p in (params[-1].split(",") if params else []) if p.strip()]
    done = []
    for x in re.findall(r"(?m)^[ \t]*(?:unsigned )?long (v\d+);$", body):   # (a long a pass made, its high word
        if hi_of(x) and re.search(r"\b%s\b" % hi_of(x), body.split("{", 1)[-1]):   #  still read by name: the pair)
            pairs.add(x)
    for lo in sorted(pairs):
        hi = hi_of(lo)
        if not hi:
            continue
        isparam = lo in plist and hi in plist
        islocal = re.search(r"(?m)^[ \t]*(?:int|long) (?:\w+, )*%s\b" % lo, body) and (
            re.search(r"(?m)^[ \t]*int (?:\w+, )*%s\b" % hi, body) or re.search(r"(?m)^[ \t]*long %s;" % lo, body))
        if not isparam and not islocal:
            continue
        uns = "unsigned long" if lo in LONGINFO.get("unsigned", ()) else "long"
        body = re.sub(r"\*\(long \*\)&%s\b" % lo, lo, body)
        body = re.sub(r"\bLONG\(%s, %s\)" % (hi, lo), lo, body)
        body = re.sub(r"\(%s \| %s\)|\(%s \| %s\)" % (lo, hi, hi, lo), lo, body)
        body = re.sub(r"\(%s (<|>=) 0\)" % hi, r"(%s \1 0)" % lo, body)   # (the high word's sign: the long's)
        for lo2 in [x for x in sorted(pairs) if x != lo and hi_of(x)]:   # (two longs compared a half at a time)
            hi2 = hi_of(lo2)
            body = body.replace("(%s != %s) || (%s != %s)" % (hi, hi2, lo, lo2), "(%s != %s)" % (lo, lo2))
            body = body.replace("(%s == %s) && (%s == %s)" % (hi, hi2, lo, lo2), "(%s == %s)" % (lo, lo2))
            body = body.replace("((%s & %s) == %s) && ((%s & %s) == %s)" % (hi, hi2, hi2, lo, lo2, lo2), "((%s & %s) == %s)" % (lo, lo2, lo2))
            # (one masked with the other and compared with it: dx the high words' and)
            body = re.sub(r"(?m)^([ \t]*)dx = %s & %s;\n([ \t]*if \(.*)\(dx == %s\) && \(\(%s & %s\) == %s\)" %
                          (hi, hi2, hi2, lo, lo2, lo2), r"\2((%s & %s) == %s)" % (lo, lo2, lo2), body)
        body = re.sub(r"(?m)^([ \t]*)%s = (\w+(?:->f\d+)*)->f(\d+);\n[ \t]*%s = \2->f(\d+);$" % (lo, hi),
                      lambda m: "%s%s = *(long *)&%s->f%s;" % (m.group(1), lo, m.group(2), m.group(3))
                      if int(m.group(4)) == int(m.group(3)) + 2 else m.group(0), body)   # (a long field read)
        body = re.sub(r"\(%s & (-?\d+)\)" % hi, lambda m: "(%s & 0x%xL)" % (lo, (int(m.group(1)) & 0xFFFF) << 16), body)
        if re.search(r"\b%s\b" % hi, body.split("\n", 1)[-1] if islocal else body):
            pass                                            # (the high word read alone: kept as it is)
        if isparam:
            plist = [p for p in plist if p != hi]
            head = re.sub(r"(?m)^(\w+)\([\w, ]*\)", lambda m: "%s(%s)" % (m.group(1), ", ".join(plist)), head, count=1)
            head = _drop_decl(head, hi)
            head = _redeclare(head, lo, "%s %s" % (uns, lo)) or head
        else:
            body = _drop_decl(body, hi)
            l2 = _redeclare(body, lo, "%s %s" % (uns, lo))
            body = l2 or body
        done.append(lo)
    longs_ = set(re.findall(r"(?m)^[ \t]*(?:register )?(?:unsigned )?long (\w+);", body))

    def lstore(m):                                          # (a long value - one of a long's - stored a word at a time
        ind, e, p, n, op, n2 = m.group(1), m.group(2), m.group(3), int(m.group(4)), m.group(5) or "", int(m.group(6))
        if n2 != n + 2 or not any(re.search(r"\b%s\b" % x, e) for x in longs_):   #  into a field, dx the high one)
            return m.group(0)
        LONGINFO.setdefault("fields", set()).add(n)
        return "%s*(long *)&%s->f%d = %s%s;" % (ind, p, n, op, e if not op or e.startswith("(") else "(%s)" % e)
    if longs_:
        body = re.sub(r"(?m)^([ \t]*)ax = (.+);\n[ \t]*(\w+(?:->f\d+)*)->f(\d+) = \(?(~?)ax\)?;\n[ \t]*\3->f(\d+) = \(?~?dx\)?;$", lstore, body)
        body = re.sub(r"(?m)^([ \t]*)(\w+(?:->f\d+)*)->f(\d+) = (\(.+\));\n[ \t]*\2->f(\d+) = dx;$",
                      lambda m: lstore(type("M", (), {"group": lambda s_, i: {0: m.group(0), 1: m.group(1), 2: m.group(4), 3: m.group(2),
                                                                         4: m.group(3), 5: "", 6: m.group(5)}[i]})()), body)
    lrets = [x for x in re.findall(r"(?m)^[ \t]*return (\w+);$", body)
             if re.search(r"(?m)^[ \t]*(?:unsigned )?long (?:\w+, )*%s\b" % x, body)]
    if lrets and LONGINFO.get("ret") is not False and not re.search(r"(?m)^[ \t]*return [^;]*\w\(", body):
        LONGINFO["ret"] = True                              # (a long local returned: the function's a long)
    if LONGINFO.get("ret"):
        m = re.search(r"(?m)^\w+\(", head)
        if m and not re.search(r"(?m)^(?:unsigned )?long\s*$", head):
            head = head[:m.start()] + "long\n" + head[m.start():]
        for n in re.findall(r"(?m)^[ \t]*return \w+(?:->f\d+)*->f(\d+);$", body):   # (a field returned whole: a long one)
            LONGINFO.setdefault("fields", set()).add(int(n))
    return head + sep + body


def _line_pairs(text, rx, fn):
    """text with each two lines in a row that rx (over both) matches as fn makes them - tried at every line,
    a pair that does not change left for the next (no overlapping match lost)"""
    lines = text.split("\n")
    out = []
    k = 0
    while k < len(lines):
        if k + 1 < len(lines):
            two = lines[k] + "\n" + lines[k + 1]
            m = re.fullmatch(rx, two)
            if m:
                new = fn(m)
                if new != two:
                    out.append(new)
                    k += 2
                    continue
        out.append(lines[k])
        k += 1
    return "\n".join(out)


def long_ladders(text):
    """two longs compared the compiler's way - the high words, then the low ones - left as ifs of the halves:
        if ((H <= H2)) { if ((H < H2) || (L < L2)) { A } else { L_x: B } } else { goto L_x; }  ->  if ((L < L2)) A else B
    (L, L2 longs - declared so - H, H2 their high words; the same for >= and >)"""
    longs = set(re.findall(r"(?m)^[ \t]*(?:register )?(?:unsigned )?long (\w+);", text))
    if not longs:
        return text
    hi = lambda x: chr(ord(x) + 1) if re.fullmatch(r"[a-y]", x) else "v%d" % (int(x[1:]) - 2) if re.fullmatch(r"v\d+", x) else None
    lines = text.split("\n")
    for k in range(len(lines)):
        p = _if_parts(lines, k)
        if not p or p[2] is None:
            continue
        m = re.fullmatch(r"\((\w+) ([<>])= (\w+)\)", _strip("(" + p[0] + ")"))
        q = _if_parts(p[1], 0) if p[1] else None
        if not m or not q or q[3] != len(p[1]) or q[2] is None:
            continue
        n = re.fullmatch(r"\(?\((\w+) ([<>]) (\w+)\) \|\| \((\w+) ([<>]=?) (\w+)\)\)?", q[0])
        if not n or (n.group(1), n.group(2), n.group(3)) != (m.group(1), m.group(2), m.group(3)):
            continue
        L, L2 = n.group(4), n.group(6)
        if L not in longs or hi(L) != m.group(1) or (L2 in longs and hi(L2) != m.group(3)) or n.group(5)[0] != m.group(2):
            continue
        dd = lambda ls: [l[4:] if l.startswith("    ") else l for l in ls]
        if not _same_arm(p[2], dd(q[2])):
            continue
        ind = re.match(r"\s*", lines[k]).group(0)
        new = ["%sif ((%s %s %s)) {" % (ind, L, n.group(5), L2)] + dd(q[1]) + [ind + "} else {"] + dd(q[2]) + [ind + "}"]
        cand = lines[:k] + new + lines[p[3]:]
        whole = "\n".join(cand)
        cand = [l for l in cand if not (re.fullmatch(r"\s*L_\w+:", l) and not re.search(r"\bgoto %s;" % l.strip()[:-1], whole))]
        return long_ladders("\n".join(cand))
    return text


def local_pointers(text):
    """a register given the place of a local (`r = bp - N`, the model's lea of [bp-N]): a char pointer into it,
    `r = (char *)&vN;`, its fields the bytes it reaches (`r->fK` is r[K])"""
    text = re.sub(r"(?m)^([ \t]*)(\w+) = bp;\n\s*\2 = \2 - (\d+);$", r"\1\2 = bp - \3;", text)   # (mov r,bp; sub r,N)
    for reg, n in re.findall(r"(?m)^[ \t]*(\w+) = bp - (\d+);", text):
        buf = next(((k_, c_) for k_, c_ in re.findall(r"(?m)^[ \t]*char _p(\d+)\[(\d+)\];", text) if int(k_) + int(c_) == int(n)), None)
        if buf:                                             # (the start of a filler the frame keeps: a buffer there)
            text = re.sub(r"(?m)^([ \t]*)char _p%s\[%s\];" % buf, r"\1char v%s[%s];" % (n, buf[1]), text)
            text = re.sub(r"(?m)^([ \t]*)%s = bp - %s;" % (reg, n), r"\1%s = v%s;" % (reg, n), text)
            text = re.sub(r"(?m)^([ \t]*)(register )?int %s;" % reg, r"\1\2char *%s;" % reg, text)
            continue
        if not re.search(r"(?m)^[ \t]*(?:int|long) v%s;" % n, text):
            continue
        text = re.sub(r"(?m)^([ \t]*)%s = bp - %s;" % (reg, n), r"\1%s = (char *)&v%s;" % (reg, n), text)
        text = re.sub(r"\b%s->f(\d+)\b" % reg, r"%s[\1]" % reg, text)
        text = re.sub(r"(?m)^([ \t]*)(register )?int %s;" % reg, r"\1\2char *%s;" % reg, text)
    return text


def void_variant(text):
    """the draft without its last `return X;` (a function of no value whose last call's result the model kept)"""
    m = list(re.finditer(r"(?m)^([ \t]*)return (.+);\s*$", text))
    if not m:
        return None
    x = m[-1]
    if not re.match(r"\s*\}\s*$", text[x.end():]) and len(x.group(1)) > 4:
        return None                                         # (a return inside the body, not its end: a value)
    return text[:x.start()] + ("%s%s;" % (x.group(1), x.group(2)) if "(" in x.group(2) else "") + text[x.end():]


def register_variables(text, regs=("si", "di"), regparam=None):
    """the registers a compiler keeps its register variables in (MS C 3.00: SI, then DI), made those variables:
    declared in that order, pointers where the text goes through them; a register a parameter is loaded into
    at the start (regparam {register: parameter index}) is that parameter, declared register"""
    head, sep, body = split_body(text)
    if not sep:
        return text
    rdecl = []                                              # (declared in the order of their registers: the
    for r, ix in sorted((regparam or {}).items(), key=lambda kv: list(regs).index(kv[0]) if kv[0] in regs else 9):
        p = chr(ord("a") + ix)                              #  compiler gives the first declared the first)
        body = re.sub(r"\b%s\b" % r, p, body)
        ptr = re.search(r"\*\s*\(?\s*(?:\+\+|--)?\s*%s\b|\b%s\s*(?:\+\+|--)?\s*\[" % (p, p), body)
        lines = head.split("\n")
        for j, l in enumerate(lines):                       # (out of the plain declaration of the parameters)
            m = re.match(r"(\s*)int (.*);$", l)
            if m and re.search(r"\b%s\b" % p, m.group(2)):
                rest = [x.strip() for x in m.group(2).split(",") if x.strip() != p]
                lines[j] = m.group(1) + "int " + ", ".join(rest) + ";" if rest else None
        head = "\n".join(l for l in lines if l is not None)
        pp = re.search(r"\*\(\*%s\)|\*\*%s\b" % (p, p), body) and not re.search(r"\b%s->" % p, body)   # (read through
                                                                                                    #  twice: a pointer to pointers)
        rdecl.append("register %s%s;" % ("char **" if pp else "char *" if ptr else "int ", p))
    if rdecl:
        head = head.rstrip("\n") + "\n" + "\n".join(rdecl) + "\n"
        sig = re.search(r"(?m)^(\w+)\(([\w, ]*)\)\s*$", head)    # (the parameters in the signature, all of them)
        if sig:
            have = [x.strip() for x in sig.group(2).split(",") if x.strip()]
            want = sorted(set(have) | {chr(ord("a") + ix) for ix in (regparam or {}).values()})
            if want != have:
                head = head[:sig.start()] + "%s(%s)" % (sig.group(1), ", ".join(want)) + head[sig.end():]
    used = [r for r in regs if re.search(r"\b%s\b" % r, body)]
    if not used:
        return head + sep + body
    decls = []
    pnames = set(re.findall(r"(?m)^[ \t]*(?:register\s+)?char\s*\*\s*(\w+);", head))     # (pointer parameters)
    for r in used:
        ptr = re.search(r"\*\s*%s\b|\b%s\s*\[" % (r, r), body) or \
            any(re.search(r"(?m)^[ \t]*%s = %s;" % (r, p), body) for p in pnames)       # (given a pointer's value)
        pp = re.search(r"\*\(\*%s\)|\*\*%s\b" % (r, r), body) and not re.search(r"\b%s->" % r, body)
        decls.append("    register %s%s;" % ("char **" if pp else "char *" if ptr else "int ", "r_" + r))
        body = re.sub(r"\b%s\b" % r, "r_" + r, body)
    return head + "{\n" + "\n".join(decls) + body


def register_slots(text):
    """the model's trailing frame filler less two bytes for each register local (a variant: MS C 3.00 gives some
    register locals a slot in the frame - which ones the compiler alone says)"""
    if not REGSLOTS[0]:
        return None
    n_regs = len(re.findall(r"(?m)^[ \t]*register [^;(]*\br_\w+;$", text))
    fl = list(re.finditer(r"(?m)^([ \t]*)char (_p\w*)\[(\d+)\];\n", text))
    nxt = re.match(r"[ \t]*(\S+)", text[fl[-1].end():]) if fl else None
    if not n_regs or not fl or not nxt or re.match(r"(?:register|int|char|long|unsigned|struct|short)\b", nxt.group(1)):
        return None
    m = fl[-1]                                              # (the trailing one, after the last local)
    n = int(m.group(3)) - 2 * n_regs
    return text[:m.start()] + ("%schar %s[%d];\n" % (m.group(1), m.group(2), n) if n > 0 else "") + text[m.end():]


def unsigned_locals(text, most=3):
    """[the text with one int local unsigned] - a variant each, for the locals compared (<, <=, >, >=): the
    compiler's unsigned jumps"""
    out = []
    for name in re.findall(r"(?m)^[ \t]*int (v\d+);$", text)[:most]:
        if re.search(r"(?:[<>]=? %s\b|\b%s [<>]=?)(?![<>])" % (name, name), text):
            out.append(re.sub(r"(?m)^([ \t]*)int %s;$" % name, r"\1unsigned %s;" % name, text))
    return out


def registers_last(text):
    """the register locals declared after the others (a variant: the slots MS C 3.00 gives some register locals
    below the frame's locals, in the order declared)"""
    if not REGSLOTS[0]:
        return None
    head, sep, body = split_body(text)
    if not sep:
        return None
    lines = body.split("\n")
    k = 0
    while k < len(lines) and (not lines[k].strip() or lines[k].strip() == "{"):
        k += 1
    j = k
    while j < len(lines) and re.fullmatch(r"[ \t]*(?:register |unsigned |int |char |long |short |struct \w+ )[^;(=]*;", lines[j]):
        j += 1
    decl = lines[k:j]
    regs = [x for x in decl if x.strip().startswith("register ")]
    rest = [x for x in decl if not x.strip().startswith("register ")]
    if not regs or not rest or decl == rest + regs:
        return None
    return head + sep + "\n".join(lines[:k] + rest + regs + lines[j:])


def string_variants(prj, text):
    """the drafts where a number passed or given is the place of a string in the data: that string (the model
    reads a literal's address as the number it is); the compiler decides"""
    ds = prj.image[prj.dgroup:]
    nums = sorted(set(int(x) for x in re.findall(r"(?<![\w.])(\d{2,5})(?![\w.])", text.split("{", 1)[-1])))
    cands = []
    for v in nums:
        if 0 < v < len(ds) - 1 and 32 <= ds[v] < 127 and (v == 0 or ds[v - 1] == 0):
            e = ds.find(b"\0", v)
            s = ds[v:e]
            if 0 < len(s) < 200 and all(32 <= c < 127 or c in (9, 10, 13) for c in s):
                from codebreaker.exact.data import cstr
                cands.append((v, cstr(s)))
    out = []
    for v, lit in cands:                                    # (each alone, then all together)
        out.append(_string_decls(re.sub(r"(?<![\w.])%d(?![\w.])" % v, lambda m: lit, text)))
    if len(cands) > 1:
        t = text
        for v, lit in cands:
            t = re.sub(r"(?<![\w.])%d(?![\w.])" % v, lambda m: lit, t)
        out.append(_string_decls(t))
    return out


def _string_decls(text):
    """data given a string (`g = "..";`, `g[i] = "..";`) declared int: a char * (an array of them)"""
    for g, ix in re.findall(r"\b(g_[0-9a-f]{4})(\[[^\]]*\])? = \"", text):
        text = re.sub(r"(?m)^extern int ((?:\w+(?:\[\])?, )*)%s(\[\])?((?:, \w+(?:\[\])?)*);$" % g, lambda d: (
            ("extern int %s;\n" % (d.group(1) + d.group(3).lstrip(", ")).strip(", ") if (d.group(1) + d.group(3)).strip(", ") else "") +
            "extern char *%s%s;" % (g, d.group(2) or "")), text)
    return text


def localize(prj, text, at, names):
    """the model's text with the engine's names and the declarations it needs; None if a machine register is left"""
    def call(m):
        rel = int(m.group(1), 16)
        if rel >= 0x80000000:
            rel -= 1 << 32
        t = (at + rel) & 0xFFFFF if at + rel >= 0 else at + rel + 0x10000   # (a near call wraps in its 64K: the segment's)
        if t not in names:                                  # (the 64K it wraps in: where a routine is)
            t = next((x for x in (t - 0x10000, t + 0x10000) if x in names), t)
        return (names.get(t) or "f%05x" % t) + "("
    longf = {re.sub(r"\bsub_(-?[0-9a-f]+)\(", call, f + "(")[:-1] for f in LONGINFO.get("funcs", ())}   # (routines of a long value)
    text = re.sub(r"\bsub_(-?[0-9a-f]+)\(", call, text)
    text = re.sub(r"\bg_([0-9a-f]{1,4})\b", lambda m: "g_%04x" % int(m.group(1), 16), text)
    text = re.sub(r"(?m)^([ \t]*)(L_\w+:)\n(?=[ \t]*\})", r"\1\2\n\1;\n", text)   # (a label last in its block: on an
    macros = prj.toolchain().p.get("macros")                                         #  empty statement)
    text = re.sub(r"\*\((\w+)(\+\+|--)\)", r"*\1\2", text)   # (*(p++): *p++ - the same, and mendable)
    text = fp_conversions(text, prj.toolchain().p.get("fp_conversions"))
    text = long_helpers(text, prj.toolchain().p.get("long_helpers"))
    text = library_macros(text, macros)                     # (the routines named: the macros they show)
    text = call_tests(data_macros(prj, text, prj.toolchain().p.get("data_macros")))   # (a macro's value tested, switched on)
    if FILEPTR:                                             # (the stream's fields the struct pass read: the macro's now)
        norm = lambda n: "g_%04x" % int(n[2:], 16) if re.fullmatch(r"g_[0-9a-f]{1,4}", n) else n
        streams = {norm(k) for k in FILEPTR if not k.startswith("#")}
        text = re.sub(r"(?m)^struct s_(\w+) \{[^}]*\};\n", lambda m: "" if norm(m.group(1)) in streams else m.group(0), text)
        text = fold_registers(stored_calls(text))
        text = unused_locals(text)
    macnames = {re.match(r"\w+", m["as"]).group(0) for m in (macros or [])} | \
        {re.match(r"\w+", f).group(0) for sp in (prj.toolchain().p.get("data_macros") or []) for f in sp.get("fields", {}).values()}
    body = text.split("{", 1)[1] if "{" in text else text
    # registers the model left: a draft for a person, not for the compiler
    left = sorted(set(re.findall(REGS, re.sub(r'"(?:[^"\\]|\\.)*"', "", body))))
    data = sorted(set(re.findall(r"\bg_[0-9a-f]{4}\b", text)))
    head = text.split("(", 1)[0].split()[-1] if text.split("(", 1)[0].split() else ""   # (its own name: after a type)
    funcs = sorted(set(re.findall(r"\b([A-Za-z_]\w*)\(", body)) - {head, "if", "while", "for", "switch", "return", "sizeof"} - macnames)
    decl = []
    fptr = [d for d in data if re.search(r"\(\s*\*\s*%s\s*\)\s*\(" % d, body)]     # (data called through)
    for d in fptr:                                          # (a number given it that is a function's place: it)
        text = re.sub(r"(?m)^(\s*%s = )(\d+);" % d, lambda m: m.group(1) + (names.get(int(m.group(2))) or
                                                                             "f%05x" % int(m.group(2))) + ";", text)
        body = text.split("{", 1)[1] if "{" in text else text
    funcs = sorted(set(funcs) | {w for d in fptr for w in re.findall(r"(?m)^[ \t]*%s = ([A-Za-z_]\w*);" % d, body)})
    for a_ in [d for d in data if ARRAYS.get(d) == "int"]:  # (an array of words named whole, and the words just
        base = int(a_[2:], 16)                              #  after it by their own places: its elements)
        near = [d for d in data if d != a_ and d not in ARRAYS and 0 < int(d[2:], 16) - base < 0x40 and (int(d[2:], 16) - base) % 2 == 0]
        if re.search(r"\b%s\b(?!\[)" % a_, body) or near:
            hd, sp_, bd = text.partition("{")
            bd = re.sub(r"\b%s\b(?!\[)" % a_, "%s[0]" % a_, bd)
            for d in near:
                bd = re.sub(r"\b%s\b" % d, "%s[%d]" % (a_, (int(d[2:], 16) - base) // 2), bd)
            text = hd + sp_ + bd
            body = bd
            data = [d for d in data if d not in near]
    arrs = [d for d in data if d in ARRAYS and d not in fptr]   # (indexed in place: an array, not a pointer)
    pvars = set(re.findall(r"(?m)^[ \t]*(?:register\s+)?(?:unsigned\s+)?(?:char|int)\s*\*\s*(\w+);", text))   # (pointers the draft declares)
    sptrs = dict((n, t_) for t_, n in re.findall(r"(?m)^[ \t]*(?:register\s+)?(struct \w+)\s*\*\s*(\w+);", text))
    pvars |= set(sptrs)
    flows = lambda d: any(re.search(r"(?m)^[ \t]*%s = %s\b|^\s*%s = %s\b" % (p, d, d, p), body) for p in pvars)
    for d in data:                                          # (data given a struct pointer's value: a pointer to it)
        src_ = next((p for p in sptrs if re.search(r"(?m)^[ \t]*(?:\w+ = )*%s = (?:\w+ = )*%s;$" % (d, p), body)), None)
        if src_ and d not in STRUCTPTR:
            STRUCTPTR[d] = sptrs[src_].split()[1]
    ptrs = [d for d in data if d not in fptr and d not in arrs and (re.search(r"\*\s*\(?\s*(?:\+\+|--)?\s*%s\b|\b%s\s*(?:\+\+|--)?\s*(?:->|\[)" % (d, d), body)
                                                                  or flows(d))]   # (or given a pointer's value, or giving one)
    ints = [d for d in data if d not in ptrs and d not in fptr and d not in arrs]
    for nb, x_, y_ in re.findall(r"\*\(struct _b(\d+) \*\)(\w+) = \*\(struct _b\1 \*\)(\w+);", text):
        for v_ in (x_, y_):                                 # (the pointers copied through: pointers to the block)
            t_ = _redeclare(text, v_, "struct _b%s *%s" % (nb, v_))
            if t_ and not re.search(r"\b%s\s*(?:->|\[)" % v_, text):
                text = t_.replace("*(struct _b%s *)%s" % (nb, v_), "*%s" % v_)
    for nb in sorted(set(re.findall(r"\bstruct _b(\d+) \*", text)), key=int):   # (a block of bytes copied whole)
        decl.insert(0, "struct _b%s { char _c[%s]; };" % (nb, nb))
    if arrs:
        # (arrays of structs of one size whose places lie within one element: one array, its fields at the
        #  places' distances from the first)
        sa = sorted((int(d[2:], 16), d) for d in arrs if d in STRUCTARR)
        merged = {}
        for b, d in sa:
            base = next((x for x in merged if STRUCTARR[merged[x][0]][0] == STRUCTARR[d][0] and 0 < b - x < STRUCTARR[d][0]), None)
            if base is None:
                merged[b] = [d]
            else:
                merged[base].append(d)
        for base, ds in merged.items():
            if len(ds) < 2:
                continue
            bn = ds[0]
            for d in ds[1:]:
                off = int(d[2:], 16) - base
                text = re.sub(r"\b%s\[([^\[\]]+)\]\.f0\b" % d, lambda m, o=off: "%s[%s].f%d" % (bn, m.group(1), o), text)
                arrs = [x for x in arrs if x != d]
            elems = []
            for d in ds:
                t_ = STRUCTARR[d][1]
                if isinstance(t_, list):                    # (laid out before - by an earlier variant of this text: its first field's type)
                    t_ = t_[0][1]
                elems.append((int(d[2:], 16) - base, "int" if str(t_).startswith("struct") else t_))
            STRUCTARR[bn] = (STRUCTARR[bn][0], elems)
        for d in arrs:                                      # (an array of structs: its element laid out - the field
            if d in STRUCTARR:                              #  the code reads first, the rest of its size after)
                k, ty = STRUCTARR[d]
                fl, pos = [], 0
                for off, t_ in (ty if isinstance(ty, list) else [(0, ty)]):
                    if off > pos:
                        fl.append("char _%d[%d];" % (pos, off - pos))
                    if t_ == "int" and re.search(r"\b%s\[[^\]]*\]\.f%d->" % (d, off), text) and \
                       re.search(r"(?m)^struct s_f%d \{" % off, text):
                        t_ = "struct s_f%d *" % off             # (a field gone through: a pointer to what it reads)
                    elif t_ == "int" and re.search(r"(?<![\w)\]])\*%s\[[^\]]*\]\.f%d\b(?!\s*(?:->|\[|\.))" % (d, off), text):
                        t_ = "int *"
                    fl.append("%s f%d;" % (t_, off))
                    pos = off + (1 if t_ == "char" else 2)
                if k > pos:
                    fl.append("char _%d[%d];" % (pos, k - pos))
                decl.append("struct s_%s { %s };" % (d, " ".join(fl)))
        decl += ["extern %s %s[];" % (ARRAYS[d], d) for d in arrs]
    if fptr:
        decl.append("extern int %s;" % ", ".join("(*%s)()" % d for d in fptr))
    streams = [d for d in data if d in FILEPTR]             # (a stream a library macro reads)
    ptrs = [d for d in ptrs if d not in streams]
    ints = [d for d in ints if d not in streams]
    decl += ["extern %s *%s;" % (FILEPTR[d], d) for d in streams]
    head2, sep2, body2 = text.partition("{")
    for p in [n for n in FILEPTR if not n.startswith("#") and n not in data]:   # (a parameter that is one)
        head2 = re.sub(r"(?m)^([ \t]*)(?:register )?(?:int|char \*|struct \w+ \*) ?%s;$" % p, r"\1%s *%s;" % (FILEPTR[p], p), head2)
        head2 = re.sub(r"(?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);$" % p,
                       lambda m: ("%sint %s;\n" % (m.group(1), (m.group(2) + m.group(3).lstrip(", ")).strip(", ")) if (m.group(2) + m.group(3)).strip(", ") else "") +
                       "%s%s *%s;" % (m.group(1), FILEPTR[p], p), head2)
    text = head2 + sep2 + body2
    incs = list(dict.fromkeys(FILEPTR[k] for k in ("#include", "#include2") if FILEPTR.get(k)))
    for h in reversed(incs):
        decl.insert(0, "#include <%s>" % h)
    if ptrs:                                                # (data the draft goes through: pointers)
        sp_ = {("g_%04x" % int(k[2:], 16) if re.fullmatch(r"g_[0-9a-f]{1,4}", k) else k): v for k, v in STRUCTPTR.items()}
        for d in ptrs:                                      # (a word read through it, never a byte: an int *)
            word = any((r + "w", 0) in BYTEPTR for r in ("bx", "si", "di")) and \
                not any((r + "*", 0) in BYTEPTR for r in ("bx", "si", "di")) and re.search(r"\*%s\b" % d, body)
            decl.append("extern %s *%s;" % ("struct " + sp_[d] if d in sp_ else "char *" if PTRSIZE.get(d) == 2 and re.search(r"\*\+\+%s\b" % d, body)
                                            else "int" if word or PTRSIZE.get(d) == 2 else "char", d))
    uch = [d for d in ints if d in UCHAR]
    chs = [d for d in ints if d in BYTEG and d not in UCHAR]
    ints = [d for d in ints if d not in UCHAR and d not in BYTEG]
    if uch:
        decl.append("extern unsigned char %s;" % ", ".join(uch))
    if chs:
        decl.append("extern char %s;" % ", ".join(chs))
    lngs = [d for d in ints if d in LONGINFO.get("globals", ())]   # (a long in the data: its two words one)
    ints = [d for d in ints if d not in lngs]
    if lngs:
        decl.append("extern long %s;" % ", ".join(lngs))
    if ints:
        decl.append("extern int %s;" % ", ".join(ints))
    for h in incs:                                          # (what an included header declares: its)
        funcs = [f for f in funcs if f not in header_names(prj, h)]
    if [f for f in funcs if f in longf]:
        decl.append("extern long %s;" % ", ".join("%s()" % f for f in funcs if f in longf))
    funcs = [f for f in funcs if f not in longf]
    if funcs:
        decl.append("extern %s;" % ", ".join("%s()" % f for f in funcs))
    text = fp_types(text)
    # (a byte through a char pointer compared with 128..255: the compiler compares bytes only of an unsigned char)
    for p in set(re.findall(r"\*\(?(?:\+\+|--)?(\w+)(?:\+\+|--)?\)? [!=]= (?:1(?:2[89]|[3-9]\d)|2[0-4]\d|25[0-5])\b", text)):
        text = re.sub(r"(?m)^([ \t]*)(register )?char \*%s;$" % p, r"\1\2unsigned char *%s;" % p, text)
    for p_, k_, e_, n_ in re.findall(r"(?m)^[ \t]*(\w+) = \((\d+) \* ([^()]+?)\) \+ (\d+);$", text):
        tag = re.search(r"(?m)^[ \t]*(?:register )?struct (\w+) \*%s;$" % p_, text)   # (a struct pointer given
        dm = re.search(r"(?m)^struct %s \{([^}]*)\};$" % tag.group(1), text) if tag else None   # K * E + N: the
        if dm and int(n_) >= 0x100:                                                       # address of an element
            k, n = int(k_), int(n_)                                                       # of an array there)
            fl = re.findall(r"(char|int|long|\*) ?_?f?(\d+)(?:\[(\d+)\])?;", dm.group(1))
            size = max((int(o) + (int(c) if c else 4 if t == "long" else 1 if t == "char" else 2) for t, o, c in fl), default=0)
            if size <= k:
                body_ = dm.group(1) + (" char _%d[%d];" % (size, k - size) if size < k else "")
                name = "g_%04x" % n
                text = text.replace(dm.group(0), "struct %s {%s };\nextern struct %s %s[];" % (tag.group(1), body_.rstrip(), tag.group(1), name))
                text = re.sub(r"(?m)^([ \t]*)%s = \(%s \* %s\) \+ %s;$" % (p_, k_, re.escape(e_), n_), r"\1%s = &%s[%s];" % (p_, name, e_), text)
    for ty, x in set(re.findall(r"\*\((char|int|unsigned char) \*\)(\w+)\b", text)):   # (only ever read through one
        others = re.findall(r"\*\((?:char|int|unsigned char) \*\)%s\b" % x, text)       #  cast: a pointer to it)
        if all(o == "*(%s *)%s" % (ty, x) for o in others) and not re.search(r"(?<![)\w])\*\s*%s\b|\b%s\s*(?:->|\[)" % (x, x), text):
            t2 = _redeclare(text, x, "%s *%s" % (ty, x)) if re.search(r"(?m)^[ \t]*(?:register )?int (?:\w+, )*%s\b" % x, text) else None
            if t2:
                text = t2.replace("*(%s *)%s" % (ty, x), "*%s" % x)
    out = ("\n".join(decl) + "\n\n" if decl else "") + text
    dn = _data_names(prj)                                   # (data the program has names for: those, by place - a
    if dn:                                                  #  name of the form g_XXXX may be another place's)
        out = re.sub(r"\bg_([0-9a-f]{4})\b", lambda m: dn.get(int(m.group(1), 16), m.group(0)), out)
    return out, left


def _data_names(prj):
    """{data offset: name} of the data SYMBOLS names (where a name differs from the place's own g_XXXX)"""
    if getattr(prj, "_dnames", None) is None:
        prj._dnames = {a: n.lstrip("_") for n, (t, a) in prj.symbols().items()
                       if t == "D" and ":" not in n and re.fullmatch(r"_?\w+", n) and n.lstrip("_") != "g_%04x" % a}
    return prj._dnames


def long_helpers(text, helpers):
    """the library's long arithmetic (the profile's "long_helpers": routine -> C operator) as the operator:
    lmul(X, Y) is (X * Y); an assigning one, almul(&X, Y), is X *= Y"""
    if not helpers:
        return text
    rx = re.compile(r"\b(%s)\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)" % "|".join(re.escape(h) for h in helpers))

    uns = set()

    def one(m):
        op = helpers[m.group(1)]
        args = [x.strip() for x in _top_commas(m.group(2))]
        if len(args) == 3:                                  # (the long operand pushed a word at a time: its words
            f1 = re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", args[1])   #  a field's two - the long there)
            f2 = re.fullmatch(r"(\w+(?:->f\d+)*)->f(\d+)", args[2])
            if f1 and f2 and f1.group(1) == f2.group(1) and int(f2.group(2)) == int(f1.group(2)) + 2:
                args = [args[0], "*(long *)&%s" % args[1]]
        if len(args) != 2:
            return m.group(0)
        if re.match(r"a?ul", m.group(1)) and re.fullmatch(r"&?\w+", args[0]):   # (an unsigned one's operand:
            uns.add(args[0].lstrip("&"))                                         #  an unsigned long)
        if op.endswith("=") and op not in ("==", "<=", ">=", "!="):
            if not args[0].startswith("&"):
                return m.group(0)
            return "(%s %s %s)" % (args[0][1:], op, args[1])
        v = "(%s %s %s)" % (args[0], op, args[1])
        before, after = m.string[:m.start()].rstrip(), m.string[m.end():].lstrip()
        if re.search(r"\w\($|,$", before) and after[:1] in (")", ","):   # (an argument pushed as one word: the
            return "(int)%s" % v                                            #  long's low word - an int)
        return v
    t = rx.sub(one, text)
    for x in uns:
        t = re.sub(r"(?m)^([ \t]*)(register )?long %s;$" % x, r"\1\2unsigned long %s;" % x, t)
    t = re.sub(r"(?m)^([ \t]*)\((.+ [*/%<>+-]{1,2}= .+)\);$", r"\1\2;", t)   # (a statement of its own: no parens)
    # (the long it leaves in dx:ax tested at once - `ax = (ax | dx)`: the assignment's value)
    t2 = re.sub(r"(?m)^([ \t]*)(\w+ (?:[*/%+-]|<<|>>)= [^;\n]+);\n\s*ax = \(ax \| dx\);$", r"\1ax = (\2);", t)
    t2 = re.sub(r"(?m)^[ \t]*(\w+ (?:[*/%+-]|<<|>>)= [^;\n]+);\n(\s*)\} while \(\(\(ax \| dx\) != 0\)\);$",
                r"\2} while (((\1) != 0));", t2)              # (a do's test of it)
    return fold_registers(t2) if t2 != t else t


def fp_conversions(text, routines):
    """the library's conversion of the 8087's top to an integer (the profile's "fp_conversions": its names - ftol)
    is C's own conversion in an assignment: `X = ftol(E);` is `X = E;`, and `p->f0 = ftol(E); p->f2 = dx;` (the
    high half too) `*p = E;` of a long *p"""
    if not routines:
        return text
    rx = "|".join(re.escape(r) for r in routines)
    arg = r"((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)"
    def long_(m):
        FPINFO.setdefault("types", {})["*" + m.group(2)] = "long"
        return "%s*%s = %s;" % (m.group(1), m.group(2), m.group(3))
    text = re.sub(r"(?m)^([ \t]*)(\w+)->f0 = (?:%s)\(%s\);\n\s*\2->f2 = dx;$" % (rx, arg), lambda m: long_(m) if True else "", text)
    text = re.sub(r"(?m)^([ \t]*)(.+?) = (?:%s)\(%s\);$" % (rx, arg), r"\1\2 = \3;", text)
    return text


def fp_types(text):
    """the types the 8087's operands showed (FPINFO): `*p` a float/double/long - p a pointer to one; vN or a
    parameter one itself; data declared so; a function giving a double back declared double"""
    types = FPINFO.get("types") or {}
    if not types and not FPINFO.get("ret"):
        return text
    head, sep, body = split_body(text)
    if not sep:
        return text
    pre, fn = "", head
    m = re.search(r"(?m)^(?:\w+\s*\n)?\w+\(", head)         # (the declarations before the function: data's)
    if m:
        pre, fn = head[:m.start()], head[m.start():]
    for nm, ty in types.items():
        m = re.fullmatch(r"\*\(?(\w+)\)?", nm)
        x, ptr = (m.group(1), True) if m else (nm, False)
        if not re.fullmatch(r"\w+", x):
            continue
        new = "%s %s%s" % (ty, "*" if ptr else "", x)
        f2 = _redeclare(fn, x, new)
        if f2:
            fn = f2
            continue
        lines = body.split("\n")                            # (a local: among the declarations at the top)
        for k, l in enumerate(lines):
            if l.strip() and not re.match(r"\s*(?:register\s+)?(?:int|char|long|unsigned|short|struct|float|double)\b", l):
                break
            l2 = _redeclare(l, x, new) if l.strip() else None
            if l2:
                lines[k] = l2
                break
        else:
            lines = None
        if lines is not None:
            body = "\n".join(lines)
        if x.startswith("g_"):
            pre = re.sub(r"(?m)^extern int ((?:\w+, )*)%s((?:, \w+)*);$" % x, lambda d: (
                ("extern int %s;\n" % (d.group(1) + d.group(2).lstrip(", ")).strip(", ") if (d.group(1) + d.group(2)).strip(", ") else "") +
                "extern %s;" % new), pre)
    if FPINFO.get("ret"):
        m = re.search(r"(?m)^\w+\(", fn)
        if m and not re.search(r"(?m)^double\s*$", fn):
            fn = fn[:m.start()] + FPINFO["ret"] + "\n" + fn[m.start():]
    return pre + fn + sep + body


FRAME = [0]


def fill_frame(text, fs, regs_count=True):
    """locals declared for the part of the frame no declared local takes (a local the source declared and never
    used still has its place; MS C 3.00 gives a register local one too): a filler `char _pf[n]` at the end"""
    if not fs or "{" not in text:
        return text
    head, sep, body = split_body(text)
    size = 0
    lines = body.split("\n")
    last = 0
    gaps = []                                               # ((line, bytes): a filler before a local vN the
    for k, l in enumerate(lines[1:], 1):                    #  declarations above would put nearer bp than N)
        m = re.match(r"\s+(register\s+)?((?:unsigned\s+)?(?:int|char|long|short|struct\s+\w+))\s*(\**)\s*(\w+)\s*(?:\[(\d+)\])?;\s*$", l)
        if not m:
            if l.strip():
                break
            continue
        reg, ty, star, name, cnt = m.groups()
        last = k
        if reg:
            size += 2 if regs_count else 0
            continue
        if ty.startswith("struct") and not star:
            return text                                     # (a struct's size the text does not say)
        one = 2 if star else SIZES.get(ty.split()[-1], 2)
        n = one * int(cnt or 1)
        v = re.fullmatch(r"v(\d+)", name)
        if v and not star and not cnt and ty.split()[-1] == "long" and size + n < int(v.group(1)):
            gaps.append((k, int(v.group(1)) - size - n))    # (a long the pair made of vN and the word above it:
            size = int(v.group(1)) - n                      #  where vN was)
        size += n + (n & 1)
    for k, g in reversed(gaps):
        lines.insert(k, "    char _pg%d[%d];" % (k, g))
        last += 1
    if gaps and fs > size:
        lines.insert(last + 1, "    char _pf[%d];" % (fs - size))
        return head + sep + "\n".join(lines)
    if fs <= size or re.search(r"(?m)^\s+(?:int|char|long|short)\s+v\d+", body):
        return text if not gaps else head + sep + "\n".join(lines)   # (the model's autos: the frame model lays those out)
    lines.insert(last + 1, "    char %s[%d];" % (FILLNAME[0] % size if "%" in FILLNAME[0] else FILLNAME[0], fs - size))
    return head + sep + "\n".join(lines)


FILLNAME = ["_pf"]                                          # (the trailing filler's name: _pf, or _p%d - its place,
FPINFO = {}                                               # (the function's floating point: types, its value, any)
