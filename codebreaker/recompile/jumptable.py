"""jumptable.py -- the switch tables behind a `bctr`, so a recompiled switch is a C++ switch.

    jumptable.detect_jump_tables(insns, lo, hi, rd_u8, rd_u16, rd_u32, is_exec)
        {bctr address: (index register, [targets])}

The Xbox 360 compiler builds a switch four ways (a table of addresses; of byte, halfword or
word offsets from a base); each is recognised by scanning back from the `bctr` for the bounds
check, the table's address (lis/addi, also when the table register is the base register --
the base is found first), and the load, and the table is read from the image. A target
outside the function becomes a tail call. Derived from the rexglue SDK's function scanner
(src/codegen/function_scanner.cpp; BSD-3-Clause -- see NOTICE), with its register-reuse and
RA/RB-swap handling.
"""

MAX_SCAN_BACK = 64

# ---- PPC instruction field decode (big-endian word) -----------------------
def _op(w):    return (w >> 26) & 0x3F
def _xo(w):    return (w >> 1) & 0x3FF          # X/XO extended opcode (bits 1-10)
# D-form
def _d_rt(w):  return (w >> 21) & 0x1F
def _d_ra(w):  return (w >> 16) & 0x1F
def _d_uimm(w):return w & 0xFFFF
# X-form / XO-form
def _x_rt(w):  return (w >> 21) & 0x1F
def _x_ra(w):  return (w >> 16) & 0x1F
def _x_rb(w):  return (w >> 11) & 0x1F
# M-form (rlwinm): RS=src(21-25), RA=dest(16-20), SH(11-15), MB(6-10), ME(1-5)
def _m_rs(w):  return (w >> 21) & 0x1F
def _m_ra(w):  return (w >> 16) & 0x1F
def _m_sh(w):  return (w >> 11) & 0x1F
def _m_mb(w):  return (w >> 6) & 0x1F
def _m_me(w):  return (w >> 1) & 0x1F


# ---- opcode predicates (match decode_instruction's Opcode:: classification) --
def _is_mtctr(w):
    if _op(w) != 31 or _xo(w) != 467:
        return None
    spr = ((w >> 16) & 0x1F) | (((w >> 11) & 0x1F) << 5)
    return _x_rt(w) if spr == 9 else None

def _is_mflr(w):
    if _op(w) != 31 or _xo(w) != 339:
        return False
    spr = ((w >> 16) & 0x1F) | (((w >> 11) & 0x1F) << 5)
    return spr == 8

def _is_add(w):    return _op(w) == 31 and _xo(w) == 266
def _is_lwzx(w):   return _op(w) == 31 and _xo(w) == 23
def _is_lbzx(w):   return _op(w) == 31 and _xo(w) == 87
def _is_lhzx(w):   return _op(w) == 31 and _xo(w) == 279
def _is_rlwinm(w): return _op(w) == 21
def _is_lis(w):    return _op(w) == 15 and _d_ra(w) == 0      # addis rX,r0,imm
def _is_addi(w):   return _op(w) == 14
def _is_ori(w):    return _op(w) == 24
def _is_bctr(w):   return _op(w) == 19 and _xo(w) == 528 and (w & 1) == 0
def _is_bcctr_any(w): return _op(w) == 19 and _xo(w) == 528   # bctr or bctrl
def _is_b(w):      return _op(w) == 18                        # b / ba
def _is_blr(w):
    # bclr (op19,xo16) with BO=20 (unconditional) -> blr
    return _op(w) == 19 and _xo(w) == 16 and ((w >> 21) & 0x1F) == 20
def _is_cmpli(w):  return _op(w) == 10                        # cmplwi/cmpldi
def _is_cmpi(w):   return _op(w) == 11                        # cmpwi/cmpdi
def _is_stwu(w):   return _op(w) == 37


def _is_function_boundary(w):
    """Port of is_function_boundary()."""
    if w == 0:
        return True
    if _is_blr(w):
        return True
    if _is_bcctr_any(w):
        return True
    if _is_b(w):
        return True
    if _is_mflr(w):
        return True
    if _is_stwu(w) and _d_ra(w) == 1 and _d_rt(w) == 1:   # stwu r1,-N(r1)
        return True
    return False


def _s16(v):
    return v - 0x10000 if v & 0x8000 else v


def _scan_bounds(rd_u32, bctr_addr, expected_index_reg):
    """Port of scanForBounds -> (max_entries, default_target)."""
    max_entries = 0
    default_target = 0
    cr_field = 0xFF
    for i in range(1, MAX_SCAN_BACK + 1):
        addr = bctr_addr - i * 4
        if addr < 4:
            break
        w = rd_u32(addr)
        if w is None or w == 0:
            break
        # conditional branch testing GT bit -> capture CR field + default target
        if cr_field == 0xFF and _op(w) == 16:                # bc family
            bi = (w >> 16) & 0x1F
            if (bi & 0x3) == 1:                              # tests GT bit (bgt/ble)
                cr_field = (bi >> 2) & 0x7
                bd = w & 0xFFFC
                if bd & 0x8000:
                    bd -= 0x10000
                aa = (w >> 1) & 1
                default_target = (bd & 0xFFFFFFFF) if aa else ((addr + bd) & 0xFFFFFFFF)
        # clrlwi/rlwinm implicit mask on the index register -> 2^(32-mb) entries
        if _is_rlwinm(w) and _m_ra(w) == expected_index_reg:
            sh, mb, me = _m_sh(w), _m_mb(w), _m_me(w)
            if sh == 0 and me == 31 and 0 < mb < 32:
                cnt = 1 << (32 - mb)
                if 2 <= cnt <= 256:
                    return cnt, default_target
        # cmpli/cmpi: count = imm + 1
        if _is_cmpli(w) or _is_cmpi(w):
            cmp_cr = _d_rt(w) >> 2
            cmp_ra = _d_ra(w)
            cmp_imm = _d_uimm(w)
            if cmp_imm <= 1:
                continue
            reg_ok = (cmp_ra == expected_index_reg)
            cr_ok = (cr_field != 0xFF and cmp_cr == cr_field)
            if reg_ok or (cr_ok and cmp_imm > 1):
                max_entries = cmp_imm + 1
                if reg_ok:
                    return max_entries, default_target
    return max_entries, default_target


def _read_targets(m, count, rd_u8, rd_u16, rd_u32, is_exec):
    """Port of read_table_entries."""
    table = (m["table_high"] + _s16(m["table_low"])) & 0xFFFFFFFF
    basev = (m["base_high"] + _s16(m["base_low"])) & 0xFFFFFFFF
    ttype = m["type"]
    shift = m["shift"]
    out = []
    i = 0
    while count == 0 or i < count:
        if ttype == "abs":
            t = rd_u32(table + i * 4)
            if t is None:
                break
            target = t
        elif ttype == "computed":
            b = rd_u8(table + i)
            if b is None:
                break
            target = (basev + (b << shift)) & 0xFFFFFFFF
        elif ttype == "byteoff":
            b = rd_u8(table + i)
            if b is None:
                break
            target = (basev + b) & 0xFFFFFFFF
        else:  # shortoff
            h = rd_u16(table + i * 2)
            if h is None:
                break
            target = (basev + h) & 0xFFFFFFFF
        if target & 3:
            break
        if target == 0 and ttype == "abs":
            break
        if not is_exec(target):
            if ttype == "abs":
                break
            i += 1
            continue
        out.append(target)
        i += 1
    return out


def _detect_one(rd_u8, rd_u16, rd_u32, is_exec, bctr_addr):
    """Port of FunctionScanner::detect_jump_table for a single bctr address."""
    m = {"type": None, "ctr_reg": None, "table_reg": None, "index_reg": None,
         "base_reg": None, "offset_reg": None, "shift": 0,
         "table_high": 0, "table_low": 0, "base_high": 0, "base_low": 0,
         "found_mtctr": False, "found_add": False, "found_load": False,
         "found_shift": False, "found_table_lis": False, "found_table_addi": False,
         "found_base_lis": False, "found_base_addi": False}

    for i in range(1, MAX_SCAN_BACK + 1):
        addr = bctr_addr - i * 4
        if addr < 4:
            break
        w = rd_u32(addr)
        if w is None:
            break

        if _is_function_boundary(w):
            # allow continuing past an adjacent bctr while still hunting the lis
            if _is_bcctr_any(w) and m["found_load"] and not m["found_table_lis"]:
                continue
            break

        # Step 1: mtctr rX
        if not m["found_mtctr"]:
            if _is_mtctr(w) is not None:
                m["found_mtctr"] = True
                m["ctr_reg"] = _is_mtctr(w)
            continue
        if not m["found_mtctr"]:
            continue

        # Step 2a: add rT(=ctr), rBase, rOffset  (offset-based types)
        if not m["found_add"] and not m["found_load"] and _is_add(w):
            if _x_rt(w) == m["ctr_reg"]:
                m["base_reg"] = _x_ra(w)
                m["offset_reg"] = _x_rb(w)
                m["found_add"] = True
                continue

        # Step 2b: lwzx rT(=ctr) -> ABSOLUTE (RA=table, RB=index, tentative)
        if not m["found_add"] and not m["found_load"] and _is_lwzx(w):
            if _x_rt(w) == m["ctr_reg"]:
                m["type"] = "abs"
                m["table_reg"] = _x_ra(w)
                m["index_reg"] = _x_rb(w)
                m["found_load"] = True
                continue

        # offset-based load: lbzx (byteoff/computed) or lhzx (shortoff)
        if m["found_add"] and not m["found_load"]:
            if _is_lbzx(w) and _x_rt(w) == m["offset_reg"]:
                m["table_reg"] = _x_ra(w)
                m["index_reg"] = _x_rb(w)
                m["found_load"] = True
                if not m["found_shift"]:
                    m["type"] = "byteoff"
                continue
            if _is_lhzx(w) and _x_rt(w) == m["offset_reg"]:
                m["type"] = "shortoff"
                m["table_reg"] = _x_ra(w)
                m["index_reg"] = _x_rb(w)
                m["found_load"] = True
                continue

        # Step 3: rlwinm — index scaling (back-track index_reg) or offset shift (COMPUTED)
        if not m["found_shift"] and _is_rlwinm(w):
            if m["found_load"] and _m_ra(w) == m["index_reg"]:
                m["index_reg"] = _m_rs(w)
                m["found_shift"] = True
                continue
            if (m["found_add"] and _m_ra(w) == m["offset_reg"]
                    and m["type"] != "shortoff"):
                m["shift"] = _m_sh(w)
                m["type"] = "computed"
                m["found_shift"] = True
                m["offset_reg"] = _m_rs(w)
                continue

        # Step 4: lis/addi for the TABLE address (+ RA/RB swap, + register reuse)
        if m["found_load"]:
            register_reuse = m["found_add"] and (m["table_reg"] == m["base_reg"])
            # lis matching table_reg
            if _is_lis(w) and _d_rt(w) == m["table_reg"]:
                if register_reuse:
                    if not m["found_base_lis"]:
                        m["base_high"] = (_d_uimm(w) << 16) & 0xFFFFFFFF
                        m["found_base_lis"] = True
                        continue
                    elif not m["found_table_lis"]:
                        m["table_high"] = (_d_uimm(w) << 16) & 0xFFFFFFFF
                        m["found_table_lis"] = True
                        continue
                elif not m["found_table_lis"]:
                    m["table_high"] = (_d_uimm(w) << 16) & 0xFFFFFFFF
                    m["found_table_lis"] = True
                    continue
            # lis matching index_reg -> guessed wrong, swap table/index
            elif (not m["found_table_lis"] and _is_lis(w)
                  and _d_rt(w) == m["index_reg"]):
                m["table_reg"], m["index_reg"] = m["index_reg"], m["table_reg"]
                m["table_high"] = (_d_uimm(w) << 16) & 0xFFFFFFFF
                m["found_table_lis"] = True
                continue
            # addi/ori matching table_reg
            if (_is_addi(w) or _is_ori(w)) and _d_rt(w) == m["table_reg"]:
                if register_reuse:
                    if not m["found_base_addi"]:
                        m["base_low"] = _d_uimm(w)
                        m["found_base_addi"] = True
                        continue
                    elif not m["found_table_addi"]:
                        m["table_low"] = _d_uimm(w)
                        m["found_table_addi"] = True
                        continue
                elif not m["found_table_addi"]:
                    m["table_low"] = _d_uimm(w)
                    m["found_table_addi"] = True
                    continue
            # addi/ori matching index_reg -> guessed wrong, swap
            elif (not m["found_table_addi"] and (_is_addi(w) or _is_ori(w))
                  and _d_rt(w) == m["index_reg"]):
                m["table_reg"], m["index_reg"] = m["index_reg"], m["table_reg"]
                m["table_low"] = _d_uimm(w)
                m["found_table_addi"] = True
                continue

        # Step 5: lis/addi for the BASE address (offset-based, non-reuse), + swap
        if m["found_add"]:
            if not m["found_base_lis"] and _is_lis(w):
                if _d_rt(w) == m["base_reg"]:
                    m["base_high"] = (_d_uimm(w) << 16) & 0xFFFFFFFF
                    m["found_base_lis"] = True
                    continue
                elif _d_rt(w) == m["offset_reg"]:
                    m["base_reg"], m["offset_reg"] = m["offset_reg"], m["base_reg"]
                    m["base_high"] = (_d_uimm(w) << 16) & 0xFFFFFFFF
                    m["found_base_lis"] = True
                    continue
            if not m["found_base_addi"] and (_is_addi(w) or _is_ori(w)):
                if _d_rt(w) == m["base_reg"]:
                    m["base_low"] = _d_uimm(w)
                    m["found_base_addi"] = True
                    continue
                elif _d_rt(w) == m["offset_reg"]:
                    m["base_reg"], m["offset_reg"] = m["offset_reg"], m["base_reg"]
                    m["base_low"] = _d_uimm(w)
                    m["found_base_addi"] = True
                    continue

        # complete?
        table_complete = m["found_table_lis"] and m["found_table_addi"]
        base_complete = (not m["found_add"]) or (m["found_base_lis"] and m["found_base_addi"])
        if m["found_mtctr"] and m["found_load"] and table_complete and base_complete:
            break

    if not (m["found_mtctr"] and m["found_load"]
            and m["found_table_lis"] and m["found_table_addi"]):
        return None
    if m["found_add"] and not (m["found_base_lis"] and m["found_base_addi"]):
        return None

    table_addr = (m["table_high"] + _s16(m["table_low"])) & 0xFFFFFFFF
    if rd_u8(table_addr) is None:
        return None

    cnt, _default = _scan_bounds(rd_u32, bctr_addr, m["index_reg"])
    targets = _read_targets(m, cnt, rd_u8, rd_u16, rd_u32, is_exec)
    if len(targets) < 2:
        return None
    return m["index_reg"], targets


def detect_jump_tables(insns, func_lo, func_hi, rd_u8, rd_u16, rd_u32, is_exec):
    """For each `bctr` in the function, detect a jump table (the four kinds above).
    insns: list of (va, raw_word, mn, op_str); raw_word may be int or big-endian
    bytes. rd_u8/rd_u16/rd_u32(addr) read the image big-endian (None if unmapped);
    is_exec(addr) validates a candidate target lands in executable code.
    Returns {bctr_va: (index_reg, [targets])}."""
    out = {}
    for (va, w, mn, _op) in insns:
        wi = w if isinstance(w, int) else int.from_bytes(w, "big")
        if not _is_bctr(wi):
            continue
        res = _detect_one(rd_u8, rd_u16, rd_u32, is_exec, va)
        if res is not None:
            out[va] = res
    return out
