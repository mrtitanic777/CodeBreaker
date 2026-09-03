; unimodem_pcode_03_01.asm -- UNIMODEM object 3 segment _PTEXT  (part 1/2)
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        db  0CDh, 020h, 000h, 000h, 02Bh, 000h
        jae    9
        ret
        db  068h, 024h, 000h, 000h, 000h, 0CDh, 020h, 001h, 000h, 02Bh, 000h, 083h, 0C4h, 004h, 080h, 03Dh
        db  004h, 000h, 000h, 000h, 000h, 074h, 002h, 0F8h, 0C3h, 0F9h, 0C3h
loc_3_0024:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_3_002D
        ret
loc_3_002D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   dword ptr [ebp + 1ch]
        call   sub_3_42CC
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_008D
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 0ch]
        call   sub_3_44E8
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_3_008D
        mov    word ptr [esi], 94h
        mov    word ptr [esi + 2], 10ah
        db  0C7h, 046h, 004h, 050h, 006h, 000h, 000h
        mov    dword ptr [esi + 8], 17h
        db  0FEh, 005h, 004h, 000h, 000h, 000h
        lea    ebx, [esi + 9ch]
        push   ebx
        db  068h, 0D4h, 000h, 000h, 000h
        push   dword ptr [ebp + 10h]
        db  0CDh, 020h, 019h, 000h, 02Bh, 000h
        add    esp, 0ch
loc_3_008D:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_3_0092:
        push   edx
        call   sub_3_1E80
        pop    edx
        ret
loc_3_009A:
        push   edx
        call   sub_3_2C5B
        pop    edx
        ret
sub_3_00A2:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        xor    esi, esi
        xchg   dword ptr [edx + 4f4h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        leave
        ret
sub_3_00BA:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        xor    esi, esi
        xchg   dword ptr [edx + 620h], esi
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        pop    esi
        leave
        ret
        db  000h, 000h
loc_3_00D4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_42CC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_0129
        push   eax
        call   sub_3_4303
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_0130
        mov    esi, eax
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_3_0137
        add    esp, 8
        cmp    eax, -1
        jne    SHORT loc_3_0129
        sub    eax, eax
        mov    dword ptr [esi + 0ch], eax
        push   eax
        push   esi
        call   sub_3_0214
        add    esp, 4
        pop    eax
loc_3_0118:
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx], eax
        or     eax, eax
        mov    eax, esi
        je     SHORT loc_3_0125
        xor    eax, eax
loc_3_0125:
        pop    edi
        pop    esi
        leave
        ret
loc_3_0129:
        mov    eax, 0fffffffbh
        jmp    SHORT loc_3_0118
loc_3_0130:
        mov    eax, 0fffffffeh
        jmp    SHORT loc_3_0118
sub_3_0137:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 29ch]
        or     eax, eax
        jne    NEAR PTR loc_3_020D
        lea    eax, [esi + 19ch]
        push   dword ptr [ebp + 0ch]
        push   eax
        db  0CDh, 020h, 004h, 000h, 02Bh, 000h
        add    esp, 8
        cmp    eax, -14h
        jae    NEAR PTR loc_3_020D
        mov    dword ptr [esi + 29ch], eax
        mov    dword ptr [esi + 2d0h], 0ffffffffh
        mov    dword ptr [esi + 2dch], 0ffffffffh
        db  0BBh, 072h, 001h, 000h, 000h
        push   esi
        push   ebx
        push   eax
        db  0CDh, 020h, 014h, 000h, 02Bh, 000h
        add    esp, 0ch
        lea    edi, [esi + 88h]
        mov    eax, dword ptr [esi + 29ch]
        push   0
        push   edi
        push   13h
        push   eax
        db  0CDh, 020h, 00Eh, 000h, 02Bh, 000h
        add    esp, 10h
        mov    eax, dword ptr [esi + 29ch]
        mov    ebx, dword ptr [eax + 4]
        cmp    dword ptr [ebx + 1ch], 0
        je     SHORT loc_3_01E9
        lea    edi, [esi + 38h]
        mov    dword ptr [esi + 2ach], edi
        push   edi
        push   eax
        call   dword ptr [ebx + 1ch]
        add    esp, 8
        push   esi
        lea    eax, [esi + 7a2h]
        push   eax
        push   dword ptr [esi + 29ch]
        lea    eax, [esi + 640h]
        push   eax
        call   sub_3_64C0
loc_3_01E9:
        mov    eax, 34h
        add    eax, 2dh
        push   eax
        lea    eax, [esi + 54dh]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_3_54CC
        add    esp, 0ch
        mov    eax, 0ffffffffh
loc_3_0209:
        pop    edi
        pop    esi
        leave
        ret
loc_3_020D:
        mov    eax, 0
        jmp    SHORT loc_3_0209
sub_3_0214:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    dword ptr [esi + 2b8h], eax
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_3_030E
        db  0CDh, 020h, 000h, 000h, 048h, 000h
        or     eax, eax
        je     NEAR PTR loc_3_030E
        push   0
        lea    ebx, [esi + 9ch]
        push   ebx
        push   ebx
        push   0
        push   0
        push   esp
        db  0CDh, 020h, 001h, 000h, 048h, 000h
        add    esp, 18h
        or     eax, eax
        je     NEAR PTR loc_3_030E
        mov    dword ptr [esi + 2b8h], eax
        lea    ebx, [esi + 2b4h]
        push   ebx
        db  00Fh, 0B7h, 01Dh, 07Eh, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        push   ebx
        db  00Fh, 0B7h, 01Dh, 08Ah, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        push   ebx
        db  00Fh, 0B7h, 01Dh, 082h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        push   ebx
        db  00Fh, 0B7h, 01Dh, 080h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        push   ebx
        push   2
        push   0
        push   esp
        mov    eax, dword ptr [esi + 2b8h]
        push   eax
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        lea    ebx, [esi + 2b0h]
        push   ebx
        db  00Fh, 0B7h, 01Dh, 084h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        push   ebx
        db  00Fh, 0B7h, 01Dh, 08Ah, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        push   ebx
        db  00Fh, 0B7h, 01Dh, 088h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        push   ebx
        db  00Fh, 0B7h, 01Dh, 086h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        push   ebx
        push   2
        push   0
        push   esp
        mov    eax, dword ptr [esi + 2b8h]
        push   eax
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
loc_3_030E:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_3_0313:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 2b8h]
        cmp    eax, 0
        je     SHORT loc_3_0330
        push   eax
        db  0CDh, 020h, 002h, 000h, 048h, 000h
        add    esp, 4
loc_3_0330:
        pop    edi
        pop    esi
        leave
        ret
loc_3_0334:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 29ch]
        or     eax, eax
        je     NEAR PTR loc_3_03F0
        push   0
        push   dword ptr [esi + 29ch]
        db  0CDh, 020h, 00Fh, 000h, 02Bh, 000h
        add    esp, 8
        push   1
        push   dword ptr [esi + 29ch]
        db  0CDh, 020h, 00Fh, 000h, 02Bh, 000h
        add    esp, 8
        mov    eax, dword ptr [esi + 2e0h]
        cmp    eax, 7
        je     SHORT loc_3_03E1
        mov    eax, dword ptr [esi + 5eeh]
        or     eax, eax
        je     SHORT loc_3_03E1
        push   0
        push   4
        push   dword ptr [esi + 29ch]
        db  0CDh, 020h, 010h, 000h, 02Bh, 000h
        add    esp, 0ch
        mov    dword ptr [esi + 5f6h], 0
        lea    eax, [esi + 4f0h]
        push   eax
        push   dword ptr [esi + 5f2h]
        push   dword ptr [esi + 5eeh]
        push   dword ptr [esi + 29ch]
        db  0CDh, 020h, 012h, 000h, 02Bh, 000h
        add    esp, 10h
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    ebx, eax
loc_3_03C9:
        cmp    dword ptr [esi + 5f6h], 1
        je     SHORT loc_3_03E1
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        sub    eax, ebx
        cmp    eax, 3e8h
        jb     SHORT loc_3_03C9
loc_3_03E1:
        push   dword ptr [esi + 29ch]
        db  0CDh, 020h, 009h, 000h, 02Bh, 000h
        add    esp, 4
loc_3_03F0:
        push   esi
        call   sub_3_0313
        add    esp, 4
        push   esi
        call   sub_3_3F80
        add    esp, 4
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_3_0407:
        mov    ecx, 4
        db  0E9h, 04Ah, 0FFh, 0FCh, 0FFh
loc_3_0411:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 2ach], ecx
        mov    ecx, 1ch
        db  0E9h, 032h, 0FFh, 0FCh, 0FFh
loc_3_0429:
        mov    ecx, 28h
        db  0E9h, 00Ch, 0FFh, 0FCh, 0FFh
loc_3_0433:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 5b6h], ecx
        cmp    dword ptr [eax + 2e0h], 7
        jne    SHORT loc_3_0454
        mov    ecx, dword ptr [eax + 5bah]
        or     dword ptr [esp + 8], ecx
loc_3_0454:
        mov    ecx, 2ch
        db  0E9h, 0FDh, 0FEh, 0FCh, 0FFh
loc_3_045E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        cli
        mov    dword ptr [edx + 2c0h], eax
        mov    dword ptr [edx + 2bch], ecx
        and    dword ptr [edx + 2c4h], 0fffffffbh
        jecxz  loc_3_04A9
        or     dword ptr [edx + 2c4h], 4
        mov    eax, dword ptr [edx + 30h]
        and    eax, dword ptr [edx + 10h]
        je     SHORT loc_3_04A9
        sti
        push   edx
        push   dword ptr [edx + 30h]
        push   4
        push   edx
        push   dword ptr [edx + 29ch]
        db  0E8h, 0CDh, 0FCh, 0FCh, 0FFh
        add    esp, 10h
        pop    edx
loc_3_04A9:
        sti
        mov    dword ptr [edx + 0ch], 0
        mov    eax, 0ffffffffh
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_3_04BB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 29ch]
        or     eax, eax
        jne    SHORT loc_3_04D5
        mov    eax, 0
        jmp    SHORT loc_3_053B
loc_3_04D5:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 2d0h], ecx
        inc    ecx
        jecxz  loc_3_04F3
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [edx + 2c8h], ecx
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [edx + 2cch], ecx
loc_3_04F3:
        cmp    dword ptr [edx + 2e0h], 7
        je     SHORT loc_3_050C
        cmp    dword ptr [edx + 2e0h], 1ah
        je     SHORT loc_3_050C
        mov    eax, 0
        jmp    SHORT loc_3_053B
loc_3_050C:
        mov    esi, dword ptr [edx + 29ch]
        mov    edi, dword ptr [esi + 4]
        mov    eax, 40h
        mov    eax, dword ptr [eax + edi]
        push   edx
        db  068h, 072h, 001h, 000h, 000h
        push   dword ptr [ebp + 0ch]
        push   esi
        call   eax
        add    esp, 10h
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 29ch]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], ecx
loc_3_053B:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_3_0540:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 29ch]
        or     eax, eax
        jne    SHORT loc_3_055A
        mov    eax, 0
        jmp    SHORT loc_3_05C0
loc_3_055A:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 2dch], ecx
        inc    ecx
        jecxz  loc_3_0578
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [edx + 2d4h], ecx
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [edx + 2d8h], ecx
loc_3_0578:
        cmp    dword ptr [edx + 2e0h], 7
        je     SHORT loc_3_0591
        cmp    dword ptr [edx + 2e0h], 1ah
        je     SHORT loc_3_0591
        mov    eax, 0
        jmp    SHORT loc_3_05C0
loc_3_0591:
        mov    esi, dword ptr [edx + 29ch]
        mov    edi, dword ptr [esi + 4]
        mov    eax, 44h
        mov    eax, dword ptr [eax + edi]
        push   edx
        db  068h, 072h, 001h, 000h, 000h
        push   dword ptr [ebp + 0ch]
        push   esi
        call   eax
        add    esp, 10h
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 29ch]
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], ecx
loc_3_05C0:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
loc_3_05C5:
        mov    ecx, 48h
        db  0E9h, 08Ch, 0FDh, 0FCh, 0FFh
loc_3_05CF:
        mov    ecx, 54h
        db  0E9h, 082h, 0FDh, 0FCh, 0FFh
loc_3_05D9:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 0a000h
        jb     SHORT loc_3_0611
        cmp    eax, 0a00dh
        ja     SHORT loc_3_0611
        sub    eax, 0a000h
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [edx], ecx
        push   edx
        db  0FFh, 014h, 085h, 0E8h, 005h, 000h, 000h
        add    esp, 4
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [edx], eax
        mov    eax, 0ffffffffh
        ret
loc_3_0611:
        mov    ecx, 58h
        db  0E9h, 040h, 0FDh, 0FCh, 0FFh
        db  000h
sub_3_061C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        repe cmpsb
        movzx  eax, byte ptr [esi - 1]
        movzx  edx, byte ptr [edi - 1]
        sub    eax, edx
        pop    edi
        pop    esi
        leave
        ret
sub_3_063B:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 2a0h]
        or     eax, eax
        je     SHORT loc_3_0683
        mov    eax, dword ptr [edx + 61ch]
        mov    dword ptr [edx + 61ch], 0ffffffffh
        cmp    eax, -1
        jne    SHORT loc_3_0663
        mov    eax, dword ptr [ebp + 0ch]
loc_3_0663:
        push   0
        push   0
        push   dword ptr [edx + 2a8h]
        push   eax
        push   dword ptr [edx + 2a4h]
        push   dword ptr [edx + 2a0h]
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
loc_3_0683:
        cld
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    al, 27h
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_3_0695
        mov    al, 7
loc_3_0695:
        mov    byte ptr [ebp + 18h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 10h]
loc_3_06A4:
        xor    edx, edx
        div    ecx
        xchg   edx, eax
        add    al, 30h
        cmp    al, 39h
        jbe    SHORT loc_3_06B2
        add    al, byte ptr [ebp + 18h]
loc_3_06B2:
        dec    esi
        je     SHORT loc_3_06BC
        stosb
        mov    eax, edx
        or     eax, eax
        jne    SHORT loc_3_06A4
loc_3_06BC:
        mov    eax, edi
        sub    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, edi
        sub    ecx, esi
        inc    ecx
        shr    ecx, 1
        jcxz   loc_3_06E6
loc_3_06DA:
        mov    ah, byte ptr [edi]
        mov    al, byte ptr [esi]
        mov    byte ptr [esi], ah
        mov    byte ptr [edi], al
        inc    esi
        dec    edi
        loop   loc_3_06DA
loc_3_06E6:
        pop    edi
        pop    esi
        leave
        ret
        db  0CDh, 020h, 032h, 080h, 040h, 000h, 0CDh, 020h, 006h, 080h, 040h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_3_0700:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        mov    esi, 2
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ecx + 2e0h]
        cmp    eax, 3
        je     SHORT loc_3_0735
        cmp    eax, 18h
        je     SHORT loc_3_0735
        cmp    eax, 13h
        je     SHORT loc_3_0735
        cmp    eax, 4
        je     SHORT loc_3_0735
        cmp    eax, 8
        jne    NEAR PTR loc_3_07F3
loc_3_0735:
        push   0
        push   ecx
        call   sub_3_3E17
        add    esp, 8
        lea    ebx, [edi + 10h]
        lea    eax, [esp + 10h]
        push   eax
        push   ebx
        push   dword ptr [edi]
        call   sub_3_3523
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_3_07C5
        mov    esi, dword ptr [edi]
        mov    eax, dword ptr [esi + 2e0h]
        cmp    eax, 18h
        je     SHORT loc_3_077A
        cmp    eax, 13h
        je     SHORT loc_3_077A
        cmp    dword ptr [esp + 10h], 1
        sbb    eax, eax
        and    eax, 0fffffffch
        add    eax, 9
        jmp    SHORT loc_3_0787
loc_3_077A:
        cmp    dword ptr [esp + 10h], 1
        sbb    eax, eax
        and    eax, 2
        add    eax, 10h
loc_3_0787:
        mov    dword ptr [esi + 2e0h], eax
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_07AE
        db  00Fh, 0B7h, 005h, 00Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0DDh, 0FDh, 0FCh, 0FFh
        add    esp, 8
loc_3_07AE:
        push   ebp
        push   dword ptr [edi + 0ch]
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        push   esi
        call   sub_3_11F4
        add    esp, 14h
        mov    esi, eax
        jmp    SHORT loc_3_07F3
loc_3_07C5:
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 2e0h], 1
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_07F9
        push   ebx
        db  00Fh, 0B7h, 005h, 0A2h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 098h, 0FDh, 0FCh, 0FFh
        add    esp, 0ch
loc_3_07F3:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
loc_3_07F9:
        cmp    esi, 2
        jne    SHORT loc_3_0819
        or     ecx, ecx
        je     SHORT loc_3_0819
        db  00Fh, 0B7h, 005h, 0A4h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 072h, 0FDh, 0FCh, 0FFh
        add    esp, 8
loc_3_0819:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_3_0823:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   80h
        push   dword ptr [esi]
        call   sub_3_3E17
        add    esp, 8
        push   esi
        call   sub_3_0842
        add    esp, 4
        pop    esi
        ret
sub_3_0842:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 18h]
        mov    ebx, 2
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 7d3h], eax
        mov    eax, dword ptr [esi + 8]
        db  0BDh, 030h, 008h, 000h, 000h
        mov    dword ptr [ecx + 7d7h], eax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ecx + 7dbh], eax
        mov    eax, dword ptr [ecx + 2e0h]
        cmp    eax, 3
        je     SHORT loc_3_088A
        cmp    eax, 4
        jne    NEAR PTR loc_3_0957
loc_3_088A:
        cmp    word ptr [ecx + 899h], 0
        je     SHORT loc_3_08B8
        cmp    word ptr [ecx + 89bh], 80h
        jne    SHORT loc_3_08A6
        db  0BDh, 008h, 008h, 000h, 000h
        jmp    SHORT loc_3_08FA
loc_3_08A6:
        db  0BDh, 01Ch, 008h, 000h, 000h
        push   9
        push   ecx
        call   sub_3_3DE5
        add    esp, 8
        jmp    SHORT loc_3_08FA
loc_3_08B8:
        test   byte ptr [esi + 14h], 2
        je     SHORT loc_3_08D6
        push   0
        db  068h, 038h, 008h, 000h, 000h
        push   ecx
        call   sub_3_31F4
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_3_08D6
        sub    edi, edi
loc_3_08D6:
        test   byte ptr [esi + 14h], 1
        je     SHORT loc_3_08FA
        mov    dword ptr [esp + 10h], edi
        push   edi
        db  068h, 048h, 008h, 000h, 000h
        push   dword ptr [esi]
        call   sub_3_31F4
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_3_08FA
        mov    edi, dword ptr [esp + 10h]
loc_3_08FA:
        push   edi
        push   ebp
        push   dword ptr [esi]
        call   sub_3_31F4
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_3_0957
        mov    ebx, dword ptr [esi]
        mov    dword ptr [ebx + 2e0h], 4
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_0939
        db  00Fh, 0B7h, 005h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 052h, 0FCh, 0FCh, 0FFh
        add    esp, 8
loc_3_0939:
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebx + 5beh], eax
        push   edi
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   ebx
        call   sub_3_11F4
        add    esp, 14h
        mov    ebx, eax
loc_3_0957:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        cmp    ebx, 2
        jne    SHORT loc_3_097D
        or     ecx, ecx
        je     SHORT loc_3_097D
        db  00Fh, 0B7h, 005h, 0A6h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 00Eh, 0FCh, 0FCh, 0FFh
        add    esp, 8
loc_3_097D:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_3_0987:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, 2
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ecx + 2e0h]
        cmp    eax, 3
        je     SHORT loc_3_09AF
        cmp    eax, 0fh
        je     SHORT loc_3_09AF
        cmp    eax, 4
        je     SHORT loc_3_09AF
        cmp    eax, 8
        jne    SHORT loc_3_0A24
loc_3_09AF:
        push   0
        push   ecx
        call   sub_3_3E17
        add    esp, 8
        mov    eax, dword ptr [esi]
        push   0
        cmp    dword ptr [eax + 2e0h], 0fh
        jne    SHORT loc_3_09CE
        db  068h, 0E4h, 008h, 000h, 000h
        jmp    SHORT loc_3_09D3
loc_3_09CE:
        db  068h, 060h, 008h, 000h, 000h
loc_3_09D3:
        push   eax
        call   sub_3_31F4
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_3_0A24
        mov    edi, dword ptr [esi]
        mov    dword ptr [edi + 2e0h], 6
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_0A0F
        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 07Ch, 0FBh, 0FCh, 0FFh
        add    esp, 8
loc_3_0A0F:
        push   ebx
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   edi
        call   sub_3_11F4
        add    esp, 14h
        mov    edi, eax
loc_3_0A24:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        cmp    edi, 2
        jne    SHORT loc_3_0A4A
        or     ecx, ecx
        je     SHORT loc_3_0A4A
        db  00Fh, 0B7h, 005h, 0A8h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 041h, 0FBh, 0FCh, 0FFh
        add    esp, 8
loc_3_0A4A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_0A50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    ebx, 2
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ecx + 2e0h]
        cmp    eax, 1
        je     SHORT loc_3_0A78
        cmp    eax, 3
        je     SHORT loc_3_0A78
        cmp    eax, 4
        jne    NEAR PTR loc_3_0C09
loc_3_0A78:
        push   0
        push   ecx
        call   sub_3_3E17
        add    esp, 8
        mov    eax, dword ptr [esi]
        push   eax
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 4]
        add    eax, 7a2h
        push   0
        push   dword ptr [eax + 0a7h]
        push   eax
        call   sub_3_7ED0
        push   1
        mov    eax, dword ptr [esi]
        add    eax, 640h
        push   eax
        call   sub_3_64E3
        mov    edi, dword ptr [esi]
        add    edi, 6fah
        sub    eax, eax
        mov    ecx, 26h
        rep stosd
        mov    eax, dword ptr [esi]
        push   0
        mov    byte ptr [eax + 7fbh], 0
        mov    dword ptr [eax + 7f3h], 10000h
        mov    dword ptr [eax + 7f7h], 10000h
        mov    dword ptr [eax + 6fah], 10h
        mov    dword ptr [eax + 7c7h], 0
        db  068h, 068h, 008h, 000h, 000h
        push   eax
        call   sub_3_31F4
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_3_0C09
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 5c2h], 0
        jne    SHORT loc_3_0B69
        push   eax
        call   sub_3_5651
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_0B5D
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 618h], 0
        je     SHORT loc_3_0B5D
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0B4D
        db  00Fh, 0B7h, 00Dh, 09Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 03Eh, 0FAh, 0FCh, 0FFh
        add    esp, 8
loc_3_0B4D:
        push   0
        push   ebp
        db  0E8h, 07Bh, 0F9h, 0FCh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_3_0C0E
loc_3_0B5D:
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 5c2h], 1
loc_3_0B69:
        push   ebp
        db  068h, 0B8h, 00Ah, 000h, 000h
        push   dword ptr [esi]
        call   sub_3_31F4
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_3_0BFE
        mov    eax, dword ptr [esi]
        push   0
        mov    dword ptr [eax + 5bah], 0
        push   0
        push   dword ptr [eax + 29ch]
        db  0E8h, 0F6h, 0F8h, 0FCh, 0FFh
        add    esp, 0ch
        mov    ebx, dword ptr [esi]
        mov    dword ptr [ebx + 2e0h], 2
        mov    dword ptr [ebx + 5a5h], 0
        mov    dword ptr [ebx + 5a9h], 0
        mov    dword ptr [ebx + 5cah], 0
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0BE7
        db  00Fh, 0B7h, 00Dh, 012h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 0A4h, 0F9h, 0FCh, 0FFh
        add    esp, 8
loc_3_0BE7:
        push   edi
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   ebx
        call   sub_3_11F4
        add    esp, 14h
        mov    ebx, eax
        jmp    SHORT loc_3_0C09
loc_3_0BFE:
        push   0
        push   ebp
        db  0E8h, 0CAh, 0F8h, 0FCh, 0FFh
        add    esp, 8
loc_3_0C09:
        cmp    ebx, 2
        jne    SHORT loc_3_0C2E
loc_3_0C0E:
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0C2E
        db  00Fh, 0B7h, 00Dh, 0A0h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 05Dh, 0F9h, 0FCh, 0FFh
        add    esp, 8
loc_3_0C2E:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_0C35:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, 2
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0C61
        db  00Fh, 0B7h, 00Dh, 014h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 02Ah, 0F9h, 0FCh, 0FFh
        add    esp, 8
loc_3_0C61:
        mov    edi, dword ptr [esp + 30h]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ecx + 61ch], 0
        mov    eax, dword ptr [ecx + 2e0h]
        lea    edx, [ecx + 2e0h]
        cmp    eax, 0ah
        je     NEAR PTR loc_3_0FDB
        cmp    eax, 0bh
        je     NEAR PTR loc_3_0FDB
        cmp    eax, 0ch
        je     NEAR PTR loc_3_0FDB
        cmp    eax, 0dh
        je     NEAR PTR loc_3_0FDB
        cmp    eax, 1ah
        je     NEAR PTR loc_3_0FC1
        cmp    eax, 17h
        je     NEAR PTR loc_3_0FBC
        cmp    eax, 10h
        je     NEAR PTR loc_3_0F5F
        cmp    eax, 0eh
        je     NEAR PTR loc_3_0F5F
        push   ecx
        call   sub_3_00A2
        add    esp, 4
        mov    eax, dword ptr [edi]
        push   0
        mov    dword ptr [eax + 5bah], 0
        push   0
        push   dword ptr [eax + 29ch]
        db  0E8h, 0A7h, 0F7h, 0FCh, 0FFh
        add    esp, 0ch
        push   dword ptr [edi]
        call   sub_3_00BA
        add    esp, 4
        push   1
        mov    eax, dword ptr [edi]
        add    eax, 640h
        push   eax
        call   sub_3_64E3
        push   0
        push   0
        mov    eax, dword ptr [edi]
        push   -1
        push   dword ptr [eax + 29ch]
        db  0E8h, 0A8h, 0F7h, 0FCh, 0FFh
        add    esp, 10h
        mov    ebx, dword ptr [edi]
        push   0
        push   dword ptr [ebx + 29ch]
        db  0E8h, 05Eh, 0F7h, 0FCh, 0FFh
        add    esp, 8
        mov    eax, dword ptr [ebx + 5e2h]
        or     eax, eax
        je     SHORT loc_3_0D56
        cmp    dword ptr [ebx + 5deh], eax
        je     SHORT loc_3_0D4A
        push   0
        push   eax
        db  0E8h, 089h, 0F7h, 0FCh, 0FFh
        add    esp, 8
loc_3_0D4A:
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 5e2h], 0
loc_3_0D56:
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        jne    SHORT loc_3_0D6B
        mov    eax, 2
        jmp    NEAR PTR loc_3_0FE0
loc_3_0D6B:
        mov    ebx, dword ptr [edi]
        mov    dword ptr [ebx + 5d2h], 1
        mov    eax, dword ptr [ebx + 2e0h]
        lea    ebp, [ebx + 2e0h]
        dec    eax
        cmp    eax, 12h
        ja     SHORT loc_3_0D90
        db  0FFh, 024h, 085h, 0E8h, 00Fh, 000h, 000h
loc_3_0D90:
        mov    dword ptr [ebp], 1
        sub    esi, esi
        jmp    NEAR PTR loc_3_0F1C
loc_3_0D9E:
        sub    esi, esi
        jmp    NEAR PTR loc_3_0F1C
loc_3_0DA5:
        cmp    dword ptr [ebx + 618h], 0
        je     NEAR PTR loc_3_0E59
        push   0
        push   1
        db  068h, 0FCh, 008h, 000h, 000h
        call   sub_3_33D7
        lea    eax, [esp + 20h]
        add    esp, 0ch
        push   eax
        push   1
        mov    eax, dword ptr [edi]
        db  068h, 000h, 009h, 000h, 000h
        push   dword ptr [eax + 29ch]
        db  0E8h, 0C4h, 0F6h, 0FCh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_3_0DF4
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 2e0h], 1
        jmp    NEAR PTR loc_3_0F1C
loc_3_0DF4:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_0E09
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [esp + 14h]
        add    dword ptr [eax + 2b4h], ecx
        jmp    SHORT loc_3_0E15
loc_3_0E09:
        push   0
        push   dword ptr [edi]
        call   sub_3_3349
        add    esp, 8
loc_3_0E15:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 2a0h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ecx + 2a4h], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    esi, 1
        mov    dword ptr [ecx + 2a8h], eax
        mov    dword ptr [ecx + 4ech], 1
        mov    dword ptr [ecx + 2e0h], 0ch
        push   ecx
        call   sub_3_1E80
        add    esp, 4
        jmp    NEAR PTR loc_3_0F1C
loc_3_0E59:
        mov    dword ptr [ebp], 3
        sub    esi, esi
        jmp    NEAR PTR loc_3_0F1C
loc_3_0E67:
        cmp    dword ptr [ebx + 618h], 0
        jne    NEAR PTR loc_3_0EFA
        sub    esi, esi
        jmp    NEAR PTR loc_3_0F1C
loc_3_0E7B:
        mov    dword ptr [ebp], 3
        sub    esi, esi
        jmp    NEAR PTR loc_3_0F1C
loc_3_0E89:
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0EA9
        db  00Fh, 0B7h, 00Dh, 02Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 0E2h, 0F6h, 0FCh, 0FFh
        add    esp, 8
loc_3_0EA9:
        push   0
        push   0
        push   6
        push   dword ptr [ebx + 29ch]
        db  0E8h, 0C6h, 0F5h, 0FCh, 0FFh
        mov    eax, dword ptr [esp + 20h]
        add    esp, 10h
        mov    dword ptr [ebx + 2a0h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 2a4h], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    esi, 1
        mov    dword ptr [ebx + 2a8h], eax
        mov    dword ptr [ebp], 0bh
        mov    dword ptr [ebx + 4ech], 0fh
        push   ebx
        call   sub_3_1E80
        add    esp, 4
        jmp    SHORT loc_3_0F1C
loc_3_0EFA:
        mov    dword ptr [ebp], 0dh
        push   dword ptr [ebx + 5deh]
        push   dword ptr [edi + 0ch]
        push   dword ptr [edi + 8]
        push   dword ptr [esp + 1ch]
        push   ebx
        call   sub_3_11F4
        add    esp, 14h
        mov    esi, eax
loc_3_0F1C:
        lea    eax, [esp + 18h]
        push   eax
        mov    eax, dword ptr [edi]
        push   0
        push   dword ptr [eax + 29ch]
        db  0E8h, 040h, 0F5h, 0FCh, 0FFh
        add    esp, 0ch
        cmp    esi, 2
        jne    SHORT loc_3_0F58
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0F58
        db  00Fh, 0B7h, 00Dh, 0AAh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 033h, 0F6h, 0FCh, 0FFh
        add    esp, 8
loc_3_0F58:
        mov    eax, esi
        jmp    NEAR PTR loc_3_0FE0
loc_3_0F5F:
        mov    dword ptr [edx], 19h
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 7c7h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ecx + 7cbh], eax
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ecx + 7cfh], eax
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [ecx + 29ch]
        db  0E8h, 0D8h, 0F4h, 0FCh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_0FDB
        cmp    dword ptr [esp + 28h], 0
        jne    SHORT loc_3_0FDB
        lea    eax, [esp + 1ch]
        push   eax
        push   1
        mov    eax, dword ptr [edi]
        db  068h, 0F8h, 008h, 000h, 000h
        push   dword ptr [eax + 29ch]
        db  0E8h, 0E9h, 0F4h, 0FCh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_3_0FDB
loc_3_0FBC:
        cmp    eax, 1ah
        jne    SHORT loc_3_0FCE
loc_3_0FC1:
        cmp    dword ptr [ecx + 7fch], 1
        jne    SHORT loc_3_0FCE
        sub    eax, eax
        jmp    SHORT loc_3_0FE0
loc_3_0FCE:
        push   4
        push   edi
        call   sub_3_7341
        add    esp, 8
        jmp    SHORT loc_3_0FE0
loc_3_0FDB:
        mov    eax, 1
loc_3_0FE0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  09Eh, 00Dh, 000h, 000h, 0A5h, 00Dh, 000h, 000h, 067h, 00Eh, 000h, 000h, 07Bh, 00Eh, 000h, 000h
        db  0A5h, 00Dh, 000h, 000h, 0A5h, 00Dh, 000h, 000h, 089h, 00Eh, 000h, 000h, 0FAh, 00Eh, 000h, 000h
        db  0A5h, 00Dh, 000h, 000h, 090h, 00Dh, 000h, 000h, 090h, 00Dh, 000h, 000h, 090h, 00Dh, 000h, 000h
        db  090h, 00Dh, 000h, 000h, 0FAh, 00Eh, 000h, 000h, 0FAh, 00Eh, 000h, 000h, 0A5h, 00Dh, 000h, 000h
        db  0FAh, 00Eh, 000h, 000h, 0A5h, 00Dh, 000h, 000h, 0FAh, 00Eh, 000h, 000h
sub_3_1034:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 2e0h], 1ah
        jne    SHORT loc_3_1052
        mov    eax, 2
        jmp    NEAR PTR loc_3_118E
loc_3_1052:
        push   2
        push   eax
        call   sub_3_75C1
        add    esp, 8
        push   dword ptr [esi]
        call   sub_3_00A2
        add    esp, 4
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 1
        je     SHORT loc_3_1082
        cmp    eax, 2
        je     SHORT loc_3_10DC
        cmp    eax, 3
        je     NEAR PTR loc_3_1163
        jmp    NEAR PTR loc_3_118C
loc_3_1082:
        mov    esi, dword ptr [esi]
        push   esi
        mov    dword ptr [esi + 2e0h], 7
        mov    edi, dword ptr [esi + 29ch]
        db  068h, 072h, 001h, 000h, 000h
        push   dword ptr [esi + 2d0h]
        push   edi
        db  0E8h, 012h, 0F4h, 0FCh, 0FFh
        add    esp, 10h
        push   esi
        db  068h, 072h, 001h, 000h, 000h
        push   dword ptr [esi + 2dch]
        push   edi
        db  0E8h, 005h, 0F4h, 0FCh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 5bah], 80h
        push   0
        mov    eax, dword ptr [esi + 5b6h]
        or     eax, 80h
        push   eax
        push   edi
        jmp    NEAR PTR loc_3_1184
loc_3_10DC:
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 2e0h], 7
        je     SHORT loc_3_10F1
        mov    eax, 2
        jmp    NEAR PTR loc_3_118E
loc_3_10F1:
        push   1
        add    eax, 640h
        push   eax
        call   sub_3_64E3
        push   0
        push   0
        mov    eax, dword ptr [esi]
        push   -1
        push   dword ptr [eax + 29ch]
        db  0E8h, 0AFh, 0F3h, 0FCh, 0FFh
        add    esp, 10h
        mov    eax, dword ptr [esi]
        push   0
        mov    dword ptr [eax + 5bah], 0
        push   0
        push   dword ptr [eax + 29ch]
        db  0E8h, 061h, 0F3h, 0FCh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [esi]
        push   0
        mov    edi, dword ptr [eax + 29ch]
        push   edi
        db  0E8h, 046h, 0F3h, 0FCh, 0FFh
        lea    eax, [esp + 10h]
        add    esp, 8
        push   eax
        push   0
        push   edi
        db  0E8h, 01Eh, 0F3h, 0FCh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 2e0h], 1
        jmp    SHORT loc_3_118C
loc_3_1163:
        mov    eax, dword ptr [esi]
        push   0
        mov    dword ptr [eax + 5bah], 0b0h
        mov    ecx, dword ptr [eax + 5b6h]
        or     ecx, 0b0h
        push   ecx
        push   dword ptr [eax + 29ch]
loc_3_1184:
        db  0E8h, 007h, 0F3h, 0FCh, 0FFh
        add    esp, 0ch
loc_3_118C:
        sub    eax, eax
loc_3_118E:
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_3_1194:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [eax + 2e0h], 7
        jne    SHORT loc_3_11B0
        mov    eax, dword ptr [eax + 5a9h]
        mov    dword ptr [ecx + 0ch], eax
        sub    eax, eax
        jmp    SHORT loc_3_11BC
loc_3_11B0:
        mov    dword ptr [ecx + 0ch], 0
        mov    eax, 2
loc_3_11BC:
        ret
loc_3_11BD:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [ecx + 2e0h]
        sub    eax, 7
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 5a9h]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 2b0h]
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [ecx + 2b4h]
        mov    dword ptr [edx + 10h], eax
        sub    eax, eax
        ret
sub_3_11F4:
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        or     eax, eax
        mov    esi, dword ptr [esp + 10h]
        jne    SHORT loc_3_1214
        cmp    word ptr [esi + 899h], 0
        jne    SHORT loc_3_1214
        jmp    NEAR PTR loc_3_1302
loc_3_1214:
        mov    dword ptr [esi + 2a0h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 2a4h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 2a8h], eax
        mov    eax, dword ptr [esp + 20h]
        or     eax, eax
        je     NEAR PTR loc_3_1302
        mov    dword ptr [esi + 5e2h], eax
        mov    dword ptr [esi + 5e6h], eax
        push   esi
        call   sub_3_00A2
        add    esp, 4
        lea    edi, [esi + 640h]
        push   0
        push   0
        push   -1
        push   edi
        call   sub_3_687B
        push   0
        push   0
        push   -1
        push   dword ptr [esi + 29ch]
        db  0E8h, 04Eh, 0F2h, 0FCh, 0FFh
        lea    eax, [esp + 18h]
        add    esp, 10h
        push   eax
        push   0
        push   dword ptr [esi + 29ch]
        db  0E8h, 0E9h, 0F1h, 0FCh, 0FFh
        add    esp, 0ch
        push   0
        push   dword ptr [esi + 29ch]
        db  0E8h, 0F1h, 0F1h, 0FCh, 0FFh
        add    esp, 8
        push   edi
        call   sub_3_6847
        cmp    dword ptr [esi + 2e0h], 0dh
        je     SHORT loc_3_12BD
        push   0
        push   0
        push   5
        push   dword ptr [esi + 29ch]
        db  0E8h, 0C6h, 0F1h, 0FCh, 0FFh
        add    esp, 10h
loc_3_12BD:
        mov    dword ptr [esi + 5ceh], 1
        push   esi
        call   sub_3_130D
        add    esp, 4
        mov    edi, eax
        cmp    edi, 1
        je     SHORT loc_3_12FE
        mov    eax, dword ptr [esi + 5e2h]
        or     eax, eax
        je     SHORT loc_3_12FE
        cmp    dword ptr [esi + 5deh], eax
        je     SHORT loc_3_12F4
        push   0
        push   eax
        db  0E8h, 0DFh, 0F1h, 0FCh, 0FFh
        add    esp, 8
loc_3_12F4:
        mov    dword ptr [esi + 5e2h], 0
loc_3_12FE:
        mov    eax, edi
        jmp    SHORT loc_3_1307
loc_3_1302:
        mov    eax, 2
loc_3_1307:
        pop    edi
        pop    esi
        add    esp, 4
        ret
sub_3_130D:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   1
        push   100h
        db  0E8h, 0ABh, 0F1h, 0FCh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_3_1330
        mov    eax, 2
        jmp    NEAR PTR loc_3_151F
loc_3_1330:
        mov    ebx, dword ptr [esp + 14h]
        cmp    dword ptr [ebx + 5e2h], 0
        je     NEAR PTR loc_3_1512
        mov    edi, dword ptr [ebx + 5e6h]
        cmp    byte ptr [edi], 0
        je     NEAR PTR loc_3_1512
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 3e8h], ecx
        mov    eax, dword ptr [ebx + 5e6h]
        mov    dword ptr [ebx + 5eah], eax
        add    ecx, eax
        inc    ecx
        mov    dword ptr [ebx + 5e6h], ecx
        lea    esi, [ebx + 2e8h]
        push   0
        push   0
        lea    eax, [ebx + 3e8h]
        push   eax
        push   esi
        push   ebp
        call   sub_3_2F1C
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_3_13D6
        mov    dword ptr [ebx + 2e0h], 1
        push   0
        push   ebp
        db  0E8h, 007h, 0F1h, 0FCh, 0FFh
        add    esp, 8
        mov    eax, 2
        jmp    NEAR PTR loc_3_151F
loc_3_13D6:
        mov    ecx, esi
        db  0BAh, 07Ch, 008h, 000h, 000h
loc_3_13DD:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    SHORT loc_3_13FD
        or     al, al
        je     SHORT loc_3_13F9
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    SHORT loc_3_13FD
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    SHORT loc_3_13DD
loc_3_13F9:
        sub    eax, eax
        jmp    SHORT loc_3_1402
loc_3_13FD:
        sbb    eax, eax
        sbb    eax, -1
loc_3_1402:
        or     eax, eax
        je     NEAR PTR loc_3_149C
        mov    eax, dword ptr [ebx + 2e0h]
        cmp    eax, 5
        je     SHORT loc_3_141F
        cmp    eax, 9
        mov    eax, 0
        jne    SHORT loc_3_1424
loc_3_141F:
        mov    eax, 1
loc_3_1424:
        push   eax
        push   dword ptr [ebx + 3e8h]
        push   esi
        call   sub_3_33D7
        add    esp, 0ch
        lea    eax, [ebx + 640h]
        push   eax
        call   sub_3_6847
        lea    eax, [ebx + 3e8h]
        push   eax
        push   dword ptr [ebx + 3e8h]
        push   esi
        push   dword ptr [ebx + 29ch]
        db  0E8h, 047h, 0F0h, 0FCh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_3_147F
        mov    dword ptr [ebx + 2e0h], 1
        push   0
        push   ebp
        db  0E8h, 05Eh, 0F0h, 0FCh, 0FFh
        add    esp, 8
        mov    eax, 2
        jmp    NEAR PTR loc_3_151F
loc_3_147F:
        mov    eax, dword ptr [ebx + 3e8h]
        or     eax, eax
        je     SHORT loc_3_1491
        add    dword ptr [ebx + 2b4h], eax
        jmp    SHORT loc_3_149C
loc_3_1491:
        push   0
        push   ebx
        call   sub_3_3349
        add    esp, 8
loc_3_149C:
        mov    eax, dword ptr [ebx + 5e6h]
        db  0BAh, 070h, 008h, 000h, 000h
loc_3_14A7:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_3_14C7
        or     cl, cl
        je     SHORT loc_3_14C3
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_3_14C7
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_3_14A7
loc_3_14C3:
        sub    eax, eax
        jmp    SHORT loc_3_14CC
loc_3_14C7:
        sbb    eax, eax
        sbb    eax, -1
loc_3_14CC:
        or     eax, eax
        jne    SHORT loc_3_14ED
        push   0
        push   ebx
        call   sub_3_1524
        add    esp, 8
        push   0
        push   ebp
        db  0E8h, 0EDh, 0EFh, 0FCh, 0FFh
        add    esp, 8
        mov    eax, 1
        jmp    SHORT loc_3_151F
loc_3_14ED:
        mov    dword ptr [ebx + 4ech], 1
        push   ebx
        call   sub_3_1E80
        add    esp, 4
        push   0
        push   ebp
        db  0E8h, 0C8h, 0EFh, 0FCh, 0FFh
        add    esp, 8
        mov    eax, 1
        jmp    SHORT loc_3_151F
loc_3_1512:
        push   0
        push   ebp
        db  0E8h, 0B6h, 0EFh, 0FCh, 0FFh
        add    esp, 8
        sub    eax, eax
loc_3_151F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_1524:
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 48h]
        mov    dword ptr [esi + 4ech], 0eh
        mov    eax, dword ptr [esi + 2e0h]
        sub    eax, 2
        cmp    eax, 17h
        ja     SHORT loc_3_154D
        db  0FFh, 024h, 085h, 0ECh, 01Dh, 000h, 000h
loc_3_154D:
        mov    edi, 2
        mov    dword ptr [esi + 2e0h], 1
        jmp    NEAR PTR loc_3_1CBA
loc_3_1561:
        mov    edi, dword ptr [esp + 4ch]
        or     edi, edi
        je     SHORT loc_3_1587
        cmp    edi, 4
        jb     SHORT loc_3_1573
        cmp    edi, 7
        jbe    SHORT loc_3_1596
loc_3_1573:
        mov    edi, 2
        mov    dword ptr [esi + 2e0h], 1
        jmp    NEAR PTR loc_3_1CBA
loc_3_1587:
        mov    dword ptr [esi + 2e0h], 3
        jmp    NEAR PTR loc_3_1CBA
loc_3_1596:
        mov    eax, dword ptr [esi + 5ceh]
        cmp    eax, 5
        jae    SHORT loc_3_1573
        inc    eax
        mov    dword ptr [esi + 5ceh], eax
        mov    eax, dword ptr [esi + 5eah]
        mov    dword ptr [esi + 5e6h], eax
        push   esi
        call   sub_3_130D
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_15D4
        cmp    eax, 1
        je     NEAR PTR loc_3_1DE5
        cmp    eax, 2
        je     SHORT loc_3_15E3
        jmp    NEAR PTR loc_3_1CBA
loc_3_15D4:
        mov    dword ptr [esi + 2e0h], 3
        jmp    NEAR PTR loc_3_1CBA
loc_3_15E3:
        mov    dword ptr [esi + 2e0h], 1
        jmp    NEAR PTR loc_3_1CBA
loc_3_15F2:
        mov    edi, dword ptr [esp + 4ch]
        or     edi, edi
        jne    SHORT loc_3_1634
        cmp    dword ptr [esi + 5beh], 0
        je     SHORT loc_3_1625
        push   edi
        push   esi
        call   sub_3_063B
        add    esp, 8
        mov    dword ptr [esi + 4ech], 2
        push   esi
        call   sub_3_1E80
        add    esp, 4
        jmp    NEAR PTR loc_3_1DE5
loc_3_1625:
        mov    dword ptr [esi + 2e0h], 3
        jmp    NEAR PTR loc_3_1CBA
loc_3_1634:
        mov    edi, 2
        mov    dword ptr [esi + 2e0h], 1
        jmp    NEAR PTR loc_3_1CBA
loc_3_1648:
        mov    edi, dword ptr [esp + 4ch]
        or     edi, edi
        je     SHORT loc_3_166E
        cmp    edi, 4
        jb     SHORT loc_3_165A
        cmp    edi, 7
        jbe    SHORT loc_3_167D
loc_3_165A:
        mov    edi, 2
        mov    dword ptr [esi + 2e0h], 1
        jmp    NEAR PTR loc_3_1CBA
loc_3_166E:
        mov    dword ptr [esi + 2e0h], 8
        jmp    NEAR PTR loc_3_1CBA
loc_3_167D:
        mov    dword ptr [esi + 2e0h], 3
        jmp    NEAR PTR loc_3_1CBA
loc_3_168C:
        mov    edi, dword ptr [esp + 4ch]
        or     edi, edi
        je     SHORT loc_3_16B6
        cmp    edi, 4
        jb     SHORT loc_3_16A2
        cmp    edi, 7
        jbe    NEAR PTR loc_3_1895
loc_3_16A2:
        mov    edi, 2
        mov    dword ptr [esi + 2e0h], 1
        jmp    NEAR PTR loc_3_1CBA
loc_3_16B6:
        mov    dword ptr [esi + 2e0h], 7
        cmp    dword ptr [esi + 5a9h], 0
        jne    NEAR PTR loc_3_1756
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esi + 29ch]
        db  0E8h, 083h, 0EDh, 0FCh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_16F8
        mov    dword ptr [esi + 2e0h], 1
        mov    edi, 2
        jmp    NEAR PTR loc_3_1CBA
loc_3_16F8:
        mov    eax, dword ptr [esi + 5cah]
        or     eax, eax
        je     SHORT loc_3_174C
        mov    dword ptr [esi + 5a9h], eax
        cmp    dword ptr [esp + 18h], eax
        je     SHORT loc_3_1756
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 14h]
        push   eax
        call   sub_3_34E7
        add    esp, 4
        lea    eax, [esp + 14h]
        push   -1
        push   eax
        push   dword ptr [esi + 29ch]
        db  0E8h, 027h, 0EDh, 0FCh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_1756
        mov    dword ptr [esi + 2e0h], 1
        mov    edi, 2
        jmp    NEAR PTR loc_3_1CBA
loc_3_174C:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 5a9h], eax
loc_3_1756:
        mov    dword ptr [esi + 5bah], 0b0h
        push   0
        mov    eax, dword ptr [esi + 5b6h]
        or     eax, 0b0h
        push   eax
        push   dword ptr [esi + 29ch]
        db  0E8h, 017h, 0EDh, 0FCh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [esi + 5a9h]
        or     eax, eax
        je     SHORT loc_3_17AA
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_17F9
        push   eax
        db  00Fh, 0B7h, 005h, 072h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0E3h, 0EDh, 0FCh, 0FFh
        add    esp, 0ch
        jmp    SHORT loc_3_17F3
loc_3_17AA:
        mov    eax, dword ptr [esi + 5cah]
        or     eax, eax
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        je     SHORT loc_3_17D8
        or     ecx, ecx
        je     SHORT loc_3_17F9
        push   eax
        db  00Fh, 0B7h, 005h, 072h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0B5h, 0EDh, 0FCh, 0FFh
        add    esp, 0ch
        jmp    SHORT loc_3_17F3
loc_3_17D8:
        or     ecx, ecx
        je     SHORT loc_3_17F9
        db  00Fh, 0B7h, 005h, 070h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 098h, 0EDh, 0FCh, 0FFh
        add    esp, 8
loc_3_17F3:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
loc_3_17F9:
        mov    ebx, dword ptr [esi + 5a5h]
        test   bl, 2
        je     SHORT loc_3_1823
        test   bl, 8
        je     SHORT loc_3_1816
        or     ecx, ecx
        je     SHORT loc_3_1844
        db  00Fh, 0B7h, 005h, 076h, 000h, 000h, 000h
        jmp    SHORT loc_3_182E
loc_3_1816:
        or     ecx, ecx
        je     SHORT loc_3_1844
        db  00Fh, 0B7h, 005h, 078h, 000h, 000h, 000h
        jmp    SHORT loc_3_182E
loc_3_1823:
        or     ecx, ecx
        je     SHORT loc_3_1844
        db  00Fh, 0B7h, 005h, 074h, 000h, 000h, 000h
loc_3_182E:
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 04Dh, 0EDh, 0FCh, 0FFh
        add    esp, 8
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
loc_3_1844:
        test   bl, 1
        je     SHORT loc_3_185A
        or     ecx, ecx
        je     NEAR PTR loc_3_1CBA
        db  00Fh, 0B7h, 005h, 07Ah, 000h, 000h, 000h
        jmp    SHORT loc_3_1869
loc_3_185A:
        or     ecx, ecx
        je     NEAR PTR loc_3_1CBA
        db  00Fh, 0B7h, 005h, 07Ch, 000h, 000h, 000h
loc_3_1869:
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 012h, 0EDh, 0FCh, 0FFh
        add    esp, 8
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     NEAR PTR loc_3_1CBA
        push   ecx
        db  0E8h, 003h, 0EDh, 0FCh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_3_1CBA
loc_3_1895:
        mov    dword ptr [esi + 2e0h], 3
        jmp    NEAR PTR loc_3_1CBA
loc_3_18A4:
        mov    edi, dword ptr [esp + 4ch]
        or     edi, edi
        je     SHORT loc_3_18C2
        cmp    edi, 4
        jb     SHORT loc_3_18B6
        cmp    edi, 7
        jbe    SHORT loc_3_18C2
loc_3_18B6:
        mov    dword ptr [esi + 2e0h], 1
        jmp    SHORT loc_3_18CC
loc_3_18C2:
        mov    dword ptr [esi + 2e0h], 3
loc_3_18CC:
        mov    eax, dword ptr [esi + 2a8h]
        and    eax, 0ffffh
        mov    dword ptr [esi + 2a8h], eax
        or     eax, 0ffff0000h
        mov    dword ptr [esi + 2a8h], eax
        mov    edi, 3
        jmp    NEAR PTR loc_3_1CBA
loc_3_18F2:
        mov    eax, dword ptr [esi + 2ach]
        mov    al, byte ptr [eax]
        test   al, 80h
        je     NEAR PTR loc_3_19CF
        test   al, 20h
        je     NEAR PTR loc_3_19CF
        cmp    dword ptr [esi + 618h], 0
        je     NEAR PTR loc_3_19BB
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_1938
        db  00Fh, 0B7h, 005h, 0C8h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 053h, 0ECh, 0FCh, 0FFh
        add    esp, 8
loc_3_1938:
        push   0
        push   3
        db  068h, 084h, 008h, 000h, 000h
        call   sub_3_33D7
        lea    eax, [esp + 18h]
        add    esp, 0ch
        push   eax
        push   3
        db  068h, 084h, 008h, 000h, 000h
        push   dword ptr [esi + 29ch]
        db  0E8h, 040h, 0EBh, 0FCh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_3_197B
        mov    dword ptr [esi + 2e0h], 1
        mov    edi, 2
        jmp    NEAR PTR loc_3_1CBA
loc_3_197B:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_3_198E
        mov    eax, dword ptr [esp + 0ch]
        add    dword ptr [esi + 2b4h], eax
        jmp    SHORT loc_3_1999
loc_3_198E:
        push   0
        push   esi
        call   sub_3_3349
        add    esp, 8
loc_3_1999:
        mov    dword ptr [esi + 4ech], 1
        mov    dword ptr [esi + 2e0h], 0ch
        push   esi
        call   sub_3_1E80
        add    esp, 4
        jmp    NEAR PTR loc_3_1DE5
loc_3_19BB:
        mov    dword ptr [esi + 2e0h], 1
        mov    edi, 2
        jmp    NEAR PTR loc_3_1CBA
loc_3_19CF:
        push   0
        push   0
        push   5
        push   dword ptr [esi + 29ch]
        db  0E8h, 0A0h, 0EAh, 0FCh, 0FFh
        add    esp, 10h
        cmp    dword ptr [esi + 618h], 0
        je     SHORT loc_3_1A0E
        mov    dword ptr [esi + 4ech], 2
        mov    dword ptr [esi + 2e0h], 0ch
        push   esi
        call   sub_3_1E80
        add    esp, 4
        jmp    NEAR PTR loc_3_1DE5
loc_3_1A0E:
        mov    dword ptr [esi + 2e0h], 3
        sub    edi, edi
        jmp    NEAR PTR loc_3_1CBA
loc_3_1A1F:
        mov    edi, dword ptr [esp + 4ch]
        cmp    edi, 2
        je     SHORT loc_3_1A85
        mov    dword ptr [esi + 2e0h], 0dh
        mov    eax, dword ptr [esi + 5e2h]
        or     eax, eax
        je     SHORT loc_3_1A59
        cmp    dword ptr [esi + 5deh], eax
        je     SHORT loc_3_1A4F
        push   0
        push   eax
        db  0E8h, 084h, 0EAh, 0FCh, 0FFh
        add    esp, 8
loc_3_1A4F:
        mov    dword ptr [esi + 5e2h], 0
loc_3_1A59:
        push   dword ptr [esi + 5deh]
        push   dword ptr [esi + 2a8h]
        push   dword ptr [esi + 2a4h]
        push   dword ptr [esi + 2a0h]
        push   esi
        call   sub_3_11F4
        add    esp, 14h
        mov    edi, eax
        cmp    edi, 1
        je     NEAR PTR loc_3_1DE5
loc_3_1A85:
        or     edi, edi
        jne    SHORT loc_3_1A98
        mov    dword ptr [esi + 2e0h], 3
        jmp    NEAR PTR loc_3_1CBA
loc_3_1A98:
        mov    dword ptr [esi + 2e0h], 1
        jmp    NEAR PTR loc_3_1CBA
loc_3_1AA7:
        mov    edi, dword ptr [esp + 4ch]
        cmp    edi, 2
        je     SHORT loc_3_1AEC
        push   0
        push   0
        push   5
        push   dword ptr [esi + 29ch]
        db  0E8h, 0BFh, 0E9h, 0FCh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 2e0h], 3
        push   0
        push   edi
        push   esi
        call   sub_3_7475
        add    esp, 0ch
        push   2
        push   esi
        call   sub_3_75C1
        add    esp, 8
        sub    edi, edi
        jmp    NEAR PTR loc_3_1CBA
loc_3_1AEC:
        mov    eax, dword ptr [esi + 5d2h]
        cmp    eax, 3
        jae    SHORT loc_3_1B55
        inc    eax
        mov    dword ptr [esi + 5d2h], eax
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_1B1F
        db  00Fh, 0B7h, 005h, 02Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 06Ch, 0EAh, 0FCh, 0FFh
        add    esp, 8
loc_3_1B1F:
        push   0
        push   0
        push   6
        push   dword ptr [esi + 29ch]
        db  0E8h, 050h, 0E9h, 0FCh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 2e0h], 0bh
        mov    dword ptr [esi + 4ech], 0fh
        push   esi
        call   sub_3_1E80
        add    esp, 4
        jmp    NEAR PTR loc_3_1DE5
loc_3_1B55:
        mov    dword ptr [esi + 2e0h], 1
        jmp    NEAR PTR loc_3_1CBA
loc_3_1B64:
        mov    edi, dword ptr [esp + 4ch]
        or     edi, edi
        je     SHORT loc_3_1B8A
        cmp    edi, 4
        jb     SHORT loc_3_1B76
        cmp    edi, 7
        jbe    SHORT loc_3_1B99
loc_3_1B76:
        mov    edi, 2
        mov    dword ptr [esi + 2e0h], 1
        jmp    NEAR PTR loc_3_1CBA
loc_3_1B8A:
        mov    dword ptr [esi + 2e0h], 0fh
        jmp    NEAR PTR loc_3_1CBA
loc_3_1B99:
        mov    dword ptr [esi + 2e0h], 3
        jmp    NEAR PTR loc_3_1CBA
loc_3_1BA8:
        mov    edi, dword ptr [esp + 4ch]
        or     edi, edi
        je     SHORT loc_3_1BC4
        mov    edi, 2
        mov    dword ptr [esi + 2e0h], 1
        jmp    NEAR PTR loc_3_1CBA
loc_3_1BC4:
        mov    dword ptr [esi + 2e0h], 18h
        jmp    NEAR PTR loc_3_1CBA
loc_3_1BD3:
        mov    edi, dword ptr [esp + 4ch]
        or     edi, edi
        je     SHORT loc_3_1BF9
        cmp    edi, 4
        jb     SHORT loc_3_1BE5
        cmp    edi, 7
        jbe    SHORT loc_3_1C08
loc_3_1BE5:
        mov    edi, 2
        mov    dword ptr [esi + 2e0h], 1
        jmp    NEAR PTR loc_3_1CBA
loc_3_1BF9:
        mov    dword ptr [esi + 2e0h], 13h
        jmp    NEAR PTR loc_3_1CBA
loc_3_1C08:
        mov    dword ptr [esi + 2e0h], 3
        jmp    NEAR PTR loc_3_1CBA
loc_3_1C17:
        mov    dword ptr [esi + 2e0h], 15h
        mov    edi, dword ptr [esp + 4ch]
        jmp    NEAR PTR loc_3_1CBA
loc_3_1C2A:
        push   1
        push   dword ptr [esp + 50h]
        push   esi
        call   sub_3_7475
        add    esp, 0ch
        jmp    NEAR PTR loc_3_1DE5
loc_3_1C3E:
        push   dword ptr [esi + 810h]
        db  0E8h, 0FBh, 0E8h, 0FCh, 0FFh
        mov    dword ptr [esi + 810h], 0
        mov    dword ptr [esi + 2e0h], 0dh
        mov    eax, dword ptr [esi + 5e2h]
        or     eax, eax
        je     SHORT loc_3_1C84
        cmp    dword ptr [esi + 5deh], eax
        je     SHORT loc_3_1C7A
        push   0
        push   eax
        db  0E8h, 059h, 0E8h, 0FCh, 0FFh
        add    esp, 8
loc_3_1C7A:
        mov    dword ptr [esi + 5e2h], 0
loc_3_1C84:
        push   dword ptr [esi + 5deh]
        push   dword ptr [esi + 7cfh]
        push   dword ptr [esi + 7cbh]
        push   dword ptr [esi + 7c7h]
        push   esi
        call   sub_3_11F4
        add    esp, 14h
        mov    edi, eax
        mov    dword ptr [esi + 7c7h], 0
        cmp    edi, 1
        je     NEAR PTR loc_3_1DE5
loc_3_1CBA:
        mov    ecx, dword ptr [esi + 5d6h]
        or     ecx, ecx
        je     SHORT loc_3_1D10
        mov    eax, dword ptr [esi + 5dah]
        or     eax, eax
        je     SHORT loc_3_1D10
        mov    dword ptr [esi + 2e0h], ecx
        mov    dword ptr [esi + 5e2h], 0
        push   eax
        push   dword ptr [esi + 2a8h]
        push   dword ptr [esi + 2a4h]
        push   dword ptr [esi + 2a0h]
        push   esi
        call   sub_3_11F4
        add    esp, 14h
        mov    edi, eax
        mov    dword ptr [esi + 5dah], 0
        cmp    edi, 1
        jne    SHORT loc_3_1D24
        jmp    NEAR PTR loc_3_1DE5
loc_3_1D10:
        mov    dword ptr [esi + 5dah], 0
        mov    dword ptr [esi + 5d6h], 0
loc_3_1D24:
        mov    eax, dword ptr [esi + 5e2h]
        or     eax, eax
        je     SHORT loc_3_1D4B
        cmp    dword ptr [esi + 5deh], eax
        je     SHORT loc_3_1D41
        push   0
        push   eax
        db  0E8h, 092h, 0E7h, 0FCh, 0FFh
        add    esp, 8
loc_3_1D41:
        mov    dword ptr [esi + 5e2h], 0
loc_3_1D4B:
        lea    eax, [esp + 10h]
        push   eax
        push   0
        push   dword ptr [esi + 29ch]
        db  0E8h, 013h, 0E7h, 0FCh, 0FFh
        add    esp, 0ch
        cmp    dword ptr [esi + 2e0h], 7
        jne    SHORT loc_3_1D93
        mov    ebx, dword ptr [esi + 29ch]
        push   esi
        db  068h, 072h, 001h, 000h, 000h
        push   dword ptr [esi + 2d0h]
        push   ebx
        db  0E8h, 037h, 0E7h, 0FCh, 0FFh
        add    esp, 10h
        push   esi
        db  068h, 072h, 001h, 000h, 000h
        push   dword ptr [esi + 2dch]
        push   ebx
        jmp    SHORT loc_3_1DB1
loc_3_1D93:
        push   0
        push   0
        push   -1
        lea    eax, [esi + 640h]
        push   eax
        call   sub_3_687B
        push   0
        push   0
        push   -1
        push   dword ptr [esi + 29ch]
loc_3_1DB1:
        db  0E8h, 00Ah, 0E7h, 0FCh, 0FFh
        add    esp, 10h
        cmp    dword ptr [esi + 2e0h], 15h
        jne    SHORT loc_3_1DDB
        mov    dword ptr [esi + 2e0h], 16h
        push   edi
        add    esi, 7dfh
        push   esi
        db  0E8h, 093h, 0E9h, 0FCh, 0FFh
        jmp    SHORT loc_3_1DE5
loc_3_1DDB:
        push   edi
        push   esi
        call   sub_3_063B
        add    esp, 8
loc_3_1DE5:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  061h, 015h, 000h, 000h, 04Dh, 015h, 000h, 000h, 0F2h, 015h, 000h, 000h, 048h, 016h, 000h, 000h
        db  08Ch, 016h, 000h, 000h, 04Dh, 015h, 000h, 000h, 04Dh, 015h, 000h, 000h, 08Ch, 016h, 000h, 000h
        db  0A4h, 018h, 000h, 000h, 0F2h, 018h, 000h, 000h, 01Fh, 01Ah, 000h, 000h, 0A7h, 01Ah, 000h, 000h
        db  064h, 01Bh, 000h, 000h, 04Dh, 015h, 000h, 000h, 064h, 01Bh, 000h, 000h, 0A8h, 01Bh, 000h, 000h
        db  0D3h, 01Bh, 000h, 000h, 04Dh, 015h, 000h, 000h, 017h, 01Ch, 000h, 000h, 04Dh, 015h, 000h, 000h
        db  04Dh, 015h, 000h, 000h, 02Ah, 01Ch, 000h, 000h, 04Dh, 015h, 000h, 000h, 03Eh, 01Ch, 000h, 000h
loc_3_1E4C:
        sub    esp, 8
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 810h], 0
        lea    eax, [esp + 4]
        push   eax
        push   1
        db  068h, 004h, 009h, 000h, 000h
        mov    eax, dword ptr [esp + 0ch]
        push   dword ptr [eax + 29ch]
        db  0E8h, 024h, 0E6h, 0FCh, 0FFh
        add    esp, 18h
        ret
sub_3_1E80:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 34h]
loc_3_1E8B:
        cmp    dword ptr [ebx + 620h], 0
        je     SHORT loc_3_1ECD
        push   ebx
        call   sub_3_00A2
        add    esp, 4
        lea    eax, [ebx + 640h]
        push   0
        push   0
        push   -1
        push   eax
        call   sub_3_687B
        push   0
        push   0
        push   -1
        push   dword ptr [ebx + 29ch]
        db  0E8h, 000h, 0E6h, 0FCh, 0FFh
        add    esp, 10h
        mov    dword ptr [ebx + 620h], 0
loc_3_1ECD:
        mov    eax, dword ptr [ebx + 4ech]
        dec    eax
        cmp    eax, 0fh
        ja     NEAR PTR loc_3_2B38
        db  0FFh, 024h, 085h, 04Bh, 02Bh, 000h, 000h
loc_3_1EE4:
        mov    dword ptr [ebx + 4ech], 2
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [ebx + 29ch]
        db  0E8h, 06Ah, 0E5h, 0FCh, 0FFh
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_3_2B38
        cmp    dword ptr [esp + 2ch], 0
        je     NEAR PTR loc_3_25D2
        push   ebx
        db  068h, 054h, 000h, 000h, 000h
        push   1
        push   dword ptr [ebx + 29ch]
        db  0E8h, 099h, 0E5h, 0FCh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_3_2729
        jmp    NEAR PTR loc_3_2B38
loc_3_1F37:
        mov    eax, dword ptr [ebx + 4f0h]
        cmp    eax, 100h
        jb     SHORT loc_3_1F53
        mov    dword ptr [ebx + 4ech], 0ah
        jmp    NEAR PTR loc_3_1ECD
loc_3_1F53:
        lea    edx, [ebx + 640h]
        lea    ecx, [esp + 14h]
        push   ecx
        push   1
        add    eax, ebx
        add    eax, 3ech
        push   eax
        push   edx
        call   sub_3_67E7
        or     eax, eax
        je     NEAR PTR loc_3_2B38
        mov    eax, dword ptr [esp + 14h]
        add    dword ptr [ebx + 2b0h], eax
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_1FA0
        mov    eax, dword ptr [esp + 14h]
        add    dword ptr [ebx + 4f0h], eax
        mov    dword ptr [ebx + 4ech], 7
        jmp    NEAR PTR loc_3_1ECD
loc_3_1FA0:
        mov    dword ptr [ebx + 4ech], 5
        jmp    NEAR PTR loc_3_1ECD
loc_3_1FAF:
        lea    esi, [ebx + 640h]
        push   0
        push   0
        push   -1
        push   esi
        call   sub_3_687B
        or     eax, eax
        je     NEAR PTR loc_3_2B38
        lea    eax, [esp + 10h]
        push   eax
        push   1
        mov    eax, dword ptr [ebx + 4f0h]
        add    eax, ebx
        add    eax, 3ech
        push   eax
        push   esi
        call   sub_3_67E7
        or     eax, eax
        je     NEAR PTR loc_3_2B38
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [ebx + 2b0h], eax
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_2016
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [ebx + 4f0h], eax
        mov    dword ptr [ebx + 4ech], 7
        jmp    NEAR PTR loc_3_1ECD
loc_3_2016:
        cmp    dword ptr [ebx + 608h], 0
        je     SHORT loc_3_202E
        mov    dword ptr [ebx + 4ech], 0ch
        jmp    NEAR PTR loc_3_1ECD
loc_3_202E:
        cmp    dword ptr [ebx + 2e0h], 0ch
        je     NEAR PTR loc_3_2811
        mov    dword ptr [ebx + 4ech], 0ah
        jmp    NEAR PTR loc_3_1ECD
loc_3_204A:
        cmp    dword ptr [ebx + 614h], 0
        je     SHORT loc_3_2062
        mov    dword ptr [ebx + 4ech], 4
        jmp    NEAR PTR loc_3_1ECD
loc_3_2062:
        cmp    byte ptr [ebx + 840h], 0
        je     NEAR PTR loc_3_20FA
        mov    dword ptr [ebx + 4ech], 4
        mov    esi, dword ptr [ebx + 4f0h]
        mov    edx, dword ptr [ebx + 796h]
        mov    eax, esi
        sub    eax, dword ptr [ebx + 841h]
        cmp    eax, edx
        jb     NEAR PTR loc_3_1ECD
        mov    eax, ebx
        sub    eax, edx
        mov    ecx, edx
        shr    ecx, 2
        lea    esi, [eax + esi + 3ech]
        mov    edi, dword ptr [ebx + 792h]
        repe cmpsd
        jne    NEAR PTR loc_3_1ECD
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb
        jne    NEAR PTR loc_3_1ECD
        mov    dword ptr [ebx + 4ech], 0bh
        mov    al, byte ptr [ebx + 840h]
        and    al, 7fh
        mov    byte ptr [esp + 18h], al
        mov    byte ptr [ebx + 840h], 0
        mov    eax, dword ptr [ebx + 4f0h]
        sub    eax, dword ptr [ebx + 841h]
        sub    eax, dword ptr [ebx + 796h]
        mov    dword ptr [ebx + 845h], eax
        jmp    NEAR PTR loc_3_1ECD
loc_3_20FA:
        lea    eax, [esp + 18h]
        push   eax
        push   ebx
        call   sub_3_2E0A
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_2129
        cmp    eax, 1
        je     SHORT loc_3_2164
        cmp    eax, 2
        je     SHORT loc_3_218A
        cmp    eax, 3
        je     SHORT loc_3_2199
        cmp    eax, 4
        je     NEAR PTR loc_3_21EF
        jmp    NEAR PTR loc_3_2B38
loc_3_2129:
        test   byte ptr [esp + 18h], 80h
        je     SHORT loc_3_2155
        mov    al, byte ptr [esp + 18h]
        mov    byte ptr [ebx + 840h], al
        mov    eax, dword ptr [ebx + 4f0h]
        mov    dword ptr [ebx + 841h], eax
        mov    dword ptr [ebx + 4ech], 4
        jmp    NEAR PTR loc_3_1ECD
loc_3_2155:
        mov    dword ptr [ebx + 4ech], 0bh
        jmp    NEAR PTR loc_3_1ECD
loc_3_2164:
        push   2
        push   dword ptr [ebx + 4f0h]
        lea    eax, [ebx + 3ech]
        push   eax
        call   sub_3_33D7
        add    esp, 0ch
        mov    dword ptr [ebx + 4ech], 2
        jmp    NEAR PTR loc_3_1ECD
loc_3_218A:
        mov    dword ptr [ebx + 4ech], 4
        jmp    NEAR PTR loc_3_1ECD
loc_3_2199:
        cmp    dword ptr [ebx + 608h], 0
        je     SHORT loc_3_21B1
        mov    dword ptr [ebx + 4ech], 0ch
        jmp    NEAR PTR loc_3_1ECD
loc_3_21B1:
        cmp    dword ptr [ebx + 2e0h], 4
        jne    SHORT loc_3_21E0
        push   2
        push   dword ptr [ebx + 4f0h]
        lea    eax, [ebx + 3ech]
        push   eax
        call   sub_3_33D7
        add    esp, 0ch
        mov    dword ptr [ebx + 4ech], 2
        jmp    NEAR PTR loc_3_1ECD
loc_3_21E0:
        mov    dword ptr [ebx + 4ech], 9
        jmp    NEAR PTR loc_3_1ECD
loc_3_21EF:
        mov    dword ptr [ebx + 4ech], 4
        lea    esi, [esp + 18h]
        lea    edi, [ebx + 5feh]
        movsd
        movsd
        movsw
        mov    eax, dword ptr [ebx + 4f0h]
        mov    dword ptr [ebx + 608h], eax
        jmp    NEAR PTR loc_3_1ECD
loc_3_2218:
        mov    dword ptr [ebx + 608h], 0
        mov    dword ptr [ebx + 614h], 1
        mov    dword ptr [ebx + 4ech], 4
        jmp    NEAR PTR loc_3_1ECD
loc_3_223B:
        cmp    dword ptr [ebx + 2e0h], 2
        jne    SHORT loc_3_2253
        mov    eax, dword ptr [ebx + 2ach]
        test   byte ptr [eax], 80h
        jne    NEAR PTR loc_3_267B
loc_3_2253:
        cmp    dword ptr [ebx + 5ceh], 5
        push   2
        push   dword ptr [ebx + 4f0h]
        lea    eax, [ebx + 3ech]
        push   eax
        jae    NEAR PTR loc_3_2901
        call   sub_3_33D7
        add    esp, 0ch
        db  083h, 03Dh, 000h, 00Ch, 000h, 000h, 000h
        je     SHORT loc_3_229C
        db  00Fh, 0B7h, 005h, 0C4h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0FFh, 035h, 000h, 00Ch, 000h, 000h
        db  0E8h, 0EFh, 0E2h, 0FCh, 0FFh
        add    esp, 8
loc_3_229C:
        inc    dword ptr [ebx + 5ceh]
        mov    dword ptr [ebx + 4ech], 8
        jmp    NEAR PTR loc_3_1ECD
loc_3_22B1:
        push   2
        push   dword ptr [ebx + 4f0h]
        lea    eax, [ebx + 3ech]
        push   eax
        call   sub_3_33D7
        add    esp, 0ch
        mov    dword ptr [ebx + 4ech], 0dh
        jmp    NEAR PTR loc_3_1ECD
loc_3_22D7:
        lea    esi, [ebx + 5feh]
        lea    edi, [esp + 18h]
        movsd
        movsd
        movsw
        mov    eax, dword ptr [ebx + 608h]
        lea    edi, [ebx + eax + 3ech]
        mov    eax, dword ptr [ebx + 4f0h]
        lea    ebp, [ebx + eax + 3ebh]
        cmp    ebp, edi
        jb     SHORT loc_3_2318
        lea    esi, [ebx + 640h]
loc_3_2309:
        mov    al, byte ptr [ebp]
        push   eax
        push   esi
        call   sub_3_67B3
        dec    ebp
        cmp    ebp, edi
        jae    SHORT loc_3_2309
loc_3_2318:
        push   2
        push   dword ptr [ebx + 608h]
        lea    eax, [ebx + 3ech]
        push   eax
        call   sub_3_33D7
        add    esp, 0ch
        mov    dword ptr [ebx + 4ech], 0dh
        jmp    NEAR PTR loc_3_1ECD
loc_3_233E:
        db  083h, 03Dh, 000h, 00Ch, 000h, 000h, 000h
        je     SHORT loc_3_238E
        cmp    byte ptr [esp + 18h], 0
        jae    SHORT loc_3_235E
        cmp    byte ptr [esp + 18h], 1ah
        jbe    SHORT loc_3_235E
        db  00Fh, 0B7h, 005h, 038h, 000h, 000h, 000h
        jmp    SHORT loc_3_236B
loc_3_235E:
        movzx  eax, byte ptr [esp + 18h]
        db  00Fh, 0B7h, 004h, 045h, 03Ah, 000h, 000h, 000h
loc_3_236B:
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  00Fh, 0B7h, 005h, 036h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0FFh, 035h, 000h, 00Ch, 000h, 000h
        db  0E8h, 0FDh, 0E1h, 0FCh, 0FFh
        add    esp, 0ch
loc_3_238E:
        mov    al, byte ptr [esp + 19h]
        and    eax, 0bh
        or     dword ptr [ebx + 5a5h], eax
        cmp    dword ptr [esp + 1ah], 0
        je     SHORT loc_3_23AC
        mov    eax, dword ptr [esp + 1ah]
        mov    dword ptr [ebx + 5a9h], eax
loc_3_23AC:
        cmp    dword ptr [esp + 1eh], 0
        je     SHORT loc_3_23BD
        mov    eax, dword ptr [esp + 1eh]
        mov    dword ptr [ebx + 5cah], eax
loc_3_23BD:
        movzx  eax, byte ptr [esp + 18h]
        cmp    eax, 1ah
        ja     NEAR PTR loc_3_2B38
        db  0FFh, 024h, 085h, 08Bh, 02Bh, 000h, 000h
loc_3_23D2:
        sub    edx, edx
        sub    esi, esi
        mov    ecx, dword ptr [ebx + 845h]
        cmp    ecx, edx
        je     SHORT loc_3_23FE
        mov    edi, dword ptr [ebx + 841h]
        add    edi, ebx
loc_3_23E8:
        mov    al, byte ptr [edi + esi + 3ech]
        sub    al, 30h
        imul   edx, edx, 0ah
        movsx  eax, al
        add    edx, eax
        inc    esi
        cmp    esi, ecx
        jb     SHORT loc_3_23E8
loc_3_23FE:
        cmp    byte ptr [esp + 18h], 12h
        jne    SHORT loc_3_2407
        neg    edx
loc_3_2407:
        mov    eax, dword ptr [ebx + 702h]
        cmp    eax, 10h
        jae    SHORT loc_3_241F
        mov    dword ptr [ebx + eax*4 + 706h], edx
        inc    dword ptr [ebx + 702h]
loc_3_241F:
        mov    dword ptr [ebx + 4ech], 3
        jmp    NEAR PTR loc_3_1ECD
loc_3_242E:
        mov    dword ptr [ebx + 4ech], 3
        jmp    NEAR PTR loc_3_1ECD
loc_3_243D:
        mov    ebp, dword ptr [ebx + 845h]
        cmp    ebp, 0ah
        jb     SHORT loc_3_244D
        mov    ebp, 0ah
loc_3_244D:
        lea    edx, [ebx + 746h]
        mov    eax, dword ptr [ebx + 841h]
        mov    edi, edx
        lea    esi, [eax + ebx + 3ech]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 845h]
        mov    byte ptr [ebx + eax + 746h], 0
        mov    eax, dword ptr [ebx + 79ah]
        or     eax, eax
        je     SHORT loc_3_24BF
        mov    esi, edx
loc_3_248A:
        mov    cl, byte ptr [esi]
        cmp    cl, byte ptr [eax]
        jne    SHORT loc_3_24AA
        or     cl, cl
        je     SHORT loc_3_24A6
        mov    cl, byte ptr [esi + 1]
        cmp    cl, byte ptr [eax + 1]
        jne    SHORT loc_3_24AA
        add    esi, 2
        add    eax, 2
        or     cl, cl
        jne    SHORT loc_3_248A
loc_3_24A6:
        sub    eax, eax
        jmp    SHORT loc_3_24AF
loc_3_24AA:
        sbb    eax, eax
        sbb    eax, -1
loc_3_24AF:
        or     eax, eax
        jne    SHORT loc_3_24BF
        mov    dword ptr [ebx + 6fah], 1
        jmp    SHORT loc_3_2516
loc_3_24BF:
        mov    eax, dword ptr [ebx + 79eh]
        or     eax, eax
        je     SHORT loc_3_24FE
loc_3_24C9:
        mov    cl, byte ptr [edx]
        cmp    cl, byte ptr [eax]
        jne    SHORT loc_3_24E9
        or     cl, cl
        je     SHORT loc_3_24E5
        mov    cl, byte ptr [edx + 1]
        cmp    cl, byte ptr [eax + 1]
        jne    SHORT loc_3_24E9
        add    edx, 2
        add    eax, 2
        or     cl, cl
        jne    SHORT loc_3_24C9
loc_3_24E5:
        sub    eax, eax
        jmp    SHORT loc_3_24EE
loc_3_24E9:
        sbb    eax, eax
        sbb    eax, -1
loc_3_24EE:
        or     eax, eax
        jne    SHORT loc_3_24FE
        mov    dword ptr [ebx + 6fah], 2
        jmp    SHORT loc_3_2516
loc_3_24FE:
        mov    eax, dword ptr [ebx + 6fah]
        and    eax, 0ffffffefh
        mov    dword ptr [ebx + 6fah], eax
        or     eax, 4
        mov    dword ptr [ebx + 6fah], eax
loc_3_2516:
        mov    dword ptr [ebx + 4ech], 3
        jmp    NEAR PTR loc_3_1ECD
loc_3_2525:
        mov    edx, dword ptr [ebx + 845h]
        cmp    edx, 40h
        jb     SHORT loc_3_2535
        mov    edx, 40h
loc_3_2535:
        mov    eax, dword ptr [ebx + 841h]
        lea    edi, [ebx + 751h]
        lea    esi, [eax + ebx + 3ech]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 845h]
        mov    byte ptr [ebx + eax + 751h], 0
        mov    eax, dword ptr [ebx + 6fah]
        and    eax, 0ffffffefh
        mov    dword ptr [ebx + 6fah], eax
        or     eax, 8
        mov    dword ptr [ebx + 6fah], eax
        mov    dword ptr [ebx + 4ech], 3
        jmp    NEAR PTR loc_3_1ECD
loc_3_258B:
        or     dword ptr [ebx + 6fah], 20h
        movzx  eax, byte ptr [esp + 18h]
        sub    eax, 18h
        mov    dword ptr [ebx + 706h], eax
loc_3_25A0:
        inc    dword ptr [ebx + 6feh]
        mov    eax, dword ptr [ebx + 2e0h]
        cmp    eax, 6
        je     SHORT loc_3_25C3
        cmp    eax, 9
        je     SHORT loc_3_25C3
        cmp    dword ptr [ebx + 5e2h], 0
        jne    NEAR PTR loc_3_2A50
loc_3_25C3:
        mov    dword ptr [ebx + 4ech], 3
        jmp    NEAR PTR loc_3_1ECD
loc_3_25D2:
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [ebx + 29ch]
        db  0E8h, 086h, 0DEh, 0FCh, 0FFh
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_3_2B38
        cmp    dword ptr [esp + 2ch], 1
        ja     NEAR PTR loc_3_2B18
        push   0
        push   0
        push   -1
        push   dword ptr [ebx + 29ch]
        db  0E8h, 0B7h, 0DEh, 0FCh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_3_2B38
        cmp    dword ptr [ebx + 2e0h], 19h
        jne    SHORT loc_3_2633
        push   ebx
        push   12ch
        db  068h, 04Ch, 01Eh, 000h, 000h
        db  0E8h, 0FBh, 0DEh, 0FCh, 0FFh
        mov    dword ptr [ebx + 810h], eax
loc_3_2633:
        mov    dword ptr [ebx + 4f0h], 0
        mov    dword ptr [ebx + 608h], 0
        mov    dword ptr [ebx + 4ech], 4
        mov    dword ptr [ebx + 610h], 0
        mov    dword ptr [ebx + 614h], 0
        mov    byte ptr [ebx + 840h], 0
        mov    dword ptr [ebx + 841h], 0
        jmp    NEAR PTR loc_3_1ECD
loc_3_267B:
        cmp    dword ptr [ebx + 5d6h], 0
        jne    NEAR PTR loc_3_2B38
        db  083h, 03Dh, 000h, 00Ch, 000h, 000h, 000h
        je     SHORT loc_3_26AD
        db  00Fh, 0B7h, 005h, 0CAh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0FFh, 035h, 000h, 00Ch, 000h, 000h
        db  0E8h, 0DEh, 0DEh, 0FCh, 0FFh
        add    esp, 8
loc_3_26AD:
        mov    dword ptr [ebx + 5d6h], 2
        mov    eax, dword ptr [ebx + 5e2h]
        mov    dword ptr [ebx + 5dah], eax
        mov    dword ptr [ebx + 5e2h], 0
        mov    dword ptr [ebx + 5d2h], 1
        db  083h, 03Dh, 000h, 00Ch, 000h, 000h, 000h
        je     SHORT loc_3_26FC
        db  00Fh, 0B7h, 005h, 02Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0FFh, 035h, 000h, 00Ch, 000h, 000h
        db  0E8h, 08Fh, 0DEh, 0FCh, 0FFh
        add    esp, 8
loc_3_26FC:
        push   0
        push   0
        push   6
        push   dword ptr [ebx + 29ch]
        db  0E8h, 073h, 0DDh, 0FCh, 0FFh
        add    esp, 10h
        mov    dword ptr [ebx + 2e0h], 0bh
        mov    dword ptr [ebx + 4ech], 0fh
        jmp    NEAR PTR loc_3_1E8B
loc_3_2729:
        push   ebx
        push   7d0h
        db  068h, 00Bh, 000h, 000h, 000h
        db  0E8h, 0EFh, 0DDh, 0FCh, 0FFh
        mov    dword ptr [ebx + 4f4h], eax
        or     eax, eax
        jne    NEAR PTR loc_3_2B43
        jmp    NEAR PTR loc_3_2B38
loc_3_274C:
        cmp    dword ptr [ebx + 4f0h], 0
        jne    SHORT loc_3_27BC
        mov    eax, dword ptr [ebx + 2e0h]
        sub    eax, 2
        cmp    eax, 18h
        ja     NEAR PTR loc_3_2B38
        db  0FFh, 024h, 085h, 0F7h, 02Bh, 000h, 000h
loc_3_276E:
        mov    esi, 7d0h
        jmp    SHORT loc_3_27C1
loc_3_2775:
        sub    esi, esi
        jmp    SHORT loc_3_27C1
loc_3_2779:
        cmp    dword ptr [ebx + 528h], 0
        je     SHORT loc_3_2797
        mov    esi, dword ptr [ebx + 591h]
        or     esi, esi
        je     SHORT loc_3_2797
        add    esi, 0ah
        imul   esi, esi, 3e8h
        jmp    SHORT loc_3_27C1
loc_3_2797:
        cmp    dword ptr [ebx + 618h], 1
        sbb    esi, esi
        and    esi, 0ffff1d70h
        add    esi, 0ea60h
        jmp    SHORT loc_3_27C1
loc_3_27AE:
        mov    esi, 0bb8h
        jmp    SHORT loc_3_27C1
loc_3_27B5:
        mov    esi, 1388h
        jmp    SHORT loc_3_27C1
loc_3_27BC:
        mov    esi, 3e8h
loc_3_27C1:
        push   ebx
        db  068h, 080h, 01Eh, 000h, 000h
        push   1
        lea    eax, [ebx + 640h]
        push   eax
        call   sub_3_687B
        or     eax, eax
        je     NEAR PTR loc_3_2B38
        or     esi, esi
        je     SHORT loc_3_2802
        push   ebx
        push   esi
        db  068h, 00Bh, 000h, 000h, 000h
        db  0E8h, 03Bh, 0DDh, 0FCh, 0FFh
        mov    dword ptr [ebx + 4f4h], eax
        or     eax, eax
        jne    SHORT loc_3_2802
        push   2
        push   ebx
        call   sub_3_1524
        add    esp, 8
loc_3_2802:
        mov    dword ptr [ebx + 4ech], 6
        jmp    NEAR PTR loc_3_2B43
loc_3_2811:
        push   0
        jmp    NEAR PTR loc_3_2B3A
loc_3_2818:
        cmp    dword ptr [ebx + 618h], 0
        je     NEAR PTR loc_3_28CD
        mov    eax, dword ptr [ebx + 2e0h]
        cmp    eax, 6
        je     NEAR PTR loc_3_28B7
        cmp    eax, 9
        je     SHORT loc_3_28B7
        cmp    eax, 5
        jne    NEAR PTR loc_3_28CD
        mov    dword ptr [ebx + 61ch], 2
        mov    dword ptr [ebx + 2e0h], 0dh
        mov    dword ptr [ebx + 5d2h], 1
        mov    eax, dword ptr [ebx + 5e2h]
        or     eax, eax
        je     SHORT loc_3_2887
        cmp    dword ptr [ebx + 5deh], eax
        je     SHORT loc_3_287D
        push   0
        push   eax
        db  0E8h, 056h, 0DCh, 0FCh, 0FFh
        add    esp, 8
loc_3_287D:
        mov    dword ptr [ebx + 5e2h], 0
loc_3_2887:
        push   dword ptr [ebx + 5deh]
        push   dword ptr [ebx + 2a8h]
        push   dword ptr [ebx + 2a4h]
        push   dword ptr [ebx + 2a0h]
        push   ebx
        call   sub_3_11F4
        add    esp, 14h
        cmp    eax, 1
        je     NEAR PTR loc_3_2B43
        push   eax
        jmp    NEAR PTR loc_3_2B3A
loc_3_28B7:
        mov    eax, dword ptr [ebx + 2ach]
        test   byte ptr [eax], 80h
        je     NEAR PTR loc_3_2B38
        push   0
        jmp    NEAR PTR loc_3_2B3A
loc_3_28CD:
        cmp    dword ptr [ebx + 5e2h], 0
        je     SHORT loc_3_28FA
        mov    eax, dword ptr [ebx + 5eah]
        mov    dword ptr [ebx + 5e6h], eax
        push   ebx
        call   sub_3_130D
        add    esp, 4
        cmp    eax, 1
        je     NEAR PTR loc_3_2B43
        push   eax
        jmp    NEAR PTR loc_3_2B3A
loc_3_28FA:
        push   0
        jmp    NEAR PTR loc_3_2B3A
loc_3_2901:
        call   sub_3_33D7
        add    esp, 0ch
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_3_2B38
        db  00Fh, 0B7h, 00Dh, 0AEh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 05Eh, 0DCh, 0FCh, 0FFh
        add    esp, 8
        jmp    NEAR PTR loc_3_2B38
loc_3_2932:
        cmp    dword ptr [ebx + 2e0h], 0ch
        jne    NEAR PTR loc_3_29F2
        push   0
        jmp    NEAR PTR loc_3_2B3A
loc_3_2946:
        cmp    dword ptr [ebx + 2e0h], 0ch
        jne    SHORT loc_3_2953
        push   0
        jmp    SHORT loc_3_2955
loc_3_2953:
        push   2
loc_3_2955:
        push   ebx
        call   sub_3_1524
        add    esp, 8
        cmp    dword ptr [ebx + 2e0h], 4
        jne    NEAR PTR loc_3_2B43
        cmp    word ptr [ebx + 89bh], 80h
        jne    SHORT loc_3_298C
        push   0bh
        push   ebx
        call   sub_3_3DE5
        add    esp, 8
        push   0
        push   ebx
        call   sub_3_3E17
        add    esp, 8
loc_3_298C:
        mov    eax, dword ptr [ebx + 8a9h]
        or     eax, eax
        je     NEAR PTR loc_3_2B43
        push   eax
        db  0E8h, 070h, 0DBh, 0FCh, 0FFh
        jmp    NEAR PTR loc_3_2B43
loc_3_29A5:
        push   6
        jmp    NEAR PTR loc_3_2B3A
loc_3_29AC:
        push   7
        jmp    NEAR PTR loc_3_2B3A
loc_3_29B3:
        push   4
        jmp    NEAR PTR loc_3_2B3A
loc_3_29BA:
        push   5
        jmp    NEAR PTR loc_3_2B3A
loc_3_29C1:
        movzx  ax, byte ptr [esp + 18h]
        sub    ax, 8
        push   eax
        push   ebx
        call   sub_3_3DE5
        jmp    NEAR PTR loc_3_2B40
loc_3_29D7:
        movzx  ax, byte ptr [esp + 18h]
        sub    ax, 8
        push   eax
        push   ebx
        call   sub_3_3DE5
        add    esp, 8
        push   0
        jmp    NEAR PTR loc_3_2B3A
loc_3_29F2:
        mov    dword ptr [ebx + 5ceh], 1
        push   ebx
        call   sub_3_130D
        add    esp, 4
        cmp    eax, 1
        je     SHORT loc_3_2A14
        push   eax
        push   ebx
        call   sub_3_1524
        add    esp, 8
loc_3_2A14:
        cmp    dword ptr [ebx + 2e0h], 4
        jne    NEAR PTR loc_3_2B43
        cmp    word ptr [ebx + 89bh], 80h
        jne    SHORT loc_3_2A37
        push   0ah
        push   ebx
        call   sub_3_3DE5
        add    esp, 8
loc_3_2A37:
        mov    eax, dword ptr [ebx + 8a9h]
        or     eax, eax
        je     NEAR PTR loc_3_2B43
        push   eax
        db  0E8h, 0C5h, 0DAh, 0FCh, 0FFh
        jmp    NEAR PTR loc_3_2B43
loc_3_2A50:
        cmp    dword ptr [ebx + 2e0h], 4
        je     SHORT loc_3_2A65
        mov    eax, dword ptr [ebx + 5eah]
        mov    dword ptr [ebx + 5e6h], eax
loc_3_2A65:
        mov    dword ptr [ebx + 5ceh], 1
        push   ebx
        call   sub_3_130D
        add    esp, 4
        cmp    eax, 1
        je     NEAR PTR loc_3_2B43
        push   eax
        jmp    NEAR PTR loc_3_2B3A
loc_3_2A87:
        cmp    dword ptr [ebx + 2e0h], 0bh
        je     SHORT loc_3_2A9F
        push   2
        push   ebx
        call   sub_3_1524
        add    esp, 8
        sub    eax, eax
        jmp    SHORT loc_3_2AC7
loc_3_2A9F:
        mov    dword ptr [ebx + 5bah], 30h
        push   0
        push   30h
        push   dword ptr [ebx + 29ch]
        db  0E8h, 0D8h, 0D9h, 0FCh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebx + 5d2h]
        imul   eax, eax, 4b0h
loc_3_2AC7:
        or     eax, eax
        je     SHORT loc_3_2B43
        mov    dword ptr [ebx + 4ech], 10h
        push   ebx
        push   eax
        db  068h, 00Bh, 000h, 000h, 000h
        db  0E8h, 047h, 0DAh, 0FCh, 0FFh
        mov    dword ptr [ebx + 4f4h], eax
        or     eax, eax
        jne    SHORT loc_3_2B43
        jmp    SHORT loc_3_2B38
loc_3_2AED:
        cmp    dword ptr [ebx + 2e0h], 0bh
        je     SHORT loc_3_2AF8
        jmp    SHORT loc_3_2B38
loc_3_2AF8:
        mov    dword ptr [ebx + 5bah], 0
        push   0
        push   0
        push   dword ptr [ebx + 29ch]
        db  0E8h, 07Fh, 0D9h, 0FCh, 0FFh
        add    esp, 0ch
        push   0
        jmp    SHORT loc_3_2B3A
loc_3_2B18:
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_2B38
        db  00Fh, 0B7h, 00Dh, 0ACh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 053h, 0DAh, 0FCh, 0FFh
        add    esp, 8
loc_3_2B38:
        push   2
loc_3_2B3A:
        push   ebx
        call   sub_3_1524
loc_3_2B40:
        add    esp, 8
loc_3_2B43:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  0E4h, 01Eh, 000h, 000h, 0D2h, 025h, 000h, 000h, 033h, 026h, 000h, 000h, 037h, 01Fh, 000h, 000h
        db  04Ch, 027h, 000h, 000h, 0AFh, 01Fh, 000h, 000h, 04Ah, 020h, 000h, 000h, 018h, 028h, 000h, 000h
        db  018h, 022h, 000h, 000h, 03Bh, 022h, 000h, 000h, 0B1h, 022h, 000h, 000h, 0D7h, 022h, 000h, 000h
        db  03Eh, 023h, 000h, 000h, 043h, 02Bh, 000h, 000h, 087h, 02Ah, 000h, 000h, 0EDh, 02Ah, 000h, 000h
        db  032h, 029h, 000h, 000h, 0C3h, 025h, 000h, 000h, 0F2h, 029h, 000h, 000h, 046h, 029h, 000h, 000h
        db  0A5h, 029h, 000h, 000h, 0ACh, 029h, 000h, 000h, 0B3h, 029h, 000h, 000h, 0BAh, 029h, 000h, 000h
        db  0A0h, 025h, 000h, 000h, 0C1h, 029h, 000h, 000h, 0D7h, 029h, 000h, 000h, 0D7h, 029h, 000h, 000h
        db  0D7h, 029h, 000h, 000h, 0D7h, 029h, 000h, 000h, 0D7h, 029h, 000h, 000h, 0D7h, 029h, 000h, 000h
        db  0D7h, 029h, 000h, 000h, 0D2h, 023h, 000h, 000h, 0D2h, 023h, 000h, 000h, 02Eh, 024h, 000h, 000h
        db  02Eh, 024h, 000h, 000h, 03Dh, 024h, 000h, 000h, 025h, 025h, 000h, 000h, 02Eh, 024h, 000h, 000h
        db  08Bh, 025h, 000h, 000h, 08Bh, 025h, 000h, 000h, 08Bh, 025h, 000h, 000h, 06Eh, 027h, 000h, 000h
        db  038h, 02Bh, 000h, 000h, 075h, 027h, 000h, 000h, 079h, 027h, 000h, 000h, 079h, 027h, 000h, 000h
        db  038h, 02Bh, 000h, 000h, 038h, 02Bh, 000h, 000h, 079h, 027h, 000h, 000h, 06Eh, 027h, 000h, 000h
        db  038h, 02Bh, 000h, 000h, 06Eh, 027h, 000h, 000h, 06Eh, 027h, 000h, 000h, 079h, 027h, 000h, 000h
        db  038h, 02Bh, 000h, 000h, 079h, 027h, 000h, 000h, 079h, 027h, 000h, 000h, 079h, 027h, 000h, 000h
        db  038h, 02Bh, 000h, 000h, 0AEh, 027h, 000h, 000h, 038h, 02Bh, 000h, 000h, 038h, 02Bh, 000h, 000h
        db  0AEh, 027h, 000h, 000h, 038h, 02Bh, 000h, 000h, 079h, 027h, 000h, 000h, 0B5h, 027h, 000h, 000h
sub_3_2C5B:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, 1
        mov    esi, dword ptr [esp + 14h]
        lea    eax, [esp + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 5b6h]
        not    eax
        push   eax
        push   dword ptr [esi + 29ch]
        db  0E8h, 015h, 0D8h, 0FCh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [esi + 5b2h]
        test   dword ptr [esi + 5bah], eax
        je     NEAR PTR loc_3_2DE1
        mov    ecx, dword ptr [esi + 2e0h]
        cmp    ecx, 7
        je     SHORT loc_3_2CB1
        cmp    ecx, 0bh
        je     NEAR PTR loc_3_2DCE
        jmp    NEAR PTR loc_3_2DE1
loc_3_2CB1:
        test   al, 80h
        je     SHORT loc_3_2CC9
        mov    eax, dword ptr [esi + 60ch]
        or     eax, eax
        je     SHORT loc_3_2CC9
        push   eax
        push   esi
        call   sub_3_3349
        add    esp, 8
loc_3_2CC9:
        mov    eax, dword ptr [esi + 5b2h]
        test   al, 20h
        je     SHORT loc_3_2CDE
        mov    ecx, dword ptr [esi + 2ach]
        test   byte ptr [ecx], 80h
        je     SHORT loc_3_2CF5
loc_3_2CDE:
        test   al, 10h
        je     NEAR PTR loc_3_2DE1
        mov    eax, dword ptr [esi + 2ach]
        test   byte ptr [eax], 20h
        jne    NEAR PTR loc_3_2DE1
loc_3_2CF5:
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_2D15
        db  00Fh, 0B7h, 00Dh, 02Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 076h, 0D8h, 0FCh, 0FFh
        add    esp, 8
loc_3_2D15:
        push   esi
        call   sub_3_00A2
        add    esp, 4
        push   0
        lea    eax, [esi + 640h]
        push   eax
        call   sub_3_64E3
        push   0
        push   0
        push   -1
        push   dword ptr [esi + 29ch]
        db  0E8h, 083h, 0D7h, 0FCh, 0FFh
        add    esp, 10h
        mov    ebx, dword ptr [esi + 29ch]
        push   0
        push   ebx
        db  0E8h, 03Ah, 0D7h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [esi + 5bah], 0
        push   0
        push   0
        push   ebx
        db  0E8h, 02Bh, 0D7h, 0FCh, 0FFh
        add    esp, 0ch
        cmp    dword ptr [esi + 618h], 0
        je     SHORT loc_3_2D87
        mov    dword ptr [esi + 2e0h], 0ah
        mov    dword ptr [esi + 4ech], 2
        jmp    SHORT loc_3_2DD6
loc_3_2D87:
        push   0
        push   0
        push   6
        push   dword ptr [esi + 29ch]
        db  0E8h, 0E8h, 0D6h, 0FCh, 0FFh
        add    esp, 10h
        mov    dword ptr [esi + 2e0h], 3
        mov    eax, dword ptr [esi + 2a8h]
        and    eax, 0ffffh
        mov    dword ptr [esi + 2a8h], eax
        or     eax, 0ffff0000h
        mov    dword ptr [esi + 2a8h], eax
        push   3
        push   esi
        call   sub_3_063B
        add    esp, 8
        jmp    SHORT loc_3_2DE1
loc_3_2DCE:
        test   al, 20h
        jne    SHORT loc_3_2DD6
        test   al, 10h
        je     SHORT loc_3_2DE1
loc_3_2DD6:
        push   esi
        call   sub_3_1E80
        add    esp, 4
        sub    edi, edi
loc_3_2DE1:
        mov    dword ptr [esi + 5b2h], 0
        mov    dword ptr [esi + 60ch], 0
        or     edi, edi
        je     SHORT loc_3_2E03
        mov    dword ptr [esi + 620h], 0
loc_3_2E03:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_3_2E0A:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_3_2E61
        add    esp, 8
        mov    edi, eax
        cmp    edi, 3
        jne    SHORT loc_3_2E5C
        push   dword ptr [esi + 4f0h]
        lea    eax, [esi + 3ech]
        push   eax
        lea    eax, [esi + 2e8h]
        push   eax
        call   sub_3_061C
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_2E5C
        mov    eax, dword ptr [esi + 4f0h]
        cmp    dword ptr [esi + 3e8h], eax
        mov    edi, 1
        je     SHORT loc_3_2E5C
        mov    edi, 2
loc_3_2E5C:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_3_2E61:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esp + 10h], 0
        mov    ebx, dword ptr [eax + 5fah]
        add    eax, 3ech
        mov    dword ptr [esp + 14h], 0
        mov    ebp, dword ptr [eax + 104h]
        mov    dword ptr [esp + 18h], eax
        or     ebx, ebx
        je     SHORT loc_3_2ED8
loc_3_2E95:
        push   ebp
        lea    eax, [ebx + 0fh]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_3_3146
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_2ED2
        movzx  eax, byte ptr [ebx + 0eh]
        sub    eax, ebp
        cmp    eax, -1
        jne    SHORT loc_3_2ECA
        mov    dword ptr [esp + 10h], 1
        lea    esi, [ebx + 4]
        mov    edi, dword ptr [esp + 24h]
        movsd
        movsd
        movsw
        jmp    SHORT loc_3_2ED2
loc_3_2ECA:
        mov    dword ptr [esp + 14h], 1
loc_3_2ED2:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_3_2E95
loc_3_2ED8:
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, dword ptr [esp + 10h]
        lea    eax, [ecx + eax*2]
        or     eax, eax
        je     SHORT loc_3_2EFD
        cmp    eax, 1
        je     SHORT loc_3_2F04
        cmp    eax, 2
        je     SHORT loc_3_2F0B
        cmp    eax, 3
        je     SHORT loc_3_2F0F
        mov    eax, 3
        jmp    SHORT loc_3_2F14
loc_3_2EFD:
        mov    eax, 3
        jmp    SHORT loc_3_2F14
loc_3_2F04:
        mov    eax, 2
        jmp    SHORT loc_3_2F14
loc_3_2F0B:
        sub    eax, eax
        jmp    SHORT loc_3_2F14
loc_3_2F0F:
        mov    eax, 4
loc_3_2F14:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_3_2F1C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [esp + 20h]
        cmp    byte ptr [ebp], 0
        je     NEAR PTR loc_3_30D7
loc_3_2F35:
        cmp    byte ptr [ebp], 3ch
        jne    NEAR PTR loc_3_30C6
        push   4
        db  068h, 008h, 009h, 000h, 000h
        push   ebp
        call   sub_3_3146
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_2F5F
        mov    byte ptr [ebx], 0dh
        inc    ebx
        add    ebp, 4
        jmp    NEAR PTR loc_3_30CD
loc_3_2F5F:
        push   4
        db  068h, 010h, 009h, 000h, 000h
        push   ebp
        call   sub_3_3146
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_2F7F
        mov    byte ptr [ebx], 0ah
        inc    ebx
        add    ebp, 4
        jmp    NEAR PTR loc_3_30CD
loc_3_2F7F:
        mov    al, byte ptr [ebp + 1]
        cmp    al, 68h
        je     SHORT loc_3_2F8E
        cmp    al, 48h
        jne    NEAR PTR loc_3_3038
loc_3_2F8E:
        mov    cl, byte ptr [ebp + 2]
        cmp    cl, 61h
        jl     SHORT loc_3_2F9B
        cmp    cl, 66h
        jle    SHORT loc_3_2FB7
loc_3_2F9B:
        cmp    cl, 41h
        jl     SHORT loc_3_2FA5
        cmp    cl, 46h
        jle    SHORT loc_3_2FB7
loc_3_2FA5:
        cmp    cl, 30h
        jl     NEAR PTR loc_3_3038
        cmp    cl, 39h
        jg     NEAR PTR loc_3_3038
loc_3_2FB7:
        mov    al, byte ptr [ebp + 3]
        cmp    al, 61h
        jl     SHORT loc_3_2FC2
        cmp    al, 66h
        jle    SHORT loc_3_2FD2
loc_3_2FC2:
        cmp    al, 41h
        jl     SHORT loc_3_2FCA
        cmp    al, 46h
        jle    SHORT loc_3_2FD2
loc_3_2FCA:
        cmp    al, 30h
        jl     SHORT loc_3_3038
        cmp    al, 39h
        jg     SHORT loc_3_3038
loc_3_2FD2:
        cmp    byte ptr [ebp + 4], 3eh
        jne    SHORT loc_3_3038
        cmp    cl, 30h
        jl     SHORT loc_3_2FEA
        cmp    cl, 39h
        jg     SHORT loc_3_2FEA
        movsx  ecx, cl
        sub    ecx, 30h
        jmp    SHORT loc_3_3002
loc_3_2FEA:
        cmp    cl, 61h
        jl     SHORT loc_3_2FFC
        cmp    cl, 7ah
        jg     SHORT loc_3_2FFC
        movsx  ecx, cl
        sub    ecx, 20h
        jmp    SHORT loc_3_2FFF
loc_3_2FFC:
        movsx  ecx, cl
loc_3_2FFF:
        sub    ecx, 37h
loc_3_3002:
        cmp    al, 30h
        jl     SHORT loc_3_3012
        cmp    al, 39h
        jg     SHORT loc_3_3012
        movsx  eax, al
        sub    eax, 30h
        jmp    SHORT loc_3_3028
loc_3_3012:
        cmp    al, 61h
        jl     SHORT loc_3_3022
        cmp    al, 7ah
        jg     SHORT loc_3_3022
        movsx  eax, al
        sub    eax, 20h
        jmp    SHORT loc_3_3025
loc_3_3022:
        movsx  eax, al
loc_3_3025:
        sub    eax, 37h
loc_3_3028:
        shl    cl, 4
        add    cl, al
        mov    byte ptr [ebx], cl
        inc    ebx
        add    ebp, 5
        jmp    NEAR PTR loc_3_30CD
loc_3_3038:
        cmp    dword ptr [esp + 28h], 0
        je     NEAR PTR loc_3_30C6
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 10h], 0
        cmp    dword ptr [esp + 2ch], 0
        je     SHORT loc_3_30BF
        mov    esi, dword ptr [esp + 28h]
loc_3_305E:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx - 1]
        push   edi
        push   esi
        push   ebp
        call   sub_3_3146
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_3093
        add    esi, 200h
        inc    dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, dword ptr [esp + 2ch]
        jb     SHORT loc_3_305E
        jmp    SHORT loc_3_30BF
loc_3_3093:
        mov    ecx, dword ptr [esp + 10h]
        shl    ecx, 9
        mov    eax, dword ptr [esp + 28h]
        lea    ecx, [ecx + eax + 100h]
        cmp    byte ptr [ecx], 0
        je     SHORT loc_3_30B5
loc_3_30AA:
        mov    al, byte ptr [ecx]
        mov    byte ptr [ebx], al
        inc    ecx
        inc    ebx
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_3_30AA
loc_3_30B5:
        add    ebp, edi
        mov    dword ptr [esp + 14h], 1
loc_3_30BF:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_3_30CD
loc_3_30C6:
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebx], al
        inc    ebp
        inc    ebx
loc_3_30CD:
        cmp    byte ptr [ebp], 0
        jne    NEAR PTR loc_3_2F35
loc_3_30D7:
        mov    byte ptr [ebx], 0
        mov    eax, dword ptr [esp + 24h]
        or     eax, eax
        je     SHORT loc_3_30E8
        sub    ebx, dword ptr [esp + 20h]
        mov    dword ptr [eax], ebx
loc_3_30E8:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_3_30F5:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    esi, edi
        mov    ebx, dword ptr [esp + 10h]
        or     ebx, ebx
        jne    SHORT loc_3_310F
        mov    byte ptr [edi], 30h
        mov    byte ptr [edi + 1], 0
        jmp    SHORT loc_3_3142
loc_3_310F:
        mov    ecx, 0ah
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        add    dl, 30h
        mov    byte ptr [esi], dl
        inc    esi
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_310F
        mov    byte ptr [esi], 0
        dec    esi
        cmp    esi, edi
        jbe    SHORT loc_3_3142
loc_3_3134:
        mov    al, byte ptr [esi]
        mov    cl, byte ptr [edi]
        mov    byte ptr [esi], cl
        mov    byte ptr [edi], al
        inc    edi
        dec    esi
        cmp    esi, edi
        ja     SHORT loc_3_3134
loc_3_3142:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_3146:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 18h]
        or     edx, edx
        je     SHORT loc_3_3194
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
loc_3_3159:
        mov    bl, byte ptr [eax]
        cmp    bl, 61h
        jl     SHORT loc_3_316D
        cmp    bl, 7ah
        jg     SHORT loc_3_316D
        movsx  edi, bl
        sub    edi, 20h
        jmp    SHORT loc_3_3170
loc_3_316D:
        movsx  edi, bl
loc_3_3170:
        mov    bl, byte ptr [ecx]
        cmp    bl, 61h
        jl     SHORT loc_3_3184
        cmp    bl, 7ah
        jg     SHORT loc_3_3184
        movsx  esi, bl
        sub    esi, 20h
        jmp    SHORT loc_3_3187
loc_3_3184:
        movsx  esi, bl
loc_3_3187:
        cmp    edi, esi
        jne    SHORT loc_3_319A
        or     bl, bl
        je     SHORT loc_3_3194
        inc    eax
        inc    ecx
        dec    edx
        jne    SHORT loc_3_3159
loc_3_3194:
        sub    eax, eax
loc_3_3196:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_319A:
        mov    eax, 1
        jmp    SHORT loc_3_3196
sub_3_31A1:
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
loc_3_31AB:
        mov    cl, byte ptr [eax]
        cmp    cl, 61h
        jl     SHORT loc_3_31BF
        cmp    cl, 7ah
        jg     SHORT loc_3_31BF
        movsx  edx, cl
        sub    edx, 20h
        jmp    SHORT loc_3_31C2
loc_3_31BF:
        movsx  edx, cl
loc_3_31C2:
        mov    bl, byte ptr [esi]
        cmp    bl, 61h
        jl     SHORT loc_3_31D6
        cmp    bl, 7ah
        jg     SHORT loc_3_31D6
        movsx  ecx, bl
        sub    ecx, 20h
        jmp    SHORT loc_3_31D9
loc_3_31D6:
        movsx  ecx, bl
loc_3_31D9:
        cmp    edx, ecx
        jne    SHORT loc_3_31E9
        or     bl, bl
        je     SHORT loc_3_31E5
        inc    eax
        inc    esi
        jmp    SHORT loc_3_31AB
loc_3_31E5:
        sub    eax, eax
        jmp    SHORT loc_3_31F1
loc_3_31E9:
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [esi]
        sub    eax, ecx
loc_3_31F1:
        pop    esi
        pop    ebx
        ret
sub_3_31F4:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 14h]
        push   eax
        sub    esi, esi
        mov    eax, dword ptr [esp + 30h]
        push   dword ptr [esp + 34h]
        push   dword ptr [eax + 2e4h]
        db  0E8h, 013h, 0D2h, 0FCh, 0FFh
        add    esp, 0ch
        cmp    eax, esi
        je     SHORT loc_3_3223
        sub    eax, eax
        jmp    NEAR PTR loc_3_3341
loc_3_3223:
        mov    edi, 1
        lea    eax, [esp + 1ch]
        push   eax
        push   edi
        call   sub_3_30F5
        add    esp, 8
        mov    ebx, edi
loc_3_3238:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        lea    eax, [esp + 28h]
        push   0
        push   eax
        push   dword ptr [esp + 28h]
        db  0E8h, 0F4h, 0D1h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_327A
        cmp    dword ptr [esp + 18h], ebx
        jne    NEAR PTR loc_3_3331
        add    esi, dword ptr [esp + 10h]
        inc    edi
        lea    eax, [esp + 1ch]
        push   eax
        push   edi
        call   sub_3_30F5
        add    esp, 8
        jmp    SHORT loc_3_3238
loc_3_327A:
        cmp    eax, 2
        je     SHORT loc_3_3284
        jmp    NEAR PTR loc_3_3331
loc_3_3284:
        mov    edi, dword ptr [esp + 34h]
        or     edi, edi
        je     SHORT loc_3_32B0
        push   0
        push   edi
        db  0E8h, 04Ch, 0D2h, 0FCh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_32A0
        jmp    NEAR PTR loc_3_3331
loc_3_32A0:
        add    esi, eax
        push   1
        push   esi
        push   edi
        db  0E8h, 02Dh, 0D2h, 0FCh, 0FFh
        add    esp, 0ch
        jmp    SHORT loc_3_32BC
loc_3_32B0:
        inc    esi
        push   1
        push   esi
        db  0E8h, 00Fh, 0D2h, 0FCh, 0FFh
        add    esp, 8
loc_3_32BC:
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_3_3333
        mov    ebx, ebp
        cmp    byte ptr [ebp], 0
        je     SHORT loc_3_32DE
loc_3_32CA:
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebx, ecx
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_3_32CA
loc_3_32DE:
        mov    edi, 1
loc_3_32E3:
        lea    eax, [esp + 1ch]
        push   eax
        push   edi
        call   sub_3_30F5
        add    esp, 8
        mov    eax, ebp
        sub    eax, ebx
        add    eax, esi
        mov    dword ptr [esp + 10h], eax
        lea    eax, [esp + 10h]
        push   eax
        push   ebx
        push   0
        lea    eax, [esp + 28h]
        push   0
        push   eax
        push   dword ptr [esp + 28h]
        db  0E8h, 035h, 0D1h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_3321
        add    ebx, dword ptr [esp + 10h]
        inc    edi
        jmp    SHORT loc_3_32E3
loc_3_3321:
        cmp    eax, 2
        je     SHORT loc_3_3333
        push   0
        push   ebp
        db  0E8h, 0A2h, 0D1h, 0FCh, 0FFh
        add    esp, 8
loc_3_3331:
        sub    ebp, ebp
loc_3_3333:
        push   dword ptr [esp + 14h]
        db  0E8h, 0F4h, 0D0h, 0FCh, 0FFh
        add    esp, 4
        mov    eax, ebp
loc_3_3341:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_3_3349:
        push   esi
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_3_3375
        lea    eax, [esp + 0ch]
        push   eax
        mov    esi, dword ptr [esp + 0ch]
        push   0
        push   dword ptr [esi + 29ch]
        db  0E8h, 009h, 0D1h, 0FCh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_3379
        mov    eax, 0ffffffffh
        jmp    SHORT loc_3_33D5
loc_3_3375:
        mov    esi, dword ptr [esp + 8]
loc_3_3379:
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_3_33D1
        test   dword ptr [esp + 0ch], 9bf0h
        je     SHORT loc_3_3390
        inc    dword ptr [esi + 628h]
loc_3_3390:
        test   byte ptr [esp + 0ch], 8
        je     SHORT loc_3_339D
        inc    dword ptr [esi + 62ch]
loc_3_339D:
        test   byte ptr [esp + 0dh], 4
        je     SHORT loc_3_33AA
        inc    dword ptr [esi + 630h]
loc_3_33AA:
        test   byte ptr [esp + 0ch], 2
        je     SHORT loc_3_33B7
        inc    dword ptr [esi + 634h]
loc_3_33B7:
        test   byte ptr [esp + 0ch], 1
        je     SHORT loc_3_33C4
        inc    dword ptr [esi + 638h]
loc_3_33C4:
        test   byte ptr [esp + 0ch], 4
        je     SHORT loc_3_33D1
        inc    dword ptr [esi + 63ch]
loc_3_33D1:
        mov    eax, dword ptr [esp + 0ch]
loc_3_33D5:
        pop    esi
        ret
sub_3_33D7:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 4ch]
        mov    edi, ebp
        mov    ecx, dword ptr [esp + 48h]
        lea    esi, [esp + 10h]
        mov    eax, edi
        dec    edi
        or     eax, eax
        mov    eax, dword ptr [esp + 50h]
        je     NEAR PTR loc_3_3481
loc_3_33FB:
        mov    edx, esi
        lea    ebx, [esp + 10h]
        sub    edx, ebx
        cmp    edx, 2eh
        jge    SHORT loc_3_3481
        mov    dl, byte ptr [ecx]
        cmp    dl, 20h
        jl     SHORT loc_3_342C
        cmp    dl, 7eh
        jg     SHORT loc_3_342C
        cmp    eax, 1
        jne    SHORT loc_3_3428
        cmp    dl, 30h
        jl     SHORT loc_3_3428
        cmp    dl, 39h
        jg     SHORT loc_3_3428
        mov    byte ptr [esi], 23h
        jmp    SHORT loc_3_3474
loc_3_3428:
        mov    byte ptr [esi], dl
        jmp    SHORT loc_3_3474
loc_3_342C:
        movsx  ebx, dl
        cmp    ebx, 0ah
        je     SHORT loc_3_345B
        cmp    ebx, 0dh
        mov    byte ptr [esi], 3ch
        je     SHORT loc_3_3468
        mov    bl, dl
        shr    bl, 4
        and    ebx, 0fh
        inc    esi
        db  08Ah, 09Bh, 088h, 008h, 000h, 000h
        mov    byte ptr [esi], bl
        and    edx, 0fh
        inc    esi
        db  08Ah, 092h, 088h, 008h, 000h, 000h
        mov    byte ptr [esi], dl
        jmp    SHORT loc_3_3470
loc_3_345B:
        mov    byte ptr [esi], 3ch
        inc    esi
        mov    byte ptr [esi], 6ch
        inc    esi
        mov    byte ptr [esi], 66h
        jmp    SHORT loc_3_3470
loc_3_3468:
        inc    esi
        mov    byte ptr [esi], 63h
        inc    esi
        mov    byte ptr [esi], 72h
loc_3_3470:
        inc    esi
        mov    byte ptr [esi], 3eh
loc_3_3474:
        inc    esi
        inc    ecx
        mov    edx, edi
        dec    edi
        or     edx, edx
        jne    NEAR PTR loc_3_33FB
loc_3_3481:
        mov    byte ptr [esi], 0
        cmp    eax, 1
        jbe    SHORT loc_3_3490
        cmp    eax, 2
        je     SHORT loc_3_34A8
        jmp    SHORT loc_3_34DF
loc_3_3490:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_34DF
        lea    eax, [esp + 10h]
        push   eax
        db  00Fh, 0B7h, 005h, 030h, 000h, 000h, 000h
        jmp    SHORT loc_3_34CF
loc_3_34A8:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_34DF
        or     ebp, ebp
        lea    eax, [esp + 10h]
        jne    SHORT loc_3_34C7
        db  00Fh, 0B7h, 005h, 034h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
loc_3_34C7:
        push   eax
        db  00Fh, 0B7h, 005h, 032h, 000h, 000h, 000h
loc_3_34CF:
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0ACh, 0D0h, 0FCh, 0FFh
        add    esp, 0ch
loc_3_34DF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
sub_3_34E7:
        db  08Bh, 015h, 000h, 00Ch, 000h, 000h
        or     edx, edx
        je     SHORT loc_3_3522
        mov    ecx, dword ptr [esp + 4]
        movzx  eax, byte ptr [ecx + 18h]
        db  0FFh, 034h, 085h, 024h, 009h, 000h, 000h
        movzx  eax, byte ptr [ecx + 16h]
        push   eax
        movzx  eax, byte ptr [ecx + 17h]
        db  00Fh, 0BEh, 080h, 09Ch, 008h, 000h, 000h
        push   eax
        push   dword ptr [ecx + 4]
        db  068h, 030h, 009h, 000h, 000h
        push   edx
        db  0E8h, 069h, 0D0h, 0FCh, 0FFh
        add    esp, 18h
loc_3_3522:
        ret
sub_3_3523:
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 40h]
        mov    dword ptr [esp + 28h], 0
        mov    eax, dword ptr [ebx + 2e0h]
        cmp    eax, 18h
        je     SHORT loc_3_3546
        cmp    eax, 13h
        jne    SHORT loc_3_3560
loc_3_3546:
        test   byte ptr [ebx + 825h], 2
        mov    ebp, dword ptr [esp + 44h]
        je     SHORT loc_3_3564
        cmp    byte ptr [ebp], 3bh
        je     SHORT loc_3_3564
        mov    esi, 1
        jmp    SHORT loc_3_3566
loc_3_3560:
        mov    ebp, dword ptr [esp + 44h]
loc_3_3564:
        sub    esi, esi
loc_3_3566:
        cmp    eax, 18h
        je     SHORT loc_3_3570
        cmp    eax, 13h
        jne    SHORT loc_3_3581
loc_3_3570:
        test   byte ptr [ebx + 824h], 40h
        mov    dword ptr [esp + 34h], 1
        jne    SHORT loc_3_3589
loc_3_3581:
        mov    dword ptr [esp + 34h], 0
loc_3_3589:
        mov    ecx, dword ptr [esp + 48h]
        mov    eax, ebp
        mov    dword ptr [ecx], 1
        cmp    byte ptr [ebp], 0
        je     SHORT loc_3_35B0
loc_3_359B:
        cmp    byte ptr [eax], 3bh
        jne    SHORT loc_3_35AA
        mov    ecx, dword ptr [esp + 48h]
        mov    dword ptr [ecx], 0
loc_3_35AA:
        inc    eax
        cmp    byte ptr [eax], 0
        jne    SHORT loc_3_359B
loc_3_35B0:
        mov    ecx, dword ptr [esp + 48h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_3_35BD
        mov    byte ptr [eax - 1], 0
loc_3_35BD:
        push   1
        push   0a4h
        db  0E8h, 0FFh, 0CEh, 0FCh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_35DB
        sub    eax, eax
        jmp    NEAR PTR loc_3_3B94
loc_3_35DB:
        mov    eax, dword ptr [esp + 14h]
        add    eax, 29h
        mov    dword ptr [esp + 18h], eax
        add    eax, 29h
        mov    dword ptr [esp + 1ch], eax
        add    eax, 29h
        mov    dword ptr [esp + 30h], eax
        lea    eax, [esp + 2ch]
        push   eax
        db  068h, 0ACh, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 01Fh, 0CEh, 0FCh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_3625
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 0B5h, 0CEh, 0FCh, 0FFh
        add    esp, 8
        sub    eax, eax
        jmp    NEAR PTR loc_3_3B94
loc_3_3625:
        mov    dword ptr [esp + 20h], 28h
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   dword ptr [esp + 18h]
        push   eax
        push   0
        db  068h, 0C8h, 00Ah, 000h, 000h
        push   dword ptr [esp + 40h]
        db  0E8h, 0FDh, 0CDh, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_3673
        cmp    dword ptr [esp + 24h], 1
        jne    NEAR PTR loc_3_3B59
        push   0
        push   0
        push   0
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        call   sub_3_2F1C
        add    esp, 14h
loc_3_3673:
        or     esi, esi
        je     SHORT loc_3_3696
        mov    dword ptr [esp + 20h], 28h
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   dword ptr [esp + 18h]
        push   eax
        push   0
        db  068h, 0A4h, 008h, 000h, 000h
        jmp    SHORT loc_3_36D4
loc_3_3696:
        test   byte ptr [ebx + 539h], 2
        je     SHORT loc_3_371D
        cmp    dword ptr [ebx + 5c2h], 0
        jne    SHORT loc_3_371D
        mov    dword ptr [esp + 20h], 28h
        lea    eax, [esp + 20h]
        push   eax
        test   byte ptr [ebx + 5a2h], 2
        lea    eax, [esp + 28h]
        push   dword ptr [esp + 18h]
        push   eax
        db  0B8h, 0A4h, 008h, 000h, 000h
        push   0
        jne    SHORT loc_3_36D3
        db  0B8h, 0B0h, 008h, 000h, 000h
loc_3_36D3:
        push   eax
loc_3_36D4:
        push   dword ptr [esp + 40h]
        db  0E8h, 06Bh, 0CDh, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_3_3B59
        cmp    dword ptr [esp + 24h], 1
        jne    NEAR PTR loc_3_3B59
        push   0
        push   0
        mov    edi, dword ptr [esp + 20h]
        push   0
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [esp + 24h]
        lea    eax, [ecx + eax - 1]
        push   eax
        push   dword ptr [esp + 24h]
        call   sub_3_2F1C
        add    esp, 14h
loc_3_371D:
        mov    dword ptr [esp + 20h], 28h
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   dword ptr [esp + 18h]
        push   eax
        push   0
        db  068h, 0BCh, 008h, 000h, 000h
        push   dword ptr [esp + 40h]
        db  0E8h, 005h, 0CDh, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_3_3B59
        cmp    dword ptr [esp + 24h], 1
        jne    NEAR PTR loc_3_3B59
        push   0
        push   0
        mov    edi, dword ptr [esp + 20h]
        push   0
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [esp + 24h]
        lea    eax, [ecx + eax - 1]
        push   eax
        push   dword ptr [esp + 24h]
        call   sub_3_2F1C
        add    esp, 14h
        test   byte ptr [ebx + 539h], 1
        je     SHORT loc_3_3801
        mov    dword ptr [esp + 20h], 28h
        lea    eax, [esp + 20h]
        push   eax
        test   byte ptr [ebx + 5a2h], 1
        lea    eax, [esp + 28h]
        push   dword ptr [esp + 18h]
        push   eax
        db  0B8h, 0D4h, 008h, 000h, 000h
        push   0
        jne    SHORT loc_3_37B7
        db  0B8h, 0DCh, 008h, 000h, 000h
loc_3_37B7:
        push   eax
        push   dword ptr [esp + 40h]
        db  0E8h, 087h, 0CCh, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_3_3B59
        cmp    dword ptr [esp + 24h], 1
        jne    NEAR PTR loc_3_3B59
        push   0
        push   0
        mov    edi, dword ptr [esp + 20h]
        push   0
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [esp + 24h]
        lea    eax, [ecx + eax - 1]
        push   eax
        push   dword ptr [esp + 24h]
        call   sub_3_2F1C
        add    esp, 14h
loc_3_3801:
        mov    dword ptr [esp + 20h], 28h
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   dword ptr [esp + 18h]
        push   eax
        push   0
        db  068h, 0C8h, 008h, 000h, 000h
        push   dword ptr [esp + 40h]
        db  0E8h, 021h, 0CCh, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_3864
        cmp    dword ptr [esp + 20h], 1
        jbe    SHORT loc_3_3864
        add    ebx, 5c6h
        mov    dword ptr [ebx], 1
        cmp    dword ptr [esp + 24h], 1
        jne    NEAR PTR loc_3_3B59
        push   0
        push   0
        push   0
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        call   sub_3_2F1C
        add    esp, 14h
        jmp    SHORT loc_3_387A
loc_3_3864:
        db  0BEh, 040h, 009h, 000h, 000h
        mov    edi, dword ptr [esp + 1ch]
        add    ebx, 5c6h
        movsb
        mov    dword ptr [ebx], 0
loc_3_387A:
        mov    dword ptr [esp + 20h], 28h
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   dword ptr [esp + 18h]
        push   eax
        push   0
        db  068h, 0D0h, 00Ah, 000h, 000h
        push   dword ptr [esp + 40h]
        db  0E8h, 0A8h, 0CBh, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_38F9
        cmp    dword ptr [esp + 24h], 1
        jne    NEAR PTR loc_3_3B59
        push   0
        push   0
        push   0
        push   dword ptr [esp + 3ch]
        push   dword ptr [esp + 24h]
        call   sub_3_2F1C
        mov    edi, dword ptr [esp + 44h]
        add    esp, 14h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_3_38F9:
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [esp + 18h]
        lea    edx, [ecx - 1]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [edx + ecx - 1]
        mov    eax, 28h
        sub    eax, esi
        mov    dword ptr [esp + 38h], eax
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, 1
        je     SHORT loc_3_3957
        mov    eax, ecx
        sub    edx, edx
        div    dword ptr [esp + 38h]
        cmp    edx, 1
        sbb    edi, edi
        mov    eax, ecx
        sub    edx, edx
        div    dword ptr [esp + 38h]
        lea    eax, [edi + eax + 1]
loc_3_3957:
        inc    esi
        imul   esi, eax
        add    esi, ecx
        inc    esi
        mov    dword ptr [esp + 20h], esi
        push   1
        push   dword ptr [esp + 24h]
        db  0E8h, 05Bh, 0CBh, 0FCh, 0FFh
        mov    dword ptr [esp + 30h], eax
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_3_3B59
        mov    edx, eax
        cmp    dword ptr [ebx], 0
        jne    NEAR PTR loc_3_3A11
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 28h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, dword ptr [esp + 28h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, dword ptr [esp + 28h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        jmp    NEAR PTR loc_3_3B76
loc_3_3A11:
        mov    byte ptr [esp + 13h], 0
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 28h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    byte ptr [ebp], 0
        je     NEAR PTR loc_3_3B12
loc_3_3A45:
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [esp + 1ch]
        lea    esi, [ecx - 1]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [esi + ecx - 1]
        inc    esi
        cmp    esi, 28h
        jbe    SHORT loc_3_3AD1
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    edx, ecx
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_3_3B08
loc_3_3AD1:
        mov    al, byte ptr [ebp]
        mov    byte ptr [esp + 12h], al
        lea    edi, [esp + 12h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        inc    ebp
loc_3_3B08:
        cmp    byte ptr [ebp], 0
        jne    NEAR PTR loc_3_3A45
loc_3_3B12:
        mov    ecx, dword ptr [esp + 48h]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_3_3B28
        cmp    dword ptr [esp + 34h], 0
        jne    SHORT loc_3_3B28
        mov    edi, dword ptr [esp + 30h]
        jmp    SHORT loc_3_3B2C
loc_3_3B28:
        mov    edi, dword ptr [esp + 1ch]
loc_3_3B2C:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_3_3B76
loc_3_3B59:
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_3_3B76
        push   0
        push   dword ptr [esp + 2ch]
        db  0E8h, 065h, 0C9h, 0FCh, 0FFh
        mov    dword ptr [esp + 30h], 0
        add    esp, 8
loc_3_3B76:
        push   dword ptr [esp + 2ch]
        db  0E8h, 0B1h, 0C8h, 0FCh, 0FFh
        add    esp, 4
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 043h, 0C9h, 0FCh, 0FFh
        mov    eax, dword ptr [esp + 30h]
        add    esp, 8
loc_3_3B94:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
sub_3_3B9C:
        sub    esp, 0c4h
        push   esi
        push   edi
        mov    esi, 1
        mov    edi, dword ptr [esp + 0d0h]
        or     edi, edi
        jne    SHORT loc_3_3BBB
        sub    eax, eax
        jmp    NEAR PTR loc_3_3DC4
loc_3_3BBB:
        mov    eax, dword ptr [esp + 0d4h]
        add    eax, 1000000fh
        cmp    eax, 5
        ja     SHORT loc_3_3BD3
        db  0FFh, 024h, 085h, 0CDh, 03Dh, 000h, 000h
loc_3_3BD3:
        sub    esi, esi
        jmp    NEAR PTR loc_3_3DC2
loc_3_3BDA:
        lea    eax, [esp + 8]
        push   eax
        db  068h, 008h, 008h, 000h, 000h
        push   dword ptr [edi + 2e4h]
        db  0E8h, 039h, 0C8h, 0FCh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_3C00
        mov    esi, 2
        jmp    NEAR PTR loc_3_3DC2
loc_3_3C00:
        cmp    word ptr [edi + 899h], 0
        je     SHORT loc_3_3C0F
        jmp    NEAR PTR loc_3_3DBD
loc_3_3C0F:
        mov    eax, dword ptr [esp + 0d8h]
        or     eax, eax
        jne    SHORT loc_3_3C24
        mov    esi, 2
        jmp    NEAR PTR loc_3_3DC2
loc_3_3C24:
        mov    word ptr [edi + 899h], 1
        mov    dword ptr [edi + 8a1h], eax
        jmp    NEAR PTR loc_3_3DC2
loc_3_3C38:
        cmp    word ptr [edi + 899h], 1
        je     SHORT loc_3_3C47
        jmp    NEAR PTR loc_3_3DBD
loc_3_3C47:
        mov    word ptr [edi + 899h], 0
        mov    word ptr [edi + 89bh], 0
        mov    dword ptr [edi + 89dh], 0
        mov    dword ptr [edi + 8a1h], 0
        jmp    NEAR PTR loc_3_3DC2
loc_3_3C72:
        cmp    word ptr [edi + 899h], 1
        je     SHORT loc_3_3C81
        jmp    NEAR PTR loc_3_3DBD
loc_3_3C81:
        mov    word ptr [edi + 899h], 2
        mov    eax, dword ptr [esp + 0d8h]
        mov    dword ptr [edi + 89dh], eax
        push   80h
        push   edi
        call   sub_3_3E17
        add    esp, 8
        jmp    NEAR PTR loc_3_3DC2
loc_3_3CAA:
        cmp    word ptr [edi + 899h], 2
        je     SHORT loc_3_3CB9
        jmp    NEAR PTR loc_3_3DBD
loc_3_3CB9:
        mov    word ptr [edi + 899h], 1
        mov    eax, dword ptr [edi + 5beh]
        mov    dword ptr [edi + 8a5h], eax
        push   0
        push   edi
        call   sub_3_3E17
        add    esp, 8
        jmp    NEAR PTR loc_3_3DC2
loc_3_3CDE:
        cmp    word ptr [edi + 899h], 2
        je     SHORT loc_3_3CED
        jmp    NEAR PTR loc_3_3DBD
loc_3_3CED:
        cmp    word ptr [edi + 89bh], 80h
        je     SHORT loc_3_3D02
        mov    esi, 0fh
        jmp    NEAR PTR loc_3_3DC2
loc_3_3D02:
        mov    dword ptr [esp + 0ch], edi
        mov    dword ptr [esp + 18h], 1
        mov    dword ptr [esp + 10h], 0
        mov    dword ptr [esp + 14h], 0
        lea    eax, [esp + 0ch]
        mov    byte ptr [esp + 1ch], 0
        push   eax
        call   sub_3_1034
        add    esp, 4
        jmp    NEAR PTR loc_3_3DC2
loc_3_3D35:
        cmp    word ptr [edi + 899h], 2
        jne    SHORT loc_3_3DBD
        cmp    word ptr [edi + 89bh], 80h
        jne    SHORT loc_3_3DBD
        mov    dword ptr [esp + 0ch], edi
        mov    dword ptr [esp + 18h], 2
        mov    dword ptr [esp + 10h], 0
        mov    dword ptr [esp + 14h], 0
        lea    eax, [esp + 0ch]
        mov    byte ptr [esp + 1ch], 0
        push   eax
        call   sub_3_1034
        add    esp, 4
        mov    dword ptr [edi + 2e0h], 4
        mov    dword ptr [esp + 6ch], edi
        mov    eax, dword ptr [edi + 7d3h]
        mov    dword ptr [esp + 70h], eax
        mov    eax, dword ptr [edi + 7d7h]
        mov    dword ptr [esp + 74h], eax
        mov    eax, dword ptr [edi + 7dbh]
        mov    dword ptr [esp + 78h], eax
        mov    eax, dword ptr [edi + 5beh]
        mov    dword ptr [esp + 7ch], eax
        lea    eax, [esp + 6ch]
        push   eax
        call   sub_3_0823
        add    esp, 4
        jmp    SHORT loc_3_3DC2
loc_3_3DBD:
        mov    esi, 0ah
loc_3_3DC2:
        mov    eax, esi
loc_3_3DC4:
        pop    edi
        pop    esi
        add    esp, 0c4h
        ret
loc_3_3DCD:
        fidivr dword ptr [ebx]
        add    byte ptr [eax], al
        cmp    byte ptr [eax + eax], bh
        db  000h, 072h, 03Ch
        add    byte ptr [eax], al
        stosb
        cmp    al, 0
        db  000h, 0DEh
        cmp    al, 0
        db  000h, 035h, 03Dh, 000h, 000h
sub_3_3DE5:
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        je     SHORT loc_3_3E14
        mov    ax, word ptr [esp + 8]
        cmp    ax, 1
        jb     SHORT loc_3_3E14
        cmp    ax, 0bh
        ja     SHORT loc_3_3E14
        movzx  eax, ax
        push   eax
        push   dword ptr [ecx + 89dh]
        push   ecx
        call   dword ptr [ecx + 8a1h]
        add    esp, 0ch
        jmp    SHORT loc_3_3E16
loc_3_3E14:
        sub    eax, eax
loc_3_3E16:
        ret
sub_3_3E17:
        sub    esp, 60h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 70h]
        or     esi, esi
        je     NEAR PTR loc_3_3F42
        mov    ax, word ptr [esi + 899h]
        cmp    ax, 1
        je     SHORT loc_3_3E40
        cmp    ax, 2
        jne    NEAR PTR loc_3_3F42
loc_3_3E40:
        mov    cx, word ptr [esp + 74h]
        or     cx, cx
        je     SHORT loc_3_3E55
        cmp    cx, 80h
        jne    NEAR PTR loc_3_3F42
loc_3_3E55:
        cmp    word ptr [esi + 89bh], cx
        jne    SHORT loc_3_3E68
        mov    eax, 1
        jmp    NEAR PTR loc_3_3F44
loc_3_3E68:
        mov    word ptr [esi + 89bh], cx
        cmp    dword ptr [esi + 2e0h], 4
        je     SHORT loc_3_3E82
        mov    eax, 1
        jmp    NEAR PTR loc_3_3F44
loc_3_3E82:
        mov    dword ptr [esp + 0ch], esi
        mov    eax, dword ptr [esi + 7dbh]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esi + 7d3h]
        cmp    cx, 80h
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esi + 7d7h]
        mov    dword ptr [esp + 14h], eax
        lea    ecx, [esi + 5beh]
        jne    SHORT loc_3_3EC1
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 8a5h], eax
        mov    dword ptr [ecx], 1
        jmp    SHORT loc_3_3EC9
loc_3_3EC1:
        mov    eax, dword ptr [esi + 8a5h]
        mov    dword ptr [ecx], eax
loc_3_3EC9:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 0ch]
        push   eax
        call   sub_3_0842
        add    esp, 4
        mov    ebx, eax
        cmp    ebx, 1
        jne    SHORT loc_3_3F3E
        push   0
        db  0E8h, 00Ah, 0C6h, 0FCh, 0FFh
        mov    dword ptr [esi + 8a9h], eax
        push   esi
        push   7d0h
        db  068h, 04Bh, 03Fh, 000h, 000h
        db  0E8h, 028h, 0C6h, 0FCh, 0FFh
        mov    dword ptr [esi + 8adh], eax
        mov    edi, dword ptr [esi + 8a9h]
        push   5
        push   edi
        db  0E8h, 008h, 0C6h, 0FCh, 0FFh
        mov    eax, dword ptr [esi + 8adh]
        or     eax, eax
        je     SHORT loc_3_3F2E
        push   eax
        db  0E8h, 020h, 0C6h, 0FCh, 0FFh
        mov    dword ptr [esi + 8adh], 0
loc_3_3F2E:
        mov    dword ptr [esi + 8a9h], 0
        push   edi
        db  0E8h, 0C6h, 0C5h, 0FCh, 0FFh
loc_3_3F3E:
        mov    eax, ebx
        jmp    SHORT loc_3_3F44
loc_3_3F42:
        sub    eax, eax
loc_3_3F44:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 60h
        ret
loc_3_3F4B:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 8adh], 0
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 8a9h]
        or     eax, eax
        je     SHORT loc_3_3F74
        push   eax
        db  0E8h, 09Ch, 0C5h, 0FCh, 0FFh
loc_3_3F74:
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_3F80:
        push   esi
        db  08Bh, 015h, 044h, 009h, 000h, 000h
        mov    esi, dword ptr [esp + 8]
        cmp    edx, esi
        jne    SHORT loc_3_3F9C
        mov    eax, dword ptr [esi + 94h]
        db  0A3h, 044h, 009h, 000h, 000h
        jmp    SHORT loc_3_3FC9
loc_3_3F9C:
        cmp    dword ptr [edx + 94h], 0
        je     SHORT loc_3_3FC9
        sub    ecx, ecx
loc_3_3FA7:
        mov    eax, dword ptr [edx + 94h]
        cmp    eax, esi
        je     SHORT loc_3_3FBD
        mov    edx, eax
        cmp    dword ptr [edx + 94h], ecx
        jne    SHORT loc_3_3FA7
        jmp    SHORT loc_3_3FC9
loc_3_3FBD:
        mov    eax, dword ptr [esi + 94h]
        mov    dword ptr [edx + 94h], eax
loc_3_3FC9:
        push   dword ptr [esi + 2e4h]
        db  0E8h, 03Ch, 0C0h, 0FCh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 545h]
        or     eax, eax
        je     SHORT loc_3_3FEC
        push   0
        push   eax
        db  0E8h, 047h, 0C0h, 0FCh, 0FFh
        add    esp, 8
loc_3_3FEC:
        mov    eax, dword ptr [esi + 549h]
        or     eax, eax
        je     SHORT loc_3_4001
        push   0
        push   eax
        db  0E8h, 032h, 0C0h, 0FCh, 0FFh
        add    esp, 8
loc_3_4001:
        mov    eax, dword ptr [esi + 5eeh]
        or     eax, eax
        je     SHORT loc_3_4016
        push   0
        push   eax
        db  0E8h, 01Dh, 0C0h, 0FCh, 0FFh
        add    esp, 8
loc_3_4016:
        mov    eax, dword ptr [esi + 5fah]
        or     eax, eax
        je     SHORT loc_3_402B
        push   0
        push   eax
        db  0E8h, 008h, 0C0h, 0FCh, 0FFh
        add    esp, 8
loc_3_402B:
        mov    eax, dword ptr [esi + 5deh]
        or     eax, eax
        je     SHORT loc_3_4040
        push   0
        push   eax
        db  0E8h, 0F3h, 0BFh, 0FCh, 0FFh
        add    esp, 8
loc_3_4040:
        mov    eax, dword ptr [esi + 792h]
        or     eax, eax
        je     SHORT loc_3_4055
        push   0
        push   eax
        db  0E8h, 0DEh, 0BFh, 0FCh, 0FFh
        add    esp, 8
loc_3_4055:
        mov    eax, dword ptr [esi + 79ah]
        or     eax, eax
        je     SHORT loc_3_406A
        push   0
        push   eax
        db  0E8h, 0C9h, 0BFh, 0FCh, 0FFh
        add    esp, 8
loc_3_406A:
        mov    eax, dword ptr [esi + 79eh]
        or     eax, eax
        je     SHORT loc_3_407F
        push   0
        push   eax
        db  0E8h, 0B4h, 0BFh, 0FCh, 0FFh
        add    esp, 8
loc_3_407F:
        mov    eax, dword ptr [esi + 849h]
        or     eax, eax
        je     SHORT loc_3_409B
        db  03Dh, 018h, 00Dh, 000h, 000h
        je     SHORT loc_3_409B
        push   0
        push   eax
        db  0E8h, 098h, 0BFh, 0FCh, 0FFh
        add    esp, 8
loc_3_409B:
        mov    eax, dword ptr [esi + 5e2h]
        or     eax, eax
        je     SHORT loc_3_40B8
        cmp    dword ptr [esi + 5deh], eax
        je     SHORT loc_3_40B8
        push   0
        push   eax
        db  0E8h, 07Bh, 0BFh, 0FCh, 0FFh
        add    esp, 8
loc_3_40B8:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_4106
        db  00Fh, 0B7h, 005h, 08Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 082h, 0BFh, 0FCh, 0FFh
        add    esp, 8
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_412D
        push   dword ptr [esi + 2b0h]
        db  00Fh, 0B7h, 005h, 08Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 05Bh, 0BFh, 0FCh, 0FFh
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
loc_3_4106:
        or     ecx, ecx
        je     SHORT loc_3_412D
        push   dword ptr [esi + 2b4h]
        db  00Fh, 0B7h, 005h, 090h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 034h, 0BFh, 0FCh, 0FFh
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
loc_3_412D:
        mov    eax, dword ptr [esi + 628h]
        or     eax, eax
        je     SHORT loc_3_4159
        or     ecx, ecx
        je     SHORT loc_3_4159
        push   eax
        db  00Fh, 0B7h, 005h, 092h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 008h, 0BFh, 0FCh, 0FFh
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
loc_3_4159:
        mov    eax, dword ptr [esi + 62ch]
        or     eax, eax
        je     SHORT loc_3_4185
        or     ecx, ecx
        je     SHORT loc_3_4185
        push   eax
        db  00Fh, 0B7h, 005h, 094h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0DCh, 0BEh, 0FCh, 0FFh
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
loc_3_4185:
        mov    eax, dword ptr [esi + 630h]
        or     eax, eax
        je     SHORT loc_3_41B1
        or     ecx, ecx
        je     SHORT loc_3_41B1
        push   eax
        db  00Fh, 0B7h, 005h, 096h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0B0h, 0BEh, 0FCh, 0FFh
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
loc_3_41B1:
        mov    eax, dword ptr [esi + 634h]
        or     eax, eax
        je     SHORT loc_3_41DD
        or     ecx, ecx
        je     SHORT loc_3_41DD
        push   eax
        db  00Fh, 0B7h, 005h, 098h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 084h, 0BEh, 0FCh, 0FFh
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
loc_3_41DD:
        mov    eax, dword ptr [esi + 638h]
        or     eax, eax
        je     SHORT loc_3_4209
        or     ecx, ecx
        je     SHORT loc_3_4209
        push   eax
        db  00Fh, 0B7h, 005h, 09Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 058h, 0BEh, 0FCh, 0FFh
        add    esp, 0ch
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
loc_3_4209:
        mov    eax, dword ptr [esi + 63ch]
        or     eax, eax
        je     SHORT loc_3_422F
        or     ecx, ecx
        je     SHORT loc_3_422F
        push   eax
        db  00Fh, 0B7h, 005h, 09Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 02Ch, 0BEh, 0FCh, 0FFh
        add    esp, 0ch
loc_3_422F:
        cmp    dword ptr [esi + 4f4h], 0
        je     SHORT loc_3_4241
        push   esi
        call   sub_3_00A2
        add    esp, 4
loc_3_4241:
        cmp    dword ptr [esi + 620h], 0
        je     SHORT loc_3_4253
        push   esi
        call   sub_3_00BA
        add    esp, 4
loc_3_4253:
        lea    eax, [esi + 640h]
        push   eax
        call   sub_3_65CD
        mov    eax, dword ptr [esi + 804h]
        or     eax, eax
        je     SHORT loc_3_426F
        push   eax
        db  0E8h, 0C9h, 0BDh, 0FCh, 0FFh
loc_3_426F:
        mov    eax, dword ptr [esi + 808h]
        or     eax, eax
        je     SHORT loc_3_4282
        push   eax
        db  0E8h, 081h, 0BDh, 0FCh, 0FFh
        add    esp, 4
loc_3_4282:
        mov    eax, dword ptr [esi + 810h]
        or     eax, eax
        je     SHORT loc_3_4292
        push   eax
        db  0E8h, 0A6h, 0BDh, 0FCh, 0FFh
loc_3_4292:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_42BA
        lea    eax, [esi + 9ch]
        push   eax
        db  00Fh, 0B7h, 005h, 0D4h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0A1h, 0BDh, 0FCh, 0FFh
        add    esp, 0ch
loc_3_42BA:
        call   sub_3_648B
        push   0
        push   esi
        db  0E8h, 069h, 0BDh, 0FCh, 0FFh
        add    esp, 8
        pop    esi
        ret
sub_3_42CC:
        push   esi
        push   edi
        db  08Bh, 03Dh, 044h, 009h, 000h, 000h
        or     edi, edi
        je     SHORT loc_3_42FA
        mov    esi, dword ptr [esp + 0ch]
loc_3_42DC:
        push   esi
        lea    eax, [edi + 9ch]
        push   eax
        call   sub_3_31A1
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_42FF
        mov    edi, dword ptr [edi + 94h]
        or     edi, edi
        jne    SHORT loc_3_42DC
loc_3_42FA:
        sub    eax, eax
loc_3_42FC:
        pop    edi
        pop    esi
        ret
loc_3_42FF:
        mov    eax, edi
        jmp    SHORT loc_3_42FC
sub_3_4303:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 64h], 46544d53h
        jne    SHORT loc_3_4312
        sub    eax, eax
loc_3_4312:
        ret
sub_3_4313:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 20h], 0
        sub    edi, edi
        lea    eax, [esp + 1ch]
        push   eax
        mov    ecx, dword ptr [esp + 2ch]
        db  068h, 048h, 009h, 000h, 000h
        push   dword ptr [ecx + 2e4h]
        db  0E8h, 0CBh, 0BCh, 0FCh, 0FFh
        add    esp, 0ch
        cmp    eax, edi
        je     SHORT loc_3_434B
        sub    eax, eax
        jmp    NEAR PTR loc_3_44B6
loc_3_434B:
        push   1
        push   200h
        db  0E8h, 0D1h, 0BCh, 0FCh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_3_4373
        push   dword ptr [esp + 1ch]
        db  0E8h, 0A7h, 0BCh, 0FCh, 0FFh
        add    esp, 4
        sub    eax, eax
        jmp    NEAR PTR loc_3_44B6
loc_3_4373:
        lea    ebx, [ebp + 100h]
        sub    esi, esi
loc_3_437B:
        mov    dword ptr [esp + 10h], 100h
        mov    dword ptr [esp + 14h], 0ah
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        push   ebp
        push   esi
        push   dword ptr [esp + 38h]
        db  0E8h, 06Fh, 0BCh, 0FCh, 0FFh
        add    esp, 20h
        or     eax, eax
        jne    SHORT loc_3_43ED
        cmp    dword ptr [esp + 14h], 0ah
        jne    NEAR PTR loc_3_449B
        cmp    dword ptr [esp + 18h], 3
        jne    NEAR PTR loc_3_449B
        push   0
        lea    eax, [esp + 14h]
        push   0
        push   eax
        push   ebx
        push   ebp
        call   sub_3_2F1C
        add    esp, 14h
        or     eax, eax
        je     NEAR PTR loc_3_449B
        mov    eax, dword ptr [esp + 10h]
        add    eax, 0fh
        add    edi, eax
        inc    esi
        jmp    SHORT loc_3_437B
loc_3_43ED:
        cmp    eax, 103h
        jne    NEAR PTR loc_3_449B
        push   1
        inc    edi
        push   edi
        db  0E8h, 027h, 0BCh, 0FCh, 0FFh
        mov    ecx, dword ptr [esp + 30h]
        add    esp, 8
        mov    dword ptr [ecx + 5fah], eax
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_3_449B
        sub    edi, edi
        cmp    esi, edi
        je     SHORT loc_3_4493
loc_3_441E:
        mov    dword ptr [esp + 10h], 100h
        mov    dword ptr [esp + 14h], 0ah
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [ebx + 4]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        push   ebp
        push   edi
        push   dword ptr [esp + 38h]
        db  0E8h, 0CAh, 0BBh, 0FCh, 0FFh
        add    esp, 20h
        or     eax, eax
        push   0
        jne    SHORT loc_3_44BE
        lea    eax, [esp + 14h]
        push   0
        push   eax
        lea    eax, [ebx + 0fh]
        push   eax
        push   ebp
        call   sub_3_2F1C
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_3_44D2
        mov    eax, dword ptr [esp + 10h]
        dec    al
        mov    byte ptr [ebx + 0eh], al
        mov    eax, edi
        sub    eax, esi
        cmp    eax, -1
        je     SHORT loc_3_448C
        mov    eax, dword ptr [esp + 10h]
        add    eax, ebx
        add    eax, 0fh
        mov    dword ptr [ebx], eax
loc_3_448C:
        inc    edi
        mov    ebx, dword ptr [ebx]
        cmp    edi, esi
        jb     SHORT loc_3_441E
loc_3_4493:
        mov    dword ptr [esp + 20h], 1
loc_3_449B:
        push   dword ptr [esp + 1ch]
        db  0E8h, 06Ch, 0BBh, 0FCh, 0FFh
        add    esp, 4
        push   0
        push   ebp
        db  0E8h, 081h, 0BBh, 0FCh, 0FFh
        mov    eax, dword ptr [esp + 28h]
        add    esp, 8
loc_3_44B6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_3_44BE:
        mov    ecx, dword ptr [esp + 2ch]
        push   dword ptr [ecx + 5fah]
        db  0E8h, 063h, 0BBh, 0FCh, 0FFh
        add    esp, 8
        jmp    SHORT loc_3_449B
loc_3_44D2:
        mov    ecx, dword ptr [esp + 28h]
        push   0
        push   dword ptr [ecx + 5fah]
        db  0E8h, 04Dh, 0BBh, 0FCh, 0FFh
        add    esp, 8
        jmp    SHORT loc_3_449B
sub_3_44E8:
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 5ch]
        call   sub_3_42CC
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_451D
        push   1
        push   8b1h
        db  0E8h, 01Bh, 0BBh, 0FCh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_451D
        sub    eax, eax
        jmp    NEAR PTR loc_3_51E3
loc_3_451D:
        push   1
        push   400h
        db  0E8h, 0FFh, 0BAh, 0FCh, 0FFh
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_3_4544
        push   0
        push   ebx
        db  0E8h, 0F6h, 0BAh, 0FCh, 0FFh
        add    esp, 8
        sub    eax, eax
        jmp    NEAR PTR loc_3_51E3
loc_3_4544:
        mov    edi, dword ptr [esp + 5ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 9ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    dword ptr [esp + 14h], 100h
        lea    edi, [ebx + 19ch]
        lea    eax, [esp + 14h]
        push   0
        push   eax
        push   edi
        push   1
        db  068h, 06Ch, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 0BDh, 0BAh, 0FCh, 0FFh
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_3_45C7
        mov    dword ptr [esp + 14h], 100h
        lea    eax, [esp + 14h]
        push   1
        push   eax
        push   edi
        push   1
        db  068h, 054h, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 094h, 0BAh, 0FCh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    NEAR PTR loc_3_51C4
loc_3_45C7:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, 0ffh
        sub    eax, ecx
        mov    dword ptr [esp + 14h], eax
        lea    esi, [ecx + ebx]
        lea    eax, [esp + 14h]
        push   1
        push   eax
        lea    eax, [esi + 19dh]
        push   eax
        push   1
        db  068h, 060h, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 04Fh, 0BAh, 0FCh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_3_460F
        mov    byte ptr [esi + 19ch], 5ch
loc_3_460F:
        mov    dword ptr [ebx + 29ch], 0
        mov    dword ptr [ebx + 2e0h], 1
        mov    eax, dword ptr [esp + 58h]
        mov    dword ptr [ebx + 98h], eax
        mov    dword ptr [ebx + 5e2h], 0
        push   1
        push   100h
        push   ebp
        push   0
        mov    dword ptr [ebx + 61ch], 0ffffffffh
        push   dword ptr [esp + 68h]
        db  0E8h, 0F4h, 0B9h, 0FCh, 0FFh
        add    esp, 14h
        lea    eax, [ebx + 2e4h]
        push   eax
        push   ebp
        push   80000002h
        db  0E8h, 09Fh, 0B9h, 0FCh, 0FFh
        mov    dword ptr [esp + 20h], 1
        add    esp, 0ch
        lea    eax, [esp + 14h]
        push   1
        push   eax
        lea    eax, [esp + 1bh]
        push   eax
        push   3
        db  068h, 090h, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 0BEh, 0B9h, 0FCh, 0FFh
        add    esp, 1ch
        cmp    dword ptr [esp + 14h], 1
        jne    SHORT loc_3_470F
        or     eax, eax
        jne    SHORT loc_3_470F
        cmp    byte ptr [esp + 13h], 1
        jne    SHORT loc_3_470F
        mov    dword ptr [esp + 14h], 400h
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   ebp
        push   eax
        push   0
        db  068h, 098h, 009h, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 054h, 0B9h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_3_51C4
        cmp    dword ptr [esp + 18h], 1
        jne    NEAR PTR loc_3_51C4
        push   ebp
        call   sub_3_63A0
        add    esp, 4
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_470F
        push   dword ptr [esp + 5ch]
        db  00Fh, 0B7h, 00Dh, 0CCh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 04Ch, 0B9h, 0FCh, 0FFh
        add    esp, 0ch
loc_3_470F:
        mov    dword ptr [esp + 14h], 4
        lea    esi, [ebx + 5aeh]
        lea    eax, [esp + 14h]
        push   1
        push   eax
        push   esi
        push   3
        db  068h, 0F8h, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 019h, 0B9h, 0FCh, 0FFh
        add    esp, 1ch
        cmp    dword ptr [esp + 14h], 4
        jne    SHORT loc_3_474A
        or     eax, eax
        jne    SHORT loc_3_474A
        cmp    dword ptr [esi], 0
        jne    SHORT loc_3_4750
loc_3_474A:
        mov    dword ptr [esi], 0ah
loc_3_4750:
        mov    dword ptr [esp + 14h], 20h
        lea    eax, [esp + 14h]
        push   1
        push   eax
        lea    eax, [esp + 3ch]
        push   eax
        push   3
        db  068h, 078h, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 0DAh, 0B8h, 0FCh, 0FFh
        add    esp, 1ch
        cmp    dword ptr [esp + 14h], 20h
        jne    NEAR PTR loc_3_51C4
        or     eax, eax
        jne    NEAR PTR loc_3_51C4
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [ebx + 524h], eax
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [ebx + 528h], eax
        mov    eax, dword ptr [esi]
        imul   eax, dword ptr [esp + 3ch]
        mov    dword ptr [ebx + 52ch], eax
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [ebx + 530h], eax
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [ebx + 534h], eax
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [ebx + 538h], eax
        mov    eax, dword ptr [esp + 4ch]
        mov    dword ptr [ebx + 53ch], eax
        mov    eax, dword ptr [esp + 50h]
        mov    dword ptr [ebx + 540h], eax
        mov    dword ptr [ebx + 4f8h], 4dh
        mov    dword ptr [ebx + 4fch], 4dh
        mov    dword ptr [ebx + 500h], 0
        mov    dword ptr [ebx + 504h], 0
        mov    dword ptr [ebx + 508h], 0
        lea    eax, [esp + 14h]
        mov    dword ptr [ebx + 50ch], 0
        mov    dword ptr [ebx + 510h], 0
        mov    dword ptr [ebx + 514h], 0
        mov    dword ptr [ebx + 518h], 0
        mov    dword ptr [ebx + 51ch], 0
        mov    dword ptr [ebx + 520h], 0
        push   eax
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        push   0
        db  068h, 0C4h, 009h, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 0B5h, 0B7h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_48C5
        cmp    dword ptr [esp + 18h], 1
        jne    SHORT loc_3_48C5
        push   1
        push   dword ptr [esp + 18h]
        db  0E8h, 0A4h, 0B7h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [ebx + 545h], eax
        or     eax, eax
        je     SHORT loc_3_48CF
        lea    ecx, [esp + 14h]
        push   ecx
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   0
        db  068h, 0C4h, 009h, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 072h, 0B7h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_3_48CF
        push   0
        push   dword ptr [ebx + 545h]
        db  0E8h, 06Eh, 0B7h, 0FCh, 0FFh
        add    esp, 8
loc_3_48C5:
        mov    dword ptr [ebx + 545h], 0
loc_3_48CF:
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        push   0
        db  068h, 0D4h, 009h, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 033h, 0B7h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_4947
        cmp    dword ptr [esp + 18h], 1
        jne    SHORT loc_3_4947
        push   1
        push   dword ptr [esp + 18h]
        db  0E8h, 022h, 0B7h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [ebx + 549h], eax
        or     eax, eax
        je     SHORT loc_3_4951
        lea    ecx, [esp + 14h]
        push   ecx
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   0
        db  068h, 0D4h, 009h, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 0F0h, 0B6h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_3_4951
        push   0
        push   dword ptr [ebx + 549h]
        db  0E8h, 0ECh, 0B6h, 0FCh, 0FFh
        add    esp, 8
loc_3_4947:
        mov    dword ptr [ebx + 549h], 0
loc_3_4951:
        mov    dword ptr [esp + 14h], 14h
        lea    eax, [esp + 14h]
        push   1
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        push   3
        db  068h, 084h, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 0D9h, 0B6h, 0FCh, 0FFh
        add    esp, 1ch
        cmp    dword ptr [esp + 14h], 14h
        jne    NEAR PTR loc_3_51C4
        or     eax, eax
        jne    NEAR PTR loc_3_51C4
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebx + 591h], eax
        mov    eax, dword ptr [esi]
        imul   eax, dword ptr [esp + 24h]
        mov    dword ptr [ebx + 595h], eax
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ebx + 599h], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ebx + 59dh], eax
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [ebx + 5a1h], eax
        mov    dword ptr [ebx + 581h], 30h
        mov    dword ptr [ebx + 585h], 30h
        mov    dword ptr [ebx + 589h], 0
        mov    dword ptr [ebx + 58dh], 0
        mov    dword ptr [ebx + 5a5h], 0
        mov    dword ptr [ebx + 5a9h], 0
        mov    ecx, dword ptr [ebx + 538h]
        and    ecx, eax
        mov    dword ptr [ebx + 5a1h], ecx
        mov    dword ptr [ebx + 5c2h], 0
        mov    dword ptr [esp + 14h], 1ch
        lea    eax, [esp + 14h]
        push   1
        push   eax
        lea    eax, [ebx + 555h]
        push   eax
        push   3
        db  068h, 08Ch, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 012h, 0B6h, 0FCh, 0FFh
        add    esp, 1ch
        cmp    dword ptr [esp + 14h], 1ah
        je     SHORT loc_3_4A53
        cmp    dword ptr [esp + 14h], 1ch
        jne    NEAR PTR loc_3_51C4
loc_3_4A53:
        or     eax, eax
        jne    NEAR PTR loc_3_51C4
        mov    dword ptr [ebx + 54dh], 61h
        mov    word ptr [ebx + 551h], 1
        mov    dword ptr [ebx + 571h], 6
        mov    dword ptr [ebx + 575h], 34h
        lea    eax, [esp + 14h]
        mov    dword ptr [ebx + 579h], 2dh
        mov    dword ptr [esp + 14h], 100h
        push   eax
        lea    eax, [esp + 1ch]
        push   ebp
        push   eax
        push   0
        db  068h, 0DCh, 009h, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 06Fh, 0B5h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_3_4B3F
        cmp    dword ptr [esp + 18h], 1
        jne    SHORT loc_3_4B3F
        cmp    dword ptr [esp + 14h], 1
        jbe    SHORT loc_3_4B3F
        lea    esi, [ebp + 100h]
        push   0
        push   0
        push   0
        push   esi
        mov    edi, esi
        push   ebp
        call   sub_3_2F1C
        add    esp, 14h
        push   1
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 033h, 0B5h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [ebx + 5eeh], eax
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_3_4B49
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebx + 5eeh]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 5f2h], ecx
        jmp    SHORT loc_3_4B49
loc_3_4B3F:
        mov    dword ptr [ebx + 5eeh], 0
loc_3_4B49:
        lea    eax, [ebp + 100h]
        mov    dword ptr [esp + 1ch], eax
        mov    dword ptr [esp + 14h], 4
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [ebx + 824h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   0
        db  068h, 03Ch, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 0A2h, 0B4h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_3_501F
        cmp    dword ptr [esp + 18h], 3
        jne    NEAR PTR loc_3_501F
        cmp    dword ptr [esp + 14h], 4
        jb     NEAR PTR loc_3_501F
        mov    dword ptr [esp + 14h], 10h
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [ebx + 814h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   0
        db  068h, 04Ch, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 056h, 0B4h, 0FCh, 0FFh
        mov    dword ptr [esp + 2ch], 4
        add    esp, 18h
        lea    esi, [ebx + 80ch]
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   esi
        push   eax
        push   0
        db  068h, 060h, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 028h, 0B4h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_4C0D
        cmp    dword ptr [esp + 18h], 3
        jne    SHORT loc_3_4C0D
        cmp    dword ptr [esp + 14h], 4
        jae    SHORT loc_3_4C13
loc_3_4C0D:
        mov    dword ptr [esi], 1eh
loc_3_4C13:
        mov    dword ptr [esp + 14h], 100h
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   ebp
        push   eax
        push   0
        db  068h, 008h, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 0E8h, 0B3h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_4CBA
        cmp    dword ptr [esp + 18h], 1
        jne    SHORT loc_3_4CBA
        push   0
        push   0
        push   0
        push   dword ptr [esp + 28h]
        push   ebp
        call   sub_3_2F1C
        add    esp, 14h
        mov    edi, dword ptr [esp + 1ch]
        push   1
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 0B8h, 0B3h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [ebx + 792h], eax
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_3_4CBA
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebx + 792h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 796h], ecx
loc_3_4CBA:
        mov    dword ptr [esp + 14h], 100h
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   ebp
        push   eax
        push   0
        db  068h, 01Ch, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 041h, 0B3h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_4D49
        cmp    dword ptr [esp + 18h], 1
        jne    SHORT loc_3_4D49
        push   0
        push   0
        push   0
        push   dword ptr [esp + 28h]
        push   ebp
        call   sub_3_2F1C
        add    esp, 14h
        mov    edi, dword ptr [esp + 1ch]
        push   1
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 011h, 0B3h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [ebx + 79ah], eax
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_3_4D49
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_4D49:
        mov    dword ptr [esp + 14h], 100h
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   ebp
        push   eax
        push   0
        db  068h, 02Ch, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 0B2h, 0B2h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_4DD8
        cmp    dword ptr [esp + 18h], 1
        jne    SHORT loc_3_4DD8
        push   0
        push   0
        push   0
        push   dword ptr [esp + 28h]
        push   ebp
        call   sub_3_2F1C
        add    esp, 14h
        mov    edi, dword ptr [esp + 1ch]
        push   1
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 082h, 0B2h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [ebx + 79eh], eax
        mov    edx, eax
        or     edx, edx
        je     SHORT loc_3_4DD8
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_4DD8:
        test   byte ptr [ebx + 824h], 20h
        je     NEAR PTR loc_3_4FAD
        mov    dword ptr [esp + 14h], 14h
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   ebp
        push   eax
        push   0
        db  068h, 074h, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 016h, 0B2h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_4E7D
        cmp    dword ptr [esp + 18h], 1
        jne    SHORT loc_3_4E7D
        push   0
        push   0
        push   0
        push   dword ptr [esp + 28h]
        push   ebp
        call   sub_3_2F1C
        mov    edi, dword ptr [esp + 30h]
        add    esp, 14h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 14h
        jae    SHORT loc_3_4E7D
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 869h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 87dh], ecx
loc_3_4E7D:
        mov    dword ptr [esp + 14h], 14h
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   ebp
        push   eax
        push   0
        db  068h, 084h, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 07Eh, 0B1h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_4F15
        cmp    dword ptr [esp + 18h], 1
        jne    SHORT loc_3_4F15
        push   0
        push   0
        push   0
        push   dword ptr [esp + 28h]
        push   ebp
        call   sub_3_2F1C
        mov    edi, dword ptr [esp + 30h]
        add    esp, 14h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 14h
        jae    SHORT loc_3_4F15
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 881h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 895h], ecx
loc_3_4F15:
        mov    dword ptr [esp + 14h], 14h
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   ebp
        push   eax
        push   0
        db  068h, 090h, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 0E6h, 0B0h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_4FAD
        cmp    dword ptr [esp + 18h], 1
        jne    SHORT loc_3_4FAD
        push   0
        push   0
        push   0
        push   dword ptr [esp + 28h]
        push   ebp
        call   sub_3_2F1C
        mov    edi, dword ptr [esp + 30h]
        add    esp, 14h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    ecx, 14h
        jae    SHORT loc_3_4FAD
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 851h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 865h], ecx
loc_3_4FAD:
        mov    dword ptr [esp + 14h], 80h
        push   1
        push   80h
        db  0E8h, 067h, 0B0h, 0FCh, 0FFh
        add    esp, 8
        mov    dword ptr [ebx + 849h], eax
        or     eax, eax
        je     SHORT loc_3_5029
        lea    ecx, [esp + 14h]
        push   ecx
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   0
        db  068h, 0A0h, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 035h, 0B0h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_3_5003
        cmp    dword ptr [esp + 18h], 3
        jne    SHORT loc_3_5003
        cmp    dword ptr [esp + 14h], 80h
        jae    SHORT loc_3_5029
loc_3_5003:
        push   0
        push   dword ptr [ebx + 849h]
        db  0E8h, 020h, 0B0h, 0FCh, 0FFh
        add    esp, 8
        db  0C7h, 083h, 049h, 008h, 000h, 000h, 018h, 00Dh, 000h, 000h
        jmp    SHORT loc_3_5029
loc_3_501F:
        mov    dword ptr [ebx + 824h], 1080h
loc_3_5029:
        mov    dword ptr [ebx + 838h], 0
        mov    dword ptr [ebx + 83ch], 0
        mov    dword ptr [ebx + 834h], 0
        mov    dword ptr [ebx + 5beh], 1
        lea    eax, [ebx + 7a2h]
        push   ebx
        push   0
        push   0
        push   0
        push   dword ptr [ebx + 849h]
        push   eax
        call   sub_3_7ED0
        push   ebx
        call   sub_3_4313
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_3_51C4
        mov    dword ptr [esp + 14h], 100h
        lea    eax, [esp + 14h]
        push   1
        push   eax
        push   ebp
        push   1
        db  068h, 0A4h, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 0B3h, 0AFh, 0FCh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_3_50C5
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_50C5
        push   ebp
        db  00Fh, 0B7h, 00Dh, 0CEh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 096h, 0AFh, 0FCh, 0FFh
        add    esp, 0ch
loc_3_50C5:
        mov    dword ptr [esp + 14h], 100h
        lea    eax, [esp + 14h]
        push   1
        push   eax
        push   ebp
        push   1
        db  068h, 0B0h, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 069h, 0AFh, 0FCh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_3_510F
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_510F
        push   ebp
        db  00Fh, 0B7h, 00Dh, 0D0h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 04Ch, 0AFh, 0FCh, 0FFh
        add    esp, 0ch
loc_3_510F:
        mov    dword ptr [esp + 14h], 100h
        lea    eax, [esp + 14h]
        push   1
        push   eax
        push   ebp
        push   1
        db  068h, 0B8h, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 01Fh, 0AFh, 0FCh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_3_5159
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_5159
        push   ebp
        db  00Fh, 0B7h, 00Dh, 0D2h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 002h, 0AFh, 0FCh, 0FFh
        add    esp, 0ch
loc_3_5159:
        mov    dword ptr [esp + 14h], 1
        lea    eax, [esp + 14h]
        push   1
        push   eax
        lea    eax, [esp + 1ah]
        push   eax
        push   3
        db  068h, 0E4h, 009h, 000h, 000h
        push   0
        push   dword ptr [esp + 70h]
        db  0E8h, 0D1h, 0AEh, 0FCh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    SHORT loc_3_51C4
        cmp    dword ptr [esp + 14h], 1
        jne    SHORT loc_3_51C4
        movzx  eax, byte ptr [esp + 12h]
        or     eax, eax
        je     SHORT loc_3_51EB
        cmp    eax, 2
        je     SHORT loc_3_51A0
        cmp    eax, 4
        je     SHORT loc_3_51EB
loc_3_51A0:
        mov    dword ptr [ebx + 618h], 1
        push   0
        db  068h, 0F0h, 009h, 000h, 000h
        push   ebx
        call   sub_3_31F4
        add    esp, 0ch
        mov    dword ptr [ebx + 5deh], eax
        or     eax, eax
        jne    SHORT loc_3_51F5
loc_3_51C4:
        call   sub_3_648B
        push   0
        push   ebx
        db  0E8h, 05Fh, 0AEh, 0FCh, 0FFh
        add    esp, 8
        sub    ebx, ebx
loc_3_51D6:
        push   0
        push   ebp
        db  0E8h, 052h, 0AEh, 0FCh, 0FFh
        add    esp, 8
        mov    eax, ebx
loc_3_51E3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 44h
        ret
loc_3_51EB:
        mov    dword ptr [ebx + 618h], 0
loc_3_51F5:
        db  0A1h, 044h, 009h, 000h, 000h
        or     eax, eax
        mov    dword ptr [ebx + 94h], eax
        jne    SHORT loc_3_520E
        mov    dword ptr [ebx + 94h], 0
loc_3_520E:
        db  089h, 01Dh, 044h, 009h, 000h, 000h
        jmp    SHORT loc_3_51D6
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_5220:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], 0
        mov    ebx, dword ptr [esp + 1ch]
        or     ebx, ebx
        je     NEAR PTR loc_3_53F1
        mov    ebp, dword ptr [esp + 18h]
        mov    ecx, dword ptr [ebp + 29ch]
        or     ecx, ecx
        je     NEAR PTR loc_3_53F1
        cmp    dword ptr [ebx + 34h], 0e73cf52eh
        jne    SHORT loc_3_5268
        mov    ax, word ptr [ebx]
        cmp    ax, 40h
        jae    SHORT loc_3_5263
        jmp    NEAR PTR loc_3_53F1
loc_3_5263:
        mov    word ptr [esp + 12h], ax
loc_3_5268:
        push   ebx
        push   ecx
        db  0E8h, 009h, 0B2h, 0FCh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_527B
        jmp    NEAR PTR loc_3_53F1
loc_3_527B:
        mov    dword ptr [ebx + 18h], 6
        or     dword ptr [ebx + 4], 1
        cmp    word ptr [esp + 12h], 8ch
        jb     NEAR PTR loc_3_53EA
        add    ebx, 3ch
        lea    esi, [ebp + 4f8h]
        mov    edi, ebx
        mov    ecx, 13h
        rep movsd
        movsb
        mov    edi, dword ptr [ebp + 545h]
        or     edi, edi
        je     NEAR PTR loc_3_533C
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        add    dword ptr [ebx + 4], ecx
        mov    edi, dword ptr [ebp + 545h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ecx, 8bh
        movzx  eax, word ptr [esp + 12h]
        cmp    ecx, eax
        ja     SHORT loc_3_533C
        mov    esi, dword ptr [ebp + 545h]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [ebx]
        lea    eax, [ecx - 1]
        add    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [ebx]
        mov    dword ptr [ebx + 14h], edx
        mov    edi, dword ptr [ebp + 545h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 18h], ecx
        mov    edi, dword ptr [ebp + 545h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [edx + ecx - 1]
        mov    dword ptr [ebx], eax
loc_3_533C:
        mov    edi, dword ptr [ebp + 549h]
        or     edi, edi
        je     NEAR PTR loc_3_53EA
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        add    dword ptr [ebx + 4], ecx
        mov    edi, dword ptr [ebp + 545h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [ebp + 549h]
        lea    edx, [ecx - 1]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [edx + ecx - 1]
        add    edx, 8ch
        movzx  eax, word ptr [esp + 12h]
        cmp    edx, eax
        ja     SHORT loc_3_53EA
        mov    esi, dword ptr [ebp + 549h]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [ebx]
        lea    eax, [ecx - 1]
        add    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [ebx]
        mov    dword ptr [ebx + 1ch], edx
        mov    edi, dword ptr [ebp + 549h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 20h], ecx
        mov    edi, dword ptr [ebp + 549h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [edx + ecx - 1]
        mov    dword ptr [ebx], eax
loc_3_53EA:
        mov    eax, 1
        jmp    SHORT loc_3_53F3
loc_3_53F1:
        sub    eax, eax
loc_3_53F3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_3_53FB:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 44h]
        or     edi, edi
        mov    ebx, dword ptr [esp + 48h]
        je     NEAR PTR loc_3_54B9
        or     ebx, ebx
        je     NEAR PTR loc_3_54C3
        cmp    dword ptr [ebx], 34h
        jb     NEAR PTR loc_3_54B9
        mov    esi, dword ptr [esp + 40h]
        mov    eax, dword ptr [esi + 29ch]
        or     eax, eax
        je     NEAR PTR loc_3_54B9
        mov    word ptr [edi + 4], 1
        mov    dword ptr [edi + 24h], 6
        lea    ecx, [esp + 0ch]
        push   ecx
        push   eax
        db  0E8h, 014h, 0B0h, 0FCh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_5455
        jmp    SHORT loc_3_54C3
loc_3_5455:
        lea    eax, [edi + 8]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        db  0E8h, 05Dh, 0B2h, 0FCh, 0FFh
        add    esp, 8
        cmp    dword ptr [ebx], 60h
        jae    SHORT loc_3_5487
        mov    dword ptr [ebx], 34h
        mov    dword ptr [edi], 34h
        mov    dword ptr [edi + 28h], 0
        mov    dword ptr [edi + 2ch], 0
        jmp    SHORT loc_3_54B2
loc_3_5487:
        mov    dword ptr [ebx], 60h
        mov    dword ptr [edi], 60h
        mov    dword ptr [edi + 28h], 30h
        mov    dword ptr [edi + 2ch], 30h
        add    esi, 581h
        add    edi, 30h
        mov    ecx, 0bh
        rep movsd
        movsb
loc_3_54B2:
        mov    eax, 1
        jmp    SHORT loc_3_54C5
loc_3_54B9:
        or     ebx, ebx
        je     SHORT loc_3_54C3
        mov    dword ptr [ebx], 34h
loc_3_54C3:
        sub    eax, eax
loc_3_54C5:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
_PTEXT ends
        end
