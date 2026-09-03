; vdd_pcode_13.asm -- VDD object 13 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_13_0000:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_13_0018
        jb     SHORT loc_13_0015
        db  0E8h, 035h, 048h, 0F3h, 0FFh
        clc
loc_13_0015:
        ret
        db  000h, 000h
sub_13_0018:
        push   esi
        lea    esi, [edi + 543h]
        mov    ecx, 4
loc_13_0024:
        push   ecx
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_13_0045
        movzx  ecx, word ptr [esi + 0ah]
        push   100h
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_13_004E
loc_13_0045:
        add    esi, 0ch
        pop    ecx
        loop   loc_13_0024
        clc
        pop    esi
        ret
loc_13_004E:
        pop    ecx
        neg    ecx
        add    ecx, 4
        jecxz  loc_13_005B
        db  0E8h, 0BEh, 0FFh, 0F2h, 0FFh
loc_13_005B:
        stc
        pop    esi
        ret
        db  000h, 000h
_PTEXT ends
        end
