; vcache_icode_02.asm -- VCACHE object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        cmp    eax, 22h
        jne    SHORT loc_2_002B
        db  0BEh, 0ADh, 002h, 000h, 000h
        lodsd
loc_2_0011:
        xchg   edi, eax
        lodsd
        push   esi
        db  0BEh, 080h, 002h, 000h, 000h
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h
        pop    esi
        xchg   ebx, eax
        lodsd
        mov    dword ptr [eax], ebx
        lodsd
        test   eax, eax
        jne    SHORT loc_2_0011
        clc
        ret
loc_2_002B:
        stc
        ret
loc_2_002D:
        push   0
        db  0CDh, 020h, 05Ah, 000h, 001h, 000h
        add    esp, 4
        mov    ebx, edx
        db  089h, 01Dh, 07Ch, 002h, 000h, 000h
        db  0A1h, 0F4h, 008h, 000h, 000h
        add    eax, 3
        shr    eax, 2
        db  0A3h, 0F4h, 008h, 000h, 000h
        db  0A1h, 0F0h, 008h, 000h, 000h
        add    eax, 3
        shr    eax, 2
        jne    SHORT loc_2_0077
        mov    eax, ebx
        xor    edx, edx
        mov    ecx, 0ah
        cmp    eax, 500h
        jb     SHORT loc_2_0072
        mov    ecx, 6
loc_2_0072:
        div    ecx
        shr    eax, 2
loc_2_0077:
        sub    ebx, 180h
        cmp    ebx, 8
        jg     SHORT loc_2_0087
        mov    ebx, 8
loc_2_0087:
        cmp    eax, ebx
        jbe    SHORT loc_2_008D
        mov    eax, ebx
loc_2_008D:
        cmp    eax, 8
        ja     SHORT loc_2_0097
        mov    eax, 8
loc_2_0097:
        db  0A3h, 0F0h, 008h, 000h, 000h
        db  039h, 005h, 0F4h, 008h, 000h, 000h
        jae    SHORT loc_2_00AA
        db  089h, 01Dh, 0F4h, 008h, 000h, 000h
loc_2_00AA:
        db  039h, 01Dh, 0F4h, 008h, 000h, 000h
        jbe    SHORT loc_2_00B8
        db  089h, 01Dh, 0F4h, 008h, 000h, 000h
loc_2_00B8:
        db  0A1h, 0F4h, 008h, 000h, 000h
        cmp    eax, 32000h
        jb     SHORT loc_2_00DB
        mov    eax, 32000h
        db  0A3h, 0F4h, 008h, 000h, 000h
        db  03Bh, 005h, 0F0h, 008h, 000h, 000h
        jae    SHORT loc_2_00DB
        db  0A3h, 0F0h, 008h, 000h, 000h
loc_2_00DB:
        mov    ecx, 1
loc_2_00E0:
        add    ecx, ecx
        cmp    eax, ecx
        ja     SHORT loc_2_00E0
        shr    ecx, 1
        push   ecx
        mov    ecx, 40h
        add    eax, ecx
        dec    eax
        xor    edx, edx
        div    ecx
        db  0A3h, 0B8h, 009h, 000h, 000h
        shl    eax, 2
        pop    ecx
        mov    edi, eax
        lea    eax, [edi + ecx*8]
        push   ecx
        dec    ecx
        shl    ecx, 3
        db  089h, 00Dh, 014h, 009h, 000h, 000h
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        pop    ecx
        test   eax, eax
        je     NEAR PTR loc_2_024A
        db  0A3h, 0B0h, 009h, 000h, 000h
        add    edi, eax
        db  089h, 03Dh, 018h, 009h, 000h, 000h
        cld
loc_2_0131:
        mov    eax, edi
        stosd
        stosd
        loop   loc_2_0131
        xor    eax, eax
        push   8
        db  0FFh, 035h, 0F4h, 008h, 000h, 000h
        push   80080000h
        db  0CDh, 020h, 01Dh, 001h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_2_024A
        db  0A3h, 020h, 009h, 000h, 000h
        db  0A3h, 0FCh, 008h, 000h, 000h
        push   eax
        mov    eax, 40h
        db  03Bh, 005h, 0F0h, 008h, 000h, 000h
        jae    SHORT loc_2_0174
        db  0A1h, 0F0h, 008h, 000h, 000h
loc_2_0174:
        db  08Bh, 00Dh, 0F4h, 008h, 000h, 000h
        sub    ecx, 400h
        cmp    ecx, eax
        jg     SHORT loc_2_0186
        mov    ecx, eax
loc_2_0186:
        pop    eax
        db  03Bh, 00Dh, 0F4h, 008h, 000h, 000h
        jbe    SHORT loc_2_0195
        db  08Bh, 00Dh, 0F4h, 008h, 000h, 000h
loc_2_0195:
        cmp    ecx, 900h
        jbe    SHORT loc_2_01A2
        mov    ecx, 900h
loc_2_01A2:
        db  089h, 00Dh, 010h, 009h, 000h, 000h
        shr    eax, 0ch
        push   8
        push   0
        push   3
        push   ecx
        push   eax
        db  0CDh, 020h, 01Eh, 001h, 001h, 000h
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_2_024A
        db  08Bh, 00Dh, 010h, 009h, 000h, 000h
        mov    eax, ecx
        shr    eax, 3
        cmp    eax, 10h
        jb     SHORT loc_2_01D9
        mov    eax, 10h
loc_2_01D9:
        db  0A3h, 00Ch, 009h, 000h, 000h
        db  03Bh, 005h, 0F0h, 008h, 000h, 000h
        jbe    SHORT loc_2_01EB
        db  0A3h, 0F0h, 008h, 000h, 000h
loc_2_01EB:
        db  0A1h, 0F8h, 008h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_2_01FE
        lea    eax, [ecx + ecx*2]
        shr    eax, 1
        db  0A3h, 0F8h, 008h, 000h, 000h
loc_2_01FE:
        inc    eax
        db  0A3h, 008h, 009h, 000h, 000h
        mov    eax, ecx
        shr    eax, 1
        db  08Bh, 015h, 0FCh, 008h, 000h, 000h
        db  089h, 00Dh, 0ACh, 009h, 000h, 000h
loc_2_0214:
        db  08Bh, 03Dh, 0A4h, 009h, 000h, 000h
        mov    dword ptr [edx + 100h], edi
        db  089h, 015h, 0A4h, 009h, 000h, 000h
        lea    edx, [edx + 1000h]
        loop   loc_2_0214
        db  0BEh, 070h, 00Ah, 000h, 000h
        mov    dword ptr [esi], esi
        mov    dword ptr [esi + 4], esi
        db  0BEh, 030h, 00Ah, 000h, 000h
        mov    dword ptr [esi + 38h], esi
        mov    dword ptr [esi + 3ch], esi
        db  0E8h, 0D8h, 00Dh, 0FEh, 0FFh
        clc
        ret
loc_2_024A:
        db  0A1h, 0B0h, 009h, 000h, 000h
        test   eax, eax
        je     SHORT loc_2_0271
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 040h, 000h
        add    esp, 4
        db  0C7h, 005h, 0B0h, 009h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 018h, 009h, 000h, 000h, 000h, 000h, 000h, 000h
loc_2_0271:
        stc
        ret
loc_2_0273:
        db  0C6h, 005h, 0C5h, 009h, 000h, 000h, 001h
        clc
        ret
        db  000h, 000h, 000h, 000h
loc_2_0280:
        push   esi
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        db  000h
loc_2_0287:
        insd
        imul   ebp, dword ptr [esi + 66h], 63656c69h
        popad
        arpl   word ptr [eax + 65h], bp
        db  000h
loc_2_0294:
        insd
        popad
        db  078h, 066h
        imul   ebp, dword ptr [ebp + 63h], 65686361h
        db  000h
loc_2_02A1:
        inc    ebx
        popad
        arpl   word ptr [eax + 65h], bp
        inc    edx
        db  075h, 066h
        push   edx
        push   edx
        push   esp
        db  000h
loc_2_02AD:
        xchg   dword ptr [edx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        lock or byte ptr [eax], al
        db  000h, 094h, 002h, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
        hlt
        or     byte ptr [eax], al
        db  000h, 0A1h, 002h, 000h, 000h, 000h
        add    byte ptr [eax], al
        db  000h, 0F8h
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_ITEXT ends
        end
