; msodisup_icode_02.asm -- MSODISUP object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 038h, 038h, 000h, 000h
        mov    eax, dword ptr [eax + 28h]
        push   eax
        db  0E8h, 06Ch, 02Ch, 0FEh, 0FFh
        add    esp, 4
        db  0A3h, 0E4h, 038h, 000h, 000h
        push   0
        push   0
        push   0
        db  068h, 0A4h, 038h, 000h, 000h
        db  0E8h, 0DEh, 035h, 0FEh, 0FFh
        push   -1
        push   0
        push   3fh
        db  068h, 0B8h, 038h, 000h, 000h
        db  0E8h, 0BAh, 036h, 0FEh, 0FFh
        db  083h, 03Dh, 0B8h, 038h, 000h, 000h, 000h
        jne    SHORT loc_2_0050
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_2_0050:
        push   -1
        db  0A1h, 0E4h, 038h, 000h, 000h
        push   0
        movsx  ecx, word ptr [eax]
        shl    ecx, 4
        movsx  eax, byte ptr [eax + ecx + 3]
        lea    eax, [eax*4 + 4]
        push   eax
        db  068h, 0E0h, 038h, 000h, 000h
        db  0E8h, 080h, 036h, 0FEh, 0FFh
        db  083h, 03Dh, 0E0h, 038h, 000h, 000h, 000h
        je     NEAR PTR loc_2_016B
        db  0A1h, 0B8h, 038h, 000h, 000h
        mov    byte ptr [eax], 3
        db  0A1h, 0B8h, 038h, 000h, 000h
        mov    byte ptr [eax + 1], 0
        db  0A1h, 0B8h, 038h, 000h, 000h
        db  0C7h, 040h, 006h, 0A0h, 00Dh, 000h, 000h
        db  0A1h, 0B8h, 038h, 000h, 000h
        db  0C7h, 040h, 00Ah, 0B0h, 00Fh, 000h, 000h
        db  0A1h, 0B8h, 038h, 000h, 000h
        db  0C7h, 040h, 00Eh, 040h, 015h, 000h, 000h
        db  0A1h, 0B8h, 038h, 000h, 000h
        db  0C7h, 040h, 012h, 0C0h, 016h, 000h, 000h
        db  0A1h, 0B8h, 038h, 000h, 000h
        db  0C7h, 040h, 016h, 0F0h, 019h, 000h, 000h
        db  0A1h, 0B8h, 038h, 000h, 000h
        db  0C7h, 040h, 01Ah, 020h, 027h, 000h, 000h
        db  0A1h, 0B8h, 038h, 000h, 000h
        db  0C7h, 040h, 01Eh, 020h, 027h, 000h, 000h
        db  0A1h, 0B8h, 038h, 000h, 000h
        db  068h, 0A8h, 039h, 000h, 000h
        db  0C7h, 040h, 022h, 060h, 027h, 000h, 000h
        db  0A1h, 0B8h, 038h, 000h, 000h
        db  0C7h, 040h, 026h, 020h, 028h, 000h, 000h
        db  0A1h, 0B8h, 038h, 000h, 000h
        db  0C7h, 040h, 02Ah, 0D0h, 027h, 000h, 000h
        db  0A1h, 0B8h, 038h, 000h, 000h
        add    eax, 2eh
        push   eax
        db  0E8h, 0D1h, 02Ah, 0FEh, 0FFh
        add    esp, 8
        push   3fh
        db  0A1h, 0B8h, 038h, 000h, 000h
        push   eax
        push   0
        db  0A1h, 0A4h, 038h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        db  068h, 0BCh, 038h, 000h, 000h
        push   eax
        db  0E8h, 0D6h, 034h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_0183
        db  0A1h, 0E4h, 038h, 000h, 000h
        push   0
        movsx  ecx, word ptr [eax]
        shl    ecx, 4
        movsx  eax, byte ptr [eax + ecx + 3]
        lea    eax, [eax*4 + 4]
        push   eax
        db  0A1h, 0E0h, 038h, 000h, 000h
        push   eax
        db  0E8h, 091h, 035h, 0FEh, 0FFh
loc_2_016B:
        push   0
        push   3fh
        db  0A1h, 0B8h, 038h, 000h, 000h
        push   eax
        db  0E8h, 082h, 035h, 0FEh, 0FFh
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0183:
        db  0C7h, 005h, 0C4h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D8h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0DCh, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D4h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0D0h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0ACh, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0B4h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 0B0h, 038h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_2_01E0
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_2_01E0:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        sub    edi, edi
loc_2_01E7:
        db  0A1h, 0E4h, 038h, 000h, 000h
        mov    eax, dword ptr [edi + eax + 0ah]
        push   eax
        db  0E8h, 08Ah, 02Ah, 0FEh, 0FFh
        add    esp, 4
        movsx  ecx, word ptr [eax + 28h]
        db  08Bh, 015h, 070h, 038h, 000h, 000h
        sub    edx, 4
        cmp    ecx, edx
        jbe    SHORT loc_2_023B
        mov    edx, 8000h
        nop
loc_2_0210:
        db  039h, 015h, 070h, 038h, 000h, 000h
        jae    SHORT loc_2_023B
        db  0D1h, 025h, 070h, 038h, 000h, 000h
        db  08Bh, 00Dh, 070h, 038h, 000h, 000h
        db  089h, 00Dh, 08Ch, 038h, 000h, 000h
        movsx  ecx, word ptr [eax + 28h]
        db  08Bh, 01Dh, 070h, 038h, 000h, 000h
        sub    ebx, 4
        cmp    ecx, ebx
        ja     SHORT loc_2_0210
loc_2_023B:
        db  081h, 03Dh, 08Ch, 038h, 000h, 000h, 000h, 040h, 000h, 000h
        jae    SHORT loc_2_0251
        db  0C7h, 005h, 08Ch, 038h, 000h, 000h, 000h, 040h, 000h, 000h
loc_2_0251:
        add    edi, 10h
        db  0A1h, 0E4h, 038h, 000h, 000h
        inc    esi
        movsx  eax, word ptr [eax]
        cmp    eax, esi
        jae    SHORT loc_2_01E7
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0270:
        sub    eax, eax
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0280:
        stc
        ret
MSODISUP_SYS_CRITICAL_INIT:
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        ret
MSODISUP_DEVICE_INIT:
        mov    ecx, edx
        db  0E8h, 006h, 02Ah, 0FEh, 0FFh
        db  089h, 00Dh, 038h, 038h, 000h, 000h
        mov    edx, ecx
        mov    eax, dword ptr [edx + 0ch]
        db  0A3h, 03Ch, 038h, 000h, 000h
        mov    eax, dword ptr [edx + 14h]
        db  0A3h, 040h, 038h, 000h, 000h
        db  0BEh, 036h, 02Dh, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        mov    dword ptr [ecx + 0ch], eax
        db  0A3h, 048h, 038h, 000h, 000h
        db  0BEh, 0C7h, 02Dh, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        mov    dword ptr [ecx + 14h], eax
        db  0A3h, 044h, 038h, 000h, 000h
        mov    eax, dword ptr [edx + 4]
        db  0A3h, 04Ch, 038h, 000h, 000h
        mov    ecx, dword ptr [edx + 24h]
        db  0E8h, 0B8h, 029h, 0FEh, 0FFh
        db  089h, 00Dh, 054h, 038h, 000h, 000h
        cld
        call   0
        cmp    eax, 0c0000001h
        je     NEAR PTR loc_2_0280
        db  08Bh, 00Dh, 038h, 038h, 000h, 000h
        call   sub_2_0340
        or     eax, eax
        je     NEAR PTR loc_2_0280
        db  08Bh, 00Dh, 038h, 038h, 000h, 000h
        db  0BEh, 0A6h, 02Ch, 000h, 000h
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
        mov    dword ptr [ecx + 1ch], eax
        clc
        ret
MSODISUP_INIT_COMPLETE:
        push   ebx
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        jb     SHORT loc_2_032D
        db  088h, 01Dh, 0E4h, 037h, 000h, 000h
loc_2_032D:
        pop    ebx
        call   sub_2_0270
        cmp    eax, 0c0000001h
        je     NEAR PTR loc_2_0280
        clc
        ret
sub_2_0340:
        push   esi
        push   edi
        push   ebx
        push   edx
        db  0A1h, 070h, 038h, 000h, 000h
        dec    eax
        db  0A3h, 06Ch, 038h, 000h, 000h
        inc    eax
        db  0A3h, 060h, 038h, 000h, 000h
        db  083h, 02Dh, 060h, 038h, 000h, 000h, 004h
        push   208h
        push   eax
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_04D3
        db  0A3h, 074h, 038h, 000h, 000h
        db  0A1h, 08Ch, 038h, 000h, 000h
        dec    eax
        db  0A3h, 088h, 038h, 000h, 000h
        inc    eax
        db  0A3h, 07Ch, 038h, 000h, 000h
        db  083h, 02Dh, 07Ch, 038h, 000h, 000h, 004h
        push   208h
        push   eax
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_04D3
        db  0A3h, 090h, 038h, 000h, 000h
        mov    eax, 1000h
        push   208h
        push   eax
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_04D3
        db  0A3h, 078h, 038h, 000h, 000h
        xor    esi, esi
        mov    ecx, 10h
loc_2_03D4:
        mov    ebx, eax
        mov    word ptr [ebx + 3eh], si
        db  08Bh, 015h, 044h, 038h, 000h, 000h
        mov    dword ptr [ebx + 0eh], edx
        xor    edx, edx
        dec    edx
        mov    word ptr [ebx + 12h], dx
        add    eax, 4
        db  0E8h, 084h, 028h, 0FEh, 0FFh
        mov    dword ptr [ebx + 26h], edx
        db  08Bh, 015h, 074h, 038h, 000h, 000h
        shr    edx, 4
        mov    word ptr [ebx + 34h], dx
        mov    word ptr [ebx + 3ah], dx
        add    eax, 3ch
        inc    esi
        mov    dword ptr [ebx + 4], esi
        loop   loc_2_03D4
        mov    dword ptr [eax - 3ch], 0ffffffffh
        db  0A3h, 094h, 038h, 000h, 000h
        xor    esi, esi
        mov    ecx, 10h
loc_2_0422:
        mov    ebx, eax
        mov    word ptr [ebx + 3eh], si
        db  08Bh, 015h, 048h, 038h, 000h, 000h
        mov    dword ptr [ebx + 0eh], edx
        add    eax, 4
        db  0E8h, 03Dh, 028h, 0FEh, 0FFh
        mov    dword ptr [ebx + 26h], edx
        db  08Bh, 015h, 090h, 038h, 000h, 000h
        shr    edx, 4
        mov    word ptr [ebx + 34h], dx
        mov    word ptr [ebx + 3ah], dx
        add    eax, 3ch
        inc    esi
        mov    dword ptr [ebx + 4], esi
        loop   loc_2_0422
        mov    dword ptr [eax - 3ch], 0ffffffffh
        xor    esi, esi
        mov    ecx, 10h
        mov    edx, eax
        shl    edx, 0ch
        shr    dx, 0ch
        db  08Bh, 01Dh, 038h, 038h, 000h, 000h
        mov    dword ptr [ebx + 3eh], edx
        mov    ebx, dword ptr [ebx + 2eh]
        add    edx, 38h
loc_2_047C:
        mov    word ptr [eax + 3eh], si
        mov    dword ptr [eax + 0eh], ebx
        mov    word ptr [eax + 30h], 1
        mov    dword ptr [eax + 32h], edx
        mov    word ptr [eax + 36h], 4
        inc    esi
        mov    dword ptr [eax + 4], esi
        add    eax, 40h
        add    edx, 40h
        loop   loc_2_047C
        mov    dword ptr [eax - 3ch], 0ffffffffh
        mov    edi, eax
        add    eax, 100h
        shl    edi, 0ch
        mov    di, 100h
        db  089h, 03Dh, 098h, 038h, 000h, 000h
        db  0A3h, 05Ch, 038h, 000h, 000h
        db  0E8h, 0B3h, 027h, 0FEh, 0FFh
        db  089h, 015h, 058h, 038h, 000h, 000h
        mov    eax, 1
loc_2_04CE:
        pop    edx
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_2_04D3:
        xor    eax, eax
        jmp    SHORT loc_2_04CE
        db  000h
_ITEXT ends
        end
