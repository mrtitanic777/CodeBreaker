; hpfend_pcode_04.asm -- HPFEND object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0010:
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0020:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 8
        test   eax, eax
        jne    SHORT loc_4_004C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 10h]
        push   eax
        lea    edx, [ebp - 4]
        push   3
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0043:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_004C:
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        mov    eax, dword ptr [edx + 205h]
        lea    ecx, [ebp - 4]
        push   eax
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0068:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0080:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 0fh
        sub    esp, 3ch
        lea    edx, [ebp - 3ch]
        push   ebx
        push   esi
        push   edi
        lea    edi, [ebp - 3ch]
        rep stosd
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 10h]
        cmp    byte ptr [eax], cl
        je     SHORT loc_4_00B3
loc_4_00A1:
        cmp    ecx, 3ch
        jae    SHORT loc_4_00B3
        mov    bl, byte ptr [eax + ecx]
        inc    ecx
        mov    byte ptr [edx], bl
        inc    edx
        cmp    byte ptr [eax + ecx], 0
        jne    SHORT loc_4_00A1
loc_4_00B3:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp + 18h]
        push   eax
        mov    edx, dword ptr [ebp + 14h]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        mov    eax, dword ptr [ebp - 10h]
        push   edx
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 18h]
        push   ecx
        mov    eax, dword ptr [ebp - 1ch]
        push   edx
        mov    ecx, dword ptr [ebp - 20h]
        push   eax
        mov    edx, dword ptr [ebp - 24h]
        push   ecx
        mov    eax, dword ptr [ebp - 28h]
        push   edx
        mov    ecx, dword ptr [ebp - 2ch]
        push   eax
        mov    edx, dword ptr [ebp - 30h]
        push   ecx
        mov    eax, dword ptr [ebp - 34h]
        push   edx
        mov    ecx, dword ptr [ebp - 38h]
        push   eax
        mov    edx, dword ptr [ebp - 3ch]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   edx
        push   12h
        push   0aa000003h
        mov    ecx, dword ptr [eax + 205h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0111:
        add    esp, 54h
        int3
        mov    si, word ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        cmp    word ptr [eax], si
        je     SHORT loc_4_0135
        push   eax
        call   sub_4_2FD0
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax]
        mov    word ptr [ecx], si
        or     dword ptr [eax + 47h], 2
loc_4_0135:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh, 068h, 0E8h, 003h, 000h, 000h, 0E8h, 000h, 000h, 000h, 000h, 0FFh, 005h, 052h, 000h
        db  000h, 000h, 0A1h, 052h, 000h, 000h, 000h, 0C3h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h
        db  000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 05Dh, 0C2h, 004h, 000h, 005h, 000h, 000h
        db  000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    eax, dword ptr [ebp + 10h]
        push   esi
        mov    ebx, dword ptr [ebp + 14h]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 18h]
        push   ebx
        mov    edi, dword ptr [esi + 4bh]
        and    ecx, 0ffffh
        push   eax
        lea    edx, [esi + 53h]
        mov    dword ptr [esi + 63h], ebx
        mov    dword ptr [esi + 5fh], ecx
        lea    ecx, [ebp - 4]
        mov    eax, dword ptr [esi + 4fh]
        push   eax
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01A7:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_01BB
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_4_01BB:
        mov    ax, word ptr [ebp + 18h]
        dec    ax
        mov    ecx, dword ptr [esi + 53h]
        cmp    ax, 1
        sbb    al, al
        lea    edx, [ebp - 4]
        inc    al
        push   eax
        push   ebx
        push   0
        push   ecx
        push   edi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01DB:
        mov    eax, 0
        pop    edi
        cmp    dword ptr [ebp - 4], 1
        adc    eax, -1
        pop    esi
        and    eax, 0fffffffeh
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 048h, 04Bh, 051h, 0E8h, 000h, 000h, 000h, 000h, 048h, 05Dh, 0C2h
        db  008h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 5fh]
        mov    ecx, dword ptr [esi + 63h]
        dec    eax
        mov    edx, dword ptr [esi + 53h]
        cmp    eax, 1
        sbb    al, al
        inc    al
        push   eax
        push   ecx
        push   0
        mov    eax, dword ptr [esi + 4bh]
        push   edx
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_024D:
        mov    ecx, dword ptr [esi + 53h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0256:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_0260:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        in     al, dx
        mov    ebp, esp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 054h, 024h, 00Ch, 066h, 0EDh, 08Bh, 0ECh, 05Dh, 0C2h, 008h
        db  000h, 08Dh, 049h, 000h, 055h, 08Bh, 054h, 024h, 00Ch, 0EDh, 08Bh, 0ECh, 05Dh, 0C2h, 008h, 000h
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 014h, 08Bh, 0ECh, 050h, 08Bh, 04Dh, 010h, 08Bh
        db  055h, 00Ch, 051h, 052h, 0E8h, 000h, 000h, 000h, 000h, 05Dh, 0C2h, 010h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h, 055h, 08Bh, 054h, 024h, 00Ch, 08Bh, 0ECh, 08Ah, 044h, 024h, 010h, 0EEh
        db  05Dh, 0C2h, 00Ch, 000h, 055h, 08Bh, 054h, 024h, 00Ch, 066h, 08Bh, 044h, 024h, 010h, 08Bh, 0ECh
        db  066h, 0EFh, 05Dh, 0C2h, 00Ch, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h, 055h, 08Bh, 054h, 024h, 00Ch, 08Bh, 0ECh, 08Bh, 044h, 024h, 010h, 0EFh
        db  05Dh, 0C2h, 00Ch, 000h, 055h, 08Bh, 044h, 024h, 014h, 08Bh, 0ECh, 050h, 08Bh, 04Dh, 010h, 08Bh
        db  055h, 00Ch, 051h, 052h, 0E8h, 000h, 000h, 000h, 000h, 05Dh, 0C2h, 010h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h
loc_4_0310:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_3FF0
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0330:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        test   byte ptr [esi + 47h], 0bh
        jne    SHORT loc_4_0360
        push   esi
        call   sub_4_1B80
        test   al, al
        je     SHORT loc_4_0360
        mov    dword ptr [esi + 0b7h], 0
        mov    eax, dword ptr [esi]
        push   eax
        push   esi
        call   sub_4_3E40
        push   esi
        call   sub_4_1BB0
loc_4_0360:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0370:
        push   ebp
        mov    eax, 0dh
        mov    ebp, esp
        sub    esp, 1a0h
        mov    word ptr [ebp - 40h], ax
        push   ebx
        mov    word ptr [ebp - 3eh], ax
        push   esi
        push   edi
        mov    eax, 0ah
        db  0C7h, 045h, 0C4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B4h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 50h], ax
        mov    word ptr [ebp - 4eh], ax
        db  0C7h, 045h, 0A4h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 48h], ax
        db  0C7h, 045h, 0CCh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 46h], ax
        mov    word ptr [ebp - 60h], ax
        mov    word ptr [ebp - 5eh], ax
        mov    eax, 18h
        db  0C7h, 045h, 0ACh, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 38h], ax
        mov    word ptr [ebp - 36h], ax
        mov    eax, 0ch
        mov    word ptr [ebp - 58h], ax
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 56h], ax
        mov    ecx, dword ptr [edi + 4]
        mov    eax, 9
        mov    dword ptr [ebp - 1ch], ecx
        mov    word ptr [ebp - 30h], ax
        mov    edx, dword ptr [edi + 10h]
        mov    word ptr [ebp - 2eh], ax
        mov    ecx, dword ptr [edi + 14h]
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebp - 14h], edx
        xor    esi, esi
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [edi + 18h]
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        mov    dword ptr [ebp - 28h], ecx
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 24h], esi
        db  0C7h, 045h, 0D4h, 000h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0435:
        cmp    dword ptr [ebp - 4], esi
        je     SHORT loc_4_0448
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0448:
        push   1
        lea    eax, [ebp - 60h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_045F:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_046E
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 18h], ecx
loc_4_046E:
        push   1
        lea    eax, [ebp - 58h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0485:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_4_060F
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    SHORT loc_4_04EC
        push   1
        lea    eax, [ebp - 50h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04B0:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_4_060F
        mov    eax, dword ptr [ebp - 8]
        push   0
        lea    edx, [ebp - 48h]
        mov    ecx, dword ptr [eax + 4]
        push   edx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 20h], ecx
        lea    ecx, [ebp - 8]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_04DA:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_4_060F
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [eax + 4]
        jmp    SHORT loc_4_0563
loc_4_04EC:
        cmp    eax, 1
        jne    NEAR PTR loc_4_060F
        lea    eax, [ebp - 1a0h]
        lea    ecx, [ebp - 24h]
        push   eax
        lea    edx, [ebp - 4]
        push   ecx
        push   ebx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_050A:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_4_060F
        cmp    dword ptr [ebp - 1a0h], 2019f022h
        je     SHORT loc_4_053C
        cmp    dword ptr [ebp - 1a0h], 80f1f022h
        je     SHORT loc_4_053C
        cmp    dword ptr [ebp - 1a0h], 4019f022h
        jne    NEAR PTR loc_4_060F
loc_4_053C:
        mov    bl, byte ptr [ebp - 0eeh]
        and    ebx, 0fh
        test   byte ptr [ebp - 0eeh], 20h
        jne    SHORT loc_4_0555
        mov    dword ptr [ebp - 14h], 1
loc_4_0555:
        mov    eax, dword ptr [ebp - 24h]
        shl    eax, 0ch
        add    eax, 0c00h
        mov    dword ptr [ebp - 20h], eax
loc_4_0563:
        push   0
        lea    eax, [ebp - 40h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_057A:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_0589
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], ecx
loc_4_0589:
        push   1
        lea    eax, [ebp - 38h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05A0:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_05AF
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 1ch], ecx
loc_4_05AF:
        push   1
        lea    eax, [ebp - 30h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05C6:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_05D2
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 4]
loc_4_05D2:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05DB:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    edx, dword ptr [ebp - 18h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [edi + 4], ecx
        mov    ecx, dword ptr [ebp - 28h]
        mov    dword ptr [edi + 8], edx
        mov    edx, dword ptr [ebp - 10h]
        mov    dword ptr [edi + 0ch], ebx
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [edi + 14h], ecx
        xor    eax, eax
        mov    dword ptr [edi + 18h], edx
        mov    dword ptr [edi + 1ch], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_060F:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0618:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0630:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 20h
        mov    dword ptr [ebp - 20h], eax
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 14h], eax
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 10h], eax
        push   ecx
        mov    edx, dword ptr [ebp + 0ch]
        push   edx
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0ch], 0ffffffffh
        mov    dword ptr [ebp - 8], 14h
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_4_0370
        test   eax, eax
        je     SHORT loc_4_0678
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_0678:
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx + 209h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 225h], edx
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 221h], eax
        cmp    dword ptr [ebp - 10h], 0
        mov    dword ptr [ecx + 219h], edx
        jne    SHORT loc_4_06AE
        mov    dword ptr [ecx + 20h], 1
        jmp    SHORT loc_4_06BB
loc_4_06AE:
        cmp    dword ptr [ebp - 10h], 1
        jne    SHORT loc_4_06BB
        mov    dword ptr [ecx + 20h], 2
loc_4_06BB:
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 20dh], eax
        mov    dword ptr [ecx + 211h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 215h], eax
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_4_06E0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0dh]
        inc    dword ptr [esi]
        cmp    byte ptr [esi + 4], al
        je     SHORT loc_4_0701
        mov    eax, 0c0010002h
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0701:
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edx + 10h]
        test   ecx, ecx
        jne    SHORT loc_4_071B
        push   edx
        push   esi
        push   edi
        call   sub_4_0B40
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_071B:
        cmp    ecx, 1
        jne    SHORT loc_4_0753
        mov    ecx, dword ptr [edi + 47h]
        test   ecx, ecx
        je     SHORT loc_4_0743
        test   cl, 2
        je     SHORT loc_4_0731
        mov    eax, 0c0000001h
loc_4_0731:
        test   cl, 1
        je     SHORT loc_4_0758
        mov    eax, 0c001000dh
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0743:
        push   edx
        push   esi
        push   edi
        call   sub_4_0B90
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
loc_4_0753:
        mov    eax, 10001h
loc_4_0758:
        pop    edi
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    8
sub_4_0760:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 24h
        mov    word ptr [ebp - 2], ax
        push   ebx
        push   esi
        mov    dword ptr [ebp - 1ch], eax
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 18h], eax
        cmp    esi, 1010101h
        mov    eax, esi
        ja     SHORT loc_4_079F
        je     NEAR PTR loc_4_0998
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_4_07C0
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_079F:
        cmp    eax, 1010102h
        je     NEAR PTR loc_4_09B7
        cmp    eax, 1010103h
        je     NEAR PTR loc_4_09D6
        cmp    eax, 1010104h
        je     NEAR PTR loc_4_0A86
loc_4_07C0:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_4_0A9C
loc_4_07CC:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_4_07E1
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_4_0AA2
loc_4_07E1:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_4_0AA2
loc_4_07F0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 47h]
        test   al, 2
        je     SHORT loc_4_0803
        mov    dword ptr [ebp - 18h], 4
        jmp    SHORT loc_4_0815
loc_4_0803:
        mov    dword ptr [ebp - 18h], 2
        test   al, 1
        jne    SHORT loc_4_0815
        mov    dword ptr [ebp - 18h], 0
loc_4_0815:
        lea    eax, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_4_0AA2
loc_4_0822:
        lea    eax, [ebp - 1ch]
        mov    edi, 4
        jmp    NEAR PTR loc_4_0AA2
loc_4_082F:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_4_0AA2
loc_4_0843:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_4_0AA2
loc_4_0857:
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 186a0h
        cmp    dword ptr [edi + 0afh], 0ah
        je     SHORT loc_4_0871
        mov    dword ptr [ebp - 8], 0f4240h
loc_4_0871:
        lea    eax, [ebp - 8]
        mov    edi, 4
        jmp    NEAR PTR loc_4_0AA2
loc_4_087E:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a3h]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_0AA2
loc_4_0897:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0a7h]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_0AA2
loc_4_08B0:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_0AA2
loc_4_08C4:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0abh]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_0AA2
loc_4_08DD:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 6dh
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08F4:
        mov    byte ptr [ebp - 21h], 0
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_0AA2
loc_4_0900:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    edi, 17h
        jmp    NEAR PTR loc_4_0AA2
loc_4_090F:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_0920
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 7bh]
        mov    ecx, dword ptr [eax + 0ch]
        jmp    SHORT loc_4_0929
loc_4_0920:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 1dh]
        mov    ecx, dword ptr [eax + 8]
loc_4_0929:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_4_0AA2
loc_4_0939:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_0947
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 7fh]
        jmp    SHORT loc_4_094D
loc_4_0947:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 21h]
loc_4_094D:
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    NEAR PTR loc_4_0AA2
loc_4_095D:
        lea    eax, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_4_0AA2
loc_4_0970:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_0AA2
loc_4_0984:
        lea    eax, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 8], 7
        jmp    NEAR PTR loc_4_0AA2
loc_4_0998:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 6dh
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_09AF:
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_0AA2
loc_4_09B7:
        push   6
        mov    edi, dword ptr [ebp + 8]
        add    edi, 67h
        lea    eax, [ebp - 24h]
        push   edi
        push   eax
        mov    edi, 6
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_09CE:
        lea    eax, [ebp - 24h]
        jmp    NEAR PTR loc_4_0AA2
loc_4_09D6:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_0A2C
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 7bh]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_4_0A03
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_4_0A9C
loc_4_0A03:
        mov    eax, dword ptr [ebp + 18h]
        lea    edx, [ebp - 10h]
        push   eax
        mov    ebx, dword ptr [ebp + 1ch]
        push   edx
        lea    eax, [ebp - 0ch]
        push   ebx
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A19:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        cmp    dword ptr [ebp - 0ch], 0
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_0A9C
loc_4_0A2C:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 1dh]
        mov    ecx, dword ptr [edi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A3F:
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp + 1ch], eax
        jae    SHORT loc_4_0A5A
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_0A9C
loc_4_0A5A:
        mov    eax, dword ptr [ebp + 18h]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [ebx + 1dh]
        push   edx
        mov    ecx, dword ptr [edi + 7bh]
        push   eax
        lea    edx, [ebp - 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A77:
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_0A9C
loc_4_0A86:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 211h]
        mov    edi, 4
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        jmp    SHORT loc_4_0AA2
loc_4_0A9C:
        mov    eax, dword ptr [ebp - 14h]
        mov    edi, dword ptr [ebp - 14h]
loc_4_0AA2:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_0ADD
        cmp    esi, 1010103h
        je     SHORT loc_4_0ADD
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_4_0ACD
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    ecx, dword ptr [ebp + 20h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_4_0ACD:
        push   edi
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0AD8:
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_4_0ADD:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh
        db  064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_0B40:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 18h]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_4_0760
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010103h
        ja     SHORT loc_4_0BCD
        je     NEAR PTR loc_4_0C9F
        cmp    eax, 1010eh
        je     SHORT loc_4_0BDF
        cmp    eax, 1010fh
        je     SHORT loc_4_0C19
        cmp    eax, 10112h
        je     NEAR PTR loc_4_0C8B
loc_4_0BCD:
        mov    ebx, 0c0010017h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0CDA
loc_4_0BDF:
        cmp    edi, 4
        je     SHORT loc_4_0BF6
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0CDA
loc_4_0BF6:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0C02:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_4_0D00
        jmp    NEAR PTR loc_4_0CD8
loc_4_0C19:
        cmp    edi, 4
        je     SHORT loc_4_0C30
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_4_0CDA
loc_4_0C30:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0C3C:
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_4_0C84
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 7fh]
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_4_0C5E
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 7fh], eax
        mov    dword ptr [ecx + 21h], eax
        jmp    SHORT loc_4_0CDA
loc_4_0C5E:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    eax, dword ptr [ecx + 21h]
        jne    SHORT loc_4_0C7C
        mov    eax, dword ptr [ecx + 21h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_4_0C7C
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 21h], eax
        call   sub_4_0F50
        jmp    SHORT loc_4_0CDA
loc_4_0C7C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 21h], eax
        jmp    SHORT loc_4_0CDA
loc_4_0C84:
        mov    ebx, 0c0000001h
        jmp    SHORT loc_4_0CDA
loc_4_0C8B:
        cmp    edi, 4
        je     SHORT loc_4_0CDA
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_4_0CDA
loc_4_0C9F:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     SHORT loc_4_0CBD
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_4_0CDA
loc_4_0CBD:
        push   ecx
        mov    eax, edi
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        call   sub_4_0D60
loc_4_0CD8:
        mov    ebx, eax
loc_4_0CDA:
        test   ebx, ebx
        jne    SHORT loc_4_0CE8
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
loc_4_0CE8:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0D00:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 47h]
        test   eax, eax
        jne    SHORT loc_4_0D42
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 4], 0
        jne    SHORT loc_4_0D38
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 1dh]
        push   eax
        inc    dword ptr [esi]
        mov    ecx, dword ptr [ecx + 7bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D31:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    10h
loc_4_0D38:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
loc_4_0D42:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1000ch
        sub    eax, 3fffffffh
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0D60:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ecx + 47h]
        test   eax, eax
        jne    SHORT loc_4_0DA6
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 4], 0
        jne    SHORT loc_4_0D9C
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    edx, dword ptr [ebp + 14h]
        push   edx
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [esi + 1dh]
        push   edx
        inc    dword ptr [esi]
        mov    ecx, dword ptr [ecx + 7bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D95:
        dec    dword ptr [esi]
        pop    esi
        pop    ebp
        ret    14h
loc_4_0D9C:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
loc_4_0DA6:
        and    eax, 2
        pop    esi
        pop    ebp
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1000ch
        sub    eax, 3fffffffh
        ret    14h
        db  08Dh, 049h, 000h
sub_4_0DC0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        mov    dword ptr [ebp - 0ch], ecx
        push   esi
        mov    dword ptr [ebp - 10h], ecx
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 0ch]
        lea    ebx, [ebp - 10h]
        mov    edi, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 18h]
        push   edx
        mov    dword ptr [ebp - 14h], eax
        push   ebx
        mov    edx, dword ptr [esi + 14h]
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        mov    dword ptr [ebp - 4], ecx
        push   1
        push   edx
        push   ecx
        push   ebx
        call   sub_4_0760
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0c00000bbh
        jne    NEAR PTR loc_4_0E95
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1020101h
        ja     SHORT loc_4_0E25
        je     SHORT loc_4_0E64
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_4_0E33
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_0E25:
        cmp    eax, 1020102h
        je     SHORT loc_4_0E6C
        cmp    eax, 1020103h
        je     SHORT loc_4_0E74
loc_4_0E33:
        mov    dword ptr [ebp - 8], 0c0010017h
        jmp    SHORT loc_4_0E7D
loc_4_0E3C:
        mov    eax, dword ptr [ebx + 83h]
        jmp    SHORT loc_4_0E7A
loc_4_0E44:
        mov    eax, dword ptr [ebx + 87h]
        jmp    SHORT loc_4_0E7A
loc_4_0E4C:
        mov    eax, dword ptr [ebx + 8bh]
        jmp    SHORT loc_4_0E7A
loc_4_0E54:
        mov    eax, dword ptr [ebx + 9bh]
        jmp    SHORT loc_4_0E7A
loc_4_0E5C:
        mov    eax, dword ptr [ebx + 9fh]
        jmp    SHORT loc_4_0E7A
loc_4_0E64:
        mov    eax, dword ptr [ebx + 97h]
        jmp    SHORT loc_4_0E7A
loc_4_0E6C:
        mov    eax, dword ptr [ebx + 8fh]
        jmp    SHORT loc_4_0E7A
loc_4_0E74:
        mov    eax, dword ptr [ebx + 93h]
loc_4_0E7A:
        mov    dword ptr [ebp - 4], eax
loc_4_0E7D:
        cmp    edi, 4
        jb     SHORT loc_4_0E95
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0E91:
        add    dword ptr [ebp - 0ch], 4
loc_4_0E95:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        pop    edi
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h
loc_4_0ED0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [edx + 14h]
        cmp    ecx, 10113h
        ja     SHORT loc_4_0EF8
        je     SHORT loc_4_0F2F
        cmp    ecx, 10101h
        jb     SHORT loc_4_0F2A
        cmp    ecx, 10111h
        jbe    SHORT loc_4_0F2F
        jmp    SHORT loc_4_0F2A
loc_4_0EF8:
        cmp    ecx, 1010104h
        ja     SHORT loc_4_0F1A
        cmp    ecx, 1010101h
        jae    SHORT loc_4_0F2F
        cmp    ecx, 20101h
        jb     SHORT loc_4_0F2A
        cmp    ecx, 20105h
        jbe    SHORT loc_4_0F2F
        jmp    SHORT loc_4_0F2A
loc_4_0F1A:
        cmp    ecx, 1020101h
        jb     SHORT loc_4_0F2A
        cmp    ecx, 1020103h
        jbe    SHORT loc_4_0F2F
loc_4_0F2A:
        mov    eax, 0c0010017h
loc_4_0F2F:
        test   eax, eax
        jne    SHORT loc_4_0F3D
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_0DC0
loc_4_0F3D:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0F50:
        push   ebp
        mov    ecx, 0eeh
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 1c9h]
        test   edx, edx
        je     SHORT loc_4_0F76
loc_4_0F66:
        mov    esi, dword ptr [edx + 21h]
        cmp    ecx, esi
        jae    SHORT loc_4_0F6F
        mov    ecx, esi
loc_4_0F6F:
        mov    edx, dword ptr [edx + 11h]
        test   edx, edx
        jne    SHORT loc_4_0F66
loc_4_0F76:
        pop    esi
        mov    dword ptr [eax + 7fh], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_0F80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        lea    ecx, [ebp - 14h]
        push   esi
        lea    edx, [ebp - 10h]
        push   edi
        mov    esi, dword ptr [eax + 0dh]
        push   ecx
        lea    eax, [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   edx
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0FA7:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 4], 0
        je     SHORT loc_4_0FBE
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FBE:
        mov    eax, dword ptr [esi + 47h]
        test   eax, eax
        je     SHORT loc_4_0FE9
        test   al, 0ah
        je     SHORT loc_4_0FD7
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FD7:
        test   al, 1
        je     SHORT loc_4_0FE9
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_0FE9:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax]
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_4_1010
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_11A0
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1010:
        push   esi
        call   sub_4_1B80
        test   al, al
        jne    SHORT loc_4_103A
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_11A0
        mov    eax, 103h
        pop    edi
        mov    byte ptr [esi + 46h], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_103A:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_104B:
        mov    ecx, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3690
        test   eax, eax
        je     SHORT loc_4_1099
        jle    SHORT loc_4_1087
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_11A0
        push   esi
        call   sub_4_1BB0
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1087:
        test   eax, eax
        jge    SHORT loc_4_1099
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1099:
        xor    edi, edi
        xor    eax, eax
loc_4_109D:
        cmp    eax, dword ptr [ebp - 8]
        jae    SHORT loc_4_10B7
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        inc    edi
        mov    cl, byte ptr [ecx + eax - 1]
        cmp    edi, 6
        mov    byte ptr [esi + edi + 0bah], cl
        jb     SHORT loc_4_109D
loc_4_10B7:
        xor    ebx, ebx
        mov    eax, dword ptr [ebp - 10h]
        dec    eax
        je     SHORT loc_4_111B
loc_4_10BF:
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10CC:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_10DD:
        xor    ecx, ecx
        cmp    edi, 6
        jae    SHORT loc_4_10FE
loc_4_10E4:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_10FE
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    edi
        mov    al, byte ptr [eax + ecx - 1]
        cmp    edi, 6
        mov    byte ptr [esi + edi + 0bah], al
        jb     SHORT loc_4_10E4
loc_4_10FE:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3820
        test   eax, eax
        jl     SHORT loc_4_1135
        inc    ebx
        mov    eax, dword ptr [ebp - 10h]
        dec    eax
        cmp    eax, ebx
        ja     SHORT loc_4_10BF
loc_4_111B:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_38E0
        test   eax, eax
        jge    SHORT loc_4_1143
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1135:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1143:
        lea    eax, [esi + 0bbh]
        mov    ecx, dword ptr [esi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1153:
        test   al, al
        je     SHORT loc_4_1179
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        push   esi
        call   sub_4_11E0
        push   esi
        call   sub_4_1BB0
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1179:
        push   esi
        inc    dword ptr [esi + 83h]
        call   sub_4_1BB0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_4_18F0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_11A0:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 24h], 0
        jne    SHORT loc_4_11CC
        pop    ebp
        mov    dword ptr [eax + 24h], edx
        mov    dword ptr [eax + 28h], edx
        inc    dword ptr [eax + 2ch]
        ret    0ch
loc_4_11CC:
        mov    ecx, dword ptr [eax + 28h]
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [eax + 28h], edx
        inc    dword ptr [eax + 2ch]
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_4_11E0:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 38h], 0
        jne    SHORT loc_4_1209
        pop    ebp
        mov    dword ptr [eax + 38h], edx
        mov    dword ptr [eax + 34h], edx
        ret    0ch
loc_4_1209:
        mov    ecx, dword ptr [eax + 34h]
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [eax + 34h], edx
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1220:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 24h], 0
        mov    edx, dword ptr [eax + 24h]
        test   edx, edx
        jne    SHORT loc_4_1245
        pop    ebp
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [eax + 28h], ecx
        inc    dword ptr [eax + 2ch]
        ret    8
loc_4_1245:
        pop    ebp
        mov    dword ptr [ecx + 24h], edx
        mov    dword ptr [eax + 24h], ecx
        inc    dword ptr [eax + 2ch]
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1260:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        mov    al, byte ptr [ebp + 0ch]
        push   esi
        test   al, al
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0b7h], 0
        je     SHORT loc_4_12B8
        test   al, 1
        je     SHORT loc_4_1284
        inc    dword ptr [esi + 30h]
loc_4_1284:
        test   al, 2
        je     SHORT loc_4_1291
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 93h]
loc_4_1291:
        test   al, 4
        je     SHORT loc_4_129E
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_129E:
        test   al, 8
        je     SHORT loc_4_12AB
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_12AB:
        test   al, 10h
        je     SHORT loc_4_12B8
        inc    dword ptr [esi + 30h]
        inc    dword ptr [esi + 8bh]
loc_4_12B8:
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_4_12CC
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_12CC:
        mov    edi, dword ptr [esi + 24h]
        lea    ecx, [ebp - 18h]
        push   ecx
        lea    edx, [ebp - 0ch]
        mov    eax, dword ptr [edi + 24h]
        push   edx
        mov    dword ptr [esi + 24h], eax
        lea    eax, [ebp - 14h]
        push   eax
        dec    dword ptr [esi + 2ch]
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12EC:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_12FD:
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3690
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_1347
        push   edi
        push   esi
        call   sub_4_1220
        test   ebx, ebx
        jge    SHORT loc_4_1339
        add    ebx, 2
        pop    edi
        pop    esi
        cmp    ebx, 1
        sbb    eax, eax
        pop    ebx
        and    eax, 0fffffffch
        mov    esp, ebp
        add    eax, 2
        pop    ebp
        ret    8
loc_4_1339:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1347:
        xor    ebx, ebx
        xor    ecx, ecx
loc_4_134B:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_1365
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [esi + ebx + 0bah], al
        jb     SHORT loc_4_134B
loc_4_1365:
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp - 14h]
        dec    eax
        je     SHORT loc_4_13D2
loc_4_1372:
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_137F:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1390:
        xor    ecx, ecx
        cmp    ebx, 6
        jae    SHORT loc_4_13B1
loc_4_1397:
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_4_13B1
        mov    eax, dword ptr [ebp - 4]
        inc    ecx
        inc    ebx
        mov    al, byte ptr [eax + ecx - 1]
        cmp    ebx, 6
        mov    byte ptr [esi + ebx + 0bah], al
        jb     SHORT loc_4_1397
loc_4_13B1:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3820
        cmp    eax, -2
        je     SHORT loc_4_13F4
        mov    eax, dword ptr [ebp - 14h]
        inc    dword ptr [ebp - 10h]
        dec    eax
        cmp    eax, dword ptr [ebp - 10h]
        ja     SHORT loc_4_1372
loc_4_13D2:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_38E0
        cmp    eax, -2
        jne    SHORT loc_4_1409
        push   edi
        push   esi
        call   sub_4_1220
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_13F4:
        push   edi
        push   esi
        call   sub_4_1220
        mov    eax, 0fffffffeh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1409:
        lea    eax, [esi + 0bbh]
        mov    ecx, dword ptr [esi + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1419:
        test   al, al
        je     SHORT loc_4_1439
        push   edi
        mov    eax, dword ptr [edi + 28h]
        push   eax
        push   esi
        call   sub_4_11E0
        push   esi
        call   sub_4_44A0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1439:
        push   0
        inc    dword ptr [esi + 83h]
        mov    esi, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_144E:
        push   esi
        call   sub_4_18F0
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_4_1460:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        push   edi
        mov    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 0ffffffffh
        lea    eax, [eax + edx*4]
loc_4_1485:
        test   edx, edx
        je     SHORT loc_4_1494
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_4_1485
        mov    dword ptr [ecx], edx
loc_4_1494:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_4_14A7
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_14A7:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   25h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_14BA:
        test   eax, eax
        je     SHORT loc_4_14CC
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_14CC:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 1ch]
        add    eax, 1dh
        mov    ebx, dword ptr [ebp + 20h]
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebx + 7bh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_14E7:
        test   al, al
        jne    SHORT loc_4_1506
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   25h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_14F8:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_1506:
        test   byte ptr [ebx + 47h], 1
        je     SHORT loc_4_1528
        mov    eax, dword ptr [ebx + 1d1h]
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, 103h
        mov    dword ptr [ecx + 11h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 1d1h], ecx
        jmp    SHORT loc_4_153F
loc_4_1528:
        mov    eax, dword ptr [ebx + 1c9h]
        mov    ecx, dword ptr [ebp - 4]
        xor    esi, esi
        mov    dword ptr [ecx + 11h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 1c9h], ecx
loc_4_153F:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0dh], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebx + 1c5h]
        mov    dword ptr [eax + 19h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 28h]
        mov    dword ptr [eax + 5], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 9], edx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebx + 7fh]
        mov    dword ptr [eax + 21h], edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        cmp    dword ptr [ebx + 1cdh], 0
        jne    SHORT loc_4_1592
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_4_4010
loc_4_1592:
        mov    eax, esi
        pop    edi
        pop    esi
        inc    dword ptr [ebx + 1cdh]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_15B0:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0dh]
        xor    eax, eax
loc_4_15C2:
        mov    dl, byte ptr [esi + eax + 67h]
        mov    ecx, dword ptr [esi + 73h]
        inc    eax
        cmp    eax, 6
        mov    byte ptr [ecx + eax - 1], dl
        jb     SHORT loc_4_15C2
        mov    edi, dword ptr [esi + 73h]
        add    edi, 6
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_4_1604
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 14h]
loc_4_15EA:
        xor    ebx, ebx
loc_4_15EC:
        lea    edx, [ecx + ebx]
        inc    edi
        inc    ebx
        mov    dl, byte ptr [edx + eax]
        mov    byte ptr [edi - 1], dl
        cmp    ebx, 6
        jb     SHORT loc_4_15EC
        add    ecx, 6
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_4_15EA
loc_4_1604:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 77h], eax
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_4_161F
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 44h], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_161F:
        push   esi
        lea    eax, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_162E:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3040
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3120
        mov    ecx, dword ptr [esi + 215h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_31F0
        mov    ecx, dword ptr [esi + 1bdh]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3390
        test   eax, eax
        jge    SHORT loc_4_1676
        mov    eax, 0c0000001h
        pop    edi
        or     dword ptr [esi + 47h], 2
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_1676:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_4010
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1690:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        mov    esi, dword ptr [eax + 0dh]
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, 0ffffffd0h
        je     SHORT loc_4_16B0
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    14h
loc_4_16B0:
        test   al, 1
        je     SHORT loc_4_16B9
        mov    ecx, 1
loc_4_16B9:
        test   al, 2
        je     SHORT loc_4_16C0
        or     ecx, 4
loc_4_16C0:
        test   al, 4
        je     SHORT loc_4_16C7
        or     ecx, 8
loc_4_16C7:
        test   al, 8
        je     SHORT loc_4_16CE
        or     ecx, 2
loc_4_16CE:
        test   al, 20h
        je     SHORT loc_4_16D5
        or     ecx, 10h
loc_4_16D5:
        test   dword ptr [esi + 1c1h], ecx
        je     SHORT loc_4_16E7
        mov    eax, 0c00000bbh
        pop    esi
        pop    ebp
        ret    14h
loc_4_16E7:
        mov    dword ptr [esi + 1bdh], ecx
        cmp    byte ptr [esi + 40h], 0
        je     SHORT loc_4_16FE
        mov    byte ptr [esi + 43h], 1
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    14h
loc_4_16FE:
        push   esi
        lea    eax, [esi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_170D:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3040
        mov    eax, dword ptr [esi + 1bdh]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3390
        test   eax, eax
        jge    SHORT loc_4_1736
        or     dword ptr [esi + 47h], 2
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    14h
loc_4_1736:
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_4010
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1750:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0dh]
        push   1
        add    ecx, 1d5h
        push   ecx
        mov    edx, dword ptr [ecx + 1dh]
        mov    dword ptr [eax + 15h], edx
        mov    dword ptr [ecx + 1dh], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1771:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1780:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 1f2h]
        test   ecx, ecx
        je     SHORT loc_4_17B0
loc_4_1791:
        mov    eax, dword ptr [esi + 1f2h]
        push   ecx
        mov    edx, dword ptr [eax + 15h]
        mov    dword ptr [esi + 1f2h], edx
        call   sub_4_18F0
        mov    ecx, dword ptr [esi + 1f2h]
        test   ecx, ecx
        jne    SHORT loc_4_1791
loc_4_17B0:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_17C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        test   byte ptr [esi + 47h], 8
        jne    SHORT loc_4_17F6
        push   esi
        call   sub_4_1B80
        test   al, al
        je     SHORT loc_4_17E5
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3D90
        push   esi
        call   sub_4_1BB0
loc_4_17E5:
        push   3e8h
        add    esi, 1e1h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_17F6:
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_4_1800:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 4], 0
        mov    edi, dword ptr [esi + 0dh]
        je     SHORT loc_4_181E
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_181E:
        inc    dword ptr [esi]
        mov    eax, dword ptr [edi + 1c9h]
        cmp    esi, eax
        jne    SHORT loc_4_1835
        mov    eax, dword ptr [esi + 11h]
        mov    dword ptr [edi + 1c9h], eax
        jmp    SHORT loc_4_1848
loc_4_1835:
        cmp    dword ptr [eax + 11h], esi
        je     SHORT loc_4_1842
loc_4_183A:
        mov    eax, dword ptr [eax + 11h]
        cmp    dword ptr [eax + 11h], esi
        jne    SHORT loc_4_183A
loc_4_1842:
        mov    ecx, dword ptr [esi + 11h]
        mov    dword ptr [eax + 11h], ecx
loc_4_1848:
        push   0
        mov    eax, dword ptr [esi + 1dh]
        push   eax
        mov    ecx, dword ptr [edi + 7bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1857:
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_4_186F
        cmp    ebx, 103h
        je     SHORT loc_4_1883
        cmp    ebx, 0c001000eh
        je     SHORT loc_4_188C
        jmp    SHORT loc_4_1895
loc_4_186F:
        mov    eax, dword ptr [esi]
        sub    eax, 2
        mov    dword ptr [esi], eax
        je     SHORT loc_4_1897
        mov    byte ptr [esi + 4], 1
        mov    ebx, 103h
        jmp    SHORT loc_4_1897
loc_4_1883:
        mov    byte ptr [esi + 4], 1
        sub    dword ptr [esi], 2
        jmp    SHORT loc_4_1897
loc_4_188C:
        mov    byte ptr [esi + 4], 1
        mov    ebx, 103h
loc_4_1895:
        dec    dword ptr [esi]
loc_4_1897:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_18DB
        mov    eax, dword ptr [edi + 7fh]
        cmp    dword ptr [esi + 21h], eax
        jne    SHORT loc_4_18AA
        push   edi
        call   sub_4_0F50
loc_4_18AA:
        cmp    dword ptr [edi + 1c9h], 0
        jne    SHORT loc_4_18C2
        push   edi
        lea    eax, [edi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_18C2:
        push   0
        dec    dword ptr [edi + 1cdh]
        push   25h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_18D2:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_18DB:
        mov    ebx, 103h
        pop    edi
        mov    eax, ebx
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_18F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0dh]
        mov    eax, dword ptr [esi]
        dec    eax
        mov    dword ptr [esi], eax
        jne    SHORT loc_4_1943
        mov    eax, dword ptr [esi + 21h]
        cmp    dword ptr [edi + 7fh], eax
        jne    SHORT loc_4_1910
        push   edi
        call   sub_4_0F50
loc_4_1910:
        cmp    dword ptr [edi + 1c9h], 0
        jne    SHORT loc_4_1928
        push   edi
        lea    eax, [edi + 8]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1928:
        push   0
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1933:
        push   0
        push   25h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_193D:
        dec    dword ptr [edi + 1cdh]
loc_4_1943:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1950:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0dh]
        cmp    byte ptr [eax + 4], 0
        je     SHORT loc_4_196D
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_196D:
        test   byte ptr [esi + 47h], 1
        je     SHORT loc_4_197E
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_197E:
        push   40010004h
        inc    dword ptr [eax]
        push   esi
        mov    dword ptr [esi + 1eeh], eax
        call   sub_4_1B20
        or     dword ptr [esi + 47h], 1
        mov    ecx, dword ptr [esi + 24h]
        test   ecx, ecx
        je     SHORT loc_4_19C7
loc_4_199C:
        mov    eax, dword ptr [ecx + 24h]
        push   0c001000ch
        push   ecx
        mov    dword ptr [esi + 24h], eax
        dec    dword ptr [esi + 2ch]
        inc    dword ptr [esi + 30h]
        mov    edi, dword ptr [ecx + 28h]
        mov    eax, dword ptr [edi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19BA:
        push   edi
        call   sub_4_18F0
        mov    ecx, dword ptr [esi + 24h]
        test   ecx, ecx
        jne    SHORT loc_4_199C
loc_4_19C7:
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        je     SHORT loc_4_19F3
loc_4_19CE:
        mov    eax, dword ptr [ecx + 24h]
        push   0c001000ch
        push   ecx
        mov    dword ptr [esi + 38h], eax
        mov    edi, dword ptr [ecx + 28h]
        mov    eax, dword ptr [edi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_19E6:
        push   edi
        call   sub_4_18F0
        mov    ecx, dword ptr [esi + 38h]
        test   ecx, ecx
        jne    SHORT loc_4_19CE
loc_4_19F3:
        mov    byte ptr [esi + 203h], 1
        push   esi
        call   sub_4_1B80
        test   al, al
        je     SHORT loc_4_1A1B
        push   esi
        call   sub_4_1A30
        push   esi
        call   sub_4_1BB0
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_1A1B:
        mov    eax, 103h
        pop    edi
        mov    byte ptr [esi + 203h], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_1A30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 1eeh]
        mov    eax, dword ptr [esi]
        mov    byte ptr [esi + 203h], 0
        mov    byte ptr [esi + 42h], 1
        and    dword ptr [esi + 47h], 0fffffffdh
        push   eax
        call   sub_4_2FD0
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3120
        mov    ecx, dword ptr [esi + 215h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_31F0
        mov    ecx, dword ptr [esi + 1bdh]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3390
        test   eax, eax
        jge    SHORT loc_4_1A87
        or     dword ptr [esi + 47h], 2
loc_4_1A87:
        push   40010005h
        push   esi
        call   sub_4_1B20
        mov    eax, dword ptr [esi + 47h]
        and    eax, 0fffffffeh
        mov    dword ptr [esi + 47h], eax
        test   al, 2
        je     SHORT loc_4_1AA6
        push   80010004h
        jmp    SHORT loc_4_1AA8
loc_4_1AA6:
        push   0
loc_4_1AA8:
        mov    eax, dword ptr [edi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1AB1:
        push   edi
        call   sub_4_18F0
        push   esi
        call   sub_4_1AD0
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1AD0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 1d1h]
        test   ecx, ecx
        je     SHORT loc_4_1B16
loc_4_1AE1:
        mov    eax, dword ptr [esi + 1d1h]
        push   0
        push   0
        mov    edx, dword ptr [eax + 11h]
        mov    eax, dword ptr [esi + 1c9h]
        mov    dword ptr [esi + 1d1h], edx
        mov    dword ptr [ecx + 11h], eax
        mov    dword ptr [esi + 1c9h], ecx
        mov    ecx, dword ptr [ecx + 5]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1B0C:
        mov    ecx, dword ptr [esi + 1d1h]
        test   ecx, ecx
        jne    SHORT loc_4_1AE1
loc_4_1B16:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_1B20:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 1c9h]
        test   edi, edi
        je     SHORT loc_4_1B6B
        mov    esi, dword ptr [ebp + 0ch]
loc_4_1B3A:
        push   4
        lea    eax, [ebp - 4]
        push   eax
        inc    dword ptr [edi]
        mov    ebx, dword ptr [edi + 11h]
        push   esi
        mov    dword ptr [ebp - 4], 0
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1B56:
        mov    ecx, dword ptr [edi + 5]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1B5F:
        push   edi
        call   sub_4_18F0
        mov    edi, ebx
        test   ebx, ebx
        jne    SHORT loc_4_1B3A
loc_4_1B6B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1B80:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 40h], 0
        je     SHORT loc_4_1B93
        xor    al, al
        pop    ebp
        ret    4
loc_4_1B93:
        mov    byte ptr [eax + 40h], 1
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        add    eax, 8
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1BA6:
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_1BB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 38h], 1
        sbb    ecx, ecx
        xor    eax, eax
        inc    ecx
        mov    al, byte ptr [esi + 46h]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 41h]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 43h]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 44h]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 203h]
        or     ecx, eax
        je     NEAR PTR loc_4_1CDC
loc_4_1BED:
        cmp    byte ptr [esi + 203h], 0
        je     SHORT loc_4_1BFE
        mov    bl, 1
        push   esi
        call   sub_4_1A30
loc_4_1BFE:
        cmp    byte ptr [esi + 44h], 0
        je     SHORT loc_4_1C48
        mov    byte ptr [esi + 44h], 0
        mov    byte ptr [esi + 43h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3040
        mov    eax, dword ptr [esi + 77h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3120
        mov    ecx, dword ptr [esi + 215h]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_31F0
        mov    ecx, dword ptr [esi + 1bdh]
        mov    edx, dword ptr [esi]
        push   ecx
        push   edx
        call   sub_4_3390
        test   eax, eax
        jge    SHORT loc_4_1C46
        or     dword ptr [esi + 47h], 2
loc_4_1C46:
        mov    bl, 1
loc_4_1C48:
        cmp    byte ptr [esi + 43h], 0
        je     SHORT loc_4_1C73
        mov    byte ptr [esi + 43h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3040
        mov    eax, dword ptr [esi + 1bdh]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3390
        test   eax, eax
        jge    SHORT loc_4_1C71
        or     dword ptr [esi + 47h], 2
loc_4_1C71:
        mov    bl, 1
loc_4_1C73:
        cmp    byte ptr [esi + 46h], 0
        je     SHORT loc_4_1C88
        mov    eax, dword ptr [esi]
        mov    bl, 1
        mov    byte ptr [esi + 46h], 0
        push   eax
        push   esi
        call   sub_4_3E40
loc_4_1C88:
        cmp    dword ptr [esi + 38h], 0
        je     SHORT loc_4_1C96
        push   esi
        mov    bl, 1
        call   sub_4_44A0
loc_4_1C96:
        cmp    byte ptr [esi + 41h], 0
        je     SHORT loc_4_1CA9
        mov    byte ptr [esi + 41h], 0
        mov    eax, dword ptr [esi + 7bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1CA9:
        cmp    dword ptr [esi + 38h], 1
        sbb    ecx, ecx
        xor    eax, eax
        inc    ecx
        mov    al, byte ptr [esi + 46h]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 41h]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 43h]
        or     ecx, eax
        xor    edx, edx
        mov    dl, byte ptr [esi + 44h]
        xor    eax, eax
        or     ecx, edx
        mov    al, byte ptr [esi + 203h]
        or     ecx, eax
        jne    NEAR PTR loc_4_1BED
loc_4_1CDC:
        mov    byte ptr [esi + 40h], 0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_4010
        mov    al, bl
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_1CF0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_4_3FF0
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h
        db  024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 051h, 0E8h, 0F1h, 022h, 000h, 000h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1D30:
        push   ebp
        mov    eax, 7
        mov    ebp, esp
        sub    esp, 18h
        mov    word ptr [ebp - 0ch], ax
        push   ebx
        mov    word ptr [ebp - 0ah], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 16h], bx
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 18h]
        db  0C7h, 045h, 0F8h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 14h], 4019f022h
        mov    word ptr [ebp - 18h], 1
        push   ecx
        push   eax
        db  068h, 00Ch, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1D71:
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        push   1
        db  089h, 015h, 046h, 000h, 000h, 000h
        db  089h, 00Dh, 04Ah, 000h, 000h, 000h
        db  089h, 01Dh, 004h, 000h, 000h, 000h
        db  089h, 01Dh, 052h, 000h, 000h, 000h
        call   sub_4_2560
        db  0A3h, 04Eh, 000h, 000h, 000h
        db  0C6h, 005h, 010h, 000h, 000h, 000h, 003h
        db  088h, 01Dh, 011h, 000h, 000h, 000h
        db  089h, 01Dh, 012h, 000h, 000h, 000h
        db  0C7h, 005h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 01Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 026h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        db  0C7h, 005h, 02Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 02Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 036h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 03Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   36h
        db  089h, 015h, 03Eh, 000h, 000h, 000h
        db  068h, 010h, 000h, 000h, 000h
        lea    edx, [ebp - 4]
        db  068h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 042h, 000h, 000h, 000h
        db  0A1h, 00Ch, 000h, 000h, 000h
        push   eax
        db  068h, 008h, 000h, 000h, 000h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E44:
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_4_1E71
        db  039h, 01Dh, 004h, 000h, 000h, 000h
        jbe    SHORT loc_4_1E5A
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1E5A:
        push   0
        db  0A1h, 00Ch, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E67:
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_1E71:
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_4_1E80:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   0
        push   22dh
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1E9E:
        test   eax, eax
        je     SHORT loc_4_1EAF
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1EAF:
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 0ch]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_0630
        test   eax, eax
        je     SHORT loc_4_1EE1
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   22dh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1ED4:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1EE1:
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   esi
        push   eax
        call   sub_4_1F20
        test   eax, eax
        jne    SHORT loc_4_1EFA
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1EFA:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   22dh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1F0A:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1F20:
        push   ebp
        mov    ebp, esp
        sub    esp, 64h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_23F0
        push   esi
        call   sub_4_22E0
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    NEAR PTR loc_4_2252
        lea    edi, [ebp - 64h]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 64h], 0
        mov    eax, dword ptr [esi + 20dh]
        cmp    eax, -1
        je     SHORT loc_4_1F66
        mov    dword ptr [ebp - 64h], eax
loc_4_1F66:
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 209h]
        mov    dword ptr [ebp - 56h], ebx
        mov    dword ptr [ebp - 4ah], eax
        mov    dword ptr [ebp - 5ah], 1
        lea    ecx, [ebp - 64h]
        mov    edx, dword ptr [ebp + 10h]
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   edx
        db  08Bh, 00Dh, 008h, 000h, 000h, 000h
        push   eax
        lea    edx, [esi + 205h]
        mov    dword ptr [ebp - 52h], 600h
        mov    dword ptr [ebp - 4eh], 1
        mov    dword ptr [ebp - 46h], 40h
        push   esi
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1FB0:
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    NEAR PTR loc_4_224C
        mov    eax, dword ptr [esi + 20dh]
        cmp    eax, -1
        je     SHORT loc_4_2031
        lea    ecx, [ebp - 20h]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [ecx + 8], ebx
        mov    dword ptr [ecx + 0ch], ebx
        push   600h
        mov    word ptr [ecx + 10h], bx
        push   ecx
        mov    byte ptr [ecx + 12h], bl
        lea    edx, [esi + 4bh]
        mov    byte ptr [ebp - 20h], 1
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 15h], eax
        mov    dword ptr [ebp - 1dh], 2
        mov    dword ptr [ebp - 19h], 3
        mov    eax, dword ptr [esi + 205h]
        push   eax
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_200A:
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_4_2031
        mov    eax, dword ptr [esi + 20dh]
        mov    ecx, dword ptr [esi + 205h]
        push   eax
        push   1
        push   0aa000001h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2029:
        add    esp, 10h
        jmp    NEAR PTR loc_4_2240
loc_4_2031:
        lea    eax, [ebp - 3ch]
        mov    ecx, dword ptr [esi + 219h]
        push   eax
        mov    edx, dword ptr [esi + 209h]
        push   ecx
        push   edx
        call   sub_4_4090
        mov    ecx, dword ptr [esi + 209h]
        mov    edx, dword ptr [esi + 20dh]
        mov    ebx, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   edx
        push   ebx
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        lea    edx, [ecx + 40h]
        push   edx
        mov    eax, dword ptr [esi + 73h]
        push   ecx
        mov    edx, dword ptr [esi]
        mov    ecx, dword ptr [esi + 219h]
        push   ecx
        push   eax
        push   esi
        push   edx
        call   sub_4_2EC0
        test   eax, eax
        jge    SHORT loc_4_20B1
        mov    eax, dword ptr [esi + 209h]
        mov    ecx, dword ptr [esi + 219h]
        push   eax
        mov    edx, dword ptr [esi + 205h]
        push   ecx
        push   2
        push   0aa000004h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_20A2:
        mov    dword ptr [ebp - 4], 0c0000001h
        add    esp, 14h
        jmp    NEAR PTR loc_4_222E
loc_4_20B1:
        dec    eax
        cmp    eax, 1
        mov    eax, 2
        adc    eax, -1
        xor    ecx, ecx
        mov    dword ptr [esi + 20h], eax
loc_4_20C2:
        lea    eax, [ecx + esi]
        mov    edx, dword ptr [esi + 73h]
        inc    ecx
        cmp    ecx, 6
        mov    dl, byte ptr [edx + ecx - 1]
        mov    byte ptr [eax + 6dh], dl
        mov    byte ptr [eax + 67h], dl
        jb     SHORT loc_4_20C2
        lea    ebx, [esi + 7bh]
        lea    eax, [esi + 4]
        push   ebx
        lea    ecx, [esi + 67h]
        push   eax
        mov    edx, dword ptr [esi + 211h]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_20FF:
        test   al, al
        jne    SHORT loc_4_210F
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    NEAR PTR loc_4_222E
loc_4_210F:
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3120
        mov    eax, dword ptr [esi + 215h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_31F0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_3D50
        mov    edi, eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        lea    eax, [ebp - 8]
        push   0
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2145:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_4_2158
        mov    dword ptr [ebp - 4], 0c000009ah
        jmp    NEAR PTR loc_4_2226
loc_4_2158:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_3D70
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [esi + 0a3h], edx
        push   0
        mov    eax, dword ptr [ecx + 4]
        push   edi
        push   ecx
        mov    dword ptr [esi + 0a7h], eax
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [esi + 0abh], edx
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esi + 0afh], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2193:
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_3FD0
        push   0
        mov    ecx, dword ptr [esi]
        not    eax
        push   ecx
        mov    dword ptr [esi + 1c1h], eax
        mov    dword ptr [esi + 1bdh], 0
        call   sub_4_3390
        test   eax, eax
        jge    SHORT loc_4_21C3
        or     dword ptr [esi + 47h], 2
        jmp    SHORT loc_4_2226
loc_4_21C3:
        lea    edx, [esi + 8]
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 6
        rep stosd
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [esi + 219h]
        push   eax
        push   0
        push   ecx
        mov    eax, dword ptr [esi + 205h]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   edx
        lea    edx, [ebp - 4]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_21FB:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_225E
        mov    eax, dword ptr [esi + 219h]
        mov    ecx, dword ptr [esi + 205h]
        push   eax
        push   1
        push   0aa000005h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_221B:
        add    esp, 10h
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_4_3040
loc_4_2226:
        mov    eax, dword ptr [ebx]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_222E:
        cmp    dword ptr [esi + 20dh], -1
        je     SHORT loc_4_2240
        mov    eax, dword ptr [esi + 4bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2240:
        mov    eax, dword ptr [esi + 205h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_224C:
        push   esi
        call   sub_4_23B0
loc_4_2252:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_225E:
        db  0C7h, 086h, 0C5h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 45h], 1
        xor    ecx, ecx
        mov    dword ptr [esi + 1cdh], ecx
        mov    dword ptr [esi + 1c9h], ecx
        mov    dword ptr [esi + 1d1h], ecx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 229h], eax
        lea    eax, [esi + 1e1h]
        db  089h, 035h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 1d5h], ecx
        mov    dword ptr [esi + 1ddh], esi
        push   3e8h
        mov    dword ptr [eax], ecx
        push   eax
        mov    dword ptr [esi + 1e9h], esi
        db  0C7h, 086h, 0D9h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 086h, 0E5h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_22CA:
        mov    ecx, dword ptr [esi + 1c5h]
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        inc    dword ptr [ecx + 4]
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_4_22E0:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        db  08Bh, 00Dh, 04Eh, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   3
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2301:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_4_2316
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_2316:
        lea    ebx, [esi + 73h]
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        mov    ecx, dword ptr [esi + 211h]
        push   0
        lea    eax, [ecx + ecx*2 + 3]
        add    eax, eax
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2334:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_4_2385
        mov    edx, dword ptr [esi + 211h]
        mov    edi, dword ptr [ebx]
        xor    eax, eax
        lea    edx, [edx + edx*2 + 3]
        add    edx, edx
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        push   2
        and    ecx, 3
        rep stosb
        lea    ecx, [ebp - 4]
        lea    eax, [esi + 4fh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2368:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_23A3
        push   0
        mov    eax, dword ptr [esi + 211h]
        mov    ecx, dword ptr [ebx]
        lea    eax, [eax + eax*2 + 3]
        add    eax, eax
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2385:
        push   3
        db  0A1h, 04Eh, 000h, 000h, 000h
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2395:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_23A3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_4_23B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4fh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_23C0:
        push   3
        db  0A1h, 04Eh, 000h, 000h, 000h
        push   eax
        mov    ecx, dword ptr [esi]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_23D0:
        push   0
        mov    ecx, dword ptr [esi + 211h]
        lea    eax, [ecx + ecx*2 + 3]
        mov    ecx, dword ptr [esi + 73h]
        add    eax, eax
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_23E8:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_23F0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 1cdh], ecx
        mov    byte ptr [eax + 40h], cl
        mov    byte ptr [eax + 44h], cl
        mov    byte ptr [eax + 202h], cl
        mov    byte ptr [eax + 45h], cl
        mov    byte ptr [eax + 41h], cl
        mov    byte ptr [eax + 1edh], cl
        mov    dword ptr [eax + 47h], ecx
        mov    dword ptr [eax + 1bdh], ecx
        mov    dword ptr [eax + 77h], ecx
        mov    byte ptr [eax + 43h], cl
        mov    byte ptr [eax + 203h], cl
        mov    byte ptr [eax + 204h], cl
        mov    dword ptr [eax + 28h], ecx
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [eax + 2ch], ecx
        mov    dword ptr [eax + 30h], ecx
        mov    byte ptr [eax + 46h], cl
        pop    ebp
        mov    dword ptr [eax + 34h], ecx
        mov    dword ptr [eax + 38h], ecx
        mov    dword ptr [eax + 3ch], ecx
        mov    dword ptr [eax + 7fh], 0eeh
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2460:
        push   ebp
        db  0A1h, 008h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2475:
        push   0
        db  08Bh, 00Dh, 00Ch, 000h, 000h, 000h
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2483:
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2490:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   4c4b40h
        mov    esi, dword ptr [ebp + 8]
        or     dword ptr [esi + 47h], 8
        lea    edi, [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_24A9:
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_24B5:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_24BB:
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_4_24D0
        mov    eax, dword ptr [esi + 229h]
        db  0A3h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_24FE
loc_4_24D0:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 229h], esi
        je     SHORT loc_4_24EC
loc_4_24DE:
        mov    ecx, dword ptr [ecx + 229h]
        cmp    dword ptr [ecx + 229h], esi
        jne    SHORT loc_4_24DE
loc_4_24EC:
        mov    eax, dword ptr [ecx + 229h]
        mov    edx, dword ptr [eax + 229h]
        mov    dword ptr [ecx + 229h], edx
loc_4_24FE:
        cmp    dword ptr [esi + 20dh], -1
        je     SHORT loc_4_2510
        mov    eax, dword ptr [esi + 4bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2510:
        mov    eax, dword ptr [esi + 7bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2519:
        mov    eax, dword ptr [esi + 205h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2525:
        push   esi
        call   sub_4_23B0
        push   0
        push   22dh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2538:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_2540:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_3040
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_4_2FD0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_2560:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_4_257A
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
loc_4_257A:
        mov    eax, 186h
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2590:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    bx, bx
        push   edi
        mov    esi, dword ptr [ecx + 4]
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        cmp    al, 50h
        jne    NEAR PTR loc_4_275C
        lea    edx, [esi + 1]
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        cmp    al, 48h
        jne    NEAR PTR loc_4_275C
        lea    edx, [esi + 2]
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        mov    eax, dword ptr [ecx + 104h]
        and    al, 0f0h
        cmp    al, 50h
        jne    NEAR PTR loc_4_275C
        lea    edx, [esi + 3]
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        cmp    al, 53h
        jne    NEAR PTR loc_4_275C
        mov    edx, dword ptr [ecx + 0ch]
        mov    eax, 6
        out    dx, ax
        mov    esi, dword ptr [ecx + 44h]
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        cmp    al, 8
        jne    NEAR PTR loc_4_274F
        lea    edx, [esi + 1]
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        cmp    al, bl
        jne    NEAR PTR loc_4_274F
        lea    edx, [esi + 2]
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        cmp    al, 9
        jne    NEAR PTR loc_4_274F
        mov    esi, dword ptr [ecx + 24h]
        mov    edx, esi
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        cmp    al, 22h
        jne    NEAR PTR loc_4_2742
        lea    edx, [esi + 1]
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        cmp    al, 0f0h
        jne    NEAR PTR loc_4_2742
        lea    edx, [esi + 2]
        in     al, dx
        lea    edx, [esi + 3]
        mov    byte ptr [ecx + 104h], al
        mov    byte ptr [ebp - 1], al
        in     al, dx
        cmp    byte ptr [ebp - 1], 0f1h
        mov    byte ptr [ecx + 104h], al
        jne    SHORT loc_4_26AB
        cmp    al, 80h
        je     SHORT loc_4_2685
        cmp    byte ptr [ebp - 1], 0f1h
        jne    SHORT loc_4_26AB
        cmp    al, 50h
        jne    SHORT loc_4_26AB
loc_4_2685:
        push   0
        mov    eax, dword ptr [ecx + 108h]
        push   0
        push   0ah
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        call   sub_4_0080
        mov    ax, 0ff02h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_26AB:
        cmp    byte ptr [ebp - 1], 19h
        jne    NEAR PTR loc_4_2735
        cmp    al, 40h
        je     SHORT loc_4_26BD
        cmp    al, 50h
        jne    SHORT loc_4_2735
loc_4_26BD:
        xor    edi, edi
        mov    esi, dword ptr [ebp + 0ch]
loc_4_26C2:
        mov    edx, dword ptr [ecx + 44h]
        inc    esi
        add    edx, edi
        inc    edi
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        mov    byte ptr [esi - 1], al
        and    ax, 0ffh
        add    bx, ax
        cmp    edi, 6
        jl     SHORT loc_4_26C2
        mov    edx, dword ptr [ecx + 5ch]
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        and    ax, 0ffh
        add    bx, ax
        cmp    bl, 0ffh
        jne    SHORT loc_4_2725
        xor    bx, bx
        xor    edi, edi
        mov    esi, dword ptr [ecx + 24h]
loc_4_26FD:
        lea    edx, [edi + esi]
        inc    edi
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        and    ax, 0ffh
        add    bx, ax
        cmp    edi, 4
        jl     SHORT loc_4_26FD
        mov    edx, dword ptr [ecx + 34h]
        in     al, dx
        mov    byte ptr [ecx + 104h], al
        and    ax, 0ffh
        add    bx, ax
loc_4_2725:
        mov    ax, bx
        pop    edi
        and    ax, 0ffh
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2735:
        mov    ax, 0ff02h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2742:
        mov    ax, 0ff02h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_274F:
        mov    ax, 0ff01h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_275C:
        mov    ax, 0ff00h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2770:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        test   ebx, ebx
        jne    SHORT loc_4_278E
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_278E:
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 14h]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        test   al, 40h
        jne    SHORT loc_4_27D0
        mov    eax, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 2]
        push   eax
        mov    dx, word ptr [esi]
        push   ebx
        push   ecx
        mov    eax, dword ptr [esi + 108h]
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   eax
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_27D0:
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [esi + 2ch]
        in     ax, dx
        mov    cx, ax
        mov    edx, dword ptr [esi + 24h]
        mov    word ptr [esi + 104h], ax
        and    ecx, 0ffh
        in     ax, dx
        shl    ecx, 10h
        mov    word ptr [esi + 104h], ax
        mov    eax, dword ptr [esi + 104h]
        and    eax, 0ff00h
        or     eax, ecx
        mov    dword ptr [ebp - 0ch], eax
        cmp    ebx, eax
        je     SHORT loc_4_283E
        mov    dx, word ptr [esi]
        push   ebx
        push   eax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 2]
        mov    eax, dword ptr [esi + 108h]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   eax
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_283E:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, eax
        sub    ecx, ebx
        cmp    ecx, 1fffh
        jae    SHORT loc_4_2879
        mov    cx, word ptr [esi]
        push   eax
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 2]
        mov    edx, dword ptr [esi + 108h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   edx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_2879:
        push   2000h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [esi + 108h]
        push   ecx
        call   sub_4_0020
        mov    edi, eax
        mov    dword ptr [esi + 114h], eax
        test   edi, edi
        jne    SHORT loc_4_28C9
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        mov    dx, word ptr [esi]
        push   eax
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 108h]
        mov    cx, word ptr [esi + 2]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   eax
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_28C9:
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, 2
        out    dx, ax
        mov    ebx, dword ptr [esi + 3ch]
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    eax, 2000h
        mov    ecx, dword ptr [esi + 104h]
        mov    edx, dword ptr [esi + 0ch]
        shr    cl, 5
        shl    eax, cl
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [esi + 14eh], eax
        mov    eax, 5
        mov    ecx, dword ptr [ebp - 14h]
        out    dx, ax
        shr    ecx, 4
        mov    edx, dword ptr [esi + 34h]
        dec    cx
        xor    eax, eax
        out    dx, ax
        mov    ax, cx
        mov    edx, ebx
        out    dx, ax
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 2
        out    dx, ax
        mov    dword ptr [ebp - 4], 0ch
        mov    eax, 3
        out    dx, ax
        mov    edx, dword ptr [esi + 0ch]
        xor    eax, eax
        out    dx, ax
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0fffffc00h
        mov    dword ptr [ebp - 18h], eax
        je     SHORT loc_4_29AF
        mov    eax, dword ptr [ebp - 14h]
        shr    eax, 0ah
        mov    dword ptr [ebp - 8], eax
loc_4_2950:
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp - 10h]
        cmp    eax, 1
        jne    SHORT loc_4_2962
        mov    dword ptr [ebp - 4], 1ch
loc_4_2962:
        mov    cx, 400h
        mov    ebx, 400h
        sub    cx, word ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 44h]
        mov    eax, ebx
        out    dx, eax
        mov    ax, cx
        mov    edx, dword ptr [esi + 34h]
        out    dx, ax
        xor    ecx, ecx
        test   ebx, 0fffffffch
        jbe    SHORT loc_4_2998
        shr    ebx, 2
        mov    edx, dword ptr [esi + 44h]
loc_4_2990:
        mov    eax, ecx
        inc    ecx
        out    dx, eax
        cmp    ecx, ebx
        jb     SHORT loc_4_2990
loc_4_2998:
        mov    edx, dword ptr [esi + 1ch]
        mov    eax, 3
        out    dx, ax
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_4_2950
        jmp    SHORT loc_4_29B2
loc_4_29AF:
        mov    ecx, dword ptr [ebp - 4]
loc_4_29B2:
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 8000h
        out    dx, ax
        mov    eax, 2
        out    dx, ax
        mov    eax, 3
        out    dx, ax
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, 7
        out    dx, ax
        mov    edx, dword ptr [esi + 6ch]
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 8100h
        out    dx, ax
        mov    edx, dword ptr [esi + 0ch]
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 2000h
        mov    dword ptr [ebp - 4], 0ch
        out    dx, ax
        mov    eax, dword ptr [edi + 4]
        xor    ebx, ebx
        add    eax, dword ptr [edi]
        inc    eax
        cmp    dword ptr [ebp - 18h], ebx
        mov    dword ptr [esi + 104h], eax
        je     SHORT loc_4_2A7C
        mov    eax, dword ptr [ebp - 14h]
        shr    eax, 0ah
        mov    dword ptr [ebp - 8], eax
loc_4_2A19:
        mov    eax, dword ptr [ebp - 8]
        sub    eax, ebx
        cmp    eax, 1
        jne    SHORT loc_4_2A2A
        mov    dword ptr [ebp - 4], 1ch
loc_4_2A2A:
        mov    ecx, 3
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        mov    ecx, ebx
        mov    eax, dword ptr [edi + edx*4]
        and    ecx, 1
        add    eax, dword ptr [edi + ecx*4]
        add    eax, dword ptr [edi]
        jne    SHORT loc_4_2A4A
        mov    dword ptr [esi + 104h], ebx
loc_4_2A4A:
        xor    ecx, ecx
        mov    eax, 400h
        sub    eax, dword ptr [ebp - 4]
        test   eax, 0fffffffch
        jbe    SHORT loc_4_2A6F
loc_4_2A5B:
        mov    edx, ecx
        and    edx, 1
        cmp    dword ptr [edi + edx*4], ecx
        jne    SHORT loc_4_2A6F
        inc    ecx
        mov    edx, eax
        shr    edx, 2
        cmp    edx, ecx
        ja     SHORT loc_4_2A5B
loc_4_2A6F:
        shr    eax, 2
        cmp    eax, ecx
        ja     SHORT loc_4_2A7C
        inc    ebx
        cmp    ebx, dword ptr [ebp - 8]
        jb     SHORT loc_4_2A19
loc_4_2A7C:
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 2100h
        out    dx, ax
        mov    eax, 8000h
        out    dx, ax
        mov    eax, 2
        out    dx, ax
        mov    eax, 3
        out    dx, ax
        mov    eax, 400h
        sub    eax, dword ptr [ebp - 4]
        shr    eax, 2
        cmp    eax, ecx
        jbe    SHORT loc_4_2AD6
        push   edi
        xor    eax, eax
        mov    ax, word ptr [esi + 2]
        push   ecx
        mov    cx, word ptr [esi]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esi + 108h]
        push   ecx
        push   edx
        call   sub_4_0080
        mov    eax, 2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_2AD6:
        db  08Bh, 00Dh, 00Ch, 000h, 000h, 000h
        mov    eax, 3
        shr    dword ptr [ebp - 0ch], 8
        add    ecx, 16h
loc_4_2AE8:
        shr    dword ptr [ebp - 0ch], 4
        mov    edx, dword ptr [ebp - 0ch]
        dec    ecx
        and    edx, 0fh
        dec    eax
        db  08Ah, 092h, 000h, 000h, 000h, 000h
        mov    byte ptr [ecx + 1], dl
        jne    SHORT loc_4_2AE8
        mov    eax, 3
        pop    edi
        or     byte ptr [esi + 14ch], 4
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        mov    ecx, dword ptr [ebp + 14h]
        test   cl, 3fh
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 14ch], 2
        jne    SHORT loc_4_2B57
        cmp    ecx, 100h
        jb     SHORT loc_4_2B57
        cmp    ecx, 3c0h
        ja     SHORT loc_4_2B57
        mov    eax, dword ptr [ebp + 18h]
        sub    eax, ecx
        cmp    eax, 1fh
        jae    SHORT loc_4_2B96
loc_4_2B57:
        mov    eax, ecx
        and    eax, 0ffff0fffh
        cmp    eax, 0c00h
        jne    NEAR PTR loc_4_2EA3
        mov    eax, dword ptr [ebp + 18h]
        sub    eax, ecx
        cmp    eax, 3fh
        jb     NEAR PTR loc_4_2EA3
        lea    ecx, [esi + 4]
        mov    eax, 40h
        mov    word ptr [esi + 14ch], 1
        add    word ptr [esi + 2], 38h
loc_4_2B8D:
        add    dword ptr [ecx], 38h
        add    ecx, 4
        dec    eax
        jne    SHORT loc_4_2B8D
loc_4_2B96:
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        push   esi
        call   sub_4_2590
        cmp    ax, 0ffh
        je     SHORT loc_4_2BBE
        mov    cx, 0ffh
        pop    edi
        cmp    cx, ax
        pop    esi
        sbb    eax, eax
        pop    ebx
        and    eax, 0fffffffbh
        mov    esp, ebp
        add    eax, 9
        pop    ebp
        ret    1ch
loc_4_2BBE:
        mov    ecx, dword ptr [esi + 0ch]
        mov    eax, 6
        mov    edx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 38h]
        in     al, dx
        mov    edx, dword ptr [esi + 14h]
        mov    byte ptr [esi + 104h], al
        mov    byte ptr [esi + 10ch], al
        mov    eax, 0ch
        out    dx, ax
        mov    eax, 303h
        out    dx, ax
        mov    edx, ecx
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [esi + 40h]
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    eax, dword ptr [esi + 104h]
        and    al, 0fbh
        out    dx, al
        mov    edx, ecx
        mov    eax, 4
        out    dx, ax
        mov    edx, dword ptr [esi + 34h]
        xor    ebx, ebx
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    eax, dword ptr [esi + 104h]
        and    al, 0cfh
        out    dx, al
loc_4_2C25:
        mov    edx, dword ptr [esi + 34h]
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    eax, dword ptr [esi + 104h]
        and    al, 0c0h
        cmp    al, 0c0h
        je     SHORT loc_4_2C52
        push   61h
        mov    eax, dword ptr [esi + 108h]
        push   eax
        inc    ebx
        call   sub_4_0260
        cmp    ebx, 0a28h
        jb     SHORT loc_4_2C25
loc_4_2C52:
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, 3
        out    dx, ax
        mov    edx, dword ptr [esi + 24h]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        mov    ebx, 7d0h
        mov    eax, dword ptr [esi + 104h]
        and    ax, 0fffeh
        out    dx, ax
        or     al, 1
        out    dx, ax
loc_4_2C87:
        push   61h
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   sub_4_0260
        dec    ebx
        jne    SHORT loc_4_2C87
        mov    edx, dword ptr [esi + 1ch]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        test   al, 40h
        je     SHORT loc_4_2CB6
        mov    eax, 0ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_2CB6:
        push   edi
        push   esi
        call   sub_4_2590
        cmp    ax, 0ffh
        je     SHORT loc_4_2CD1
        mov    eax, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_2CD1:
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 0d00ch
        out    dx, ax
        mov    eax, 2300h
        mov    ecx, dword ptr [ebp + 1ch]
        out    dx, ax
        mov    edx, dword ptr [esi + 1ch]
        mov    eax, 86h
        out    dx, ax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        push   ecx
        push   esi
        call   sub_4_2770
        test   byte ptr [esi + 14ch], 1
        mov    dword ptr [ebp - 4], eax
        je     SHORT loc_4_2D12
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 4], eax
loc_4_2D12:
        mov    edx, dword ptr [esi + 0ch]
        xor    eax, eax
        out    dx, ax
        mov    ebx, dword ptr [esi + 24h]
        mov    eax, 0ffffh
        mov    edx, ebx
        out    dx, ax
        mov    edx, dword ptr [esi + 2ch]
        mov    eax, 0fefeh
        out    dx, ax
        mov    eax, 700h
        xor    ecx, ecx
        out    dx, ax
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, 1
        out    dx, ax
loc_4_2D42:
        lea    edx, [ebx + ecx]
        mov    al, byte ptr [edi]
        inc    ecx
        inc    edi
        out    dx, al
        cmp    ecx, 6
        jb     SHORT loc_4_2D42
        xor    ecx, ecx
        mov    edi, dword ptr [esi + 44h]
loc_4_2D54:
        lea    edx, [edi + ecx]
        xor    al, al
        out    dx, al
        inc    ecx
        cmp    ecx, 8
        jb     SHORT loc_4_2D54
        mov    ecx, dword ptr [ebp + 10h]
        cmp    ecx, 3
        jb     NEAR PTR loc_4_2E95
        cmp    ecx, 8
        je     NEAR PTR loc_4_2E95
        cmp    ecx, 0dh
        je     NEAR PTR loc_4_2E95
        cmp    ecx, 0eh
        je     NEAR PTR loc_4_2E95
        cmp    ecx, 0fh
        ja     NEAR PTR loc_4_2E95
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [esi + 38h]
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    eax, dword ptr [esi + 104h]
        and    eax, 0fh
        cmp    eax, ecx
        je     SHORT loc_4_2DBF
        mov    eax, 7
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_2DBF:
        mov    edx, dword ptr [esi + 3ch]
        mov    edi, 2000h
        in     al, dx
        mov    edx, dword ptr [esi + 0ch]
        mov    byte ptr [esi + 104h], al
        mov    ecx, dword ptr [esi + 104h]
        mov    eax, 5
        out    dx, ax
        shr    cl, 5
        shl    edi, cl
        mov    ecx, edi
        mov    edx, edi
        shr    ecx, 2
        mov    dword ptr [esi + 14eh], edi
        shr    edx, 1
        add    edx, ecx
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 0ch], edx
        shr    dword ptr [ebp - 0ch], 4
        mov    ax, word ptr [ebp - 0ch]
        mov    edx, dword ptr [esi + 34h]
        mov    dword ptr [esi + 15ah], 100h
        out    dx, ax
        mov    eax, edi
        mov    edx, dword ptr [esi + 3ch]
        shr    eax, 4
        mov    dword ptr [esi + 152h], ecx
        mov    dword ptr [esi + 15eh], 0ah
        dec    ax
        out    dx, ax
        mov    edx, dword ptr [ebp - 8]
        mov    eax, 4
        mov    dword ptr [esi + 156h], edx
        mov    edx, dword ptr [esi + 0ch]
        out    dx, ax
        mov    edx, ebx
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        mov    edx, dword ptr [esi + 38h]
        in     al, dx
        mov    ecx, dword ptr [esi + 104h]
        mov    word ptr [esi + 182h], cx
        mov    byte ptr [esi + 104h], al
        mov    eax, dword ptr [esi + 104h]
        and    al, 85h
        out    dx, al
        mov    edx, dword ptr [esi + 34h]
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    eax, dword ptr [esi + 104h]
        and    al, 0f0h
        pop    edi
        out    dx, al
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 3
        out    dx, ax
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_2E95:
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_2EA3:
        mov    eax, 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2EC0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 61h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, esi
        rep stosd
        stosw
        mov    word ptr [esi], ax
        mov    ebx, dword ptr [ebp + 18h]
        mov    word ptr [esi + 2], bx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 108h], ecx
        mov    byte ptr [esi + 185h], al
        mov    dword ptr [esi + 11eh], edi
        mov    dword ptr [edi], eax
        mov    word ptr [edi + 4], ax
loc_4_2EFE:
        lea    ecx, [ebx + eax]
        inc    eax
        cmp    eax, 40h
        mov    dword ptr [esi + eax*4], ecx
        jl     SHORT loc_4_2EFE
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   0
        mov    eax, dword ptr [ebp + 24h]
        mov    ecx, dword ptr [ebp + 20h]
        push   eax
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [ebp + 14h]
        push   edx
        push   ebx
        push   eax
        push   edi
        push   esi
        call   sub_4_2B20
        mov    edi, eax
        mov    ecx, dword ptr [esi + 108h]
        push   ecx
        call   sub_4_0010
        cmp    edi, 4
        jge    SHORT loc_4_2F99
        test   edi, edi
        jl     SHORT loc_4_2F99
        mov    edx, dword ptr [esi + 0ch]
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [esi + 24h]
        mov    eax, 0ffffh
        out    dx, ax
        mov    edx, dword ptr [esi + 2ch]
        mov    eax, 0fefeh
        mov    word ptr [esi], 2
        mov    ecx, dword ptr [ebp + 2ch]
        out    dx, ax
        mov    eax, 700h
        out    dx, ax
        db  08Bh, 004h, 0BDh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx], eax
        xor    eax, eax
        mov    edx, dword ptr [esi + 0ch]
        pop    edi
        out    dx, ax
        mov    ax, word ptr [esi + 14ch]
        pop    esi
        and    ax, 2
        pop    ebx
        cmp    ax, 1
        pop    ebp
        sbb    eax, eax
        neg    eax
        ret    28h
loc_4_2F99:
        cmp    edi, 0bh
        jae    SHORT loc_4_2FB0
        test   edi, edi
        jl     SHORT loc_4_2FB0
        db  08Bh, 004h, 0BDh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 2ch]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_4_2FB9
loc_4_2FB0:
        mov    ecx, dword ptr [ebp + 2ch]
        db  0C7h, 001h, 000h, 000h, 000h, 000h
loc_4_2FB9:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    28h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2FD0:
        push   ebp
        mov    eax, 0eh
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   61h
        mov    dword ptr [esi + 13eh], 0
        mov    word ptr [esi], 2
        mov    edx, dword ptr [esi + 14h]
        out    dx, ax
        mov    eax, 2300h
        out    dx, ax
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   sub_4_0260
        push   61h
        mov    ecx, dword ptr [esi + 108h]
        push   ecx
        call   sub_4_0260
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 3
        out    dx, ax
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        push   esi
        call   sub_4_3FF0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3040:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, 4
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    dword ptr [esi + 13eh], edi
        mov    edx, dword ptr [esi + 0ch]
        out    dx, ax
        mov    edx, dword ptr [esi + 34h]
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    eax, dword ptr [esi + 104h]
        and    al, 0cfh
        mov    edx, dword ptr [esi + 34h]
        out    dx, al
loc_4_3070:
        mov    edx, dword ptr [esi + 34h]
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    eax, dword ptr [esi + 104h]
        and    al, 0c0h
        cmp    al, 0c0h
        je     SHORT loc_4_309D
        push   61h
        mov    eax, dword ptr [esi + 108h]
        push   eax
        inc    edi
        call   sub_4_0260
        cmp    edi, 0a28h
        jl     SHORT loc_4_3070
loc_4_309D:
        cmp    byte ptr [esi + 10eh], 64h
        jne    SHORT loc_4_3104
        mov    edx, dword ptr [esi + 2ch]
        xor    edi, edi
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        mov    eax, dword ptr [esi + 104h]
        and    eax, 0ffe7h
        mov    edx, dword ptr [esi + 2ch]
        out    dx, ax
loc_4_30C4:
        push   61h
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   sub_4_0260
        mov    edx, dword ptr [esi + 2ch]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        test   al, 62h
        je     SHORT loc_4_30EB
        inc    edi
        cmp    edi, 0fa0h
        jl     SHORT loc_4_30C4
loc_4_30EB:
        mov    edx, dword ptr [esi + 2ch]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        mov    eax, dword ptr [esi + 104h]
        or     al, 8
        mov    edx, dword ptr [esi + 2ch]
        out    dx, ax
loc_4_3104:
        mov    word ptr [esi], 4
        push   esi
        call   sub_4_3FF0
        mov    edx, dword ptr [esi + 0ch]
        xor    eax, eax
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_3120:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    word ptr [esi + 11ch], di
        mov    ebx, dword ptr [ebp + 0ch]
        lea    eax, [esi + 122h]
        cmp    ebx, edi
        mov    dword ptr [eax], edi
        mov    dword ptr [eax + 4], edi
        je     SHORT loc_4_315C
loc_4_3144:
        mov    eax, dword ptr [esi + 11eh]
        add    eax, edi
        add    edi, 6
        add    eax, 6
        push   eax
        push   esi
        call   sub_4_3170
        dec    ebx
        jne    SHORT loc_4_3144
loc_4_315C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3170:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 6
        push   ebx
        push   esi
        push   edi
        mov    esi, 0ffffffffh
        mov    edi, dword ptr [ebp + 0ch]
loc_4_3188:
        mov    dl, byte ptr [edi]
        inc    edi
        mov    ecx, 8
loc_4_3190:
        mov    eax, esi
        mov    bl, dl
        shr    dl, 1
        and    eax, 80000000h
        shr    eax, 1fh
        and    ebx, 1
        xor    eax, ebx
        add    esi, esi
        test   eax, eax
        je     SHORT loc_4_31B2
        xor    esi, 4c11db6h
        or     esi, eax
loc_4_31B2:
        dec    ecx
        jne    SHORT loc_4_3190
        dec    dword ptr [ebp - 4]
        jne    SHORT loc_4_3188
        shr    esi, 1ah
        mov    ecx, 3fh
        sub    ecx, esi
        mov    bl, 1
        mov    edx, ecx
        mov    eax, dword ptr [ebp + 8]
        shr    edx, 3
        and    cl, 7
        shl    bl, cl
        pop    edi
        or     byte ptr [edx + eax + 122h], bl
        inc    word ptr [eax + 11ch]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_31F0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 13eh], eax
        mov    byte ptr [esi + 146h], al
        mov    eax, 4
        mov    edx, dword ptr [esi + 0ch]
        out    dx, ax
        mov    edx, dword ptr [esi + 2ch]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        test   al, 80h
        mov    ax, word ptr [esi + 182h]
        je     SHORT loc_4_3296
        test   al, 0c0h
        je     SHORT loc_4_326B
        test   al, 0ch
        je     SHORT loc_4_323D
        lea    edi, [esi + 10eh]
        mov    byte ptr [edi], 0ah
        jmp    NEAR PTR loc_4_32DB
loc_4_323D:
        mov    edx, dword ptr [esi + 2ch]
        lea    edi, [esi + 10eh]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        test   al, 20h
        je     SHORT loc_4_325B
        mov    byte ptr [edi], 64h
        jmp    NEAR PTR loc_4_32DB
loc_4_325B:
        push   0
        mov    byte ptr [edi], 0
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_32CD
loc_4_326B:
        mov    edx, dword ptr [esi + 2ch]
        lea    edi, [esi + 10eh]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        test   al, 20h
        je     SHORT loc_4_3286
        mov    byte ptr [edi], 64h
        jmp    SHORT loc_4_32DB
loc_4_3286:
        push   0
        mov    byte ptr [edi], 0
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_32CD
loc_4_3296:
        test   al, 0c0h
        je     SHORT loc_4_32B9
        lea    edi, [esi + 10eh]
        test   al, 0ch
        je     SHORT loc_4_32A9
        mov    byte ptr [edi], 0ah
        jmp    SHORT loc_4_32DB
loc_4_32A9:
        push   0
        mov    byte ptr [edi], 0
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_32CD
loc_4_32B9:
        lea    edi, [esi + 10eh]
        push   1
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        mov    byte ptr [edi], 0
loc_4_32CD:
        push   1
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   sub_4_0080
loc_4_32DB:
        mov    al, byte ptr [edi]
        cmp    al, 0ah
        jne    SHORT loc_4_3313
        mov    dword ptr [esi + 15eh], 0ah
        mov    dword ptr [esi + 132h], 158h
        mov    dword ptr [esi + 136h], 10h
        mov    word ptr [esi + 118h], 3ch
        mov    word ptr [esi + 11ah], 5eah
        jmp    SHORT loc_4_3376
loc_4_3313:
        cmp    al, 64h
        jne    SHORT loc_4_336C
        mov    dword ptr [esi + 15eh], 64h
        mov    dword ptr [esi + 132h], 7f48h
        mov    dword ptr [esi + 136h], 200h
        mov    word ptr [esi + 118h], 3ch
        mov    word ptr [esi + 11ah], 5eah
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 1
        sbb    al, al
        inc    al
        mov    byte ptr [esi + 185h], al
        cmp    byte ptr [edi], 64h
        jne    SHORT loc_4_3376
        test   al, al
        je     SHORT loc_4_3376
        mov    edx, dword ptr [esi + 1ch]
        mov    eax, 81h
        out    dx, ax
        jmp    SHORT loc_4_3376
loc_4_336C:
        mov    dword ptr [esi + 15eh], 0
loc_4_3376:
        mov    edx, dword ptr [esi + 0ch]
        xor    eax, eax
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3390:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    byte ptr [ebp - 1], al
        test   edi, 2
        mov    dword ptr [esi + 13eh], eax
        mov    dword ptr [esi + 110h], edi
        je     SHORT loc_4_33BC
        mov    byte ptr [ebp - 1], 2
loc_4_33BC:
        mov    eax, edi
        and    eax, 4
        mov    dword ptr [ebp - 8], eax
        jne    SHORT loc_4_33CE
        test   edi, 8
        je     SHORT loc_4_33D2
loc_4_33CE:
        or     byte ptr [ebp - 1], 4
loc_4_33D2:
        mov    eax, edi
        and    eax, 10h
        mov    dword ptr [ebp - 0ch], eax
        je     SHORT loc_4_33E0
        or     byte ptr [ebp - 1], 7
loc_4_33E0:
        test   edi, 20h
        je     SHORT loc_4_33EC
        or     byte ptr [ebp - 1], 8
loc_4_33EC:
        push   esi
        call   sub_4_3FF0
        mov    edx, dword ptr [esi + 0ch]
        xor    eax, eax
        out    dx, ax
        mov    edx, dword ptr [esi + 24h]
        mov    eax, 0ffffh
        mov    ecx, dword ptr [esi + 11eh]
        xor    ebx, ebx
        out    dx, ax
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, 1
        out    dx, ax
loc_4_3415:
        mov    al, byte ptr [ecx]
        mov    edx, dword ptr [esi + 24h]
        add    edx, ebx
        inc    ecx
        out    dx, al
        inc    ebx
        cmp    ebx, 6
        jl     SHORT loc_4_3415
        test   edi, 8
        jne    SHORT loc_4_3452
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_4_3452
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_3463
        lea    edi, [esi + 122h]
        xor    ebx, ebx
        mov    ecx, dword ptr [esi + 44h]
loc_4_3443:
        lea    edx, [ecx + ebx]
        mov    al, byte ptr [edi]
        inc    ebx
        inc    edi
        out    dx, al
        cmp    ebx, 8
        jl     SHORT loc_4_3443
        jmp    SHORT loc_4_3463
loc_4_3452:
        xor    edi, edi
        mov    ecx, dword ptr [esi + 44h]
loc_4_3457:
        lea    edx, [ecx + edi]
        mov    al, 0ffh
        out    dx, al
        inc    edi
        cmp    edi, 8
        jl     SHORT loc_4_3457
loc_4_3463:
        cmp    byte ptr [esi + 10eh], 64h
        jne    NEAR PTR loc_4_3618
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, 4
        out    dx, ax
        mov    edx, dword ptr [esi + 2ch]
        xor    edi, edi
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        mov    eax, dword ptr [esi + 104h]
        or     al, 1
        mov    edx, dword ptr [esi + 2ch]
        out    dx, ax
loc_4_3495:
        mov    edx, dword ptr [esi + 2ch]
        xor    ebx, ebx
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        mov    eax, dword ptr [esi + 104h]
        and    eax, 0ffe7h
        mov    edx, dword ptr [esi + 2ch]
        out    dx, ax
loc_4_34B3:
        push   61h
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   sub_4_0260
        mov    edx, dword ptr [esi + 2ch]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        test   al, 62h
        je     SHORT loc_4_34DA
        inc    ebx
        cmp    ebx, 0fa0h
        jl     SHORT loc_4_34B3
loc_4_34DA:
        mov    edx, dword ptr [esi + 2ch]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        mov    eax, dword ptr [esi + 104h]
        mov    edx, dword ptr [esi + 2ch]
        or     al, 10h
        and    eax, 0ffffh
        out    dx, ax
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        xor    ebx, ebx
        mov    eax, dword ptr [esi + 104h]
        mov    edx, dword ptr [esi + 2ch]
        or     al, 8
        out    dx, ax
loc_4_3510:
        push   61h
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   sub_4_0260
        mov    edx, dword ptr [esi + 2ch]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        mov    eax, dword ptr [esi + 104h]
        and    al, 62h
        cmp    al, 62h
        je     SHORT loc_4_353F
        inc    ebx
        cmp    ebx, 1312d00h
        jl     SHORT loc_4_3510
loc_4_353F:
        cmp    ebx, 1312d00h
        jl     SHORT loc_4_3551
        inc    edi
        cmp    edi, 5
        jl     NEAR PTR loc_4_3495
loc_4_3551:
        cmp    edi, 5
        jl     NEAR PTR loc_4_3618
        mov    edx, dword ptr [esi + 2ch]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        test   al, 20h
        jne    SHORT loc_4_3577
        push   1
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_3597
loc_4_3577:
        test   al, 2
        jne    SHORT loc_4_3588
        push   1
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_3597
loc_4_3588:
        test   al, 40h
        jne    SHORT loc_4_35A5
        push   1
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
loc_4_3597:
        push   1
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   sub_4_0080
loc_4_35A5:
        mov    edx, dword ptr [esi + 2ch]
        xor    edi, edi
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        mov    eax, dword ptr [esi + 104h]
        and    eax, 0ffe7h
        mov    edx, dword ptr [esi + 2ch]
        out    dx, ax
loc_4_35C3:
        push   61h
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   sub_4_0260
        mov    edx, dword ptr [esi + 2ch]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        test   al, 62h
        je     SHORT loc_4_35EA
        inc    edi
        cmp    edi, 0fa0h
        jl     SHORT loc_4_35C3
loc_4_35EA:
        mov    edx, dword ptr [esi + 2ch]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        mov    eax, dword ptr [esi + 104h]
        or     al, 8
        mov    edx, dword ptr [esi + 2ch]
        out    dx, ax
        mov    edx, dword ptr [esi + 0ch]
        xor    eax, eax
        out    dx, ax
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_3618:
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, 4
        out    dx, ax
        mov    edx, dword ptr [esi + 34h]
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    eax, dword ptr [esi + 104h]
        mov    edx, dword ptr [esi + 34h]
        or     al, byte ptr [ebp - 1]
        or     al, 30h
        out    dx, al
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    eax, dword ptr [esi + 104h]
        sub    al, 30h
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        test   al, 30h
        je     SHORT loc_4_3668
        mov    edx, dword ptr [esi + 0ch]
        xor    eax, eax
        out    dx, ax
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_3668:
        mov    word ptr [esi], 8
        xor    eax, eax
        mov    edx, dword ptr [esi + 0ch]
        pop    edi
        out    dx, ax
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 200h
        out    dx, ax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3690:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 8
        jne    SHORT loc_4_36AF
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_36AF:
        mov    edx, dword ptr [esi + 54h]
        in     eax, dx
        mov    dword ptr [esi + 104h], eax
        test   eax, 80000000h
        jne    SHORT loc_4_3707
        and    eax, 7ffffffeh
        inc    dword ptr [esi + 17ah]
        shr    eax, 1
        cmp    eax, dword ptr [ebp + 14h]
        jae    SHORT loc_4_3707
        mov    eax, 11h
        inc    dword ptr [esi + 176h]
        mov    byte ptr [esi + 14ah], 1
        mov    dword ptr [esi + 13eh], 2
        mov    edx, dword ptr [esi + 24h]
        pop    edi
        out    dx, ax
        mov    edx, dword ptr [esi + 2ch]
        pop    esi
        out    dx, ax
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3707:
        mov    dl, byte ptr [esi + 146h]
        cmp    dl, 0ffh
        lea    eax, [edx + 1]
        mov    byte ptr [esi + 146h], al
        jne    SHORT loc_4_376D
        mov    edx, dword ptr [esi + 68h]
        in     al, dx
        mov    byte ptr [esi + 104h], al
        cmp    al, 0ffh
        mov    byte ptr [esi + 146h], al
        jne    SHORT loc_4_3763
        mov    eax, 11h
        inc    dword ptr [esi + 176h]
        mov    dword ptr [esi + 13eh], 2
        mov    byte ptr [esi + 14ah], 1
        mov    edx, dword ptr [esi + 24h]
        pop    edi
        out    dx, ax
        mov    edx, dword ptr [esi + 2ch]
        pop    esi
        out    dx, ax
        mov    eax, 1
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3763:
        test   al, al
        je     SHORT loc_4_376D
        inc    dword ptr [esi + 17ah]
loc_4_376D:
        mov    ecx, dword ptr [esi + 44h]
        mov    eax, dword ptr [ebp + 14h]
        mov    edx, ecx
        cmp    eax, 3ch
        ja     SHORT loc_4_377F
        mov    eax, 3ch
loc_4_377F:
        out    dx, eax
        mov    edx, dword ptr [esi + 34h]
        mov    edi, dword ptr [ebp + 10h]
        mov    ax, di
        test   byte ptr [esi + 14ch], 4
        out    dx, ax
        je     SHORT loc_4_37C0
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 2000h
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 114h]
        out    dx, ax
        mov    eax, edi
        and    eax, 0fffffffch
        push   eax
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_37B4:
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 2100h
        out    dx, ax
        jmp    SHORT loc_4_37D0
loc_4_37C0:
        mov    eax, edi
        mov    ebx, dword ptr [ebp + 0ch]
        shr    eax, 2
        push   eax
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_37D0:
        mov    eax, edi
        and    eax, 0fffffffch
        add    eax, ebx
        test   edi, 2
        je     SHORT loc_4_37F8
        mov    cx, word ptr [eax]
        add    eax, 2
        test   edi, 1
        mov    word ptr [ebp - 4], cx
        je     SHORT loc_4_3805
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 2], al
        jmp    SHORT loc_4_3805
loc_4_37F8:
        test   edi, 1
        je     SHORT loc_4_380C
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 4], al
loc_4_3805:
        mov    edx, dword ptr [esi + 44h]
        mov    eax, dword ptr [ebp - 4]
        out    dx, eax
loc_4_380C:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [esi + 142h], eax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_4_3820:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 10h]
        mov    edx, dword ptr [esi + 34h]
        mov    ax, di
        test   byte ptr [esi + 14ch], 4
        out    dx, ax
        je     SHORT loc_4_386C
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 2000h
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 114h]
        out    dx, ax
        mov    eax, edi
        and    eax, 0fffffffch
        push   eax
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3860:
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 2100h
        out    dx, ax
        jmp    SHORT loc_4_387F
loc_4_386C:
        mov    eax, edi
        mov    ebx, dword ptr [ebp + 0ch]
        shr    eax, 2
        mov    ecx, dword ptr [esi + 44h]
        push   eax
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_387F:
        mov    eax, edi
        and    eax, 0fffffffch
        add    eax, ebx
        test   edi, 2
        je     SHORT loc_4_38B7
        mov    cx, word ptr [eax]
        add    eax, 2
        test   edi, 1
        mov    word ptr [ebp - 4], cx
        je     SHORT loc_4_38A5
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 2], al
loc_4_38A5:
        mov    edx, dword ptr [esi + 44h]
        mov    eax, dword ptr [ebp - 4]
        out    dx, eax
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_38B7:
        test   edi, 1
        je     SHORT loc_4_38CB
        mov    al, byte ptr [eax]
        mov    edx, dword ptr [esi + 44h]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        out    dx, eax
loc_4_38CB:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_38E0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 142h]
        cmp    ecx, 3ch
        jae    SHORT loc_4_3918
        mov    eax, 3ch
        mov    edx, dword ptr [esi + 34h]
        sub    eax, ecx
        out    dx, ax
        lea    ecx, [eax + 3]
        shr    ecx, 2
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     SHORT loc_4_3918
        mov    edx, dword ptr [esi + 44h]
loc_4_390E:
        xor    eax, eax
        out    dx, eax
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        jne    SHORT loc_4_390E
loc_4_3918:
        mov    dword ptr [esi + 142h], 0
        mov    edx, dword ptr [esi + 1ch]
        mov    eax, 3
        out    dx, ax
        xor    eax, eax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3940:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebx + 44h]
        mov    ecx, dword ptr [ebp + 10h]
        in     eax, dx
        mov    dword ptr [ebx + 104h], eax
        mov    dword ptr [ebp - 8], eax
        and    eax, 0fffh
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebx + 12ah], eax
        cmp    eax, ecx
        jb     SHORT loc_4_3970
        mov    eax, ecx
loc_4_3970:
        mov    dword ptr [ebx + 12eh], eax
        xor    eax, eax
        mov    edx, dword ptr [ebx + 3ch]
        out    dx, ax
        mov    edx, dword ptr [ebx + 4]
        in     ax, dx
        mov    word ptr [ebx + 104h], ax
        mov    edx, dword ptr [ebx + 4]
        in     ax, dx
        mov    word ptr [ebx + 104h], ax
        test   byte ptr [ebx + 14ch], 4
        je     SHORT loc_4_39CE
        mov    edx, dword ptr [ebx + 14h]
        mov    eax, 2000h
        out    dx, ax
        mov    eax, dword ptr [ebx + 12eh]
        mov    ecx, dword ptr [ebx + 114h]
        and    eax, 0fffffffch
        mov    edx, dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_39C2:
        mov    edx, dword ptr [ebx + 14h]
        mov    eax, 2100h
        out    dx, ax
        jmp    SHORT loc_4_39E5
loc_4_39CE:
        mov    eax, dword ptr [ebx + 12eh]
        mov    edx, dword ptr [ebp + 0ch]
        shr    eax, 2
        mov    ecx, dword ptr [ebx + 44h]
        push   eax
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_39E5:
        mov    eax, dword ptr [ebx + 12eh]
        mov    ecx, eax
        and    ecx, 0fffffffch
        add    ecx, dword ptr [ebp + 0ch]
        test   al, 2
        je     SHORT loc_4_3A13
        mov    edx, dword ptr [ebx + 44h]
        inc    ecx
        in     al, dx
        inc    ecx
        mov    byte ptr [ebx + 104h], al
        mov    byte ptr [ecx - 2], al
        mov    edx, dword ptr [ebx + 44h]
        in     al, dx
        mov    byte ptr [ebx + 104h], al
        mov    byte ptr [ecx - 1], al
loc_4_3A13:
        test   byte ptr [ebx + 12eh], 1
        je     SHORT loc_4_3A28
        mov    edx, dword ptr [ebx + 44h]
        in     al, dx
        mov    byte ptr [ebx + 104h], al
        mov    byte ptr [ecx], al
loc_4_3A28:
        shr    dword ptr [ebp - 8], 10h
        mov    ecx, dword ptr [ebx + 110h]
        test   cl, 20h
        je     NEAR PTR loc_4_3AD6
        mov    eax, dword ptr [ebp - 8]
        test   dword ptr [ebx + 132h], eax
        je     NEAR PTR loc_4_3AD6
        mov    edx, eax
        and    edx, 8
        jne    SHORT loc_4_3A84
        test   dword ptr [ebx + 136h], eax
        jne    SHORT loc_4_3A84
        test   al, 40h
        mov    eax, dword ptr [ebp + 14h]
        je     SHORT loc_4_3A72
        pop    edi
        mov    dword ptr [eax], 208h
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3A72:
        pop    edi
        mov    dword ptr [eax], 201h
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3A84:
        mov    eax, dword ptr [ebx + 136h]
        mov    ecx, dword ptr [ebp - 8]
        or     eax, 8
        and    ecx, eax
        cmp    ecx, eax
        jne    SHORT loc_4_3AAB
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 206h
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3AAB:
        mov    eax, dword ptr [ebp + 14h]
        test   edx, edx
        je     SHORT loc_4_3AC4
        pop    edi
        mov    dword ptr [eax], 202h
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3AC4:
        pop    edi
        mov    dword ptr [eax], 204h
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3AD6:
        xor    eax, eax
        mov    ax, word ptr [ebx + 11ah]
        cmp    eax, dword ptr [ebx + 12ah]
        jae    SHORT loc_4_3B10
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], 208h
        test   byte ptr [ebx + 110h], 20h
        je     SHORT loc_4_3B05
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3B05:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3B10:
        mov    eax, dword ptr [ebx + 11eh]
        mov    edx, dword ptr [ebp + 0ch]
        test   byte ptr [edx], 1
        jne    SHORT loc_4_3B8B
        mov    eax, dword ptr [ebp - 8]
        and    al, 3
        cmp    al, 1
        jne    SHORT loc_4_3B66
        test   cl, 1
        je     SHORT loc_4_3B41
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3B41:
        mov    eax, dword ptr [ebp + 14h]
        test   cl, 10h
        mov    dword ptr [eax], 100h
        je     SHORT loc_4_3B5B
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3B5B:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3B66:
        mov    eax, dword ptr [ebp + 14h]
        test   cl, 10h
        mov    dword ptr [eax], 104h
        je     SHORT loc_4_3B80
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3B80:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3B8B:
        mov    edx, dword ptr [ebp - 8]
        and    dl, 3
        cmp    dl, 3
        jne    SHORT loc_4_3BD5
        test   cl, 2
        je     SHORT loc_4_3BB0
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3BB0:
        mov    eax, dword ptr [ebp + 14h]
        test   cl, 10h
        mov    dword ptr [eax], 101h
        je     SHORT loc_4_3BCA
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3BCA:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3BD5:
        xor    edx, edx
        cmp    word ptr [ebx + 11ch], dx
        jbe    SHORT loc_4_3C08
loc_4_3BE0:
        add    eax, 6
        mov    ecx, 6
        mov    esi, eax
        mov    edi, dword ptr [ebp + 0ch]
        repe cmpsb
        jne    SHORT loc_4_3BFA
        test   byte ptr [ebx + 110h], 4
        jne    SHORT loc_4_3C26
loc_4_3BFA:
        inc    edx
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 11ch]
        cmp    ecx, edx
        jg     SHORT loc_4_3BE0
loc_4_3C08:
        mov    eax, dword ptr [ebx + 110h]
        test   al, 18h
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], 102h
        jne    SHORT loc_4_3C3B
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3C26:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 2
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3C3B:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3C50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 14h]
        test   edi, edi
        jne    SHORT loc_4_3C66
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_4_3C66:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    ebx, dword ptr [ebp + 10h]
        test   bl, 1
        mov    edx, dword ptr [esi + 3ch]
        out    dx, ax
        mov    edx, dword ptr [esi + 4]
        in     ax, dx
        mov    word ptr [esi + 104h], ax
        je     SHORT loc_4_3C99
        mov    edx, dword ptr [esi + 44h]
        inc    ebx
        in     al, dx
        dec    edi
        mov    byte ptr [esi + 104h], al
        mov    byte ptr [ebx - 1], al
loc_4_3C99:
        test   bl, 2
        je     SHORT loc_4_3CBD
        mov    edx, dword ptr [esi + 44h]
        inc    ebx
        in     al, dx
        inc    ebx
        mov    byte ptr [esi + 104h], al
        sub    edi, 2
        mov    byte ptr [ebx - 2], al
        mov    edx, dword ptr [esi + 44h]
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    byte ptr [ebx - 1], al
loc_4_3CBD:
        test   byte ptr [esi + 14ch], 4
        je     SHORT loc_4_3CEF
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 2000h
        out    dx, ax
        mov    eax, edi
        mov    ecx, dword ptr [esi + 114h]
        and    eax, 0fffffffch
        push   eax
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3CE3:
        mov    edx, dword ptr [esi + 14h]
        mov    eax, 2100h
        out    dx, ax
        jmp    SHORT loc_4_3CFF
loc_4_3CEF:
        mov    eax, edi
        mov    ecx, dword ptr [esi + 44h]
        shr    eax, 2
        push   eax
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3CFF:
        mov    eax, edi
        and    eax, 0fffffffch
        add    ebx, eax
        test   edi, 2
        je     SHORT loc_4_3D2A
        mov    edx, dword ptr [esi + 44h]
        inc    ebx
        in     al, dx
        inc    ebx
        mov    byte ptr [esi + 104h], al
        mov    byte ptr [ebx - 2], al
        mov    edx, dword ptr [esi + 44h]
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    byte ptr [ebx - 1], al
loc_4_3D2A:
        test   edi, 1
        je     SHORT loc_4_3D3E
        mov    edx, dword ptr [esi + 44h]
        in     al, dx
        mov    byte ptr [esi + 104h], al
        mov    byte ptr [ebx], al
loc_4_3D3E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3D50:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, 8
        push   edi
        mov    edi, dword ptr [ebp + 8]
        add    edi, 162h
        rep stosd
        mov    eax, 30h
        pop    edi
        pop    ebp
        ret    4
sub_4_3D70:
        push   ebp
        mov    ecx, 0ch
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 152h
        mov    edi, dword ptr [ebp + 0ch]
        rep movsd
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_3D90:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 8
        je     SHORT loc_4_3E12
        mov    eax, dword ptr [esi + 13eh]
        test   eax, eax
        jle    SHORT loc_4_3E12
        dec    eax
        mov    dword ptr [esi + 13eh], eax
        test   eax, eax
        jg     SHORT loc_4_3E12
        push   esi
        call   sub_4_2FD0
        inc    dword ptr [esi + 17eh]
        mov    eax, dword ptr [esi + 110h]
        push   eax
        push   esi
        call   sub_4_3390
        test   eax, eax
        je     SHORT loc_4_3DEC
        push   0
        mov    eax, dword ptr [esi + 108h]
        push   0
        push   0ah
        db  068h, 000h, 000h, 000h, 000h
        push   1
        push   eax
        call   sub_4_0080
        pop    esi
        pop    ebp
        ret    4
loc_4_3DEC:
        push   10h
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   sub_4_1260
        cmp    eax, -1
        jge    SHORT loc_4_3E12
loc_4_3DFF:
        push   0
        mov    eax, dword ptr [esi + 108h]
        push   eax
        call   sub_4_1260
        cmp    eax, -1
        jl     SHORT loc_4_3DFF
loc_4_3E12:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh
        db  0FFh, 080h, 06Ah, 001h, 000h, 000h, 066h, 0C7h, 000h, 008h, 000h, 05Dh, 0C2h, 008h, 000h, 08Dh
        db  049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3E40:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 108h]
        push   eax
        call   0
        mov    edx, dword ptr [edi + 64h]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 3], 0
        mov    byte ptr [ebp - 1], 1
        in     ax, dx
        mov    word ptr [edi + 104h], ax
        mov    bl, al
loc_4_3E71:
        test   bl, bl
        je     NEAR PTR loc_4_3F11
loc_4_3E79:
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 1
        cmp    dword ptr [edi + 110h], 0
        mov    eax, 2
        je     SHORT loc_4_3E95
        push   esi
        call   sub_4_4290
loc_4_3E95:
        cmp    eax, 1
        jne    SHORT loc_4_3ED5
        mov    edx, dword ptr [edi + 1ch]
        mov    eax, 5
        out    dx, ax
        in     ax, dx
        mov    word ptr [edi + 104h], ax
        mov    edx, dword ptr [edi + 1ch]
        in     ax, dx
        mov    word ptr [edi + 104h], ax
        dec    bl
        mov    edx, dword ptr [edi + 1ch]
        in     ax, dx
        mov    word ptr [edi + 104h], ax
        mov    edx, dword ptr [edi + 1ch]
        in     ax, dx
        mov    word ptr [edi + 104h], ax
        jmp    SHORT loc_4_3F15
loc_4_3ED5:
        cmp    eax, 2
        je     SHORT loc_4_3EE3
        test   eax, eax
        jge    SHORT loc_4_3F15
        jmp    NEAR PTR loc_4_3F96
loc_4_3EE3:
        mov    edx, dword ptr [edi + 64h]
        in     ax, dx
        mov    word ptr [edi + 104h], ax
        test   al, al
        je     SHORT loc_4_3EFF
        mov    edx, dword ptr [edi + 1ch]
        mov    eax, 5
        out    dx, ax
        jmp    SHORT loc_4_3EE3
loc_4_3EFF:
        mov    edx, dword ptr [edi + 24h]
        mov    eax, 700h
        xor    bl, bl
        inc    dword ptr [edi + 172h]
        out    dx, ax
loc_4_3F11:
        mov    byte ptr [ebp - 2], 1
loc_4_3F15:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_4_3F64
        mov    dword ptr [edi + 13eh], 0
        mov    byte ptr [edi + 14ah], 0
loc_4_3F2C:
        push   0
        push   esi
        call   sub_4_1260
        mov    byte ptr [edi + 14bh], al
        and    eax, 0ffh
        cmp    eax, -1
        jl     SHORT loc_4_3F2C
        cmp    byte ptr [edi + 14bh], 0
        jne    SHORT loc_4_3F5C
        xor    cl, cl
        mov    eax, 1
        mov    byte ptr [ebp - 1], al
        mov    byte ptr [ebp - 3], al
        jmp    SHORT loc_4_3F6C
loc_4_3F5C:
        mov    byte ptr [ebp - 1], 0
        mov    cl, 1
        jmp    SHORT loc_4_3F6C
loc_4_3F64:
        mov    ecx, 1
        mov    byte ptr [ebp - 1], cl
loc_4_3F6C:
        test   bl, bl
        jne    SHORT loc_4_3F85
        mov    edx, dword ptr [edi + 64h]
        in     ax, dx
        mov    bx, ax
        mov    word ptr [edi + 104h], ax
        mov    byte ptr [edi + 146h], bh
loc_4_3F85:
        test   byte ptr [ebp - 2], cl
        je     NEAR PTR loc_4_3E71
        test   bl, bl
        jne    NEAR PTR loc_4_3E79
loc_4_3F96:
        mov    edx, dword ptr [edi + 24h]
        mov    eax, 700h
        cmp    byte ptr [edi + 14ah], 0
        out    dx, ax
        jne    SHORT loc_4_3FB3
        mov    edx, dword ptr [edi + 2ch]
        mov    eax, 10h
        out    dx, ax
loc_4_3FB3:
        mov    eax, dword ptr [edi + 108h]
        push   eax
        call   sub_4_0010
        xor    eax, eax
        pop    edi
        mov    al, byte ptr [ebp - 3]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_4_3FD0:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_4_3FE3
        mov    eax, 3fh
        pop    ebp
        ret    8
loc_4_3FE3:
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 110h]
        ret    8
sub_4_3FF0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    byte ptr [eax + 184h], 0
        mov    edx, dword ptr [eax + 14h]
        pop    ebp
        mov    eax, 4
        out    dx, ax
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_4010:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    byte ptr [eax + 184h], 1
        mov    edx, dword ptr [eax + 14h]
        pop    ebp
        mov    eax, 5
        out    dx, ax
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_4030:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 12ah]
        mov    edx, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [edx], ecx
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4050:
        push   ebp
        mov    edx, 8
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        add    ecx, 7
loc_4_4064:
        mov    esi, eax
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        sar    eax, 4
        and    esi, 0fh
        dec    ecx
        mov    bl, byte ptr [esi + edi]
        dec    edx
        mov    byte ptr [ecx + 1], bl
        jne    SHORT loc_4_4064
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4090:
        push   ebp
        mov    ebp, esp
        sub    esp, 108h
        push   ebx
        mov    ecx, dword ptr [ebp + 10h]
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ecx + 14h], esi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 10h], esi
        mov    dword ptr [ecx], esi
        cmp    eax, 3
        mov    dword ptr [ecx + 18h], 0ffffffffh
        mov    dword ptr [ecx + 4], 0ffffffffh
        jl     NEAR PTR loc_4_4264
        cmp    eax, 0fh
        jg     NEAR PTR loc_4_4264
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    ebx, edx
        and    ebx, 3fh
        jne    SHORT loc_4_40EE
        cmp    edx, 100h
        jl     SHORT loc_4_40EE
        cmp    edx, 3c0h
        jle    SHORT loc_4_410D
loc_4_40EE:
        mov    eax, edx
        and    eax, 0ffff0fffh
        cmp    eax, 0c00h
        jne    NEAR PTR loc_4_4247
        test   ebx, ebx
        jne    NEAR PTR loc_4_4247
        mov    edi, 38h
loc_4_410D:
        lea    eax, [edx + 3fh]
        mov    dword ptr [ecx + 8], edx
        mov    dword ptr [ecx + 0ch], eax
        xor    eax, eax
loc_4_4118:
        lea    ebx, [edx + eax]
        inc    eax
        add    ebx, edi
        cmp    eax, 40h
        mov    dword ptr [ebp + eax*4 - 10ch], ebx
        jl     SHORT loc_4_4118
        mov    edx, dword ptr [ebp - 108h]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        cmp    al, 50h
        jne    NEAR PTR loc_4_4223
        mov    edx, dword ptr [ebp - 108h]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        cmp    al, 48h
        jne    NEAR PTR loc_4_4223
        mov    edx, dword ptr [ebp - 108h]
        add    edx, 2
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 8]
        and    al, 0f0h
        cmp    al, 50h
        jne    NEAR PTR loc_4_4223
        mov    edx, dword ptr [ebp - 108h]
        add    edx, 3
        in     al, dx
        mov    byte ptr [ebp - 8], al
        cmp    al, 53h
        jne    NEAR PTR loc_4_4223
        mov    edx, dword ptr [ebp - 100h]
        mov    eax, 6
        out    dx, ax
        xor    edi, edi
loc_4_418D:
        mov    edx, dword ptr [ebp - 0c8h]
        add    edx, edi
        inc    edi
        in     al, dx
        mov    byte ptr [ebp - 8], al
        and    eax, 0ffh
        add    esi, eax
        cmp    edi, 6
        jl     SHORT loc_4_418D
        mov    edx, dword ptr [ebp - 0b0h]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        and    eax, 0ffh
        add    eax, esi
        cmp    al, 0ffh
        jne    SHORT loc_4_4219
        mov    dword ptr [ebp - 4], 1
        mov    edx, dword ptr [ebp - 0f8h]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        test   al, 40h
        je     SHORT loc_4_4219
        mov    edx, dword ptr [ebp - 100h]
        mov    eax, 2
        out    dx, ax
        mov    edx, dword ptr [ebp - 0e0h]
        in     ax, dx
        mov    si, ax
        mov    word ptr [ebp - 8], ax
        and    esi, 0ffh
        shl    esi, 10h
        mov    edx, dword ptr [ebp - 0e8h]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ff00h
        or     esi, eax
        mov    dword ptr [ecx + 10h], esi
        add    esi, 1fffh
        mov    dword ptr [ecx + 14h], esi
loc_4_4219:
        mov    edx, dword ptr [ebp - 100h]
        xor    eax, eax
        out    dx, ax
loc_4_4223:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_4237
        db  0A1h, 008h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_4237:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    dword ptr [ecx], eax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_4247:
        db  0A1h, 004h, 000h, 000h, 000h
        add    eax, 1eh
        push   eax
        push   edx
        call   sub_4_4050
        db  0A1h, 004h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_4264:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        add    eax, 16h
        push   eax
        push   edx
        call   sub_4_4050
        db  0A1h, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_4290:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0b7h]
        inc    eax
        mov    dword ptr [esi + 0b7h], eax
        cmp    eax, 0ah
        jbe    SHORT loc_4_42CD
        cmp    dword ptr [esi + 2ch], 0
        je     SHORT loc_4_42CD
        push   0
        push   esi
        call   sub_4_1260
        cmp    eax, -1
        jne    SHORT loc_4_42CD
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_42CD:
        lea    edi, [esi + 0c1h]
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [esi]
        push   0fch
        push   edi
        push   ecx
        call   sub_4_3940
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jg     SHORT loc_4_433F
        add    eax, 3
        cmp    eax, 3
        ja     SHORT loc_4_433F
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_42FB:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_4305:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_430F:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_431C:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_4_4650
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        call   sub_4_4030
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_433F:
        sub    dword ptr [ebp - 4], 0eh
        mov    ecx, dword ptr [esi + 7fh]
        mov    eax, dword ptr [ebp - 4]
        cmp    ecx, eax
        jb     SHORT loc_4_434F
        mov    ecx, eax
loc_4_434F:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 20h], esi
        push   eax
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 10h], 0
        push   ecx
        inc    dword ptr [esi + 87h]
        lea    ecx, [esi + 0cfh]
        lea    edx, [ebp - 20h]
        mov    byte ptr [esi + 41h], 1
        push   ecx
        mov    eax, dword ptr [esi + 7bh]
        push   0eh
        push   edi
        push   edi
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4383:
        cmp    dword ptr [ebp - 1ch], 0
        sbb    eax, eax
        pop    edi
        inc    eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_43B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        mov    ecx, dword ptr [ebp + 1ch]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ecx], 0
        mov    dword ptr [esi + 4], 0
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_4_43FB
        push   ecx
        mov    edx, dword ptr [ebp + 10h]
        add    edx, 0eh
        mov    ecx, dword ptr [ebp + 18h]
        push   edx
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_43F0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_4_43FB:
        mov    edx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp + 14h]
        lea    edi, [edx + eax]
        cmp    edi, ecx
        jbe    SHORT loc_4_4412
        sub    ecx, edx
        mov    dword ptr [ebp - 18h], ecx
        jmp    SHORT loc_4_4415
loc_4_4412:
        mov    dword ptr [ebp - 18h], eax
loc_4_4415:
        mov    edi, dword ptr [ebp - 18h]
        add    edx, 0eh
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        mov    dword ptr [ebp - 8], edx
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4431:
        test   edi, edi
        je     SHORT loc_4_4484
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        mov    ebx, edi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4448:
        mov    eax, dword ptr [ebp - 10h]
        cmp    ebx, eax
        jb     SHORT loc_4_4451
        mov    ebx, eax
loc_4_4451:
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        mov    eax, dword ptr [edx]
        push   eax
        call   sub_4_3C50
        mov    dword ptr [esi + 4], eax
        add    dword ptr [ebp - 8], ebx
        sub    edi, ebx
        je     SHORT loc_4_4484
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_447E:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_4431
loc_4_4484:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 1ch]
        sub    eax, edi
        pop    edi
        pop    esi
        mov    dword ptr [ecx], eax
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_44A0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 38h]
        test   eax, eax
        je     SHORT loc_4_44C2
loc_4_44AE:
        mov    ecx, dword ptr [eax + 24h]
        push   eax
        push   esi
        mov    dword ptr [esi + 38h], ecx
        call   sub_4_44D0
        mov    eax, dword ptr [esi + 38h]
        test   eax, eax
        jne    SHORT loc_4_44AE
loc_4_44C2:
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_44D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_44EC:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [esi + 7fh]
        add    ebx, 0eh
        cmp    ebx, eax
        jb     SHORT loc_4_44FE
        mov    ebx, eax
loc_4_44FE:
        lea    eax, [ebx - 0eh]
        push   0
        lea    ecx, [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    dword ptr [ebp - 4], 0eh
        push   ecx
        push   0
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4518:
        lea    ecx, [ebp - 14h]
        lea    edx, [ebp - 10h]
        push   ecx
        lea    eax, [ebp - 0ch]
        push   edx
        lea    ecx, [esi + 0c1h]
        mov    dword ptr [ebp - 10h], 0
        push   eax
        push   ebx
        push   ecx
        call   sub_4_45A0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        push   ecx
        lea    edx, [esi + 0cfh]
        push   eax
        mov    dword ptr [ebp - 28h], esi
        push   edx
        mov    dword ptr [ebp - 20h], ecx
        push   0eh
        lea    ecx, [esi + 0c1h]
        push   ecx
        lea    edx, [ebp - 28h]
        push   ecx
        mov    dword ptr [ebp - 18h], edi
        push   edx
        mov    ecx, dword ptr [esi + 7bh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4567:
        push   0
        inc    dword ptr [esi + 83h]
        mov    byte ptr [esi + 41h], 1
        mov    esi, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [esi + 5]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4580:
        push   esi
        call   sub_4_18F0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_45A0:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        lea    edx, [ebp - 4]
        push   ecx
        mov    esi, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_45C6:
        mov    ebx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp + 14h]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_4644
loc_4_45D3:
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [eax]
        lea    edx, [ebx + ecx]
        cmp    edx, dword ptr [ebp + 0ch]
        jae    SHORT loc_4_461E
        push   ebx
        mov    eax, dword ptr [edi]
        add    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_45F0:
        mov    ecx, dword ptr [ebp + 18h]
        push   esi
        add    dword ptr [ecx], ebx
        mov    edx, dword ptr [esi]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_45FE:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        push   ecx
        mov    dword ptr [edi], 0
        push   edx
        mov    eax, dword ptr [esi]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_4614:
        mov    ebx, dword ptr [ebp - 8]
        sub    ebx, dword ptr [edi]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_4_45D3
loc_4_461E:
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_4644
        mov    eax, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [eax]
        sub    esi, eax
        add    ecx, dword ptr [ebp - 4]
        push   esi
        add    eax, dword ptr [ebp + 8]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_463D:
        mov    ecx, dword ptr [ebp + 18h]
        add    dword ptr [ecx], esi
        add    dword ptr [edi], esi
loc_4_4644:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h
sub_4_4650:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        test   eax, 208h
        jne    SHORT loc_4_469C
        test   eax, 206h
        je     SHORT loc_4_4678
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 97h]
        inc    dword ptr [eax + 9bh]
        ret    8
loc_4_4678:
        test   eax, 204h
        je     SHORT loc_4_468C
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        inc    dword ptr [eax + 97h]
        ret    8
loc_4_468C:
        test   eax, 202h
        je     SHORT loc_4_469C
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 9bh]
loc_4_469C:
        pop    ebp
        ret    8
_PTEXT ends
        end
