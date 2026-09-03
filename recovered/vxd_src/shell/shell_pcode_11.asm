; shell_pcode_11.asm -- SHELL object 11 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_11_0000:
        mov    word ptr [ebp + 1ch], 400h
        ret
SHELL_Hook_Properties:
        push   ebx
        push   esi
        push   edi
        cmp    eax, 0ffh
        ja     SHORT loc_11_001D
        or     eax, eax
        je     SHORT loc_11_001A
        cmp    eax, 0ah
        jbe    SHORT loc_11_001D
loc_11_001A:
        stc
        jmp    SHORT loc_11_0077
loc_11_001D:
        mov    ebx, esi
        xchg   edx, eax
        xchg   edi, eax
        db  08Bh, 035h, 020h, 000h, 000h, 000h
        or     esi, esi
        jne    SHORT loc_11_0043
        mov    eax, 4
        mov    ecx, 0ch
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_11_0077
        db  089h, 035h, 020h, 000h, 000h, 000h
loc_11_0043:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_11_0077
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], ebx
        push   eax
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_11_006B
loc_11_005C:
        db  0E8h, 07Ch, 000h, 0F5h, 0FFh
        je     SHORT loc_11_006D
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_11_005C
loc_11_006B:
        sub    eax, eax
loc_11_006D:
        mov    ecx, eax
        pop    eax
        db  0CDh, 020h, 0A0h, 000h, 001h, 000h
        clc
loc_11_0077:
        pop    edi
        pop    esi
        pop    ebx
        ret
SHELL_Unhook_Properties:
        push   ebx
        push   esi
        cmp    eax, 0ffh
        ja     SHORT loc_11_008C
        cmp    eax, 0ah
        jbe    SHORT loc_11_008C
        stc
        jmp    SHORT loc_11_00C7
loc_11_008C:
        mov    ebx, esi
        mov    edx, eax
        db  08Bh, 035h, 020h, 000h, 000h, 000h
        or     esi, esi
        je     SHORT loc_11_00C7
loc_11_009A:
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_11_00C7
loc_11_00A2:
        db  0E8h, 036h, 000h, 0F5h, 0FFh
        jne    SHORT loc_11_00BE
        cmp    dword ptr [eax + 8], ebx
        jne    SHORT loc_11_00BE
        push   eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        jmp    SHORT loc_11_009A
loc_11_00BE:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_11_00A2
        clc
loc_11_00C7:
        pop    esi
        pop    ebx
        ret
loc_11_00CA:
        db  080h, 03Dh, 018h, 00Ah, 000h, 000h, 001h
        jne    SHORT loc_11_0108
        xor    esi, esi
        db  087h, 035h, 014h, 00Ah, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        movzx  ecx, word ptr [ebp + 10h]
        jecxz  loc_11_0103
        imul   eax, ecx, 3e8h
        db  0A3h, 010h, 00Ah, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 014h, 00Ah, 000h, 000h
loc_11_0103:
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_11_0108:
        or     byte ptr [ebp + 2ch], 1
        ret
loc_11_010D:
        db  0CDh, 020h, 03Fh, 001h, 001h, 000h
        xchg   ecx, eax
        movzx  eax, word ptr [ebp + 1ch]
loc_11_0118:
        mov    ecx, dword ptr [ecx]
        jecxz  loc_11_0125
        cmp    word ptr [ecx + 6], ax
        jne    SHORT loc_11_0118
        mov    ecx, dword ptr [ecx + 4]
loc_11_0125:
        mov    dword ptr [ebp + 1ch], ecx
        ret
loc_11_0129:
        db  08Bh, 035h, 024h, 000h, 000h, 000h
        db  08Bh, 03Dh, 028h, 000h, 000h, 000h
        cmp    edi, 280h
        jne    SHORT loc_11_0142
        mov    edi, 0ffffh
loc_11_0142:
        cmp    esi, 280h
        jne    SHORT loc_11_014F
        mov    esi, 0ffffh
loc_11_014F:
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        shl    eax, 2
        cmp    esi, 0ffffh
        jne    SHORT loc_11_016E
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        shl    eax, 2
        mov    esi, eax
        jmp    SHORT loc_11_0170
loc_11_016E:
        add    esi, eax
loc_11_0170:
        cmp    edi, 0ffffh
        jne    SHORT loc_11_0186
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        inc    eax
        shl    eax, 2
        mov    edi, eax
        jmp    SHORT loc_11_0188
loc_11_0186:
        add    edi, eax
loc_11_0188:
        cmp    edi, esi
        jbe    SHORT loc_11_018E
        mov    edi, esi
loc_11_018E:
        xor    ecx, ecx
        db  08Bh, 015h, 000h, 00Ah, 000h, 000h
        test   byte ptr [edx + ebx + 0eh], 4
        je     SHORT loc_11_01A0
        or     ecx, 1
loc_11_01A0:
        db  0CDh, 020h, 001h, 000h, 006h, 000h
        jb     NEAR PTR loc_11_0235
        db  0A3h, 024h, 000h, 000h, 000h
        db  0CDh, 020h, 032h, 000h, 001h, 000h
        db  08Bh, 035h, 000h, 00Ah, 000h, 000h
        and    byte ptr [esi + ebx + 0ch], 0f2h
        or     byte ptr [esi + ebx + 0ch], 2
        mov    word ptr [esi + ebx + 2ah], cx
        mov    word ptr [esi + ebx + 2ch], dx
        and    eax, 0fffffffeh
        or     eax, 2
        db  00Bh, 005h, 030h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  0CDh, 020h, 004h, 000h, 00Ah, 000h
        pop    eax
        db  0CDh, 020h, 033h, 000h, 001h, 000h
        jb     SHORT loc_11_0235
        db  08Bh, 03Dh, 02Ch, 000h, 000h, 000h
        lea    eax, [edi]
        lea    ecx, [edi + 100h]
        lea    edx, [edi + 80h]
        push   ecx
        push   eax
        push   edx
        push   ebx
        db  0CDh, 020h, 001h, 000h, 015h, 000h
        add    esp, 10h
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        push   0
        db  068h, 07Bh, 001h, 000h, 000h
        db  0FFh, 035h, 024h, 000h, 000h, 000h
        push   ebx
        db  0CDh, 020h, 003h, 000h, 015h, 000h
        add    esp, 10h
        clc
        ret
loc_11_0235:
        push   80004h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        pushad
        mov    esi, eax
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_11_0250:
        mov    eax, dword ptr [esp + 0ch]
        pushad
        cmp    byte ptr [eax + 11h], 3
        jne    SHORT loc_11_0281
        mov    edi, dword ptr [eax + 0ch]
        db  08Bh, 00Dh, 000h, 00Ah, 000h, 000h
        mov    ebx, dword ptr [edi + 14h]
        cmp    word ptr [ecx + ebx + 36h], 0
        je     SHORT loc_11_0281
        or     byte ptr [ecx + ebx + 0fh], 8
        xor    esi, esi
        xor    edx, edx
        lea    eax, [edx + 0fh]
        db  0CDh, 020h, 093h, 000h, 001h, 000h
loc_11_0281:
        popad
        ret
loc_11_0283:
        push   ebx
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax + 11h], 3
        jne    SHORT loc_11_029E
        mov    eax, dword ptr [eax + 0ch]
        mov    ebx, dword ptr [eax + 14h]
        db  0A1h, 000h, 00Ah, 000h, 000h
        and    byte ptr [eax + ebx + 0fh], 0f7h
loc_11_029E:
        pop    ebx
        ret
loc_11_02A0:
        mov    ebx, edx
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_11_02BD
        db  0E8h, 060h, 0FDh, 0F4h, 0FFh
        and    byte ptr [eax + 6], 0fdh
        push   eax
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
loc_11_02BD:
        ret
loc_11_02BE:
        db  0CDh, 020h, 041h, 001h, 001h, 000h
        cmp    eax, 1
        cmc
        ret
loc_11_02C9:
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        and    dword ptr [esi + ebx + 14h], 0
        ret
loc_11_02D5:
        db  066h, 00Fh, 0BAh, 035h, 06Dh, 00Bh, 000h, 000h, 002h
        ret
        db  000h
sub_11_02E0:
        enter  18h, 0
        or     dword ptr [ebp - 8], 0ffffffffh
        test   eax, 10000000h
        je     SHORT loc_11_02F6
        call   sub_11_0F3E
        jmp    SHORT loc_11_0307
loc_11_02F6:
        db  0BEh, 009h, 003h, 000h, 000h
        call   sub_11_0D00
        cmp    eax, -2
        jb     SHORT loc_11_0307
        mov    al, 0ffh
loc_11_0307:
        leave
        ret
loc_11_0309:
        db  0BEh, 02Bh, 003h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 1fffffffh
        mov    ecx, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp + 8]
        db  0CDh, 020h, 004h, 000h, 017h, 000h
        jb     SHORT loc_11_0328
        ret
loc_11_0328:
        or     eax, 0ffffffffh
loc_11_032B:
        mov    ebp, edx
        jmp    NEAR PTR loc_11_0DBB
sub_11_0332:
        or     dword ptr [ebp + 0ch], 0ffffffffh
        test   eax, 40000000h
        jne    NEAR PTR loc_11_03F1
        call   sub_11_041A
        jb     NEAR PTR loc_11_03F1
        db  0E8h, 0BEh, 0FCh, 0F4h, 0FFh
        cmp    word ptr [eax + 2ah], 0
        je     SHORT loc_11_03B9
        db  08Bh, 03Dh, 06Ch, 00Ah, 000h, 000h
        bts    dword ptr [edi + ebx + 10h], 4
        jb     SHORT loc_11_03DF
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [edx + 1ch], eax
        push   ebx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        pop    ebx
        jb     SHORT loc_11_039A
        test   dword ptr [ebp + 1ch], 80001000h
        jne    SHORT loc_11_039A
        db  0BEh, 095h, 004h, 000h, 000h
        mov    ecx, 419h
        xor    eax, eax
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jae    SHORT loc_11_03F1
        call   esi
        jmp    SHORT loc_11_03F1
loc_11_039A:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        or     byte ptr [edx + 18h], 1
        push   7
        pop    ecx
        mov    eax, 1000h
        db  0BEh, 065h, 004h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        jmp    SHORT loc_11_03F1
loc_11_03B9:
        push   edx
        db  0CDh, 020h, 00Dh, 000h, 017h, 000h
        pop    edx
        test   al, 1
        je     SHORT loc_11_03DF
        push   1388h
        push   2
        push   edx
        db  068h, 0F5h, 003h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_11_03F1
loc_11_03DF:
        or     eax, 0ffffffffh
        xchg   dword ptr [ebp + 0ch], eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_11_03F1:
        mov    eax, dword ptr [ebp + 0ch]
        ret
loc_11_03F5:
        pushad
        mov    edi, dword ptr [esp + 24h]
        test   byte ptr [esp + 28h], 2
        jne    SHORT loc_11_0413
        db  08Bh, 01Dh, 008h, 00Ah, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_11_0413
        xor    byte ptr [edi + 0bh], 50h
loc_11_0413:
        call   sub_11_04AF
        popad
        ret
sub_11_041A:
        db  08Bh, 035h, 078h, 00Ah, 000h, 000h
        cmp    esi, 1
        jb     SHORT loc_11_0464
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_11_0464
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        xchg   edx, eax
        xor    eax, eax
        mov    dword ptr [edx + 18h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [edx + 14h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edx + 10h], eax
        mov    eax, dword ptr [ebp + 1ch]
        or     eax, 40000000h
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [ebp + 0ch], edx
loc_11_0464:
        ret
loc_11_0465:
        mov    ebx, dword ptr [edx + 14h]
        db  0BEh, 095h, 004h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_11_0492
        xor    eax, eax
        test   dword ptr [edx + 8], 80001000h
        je     SHORT loc_11_0485
        or     eax, 100001h
loc_11_0485:
        mov    ecx, 419h
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jae    SHORT loc_11_0494
loc_11_0492:
        call   esi
loc_11_0494:
        ret
loc_11_0495:
        pushfd
        mov    edi, edx
        mov    ebx, dword ptr [edi + 14h]
        db  003h, 01Dh, 06Ch, 00Ah, 000h, 000h
        and    byte ptr [ebx + 10h], 0efh
        popfd
        jae    SHORT loc_11_04AA
        jmp    SHORT sub_11_04AF
loc_11_04AA:
        mov    eax, dword ptr [ebp + 18h]
        jmp    SHORT loc_11_04CF
sub_11_04AF:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        call   sub_11_04F8
        push   edi
        push   ebx
        mov    ebx, dword ptr [edi + 14h]
        mov    eax, dword ptr [edi + 8]
        mov    ecx, dword ptr [edi]
        mov    edi, dword ptr [edi + 4]
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        pop    ebx
        pop    edi
loc_11_04CF:
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_11_04DF
        push   edi
        push   ebx
        mov    edx, dword ptr [edi + 10h]
        call   dword ptr [edi + 0ch]
        pop    ebx
        pop    edi
loc_11_04DF:
        call   sub_11_04F8
        mov    eax, edi
        db  08Bh, 035h, 078h, 00Ah, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        db  0CDh, 020h, 0A2h, 080h, 001h, 000h
sub_11_04F8:
        btr    dword ptr [edi + 18h], 0
        jae    SHORT loc_11_0525
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, dword ptr [edi + 1ch]
        db  0CDh, 020h, 037h, 000h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, 0fffff000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_11_0525:
        ret
        db  000h, 000h
sub_11_0528:
        enter  18h, 0
        db  0BEh, 05Bh, 005h, 000h, 000h
        call   sub_11_0D00
        jae    SHORT loc_11_0544
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  089h, 01Dh, 078h, 00Bh, 000h, 000h
loc_11_0544:
        leave
        ret
loc_11_0546:
        mov    ebx, ebp
        mov    esi, dword ptr [ebp]
        mov    ebp, dword ptr [esi + 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 14h], eax
        ret
loc_11_055B:
        db  0BEh, 073h, 005h, 000h, 000h
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, 41dh
        xor    eax, eax
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jb     SHORT loc_11_0573
        ret
loc_11_0573:
        jb     NEAR PTR loc_11_0DD6
        mov    eax, dword ptr [ebp + 18h]
        mov    ebp, edx
        jmp    NEAR PTR loc_11_0DBB
sub_11_0583:
        db  08Bh, 01Dh, 078h, 00Bh, 000h, 000h
        db  0E8h, 081h, 0FAh, 0F4h, 0FFh
        test   byte ptr [eax], 4
        je     SHORT loc_11_05A4
        mov    ecx, 420h
        xor    eax, eax
        xor    esi, esi
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 017h, 000h
loc_11_05A4:
        ret
        db  000h, 000h, 000h
loc_11_05A8:
        enter  70h, 0
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_11_0615
        db  0E8h, 04Bh, 006h, 0F5h, 0FFh
        jb     SHORT loc_11_0615
        db  0E8h, 038h, 000h, 0F5h, 0FFh
        or     byte ptr [eax + 3], 1
        mov    dword ptr [ebp - 4], ebx
        lea    edi, [ebp - 70h]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        db  0BEh, 02Ch, 006h, 000h, 000h
        mov    ecx, 417h
        movzx  eax, byte ptr [ebp - 54h]
        or     eax, 100000h
        mov    edx, ebp
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jb     SHORT loc_11_05F4
        db  0E8h, 031h, 006h, 0F5h, 0FFh
        jmp    SHORT loc_11_0604
loc_11_05F4:
        cmp    byte ptr [ebp - 54h], 0
        je     SHORT loc_11_0604
        and    word ptr [ebp - 54h], 0
        and    word ptr [ebp - 5ch], 0
loc_11_0604:
        db  0E8h, 028h, 006h, 0F5h, 0FFh
        lea    esi, [ebp - 70h]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
loc_11_0612:
        leave
        clc
        ret
loc_11_0615:
        mov    edi, dword ptr [ebp]
        cmp    byte ptr [edi + 1ch], 0
        je     SHORT loc_11_0612
        mov    word ptr [edi + 1ch], 0
        mov    word ptr [edi + 14h], 0
        jmp    SHORT loc_11_0612
loc_11_062C:
        mov    ecx, dword ptr [ebp + 18h]
        mov    ebp, edx
        jae    SHORT loc_11_063B
        cmp    byte ptr [ebp - 54h], 0
        je     SHORT loc_11_0646
        xor    ecx, ecx
loc_11_063B:
        mov    word ptr [ebp - 54h], cx
        shr    ecx, 10h
        mov    word ptr [ebp - 5ch], cx
loc_11_0646:
        db  0E9h, 0F4h, 005h, 0F5h, 0FFh
loc_11_064B:
        mov    esi, dword ptr [ebp]
        xchg   ebp, esi
        mov    ebp, dword ptr [ebp + 4]
        mov    ebx, dword ptr [ebp - 4]
        lea    ebp, [ebp - 70h]
        movzx  eax, byte ptr [esi + 18h]
        cmp    eax, 0bh
        jae    SHORT loc_11_066B
        db  0FFh, 024h, 085h, 07Ch, 00Bh, 000h, 000h
loc_11_066B:
        ret
loc_11_066C:
        call   sub_11_06C1
        call   sub_11_068A
        movzx  eax, word ptr [ebp + 38h]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 10h]
        add    eax, ecx
        add    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], eax
        ret
sub_11_068A:
        movzx  eax, word ptr [ebp + 14h]
        mov    dword ptr [esi + 14h], eax
        ret
loc_11_0692:
        call   sub_11_068A
        movzx  eax, word ptr [ebp + 38h]
        mov    edi, eax
        shl    eax, 4
        shl    edi, 10h
        movzx  ecx, word ptr [ebp + 10h]
        add    eax, ecx
        add    edi, ecx
        add    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], edi
        db  066h, 083h, 03Dh, 0E0h, 000h, 000h, 000h, 000h
        jne    SHORT loc_11_06BE
        or     eax, 0ffffffffh
loc_11_06BE:
        mov    dword ptr [esi], eax
        ret
sub_11_06C1:
        movzx  eax, word ptr [ebp + 4]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 18h]
        mov    dword ptr [esi + 18h], eax
        ret
loc_11_06D0:
        push   ebx
        db  0CDh, 020h, 030h, 000h, 001h, 000h
        mov    edi, ebx
        pop    ebx
        xor    edx, edx
        lea    esi, [edx + 2]
        lea    eax, [edx + 0fh]
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_11_06ED:
        ret
loc_11_06EE:
        mov    esi, dword ptr [ebp]
        mov    esi, dword ptr [esi + 4]
        mov    ax, 3c10h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        push   10h
        pop    ecx
        cld
        rep movsb
        ret
loc_11_0706:
        or     byte ptr [ebp + 2ch], 1
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_11_0721
        db  0E8h, 0F5h, 0F8h, 0F4h, 0FFh
        mov    dword ptr [ebp + 4], eax
        and    byte ptr [ebp + 2ch], 0feh
loc_11_0721:
        ret
loc_11_0722:
        mov    esi, dword ptr [ebp]
        mov    esi, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp + 18h], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp + 1ch], eax
        btr    dword ptr [esi + 18h], 0
        jae    SHORT loc_11_074C
        push   0fffff000h
        pop    eax
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_11_074C:
        ret
loc_11_074D:
        and    byte ptr [ebp + 2ch], 0feh
        mov    edx, ebx
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_11_07D4
        db  0E8h, 0ACh, 0F8h, 0F4h, 0FFh
        bts    dword ptr [eax], 1fh
        jb     SHORT loc_11_07D4
        xchg   edx, ebx
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        inc    eax
        je     SHORT loc_11_07C8
        dec    eax
        push   5
        pop    ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    ebx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        db  0BEh, 0C5h, 008h, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 00Ah, 000h, 00Dh, 000h
        jb     SHORT loc_11_07C2
        db  0BEh, 08Ch, 00Ah, 000h, 000h
        lodsd
        mov    ecx, eax
        jecxz  loc_11_07BB
loc_11_07A1:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_11_07B6
        cmp    dword ptr [esi], 80000000h
        jne    SHORT loc_11_07B6
        db  0CDh, 020h, 003h, 000h, 00Dh, 000h
loc_11_07B6:
        add    esi, 14h
        loop   loc_11_07A1
loc_11_07BB:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_11_07C1:
        ret
loc_11_07C2:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_11_07C8:
        mov    ebx, dword ptr [ebp + 10h]
        db  0E8h, 03Fh, 0F8h, 0F4h, 0FFh
        btr    dword ptr [eax], 1fh
loc_11_07D4:
        or     byte ptr [ebp + 2ch], 1
        jmp    SHORT loc_11_07C1
loc_11_07DA:
        db  0A1h, 0B0h, 00Bh, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        ret
loc_11_07E3:
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 01Ah, 000h, 017h, 000h
        jb     SHORT loc_11_0808
        db  00Fh, 0B7h, 00Dh, 022h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        xor    edi, edi
        mov    eax, 40001000h
        db  0CDh, 020h, 003h, 080h, 017h, 000h
loc_11_0808:
        ret
loc_11_0809:
        xor    esi, esi
        mov    eax, 100001h
        mov    ecx, 415h
        db  0CDh, 020h, 002h, 080h, 017h, 000h
        db  000h
loc_11_081C:
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_11_0868
        db  08Bh, 03Dh, 06Ch, 00Ah, 000h, 000h
        test   byte ptr [edi + ebx + 10h], 80h
        je     SHORT loc_11_0868
        or     byte ptr [edi + ebx + 10h], 2
        mov    edx, ebx
loc_11_083B:
        mov    ebx, edx
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_11_0868
        db  08Bh, 03Dh, 06Ch, 00Ah, 000h, 000h
        test   byte ptr [edi + ebx + 10h], 2
        je     SHORT loc_11_0868
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        mov    eax, 3e8h
        db  0BEh, 03Bh, 008h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
loc_11_0868:
        ret
loc_11_0869:
        mov    ebx, dword ptr [ebp + 10h]
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_11_0868
sub_11_0874:
        db  08Bh, 03Dh, 06Ch, 00Ah, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_11_089E
        and    byte ptr [edi + ebx + 10h], 0feh
        btr    dword ptr [edi + ebx + 10h], 1
        jae    SHORT loc_11_089E
        mov    ecx, 424h
        xor    eax, eax
        xor    esi, esi
        db  0CDh, 020h, 002h, 000h, 017h, 000h
loc_11_089E:
        stc
        ret
loc_11_08A0:
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_11_090D
        db  0E8h, 05Ch, 0F7h, 0F4h, 0FFh
        test   byte ptr [eax + 3], 80h
        je     SHORT loc_11_08C0
        db  0CDh, 020h, 00Bh, 000h, 00Dh, 000h
        ret
loc_11_08C0:
        mov    eax, 1
loc_11_08C5:
        push   eax
        db  0E8h, 044h, 0F7h, 0F4h, 0FFh
        btr    dword ptr [eax], 1fh
        db  0BEh, 08Ch, 00Ah, 000h, 000h
        lodsd
        mov    ecx, eax
        jecxz  loc_11_08F3
loc_11_08D9:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_11_08EE
        cmp    dword ptr [esi], 80000000h
        jne    SHORT loc_11_08EE
        db  0CDh, 020h, 004h, 000h, 00Dh, 000h
loc_11_08EE:
        add    esi, 14h
        loop   loc_11_08D9
loc_11_08F3:
        mov    ecx, 41ch
        pop    eax
        xor    esi, esi
        cmp    eax, 1
        jne    SHORT loc_11_0905
        db  0BEh, 016h, 00Ah, 000h, 000h
loc_11_0905:
        xor    edx, edx
        db  0CDh, 020h, 002h, 000h, 017h, 000h
loc_11_090D:
        ret
loc_11_090E:
        db  0CDh, 020h, 006h, 000h, 00Dh, 000h
        xor    esi, esi
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0E8h, 009h, 0F7h, 0F4h, 0FFh
        jb     SHORT loc_11_093C
        mov    eax, 100000h
        test   ecx, 1
        setne  al
        mov    ecx, 41eh
        db  0CDh, 020h, 002h, 080h, 017h, 000h
loc_11_093C:
        ret
loc_11_093D:
        push   ebx
        push   ecx
        db  0CDh, 020h, 008h, 000h, 00Dh, 000h
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_11_096D
        db  0E8h, 0BDh, 0F6h, 0F4h, 0FFh
        test   byte ptr [eax], 4
        je     SHORT loc_11_0967
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        pop    ecx
        pop    eax
        db  0CDh, 020h, 005h, 000h, 00Dh, 000h
        jmp    SHORT loc_11_0970
loc_11_0967:
        db  0FFh, 015h, 088h, 00Ah, 000h, 000h
loc_11_096D:
        add    esp, 8
loc_11_0970:
        ret
        db  000h, 000h, 000h
_SHELL_SuggestSingleMSDOSMode:
        pushad
        mov    ebx, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 28h]
        test   dl, 1
        jne    SHORT sub_11_09D3
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_11_09C4
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        test   byte ptr [esi + ebx + 11h], 1
        jne    SHORT loc_11_09C4
        test   dl, 20h
        je     SHORT loc_11_09A8
        db  0CDh, 020h, 040h, 000h, 001h, 000h
        mov    dword ptr [esi + ebx + 14h], eax
        jmp    SHORT loc_11_09C4
loc_11_09A8:
        test   dl, 8
        je     SHORT loc_11_09B2
        or     dl, 1
        jmp    SHORT sub_11_09D3
loc_11_09B2:
        xor    eax, eax
        mov    ecx, 6bh
        db  0BEh, 0D3h, 009h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_11_09C4:
        popad
        ret
loc_11_09C6:
        inc    ebx
        dec    edi
        dec    esi
        inc    ecx
        inc    edi
        inc    ebp
        dec    esi
        push   esp
        inc    ebp
        pop    eax
        inc    ebp
        db  000h
sub_11_09D3:
        xor    eax, eax
        test   dl, 1
        jne    SHORT loc_11_0A2A
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        mov    esi, dword ptr [esi + ebx + 0ch]
        test   byte ptr [esi + 1c4h], 4
        jne    NEAR PTR loc_11_0B08
        lea    esi, [esi]
        db  0E8h, 021h, 005h, 0F5h, 0FFh
        push   edx
        push   esi
        db  068h, 0C6h, 009h, 000h, 000h
        db  0CDh, 020h, 08Eh, 001h, 001h, 000h
        add    esp, 8
        pop    edx
        test   eax, eax
        je     NEAR PTR loc_11_0B08
        db  0CDh, 020h, 01Ah, 000h, 017h, 000h
        jb     NEAR PTR loc_11_0B08
        xor    ecx, ecx
        db  0CDh, 020h, 025h, 000h, 001h, 000h
        jae    SHORT loc_11_0A2A
        or     dl, 1
loc_11_0A2A:
        mov    edi, eax
        db  08Bh, 035h, 084h, 00Ah, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_11_0B09
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 0ch], edi
        push   eax
        mov    dword ptr [eax + 8], esp
        pushad
        xchg   edi, eax
        db  08Bh, 035h, 06Ch, 00Ah, 000h, 000h
        mov    esi, dword ptr [esi + ebx + 0ch]
        lea    edx, [edi + 114h]
        db  0E8h, 0AEh, 0F5h, 0F4h, 0FFh
        lea    ecx, [eax + 2eh]
        push   20h
        push   ecx
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        lea    edx, [edi + 134h]
        lea    ecx, [esi]
        push   40h
        push   ecx
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        lea    edx, [edi + 1b4h]
        lea    ecx, [esi + 80h]
        push   40h
        push   ecx
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        lea    edx, [edi + 10h]
        lea    ecx, [esi + 0c0h]
        push   104h
        push   ecx
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        lea    edi, [edi + 174h]
        lea    esi, [esi + 40h]
        push   40h
        pop    ecx
loc_11_0AC4:
        lodsb
        stosb
        cmp    al, 0dh
        loopne loc_11_0AC4
        mov    byte ptr [edi - 1], 0
        popad
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    edi, eax
        push   0
        push   0
        db  068h, 074h, 00Bh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        test   byte ptr [edi], 1
        jne    SHORT loc_11_0B1A
        mov    eax, dword ptr [edi + 0ch]
        mov    ecx, 1
        db  0CDh, 020h, 027h, 000h, 001h, 000h
        db  0CDh, 020h, 026h, 000h, 001h, 000h
        pop    eax
        cmp    eax, 1
        je     SHORT loc_11_0B20
loc_11_0B08:
        ret
loc_11_0B09:
        test   edi, edi
        je     SHORT loc_11_0B15
        mov    eax, edi
        db  0CDh, 020h, 026h, 000h, 001h, 000h
loc_11_0B15:
        test   dl, 1
        je     SHORT loc_11_0B08
loc_11_0B1A:
        db  0CDh, 020h, 02Fh, 000h, 001h, 000h
loc_11_0B20:
        xor    ecx, ecx
        mov    eax, 1388h
        db  0CDh, 020h, 0ECh, 000h, 001h, 000h
        ret
loc_11_0B2E:
        inc    esi
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [ecx], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 55h], dl
        dec    esi
        inc    esp
        dec    esp
        dec    esp
        xor    esi, dword ptr [edx]
        add    byte ptr [ecx + 50h], al
        push   eax
        push   edi
        dec    ecx
        pop    edx
        inc    ebx
        push   eax
        dec    esp
        sub    al, 53h
        imul   ebp, dword ptr [esi + 67h], 534d656ch
        inc    esp
        dec    edi
        push   ebx
        push   edi
        db  069h, 07Ah, 061h, 072h, 064h, 000h
loc_11_0B74:
        db  068h, 02Eh, 00Bh, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 017h, 000h
        add    esp, 4
        ret
        db  0A6h, 00Bh, 000h, 000h, 0F6h, 00Bh, 000h, 000h, 0ADh, 00Bh, 000h, 000h, 0DEh, 00Bh, 000h, 000h
loc_11_0B93:
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 4
        jae    NEAR PTR loc_11_0C30
        db  0FFh, 024h, 085h, 083h, 00Bh, 000h, 000h
loc_11_0BA6:
        xor    eax, eax
        mov    ecx, eax
        mov    ch, 4
        ret
loc_11_0BAD:
        mov    ecx, dword ptr [esi + 1ch]
        cmp    ecx, 1f4h
        jne    SHORT loc_11_0C26
        mov    edi, dword ptr [esi + 20h]
        mov    dword ptr [edi], ecx
        mov    edi, dword ptr [esi + 18h]
        db  08Bh, 035h, 084h, 00Ah, 000h, 000h
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        je     SHORT loc_11_0C2B
        push   esi
        mov    esi, eax
        cld
        rep movsb
        pop    esi
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        xor    eax, eax
        ret
loc_11_0BDE:
        cmp    dword ptr [esi + 14h], 1f4h
        jne    SHORT loc_11_0C26
        mov    eax, dword ptr [esi + 10h]
        mov    eax, dword ptr [eax + 0ch]
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        xor    eax, eax
        ret
loc_11_0BF6:
        cmp    dword ptr [esi + 14h], 0ch
        jne    SHORT loc_11_0C26
        cmp    dword ptr [esi + 1ch], 4
        jne    SHORT loc_11_0C26
        mov    eax, dword ptr [esi + 10h]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    ecx, dword ptr [eax]
        mov    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        or     eax, 0c0001000h
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        ret
loc_11_0C26:
        xor    eax, eax
        mov    al, 57h
        ret
loc_11_0C2B:
        xor    eax, eax
        mov    al, 0dh
        ret
loc_11_0C30:
        xor    eax, eax
        inc    eax
        ret
loc_11_0C34:
        push   ebx
        outsd
        db  066h, 074h, 077h
        popad
        jb     SHORT loc_11_0CA1
        pop    esp
        dec    ebp
        imul   esp, dword ptr [ebx + 72h], 666f736fh
        je     SHORT loc_11_0CA3
        push   edi
        imul   ebp, dword ptr [esi + 64h], 5c73776fh
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        db  074h, 056h
        jb     NEAR PTR loc_11_0CCD
        imul   ebp, dword ptr [edi + 6eh], 2d534d5ch
        inc    esp
        dec    edi
        push   ebx
        and    byte ptr [ebp + 6dh], al
        jne    SHORT sub_11_0CD5
        popad
        je     SHORT sub_11_0CD5
        outsd
        outsb
        pop    esp
        inc    ecx
        db  070h, 070h
        inc    ebx
        outsd
        insd
        db  070h, 061h
        je     SHORT sub_11_0C79
sub_11_0C79:
        sub    esp, 10h
        pushad
        db  08Dh, 005h, 0A9h, 00Ch, 000h, 000h
        push   eax
        db  068h, 034h, 00Ch, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_11_0CD0
        xor    ebx, ebx
        lea    edi, [esp + 20h]
loc_11_0CA1:
        push   0dh
loc_11_0CA3:
        push   edi
        push   ebx
        db  0FFh, 035h, 0A9h, 00Ch, 000h, 000h
        db  0CDh, 020h, 04Ch, 001h, 001h, 000h
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_11_0CD0
        mov    esi, edi
        db  0E8h, 06Bh, 002h, 0F5h, 0FFh
        db  00Fh, 0ABh, 00Dh, 0ADh, 00Ch, 000h, 000h
        db  00Fh, 0ABh, 015h, 0ADh, 00Ch, 000h, 000h
loc_11_0CCD:
        inc    ebx
        jmp    SHORT loc_11_0CA1
loc_11_0CD0:
        popad
        add    esp, 10h
        ret
sub_11_0CD5:
        pushad
        db  00Fh, 0B7h, 015h, 00Ch, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        mov    ah, 9
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    edx, 10h
        call   sub_11_09D3
        test   eax, eax
        je     SHORT loc_11_0CFC
        stc
loc_11_0CFC:
        popad
        ret
        db  000h, 000h
sub_11_0D00:
        mov    dword ptr [ebp - 14h], esi
        db  08Bh, 03Dh, 06Ch, 00Ah, 000h, 000h
        xor    eax, eax
        mov    dword ptr [ebp - 18h], eax
        or     dword ptr [ebp - 8], 0ffffffffh
        mov    dword ptr [ebp - 4], ebx
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    NEAR PTR loc_11_0DA2
        db  083h, 03Dh, 00Ch, 00Ch, 000h, 000h, 000h
        je     SHORT loc_11_0DA2
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_11_0DA2
        mov    ecx, 1
        db  0E8h, 0F0h, 0F9h, 0F4h, 0FFh
        jb     SHORT loc_11_0DA2
        inc    byte ptr [ebp - 18h]
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        mov    dword ptr [ebp - 0ch], eax
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, 1000h
        mov    ecx, 0fh
        mov    edx, ebp
        db  0BEh, 0AAh, 00Dh, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    dword ptr [ebp - 10h], esi
        mov    ebx, dword ptr [ebp - 4]
        db  0CDh, 020h, 021h, 000h, 001h, 000h
        jb     SHORT loc_11_0D80
        inc    byte ptr [ebp - 17h]
        dec    byte ptr [ebp - 18h]
        dec    byte ptr [ebp - 16h]
        je     SHORT loc_11_0DA2
loc_11_0D80:
        mov    esi, dword ptr [ebp - 10h]
        db  0CDh, 020h, 015h, 000h, 001h, 000h
        dec    byte ptr [ebp - 17h]
        js     SHORT loc_11_0D97
        mov    ebx, dword ptr [ebp - 4]
        db  0CDh, 020h, 02Ch, 000h, 001h, 000h
loc_11_0D97:
        dec    byte ptr [ebp - 18h]
        js     SHORT loc_11_0DA2
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_11_0DA2:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, -1
        cmc
        ret
loc_11_0DAA:
        mov    ebp, edx
        and    dword ptr [ebp - 10h], 0
        mov    ebx, dword ptr [ebp - 4]
        test   byte ptr [ebx], 8
        je     SHORT loc_11_0DD6
        jmp    dword ptr [ebp - 14h]
loc_11_0DBB:
        mov    dword ptr [ebp - 8], eax
        inc    byte ptr [ebp - 16h]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        sub    eax, dword ptr [ebp - 0ch]
        db  0CDh, 020h, 037h, 000h, 001h, 000h
loc_11_0DD6:
        dec    byte ptr [ebp - 17h]
        mov    ebx, dword ptr [ebp - 4]
        db  0CDh, 020h, 02Ch, 000h, 001h, 000h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, 0fffff000h
        db  0CDh, 020h, 01Eh, 080h, 001h, 000h
        db  000h, 08Bh, 035h, 0E0h, 00Bh, 000h
        add    byte ptr [edx - 5edf3206h], dl
        add    byte ptr [ecx], al
        add    bl, bh
        jb     SHORT loc_11_0E16
        push   2
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        call   ecx
        pop    eax
        pop    eax
loc_11_0E16:
        ret
_SHELL_PostShellMessage:
        db  08Bh, 00Dh, 0B0h, 00Bh, 000h, 000h
        jecxz  loc_11_0E3A
        push   0
        push   0
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   506h
        push   ecx
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
loc_11_0E3A:
        ret
loc_11_0E3B:
        db  0CDh, 020h, 06Dh, 000h, 001h, 000h
        mov    word ptr [ebp + 38h], ax
        db  089h, 03Dh, 0D8h, 00Bh, 000h, 000h
        mov    ax, 2810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        db  0BFh, 0ECh, 00Bh, 000h, 000h
        mov    ecx, 0dh
        cld
        rep movsd
        movzx  eax, word ptr [ebp + 1ch]
        db  0A3h, 0B0h, 00Bh, 000h, 000h
        mov    ax, word ptr [ebp + 3ch]
        inc    ecx
        db  066h, 0A3h, 0DCh, 00Bh, 000h, 000h
        db  08Bh, 035h, 0E0h, 00Bh, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_11_0EA6
        push   ecx
        xor    eax, eax
        db  080h, 00Dh, 0E8h, 00Bh, 000h, 000h, 001h
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    ecx, 0ah
        db  0BEh, 03Ah, 000h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    ecx
loc_11_0EA6:
        push   1
        mov    eax, esp
        push   0
        push   ecx
        push   219h
        push   eax
        push   0
        db  0CDh, 020h, 010h, 000h, 017h, 000h
        add    esp, 14h
        pop    eax
        ret
loc_11_0EC0:
        xor    ecx, ecx
        jmp    SHORT loc_11_0EA6
_SHELL_HookSystemBroadcast:
        push   esi
        db  08Bh, 035h, 0E4h, 00Bh, 000h, 000h
        or     esi, esi
        jne    SHORT loc_11_0ED4
        db  0E8h, 0ECh, 0F7h, 0F4h, 0FFh
loc_11_0ED4:
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_11_0F1D
        push   1
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 10h]
        pop    dword ptr [eax + 0ch]
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 4], edx
        mov    edx, dword ptr [esp + 10h]
        cmp    edx, 1
        jae    SHORT loc_11_0EF9
        rcr    edx, 1
loc_11_0EF9:
        mov    dword ptr [eax + 8], edx
        xor    ecx, ecx
        push   eax
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        jmp    SHORT loc_11_0F0F
loc_11_0F07:
        mov    ecx, eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
loc_11_0F0F:
        je     SHORT loc_11_0F16
        cmp    dword ptr [eax + 8], edx
        jb     SHORT loc_11_0F07
loc_11_0F16:
        pop    eax
        db  0CDh, 020h, 0A0h, 000h, 001h, 000h
loc_11_0F1D:
        pop    esi
        ret
_SHELL_UnhookSystemBroadcast:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        db  08Bh, 035h, 0E4h, 00Bh, 000h, 000h
        jecxz  loc_11_0F38
        db  0C7h, 001h, 03Ah, 00Fh, 000h, 000h
        xchg   ecx, eax
        db  0E8h, 0F0h, 0F0h, 0F4h, 0FFh
loc_11_0F38:
        pop    esi
        ret
loc_11_0F3A:
        xor    eax, eax
        inc    eax
        ret
sub_11_0F3E:
        sub    esp, 10h
        mov    word ptr [esp], ax
        or     dword ptr [esp + 0ch], 0ffffffffh
        and    word ptr [esp + 0ah], 0
        or     edi, edi
        jne    SHORT loc_11_0F5C
        db  0E8h, 0B6h, 0F0h, 0F4h, 0FFh
        lea    edi, [eax + 2eh]
loc_11_0F5C:
        push   ecx
        push   0
        push   30000h
        db  0CDh, 020h, 013h, 000h, 017h, 000h
        add    esp, 0ch
        mov    dword ptr [esp + 6], eax
        or     eax, eax
        je     SHORT loc_11_0FC6
        push   edi
        push   0
        push   30000h
        db  0CDh, 020h, 013h, 000h, 017h, 000h
        add    esp, 0ch
        mov    dword ptr [esp + 2], eax
        or     eax, eax
        je     SHORT loc_11_0FB9
        lea    eax, [esp]
        push   eax
        push   0ch
        db  0FFh, 035h, 008h, 00Ch, 000h, 000h
        push   0
        db  0CDh, 020h, 018h, 000h, 017h, 000h
        add    esp, 10h
        movzx  eax, ax
        mov    dword ptr [esp + 0ch], eax
        push   dword ptr [esp + 2]
        db  0CDh, 020h, 014h, 000h, 017h, 000h
        add    esp, 4
loc_11_0FB9:
        push   dword ptr [esp + 6]
        db  0CDh, 020h, 014h, 000h, 017h, 000h
        add    esp, 4
loc_11_0FC6:
        mov    eax, dword ptr [esp + 0ch]
        add    esp, 10h
        ret
        db  000h, 000h
loc_11_0FD0:
        push   ebx
        outsd
        db  066h, 074h, 077h
        popad
        jb     SHORT loc_11_103D
        pop    esp
        dec    ebp
        imul   esp, dword ptr [ebx + 72h], 666f736fh
        je     SHORT loc_11_103F
        push   edi
        imul   ebp, dword ptr [esi + 64h], 5c73776fh
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        je     SHORT loc_11_1049
        db  065h, 072h, 073h
        imul   ebp, dword ptr [edi + 6eh], 74654e5ch
        db  077h, 06Fh
        db  072h, 06Bh
        pop    esp
        push   edx
        popad
        insb
        and    byte ptr [ebp + 6fh], cl
        and    byte ptr gs:[esi + 65h], cl
        je     SHORT loc_11_1010
loc_11_1010:
        push   ebx
        db  065h, 074h, 075h
        jo     SHORT loc_11_1064
        db  000h
loc_11_1017:
        push   ebx
        db  065h, 074h, 075h
        jo     SHORT loc_11_106B
        push   eax
        popad
        db  074h, 068h
        add    byte ptr [ebx], dh
        db  0C0h, 0A3h, 020h, 00Ch, 000h, 000h, 066h
        dec    eax
        xor    ecx, ecx
        push   ecx
        push   ecx
        push   ecx
        push   ecx
        push   1eh
        push   eax
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
loc_11_103D:
        ret
        db  066h
loc_11_103F:
        cmp    dword ptr [ebp + 18h], 24732400h
        xor    esi, esi
        db  087h
loc_11_1049:
        db  035h, 020h, 00Ch, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    eax, 7d0h
        db  0BEh, 022h, 010h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
loc_11_1064:
        db  089h, 035h, 020h, 00Ch, 000h, 000h
        stc
loc_11_106B:
        ret
_PTEXT ends
        end
