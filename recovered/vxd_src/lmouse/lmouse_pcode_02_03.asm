; lmouse_pcode_02_03.asm -- LMOUSE object 2 segment _PTEXT  (part 3/3)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_861A:
        push   ebp
        mov    ebp, esp
        push   80000002h
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_8545
        mov    esp, ebp
        pop    ebp
        ret
sub_2_863A:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_2_8662
        db  083h, 03Dh, 02Ch, 000h, 000h, 000h, 000h
        je     SHORT loc_2_8662
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_2_85FA
        jmp    SHORT loc_2_8676
loc_2_8662:
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_2_861A
loc_2_8676:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_867A:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_2_869C
        db  083h, 03Dh, 02Ch, 000h, 000h, 000h, 000h
        je     SHORT loc_2_869C
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_2_852B
        jmp    SHORT loc_2_86AA
loc_2_869C:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   sub_2_8511
loc_2_86AA:
        mov    esp, ebp
        pop    ebp
        ret
        db  0B8h, 001h, 000h, 000h, 000h, 0C3h, 0A1h, 02Ch, 000h, 000h, 000h, 0C3h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 02Ch, 000h, 000h, 000h
        pop    ebp
        ret
        db  000h
sub_2_86C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    ebx, 1
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        call   sub_2_0798
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_2_873F
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_2_6E78
        add    esp, 0ch
        mov    word ptr [ebp - 8], 4300h
        mov    eax, esi
        and    ax, 0fh
        mov    word ptr [ebp - 10h], ax
        shr    esi, 4
        mov    word ptr [ebp - 4], si
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        push   eax
        push   21h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_8735:
        add    esp, 10h
        mov    ebx, eax
        call   sub_2_07AB
loc_2_873F:
        and    ebx, 1
        cmp    ebx, 1
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8750:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_890E
        call   sub_2_03D1
        or     eax, eax
        je     NEAR PTR loc_2_890E
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0B5h, 000h, 001h, 000h
        jb     NEAR PTR loc_2_890E
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_87AF
loc_2_8799:
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, byte ptr [eax]
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_8799
loc_2_87AF:
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [ecx - 1]
        or     esi, esi
        je     NEAR PTR loc_2_890E
        push   1
        lea    eax, [esi + 2]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_87D3:
        add    esp, 8
        db  0A3h, 008h, 000h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_2_890E
        inc    esi
        push   esi
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_2_6E78
        add    esp, 0ch
        db  08Bh, 03Dh, 008h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  0A1h, 008h, 000h, 000h, 000h
        cmp    byte ptr [ecx + eax - 2], 3bh
        jne    SHORT loc_2_8825
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  0A1h, 008h, 000h, 000h, 000h
        mov    byte ptr [ecx + eax - 2], 0
loc_2_8825:
        db  08Bh, 03Dh, 008h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  0A1h, 008h, 000h, 000h, 000h
        cmp    byte ptr [ecx + eax - 2], 5ch
        jne    SHORT loc_2_8859
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  0A1h, 008h, 000h, 000h, 000h
        mov    byte ptr [ecx + eax - 2], 0
loc_2_8859:
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        call   sub_2_86C8
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_889D
        db  08Bh, 03Dh, 008h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  0A1h, 008h, 000h, 000h, 000h
        cmp    byte ptr [ecx + eax - 2], 5ch
        je     SHORT loc_2_88F7
        db  0BEh, 00Ch, 000h, 000h, 000h
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        movsw
        jmp    SHORT loc_2_88F7
loc_2_889D:
        push   0
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_88AA:
        add    esp, 8
        db  0C7h, 005h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0B7h, 000h, 001h, 000h
        mov    dword ptr [ebp - 4], edx
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    esi, [ecx - 1]
        or     esi, esi
        je     SHORT loc_2_88F7
        inc    esi
        push   1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_88DE:
        add    esp, 8
        db  0A3h, 008h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_88F7
        push   esi
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_2_6E78
        add    esp, 0ch
loc_2_88F7:
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_890E
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        call   sub_2_7058
        add    esp, 4
loc_2_890E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  0A1h, 008h, 000h, 000h, 000h, 00Bh, 0C0h, 074h, 015h, 06Ah, 000h, 050h, 0E8h, 000h, 000h, 000h
        db  000h, 083h, 0C4h, 008h, 0C7h, 005h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C3h
sub_2_8933:
        push   ebp
        mov    ebp, esp
        push   edi
        db  08Bh, 015h, 008h, 000h, 000h, 000h
        or     edx, edx
        je     SHORT loc_2_897E
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        je     SHORT loc_2_897E
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, dword ptr [ebp + 0ch]
        cmp    ecx, eax
        jae    SHORT loc_2_8977
        push   eax
        push   edx
        push   dword ptr [ebp + 8]
        call   sub_2_6E78
        add    esp, 0ch
        sub    eax, eax
        jmp    SHORT loc_2_8983
loc_2_8977:
        mov    eax, 0fffffff8h
        jmp    SHORT loc_2_8983
loc_2_897E:
        mov    eax, 0ffffffebh
loc_2_8983:
        pop    edi
        pop    ebp
        ret
        db  000h, 000h
sub_2_8988:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        push   dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_6E78
        add    esp, 0ch
        mov    byte ptr [esi + edi - 1], 0
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_89AB:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        call   sub_2_0798
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_8A2F
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_2_8988
        add    esp, 0ch
        mov    byte ptr [ebp - 7], 3dh
        mov    al, byte ptr [ebp + 0ch]
        mov    byte ptr [ebp - 8], al
        mov    eax, ebx
        and    ax, 0fh
        mov    word ptr [ebp - 10h], ax
        shr    ebx, 4
        mov    word ptr [ebp - 4], bx
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        push   eax
        push   21h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_8A1C:
        add    esp, 10h
        mov    esi, eax
        mov    eax, dword ptr [ebp + 10h]
        movzx  ecx, word ptr [ebp - 8]
        mov    dword ptr [eax], ecx
        call   sub_2_07AB
loc_2_8A2F:
        test   esi, 1
        mov    eax, 0
        je     SHORT loc_2_8A40
        movzx  eax, word ptr [ebp - 8]
loc_2_8A40:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8A47:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        mov    byte ptr [ebp - 3], 3eh
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 10h], ax
        lea    eax, [ebp - 20h]
        push   eax
        push   eax
        push   21h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_8A64:
        add    esp, 0ch
        test   al, 1
        mov    eax, 0
        je     SHORT loc_2_8A74
        movzx  eax, word ptr [ebp - 4]
loc_2_8A74:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8A78:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    edi, 1
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        call   sub_2_0798
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_8AEA
        mov    byte ptr [ebp - 7], 3fh
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 14h], ax
        mov    word ptr [ebp - 0ch], si
        mov    eax, ebx
        and    ax, 0fh
        mov    word ptr [ebp - 10h], ax
        mov    eax, ebx
        shr    eax, 4
        mov    word ptr [ebp - 4], ax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        push   eax
        push   21h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_8ACA:
        add    esp, 10h
        mov    edi, eax
        mov    eax, dword ptr [ebp + 14h]
        movzx  ecx, word ptr [ebp - 8]
        mov    dword ptr [eax], ecx
        push   esi
        push   ebx
        push   dword ptr [ebp + 0ch]
        call   sub_2_8988
        add    esp, 0ch
        call   sub_2_07AB
loc_2_8AEA:
        test   edi, 1
        mov    eax, 0
        je     SHORT loc_2_8AFB
        movzx  eax, word ptr [ebp - 8]
loc_2_8AFB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8B02:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        mov    byte ptr [ebp - 3], 42h
        mov    al, byte ptr [ebp + 10h]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, eax
        shr    ecx, 10h
        mov    word ptr [ebp - 8], cx
        mov    word ptr [ebp - 0ch], ax
        lea    eax, [ebp - 20h]
        push   eax
        push   eax
        push   21h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_8B35:
        add    esp, 0ch
        test   al, 1
        mov    eax, 0ffffffffh
        jne    SHORT loc_2_8B4E
        movzx  eax, word ptr [ebp - 0ch]
        shl    eax, 10h
        movzx  ecx, word ptr [ebp - 4]
        or     eax, ecx
loc_2_8B4E:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8B52:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        push   1
        push   ebx
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        call   sub_2_8B02
        add    esp, 0ch
        cmp    eax, ebx
        jne    SHORT loc_2_8B76
        db  089h, 01Dh, 08Ch, 000h, 000h, 000h
loc_2_8B76:
        push   0
        db  0FFh, 035h, 08Ch, 000h, 000h, 000h
        push   edi
        call   sub_2_8B02
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_2_8BFE
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    eax
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   edi
        call   sub_2_8A78
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_2_8BFE
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_8BFE
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [esi + eax], 0
        push   0ah
        push   esi
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_8BC5
        mov    byte ptr [eax], 0
loc_2_8BC5:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  001h, 00Dh, 08Ch, 000h, 000h, 000h
        push   0dh
        push   esi
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_8BEA
        mov    byte ptr [eax], 0
loc_2_8BEA:
        push   1ah
        push   esi
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_8BFC
        mov    byte ptr [eax], 0
loc_2_8BFC:
        mov    ebx, esi
loc_2_8BFE:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8C07:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, esi
        movsx  eax, byte ptr [esi]
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_8C34
loc_2_8C22:
        movsx  eax, byte ptr [ebx + 1]
        inc    ebx
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_8C22
loc_2_8C34:
        cmp    byte ptr [esi], 0
        je     SHORT loc_2_8C55
        cmp    ebx, esi
        je     SHORT loc_2_8C55
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   ebx
        push   esi
        call   sub_2_8988
        add    esp, 0ch
loc_2_8C55:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        je     SHORT loc_2_8C9C
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [esi + ecx - 2]
        movsx  eax, byte ptr [edi]
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_8C98
loc_2_8C86:
        movsx  eax, byte ptr [edi - 1]
        dec    edi
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_8C86
loc_2_8C98:
        mov    byte ptr [edi + 1], 0
loc_2_8C9C:
        movsx  eax, byte ptr [esi]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_8CA4:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_8C07
        add    esp, 4
        push   3bh
        push   esi
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_8CC6
        mov    byte ptr [eax], 0
loc_2_8CC6:
        push   esi
        call   sub_2_8C07
        add    esp, 4
        movsx  eax, byte ptr [esi]
        pop    esi
        pop    ebp
        ret
sub_2_8CD5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_2_8CDF:
        push   esi
        push   80h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_8B52
        add    esp, 0ch
        or     eax, eax
        db  0A1h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_8D0E
        push   eax
        call   sub_2_8CA4
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_8CDF
        mov    edi, 1
loc_2_8D0E:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_8D14:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        sub    esi, esi
        cmp    byte ptr [eax], 5bh
        jne    SHORT loc_2_8D6E
        push   5dh
        push   eax
        call   sub_2_7000
        add    esp, 8
        cmp    eax, esi
        je     SHORT loc_2_8D6E
        mov    esi, 1
        push   28h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, esi
        push   eax
        db  0FFh, 035h, 004h, 000h, 000h, 000h
        call   sub_2_8988
        add    esp, 0ch
        push   5dh
        db  0FFh, 035h, 004h, 000h, 000h, 000h
        call   sub_2_7000
        add    esp, 8
        mov    byte ptr [eax], 0
        db  0FFh, 035h, 004h, 000h, 000h, 000h
        call   sub_2_8C07
        add    esp, 4
loc_2_8D6E:
        mov    eax, esi
        pop    esi
        ret
sub_2_8D72:
        push   esi
        push   edi
        sub    esi, esi
        push   3dh
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_7000
        add    esp, 8
        cmp    eax, esi
        je     NEAR PTR loc_2_8E24
        push   28h
        push   3dh
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_7000
        add    esp, 8
        inc    eax
        push   eax
        db  0FFh, 035h, 00Ch, 000h, 000h, 000h
        call   sub_2_8988
        add    esp, 0ch
        db  0FFh, 035h, 00Ch, 000h, 000h, 000h
        call   sub_2_8C07
        add    esp, 4
        push   3dh
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_2_7000
        add    esp, 8
        mov    byte ptr [eax], 0
        push   28h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        call   sub_2_8988
        add    esp, 0ch
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        call   sub_2_8C07
        add    esp, 4
        db  08Bh, 03Dh, 008h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        je     SHORT loc_2_8E22
        db  08Bh, 03Dh, 00Ch, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    esi, 1
        jne    SHORT loc_2_8E24
loc_2_8E22:
        sub    esi, esi
loc_2_8E24:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
sub_2_8E29:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        db  0BBh, 028h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
loc_2_8E39:
        push   dword ptr [ebx]
        push   esi
        call   sub_2_6EA0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_8E55
        add    ebx, 4
        db  081h, 0FBh, 068h, 000h, 000h, 000h
        jb     SHORT loc_2_8E39
        jmp    SHORT loc_2_8E5A
loc_2_8E55:
        mov    edi, 1
loc_2_8E5A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_8E61:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, 1
        db  0BBh, 080h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
loc_2_8E74:
        push   dword ptr [ebx]
        push   esi
        call   sub_2_6EA0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_8E90
        add    ebx, 4
        db  081h, 0FBh, 08Ch, 000h, 000h, 000h
        jb     SHORT loc_2_8E74
        jmp    SHORT loc_2_8E92
loc_2_8E90:
        sub    edi, edi
loc_2_8E92:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_8E99:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   5ch
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_8EE1
        db  0BFh, 010h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
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
loc_2_8EE1:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_8EE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp - 8]
loc_2_8EF7:
        or     ebx, ebx
        je     SHORT loc_2_8F1E
        cmp    ebx, 1
        je     SHORT loc_2_8F5D
        cmp    ebx, 2
        je     NEAR PTR loc_2_8FB1
        push   dword ptr [ebp - 4]
        call   sub_2_8A47
        add    esp, 4
        mov    ebx, 4
        jmp    NEAR PTR loc_2_9027
loc_2_8F1E:
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_2_89AB
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_8F40
        mov    edi, 2
        mov    ebx, 4
        jmp    NEAR PTR loc_2_9027
loc_2_8F40:
        push   dword ptr [ebp - 4]
        call   sub_2_8CD5
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_2_9022
        mov    ebx, 1
        jmp    NEAR PTR loc_2_9027
loc_2_8F5D:
        call   sub_2_8D14
        or     eax, eax
        jne    SHORT loc_2_8F7A
        push   dword ptr [ebp - 4]
        call   sub_2_8CD5
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_8F5D
        mov    ebx, 3
loc_2_8F7A:
        db  0FFh, 035h, 004h, 000h, 000h, 000h
        call   sub_2_8E29
        add    esp, 4
        or     eax, eax
        push   dword ptr [ebp - 4]
        je     SHORT loc_2_8F9C
        call   sub_2_8CD5
        add    esp, 4
        jmp    NEAR PTR loc_2_9027
loc_2_8F9C:
        call   sub_2_8CD5
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_8FAA
        jmp    SHORT loc_2_9022
loc_2_8FAA:
        mov    ebx, 2
        jmp    SHORT loc_2_9027
loc_2_8FB1:
        call   sub_2_8D14
        or     eax, eax
        jne    SHORT loc_2_9039
        call   sub_2_8D72
        or     eax, eax
        db  0A1h, 008h, 000h, 000h, 000h
        je     SHORT loc_2_9013
        push   eax
        call   sub_2_8E29
        add    esp, 4
        or     eax, eax
        db  0A1h, 004h, 000h, 000h, 000h
        je     SHORT loc_2_8FDC
        jmp    SHORT loc_2_9013
loc_2_8FDC:
        push   eax
        call   sub_2_8E61
        add    esp, 4
        or     eax, eax
        db  0A1h, 004h, 000h, 000h, 000h
        je     SHORT loc_2_8FFC
        push   eax
        call   sub_2_8E99
        add    esp, 4
        db  0A1h, 004h, 000h, 000h, 000h
loc_2_8FFC:
        db  0FFh, 035h, 00Ch, 000h, 000h, 000h
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        push   eax
        push   1
        call   sub_2_867A
        add    esp, 10h
loc_2_9013:
        push   dword ptr [ebp - 4]
        call   sub_2_8CD5
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_8FB1
loc_2_9022:
        mov    ebx, 3
loc_2_9027:
        cmp    ebx, 4
        jne    NEAR PTR loc_2_8EF7
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9039:
        mov    ebx, 1
        jmp    SHORT loc_2_9027
sub_2_9040:
        push   ebx
        push   esi
        push   edi
        call   sub_2_03D1
        or     eax, eax
        je     NEAR PTR loc_2_9172
        push   1
        push   80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_905C:
        add    esp, 8
        db  0A3h, 000h, 000h, 000h, 000h
        push   1
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_906D:
        add    esp, 8
        db  0A3h, 004h, 000h, 000h, 000h
        push   1
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_907E:
        add    esp, 8
        db  0A3h, 008h, 000h, 000h, 000h
        push   1
        push   28h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_908F:
        add    esp, 8
        db  0A3h, 00Ch, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        or     edx, edx
        db  0A1h, 004h, 000h, 000h, 000h
        je     SHORT loc_2_911D
        or     eax, eax
        je     SHORT loc_2_911D
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_911D
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 000h
        je     SHORT loc_2_911D
        push   80h
        push   edx
        call   sub_2_8933
        add    esp, 8
        or     eax, eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  0A1h, 004h, 000h, 000h, 000h
        jl     SHORT loc_2_911D
        db  0BFh, 018h, 000h, 000h, 000h
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
        push   edx
        call   sub_2_8EE6
        add    esp, 4
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  0A1h, 004h, 000h, 000h, 000h
loc_2_911D:
        or     edx, edx
        je     SHORT loc_2_9131
        push   0
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_9129:
        add    esp, 8
        db  0A1h, 004h, 000h, 000h, 000h
loc_2_9131:
        or     eax, eax
        je     SHORT loc_2_9140
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_913D:
        add    esp, 8
loc_2_9140:
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_9159
        push   0
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_9156:
        add    esp, 8
loc_2_9159:
        db  083h, 03Dh, 00Ch, 000h, 000h, 000h, 000h
        je     SHORT loc_2_9172
        push   0
        db  0FFh, 035h, 00Ch, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_916F:
        add    esp, 8
loc_2_9172:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h
sub_2_9178:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_9189:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_918D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_91B1:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_91B5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    dword ptr [ebp - 8], ebx
        mov    esi, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 4], esi
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 1ch]
        call   sub_2_9178
        add    esp, 0ch
        cmp    eax, ebx
        jne    SHORT loc_2_9203
        lea    eax, [ebp - 4]
        push   eax
        mov    edi, dword ptr [ebp + 14h]
        push   edi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        call   sub_2_918D
        add    esp, 10h
        cmp    eax, ebx
        jne    SHORT loc_2_9206
        mov    esi, dword ptr [ebp - 4]
        mov    ebx, 1
        jmp    SHORT loc_2_9206
loc_2_9203:
        mov    edi, dword ptr [ebp + 14h]
loc_2_9206:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_9217
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_9214:
        add    esp, 4
loc_2_9217:
        or     ebx, ebx
        jne    SHORT loc_2_9237
        push   esi
        push   dword ptr [ebp + 10h]
        push   edi
        call   sub_2_6E78
        add    esp, 0ch
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebp - 4], ecx
loc_2_9237:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_9241:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_2_9262
        push   80000003h
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        db  068h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_9279
loc_2_9262:
        push   80000001h
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        db  0FFh, 035h, 020h, 000h, 000h, 000h
loc_2_9279:
        call   sub_2_91B5
        mov    esp, ebp
        pop    ebp
        ret
sub_2_9282:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, 0ffffffffh
        push   1
        push   20h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_9295:
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_2_92CC
        push   20h
        push   esi
        db  068h, 024h, 000h, 000h, 000h
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_9241
        add    esp, 14h
        push   0ah
        push   esi
        call   sub_2_71AC
        add    esp, 8
        mov    edi, eax
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_92C9:
        add    esp, 8
loc_2_92CC:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_92D2:
        push   ebp
        mov    ebp, esp
        push   esi
        db  068h, 028h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9282
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_9302
        db  068h, 034h, 000h, 000h, 000h
        push   esi
        call   sub_2_9282
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_2_9307
loc_2_9302:
        mov    eax, 1
loc_2_9307:
        pop    esi
        pop    ebp
        ret
        db  000h, 000h
loc_2_930C:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, edx
        je     SHORT loc_2_932E
        mov    ecx, dword ptr [eax + 2ch]
        cmp    dword ptr [edx + 2ch], ecx
        jne    SHORT loc_2_932E
        mov    eax, dword ptr [eax + 34h]
        cmp    dword ptr [edx + 34h], eax
        mov    eax, 1
        je     SHORT loc_2_9330
loc_2_932E:
        sub    eax, eax
loc_2_9330:
        pop    ebp
        ret
sub_2_9332:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    dword ptr [esi + 34h], 0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_4594
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_936B
loc_2_9352:
        mov    eax, dword ptr [esi + 34h]
        inc    eax
        mov    dword ptr [esi + 34h], eax
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_4594
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_9352
loc_2_936B:
        mov    eax, dword ptr [esi + 34h]
        pop    esi
        pop    ebp
        ret
sub_2_9371:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9332
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        push   eax
        call   sub_2_7E48
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        push   eax
        push   esi
        call   sub_2_A58E
        add    esp, 8
        push   edi
        push   esi
        call   sub_2_95DC
        add    esp, 8
        push   edi
        push   esi
        call   sub_2_95E4
        add    esp, 8
        mov    dword ptr [ebp - 4], 0
        mov    ebx, 4
loc_2_93C3:
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 10h]
        call   sub_2_96D5
        add    esp, 8
        movzx  eax, ax
        imul   eax, eax, 41h
        db  005h, 000h, 000h, 000h, 000h
        push   eax
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_2_9E2E
        add    esp, 0ch
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], ebx
        jl     SHORT loc_2_93C3
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7ABA
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_66BF
        add    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_2_676C
        add    esp, 8
        push   dword ptr [ebp - 4]
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7EC4
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        lea    eax, [esi + 74h]
        push   eax
        call   sub_2_A0AC
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_9442
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_2_A57E
        add    esp, 8
loc_2_9442:
        lea    eax, [ebp - 0ch]
        push   eax
        push   esi
        call   sub_2_676C
        add    esp, 8
        mov    ebx, 1
        cmp    dword ptr [ebp - 0ch], ebx
        jb     SHORT loc_2_9484
loc_2_9459:
        push   dword ptr [ebp - 0ch]
        push   ebx
        call   sub_2_A035
        add    esp, 4
        push   eax
        push   ebx
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7B95
        add    esp, 14h
        push   eax
        push   ebx
        push   esi
        call   sub_2_A494
        add    esp, 0ch
        inc    ebx
        cmp    ebx, dword ptr [ebp - 0ch]
        jbe    SHORT loc_2_9459
loc_2_9484:
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_2_94A0
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7D92
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_6A78
        add    esp, 8
loc_2_94A0:
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7EEF
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_6B7C
        add    esp, 8
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7F44
        add    esp, 8
        push   eax
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7F44
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_672F
        add    esp, 0ch
        push   dword ptr [ebp - 10h]
        call   sub_2_9645
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_9532
        lea    eax, [ebp - 0ch]
        push   eax
        push   esi
        call   sub_2_676C
        add    esp, 8
        mov    ebx, 1
        cmp    dword ptr [ebp - 0ch], ebx
        jb     SHORT loc_2_9532
loc_2_94FF:
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   dword ptr [ebp - 10h]
        call   sub_2_9612
        add    esp, 0ch
        push   dword ptr [ebp - 4]
        push   ebx
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7C35
        add    esp, 10h
        push   eax
        push   ebx
        push   dword ptr [ebp - 10h]
        call   sub_2_962D
        add    esp, 0ch
        inc    ebx
        cmp    ebx, dword ptr [ebp - 0ch]
        jbe    SHORT loc_2_94FF
loc_2_9532:
        push   dword ptr [ebp - 10h]
        call   sub_2_9684
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_9557
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7FA5
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_A59E
        add    esp, 8
loc_2_9557:
        lea    eax, [ebp - 0ch]
        push   eax
        push   esi
        call   sub_2_676C
        add    esp, 8
        cmp    dword ptr [ebp - 0ch], 2
        jne    SHORT loc_2_9580
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7C93
        add    esp, 8
        push   eax
        push   esi
        call   sub_2_6CD9
        add    esp, 8
loc_2_9580:
        push   1
        push   dword ptr [ebp - 0ch]
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7D3D
        add    esp, 0ch
        push   eax
        push   esi
        call   sub_2_A4E4
        add    esp, 0ch
        push   1
        push   dword ptr [ebp - 0ch]
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7CE8
        add    esp, 0ch
        push   eax
        push   esi
        call   sub_2_A531
        add    esp, 0ch
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_2_7DF3
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        lea    eax, [ebp - 14h]
        push   eax
        push   1
        push   esi
        call   sub_2_9825
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h
sub_2_95DC:
        ret
sub_2_95DD:
        jmp    NEAR PTR loc_2_AB20
        db  000h, 000h
sub_2_95E4:
        ret
        db  000h, 000h, 000h
sub_2_95E8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        db  08Dh, 088h, 010h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        mov    dl, byte ptr [ecx]
        and    edx, 3
        mov    dword ptr [eax], edx
        mov    al, byte ptr [ecx]
        and    al, 0ch
        shr    al, 2
        movzx  ecx, al
        mov    eax, dword ptr [ebp + 10h]
        pop    ebp
        mov    dword ptr [eax], ecx
        ret
sub_2_9612:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        imul   ecx, ecx, 9
        mov    eax, dword ptr [ebp + 0ch]
        db  00Fh, 0B6h, 08Ch, 001h, 010h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        pop    ebp
        mov    dword ptr [eax], ecx
        ret
sub_2_962D:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        imul   ecx, ecx, 9
        mov    dl, byte ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 0ch]
        db  088h, 094h, 001h, 010h, 000h, 000h, 000h
        pop    ebp
        ret
sub_2_9645:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        db  066h, 08Bh, 080h, 017h, 000h, 000h, 000h
        and    eax, 8
        ret
sub_2_965A:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        db  066h, 08Bh, 080h, 017h, 000h, 000h, 000h
        and    eax, 1
        ret
sub_2_966F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        db  066h, 08Bh, 080h, 017h, 000h, 000h, 000h
        and    eax, 2
        ret
sub_2_9684:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        db  066h, 08Bh, 080h, 017h, 000h, 000h, 000h
        and    eax, 20h
        ret
sub_2_9699:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        db  066h, 08Bh, 080h, 017h, 000h, 000h, 000h
        and    eax, 10h
        ret
sub_2_96AE:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        db  066h, 08Bh, 080h, 017h, 000h, 000h, 000h
        and    eax, 40h
        ret
sub_2_96C3:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        pop    ebp
        db  00Fh, 0B6h, 080h, 016h, 000h, 000h, 000h
        ret
sub_2_96D5:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        imul   eax, eax, 9
        db  00Fh, 0B6h, 088h, 015h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        db  066h, 00Fh, 0B6h, 084h, 088h, 000h, 000h, 000h, 000h
        ret
        db  000h
sub_2_96F4:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax + 4ch], 0
        ret
sub_2_9703:
        push   ebp
        mov    ebp, esp
        push   1
        push   dword ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_9710:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_9714:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_9721:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_9725:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_96F4
        add    esp, 4
        push   8
        push   1
        call   sub_2_9703
        add    esp, 8
        mov    dword ptr [esi + 0ch], eax
        push   30h
        push   1
        call   sub_2_9703
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_97B1
        mov    dword ptr [ebp - 8], 1
        mov    dword ptr [esi + 24h], eax
        db  0BBh, 008h, 000h, 000h, 000h
        sub    edi, edi
loc_2_9771:
        mov    ecx, dword ptr [esi + 24h]
        mov    dword ptr [ebp - 4], ecx
        push   dword ptr [ebx]
        push   dword ptr [ebx - 4]
        call   sub_2_9703
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + ecx], eax
        mov    eax, dword ptr [esi + 24h]
        cmp    dword ptr [edi + eax], 0
        je     SHORT loc_2_97AA
        push   dword ptr [eax + edi]
        push   esi
        call   dword ptr [ebx + 4]
        add    esp, 8
        add    ebx, 20h
        add    edi, 4
        cmp    edi, 30h
        jb     SHORT loc_2_9771
        jmp    SHORT loc_2_97B1
loc_2_97AA:
        mov    dword ptr [ebp - 8], 0
loc_2_97B1:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_97BB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0BBh, 010h, 000h, 000h, 000h
        sub    edi, edi
        mov    esi, dword ptr [ebp + 8]
loc_2_97CB:
        mov    eax, dword ptr [esi + 24h]
        push   dword ptr [edi + eax]
        push   esi
        call   dword ptr [ebx]
        add    esp, 8
        mov    eax, dword ptr [esi + 24h]
        push   dword ptr [edi + eax]
        push   dword ptr [ebx - 0ch]
        call   sub_2_9714
        add    esp, 8
        add    ebx, 20h
        add    edi, 4
        cmp    edi, 30h
        jb     SHORT loc_2_97CB
        push   dword ptr [esi + 24h]
        push   1
        call   sub_2_9714
        add    esp, 8
        mov    dword ptr [esi + 24h], 0
        push   dword ptr [esi + 0ch]
        push   1
        call   sub_2_9714
        add    esp, 8
        mov    dword ptr [esi + 0ch], 0
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_9825:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [ecx + eax*4]
        shl    eax, 5
        db  0FFh, 090h, 018h, 000h, 000h, 000h
        add    esp, 8
        sub    eax, eax
        pop    ebp
        ret
sub_2_9847:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [ebp + 10h]
        mov    ecx, dword ptr [eax + 24h]
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [ecx + eax*4]
        shl    eax, 5
        db  0FFh, 090h, 014h, 000h, 000h, 000h
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 24h]
        mov    ecx, dword ptr [ebp + 0ch]
        push   dword ptr [eax + ecx*4]
        shl    ecx, 5
        push   dword ptr [ebp + 10h]
        db  0FFh, 091h, 01Ch, 000h, 000h, 000h
        mov    esp, ebp
        pop    ebp
        ret
sub_2_9885:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_2_98A5
        db  083h, 025h, 044h, 000h, 000h, 000h, 0FBh
        db  083h, 025h, 024h, 001h, 000h, 000h, 0FBh
        db  083h, 025h, 004h, 001h, 000h, 000h, 0FBh
        jmp    SHORT loc_2_98BA
loc_2_98A5:
        db  083h, 00Dh, 044h, 000h, 000h, 000h, 004h
        db  083h, 00Dh, 024h, 001h, 000h, 000h, 004h
        db  083h, 00Dh, 004h, 001h, 000h, 000h, 004h
loc_2_98BA:
        pop    ebp
        ret
loc_2_98BC:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + ecx*4 - 4]
        ret
loc_2_98CD:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx]
        lea    ecx, [ecx + eax*4 - 4]
        mov    edx, dword ptr [edx + 4]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ecx], edx
        pop    ebp
        ret
loc_2_98E5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 20h
        rep movsd
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_98FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        push   edi
        mov    edi, 1
        mov    esi, dword ptr [ebp + 8]
loc_2_990B:
        cmp    edi, 3
        ja     SHORT loc_2_9924
        push   edi
        call   sub_2_A035
        add    esp, 4
        mov    dword ptr [ebp - 10h], edi
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 10h]
        jmp    SHORT loc_2_9937
loc_2_9924:
        push   1
        call   sub_2_A035
        add    esp, 4
        mov    dword ptr [ebp - 8], edi
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 8]
loc_2_9937:
        push   eax
        push   2
        push   esi
        call   sub_2_9825
        add    esp, 0ch
        inc    edi
        cmp    edi, 20h
        jbe    SHORT loc_2_990B
        mov    eax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9954:
        ret
        db  000h, 000h, 000h
sub_2_9958:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        cmp    edx, 0b4h
        jle    SHORT loc_2_996F
        mov    eax, 168h
        sub    eax, edx
        mov    edx, eax
loc_2_996F:
        cmp    edx, 5ah
        jle    SHORT loc_2_9989
        db  0B8h, 068h, 001h, 000h, 000h
        lea    ecx, [edx*2]
        sub    eax, ecx
        movzx  eax, word ptr [eax]
        neg    eax
        jmp    SHORT loc_2_9991
loc_2_9989:
        db  00Fh, 0B7h, 004h, 055h, 000h, 000h, 000h, 000h
loc_2_9991:
        or     edx, edx
        jne    SHORT loc_2_9998
        inc    eax
        jmp    SHORT loc_2_99A1
loc_2_9998:
        cmp    edx, 0b4h
        jne    SHORT loc_2_99A1
        dec    eax
loc_2_99A1:
        pop    ebp
        ret
loc_2_99A3:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret
loc_2_99AD:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, 168h
        mov    eax, dword ptr [eax]
        cdq
        idiv   ecx
        mov    edi, edx
        or     edi, edi
        jge    SHORT loc_2_99C7
        add    edi, ecx
loc_2_99C7:
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi], edi
        cli
        push   edi
        call   sub_2_9958
        add    esp, 4
        mov    dword ptr [esi + 4], eax
        sub    edi, 5ah
        jns    SHORT loc_2_99E4
        add    edi, 168h
loc_2_99E4:
        push   edi
        call   sub_2_9958
        add    esp, 4
        mov    dword ptr [esi + 8], eax
        sti
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_99F7:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 5
        rep movsd
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_9A0D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        push   6
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0ch
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9A31:
        ret
        db  000h, 000h
loc_2_9A34:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [esi]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    esi
        pop    ebp
        ret
loc_2_9A52:
        push   ebp
        mov    ebp, esp
        cli
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 8], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 0ch], eax
        sti
        sub    eax, eax
        pop    ebp
        ret
loc_2_9A6C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 6
        rep movsd
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_9A82:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 1
        push   eax
        push   4
        push   ecx
        call   sub_2_9825
        add    esp, 0ch
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9AB3:
        ret
sub_2_9AB4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 18h]
        mov    esi, dword ptr [ebp + 14h]
        sub    edx, esi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, ecx
        imul   edx, eax
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, ecx
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [ebp - 8]
        xor    edx, edx
        div    dword ptr [ebp - 4]
        add    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_9AE8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 30h]
        call   sub_2_96C3
        add    esp, 4
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        movzx  eax, word ptr [ebp - 2]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        push   eax
        push   64h
        push   1
        push   dword ptr [ebp + 0ch]
        call   sub_2_9AB4
        mov    esp, ebp
        pop    ebp
        ret
sub_2_9B20:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 30h]
        call   sub_2_96C3
        add    esp, 4
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        push   64h
        push   1
        movzx  eax, word ptr [ebp - 2]
        push   eax
        movzx  eax, word ptr [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0ch]
        call   sub_2_9AB4
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9B58:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   sub_2_9B20
        mov    edi, dword ptr [ebp + 0ch]
        add    esp, 8
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ecx], eax
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi]
        call   sub_2_9B20
        add    esp, 8
        mov    ecx, dword ptr [edi + 4]
        pop    edi
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    esi
        pop    ebp
        ret
loc_2_9B8C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cli
        mov    edi, dword ptr [ebp + 0ch]
        push   dword ptr [edi]
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi]
        call   sub_2_9AE8
        add    esp, 8
        mov    dword ptr [esi + 4], eax
        push   dword ptr [edi + 4]
        push   dword ptr [esi]
        call   sub_2_9AE8
        add    esp, 8
        mov    dword ptr [esi + 0ch], eax
        sti
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_9BBE:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 5
        rep movsd
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_9BD4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 8], 32h
        mov    dword ptr [ebp - 4], 32h
        push   eax
        push   9
        push   ecx
        call   sub_2_9825
        add    esp, 0ch
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9C08:
        ret
        db  000h, 000h, 000h
loc_2_9C0C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax]
        cmp    eax, 64h
        jbe    SHORT loc_2_9C1E
        mov    eax, 64h
loc_2_9C1E:
        mov    ecx, 5
        sub    edx, edx
        div    ecx
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx + 18h], eax
        sub    eax, eax
        ret
loc_2_9C31:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 18h]
        imul   eax, eax, 5
        ret
loc_2_9C3F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 0bh
        rep movsd
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_9C55:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 3
        mov    dword ptr [eax + 0ch], 4
        mov    dword ptr [eax], ecx
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        push   eax
        push   3
        push   ecx
        call   sub_2_9825
        add    esp, 0ch
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9C90:
        ret
        db  000h, 000h, 000h
loc_2_9C94:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [esi]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    esi
        pop    ebp
        ret
loc_2_9CB2:
        push   ebp
        mov    ebp, esp
        cli
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 8], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [edx + 10h], 0
        sti
        sub    eax, eax
        pop    ebp
        ret
loc_2_9CD3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 5
        rep movsd
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_9CE9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 1
        push   eax
        push   5
        push   ecx
        call   sub_2_9825
        add    esp, 0ch
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9D1D:
        ret
        db  000h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 4]
        ret
        push   ebp
        mov    ebp, esp
        cli
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4], ecx
        sti
        sub    eax, eax
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0ch
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret
        db  0C3h
loc_2_9D7C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret
loc_2_9D86:
        push   ebp
        mov    ebp, esp
        cli
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        sti
        sub    eax, eax
        pop    ebp
        ret
loc_2_9D99:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        pop    ebp
        ret
loc_2_9DAE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        push   1
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0ch
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9DD5:
        ret
        db  000h, 000h
loc_2_9DD8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret
loc_2_9DE2:
        push   ebp
        mov    ebp, esp
        cli
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
        sti
        sub    eax, eax
        pop    ebp
        ret
loc_2_9DF5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, 44h
        rep movsd
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_9E0B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cli
        mov    ecx, dword ptr [ebp + 0ch]
        imul   ecx, ecx, 41h
        mov    eax, dword ptr [ebp + 8]
        lea    edi, [ecx + eax + 4]
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, 10h
        rep movsd
        movsb
        sti
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_9E2E:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   8
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_9E41:
        add    esp, 8
        push   eax
        call   sub_2_9E0B
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9E4E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 0ch]
loc_2_9E61:
        push   esi
        push   ebx
        push   edi
        call   sub_2_9E0B
        add    esp, 0ch
        add    esi, 41h
        inc    ebx
        db  081h, 0FEh, 004h, 001h, 000h, 000h
        jb     SHORT loc_2_9E61
        mov    dword ptr [ebp - 4], 1
        lea    eax, [ebp - 4]
        push   eax
        push   8
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0ch
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9E9C:
        ret
        db  000h, 000h, 000h
loc_2_9EA0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret
loc_2_9EAA:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], ecx
        sub    eax, eax
        ret
loc_2_9EBB:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        pop    ebp
        ret
loc_2_9ED0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        lea    eax, [ebp - 4]
        push   eax
        push   7
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0ch
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9EF7:
        ret
loc_2_9EF8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret
loc_2_9F02:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], ecx
        mov    eax, 1
        ret
loc_2_9F16:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], ecx
        ret
loc_2_9F25:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0ch
        mov    eax, 1
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9F4C:
        ret
        db  000h, 000h, 000h
loc_2_9F50:
        db  0E9h, 000h, 000h, 000h, 000h
loc_2_9F55:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_9F65:
        add    esp, 8
        sub    eax, eax
        pop    ebp
        ret
loc_2_9F6C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], ecx
        ret
loc_2_9F7B:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        db  0BFh, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], 0
        sub    eax, eax
        stosw
        stosb
        mov    eax, 1
        pop    edi
        pop    ebp
        ret
loc_2_9F9A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0ffffffffh
        lea    eax, [ebp - 8]
        push   eax
        push   0ah
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        mov    esp, ebp
        pop    ebp
        ret
loc_2_9FC0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax]
        ret
loc_2_9FCA:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        pop    ebp
        ret
loc_2_9FD9:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        pop    ebp
        ret
loc_2_9FEE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        push   0bh
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9825
        mov    eax, dword ptr [ebp + 0ch]
        add    esp, 0ch
        mov    dword ptr [eax + 4], esi
        mov    eax, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_A01E:
        ret
        db  000h
sub_2_A020:
        push   ebp
        mov    ebp, esp
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, eax
        je     SHORT loc_2_A033
loc_2_A02C:
        inc    eax
        shr    ecx, 1
        or     ecx, ecx
        jne    SHORT loc_2_A02C
loc_2_A033:
        pop    ebp
        ret
sub_2_A035:
        push   ebp
        mov    ebp, esp
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, eax
        je     SHORT loc_2_A04A
        mov    eax, 1
        dec    cl
        shl    eax, cl
loc_2_A04A:
        pop    ebp
        ret
sub_2_A04C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 2ch], 18h
        je     SHORT loc_2_A072
        cmp    dword ptr [esi + 2ch], 19h
        je     SHORT loc_2_A072
        lea    eax, [esi + 74h]
        push   eax
        push   dword ptr [esi + 28h]
        call   sub_2_A44C
        add    esp, 8
        mov    dword ptr [esi + 2ch], eax
loc_2_A072:
        lea    edi, [esi + 74h]
        cmp    dword ptr [edi], 4
        je     SHORT loc_2_A07F
        cmp    dword ptr [edi], 5
        jne    SHORT loc_2_A098
loc_2_A07F:
        cmp    dword ptr [esi + 28h], 2
        jne    SHORT loc_2_A098
        lea    eax, [esi + 89h]
        push   eax
        push   edi
        push   dword ptr [esi + 2ch]
        call   sub_2_A107
        add    esp, 0ch
loc_2_A098:
        push   edi
        push   dword ptr [esi + 2ch]
        call   sub_2_A0BB
        add    esp, 8
        mov    dword ptr [esi + 4ch], eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h
sub_2_A0AC:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        cmp    byte ptr [eax + 0eh], 1
        sbb    eax, eax
        inc    eax
        ret
sub_2_A0BB:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 7
        jg     SHORT loc_2_A0D9
        je     SHORT loc_2_A100
        cmp    eax, 1
        je     SHORT loc_2_A0F9
        cmp    eax, 2
        je     SHORT loc_2_A100
        cmp    eax, 4
        je     SHORT loc_2_A0F9
        jmp    SHORT loc_2_A0E8
loc_2_A0D9:
        cmp    eax, 1bh
        je     SHORT loc_2_A100
        cmp    eax, 1eh
        je     SHORT loc_2_A0F9
        cmp    eax, 26h
        je     SHORT loc_2_A100
loc_2_A0E8:
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 0eh]
        or     al, al
        jne    SHORT loc_2_A0F4
        jmp    SHORT loc_2_A100
loc_2_A0F4:
        movzx  eax, al
        jmp    SHORT loc_2_A105
loc_2_A0F9:
        mov    eax, 3
        jmp    SHORT loc_2_A105
loc_2_A100:
        mov    eax, 2
loc_2_A105:
        pop    ebp
        ret
sub_2_A107:
        ret
sub_2_A108:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        cmp    eax, 7
        ja     SHORT loc_2_A11B
        db  0FFh, 024h, 085h, 004h, 001h, 000h, 000h
loc_2_A11B:
        sub    eax, eax
        jmp    NEAR PTR loc_2_A1AE
loc_2_A122:
        mov    eax, 1
        jmp    NEAR PTR loc_2_A1AE
loc_2_A12C:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx]
        cmp    eax, 6
        je     SHORT loc_2_A14F
        cmp    eax, 7
        je     SHORT loc_2_A156
        cmp    byte ptr [ecx + 0eh], 3
        jne    SHORT loc_2_A148
        mov    eax, 8
        jmp    SHORT loc_2_A1AE
loc_2_A148:
        mov    eax, 7
        jmp    SHORT loc_2_A1AE
loc_2_A14F:
        mov    eax, 1eh
        jmp    SHORT loc_2_A1AE
loc_2_A156:
        mov    eax, 4
        jmp    SHORT loc_2_A1AE
loc_2_A15D:
        mov    eax, 2
        jmp    SHORT loc_2_A1AE
loc_2_A164:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx], 6
        jne    SHORT loc_2_A173
        mov    eax, 1eh
        jmp    SHORT loc_2_A1AE
loc_2_A173:
        cmp    byte ptr [ecx + 0eh], 3
        jne    SHORT loc_2_A190
        mov    ax, word ptr [ecx + 4]
        cmp    ax, 0ah
        je     SHORT loc_2_A189
        cmp    ax, 14h
        jne    SHORT loc_2_A190
loc_2_A189:
        mov    eax, 4
        jmp    SHORT loc_2_A1AE
loc_2_A190:
        mov    eax, 3
        jmp    SHORT loc_2_A1AE
loc_2_A197:
        sub    eax, eax
        jmp    SHORT loc_2_A1AE
loc_2_A19B:
        mov    eax, 12h
        jmp    SHORT loc_2_A1AE
loc_2_A1A2:
        mov    eax, 21h
        jmp    SHORT loc_2_A1AE
loc_2_A1A9:
        mov    eax, 28h
loc_2_A1AE:
        pop    ebp
        ret
        db  076h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 0B1h, 000h, 000h, 000h, 0B8h, 000h, 000h, 000h
        db  0EBh, 000h, 000h, 000h, 0EFh, 000h, 000h, 000h, 0F6h, 000h, 000h, 000h, 0FDh, 000h, 000h, 000h
sub_2_A1D0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [ecx + 6]
        cmp    eax, 20h
        jg     SHORT loc_2_A1F3
        je     NEAR PTR loc_2_A2D6
        cmp    eax, 17h
        ja     SHORT loc_2_A21B
        shl    eax, 2
        db  0FFh, 0A0h, 04Dh, 002h, 000h, 000h
loc_2_A1F3:
        cmp    eax, 40h
        je     NEAR PTR loc_2_A2DD
        cmp    eax, 60h
        je     NEAR PTR loc_2_A2E4
        cmp    eax, 80h
        je     NEAR PTR loc_2_A2EB
        cmp    eax, 81h
        je     NEAR PTR loc_2_A2F2
loc_2_A21B:
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_2_A108
        add    esp, 8
        jmp    NEAR PTR loc_2_A2F7
loc_2_A22C:
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_2_A108
        add    esp, 8
        jmp    NEAR PTR loc_2_A2F7
loc_2_A23D:
        cmp    byte ptr [ecx + 0eh], 3
        mov    eax, dword ptr [ebp + 8]
        jne    SHORT loc_2_A268
        cmp    eax, 2
        jne    SHORT loc_2_A252
        cmp    word ptr [ecx + 4], 2
        je     SHORT loc_2_A25E
loc_2_A252:
        cmp    eax, 4
        jne    SHORT loc_2_A268
        cmp    word ptr [ecx + 4], 50h
        jne    SHORT loc_2_A268
loc_2_A25E:
        mov    eax, 13h
        jmp    NEAR PTR loc_2_A2F7
loc_2_A268:
        push   ecx
        push   eax
        call   sub_2_A108
        add    esp, 8
        jmp    NEAR PTR loc_2_A2F7
loc_2_A277:
        mov    eax, 0ah
        jmp    SHORT loc_2_A2F7
loc_2_A27E:
        mov    eax, 16h
        jmp    SHORT loc_2_A2F7
loc_2_A285:
        mov    eax, 1ch
        jmp    SHORT loc_2_A2F7
loc_2_A28C:
        mov    eax, 13h
        jmp    SHORT loc_2_A2F7
loc_2_A293:
        mov    eax, 15h
        jmp    SHORT loc_2_A2F7
loc_2_A29A:
        mov    eax, 0bh
        jmp    SHORT loc_2_A2F7
loc_2_A2A1:
        mov    eax, 17h
        jmp    SHORT loc_2_A2F7
loc_2_A2A8:
        mov    eax, 29h
        jmp    SHORT loc_2_A2F7
loc_2_A2AF:
        mov    eax, 2ah
        jmp    SHORT loc_2_A2F7
loc_2_A2B6:
        mov    eax, 2bh
        jmp    SHORT loc_2_A2F7
loc_2_A2BD:
        sub    eax, eax
        jmp    SHORT loc_2_A2F7
loc_2_A2C1:
        mov    eax, 22h
        jmp    SHORT loc_2_A2F7
loc_2_A2C8:
        mov    eax, 23h
        jmp    SHORT loc_2_A2F7
loc_2_A2CF:
        mov    eax, 22h
        jmp    SHORT loc_2_A2F7
loc_2_A2D6:
        mov    eax, 1fh
        jmp    SHORT loc_2_A2F7
loc_2_A2DD:
        mov    eax, 2fh
        jmp    SHORT loc_2_A2F7
loc_2_A2E4:
        mov    eax, 33h
        jmp    SHORT loc_2_A2F7
loc_2_A2EB:
        mov    eax, 20h
        jmp    SHORT loc_2_A2F7
loc_2_A2F2:
        mov    eax, 27h
loc_2_A2F7:
        pop    ebp
        ret
loc_2_A2F9:
        add    byte ptr [ecx], 0
        db  000h, 091h, 001h, 000h, 000h, 0CBh
        add    dword ptr [eax], eax
        db  000h, 0D2h
        add    dword ptr [eax], eax
        db  000h, 0D9h
        add    dword ptr [eax], eax
        db  000h, 0E0h
        add    dword ptr [eax], eax
        db  000h, 0E7h
        add    dword ptr [eax], eax
        db  000h, 0EEh
        add    dword ptr [eax], eax
        db  000h, 0F5h
        add    dword ptr [eax], eax
        db  000h, 0FCh
        add    dword ptr [eax], eax
        db  000h, 003h
        add    al, byte ptr [eax]
        db  000h, 00Ah
        add    al, byte ptr [eax]
        db  000h, 06Fh, 001h
        add    byte ptr [eax], al
        outsd
        add    dword ptr [eax], eax
        db  000h, 06Fh, 001h
        add    byte ptr [eax], al
        outsd
        add    dword ptr [eax], eax
        db  000h, 011h
        add    al, byte ptr [eax]
        db  000h, 015h, 002h, 000h, 000h, 01Ch
        add    al, byte ptr [eax]
        db  000h, 023h
        add    al, byte ptr [eax]
        db  000h, 023h
        add    al, byte ptr [eax]
        db  000h, 023h
        add    al, byte ptr [eax]
        db  000h, 023h
        add    al, byte ptr [eax]
        db  000h, 023h
        add    al, byte ptr [eax]
        db  000h
sub_2_A359:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        cmp    word ptr [eax + 6], 0
        je     SHORT loc_2_A374
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_A1D0
        add    esp, 8
        jmp    SHORT loc_2_A3AE
loc_2_A374:
        movzx  ecx, byte ptr [eax + 9]
        or     ecx, ecx
        je     SHORT loc_2_A394
        cmp    ecx, 1
        je     SHORT loc_2_A3A2
        cmp    ecx, 2
        je     SHORT loc_2_A3A9
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_A108
        add    esp, 8
        jmp    SHORT loc_2_A3AE
loc_2_A394:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_A108
        add    esp, 8
        jmp    SHORT loc_2_A3AE
loc_2_A3A2:
        mov    eax, 1fh
        jmp    SHORT loc_2_A3AE
loc_2_A3A9:
        mov    eax, 2fh
loc_2_A3AE:
        pop    ebp
        ret
sub_2_A3B0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        mov    cx, word ptr [eax + 8]
        or     cl, cl
        jne    SHORT loc_2_A3CC
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_A359
        add    esp, 8
        jmp    SHORT loc_2_A44A
loc_2_A3CC:
        movzx  ecx, cx
        cmp    ecx, 104h
        jg     SHORT loc_2_A3F3
        je     SHORT loc_2_A42E
        cmp    ecx, 101h
        je     SHORT loc_2_A419
        cmp    ecx, 102h
        je     SHORT loc_2_A420
        cmp    ecx, 103h
        je     SHORT loc_2_A427
        jmp    SHORT loc_2_A40B
loc_2_A3F3:
        cmp    ecx, 201h
        jl     SHORT loc_2_A40B
        cmp    ecx, 203h
        jle    SHORT loc_2_A43E
        cmp    ecx, 204h
        je     SHORT loc_2_A445
loc_2_A40B:
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_A359
        add    esp, 8
        jmp    SHORT loc_2_A44A
loc_2_A419:
        mov    eax, 1bh
        jmp    SHORT loc_2_A44A
loc_2_A420:
        mov    eax, 39h
        jmp    SHORT loc_2_A44A
loc_2_A427:
        mov    eax, 3ah
        jmp    SHORT loc_2_A44A
loc_2_A42E:
        cmp    word ptr [eax + 6], 40h
        jne    SHORT loc_2_A437
        jmp    SHORT loc_2_A445
loc_2_A437:
        mov    eax, 3bh
        jmp    SHORT loc_2_A44A
loc_2_A43E:
        mov    eax, 26h
        jmp    SHORT loc_2_A44A
loc_2_A445:
        mov    eax, 3ch
loc_2_A44A:
        pop    ebp
        ret
sub_2_A44C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        cmp    dword ptr [eax + 0ah], 0
        push   dword ptr [ebp + 8]
        je     SHORT loc_2_A463
        call   sub_2_A3B0
        jmp    SHORT loc_2_A468
loc_2_A463:
        call   sub_2_A1D0
loc_2_A468:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_A46C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        push   2
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 10h]
        add    esp, 0ch
        mov    dword ptr [ecx], eax
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
sub_2_A494:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 0ch], esi
        mov    edi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], edi
        push   eax
        push   2
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_2_9825
        add    esp, 0ch
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_676C
        add    esp, 8
        push   dword ptr [ebp - 4]
        push   edi
        push   esi
        push   dword ptr [ebx + 2ch]
        push   dword ptr [ebx + 34h]
        call   sub_2_7BE5
        add    esp, 14h
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_A4E4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_676C
        add    esp, 8
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], edi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   5
        push   esi
        call   sub_2_9825
        add    esp, 0ch
        push   dword ptr [ebp - 4]
        push   edi
        push   dword ptr [esi + 2ch]
        push   dword ptr [esi + 34h]
        call   sub_2_7D67
        add    esp, 10h
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_A531:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_676C
        add    esp, 8
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], edi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   4
        push   esi
        call   sub_2_9825
        add    esp, 0ch
        push   dword ptr [ebp - 4]
        push   edi
        push   dword ptr [esi + 2ch]
        push   dword ptr [esi + 34h]
        call   sub_2_7D12
        add    esp, 10h
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_A57E:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 4ch], ecx
        sub    eax, eax
        pop    ebp
        ret
sub_2_A58E:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 30h], ecx
        sub    eax, eax
        pop    ebp
        ret
sub_2_A59E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        push   7
        push   dword ptr [ebp + 8]
        call   sub_2_9825
        add    esp, 0ch
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 0C3h, 000h, 000h, 000h
sub_2_A5C8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        dec    eax
        cmp    eax, 28h
        ja     SHORT loc_2_A5DB
        db  0FFh, 024h, 085h, 06Bh, 000h, 000h, 000h
loc_2_A5DB:
        mov    eax, 0ch
        jmp    SHORT loc_2_A631
loc_2_A5E2:
        sub    eax, eax
        jmp    SHORT loc_2_A631
loc_2_A5E6:
        mov    eax, 3
        jmp    SHORT loc_2_A631
loc_2_A5ED:
        mov    eax, 4
        jmp    SHORT loc_2_A631
loc_2_A5F4:
        mov    eax, 1
        jmp    SHORT loc_2_A631
loc_2_A5FB:
        mov    eax, 6
        jmp    SHORT loc_2_A631
loc_2_A602:
        mov    eax, 2
        jmp    SHORT loc_2_A631
loc_2_A609:
        mov    eax, 0bh
        jmp    SHORT loc_2_A631
loc_2_A610:
        mov    eax, 7
        jmp    SHORT loc_2_A631
loc_2_A617:
        mov    eax, 8
        jmp    SHORT loc_2_A631
loc_2_A61E:
        mov    eax, 0ah
        jmp    SHORT loc_2_A631
loc_2_A625:
        mov    eax, 9
        jmp    SHORT loc_2_A631
loc_2_A62C:
        mov    eax, 5
loc_2_A631:
        pop    ebp
        ret
        db  01Ah, 000h, 000h, 000h, 01Eh, 000h, 000h, 000h, 025h, 000h, 000h, 000h, 02Ch, 000h, 000h, 000h
        db  02Ch, 000h, 000h, 000h, 02Ch, 000h, 000h, 000h, 013h, 000h, 000h, 000h, 013h, 000h, 000h, 000h
        db  02Ch, 000h, 000h, 000h, 033h, 000h, 000h, 000h, 033h, 000h, 000h, 000h, 013h, 000h, 000h, 000h
        db  013h, 000h, 000h, 000h, 013h, 000h, 000h, 000h, 013h, 000h, 000h, 000h, 013h, 000h, 000h, 000h
        db  013h, 000h, 000h, 000h, 013h, 000h, 000h, 000h, 03Ah, 000h, 000h, 000h, 041h, 000h, 000h, 000h
        db  041h, 000h, 000h, 000h, 048h, 000h, 000h, 000h, 048h, 000h, 000h, 000h, 04Fh, 000h, 000h, 000h
        db  056h, 000h, 000h, 000h, 03Ah, 000h, 000h, 000h, 05Dh, 000h, 000h, 000h, 013h, 000h, 000h, 000h
        db  064h, 000h, 000h, 000h, 013h, 000h, 000h, 000h, 013h, 000h, 000h, 000h, 013h, 000h, 000h, 000h
        db  013h, 000h, 000h, 000h, 056h, 000h, 000h, 000h, 013h, 000h, 000h, 000h, 013h, 000h, 000h, 000h
        db  013h, 000h, 000h, 000h, 013h, 000h, 000h, 000h, 013h, 000h, 000h, 000h, 013h, 000h, 000h, 000h
        db  048h, 000h, 000h, 000h
loc_2_A6D7:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, 0fffffffeh
        push   6
        call   sub_2_4550
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_2_A7B9
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, 51h
        ja     SHORT loc_2_A70F
        cmp    ecx, 50h
        jae    NEAR PTR loc_2_A7B2
        cmp    ecx, 8
        ja     SHORT loc_2_A70F
        db  0FFh, 024h, 08Dh, 0FFh, 001h, 000h, 000h
loc_2_A70F:
        mov    esi, 0fffffffeh
        jmp    NEAR PTR loc_2_A7B9
loc_2_A719:
        push   0
        push   3
        push   eax
        call   sub_2_67F4
        add    esp, 0ch
        mov    esi, eax
        jmp    NEAR PTR loc_2_A7B9
loc_2_A72D:
        push   0
        push   3
        push   eax
        call   sub_2_67F4
        add    esp, 0ch
        mov    esi, 0fffffffeh
        jmp    SHORT loc_2_A7B9
loc_2_A741:
        push   1
        push   3
        push   eax
        call   sub_2_67F4
        add    esp, 0ch
        mov    esi, eax
        jmp    SHORT loc_2_A7B9
loc_2_A752:
        push   1
        push   eax
        call   sub_2_68C6
        add    esp, 8
        mov    esi, eax
        jmp    SHORT loc_2_A7B9
loc_2_A761:
        push   0
        push   eax
        call   sub_2_68C6
        add    esp, 8
        mov    esi, eax
        jmp    SHORT loc_2_A7B9
loc_2_A770:
        push   0
        push   eax
        call   sub_2_6B7C
        add    esp, 8
        mov    esi, eax
        jmp    SHORT loc_2_A7B9
loc_2_A77F:
        push   0b4h
        push   eax
        call   sub_2_6B7C
        add    esp, 8
        mov    esi, eax
        jmp    SHORT loc_2_A7B9
loc_2_A791:
        push   5ah
        push   eax
        call   sub_2_6B7C
        add    esp, 8
        mov    esi, eax
        jmp    SHORT loc_2_A7B9
loc_2_A7A0:
        push   10eh
        push   eax
        call   sub_2_6B7C
        add    esp, 8
        mov    esi, eax
        jmp    SHORT loc_2_A7B9
loc_2_A7B2:
        call   sub_2_AB13
        sub    esi, esi
loc_2_A7B9:
        or     esi, esi
        mov    eax, 0
        setge  al
        dec    eax
        pop    esi
        pop    ebp
        ret
        db  051h, 001h, 000h, 000h, 065h, 001h, 000h, 000h, 079h, 001h, 000h, 000h, 08Ah, 001h, 000h, 000h
        db  099h, 001h, 000h, 000h, 0A8h, 001h, 000h, 000h, 0B7h, 001h, 000h, 000h, 0C9h, 001h, 000h, 000h
        db  0D8h, 001h, 000h, 000h
loc_2_A7EB:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    esi, 0fffffffeh
        mov    dword ptr [ebp - 4], 0
        push   6
        call   sub_2_4550
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_2_AA5B
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0ch
        ja     SHORT loc_2_A823
        db  0FFh, 024h, 085h, 0A6h, 004h, 000h, 000h
loc_2_A823:
        mov    esi, 0fffffffeh
        jmp    NEAR PTR loc_2_AA5B
loc_2_A82D:
        lea    eax, [ebp - 8]
        push   eax
        push   edi
        call   sub_2_6692
        add    esp, 8
        mov    esi, eax
        push   dword ptr [ebp - 8]
        call   sub_2_A5C8
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_AA5B
loc_2_A84F:
        lea    eax, [ebp - 20h]
        push   eax
        push   edi
        call   sub_2_6658
        add    esp, 8
        mov    esi, eax
        cmp    word ptr [ebp - 20h], 2
        jne    SHORT loc_2_A872
        movzx  eax, word ptr [ebp - 18h]
        dec    eax
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_AA5B
loc_2_A872:
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_2_AA5B
loc_2_A87E:
        lea    eax, [ebp - 14h]
        push   eax
        push   edi
        call   sub_2_6BE4
        add    esp, 8
        mov    esi, eax
        mov    eax, dword ptr [ebp - 14h]
        or     eax, eax
        je     SHORT loc_2_A8AA
        cmp    eax, 1
        je     SHORT loc_2_A8B6
        cmp    eax, 2
        je     SHORT loc_2_A8C2
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_AA5B
loc_2_A8AA:
        mov    dword ptr [ebp - 4], 3
        jmp    NEAR PTR loc_2_AA5B
loc_2_A8B6:
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_2_AA5B
loc_2_A8C2:
        mov    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_2_AA5B
loc_2_A8CE:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_6BB3
        add    esp, 8
        mov    esi, eax
        dec    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_2_AA5B
loc_2_A8E5:
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_65A9
        add    esp, 4
        mov    esi, eax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffh
        mov    ebx, dword ptr [ebp - 4]
        and    ebx, 0ffffff00h
        imul   ebx, ebx, 0ah
        mov    dword ptr [ebp - 10h], 0ah
        mov    eax, ecx
        cdq
        idiv   dword ptr [ebp - 10h]
        shl    eax, 8
        add    ebx, eax
        mov    dword ptr [ebp - 0ch], 0ah
        mov    eax, ecx
        cdq
        idiv   dword ptr [ebp - 0ch]
        or     ebx, edx
        mov    dword ptr [ebp - 4], ebx
        call   sub_2_655C
        push   0
        push   edi
        call   sub_2_66BF
        add    esp, 8
        push   32h
        push   32h
        push   edi
        call   sub_2_672F
        add    esp, 0ch
        jmp    NEAR PTR loc_2_AA5B
loc_2_A94E:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_676C
        add    esp, 8
        mov    esi, eax
        mov    eax, 0
        cmp    dword ptr [ebp - 4], 2
        setg   al
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_2_AA5B
loc_2_A971:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_6B61
        add    esp, 8
        mov    esi, eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, eax
        je     SHORT loc_2_A9A6
        cmp    eax, 5ah
        je     SHORT loc_2_A9B2
        cmp    eax, 0b4h
        je     SHORT loc_2_A9BE
        cmp    eax, 10eh
        je     SHORT loc_2_A9CA
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_AA5B
loc_2_A9A6:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_2_AA5B
loc_2_A9B2:
        mov    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_2_AA5B
loc_2_A9BE:
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_2_AA5B
loc_2_A9CA:
        mov    dword ptr [ebp - 4], 3
        jmp    NEAR PTR loc_2_AA5B
loc_2_A9D6:
        lea    eax, [ebp - 4]
        push   eax
        push   3
        push   edi
        call   sub_2_67A2
        add    esp, 0ch
        mov    esi, eax
        cmp    dword ptr [ebp - 4], 1
        cmc
        sbb    eax, eax
        and    eax, 2
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_2_AA5B
loc_2_A9F6:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_6860
        add    esp, 8
        mov    esi, eax
        cmp    dword ptr [ebp - 4], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_2_AA5B
loc_2_AA11:
        mov    esi, 0fffffffeh
        jmp    SHORT loc_2_AA5B
loc_2_AA18:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_6A59
        add    esp, 8
        mov    esi, eax
        jmp    SHORT loc_2_AA5B
loc_2_AA29:
        lea    eax, [ebp - 8]
        push   eax
        push   edi
        call   sub_2_6692
        add    esp, 8
        mov    esi, eax
        push   dword ptr [ebp - 8]
        call   sub_2_A5C8
        add    esp, 4
        sub    eax, 0bh
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_2_AA5B
loc_2_AA52:
        mov    dword ptr [ebp - 4], 20h
        sub    esi, esi
loc_2_AA5B:
        or     esi, esi
        mov    eax, 0ffffffffh
        jl     SHORT loc_2_AA67
        mov    eax, dword ptr [ebp - 4]
loc_2_AA67:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  065h, 002h, 000h, 000h, 087h, 002h, 000h, 000h, 0B6h, 002h, 000h, 000h, 006h, 003h, 000h, 000h
        db  01Dh, 003h, 000h, 000h, 086h, 003h, 000h, 000h, 0A9h, 003h, 000h, 000h, 00Eh, 004h, 000h, 000h
        db  02Eh, 004h, 000h, 000h, 049h, 004h, 000h, 000h, 050h, 004h, 000h, 000h, 061h, 004h, 000h, 000h
        db  08Ah, 004h, 000h, 000h, 000h, 000h
loc_2_AAA4:
        push   ebp
        mov    ebp, esp
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   6
        db  068h, 00Ch, 000h, 000h, 000h
        db  068h, 014h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_AAC5:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_AAC9:
        push   ebp
        mov    ebp, esp
        sub    eax, eax
        db  039h, 005h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_AB01
        mov    ax, word ptr [ebp + 8]
        db  066h, 0A3h, 008h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        db  0A3h, 004h, 000h, 000h, 000h
        push   1
        db  068h, 004h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_AAF9:
        add    esp, 0ch
        db  0A3h, 000h, 000h, 000h, 000h
loc_2_AB01:
        pop    ebp
        ret
        db  06Ah, 000h, 06Ah, 000h, 0E8h, 0BDh, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 008h, 066h, 02Bh, 0C0h, 0C3h
sub_2_AB13:
        push   0
        push   1
        call   sub_2_AAC9
        add    esp, 8
        ret
loc_2_AB20:
        push   0
        push   2
        call   sub_2_AAC9
        add    esp, 8
        ret
sub_2_AB2D:
        sub    eax, eax
        ret
sub_2_AB30:
        sub    eax, eax
        ret
        db  000h
sub_2_AB34:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    SHORT loc_2_AB47
        sub    eax, eax
        jmp    NEAR PTR loc_2_AC0E
loc_2_AB47:
        cmp    esi, -1
        jne    SHORT loc_2_AB56
        mov    eax, 1
        jmp    NEAR PTR loc_2_AC0E
loc_2_AB56:
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 001h
        jne    NEAR PTR loc_2_ABFA
        cmp    esi, 100h
        jb     SHORT loc_2_AB9D
        cmp    esi, 10bh
        jae    SHORT loc_2_AB9D
        imul   esi, esi, 5
        mov    edi, dword ptr [ebp + 14h]
        db  0FFh, 0B6h, 001h, 0FBh, 0FFh, 0FFh
        push   dword ptr [edi + 18h]
        db  00Fh, 0B6h, 086h, 000h, 0FBh, 0FFh, 0FFh
        db  0FFh, 014h, 085h, 000h, 001h, 000h, 000h
        add    esp, 8
        sub    eax, eax
        jmp    SHORT loc_2_AC08
loc_2_AB9D:
        cmp    esi, 200h
        jb     SHORT loc_2_ABE7
        cmp    esi, 228h
        jae    SHORT loc_2_ABE7
        mov    edi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edi + 18h]
        push   ecx
        lea    eax, [ecx + 4]
        push   eax
        push   dword ptr [eax]
        call   sub_2_455F
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_ABE3
        imul   esi, esi, 5
        db  0FFh, 0B6h, 039h, 0F6h, 0FFh, 0FFh
        push   dword ptr [edi + 18h]
        db  00Fh, 0B6h, 086h, 038h, 0F6h, 0FFh, 0FFh
        db  0FFh, 014h, 085h, 010h, 001h, 000h, 000h
        add    esp, 8
loc_2_ABE3:
        sub    eax, eax
        jmp    SHORT loc_2_AC08
loc_2_ABE7:
        mov    edi, dword ptr [ebp + 14h]
        mov    eax, 50h
        mov    ecx, dword ptr [edi + 18h]
        mov    dword ptr [ecx], 0ffffffeah
        jmp    SHORT loc_2_AC08
loc_2_ABFA:
        mov    edi, dword ptr [ebp + 14h]
        sub    eax, eax
        mov    ecx, dword ptr [edi + 18h]
        mov    dword ptr [ecx], 0ffffffech
loc_2_AC08:
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
loc_2_AC0E:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_AC12:
        push   ebp
        mov    ebp, esp
        call   dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [ecx], eax
        ret
loc_2_AC1F:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 4]
        push   eax
        call   dword ptr [ebp + 0ch]
        add    esp, 4
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret
loc_2_AC35:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 4]
        lea    eax, [esi + 8]
        push   eax
        call   dword ptr [ebp + 0ch]
        add    esp, 8
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret
loc_2_AC4E:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0ch]
        add    esp, 8
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret
loc_2_AC67:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0ch]
        add    esp, 8
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret
loc_2_AC80:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0ch]
        add    esp, 8
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret
loc_2_AC98:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0ch]
        push   eax
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0ch]
        add    esp, 0ch
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret
loc_2_ACB5:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0ch]
        add    esp, 0ch
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret
loc_2_ACD0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0ch]
        push   eax
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0ch]
        add    esp, 0ch
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret
loc_2_ACEC:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        call   dword ptr [ebp + 0ch]
        add    esp, 8
        mov    dword ptr [esi], eax
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h
sub_2_AD08:
        db  0CDh, 020h, 004h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_2_AD10:
        db  0CDh, 020h, 009h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_2_AD18:
        db  0CDh, 020h, 00Eh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_2_AD20:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_2_AD28:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_2_AD30:
        db  0CDh, 020h, 002h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_AD38:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_AD40:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_AD48:
        db  0CDh, 020h, 03Ch, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_AD50:
        db  0CDh, 020h, 03Dh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_AD58:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_2_AD60:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
_PTEXT ends
        end
