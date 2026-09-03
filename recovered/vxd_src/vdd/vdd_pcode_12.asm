; vdd_pcode_12.asm -- VDD object 12 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_12_0000:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  0E8h, 06Ah, 048h, 0F4h, 0FFh
        call   sub_12_0014
        clc
        ret
sub_12_0014:
        mov    ecx, 4
sub_12_0019:
        push   esi
        lea    esi, [edi + 543h]
loc_12_0020:
        push   ecx
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_12_003D
        movzx  ecx, word ptr [esi + 0ah]
        push   100h
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_12_003D:
        add    esi, 0ch
        pop    ecx
        loop   loc_12_0020
        pop    esi
        ret
        db  000h, 000h, 000h
_PTEXT ends
        end
