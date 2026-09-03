; pppmac_lcode_01_01.asm -- PPPMAC object 1 segment _LTEXT  (part 1/6)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [edx + 20h]
        mov    edi, dword ptr [esi + 0b3h]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0b3h
        jne    SHORT loc_1_0029
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_0029:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], edi
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edi]
        mov    ebx, dword ptr [edi + 4]
        mov    dword ptr [eax + 4], ebx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 0ch], esi
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 10h], edx
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [edx + 14h], eax
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 2bh], 0
        mov    eax, dword ptr [ecx]
        pop    edi
        mov    word ptr [eax + 28h], 0c0deh
        mov    eax, dword ptr [ecx]
        pop    esi
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ecx]
        pop    ebx
        mov    dword ptr [eax + 24h], 0
        sub    eax, eax
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_1_0080:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0ch]
        mov    word ptr [edx + 28h], 0feedh
        lea    eax, [ecx + 0b3h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 0b7h]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 0b7h]
        pop    ebp
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 0b7h], edx
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_00C0:
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_00D8
        push   0
        push   0
        db  0E8h, 02Fh, 0FFh, 0FDh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_00D8
        clc
        ret
loc_1_00D8:
        stc
        ret
PPPMAC_SYS_DYNAMIC_DEVICE_EXIT:
        db  0E8h, 0E1h, 000h, 0FEh, 0FFh
        clc
        ret
PPPMAC_SYS_VM_TERMINATE:
        db  0E8h, 0EAh, 000h, 0FEh, 0FFh
        clc
        ret
PPPMAC_Control:
        sub    eax, 1
        cmp    eax, 23h
        jae    SHORT loc_1_00F7
        db  0FFh, 024h, 085h, 0F9h, 000h, 000h, 000h
loc_1_00F7:
        clc
        ret
        db  0C0h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0E1h, 000h, 000h, 000h
        db  0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h
        db  0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h
        db  0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h
        db  0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h
        db  0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h
        db  0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0C0h, 000h, 000h, 000h, 0DAh, 000h, 000h, 000h
        db  0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h
        db  0F7h, 000h, 000h, 000h, 0F7h, 000h, 000h, 000h, 044h, 019h, 000h, 000h
sub_1_0185:
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        ret
sub_1_018C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 050h, 001h, 001h, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_01F7:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        ret
sub_1_01FE:
        mov    eax, 499h
        mov    edi, 0
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        cmp    ecx, 0
        je     SHORT loc_1_0219
        and    word ptr [ecx + 0ah], 0dfffh
loc_1_0219:
        ret
sub_1_021A:
        pushad
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        popad
        ret
sub_1_024D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0ffffh
        mov    ebx, dword ptr [ebp + 0ch]
        and    ebx, 0ffffh
        mov    ecx, dword ptr [ebp + 10h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 14h]
        push   0
        push   0
        push   edx
        push   ecx
        push   ebx
        push   eax
        db  0CDh, 020h, 006h, 000h, 017h, 000h
        add    esp, 18h
        cld
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_0287:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 000h, 000h, 09Ah, 004h
        jae    SHORT loc_1_029F
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_029F:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 002h, 000h, 09Ah, 004h
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_1_02B9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        db  0CDh, 020h, 001h, 000h, 02Ah, 000h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
PPPMAC_Service_00:
        mov    eax, 1
        clc
        ret
sub_1_02D4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    edx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, 4bh
        sub    eax, eax
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        mov    eax, esi
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        leave
        ret
        db  000h, 000h
sub_1_0300:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0330
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_0330:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_03D0
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_0370
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_0365
        cmp    al, 0ffh
        jne    SHORT loc_1_0365
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_0365
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_03D7
loc_1_0365:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_03D7
        db  08Bh, 0FFh
loc_1_0370:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_03C0
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_0396
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_039D
loc_1_0396:
        mov    dword ptr [ebp - 10h], 1
loc_1_039D:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_03B0
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_03DA
        db  08Dh, 064h, 024h, 000h
loc_1_03B0:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_03DA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_03C0:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_03DA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_03D0:
        mov    dword ptr [ebp - 4], 20h
loc_1_03D7:
        mov    esi, dword ptr [ebp + 8]
loc_1_03DA:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_04D1
loc_1_03E5:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_043D
        test   al, 2
        je     SHORT loc_1_0429
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 0F6h, 01Fh, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_1_0429
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_043D
loc_1_0429:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_0434
        test   bl, 4
        jne    SHORT loc_1_043D
loc_1_0434:
        test   bl, 20h
        je     NEAR PTR loc_1_04C6
loc_1_043D:
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_06B2
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_04C6
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_047F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_047F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_0490
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_0496
        db  08Bh, 0FFh
loc_1_0490:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_0496:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_04A5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_06B8
loc_1_04A5:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_075C
        mov    edi, ebx
        jmp    SHORT loc_1_04C9
loc_1_04C6:
        mov    edi, dword ptr [edi + 10h]
loc_1_04C9:
        or     edi, edi
        jne    NEAR PTR loc_1_03E5
loc_1_04D1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_04E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_04F0
        pop    ebp
        ret    4
sub_1_04F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_056E
loc_1_0500:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_0567
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_06B8
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_0567
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_052C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_052C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_0540
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_0546
        db  005h, 000h, 000h, 000h, 000h
loc_1_0540:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_0546:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_075C
        mov    edi, ebx
        jmp    SHORT loc_1_056A
loc_1_0567:
        mov    edi, dword ptr [edi + 10h]
loc_1_056A:
        or     edi, edi
        jne    SHORT loc_1_0500
loc_1_056E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_0595
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_0595:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_05D1
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_05C2
        cmp    bl, 0ffh
        jne    SHORT loc_1_05C2
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_05C2
        test   al, 8
        je     SHORT loc_1_05BB
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_05BB:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_05C2:
        test   al, 4
        jne    SHORT loc_1_05CA
        test   al, 2
        je     SHORT loc_1_05E9
loc_1_05CA:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_05D1:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_05E9
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_05E9
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_05E9:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
        db  056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh
        db  05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h
        db  024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch
        db  000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_066A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_0670:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_0676:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_067C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_0682:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_0688:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_0694:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_069A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_06A6:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_06AC:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_06B2:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_06B8:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_06BE:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_06C4:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_06DC:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_0732:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_0738:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_0756:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_075C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_0780:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_07A4:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_07C8:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
sub_1_07CE:
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_07D4:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_07DA:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_0828:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_083C:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        rep movsd
        mov    cl, al
        rep movsb
        pop    edi
        pop    esi
        ret    0ch
sub_1_085C:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_087C
        or     ecx, ecx
        je     SHORT loc_1_0892
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_0890
loc_1_087C:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_0890
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_0890:
        rep movsb
loc_1_0892:
        popfd
        pop    edi
        pop    esi
        ret    0ch
        db  0CDh, 002h, 000h, 000h, 020h, 078h, 000h, 000h, 010h, 07Ah, 000h, 000h, 080h, 07Ah, 000h, 000h
        db  090h, 07Ah, 000h, 000h, 0A0h, 07Ah, 000h, 000h, 080h, 07Bh, 000h, 000h, 0F0h, 07Bh, 000h, 000h
        db  050h, 07Ch, 000h, 000h, 060h, 016h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 099h, 004h, 003h, 000h, 000h, 000h, 050h, 050h, 050h, 04Dh
        db  041h, 043h, 020h, 020h, 000h, 040h, 000h, 054h, 0E8h, 000h, 000h, 000h, 0E0h, 018h, 000h, 000h
        db  0E0h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  098h, 008h, 000h, 000h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0920:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        mov    dword ptr [ebp - 4ch], 4ch
        mov    word ptr [ebp - 48h], 2
        mov    word ptr [ebp - 46h], 29c0h
        mov    word ptr [ebp - 44h], 0
        mov    dword ptr [ebp - 42h], 43h
        mov    byte ptr [ebp - 3eh], 0
        mov    byte ptr [ebp - 3dh], 0
        mov    dword ptr [ebp - 3ch], 0
        mov    dword ptr [ebp - 38h], 0
        mov    dword ptr [ebp - 34h], 0
        db  0C7h, 045h, 0D0h, 0E0h, 009h, 000h, 000h
        mov    dword ptr [ebp - 2ch], 0
        mov    dword ptr [ebp - 28h], 0
        db  0C7h, 045h, 0DCh, 040h, 00Ah, 000h, 000h
        mov    dword ptr [ebp - 20h], 0
        db  0C7h, 045h, 0E4h, 060h, 00Ah, 000h, 000h
        xor    eax, eax
        db  0C7h, 045h, 0E8h, 070h, 00Ah, 000h, 000h
        db  0C7h, 045h, 0ECh, 0D0h, 00Fh, 000h, 000h
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        db  066h, 0A1h, 008h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp - 3ch], eax
        lea    eax, [ebp - 4ch]
        push   eax
        call   PPPMAC_Service_01
        add    esp, 4
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_09E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 4]
        mov    esi, dword ptr [ecx + 0ch]
        cmp    dword ptr [esi + 0dch], eax
        jne    SHORT loc_1_0A33
        mov    dword ptr [esi + 0d4h], 0
        mov    dword ptr [esi + 0dch], 0
        mov    dword ptr [esi + 0e0h], 0
        mov    eax, dword ptr [esi + 0d8h]
        or     eax, eax
        je     SHORT loc_1_0A29
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   eax
        call   sub_1_075C
loc_1_0A29:
        mov    dword ptr [esi + 0d8h], 0
loc_1_0A33:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0A40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4bh
        pop    ebp
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 3fh], 0fh
        ret
        db  090h
loc_1_0A60:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    word ptr [eax], 0
        sub    al, al
        ret
        db  090h
loc_1_0A70:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 8]
        add    edi, 4bh
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], ecx
        mov    eax, dword ptr [eax + 2fh]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        cmp    dword ptr [edi], 0
        mov    ax, word ptr [eax + 33h]
        mov    word ptr [ebp - 4], ax
        je     SHORT loc_1_0AE0
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [edi + 9], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0D00
        mov    eax, dword ptr [ebp + 8]
        add    esp, 4
        mov    byte ptr [eax + 11h], 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_0AE0:
        mov    edx, dword ptr [ebp - 18h]
        sub    word ptr [ebp - 4], 4
        lea    edx, [edx + 4]
        mov    esi, 0
        je     SHORT loc_1_0B33
loc_1_0AF2:
        mov    bl, byte ptr [edx]
        mov    al, byte ptr [edx + 1]
        inc    edx
        inc    edx
        mov    cl, al
        and    ecx, 0ffff00ffh
        mov    word ptr [ebp - 8], cx
        mov    eax, ecx
        and    eax, 0ffffh
        mov    byte ptr [ebp - 0ch], 1
        lea    ecx, [ebx - 1]
        shl    byte ptr [ebp - 0ch], cl
        mov    cl, byte ptr [ebp - 0ch]
        or     byte ptr [ebp - 1], cl
        cmp    bl, 4
        jne    SHORT loc_1_0B25
        lea    esi, [eax + esi - 4]
loc_1_0B25:
        mov    cx, word ptr [ebp - 8]
        sub    word ptr [ebp - 4], cx
        lea    edx, [eax + edx - 2]
        jne    SHORT loc_1_0AF2
loc_1_0B33:
        mov    cl, byte ptr [ebp - 1]
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [edi + 8], cl
        cmp    cl, 1
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [edi + 9], al
        jne    SHORT loc_1_0B79
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [edi + 4], 0
        mov    al, byte ptr [eax + 1]
        mov    ecx, 0ch
        mov    byte ptr [edi + 9], al
        mov    dword ptr [edi + 3bh], 0
        add    edi, 0ah
        sub    eax, eax
        rep stosd
        stosb
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0D00
        jmp    NEAR PTR loc_1_0C3A
loc_1_0B79:
        mov    bl, byte ptr [ebp - 1]
        and    bl, 4
        mov    esi, 1ch
        jne    SHORT loc_1_0B8B
        mov    esi, 3
loc_1_0B8B:
        lea    edi, [esi + 18h]
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        push   edi
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_0BB4
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    byte ptr [eax + 11h], 10h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0BB4:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 0ch], edi
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [eax + 10h], 800h
        mov    eax, dword ptr [ebp - 18h]
        or     bl, bl
        mov    cl, byte ptr [eax + 1]
        mov    eax, dword ptr [ebp - 10h]
        push   esi
        mov    byte ptr [eax + 14h], cl
        mov    eax, dword ptr [ebp - 10h]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [eax + 15h], cl
        je     SHORT loc_1_0BF0
        db  068h, 0C0h, 081h, 001h, 000h
        jmp    SHORT loc_1_0BF5
        db  090h
loc_1_0BF0:
        db  068h, 0DCh, 081h, 001h, 000h
loc_1_0BF5:
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 18h
        push   eax
        call   sub_1_083C
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 0d0h], 800h
        mov    dword ptr [ecx + 0d4h], 902h
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 0d8h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0dch], eax
        db  0C7h, 081h, 0E0h, 000h, 000h, 000h, 050h, 00Ch, 000h, 000h
        push   ecx
        call   PPPMAC_Service_09
loc_1_0C3A:
        mov    eax, dword ptr [ebp + 8]
        add    esp, 4
        mov    byte ptr [eax + 11h], 10h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_0C50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    ebx, dword ptr [eax + 0ch]
        lea    edi, [esi + 4bh]
        mov    dword ptr [ebx + 0d0h], 801h
        mov    dword ptr [ebx + 0d4h], 0
        mov    dword ptr [ebx + 0dch], 0
        mov    dword ptr [ebx + 0e0h], 0
        mov    eax, dword ptr [ebx + 0d8h]
        or     eax, eax
        je     SHORT loc_1_0CAA
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   eax
        call   sub_1_075C
        mov    dword ptr [ebx + 0d8h], 0
loc_1_0CAA:
        mov    ecx, dword ptr [ebp + 0ch]
        sub    ebx, ebx
        mov    dword ptr [ecx + 10h], 801h
        mov    dword ptr [edi], 1
        mov    eax, dword ptr [ecx + 18h]
        lea    edx, [ecx + 1dh]
        add    ecx, 1ch
        mov    dword ptr [edi + 4], eax
        cmp    byte ptr [ecx], bl
        je     SHORT loc_1_0CDE
loc_1_0CD0:
        cmp    ebx, 30h
        jae    SHORT loc_1_0CDE
        inc    ebx
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0CD0
loc_1_0CDE:
        push   ebx
        push   ecx
        lea    eax, [edi + 0ah]
        push   eax
        call   sub_1_083C
        mov    dword ptr [edi + 3bh], ebx
        push   esi
        call   sub_1_0D00
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_0D00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        lea    edi, [esi + 4bh]
        mov    ebx, dword ptr [eax + 0ch]
        cmp    dword ptr [edi + 4], 0
        je     SHORT loc_1_0D30
        test   byte ptr [edi + 8], 4
        je     SHORT loc_1_0D25
        mov    al, 3
        jmp    SHORT loc_1_0D3C
loc_1_0D25:
        mov    al, 2
        jmp    SHORT loc_1_0D3C
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D30:
        test   byte ptr [edi + 8], 1
        je     NEAR PTR loc_1_0F80
        mov    al, 1
loc_1_0D3C:
        and    eax, 0ffh
        cmp    eax, 1
        je     SHORT loc_1_0D60
        cmp    eax, 2
        je     NEAR PTR loc_1_0DE8
        cmp    eax, 3
        je     NEAR PTR loc_1_0EC0
        jmp    NEAR PTR loc_1_0F4E
        db  08Dh, 049h, 000h
loc_1_0D60:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_0D86
        xor    eax, eax
        db  066h, 0A1h, 0E8h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0B1h, 00Eh, 0FEh, 0FFh
        add    esp, 8
loc_1_0D86:
        push   6
        push   ebx
        call   PPPMAC_Service_02
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_0FA9
        mov    ecx, dword ptr [eax + 1ch]
        add    ecx, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [esi + 3bh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 35h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 2
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edi + 9]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax], 600h
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 1
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 2
        inc    dword ptr [ebp - 4]
        mov    word ptr [esi + 39h], 6
        jmp    NEAR PTR loc_1_0F4E
loc_1_0DE8:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_0E0E
        xor    eax, eax
        db  066h, 0A1h, 0EAh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 029h, 00Eh, 0FEh, 0FFh
        add    esp, 8
loc_1_0E0E:
        mov    eax, dword ptr [edi + 3bh]
        add    eax, 9
        push   eax
        push   ebx
        call   PPPMAC_Service_02
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_0FA9
        mov    ecx, dword ptr [eax + 1ch]
        add    ecx, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [esi + 3bh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 35h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 2
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edi + 9]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 3bh]
        lea    ecx, [eax + 9]
        mov    cl, ch
        add    ax, 9
        mov    ch, al
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax], cx
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 2
        inc    dword ptr [ebp - 4]
        mov    ecx, dword ptr [edi + 3bh]
        mov    eax, dword ptr [ebp - 4]
        add    cl, 5
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edi + 3fh]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 1
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 3bh]
        push   eax
        lea    eax, [edi + 0ah]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [edi + 3bh]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 3bh]
        add    ax, 9
        mov    word ptr [esi + 39h], ax
        jmp    NEAR PTR loc_1_0F4E
        db  090h
loc_1_0EC0:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_0EE6
        xor    eax, eax
        db  066h, 0A1h, 0ECh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 051h, 00Dh, 0FEh, 0FFh
        add    esp, 8
loc_1_0EE6:
        push   7
        push   ebx
        call   PPPMAC_Service_02
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_0FA9
        mov    ecx, dword ptr [eax + 1ch]
        add    ecx, dword ptr [eax + 20h]
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [esi + 3bh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 35h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 2
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edi + 9]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax], 700h
        add    dword ptr [ebp - 4], 2
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 3
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 3
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edi + 3fh]
        mov    byte ptr [eax], cl
        inc    dword ptr [ebp - 4]
        mov    word ptr [esi + 39h], 7
loc_1_0F4E:
        mov    byte ptr [esi + 11h], 6
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
        mov    dword ptr [esi + 3bh], 0
        mov    dword ptr [esi + 35h], 0
        mov    word ptr [esi + 39h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
loc_1_0F80:
        mov    dword ptr [ebx + 0d0h], 802h
        push   ebx
        call   PPPMAC_Service_09
        add    esp, 4
        mov    byte ptr [esi + 11h], 5
        push   esi
        call   PPPMAC_Service_07
        add    esp, 4
        push   esi
        call   PPPMAC_Service_08
        add    esp, 4
loc_1_0FA9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_0FB0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4fh], 0
        je     SHORT loc_1_0FC2
        mov    byte ptr [eax + 11h], 5
        pop    ebp
        ret
loc_1_0FC2:
        mov    byte ptr [eax + 11h], 8
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0FD0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 2fh]
        cmp    byte ptr [ecx + 10h], 3
        mov    dl, byte ptr [eax]
        mov    eax, dword ptr [ecx + 4]
        mov    eax, dword ptr [eax + 0ch]
        jne    SHORT loc_1_0FF0
        mov    byte ptr [ecx + 11h], 10h
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_0FF0:
        cmp    dword ptr [eax + 0dch], 0
        je     SHORT loc_1_1000
        mov    byte ptr [ecx + 11h], 10h
        pop    ebp
        ret
        db  090h
loc_1_1000:
        xor    eax, eax
        mov    al, dl
        cmp    eax, 1
        je     SHORT loc_1_1019
        cmp    eax, 2
        je     SHORT loc_1_1023
        cmp    eax, 3
        je     SHORT loc_1_1029
        mov    byte ptr [ecx + 11h], 0ch
        pop    ebp
        ret
loc_1_1019:
        push   ecx
        call   sub_1_0A80
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1023:
        mov    byte ptr [ecx + 11h], 0ch
        pop    ebp
        ret
loc_1_1029:
        push   ecx
        call   sub_1_0FB0
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1040:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0feh]
        cmp    byte ptr [eax + 102h], 0
        jne    SHORT loc_1_1060
        mov    eax, 0c0000001h
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1060:
        cmp    dword ptr [ecx + 4], 2
        jne    SHORT loc_1_1070
        mov    eax, 0c0000001h
        pop    ebp
        ret    4
        db  090h
loc_1_1070:
        cmp    dword ptr [ecx + 20h], 0
        je     SHORT loc_1_1099
        cmp    dword ptr [ecx + 28h], 0
        je     SHORT loc_1_1099
        mov    byte ptr [eax + 103h], 1
        push   3e8h
        add    eax, 104h
        push   eax
        call   sub_1_0732
        sub    eax, eax
        pop    ebp
        ret    4
loc_1_1099:
        mov    eax, 0c0000001h
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [esi + 0feh]
        mov    eax, dword ptr [esi]
        cmp    eax, 3
        je     NEAR PTR loc_1_1285
        cmp    eax, 2
        je     NEAR PTR loc_1_1285
        xor    eax, eax
        mov    al, byte ptr [esi + 103h]
        cmp    eax, 1
        je     SHORT loc_1_1100
        cmp    eax, 2
        je     NEAR PTR loc_1_1197
        cmp    eax, 3
        je     NEAR PTR loc_1_1230
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
loc_1_1100:
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   0
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1230
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + 1ch]
        mov    eax, dword ptr [edi + 20h]
        push   eax
        mov    eax, dword ptr [edi + 30h]
        push   eax
        push   ebx
        call   sub_1_083C
        add    ebx, dword ptr [edi + 20h]
        mov    byte ptr [ebx], 0dh
        mov    eax, dword ptr [ebp - 4]
        inc    ebx
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, dword ptr [eax + 1ch]
        mov    dword ptr [eax + 24h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2bh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 55h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 1ch]
        add    esi, 104h
        mov    byte ptr [ecx + eax], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4550
        mov    byte ptr [esi - 1], 2
        push   3e8h
        push   esi
        call   sub_1_0732
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1197:
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   0
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1230
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + 1ch]
        mov    eax, dword ptr [edi + 28h]
        push   eax
        mov    eax, dword ptr [edi + 38h]
        push   eax
        push   ebx
        call   sub_1_083C
        add    ebx, dword ptr [edi + 28h]
        mov    byte ptr [ebx], 0dh
        mov    eax, dword ptr [ebp - 4]
        inc    ebx
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, dword ptr [eax + 1ch]
        mov    dword ptr [eax + 24h], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2bh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 55h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 1ch]
        add    esi, 104h
        mov    byte ptr [ecx + eax], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4550
        mov    byte ptr [esi - 1], 3
        push   1f4h
        push   esi
        call   sub_1_0732
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
loc_1_1230:
        mov    eax, dword ptr [esi + 0ech]
        cmp    eax, 2
        je     SHORT loc_1_1250
        cmp    eax, 3
        je     SHORT loc_1_1260
        cmp    eax, 4
        je     SHORT loc_1_1277
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
loc_1_1250:
        push   esi
        call   sub_1_64B0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  090h
loc_1_1260:
        push   3f00h
        push   esi
        call   PPPMAC_Service_05
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1277:
        push   2100h
        push   esi
        call   PPPMAC_Service_05
        add    esp, 8
loc_1_1285:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_1_1290:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 14h]
        add    eax, dword ptr [esi + 18h]
        add    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 10h]
        lea    ebx, [eax + ecx + 52h]
        and    ebx, 0fffffffch
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_12D5
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_12D5:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 48h], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0feh], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [ecx + 8], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esi + 24h]
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ecx + 24h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ecx + 28h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ecx + 2ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 4ch
        mov    dword ptr [ecx + 30h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 30h]
        add    ecx, dword ptr [eax + 20h]
        inc    ecx
        mov    dword ptr [eax + 34h], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 24h]
        add    ecx, dword ptr [eax + 34h]
        inc    ecx
        mov    dword ptr [eax + 38h], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 38h]
        add    ecx, dword ptr [eax + 28h]
        inc    ecx
        mov    dword ptr [eax + 3ch], ecx
        push   8
        lea    eax, [esi + 30h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 40h
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 4]
        add    esi, 38h
        mov    eax, dword ptr [eax + 20h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    eax, dword ptr [eax + 30h]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 30h]
        mov    byte ptr [ecx + eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    esi, ecx
        mov    eax, dword ptr [eax + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 34h]
        mov    byte ptr [ecx + eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    esi, ecx
        mov    eax, dword ptr [eax + 28h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    eax, dword ptr [eax + 38h]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 28h]
        mov    eax, dword ptr [eax + 38h]
        mov    byte ptr [ecx + eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    esi, ecx
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    eax, dword ptr [eax + 3ch]
        push   eax
        call   sub_1_083C
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_1420:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 0feh]
        or     edx, edx
        je     NEAR PTR loc_1_15C7
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 18h]
        add    eax, dword ptr [esi + 1ch]
        add    eax, dword ptr [esi + 10h]
        mov    ecx, dword ptr [esi + 14h]
        lea    ebx, [eax + ecx + 52h]
        and    ebx, 0fffffffch
        cmp    dword ptr [edx + 48h], ebx
        jae    SHORT loc_1_14D0
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_147B
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_147B:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd
        mov    ecx, ebx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp - 4]
        push   20h
        mov    dword ptr [eax + 48h], ebx
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0feh]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0feh]
        push   0
        mov    ecx, dword ptr [eax + 48h]
        push   ecx
        push   eax
        call   sub_1_075C
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0feh], ecx
        jmp    SHORT loc_1_14D3
        db  08Dh, 064h, 024h, 000h
loc_1_14D0:
        mov    dword ptr [ebp - 4], edx
loc_1_14D3:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ecx + 24h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ecx + 28h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ecx + 2ch], eax
        lea    eax, [esi + 30h]
        push   8
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 40h
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    eax, 4ch
        mov    dword ptr [ecx + 30h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 30h]
        add    ecx, dword ptr [eax + 20h]
        inc    ecx
        mov    dword ptr [eax + 34h], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 24h]
        add    ecx, dword ptr [eax + 34h]
        inc    ecx
        mov    dword ptr [eax + 38h], ecx
        mov    eax, dword ptr [ebp - 4]
        add    esi, 38h
        mov    ecx, dword ptr [eax + 38h]
        add    ecx, dword ptr [eax + 28h]
        inc    ecx
        mov    dword ptr [eax + 3ch], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 20h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    eax, dword ptr [eax + 30h]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 30h]
        mov    byte ptr [ecx + eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    esi, ecx
        mov    eax, dword ptr [eax + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    eax, dword ptr [eax + 34h]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [eax + 34h]
        mov    byte ptr [ecx + eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    esi, ecx
        mov    eax, dword ptr [eax + 28h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    eax, dword ptr [eax + 38h]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 28h]
        mov    eax, dword ptr [eax + 38h]
        mov    byte ptr [ecx + eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    esi, ecx
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    eax, dword ptr [eax + 3ch]
        push   eax
        call   sub_1_083C
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_15C7:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, esi
        mov    eax, dword ptr [esi + 12h]
        sub    ecx, eax
        cmp    ecx, -12h
        je     SHORT loc_1_1608
        mov    cx, word ptr [ebp + 0ch]
loc_1_15F7:
        cmp    word ptr [eax + 47h], cx
        je     SHORT loc_1_1608
        mov    eax, dword ptr [eax]
        mov    edx, esi
        sub    edx, eax
        cmp    edx, -12h
        jne    SHORT loc_1_15F7
loc_1_1608:
        sub    esi, eax
        cmp    esi, -12h
        jne    SHORT loc_1_1611
        sub    eax, eax
loc_1_1611:
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1620:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 20h]
        push   eax
        call   sub_1_15E0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1641
        mov    eax, 804h
        pop    ebp
        ret
loc_1_1641:
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 4eh], ecx
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [eax + 52h], ecx
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 56h], ecx
        mov    eax, 800h
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
PPPMAC_Service_09:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0e4h]
        or     eax, eax
        je     SHORT loc_1_1684
        push   eax
        call   sub_1_02B9
        add    esp, 4
        mov    dword ptr [esi + 0e4h], 0
loc_1_1684:
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1690:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   0
        mov    ecx, dword ptr [eax + 1a9h]
        push   ecx
        mov    eax, dword ptr [eax + 1a5h]
        push   eax
        call   sub_1_024D
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16C0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax], 3
        jne    SHORT loc_1_16D1
        mov    dword ptr [eax], 2
loc_1_16D1:
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        cmp    ecx, eax
        jne    SHORT loc_1_16FB
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_16FB:
        or     eax, eax
        je     SHORT loc_1_1760
        or     ecx, ecx
        je     SHORT loc_1_1760
        mov    esi, dword ptr [ebp + 10h]
        mov    edx, esi
        dec    esi
        or     edx, edx
        je     SHORT loc_1_172A
loc_1_1710:
        mov    bl, byte ptr [ecx + edi]
        mov    dl, byte ptr [eax + edi]
        cmp    bl, dl
        jne    SHORT loc_1_172A
        or     dl, dl
        je     SHORT loc_1_172A
        or     bl, bl
        je     SHORT loc_1_172A
        inc    edi
        mov    edx, esi
        dec    esi
        or     edx, edx
        jne    SHORT loc_1_1710
loc_1_172A:
        or     esi, esi
        jne    SHORT loc_1_172F
        dec    edi
loc_1_172F:
        mov    cl, byte ptr [ecx + edi]
        mov    al, byte ptr [eax + edi]
        cmp    cl, al
        jne    SHORT loc_1_1742
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_1742:
        jbe    SHORT loc_1_1750
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_1750:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_1_1760:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_1_1770:
        push   ebp
        mov    ebp, esp
        sub    esp, 9ch
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 34h]
        mov    dword ptr [ebp - 44h], eax
        db  0C7h, 045h, 0C0h, 094h, 084h, 001h, 000h
        mov    dword ptr [ebp - 3ch], 80000002h
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_18E9
        sub    esi, esi
        mov    dword ptr [ebp - 8], 29h
        mov    dword ptr [ebp - 0ch], 29h
loc_1_17C0:
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 30h], eax
        lea    eax, [ebp - 70h]
        mov    dword ptr [ebp - 2ch], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 24h], 0
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 20h], eax
        lea    eax, [ebp - 9ch]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 18h], esi
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 050h, 001h, 001h, 000h
        add    esp, 20h
        mov    edi, eax
        or     edi, edi
        jne    NEAR PTR loc_1_18B0
        cmp    dword ptr [ebp - 10h], 1
        jne    NEAR PTR loc_1_18BA
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 70h]
        push   eax
        db  0E8h, 00Ah, 003h, 0FEh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_18BA
        db  0A1h, 050h, 082h, 001h, 000h
        push   eax
        push   0
        push   16h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0756
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_18BA
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 12h], ebx
        mov    ax, word ptr [ebx + 6]
        mov    ecx, dword ptr [ebp - 4]
        add    ebx, 0ch
        mov    word ptr [ecx + 8], ax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx + 0ah], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 0eh], eax
        add    ecx, 0ah
        mov    ecx, dword ptr [ebp - 4]
        db  0A1h, 0D4h, 098h, 001h, 000h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 040h, 004h, 0D4h, 098h, 001h, 000h
        db  08Bh, 00Dh, 0D4h, 098h, 001h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 0D4h, 098h, 001h, 000h
        jmp    SHORT loc_1_18BA
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_18B0:
        cmp    edi, 0eah
        jne    SHORT loc_1_18BA
        sub    edi, edi
loc_1_18BA:
        mov    dword ptr [ebp - 8], 29h
        mov    dword ptr [ebp - 0ch], 29h
        inc    esi
        or     edi, edi
        je     NEAR PTR loc_1_17C0
        mov    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 38h], eax
        push   dword ptr [ebp - 38h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_1_18E9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_18F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  081h, 03Dh, 0D4h, 098h, 001h, 000h, 0D4h, 098h, 001h, 000h
        je     SHORT loc_1_1964
        db  0BFh, 0D4h, 098h, 001h, 000h
        sub    ebx, ebx
        lea    esp, [esp]
loc_1_1910:
        db  08Bh, 035h, 0D4h, 098h, 001h, 000h
        mov    eax, esi
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], edi
        mov    eax, dword ptr [eax]
        db  0A3h, 0D4h, 098h, 001h, 000h
        mov    ecx, ebx
loc_1_1926:
        mov    al, byte ptr [esi + ecx + 0ah]
        cmp    al, 20h
        je     SHORT loc_1_1940
        mov    byte ptr [ebp + ecx - 0ch], al
        inc    ecx
        cmp    ecx, 8
        jl     SHORT loc_1_1926
        jmp    SHORT loc_1_1944
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1940:
        mov    byte ptr [ebp + ecx - 0ch], bl
loc_1_1944:
        mov    byte ptr [ebp - 4], bl
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 043h, 002h, 0FEh, 0FFh
        add    esp, 4
        push   ebx
        push   16h
        push   esi
        call   sub_1_075C
        db  039h, 03Dh, 0D4h, 098h, 001h, 000h
        jne    SHORT loc_1_1910
loc_1_1964:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_1970:
        push   ebp
        mov    ebp, esp
        sub    esp, 74h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    ecx, dword ptr [eax + 0ch]
        mov    edi, dword ptr [eax + 10h]
        mov    edx, dword ptr [eax + 14h]
        mov    eax, ecx
        cmp    eax, 300054h
        ja     SHORT loc_1_1A00
        je     NEAR PTR loc_1_1C80
        cmp    eax, 30003ch
        ja     SHORT loc_1_19C0
        je     NEAR PTR loc_1_1C01
        sub    eax, 300000h
        cmp    eax, 14h
        ja     NEAR PTR loc_1_1A7B
        shl    eax, 2
        db  0FFh, 0A0h, 08Ch, 032h, 000h, 000h
        db  090h
loc_1_19C0:
        cmp    eax, 300040h
        je     NEAR PTR loc_1_1C18
        cmp    eax, 300044h
        je     NEAR PTR loc_1_1C26
        cmp    eax, 300048h
        je     NEAR PTR loc_1_1C40
        cmp    eax, 30004ch
        je     NEAR PTR loc_1_1C57
        cmp    eax, 300050h
        je     NEAR PTR loc_1_1C70
        jmp    NEAR PTR loc_1_1A7B
        db  08Dh, 064h, 024h, 000h
loc_1_1A00:
        cmp    eax, 30006ch
        ja     SHORT loc_1_1A46
        je     NEAR PTR loc_1_1CD1
        cmp    eax, 300058h
        je     NEAR PTR loc_1_1C97
        cmp    eax, 30005ch
        je     NEAR PTR loc_1_1CA7
        cmp    eax, 300060h
        je     NEAR PTR loc_1_1CB7
        cmp    eax, 300064h
        je     NEAR PTR loc_1_1CCA
        cmp    eax, 300068h
        je     NEAR PTR loc_1_1CD1
        jmp    SHORT loc_1_1A7B
loc_1_1A46:
        cmp    eax, 300070h
        je     NEAR PTR loc_1_1CE1
        cmp    eax, 300074h
        je     NEAR PTR loc_1_1CE1
        cmp    eax, 300078h
        je     NEAR PTR loc_1_1CF1
        cmp    eax, 3000a0h
        je     NEAR PTR loc_1_1D01
        cmp    eax, -1
        je     NEAR PTR loc_1_3283
loc_1_1A7B:
        mov    dword ptr [ebp - 8], 0c0000010h
        jmp    NEAR PTR loc_1_1D17
loc_1_1A87:
        cmp    edx, 40h
        jb     NEAR PTR loc_1_1D10
        mov    dword ptr [ebp - 14h], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_1D1D
        db  08Bh, 0FFh
loc_1_1AA0:
        cmp    edx, 0ch
        jb     NEAR PTR loc_1_1D10
        mov    dword ptr [ebp - 5ch], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_1D1D
loc_1_1AB7:
        cmp    edx, 1ch
        jb     NEAR PTR loc_1_1D10
        mov    dword ptr [ebp - 28h], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_1D1D
        db  08Bh, 0FFh
loc_1_1AD0:
        xor    eax, eax
        db  066h, 0A1h, 04Ch, 082h, 001h, 000h
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        shl    eax, 2
        add    eax, 4
        cmp    eax, edx
        ja     NEAR PTR loc_1_1BB0
        db  066h, 0A1h, 04Ch, 082h, 001h, 000h
        mov    word ptr [edi], ax
        db  0BEh, 0BCh, 098h, 001h, 000h
        db  066h, 083h, 03Dh, 04Ch, 082h, 001h, 000h, 000h
        mov    word ptr [ebp - 4], 0
        jbe    NEAR PTR loc_1_1BB7
loc_1_1B10:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*2]
        lea    ebx, [edi + eax*4 + 4]
        mov    esi, dword ptr [esi]
        mov    dword ptr [ebx], esi
        mov    ax, word ptr [esi + 0d5h]
        mov    word ptr [ebx + 4], ax
        lea    eax, [esi + 0d7h]
        push   20h
        push   eax
        lea    eax, [ebx + 6]
        push   eax
        call   sub_1_083C
        mov    byte ptr [ebx + 26h], 3
        mov    byte ptr [ebx + 27h], 0
        push   6
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [ebx + 28h]
        push   eax
        call   sub_1_083C
        mov    word ptr [ebx + 2eh], 1
        mov    ax, word ptr [esi + 0d3h]
        mov    word ptr [ebx + 30h], ax
        mov    eax, dword ptr [esi + 0c3h]
        inc    word ptr [ebp - 4]
        mov    word ptr [ebx + 32h], ax
        mov    eax, dword ptr [esi + 0cbh]
        mov    dword ptr [ebx + 34h], eax
        mov    eax, dword ptr [esi + 0cfh]
        mov    dword ptr [ebx + 38h], eax
        mov    byte ptr [ebx + 3ch], 0
        mov    ax, word ptr [ebp - 4]
        db  066h, 03Bh, 005h, 04Ch, 082h, 001h, 000h
        jb     NEAR PTR loc_1_1B10
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_1BB0:
        mov    dword ptr [ebp - 8], 0c0000004h
loc_1_1BB7:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1BC1:
        cmp    edx, 48h
        jb     NEAR PTR loc_1_1D10
        mov    dword ptr [ebp - 4ch], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_1D1D
loc_1_1BD8:
        cmp    edx, 1
        jb     SHORT loc_1_1BF0
        mov    al, byte ptr [edi]
        db  0A2h, 0E4h, 081h, 001h, 000h
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_1_1BF0:
        mov    dword ptr [ebp - 8], 0c0000004h
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1C01:
        cmp    edx, 34h
        jb     NEAR PTR loc_1_1D10
        mov    dword ptr [ebp - 20h], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_1D1D
loc_1_1C18:
        cmp    edx, 38h
        jb     NEAR PTR loc_1_1D10
        jmp    NEAR PTR loc_1_1D06
loc_1_1C26:
        cmp    edx, 34h
        jb     NEAR PTR loc_1_1D10
        mov    dword ptr [ebp - 20h], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_1D1D
        db  08Dh, 049h, 000h
loc_1_1C40:
        cmp    edx, 28h
        jb     NEAR PTR loc_1_1D10
        mov    dword ptr [ebp - 24h], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_1D1D
loc_1_1C57:
        cmp    edx, 28h
        jb     NEAR PTR loc_1_1D10
        mov    dword ptr [ebp - 44h], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_1D1D
        db  08Bh, 0FFh
loc_1_1C70:
        cmp    edx, 38h
        jb     NEAR PTR loc_1_1D10
        jmp    NEAR PTR loc_1_1D06
        db  08Bh, 0FFh
loc_1_1C80:
        cmp    edx, 18h
        jb     NEAR PTR loc_1_1D10
        mov    dword ptr [ebp - 30h], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_1D1D
loc_1_1C97:
        cmp    edx, 50h
        jb     SHORT loc_1_1D10
        mov    dword ptr [ebp - 40h], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_1D1D
loc_1_1CA7:
        cmp    edx, 1ch
        jb     SHORT loc_1_1D10
        mov    dword ptr [ebp - 2ch], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_1D1D
loc_1_1CB7:
        cmp    edx, 98h
        jb     SHORT loc_1_1D10
        mov    dword ptr [ebp - 3ch], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_1D1D
loc_1_1CCA:
        cmp    edx, 38h
        jb     SHORT loc_1_1D10
        jmp    SHORT loc_1_1D06
loc_1_1CD1:
        cmp    edx, 14h
        jb     SHORT loc_1_1D10
        mov    dword ptr [ebp - 38h], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_1D1D
loc_1_1CE1:
        cmp    edx, 14h
        jb     SHORT loc_1_1D10
        mov    dword ptr [ebp - 1ch], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_1D1D
loc_1_1CF1:
        cmp    edx, 28h
        jb     SHORT loc_1_1D10
        mov    dword ptr [ebp - 18h], edi
        mov    ebx, dword ptr [edi + 4]
        mov    edi, dword ptr [ebp - 0ch]
        jmp    SHORT loc_1_1D1D
loc_1_1D01:
        cmp    edx, 38h
        jb     SHORT loc_1_1D10
loc_1_1D06:
        mov    ebx, dword ptr [edi + 4]
        jmp    SHORT loc_1_1D1D
        db  005h, 000h, 000h, 000h, 000h
loc_1_1D10:
        mov    dword ptr [ebp - 8], 0c0000004h
loc_1_1D17:
        mov    edi, dword ptr [ebp - 0ch]
        mov    ebx, dword ptr [ebp - 10h]
loc_1_1D1D:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_1D30
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_1_1D30:
        cmp    ecx, 300068h
        je     SHORT loc_1_1D40
        cmp    ecx, 30006ch
        jne    SHORT loc_1_1D50
loc_1_1D40:
        cmp    ebx, -1
        jne    SHORT loc_1_1D50
        sub    ebx, ebx
        sub    esi, esi
        jmp    SHORT loc_1_1DAB
        db  005h, 000h, 000h, 000h, 000h
loc_1_1D50:
        db  081h, 03Dh, 0BCh, 098h, 001h, 000h, 0BCh, 098h, 001h, 000h
        jne    SHORT loc_1_1D68
        mov    eax, 2bch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1D68:
        or     ebx, ebx
        je     SHORT loc_1_1D98
        db  0A1h, 0BCh, 098h, 001h, 000h
        db  03Dh, 0BCh, 098h, 001h, 000h
        je     SHORT loc_1_1D8C
loc_1_1D78:
        cmp    eax, ebx
        je     SHORT loc_1_1D85
        mov    eax, dword ptr [eax]
        db  03Dh, 0BCh, 098h, 001h, 000h
        jne    SHORT loc_1_1D78
loc_1_1D85:
        db  03Dh, 0BCh, 098h, 001h, 000h
        jne    SHORT loc_1_1D9E
loc_1_1D8C:
        mov    eax, 2bfh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_1D98:
        db  08Bh, 01Dh, 0BCh, 098h, 001h, 000h
loc_1_1D9E:
        inc    word ptr [ebx + 2a6h]
        lea    esi, [ebx + 103h]
loc_1_1DAB:
        cmp    ecx, 300054h
        ja     NEAR PTR loc_1_1E51
        je     NEAR PTR loc_1_2D53
        cmp    ecx, 30003ch
        ja     SHORT loc_1_1E10
        je     NEAR PTR loc_1_2142
        cmp    ecx, 300000h
        je     NEAR PTR loc_1_1ED5
        cmp    ecx, 300004h
        je     NEAR PTR loc_1_2180
        cmp    ecx, 300008h
        je     NEAR PTR loc_1_2142
        cmp    ecx, 300010h
        je     NEAR PTR loc_1_2142
        cmp    ecx, 300014h
        je     NEAR PTR loc_1_2710
        jmp    NEAR PTR loc_1_3267
        db  08Dh, 064h, 024h, 000h
loc_1_1E10:
        cmp    ecx, 300040h
        je     NEAR PTR loc_1_2719
        cmp    ecx, 300044h
        je     NEAR PTR loc_1_2142
        cmp    ecx, 300048h
        je     NEAR PTR loc_1_2B80
        cmp    ecx, 30004ch
        je     NEAR PTR loc_1_2C00
        cmp    ecx, 300050h
        je     NEAR PTR loc_1_2CA0
        jmp    NEAR PTR loc_1_3267
loc_1_1E51:
        cmp    ecx, 30006ch
        ja     SHORT loc_1_1EA0
        je     NEAR PTR loc_1_2FEA
        cmp    ecx, 300058h
        je     NEAR PTR loc_1_2DD3
        cmp    ecx, 30005ch
        je     NEAR PTR loc_1_2E71
        cmp    ecx, 300060h
        je     NEAR PTR loc_1_2EF0
        cmp    ecx, 300064h
        je     NEAR PTR loc_1_2F90
        cmp    ecx, 300068h
        je     NEAR PTR loc_1_2FEA
        jmp    NEAR PTR loc_1_3267
loc_1_1EA0:
        cmp    ecx, 300070h
        je     NEAR PTR loc_1_3031
        cmp    ecx, 300074h
        je     NEAR PTR loc_1_30A0
        cmp    ecx, 300078h
        je     NEAR PTR loc_1_3120
        cmp    ecx, 3000a0h
        je     NEAR PTR loc_1_322A
        jmp    NEAR PTR loc_1_3267
loc_1_1ED5:
        cmp    dword ptr [esi], 2
        je     SHORT loc_1_1EF0
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 3ch], 812h
        jmp    NEAR PTR loc_1_3267
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1EF0:
        mov    dword ptr [esi], 4
        inc    dword ptr [ebx + 0eh]
        cmp    byte ptr [esi + 110h], 0
        je     SHORT loc_1_1F37
        push   0
        db  068h, 0C8h, 084h, 001h, 000h
        db  0E8h, 0B2h, 0FCh, 0FDh, 0FFh
        add    esp, 8
        db  0A3h, 018h, 009h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_1F37
        xor    eax, eax
        db  066h, 0A1h, 00Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 000h, 0FDh, 0FDh, 0FFh
        add    esp, 8
loc_1_1F37:
        db  08Bh, 03Dh, 0D4h, 098h, 001h, 000h
        db  081h, 0FFh, 0D4h, 098h, 001h, 000h
        je     SHORT loc_1_1F96
        mov    byte ptr [ebp - 60h], 0
        db  081h, 0FFh, 0D4h, 098h, 001h, 000h
        je     SHORT loc_1_1FBC
loc_1_1F51:
        lea    eax, [edi + 0ah]
        lea    ecx, [ebp - 68h]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], eax
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_1F8A
        mov    eax, ecx
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 02Eh, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0ADh, 0FCh, 0FDh, 0FFh
        add    esp, 0ch
loc_1_1F8A:
        mov    edi, dword ptr [edi]
        db  081h, 0FFh, 0D4h, 098h, 001h, 000h
        jne    SHORT loc_1_1F51
        jmp    SHORT loc_1_1FBC
loc_1_1F96:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_1FBC
        xor    eax, eax
        db  066h, 0A1h, 030h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 07Bh, 0FCh, 0FDh, 0FFh
        add    esp, 8
loc_1_1FBC:
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [esi + 4], eax
        mov    eax, ecx
        add    eax, 8
        mov    dword ptr [ebp - 34h], eax
        cmp    dword ptr [eax], 7d0h
        jae    SHORT loc_1_1FDB
        mov    dword ptr [eax], 7d0h
loc_1_1FDB:
        mov    eax, dword ptr [ebp - 34h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebx + 0cbh]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [ebx + 0cfh]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebx + 0c3h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebx + 0c3h]
        mov    dword ptr [ecx + 20h], eax
        lea    edi, [esi + 90h]
        mov    eax, dword ptr [ebp - 34h]
        push   6
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 24h], eax
        mov    dword ptr [ecx + 4], ebx
        mov    ax, word ptr [ebx + 2b0h]
        mov    word ptr [ecx], ax
        lea    eax, [ebx + 8]
        push   eax
        push   edi
        call   sub_1_083C
        push   6
        db  068h, 0D4h, 084h, 001h, 000h
        lea    eax, [esi + 96h]
        push   eax
        call   sub_1_083C
        mov    al, byte ptr [esi + 94h]
        mov    byte ptr [esi + 9ah], al
        mov    al, byte ptr [esi + 95h]
        mov    byte ptr [esi + 9bh], al
        push   6
        push   edi
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 0eh
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebx + 0cbh]
        and    eax, 0fffffff0h
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [ebx + 0cfh]
        and    eax, 0fffffff0h
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [ebx + 0c3h]
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [ebx + 0c3h]
        mov    dword ptr [esi + 1ch], eax
        lea    eax, [esi + 9ch]
        lea    edi, [esi + 0b0h]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    ecx, 7
        mov    dword ptr [eax + 10h], 0
        sub    eax, eax
        rep stosd
        mov    ecx, dword ptr [ebp - 14h]
        xor    eax, eax
        mov    ax, word ptr [ecx + 0ch]
        mov    dword ptr [esi], 1
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 20h], ebx
        lea    eax, [esi + 24h]
        mov    dword ptr [esi + 28h], eax
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 2ch], 0
        lea    eax, [esi + 30h]
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 38h], 0
        mov    dword ptr [esi + 55h], 0
        lea    eax, [esi + 40h]
        mov    dword ptr [esi + 44h], eax
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3A10
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ecx + 34h]
        mov    dword ptr [esi + 1a5h], eax
        mov    eax, dword ptr [ecx + 38h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [esi + 1a9h], eax
        mov    dword ptr [ecx + 3ch], 800h
        jmp    NEAR PTR loc_1_3267
loc_1_2142:
        cmp    ecx, 300004h
        je     SHORT loc_1_2180
        cmp    ecx, 300008h
        je     NEAR PTR loc_1_25B0
        cmp    ecx, 300010h
        je     NEAR PTR loc_1_2680
        cmp    ecx, 30003ch
        je     NEAR PTR loc_1_26B0
        cmp    ecx, 300044h
        je     NEAR PTR loc_1_26E3
        jmp    NEAR PTR loc_1_26FE
        db  090h
loc_1_2180:
        mov    eax, dword ptr [esi]
        cmp    eax, 2
        jne    SHORT loc_1_2193
        mov    dword ptr [ebp - 8], 2c0h
        jmp    NEAR PTR loc_1_26FE
loc_1_2193:
        cmp    eax, 1
        jne    NEAR PTR loc_1_25A2
        sub    edi, edi
        mov    dword ptr [esi], 3
        inc    dword ptr [ebx + 0eh]
        db  039h, 03Dh, 018h, 009h, 000h, 000h
        je     SHORT loc_1_21CC
        xor    eax, eax
        db  066h, 0A1h, 018h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 06Bh, 0FAh, 0FDh, 0FFh
        add    esp, 8
loc_1_21CC:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_2226
        mov    eax, dword ptr [esi + 0b0h]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 018h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 03Eh, 0FAh, 0FDh, 0FFh
        add    esp, 0ch
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_2253
        mov    eax, dword ptr [esi + 0b4h]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 01Ah, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 011h, 0FAh, 0FDh, 0FFh
        add    esp, 0ch
loc_1_2226:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_2280
        mov    eax, dword ptr [esi + 0b8h]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 01Ch, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0E4h, 0F9h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_2253:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_22AD
        mov    eax, dword ptr [esi + 0bch]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 01Eh, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0B7h, 0F9h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_2280:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_22DA
        mov    eax, dword ptr [esi + 0c0h]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 020h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 08Ah, 0F9h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_22AD:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_2307
        mov    eax, dword ptr [esi + 0c4h]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 022h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 05Dh, 0F9h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_22DA:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_2334
        mov    eax, dword ptr [esi + 0c8h]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 024h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 030h, 0F9h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_2307:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_2361
        mov    eax, dword ptr [esi + 0a0h]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 026h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 003h, 0F9h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_2334:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_238E
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 028h, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0D6h, 0F8h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_2361:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_23BB
        mov    eax, dword ptr [esi + 0a8h]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 02Ah, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0A9h, 0F8h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_238E:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_23BB
        mov    eax, dword ptr [esi + 0a4h]
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 02Ch, 001h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 07Ch, 0F8h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_23BB:
        mov    eax, dword ptr [esi + 0ech]
        cmp    eax, 2
        je     SHORT loc_1_23D7
        cmp    eax, 3
        je     SHORT loc_1_23E6
        cmp    eax, 4
        je     SHORT loc_1_23F6
        cmp    eax, 5
        je     SHORT loc_1_2406
        jmp    SHORT loc_1_2415
loc_1_23D7:
        mov    eax, dword ptr [ebp - 5ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        push   esi
        call   sub_1_79D0
        jmp    SHORT loc_1_2413
loc_1_23E6:
        push   3f00h
        push   esi
        call   PPPMAC_Service_06
        add    esp, 8
        jmp    SHORT loc_1_2415
loc_1_23F6:
        push   2100h
        push   esi
        call   PPPMAC_Service_06
        add    esp, 8
        jmp    SHORT loc_1_2415
loc_1_2406:
        mov    eax, dword ptr [ebp - 5ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        push   esi
        call   sub_1_114D0
loc_1_2413:
        mov    edi, eax
loc_1_2415:
        cmp    edi, 103h
        jne    SHORT loc_1_2460
        mov    dword ptr [ebp - 74h], 0
        db  0C7h, 045h, 090h, 0E0h, 032h, 000h, 000h
        mov    dword ptr [ebp - 6ch], esi
        push   1f4h
        lea    eax, [ebp - 74h]
        push   eax
        call   sub_1_0732
        cmp    dword ptr [esi], 2
        je     SHORT loc_1_244B
loc_1_2441:
        call   sub_1_021A
        cmp    dword ptr [esi], 2
        jne    SHORT loc_1_2441
loc_1_244B:
        lea    eax, [ebp - 2]
        push   eax
        lea    eax, [ebp - 74h]
        push   eax
        call   sub_1_0738
        jmp    SHORT loc_1_2466
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2460:
        mov    dword ptr [esi], 2
loc_1_2466:
        push   esi
        call   sub_1_3B00
        mov    eax, dword ptr [esi + 80h]
        or     eax, eax
        je     SHORT loc_1_2489
        push   eax
        call   sub_1_018C
        add    esp, 4
        mov    dword ptr [esi + 80h], 0
loc_1_2489:
        mov    eax, dword ptr [esi + 84h]
        or     eax, eax
        je     SHORT loc_1_24AE
        push   eax
        call   sub_1_018C
        add    esp, 4
        mov    dword ptr [esi + 84h], 0
        mov    byte ptr [esi + 7eh], 0
        mov    byte ptr [esi + 7fh], 0
loc_1_24AE:
        cmp    dword ptr [esi + 55h], 0
        je     SHORT loc_1_24BD
        push   esi
        call   sub_1_44B0
        add    esp, 4
loc_1_24BD:
        cmp    word ptr [esi + 1a3h], 1
        jbe    SHORT loc_1_24D6
loc_1_24C7:
        call   sub_1_021A
        cmp    word ptr [esi + 1a3h], 1
        ja     SHORT loc_1_24C7
loc_1_24D6:
        mov    eax, dword ptr [esi + 0ech]
        cmp    eax, 2
        je     SHORT loc_1_24F0
        cmp    eax, 4
        je     SHORT loc_1_24F8
        cmp    eax, 5
        je     SHORT loc_1_2507
        jmp    SHORT loc_1_250D
        db  08Dh, 049h, 000h
loc_1_24F0:
        push   esi
        call   sub_1_6560
        jmp    SHORT loc_1_250D
loc_1_24F8:
        push   esi
        call   sub_1_5920
        mov    byte ptr [esi + 112h], 0
        jmp    SHORT loc_1_250D
loc_1_2507:
        push   esi
        call   sub_1_11480
loc_1_250D:
        mov    dword ptr [esi + 0e8h], 806h
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 104h]
        push   eax
        call   sub_1_0738
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 48h]
        push   eax
        call   sub_1_0738
        mov    eax, dword ptr [esi + 0feh]
        or     eax, eax
        je     SHORT loc_1_2554
        push   0
        mov    ecx, dword ptr [eax + 48h]
        push   ecx
        push   eax
        call   sub_1_075C
        mov    dword ptr [esi + 0feh], 0
loc_1_2554:
        mov    dword ptr [esi + 69h], 0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_26FE
        xor    eax, eax
        db  066h, 0A1h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0B2h, 0F6h, 0FDh, 0FFh
        add    esp, 8
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 08Dh, 0F6h, 0FDh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 018h, 009h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_26FE
loc_1_25A2:
        mov    dword ptr [ebp - 8], 2c1h
        jmp    NEAR PTR loc_1_26FE
        db  08Bh, 0FFh
loc_1_25B0:
        cmp    dword ptr [esi], 1
        je     SHORT loc_1_25C1
        mov    dword ptr [ebp - 8], 2c1h
        jmp    NEAR PTR loc_1_26FE
loc_1_25C1:
        mov    eax, dword ptr [ebp - 28h]
        cmp    dword ptr [ebx + 0cbh], 1
        mov    ecx, dword ptr [eax + 8]
        sbb    eax, eax
        neg    eax
        test   ecx, eax
        jne    NEAR PTR loc_1_2670
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 0ch
        mov    dword ptr [ebp - 58h], eax
        cmp    dword ptr [ebx + 0cfh], 1
        sbb    edx, edx
        neg    edx
        test   dword ptr [eax], edx
        jne    SHORT loc_1_2670
        mov    edx, dword ptr [ebx + 0c3h]
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 10h
        mov    dword ptr [ebp - 54h], eax
        cmp    dword ptr [eax], edx
        ja     SHORT loc_1_2670
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 14h
        mov    dword ptr [ebp - 50h], eax
        cmp    dword ptr [eax], edx
        ja     SHORT loc_1_2670
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [esi + 10h], ecx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [ebp - 54h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [ebp - 50h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 1ch], eax
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 18h
        mov    dword ptr [ebp - 48h], eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     NEAR PTR loc_1_26FE
        cmp    eax, 7d0h
        jae    SHORT loc_1_2650
        mov    eax, dword ptr [ebp - 48h]
        mov    dword ptr [eax], 7d0h
loc_1_2650:
        mov    eax, dword ptr [ebp - 48h]
        push   3f00h
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 8], eax
        push   esi
        call   PPPMAC_Service_05
        add    esp, 8
        jmp    NEAR PTR loc_1_26FE
        db  005h, 000h, 000h, 000h, 000h
loc_1_2670:
        mov    dword ptr [ebp - 8], 2c2h
        jmp    NEAR PTR loc_1_26FE
        db  08Dh, 064h, 024h, 000h
loc_1_2680:
        push   14h
        lea    eax, [esi + 9ch]
        push   eax
        mov    eax, dword ptr [ebp - 4ch]
        add    eax, 8
        push   eax
        call   sub_1_083C
        push   1ch
        lea    eax, [esi + 0b0h]
        push   eax
        mov    eax, dword ptr [ebp - 4ch]
        add    eax, 1ch
        push   eax
        call   sub_1_083C
        jmp    SHORT loc_1_26FE
        db  08Dh, 064h, 024h, 000h
loc_1_26B0:
        mov    ecx, dword ptr [ebp - 20h]
        mov    eax, dword ptr [esi + 0d0h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esi + 0d4h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esi + 0d8h]
        or     eax, eax
        je     SHORT loc_1_26D7
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 10h], eax
        jmp    SHORT loc_1_26FE
loc_1_26D7:
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [ecx + 10h], 0
        jmp    SHORT loc_1_26FE
loc_1_26E3:
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [ecx + 8], 800h
        mov    dword ptr [ecx + 0ch], 0
        mov    dword ptr [esi + 0e4h], 0
loc_1_26FE:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_3267
        inc    dword ptr [ebx + 0eh]
        jmp    NEAR PTR loc_1_3267
loc_1_2710:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_2719:
        inc    dword ptr [ebx + 0eh]
        mov    eax, dword ptr [edi + 20h]
        cmp    eax, 2
        je     SHORT loc_1_2754
        cmp    eax, 3
        je     NEAR PTR loc_1_2841
        cmp    eax, 4
        je     NEAR PTR loc_1_2920
        cmp    eax, 5
        je     NEAR PTR loc_1_2960
        cmp    eax, 6
        je     NEAR PTR loc_1_2A88
        mov    dword ptr [edi + 8], 802h
        jmp    NEAR PTR loc_1_3267
loc_1_2754:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_2789
        xor    eax, eax
        db  066h, 0A1h, 0F0h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0AEh, 0F4h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_2789:
        mov    dword ptr [esi + 0e8h], 807h
        mov    dword ptr [esi + 0ech], 2
        db  0C7h, 086h, 0F0h, 000h, 000h, 000h, 0A0h, 047h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 086h, 0F4h, 000h, 000h, 000h, 0C0h, 04Eh, 000h, 000h
        mov    eax, dword ptr [eax + 24h]
        or     eax, eax
        jne    SHORT loc_1_27D0
        mov    dword ptr [edi + 8], 802h
        mov    dword ptr [ebp - 8], 802h
        jmp    NEAR PTR loc_1_3267
        db  08Bh, 0FFh
loc_1_27D0:
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0e4h], eax
        mov    eax, dword ptr [ebp + 8]
        push   edi
        mov    eax, dword ptr [eax + 24h]
        push   esi
        mov    dword ptr [eax + 4], 0
        call   sub_1_1290
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_2800
        mov    dword ptr [edi + 8], eax
        jmp    NEAR PTR loc_1_3267
        db  005h, 000h, 000h, 000h, 000h
loc_1_2800:
        push   esi
        call   sub_1_5EC0
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_2815
        mov    dword ptr [edi + 8], eax
        jmp    NEAR PTR loc_1_3267
loc_1_2815:
        push   esi
        call   sub_1_4750
        push   esi
        call   sub_1_1040
        or     eax, eax
        je     SHORT loc_1_282B
        push   esi
        call   sub_1_64B0
loc_1_282B:
        mov    dword ptr [edi + 8], 801h
        mov    dword ptr [esi + 0d0h], 801h
        jmp    NEAR PTR loc_1_3267
loc_1_2841:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_2876
        xor    eax, eax
        db  066h, 0A1h, 0F2h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0C1h, 0F3h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_2876:
        mov    dword ptr [esi + 0e8h], 808h
        mov    dword ptr [esi + 0ech], 3
        db  0C7h, 086h, 0F0h, 000h, 000h, 000h, 0A0h, 051h, 000h, 000h
        db  0C7h, 086h, 0F4h, 000h, 000h, 000h, 0F0h, 052h, 000h, 000h
        push   3f00h
        push   ebx
        call   sub_1_15E0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_28F0
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_28D6
        xor    eax, eax
        db  066h, 0A1h, 012h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 061h, 0F3h, 0FDh, 0FFh
        add    esp, 8
loc_1_28D6:
        mov    dword ptr [edi + 8], 804h
        mov    dword ptr [esi + 0d0h], 804h
        jmp    NEAR PTR loc_1_3267
        db  08Dh, 064h, 024h, 000h
loc_1_28F0:
        push   esi
        call   sub_1_5170
        push   3f00h
        push   esi
        call   PPPMAC_Service_05
        add    esp, 8
        mov    dword ptr [edi + 8], 800h
        mov    dword ptr [esi + 0d0h], 800h
        jmp    NEAR PTR loc_1_3267
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2920:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     NEAR PTR loc_1_2AC4
        xor    eax, eax
        db  066h, 0A1h, 0F4h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0DEh, 0F2h, 0FDh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_1_2AC4
        db  08Bh, 0FFh
loc_1_2960:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_2995
        xor    eax, eax
        db  066h, 0A1h, 0F8h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 0A2h, 0F2h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_2995:
        mov    dword ptr [esi + 0e8h], 80ah
        mov    dword ptr [esi + 0ech], 5
        db  0C7h, 086h, 0F0h, 000h, 000h, 000h, 0A0h, 00Dh, 001h, 000h
        db  0C7h, 086h, 0F4h, 000h, 000h, 000h, 080h, 00Eh, 001h, 000h
        push   2b00h
        push   ebx
        call   sub_1_15E0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_2A0B
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_29F5
        xor    eax, eax
        db  066h, 0A1h, 016h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 042h, 0F2h, 0FDh, 0FFh
        add    esp, 8
loc_1_29F5:
        mov    dword ptr [edi + 8], 804h
        mov    dword ptr [esi + 0d0h], 804h
        jmp    NEAR PTR loc_1_3267
loc_1_2A0B:
        push   edi
        push   esi
        call   sub_1_1290
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_2A21
        mov    dword ptr [edi + 8], eax
        jmp    NEAR PTR loc_1_3267
loc_1_2A21:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        or     eax, eax
        jne    SHORT loc_1_2A40
        mov    dword ptr [edi + 8], 802h
        mov    dword ptr [ebp - 8], 802h
        jmp    NEAR PTR loc_1_3267
        db  08Bh, 0FFh
loc_1_2A40:
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0e4h], eax
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [eax + 4], 0
        call   sub_1_112E0
        or     al, al
        jne    SHORT loc_1_2A72
        mov    dword ptr [edi + 8], 802h
        mov    dword ptr [ebp - 8], 802h
        jmp    NEAR PTR loc_1_3267
loc_1_2A72:
        mov    dword ptr [esi + 0d0h], 801h
        mov    dword ptr [edi + 8], 801h
        jmp    NEAR PTR loc_1_3267
loc_1_2A88:
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_2ABD
        xor    eax, eax
        db  066h, 0A1h, 0F6h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        xor    eax, eax
        db  066h, 0A1h, 010h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 07Ah, 0F1h, 0FDh, 0FFh
        add    esp, 0ch
loc_1_2ABD:
        mov    byte ptr [esi + 112h], 1
loc_1_2AC4:
        mov    dword ptr [esi + 0e8h], 809h
        mov    dword ptr [esi + 0ech], 4
        db  0C7h, 086h, 0F0h, 000h, 000h, 000h, 0C0h, 05Ah, 000h, 000h
        db  0C7h, 086h, 0F4h, 000h, 000h, 000h, 060h, 05Ch, 000h, 000h
        push   2100h
        push   ebx
        call   sub_1_15E0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_2B3A
        db  083h, 03Dh, 018h, 009h, 000h, 000h, 000h
        je     SHORT loc_1_2B24
        xor    eax, eax
        db  066h, 0A1h, 014h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        db  0A1h, 018h, 009h, 000h, 000h
        push   eax
        db  0E8h, 013h, 0F1h, 0FDh, 0FFh
        add    esp, 8
loc_1_2B24:
        mov    dword ptr [edi + 8], 804h
        mov    dword ptr [esi + 0d0h], 804h
        jmp    NEAR PTR loc_1_3267
loc_1_2B3A:
        push   esi
        call   sub_1_59D0
        mov    dword ptr [edi + 8], eax
        cmp    eax, 800h
        je     SHORT loc_1_2B55
        mov    dword ptr [esi + 0d0h], eax
        jmp    NEAR PTR loc_1_3267
loc_1_2B55:
        push   edi
        push   esi
        call   sub_1_1290
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_2B70
        mov    dword ptr [edi + 8], eax
        jmp    NEAR PTR loc_1_3267
        db  005h, 000h, 000h, 000h, 000h
loc_1_2B70:
        mov    dword ptr [esi + 0d0h], 800h
        jmp    NEAR PTR loc_1_3267
        db  090h
loc_1_2B80:
        mov    edx, dword ptr [esi + 0d8h]
        cmp    dword ptr [esi], 1
        je     SHORT loc_1_2B97
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2B97:
        or     edx, edx
        jne    SHORT loc_1_2BA7
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2BA7:
        mov    eax, dword ptr [esi + 0d0h]
        cmp    eax, 800h
        je     SHORT loc_1_2BCA
        cmp    eax, 801h
        je     SHORT loc_1_2BCA
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 10h], 802h
        jmp    NEAR PTR loc_1_3267
loc_1_2BCA:
        inc    dword ptr [ebx + 0eh]
        mov    ecx, dword ptr [edx + 0ch]
        mov    eax, dword ptr [ebp - 24h]
        sub    ecx, 18h
        mov    dword ptr [eax + 10h], 800h
        mov    edi, dword ptr [edx + 8]
        mov    dword ptr [eax + 8], edi
        mov    edi, dword ptr [edx + 14h]
        add    edx, 18h
        mov    dword ptr [eax + 14h], edi
        push   ecx
        push   edx
        add    eax, 18h
        push   eax
        call   sub_1_083C
        jmp    NEAR PTR loc_1_3267
        db  08Dh, 064h, 024h, 000h
loc_1_2C00:
        cmp    dword ptr [esi], 1
        je     SHORT loc_1_2C11
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2C11:
        cmp    dword ptr [esi + 0e0h], 0
        jne    SHORT loc_1_2C26
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2C26:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        or     eax, eax
        jne    SHORT loc_1_2C46
        mov    eax, dword ptr [ebp - 44h]
        mov    dword ptr [ebp - 8], 802h
        mov    dword ptr [eax + 10h], 802h
        jmp    NEAR PTR loc_1_3267
loc_1_2C46:
        mov    ecx, dword ptr [esi + 0d0h]
        cmp    ecx, 800h
        je     SHORT loc_1_2C70
        cmp    ecx, 801h
        je     SHORT loc_1_2C70
        mov    eax, dword ptr [ebp - 44h]
        mov    dword ptr [eax + 10h], 802h
        jmp    NEAR PTR loc_1_3267
        db  005h, 000h, 000h, 000h, 000h
loc_1_2C70:
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0e4h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [eax + 4], 0
        inc    dword ptr [ebx + 0eh]
        mov    eax, dword ptr [ebp - 44h]
        push   eax
        mov    eax, dword ptr [esi + 0dch]
        push   eax
        call   dword ptr [esi + 0e0h]
        jmp    NEAR PTR loc_1_3267
        db  08Bh, 0FFh
loc_1_2CA0:
        cmp    dword ptr [esi], 1
        je     SHORT loc_1_2CB1
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2CB1:
        cmp    dword ptr [esi + 0e0h], 0
        jne    SHORT loc_1_2CC6
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2CC6:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 24h]
        or     ecx, ecx
        jne    SHORT loc_1_2CE3
        mov    dword ptr [edi + 8], 802h
        mov    dword ptr [ebp - 8], 802h
        jmp    NEAR PTR loc_1_3267
loc_1_2CE3:
        mov    eax, dword ptr [esi + 0d0h]
        cmp    eax, 800h
        je     SHORT loc_1_2D03
        cmp    eax, 801h
        je     SHORT loc_1_2D03
        mov    dword ptr [edi + 8], 802h
        jmp    NEAR PTR loc_1_3267
loc_1_2D03:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0e4h], eax
        mov    eax, dword ptr [ebp + 8]
        push   edi
        mov    eax, dword ptr [eax + 24h]
        push   esi
        mov    dword ptr [eax + 4], 0
        inc    dword ptr [ebx + 0eh]
        call   sub_1_1420
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_2D40
        mov    dword ptr [edi + 8], 802h
        mov    dword ptr [ebp - 8], 802h
        jmp    NEAR PTR loc_1_3267
        db  08Dh, 064h, 024h, 000h
loc_1_2D40:
        push   edi
        mov    eax, dword ptr [esi + 0dch]
        push   eax
        call   dword ptr [esi + 0e0h]
        jmp    NEAR PTR loc_1_3267
loc_1_2D53:
        cmp    dword ptr [esi], 1
        je     SHORT loc_1_2D64
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_2D64:
        mov    ecx, dword ptr [esi + 0d8h]
        or     ecx, ecx
        jne    SHORT loc_1_2D80
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2D80:
        mov    eax, dword ptr [esi + 0d0h]
        cmp    eax, 800h
        je     SHORT loc_1_2DA3
        cmp    eax, 801h
        je     SHORT loc_1_2DA3
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 10h], 802h
        jmp    NEAR PTR loc_1_3267
loc_1_2DA3:
        mov    eax, dword ptr [ebp - 30h]
        inc    dword ptr [ebx + 0eh]
        mov    dword ptr [eax + 10h], 800h
        add    ecx, 14h
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ecx - 8]
        sub    eax, 14h
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 14h
        push   eax
        call   sub_1_083C
        jmp    NEAR PTR loc_1_3267
loc_1_2DD3:
        cmp    dword ptr [esi], 1
        je     SHORT loc_1_2DE4
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2DE4:
        cmp    dword ptr [esi + 0e0h], 0
        jne    SHORT loc_1_2DF9
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2DF9:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 24h]
        or     ecx, ecx
        jne    SHORT loc_1_2E20
        mov    eax, dword ptr [ebp - 40h]
        mov    dword ptr [ebp - 8], 802h
        mov    dword ptr [eax + 10h], 802h
        jmp    NEAR PTR loc_1_3267
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2E20:
        mov    eax, dword ptr [esi + 0d0h]
        cmp    eax, 800h
        je     SHORT loc_1_2E43
        cmp    eax, 801h
        je     SHORT loc_1_2E43
        mov    eax, dword ptr [ebp - 40h]
        mov    dword ptr [eax + 10h], 802h
        jmp    NEAR PTR loc_1_3267
loc_1_2E43:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0e4h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [eax + 4], 0
        inc    dword ptr [ebx + 0eh]
        mov    eax, dword ptr [ebp - 40h]
        push   eax
        mov    eax, dword ptr [esi + 0dch]
        push   eax
        call   dword ptr [esi + 0e0h]
        jmp    NEAR PTR loc_1_3267
loc_1_2E71:
        cmp    dword ptr [esi], 1
        je     SHORT loc_1_2E82
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2E82:
        mov    ecx, dword ptr [esi + 0d8h]
        or     ecx, ecx
        jne    SHORT loc_1_2E98
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2E98:
        mov    eax, dword ptr [esi + 0d0h]
        cmp    eax, 800h
        je     SHORT loc_1_2EC0
        cmp    eax, 801h
        je     SHORT loc_1_2EC0
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [eax + 10h], 802h
        jmp    NEAR PTR loc_1_3267
        db  005h, 000h, 000h, 000h, 000h
loc_1_2EC0:
        mov    eax, dword ptr [ebp - 2ch]
        inc    dword ptr [ebx + 0eh]
        mov    dword ptr [eax + 10h], 800h
        add    ecx, 14h
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ecx - 8]
        sub    eax, 14h
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 2ch]
        add    eax, 14h
        push   eax
        call   sub_1_083C
        jmp    NEAR PTR loc_1_3267
loc_1_2EF0:
        cmp    dword ptr [esi], 1
        je     SHORT loc_1_2F01
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2F01:
        cmp    dword ptr [esi + 0e0h], 0
        jne    SHORT loc_1_2F16
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2F16:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        or     eax, eax
        jne    SHORT loc_1_2F36
        mov    eax, dword ptr [ebp - 3ch]
        mov    dword ptr [ebp - 8], 802h
        mov    dword ptr [eax + 10h], 802h
        jmp    NEAR PTR loc_1_3267
loc_1_2F36:
        mov    ecx, dword ptr [esi + 0d0h]
        cmp    ecx, 800h
        je     SHORT loc_1_2F60
        cmp    ecx, 801h
        je     SHORT loc_1_2F60
        mov    eax, dword ptr [ebp - 3ch]
        mov    dword ptr [eax + 10h], 802h
        jmp    NEAR PTR loc_1_3267
        db  005h, 000h, 000h, 000h, 000h
loc_1_2F60:
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 0e4h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [eax + 4], 0
        inc    dword ptr [ebx + 0eh]
        mov    eax, dword ptr [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [esi + 0dch]
        push   eax
        call   dword ptr [esi + 0e0h]
        jmp    NEAR PTR loc_1_3267
        db  08Bh, 0FFh
loc_1_2F90:
        cmp    dword ptr [esi], 1
        je     SHORT loc_1_2FA1
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_3267
loc_1_2FA1:
        push   edi
        push   esi
        call   sub_1_1420
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_2FC2
        mov    dword ptr [edi + 8], 802h
        mov    dword ptr [ebp - 8], 802h
        jmp    NEAR PTR loc_1_3267
loc_1_2FC2:
        mov    eax, dword ptr [esi + 0d0h]
        cmp    eax, 800h
        je     NEAR PTR loc_1_3260
        cmp    eax, 801h
        je     NEAR PTR loc_1_3260
        mov    dword ptr [edi + 8], 802h
        jmp    NEAR PTR loc_1_3267
loc_1_2FEA:
        or     ebx, ebx
        je     SHORT loc_1_3010
        inc    dword ptr [ebx + 0eh]
        or     ebx, ebx
        je     SHORT loc_1_3010
        mov    edx, dword ptr [esi + 0e8h]
        cmp    edx, 807h
        je     SHORT loc_1_3010
        mov    eax, dword ptr [ebp - 38h]
        mov    dword ptr [eax + 0ch], edx
        jmp    NEAR PTR loc_1_3267
        db  08Bh, 0FFh
loc_1_3010:
        cmp    ecx, 300068h
        mov    eax, dword ptr [ebp - 38h]
        push   eax
        push   esi
        jne    SHORT loc_1_3027
        call   sub_1_7660
        jmp    NEAR PTR loc_1_3267
loc_1_3027:
        call   sub_1_77A0
        jmp    NEAR PTR loc_1_3267
loc_1_3031:
        mov    eax, dword ptr [esi + 0d0h]
        cmp    eax, 800h
        mov    ecx, dword ptr [ebp - 1ch]
        je     SHORT loc_1_3050
        mov    dword ptr [ecx + 0ch], eax
        jmp    NEAR PTR loc_1_3267
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3050:
        mov    dword ptr [ecx + 0ch], 800h
        push   ecx
        push   esi
        call   sub_1_5790
        push   esi
        call   sub_1_5A60
        push   esi
        call   sub_1_1040
        or     eax, eax
        je     SHORT loc_1_3090
        push   2100h
        push   esi
        call   PPPMAC_Service_05
        add    esp, 8
        mov    dword ptr [esi + 0d0h], 800h
        jmp    NEAR PTR loc_1_3267
        db  005h, 000h, 000h, 000h, 000h
loc_1_3090:
        mov    dword ptr [esi + 0d0h], 801h
        jmp    NEAR PTR loc_1_3267
        db  090h
loc_1_30A0:
        mov    eax, dword ptr [esi + 0d0h]
        cmp    eax, 800h
        je     SHORT loc_1_30B8
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 0ch], eax
        jmp    NEAR PTR loc_1_3267
loc_1_30B8:
        cmp    dword ptr [esi + 187h], 0
        je     SHORT loc_1_30D2
        test   byte ptr [esi + 18bh], 1
        je     SHORT loc_1_30D2
        mov    ecx, dword ptr [esi + 18fh]
        jmp    SHORT loc_1_30D8
loc_1_30D2:
        mov    ecx, dword ptr [esi + 17bh]
loc_1_30D8:
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        mov    ecx, dword ptr [ebp - 1ch]
        or     edx, eax
        mov    dword ptr [ecx + 10h], edx
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [esi + 0d0h], 800h
        mov    dword ptr [ecx + 0ch], 800h
        jmp    NEAR PTR loc_1_3267
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3120:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 8], 800h
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esi + 18bh], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [eax + 14h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 18fh], edx
        mov    ecx, dword ptr [eax + 18h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 193h], edx
        mov    ecx, dword ptr [eax + 1ch]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 197h], edx
        mov    ecx, dword ptr [eax + 20h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 19bh], edx
        mov    ecx, dword ptr [eax + 24h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [esi + 19fh], edx
        mov    dword ptr [esi + 187h], 1
        jmp    SHORT loc_1_3267
loc_1_322A:
        cmp    dword ptr [esi], 1
        je     SHORT loc_1_3238
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    SHORT loc_1_3267
loc_1_3238:
        inc    dword ptr [ebx + 0eh]
        push   edi
        push   esi
        call   sub_1_1420
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_1_3260
        mov    dword ptr [edi + 8], 802h
        mov    dword ptr [ebp - 8], 802h
        jmp    SHORT loc_1_3267
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3260:
        mov    dword ptr [edi + 8], 800h
loc_1_3267:
        or     ebx, ebx
        je     SHORT loc_1_326E
        dec    dword ptr [ebx + 0eh]
loc_1_326E:
        or     esi, esi
        je     SHORT loc_1_3279
        dec    word ptr [esi + 1a3h]
loc_1_3279:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_3283:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_328C:
        xchg   dword ptr [edx], ebx
        add    byte ptr [eax], al
        jnp    SHORT loc_1_32AC
        add    byte ptr [eax], al
        jnp    SHORT loc_1_32B0
        add    byte ptr [eax], al
        jnp    SHORT loc_1_32B4
        add    byte ptr [eax], al
        db  0A0h, 01Ah, 000h, 000h, 07Bh
        sbb    al, byte ptr [eax]
        db  000h, 07Bh, 01Ah
        add    byte ptr [eax], al
        jnp    SHORT loc_1_32C4
        add    byte ptr [eax], al
loc_1_32AC:
        mov    bh, 1ah
        add    byte ptr [eax], al
loc_1_32B0:
        jnp    SHORT loc_1_32CC
        add    byte ptr [eax], al
loc_1_32B4:
        db  07Bh, 01Ah
        add    byte ptr [eax], al
        jnp    SHORT loc_1_32D4
        add    byte ptr [eax], al
        rcr    byte ptr [edx], 1
        add    byte ptr [eax], al
        jnp    SHORT loc_1_32DC
        add    byte ptr [eax], al
loc_1_32C4:
        jnp    SHORT loc_1_32E0
        add    byte ptr [eax], al
        db  07Bh, 01Ah
        add    byte ptr [eax], al
loc_1_32CC:
        rcr    dword ptr [ebx], 0
        db  000h, 07Bh, 01Ah
        add    byte ptr [eax], al
loc_1_32D4:
        jnp    SHORT sub_1_32F0
        add    byte ptr [eax], al
        jnp    SHORT loc_1_32F4
        add    byte ptr [eax], al
loc_1_32DC:
        fcomp  dword ptr [ebx]
        add    byte ptr [eax], al
loc_1_32E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [eax], 2
        ret    10h
sub_1_32F0:
        push   ebp
        mov    ebp, esp
        db  083h
loc_1_32F4:
        in     al, dx
        adc    byte ptr [ebx + 56h], dl
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 10h], esi
        je     NEAR PTR loc_1_33BA
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0780
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_1_33BA
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_07A4
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_33B5
        mov    edi, dword ptr [ebp + 0ch]
        mov    edi, edi
loc_1_3340:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_336C
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_07C8
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_33B5
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_07A4
        jmp    SHORT loc_1_33B0
loc_1_336C:
        or     edi, edi
        je     SHORT loc_1_3389
        cmp    edi, dword ptr [ebp - 4]
        jbe    SHORT loc_1_3381
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_33B0
loc_1_3381:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_3389:
        mov    ebx, dword ptr [ebp + 10h]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_3397
        mov    ebx, eax
loc_1_3397:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        add    esi, ebx
        push   eax
        call   sub_1_083C
        add    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_33B0:
        cmp    dword ptr [ebp + 10h], esi
        ja     SHORT loc_1_3340
loc_1_33B5:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_33BA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_33D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0ch], esi
        je     NEAR PTR loc_1_349B
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_0780
        cmp    dword ptr [ebp - 14h], esi
        je     NEAR PTR loc_1_349B
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_07A4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp + 0ch], esi
        je     SHORT loc_1_3496
        mov    edi, dword ptr [ebp + 14h]
loc_1_3424:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_3450
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_07C8
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_3496
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_07A4
        jmp    SHORT loc_1_3491
loc_1_3450:
        or     edi, edi
        je     SHORT loc_1_346D
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_3465
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_3491
loc_1_3465:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_346D:
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, esi
        mov    ebx, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_347B
        mov    ebx, eax
loc_1_347B:
        push   ebx
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    esi, ebx
        push   eax
        call   sub_1_083C
        add    dword ptr [ebp - 10h], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_3491:
        cmp    esi, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_3424
loc_1_3496:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_349B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_34B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        inc    dword ptr [edi + 0eh]
        cmp    byte ptr [esi + 1ch], 0
        jne    SHORT loc_1_3510
        inc    dword ptr [esi + 18h]
        mov    ecx, dword ptr [ebp + 0ch]
        or     ecx, ecx
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        jne    SHORT loc_1_34F3
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 0eh
        push   eax
        mov    eax, dword ptr [edi + 52h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_07CE
        dec    dword ptr [esi + 18h]
        sub    esi, esi
        jmp    SHORT loc_1_3515
loc_1_34F3:
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, ecx
        push   eax
        call   sub_1_33D0
        dec    dword ptr [esi + 18h]
        sub    esi, esi
        jmp    SHORT loc_1_3515
        db  090h
loc_1_3510:
        mov    esi, 0c001000ch
loc_1_3515:
        dec    dword ptr [edi + 0eh]
        push   edi
        call   sub_1_3850
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3530:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        dec    dword ptr [esi + 3ah]
        cmp    dword ptr [esi + 42h], 0
        je     NEAR PTR loc_1_3618
loc_1_3549:
        mov    edi, dword ptr [esi + 42h]
        mov    eax, dword ptr [edi + 24h]
        lea    ebx, [edi + 24h]
        mov    dword ptr [esi + 42h], eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        push   0
        push   edi
        call   sub_1_0780
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_07A4
        mov    eax, dword ptr [esi + 0a9h]
        mov    ecx, dword ptr [ebp - 4]
        cmp    eax, ecx
        jb     SHORT loc_1_3587
        mov    eax, ecx
loc_1_3587:
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 5eah
        jae    SHORT loc_1_35B8
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 4], eax
        je     SHORT loc_1_35B8
        lea    eax, [ebp - 4]
        push   eax
        db  0A1h, 0B4h, 098h, 001h, 000h
        push   eax
        push   5eah
        push   0
        push   edi
        call   sub_1_32F0
        db  0A1h, 0B4h, 098h, 001h, 000h
        mov    dword ptr [ebp - 8], eax
loc_1_35B8:
        mov    dword ptr [esi + 52h], edi
        mov    eax, dword ptr [ebp - 10h]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 4]
        sub    eax, 0eh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0eh
        push   eax
        push   0eh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   eax
        push   0
        mov    eax, dword ptr [esi + 3eh]
        push   eax
        call   sub_1_0300
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    al, byte ptr [ebx + 8]
        test   al, 1
        je     SHORT loc_1_3608
        and    al, 2
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   edi
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_069A
loc_1_3608:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 18h]
        cmp    dword ptr [esi + 42h], 0
        jne    NEAR PTR loc_1_3549
loc_1_3618:
        mov    eax, dword ptr [esi + 3eh]
        push   eax
        call   sub_1_04E0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3630:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [eax + 24h]
        cmp    dword ptr [edx + 42h], 0
        jne    SHORT loc_1_3649
        mov    dword ptr [edx + 42h], eax
        jmp    SHORT loc_1_364F
loc_1_3649:
        mov    ecx, dword ptr [edx + 46h]
        mov    dword ptr [ecx + 24h], eax
loc_1_364F:
        mov    dword ptr [esi], 0
        mov    dword ptr [edx + 46h], eax
        mov    cl, byte ptr [esi + 8]
        mov    bl, byte ptr [ebp + 10h]
        mov    al, bl
        xor    al, cl
        and    al, 1
        xor    al, cl
        mov    byte ptr [esi + 8], al
        or     bl, bl
        je     SHORT loc_1_3672
        or     al, 2
        mov    byte ptr [esi + 8], al
loc_1_3672:
        mov    eax, dword ptr [esi + 4]
        inc    dword ptr [eax + 18h]
        mov    eax, dword ptr [edx + 3ah]
        or     eax, eax
        jne    SHORT loc_1_368E
        inc    eax
        mov    dword ptr [edx + 3ah], eax
        push   1
        add    edx, 2eh
        push   edx
        call   sub_1_0732
loc_1_368E:
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_36A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    eax, [esi + 103h]
        mov    dword ptr [ebp - 18h], eax
        mov    edx, eax
        add    edx, 40h
        mov    dword ptr [ebp - 8], edx
        mov    byte ptr [eax + 54h], 0
        cmp    dword ptr [edx], edx
        je     NEAR PTR loc_1_3800
        add    eax, 96h
        mov    dword ptr [ebp - 10h], eax
loc_1_36D1:
        mov    edx, dword ptr [ebp - 8]
        mov    edi, dword ptr [edx]
        mov    eax, edi
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        mov    ecx, dword ptr [edi + 14h]
        cmp    byte ptr [ecx + 30h], 0
        je     SHORT loc_1_374B
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [ecx + 28h], eax
        cmp    dword ptr [esi + 42h], 0
        jne    SHORT loc_1_3700
        mov    dword ptr [esi + 42h], ecx
        jmp    SHORT loc_1_3706
        db  005h, 000h, 000h, 000h, 000h
loc_1_3700:
        mov    eax, dword ptr [esi + 46h]
        mov    dword ptr [eax + 24h], ecx
loc_1_3706:
        mov    dword ptr [ecx + 24h], 0
        mov    dword ptr [esi + 46h], ecx
        mov    al, byte ptr [ecx + 2ch]
        or     al, 1
        mov    byte ptr [ecx + 2ch], al
        or     al, 2
        mov    byte ptr [ecx + 2ch], al
        mov    eax, dword ptr [ecx + 28h]
        inc    dword ptr [eax + 18h]
        mov    eax, dword ptr [esi + 3ah]
        or     eax, eax
        je     SHORT loc_1_3739
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 2eh]
        push   eax
        call   sub_1_0738
        jmp    SHORT loc_1_373D
loc_1_3739:
        inc    eax
        mov    dword ptr [esi + 3ah], eax
loc_1_373D:
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_3530
        jmp    SHORT loc_1_3757
loc_1_374B:
        push   0
        push   ecx
        mov    eax, dword ptr [edi + 2bh]
        push   eax
        call   sub_1_069A
loc_1_3757:
        mov    ebx, dword ptr [edi + 1ch]
        add    ebx, dword ptr [edi + 20h]
        push   6
        lea    eax, [ebx + 6]
        push   eax
        push   ebx
        call   sub_1_083C
        push   6
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebx + 6]
        push   eax
        call   sub_1_083C
        mov    byte ptr [ebx + 15h], 2
        push   4
        lea    eax, [ebx + 26h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_083C
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 183h], ecx
        je     SHORT loc_1_37EF
        push   0ah
        lea    eax, [ebx + 16h]
        push   eax
        lea    eax, [ebx + 20h]
        push   eax
        call   sub_1_083C
        push   4
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebx + 1ch]
        push   eax
        call   sub_1_083C
        push   6
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebx + 16h]
        push   eax
        call   sub_1_083C
        lea    eax, [ebx + 0eh]
        mov    ecx, dword ptr [edi + 24h]
        push   ecx
        push   ecx
        push   eax
        push   0eh
        push   ebx
        push   eax
        mov    eax, dword ptr [edi + 2bh]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_06B2
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_1_37EF
        mov    eax, dword ptr [edi + 2bh]
        push   eax
        call   sub_1_06B8
loc_1_37EF:
        push   edi
        call   sub_1_0080
        mov    edx, dword ptr [ebp - 8]
        cmp    dword ptr [edx], edx
        jne    NEAR PTR loc_1_36D1
loc_1_3800:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3810:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 10h]
        lea    eax, [edx + 40h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 44h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 44h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx + 44h], ecx
        cmp    byte ptr [edx + 54h], 0
        jne    SHORT loc_1_3841
        mov    byte ptr [edx + 54h], 1
        push   1
        add    edx, 48h
        push   edx
        call   sub_1_0732
loc_1_3841:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3850:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 3ah], 0
        je     SHORT loc_1_3884
        cmp    byte ptr [esi + 92h], 0
        je     SHORT loc_1_3884
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 2eh]
        push   eax
        call   sub_1_0738
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_3530
loc_1_3884:
        mov    byte ptr [esi + 92h], 0
        mov    byte ptr [esi + 93h], 0
        mov    edi, dword ptr [esi + 95h]
        mov    eax, dword ptr [esi + 9dh]
        or     edi, edi
        mov    ecx, dword ptr [esi + 99h]
        je     SHORT loc_1_3910
        cmp    eax, 4
        je     SHORT loc_1_3909
        or     ecx, ecx
        je     SHORT loc_1_38BB
        push   1
        push   esi
        db  0E8h, 077h, 0DEh, 0FDh, 0FFh
        jmp    SHORT loc_1_3910
loc_1_38BB:
        cmp    eax, 8
        jne    SHORT loc_1_3910
        mov    ebx, dword ptr [esi + 12h]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -12h
        je     SHORT loc_1_38FC
        lea    esp, [esp]
loc_1_38D0:
        inc    dword ptr [ebx + 18h]
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_06BE
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_06C4
        dec    dword ptr [ebx + 18h]
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -12h
        jne    SHORT loc_1_38D0
loc_1_38FC:
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_06A6
        jmp    SHORT loc_1_390D
loc_1_3909:
        or     edi, edi
        je     SHORT loc_1_3910
loc_1_390D:
        dec    dword ptr [edi + 18h]
loc_1_3910:
        mov    ecx, dword ptr [esi + 0a1h]
        or     ecx, ecx
        je     SHORT loc_1_394E
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0a1h], eax
        cmp    dword ptr [esi + 0a5h], ecx
        jne    SHORT loc_1_3934
        mov    dword ptr [esi + 0a5h], 0
loc_1_3934:
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, 4
        jne    SHORT loc_1_3942
        push   4
        push   0
        jmp    SHORT loc_1_3944
loc_1_3942:
        push   eax
        push   ecx
loc_1_3944:
        mov    eax, dword ptr [ecx + 4]
        push   eax
        push   esi
        db  0E8h, 0A2h, 0DDh, 0FDh, 0FFh
loc_1_394E:
        mov    eax, dword ptr [esi + 1ah]
        lea    edi, [esi + 1ah]
        sub    eax, esi
        cmp    eax, 1ah
        je     SHORT loc_1_3997
        add    eax, 0
loc_1_3960:
        mov    ebx, dword ptr [edi]
        cmp    dword ptr [ebx + 18h], 0
        jne    SHORT loc_1_398C
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_0694
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   5ah
        push   ebx
        call   sub_1_075C
loc_1_398C:
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [edi]
        sub    eax, esi
        cmp    eax, 1ah
        jne    SHORT loc_1_3960
loc_1_3997:
        dec    dword ptr [esi + 0eh]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_39B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 00Bh, 000h, 02Bh, 000h
        add    esp, 0ch
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_39E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 00Fh, 000h, 02Bh, 000h
        add    esp, 8
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        xor    eax, eax
        push   eax
        push   eax
        mov    eax, 15h
        push   eax
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 00Eh, 000h, 02Bh, 000h
        add    esp, 10h
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_3A40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 012h, 000h, 02Bh, 000h
        add    esp, 10h
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 013h, 000h, 02Bh, 000h
        add    esp, 10h
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3AA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 017h, 000h, 02Bh, 000h
        add    esp, 10h
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 018h, 000h, 02Bh, 000h
        add    esp, 10h
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3B00:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   0
        push   -1
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3AA0
        push   0
        push   0
        push   -1
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3AD0
        push   0
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_39E0
        push   1
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_39E0
        pop    esi
        pop    ebp
        ret    4
sub_1_3B40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 006h, 000h, 02Bh, 000h
        add    esp, 8
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 005h, 000h, 02Bh, 000h
        add    esp, 0ch
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3BA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 011h, 000h, 02Bh, 000h
        add    esp, 0ch
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3BD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 010h, 000h, 02Bh, 000h
        add    esp, 0ch
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3C00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 014h, 000h, 02Bh, 000h
        add    esp, 0ch
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 30h
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3B40
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [ebp - 13h], al
        push   800h
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3B70
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3C70:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 103h
        sti
        cld
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi + 8]
        inc    dword ptr [ebx + 0eh]
        cmp    byte ptr [ebx + 92h], 0
        jne    NEAR PTR loc_1_3D70
        cmp    byte ptr [esi + 1ch], 0
        jne    NEAR PTR loc_1_3D60
        inc    dword ptr [esi + 18h]
        lea    eax, [ebp - 8]
        push   eax
        push   0
        push   0
        push   0
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        call   sub_1_0780
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_3D51
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 0eh
        cmp    dword ptr [ebx + 0c3h], eax
        jb     NEAR PTR loc_1_3D51
        push   edi
        push   esi
        call   sub_1_3DC0
        test   al, 6
        jne    SHORT loc_1_3CEA
        mov    ecx, eax
        or     ecx, 1
        cmp    ecx, eax
        mov    cl, 0
        je     SHORT loc_1_3CEC
loc_1_3CEA:
        mov    cl, 1
loc_1_3CEC:
        mov    dword ptr [ebp - 4], 0
        and    al, 9
        cmp    al, 9
        jne    SHORT loc_1_3D2C
        lea    edx, [ebx + 103h]
        mov    dword ptr [edi + 2ch], esi
        lea    eax, [edi + 24h]
        or     cl, cl
        jne    SHORT loc_1_3D16
        cmp    byte ptr [ebx + 94h], 0
        mov    byte ptr [eax + 0ch], 0
        je     SHORT loc_1_3D1A
loc_1_3D16:
        mov    byte ptr [eax + 0ch], 1
loc_1_3D1A:
        push   edi
        push   edx
        call   sub_1_3EA0
        mov    dword ptr [ebp - 4], eax
        dec    dword ptr [esi + 18h]
        jmp    NEAR PTR loc_1_3DA7
loc_1_3D2C:
        or     cl, cl
        jne    SHORT loc_1_3D39
        cmp    byte ptr [ebx + 94h], 0
        je     SHORT loc_1_3D58
loc_1_3D39:
        mov    dword ptr [edi + 28h], esi
        push   1
        push   edi
        push   ebx
        call   sub_1_3630
        mov    dword ptr [ebp - 4], 103h
        dec    dword ptr [esi + 18h]
        jmp    SHORT loc_1_3DA7
loc_1_3D51:
        mov    dword ptr [ebp - 4], 0c000009ah
loc_1_3D58:
        dec    dword ptr [esi + 18h]
        jmp    SHORT loc_1_3DA7
        db  08Dh, 049h, 000h
loc_1_3D60:
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_1_3DA7
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3D70:
        cmp    dword ptr [ebx + 9dh], 8
        jne    SHORT loc_1_3D82
        mov    dword ptr [ebp - 4], 0c001000dh
        jmp    SHORT loc_1_3DA7
loc_1_3D82:
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        push   esi
        call   sub_1_3DC0
        cmp    eax, 1
        je     SHORT loc_1_3DA0
        mov    dword ptr [edi + 28h], esi
        push   1
        push   edi
        push   ebx
        call   sub_1_3630
        jmp    SHORT loc_1_3DA7
        db  090h
loc_1_3DA0:
        mov    dword ptr [ebp - 4], 0
loc_1_3DA7:
        dec    dword ptr [ebx + 0eh]
        push   ebx
        call   sub_1_3850
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_3DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   esi
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   6
        push   0
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_32F0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        push   6
        push   6
        push   esi
        call   sub_1_32F0
        mov    eax, dword ptr [ebp - 16h]
        cmp    dword ptr [ebp - 0eh], eax
        jae    SHORT loc_1_3E06
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_3E48
loc_1_3E06:
        mov    eax, dword ptr [ebp - 16h]
        cmp    dword ptr [ebp - 0eh], eax
        jbe    SHORT loc_1_3E17
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_1_3E48
loc_1_3E17:
        mov    ax, word ptr [ebp - 18h]
        cmp    word ptr [ebp - 10h], ax
        jae    SHORT loc_1_3E30
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_3E48
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3E30:
        mov    ax, word ptr [ebp - 18h]
        cmp    word ptr [ebp - 10h], ax
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_1_3E48
        mov    dword ptr [ebp - 4], 0
loc_1_3E48:
        mov    cl, byte ptr [ebp - 10h]
        and    ecx, 1
        cmp    dword ptr [ebp - 0eh], -1
        jne    SHORT loc_1_3E65
        cmp    byte ptr [ebp - 10h], 0ffh
        jne    SHORT loc_1_3E65
        cmp    byte ptr [ebp - 0fh], 0ffh
        mov    eax, 1
        je     SHORT loc_1_3E67
loc_1_3E65:
        sub    eax, eax
loc_1_3E67:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_3E74
        mov    dword ptr [ebp - 4], 1
loc_1_3E74:
        or     ecx, ecx
        je     SHORT loc_1_3E7C
        or     dword ptr [ebp - 4], 2
loc_1_3E7C:
        or     eax, eax
        je     SHORT loc_1_3E84
        or     dword ptr [ebp - 4], 4
loc_1_3E84:
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 49h], 0
        je     SHORT loc_1_3E91
        or     dword ptr [ebp - 4], 8
loc_1_3E91:
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_1_3EA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        lea    ebx, [esi + 24h]
        or     edi, edi
        jne    SHORT loc_1_3ED0
        cmp    dword ptr [ebx], ebx
        jne    SHORT loc_1_3ED0
        push   esi
        call   sub_1_43C0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_3ED0:
        mov    eax, dword ptr [esi + 20h]
        mov    eax, dword ptr [eax + 0bbh]
        mov    eax, dword ptr [eax + 8]
        sub    eax, 2
        cmp    eax, dword ptr [esi + 3ch]
        jbe    NEAR PTR loc_1_4330
        push   0
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   0
        or     eax, eax
        jne    NEAR PTR loc_1_4330
        cmp    dword ptr [ebx], ebx
        je     SHORT loc_1_3F2E
        or     edi, edi
        je     SHORT loc_1_3F1B
        mov    dword ptr [edi + 24h], ebx
        add    edi, 24h
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 28h], edi
        inc    dword ptr [esi + 2ch]
loc_1_3F1B:
        mov    edi, dword ptr [ebx]
        mov    eax, edi
        sub    edi, 24h
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], ebx
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx], eax
        dec    dword ptr [esi + 2ch]
loc_1_3F2E:
        mov    eax, dword ptr [edi + 2ch]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 47h]
        mov    word ptr [ecx + 28h], ax
        mov    eax, dword ptr [ebp - 4]
        cmp    word ptr [eax + 28h], 2100h
        jne    SHORT loc_1_3FC0
        add    eax, 24h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        push   2
        push   0ch
        push   edi
        call   sub_1_32F0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 1ch]
        cmp    word ptr [ecx], 608h
        jne    SHORT loc_1_3FC0
        add    eax, 24h
        push   eax
        push   ecx
        push   1770h
        push   0
        push   edi
        call   sub_1_32F0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], edi
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ecx + 2bh], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_3810
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_3FC0:
        mov    eax, dword ptr [esi + 0ech]
        cmp    eax, 3
        je     NEAR PTR loc_1_4240
        cmp    eax, 4
        je     NEAR PTR loc_1_426A
        cmp    eax, 5
        mov    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_42AB
        cmp    word ptr [eax + 28h], 3f00h
        jne    SHORT loc_1_4030
        cmp    dword ptr [esi + 59h], 0
        je     SHORT loc_1_4030
        mov    ecx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 1ch]
        lea    ebx, [ecx + eax + 0ch]
        lea    eax, [ebp - 0ch]
        push   eax
        push   ebx
        push   0ch
        push   0
        push   edi
        call   sub_1_32F0
        add    ebx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 24h
        push   eax
        push   ebx
        push   1770h
        push   0eh
        push   edi
        call   sub_1_32F0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ecx + 24h], eax
        jmp    SHORT loc_1_404E
        db  08Bh, 0FFh
loc_1_4030:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 24h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1ch]
        add    eax, 0ch
        push   eax
        push   1770h
        push   0eh
        push   edi
        call   sub_1_32F0
loc_1_404E:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], 0ch
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 56h]
        or     ecx, ecx
        je     SHORT loc_1_4072
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4eh]
        push   eax
        call   ecx
        add    esp, 8
loc_1_4072:
        cmp    dword ptr [esi + 120h], 0
        je     NEAR PTR loc_1_42FB
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax + 28h], 0
        jne    NEAR PTR loc_1_42FB
        sub    dword ptr [eax + 20h], 2
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [eax + 24h], 2
        mov    eax, dword ptr [ebp - 4]
        mov    cx, word ptr [eax + 28h]
        mov    edx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 1ch]
        mov    word ptr [edx + eax], cx
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 28h], 0fd00h
        test   byte ptr [esi + 120h], 1
        je     SHORT loc_1_40F3
        cmp    dword ptr [esi + 128h], 0
        je     SHORT loc_1_40D0
        mov    eax, dword ptr [esi + 118h]
        push   eax
        call   dword ptr [esi + 139h]
loc_1_40D0:
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + 20h]
        add    ebx, dword ptr [eax + 1ch]
        mov    eax, dword ptr [esi + 118h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 24h
        push   eax
        push   ebx
        call   dword ptr [esi + 13dh]
        mov    byte ptr [ebx - 2], al
        jmp    SHORT loc_1_4101
loc_1_40F3:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 1ch]
        mov    byte ptr [ecx + eax - 2], 0
loc_1_4101:
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 20h], 2
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [eax + 24h], 2
        mov    eax, dword ptr [ebp - 4]
        mov    cx, word ptr [esi + 114h]
        shr    cx, 8
        mov    edx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 1ch]
        and    cl, 0fh
        or     byte ptr [edx + eax], cl
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [esi + 114h]
        mov    edx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 1ch]
        mov    byte ptr [edx + eax + 1], cl
        inc    word ptr [esi + 114h]
        test   byte ptr [esi + 120h], 10h
        je     NEAR PTR loc_1_4215
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 1ch]
        test   byte ptr [ecx + eax], 80h
        je     SHORT loc_1_4174
        lea    eax, [esi + 159h]
        push   eax
        push   8
        mov    eax, dword ptr [esi + 151h]
        push   eax
        call   sub_1_17730
loc_1_4174:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 1ch]
        or     byte ptr [ecx + eax], 10h
        cmp    byte ptr [esi + 114h], 0
        jne    SHORT loc_1_41F0
        inc    byte ptr [esi + 15ch]
        add    byte ptr [esi + 15dh], 3
        add    byte ptr [esi + 15eh], 0dh
        add    byte ptr [esi + 15fh], 39h
        lea    ebx, [esi + 159h]
        add    byte ptr [esi + 160h], 13h
        push   ebx
        push   8
        mov    eax, dword ptr [esi + 151h]
        push   eax
        call   sub_1_17730
        push   ebx
        push   8
        mov    eax, dword ptr [esi + 151h]
        push   eax
        call   sub_1_177C0
        mov    byte ptr [ebx], 0d1h
        mov    byte ptr [esi + 15ah], 26h
        mov    byte ptr [esi + 15bh], 9eh
        push   ebx
        push   8
        mov    eax, dword ptr [esi + 151h]
        push   eax
        call   sub_1_17730
loc_1_41F0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 20h]
        add    eax, dword ptr [ecx + 1ch]
        add    eax, 2
        push   eax
        mov    eax, ecx
        mov    eax, dword ptr [eax + 24h]
        sub    eax, 2
        push   eax
        mov    eax, dword ptr [esi + 151h]
        push   eax
        call   sub_1_177C0
loc_1_4215:
        cmp    dword ptr [esi + 128h], 0
        je     NEAR PTR loc_1_42FB
        mov    dword ptr [esi + 128h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 20h]
        mov    eax, dword ptr [eax + 1ch]
        or     byte ptr [ecx + eax], 80h
        jmp    NEAR PTR loc_1_42FB
        db  08Bh, 0FFh
loc_1_4240:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 24h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        push   1770h
        push   0
        push   edi
        call   sub_1_32F0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], 0
        jmp    NEAR PTR loc_1_42FB
loc_1_426A:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 24h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        push   1770h
        push   0eh
        push   edi
        call   sub_1_32F0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 56h]
        or     ecx, ecx
        je     SHORT loc_1_42FB
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4eh]
        push   eax
        call   ecx
        add    esp, 8
        jmp    SHORT loc_1_42FB
loc_1_42AB:
        add    eax, 24h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 1ch]
        add    eax, 0ch
        push   eax
        push   1770h
        push   0eh
        push   edi
        call   sub_1_32F0
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        push   6
        push   0
        push   edi
        call   sub_1_32F0
        cmp    dword ptr [ebp - 16h], -1
        jne    SHORT loc_1_42EF
        cmp    byte ptr [ebp - 18h], 0ffh
        jne    SHORT loc_1_42EF
        cmp    byte ptr [ebp - 17h], 0ffh
        mov    eax, 1
        je     SHORT loc_1_42F1
loc_1_42EF:
        sub    eax, eax
loc_1_42F1:
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_11A20
loc_1_42FB:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], edi
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ecx + 2bh], eax
        mov    eax, dword ptr [ebp - 4]
        push   1
        mov    byte ptr [eax + 2ah], 1
        inc    dword ptr [esi + 3ch]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_4360
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4330:
        push   esi
        call   sub_1_43C0
        or     edi, edi
        je     SHORT loc_1_4351
        mov    dword ptr [edi + 24h], ebx
        add    edi, 24h
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 28h]
        mov    dword ptr [eax], edi
        mov    dword ptr [esi + 28h], edi
        inc    dword ptr [esi + 2ch]
loc_1_4351:
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_4360:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        push   esi
        call   dword ptr [edi + 0f0h]
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_4390
        lea    eax, [edi + 30h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 34h]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 34h]
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 34h], esi
        jmp    SHORT loc_1_43A2
        db  08Dh, 049h, 000h
loc_1_4390:
        mov    eax, dword ptr [edi + 30h]
        lea    ecx, [edi + 30h]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], ecx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [ecx], esi
loc_1_43A2:
        inc    dword ptr [edi + 38h]
        push   edi
        call   sub_1_43C0
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_43C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 30h]
        lea    edi, [esi + 30h]
        cmp    edx, edi
        jne    SHORT loc_1_43DA
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_43DA:
        cmp    dword ptr [esi + 55h], 0
        je     SHORT loc_1_43EB
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_43EB:
        mov    eax, edx
        push   edx
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], edi
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
        dec    dword ptr [esi + 38h]
        mov    dword ptr [esi + 55h], edx
        call   sub_1_4550
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_4410:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    dword ptr [ebp - 4], edx
        cld
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [eax + 10h]
        mov    dword ptr [esi + 80h], 0
        inc    dword ptr [esi + 0a4h]
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 24h]
        add    dword ptr [esi + 9ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 55h], 0
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax + 2ah], 0
        je     SHORT loc_1_445D
        dec    dword ptr [esi + 3ch]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2ah], 0
loc_1_445D:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 2bh]
        or     eax, eax
        je     SHORT loc_1_448D
        mov    edx, dword ptr [ebp - 4]
        mov    edx, dword ptr [edx + 14h]
        cmp    byte ptr [edx + 30h], 0
        je     SHORT loc_1_4484
        mov    eax, dword ptr [edx + 2ch]
        mov    dword ptr [edx + 28h], eax
        push   1
        push   edx
        push   ecx
        call   sub_1_3630
        jmp    SHORT loc_1_448D
loc_1_4484:
        push   0
        push   edx
        push   eax
        call   sub_1_069A
loc_1_448D:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0080
        push   0
        push   esi
        call   sub_1_3EA0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_44B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 55h]
        or     esi, esi
        je     SHORT loc_1_451B
        inc    dword ptr [eax + 0a4h]
        mov    ecx, dword ptr [esi + 24h]
        add    dword ptr [eax + 9ch], ecx
        mov    edx, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 55h], 0
        cmp    byte ptr [esi + 2ah], 0
        je     SHORT loc_1_44E4
        dec    dword ptr [eax + 3ch]
        mov    byte ptr [esi + 2ah], 0
loc_1_44E4:
        mov    eax, dword ptr [esi + 2bh]
        or     eax, eax
        je     SHORT loc_1_4515
        mov    ecx, dword ptr [esi + 14h]
        cmp    byte ptr [ecx + 30h], 0
        je     SHORT loc_1_450C
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [ecx + 28h], eax
        push   1
        push   ecx
        push   edx
        call   sub_1_3630
        push   esi
        call   sub_1_0080
        pop    esi
        pop    ebp
        ret
loc_1_450C:
        push   0
        push   ecx
        push   eax
        call   sub_1_069A
loc_1_4515:
        push   esi
        call   sub_1_0080
loc_1_451B:
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_1_4520:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    edx, dword ptr [ebp + 0ch]
        db  0BEh, 010h, 044h, 000h, 000h
        call   sub_1_0185
        mov    dword ptr [ebp - 4], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [eax + 80h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_1_4550:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        cmp    dword ptr [edi], 2
        jne    SHORT loc_1_4580
        push   0
        push   2
        push   esi
        push   0
        call   sub_1_4520
        add    esp, 10h
loc_1_4572:
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_4580:
        push   esi
        db  068h, 020h, 045h, 000h, 000h
        push   1
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3AD0
        or     al, al
        jne    SHORT loc_1_45B1
        push   0
        push   2
        push   esi
        push   0
        call   sub_1_4520
        add    esp, 10h
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_45B1:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        add    eax, dword ptr [esi + 20h]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_3A40
        or     al, al
        je     SHORT loc_1_45E5
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_45E5
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        je     SHORT loc_1_4572
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_45E5:
        push   0
        push   2
        push   esi
        push   0
        call   sub_1_4520
        add    esp, 10h
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_4600:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4610:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], edx
        cld
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 84h], 0
loc_1_4627:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 7fh], 0
        sti
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   dword ptr [eax + 0f4h]
        cli
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax + 7fh], 0
        jne    SHORT loc_1_4627
        mov    byte ptr [eax + 7eh], 0
        sti
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
loc_1_4650:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        cmp    byte ptr [edi + 7eh], 0
        je     SHORT loc_1_4670
        mov    byte ptr [edi + 7fh], 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_1_4670:
        mov    byte ptr [edi + 7eh], 1
        mov    edx, dword ptr [ebp + 0ch]
        db  0BEh, 010h, 046h, 000h, 000h
        call   sub_1_0185
        mov    dword ptr [ebp - 4], esi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 84h], eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_46A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp + 0ch]
        mov    byte ptr [eax + 0f8h], cl
        and    ecx, 0ffh
        mov    byte ptr [eax + 0f9h], 1
        push   ecx
        push   eax
        call   sub_1_3C30
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_46D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 65h]
        cmp    dword ptr [esi + 69h], eax
        je     SHORT loc_1_46F8
        push   esi
        db  068h, 050h, 046h, 000h, 000h
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3AA0
        mov    eax, dword ptr [esi + 65h]
        mov    dword ptr [esi + 69h], eax
loc_1_46F8:
        cmp    byte ptr [esi + 0f9h], 0
        lea    eax, [ebp - 4]
        push   eax
        push   2
        mov    eax, dword ptr [esi + 4]
        push   eax
        je     SHORT loc_1_4740
        call   sub_1_3BA0
        lea    eax, [esi + 0fah]
        push   eax
        push   2
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3BD0
        push   esi
        db  068h, 050h, 046h, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_1_3C00
        mov    byte ptr [esi + 0f9h], 0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_4740:
        call   sub_1_3BA0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_4750:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 61h]
        mov    ecx, dword ptr [esi + 20h]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax + 10h], esi
        mov    dword ptr [esi + 75h], 8eh
        mov    dword ptr [esi + 71h], 8eh
        mov    byte ptr [esi + 7bh], 0
        mov    byte ptr [esi + 7ah], 0
        mov    byte ptr [esi + 79h], 1
        push   7eh
        push   esi
        call   sub_1_46A0
        mov    dword ptr [esi + 6dh], 0
        push   esi
        call   sub_1_4EC0
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_47A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    ebx, dword ptr [esi + 10h]
        mov    cx, word ptr [esi + 28h]
        cmp    cx, 21c0h
        jne    SHORT loc_1_47D9
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 20h]
        mov    al, byte ptr [eax + edx]
        cmp    al, 1
        jb     SHORT loc_1_47D9
        cmp    al, 7
        ja     SHORT loc_1_47D9
        mov    dword ptr [ebp - 8], 1
loc_1_47D9:
        cmp    byte ptr [ebx + 8ah], 0
        je     SHORT loc_1_4800
        or     cl, cl
        jne    SHORT loc_1_4800
        dec    dword ptr [esi + 20h]
        inc    dword ptr [esi + 24h]
        mov    cx, word ptr [esi + 28h]
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 20h]
        mov    byte ptr [eax + edx], ch
        jmp    SHORT loc_1_481F
        db  005h, 000h, 000h, 000h, 000h
loc_1_4800:
        lea    eax, [esi + 20h]
        sub    dword ptr [esi + 20h], 2
        lea    eax, [esi + 24h]
        lea    eax, [esi + 1ch]
        add    dword ptr [esi + 24h], 2
        mov    cx, word ptr [esi + 28h]
        mov    edx, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 20h]
        mov    word ptr [edx + eax], cx
loc_1_481F:
        cmp    byte ptr [ebx + 8bh], 0
        je     SHORT loc_1_4830
        cmp    word ptr [esi + 28h], 21c0h
        jne    SHORT loc_1_484D
loc_1_4830:
        sub    dword ptr [esi + 20h], 2
        add    dword ptr [esi + 24h], 2
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 20h]
        mov    byte ptr [eax + ecx], 0ffh
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 20h]
        mov    byte ptr [eax + ecx + 1], 3
loc_1_484D:
        mov    eax, dword ptr [esi + 24h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        add    eax, dword ptr [esi + 20h]
        push   eax
        push   0ffffh
        call   sub_1_5E30
        mov    ecx, dword ptr [esi + 24h]
        add    ecx, dword ptr [esi + 1ch]
        add    ecx, dword ptr [esi + 20h]
        not    ax
        mov    byte ptr [ecx], al
        mov    byte ptr [ecx + 1], ah
        mov    byte ptr [ecx + 2], 7eh
        dec    dword ptr [esi + 20h]
        add    dword ptr [esi + 24h], 4
        mov    ecx, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 20h]
        mov    byte ptr [ecx + eax], 7eh
        mov    ax, word ptr [esi + 24h]
        mov    ecx, eax
        mov    edi, dword ptr [esi + 1ch]
        and    ecx, 0ffffh
        add    ecx, dword ptr [esi + 20h]
        lea    edx, [edi + ecx - 1]
        mov    ecx, dword ptr [esi + 0ch]
        cmp    dword ptr [ebp - 8], 0
        mov    ecx, dword ptr [ecx + 0c7h]
        lea    edi, [edi + ecx - 1]
        mov    dword ptr [ebp - 0ch], edi
        je     SHORT loc_1_48C0
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_1_48C9
        db  08Dh, 049h, 000h
loc_1_48C0:
        mov    ecx, dword ptr [ebx + 8ch]
        mov    dword ptr [ebp - 4], ecx
loc_1_48C9:
        sub    ax, 2
        mov    cl, byte ptr [edx]
        mov    byte ptr [edi], cl
        dec    edx
        dec    edi
        mov    cx, ax
        dec    ax
        or     cx, cx
        je     SHORT loc_1_4927
loc_1_48DD:
        mov    cl, byte ptr [edx]
        dec    edx
        cmp    cl, 7dh
        je     SHORT loc_1_4913
        cmp    cl, 7eh
        je     SHORT loc_1_4913
        cmp    cl, 11h
        je     SHORT loc_1_4913
        cmp    cl, 13h
        je     SHORT loc_1_4913
        cmp    cl, 91h
        je     SHORT loc_1_4913
        cmp    cl, 93h
        je     SHORT loc_1_4913
        cmp    cl, 20h
        jae    SHORT loc_1_490F
        mov    ebx, 1
        shl    ebx, cl
        test   dword ptr [ebp - 4], ebx
        jne    SHORT loc_1_4913
loc_1_490F:
        mov    byte ptr [edi], cl
        jmp    SHORT loc_1_491C
loc_1_4913:
        xor    cl, 20h
        mov    byte ptr [edi], cl
        dec    edi
        mov    byte ptr [edi], 7dh
loc_1_491C:
        dec    edi
        mov    cx, ax
        dec    ax
        or     cx, cx
        jne    SHORT loc_1_48DD
loc_1_4927:
        mov    al, byte ptr [edx]
        mov    byte ptr [edi], al
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, edi
        sub    edi, dword ptr [esi + 1ch]
        inc    ax
        and    eax, 0ffffh
        mov    dword ptr [esi + 24h], eax
        mov    dword ptr [esi + 20h], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4950:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 61h]
        mov    dword ptr [ebp - 8], ecx
        add    ecx, 24h
        mov    dword ptr [ebp - 10h], ecx
        mov    ax, word ptr [ecx]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ebp - 2], ax
        mov    ebx, dword ptr [ecx + 1ch]
        add    ebx, dword ptr [ecx + 20h]
        or     ax, ax
        mov    di, word ptr [ecx + 28h]
        mov    dword ptr [ebp - 14h], 0
        jne    SHORT loc_1_49D0
        cmp    dword ptr [esi + 124h], 0
        je     SHORT loc_1_499A
        push   esi
        call   sub_1_142E0
        add    esp, 4
loc_1_499A:
        mov    eax, dword ptr [esi + 20h]
        mov    edi, dword ptr [eax + 12h]
        sub    eax, edi
        cmp    eax, -12h
        je     NEAR PTR loc_1_4E9B
loc_1_49AB:
        mov    edx, dword ptr [edi + 52h]
        or     edx, edx
        je     SHORT loc_1_49BF
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [edi + 4eh]
        push   eax
        call   edx
        add    esp, 8
loc_1_49BF:
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [esi + 20h]
        sub    eax, edi
        cmp    eax, -12h
        jne    SHORT loc_1_49AB
        jmp    NEAR PTR loc_1_4E9B
loc_1_49D0:
        cmp    di, 0fd00h
        jne    NEAR PTR loc_1_4CD2
        cmp    dword ptr [esi + 124h], 0
        je     NEAR PTR loc_1_4EAA
        mov    al, byte ptr [ebx]
        and    eax, 0ffff00ffh
        sub    word ptr [ebp - 2], 2
        shl    ax, 8
        mov    cl, byte ptr [ebx + 1]
        and    ecx, 0ffff00ffh
        lea    edi, [eax + ecx]
        add    ebx, 2
        test   di, 8000h
        je     SHORT loc_1_4A85
        mov    ax, word ptr [esi + 116h]
        mov    cx, di
        and    cx, 0fffh
        mov    dx, ax
        and    dx, 0fffh
        cmp    dx, cx
        jbe    SHORT loc_1_4A35
        add    ax, 1000h
        mov    word ptr [esi + 116h], ax
loc_1_4A35:
        mov    ax, word ptr [esi + 116h]
        and    ax, 0f000h
        mov    word ptr [esi + 116h], ax
        or     ax, cx
        test   byte ptr [esi + 124h], 10h
        mov    word ptr [esi + 116h], ax
        je     SHORT loc_1_4A6F
        lea    eax, [esi + 161h]
        push   eax
        push   8
        mov    eax, dword ptr [esi + 155h]
        push   eax
        call   sub_1_17730
loc_1_4A6F:
        test   byte ptr [esi + 124h], 1
        je     SHORT loc_1_4A85
        mov    eax, dword ptr [esi + 11ch]
        push   eax
        call   dword ptr [esi + 149h]
loc_1_4A85:
        mov    cx, word ptr [esi + 116h]
        mov    ax, di
        xor    ax, cx
        test   ax, 0fffh
        jne    NEAR PTR loc_1_4CC0
        inc    cx
        mov    word ptr [esi + 116h], cx
        test   di, 1000h
        je     NEAR PTR loc_1_4B90
        test   byte ptr [esi + 124h], 10h
        je     NEAR PTR loc_1_4EAA
        mov    eax, ecx
        xor    edx, edx
        mov    dx, word ptr [esi + 169h]
        and    eax, 0ffffh
        sub    eax, edx
        cmp    eax, 100h
        jl     NEAR PTR loc_1_4B7C
        and    cx, 0ff00h
        mov    word ptr [esi + 169h], cx
        mov    ax, cx
        and    ah, 0f0h
        cmp    ah, 0f0h
        jne    SHORT loc_1_4B06
        and    cx, 0fffh
        and    word ptr [esi + 116h], 0fffh
        mov    word ptr [esi + 169h], cx
loc_1_4B06:
        inc    byte ptr [esi + 164h]
        add    byte ptr [esi + 165h], 3
        add    byte ptr [esi + 166h], 0dh
        add    byte ptr [esi + 167h], 39h
        lea    eax, [esi + 161h]
        add    byte ptr [esi + 168h], 13h
        push   eax
        push   8
        mov    eax, dword ptr [esi + 155h]
        push   eax
        call   sub_1_17730
        lea    eax, [esi + 161h]
        push   eax
        push   8
        mov    eax, dword ptr [esi + 155h]
        push   eax
        call   sub_1_177C0
        mov    byte ptr [esi + 161h], 0d1h
        mov    byte ptr [esi + 162h], 26h
        mov    byte ptr [esi + 163h], 9eh
        lea    eax, [esi + 161h]
        push   eax
        push   8
        mov    eax, dword ptr [esi + 155h]
        push   eax
        call   sub_1_17730
loc_1_4B7C:
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        push   eax
        mov    eax, dword ptr [esi + 155h]
        push   eax
        call   sub_1_177C0
loc_1_4B90:
        test   di, 2000h
        je     NEAR PTR loc_1_4C3C
        test   byte ptr [esi + 124h], 1
        je     NEAR PTR loc_1_4EAA
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], 1
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [esi + 12dh]
        add    eax, 0c8h
        mov    dword ptr [ecx + 1ch], eax
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx], 5eah
        mov    eax, dword ptr [esi + 11ch]
        and    di, 4000h
        push   eax
        push   ecx
        mov    ecx, dword ptr [ebp - 8]
        shr    di, 8
        mov    eax, dword ptr [ecx + 1ch]
        push   eax
        mov    eax, edi
        and    eax, 0ffffh
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        push   eax
        push   ebx
        call   dword ptr [esi + 14dh]
        or     eax, eax
        jne    SHORT loc_1_4C20
        mov    eax, dword ptr [esi + 11ch]
        push   eax
        call   dword ptr [esi + 149h]
        push   esi
        call   sub_1_142E0
        add    esp, 4
        jmp    NEAR PTR loc_1_4E9B
        db  08Dh, 064h, 024h, 000h
loc_1_4C20:
        mov    ecx, dword ptr [ebp - 10h]
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [esi + 12dh]
        mov    ecx, dword ptr [ebp - 8]
        lea    ebx, [eax + 0c8h]
        mov    dword ptr [ecx + 1ch], eax
loc_1_4C3C:
        mov    ecx, dword ptr [ebp - 8]
        mov    di, word ptr [ebx]
        sub    word ptr [ebp - 2], 2
        add    ebx, 2
        mov    word ptr [ecx + 28h], di
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 10h]
        mov    ax, word ptr [ebp - 2]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, ebx
        sub    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [ecx + 20h], eax
loc_1_4C64:
        mov    ecx, edi
        and    ecx, 0ffffh
        cmp    ecx, 2100h
        je     NEAR PTR loc_1_4D20
        cmp    ecx, 2b00h
        je     NEAR PTR loc_1_4D70
        cmp    ecx, 2d00h
        je     NEAR PTR loc_1_4D20
        cmp    ecx, 2f00h
        je     NEAR PTR loc_1_4D20
        cmp    ecx, 3f00h
        je     NEAR PTR loc_1_4DB3
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        push   eax
        push   ebx
        push   ecx
        push   esi
        call   sub_1_66A0
        jmp    NEAR PTR loc_1_4E9B
        db  08Dh, 064h, 024h, 000h
loc_1_4CC0:
        push   esi
        call   sub_1_142E0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4CD2:
        cmp    di, 0fd80h
        jne    SHORT loc_1_4C64
        cmp    byte ptr [ebx], 0eh
        jne    SHORT loc_1_4C64
        cmp    dword ptr [esi + 120h], 0
        je     NEAR PTR loc_1_4EAA
        mov    dword ptr [esi + 128h], 1
        test   byte ptr [esi + 120h], 10h
        je     NEAR PTR loc_1_4EAA
        lea    eax, [esi + 159h]
        push   eax
        push   8
        mov    eax, dword ptr [esi + 151h]
        push   eax
        call   sub_1_17730
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4D20:
        push   2100h
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_15E0
        add    esp, 8
        mov    edx, dword ptr [eax + 52h]
        or     edx, edx
        je     SHORT loc_1_4DB3
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [eax + 4eh]
        push   eax
        call   edx
        add    esp, 8
        cmp    eax, 800h
        jne    NEAR PTR loc_1_4E9B
        mov    ecx, dword ptr [ebp - 10h]
        mov    ax, word ptr [ecx]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ebp - 2], ax
        mov    ebx, dword ptr [ecx + 1ch]
        add    ebx, dword ptr [ecx + 20h]
        mov    di, word ptr [ecx + 28h]
        jmp    SHORT loc_1_4DB3
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4D70:
        push   2b00h
        mov    eax, dword ptr [esi + 20h]
        push   eax
        call   sub_1_15E0
        add    esp, 8
        mov    edx, dword ptr [eax + 52h]
        or     edx, edx
        je     SHORT loc_1_4DB3
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [eax + 4eh]
        push   eax
        call   edx
        add    esp, 8
        cmp    eax, 800h
        jne    NEAR PTR loc_1_4E9B
        mov    ecx, dword ptr [ebp - 10h]
        mov    ax, word ptr [ecx]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ebp - 2], ax
        mov    ebx, dword ptr [ecx + 1ch]
        add    ebx, dword ptr [ecx + 20h]
loc_1_4DB3:
        cmp    di, 3f00h
        jne    SHORT loc_1_4DEB
        cmp    dword ptr [esi + 5dh], 0
        je     SHORT loc_1_4DEB
        sub    ebx, 2
        push   0ch
        lea    eax, [ebx + 2]
        push   eax
        push   ebx
        call   sub_1_083C
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        sub    eax, 0ch
        shr    eax, 8
        mov    cx, word ptr [ebp - 2]
        sub    cx, 0ch
        mov    ah, cl
        mov    word ptr [ebx + 0ch], ax
        jmp    SHORT loc_1_4E3C
loc_1_4DEB:
        sub    ebx, 0eh
        mov    eax, edi
        and    eax, 0ffffh
        cmp    eax, 2100h
        push   6
        je     SHORT loc_1_4E17
        lea    eax, [esi + 90h]
        push   eax
        push   ebx
        call   sub_1_083C
        mov    ah, byte ptr [ebp - 2]
        mov    al, byte ptr [ebp - 1]
        mov    word ptr [ebx + 0ch], ax
        jmp    SHORT loc_1_4E2A
loc_1_4E17:
        mov    eax, dword ptr [esi + 20h]
        add    eax, 8
        push   eax
        push   ebx
        call   sub_1_083C
        mov    word ptr [ebx + 0ch], 8
loc_1_4E2A:
        push   6
        lea    eax, [esi + 96h]
        push   eax
        lea    eax, [ebx + 6]
        push   eax
        call   sub_1_083C
loc_1_4E3C:
        mov    eax, dword ptr [esi + 20h]
        mov    edi, dword ptr [eax + 12h]
        sub    eax, edi
        cmp    eax, -12h
        je     SHORT loc_1_4E78
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        mov    dword ptr [ebp - 0ch], eax
loc_1_4E52:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   eax
        lea    eax, [ebx + 0eh]
        push   eax
        push   0eh
        push   ebx
        push   eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_06B2
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [esi + 20h]
        sub    eax, edi
        cmp    eax, -12h
        jne    SHORT loc_1_4E52
loc_1_4E78:
        mov    eax, dword ptr [esi + 20h]
        mov    edi, dword ptr [eax + 12h]
        sub    eax, edi
        cmp    eax, -12h
        je     SHORT loc_1_4E9A
loc_1_4E85:
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_06B8
        mov    edi, dword ptr [edi]
        mov    eax, dword ptr [esi + 20h]
        sub    eax, edi
        cmp    eax, -12h
        jne    SHORT loc_1_4E85
loc_1_4E9A:
        sti
loc_1_4E9B:
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_1_4EAA
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 1ch], eax
loc_1_4EAA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
