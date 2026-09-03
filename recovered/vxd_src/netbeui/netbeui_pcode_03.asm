; netbeui_pcode_03.asm -- NETBEUI object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        enter  0ch, 0
        push   esi
        push   edi
        push   ebx
        call   sub_3_0208
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_001C
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_3_0104
loc_3_001C:
        push   esi
        lea    edi, [esi + 14h]
        mov    esi, dword ptr [ebp + 10h]
        movzx  ecx, word ptr [esi]
        mov    esi, dword ptr [esi + 4]
        rep movsb
        pop    esi
        or     dword ptr [esi], 200h
        db  0E8h, 0DAh, 001h, 0FDh, 0FFh
        jae    SHORT loc_3_003E
        jmp    NEAR PTR loc_3_0104
loc_3_003E:
        db  0E8h, 039h, 000h, 0FDh, 0FFh
        jae    SHORT loc_3_004F
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_3_0104
loc_3_004F:
        mov    edi, dword ptr [esi + 84h]
        mov    edx, dword ptr [esi + 31h]
        mov    dword ptr [edi + 12h], edx
        mov    dx, word ptr [esi + 35h]
        mov    word ptr [edi + 16h], dx
        push   esi
        mov    esi, dword ptr [ebp + 14h]
        or     esi, esi
        je     SHORT loc_3_00BB
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0C1h, 06Bh, 0FDh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_3_00BB
        push   0
        db  068h, 010h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 063h, 06Ch, 0FDh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        mov    eax, 0ffffffffh
        jne    SHORT loc_3_00A8
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
loc_3_00A8:
        push   dword ptr [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        db  0E8h, 04Dh, 06Ch, 0FDh, 0FFh
        mov    ebx, dword ptr [ebp - 8]
        cmp    ebx, 8
        jb     SHORT loc_3_00BD
loc_3_00BB:
        mov    bl, 0ffh
loc_3_00BD:
        mov    al, bl
        mov    byte ptr [ebp - 0ch], al
        db  0BAh, 06Bh, 06Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 18h]
        db  0CDh, 020h, 005h, 000h, 014h, 000h
        pop    esi
        jae    SHORT loc_3_00E7
        cmp    byte ptr [ebp - 0ch], 0ffh
        push   esi
        jne    SHORT loc_3_00BB
        pop    esi
loc_3_00DB:
        db  0E8h, 0D0h, 001h, 0FDh, 0FFh
        mov    eax, 0c000009ah
        jmp    SHORT loc_3_0104
loc_3_00E7:
        cmp    al, 8
        jb     SHORT loc_3_00ED
        jmp    SHORT loc_3_00DB
loc_3_00ED:
        movzx  eax, al
        db  08Dh, 01Ch, 085h, 040h, 06Fh, 000h, 000h
        mov    dword ptr [ebx], esi
        mov    dword ptr [esi], 840h
        or     dword ptr [esi], 2
        xor    eax, eax
loc_3_0104:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        test   eax, eax
        je     SHORT loc_3_0114
        push   esi
        call   sub_3_041F
        pop    ecx
loc_3_0114:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret    14h
sub_3_011B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 8
        db  0BBh, 040h, 06Fh, 000h, 000h
loc_3_012E:
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        jne    SHORT loc_3_0148
        mov    dword ptr [ebx], eax
        mov    eax, ebx
        db  02Dh, 040h, 06Fh, 000h, 000h
        shr    eax, 2
        db  0CDh, 020h, 004h, 000h, 014h, 000h
        jmp    SHORT loc_3_014D
loc_3_0148:
        add    ebx, 4
        loop   loc_3_012E
loc_3_014D:
        test   dword ptr [esi], 200h
        jne    SHORT loc_3_0163
        mov    ecx, esp
        push   esi
        db  0E8h, 014h, 015h, 0FDh, 0FFh
        pop    esi
        db  0E8h, 04Dh, 001h, 0FDh, 0FFh
loc_3_0163:
        call   sub_3_017F
        push   esi
        call   sub_3_041F
        add    esp, 4
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    dword ptr [ebx], eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret    0ch
sub_3_017F:
        push   ecx
        push   eax
loc_3_0181:
        push   eax
        mov    ecx, dword ptr [esi + 0c4h]
        mov    ecx, dword ptr [ecx]
        lea    eax, [esi + 0c4h]
        mov    dword ptr [ecx + 4], eax
        xchg   dword ptr [esi + 0c4h], ecx
        cmp    ecx, eax
        lea    ecx, [ecx - 24h]
        pop    eax
        je     SHORT loc_3_01AA
        and    byte ptr [ecx + 0b8h], 0feh
        jmp    SHORT loc_3_0181
loc_3_01AA:
        xor    eax, eax
        db  08Bh, 00Dh, 0B8h, 06Fh, 000h, 000h
loc_3_01B2:
        jecxz  loc_3_01C1
        cmp    dword ptr [ecx + 40h], esi
        jne    SHORT loc_3_01BC
        mov    dword ptr [ecx + 40h], eax
loc_3_01BC:
        mov    ecx, dword ptr [ecx + 18h]
        jmp    SHORT loc_3_01B2
loc_3_01C1:
        pop    eax
        pop    ecx
        ret
loc_3_01C4:
        enter  4, 0
        push   esi
        push   edi
        push   ebx
        db  08Bh, 035h, 038h, 06Fh, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        db  08Bh, 035h, 03Ch, 06Fh, 000h, 000h
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        push   0
        db  0FFh, 035h, 03Ah, 06Eh, 000h, 000h
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        db  0FFh, 035h, 0B4h, 06Dh, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 007h, 06Ah, 0FDh, 0FFh
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0208:
        enter  4, 0
        db  00Fh, 0B6h, 005h, 0F1h, 06Eh, 000h, 000h
        imul   eax, eax, 44h
        add    eax, 2bbh
        and    al, 0fch
        push   eax
        push   -1
        push   2
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 04Ah, 06Ah, 0FDh, 0FFh
        pop    ecx
        xor    eax, eax
        mov    edi, dword ptr [ebp - 4]
        test   edi, edi
        jne    SHORT loc_3_023B
        jmp    NEAR PTR loc_3_041D
loc_3_023B:
        shr    ecx, 2
        cld
        rep stosd
        mov    edi, dword ptr [ebp - 4]
        db  08Ah, 015h, 030h, 06Fh, 000h, 000h
        mov    byte ptr [edi + 30h], dl
        db  083h, 03Dh, 027h, 06Fh, 000h, 000h, 000h
        jne    SHORT loc_3_0264
        db  03Ah, 015h, 02Ch, 06Fh, 000h, 000h
        jbe    SHORT loc_3_0264
        db  08Ah, 015h, 02Ch, 06Fh, 000h, 000h
loc_3_0264:
        mov    byte ptr [edi + 134h], dl
        mov    byte ptr [edi + 91h], 1
        mov    byte ptr [edi + 90h], 1
        mov    dword ptr [edi + 5ch], edi
        lea    eax, [edi + 94h]
        mov    dword ptr [edi + 94h], eax
        mov    dword ptr [edi + 98h], eax
        lea    eax, [edi + 9ch]
        mov    dword ptr [edi + 9ch], eax
        mov    dword ptr [edi + 0a0h], eax
        lea    eax, [edi + 0a4h]
        mov    dword ptr [edi + 0a4h], eax
        mov    dword ptr [edi + 0a8h], eax
        lea    eax, [edi + 0bch]
        mov    dword ptr [edi + 0bch], eax
        mov    dword ptr [edi + 0c0h], eax
        lea    eax, [edi + 0c4h]
        mov    dword ptr [edi + 0c4h], eax
        mov    dword ptr [edi + 0c8h], eax
        lea    eax, [edi + 0ach]
        mov    dword ptr [edi + 0ach], eax
        mov    dword ptr [edi + 0b0h], eax
        lea    eax, [edi + 0b4h]
        mov    dword ptr [edi + 0b4h], eax
        mov    dword ptr [edi + 0b8h], eax
        mov    esi, edi
        add    edi, 138h
        mov    dword ptr [esi + 84h], edi
        mov    dword ptr [esi + 8ch], edi
        mov    dword ptr [esi + 88h], edi
        db  00Fh, 0B6h, 00Dh, 0F1h, 06Eh, 000h, 000h
        jecxz  loc_3_0330
loc_3_031C:
        mov    dword ptr [edi + 4], esi
        add    edi, 44h
        loop   loc_3_031C
        sub    edi, 44h
        mov    dword ptr [esi + 88h], edi
        add    edi, 44h
loc_3_0330:
        mov    edx, dword ptr [esi + 84h]
        mov    byte ptr [edx], 4
        mov    byte ptr [edx + 1], 2
        mov    byte ptr [edx + 2], 1
        lea    eax, [edx + 1ch]
        mov    dword ptr [edx + 1ch], eax
        mov    dword ptr [edx + 20h], eax
        lea    eax, [edx + 24h]
        mov    dword ptr [edx + 24h], eax
        mov    dword ptr [edx + 28h], eax
        lea    eax, [edx + 2ch]
        mov    dword ptr [edx + 2ch], eax
        mov    dword ptr [edx + 30h], eax
        lea    eax, [edx + 34h]
        mov    dword ptr [edx + 34h], eax
        mov    dword ptr [edx + 38h], eax
        lea    eax, [edx + 3ch]
        mov    dword ptr [edx + 3ch], eax
        mov    dword ptr [edx + 40h], eax
        xor    eax, eax
        mov    dword ptr [esi + 128h], eax
        mov    dword ptr [esi + 12ch], eax
        mov    dword ptr [esi + 78h], eax
        push   8
        db  00Fh, 0B6h, 005h, 022h, 06Fh, 000h, 000h
        push   eax
        lea    eax, [esi + 128h]
        push   eax
        lea    eax, [esi + 2ch]
        push   eax
        db  0E8h, 0E9h, 068h, 0FDh, 0FFh
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_3_03BD
        db  00Fh, 0B6h, 005h, 022h, 06Fh, 000h, 000h
        push   eax
        lea    eax, [esi + 12ch]
        push   eax
        lea    eax, [esi + 2ch]
        push   eax
        db  0E8h, 0EFh, 068h, 0FDh, 0FFh
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_3_03C8
loc_3_03BD:
        push   esi
        call   sub_3_041F
        pop    ecx
        xor    eax, eax
        jmp    SHORT loc_3_041D
loc_3_03C8:
        xor    ecx, ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_3_03D4
        jmp    SHORT loc_3_03BD
loc_3_03D4:
        mov    dword ptr [esi + 130h], eax
        mov    dword ptr [esi + 124h], 0
        mov    ecx, 6
loc_3_03E9:
        mov    eax, dword ptr [esi + 124h]
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 124h], edi
        add    edi, 40h
        loop   loc_3_03E9
        sub    esp, 8
        mov    eax, esp
        push   dword ptr [esi + 128h]
        push   eax
        add    eax, 4
        push   eax
        db  0E8h, 07Dh, 068h, 0FDh, 0FFh
        pop    eax
        pop    edx
        cmp    edx, 0
        jne    SHORT loc_3_03BD
        mov    dword ptr [esi + 78h], eax
        mov    eax, esi
loc_3_041D:
        leave
        ret
sub_3_041F:
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        je     SHORT loc_3_0480
        mov    eax, dword ptr [esi + 78h]
        test   eax, eax
        je     SHORT loc_3_0436
        push   eax
        db  0E8h, 064h, 068h, 0FDh, 0FFh
loc_3_0436:
        mov    eax, dword ptr [esi + 12ch]
        test   eax, eax
        je     SHORT loc_3_0446
        push   eax
        db  0E8h, 066h, 068h, 0FDh, 0FFh
loc_3_0446:
        mov    eax, dword ptr [esi + 128h]
        test   eax, eax
        je     SHORT loc_3_0456
        push   eax
        db  0E8h, 032h, 068h, 0FDh, 0FFh
loc_3_0456:
        mov    eax, dword ptr [esi + 130h]
        test   eax, eax
        je     SHORT loc_3_0466
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_3_0466:
        db  00Fh, 0B6h, 005h, 0F1h, 06Eh, 000h, 000h
        imul   eax, eax, 44h
        add    eax, 2bbh
        and    al, 0fch
        push   2
        push   eax
        push   esi
        db  0E8h, 0FCh, 067h, 0FDh, 0FFh
loc_3_0480:
        leave
        ret
        db  000h, 000h
_PTEXT ends
        end
