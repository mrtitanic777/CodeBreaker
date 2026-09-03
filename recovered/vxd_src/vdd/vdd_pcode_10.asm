; vdd_pcode_10.asm -- VDD object 10 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_10_0000:
        db  0FFh, 005h, 094h, 007h, 000h, 000h
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 34h], 0
        je     SHORT loc_10_0015
        call   dword ptr [edi + 34h]
loc_10_0015:
        pop    edi
        db  081h, 00Dh, 09Ch, 010h, 000h, 000h, 000h, 000h, 000h, 080h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  066h, 0A1h, 0EEh, 00Eh, 000h, 000h
        mov    word ptr [edi + 5e2h], ax
        call   sub_10_0238
        jb     SHORT loc_10_0042
        call   sub_10_0070
        clc
loc_10_0042:
        ret
loc_10_0043:
        call   sub_10_0050
        db  080h, 00Dh, 0C4h, 006h, 000h, 000h, 004h
        ret
sub_10_0050:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_10_0113
        clc
        ret
loc_10_005F:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  0E8h, 09Dh, 034h, 0F6h, 0FFh
        clc
        ret
        db  000h, 000h
sub_10_0070:
        push   edi
        db  0A1h, 0C8h, 006h, 000h, 000h
        mov    dword ptr [edi + 4], eax
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        cld
        je     SHORT loc_10_00A3
        db  0BEh, 0C8h, 007h, 000h, 000h
        push   esi
        lea    edi, [edi + 0ch]
        mov    ecx, 25h
        rep movsd
        pop    esi
        mov    edi, dword ptr [esp]
        lea    edi, [edi + 4afh]
        mov    ecx, 25h
        rep movsd
loc_10_00A3:
        mov    edi, dword ptr [esp]
        lea    edi, [edi + 0a0h]
        push   edi
        mov    ecx, 100h
        xor    eax, eax
loc_10_00B4:
        stosd
        inc    eax
        loop   loc_10_00B4
        pop    edi
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_10_00D5
        db  0BEh, 05Ch, 008h, 000h, 000h
        mov    ecx, 10h
loc_10_00CC:
        lodsd
        movzx  edx, al
        mov    dword ptr [edi + edx*4], eax
        loop   loc_10_00CC
loc_10_00D5:
        pop    edi
        lea    esi, [edi + 0a0h]
        mov    dword ptr [edi + 4a0h], esi
        mov    byte ptr [edi + 4a4h], 1
        mov    byte ptr [edi + 4a5h], 0
        mov    byte ptr [edi + 5e0h], 0ch
        mov    byte ptr [edi + 5e1h], 0ffh
        xor    eax, eax
        mov    dword ptr [edi + 5e4h], eax
        mov    dword ptr [edi + 600h], eax
        db  0E8h, 0ABh, 04Ah, 0F6h, 0FFh
        clc
        ret
sub_10_0113:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     NEAR PTR loc_10_01D7
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 1202h
        db  080h, 03Dh, 098h, 00Eh, 000h, 000h, 02Bh
        jne    SHORT loc_10_0151
        mov    word ptr [ebp + 1ch], 1201h
loc_10_0151:
        mov    byte ptr [ebp + 10h], 30h
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        cmp    byte ptr [ebp + 1ch], 12h
        je     SHORT loc_10_016D
        db  0C6h, 005h, 098h, 00Eh, 000h, 000h, 019h
loc_10_016D:
        mov    word ptr [ebp + 1ch], 3
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  080h, 03Dh, 098h, 00Eh, 000h, 000h, 019h
        je     SHORT loc_10_01B3
        mov    word ptr [ebp + 1ch], 1112h
        mov    byte ptr [ebp + 10h], 0
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 100h
        mov    word ptr [ebp + 18h], 407h
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_10_01B3:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        test   byte ptr [edi], 1
        je     SHORT loc_10_01D7
        mov    dword ptr [edi + 5f0h], 28h
loc_10_01D7:
        bt     dword ptr [edi + 8], 8
        ret
        db  000h, 000h, 000h
VDD_PIF_State:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_10_0208
        push   edi
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        cmp    word ptr [edi + 5e2h], ax
        je     SHORT loc_10_0206
        mov    word ptr [edi + 5e2h], ax
        db  0E8h, 0B6h, 049h, 0F6h, 0FFh
loc_10_0206:
        pop    edi
        ret
loc_10_0208:
        db  066h, 0A3h, 0EEh, 00Eh, 000h, 000h
        ret
loc_10_020F:
        db  0CDh, 020h, 000h, 000h, 00Ah, 000h
        mov    dword ptr [ebp + 1ch], eax
        movzx  edi, word ptr [ebp + 38h]
        push   edx
        push   0
        push   edi
        push   edx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        inc    eax
        je     SHORT loc_10_0237
        dec    eax
        mov    edi, eax
        add    edi, dword ptr [ebp]
        cld
        movsd
        movsd
loc_10_0237:
        ret
sub_10_0238:
        db  0A1h, 0DCh, 00Fh, 000h, 000h
        mov    word ptr [edi + 5d0h], ax
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_10_025C
        db  0A1h, 0D2h, 00Eh, 000h, 000h
        db  0E8h, 026h, 032h, 0F6h, 0FFh
        jb     NEAR PTR loc_10_031A
loc_10_025C:
        xor    eax, eax
        push   eax
        push   7
        push   eax
        push   -8
        push   10h
        push   0a0h
        push   ebx
        db  0CDh, 020h, 060h, 000h, 001h, 000h
        add    esp, 1ch
        xor    eax, eax
        push   eax
        push   7
        push   eax
        push   -8
        push   8
        push   0b8h
        push   ebx
        db  0CDh, 020h, 060h, 000h, 001h, 000h
        add    esp, 1ch
        test   byte ptr [edi + 4], 4
        je     SHORT loc_10_02AD
        xor    eax, eax
        push   eax
        push   7
        push   eax
        push   -8
        push   8
        push   0b0h
        push   ebx
        db  0CDh, 020h, 060h, 000h, 001h, 000h
        add    esp, 1ch
loc_10_02AD:
        push   edi
        mov    eax, 0a0a0a0ah
        test   byte ptr [edi + 5e2h], 40h
        jne    SHORT loc_10_02D4
        mov    eax, 4
        test   byte ptr [edi + 5e2h], 20h
        jne    SHORT loc_10_02D4
        mov    eax, 8
        or     eax, 20000h
loc_10_02D4:
        lea    edi, [edi + 543h]
        mov    ecx, 4
        xor    esi, esi
loc_10_02E1:
        movzx  edx, al
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 4], esi
        mov    word ptr [edi + 8], dx
        mov    word ptr [edi + 0ah], si
        add    edi, 0ch
        shr    eax, 8
        loop   loc_10_02E1
        pop    edi
        lea    esi, [edi + 543h]
        db  0E8h, 0C8h, 034h, 0F6h, 0FFh
        jb     SHORT loc_10_0314
        mov    byte ptr [edi + 4a6h], 1
        or     byte ptr [edi + 3], 80h
        clc
        ret
loc_10_0314:
        db  0E8h, 0ACh, 033h, 0F6h, 0FFh
        stc
loc_10_031A:
        ret
        db  000h
_PTEXT ends
        end
