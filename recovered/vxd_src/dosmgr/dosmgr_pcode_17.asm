; dosmgr_pcode_17.asm -- DOSMGR object 17 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

DOSMGR_VM_SUSPEND:
        xor    eax, eax
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        db  039h, 005h, 014h, 003h, 000h, 000h
        je     SHORT loc_17_0049
        db  038h, 005h, 020h, 003h, 000h, 000h
        je     SHORT loc_17_0049
        db  039h, 005h, 018h, 003h, 000h, 000h
        je     SHORT loc_17_0049
        test   byte ptr [edi + ebx + 1], 1
        jne    SHORT loc_17_0033
        mov    eax, 100h
        test   byte ptr [edi + ebx + 1], 4
        je     SHORT loc_17_0049
loc_17_0033:
        push   eax
        push   0
        db  0FFh, 035h, 018h, 003h, 000h, 000h
        push   dword ptr [edi + ebx + 20h]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_17_0049:
        test   byte ptr [edi + ebx + 1], 1
        je     SHORT loc_17_006A
        cmp    dword ptr [edi + ebx + 48h], 0
        je     SHORT loc_17_006A
        push   0
        push   0
        push   1
        push   dword ptr [edi + ebx + 48h]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_17_006A:
        clc
        ret
_PTEXT ends
        end
