; dosmgr_pcode_18.asm -- DOSMGR object 18 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

DOSMGR_VM_RESUME:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   byte ptr [edi + ebx + 1], 1
        je     SHORT loc_18_002B
        cmp    dword ptr [edi + ebx + 48h], 0
        je     SHORT loc_18_002B
        push   0
        push   0
        push   1
        push   dword ptr [edi + ebx + 48h]
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_18_009C
loc_18_002B:
        xor    eax, eax
        db  039h, 005h, 014h, 003h, 000h, 000h
        je     SHORT loc_18_007A
        db  038h, 005h, 020h, 003h, 000h, 000h
        je     SHORT loc_18_007A
        db  039h, 005h, 018h, 003h, 000h, 000h
        je     SHORT loc_18_007A
        bts    word ptr [edi + ebx], 9
        jae    SHORT loc_18_007A
        test   byte ptr [edi + ebx + 1], 1
        jne    SHORT loc_18_0060
        mov    eax, 100h
        test   byte ptr [edi + ebx + 1], 4
        je     SHORT loc_18_007A
loc_18_0060:
        push   eax
        push   0
        db  0FFh, 035h, 018h, 003h, 000h, 000h
        push   dword ptr [edi + ebx + 20h]
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_18_007B
loc_18_007A:
        ret
loc_18_007B:
        test   byte ptr [edi + ebx + 1], 1
        je     SHORT loc_18_009C
        cmp    dword ptr [edi + ebx + 48h], 0
        je     SHORT loc_18_009C
        push   0
        push   0
        push   1
        push   dword ptr [edi + ebx + 48h]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_18_009C:
        stc
        jmp    SHORT loc_18_007A
        db  000h
_PTEXT ends
        end
