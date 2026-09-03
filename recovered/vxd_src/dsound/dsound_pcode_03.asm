; dsound_pcode_03.asm -- DSOUND object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_3_0000:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0ch]
        push   edi
        push   8
        xor    esi, esi
        mov    edi, dword ptr [ebx + 18h]
        push   esi
        push   esi
        push   esi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0017:
        push   eax
        push   esi
        push   1
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0020:
        add    esp, 20h
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 8007000eh
        mov    dword ptr [edi], eax
        add    edi, 4
        sub    edi, dword ptr [ebx + 18h]
        mov    eax, dword ptr [ebx + 20h]
        mov    dword ptr [eax], edi
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_0045:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edi + 18h]
        je     SHORT loc_3_0067
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0064:
        add    esp, 8
loc_3_0067:
        mov    dword ptr [esi], 0
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_007D:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 8]
        mov    edx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   edx
        call   sub_3_08C1
        add    esp, 0ch
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_00AE:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 8]
        mov    edx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   edx
        call   sub_3_08D5
        add    esp, 0ch
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_00DF:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_3_092A
        add    esp, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_010A:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_3_0960
        add    esp, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_0135:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 10h]
        push   dword ptr [eax]
        call   sub_3_099D
        add    esp, 4
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_015D:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_3_09C0
        add    esp, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_0188:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 20h]
        mov    ebp, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        mov    ecx, dword ptr [ebp]
        add    ebp, 4
        mov    edx, dword ptr [ebp]
        add    ebp, 4
        push   dword ptr [ebp + 10h]
        mov    ebx, dword ptr [ebp]
        add    ebp, 4
        mov    eax, dword ptr [ebp]
        add    ebp, 4
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [ebp]
        add    ebp, 4
        mov    dword ptr [esp + 18h], eax
        mov    ebp, dword ptr [ebp]
        push   ebp
        push   dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 24h]
        push   ebp
        push   ebx
        push   edx
        push   ecx
        call   sub_3_09D1
        add    esp, 1ch
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    ebp
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_3_01F1:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 8]
        mov    edx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   edx
        call   sub_3_09F4
        add    esp, 0ch
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_0222:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 8]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_0248:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 24h]
        mov    ebp, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        mov    ecx, dword ptr [ebp]
        add    ebp, 4
        mov    edx, dword ptr [ebp]
        add    ebp, 4
        mov    ebx, dword ptr [ebp]
        add    ebp, 4
        mov    eax, dword ptr [ebp]
        add    ebp, 4
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [ebp]
        add    ebp, 4
        mov    dword ptr [esp + 18h], eax
        push   dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp]
        add    ebp, 4
        mov    dword ptr [esp + 18h], eax
        mov    ebp, dword ptr [ebp]
        push   ebp
        push   dword ptr [esp + 1ch]
        mov    ebp, dword ptr [esp + 24h]
        mov    eax, dword ptr [ecx]
        push   ebp
        push   dword ptr [esp + 2ch]
        push   ebx
        push   edx
        push   ecx
        call   dword ptr [eax + 0ch]
        pop    ebp
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_3_02BC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        mov    ebx, dword ptr [eax]
        mov    ebp, dword ptr [eax + 10h]
        add    eax, 4
        push   ebp
        mov    ecx, dword ptr [eax]
        add    eax, 4
        mov    edx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   edx
        push   ecx
        mov    eax, dword ptr [ebx]
        push   ebx
        call   dword ptr [eax + 10h]
        pop    ebp
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        ret
loc_3_02FB:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 14h]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_0324:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 18h]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_034D:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 1ch]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_0376:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 20h]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_039F:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 8]
        mov    edx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   edx
        mov    eax, dword ptr [edx]
        call   dword ptr [eax + 24h]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_03CD:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 0ch]
        mov    edx, dword ptr [eax]
        add    eax, 4
        mov    ebx, dword ptr [eax]
        add    eax, 4
        push   dword ptr [eax]
        push   ebx
        push   edx
        mov    eax, dword ptr [edx]
        call   dword ptr [eax + 28h]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        ret
loc_3_0403:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 10h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 2ch]
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_0429:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_043F:
        add    esp, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_0454:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 10h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 8], ecx
        xor    ecx, ecx
        xor    edx, edx
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 014h, 000h, 02Ah, 000h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0496:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0ch]
        push   edi
        push   ebp
        mov    eax, dword ptr [ebx + 10h]
        mov    edi, dword ptr [ebx + 18h]
        mov    esi, dword ptr [eax]
        mov    ebp, esi
        mov    eax, dword ptr [eax + 4]
        and    ebp, 0fffff000h
        push   8
        sub    eax, ebp
        lea    eax, [eax + esi + 0fffh]
        cdq
        and    edx, 0fffh
        add    eax, edx
        sar    eax, 0ch
        push   eax
        push   80060000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_04D3:
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_3_04E5
        test   eax, eax
        je     SHORT loc_3_04E5
        sub    ebp, esi
        add    eax, ebp
        jmp    SHORT loc_3_04E7
loc_3_04E5:
        xor    eax, eax
loc_3_04E7:
        pop    ebp
        mov    dword ptr [edi], eax
        add    edi, 4
        mov    eax, dword ptr [ebx + 20h]
        sub    edi, dword ptr [ebx + 18h]
        mov    dword ptr [eax], edi
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_04FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        mov    ebx, dword ptr [edx]
        add    edx, 4
        mov    eax, dword ptr [edx]
        mov    edx, dword ptr [edx + 4]
        mov    ecx, eax
        and    ecx, 0fffff000h
        sub    edx, ecx
        shr    ecx, 0ch
        lea    eax, [edx + eax + 0fffh]
        cdq
        shr    ebx, 0ch
        and    edx, 0fffh
        add    eax, edx
        sar    eax, 0ch
        mov    edx, eax
        xor    eax, eax
        db  0CDh, 020h, 057h, 001h, 001h, 000h
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 14h], edx
        mov    dword ptr [ebp - 10h], ebx
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], ecx
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 025h, 001h, 001h, 000h
        add    esp, 10h
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_0591:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, edx
        and    ecx, 0fffff000h
        sub    eax, ecx
        push   0
        shr    ecx, 0ch
        lea    eax, [eax + edx + 0fffh]
        cdq
        and    edx, 0fffh
        add    eax, edx
        sar    eax, 0ch
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_05CB:
        add    esp, 0ch
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esi], eax
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_05E6:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, edx
        and    ecx, 0fffff000h
        sub    eax, ecx
        shr    ecx, 0ch
        lea    eax, [eax + edx + 0fffh]
        cdq
        and    edx, 0fffh
        mov    dword ptr [ebp - 4], ecx
        add    eax, edx
        sar    eax, 0ch
        mov    dword ptr [ebp - 8], eax
        db  0A1h, 000h, 000h, 000h, 000h
        shr    eax, 0ch
        mov    dword ptr [ebp - 20h], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 035h, 001h, 001h, 000h
        push   0
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0647:
        add    esp, 0ch
        cmp    eax, 1
        sbb    ebx, ebx
        inc    ebx
        je     SHORT loc_3_06AA
loc_3_0652:
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [ebp - 8]
        test   eax, eax
        je     SHORT loc_3_06AA
        mov    ecx, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 4]
        xor    eax, eax
        db  0CDh, 020h, 057h, 001h, 001h, 000h
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 18h], ecx
        mov    ecx, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 1ch], 1
        mov    dword ptr [ebp - 10h], ecx
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        db  0CDh, 020h, 025h, 001h, 001h, 000h
        add    esp, 10h
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 1
        sbb    ebx, ebx
        inc    ebx
        jne    SHORT loc_3_0652
loc_3_06AA:
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 036h, 001h, 001h, 000h
        mov    dword ptr [esi], ebx
        add    esi, 4
        sub    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_06CC:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        push   0
        mov    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 10h]
        mov    eax, dword ptr [eax]
        and    eax, 0fffff000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06E7:
        add    esp, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        ret
loc_3_06FC:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [ecx]
        add    ecx, 4
        mov    ebx, eax
        mov    edx, dword ptr [ecx]
        and    ebx, 0fffff000h
        sub    edx, ebx
        push   dword ptr [ecx + 4]
        shr    ebx, 0ch
        lea    eax, [edx + eax + 0fffh]
        cdq
        and    edx, 0fffh
        add    eax, edx
        sar    eax, 0ch
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_073A:
        add    esp, 0ch
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        ret
loc_3_0750:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edi + 10h]
        mov    esi, dword ptr [edi + 18h]
        mov    eax, dword ptr [ecx]
        add    ecx, 4
        mov    ebx, eax
        mov    edx, dword ptr [ecx]
        and    ebx, 0fffff000h
        sub    edx, ebx
        push   dword ptr [ecx + 4]
        shr    ebx, 0ch
        lea    eax, [edx + eax + 0fffh]
        cdq
        and    edx, 0fffh
        add    eax, edx
        sar    eax, 0ch
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_078E:
        add    esp, 0ch
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, dword ptr [edi + 20h]
        sub    esi, dword ptr [edi + 18h]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        ret
sub_3_07A4:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        xor    edi, edi
        xor    esi, esi
loc_3_07AD:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, esi
        cmp    dword ptr [ecx + 4], 0
        jne    SHORT loc_3_07CC
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, ebx
        lea    edx, [eax + 1]
        mov    dword ptr [ecx + 8], edx
        jb     SHORT loc_3_07CC
        mov    edi, ecx
        mov    ebx, edx
loc_3_07CC:
        add    esi, 0ch
        cmp    esi, 78h
        jl     SHORT loc_3_07AD
        mov    eax, 0
        test   edi, edi
        je     SHORT loc_3_07E6
        mov    dword ptr [edi + 4], 1
        mov    eax, dword ptr [edi]
loc_3_07E6:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_07EA:
        push   ebx
        mov    edx, 0
        push   esi
        mov    ebx, 0
        push   edi
        push   ebp
        mov    ebp, 0
loc_3_07FD:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, 10h
        mov    edi, dword ptr [esp + 14h]
        mov    esi, dword ptr [eax + ebp]
        repe cmpsb
        je     SHORT loc_3_081D
        add    ebp, 0ch
        inc    edx
        cmp    ebp, 78h
        jl     SHORT loc_3_07FD
        jmp    SHORT loc_3_0828
loc_3_081D:
        imul   edx, edx, 0ch
        db  003h, 015h, 000h, 000h, 000h, 000h
        mov    ebx, edx
loc_3_0828:
        xor    eax, eax
        test   ebx, ebx
        je     SHORT loc_3_0834
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 8], eax
loc_3_0834:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_0839:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0845:
        test   eax, eax
        je     SHORT loc_3_0861
        mov    esi, dword ptr [esp + 8]
loc_3_084D:
        cmp    dword ptr [eax], esi
        je     SHORT loc_3_0861
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_085D:
        test   eax, eax
        jne    SHORT loc_3_084D
loc_3_0861:
        pop    esi
        ret
sub_3_0863:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0870:
        test   eax, eax
        je     SHORT loc_3_0894
loc_3_0874:
        mov    ecx, 10h
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [eax + 4]
        repe cmpsb
        je     SHORT loc_3_0894
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0890:
        test   eax, eax
        jne    SHORT loc_3_0874
loc_3_0894:
        pop    edi
        pop    esi
        ret
sub_3_0897:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 8]
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_08A8:
        push   dword ptr [esi + 4]
        call   sub_3_07EA
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_08BF:
        pop    esi
        ret
sub_3_08C1:
        push   dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax]
        ret
sub_3_08D5:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        test   eax, eax
        jne    SHORT loc_3_08EC
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_08EA:
        jmp    SHORT loc_3_0905
loc_3_08EC:
        push   eax
        call   sub_3_0863
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_0909
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0905:
        test   eax, eax
        jne    SHORT loc_3_0910
loc_3_0909:
        mov    eax, 88780078h
        jmp    SHORT loc_3_0927
loc_3_0910:
        mov    edi, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax + 4]
        movsd
        movsd
        movsd
        movsd
        push   dword ptr [esp + 14h]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0ch]
loc_3_0927:
        pop    edi
        pop    esi
        ret
sub_3_092A:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        jne    SHORT loc_3_093F
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_093D:
        jmp    SHORT loc_3_0948
loc_3_093F:
        push   eax
        call   sub_3_0863
        add    esp, 4
loc_3_0948:
        test   eax, eax
        jne    SHORT loc_3_0953
        mov    eax, 88780078h
        jmp    SHORT loc_3_095F
loc_3_0953:
        push   dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0ch]
loc_3_095F:
        ret
sub_3_0960:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        push   dword ptr [esp + 10h]
        mov    dword ptr [esi], 0
        call   sub_3_0863
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_3_0986
        mov    eax, 88780078h
        jmp    SHORT loc_3_0999
loc_3_0986:
        mov    eax, dword ptr [edi]
        push   eax
        mov    ebx, dword ptr [eax]
        call   dword ptr [ebx + 10h]
        test   eax, eax
        jne    SHORT loc_3_0999
        xor    eax, eax
        inc    dword ptr [edi + 8]
        mov    dword ptr [esi], edi
loc_3_0999:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_099D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_3_09B0
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 14h]
        jmp    SHORT loc_3_09BB
loc_3_09B0:
        push   esi
        call   sub_3_0897
        add    esp, 4
        xor    eax, eax
loc_3_09BB:
        dec    dword ptr [esi + 8]
        pop    esi
        ret
sub_3_09C0:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 18h]
        ret
sub_3_09D1:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax]
        push   dword ptr [ebp + 20h]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        mov    eax, dword ptr [ecx]
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   dword ptr [eax + 1ch]
        pop    ebp
        ret
sub_3_09F4:
        push   dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax]
        push   ecx
        mov    eax, dword ptr [ecx]
        call   dword ptr [eax + 20h]
        ret
DSOUND_Service_01:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A16:
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_0A23
        mov    eax, 8007000eh
        jmp    SHORT loc_3_0A5D
loc_3_0A23:
        mov    edi, esi
        xor    eax, eax
        stosd
        stosd
        stosd
        call   sub_3_07A4
        mov    dword ptr [esi + 4], eax
        test   eax, eax
        jne    SHORT loc_3_0A49
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A42:
        mov    eax, 80004005h
        jmp    SHORT loc_3_0A5D
loc_3_0A49:
        mov    eax, dword ptr [esp + 0ch]
        push   esi
        mov    dword ptr [esi], eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A5B:
        xor    eax, eax
loc_3_0A5D:
        pop    edi
        pop    esi
        ret
DSOUND_Service_02:
        push   dword ptr [esp + 4]
        call   sub_3_0839
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_0A77
        mov    eax, 80070057h
        jmp    SHORT loc_3_0A8B
loc_3_0A77:
        xor    ecx, ecx
        mov    dword ptr [eax], ecx
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_3_0A89
        push   eax
        call   sub_3_0897
        add    esp, 4
loc_3_0A89:
        xor    eax, eax
loc_3_0A8B:
        ret
loc_3_0A8C:
        push   esi
        push   0ch
        push   4
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0A96:
        db  0A3h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_3_0AA3
        xor    eax, eax
        jmp    SHORT loc_3_0AEF
loc_3_0AA3:
        push   1
        xor    esi, esi
        push   78h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0AAE:
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, esi
        jne    SHORT loc_3_0ACF
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0AC5:
        xor    eax, eax
        db  089h, 035h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_3_0AEF
loc_3_0ACF:
        db  0B9h, 000h, 000h, 000h, 000h
        xor    eax, eax
loc_3_0AD6:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [edx + eax], ecx
        add    ecx, 10h
        add    eax, 0ch
        cmp    eax, 78h
        jl     SHORT loc_3_0AD6
        mov    eax, 1
loc_3_0AEF:
        pop    esi
        ret
loc_3_0AF1:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        xor    esi, esi
        test   eax, eax
        je     SHORT loc_3_0B0D
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B04:
        add    esp, 8
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_3_0B0D:
        db  0A1h, 000h, 000h, 000h, 000h
        xor    esi, esi
        test   eax, eax
        je     SHORT loc_3_0B24
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0B1E:
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_3_0B24:
        mov    eax, 1
        pop    esi
        ret
_PTEXT ends
        end
