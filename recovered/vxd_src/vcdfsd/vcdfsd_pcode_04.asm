; vcdfsd_pcode_04.asm -- VCDFSD object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  08Dh, 044h, 024h, 004h, 050h, 06Ah, 002h, 0A1h, 0CCh, 000h, 000h, 000h, 0FFh, 050h, 008h, 08Dh
        db  064h, 024h, 008h, 0C3h
loc_4_0014:
        db  0A1h, 0CCh, 000h, 000h, 000h
        jmp    dword ptr [eax + 18h]
loc_4_001C:
        db  0A1h, 0CCh, 000h, 000h, 000h
        jmp    dword ptr [eax + 1ch]
loc_4_0024:
        movzx  eax, byte ptr [ebp + 1ch]
        cmp    eax, 11h
        jae    SHORT loc_4_0034
        db  0FFh, 024h, 085h, 03Ch, 000h, 000h, 000h
loc_4_0034:
        mov    word ptr [ebp + 1ch], 1
        jmp    SHORT loc_4_00B2
loc_4_003C:
        add    byte ptr [eax], 0
        db  000h, 0A6h, 000h, 000h, 000h, 0ACh
        add    byte ptr [eax], al
        db  000h, 0ACh, 000h, 000h, 000h, 0ACh, 000h
        add    byte ptr [eax], al
        lodsb
        add    byte ptr [eax], al
        db  000h, 0ACh, 000h, 000h, 000h, 0ACh, 000h
        add    byte ptr [eax], al
        lodsb
        add    byte ptr [eax], al
        db  000h, 0ACh, 000h, 000h, 000h, 0ACh, 000h
        add    byte ptr [eax], al
        mov    es, word ptr [eax]
        add    byte ptr [eax], al
        pushfd
        add    byte ptr [eax], al
        db  000h, 0A6h, 000h, 000h, 000h, 0ACh
        add    byte ptr [eax], al
        db  000h, 0ACh, 000h, 000h, 000h, 0ACh, 000h
        add    byte ptr [eax], al
loc_4_0080:
        mov    word ptr [ebp + 10h], 0
        mov    word ptr [ebp + 18h], 0
        jmp    SHORT loc_4_00A6
loc_4_008E:
        mov    word ptr [ebp + 10h], 0adadh
        mov    word ptr [ebp + 1ch], 0
        jmp    SHORT loc_4_00A6
loc_4_009C:
        db  066h, 0A1h, 0C6h, 000h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
loc_4_00A6:
        and    word ptr [ebp + 2ch], 0fffeh
        ret
loc_4_00AC:
        mov    word ptr [ebp + 1ch], 0fh
loc_4_00B2:
        or     word ptr [ebp + 2ch], 1
        ret
_PTEXT ends
        end
