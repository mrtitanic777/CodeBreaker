; declicl_icode_01.asm -- DECLICL object 1 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_1_0000:
        db  083h, 03Dh, 008h, 036h, 002h, 000h, 000h
        jne    SHORT loc_1_0075
        db  0C7h, 005h, 004h, 036h, 002h, 000h, 000h, 000h, 000h, 000h
        cmp    eax, 1bh
        jne    SHORT loc_1_0022
        db  0C7h, 005h, 004h, 036h, 002h, 000h, 001h, 000h, 000h, 000h
loc_1_0022:
        db  089h, 01Dh, 000h, 036h, 002h, 000h
        db  0BAh, 0A4h, 035h, 002h, 000h
        db  089h, 015h, 0FCh, 035h, 002h, 000h
        mov    edx, dword ptr [edx + 2ch]
        db  089h, 015h, 0F8h, 035h, 002h, 000h
        xor    ecx, ecx
        db  0CDh, 020h, 000h, 000h, 0AAh, 033h
        or     ecx, ecx
        jne    SHORT loc_1_0050
        db  0CDh, 020h, 000h, 000h, 000h, 069h
        jecxz  loc_1_0082
loc_1_0050:
        db  089h, 00Dh, 0F4h, 035h, 002h, 000h
        db  0FFh, 035h, 004h, 036h, 002h, 000h
        push   ebx
        db  0FFh, 035h, 0F8h, 035h, 002h, 000h
        db  0FFh, 035h, 0F4h, 035h, 002h, 000h
        db  0E8h, 022h, 001h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_0082
loc_1_0075:
        db  0C7h, 005h, 008h, 036h, 002h, 000h, 001h, 000h, 000h, 000h
        xor    eax, eax
        ret
loc_1_0082:
        stc
        ret
_ITEXT ends
        end
