; cdfs_pcode_03_01.asm -- CDFS object 3 segment _PTEXT  (part 1/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_0000:
        db  0F6h, 005h, 07Ch, 008h, 000h, 000h, 080h
        jne    NEAR PTR loc_3_00A2
        db  080h, 00Dh, 07Ch, 008h, 000h, 000h, 040h
        db  0F6h, 005h, 07Ch, 008h, 000h, 000h, 020h
        je     SHORT loc_3_002C
        db  068h, 0DCh, 008h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 041h, 000h
        lea    esp, [esp + 4]
loc_3_002C:
        db  083h, 025h, 004h, 000h, 000h, 000h, 0FEh
        db  0F7h, 005h, 004h, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        je     SHORT loc_3_0050
        db  0F7h, 005h, 004h, 000h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_3_0050
        db  0E8h, 0D0h, 002h, 0FDh, 0FFh
loc_3_0050:
        db  0F7h, 005h, 004h, 000h, 000h, 000h, 000h, 020h, 000h, 000h
        je     SHORT loc_3_006B
        db  081h, 00Dh, 004h, 000h, 000h, 000h, 000h, 010h, 000h, 000h
        db  0E8h, 065h, 003h, 0FDh, 0FFh
loc_3_006B:
        call   sub_3_0450
        db  0F6h, 005h, 07Ch, 008h, 000h, 000h, 004h
        je     SHORT loc_3_0088
        db  0FFh, 035h, 098h, 008h, 000h, 000h
        call   sub_3_86F0
        lea    esp, [esp + 4]
loc_3_0088:
        db  0F6h, 005h, 07Ch, 008h, 000h, 000h, 002h
        je     SHORT loc_3_009B
        db  0A1h, 090h, 008h, 000h, 000h
        and    word ptr [eax + 4], 0fffdh
loc_3_009B:
        db  080h, 025h, 07Ch, 008h, 000h, 000h, 010h
loc_3_00A2:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_00B0:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 28h], 0
        push   1
        db  0A1h, 098h, 008h, 000h, 000h
        push   eax
        call   sub_3_8710
        mov    edi, dword ptr [esp + 38h]
        add    esp, 8
loc_3_00D3:
        db  08Bh, 00Dh, 0E0h, 09Bh, 000h, 000h
        lea    esp, [esp]
loc_3_00E0:
        mov    eax, dword ptr [esp + 28h]
        inc    dword ptr [esp + 28h]
        or     eax, eax
        je     SHORT loc_3_0123
        cmp    eax, 1
        jne    NEAR PTR loc_3_02E0
        or     edi, edi
        je     SHORT loc_3_0105
        db  0C7h, 005h, 00Ch, 09Ch, 000h, 000h, 01Bh, 000h, 000h, 000h
        jmp    SHORT loc_3_0123
loc_3_0105:
        db  0C7h, 005h, 000h, 09Ch, 000h, 000h, 020h, 000h, 000h, 000h
        db  0C7h, 005h, 004h, 09Ch, 000h, 000h, 01Bh, 000h, 000h, 000h
        db  0C7h, 005h, 008h, 09Ch, 000h, 000h, 002h, 000h, 000h, 000h
loc_3_0123:
        or     edi, edi
        je     SHORT loc_3_0145
        mov    dword ptr [esp + 10h], 930h
        db  08Bh, 01Dh, 00Ch, 09Ch, 000h, 000h
        mov    edx, ebx
        mov    dword ptr [esp + 1ch], edx
        mov    dword ptr [esp + 24h], 2
        jmp    SHORT loc_3_0167
loc_3_0145:
        mov    dword ptr [esp + 10h], 800h
        db  08Bh, 01Dh, 000h, 09Ch, 000h, 000h
        db  08Bh, 015h, 004h, 09Ch, 000h, 000h
        mov    dword ptr [esp + 1ch], edx
        db  08Bh, 015h, 008h, 09Ch, 000h, 000h
        mov    dword ptr [esp + 24h], edx
loc_3_0167:
        or     edi, edi
        je     SHORT loc_3_0180
        or     ecx, ecx
        jne    NEAR PTR loc_3_02FB
        db  0C7h, 044h, 024h, 020h, 0DCh, 09Bh, 000h, 000h
        jmp    SHORT loc_3_0188
        db  08Dh, 049h, 000h
loc_3_0180:
        db  0C7h, 044h, 024h, 020h, 0D8h, 09Bh, 000h, 000h
loc_3_0188:
        imul   ebx, dword ptr [esp + 10h]
        add    ebx, 0fffh
        shr    ebx, 0ch
        mov    eax, ebx
        shl    eax, 0ch
        sub    edx, edx
        div    dword ptr [esp + 10h]
        mov    dword ptr [esp + 14h], eax
        mov    ebp, 1
        shr    eax, 2
        push   eax
        call   sub_3_6860
        add    esp, 4
        mov    cl, al
        shl    ebp, cl
        cmp    ebp, 10h
        jae    SHORT loc_3_01C4
        mov    ebp, 10h
loc_3_01C4:
        mov    ecx, dword ptr [esp + 14h]
        lea    eax, [ecx + ecx*8]
        lea    eax, [eax + ebx*2]
        add    eax, ebp
        lea    eax, [eax*4 + 0a4h]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 20h]
        db  02Dh, 0DCh, 09Bh, 000h, 000h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0ffffff00h
        add    eax, 101h
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   eax
        call   sub_3_43B0
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        db  08Bh, 00Dh, 0E0h, 09Bh, 000h, 000h
        je     NEAR PTR loc_3_00E0
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 44h], eax
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [esi + 40h], ax
        lea    eax, [ebp - 1]
        mov    dword ptr [esi + 6ch], eax
        lea    eax, [esi + ebp*4 + 80h]
        mov    dword ptr [esi + 4], eax
        lea    eax, [eax + ebx*8]
        mov    dword ptr [esi], eax
        or     edi, edi
        jne    SHORT loc_3_025B
        mov    eax, dword ptr [esp + 18h]
        push   0
        add    eax, 0fffh
        shr    eax, 0ch
        push   eax
        mov    eax, esi
        shr    eax, 0ch
        push   eax
        call   sub_3_5150
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_02CD
loc_3_025B:
        mov    dword ptr [esi + 64h], ebx
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   ebx
        call   sub_3_04E0
        add    esp, 20h
        mov    dword ptr [esi + 68h], eax
        or     eax, eax
        jne    SHORT loc_3_02A0
        or     edi, edi
        jne    SHORT loc_3_02CD
        mov    eax, dword ptr [esp + 18h]
        push   0
        add    eax, 0fffh
        shr    eax, 0ch
        push   eax
        mov    eax, esi
        shr    eax, 0ch
        push   eax
        call   sub_3_5180
        add    esp, 0ch
        jmp    SHORT loc_3_02CD
        db  090h
loc_3_02A0:
        or     edi, edi
        je     NEAR PTR loc_3_0360
        push   0
        push   ebx
        shr    eax, 0ch
        push   eax
        call   sub_3_5150
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_3_0347
        push   0
        mov    eax, dword ptr [esi + 68h]
        push   eax
        call   sub_3_5809
        add    esp, 8
loc_3_02CD:
        push   0
        push   0
        push   esi
        call   sub_3_8750
        add    esp, 0ch
        jmp    NEAR PTR loc_3_00D3
        db  090h
loc_3_02E0:
        db  0A1h, 098h, 008h, 000h, 000h
        push   eax
        call   sub_3_8700
        add    esp, 4
        mov    eax, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_3_02FB:
        push   0
        mov    eax, dword ptr [ecx + 64h]
        push   eax
        mov    eax, dword ptr [ecx + 68h]
        shr    eax, 0ch
        push   eax
        call   sub_3_5150
        add    esp, 0ch
        or     eax, eax
        mov    esi, 8
        je     SHORT loc_3_032F
        db  0A1h, 0E0h, 09Bh, 000h, 000h
        db  0A3h, 0DCh, 09Bh, 000h, 000h
        db  0C7h, 005h, 0E0h, 09Bh, 000h, 000h, 000h, 000h, 000h, 000h
        sub    esi, esi
loc_3_032F:
        db  0A1h, 098h, 008h, 000h, 000h
        push   eax
        call   sub_3_8700
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_3_0347:
        or     ebx, ebx
        je     SHORT loc_3_0360
        sub    eax, eax
        mov    ecx, 1
loc_3_0352:
        mov    edx, dword ptr [esi + 4]
        mov    word ptr [eax + edx + 6], cx
        add    eax, 8
        dec    ebx
        jne    SHORT loc_3_0352
loc_3_0360:
        mov    ecx, dword ptr [esp + 14h]
        cmp    dword ptr [esp + 1ch], ecx
        jbe    SHORT loc_3_036E
        mov    dword ptr [esp + 1ch], ecx
loc_3_036E:
        mov    edx, dword ptr [esp + 1ch]
        cmp    dword ptr [esp + 24h], edx
        jbe    SHORT loc_3_037C
        mov    dword ptr [esp + 24h], edx
loc_3_037C:
        mov    edx, dword ptr [esi]
        mov    edi, dword ptr [esi + 68h]
        sub    ecx, ecx
        lea    ebx, [edx + 1ch]
        mov    eax, dword ptr [esp + 14h]
        sub    eax, dword ptr [esp + 1ch]
        mov    edi, edi
loc_3_0390:
        lea    ebp, [ebx + 8]
        mov    dword ptr [ebx - 1ch], ebp
        lea    ebp, [ebx - 40h]
        mov    dword ptr [ebx - 18h], ebp
        mov    dword ptr [ebx + 4], edi
        cmp    ecx, eax
        mov    byte ptr [ebx], 5
        jb     SHORT loc_3_03A9
        mov    byte ptr [ebx], 1
loc_3_03A9:
        add    edi, dword ptr [esp + 10h]
        add    ebx, 24h
        inc    ecx
        cmp    dword ptr [esp + 14h], ecx
        jae    SHORT loc_3_0390
        mov    ecx, dword ptr [esp + 14h]
        lea    eax, [ecx*4]
        mov    dword ptr [edx + 4], 0
        lea    eax, [eax + eax*8]
        add    eax, edx
        mov    dword ptr [eax], 0
        mov    byte ptr [eax + 1ch], 0
        mov    dword ptr [eax + 20h], 0ffffffffh
        mov    dword ptr [esi + 8], eax
        mov    eax, ecx
        sub    eax, dword ptr [esp + 1ch]
        lea    ecx, [eax*4]
        mov    dword ptr [esi + 38h], edx
        lea    ecx, [ecx + ecx*8]
        add    ecx, edx
        mov    dword ptr [esi + 3ch], ecx
        mov    dword ptr [esi + 34h], edx
        mov    dword ptr [esi + 30h], edx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [esi + 0ch], ecx
        mov    edx, dword ptr [esp + 1ch]
        shr    eax, 2
        mov    dword ptr [esi + 28h], edx
        mov    dword ptr [esi + 1ch], edx
        mov    edx, dword ptr [esp + 24h]
        mov    dword ptr [esi + 2ch], edx
        mov    dword ptr [esi + 60h], ecx
        mov    dword ptr [esi + 5ch], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 58h], 1f4h
        mov    dword ptr [eax], esi
        db  0A1h, 098h, 008h, 000h, 000h
        push   eax
        call   sub_3_8700
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0450:
        sub    esp, 8
        xor    eax, eax
        db  087h, 005h, 0DCh, 09Bh, 000h, 000h
        mov    dword ptr [esp], eax
        or     eax, eax
        jne    SHORT loc_3_046F
        xor    eax, eax
        db  087h, 005h, 0E0h, 09Bh, 000h, 000h
        mov    dword ptr [esp], eax
loc_3_046F:
        xor    eax, eax
        db  087h, 005h, 0D8h, 09Bh, 000h, 000h
        mov    dword ptr [esp + 4], eax
        cmp    dword ptr [esp], 0
        je     SHORT loc_3_04A9
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 68h]
        or     eax, eax
        je     SHORT loc_3_0498
        push   0
        push   eax
        call   sub_3_5809
        add    esp, 8
loc_3_0498:
        push   0
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0ch
loc_3_04A9:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_3_04D7
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 68h]
        or     eax, eax
        je     SHORT loc_3_04C6
        push   0
        push   eax
        call   sub_3_5809
        add    esp, 8
loc_3_04C6:
        push   0
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0ch
loc_3_04D7:
        add    esp, 8
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_3_04E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   dword ptr [ebp + 24h]
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0520:
        sub    esp, 4
        push   esi
        push   edi
        mov    dword ptr [esp + 8], 53464443h
        mov    ecx, dword ptr [esp + 10h]
        xor    eax, eax
        mov    ax, word ptr [ecx + 18h]
        add    ecx, 18h
        or     eax, eax
        je     SHORT loc_3_0556
        cmp    eax, 2
        je     SHORT loc_3_0560
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_3_3C40
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_3_0556:
        mov    word ptr [ecx], 0
        jmp    SHORT loc_3_058A
        db  08Dh, 049h, 000h
loc_3_0560:
        xor    eax, eax
        mov    edx, dword ptr [esp + 10h]
        lea    esi, [esp + 8]
        mov    edi, dword ptr [edx + 14h]
        mov    ecx, 5
        cmp    ecx, dword ptr [edx]
        jbe    SHORT loc_3_057A
        mov    ecx, dword ptr [edx]
        jecxz  loc_3_057E
loc_3_057A:
        dec    ecx
        rep movsb
        stosb
loc_3_057E:
        mov    dword ptr [edx], 10400ffh
        mov    word ptr [edx + 18h], 4002h
loc_3_058A:
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 1ah], 0
        mov    eax, dword ptr [esp + 10h]
        pop    edi
        movsx  eax, word ptr [eax + 1ah]
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_05B0:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 18h]
        db  08Bh, 00Dh, 08Ch, 008h, 000h, 000h
        mov    dword ptr [edx + 1], ecx
        mov    eax, dword ptr [eax + 14h]
        mov    word ptr [eax + 3], 100h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_05D0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 30h], 4000h
        mov    word ptr [eax + 28h], 0dh
        mov    word ptr [eax + 2ah], 0cd17h
        mov    ecx, dword ptr [eax + 18h]
        inc    ecx
        mov    dword ptr [eax + 20h], ecx
        call   sub_3_37B0
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0600:
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [esp + 14h]
        lea    eax, [esp + 4]
        or     dword ptr [esi + 30h], 4000h
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd09h
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 0ch
        mov    dword ptr [esp + 4], 0
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_068E
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [esp + 8]
        mov    byte ptr [eax + 1], cl
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [esp + 9]
        mov    byte ptr [eax + 2], cl
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [esp + 0ah]
        mov    byte ptr [eax + 3], cl
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [esp + 0bh]
        mov    byte ptr [eax + 4], cl
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [esp + 0ch]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [esp + 0dh]
        mov    byte ptr [eax + 6], cl
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [esp + 0eh]
        mov    byte ptr [eax + 7], cl
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [esp + 0fh]
        mov    byte ptr [eax + 8], cl
loc_3_068E:
        pop    esi
        add    esp, 0ch
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_06A0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 30h], 4000h
        mov    word ptr [eax + 28h], 0dh
        mov    word ptr [eax + 2ah], 0cd16h
        mov    ecx, dword ptr [eax + 18h]
        inc    ecx
        mov    dword ptr [eax + 20h], ecx
        call   sub_3_37B0
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_06D0:
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [esp + 14h]
        lea    eax, [esp + 4]
        or     dword ptr [esi + 30h], 4000h
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd03h
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 0ch
        mov    dword ptr [esp + 4], 0
        mov    eax, dword ptr [esi + 18h]
        push   esi
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [esp + 0ch], eax
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_0728
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esp + 0ch]
        mov    word ptr [eax + 2], cx
loc_3_0728:
        pop    esi
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_3_0730:
        sub    esp, 20h
        push   esi
        mov    esi, dword ptr [esp + 28h]
        lea    eax, [esp + 0ch]
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd0bh
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 18h
        mov    dword ptr [esp + 0ch], 0
        mov    byte ptr [esp + 1eh], 0ffh
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_07B2
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esp + 8], eax
        xor    ecx, ecx
        mov    cl, byte ptr [esp + 0ah]
        shl    ecx, 2
        xor    eax, eax
        lea    ecx, [ecx + ecx*2]
        mov    al, byte ptr [esp + 9]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        xor    eax, eax
        lea    ecx, [ecx + ecx*4]
        mov    al, byte ptr [esp + 8]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        sub    ecx, 96h
        mov    dword ptr [esp + 8], ecx
        mov    eax, dword ptr [esi + 18h]
        add    ecx, 96h
        mov    dword ptr [eax + 1], ecx
loc_3_07B2:
        pop    esi
        add    esp, 20h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_07C0:
        sub    esp, 4
        mov    eax, dword ptr [esp + 8]
        push   eax
        call   sub_3_3640
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_0821
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 10h]
        db  00Fh, 0B3h, 005h, 0F4h, 09Bh, 000h, 000h
        mov    al, 0ffh
        jb     SHORT loc_3_07E8
        mov    al, 1
loc_3_07E8:
        mov    byte ptr [esp + 3], al
        mov    cl, byte ptr [esp + 3]
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 18h]
        mov    byte ptr [eax + 1], cl
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 14h]
        mov    word ptr [eax + 3], 100h
        mov    eax, dword ptr [esp + 8]
        push   eax
        call   sub_3_3160
        add    esp, 4
        mov    eax, dword ptr [esp + 8]
        push   eax
        call   sub_3_36C0
        add    esp, 4
loc_3_0821:
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0830:
        sub    esp, 18h
        push   esi
        mov    esi, dword ptr [esp + 20h]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd0bh
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 18h
        mov    dword ptr [esp + 4], 0
        mov    byte ptr [esp + 16h], 0ffh
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_088F
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [esp + 8]
        mov    byte ptr [eax + 1], cl
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [esp + 0ch]
        mov    byte ptr [eax + 2], cl
        mov    byte ptr [esp + 13h], 0
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 3], ecx
loc_3_088F:
        pop    esi
        add    esp, 18h
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_08A0:
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [esp + 18h]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd0ch
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 10h
        mov    dword ptr [esp + 4], 0
        mov    eax, dword ptr [esi + 18h]
        push   esi
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [esp + 0ch], eax
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_08FF
        mov    byte ptr [esp + 0fh], 0
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 2], eax
        mov    eax, dword ptr [esi + 18h]
        mov    cl, byte ptr [esp + 10h]
        mov    byte ptr [eax + 6], cl
loc_3_08FF:
        pop    esi
        add    esp, 10h
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0910:
        sub    esp, 14h
        push   esi
        mov    esi, dword ptr [esp + 1ch]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd0dh
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 14h
        mov    dword ptr [esp + 4], 0
        mov    eax, dword ptr [esi + 18h]
        mov    eax, dword ptr [eax + 1]
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esi + 18h]
        mov    eax, dword ptr [eax + 9]
        mov    dword ptr [esp + 0ch], eax
        mov    eax, dword ptr [esi + 18h]
        mov    eax, dword ptr [eax + 5]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_3_5958
        mov    dword ptr [esp + 18h], eax
        mov    byte ptr [esp + 1ch], 0
        mov    byte ptr [esp + 1dh], 0
        mov    byte ptr [esp + 1eh], 0
        mov    byte ptr [esp + 1fh], 0
        add    esp, 8
        push   esi
        call   sub_3_37B0
        add    esp, 4
        pop    esi
        add    esp, 14h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0990:
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [esp + 18h]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd05h
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 10h
        mov    dword ptr [esp + 4], 0
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_09E1
        mov    edx, dword ptr [esi + 18h]
        lea    ecx, [esp + 8]
        inc    edx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [edx + 8], ax
loc_3_09E1:
        pop    esi
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_09F0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 30h], 4000h
        mov    word ptr [eax + 28h], 8
        mov    word ptr [eax + 2ah], 0
        mov    dword ptr [eax + 20h], 0
        call   sub_3_37B0
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0A20:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 1ch]
        cmp    byte ptr [ecx + 1], 0
        je     SHORT loc_3_0A35
        mov    word ptr [eax + 28h], 9
        jmp    SHORT loc_3_0A42
loc_3_0A35:
        or     dword ptr [eax + 30h], 4000h
        mov    word ptr [eax + 28h], 0ah
loc_3_0A42:
        mov    word ptr [eax + 2ah], 0
        mov    dword ptr [eax + 20h], 0
        push   eax
        call   sub_3_37B0
        add    esp, 4
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0A60:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 30h], 4000h
        mov    word ptr [eax + 28h], 0dh
        mov    word ptr [eax + 2ah], 0cd00h
        mov    dword ptr [eax + 20h], 0
        mov    dword ptr [eax + 24h], 0
        call   sub_3_37B0
        add    esp, 4
        ret
        db  090h
loc_3_0A90:
        sub    esp, 0ch
        mov    ecx, dword ptr [esp + 10h]
        lea    eax, [esp]
        or     dword ptr [ecx + 30h], 4000h
        mov    word ptr [ecx + 28h], 0dh
        mov    word ptr [ecx + 2ah], 0cd0ah
        mov    dword ptr [ecx + 20h], eax
        mov    dword ptr [ecx + 24h], 0ch
        mov    dword ptr [esp], 0
        mov    eax, dword ptr [ecx + 1ch]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [esp + 4], al
        mov    eax, dword ptr [ecx + 1ch]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [esp + 5], al
        mov    eax, dword ptr [ecx + 1ch]
        mov    al, byte ptr [eax + 3]
        mov    byte ptr [esp + 6], al
        mov    eax, dword ptr [ecx + 1ch]
        mov    al, byte ptr [eax + 4]
        mov    byte ptr [esp + 7], al
        mov    eax, dword ptr [ecx + 1ch]
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [esp + 8], al
        mov    eax, dword ptr [ecx + 1ch]
        mov    al, byte ptr [eax + 6]
        mov    byte ptr [esp + 9], al
        mov    eax, dword ptr [ecx + 1ch]
        push   ecx
        mov    al, byte ptr [eax + 7]
        mov    byte ptr [esp + 0eh], al
        mov    eax, dword ptr [ecx + 1ch]
        mov    al, byte ptr [eax + 8]
        mov    byte ptr [esp + 0fh], al
        call   sub_3_37B0
        add    esp, 10h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0B20:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 30h], 4000h
        mov    word ptr [eax + 28h], 0dh
        mov    word ptr [eax + 2ah], 0cd18h
        mov    ecx, dword ptr [eax + 1ch]
        inc    ecx
        mov    dword ptr [eax + 20h], ecx
        call   sub_3_37B0
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0B50:
        mov    eax, dword ptr [esp + 4]
        push   eax
        or     dword ptr [eax + 30h], 4000h
        mov    word ptr [eax + 28h], 7
        mov    word ptr [eax + 2ah], 0
        mov    dword ptr [eax + 20h], 0
        call   sub_3_37B0
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0B80:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_3640
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_0BAD
        mov    eax, dword ptr [esi + 14h]
        push   esi
        mov    word ptr [eax + 3], 8103h
        call   sub_3_3160
        add    esp, 4
        push   esi
        call   sub_3_36C0
        add    esp, 4
loc_3_0BAD:
        pop    esi
        ret
        db  090h
loc_3_0BB0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_3640
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_0BDD
        mov    eax, dword ptr [esi + 14h]
        push   esi
        mov    word ptr [eax + 3], 100h
        call   sub_3_3160
        add    esp, 4
        push   esi
        call   sub_3_36C0
        add    esp, 4
loc_3_0BDD:
        pop    esi
        ret
        db  090h
loc_3_0BE0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 14h]
        mov    word ptr [eax + 3], 100h
        ret
        db  08Bh, 0FFh
loc_3_0BF0:
        sub    esp, 0ch
        mov    ecx, dword ptr [esp + 10h]
        lea    eax, [esp]
        mov    word ptr [ecx + 28h], 0dh
        mov    word ptr [ecx + 2ah], 0cd01h
        mov    dword ptr [ecx + 20h], eax
        mov    dword ptr [ecx + 24h], 0ch
        mov    dword ptr [esp], 0
        mov    eax, dword ptr [ecx + 14h]
        push   ecx
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [ecx + 14h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [esp + 0ch], eax
        call   sub_3_37B0
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_0C40:
        sub    esp, 20h
        push   esi
        mov    esi, dword ptr [esp + 28h]
        push   esi
        call   sub_3_3640
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_3_0DC4
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd0eh
        lea    eax, [esp + 10h]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 14h
        mov    dword ptr [esp + 10h], 0
        mov    dword ptr [esp + 20h], 0
        mov    eax, dword ptr [esi + 14h]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esi + 14h]
        mov    eax, dword ptr [eax + 0eh]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esp + 0ch], eax
        cmp    dword ptr [esp + 8], 0
        je     SHORT loc_3_0CED
        xor    ecx, ecx
        mov    cl, byte ptr [esp + 0eh]
        shl    ecx, 2
        mov    dword ptr [esp + 8], 0
        lea    ecx, [ecx + ecx*2]
        xor    eax, eax
        mov    al, byte ptr [esp + 0dh]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        xor    eax, eax
        lea    ecx, [ecx + ecx*4]
        mov    al, byte ptr [esp + 0ch]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        sub    ecx, 96h
        mov    dword ptr [esp + 0ch], ecx
loc_3_0CED:
        mov    eax, dword ptr [esi + 14h]
        mov    eax, dword ptr [eax + 12h]
        dec    eax
        add    dword ptr [esp + 0ch], eax
        cmp    dword ptr [esp + 14h], 1
        jne    SHORT loc_3_0D5E
        mov    dword ptr [esp + 8], 1
        add    dword ptr [esp + 0ch], 96h
        mov    ecx, 4bh
        mov    eax, dword ptr [esp + 0ch]
        sub    edx, edx
        div    ecx
        mov    byte ptr [esp + 4], dl
        mov    eax, dword ptr [esp + 0ch]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 0ch], eax
        mov    ecx, 3ch
        sub    edx, edx
        div    ecx
        mov    byte ptr [esp + 5], dl
        mov    eax, dword ptr [esp + 0ch]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 0ch], eax
        mov    byte ptr [esp + 0fh], 0
        mov    byte ptr [esp + 0eh], al
        mov    al, byte ptr [esp + 5]
        mov    byte ptr [esp + 0dh], al
        mov    al, byte ptr [esp + 4]
        mov    byte ptr [esp + 0ch], al
loc_3_0D5E:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esi + 14h]
        cmp    dword ptr [eax + 12h], 0
        je     SHORT loc_3_0D80
        push   esi
        call   sub_3_30C0
        add    esp, 4
        jmp    SHORT loc_3_0D87
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0D80:
        mov    dword ptr [esi + 2ch], 0
loc_3_0D87:
        cmp    dword ptr [esi + 2ch], 0
        jne    SHORT loc_3_0D98
        mov    eax, dword ptr [esi + 14h]
        mov    word ptr [eax + 3], 100h
        jmp    SHORT loc_3_0DB2
loc_3_0D98:
        cmp    dword ptr [esi + 2ch], 57h
        jne    SHORT loc_3_0DA9
        mov    eax, dword ptr [esi + 14h]
        mov    word ptr [eax + 3], 810dh
        jmp    SHORT loc_3_0DB2
loc_3_0DA9:
        push   esi
        call   sub_3_3820
        add    esp, 4
loc_3_0DB2:
        push   esi
        call   sub_3_3160
        add    esp, 4
        push   esi
        call   sub_3_36C0
        add    esp, 4
loc_3_0DC4:
        pop    esi
        add    esp, 20h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0DD0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    word ptr [eax + 28h], 0dh
        mov    word ptr [eax + 2ah], 0cd0fh
        mov    dword ptr [eax + 20h], 0
        mov    dword ptr [eax + 24h], 0
        call   sub_3_37B0
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0E00:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    word ptr [eax + 28h], 0dh
        mov    word ptr [eax + 2ah], 0cd10h
        mov    dword ptr [eax + 20h], 0
        mov    dword ptr [eax + 24h], 0
        call   sub_3_37B0
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0E30:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi]
        cmp    dword ptr [edi + 28h], 1
        jne    SHORT loc_3_0E57
        or     dword ptr [esi + 20h], 80000h
        mov    eax, dword ptr [edi]
        mov    ecx, eax
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*8]
        lea    eax, [ecx + eax*2]
        shl    eax, 4
        jmp    SHORT loc_3_0E5C
loc_3_0E57:
        mov    eax, dword ptr [edi]
        shl    eax, 0bh
loc_3_0E5C:
        mov    dword ptr [esi + 0a0h], eax
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [esi + 98h], eax
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 1ch]
        or     eax, eax
        mov    dword ptr [esi + 14h], eax
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [esi + 9ch], ecx
        mov    dword ptr [esi + 0a4h], 0
        mov    dword ptr [esi + 24h], 0
        je     SHORT loc_3_0EA0
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 44h], eax
        jmp    SHORT loc_3_0ECE
        db  090h
loc_3_0EA0:
        add    edi, 40h
        mov    dword ptr [esi + 44h], edi
        mov    dword ptr [edi], 10h
        mov    byte ptr [edi + 4], 1
        mov    byte ptr [edi + 5], 1
        mov    byte ptr [edi + 6], 1
        mov    byte ptr [edi + 7], 1
        call   sub_3_7720
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 44h]
        mov    dword ptr [eax + 0ch], 0bh
loc_3_0ECE:
        push   42h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_0EE0:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        cmp    dword ptr [ecx + 24h], 0
        jne    NEAR PTR loc_3_0F88
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [eax + 3ch], 5
        jne    NEAR PTR loc_3_0F8F
        mov    edi, dword ptr [ecx + 4ch]
        db  0B8h, 034h, 0A3h, 000h, 000h
        lea    edx, [edi + 9]
        mov    esi, dword ptr [eax]
        cmp    esi, dword ptr [edx]
        jne    SHORT loc_3_0F14
        mov    al, byte ptr [eax + 4]
        cmp    al, byte ptr [edx + 4]
loc_3_0F14:
        mov    edx, 0
        sete   dl
        db  0B8h, 03Ch, 0A3h, 000h, 000h
        lea    ebx, [edi + 1]
        mov    esi, dword ptr [eax]
        cmp    esi, dword ptr [ebx]
        jne    SHORT loc_3_0F30
        mov    al, byte ptr [eax + 4]
        cmp    al, byte ptr [ebx + 4]
loc_3_0F30:
        mov    eax, 0
        sete   al
        or     eax, eax
        jne    SHORT loc_3_0F4D
        or     edx, edx
        jne    SHORT loc_3_0F49
        mov    dword ptr [ecx + 24h], 0bh
        jmp    SHORT loc_3_0F8F
loc_3_0F49:
        or     eax, eax
        je     SHORT loc_3_0F53
loc_3_0F4D:
        xor    eax, eax
        mov    al, byte ptr [edi]
        jmp    SHORT loc_3_0F58
loc_3_0F53:
        xor    eax, eax
        mov    al, byte ptr [edi + 8]
loc_3_0F58:
        cmp    eax, 1
        je     SHORT loc_3_0F71
        cmp    eax, 0ffh
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax + 38h]
        je     SHORT loc_3_0F80
        mov    word ptr [eax + 1ch], 0
        jmp    SHORT loc_3_0F8F
loc_3_0F71:
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax + 38h]
        mov    word ptr [eax + 1ch], 1
        jmp    SHORT loc_3_0F8F
        db  08Bh, 0FFh
loc_3_0F80:
        mov    word ptr [eax + 1ch], 0ffh
        jmp    SHORT loc_3_0F8F
loc_3_0F88:
        mov    dword ptr [ecx + 24h], 15h
loc_3_0F8F:
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 24h]
        mov    word ptr [edx + 1ah], ax
        cmp    dword ptr [ecx + 14h], 0
        jne    SHORT loc_3_0FAD
        mov    eax, dword ptr [ecx + 44h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_3_7730
        add    esp, 4
loc_3_0FAD:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0FC0:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax], 0
        je     SHORT loc_3_0FE0
loc_3_0FC9:
        mov    cl, byte ptr [eax]
        cmp    cl, 61h
        jb     SHORT loc_3_0FDA
        cmp    cl, 7ah
        ja     SHORT loc_3_0FDA
        sub    cl, 20h
        mov    byte ptr [eax], cl
loc_3_0FDA:
        inc    eax
        cmp    byte ptr [eax], 0
        jne    SHORT loc_3_0FC9
loc_3_0FE0:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0FF0:
        sub    esp, 78h
        push   esi
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 4], eax
        mov    esi, dword ptr [esp + 80h]
        xor    eax, eax
        mov    al, byte ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [esp + 88h]
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   esi
        push   eax
        call   sub_3_9AC0
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_3_1030
        mov    eax, 1
        pop    esi
        add    esp, 78h
        ret
        db  08Dh, 049h, 000h
loc_3_1030:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 1ah], 0
        jne    SHORT loc_3_10A0
        push   3810h
        call   sub_3_57FF
        mov    ecx, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [ecx + 14h], eax
        push   3eh
        mov    eax, dword ptr [esp + 8]
        push   5
        push   eax
        call   sub_3_55E0
        mov    eax, dword ptr [esp + 10h]
        add    esp, 0ch
        push   eax
        call   sub_3_9BA0
        mov    eax, dword ptr [esp + 8]
        add    esp, 4
        cmp    word ptr [eax + 1ah], 0
        jne    SHORT loc_3_1083
        and    dword ptr [esi + 2ch], 0fffffffeh
        sub    eax, eax
        pop    esi
        add    esp, 78h
        ret
loc_3_1083:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 1ah]
        or     dword ptr [esi + 2ch], 1
        mov    word ptr [esi + 1ch], ax
        sub    eax, eax
        pop    esi
        add    esp, 78h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_10A0:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 10h], 0
        je     SHORT loc_3_10B7
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_9BA0
        add    esp, 4
loc_3_10B7:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 1ah]
        or     dword ptr [esi + 2ch], 1
        mov    word ptr [esi + 1ch], ax
        sub    eax, eax
        pop    esi
        add    esp, 78h
        ret
        db  08Bh, 0FFh
loc_3_10D0:
        sub    esp, 78h
        push   esi
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 4], eax
        mov    esi, dword ptr [esp + 80h]
        xor    eax, eax
        mov    al, byte ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [esp + 88h]
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   esi
        push   eax
        call   sub_3_9AC0
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_3_1110
        mov    eax, 1
        pop    esi
        add    esp, 78h
        ret
        db  08Dh, 049h, 000h
loc_3_1110:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 1ah], 0
        jne    NEAR PTR loc_3_11B0
        push   3810h
        call   sub_3_57FF
        mov    ecx, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax], 1
        xor    eax, eax
        mov    ax, word ptr [esi + 14h]
        add    eax, 10h
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [esp + 4]
        push   40h
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [esp + 8]
        push   5
        mov    dword ptr [eax + 3ch], 5
        mov    eax, dword ptr [esp + 0ch]
        push   eax
        call   sub_3_55E0
        mov    eax, dword ptr [esp + 10h]
        add    esp, 0ch
        push   eax
        call   sub_3_9BA0
        mov    eax, dword ptr [esp + 8]
        add    esp, 4
        cmp    word ptr [eax + 1ah], 0
        jne    SHORT loc_3_1197
        and    dword ptr [esi + 2ch], 0fffffffeh
        sub    eax, eax
        pop    esi
        add    esp, 78h
        ret
loc_3_1197:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 1ah]
        or     dword ptr [esi + 2ch], 1
        mov    word ptr [esi + 1ch], ax
        sub    eax, eax
        pop    esi
        add    esp, 78h
        ret
        db  08Bh, 0FFh
loc_3_11B0:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 10h], 0
        je     SHORT loc_3_11C7
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_9BA0
        add    esp, 4
loc_3_11C7:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 1ah], 0fh
        je     SHORT loc_3_11DC
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 15h
loc_3_11DC:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 1ah]
        or     dword ptr [esi + 2ch], 1
        mov    word ptr [esi + 1ch], ax
        sub    eax, eax
        pop    esi
        add    esp, 78h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1200:
        mov    eax, dword ptr [esp + 4]
        and    dword ptr [eax + 2ch], 0fffffffeh
        sub    eax, eax
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_1210:
        sub    esp, 78h
        push   esi
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 4], eax
        mov    esi, dword ptr [esp + 80h]
        xor    eax, eax
        mov    al, byte ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [esp + 88h]
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   esi
        push   eax
        call   sub_3_9AC0
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_3_1250
        mov    eax, 1
        pop    esi
        add    esp, 78h
        ret
        db  08Dh, 049h, 000h
loc_3_1250:
        push   3810h
        call   sub_3_57FF
        mov    ecx, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [ecx + 14h], eax
        xor    eax, eax
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [esi + 14h]
        mov    dword ptr [ecx], eax
        xor    ecx, ecx
        mov    cx, word ptr [esi + 4]
        xor    eax, eax
        mov    ax, word ptr [esi]
        shl    ecx, 10h
        add    ecx, eax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 24h], ecx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 3ch], 0
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 10h], 0
        je     SHORT loc_3_12F4
        push   40h
        mov    eax, dword ptr [esp + 8]
        push   5
        push   eax
        call   sub_3_55E0
        mov    eax, dword ptr [esp + 10h]
        add    esp, 0ch
        cmp    word ptr [eax + 1ah], 0
        jne    SHORT loc_3_12D0
        mov    word ptr [esi + 1ch], 0
        and    dword ptr [esi + 2ch], 0fffffffeh
        jmp    SHORT loc_3_12E0
        db  090h
loc_3_12D0:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 1ah]
        or     dword ptr [esi + 2ch], 1
        mov    word ptr [esi + 1ch], ax
loc_3_12E0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_9BA0
        add    esp, 4
        sub    eax, eax
        pop    esi
        add    esp, 78h
        ret
loc_3_12F4:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 1ah], 0fh
        je     SHORT loc_3_1309
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 15h
loc_3_1309:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 1ah]
        or     dword ptr [esi + 2ch], 1
        mov    word ptr [esi + 1ch], ax
        sub    eax, eax
        pop    esi
        add    esp, 78h
        ret
loc_3_1320:
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 2ch], 1
        mov    word ptr [eax + 1ch], 1
        sub    eax, eax
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1340:
        mov    eax, dword ptr [esp + 4]
        and    dword ptr [eax + 2ch], 0fffffffeh
        sub    eax, eax
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_1350:
        push   esi
        mov    esi, dword ptr [esp + 8]
        xor    eax, eax
        mov    al, byte ptr [esi + 18h]
        db  08Bh, 00Dh, 090h, 008h, 000h, 000h
        lea    edx, [eax + eax*2]
        lea    edx, [ecx + edx*2 + 14h]
        cmp    eax, 1ah
        jae    NEAR PTR loc_3_1431
        mov    cl, byte ptr [edx + 1]
        test   cl, 8
        je     NEAR PTR loc_3_1431
        test   cl, 10h
        jne    SHORT loc_3_1388
        mov    eax, 1
        pop    esi
        ret
loc_3_1388:
        xor    eax, eax
        mov    ax, word ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_3_13A5
        cmp    eax, 1
        je     SHORT loc_3_13DA
        mov    word ptr [esi + 1ch], 1
        or     dword ptr [esi + 2ch], 1
        sub    eax, eax
        pop    esi
        ret
loc_3_13A5:
        and    ecx, 7
        cmp    ecx, 1
        je     SHORT loc_3_13C0
        cmp    ecx, 2
        je     SHORT loc_3_13D0
        mov    word ptr [esi + 14h], 100h
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_13C0:
        mov    word ptr [esi + 14h], 201h
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_13D0:
        mov    word ptr [esi + 14h], 202h
        sub    eax, eax
        pop    esi
        ret
loc_3_13DA:
        xor    eax, eax
        and    dword ptr [esi + 2ch], 0fffffffeh
        mov    ax, word ptr [esi + 14h]
        cmp    eax, 100h
        je     SHORT loc_3_1407
        cmp    eax, 201h
        je     SHORT loc_3_1411
        cmp    eax, 202h
        je     SHORT loc_3_1421
        mov    word ptr [esi + 1ch], 1
        or     dword ptr [esi + 2ch], 1
        sub    eax, eax
        pop    esi
        ret
loc_3_1407:
        and    cl, 0f8h
        mov    byte ptr [edx + 1], cl
        sub    eax, eax
        pop    esi
        ret
loc_3_1411:
        and    cl, 0f8h
        mov    byte ptr [edx + 1], cl
        or     cl, 1
        mov    byte ptr [edx + 1], cl
        sub    eax, eax
        pop    esi
        ret
loc_3_1421:
        and    cl, 0f8h
        mov    byte ptr [edx + 1], cl
        or     cl, 2
        mov    byte ptr [edx + 1], cl
        sub    eax, eax
        pop    esi
        ret
loc_3_1431:
        mov    word ptr [esi + 1ch], 0fh
        or     dword ptr [esi + 2ch], 1
        sub    eax, eax
        pop    esi
        ret
        db  090h
loc_3_1440:
        sub    esp, 78h
        push   esi
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 4], eax
        mov    esi, dword ptr [esp + 80h]
        xor    eax, eax
        mov    al, byte ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [esp + 88h]
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        push   esi
        push   eax
        call   sub_3_9AC0
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_3_1480
        mov    eax, 1
        pop    esi
        add    esp, 78h
        ret
        db  08Dh, 049h, 000h
loc_3_1480:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 1ah], 0
        jne    NEAR PTR loc_3_1550
        push   3810h
        call   sub_3_57FF
        mov    ecx, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [ecx + 0ch], eax
        push   400h
        call   sub_3_57FF
        mov    ecx, dword ptr [esp + 8]
        add    esp, 4
        mov    dword ptr [ecx + 14h], eax
        mov    al, byte ptr [esi + 19h]
        mov    ecx, dword ptr [esp + 4]
        and    eax, 0ffff00ffh
        mov    word ptr [ecx + 18h], ax
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        call   sub_3_0FC0
        add    esp, 4
        push   44h
        mov    eax, dword ptr [esp + 8]
        push   5
        push   eax
        call   sub_3_55E0
        mov    eax, dword ptr [esp + 10h]
        add    esp, 0ch
        push   eax
        call   sub_3_9BA0
        mov    eax, dword ptr [esp + 8]
        add    esp, 4
        cmp    word ptr [eax + 1ah], 0
        mov    eax, dword ptr [esp + 4]
        jne    SHORT loc_3_1520
        mov    ax, word ptr [eax + 18h]
        and    dword ptr [esi + 2ch], 0fffffffeh
        mov    word ptr [esi + 1ch], ax
        mov    byte ptr [esi + 19h], 0
        sub    eax, eax
        pop    esi
        add    esp, 78h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_1520:
        cmp    word ptr [eax + 1ah], 3
        jne    SHORT loc_3_1531
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 2
loc_3_1531:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 1ah]
        or     dword ptr [esi + 2ch], 1
        mov    word ptr [esi + 1ch], ax
        sub    eax, eax
        mov    byte ptr [esi + 19h], 0
        pop    esi
        add    esp, 78h
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_1550:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 10h], 0
        je     SHORT loc_3_1567
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_9BA0
        add    esp, 4
loc_3_1567:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 1ah], 0fh
        je     SHORT loc_3_157C
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 15h
loc_3_157C:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 1ah]
        or     dword ptr [esi + 2ch], 1
        mov    word ptr [esi + 1ch], ax
        sub    eax, eax
        mov    byte ptr [esi + 19h], 0
        pop    esi
        add    esp, 78h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_15A0:
        mov    ecx, dword ptr [esp + 4]
        push   0ah
        mov    eax, dword ptr [ecx + 14h]
        push   ecx
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [ecx + 48h], eax
        call   sub_3_8380
        add    esp, 8
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_15C0:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        cmp    dword ptr [ebx + 24h], 0
        je     SHORT loc_3_15D9
        mov    eax, dword ptr [ebx]
        pop    edi
        mov    word ptr [eax + 1ah], 1eh
        pop    esi
        pop    ebx
        ret
loc_3_15D9:
        mov    eax, dword ptr [ebx]
        mov    esi, dword ptr [ebx + 4ch]
        mov    edx, dword ptr [eax + 14h]
        mov    eax, dword ptr [ebx + 14h]
        movzx  edi, byte ptr [eax + 1eh]
        cmp    edi, 1
        sbb    ecx, ecx
        and    ecx, 0fffffffbh
        mov    eax, dword ptr [ebx]
        add    ecx, 25h
        mov    eax, dword ptr [eax + 38h]
        mov    al, byte ptr [eax + 1ch]
        and    eax, 0ffh
        cmp    eax, 2
        je     SHORT loc_3_1630
        cmp    eax, 3
        je     SHORT loc_3_1648
        cmp    eax, 4
        je     SHORT loc_3_165C
        or     edi, edi
        je     SHORT loc_3_1617
        mov    al, byte ptr [esi]
        jmp    SHORT loc_3_161A
loc_3_1617:
        mov    al, byte ptr [esi + 8]
loc_3_161A:
        mov    byte ptr [edx], al
        or     edi, edi
        je     SHORT loc_3_1627
        add    esi, 28h
        inc    edx
        dec    ecx
        jmp    SHORT loc_3_166D
loc_3_1627:
        add    esi, 30h
        inc    edx
        dec    ecx
        jmp    SHORT loc_3_166D
        db  08Bh, 0FFh
loc_3_1630:
        or     edi, edi
        je     SHORT loc_3_1640
        add    esi, 2beh
        jmp    SHORT loc_3_166D
        db  08Dh, 064h, 024h, 000h
loc_3_1640:
        add    esi, 2d6h
        jmp    SHORT loc_3_166D
loc_3_1648:
        or     edi, edi
        je     SHORT loc_3_1654
        add    esi, 2e3h
        jmp    SHORT loc_3_166D
loc_3_1654:
        add    esi, 2f6h
        jmp    SHORT loc_3_166D
loc_3_165C:
        or     edi, edi
        je     SHORT loc_3_1668
        add    esi, 308h
        jmp    SHORT loc_3_166D
loc_3_1668:
        db  0BEh, 060h, 0A3h, 000h, 000h
loc_3_166D:
        sub    ebx, ebx
        cmp    ecx, ebx
        je     SHORT loc_3_1683
loc_3_1673:
        mov    al, byte ptr [esi + ebx]
        or     al, al
        je     SHORT loc_3_1683
        cmp    al, 20h
        je     SHORT loc_3_1683
        inc    ebx
        cmp    ecx, ebx
        ja     SHORT loc_3_1673
loc_3_1683:
        or     ebx, ebx
        je     SHORT loc_3_1697
        mov    edi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_3_1697:
        mov    byte ptr [edx + ebx], 0
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_3_16A0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax + 70h], 10h
        mov    eax, dword ptr [esp + 14h]
        or     dword ptr [eax + 20h], 2000000h
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_3_54A0
        mov    eax, dword ptr [esp + 18h]
        add    esp, 4
        cmp    dword ptr [eax + 24h], 0
        jne    NEAR PTR loc_3_1795
        mov    eax, dword ptr [esp + 14h]
        mov    esi, dword ptr [eax]
        mov    ecx, 104h
        mov    edx, dword ptr [eax + 10h]
        mov    esi, dword ptr [esi + 0ch]
        lea    edi, [edx + 4ch]
        mov    dword ptr [esp + 0ch], edx
        xor    eax, eax
loc_3_16F0:
        lodsb
        stosw
        or     eax, eax
        je     SHORT loc_3_1707
        dec    ecx
        jne    SHORT loc_3_16F0
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 24h], 3
        jmp    SHORT loc_3_175C
loc_3_1707:
        lea    esi, [edx + 4ch]
        lodsw
        mov    edi, esi
        jmp    SHORT loc_3_171E
loc_3_1710:
        mov    edi, esi
loc_3_1712:
        lodsw
        or     eax, eax
        je     SHORT loc_3_171E
        cmp    ax, 5ch
        jne    SHORT loc_3_1712
loc_3_171E:
        mov    ecx, esi
        sub    ecx, edi
        mov    word ptr [edi - 2], cx
        or     eax, eax
        jne    SHORT loc_3_1710
        lea    eax, [edx + 4eh]
        mov    ecx, esi
        sub    edi, eax
        sub    ecx, eax
        mov    word ptr [edx + 4ah], di
        mov    word ptr [edx + 48h], cx
        mov    ebx, dword ptr [esp + 14h]
        mov    ebx, dword ptr [ebx]
        and    dword ptr [ebx], 7fffffffh
        shr    ecx, 1
        lea    edi, [edx + 4ch]
        mov    eax, 3fh
        repne scasw
        jne    SHORT loc_3_175C
        or     dword ptr [ebx], 80000000h
loc_3_175C:
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [eax + 24h], 0
        jne    SHORT loc_3_1795
        mov    eax, dword ptr [esp + 0ch]
        push   25h
        mov    ax, word ptr [eax + 48h]
        and    eax, 0fffeh
        mov    ecx, dword ptr [esp + 10h]
        mov    word ptr [eax + ecx + 4ch], 0
        mov    eax, dword ptr [esp + 18h]
        mov    byte ptr [eax + 6dh], 0
        mov    eax, dword ptr [esp + 18h]
        push   eax
        call   sub_3_8380
        add    esp, 8
loc_3_1795:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_17A0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 1ch]
        cmp    dword ptr [ebp + 24h], 0
        je     SHORT loc_3_17C3
        mov    eax, dword ptr [ebp]
        mov    ecx, dword ptr [ebp + 24h]
        mov    word ptr [eax + 1ah], cx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_17C3:
        mov    ecx, dword ptr [ebp + 14h]
        xor    eax, eax
        mov    al, byte ptr [ecx + 1eh]
        mov    ebx, dword ptr [ebp + 90h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [ebp]
        mov    edx, dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp]
        test   byte ptr [eax + 18h], 1
        je     NEAR PTR loc_3_18EA
        mov    al, byte ptr [ebx]
        mov    byte ptr [edx], al
        mov    eax, dword ptr [ebx + 2]
        mov    dword ptr [edx + 1], eax
        mov    ax, 1
        mov    cl, byte ptr [ecx + 44h]
        shl    ax, cl
        mov    word ptr [edx + 5], ax
        mov    eax, dword ptr [ebx + 0ah]
        mov    dword ptr [edx + 7], eax
        lea    ecx, [edx + 0bh]
        mov    eax, dword ptr [ebx + 12h]
        mov    dword ptr [ecx], eax
        cmp    dword ptr [esp + 10h], 0
        mov    ax, word ptr [ebx + 16h]
        lea    esi, [ebx + 12h]
        mov    word ptr [ecx + 4], ax
        mov    al, byte ptr [esi + 6]
        mov    byte ptr [ecx + 6], al
        jne    SHORT loc_3_1831
        mov    byte ptr [edx + 12h], 0
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_1836
loc_3_1831:
        mov    al, byte ptr [ebx + 19h]
        jmp    SHORT loc_3_1839
loc_3_1836:
        mov    al, byte ptr [ebx + 18h]
loc_3_1839:
        mov    byte ptr [edx + 12h], al
        mov    al, byte ptr [ebx + 1ah]
        mov    byte ptr [edx + 13h], al
        mov    al, byte ptr [ebx + 1bh]
        mov    byte ptr [edx + 14h], al
        mov    ax, word ptr [ebx + 1ch]
        mov    word ptr [edx + 15h], ax
        mov    al, byte ptr [ebx + 20h]
        mov    ecx, eax
        mov    byte ptr [edx + 17h], al
        and    ecx, 0feh
        inc    ecx
        mov    eax, 0ffffffdfh
        sub    eax, ebx
        sub    al, cl
        add    al, byte ptr [ebx]
        add    al, bl
        mov    byte ptr [edx + 40h], al
        and    eax, 0ffh
        lea    edi, [edx + 41h]
        lea    esi, [ecx + ebx + 21h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        xor    ecx, ecx
        mov    cl, byte ptr [ebx + 20h]
        lea    eax, [edx + 18h]
        mov    dword ptr [esp + 14h], ecx
        lea    esi, [ebx + 21h]
        mov    edi, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [esp + 14h]
        and    ecx, 3
        rep movsb
        xor    ecx, ecx
        mov    cl, byte ptr [ebx + 20h]
        or     ecx, ecx
        je     SHORT loc_3_18BA
loc_3_18B1:
        cmp    byte ptr [eax], 3bh
        je     SHORT loc_3_18BA
        inc    eax
        dec    ecx
        jne    SHORT loc_3_18B1
loc_3_18BA:
        mov    byte ptr [eax], 0
        sub    di, di
        or     ecx, ecx
        je     SHORT loc_3_1900
        sub    byte ptr [edx + 17h], cl
        dec    ecx
        mov    esi, ecx
        dec    ecx
        or     esi, esi
        je     SHORT loc_3_18E4
        nop
loc_3_18D0:
        inc    eax
        imul   di, di, 0ah
        movzx  si, byte ptr [eax]
        lea    edi, [edi + esi - 30h]
        mov    esi, ecx
        dec    ecx
        or     esi, esi
        jne    SHORT loc_3_18D0
loc_3_18E4:
        mov    word ptr [edx + 3eh], di
        jmp    SHORT loc_3_1900
loc_3_18EA:
        xor    eax, eax
        mov    al, byte ptr [ebx]
        mov    edi, edx
        mov    esi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_1900:
        cmp    dword ptr [esp + 10h], 0
        mov    eax, dword ptr [ebp]
        je     SHORT loc_3_1912
        mov    word ptr [eax + 18h], 1
        jmp    SHORT loc_3_1918
loc_3_1912:
        mov    word ptr [eax + 18h], 0
loc_3_1918:
        mov    eax, dword ptr [ebp]
        pop    ebp
        mov    word ptr [eax + 1ah], 0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1930:
        sub    esp, 14h
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 20h]
        xor    eax, eax
        mov    al, byte ptr [edi + 4]
        or     eax, eax
        je     SHORT loc_3_1963
        cmp    eax, 2
        je     SHORT loc_3_1982
        cmp    eax, 3
        je     SHORT loc_3_19C0
        mov    si, word ptr [esp + 0ah]
loc_3_1951:
        push   esi
        push   1
        push   edi
        call   sub_3_55E0
        add    esp, 0ch
        pop    edi
        pop    esi
        add    esp, 14h
        ret
loc_3_1963:
        mov    si, 0ch
        push   edi
        call   sub_3_19D0
        add    esp, 4
        cmp    eax, 11ch
        jne    SHORT loc_3_1951
        mov    eax, 11ch
        pop    edi
        pop    esi
        add    esp, 14h
        ret
loc_3_1982:
        mov    dword ptr [esp + 0ch], 0baadf00dh
        mov    byte ptr [esp + 10h], 0dh
        mov    byte ptr [esp + 11h], 0f0h
        mov    byte ptr [esp + 12h], 0adh
        mov    si, 3ch
        mov    byte ptr [esp + 13h], 0bah
        mov    dword ptr [esp + 14h], 0baadf00dh
        mov    dword ptr [esp + 18h], 0baadf00dh
        lea    eax, [esp + 0ch]
        mov    dword ptr [edi + 24h], eax
        jmp    SHORT loc_3_1951
        db  005h, 000h, 000h, 000h, 000h
loc_3_19C0:
        mov    si, 3
        jmp    SHORT loc_3_1951
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_19D0:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 03Dh, 0D0h, 09Bh, 000h, 000h
        or     edi, edi
        je     SHORT loc_3_19FD
        mov    esi, dword ptr [esp + 20h]
        mov    ecx, 1
loc_3_19EA:
        test   byte ptr [edi + 1fh], cl
        je     SHORT loc_3_19F7
        mov    eax, dword ptr [esi + 28h]
        cmp    dword ptr [edi + 8], eax
        je     SHORT loc_3_1A0A
loc_3_19F7:
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_3_19EA
loc_3_19FD:
        mov    eax, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_3_1A0A:
        mov    ebx, dword ptr [esi + 14h]
        mov    cl, byte ptr [esi + 4]
        mov    byte ptr [esp + 18h], cl
        mov    ebp, dword ptr [esi + 28h]
        mov    cx, word ptr [esi + 1ah]
        mov    word ptr [esp + 14h], cx
        mov    ecx, dword ptr [esi + 24h]
        mov    dword ptr [esp + 10h], ecx
        mov    byte ptr [esi + 4], 2
        mov    dword ptr [esi + 28h], edi
        mov    dword ptr [esi + 14h], 0
        push   esi
        call   sub_3_1930
        add    esp, 4
        mov    dword ptr [esi + 14h], ebx
        mov    cl, byte ptr [esp + 18h]
        mov    byte ptr [esi + 4], cl
        mov    dword ptr [esi + 28h], ebp
        or     eax, eax
        mov    cx, word ptr [esp + 14h]
        mov    word ptr [esi + 1ah], cx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [esi + 24h], ecx
        jne    SHORT loc_3_19FD
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 1ch], eax
        mov    word ptr [esi + 1ah], 11ch
        db  0C7h, 046h, 010h, 044h, 009h, 000h, 000h
        mov    eax, 11ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
loc_3_1A80:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 41h
        mov    eax, 41h
        ret
loc_3_1A90:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_5825
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_3_1AA0:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 0
        sub    eax, eax
        ret
        db  08Dh, 049h, 000h
loc_3_1AB0:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 0
        sub    eax, eax
        ret
        db  08Dh, 049h, 000h
loc_3_1AC0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        sub    esi, esi
        cmp    dword ptr [edi + 4ch], esi
        je     SHORT loc_3_1AF0
        mov    eax, dword ptr [edi + 20h]
        push   eax
        push   esi
        mov    eax, dword ptr [edi + 4ch]
        push   eax
        call   sub_3_8750
        add    esp, 0ch
        mov    dword ptr [edi + 4ch], esi
        cmp    dword ptr [edi + 24h], esi
        je     SHORT loc_3_1AF5
        mov    esi, 5
        jmp    SHORT loc_3_1AF5
        db  08Dh, 049h, 000h
loc_3_1AF0:
        mov    esi, 8
loc_3_1AF5:
        or     esi, esi
        je     SHORT loc_3_1B11
        mov    eax, dword ptr [edi + 1ch]
        push   edi
        mov    cl, byte ptr [eax + 0bh]
        mov    eax, dword ptr [edi + 1ch]
        or     cl, 1
        mov    byte ptr [eax + 0bh], cl
        call   sub_3_47C0
        add    esp, 4
loc_3_1B11:
        mov    eax, dword ptr [edi]
        mov    dword ptr [edi + 50h], 0
        mov    word ptr [eax + 1ah], si
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1B30:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [esi + 14h], eax
        db  03Bh, 005h, 080h, 09Ch, 000h, 000h
        jne    SHORT loc_3_1B4F
        db  0C7h, 005h, 080h, 09Ch, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_1B4F:
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 44h], ecx
        or     byte ptr [eax + 1fh], 8
        push   eax
        call   sub_3_7800
        add    esp, 4
        push   esi
        call   sub_3_4660
        add    esp, 4
        mov    eax, dword ptr [esi]
        pop    esi
        mov    word ptr [eax + 1ah], 0
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1B80:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 14h], 0
        je     SHORT loc_3_1B94
        or     dword ptr [esi + 20h], 80h
loc_3_1B94:
        mov    eax, dword ptr [esi]
        mov    byte ptr [esi + 63h], 1
        mov    eax, dword ptr [eax + 28h]
        mov    edx, eax
        mov    dword ptr [esi + 14h], eax
        mov    ecx, dword ptr [edx + 0ch]
        mov    dword ptr [esi + 44h], ecx
        mov    eax, dword ptr [edx + 38h]
        mov    dword ptr [esi + 48h], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esi + 40h], eax
        inc    word ptr [edx + 1ch]
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [eax + 24h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [esi + 44h], eax
        call   sub_3_7720
        mov    ecx, dword ptr [esi + 44h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esi + 14h]
        test   byte ptr [eax + 1fh], 2
        jne    SHORT loc_3_1C10
        mov    ecx, dword ptr [esi + 40h]
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_3_1C10
        push   0ah
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_1C10:
        mov    dword ptr [esi + 24h], 0ch
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1C20:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        cmp    dword ptr [ebx + 24h], 0
        jne    NEAR PTR loc_3_1D36
        mov    esi, dword ptr [ebx + 14h]
        mov    edx, 1
        mov    eax, dword ptr [esi + 0ch]
        mov    cl, byte ptr [eax + 0ch]
        shl    edx, cl
        push   edx
        mov    eax, dword ptr [ebx + 4ch]
        push   eax
        call   sub_3_6890
        add    esp, 8
        cmp    eax, dword ptr [esi + 3ch]
        jne    NEAR PTR loc_3_1D36
        mov    esi, dword ptr [ebx + 14h]
        mov    edx, dword ptr [ebx + 4ch]
        cmp    byte ptr [esi + 1eh], 0
        lea    esi, [esi + 58h]
        je     SHORT loc_3_1CD6
        lea    edi, [edx + 28h]
        mov    ecx, 8
        repe cmpsd
        jne    NEAR PTR loc_3_1D36
        mov    esi, dword ptr [ebx + 14h]
        lea    edi, [edx + 0beh]
        mov    ecx, 20h
        lea    esi, [esi + 78h]
        repe cmpsd
        jne    NEAR PTR loc_3_1D36
        mov    ecx, dword ptr [ebx + 14h]
        mov    ax, word ptr [edx + 78h]
        cmp    word ptr [ecx + 0f8h], ax
        jne    NEAR PTR loc_3_1D36
        mov    ax, word ptr [ecx + 0fah]
        cmp    word ptr [edx + 7ch], ax
        jne    NEAR PTR loc_3_1D36
        mov    eax, dword ptr [ecx + 0fch]
        cmp    dword ptr [edx + 8ch], eax
        jne    SHORT loc_3_1D36
        mov    eax, dword ptr [ecx + 100h]
        cmp    dword ptr [edx + 84h], eax
        je     NEAR PTR loc_3_1D68
        jmp    SHORT loc_3_1D36
loc_3_1CD6:
        lea    edi, [edx + 30h]
        mov    ecx, 8
        repe cmpsd
        jne    SHORT loc_3_1D36
        mov    esi, dword ptr [ebx + 14h]
        lea    edi, [edx + 0d6h]
        mov    ecx, 20h
        lea    esi, [esi + 78h]
        repe cmpsd
        jne    SHORT loc_3_1D36
        mov    ecx, dword ptr [ebx + 14h]
        mov    ax, word ptr [ecx + 0f8h]
        cmp    word ptr [edx + 80h], ax
        jne    SHORT loc_3_1D36
        mov    ax, word ptr [ecx + 0fah]
        cmp    word ptr [edx + 84h], ax
        jne    SHORT loc_3_1D36
        mov    eax, dword ptr [ecx + 0fch]
        cmp    dword ptr [edx + 94h], eax
        jne    SHORT loc_3_1D36
        mov    eax, dword ptr [ecx + 100h]
        cmp    dword ptr [edx + 8ch], eax
        je     SHORT loc_3_1D68
loc_3_1D36:
        mov    eax, dword ptr [ebx]
        cmp    dword ptr [eax + 14h], 0
        je     SHORT loc_3_1D61
        mov    esi, dword ptr [ebx + 14h]
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [esi + 3ch]
        mov    dword ptr [ecx + 28h], eax
        mov    ecx, 8
        mov    eax, dword ptr [ebx]
        add    esi, 58h
        mov    edi, dword ptr [eax + 14h]
        rep movsd
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 14h]
        mov    byte ptr [eax + 20h], 0
loc_3_1D61:
        mov    dword ptr [ebx + 24h], 0ffffffffh
loc_3_1D68:
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 24h]
        mov    word ptr [ecx + 1ah], ax
        push   ebx
        call   sub_3_75B0
        add    esp, 4
        mov    eax, dword ptr [ebx + 44h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_3_7730
        add    esp, 4
        push   ebx
        call   sub_3_4660
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1DA0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 44h]
        or     eax, eax
        je     SHORT loc_3_1DD1
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_3_7730
        add    esp, 4
        mov    eax, dword ptr [esi + 20h]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 44h]
        push   eax
        call   sub_3_8750
        add    esp, 0ch
        mov    dword ptr [esi + 44h], 0
loc_3_1DD1:
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1DE0:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        and    eax, 0ffffffh
        cmp    eax, 3a2b24h
        je     SHORT loc_3_1E10
        cmp    eax, 402f25h
        je     SHORT loc_3_1E22
        cmp    eax, 432f25h
        je     SHORT loc_3_1E22
        cmp    eax, 452f25h
        je     SHORT loc_3_1E22
loc_3_1E07:
        mov    eax, 0ffffh
        ret
        db  08Dh, 049h, 000h
loc_3_1E10:
        db  081h, 03Dh, 010h, 09Ch, 000h, 000h, 0AEh, 003h, 000h, 000h
        jne    SHORT loc_3_1E07
        mov    eax, 3aeh
        ret
loc_3_1E22:
        mov    eax, 4b0h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_1E30:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, 1
        mov    ecx, dword ptr [esi + 40h]
        shl    edx, cl
        db  0A1h, 0F4h, 09Bh, 000h, 000h
        or     eax, edx
        db  0A3h, 0F4h, 09Bh, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 10h], 800h
        jne    NEAR PTR loc_3_1F30
        db  0A1h, 090h, 008h, 000h, 000h
        lea    edx, [ecx + ecx*2]
        test   byte ptr [eax + edx*2 + 15h], 8
        je     NEAR PTR loc_3_1F30
        or     dword ptr [esi + 20h], 4000h
        db  0A0h, 040h, 01Fh, 000h, 000h
        db  0A1h, 0D0h, 09Bh, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_1EA0
loc_3_1E84:
        cmp    dword ptr [eax + 8], ecx
        je     SHORT loc_3_1E91
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_3_1E84
        jmp    SHORT loc_3_1EA0
loc_3_1E91:
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_3_7730
        add    esp, 4
loc_3_1EA0:
        push   1
        push   10h
        call   sub_3_43B0
        add    esp, 8
        mov    dword ptr [esi + 44h], eax
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_3_1EC4
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_3_36E0
        add    esp, 4
        mov    dword ptr [esi + 8], eax
loc_3_1EC4:
        mov    edi, dword ptr [esi + 44h]
        or     edi, edi
        je     SHORT loc_3_1F20
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_3_1F20
        mov    dword ptr [eax + 40h], esi
        call   sub_3_7720
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 44h]
        mov    dword ptr [eax + 0ch], 0bh
        mov    byte ptr [eax + 4], 0ffh
        mov    byte ptr [eax + 5], 0ffh
        mov    byte ptr [eax + 6], 0ffh
        mov    byte ptr [eax + 7], 0ffh
        mov    word ptr [esi + 70h], 0ffh
        mov    dword ptr [esi + 0a0h], 0ffh
        mov    dword ptr [esi + 98h], 0ffh
        push   16h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        ret
        db  090h
loc_3_1F20:
        mov    dword ptr [esi + 24h], 8
        mov    word ptr [esi + 2eh], 15h
        pop    edi
        pop    esi
        ret
loc_3_1F30:
        mov    dword ptr [esi + 24h], 0bh
        mov    word ptr [esi + 2eh], 15h
        pop    edi
        pop    esi
        ret
loc_3_1F40:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_1F50:
        sub    esp, 34h
        push   ebx
        mov    ecx, dword ptr [esp + 3ch]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp + 8], 0
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esp + 0ch], eax
        add    edx, 3ch
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [ecx + 40h]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 20h], 0
        mov    dword ptr [esp + 24h], edx
        mov    dword ptr [esp + 28h], 18h
        mov    word ptr [esp + 2ch], 0dh
        mov    word ptr [esp + 2eh], 0cd0bh
        mov    dword ptr [esp + 30h], 0
        mov    dword ptr [esp + 34h], 400h
        mov    dword ptr [edx], 0
        mov    al, byte ptr [ecx + 70h]
        mov    byte ptr [edx + 12h], al
        mov    eax, dword ptr [ecx + 44h]
        push   ecx
        mov    bl, byte ptr [eax + 4]
        mov    byte ptr [edx + 10h], bl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [edx + 11h], al
        mov    byte ptr [edx + 13h], 0ffh
        call   sub_3_84F0
        lea    eax, [esp + 8]
        add    esp, 4
        push   eax
        call   sub_3_30C0
        add    esp, 4
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_3_2000:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 18h]
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 8]
        add    edx, 3ch
        cmp    dword ptr [eax + 3ch], 0
        jne    NEAR PTR loc_3_20E0
        mov    esi, dword ptr [ecx + 44h]
        xor    edi, edi
        mov    di, word ptr [ecx + 70h]
        xor    eax, eax
        mov    al, byte ptr [esi + 4]
        cmp    eax, edi
        jne    SHORT loc_3_203A
        mov    eax, dword ptr [edx + 4]
        mov    byte ptr [esi + 6], al
        mov    dword ptr [ecx + 0a0h], eax
loc_3_203A:
        xor    eax, eax
        mov    al, byte ptr [esi + 5]
        cmp    eax, edi
        jne    SHORT loc_3_209A
        mov    eax, dword ptr [edx + 14h]
        add    eax, 10h
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [esp + 10h], eax
        movzx  esi, byte ptr [esp + 12h]
        shl    esi, 2
        xor    eax, eax
        lea    esi, [esi + esi*2]
        mov    al, byte ptr [esp + 11h]
        lea    esi, [esi + esi*4]
        add    esi, eax
        xor    eax, eax
        lea    esi, [esi + esi*4]
        mov    al, byte ptr [esp + 10h]
        lea    esi, [esi + esi*2]
        lea    esi, [esi + esi*4]
        add    esi, eax
        sub    esi, 96h
        mov    dword ptr [esp + 10h], esi
        mov    eax, esi
        mov    dword ptr [ecx + 0ach], eax
        mov    eax, dword ptr [ecx + 44h]
        mov    ebx, dword ptr [edx + 8]
        mov    byte ptr [eax + 7], bl
        mov    dword ptr [ecx + 98h], ebx
loc_3_209A:
        mov    eax, dword ptr [ecx + 44h]
        mov    bl, byte ptr [edx + 10h]
        mov    byte ptr [eax + 4], bl
        mov    dl, byte ptr [edx + 11h]
        mov    byte ptr [eax + 5], dl
        cmp    word ptr [ecx + 70h], 0ffh
        jne    SHORT loc_3_20C7
        cmp    dl, bl
        je     SHORT loc_3_20C7
        mov    al, bl
        and    eax, 0ffff00ffh
        mov    word ptr [ecx + 70h], ax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_3_20C7:
        mov    dword ptr [ecx + 24h], 0
        push   ecx
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_3_20E0:
        mov    eax, dword ptr [ecx + 44h]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_3_20F1
        mov    dword ptr [ecx + 24h], 0bh
        jmp    SHORT loc_3_20F8
loc_3_20F1:
        mov    dword ptr [ecx + 24h], 0
loc_3_20F8:
        push   ecx
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_2110:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_3_2123
        mov    word ptr [esi + 2eh], 15h
        pop    esi
        ret
loc_3_2123:
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   esi
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [esi + 44h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 48h], eax
        mov    dword ptr [esi + 58h], 0
        mov    dword ptr [esi + 5ch], 0
        mov    dword ptr [esi + 68h], 1
        mov    byte ptr [esi + 60h], 0ffh
        mov    byte ptr [esi + 61h], 0
        mov    byte ptr [esi + 62h], 0ffh
        push   18h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_2170:
        mov    eax, dword ptr [esp + 4]
        push   0ah
        push   eax
        call   sub_3_8380
        add    esp, 8
        ret
loc_3_2180:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_3_21C5
        xor    eax, eax
        mov    al, byte ptr [esi + 61h]
        or     eax, eax
        je     SHORT loc_3_21A4
        cmp    eax, 4
        jl     SHORT loc_3_21A0
        cmp    eax, 5
        jle    SHORT loc_3_21A8
loc_3_21A0:
        or     byte ptr [esi + 6fh], 8
loc_3_21A4:
        mov    byte ptr [esi + 61h], 3
loc_3_21A8:
        mov    dword ptr [esi + 24h], 0
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_21C5:
        mov    edi, dword ptr [esi + 4ch]
        db  0B8h, 044h, 0A3h, 000h, 000h
        lea    ecx, [edi + 9]
        mov    edx, dword ptr [eax]
        cmp    edx, dword ptr [ecx]
        jne    SHORT loc_3_21DC
        mov    al, byte ptr [eax + 4]
        cmp    al, byte ptr [ecx + 4]
loc_3_21DC:
        mov    eax, 0
        sete   al
        db  0B9h, 04Ch, 0A3h, 000h, 000h
        lea    ebx, [edi + 1]
        mov    edx, dword ptr [ecx]
        cmp    edx, dword ptr [ebx]
        jne    SHORT loc_3_21F8
        mov    cl, byte ptr [ecx + 4]
        cmp    cl, byte ptr [ebx + 4]
loc_3_21F8:
        mov    ecx, 0
        sete   cl
        or     eax, eax
        jne    SHORT loc_3_224A
        or     ecx, ecx
        jne    SHORT loc_3_224A
        test   byte ptr [esi + 23h], 4
        jne    SHORT loc_3_2220
        mov    al, byte ptr [esi + 61h]
        cmp    al, 4
        je     SHORT loc_3_2219
        cmp    al, 5
        jne    SHORT loc_3_2220
loc_3_2219:
        or     dword ptr [esi + 20h], 4000000h
loc_3_2220:
        cmp    byte ptr [esi + 61h], 0
        je     SHORT loc_3_222C
        test   byte ptr [esi + 23h], 4
        jne    SHORT loc_3_2234
loc_3_222C:
        or     byte ptr [esi + 6fh], 8
        mov    byte ptr [esi + 61h], 3
loc_3_2234:
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_224A:
        test   byte ptr [esi + 23h], 4
        je     SHORT loc_3_2266
        push   esi
        call   sub_3_75B0
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_2266:
        mov    byte ptr [esi + 62h], cl
        or     ecx, ecx
        je     SHORT loc_3_2273
        xor    eax, eax
        mov    al, byte ptr [edi]
        jmp    SHORT loc_3_2278
loc_3_2273:
        xor    eax, eax
        mov    al, byte ptr [edi + 8]
loc_3_2278:
        or     ecx, ecx
        je     SHORT loc_3_2283
        xor    edx, edx
        mov    dl, byte ptr [edi + 6]
        jmp    SHORT loc_3_2288
loc_3_2283:
        xor    edx, edx
        mov    dl, byte ptr [edi + 0eh]
loc_3_2288:
        cmp    byte ptr [esi + 60h], 0ffh
        jne    SHORT loc_3_2291
        mov    byte ptr [esi + 60h], cl
loc_3_2291:
        cmp    eax, 1
        je     SHORT loc_3_22B0
        cmp    eax, 2
        je     SHORT loc_3_22C9
        cmp    eax, 0ffh
        je     NEAR PTR loc_3_2332
        jmp    NEAR PTR loc_3_233D
        db  005h, 000h, 000h, 000h, 000h
loc_3_22B0:
        cmp    edx, 1
        jne    NEAR PTR loc_3_233D
        cmp    byte ptr [esi + 61h], 0
        jne    NEAR PTR loc_3_233D
        mov    byte ptr [esi + 61h], 4
        jmp    SHORT loc_3_22FF
loc_3_22C9:
        cmp    edx, 1
        jne    SHORT loc_3_233D
        or     ecx, ecx
        lea    eax, [edi + 58h]
        jne    SHORT loc_3_22D8
        lea    eax, [edi + 60h]
loc_3_22D8:
        push   eax
        call   sub_3_1DE0
        add    esp, 4
        cmp    eax, 0ffffh
        je     SHORT loc_3_233D
        cmp    eax, 4b0h
        je     SHORT loc_3_22F8
        cmp    dword ptr [esi + 68h], 4b0h
        je     SHORT loc_3_233D
loc_3_22F8:
        mov    dword ptr [esi + 68h], eax
        mov    byte ptr [esi + 61h], 5
loc_3_22FF:
        mov    eax, dword ptr [esi + 58h]
        or     eax, eax
        je     SHORT loc_3_2319
        mov    dword ptr [esi + 4ch], eax
        mov    dword ptr [esi + 50h], 4
        push   esi
        call   sub_3_75B0
        add    esp, 4
loc_3_2319:
        mov    eax, dword ptr [esi + 48h]
        mov    dword ptr [esi + 5ch], eax
        mov    dword ptr [esi + 58h], edi
        mov    dword ptr [esi + 4ch], 0
        mov    dword ptr [esi + 50h], 0
        jmp    SHORT loc_3_233D
loc_3_2332:
        or     dword ptr [esi + 20h], 4000000h
        mov    byte ptr [esi + 60h], 0ffh
loc_3_233D:
        push   esi
        call   sub_3_75B0
        add    esp, 4
        inc    dword ptr [esi + 48h]
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_3_2350:
        mov    ecx, dword ptr [esp + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 61h]
        cmp    eax, 3
        je     SHORT loc_3_2391
        cmp    eax, 4
        jl     SHORT loc_3_2368
        cmp    eax, 5
        jle    SHORT loc_3_2370
loc_3_2368:
        mov    word ptr [ecx + 2eh], 15h
        ret
        db  090h
loc_3_2370:
        mov    eax, dword ptr [ecx + 5ch]
        mov    dword ptr [ecx + 48h], eax
        mov    eax, dword ptr [ecx + 58h]
        mov    dword ptr [ecx + 4ch], eax
        mov    dword ptr [ecx + 54h], 0
        mov    dword ptr [ecx + 50h], 4
        mov    dword ptr [ecx + 58h], 0
loc_3_2391:
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_23A0:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 28h]
        mov    al, byte ptr [edx + 61h]
        cmp    al, 4
        je     SHORT loc_3_23D0
        cmp    al, 5
        je     SHORT loc_3_23D0
        cmp    al, 3
        je     NEAR PTR loc_3_2490
        mov    word ptr [edx + 2eh], 15h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_23D0:
        cmp    al, 3
        je     NEAR PTR loc_3_2490
        mov    edx, dword ptr [esp + 28h]
        xor    eax, eax
        mov    al, byte ptr [edx + 62h]
        mov    ebx, dword ptr [edx + 4ch]
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     SHORT loc_3_23F4
        mov    esi, dword ptr [ebx + 84h]
        jmp    SHORT loc_3_23FA
loc_3_23F4:
        mov    esi, dword ptr [ebx + 8ch]
loc_3_23FA:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_2410
        mov    edi, dword ptr [ebx + 8ch]
        jmp    SHORT loc_3_2416
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2410:
        mov    edi, dword ptr [ebx + 94h]
loc_3_2416:
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_2428
        xor    eax, eax
        mov    ax, word ptr [ebx + 80h]
        jmp    SHORT loc_3_2431
loc_3_2428:
        xor    eax, eax
        mov    ax, word ptr [ebx + 88h]
loc_3_2431:
        push   eax
        call   sub_3_6860
        mov    dword ptr [esp + 18h], eax
        mov    edx, dword ptr [esp + 2ch]
        add    esp, 4
        mov    eax, dword ptr [edx + 44h]
        mov    ebp, edi
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 1ch], eax
        mov    dl, al
        mov    al, dl
        sub    al, byte ptr [esp + 14h]
        mov    cl, al
        shr    ebp, cl
        shl    ebp, cl
        sub    edi, ebp
        mov    cl, byte ptr [esp + 14h]
        shl    edi, cl
        mov    ebp, 1
        mov    cl, dl
        shl    ebp, cl
        add    edi, ebp
        add    edi, esi
        dec    edi
        shr    edi, cl
        or     edi, edi
        je     NEAR PTR loc_3_2760
        lea    eax, [edi*2 - 1]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [eax + eax*2 + 51h]
        shl    eax, 2
        jmp    SHORT loc_3_24B3
loc_3_2490:
        mov    edx, dword ptr [esp + 28h]
        mov    dword ptr [esp + 14h], 0bh
        mov    eax, dword ptr [edx + 98h]
        sub    eax, dword ptr [edx + 0a0h]
        lea    eax, [eax*8 + 14ch]
        mov    ebx, dword ptr [esp + 20h]
loc_3_24B3:
        push   1
        push   eax
        call   sub_3_43B0
        mov    ebp, eax
        mov    edx, dword ptr [esp + 30h]
        add    esp, 8
        mov    dword ptr [edx + 14h], ebp
        or     ebp, ebp
        je     NEAR PTR loc_3_2760
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebp + 4], eax
        mov    eax, dword ptr [edx + 40h]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [edx + 44h]
        mov    dword ptr [ebp + 0ch], eax
        mov    word ptr [ebp + 1ch], 1
        mov    eax, dword ptr [edx + 48h]
        mov    dword ptr [ebp + 38h], eax
        mov    eax, dword ptr [edx + 68h]
        mov    dword ptr [ebp + 4ch], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebp + 44h], eax
        cmp    byte ptr [edx + 61h], 3
        jne    NEAR PTR loc_3_25E5
        mov    cl, byte ptr [ebp + 1fh]
        or     cl, 2
        mov    byte ptr [ebp + 1fh], cl
        mov    byte ptr [ebp + 1eh], 1
        mov    edi, dword ptr [edx + 0a0h]
        mov    dword ptr [ebp + 138h], edi
        mov    esi, dword ptr [edx + 98h]
        mov    dword ptr [ebp + 13ch], esi
        sub    esi, edi
        mov    eax, dword ptr [edx + 0ach]
        mov    dword ptr [ebp + 140h], eax
        mov    eax, dword ptr [edx + 0ach]
        mov    dword ptr [ebp + 48h], eax
        inc    esi
        mov    dword ptr [ebp + 130h], esi
        lea    eax, [ebp + 144h]
        mov    dword ptr [ebp + 134h], eax
        mov    dword ptr [ebp + 104h], 3
        test   byte ptr [edx + 6fh], 8
        je     SHORT loc_3_2580
        or     cl, 20h
        mov    byte ptr [ebp + 1fh], cl
        db  0C7h, 085h, 008h, 001h, 000h, 000h, 00Ch, 0A0h, 000h, 000h
        db  0BEh, 0E0h, 09Eh, 000h, 000h
        jmp    SHORT loc_3_258F
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_2580:
        db  0C7h, 085h, 008h, 001h, 000h, 000h, 030h, 0A0h, 000h, 000h
        db  0BEh, 000h, 09Fh, 000h, 000h
loc_3_258F:
        lea    edi, [ebp + 58h]
        mov    ecx, 8
        rep movsd
        db  0BEh, 020h, 09Fh, 000h, 000h
        lea    edi, [ebp + 10ch]
        mov    ecx, 8
        rep movsd
        movsw
        lea    eax, [ebp + 50h]
        push   eax
        db  068h, 056h, 09Fh, 000h, 000h
        call   sub_3_5300
        add    esp, 8
        cmp    dword ptr [ebp + 130h], 2
        jbe    SHORT loc_3_25D3
        mov    dword ptr [ebp + 3ch], 0
        jmp    NEAR PTR loc_3_274A
loc_3_25D3:
        mov    edx, dword ptr [esp + 28h]
        mov    eax, dword ptr [edx + 0ach]
        mov    dword ptr [ebp + 3ch], eax
        jmp    NEAR PTR loc_3_274A
loc_3_25E5:
        mov    eax, 1
        mov    cl, byte ptr [esp + 1ch]
        shl    eax, cl
        push   eax
        push   ebx
        call   sub_3_6890
        add    esp, 8
        mov    dword ptr [ebp + 3ch], eax
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_260B
        xor    eax, eax
        mov    al, byte ptr [ebx + 7]
        jmp    SHORT loc_3_2610
loc_3_260B:
        xor    eax, eax
        mov    al, byte ptr [ebx + 0fh]
loc_3_2610:
        mov    dword ptr [ebp + 40h], eax
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_2620
        mov    eax, dword ptr [ebx + 50h]
        jmp    SHORT loc_3_2623
        db  090h
loc_3_2620:
        mov    eax, dword ptr [ebx + 58h]
loc_3_2623:
        mov    dword ptr [ebp + 48h], eax
        cmp    dword ptr [esp + 10h], 0
        lea    eax, [ebx + 32dh]
        jne    SHORT loc_3_2639
        lea    eax, [ebx + 316h]
loc_3_2639:
        lea    ecx, [ebp + 50h]
        push   ecx
        push   eax
        call   sub_3_5220
        add    esp, 8
        lea    esi, [ebx + 28h]
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_3_2653
        lea    esi, [ebx + 30h]
loc_3_2653:
        lea    edi, [ebp + 58h]
        mov    ecx, 8
        rep movsd
        cmp    dword ptr [esp + 10h], 0
        lea    esi, [ebx + 0beh]
        jne    SHORT loc_3_2670
        lea    esi, [ebx + 0d6h]
loc_3_2670:
        lea    edi, [ebp + 78h]
        mov    ecx, 20h
        rep movsd
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_2687
        mov    ax, word ptr [ebx + 78h]
        jmp    SHORT loc_3_268E
loc_3_2687:
        mov    ax, word ptr [ebx + 80h]
loc_3_268E:
        mov    word ptr [ebp + 0f8h], ax
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_26A2
        mov    ax, word ptr [ebx + 7ch]
        jmp    SHORT loc_3_26A9
loc_3_26A2:
        mov    ax, word ptr [ebx + 84h]
loc_3_26A9:
        mov    word ptr [ebp + 0fah], ax
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_26C0
        mov    eax, dword ptr [ebx + 8ch]
        jmp    SHORT loc_3_26C6
        db  090h
loc_3_26C0:
        mov    eax, dword ptr [ebx + 94h]
loc_3_26C6:
        mov    dword ptr [ebp + 0fch], eax
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_26E0
        mov    eax, dword ptr [ebx + 84h]
        jmp    SHORT loc_3_26E6
        db  005h, 000h, 000h, 000h, 000h
loc_3_26E0:
        mov    eax, dword ptr [ebx + 8ch]
loc_3_26E6:
        mov    dword ptr [ebp + 100h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebp + 104h], eax
        lea    eax, [ebp + 144h]
        mov    dword ptr [ebp + 108h], eax
        cmp    dword ptr [esp + 10h], 0
        lea    esi, [ebx + 9ch]
        jne    SHORT loc_3_2715
        lea    esi, [ebx + 0b4h]
loc_3_2715:
        lea    edi, [ebp + 10ch]
        mov    ecx, 8
        rep movsd
        movsw
        mov    al, byte ptr [esp + 10h]
        mov    byte ptr [ebp + 1eh], al
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_274A
        lea    esi, [ebx + 400h]
        db  0BFh, 054h, 0A3h, 000h, 000h
        mov    ecx, 2
        repe cmpsd
        jne    SHORT loc_3_274A
        or     byte ptr [ebp + 1fh], 4
loc_3_274A:
        mov    edx, dword ptr [esp + 28h]
        push   edx
        call   sub_3_75B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
loc_3_2760:
        mov    edx, dword ptr [esp + 28h]
        pop    ebp
        mov    dword ptr [edx + 24h], 8
        mov    word ptr [edx + 2eh], 15h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2780:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esi + 14h]
        test   byte ptr [ebx + 1fh], 2
        je     SHORT loc_3_27D0
        push   esi
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_3_36E0
        add    esp, 4
        mov    dword ptr [esi + 8], eax
        or     eax, eax
        jne    SHORT loc_3_27C0
        mov    dword ptr [esi + 24h], 8
        mov    word ptr [esi + 2eh], 15h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_3_27C0:
        mov    dword ptr [eax + 40h], esi
        mov    dword ptr [esi + 0a4h], 0
        jmp    SHORT loc_3_2809
        db  090h
loc_3_27D0:
        mov    word ptr [esi + 74h], 1
        mov    word ptr [esi + 64h], 0
        mov    edi, dword ptr [ebx + 0fch]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dl, byte ptr [ebx + 44h]
        mov    ecx, dword ptr [eax + 0ch]
        sub    cl, dl
        mov    eax, edi
        shr    edi, cl
        shl    edi, cl
        sub    eax, edi
        mov    cl, dl
        shl    eax, cl
        add    eax, dword ptr [ebx + 100h]
        mov    dword ptr [esi + 98h], eax
        mov    byte ptr [esi + 63h], 7
loc_3_2809:
        push   1ah
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_2820:
        sub    esp, 34h
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 40h]
        mov    esi, dword ptr [edx + 14h]
        test   byte ptr [esi + 1fh], 2
        je     NEAR PTR loc_3_28D3
        mov    ecx, dword ptr [edx]
        mov    dword ptr [esp + 8], 0
        mov    dword ptr [esp + 0ch], 0
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esp + 10h], eax
        add    ecx, 3ch
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [edx + 40h]
        mov    dword ptr [esp + 18h], eax
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 20h], 0
        mov    dword ptr [esp + 24h], 0
        mov    dword ptr [esp + 28h], ecx
        mov    dword ptr [esp + 2ch], 10h
        mov    word ptr [esp + 30h], 0dh
        mov    word ptr [esp + 32h], 0cd0ch
        mov    dword ptr [esp + 34h], 0
        mov    dword ptr [esp + 38h], 400h
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [esi + 138h]
        add    eax, dword ptr [edx + 0a4h]
        mov    dword ptr [ecx + 4], eax
        push   edx
        call   sub_3_84F0
        lea    eax, [esp + 0ch]
        add    esp, 4
        push   eax
        call   sub_3_30C0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 34h
        ret
loc_3_28D3:
        mov    byte ptr [edx + 63h], 7
        mov    eax, dword ptr [esi + 0ch]
        mov    edi, dword ptr [esi + 0fch]
        mov    ecx, dword ptr [eax + 0ch]
        xor    eax, eax
        sub    cl, byte ptr [esi + 44h]
        mov    ax, word ptr [edx + 64h]
        shr    edi, cl
        inc    eax
        sar    eax, 1
        add    edi, eax
        mov    dword ptr [edx + 48h], edi
        push   0ah
        push   edx
        call   sub_3_8380
        add    esp, 8
        pop    edi
        pop    esi
        add    esp, 34h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2910:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 48h]
        mov    edx, dword ptr [ebx + 14h]
        mov    cl, byte ptr [edx + 1fh]
        and    cl, 2
        je     SHORT loc_3_292F
        mov    eax, dword ptr [ebx + 8]
        mov    eax, dword ptr [eax + 3ch]
        mov    dword ptr [ebx + 24h], eax
loc_3_292F:
        cmp    dword ptr [ebx + 24h], 0
        je     SHORT loc_3_2946
        push   ebx
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
loc_3_2946:
        or     cl, cl
        je     NEAR PTR loc_3_29F5
        mov    ecx, dword ptr [edx + 134h]
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 0a4h]
        mov    eax, dword ptr [eax + 44h]
        mov    dword ptr [esp + 40h], eax
        mov    eax, dword ptr [ebx + 14h]
        mov    esi, dword ptr [esp + 40h]
        add    dword ptr [eax + 3ch], esi
        xor    eax, eax
        movzx  esi, byte ptr [esp + 42h]
        shl    esi, 2
        mov    al, byte ptr [esp + 41h]
        lea    esi, [esi + esi*2]
        lea    esi, [esi + esi*4]
        add    esi, eax
        xor    eax, eax
        lea    esi, [esi + esi*4]
        mov    al, byte ptr [esp + 40h]
        lea    esi, [esi + esi*2]
        lea    esi, [esi + esi*4]
        add    esi, eax
        sub    esi, 96h
        mov    dword ptr [esp + 40h], esi
        mov    eax, esi
        mov    dword ptr [ecx + edx*8], eax
        mov    edi, dword ptr [ebx + 14h]
        inc    dword ptr [ebx + 0a4h]
        mov    edx, dword ptr [edi + 130h]
        cmp    dword ptr [ebx + 0a4h], edx
        jb     NEAR PTR loc_3_2D80
        sub    esi, esi
        dec    edx
        je     SHORT loc_3_29D6
        lea    eax, [ecx + 4]
        mov    esi, edx
loc_3_29C8:
        mov    ebp, dword ptr [eax + 4]
        sub    ebp, dword ptr [eax - 4]
        mov    dword ptr [eax], ebp
        add    eax, 8
        dec    edx
        jne    SHORT loc_3_29C8
loc_3_29D6:
        lea    eax, [ecx + esi*8]
        mov    ecx, dword ptr [edi + 140h]
        sub    ecx, dword ptr [eax]
        mov    dword ptr [eax + 4], ecx
        push   ebx
        call   sub_3_83B0
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
loc_3_29F5:
        xor    eax, eax
        mov    al, byte ptr [edx + 1eh]
        mov    ecx, dword ptr [edx + 0ch]
        mov    dword ptr [esp + 14h], eax
        mov    ecx, dword ptr [ecx + 0ch]
        mov    dword ptr [esp + 34h], ecx
        mov    ecx, dword ptr [edx + 108h]
        mov    dword ptr [esp + 30h], ecx
        mov    cx, word ptr [ebx + 64h]
        mov    eax, dword ptr [edx + 44h]
        mov    esi, ecx
        and    esi, 0ffffh
        mov    dword ptr [esp + 24h], esi
        mov    ebp, dword ptr [ebx + 4ch]
        or     cx, cx
        je     NEAR PTR loc_3_2BA2
        mov    ecx, dword ptr [esp + 30h]
        lea    eax, [esi + esi*2]
        lea    edx, [ecx + eax*4]
        mov    dword ptr [esp + 2ch], edx
        mov    ax, word ptr [ebx + 74h]
        mov    word ptr [edx], ax
        mov    ax, word ptr [edx + 4]
        or     ax, ax
        je     NEAR PTR loc_3_2B80
        mov    dx, 1
        mov    cl, byte ptr [esp + 34h]
        shl    dx, cl
        sub    dx, ax
        mov    word ptr [esp + 10h], dx
        mov    edx, dword ptr [esp + 2ch]
        cmp    word ptr [esp + 10h], 8
        mov    eax, dword ptr [edx + 8]
        jae    NEAR PTR loc_3_2B10
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0ffffh
        mov    dword ptr [esp + 38h], eax
        mov    ecx, dword ptr [ebx]
        lea    edi, [eax + ecx + 3ch]
        mov    eax, 8
        sub    eax, dword ptr [esp + 38h]
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    dword ptr [esp + 14h], 0
        mov    eax, dword ptr [ebx]
        je     SHORT loc_3_2AB5
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 3ch]
        jmp    SHORT loc_3_2ABA
loc_3_2AB5:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 41h]
loc_3_2ABA:
        cmp    dword ptr [esp + 14h], 0
        mov    eax, dword ptr [ebx]
        je     SHORT loc_3_2AC8
        mov    al, byte ptr [eax + 3ch]
        jmp    SHORT loc_3_2ACB
loc_3_2AC8:
        mov    al, byte ptr [eax + 41h]
loc_3_2ACB:
        and    eax, 0ffh
        and    eax, 1
        lea    ecx, [ecx + eax + 8]
        mov    dword ptr [esp + 1ch], ecx
        push   0
        push   ecx
        call   sub_3_43B0
        mov    edx, dword ptr [esp + 34h]
        add    esp, 8
        mov    dword ptr [edx + 8], eax
        or     eax, eax
        je     NEAR PTR loc_3_2E66
        mov    edx, dword ptr [esp + 38h]
        mov    esi, dword ptr [ebx]
        mov    edi, eax
        add    esi, 3ch
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_3_2B42
loc_3_2B10:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_2B20
        xor    edx, edx
        mov    dl, byte ptr [eax]
        jmp    SHORT loc_3_2B25
        db  08Dh, 049h, 000h
loc_3_2B20:
        xor    edx, edx
        mov    dl, byte ptr [eax + 5]
loc_3_2B25:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_2B32
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        jmp    SHORT loc_3_2B37
loc_3_2B32:
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 5]
loc_3_2B37:
        and    ecx, 1
        lea    ecx, [edx + ecx + 8]
        mov    dword ptr [esp + 1ch], ecx
loc_3_2B42:
        mov    ecx, dword ptr [esp + 10h]
        and    ecx, 0ffffh
        mov    edx, dword ptr [esp + 1ch]
        sub    edx, ecx
        lea    edi, [eax + ecx]
        mov    esi, ebp
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 1ch]
        inc    word ptr [ebx + 74h]
        sub    ax, word ptr [esp + 10h]
        mov    word ptr [esp + 10h], ax
        jmp    SHORT loc_3_2B8F
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2B80:
        mov    word ptr [esp + 10h], 0
        mov    dword ptr [esp + 1ch], 0
loc_3_2B8F:
        mov    edx, dword ptr [esp + 2ch]
        mov    cx, word ptr [esp + 1ch]
        inc    dword ptr [esp + 24h]
        mov    word ptr [edx + 2], cx
        jmp    SHORT loc_3_2BC2
loc_3_2BA2:
        mov    edx, dword ptr [edx + 0fch]
        mov    ecx, dword ptr [esp + 34h]
        sub    cl, al
        mov    esi, edx
        shr    edx, cl
        shl    dx, cl
        sub    si, dx
        mov    cl, al
        shl    si, cl
        mov    word ptr [esp + 10h], si
loc_3_2BC2:
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebx + 98h]
        jbe    SHORT loc_3_2C05
        cmp    dword ptr [esp + 24h], 0
        je     NEAR PTR loc_3_2E66
        mov    esi, dword ptr [esp + 24h]
        dec    word ptr [ebx + 74h]
        mov    ecx, dword ptr [esp + 30h]
        lea    eax, [esi + esi*2]
        mov    word ptr [ecx + eax*4 - 0ah], 0
        mov    dword ptr [ebx + 98h], 0
        mov    word ptr [esp + 10h], 0
loc_3_2C05:
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0ffffh
        mov    edx, dword ptr [ebx + 98h]
        sub    edx, eax
        mov    dword ptr [ebx + 98h], edx
        add    ebp, eax
        mov    ax, word ptr [ebx + 74h]
        mov    esi, dword ptr [esp + 24h]
        mov    word ptr [esp + 12h], ax
        mov    ecx, dword ptr [esp + 30h]
        lea    eax, [esi + esi*2]
        lea    esi, [ecx + eax*4]
        mov    dword ptr [esp + 18h], esi
        mov    ax, word ptr [esp + 12h]
        mov    word ptr [esi], ax
        mov    ax, word ptr [esp + 10h]
        or     edx, edx
        mov    word ptr [esi + 4], ax
        je     NEAR PTR loc_3_2D18
        mov    eax, 1
        mov    cl, byte ptr [esp + 34h]
        shl    eax, cl
        mov    dword ptr [esp + 28h], eax
loc_3_2C61:
        mov    esi, dword ptr [esp + 10h]
        and    esi, 0ffffh
        mov    eax, dword ptr [esp + 28h]
        sub    eax, esi
        cmp    eax, 8
        jl     NEAR PTR loc_3_2D18
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_2C88
        xor    edx, edx
        mov    dl, byte ptr [ebp]
        jmp    SHORT loc_3_2C8D
loc_3_2C88:
        xor    edx, edx
        mov    dl, byte ptr [ebp + 5]
loc_3_2C8D:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_2C9B
        xor    eax, eax
        mov    al, byte ptr [ebp]
        jmp    SHORT loc_3_2CA0
loc_3_2C9B:
        xor    eax, eax
        mov    al, byte ptr [ebp + 5]
loc_3_2CA0:
        and    eax, 1
        lea    edx, [edx + eax + 8]
        mov    eax, 1
        shl    eax, cl
        sub    eax, esi
        cmp    eax, edx
        jb     SHORT loc_3_2D18
        mov    edi, dword ptr [ebx + 98h]
        cmp    edx, edi
        ja     SHORT loc_3_2D03
        mov    ax, word ptr [ebx + 74h]
        inc    ax
        mov    word ptr [ebx + 74h], ax
        xor    esi, esi
        mov    si, word ptr [esp + 12h]
        and    eax, 0ffffh
        sub    eax, esi
        cmp    eax, 40h
        jne    SHORT loc_3_2CEC
        mov    eax, ebp
        mov    esi, dword ptr [esp + 18h]
        sub    eax, dword ptr [ebx + 4ch]
        sub    ax, word ptr [esi + 4]
        mov    word ptr [esi + 6], ax
loc_3_2CEC:
        add    word ptr [esp + 10h], dx
        add    ebp, edx
        sub    edi, edx
        mov    dword ptr [ebx + 98h], edi
        jne    NEAR PTR loc_3_2C61
        jmp    SHORT loc_3_2D18
loc_3_2D03:
        cmp    word ptr [ebx + 74h], 1
        jbe    NEAR PTR loc_3_2E66
        mov    dword ptr [ebx + 98h], 0
loc_3_2D18:
        mov    esi, dword ptr [esp + 18h]
        mov    ax, word ptr [esp + 10h]
        sub    ax, word ptr [esi + 4]
        mov    word ptr [esi + 2], ax
        mov    esi, dword ptr [ebx + 98h]
        or     esi, esi
        je     SHORT loc_3_2D6E
        mov    eax, 1
        mov    cl, byte ptr [esp + 34h]
        shl    eax, cl
        mov    edx, dword ptr [esp + 10h]
        and    edx, 0ffffh
        sub    eax, edx
        mov    dword ptr [esp + 1ch], eax
        mov    ax, 1
        shl    ax, cl
        dec    ax
        and    word ptr [esp + 10h], ax
        je     SHORT loc_3_2D88
        cmp    dword ptr [esp + 1ch], esi
        jb     SHORT loc_3_2D88
loc_3_2D64:
        mov    dword ptr [ebx + 98h], 0
loc_3_2D6E:
        push   ebx
        call   sub_3_83B0
        add    esp, 4
loc_3_2D77:
        push   ebx
        call   sub_3_75B0
        add    esp, 4
loc_3_2D80:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
loc_3_2D88:
        inc    dword ptr [esp + 24h]
        mov    esi, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esp + 30h]
        lea    eax, [esi + esi*2]
        lea    esi, [ecx + eax*4]
        mov    ax, word ptr [esp + 10h]
        mov    word ptr [esi + 4], ax
        or     ax, ax
        je     NEAR PTR loc_3_2E49
        mov    ax, word ptr [esp + 1ch]
        mov    word ptr [esp + 10h], ax
        cmp    ax, 8
        jae    SHORT loc_3_2DC3
        mov    edi, dword ptr [ebx]
        add    edi, 3ch
        jmp    SHORT loc_3_2E08
loc_3_2DC3:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_2DD1
        xor    eax, eax
        mov    al, byte ptr [ebp]
        jmp    SHORT loc_3_2DD6
loc_3_2DD1:
        xor    eax, eax
        mov    al, byte ptr [ebp + 5]
loc_3_2DD6:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_2DE4
        xor    ecx, ecx
        mov    cl, byte ptr [ebp]
        jmp    SHORT loc_3_2DE9
loc_3_2DE4:
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 5]
loc_3_2DE9:
        and    ecx, 1
        lea    eax, [eax + ecx + 8]
        mov    dword ptr [esp + 1ch], eax
        push   0
        push   eax
        call   sub_3_43B0
        add    esp, 8
        mov    edi, eax
        mov    dword ptr [esi + 8], edi
        or     edi, edi
        je     SHORT loc_3_2E66
loc_3_2E08:
        mov    eax, dword ptr [esp + 1ch]
        cmp    dword ptr [ebx + 98h], eax
        jae    SHORT loc_3_2E30
        mov    word ptr [esi + 2], 0
        mov    ax, word ptr [ebx + 74h]
        mov    word ptr [esi + 0ch], ax
        mov    word ptr [esi], ax
        jmp    NEAR PTR loc_3_2D64
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_2E30:
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0ffffh
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_2E49:
        mov    eax, dword ptr [esp + 10h]
        and    eax, 0ffffh
        mov    si, word ptr [esp + 24h]
        sub    dword ptr [ebx + 98h], eax
        mov    word ptr [ebx + 64h], si
        jmp    NEAR PTR loc_3_2D77
loc_3_2E66:
        mov    dword ptr [ebx + 24h], 8
        jmp    NEAR PTR loc_3_2D6E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2E80:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 24h], 0
        je     SHORT loc_3_2E90
        mov    word ptr [eax + 2eh], 15h
loc_3_2E90:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2EA0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 14h]
        test   byte ptr [ecx + 1fh], 2
        jne    SHORT loc_3_2EB8
        push   1ch
        push   eax
        call   sub_3_8380
        add    esp, 8
loc_3_2EB8:
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2EC0:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 24h], 0
        mov    ecx, dword ptr [eax + 14h]
        test   byte ptr [ecx + 1fh], 2
        je     SHORT loc_3_2EDD
        push   eax
        call   sub_3_83B0
        add    esp, 4
loc_3_2EDD:
        ret
        db  08Bh, 0FFh
loc_3_2EE0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    word ptr [eax + 20h], 0
        mov    dword ptr [eax + 28h], 1
        call   sub_3_7840
        add    esp, 4
        push   esi
        call   sub_3_83B0
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2F10:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 24h], 0
        je     SHORT loc_3_2F20
        mov    word ptr [eax + 2eh], 15h
loc_3_2F20:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2F30:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A0h, 050h, 030h, 000h, 000h
        db  08Bh, 02Dh, 0D0h, 09Bh, 000h, 000h
        or     ebp, ebp
        mov    ebx, dword ptr [esp + 14h]
        je     NEAR PTR loc_3_2FDB
        mov    edx, 2
loc_3_2F50:
        mov    edi, dword ptr [ebx + 14h]
        test   byte ptr [edi + 1fh], dl
        mov    eax, dword ptr [edi + 8]
        jne    SHORT loc_3_2F79
        cmp    dword ptr [ebp + 8], eax
        jne    SHORT loc_3_2FD0
        mov    eax, dword ptr [edi + 3ch]
        cmp    dword ptr [ebp + 3ch], eax
        jne    SHORT loc_3_2FD0
        lea    esi, [ebp + 58h]
        lea    edi, [edi + 58h]
        mov    ecx, 2bh
        repe cmpsd
        jne    SHORT loc_3_2FD0
        jmp    SHORT loc_3_2FDB
loc_3_2F79:
        cmp    dword ptr [ebp + 8], eax
        jne    SHORT loc_3_2FD0
        mov    eax, dword ptr [edi + 138h]
        cmp    dword ptr [ebp + 138h], eax
        jne    SHORT loc_3_2FD0
        mov    eax, dword ptr [edi + 13ch]
        cmp    dword ptr [ebp + 13ch], eax
        jne    SHORT loc_3_2FD0
        mov    eax, dword ptr [edi + 140h]
        cmp    dword ptr [ebp + 140h], eax
        jne    SHORT loc_3_2FD0
        mov    eax, dword ptr [edi + 3ch]
        cmp    dword ptr [ebp + 3ch], eax
        jne    SHORT loc_3_2FD0
        mov    eax, dword ptr [ebp + 130h]
        lea    ecx, [eax*8]
        shr    ecx, 2
        mov    esi, dword ptr [ebp + 134h]
        mov    edi, dword ptr [edi + 134h]
        repe cmpsd
        je     SHORT loc_3_2FDB
loc_3_2FD0:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    NEAR PTR loc_3_2F50
loc_3_2FDB:
        or     ebp, ebp
        jne    SHORT loc_3_3020
        mov    esi, dword ptr [ebx + 14h]
        db  0A1h, 0D0h, 09Bh, 000h, 000h
        mov    dword ptr [esi], eax
        db  089h, 035h, 0D0h, 09Bh, 000h, 000h
        push   esi
        call   sub_3_77B0
        add    esp, 4
        push   ebx
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 1ch], esi
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [eax + 14h], esi
        db  0C7h, 040h, 018h, 030h, 019h, 000h, 000h
        mov    dword ptr [ebx + 24h], 0
        jmp    SHORT loc_3_3035
        db  08Dh, 064h, 024h, 000h
loc_3_3020:
        mov    word ptr [ebx + 2eh], 15h
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [ecx + 1ch], eax
        mov    dword ptr [ebx + 24h], 11ch
loc_3_3035:
        mov    eax, dword ptr [ebx]
        pop    ebp
        db  0C7h, 040h, 010h, 044h, 009h, 000h, 000h
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 24h]
        mov    word ptr [eax + 1ah], cx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_3050:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3060:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_3_75B0
        add    esp, 4
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_3_3084
        push   esi
        call   sub_3_4660
        add    esp, 4
        mov    dword ptr [esi + 44h], 0
loc_3_3084:
        mov    eax, dword ptr [esi + 44h]
        or     eax, eax
        je     SHORT loc_3_30A1
        mov    ecx, dword ptr [esi + 20h]
        push   ecx
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0ch
        mov    dword ptr [esi + 44h], 0
loc_3_30A1:
        push   esi
        call   sub_3_6820
        add    esp, 4
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 24h]
        mov    word ptr [eax + 1ah], cx
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_30C0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 0ch]
        mov    ax, word ptr [esi + 28h]
        mov    word ptr [edi + 4], ax
        mov    word ptr [edi + 6], 0
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 44h], eax
        or     dword ptr [edi + 8], 8
        mov    al, byte ptr [esi + 10h]
        mov    byte ptr [edi + 4dh], al
        cmp    word ptr [esi + 28h], 0dh
        jne    SHORT loc_3_311D
        mov    eax, dword ptr [esi + 10h]
        inc    ax
        mov    word ptr [edi + 2ch], ax
        mov    ax, word ptr [esi + 2ah]
        mov    word ptr [edi + 2eh], ax
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [edi + 30h], eax
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [edi + 34h], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 38h], eax
        mov    dword ptr [edi + 3ch], 1
loc_3_311D:
        push   edi
        call   sub_3_57E0
        add    esp, 4
        cmp    word ptr [esi + 28h], 0dh
        jne    SHORT loc_3_3136
        mov    eax, dword ptr [edi + 3ch]
        mov    dword ptr [esi + 2ch], eax
        pop    edi
        pop    esi
        ret
loc_3_3136:
        mov    ax, word ptr [edi + 6]
        cmp    ax, 10h
        jb     SHORT loc_3_3154
        and    eax, 0ffffh
        push   eax
        call   sub_3_57F5
        add    esp, 4
        mov    dword ptr [esi + 2ch], eax
        pop    edi
        pop    esi
        ret
loc_3_3154:
        mov    dword ptr [esi + 2ch], 0
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_3_3160:
        mov    ecx, dword ptr [esp + 4]
        cmp    dword ptr [ecx + 8], 0
        je     SHORT loc_3_3197
        mov    eax, dword ptr [ecx + 8]
        mov    eax, dword ptr [eax + 1ch]
        sub    eax, 0c3h
        cmp    byte ptr [eax + 40h], 5
        jne    SHORT loc_3_3197
        test   byte ptr [eax + 47eh], 1
        je     SHORT loc_3_3197
        mov    eax, dword ptr [ecx + 14h]
        mov    ecx, dword ptr [ecx + 14h]
        mov    ax, word ptr [eax + 3]
        or     ax, 200h
        mov    word ptr [ecx + 3], ax
        ret
loc_3_3197:
        mov    eax, dword ptr [ecx + 14h]
        mov    ecx, dword ptr [ecx + 14h]
        mov    ax, word ptr [eax + 3]
        and    ax, 0fdffh
        mov    word ptr [ecx + 3], ax
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_3_31B0:
        db  0CDh, 020h, 02Fh, 080h, 040h, 000h
sub_3_31B6:
        db  0CDh, 020h, 030h, 080h, 040h, 000h
loc_3_31BC:
        push   ebx
        push   ebp
        call   sub_3_3240
        lea    esp, [esp + 8]
loc_3_31C7:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        ret
        db  000h, 000h
loc_3_31D0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        xor    eax, eax
        mov    al, byte ptr [esi + 18h]
        db  08Bh, 00Dh, 090h, 008h, 000h, 000h
        lea    edx, [eax + eax*2]
        lea    edi, [ecx + edx*2 + 14h]
        cmp    eax, 1ah
        jae    SHORT loc_3_3225
        mov    al, byte ptr [edi + 1]
        test   al, 8
        je     SHORT loc_3_3225
        test   al, 10h
        jne    SHORT loc_3_3200
        mov    eax, 1
        pop    edi
        pop    esi
        ret
loc_3_3200:
        push   3810h
        call   sub_3_57FF
        add    esp, 4
        mov    cl, byte ptr [edi]
        mov    byte ptr [eax + 1], cl
        mov    eax, dword ptr [esp + 10h]
        push   eax
        push   esi
        call   sub_3_3240
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        ret
loc_3_3225:
        mov    word ptr [esi + 1ch], 0fh
        or     dword ptr [esi + 2ch], 1
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3240:
        sub    esp, 34h
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [esp], eax
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [esp + 4], eax
        push   3810h
        call   sub_3_57FF
        mov    dword ptr [esp + 18h], eax
        mov    dword ptr [esp + 28h], 0
        mov    dword ptr [esp + 34h], 500h
        lea    eax, [esp + 4]
        add    esp, 4
        push   eax
        db  0E8h, 012h, 0D7h, 0FCh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_3290
        lea    ecx, [esp]
        push   ecx
        call   eax
        add    esp, 38h
        ret
loc_3_3290:
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax + 3], 8103h
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_3_32A0:
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [esp + 14h]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd04h
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 0ch
        mov    dword ptr [esp + 4], 0
        mov    eax, dword ptr [esi + 18h]
        push   esi
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [esp + 0ch], eax
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_32F0
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 2], ecx
loc_3_32F0:
        pop    esi
        add    esp, 0ch
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3300:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 10h]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd02h
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 8
        mov    dword ptr [esp + 4], 0
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_3341
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 1], eax
loc_3_3341:
        pop    esi
        add    esp, 8
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3350:
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [esp + 18h]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd07h
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 10h
        mov    dword ptr [esp + 4], 0
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_33A2
        mov    al, byte ptr [esp + 8]
        and    al, 0fh
        cmp    al, 1
        jne    SHORT loc_3_33C2
        lea    ecx, [esp + 9]
        mov    al, byte ptr [ecx]
        aam    0ah
        shl    ah, 4
        or     al, ah
        mov    byte ptr [ecx], al
        jmp    SHORT loc_3_33C2
loc_3_33A2:
        lea    eax, [esp + 8]
        push   esi
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    word ptr [eax + 8], 0
        call   sub_3_3900
        add    esp, 4
loc_3_33C2:
        mov    edx, dword ptr [esi + 18h]
        lea    ecx, [esp + 8]
        inc    edx
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [edx + 8], ax
        pop    esi
        add    esp, 10h
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_33F0:
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [esp + 18h]
        lea    eax, [esp + 4]
        mov    word ptr [esi + 28h], 0dh
        mov    word ptr [esi + 2ah], 0cd08h
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 10h
        mov    dword ptr [esp + 4], 0
        push   esi
        call   sub_3_37B0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_3_3446
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [ecx + 1], ax
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 3], eax
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 7], ecx
loc_3_3446:
        pop    esi
        add    esp, 10h
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_3450:
        sub    esp, 80h
        push   esi
        push   edi
        lea    eax, [esp + 14h]
        mov    esi, dword ptr [esp + 8ch]
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esi + 14h]
        mov    al, byte ptr [eax + 1]
        push   eax
        call   sub_3_3920
        add    esp, 4
        mov    dword ptr [esi + 10h], eax
        cmp    dword ptr [esi + 10h], -1
        je     NEAR PTR loc_3_35D1
        mov    eax, dword ptr [esi + 10h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   eax
        call   sub_3_9AC0
        mov    eax, dword ptr [esp + 18h]
        add    esp, 10h
        cmp    dword ptr [eax + 10h], 1
        sbb    eax, eax
        neg    eax
        or     eax, eax
        jne    NEAR PTR loc_3_35D1
        mov    eax, dword ptr [esp + 8]
        mov    edi, dword ptr [esi + 14h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [edi + 0eh]
        add    edi, 0dh
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        call   sub_3_5958
        mov    ecx, dword ptr [esp + 10h]
        add    esp, 8
        mov    dword ptr [ecx + 14h], eax
        xor    eax, eax
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [edi + 5]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        mov    al, byte ptr [edi]
        mov    dword ptr [esp + 0ch], eax
        mov    eax, dword ptr [edi + 7]
        mov    dword ptr [esp + 10h], eax
        cmp    dword ptr [esp + 0ch], 2
        jae    NEAR PTR loc_3_3590
        cmp    byte ptr [edi + 0bh], 2
        jae    NEAR PTR loc_3_3590
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_3_3547
        xor    ecx, ecx
        mov    cl, byte ptr [esp + 12h]
        shl    ecx, 2
        mov    dword ptr [esp + 0ch], 0
        lea    ecx, [ecx + ecx*2]
        xor    eax, eax
        mov    al, byte ptr [esp + 11h]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        xor    eax, eax
        lea    ecx, [ecx + ecx*4]
        mov    al, byte ptr [esp + 10h]
        lea    ecx, [ecx + ecx*2]
        lea    ecx, [ecx + ecx*4]
        add    ecx, eax
        sub    ecx, 96h
        mov    dword ptr [esp + 10h], ecx
loc_3_3547:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 24h], eax
        xor    eax, eax
        mov    ecx, dword ptr [esp + 8]
        mov    al, byte ptr [edi + 0bh]
        push   40h
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [esp + 0ch]
        push   5
        mov    dword ptr [eax + 3ch], 0
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_3_55E0
        add    esp, 0ch
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [esp + 8]
        movsx  eax, word ptr [eax + 1ah]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_3_3599
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3590:
        lea    ecx, [esi + 2ch]
        mov    dword ptr [ecx], 16h
loc_3_3599:
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_3_35A9
        mov    eax, dword ptr [esi + 14h]
        mov    word ptr [eax + 3], 100h
        jmp    SHORT loc_3_35B2
loc_3_35A9:
        push   esi
        call   sub_3_3820
        add    esp, 4
loc_3_35B2:
        push   esi
        call   sub_3_3160
        mov    eax, dword ptr [esp + 0ch]
        add    esp, 4
        push   eax
        call   sub_3_9BA0
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 80h
        ret
loc_3_35D1:
        mov    eax, dword ptr [esi + 14h]
        pop    edi
        mov    word ptr [eax + 3], 8101h
        pop    esi
        add    esp, 80h
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_35F0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        push   1
        lea    esi, [eax + 8]
        push   esi
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_3_31B0
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_3611
        mov    dword ptr [esi], 0
loc_3_3611:
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3620:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_3_31B6
        add    esp, 8
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3640:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        mov    al, byte ptr [eax + 1]
        push   eax
        call   sub_3_3920
        add    esp, 4
        mov    dword ptr [esi + 10h], eax
        cmp    dword ptr [esi + 10h], -1
        je     SHORT loc_3_36A4
        push   esi
        call   sub_3_35F0
        add    esp, 4
        cmp    dword ptr [esi + 8], 1
        sbb    eax, eax
        neg    eax
        or     eax, eax
        jne    SHORT loc_3_36A4
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_3_36E0
        add    esp, 4
        mov    dword ptr [esi + 0ch], eax
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_3_36A0
        push   esi
        call   sub_3_3620
        add    esp, 4
        mov    eax, dword ptr [esi + 14h]
        pop    esi
        mov    word ptr [eax + 3], 810ch
        mov    eax, 1
        ret
loc_3_36A0:
        sub    eax, eax
        pop    esi
        ret
loc_3_36A4:
        mov    eax, dword ptr [esi + 14h]
        pop    esi
        mov    word ptr [eax + 3], 8101h
        mov    eax, 1
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_36C0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        call   sub_3_3780
        add    esp, 4
        push   esi
        call   sub_3_3620
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
sub_3_36E0:
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [esp + 18h]
        mov    word ptr [esp + 4], 2
        mov    ax, word ptr [esi + 8]
        shl    ax, 3
        add    ax, word ptr [esi + 0ah]
        mov    word ptr [esp + 8], ax
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [esp + 0ah], eax
        lea    eax, [esp + 4]
        mov    byte ptr [esp + 12h], 2
        push   eax
        db  0FFh, 015h, 0A8h, 008h, 000h, 000h
        mov    eax, dword ptr [esp + 0eh]
        add    eax, dword ptr [esp + 12h]
        add    esp, 4
        lea    ecx, [eax + 58h]
        mov    dword ptr [eax + 48h], ecx
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0dh
        mov    word ptr [eax + 6], 0
        mov    dword ptr [eax + 8], 500h
        db  0C7h, 040h, 00Ch, 080h, 085h, 000h, 000h
        mov    dword ptr [eax + 40h], 0
        mov    dword ptr [eax + 10h], 0
        mov    dword ptr [eax + 14h], 0
        mov    dword ptr [eax + 18h], 0
        mov    byte ptr [eax + 4ch], 0
        mov    ecx, dword ptr [esp + 0ah]
        mov    dword ptr [eax + 20h], ecx
        mov    dword ptr [eax + 44h], esi
        pop    esi
        add    esp, 10h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3780:
        sub    esp, 8
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [esp], 4
        sub    eax, dword ptr [eax + 20h]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp]
        push   eax
        db  0FFh, 015h, 0A8h, 008h, 000h, 000h
        add    esp, 0ch
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_37B0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        push   edi
        call   sub_3_3640
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_37D0
        mov    eax, 1
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_37D0:
        push   edi
        call   sub_3_30C0
        add    esp, 4
        cmp    dword ptr [edi + 2ch], 0
        jne    SHORT loc_3_37F0
        mov    eax, dword ptr [edi + 14h]
        sub    esi, esi
        mov    word ptr [eax + 3], 100h
        jmp    SHORT loc_3_37FE
        db  08Dh, 064h, 024h, 000h
loc_3_37F0:
        push   edi
        call   sub_3_3820
        add    esp, 4
        mov    esi, 1
loc_3_37FE:
        push   edi
        call   sub_3_3160
        add    esp, 4
        push   edi
        call   sub_3_36C0
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3820:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx + 14h]
        mov    word ptr [eax + 3], 8100h
        mov    eax, dword ptr [edx + 2ch]
        sub    eax, 13h
        cmp    eax, 0fh
        ja     SHORT loc_3_3840
        db  0FFh, 024h, 085h, 0C0h, 038h, 000h, 000h
        db  090h
loc_3_3840:
        mov    eax, 2
        jmp    SHORT loc_3_38AC
loc_3_3847:
        sub    eax, eax
        jmp    SHORT loc_3_38AC
        db  005h, 000h, 000h, 000h, 000h
loc_3_3850:
        mov    eax, 1
        jmp    SHORT loc_3_38AC
loc_3_3857:
        mov    eax, 3
        jmp    SHORT loc_3_38AC
        db  08Bh, 0FFh
loc_3_3860:
        mov    eax, 4
        jmp    SHORT loc_3_38AC
loc_3_3867:
        mov    eax, 5
        jmp    SHORT loc_3_38AC
        db  08Bh, 0FFh
loc_3_3870:
        mov    eax, 6
        jmp    SHORT loc_3_38AC
loc_3_3877:
        mov    eax, 7
        jmp    SHORT loc_3_38AC
        db  08Bh, 0FFh
loc_3_3880:
        mov    eax, 8
        jmp    SHORT loc_3_38AC
loc_3_3887:
        mov    eax, 9
        jmp    SHORT loc_3_38AC
        db  08Bh, 0FFh
loc_3_3890:
        mov    eax, 0ah
        jmp    SHORT loc_3_38AC
loc_3_3897:
        mov    eax, 0bh
        jmp    SHORT loc_3_38AC
        db  08Bh, 0FFh
loc_3_38A0:
        mov    eax, 0ch
        jmp    SHORT loc_3_38AC
loc_3_38A7:
        mov    eax, 0fh
loc_3_38AC:
        mov    ecx, dword ptr [edx + 14h]
        mov    cx, word ptr [ecx + 3]
        or     cx, ax
        mov    eax, dword ptr [edx + 14h]
        mov    word ptr [eax + 3], cx
        ret
        db  08Bh, 0FFh, 047h, 038h, 000h, 000h, 050h, 038h, 000h, 000h, 040h, 038h, 000h, 000h, 057h, 038h
        db  000h, 000h, 060h, 038h, 000h, 000h, 067h, 038h, 000h, 000h, 070h, 038h, 000h, 000h, 077h, 038h
        db  000h, 000h, 080h, 038h, 000h, 000h, 087h, 038h, 000h, 000h, 090h, 038h, 000h, 000h, 097h, 038h
        db  000h, 000h, 0A0h, 038h, 000h, 000h, 040h, 038h, 000h, 000h, 040h, 038h, 000h, 000h, 0A7h, 038h
        db  000h, 000h
sub_3_3900:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 14h]
        add    eax, 3
        cmp    byte ptr [eax], 0ch
        jne    SHORT loc_3_3912
        mov    byte ptr [eax], 2
loc_3_3912:
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3920:
        push   ebx
        db  08Bh, 00Dh, 090h, 008h, 000h, 000h
        add    ecx, 14h
        sub    eax, eax
        inc    ecx
        mov    dl, byte ptr [esp + 8]
loc_3_3931:
        mov    bl, byte ptr [ecx]
        and    bl, 18h
        cmp    bl, 18h
        jne    SHORT loc_3_3940
        cmp    byte ptr [ecx - 1], dl
        je     SHORT loc_3_394E
loc_3_3940:
        add    ecx, 6
        inc    eax
        cmp    eax, 1ah
        jb     SHORT loc_3_3931
        mov    eax, 0ffffffffh
loc_3_394E:
        pop    ebx
        ret
loc_3_3950:
        push   esi
        db  08Bh, 00Dh, 090h, 008h, 000h, 000h
        add    ecx, 14h
        sub    dx, dx
        sub    si, si
        sub    eax, eax
        inc    ecx
loc_3_3963:
        test   byte ptr [ecx], 8
        je     SHORT loc_3_3972
        or     dx, dx
        jne    SHORT loc_3_3970
        mov    si, ax
loc_3_3970:
        inc    dx
loc_3_3972:
        add    ecx, 6
        inc    eax
        cmp    eax, 1ah
        jb     SHORT loc_3_3963
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax + 1ch], 0ffh
        and    dword ptr [eax + 2ch], 0fffffffeh
        mov    word ptr [eax + 10h], dx
        mov    word ptr [eax + 18h], si
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_39A0:
        push   esi
        db  08Bh, 035h, 090h, 008h, 000h, 000h
        add    esi, 14h
        push   3810h
        call   sub_3_57FF
        add    esp, 4
        inc    esi
        mov    ecx, 1ah
        lea    ecx, [ecx]
loc_3_39C0:
        test   byte ptr [esi], 8
        je     SHORT loc_3_39D3
        mov    dl, byte ptr [esi - 1]
        mov    byte ptr [eax], dl
        inc    eax
        mov    edx, dword ptr [esi + 1]
        mov    dword ptr [eax], edx
        add    eax, 4
loc_3_39D3:
        add    esi, 6
        dec    ecx
        jne    SHORT loc_3_39C0
        mov    eax, dword ptr [esp + 8]
        pop    esi
        and    dword ptr [eax + 2ch], 0fffffffeh
        sub    eax, eax
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_39F0:
        mov    edx, dword ptr [esp + 4]
        xor    eax, eax
        mov    al, byte ptr [edx + 18h]
        cmp    eax, 1ah
        jae    SHORT loc_3_3A13
        lea    ecx, [eax + eax*2]
        db  0A1h, 090h, 008h, 000h, 000h
        test   byte ptr [eax + ecx*2 + 15h], 8
        mov    word ptr [edx + 1ch], 1
        jne    SHORT loc_3_3A19
loc_3_3A13:
        mov    word ptr [edx + 1ch], 0
loc_3_3A19:
        mov    word ptr [edx + 10h], 0adadh
        and    dword ptr [edx + 2ch], 0fffffffeh
        sub    eax, eax
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3A30:
        db  0A1h, 090h, 008h, 000h, 000h
        mov    cx, word ptr [eax + 0ah]
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 10h], cx
        and    dword ptr [eax + 2ch], 0fffffffeh
        sub    eax, eax
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_3A50:
        push   ebx
        push   esi
        db  08Bh, 035h, 090h, 008h, 000h, 000h
        add    esi, 14h
        push   3810h
        call   sub_3_57FF
        add    esp, 4
        sub    dl, dl
        mov    ebx, 8
        nop
loc_3_3A70:
        xor    ecx, ecx
        mov    cl, dl
        lea    ecx, [ecx + ecx*2]
        test   byte ptr [esi + ecx*2 + 1], bl
        je     SHORT loc_3_3A80
        mov    byte ptr [eax], dl
        inc    eax
loc_3_3A80:
        inc    dl
        cmp    dl, 1ah
        jb     SHORT loc_3_3A70
        mov    eax, dword ptr [esp + 0ch]
        pop    esi
        and    dword ptr [eax + 2ch], 0fffffffeh
        sub    eax, eax
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3AA0:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 4]
        cmp    ecx, 2
        jl     SHORT loc_3_3AB3
        cmp    ecx, 4
        jle    SHORT loc_3_3AC0
loc_3_3AB3:
        mov    word ptr [eax + 1ah], 5
        mov    eax, 5
        ret
        db  090h
loc_3_3AC0:
        push   29h
        push   2
        push   eax
        call   sub_3_55E0
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_3_3AD0:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax + 4], 0
        je     SHORT loc_3_3AE6
        mov    word ptr [eax + 1ah], 5
        mov    eax, 5
        ret
loc_3_3AE6:
        push   2bh
        push   2
        push   eax
        call   sub_3_55E0
        add    esp, 0ch
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3B00:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [eax + 4]
        and    cl, 7
        cmp    cl, 1
        je     SHORT loc_3_3B38
        mov    cx, word ptr [eax + 18h]
        and    cx, 0ffh
        cmp    cx, 10h
        je     SHORT loc_3_3B38
        cmp    cx, 12h
        je     SHORT loc_3_3B38
        cmp    cx, 2
        je     SHORT loc_3_3B38
        push   2dh
        push   2
        push   eax
        call   sub_3_55E0
        add    esp, 0ch
        ret
loc_3_3B38:
        mov    word ptr [eax + 1ah], 5
        mov    eax, 5
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3B50:
        sub    esp, 90h
        mov    eax, dword ptr [esp + 94h]
        lea    ecx, [esp]
        mov    dword ptr [eax + 3ch], ecx
        push   31h
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 9ch
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3B80:
        push   33h
        mov    eax, dword ptr [esp + 8]
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 0ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3BA0:
        mov    eax, dword ptr [esp + 4]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 4]
        cmp    ecx, 7
        ja     SHORT loc_3_3BB5
        db  0FFh, 024h, 08Dh, 0ECh, 03Bh, 000h, 000h
loc_3_3BB5:
        mov    word ptr [eax + 1ah], 1
        mov    eax, 1
        ret
loc_3_3BC1:
        push   35h
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 0ch
        ret
        db  090h
loc_3_3BD0:
        mov    word ptr [eax + 1ah], 0
        sub    eax, eax
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3BE0:
        mov    word ptr [eax + 1ah], 5
        mov    eax, 5
        ret
loc_3_3BEC:
        sar    dword ptr [ebx], 0
        db  000h, 0D0h
        cmp    eax, dword ptr [eax]
        db  000h, 0B5h, 03Bh, 000h, 000h, 0B5h
        cmp    eax, dword ptr [eax]
        db  000h, 0C1h
        cmp    eax, dword ptr [eax]
        db  000h, 0E0h
        cmp    eax, dword ptr [eax]
        db  000h, 0C1h
        cmp    eax, dword ptr [eax]
        db  000h, 0E0h
        cmp    eax, dword ptr [eax]
        add    byte ptr [ebp - 74ffdb9ch], cl
        inc    esp
        and    al, 4
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 4]
        or     ecx, ecx
        je     SHORT loc_3_3C29
        mov    word ptr [eax + 1ah], 1
        mov    eax, 1
        ret
loc_3_3C29:
        push   4dh
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 0ch
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_3C40:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 1ah], 5
        mov    eax, 5
        ret
loc_3_3C50:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        test   byte ptr [eax + 3], 24h
        je     SHORT loc_3_3C64
        or     dword ptr [esi + 20h], 8000h
loc_3_3C64:
        push   esi
        call   sub_3_54A0
        add    esp, 4
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_3_3C82
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [ecx + 1ah], ax
        or     dword ptr [esi + 20h], 10h
        pop    esi
        ret
loc_3_3C82:
        mov    byte ptr [esi + 6dh], 1
        mov    eax, dword ptr [esi]
        mov    word ptr [esi + 70h], 16h
        cmp    byte ptr [eax + 4], 2
        jne    SHORT loc_3_3CAC
        mov    eax, dword ptr [esi + 10h]
        push   22h
        mov    cx, word ptr [eax + 48h]
        mov    word ptr [eax + 4ah], cx
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret
loc_3_3CAC:
        push   25h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3CC0:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 10h]
        mov    esi, dword ptr [edx + 10h]
        cmp    dword ptr [edx + 24h], 0
        jne    NEAR PTR loc_3_3D70
        mov    eax, dword ptr [edx]
        cmp    byte ptr [eax + 4], 2
        jne    SHORT loc_3_3CF2
        xor    eax, eax
        mov    ax, word ptr [esi + 48h]
        sub    eax, dword ptr [esi]
        add    eax, esi
        cmp    eax, -4ch
        je     SHORT loc_3_3CF2
        mov    dword ptr [edx + 24h], 3
loc_3_3CF2:
        cmp    dword ptr [edx + 24h], 0
        jne    SHORT loc_3_3D70
        mov    eax, dword ptr [edx]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        cmp    eax, 3
        je     SHORT loc_3_3D10
        cmp    eax, 4
        je     SHORT loc_3_3D25
        jmp    SHORT loc_3_3D70
        db  08Bh, 0FFh
loc_3_3D10:
        mov    eax, dword ptr [esi + 8]
        mov    cx, word ptr [esi + 2eh]
        mov    bx, word ptr [eax]
        add    bx, cx
        add    esi, 4dch
        jmp    SHORT loc_3_3D38
loc_3_3D25:
        mov    eax, dword ptr [esi + 4]
        mov    cx, word ptr [esi + 2ah]
        mov    bx, word ptr [eax]
        add    bx, cx
        add    esi, 254h
loc_3_3D38:
        mov    eax, dword ptr [edx]
        mov    edi, dword ptr [eax + 10h]
        add    cx, 4
        mov    word ptr [edi + 2], cx
        lea    eax, [ebx + 4]
        mov    word ptr [edi], ax
        and    ebx, 0ffffh
        mov    eax, ebx
        and    eax, 0fffffffeh
        mov    word ptr [esi + eax], 0
        add    ebx, 2
        add    edi, 4
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_3_3D70:
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [edx + 24h]
        mov    word ptr [eax + 1ah], cx
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_3_3D80:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    word ptr [esi + 70h], 16h
        mov    eax, dword ptr [esi]
        test   byte ptr [eax + 3], 24h
        je     SHORT loc_3_3D9A
        or     dword ptr [esi + 20h], 8000h
loc_3_3D9A:
        push   esi
        call   sub_3_54A0
        add    esp, 4
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_3_3DB8
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [ecx + 1ah], ax
        or     dword ptr [esi + 20h], 10h
        pop    esi
        ret
loc_3_3DB8:
        mov    byte ptr [esi + 6dh], 0
        push   25h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3DD0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi]
        cmp    dword ptr [esi + 24h], 0
        jne    SHORT loc_3_3DEE
        push   esi
        call   sub_3_94D0
        add    esp, 4
        and    eax, 0ffh
        mov    dword ptr [edi], eax
loc_3_3DEE:
        test   byte ptr [esi + 21h], 8
        je     SHORT loc_3_3DFB
        mov    dword ptr [esi + 24h], 5
loc_3_3DFB:
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [edi + 1ah], ax
        pop    edi
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3E10:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0A1h, 080h, 09Ch, 000h, 000h
        cmp    dword ptr [esi + 14h], eax
        jne    SHORT loc_3_3E29
        db  0C7h, 005h, 080h, 09Ch, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_3E29:
        mov    eax, dword ptr [esi]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffffh
        cmp    eax, 1
        jl     SHORT loc_3_3E4D
        cmp    eax, 2
        jg     SHORT loc_3_3E4D
        mov    eax, dword ptr [esi + 44h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        call   sub_3_7730
        add    esp, 4
loc_3_3E4D:
        mov    eax, dword ptr [esi]
        pop    esi
        mov    word ptr [eax + 1ah], 0
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3E60:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    word ptr [esi + 70h], 6
        mov    eax, dword ptr [esi]
        test   byte ptr [eax + 3], 24h
        je     SHORT loc_3_3E7A
        or     dword ptr [esi + 20h], 8000h
loc_3_3E7A:
        push   esi
        call   sub_3_54A0
        add    esp, 4
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_3_3E98
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [ecx + 1ah], ax
        or     dword ptr [esi + 20h], 10h
        pop    esi
        ret
loc_3_3E98:
        mov    byte ptr [esi + 6dh], 1
        push   25h
        push   esi
        call   sub_3_8380
        add    esp, 8
        pop    esi
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_3EB0:
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 50h]
        mov    edx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [ebx + 1ch]
        mov    ax, word ptr [eax + 8]
        inc    ax
        mov    word ptr [ecx + 8], ax
        mov    esi, dword ptr [edx + 24h]
        mov    dword ptr [ebx + 98h], esi
        mov    eax, dword ptr [edx + 14h]
        mov    dword ptr [ebx + 9ch], eax
        mov    eax, dword ptr [edx]
        or     eax, eax
        mov    dword ptr [ebx + 0a0h], eax
        mov    dword ptr [ebx + 0a4h], 0
        mov    dword ptr [ebx + 24h], 0
        mov    byte ptr [ebx + 63h], 5
        mov    ecx, dword ptr [edx + 3ch]
        mov    dword ptr [ebx + 0b4h], ecx
        je     NEAR PTR loc_3_4177
        mov    eax, dword ptr [ebx + 18h]
        test   word ptr [eax + 12h], 4050h
        je     NEAR PTR loc_3_416C
        cmp    esi, 2ch
        jae    NEAR PTR loc_3_416C
        mov    eax, dword ptr [ebx + 18h]
        test   byte ptr [eax + 13h], 40h
        je     SHORT loc_3_3F37
        db  0C7h, 044h, 024h, 010h, 0A8h, 09Fh, 000h, 000h
        jmp    SHORT loc_3_3F50
loc_3_3F37:
        mov    eax, dword ptr [ebx + 18h]
        test   byte ptr [eax + 12h], 40h
        db  0C7h, 044h, 024h, 010h, 07Ch, 09Fh, 000h, 000h
        jne    SHORT loc_3_3F50
        db  0C7h, 044h, 024h, 010h, 0D4h, 09Fh, 000h, 000h
loc_3_3F50:
        lea    edi, [esp + 20h]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, 0bh
        rep movsd
        mov    eax, dword ptr [ebx + 18h]
        test   byte ptr [eax + 13h], 40h
        mov    eax, dword ptr [ebx + 18h]
        jne    NEAR PTR loc_3_401A
        mov    eax, dword ptr [eax + 34h]
        sub    eax, 2ch
        add    dword ptr [esp + 24h], eax
        add    dword ptr [esp + 48h], eax
        mov    eax, dword ptr [ebx + 18h]
        test   byte ptr [eax + 12h], 40h
        mov    eax, dword ptr [ebx + 18h]
        je     SHORT loc_3_3FE5
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        db  08Ah, 08Ch, 080h, 054h, 0A0h, 000h, 000h
        mov    al, cl
        and    eax, 1
        mov    dl, cl
        and    dl, 2
        shr    dl, 1
        and    edx, 0ffh
        and    cl, 0ch
        shr    cl, 2
        and    ecx, 0ffh
        mov    dword ptr [esp + 14h], ecx
        mov    cl, al
        shr    word ptr [esp + 36h], cl
        mov    cl, dl
        shr    word ptr [esp + 42h], cl
        lea    ecx, [edx + eax]
        shr    word ptr [esp + 40h], cl
        mov    cl, byte ptr [esp + 14h]
        shr    dword ptr [esp + 38h], cl
        add    cl, al
        add    cl, dl
        shr    dword ptr [esp + 3ch], cl
        jmp    NEAR PTR loc_3_4135
loc_3_3FE5:
        test   byte ptr [eax + 12h], 10h
        je     NEAR PTR loc_3_4135
        mov    eax, dword ptr [ebx + 18h]
        add    eax, 3ch
        mov    dword ptr [esp + 10h], eax
        mov    ecx, eax
        lea    edx, [esp + 34h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 0ch], eax
        jmp    NEAR PTR loc_3_4135
loc_3_401A:
        xor    ecx, ecx
        mov    cx, word ptr [eax + 1ah]
        lea    eax, [ecx - 1]
        sub    ecx, 2
        mov    word ptr [esp + 36h], ax
        mov    eax, dword ptr [ebx + 14h]
        mov    eax, dword ptr [eax + 3ch]
        mov    dword ptr [esp + 38h], eax
        mov    esi, 4bh
        mov    eax, dword ptr [ebx + 14h]
        sub    edx, edx
        mov    eax, dword ptr [eax + 134h]
        mov    eax, dword ptr [eax + ecx*8]
        mov    dword ptr [esp + 1ch], eax
        mov    dword ptr [esp + 3ch], eax
        add    dword ptr [esp + 1ch], 96h
        mov    eax, dword ptr [esp + 1ch]
        div    esi
        mov    byte ptr [esp + 0ch], dl
        mov    eax, dword ptr [esp + 1ch]
        sub    edx, edx
        div    esi
        mov    dword ptr [esp + 1ch], eax
        mov    esi, 3ch
        sub    edx, edx
        div    esi
        mov    byte ptr [esp + 0dh], dl
        mov    eax, dword ptr [esp + 1ch]
        sub    edx, edx
        div    esi
        mov    dword ptr [esp + 1ch], eax
        mov    byte ptr [esp + 0eh], al
        mov    byte ptr [esp + 0fh], 0
        mov    byte ptr [esp + 1fh], 0
        mov    byte ptr [esp + 1eh], al
        mov    al, byte ptr [esp + 0dh]
        mov    byte ptr [esp + 1dh], al
        mov    al, byte ptr [esp + 0ch]
        mov    byte ptr [esp + 1ch], al
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esp + 44h], eax
        mov    eax, dword ptr [ebx + 14h]
        mov    eax, dword ptr [eax + 134h]
        mov    eax, dword ptr [eax + ecx*8 + 4]
        mov    ecx, 4bh
        mov    dword ptr [esp + 1ch], eax
        mov    dword ptr [esp + 40h], eax
        sub    dword ptr [esp + 1ch], 96h
        add    dword ptr [esp + 1ch], 96h
        sub    edx, edx
        mov    eax, dword ptr [esp + 1ch]
        div    ecx
        mov    byte ptr [esp + 0ch], dl
        mov    eax, dword ptr [esp + 1ch]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 1ch], eax
        mov    ecx, esi
        sub    edx, edx
        div    ecx
        mov    byte ptr [esp + 0dh], dl
        mov    ecx, esi
        mov    eax, dword ptr [esp + 1ch]
        sub    edx, edx
        div    ecx
        mov    dword ptr [esp + 1ch], eax
        mov    byte ptr [esp + 0eh], al
        mov    byte ptr [esp + 0fh], 0
        mov    byte ptr [esp + 1fh], 0
        mov    byte ptr [esp + 1eh], al
        mov    al, byte ptr [esp + 0dh]
        mov    byte ptr [esp + 1dh], al
        mov    al, byte ptr [esp + 0ch]
        mov    byte ptr [esp + 1ch], al
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esp + 48h], eax
loc_3_4135:
        mov    edx, dword ptr [ebx + 0a0h]
        mov    ecx, dword ptr [ebx + 98h]
        mov    eax, 2ch
        sub    eax, ecx
        cmp    edx, eax
        jbe    SHORT loc_3_414E
        mov    edx, eax
loc_3_414E:
        lea    esi, [esp + ecx + 20h]
        mov    edi, dword ptr [ebx + 9ch]
        mov    ecx, edx
        shr    ecx, 2
        add    dword ptr [ebx + 0a4h], edx
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_3_416C:
        push   38h
        push   ebx
        call   sub_3_8380
        add    esp, 8
loc_3_4177:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 40h
        ret
        db  08Bh, 0FFh
loc_3_4180:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi]
        cmp    dword ptr [esi + 0a4h], 0
        jne    SHORT loc_3_419A
        lea    ebx, [esi + 24h]
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_3_41C5
loc_3_419A:
        mov    eax, dword ptr [esi + 0a4h]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0a4h]
        add    dword ptr [edi + 24h], eax
        lea    ebx, [esi + 24h]
        db  0A1h, 090h, 008h, 000h, 000h
        mov    ecx, dword ptr [esi + 0a4h]
        add    dword ptr [eax + 0b4h], ecx
        mov    dword ptr [ebx], 0
loc_3_41C5:
        push   esi
        call   sub_3_47C0
        add    esp, 4
        mov    eax, dword ptr [ebx]
        mov    word ptr [edi + 1ah], ax
        or     dword ptr [esi + 20h], 10h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_41E0:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 1ch]
        mov    ecx, dword ptr [edx + 1ch]
        mov    ax, word ptr [eax + 8]
        inc    ax
        mov    word ptr [ecx + 8], ax
        xor    eax, eax
        mov    al, byte ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_3_422C
        cmp    eax, 2
        je     SHORT loc_3_4216
        mov    word ptr [esi + 1ah], 1
        push   edx
        call   sub_3_47C0
        add    esp, 4
        pop    esi
        ret
loc_3_4216:
        mov    eax, dword ptr [edx + 18h]
        add    eax, 28h
        je     SHORT loc_3_422C
        mov    edi, edi
loc_3_4220:
        mov    ecx, dword ptr [eax + 0ch]
        add    dword ptr [esi + 24h], ecx
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_3_4220
loc_3_422C:
        mov    word ptr [esi + 1ah], 0
        push   edx
        call   sub_3_47C0
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_3_4240:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        mov    eax, dword ptr [esi + 1ch]
        mov    cl, byte ptr [eax + 0bh]
        mov    eax, dword ptr [esi + 1ch]
        or     cl, 1
        mov    byte ptr [eax + 0bh], cl
        call   sub_3_47C0
        add    esp, 4
        mov    eax, dword ptr [esi]
        pop    esi
        mov    word ptr [eax + 1ah], 0
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4270:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 1ch]
        mov    ax, word ptr [eax + 8]
        inc    ax
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax + 4], 4
        je     SHORT loc_3_42D0
        mov    eax, dword ptr [esi + 18h]
        add    eax, 1ch
        push   eax
        call   sub_3_4850
        add    esp, 4
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax + 4], 6
        jne    SHORT loc_3_42D9
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, 989680h
        mov    eax, dword ptr [eax + 1ch]
        sub    edx, edx
        div    ecx
        mov    ecx, 0c8h
        sub    edx, edx
        div    ecx
        mov    eax, dword ptr [esi]
        mov    word ptr [eax + 18h], dx
        jmp    SHORT loc_3_42D9
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_42D0:
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 28h], 0
loc_3_42D9:
        mov    eax, dword ptr [esi]
        push   esi
        mov    word ptr [eax + 1ah], 0
        call   sub_3_47C0
        add    esp, 4
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h
loc_3_42F0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 1ch]
        mov    ax, word ptr [eax + 8]
        inc    ax
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [eax + 14h]
        mov    ebx, dword ptr [esi + 18h]
        lea    ebp, [edi + 4]
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [ebp], eax
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [edi + 8], eax
        push   ebp
        call   sub_3_486B
        add    esp, 4
        mov    eax, dword ptr [ebp]
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [edi + 18h], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 18h]
        mov    dword ptr [edi + 2ch], eax
        xor    eax, eax
        mov    ax, word ptr [ebx + 1ah]
        mov    dword ptr [edi + 30h], eax
        mov    eax, dword ptr [esi + 14h]
        mov    eax, dword ptr [eax + 3ch]
        mov    dword ptr [edi + 1ch], eax
        mov    dword ptr [edi + 0ch], 0
        mov    dword ptr [edi + 10h], 0
        mov    dword ptr [edi + 20h], 0
        mov    dword ptr [edi + 24h], 0
        mov    dword ptr [edi + 28h], 1
        mov    al, byte ptr [ebx + 27h]
        push   eax
        call   sub_3_9500
        add    esp, 4
        and    eax, 0ffh
        mov    dword ptr [edi], eax
        add    ebx, 28h
        je     SHORT loc_3_4396
loc_3_438A:
        mov    eax, dword ptr [ebx + 0ch]
        add    dword ptr [edi + 24h], eax
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_3_438A
loc_3_4396:
        push   esi
        call   sub_3_47C0
        add    esp, 4
        mov    eax, dword ptr [esi]
        pop    ebp
        mov    word ptr [eax + 1ah], 0
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h
sub_3_43B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
        db  090h
sub_3_43D0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, 3ch
        mov    edi, dword ptr [esp + 14h]
        push   edi
        call   sub_3_9620
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_3_43F1
        mov    esi, 4ch
loc_3_43F1:
        push   1
        push   esi
        call   sub_3_43B0
        add    esp, 8
        mov    esi, eax
        mov    dword ptr [edi + 18h], esi
        or     esi, esi
        jne    SHORT loc_3_4411
        mov    dword ptr [edi + 24h], 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_4411:
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [esi + 4], eax
        mov    ax, word ptr [edi + 66h]
        mov    word ptr [esi + 18h], ax
        mov    eax, dword ptr [edi + 84h]
        mov    word ptr [esi + 1ah], ax
        lea    eax, [esi + 1ch]
        push   eax
        mov    eax, dword ptr [edi + 90h]
        add    eax, 12h
        push   eax
        call   sub_3_5300
        add    esp, 8
        mov    ecx, dword ptr [edi + 14h]
        lea    ebp, [esi + 27h]
        cmp    byte ptr [ecx + 1eh], 0
        mov    eax, dword ptr [edi + 90h]
        je     SHORT loc_3_4456
        mov    al, byte ptr [eax + 19h]
        jmp    SHORT loc_3_4459
loc_3_4456:
        mov    al, byte ptr [eax + 18h]
loc_3_4459:
        mov    byte ptr [ebp], al
        or     ebx, ebx
        je     SHORT loc_3_44B1
        mov    edx, dword ptr [ebx]
        mov    dword ptr [esi + 3ch], edx
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 40h], edx
        lea    eax, [esi + 3ch]
        mov    edx, dword ptr [ebx + 8]
        mov    dword ptr [eax + 8], edx
        mov    dx, word ptr [ebx + 0ch]
        mov    word ptr [eax + 0ch], dx
        mov    al, byte ptr [ecx + 1fh]
        and    al, 2
        cmp    al, 1
        cmc
        sbb    eax, eax
        and    eax, 4000h
        mov    cx, word ptr [ebx + 4]
        and    cx, 0f8h
        or     ax, cx
        or     ax, word ptr [esi + 12h]
        or     ax, 2200h
        mov    word ptr [esi + 12h], ax
        test   al, 40h
        je     SHORT loc_3_44B1
        cmp    byte ptr [esi + 44h], 10h
        jb     SHORT loc_3_44B1
        mov    byte ptr [esi + 44h], 0
loc_3_44B1:
        mov    ecx, dword ptr [edi + 90h]
        push   1
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [esi + 26h], al
        and    eax, 0ffh
        mov    ecx, dword ptr [ecx + 2]
        add    ecx, eax
        mov    dword ptr [esi + 30h], ecx
        push   ebx
        push   edi
        call   sub_3_9650
        add    esp, 0ch
        mov    dword ptr [esi + 34h], eax
        mov    ecx, dword ptr [edi + 90h]
        mov    al, byte ptr [ecx + 1ah]
        mov    byte ptr [esi + 3ah], al
        mov    al, byte ptr [ecx + 1bh]
        mov    byte ptr [esi + 3bh], al
        mov    ax, word ptr [ecx + 1ch]
        mov    word ptr [esi + 38h], ax
        lea    eax, [esi + 28h]
        push   eax
        push   edi
        call   sub_3_96B0
        add    esp, 8
        test   byte ptr [ebp], 80h
        je     SHORT loc_3_450C
        or     word ptr [esi + 12h], 400h
loc_3_450C:
        mov    dword ptr [edi + 24h], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_4520:
        push   esi
        db  0A0h, 050h, 045h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 14h]
        mov    ecx, dword ptr [esi + 10h]
        mov    edx, dword ptr [eax + 18h]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [esi + 10h], eax
        inc    word ptr [esi + 1ch]
        pop    esi
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4550:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4560:
        push   esi
        db  0A0h, 0B0h, 045h, 000h, 000h
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 14h]
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, ecx
        je     SHORT loc_3_45AB
        sub    edx, edx
loc_3_4576:
        cmp    word ptr [ecx + 10h], dx
        je     SHORT loc_3_4594
        mov    ax, word ptr [esi + 66h]
        cmp    word ptr [ecx + 18h], ax
        jne    SHORT loc_3_4594
        xor    eax, eax
        mov    ax, word ptr [ecx + 1ah]
        cmp    eax, dword ptr [esi + 84h]
        je     SHORT loc_3_45A0
loc_3_4594:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, edx
        jne    SHORT loc_3_4576
        mov    dword ptr [esi + 18h], ecx
        pop    esi
        ret
        db  090h
loc_3_45A0:
        inc    word ptr [ecx + 10h]
        or     dword ptr [esi + 20h], 10000000h
loc_3_45AB:
        mov    dword ptr [esi + 18h], ecx
        pop    esi
        ret
loc_3_45B0:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_45C0:
        push   1
        push   10h
        call   sub_3_43B0
        mov    edx, dword ptr [esp + 0ch]
        add    esp, 8
        mov    dword ptr [edx + 1ch], eax
        or     eax, eax
        jne    SHORT loc_3_45E0
        mov    dword ptr [edx + 24h], 8
        ret
        db  090h
loc_3_45E0:
        mov    ecx, dword ptr [edx + 18h]
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax + 8], 1
        mov    ecx, dword ptr [edx]
        mov    cl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 0ah], cl
        mov    ecx, dword ptr [edx]
        mov    ecx, dword ptr [ecx + 8]
        mov    word ptr [eax + 0ch], cx
        mov    ecx, dword ptr [edx]
        mov    cl, byte ptr [ecx + 5]
        and    ecx, 0ffff00ffh
        mov    word ptr [eax + 0eh], cx
        mov    dword ptr [edx + 24h], 0
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4620:
        db  0A0h, 050h, 046h, 000h, 000h
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx + 18h]
        mov    eax, dword ptr [eax + 14h]
        mov    ecx, dword ptr [edx + 1ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 1ch]
        mov    ecx, dword ptr [edx + 18h]
        mov    dword ptr [ecx + 14h], eax
        mov    eax, dword ptr [edx + 18h]
        mov    cx, word ptr [eax + 10h]
        inc    cx
        mov    eax, dword ptr [edx + 18h]
        mov    word ptr [eax + 10h], cx
        ret
        db  08Bh, 0FFh
loc_3_4650:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4660:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A0h, 020h, 047h, 000h, 000h
        mov    dword ptr [esp + 10h], 0
        mov    edi, dword ptr [esp + 18h]
        mov    esi, dword ptr [edi + 14h]
        dec    word ptr [esi + 1ch]
        jne    NEAR PTR loc_3_4712
        db  0B9h, 0D0h, 09Bh, 000h, 000h
        db  083h, 03Dh, 0D0h, 09Bh, 000h, 000h, 000h
        je     SHORT loc_3_46A0
loc_3_4693:
        mov    eax, dword ptr [ecx]
        cmp    esi, eax
        je     SHORT loc_3_46A0
        mov    ecx, eax
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_3_4693
loc_3_46A0:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_3_46B1
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [esp + 10h], 1
loc_3_46B1:
        test   byte ptr [esi + 1fh], 2
        jne    SHORT loc_3_46EC
        sub    ebx, ebx
        cmp    dword ptr [esi + 104h], ebx
        jbe    SHORT loc_3_46EC
        sub    ebp, ebp
loc_3_46C3:
        mov    eax, dword ptr [esi + 108h]
        mov    eax, dword ptr [ebp + eax + 8]
        or     eax, eax
        je     SHORT loc_3_46E0
        mov    ecx, dword ptr [edi + 20h]
        push   ecx
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0ch
loc_3_46E0:
        add    ebp, 0ch
        inc    ebx
        cmp    dword ptr [esi + 104h], ebx
        ja     SHORT loc_3_46C3
loc_3_46EC:
        mov    eax, dword ptr [edi + 20h]
        push   eax
        push   0
        push   esi
        call   sub_3_8750
        add    esp, 0ch
        mov    dword ptr [edi + 14h], 0
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_3_4712
        push   edi
        call   sub_3_1DA0
        add    esp, 4
loc_3_4712:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_4720:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4730:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A0h, 0B0h, 047h, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi + 18h]
        dec    word ptr [edi + 10h]
        jne    SHORT loc_3_47A8
        mov    ecx, dword ptr [esi + 14h]
        add    ecx, 10h
        cmp    dword ptr [ecx], 0
        je     SHORT loc_3_475E
loc_3_4751:
        mov    eax, dword ptr [ecx]
        cmp    edi, eax
        je     SHORT loc_3_475E
        mov    ecx, eax
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_3_4751
loc_3_475E:
        sub    ebx, ebx
        cmp    dword ptr [ecx], ebx
        je     SHORT loc_3_476D
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    ebx, 1
loc_3_476D:
        mov    ebp, dword ptr [edi + 28h]
        or     ebp, ebp
        je     SHORT loc_3_478C
loc_3_4774:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp]
        mov    ecx, dword ptr [esi + 20h]
        push   ecx
        push   0
        push   eax
        call   sub_3_8750
        add    esp, 0ch
        or     ebp, ebp
        jne    SHORT loc_3_4774
loc_3_478C:
        mov    eax, dword ptr [esi + 20h]
        push   eax
        push   0
        push   edi
        call   sub_3_8750
        add    esp, 0ch
        or     ebx, ebx
        je     SHORT loc_3_47A8
        push   esi
        call   sub_3_4660
        add    esp, 4
loc_3_47A8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_3_47B0:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_47C0:
        push   ebx
        push   esi
        push   edi
        db  0A0h, 040h, 048h, 000h, 000h
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esi + 1ch]
        dec    word ptr [ecx + 8]
        jne    SHORT loc_3_4834
        mov    eax, dword ptr [esi + 1ch]
        sub    edi, edi
        mov    bl, byte ptr [eax + 0bh]
        and    ebx, 2
        jne    SHORT loc_3_4807
        mov    edx, dword ptr [esi + 18h]
        add    edx, 14h
        cmp    dword ptr [edx], edi
        je     SHORT loc_3_47F9
loc_3_47EC:
        mov    eax, dword ptr [edx]
        cmp    eax, ecx
        je     SHORT loc_3_47F9
        mov    edx, eax
        cmp    dword ptr [edx], 0
        jne    SHORT loc_3_47EC
loc_3_47F9:
        cmp    dword ptr [edx], 0
        je     SHORT loc_3_4807
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    edi, 1
loc_3_4807:
        mov    eax, dword ptr [esi + 20h]
        push   eax
        push   0
        push   ecx
        call   sub_3_8750
        add    esp, 0ch
        or     edi, edi
        je     SHORT loc_3_4827
        push   esi
        call   sub_3_4730
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_4827:
        or     ebx, ebx
        je     SHORT loc_3_4834
        push   esi
        call   sub_3_4660
        add    esp, 4
loc_3_4834:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_4840:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4850:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        push   esp
        call   sub_3_486B
        pop    eax
        db  0CDh, 020h, 03Ah, 000h, 040h, 000h
        lea    esp, [esp + 8]
        ret
sub_3_486B:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 4]
        or     ecx, dword ptr [eax]
        je     SHORT loc_3_488C
        db  0CDh, 020h, 060h, 000h, 040h, 000h
        mov    edx, 0dc3cba00h
        imul   edx
        mov    ecx, dword ptr [esp + 4]
        sub    dword ptr [ecx], eax
        sbb    dword ptr [ecx + 4], edx
loc_3_488C:
        ret
sub_3_488D:
        mov    ecx, dword ptr [esp + 4]
        push   eax
        push   eax
        push   esp
        push   ecx
        call   sub_3_5300
        lea    esp, [esp + 8]
        push   esp
        call   sub_3_4850
        lea    esp, [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        ret
sub_3_48AF:
        cmp    dword ptr [esp + 0ch], 4
        ja     SHORT loc_3_48F8
        mov    eax, dword ptr [esp + 8]
        mov    edx, 2e002eh
        mov    eax, dword ptr [eax]
        jb     SHORT loc_3_48C9
        cmp    eax, edx
        jne    SHORT loc_3_48F8
        jmp    SHORT loc_3_48DA
loc_3_48C9:
        cmp    dword ptr [esp + 0ch], 2
        jne    SHORT loc_3_48F8
        cmp    ax, dx
        jne    SHORT loc_3_48F8
        mov    edx, 20002eh
loc_3_48DA:
        mov    eax, 200020h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [ecx + 10h], eax
        mov    word ptr [ecx + 14h], ax
        xor    eax, eax
        ret
loc_3_48F8:
        db  0CDh, 020h, 045h, 080h, 040h, 000h
sub_3_48FE:
        db  0CDh, 020h, 047h, 080h, 040h, 000h
sub_3_4904:
        db  0CDh, 020h, 048h, 080h, 040h, 000h
sub_3_490A:
        db  0CDh, 020h, 03Dh, 080h, 040h, 000h
sub_3_4910:
        db  0CDh, 020h, 042h, 080h, 040h, 000h
sub_3_4916:
        db  0CDh, 020h, 040h, 080h, 040h, 000h
sub_3_491C:
        cmp    dword ptr [esp + 0ch], 4
        ja     SHORT loc_3_4968
        mov    eax, dword ptr [esp + 8]
        mov    edx, 2e002eh
        mov    eax, dword ptr [eax]
        jb     SHORT loc_3_4936
        cmp    eax, edx
        jne    SHORT loc_3_4968
        jmp    SHORT loc_3_4947
loc_3_4936:
        cmp    dword ptr [esp + 0ch], 2
        jne    SHORT loc_3_4968
        cmp    ax, dx
        jne    SHORT loc_3_4968
        mov    edx, 20002eh
loc_3_4947:
        mov    eax, 200020h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [ecx + 10h], eax
        mov    word ptr [ecx + 14h], ax
        mov    eax, 16h
        ret
loc_3_4968:
        db  0CDh, 020h, 049h, 080h, 040h, 000h
sub_3_496E:
        mov    eax, dword ptr [esp + 4]
        mov    cx, word ptr [eax + 70h]
        mov    al, byte ptr [eax + 6eh]
        not    cl
        mov    ah, al
        not    ah
        and    ax, cx
        and    ax, 3f1eh
        sete   al
        movzx  eax, al
        ret
        db  000h, 000h, 000h
loc_3_4990:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esi], 800h
        mov    word ptr [esi + 18h], 10h
        mov    eax, dword ptr [esp + 8]
        xor    edx, edx
        mov    ecx, dword ptr [eax + 44h]
        mov    eax, dword ptr [eax + 48h]
        shld   edx, eax, cl
        shl    eax, cl
        shrd   eax, edx, 0fh
        shr    edx, 0fh
        jne    SHORT loc_3_49CC
        test   eax, 0ffff0000h
        je     SHORT loc_3_49D0
loc_3_49CC:
        mov    ax, 0ffffh
loc_3_49D0:
        mov    word ptr [esp + 6], ax
        xor    eax, eax
        mov    ax, word ptr [esp + 6]
        mov    dword ptr [esi + 24h], eax
        mov    dword ptr [esi + 28h], 0
        mov    dword ptr [esi + 14h], 0
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    esi
        add    esp, 8
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_4A00:
        push   2fh
        mov    eax, dword ptr [esp + 8]
        push   2
        push   eax
        call   sub_3_55E0
        add    esp, 0ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4A20:
        push   4eh
        mov    eax, dword ptr [esp + 8]
        push   2
        push   eax
        call   sub_3_55E0
        add    esp, 0ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4A40:
        push   36h
        mov    eax, dword ptr [esp + 8]
        push   3
        push   eax
        call   sub_3_55E0
        add    esp, 0ch
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4A60:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    ecx, dword ptr [eax]
        mov    byte ptr [ecx + 4], 0
        call   sub_3_4A80
        add    esp, 4
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_4A80:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        mov    edi, dword ptr [ebx]
        mov    eax, dword ptr [ebx]
        test   byte ptr [eax + 3], 34h
        je     SHORT loc_3_4A99
        or     dword ptr [ebx + 20h], 8000h
loc_3_4A99:
        cmp    byte ptr [edi + 4], 0
        jne    NEAR PTR loc_3_4B30
        push   ebx
        call   sub_3_54A0
        add    esp, 4
        mov    ecx, dword ptr [edi]
        mov    eax, ecx
        and    ax, 0ff9eh
        lea    esi, [ebx + 24h]
        mov    word ptr [ebx + 70h], ax
        cmp    dword ptr [esi], 0
        jne    SHORT loc_3_4B26
        mov    edx, dword ptr [ebx + 10h]
        cmp    word ptr [edx + 48h], 2
        ja     SHORT loc_3_4B26
        test   al, 8
        je     SHORT loc_3_4B20
        test   byte ptr [ebx + 21h], 80h
        jne    SHORT loc_3_4B20
        and    ecx, 0ffff0000h
        mov    dword ptr [edi], ecx
        or     ecx, 80000808h
        mov    dword ptr [edi], ecx
        mov    eax, dword ptr [ebx]
        xor    edx, edx
        db  066h, 08Bh, 015h, 0A0h, 09Eh, 000h, 000h
        add    edx, 2
        db  0BEh, 0A0h, 09Eh, 000h, 000h
        mov    edi, dword ptr [eax + 0ch]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        push   ebx
        call   sub_3_5590
        add    esp, 4
        mov    ax, word ptr [ebx + 2ch]
        mov    word ptr [ebx + 2eh], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_3_4B20:
        mov    dword ptr [esi], 12h
loc_3_4B26:
        mov    byte ptr [ebx + 6dh], 0
        jmp    NEAR PTR loc_3_4BA8
        db  090h
loc_3_4B30:
        or     dword ptr [ebx + 20h], 2000000h
        push   ebx
        call   sub_3_54A0
        add    esp, 4
        lea    esi, [ebx + 24h]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_3_4BA2
        mov    eax, dword ptr [ebx]
        mov    ebp, dword ptr [ebx + 10h]
        mov    edi, dword ptr [eax + 14h]
        push   1
        lea    eax, [edi + 1]
        push   0bh
        push   eax
        lea    eax, [ebp + 30h]
        push   eax
        call   sub_3_4910
        add    esp, 10h
        or     word ptr [ebp + 0eh], 2
        mov    dl, byte ptr [edi + 0ch]
        and    edx, 0ffff00ffh
        mov    word ptr [ebx + 70h], dx
        mov    ax, word ptr [edi + 0dh]
        mov    word ptr [ebx + 66h], ax
        xor    ecx, ecx
        mov    cx, word ptr [edi + 0fh]
        inc    ecx
        mov    dword ptr [ebx + 80h], ecx
        mov    byte ptr [ebx + 6dh], 12h
        and    dx, 0ff9eh
        mov    word ptr [ebx + 70h], dx
        cmp    dword ptr [esi], 0
        jne    SHORT loc_3_4BB8
        or     ax, ax
        jne    SHORT loc_3_4BA8
loc_3_4BA2:
        mov    dword ptr [esi], 12h
loc_3_4BA8:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_3_4BB8
        push   25h
        push   ebx
        call   sub_3_8380
        add    esp, 8
loc_3_4BB8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_3_4BC0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 24h], 0
        je     SHORT loc_3_4BE7
        cmp    dword ptr [esi + 24h], 2
        jne    SHORT loc_3_4BDA
        mov    dword ptr [esi + 24h], 12h
loc_3_4BDA:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [esi + 24h]
        mov    word ptr [eax + 1ah], cx
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_4BE7:
        test   byte ptr [esi + 70h], 8
        jne    SHORT loc_3_4BFE
        push   esi
        call   sub_3_9520
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_3_4CED
loc_3_4BFE:
        mov    eax, dword ptr [esi]
        mov    ebx, dword ptr [esi + 10h]
        mov    edi, dword ptr [eax + 14h]
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax + 4], 0
        jne    SHORT loc_3_4C37
        push   1
        push   0bh
        push   16h
        lea    eax, [ebx + 30h]
        push   eax
        lea    eax, [edi + 1]
        push   eax
        call   sub_3_4916
        add    esp, 14h
        mov    ax, word ptr [esi + 70h]
        test   al, 8
        je     SHORT loc_3_4C30
        and    al, 0f7h
        jmp    SHORT loc_3_4C34
loc_3_4C30:
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax]
loc_3_4C34:
        mov    byte ptr [edi + 0ch], al
loc_3_4C37:
        test   byte ptr [esi + 70h], 8
        je     SHORT loc_3_4C66
        mov    dword ptr [esi + 84h], 0
        mov    eax, dword ptr [esi + 14h]
        add    eax, 50h
        push   eax
        call   sub_3_4850
        add    esp, 4
        mov    dword ptr [edi + 16h], eax
        mov    byte ptr [edi + 15h], 9
        mov    dword ptr [edi + 1ah], 0
        jmp    SHORT loc_3_4C91
loc_3_4C66:
        lea    eax, [edi + 16h]
        push   eax
        mov    eax, dword ptr [esi + 90h]
        add    eax, 12h
        push   eax
        call   sub_3_488D
        add    esp, 8
        push   esi
        call   sub_3_94D0
        add    esp, 4
        mov    byte ptr [edi + 15h], al
        mov    eax, dword ptr [esi + 0c0h]
        mov    dword ptr [edi + 1ah], eax
loc_3_4C91:
        mov    ax, word ptr [esi + 66h]
        mov    word ptr [edi + 0dh], ax
        mov    eax, dword ptr [esi + 84h]
        mov    word ptr [edi + 0fh], ax
        xor    ecx, ecx
        mov    eax, dword ptr [ebx + 8]
        push   1
        push   0ch
        mov    cx, word ptr [eax]
        sub    ecx, 2
        push   ecx
        add    eax, 2
        push   eax
        lea    eax, [edi + 1eh]
        push   eax
        call   sub_3_4916
        add    esp, 14h
        cmp    eax, 8
        jne    SHORT loc_3_4CE0
        test   byte ptr [esi + 70h], 8
        je     SHORT loc_3_4CE0
        mov    ecx, dword ptr [esi + 14h]
        cmp    dword ptr [ecx + 4ch], 4b0h
        je     SHORT loc_3_4CE0
        mov    byte ptr [eax + edi + 1eh], 2eh
        inc    eax
loc_3_4CE0:
        mov    byte ptr [eax + edi + 1eh], 0
        mov    eax, dword ptr [esi]
        mov    word ptr [eax + 1ah], 0
loc_3_4CED:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4D00:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        mov    ebp, dword ptr [ebx + 1ch]
        test   byte ptr [ebx + 22h], 40h
        jne    SHORT loc_3_4D50
        or     dword ptr [ebx + 20h], 400100h
        inc    word ptr [ebp + 8]
        lea    eax, [ebp + 14h]
        mov    dword ptr [ebx + 0b0h], eax
        push   ebx
        call   sub_3_5971
        add    esp, 4
        or     eax, eax
        je     SHORT loc_3_4D50
        mov    ax, word ptr [ebx + 2ch]
        mov    word ptr [ebx + 2eh], ax
        push   46h
        push   ebx
        call   sub_3_8380
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_4D50:
        or     dword ptr [ebx + 20h], 3008000h
        push   ebx
        call   sub_3_54A0
        add    esp, 4
        cmp    dword ptr [ebx + 24h], 0
        jne    NEAR PTR loc_3_4E7F
        mov    ax, word ptr [ebp + 1ch]
        mov    word ptr [ebx + 66h], ax
        or     ax, ax
        jne    SHORT loc_3_4D83
        mov    dword ptr [ebx + 24h], 12h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_4D83:
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [ebx + 8ch], eax
        mov    cx, word ptr [ebp + 1eh]
        mov    word ptr [ebx + 76h], cx
        mov    edx, dword ptr [ebp + 28h]
        mov    dword ptr [ebx + 84h], edx
        lea    eax, [edx + 1]
        mov    dword ptr [ebx + 80h], eax
        mov    eax, dword ptr [ebp + 2ch]
        mov    dword ptr [ebx + 88h], eax
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebx + 78h], eax
        mov    ax, word ptr [ebp + 36h]
        mov    word ptr [ebx + 70h], ax
        mov    eax, dword ptr [ebp + 30h]
        mov    dword ptr [ebx + 7ch], eax
        mov    eax, dword ptr [ebx]
        test   byte ptr [eax + 18h], 40h
        je     SHORT loc_3_4E02
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 24h]
        inc    eax
        mov    dword ptr [ebx + 80h], eax
        cmp    eax, edx
        jae    SHORT loc_3_4E02
        mov    dword ptr [ebx + 84h], 0
        mov    edx, ecx
        mov    eax, dword ptr [ebx + 14h]
        and    edx, 0ffffh
        mov    cl, byte ptr [eax + 44h]
        shl    edx, cl
        mov    dword ptr [ebx + 88h], edx
        mov    dword ptr [ebx + 7ch], 0
loc_3_4E02:
        mov    edx, dword ptr [ebx + 10h]
        mov    al, byte ptr [ebp + 34h]
        and    ax, 6
        or     word ptr [edx + 0eh], ax
        mov    cx, word ptr [ebp + 38h]
        mov    word ptr [edx + 4ah], cx
        mov    eax, ecx
        and    eax, 0ffffh
        lea    edi, [edx + eax + 4ch]
        mov    dword ptr [edx], edi
        mov    word ptr [edx + 4ch], cx
        mov    ax, word ptr [ebp + 3ah]
        lea    esi, [ebp + 3ah]
        add    cx, ax
        mov    word ptr [edx + 48h], cx
        and    eax, 0ffffh
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ax, word ptr [edx + 48h]
        and    eax, 0fffeh
        mov    word ptr [edx + eax + 4ch], 0
        test   byte ptr [ebp + 35h], 8
        jne    SHORT loc_3_4E70
        and    dword ptr [ebx + 20h], 0ffff7fffh
        push   ebx
        call   sub_3_8D90
        add    esp, 4
loc_3_4E70:
        mov    byte ptr [ebx + 6dh], 22h
        push   27h
        push   ebx
        call   sub_3_8380
        add    esp, 8
loc_3_4E7F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_4E90:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        cmp    dword ptr [ebx + 24h], 0
        je     SHORT loc_3_4EBA
        mov    ebp, dword ptr [ebx + 1ch]
        cmp    dword ptr [ebx + 24h], 2
        jne    NEAR PTR loc_3_511C
        mov    dword ptr [ebx + 24h], 12h
        jmp    NEAR PTR loc_3_511C
loc_3_4EBA:
        test   byte ptr [ebx + 70h], 8
        jne    SHORT loc_3_4ED1
        push   ebx
        call   sub_3_9520
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_3_5146
loc_3_4ED1:
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [esp + 14h], eax
        test   byte ptr [ebx + 23h], 1
        jne    NEAR PTR loc_3_4F93
        mov    eax, dword ptr [eax]
        push   1
        mov    ax, word ptr [eax]
        and    eax, 0ffffh
        add    eax, 3ch
        push   eax
        call   sub_3_43B0
        add    esp, 8
        mov    ebp, eax
        mov    dword ptr [ebx + 1ch], ebp
        or     ebp, ebp
        jne    SHORT loc_3_4F10
        mov    dword ptr [ebx + 24h], 8
        jmp    NEAR PTR loc_3_511C
        db  090h
loc_3_4F10:
        lea    eax, [ebp + 0ch]
        mov    dword ptr [ebp + 4], eax
        mov    word ptr [ebp + 8], 1
        mov    byte ptr [ebp + 0bh], 2
        mov    edx, dword ptr [ebx + 14h]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebp + 10h], edx
        mov    ax, word ptr [eax + 0eh]
        and    al, 6
        mov    byte ptr [ebp + 34h], al
        test   byte ptr [ebx + 21h], 80h
        je     SHORT loc_3_4F3D
        or     byte ptr [ebp + 35h], 8
loc_3_4F3D:
        mov    ax, word ptr [ebx + 70h]
        and    ax, 0fff7h
        mov    word ptr [ebp + 36h], ax
        mov    eax, dword ptr [esp + 14h]
        lea    edi, [ebp + 3ah]
        mov    ax, word ptr [eax + 4ah]
        mov    word ptr [ebp + 38h], ax
        mov    eax, dword ptr [esp + 14h]
        mov    esi, dword ptr [eax]
        xor    eax, eax
        mov    ax, word ptr [esi]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        inc    word ptr [edx + 1ch]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 20h], ebp
        mov    eax, dword ptr [ebx]
        mov    eax, dword ptr [eax + 10h]
        db  0C7h, 000h, 040h, 04Ah, 000h, 000h
        db  0C7h, 040h, 004h, 040h, 03Ch, 000h, 000h
        db  0C7h, 040h, 008h, 020h, 009h, 000h, 000h
loc_3_4F93:
        mov    eax, dword ptr [ebx]
        mov    ebp, dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [eax + 14h]
        mov    dword ptr [esp + 10h], ecx
        test   byte ptr [ebx + 70h], 8
        je     SHORT loc_3_4FE9
        mov    dword ptr [ebx + 84h], 0
        mov    edx, dword ptr [ebx + 14h]
        xor    eax, eax
        mov    ax, word ptr [ebx + 76h]
        mov    cl, byte ptr [edx + 44h]
        shl    eax, cl
        mov    dword ptr [ebx + 88h], eax
        mov    dword ptr [ebx + 7ch], 0
        mov    ecx, dword ptr [esp + 10h]
        mov    eax, dword ptr [edx + 54h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx + 50h]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], 9
        mov    dword ptr [ecx + 20h], 0
        jmp    SHORT loc_3_5020
loc_3_4FE9:
        mov    eax, dword ptr [esp + 10h]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebx + 90h]
        add    eax, 12h
        push   eax
        call   sub_3_5300
        add    esp, 8
        push   ebx
        call   sub_3_94D0
        and    eax, 0ffh
        mov    ecx, dword ptr [esp + 14h]
        add    esp, 4
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 0c0h]
        mov    dword ptr [ecx + 20h], eax
loc_3_5020:
        mov    esi, dword ptr [esp + 10h]
        add    esi, 4
        push   esi
        call   sub_3_486B
        mov    ecx, dword ptr [esp + 14h]
        add    esp, 4
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx + 14h], eax
        mov    dword ptr [ecx + 10h], 0
        mov    dword ptr [ecx + 0ch], 0
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 1ch], 0
        mov    eax, dword ptr [eax + 4]
        mov    cx, word ptr [eax]
        and    ecx, 0fffeh
        mov    word ptr [eax + ecx], 0
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax + 4]
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    esi, 2
        add    edi, 2ch
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax + 0eh], 48h
        je     SHORT loc_3_50D3
        mov    eax, dword ptr [eax + 8]
        mov    cx, word ptr [eax]
        and    ecx, 0fffeh
        mov    word ptr [eax + ecx], 0
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax + 8]
        xor    eax, eax
        mov    ax, word ptr [esi]
        add    esi, 2
        add    edi, 234h
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_3_50E0
loc_3_50D3:
        mov    ecx, dword ptr [esp + 10h]
        mov    word ptr [ecx + 234h], 0
loc_3_50E0:
        mov    ax, word ptr [ebx + 66h]
        mov    word ptr [ebp + 1ch], ax
        mov    ax, word ptr [ebx + 76h]
        mov    word ptr [ebp + 1eh], ax
        mov    eax, dword ptr [ebx + 78h]
        mov    dword ptr [ebp + 20h], eax
        mov    eax, dword ptr [ebx + 8ch]
        mov    dword ptr [ebp + 24h], eax
        mov    ecx, dword ptr [ebx + 84h]
        mov    dword ptr [ebp + 28h], ecx
        mov    eax, dword ptr [ebx + 88h]
        mov    dword ptr [ebp + 2ch], eax
        mov    eax, dword ptr [ebx + 7ch]
        mov    dword ptr [ebp + 30h], eax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 24h], ecx
loc_3_511C:
        test   byte ptr [ebx + 23h], 1
        je     SHORT loc_3_513D
        add    ebp, 14h
        mov    dword ptr [ebx + 0b0h], ebp
        push   ebx
        call   sub_3_59D8
        add    esp, 4
        push   ebx
        call   sub_3_47C0
        add    esp, 4
loc_3_513D:
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 24h]
        mov    word ptr [eax + 1ah], cx
loc_3_5146:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
_PTEXT ends
        end
