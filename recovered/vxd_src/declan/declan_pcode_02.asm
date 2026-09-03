; declan_pcode_02.asm -- DECLAN object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 168h], 1
        call   sub_2_1540
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 16eh]
        push   eax
        db  0E8h, 0B1h, 019h, 0FEh, 0FFh
        push   2625a0h
        db  0E8h, 0BFh, 019h, 0FEh, 0FFh
        lea    eax, [esi + 1]
        push   eax
        db  0E8h, 0AAh, 019h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 21h]
        mov    dword ptr [ebp - 8], eax
        and    dword ptr [ebp - 8], 0ffff0000h
        or     dword ptr [ebp - 8], 0bffeh
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [esi + 17ch]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [esi + 21h]
        mov    dword ptr [ebp - 8], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 17eh]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 182h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0adh]
        push   eax
        call   sub_2_3110
        mov    dx, word ptr [esi + 41h]
        mov    ax, 4
        out    dx, ax
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 08Eh, 018h, 0FEh, 0FFh
        push   esi
        call   sub_2_0140
        push   0
        push   27eh
        push   esi
        db  0E8h, 04Fh, 019h, 0FEh, 0FFh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_00C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0A1h, 0D8h, 01Dh, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 047h, 018h, 0FEh, 0FFh
        push   0
        db  0A1h, 0D4h, 01Dh, 000h, 000h
        push   eax
        db  0E8h, 02Eh, 018h, 0FEh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_00F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 148h], 0
        jne    SHORT loc_2_0137
        mov    dword ptr [ebp - 18h], edi
        mov    word ptr [ebp - 10h], 0
        lea    esi, [edi + 1]
        lea    eax, [ebp - 18h]
        push   eax
        db  068h, 080h, 00Ah, 000h, 000h
        push   esi
        db  0E8h, 0CBh, 018h, 0FEh, 0FFh
        mov    dword ptr [ebp - 0ch], edi
        mov    word ptr [ebp - 4], 42h
        lea    eax, [ebp - 0ch]
        push   eax
        db  068h, 0A0h, 00Ah, 000h, 000h
        push   esi
        db  0E8h, 0B3h, 018h, 0FEh, 0FFh
loc_2_0137:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_2_0140:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 6ch]
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [esi + 70h]
        mov    ebx, dword ptr [esi + 74h]
        cmp    dword ptr [esi + 104h], 0
        je     SHORT loc_2_0193
        sub    eax, eax
        mov    edx, dword ptr [esi + 64h]
        cmp    edx, eax
        je     SHORT loc_2_017D
        mov    ecx, dword ptr [esi + 104h]
loc_2_0170:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_2_017D
        add    ecx, 4
        inc    eax
        cmp    edx, eax
        ja     SHORT loc_2_0170
loc_2_017D:
        push   0
        lea    eax, [edx*4]
        push   eax
        mov    eax, dword ptr [esi + 104h]
        push   eax
        db  0E8h, 069h, 018h, 0FEh, 0FFh
loc_2_0193:
        mov    eax, dword ptr [esi + 0c5h]
        or     eax, eax
        je     SHORT loc_2_01AF
        mov    ecx, dword ptr [esi + 60h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 04Dh, 018h, 0FEh, 0FFh
loc_2_01AF:
        mov    edx, dword ptr [esi + 0f5h]
        or     edx, edx
        je     SHORT loc_2_01E0
        sub    ecx, ecx
        lea    eax, [edi + ebx]
        mov    esi, dword ptr [ebp - 4]
        add    esi, eax
        je     SHORT loc_2_01D4
        mov    eax, edx
loc_2_01C7:
        cmp    dword ptr [eax], 0
        je     SHORT loc_2_01D4
        add    eax, 10h
        inc    ecx
        cmp    ecx, esi
        jb     SHORT loc_2_01C7
loc_2_01D4:
        push   0
        shl    esi, 4
        push   esi
        push   edx
        db  0E8h, 01Ch, 018h, 0FEh, 0FFh
loc_2_01E0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_01F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    eax, dword ptr [ecx + 84h]
        lea    esi, [ecx + 84h]
        sub    eax, ecx
        cmp    eax, 84h
        je     SHORT loc_2_0226
        mov    edi, edi
loc_2_0210:
        mov    esi, dword ptr [esi]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, edx
        jbe    SHORT loc_2_021B
        mov    edx, eax
loc_2_021B:
        mov    eax, dword ptr [esi]
        sub    eax, ecx
        cmp    eax, 84h
        jne    SHORT loc_2_0210
loc_2_0226:
        or     edx, edx
        jne    SHORT loc_2_022F
        mov    edx, 0eah
loc_2_022F:
        mov    dword ptr [ecx + 162h], edx
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0240:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 8]
        inc    dword ptr [esi + 80h]
        cmp    byte ptr [ebx + 1ch], 0
        jne    NEAR PTR loc_2_0380
        inc    dword ptr [ebx + 18h]
        push   0
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 0adh]
        push   eax
        call   sub_2_3230
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_2_02F0
        cmp    dword ptr [ebx + 18h], 2
        jne    SHORT loc_2_02B0
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 162h]
        cmp    dword ptr [ebx + 14h], eax
        jne    SHORT loc_2_029D
        push   esi
        call   sub_2_01F0
loc_2_029D:
        push   0
        push   39h
        push   ebx
        db  0E8h, 055h, 017h, 0FEh, 0FFh
        jmp    NEAR PTR loc_2_0385
        db  08Dh, 064h, 024h, 000h
loc_2_02B0:
        mov    byte ptr [ebx + 1ch], 1
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 8ch]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 90h]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 90h]
        mov    edi, 103h
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 90h], ebx
        sub    dword ptr [ebx + 18h], 2
        jmp    NEAR PTR loc_2_0385
loc_2_02F0:
        cmp    edi, 103h
        jne    SHORT loc_2_0330
        mov    byte ptr [ebx + 1ch], 1
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 8ch]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 90h]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 90h]
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 90h], ebx
        sub    dword ptr [ebx + 18h], 2
        jmp    SHORT loc_2_0385
loc_2_0330:
        cmp    edi, 0c001000eh
        jne    SHORT loc_2_0374
        mov    byte ptr [ebx + 1ch], 1
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    edi, 103h
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [esi + 8ch]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [esi + 90h]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 90h]
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 90h], ebx
        dec    dword ptr [ebx + 18h]
        jmp    SHORT loc_2_0385
loc_2_0374:
        dec    dword ptr [ebx + 18h]
        jmp    SHORT loc_2_0385
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0380:
        mov    edi, 0c0010002h
loc_2_0385:
        mov    eax, dword ptr [esi + 80h]
        dec    eax
        mov    dword ptr [esi + 80h], eax
        jne    SHORT loc_2_03C3
        cmp    byte ptr [esi + 148h], 0
        jne    SHORT loc_2_03B5
        cmp    dword ptr [esi + 0d5h], 0
        jne    SHORT loc_2_03B5
        mov    eax, dword ptr [esi + 8ch]
        sub    eax, esi
        cmp    eax, 8ch
        je     SHORT loc_2_03C3
loc_2_03B5:
        push   0
        add    esi, 0a0h
        push   esi
        db  0E8h, 00Fh, 016h, 0FEh, 0FFh
loc_2_03C3:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_03D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ecx + 8]
        cmp    byte ptr [esi + 169h], 0
        je     SHORT loc_2_03F0
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  090h
loc_2_03F0:
        mov    edx, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 80h]
        mov    eax, dword ptr [edx + 10h]
        or     eax, eax
        jne    SHORT loc_2_0410
        push   edx
        push   ecx
        push   esi
        call   sub_2_0850
        jmp    SHORT loc_2_0422
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0410:
        cmp    eax, 1
        mov    edi, 10001h
        jne    SHORT loc_2_0424
        push   edx
        push   ecx
        push   esi
        call   sub_2_08A0
loc_2_0422:
        mov    edi, eax
loc_2_0424:
        mov    eax, dword ptr [esi + 80h]
        dec    eax
        mov    dword ptr [esi + 80h], eax
        jne    SHORT loc_2_0462
        cmp    byte ptr [esi + 148h], 0
        jne    SHORT loc_2_0454
        cmp    dword ptr [esi + 0d5h], 0
        jne    SHORT loc_2_0454
        mov    eax, dword ptr [esi + 8ch]
        sub    eax, esi
        cmp    eax, 8ch
        je     SHORT loc_2_0462
loc_2_0454:
        push   0
        add    esi, 0a0h
        push   esi
        db  0E8h, 070h, 015h, 0FEh, 0FFh
loc_2_0462:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0470:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], 0
        lea    ebx, [ebp - 8]
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 14h], 0
        mov    edi, 4
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1010101h
        ja     SHORT loc_2_04C0
        je     NEAR PTR loc_2_06A0
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_2_04E1
        shl    eax, 2
        db  0FFh, 0A0h, 000h, 008h, 000h, 000h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_04C0:
        cmp    eax, 1010102h
        je     NEAR PTR loc_2_06C0
        cmp    eax, 1010103h
        je     NEAR PTR loc_2_06E0
        cmp    eax, 1010104h
        je     NEAR PTR loc_2_07B0
loc_2_04E1:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_2_07BE
        db  08Dh, 049h, 000h
loc_2_04F0:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_2_0505
        db  0BBh, 0B4h, 01Bh, 000h, 000h
        mov    edi, 58h
        jmp    NEAR PTR loc_2_07BE
loc_2_0505:
        db  0BBh, 03Ch, 01Bh, 000h, 000h
        mov    edi, 78h
        jmp    NEAR PTR loc_2_07BE
loc_2_0514:
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 148h], 0
        mov    dword ptr [ebp - 14h], 2
        jne    SHORT loc_2_052E
        mov    dword ptr [ebp - 14h], 0
loc_2_052E:
        lea    ebx, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_2_07BE
        db  005h, 000h, 000h, 000h, 000h
loc_2_0540:
        lea    ebx, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_2_07BE
        db  08Dh, 049h, 000h
loc_2_0550:
        mov    dword ptr [ebp - 8], 0eah
        jmp    NEAR PTR loc_2_07BE
        db  08Dh, 064h, 024h, 000h
loc_2_0560:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_2_07BE
        db  08Dh, 064h, 024h, 000h
loc_2_0570:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_2_07BE
        db  08Dh, 064h, 024h, 000h
loc_2_0580:
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 70h]
        shl    edx, 2
        add    edx, dword ptr [esi + 6ch]
        mov    eax, dword ptr [esi + 74h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [ecx + eax*4]
        shl    edx, 6
        lea    eax, [eax + eax*8]
        lea    eax, [ecx + eax*4]
        lea    edx, [edx + eax*2]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_2_07BE
        db  005h, 000h, 000h, 000h, 000h
loc_2_05B0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 68h]
        imul   eax, dword ptr [esi + 64h]
        jmp    NEAR PTR loc_2_07BB
        db  090h
loc_2_05C0:
        mov    dword ptr [ebp - 8], 40h
        jmp    NEAR PTR loc_2_07BE
        db  08Dh, 064h, 024h, 000h
loc_2_05D0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 68h]
        jmp    NEAR PTR loc_2_07BB
loc_2_05DB:
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi]
        cmp    al, 2
        jne    SHORT loc_2_05F3
        db  0BBh, 00Ch, 01Ch, 000h, 000h
        mov    edi, 15h
        jmp    NEAR PTR loc_2_07BE
loc_2_05F3:
        cmp    al, 1
        jne    SHORT loc_2_0606
        db  0BBh, 024h, 01Ch, 000h, 000h
        mov    edi, 15h
        jmp    NEAR PTR loc_2_07BE
loc_2_0606:
        cmp    al, 10h
        jne    SHORT loc_2_0620
        db  0BBh, 03Ch, 01Ch, 000h, 000h
        mov    edi, 15h
        jmp    NEAR PTR loc_2_07BE
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0620:
        db  0BBh, 054h, 01Ch, 000h, 000h
        mov    edi, 0fh
        jmp    NEAR PTR loc_2_07BE
        db  090h
loc_2_0630:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_0647
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0adh]
        mov    eax, dword ptr [eax + 0ch]
        jmp    NEAR PTR loc_2_07BB
loc_2_0647:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        jmp    NEAR PTR loc_2_07BB
loc_2_0655:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_2_0669
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 162h]
        jmp    NEAR PTR loc_2_07BB
loc_2_0669:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        jmp    NEAR PTR loc_2_07BB
loc_2_0674:
        mov    word ptr [ebp - 2], 301h
        lea    ebx, [ebp - 2]
        mov    edi, 2
        jmp    NEAR PTR loc_2_07BE
loc_2_0687:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_2_07BE
loc_2_0693:
        mov    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_2_07BE
        db  090h
loc_2_06A0:
        push   6
        mov    esi, dword ptr [ebp + 8]
        add    esi, 50h
        push   esi
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 02Ah, 014h, 0FEh, 0FFh
        lea    ebx, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_2_07BE
        db  090h
loc_2_06C0:
        push   6
        mov    esi, dword ptr [ebp + 8]
        add    esi, 56h
        push   esi
        lea    eax, [ebp - 20h]
        push   eax
        db  0E8h, 00Ah, 014h, 0FEh, 0FFh
        lea    ebx, [ebp - 20h]
        mov    edi, 6
        jmp    NEAR PTR loc_2_07BE
        db  090h
loc_2_06E0:
        cmp    byte ptr [ebp + 14h], 0
        mov    esi, dword ptr [ebp + 8]
        je     SHORT loc_2_0744
        mov    eax, dword ptr [esi + 0adh]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    ecx, dword ptr [ebp + 1ch]
        jbe    SHORT loc_2_0715
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    NEAR PTR loc_2_07BE
loc_2_0715:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 0adh]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_3750
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        sub    ebx, ebx
        sub    edi, edi
        jmp    SHORT loc_2_07BE
loc_2_0744:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 0adh]
        push   eax
        call   sub_2_3650
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [eax + eax*2]
        lea    ecx, [eax*2]
        cmp    ecx, dword ptr [ebp + 1ch]
        jbe    SHORT loc_2_0777
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0ch], 0c0010014h
        jmp    SHORT loc_2_07BE
loc_2_0777:
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 0adh]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_3690
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [ecx], eax
        sub    ebx, ebx
        sub    edi, edi
        jmp    SHORT loc_2_07BE
        db  08Dh, 049h, 000h
loc_2_07B0:
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    al, byte ptr [esi + 166h]
loc_2_07BB:
        mov    dword ptr [ebp - 8], eax
loc_2_07BE:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_07F1
        cmp    edi, dword ptr [ebp + 1ch]
        jbe    SHORT loc_2_07E1
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], edi
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_2_07E1:
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 0F0h, 012h, 0FEh, 0FFh
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_2_07F1:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 049h, 000h, 0F0h, 004h, 000h, 000h, 014h, 005h, 000h, 000h, 040h, 005h, 000h, 000h, 040h
        db  005h, 000h, 000h, 050h, 005h, 000h, 000h, 060h, 005h, 000h, 000h, 070h, 005h, 000h, 000h, 080h
        db  005h, 000h, 000h, 0B0h, 005h, 000h, 000h, 0C0h, 005h, 000h, 000h, 0D0h, 005h, 000h, 000h, 0E1h
        db  004h, 000h, 000h, 0DBh, 005h, 000h, 000h, 030h, 006h, 000h, 000h, 055h, 006h, 000h, 000h, 074h
        db  006h, 000h, 000h, 087h, 006h, 000h, 000h, 0E1h, 004h, 000h, 000h, 093h, 006h, 000h, 000h, 08Dh
        db  064h, 024h, 000h
sub_2_0850:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0470
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_2_08A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        sub    edi, edi
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010eh
        je     SHORT loc_2_08FA
        cmp    eax, 1010fh
        je     NEAR PTR loc_2_0960
        cmp    eax, 10112h
        je     NEAR PTR loc_2_09F0
        cmp    eax, 1010103h
        je     NEAR PTR loc_2_0A20
        mov    edi, 0c0010017h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_0A8A
loc_2_08FA:
        cmp    dword ptr [ebp - 4], 4
        je     SHORT loc_2_0918
        mov    edi, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_0A8A
loc_2_0918:
        push   4
        push   ecx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 0B8h, 011h, 0FEh, 0FFh
        test   dword ptr [ebp - 0ch], 0f050h
        je     SHORT loc_2_0945
        mov    edi, 0c00000bbh
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_0A8A
loc_2_0945:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_2_0AB0
        mov    edi, eax
        jmp    NEAR PTR loc_2_0A8A
        db  08Bh, 0FFh
loc_2_0960:
        cmp    dword ptr [ebp - 4], 4
        je     SHORT loc_2_0980
        mov    edi, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    NEAR PTR loc_2_0A8A
        db  08Bh, 0FFh
loc_2_0980:
        push   4
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 050h, 011h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 8], 0eah
        ja     SHORT loc_2_09E3
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebx + 14h]
        mov    eax, dword ptr [ecx + 162h]
        cmp    edx, eax
        jbe    SHORT loc_2_09C0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 162h], eax
        jmp    NEAR PTR loc_2_0A8A
        db  08Dh, 064h, 024h, 000h
loc_2_09C0:
        jne    SHORT loc_2_09D8
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_2_09D8
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 14h], eax
        push   ecx
        call   sub_2_01F0
        jmp    NEAR PTR loc_2_0A8A
loc_2_09D8:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 14h], eax
        jmp    NEAR PTR loc_2_0A8A
loc_2_09E3:
        mov    edi, 0c0010014h
        jmp    NEAR PTR loc_2_0A8A
        db  08Dh, 049h, 000h
loc_2_09F0:
        cmp    dword ptr [ebp - 4], 4
        je     SHORT loc_2_0A0B
        mov    edi, 0c0010014h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_2_0A8A
loc_2_0A0B:
        push   4
        push   ecx
        mov    ebx, dword ptr [ebp + 0ch]
        add    ebx, 35h
        push   ebx
        db  0E8h, 0C2h, 010h, 0FEh, 0FFh
        sub    edi, edi
        jmp    SHORT loc_2_0A8A
        db  08Bh, 0FFh
loc_2_0A20:
        mov    edi, 6
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    edi
        or     edx, edx
        je     SHORT loc_2_0A50
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        mov    eax, 0c0010014h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_2_0A50:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edi, 0c0010002h
        cmp    byte ptr [ebx + 1ch], 0
        jne    SHORT loc_2_0A8A
        inc    dword ptr [ebx + 18h]
        push   1
        push   ecx
        mov    ecx, 6
        mov    eax, dword ptr [ebp - 4]
        sub    edx, edx
        div    ecx
        push   eax
        push   esi
        mov    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 0adh]
        push   eax
        call   sub_2_32F0
        mov    edi, eax
        dec    dword ptr [ebx + 18h]
loc_2_0A8A:
        or     edi, edi
        jne    SHORT loc_2_0A9B
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], 0
loc_2_0A9B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, 0c0010002h
        inc    dword ptr [esi + 80h]
        cmp    byte ptr [edi + 1ch], 0
        jne    SHORT loc_2_0AEC
        inc    dword ptr [edi + 18h]
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [edi + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 0adh]
        push   eax
        call   sub_2_35D0
        dec    dword ptr [edi + 18h]
loc_2_0AEC:
        dec    dword ptr [esi + 80h]
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0B00:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_0470
        mov    edi, eax
        cmp    edi, 0c00000bbh
        jne    NEAR PTR loc_2_0C0D
        sub    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_2_0B86
        je     SHORT loc_2_0BC8
        cmp    eax, 20101h
        je     SHORT loc_2_0BA2
        cmp    eax, 20102h
        je     SHORT loc_2_0BB0
        cmp    eax, 20103h
        je     SHORT loc_2_0BB8
        cmp    eax, 20104h
        je     SHORT loc_2_0BC0
        mov    edi, 0c0010017h
        jmp    SHORT loc_2_0BE9
loc_2_0B86:
        cmp    eax, 1020101h
        je     SHORT loc_2_0BD0
        cmp    eax, 1020102h
        je     SHORT loc_2_0BD8
        cmp    eax, 1020103h
        je     SHORT loc_2_0BE0
        mov    edi, 0c0010017h
        jmp    SHORT loc_2_0BE9
loc_2_0BA2:
        mov    eax, dword ptr [ebx + 140h]
        add    eax, dword ptr [ebx + 120h]
        jmp    SHORT loc_2_0BE6
loc_2_0BB0:
        mov    eax, dword ptr [ebx + 144h]
        jmp    SHORT loc_2_0BE6
loc_2_0BB8:
        mov    eax, dword ptr [ebx + 11ch]
        jmp    SHORT loc_2_0BE6
loc_2_0BC0:
        mov    eax, dword ptr [ebx + 110h]
        jmp    SHORT loc_2_0BE6
loc_2_0BC8:
        mov    eax, dword ptr [ebx + 10ch]
        jmp    SHORT loc_2_0BE6
loc_2_0BD0:
        mov    eax, dword ptr [ebx + 114h]
        jmp    SHORT loc_2_0BE6
loc_2_0BD8:
        mov    eax, dword ptr [ebx + 12ch]
        jmp    SHORT loc_2_0BE6
loc_2_0BE0:
        mov    eax, dword ptr [ebx + 130h]
loc_2_0BE6:
        mov    dword ptr [ebp - 4], eax
loc_2_0BE9:
        or     edi, edi
        jne    SHORT loc_2_0C0D
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_2_0C08
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 0DAh, 00Eh, 0FEh, 0FFh
        add    dword ptr [ebp - 8], 4
        jmp    SHORT loc_2_0C0D
loc_2_0C08:
        mov    edi, 0c0010014h
loc_2_0C0D:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0C30:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        sub    eax, eax
        mov    ecx, dword ptr [edx + 14h]
        cmp    ecx, 20105h
        ja     SHORT loc_2_0C75
        cmp    ecx, 20101h
        jae    SHORT loc_2_0CA2
        cmp    ecx, 10101h
        jb     SHORT loc_2_0C9D
        cmp    ecx, 1010bh
        jbe    SHORT loc_2_0CA2
        cmp    ecx, 1010dh
        jb     SHORT loc_2_0C9D
        cmp    ecx, 10111h
        jbe    SHORT loc_2_0CA2
        cmp    ecx, 10113h
        je     SHORT loc_2_0CA2
        jmp    SHORT loc_2_0C9D
loc_2_0C75:
        cmp    ecx, 1010101h
        jb     SHORT loc_2_0C9D
        cmp    ecx, 1010104h
        jbe    SHORT loc_2_0CA2
        cmp    ecx, 1020101h
        jb     SHORT loc_2_0C9D
        cmp    ecx, 1020103h
        jbe    SHORT loc_2_0CA2
        cmp    ecx, 2010103h
        je     SHORT loc_2_0CA2
loc_2_0C9D:
        mov    eax, 0c0010017h
loc_2_0CA2:
        or     eax, eax
        jne    SHORT loc_2_0CB0
        push   edx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0B00
loc_2_0CB0:
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0CC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 103h
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 80h]
        cmp    byte ptr [esi + 148h], 0
        jne    SHORT loc_2_0D40
        cmp    byte ptr [edi + 1ch], 0
        jne    SHORT loc_2_0D35
        inc    dword ptr [edi + 18h]
        mov    ebx, dword ptr [esi + 84h]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 0ffffff7ch
        je     SHORT loc_2_0D25
        nop
loc_2_0D00:
        inc    dword ptr [ebx + 18h]
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        db  0E8h, 049h, 00Ch, 0FEh, 0FFh
        dec    dword ptr [ebx + 18h]
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, 0ffffff7ch
        jne    SHORT loc_2_0D00
loc_2_0D25:
        push   8
        push   0
        push   edi
        push   esi
        call   sub_2_1460
        dec    dword ptr [edi + 18h]
        jmp    SHORT loc_2_0D47
loc_2_0D35:
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_2_0D47
        db  08Bh, 0FFh
loc_2_0D40:
        mov    dword ptr [ebp - 4], 0
loc_2_0D47:
        mov    eax, dword ptr [esi + 80h]
        dec    eax
        mov    dword ptr [esi + 80h], eax
        jne    SHORT loc_2_0D85
        cmp    byte ptr [esi + 148h], 0
        jne    SHORT loc_2_0D77
        cmp    dword ptr [esi + 0d5h], 0
        jne    SHORT loc_2_0D77
        mov    eax, dword ptr [esi + 8ch]
        sub    eax, esi
        cmp    eax, 8ch
        je     SHORT loc_2_0D85
loc_2_0D77:
        push   0
        add    esi, 0a0h
        push   esi
        db  0E8h, 04Dh, 00Ch, 0FEh, 0FFh
loc_2_0D85:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0DA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0cch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    eax, eax
        mov    edi, dword ptr [esi + 0b1h]
        mov    ecx, 6
        rep stosd
        sub    cl, cl
        nop
loc_2_0DC0:
        xor    eax, eax
        mov    al, cl
        mov    dl, byte ptr [esi + eax + 56h]
        mov    edi, dword ptr [esi + 0b1h]
        mov    byte ptr [eax + edi + 2], dl
        inc    cl
        cmp    cl, 6
        jb     SHORT loc_2_0DC0
        mov    ecx, dword ptr [esi + 0c9h]
        sub    ecx, dword ptr [esi + 21h]
        mov    eax, dword ptr [esi + 0b1h]
        add    ecx, dword ptr [esi + 25h]
        mov    word ptr [eax + 14h], cx
        mov    eax, dword ptr [esi + 0b1h]
        shr    ecx, 10h
        mov    byte ptr [eax + 16h], cl
        mov    edi, dword ptr [esi + 0d1h]
        mov    eax, dword ptr [esi + 25h]
        sub    eax, dword ptr [esi + 21h]
        lea    ecx, [edi + eax]
        mov    byte ptr [ebp - 1], 0
        cmp    dword ptr [esi + 64h], 0
        jbe    SHORT loc_2_0E2C
        mov    edx, 80h
loc_2_0E19:
        mov    byte ptr [edi + 3], dl
        inc    byte ptr [ebp - 1]
        add    edi, 8
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        cmp    eax, dword ptr [esi + 64h]
        jb     SHORT loc_2_0E19
loc_2_0E2C:
        mov    eax, dword ptr [esi + 0b1h]
        mov    word ptr [eax + 10h], cx
        mov    eax, dword ptr [esi + 0b1h]
        shr    ecx, 10h
        mov    byte ptr [eax + 12h], cl
        mov    ecx, dword ptr [esi + 78h]
        mov    eax, dword ptr [esi + 0b1h]
        shl    cl, 5
        mov    byte ptr [eax + 17h], cl
        mov    ecx, dword ptr [esi + 7ch]
        mov    eax, dword ptr [esi + 0b1h]
        shl    cl, 5
        mov    byte ptr [eax + 13h], cl
        mov    ecx, dword ptr [esi + 0adh]
        mov    eax, dword ptr [ecx + 0ch]
        test   al, 20h
        je     SHORT loc_2_0E90
        mov    eax, dword ptr [esi + 0b1h]
        pop    edi
        mov    ax, word ptr [eax]
        mov    cx, ax
        mov    eax, dword ptr [esi + 0b1h]
        or     cx, 8000h
        mov    word ptr [eax], cx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_0E90:
        test   al, 4
        je     SHORT loc_2_0EF0
        mov    eax, dword ptr [esi + 0b1h]
        mov    byte ptr [eax + 8], 0ffh
        mov    eax, dword ptr [esi + 0b1h]
        mov    byte ptr [eax + 9], 0ffh
        mov    eax, dword ptr [esi + 0b1h]
        mov    byte ptr [eax + 0ah], 0ffh
        mov    eax, dword ptr [esi + 0b1h]
        mov    byte ptr [eax + 0bh], 0ffh
        mov    eax, dword ptr [esi + 0b1h]
        mov    byte ptr [eax + 0ch], 0ffh
        mov    eax, dword ptr [esi + 0b1h]
        pop    edi
        mov    byte ptr [eax + 0dh], 0ffh
        mov    eax, dword ptr [esi + 0b1h]
        mov    byte ptr [eax + 0eh], 0ffh
        mov    eax, dword ptr [esi + 0b1h]
        pop    esi
        mov    byte ptr [eax + 0fh], 0ffh
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_0EF0:
        test   al, 2
        je     NEAR PTR loc_2_0FBF
        lea    eax, [ebp - 0cch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   0c0h
        push   ecx
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_3750
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_2_0FBF
        lea    esp, [esp]
loc_2_0F20:
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        lea    eax, [ebp + eax*2 - 0d2h]
        push   eax
        push   6
        call   sub_2_0FD0
        mov    edx, eax
        and    edx, 1
        cmp    edx, 1
        cmc
        sbb    edx, edx
        and    edx, 20h
        mov    edi, eax
        and    edi, 2
        cmp    edi, 1
        cmc
        sbb    edi, edi
        and    edi, 10h
        or     edx, edi
        mov    edi, eax
        and    edi, 4
        cmp    edi, 1
        cmc
        sbb    edi, edi
        and    edi, 8
        or     edx, edi
        mov    edi, eax
        and    edi, 8
        cmp    edi, 1
        cmc
        sbb    edi, edi
        and    edi, 4
        or     edx, edi
        mov    edi, eax
        and    edi, 10h
        cmp    edi, 1
        cmc
        sbb    edi, edi
        and    edi, 2
        or     edx, edi
        and    eax, 20h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        or     edx, eax
        mov    edi, edx
        shr    edi, 3
        mov    eax, dword ptr [esi + 0b1h]
        mov    al, byte ptr [edi + eax + 8]
        mov    byte ptr [ebp - 1], al
        mov    al, 1
        and    dl, 7
        mov    cl, dl
        shl    al, cl
        or     al, byte ptr [ebp - 1]
        mov    ecx, dword ptr [esi + 0b1h]
        mov    byte ptr [edi + ecx + 8], al
        dec    dword ptr [ebp - 8]
        jne    NEAR PTR loc_2_0F20
loc_2_0FBF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0FD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 0ffffffffh
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_2_1012
        mov    ecx, dword ptr [ebp + 0ch]
loc_2_0FE4:
        mov    dl, byte ptr [ecx]
        inc    ecx
        mov    esi, 8
loc_2_0FF0:
        mov    edi, eax
        shr    edi, 1fh
        add    eax, eax
        mov    bl, dl
        and    ebx, 1
        cmp    ebx, edi
        je     SHORT loc_2_1008
        xor    eax, 4c11db6h
        or     eax, 1
loc_2_1008:
        shr    dl, 1
        dec    esi
        jne    SHORT loc_2_0FF0
        dec    dword ptr [ebp + 8]
        jne    SHORT loc_2_0FE4
loc_2_1012:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1020:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [edx + 8]
        cmp    byte ptr [eax + 169h], 0
        je     SHORT loc_2_103A
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    1ch
loc_2_103A:
        mov    esi, dword ptr [ebp + 1ch]
        or     esi, esi
        jne    SHORT loc_2_1044
        lea    esi, [edx + 1dh]
loc_2_1044:
        cmp    byte ptr [eax + 148h], 0
        je     SHORT loc_2_1090
        cmp    dword ptr [eax + 156h], 0
        jne    SHORT loc_2_1064
        mov    dword ptr [eax + 15ah], esi
        mov    dword ptr [eax + 156h], esi
        jmp    SHORT loc_2_106C
loc_2_1064:
        mov    eax, dword ptr [eax + 15ah]
        mov    dword ptr [eax], esi
loc_2_106C:
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], edx
        cmp    byte ptr [ebp + 20h], 1
        sbb    eax, eax
        and    eax, 0fffffffbh
        add    eax, 9
        mov    dword ptr [esi + 8], eax
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh
loc_2_1090:
        cmp    byte ptr [ebp + 20h], 1
        sbb    ecx, ecx
        and    ecx, 0fffffffbh
        add    ecx, 9
        push   ecx
        push   esi
        push   edx
        push   eax
        call   sub_2_1460
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    1ch
        db  090h
loc_2_10B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [edx + 8]
        cmp    byte ptr [eax + 169h], 0
        je     SHORT loc_2_10CA
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    14h
loc_2_10CA:
        mov    esi, dword ptr [ebp + 14h]
        or     esi, esi
        jne    SHORT loc_2_10D4
        lea    esi, [edx + 29h]
loc_2_10D4:
        cmp    byte ptr [eax + 148h], 0
        je     SHORT loc_2_1120
        cmp    dword ptr [eax + 156h], 0
        jne    SHORT loc_2_10F4
        mov    dword ptr [eax + 15ah], esi
        mov    dword ptr [eax + 156h], esi
        jmp    SHORT loc_2_10FC
loc_2_10F4:
        mov    eax, dword ptr [eax + 15ah]
        mov    dword ptr [eax], esi
loc_2_10FC:
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], edx
        cmp    byte ptr [ebp + 18h], 1
        sbb    eax, eax
        and    eax, 0fffffffbh
        add    eax, 9
        mov    dword ptr [esi + 8], eax
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_2_1120:
        cmp    byte ptr [ebp + 18h], 1
        sbb    ecx, ecx
        and    ecx, 0fffffffah
        add    ecx, 0ah
        push   ecx
        push   esi
        push   edx
        push   eax
        call   sub_2_1460
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    14h
        db  090h
loc_2_1140:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 18h]
        ret    4
        db  08Dh, 049h, 000h
sub_2_1150:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 3
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp - 8]
loc_2_1166:
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 1
        je     SHORT loc_2_1180
        cmp    eax, 2
        je     SHORT loc_2_1188
        cmp    eax, 3
        je     SHORT loc_2_1190
        jmp    SHORT loc_2_1196
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1180:
        mov    edi, dword ptr [esi + 0ddh]
        jmp    SHORT loc_2_1196
loc_2_1188:
        mov    edi, dword ptr [esi + 0fch]
        jmp    SHORT loc_2_1196
loc_2_1190:
        mov    edi, dword ptr [esi + 0d5h]
loc_2_1196:
        or     edi, edi
        je     SHORT loc_2_11E6
loc_2_119A:
        lea    eax, [edi + 24h]
        mov    ecx, edi
        mov    edi, dword ptr [eax]
        mov    ebx, dword ptr [eax + 4]
        cmp    dword ptr [esi + 152h], 8
        je     SHORT loc_2_11D0
        cmp    dword ptr [ebp - 4], 1
        je     SHORT loc_2_11B9
        cmp    dword ptr [ebp - 4], 3
        jne    SHORT loc_2_11E2
loc_2_11B9:
        inc    dword ptr [esi + 140h]
        push   0
        push   ecx
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        db  0E8h, 06Fh, 007h, 0FEh, 0FFh
        dec    dword ptr [ebx + 18h]
        jmp    SHORT loc_2_11E2
loc_2_11D0:
        dec    dword ptr [ebx + 18h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        db  0E8h, 058h, 007h, 0FEh, 0FFh
loc_2_11E2:
        or     edi, edi
        jne    SHORT loc_2_119A
loc_2_11E6:
        dec    dword ptr [ebp - 4]
        jne    NEAR PTR loc_2_1166
        mov    word ptr [esi + 5eh], 0
        mov    eax, dword ptr [esi + 60h]
        mov    dword ptr [esi + 0b5h], eax
        mov    eax, dword ptr [esi + 0c9h]
        mov    dword ptr [esi + 0b9h], eax
        mov    dword ptr [esi + 0bdh], eax
        mov    dword ptr [esi + 0c1h], eax
        mov    edx, 80h
        mov    byte ptr [esi + 0f9h], 1
        mov    byte ptr [esi + 0fah], 0
        mov    dword ptr [esi + 108h], 0
        mov    ecx, dword ptr [esi + 0d1h]
        mov    eax, dword ptr [esi + 64h]
        lea    eax, [ecx + eax*8]
loc_2_1240:
        mov    byte ptr [ecx + 3], dl
        add    ecx, 8
        cmp    ecx, eax
        jne    SHORT loc_2_1240
        mov    ecx, dword ptr [esi + 0c9h]
        mov    eax, dword ptr [esi + 60h]
        lea    eax, [ecx + eax*8]
        sub    edx, edx
loc_2_1258:
        mov    byte ptr [ecx + 3], dl
        add    ecx, 8
        cmp    ecx, eax
        jne    SHORT loc_2_1258
        sub    edx, edx
        mov    eax, dword ptr [esi + 6ch]
        add    eax, dword ptr [esi + 70h]
        add    eax, dword ptr [esi + 74h]
        je     SHORT loc_2_128C
        sub    ecx, ecx
loc_2_1271:
        inc    edx
        mov    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [ecx + eax + 4], edx
        add    ecx, 10h
        mov    eax, dword ptr [esi + 6ch]
        add    eax, dword ptr [esi + 70h]
        add    eax, dword ptr [esi + 74h]
        cmp    eax, edx
        ja     SHORT loc_2_1271
loc_2_128C:
        mov    dword ptr [esi + 0e5h], 0ffffffffh
        mov    dword ptr [esi + 0e9h], 0
        mov    eax, dword ptr [esi + 6ch]
        shl    eax, 4
        mov    ecx, dword ptr [esi + 0f5h]
        mov    dword ptr [eax + ecx - 0ch], 0ffffffffh
        mov    eax, dword ptr [esi + 6ch]
        mov    dword ptr [esi + 0edh], eax
        mov    ecx, dword ptr [esi + 70h]
        add    ecx, eax
        shl    ecx, 4
        mov    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [ecx + eax - 0ch], 0ffffffffh
        mov    eax, dword ptr [esi + 6ch]
        add    eax, dword ptr [esi + 70h]
        mov    dword ptr [esi + 0f1h], eax
        mov    eax, dword ptr [esi + 70h]
        add    eax, dword ptr [esi + 6ch]
        add    eax, dword ptr [esi + 74h]
        shl    eax, 4
        mov    ecx, dword ptr [esi + 0f5h]
        mov    dword ptr [eax + ecx - 0ch], 0ffffffffh
        cmp    dword ptr [esi + 152h], 8
        jne    SHORT loc_2_1340
        mov    dword ptr [esi + 0d5h], 0
        mov    dword ptr [esi + 0d9h], 0
        mov    dword ptr [esi + 0ddh], 0
        mov    dword ptr [esi + 0e1h], 0
        mov    dword ptr [esi + 0fch], 0
        mov    dword ptr [esi + 100h], 0
        jmp    SHORT loc_2_1388
loc_2_1340:
        mov    dword ptr [esi + 0ddh], 0
        mov    dword ptr [esi + 0e1h], 0
        mov    edi, dword ptr [esi + 0fch]
        mov    dword ptr [esi + 0fch], 0
        or     edi, edi
        mov    dword ptr [esi + 100h], 0
        je     SHORT loc_2_1388
loc_2_1372:
        lea    ecx, [edi + 24h]
        mov    eax, edi
        mov    edi, dword ptr [ecx]
        push   eax
        mov    eax, dword ptr [ecx + 4]
        push   eax
        push   esi
        db  0E8h, 05Ch, 0FAh, 0FDh, 0FFh
        or     edi, edi
        jne    SHORT loc_2_1372
loc_2_1388:
        push   esi
        call   sub_2_13A0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_13A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_0DA0
        mov    ebx, dword ptr [edi + 25h]
        sub    ebx, dword ptr [edi + 21h]
        add    ebx, dword ptr [edi + 0b1h]
        mov    dword ptr [ebp - 48h], edi
        lea    esi, [edi + 1]
        mov    word ptr [ebp - 40h], 1
        lea    eax, [ebp - 48h]
        push   eax
        db  068h, 080h, 00Ah, 000h, 000h
        push   esi
        db  0E8h, 011h, 006h, 0FEh, 0FFh
        mov    dword ptr [ebp - 3ch], edi
        mov    word ptr [ebp - 34h], bx
        lea    eax, [ebp - 3ch]
        push   eax
        db  068h, 0A0h, 00Ah, 000h, 000h
        push   esi
        db  0E8h, 0FBh, 005h, 0FEh, 0FFh
        mov    dword ptr [ebp - 30h], edi
        mov    word ptr [ebp - 28h], 2
        lea    eax, [ebp - 30h]
        push   eax
        db  068h, 080h, 00Ah, 000h, 000h
        push   esi
        db  0E8h, 0E3h, 005h, 0FEh, 0FFh
        mov    dword ptr [ebp - 24h], edi
        shr    ebx, 10h
        mov    al, bl
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 1ch], ax
        lea    eax, [ebp - 24h]
        push   eax
        db  068h, 0A0h, 00Ah, 000h, 000h
        push   esi
        db  0E8h, 0C3h, 005h, 0FEh, 0FFh
        mov    dword ptr [ebp - 18h], edi
        mov    word ptr [ebp - 10h], 0
        lea    eax, [ebp - 18h]
        push   eax
        db  068h, 080h, 00Ah, 000h, 000h
        push   esi
        db  0E8h, 0ABh, 005h, 0FEh, 0FFh
        mov    dword ptr [ebp - 0ch], edi
        mov    word ptr [ebp - 4], 41h
        lea    eax, [ebp - 0ch]
        push   eax
        db  068h, 0A0h, 00Ah, 000h, 000h
        push   esi
        db  0E8h, 093h, 005h, 0FEh, 0FFh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_1460:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  068h, 040h, 015h, 000h, 000h
        lea    eax, [esi + 1]
        push   eax
        db  0E8h, 074h, 005h, 0FEh, 0FFh
        mov    word ptr [esi + 5eh], 0
        mov    byte ptr [esi + 148h], 1
        mov    byte ptr [esi + 149h], 0
        mov    byte ptr [esi + 0f9h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 14eh], eax
        mov    eax, dword ptr [ebp + 0ch]
        or     eax, eax
        mov    dword ptr [esi + 14ah], eax
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [esi + 152h], ecx
        je     SHORT loc_2_14B3
        inc    dword ptr [eax + 18h]
loc_2_14B3:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_14C0:
        push   ebp
        mov    ebp, esp
        push   esi
        cmp    byte ptr [ebp + 0ch], 0
        mov    esi, dword ptr [ebp + 8]
        je     SHORT loc_2_1500
        push   0
        mov    eax, dword ptr [esi + 14eh]
        push   eax
        mov    eax, dword ptr [esi + 14ah]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 067h, 004h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 14ah]
        mov    dword ptr [esi + 14eh], 0
        dec    dword ptr [eax + 18h]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_2_1500:
        push   0c0000001h
        mov    eax, dword ptr [esi + 14eh]
        push   eax
        mov    eax, dword ptr [esi + 14ah]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 031h, 004h, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 14ah]
        mov    dword ptr [esi + 14eh], 0
        dec    dword ptr [eax + 18h]
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1540:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    ax, ax
        mov    di, word ptr [ecx + 39h]
        test   byte ptr [ecx], 13h
        mov    dx, di
        out    dx, ax
        mov    si, word ptr [ecx + 3dh]
        mov    dx, si
        mov    ax, 4
        out    dx, ax
        je     SHORT loc_2_1578
        mov    dx, di
        mov    ax, 3
        out    dx, ax
        mov    dx, si
        mov    ax, 2
        out    dx, ax
loc_2_1578:
        mov    dx, di
        sub    ax, ax
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1590:
        push   ebp
        mov    ebp, esp
        sub    esp, 50h
        mov    word ptr [ebp - 0ch], 7
        mov    word ptr [ebp - 0ah], 7
        db  0C7h, 045h, 0F8h, 06Ch, 01Ch, 000h, 000h
        mov    word ptr [ebp - 18h], 1
        mov    word ptr [ebp - 16h], 0
        mov    dword ptr [ebp - 14h], 2042a310h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        db  068h, 0D4h, 01Dh, 000h, 000h
        db  0E8h, 03Ah, 003h, 0FEh, 0FFh
        mov    byte ptr [ebp - 50h], 3
        mov    byte ptr [ebp - 4fh], 0
        db  0C7h, 045h, 0B6h, 0E0h, 02Bh, 000h, 000h
        db  0C7h, 045h, 0BAh, 040h, 002h, 000h, 000h
        db  0C7h, 045h, 0BEh, 0A0h, 00Ch, 000h, 000h
        db  0C7h, 045h, 0C2h, 0A0h, 012h, 000h, 000h
        db  0C7h, 045h, 0C6h, 0C0h, 00Ch, 000h, 000h
        db  0C7h, 045h, 0CAh, 0D0h, 003h, 000h, 000h
        db  0C7h, 045h, 0D6h, 070h, 016h, 000h, 000h
        db  0C7h, 045h, 0D2h, 0C0h, 000h, 000h, 000h
        db  0C7h, 045h, 0DAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 030h, 00Ch, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 22h], eax
        lea    eax, [ebp - 50h]
        mov    dword ptr [ebp - 1eh], ecx
        push   36h
        push   eax
        db  068h, 0D8h, 01Dh, 000h, 000h
        db  0A1h, 0D4h, 01Dh, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        db  068h, 0D8h, 01Dh, 000h, 000h
        push   eax
        db  0E8h, 0CDh, 002h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_1657
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_1657:
        push   0
        db  0A1h, 0D4h, 01Dh, 000h, 000h
        push   eax
        db  0E8h, 0ACh, 002h, 0FEh, 0FFh
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_2_1670:
        push   ebp
        mov    ebp, esp
        sub    esp, 200h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 48h], 0ah
        mov    word ptr [ebp - 46h], 0ah
        db  0C7h, 045h, 0BCh, 080h, 01Ch, 000h, 000h
        mov    word ptr [ebp - 50h], 0dh
        mov    word ptr [ebp - 4eh], 0dh
        db  0C7h, 045h, 0B4h, 09Ch, 01Ch, 000h, 000h
        mov    word ptr [ebp - 40h], 0ah
        mov    word ptr [ebp - 3eh], 0ah
        db  0C7h, 045h, 0C4h, 0D0h, 01Ch, 000h, 000h
        mov    word ptr [ebp - 88h], 0ch
        mov    word ptr [ebp - 86h], 0ch
        db  0C7h, 085h, 07Ch, 0FFh, 0FFh, 0FFh, 0E8h, 01Ch, 000h, 000h
        mov    word ptr [ebp - 38h], 0bh
        mov    word ptr [ebp - 36h], 0bh
        db  0C7h, 045h, 0CCh, 000h, 01Dh, 000h, 000h
        mov    word ptr [ebp - 80h], 6
        mov    word ptr [ebp - 7eh], 6
        db  0C7h, 045h, 084h, 014h, 01Dh, 000h, 000h
        mov    word ptr [ebp - 78h], 6
        mov    word ptr [ebp - 76h], 6
        db  0C7h, 045h, 08Ch, 024h, 01Dh, 000h, 000h
        mov    word ptr [ebp - 70h], 6
        mov    word ptr [ebp - 6eh], 6
        db  0C7h, 045h, 094h, 034h, 01Dh, 000h, 000h
        mov    word ptr [ebp - 68h], 6
        mov    word ptr [ebp - 66h], 6
        db  0C7h, 045h, 09Ch, 054h, 01Dh, 000h, 000h
        sub    si, si
        mov    word ptr [ebp - 60h], 6
        mov    word ptr [ebp - 5eh], 6
        db  0C7h, 045h, 0A4h, 064h, 01Dh, 000h, 000h
        mov    word ptr [ebp - 58h], 6
        sub    bl, bl
        mov    word ptr [ebp - 56h], 6
        db  0C7h, 045h, 0ACh, 074h, 01Dh, 000h, 000h
        mov    dword ptr [ebp - 28h], 20h
        mov    byte ptr [ebp - 2], bl
        db  0A1h, 038h, 01Bh, 000h, 000h
        push   eax
        lea    eax, [ebp - 0ch]
        push   0
        push   27eh
        push   eax
        db  0E8h, 07Bh, 002h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_1790
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_2_1790:
        mov    edi, dword ptr [ebp - 0ch]
        sub    eax, eax
        mov    ecx, 9fh
        rep stosd
        stosw
        mov    ecx, dword ptr [ebp - 0ch]
        db  0A1h, 0D8h, 01Dh, 000h, 000h
        mov    dword ptr [ecx + 98h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 2
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 39h], 306h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 3dh], 304h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 41h], 300h
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 49h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 25h], 0d0000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 2dh], 10000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 5ch], 5
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 5dh], 5
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 168h], 0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 05Eh, 002h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_2_1830
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1830:
        push   2
        lea    eax, [ebp - 88h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 030h, 002h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_2_1870
        push   1
        lea    eax, [ebp - 80h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 4
        push   eax
        db  0E8h, 024h, 002h, 0FEh, 0FFh
        or     al, al
        je     NEAR PTR loc_2_19B2
loc_2_186A:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 2
loc_2_1870:
        push   0
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 0F3h, 001h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_2_1896
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 28h], ecx
loc_2_1896:
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 0D1h, 000h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 30h], 6
        jne    SHORT loc_2_18D2
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_2_18D2
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 2ch]
        mov    ecx, edx
        add    eax, 56h
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edx + 5]
        mov    byte ptr [eax + 5], cl
loc_2_18D2:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        test   al, 3
        je     NEAR PTR loc_2_1B71
        push   1
        lea    eax, [ebp - 48h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 084h, 001h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_2_1935
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 300h
        jne    NEAR PTR loc_2_1A61
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 39h], 306h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 3dh], 304h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 41h], 300h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 4ch], 30ch
loc_2_1935:
        push   0
        lea    eax, [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 02Eh, 001h, 0FEh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_2_1AD0
        mov    eax, dword ptr [ebp - 18h]
        mov    cl, byte ptr [eax + 4]
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 5ch], cl
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [eax + 5ch]
        mov    byte ptr [eax + 5dh], cl
        mov    eax, dword ptr [ebp - 0ch]
        cmp    byte ptr [eax], 2
        jne    NEAR PTR loc_2_1AA5
        mov    al, byte ptr [eax + 5ch]
        cmp    al, 5
        je     NEAR PTR loc_2_1AD0
        cmp    al, 9
        je     NEAR PTR loc_2_1AD0
        cmp    al, 0ah
        je     NEAR PTR loc_2_1AD0
        cmp    al, 0bh
        je     NEAR PTR loc_2_1AD0
        cmp    al, 0fh
        je     NEAR PTR loc_2_1AD0
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 20h], 0aa00000ah
        jmp    NEAR PTR loc_2_1D52
loc_2_19B2:
        push   1
        lea    eax, [ebp - 78h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 4
        push   eax
        db  0E8h, 0C2h, 000h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_19D3
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 1
        jmp    NEAR PTR loc_2_1870
loc_2_19D3:
        push   1
        lea    eax, [ebp - 70h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 4
        push   eax
        db  0E8h, 0A1h, 000h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_19F4
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 4
        jmp    NEAR PTR loc_2_1870
loc_2_19F4:
        push   1
        lea    eax, [ebp - 68h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 4
        push   eax
        db  0E8h, 080h, 000h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_1A15
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 10h
        jmp    NEAR PTR loc_2_1870
loc_2_1A15:
        push   1
        lea    eax, [ebp - 60h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 4
        push   eax
        db  0E8h, 05Fh, 000h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_1A30
        jmp    NEAR PTR loc_2_186A
loc_2_1A30:
        push   1
        lea    eax, [ebp - 58h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        add    eax, 4
        push   eax
        db  0E8h, 044h, 000h, 0FEh, 0FFh
        or     al, al
        je     SHORT loc_2_1A51
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 1
        jmp    NEAR PTR loc_2_1870
loc_2_1A51:
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 20h], 0aa00000ah
        jmp    NEAR PTR loc_2_1D52
loc_2_1A61:
        cmp    eax, 200h
        jne    SHORT loc_2_1A95
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 39h], 206h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 3dh], 204h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 41h], 200h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 4ch], 20ch
        jmp    NEAR PTR loc_2_1935
loc_2_1A95:
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 20h], 0aa00000dh
        jmp    NEAR PTR loc_2_1D52
loc_2_1AA5:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 5ch]
        cmp    al, 2
        je     SHORT loc_2_1AD0
        cmp    al, 3
        je     SHORT loc_2_1AD0
        cmp    al, 4
        je     SHORT loc_2_1AD0
        cmp    al, 5
        je     SHORT loc_2_1AD0
        cmp    al, 7
        je     SHORT loc_2_1AD0
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 20h], 0aa00000ah
        jmp    NEAR PTR loc_2_1D52
        db  090h
loc_2_1AD0:
        push   1
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 093h, 0FFh, 0FDh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_2_1B39
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 25h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 25h]
        cmp    eax, 0c0000h
        je     SHORT loc_2_1B39
        cmp    eax, 0c8000h
        je     SHORT loc_2_1B39
        cmp    eax, 0d0000h
        je     SHORT loc_2_1B39
        cmp    eax, 0d8000h
        je     SHORT loc_2_1B39
        cmp    eax, 0e0000h
        je     SHORT loc_2_1B39
        cmp    eax, 0e8000h
        je     SHORT loc_2_1B39
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 20h], 0aa00000ah
        jmp    NEAR PTR loc_2_1D52
loc_2_1B39:
        mov    eax, dword ptr [ebp - 0ch]
        test   byte ptr [eax + 26h], 80h
        je     SHORT loc_2_1B58
        mov    dword ptr [eax + 2dh], 8000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 29h], 8000h
        jmp    NEAR PTR loc_2_1D52
loc_2_1B58:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 2dh], 10000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 29h], 0
        jmp    NEAR PTR loc_2_1D52
loc_2_1B71:
        cmp    al, 4
        jne    SHORT loc_2_1BD0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 4ch], 20ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 5ch], 5
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 5dh], 5
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 39h], 206h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 3dh], 204h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 2dh], 10000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 25h], 0d0000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 41h], 200h
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax], 1
        jmp    NEAR PTR loc_2_1D52
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1BD0:
        cmp    al, 10h
        jne    NEAR PTR loc_2_1D52
        lea    eax, [ebp - 200h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 45h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        db  0E8h, 083h, 0FDh, 0FDh, 0FFh
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_2_1C09
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 20h], 0aa00000ah
        jmp    NEAR PTR loc_2_1D52
loc_2_1C09:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 45h]
        shl    eax, 0ch
        add    eax, 0c06h
        mov    dword ptr [ecx + 39h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 45h]
        shl    eax, 0ch
        add    eax, 0c04h
        mov    dword ptr [ecx + 3dh], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 45h]
        shl    eax, 0ch
        add    eax, 0c00h
        mov    dword ptr [ecx + 41h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 45h]
        shl    eax, 0ch
        add    eax, 0c0ch
        mov    dword ptr [ecx + 4ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [ebp - 1], 0
        mov    eax, dword ptr [eax + 4ch]
        lea    edx, [ebp - 0fch]
        mov    dword ptr [ebp - 14h], eax
        lea    esp, [esp]
loc_2_1C70:
        mov    al, byte ptr [edx]
        mov    cx, word ptr [edx + 1]
        inc    edx
        mov    word ptr [ebp - 6], cx
        inc    edx
        inc    edx
        test   al, 80h
        jne    SHORT loc_2_1C85
        mov    byte ptr [ebp - 1], 1
loc_2_1C85:
        mov    cx, word ptr [ebp - 14h]
        cmp    cx, word ptr [ebp - 6]
        jne    SHORT loc_2_1C94
        mov    cx, word ptr [edx]
        jmp    SHORT loc_2_1CB5
loc_2_1C94:
        mov    ecx, dword ptr [ebp - 0ch]
        xor    edi, edi
        mov    ecx, dword ptr [ecx + 45h]
        mov    di, word ptr [ebp - 6]
        shl    ecx, 0ch
        sub    ecx, edi
        cmp    ecx, 0fffff3f8h
        jne    SHORT loc_2_1CF4
        mov    cl, byte ptr [edx]
        and    ecx, 0ffff00ffh
loc_2_1CB5:
        inc    edx
        test   al, 40h
        je     SHORT loc_2_1CDA
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, dword ptr [ebp - 14h]
        jne    SHORT loc_2_1CD0
        mov    ax, word ptr [edx]
        inc    edx
        jmp    SHORT loc_2_1CDD
        db  005h, 000h, 000h, 000h, 000h
loc_2_1CD0:
        mov    al, byte ptr [edx]
        and    eax, 0ffff00ffh
        inc    edx
        jmp    SHORT loc_2_1CDD
loc_2_1CDA:
        sub    ax, ax
loc_2_1CDD:
        xor    edi, edi
        mov    di, word ptr [ebp - 6]
        cmp    edi, dword ptr [ebp - 14h]
        jne    SHORT loc_2_1CF0
        and    si, ax
        or     si, cx
        jmp    SHORT loc_2_1CF4
loc_2_1CF0:
        and    bl, al
        or     bl, cl
loc_2_1CF4:
        cmp    byte ptr [ebp - 1], 0
        je     NEAR PTR loc_2_1C70
        mov    ax, si
        and    eax, 78h
        cmp    eax, 8
        je     SHORT loc_2_1D25
        cmp    eax, 10h
        je     SHORT loc_2_1D30
        cmp    eax, 20h
        je     SHORT loc_2_1D39
        cmp    eax, 40h
        je     SHORT loc_2_1D42
        mov    byte ptr [ebp - 2], 1
        mov    dword ptr [ebp - 20h], 0aa00000ah
        jmp    SHORT loc_2_1D52
loc_2_1D25:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 5ch], 5
        jmp    SHORT loc_2_1D49
        db  08Bh, 0FFh
loc_2_1D30:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 5ch], 9
        jmp    SHORT loc_2_1D49
loc_2_1D39:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 5ch], 0ah
        jmp    SHORT loc_2_1D49
loc_2_1D42:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 5ch], 0bh
loc_2_1D49:
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [eax + 5ch]
        mov    byte ptr [eax + 5dh], cl
loc_2_1D52:
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 025h, 0FDh, 0FDh, 0FFh
        lea    edi, [ebp - 0c0h]
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax]
        test   al, 3
        je     SHORT loc_2_1D81
        mov    dword ptr [ebp - 0b6h], 1
        jmp    SHORT loc_2_1D8F
loc_2_1D81:
        cmp    al, 10h
        jne    SHORT loc_2_1D8F
        mov    dword ptr [ebp - 0b6h], 2
loc_2_1D8F:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 0aah], 1
        mov    al, byte ptr [eax]
        test   al, 3
        je     SHORT loc_2_1DBA
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 41h]
        mov    dword ptr [ebp - 0a6h], eax
        mov    dword ptr [ebp - 0a2h], 10h
        jmp    SHORT loc_2_1DD4
loc_2_1DBA:
        cmp    al, 10h
        jne    SHORT loc_2_1DD4
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 41h]
        mov    dword ptr [ebp - 0a6h], eax
        mov    dword ptr [ebp - 0a2h], 90h
loc_2_1DD4:
        lea    eax, [ebp - 0c0h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [eax + 98h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 9ch
        push   eax
        db  0E8h, 026h, 0FBh, 0FDh, 0FFh
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        je     SHORT loc_2_1E20
        push   0
        push   27eh
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 0E9h, 0FBh, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_2_1E20:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_2_1E70
        push   0
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 08Dh, 0FCh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 0DBh, 0FAh, 0FDh, 0FFh
        push   0
        push   27eh
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 09Fh, 0FBh, 0FDh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_2_1E70:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    byte ptr [eax], 10h
        jne    NEAR PTR loc_2_220C
        mov    ecx, eax
        mov    edx, dword ptr [ecx + 45h]
        add    ecx, 45h
        shl    dx, 0ch
        add    dx, 0c80h
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        cmp    ax, 0a310h
        je     SHORT loc_2_1EE0
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   0aa000004h
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 019h, 0FCh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 067h, 0FAh, 0FDh, 0FFh
        push   0
        push   27eh
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 02Bh, 0FBh, 0FDh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
loc_2_1EE0:
        mov    edx, dword ptr [ecx]
        shl    dx, 0ch
        add    dx, 0c82h
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        cmp    ax, 2042h
        je     SHORT loc_2_1F40
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   0aa000004h
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 0BBh, 0FBh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 009h, 0FAh, 0FDh, 0FFh
        push   0
        push   27eh
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 0CDh, 0FAh, 0FDh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_2_1F40:
        mov    edx, dword ptr [ecx]
        shl    dx, 0ch
        add    dx, 0c84h
        in     al, dx
        mov    byte ptr [ebp - 3], al
        test   al, 1
        jne    SHORT loc_2_1F99
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   0aa00000fh
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 05Fh, 0FBh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 0ADh, 0F9h, 0FDh, 0FFh
        push   0
        push   27eh
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 071h, 0FAh, 0FDh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_1F99:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dx, word ptr [eax + 41h]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        test   al, 20h
        je     SHORT loc_2_1FB6
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 2dh], 8000h
        jmp    SHORT loc_2_1FD8
loc_2_1FB6:
        test   byte ptr [ebp - 8], 8
        mov    eax, dword ptr [ebp - 0ch]
        je     SHORT loc_2_1FD1
        mov    dword ptr [eax + 2dh], 20000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    word ptr [eax + 49h], 8
        jmp    SHORT loc_2_1FD8
loc_2_1FD1:
        mov    dword ptr [eax + 2dh], 10000h
loc_2_1FD8:
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 2dh]
        cmp    eax, 8000h
        je     SHORT loc_2_2000
        cmp    eax, 10000h
        je     NEAR PTR loc_2_20E9
        cmp    eax, 20000h
        je     NEAR PTR loc_2_2192
        jmp    NEAR PTR loc_2_220C
loc_2_2000:
        and    esi, 7
        cmp    esi, 4
        je     SHORT loc_2_2065
        cmp    esi, 5
        je     SHORT loc_2_2086
        cmp    esi, 6
        je     NEAR PTR loc_2_20A7
        cmp    esi, 7
        je     NEAR PTR loc_2_20C8
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   0aa00000ah
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 093h, 0FAh, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 0E1h, 0F8h, 0FDh, 0FFh
        push   0
        push   27eh
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 0A5h, 0F9h, 0FDh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_2065:
        xor    eax, eax
        mov    al, bl
        shl    eax, 18h
        mov    ecx, dword ptr [ebp - 0ch]
        add    eax, 0c8000h
        mov    dword ptr [ecx + 25h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 29h], 8000h
        jmp    NEAR PTR loc_2_220C
loc_2_2086:
        xor    eax, eax
        mov    al, bl
        shl    eax, 18h
        mov    ecx, dword ptr [ebp - 0ch]
        add    eax, 0e8000h
        mov    dword ptr [ecx + 25h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 29h], 8000h
        jmp    NEAR PTR loc_2_220C
loc_2_20A7:
        xor    eax, eax
        mov    al, bl
        shl    eax, 18h
        mov    ecx, dword ptr [ebp - 0ch]
        add    eax, 0d8000h
        mov    dword ptr [ecx + 25h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 29h], 8000h
        jmp    NEAR PTR loc_2_220C
loc_2_20C8:
        xor    eax, eax
        mov    al, bl
        shl    eax, 18h
        mov    ecx, dword ptr [ebp - 0ch]
        add    eax, 0f8000h
        mov    dword ptr [ecx + 25h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 29h], 8000h
        jmp    NEAR PTR loc_2_220C
loc_2_20E9:
        and    esi, 7
        je     SHORT loc_2_2147
        cmp    esi, 1
        je     SHORT loc_2_2158
        cmp    esi, 2
        je     SHORT loc_2_2169
        cmp    esi, 3
        je     NEAR PTR loc_2_2181
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   0aa00000ah
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 0B1h, 0F9h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 0FFh, 0F7h, 0FDh, 0FFh
        push   0
        push   27eh
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 0C3h, 0F8h, 0FDh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_2147:
        xor    eax, eax
        mov    al, bl
        shl    eax, 18h
        add    eax, 0c0000h
        jmp    NEAR PTR loc_2_21FC
loc_2_2158:
        xor    eax, eax
        mov    al, bl
        shl    eax, 18h
        add    eax, 0e0000h
        jmp    NEAR PTR loc_2_21FC
loc_2_2169:
        xor    ecx, ecx
        mov    cl, bl
        shl    ecx, 18h
        mov    eax, dword ptr [ebp - 0ch]
        add    ecx, 0d0000h
        mov    dword ptr [eax + 25h], ecx
        jmp    NEAR PTR loc_2_2202
loc_2_2181:
        xor    eax, eax
        mov    al, bl
        shl    eax, 18h
        add    eax, 0f0000h
        jmp    NEAR PTR loc_2_21FC
loc_2_2192:
        and    esi, 7
        je     SHORT loc_2_21E2
        cmp    esi, 1
        je     SHORT loc_2_21F0
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   0aa00000ah
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 016h, 0F9h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 064h, 0F7h, 0FDh, 0FFh
        push   0
        push   27eh
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 028h, 0F8h, 0FDh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_21E2:
        xor    eax, eax
        mov    al, bl
        shl    eax, 18h
        add    eax, 0c0000h
        jmp    SHORT loc_2_21FC
loc_2_21F0:
        xor    eax, eax
        mov    al, bl
        shl    eax, 18h
        add    eax, 0d0000h
loc_2_21FC:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 25h], eax
loc_2_2202:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 29h], 0
loc_2_220C:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 4bh], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [ebp - 28h]
        mov    byte ptr [eax + 166h], cl
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_2_2270
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        je     SHORT loc_2_225C
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 9ch]
        push   eax
        db  0E8h, 0EAh, 0F6h, 0FDh, 0FFh
        push   0
        push   27eh
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 0AEh, 0F7h, 0FDh, 0FFh
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_225C:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2270:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 60h], 40h
        mov    dword ptr [esi + 78h], 6
        mov    eax, dword ptr [esi + 2dh]
        cmp    eax, 20000h
        je     SHORT loc_2_22F3
        cmp    eax, 10000h
        jne    SHORT loc_2_22C7
        mov    dword ptr [esi + 64h], 80h
        mov    dword ptr [esi + 7ch], 7
        mov    dword ptr [esi + 68h], 100h
        mov    dword ptr [esi + 6ch], 0ah
        mov    dword ptr [esi + 70h], 0ah
        mov    dword ptr [esi + 74h], 4
        jmp    SHORT loc_2_231D
loc_2_22C7:
        mov    dword ptr [esi + 64h], 20h
        mov    dword ptr [esi + 7ch], 5
        mov    dword ptr [esi + 68h], 100h
        mov    dword ptr [esi + 6ch], 0ah
        mov    dword ptr [esi + 70h], 0ah
        mov    dword ptr [esi + 74h], 2
        jmp    SHORT loc_2_231D
loc_2_22F3:
        mov    dword ptr [esi + 68h], 200h
        mov    dword ptr [esi + 6ch], 64h
        mov    dword ptr [esi + 70h], 32h
        mov    dword ptr [esi + 74h], 14h
        mov    dword ptr [esi + 64h], 80h
        mov    dword ptr [esi + 7ch], 7
loc_2_231D:
        push   esi
        call   sub_2_2D70
        or     al, al
        je     NEAR PTR loc_2_26C7
        lea    edi, [esi + 56h]
        cmp    byte ptr [edi], 0
        jne    SHORT loc_2_2374
        cmp    byte ptr [esi + 57h], 0
        jne    SHORT loc_2_2374
        cmp    byte ptr [esi + 58h], 0
        jne    SHORT loc_2_2374
        cmp    byte ptr [esi + 59h], 0
        jne    SHORT loc_2_2374
        cmp    byte ptr [esi + 5ah], 0
        jne    SHORT loc_2_2374
        cmp    byte ptr [esi + 5bh], 0
        jne    SHORT loc_2_2374
        mov    al, byte ptr [esi + 50h]
        mov    byte ptr [edi], al
        mov    al, byte ptr [esi + 51h]
        mov    byte ptr [esi + 57h], al
        mov    al, byte ptr [esi + 52h]
        mov    byte ptr [esi + 58h], al
        mov    al, byte ptr [esi + 53h]
        mov    byte ptr [esi + 59h], al
        mov    al, byte ptr [esi + 54h]
        mov    byte ptr [esi + 5ah], al
        mov    al, byte ptr [esi + 55h]
        mov    byte ptr [esi + 5bh], al
loc_2_2374:
        mov    eax, dword ptr [esi + 2dh]
        push   eax
        mov    eax, dword ptr [esi + 25h]
        push   eax
        lea    ebx, [esi + 21h]
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 032h, 0F7h, 0FDh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_23B8
        push   0
        push   0aa000001h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 01Fh, 0F7h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_23B8:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp - 8], eax
        and    dword ptr [ebp - 8], 0ffff0000h
        or     dword ptr [ebp - 8], 0bffeh
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        mov    word ptr [esi + 17ch], ax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 17eh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 2dh]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 182h], eax
        mov    eax, dword ptr [ebx]
        add    ecx, eax
        mov    dword ptr [esi + 31h], eax
        mov    dword ptr [esi + 35h], ecx
        push   esi
        call   sub_2_2780
        or     al, al
        jne    SHORT loc_2_2430
        push   0
        push   0aa000002h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 0ADh, 0F6h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_2430:
        lea    eax, [esi + 84h]
        mov    dword ptr [esi + 88h], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 8ch]
        mov    dword ptr [esi + 90h], eax
        mov    dword ptr [eax], eax
        db  0C7h, 046h, 019h, 0F0h, 000h, 000h, 000h
        db  0C7h, 046h, 01Dh, 0F0h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 0c9h]
        mov    dword ptr [esi + 0b9h], eax
        mov    dword ptr [esi + 0bdh], eax
        mov    dword ptr [esi + 0c1h], eax
        mov    eax, dword ptr [esi + 60h]
        mov    dword ptr [esi + 0b5h], eax
        shl    eax, 3
        add    eax, dword ptr [esi + 0c9h]
        sub    eax, 8
        mov    dword ptr [esi + 0cdh], eax
        mov    dword ptr [esi + 0d5h], 0
        mov    dword ptr [esi + 0d9h], 0
        mov    dword ptr [esi + 0ddh], 0
        mov    dword ptr [esi + 0e1h], 0
        mov    byte ptr [esi + 0f9h], 1
        mov    byte ptr [esi + 0fah], 0
        mov    dword ptr [esi + 0fch], 0
        mov    dword ptr [esi + 100h], 0
        mov    dword ptr [esi + 108h], 0
        mov    dword ptr [esi + 10ch], 0
        mov    dword ptr [esi + 110h], 0
        mov    dword ptr [esi + 114h], 0
        mov    dword ptr [esi + 118h], 0
        mov    dword ptr [esi + 11ch], 0
        mov    dword ptr [esi + 120h], 0
        mov    dword ptr [esi + 124h], 0
        mov    dword ptr [esi + 128h], 0
        mov    dword ptr [esi + 12ch], 0
        mov    dword ptr [esi + 130h], 0
        mov    byte ptr [esi + 148h], 0
        mov    byte ptr [esi + 149h], 0
        mov    dword ptr [esi + 14ah], 0
        lea    ebx, [esi + 1]
        mov    byte ptr [esi + 167h], 1
        mov    byte ptr [esi + 169h], 0
        mov    dword ptr [esi + 156h], 0
        mov    dword ptr [esi + 15ah], 0
        push   esi
        call   sub_2_1540
        push   1
        push   0
        xor    eax, eax
        mov    al, byte ptr [esi + 5dh]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 5ch]
        push   eax
        mov    eax, dword ptr [esi + 19h]
        push   eax
        push   esi
        db  068h, 020h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 02Bh, 0F4h, 0FDh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_25E1
        push   0
        push   0aa000005h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 0FCh, 0F4h, 0FDh, 0FFh
        add    esp, 0ch
        push   esi
        call   sub_2_0140
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_25E1:
        lea    eax, [esi + 0adh]
        push   eax
        lea    eax, [esi + 94h]
        push   eax
        push   edi
        db  068h, 040h, 011h, 000h, 000h
        db  068h, 0B0h, 010h, 000h, 000h
        xor    eax, eax
        db  068h, 020h, 010h, 000h, 000h
        mov    al, byte ptr [esi + 166h]
        push   eax
        call   sub_2_2FF0
        or     al, al
        jne    SHORT loc_2_2641
        push   0
        push   0aa000002h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 0A4h, 0F4h, 0FDh, 0FFh
        add    esp, 0ch
        push   esi
        call   sub_2_0140
        push   ebx
        db  0E8h, 0B1h, 0F3h, 0FDh, 0FFh
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2641:
        push   esi
        call   sub_2_26F0
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_2_2688
        push   0
        push   0aa000003h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 067h, 0F4h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, dword ptr [esi + 0adh]
        push   eax
        call   sub_2_3110
        push   esi
        call   sub_2_0140
        push   ebx
        db  0E8h, 068h, 0F3h, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2688:
        lea    eax, [esi + 16eh]
        db  0C7h, 086h, 06Ah, 001h, 000h, 000h, 000h, 00Bh, 000h, 000h
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [esi + 16ah]
        mov    dword ptr [esi + 172h], ecx
        mov    dword ptr [esi + 176h], esi
        push   1388h
        push   eax
        db  0E8h, 017h, 0F3h, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_26C7:
        push   0
        push   0aa000004h
        mov    eax, dword ptr [esi + 9ch]
        push   eax
        db  0E8h, 0EEh, 0F3h, 0FDh, 0FFh
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_26F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi], 13h
        je     SHORT loc_2_2724
        mov    byte ptr [esi + 4bh], 0
        mov    dword ptr [ebp - 0ch], esi
        mov    ax, word ptr [esi + 49h]
        or     ax, 2
        mov    word ptr [ebp - 4], ax
        lea    eax, [ebp - 0ch]
        push   eax
        db  068h, 0E0h, 00Ah, 000h, 000h
        lea    eax, [esi + 1]
        push   eax
        db  0E8h, 0C6h, 0F2h, 0FDh, 0FFh
loc_2_2724:
        push   esi
        call   sub_2_13A0
        push   7a120h
        db  0E8h, 0BCh, 0F2h, 0FDh, 0FFh
        cmp    byte ptr [esi + 167h], 0
        jne    SHORT loc_2_2765
        mov    dword ptr [esi + 0a0h], 0
        mov    eax, dword ptr [esi + 1dh]
        mov    dword ptr [esi + 0a4h], eax
        mov    dword ptr [esi + 0a8h], esi
        push   esi
        call   sub_2_00F0
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2765:
        mov    eax, 0c0000001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2780:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 6ch]
        mov    dword ptr [ebp - 0ch], edi
        mov    eax, dword ptr [esi + 70h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [esi + 74h]
        mov    ecx, dword ptr [esi + 31h]
        mov    dword ptr [ebp - 18h], eax
        lea    edx, [ecx + 18h]
        mov    eax, dword ptr [esi + 35h]
        inc    eax
        cmp    eax, edx
        lea    eax, [esi + 0b1h]
        jae    SHORT loc_2_27BA
        mov    dword ptr [eax], 0
        jmp    SHORT loc_2_27BF
loc_2_27BA:
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 31h], edx
loc_2_27BF:
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_27D5
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_27D5:
        mov    eax, dword ptr [esi + 60h]
        shl    eax, 3
        add    eax, 7
        and    eax, 0fffffff8h
        jne    SHORT loc_2_27F1
        lea    eax, [esi + 0c9h]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_2_2815
loc_2_27F1:
        mov    edx, dword ptr [esi + 31h]
        lea    ecx, [eax + edx]
        mov    eax, dword ptr [esi + 35h]
        inc    eax
        cmp    eax, ecx
        lea    eax, [esi + 0c9h]
        jae    SHORT loc_2_2810
        mov    dword ptr [eax], 0
        jmp    SHORT loc_2_2815
        db  08Dh, 049h, 000h
loc_2_2810:
        mov    dword ptr [eax], edx
        mov    dword ptr [esi + 31h], ecx
loc_2_2815:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_2_2830
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_2830:
        sub    ecx, ecx
        cmp    dword ptr [esi + 60h], ecx
        jbe    SHORT loc_2_2847
        sub    edx, edx
loc_2_2839:
        mov    dword ptr [eax], edx
        mov    dword ptr [eax + 4], edx
        inc    ecx
        add    eax, 8
        cmp    dword ptr [esi + 60h], ecx
        ja     SHORT loc_2_2839
loc_2_2847:
        mov    eax, dword ptr [esi + 64h]
        shl    eax, 3
        add    eax, 7
        and    eax, 0fffffff8h
        jne    SHORT loc_2_2863
        lea    ebx, [esi + 0d1h]
        mov    dword ptr [ebx], 0
        jmp    SHORT loc_2_2885
loc_2_2863:
        mov    edx, dword ptr [esi + 31h]
        lea    ecx, [eax + edx]
        mov    eax, dword ptr [esi + 35h]
        inc    eax
        cmp    eax, ecx
        lea    ebx, [esi + 0d1h]
        jae    SHORT loc_2_2880
        mov    dword ptr [ebx], 0
        jmp    SHORT loc_2_2885
        db  090h
loc_2_2880:
        mov    dword ptr [ebx], edx
        mov    dword ptr [esi + 31h], ecx
loc_2_2885:
        cmp    dword ptr [ebx], 0
        jne    SHORT loc_2_28A0
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_2_28A0:
        db  0A1h, 038h, 01Bh, 000h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [esi + 64h]
        shl    eax, 2
        push   eax
        lea    eax, [esi + 104h]
        push   eax
        db  0E8h, 03Bh, 0F1h, 0FDh, 0FFh
        mov    edi, dword ptr [esi + 104h]
        or     edi, edi
        jne    SHORT loc_2_28D6
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_28D6:
        mov    eax, dword ptr [esi + 64h]
        lea    ecx, [eax*4]
        sub    eax, eax
        shr    ecx, 2
        rep stosd
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebx]
        cmp    dword ptr [esi + 64h], eax
        jbe    NEAR PTR loc_2_297D
        sub    edx, edx
loc_2_28F7:
        mov    eax, dword ptr [esi + 68h]
        add    eax, 7
        and    eax, 0fffffff8h
        jne    SHORT loc_2_2911
        mov    eax, dword ptr [esi + 104h]
        mov    dword ptr [edx + eax], 0
        jmp    SHORT loc_2_293C
loc_2_2911:
        mov    edi, dword ptr [esi + 31h]
        add    eax, edi
        mov    ebx, dword ptr [esi + 35h]
        inc    ebx
        cmp    ebx, eax
        jae    SHORT loc_2_2930
        mov    eax, dword ptr [esi + 104h]
        mov    dword ptr [edx + eax], 0
        jmp    SHORT loc_2_293C
        db  08Dh, 049h, 000h
loc_2_2930:
        mov    ebx, dword ptr [esi + 104h]
        mov    dword ptr [edx + ebx], edi
        mov    dword ptr [esi + 31h], eax
loc_2_293C:
        mov    eax, dword ptr [esi + 104h]
        mov    eax, dword ptr [edx + eax]
        or     eax, eax
        je     SHORT loc_2_29B2
        sub    eax, dword ptr [esi + 21h]
        add    eax, dword ptr [esi + 29h]
        mov    word ptr [ecx], ax
        shr    eax, 10h
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [esi + 68h]
        inc    dword ptr [ebp - 4]
        not    ax
        inc    ax
        mov    word ptr [ecx + 4], ax
        mov    byte ptr [ecx + 3], 80h
        add    edx, 4
        add    ecx, 8
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 64h], eax
        ja     NEAR PTR loc_2_28F7
loc_2_297D:
        lea    edi, [esi + 0c5h]
        db  0A1h, 038h, 01Bh, 000h, 000h
        push   eax
        mov    eax, dword ptr [esi + 60h]
        push   0
        lea    eax, [eax + eax*2]
        shl    eax, 2
        push   eax
        push   edi
        db  0E8h, 05Bh, 0F0h, 0FDh, 0FFh
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_2_29C3
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_29B2:
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_29C3:
        mov    edx, dword ptr [esi + 60h]
        sub    eax, eax
        lea    edx, [edx + edx*2]
        shl    edx, 2
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp - 18h]
        add    eax, dword ptr [ebp - 14h]
        add    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 038h, 01Bh, 000h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 4
        push   eax
        lea    eax, [esi + 0f5h]
        push   eax
        db  0E8h, 0F3h, 0EFh, 0FDh, 0FFh
        mov    edi, dword ptr [esi + 0f5h]
        or     edi, edi
        jne    SHORT loc_2_2A20
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_2A20:
        mov    ecx, dword ptr [ebp - 8]
        shl    ecx, 4
        sub    eax, eax
        shr    ecx, 2
        rep stosd
        mov    dword ptr [esi + 0e5h], 0ffffffffh
        mov    dword ptr [esi + 0e9h], eax
        sub    ecx, ecx
        cmp    dword ptr [ebp - 0ch], eax
        je     SHORT loc_2_2A96
        sub    edx, edx
loc_2_2A46:
        mov    edi, dword ptr [esi + 31h]
        mov    ebx, dword ptr [esi + 35h]
        lea    eax, [edi + 40h]
        inc    ebx
        cmp    ebx, eax
        jae    SHORT loc_2_2A63
        mov    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [edx + eax], 0
        jmp    SHORT loc_2_2A6F
loc_2_2A63:
        mov    ebx, dword ptr [esi + 0f5h]
        mov    dword ptr [edx + ebx], edi
        mov    dword ptr [esi + 31h], eax
loc_2_2A6F:
        mov    eax, dword ptr [esi + 0f5h]
        add    eax, edx
        cmp    dword ptr [eax], 0
        je     SHORT loc_2_2AE0
        inc    ecx
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [edx + eax + 8], 40h
        add    edx, 10h
        cmp    dword ptr [ebp - 0ch], ecx
        ja     SHORT loc_2_2A46
loc_2_2A96:
        mov    edx, ecx
        shl    edx, 4
        mov    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [edx + eax - 0ch], 0ffffffffh
        mov    dword ptr [esi + 0edh], ecx
        mov    eax, dword ptr [ebp - 14h]
        add    eax, dword ptr [ebp - 0ch]
        cmp    ecx, eax
        mov    dword ptr [ebp - 10h], eax
        jae    SHORT loc_2_2B24
loc_2_2ABC:
        mov    edi, dword ptr [esi + 31h]
        mov    ebx, dword ptr [esi + 35h]
        lea    eax, [edi + 100h]
        inc    ebx
        cmp    ebx, eax
        jae    SHORT loc_2_2AF1
        mov    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [edx + eax], 0
        jmp    SHORT loc_2_2AFD
        db  08Dh, 064h, 024h, 000h
loc_2_2AE0:
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2AF1:
        mov    ebx, dword ptr [esi + 0f5h]
        mov    dword ptr [edx + ebx], edi
        mov    dword ptr [esi + 31h], eax
loc_2_2AFD:
        mov    eax, dword ptr [esi + 0f5h]
        add    eax, edx
        cmp    dword ptr [eax], 0
        je     SHORT loc_2_2B62
        inc    ecx
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [edx + eax + 8], 100h
        add    edx, 10h
        cmp    ecx, dword ptr [ebp - 10h]
        jb     SHORT loc_2_2ABC
loc_2_2B24:
        mov    edx, ecx
        shl    edx, 4
        mov    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [edx + eax - 0ch], 0ffffffffh
        mov    dword ptr [esi + 0f1h], ecx
        cmp    ecx, dword ptr [ebp - 8]
        jae    SHORT loc_2_2BA6
loc_2_2B42:
        mov    eax, dword ptr [esi + 31h]
        mov    ebx, dword ptr [esi + 35h]
        lea    edi, [eax + 5f0h]
        inc    ebx
        cmp    ebx, edi
        jae    SHORT loc_2_2B73
        mov    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [edx + eax], 0
        jmp    SHORT loc_2_2B7F
loc_2_2B62:
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2B73:
        mov    ebx, dword ptr [esi + 0f5h]
        mov    dword ptr [edx + ebx], eax
        mov    dword ptr [esi + 31h], edi
loc_2_2B7F:
        mov    eax, dword ptr [esi + 0f5h]
        add    eax, edx
        cmp    dword ptr [eax], 0
        je     SHORT loc_2_2BC2
        inc    ecx
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [edx + eax + 8], 5eah
        add    edx, 10h
        cmp    ecx, dword ptr [ebp - 8]
        jb     SHORT loc_2_2B42
loc_2_2BA6:
        shl    ecx, 4
        mov    eax, dword ptr [esi + 0f5h]
        mov    dword ptr [ecx + eax - 0ch], 0ffffffffh
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2BC2:
        push   esi
        call   sub_2_0140
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_2BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        sub    esi, esi
        mov    edx, dword ptr [ebp + 18h]
        cmp    edx, esi
        mov    ecx, dword ptr [ebp + 14h]
        je     SHORT loc_2_2C06
        lea    eax, [ecx + edx*4 - 4]
loc_2_2BF8:
        cmp    dword ptr [eax], 0
        je     SHORT loc_2_2C05
        sub    eax, 4
        dec    edx
        jne    SHORT loc_2_2BF8
        jmp    SHORT loc_2_2C06
loc_2_2C05:
        dec    edx
loc_2_2C06:
        cmp    dword ptr [ecx + edx*4], 0
        je     SHORT loc_2_2C19
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_2_2C19:
        mov    eax, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 20h]
        mov    dword ptr [eax], edx
        inc    dword ptr [edi + 80h]
        db  0A1h, 038h, 01Bh, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   39h
        push   eax
        db  0E8h, 0BCh, 0EDh, 0FDh, 0FFh
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_2_2D03
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], edi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 35h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0eah
        mov    dword ptr [edi + 162h], 0eah
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0adh]
        push   eax
        call   sub_2_3160
        or     al, al
        jne    SHORT loc_2_2CD0
        push   0
        push   39h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 033h, 0EDh, 0FDh, 0FFh
        mov    esi, 0c0000001h
        jmp    SHORT loc_2_2D20
loc_2_2CD0:
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [edi + 84h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edi + 88h]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [edi + 88h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 88h], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 18h]
        jmp    SHORT loc_2_2D20
loc_2_2D03:
        push   0
        push   1
        push   0aa000002h
        mov    eax, dword ptr [edi + 9ch]
        mov    esi, 0c000009ah
        push   eax
        db  0E8h, 0ABh, 0EDh, 0FDh, 0FFh
        add    esp, 10h
loc_2_2D20:
        mov    eax, dword ptr [edi + 80h]
        dec    eax
        mov    dword ptr [edi + 80h], eax
        jne    SHORT loc_2_2D5E
        cmp    byte ptr [edi + 148h], 0
        jne    SHORT loc_2_2D50
        cmp    dword ptr [edi + 0d5h], 0
        jne    SHORT loc_2_2D50
        mov    eax, dword ptr [edi + 8ch]
        sub    eax, edi
        cmp    eax, 8ch
        je     SHORT loc_2_2D5E
loc_2_2D50:
        push   0
        add    edi, 0a0h
        push   edi
        db  0E8h, 074h, 0ECh, 0FDh, 0FFh
loc_2_2D5E:
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_2D70:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 10h], 0ffh
        mov    byte ptr [ebp - 0fh], 0
        sub    esi, esi
        mov    byte ptr [ebp - 0eh], 55h
        mov    byte ptr [ebp - 0dh], 0aah
        mov    byte ptr [ebp - 0ch], 0ffh
        mov    byte ptr [ebp - 0bh], 0
        mov    byte ptr [ebp - 0ah], 55h
        mov    byte ptr [ebp - 9], 0aah
        mov    ecx, dword ptr [ebp + 8]
        mov    edi, edi
loc_2_2DA0:
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp + esi - 8], al
        inc    esi
        cmp    esi, 8
        jb     SHORT loc_2_2DA0
        mov    esi, 8
loc_2_2DB4:
        sub    eax, eax
loc_2_2DB6:
        mov    dl, byte ptr [ebp + eax - 10h]
        cmp    byte ptr [ebp + eax - 8], dl
        jne    SHORT loc_2_2DC8
        inc    eax
        cmp    eax, 8
        jb     SHORT loc_2_2DB6
        jmp    SHORT loc_2_2DCD
loc_2_2DC8:
        mov    eax, 0ah
loc_2_2DCD:
        cmp    eax, 0ah
        jne    SHORT loc_2_2DF0
        sub    eax, eax
loc_2_2DD4:
        mov    dl, byte ptr [ebp + eax - 7]
        mov    byte ptr [ebp + eax - 8], dl
        inc    eax
        cmp    eax, 7
        jb     SHORT loc_2_2DD4
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        inc    esi
        cmp    esi, 28h
        jb     SHORT loc_2_2DB4
loc_2_2DF0:
        cmp    esi, 28h
        jne    SHORT loc_2_2E00
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2E00:
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ecx + 50h], al
        in     al, dx
        mov    byte ptr [ecx + 51h], al
        in     al, dx
        mov    byte ptr [ecx + 52h], al
        in     al, dx
        mov    byte ptr [ecx + 53h], al
        in     al, dx
        mov    byte ptr [ecx + 54h], al
        in     al, dx
        mov    byte ptr [ecx + 55h], al
        mov    al, byte ptr [ecx]
        test   al, 12h
        jne    SHORT loc_2_2E41
        cmp    al, 4
        jne    SHORT loc_2_2E36
        mov    dx, 800h
        sub    al, al
        out    dx, al
        mov    dx, word ptr [ecx + 41h]
        sub    ax, ax
        out    dx, ax
loc_2_2E36:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2E41:
        sub    eax, eax
        sub    edx, edx
loc_2_2E45:
        cmp    byte ptr [ecx + eax + 50h], dl
        jne    SHORT loc_2_2E53
        inc    eax
        cmp    eax, 6
        jb     SHORT loc_2_2E45
        jmp    SHORT loc_2_2E58
loc_2_2E53:
        mov    eax, 0ah
loc_2_2E58:
        cmp    eax, 0ah
        je     SHORT loc_2_2E68
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2E68:
        test   byte ptr [ecx + 50h], 1
        je     SHORT loc_2_2E79
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2E79:
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    al, byte ptr [ebp - 8]
        cmp    byte ptr [ebp - 1], al
        jne    NEAR PTR loc_2_2FD6
        mov    al, byte ptr [ebp - 7]
        cmp    byte ptr [ebp - 2], al
        jne    NEAR PTR loc_2_2FD6
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        sub    eax, eax
loc_2_2EE3:
        mov    dl, byte ptr [ebp + eax - 8]
        cmp    byte ptr [ecx + eax + 50h], dl
        jne    NEAR PTR loc_2_2FA9
        inc    eax
        cmp    eax, 6
        jb     SHORT loc_2_2EE3
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 3], al
        sub    eax, eax
        lea    esp, [esp]
loc_2_2F30:
        mov    dl, byte ptr [ebp + eax - 8]
        cmp    byte ptr [ecx + eax + 50h], dl
        jne    SHORT loc_2_2FB4
        inc    eax
        cmp    eax, 6
        jb     SHORT loc_2_2F30
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    al, byte ptr [ebp - 8]
        cmp    byte ptr [ebp - 2], al
        jne    SHORT loc_2_2FCB
        mov    al, byte ptr [ebp - 7]
        cmp    byte ptr [ebp - 1], al
        jne    SHORT loc_2_2FCB
        sub    edi, edi
loc_2_2F62:
        lea    esi, [ebp + edi - 8]
        mov    dx, word ptr [ecx + 4ch]
        in     al, dx
        mov    byte ptr [esi], al
        cmp    byte ptr [ebp + edi - 10h], al
        jne    SHORT loc_2_2FC0
        inc    edi
        cmp    edi, 8
        jb     SHORT loc_2_2F62
        mov    bl, byte ptr [ecx]
        cmp    bl, 4
        jne    SHORT loc_2_2F90
        mov    dx, 800h
        sub    al, al
        out    dx, al
        mov    dx, word ptr [ecx + 41h]
        sub    ax, ax
        out    dx, ax
loc_2_2F90:
        test   bl, 12h
        je     SHORT loc_2_2F9E
        mov    dx, word ptr [ecx + 41h]
        sub    ax, ax
        out    dx, ax
loc_2_2F9E:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2FA9:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2FB4:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_2FC0:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2FCB:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_2FD6:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2FF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 0D0h, 01Dh, 000h, 000h
        lea    eax, [ebp - 4]
        push   ebx
        push   0
        push   32h
        push   eax
        db  0E8h, 0E9h, 0E9h, 0FDh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_3024
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_3024:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, eax
        jne    SHORT loc_2_303E
        mov    esi, 2
loc_2_303E:
        push   ebx
        push   0
        lea    eax, [esi + esi*2]
        shl    eax, 2
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 0A5h, 0E9h, 0FDh, 0FFh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 4], ecx
        or     eax, eax
        je     SHORT loc_2_3072
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_3110
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_3072:
        push   ebx
        push   0
        lea    eax, [esi*8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 8
        push   eax
        db  0E8h, 06Dh, 0E9h, 0FDh, 0FFh
        or     eax, eax
        mov    eax, dword ptr [ebp - 4]
        je     SHORT loc_2_30A1
        push   eax
        call   sub_2_3110
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_2_30A1:
        mov    dword ptr [eax + 28h], 0ffffffffh
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 10h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [eax + 2ch], edx
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 30h], dl
        add    eax, 2ch
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 14h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 18h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 20h], esi
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_3110:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_2_3130
        mov    ecx, dword ptr [esi + 20h]
        push   0
        lea    ecx, [ecx + ecx*2]
        shl    ecx, 2
        push   ecx
        push   eax
        db  0E8h, 0CCh, 0E8h, 0FDh, 0FFh
loc_2_3130:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_2_3146
        push   0
        mov    ecx, dword ptr [esi + 20h]
        shl    ecx, 3
        push   ecx
        push   eax
        db  0E8h, 0B6h, 0E8h, 0FDh, 0FFh
loc_2_3146:
        push   0
        push   32h
        push   esi
        db  0E8h, 0ACh, 0E8h, 0FDh, 0FFh
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_3160:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 28h], 0
        jne    SHORT loc_2_3179
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_3179:
        db  0A1h, 0D0h, 01Dh, 000h, 000h
        push   eax
        push   0
        push   1ah
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 06Ah, 0E8h, 0FDh, 0FFh
        or     eax, eax
        je     SHORT loc_2_3199
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_3199:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 28h]
        mov    edi, edi
loc_2_31A0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_31B3
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_31A0
        jmp    SHORT loc_2_31C2
loc_2_31B3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 28h], eax
loc_2_31C2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_2_31D8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_31D8:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 19h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Bh, 0FFh
sub_2_3230:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_35D0
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_3259
        cmp    ebx, 103h
        jne    SHORT loc_2_3270
loc_2_3259:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_32F0
        or     eax, eax
        je     SHORT loc_2_3270
        mov    ebx, eax
loc_2_3270:
        or     ebx, ebx
        je     SHORT loc_2_327C
        cmp    ebx, 103h
        jne    SHORT loc_2_32DB
loc_2_327C:
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_2_32D6
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_3292
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_3292:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_32A1
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_32A7
loc_2_32A1:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_2_32A7:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_2_32B6
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 0A2h, 0E6h, 0FDh, 0FFh
loc_2_32B6:
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        db  0E8h, 02Fh, 0E7h, 0FDh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_2_32D6:
        mov    ebx, 0c001000eh
loc_2_32DB:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_32F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 20h]
        lea    ecx, [eax + eax*2]
        add    ecx, ecx
        add    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 20h], ecx
        mov    ecx, dword ptr [esi + 8]
        lea    eax, [ecx + eax*4]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [esi + 24h]
        shl    eax, 2
        push   eax
        push   ecx
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 0D1h, 0E7h, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        sub    edi, edi
        push   eax
        db  0E8h, 0B9h, 0E7h, 0FDh, 0FFh
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_2_3374
        sub    eax, eax
        nop
loc_2_3350:
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 1ch], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        not    ebx
        mov    ecx, dword ptr [ebp - 1ch]
        and    dword ptr [eax + ecx], ebx
        add    eax, 4
        inc    edi
        cmp    dword ptr [esi + 24h], edi
        ja     SHORT loc_2_3350
loc_2_3374:
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [esi + 24h], 0
        jbe    SHORT loc_2_33DF
loc_2_3381:
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + ecx*4], 0
        jne    SHORT loc_2_33D4
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        add    ecx, dword ptr [esi + 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        sub    eax, 6
        push   eax
        lea    eax, [ecx + 6]
        push   eax
        push   ecx
        db  0E8h, 04Eh, 0E7h, 0FDh, 0FFh
        mov    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        add    ecx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 4]
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ecx + 4]
        push   eax
        push   ecx
        db  0E8h, 02Dh, 0E7h, 0FDh, 0FFh
        dec    dword ptr [esi + 24h]
        jmp    SHORT loc_2_33D7
loc_2_33D4:
        inc    dword ptr [ebp - 4]
loc_2_33D7:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 24h], eax
        ja     SHORT loc_2_3381
loc_2_33DF:
        mov    dword ptr [ebp - 10h], 0
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_2_34CA
        mov    edi, dword ptr [ebp + 18h]
loc_2_33F3:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        call   sub_2_37B0
        or     al, al
        je     SHORT loc_2_342A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [ebp + 0ch]
        mov    ebx, 1
        mov    cl, byte ptr [edx + 19h]
        shl    ebx, cl
        mov    ecx, dword ptr [ebp - 18h]
        or     dword ptr [ecx + eax*4], ebx
        jmp    NEAR PTR loc_2_34B8
loc_2_342A:
        mov    ecx, dword ptr [esi + 24h]
        cmp    dword ptr [esi + 20h], ecx
        jbe    NEAR PTR loc_2_3504
        mov    eax, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        push   ecx
        push   eax
        add    eax, 6
        push   eax
        db  0E8h, 0A8h, 0E6h, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [eax + eax*2]
        add    eax, eax
        add    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [eax], ecx
        mov    cl, byte ptr [edi + 4]
        mov    byte ptr [eax + 4], cl
        mov    cl, byte ptr [edi + 5]
        mov    byte ptr [eax + 5], cl
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 2
        add    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 24h]
        sub    ecx, dword ptr [ebp - 4]
        shl    ecx, 2
        push   ecx
        push   eax
        add    eax, 4
        push   eax
        db  0E8h, 070h, 0E6h, 0FDh, 0FFh
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edx + eax*4], 0
        mov    ebx, 1
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 14h], edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx + 19h]
        mov    edx, dword ptr [ebp - 14h]
        shl    ebx, cl
        or     dword ptr [edx + eax*4], ebx
        inc    dword ptr [esi + 24h]
loc_2_34B8:
        add    edi, 6
        inc    dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp + 14h]
        jb     NEAR PTR loc_2_33F3
loc_2_34CA:
        mov    eax, dword ptr [esi + 24h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        sub    edi, edi
        cmp    dword ptr [ebp - 0ch], edi
        je     SHORT loc_2_3555
        mov    edx, dword ptr [ebp - 20h]
        sub    ecx, ecx
        nop
loc_2_34E0:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_355B
        mov    eax, dword ptr [esi + 4]
        add    eax, ecx
        mov    ebx, dword ptr [eax + 2]
        cmp    dword ptr [edx + 2], ebx
        jne    SHORT loc_2_3542
        mov    ax, word ptr [eax]
        cmp    word ptr [edx], ax
        jne    SHORT loc_2_3542
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_2_3549
loc_2_3504:
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 0E2h, 0E5h, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        shl    eax, 2
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        db  0E8h, 0CEh, 0E5h, 0FDh, 0FFh
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 24h], eax
        mov    eax, 0c0010009h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_3542:
        mov    dword ptr [ebp - 8], 1
loc_2_3549:
        add    edx, 6
        add    ecx, 6
        inc    edi
        cmp    edi, dword ptr [ebp - 0ch]
        jb     SHORT loc_2_34E0
loc_2_3555:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_35C0
loc_2_355B:
        mov    al, byte ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edx]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   dword ptr [esi + 14h]
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_35C2
        cmp    edi, 103h
        je     SHORT loc_2_35C2
        mov    eax, dword ptr [ebp - 0ch]
        lea    eax, [eax + eax*2]
        lea    ebx, [eax*2]
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 057h, 0E5h, 0FDh, 0FFh
        push   ebx
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        db  0E8h, 049h, 0E5h, 0FDh, 0FFh
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Bh, 0FFh
loc_2_35C0:
        sub    edi, edi
loc_2_35C2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 049h, 000h
sub_2_35D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        mov    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_360D
        lea    esp, [esp]
loc_2_3600:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 0ch], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_3600
loc_2_360D:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ebx, ecx
        je     SHORT loc_2_3641
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 18h]
        or     eax, eax
        je     SHORT loc_2_3643
        cmp    eax, 103h
        je     SHORT loc_2_3643
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 0ch], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_3641:
        sub    eax, eax
loc_2_3643:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
sub_2_3650:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, eax
        mov    cl, byte ptr [ecx + 19h]
        mov    edx, dword ptr [ebp + 8]
        and    ecx, 0ffh
        mov    edi, dword ptr [edx + 24h]
        cmp    edi, eax
        je     SHORT loc_2_3682
        mov    esi, dword ptr [edx + 8]
        mov    edx, 1
        shl    edx, cl
loc_2_3677:
        test   dword ptr [esi], edx
        je     SHORT loc_2_367C
        inc    eax
loc_2_367C:
        add    esi, 4
        dec    edi
        jne    SHORT loc_2_3677
loc_2_3682:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_3690:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 19h]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebx + 24h], 0
        jbe    SHORT loc_2_370C
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 8], 0
        sub    esi, esi
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [ebp - 10h], eax
        mov    edi, edi
loc_2_36D0:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp - 10h]
        test   dword ptr [ecx + esi], eax
        je     SHORT loc_2_36FA
        cmp    dword ptr [ebp + 14h], 6
        jb     SHORT loc_2_3726
        sub    dword ptr [ebp + 14h], 6
        push   6
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        db  0E8h, 008h, 0E4h, 0FDh, 0FFh
        add    edi, 6
        inc    dword ptr [ebp - 0ch]
loc_2_36FA:
        add    dword ptr [ebp - 8], 6
        add    esi, 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebx + 24h], eax
        ja     SHORT loc_2_36D0
loc_2_370C:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [eax], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_2_3726:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 18h]
        pop    esi
        mov    dword ptr [eax], 0
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_3750:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 24h]
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        cmp    ecx, dword ptr [ebp + 10h]
        jbe    SHORT loc_2_3780
        mov    dword ptr [eax], 0c0000001h
        mov    eax, dword ptr [ebp + 14h]
        pop    ebp
        mov    dword ptr [eax], 0
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_3780:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edx + 24h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [edx + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        push   eax
        mov    eax, dword ptr [edx + 4]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 058h, 0E3h, 0FDh, 0FFh
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_37B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, esi
        shr    edx, 1
        cmp    esi, ecx
        je     NEAR PTR loc_2_3849
        dec    esi
        cmp    esi, edx
        jb     SHORT loc_2_3849
        nop
loc_2_37D0:
        cmp    edx, ecx
        jb     SHORT loc_2_3849
        mov    eax, dword ptr [ebp + 0ch]
        lea    edi, [edx + edx*2]
        lea    edi, [eax + edi*2]
        mov    eax, dword ptr [ebp + 10h]
        mov    ebx, dword ptr [eax + 2]
        mov    eax, dword ptr [edi + 2]
        cmp    ebx, eax
        jae    SHORT loc_2_37F3
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_3827
loc_2_37F3:
        jbe    SHORT loc_2_3800
        mov    dword ptr [ebp - 4], 0ffffffffh
        jmp    SHORT loc_2_3827
        db  08Bh, 0FFh
loc_2_3800:
        mov    eax, dword ptr [ebp + 10h]
        mov    bx, word ptr [eax]
        mov    ax, word ptr [edi]
        cmp    bx, ax
        jae    SHORT loc_2_3817
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_2_3827
loc_2_3817:
        mov    dword ptr [ebp - 4], 0ffffffffh
        ja     SHORT loc_2_3827
        mov    dword ptr [ebp - 4], 0
loc_2_3827:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_3859
        jle    SHORT loc_2_3838
        or     edx, edx
        je     SHORT loc_2_3849
        lea    esi, [edx - 1]
        jmp    SHORT loc_2_383B
loc_2_3838:
        lea    ecx, [edx + 1]
loc_2_383B:
        mov    eax, esi
        sub    eax, ecx
        inc    eax
        shr    eax, 1
        lea    edx, [ecx + eax]
        cmp    esi, edx
        jae    SHORT loc_2_37D0
loc_2_3849:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_3859:
        mov    eax, dword ptr [ebp + 14h]
        pop    edi
        mov    dword ptr [eax], edx
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
