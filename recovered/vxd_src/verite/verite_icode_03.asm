; verite_icode_03.asm -- VERITE object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        push   ebx
        db  089h, 01Dh, 04Ch, 001h, 000h, 000h
        push   0
        push   82h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        add    esp, 8
        db  0A3h, 050h, 001h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_3_012D
        db  0CDh, 020h, 00Eh, 000h, 00Ah, 000h
        cmp    ecx, 31h
        jb     NEAR PTR loc_3_012D
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        db  0C7h, 047h, 020h, 09Fh, 000h, 000h, 000h
        db  0C7h, 047h, 024h, 0A0h, 000h, 000h, 000h
        db  0C7h, 047h, 010h, 055h, 000h, 000h, 000h
        db  0C7h, 047h, 014h, 063h, 000h, 000h, 000h
        db  0C7h, 047h, 018h, 071h, 000h, 000h, 000h
        db  0C7h, 047h, 01Ch, 09Ch, 000h, 000h, 000h
        db  0C7h, 047h, 034h, 09Dh, 000h, 000h, 000h
        db  0C7h, 047h, 038h, 09Eh, 000h, 000h, 000h
        db  0CDh, 020h, 011h, 000h, 00Ah, 000h
        db  089h, 03Dh, 058h, 001h, 000h, 000h
        db  089h, 035h, 054h, 001h, 000h, 000h
        xor    eax, eax
        db  0A3h, 048h, 001h, 000h, 000h
        db  08Dh, 005h, 030h, 001h, 000h, 000h
        mov    ecx, 32h
        db  0CDh, 020h, 014h, 000h, 00Ah, 000h
        cmp    eax, -1
        je     SHORT loc_3_0112
        push   0
        db  0FFh, 035h, 034h, 001h, 000h, 000h
        db  068h, 062h, 001h, 000h, 000h
        db  0CDh, 020h, 03Bh, 000h, 033h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_3_0112
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 062h, 001h, 000h, 000h
        dec    ecx
loc_3_00C5:
        db  08Bh, 004h, 08Dh, 064h, 001h, 000h, 000h
        test   eax, 0ffffffh
        je     SHORT loc_3_00D6
        dec    ecx
        jge    SHORT loc_3_00C5
loc_3_00D6:
        test   eax, 0ffffffh
        jne    SHORT loc_3_0112
        db  0A3h, 040h, 001h, 000h, 000h
        xor    eax, eax
        xor    ecx, ecx
        db  066h, 08Bh, 00Dh, 0BEh, 001h, 000h, 000h
        dec    ecx
loc_3_00EE:
        db  066h, 08Bh, 004h, 04Dh, 0C0h, 001h, 000h, 000h
        test   eax, 0ffh
        je     SHORT loc_3_0100
        dec    ecx
        jge    SHORT loc_3_00EE
loc_3_0100:
        test   eax, 0ffh
        jne    SHORT loc_3_0112
        db  0A3h, 03Ch, 001h, 000h, 000h
        db  0FFh, 005h, 048h, 001h, 000h, 000h
loc_3_0112:
        db  0F7h, 005h, 048h, 001h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_3_012A
        xor    eax, eax
        db  0A3h, 03Ch, 001h, 000h, 000h
        db  0A3h, 040h, 001h, 000h, 000h
loc_3_012A:
        clc
        jmp    SHORT loc_3_012E
loc_3_012D:
        stc
loc_3_012E:
        pop    ebx
        ret
loc_3_0130:
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_3_0134:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0162:
        add    byte ptr [eax], al
loc_3_0164:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_01BE:
        add    byte ptr [eax], al
loc_3_01C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
