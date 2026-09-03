; ifsmgr_pcode_04_05.asm -- IFSMGR object 4 segment _PTEXT  (part 5/5)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_4_13C9E:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        db  0A1h, 084h, 06Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_4_13D0F
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        sub    ebx, ebx
loc_4_13CCA:
        mov    dword ptr [ebp - 88h], eax
        cmp    byte ptr [eax + 30h], bl
        jbe    SHORT loc_4_13D02
        cmp    esi, ebx
        je     SHORT loc_4_13CF1
        cmp    edi, ebx
        jne    SHORT loc_4_13CE7
        mov    ecx, dword ptr [eax + 28h]
        shr    ecx, 10h
        cmp    ecx, esi
        je     SHORT loc_4_13CF1
loc_4_13CE7:
        cmp    edi, 1
        jne    SHORT loc_4_13D02
        cmp    dword ptr [eax + 28h], esi
        jne    SHORT loc_4_13D02
loc_4_13CF1:
        push   1
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_13AC3
        add    esp, 8
loc_4_13D02:
        mov    eax, dword ptr [ebp - 88h]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, ebx
        jne    SHORT loc_4_13CCA
loc_4_13D0F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  0CDh, 020h, 020h, 000h, 001h, 000h, 0C3h
sub_4_13D1D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    bl, bl
        mov    edi, dword ptr [esi + 7ch]
        test   byte ptr [esi + 19h], 10h
        jne    NEAR PTR loc_4_13DCD
        push   esi
        db  0E8h, 034h, 027h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_4_13DCD
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1
        jne    SHORT loc_4_13D9B
        mov    al, byte ptr [edi + 30h]
        cmp    al, 3
        db  072h, 042h
        cmp    al, 4
        je     SHORT loc_4_13D6F
        cmp    al, 3
        jne    SHORT loc_4_13D66
        call   sub_4_12001
        or     eax, eax
        je     SHORT loc_4_13D6F
loc_4_13D66:
        db  080h, 03Dh, 098h, 065h, 000h, 000h, 000h
        db  074h, 028h
loc_4_13D6F:
        inc    word ptr [edi + 3ch]
        lea    ebx, [edi + 3ch]
        lea    eax, [edi + 38h]
        cmp    word ptr [eax], 0
        jbe    SHORT loc_4_13D8D
        mov    word ptr [eax], 0
        push   eax
        db  0E8h, 067h, 0F1h, 0F9h, 0FFh
        add    esp, 4
loc_4_13D8D:
        push   ebx
        push   edi
        db  0E8h
IFSMgr_ParseOneElement:
        out    28h, eax
        cli
        inc    dword ptr [ebx + 10b308c4h]
        jmp    SHORT loc_4_13DCD
loc_4_13D9B:
        or     eax, eax
        jne    SHORT loc_4_13DCD
        cmp    byte ptr [edi + 30h], 4
        jne    SHORT loc_4_13DCD
        lea    eax, [edi + 38h]
        inc    word ptr [edi + 3ah]
        cmp    word ptr [eax], 0
        jbe    SHORT loc_4_13DC0
        mov    word ptr [eax], 0
        push   eax
        db  0E8h, 034h, 0F1h, 0F9h, 0FFh
        add    esp, 4
loc_4_13DC0:
        lea    eax, [edi + 3ah]
        push   eax
        push   edi
        db  0E8h, 0B1h, 028h, 0FAh, 0FFh
        add    esp, 8
loc_4_13DCD:
        cmp    byte ptr [edi + 30h], 0
        push   1
        mov    ecx, dword ptr [ebp + 0ch]
        push   esi
        lea    eax, [ecx + 1fh]
        push   eax
        jbe    SHORT loc_4_13DE2
        mov    eax, dword ptr [edi + 2ch]
        jmp    SHORT loc_4_13DE5
loc_4_13DE2:
        mov    eax, dword ptr [edi + 0ch]
loc_4_13DE5:
        push   dword ptr [eax + ecx*4 + 4]
        db  0E8h, 0A2h, 0FDh, 0F9h, 0FFh
        add    esp, 10h
        cmp    byte ptr [edi + 30h], 0
        jbe    SHORT loc_4_13DFF
        movzx  ax, bl
        or     word ptr [edi + 32h], ax
loc_4_13DFF:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_13E08:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
loc_4_13E11:
        mov    edi, dword ptr [esi + 7ch]
        db  0E8h, 056h, 026h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
loc_4_13E1E:
        jne    SHORT loc_4_13E4E
        cmp    byte ptr [edi + 30h], 1
        jne    SHORT loc_4_13E33
        mov    word ptr [esi + 1ah], 28h
        mov    eax, 28h
        jmp    SHORT loc_4_13E5E
loc_4_13E33:
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1
        jne    SHORT loc_4_13E51
        test   byte ptr [edi + 32h], 20h
        jne    SHORT loc_4_13E51
        mov    word ptr [esi + 1ah], 13h
        mov    eax, 13h
        jmp    SHORT loc_4_13E5E
loc_4_13E4E:
        mov    eax, dword ptr [ebp + 10h]
loc_4_13E51:
        push   eax
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_4_13D1D
        add    esp, 0ch
loc_4_13E5E:
        pop    edi
loc_4_13E5F:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   1
        push   0
        push   dword ptr [ebp + 8]
        call   sub_4_13E08
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [eax + 4]
        or     cl, cl
        je     SHORT loc_4_13E8B
        cmp    cl, 1
        je     SHORT loc_4_13E8B
        push   0
        jmp    SHORT loc_4_13E8D
loc_4_13E8B:
        push   1
loc_4_13E8D:
        push   1
        push   eax
        call   sub_4_13E08
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 4], 1
        je     SHORT loc_4_13EA9
        push   1
        jmp    SHORT loc_4_13EAB
loc_4_13EA9:
        push   0
loc_4_13EAB:
        push   2
        push   eax
        call   sub_4_13E08
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   1
        push   3
        push   dword ptr [ebp + 8]
        call   sub_4_13E08
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   4
        push   dword ptr [ebp + 8]
        call   sub_4_13E08
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
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 07Ch, 025h, 0FAh, 0FFh
        add    esp, 4
        cmp    eax, ebx
        jne    NEAR PTR loc_4_13FF1
        mov    eax, dword ptr [esi + 7ch]
        cmp    byte ptr [eax + 30h], 1
        jne    SHORT loc_4_13F17
        mov    word ptr [esi + 1ah], 28h
        mov    eax, 28h
        jmp    NEAR PTR loc_4_13FFE
loc_4_13F17:
        test   byte ptr [eax + 32h], 20h
        jne    SHORT loc_4_13F45
        mov    ax, word ptr [esi + 18h]
        test   al, 10h
        je     SHORT loc_4_13F32
        mov    edi, 1
        and    ax, 0ffefh
        mov    word ptr [esi + 18h], ax
loc_4_13F32:
        test   byte ptr [esi + 18h], 2
        je     SHORT loc_4_13F53
        mov    edi, 2
        mov    word ptr [esi + 18h], 1
        jmp    SHORT loc_4_13F53
loc_4_13F45:
        lea    eax, [esi + 18h]
        test   byte ptr [esi + 18h], 12h
        je     SHORT loc_4_13F53
        mov    ebx, 1
loc_4_13F53:
        push   ebx
        push   5
        push   esi
        call   sub_4_13D1D
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_13FDA
        cmp    edi, 2
        jne    SHORT loc_4_13FA3
        mov    bl, byte ptr [esi + 4]
        mov    di, word ptr [esi + 18h]
        mov    byte ptr [esi + 4], 2
        mov    word ptr [esi + 18h], 1
        push   0
        push   esi
        mov    eax, dword ptr [esi + 74h]
        push   0bh
        mov    eax, dword ptr [eax + 8]
        push   dword ptr [eax + 8]
        db  0E8h, 004h, 0FCh, 0F9h, 0FFh
        add    esp, 10h
        mov    word ptr [esi + 18h], di
        mov    byte ptr [esi + 4], bl
        mov    word ptr [esi + 1ah], 13h
        mov    eax, 13h
        jmp    SHORT loc_4_13FFE
loc_4_13FA3:
        mov    eax, dword ptr [esi + 74h]
        cmp    dword ptr [eax + 24h], 0
        jne    SHORT loc_4_13FD4
        mov    eax, dword ptr [esi + 8ch]
        add    eax, 410h
        mov    dword ptr [esi + 0ch], eax
        mov    bl, byte ptr [esi + 4]
        push   3
        push   esi
        call   sub_4_122E1
        add    esp, 8
        mov    byte ptr [esi + 4], bl
        mov    eax, dword ptr [esi + 8ch]
        mov    dword ptr [esi + 0ch], eax
loc_4_13FD4:
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_13FFE
loc_4_13FDA:
        or     edi, edi
        je     SHORT loc_4_13FEB
        mov    word ptr [esi + 1ah], 13h
        mov    eax, 13h
        jmp    SHORT loc_4_13FFE
loc_4_13FEB:
        movsx  eax, word ptr [esi + 1ah]
        jmp    SHORT loc_4_13FFE
loc_4_13FF1:
        push   0
        push   5
        push   esi
        call   sub_4_13D1D
        add    esp, 0ch
loc_4_13FFE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   1
        push   6
        push   dword ptr [ebp + 8]
        call   sub_4_13E08
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   7
        push   dword ptr [ebp + 8]
        call   sub_4_13E08
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   8
        push   dword ptr [ebp + 8]
        call   sub_4_13E08
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   9
        push   dword ptr [ebp + 8]
        call   sub_4_13E08
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   0ah
        push   dword ptr [ebp + 8]
        call   sub_4_13E08
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [esi + 28h]
        mov    bl, byte ptr [eax + 18h]
        db  0E8h, 0F7h, 023h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_140E3
        movzx  eax, byte ptr [esi + 4]
        cmp    eax, 4
        jl     SHORT loc_4_140B6
        cmp    eax, 5
        jle    SHORT loc_4_14097
        cmp    eax, 0dh
        je     SHORT loc_4_140A4
        cmp    eax, 0fh
        jne    SHORT loc_4_140B6
loc_4_14097:
        mov    word ptr [esi + 1ah], 28h
        mov    eax, 28h
        jmp    SHORT loc_4_140F0
loc_4_140A4:
        cmp    bl, 60h
        jae    SHORT loc_4_140B6
        mov    word ptr [esi + 1ah], 28h
        mov    eax, 28h
        jmp    SHORT loc_4_140F0
loc_4_140B6:
        mov    ecx, dword ptr [esi + 7ch]
        cmp    byte ptr [ecx + 30h], 1
        jne    SHORT loc_4_140E3
        cmp    eax, 0dh
        jne    SHORT loc_4_140E3
        cmp    bl, 6fh
        je     SHORT loc_4_140E3
        cmp    bl, 60h
        jne    SHORT loc_4_140D6
        mov    eax, dword ptr [esi + 14h]
        test   byte ptr [eax], 1
        je     SHORT loc_4_140E3
loc_4_140D6:
        mov    word ptr [esi + 1ah], 28h
        mov    eax, 28h
        jmp    SHORT loc_4_140F0
loc_4_140E3:
        push   0
        push   0bh
        push   esi
        call   sub_4_13D1D
        add    esp, 0ch
loc_4_140F0:
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   0ch
        push   dword ptr [ebp + 8]
        call   sub_4_13E08
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        push   0dh
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_13E08
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_14138
        mov    eax, dword ptr [esi + 74h]
        mov    eax, dword ptr [eax + 10h]
        cmp    byte ptr [eax + 30h], 0
        jbe    SHORT loc_4_14138
        push   2
        push   esi
        call   sub_4_122E1
        add    esp, 8
        jmp    SHORT loc_4_1413C
loc_4_14138:
        movsx  eax, word ptr [esi + 1ah]
loc_4_1413C:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 023h, 023h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_1417F
        mov    eax, dword ptr [esi + 7ch]
        cmp    byte ptr [eax + 30h], 2
        jne    SHORT loc_4_14162
        cmp    byte ptr [esi + 4], 0
        je     SHORT loc_4_1417F
loc_4_14162:
        cmp    byte ptr [esi + 4], 1
        ja     SHORT loc_4_14172
        add    esi, 1ah
        mov    word ptr [esi], 107h
        jmp    SHORT loc_4_1417A
loc_4_14172:
        add    esi, 1ah
        mov    word ptr [esi], 28h
loc_4_1417A:
        movsx  eax, word ptr [esi]
        jmp    SHORT loc_4_1418C
loc_4_1417F:
        push   0
        push   0eh
        push   esi
        call   sub_4_13D1D
        add    esp, 0ch
loc_4_1418C:
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 034h, 026h, 0FAh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 7ch]
        test   byte ptr [eax + 32h], 20h
        jne    SHORT loc_4_141B6
        jmp    SHORT loc_4_141C6
loc_4_141B6:
        mov    eax, dword ptr [esi + 74h]
        test   byte ptr [eax + 1ch], 40h
        jne    SHORT loc_4_141C6
        mov    ecx, 1
        jmp    SHORT loc_4_141CB
loc_4_141C6:
        or     word ptr [esi + 18h], 1
loc_4_141CB:
        push   ecx
        push   1
        push   esi
        db  0E8h, 0FEh, 025h, 0FAh, 0FFh
        add    esp, 0ch
        cmp    byte ptr [esi + 4], 2
        je     SHORT loc_4_141E6
        mov    eax, dword ptr [esi + 74h]
        test   byte ptr [eax + 1fh], 0ch
        je     SHORT loc_4_141F1
loc_4_141E6:
        push   dword ptr [esi + 74h]
        call   sub_4_12626
        add    esp, 4
loc_4_141F1:
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        sub    ecx, ecx
        mov    edx, dword ptr [eax + 74h]
        test   byte ptr [edx + 1ch], 40h
        jne    SHORT loc_4_14210
        mov    ecx, 1
        jmp    SHORT loc_4_14215
loc_4_14210:
        or     word ptr [eax + 18h], 1
loc_4_14215:
        push   ecx
        push   2
        push   eax
        db  0E8h, 0B4h, 025h, 0FAh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   3
        push   dword ptr [ebp + 8]
        db  0E8h, 0A1h, 025h, 0FAh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [eax + 4]
        cmp    cl, 1
        je     SHORT loc_4_14251
        cmp    cl, 5
        je     SHORT loc_4_14251
        cmp    cl, 7
        je     SHORT loc_4_14251
        push   0
        jmp    SHORT loc_4_14253
loc_4_14251:
        push   1
loc_4_14253:
        push   4
        push   eax
        db  0E8h, 077h, 025h, 0FAh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   5
        push   dword ptr [ebp + 8]
        db  0E8h, 064h, 025h, 0FAh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   6
        push   dword ptr [ebp + 8]
        db  0E8h, 051h, 025h, 0FAh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        push   7
        push   dword ptr [ebp + 8]
        db  0E8h, 03Eh, 025h, 0FAh, 0FFh
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 89h]
        push   eax
        push   esi
        call   sub_4_12200
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_14322
        movzx  eax, byte ptr [esi + 89h]
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        push   esi
        mov    edi, dword ptr [eax]
IFSMgr_TransMatch:
        mov    dword ptr [esi + 7ch], edi
        db  0E8h, 0AEh, 00Ah, 0FAh, 0FFh
        add    esp, 4
        push   esi
        db  0E8h, 099h, 021h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_142E5
        mov    word ptr [esi + 1ah], 5
        jmp    SHORT loc_4_14322
loc_4_142E5:
        mov    ax, word ptr [edi + 32h]
        and    ax, 10h
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        mov    word ptr [esi + 0b4h], ax
        mov    ax, word ptr [edi + 32h]
        and    ax, 0ffefh
        mov    word ptr [edi + 32h], ax
        and    ax, 4
        cmp    ax, 1
        cmc
        sbb    eax, eax
        and    eax, 2
        or     word ptr [esi + 0b4h], ax
        and    word ptr [edi + 32h], 0fffbh
loc_4_14322:
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
        mov    esi, dword ptr [ebp + 8]
        movzx  edi, word ptr [esi + 9ch]
        mov    ax, word ptr [esi + 98h]
        or     ax, ax
        je     SHORT loc_4_14356
        cmp    ax, 1
        je     SHORT loc_4_14356
        mov    word ptr [esi + 1ah], 1
        jmp    NEAR PTR loc_4_144FA
loc_4_14356:
        mov    al, byte ptr [esi + 89h]
        push   eax
        push   esi
        call   sub_4_12200
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_144FA
        movzx  eax, byte ptr [esi + 89h]
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi + 7ch], eax
        or     eax, eax
        mov    dword ptr [ebp - 4], eax
        jne    SHORT loc_4_14393
        mov    word ptr [esi + 1ah], 0fh
        jmp    NEAR PTR loc_4_144FA
loc_4_14393:
        push   esi
        db  0E8h, 0E2h, 009h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 16h]
        or     ax, ax
        je     NEAR PTR loc_4_144F4
        movzx  eax, ax
        cmp    eax, edi
        jle    NEAR PTR loc_4_144F4
        push   esi
        db  0E8h, 015h, 0F4h, 0F9h, 0FFh
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_143CF
        mov    word ptr [esi + 1ah], 8
        jmp    NEAR PTR loc_4_144FA
loc_4_143CF:
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, dword ptr [eax + 24h]
        or     ebx, ebx
        je     NEAR PTR loc_4_144F4
loc_4_143DD:
        mov    al, byte ptr [ebx + 1fh]
        test   al, 0ch
        jne    SHORT loc_4_14419
        cmp    word ptr [esi + 98h], 1
        jne    SHORT loc_4_143F2
        test   al, 60h
        je     SHORT loc_4_14419
loc_4_143F2:
        or     edi, edi
        jne    SHORT loc_4_14418
        mov    dword ptr [esi + 74h], ebx
        push   esi
        db  0E8h, 02Ch, 000h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_14419
        push   esi
        db  0E8h, 0F2h, 004h, 0FAh, 0FFh
        add    esp, 4
loc_4_1440F:
        or     ebx, ebx
        jne    SHORT loc_4_14422
        jmp    NEAR PTR loc_4_144F4
loc_4_14418:
        dec    edi
loc_4_14419:
        mov    ebx, dword ptr [ebx + 2ch]
        or     ebx, ebx
        jne    SHORT loc_4_143DD
        jmp    SHORT loc_4_1440F
loc_4_14422:
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax + 30h], 1
        jb     SHORT loc_4_1443C
        cmp    dword ptr [ebx + 24h], 0
        je     SHORT loc_4_1443C
        push   esi
        db  0E8h, 015h, 026h, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_4_14463
loc_4_1443C:
        mov    ax, word ptr [ebx + 30h]
        mov    word ptr [esi + 6], ax
        mov    byte ptr [esi + 4], 1
        push   0
        push   esi
        mov    eax, dword ptr [ebx + 8]
        push   11h
        push   dword ptr [eax + 20h]
        db  0E8h, 038h, 0F7h, 0F9h, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_144FA
loc_4_14463:
        mov    edi, dword ptr [esi + 14h]
        mov    al, byte ptr [esi + 89h]
        add    al, 41h
        mov    byte ptr [edi], al
        inc    edi
        mov    byte ptr [edi], 3ah
        inc    edi
        mov    al, byte ptr [esi + 8bh]
        and    eax, 3
        push   eax
        push   103h
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 4
        push   eax
        push   edi
        call   sub_4_14A26
        add    esp, 10h
        mov    byte ptr [eax + edi], 0
        mov    word ptr [esi + 0b0h], 0
        test   byte ptr [ebx + 1fh], 20h
        je     SHORT loc_4_144C6
        mov    word ptr [esi + 0b0h], 1
        mov    al, byte ptr [ebx + 1eh]
        and    al, 2
        cmp    al, 1
        cmc
        sbb    eax, eax
        and    eax, 2
        mov    word ptr [esi + 0b4h], ax
        jmp    SHORT loc_4_144FA
loc_4_144C6:
        test   byte ptr [ebx + 23h], 10h
        je     SHORT loc_4_144D5
        mov    word ptr [esi + 0b0h], 4
loc_4_144D5:
        test   byte ptr [ebx + 1fh], 40h
        je     SHORT loc_4_144E3
        or     word ptr [esi + 0b0h], 2
loc_4_144E3:
        mov    ax, word ptr [ebx + 22h]
        and    ax, 0ffh
        mov    word ptr [esi + 0b4h], ax
        jmp    SHORT loc_4_144FA
loc_4_144F4:
        mov    word ptr [esi + 1ah], 12h
loc_4_144FA:
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
        mov    al, byte ptr [esi + 89h]
        push   eax
        push   esi
        call   sub_4_12200
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_4_145CD
        movzx  eax, byte ptr [esi + 89h]
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        mov    edi, dword ptr [eax]
        mov    dword ptr [esi + 7ch], edi
        or     edi, edi
        jne    SHORT loc_4_14543
        mov    word ptr [esi + 1ah], 0fh
        jmp    NEAR PTR loc_4_145CD
loc_4_14543:
        test   byte ptr [edi + 1bh], 10h
        jne    SHORT loc_4_14558
        mov    word ptr [esi + 1ah], 1
        mov    dword ptr [esi + 7ch], 0
        jmp    SHORT loc_4_145CD
loc_4_14558:
        push   esi
        db  0E8h, 01Dh, 008h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [esi + 0b0h], 0
        movzx  eax, byte ptr [edi + 30h]
        dec    eax
        mov    word ptr [esi + 0b4h], ax
        js     SHORT loc_4_145CD
        mov    ax, word ptr [edi + 32h]
        and    ax, 20h
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        or     ax, word ptr [esi + 0b0h]
        mov    word ptr [esi + 0b0h], ax
        mov    cx, word ptr [edi + 32h]
        and    cx, 8
        cmp    cx, 1
        cmc
        sbb    ecx, ecx
        and    ecx, 2
        or     cx, ax
        mov    word ptr [esi + 0b0h], cx
        mov    ax, word ptr [edi + 32h]
        and    ax, 100h
        cmp    ax, 1
        cmc
        sbb    eax, eax
        and    eax, 4
        or     ax, cx
        mov    word ptr [esi + 0b0h], ax
loc_4_145CD:
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        movzx  eax, byte ptr [ebx + 0b4h]
        mov    dword ptr [ebp - 4], eax
        mov    al, byte ptr [ebx + 8bh]
        test   al, 10h
        je     SHORT loc_4_145FC
        mov    eax, dword ptr [ebx + 0ach]
        jmp    NEAR PTR loc_4_14705
loc_4_145FC:
        test   al, 4
        lea    esi, [ebx + 14h]
        je     SHORT loc_4_1460B
        mov    eax, dword ptr [ebx + 0a8h]
        jmp    SHORT loc_4_14618
loc_4_1460B:
        push   3c10h
        call   sub_4_2154
        add    esp, 4
loc_4_14618:
        mov    dword ptr [esi], eax
        cmp    byte ptr [ebx + 88h], 0e9h
        mov    word ptr [ebx + 18h], 0
        jne    SHORT loc_4_1469F
        cmp    word ptr [ebx + 0b0h], 0ffffh
        je     SHORT loc_4_1463F
        db  066h, 0C7h, 043h, 01Ah
IFSMgr_PNPEvent:
        add    dword ptr [eax], eax
        jmp    NEAR PTR loc_4_1487A
loc_4_1463F:
        mov    byte ptr [ebx + 88h], 0ddh
        mov    ax, word ptr [ebx + 9ch]
        and    ax, 1
        cmp    ax, 1
        jne    SHORT loc_4_146CF
        mov    byte ptr [ebx + 88h], 0deh
loc_4_1465E:
        mov    word ptr [ebx + 18h], 1
        cmp    byte ptr [ebx + 88h], 0deh
        jne    SHORT loc_4_14680
        mov    ax, word ptr [ebx + 9ch]
        and    ax, 0e001h
        or     ax, 1
        mov    word ptr [ebx + 18h], ax
loc_4_14680:
        movzx  eax, byte ptr [ebx + 0ach]
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    SHORT loc_4_1469C
        mov    eax, dword ptr [ebx + 94h]
        movzx  eax, byte ptr [eax + 0ah]
        inc    eax
        mov    dword ptr [ebp - 4], eax
loc_4_1469C:
        dec    dword ptr [ebp - 4]
loc_4_1469F:
        mov    eax, dword ptr [ebx]
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        jne    SHORT loc_4_146FE
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebx + 24h], eax
        movzx  eax, word ptr [ecx + 4]
        db  089h
NotifyVolumeArrival:
        add    ecx, dword ptr [ebx + 9083h]
        add    bh, dh
        add    byte ptr [eax + 74000001h], al
        pop    ss
        mov    eax, dword ptr [ecx + 6]
        mov    dword ptr [esi], eax
        jmp    SHORT loc_4_14708
loc_4_146CF:
        or     ax, ax
        je     SHORT loc_4_1465E
        mov    word ptr [ebx + 1ah], 1
        jmp    NEAR PTR loc_4_1487A
        db  0F6h, 083h, 08Bh, 000h, 000h, 000h, 004h, 074h, 007h, 08Bh, 041h, 006h, 089h, 006h, 0EBh, 019h
        db  0FFh, 071h, 006h, 0E8h, 0ABh, 0DAh, 0FEh, 0FFh, 083h, 0C4h, 004h, 089h, 006h, 0EBh, 00Ah
loc_4_146FE:
        movzx  eax, word ptr [ebx + 0ach]
loc_4_14705:
        mov    dword ptr [ebx + 24h], eax
loc_4_14708:
        mov    eax, dword ptr [ebp - 4]
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 001h
        je     SHORT loc_4_1474E
        mov    esi, eax
        push   1
        push   esi
        push   ebx
        call   sub_4_4336
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_14735
        push   0
        push   esi
        call   sub_4_4114
        add    esp, 8
        mov    word ptr [ebx + 1ah], ax
loc_4_14735:
        cmp    word ptr [ebx + 1ah], 0
        je     SHORT loc_4_1474E
        mov    dword ptr [ebx + 7ch], 0
        mov    word ptr [ebx + 1ah], 201h
        jmp    NEAR PTR loc_4_1487A
loc_4_1474E:
        mov    eax, dword ptr [ebp - 4]
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 7ch], eax
        or     eax, eax
        jne    SHORT loc_4_1476C
        mov    word ptr [ebx + 1ah], 0fh
        jmp    NEAR PTR loc_4_1487A
loc_4_1476C:
        push   ebx
        db  0E8h, 009h, 006h, 0FAh, 0FFh
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        inc    al
        mov    byte ptr [ebx + 89h], al
        mov    eax, dword ptr [ebx + 7ch]
        cmp    byte ptr [ebx + 88h], 0ddh
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebx + 1ch], eax
        jne    SHORT loc_4_1479B
        mov    byte ptr [ebx + 4], 0
        jmp    NEAR PTR loc_4_14864
loc_4_1479B:
        mov    eax, dword ptr [ebx + 7ch]
        mov    ecx, dword ptr [eax + 20h]
        cmp    byte ptr [eax + 30h], 0
        jne    NEAR PTR loc_4_14860
        test   byte ptr [ecx + 2], 20h
        jne    NEAR PTR loc_4_14860
        test   byte ptr [ecx + 4], 40h
        je     SHORT loc_4_147C6
        mov    word ptr [ebx + 1ah], 300h
        jmp    NEAR PTR loc_4_1487A
loc_4_147C6:
        mov    esi, dword ptr [ebx + 14h]
        mov    eax, dword ptr [ecx + 10h]
        imul   eax, dword ptr [ebx]
        mov    dword ptr [ebp - 0ch], eax
        push   1
        push   eax
        db  0E8h, 01Eh, 03Eh, 000h, 000h
        add    esp, 8
        db  089h
QueryVolumeRemoval:
        inc    ebx
        adc    al, 89h
        inc    ebp
        clc
        or     eax, eax
        jne    SHORT loc_4_147F2
        mov    word ptr [ebx + 1ah], 300h
        jmp    NEAR PTR loc_4_1487A
loc_4_147F2:
        mov    byte ptr [ebx + 4], 0
        push   0
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   2dh
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 3ch]
        db  0E8h, 087h, 0F3h, 0F9h, 0FFh
        add    esp, 10h
        mov    dword ptr [ebx + 14h], esi
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, eax
        shr    ecx, 2
        mov    edi, dword ptr [ebp - 8]
        repe cmpsd
        jne    SHORT loc_4_14836
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_4_14836
        push   0
        push   dword ptr [ebp - 8]
        db  0E8h, 0CFh, 03Dh, 000h, 000h
        add    esp, 8
        jmp    SHORT loc_4_1487A
loc_4_14836:
        push   0
        push   dword ptr [ebp - 8]
        db  0E8h, 0C0h, 03Dh, 000h, 000h
        add    esp, 8
        push   ebx
        call   sub_4_12199
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_4_14858
        mov    word ptr [ebx + 1ah], 300h
        jmp    SHORT loc_4_1487A
loc_4_14858:
        mov    word ptr [ebx + 1ah], 0ffbfh
        jmp    SHORT loc_4_1487A
loc_4_14860:
        mov    byte ptr [ebx + 4], 1
loc_4_14864:
        push   0
        push   ebx
        mov    eax, dword ptr [ebx + 7ch]
        push   2dh
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 3ch]
        db  0E8h, 019h, 0F3h, 0F9h, 0FFh
        add    esp, 10h
loc_4_1487A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_14881:
        push   ebp
        mov    ebp, esp
        sub    esp, 104h
        push   esi
        mov    al, byte ptr [ebp + 8]
        cmp    al, 20h
        jae    NEAR PTR loc_4_14951
        movzx  eax, al
        db  0F6h, 080h, 08Ch, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_4_14951
        db  08Bh, 004h, 085h, 0C0h, 065h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_4_1494D
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     NEAR PTR loc_4_1494D
        lea    eax, [ebp - 104h]
        push   eax
        call   sub_4_2751
        add    esp, 4
        mov    cl, byte ptr [ebp + 0ch]
        movzx  ax, cl
        mov    word ptr [ebp - 0ech], ax
        mov    dword ptr [ebp - 88h], esi
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 0e8h], eax
        cmp    cl, 2
        jne    SHORT loc_4_1490D
        mov    al, byte ptr [esi + 30h]
        cmp    al, 1
        je     SHORT loc_4_148FB
        cmp    al, 4
        db  075h, 00Bh
loc_4_148FB:
        mov    eax, dword ptr [ebp - 0fch]
        db  039h
NotifyVolumeRemoval:
        inc    esi
        sub    byte ptr [edi + eax - 48h], dh
        add    eax, 0eb000000h
        dec    ecx
loc_4_1490D:
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 062h, 004h, 0FAh, 0FFh
        add    esp, 4
        push   0
        lea    eax, [ebp - 104h]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        push   22h
        push   dword ptr [eax + 10h]
        db  0E8h, 05Eh, 0F2h, 0F9h, 0FFh
        add    esp, 10h
        lea    eax, [ebp - 104h]
        push   eax
        db  0E8h, 0BDh, 0FFh, 0F9h, 0FFh
        add    esp, 4
        movsx  eax, word ptr [ebp - 0eah]
        jmp    SHORT loc_4_14956
loc_4_1494D:
        sub    eax, eax
        jmp    SHORT loc_4_14956
loc_4_14951:
        mov    eax, 0fh
loc_4_14956:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 0B8h, 088h, 001h, 000h, 000h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 1
        je     NEAR PTR loc_4_149F7
        cmp    ecx, 0ch
        ja     SHORT loc_4_149F7
        mov    edi, dword ptr [ebp + 8]
        mov    eax, 20202020h
        cld
        stosd
        stosd
        stosb
        stosw
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        sub    eax, eax
        mov    dword ptr [ebp - 4], eax
        lea    ebx, [ebp - 3]
loc_4_14997:
        lodsw
        call   sub_4_14D81
        or     edx, edx
        jne    SHORT loc_4_149F7
        cmp    eax, 2eh
        je     SHORT loc_4_149BC
        inc    byte ptr [ebx]
        or     ah, ah
        jne    SHORT loc_4_149B2
        stosb
        loop   loc_4_14997
        jmp    SHORT loc_4_149CA
loc_4_149B2:
        inc    byte ptr [ebx]
        xchg   al, ah
        stosw
        loop   loc_4_14997
        jmp    SHORT loc_4_149CA
loc_4_149BC:
        inc    byte ptr [ebp - 2]
        mov    edi, dword ptr [ebp + 8]
        lea    edi, [edi + 8]
        lea    ebx, [ebp - 4]
        loop   loc_4_14997
loc_4_149CA:
        cmp    byte ptr [ebp - 4], 3
        ja     SHORT loc_4_149F7
        cmp    byte ptr [ebp - 2], 1
        ja     SHORT loc_4_149F7
        cmp    byte ptr [ebp - 3], 8
        ja     SHORT loc_4_149F7
        cmp    byte ptr [ebp - 3], 0
        je     SHORT loc_4_149F7
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx], 0e5h
        je     SHORT loc_4_149F2
loc_4_149EA:
        or     eax, 1
loc_4_149ED:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_4_149F2:
        mov    byte ptr [ecx], 5
        jmp    SHORT loc_4_149EA
loc_4_149F7:
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], al
        jmp    SHORT loc_4_149ED
sub_4_14A00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 14h]
        add    edx, edi
        call   sub_4_14DD7
        mov    eax, edi
        sub    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_14A26:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 10h]
        add    edx, edi
        push   0
loc_4_14A39:
        mov    ebx, dword ptr [ebp + 14h]
        lodsw
        movzx  ecx, ax
        jecxz  loc_4_14A5B
        sub    ecx, 2
        mov    al, 5ch
        stosb
        push   edx
        call   sub_4_14DD7
        mov    eax, edx
        pop    edx
        or     dword ptr [esp], eax
        cmp    edx, edi
        jbe    SHORT loc_4_14A5E
        jmp    SHORT loc_4_14A39
loc_4_14A5B:
        mov    byte ptr [edi], 0
loc_4_14A5E:
        mov    eax, edi
        sub    eax, dword ptr [ebp + 8]
        pop    edx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_14A69:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_4_14E76
        mov    eax, edi
        sub    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_14A8A:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        sub    eax, eax
        sub    edx, edx
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 1
        je     SHORT loc_4_14AB9
loc_4_14AA0:
        lodsw
        cmp    ax, 586h
        ja     SHORT loc_4_14AB5
        db  08Ah, 090h, 0F2h, 001h, 000h, 000h
        db  066h, 02Bh, 082h, 0A4h, 001h, 000h, 000h
loc_4_14AB5:
        stosw
        loop   loc_4_14AA0
loc_4_14AB9:
        mov    eax, edi
        sub    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        leave
        ret
sub_4_14AC2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 20h
        mov    ecx, 8
        lea    edi, [ebx + 0eh]
        std
        repe scasw
        cld
        mov    edi, edx
        je     SHORT loc_4_14AEC
        inc    ecx
        lea    esi, [ebx]
        rep movsw
        mov    edx, edi
loc_4_14AEC:
        lea    edi, [ebx + 14h]
        mov    ecx, 3
        std
        repe scasw
        cld
        jne    SHORT loc_4_14B03
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_4_14B19
        jmp    SHORT loc_4_14B07
loc_4_14B03:
        inc    ecx
        lea    esi, [ebx + 10h]
loc_4_14B07:
        mov    edi, edx
        cmp    word ptr [esi - 2], 2eh
        je     SHORT loc_4_14B14
        mov    al, 2eh
        stosw
loc_4_14B14:
        rep movsw
        mov    edx, edi
loc_4_14B19:
        mov    eax, edx
        sub    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_14B23:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    ax, 20h
        mov    ecx, 8
        mov    edx, dword ptr [ebp + 10h]
        shr    edx, 1
        je     SHORT loc_4_14B49
loc_4_14B3E:
        cmp    word ptr [esi], 2eh
        je     SHORT loc_4_14B49
        movsw
        dec    edx
        loopne loc_4_14B3E
loc_4_14B49:
        rep stosw
        test   edx, edx
        je     SHORT loc_4_14B5A
        cmp    word ptr [esi], 2eh
        jne    SHORT loc_4_14B5A
        add    esi, 2
        dec    edx
loc_4_14B5A:
        mov    ecx, 3
        sub    edx, ecx
        jae    SHORT loc_4_14B6F
        add    ecx, edx
        neg    edx
        rep movsw
        mov    ecx, edx
        rep stosw
loc_4_14B6F:
        rep movsw
        mov    eax, 16h
        pop    edi
        pop    esi
        leave
        ret
sub_4_14B7B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        sub    edx, edx
        sub    eax, eax
        test   dword ptr [ebp + 10h], 6
        je     NEAR PTR loc_4_14C56
        test   dword ptr [ebp + 10h], 1
        jne    SHORT loc_4_14BC7
loc_4_14BA1:
        lodsw
        cmp    ax, 586h
        ja     SHORT loc_4_14BB6
        db  08Ah, 090h, 0F2h, 001h, 000h, 000h
        db  066h, 02Bh, 082h, 0A4h, 001h, 000h, 000h
loc_4_14BB6:
        scasw
        jne    NEAR PTR loc_4_14CAB
        test   eax, eax
        jne    SHORT loc_4_14BA1
        jmp    NEAR PTR loc_4_14CAB
loc_4_14BC7:
        cmp    dword ptr [edi], 2ah
        je     NEAR PTR loc_4_14C6B
sub_4_14BD0:
        cmp    dword ptr [edi], 2e002ah
        jne    SHORT loc_4_14BE2
        cmp    dword ptr [edi + 4], 2ah
        je     NEAR PTR loc_4_14C6B
loc_4_14BE2:
        sub    ecx, ecx
loc_4_14BE4:
        mov    ax, word ptr [esi]
        cmp    ax, 2eh
        jne    SHORT loc_4_14BF4
        or     dword ptr [ebp + 10h], 80000000h
loc_4_14BF4:
        cmp    ax, 586h
        ja     SHORT loc_4_14C08
        db  00Fh, 0B6h, 098h, 0F2h, 001h, 000h, 000h
        db  066h, 02Bh, 083h, 0A4h, 001h, 000h, 000h
loc_4_14C08:
        movzx  ebx, word ptr [edi]
        cmp    eax, ebx
        je     SHORT loc_4_14C32
        test   ebx, ebx
        je     SHORT loc_4_14C21
        cmp    ebx, 2ah
        je     SHORT loc_4_14C3A
        test   eax, eax
        je     SHORT loc_4_14C52
        cmp    ebx, 3fh
        je     SHORT loc_4_14C36
loc_4_14C21:
        jecxz  loc_4_14C52
        cmp    word ptr [ecx], 0
        je     SHORT loc_4_14C52
        lea    esi, [ecx + 2]
        mov    ecx, esi
        mov    edi, edx
        jmp    SHORT loc_4_14BE4
loc_4_14C32:
        test   eax, eax
        je     SHORT loc_4_14C6B
loc_4_14C36:
        cmpsw
        jmp    SHORT loc_4_14BE4
loc_4_14C3A:
        add    edi, 2
        cmp    word ptr [edi], bx
        je     SHORT loc_4_14C3A
        cmp    word ptr [edi], 0
        je     SHORT loc_4_14C6B
        test   eax, eax
        je     SHORT loc_4_14C08
        mov    ecx, esi
        mov    edx, edi
        jmp    SHORT loc_4_14C08
loc_4_14C52:
        or     al, 1
        jmp    SHORT loc_4_14C6B
loc_4_14C56:
        mov    ecx, 0bh
loc_4_14C5B:
        repe cmpsw
        je     SHORT loc_4_14C6B
        cmp    word ptr [edi - 2], 3fh
        jne    SHORT loc_4_14C6B
        test   ecx, ecx
        jne    SHORT loc_4_14C5B
loc_4_14C6B:
        sete   al
        movzx  eax, al
        jne    SHORT loc_4_14C78
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_4_14C78:
        cmp    word ptr [esi], 0
        jne    SHORT loc_4_14CB1
        test   dword ptr [ebp + 10h], 4
        je     SHORT loc_4_14CB1
        test   dword ptr [ebp + 10h], 80000000h
        jne    SHORT loc_4_14CB1
        cmp    word ptr [edi], 2eh
        je     SHORT loc_4_14C9C
        cmp    word ptr [edi], 3fh
        jne    SHORT loc_4_14CB1
loc_4_14C9C:
        add    edi, 2
        cmp    word ptr [edi], 0
        je     SHORT loc_4_14CAB
        cmp    word ptr [edi], 2ah
        je     SHORT loc_4_14C9C
loc_4_14CAB:
        sete   al
        movzx  eax, al
loc_4_14CB1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edx, edx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        lea    esi, [esi + 14h]
        lea    edi, [edi + 14h]
        mov    ecx, 3
        std
        repe cmpsw
        jne    NEAR PTR loc_4_14D5E
        mov    ecx, 8
        mov    ax, 7eh
        repne scasw
        jne    SHORT loc_4_14D35
        lea    ecx, [edi + 2]
        sub    ecx, dword ptr [ebp + 0ch]
        shr    ecx, 1
        mov    eax, 7
        sub    eax, ecx
        jecxz  loc_4_14D03
        mov    esi, dword ptr [ebp + 8]
        lea    esi, [esi + ecx*2 - 2]
        repe cmpsw
        jne    SHORT loc_4_14D5E
loc_4_14D03:
        mov    ecx, eax
        jecxz  loc_4_14D5E
        neg    eax
        mov    esi, dword ptr [ebp + 0ch]
        lea    esi, [esi + eax*2 + 10h]
        cmp    word ptr [esi], 30h
        je     SHORT loc_4_14D5E
        cld
        sub    eax, eax
loc_4_14D19:
        lodsw
        cmp    ax, 20h
        je     SHORT loc_4_14D52
        sub    ax, 30h
        cmp    ax, 9
        ja     SHORT loc_4_14D5C
        lea    edx, [edx + edx*4]
        lea    edx, [eax + edx*2]
        loop   loc_4_14D19
        jmp    SHORT loc_4_14D5E
loc_4_14D35:
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        lea    esi, [esi + 0eh]
        lea    edi, [edi + 0eh]
        mov    ecx, 8
        repe cmpsw
        jne    SHORT loc_4_14D5E
        mov    edx, 0ffffffffh
        jmp    SHORT loc_4_14D5E
loc_4_14D52:
        dec    ecx
        je     SHORT loc_4_14D5E
        mov    edi, esi
        repe scasw
        je     SHORT loc_4_14D5E
loc_4_14D5C:
        sub    edx, edx
loc_4_14D5E:
        cld
        mov    eax, edx
        pop    edi
        pop    esi
        leave
        ret
sub_4_14D65:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        lea    ecx, [eax - 1]
        repne scasw
        not    ecx
        lea    eax, [ecx - 1]
        pop    edi
        leave
        ret
        db  00Fh, 0B7h, 044h, 024h, 004h
sub_4_14D81:
        push   ebx
        db  08Bh, 01Dh, 090h, 001h, 000h, 000h
        sub    edx, edx
        jmp    SHORT loc_4_14D90
loc_4_14D8C:
        or     edx, edx
        je     SHORT loc_4_14DAB
loc_4_14D90:
        cmp    ax, word ptr [edx + ebx + 4]
        jae    SHORT loc_4_14D9D
        movzx  edx, word ptr [edx + ebx]
        jmp    SHORT loc_4_14D8C
loc_4_14D9D:
        cmp    ax, word ptr [edx + ebx + 6]
        jbe    SHORT loc_4_14DB7
        movzx  edx, word ptr [edx + ebx + 2]
        jmp    SHORT loc_4_14D8C
loc_4_14DAB:
        mov    eax, 5fh
        pop    ebx
        mov    edx, 1
        ret
loc_4_14DB7:
        sub    ax, word ptr [edx + ebx + 4]
        mov    edx, dword ptr [edx + ebx + 8]
        shr    edx, 1
        lea    edx, [edx + ebx]
        jae    SHORT loc_4_14DCF
        movzx  eax, byte ptr [eax + edx]
        pop    ebx
        sub    edx, edx
        ret
loc_4_14DCF:
        movzx  eax, word ptr [edx + eax*2]
        pop    ebx
        sub    edx, edx
        ret
sub_4_14DD7:
        sub    eax, eax
        push   eax
        push   edx
        push   eax
        db  08Bh, 01Ch, 09Dh, 08Ch, 001h, 000h, 000h
        shr    ecx, 1
        je     SHORT loc_4_14E24
loc_4_14DE7:
        cmp    edi, dword ptr [esp + 4]
        jae    SHORT loc_4_14E24
        sub    edx, edx
        lodsw
        or     eax, eax
        je     SHORT loc_4_14E24
        mov    word ptr [esp], ax
loc_4_14DF9:
        cmp    ax, word ptr [edx + ebx + 4]
        jae    SHORT loc_4_14E0A
        movzx  edx, word ptr [edx + ebx]
        or     edx, edx
        jne    SHORT loc_4_14DF9
        jmp    SHORT loc_4_14E1A
loc_4_14E0A:
        cmp    ax, word ptr [edx + ebx + 6]
        jbe    SHORT loc_4_14E28
        movzx  edx, word ptr [edx + ebx + 2]
        or     edx, edx
        jne    SHORT loc_4_14DF9
loc_4_14E1A:
        or     dword ptr [esp + 8], 1
        mov    al, 5fh
        stosb
        loop   loc_4_14DE7
loc_4_14E24:
        pop    eax
        pop    eax
        pop    edx
        ret
loc_4_14E28:
        sub    ax, word ptr [edx + ebx + 4]
        mov    edx, dword ptr [edx + ebx + 8]
        shr    edx, 1
        lea    edx, [edx + ebx]
        jae    SHORT loc_4_14E46
        mov    al, byte ptr [eax + edx]
        stosb
        cmp    al, 5fh
        je     SHORT loc_4_14E68
loc_4_14E40:
        loop   loc_4_14DE7
        pop    eax
        pop    eax
        pop    edx
        ret
loc_4_14E46:
        mov    ax, word ptr [edx + eax*2]
        xchg   ah, al
        dec    dword ptr [esp + 4]
        cmp    edi, dword ptr [esp + 4]
        jae    SHORT loc_4_14E24
        stosw
        inc    dword ptr [esp + 4]
        cmp    ax, 5f5fh
        je     SHORT loc_4_14E6F
        loop   loc_4_14DE7
        pop    eax
        pop    eax
        pop    edx
        ret
loc_4_14E68:
        cmp    word ptr [esp], 5fh
        je     SHORT loc_4_14E40
loc_4_14E6F:
        or     dword ptr [esp + 8], 1
        jmp    SHORT loc_4_14E40
sub_4_14E76:
        push   0
        push   0
        jecxz  loc_4_14EF1
        db  08Bh, 01Ch, 09Dh, 094h, 001h, 000h, 000h
loc_4_14E83:
        sub    eax, eax
        lodsb
        bt     dword ptr [ebx], eax
        jb     SHORT loc_4_14E91
        mov    ah, al
        lodsb
        dec    ecx
        jecxz  loc_4_14EC5
loc_4_14E91:
        sub    edx, edx
loc_4_14E93:
        cmp    ax, word ptr [edx + ebx + 24h]
        jae    SHORT loc_4_14EA5
        movzx  edx, word ptr [edx + ebx + 20h]
        or     edx, edx
        jne    SHORT loc_4_14E93
        jmp    SHORT loc_4_14EB5
loc_4_14EA5:
        cmp    ax, word ptr [edx + ebx + 26h]
        jbe    SHORT loc_4_14ED1
        movzx  edx, word ptr [edx + ebx + 22h]
        or     edx, edx
        jne    SHORT loc_4_14E93
loc_4_14EB5:
        mov    ax, 0fffdh
        stosw
        or     dword ptr [esp + 4], 1
        loop   loc_4_14E83
        pop    edx
        pop    edx
        ret
loc_4_14EC5:
        mov    ax, 0fffdh
        stosw
        pop    edx
        pop    edx
        or     edx, 2
        ret
loc_4_14ED1:
        mov    dword ptr [esp], eax
        sub    ax, word ptr [edx + ebx + 24h]
        mov    edx, dword ptr [edx + ebx + 28h]
        shr    edx, 1
        lea    edx, [edx + ebx + 20h]
        mov    ax, word ptr [edx + eax*2]
        stosw
        cmp    ax, 5fh
        je     SHORT loc_4_14EF4
loc_4_14EEF:
        loop   loc_4_14E83
loc_4_14EF1:
        pop    edx
        pop    edx
        ret
loc_4_14EF4:
        cmp    dword ptr [esp], 5fh
        je     SHORT loc_4_14EEF
        or     dword ptr [esp + 4], 1
        jmp    SHORT loc_4_14EEF
sub_4_14F01:
        mov    edi, esi
        jecxz  loc_4_14F67
        db  08Bh, 01Ch, 09Dh, 094h, 001h, 000h, 000h
loc_4_14F0C:
        sub    eax, eax
        lodsb
        bt     dword ptr [ebx], eax
        jb     SHORT loc_4_14F17
        mov    ah, al
        lodsb
loc_4_14F17:
        sub    edx, edx
loc_4_14F19:
        cmp    ax, word ptr [edx + ebx + 24h]
        jae    SHORT loc_4_14F2B
        movzx  edx, word ptr [edx + ebx + 20h]
        or     edx, edx
        jne    SHORT loc_4_14F19
        jmp    SHORT loc_4_14F3B
loc_4_14F2B:
        cmp    ax, word ptr [edx + ebx + 26h]
        jbe    SHORT loc_4_14F3E
        movzx  edx, word ptr [edx + ebx + 22h]
        or     edx, edx
        jne    SHORT loc_4_14F19
loc_4_14F3B:
        loop   loc_4_14F0C
        ret
loc_4_14F3E:
        sub    ax, word ptr [edx + ebx + 24h]
        mov    edx, dword ptr [edx + ebx + 28h]
        shr    edx, 1
        lea    edx, [edx + ebx + 20h]
        mov    ax, word ptr [edx + eax*2]
        cmp    ax, 5ch
        je     SHORT loc_4_14F63
        cmp    ax, 2fh
        je     SHORT loc_4_14F63
        cmp    ax, 3ah
        jne    SHORT loc_4_14F65
loc_4_14F63:
        mov    edi, esi
loc_4_14F65:
        loop   loc_4_14F0C
loc_4_14F67:
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -10h
        push   ebx
        push   esi
        push   edi
        sub    eax, eax
        cmp    dword ptr [ebp + 0ch], 1
        jb     NEAR PTR loc_4_1503C
        cmp    dword ptr [ebp + 0ch], 98967fh
        ja     NEAR PTR loc_4_1503C
        lea    edi, [ebp]
        mov    ebx, 0ah
        mov    eax, dword ptr [ebp + 0ch]
loc_4_14F95:
        sub    edi, 2
        cdq
        div    ebx
        or     dl, 30h
        mov    word ptr [edi], dx
        test   eax, eax
        jne    SHORT loc_4_14F95
        sub    edi, 2
        mov    word ptr [edi], 7eh
        lea    ecx, [ebp]
        sub    ecx, edi
        shr    ecx, 1
        cmp    ecx, 8
        jb     SHORT loc_4_14FC3
        mov    esi, edi
        mov    edi, dword ptr [ebp + 8]
        rep movsw
        jmp    SHORT loc_4_15037
loc_4_14FC3:
        mov    esi, edi
        lea    ecx, [ebp]
        sub    ecx, esi
        shr    ecx, 1
        mov    edi, dword ptr [ebp + 8]
        sub    edi, 2
        db  08Bh, 01Dh, 090h, 001h, 000h, 000h
loc_4_14FD8:
        add    edi, 2
        mov    ax, word ptr [edi]
        cmp    ax, 20h
        je     SHORT loc_4_15020
        sub    edx, edx
loc_4_14FE6:
        cmp    ax, word ptr [edx + ebx + 4]
        jae    SHORT loc_4_14FF7
        movzx  edx, word ptr [edx + ebx]
        or     edx, edx
        jne    SHORT loc_4_14FE6
        jmp    SHORT loc_4_15010
loc_4_14FF7:
        cmp    ax, word ptr [edx + ebx + 6]
        jbe    SHORT loc_4_15009
        movzx  edx, word ptr [edx + ebx + 2]
        or     edx, edx
        jne    SHORT loc_4_14FE6
        jmp    SHORT loc_4_15010
loc_4_15009:
        bt     dword ptr [edx + ebx + 8], 0
        cmc
loc_4_15010:
        adc    ch, 1
        mov    al, cl
        add    al, ch
        cmp    al, 8
        jb     SHORT loc_4_14FD8
        ja     SHORT loc_4_15020
        add    edi, 2
loc_4_15020:
        movzx  ecx, cl
        rep movsw
        mov    ecx, dword ptr [ebp + 8]
        lea    ecx, [ecx + 10h]
        sub    ecx, edi
        shr    ecx, 1
        mov    ax, 20h
        rep stosw
loc_4_15037:
        mov    eax, 1
loc_4_1503C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_15041:
        push   ebp
        mov    ebp, esp
        add    esp, -0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, 20h
        mov    ecx, 0bh
        mov    edi, dword ptr [ebp + 8]
        rep stosw
        mov    ecx, dword ptr [ebp + 10h]
        shr    ecx, 1
        je     NEAR PTR loc_4_151E5
        mov    edi, dword ptr [ebp + 0ch]
        lea    edx, [edi + ecx*2]
loc_4_15072:
        cmp    word ptr [edi], 2eh
        je     SHORT loc_4_1507E
        cmp    word ptr [edi], 20h
        jne    SHORT loc_4_1508C
loc_4_1507E:
        or     dword ptr [ebp - 4], 1
        add    edi, 2
        loop   loc_4_15072
        jmp    NEAR PTR loc_4_151E5
loc_4_1508C:
        mov    ecx, edx
        sub    ecx, edi
        shr    ecx, 1
        je     NEAR PTR loc_4_151E5
        mov    dword ptr [ebp + 0ch], edi
        lea    edx, [edi + ecx*2]
        mov    dword ptr [ebp - 0ch], edx
        lea    edi, [edx - 2]
        mov    eax, 2eh
        std
        repne scasw
        cld
        jne    NEAR PTR loc_4_15139
        add    edi, 2
        mov    dword ptr [ebp - 0ch], edi
        lea    esi, [edi + 2]
        mov    edi, dword ptr [ebp + 8]
        lea    edi, [edi + 10h]
        mov    cx, 300h
loc_4_150C7:
        mov    dword ptr [ebp - 8], edx
loc_4_150CA:
        cmp    esi, dword ptr [ebp - 8]
        jae    SHORT loc_4_15134
        lodsw
        cmp    ax, 586h
        ja     SHORT loc_4_15119
        cmp    ax, 20h
        je     SHORT loc_4_150E3
        cmp    ax, 2eh
        jne    SHORT loc_4_150E9
loc_4_150E3:
        or     dword ptr [ebp - 4], 1
        jmp    SHORT loc_4_150CA
loc_4_150E9:
        db  00Fh, 0B6h, 098h, 0F2h, 001h, 000h, 000h
        cmp    ebx, 0ah
        jae    SHORT loc_4_15104
        cmp    ebx, 7
        jne    SHORT loc_4_15119
        mov    ax, 5fh
        or     dword ptr [ebp - 4], 2
        jmp    SHORT loc_4_15119
loc_4_15104:
        db  066h, 02Bh, 083h, 0A4h, 001h, 000h, 000h
        db  066h, 083h, 0BBh, 0A4h, 001h, 000h, 000h, 000h
        je     SHORT loc_4_15119
        or     dword ptr [ebp - 4], 4
loc_4_15119:
        stosw
        test   ax, 0ff80h
        je     SHORT loc_4_15125
        or     dword ptr [ebp - 4], 20h
loc_4_15125:
        inc    cl
        cmp    cl, ch
        jb     SHORT loc_4_150CA
        cmp    esi, dword ptr [ebp - 8]
        jae    SHORT loc_4_15134
        or     dword ptr [ebp - 4], 1
loc_4_15134:
        cmp    ch, 8
        je     SHORT loc_4_1514B
loc_4_15139:
        mov    cx, 800h
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_4_150C7
loc_4_1514B:
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 10h]
        mov    cx, 800h
        db  08Bh, 01Dh, 090h, 001h, 000h, 000h
loc_4_1515B:
        sub    edx, edx
        lodsw
loc_4_1515F:
        cmp    ax, word ptr [edx + ebx + 4]
        jae    SHORT loc_4_15170
        movzx  edx, word ptr [edx + ebx]
        or     edx, edx
        jne    SHORT loc_4_1515F
        jmp    SHORT loc_4_151AA
loc_4_15170:
        cmp    ax, word ptr [edx + ebx + 6]
        jbe    SHORT loc_4_15182
        movzx  edx, word ptr [edx + ebx + 2]
        or     edx, edx
        jne    SHORT loc_4_1515F
        jmp    SHORT loc_4_151AA
loc_4_15182:
        cmp    ax, 5fh
        je     SHORT loc_4_151B8
        sub    ax, word ptr [edx + ebx + 4]
        mov    edx, dword ptr [edx + ebx + 8]
        shr    edx, 1
        lea    edx, [edx + ebx]
        jb     SHORT loc_4_1519E
        movzx  eax, word ptr [edx + eax*2]
        jmp    SHORT loc_4_151A2
loc_4_1519E:
        movzx  eax, byte ptr [eax + edx]
loc_4_151A2:
        setae  dl
        cmp    eax, 5fh
        jne    SHORT loc_4_151B6
loc_4_151AA:
        mov    word ptr [esi - 2], 5fh
        or     dword ptr [ebp - 4], 2
        sub    dl, dl
loc_4_151B6:
        add    cl, dl
loc_4_151B8:
        inc    cl
        cmp    ch, cl
        jae    SHORT loc_4_151D0
        mov    ax, 20h
        cmp    word ptr [esi - 2], ax
        je     SHORT loc_4_151D0
        mov    word ptr [esi - 2], ax
        or     dword ptr [ebp - 4], 1
loc_4_151D0:
        cmp    esi, edi
        jb     SHORT loc_4_1515B
        cmp    ch, 3
        je     SHORT loc_4_151E5
        lea    edi, [esi + 6]
        mov    cx, 300h
        jmp    NEAR PTR loc_4_1515B
loc_4_151E5:
        mov    eax, dword ptr [ebp - 4]
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
        mov    eax, 20h
        mov    ecx, 0bh
        mov    edi, dword ptr [ebp + 8]
        rep stosw
        mov    edx, dword ptr [ebp + 10h]
        shr    edx, 1
        je     NEAR PTR loc_4_152FC
        cmp    edx, 0ch
        ja     NEAR PTR loc_4_15306
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    cx, 800h
loc_4_15221:
        lodsw
        cmp    ax, 2eh
        je     SHORT loc_4_15250
        stosw
        dec    dl
        je     SHORT loc_4_15269
        inc    cl
        cmp    cl, ch
        jb     SHORT loc_4_15221
        cmp    ch, 3
        je     SHORT loc_4_15269
        cmp    word ptr [esi], 2eh
        jne    NEAR PTR loc_4_15306
loc_4_15244:
        mov    edi, dword ptr [ebp + 8]
        lea    edi, [edi + 10h]
        mov    cx, 300h
        jmp    SHORT loc_4_15221
loc_4_15250:
        inc    dh
        dec    dl
        je     SHORT loc_4_15269
        cmp    cx, 800h
        ja     SHORT loc_4_15244
        cmp    cx, 300h
        je     SHORT loc_4_15244
        jmp    NEAR PTR loc_4_15306
loc_4_15269:
        or     dl, dl
        jne    NEAR PTR loc_4_15306
        cmp    dh, 1
        db  00Fh, 087h, 08Ch, 000h
sub_4_15278:
        add    byte ptr [eax], al
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 10h]
        mov    cx, 800h
        db  08Bh, 01Dh, 090h, 001h, 000h, 000h
loc_4_1528A:
        sub    edx, edx
        lodsw
loc_4_1528E:
        cmp    ax, word ptr [edx + ebx + 4]
        jae    SHORT loc_4_1529F
        movzx  edx, word ptr [edx + ebx]
        or     edx, edx
        jne    SHORT loc_4_1528E
        jmp    SHORT loc_4_152D3
loc_4_1529F:
        cmp    ax, word ptr [edx + ebx + 6]
        jbe    SHORT loc_4_152B1
        movzx  edx, word ptr [edx + ebx + 2]
        or     edx, edx
        jne    SHORT loc_4_1528E
        jmp    SHORT loc_4_152D3
loc_4_152B1:
        sub    ax, word ptr [edx + ebx + 4]
        mov    edx, dword ptr [edx + ebx + 8]
        shr    edx, 1
        lea    edx, [edx + ebx]
        jb     SHORT loc_4_152C7
        movzx  eax, word ptr [edx + eax*2]
        jmp    SHORT loc_4_152CB
loc_4_152C7:
        movzx  eax, byte ptr [eax + edx]
loc_4_152CB:
        setae  dl
        cmp    eax, 5fh
        jne    SHORT loc_4_152DB
loc_4_152D3:
        mov    word ptr [esi - 2], 5fh
        sub    dl, dl
loc_4_152DB:
        add    cl, dl
        inc    cl
        cmp    ch, cl
        jae    SHORT loc_4_152EA
        cmp    word ptr [esi - 2], 20h
        jne    SHORT loc_4_15306
loc_4_152EA:
        cmp    esi, edi
        jb     SHORT loc_4_1528A
        cmp    ch, 3
        je     SHORT loc_4_152FC
        lea    edi, [esi + 6]
        mov    cx, 300h
        jmp    SHORT loc_4_1528A
loc_4_152FC:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_4_15306:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        db  08Bh, 004h, 085h, 094h, 001h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        db  08Bh, 004h, 085h, 08Ch, 001h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        sub    eax, eax
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 10h], eax
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 18h]
        or     ecx, ecx
        je     NEAR PTR loc_4_1541D
loc_4_15349:
        mov    ebx, dword ptr [ebp - 8]
        sub    eax, eax
        lodsb
        bt     dword ptr [ebx], eax
        jb     SHORT loc_4_1535A
        mov    ah, al
        lodsb
        dec    ecx
        je     SHORT loc_4_153C1
loc_4_1535A:
        or     eax, eax
        je     NEAR PTR loc_4_1540D
        sub    edx, edx
loc_4_15364:
        cmp    ax, word ptr [edx + ebx + 24h]
        jae    SHORT loc_4_15376
        movzx  edx, word ptr [edx + ebx + 20h]
        or     edx, edx
        jne    SHORT loc_4_15364
        jmp    SHORT loc_4_153C1
loc_4_15376:
        cmp    ax, word ptr [edx + ebx + 26h]
        jbe    SHORT loc_4_15388
        movzx  edx, word ptr [edx + ebx + 22h]
        or     edx, edx
        jne    SHORT loc_4_15364
        jmp    SHORT loc_4_153C1
loc_4_15388:
        sub    ax, word ptr [edx + ebx + 24h]
        mov    edx, dword ptr [edx + ebx + 28h]
        shr    edx, 1
        lea    edx, [edx + ebx + 20h]
        mov    ax, word ptr [edx + eax*2]
        mov    ebx, dword ptr [ebp - 0ch]
        sub    edx, edx
loc_4_153A0:
        cmp    ax, word ptr [edx + ebx + 4]
        jae    SHORT loc_4_153B1
        movzx  edx, word ptr [edx + ebx]
        or     edx, edx
        jne    SHORT loc_4_153A0
        jmp    SHORT loc_4_153C1
loc_4_153B1:
        cmp    ax, word ptr [edx + ebx + 6]
        jbe    SHORT loc_4_153D6
        movzx  edx, word ptr [edx + ebx + 2]
        or     edx, edx
        jne    SHORT loc_4_153A0
loc_4_153C1:
        or     dword ptr [ebp - 4], 1
        mov    al, 5fh
        stosb
        inc    dword ptr [ebp - 10h]
        jecxz  loc_4_1540D
        dec    ecx
        jne    NEAR PTR loc_4_15349
        jmp    SHORT loc_4_1540D
loc_4_153D6:
        sub    ax, word ptr [edx + ebx + 4]
        mov    edx, dword ptr [edx + ebx + 8]
        shr    edx, 1
        lea    edx, [edx + ebx]
        jae    SHORT loc_4_153F8
        mov    al, byte ptr [eax + edx]
        stosb
        inc    dword ptr [ebp - 10h]
        jecxz  loc_4_1540D
        dec    ecx
        jne    NEAR PTR loc_4_15349
        jmp    SHORT loc_4_1540D
loc_4_153F8:
        mov    ax, word ptr [edx + eax*2]
        xchg   ah, al
        stosw
        add    dword ptr [ebp - 10h], 2
        jecxz  loc_4_1540D
        dec    ecx
        jne    NEAR PTR loc_4_15349
loc_4_1540D:
        jecxz  loc_4_15419
        or     eax, eax
        mov    eax, 0
        stosb
        je     SHORT loc_4_1541D
loc_4_15419:
        or     dword ptr [ebp - 4], 2
loc_4_1541D:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        db  08Bh, 004h, 085h, 094h, 001h, 000h, 000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        db  08Bh, 004h, 085h, 08Ch, 001h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        sub    eax, eax
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 10h], eax
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 18h]
        or     ecx, ecx
        je     NEAR PTR loc_4_1554D
loc_4_15464:
        mov    ebx, dword ptr [ebp - 8]
        sub    eax, eax
        lodsb
        bt     dword ptr [ebx], eax
        jb     SHORT loc_4_15475
        mov    ah, al
        lodsb
        dec    ecx
        je     SHORT loc_4_154F1
loc_4_15475:
        or     eax, eax
        je     NEAR PTR loc_4_1553D
        sub    edx, edx
loc_4_1547F:
        cmp    ax, word ptr [edx + ebx + 24h]
        jae    SHORT loc_4_15491
        movzx  edx, word ptr [edx + ebx + 20h]
        or     edx, edx
        jne    SHORT loc_4_1547F
        jmp    SHORT loc_4_154F1
loc_4_15491:
        cmp    ax, word ptr [edx + ebx + 26h]
        jbe    SHORT loc_4_154A3
        movzx  edx, word ptr [edx + ebx + 22h]
        or     edx, edx
        jne    SHORT loc_4_1547F
        jmp    SHORT loc_4_154F1
loc_4_154A3:
        sub    ax, word ptr [edx + ebx + 24h]
        mov    edx, dword ptr [edx + ebx + 28h]
        shr    edx, 1
        lea    edx, [edx + ebx + 20h]
        mov    ax, word ptr [edx + eax*2]
        cmp    eax, 586h
        ja     SHORT loc_4_154CB
        db  00Fh, 0B6h, 098h, 0F2h, 001h, 000h, 000h
        db  066h, 02Bh, 083h, 0A4h, 001h, 000h, 000h
loc_4_154CB:
        mov    ebx, dword ptr [ebp - 0ch]
        sub    edx, edx
loc_4_154D0:
        cmp    ax, word ptr [edx + ebx + 4]
        jae    SHORT loc_4_154E1
        movzx  edx, word ptr [edx + ebx]
        or     edx, edx
        jne    SHORT loc_4_154D0
        jmp    SHORT loc_4_154F1
loc_4_154E1:
        cmp    ax, word ptr [edx + ebx + 6]
        jbe    SHORT loc_4_15506
        movzx  edx, word ptr [edx + ebx + 2]
        or     edx, edx
        jne    SHORT loc_4_154D0
loc_4_154F1:
        or     dword ptr [ebp - 4], 1
        mov    al, 5fh
        stosb
        inc    dword ptr [ebp - 10h]
        jecxz  loc_4_1553D
        dec    ecx
        jne    NEAR PTR loc_4_15464
        jmp    SHORT loc_4_1553D
loc_4_15506:
        sub    ax, word ptr [edx + ebx + 4]
        mov    edx, dword ptr [edx + ebx + 8]
        shr    edx, 1
        lea    edx, [edx + ebx]
        jae    SHORT loc_4_15528
        mov    al, byte ptr [eax + edx]
        stosb
        inc    dword ptr [ebp - 10h]
        jecxz  loc_4_1553D
        dec    ecx
        jne    NEAR PTR loc_4_15464
        jmp    SHORT loc_4_1553D
loc_4_15528:
        mov    ax, word ptr [edx + eax*2]
        xchg   ah, al
        stosw
        add    dword ptr [ebp - 10h], 2
        jecxz  loc_4_1553D
        dec    ecx
        jne    NEAR PTR loc_4_15464
loc_4_1553D:
        jecxz  loc_4_15549
        or     eax, eax
        mov    eax, 0
        stosb
        je     SHORT loc_4_1554D
loc_4_15549:
        or     dword ptr [ebp - 4], 2
loc_4_1554D:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 10h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_15558:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        db  0E8h, 06Bh, 0E2h, 0F9h, 0FFh
        pop    ecx
        test   eax, eax
        jne    NEAR PTR loc_4_15670
        mov    ah, byte ptr [ebx + 8bh]
        and    ah, 40h
        je     SHORT loc_4_15580
        or     eax, 10000000h
loc_4_15580:
        push   eax
        xor    eax, eax
        push   eax
        push   eax
        mov    edx, dword ptr [ebx + 0ch]
        lea    edi, [edx + 206h]
        push   edi
        mov    edx, dword ptr [ebx + 8ch]
        add    edx, 618h
        push   edx
        push   eax
        push   eax
        push   eax
        mov    al, byte ptr [ebx + 89h]
        call   sub_4_15D54
        mov    esi, dword ptr [ebx + 14h]
        mov    edi, dword ptr [ebx + 8ch]
        add    edi, 410h
        mov    dword ptr [ebx + 2ch], edi
        test   byte ptr [ebx + 8bh], 2
        je     SHORT loc_4_155DF
        push   esi
        call   sub_4_14D65
        pop    esi
        cmp    eax, 104h
        jae    NEAR PTR loc_4_15ACF
        lea    ecx, [eax + 1]
        rep movsw
        jmp    SHORT loc_4_15605
loc_4_155DF:
        mov    edi, esi
        xor    eax, eax
        mov    ecx, 104h
        repne scasb
        jne    NEAR PTR loc_4_15ACF
        mov    ecx, edi
        sub    ecx, esi
        mov    edi, dword ptr [ebx + 2ch]
        mov    bl, byte ptr [ebx + 8bh]
sub_4_155FD:
        and    ebx, 3
        call   sub_4_14E76
loc_4_15605:
        lea    ecx, [edi - 2]
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebx + 2ch]
        sub    ecx, esi
        mov    edi, dword ptr [ebx + 0ch]
        xor    eax, eax
        stosd
        cmp    word ptr [esi], 0
        jne    SHORT loc_4_15643
        mov    al, byte ptr [ebx + 88h]
        mov    dl, 2
        cmp    al, 3ch
        je     NEAR PTR loc_4_15AD1
        cmp    al, 3dh
        je     NEAR PTR loc_4_15AD1
        cmp    al, 6ch
        je     NEAR PTR loc_4_15AD1
        mov    dl, 3
        jmp    NEAR PTR loc_4_15AD1
loc_4_15643:
        cmp    word ptr [esi + 2], 3ah
        jne    SHORT loc_4_15681
        mov    ax, word ptr [esi]
        add    esi, 4
        cmp    al, 61h
        cmc
        sbb    ah, ah
        and    ah, 20h
        add    ah, 41h
        sub    al, ah
        sub    ah, ah
        cmp    eax, 20h
        jae    SHORT loc_4_1567A
        inc    eax
        or     byte ptr [ebp - 0fh], 1
        call   sub_4_15D54
        jmp    SHORT loc_4_15681
loc_4_15670:
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_4_1567A:
        mov    dl, 0fh
        jmp    NEAR PTR loc_4_15AD1
loc_4_15681:
        xor    ebx, ebx
        mov    ax, word ptr [esi]
        cmp    eax, 2fh
        je     SHORT loc_4_15694
        cmp    eax, 5ch
        jne    NEAR PTR loc_4_15748
loc_4_15694:
        add    esi, 2
        mov    ax, word ptr [esi]
        cmp    eax, 2fh
        je     SHORT loc_4_156A8
        cmp    eax, 5ch
        jne    NEAR PTR loc_4_157A8
loc_4_156A8:
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 7ch], ebx
        mov    byte ptr [edx + 89h], 0ffh
        mov    byte ptr [ebp - 10h], 2
        and    byte ptr [ebp - 0fh], 0fdh
        mov    ecx, 3fh
        add    esi, 2
        push   0
        push   3fh
        mov    edx, edi
        mov    word ptr [edi + 2], 5ch
        add    edi, 4
loc_4_156D4:
        lodsw
        test   eax, eax
        je     SHORT loc_4_156F9
        cmp    ax, 586h
        ja     SHORT loc_4_156F2
        db  08Ah, 098h, 0F2h, 001h, 000h, 000h
        cmp    bl, 1
        je     SHORT loc_4_156FC
        db  066h, 02Bh, 083h, 0A4h, 001h, 000h, 000h
loc_4_156F2:
        jecxz  loc_4_156D4
        dec    ecx
        stosw
        jmp    SHORT loc_4_156D4
loc_4_156F9:
        sub    esi, 2
loc_4_156FC:
        mov    ecx, edi
        sub    ecx, edx
        mov    word ptr [edx], cx
        pop    ecx
        or     ecx, ecx
        je     SHORT loc_4_1570F
        mov    edx, edi
        add    edi, 2
        jmp    SHORT loc_4_156D4
loc_4_1570F:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0ch]
        push   eax
        push   ecx
        mov    ecx, edi
        sub    ecx, eax
        mov    word ptr [eax], cx
        mov    word ptr [eax + 2], cx
        call   sub_4_C586
        add    esp, 4
        pop    edx
        or     eax, eax
        je     NEAR PTR loc_4_157F0
        lea    edi, [edx + 4]
        push   esi
        mov    esi, dword ptr [eax + 8]
        mov    byte ptr [ebp - 10h], 0
        sub    eax, eax
        mov    ebx, dword ptr [ebp + 8]
        jmp    NEAR PTR loc_4_15643
loc_4_15748:
        test   byte ptr [ebp - 0fh], 1
        jne    SHORT loc_4_1578A
        db  0FFh, 015h, 004h, 06Bh, 000h, 000h
        jb     SHORT loc_4_1578A
        inc    word ptr [ebp - 22h]
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 10h], 4
        mov    dword ptr [ebx + 84h], edx
        mov    dword ptr [ebx + 7ch], 0
        mov    byte ptr [ebx + 89h], 0ffh
        pop    edx
        xor    ebx, ebx
        cmp    word ptr [esi], bx
        jne    SHORT loc_4_157EB
        sub    edi, eax
        mov    word ptr [edi], bx
        add    edi, eax
        mov    dword ptr [ebp - 18h], edx
        jmp    SHORT loc_4_157F3
loc_4_1578A:
        movzx  eax, byte ptr [ebp - 0eh]
        test   eax, eax
        je     SHORT loc_4_157EB
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    esi, dword ptr [esi + 94h]
        mov    esi, dword ptr [esi + eax*4 + 4ch]
        test   esi, esi
        lea    esi, [esi + 4]
        jne    SHORT loc_4_157A8
        pop    esi
loc_4_157A8:
        movzx  eax, byte ptr [ebp - 0eh]
        test   eax, eax
        je     SHORT loc_4_157EB
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 080h
        je     SHORT loc_4_157EB
        db  08Bh, 00Ch, 085h, 0BCh, 065h, 000h, 000h
        movzx  eax, word ptr [ecx + 0ch]
        sub    eax, 4
        je     SHORT loc_4_157EB
        push   esi
        mov    esi, dword ptr [ecx + 10h]
        or     esi, esi
        jne    SHORT loc_4_157D9
        pop    esi
        mov    dl, 0fh
        jmp    NEAR PTR loc_4_15AD1
loc_4_157D9:
        add    esi, 6
        mov    dword ptr [ebp - 28h], edi
        add    dword ptr [ebp - 28h], eax
        jmp    SHORT loc_4_157EB
loc_4_157E4:
        db  0FFh, 024h, 09Dh, 034h, 03Dh, 001h, 000h
loc_4_157EB:
        xor    eax, eax
        mov    eax, eax
loc_4_157F0:
        mov    dword ptr [ebp - 18h], esi
loc_4_157F3:
        mov    ecx, 308h
        and    byte ptr [ebp - 0dh], 0f3h
        and    byte ptr [ebp - 0fh], 0fbh
        mov    dword ptr [ebp - 14h], edi
        stosw
        mov    edx, edi
loc_4_15807:
        cmp    edi, dword ptr [ebp - 1ch]
        jb     SHORT loc_4_15817
        cmp    word ptr [esi], 0
        je     SHORT loc_4_15817
        jmp    NEAR PTR loc_4_15AAE
loc_4_15817:
        lodsw
        cmp    ax, 586h
        ja     SHORT loc_4_15840
        db  08Ah, 098h, 0F2h, 001h, 000h, 000h
        cmp    bl, 8
        jb     SHORT loc_4_157E4
        db  066h, 02Bh, 083h, 0A4h, 001h, 000h, 000h
loc_4_15831:
        test   cl, cl
        je     SHORT loc_4_15864
        dec    ecx
loc_4_15836:
        stosw
        mov    edx, edi
        or     byte ptr [ebp - 0fh], 4
        jmp    SHORT loc_4_15807
loc_4_15840:
        cmp    eax, 0fffdh
        jae    NEAR PTR loc_4_15AAE
        test   ecx, ecx
        js     SHORT loc_4_15836
        test   cl, cl
        je     SHORT loc_4_15864
        push   eax
        push   edx
        call   sub_4_14D81
        add    ah, 0ffh
        pop    edx
        pop    eax
        sbb    cl, 0
        jmp    SHORT loc_4_15831
loc_4_15864:
        cmp    dword ptr [esp], 0
        jne    SHORT loc_4_15870
        test   byte ptr [ebp - 0fh], 40h
        je     SHORT loc_4_15807
loc_4_15870:
        or     ecx, 0ffffffffh
        or     byte ptr [ebp - 0dh], 24h
        jmp    SHORT loc_4_15836
        db  0F6h, 045h, 0F1h, 040h, 075h, 01Bh, 080h, 04Dh, 0F3h, 080h, 08Ah, 0D5h, 083h, 0E1h, 00Fh, 0B0h
        db  03Fh, 0F3h, 066h, 0ABh, 08Ah, 0EAh, 08Bh, 0D7h, 080h, 04Dh, 0F1h, 004h, 0E9h, 06Dh, 0FFh, 0FFh
        db  0FFh, 080h, 04Dh, 0F3h, 0C0h, 066h, 03Bh, 006h, 075h, 08Eh, 083h, 0C6h, 002h, 0EBh, 0F6h, 066h
        db  083h, 03Eh, 02Eh, 074h, 00Ah, 066h, 083h, 03Eh, 000h, 074h, 004h, 080h, 04Dh, 0F3h, 008h, 08Dh
        db  047h, 0FEh, 02Bh, 045h, 0ECh, 00Fh, 084h, 02Dh, 002h, 000h, 000h, 0B8h, 02Eh, 000h, 000h, 000h
        db  083h, 03Ch, 024h, 000h, 075h, 019h, 0F6h, 045h, 0F1h, 040h, 074h, 047h, 066h, 039h, 047h, 0FEh
        db  074h, 00Dh, 0F6h, 045h, 0F3h, 040h, 074h, 007h, 08Dh, 057h, 002h, 080h, 04Dh, 0F3h, 008h, 03Bh
        db  0FAh, 066h, 0ABh, 077h, 00Bh, 0C1h, 0F9h, 008h, 00Fh, 085h, 010h, 0FFh, 0FFh, 0FFh, 0EBh, 017h
        db  050h, 08Bh, 045h, 008h, 0F6h, 080h, 08Bh, 000h, 000h, 000h, 008h, 058h, 075h, 015h, 0F6h, 005h
        db  00Ch, 06Bh, 000h, 000h, 001h, 075h, 00Ch, 083h, 0C9h, 0FFh, 080h, 04Dh, 0F3h, 024h, 0E9h, 0EBh
        db  0FEh, 0FFh, 0FFh, 0C1h, 0F9h, 008h, 00Fh, 084h, 089h, 001h, 000h, 000h, 08Bh, 0FAh, 066h, 0ABh
        db  0E9h, 0D9h, 0FEh, 0FFh, 0FFh, 08Dh, 047h, 0FEh, 02Bh, 045h, 0ECh, 0B8h, 020h, 000h, 000h, 000h
        db  075h, 00Ah, 0F6h, 045h, 0F1h, 040h, 00Fh, 084h, 069h, 001h, 000h, 000h, 066h, 0ABh, 066h, 083h
        db  03Eh, 020h, 074h, 035h, 066h, 083h, 03Eh, 000h, 074h, 02Fh, 0F6h, 045h, 0F1h, 040h, 074h, 029h
        db  083h, 03Ch, 024h, 000h, 075h, 017h, 050h, 08Bh, 045h, 008h, 0F6h, 080h, 08Bh, 000h, 000h, 000h
        db  008h, 058h, 075h, 015h, 0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 001h, 075h, 00Ch, 083h, 0C9h, 0FFh
        db  080h, 04Dh, 0F3h, 024h, 0E9h, 085h, 0FEh, 0FFh, 0FFh, 084h, 0C9h, 00Fh, 084h, 07Dh, 0FEh, 0FFh
        db  0FFh, 049h, 0E9h, 077h, 0FEh, 0FFh, 0FFh, 080h, 04Dh, 0F3h, 080h, 0E9h, 098h, 0FEh, 0FFh, 0FFh
        db  066h, 0B8h, 02Eh, 000h, 083h, 03Ch, 024h, 000h, 075h, 00Ah, 0F6h, 045h, 0F1h, 040h, 00Fh, 084h
        db  001h, 001h, 000h, 000h, 083h, 0C9h, 0FFh, 080h, 04Dh, 0F3h, 024h, 066h, 083h, 0F8h, 02Eh, 00Fh
        db  085h, 078h, 0FEh, 0FFh, 0FFh, 066h, 0ABh, 0E9h, 042h, 0FEh, 0FFh, 0FFh, 05Eh, 00Bh, 0F6h, 074h
        db  043h, 089h, 075h, 0E8h, 0EBh, 03Eh, 0F6h, 045h, 0F3h, 080h, 00Fh, 085h, 0F6h, 000h, 000h, 000h
        db  066h, 083h, 03Eh, 000h, 075h, 02Eh, 08Bh, 045h, 008h, 08Ah, 080h, 088h, 000h, 000h, 000h, 03Ch
        db  03Bh, 074h, 0D9h, 03Ch, 060h, 074h, 0D5h, 066h, 083h, 07Dh, 0DEh, 000h, 075h, 0CEh, 080h, 03Dh
        db  044h, 000h, 000h, 000h, 000h, 074h, 006h, 0F6h, 045h, 0F0h, 002h, 075h, 0BFh, 0B2h, 003h, 0E9h
        db  0C4h, 000h, 000h, 000h, 0F6h, 045h, 0F1h, 004h, 075h, 013h, 0F6h, 045h, 0F3h, 008h, 074h, 00Dh
        db  0F6h, 045h, 0F1h, 002h, 075h, 007h, 0B2h, 005h, 0E9h, 0ABh, 000h, 000h, 000h, 08Bh, 0FAh, 08Bh
        db  0C7h, 02Bh, 045h, 0ECh, 083h, 0F8h, 002h, 077h, 006h, 083h, 0EFh, 002h, 066h, 08Bh, 007h, 03Dh
        db  000h, 002h, 000h, 000h, 076h, 007h, 0B2h, 005h, 0E9h, 08Bh, 000h, 000h, 000h, 085h, 0F6h, 00Fh
        db  084h, 094h, 001h, 000h, 000h, 066h, 083h, 03Eh, 000h, 00Fh, 084h, 06Dh, 0FFh, 0FFh, 0FFh, 083h
        db  03Ch, 024h, 000h, 00Fh, 085h, 08Eh, 0FDh, 0FFh, 0FFh, 050h, 08Bh, 0C7h, 02Bh, 0C2h, 075h, 024h
        db  03Bh, 075h, 0E8h, 074h, 01Fh, 051h, 057h, 08Bh, 04Dh, 008h, 08Bh, 049h, 02Ch, 08Bh, 07Dh, 0E0h
        db  00Fh, 0B7h, 045h, 0DCh, 02Bh, 04Dh, 0E8h, 0F7h, 0D9h, 066h, 089h, 00Ch, 047h, 040h, 066h, 089h
        db  045h, 0DCh, 05Fh, 059h, 058h, 0E9h, 05Dh, 0FDh, 0FFh, 0FFh, 083h, 07Eh, 0FEh, 03Ah, 075h, 015h
        db  08Bh, 045h, 008h, 080h, 0B8h, 0B5h, 000h, 000h, 000h, 05Fh, 075h, 009h, 05Eh, 085h, 0F6h, 00Fh
        db  084h, 05Fh, 0FFh, 0FFh, 0FFh
loc_4_15AAE:
        test   esi, esi
        je     SHORT loc_4_15ACB
        cmp    dword ptr [esp], 0
        jne    SHORT loc_4_15ACF
loc_4_15AB8:
        lodsw
        cmp    ax, 5ch
        je     SHORT loc_4_15ACF
        cmp    ax, 2fh
        je     SHORT loc_4_15ACF
        test   ax, ax
        jne    SHORT loc_4_15AB8
loc_4_15ACB:
        mov    dl, 2
        jmp    SHORT loc_4_15AD1
loc_4_15ACF:
        mov    dl, 3
loc_4_15AD1:
        test   esi, esi
        je     SHORT loc_4_15AD8
        pop    esi
        jmp    SHORT loc_4_15AD1
loc_4_15AD8:
        mov    ebx, dword ptr [ebp + 8]
        mov    dh, 0
        mov    word ptr [ebx + 1ah], dx
        mov    dword ptr [ebx + 7ch], 0
        mov    byte ptr [ebp - 10h], 3
        jmp    NEAR PTR loc_4_15D26
        db  033h, 0D2h, 066h, 08Bh, 004h, 056h, 042h, 083h, 0F8h, 02Eh, 074h, 0F6h, 085h, 0C0h, 074h, 00Eh
        db  083h, 0F8h, 02Fh, 074h, 009h, 083h, 0F8h, 05Ch, 00Fh, 085h, 08Ah, 0FEh, 0FFh, 0FFh, 08Dh, 074h
        db  056h, 0FEh, 0F6h, 045h, 0F1h, 002h, 075h, 018h, 083h, 0EFh, 002h, 052h, 066h, 08Bh, 007h, 04Ah
        db  074h, 01Eh, 02Bh, 0F8h, 03Bh, 07Dh, 0D8h, 072h, 004h, 085h, 0C0h, 075h, 0EFh, 05Ah, 0EBh, 09Eh
        db  066h, 0B8h, 02Eh, 000h, 08Bh, 0CAh, 0F3h, 066h, 0ABh, 08Bh, 0D7h, 0E9h, 0C6h, 0FCh, 0FFh, 0FFh
        db  05Ah, 066h, 083h, 03Eh, 000h, 075h, 046h, 00Fh, 0B7h, 04Dh, 0DCh, 00Bh, 0C9h, 074h, 060h, 02Bh
        db  0CAh, 078h, 05Ch, 08Bh, 055h, 0E0h, 00Fh, 0B7h, 004h, 04Ah, 08Bh, 075h, 008h, 08Bh, 076h, 02Ch
        db  003h, 0F0h, 056h, 0FCh, 066h, 0ADh, 066h, 00Bh, 0C0h, 074h, 00Eh, 066h, 083h, 0F8h, 05Ch, 074h
        db  008h, 066h, 083h, 0F8h, 02Fh, 074h, 002h, 0EBh, 0EBh, 066h, 0C7h, 046h, 0FEh, 000h, 000h, 05Eh
        db  066h, 08Bh, 007h, 02Bh, 0F8h, 00Fh, 0B7h, 007h, 0E9h, 062h, 0FCh, 0FFh, 0FFh, 00Bh, 0C0h, 075h
        db  009h, 066h, 089h, 045h, 0DCh, 0E9h, 055h, 0FCh, 0FFh, 0FFh, 04Ah, 066h, 083h, 07Dh, 0DCh, 000h
        db  00Fh, 084h, 049h, 0FCh, 0FFh, 0FFh, 066h, 029h, 055h, 0DCh, 0E9h, 040h, 0FCh, 0FFh, 0FFh, 00Bh
        db  0C0h, 00Fh, 084h, 038h, 0FCh, 0FFh, 0FFh, 050h, 08Bh, 0CFh, 066h, 0C7h, 001h, 000h, 000h, 02Bh
        db  0C8h, 083h, 0C1h, 002h, 051h, 0E8h, 0C5h, 001h, 000h, 000h, 059h, 0C1h, 0E8h, 018h, 083h, 0E0h
        db  00Ch, 008h, 045h, 0F3h, 080h, 04Dh, 0F1h, 004h, 058h, 089h, 075h, 0E8h, 0E9h, 01Eh, 0FCh, 0FFh
        db  0FFh, 066h, 0C7h, 007h, 000h, 000h, 08Bh, 05Dh, 008h, 08Bh, 055h, 0E8h, 089h, 053h, 028h, 00Bh
        db  0C0h, 074h, 016h, 08Dh, 04Ch, 010h, 0FEh, 02Bh, 04Bh, 02Ch, 081h, 0F9h, 008h, 002h, 000h, 000h
        db  073h, 007h, 066h, 0C7h, 044h, 010h, 0FEh, 000h, 000h, 02Bh, 0F8h, 08Bh, 04Bh, 00Ch, 08Bh, 0D7h
        db  02Bh, 0D1h, 066h, 089h, 051h, 002h, 003h, 0D0h, 066h, 089h, 011h, 066h, 087h, 007h, 083h, 0FAh
        db  004h, 00Fh, 086h, 0FEh, 000h, 000h, 000h, 080h, 07Dh, 0F0h, 002h, 00Fh, 084h, 0E9h, 000h, 000h
        db  000h, 0F6h, 045h, 0F3h, 004h, 00Fh, 085h, 0DFh, 000h, 000h, 000h, 081h, 07Fh, 002h, 04Ch, 000h
        db  050h, 000h, 075h, 01Bh, 066h, 083h, 07Fh, 006h, 054h, 075h, 079h, 00Fh, 0B7h, 04Fh, 00Ah, 00Fh
        db  0B7h, 057h, 008h, 083h, 0EAh, 031h, 072h, 06Ch, 083h, 0FAh, 009h, 076h, 018h, 0EBh, 065h, 081h
        db  07Fh, 002h, 050h, 000h, 052h, 000h, 075h, 05Ch, 066h, 083h, 07Fh, 006h, 04Eh, 075h, 055h, 033h
        db  0D2h, 00Fh, 0B7h, 04Fh, 008h, 0E3h, 005h, 083h, 0F9h, 02Eh, 075h, 048h, 08Bh, 00Ch, 095h, 034h
        db  06Eh, 000h, 000h, 089h, 04Bh, 07Ch, 080h, 0C2h, 021h, 088h, 093h, 089h, 000h, 000h, 000h, 0C6h
        db  045h, 0F0h, 006h, 0E3h, 010h, 0C6h, 045h, 0F0h, 005h, 0C7h, 083h, 084h, 000h, 000h, 000h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0EBh, 075h, 083h, 0EAh, 021h, 08Bh, 00Ch, 095h, 058h, 06Eh, 000h, 000h, 0E3h
        db  013h, 0C6h, 045h, 0F0h, 007h, 089h, 04Bh, 07Ch, 0C7h, 083h, 084h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0EBh, 056h, 080h, 03Dh, 0CDh, 061h, 000h, 000h, 000h, 074h, 023h, 081h, 07Fh, 002h
        db  04Eh, 000h, 045h, 000h, 075h, 01Ah, 081h, 07Fh, 006h, 054h, 000h, 051h, 000h, 075h, 011h, 00Fh
        db  0B7h, 04Fh, 00Ah, 0E3h, 005h, 083h, 0F9h, 02Eh, 075h, 006h, 0C6h, 045h, 0F0h, 006h, 0EBh, 02Ah
        db  050h, 057h, 0E8h, 0E6h, 002h, 000h, 000h, 059h, 085h, 0C0h, 074h, 01Dh, 0C6h, 045h, 0F0h, 008h
        db  083h, 0F8h, 001h, 074h, 014h, 0C6h, 045h, 0F0h, 006h, 083h, 0F8h, 002h, 075h, 00Bh, 0C7h, 043h
        db  07Ch, 000h, 000h, 000h, 000h, 0C6h, 045h, 0F0h, 007h, 058h, 085h, 0C0h, 074h, 007h, 02Bh, 0F8h
        db  066h, 087h, 007h, 0EBh, 0F5h
loc_4_15D26:
        add    esp, 18h
        pop    eax
        and    eax, 0ff0000ffh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  0C5h, 039h, 001h, 000h, 0CFh, 039h, 001h, 000h, 093h, 03Ah, 001h, 000h, 02Eh, 039h, 001h, 000h
        db  079h, 038h, 001h, 000h, 090h, 039h, 001h, 000h, 0A8h, 038h, 001h, 000h, 09Dh, 039h, 001h, 000h
sub_4_15D54:
        and    byte ptr [ebp - 0fh], 0fdh
        movzx  ecx, al
        jecxz  loc_4_15D64
        db  08Bh, 00Ch, 08Dh, 0BCh, 065h, 000h, 000h
loc_4_15D64:
        mov    dl, 6
        jecxz  loc_4_15D6F
        mov    dl, byte ptr [ecx + 0ah]
        mov    ecx, dword ptr [ecx]
        jmp    SHORT loc_4_15D80
loc_4_15D6F:
        or     eax, eax
        je     SHORT loc_4_15D7C
        db  0F6h, 080h, 08Bh, 000h, 000h, 000h, 001h
        jne    SHORT loc_4_15D80
loc_4_15D7C:
        or     byte ptr [ebp - 0fh], 2
loc_4_15D80:
        mov    byte ptr [ebx + 89h], al
        mov    byte ptr [ebp - 0eh], al
        mov    dword ptr [ebx + 7ch], ecx
        mov    byte ptr [ebp - 10h], dl
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -0ch
        push   ebx
        push   esi
        push   edi
        sub    eax, eax
        sub    ebx, ebx
        mov    ecx, 308h
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], esi
loc_4_15DB6:
        lodsw
        cmp    ax, 586h
        ja     SHORT loc_4_15DE4
        db  08Ah, 098h, 0F2h, 001h, 000h, 000h
        cmp    bl, 8
        jb     SHORT loc_4_15DDD
        db  066h, 02Bh, 083h, 0A4h, 001h, 000h, 000h
loc_4_15DD0:
        test   cl, cl
        je     SHORT loc_4_15E08
        dec    ecx
loc_4_15DD5:
        mov    edx, esi
        or     dword ptr [ebp - 8], 4
        jmp    SHORT loc_4_15DB6
loc_4_15DDD:
        db  0FFh, 024h, 09Dh, 0ACh, 03Eh, 001h, 000h
loc_4_15DE4:
        cmp    eax, 0fffdh
        jae    NEAR PTR loc_4_15EA0
        test   ecx, ecx
        js     SHORT loc_4_15DD5
        test   cl, cl
        je     SHORT loc_4_15E08
        push   eax
        push   edx
        call   sub_4_14D81
        add    ah, 0ffh
        pop    edx
        pop    eax
        sbb    cl, 0
        jmp    SHORT loc_4_15DD0
loc_4_15E08:
        or     ecx, 0ffffffffh
        or     dword ptr [ebp - 4], 4
        jmp    SHORT loc_4_15DD5
        db  083h, 04Dh, 0FCh, 040h, 081h, 04Dh, 0FCh, 080h, 000h, 000h, 000h, 0EBh, 0B2h, 066h, 083h, 03Eh
        db  02Eh, 074h, 00Ah, 066h, 083h, 03Eh, 000h, 074h, 004h, 083h, 04Dh, 0FCh, 008h, 08Dh, 046h, 0FEh
        db  02Bh, 045h, 0F4h, 074h, 01Dh
loc_4_15E36:
        test   dword ptr [ebp - 4], 40h
        je     SHORT loc_4_15E43
        or     dword ptr [ebp - 4], 8
loc_4_15E43:
        lea    eax, [esi - 2]
        sub    eax, edx
        jg     SHORT loc_4_15E53
        sar    ecx, 8
        jne    NEAR PTR loc_4_15DB6
loc_4_15E53:
        or     ecx, 0ffffffffh
        or     dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_4_15DB6
        db  08Dh, 046h, 0FEh, 02Bh, 045h, 0F4h, 074h, 0ECh, 066h, 083h, 03Eh, 020h, 074h, 011h, 066h, 083h
        db  03Eh, 000h, 074h, 00Bh, 0F6h, 005h, 00Ch, 06Bh, 000h, 000h, 001h, 075h, 002h, 0EBh, 0D5h, 084h
        db  0C9h, 00Fh, 084h, 030h, 0FFh, 0FFh, 0FFh, 049h, 0E9h, 02Ah, 0FFh, 0FFh, 0FFh, 0F7h, 045h, 0F8h
        db  004h, 000h, 000h, 000h, 074h, 00Bh, 08Bh, 045h, 0FCh, 0C1h, 0E0h, 018h, 05Fh, 05Eh, 05Bh, 0C9h
        db  0C3h
loc_4_15EA0:
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0C0h, 08Ch, 03Eh, 001h, 000h, 0A0h, 03Eh, 001h, 000h, 0A0h, 03Eh, 001h, 000h, 05Fh, 03Eh
        db  001h, 000h, 011h, 03Eh, 001h, 000h, 015h, 03Eh, 001h, 000h, 01Eh, 03Eh, 001h, 000h, 008h, 03Eh
        db  001h, 000h
sub_4_15ECC:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    eax, eax
        push   eax
        mov    esi, dword ptr [ebp + 10h]
        cmp    word ptr [esi + 2], 3ah
        jne    SHORT loc_4_15EE2
        add    esi, 4
loc_4_15EE2:
        lodsw
        cmp    ax, 2fh
        je     SHORT loc_4_15EFE
        cmp    ax, 5ch
        je     SHORT loc_4_15EFE
        sub    esi, 2
        mov    edx, dword ptr [ebp + 0ch]
        test   edx, edx
        je     SHORT loc_4_15EFE
        push   esi
        lea    esi, [edx + 4]
loc_4_15EFE:
        mov    edi, dword ptr [ebp + 8]
        lea    ebx, [edi + 206h]
        xor    eax, eax
loc_4_15F09:
        mov    edx, edi
        nop
loc_4_15F0C:
        stosw
        cmp    edi, ebx
        jae    SHORT loc_4_15F2A
        lodsw
        cmp    ax, 2eh
        je     SHORT loc_4_15F40
        cmp    ax, 2fh
        je     SHORT loc_4_15F2B
        cmp    ax, 5ch
        je     SHORT loc_4_15F2B
        test   eax, eax
        jne    SHORT loc_4_15F0C
loc_4_15F2A:
        pop    esi
loc_4_15F2B:
        mov    eax, edi
        sub    eax, edx
        cmp    eax, 2
        ja     SHORT loc_4_15F3A
        sub    edi, 2
        mov    ax, word ptr [edi]
loc_4_15F3A:
        test   esi, esi
        jne    SHORT loc_4_15F09
        jmp    SHORT loc_4_15F84
loc_4_15F40:
        lea    eax, [edx + 2]
        sub    eax, edi
        jne    SHORT loc_4_15F68
loc_4_15F47:
        inc    eax
        cmp    word ptr [esi + eax*2 - 2], 2eh
        je     SHORT loc_4_15F47
        mov    edx, eax
        mov    ax, word ptr [esi + eax*2 - 2]
        test   eax, eax
        je     SHORT loc_4_15F6F
        cmp    eax, 2fh
        je     SHORT loc_4_15F6F
        cmp    eax, 5ch
        je     SHORT loc_4_15F6F
        lea    edx, [edi - 2]
loc_4_15F68:
        mov    eax, 2eh
        jmp    SHORT loc_4_15F0C
loc_4_15F6F:
        lea    esi, [esi + edx*2 - 2]
        sub    edi, 2
loc_4_15F76:
        mov    ax, word ptr [edi]
        dec    edx
        je     SHORT loc_4_15F09
        sub    edi, eax
        test   eax, eax
        jne    SHORT loc_4_15F76
        jmp    SHORT loc_4_15F09
loc_4_15F84:
        mov    word ptr [edi], 0
        mov    edx, edi
loc_4_15F8B:
        sub    edi, eax
        mov    ax, 5ch
        xchg   word ptr [edi], ax
        test   eax, eax
        jne    SHORT loc_4_15F8B
        xchg   edx, eax
        sub    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
sub_4_15FA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        jecxz  loc_4_15FD9
        xor    eax, eax
loc_4_15FB1:
        add    ax, word ptr [eax + edx + 4]
        loop   loc_4_15FB1
        mov    cx, word ptr [edx]
        sub    cx, ax
        mov    word ptr [eax + edx], cx
        cmp    cx, 4
        je     SHORT loc_4_15FCF
        mov    cx, word ptr [edx + 2]
        sub    cx, ax
loc_4_15FCF:
        mov    word ptr [eax + edx + 2], cx
        add    edx, eax
        mov    dword ptr [ebx + 0ch], edx
loc_4_15FD9:
        xor    eax, eax
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld
        mov    esi, dword ptr [ebp + 8]
        push   -1
        push   esi
        call   sub_4_16071
        add    esp, 8
        inc    eax
        je     SHORT loc_4_15FFC
        mov    eax, 2
        jmp    SHORT loc_4_1603A
loc_4_15FFC:
        lodsw
        lea    ecx, [eax + esi - 2]
        mov    edx, 20202020h
        push   edx
        push   edx
        push   edx
        xor    edi, edi
loc_4_1600C:
        lodsw
        cmp    ax, 2eh
        je     SHORT loc_4_1602B
        call   sub_4_14D81
loc_4_16019:
        mov    byte ptr [esp + edi], al
        inc    edi
        cmp    edi, 8
        ja     SHORT loc_4_1603E
        shr    eax, 8
        jne    SHORT loc_4_16019
        cmp    esi, ecx
        jb     SHORT loc_4_1600C
loc_4_1602B:
        pop    edx
        pop    edi
        pop    eax
        call   sub_4_16045
        jb     SHORT loc_4_1603A
        mov    eax, 1
loc_4_1603A:
        pop    edi
        pop    esi
        leave
        ret
loc_4_1603E:
        add    esp, 0ch
        xor    eax, eax
        jmp    SHORT loc_4_1603A
sub_4_16045:
        db  0A1h, 038h, 059h, 000h, 000h
loc_4_1604A:
        test   byte ptr [eax + 5], 80h
        je     SHORT loc_4_16055
        cmp    edx, dword ptr [eax + 0ah]
        je     SHORT loc_4_1606B
loc_4_16055:
        movzx  ecx, word ptr [eax]
        movzx  eax, word ptr [eax + 2]
        shl    eax, 4
        add    eax, ecx
        cmp    cx, -1
        jne    SHORT loc_4_1604A
        xor    eax, eax
        stc
        ret
loc_4_1606B:
        cmp    edi, dword ptr [eax + 0eh]
        jne    SHORT loc_4_16055
        ret
sub_4_16071:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        db  0BAh, 074h, 057h, 000h, 000h
        db  00Fh, 0B6h, 00Dh, 02Bh, 059h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_4_160C6
        mov    eax, dword ptr [ebp + 0ch]
        inc    eax
        je     SHORT loc_4_16096
        xor    ah, ah
        cmp    eax, ecx
        jae    SHORT loc_4_160C6
loc_4_16096:
        push   eax
        mov    ah, 14h
        mul    ah
        lea    edx, [eax + edx]
        pop    eax
        sub    ecx, eax
loc_4_160A1:
        mov    ebx, dword ptr [edx + 10h]
loc_4_160A4:
        mov    edi, dword ptr [ebx]
        or     edi, edi
        je     SHORT loc_4_160C1
        push   esi
loc_4_160AB:
        lodsw
        cmp    ax, word ptr [edi]
        jne    SHORT loc_4_160BB
        or     ax, ax
        je     SHORT loc_4_160CE
        inc    edi
        inc    edi
        jmp    SHORT loc_4_160AB
loc_4_160BB:
        pop    esi
        add    ebx, 4
        jmp    SHORT loc_4_160A4
loc_4_160C1:
        add    edx, 14h
        loop   loc_4_160A1
loc_4_160C6:
        xor    eax, eax
        dec    eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_4_160CE:
        pop    esi
        mov    eax, dword ptr [edx + 10h]
        sub    ebx, eax
        shr    ebx, 2
        db  02Ah, 00Dh, 02Bh, 059h, 000h, 000h
        not    cl
        inc    cl
        mov    ch, bl
        mov    eax, ecx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_4_160F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        test   byte ptr [ebx + 3], 80h
        je     NEAR PTR loc_4_161C6
        mov    eax, dword ptr [ebx + 0ch]
        mov    edi, dword ptr [ebp + 0ch]
        movzx  ecx, word ptr [eax + 2]
        lea    esi, [eax + ecx]
        movzx  eax, word ptr [esi]
        add    esi, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 28h]
        cmp    dword ptr [ebp + 10h], 0
        mov    dword ptr [eax], 2e002ah
        mov    dword ptr [eax + 4], 2ah
        jne    NEAR PTR loc_4_161D4
        push   201h
        push   208h
        db  0E8h, 0A8h, 024h, 000h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    SHORT loc_4_1616A
        mov    word ptr [ebx + 1ah], 4
        mov    eax, 4
        jmp    NEAR PTR loc_4_1621C
loc_4_1616A:
        mov    edi, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebx + 28h]
        movsd
        movsd
        mov    al, byte ptr [ebx + 8bh]
        mov    byte ptr [ebp - 1], al
        and    al, 0bfh
        mov    byte ptr [ebx + 8bh], al
        push   8
        push   dword ptr [ebp - 8]
        push   ebx
        call   sub_4_A09F
        add    esp, 0ch
        mov    esi, eax
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 8bh], al
        push   0
        db  0FFh, 075h
sub_4_161A0:
        clc
        db  0E8h, 05Ah, 024h, 000h, 000h
        add    esp, 8
        mov    ax, word ptr [ebx + 1ah]
        or     ax, ax
        je     SHORT loc_4_161B7
        movsx  eax, ax
        jmp    SHORT loc_4_1621C
loc_4_161B7:
        mov    eax, dword ptr [ebx]
        xor    eax, esi
        and    eax, 0ffffffh
        xor    eax, esi
        mov    dword ptr [ebx], eax
        jmp    SHORT loc_4_161CE
loc_4_161C6:
        mov    edi, dword ptr [ebp + 0ch]
        mov    word ptr [edi], 0
loc_4_161CE:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_4_16214
loc_4_161D4:
        push   1
        push   102h
        push   208h
        push   dword ptr [ebx + 2ch]
        db  0FFh, 035h, 0C4h, 061h, 000h, 000h
        call   sub_4_14A00
        add    esp, 14h
        cmp    eax, 102h
        jb     SHORT loc_4_1620A
        mov    ax, word ptr [ebx + 1ah]
        sub    ax, 5
        cmp    ax, 1
        sbb    eax, eax
        neg    eax
        jmp    SHORT loc_4_1621C
loc_4_1620A:
        db  08Bh, 00Dh, 0C4h, 061h, 000h, 000h
        mov    byte ptr [eax + ecx], 0
loc_4_16214:
        and    dword ptr [ebx], 0efffffffh
        sub    eax, eax
loc_4_1621C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_16223:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 0C4h, 061h, 000h, 000h
        db  0E8h, 00Ch, 024h, 000h, 000h
        db  08Bh, 03Dh, 0BCh, 055h, 000h, 000h
        add    edi, eax
        add    esi, 104h
        mov    eax, dword ptr [edi]
        test   eax, 80000000h
        je     SHORT loc_4_16276
        and    eax, 7fffffffh
        mov    dword ptr [edi], eax
        db  0FFh, 00Dh, 0A0h, 072h, 000h, 000h
        db  083h, 03Dh, 0A0h, 072h, 000h, 000h, 001h
        jne    SHORT loc_4_1626E
        db  068h, 0A0h, 072h, 000h, 000h
        db  0E8h, 086h, 0CCh, 0F9h, 0FFh
        add    esp, 4
loc_4_1626E:
        or     dword ptr [edi], 20000000h
        jmp    SHORT loc_4_1627D
loc_4_16276:
        or     eax, 20000000h
        mov    dword ptr [edi], eax
loc_4_1627D:
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
loc_4_16283:
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_4_298D
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_162AF
        mov    dword ptr [ebp + 14h], 4fh
        push   dword ptr [ebp + 10h]
        lea    eax, [edi + 4]
        push   eax
        push   esi
        push   ebx
        call   sub_4_162C5
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_16283
loc_4_162AF:
        add    edi, 20h
        mov    ecx, 0ah
        rep movsd
        movsw
        movsb
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_162C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
        sub    eax, eax
        mov    esi, dword ptr [ebp + 14h]
        mov    edi, esi
        mov    ecx, 94h
        rep stosd
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 1eh
        lea    ebx, [esi + 2ch]
sub_4_162EC:
        push   1
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   edx
loc_4_162FE:
        push   ebx
        call   sub_4_14A69
        add    esp, 10h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [esi + 234h], 0
        test   byte ptr [ecx + 8bh], 8
        jne    SHORT loc_4_1632A
        mov    dword ptr [ebp - 4], 3
        mov    edi, dword ptr [ebp + 10h]
        mov    eax, edi
        jmp    SHORT loc_4_1635A
loc_4_1632A:
        push   eax
        push   ebx
        lea    eax, [ebp - 34h]
        push   eax
        call   sub_4_14B23
        add    esp, 0ch
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        call   sub_4_14D65
        add    esp, 4
        add    eax, eax
        push   eax
        push   edi
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_4_14B23
        add    esp, 0ch
        lea    ebx, [ebp - 34h]
        lea    eax, [ebp - 1ch]
loc_4_1635A:
        or     edi, edi
        je     SHORT loc_4_1637A
        cmp    word ptr [edi], 0
        je     SHORT loc_4_1637A
        push   dword ptr [ebp - 4]
        push   ebx
        push   eax
        call   sub_4_14B7B
        add    esp, 0ch
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_4_163B4
loc_4_1637A:
        mov    eax, dword ptr [ebp + 0ch]
        movzx  eax, byte ptr [eax + 15h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [eax + 16h]
        call   sub_4_642D
        add    esp, 4
        mov    dword ptr [esi + 14h], eax
        mov    dword ptr [esi + 18h], edx
        lea    ecx, [esi + 14h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ah]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0dh]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, 1
loc_4_163B4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_4_163BB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   201h
        push   4bh
        db  0E8h, 02Ch, 022h, 000h, 000h
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_4_163E0
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [edi + 1ah], 4
        jmp    SHORT loc_4_16421
loc_4_163E0:
        push   1
        lea    eax, [esi + 4]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_4_160F0
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_16416
        push   4eh
        push   dword ptr [edi + 14h]
        push   esi
        push   edi
        call   sub_4_16223
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_16425
        cmp    word ptr [edi + 1ah], 12h
        jne    SHORT loc_4_16416
        mov    word ptr [edi + 1ah], 2
loc_4_16416:
        push   0
        push   esi
        db  0E8h, 0E2h, 021h, 000h, 000h
        add    esp, 8
loc_4_16421:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_4_16425:
        mov    eax, dword ptr [edi + 10h]
        db  0C7h, 000h, 041h, 044h, 001h, 000h
        db  0C7h, 040h, 004h, 06Dh, 041h, 000h, 000h
        db  0C7h, 040h, 008h, 0E8h, 072h, 000h, 000h
        mov    dword ptr [edi + 20h], esi
        jmp    SHORT loc_4_16421
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edx + 20h]
        test   byte ptr [edx + 18h], 40h
        je     SHORT loc_4_16459
        mov    eax, dword ptr [edx + 24h]
        mov    dword ptr [ebx + 2dh], eax
loc_4_16459:
        lea    esi, [ebx + 20h]
        db  08Bh, 03Dh, 0C4h, 061h, 000h, 000h
        add    edi, 104h
        db  0B9h, 00Ah, 000h
IFSMgr_GetTimeZoneBias:
        add    byte ptr [eax], al
        rep movsd
        movsw
        movsb
        push   4fh
        push   dword ptr [edx + 14h]
        push   ebx
        push   edx
        call   sub_4_16223
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        db  083h, 03Dh, 074h, 059h, 000h, 000h, 000h
        jne    SHORT loc_4_164A5
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   dword ptr [esi + 20h]
        db  0E8h, 060h, 021h, 000h, 000h
        add    esp, 8
        jmp    SHORT loc_4_164A8
loc_4_164A5:
        mov    esi, dword ptr [ebp + 8]
loc_4_164A8:
        mov    word ptr [esi + 1ah], 0
        sub    eax, eax
        pop    esi
        pop    ebp
        ret
sub_4_164B3:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        lea    ebx, [edi + 20h]
        mov    al, byte ptr [ebp + 14h]
        mov    byte ptr [esi + 4], al
        mov    dword ptr [esi + 14h], ebx
loc_4_164CB:
        push   0
        push   esi
        mov    eax, dword ptr [esi + 7ch]
        push   26h
        mov    eax, dword ptr [eax + 0ch]
        push   dword ptr [eax + 20h]
        db  0E8h, 0B2h, 0D6h, 0F9h, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_164FE
        mov    byte ptr [esi + 4], 1
        push   dword ptr [ebp + 10h]
        lea    eax, [edi + 4]
        push   eax
        push   ebx
        push   esi
        call   sub_4_162C5
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_164CB
loc_4_164FE:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_4_16507:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   201h
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebp - 4], eax
        push   4bh
        db  0E8h, 0D3h, 020h, 000h, 000h
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_4_16539
        mov    word ptr [esi + 1ah], 4
        jmp    NEAR PTR loc_4_165D5
loc_4_16539:
        mov    eax, dword ptr [esi]
        mov    ecx, eax
        and    ecx, 0ffffffh
        cmp    ecx, 1016h
        je     SHORT loc_4_16552
        and    eax, 0ff0000ffh
        mov    dword ptr [esi], eax
loc_4_16552:
        lea    eax, [ebx + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    edi, eax
        add    edi, 0dh
        sub    eax, eax
        stosd
        stosd
loc_4_16561:
        push   eax
        lea    eax, [ebx + 4]
        push   eax
        push   esi
        call   sub_4_160F0
        add    esp, 0ch
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT sub_4_165AF
        push   0
        push   dword ptr [ebp - 4]
        push   ebx
        push   esi
        call   sub_4_164B3
        add    esp, 10h
        or     eax, eax
        jne    SHORT sub_4_165AF
        mov    eax, dword ptr [esi + 10h]
        db  0C7h, 000h, 0DCh, 045h, 001h, 000h
        db  0C7h, 040h, 004h, 06Dh, 041h, 000h, 000h
        db  0C7h, 040h, 008h, 0E8h, 072h, 000h, 000h
        mov    eax, dword ptr [esi + 7ch]
        mov    dword ptr [esi + 20h], ebx
        mov    cl, byte ptr [eax + 2]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 14h], cl
sub_4_165AF:
        mov    ax, word ptr [esi + 1ah]
        or     ax, ax
        je     SHORT loc_4_165CF
        cmp    ax, 12h
        jne    SHORT loc_4_165C4
        mov    word ptr [esi + 1ah], 2
loc_4_165C4:
        push   0
        push   ebx
        db  0E8h, 034h, 020h, 000h, 000h
        add    esp, 8
loc_4_165CF:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
loc_4_165D5:
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
        mov    ecx, dword ptr [esi + 20h]
sub_4_165E8:
        mov    ebx, dword ptr [esi + 14h]
        lea    edi, [ecx + 20h]
        mov    edx, dword ptr [esi + 7ch]
        mov    al, byte ptr [edi + 14h]
        cmp    byte ptr [edx + 2], al
        je     SHORT loc_4_16606
        mov    word ptr [esi + 1ah], 12h
        db  0B8h
sub_4_16600:
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        jmp    SHORT loc_4_16631
loc_4_16606:
        mov    byte ptr [esi + 4], 1
        mov    dword ptr [esi + 14h], edi
        push   1
        push   ebx
        push   ecx
        push   esi
        call   sub_4_164B3
        add    esp, 10h
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_4_1662A
        mov    eax, dword ptr [esi + 7ch]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [edi + 14h], al
loc_4_1662A:
        mov    dword ptr [esi + 14h], ebx
        movsx  eax, word ptr [esi + 1ah]
loc_4_16631:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        db  08Bh, 045h
sub_4_16640:
        adc    byte ptr [0f0000000h], ah
        cmp    eax, 10000000h
_PTEXT ends
        end
