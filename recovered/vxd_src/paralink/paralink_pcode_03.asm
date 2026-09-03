; paralink_pcode_03.asm -- PARALINK object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 04Fh, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 8]
        db  08Bh, 035h, 008h, 025h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
loc_3_0021:
        je     SHORT loc_3_004E
        db  0E8h, 086h, 002h, 0FDh, 0FFh
        je     SHORT loc_3_0032
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jmp    SHORT loc_3_0021
loc_3_0032:
        mov    esi, eax
        cmp    dword ptr [esi + 64h], 534d5446h
        je     SHORT loc_3_0058
        mov    eax, dword ptr [ebp + 0ch]
        call   sub_3_0764
        jae    SHORT loc_3_006C
        jb     SHORT loc_3_0058
        call   sub_3_0794
loc_3_004E:
        mov    eax, 0fffffff6h
        jmp    NEAR PTR loc_3_02D9
loc_3_0058:
        mov    eax, 0fffffffeh
        cmp    dword ptr [esi + 0c8h], -1
        jne    NEAR PTR loc_3_02D9
        jmp    SHORT loc_3_004E
loc_3_006C:
        xor    eax, eax
        mov    dword ptr [esi + 13ch], eax
        mov    byte ptr [esi + 136h], al
        mov    eax, dword ptr [esi + 138h]
        or     eax, eax
        jne    SHORT loc_3_00AD
        mov    edi, dword ptr [ebp + 8]
        mov    edi, dword ptr [edi]
        db  08Dh, 015h, 005h, 000h, 000h, 000h
        mov    edx, dword ptr [edx]
        cmp    edx, edi
        jne    SHORT loc_3_009C
        mov    eax, 5
        jmp    SHORT loc_3_00AD
loc_3_009C:
        db  08Dh, 015h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [edx]
        cmp    edx, edi
        jne    SHORT loc_3_010C
        mov    eax, 7
loc_3_00AD:
        db  0BFh, 018h, 025h, 000h, 000h
        mov    word ptr [edi], ax
        db  0C7h, 047h, 004h, 0C0h, 01Ch, 000h, 000h
        mov    word ptr [edi + 2], 4
        mov    dword ptr [edi + 1ch], esi
        db  0CDh, 020h, 001h, 000h, 003h, 000h
        jb     SHORT loc_3_010C
        mov    dword ptr [esi + 13ch], eax
        mov    byte ptr [esi + 136h], 10h
        db  08Bh, 01Dh, 00Ch, 025h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        and    cl, 4
        mov    byte ptr [esi + 137h], cl
        db  0CDh, 020h, 009h, 000h, 003h, 000h
        db  08Bh, 01Dh, 00Ch, 025h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 003h, 000h
        test   cl, 8
        je     SHORT loc_3_010C
        db  0CDh, 020h, 004h, 000h, 003h, 000h
loc_3_010C:
        db  083h, 03Dh, 010h, 025h, 000h, 000h, 000h
        jne    SHORT loc_3_012D
        push   esi
        db  0BEh, 0D0h, 01Ch, 000h, 000h
        mov    eax, 64h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 010h, 025h, 000h, 000h
        pop    esi
loc_3_012D:
        db  0FFh, 005h, 014h, 025h, 000h, 000h
        lea    edi, [esi + 4ch]
        xor    eax, eax
        mov    ecx, 6
        cld
        rep stosd
        mov    byte ptr [esi + 129h], al
        mov    byte ptr [esi + 3bh], al
        mov    dword ptr [esi + 120h], eax
        mov    dword ptr [esi + 0e8h], eax
        dec    eax
        mov    dword ptr [esi + 11ch], eax
        mov    dword ptr [esi + 0a0h], eax
        mov    dword ptr [esi + 0a4h], eax
        mov    dword ptr [esi + 9ch], 1
        lea    edi, [esi + 30h]
        mov    dword ptr [esi + 0e4h], edi
        lea    edi, [esi + 38h]
        mov    dword ptr [esi + 12ch], edi
        lea    edi, [esi + 88h]
        mov    dword ptr [esi + 130h], edi
        xor    eax, eax
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [esi + 0fch], eax
        mov    dword ptr [esi + 104h], eax
        mov    byte ptr [esi + 38h], 0b0h
        mov    byte ptr [esi + 12ah], 0
        mov    byte ptr [esi + 128h], 0
        mov    bl, 0
        xor    ah, ah
        xor    ecx, ecx
        cmp    ecx, dword ptr [esi + 13ch]
        db  0E8h, 020h, 001h, 0FDh, 0FFh
        jb     NEAR PTR loc_3_02F4
        mov    dword ptr [esi + 16ch], 1000h
        mov    dword ptr [esi + 170h], 7000h
        mov    dword ptr [esi + 174h], 1000h
        xor    eax, eax
        mov    dword ptr [esi + 1a0h], eax
        mov    dword ptr [esi + 1a4h], eax
        mov    dword ptr [esi + 198h], eax
        mov    dword ptr [esi + 19ch], eax
        mov    dword ptr [esi + 1a8h], eax
        mov    dword ptr [esi + 1ach], eax
        mov    dword ptr [esi + 1b0h], eax
        mov    dword ptr [esi + 1b4h], eax
        mov    dword ptr [esi + 1b8h], eax
        mov    dword ptr [esi + 1bch], eax
        mov    dword ptr [esi + 1c0h], eax
        mov    dword ptr [esi + 1c4h], eax
        mov    dword ptr [esi + 1c8h], eax
        mov    dword ptr [esi + 1cch], eax
        mov    dword ptr [esi + 1d0h], eax
        mov    dword ptr [esi + 1d4h], eax
        mov    dword ptr [esi + 1d8h], eax
        mov    dword ptr [esi + 1dch], eax
        mov    dword ptr [esi + 1e0h], eax
        mov    dword ptr [esi + 1e4h], eax
        mov    dword ptr [esi + 1e8h], eax
        mov    dword ptr [esi + 1ech], eax
        push   81h
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   1
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    dword ptr [esi + 10ch], eax
        mov    dword ptr [esi + 114h], edx
        mov    dword ptr [esi + 118h], 1000h
        push   81h
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   1
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        xor    ebx, ebx
        mov    dword ptr [esi + 110h], eax
        mov    dword ptr [esi + 3ch], edx
        mov    dword ptr [esi + 40h], 1000h
        mov    dword ptr [esi + 4ch], ebx
        mov    dword ptr [esi + 50h], edx
        mov    dword ptr [esi + 54h], edx
        xor    eax, eax
loc_3_02D9:
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx], eax
        or     esi, esi
        je     SHORT loc_3_02F4
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 140h], ebx
        mov    dword ptr [esi + 0ch], eax
        or     eax, eax
        mov    eax, esi
        je     SHORT loc_3_02F6
loc_3_02F4:
        xor    eax, eax
loc_3_02F6:
        push   eax
        db  0E8h, 0CCh, 01Ah, 0FDh, 0FFh
        pop    eax
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
loc_3_0304:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0ch], 0
        lea    esi, [esi + 94h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ch
        cld
        rep movsd
        or     al, 1
        pop    edi
        pop    esi
        pop    ecx
        leave
        ret
loc_3_032C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        db  0C7h, 005h, 099h, 027h, 000h, 000h, 043h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        call   sub_3_0794
        mov    dword ptr [esi + 0ch], 0
        mov    edx, dword ptr [esi + 0c8h]
        inc    dx
        inc    dx
        mov    al, 0ch
        out    dx, al
        db  0A1h, 010h, 025h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_037C
        db  0FFh, 00Dh, 014h, 025h, 000h, 000h
        jne    SHORT loc_3_037C
        push   esi
        mov    esi, eax
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        xor    eax, eax
        db  0A3h, 010h, 025h, 000h, 000h
loc_3_037C:
        mov    eax, dword ptr [esi + 13ch]
        or     eax, eax
        je     SHORT loc_3_03A3
        cmp    byte ptr [esi + 137h], 0
        je     SHORT loc_3_0395
        db  0CDh, 020h, 008h, 000h, 003h, 000h
loc_3_0395:
        xor    eax, eax
        xchg   dword ptr [esi + 13ch], eax
        db  0CDh, 020h, 011h, 000h, 003h, 000h
loc_3_03A3:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 10ch]
        or     eax, eax
        je     SHORT loc_3_03C4
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 10ch], eax
loc_3_03C4:
        mov    eax, dword ptr [esi + 110h]
        or     eax, eax
        je     SHORT loc_3_03E2
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 110h], eax
loc_3_03E2:
        mov    ebx, esi
        call   sub_3_0730
        push   eax
        db  0E8h, 0D9h, 019h, 0FDh, 0FFh
        pop    eax
        or     al, 1
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
loc_3_03F9:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        jecxz  loc_3_0418
        mov    eax, dword ptr [edx + 4ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 58h]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx], 0
loc_3_0418:
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        dec    eax
        pop    edx
        pop    ecx
        leave
        ret
loc_3_0422:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 12ch], ecx
        mov    al, byte ptr [edx + 38h]
        mov    byte ptr [ecx], al
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        pop    edx
        pop    ecx
        leave
        ret
loc_3_0442:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    edx, edi
        mov    ecx, 40h
        xor    eax, eax
        rep stosb
        mov    word ptr [edx], 40h
        mov    word ptr [edx + 2], 1
        mov    dword ptr [edx + 4], 1
        mov    dword ptr [edx + 18h], 2
        xor    eax, eax
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        pop    edi
        pop    edx
        pop    ecx
        leave
        ret
loc_3_047D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    edx, dword ptr [esi + 3ch]
        mov    dword ptr [esi + 4ch], ebx
        mov    dword ptr [esi + 50h], edx
        mov    dword ptr [esi + 54h], edx
        xor    eax, eax
        mov    dword ptr [esi + 0ch], eax
        inc    eax
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
loc_3_04A3:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 0e0h], eax
        mov    ecx, dword ptr [ebp + 10h]
        jecxz  loc_3_04BF
        mov    dword ptr [edx + 0e4h], ecx
loc_3_04BF:
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        pop    edx
        pop    ecx
        leave
        ret
loc_3_04C9:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [edx + 108h], ecx
        mov    ecx, dword ptr [ebp + 10h]
        inc    eax
        je     SHORT loc_3_04F8
        dec    eax
        test   byte ptr [edx + 0f0h], 2
        je     SHORT loc_3_04F8
        cmp    eax, dword ptr [edx + 48h]
        jb     SHORT loc_3_04F8
        mov    eax, dword ptr [edx + 48h]
        sub    eax, 0ah
loc_3_04F8:
        mov    dword ptr [edx + 104h], ecx
        or     ecx, ecx
        jne    SHORT loc_3_0504
        xor    eax, eax
loc_3_0504:
        mov    dword ptr [edx + 0dch], eax
        mov    dword ptr [edx + 120h], eax
        or     byte ptr [edx + 0e9h], 2
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        pop    edx
        pop    ecx
        leave
        ret
loc_3_0521:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, ecx
        inc    eax
        je     SHORT loc_3_0541
        cmp    ecx, dword ptr [esi + 40h]
        jb     SHORT loc_3_0541
        mov    ecx, dword ptr [esi + 40h]
        sub    ecx, 0ah
loc_3_0541:
        mov    ebx, dword ptr [ebp + 10h]
        or     ebx, ebx
        jne    SHORT loc_3_054B
        or     ecx, 0ffffffffh
loc_3_054B:
        mov    dword ptr [esi + 0fch], ebx
        mov    dword ptr [esi + 11ch], ecx
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 100h], ecx
        cmp    dword ptr [esi + 4ch], 0
        jne    SHORT loc_3_0570
        xor    eax, eax
        mov    edi, dword ptr [esi + 130h]
        mov    dword ptr [edi], eax
loc_3_0570:
        or     al, 1
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        leave
        ret
loc_3_0578:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [edx + 0f8h], eax
        mov    dword ptr [edx + 0f4h], ecx
        and    byte ptr [edx + 0e9h], 0bfh
        jecxz  loc_3_05C5
        mov    ecx, dword ptr [edx + 0e4h]
        mov    eax, dword ptr [edx + 0e0h]
        cli
        or     byte ptr [edx + 0e9h], 40h
        and    eax, dword ptr [ecx]
        sti
        or     eax, eax
        je     SHORT loc_3_05C5
        push   esi
        mov    esi, edx
        mov    eax, 4
        db  0E8h, 067h, 0FCh, 0FCh, 0FFh
        pop    esi
loc_3_05C5:
        xor    eax, eax
        mov    dword ptr [edx + 0ch], eax
        inc    eax
        pop    esi
        pop    edx
        pop    ecx
        leave
        ret
loc_3_05D0:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 30h
        or     ecx, ecx
        jne    NEAR PTR loc_3_0304
        mov    dword ptr [edx + 0ch], ecx
        pop    edx
        pop    ecx
        leave
        ret
loc_3_05F3:
        push   ebp
        mov    ebp, esp
        push   ecx
        push   edx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 10h], 30h
        jne    SHORT loc_3_0610
        mov    dword ptr [ebp + 10h], 0ffffffffh
        db  0E9h, 024h, 0FAh, 0FCh, 0FFh
loc_3_0610:
        xor    eax, eax
        mov    dword ptr [edx + 0ch], 0ffffffebh
        pop    edx
        pop    ecx
        leave
        ret
loc_3_061D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ecx + 34h]
        mov    dword ptr [eax], ebx
        or     al, 1
        pop    ecx
        pop    ebx
        leave
        ret
        db  000h
loc_3_0634:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_3_063D
        ret
loc_3_063D:
        push   esi
        push   edi
        push   ebx
        push   edx
        db  08Bh, 035h, 008h, 025h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_3_0667
        mov    ecx, dword ptr [esp + 20h]
loc_3_0653:
        cmp    dword ptr [eax + 0c8h], ecx
        je     NEAR PTR loc_3_0724
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_3_0653
loc_3_0667:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_3_0724
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    edi, eax
        mov    ebx, eax
        xor    eax, eax
        mov    ecx, 0a8h
        rep stosd
        mov    dword ptr [ebx], 10a0094h
        db  0C7h, 043h, 004h, 0ACh, 024h, 000h, 000h
        mov    dword ptr [ebx + 8], 17h
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [ebx + 0c8h], ecx
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ebx + 138h], ecx
        mov    edi, dword ptr [esp + 28h]
        mov    esi, edi
        xor    eax, eax
        xor    ecx, ecx
        dec    ecx
        repne scasb
        neg    ecx
        dec    ecx
        push   ecx
        push   0
        push   ecx
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    edi, eax
        pop    ecx
        or     eax, eax
        je     SHORT loc_3_0729
        mov    dword ptr [ebx + 144h], eax
        rep movsb
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 24h]
        db  0CDh, 020h, 019h, 000h, 02Bh, 000h
        add    esp, 0ch
        push   dword ptr [esp + 28h]
        db  0CDh, 020h, 021h, 000h, 02Bh, 000h
        add    esp, 4
        mov    dword ptr [ebx + 0d0h], eax
        test   eax, eax
        je     SHORT loc_3_0717
        push   dword ptr [esp + 28h]
        db  0CDh, 020h, 022h, 000h, 02Bh, 000h
        add    esp, 4
        mov    dword ptr [ebx + 0d8h], eax
loc_3_0717:
        mov    esi, ebx
        mov    edx, dword ptr [esi + 0c8h]
        db  0E8h, 0E4h, 018h, 0FDh, 0FFh
loc_3_0724:
        pop    edx
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_3_0729:
        call   sub_3_0730
        jmp    SHORT loc_3_0724
sub_3_0730:
        push   esi
        cmp    dword ptr [ebx + 144h], 0
        je     SHORT loc_3_074B
        push   0
        push   dword ptr [ebx + 144h]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_3_074B:
        db  08Bh, 035h, 008h, 025h, 000h, 000h
        mov    eax, ebx
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        pop    esi
        ret
        db  000h, 000h, 000h
sub_3_0764:
        push   edi
        mov    edi, dword ptr [esi + 0d0h]
        test   edi, edi
        je     SHORT loc_3_0792
        mov    eax, dword ptr [esi + 0d8h]
        test   eax, eax
        je     SHORT loc_3_0792
        push   1
        push   esi
        db  068h, 028h, 002h, 000h, 000h
        push   eax
        push   1
        call   edi
        add    esp, 14h
        mov    dword ptr [esi + 0d4h], eax
        cmp    eax, 1
loc_3_0792:
        pop    edi
        ret
sub_3_0794:
        mov    ecx, dword ptr [esi + 0d0h]
        jecxz  loc_3_07AE
        db  068h, 028h, 002h, 000h, 000h
        push   dword ptr [esi + 0d4h]
        push   3
        call   ecx
        add    esp, 0ch
loc_3_07AE:
        ret
        db  000h
_PTEXT ends
        end
