#!/usr/bin/env python3
"""mc2model.py - deterministic forward model of the MC2/LC2 code generator:
quad records -> exact 8086 bytes, no compiler. First brick of the Compiler-Inverse
Engine. Operand/action model: const/global/param/deref(+field) x load/store/alu.
Covers integer arith + relationals + assignment + globals/pointers/struct-fields.
"""
import sys
from codebreaker.engines.inverse.qalign import parse_quads, parse_obj_funcs

def bs(*h):
    o=bytearray()
    for x in h: o+=bytes.fromhex(x.replace(" ",""))
    return bytes(o)
def w16(v): return bs("%02x"%(v&0xff),"%02x"%((v>>8)&0xff))

ALU_MEM={0x13:0x03,0x14:0x2b,0x1d:0x23,0x1f:0x0b,0x1e:0x33}   # ax, [mem]  (mod=01 rm=110 [bp+d8] -> +0x46)
ALU_IMM={0x13:0x05,0x14:0x2d,0x1d:0x25,0x1f:0x0d,0x1e:0x35}   # ax, imm16
RELJCC ={0x17:0x7c,0x18:0x7e,0x19:0x7f,0x1a:0x7d,0x1b:0x74,0x1c:0x75}
BINCLASS=set(ALU_MEM)|set(RELJCC)|{0x10,0x11,0x12,0x15,0x16}

# --- operand parsing / classification ------------------------------------
def parse_operands(payload):
    ops=[]; i=0
    while i < len(payload):
        k=payload[i]
        if k==0x04 and i+1<len(payload) and payload[i+1]==0x80:
            ops.append({"k":"const","val":payload[i+3]|(payload[i+4]<<8)}); i+=7
        elif k==0x0a and i+1<len(payload) and payload[i+1]==0x00 and i+11<=len(payload):
            ops.append({"k":"node","id":payload[i+3]|(payload[i+4]<<8),"ty":payload[i+5]}); i+=11
        elif k==0x0a and i+1<len(payload) and payload[i+1]==0x20 and i+11<=len(payload):
            ops.append({"k":"deref","id":payload[i+3]|(payload[i+4]<<8),
                        "off":payload[i+9]}); i+=11          # deref = 11B, field offset at [+9]
        elif k==0x0e and i+15<=len(payload):
            ops.append({"k":"temp"}); i+=15          # temp = 11B + 4B result back-link (XX 01 00 00)
        elif k==0x00 and i+1<len(payload) and payload[i+1] in (0x04,0x0a,0x0e):
            i+=1                                            # skip 1-byte inter-operand separator
        else: break
    return ops

class Ctx:
    def __init__(s, fn, fs, psz=2): s.fn=fn; s.fs=fs; s.psz=psz    # psz = param stride (2 int, 4 long)
    def is_param(s, nid): return nid > s.fn
    def frame(s, nid): return s.fs + 4 + s.psz*(nid - s.fn - 1)

def emit_binop_long(opcode, srcs, c):
    a,b=srcs[0],srcs[1]; af=c.frame(a["id"]); bf=c.frame(b["id"])   # long: lo=af, hi=af+2
    if opcode==0x13: return bs("8b46","%02x"%af,"0346","%02x"%bf,"8b5e","%02x"%(af+2),"135e","%02x"%(bf+2))
    if opcode==0x14: return bs("8b46","%02x"%af,"2b46","%02x"%bf,"8b5e","%02x"%(af+2),"1b5e","%02x"%(bf+2))
    if opcode in (0x10,0x11):   # mul/div via CX helper (call = reloc placeholder), result mov ax,bx
        return bs("8b46","%02x"%(af+2),"8b5e","%02x"%af,"8b4e","%02x"%(bf+2),"8b56","%02x"%bf,"e8 0000","8bc3")
    raise NotImplementedError("long binop %02x"%opcode)

# --- action emitters ------------------------------------------------------
def load_ax(op, c):
    if op["k"]=="const": v=op["val"]; return bs("33c0") if v==0 else bs("b8")+w16(v)
    if op["k"]=="node":
        return bs("8b46","%02x"%c.frame(op["id"])) if c.is_param(op["id"]) else bs("a1")+w16(0)
    if op["k"]=="deref":                                    # ptr is a param -> si; then [si]/[si+off]
        out=bs("8b76","%02x"%c.frame(op["id"]))
        return out + (bs("8b04") if op["off"]==0 else bs("8b44","%02x"%op["off"]))
    return b""                                              # temp already in ax

def store_ax(op, c):
    if op["k"]=="node":
        return bs("a3")+w16(0) if not c.is_param(op["id"]) else bs("8946","%02x"%c.frame(op["id"]))
    if op["k"]=="deref":
        out=bs("8b76","%02x"%c.frame(op["id"]))
        return out + (bs("8904") if op["off"]==0 else bs("8944","%02x"%op["off"]))
    raise NotImplementedError("store %s"%op)

def alu_ax(opcode, op, c):                                  # ax <op>= operand
    if op["k"]=="const": return bs("%02x"%ALU_IMM[opcode])+w16(op["val"])
    if op["k"]=="node" and c.is_param(op["id"]): return bs("%02x"%ALU_MEM[opcode],"46","%02x"%c.frame(op["id"]))
    if op["k"]=="node": return bs("%02x"%ALU_MEM[opcode],"06")+w16(0)   # ax,[global]
    raise NotImplementedError("alu operand %s"%op)

def emit_binop(opcode, srcs, c):
    a,b=srcs[0],srcs[1]
    if opcode in ALU_MEM: return load_ax(a,c)+alu_ax(opcode,b,c)
    af=c.frame(a["id"]) if a.get("k")=="node" and c.is_param(a["id"]) else None
    bf=c.frame(b["id"]) if b.get("k")=="node" and c.is_param(b["id"]) else None
    if opcode in RELJCC:
        return bs("8b46","%02x"%af,"3b46","%02x"%bf,"%02x"%RELJCC[opcode],"04","33db","eb03","33db","43","8bc3")
    if opcode==0x10: return bs("8b46","%02x"%af,"f76e","%02x"%bf)
    if opcode==0x11: return bs("8b46","%02x"%af,"99","f77e","%02x"%bf)
    if opcode==0x12: return bs("8b46","%02x"%af,"99","f77e","%02x"%bf,"8bc2")
    if opcode==0x15: return bs("8b4e","%02x"%bf,"8b46","%02x"%af,"d3e0")
    if opcode==0x16: return bs("8b4e","%02x"%bf,"8b46","%02x"%af,"d3f8")
    raise NotImplementedError("binop %02x"%opcode)

def parse_funcbegin(pl): return pl[3]|(pl[4]<<8)

INV_JCC={0x17:0x7d,0x18:0x7f,0x19:0x7e,0x1a:0x7c,0x1b:0x75,0x1c:0x74}  # if(cond) -> skip if NOT cond

def resolve(items):
    """items: ('b',bytes) | ('L',name) | ('j',opcode,name) short rel8. Two-pass."""
    pos={}; off=0
    for it in items:
        if it[0]=="b": off+=len(it[1])
        elif it[0]=="L": pos[it[1]]=off
        elif it[0]=="j": off+=2
    out=bytearray(); off=0
    for it in items:
        if it[0]=="b": out+=it[1]; off+=len(it[1])
        elif it[0]=="L": pass
        elif it[0]=="j":
            rel=pos[it[2]]-(off+2); out+=bytes([it[1], rel&0xff]); off+=2
    return bytes(out)

def model(func):
    fn=parse_funcbegin(func[0][1])
    body=func[1:]
    binops_pl=[parse_operands(pl) for op,pl in body if op in BINCLASS]
    long_mode=any(o.get("ty")==4 for ops in binops_pl for o in ops)
    has_bin=bool(binops_pl)
    fs=(4 if long_mode else 2) if has_bin else 0
    c=Ctx(fn,fs,psz=4 if long_mode else 2)
    epi=(bs("83c4","%02x"%fs) if fs else b"")+bs("5dc3")
    items=[("b", bs("55")+(bs("83ec","%02x"%fs,"3b26 0000","7703","e9 0000") if fs else b"")+bs("8bec"))]
    lstack=[]; lc=[0]; loopstack=[]
    i=0
    while i < len(body):
        op,pl=body[i]
        # peephole: x = x +/- 1  (sub/add by const 1, assigned back to same var) -> inc/dec [x]
        if op in (0x13,0x14):
            ops=parse_operands(pl)
            if (len(ops)>=2 and ops[0].get("k")=="node" and c.is_param(ops[0].get("id",-1))
                    and ops[1].get("k")=="const" and ops[1].get("val")==1
                    and i+1<len(body) and body[i+1][0]==0x00):
                ao=parse_operands(body[i+1][1])
                if len(ao)>=2 and ao[1].get("k")=="node" and ao[1].get("id")==ops[0]["id"]:
                    items.append(("b", bs("ff","46" if op==0x13 else "4e","%02x"%c.frame(ops[0]["id"]))))
                    i+=2; continue
        if op in BINCLASS and op in RELJCC and i+1<len(body) and body[i+1][0]==0x2c:
            ops=parse_operands(pl); a,b=ops[0],ops[1]
            items.append(("b", load_ax(a,c)+bs("3b46","%02x"%c.frame(b["id"]))))
            nm="L%d"%lc[0]; lc[0]+=1; lstack.append(nm); items.append(("j", INV_JCC[op], nm)); i+=2; continue
        if op in BINCLASS:
            ops=parse_operands(pl)
            items.append(("b", emit_binop_long(op,ops[:2],c) if long_mode else emit_binop(op,ops[:2],c)))
        elif op==0x00:
            o=parse_operands(pl); items.append(("b", load_ax(o[0],c)+store_ax(o[1],c)))
        elif op==0x2e:                                       # LABEL
            items.append(("L", lstack.pop()))
        elif op==0x2f:                                       # LOOP top
            nm="T%d"%lc[0]; lc[0]+=1; loopstack.append(nm); items.append(("L", nm))
        elif op==0x30:                                       # while-test: load cond; test; je exit
            o=parse_operands(pl); ex="X%d"%lc[0]; lc[0]+=1; loopstack.append(ex)
            items.append(("b", load_ax(o[0],c)+bs("85c0"))); items.append(("j",0x74,ex))
        elif op==0x31:                                       # loop-back: jmp top; then exit label
            ex=loopstack.pop(); top=loopstack.pop()
            items.append(("j",0xeb,top)); items.append(("L",ex))
        elif op==0x24:                                       # RETURN
            o=parse_operands(pl); b=b""
            if o and o[0].get("k")!="temp": b=load_ax(o[0],c)
            items.append(("b", b+epi))
        i+=1
    return resolve(items)

def main():
    quads=parse_quads(sys.argv[1])
    objf=[f for f in parse_obj_funcs(sys.argv[2]) if f[2][:1] in (b"\x55",b"\x53",b"\x8b",b"\xb8")]
    ok=0
    for i,(nm,po,code) in enumerate(objf):
        try: pred=model(quads[i]) if i<len(quads) else b""
        except Exception as e: pred=b""
        m=pred==code; ok+=m
        print("  %-7s %-5s"%(nm,"OK" if m else "FAIL")+("" if m else "  pred %s\n            orig %s"%(pred.hex(' '),code.hex(' '))))
    print("\nMC2 FORWARD MODEL: %d/%d BYTE-EXACT"%(ok,len(objf)))

if __name__=="__main__": main()
