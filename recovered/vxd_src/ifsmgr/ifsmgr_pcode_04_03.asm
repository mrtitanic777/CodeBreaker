; ifsmgr_pcode_04_03.asm -- IFSMGR object 4 segment _PTEXT  (part 3/5)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_A4FD:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        push   dword ptr [ebp + 10h]
        mov    ax, word ptr [ebx + 18h]
        mov    word ptr [ebp - 2], ax
        push   ebx
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_A70E
        mov    ecx, dword ptr [ebx + 7ch]
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebx + 8ch]
        add    eax, 208h
        mov    dword ptr [ebx + 0ch], eax
        sub    eax, eax
        test   byte ptr [ecx + 1bh], 8
        je     SHORT loc_4_A558
        mov    eax, 1
loc_4_A558:
        mov    ecx, dword ptr [ebp - 0ch]
        push   dword ptr [ecx + 1ch]
        push   eax
        call   sub_4_2047
        add    esp, 8
        mov    ecx, dword ptr [ebp - 0ch]
        test   byte ptr [ecx + 1bh], 8
        je     SHORT loc_4_A57B
        imul   eax, eax, 14h
        db  08Bh, 080h, 0A0h, 056h, 000h, 000h
        jmp    SHORT loc_4_A584
loc_4_A57B:
        shl    eax, 4
        db  08Bh, 080h, 000h, 056h, 000h, 000h
loc_4_A584:
        test   al, 1
        je     SHORT loc_4_A5D0
        push   201h
        push   208h
        db  0E8h, 061h, 0E0h, 000h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    SHORT loc_4_A5AC
        mov    word ptr [ebx + 1ah], 8
        jmp    NEAR PTR loc_4_A70E
loc_4_A5AC:
        mov    esi, dword ptr [ebx + 2ch]
        push   esi
        call   sub_4_14D65
        add    esp, 4
        lea    eax, [eax*2 + 2]
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_4_A5D0:
        mov    eax, dword ptr [ebx + 94h]
        push   dword ptr [ebp + 10h]
        mov    al, byte ptr [eax + 0ah]
        inc    al
        mov    byte ptr [ebx + 89h], al
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 14h], eax
        push   ebx
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_A613
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 7ch], ecx
        cmp    word ptr [ebx + 1ah], -40h
        jne    NEAR PTR loc_4_A6FB
        mov    word ptr [ebx + 1ah], 5
        jmp    NEAR PTR loc_4_A6FB
loc_4_A613:
        mov    esi, dword ptr [ebx + 7ch]
        push   ebx
        db  0E8h, 0E2h, 0A2h, 0FAh, 0FFh
        add    esp, 4
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 7ch], ecx
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebx + 10h], eax
        cmp    esi, ecx
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebx + 24h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebx], eax
        jne    NEAR PTR loc_4_A6F5
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebx + 14h], 0
        push   0
        push   1
        push   -1
        push   ebx
        db  0E8h, 00Bh, 0C5h, 0FAh, 0FFh
        add    esp, 10h
        mov    esi, eax
        push   1
        push   -25h
        push   -1
        push   ebx
        db  0E8h, 0FAh, 0C4h, 0FAh, 0FFh
        add    esp, 10h
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_4_A684
        push   4
        push   -25h
        push   edi
        push   ebx
        db  0E8h, 0E6h, 0C4h, 0FAh, 0FFh
        add    esp, 10h
loc_4_A684:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_A690
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 20h], eax
loc_4_A690:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebx + 18h], ax
        push   0
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   25h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 1ch]
        db  0E8h, 0E5h, 094h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_A6C9
        push   1
        push   25h
        push   -1
        push   ebx
        db  0E8h, 0A9h, 0C4h, 0FAh, 0FFh
        add    esp, 10h
        or     esi, esi
        je     SHORT loc_4_A6DB
        push   3
        jmp    SHORT loc_4_A6CF
loc_4_A6C9:
        or     esi, esi
        je     SHORT loc_4_A6DB
        push   2
loc_4_A6CF:
        push   -1
        push   esi
        push   ebx
        db  0E8h, 08Fh, 0C4h, 0FAh, 0FFh
        add    esp, 10h
loc_4_A6DB:
        or     edi, edi
        je     SHORT loc_4_A6ED
        push   5
        push   25h
        push   edi
        push   ebx
        db  0E8h, 07Dh, 0C4h, 0FAh, 0FFh
        add    esp, 10h
loc_4_A6ED:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebx + 14h], eax
        jmp    SHORT loc_4_A6FB
loc_4_A6F5:
        mov    word ptr [ebx + 1ah], 11h
loc_4_A6FB:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_A70E
        push   0
        push   dword ptr [ebp - 8]
        db  0E8h, 0F5h, 0DEh, 000h, 000h
        add    esp, 8
loc_4_A70E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_A72D
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_A73A
loc_4_A72D:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_A73A:
        test   byte ptr [esi + 8bh], 80h
        mov    ecx, 2
        jne    SHORT loc_4_A750
        mov    dword ptr [esi], 16h
        sub    ecx, ecx
loc_4_A750:
        test   byte ptr [esi + 8bh], 10h
        db  074h, 006h
        mov    eax, dword ptr [esi + 0ach]
        db  066h, 0C7h
IFSMgr_ClearLoopback:
        inc    esi
        sbb    byte ptr [eax], al
        add    byte ptr [ecx + 50h], dl
        push   esi
        call   sub_4_A4FD
        add    esp, 0ch
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_A78B
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_A798
loc_4_A78B:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_A798:
        test   byte ptr [esi + 8bh], 80h
        mov    ecx, 2
        jne    SHORT loc_4_A7AE
        mov    dword ptr [esi], 16h
        sub    ecx, ecx
loc_4_A7AE:
        test   byte ptr [esi + 8bh], 10h
        je     SHORT loc_4_A7BD
        mov    eax, dword ptr [esi + 0ach]
loc_4_A7BD:
        mov    dx, word ptr [esi + 0b0h]
        mov    word ptr [esi + 18h], dx
        push   ecx
        push   eax
        push   esi
        call   sub_4_A4FD
        add    esp, 0ch
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        db  0A1h, 0A8h, 065h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 2]
        mov    dword ptr [ecx], eax
        db  0A1h, 0A0h, 065h, 000h, 000h
        push   2
        movzx  eax, word ptr [eax + 14h]
        db  003h, 005h, 0B0h, 065h, 000h, 000h
        mov    dword ptr [ecx + 14h], eax
        mov    word ptr [ecx + 18h], 0
        db  0A1h, 0A0h, 065h, 000h, 000h
        movzx  eax, word ptr [eax + 16h]
        db  003h, 005h, 0B0h, 065h, 000h, 000h
        push   eax
        push   ecx
        call   sub_4_A4FD
        mov    esp, ebp
        pop    ebp
        ret
sub_4_A81B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        or     edi, edi
        je     SHORT loc_4_A87E
        mov    esi, dword ptr [ebp + 8]
loc_4_A82A:
        mov    dword ptr [esi + 7ch], edi
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 1ch], eax
        push   esi
        db  0E8h, 042h, 0A5h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 7ch]
        cmp    edi, eax
        je     SHORT loc_4_A85B
        db  03Dh, 0DCh, 06Dh, 000h, 000h
        je     SHORT loc_4_A853
        push   esi
        db  0E8h, 0AEh, 0A0h, 0FAh, 0FFh
        add    esp, 4
loc_4_A853:
        db  08Bh, 03Dh, 084h, 06Eh, 000h, 000h
        jmp    SHORT loc_4_A87A
loc_4_A85B:
        push   0
        push   esi
        mov    eax, dword ptr [edi + 0ch]
        push   22h
        push   dword ptr [eax + 10h]
        db  0E8h, 025h, 093h, 0FAh, 0FFh
        add    esp, 10h
        mov    edi, dword ptr [edi + 4]
        push   esi
        db  0E8h, 087h, 0A0h, 0FAh, 0FFh
        add    esp, 4
loc_4_A87A:
        or     edi, edi
        jne    SHORT loc_4_A82A
loc_4_A87E:
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 8bh], 40h
        jne    SHORT loc_4_A8B7
        db  0FFh, 035h, 088h, 06Eh, 000h, 000h
        push   esi
        call   sub_4_A81B
        add    esp, 8
        db  0FFh, 035h, 084h, 06Eh, 000h, 000h
        push   esi
        call   sub_4_A81B
        add    esp, 8
        jmp    NEAR PTR loc_4_A94C
loc_4_A8B7:
        cmp    byte ptr [esi + 0b0h], 2
        jbe    SHORT loc_4_A8CB
        mov    word ptr [esi + 1ah], 1
        jmp    NEAR PTR loc_4_A953
loc_4_A8CB:
        movzx  ebx, byte ptr [esi + 0ach]
        or     ebx, ebx
        jne    SHORT loc_4_A8E1
        mov    eax, dword ptr [esi + 94h]
        movzx  ebx, byte ptr [eax + 0ah]
        inc    ebx
loc_4_A8E1:
        mov    byte ptr [esi + 89h], bl
        db  0F6h, 083h, 08Bh, 000h, 000h, 000h, 001h
        lea    edi, [ebx - 1]
        je     SHORT loc_4_A919
        push   1
        push   edi
        push   esi
        call   sub_4_4336
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_A912
        push   0
        push   edi
        call   sub_4_4114
        add    esp, 8
        mov    word ptr [esi + 1ah], ax
loc_4_A912:
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_4_A953
loc_4_A919:
        db  08Bh, 004h, 09Dh, 0BCh, 065h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        or     eax, eax
        jne    SHORT loc_4_A931
        mov    word ptr [esi + 1ah], 0fh
        jmp    SHORT loc_4_A953
loc_4_A931:
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        movzx  eax, byte ptr [esi + 0b0h]
        push   eax
        push   edi
        call   sub_4_14881
        add    esp, 8
        mov    word ptr [esi + 1ah], ax
loc_4_A94C:
        mov    dword ptr [esi + 7ch], 0
loc_4_A953:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    di, 1
        mov    dword ptr [ebp - 0ch], 40h
        mov    dword ptr [ebp - 18h], 2
        mov    dword ptr [ebp - 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        sub    esi, esi
        mov    al, byte ptr [ebx + 8bh]
        test   al, 40h
        je     SHORT loc_4_A9B6
        mov    dword ptr [ebp - 0ch], 104h
        mov    si, word ptr [ebx + 0b0h]
        mov    di, si
        and    di, 7fffh
        cmp    di, 2
        jbe    SHORT loc_4_A9B0
        mov    word ptr [ebx + 1ah], 1
        jmp    NEAR PTR loc_4_AD5B
loc_4_A9B0:
        and    esi, 8000h
loc_4_A9B6:
        test   al, 4
        je     SHORT loc_4_A9C2
        mov    eax, dword ptr [ebx + 9ch]
        jmp    SHORT loc_4_A9CF
loc_4_A9C2:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_A9CF:
        mov    dword ptr [ebx + 14h], eax
        test   byte ptr [ebx + 8bh], 4
        je     SHORT loc_4_A9E6
        mov    ecx, dword ptr [ebx + 98h]
        mov    dword ptr [ebp - 8], ecx
        jmp    SHORT loc_4_A9F6
loc_4_A9E6:
        push   3800h
        call   sub_4_2154
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
loc_4_A9F6:
        push   ebx
        call   sub_4_15558
        add    esp, 4
        mov    dword ptr [ebx + 7ch], 0
        mov    ecx, eax
        and    ecx, 0fh
        cmp    ecx, 8
        jne    NEAR PTR loc_4_AABB
        movzx  eax, byte ptr [ebx + 89h]
        push   eax
        call   sub_4_B8E5
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_AA3C
        mov    word ptr [ebx + 1ah], 0ffc0h
        push   ebx
        call   sub_4_2E09
        add    esp, 4
        jmp    NEAR PTR loc_4_AD5B
loc_4_AA3C:
        or     esi, esi
        je     SHORT loc_4_AA4A
        movzx  ax, byte ptr [ebx + 89h]
        jmp    SHORT loc_4_AA4D
loc_4_AA4A:
        lea    ecx, [ebx + 2ch]
loc_4_AA4D:
        mov    ecx, dword ptr [ebx + 2ch]
        add    ax, 40h
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebx + 2ch]
        mov    word ptr [eax + 2], 3ah
        mov    eax, dword ptr [ebx + 2ch]
        mov    word ptr [eax + 4], 2fh
        mov    eax, dword ptr [ebx + 0ch]
        mov    edi, dword ptr [ebx + 2ch]
        add    edi, 6
        movzx  ecx, word ptr [eax + 2]
        lea    edx, [eax + ecx]
        movzx  eax, word ptr [edx]
        lea    esi, [edx + 2]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    al, byte ptr [ebx + 8bh]
        and    eax, 3
        push   eax
        push   dword ptr [ebp - 0ch]
        movzx  eax, word ptr [edx]
        add    eax, 4
        push   eax
        push   dword ptr [ebx + 2ch]
        push   dword ptr [ebp - 8]
        call   sub_4_14A00
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], 0
        jmp    NEAR PTR loc_4_AD5B
loc_4_AABB:
        test   eax, 20000000h
        jne    SHORT loc_4_AAC9
        test   eax, 4000000h
        je     SHORT loc_4_AAD3
loc_4_AAC9:
        cmp    di, 1
        je     NEAR PTR loc_4_ABC3
loc_4_AAD3:
        cmp    di, 2
        je     NEAR PTR loc_4_ABC3
        cmp    di, 1
        jne    SHORT loc_4_AAF0
        test   byte ptr [ebx + 8bh], 40h
        jne    NEAR PTR loc_4_ABC3
loc_4_AAF0:
        or     ecx, ecx
        jne    NEAR PTR loc_4_ABC3
        movzx  eax, byte ptr [ebx + 89h]
        mov    dword ptr [ebp - 24h], eax
        push   eax
        call   sub_4_B8E5
        add    esp, 4
        cmp    eax, dword ptr [ebp - 24h]
        je     SHORT loc_4_AB34
        movzx  eax, byte ptr [ebx + 89h]
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 001h
        jne    SHORT loc_4_AB34
        mov    word ptr [ebx + 1ah], 0ffc0h
        push   ebx
        call   sub_4_2E09
        add    esp, 4
        jmp    NEAR PTR loc_4_AD5B
loc_4_AB34:
        movzx  eax, byte ptr [ebx + 89h]
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 001h
        jne    SHORT loc_4_AB51
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        test   byte ptr [eax + ecx + 2eh], 1
        je     SHORT loc_4_ABC3
loc_4_AB51:
        or     esi, esi
        je     SHORT loc_4_AB57
        jmp    SHORT loc_4_AB60
loc_4_AB57:
        push   eax
        call   sub_4_B8E5
        add    esp, 4
loc_4_AB60:
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebx + 0ch]
        cmp    word ptr [eax + 4], 0
        jne    SHORT loc_4_AB88
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        add    al, 40h
        mov    byte ptr [ecx], al
        mov    byte ptr [ecx + 1], 3ah
        mov    byte ptr [ecx + 2], 5ch
        mov    byte ptr [ecx + 3], 0
        jmp    NEAR PTR loc_4_AD5B
loc_4_AB88:
        dec    di
        cmp    di, 1
        sbb    eax, eax
        inc    eax
        push   esi
        push   eax
        push   dword ptr [ebp - 0ch]
        push   0
        push   ebx
        mov    eax, dword ptr [ebp - 8]
        add    eax, 2
        push   eax
        call   sub_4_AD8C
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_4_AD5B
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        add    al, 40h
        mov    byte ptr [ecx], al
        mov    byte ptr [ecx + 1], 3ah
        jmp    NEAR PTR loc_4_AD5B
loc_4_ABC3:
        push   0
        push   ebx
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_ABEB
        cmp    word ptr [ebx + 1ah], -40h
        jne    NEAR PTR loc_4_AD5B
        push   ebx
        call   sub_4_2E09
        add    esp, 4
        jmp    NEAR PTR loc_4_AD5B
loc_4_ABEB:
        mov    eax, dword ptr [ebx + 7ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [ebx + 7ch]
        test   byte ptr [eax + 1bh], 80h
        je     SHORT loc_4_AC0E
        mov    eax, dword ptr [ebx + 94h]
        mov    al, byte ptr [eax + 0ah]
        inc    al
        mov    byte ptr [ebx + 89h], al
loc_4_AC0E:
        mov    al, byte ptr [ebx + 89h]
        cmp    al, 0ffh
        jne    NEAR PTR loc_4_AD01
        mov    word ptr [ebx + 18h], 0
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 20h], eax
        push   201h
        push   dword ptr [ebp - 0ch]
        db  0E8h, 0C3h, 0D9h, 000h, 000h
        add    esp, 8
        mov    dword ptr [ebx + 0ch], eax
        or     eax, eax
        je     NEAR PTR loc_4_ACF9
        push   0
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   27h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 24h]
        db  0E8h, 03Ah, 08Fh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_ACE6
        mov    eax, dword ptr [ebx + 0ch]
        movzx  ecx, word ptr [eax]
        sub    ecx, 4
        sar    ecx, 1
        mov    dword ptr [ebp - 18h], ecx
        mov    edx, ecx
        cmp    edx, dword ptr [ebp - 0ch]
        jge    SHORT loc_4_ACE6
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebx + 0ch], eax
loc_4_AC7F:
        mov    edx, dword ptr [ebp - 18h]
        sub    dword ptr [ebp - 0ch], edx
        mov    eax, edx
        add    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 1ch], eax
        push   esi
        push   1
        movzx  eax, di
        push   dword ptr [ebp - 0ch]
        push   eax
        push   ebx
        push   dword ptr [ebp - 1ch]
        call   sub_4_AD8C
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_4_AD48
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_4_AD3A
        mov    eax, dword ptr [ebp - 1ch]
        mov    cl, byte ptr [eax]
        mov    byte ptr [ebp - 1], cl
        mov    al, byte ptr [ebx + 8bh]
        and    eax, 3
        push   eax
        push   dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 4
        push   eax
        push   dword ptr [ebp - 8]
        call   sub_4_14A26
        add    esp, 10h
        mov    eax, dword ptr [ebp - 1ch]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [eax], cl
        jmp    SHORT loc_4_AD48
loc_4_ACE6:
        push   0
        push   dword ptr [ebx + 0ch]
        db  0E8h, 010h, 0D9h, 000h, 000h
        add    esp, 8
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebx + 0ch], eax
loc_4_ACF9:
        mov    word ptr [ebx + 1ah], 3
        jmp    SHORT loc_4_AD5B
loc_4_AD01:
        or     esi, esi
        movzx  eax, al
        je     SHORT loc_4_AD0A
        jmp    SHORT loc_4_AD13
loc_4_AD0A:
        push   eax
        call   sub_4_B8E5
        add    esp, 4
loc_4_AD13:
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebx + 0ch]
        cmp    word ptr [eax + 4], 0
        jne    NEAR PTR loc_4_AC7F
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        add    al, 40h
        mov    byte ptr [ecx], al
        mov    byte ptr [ecx + 1], 3ah
        mov    word ptr [ecx + 2], 5ch
        jmp    SHORT loc_4_AD5B
loc_4_AD3A:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        add    al, 40h
        mov    byte ptr [ecx], al
        mov    byte ptr [ecx + 1], 3ah
loc_4_AD48:
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_4_AD5B
        push   0
        push   dword ptr [ebp - 14h]
        db  0E8h, 0A8h, 0D8h, 000h, 000h
        add    esp, 8
loc_4_AD5B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_AD62:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        db  08Bh, 004h, 085h, 0BCh, 065h, 000h, 000h
        mov    esi, dword ptr [edx]
        movzx  eax, byte ptr [eax + 0eh]
        or     eax, eax
        jle    SHORT loc_4_AD87
loc_4_AD7D:
        movzx  ecx, word ptr [esi]
        add    esi, ecx
        dec    eax
        or     eax, eax
        jg     SHORT loc_4_AD7D
loc_4_AD87:
        mov    dword ptr [edx], esi
        pop    esi
        pop    ebp
        ret
sub_4_AD8C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebx + 28h]
        mov    cx, word ptr [eax]
        cmp    cx, 5ch
        mov    word ptr [ebp - 2], cx
        je     SHORT loc_4_ADAE
        cmp    cx, 2fh
        jne    SHORT loc_4_ADB5
loc_4_ADAE:
        cmp    word ptr [eax + 2], 0
        je     SHORT loc_4_AE1E
loc_4_ADB5:
        movzx  eax, byte ptr [ebx + 89h]
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 080h
        je     SHORT loc_4_ADE0
        db  08Bh, 00Ch, 085h, 0BCh, 065h, 000h, 000h
        mov    eax, dword ptr [ebx + 0ch]
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx + 0ch], ax
        jne    SHORT loc_4_ADE0
        mov    word ptr [ebp - 2], 0
        jmp    SHORT loc_4_AE27
loc_4_ADE0:
        mov    edi, dword ptr [ebx + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    esi, dword ptr [ebx + 8ch]
        add    esi, 208h
        push   1
        push   ecx
        push   dword ptr [ebx + 14h]
        push   esi
        call   sub_4_14A69
        add    esp, 10h
        mov    dword ptr [ebp - 0ch], eax
        mov    ax, word ptr [eax + esi - 2]
        cmp    ax, 5ch
        je     SHORT loc_4_AE1E
        cmp    ax, 2fh
        jne    SHORT loc_4_AE27
loc_4_AE1E:
        dec    dword ptr [ebp + 14h]
        mov    word ptr [ebp - 2], 5ch
loc_4_AE27:
        mov    ax, word ptr [ebp + 10h]
        or     ax, ax
        jne    SHORT loc_4_AE70
        cmp    word ptr [ebp - 2], 0
        je     SHORT loc_4_AE6B
        cmp    word ptr [ebp - 2], 5ch
        je     SHORT loc_4_AE6B
        cmp    dword ptr [ebp + 18h], 0
        je     SHORT loc_4_AE6B
        mov    eax, dword ptr [ebx + 0ch]
        movzx  ecx, word ptr [eax + 2]
        add    ecx, eax
        mov    eax, ecx
        mov    dword ptr [ebp - 8], ecx
        movzx  eax, word ptr [eax]
        mov    edi, ecx
        add    edi, 2
        mov    esi, dword ptr [ebx + 28h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_4_AE6B:
        mov    eax, dword ptr [ebx + 0ch]
        jmp    SHORT loc_4_AEAD
loc_4_AE70:
        add    al, 2
        mov    byte ptr [ebx + 4], al
        mov    eax, dword ptr [ebx + 8ch]
        add    eax, 208h
        mov    dword ptr [ebx + 10h], eax
        push   0
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 0FAh, 08Ch, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_AEAA
        mov    word ptr [ebx + 1ah], 3
        mov    eax, 3
        jmp    SHORT loc_4_AF03
loc_4_AEAA:
        mov    eax, dword ptr [ebx + 10h]
loc_4_AEAD:
        add    eax, 4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp + 1ch], 0
        je     SHORT loc_4_AED6
        movzx  eax, byte ptr [ebx + 89h]
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 080h
        je     SHORT loc_4_AED6
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_4_AD62
        add    esp, 8
loc_4_AED6:
        mov    eax, dword ptr [ebp - 8]
        cmp    word ptr [eax], 0
        jne    SHORT loc_4_AF0A
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi], 5ch
        mov    byte ptr [esi + 1], 0
loc_4_AEE9:
        cmp    word ptr [ebp - 2], 5ch
        jne    SHORT loc_4_AEFB
        mov    eax, dword ptr [ebp - 0ch]
        mov    cx, word ptr [ebp - 2]
        mov    word ptr [esi + eax], cx
loc_4_AEFB:
        mov    word ptr [ebx + 1ah], 0
        sub    eax, eax
loc_4_AF03:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_4_AF0A:
        mov    al, byte ptr [ebx + 8bh]
        and    eax, 3
        push   eax
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_14A26
        add    esp, 10h
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, dword ptr [ebp + 14h]
        jl     SHORT loc_4_AEE9
        mov    word ptr [ebx + 1ah], 3
        mov    eax, 3
        jmp    SHORT loc_4_AF03
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, byte ptr [esi + 0a8h]
        or     eax, eax
        je     SHORT loc_4_AF64
        cmp    eax, 1
        je     NEAR PTR loc_4_AFEC
        mov    word ptr [esi + 1ah], 1
        jmp    NEAR PTR loc_4_B052
loc_4_AF64:
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_AF75
        mov    eax, dword ptr [esi + 9ch]
IFSMgr_UseAdd:
        jmp    SHORT loc_4_AF82
loc_4_AF75:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_AF82:
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp - 4]
        call   sub_4_11FB5
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_AFE4
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_4_11F7F
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], edx
        push   eax
        call   sub_4_6260
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    word ptr [esi + 0b0h], ax
        mov    ax, word ptr [ebp - 6]
        mov    word ptr [esi + 0ach], ax
        mov    ecx, 186a0h
        mov    eax, dword ptr [ebp - 10h]
        sub    edx, edx
        div    ecx
        test   byte ptr [ebp - 14h], 1
        je     SHORT loc_4_AFDC
        add    al, 64h
loc_4_AFDC:
        mov    byte ptr [esi + 0a9h], al
        jmp    SHORT loc_4_B052
loc_4_AFE4:
        mov    word ptr [esi + 1ah], 0dh
        jmp    SHORT loc_4_B052
loc_4_AFEC:
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_AFFD
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_B00A
loc_4_AFFD:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_B00A:
        mov    dword ptr [ebp - 4], eax
        mov    ax, word ptr [esi + 0b0h]
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [esi + 0ach]
        mov    word ptr [ebp - 6], ax
        push   dword ptr [ebp - 8]
        call   sub_4_642D
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], edx
        movzx  eax, byte ptr [esi + 0a9h]
        imul   eax, eax, 186a0h
        mov    dword ptr [ebp - 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ecx], eax
        adc    dword ptr [ecx + 4], 0
loc_4_B052:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        push   edi
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_2751
        mov    esi, dword ptr [ebp + 8]
        add    esp, 4
        db  0F6h, 086h, 08Ch, 000h, 000h, 000h, 001h
        je     SHORT loc_4_B0F2
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        push   esi
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_4336
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_B0AA
        or     edi, edi
        jne    SHORT loc_4_B09B
        jmp    SHORT loc_4_B0F2
loc_4_B09B:
        push   0
        push   esi
        call   sub_4_4114
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_B0F7
loc_4_B0AA:
        cmp    word ptr [ebp - 0eah], 0
        je     SHORT loc_4_B0BD
        movsx  eax, word ptr [ebp - 0eah]
        jmp    SHORT loc_4_B0F7
loc_4_B0BD:
        db  08Bh, 004h, 0B5h, 0C0h, 065h, 000h, 000h
        mov    esi, dword ptr [eax]
        or     esi, esi
        jne    SHORT loc_4_B0D1
        mov    eax, 0fh
        jmp    SHORT loc_4_B0F7
loc_4_B0D1:
        mov    dword ptr [ebp - 88h], esi
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 098h, 09Ch, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 20h]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        jmp    SHORT loc_4_B0F7
loc_4_B0F2:
        mov    eax, 0ffffffffh
loc_4_B0F7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 034h, 085h, 0C0h, 065h, 000h, 000h
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_4_B15C
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 20h], ecx
        jne    SHORT loc_4_B130
        db  0EBh, 017h
loc_4_B130:
        db  0A1h, 084h, 06Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_B15C
        cmp    dword ptr [eax + 20h], ecx
        db  074h, 009h
        db  08Bh, 040h
IFSMgr_UseDel:
        add    al, 0bh
        sal    byte ptr [ebp - 0ch], 0ebh
        adc    eax, 0ff788589h
        db  0FFh, 0FFh, 08Dh, 085h, 0FCh, 0FEh, 0FFh, 0FFh, 050h, 0E8h, 0A5h, 097h, 0FAh, 0FFh, 083h, 0C4h
        db  004h
loc_4_B15C:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        test   byte ptr [ebx + 8bh], 4
        je     SHORT loc_4_B17E
        mov    eax, dword ptr [ebx + 9ch]
        jmp    SHORT loc_4_B18B
loc_4_B17E:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_B18B:
        mov    dword ptr [ebp - 4], eax
        test   byte ptr [ebx + 8bh], 4
        je     SHORT loc_4_B19F
        mov    eax, dword ptr [ebx + 98h]
        jmp    SHORT loc_4_B1AC
loc_4_B19F:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_B1AC:
        mov    dword ptr [ebp - 8], eax
        mov    al, byte ptr [ebx + 0ach]
        mov    cl, al
        and    ecx, 0fh
        mov    dword ptr [ebp - 0ch], ecx
        shr    al, 4
        cmp    byte ptr [ebx + 0adh], 1
loc_4_B1C7:
        movzx  esi, al
        ja     NEAR PTR loc_4_B31B
        cmp    ecx, 2
        jg     NEAR PTR loc_4_B31B
        cmp    esi, 2
        jg     NEAR PTR loc_4_B31B
        push   ebx
        db  0E8h, 0EAh, 085h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_B321
        cmp    dword ptr [ebp - 0ch], 2
        je     SHORT loc_4_B211
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        db  0B9h, 004h, 001h
IFSMgr_DoDelAllUses:
        add    byte ptr [eax], al
        jmp    SHORT loc_4_B221
loc_4_B211:
        push   dword ptr [ebp - 4]
        call   sub_4_1201E
        add    esp, 4
        mov    ecx, 208h
loc_4_B221:
        cmp    ecx, eax
        jle    NEAR PTR loc_4_B313
        cmp    dword ptr [ebp - 0ch], 2
        je     SHORT loc_4_B247
        push   dword ptr [ebp - 0ch]
        push   eax
        push   dword ptr [ebp - 4]
        push   dword ptr [ebx + 0ch]
        call   sub_4_14A69
        add    esp, 10h
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [ebp - 4], ecx
loc_4_B247:
        push   eax
        push   dword ptr [ebp - 4]
        lea    eax, [ebp - 28h]
        push   eax
        call   sub_4_15041
        add    esp, 0ch
        mov    dword ptr [ebp - 10h], eax
        mov    word ptr [ebp - 12h], 0
        cmp    byte ptr [ebx + 0adh], 0
        jne    SHORT loc_4_B26E
        lea    eax, [ebp - 28h]
        jmp    SHORT loc_4_B283
loc_4_B26E:
        push   0
        lea    eax, [ebp - 28h]
        push   eax
        lea    eax, [ebp - 44h]
        push   eax
        call   sub_4_14AC2
        add    esp, 0ch
        lea    eax, [ebp - 44h]
loc_4_B283:
        mov    dword ptr [ebp - 4], eax
        cmp    esi, 2
        je     SHORT loc_4_B2F7
        cmp    byte ptr [ebx + 0adh], 0
        jne    SHORT loc_4_B2D8
        mov    edi, dword ptr [ebp - 8]
        mov    eax, 20202020h
        stosd
        stosd
        stosw
        stosb
        mov    eax, dword ptr [ebp - 8]
        push   esi
        mov    byte ptr [eax + 0bh], 0
        push   10h
        push   10h
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        call   sub_4_14A00
        add    esp, 14h
        push   esi
        push   6
        push   6
        mov    eax, dword ptr [ebp - 4]
        add    eax, 10h
        push   eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 8
        push   eax
        call   sub_4_14A00
        add    esp, 14h
        jmp    SHORT loc_4_B306
loc_4_B2D8:
        push   esi
        push   0dh
        push   dword ptr [ebp - 4]
        call   sub_4_1201E
        add    esp, 4
        push   eax
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        call   sub_4_14A00
        add    esp, 14h
        jmp    SHORT loc_4_B306
loc_4_B2F7:
        mov    esi, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp - 8]
loc_4_B2FD:
        movsw
        cmp    word ptr [esi - 2], 0
        jne    SHORT loc_4_B2FD
loc_4_B306:
        mov    ax, word ptr [ebp - 10h]
        mov    word ptr [ebx + 0b4h], ax
        jmp    SHORT loc_4_B321
loc_4_B313:
        mov    word ptr [ebx + 1ah], 7bh
        jmp    SHORT loc_4_B321
loc_4_B31B:
        mov    word ptr [ebx + 1ah], 1
loc_4_B321:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_B328:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        movzx  ebx, byte ptr [eax + 89h]
        db  08Bh, 034h, 09Dh, 0BCh, 065h, 000h, 000h
        db  080h, 0A3h, 08Bh, 000h, 000h, 000h, 07Fh
        mov    al, byte ptr [esi + 0bh]
        and    eax, 8
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 4], 0
        mov    word ptr [esi + 0ch], 0
        mov    dword ptr [esi + 10h], 0
        mov    byte ptr [esi + 0fh], 0
        je     SHORT loc_4_B37E
        db  0C6h, 083h, 08Bh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_B387
loc_4_B37E:
        mov    edi, esi
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        stosd
loc_4_B387:
        mov    eax, dword ptr [ebp + 8]
        push   1
        push   dword ptr [eax + 7ch]
        push   0
        push   ebx
        push   eax
        db  0E8h, 039h, 083h, 0FAh, 0FFh
        add    esp, 14h
        push   dword ptr [ebp + 8]
        db  0E8h, 05Bh, 095h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_B3BD
        push   0
        push   0
        push   esi
        push   ebx
        push   dword ptr [ebp + 8]
        db  0E8h, 017h, 083h, 0FAh, 0FFh
        add    esp, 14h
loc_4_B3BD:
        db  083h, 03Dh, 074h, 059h, 000h, 000h, 000h
        jne    SHORT loc_4_B3E4
        push   0
        push   dword ptr [ebp - 8]
        db  0E8h, 030h, 0D2h, 000h, 000h
        add    esp, 8
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_B3E4
        push   0
        push   esi
        db  0E8h, 01Fh, 0D2h, 000h, 000h
        add    esp, 8
loc_4_B3E4:
        dec    ebx
        push   0
        push   ebx
        db  068h, 09Fh, 06Bh, 000h, 000h
        db  0E8h, 0F6h, 0D1h, 000h, 000h
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_B40B
        push   0
        push   ebx
        db  068h, 08Bh, 06Bh, 000h, 000h
        db  0E8h, 0E0h, 0D1h, 000h, 000h
        add    esp, 0ch
loc_4_B40B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        db  068h, 028h, 06Eh, 000h, 000h
        db  0E8h, 074h, 091h, 0FAh, 0FFh
        mov    ebx, dword ptr [ebp + 8]
        add    esp, 4
        movzx  eax, byte ptr [ebx + 0a9h]
        or     eax, eax
        je     SHORT loc_4_B45A
        cmp    eax, 1
        je     NEAR PTR loc_4_B699
        cmp    eax, 2
        je     NEAR PTR loc_4_B74D
        mov    word ptr [ebx + 1ah], 1
        jmp    NEAR PTR loc_4_B7BF
loc_4_B45A:
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_4_B7D3
        add    esp, 8
        mov    esi, eax
        db  08Bh
sub_4_B46A:
        adc    dword ptr [eax + eax - 75ad0000h], 40h
        or     bh, dh
        ror    byte ptr [eax + 8983h], 0
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_4_B87C
        add    esp, 8
        or     esi, esi
        jne    NEAR PTR loc_4_B7B9
        or     eax, eax
        jne    NEAR PTR loc_4_B685
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 14h], eax
        je     NEAR PTR loc_4_B685
        mov    byte ptr [ebx + 4], 2
        push   0
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 0D6h, 086h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_B7BF
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 03Ch, 085h, 0BCh, 065h, 000h, 000h
        mov    dword ptr [ebp - 0ch], edi
        or     edi, edi
        jne    SHORT loc_4_B504
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 001h
        jne    SHORT loc_4_B4E6
        mov    dword ptr [ebp - 10h], 1
loc_4_B4E6:
        push   1
        push   18h
        db  0E8h, 009h, 0D1h, 000h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        jne    SHORT loc_4_B533
        mov    word ptr [ebx + 1ah], 54h
        jmp    NEAR PTR loc_4_B7BF
loc_4_B504:
        mov    edi, dword ptr [ebp - 0ch]
        cmp    dword ptr [edi], 0
        je     SHORT loc_4_B533
        mov    eax, dword ptr [ebp - 4]
        dec    al
        mov    byte ptr [ebx + 89h], al
        mov    esi, dword ptr [ebx + 7ch]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 7ch], eax
        mov    eax, dword ptr [edi]
        cmp    byte ptr [eax + 19h], 1
        jbe    SHORT loc_4_B582
        push   ebx
        db  0E8h, 0EEh, 098h, 0FAh, 0FFh
        add    esp, 4
loc_4_B530:
        mov    dword ptr [ebx + 7ch], esi
loc_4_B533:
        mov    eax, dword ptr [ebx + 0ch]
        push   201h
        movzx  eax, word ptr [eax]
        mov    dword ptr [ebp - 18h], eax
        add    eax, 2
        mov    dword ptr [ebp - 1ch], eax
        push   eax
        db  0E8h, 0ABh, 0D0h, 000h, 000h
        add    esp, 8
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_4_B599
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_4_B577
        push   0
        push   dword ptr [ebp - 0ch]
        db  0E8h, 09Ah, 0D0h, 000h, 000h
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 004h, 085h, 0BCh, 065h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_B577:
        mov    word ptr [ebx + 1ah], 54h
        jmp    NEAR PTR loc_4_B7BF
loc_4_B582:
        push   0
        push   ebx
        call   sub_4_E4EA
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_B530
        mov    dword ptr [ebx + 7ch], esi
        jmp    NEAR PTR loc_4_B7BF
loc_4_B599:
        mov    eax, dword ptr [ebp - 1ch]
        mov    edi, edx
        mov    esi, dword ptr [ebx + 0ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        sub    ecx, ecx
        mov    eax, dword ptr [ebp - 14h]
        add    ax, 40h
        mov    word ptr [edx], ax
        mov    word ptr [edx + 2], 3ah
        lea    eax, [edx + 4]
        mov    dword ptr [ebp - 8], eax
        cmp    word ptr [eax], cx
        je     SHORT loc_4_B5E7
loc_4_B5CC:
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 8]
        movzx  eax, word ptr [eax]
        mov    word ptr [esi], 5ch
        inc    ecx
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    word ptr [eax], 0
        jne    SHORT loc_4_B5CC
loc_4_B5E7:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_4_B5F7
        mov    edi, dword ptr [ebp - 0ch]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        stosd
loc_4_B5F7:
        mov    edi, dword ptr [ebp - 0ch]
        mov    ax, word ptr [ebp - 18h]
        mov    word ptr [edi + 0ch], ax
        mov    dword ptr [edi + 10h], edx
        mov    byte ptr [edi + 0eh], cl
        mov    byte ptr [edi + 0ah], 0
        mov    al, byte ptr [ebp - 14h]
        mov    byte ptr [edi + 0fh], al
        push   ebx
        db  0E8h, 063h, 097h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebx + 7ch]
        mov    dword ptr [edi], eax
        push   0
        push   dword ptr [ebx + 7ch]
        push   edi
        push   dword ptr [ebp - 4]
        push   ebx
        db  0E8h, 0A2h, 080h, 0FAh, 0FFh
        add    esp, 14h
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_4_B648
        or     byte ptr [edi + 0bh], 8
        mov    eax, dword ptr [ebp - 4]
        db  0C6h, 080h, 08Bh, 000h, 000h, 000h, 081h
        jmp    SHORT loc_4_B652
loc_4_B648:
        mov    eax, dword ptr [ebp - 4]
        db  080h, 088h, 08Bh, 000h, 000h, 000h, 080h
loc_4_B652:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_4_B66C
        push   0
        mov    eax, dword ptr [ebp - 4]
        dec    eax
        push   eax
        db  068h, 09Fh, 06Bh, 000h, 000h
        db  0E8h, 07Fh, 0CFh, 000h, 000h
        add    esp, 0ch
loc_4_B66C:
        push   0
        mov    eax, dword ptr [ebp - 4]
        dec    eax
        push   eax
        db  068h, 08Bh, 06Bh, 000h, 000h
        db  0E8h, 06Bh, 0CFh, 000h, 000h
        add    esp, 0ch
        jmp    NEAR PTR loc_4_B7BF
loc_4_B685:
        or     esi, esi
        je     SHORT loc_4_B68E
        jmp    NEAR PTR loc_4_B7B9
loc_4_B68E:
        mov    word ptr [ebx + 1ah], 3
        jmp    NEAR PTR loc_4_B7BF
loc_4_B699:
        movzx  eax, byte ptr [ebx + 0a8h]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 1
        jl     NEAR PTR loc_4_B7B9
        cmp    eax, 20h
        jg     NEAR PTR loc_4_B7B9
        db  08Ah, 080h, 08Bh, 000h, 000h, 000h
        and    al, 81h
        cmp    al, 81h
        jne    NEAR PTR loc_4_B7B9
        mov    eax, dword ptr [ebp - 4]
        dec    eax
        push   eax
        push   ebx
        call   sub_4_501C
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_B6E2
        mov    word ptr [ebx + 1ah], 5
        jmp    NEAR PTR loc_4_B7BF
loc_4_B6E2:
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [ebx + 89h], al
        mov    eax, dword ptr [ebp - 4]
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_4_B7B9
        push   1
        dec    eax
        push   eax
        push   ebx
        call   sub_4_4336
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_B71F
        push   0
        mov    eax, dword ptr [ebp - 4]
        dec    eax
        push   eax
        call   sub_4_4114
        add    esp, 8
        mov    word ptr [ebx + 1ah], ax
loc_4_B71F:
        cmp    word ptr [ebx + 1ah], 0
        jne    NEAR PTR loc_4_B7BF
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 004h, 085h, 0BCh, 065h, 000h, 000h
        push   ebx
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 7ch], eax
        db  0E8h, 03Ch, 096h, 0FAh, 0FFh
        add    esp, 4
        push   ebx
        call   sub_4_B328
        add    esp, 4
        jmp    SHORT loc_4_B7BF
loc_4_B74D:
        movzx  eax, byte ptr [ebx + 0a8h]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 1
        jl     SHORT loc_4_B7B9
        cmp    eax, 1ah
        jg     SHORT loc_4_B7B9
        db  08Ah, 080h, 08Bh, 000h, 000h, 000h
        and    al, 81h
        cmp    al, 81h
        jne    SHORT loc_4_B7B9
        test   byte ptr [ebx + 8bh], 4
        je     SHORT loc_4_B77E
        mov    esi, dword ptr [ebx + 0ach]
        jmp    SHORT loc_4_B78D
loc_4_B77E:
        push   3c14h
        call   sub_4_2154
        add    esp, 4
        mov    esi, eax
loc_4_B78D:
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 00Ch, 085h, 0BCh, 065h, 000h, 000h
        mov    dl, byte ptr [ebx + 8bh]
        movzx  eax, word ptr [ecx + 0ch]
        and    edx, 3
        push   edx
        push   eax
        push   eax
sub_4_B7A7:
        push   dword ptr [ecx + 10h]
        push   esi
        call   sub_4_14A00
        add    esp, 14h
        mov    byte ptr [eax + esi], 0
        jmp    SHORT loc_4_B7BF
loc_4_B7B9:
        mov    word ptr [ebx + 1ah], 0fh
loc_4_B7BF:
        db  068h, 028h, 06Eh, 000h, 000h
        db  0E8h, 055h, 08Eh, 0FAh, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_B7D3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        movzx  ecx, byte ptr [esi + 0a8h]
        cmp    ecx, 1
        mov    dword ptr [edi], ecx
        jl     NEAR PTR loc_4_B873
        cmp    ecx, 20h
        jg     SHORT loc_4_B873
        db  0F6h, 081h, 08Bh, 000h, 000h, 000h, 080h
        jne    SHORT loc_4_B873
        db  08Bh, 004h, 08Dh, 0BCh, 065h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_B815
        mov    eax, dword ptr [eax]
        or     eax, eax
IFSMgr_SetupFailedConnection:
        je     SHORT loc_4_B815
        test   byte ptr [eax + 1bh], 10h
        je     SHORT loc_4_B873
loc_4_B815:
        dec    ecx
        push   ecx
        call   sub_4_120CA
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_B873
        push   dword ptr [edi]
        call   sub_4_D42C
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_B873
        mov    ecx, dword ptr [edi]
        db  08Bh, 004h, 08Dh, 044h, 066h, 000h, 000h
        test   byte ptr [eax + 44h], 40h
        je     SHORT loc_4_B84C
        db  0A1h, 044h, 059h, 000h, 000h
        test   byte ptr [ecx + eax + 2eh], 1
        je     SHORT loc_4_B873
loc_4_B84C:
        db  0A1h, 00Ch, 072h, 000h, 000h
        mov    eax, dword ptr [eax + 10h]
        movzx  eax, word ptr [eax + 14h]
        sub    eax, ecx
        cmp    eax, -1
        je     SHORT loc_4_B873
        dec    ecx
        push   ecx
        push   esi
        call   sub_4_501C
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_4_B878
loc_4_B873:
        mov    eax, 1
loc_4_B878:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_B87C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_B89D
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_4_B8A9
loc_4_B89D:
        mov    eax, dword ptr [ebp + 8]
        movzx  ecx, byte ptr [esi + 89h]
        mov    dword ptr [eax], ecx
loc_4_B8A9:
        mov    eax, dword ptr [eax]
        cmp    eax, 1
        jl     SHORT loc_4_B8DD
        cmp    eax, 20h
        jg     SHORT loc_4_B8DD
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 080h
        jne    SHORT loc_4_B8DD
        dec    eax
        push   eax
        call   sub_4_120CA
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_B8DD
        mov    eax, dword ptr [esi + 7ch]
        or     eax, eax
        je     SHORT loc_4_B8D9
        test   byte ptr [eax + 1bh], 10h
        je     SHORT loc_4_B8DD
loc_4_B8D9:
        sub    eax, eax
        jmp    SHORT loc_4_B8E2
loc_4_B8DD:
        mov    eax, 1
loc_4_B8E2:
        pop    esi
        pop    ebp
        ret
sub_4_B8E5:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [eax*4]
        db  08Ah, 090h, 08Bh, 000h, 000h, 000h
        db  08Bh, 0B1h, 044h, 066h, 000h, 000h
        test   dl, 80h
        je     SHORT loc_4_B910
        db  08Bh, 081h, 0BCh, 065h, 000h, 000h
        movzx  eax, byte ptr [eax + 0fh]
        jmp    SHORT loc_4_B941
loc_4_B910:
        mov    cx, word ptr [esi + 43h]
        test   ch, 40h
        je     SHORT loc_4_B92D
        test   ch, 10h
        je     SHORT loc_4_B92D
        movzx  eax, byte ptr [esi]
        db  0F6h, 080h, 04Bh, 000h, 000h, 000h, 001h
        sub    eax, 40h
        jmp    SHORT loc_4_B93D
loc_4_B92D:
        test   dl, 1
        jne    SHORT loc_4_B941
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        test   byte ptr [eax + ecx + 2eh], 1
loc_4_B93D:
        jne    SHORT loc_4_B941
        sub    eax, eax
loc_4_B941:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 0a8h], 2
        jbe    SHORT loc_4_B960
        mov    word ptr [esi + 1ah], 1
        jmp    NEAR PTR loc_4_B9FB
loc_4_B960:
        mov    al, byte ptr [esi + 8bh]
        and    al, 0fch
        mov    byte ptr [esi + 8bh], al
        mov    cx, word ptr [esi + 0a8h]
        or     cl, al
        or     cl, 40h
        mov    byte ptr [esi + 8bh], cl
        push   0
        push   esi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_B9FB
        mov    eax, dword ptr [esi + 7ch]
        cmp    byte ptr [eax + 30h], 0
        je     SHORT loc_4_B9F5
        push   esi
        db  0E8h, 0D1h, 0AAh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_B9F5
        mov    byte ptr [esi + 4], 9
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   21h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 0ch]
        db  0E8h, 0D4h, 081h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_B9FB
        mov    ax, word ptr [esi + 18h]
        cmp    ax, 0ffffh
        je     SHORT loc_4_B9DF
        mov    word ptr [esi + 0b4h], ax
        mov    word ptr [esi + 0ach], 0
        jmp    SHORT loc_4_B9FB
loc_4_B9DF:
        mov    eax, dword ptr [esi + 24h]
        mov    word ptr [esi + 0b4h], ax
        shr    eax, 10h
        mov    word ptr [esi + 0ach], ax
        jmp    SHORT loc_4_B9FB
loc_4_B9F5:
        mov    word ptr [esi + 1ah], 5
loc_4_B9FB:
        pop    esi
        pop    ebp
        ret
        db  000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        db  0E8h, 0C1h, 07Dh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_BC56
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ecx + 8ch]
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [ebp - 0ch], eax
        lea    ecx, [eax + 15h]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp + 8]
        add    ebx, 208h
        movzx  eax, byte ptr [ecx + 88h]
        cmp    eax, 8fh
        je     NEAR PTR loc_4_BB53
        cmp    eax, 90h
        je     NEAR PTR loc_4_BBAF
        cmp    eax, 91h
        je     NEAR PTR loc_4_BB53
        cmp    eax, 92h
        je     NEAR PTR loc_4_BBAF
loc_4_BA6D:
        mov    ecx, dword ptr [ebp + 8]
        push   0
        mov    dword ptr [ecx + 14h], ebx
        mov    dword ptr [ecx + 28h], 0
        push   ecx
        mov    eax, dword ptr [ecx + 7ch]
        push   26h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 20h]
        db  0E8h, 003h, 081h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_BC56
        mov    ecx, dword ptr [ebp + 8]
        lea    esi, [ebx + 1]
        mov    eax, dword ptr [ecx + 7ch]
        mov    edi, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 3]
        mov    byte ptr [ebx + 13h], al
        inc    edi
        mov    eax, dword ptr [ecx + 7ch]
        mov    ecx, 5
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebx + 14h], al
        rep movsd
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 88h], 12h
        jbe    SHORT loc_4_BAD2
        mov    cl, byte ptr [ebp - 1]
        mov    eax, dword ptr [ebp - 0ch]
        or     cl, 80h
        mov    byte ptr [eax], cl
loc_4_BAD2:
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebx + 15h]
        mov    byte ptr [ecx + 0bh], al
        mov    ax, word ptr [ebx + 16h]
        mov    word ptr [ecx + 16h], ax
        mov    ax, word ptr [ebx + 18h]
        mov    word ptr [ecx + 18h], ax
        mov    word ptr [ecx + 12h], 0
        mov    eax, dword ptr [ebx + 1ah]
        mov    word ptr [ecx + 1ch], ax
        mov    eax, dword ptr [ebx + 1ah]
        shr    eax, 10h
        mov    word ptr [ecx + 1eh], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 28h]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 1ah], ax
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 28h]
        mov    ecx, dword ptr [ebp - 8]
        shr    eax, 10h
        mov    word ptr [ecx + 14h], ax
        mov    edi, ecx
        mov    eax, 20202020h
        stosd
        stosd
        stosw
        stosb
        mov    cl, byte ptr [ebx + 1eh]
        add    ebx, 1eh
        cmp    cl, 2eh
        jne    NEAR PTR loc_4_BC2B
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], 2eh
        cmp    byte ptr [ebx + 1], 0
        je     NEAR PTR loc_4_BC4C
        mov    byte ptr [ecx + 1], 2eh
        jmp    NEAR PTR loc_4_BC4C
loc_4_BB53:
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 0ah
        rep stosd
        stosw
        stosb
        db  0A1h, 0A8h, 065h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [eax + 2]
        cmp    eax, 0d7h
        mov    dword ptr [ecx], eax
        jne    SHORT loc_4_BB7C
        mov    dword ptr [ecx], 1016h
loc_4_BB7C:
        push   2
        push   dword ptr [ebp + 8]
        call   sub_4_912E
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_BC56
        mov    edi, dword ptr [ebp - 0ch]
        add    edi, 0dh
        sub    eax, eax
        stosd
        stosd
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [ecx + 89h]
        mov    byte ptr [ebp - 1], al
        jmp    NEAR PTR loc_4_BA6D
loc_4_BBAF:
        mov    edi, ebx
        mov    esi, dword ptr [ebp - 0ch]
        mov    ecx, 5
        rep movsd
        movsb
        movzx  eax, byte ptr [ebx + 13h]
        cmp    eax, 100h
        jbe    SHORT loc_4_BBD5
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 12h
        jmp    NEAR PTR loc_4_BC56
loc_4_BBD5:
        mov    ecx, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0B0h, 083h, 000h, 000h
        mov    dword ptr [ecx + 7ch], eax
        or     eax, eax
        je     SHORT loc_4_BC19
        mov    cl, byte ptr [eax + 2]
        mov    eax, dword ptr [ebp - 0ch]
        cmp    byte ptr [eax + 14h], cl
        jne    SHORT loc_4_BC19
        push   dword ptr [ebp + 8]
        db  0E8h, 082h, 091h, 0FAh, 0FFh
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 4
        mov    eax, dword ptr [ecx + 7ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 4], 1
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 1], al
        jmp    NEAR PTR loc_4_BA6D
loc_4_BC19:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 7ch], 0
        mov    word ptr [ecx + 1ah], 12h
        jmp    SHORT loc_4_BC56
loc_4_BC2B:
        sub    eax, eax
        cmp    cl, al
        je     SHORT loc_4_BC4C
loc_4_BC31:
        mov    dl, byte ptr [ebx]
        cmp    dl, 2eh
        je     SHORT loc_4_BC41
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [eax + ecx], dl
        inc    eax
        jmp    SHORT loc_4_BC46
loc_4_BC41:
        mov    eax, 8
loc_4_BC46:
        inc    ebx
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_4_BC31
loc_4_BC4C:
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 0b4h], 0
loc_4_BC56:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 8bh]
        test   cl, 40h
        je     NEAR PTR loc_4_BFB0
        mov    dword ptr [ebp - 10h], 0
        movzx  eax, word ptr [esi + 9ch]
        mov    dword ptr [ebp - 0ch], eax
        test   cl, 82h
        je     SHORT loc_4_BCD7
        cmp    byte ptr [esi + 0b5h], 0a2h
        jne    SHORT loc_4_BCA6
        mov    eax, dword ptr [esi + 0b0h]
        mov    dword ptr [esi + 24h], eax
        mov    word ptr [esi + 18h], 40h
loc_4_BCA6:
        sub    edi, edi
        mov    al, byte ptr [esi + 8bh]
        test   al, 10h
        je     SHORT loc_4_BCBA
        mov    ebx, dword ptr [esi + 0ach]
        jmp    SHORT loc_4_BD24
loc_4_BCBA:
        test   al, 4
        je     SHORT loc_4_BCC6
        mov    ebx, dword ptr [esi + 98h]
        jmp    SHORT loc_4_BD24
loc_4_BCC6:
        push   3800h
        call   sub_4_2154
        add    esp, 4
        mov    ebx, eax
        jmp    SHORT loc_4_BD24
loc_4_BCD7:
        test   cl, 10h
        je     SHORT loc_4_BCEB
        mov    edi, dword ptr [esi + 0ach]
        mov    dword ptr [ebp - 0ch], 0
        jmp    SHORT loc_4_BD07
loc_4_BCEB:
        test   cl, 4
        je     SHORT loc_4_BCF8
        mov    edi, dword ptr [esi + 98h]
        jmp    SHORT loc_4_BD07
loc_4_BCF8:
        push   3800h
        call   sub_4_2154
        add    esp, 4
        mov    edi, eax
loc_4_BD07:
        push   esi
        db  0E8h, 0C5h, 07Ah, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_C0C7
        mov    ebx, dword ptr [esi + 8ch]
        add    ebx, 208h
loc_4_BD24:
        cmp    dword ptr [ebp - 0ch], 1
        je     SHORT loc_4_BD3B
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_4_BD3B
        mov    word ptr [esi + 1ah], 1
        jmp    NEAR PTR loc_4_C0C7
loc_4_BD3B:
        cmp    byte ptr [esi + 88h], 4eh
        jne    NEAR PTR loc_4_BE38
        movzx  eax, word ptr [esi + 0b0h]
        mov    dword ptr [esi], eax
        push   1
        push   esi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_BD88
        cmp    word ptr [esi + 1ah], -40h
        jne    NEAR PTR loc_4_C0C7
        mov    dword ptr [ebp - 10h], 1
        mov    word ptr [esi + 1ah], 0
        db  0C7h, 046h, 07Ch, 0A4h, 072h, 000h, 000h
        push   esi
        db  0E8h, 0F6h, 08Fh, 0FAh, 0FFh
        add    esp, 4
loc_4_BD88:
        test   byte ptr [esi + 8bh], 8
        je     SHORT loc_4_BDB5
        mov    eax, dword ptr [esi]
        test   eax, 4000000h
        je     SHORT loc_4_BDAE
        mov    word ptr [esi + 1ah], 2
        push   esi
        db  0E8h, 058h, 08Bh, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_4_C0C7
loc_4_BDAE:
        and    eax, 0efffffffh
        mov    dword ptr [esi], eax
loc_4_BDB5:
        push   esi
        db  0E8h, 0F5h, 086h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_C0C7
        mov    dword ptr [esi + 14h], ebx
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_4_BDDA
        push   esi
        call   sub_4_163BB
        add    esp, 4
        jmp    SHORT loc_4_BE04
loc_4_BDDA:
        db  0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 001h
        je     SHORT loc_4_BDEE
        push   esi
        call   sub_4_16507
        add    esp, 4
        jmp    SHORT loc_4_BE04
loc_4_BDEE:
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   2ch
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 38h]
        db  0E8h, 08Fh, 07Dh, 0FAh, 0FFh
        add    esp, 10h
loc_4_BE04:
        cmp    word ptr [esi + 1ah], 0
        je     SHORT loc_4_BE19
        push   esi
        db  0E8h, 0DBh, 086h, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_4_C0C7
loc_4_BE19:
        mov    eax, dword ptr [esi + 74h]
        mov    word ptr [eax + 20h], 1
        mov    ecx, dword ptr [esi + 74h]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ecx + 0ch], eax
        mov    ax, word ptr [esi + 6]
        mov    word ptr [esi + 0b4h], ax
        jmp    SHORT loc_4_BE76
loc_4_BE38:
        movzx  eax, word ptr [esi + 0a8h]
        push   eax
        push   esi
        db  0E8h, 0C2h, 086h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_BE59
        mov    dword ptr [esi + 7ch], 0
        jmp    NEAR PTR loc_4_C0C7
loc_4_BE59:
        mov    dword ptr [esi + 14h], ebx
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   2
        push   dword ptr [eax]
        db  0E8h, 025h, 07Dh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_C0C7
loc_4_BE76:
        test   byte ptr [esi + 8bh], 80h
        je     SHORT loc_4_BE8D
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [esi + 0b0h], eax
        jmp    NEAR PTR loc_4_C0C7
loc_4_BE8D:
        or     edi, edi
        je     NEAR PTR loc_4_C0C7
        cmp    dword ptr [ebp - 0ch], 1
        jne    NEAR PTR loc_4_BF20
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        lea    eax, [ebx + 4]
        call   sub_4_6445
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [ebp - 6]
        shl    eax, 10h
        movzx  ecx, word ptr [ebp - 8]
        add    eax, ecx
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 8], 0
        push   dword ptr [ebx + 10h]
        push   dword ptr [ebx + 0ch]
        lea    eax, [ebx + 0ch]
        call   sub_4_6445
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [ebp - 6]
        shl    eax, 10h
        movzx  ecx, word ptr [ebp - 8]
        add    eax, ecx
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [ebx + 10h], 0
        push   dword ptr [ebx + 18h]
        push   dword ptr [ebx + 14h]
        lea    eax, [ebx + 14h]
        call   sub_4_6445
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        movzx  eax, word ptr [ebp - 6]
        shl    eax, 10h
        movzx  ecx, word ptr [ebp - 8]
        add    eax, ecx
        mov    dword ptr [ebx + 14h], eax
        mov    dword ptr [ebx + 18h], 0
loc_4_BF20:
        push   2ch
        push   ebx
        push   edi
        call   sub_4_61EB
        add    esp, 0ch
        mov    word ptr [esi + 0b0h], 0
        add    edi, 2ch
        mov    al, byte ptr [esi + 8bh]
        and    eax, 3
        push   eax
        push   103h
        push   208h
        lea    eax, [ebx + 2ch]
        push   eax
        push   edi
        call   sub_4_14A00
        add    esp, 14h
        mov    dword ptr [ebp - 14h], edx
        mov    byte ptr [eax + edi], 0
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_4_BF6E
        or     word ptr [esi + 0b0h], 1
loc_4_BF6E:
        add    edi, 104h
        mov    al, byte ptr [esi + 8bh]
        and    eax, 3
        push   eax
        db  06Ah
sub_4_BF7F:
        or     eax, 0c3811c6ah
        xor    al, 2
        add    byte ptr [eax], al
        push   ebx
        push   edi
        call   sub_4_14A00
        add    esp, 14h
        mov    dword ptr [ebp - 14h], edx
        mov    byte ptr [eax + edi], 0
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_4_C0C7
        or     word ptr [esi + 0b0h], 2
        jmp    NEAR PTR loc_4_C0C7
loc_4_BFB0:
        test   cl, 80h
        jne    SHORT loc_4_BFD8
        mov    eax, dword ptr [esi + 90h]
        test   byte ptr [eax], 20h
        je     SHORT loc_4_BFD8
        mov    eax, dword ptr [esi + 94h]
        push   dword ptr [eax + 1ch]
        push   dword ptr [eax + 18h]
        call   sub_4_21DA
        add    esp, 8
        mov    ebx, eax
        jmp    SHORT loc_4_BFDB
loc_4_BFD8:
        mov    ebx, dword ptr [esi + 10h]
loc_4_BFDB:
        cmp    byte ptr [esi + 88h], 4eh
        jne    SHORT loc_4_C042
        cmp    word ptr [esi + 0b0h], 0d7h
        jne    SHORT loc_4_BFF7
        mov    dword ptr [esi], 1016h
        jmp    SHORT loc_4_C000
loc_4_BFF7:
        movzx  eax, byte ptr [esi + 0b0h]
        mov    dword ptr [esi], eax
loc_4_C000:
        push   2
        push   esi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_C0C7
        mov    eax, dword ptr [esi + 2ch]
        cmp    word ptr [eax + 2], 3ah
        jne    SHORT loc_4_C02F
        cmp    word ptr [eax + 4], 0
        jne    SHORT loc_4_C02F
        mov    word ptr [esi + 1ah], 3
        jmp    NEAR PTR loc_4_C0C7
loc_4_C02F:
        mov    byte ptr [esi + 4], 0
        lea    edi, [ebx + 0dh]
        sub    eax, eax
        stosd
        stosd
        mov    al, byte ptr [esi + 89h]
        jmp    SHORT loc_4_C074
loc_4_C042:
        movzx  eax, byte ptr [ebx + 13h]
        db  08Bh, 004h, 085h, 0B0h, 083h, 000h, 000h
        mov    dword ptr [esi + 7ch], eax
        or     eax, eax
        je     SHORT loc_4_C0BA
        mov    al, byte ptr [eax + 2]
        cmp    byte ptr [ebx + 14h], al
        jne    SHORT loc_4_C0BA
        push   esi
        db  0E8h, 019h, 08Dh, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 7ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    byte ptr [esi + 4], 1
        mov    al, byte ptr [ebx]
loc_4_C074:
        mov    byte ptr [ebp - 1], al
        mov    dword ptr [esi + 14h], ebx
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   26h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 20h]
        db  0E8h, 003h, 07Bh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_C0C7
        mov    eax, dword ptr [esi + 7ch]
        mov    al, byte ptr [eax + 3]
        mov    byte ptr [ebx + 13h], al
        mov    eax, dword ptr [esi + 7ch]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebx + 14h], al
        mov    al, byte ptr [ebp - 1]
        and    al, 1fh
        or     al, 80h
        mov    byte ptr [ebx], al
        mov    word ptr [esi + 0b4h], 0
        jmp    SHORT loc_4_C0C7
loc_4_C0BA:
        mov    dword ptr [esi + 7ch], 0
        mov    word ptr [esi + 1ah], 12h
loc_4_C0C7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 0a8h]
        push   eax
        push   esi
        db  0E8h, 025h, 084h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_C0F3
        mov    dword ptr [esi + 7ch], 0
        jmp    SHORT loc_4_C133
loc_4_C0F3:
        push   esi
        db  0E8h, 0A8h, 070h, 0FAh, 0FFh
        add    esp, 4
        push   2
        push   esi
        db  0E8h, 0F5h, 07Bh, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_C121
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0bh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 8]
        db  0E8h, 072h, 07Ah, 0FAh, 0FFh
        add    esp, 10h
loc_4_C121:
        push   esi
        db  0E8h, 02Ah, 07Ch, 0FAh, 0FFh
        add    esp, 4
        push   esi
        db  0E8h, 067h, 081h, 0FAh, 0FFh
        add    esp, 4
loc_4_C133:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 90h]
        db  0F7h, 000h, 080h, 001h
sub_4_C147:
        add    byte ptr [eax], al
        jne    SHORT loc_4_C156
        mov    word ptr [esi + 1ah], 5
        jmp    NEAR PTR loc_4_C213
loc_4_C156:
        push   0
        push   esi
        call   sub_4_91A0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_C17B
        cmp    word ptr [esi + 1ah], -40h
        jne    NEAR PTR loc_4_C213
        mov    word ptr [esi + 1ah], 5
        jmp    NEAR PTR loc_4_C213
loc_4_C17B:
        mov    eax, dword ptr [esi + 7ch]
        cmp    byte ptr [eax + 30h], 1
        jne    SHORT loc_4_C199
        push   esi
        db  0E8h, 0E5h, 0A2h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_C199
        mov    word ptr [esi + 1ah], 28h
        jmp    SHORT loc_4_C213
loc_4_C199:
        push   esi
        db  0E8h, 011h, 083h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_C213
        mov    eax, dword ptr [esi + 74h]
        push   0
        mov    byte ptr [eax + 1fh], 8
        push   0
        push   -1
        push   esi
        db  0E8h, 0AEh, 0A9h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_C1CB
        push   esi
        db  0E8h, 026h, 083h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_C213
loc_4_C1CB:
        mov    eax, dword ptr [esi + 74h]
        mov    word ptr [eax + 20h], 1
        mov    ecx, dword ptr [esi + 74h]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esi + 7ch]
        cmp    byte ptr [eax + 30h], 1
        jbe    SHORT loc_4_C208
        mov    eax, dword ptr [esi + 0ch]
        push   2
        mov    word ptr [eax], 0
        push   esi
        call   sub_4_122E1
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_C208
        push   esi
        db  0E8h, 0E9h, 082h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_C213
loc_4_C208:
        mov    ax, word ptr [esi + 6]
        mov    word ptr [esi + 0b4h], ax
loc_4_C213:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 0a8h]
        push   eax
        push   esi
        db  0E8h, 0DDh, 082h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_C244
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   3
        push   dword ptr [eax]
        db  0E8h, 04Fh, 079h, 0FAh, 0FFh
        add    esp, 10h
loc_4_C244:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 0a8h]
        push   eax
        push   esi
        db  0E8h, 0ACh, 082h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_C2A3
        push   esi
        db  0E8h, 038h, 06Fh, 0FAh, 0FFh
        add    esp, 4
        push   2
        push   esi
        db  0E8h, 085h, 07Ah, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_C291
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0bh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 8]
        db  0E8h, 002h, 079h, 0FAh, 0FFh
        add    esp, 10h
loc_4_C291:
        push   esi
        db  0E8h, 0BAh, 07Ah, 0FAh, 0FFh
        add    esp, 4
        push   esi
        db  0E8h, 0F7h, 07Fh, 0FAh, 0FFh
        add    esp, 4
loc_4_C2A3:
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Bh, 0C0h, 0C3h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        movzx  eax, byte ptr [eax + 89h]
        ret
sub_4_C2C2:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_15558
        add    esp, 4
        mov    dword ptr [esi + 7ch], 0
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        movzx  eax, word ptr [ebp + 0ch]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C36E
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_C336
        mov    eax, dword ptr [esi + 74h]
        or     eax, eax
        je     SHORT loc_4_C331
        mov    eax, dword ptr [eax + 10h]
        mov    ecx, dword ptr [esi + 84h]
        cmp    dword ptr [eax + 1ch], ecx
        jne    SHORT loc_4_C331
        push   esi
        db  0E8h, 01Bh, 081h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_C31D
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_C336
loc_4_C31D:
        push   esi
        db  0E8h, 0DBh, 085h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 7ch], 0
        sub    eax, eax
        jmp    SHORT loc_4_C336
loc_4_C331:
        mov    eax, 6
loc_4_C336:
        pop    esi
        pop    ebp
        ret
sub_4_C339:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 0a8h]
        push   eax
        push   esi
        call   sub_4_C36E
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_C36B
        push   esi
        db  0E8h, 0D0h, 080h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_4_C36B
        movsx  eax, word ptr [esi + 1ah]
loc_4_C36B:
        pop    esi
        pop    ebp
        ret
sub_4_C36E:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    esi, 200h
        mov    ecx, dword ptr [ebp + 8]
        jl     SHORT loc_4_C38F
        mov    word ptr [ecx + 6], si
        push   ecx
        call   sub_4_27B2
        add    esp, 4
        jmp    SHORT loc_4_C3D9
loc_4_C38F:
        mov    eax, dword ptr [ecx + 80h]
        movzx  edx, word ptr [eax + 32h]
        cmp    edx, esi
        jg     SHORT loc_4_C3AA
        mov    word ptr [ecx + 1ah], 6
        mov    eax, 6
        jmp    SHORT loc_4_C3D9
loc_4_C3AA:
        mov    eax, dword ptr [eax + 34h]
        mov    edx, eax
        and    edx, 0ffff0fffh
        shr    edx, 0ch
        and    eax, 0ffffh
        add    eax, edx
        movzx  eax, byte ptr [esi + eax]
        mov    edx, dword ptr [ecx + 94h]
        mov    word ptr [ecx + 6], ax
        mov    eax, dword ptr [edx + eax*4 + 0d4h]
        mov    dword ptr [ecx + 74h], eax
        sub    eax, eax
loc_4_C3D9:
        pop    esi
        pop    ebp
        ret
sub_4_C3DC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 0d0h]
        shl    eax, 4
        movzx  ecx, word ptr [esi + 98h]
        add    eax, ecx
        jne    SHORT loc_4_C405
        mov    word ptr [esi + 1ah], 6
        mov    eax, 6
        jmp    SHORT loc_4_C43A
loc_4_C405:
        mov    dword ptr [esi + 78h], eax
        mov    cx, word ptr [eax + 1bh]
        mov    word ptr [esi + 6], cx
        movzx  ecx, word ptr [eax + 1bh]
        mov    eax, dword ptr [esi + 94h]
        push   esi
        mov    eax, dword ptr [eax + ecx*4 + 0d4h]
        mov    dword ptr [esi + 74h], eax
        db  0E8h, 001h, 080h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_4_C43A
        movsx  eax, word ptr [esi + 1ah]
loc_4_C43A:
        pop    esi
        pop    ebp
        ret
sub_4_C43D:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi + 0a8h]
        push   eax
        push   esi
        call   sub_4_C36E
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_C496
        mov    ax, word ptr [esi + 6]
        cmp    ax, 200h
        jae    SHORT loc_4_C480
        movzx  eax, ax
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        test   byte ptr [eax + ecx + 2eh], 4
        jne    SHORT loc_4_C480
        mov    word ptr [esi + 1ah], 6
        mov    eax, 6
        jmp    SHORT loc_4_C496
loc_4_C480:
        push   esi
        db  0E8h, 0A5h, 07Fh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_4_C496
        movsx  eax, word ptr [esi + 1ah]
loc_4_C496:
        pop    esi
        pop    ebp
        ret
sub_4_C499:
        push   ebp
        mov    ebp, esp
        push   esi
        push   dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_C36E
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_C4ED
        mov    ax, word ptr [esi + 6]
        cmp    ax, 200h
        jae    SHORT loc_4_C4D7
        movzx  eax, ax
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        test   byte ptr [eax + ecx + 2eh], 4
        jne    SHORT loc_4_C4D7
        mov    word ptr [esi + 1ah], 6
        mov    eax, 6
        jmp    SHORT loc_4_C4ED
loc_4_C4D7:
        push   esi
        db  0E8h, 04Eh, 07Fh, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_4_C4ED
        movsx  eax, word ptr [esi + 1ah]
loc_4_C4ED:
        pop    esi
        pop    ebp
        ret
sub_4_C4F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    edi, dword ptr [edx + 80h]
        mov    eax, dword ptr [edi + 34h]
        mov    ecx, eax
        and    ecx, 0ffff0fffh
        shr    ecx, 0ch
        and    eax, 0ffffh
        lea    esi, [ecx + eax]
        movzx  eax, word ptr [edi + 32h]
        add    eax, esi
        cmp    eax, esi
        jbe    SHORT loc_4_C530
        mov    cx, word ptr [edx + 6]
loc_4_C522:
        movzx  dx, byte ptr [esi]
        cmp    dx, cx
        je     SHORT loc_4_C536
        inc    esi
        cmp    eax, esi
        ja     SHORT loc_4_C522
loc_4_C530:
        sub    eax, eax
loc_4_C532:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_C536:
        mov    eax, 1
        jmp    SHORT loc_4_C532
sub_4_C53D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0B8h, 0E0h, 066h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
loc_4_C54B:
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     SHORT loc_4_C56E
        sub    edi, edi
        mov    ecx, dword ptr [edx + 74h]
loc_4_C556:
        cmp    dword ptr [esi], ecx
        jne    SHORT loc_4_C562
        mov    ebx, dword ptr [esi + 4]
        cmp    dword ptr [edx + 8], ebx
        je     SHORT loc_4_C57F
loc_4_C562:
        add    esi, 8
        inc    edi
        cmp    edi, 0ffh
        jle    SHORT loc_4_C556
loc_4_C56E:
        add    eax, 4
        db  03Dh, 0DCh, 06Ah, 000h, 000h
        jbe    SHORT loc_4_C54B
        sub    eax, eax
loc_4_C57A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_C57F:
        mov    eax, 1
        jmp    SHORT loc_4_C57A
sub_4_C586:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        db  03Bh, 005h, 030h, 06Eh, 000h, 000h
        jne    SHORT loc_4_C59B
        sub    eax, eax
        jmp    SHORT loc_4_C5DB
loc_4_C59B:
        db  0A1h, 02Ch, 06Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_C5DB
        mov    ebx, dword ptr [ebp + 0ch]
loc_4_C5A7:
        mov    esi, dword ptr [eax + 4]
        movzx  ecx, word ptr [ebx]
        movzx  edx, word ptr [esi]
        add    ecx, 2
        lea    edi, [edx + 2]
        cmp    ecx, edi
        jb     SHORT loc_4_C5D5
        add    esi, 4
        lea    edi, [ebx + 4]
        sub    edx, 4
        mov    ecx, edx
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_4_C5D3
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb
loc_4_C5D3:
        je     SHORT loc_4_C5DB
loc_4_C5D5:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_4_C5A7
loc_4_C5DB:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 110h
        push   ebx
        push   esi
        push   edi
        lea    edi, [ebp - 110h]
        sub    eax, eax
        mov    ecx, 41h
        rep stosd
        lea    eax, [ebp - 110h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        lea    eax, [ebp - 110h]
        push   eax
        db  0E8h, 0BCh, 071h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_C627
        mov    eax, 8
        jmp    NEAR PTR loc_4_C75A
loc_4_C627:
        push   1
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 84h]
        add    eax, 410h
        push   eax
        call   sub_4_14A69
        add    esp, 10h
        mov    esi, eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0fch], eax
        lea    eax, [ebp - 110h]
        db  0A3h, 030h, 06Eh, 000h, 000h
        push   eax
        call   sub_4_15558
        add    esp, 4
        db  0C7h, 005h, 030h, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h
        and    al, 0fh
        cmp    al, 2
        jne    NEAR PTR loc_4_C744
        push   dword ptr [ebp - 104h]
        lea    eax, [ebp - 110h]
        push   eax
        call   sub_4_C586
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_C744
        mov    dword ptr [ebp - 0ch], 1
        mov    eax, dword ptr [ebp - 104h]
        movzx  eax, word ptr [eax]
        add    eax, esi
        add    eax, 0eh
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    ebx, dword ptr [ebp - 4]
        or     ebx, ebx
        je     SHORT loc_4_C73D
        mov    edx, ebx
        mov    edi, ebx
        add    edi, 0ch
        mov    dword ptr [edx + 4], edi
        mov    eax, dword ptr [ebp - 104h]
        add    edx, 4
        movzx  eax, word ptr [eax]
        add    eax, 2
        mov    esi, dword ptr [ebp - 104h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [edx]
        push   1
        movzx  ecx, word ptr [eax]
        lea    edx, [eax + ecx + 2]
        mov    dword ptr [ebx + 8], edx
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   dword ptr [ebp + 0ch]
        push   edx
        call   sub_4_14A69
        add    esp, 10h
        db  0A1h, 02Ch, 06Eh, 000h, 000h
        mov    dword ptr [ebx], eax
        db  089h, 01Dh, 02Ch, 06Eh, 000h, 000h
        sub    esi, esi
        jmp    SHORT loc_4_C749
loc_4_C73D:
        mov    esi, 8
        jmp    SHORT loc_4_C749
loc_4_C744:
        mov    esi, 0fdh
loc_4_C749:
        lea    eax, [ebp - 110h]
        push   eax
        db  0E8h, 070h, 071h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, esi
loc_4_C75A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 110h
        push   esi
        push   edi
        lea    edi, [ebp - 110h]
        sub    eax, eax
        mov    ecx, 41h
        rep stosd
        lea    eax, [ebp - 110h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0fch], eax
        lea    eax, [ebp - 110h]
        db  0A3h, 030h, 06Eh, 000h, 000h
        push   eax
        call   sub_4_15558
        add    esp, 4
        db  0C7h, 005h, 030h, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h
        and    al, 0fh
        cmp    al, 2
        jne    SHORT loc_4_C823
        push   dword ptr [ebp - 104h]
        lea    eax, [ebp - 110h]
        push   eax
        call   sub_4_C586
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_C81C
        db  039h, 005h, 02Ch, 06Eh, 000h, 000h
        je     SHORT loc_4_C7EE
        db  08Bh, 00Dh, 02Ch, 06Eh, 000h, 000h
        cmp    dword ptr [ecx], eax
        je     SHORT loc_4_C7E8
loc_4_C7E2:
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [ecx], eax
        jne    SHORT loc_4_C7E2
loc_4_C7E8:
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        jmp    SHORT loc_4_C7F6
loc_4_C7EE:
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 02Ch, 06Eh, 000h, 000h
loc_4_C7F6:
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        sub    esi, esi
        jmp    SHORT loc_4_C828
loc_4_C81C:
        mov    esi, 3
        jmp    SHORT loc_4_C828
loc_4_C823:
        mov    esi, 0fdh
loc_4_C828:
        lea    eax, [ebp - 110h]
        push   eax
        db  0E8h, 091h, 070h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   1
        db  0BBh, 0C0h, 065h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 0DDh, 083h, 0FAh, 0FFh
        add    esp, 8
        movzx  ecx, word ptr [esi + 0a8h]
        mov    edx, 1
        mov    eax, 8
loc_4_C86A:
        mov    edi, dword ptr [ebx]
        or     edi, edi
        je     SHORT loc_4_C880
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     SHORT loc_4_C880
        test   byte ptr [edi + 1bh], al
        je     SHORT loc_4_C880
        or     ecx, ecx
        je     SHORT loc_4_C8D6
        dec    ecx
loc_4_C880:
        add    ebx, 4
        inc    edx
        db  081h, 0FBh, 03Ch, 066h, 000h, 000h
        jbe    SHORT loc_4_C86A
        sub    edx, edx
        db  0B8h, 034h, 06Eh, 000h, 000h
loc_4_C893:
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_C89D
        or     ecx, ecx
        je     SHORT loc_4_C8EE
        dec    ecx
loc_4_C89D:
        add    eax, 4
        inc    edx
        db  03Dh, 058h, 06Eh, 000h, 000h
        jb     SHORT loc_4_C893
        db  0A1h, 088h, 06Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_C8CB
        sub    edx, edx
loc_4_C8B3:
        mov    edi, dword ptr [eax + 4]
        cmp    byte ptr [eax + 18h], dl
        jne    SHORT loc_4_C8C0
        cmp    byte ptr [eax + 19h], dl
        jne    SHORT loc_4_C8C5
loc_4_C8C0:
        cmp    ecx, edx
        je     SHORT loc_4_C907
        dec    ecx
loc_4_C8C5:
        mov    eax, edi
        cmp    edi, edx
        jne    SHORT loc_4_C8B3
loc_4_C8CB:
        mov    word ptr [esi + 1ah], 12h
loc_4_C8D1:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_4_C8D6:
        db  08Bh, 004h, 095h, 0BCh, 065h, 000h, 000h
        push   edx
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        push   esi
        call   sub_4_C958
        add    esp, 8
        jmp    SHORT loc_4_C8D1
loc_4_C8EE:
        db  08Bh, 004h, 095h, 034h, 06Eh, 000h, 000h
        mov    dword ptr [esi + 7ch], eax
        add    edx, 21h
        push   edx
        push   esi
        call   sub_4_C958
        add    esp, 8
        jmp    SHORT loc_4_C8D1
loc_4_C907:
        mov    dword ptr [esi + 7ch], eax
        push   0
        push   esi
        call   sub_4_C958
        add    esp, 8
        jmp    SHORT loc_4_C8D1
sub_4_C917:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        or     ecx, ecx
        jne    SHORT loc_4_C92B
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_4_C954
loc_4_C92B:
        cmp    ecx, 20h
        jg     SHORT loc_4_C942
        mov    eax, dword ptr [ebp + 8]
        add    cl, 40h
        mov    byte ptr [eax], cl
        mov    byte ptr [eax + 1], 3ah
        mov    byte ptr [eax + 2], 0
        jmp    SHORT loc_4_C954
loc_4_C942:
        db  0BEh, 0ACh, 06Eh, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    edi, eax
        movsd
        movsb
        sub    cl, 21h
        add    byte ptr [eax + 3], cl
loc_4_C954:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_C958:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    word ptr [esi + 18h], 1
        db  0E8h, 00Fh, 084h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [esi + 7ch]
        push   0
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 1ch], eax
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   27h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 24h]
        db  0E8h, 005h, 072h, 0FAh, 0FFh
        add    esp, 10h
        cmp    word ptr [esi + 1ah], 0
        jne    NEAR PTR loc_4_CA7A
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_C9AA
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_C9B7
loc_4_C9AA:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_C9B7:
        push   1
        push   80h
        mov    ecx, dword ptr [esi + 0ch]
        add    ecx, 4
        push   ecx
        push   eax
        call   sub_4_14A26
        add    esp, 10h
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_C9DF
        mov    eax, dword ptr [esi + 9ch]
        jmp    SHORT loc_4_C9EC
loc_4_C9DF:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_C9EC:
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        push   eax
        call   sub_4_C917
        add    esp, 8
        mov    al, byte ptr [esi + 18h]
        mov    byte ptr [esi + 0a9h], al
        or     edi, edi
        jne    SHORT loc_4_CA21
        mov    eax, dword ptr [esi + 7ch]
        mov    ax, word ptr [eax + 14h]
        mov    word ptr [esi + 0b0h], ax
        mov    al, 5
        sub    al, byte ptr [esi + 4]
        mov    byte ptr [esi + 0a8h], al
        jmp    SHORT loc_4_CA51
loc_4_CA21:
        cmp    edi, 20h
        jle    SHORT loc_4_CA38
        mov    word ptr [esi + 0b0h], 0
        mov    byte ptr [esi + 0a8h], 3
        jmp    SHORT loc_4_CA51
loc_4_CA38:
        db  08Bh, 004h, 0BDh, 0BCh, 065h, 000h, 000h
        mov    ax, word ptr [eax + 8]
        mov    word ptr [esi + 0b0h], ax
        mov    byte ptr [esi + 0a8h], 4
loc_4_CA51:
        cmp    word ptr [esi + 0b4h], 5f05h
        jne    SHORT loc_4_CA7A
        mov    eax, dword ptr [esi + 38h]
        mov    word ptr [esi + 0b4h], ax
        mov    eax, dword ptr [esi + 34h]
        mov    word ptr [esi + 0ach], ax
        mov    eax, dword ptr [esi + 3ch]
        mov    word ptr [esi + 0a0h], ax
loc_4_CA7A:
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, 1
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 09Dh, 081h, 0FAh, 0FFh
        add    esp, 8
        cmp    word ptr [esi + 0a8h], bx
        jbe    SHORT loc_4_CAAD
        mov    word ptr [esi + 1ah], 7ch
        jmp    NEAR PTR loc_4_CC16
loc_4_CAAD:
        mov    word ptr [esi + 0b0h], 0
        test   byte ptr [esi + 8bh], 4
        mov    word ptr [esi + 0ach], 0
        je     SHORT loc_4_CAD0
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_CADD
loc_4_CAD0:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_CADD:
        mov    dword ptr [esi + 14h], eax
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_CAF1
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_CB04
loc_4_CAF1:
        movzx  eax, word ptr [esi + 0d0h]
        shl    eax, 10h
        movzx  ecx, word ptr [esi + 98h]
        or     eax, ecx
loc_4_CB04:
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [ebp - 4], 1
        db  0BFh, 0C0h, 065h, 000h, 000h
loc_4_CB13:
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_4_CB5D
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_4_CB5D
        test   byte ptr [ecx + 1bh], 8
        je     SHORT loc_4_CB5D
        mov    eax, dword ptr [esi + 84h]
        cmp    eax, -1
        je     SHORT loc_4_CB35
        cmp    dword ptr [ecx + 1ch], eax
        jne    SHORT loc_4_CB5D
loc_4_CB35:
        inc    word ptr [esi + 0ach]
        or     ebx, ebx
        je     SHORT loc_4_CB5D
        mov    eax, dword ptr [edi]
        push   dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        movzx  eax, word ptr [esi + 0a8h]
        push   eax
        push   esi
        call   sub_4_CD00
        add    esp, 0ch
        mov    ebx, eax
loc_4_CB5D:
        add    edi, 4
        inc    dword ptr [ebp - 4]
        db  081h, 0FFh, 03Ch, 066h, 000h, 000h
        jbe    SHORT loc_4_CB13
        sub    edi, edi
loc_4_CB6D:
        db  08Dh, 00Ch, 0BDh, 034h, 06Eh, 000h, 000h
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_4_CBB1
        mov    edx, dword ptr [esi + 84h]
        cmp    edx, -1
        je     SHORT loc_4_CB8A
        cmp    dword ptr [eax + 1ch], edx
        jne    SHORT loc_4_CBB1
loc_4_CB8A:
        inc    word ptr [esi + 0ach]
        or     ebx, ebx
        je     SHORT loc_4_CBB1
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 7ch], eax
        lea    eax, [edi + 21h]
        push   eax
        movzx  eax, word ptr [esi + 0a8h]
        push   eax
        push   esi
        call   sub_4_CD00
        add    esp, 0ch
        mov    ebx, eax
loc_4_CBB1:
        inc    edi
        cmp    edi, 9
        jl     SHORT loc_4_CB6D
        db  0A1h, 088h, 06Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_CC08
loc_4_CBC0:
        mov    edi, dword ptr [eax + 4]
        cmp    byte ptr [eax + 18h], 0
        jne    SHORT loc_4_CBCF
        cmp    byte ptr [eax + 19h], 0
        jne    SHORT loc_4_CC02
loc_4_CBCF:
        mov    ecx, dword ptr [esi + 84h]
        cmp    ecx, -1
        je     SHORT loc_4_CBDF
        cmp    dword ptr [eax + 1ch], ecx
        jne    SHORT loc_4_CC02
loc_4_CBDF:
        inc    word ptr [esi + 0ach]
        or     ebx, ebx
        je     SHORT loc_4_CC02
        mov    dword ptr [esi + 7ch], eax
        push   0
        movzx  eax, word ptr [esi + 0a8h]
        push   eax
        push   esi
        call   sub_4_CD00
        add    esp, 0ch
        mov    ebx, eax
loc_4_CC02:
        mov    eax, edi
        or     edi, edi
        jne    SHORT loc_4_CBC0
loc_4_CC08:
        cmp    word ptr [esi + 1ah], 84bh
        jne    SHORT loc_4_CC16
        mov    word ptr [esi + 1ah], 0eah
loc_4_CC16:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        test   byte ptr [ebx + 8bh], 4
        je     SHORT loc_4_CC3B
        mov    eax, dword ptr [ebx + 0ach]
        mov    dword ptr [ebx + 14h], eax
loc_4_CC3B:
        lea    edi, [ebp - 0ch]
        mov    esi, dword ptr [ebx + 14h]
        movsd
        movsd
        movsd
        cmp    word ptr [ebp - 8], 2
        jle    SHORT loc_4_CC56
        mov    word ptr [ebx + 1ah], 7ch
        jmp    NEAR PTR loc_4_CCF9
loc_4_CC56:
        test   byte ptr [ebx + 8bh], 4
        je     SHORT loc_4_CC64
        mov    eax, dword ptr [ebp - 0ch]
        jmp    SHORT loc_4_CC6F
loc_4_CC64:
        push   dword ptr [ebp - 0ch]
        call   sub_4_21A2
        add    esp, 4
loc_4_CC6F:
        mov    dword ptr [ebx + 14h], eax
        test   byte ptr [ebx + 8bh], 4
        je     SHORT loc_4_CC80
        mov    esi, dword ptr [ebp - 6]
        jmp    SHORT loc_4_CC8D
loc_4_CC80:
        push   dword ptr [ebp - 6]
        call   sub_4_21A2
        add    esp, 4
        mov    esi, eax
loc_4_CC8D:
        push   ebx
        call   sub_4_D18A
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_CCB6
        mov    ecx, dword ptr [ebx + 7ch]
        or     ecx, ecx
        je     SHORT loc_4_CCA7
        test   byte ptr [ecx + 1bh], 8
        jne    SHORT loc_4_CCB6
loc_4_CCA7:
        mov    word ptr [ebx + 1ah], 8cah
        mov    dword ptr [ebx + 7ch], 0
        jmp    SHORT loc_4_CCF9
loc_4_CCB6:
        mov    dword ptr [ebx + 14h], esi
        mov    ecx, dword ptr [ebp - 6]
        mov    dword ptr [ebx + 10h], ecx
        movzx  ecx, word ptr [ebp - 2]
        mov    dword ptr [ebx], ecx
        push   eax
        movsx  eax, word ptr [ebp - 8]
        push   eax
        push   ebx
        call   sub_4_CD00
        add    esp, 0ch
        mov    ax, word ptr [ebx + 1ah]
        or     ax, ax
        je     SHORT loc_4_CCEB
        cmp    ax, 3
        jne    SHORT loc_4_CCF9
        mov    word ptr [ebx + 1ah], 8cah
        jmp    SHORT loc_4_CCF9
loc_4_CCEB:
        mov    ax, word ptr [ebp - 2]
        sub    ax, word ptr [ebx]
        mov    word ptr [ebx + 0ach], ax
loc_4_CCF9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_CD00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 7ch]
        or     edi, edi
        jne    SHORT loc_4_CD4C
        push   0
        push   10h
        push   esi
        call   sub_4_D820
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_CD2B
        sub    eax, eax
        jmp    NEAR PTR loc_4_CE50
loc_4_CD2B:
        mov    edi, dword ptr [esi + 7ch]
        cmp    byte ptr [edi + 18h], 0
        jne    SHORT loc_4_CD41
        mov    word ptr [esi + 1ah], 8cah
        sub    eax, eax
        jmp    NEAR PTR loc_4_CE50
loc_4_CD41:
        mov    eax, dword ptr [esi + 8ch]
        mov    dword ptr [esi + 0ch], eax
        jmp    SHORT loc_4_CD55
loc_4_CD4C:
        push   esi
        db  0E8h, 029h, 080h, 0FAh, 0FFh
        add    esp, 4
loc_4_CD55:
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 1ch], eax
        mov    word ptr [esi + 18h], 1
        mov    eax, dword ptr [esi + 84h]
        cmp    eax, -1
        je     SHORT loc_4_CD8E
        cmp    dword ptr [edi + 1ch], eax
        je     SHORT loc_4_CD8E
        push   esi
        db  0E8h, 087h, 07Bh, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [esi + 1ah], 8cah
        mov    dword ptr [esi + 7ch], 0
        sub    eax, eax
        jmp    NEAR PTR loc_4_CE50
loc_4_CD8E:
        push   0
        push   esi
        mov    eax, dword ptr [edi + 0ch]
        push   27h
        push   dword ptr [eax + 24h]
        db  0E8h, 0F2h, 06Dh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_CDBC
        push   esi
        db  0E8h, 053h, 07Bh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 7ch], 0
        sub    eax, eax
        jmp    NEAR PTR loc_4_CE50
loc_4_CDBC:
        movzx  bx, byte ptr [esi + 4]
        dec    bx
        movzx  cx, byte ptr [edi + 19h]
        movzx  ax, byte ptr [edi + 18h]
        add    ax, cx
        mov    word ptr [ebp - 2], ax
        mov    di, word ptr [edi + 16h]
        push   esi
        db  0E8h, 020h, 07Bh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 7ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        push   10h
        push   0
        push   0
        push   0
        lea    ecx, [eax*8]
        push   1
        movzx  eax, word ptr [ebp - 2]
        push   eax
        movzx  eax, di
        push   eax
        movzx  eax, bx
        push   eax
        movzx  eax, word ptr [esi + 18h]
        push   eax
        push   0
        push   dword ptr [esi + 0ch]
        push   9
        push   dword ptr [ebp + 10h]
        db  0FFh, 0B1h, 0D0h, 06Eh, 000h, 000h
        db  0FFh, 0B1h, 0CCh, 06Eh, 000h, 000h
        push   esi
        call   sub_4_DC67
        add    esp, 40h
        or     eax, eax
        je     SHORT loc_4_CE44
        cmp    word ptr [esi + 1ah], 0eah
        jne    SHORT loc_4_CE40
        inc    word ptr [esi + 0b0h]
loc_4_CE40:
        sub    eax, eax
        jmp    SHORT loc_4_CE50
loc_4_CE44:
        inc    word ptr [esi + 0b0h]
        mov    eax, 1
loc_4_CE50:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 54h
        push   esi
        push   edi
        lea    edi, [ebp - 54h]
        sub    eax, eax
        mov    ecx, 0dh
        rep stosd
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 3ah], 1
        movzx  eax, byte ptr [esi + 0a8h]
        cmp    eax, 3
        je     SHORT loc_4_CE90
        cmp    eax, 4
        je     SHORT loc_4_CE98
        mov    word ptr [esi + 1ah], 1
        jmp    NEAR PTR loc_4_CF6D
loc_4_CE90:
        mov    word ptr [ebp - 40h], 1
        jmp    SHORT loc_4_CE9E
loc_4_CE98:
        mov    word ptr [ebp - 40h], 0
loc_4_CE9E:
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_CEAF
        mov    eax, dword ptr [esi + 9ch]
        jmp    SHORT loc_4_CEBC
loc_4_CEAF:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_CEBC:
        push   9
        push   eax
        lea    eax, [ebp - 54h]
        push   eax
        call   sub_4_6220
        add    esp, 0ch
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_CEDC
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_CEE9
loc_4_CEDC:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_CEE9:
        mov    dword ptr [ebp - 4ah], eax
        mov    edi, dword ptr [ebp - 4ah]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edx, dword ptr [ebp - 4ah]
        add    edx, ecx
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   1
        push   ecx
        push   edx
        lea    eax, [ebp - 20h]
        push   eax
        call   sub_4_14A69
        add    esp, 10h
        mov    edi, eax
        push   edi
        lea    eax, [ebp - 20h]
        push   eax
        push   eax
        call   sub_4_14A8A
        add    esp, 0ch
        push   1
        push   0eh
        push   edi
        lea    eax, [ebp - 20h]
        push   eax
        push   eax
        call   sub_4_14A00
        add    esp, 14h
        mov    byte ptr [ebp + eax - 20h], 0
        lea    eax, [ebp - 20h]
        mov    dword ptr [ebp - 46h], eax
        mov    dword ptr [esi + 24h], 0
        push   0
        push   0
        lea    eax, [ebp - 54h]
        push   eax
        push   esi
        call   sub_4_D46A
        add    esp, 10h
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [esi + 0b4h], ax
loc_4_CF6D:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_4_CF8E
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_4_F626
        add    esp, 8
        jmp    SHORT loc_4_CFAE
loc_4_CF8E:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 84h], eax
        push   0
        movzx  eax, byte ptr [esi + 4]
        push   eax
        push   dword ptr [esi + 14h]
        push   esi
        call   sub_4_D46A
        add    esp, 10h
        movsx  eax, word ptr [esi + 1ah]
loc_4_CFAE:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        movzx  eax, word ptr [esi + 0a8h]
        cmp    eax, 1
        je     SHORT loc_4_CFDB
        cmp    eax, 2
        je     SHORT loc_4_CFEE
        mov    word ptr [esi + 1ah], 7ch
        jmp    NEAR PTR loc_4_D0A5
loc_4_CFDB:
        mov    word ptr [ebp - 1ah], 1
        mov    word ptr [ebp - 18h], 0
        mov    ebx, 1ah
        jmp    SHORT loc_4_CFF3
loc_4_CFEE:
        mov    ebx, 34h
loc_4_CFF3:
        movzx  eax, word ptr [esi + 0b0h]
        cmp    eax, ebx
        jge    SHORT loc_4_D009
        mov    word ptr [esi + 1ah], 84bh
        jmp    NEAR PTR loc_4_D0A5
loc_4_D009:
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_D01A
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_D027
loc_4_D01A:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_D027:
        push   ebx
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        call   sub_4_6220
        add    esp, 0ch
        mov    word ptr [esi + 1ah], ax
        or     ax, ax
        jne    SHORT loc_4_D0A5
        test   byte ptr [esi + 8bh], 4
        jne    SHORT loc_4_D06C
        push   dword ptr [ebp - 2ah]
        call   sub_4_21A2
        add    esp, 4
        mov    dword ptr [ebp - 2ah], eax
        test   byte ptr [esi + 8bh], 4
        jne    SHORT loc_4_D06C
        push   dword ptr [ebp - 26h]
        call   sub_4_21A2
        add    esp, 4
        mov    dword ptr [ebp - 26h], eax
loc_4_D06C:
        cmp    word ptr [ebp - 20h], -1
        je     SHORT loc_4_D083
        mov    di, word ptr [ebp - 20h]
        and    edi, 80h
        and    word ptr [ebp - 20h], 0ff7fh
loc_4_D083:
        mov    dword ptr [esi + 24h], 0
        push   0
        push   edi
        lea    eax, [ebp - 34h]
        push   eax
        push   esi
        call   sub_4_D46A
        add    esp, 10h
        mov    ax, word ptr [esi + 18h]
        mov    word ptr [esi + 0b4h], ax
loc_4_D0A5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0b4h], 4
        jne    SHORT loc_4_D0F7
        test   byte ptr [esi + 8bh], 4
        lea    edi, [esi + 14h]
        je     SHORT loc_4_D0D1
        mov    eax, dword ptr [esi + 9ch]
        jmp    SHORT loc_4_D0DE
loc_4_D0D1:
        push   3c04h
        call   sub_4_2154
        add    esp, 4
loc_4_D0DE:
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [edi]
        cmp    word ptr [eax], 5c5ch
        jne    SHORT loc_4_D0F0
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_D13C
loc_4_D0F0:
        mov    edi, 3
        jmp    SHORT loc_4_D11F
loc_4_D0F7:
        test   byte ptr [esi + 8bh], 4
        je     SHORT loc_4_D108
        mov    eax, dword ptr [esi + 98h]
        jmp    SHORT loc_4_D115
loc_4_D108:
        push   3800h
        call   sub_4_2154
        add    esp, 4
loc_4_D115:
        mov    dword ptr [esi + 14h], eax
        movzx  edi, word ptr [esi + 0a8h]
loc_4_D11F:
        push   esi
        call   sub_4_D18A
        add    esp, 4
        cmp    eax, -1
        jne    SHORT loc_4_D12F
        jmp    SHORT loc_4_D13A
loc_4_D12F:
        push   edi
        push   eax
        push   esi
        db  0E8h, 005h, 075h, 0FAh, 0FFh
        add    esp, 0ch
loc_4_D13A:
        sub    eax, eax
loc_4_D13C:
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_4_D15B
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_4_F6ED
        add    esp, 8
        jmp    SHORT loc_4_D187
loc_4_D15B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 84h], eax
        push   esi
        call   sub_4_D18A
        add    esp, 4
        cmp    eax, -1
        jne    SHORT loc_4_D174
        jmp    SHORT loc_4_D183
loc_4_D174:
        movzx  ecx, byte ptr [esi + 4]
        push   ecx
        push   eax
        push   esi
        db  0E8h, 0BCh, 074h, 0FAh, 0FFh
        add    esp, 0ch
loc_4_D183:
        movsx  eax, word ptr [esi + 1ah]
loc_4_D187:
        pop    esi
        pop    ebp
        ret
sub_4_D18A:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 89h], 0
        call   sub_4_15558
        add    esp, 4
        and    eax, 0fh
        cmp    eax, 7
        ja     SHORT loc_4_D1C7
        db  0FFh, 024h, 085h, 0D4h, 0B1h, 000h, 000h
        db  080h, 0BEh, 089h, 000h, 000h, 000h, 000h, 074h, 00Eh, 00Fh, 0B6h, 086h, 089h, 000h, 000h, 000h
loc_4_D1C0:
        pop    esi
        pop    ebp
        ret
        db  02Bh, 0C0h, 0EBh, 0F9h
loc_4_D1C7:
        mov    word ptr [esi + 1ah], 0fh
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_D1C0
        db  0B0h, 0B1h, 000h, 000h, 0B0h, 0B1h, 000h, 000h, 0C3h, 0B1h, 000h, 000h, 0C7h, 0B1h, 000h, 000h
        db  0C7h, 0B1h, 000h, 000h, 0B9h, 0B1h, 000h, 000h, 0B0h, 0B1h, 000h, 000h, 0B9h, 0B1h, 000h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    cl, byte ptr [eax + 0a8h]
        mov    byte ptr [eax + 4], cl
        call   sub_4_D20D
        mov    esp, ebp
        pop    ebp
        ret
sub_4_D20D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   1
        mov    bl, byte ptr [esi + 4]
        push   esi
        db  0E8h, 00Fh, 07Ah, 0FAh, 0FFh
        add    esp, 8
        cmp    bl, 2
        jae    SHORT loc_4_D248
        db  0A1h, 088h, 06Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_D248
        sub    ecx, ecx
loc_4_D237:
        cmp    word ptr [eax + 16h], cx
        ja     NEAR PTR loc_4_D352
        mov    eax, dword ptr [eax + 4]
        cmp    eax, ecx
        jne    SHORT loc_4_D237
loc_4_D248:
        cmp    bl, 3
        jae    SHORT loc_4_D284
        mov    edi, 1
loc_4_D252:
        db  08Bh, 004h, 0BDh, 0BCh, 065h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_D27E
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_4_D27E
        test   byte ptr [eax + 1bh], 8
        je     SHORT loc_4_D27E
        lea    eax, [edi - 1]
        push   eax
        push   esi
        call   sub_4_501C
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_D35A
loc_4_D27E:
        inc    edi
        cmp    edi, 20h
        jle    SHORT loc_4_D252
loc_4_D284:
        mov    edi, 21h
loc_4_D289:
        db  08Bh, 00Ch, 0BDh, 0B0h, 06Dh, 000h, 000h
        mov    dword ptr [esi + 7ch], ecx
        or     ecx, ecx
        je     SHORT loc_4_D2B5
        mov    eax, dword ptr [esi + 84h]
        cmp    dword ptr [ecx + 1ch], eax
        je     SHORT loc_4_D2A7
        cmp    eax, -1
        jne    SHORT loc_4_D2B5
loc_4_D2A7:
        movzx  eax, bl
        push   eax
        push   edi
        push   esi
        db  0E8h, 08Ah, 073h, 0FAh, 0FFh
        add    esp, 0ch
loc_4_D2B5:
        inc    edi
        cmp    edi, 29h
        jle    SHORT loc_4_D289
        mov    dword ptr [ebp - 4], 1
        db  0BFh, 0C0h, 065h, 000h, 000h
loc_4_D2C7:
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     SHORT loc_4_D309
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_4_D309
        test   byte ptr [ecx + 1bh], 8
        je     SHORT loc_4_D309
        mov    dword ptr [esi + 7ch], ecx
        mov    eax, dword ptr [edi]
        test   byte ptr [eax + 0bh], 2
        je     SHORT loc_4_D2E9
        cmp    bl, 4
        jb     SHORT loc_4_D309
loc_4_D2E9:
        mov    eax, dword ptr [esi + 84h]
        cmp    dword ptr [ecx + 1ch], eax
        je     SHORT loc_4_D2F9
        cmp    eax, -1
        jne    SHORT loc_4_D309
loc_4_D2F9:
        movzx  eax, bl
        push   eax
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 036h, 073h, 0FAh, 0FFh
        add    esp, 0ch
loc_4_D309:
        add    edi, 4
        inc    dword ptr [ebp - 4]
        db  081h, 0FFh, 03Ch, 066h, 000h, 000h
        jbe    SHORT loc_4_D2C7
        db  0A1h, 088h, 06Eh, 000h, 000h
        mov    dword ptr [esi + 7ch], eax
        or     eax, eax
        je     NEAR PTR loc_4_D3CD
loc_4_D327:
        mov    edi, dword ptr [esi + 7ch]
        mov    cl, byte ptr [edi + 1bh]
        lea    edx, [edi + 1bh]
        test   cl, 20h
        je     SHORT loc_4_D33A
        cmp    bl, 3
        jbe    SHORT loc_4_D34A
loc_4_D33A:
        mov    eax, dword ptr [esi + 84h]
        cmp    dword ptr [edi + 1ch], eax
        je     SHORT loc_4_D362
        cmp    eax, -1
        je     SHORT loc_4_D362
loc_4_D34A:
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 7ch], eax
        jmp    SHORT loc_4_D3AE
loc_4_D352:
        mov    word ptr [esi + 1ah], 961h
        jmp    SHORT loc_4_D3CD
loc_4_D35A:
        mov    word ptr [esi + 1ah], 964h
        jmp    SHORT loc_4_D3CD
loc_4_D362:
        and    cl, 0feh
        mov    byte ptr [edx], cl
        mov    eax, dword ptr [esi + 7ch]
        push   esi
        mov    byte ptr [eax + 18h], 0
        mov    dword ptr [esi + 74h], 0
        db  0E8h, 000h, 07Ah, 0FAh, 0FFh
        add    esp, 4
        mov    edi, dword ptr [esi + 7ch]
        movzx  eax, bl
        push   eax
        push   esi
        call   sub_4_D7A7
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_D3BA
        mov    eax, dword ptr [esi + 7ch]
        push   esi
        mov    dword ptr [eax + 10h], 1
        mov    eax, dword ptr [esi + 7ch]
        mov    edi, dword ptr [eax + 4]
        db  0E8h, 056h, 075h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 7ch], edi
loc_4_D3AE:
        cmp    dword ptr [esi + 7ch], 0
        jne    NEAR PTR loc_4_D327
        jmp    SHORT loc_4_D3CD
loc_4_D3BA:
        mov    dword ptr [esi + 7ch], edi
        push   esi
        db  0E8h, 03Bh, 075h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 7ch], 0
loc_4_D3CD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 74h]
        mov    eax, dword ptr [esi + 7ch]
        mov    edx, dword ptr [ecx + 14h]
        cmp    dword ptr [ecx + 10h], eax
        je     SHORT loc_4_D3EF
        sub    eax, eax
        jmp    SHORT loc_4_D427
loc_4_D3EF:
        cmp    dword ptr [ecx + 0ch], ecx
        jne    SHORT loc_4_D3FA
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ecx + 4ch], eax
loc_4_D3FA:
        mov    eax, dword ptr [esi + 74h]
        mov    dword ptr [esi + 78h], edx
        cmp    byte ptr [eax + 1fh], 1
        jne    SHORT loc_4_D40E
        mov    ax, word ptr [edx + 1bh]
        mov    word ptr [esi + 6], ax
loc_4_D40E:
        mov    edi, dword ptr [esi + 7ch]
        mov    bl, byte ptr [esi + 4]
        push   esi
        db  0E8h, 0A3h, 069h, 0FAh, 0FFh
        add    esp, 4
        mov    byte ptr [esi + 4], bl
        mov    dword ptr [esi + 7ch], edi
        movsx  eax, word ptr [esi + 1ah]
loc_4_D427:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_D42C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 004h, 085h, 0BCh, 065h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_D45F
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_4_D45F
        db  0A1h, 084h, 06Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_D45F
loc_4_D44C:
        cmp    ecx, eax
        je     SHORT loc_4_D458
        mov    edx, dword ptr [eax + 20h]
        cmp    dword ptr [ecx + 20h], edx
        je     SHORT loc_4_D463
loc_4_D458:
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        jne    SHORT loc_4_D44C
loc_4_D45F:
        sub    eax, eax
loc_4_D461:
        pop    ebp
        ret
loc_4_D463:
        mov    eax, 1
        jmp    SHORT loc_4_D461
sub_4_D46A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  068h, 0A4h, 06Eh, 000h, 000h
        db  0E8h, 01Ch, 071h, 0FAh, 0FFh
        mov    eax, dword ptr [ebp + 0ch]
        add    esp, 4
        movsx  ebx, byte ptr [eax]
        or     ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        je     NEAR PTR loc_4_D5F2
        mov    dword ptr [esi + 14h], eax
        push   esi
        call   sub_4_D18A
        add    esp, 4
        mov    ebx, eax
        cmp    ebx, -1
        je     NEAR PTR loc_4_D793
        or     ebx, ebx
        jne    SHORT loc_4_D4BE
        mov    word ptr [esi + 1ah], 3
        jmp    NEAR PTR loc_4_D793
loc_4_D4BE:
        mov    eax, dword ptr [esi + 7ch]
        or     eax, eax
        je     SHORT loc_4_D4D3
        mov    al, byte ptr [eax + 1bh]
        test   al, 80h
        je     SHORT loc_4_D4EF
loc_4_D4CC:
        mov    dword ptr [esi + 7ch], 0
loc_4_D4D3:
        cmp    ebx, 20h
        jg     NEAR PTR loc_4_D5D7
        db  039h, 01Dh, 044h, 066h, 000h, 000h
        jae    SHORT loc_4_D543
        mov    word ptr [esi + 1ah], 0fh
        jmp    NEAR PTR loc_4_D793
loc_4_D4EF:
        test   al, 10h
        je     SHORT loc_4_D531
        push   ebx
        call   sub_4_D42C
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_D531
        mov    eax, dword ptr [esi + 7ch]
        movzx  eax, word ptr [eax + 14h]
        push   eax
        call   sub_4_120CA
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_D531
        dec    byte ptr [esi + 89h]
        push   0
        push   esi
        call   sub_4_E4EA
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_D531
        inc    byte ptr [esi + 89h]
        jmp    SHORT loc_4_D4CC
loc_4_D531:
        mov    dword ptr [esi + 7ch], 0
        mov    word ptr [esi + 1ah], 55h
        jmp    NEAR PTR loc_4_D793
loc_4_D543:
        lea    eax, [ebx - 1]
        push   eax
        call   sub_4_120CA
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_D55E
        mov    word ptr [esi + 1ah], 55h
        jmp    NEAR PTR loc_4_D793
loc_4_D55E:
        lea    ecx, [ebx*4]
        db  08Bh, 081h, 044h, 066h, 000h, 000h
        mov    ax, word ptr [eax + 43h]
        test   ah, 40h
        je     SHORT loc_4_D584
        test   ah, 0b0h
        je     SHORT loc_4_D584
        mov    word ptr [esi + 1ah], 55h
        jmp    NEAR PTR loc_4_D793
loc_4_D584:
        db  08Bh, 089h, 0BCh, 065h, 000h, 000h
        mov    dword ptr [ebp - 4], ecx
        or     ecx, ecx
        jne    SHORT loc_4_D5AF
        push   1
        push   18h
        db  0E8h, 05Eh, 0B0h, 000h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    SHORT loc_4_D5B9
        mov    word ptr [esi + 1ah], 54h
        jmp    NEAR PTR loc_4_D793
loc_4_D5AF:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
IFSMgr_FSDUnmountCFSD:
        stosd
        stosd
        stosd
        stosd
        stosd
loc_4_D5B9:
        mov    ecx, dword ptr [ebp - 4]
        mov    ax, word ptr [esi + 0b0h]
        cmp    dword ptr [ebp + 14h], 0
        mov    word ptr [ecx + 8], ax
        mov    byte ptr [ecx + 0ah], 0
        je     SHORT loc_4_D5F2
        or     byte ptr [ecx + 0bh], 2
        jmp    SHORT loc_4_D5F2
loc_4_D5D7:
        db  0A1h, 044h, 059h, 000h, 000h
        lea    ecx, [ebx - 21h]
        movzx  eax, word ptr [eax + 0ah]
        cmp    eax, ecx
        jg     SHORT loc_4_D5F2
sub_4_D5E7:
        mov    word ptr [esi + 1ah], 0fh
        jmp    NEAR PTR loc_4_D793
loc_4_D5F2:
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        mov    eax, dword ptr [eax + 0ah]
        mov    dword ptr [esi + 14h], eax
        call   sub_4_15558
        add    esp, 4
        and    eax, 0fh
        cmp    eax, 2
        je     SHORT loc_4_D623
        cmp    eax, 4
        je     SHORT loc_4_D623
        mov    word ptr [esi + 1ah], 35h
        mov    dword ptr [esi + 7ch], 0
        jmp    NEAR PTR loc_4_D793
loc_4_D623:
        mov    eax, dword ptr [ebp + 0ch]
        sub    edi, edi
        mov    eax, dword ptr [eax + 0eh]
        cmp    eax, edi
        je     SHORT loc_4_D647
        mov    edi, dword ptr [esi + 8ch]
        add    edi, 208h
        push   -1
        push   eax
        push   edi
        call   sub_4_6220
        add    esp, 0ch
loc_4_D647:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 28h], edi
        mov    ax, word ptr [eax + 14h]
        inc    al
        mov    byte ptr [esi + 4], al
        mov    eax, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [eax + 14h]
        inc    eax
        push   eax
        cmp    dword ptr [ebp + 10h], 1
        cmc
        sbb    ecx, ecx
        and    ecx, 8
        cmp    dword ptr [ebp + 14h], 1
        cmc
        sbb    eax, eax
        and    eax, 20h
        or     ecx, eax
        cmp    ebx, 1
        sbb    eax, eax
        and    eax, 2
        add    eax, 2
        or     ecx, eax
        push   ecx
        push   esi
        call   sub_4_D820
        add    esp, 0ch
        cmp    word ptr [esi + 1ah], 0
        je     SHORT loc_4_D6BC
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_4_D793
        db  0F6h, 083h, 08Bh, 000h, 000h, 000h, 001h
        jne    NEAR PTR loc_4_D793
        push   0
        push   dword ptr [ebp - 4]
        db  0E8h, 04Ch, 0AFh, 000h, 000h
        add    esp, 8
        jmp    NEAR PTR loc_4_D793
loc_4_D6BC:
        or     ebx, ebx
        jne    SHORT loc_4_D6F2
        mov    eax, dword ptr [esi + 7ch]
        lea    edi, [esi + 7ch]
        mov    cl, byte ptr [eax + 18h]
        add    eax, 18h
        cmp    cl, 0fah
        jb     SHORT loc_4_D6DC
        mov    word ptr [esi + 1ah], 54h
        jmp    NEAR PTR loc_4_D793
loc_4_D6DC:
        inc    cl
        mov    byte ptr [eax], cl
        mov    ecx, dword ptr [edi]
        mov    ax, word ptr [esi + 0b0h]
        mov    word ptr [ecx + 14h], ax
        jmp    NEAR PTR loc_4_D77B
loc_4_D6F2:
        cmp    ebx, 20h
        jg     SHORT loc_4_D725
        mov    eax, dword ptr [esi + 7ch]
        lea    edi, [esi + 7ch]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        push   dword ptr [edi]
        push   ecx
        push   ebx
        dec    ebx
        push   esi
        db  0E8h, 0C1h, 05Fh, 0FAh, 0FFh
        add    esp, 14h
        push   0
        push   ebx
        db  068h, 020h, 06Bh, 000h, 000h
        db  0E8h, 0C8h, 0AEh, 000h, 000h
        add    esp, 0ch
        jmp    SHORT loc_4_D77B
loc_4_D725:
        db  08Bh, 004h, 09Dh, 0D4h, 06Dh, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_D739
        push   eax
        call   sub_4_F5B4
        add    esp, 4
loc_4_D739:
        mov    eax, dword ptr [esi + 7ch]
        lea    edi, [esi + 7ch]
        inc    byte ptr [eax + 19h]
        mov    eax, dword ptr [edi]
        db  089h, 004h, 09Dh, 0B0h, 06Dh, 000h, 000h
        lea    eax, [ebx - 21h]
        push   eax
        push   esi
        call   sub_4_514C
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_D783
        lea    eax, [ebx - 21h]
        push   eax
        push   3
        call   sub_4_20C2
        add    esp, 8
        sub    bl, 21h
        mov    byte ptr [esi + 4], bl
        push   4
        push   esi
        call   sub_4_4978
        add    esp, 8
loc_4_D77B:
        mov    dword ptr [edi], 0
        jmp    SHORT loc_4_D793
loc_4_D783:
        db  0C7h, 004h, 09Dh, 0B0h, 06Dh, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edi]
        dec    byte ptr [eax + 19h]
loc_4_D793:
        db  068h, 0A4h, 06Eh, 000h, 000h
        db  0E8h, 081h, 06Eh, 0FAh, 0FFh
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_D7A7:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 7ch]
        cmp    word ptr [eax + 16h], 0
        jbe    SHORT loc_4_D808
        movzx  ecx, byte ptr [eax + 18h]
        movzx  edx, byte ptr [eax + 19h]
        add    ecx, edx
        cmp    ecx, 1
        jg     SHORT loc_4_D808
        test   byte ptr [eax + 1bh], 1
        jne    SHORT loc_4_D808
IFSMgr_DelAllUses:
        cmp    dword ptr [ebp + 0ch], 2
        jge    SHORT loc_4_D7E7
        mov    dword ptr [esi + 7ch], 0
        mov    word ptr [esi + 1ah], 961h
        mov    eax, 961h
        jmp    SHORT loc_4_D80A
loc_4_D7E7:
        push   8
        db  068h, 0D4h, 0B3h, 000h, 000h
        push   esi
        call   sub_4_4F1A
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_D808
        mov    dword ptr [esi + 7ch], 0
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_D80A
loc_4_D808:
        sub    eax, eax
loc_4_D80A:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 1ah], 35h
        mov    eax, 35h
        ret
sub_4_D820:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 4]
        db  088h, 045h
IFSMgr_CheckDelResource:
        dec    dword ptr [ebx + 45891046h]
        db  0E8h, 08Bh, 006h, 089h, 045h
        in     al, dx
        mov    ax, word ptr [esi + 6]
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [esi + 84h]
        mov    dword ptr [ebp - 1ch], eax
        db  0E8h, 0EDh, 0ADh, 000h, 000h
        db  08Bh, 03Dh, 0BCh, 055h, 000h, 000h
        add    edi, eax
        db  0A1h, 044h, 059h, 000h, 000h
        push   5
        inc    byte ptr [eax + 12fh]
        db  0FFh, 035h, 090h, 083h, 000h, 000h
        db  0E8h, 0BDh, 0ADh, 000h, 000h
        add    esp, 8
        test   byte ptr [ebp + 0ch], 10h
        je     SHORT loc_4_D8C9
        sub    ebx, ebx
loc_4_D87E:
        mov    eax, dword ptr [ebp + 10h]
        mov    byte ptr [esi + 4], al
        mov    ax, word ptr [ebp + 0ch]
        mov    word ptr [esi + 18h], ax
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 20h], ebx
        mov    eax, dword ptr [edi]
        test   eax, 80000000h
        je     SHORT loc_4_D91A
        and    eax, 7fffffffh
        mov    dword ptr [edi], eax
        db  0FFh, 00Dh, 0A0h, 072h, 000h, 000h
        db  083h, 03Dh, 0A0h, 072h, 000h, 000h, 001h
        jne    SHORT loc_4_D8C1
        db  068h, 0A0h, 072h, 000h, 000h
        db  0E8h, 033h, 056h, 0FAh, 0FFh
        add    esp, 4
loc_4_D8C1:
        or     dword ptr [edi], 20000000h
        jmp    SHORT loc_4_D928
loc_4_D8C9:
        db  08Bh, 01Dh, 07Ch, 06Eh, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_4_D8DF
        db  0C7h, 005h, 07Ch, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_D87E
loc_4_D8DF:
        push   1
        push   44h
        db  0E8h, 010h, 0ADh, 000h, 000h
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_4_D87E
        db  0FFh, 035h, 090h, 083h, 000h, 000h
        db  0E8h, 03Ch, 0ADh, 000h, 000h
        db  0A1h, 044h, 059h, 000h, 000h
        add    esp, 4
        dec    byte ptr [eax + 12fh]
        mov    word ptr [esi + 1ah], 54h
        mov    eax, 54h
        jmp    NEAR PTR loc_4_DC60
loc_4_D91A:
        test   eax, 0ffffh
        je     SHORT loc_4_D928
        or     eax, 20000000h
        mov    dword ptr [edi], eax
loc_4_D928:
        mov    dword ptr [ebp - 8], 35h
        mov    eax, dword ptr [esi + 84h]
        cmp    eax, -1
        je     SHORT loc_4_D96F
        push   eax
        push   1
        call   sub_4_2047
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, -1
        je     NEAR PTR loc_4_DA17
        push   0
        push   esi
        imul   eax, eax, 14h
        push   1eh
        db  0FFh, 0B0h, 094h, 056h, 000h, 000h
        db  0E8h, 02Ch, 062h, 0FAh, 0FFh
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_4_DA17
loc_4_D96F:
        push   dword ptr [esi + 0ch]
        db  0E8h, 063h, 0ABh, 000h, 000h
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        je     SHORT loc_4_D9B9
        mov    dword ptr [esi + 84h], eax
        push   eax
        push   1
        call   sub_4_2047
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    word ptr [esi + 1ah], 0
        cmp    eax, -1
        je     SHORT loc_4_D9B9
        push   0
        push   esi
        imul   eax, eax, 14h
        push   1eh
        db  0FFh, 0B0h, 094h, 056h, 000h, 000h
        db  0E8h, 0DDh, 061h, 0FAh, 0FFh
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
loc_4_D9B9:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_4_D9C5
        cmp    dword ptr [ebp - 8], 35h
        jne    SHORT loc_4_DA1D
loc_4_D9C5:
        mov    dword ptr [ebp - 0ch], 0
        db  080h, 03Dh, 029h, 059h, 000h, 000h, 000h
        jbe    SHORT loc_4_DA17
        db  0BFh, 098h, 056h, 000h, 000h
loc_4_D9DA:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 84h], eax
        cmp    eax, dword ptr [ebp - 10h]
        je     SHORT loc_4_DA05
        mov    word ptr [esi + 1ah], 0
        push   0
        push   esi
        push   1eh
        push   dword ptr [edi - 4]
        db  0E8h, 096h, 061h, 0FAh, 0FFh
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 35h
        jne    SHORT loc_4_DA17
loc_4_DA05:
        add    edi, 14h
        inc    dword ptr [ebp - 0ch]
        db  00Fh, 0B6h, 005h, 029h, 059h, 000h, 000h
        cmp    eax, dword ptr [ebp - 0ch]
        jg     SHORT loc_4_D9DA
loc_4_DA17:
        cmp    dword ptr [ebp - 8], 35h
        je     SHORT loc_4_DA3D
loc_4_DA1D:
        push   dword ptr [ebp - 0ch]
        push   dword ptr [esi + 0ch]
        db  0E8h, 0D3h, 0AAh, 000h, 000h
        add    esp, 8
        cmp    dword ptr [ebp - 8], 35h
        je     SHORT loc_4_DA3D
        cmp    dword ptr [ebp - 8], 43h
        je     SHORT loc_4_DA3D
        cmp    dword ptr [ebp - 8], 33h
        jne    SHORT loc_4_DA6E
loc_4_DA3D:
        cmp    dword ptr [ebp - 1ch], -1
        jne    SHORT loc_4_DA6E
        mov    word ptr [esi + 1ah], 0
        lea    eax, [esi + 84h]
        push   eax
        push   esi
        db  0E8h, 020h, 04Dh, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_DA67
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [esi + 1ah], ax
        jmp    SHORT loc_4_DA6E
loc_4_DA67:
        mov    dword ptr [ebp - 8], 0
loc_4_DA6E:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_4_DBFB
        push   dword ptr [esi]
        push   esi
        call   sub_4_15FA0
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_DBFB
        or     ebx, ebx
        jne    SHORT loc_4_DAB2
        mov    eax, dword ptr [esi + 20h]
        or     eax, eax
        jne    SHORT loc_4_DAA1
        mov    word ptr [esi + 1ah], 8cah
        jmp    NEAR PTR loc_4_DC2A
loc_4_DAA1:
        mov    dword ptr [esi + 7ch], eax
        push   esi
        db  0E8h, 0D1h, 072h, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_4_DC2A
loc_4_DAB2:
        cmp    dword ptr [esi + 20h], ebx
        je     SHORT loc_4_DAF7
        db  083h, 03Dh, 07Ch, 06Eh, 000h, 000h, 000h
        jne    SHORT loc_4_DAC8
        db  089h, 01Dh, 07Ch, 06Eh, 000h, 000h
        jmp    SHORT loc_4_DAD3
loc_4_DAC8:
        push   0
        push   ebx
        db  0E8h, 030h, 0ABh, 000h, 000h
        add    esp, 8
loc_4_DAD3:
        mov    eax, dword ptr [esi + 20h]
        cmp    byte ptr [eax + 1ah], 0
        jne    SHORT loc_4_DAE6
        mov    cl, byte ptr [esi + 4]
        or     cl, cl
        je     SHORT loc_4_DAE6
        mov    byte ptr [eax + 1ah], cl
loc_4_DAE6:
        mov    dword ptr [esi + 7ch], eax
        push   esi
        db  0E8h, 08Ch, 072h, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_4_DC2A
loc_4_DAF7:
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 11h
        rep stosd
        push   ebx
        call   sub_4_23B6
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_DB81
        mov    edi, dword ptr [esi + 0ch]
        mov    word ptr [esi + 18h], 0
        mov    dword ptr [esi + 0ch], 0
        mov    dword ptr [esi + 7ch], ebx
        mov    word ptr [ebx], 7253h
        mov    ecx, dword ptr [esi + 7ch]
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    dword ptr [ebx + 10h], 1
        push   0
        push   esi
        mov    eax, dword ptr [ebx + 0ch]
        push   27h
        push   dword ptr [eax + 24h]
        db  0E8h, 049h, 060h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_DB61
        push   0
        push   esi
        mov    eax, dword ptr [ebx + 0ch]
        push   28h
        push   dword ptr [eax + 28h]
        db  0E8h, 032h, 060h, 0FAh, 0FFh
        add    esp, 10h
loc_4_DB61:
        push   0
        push   ebx
        db  0E8h, 097h, 0AAh, 000h, 000h
        add    esp, 8
        mov    dword ptr [esi + 7ch], 0
        mov    dword ptr [esi + 0ch], edi
        mov    word ptr [esi + 1ah], 54h
        jmp    NEAR PTR loc_4_DC2A
loc_4_DB81:
        mov    dword ptr [esi + 7ch], ebx
        mov    word ptr [ebx], 7253h
        db  066h, 0FFh, 005h, 094h, 083h, 000h, 000h
        db  0A0h, 094h, 083h, 000h, 000h
        mov    byte ptr [ebx + 2], al
        mov    eax, dword ptr [esi + 84h]
        mov    dword ptr [ebx + 1ch], eax
        mov    ecx, dword ptr [esi + 7ch]
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 10h], 1
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [ebx + 1ah], al
        lea    eax, [ebx + 1bh]
        test   byte ptr [ebp + 0ch], 1
        je     SHORT loc_4_DBCB
        mov    byte ptr [eax], 9
        jmp    SHORT loc_4_DBCE
loc_4_DBCB:
        mov    byte ptr [eax], 8
loc_4_DBCE:
        test   byte ptr [ebp + 0ch], 20h
        je     SHORT loc_4_DBD7
        or     byte ptr [eax], 20h
loc_4_DBD7:
        test   byte ptr [esi + 6], 1
        je     SHORT loc_4_DBEB
        db  080h, 03Dh, 040h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_DBEB
        or     word ptr [ebx + 32h], 2
loc_4_DBEB:
        db  0A1h, 088h, 06Eh, 000h, 000h
        mov    dword ptr [ebx + 4], eax
        db  089h, 01Dh, 088h, 06Eh, 000h, 000h
        jmp    SHORT loc_4_DC2A
loc_4_DBFB:
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_4_DC0A
        mov    ax, word ptr [ebp - 8]
        mov    word ptr [esi + 1ah], ax
loc_4_DC0A:
        or     ebx, ebx
        je     SHORT loc_4_DC2A
        db  083h, 03Dh, 07Ch, 06Eh, 000h, 000h, 000h
        jne    SHORT loc_4_DC1F
        db  089h, 01Dh, 07Ch, 06Eh, 000h, 000h
        jmp    SHORT loc_4_DC2A
loc_4_DC1F:
        push   0
        push   ebx
        db  0E8h, 0D9h, 0A9h, 000h, 000h
        add    esp, 8
loc_4_DC2A:
        db  0FFh, 035h, 090h, 083h, 000h, 000h
        db  0E8h, 003h, 0AAh, 000h, 000h
        db  0A1h, 044h, 059h, 000h, 000h
        add    esp, 4
        dec    byte ptr [eax + 12fh]
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 4], al
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [esi], eax
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [esi + 6], ax
        movsx  eax, word ptr [esi + 1ah]
loc_4_DC60:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_DC67:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        mov    ecx, dword ptr [edi]
        mov    eax, ecx
        mov    dword ptr [ebp - 1ch], ecx
        mov    edi, dword ptr [edi + 14h]
        mov    dword ptr [ebp - 4], edi
        add    eax, edi
        mov    dword ptr [ebp - 14h], eax
        mov    eax, ecx
        mov    dword ptr [ebp - 18h], eax
        cmp    ecx, dword ptr [ebp + 0ch]
        jae    SHORT loc_4_DC9E
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [edi + 1ah], 84bh
loc_4_DC9E:
        lea    ebx, [ebp + 14h]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], eax
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_4_DF3A
loc_4_DCB0:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], 0
        cmp    byte ptr [eax], 30h
        jl     SHORT loc_4_DCE3
loc_4_DCBF:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        cmp    al, 39h
        jg     SHORT loc_4_DCE3
        movsx  eax, al
        mov    ecx, dword ptr [ebp - 0ch]
        imul   ecx, ecx, 0ah
        lea    eax, [eax + ecx - 30h]
        mov    dword ptr [ebp - 0ch], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    byte ptr [eax], 30h
        jge    SHORT loc_4_DCBF
loc_4_DCE3:
        mov    edi, dword ptr [ebp + 8]
        mov    cx, word ptr [edi + 1ah]
        or     cx, cx
        je     SHORT loc_4_DCF5
        lea    edi, [ebp - 24h]
        mov    dword ptr [ebp - 4], edi
loc_4_DCF5:
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        cmp    eax, 57h
        jg     SHORT loc_4_DD29
        je     NEAR PTR loc_4_DE3E
        cmp    eax, 42h
        je     SHORT loc_4_DD6A
        cmp    eax, 44h
        je     SHORT loc_4_DD7C
        cmp    eax, 49h
        je     SHORT loc_4_DD8F
        cmp    eax, 50h
        je     NEAR PTR loc_4_DDA0
        cmp    eax, 53h
        je     NEAR PTR loc_4_DDD3
        jmp    SHORT loc_4_DD4D
loc_4_DD29:
        cmp    eax, 5ah
        je     NEAR PTR loc_4_DE52
        cmp    eax, 63h
        je     NEAR PTR loc_4_DE60
        cmp    eax, 73h
        je     NEAR PTR loc_4_DE9F
        cmp    eax, 74h
        je     NEAR PTR loc_4_DEA6
loc_4_DD4D:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [edi + 1ah], 1
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_4_DF1E
        dec    dword ptr [ebp - 8]
        jmp    NEAR PTR loc_4_DF1E
loc_4_DD6A:
        mov    al, byte ptr [ebx]
        add    ebx, 4
        mov    edi, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 4]
        mov    byte ptr [edi], al
        jmp    NEAR PTR loc_4_DF1E
loc_4_DD7C:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    edi, dword ptr [ebp - 4]
        add    dword ptr [ebp - 4], 4
        mov    dword ptr [edi], eax
        jmp    NEAR PTR loc_4_DF1E
loc_4_DD8F:
        add    ebx, 4
        mov    edi, dword ptr [ebp - 4]
        add    edi, dword ptr [ebx - 4]
        mov    dword ptr [ebp - 4], edi
        jmp    NEAR PTR loc_4_DF1E
loc_4_DDA0:
        mov    edi, dword ptr [ebp + 8]
        add    ebx, 4
        mov    esi, dword ptr [edi + 2ch]
        mov    eax, dword ptr [ebx - 4]
        or     eax, eax
        je     SHORT loc_4_DDC9
        push   1
        push   103h
        add    eax, 4
        push   eax
        push   esi
        call   sub_4_14A26
        add    esp, 10h
        lea    ecx, [eax + 1]
        jmp    SHORT loc_4_DDEE
loc_4_DDC9:
        mov    ecx, 1
        mov    byte ptr [esi], 0
        jmp    SHORT loc_4_DDEE
loc_4_DDD3:
        mov    esi, dword ptr [ebx]
        add    ebx, 4
        or     esi, esi
        jne    SHORT loc_4_DDE1
        db  0BEh, 0F0h, 06Eh, 000h, 000h
loc_4_DDE1:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
loc_4_DDEE:
        mov    eax, dword ptr [ebp - 10h]
        add    eax, ecx
        cmp    eax, dword ptr [ebp - 1ch]
        jge    SHORT loc_4_DE1F
        sub    dword ptr [ebp - 14h], ecx
        mov    edi, dword ptr [ebp + 8]
        sub    dword ptr [ebp - 18h], ecx
        mov    edx, dword ptr [edi + 10h]
        mov    edi, dword ptr [ebp - 4]
        add    edx, dword ptr [ebp - 18h]
        mov    dword ptr [edi], edx
        mov    edx, ecx
        mov    edi, dword ptr [ebp - 14h]
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_4_DE32
loc_4_DE1F:
        mov    edi, dword ptr [ebp + 8]
        cmp    word ptr [edi + 1ah], 0
        jne    SHORT loc_4_DE32
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [edi], 0
loc_4_DE32:
        add    dword ptr [ebp - 4], 4
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_4_DF1E
loc_4_DE3E:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        mov    edi, dword ptr [ebp - 4]
        add    dword ptr [ebp - 4], 2
        mov    word ptr [edi], ax
        jmp    NEAR PTR loc_4_DF1E
loc_4_DE52:
        mov    edi, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 4]
        mov    byte ptr [edi], 0
        jmp    NEAR PTR loc_4_DF1E
loc_4_DE60:
        mov    esi, dword ptr [ebx]
        add    ebx, 4
        mov    edx, dword ptr [ebx]
        add    ebx, 4
        or     cx, cx
        jne    NEAR PTR loc_4_DF1B
        or     esi, esi
        mov    edi, dword ptr [ebp - 4]
        je     SHORT loc_4_DE8D
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        jmp    NEAR PTR loc_4_DF1B
loc_4_DE8D:
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        jmp    SHORT loc_4_DF1B
loc_4_DE9F:
        mov    esi, dword ptr [ebx]
        add    ebx, 4
        jmp    SHORT loc_4_DEDD
loc_4_DEA6:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        or     eax, eax
        jne    SHORT loc_4_DEB5
        mov    byte ptr [ebp - 24h], 0
        jmp    SHORT loc_4_DEDA
loc_4_DEB5:
        cmp    eax, 20h
        jg     SHORT loc_4_DECB
        add    al, 40h
        mov    byte ptr [ebp - 24h], al
        db  0BEh, 0E4h, 06Eh, 000h, 000h
        lea    edi, [ebp - 23h]
        movsw
        jmp    SHORT loc_4_DEDA
loc_4_DECB:
        db  0BEh, 0E8h, 06Eh, 000h, 000h
        lea    edi, [ebp - 24h]
        movsd
        movsb
        sub    al, 21h
        add    byte ptr [ebp - 21h], al
loc_4_DEDA:
        lea    esi, [ebp - 24h]
loc_4_DEDD:
        mov    edx, dword ptr [ebx]
        add    ebx, 4
        mov    edi, dword ptr [ebp + 8]
        cmp    word ptr [edi + 1ah], 0
        jne    SHORT loc_4_DF1E
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        cmp    edx, eax
        jg     SHORT loc_4_DF03
        lea    eax, [edx - 1]
loc_4_DF03:
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 4]
        mov    byte ptr [eax + edi], 0
loc_4_DF1B:
        add    dword ptr [ebp - 4], edx
loc_4_DF1E:
        dec    dword ptr [ebp - 0ch]
        cmp    dword ptr [ebp - 0ch], 0
        jg     NEAR PTR loc_4_DCE3
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_4_DCB0
loc_4_DF3A:
        mov    edi, dword ptr [ebp + 8]
        cmp    word ptr [edi + 1ah], 0
        jne    SHORT loc_4_DF52
        mov    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [ebp - 1ch]
        jle    SHORT loc_4_DF52
        mov    word ptr [edi + 1ah], 0eah
loc_4_DF52:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 10h]
        cmp    word ptr [edi + 1ah], 0
        je     SHORT loc_4_DF63
        mov    dword ptr [edi], eax
        jmp    SHORT loc_4_DF71
loc_4_DF63:
        mov    edi, dword ptr [ebp + 8]
        sub    dword ptr [edi], eax
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [edi + 14h], eax
        add    dword ptr [edi + 10h], eax
loc_4_DF71:
        mov    edi, dword ptr [ebp + 8]
        movsx  eax, word ptr [edi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        push   1
        push   104h
        db  0E8h, 062h, 0A6h, 000h, 000h
        add    esp, 8
        mov    esi, eax
        cmp    esi, edi
        jne    SHORT loc_4_DFA9
        mov    eax, 8
        jmp    NEAR PTR loc_4_E140
loc_4_DFA9:
        push   esi
        call   sub_4_2751
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     SHORT loc_4_DFDA
        cmp    eax, 1
        je     SHORT loc_4_DFFC
        cmp    eax, 2
        je     NEAR PTR loc_4_E07C
        cmp    eax, 3
        je     NEAR PTR loc_4_E096
        mov    eax, 1
        jmp    NEAR PTR loc_4_E140
loc_4_DFDA:
        cmp    dword ptr [ebp + 10h], 2
        jb     SHORT loc_4_DFF2
        cmp    dword ptr [ebp + 10h], 3
        ja     SHORT loc_4_DFF2
        db  0C7h, 045h, 0FCh, 084h, 06Eh, 000h, 000h
        jmp    NEAR PTR loc_4_E0AD
loc_4_DFF2:
        mov    eax, 1
        jmp    NEAR PTR loc_4_E140
loc_4_DFFC:
        push   esi
        db  0E8h, 0D0h, 057h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_E013
        mov    eax, 8
        jmp    NEAR PTR loc_4_E140
loc_4_E013:
        push   1
        push   esi
        db  0E8h, 018h, 06Ch, 0FAh, 0FFh
        db  0A1h, 044h, 059h, 000h, 000h
        add    esp, 8
        inc    byte ptr [eax + 12fh]
        push   5
        db  0FFh, 035h, 090h, 083h, 000h, 000h
        db  0E8h, 0FAh, 0A5h, 000h, 000h
        add    esp, 8
        mov    ebx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 84h], ebx
        mov    al, byte ptr [ebp + 10h]
        mov    byte ptr [esi + 4], al
        push   esi
        call   sub_4_D20D
        add    esp, 4
        movsx  edi, word ptr [esi + 1ah]
        db  0FFh, 035h, 090h, 083h, 000h, 000h
        db  0E8h, 0D8h, 0A5h, 000h, 000h
        db  0A1h, 044h, 059h, 000h, 000h
        add    esp, 4
        dec    byte ptr [eax + 12fh]
        push   esi
        db  0E8h, 051h, 058h, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_4_E133
loc_4_E07C:
        db  083h, 03Dh, 008h, 06Fh, 000h, 000h, 001h
        jbe    SHORT loc_4_E08F
        mov    eax, 5
        jmp    NEAR PTR loc_4_E140
loc_4_E08F:
        sub    eax, eax
        jmp    NEAR PTR loc_4_E140
loc_4_E096:
        cmp    dword ptr [ebp + 10h], 3
        je     SHORT loc_4_E0A6
        mov    eax, 1
        jmp    NEAR PTR loc_4_E140
loc_4_E0A6:
        db  0C7h, 045h, 0FCh, 080h, 06Eh, 000h, 000h
loc_4_E0AD:
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_4_E106
loc_4_E0B9:
        cmp    dword ptr [ecx + 1ch], ebx
        jne    SHORT loc_4_E0FF
        mov    dword ptr [esi + 7ch], ecx
        mov    dword ptr [esi + 84h], ebx
        mov    al, byte ptr [ecx + 1bh]
        test   al, 10h
        je     SHORT loc_4_E0E2
        mov    al, byte ptr [ecx + 14h]
        mov    byte ptr [esi + 89h], al
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_4_E4EA
        jmp    SHORT loc_4_E0EF
loc_4_E0E2:
        test   al, 80h
        db  074h, 00Eh
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_4_F4CE
loc_4_E0EF:
        add    esp, 8
        mov    edi, eax
        db  00Bh
IFSMgr_MakeMailSlot:
        push   dword ptr [ebp + 3bh]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax]
        jmp    SHORT loc_4_E102
loc_4_E0FF:
        mov    ecx, dword ptr [ecx + 4]
loc_4_E102:
        or     ecx, ecx
        jne    SHORT loc_4_E0B9
loc_4_E106:
        cmp    dword ptr [ebp + 8], 1
        sbb    eax, eax
        neg    eax
        cmp    edi, 1
        sbb    ecx, ecx
        neg    ecx
        test   ecx, eax
        je     SHORT loc_4_E133
        sub    eax, eax
loc_4_E11B:
        db  00Fh, 0B6h, 088h, 06Ch, 000h, 000h, 000h
        cmp    ecx, ebx
        jne    SHORT loc_4_E12D
        db  0C6h, 080h, 06Ch, 000h, 000h, 000h, 0FFh
loc_4_E12D:
        inc    eax
        cmp    eax, 20h
        jl     SHORT loc_4_E11B
loc_4_E133:
        push   0
        push   esi
        db  0E8h, 0C5h, 0A4h, 000h, 000h
        add    esp, 8
        mov    eax, edi
loc_4_E140:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        db  08Bh, 035h, 084h, 06Eh, 000h, 000h
        or     esi, esi
        mov    dword ptr [ebx + 7ch], esi
        je     NEAR PTR loc_4_E224
loc_4_E164:
        mov    al, byte ptr [esi + 14h]
        mov    byte ptr [ebx + 89h], al
        movzx  eax, al
        mov    word ptr [ebx + 1ah], 0
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 080h
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        je     SHORT loc_4_E1F9
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 7ch], eax
        or     eax, eax
        je     SHORT loc_4_E198
        push   ebx
        db  0E8h, 086h, 06Ch, 0FAh, 0FFh
        add    esp, 4
loc_4_E198:
        mov    dword ptr [ebx + 7ch], esi
        push   ebx
        db  0E8h, 0DAh, 06Bh, 0FAh, 0FFh
        add    esp, 4
        inc    byte ptr [esi + 19h]
        db  00Fh, 0B6h, 083h
loc_4_E1AA:
        mov    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        mov    edi, 1
        mov    dword ptr [eax], esi
loc_4_E1BC:
        db  08Bh, 004h, 0BDh, 0BCh, 065h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_E1E8
        movzx  eax, byte ptr [eax + 0fh]
        movzx  ecx, word ptr [esi + 14h]
        sub    eax, ecx
        cmp    eax, 1
        jne    SHORT loc_4_E1E8
        lea    eax, [edi - 1]
        mov    byte ptr [ebx + 89h], al
        push   ebx
        db  0E8h, 036h, 06Ch, 0FAh, 0FFh
        add    esp, 4
loc_4_E1E8:
        inc    edi
        cmp    edi, 20h
        jle    SHORT loc_4_E1BC
        mov    al, byte ptr [esi + 14h]
        mov    byte ptr [ebx + 89h], al
        jmp    SHORT loc_4_E1FB
loc_4_E1F9:
        mov    dword ptr [eax], esi
loc_4_E1FB:
        mov    esi, dword ptr [esi + 4]
        push   3
        push   ebx
        call   sub_4_E4EA
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_E213
        db  089h, 035h, 084h, 06Eh, 000h, 000h
loc_4_E213:
        db  08Bh, 035h, 084h, 06Eh, 000h, 000h
        mov    dword ptr [ebx + 7ch], esi
        or     esi, esi
        jne    NEAR PTR loc_4_E164
loc_4_E224:
        mov    dword ptr [ebp - 4], 0
        db  083h, 03Dh, 044h, 066h, 000h, 000h, 000h
        jbe    NEAR PTR loc_4_E375
        mov    dword ptr [ebp - 10h], 0
loc_4_E23F:
        mov    eax, dword ptr [ebx + 90h]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [ebp - 10h]
        db  003h, 088h, 048h, 066h, 000h, 000h
        mov    dword ptr [ebp - 0ch], ecx
        db  08Bh, 080h, 0C0h, 065h, 000h, 000h
        or     eax, eax
        mov    dword ptr [ebp - 8], eax
        je     SHORT loc_4_E2B7
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 08Ch, 000h, 000h, 000h
        test   al, 1
        je     SHORT loc_4_E2B7
        test   al, 80h
        jne    SHORT loc_4_E2B7
        push   1
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        inc    eax
        push   eax
        push   ebx
        db  0E8h, 04Eh, 054h, 0FAh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 14h]
        or     esi, esi
        je     SHORT loc_4_E2AA
        mov    edi, dword ptr [ebp - 0ch]
        mov    ecx, 16h
        rep movsd
        mov    eax, dword ptr [ebp - 8]
        push   0
        push   dword ptr [eax + 14h]
        db  0E8h, 059h, 0A3h, 000h, 000h
        add    esp, 8
loc_4_E2AA:
        push   0
        push   dword ptr [ebp - 8]
        db  0E8h, 04Ch, 0A3h, 000h, 000h
        add    esp, 8
loc_4_E2B7:
        mov    eax, dword ptr [ebp - 4]
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 002h
        je     SHORT loc_4_E2EA
        mov    eax, dword ptr [ebx + 90h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [eax + 4]
        db  003h, 015h, 0B0h, 065h, 000h, 000h
        and    word ptr [ecx + 43h], 0bfffh
        movzx  eax, byte ptr [ebx + 89h]
        and    byte ptr [edx + eax + 1307h], 0feh
loc_4_E2EA:
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 088h, 08Ch, 000h, 000h, 000h
        test   cl, 80h
        je     SHORT loc_4_E35F
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_E35F
        and    cl, 7eh
        db  088h, 088h, 08Ch, 000h, 000h, 000h
        db  08Bh, 00Dh, 044h, 059h, 000h, 000h
        and    byte ptr [eax + ecx + 2fh], 0feh
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax + 0bh], 10h
        je     SHORT loc_4_E34B
        push   1
        push   43h
        movzx  eax, word ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   dword ptr [eax + 10h]
        push   dword ptr [ebp - 0ch]
        call   sub_4_14A00
        add    esp, 14h
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    eax, 2
        mov    word ptr [ecx + 4fh], ax
        jne    SHORT loc_4_E342
        inc    eax
loc_4_E342:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + ecx], 0
        jmp    SHORT loc_4_E35F
loc_4_E34B:
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        inc    eax
        push   eax
        push   ebx
        db  0E8h, 075h, 053h, 0FAh, 0FFh
        add    esp, 14h
loc_4_E35F:
        add    dword ptr [ebp - 10h], 4
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        db  039h, 005h, 044h, 066h, 000h, 000h
        ja     NEAR PTR loc_4_E23F
loc_4_E375:
        mov    dword ptr [ebx + 7ch], 0
        push   0
        push   ebx
        call   sub_4_4882
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   1
        movzx  ebx, byte ptr [esi + 89h]
        db  08Bh, 004h, 09Dh, 0C0h, 065h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        push   0
        push   eax
        lea    eax, [ebx + 1]
        push   eax
        push   esi
        db  0E8h, 017h, 053h, 0FAh, 0FFh
        add    esp, 14h
        mov    byte ptr [esi + 89h], bl
        db  0C6h, 083h, 06Ch, 000h, 000h, 000h, 0FFh
        db  08Bh, 03Dh, 084h, 06Eh, 000h, 000h
        or     edi, edi
        je     SHORT loc_4_E400
loc_4_E3D4:
        movzx  eax, word ptr [edi + 14h]
        cmp    eax, ebx
        jne    SHORT loc_4_E3F9
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 7ch], edi
        mov    dword ptr [eax], edi
        mov    edi, dword ptr [edi + 4]
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_4_E4EA
        add    esp, 8
        or     eax, eax
        je     SHORT loc_4_E3FC
        jmp    SHORT loc_4_E400
loc_4_E3F9:
        mov    edi, dword ptr [edi + 4]
loc_4_E3FC:
        or     edi, edi
        jne    SHORT loc_4_E3D4
loc_4_E400:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_E407:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 7ch]
        mov    ax, word ptr [esi + 16h]
        or     ax, ax
        jne    SHORT loc_4_E42A
        movzx  ecx, byte ptr [esi + 19h]
        movzx  eax, ax
        add    ecx, eax
        cmp    ecx, dword ptr [esi + 10h]
        jae    SHORT loc_4_E483
loc_4_E42A:
        mov    ebx, dword ptr [ebp + 0ch]
        or     ebx, ebx
        jne    SHORT loc_4_E43E
        mov    word ptr [edi + 1ah], 4
        mov    eax, 4
        jmp    SHORT loc_4_E485
loc_4_E43E:
        cmp    ebx, 2
        jl     SHORT loc_4_E483
        push   10h
        db  068h, 0D4h, 0B3h, 000h, 000h
        push   edi
        call   sub_4_4F1A
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_E469
        cmp    ebx, 2
        jne    SHORT loc_4_E469
        mov    dword ptr [edi + 7ch], 0
        movsx  eax, word ptr [edi + 1ah]
        jmp    SHORT loc_4_E485
loc_4_E469:
        cmp    ebx, 3
        jne    SHORT loc_4_E483
        mov    ax, word ptr [esi + 16h]
        or     ax, ax
        je     SHORT loc_4_E483
        movzx  eax, ax
        sub    dword ptr [esi + 10h], eax
        mov    word ptr [esi + 16h], 0
loc_4_E483:
        sub    eax, eax
loc_4_E485:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_E48A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 1
        mov    edi, dword ptr [esi + 7ch]
        movzx  eax, byte ptr [esi + 89h]
        mov    dword ptr [ebp - 4], eax
loc_4_E4A8:
        db  08Bh, 004h, 09Dh, 0BCh, 065h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_E4D4
        movzx  ecx, word ptr [edi + 14h]
        sub    ecx, ebx
        cmp    ecx, -1
        je     SHORT loc_4_E4D4
        cmp    dword ptr [eax], edi
        jne    SHORT loc_4_E4D4
        lea    eax, [ebx - 1]
        mov    byte ptr [esi + 89h], al
        push   esi
        db  0E8h, 04Ah, 069h, 0FAh, 0FFh
        add    esp, 4
loc_4_E4D4:
        inc    ebx
        cmp    ebx, 20h
        jle    SHORT loc_4_E4A8
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [esi + 89h], al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_E4EA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 7ch]
        mov    dword ptr [ebp - 8], eax
        mov    al, byte ptr [eax + 3eh]
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [eax + 20h]
        movzx  eax, byte ptr [esi + 89h]
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 080h
        je     SHORT loc_4_E526
        push   esi
        db  0E8h, 0FDh, 068h, 0FAh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_4_E633
loc_4_E526:
        db  08Bh, 01Dh, 084h, 06Eh, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_4_E55C
loc_4_E530:
        cmp    dword ptr [ebx + 20h], edi
        jne    SHORT loc_4_E555
        mov    dword ptr [esi + 7ch], ebx
        mov    al, byte ptr [ebx + 14h]
        mov    byte ptr [esi + 89h], al
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_4_E407
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_E60C
loc_4_E555:
        mov    ebx, dword ptr [ebx + 4]
        or     ebx, ebx
        jne    SHORT loc_4_E530
loc_4_E55C:
        db  08Bh, 01Dh, 084h, 06Eh, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_4_E5E1
loc_4_E566:
        cmp    dword ptr [ebx + 20h], edi
        jne    SHORT loc_4_E5DA
        cmp    dword ptr [ebx + 10h], 0
        je     SHORT loc_4_E5DA
        mov    dword ptr [esi + 7ch], ebx
        cmp    byte ptr [ebx + 19h], 1
        jbe    SHORT loc_4_E583
        push   esi
        call   sub_4_E48A
        add    esp, 4
loc_4_E583:
        mov    al, byte ptr [ebx + 14h]
        mov    byte ptr [esi + 89h], al
        push   0
        push   esi
        call   sub_4_13AC3
        add    esp, 8
        push   esi
        call   sub_4_E63C
        add    esp, 4
        inc    byte ptr [esi + 89h]
        push   1
        push   esi
        call   sub_4_4882
loc_4_E5AE:
        add    esp, 8
        dec    byte ptr [esi + 89h]
        mov    al, byte ptr [ebx + 19h]
        dec    al
        mov    byte ptr [ebx + 19h], al
        jne    SHORT loc_4_E5D2
        movzx  eax, word ptr [ebx + 14h]
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        mov    dword ptr [eax], 0
loc_4_E5D2:
        cmp    ebx, dword ptr [ebp - 8]
        je     SHORT loc_4_E5DA
        dec    dword ptr [ebx + 10h]
loc_4_E5DA:
        mov    ebx, dword ptr [ebx + 4]
        or     ebx, ebx
        jne    SHORT loc_4_E566
loc_4_E5E1:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 7ch], eax
        mov    al, byte ptr [eax + 14h]
        mov    byte ptr [esi + 89h], al
        push   esi
        db  0E8h, 008h, 063h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [esi + 7ch], 0
        je     SHORT loc_4_E621
        mov    word ptr [esi + 1ah], 4
        mov    eax, 4
        jmp    SHORT loc_4_E635
loc_4_E60C:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 7ch], eax
        mov    al, byte ptr [eax + 14h]
        mov    byte ptr [esi + 89h], al
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_E635
loc_4_E621:
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_4_E62D
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_E635
loc_4_E62D:
        mov    word ptr [esi + 1ah], 0
loc_4_E633:
        sub    eax, eax
loc_4_E635:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_E63C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [ecx + 89h]
        db  08Bh, 014h, 085h, 048h, 066h, 000h, 000h
        mov    eax, dword ptr [ecx + 90h]
        mov    esi, dword ptr [ecx + 7ch]
        add    esi, 44h
        mov    eax, dword ptr [eax + 4]
        lea    edi, [edx + eax + 43h]
        mov    ecx, 5
        rep movsd
        movsb
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_E671:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 034h, 085h, 0C0h, 065h, 000h, 000h
        db  068h, 0A8h, 06Eh, 000h, 000h
        cmp    dword ptr [esi], 1
        sbb    edi, edi
        neg    edi
        db  0E8h, 00Fh, 05Fh, 0FAh, 0FFh
        add    esp, 4
        or     edi, edi
        je     SHORT loc_4_E6A6
        cmp    dword ptr [esi], 0
        je     SHORT loc_4_E6A6
        call   sub_4_E6AF
        sub    eax, eax
        jmp    SHORT loc_4_E6AB
loc_4_E6A6:
        mov    eax, 1
loc_4_E6AB:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_4_E6AF:
        db  068h, 0A8h, 06Eh, 000h, 000h
        db  0E8h, 065h, 05Fh, 0FAh, 0FFh
        add    esp, 4
        ret
sub_4_E6BD:
        push   ebp
        mov    ebp, esp
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        movzx  eax, byte ptr [ebx + 89h]
        lea    esi, [eax*4]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebx + 90h]
        mov    eax, dword ptr [eax + 4]
        db  003h, 086h, 048h, 066h, 000h, 000h
        mov    dword ptr [ebp - 2ch], eax
        db  08Bh, 086h, 0C0h, 065h, 000h, 000h
        or     eax, eax
        mov    dword ptr [ebp - 28h], eax
        jne    SHORT loc_4_E725
        push   1
        push   18h
        db  0E8h, 0F6h, 09Eh, 000h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 28h], eax
        or     eax, eax
        jne    SHORT loc_4_E71C
        mov    word ptr [ebx + 1ah], 54h
        mov    eax, 54h
        jmp    NEAR PTR loc_4_EBAA
loc_4_E71C:
        mov    eax, dword ptr [ebp - 28h]
        db  089h, 086h, 0C0h, 065h, 000h, 000h
loc_4_E725:
        push   1
loc_4_E727:
        push   59h
        db  0E8h, 0CAh, 09Eh, 000h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    SHORT loc_4_E748
        mov    word ptr [ebx + 1ah], 54h
        mov    eax, 54h
        jmp    NEAR PTR loc_4_EBAA
loc_4_E748:
        sub    esi, esi
        mov    dword ptr [ebp - 1ch], esi
        push   dword ptr [ebp - 20h]
        call   sub_4_E671
        add    esp, 4
        cmp    eax, esi
        jne    SHORT loc_4_E772
        mov    eax, dword ptr [ebp - 28h]
        cmp    word ptr [ebx + 1ah], 1
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 7ch], eax
        sbb    eax, eax
        neg    eax
        jmp    NEAR PTR loc_4_EBAA
loc_4_E772:
        push   0
        push   dword ptr [ebp - 8]
        push   ebx
        call   sub_4_4080
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebx + 28h], eax
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [ebp - 24h], eax
        mov    ecx, 11h
        mov    eax, dword ptr [ebp - 28h]
        mov    edi, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0ah], 0
        sub    eax, eax
        rep stosd
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 18h], eax
        mov    byte ptr [ecx + 31h], 0ffh
        mov    byte ptr [ebx + 4], al
        mov    edi, 0ffh
        mov    dword ptr [ebx + 20h], ecx
        mov    dword ptr [ebx + 7ch], ecx
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp + 8], eax
        jne    SHORT loc_4_E80E
        mov    eax, dword ptr [ebp - 20h]
        db  08Ah, 080h, 06Ch, 000h, 000h, 000h
        cmp    al, 0ffh
        je     SHORT loc_4_E7E1
        movzx  eax, al
        push   eax
        push   0
        call   sub_4_2047
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    edi, eax
        jmp    SHORT loc_4_E7ED
loc_4_E7E1:
        mov    dword ptr [ebp - 4], 0ffh
        mov    edi, 0ffh
loc_4_E7ED:
        db  00Fh, 0B6h, 005h, 028h, 059h, 000h, 000h
        mov    dword ptr [ebp - 18h], eax
        or     edi, edi
        je     SHORT loc_4_E803
        cmp    edi, 0ffh
        jne    SHORT loc_4_E80E
loc_4_E803:
        mov    edi, 0ffh
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 4], eax
loc_4_E80E:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_E822
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_4_E822
        mov    ax, word ptr [ebp - 1ch]
        mov    word ptr [ebx + 1ah], ax
loc_4_E822:
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        db  083h, 0B8h, 0F4h, 055h, 000h, 000h, 000h
        je     SHORT loc_4_E8AE
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_4_ED16
        add    esp, 14h
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_4_E8E9
        cmp    esi, 11ch
        je     NEAR PTR loc_4_E8E9
        cmp    esi, 15h
        je     SHORT loc_4_E89B
        cmp    esi, 28h
        je     SHORT loc_4_E89B
        cmp    esi, 67h
        jne    SHORT loc_4_E86E
        mov    dword ptr [ebp - 1ch], esi
loc_4_E86E:
        cmp    esi, 0fh
        je     SHORT loc_4_E8B6
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_E8B6
        cmp    dword ptr [ebp - 4], edi
        jne    SHORT loc_4_E88E
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, edi
        jne    SHORT loc_4_E8A2
        dec    eax
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_4_E8A2
loc_4_E88E:
        dec    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], edi
        jne    SHORT loc_4_E8A2
        dec    dword ptr [ebp - 4]
        jmp    SHORT loc_4_E8A2
loc_4_E89B:
        mov    dword ptr [ebp - 4], 0
loc_4_E8A2:
        cmp    dword ptr [ebp - 4], 0
        jge    NEAR PTR loc_4_E80E
        jmp    SHORT loc_4_E8FE
loc_4_E8AE:
        mov    word ptr [ebx + 1ah], 0ffc0h
        jmp    SHORT loc_4_E8FE
loc_4_E8B6:
        push   0
        push   dword ptr [ebp - 8]
        db  0E8h, 040h, 09Dh, 000h, 000h
        add    esp, 8
        mov    dword ptr [ebx + 7ch], 0
        cmp    esi, 0fh
        jne    SHORT loc_4_E8D6
        mov    dword ptr [ebx + 1ch], 0
loc_4_E8D6:
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebx + 10h], eax
        push   2
        push   ebx
        call   sub_4_40D4
        add    esp, 8
        jmp    SHORT loc_4_E8FE
loc_4_E8E9:
        mov    eax, dword ptr [ebp - 4]
        shl    eax, 4
        db  08Ah, 088h, 0F8h, 055h, 000h, 000h
        mov    eax, dword ptr [ebp - 20h]
        db  088h, 088h, 06Ch, 000h, 000h, 000h
loc_4_E8FE:
        or     esi, esi
        jne    NEAR PTR loc_4_EB93
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_4_EB93
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, 1
        mov    cl, byte ptr [ebp - 20h]
        shl    eax, cl
        push   eax
        call   sub_4_4681
        add    esp, 4
        mov    dword ptr [ebp - 38h], eax
        push   dword ptr [ebp - 24h]
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_4_439D
        add    esp, 10h
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_4_E97D
        push   1
        push   104h
        db  0E8h, 09Ch, 09Ch, 000h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        or     eax, eax
        jne    SHORT loc_4_E971
        mov    word ptr [ebx + 1ah], 8
        jmp    NEAR PTR loc_4_EB93
loc_4_E971:
        mov    edi, dword ptr [ebp - 14h]
        mov    esi, ebx
        mov    ecx, 41h
        rep movsd
loc_4_E97D:
        mov    eax, dword ptr [ebp - 20h]
        cmp    dword ptr [ebp - 30h], eax
        jne    SHORT loc_4_E98E
        cmp    eax, dword ptr [ebp - 10h]
        je     NEAR PTR loc_4_EAED
loc_4_E98E:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_4_EAED
        mov    eax, dword ptr [ebp - 20h]
        cmp    eax, dword ptr [ebp - 10h]
        je     NEAR PTR loc_4_EA6C
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 10h]
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_4_E9E2
        mov    dword ptr [ebx + 7ch], eax
        mov    si, word ptr [ebx + 1ah]
        push   1
        push   ebx
        call   sub_4_E4EA
        add    esp, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ebx + 1ah], si
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 1ch], eax
        mov    dword ptr [ebx + 7ch], ecx
loc_4_E9E2:
        mov    eax, dword ptr [ebp - 34h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [ecx + 14h], ax
        mov    eax, dword ptr [ebp - 10h]
        db  08Bh, 00Ch, 085h, 048h, 066h, 000h, 000h
        mov    eax, dword ptr [ebx + 90h]
        mov    edi, dword ptr [ebp - 8]
        add    edi, 44h
        mov    eax, dword ptr [eax + 4]
        lea    esi, [ecx + eax + 43h]
        mov    ecx, 5
        rep movsd
        movsb
        mov    ecx, dword ptr [ebp - 8]
        cmp    byte ptr [ecx + 19h], 1
        jbe    SHORT loc_4_EA6C
        movzx  eax, byte ptr [ebx + 89h]
        mov    dword ptr [ebp - 3ch], eax
        mov    esi, 1
loc_4_EA2E:
        db  08Bh, 014h, 0B5h, 0BCh, 065h, 000h, 000h
        or     edx, edx
        je     SHORT loc_4_EA5D
        mov    ecx, dword ptr [ebp - 8]
        movzx  eax, word ptr [ecx + 14h]
        sub    eax, esi
        cmp    eax, -1
        je     SHORT loc_4_EA5D
        cmp    dword ptr [edx], ecx
        jne    SHORT loc_4_EA5D
        lea    eax, [esi - 1]
        mov    byte ptr [ebx + 89h], al
        push   ebx
        db  0E8h, 0C1h, 063h, 0FAh, 0FFh
        add    esp, 4
loc_4_EA5D:
        inc    esi
        cmp    esi, 20h
        jle    SHORT loc_4_EA2E
        mov    al, byte ptr [ebp - 3ch]
        mov    byte ptr [ebx + 89h], al
loc_4_EA6C:
        mov    edx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [edx + 28h], eax
        mov    byte ptr [edx + 89h], al
        mov    byte ptr [edx + 4], 5
        mov    eax, dword ptr [ebp - 10h]
        mov    word ptr [edx + 18h], ax
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [edx + 24h], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + 20h], ecx
        push   dword ptr [ecx + 1ch]
        push   0
        call   sub_4_2047
        add    esp, 8
        push   0
        push   dword ptr [ebp - 14h]
        shl    eax, 4
        push   1eh
        db  0FFh, 0B0h, 0F4h, 055h, 000h, 000h
        db  0E8h, 0DCh, 050h, 0FAh, 0FFh
        add    esp, 10h
        mov    eax, dword ptr [ebp - 20h]
        cmp    eax, dword ptr [ebp - 10h]
        je     SHORT loc_4_EAED
        mov    edx, dword ptr [ebp - 14h]
        push   1
        mov    al, byte ptr [edx + 89h]
        inc    al
        mov    byte ptr [edx + 89h], al
        push   0
        push   dword ptr [ebp - 34h]
        movzx  eax, al
        push   eax
        push   edx
        db  0E8h, 0F0h, 04Bh, 0FAh, 0FFh
        add    esp, 14h
        mov    edx, dword ptr [ebp - 14h]
        dec    byte ptr [edx + 89h]
loc_4_EAED:
        mov    eax, 1
        mov    cl, byte ptr [ebp - 10h]
        shl    eax, cl
        or     eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_4_4681
        add    esp, 4
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_4_EB75
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebx + 1ch]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 14h]
        call   sub_4_EBB1
        add    esp, 18h
        mov    eax, 1
        mov    cl, byte ptr [ebp - 10h]
        shl    eax, cl
        or     eax, dword ptr [ebp - 0ch]
        mov    edx, 1
        mov    cl, byte ptr [ebp - 30h]
        shl    edx, cl
        not    edx
        and    eax, edx
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 10h], 1
        or     eax, eax
        je     SHORT loc_4_EB75
        mov    edx, 1
        mov    ecx, 4
        sub    esi, esi
loc_4_EB58:
        test   dword ptr [ebp - 0ch], edx
        je     SHORT loc_4_EB6A
        mov    eax, dword ptr [ebp - 10h]
        db  08Bh, 004h, 085h, 0BCh, 065h, 000h, 000h
        or     byte ptr [eax + 0bh], cl
loc_4_EB6A:
        inc    dword ptr [ebp - 10h]
        shr    dword ptr [ebp - 0ch], 1
        cmp    dword ptr [ebp - 0ch], esi
        jne    SHORT loc_4_EB58
loc_4_EB75:
        push   dword ptr [ebp - 38h]
        call   sub_4_4695
        add    esp, 4
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_4_EB93
        push   0
        push   dword ptr [ebp - 14h]
        db  0E8h, 070h, 09Ah, 000h, 000h
        add    esp, 8
loc_4_EB93:
        call   sub_4_E6AF
        cmp    word ptr [ebx + 1ah], 11ch
        jne    SHORT loc_4_EBA6
        mov    word ptr [ebx + 1ah], 0
loc_4_EBA6:
        movsx  eax, word ptr [ebx + 1ah]
loc_4_EBAA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_EBB1:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_4_ED0F
        db  0C7h, 045h, 0F0h, 0C0h, 065h, 000h, 000h
        db  0C7h, 045h, 0F4h, 048h, 066h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
loc_4_EBDC:
        test   byte ptr [ebp + 0ch], 1
        je     NEAR PTR loc_4_ECF7
        mov    eax, dword ptr [esi + 90h]
        push   1
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [ebp - 0ch]
        push   59h
        add    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], ecx
        mov    edi, dword ptr [eax]
        db  0E8h, 0F3h, 099h, 000h, 000h
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_4_ED0F
        mov    byte ptr [edi + 0ah], 0
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 11h
        rep stosd
        mov    byte ptr [ebx + 31h], 0ffh
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 28h], eax
        mov    byte ptr [esi + 89h], al
        mov    byte ptr [esi + 4], 4
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 20h], ebx
        mov    dword ptr [esi + 7ch], ebx
        push   dword ptr [ebp + 14h]
        push   ebx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_4_ED16
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_4_EC7F
        mov    ecx, dword ptr [ebp + 10h]
        shl    ecx, 4
        db  083h, 0B9h, 0F8h, 055h, 000h, 000h, 000h
        je     SHORT loc_4_EC7B
        push   dword ptr [ebp + 14h]
        push   ebx
        push   dword ptr [ebp - 8]
        push   0
        push   esi
        call   sub_4_ED16
        add    esp, 14h
        jmp    SHORT loc_4_ECF7
loc_4_EC7B:
        or     eax, eax
        jne    SHORT loc_4_ECF7
loc_4_EC7F:
        mov    eax, dword ptr [ebp + 1ch]
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        mov    ecx, dword ptr [eax]
        mov    al, byte ptr [ecx + 30h]
        or     al, al
        je     SHORT loc_4_ECF7
        mov    bl, byte ptr [esi + 0a9h]
        mov    di, word ptr [esi + 0ach]
        dec    al
        mov    byte ptr [esi + 0a9h], al
        mov    word ptr [esi + 0ach], 0
        test   byte ptr [ecx + 32h], 20h
        je     SHORT loc_4_ECBF
        mov    word ptr [esi + 0ach], 1
loc_4_ECBF:
        test   byte ptr [ecx + 32h], 8
        je     SHORT loc_4_ECCD
        or     word ptr [esi + 0ach], 2
loc_4_ECCD:
        push   esi
        db  0E8h, 02Eh, 075h, 0FAh, 0FFh
        add    esp, 4
        push   1
        push   esi
        call   sub_4_1331B
        add    esp, 8
        push   esi
        db  0E8h, 0D7h, 075h, 0FAh, 0FFh
        add    esp, 4
        mov    byte ptr [esi + 0a9h], bl
        mov    word ptr [esi + 0ach], di
loc_4_ECF7:
        add    dword ptr [ebp - 10h], 4
        add    dword ptr [ebp - 0ch], 4
        inc    dword ptr [ebp - 4]
        shr    dword ptr [ebp + 0ch], 1
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_4_EBDC
loc_4_ED0F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_ED16:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 18h]
        movzx  esi, byte ptr [ebx + 4]
        movzx  ecx, byte ptr [ebx + 89h]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        shl    eax, 4
        mov    dword ptr [ebp - 0ch], eax
        push   0
        push   ebx
        push   1eh
        db  0FFh, 0B0h, 0F4h, 055h, 000h, 000h
        db  0E8h, 041h, 04Eh, 0FAh, 0FFh
        add    esp, 10h
        mov    edi, eax
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [ebp - 1ch], eax
        or     edi, edi
        jne    SHORT loc_4_ED79
        or     esi, esi
        jne    SHORT loc_4_ED79
        mov    ecx, dword ptr [ebp - 8]
        db  08Bh, 004h, 08Dh, 0C0h, 065h, 000h, 000h
        mov    dword ptr [ebp - 18h], 1
        test   byte ptr [eax + 0bh], 4
        jne    SHORT loc_4_ED80
loc_4_ED79:
        mov    dword ptr [ebp - 18h], 0
loc_4_ED80:
        or     edi, edi
        jne    NEAR PTR loc_4_F06F
        push   dword ptr [ebp + 14h]
        call   sub_4_23B6
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_F06F
        mov    esi, dword ptr [ebp + 10h]
        add    esi, 43h
        mov    edi, dword ptr [ebp + 14h]
        add    edi, 44h
        mov    ecx, 5
        rep movsd
        movsb
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        db  08Bh, 080h, 0F8h, 055h, 000h, 000h
        mov    dword ptr [ecx + 1ch], eax
        mov    word ptr [ecx], 7253h
        mov    byte ptr [ecx + 1bh], 10h
        mov    byte ptr [ecx + 1ah], 1
        mov    ecx, dword ptr [ebx + 7ch]
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp + 14h]
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [ecx + 8], eax
        test   byte ptr [ebx + 6], 1
        je     SHORT loc_4_EDF1
        db  080h, 03Dh, 040h, 000h, 000h, 000h, 000h
        je     SHORT loc_4_EDF1
        or     word ptr [ecx + 32h], 2
loc_4_EDF1:
        mov    ecx, dword ptr [ebp + 14h]
        sub    edx, edx
        mov    dword ptr [ecx + 10h], 1
        movzx  ax, byte ptr [ebx + 89h]
        mov    word ptr [ecx + 14h], ax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 20h], eax
        mov    ecx, dword ptr [ebp - 8]
        db  08Bh, 034h, 08Dh, 0C0h, 065h, 000h, 000h
        cmp    esi, edx
        je     SHORT loc_4_EE1F
        mov    edx, dword ptr [esi]
loc_4_EE1F:
        mov    ecx, dword ptr [ebp - 8]
        db  0F6h, 081h, 08Ch, 000h, 000h, 000h, 080h
        je     SHORT loc_4_EE73
        mov    edi, 1
loc_4_EE30:
        db  08Bh, 004h, 0BDh, 0BCh, 065h, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_EE6D
        movzx  ecx, byte ptr [eax + 0fh]
        sub    ecx, dword ptr [ebp - 8]
        cmp    ecx, 1
        jne    SHORT loc_4_EE6D
        mov    esi, eax
        lea    eax, [edi - 1]
        mov    edx, dword ptr [esi]
        mov    byte ptr [ebx + 89h], al
        movzx  ecx, al
        mov    eax, dword ptr [ebx + 90h]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [eax + 4]
        db  003h, 004h, 08Dh, 048h, 066h, 000h, 000h
        mov    dword ptr [ebp + 10h], eax
loc_4_EE6D:
        inc    edi
        cmp    edi, 20h
        jle    SHORT loc_4_EE30
loc_4_EE73:
        or     edx, edx
        je     SHORT loc_4_EEA8
        mov    dword ptr [ebx + 7ch], edx
        cmp    byte ptr [edx + 19h], 1
        mov    di, word ptr [ebx + 1ah]
        jbe    SHORT loc_4_EE90
        push   ebx
        push   dword ptr [ebp + 14h]
        db  0E8h, 053h, 050h, 0FAh, 0FFh
        add    esp, 8
loc_4_EE90:
        push   1
        push   ebx
        call   sub_4_E4EA
        add    esp, 8
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ebx + 1ah], di
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 1ch], eax
loc_4_EEA8:
        cmp    dword ptr [ebp - 18h], 0
        je     SHORT loc_4_EEDE
        push   0
        push   ebx
        mov    eax, dword ptr [ebp - 1ch]
        push   28h
        push   dword ptr [eax + 28h]
        db  0E8h, 0D2h, 04Ch, 0FAh, 0FFh
        mov    ecx, dword ptr [ebp + 14h]
        add    esp, 10h
        movzx  eax, byte ptr [ecx + 3]
        db  0C7h, 004h, 085h, 0B0h, 083h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    word ptr [ebx + 1ah], 0fh
        jmp    NEAR PTR loc_4_F04A
loc_4_EEDE:
        mov    ecx, dword ptr [ebp + 14h]
        mov    dword ptr [esi], ecx
        db  0A1h, 084h, 06Eh, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  089h, 00Dh, 084h, 06Eh, 000h, 000h
        mov    dword ptr [ebx + 7ch], ecx
        mov    al, byte ptr [ebx + 89h]
        inc    al
        mov    byte ptr [ebx + 89h], al
        push   1
        push   ecx
        push   esi
        movzx  eax, al
        push   eax
        push   ebx
        db  0E8h, 0C1h, 047h, 0FAh, 0FFh
        add    esp, 14h
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [ebx + 4], 2
        db  083h, 0B8h, 0F8h, 055h, 000h, 000h, 000h
        je     NEAR PTR loc_4_F02C
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 14h], eax
        or     byte ptr [ebx + 8bh], 1
        mov    eax, dword ptr [ebx + 94h]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + ecx*4 + 50h]
        mov    dword ptr [ebp - 14h], eax
        push   ebx
        call   sub_4_15558
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_EFF4
        push   0
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   20h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        db  0E8h, 028h, 04Ch, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_EFF4
        mov    eax, dword ptr [ebx + 0ch]
        mov    ax, word ptr [eax]
        cmp    ax, 4
        je     SHORT loc_4_EFF4
        movzx  eax, ax
        sub    eax, 4
        push   eax
        call   sub_4_6190
        add    esp, 4
        mov    dword ptr [ebp - 10h], eax
        or     eax, eax
        je     NEAR PTR loc_4_F023
        mov    edx, dword ptr [ebx + 94h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [edx + ecx*4 + 50h], eax
        push   dword ptr [ebx + 2ch]
        push   dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebx + 8ch]
        add    eax, 208h
        push   eax
        call   sub_4_15ECC
        add    esp, 0ch
        mov    esi, dword ptr [ebx + 8ch]
        mov    edi, dword ptr [ebp - 10h]
        add    edi, 4
        add    esi, 20ah
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   dword ptr [ebp - 14h]
        call   sub_4_61C3
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        or     byte ptr [eax + 1], 1
        jmp    SHORT loc_4_F023
loc_4_EFF4:
        mov    eax, dword ptr [ebp + 10h]
        push   dword ptr [ebp - 14h]
        mov    word ptr [eax + 4fh], 2
        mov    byte ptr [eax + 3], 0
        call   sub_4_61C3
        add    esp, 4
        mov    eax, dword ptr [ebx + 94h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [eax + ecx*4 + 50h], 0
        mov    word ptr [ebx + 1ah], 0
loc_4_F023:
        push   ebx
        db  0E8h, 09Ch, 048h, 0FAh, 0FFh
        add    esp, 4
loc_4_F02C:
        mov    eax, dword ptr [ebp - 0ch]
        db  083h, 0B8h, 0F8h, 055h, 000h, 000h, 000h
        je     SHORT loc_4_F044
        push   dword ptr [ebp + 10h]
        push   ebx
        call   sub_4_25C4
        add    esp, 8
loc_4_F044:
        dec    byte ptr [ebx + 89h]
loc_4_F04A:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 10h], 0
        db  083h, 0B8h, 0F8h, 055h, 000h, 000h, 001h
        sbb    eax, eax
        and    eax, 2
        push   eax
        push   ebx
        call   sub_4_40D4
        add    esp, 8
        jmp    NEAR PTR loc_4_F129
loc_4_F06F:
        cmp    edi, 11ch
        jne    NEAR PTR loc_4_F129
        push   0
        push   dword ptr [ebp + 14h]
        db  0E8h, 07Bh, 095h, 000h, 000h
        add    esp, 8
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [ebp - 0ch]
        db  083h, 0B8h, 0F8h, 055h, 000h, 000h, 001h
        sbb    eax, eax
        and    eax, 2
        push   eax
        push   ebx
        call   sub_4_40D4
        add    esp, 8
        db  08Bh, 035h, 084h, 06Eh, 000h, 000h
        or     esi, esi
        je     SHORT loc_4_F114
loc_4_F0B1:
        mov    eax, dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 20h], eax
        jne    SHORT loc_4_F10D
        movzx  eax, word ptr [esi + 14h]
        db  08Bh, 03Ch, 085h, 0C0h, 065h, 000h, 000h
        mov    eax, dword ptr [edi]
        cmp    esi, eax
        je     SHORT loc_4_F10D
        or     eax, eax
        je     SHORT loc_4_F0EC
        mov    dword ptr [ebx + 7ch], eax
        mov    ax, word ptr [ebx + 1ah]
        mov    word ptr [ebp - 2], ax
        push   1
        push   ebx
        call   sub_4_E4EA
        add    esp, 8
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebx + 1ah], ax
loc_4_F0EC:
        mov    dword ptr [ebx + 7ch], esi
        push   ebx
        db  0E8h, 086h, 05Ch, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [edi], esi
        push   1
        push   esi
        push   edi
        movzx  eax, word ptr [esi + 14h]
        inc    eax
        push   eax
IFSMgr_CompleteAsync:
        push   ebx
        db  0E8h, 0C7h, 045h, 0FAh, 0FFh
        add    esp, 14h
loc_4_F10D:
        mov    esi, dword ptr [esi + 4]
        or     esi, esi
        jne    SHORT loc_4_F0B1
loc_4_F114:
        mov    ecx, dword ptr [ebp - 8]
        db  08Bh, 004h, 08Dh, 0C0h, 065h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 7ch], eax
        mov    word ptr [ebx + 1ah], 11ch
loc_4_F129:
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
_PTEXT ends
        end
